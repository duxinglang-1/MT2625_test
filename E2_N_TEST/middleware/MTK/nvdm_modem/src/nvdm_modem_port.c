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

#ifdef MTK_NVDM_MODEM_ENABLE
#include <string.h>
#include <assert.h>
#include "hal_flash.h"
#include "syslog.h"
#include "nvdm_modem_port.h"
#include "nvdm_modem.h"
#include "memory_map.h"
#ifdef NVDM_MODEM_PORT_USE_OS
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#else
#include "malloc.h"
#endif

/********************************************************
 * Macro & Define
 *
 ********************************************************/
#define NVDM_IMM_LOG_PRINT  0 /*FPGA*/

/* This macro defines max count of data items */
#define NVDM_MODEM_PORT_DAT_ITEM_COUNT (250)

/* This macro defines size of PEB, normally it is size of flash block */
#define NVDM_MODEM_PORT_PEB_SIZE	(4096)

/* This macro defines max size of data item during all user defined data items.
 * 1. Must not define it greater than 2048 bytes.
 * 2. Define it as smaller as possible to enhance the utilization rate of NVDM region.
 * 2. Try your best to store small data less than 256 bytes.
 */
#define NVDM_MODEM_PORT_MAX_DATA_ITEM_SIZE	(2048)

/* This macro defines max length of group name of data item */
#define GROUP_NAME_MAX_LENGTH (16)

/* This macro defines max length of data item name of data item */
#define DATA_ITEM_NAME_MAX_LENGTH (32)

#if defined(FLASH_BASE)
#define NVDM_FLASH_BASE FLASH_BASE
#else
#define NVDM_FLASH_BASE BL_BASE
#endif

 /* This macro defines start address and PEB count of the NVDM region */
#define NVDM_MODEM_PORT_NORMAL_REGION_ADDRESS(pnum, offset)     (ROM_NVDM_MODEM_NORMAL_BASE - NVDM_FLASH_BASE + pnum * NVDM_MODEM_PORT_PEB_SIZE + offset)
#define NVDM_MODEM_PORT_NORMAL_REGION_PEB_COUNT                 (ROM_NVDM_MODEM_NORMAL_LENGTH / NVDM_MODEM_PORT_PEB_SIZE)
#define NVDM_MODEM_PORT_PROTECTED_REGION_ADDRESS(pnum, offset)  (ROM_NVDM_MODEM_PROTECTED_BASE - NVDM_FLASH_BASE + pnum * NVDM_MODEM_PORT_PEB_SIZE + offset)
#define NVDM_MODEM_PORT_PROTECTED_REGION_PEB_COUNT              (ROM_NVDM_MODEM_PROTECTED_LENGTH / NVDM_MODEM_PORT_PEB_SIZE)
#define NVDM_MODEM_PORT_BACKUP_REGION_ADDRESS(pnum, offset)     (ROM_NVDM_MODEM_BACKUP_BASE - NVDM_FLASH_BASE + pnum * NVDM_MODEM_PORT_PEB_SIZE + offset)
#define NVDM_MODEM_PORT_BACKUP_REGION_PEB_COUNT                 (ROM_NVDM_MODEM_BACKUP_LENGTH / NVDM_MODEM_PORT_PEB_SIZE)

/* RAMDISK address transform*/
#define NVDM_MODEM_PORT_FLASH_TO_RAMDISK_ADDRESS(addr)          (uint32_t)(g_prRamdisk_base + addr % NVDM_MODEM_PORT_PEB_SIZE)

#if (ROM_NVDM_LENGTH < ROM_NVDM_MODEM_LENGTH)
#error "NVDM MODEM space is not enough, please check memory_map.h on project"
#endif

#ifdef MTK_MINI_DUMP_ENABLE
#if !defined(MINI_DUMP_BASE) && !defined(MINI_DUMP_LENGTH) /*TODO*/
#define MINI_DUMP_BASE      0
#define MINI_DUMP_LENGTH    0
#endif
#define NVDM_MODEM_PORT_MINIDUMP_REGION_ADDRESS(pnum, offset)    (MINI_DUMP_BASE - NVDM_FLASH_BASE + pnum * NVDM_MODEM_PORT_PEB_SIZE + offset)
#define NVDM_MODEM_PORT_MINIDUMP_REGION_PEB_COUNT                (MINI_DUMP_LENGTH / NVDM_MODEM_PORT_PEB_SIZE)
#endif
/********************************************************
 * Enum & Structures & Global variable
 *
 ********************************************************/
/* current working area */
nvdm_modem_area_t g_nvdm_modem_working_area;

/* ramdisk address */
static void *g_prRamdisk_base = NULL;

/* access method */
static mem_access_t g_mem_access = {
    .mem_read = nvdm_modem_port_flash_read,
    .mem_write = nvdm_modem_port_flash_write,
    .mem_erase = nvdm_modem_port_flash_erase,
};

extern log_control_block_t log_control_block_common;
#ifdef NVDM_MODEM_PORT_USE_OS
static SemaphoreHandle_t g_nvdm_modem_mutex;
#endif
/********************************************************
 * Function declaration
 *
 ********************************************************/
void nvdm_modem_port_log_notice(const char *message, ...)
{
#if !defined (MTK_DEBUG_LEVEL_NONE)
    va_list ap;

    va_start(ap, message);
#if NVDM_IMM_LOG_PRINT
    printf("[NVDM][MD][NOTE]");
    vprintf(message, ap);
    printf("\r\n");
#else
    vprint_module_log(&log_control_block_common, __FUNCTION__, __LINE__, PRINT_LEVEL_INFO, message, ap);
#endif
    va_end(ap);
#endif
}

void nvdm_modem_port_log_info(const char *message, ...)
{
#if !defined (MTK_DEBUG_LEVEL_NONE)
    va_list ap;

    va_start(ap, message);
#if NVDM_IMM_LOG_PRINT
    //printf("[NVDM][MD][INFO]");
    //vprintf(message, ap);
    //printf("\r\n");
#else
    //vprint_module_log(&log_control_block_common, __FUNCTION__, __LINE__, PRINT_LEVEL_INFO, message, ap);
#endif
    va_end(ap);
#endif
}

void nvdm_modem_port_log_error(const char *message, ...)
{
#if !defined (MTK_DEBUG_LEVEL_NONE)
    va_list ap;

    va_start(ap, message);
#if NVDM_IMM_LOG_PRINT
    printf("[NVDM][MD][ERRO]");
    vprintf(message, ap);
    printf("\r\n");
#else
    vprint_module_log(&log_control_block_common, __FUNCTION__, __LINE__, PRINT_LEVEL_ERROR, message, ap);
#endif
    va_end(ap);
#endif
    assert(0);
}

void nvdm_modem_port_flash_read(uint32_t address, uint8_t *buffer, uint32_t length)
{
    hal_flash_status_t status;

    while (1) {
        status = hal_flash_read(address, buffer, length);
        if (status != HAL_FLASH_STATUS_ERROR_LOCKED) {
            break;
        }
        vTaskDelay(1);
    }

    if (status != HAL_FLASH_STATUS_OK) {
        nvdm_modem_port_log_error("hal_flash_read: address = 0x%08x, buffer = 0x%08x, length = %d, status=%d", address, (uint32_t)buffer, length, status);
    }
}

void nvdm_modem_port_flash_write(uint32_t address, const uint8_t *buffer, uint32_t length)
{
    hal_flash_status_t status;

    while (1) {
        status = hal_flash_write(address, buffer, length);
        if (status != HAL_FLASH_STATUS_ERROR_LOCKED) {
            break;
        }
        vTaskDelay(1);
    }

    if (status != HAL_FLASH_STATUS_OK) {
        nvdm_modem_port_log_error("hal_flash_write: address = 0x%08x, buffer = 0x%08x, length = %d, status=%d", address, (uint32_t)buffer, length, status);
    }
}

/* erase unit is 4K large(which is size of PEB) */
void nvdm_modem_port_flash_erase(uint32_t address)
{
    hal_flash_status_t status;

    while (1) {
        status = hal_flash_erase(address, HAL_FLASH_BLOCK_4K);
        if (status != HAL_FLASH_STATUS_ERROR_LOCKED) {
            break;
        }
        vTaskDelay(1);
    }

    if (status != HAL_FLASH_STATUS_OK) {
        nvdm_modem_port_log_error("hal_flash_erase: address = 0x%08x, status=%d", address, status);
    }
}

void nvdm_modem_port_otp_read(uint32_t address, uint8_t *buffer, uint32_t length)
{
    hal_flash_status_t status;

    while (1) {
        status = hal_flash_otp_read(address, buffer, length);
        if (status != HAL_FLASH_STATUS_ERROR_LOCKED) {
            break;
        }
        vTaskDelay(1);
    }

    if (status != HAL_FLASH_STATUS_OK) {
        nvdm_modem_port_log_error("hal_flash_otp_read: address = 0x%08x, buffer = 0x%08x, length = %d, status=%d", address, (uint32_t)buffer, length, status);
    }
}

void nvdm_modem_port_otp_write(uint32_t address, const uint8_t *buffer, uint32_t length)
{
    hal_flash_status_t status;

    while (1) {
        status = hal_flash_otp_write(address, buffer, length);
        if (status != HAL_FLASH_STATUS_ERROR_LOCKED) {
            break;
        }
        vTaskDelay(1);
    }

    if (status != HAL_FLASH_STATUS_OK) {
        nvdm_modem_port_log_error("hal_flash_otp_write: address = 0x%08x, buffer = 0x%08x, length = %d, status=%d", address, (uint32_t)buffer, length, status);
    }
}

void nvdm_modem_port_otp_lock(void)
{
    hal_flash_status_t status;

    while (1) {
        status = hal_flash_otp_lock();
        if (status != HAL_FLASH_STATUS_ERROR_LOCKED) {
            break;
        }
        vTaskDelay(1);
    }

    if (status != HAL_FLASH_STATUS_OK) {
        nvdm_modem_port_log_error("hal_flash_otp_lock: status=%d", status);
    }
}

void nvdm_modem_port_get_otp_length(uint32_t *length)
{
    hal_flash_status_t status;

    status = hal_flash_get_otp_length(length);
    if (status != HAL_FLASH_STATUS_OK) {
        nvdm_modem_port_log_error("hal_flash_get_otp_length: status=%d", status);
    }
}

void nvdm_modem_port_mutex_create(void)
{
#ifdef NVDM_MODEM_PORT_USE_OS
    g_nvdm_modem_mutex = xSemaphoreCreateMutex();

    if (g_nvdm_modem_mutex == NULL) {
        nvdm_modem_port_log_error("nvdm_modem_port_mutex_create error\r\n");
    }
    nvdm_modem_port_log_info("nvdm_modem_port_mutex_create successfully");
#endif
}

void nvdm_modem_port_mutex_take(void)
{
#ifdef NVDM_MODEM_PORT_USE_OS
    if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) {
        if (xSemaphoreTake(g_nvdm_modem_mutex, portMAX_DELAY) == pdFALSE) {
            nvdm_modem_port_log_error("nvdm_modem_port_mutex_take error\r\n");
        }
        //nvdm_modem_port_log_info("nvdm_modem_port_mutex_take successfully");
    }
#endif
}

void nvdm_modem_port_mutex_give(void)
{
#ifdef NVDM_MODEM_PORT_USE_OS
    if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) {
        if (xSemaphoreGive(g_nvdm_modem_mutex) == pdFALSE) {
            nvdm_modem_port_log_error("nvdm_modem_port_mutex_give error\r\n");
        }
        //nvdm_modem_port_log_info("nvdm_modem_port_mutex_give successfully");
    }
#endif
}

void *nvdm_modem_port_malloc(uint32_t size)
{
#ifdef NVDM_MODEM_PORT_USE_OS
    return pvPortMalloc(size);
#else
    return malloc(size)
#endif
}

void nvdm_modem_port_free(void *pdata)
{
#ifdef NVDM_MODEM_PORT_USE_OS
    vPortFree(pdata);
#else
    free(pdata);
#endif
}

uint32_t nvdm_modem_port_get_data_item_config(uint32_t *max_data_item_size,
                                        uint32_t *max_group_name_size,
                                        uint32_t *max_data_item_name_size)
{
    *max_data_item_size = NVDM_MODEM_PORT_MAX_DATA_ITEM_SIZE;
    *max_group_name_size = GROUP_NAME_MAX_LENGTH;
    *max_data_item_name_size = DATA_ITEM_NAME_MAX_LENGTH;

    return NVDM_MODEM_PORT_DAT_ITEM_COUNT;
}

uint32_t nvdm_modem_port_get_peb_config(uint32_t *peb_count)
{
    nvdm_modem_area_t area = nvdm_modem_port_get_working_area(true);
    if (area == NVDM_MODEM_AREA_NORMAL) {
        *peb_count = NVDM_MODEM_PORT_NORMAL_REGION_PEB_COUNT;
    } else if (area == NVDM_MODEM_AREA_PROTECTED) {
        *peb_count = NVDM_MODEM_PORT_PROTECTED_REGION_PEB_COUNT;
    } else if (area == NVDM_MODEM_AREA_BACKUP) {
        *peb_count = NVDM_MODEM_PORT_BACKUP_REGION_PEB_COUNT;
    }
#ifdef MTK_MINI_DUMP_ENABLE
    else if (area == NVDM_MODEM_AREA_MINIDUMP) {
        *peb_count = NVDM_MODEM_PORT_MINIDUMP_REGION_PEB_COUNT;
        return NVDM_MODEM_PORT_MINIDUMP_REGION_PEB_COUNT/NVDM_MODEM_PEB_NUMBER_PER_MINIDUMP;
    }
#endif
    else {
        *peb_count = 0;
    }
    return NVDM_MODEM_PORT_PEB_SIZE;
}


uint32_t nvdm_modem_port_get_peb_address(int32_t pnum, int32_t offset)
{
    nvdm_modem_area_t area = nvdm_modem_port_get_working_area(true);

    if (area == NVDM_MODEM_AREA_NORMAL) {
        return NVDM_MODEM_PORT_NORMAL_REGION_ADDRESS(pnum, offset);
    } else if (area == NVDM_MODEM_AREA_PROTECTED) {
        return NVDM_MODEM_PORT_PROTECTED_REGION_ADDRESS(pnum, offset);
    } else if (area == NVDM_MODEM_AREA_BACKUP) {
        return NVDM_MODEM_PORT_BACKUP_REGION_ADDRESS(pnum, offset);
    }
#ifdef MTK_MINI_DUMP_ENABLE
    else if (area == NVDM_MODEM_AREA_MINIDUMP) {
        return NVDM_MODEM_PORT_MINIDUMP_REGION_ADDRESS(pnum, offset);
    }
#endif
    return 0;
}

nvdm_modem_area_t nvdm_modem_port_get_working_area(bool examine)
{
    if (examine)
        configASSERT(g_nvdm_modem_working_area != NVDM_MODEM_AREA_NONE)
    return g_nvdm_modem_working_area;
}

void nvdm_modem_port_set_working_area(nvdm_modem_area_t area)
{
    g_nvdm_modem_working_area = area;
}

void nvdm_modem_port_poweroff_time_set(void)
{}

void nvdm_modem_port_poweroff(uint32_t poweroff_time)
{}


void nvdm_modem_port_ramdisk_read(uint32_t address, uint8_t *buffer, uint32_t length)
{
    uint32_t ram_addr = NVDM_MODEM_PORT_FLASH_TO_RAMDISK_ADDRESS(address);

    memcpy(buffer, (void *)ram_addr, length);
}

void nvdm_modem_port_ramdisk_write(uint32_t address, const uint8_t *buffer, uint32_t length)
{
    uint32_t ram_addr = NVDM_MODEM_PORT_FLASH_TO_RAMDISK_ADDRESS(address);

    nvdm_modem_port_flash_write(address, buffer, length);

    memcpy((void *)ram_addr, buffer, length);
}

void nvdm_modem_port_ramdisk_erase(uint32_t address)
{
    uint32_t ram_addr = NVDM_MODEM_PORT_FLASH_TO_RAMDISK_ADDRESS(address);

    nvdm_modem_port_flash_erase(address);

    memset((void *)ram_addr, 0xFF, NVDM_MODEM_PORT_PEB_SIZE);
}

static void nvdm_modem_port_set_peb_ramdisk(uint32_t pnum)
{
    static uint32_t priv_addr = 0;
    uint32_t addr = nvdm_modem_port_get_peb_address(pnum, 0);

    if (!g_prRamdisk_base) {
        priv_addr = 0;
        return;
    }

    if (priv_addr != addr) {
        nvdm_modem_port_flash_read(addr, g_prRamdisk_base, NVDM_MODEM_PORT_PEB_SIZE);
        priv_addr = addr;
    }
}

static void nvdm_modem_port_set_flash_access()
{
    if (g_prRamdisk_base){
        nvdm_modem_port_free(g_prRamdisk_base);
        g_prRamdisk_base = NULL;
    }
    g_mem_access.mem_read  = nvdm_modem_port_flash_read;
    g_mem_access.mem_write = nvdm_modem_port_flash_write;
    g_mem_access.mem_erase = nvdm_modem_port_flash_erase;

    nvdm_modem_port_set_peb_ramdisk(0);
}

static void nvdm_modem_port_set_ramdisk_access()
{
    if (!g_prRamdisk_base) {
        g_prRamdisk_base = nvdm_modem_port_malloc(NVDM_MODEM_PORT_PEB_SIZE);
    }

    if (g_prRamdisk_base) {
        g_mem_access.mem_read  = nvdm_modem_port_ramdisk_read;
        g_mem_access.mem_write = nvdm_modem_port_ramdisk_write;
        g_mem_access.mem_erase = nvdm_modem_port_ramdisk_erase;
    } else {
        nvdm_modem_port_log_notice("memroy allocation for ramdisk failed");
    }
}

void nvdm_modem_port_mem_read(uint32_t address, uint8_t *buffer, uint32_t length)
{
    g_mem_access.mem_read(address, buffer, length);
}

void nvdm_modem_port_mem_write(uint32_t address, const uint8_t *buffer, uint32_t length)
{
    g_mem_access.mem_write(address, buffer, length);
}

void nvdm_modem_port_mem_erase(uint32_t address)
{
    g_mem_access.mem_erase(address);
}

void nvdm_modem_port_mem_control(mem_control_cmd_t cmd, uint32_t para)
{
    switch (cmd) {
        case NVDM_MODEM_MEM_SET_CONTROL_TYPE:
            if (0x0 == para) {
                nvdm_modem_port_set_flash_access();
            } else if (0x1 == para) {
                nvdm_modem_port_set_ramdisk_access();
            }
            break;
        case NVDM_MODEM_MEM_SET_PEB_RAMDISK:
            nvdm_modem_port_set_peb_ramdisk(para);
        default:
            return;
    }
}
#endif

