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
 **************************************************************************
 * File Description
 * ----------------
 *    Configuration settings for the hardware platform
 **************************************************************************/

#ifndef APPL_CFG_H
#define APPL_CFG_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/


/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/* The Task ID for the FL Task (allows several FLs to be created!) */
#define FOREGROUND_TASK_ID  TASK_FL_ID
#define FOREGROUND_QUEUE_ID FL_QUEUE_ID


/*
**  Number of preferred PLMNs in each ABMM block, and the total number
**  we can handle (determines the number of blocks we can request - the
**  first one is always there).
*/
#define ABMM_MAX_NUM_PREF_PLMNS          SIM_PLMN_LIST_SIZE
#define ABMM_NUM_PREF_PLMNS       (SIM_PLMN_LIST_SIZE)
#if defined (ABMM_UNIT_TEST)
#  define ABMM_MAX_PREF_PLMNS       64
#else
   /* 0107-11562 */
/* 0210-21042 : increase to max SIM card capacity */
/* max size of transparent file on SIM = 255, 3 bytes per PLMNentry*/
#    define ABMM_MAX_PREF_PLMNS       (255/3)
#endif

# if defined(ENABLE_EHPLMN)
/*
**  Number of EHPLMNs in each ABMM block, and the total number
**  we can handle (determines the number of blocks we can request - the
**  first one is always there).
*/
#define ABMM_NUM_EHPLMNS       (SIM_PLMN_LIST_SIZE)
#define ABMM_MAX_EHPLMNS       ABMM_MAX_PREF_PLMNS 
# endif /* (ENABLE_EHPLMN) */

/*0108-12177 Include the hash table size for client config. This must match the
 * array in abcfg.c
 */
#if defined (UPGRADE_3G)
/* job 114671: new suggested values for 3G. Please note that these are provided as an indication.
 * It is the customer responsibility to optimise it and make sure it matches their requirements*/
#define       MAX_DIR_SIZE_TABLE   (6)
#else
#define       MAX_DIR_SIZE_TABLE   (8)
#endif


/***************************************************************************
 * Audio + other Configuration
 **************************************************************************/
    /*Most audio configuration for system 2 resides in the new layer one*/
    /*Any platform specific config MUST be within platform UPGRADEs within the system 2 upgrade */

/* Configure the number of LNR/LNM/LND/ADR*/
#if defined (UPGRADE_NVRAM_OPTIMIZE)
#define REMOVE_LNM_FILE
#define REMOVE_LNR_FILE
#define REMOVE_ADR_FILE
#define REMOVE_LND_FILE
#endif /* UPGRADE_NVRAM_OPTIMIZE */
/*Place target specific audio config here*/

#  define NVRAM_SIZE         8192 /*Temporary to allow compilation without any real nvram*/


/* 0006-6836
 * Max number of LNDs ever allowed on SIM (see GSM spec 02.07)
 */
#define MAX_LND_FILE_SIZE_ON_SIM    10

/*
** Configuration for NVRAM sizes
*/
#if defined (NVRAM_TEST)
/* For unit testing only */
#if !defined (REMOVE_LNM_FILE)
#  define LNM_FILE_SIZE   5     /* max # dial nums in Last Number Missed file */
#else
   define LNM_FILE_SIZE   0
#endif
#if !defined (REMOVE_LNR_FILE)
#  define LNR_FILE_SIZE   5     /* max # dial nums in Last Number RX'd file   */
#else
#  define LNR_FILE_SIZE   0     /* max # dial nums in Last Number RX'd file   */
#endif
#if !defined (REMOVE_ADR_FILE)
#  define ADR_FILE_SIZE   5     /* max # dial nums in Abbr Dial Num file      */
#else
#  define ADR_FILE_SIZE   0     /* ADR file is removed      */
#endif

#if !defined (REMOVE_LND_FILE)
#  define LND_FILE_SIZE   5     /* max # dial nums in Last Number Missed file */
#else
#  define LND_FILE_SIZE   0     /* max # dial nums in Last Number Missed file */
#endif
#else

#if (NVRAM_SIZE >= 8192)

#  if !defined (REMOVE_LNM_FILE)
#  define LNM_FILE_SIZE   10    /* max # dial nums in Last Number Missed file */
#  else
#  define LNM_FILE_SIZE   0    /* max # dial nums in Last Number Missed file */
#  endif
#  if !defined (REMOVE_LNR_FILE)
#  define LNR_FILE_SIZE   10    /* max # dial nums in Last Number RX'd file   */
#  else
#  define LNR_FILE_SIZE   0    /* max # dial nums in Last Number RX'd file   */
#  endif

#if !defined (REMOVE_ADR_FILE)
#  if defined (REDUCED_ADR_FILE)
#    define ADR_FILE_SIZE   10    /* max # dial nums in Abbr Dial Num file      */
#  else
#    define ADR_FILE_SIZE   18    /* max # dial nums in Abbr Dial Num file      */
#  endif
#else
#  define ADR_FILE_SIZE   0     /* ADR file is removed      */
#endif /*REMOVE_ADR_FILE*/

#if !defined (REMOVE_LND_FILE)
#  define LND_FILE_SIZE   10    /* max # dial nums in Last Number Missed file */
#else
#  define LND_FILE_SIZE   0    /* max # dial nums in Last Number Missed file */
#endif
        /*
        ** When defined the bearer capability info will be stored in the
        ** appropriate NVRAM files e.g. LNM and LNR files
        */
#  define notANRM_DN_BEARER_CAP
        /*
        ** When defined the sub address info will be stored in the
        ** appropriate NVRAM files e.g. LNM and LNR files
        */
#  define notANRM_DN_SUB_ADDRESS

#elif (NVRAM_SIZE == 2048)

#if !defined (REMOVE_LNM_FILE)
#  define LNM_FILE_SIZE   3     /* max # dial nums in Last Number Missed file */
#else
#  define LNM_FILE_SIZE   0     /* max # dial nums in Last Number Missed file */
#endif

#if !defined (REMOVE_LNR_FILE)
#  define LNR_FILE_SIZE   3     /* max # dial nums in Last Number RX'd file   */
#else
#  define LNR_FILE_SIZE   0     /* max # dial nums in Last Number RX'd file   */
#endif

#if !defined (REMOVE_ADR_FILE)
#  define ADR_FILE_SIZE   5     /* max # dial nums in Abbr Dial Num file      */
#else
#  define ADR_FILE_SIZE   0     /* max # dial nums in Abbr Dial Num file      */
#endif
#if !defined (REMOVE_LND_FILE)
#  define LND_FILE_SIZE   3     /* max # dial nums in Last Number Missed file */
#else
#  define LND_FILE_SIZE   0     /* max # dial nums in Last Number Missed file */
#endif
        /*
        ** When defined the bearer capability info will be stored in the
        ** appropriate NVRAM files e.g. LNM and LNR files
        */
#  define notANRM_DN_BEARER_CAP
        /*
        ** When defined the sub address info will be stored in the
        ** appropriate NVRAM files e.g. LNM and LNR files
        */
#  define notANRM_DN_SUB_ADDRESS
#else
#  error "No NVRAM Size Configuration Defined"
#endif
#endif /* NVRAM_TEST */

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/


/* This number may be changed by object customers.
 * It is the number of time stamp records that may be stored for SMS messages. This is
 * aimed at sent messages which do not have SC timestamps, we store the local "sent" time.
 */
#define MAX_SMS_TIMESTAMPS 30

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

#endif
/* END OF FILE */
