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
/*
 **************************************************************************/
/** \file
 * GKI header file that defines the user semaphores.
 * This file must not be included directly by any file outside the GKI.
 * Within the GKI this file may be included several times within the same
 * file so there is no multiple inclusion protection required.
 **************************************************************************/

/** \addtogroup PrdGki
 * @{
 */

/** Semaphore definition macro.
 * This macro is used to define a GKI semaphore. The macro is redefined
 * several times within the GKI to create data types and storage to support
 * the defined semaphores. The GKI will create the semaphore and set it's
 * count at boot time. Applications use the given semaphore id (sEMAiD
 * parameter) when calling any semaphore related GKI functions.
 *
 * \param[in] sEMAiD This is the id that will be added to the #KiSemaphoreId
 *                enumeration. This is what the application code will use to
 *                reference a semaphore in any of the semaphore related GKI
 *                functions.
 * \param[in] iNITIALcOUNT This is the initial count that the GKI should
 *                set for the semaphore at boot time. */
#if !defined (KI_SEMA_DEF)
# define KI_SEMA_DEF(sEMAiD, iNITIALcOUNT)
#endif

/** Semaphore reservation macro.
 * This macro is used to reserve a location in the GKI semaphore id table.
 * The reserved id cannot be used in a GKI semaphore function as the
 * semaphore will not exist (this will cause an assert). This is mainly
 * used to maintain the numbering of semaphore id's when some are
 * dependent on conditional compiles.
 *
 * \param[in] sEMAiD This is the id that will be added to the #KiSemaphoreId
 *                enumeration. */
#if !defined (KI_SEMA_RESERVED)
# define KI_SEMA_RESERVED(sEMAiD)
#endif

/***************************************************************************
* SEMAPHORES SHOULD NOT BE USED WHERE POSSIBLE. THE MAIN INTERTASK
* COMMUNICATION AND SYNCHRONISATION SHOULD BE DONE USING GKI SIGNALS.
* SEMAPHORES ARE CURRENTLY NOT LOGGED AND THERE CAN BE PROBLEMS WHEN USING
* BOTH SIGNALS AND SEMAPHORES IN THE SAME TASK.
*
* ONLY ADD SEMAPHORES TO THE END OF THE LIST OR USE OLD REDUNDANT ENTRIES.
* IF A SEMAPHORE IS NO LONGER NEEDED REPLACE IT WITH A REDUNDANT ENTRY
* RATHER THAN REMOVING IT. THE NUMBERING OF THE SEMAPHORE ID'S MUST BE KEPT
* THE SAME.
*
* IF A SEMAPHORE IS CONDITIONALLY COMPILED ADD A RESERVED ENTRY IN AN
* ELSE CONDITION SO THAT THE NUMBERING DOES NOT CHANGE BASED ON CONDITIONAL
* COMPILES.
***************************************************************************/

/***************************************************************************
* Do not change anything above this line
***************************************************************************/

    KI_SEMA_DEF (KI_TICK_SEMAPHORE, 0)
    KI_SEMA_DEF (KI_DI_SEMAPHORE, 1)

    /* The BASE0002 semaphores MUST be defined in sequence: */
    KI_SEMA_DEF (KI_BASE0002_SEM_1, 1)
    KI_SEMA_DEF (KI_BASE0002_SEM_2, 1)
    KI_SEMA_DEF (KI_BASE0002_SEM_3, 1)
    KI_SEMA_DEF (KI_BASE0002_SEM_4, 1)
    KI_SEMA_DEF (KI_BASE0002_SEM_5, 1)

    KI_SEMA_DEF (KI_TMM_SEMAPHORE, 1)

#if defined (UPGRADE_FSYSTEM)
    KI_SEMA_DEF (KI_FS_SEMAPHORE, 1)
#else
    KI_SEMA_RESERVED (KI_FS_SEMAPHORE)
#endif

#if !defined (KI_DISABLE_DYNAMIC_MEMORY)
    KI_SEMA_DEF (KI_MEM_SEMAPHORE, 0)
#else
    KI_SEMA_RESERVED (KI_MEM_SEMAPHORE)
#endif

  KI_SEMA_RESERVED (KI_SEMAPHORE_UNUSED_1)

/* Cipher semaphores defined in place of irda redundant entries. */
#if defined (UPGRADE_CIPHERK)
#  if defined (FR_CIK_PRESENT)
#    define CIPHER_HW_SEMAPHORE_NEEDED
#    if defined (ENABLE_UCIPHER_DEV_INTEGRATION)
#      define CIPHER_SW_SEMAPHORE_NEEDED
#    endif /*ENABLE_UCIPHER_DEV_INTEGRATION*/
#  else  /* FR_CIK_PRESENT */
#      define CIPHER_SW_SEMAPHORE_NEEDED
#  endif /* FR_CIK_PRESENT */
#else  /* UPGRADE_CIPHERK */
#  define CIPHER_SW_SEMAPHORE_NEEDED
#endif /* UPGRADE_CIPHERK */

#if defined (CIPHER_HW_SEMAPHORE_NEEDED)
    KI_SEMA_DEF (KI_CIPHER_HW_SEMAPHORE_ID, 1)
#else
    KI_SEMA_RESERVED (KI_CIPHER_HW_SEMAPHORE_ID)
#endif

#if defined (CIPHER_SW_SEMAPHORE_NEEDED)
    KI_SEMA_DEF (KI_CIPHER_SW_SEMAPHORE_ID, 1)
#else
    KI_SEMA_RESERVED (KI_CIPHER_SW_SEMAPHORE_ID)
#endif

#if defined (USE_L1AL)
    KI_SEMA_DEF (KI_L1AL_HI_TASK_SEMAPHORE, 0)
#else
    KI_SEMA_RESERVED (KI_L1AL_HI_TASK_SEMAPHORE)
#endif

    /* Please re-use me! */
    KI_SEMA_RESERVED (KI_SEMAPHORE_UNUSED_2)

#if defined (UPGRADE_MMC)
    KI_SEMA_DEF (KI_MMC_SEMAPHORE, 0)
#else
    KI_SEMA_RESERVED (KI_MMC_SEMAPHORE)
#endif

#if defined (USE_FLASH_NVRAM) && !defined (DISABLE_FLASH_INTERFACE)
    KI_SEMA_DEF (KI_NVRAM_SEMAPHORE, 1)
#else
    KI_SEMA_RESERVED (KI_NVRAM_SEMAPHORE)
#endif

#if defined (KI_ENABLE_RAM_LOGGING)
    KI_SEMA_DEF (KI_RLG_PRINTF_SEMAPHORE, 1)
#else
    KI_SEMA_RESERVED (KI_RLG_PRINTF_SEMAPHORE)
#endif

#if !defined (DM_EXCLUDE_NVRAM_DEVICE_MANAGER)
    /* NVRAM Direct Call blocking */
    KI_SEMA_DEF (KI_DMNVP_BLOCKING_SEMAPHORE, 0)
#else
    KI_SEMA_RESERVED (KI_DMNVP_BLOCKING_SEMAPHORE)
#endif

    KI_SEMA_DEF (KI_MEDIA_OBJECT_SEMAPHORE_ID, 1)

    // TODO remove if possible
    KI_SEMA_RESERVED (KI_DEVICES_TEST_FRAMEWORK_SEMAPHORE)

#if defined (OSDRV_GKI_PORTING_TEST)
/*Gki test semaphore set*/
#include <gki_porting_test_seminc.h>
#endif

#if 0 // Co-simulation will be fixed in next delivery
#if defined (UPGRADE_GENERIC_MUX)
    KI_SEMA_DEF (KI_PPPA_SEMAPHORE, 0)
#else
    KI_SEMA_RESERVED (KI_PPPA_SEMAPHORE)
#endif
#else
    /* Please re-use me! */
    KI_SEMA_RESERVED (KI_SEMAPHORE_UNUSED_3)
#endif // 0

    KI_SEMA_DEF (KI_NBIOT_SEC_MULTI_ACCESS_SEMAPHORE, 1)
    KI_SEMA_DEF (KI_NBIOT_SEC_HW_SYNC_SEMAPHORE, 0)

    KI_SEMA_DEF (KI_SIM_DCL_SEMPHORE, 1)
    KI_SEMA_DEF (KI_SIM0_EVT_SEMPHORE, 0)
    KI_SEMA_DEF (KI_USIM0_EVT_SEMPHORE, 0)

    KI_SEMA_DEF (KI_BOOT_GKI_LOG_SEMAPHORE,0)
    KI_SEMA_DEF(KI_N1_MDSYS_SLEEP_SEMAPHORE, 0)

    KI_SEMA_DEF (KI_EMMI_HI_EVENTS_SEMAPHORE, 0)
    KI_SEMA_DEF (KI_TIM_BACKUP_TIMERS_SEMAPHORE, 1)
    KI_SEMA_DEF (KI_NBIOT_PRESYNC_SEMAPHORE, 0)
/***************************************************************************
* Do not change anything below this line
***************************************************************************/

#undef KI_SEMA_DEF
#undef KI_SEMA_RESERVED

/** @} */ /* End of PrdGki group */

/* END OF FILE */
