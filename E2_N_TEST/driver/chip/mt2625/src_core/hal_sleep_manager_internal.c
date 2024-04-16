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

#include "hal_sleep_manager.h"
#include "hal_sleep_manager_internal.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef HAL_SLEEP_MANAGER_ENABLED
#include <stdio.h>
#include <string.h>
#include "hal_log.h"
#include "memory_attribute.h"
#include "hal_nvic.h"
#include "hal_nvic_internal.h"
#include "assert.h"
#include "hal_emi_internal.h"
#include "hal_cache_internal.h"
#include "hal_flash_sf.h"
#include "hal_clock_internal.h"
#include "hal_gpt.h"
#include "hal_mpu_internal.h"
#include "hal_clock.h"
#include "hal_gpio.h"
#ifdef MTK_NVDM_MODEM_ENABLE
#include "nvdm.h"
#include "memory_map.h"
#endif

#ifdef  SLEEP_MANAGEMENT_DEBUG_ENABLE
#define log_debug(_message,...) printf(_message, ##__VA_ARGS__)
#else
#define log_debug(_message,...)
#endif

#define SLEEP_MANAGEMENT_DEEPSLEEP_TEST 0

static sleep_management_handle_t sleep_management_handle = {
    .lock_sleep_all_low = 0,
    .lock_sleep_all_high = 0,
    .lock_sleep_deep_low = 0,
    .lock_sleep_deep_high = 0,
    .user_handle_resoure_low = 0,
    .user_handle_resoure_high = 0,
    .user_handle_count = 0
};

static sleep_management_suspend_callback_func_t    suspend_callback_func_table      [SLEEP_BACKUP_RESTORE_MODULE_MAX];
static sleep_management_resume_callback_func_t     resume_callback_func_table       [SLEEP_BACKUP_RESTORE_MODULE_MAX];
static sleep_management_suspend_callback_func_t    suspend_user_callback_func_table [SLEEP_BACKUP_RESTORE_USER_CALLBACK_FUNC_MAX];
static sleep_management_resume_callback_func_t     resume_user_callback_func_table  [SLEEP_BACKUP_RESTORE_USER_CALLBACK_FUNC_MAX];
static uint32_t suspend_user_register_count = 0, resume_user_register_count = 0;

ATTR_RWDATA_IN_TCM volatile uint32_t wakeup_source_status;
ATTR_RWDATA_IN_TCM volatile uint32_t Vector0_backup, temp_reg;
ATTR_RWDATA_IN_TCM volatile uint32_t origin_msp_bak_reg, origin_psp_bak_reg, backup_return_address;
ATTR_RWDATA_IN_TCM volatile CPU_CORE_BAKEUP_REG_T cpu_core_reg;
ATTR_RWDATA_IN_TCM volatile nvic_sleep_backup_register_t nvic_backup_register;
ATTR_RWDATA_IN_TCM volatile FPU_BAKEUP_REG_T fpu_reg;
ATTR_RWDATA_IN_TCM volatile CMSYS_CFG_BAKEUP_REG_T cmsys_cfg_reg;
ATTR_RWDATA_IN_TCM volatile CMSYS_CFG_EXT_BAKEUP_REG_T cmsys_cfg_ext_reg;
ATTR_RWDATA_IN_TCM volatile CM4_SYS_CTRL_BAKEUP_REG_T cm4_sys_ctrl_reg;
uint32_t cli_dtim_sleep_mode = 0;

static hal_sleep_mode_t sleep_mode = HAL_SLEEP_MODE_IDLE;

ATTR_RWDATA_IN_RETSRAM uint32_t SystickBak;    //please do not set init value

extern uint32_t rtc_get_banks_in_active(void);
extern void uart_backup_all_registers(void);
extern void uart_restore_all_registers(void);

#if     defined (__GNUC__)      //GCC disable compiler optimize
__attribute__((optimize("O0")))
#elif   defined (__ICCARM__)    //IAR disable compiler optimize
#pragma optimize=none
#elif   defined (__CC_ARM)      //MDK disable compiler optimize
#pragma push
#pragma diag_suppress 1267
#pragma O0
#endif
ATTR_TEXT_IN_TCM void sleep_management_enter_light_sleep(void)
{
#if defined (__CC_ARM)
    /* Backup function return address(R14) */
    __asm volatile("mov backup_return_address,__return_address() \n");
#endif
    hal_clock_enable(HAL_CLOCK_CG_SPM);

    #ifdef SLEEP_MANAGEMENT_DEBUG_ENABLE
    if ((*SPM_PWR_STATUS & 0x4) == 0) {
        log_debug("[Sleep]MTCMOS CONN:Off\r\n");
    } else {
        log_debug("[Sleep]MTCMOS CONN:On\r\n");
    }
    if ((*SPM_PWR_STATUS & 0x8) == 0) {
        log_debug("[Sleep]MTCMOS SDIO:Off\r\n");
    } else {
        log_debug("[Sleep]MTCMOS SDIO:On\r\n");
    }
    log_debug("[Sleep]Enter Light Sleep\r\n");
    #endif
    //clock_suspend();

    /* Set Boot Slave */
    *CMCFG_BOOT_FROM_SLV = 0x1;

    /* peripheral driver backup callback function */
    sleep_management_suspend_callback(HAL_SLEEP_MODE_LIGHT_SLEEP);

    /* backup UART setting */
    uart_backup_all_registers();

    *SPM_PCM_CON1 = ( *SPM_PCM_CON1 | 0xB2160000)  | 0x1<<5;
    *SPM_PCM_TIMER_VAL  = 0x180;
    /* spm Kick start*/
    //*SPM_MONITOR_WAKEUP_EVENT_EN =0x1;
    *SPM_PCM_RESERVE = 0x800000; //MT7686 connsys

    /* enable spm irq and clear pending bits */
    NVIC_EnableIRQ(SPM_IRQn);
    NVIC_ClearPendingIRQ(SPM_IRQn);

    /* backup cmsys register */
    deep_sleep_cmsys_backup();

    /* general register backup */
    __CPU_STACK_POINT_BACKUP(origin_psp_bak_reg, origin_msp_bak_reg);

    /* Set CM4 SLEEPDEEP bits */
    *CM4_SYSTEM_CONTROL = *CM4_SYSTEM_CONTROL | 0x4;

#if SLEEP_MANAGEMENT_SLEEP_DUMP_PMU
    sleep_management_light_sleep_pmu_get();
#endif

    /* Flash Enter Powerdown Mode */
#ifdef HAL_FLASH_MODULE_ENABLED
    SF_DAL_DEV_Enter_DPD();
#endif

    /* PSRAM Enter Sleep Mode */
    mtk_psram_half_sleep_enter();

    /* backup BootVector0 Stack Address */
    Vector0_backup = *CMCFG_BOOT_VECTOR0;   //boot vector 0(boot slave stack point   // *SPM_EVENT_CONTROL_0 = 0x1000000;)

    /* backup MSP Address */
#if (defined (__GNUC__) || defined (__ICCARM__))
    __asm volatile("push {r0-r12, lr}");
    __asm volatile("mov %0, sp" : "=r"(temp_reg));
#elif defined (__CC_ARM)
    __PUSH_CPU_REG();
    __BACKUP_SP(temp_reg);
#endif
    *CMCFG_BOOT_VECTOR0 = temp_reg;  //VECTOR0 write MSP Address

    if (*SPM_SLEEP_ISR_STATUS == 1) {
        *SPM_PCM_CON0 = *SPM_PCM_CON0 & 0x1E; // [0]: kick PCM process
        *SPM_SLEEP_ISR_STATUS = 1;
        *SPM_SLEEP_ISR_STATUS = 0;
    } else {
        /* Enter Deep Sleep */
        temp_reg = CMCFG_BOOT_VECTOR1;  //CMCFG_BOOT_VECTOR1 Address
        __ENTER_LIGHT_SLEEP(temp_reg);
        /* CMSYS Peripheral : make virtual space available */
        *((volatile uint32_t*)0xE0181000) = 0x10000023;
        *((volatile uint32_t*)0xE0181004) = 0x0;
        __POP_CPU_REG();
    }

    /* wait protect_en release */
    while(((*(volatile uint32_t*)(0xA21F001C)) & 0x1000100) != 0);

    /* add 2us delay for protect_en ready (40Mhz Xtal) */
    hal_gpt_delay_us(2);

    /* PSRAM Leave Sleep Mode */
    mtk_psram_half_sleep_exit();

    /* Flash Leave Powerdown Mode */
#ifdef HAL_FLASH_MODULE_ENABLED
    SF_DAL_DEV_Leave_DPD();
#endif
    /* get wakeup source */
    wakeup_source_status = *SPM_WAKEUP_SOURCE_STA;

    /* restore MSP */
    temp_reg = (unsigned int)&origin_msp_bak_reg;
    __MSP_RESTORE(temp_reg);

    /* swtich stack point to psp */
    __SWITCH_TO_PSP_STACK_POINT();

    /* restore PSP */
    temp_reg = (unsigned int)&origin_psp_bak_reg;
    __PSP_RESTORE(temp_reg);

    /* restore Core register - CONTROL */
    temp_reg = (unsigned int)&cpu_core_reg.CONTROL;
    __CPU_CORE_CONTROL_REG_RESTORE(temp_reg);
    /* restore boot Vector */
    *CMCFG_BOOT_FROM_SLV = 0x0;
    *CMCFG_BOOT_VECTOR0 = Vector0_backup ;

    /* Clear CM4 Deep Sleep bits */
    *CM4_SYSTEM_CONTROL = *CM4_SYSTEM_CONTROL & (~0x4);

    /* restore cmsys register */
    deep_sleep_cmsys_restore();

    /* disable spm irq and clear pending bits */
    NVIC_DisableIRQ(SPM_IRQn);
    NVIC_ClearPendingIRQ(SPM_IRQn);
    //clock_resume();

    /* resume UART first */
    uart_restore_all_registers();

    /* peripheral driver restore callback function */
    sleep_management_resume_callback(HAL_SLEEP_MODE_LIGHT_SLEEP);

    /* disable spm clock*/
    //hal_clock_disable(HAL_CLOCK_CG_SW_SPM);
#ifdef  SLEEP_MANAGEMENT_DEBUG_ENABLE
    sleep_management_dump_wakeup_source(wakeup_source_status);
#endif

#if defined (__CC_ARM)
    __RESTORE_LR(backup_return_address);
#endif
}
#if     defined (__GNUC__)
#elif   defined (__CC_ARM)
#pragma pop
#endif

ATTR_TEXT_IN_TCM inline void deep_sleep_cmsys_backup()
{
    uint32_t i;
    /* backup CPU core registers */
    temp_reg = (unsigned int)&cpu_core_reg;
    __CPU_CORE_REG_BACKUP(temp_reg);

    /* NVIC backup */
    nvic_backup_register.nvic_iser = NVIC->ISER[0];
    nvic_backup_register.nvic_iser1 = NVIC->ISER[1];
    nvic_backup_register.nvic_iser2 = NVIC->ISER[2];
    for (i = 0; i < SAVE_PRIORITY_GROUP; i++) {
        nvic_backup_register.nvic_ip[i] = NVIC->IP[i];
    }

    /* cache backcp */
#ifdef HAL_CACHE_MODULE_ENABLED
    cache_status_save();
#endif

    /* mpu backcp */
#ifdef HAL_MPU_MODULE_ENABLED
    mpu_status_save();
#endif

#if 0
    /* cmsys config backup */
    cmsys_cfg_reg.STCALIB = CMSYS_CFG->STCALIB;
    cmsys_cfg_reg.AHB_BUFFERALBE = CMSYS_CFG->AHB_BUFFERALBE;
    cmsys_cfg_reg.AHB_FIFO_TH = CMSYS_CFG->AHB_FIFO_TH;
    cmsys_cfg_reg.INT_ACTIVE_HL0 = CMSYS_CFG->INT_ACTIVE_HL0;
    cmsys_cfg_reg.INT_ACTIVE_HL1 = CMSYS_CFG->INT_ACTIVE_HL1;
    cmsys_cfg_reg.DCM_CTRL_REG = CMSYS_CFG->DCM_CTRL_REG;

    cmsys_cfg_ext_reg.CG_EN = CMSYS_CFG_EXT->CG_EN;
    cmsys_cfg_ext_reg.DCM_EN = CMSYS_CFG_EXT->DCM_EN;
#endif

    /* fpu backup */
    fpu_reg.FPCCR = FPU->FPCCR;
    fpu_reg.FPCAR = FPU->FPCAR;
    /* CM4 system control registers backup */
    cm4_sys_ctrl_reg.ACTLR = SCnSCB->ACTLR;
    cm4_sys_ctrl_reg.VTOR = SCB->VTOR;
    cm4_sys_ctrl_reg.SCR = SCB->SCR;
    cm4_sys_ctrl_reg.CCR = SCB->CCR;

    cm4_sys_ctrl_reg.SHP[0] = SCB->SHP[0]; /* MemMange */
    cm4_sys_ctrl_reg.SHP[1] = SCB->SHP[1]; /* BusFault */
    cm4_sys_ctrl_reg.SHP[2] = SCB->SHP[2]; /* UsageFault */
    cm4_sys_ctrl_reg.SHP[7] = SCB->SHP[7]; /* SVCall */
    cm4_sys_ctrl_reg.SHP[8] = SCB->SHP[8]; /* DebugMonitor */
    cm4_sys_ctrl_reg.SHP[10] = SCB->SHP[10]; /* PendSV */
    cm4_sys_ctrl_reg.SHP[11] = SCB->SHP[11]; /* SysTick */

    cm4_sys_ctrl_reg.SHCSR = SCB->SHCSR;
    cm4_sys_ctrl_reg.CPACR = SCB->CPACR;
}

ATTR_TEXT_IN_TCM inline void deep_sleep_cmsys_restore()
{
    uint32_t i;

    /* CM4 system control registers restore */
    SCnSCB->ACTLR = cm4_sys_ctrl_reg.ACTLR;
    SCB->VTOR = cm4_sys_ctrl_reg.VTOR;
    SCB->SCR = cm4_sys_ctrl_reg.SCR;
    SCB->CCR = cm4_sys_ctrl_reg.CCR;
    SCB->SHP[0] = cm4_sys_ctrl_reg.SHP[0]; /* MemMange */
    SCB->SHP[1] = cm4_sys_ctrl_reg.SHP[1]; /* BusFault */
    SCB->SHP[2] = cm4_sys_ctrl_reg.SHP[2]; /* UsageFault */
    SCB->SHP[7] = cm4_sys_ctrl_reg.SHP[7]; /* SVCall */
    SCB->SHP[8] = cm4_sys_ctrl_reg.SHP[8]; /* DebugMonitor */
    SCB->SHP[10] = cm4_sys_ctrl_reg.SHP[10]; /* PendSV */
    SCB->SHP[11] = cm4_sys_ctrl_reg.SHP[11]; /* SysTick */
    SCB->SHCSR = cm4_sys_ctrl_reg.SHCSR;
    SCB->CPACR = cm4_sys_ctrl_reg.CPACR;

    /* fpu restore */
    FPU->FPCCR = fpu_reg.FPCCR;
    FPU->FPCAR = fpu_reg.FPCAR;

#if 0
    /* cmsys config restore */
    CMSYS_CFG->STCALIB = cmsys_cfg_reg.STCALIB;
    CMSYS_CFG->AHB_BUFFERALBE = cmsys_cfg_reg.AHB_BUFFERALBE;
    CMSYS_CFG->AHB_FIFO_TH = cmsys_cfg_reg.AHB_FIFO_TH;
    CMSYS_CFG->INT_ACTIVE_HL0 = cmsys_cfg_reg.INT_ACTIVE_HL0;
    CMSYS_CFG->INT_ACTIVE_HL1 = cmsys_cfg_reg.INT_ACTIVE_HL1;
    CMSYS_CFG->DCM_CTRL_REG = cmsys_cfg_reg.DCM_CTRL_REG;

    CMSYS_CFG_EXT->CG_EN = cmsys_cfg_ext_reg.CG_EN;
    CMSYS_CFG_EXT->DCM_EN = cmsys_cfg_ext_reg.DCM_EN;
#endif

    /* mpu restore */
#ifdef HAL_MPU_MODULE_ENABLED
    mpu_status_restore();
#endif

    /* cache restore */
#ifdef HAL_CACHE_MODULE_ENABLED
    cache_status_restore();
#endif

    /* restore CPU core registers */
    temp_reg = (unsigned int)&cpu_core_reg;
    __CPU_CORE_REG_RESTORE(temp_reg);

    /* NVIC restore */
    NVIC->ISER[0] = nvic_backup_register.nvic_iser;
    NVIC->ISER[1] = nvic_backup_register.nvic_iser1;
    NVIC->ISER[2] = nvic_backup_register.nvic_iser2;
    for (i = 0; i < SAVE_PRIORITY_GROUP; i++) {
        NVIC->IP[i] = nvic_backup_register.nvic_ip[i];
    }
}

void sleep_management_register_suspend_callback(sleep_management_backup_restore_module_t module, sleep_management_suspend_callback_t callback, void *data, uint32_t mode)
{
    if (module == SLEEP_BACKUP_RESTORE_USER) {
        if (suspend_user_register_count < SLEEP_BACKUP_RESTORE_USER_CALLBACK_FUNC_MAX) {
            suspend_user_callback_func_table[suspend_user_register_count].func        = callback;
            suspend_user_callback_func_table[suspend_user_register_count].data        = data;
            suspend_user_callback_func_table[suspend_user_register_count].init_status = SLEEP_MANAGEMENT_INITIALIZED;
            suspend_user_callback_func_table[suspend_user_register_count].mode        = mode;
            suspend_user_register_count++;
        } else {
            log_hal_error("[Sleep Management]register suspend callback function overflow\r\n");
            assert(0);
        }
    } else {
        suspend_callback_func_table[module].func        = callback;
        suspend_callback_func_table[module].data        = data;
        suspend_callback_func_table[module].init_status = SLEEP_MANAGEMENT_INITIALIZED;
        suspend_callback_func_table[module].mode        = mode;
    }
}

void sleep_management_register_resume_callback(sleep_management_backup_restore_module_t module, sleep_management_resume_callback_t callback, void *data, uint32_t mode)
{
    if (module == SLEEP_BACKUP_RESTORE_USER) {
        if (resume_user_register_count < SLEEP_BACKUP_RESTORE_USER_CALLBACK_FUNC_MAX) {
            resume_user_callback_func_table[resume_user_register_count].func        = callback;
            resume_user_callback_func_table[resume_user_register_count].data        = data;
            resume_user_callback_func_table[resume_user_register_count].init_status = SLEEP_MANAGEMENT_INITIALIZED;
            resume_user_callback_func_table[resume_user_register_count].mode        = mode;
            resume_user_register_count++;
        } else {
            log_hal_error("[Sleep Management]register resume callback function overflow\r\n");
            assert(0);
        }
    } else {
        resume_callback_func_table[module].func        = callback;
        resume_callback_func_table[module].data        = data;
        resume_callback_func_table[module].init_status = SLEEP_MANAGEMENT_INITIALIZED;
        resume_callback_func_table[module].mode        = mode;
    }
}

ATTR_TEXT_IN_TCM void sleep_management_suspend_callback(uint32_t mode)
{
    static uint32_t i;

    for (i = 0; i < SLEEP_BACKUP_RESTORE_MODULE_MAX; i++) {
        if (suspend_callback_func_table[i].init_status == SLEEP_MANAGEMENT_INITIALIZED) {
            if (suspend_callback_func_table[i].mode & mode ) {
#ifdef  SLEEP_MANAGEMENT_DEBUG_ENABLE
                sleep_management_debug_backup_restore_fun_timelog(0, 0, i);
#endif
                suspend_callback_func_table[i].func(suspend_callback_func_table[i].data,mode);
#ifdef  SLEEP_MANAGEMENT_DEBUG_ENABLE
                sleep_management_debug_backup_restore_fun_timelog(0, 1, i);
#endif
            }
        }
    }

    for (i = 0; i < SLEEP_BACKUP_RESTORE_USER_CALLBACK_FUNC_MAX; i++) {
        if (suspend_user_callback_func_table[i].init_status == SLEEP_MANAGEMENT_INITIALIZED) {
            if (suspend_user_callback_func_table[i].mode & mode) {
#ifdef  SLEEP_MANAGEMENT_DEBUG_ENABLE
                sleep_management_debug_backup_restore_fun_timelog(1, 0, i+SLEEP_BACKUP_RESTORE_MODULE_MAX);
#endif
                suspend_user_callback_func_table[i].func(suspend_user_callback_func_table[i].data,mode);
#ifdef  SLEEP_MANAGEMENT_DEBUG_ENABLE
                sleep_management_debug_backup_restore_fun_timelog(1, 1, i+SLEEP_BACKUP_RESTORE_MODULE_MAX);
#endif
            }
        }
    }
}


ATTR_TEXT_IN_TCM void sleep_management_resume_callback(uint32_t mode)
{
    static uint32_t i;

    for (i = 0; i < SLEEP_BACKUP_RESTORE_MODULE_MAX; i++) {
        if (resume_callback_func_table[i].init_status == SLEEP_MANAGEMENT_INITIALIZED) {

#ifdef  SLEEP_MANAGEMENT_DEBUG_ENABLE
            sleep_management_debug_backup_restore_fun_timelog(1, 0, i);
#endif

            resume_callback_func_table[i].func(resume_callback_func_table[i].data,mode);

#ifdef  SLEEP_MANAGEMENT_DEBUG_ENABLE
            sleep_management_debug_backup_restore_fun_timelog(1, 1, i);
#endif
        }
    }
    for (i = 0; i < SLEEP_BACKUP_RESTORE_USER_CALLBACK_FUNC_MAX; i++) {
            if (resume_user_callback_func_table[i].init_status == SLEEP_MANAGEMENT_INITIALIZED) {

    #ifdef  SLEEP_MANAGEMENT_DEBUG_ENABLE
                sleep_management_debug_backup_restore_fun_timelog(1, 0, i+SLEEP_BACKUP_RESTORE_MODULE_MAX);
    #endif

                resume_user_callback_func_table[i].func(resume_user_callback_func_table[i].data,mode);

    #ifdef  SLEEP_MANAGEMENT_DEBUG_ENABLE
                sleep_management_debug_backup_restore_fun_timelog(1, 1, i+SLEEP_BACKUP_RESTORE_MODULE_MAX);
    #endif
            }
        }
}

void sleep_managerment_backup_rtcram(void)
{
#ifdef MTK_NVDM_MODEM_ENABLE
    char item_name[50];
    uint8_t i = 0;
    nvdm_status_t status;
    uint32_t rtc_avtive_banks = rtc_get_banks_in_active();

    // copy RTC RAM content to NVDM
    // there are 4 banks in RTC RAM
    for (i = 0; i < 4; i++) {
        if (rtc_avtive_banks & (1 << i)) {
            // seperate 1 bank to 2 flash write operation for better flash life
            sprintf(item_name, "backup_data_%d_0", i);
            status = nvdm_write_data_item("deeper_sleep",
                                          item_name,
                                          NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                          (uint8_t *)(RETSRAM_BASE + (i * 0x800)),
                                          0x400);
            if (status != NVDM_STATUS_OK) {
                printf("Backup to NVDM fail(%d)\r\n", status);
                assert(0);
            }

            sprintf(item_name, "backup_data_%d_1", i);
            status = nvdm_write_data_item("deeper_sleep",
                                          item_name,
                                          NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                          (uint8_t *)(RETSRAM_BASE + (i * 0x800) + 0x400),
                                          0x400);
            if (status != NVDM_STATUS_OK) {
                printf("Backup to NVDM fail(%d)\r\n", status);
                assert(0);
            }
        }
    }
#endif
}

void sleep_managerment_restore_rtcram(void)
{
#ifdef MTK_NVDM_MODEM_ENABLE
    char item_name[50];
    uint8_t i = 0;
    nvdm_status_t status;
    uint32_t rtc_avtive_banks = rtc_get_banks_in_active();
    uint32_t len;

    for (i = 0; i < 4; i++) {
        len = 0x400;
        if (rtc_avtive_banks & (1 << i)) {
            sprintf(item_name, "backup_data_%d_0", i);
            status = nvdm_read_data_item("deeper_sleep",
                                         item_name,
                                         (uint8_t *)(RETSRAM_BASE + (i * 0x800)),
                                         &len);
            if (status != NVDM_STATUS_OK) {
                printf("Restore to RTC Ram fail(%d)\r\n", status);
                assert(0);
            }

            len = 0x400;
            sprintf(item_name, "backup_data_%d_1", i);
            status = nvdm_read_data_item("deeper_sleep",
                                         item_name,
                                         (uint8_t *)(RETSRAM_BASE + (i * 0x800) + 0x400),
                                         &len);
            if (status != NVDM_STATUS_OK) {
                printf("Restore to RTC Ram fail(%d)\r\n", status);
                assert(0);
            }
        }
    }
#endif
}

uint8_t sleep_management_get_lock_handle(const char *handle_name)
{
    uint8_t index = 0, i;
    uint32_t mask, name_len;

    // check handle name valid
    name_len = strlen(handle_name);
    if (name_len >= SLEEP_HANDLE_NAME_LEN) {
        name_len = SLEEP_HANDLE_NAME_LEN-1;
    }

    if (name_len == 0) {
        log_hal_error("[Sleep Management]sleep handle name error\r\n");
        assert(0);
    }
    for (i = 0; i < name_len; i++) {
        if ((*(handle_name+i) <= 0x20) || (*(handle_name+i) >= 0x7E)) {
            log_hal_error("[Sleep Management]sleep handle name error\r\n");
            assert(0);
        }
    }

    mask = save_and_set_interrupt_mask();
    for (index = 0 ; index < SLEEP_LOCK_HANDLE_USER_MAX; index++) {
        if (index < 32) {
            if (((sleep_management_handle.user_handle_resoure_low >> index) & 0x01) == 0) {
                sleep_management_handle.user_handle_resoure_low |= (1 << index);
                sleep_management_handle.user_handle_count++;

                memset(&sleep_management_handle.user_handle_name[index][0], 0, SLEEP_HANDLE_NAME_LEN);
                memcpy(&sleep_management_handle.user_handle_name[index][0], handle_name, name_len);
                break;
            }
        } else {
            if (((sleep_management_handle.user_handle_resoure_high >> (index-32)) & 0x01) == 0) {
                sleep_management_handle.user_handle_resoure_high |= (1 << (index-32));
                sleep_management_handle.user_handle_count++;

                memset(&sleep_management_handle.user_handle_name[index][0], 0, SLEEP_HANDLE_NAME_LEN);
                memcpy(&sleep_management_handle.user_handle_name[index][0], handle_name, name_len);
                break;
            }
        }
    }
    restore_interrupt_mask(mask);

    if (index >= SLEEP_LOCK_HANDLE_USER_MAX) {
        log_hal_error("[Sleep Management]cannot get sleep handle for %s\r\n", &sleep_management_handle.user_handle_name[index][0]);
        assert(0);
        return (SLEEP_LOCK_INVALID_ID);
    }

    log_hal_info("[Sleep Management]sleep handle[%d] name: %s\r\n", index+SLEEP_LOCK_USER_START_ID, &sleep_management_handle.user_handle_name[index][0]);
    index += SLEEP_LOCK_USER_START_ID;

    return (index);
}

void sleep_management_release_lock_handle(uint8_t handle_index)
{
    uint32_t mask;
    /*  check handle index range */
    if ((handle_index >= SLEEP_LOCK_HANDLE_MAX) || (handle_index < SLEEP_LOCK_USER_START_ID)) {
        log_hal_error("[Sleep Management]sleep handle index %d error\r\n", handle_index);
        return;
    }

    handle_index -= SLEEP_LOCK_USER_START_ID;

    mask = save_and_set_interrupt_mask();
    if (handle_index < 32) {
        if (((sleep_management_handle.user_handle_resoure_low >> handle_index) & 0x01) == 1) {
            sleep_management_handle.user_handle_count--;
            sleep_management_handle.user_handle_resoure_low &= ~(1 << handle_index);
            memset(&sleep_management_handle.user_handle_name[handle_index][0], 0, SLEEP_HANDLE_NAME_LEN);
        } else {
            log_hal_warning("[Sleep Management]sleep handle %d already release \r\n", handle_index);
        }
    } else {
        if (((sleep_management_handle.user_handle_resoure_high >> (handle_index-32)) & 0x01) == 1) {
            sleep_management_handle.user_handle_count--;
            sleep_management_handle.user_handle_resoure_high &= ~(1 << (handle_index-32));
            memset(&sleep_management_handle.user_handle_name[handle_index][0], 0, SLEEP_HANDLE_NAME_LEN);
        } else {
            log_hal_warning("[Sleep Management]sleep handle %d already release \r\n", handle_index);
        }
    }
    restore_interrupt_mask(mask);
}

void sleep_management_lock_sleep(sleep_management_lock_sleep_t lock, uint8_t handle_index)
{
    uint32_t mask;

    if (handle_index >= SLEEP_LOCK_HANDLE_MAX) {
        log_hal_error("[Sleep Management]sleep handle index error\r\n");
        return;
    }

    if (lock == LOCK_SLEEP) {
        /* Lock sleep request */
        mask = save_and_set_interrupt_mask();
        sleep_management_handle.lock_sleep_request_count[handle_index]++;
        if (handle_index >= 32) {
            sleep_management_handle.lock_sleep_all_high |= (1 << (handle_index - 32));
        } else {
            sleep_management_handle.lock_sleep_all_low |= (1 << handle_index);
        }
        restore_interrupt_mask(mask);
#ifdef  SLEEP_MANAGEMENT_DEBUG_ENABLE
        sleep_management_debug_lock_sleep_timelog(lock, handle_index);
#endif
        if (sleep_management_handle.lock_sleep_request_count[handle_index] == 0xFF) {
            log_hal_warning("[Sleep Management]sleep handle=%d,lock sleep count full \r\n", handle_index);
        }
    } else {
        /* Unlock sleep request */
        if (sleep_management_handle.lock_sleep_request_count[handle_index] > 0) {
            sleep_management_handle.lock_sleep_request_count[handle_index]--;
            if (sleep_management_handle.lock_sleep_request_count[handle_index] == 0) {
                if (handle_index >= 32) {
                    sleep_management_handle.lock_sleep_all_high &= ~(1 << (handle_index - 32));
                } else {
                    sleep_management_handle.lock_sleep_all_low &= ~(1 << handle_index);
                }
#ifdef  SLEEP_MANAGEMENT_DEBUG_ENABLE
                sleep_management_debug_lock_sleep_timelog(lock, handle_index);
#endif
            }
        } else {
            if (handle_index < SLEEP_LOCK_USER_START_ID) {
                log_hal_warning("[Sleep Management]sleep handle=%d, lock sleep has already released\r\n", handle_index);
            } else {
                log_hal_warning("[Sleep Management]sleep handle=%d,%s, lock sleep has already released\r\n", handle_index, (char *)&sleep_management_handle.user_handle_name[handle_index-SLEEP_LOCK_USER_START_ID]);
            }
        }
    }
}

uint32_t sleep_management_sleeplock_control(uint8_t handle_index, hal_sleep_lock_t level, sleep_management_lock_sleep_t control)
{
    uint32_t mask;
    uint32_t err = 0;

    if (handle_index >= SLEEP_LOCK_HANDLE_MAX) {
        log_hal_error("[Sleep Management]sleep handle index error\r\n");
        return 0xFF;
    }

#ifdef  SLEEP_MANAGEMENT_DEBUG_ENABLE
    sleep_management_debug_lock_sleep_timelog(control, handle_index);
#endif

    mask = save_and_set_interrupt_mask();
    if (control == LOCK_SLEEP) {
        /* Lock sleep request */
        if (level == HAL_SLEEP_LOCK_ALL) {
            if (handle_index < 32) {
                if (sleep_management_handle.lock_sleep_deep_low & (1 << handle_index)) {
                    err = 1;
                } else {
                    sleep_management_handle.lock_sleep_all_low |= (1 << handle_index);
                    sleep_management_handle.lock_sleep_request_count[handle_index]++;
                }
            } else {
                if (sleep_management_handle.lock_sleep_deep_high & (1 << (handle_index-32))) {
                    err = 1;
                } else {
                    sleep_management_handle.lock_sleep_all_high |= (1 << (handle_index-32));
                    sleep_management_handle.lock_sleep_request_count[handle_index]++;
                }
            }
        } else if (level == HAL_SLEEP_LOCK_DEEP) {
            if (handle_index < 32) {
                if (sleep_management_handle.lock_sleep_all_low & (1 << handle_index)) {
                    err = 2;
                } else {
                    sleep_management_handle.lock_sleep_deep_low |= (1 << handle_index);
                    sleep_management_handle.lock_sleep_request_count[handle_index]++;
                }
            } else {
                if (sleep_management_handle.lock_sleep_all_high & (1 << (handle_index-32))) {
                    err = 2;
                } else {
                    sleep_management_handle.lock_sleep_deep_high |= (1 << (handle_index-32));
                    sleep_management_handle.lock_sleep_request_count[handle_index]++;
                }
            }
        } else {
            err = 3;
        }

        if (sleep_management_handle.lock_sleep_request_count[handle_index] == 0xFF) {
            if (handle_index < SLEEP_LOCK_USER_START_ID) {
                err = 4;
            } else {
                err = 5;
            }
        }
    } else {
        /* Unlock sleep request */
        if (sleep_management_handle.lock_sleep_request_count[handle_index] > 0) {
            sleep_management_handle.lock_sleep_request_count[handle_index]--;
            if (sleep_management_handle.lock_sleep_request_count[handle_index] == 0) {
                if (level == HAL_SLEEP_LOCK_ALL) {
                    if (handle_index >= 32) {
                        sleep_management_handle.lock_sleep_all_high &= ~(1 << (handle_index-32));
                    } else {
                        sleep_management_handle.lock_sleep_all_low &= ~(1 << handle_index);
                    }
                } else if (level == HAL_SLEEP_LOCK_DEEP) {
                    if (handle_index >= 32) {
                        sleep_management_handle.lock_sleep_deep_high &= ~(1 << (handle_index-32));
                    } else {
                        sleep_management_handle.lock_sleep_deep_low &= ~(1 << handle_index);
                    }
                } else {
                    err = 3;
                    sleep_management_handle.lock_sleep_request_count[handle_index]++;
                }
            }
        } else {
            if (handle_index < SLEEP_LOCK_USER_START_ID) {
                err = 6;
            } else {
                err = 7;
            }
        }
    }
    restore_interrupt_mask(mask);

    if (err != 0) {
        switch (err) {
            case 1:
                log_hal_error("this handle has locked deep sleep already\r\n");
                break;
            case 2:
                log_hal_error("this handle has locked all already\r\n");
                break;
            case 3:
                log_hal_warning("unsupported lock level %d\r\n", level);
                break;
            case 4:
                log_hal_warning("[Sleep Management]sleep handle=%d, lock sleep count full\r\n", handle_index);
                break;
            case 5:
                log_hal_warning("[Sleep Management]sleep handle=%d,%s, lock sleep count full\r\n", handle_index, (char *)&sleep_management_handle.user_handle_name[handle_index-SLEEP_LOCK_USER_START_ID]);
                break;
            case 6:
                log_hal_warning("[Sleep Management]sleep handle=%d, lock sleep has already released\r\n", handle_index);
                break;
            case 7:
                log_hal_warning("[Sleep Management]sleep handle=%d,%s, lock sleep has already released\r\n", handle_index, (char *)&sleep_management_handle.user_handle_name[handle_index-SLEEP_LOCK_USER_START_ID]);
                break;
        }
    }
    return err;
}

#if 0
bool sleep_management_check_sleep_locks(void)
{
    uint32_t mask;
    bool lock;
    mask = save_and_set_interrupt_mask();

    if ((sleep_management_handle.lock_sleep_all_high == 0) &&
        (sleep_management_handle.lock_sleep_all_low == 0)) {
        lock = false;
    } else {
        lock = true;
    }
    restore_interrupt_mask(mask);
    return lock;
}
#endif

bool sleep_management_is_sleep_level_locked(hal_sleep_lock_t level)
{
    uint32_t mask;
    bool lock = true;

    mask = save_and_set_interrupt_mask();
    if (level == HAL_SLEEP_LOCK_ALL) {
        if ((sleep_management_handle.lock_sleep_all_low == 0) &&
            (sleep_management_handle.lock_sleep_all_high == 0)) {
            lock = false;
        }
    } else if (level == HAL_SLEEP_LOCK_DEEP) {
        if ((sleep_management_handle.lock_sleep_deep_low == 0) &&
            (sleep_management_handle.lock_sleep_deep_high == 0)) {
            lock = false;
        }
    } else {
        log_hal_error("unsupported lock level %d\n", level);
    }

    restore_interrupt_mask(mask);
    return lock;
}

bool sleep_management_check_handle_status(uint8_t handle_index)
{
    // check handle index range
    if (handle_index >= SLEEP_LOCK_HANDLE_MAX) {
        log_hal_error("[Sleep Management]sleep handle index error\r\n");
        return false;
    }

    // return true for pre-allocate handler
    if (handle_index < SLEEP_LOCK_USER_START_ID) {
        return true;
    }

    // Check user allcate handler
    handle_index -= SLEEP_LOCK_USER_START_ID;
    if (handle_index < 32) {
        if (sleep_management_handle.user_handle_resoure_low & (1 << handle_index)) {
            return true;
        } else {
            return false;
        }
    } else {
        if (sleep_management_handle.user_handle_resoure_high & (1 << (handle_index-32))) {
            return true;
        } else {
            return false;
        }
    }
}

#if 0
uint32_t sleep_management_get_lock_sleep_request_info(void)
{
    return sleep_management_handle.lock_sleep_all;
}
#endif

bool sleep_management_get_lock_sleep_info(hal_sleep_lock_t level, uint32_t* info_high, uint32_t* info_low)
{
    if (level == HAL_SLEEP_LOCK_ALL) {
        *info_high = sleep_management_handle.lock_sleep_all_high;
        *info_low = sleep_management_handle.lock_sleep_all_low;
        return true;
    } else if (level == HAL_SLEEP_LOCK_DEEP) {
        *info_high = sleep_management_handle.lock_sleep_deep_high;
        *info_low = sleep_management_handle.lock_sleep_deep_low;
        return true;
    } else {
        log_hal_error("unsupported lock level %d\n", level);
        return false;
    }
}

void sleep_manager_dump_all_user_lock_sleep_handle(void)
{
    uint8_t i;

    for (i = SLEEP_LOCK_USER_START_ID; i < SLEEP_LOCK_HANDLE_MAX; i++) 
	{
        if (sleep_management_check_handle_status(i)) {
            printf("sleep handle=%d,%s\r\n", i, (char *)&sleep_management_handle.user_handle_name[i-SLEEP_LOCK_USER_START_ID][0]);
        }
    }
}

void sleep_management_get_lock_sleep_handle_list(void)
{
    uint8_t i;
    
    log_hal_warning("[Sleep Management]lock sleep handle list : \r\n");
    for (i = 0; i < SLEEP_LOCK_HANDLE_MAX; i++) {
        if (i < 32) {
            if (sleep_management_handle.lock_sleep_all_low & (1 << i)) {
                if (i < SLEEP_LOCK_USER_START_ID) {
                    log_hal_warning("sleep handle=%d\r\n", i);
                } else {
                    log_hal_warning("sleep handle=%d,%s\r\n", i, (char *)&sleep_management_handle.user_handle_name[i-SLEEP_LOCK_USER_START_ID][0]);
                }
            } else if (sleep_management_handle.lock_sleep_deep_low & (1 << i)) {
                if (i < SLEEP_LOCK_USER_START_ID) {
                    log_hal_warning("ds sleep handle=%d\r\n", i);
                } else {
                    log_hal_warning("ds sleep handle=%d,%s\r\n", i, (char *)&sleep_management_handle.user_handle_name[i-SLEEP_LOCK_USER_START_ID][0]);
                }
            }
        } else {
            if (sleep_management_handle.lock_sleep_all_high & (1 << (i-32))) {
                log_hal_warning("sleep handle=%d,%s\r\n", i, (char *)&sleep_management_handle.user_handle_name[i-SLEEP_LOCK_USER_START_ID][0]);
            } else if (sleep_management_handle.lock_sleep_deep_high & (1 << (i-32))) {
                log_hal_warning("ds sleep handle=%d,%s\r\n", i, (char *)&sleep_management_handle.user_handle_name[i-SLEEP_LOCK_USER_START_ID][0]);
            }
        }
    }

    return;
}


void sleep_management_low_power_init_setting(void)
{
    unsigned int system_info;
    system_info = *SYSTEM_INFOD;

    /* TINFO = "Enable CM4 DCM features" */
    *((volatile uint32_t*) (0xe00fe000)) = 0x3;
    *((volatile uint32_t*) (0xe00fe004)) = 0x47;
    *((volatile uint32_t*) (0xe0800040)) = 0x107;
    /*TINFO = " --------------------system_info == %h,-------------------- ", system_info */
    if( !((system_info >> 8) & 0x1)) //
    {
        /*TINFO = " -------------------- bond_psram_sip == 0-------------------- "*/
        *IO_CFG_1_PU_CFG0_CLR = 0x2000;
        *IO_CFG_1_PD_CFG0_SET = 0x2000;
    }
    if( !((system_info >> 9) & 0x1))
    {
        /*TINFO = " -------------------- bond_sf_sip == 0 -------------------- "*/
        *IO_CFG_1_PU_CFG0_CLR = 0x4000;
        *IO_CFG_1_PD_CFG0_SET = 0x4000;
    }

    if( !((system_info >> 10) & 0x1))
    {
        /*TINFO = " -------------------- bond_rsv == 0 -------------------- "*/
        *IO_CFG_1_PU_CFG0_CLR = 0x8000;
        *IO_CFG_1_PD_CFG0_SET = 0x8000;
    }
    if( ((system_info >> 5) & 0x1))
    {
        /*TINFO = " -------------------- trap_slow_src == 1 -------------------- "*/
        *IO_CFG_1_PD_CFG0_CLR = 0x40;
        *IO_CFG_1_PU_CFG0_SET = 0x40;
    }
    if( ((system_info >> 4) & 0x1))
    {
        /*TINFO = " -------------------- trap_hif_en == 1 -------------------- "*/
        *IO_CFG_1_PD_CFG0_CLR = 0x80;
        *IO_CFG_1_PU_CFG0_SET = 0x80;
    }
    if( ((system_info >> 3) & 0x1))
    {
        /*TINFO = " -------------------- trap_hif_sel == 1 -------------------- "*/
        *IO_CFG_1_PD_CFG0_CLR = 0x100;
        *IO_CFG_1_PU_CFG0_SET = 0x100;
    }
}

#ifdef  SLEEP_MANAGEMENT_DEBUG_ENABLE
#include "hal_gpt.h"
uint32_t sleep_handle_total_lock_sleep_time[SLEEP_LOCK_HANDLE_MAX];
uint32_t sleep_handle_total_lock_sleep_count[SLEEP_LOCK_HANDLE_MAX];
uint32_t sleep_backup_fun_time[SLEEP_BACKUP_RESTORE_MODULE_MAX];
uint32_t sleep_restore_fun_time[SLEEP_BACKUP_RESTORE_MODULE_MAX];

void sleep_management_debug_lock_sleep_timelog(sleep_management_lock_sleep_t lock, uint8_t handle_index)
{
    static uint32_t lock_sleep_time[SLEEP_LOCK_HANDLE_MAX], unlock_sleep_time;

    if (lock == LOCK_SLEEP) {
        if (sleep_management_handle.lock_sleep_request_count[handle_index] == 1) {
            hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &lock_sleep_time[handle_index]);
        }
        sleep_handle_total_lock_sleep_count[handle_index]++;
    } else {
        hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &unlock_sleep_time);

        if (unlock_sleep_time >= lock_sleep_time[handle_index]) {
            sleep_handle_total_lock_sleep_time[handle_index] += unlock_sleep_time - lock_sleep_time[handle_index];
        } else {
            sleep_handle_total_lock_sleep_time[handle_index] += unlock_sleep_time + (0xFFFFFFFF - lock_sleep_time[handle_index]);
        }
    }
}

void sleep_management_debug_backup_restore_fun_timelog(uint32_t type, uint32_t order, uint32_t callback)
{
    static  uint32_t enter, exit;

    if (order == 0) {
        hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &enter);
    } else {
        hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &exit);
        if (exit >= enter) {
            exit = exit - enter;
        } else {
            exit = exit + (0xFFFFFFFF - enter);
        }
        if (type == 0) {
            sleep_backup_fun_time[callback] = exit;
        } else {
            sleep_restore_fun_time[callback] = exit;
        }
    }

}

void sleep_management_debug_dump_lock_sleep_time()
{
    uint32_t i;

    printf("dump lock sleep time : \r\n");
    for (i = 0; i < SLEEP_LOCK_HANDLE_MAX; i++) {
        if (sleep_handle_total_lock_sleep_count[i] > 0) {
            printf("handle[%d] count : %d", (int)i, (int)sleep_handle_total_lock_sleep_count[i]);
            printf("handle[%d] total lock time : %d", (int)i, (int)sleep_handle_total_lock_sleep_time[i]);
        }
    }
}

void sleep_management_debug_dump_backup_restore_time()
{
    uint32_t i;

    printf("dump backup restore function execute time : \r\n");
    for (i = 0; i < SLEEP_BACKUP_RESTORE_MODULE_MAX; i++) {
        printf("backup fun[%d]  : %d", (int)i, (int)sleep_backup_fun_time[i]);
        printf("restore fun[%d] : %d", (int)i, (int)sleep_restore_fun_time[i]);
    }
}
#endif

void sleep_management_dump_wakeup_source(uint32_t wakeup_source)
{
    printf("===============Wakeup from Light Sleep===============\r\n");
    printf("[Sleep Management]WAKEUP_SOURCE = 0x%u\r\n", (unsigned int)wakeup_source);
    if (wakeup_source == SPM_WAKEUP_SOURCE_GPT)
    {
        printf("[Sleep Management]WAKEUP_SOURCE : GPT\r\n");
    } else if (wakeup_source == SPM_WAKEUP_SOURCE_EINT) {
        printf("[Sleep Management]WAKEUP_SOURCE : EINT\r\n");
    } else if (wakeup_source == SPM_WAKEUP_SOURCE_SDIO_SLV) {
        printf("[Sleep Management]WAKEUP_SOURCE : SDIO_SLV\r\n");
    } else if (wakeup_source == SPM_WAKEUP_SOURCE_SPI_SLAVE_A) {
        printf("[Sleep Management]WAKEUP_SOURCE : SPI_SLAVE_A\r\n");
    } else if (wakeup_source == SPM_WAKEUP_SOURCE_WDT) {
        printf("[Sleep Management]WAKEUP_SOURCE : WDT\r\n");
    } else if (wakeup_source == SPM_WAKEUP_SOURCE_KP) {
        printf("[Sleep Management]WAKEUP_SOURCE : KP\r\n");
    } else if (wakeup_source == SPM_WAKEUP_SOURCE_MD) {
        printf("[Sleep Management]WAKEUP_SOURCE : MD\r\n");
    } else if (wakeup_source == SPM_WAKEUP_SOURCE_OST) {
        printf("[Sleep Management]WAKEUP_SOURCE : OST\r\n");
    } else if (wakeup_source == SPM_WAKEUP_SOURCE_MD_DEBUGSYS) {
        printf("[Sleep Management]WAKEUP_SOURCE : MD_DEBUGSYS\r\n");
    } else if (wakeup_source == SPM_WAKEUP_SOURCE_DEBUGSYS) {
        printf("[Sleep Management]WAKEUP_SOURCE : DEBUGSYS\r\n");
    } else if (wakeup_source == SPM_WAKEUP_SOURCE_SENSOR_CTRLER) {
        printf("[Sleep Management]WAKEUP_SOURCE : SENSOR_CTRLER\r\n");
    } else {
        printf("[Sleep Management]Unknown wake up source\r\n");
    }
}

#ifdef MTK_NVDM_MODEM_ENABLE
#if SLEEP_MANAGEMENT_DEEPSLEEP_TEST
bool __sleep_management_deeper_sleep_test(void)
{
    uint32_t i = 0;
    nvdm_status_t status;
    uint32_t read_size=0x800;

    uint8_t debug = 0;

    vTaskSuspendAll();
    __asm volatile("cpsid i");

    if (debug == 1) {
        printf("\n\noriginal RAM content:\r\n");

        for (i = 0; i < 0x2000; i+=4) {
            if (i % 16 == 0)
                printf("\r\n");

            printf("%x ", *((volatile unsigned int*)(RETSRAM_BASE+i)));
        }
    }

    printf("\n\nmodified RAM content:\r\n");
    for (i = 0; i < 0x2000; i+=4) {
        if (debug == 1) {
            if (i % 16 == 0)
                printf("\r\n");
        }

        *((volatile unsigned int*)(RETSRAM_BASE+i)) = i;
        if (debug == 1) {
            printf("%x ", *((volatile unsigned int*)(RETSRAM_BASE+i)));
        }
    }

    // copy RTC RAM content to NVDM
    status = nvdm_write_data_item("deeper_sleep",
                                  "backup_data",
                                  NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                  (uint8_t *)RETSRAM_BASE,
                                  0x800);

    if (status != NVDM_STATUS_OK) {
        log_hal_error("write to NVDM fail!! %d\r\n", status);
        __asm volatile("cpsie i");
        (void)xTaskResumeAll();
        return false;
    }

    // clear RTC RAM
    memset((uint8_t *)RETSRAM_BASE, 0, 0x2000);
    if (debug == 1) {
        printf("clear RTC RAM\n");
        for (i = 0; i < 0x2000; i+=4) {
            if (i % 16 == 0)
                printf("\r\n");

            printf("%x ", *((volatile unsigned int*)(RETSRAM_BASE+i)));
        }
    }

    printf("read back from NVDM\r\n");
    // read back from NVDM
    status = nvdm_read_data_item("deeper_sleep",
                                 "backup_data",
                                 (uint8_t *)RETSRAM_BASE,
                                 &read_size);

    printf("Compare data\r\n");
    for (i = 0; i < 0x800; i+=4) {
        if (*((volatile unsigned int*)(RETSRAM_BASE+i)) != i) {
            log_hal_error("data error on address %x, %x", RETSRAM_BASE+i, *((volatile unsigned int*)(RETSRAM_BASE+i)));
            __asm volatile("cpsie i");
            (void)xTaskResumeAll();
            return false;
        }
    }

    log_hal_info("deeper sleep backup to NVDM test pass\n");

    __asm volatile("cpsie i");
    (void)xTaskResumeAll();

    return true;
}

bool __sleep_management_deeper_sleep_test2(void)
{
    uint32_t i = 0;
    nvdm_status_t status;
    uint32_t read_size=0x800;

    uint8_t debug = 0;

    vTaskSuspendAll();
    __asm volatile("cpsid i");

    if (debug == 1) {
        printf("\n\noriginal RAM content:\r\n");

        for (i = 0; i < 0x2000; i+=4) {
            if (i % 16 == 0)
                printf("\r\n");

            printf("%x ", *((volatile unsigned int*)(RETSRAM_BASE+i)));
        }
    }

    printf("\n\nmodified RAM content:\r\n");
    for (i = 0; i < 0x2000; i+=4) {
        if (debug == 1) {
            if (i % 16 == 0)
                printf("\r\n");
        }

        *((volatile unsigned int*)(RETSRAM_BASE+i)) = i;
        if (debug == 1) {
            printf("%x ", *((volatile unsigned int*)(RETSRAM_BASE+i)));
        }
    }

    // copy RTC RAM content to NVDM
    sleep_managerment_backup_rtcram();

#if 0
    if (status != NVDM_STATUS_OK) {
        log_hal_error("write to NVDM fail!! %d\r\n", status);
        __asm volatile("cpsie i");
        (void)xTaskResumeAll();
        return false;
    }
#endif

    // clear RTC RAM
    memset((uint8_t *)RETSRAM_BASE, 0, 0x2000);
    if (debug == 1) {
        printf("clear RTC RAM\n");
        for (i = 0; i < 0x2000; i+=4) {
            if (i % 16 == 0)
                printf("\r\n");

            printf("%x ", *((volatile unsigned int*)(RETSRAM_BASE+i)));
        }
    }

    printf("read back from NVDM\r\n");
    // read back from NVDM
    sleep_managerment_restore_rtcram();

    printf("Compare data\r\n");
    for (i = 0; i < 0x2000; i+=4) {
        if (*((volatile unsigned int*)(RETSRAM_BASE+i)) != i) {
            log_hal_error("data error on address %x, %x", RETSRAM_BASE+i, *((volatile unsigned int*)(RETSRAM_BASE+i)));
            __asm volatile("cpsie i");
            (void)xTaskResumeAll();
            return false;
        }
    }

    log_hal_info("deeper sleep backup to NVDM test pass\n");

    __asm volatile("cpsie i");
    (void)xTaskResumeAll();

    return true;
}

bool __sleep_management_deeper_sleep_test3(void)
{
    uint32_t backup_start_time = 0;
    uint32_t backup_end_time = 0;
    uint32_t restore_start_time = 0;
    uint32_t restore_end_time = 0;

    vTaskSuspendAll();
    __asm volatile("cpsid i");

    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &backup_start_time);
    sleep_managerment_backup_rtcram();
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &backup_end_time);

    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &restore_start_time);
    sleep_managerment_restore_rtcram();
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &restore_end_time);

    __asm volatile("cpsie i");
    (void)xTaskResumeAll();

    printf("backup RTC ram for 100 times cost %u ticks\r\n", backup_end_time - backup_start_time);
    printf("restore RTC ram for 100 times cost %u ticks\r\n", restore_end_time - restore_start_time);

    return true;
}
#endif
#endif

#if SLEEP_MANAGEMENT_SLEEP_OVERHEAD_TEST
void sleep_management_light_sleep_overhead_test(uint32_t sleep_ms, uint32_t test_time)
{
    uint32_t i = 0;
    uint32_t start_time = 0;
    uint32_t end_time = 0;

    __asm volatile("cpsid i");
    sleep_manager_set_sleep_mode(HAL_SLEEP_MODE_LIGHT_SLEEP);
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &start_time);
    for (i = 0; i < test_time; i++) {
        hal_sleep_manager_set_sleep_time(sleep_ms);
        hal_sleep_manager_enter_sleep_mode(HAL_SLEEP_MODE_LIGHT_SLEEP);
    }
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &end_time);
    __asm volatile("cpsie i");

    printf("start_time = %u, end_time = %u, delta = %u(32k tick)\r\n", start_time, end_time, end_time - start_time);
    printf("delta without sleep time = %u(32k tick)\r\n", (end_time - start_time) - (uint32_t)(sleep_ms * test_time * 32.768f));
    printf("overhead per sleep = %u(32k tick)\r\n", ((end_time - start_time) - (uint32_t)(sleep_ms * test_time * 32.768f)) / test_time);
}
#endif

#if SLEEP_MANAGEMENT_SLEEP_DUMP_PMU
#include "hal_pmu_platform_mt2625.h"

ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_EN_VBATSNS_ROUT;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_ADC_TS_EN;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_VCORE_VOSEL_LPM;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_T_LONGPRESS_SEL;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_LONGPRESS_TYPE_SEL;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RGS_VSRAM_EN;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_VPA_VHSEL;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_BUCK_VPA_OC_SDN_EN;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_BUCK_VPA_MSFG_RRATE2;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_BUCK_VPA_MSFG_RRATE3;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_BUCK_VPA_MSFG_RRATE4;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_BUCK_VPA_MSFG_RRATE5;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_BUCK_VPA_DVS_TRANST_ONCE;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_BUCK_VPA_DVS_TRANST_TD;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_DEG_WND_OC_VPA;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_VPA_CSMIR;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_VPA_CSL;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_VPA_RSV2;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_VCORE_IPK_OS_LPM;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_VRF_RSV1;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_VCORE_DVS_STEP_SEL;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_VCORE_RSV1;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_VRF_EN;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_VSRAM_EN;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_BUCK_VPA_EN;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_VSIM_EN;
ATTR_RWDATA_IN_RETSRAM uint32_t PMU_RG_VFEM_EN;

void sleep_management_light_sleep_pmu_get(void)
{
    PMU_RG_EN_VBATSNS_ROUT = *((volatile uint32_t *)PMU_RG_EN_VBATSNS_ROUT_ADDR);
    PMU_RG_ADC_TS_EN = *((volatile uint32_t *)PMU_RG_ADC_TS_EN_ADDR);
    PMU_RG_VCORE_VOSEL_LPM = *((volatile uint32_t *)PMU_RG_VCORE_VOSEL_LPM_ADDR);
    PMU_RG_T_LONGPRESS_SEL = *((volatile uint32_t *)PMU_RG_T_LONGPRESS_SEL_ADDR);
    PMU_RG_LONGPRESS_TYPE_SEL = *((volatile uint32_t *)PMU_RG_LONGPRESS_TYPE_SEL_ADDR);
    PMU_RGS_VSRAM_EN = *((volatile uint32_t *)PMU_RGS_VSRAM_EN_ADDR);
    PMU_RG_VPA_VHSEL = *((volatile uint32_t *)PMU_RG_VPA_VHSEL_ADDR);
    PMU_RG_BUCK_VPA_OC_SDN_EN = *((volatile uint32_t *)PMU_RG_BUCK_VPA_OC_SDN_EN_ADDR);
    PMU_RG_BUCK_VPA_MSFG_RRATE2 = *((volatile uint32_t *)PMU_RG_BUCK_VPA_MSFG_RRATE2_ADDR);
    PMU_RG_BUCK_VPA_MSFG_RRATE3 = *((volatile uint32_t *)PMU_RG_BUCK_VPA_MSFG_RRATE3_ADDR);
    PMU_RG_BUCK_VPA_MSFG_RRATE4 = *((volatile uint32_t *)PMU_RG_BUCK_VPA_MSFG_RRATE4_ADDR);
    PMU_RG_BUCK_VPA_MSFG_RRATE5 = *((volatile uint32_t *)PMU_RG_BUCK_VPA_MSFG_RRATE5_ADDR);
    PMU_RG_BUCK_VPA_DVS_TRANST_ONCE = *((volatile uint32_t *)PMU_RG_BUCK_VPA_DVS_TRANST_ONCE_ADDR);
    PMU_RG_BUCK_VPA_DVS_TRANST_TD = *((volatile uint32_t *)PMU_RG_BUCK_VPA_DVS_TRANST_TD_ADDR);
    PMU_RG_DEG_WND_OC_VPA = *((volatile uint32_t *)PMU_RG_DEG_WND_OC_VPA_ADDR);
    PMU_RG_VPA_CSMIR = *((volatile uint32_t *)PMU_RG_VPA_CSMIR_ADDR);
    PMU_RG_VPA_CSL = *((volatile uint32_t *)PMU_RG_VPA_CSL_ADDR);
    PMU_RG_VPA_RSV2 = *((volatile uint32_t *)PMU_RG_VPA_RSV2_ADDR);
    PMU_RG_VCORE_IPK_OS_LPM = *((volatile uint32_t *)PMU_RG_VCORE_IPK_OS_LPM_ADDR);
    PMU_RG_VRF_RSV1 = *((volatile uint32_t *)PMU_RG_VRF_RSV1_ADDR);
    PMU_RG_VCORE_DVS_STEP_SEL = *((volatile uint32_t *)PMU_RG_VCORE_DVS_STEP_SEL_ADDR);
    PMU_RG_VCORE_RSV1 = *((volatile uint32_t *)PMU_RG_VCORE_RSV1_ADDR);
    PMU_RG_VRF_EN = *((volatile uint32_t *)PMU_RG_VRF_EN_ADDR);
    PMU_RG_VSRAM_EN = *((volatile uint32_t *)PMU_RG_VSRAM_EN_ADDR);
    PMU_RG_BUCK_VPA_EN = *((volatile uint32_t *)PMU_RG_BUCK_VPA_EN_ADDR);
    PMU_RG_VSIM_EN = *((volatile uint32_t *)PMU_RG_VSIM_EN_ADDR);
    PMU_RG_VFEM_EN = *((volatile uint32_t *)PMU_RG_VFEM_EN_ADDR);
}

void sleep_management_light_sleep_pmu_dump(void)
{
    printf("PMU_RG_EN_VBATSNS_ROUT = 0x%x\r\n", PMU_RG_EN_VBATSNS_ROUT);
    printf("PMU_RG_ADC_TS_EN = 0x%x\r\n", PMU_RG_ADC_TS_EN);
    printf("PMU_RG_VCORE_VOSEL_LPM = 0x%x\r\n", PMU_RG_VCORE_VOSEL_LPM);
    printf("PMU_RG_T_LONGPRESS_SEL = 0x%x\r\n", PMU_RG_T_LONGPRESS_SEL);
    printf("PMU_RG_LONGPRESS_TYPE_SEL = 0x%x\r\n", PMU_RG_LONGPRESS_TYPE_SEL);
    printf("PMU_RGS_VSRAM_EN = 0x%x\r\n", PMU_RGS_VSRAM_EN);
    printf("PMU_RG_VPA_VHSEL = 0x%x\r\n", PMU_RG_VPA_VHSEL);
    printf("PMU_RG_BUCK_VPA_OC_SDN_EN = 0x%x\r\n", PMU_RG_BUCK_VPA_OC_SDN_EN);
    printf("PMU_RG_BUCK_VPA_MSFG_RRATE2 = 0x%x\r\n", PMU_RG_BUCK_VPA_MSFG_RRATE2);
    printf("PMU_RG_BUCK_VPA_MSFG_RRATE3 = 0x%x\r\n", PMU_RG_BUCK_VPA_MSFG_RRATE3);
    printf("PMU_RG_BUCK_VPA_MSFG_RRATE4 = 0x%x\r\n", PMU_RG_BUCK_VPA_MSFG_RRATE4);
    printf("PMU_RG_BUCK_VPA_MSFG_RRATE5 = 0x%x\r\n", PMU_RG_BUCK_VPA_MSFG_RRATE5);
    printf("PMU_RG_BUCK_VPA_DVS_TRANST_ONCE = 0x%x\r\n", PMU_RG_BUCK_VPA_DVS_TRANST_ONCE);
    printf("PMU_RG_BUCK_VPA_DVS_TRANST_TD = 0x%x\r\n", PMU_RG_BUCK_VPA_DVS_TRANST_TD);
    printf("PMU_RG_DEG_WND_OC_VPA = 0x%x\r\n", PMU_RG_DEG_WND_OC_VPA);
    printf("PMU_RG_VPA_CSMIR = 0x%x\r\n", PMU_RG_VPA_CSMIR);
    printf("PMU_RG_VPA_CSL = 0x%x\r\n", PMU_RG_VPA_CSL);
    printf("PMU_RG_VPA_RSV2 = 0x%x\r\n", PMU_RG_VPA_RSV2);
    printf("PMU_RG_VCORE_IPK_OS_LPM = 0x%x\r\n", PMU_RG_VCORE_IPK_OS_LPM);
    printf("PMU_RG_VRF_RSV1 = 0x%x\r\n", PMU_RG_VRF_RSV1);
    printf("PMU_RG_VCORE_DVS_STEP_SEL = 0x%x\r\n", PMU_RG_VCORE_DVS_STEP_SEL);
    printf("PMU_RG_VCORE_RSV1 = 0x%x\r\n", PMU_RG_VCORE_RSV1);
    printf("PMU_RG_VRF_EN = 0x%x\r\n", PMU_RG_VRF_EN);
    printf("PMU_RG_VSRAM_EN = 0x%x\r\n", PMU_RG_VSRAM_EN);
    printf("PMU_RG_BUCK_VPA_EN = 0x%x\r\n", PMU_RG_BUCK_VPA_EN);
    printf("PMU_RG_VSIM_EN = 0x%x\r\n", PMU_RG_VSIM_EN);
    printf("PMU_RG_VFEM_EN = 0x%x\r\n", PMU_RG_VFEM_EN);
}
#endif

void sleep_manager_set_sleep_mode(hal_sleep_mode_t mode)
{
    sleep_mode = mode;
}

hal_sleep_mode_t sleep_manager_get_sleep_mode(void)
{
    return sleep_mode;
}

void sleep_management_backup_systime(uint32_t systime)
{
    SystickBak = systime;
}

uint32_t sleep_management_get_backup_systime(void)
{
    return SystickBak;
}

#endif /* HAL_SLEEP_MANAGER_ENABLED */
