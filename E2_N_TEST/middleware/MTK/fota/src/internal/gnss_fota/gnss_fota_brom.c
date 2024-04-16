/* Copyright Statement:
 *
 * (C) 2005-2017  MediaTek Inc. All rights reserved.
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

#include "gnss_fota_brom.h"
#include "string.h"
#include "hal_pinmux_define.h"
#include "hal_gpio.h"
#include "gnss_fota_uart.h"
#include "gnss_fota_log.h"

/*************************************************************************************************
*                             Macros Definitions                                                 *
**************************************************************************************************/

/* BootRom Command */
#define BOOT_ROM_WRITE_CMD              0xA1
#define BOOT_ROM_CHECKSUM_CMD           0xA4
#define BOOT_ROM_JUMP_CMD               0xA8
#define BOOT_ROM_WRITE32_CMD            0xAE
/* MT3301 BootRom Start Command */
#define MT3301_BOOT_ROM_START_CMD1      0xA0
#define MT3301_BOOT_ROM_START_CMD2      0x0A
#define MT3301_BOOT_ROM_START_CMD3      0x50
#define MT3301_BOOT_ROM_START_CMD4      0x05

#define BROM_DA_START_ADDR              0x00000C00

static const char* NMEA_START_CMD = "$PMTK180*3B\r\n";

/*************************************************************************************************
*                             Static Variants Definitions                                        *
**************************************************************************************************/
static const unsigned char MT3301_BOOT_ROM_START_CMD[] = {
    MT3301_BOOT_ROM_START_CMD1,
    MT3301_BOOT_ROM_START_CMD2,
    MT3301_BOOT_ROM_START_CMD3,
    MT3301_BOOT_ROM_START_CMD4
};
/*************************************************************************************************
*                             Local Functions Definitions                                        *
**************************************************************************************************/
static void gnss_enable_power(void);
static void gnss_disable_power(void);
static void gnss_enable_32k_output(void);
static void gnss_disable_32k_output(void);
static gnss_brom_result_t gnss_fota_brom_handshake(void);
static gnss_brom_result_t gnss_fota_brom_send_cmd(uint8_t cmd);
static gnss_brom_result_t gnss_fota_brom_send_data(uint32_t data);
static gnss_brom_result_t gnss_fota_brom_load_da(gnss_fota_brom_arg_t* p_brom_arg);
static gnss_brom_result_t gnss_fota_brom_send_checksum_cmd(uint32_t da_base_addr, uint32_t num_of_word, uint16_t *result);
static gnss_brom_result_t gnss_fota_brom_jump_to_da(uint32_t da_base_addr);
/*************************************************************************************************
*                             Public Functions Implementation                                    *
**************************************************************************************************/
gnss_brom_result_t gnss_fota_brom_processor(gnss_fota_brom_arg_t* p_brom_arg)
{
    gnss_brom_result_t result = GNSS_FOTA_BROM_OK;
    if ( NULL == p_brom_arg ) {
        return GNSS_FOTA_BROM_INVALID_ARGUMENTS;
    }

    gnss_fota_log_info("BRom processor begining!");

    result = gnss_fota_brom_handshake();
    if (result != GNSS_FOTA_BROM_OK) {
        gnss_fota_log_error("BROM : Sync Failed");
        return GNSS_FOTA_BROM_CMD_HANDSHAKE_FAIL;
    }

    result = gnss_fota_brom_load_da(p_brom_arg);
    if (result != GNSS_FOTA_BROM_OK) {
        gnss_fota_log_error("BROM : download DA Failed");
        return GNSS_FOTA_BROM_DOWNLOAD_DA_FAIL;
    }

    result = gnss_fota_brom_jump_to_da(BROM_DA_START_ADDR);
    if (result != GNSS_FOTA_BROM_OK) {
        gnss_fota_log_error("BROM : Jump to DA Failed");
        return GNSS_FOTA_BROM_CMD_JUMP_FAIL;
    }
    return GNSS_FOTA_BROM_OK;
}

/************************************************************************************************
*                             Local Functions Definitions                                       *
*************************************************************************************************/
static gnss_brom_result_t gnss_fota_brom_handshake(void)
{
    uint8_t data8;
    uint8_t fail_retry = 0;
    uint32_t i;
    uint8_t tmp8;
    uint32_t cnt = 0;
    uint8_t debug = 0;
    gnss_brom_result_t result = GNSS_FOTA_BROM_OK;

    gnss_fota_uart_init();
    gnss_enable_power();
    gnss_fota_uart_delay();
    gnss_enable_32k_output();
    gnss_fota_uart_delay();
    *(volatile int *)(0xa0020604) = 0x00021000;/* Enable Schmitt trigger */
    gnss_fota_uart_clear_buffer();

    //Wait 3333 FW initial
    for (i = 0; i < 200; ++i) {
        gnss_fota_uart_delay();
    }

    /* Send PMTK180 to force 3333 into boot rom */
    for (i = 1; i < 8; ++i) {
        gnss_fota_uart_put_byte_buffer((uint8_t*)NMEA_START_CMD, strlen(NMEA_START_CMD));
        gnss_fota_uart_delay();
    }

    //delay, wait for system restart.
    for (i = 0; i < 10; ++i) {
        gnss_fota_uart_delay();
    }
    gnss_fota_uart_clear_buffer();

    gnss_fota_log_info( "BROM: SEND SYNC CHAR");
    cnt = 0;
    while (1) {
        gnss_fota_uart_put_byte(MT3301_BOOT_ROM_START_CMD[0]); //First start command sync char
        if (true == gnss_fota_uart_get_byte_no_timeout(&data8)) {

            tmp8 = 0x5F;
            gnss_fota_log_info("Received data:%x \n\r", data8);
            if (tmp8 == data8) {
                gnss_fota_uart_clear_buffer();
                goto SECOND_CHAR;
            }
        }

        cnt++;
        data8 = 0xDD;
        if (cnt > 10000) {
            data8 = 0xCC;
            break;
        }
    }

    //Use Reset way to sync boot rom
    gnss_disable_32k_output();
    gnss_fota_uart_clear_buffer();
    debug = 0xC1;
    cnt = 0;
    for (fail_retry = 0; fail_retry < 5; ++fail_retry) {
        gnss_disable_power();
        for (i = 0; i < 300; ++i) {
            gnss_fota_uart_delay();
        }
        gnss_enable_power();

        i = 0;
        while (1) {
            gnss_fota_uart_put_byte(MT3301_BOOT_ROM_START_CMD[0]); //First start command sync char
            if (1 == gnss_fota_uart_get_byte_no_timeout(&data8)) {
                tmp8 = 0x5F;
                if (tmp8 == data8) {
                    gnss_fota_uart_clear_buffer();
                    goto SECOND_CHAR;
                }
            }
            cnt++;
            if (cnt > 50000) {
                debug++;
                cnt = 0;
                if (fail_retry == 4) {
                    return GNSS_FOTA_BROM_CMD_HANDSHAKE_FAIL;
                } else {
                    break;
                }
            }
        }
    }

SECOND_CHAR:
    gnss_fota_log_info("BROM : Sync second char");

    i = 1;
    gnss_fota_uart_put_byte(MT3301_BOOT_ROM_START_CMD[i]); //2nd sync char
    tmp8 = 0xF5;
    data8 = gnss_fota_uart_get_byte();
    gnss_fota_log_info("The second char::0x%x\n\r", data8);
    if (tmp8 != data8) {
        return GNSS_FOTA_BROM_CMD_HANDSHAKE_FAIL;
    }

    i = 2;
    gnss_fota_uart_put_byte(MT3301_BOOT_ROM_START_CMD[i]); //3rd sync char
    tmp8 = 0xAF;
    data8 = gnss_fota_uart_get_byte();
    if (tmp8 != data8) {
        return GNSS_FOTA_BROM_CMD_HANDSHAKE_FAIL;
    }

    i = 3;
    gnss_fota_uart_put_byte(MT3301_BOOT_ROM_START_CMD[i]); //4th sync char
    tmp8 = 0xFA;
    data8 = gnss_fota_uart_get_byte();
    if (tmp8 != data8) {
        return GNSS_FOTA_BROM_CMD_HANDSHAKE_FAIL;
    }
    return result;
}

static gnss_brom_result_t gnss_fota_brom_load_da(gnss_fota_brom_arg_t* p_brom_arg)
{
    gnss_bin_handle_t* p_da_handle = NULL;
    gnss_brom_result_t result = GNSS_FOTA_BROM_OK;
    uint32_t da_total_len = 0;
    uint16_t data;
    uint32_t num_of_word = 0;
    uint32_t i = 0;
    uint16_t checksum = 0;
    uint16_t brom_checksum = 0;
    uint8_t write_buf[10];
    uint8_t temp_buf[10];
    uint8_t read_buf[10];
    int32_t j;
    uint32_t last_percent = 0;

    if (NULL == p_brom_arg){
        return GNSS_FOTA_BROM_INVALID_ARGUMENTS;
    }

    p_da_handle = &(p_brom_arg->gnss_da_bin_handler);
    p_da_handle->gnss_fota_bin_init();
    da_total_len = p_da_handle->gnss_fota_get_bin_length();
    num_of_word = (da_total_len + 1) / 2;

    result = gnss_fota_brom_send_cmd(BOOT_ROM_WRITE_CMD);
    if (result != GNSS_FOTA_BROM_OK) {
        return result;
    }

    result = gnss_fota_brom_send_data(BROM_DA_START_ADDR);
    if (result != GNSS_FOTA_BROM_OK) {
        gnss_fota_log_error("BROM: send base address fail!, Err(%d).", result);
        return result;
    }

    result = gnss_fota_brom_send_data(num_of_word);
    if (result != GNSS_FOTA_BROM_OK) {
        gnss_fota_log_error("BROM: send number of word fail!,num_of_word(%lu), da_total_len(%lu), Err(%d).", num_of_word, da_total_len, result);
        return result;
    }

    gnss_fota_log_info("BROM: DA download Start");

    while ( i < da_total_len ) {
        p_da_handle->gnss_fota_get_data(temp_buf, 10);
        // copy from buf to write_buf by swap order
        for (j = 0; j < 10; j += 2) {
            write_buf[j] = temp_buf[j + 1];
            write_buf[j + 1] = temp_buf[j];
            data = write_buf[j + 1];
            data += (write_buf[j] << 8);
            checksum ^= data;
        }

        // write
        gnss_fota_uart_put_byte_buffer(write_buf, 10);

        // read bootrom echo to verify
        gnss_fota_uart_get_byte_buffer((uint32_t *) read_buf, 10);
        if (p_brom_arg->gnss_brom_progress != NULL){
            uint32_t percent = (uint32_t)((float)i / (float)da_total_len * 100.0f);
            if (last_percent != percent){
                p_brom_arg->gnss_brom_progress(GNSS_BROM_STATE, percent);
                last_percent = percent;
            }
        }

        if (memcmp(write_buf, read_buf, 10)) {
            gnss_fota_log_info( "Load DA data: write_buf={ %x, %x, %x, %x, %x, %x, %x, %x, %x, %x }. \n\r", write_buf[0], write_buf[1], write_buf[2], write_buf[3], write_buf[4], write_buf[5], write_buf[6], write_buf[7], write_buf[8], write_buf[9]);
            gnss_fota_log_info( "Load DA data: read_buf={ %x, %x, %x, %x, %x, %x, %x, %x, %x, %x }. \n\r", read_buf[0], read_buf[1], read_buf[2], read_buf[3], read_buf[4], read_buf[5], read_buf[6], read_buf[7], read_buf[8], read_buf[9]);
            gnss_fota_log_info( "Load DA data: write_buf != read_buf  \n\r");
            return GNSS_FOTA_BROM_SEND_DA_DATA_ERROR;
        }

        /* increase by 10, because we send 5 WORDs each time */
        i += 10;

    }
    if (p_brom_arg->gnss_brom_progress != NULL){
        p_brom_arg->gnss_brom_progress(GNSS_BROM_STATE, 100);
    }
    /* perform checksum verification */
    result = gnss_fota_brom_send_checksum_cmd(BROM_DA_START_ADDR, num_of_word, &brom_checksum);
    if (result != GNSS_FOTA_BROM_OK) {
        gnss_fota_log_error("Brom:checksum fail!, Err(%d). \n\r", result);
        return result;
    }

    // compare checksum
    if ( checksum != brom_checksum ) {
        gnss_fota_log_error("Brom: checksum error!, checksum(%x) != brom_checksum(%x) \n\r", checksum, brom_checksum);
        return GNSS_FOTA_BROM_CHKSUM16_MEM_RESULT_DIFF;
    } else {
        gnss_fota_log_info("Brom: checksum ok!, checksum(%x) == brom_checksum(%x). \n\r", checksum, brom_checksum);
    }

    return result;
}

static void gnss_enable_power(void)
{
    (void)hal_pinmux_set_function(HAL_GPIO_8, HAL_GPIO_8_GPIO8);
    (void)hal_gpio_set_direction(HAL_GPIO_8, HAL_GPIO_DIRECTION_OUTPUT);
    (void)hal_gpio_set_output(HAL_GPIO_8, HAL_GPIO_DATA_HIGH);
}
static void gnss_disable_power(void)
{
    (void)hal_pinmux_set_function(HAL_GPIO_8, HAL_GPIO_8_GPIO8);
    (void)hal_gpio_set_direction(HAL_GPIO_8, HAL_GPIO_DIRECTION_OUTPUT);
    (void)hal_gpio_set_output(HAL_GPIO_8, HAL_GPIO_DATA_LOW);
}

static void gnss_enable_32k_output(void)
{
    (void)hal_pinmux_set_function(HAL_GPIO_30, HAL_GPIO_30_CLKO3);
    (void)hal_gpio_set_clockout(HAL_GPIO_30, HAL_GPIO_CLOCK_MODE_32K);
}

static void gnss_disable_32k_output(void)
{
    (void)hal_pinmux_set_function(HAL_GPIO_30, HAL_GPIO_30_GPIO30);
    (void)hal_gpio_set_direction(HAL_GPIO_30, HAL_GPIO_DIRECTION_OUTPUT);
    (void)hal_gpio_set_output(HAL_GPIO_30, HAL_GPIO_DATA_LOW);
}

static gnss_brom_result_t gnss_fota_brom_send_cmd(uint8_t cmd)
{
    uint8_t  result;
    uint8_t* p_result;
    p_result = &result;

    gnss_fota_uart_put_byte(cmd);
    *p_result = gnss_fota_uart_get_byte();
    if ( cmd != *p_result ) {
        return GNSS_FOTA_BROM_SEND_CMD_ERROR;
    } else {
        return GNSS_FOTA_BROM_OK;
    }
}
static gnss_brom_result_t gnss_fota_brom_send_data(uint32_t data)
{
    uint32_t tmp32;
    gnss_fota_uart_put_uint32(data);
    tmp32 = gnss_fota_uart_get_uint32();

    if (tmp32 != data) {
        return GNSS_FOTA_BROM_SEND_DATA_ERROR;
    }
    return GNSS_FOTA_BROM_OK;
}

static gnss_brom_result_t gnss_fota_brom_send_checksum_cmd(uint32_t da_base_addr, uint32_t num_of_word, uint16_t *checksum)
{
    gnss_brom_result_t result = GNSS_FOTA_BROM_OK;

    result = gnss_fota_brom_send_cmd(BOOT_ROM_CHECKSUM_CMD);
    if (result != GNSS_FOTA_BROM_OK) {
        return result;
    }

    result = gnss_fota_brom_send_data(da_base_addr);
    if (result != GNSS_FOTA_BROM_OK) {
        gnss_fota_log_error( "Brom Checksum::base_address(%lx): send base address fail!, Err(%d).", da_base_addr, result);
        return result;
    }

    result = gnss_fota_brom_send_data(num_of_word);
    if (result != GNSS_FOTA_BROM_OK) {
        gnss_fota_log_info( "Brom Checksum::num_of_word(%lu): send number of word fail!, Err(%d).", num_of_word, result);
        return result;
    }

    /*read checksum from GNSS.*/
    *checksum = gnss_fota_uart_get_uint16();
    return 0;
}

static gnss_brom_result_t gnss_fota_brom_jump_to_da(uint32_t da_base_addr)
{
    gnss_brom_result_t result = GNSS_FOTA_BROM_OK;

    result = gnss_fota_brom_send_cmd(BOOT_ROM_JUMP_CMD);
    if (result != GNSS_FOTA_BROM_OK) {
        return result;
    }

    result = gnss_fota_brom_send_data(da_base_addr);
    if (result != GNSS_FOTA_BROM_OK) {
        gnss_fota_log_info("Brom: send jump address fail!,address(0x%lx), Err(%d).", da_base_addr, result);
        return result;
    }
    return result;
}
