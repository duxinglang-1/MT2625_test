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
 * File Description                                                      */
/** \file
 *
 *  The general Application Layer kernel definitions are declared.
 *  As for Protocol Stack Release 3.1
 **************************************************************************/

#ifndef APPLAYER_H
#define APPLAYER_H

#define NUM_OF_LANGUAGES    (16)  /* Should be in SIM Manager */

/* definition to allow switching of variable / variables to static storage */
/* class                                                                   */
#define PRIVATE

/*0012-8469*/
#if !defined (FL_UCS2)
#  define FL_UCS2
#endif

/***************************************************************************
 * Nested Include Files
 ***************************************************************************/

#include <nbiot_modem_common_config.h>

#if !defined(KI_SIGBUF_H)
#   include <ki_sigbuf.h>
#endif

#if ! defined (KITQID_H)
#   include <kitqid.h>
#endif

#if !defined(APPL_SIG_H)
#   include <appl_sig.h>
#endif

#if !defined(APPL_CFG_H)
#   include <appl_cfg.h>
#endif

#ifndef GEN_NVDM_DATABASE_FOR_PC
#include <string.h>
#endif
#include <ctype.h>

#if defined(ON_PC)
#   include <stdio.h>
#endif

/* The following type definition is used throughout the
 * Foreground and Background layers wherever a SIM or
 * NVRAM record is referenced */
typedef Int16 LmRecordNumber;               /**< SIM / NVRAM record number */

typedef Int8  LmEntryNumber;                /**< SIM / NVRAM number of different entries per record */


/***************************************************************************
 * Manifest Constants
 **************************************************************************/
 
/* The following are key defines for the signal SIG_AL_USER_INTERRUPT_IND */
/* which are used by individual modules to trigger user generated events  */
#if defined(ON_PC)
#define   AL_INT_KEY         'i'
#define   CC_INT_KEY         'a'
#define   SS_INT_KEY         'w'
#define   LC_INT_KEY         'g'
#define   HI_INT_KEY         'h'
#define   MM_INT_KEY         'b'
#define   PM_INT_KEY         'o'
#define   SI_INT_KEY         'z'
#define   SM_INT_KEY         'x'
#define   TL_INT_KEY         't'
#define   CB_INT_KEY         'y'
#endif

/* Character Constants */
#define CHR_SPACE         ((Char)' ')
#define CHR_SEPARATOR     ((Char)'\n')
#define STR_SEPARATOR     "\n"
#define CHR_STAR          ((Char)'*')
#define STR_STAR          "*"
#define CHR_DOT           ((Char)'.')
#define STR_DOT           "."
#define CHR_HASH          ((Char)'#')
#define STR_HASH          "#"
#define CHR_PLUS          ((Char)'+')
#define STR_PLUS          "+"
#define CHR_PAUSE         ((Char)'P')
#define STR_PAUSE         "P"
#define CHR_REPLACEMENT   ((Char)'*')   /* (for password entry) */
#define CHR_WILDCARD      ((Char)'?')


/* Bcd Values for Dial Number storage */
#define   DIALBCD_0                 0x0
#define   DIALBCD_1                 0x1
#define   DIALBCD_2                 0x2
#define   DIALBCD_3                 0x3
#define   DIALBCD_4                 0x4
#define   DIALBCD_5                 0x5
#define   DIALBCD_6                 0x6
#define   DIALBCD_7                 0x7
#define   DIALBCD_8                 0x8
#define   DIALBCD_9                 0x9
#define   DIALBCD_STAR              0xA
#define   DIALBCD_HASH              0xB
#define   DIALBCD_PAUSE             0xC
#define   DIALBCD_WILD              0xD
#define   DIALBCD_EXPANSION         0xE
#define   DIALBCD_FILLER            0xF

/* max length of a Dial Number Alpha ID in the AL (c.f. MAX_LENGTH_ALPHA_TAG) */
#if defined(FL_UCS2)
# define MAX_AL_DN_ALPHA_TAG   21
#else
# define MAX_AL_DN_ALPHA_TAG   32
# endif  /* FL_UCS2 */


/*============================================================================*/
/*  BL Power Down Timers (grouped here as they are inter-related)             */

#if !defined (FAST_AL_POWERDOWN)
/* time delay before ABPM instructs L1 to turn off the power for real         */
/* all other power off timeouts used to clean up should be less than this!    */
/* FR9803-2164 - test 26.2.4.p6 requires a number of RACHs in the attempt to
 * IMSI detach before the power is finally removed.  5 seconds isn't long
 * enough, so the timeout is extended for phase 2.  In normal operation
 * the power-off procedure will not take nearly this long
 */
#define POWER_DOWN_POWER_OFF_TICKS          MILLISECONDS_TO_TICKS( 6500L )

/* time after receipt of an AbsiPowerDownSig before ABSI issues a Terminte    */
/* Session Request to SIM Manager.                                            */
/* MUST be less that POWER_DOWN_POWER_OFF_TICKS (and allow time for the SIM   */
/* Task to handle the Terminate Request)                                      */
/* FR9803-2164 - see comment above */
#define POWER_DOWN_SIM_TERMINATE_TICKS      MILLISECONDS_TO_TICKS( 5500L )

#else  /* !FAST_AL_POWERDOWN */
  /*ACR156 power down as fast as possible*/
#define POWER_DOWN_POWER_OFF_TICKS          MILLISECONDS_TO_TICKS( 100L )
#define POWER_DOWN_SIM_TERMINATE_TICKS      MILLISECONDS_TO_TICKS( 100L )

#endif

/***************************************************************************
 * Typed Constants
 **************************************************************************/

 /******************************************************************************
 * Enumerated types
 *****************************************************************************/

/** Procedure identifier element */
typedef enum ProcIdTag
{
  /** Call control procedure */
  PROCEDURE_BL_CC_ID,
  /** Mobility management procedure */
  PROCEDURE_BL_MM_ID,
  /** Power management procedure */
  PROCEDURE_BL_PM_ID,
  /** SIM procedure */
  PROCEDURE_BL_SIM_ID,
  /** Background Layer shell procedure */
  PROCEDURE_BL_SHELL_ID,
  /** Short Message Service procedure */
  PROCEDURE_BL_SM_ID,
  /** Supplementary services procedure */
  PROCEDURE_BL_SS_ID,
  /** General library procedure */
  PROCEDURE_BL_GL_ID,
  /** List management procedure */
  PROCEDURE_BL_LM_ID,
  /** Engineering mode procedure */
  PROCEDURE_BL_EM_ID,

#if defined (UPGRADE_NASMDL2)
  /** Background layer Packet Data procedure  */
  PROCEDURE_BL_PD_ID,
#else /* UPGRADE_NASMDL2 */
  /** Background layer GPRS procedure  */
  PROCEDURE_BL_GP_ID,
#endif /* else UPGRADE_NASMDL2 */

#if defined (UPGRADE_APP_INFO)
  /** Assisted GPS procedure */
  PROCEDURE_BL_PS_ID,
  /** Application Information Procedure  */
  PROCEDURE_BL_AI_ID,
#endif /* UPGRADE_APP_INFO */

#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_MBMS)
#  if defined (UPGRADE_3G_TDD128)
  /** MBMS procedure */
  PROCEDURE_BL_MBMS_ID,
#  endif /*UPGRADE_3G_TDD128*/
# endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G */

/* Calibratiion procedure */
  PROCEDURE_BL_CAL_ID,
  PROCEDURE_BL_CDMG_ID,
  PROCEDURE_BL_CPHY_ID,
  PROCEDURE_BL_UMPH_ID,
  PROCEDURE_BL_DM_ID,
  /** Radio Policy Manager library */
  PROCEDURE_BL_RPM_ID,
  /** Number of processes */
  NUMBER_OF_PROCS,

  /** Unkown procedure */
  PROCEDURE_NULL
} ProcId;




/** AL error class */
typedef enum AlErrorClassTag
{
  /** Fatal error */
  AL_ERROR_CLASS_FATAL,
  /** Warning */
  AL_ERROR_CLASS_WARNING,
  /** Information  */
  AL_ERROR_CLASS_INFO,

  /** Number of AL error classes */
  NUM_OF_AL_ERROR_CLASSES,

  /** NULL */
  AL_ERROR_CLASS_NULL
}
AlErrorClass;


/** AL error codes */
typedef enum AlErrorCodeTag
{
  /** No ME data available */
  AL_ERROR_NO_ME_DATA,
  /** Radio failure */
  AL_ERROR_RADIO_FAILURE,
  /** SIM error log */
  AL_ERROR_SIM_ERROR_LOG,
  /** failed to read the data from NVRAM */
  AL_ERROR_LC_READING_NVRAM,
  /** Timeout when reading data from NVRAM */
  AL_ERROR_LC_NVRAM_READ_TIMEOUT,
  /** failed to write the data to NVRAM */
  AL_ERROR_LC_WRITING_NVRAM,
  /** Timeout when writing data to NVRAM */
  AL_ERROR_LC_NVRAM_WRITE_TIMEOUT,
  /** failed to fetch the data from EPROM!! */
  AL_ERROR_NO_AOC_SESSION_DATA,
  AL_ERROR_DO_NOT_HAVE_CONTROL,
  /** failed to fetch the data from SHELL */
  AL_ERROR_NO_BL_SHELL_DATA,

  /** Default band mode is used */
  AL_ERROR_USING_DEFAULT_BAND_MODE,
  AL_ERROR_NO_CAL_DATA,
  AL_ERROR_NO_BAND_INFO,
  /** Failed CB settings reading/writing from/to NVRAM */
  AL_ERROR_NO_NVRAM_CB_SETTINGS,
  /** NVRAM ME data is not valid */
  AL_ERROR_NO_NVRAM_ME_DATA,

  /** Default service type is used */
  AL_ERROR_USING_DEFAULT_SERVICE_TYPE,
  /** Number of error codes */
  NUM_OF_AL_ERRORS,
  /** NULL */
  AL_ERROR_NULL
}
AlErrorCode;



/***************************************************************************
 * Type Definitions
 **************************************************************************/

/* basic types definitions */
typedef Int8    Percentage;         /* percentage value from 0 to 100 */


/***************************************************************************
 * Macros
 ***************************************************************************/

/* Macro to determine the number of elements in an array
 * This should only be used where there is visibility
 * of the size of an array
 */
#define ELEMENTSINARRAY(x) (sizeof(x)/sizeof(x[0]))


/***************************************************************************
 * Functions Prototypes
 ***************************************************************************/

#endif /* APPLAYER_H */

/* END OF FILE */

