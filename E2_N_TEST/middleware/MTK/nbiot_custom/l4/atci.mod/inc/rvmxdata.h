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
 **************************************************************************/

#ifndef RVMXDATA_H
#define RVMXDATA_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#if !defined (SYSTEM_H)
#  include <system.h>
#endif
#if !defined (KI_SIGBUF_H)
#  include <ki_sigbuf.h>
#endif
#if !defined (RVSYSTEM_H)
#  include <rvsystem.h>
#endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

typedef enum VoyagerSubsystemsTag
{
  SS_START = 0,
  MUX,
  OPERATIONS_MANAGER,
  COMMAND_RESPONSE_MANAGER,
  CHANGE_CONTROL_MANAGER,
#if defined (DEVELOPMENT_VERSION)
  TEST,
#endif
  MOBILITY,
#if defined (ENABLE_AT_ENG_MODE)
  ENGINEERING_MODE,
#endif
  SMS,
  SIM_TOOLKIT,
  PACKET_DATA,
  CALL_CONTROL,
  GENERAL,
  PROFILE,
  SUPPLEMENTARY,
  NVRAM,
  SIM_LOCK,
#ifdef ENABLE_AP_BRIDGE_FEATURE
  APBRIDGE,
#endif
  SLEEP_MANAGER,
  SS_END = SLEEP_MANAGER + 1
} VoyagerSubsystems_t;

typedef enum ExtendedOperationTag
{
  INVALID_EXTENDED_OPERATION,
  EXTENDED_ACTION,
  EXTENDED_QUERY,
  EXTENDED_ASSIGN,
  EXTENDED_RANGE,
  NUM_OF_EXTENDED_OPERATIONS
} ExtendedOperation_t;

typedef struct AtChannelTag
{
  /* the information which we have cached from the MUX while waiting for    */
  /* a chance to send the command                                           */
  SignalBuffer  fromMuxCache[AT_CACHE_SIZE];

  /* The signals we have cached on their way to the multiplexer because we  */
  /* have been XOFF'd by the MUX                                            */
  SignalBuffer  toMuxCache[AT_CACHE_SIZE];

  Int8     fromMuxCacheCount;   /* how many signals in from Mux cache.  */
  Int8     toMuxCacheCount;     /* how many signals in our to MUX cache */
  Boolean  waitingForCnf;       /* TRUE if waiting for AtDataCnf from the MUX */
} AtChannel_t;

typedef enum EntityStateTag
{
  ENTITY_NOT_ENABLED,
  ENTITY_IDLE,
  ENTITY_PROCESSING,
  ENTITY_RUNNING
} EntityState_t;

typedef struct CommandLineTag
{
  Int16  position;
  /** \assoc ARRAY \ref character */
  Int16  length;
#if defined (ATCI_ENABLE_DYN_AT_BUFF)
  Char   *character;
#else
  Char   character[COMMAND_LINE_SIZE + NULL_TERMINATOR_LENGTH];
#endif
} CommandLine_t;

typedef struct ShortCommandLineTag
{
  Int16  position;
  /** \assoc ARRAY \ref character */
  Int16  length;
  /** \assoc STRING */
  Char   character[SHORT_COMMAND_LINE_SIZE + NULL_TERMINATOR_LENGTH];
} ShortCommandLine_t;


typedef enum VgDataEntryModeTag
{
  DATA_ENTRY_AT_COMMAND,
  DATA_ENTRY_SMS_MESSAGE,
  DATA_ENTRY_CTM_MESSAGE,
  NUM_OF_DATA_ENTRY_MODES
} VgDataEntryMode; 

typedef enum ScanResultsTag
{
  AWAITING_NONE,
  AWAITING_A,
  AWAITING_T,
  AT_ERROR,
  ONLY_AT_FOUND,
  RUN_AT_COMMAND,
  ONLY_CR_FOUND
} ScanResults_t;

/* added for job106314 */
typedef enum CommandTypeTag
{
  BASIC_SYNTAX,
  EXTENDED_SYNTAX
} CommandType;

/* job106314: moved from rvdata.h */
typedef struct ScanParseContextTag
{
  CommandLine_t  nextCommand;

#if defined (FEA_A_SLASH)
  CommandLine_t  prevCommand;
#endif

  ScanResults_t  scanResult;
  /* added for job106314 */
  CommandType    currentCommandType;
} ScanParseContext_t;

#if defined(ATCI_ENABLE_DYN_AT_BUFF)
typedef struct atCmdBufferTag
{
  Boolean inUse;
  Char    charBuff[COMMAND_LINE_SIZE + NULL_TERMINATOR_LENGTH];
} atCmdBuffer_t;
#endif
/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

#endif  /* RVMXDATA_H */

/* END OF FILE */




