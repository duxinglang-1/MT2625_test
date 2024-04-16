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

#ifndef __NVDM_MODEM_PORT_H__
#define __NVDM_MODEM_PORT_H__

#ifdef MTK_NVDM_MODEM_ENABLE

#include <FreeRTOS.h>
#include <stdint.h>
#include "stddef.h"
#include "stdbool.h"
#include "nvdm_modem.h"

/********************************************************
 * Macro & Define
 *
 ********************************************************/
#define NVDM_MODEM_PORT_USE_OS

#define ROM_NVDM_MODEM_NORMAL_LENGTH    0x00025000   /* 148kB */
#define ROM_NVDM_MODEM_PROTECTED_LENGTH 0x00006000   /* 24kB */
#define ROM_NVDM_MODEM_BACKUP_LENGTH    0x00006000   /* 24kB */
#define ROM_NVDM_MODEM_LENGTH           (ROM_NVDM_MODEM_NORMAL_LENGTH+ROM_NVDM_MODEM_PROTECTED_LENGTH+ROM_NVDM_MODEM_BACKUP_LENGTH)

#define ROM_NVDM_MODEM_NORMAL_BASE      (ROM_NVDM_BASE + ROM_NVDM_LENGTH - ROM_NVDM_MODEM_LENGTH)
#define ROM_NVDM_MODEM_PROTECTED_BASE   (ROM_NVDM_BASE + ROM_NVDM_LENGTH - ROM_NVDM_MODEM_BACKUP_LENGTH - ROM_NVDM_MODEM_PROTECTED_LENGTH)
#define ROM_NVDM_MODEM_BACKUP_BASE      (ROM_NVDM_BASE + ROM_NVDM_LENGTH - ROM_NVDM_MODEM_BACKUP_LENGTH)


#define NVDM_MODEM_PEB_NUMBER_PER_MINIDUMP  2

/********************************************************
 * Enum & Structures
 *
 ********************************************************/
typedef struct {
    void (*mem_read)(uint32_t, uint8_t *, uint32_t);
    void (*mem_write)(uint32_t, const uint8_t *, uint32_t);
    void (*mem_erase)(uint32_t);
} mem_access_t;

typedef enum {
    NVDM_MODEM_MEM_SET_CONTROL_TYPE = 0x1,
    NVDM_MODEM_MEM_SET_PEB_RAMDISK,
} mem_control_cmd_t;

/********************************************************
 * External Function
 *
 ********************************************************/
/* NVDM port functions declare */
void nvdm_modem_port_log_notice(const char *message, ...);
void nvdm_modem_port_log_info(const char *message, ...);
void nvdm_modem_port_log_error(const char *message, ...);
void nvdm_modem_port_flash_read(uint32_t address, uint8_t *buffer, uint32_t length);
void nvdm_modem_port_flash_write(uint32_t address, const uint8_t *buffer, uint32_t length);
void nvdm_modem_port_flash_erase(uint32_t address);
void *nvdm_modem_port_malloc(uint32_t size);
void nvdm_modem_port_free(void *pdata);
void nvdm_modem_port_mutex_create(void);
void nvdm_modem_port_mutex_take(void);
void nvdm_modem_port_mutex_give(void);
uint32_t nvdm_modem_port_get_data_item_config(uint32_t *max_data_item_size,
                                        uint32_t *max_group_name_size,
                                        uint32_t *max_data_item_name_size);
uint32_t nvdm_modem_port_get_peb_config(uint32_t *peb_count);
uint32_t nvdm_modem_port_get_peb_address(int32_t pnum, int32_t offset);
nvdm_modem_area_t nvdm_modem_port_get_working_area(bool examine);
void nvdm_modem_port_set_working_area(nvdm_modem_area_t area);
void nvdm_modem_port_poweroff_time_set(void);
void nvdm_modem_port_poweroff(uint32_t poweroff_time);
void nvdm_modem_port_mem_read(uint32_t address, uint8_t *buffer, uint32_t length);
void nvdm_modem_port_mem_write(uint32_t address, const uint8_t *buffer, uint32_t length);
void nvdm_modem_port_mem_erase(uint32_t address);
void nvdm_modem_port_mem_control(mem_control_cmd_t cmd, uint32_t para);

#endif

#endif

