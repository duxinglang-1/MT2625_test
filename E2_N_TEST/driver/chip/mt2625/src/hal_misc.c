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

#include "hal_platform.h"
#include "hal_sleep_manager_internal.h"
#ifdef HAL_SLEEP_MANAGER_ENABLED

#ifdef HAL_SDIO_MODULE_ENABLED
extern void sdio_backup_all(void *data, uint32_t mode);
extern void sdio_restore_all(void *data, uint32_t mode);
#endif

#ifdef HAL_SPI_MASTER_MODULE_ENABLED
extern void spim_backup_register_callback(void *data, uint32_t mode);
extern void spim_restore_register_callback(void *data, uint32_t mode);
#endif

#ifdef HAL_I2C_MASTER_MODULE_ENABLED
extern void hal_i2c_backup_all_register(void *data, uint32_t mode);
extern void hal_i2c_restore_all_register(void *data, uint32_t mode);
#endif

#ifdef USE_ULS
extern void uls_sleep_management_suspend_cb(void *data, uint32_t mode);
#endif

#ifdef MTK_NB_MODEM_ENABLE
extern void N1HwMdsysSleepDriverSuspendCb(void *data, uint32_t mode);
extern void N1HwMdsysSleepDriverRestoreCb(void *data, uint32_t mode);
#endif

void hal_module_sleep_register_callback(void)
{
//Example
//   hal_cm4_topsm_register_suspend_cb(i2c_sleep_cb, "i2c_xxx");
//   hal_cm4_topsm_register_resume_cb(i2c_wakeup_cb, "i2c_xxx");

    /*register spi callback*/
#if 0//def HAL_SPI_MASTER_MODULE_ENABLED
    sleep_management_register_suspend_callback(SLEEP_BACKUP_RESTORE_SPI_MASTER, (sleep_management_suspend_callback_t)spim_backup_register_callback, NULL, HAL_SLEEP_MODE_LIGHT_SLEEP);
    sleep_management_register_resume_callback(SLEEP_BACKUP_RESTORE_SPI_MASTER, (sleep_management_resume_callback_t)spim_restore_register_callback, NULL, HAL_SLEEP_MODE_LIGHT_SLEEP);
#endif

    /*register i2c callback*/
#ifdef  HAL_I2C_MASTER_MODULE_ENABLED
    sleep_management_register_suspend_callback(SLEEP_BACKUP_RESTORE_I2C, (sleep_management_suspend_callback_t)hal_i2c_backup_all_register, NULL, HAL_SLEEP_MODE_LIGHT_SLEEP);
    sleep_management_register_resume_callback(SLEEP_BACKUP_RESTORE_I2C, (sleep_management_resume_callback_t)hal_i2c_restore_all_register, NULL, HAL_SLEEP_MODE_LIGHT_SLEEP);
#endif

    /*register sdio callback*/
#if 0//def HAL_SDIO_MODULE_ENABLED
    sleep_management_register_suspend_callback(SLEEP_BACKUP_RESTORE_SDIO_MST, (sleep_management_suspend_callback_t)sdio_backup_all, NULL, HAL_SLEEP_MODE_LIGHT_SLEEP);
    sleep_management_register_resume_callback(SLEEP_BACKUP_RESTORE_SDIO_MST, (sleep_management_resume_callback_t)sdio_restore_all, NULL, HAL_SLEEP_MODE_LIGHT_SLEEP);
#endif

#ifdef USE_ULS
    sleep_management_register_suspend_callback(SLEEP_BACKUP_RESTORE_ULS, uls_sleep_management_suspend_cb, NULL, HAL_SLEEP_MODE_LIGHT_SLEEP);
#endif

#ifdef MTK_NB_MODEM_ENABLE
    sleep_management_register_suspend_callback(SLEEP_BACKUP_RESTORE_MDSYS, N1HwMdsysSleepDriverSuspendCb, NULL, HAL_SLEEP_MODE_LIGHT_SLEEP);
    sleep_management_register_resume_callback(SLEEP_BACKUP_RESTORE_MDSYS, N1HwMdsysSleepDriverRestoreCb, NULL, HAL_SLEEP_MODE_LIGHT_SLEEP);
#endif
}

#endif

