/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. ("MediaTek") and/or its licensors.
 * Without the prior written permission of MediaTek and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) MediaTek Software
 * if you have agreed to and been bound by the applicable license agreement with
 * MediaTek ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
 * please cease any access or use of MediaTek Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT MEDIATEK SOFTWARE RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

#include "hal_aes.h"

#ifdef HAL_AES_MODULE_ENABLED

#include <string.h>
#include "memory_attribute.h"
#include "hal_define.h"
#include "mt2625.h"
#include "crypt_aes.h"
#include "hal_log.h"
#include "hal_crypt_internal.h"
#include "hal_nvic.h"
#include "hal_clock.h"
#include "hal_platform.h"
#include "hal_sleep_manager.h"

/* global lock used to protect the crypto engine */
int8_t g_crypt_lock = CRYPT_UNLOCK;
static uint8_t aes_sleep_handler = 0;

/* below variables will be used for crypto hardware so must be placed in physical memory address*/
ATTR_RWDATA_IN_NONCACHED_RAM_4BYTE_ALIGN uint8_t *init_vector2;
ATTR_RWDATA_IN_NONCACHED_RAM_4BYTE_ALIGN uint8_t data_block[HAL_AES_BLOCK_SIZES];

/* internal function for getting lock, -100 means the crypto engine is not available */
int32_t hal_crypt_lock_take(void)
{
    uint32_t irq_status;
    irq_status = save_and_set_interrupt_mask();
    if (g_crypt_lock == CRYPT_LOCK) {
        restore_interrupt_mask(irq_status);
        return -100;
    }
    g_crypt_lock = CRYPT_LOCK;
    restore_interrupt_mask(irq_status);
    return 0;
}


/* internal function for releasing lock */
void hal_crypt_lock_give(void)
{
    g_crypt_lock = CRYPT_UNLOCK;
}

volatile static bool g_aes_op_done = false;

/* It's about 10s at 192MHz CPU clock */
#define HAL_AES_MAX_WAIT_COUNT 0x10000000

void dump_crypto_register(void)
{
     log_hal_info("\r\n0xe000e100 =%08x", *((volatile uint32_t *) 0xE000E100));
     log_hal_info("\r\n0xe000e200 =%08x", *((volatile uint32_t *) 0xE000E200));
     log_hal_info("\r\n0xe000e300 =%08x", *((volatile uint32_t *) 0xE000E300));
}

static void aes_operation_done(hal_nvic_irq_t irq_number)
{
    uint32_t irq_status;

    hal_nvic_disable_irq(CRYPTO_IRQn);
    irq_status = save_and_set_interrupt_mask();

    g_aes_op_done = true;

    // clear interrupt status bit
    *CRYPTO_ENGINE_STA_BASE |= (1 << CRYPTO_ENGINE_STA_INT_CLR);

    restore_interrupt_mask(irq_status);
    hal_nvic_enable_irq(CRYPTO_IRQn);

#ifdef HAL_SLEEP_MANAGER_ENABLED
    if (hal_sleep_manager_release_sleeplock(aes_sleep_handler, HAL_SLEEP_LOCK_ALL) == HAL_SLEEP_MANAGER_OK){
        log_hal_info("aes op-done release sleep lock OK.");
    }else{
        log_hal_error("aes op-done release sleep lock Failed.");
    }
#endif
}

static hal_aes_status_t do_aes_encrypt(uint8_t *encrypt_buffer,
                                       uint32_t encrypt_buffer_length,
                                       uint8_t *plain_buffer,
                                       uint32_t plain_buffer_length,
                                       uint8_t init_vector[HAL_AES_CBC_IV_LENGTH])
{
    g_aes_op_done = false;

#ifdef HAL_SLEEP_MANAGER_ENABLED
    if (aes_sleep_handler == 0) {
        aes_sleep_handler = hal_sleep_manager_set_sleep_handle("SEC_AES");
    }
    if (hal_sleep_manager_acquire_sleeplock(aes_sleep_handler, HAL_SLEEP_LOCK_ALL) == HAL_SLEEP_MANAGER_OK){
        log_hal_info("aes encrypt acquire sleep lock OK.");
    }else{
        log_hal_error("aes encrypt acquire sleep lock Failed.");
    }
#endif
    int32_t ret_val = aes_operate(encrypt_buffer,
                                  encrypt_buffer_length,
                                  plain_buffer,
                                  plain_buffer_length,
                                  init_vector,
                                  AES_MODE_ENCRYPT);
    if (ret_val < 0) {
#ifdef HAL_SLEEP_MANAGER_ENABLED
        if (hal_sleep_manager_release_sleeplock(aes_sleep_handler, HAL_SLEEP_LOCK_ALL) == HAL_SLEEP_MANAGER_OK){
            log_hal_info("aes encrypt release sleep lock OK.");
        }else{
            log_hal_error("aes encrypt release sleep lock Failed.");
        }
#endif
        log_hal_error("aes_operate fail.");
        return HAL_AES_STATUS_ERROR;
    }

#if 0
    uint32_t wait_count = 0;
    while (!g_aes_op_done) {
        //simple wait
        wait_count++;

        if(wait_count % 3000) {
//            dump_crypto_register();
        }

        if (wait_count > HAL_AES_MAX_WAIT_COUNT) {
            log_hal_error("wait for encrypt timeout.");
            return HAL_AES_STATUS_ERROR;
        }
    }

    // clear interrupt status bit
//    *CRYPTO_ENGINE_STA_BASE |= (1 << CRYPTO_ENGINE_STA_INT_CLR);
#else
    while((*CRYPTO_ENGINE_CTRL_BASE) & (1 << ENGINE_CTRL_START_OFFSET));

    // clear interrupt status bit
    *CRYPTO_ENGINE_STA_BASE |= (1 << CRYPTO_ENGINE_STA_INT_CLR);
#endif
    return HAL_AES_STATUS_OK;
}

static hal_aes_status_t do_aes_decrypt(hal_aes_buffer_t *plain_text,
                                       hal_aes_buffer_t *encrypted_text,
                                       uint8_t init_vector[HAL_AES_CBC_IV_LENGTH])
{
    g_aes_op_done = false;
#ifdef HAL_SLEEP_MANAGER_ENABLED
    if (aes_sleep_handler == 0) {
        aes_sleep_handler = hal_sleep_manager_set_sleep_handle("SEC_AES");
    }
    if (hal_sleep_manager_acquire_sleeplock(aes_sleep_handler, HAL_SLEEP_LOCK_ALL) == HAL_SLEEP_MANAGER_OK){
        log_hal_info("aes decrypt acquire sleep lock OK.");
    }else{
        log_hal_error("aes decrypt acquire sleep lock Failed.");
    }
#endif
    int32_t ret_val = aes_operate(encrypted_text->buffer,
                                  encrypted_text->length,
                                  plain_text->buffer,
                                  plain_text->length,
                                  init_vector,
                                  AES_MODE_DECRYPT);
    if (ret_val < 0) {
#ifdef HAL_SLEEP_MANAGER_ENABLED
        if (hal_sleep_manager_release_sleeplock(aes_sleep_handler, HAL_SLEEP_LOCK_ALL) == HAL_SLEEP_MANAGER_OK){
            log_hal_info("aes decrypt release sleep lock OK.");
        }else{
            log_hal_error("aes decrypt release sleep lock Failed.");
        }
#endif
        log_hal_error("aes_operate fail.");
        return HAL_AES_STATUS_ERROR;
    }

#if 0
    uint32_t wait_count = 0;
    while (!g_aes_op_done) {
        //simple wait
        wait_count++;
        if (wait_count > HAL_AES_MAX_WAIT_COUNT) {
            log_hal_error("wait for decrypt timeout.");
            return HAL_AES_STATUS_ERROR;
        }
    }

    // clear interrupt status bit
//    *CRYPTO_ENGINE_STA_BASE |= (1 << CRYPTO_ENGINE_STA_INT_CLR);
#else
    while((*CRYPTO_ENGINE_CTRL_BASE) & (1 << ENGINE_CTRL_START_OFFSET));

    // clear interrupt status bit
    *CRYPTO_ENGINE_STA_BASE |= (1 << CRYPTO_ENGINE_STA_INT_CLR);
#endif
    return HAL_AES_STATUS_OK;
}

static void aes_key_map(hal_aes_key_t key_index, uint8_t *key_source, uint8_t *key_slot)
{
    if (key_source == NULL || key_slot == NULL) {
        return;
    }

    switch (key_index) {
        case HAL_AES_EFUSE_KEY1:
            *key_slot = AES_KEY_SOURCE_EFUSE_0 + 1; //IOT_CRYPTO_KEY_BANK_KEK;
            *key_source = AES_KEY_SOURCE_EFUSE_0;
            break;
        case HAL_AES_EFUSE_KEY2:
            *key_slot = AES_KEY_SOURCE_EFUSE_1 + 1; //IOT_CRYPTO_KEY_BANK_KEK;
            *key_source = AES_KEY_SOURCE_EFUSE_1;
            break;
        case HAL_AES_HW_CKDF_KEY1:
            *key_slot = 0;
            *key_source = AES_KEY_SOURCE_HW_CKDF_0;
            break;
        case HAL_AES_HW_CKDF_KEY2:
            *key_slot = 0;
            *key_source = AES_KEY_SOURCE_HW_CKDF_1;
            break;
    }
}

/* internal common function */
hal_aes_status_t hal_aes_encrypt_with_padding(hal_aes_buffer_t *encrypted_text,
                                     hal_aes_buffer_t *plain_text,
                                     hal_aes_buffer_t *key,
                                     uint8_t init_vector[HAL_AES_CBC_IV_LENGTH],
                                     uint8_t mode,
                                     uint8_t key_source,
                                     uint8_t key_slot)
{
    if ((NULL == encrypted_text)
            || (NULL == plain_text)
            || (NULL == init_vector)) {
        log_hal_error("NULL input.");
        return HAL_AES_STATUS_ERROR;
    }

    if (AES_KEY_SOURCE_SW == key_source && NULL == key) {
        log_hal_error("NULL input.");
        return HAL_AES_STATUS_ERROR;
    }

    uint32_t last_block_size = plain_text->length % HAL_AES_BLOCK_SIZES;
    uint32_t block_num = plain_text->length / HAL_AES_BLOCK_SIZES;
    uint8_t padding_size = HAL_AES_BLOCK_SIZES - last_block_size;
    uint8_t *iv;

    if (encrypted_text->length < (plain_text->length + padding_size)) {
        log_hal_error("Inadequate encrypted buffer.");
        return HAL_AES_STATUS_ERROR;
    }

    hal_crypt_lock_take();

    hal_nvic_register_isr_handler(CRYPTO_IRQn, aes_operation_done);
    hal_nvic_enable_irq(CRYPTO_IRQn);

    aes_set_key((uint8_t)key->length, key->buffer);

    if (aes_configure(key_source, mode)) {
        hal_crypt_lock_give();
        return HAL_AES_STATUS_ERROR;
    }

    if (AES_KEY_SOURCE_EFUSE_0 == key_source || AES_KEY_SOURCE_EFUSE_1 == key_source) {
        aes_set_key_slot(key_slot);
    }

    if (block_num > 0) {
        uint32_t first_encypt_size = block_num * HAL_AES_BLOCK_SIZES;
        if (HAL_AES_STATUS_OK != do_aes_encrypt(encrypted_text->buffer,
                                                encrypted_text->length,
                                                plain_text->buffer,
                                                first_encypt_size,
                                                init_vector)) {
            log_hal_error("do_aes_encrypt fail.");
            hal_crypt_lock_give();
            return HAL_AES_STATUS_ERROR;
        }

        memset(data_block, 0x00, HAL_AES_BLOCK_SIZES);
        memcpy(data_block, plain_text->buffer + first_encypt_size, last_block_size);
        memset(data_block + last_block_size, padding_size, padding_size);

        if (AES_TYPE_CBC == mode) { /* do 2nd aes cbc operation need to input newer iv */
            init_vector2 = encrypted_text->buffer + first_encypt_size - HAL_AES_BLOCK_SIZES;
            iv = init_vector2;
        } else {
            iv = init_vector;
        }

        if (HAL_AES_STATUS_OK != do_aes_encrypt(encrypted_text->buffer + first_encypt_size,
                                                encrypted_text->length,
                                                data_block,
                                                HAL_AES_BLOCK_SIZES,
                                                iv)) {
            log_hal_error("do_aes_encrypt fail.");
            hal_crypt_lock_give();
            return HAL_AES_STATUS_ERROR;
        }
    } else {
        memset(data_block, 0x00, HAL_AES_BLOCK_SIZES);
        memcpy(data_block, plain_text->buffer, plain_text->length);
        memset(data_block + last_block_size, padding_size, padding_size);
        if (HAL_AES_STATUS_OK != do_aes_encrypt(encrypted_text->buffer,
                                                encrypted_text->length,
                                                data_block,
                                                HAL_AES_BLOCK_SIZES,
                                                init_vector)) {
            log_hal_error("do_aes_encrypt fail.");
            hal_crypt_lock_give();
            return HAL_AES_STATUS_ERROR;
        }
    }

    encrypted_text->length = (block_num + 1) * HAL_AES_BLOCK_SIZES;

    hal_crypt_lock_give();

    return HAL_AES_STATUS_OK;
}

hal_aes_status_t hal_aes_decrypt(hal_aes_buffer_t *plain_text,
                                 hal_aes_buffer_t *encrypted_text,
                                 hal_aes_buffer_t *key,
                                 uint8_t init_vector[HAL_AES_CBC_IV_LENGTH],
                                 uint8_t mode,
                                 uint8_t key_source,
                                 uint8_t key_slot)
{
    uint8_t padding_size;

    if ((NULL == plain_text)
            || (NULL == encrypted_text)
            || (NULL == init_vector)) {
        log_hal_error("NULL input.");
        return HAL_AES_STATUS_ERROR;
    }

    if (AES_KEY_SOURCE_SW == key_source && NULL == key) {
        log_hal_error("NULL input.");
        return HAL_AES_STATUS_ERROR;
    }

    if ((encrypted_text->length % HAL_AES_BLOCK_SIZES) != 0) {
        log_hal_error("Invalid encrypted text length: %lu.", encrypted_text->length);
        return HAL_AES_STATUS_ERROR;
    }
    if (plain_text->length < (encrypted_text->length - HAL_AES_BLOCK_SIZES)) {
        log_hal_error("Plain text buffer lengthL %lu is too small, encrypted length is: %lu",
                      encrypted_text->length, encrypted_text->length);
        return HAL_AES_STATUS_ERROR;
    }
    if (AES_KEY_SOURCE_SW == key_source
            && (key->length != HAL_AES_KEY_LENGTH_128)
            && (key->length != HAL_AES_KEY_LENGTH_192)
            && (key->length != HAL_AES_KEY_LENGTH_256)) {
        log_hal_error("key length is %lu, invalid. It has to be 16, 24 or 32.", key->length);
        return HAL_AES_STATUS_ERROR;
    }

    hal_crypt_lock_take();

    hal_nvic_register_isr_handler(CRYPTO_IRQn, aes_operation_done);
    hal_nvic_enable_irq(CRYPTO_IRQn);

    aes_set_key(key->length, key->buffer);

    if (aes_configure(key_source, mode)) {
        hal_crypt_lock_give();
        return HAL_AES_STATUS_ERROR;
    }

    if (AES_KEY_SOURCE_EFUSE_0 == key_source || AES_KEY_SOURCE_EFUSE_1 == key_source) {
        aes_set_key_slot(key_slot);
    }

    if (HAL_AES_STATUS_OK != do_aes_decrypt(plain_text, encrypted_text, init_vector)) {
        log_hal_error("do_aes_decrypt fail");
        hal_crypt_lock_give();
        return HAL_AES_STATUS_ERROR;
    }

    padding_size = plain_text->buffer[encrypted_text->length - 1];
    plain_text->length = encrypted_text->length - padding_size;

    hal_crypt_lock_give();

    return HAL_AES_STATUS_OK;
}



hal_aes_status_t hal_aes_cbc_encrypt(hal_aes_buffer_t *encrypted_text,
                                     hal_aes_buffer_t *plain_text,
                                     hal_aes_buffer_t *key,
                                     uint8_t init_vector[HAL_AES_CBC_IV_LENGTH])
{
    hal_aes_status_t status; 

    hal_clock_enable(HAL_CLOCK_CG_CRYPTO);
    status = hal_aes_encrypt_with_padding(encrypted_text, plain_text, key, init_vector, AES_TYPE_CBC, AES_KEY_SOURCE_SW, 0);
    hal_clock_disable(HAL_CLOCK_CG_CRYPTO);

    return status;
}

hal_aes_status_t hal_aes_cbc_decrypt(hal_aes_buffer_t *plain_text,
                                     hal_aes_buffer_t *encrypted_text,
                                     hal_aes_buffer_t *key,
                                     uint8_t init_vector[HAL_AES_CBC_IV_LENGTH])
{
    hal_aes_status_t status; 

    hal_clock_enable(HAL_CLOCK_CG_CRYPTO);
    status = hal_aes_decrypt(plain_text, encrypted_text, key, init_vector, AES_TYPE_CBC, AES_KEY_SOURCE_SW, 0);
    hal_clock_disable(HAL_CLOCK_CG_CRYPTO);
    return status;
}

hal_aes_status_t hal_aes_ecb_encrypt(hal_aes_buffer_t *encrypted_text,
                                     hal_aes_buffer_t *plain_text,
                                     hal_aes_buffer_t *key)
{
    uint8_t init_vector[16] = {0};
    hal_aes_status_t status; 

    hal_clock_enable(HAL_CLOCK_CG_CRYPTO);
    status = hal_aes_encrypt_with_padding(encrypted_text, plain_text, key, init_vector, AES_TYPE_ECB, AES_KEY_SOURCE_SW, 0);
    hal_clock_disable(HAL_CLOCK_CG_CRYPTO);

    return status;
}

hal_aes_status_t hal_aes_ecb_decrypt(hal_aes_buffer_t *plain_text,
                                     hal_aes_buffer_t *encrypted_text,
                                     hal_aes_buffer_t *key)
{
    uint8_t init_vector[HAL_AES_CBC_IV_LENGTH] = {0};
    hal_aes_status_t status; 

    hal_clock_enable(HAL_CLOCK_CG_CRYPTO);
    status = hal_aes_decrypt(plain_text, encrypted_text, key, init_vector, AES_TYPE_ECB, AES_KEY_SOURCE_SW, 0);
    hal_clock_disable(HAL_CLOCK_CG_CRYPTO);

    return status;
}

hal_aes_status_t hal_aes_cbc_encrypt_ex(hal_aes_buffer_t *encrypted_text,
                                        hal_aes_buffer_t *plain_text,
                                        hal_aes_buffer_t *key,
                                        uint8_t init_vector[HAL_AES_CBC_IV_LENGTH],
                                        hal_aes_key_t key_index)
{
    hal_aes_status_t status;
    uint8_t key_source, key_slot = 0;

    aes_key_map(key_index, &key_source, &key_slot);

    hal_clock_enable(HAL_CLOCK_CG_CRYPTO);
    status = hal_aes_encrypt_with_padding(encrypted_text, plain_text, key, init_vector, AES_TYPE_CBC, key_source, key_slot);
    hal_clock_disable(HAL_CLOCK_CG_CRYPTO);

    return status;
}

hal_aes_status_t hal_aes_cbc_decrypt_ex(hal_aes_buffer_t *plain_text,
                                        hal_aes_buffer_t *encrypted_text,
                                        hal_aes_buffer_t *key,
                                        uint8_t init_vector[HAL_AES_CBC_IV_LENGTH],
                                        hal_aes_key_t key_index)
{
    hal_aes_status_t status;
    uint8_t key_source, key_slot = 0;

    aes_key_map(key_index, &key_source, &key_slot);

    hal_clock_enable(HAL_CLOCK_CG_CRYPTO);
    status = hal_aes_decrypt(plain_text, encrypted_text, key, init_vector, AES_TYPE_CBC, key_source, key_slot);
    hal_clock_disable(HAL_CLOCK_CG_CRYPTO);

    return status;
}

hal_aes_status_t hal_aes_ecb_encrypt_ex(hal_aes_buffer_t *encrypted_text,
                                        hal_aes_buffer_t *plain_text,
                                        hal_aes_buffer_t *key,
                                        hal_aes_key_t key_index)
{
    uint8_t init_vector[16] = {0};
    hal_aes_status_t status;
    uint8_t key_source, key_slot = 0;

    aes_key_map(key_index, &key_source, &key_slot);

    hal_clock_enable(HAL_CLOCK_CG_CRYPTO);
    status = hal_aes_encrypt_with_padding(encrypted_text, plain_text, key, init_vector, AES_TYPE_ECB, key_source, key_slot);
    hal_clock_disable(HAL_CLOCK_CG_CRYPTO);

    return status;
}

hal_aes_status_t hal_aes_ecb_decrypt_ex(hal_aes_buffer_t *plain_text,
                                        hal_aes_buffer_t *encrypted_text,
                                        hal_aes_buffer_t *key,
                                        hal_aes_key_t key_index)
{
    uint8_t init_vector[HAL_AES_CBC_IV_LENGTH] = {0};
    hal_aes_status_t status;
    uint8_t key_source, key_slot = 0;

    aes_key_map(key_index, &key_source, &key_slot);

    hal_clock_enable(HAL_CLOCK_CG_CRYPTO);
    status = hal_aes_decrypt(plain_text, encrypted_text, key, init_vector, AES_TYPE_ECB, key_source, key_slot);
    hal_clock_disable(HAL_CLOCK_CG_CRYPTO);

    return status;
}

#endif /* HAL_AES_MODULE_ENABLED */

