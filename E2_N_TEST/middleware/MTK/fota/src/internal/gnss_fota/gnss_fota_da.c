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

#include "gnss_fota_da.h"
#include "gnss_fota_uart.h"
#include "stdbool.h"
#include "gnss_fota_log.h"
#include "string.h"

/**********************************************************************************
*                             Macros Definitions                                  *
***********************************************************************************/
#define UART_READ_WRITE_MAX_RETRY        0
#define UART_READ_WRITE_RETRY_SLEEP      100
#define UART_BAUDRATE_FULL_SYNC_COUNT    20
#define UART_ERR_RETRY_TIMEOUT           99
#define UART_BAUDRATE_SYNC_RETRY         50
#define PACKET_RE_TRANSMISSION_TIMES     3
#define DA_FLASH_ERASE_WAITING_TIME      1000

/* DA Version */
#define DA_MAJOR_VER    0x04
#define DA_MINOR_VER    0x00

/* RETURN VALUE */
#define HW_ERROR              0x1c
#define HW_RAM_OK             0xE0
#define HW_RAM_FLOARTING      0xE1
#define HW_RAM_UNACCESSABLE   0xE2
#define HW_RAM_ERROR          0xE3
#define SOC_FAIL              0x0c
#define SYNC_CHAR             0xc0
#define CONT_CHAR             0x69
#define STOP_CHAR             0x96
#define ACK                   0x5a
#define NACK                  0xa5
#define UNKNOWN_CMD           0xbb

/* FLASH OPERATION STATUS */
typedef enum {
    S_DONE = 0
   ,S_PGM_FAILED
   ,S_ERASE_FAILED
   ,S_TIMEOUT
   ,S_IN_PROGRESS
   ,S_CMD_ERR
   ,S_BLOCK_LOCKED_ERR
   ,S_BLOCK_UNSTABLE
   ,S_VPP_RANGE_ERR
   ,S_ERASE_ADDR_ERR
   ,S_ERASE_RANGE_ERR
   ,S_PGM_AT_ODD_ADDR
   ,S_PGM_WITH_ODD_LENGTH
   ,S_BUFPGM_NO_SUPPORT
   ,S_UNKNOWN_ERR
} STATUS_E;

/* COMMANDS */
#define DA_EXT_CLOCK_CMD        0xD0
#define DA_BBCHIP_TYPE_CMD      0xD1
#define DA_SPEED_CMD            0xD2
#define DA_MEM_CMD              0xD3
#define DA_FORMAT_CMD           0xD4
#define DA_WRITE_CMD            0xD5
#define DA_READ_CMD             0xD6
#define DA_WRITE_REG16_CMD      0xD7
#define DA_READ_REG16_CMD       0xD8
#define DA_FINISH_CMD           0xD9
#define DA_GET_DSP_VER_CMD      0xDA
#define DA_ENABLE_WATCHDOG_CMD  0xDB

typedef enum
{
   TIMEOUT_DATA = 0,
   CKSUM_ERROR,
   RX_BUFFER_FULL,
   TIMEOUT_CKSUM_LSB,
   TIMEOUT_CKSUM_MSB,
   ERASE_TIMEOUT,
   PROGRAM_TIMEOUT,
   RECOVERY_BUFFER_FULL,
   UNKNOWN_ERROR
}eRX_error;

/* DEVICE_INFO */
typedef enum
{
   DEVICE_TV0057A003AABD,
   DEVICE_LAST = DEVICE_TV0057A003AABD,
   DEVICE_UNKNOWN = 0xFF         // Unknown Device 
}da_device_info_t;

typedef struct {
    uint16_t manufacture_id;
    uint16_t device_code;
    uint16_t ext_device_code1;
    uint16_t ext_device_code2;
} flash_device_key_t;

typedef enum {
    GNSS_DA_ACCURACY_AUTO     = 0,     // auto detect by baudrate
    GNSS_DA_ACCURACY_1_3      = 3,     //   33%
    GNSS_DA_ACCURACY_1_4      = 4,     //   25%
    GNSS_DA_ACCURACY_1_10     = 10,    //   10%
    GNSS_DA_ACCURACY_1_100    = 100,   //    1%
    GNSS_DA_ACCURACY_1_1000   = 1000,  //  0.1%
    GNSS_DA_ACCURACY_1_10000  = 10000  // 0.01%
} gnss_da_accuracy;

#define FOTA_GNSS_DA_PACKET_LENGTH     1024
#define FOTA_GNSS_BIN_START_ADDR       0
/***********************************************************************************************
*                             Static Variants Definitions                                      *
************************************************************************************************/
static const char ErrAckTable[][64] = {
    "TIMEOUT_DATA",
    "CKSUM_ERROR",
    "RX_BUFFER_FULL",
    "TIMEOUT_CKSUM_LSB",
    "TIMEOUT_CKSUM_MSB",
    "ERASE_TIMEOUT",
    "PROGRAM_TIMEOUT",
    "RECOVERY_BUFFER_FULL",
    "UNKNOWN_ERROR"
};
/***********************************************************************************************
*                             Local Functions Prototypes                                       *
************************************************************************************************/
static gnss_da_result_t gnss_fota_da_read_data(uint8_t* p_read_buf, uint32_t read_len);
static gnss_da_result_t gnss_fota_da_set_memory_block(uint32_t begin_addr, uint32_t end_addr);
static gnss_da_result_t gnss_fota_da_finish(void);
static gnss_da_result_t gnss_fota_da_update_gnss_binary(gnss_fota_da_parameter_t* p_parameter);
static gnss_da_result_t gnss_fota_da_sync(void);
/***********************************************************************************************
*                             Public Functions Implementation                                  *
************************************************************************************************/
/**************************************************************************************************
 * @brief     Commucation with GNSS DA to transfer the GNSS binary to GNSS chip.
 *            And then GNSS DA will write the binary to GNSS flash's right partition.
 * @param[in] p_parameter - refer to gnss_fota_da_parameter_t
 * @return    Return the GNSS FOTA final result. Refer to gnss_da_result_t.
 **************************************************************************************************/
gnss_da_result_t gnss_fota_da_processor(gnss_fota_da_parameter_t* p_parameter)
{
    gnss_da_result_t result = GNSS_DA_OK;
    gnss_bin_handle_t* p_bin_handle = NULL;
    uint32_t gnss_bin_len = 0;

    if (NULL == p_parameter){
        return GNSS_DA_WRONG_PARAMETER;
    }

    result = gnss_fota_da_sync();
    if (result != GNSS_DA_OK){
       gnss_fota_log_error("GNSS DA: DA sync failed.");
       return result;
    }
    p_bin_handle = &(p_parameter->gnss_bin_handle);
    p_bin_handle->gnss_fota_bin_init();
    gnss_bin_len = p_bin_handle->gnss_fota_get_bin_length();
    result = gnss_fota_da_set_memory_block(FOTA_GNSS_BIN_START_ADDR, gnss_bin_len - 1);
    if (result != GNSS_DA_OK){
       gnss_fota_log_error("GNSS DA: set memory block failed.");
       return result;
    }

    result = gnss_fota_da_update_gnss_binary(p_parameter);
    if (result != GNSS_DA_OK){
       gnss_fota_log_error("GNSS DA: update binary failed.");
       return result;
    }

    result = gnss_fota_da_finish();
    return result;
}
/***********************************************************************************************
*                             Local Functions Implementation                                   *
************************************************************************************************/
static gnss_da_result_t gnss_fota_da_sync(void)
{
    gnss_da_result_t result = GNSS_DA_OK;
#if 0 /*deleted, for fixing build waining.*/
    flash_device_key_t device_key;
    uint32_t flash_size = 0;
    uint32_t ext_ram_size = 0;
#endif
    uint8_t buf[32] = {0};

    result = gnss_fota_da_read_data(buf, 1);
    if (result != GNSS_DA_OK){
        gnss_fota_log_error("GNSS DA: SYNC_CHAR no response.");
        return GNSS_DA_NO_RESPONSE;
    }
    gnss_fota_log_info("GNSS DA: SYNC_CHAR Get:0x%x.", buf[0]);
    if ( SOC_FAIL == buf[0] ) {
        gnss_fota_log_error("GNSS DA: SOC_FAIL");
        return GNSS_DA_SOC_CHECK_FAIL;
    } else if ( HW_ERROR == buf[0] ) {
        gnss_fota_log_error("GNSS DA: HW ERROR");
        return GNSS_DA_HW_ERROR;
    } else if ( SYNC_CHAR != buf[0] ) {
        gnss_fota_log_error("GNSS DA: incorrect sync char.");
        return GNSS_DA_SYNC_INCORRECT;
    }

    gnss_fota_log_info("GNSS DA: wait DA_MAJOR_VER, DA_MINOR_VER.");
    result = gnss_fota_da_read_data(buf, 2);
    if (result != GNSS_DA_OK){
        gnss_fota_log_error("GNSS DA: SYNC_CHAR no response.");
        return GNSS_DA_NO_RESPONSE;
    }

    if ((DA_MAJOR_VER != buf[0]) || (DA_MINOR_VER != buf[1])) {
        gnss_fota_log_error("GNSS DA: DA_v%d.%d was expired, expect DA_v%d.%d .", buf[0], buf[1], DA_MAJOR_VER, DA_MINOR_VER);
        return GNSS_DA_VERSION_INCORRECT;
    }
    gnss_fota_log_info("GNSS DA: wait DEVICE_INFO.\n\r");
    result = gnss_fota_da_read_data(buf, 1);
    if (result != GNSS_DA_OK){
        gnss_fota_log_error("GNSS DA: SYNC_CHAR no response.");
        return GNSS_DA_NO_RESPONSE;
    }

    if ( DEVICE_UNKNOWN == ((da_device_info_t)buf[0]) ) {
        gnss_fota_log_error("GNSS DA: unknown device.");
        return GNSS_DA_UNKNOWN_FLASH_DEVICE;
    }

    /* get flash size, manufacture id and device code and ext sram size */
    result = gnss_fota_da_read_data(buf, 16);
    if (result != GNSS_DA_OK){
        gnss_fota_log_error("GNSS DA: SYNC_CHAR no response.");
        return GNSS_DA_NO_RESPONSE;
    }

#if 0 /* deleted, for fixing build warning. */
    flash_size = ((buf[0] << 24) & 0xFF000000) | ((buf[1] << 16) & 0x00FF0000) | ((buf[2] << 8) & 0x0000FF00) | ((buf[3]) & 0x000000FF);
    device_key.manufacture_id = ((buf[4] << 8) & 0xFF00) | ((buf[5]) & 0x00FF);
    device_key.device_code = ((buf[6] << 8) & 0xFF00) | ((buf[7]) & 0x00FF);
    device_key.ext_device_code1 = ((buf[8] << 8) & 0xFF00) | ((buf[9]) & 0x00FF);
    device_key.ext_device_code2 = ((buf[10] << 8) & 0xFF00) | ((buf[11]) & 0x00FF);
    ext_ram_size = ((buf[12] << 24) & 0xFF000000) | ((buf[13] << 16) & 0x00FF0000) | ((buf[14] << 8) & 0x0000FF00) | ((buf[15]) & 0x000000FF);
#endif
    gnss_fota_log_info("GNSS DA: SYNC OK.");
    return result;
}

static gnss_da_result_t gnss_fota_da_send_data(uint8_t* p_buf, uint32_t length)
{
    uint32_t writted_bytes = 0;
    uint32_t rest_of_bytes = 0;

    if ( NULL == p_buf || 0 >= length ) {
        return GNSS_DA_WRONG_PARAMETER;
    }

    while ( writted_bytes < length ) {
        rest_of_bytes = length - writted_bytes;
        gnss_fota_uart_put_byte_buffer(p_buf, rest_of_bytes);
        writted_bytes += rest_of_bytes;
    }
    return GNSS_DA_OK;
}

static gnss_da_result_t gnss_fota_da_read_data(uint8_t* p_read_buf, uint32_t read_len)
{
    uint32_t readed_bytes = 0;
    uint32_t rest_of_bytes = 0;

    if ( (NULL == p_read_buf) || (0 >= read_len) ) {
        return GNSS_DA_WRONG_PARAMETER;
    }

    memset(p_read_buf, 0, read_len);

    while ( readed_bytes < read_len ) {
        rest_of_bytes = read_len - readed_bytes;

        if (gnss_fota_uart_get_byte_buffer((uint32_t *) p_read_buf, rest_of_bytes) == false){
            gnss_fota_log_error("GNSS UART get bytes error!!!!!");
            return GNSS_DA_READ_DATA_FAILED;
        }
        readed_bytes += rest_of_bytes;
    }
    return GNSS_DA_OK;
}

static gnss_da_result_t gnss_fota_da_set_memory_block(uint32_t begin_addr, uint32_t end_addr)
{
    uint8_t buf[4];
    gnss_da_result_t result = GNSS_DA_OK;

    gnss_fota_log_info("GNSS DA: begin addr: 0x%lx, end_add 0x%lx", begin_addr, end_addr);

    /* send mem block command */
    buf[0] = DA_MEM_CMD;
    buf[1] = 0x01;
    result = gnss_fota_da_send_data(buf, 2);
    if (result != GNSS_DA_OK) {
        return result;
    }

    /* send MEM begin addr, end addr */
    begin_addr = begin_addr % 0x08000000;
    end_addr = end_addr % 0x08000000;

    /* send begin addr, high byte first */
    buf[0] = (unsigned char)((begin_addr >> 24) & 0x000000FF);
    buf[1] = (unsigned char)((begin_addr >> 16) & 0x000000FF);
    buf[2] = (unsigned char)((begin_addr >> 8) & 0x000000FF);
    buf[3] = (unsigned char)((begin_addr)    & 0x000000FF);

    result = gnss_fota_da_send_data(buf, 4);
    if (result != GNSS_DA_OK) {
        return result;
    }

    /* send end addr, high byte first */
    buf[0] = (unsigned char)((end_addr >> 24) & 0x000000FF);
    buf[1] = (unsigned char)((end_addr >> 16) & 0x000000FF);
    buf[2] = (unsigned char)((end_addr >> 8) & 0x000000FF);
    buf[3] = (unsigned char)((end_addr)    & 0x000000FF);

    result = gnss_fota_da_send_data(buf, 4);
    if (result != GNSS_DA_OK) {
        return result;
    }

    /* GNSS DA set memory block: wait for ACK. */
    result = gnss_fota_da_read_data(buf, 1);
    if (result != GNSS_DA_OK) {
        return result;
    }

    if ( ACK != buf[0] ) {
        gnss_fota_log_info("GNSS DA set memory block: non-ACK(0x%x) return.", buf[0]);
        return GNSS_DA_SET_MEMORY_BLOCK_FAILED;
    }

    /*read unchanged data blocks.*/
    result = gnss_fota_da_read_data(buf, 1);
    if (result != GNSS_DA_OK) {
        return result;
    }
    gnss_fota_log_info("GNSS DA: UNCHANED_DATA_BLOCKS=(0x%x)", buf[0]);
    return result;
}

gnss_da_result_t gnss_fota_da_finish(void)
{
    uint8_t buf[2];

    /* send DA_FINISH_CMD command */
    buf[0] = DA_FINISH_CMD;
    gnss_fota_uart_put_byte(buf[0]);
    gnss_fota_log_info("GNSS DA: Finish command: OK!");
    return GNSS_DA_OK;
}

gnss_da_result_t gnss_fota_da_update_gnss_binary(gnss_fota_da_parameter_t* p_parameter)
{
    uint8_t buf[5];
    uint32_t finish_rate = 0;
    uint32_t total_bytes = 0;
    uint32_t total_sent_bytes = 0;
    gnss_da_accuracy accuracy;
    uint32_t sent_bytes;
    uint32_t retry_count = 0;
    uint32_t j;
    uint32_t rate;
    uint16_t checksum;
    uint32_t frame_bytes;
    gnss_bin_handle_t* p_gnss_bin = NULL;
    uint8_t* p_gnss_bin_buffer = NULL;
    gnss_da_result_t result = GNSS_DA_OK;

    if ( NULL == p_parameter ) {
        gnss_fota_log_info("GNSS DA: p_parameter is NULL!");
        return GNSS_DA_WRONG_PARAMETER;
    }

    p_gnss_bin_buffer = (uint8_t *)pvPortMalloc(FOTA_GNSS_DA_PACKET_LENGTH);
    if (NULL == p_gnss_bin_buffer){
        gnss_fota_log_info("GNSS DA: out of memory!");
        return GNSS_DA_OUT_OF_MEMORY;
    }

    p_gnss_bin = &(p_parameter->gnss_bin_handle);
    p_gnss_bin->gnss_fota_bin_init();
    total_bytes = p_gnss_bin->gnss_fota_get_bin_length();

    /*---------- send write command + packet length ----------------------*/
    buf[0] = DA_WRITE_CMD;
    buf[1] = (unsigned char)((FOTA_GNSS_DA_PACKET_LENGTH >> 24) & 0x000000FF);
    buf[2] = (unsigned char)((FOTA_GNSS_DA_PACKET_LENGTH >> 16) & 0x000000FF);
    buf[3] = (unsigned char)((FOTA_GNSS_DA_PACKET_LENGTH >> 8)  & 0x000000FF);
    buf[4] = (unsigned char)((FOTA_GNSS_DA_PACKET_LENGTH)     & 0x000000FF);
    result = gnss_fota_da_send_data(buf, sizeof(buf));
    if (result != GNSS_DA_OK) {
        vPortFree(p_gnss_bin_buffer);
        return result;
    }

    accuracy = GNSS_DA_ACCURACY_1_100;

    result = gnss_fota_da_read_data(buf, 1);
    if ( GNSS_DA_OK != result ) {
        gnss_fota_log_info("GNSS DA: read data failed:%d", result);
        vPortFree(p_gnss_bin_buffer);
        return result;
    }

    if ( ACK != buf[0] ) {
        gnss_fota_log_info( "GNSS DA: fail to save all the unchanged data from flash!, %s", buf[0] > UNKNOWN_ERROR ? "UNKNOWN_ACK" : ErrAckTable[buf[0]]);
        vPortFree(p_gnss_bin_buffer);
        return GNSS_DA_SAVE_UNCHANGE_DATA_FAILED;
    }

    // wait for 1st sector erase done
    result = gnss_fota_da_read_data(buf, 1);
    if ( GNSS_DA_OK != result ) {
        gnss_fota_log_info("GNSS DA: read data failed:%d", result);
        vPortFree(p_gnss_bin_buffer);
        return GNSS_DA_READ_DATA_FAILED;
    }

    if ( ACK != buf[0] ) {
        gnss_fota_log_info("GNSS DA: fail to erase the 1st sector!, %s", buf[0] > UNKNOWN_ERROR ? "UNKNOWN_ACK" : ErrAckTable[buf[0]]);
        vPortFree(p_gnss_bin_buffer);
        return GNSS_DA_ERASE_FLASH_FAILED;
    }

    // send all rom files
    finish_rate = 0;
    total_sent_bytes = 0;
    sent_bytes = 0;
    retry_count = 0;
    gnss_fota_log_info("GNSS DA: GNSS BIN info: total_sent_bytes=%ld/%ld..\n\r",total_sent_bytes, total_bytes);
    while ( sent_bytes < total_bytes ) {
re_transmission:
        /* reset the frame checksum */
        checksum = 0;
        /* if the last frame is less than PACKET_LENGTH bytes */
        if (FOTA_GNSS_DA_PACKET_LENGTH > (total_bytes - sent_bytes)) {
            frame_bytes = total_bytes - sent_bytes;
        } else {
            /* normal frame */
            frame_bytes = FOTA_GNSS_DA_PACKET_LENGTH;
        }
        p_gnss_bin->gnss_fota_get_data(p_gnss_bin_buffer, frame_bytes);
        gnss_fota_uart_put_byte_buffer(p_gnss_bin_buffer, frame_bytes);

        /* calculate checksum */
        for (j = 0; j < frame_bytes; j++) {
            /* WARNING: MUST make sure it unsigned value to do checksum */
            checksum += p_gnss_bin_buffer[j];
        }

        // send 2 bytes checksum, high byte first
        buf[0] = (unsigned char)((checksum >> 8) & 0x000000FF);
        buf[1] = (unsigned char)((checksum)    & 0x000000FF);

        if (gnss_fota_da_send_data(buf, 2)) {
            goto read_cont_char;
        }

read_cont_char:
        // read CONT_CHAR
        buf[0] = 0xEE;
        if (GNSS_DA_OK != gnss_fota_da_read_data(buf, 1)) {
            gnss_fota_log_info("GNSS DA: read condition char failed.");
            vPortFree(p_gnss_bin_buffer);
            return GNSS_DA_READ_CONT_CHAR_FAILED;
        }
        switch (buf[0]) {
            case CONT_CHAR:
                // sent ok!, reset retry_count
                retry_count = 0;
                break;
            case ERASE_TIMEOUT:
                // flash erase timeout abort transmission
                return 9;
            case PROGRAM_TIMEOUT:
                // program timeout abort transmission
                return 10;
            case RECOVERY_BUFFER_FULL:
                // recovery buffer is not large enough to backup all the unchanged data before erase
                return 11;
            case RX_BUFFER_FULL:
            // target RX buffer is full, add delay to wait for flash erase done
            //Sleep(DA_FLASH_ERASE_WAITING_TIME);
            case CKSUM_ERROR:
            case TIMEOUT_DATA:
            case TIMEOUT_CKSUM_LSB:
            case TIMEOUT_CKSUM_MSB:
            default:
                 gnss_fota_log_info("GNSS DA: CONT_CHART Received( %x).\n\r", buf[0]);
                // check retry times
                if ( PACKET_RE_TRANSMISSION_TIMES > retry_count ) {
                    retry_count++;
                    gnss_fota_log_error("RE-TRY to send data!!!!");
                } else {
                    // fail to re-transmission
                    buf[0] = NACK;
                    gnss_fota_da_send_data(buf, 1);
                    vPortFree(p_gnss_bin_buffer);
                    return GNSS_DA_RETRANSMISSION_FAILED;
                }

                // wait for DA clean RX buffer
                result = gnss_fota_da_read_data(buf, 1);
                if ( result !=  GNSS_DA_OK) {
                    vPortFree(p_gnss_bin_buffer);
                    return result;
                }

                if ( ACK != buf[0] ) {
                    vPortFree(p_gnss_bin_buffer);
                    return 14;
                }

                // send CONT_CHAR to wakeup DA to start recieving again
                buf[0] = CONT_CHAR;
                if (gnss_fota_da_send_data(buf, 1) == GNSS_DA_OK) {
                    vPortFree(p_gnss_bin_buffer);
                    return 15;
                }

                // re-transmission this frame
                goto re_transmission;

                //break;
        }

        // update progress state
        sent_bytes += frame_bytes;
        total_sent_bytes += frame_bytes;

        // calculate finish rate
        if ( accuracy < (rate = (unsigned int)(((float)total_sent_bytes / total_bytes) * accuracy)) ) {
            rate = accuracy;
        }

        if ( 0 < (rate - finish_rate) ) {
            finish_rate = rate;
            if (p_parameter->gnss_da_progress != NULL){
                p_parameter->gnss_da_progress(GNSS_DA_STATE, finish_rate);
            }
            gnss_fota_log_info("GNSS DA update data: (%d%%): %ld bytes sent, total_bytes=%ld/%ld.", (unsigned char)finish_rate, sent_bytes, total_sent_bytes, total_bytes);
        }
    }
    //gnss_fota_log_info("GNSS DA update data: (%d%%): %d bytes sent, total_bytes=%d/%d.", (unsigned char)finish_rate, sent_bytes, total_sent_bytes, total_bytes);

    gnss_fota_log_info("GNSS DA: wait for DA to perform unchanged data recovery.");
    result = gnss_fota_da_read_data(buf, 1);
    if ( result != GNSS_DA_OK ) {
        vPortFree(p_gnss_bin_buffer);
        return result;
    }

    if ( ACK != buf[0] ) {
        gnss_fota_log_info("GNSS DA: %s( %x): fail to recover all the unchanged data to flash!\n\r", buf[0] > UNKNOWN_ERROR ? "UNKNOWN_ACK" : ErrAckTable[buf[0]], buf[0]);
        vPortFree(p_gnss_bin_buffer);
        return GNSS_DA_RECOVER_DATA_FAILED;
    }

    // wait for checksum ack
    gnss_fota_log_info("GNSS DA: wait for DA to perform flash checksum.!");
    result = gnss_fota_da_read_data(buf, 1);
    if ( result != GNSS_DA_OK ) {
        vPortFree(p_gnss_bin_buffer);
        return result;
    }

    if ( NACK == buf[0] ) {
        gnss_fota_log_info( "GNSS DA: NACK( %x) return, flash checksum error!\n\r", buf[0]);
        vPortFree(p_gnss_bin_buffer);
        return GNSS_DA_CHECKSUM_ERROR;
    } else if ( ACK != buf[0] ) {
        gnss_fota_log_info( "GNSS DA: non-ACK( %x) return.\n\r", buf[0]);
        vPortFree(p_gnss_bin_buffer);
        return GNSS_DA_CHECKSUM_ERROR;
    }

    gnss_fota_log_info( "GNSS DA: ACK( %x): checksum OK!\n\r", buf[0]);
    vPortFree(p_gnss_bin_buffer);
    return result;
}
