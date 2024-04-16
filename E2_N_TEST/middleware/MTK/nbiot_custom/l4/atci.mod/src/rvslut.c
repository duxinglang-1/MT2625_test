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
 *
 * Procedures for Sim Lock AT commands
 *
 * Contains implement Pin Code.
 *  AT+CFUN:    Sets the level of phone functionality
 *  ATD112:     Dial command for emergency calls
 *  ATH112:     Dial command for emergency calls
 *  AT*MSIMINS: SIM insertion unsolicited reporting
 *  AT+CSIM:    Generic SIM access (file reading and writing etc)
 *  AT+CRSM:    Restricted SIM access
 *  AT+CMAR:    Master Reset
 **************************************************************************/

#define MODULE_NAME "RVSLUT"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvprof.h>
#include <rvslut.h>
#include <rvslsigo.h>
#include <rvchman.h>
#include <rvutil.h>
#include <rvcrerr.h>
#include <rvcrconv.h>
#include <rvccut.h>
#include <rvcrhand.h>
#include <rvgput.h>
#include <rvcimxut.h>
#include <gkimem.h>
#include <gkisig.h>
#if defined (COARSE_TIMER)
#include <rvgnsigi.h>
#endif
#include <frhsl.h>
#include <psc_api.h>
#if !defined (ABSICUST_H)
#include <absicust.h>
#endif

#include "atci_gki_trace.h"
/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

const Char *simStateMessage[NUM_OF_VG_SIM_STATES] =
{
  (const Char*)"NOT READY",
  (const Char*)"SIM PIN",
  (const Char*)"SIM PUK",
  (const Char*)"READY",
  (const Char*)"SIM PIN2",
  (const Char*)"SIM PUK2",
  (const Char*)"SIM UPIN",
  (const Char*)"SIM UPUK"
};

static const struct vgPinAccessLevelTag {
  const Char       *name;
  VgPinAccessLevel level;
} pinAccessLevel[] =
{
  { (const Char*)"PIN",  VG_PIN   },
  { (const Char*)"PIN2", VG_PIN2  },
  { (const Char*)"UPIN", VG_UPIN  }

};

typedef enum VgShowPinStatusFlagTag
{
  VG_SHOW_PIN1,
  VG_SHOW_PIN2,
  VG_SHOW_UPIN,
  VG_SHOW_ALL
} VgShowPinStatusFlag;

#if defined (ENABLE_LONG_AT_CMD_RSP)
const Char cmarResetProcedure[AT_SMALL_BUFF_SIZE] = ";&F;&W;+CFUN=1,1;+CFUN=1";
#else
const Char cmarResetProcedure[VG_MAX_AT_DATA_IN_SIGNAL_LENGTH] = ";&F;&W;+CFUN=1,1;+CFUN=1";
#endif


typedef enum cpinrSimPinStatusTypeTag
{
  VG_CPINR_SIM_PIN = 0,
  VG_CPINR_SIM_PUK,
  VG_CPINR_SIM_PIN2,
  VG_CPINR_SIM_PUK2,
  VG_CPINR_SIM_UPIN,
  VG_CPINR_SIM_UPUK,
  NUM_CPINR_PIN_STATUS_VALUES
} cpinrSimPinStatusType;

const Char *cpinrSimPinStatusMessage[NUM_CPINR_PIN_STATUS_VALUES] =
{
  (const Char*)"SIM PIN",
  (const Char*)"SIM PUK",
  (const Char*)"SIM PIN2",
  (const Char*)"SIM PUK2",
  (const Char*)"SIM UPIN",
  (const Char*)"SIM UPUK"
};

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static void setFileAndDirId (SimEfId *fileId, SimDirId *dirId);
static SimDirId simWhichDir (const SimEfId efId);

static ResultCode_t parseCommandLineForCrsmCrla (
                     CommandLine_t *commandBuffer_p,
                      Int8  *cmd_p,
                       SimEfId *fileId_p,
                        Int8  *p1_p,
                         Int8  *p2_p,
                          Int8  *p3_p,
                           Int8  *commandDataString_p,
                            Int16 maxCommandDataStringLength,
                             Int16 *commandDataStringLength_p,
                              /* job134856: add handling for <pathid> field */
                              Int8 *pathData_p,
                               Int8 *pathLength_p);

static ResultCode_t buildCommandForCrsmCrla (Int8  *commandData,
                                              Int16 *commandDataLength,
                                               Int8  cmd, Int8 sessionId,
                                                Int8  p1, Int8 p2, Int8 p3,
                                                 const Char *commandDataString,
                                                  Int16 commandDataStringLength,
                                                   const VgmuxChannelNumber  entity);


static Boolean      convertLevelStringToLevel  (Char *levelString,
                                                VgPinAccessLevel *level);
static void         showMupinRange        (const VgmuxChannelNumber entity);
static void         showMupinValue        (const VgmuxChannelNumber entity,
                                           VgShowPinStatusFlag flag);
static ResultCode_t verifyCommand         (VgPinAccessLevel accessLevel);
static ResultCode_t enablePin             (CommandLine_t *commandBuffer_p,
                                           const VgmuxChannelNumber entity,
                                           VgPinAccessMode mode,
                                           VgPinAccessLevel accessLevel,
                                           SimUiccKeyRefValue pinValue);
static ResultCode_t verifyPin             (CommandLine_t *commandBuffer_p,
                                           const VgmuxChannelNumber entity,
                                           VgPinAccessLevel accessLevel);
static ResultCode_t unblockPin            (CommandLine_t *commandBuffer_p,
                                           const VgmuxChannelNumber entity,
                                           VgPinAccessLevel accessLevel);
static ResultCode_t changePin             (CommandLine_t *commandBuffer_p,
                                           const VgmuxChannelNumber entity,
                                           VgPinAccessLevel accessLevel);
static void         showCpinrValue        (const VgmuxChannelNumber entity,
                                           cpinrSimPinStatusType cpinrSimPinStatusValue);
static void         showAllCpinrValues    (const VgmuxChannelNumber entity);
static Boolean      noChannelCsimLocked    (void);

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Variables
 ***************************************************************************/

/***************************************************************************
 * Macros
 ***************************************************************************/

/***************************************************************************
 * Constants
 ***************************************************************************/

#define MAX_EMERGENCY_STRINGS     (8)
#define MAX_EMERGENCY_DIAL_LEN    (5)
#define CPIN_SS_PUK2_REQUEST      (52)

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
*
* Function:     noChannelCsimLocked
*
* Parameters:   none
*
* Returns:      nothing
*
* Description:  Checks through all of the channels specific data for csimLocked
*               to find if any are locked
**-------------------------------------------------------------------------*/

static Boolean noChannelCsimLocked (void)
{
  Boolean                 noLock = TRUE;
  SimLockContext_t       *simLockContext_p;
  Int8                    cIndex = 0;

  while ((noLock == TRUE) && (cIndex < CI_MAX_ENTITIES))
  {
    simLockContext_p = ptrToSimLockContext(cIndex);
    if ((simLockContext_p != PNULL) && (simLockContext_p->csimLocked == TRUE))
    {
      noLock = FALSE;
    }
    cIndex++;
  }
  return(noLock);
}

/*--------------------------------------------------------------------------
*
* Function:     setFileAndDirId
*
* Parameters:   fileId - file Id to be tested
*               dirId  -  dir Id to be set
*
* Returns:      nothing
*
* Description:  If fileId is DF or MF then set dirID
*
*-------------------------------------------------------------------------*/

static void setFileAndDirId (SimEfId *fileId, SimDirId *dirId)
{

  if  (((Int16)*fileId == (Int16)SIM_DIR_MF) ||
       ((Int16)*fileId == (Int16)SIM_DIR_DF_GRAPHICS) ||
       ((Int16)*fileId == (Int16)SIM_DIR_DF_GSM) ||
       ((Int16)*fileId == (Int16)SIM_DIR_DF_DCS1800) ||
       ((Int16)*fileId == (Int16)SIM_DIR_DF_TELECOM))
  {
    *dirId  = (SimDirId)*fileId;
    *fileId = SIM_EF_INVALID;
  }
  else
  {
    *dirId = simWhichDir (*fileId);
  }
}

/*--------------------------------------------------------------------------
*
* Function:     simWhichDir
*
* Parameters:   fileId - file Id to be tested
*
* Returns:      SimDirId - directory of supplied EF Id
*
* Description:  Finds directory that contains specified EF
*
*-------------------------------------------------------------------------*/

static SimDirId simWhichDir (const SimEfId efId)
{
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext();
  SimDirId dirId;

  if (simLockGenericContext_p->simInfo.cardIsUicc == TRUE)
  {
      dirId = SIM_DIR_ADF_USIM;
  }
  else
  {
      dirId = SIM_DIR_DF_GSM;
  }

  switch (efId)
  {
    case SIM_EF_PL:
    case SIM_EF_ICCID:
    {
      dirId = SIM_DIR_MF;
      break;
    }
#if defined (FEA_PHONEBOOK)
    case SIM_EF_ADN:
    case SIM_EF_FDN:
    case SIM_EF_CCP:
    case SIM_EF_MSISDN:
    case SIM_EF_LND:
    case SIM_EF_SDN:
    case SIM_EF_BDN:
    case SIM_EF_EXT1:
    case SIM_EF_EXT2:
    case SIM_EF_EXT3:
    case SIM_EF_EXT4:
    case SIM_EF_CPHS_INFO_NUM_OLD:
#endif /* FEA_PHONEBOOK */
    case SIM_EF_SMS:
    case SIM_EF_SMSP:
    case SIM_EF_SMSS:
    case SIM_EF_SMSR:
    {
       if (simLockGenericContext_p->simInfo.cardIsUicc == TRUE)
       {
          dirId = SIM_DIR_ADF_USIM;
       }
       else
       {
          dirId = SIM_DIR_DF_TELECOM;
       }
       break;
    }
    case SIM_EF_IMG:
    {
      dirId = SIM_DIR_DF_GRAPHICS;
      break;
    }
#if defined (FEA_PHONEBOOK)
    case SIM_EF_PBR:
    {
      dirId = SIM_DIR_DF_PHONEBOOK;
      break;
    }
#endif /* FEA_PHONEBOOK */
    /*
    job113327: added handling for ENS-specific files to ensure access under the correct Dir. */
    case SIM_EF_TST:
    {
      dirId = SIM_DIR_DF_ENS;
      break;
    }
    case SIM_EF_ACT_HPLMN:
    {
      dirId = SIM_DIR_DF_IRIDIUM;
      break;
    }
    case SIM_EF_INVALID:
    {
      dirId = SIM_DIR_INVALID;
      break;
    }
    default:
    {
      if ((efId & 0xFF00) == 0x4F00)
      {
        dirId =  SIM_DIR_DF_GRAPHICS;
      }
      else if(((efId & 0xFF00) == 0x2F00) ||
              ((efId & 0xFF00) == 0x7F00))
      {
        dirId = SIM_DIR_MF;
      }
      else
      {
        dirId = SIM_DIR_ADF_USIM;
      }
      break;
    }
  }

  return (dirId);
}


/*--------------------------------------------------------------------------
*
* Function:     parseCommandLineForCrsmCrla
*
* Scope:        Local
*
* Parameters:   in:  commandBuffer_p - the AT command line string
*               out: cmd_p - command number
*               out: fileId_p - the EF (file id)
*               out: p1_p - P1 parameter
*               out: p2_p - P2 parameter
*               out: p3_p - P3 parameter
*               out: commandDataString_p - the command data as hex string
*
* Returns:      Boolean - TRUE if parsed ok
*
* Description:  Support function for the +CSRM/+CRLA AT command.
*               Parses the command line and returns parameters.
*
*-------------------------------------------------------------------------*/
static ResultCode_t parseCommandLineForCrsmCrla (
                     CommandLine_t *commandBuffer_p,
                      Int8  *cmd_p,
                       SimEfId *fileId_p,
                        Int8  *p1_p,
                         Int8  *p2_p,
                          Int8  *p3_p,
                           Int8  *commandDataString_p,
                            Int16 maxCommandDataStringLength,
                             Int16 *commandDataStringLength_p,
                              /* job134856: add handling for <pathid> field */
                              Int8 *pathData_p,
                               Int8 *pathLength_p)
{
  Int32        cmd;
  Int32        fileId = SIM_EF_INVALID;
  Int32        p[3] = {0};
  Int32        pIndex;
  ResultCode_t result = RESULT_CODE_ERROR;

  /* job134856: add handling for <pathid> field */
  Int8         hexString[VG_CRSM_MAX_PATHID_CHARS + NULL_TERMINATOR_LENGTH] = {0};  
  Int16        hexStringLength;
  Int8         index = 0;
  Boolean      crsmError = FALSE;

  /* get the command type */
  if (getExtendedParameter (commandBuffer_p, &cmd, ULONG_MAX) == TRUE)
  {
    if ((VG_CRSM_COMMAND_READ_BINARY   == cmd) ||
        (VG_CRSM_COMMAND_READ_RECORD   == cmd) ||
        (VG_CRSM_COMMAND_UPDATE_BINARY == cmd) ||
        (VG_CRSM_COMMAND_UPDATE_RECORD == cmd) ||
        (VG_CRSM_COMMAND_GET_RESPONSE  == cmd) ||
        (VG_CRSM_COMMAND_STATUS        == cmd))
    {
      *cmd_p = (Int8) cmd;
      result = RESULT_CODE_PROCEEDING;
    }
    else
    {
      /* Invalid SIM command (for CRSM) */
      result = VG_CME_CRSM_INVALD_COMMAND;
    }
  }
  else
  {
    result = VG_CME_CRSM_MISSING_PARAMETER;
    /* Cmd parse error */
  }


  /* the file id */
  if (RESULT_CODE_PROCEEDING == result)
  {
    if (getExtendedParameter (commandBuffer_p, &fileId, SIM_EF_INVALID) == TRUE)
    {
      /* all commands except status require fileId to be specified */
      if ((VG_CRSM_COMMAND_STATUS != cmd) && (SIM_EF_INVALID == fileId))
      {
        result = VG_CME_CRSM_INVALID_FILE_ID;
      }
    }
    else
    {
      result = VG_CME_CRSM_INVALID_FILE_ID;
    }
  }

  /* the P1, P2, P3 parameters */
  if (RESULT_CODE_PROCEEDING == result)
  {
    for (pIndex = 0; pIndex < 3; pIndex++)
    {
      if (getExtendedParameter (commandBuffer_p, &p[pIndex], ULONG_MAX) == TRUE)
      {
        if (p[pIndex] == ULONG_MAX)
        {
          /* all commands except status and get response require p1,2,3 to be
           * specified */
          if ((VG_CRSM_COMMAND_STATUS       == cmd) ||
              (VG_CRSM_COMMAND_GET_RESPONSE == cmd))
          {
            p[pIndex] = 0;
          }
          else
          {
            result = VG_CME_CRSM_INVALID_P_PARAMETER;
          }
        }
      }
      else
      {
        result = VG_CME_CRSM_INVALID_P_PARAMETER;
      }
    }
  }

  /* the Data parameter is only required for UPDATE_XXX command */
  if (RESULT_CODE_PROCEEDING == result)
  {
    /* job134856: rework to allow for possible subsequent <pathid> field */
    if (getExtendedString (commandBuffer_p,
                            commandDataString_p,
                             maxCommandDataStringLength,
                              commandDataStringLength_p) == FALSE)
    {
      /* field contained invalid data */
      result = VG_CME_CRSM_MISSING_CMD_DATA;
    }
    else
    {
      if ((*commandDataStringLength_p == 0) &&
          ((VG_CRSM_COMMAND_UPDATE_BINARY == cmd) ||
           (VG_CRSM_COMMAND_UPDATE_RECORD == cmd)))
      {
        /* field contained empty string, but command requires data */
        result = VG_CME_CRSM_MISSING_CMD_DATA;
      }
    }
  }

  /* job134856: add handling for <pathid> field */
  if (RESULT_CODE_PROCEEDING == result)
  {
    if (getExtendedString (commandBuffer_p,
                            hexString,
                             VG_CRSM_MAX_PATHID_CHARS,
                              &hexStringLength) == TRUE)
    {
      if ((hexStringLength % 2) == 0)
      {
        /* proceed if an even number of characters have been entered */
        while ((index < (Int8)hexStringLength) && !crsmError)
        {
          /* convert two ASCII characters to single digit hex number */
          *pathData_p = vgAsciiToInt8 (&hexString[index], &crsmError);
          pathData_p++;
          index += 2;
        }

        if (!crsmError)
        {
          /* hex number length is half input ASCII string length */
          *pathLength_p = (Int8)hexStringLength / 2;
        }
        else
        {
          /* input string contained non-hex characters */
          result = VG_CME_CRSM_PATHID_INVALID;
        }
      }
      else
      {
        /* input string contained odd number of characters */
        result = VG_CME_CRSM_PATHID_INVALID;
      }
    }
    else
    {
      /* error reading parameter */
      result = VG_CME_CRSM_PATHID_INVALID;
    }
  }
  *fileId_p = (SimEfId) fileId;
  *p1_p     = (Int8) p[0];
  *p2_p     = (Int8) p[1];
  *p3_p     = (Int8) p[2];

  return (result);
}


/*--------------------------------------------------------------------------
*
* Function:     buildCommandForCrsmCrla
*
* Scope:        Local
*
* Parameters:   out: commandData - the AT command line string
*               out: commandDataLength - length of the SIM command data
*               in:  cmd - the SIM command number
*               in:  p1, p2, p3 - the parameters
*               in:  commandDataString - the extra command data as hex
*                                        string.
*                                             string.
* Returns:      Boolean - TRUE if build command ok.
*
* Description:  Support function for the +CSRM AT command.
*               Given the parsed command line parameters then
*               build the command to send to the SIM.
*
*-------------------------------------------------------------------------*/
static ResultCode_t buildCommandForCrsmCrla (Int8  *commandData,
                                              Int16 *commandDataLength,
                                               Int8  cmd, Int8 sessionId,
                                                Int8  p1, Int8 p2, Int8 p3,
                                                 const Char *commandDataString,
                                                  Int16 commandDataStringLength,
                                                   const VgmuxChannelNumber  entity)
{
  ResultCode_t result = RESULT_CODE_PROCEEDING;
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext();

  /* build the command data */
  memset (commandData, 0, VG_SIM_MAX_MSG_SIZE);
  if(cmd == VG_CRSM_COMMAND_STATUS)
  {
    commandData[0] = VG_CRSM_CLASS_STATUS;
  }
  else if (simLockGenericContext_p->simInfo.cardIsUicc == FALSE)
  {
      commandData[0] = VG_CRSM_CLASS_GSM_APP;
  }

  if(sessionId < SIM_LOG_CHAN_BASIC_SESSION_NUM)
  {
    commandData[0] |= sessionId;
  }
  else
  {
    commandData[0] |= SIM_LOG_CHAN_EXTEND_MASK;
    commandData[0] |= (sessionId - SIM_LOG_CHAN_BASIC_SESSION_NUM);
  }

  commandData[1] = cmd;
  commandData[2] = p1;
  commandData[3] = p2;
  commandData[4] = p3;

  /* convert data string from hex chars to bytes if present and add to command */
  if (commandDataStringLength > 0)
  {
    *commandDataLength = vgMapTEToGsm (
                          &commandData[VG_CRSM_COMMAND_HEADER_SIZE],
                           VG_CRSM_MAX_COMMAND_DATA,
                            commandDataString,
                             commandDataStringLength,
                              VG_AT_CSCS_HEX,
                               entity);

    /* check update parameters to see if data length matches p3 */
    if ((VG_CRSM_COMMAND_UPDATE_BINARY == cmd) ||
        (VG_CRSM_COMMAND_UPDATE_RECORD == cmd))
    {
      /* command data length is double actual byte length of data sent */
      if ((*commandDataLength) != p3)
      {
        result = VG_CME_CSIM_LENGTH_INCORRECT;
      }
    }
  }
  else
  {
    *commandDataLength = 0;
  }

  *commandDataLength += VG_CRSM_COMMAND_HEADER_SIZE;

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:     getCFUNMode
*
* Scope:        Local
*
* Parameters:   None.
*
* Returns:      VgCFUNType -  current power mode deduced from SIM state
*                             and radio power
*
* Description:  Deduces current power state based upon the current state
*               of the SIM (open/closed) and radio power.
*
*-------------------------------------------------------------------------*/

VgCFUNType vgGetCFUNMode (void)
{
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext();
  VgCFUNType              cfunMode;

  if (simLockGenericContext_p->powerUpProtoStack == TRUE)
  {
    if (simLockGenericContext_p->powerUpSim == TRUE)
    {
      cfunMode = VG_AT_CFUN_FULL_FUNC;
    }
    else
    {
      cfunMode = VG_AT_CFUN_FULL_RF_CLOSED_SIM;
    }
  }
  else
  {
    if (simLockGenericContext_p->powerUpSim == TRUE)
    {
      cfunMode = VG_AT_CFUN_DISABLE_RF_RXTX;
    }
    else
    {
      cfunMode = VG_AT_CFUN_MIN_FUNC;
    }
  }

  return (cfunMode);
}

/*--------------------------------------------------------------------------
 *
 * Function:        convertLevelStringToLevel
 *
 * Parameters:      level - PIN access level
 *                  accessLevel - String of PIN access level
 *
 * Returns:         Boolean true if ok.
 *
 * Description:     Converts from access level String to access level
 *
 *-------------------------------------------------------------------------*/
static Boolean convertLevelStringToLevel (Char *levelString, VgPinAccessLevel *level)
{
  Boolean found = FALSE;
  Int8    index = 0;

  vgConvertStringToUpper(levelString, levelString);

  while ( (FALSE == found) &&
          (index < VG_ARRAY_LENGTH(pinAccessLevel)) )
  {
    if ( 0 == strcmp((char*)pinAccessLevel[index].name, (char*)levelString) )
    {
      *level = pinAccessLevel[index].level;
      found = TRUE;
    }
    index++;
  }
  return (found);
}

/*--------------------------------------------------------------------------
*
* Function:     showMupinRange
*
* Parameters:
*
* Returns:      nothing
*
* Description:  show MUPIN range for MUPIN test command
*
*-------------------------------------------------------------------------*/

static void showMupinRange (const VgmuxChannelNumber entity)
{
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
  if (simLockGenericContext_p->simInfo.cardIsUicc == TRUE)
  {
    vgPutNewLine(entity);
    if (simLockGenericContext_p->simInfo.universalPinSupportedByCard == TRUE)
    {
      vgPrintf(entity, (const Char*)"*MUPIN: (0-4,10,11),(\"PIN\",\"PIN2\",\"UPIN\"),(0-1,10,11),%d,%d,(\"01\",\"81\",\"11\")",
                 MAX_PIN_ATTEMPTS,
                 MAX_UNBLOCK_PIN_ATTEMPTS);
    }
    else
    {
      vgPrintf(entity, (const Char*)"*MUPIN: (0-4,10,11),(\"PIN\",\"PIN2\"),(0-1),%d,%d,(\"01\",\"81\")",
                 MAX_PIN_ATTEMPTS,
                 MAX_UNBLOCK_PIN_ATTEMPTS);
    }
    vgPutNewLine(entity);
  }
  else
  {
    vgPutNewLine(entity);
    vgPrintf(entity, (const Char*)"*MUPIN: (0-4),(\"PIN\",\"PIN2\"),(0-1),%d,%d",
               MAX_PIN_ATTEMPTS,
               MAX_UNBLOCK_PIN_ATTEMPTS);
    vgPutNewLine(entity);
  }
}

 /*--------------------------------------------------------------------------
 *
 * Function:        showMupinValue
 *
 * Parameters:
 *
 * Returns:
 *
 * Description:     Show MUPIN value for read command
 *
 *-------------------------------------------------------------------------*/
static void showMupinValue (const VgmuxChannelNumber entity, VgShowPinStatusFlag flag)
{
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
  Int8                     pinNumRemainingRetrys = 0;
  Int8                     unblockPinNumRemainingRetrys = 0;
  Int8                     pin2NumRemainingRetrys = 0;
  Int8                     unblockPin2NumRemainingRetrys = 0;
  VgSimState               simPin1State = VG_SIM_NOT_READY;
  VgSimState               simPin2State = VG_SIM_PIN2;
  Boolean                  pin1Enabled = FALSE;
  VgSimState               simUpinState = VG_SIM_READY;
  VgPinState               upinState;

  if (simLockGenericContext_p->simInfo.cardIsUicc == TRUE)
  {
    pin1Enabled = simLockGenericContext_p->simInfo.pin1Status.enabled;
    pinNumRemainingRetrys = simLockGenericContext_p->simInfo.pin1Status.numRemainingRetrys;
    unblockPinNumRemainingRetrys = simLockGenericContext_p->simInfo.unblockPin1Status.numRemainingRetrys;
  }
  else
  {
    /* It is a 2G SIM card. So pin is PIN1 */
    pin1Enabled = simLockGenericContext_p->simInfo.pinEnabled;
    pinNumRemainingRetrys = simLockGenericContext_p->simInfo.pinNumRemainingRetrys;
    unblockPinNumRemainingRetrys = simLockGenericContext_p->simInfo.unblockPinNumRemainingRetrys;
  }
  pin2NumRemainingRetrys = simLockGenericContext_p->simInfo.pin2NumRemainingRetrys;
  unblockPin2NumRemainingRetrys = simLockGenericContext_p->simInfo.unblockPin2NumRemainingRetrys;

  if ((simLockGenericContext_p->simState == VG_SIM_PIN2) ||
       (simLockGenericContext_p->simState == VG_SIM_PUK2)||
       (pin1Enabled == FALSE))
  {
    simPin1State = VG_SIM_READY;
  }
  else
  {
    simPin1State = simLockGenericContext_p->simState;
  }

  if (simLockGenericContext_p->simInfo.pin2Verified == TRUE)
  {
    simPin2State = VG_SIM_READY;
  }
  else
  {
    simPin2State = ((pin2NumRemainingRetrys == 0) ? VG_SIM_PUK2 : VG_SIM_PIN2);
  }
  vgPutNewLine(entity);

  if ((flag == VG_SHOW_PIN1) || (flag == VG_SHOW_ALL))
  {
    if (simLockGenericContext_p->simInfo.cardIsUicc == TRUE)
    {
      if ((simLockGenericContext_p->simInfo.universalPinSupportedByCard == TRUE) &&
             (simLockGenericContext_p->simInfo.pin1KeyRef == USIM_ACCESS_UNIVERSAL_PIN))
      {
        /* If PIN key reference is USIM_ACCESS_UNIVERSAL_PIN, PIN1 is disable */
        /* Set PIN to disabled and always verified */
        vgPrintf(entity, (const Char*)"*MUPIN: \"PIN\",0,\"%s\",%d,%d,\"01\"",
                 simStateMessage[VG_SIM_READY],
                 MAX_PIN_ATTEMPTS,
                 MAX_UNBLOCK_PIN_ATTEMPTS);
        vgPutNewLine(entity);
      }
      else
      {
        vgPrintf(entity, (const Char*)"*MUPIN: \"PIN\",%d,\"%s\",%d,%d,\"%x\"",
                  ((pin1Enabled == TRUE)? 1:0),
                  simStateMessage[simPin1State],
                  pinNumRemainingRetrys,
                  unblockPinNumRemainingRetrys,
                  simLockGenericContext_p->simInfo.pin1KeyRef);
        vgPutNewLine(entity);
      }
    }
    else
    {
      vgPrintf(entity, (const Char*)"*MUPIN: \"PIN\",%d,\"%s\",%d,%d",
          ((pin1Enabled == TRUE)? 1:0),
          simStateMessage[simPin1State],
          pinNumRemainingRetrys,
          unblockPinNumRemainingRetrys);
      vgPutNewLine(entity);
    }
  }

  if ((flag == VG_SHOW_PIN2) || (flag == VG_SHOW_ALL))
  {
    /* PIN2 is always enable */
    if (simLockGenericContext_p->simInfo.cardIsUicc == TRUE)
    {
      vgPrintf(entity, (const Char*)"*MUPIN: \"PIN2\",1,\"%s\",%d,%d,\"%x\"",
                simStateMessage[simPin2State],
                pin2NumRemainingRetrys,
                unblockPin2NumRemainingRetrys,
                simLockGenericContext_p->simInfo.pin2KeyRef);
      vgPutNewLine(entity);
    }
    else
    {
      vgPrintf(entity, (const Char*)"*MUPIN: \"PIN2\",1,\"%s\",%d,%d",
          simStateMessage[simPin2State],
          pin2NumRemainingRetrys,
          unblockPin2NumRemainingRetrys);
      vgPutNewLine(entity);
    }
  }

  if ((flag == VG_SHOW_UPIN) || (flag == VG_SHOW_ALL))
  {
    if ((simLockGenericContext_p->simInfo.cardIsUicc == TRUE) &&
         (simLockGenericContext_p->simInfo.universalPinSupportedByCard == TRUE))
    {
      if (simLockGenericContext_p->simInfo.universalPinStatus.enabled == TRUE)
      {
        if (simLockGenericContext_p->simInfo.universalPinStatus.used == TRUE)
        {
          upinState = VG_UPIN_ENABLED_USED;
        }
        else
        {
          upinState = VG_UPIN_ENABLED;
        }

        /* Set universal PIN SIM state */
        if (simLockGenericContext_p->simInfo.verifyUniversalPin == TRUE)
        {
          simUpinState = VG_SIM_READY;
        }
        else if (simLockGenericContext_p->simInfo.universalPinStatus.numRemainingRetrys == 0)
        {
          simUpinState = VG_SIM_UPUK;
        }
        else
        {
          simUpinState = VG_SIM_UPIN;
        }
      }
      else
      {
        simUpinState = VG_SIM_READY;
        upinState = VG_PIN_DISABLED;
      }


      vgPrintf(entity, (const Char*)"*MUPIN: \"UPIN\",%d,\"%s\",%d,%d,\"11\"",
                upinState,
                simStateMessage[simUpinState],
                simLockGenericContext_p->simInfo.universalPinStatus.numRemainingRetrys,
                simLockGenericContext_p->simInfo.unblockUniversalPinStatus.numRemainingRetrys);
      vgPutNewLine(entity);
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        verifyCommand
 *
 * Parameters:      accessLevel - MUPIN access level
 *
 * Returns:         Boolean true if ok.
 *
 * Description:     Verify if command can be operated. If PIN is enabled and not
 *                  verified, PIN2 and UPIN command can not be operated
 *-------------------------------------------------------------------------*/
static ResultCode_t verifyCommand (VgPinAccessLevel accessLevel)
{
  ResultCode_t             result = RESULT_CODE_OK;
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();

  if (accessLevel != VG_PIN)
  {
    if (simLockGenericContext_p->simState== VG_SIM_NOT_READY)
    {
      result = VG_CME_SIM_NOT_INSERTED;
    }
    else if (simLockGenericContext_p->simState== VG_SIM_PIN)
    {
      result = VG_CME_SIM_PIN_REQUIRED;
    }
    else if (simLockGenericContext_p->simState== VG_SIM_PUK)
    {
      result = VG_CME_SIM_PUK_REQUIRED;
    }
    else
    {
      result = RESULT_CODE_OK;
    }
  }
  else
  {
    result = RESULT_CODE_OK;
  }
  return (result);

}

 /*--------------------------------------------------------------------------
 *
 * Function:        enablePin
 *
 * Parameters:      commandBuffer_p
 *                  entity
 *                  mode - MUPIN operation mode
 *                  accessLevel - MUPIN access level
 * Returns:         Boolean true if ok.
 *
 * Description:     Enable or Disable PIN1 or UPIN
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t enablePin (CommandLine_t *commandBuffer_p,
                                const VgmuxChannelNumber entity,
                                 VgPinAccessMode mode,
                                  VgPinAccessLevel accessLevel,
                                   SimUiccKeyRefValue pinValue)

{
  ResultCode_t             result = RESULT_CODE_OK;
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
  GeneralContext_t         *generalContext_p = ptrToGeneralContext       (entity);
  VgSimInfo                *simInfo;
  Char                     pin[SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH] = {0};
  Int16                    pinLen;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (simLockGenericContext_p != PNULL, entity, 0, 0);
  FatalCheck (generalContext_p != PNULL, entity, 0, 0);
#endif
  simInfo = &simLockGenericContext_p->simInfo;
  if (getExtendedString(commandBuffer_p,
                         &pin[0],
                         SIM_CHV_LENGTH,
                         &pinLen) == TRUE)
  {
    memcpy (&generalContext_p->password[0], &pin[0], pinLen);
    generalContext_p->passwordLength = pinLen;
    memset( (Char *)&generalContext_p->password[pinLen],
        UCHAR_MAX,
        (SIM_CHV_LENGTH - pinLen));

    if ((mode == VG_ENABLE_PIN) || (mode == VG_DISABLE_PIN))
    {
      if (mode == VG_ENABLE_PIN)
      {
        generalContext_p->pinFunction = SIM_PIN_FUNCT_ENABLE;
      }
      else
      {
        generalContext_p->pinFunction = SIM_PIN_FUNCT_DISABLE;
      }

      if (simInfo->cardIsUicc == TRUE)
      {
        if (accessLevel == VG_PIN)
        {
          generalContext_p->keyRef = USIM_ACCESS_PIN1_APP1;
          if (pinValue == USIM_ACCESS_UNIVERSAL_PIN)
          {
            generalContext_p->altPinKeyReference = USIM_ACCESS_UNIVERSAL_PIN;
          }
          else
          {
            generalContext_p->altPinKeyReference = USIM_ACCESS_NO_PIN;
          }
          result = vgChManContinueAction (entity, SIG_APEX_SIM_PIN_FUNCTION_REQ);
        }
        else if (accessLevel == VG_UPIN)
        {
          generalContext_p->keyRef = USIM_ACCESS_UNIVERSAL_PIN;
          result = vgChManContinueAction (entity, SIG_APEX_SIM_PIN_FUNCTION_REQ);
        }
        else
        {
          result = VG_CMS_ERROR_INVALID_PARMETER;
        }
      }
      else
      {
        if (accessLevel == VG_PIN)
        {
          generalContext_p->chvNumber = SIM_CHV_1;
          result = vgChManContinueAction (entity, SIG_APEX_SIM_CHV_FUNCTION_REQ);
        }
        else
        {
          result = VG_CMS_ERROR_INVALID_PARMETER;
        }
      }
    }
    else
    {
      /* Invalid mode */
      FatalParam(entity, mode, 0);
    }
  }

  return (result);

}

 /*--------------------------------------------------------------------------
 *
 * Function:        verifyPin
 *
 * Parameters:      commandBuffer_p
 *                  entity
 *                  accessLevel - MUPIN access level
 *
 * Returns:         Boolean true if ok.
 *
 * Description:     Verify PIN
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t verifyPin (CommandLine_t *commandBuffer_p,
                               const VgmuxChannelNumber entity,
                                VgPinAccessLevel accessLevel)
{
  ResultCode_t             result = RESULT_CODE_OK;
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
  GeneralContext_t         *generalContext_p = ptrToGeneralContext       (entity);
  VgSimInfo                *simInfo = &simLockGenericContext_p->simInfo;
  Char                     pin[SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH] = {0};
  Int16                    pinLen;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  result = verifyCommand(accessLevel);
  if (result == RESULT_CODE_OK)
  {

    if (getExtendedString(commandBuffer_p,
                           &pin[0],
                           SIM_CHV_LENGTH,
                           &pinLen) == TRUE)
    {
      if ((pinLen >= MIN_CHV_SIZE) && (pinLen <= SIM_CHV_LENGTH))
      {
        memcpy( &simLockGenericContext_p->pinCode[0], &pin[0], pinLen );
        memset( &simLockGenericContext_p->pinCode[pinLen],
                 UCHAR_MAX,
                 (SIM_CHV_LENGTH - pinLen) );

        /* Initialise response params and send signal.... */
        simLockGenericContext_p->simBlocked = FALSE;
        if (simInfo->cardIsUicc == TRUE)
        {
          if (accessLevel == VG_PIN)
          {
            simLockGenericContext_p->keyRef = USIM_ACCESS_PIN1_APP1;
          }
          else if (accessLevel == VG_PIN2)
          {
            simLockGenericContext_p->keyRef = USIM_ACCESS_PIN2_APP1;
          }
          else
          {
            simLockGenericContext_p->keyRef = USIM_ACCESS_UNIVERSAL_PIN;
          }
        }
        else
        {
          if (accessLevel == VG_PIN)
          {
            simLockGenericContext_p->chvNum = SIM_CHV_1;
          }
          else
          {
            simLockGenericContext_p->chvNum = SIM_CHV_2;
          }

        }

        if (simLockGenericContext_p->awaitingChvRsp == TRUE)
        {
          if (simInfo->cardIsUicc == TRUE)
          {
              result = vgChManContinueAction (entity, SIG_APEX_SIM_GET_PIN_RSP);
          }
          else
          {
              result = vgChManContinueAction (entity, SIG_APEX_SIM_GET_CHV_RSP);
          }
        }
        else
        {
          memcpy (generalContext_p->password,
                   simLockGenericContext_p->pinCode,
                   SIM_CHV_LENGTH);
          generalContext_p->passwordLength = pinLen;
          if (simInfo->cardIsUicc == TRUE)
          {
            generalContext_p->pinFunction = SIM_PIN_FUNCT_VERIFY;
            generalContext_p->keyRef = simLockGenericContext_p->keyRef;
            result = vgChManContinueAction (entity, SIG_APEX_SIM_PIN_FUNCTION_REQ);
          }
          else
          {
            generalContext_p->pinFunction = SIM_PIN_FUNCT_VERIFY;
            generalContext_p->chvNumber   = simLockGenericContext_p->chvNum;
            result = vgChManContinueAction (entity, SIG_APEX_SIM_CHV_FUNCTION_REQ);
          }
        }
      }
      else
      {
        result = VG_CMS_ERROR_INVALID_PARMETER;
      }


    }
    else
    {
      result = VG_CMS_ERROR_INVALID_PARMETER;
    }
  }
  return (result);
}

 /*--------------------------------------------------------------------------
 *
 * Function:        unblockPin
 *
 * Parameters:      commandBuffer_p
 *                  entity
 *                  accessLevel - MUPIN access level
 *
 * Returns:         Boolean true if ok.
 *
 * Description:     Unblock PIN
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t unblockPin (CommandLine_t *commandBuffer_p,
                                const VgmuxChannelNumber entity,
                                 VgPinAccessLevel accessLevel)
{
  ResultCode_t             result = RESULT_CODE_OK;
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
  GeneralContext_t         *generalContext_p = ptrToGeneralContext       (entity);
  VgSimInfo                *simInfo = &simLockGenericContext_p->simInfo;
  Char                     pin[SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH] = {0};
  Int16                    pinLen;
  Char                     newPin[SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH] = {0};
  Int16                    newPinLen;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  result = verifyCommand(accessLevel);
  if (result == RESULT_CODE_OK)
  {
    if (getExtendedString(commandBuffer_p,
                           &pin[0],
                           SIM_CHV_LENGTH,
                           &pinLen) == TRUE)
    {
      if ((pinLen >= MIN_CHV_SIZE) && (pinLen <= SIM_CHV_LENGTH))
      {
        if (getExtendedString(commandBuffer_p,
                  &newPin[0],
                    SIM_CHV_LENGTH,
                     &newPinLen) == TRUE)
        {
          if ((newPinLen >= MIN_CHV_SIZE) &&
              (newPinLen <= SIM_CHV_LENGTH) &&
              (checkForNumericOnlyChars(newPin)) &&
              (checkForAdditionalPassword (commandBuffer_p,
                                            newPin,
                                             &newPinLen) == TRUE))
          {
            /* PUK & PIN entered.... */
            memcpy( &simLockGenericContext_p->pukCode[0],
                    &pin[0],
                    pinLen);
            memset( &simLockGenericContext_p->pukCode[pinLen],
                    UCHAR_MAX,
                    (SIM_CHV_LENGTH - pinLen));
            memcpy( &simLockGenericContext_p->pinCode[0],
                    &newPin[0],
                    newPinLen);
            memset( &simLockGenericContext_p->pinCode[newPinLen],
                    UCHAR_MAX,
                    (SIM_CHV_LENGTH - newPinLen));

            /* Initialise response params and send signal.... */
            simLockGenericContext_p->simBlocked = TRUE;

            if (simInfo->cardIsUicc == TRUE)
            {
              if (accessLevel == VG_PIN)
              {
                simLockGenericContext_p->keyRef = USIM_ACCESS_PIN1_APP1;
              }
              else if (accessLevel == VG_PIN2)
              {
                simLockGenericContext_p->keyRef = USIM_ACCESS_PIN2_APP1;
              }
              else
              {
                simLockGenericContext_p->keyRef = USIM_ACCESS_UNIVERSAL_PIN;
              }
            }
            else
            {
              if (accessLevel == VG_PIN)
              {
                simLockGenericContext_p->chvNum = SIM_CHV_1;
              }
              else
              {
                simLockGenericContext_p->chvNum = SIM_CHV_2;
              }

            }

            if (simLockGenericContext_p->awaitingChvRsp == TRUE)
            {
              if (simInfo->cardIsUicc == TRUE)
              {
                result = vgChManContinueAction (entity, SIG_APEX_SIM_GET_PIN_RSP);
              }
              else
              {
                result = vgChManContinueAction (entity, SIG_APEX_SIM_GET_CHV_RSP);
              }
            }
            else
            {
              generalContext_p->passwordLength = pinLen;
              generalContext_p->newPasswordLength = newPinLen;
              memcpy (generalContext_p->password,
                       simLockGenericContext_p->pukCode,
                        SIM_CHV_LENGTH);

              memcpy (generalContext_p->newPassword,
                       simLockGenericContext_p->pinCode,
                        SIM_CHV_LENGTH);

              if (simInfo->cardIsUicc == TRUE)
              {
                generalContext_p->keyRef   = simLockGenericContext_p->keyRef;
                generalContext_p->pinFunction = SIM_PIN_FUNCT_UNBLOCK;
                result = vgChManContinueAction (entity, SIG_APEX_SIM_PIN_FUNCTION_REQ);
              }
              else
              {
                generalContext_p->pinFunction = SIM_PIN_FUNCT_UNBLOCK;
                generalContext_p->chvNumber   = simLockGenericContext_p->chvNum;
                result = vgChManContinueAction (entity, SIG_APEX_SIM_CHV_FUNCTION_REQ);
              }
            }
          }
          else
          {
            /* Invalid length for 2nd pin.... */
            result = VG_CME_INCORRECT_PASSWORD;
          }
        }
        else
        {
          result = VG_CMS_ERROR_INVALID_PARMETER;
        }

      }
      else
      {
        result = VG_CMS_ERROR_INVALID_PARMETER;
      }

    }
    else
    {
      result = VG_CMS_ERROR_INVALID_PARMETER;
    }
  }
  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        changePin
 *
 * Parameters:      commandBuffer_p
 *                  entity
 *                  accessLevel - MUPIN access level
 *
 * Returns:         Boolean true if ok.
 *
 * Description:     Change PIN code
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t changePin (CommandLine_t *commandBuffer_p,
                                const VgmuxChannelNumber entity,
                                 VgPinAccessLevel accessLevel)
{
  ResultCode_t             result = RESULT_CODE_OK;
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
  GeneralContext_t         *generalContext_p = ptrToGeneralContext       (entity);
  VgSimInfo                *simInfo;
  Char              oldPassword[SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH] = {0};
  Char              newPassword[SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH] = {0};
  Int16             simOldPinLen;
  Int16             simNewPinLen;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (simLockGenericContext_p != PNULL, entity, 0, 0);
  FatalCheck (generalContext_p != PNULL, entity, 0, 0);
#endif
  simInfo = &simLockGenericContext_p->simInfo;

  result = verifyCommand(accessLevel);
  if (result == RESULT_CODE_OK)
  {
    if ((getExtendedString( commandBuffer_p,
                              &oldPassword[0],
                              SIM_CHV_LENGTH,
                              &simOldPinLen) == TRUE)      &&
         (getExtendedString( commandBuffer_p,
                                &newPassword[0],
                                  SIM_CHV_LENGTH,
                                   &simNewPinLen) == TRUE))
    {
      if ((simOldPinLen >= MIN_CHV_SIZE) &&
                    (simNewPinLen >= MIN_CHV_SIZE) &&
                    (simOldPinLen <= SIM_CHV_LENGTH) &&
                    (simNewPinLen <= SIM_CHV_LENGTH) &&
                    (checkForNumericOnlyChars(newPassword)) &&
                    (checkForAdditionalPassword (commandBuffer_p,
                                                  newPassword,
                                                   &simNewPinLen) == TRUE))
      {
        memcpy(&generalContext_p->password[0], &oldPassword[0], simOldPinLen);
        memset( (Char *)&generalContext_p->password[simOldPinLen],
                UCHAR_MAX,
                (SIM_CHV_LENGTH - simOldPinLen));

        memcpy(&generalContext_p->newPassword[0], &newPassword[0], simNewPinLen);
        memset( (Char *)&generalContext_p->newPassword[simNewPinLen],
                UCHAR_MAX,
                (SIM_CHV_LENGTH - simNewPinLen));

        if (simInfo->cardIsUicc == TRUE)
        {
          if (accessLevel == VG_PIN)
          {
            generalContext_p->keyRef = USIM_ACCESS_PIN1_APP1;
          }
          else if (accessLevel == VG_PIN2)
          {
            generalContext_p->keyRef = USIM_ACCESS_PIN2_APP1;
          }
          else
          {
            generalContext_p->keyRef = USIM_ACCESS_UNIVERSAL_PIN;
          }
        }
        else
        {
          if (accessLevel == VG_PIN)
          {
            generalContext_p->chvNumber = SIM_CHV_1;
          }
          else
          {
            generalContext_p->chvNumber = SIM_CHV_2;
          }

        }

        if (simLockGenericContext_p->simInfo.cardIsUicc == TRUE)
        {
          generalContext_p->pinFunction = SIM_PIN_FUNCT_CHANGE;
          result = vgChManContinueAction (entity, SIG_APEX_SIM_PIN_FUNCTION_REQ);
        }
        else
        {
          generalContext_p->pinFunction = SIM_PIN_FUNCT_CHANGE;
          result = vgChManContinueAction (entity, SIG_APEX_SIM_CHV_FUNCTION_REQ);
        }
      }
      else
      {
        result = VG_CMS_ERROR_INVALID_PARMETER;
      }
    }
    else
    {
      result = VG_CMS_ERROR_INVALID_PARMETER;
    }
  }
  return (result);
}

 /*--------------------------------------------------------------------------
 *
 * Function:        showCpinrValue
 *
 * Parameters:      entity:                   Entity on which to display value.
 *                  cpinrSimPinStatusValue:   Reference to CPINR value to be
 *                                            displayed.
 *
 * Returns:         Nothing
 *
 * Description:     Show selected CPINR value.
 *
 *-------------------------------------------------------------------------*/
static void showCpinrValue (const VgmuxChannelNumber entity, cpinrSimPinStatusType cpinrSimPinStatusValue)
{
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
  Int8                     pinNumRemainingRetrys = 0;
  Int8                     unblockPinNumRemainingRetrys = 0;

  switch (cpinrSimPinStatusValue)
  {
    case VG_CPINR_SIM_PIN:
      if (simLockGenericContext_p->simInfo.cardIsUicc == TRUE)
      {
        pinNumRemainingRetrys = simLockGenericContext_p->simInfo.pin1Status.numRemainingRetrys;
      }
      else
      {
        /* It is a 2G SIM card. So pin is PIN1 */
        pinNumRemainingRetrys = simLockGenericContext_p->simInfo.pinNumRemainingRetrys;
      }
      vgPrintf(entity, (const Char*)"%C: \"%s\",%d,%d",
               cpinrSimPinStatusMessage[VG_CPINR_SIM_PIN],
               pinNumRemainingRetrys,
               MAX_PIN_ATTEMPTS);
      vgPutNewLine(entity);
      break;
    case VG_CPINR_SIM_PUK:
      if (simLockGenericContext_p->simInfo.cardIsUicc == TRUE)
      {
        unblockPinNumRemainingRetrys = simLockGenericContext_p->simInfo.unblockPin1Status.numRemainingRetrys;
      }
      else
      {
        /* It is a 2G SIM card. So pin is PIN1 */
        unblockPinNumRemainingRetrys = simLockGenericContext_p->simInfo.unblockPinNumRemainingRetrys;
      }
      vgPrintf(entity, (const Char*)"%C: \"%s\",%d,%d",
               cpinrSimPinStatusMessage[VG_CPINR_SIM_PUK],
               unblockPinNumRemainingRetrys,
               MAX_UNBLOCK_PIN_ATTEMPTS);
      vgPutNewLine(entity);
      break;
    case VG_CPINR_SIM_PIN2:
      vgPrintf(entity, (const Char*)"%C: \"%s\",%d,%d",
               cpinrSimPinStatusMessage[VG_CPINR_SIM_PIN2],
               simLockGenericContext_p->simInfo.pin2NumRemainingRetrys,
               MAX_PIN_ATTEMPTS);
      vgPutNewLine(entity);
      break;
    case VG_CPINR_SIM_PUK2:
      vgPrintf(entity, (const Char*)"%C: \"%s\",%d,%d",
               cpinrSimPinStatusMessage[VG_CPINR_SIM_PUK2],
               simLockGenericContext_p->simInfo.unblockPin2NumRemainingRetrys,
               MAX_UNBLOCK_PIN_ATTEMPTS);
      vgPutNewLine(entity);
      break;
    case VG_CPINR_SIM_UPIN:
      if ((simLockGenericContext_p->simInfo.cardIsUicc == TRUE) &&
           (simLockGenericContext_p->simInfo.universalPinSupportedByCard == TRUE))
      {
        vgPrintf(entity, (const Char*)"%C: \"%s\",%d,%d",
                 cpinrSimPinStatusMessage[VG_CPINR_SIM_UPIN],
                 simLockGenericContext_p->simInfo.universalPinStatus.numRemainingRetrys,
                 MAX_PIN_ATTEMPTS);
        vgPutNewLine(entity);
      }
      break;
    case VG_CPINR_SIM_UPUK:
      if ((simLockGenericContext_p->simInfo.cardIsUicc == TRUE) &&
           (simLockGenericContext_p->simInfo.universalPinSupportedByCard == TRUE))
      {
        vgPrintf(entity, (const Char*)"%C: \"%s\",%d,%d",
                 cpinrSimPinStatusMessage[VG_CPINR_SIM_UPUK],
                 simLockGenericContext_p->simInfo.unblockUniversalPinStatus.numRemainingRetrys,
                 MAX_UNBLOCK_PIN_ATTEMPTS);
        vgPutNewLine(entity);
      }
      break;
    default:
      /* Not a legal CPINR PIN status value */
      FatalParam (entity, cpinrSimPinStatusValue, 0);
      break;
  }
}

 /*--------------------------------------------------------------------------
 *
 * Function:        showAllCpinrValues
 *
 * Parameters:      entity:     Entity on which to display CPINR values.
 *
 * Returns:         Nothing
 *
 * Description:     Show all available CPINR values
 *
 *-------------------------------------------------------------------------*/
static void showAllCpinrValues (const VgmuxChannelNumber entity)
{
  cpinrSimPinStatusType    cpinrSimPinStatusValue;

  vgPutNewLine(entity);
  for (cpinrSimPinStatusValue = VG_CPINR_SIM_PIN; cpinrSimPinStatusValue < NUM_CPINR_PIN_STATUS_VALUES; cpinrSimPinStatusValue++)
  {
    showCpinrValue (entity, cpinrSimPinStatusValue);
  }
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
*
* Function:     vgSlCPIN
*
* Parameters:   commandBuffer_p - pointer to command line string
*               entity          - mux channel number
*
* Returns:      Resultcode_t    - result of function
*
* Description:  Set command sends to the ME a password which is necessary
*               before it can be operated (SIM PIN, SIM PUK, PH-SIM
*               PIN, etc.).
*-------------------------------------------------------------------------*/

ResultCode_t vgSlCPIN(CommandLine_t *commandBuffer_p,
                      const VgmuxChannelNumber  entity)
{
  ResultCode_t            result = RESULT_CODE_OK;
  Char                    pin[SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH] = {0};
  Char                    newPin[SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH] = {0};
  Int16                   pinLen,
                          newPinLen;
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
  GeneralContext_t        *generalContext_p        = ptrToGeneralContext (entity);
  VgSimInfo               *simInfo = &(simLockGenericContext_p->simInfo);
  Int32                   chvParameter;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  switch( getExtendedOperation(commandBuffer_p) )
  {
    case EXTENDED_QUERY:  /* AT+CPIN? */
    {
      if (simLockGenericContext_p->simState != VG_SIM_NOT_READY)
      {
        /* Print out state of pin.... */
        vgPutNewLine (entity);
        vgPrintf (entity,
                   (const Char*)"+CPIN: %s",
                    simStateMessage[simLockGenericContext_p->simState]);

        vgPutNewLine (entity);
      }
      else
      {
        result = vgGetSimCmeErrorCode ();

#if defined (FEA_SIMLOCK)        
        if (VG_CME_SIM_WRONG == result)
        {
          result = vgChManContinueAction (entity, APEX_SIM_MEP_STATUS_REQ);
        }
#endif /* FEA_SIMLOCK */

      }
      break;
    }

    case EXTENDED_ASSIGN: /* AT+CPIN= */
    {
      // M_FrGkiPrintf0 (0xF28C, GKI_ATCI_INFO, "pb improve: at+pin");
      GKI_TRACE0 (PB_IMPROVE_AT_PIN, GKI_ATCI_INFO);
      if (simLockGenericContext_p->simState != VG_SIM_NOT_READY)
      {
        /* Fetch pin/puk code.... */
        if (getExtendedString(commandBuffer_p,
                             &pin[0],
                              SIM_CHV_LENGTH,
                             &pinLen) == TRUE)
        {
          /* Check entered string.... */
          /* Job 107943: Allow a null (zero-length) string
           * (test for zero length removed). */
          /* Job 109119: Check minimum Pin 1 length as per GSM 11.11 - 9.3 */
          if ((pinLen >= MIN_CHV_SIZE) && (pinLen <= SIM_CHV_LENGTH))
          {
            /* Have pin/puk, do we need a new pin?.... */
            if ((simLockGenericContext_p->simState == VG_SIM_PUK)     ||
#if !defined (ENABLE_VG_ETSI_STANDARD_OPERATION)
                (simLockGenericContext_p->simState == VG_SIM_READY)   ||
#endif
                (simLockGenericContext_p->simState == VG_SIM_PUK2))
            {
              /* Yes, if in PUK/PUK2 mode we need a new pin - the first
               * pin is the PUK, the next the new pin value....
               */
              if (getExtendedString(commandBuffer_p,
                              &newPin[0],
                                SIM_CHV_LENGTH,
                                 &newPinLen) == TRUE)
              {

                /* Job 107943: Allow a null (zero-length) string for new pin
                 * (test for zero length removed). */
                /* Job 109119: Check minimum new PIN length as per GSM 11.11 - 9.3
                   and it contains no alpha characters */
                if ((newPinLen >= MIN_CHV_SIZE) &&
                    (newPinLen <= SIM_CHV_LENGTH) &&
                    (checkForNumericOnlyChars(newPin)) &&
                    (checkForAdditionalPassword (commandBuffer_p,
                                                  newPin,
                                                   &newPinLen) == TRUE))
                {
                  /* PUK & PIN entered.... */
                  memcpy( &simLockGenericContext_p->pukCode[0],
                          &pin[0],
                          pinLen);
                  memset( &simLockGenericContext_p->pukCode[pinLen],
                          UCHAR_MAX,
                          (SIM_CHV_LENGTH - pinLen));
                  memcpy( &simLockGenericContext_p->pinCode[0],
                          &newPin[0],
                          newPinLen);
                  memset( &simLockGenericContext_p->pinCode[newPinLen],
                          UCHAR_MAX,
                          (SIM_CHV_LENGTH - newPinLen));

                  /* Initialise response params and send signal.... */
                  simLockGenericContext_p->simBlocked = TRUE;

                  if (simLockGenericContext_p->simState == VG_SIM_PUK2)
                  {
                    if ( simInfo->cardIsUicc == TRUE)
                    {
                        simLockGenericContext_p->keyRef = simInfo->pin2KeyRef;
                    }
                    else
                    {
                        simLockGenericContext_p->chvNum = SIM_CHV_2;
                    }
                  }
                  else
                  {
                    if ( simInfo->cardIsUicc == TRUE)
                    {
                        if (simInfo->verifyUniversalPin == TRUE)
                        {
                            simLockGenericContext_p->keyRef = USIM_ACCESS_UNIVERSAL_PIN;
                        }
                        else
                        {
                            simLockGenericContext_p->keyRef = simInfo->pin1KeyRef;
                        }
                    }
                    else
                    {
                        simLockGenericContext_p->chvNum = SIM_CHV_1;
                    }
                  }

                  if (simLockGenericContext_p->awaitingChvRsp == TRUE)
                  {
                    if (simInfo->cardIsUicc == TRUE)
                    {
                        FatalAssert ((simLockGenericContext_p->keyRef == simInfo->pin1KeyRef)||
                                   (simLockGenericContext_p->keyRef == USIM_ACCESS_UNIVERSAL_PIN));
                        result = vgChManContinueAction (entity, SIG_APEX_SIM_GET_PIN_RSP);
                    }
                    else
                    {
                        FatalAssert (simLockGenericContext_p->chvNum == SIM_CHV_1);
                        result = vgChManContinueAction (entity, SIG_APEX_SIM_GET_CHV_RSP);
                    }
                  }
                  else
                  {
                    memcpy (generalContext_p->password,
                             simLockGenericContext_p->pukCode,
                              SIM_CHV_LENGTH);

                    memcpy (generalContext_p->newPassword,
                             simLockGenericContext_p->pinCode,
                              SIM_CHV_LENGTH);

                    /* Check if 'invisible' parameter present - if so, verify whether we are checking the
                     * PUK2 value in READY mode....
                     */
                    (void)(getExtendedParameter (commandBuffer_p,
                                                 &chvParameter,
                                                 ULONG_MAX));
                    
                    if ((CPIN_SS_PUK2_REQUEST == chvParameter) &&
                        (VG_SIM_READY == simLockGenericContext_p->simState))
                    {
                      /* SS **052* request - check PIN2/CHV2.... */
                      if (simInfo->cardIsUicc == TRUE)
                      {
                        simLockGenericContext_p->keyRef = simInfo->pin2KeyRef;
                      }
                      else
                      {
                        simLockGenericContext_p->chvNum = SIM_CHV_2;
                      }
                    }

                    generalContext_p->pinFunction = SIM_PIN_FUNCT_UNBLOCK;
                    if (simInfo->cardIsUicc == TRUE)
                    {
                       generalContext_p->keyRef   = simLockGenericContext_p->keyRef;
                       result = vgChManContinueAction (entity, SIG_APEX_SIM_PIN_FUNCTION_REQ);
                    }
                    else
                    {
                       generalContext_p->chvNumber   = simLockGenericContext_p->chvNum;
                       result = vgChManContinueAction (entity, SIG_APEX_SIM_CHV_FUNCTION_REQ);
                    }
                  }
                }

                else
                {
                  /* Invalid length for 2nd pin.... */
                  result = VG_CME_INCORRECT_PASSWORD;
                }
              }

              else
              {
                /* New Pin/PUK not provided.... */
                result = vgGetSimCmeErrorCode();
              }
            }

            else if ( (simLockGenericContext_p->simState == VG_SIM_PIN)     ||
                      (simLockGenericContext_p->simState == VG_SIM_PIN2))
            {
              /* No, we just require the PIN code.  Pad out string with
               * trailing 0xFF bytes....
               */
              memcpy( &simLockGenericContext_p->pinCode[0], &pin[0], pinLen);
              memset( &simLockGenericContext_p->pinCode[pinLen],
                      UCHAR_MAX,
                      (SIM_CHV_LENGTH-pinLen));
              memset( &simLockGenericContext_p->pukCode[0],
                      UCHAR_MAX,
                      SIM_CHV_LENGTH);

              /* Initialise response params and send signal.... */
              simLockGenericContext_p->simBlocked = FALSE;

              if (simLockGenericContext_p->simState == VG_SIM_PIN)
              {
                if (simInfo->cardIsUicc == TRUE)
                {
                    if (simInfo->verifyUniversalPin == TRUE)
                    {
                        simLockGenericContext_p->keyRef = USIM_ACCESS_UNIVERSAL_PIN;
                    }
                    else
                    {
                        simLockGenericContext_p->keyRef = simInfo->pin1KeyRef;
                    }
                }
                else
                {
                    simLockGenericContext_p->chvNum = SIM_CHV_1;
                }
              }

              else
              {
                if (simInfo->cardIsUicc == TRUE)
                {
                    simLockGenericContext_p->keyRef = simInfo->pin2KeyRef;
                }
                else
                {
                    simLockGenericContext_p->chvNum = SIM_CHV_2;
                }
              }

              if (simLockGenericContext_p->awaitingChvRsp == TRUE)
              {
                if (simInfo->cardIsUicc == TRUE)
                {
                    FatalAssert ((simLockGenericContext_p->keyRef == simInfo->pin1KeyRef)||
                               (simLockGenericContext_p->keyRef == USIM_ACCESS_UNIVERSAL_PIN));
                    result = vgChManContinueAction (entity, SIG_APEX_SIM_GET_PIN_RSP);

                }
                else
                {
                    FatalAssert (simLockGenericContext_p->chvNum == SIM_CHV_1);

                    result = vgChManContinueAction (entity, SIG_APEX_SIM_GET_CHV_RSP);
                }
              }
              else
              {
                memcpy (generalContext_p->password,
                         simLockGenericContext_p->pinCode,
                          SIM_CHV_LENGTH);

                memcpy (generalContext_p->newPassword,
                         simLockGenericContext_p->pukCode,
                          SIM_CHV_LENGTH);

                generalContext_p->pinFunction = SIM_PIN_FUNCT_VERIFY;

                if (simInfo->cardIsUicc == TRUE)
                {
                   generalContext_p->keyRef   = simLockGenericContext_p->keyRef;
                   result = vgChManContinueAction (entity, SIG_APEX_SIM_PIN_FUNCTION_REQ);

                }
                else
                {
                   generalContext_p->chvNumber   = simLockGenericContext_p->chvNum;
                   result = vgChManContinueAction (entity, SIG_APEX_SIM_CHV_FUNCTION_REQ);
                }
              }
            }

            else
            {
              /* Invalid operation.... */
              result = VG_CME_OPERATION_NOT_ALLOWED;
            }
          }

          else
          {
            /* Report invalid text input.... */
            result = VG_CME_INCORRECT_PASSWORD;
          }
        }

        else
        {
          /* Invalid input response..... */
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }

      else
      {
        /* In NOT_READY state, report error.... */
        result = vgGetSimCmeErrorCode();
      }
      break;
    }

    case EXTENDED_RANGE:  /* AT+CPIN=? */
    {
      /* Spec defines this just to return 'OK'.... */
      break;
    }

    case EXTENDED_ACTION: /* AT+CPIN */
    default:
    {
      result = RESULT_CODE_ERROR;
    }
    break;
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSlCFUN
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+CFUN (Phone Functionality) command.
*              The set command selects the level of functionality <fun> in the
*              ME. Level "full functionality" is where the highest level of
*              power is drawn. "Minimum functionality" is where minimum power
*              is drawn. Level of functionality between these may also be
*              specified by manufacturers. When supported by manufacturers,
*              ME resetting with <rst> parameter may be utilized.
*              Defined values:-
*              <fun>: 0   minimum functionality
*                     1   full functionality
*                     2   disable phone transmit RF circuits only
*                     3   disable phone receive RF circuits only
*                     4   disable phone both transmit and receive RF circuits
*                     5...127 reserved for manufacturers as intermediate
*                         states between full and minimum functionality
*              <rst>: 0   do not reset the ME before setting it to <fun> power
*                         level. This is the default when <rst> is not given.
*                     1   reset the ME before setting it to <fun> power level
*-------------------------------------------------------------------------*/
ResultCode_t vgSlCFUN ( CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
  ResultCode_t     result            = RESULT_CODE_OK;
  Int32            functionality;
  Int32            application;
  Boolean          functionalityOk   = FALSE;
  Boolean          applicationOk     = FALSE;
  SimLockContext_t        *simLockContext_p        = ptrToSimLockContext (entity);
  VgCFUNData       *vgCFUNData_p;
  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  VgMnvmMcalContext       *vgMnvmMcalContext_p      = &(generalGenericContext_p->vgMnvmMcalContext);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
  vgCFUNData_p = &(simLockContext_p->vgCFUNData);

  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_RANGE:    /* AT+CFUN=? */
    {
      vgPutNewLine(entity);
      vgPuts(entity, (const Char*)"+CFUN: (0,1,4,7),(0-2)");
      break;
    }
    case EXTENDED_ASSIGN:   /* AT+CFUN=  */
    {
      /* Get functionality parameter (mandatory) */
      if (getExtendedParameter (commandBuffer_p,
                                 &functionality,
                                  ULONG_MAX) == TRUE)
      {
        if ((functionality == VG_AT_CFUN_MIN_FUNC)      ||
            (functionality == VG_AT_CFUN_FULL_FUNC)     ||
            (functionality == VG_AT_CFUN_DISABLE_RF_TX) ||
            (functionality == VG_AT_CFUN_DISABLE_RF_RX) ||
            (functionality == VG_AT_CFUN_FULL_RF_CLOSED_SIM) ||
            (functionality == VG_AT_CFUN_DISABLE_RF_RXTX))
        {
          if(VG_CALDEV_ENABLED == vgMnvmMcalContext_p->vgCaldevStatus)
          {
             return VG_CME_OPERATION_NOT_ALLOWED;
          }
          functionalityOk = TRUE;

#if defined (COARSE_TIMER)
          if(simLockContext_p->vgCFUNData.functionality != functionality)
          {
            simLockContext_p->vgCFUNData.functionality = functionality;
            if( (functionality == VG_AT_CFUN_MIN_FUNC) || (functionality == VG_AT_CFUN_DISABLE_RF_RXTX) )
            {
               vgSigConModemStatusInd(PROTOCOL_STACK_PWR_DOWN);
            }
            else if ( (functionality == VG_AT_CFUN_FULL_FUNC) ||(functionality == VG_AT_CFUN_FULL_RF_CLOSED_SIM))
            {
               vgSigConModemStatusInd(PROTOCOL_STACK_PWR_UP);
            }
          }
#endif
        }
      }

      /* Get reset parameter (optional) */
      if (getExtendedParameter (commandBuffer_p,
                                 &application,
                                  VG_AT_CFUN_APPLY_NOW) == TRUE)
      {
        if ((application == VG_AT_CFUN_APPLY_NOW) ||
            (application == VG_AT_CFUN_APPLY_ON_RESET_AND_NOW) ||
            (application == VG_AT_CFUN_APPLY_ON_RESET))
        {
          /* update application data */
          vgCFUNData_p->application = (VgCFUNApplication)application;

          applicationOk = TRUE;
        }
      }

      /* parameters within acceptable ranges continue with command */
      if (functionalityOk && applicationOk)
      {
        /* set-up command context information */

        switch ((VgCFUNApplication)application)
        {
          case VG_AT_CFUN_APPLY_ON_RESET:
          case VG_AT_CFUN_APPLY_ON_RESET_AND_NOW:
          {
            vgCFUNData_p->resetNow = FALSE;
            break;
          }
          case VG_AT_CFUN_APPLY_NOW:
          {
            vgCFUNData_p->resetNow = TRUE;
            break;
          }
          default:
          {
            /* Invalid application value */
            FatalParam(entity, application, 0);
            break;
          }
        }

        switch ((VgCFUNType)functionality)
        {
          case VG_AT_CFUN_MIN_FUNC:
          {
            /* reduce to minimum functionality */
            vgCFUNData_p->powerUpSim        = FALSE;
            vgCFUNData_p->powerUpProtoStack = FALSE;
            result = vgChManContinueAction (entity, SIG_APEX_PM_MODE_CHANGE_REQ);
            break;
          }
          case VG_AT_CFUN_FULL_FUNC:
          {
            /* increase to full functionality */
            vgCFUNData_p->powerUpSim        = TRUE;
            vgCFUNData_p->powerUpProtoStack = TRUE;

            /* If the SIM was already off then we need to reset the
             * SIM inserted state so we will generate a *MSIMINS unsolicited event
             * indicating the SIM is not there when we try to access it
             */
            psc_set_power_state(PSC_PWR_STATE_INACTIVE,FALSE);
            result = vgChManContinueAction (entity, SIG_APEX_PM_MODE_CHANGE_REQ);
            break;
          }
          case VG_AT_CFUN_DISABLE_RF_RXTX:
          {
            vgCFUNData_p->powerUpSim        = TRUE;
            vgCFUNData_p->powerUpProtoStack = FALSE;

            /* If the SIM was already off then we need to reset the
             * SIM inserted state so we will generate a *MSIMINS unsolicited event
             * indicating the SIM is not there when we try to access it
             */

            result = vgChManContinueAction (entity, SIG_APEX_PM_MODE_CHANGE_REQ);
            break;
          }
          case VG_AT_CFUN_FULL_RF_CLOSED_SIM:
          {
            vgCFUNData_p->powerUpSim        = FALSE;
            vgCFUNData_p->powerUpProtoStack = TRUE;
            result = vgChManContinueAction (entity, SIG_APEX_PM_MODE_CHANGE_REQ);
            break;
          }
          default:
          {
            /* the requested level of functionality is not supported */
            result = VG_CME_UNSUPPORTED_MODE;
            break;
          }
        }
      }
      else /* functionality/application parameter not specified/in allowed range */
      {
        result = VG_CME_INVALID_INPUT_VALUE;
      }
      break;
    }
    case EXTENDED_QUERY:    /* AT+CFUN?  */
    {
      viewCFUN (entity);
      break;
    }
    case EXTENDED_ACTION:   /* AT+CFUN */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSlMSIMINS
*
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT*MSIMINS command which controls
*              whether an unsolicited event code (indicating whether the
*              SIM has just been inserted/removed) should be sent to the
*              terminal.
*-------------------------------------------------------------------------*/

ResultCode_t vgSlMSIMINS (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity)
{
  ResultCode_t             result = RESULT_CODE_OK;
  ExtendedOperation_t      operation = getExtendedOperation (commandBuffer_p);
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
  Int32                    mode;


  switch (operation)
  {
    case EXTENDED_RANGE:  /* *MSIMINS=? */
    {
      vgPutNewLine(entity);
      vgPrintf(entity, (const Char*)"%C: (0-1)");
      vgPutNewLine(entity);
      break;
    }
    case EXTENDED_ASSIGN: /* *MSIMINS=<mode> */
    {
      if (getExtendedParameter (commandBuffer_p, &mode, ULONG_MAX) == TRUE)
      {
        /* if the mode is invalid or does not exist then send error. */
        if (mode <= 1)
        {
          result = setProfileValue (entity, PROF_MSIMINS, (Int8)mode);
        }
        else
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }
      break;
    }
    case EXTENDED_QUERY:    /* AT*MSIMINS?  */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"%C: %d,%d",
                        (Int16)getProfileValue(entity, PROF_MSIMINS),
                        (Int16)(simLockGenericContext_p->simInsertedState == VG_SIM_INSERTED)?1:0);
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_ACTION:   /* AT*MSIMINS  */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSlMUPIN
*
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Use to get the current avaliable PINs status information used
*              by the SIM for the verification of access confition level 1,
*              level 2 and alternative PIN
*
*-------------------------------------------------------------------------*/

ResultCode_t vgSlMUPIN (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity)
{
  ResultCode_t             result                   = RESULT_CODE_OK;
  ExtendedOperation_t      operation                = getExtendedOperation (commandBuffer_p);
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
  Int32                    mode;
  Char                     *str_par                 = PNULL;
  Int16                    stringLength             = 0;
  VgPinAccessLevel         accessLevel;
  Int16                    parameterNum             = 0;
  Int16                    commandPosition;

 
#if defined (ENABLE_LONG_AT_CMD_RSP)
  KiAllocZeroMemory (sizeof(Char) * (AT_MEDIUM_LARGE_BUFF_SIZE + NULL_TERMINATOR_LENGTH), (void **)&str_par);
#else
  KiAllocZeroMemory (sizeof(Char) * (COMMAND_LINE_SIZE + NULL_TERMINATOR_LENGTH), (void **)&str_par);
#endif

  switch (operation)
  {
    case EXTENDED_RANGE:  /* *MUPIN=? */
    {
      showMupinRange(entity);
    }
    break;

    case EXTENDED_QUERY:    /* AT*MUPIN?  */
    {
      showMupinValue(entity, VG_SHOW_ALL);
    }
    break;

    case EXTENDED_ASSIGN: /* *MUPIN= */
    {
      if (getExtendedParameter(commandBuffer_p,
                                  &mode,
                                  NUM_OF_VG_PIN_MODE) == TRUE)
      {
        if ((getExtendedString(commandBuffer_p,
                                 str_par,
#if defined (ENABLE_LONG_AT_CMD_RSP)
                                 AT_MEDIUM_LARGE_BUFF_SIZE,
#else
                                 COMMAND_LINE_SIZE,
#endif
                                 &stringLength) == TRUE) &&
                                 (stringLength != 0))
         {
           switch ((VgPinAccessMode)mode)
           {
             case VG_ENABLE_PIN:
             case VG_DISABLE_PIN:
             {
               if ((convertLevelStringToLevel(str_par, &accessLevel)) && (accessLevel != VG_PIN2))

               {
                 if (simLockGenericContext_p->simState != VG_SIM_NOT_READY)
                 {
                   result = enablePin(commandBuffer_p,
                                       entity,
                                        (VgPinAccessMode)mode,
                                         accessLevel,
                                          USIM_ACCESS_NO_PIN);
                 }
                 else
                 {
                   result = RESULT_CODE_ERROR;
                 }
               }
               else
               {
                 result = VG_CMS_ERROR_INVALID_PARMETER;
               }
               break;
             }

             case VG_CHANGE_PIN:
             {
               if (convertLevelStringToLevel(str_par, &accessLevel))
               {
                 result = changePin(commandBuffer_p, entity, accessLevel);
               }
               else
               {
                 result = VG_CMS_ERROR_INVALID_PARMETER;
               }
               break;
             }

             case VG_VERIFY_PIN:
             {
               if (convertLevelStringToLevel(str_par, &accessLevel))
               {
                 commandPosition = commandBuffer_p->position - 1;
                 while ((commandPosition < commandBuffer_p->length) &&
                        (commandBuffer_p->character[commandPosition] != SEMICOLON_CHAR) &&
                        (commandBuffer_p->character[commandPosition] != getProfileValue (entity, PROF_S3)))
                 {
                   if (commandBuffer_p->character[commandPosition] == COMMA_CHAR)
                   {
                     parameterNum++;
                   }
                   commandPosition++;
                 }

                 if (parameterNum == 1)
                 {
                   result = verifyPin(commandBuffer_p, entity, accessLevel);
                 }
                 else
                 {
                   result = unblockPin(commandBuffer_p, entity, accessLevel);
                 }
               }
               else
               {
                 result = VG_CMS_ERROR_INVALID_PARMETER;
               }
               break;
             }

             case VG_QUERY_PIN_STATUS:
             {
               if (convertLevelStringToLevel(str_par, &accessLevel))

               {
                 switch (accessLevel)
                 {
                   case VG_PIN:
                     showMupinValue(entity, VG_SHOW_PIN1);
                     break;
                   case VG_PIN2:
                     showMupinValue(entity, VG_SHOW_PIN2);
                     break;
                   case VG_UPIN:
                     showMupinValue(entity, VG_SHOW_UPIN);
                     break;

                   default:
                     result = VG_CMS_ERROR_INVALID_PARMETER;
                     break;
                 }
               }
               else
               {
                 result = VG_CMS_ERROR_INVALID_PARMETER;
               }
               break;
             }

             case VG_NOT_USE_UPIN:
             {
               if ((convertLevelStringToLevel(str_par, &accessLevel)) && (accessLevel == VG_PIN))
               {
                 result = enablePin(commandBuffer_p,
                                    entity,
                                    VG_ENABLE_PIN,
                                    VG_PIN,
                                    USIM_ACCESS_NO_PIN);
               }
               else
               {
                 result = VG_CMS_ERROR_INVALID_PARMETER;
               }
               break;
             }
             case VG_DISABLE_PIN_USE_UPIN:
             {
               if ((convertLevelStringToLevel(str_par, &accessLevel)) && (accessLevel == VG_PIN))
               {
                 result = enablePin(commandBuffer_p,
                                    entity,
                                    VG_DISABLE_PIN,
                                    VG_PIN,
                                    USIM_ACCESS_UNIVERSAL_PIN);
               }
               else
               {
                 result = VG_CMS_ERROR_INVALID_PARMETER;
               }
               break;
             }

             default:
               break;
           }
         }
         else
         {
           result = VG_CME_INVALID_INPUT_VALUE;
         }
       }
       else
       {
         result = VG_CME_INVALID_INPUT_VALUE;
       }
    }
    break;

    case EXTENDED_ACTION:   /* AT*MUPIN  */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  KiFreeMemory( (void **)&str_par);

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSetSimInsertionState
 *
 * Parameters:  newSimInsertionState- the new SIM insertion state
 *
 * Returns:     Nothing
 *
 * Description: Sets the SIM insertion state, if it has changed then send
 *              *MSIMINS unsolicited information if enabled
 *-------------------------------------------------------------------------*/

void vgSetSimInsertionState (const VgSimInsertedState newSimInsertionState)
{
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
  VgmuxChannelNumber       profileEntity = VGMUX_CHANNEL_INVALID;
  Boolean                  displayChange = FALSE;

  /* check if the SIM insertion state has changed */
  if (simLockGenericContext_p->simInsertedState != newSimInsertionState)
  {
     switch (newSimInsertionState)
     {
          case VG_SIM_INSERTED:
          case VG_SIM_NOT_INSERTED:
              displayChange = TRUE;
              break;
          case VG_SIM_INSERTED_STATE_UNKNOWN:
              /* Only display if state has gone from inserted to unknown.
               */
              if (simLockGenericContext_p->simInsertedState == VG_SIM_INSERTED)
              {
                  displayChange = TRUE;
              }
              break;
          default:
              /* Unknown state */
              FatalParam(newSimInsertionState, simLockGenericContext_p->simInsertedState, 0);
              break;
     }

     simLockGenericContext_p->simInsertedState = newSimInsertionState;

     if (displayChange == TRUE)
     {
         for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
         {
             if (isEntityActive(profileEntity))
             {
                 /* display unsolicited insertion indication if enabled */
                 if (getProfileValue (profileEntity, PROF_MSIMINS) == REPORTING_ENABLED)
                 {
                     vgPutNewLine (profileEntity);

                     vgPrintf (profileEntity,
                                (const Char*)"*MSIMINS: %d,%d",
                                 (Int16)getProfileValue(profileEntity, PROF_MSIMINS),
                                  (Int16)(simLockGenericContext_p->simInsertedState == VG_SIM_INSERTED)?1:0);
                     vgPutNewLine (profileEntity);
                     vgSetCirmDataIndIsUrc(profileEntity, TRUE);
                     vgFlushBuffer (profileEntity);
                 }
             }
         }
     }
  }

}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSetSimState
 *
 * Parameters:  VgSimState - the new SIM state to be looked at
 *
 * Returns:     Nothing
 *
 * Description: Sets the current SIM state
 *-------------------------------------------------------------------------*/

void vgSetSimState (const VgSimState newSimState)
{
  SimLockGenericContext_t   *simLockGenericContext_p    = ptrToSimLockGenericContext ();

  Boolean                   setSimState                 = TRUE;
  Boolean                   setSimInsertionState        = TRUE;
  VgmuxChannelNumber        cIndex                      = 0;

  FatalAssert(newSimState < NUM_OF_VG_SIM_STATES);

  /* set sim lock status when apropriate */
  switch (newSimState)
  {
    case VG_SIM_NOT_READY:
    {
      setSimInsertionState = FALSE;
      simLockGenericContext_p->simLocked = TRUE;
      break;
    }
    case VG_SIM_PIN:
    case VG_SIM_PUK:
    {
      simLockGenericContext_p->simLocked = TRUE;
      break;
    }
    case VG_SIM_PIN2:
    case VG_SIM_PUK2:
    {
      /* if previous simState was PIN/PUK then don't change sim state
       * since PIN2/PUK2 has a lower priority */
      if ((simLockGenericContext_p->simState == VG_SIM_PIN) ||
          (simLockGenericContext_p->simState == VG_SIM_PUK))
      {
        setSimState = FALSE;
      }
      break;
    }
    case VG_SIM_READY:
    {
      simLockGenericContext_p->simLocked = FALSE;
      break;
    }
    default:
    {
      /* do nothing */
      break;
    }
  }

  /* to enter pin/ready state the SIM must be inserted */
  if (setSimInsertionState == TRUE)
  {
    vgSetSimInsertionState (VG_SIM_INSERTED);
  }

  /* only output indication if value changed */
  if ((simLockGenericContext_p->simState != newSimState) &&
      (setSimState == TRUE))
  {
    /* if we are moving from NOT_READY then start any Cid Configuration */
    simLockGenericContext_p->simState = newSimState;

    /* If we have unsolicited channel enabled - only generate "CPIN:" on that channel */
    if (vgGetMmiUnsolicitedChannel() == VGMUX_CHANNEL_INVALID)
    {
      for (cIndex = 0; cIndex < CI_MAX_ENTITIES; cIndex++)
      {
        if (((TRUE == isEntityActive (cIndex)) && (!isEntityMmiNotUnsolicited(cIndex)))
        && (ENTITY_IDLE == getEntityState(cIndex)))
        {
           /* display CPIN message */
           vgPutNewLine (cIndex);
           vgPrintf (cIndex,
                   (const Char*)"+CPIN: %s",
                      simStateMessage[newSimState]);
           vgPutNewLine (cIndex);

           vgSetCirmDataIndIsUrc(cIndex, TRUE);

           vgFlushBuffer (cIndex);
        }
      }
    }
    else
    {
      cIndex    =  vgGetMmiUnsolicitedChannel();
      /* display CPIN message */
      vgPutNewLine (cIndex);
      vgPrintf (cIndex,
                 (const Char*)"+CPIN: %s",
                  simStateMessage[newSimState]);
      vgPutNewLine (cIndex);

      vgSetCirmDataIndIsUrc(cIndex, TRUE);
      
      vgFlushBuffer (cIndex);   
    }
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgIndicatePukBlocked
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 * Description: Generated an unsolicited event to indicate the SIM is PUK
 *              blocked.
 *-------------------------------------------------------------------------*/
void vgIndicatePukBlocked   (void)
{
  VgmuxChannelNumber cIndex    = 0;

  for (cIndex = 0;
        cIndex < CI_MAX_ENTITIES;
         cIndex++)
  {
    if ((isEntityActive (cIndex) == TRUE)
#if !defined (MODEM_STANDALONE)
      &&(!isEntityMmiNotUnsolicited(cIndex))
#endif
      )
    {
      /* display *MSMPULBLKD message */
      vgPutNewLine (cIndex);

      vgPrintf (cIndex,
                 (const Char*)"*MSMPUKBLKD");

      vgPutNewLine (cIndex);

      vgFlushBuffer (cIndex);
    }
  }
}



/*--------------------------------------------------------------------------
*
* Function:     vgSlCSIM
*
* Parameters:   in:  commandBuffer_p - the AT command line string
*
* Returns:      ResultCode_t - status of extraction
*
* Description:  Executes the +CSIM AT command which allows the user to
*               to interact directly with the SIM.
*
*               +CSIM= <length>,<command>   - set command
*               Set command transmits to the ME the <command> it then shall
*               send as it is to the SIM.
*
*               In the same manner the SIM <response> shall be sent back
*               by the ME to the TA as it is. Format:
*
*               +CSIM: <length>,<response>
*
*-------------------------------------------------------------------------*/

ResultCode_t vgSlCSIM (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
  SimLockContext_t        *simLockContext_p = ptrToSimLockContext (entity);
  ResultCode_t            resultCode = RESULT_CODE_OK;
  Int32                   length;           /* Length entered by user */
  Char                    *commandString_p = PNULL;
  Int8                    *csimCommandString_p = PNULL;
  Int16                   commandStringLen = 0;

  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_RANGE:    /* AT+CSIM=? */
    {
      /* just output ok */
      resultCode = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_ASSIGN:   /* AT+CSIM=  */
    {
#if defined (ATCI_SLIM_DISABLE)

      FatalCheck(simLockContext_p != PNULL, entity, 0, 0);

#endif

      if ((simLockContext_p->csimLocked == TRUE)||(noChannelCsimLocked()))
      {
        KiAllocZeroMemory (sizeof(Char) * (VG_CSIM_MAX_COMMAND_STRING + NULL_TERMINATOR_LENGTH),
                             (void **) &commandString_p);
        KiAllocZeroMemory (sizeof(Int8) * (VG_CSIM_COMMAND_LENGTH),
                             (void **) &csimCommandString_p);

        if (simLockGenericContext_p->simState == VG_SIM_READY)
        {
          /* get the length */
          if (getExtendedParameter (commandBuffer_p, &length, ULONG_MAX) == TRUE)
          {
            /* check length validity. Must be > 0
            * and even and <= Max msg length     */
            if ((length == ULONG_MAX) ||
                (length < 8) ||
                (length > VG_CSIM_MAX_COMMAND_STRING) ||
                ((length % 2) != 0))
            {
              resultCode = VG_CME_CSIM_LENGTH_INCORRECT;
            }
          }
          else
          {
            /* must a length */
            resultCode = VG_CME_CSIM_LENGTH_INCORRECT;
          }

          /* now get the command string */
          if (RESULT_CODE_OK == resultCode )
          {
            if (getExtendedString (commandBuffer_p,
                                   (Char*)commandString_p,
                                    VG_CSIM_MAX_COMMAND_STRING,
                                     &commandStringLen) == TRUE)
            {
              if (commandStringLen > 0)
              {
                /* length must be twice the input string size */
                if (length != (Int32)commandStringLen)
                {
                  resultCode = VG_CME_CSIM_LENGTH_INCORRECT;
                }
              }
              else
              {
                /* no parameter so error */
                resultCode = VG_CME_INVALID_INPUT_VALUE;
              }

              if (RESULT_CODE_OK == resultCode )
              {
                /* Convert Char string into an array of Int8's */
                memset (csimCommandString_p, 0, VG_CSIM_COMMAND_LENGTH * sizeof (Int8));

                vgMapTEToGsm (csimCommandString_p,
                               VG_CSIM_COMMAND_LENGTH,
                                commandString_p,
                                 commandStringLen,
                                  VG_AT_CSCS_HEX,
                                   entity);


                resultCode = vgSendApexSimGenAccessReq (entity,
                                                        (Int16)(length / 2),
                                                        csimCommandString_p,
                                                        (Int16)SIM_EF_INVALID,
                                                        (Int16)SIM_DIR_INVALID,
                                                        (Int16)SIM_DIR_INVALID,
                                                        /* job134856: add handling for <pathid> field */
                                                        /* field not used for +CSIM command */
                                                        PNULL,
                                                        0);

              }
            }
            else /* error reading supplied parameter */
            {
              resultCode = VG_CME_CSIM_INVALID_INPUT_STRING;
            }
          }
        }
        else
        {
          resultCode = vgGetSimCmeErrorCode ();
        }

        KiFreeMemory( (void**)&commandString_p);
        KiFreeMemory( (void**)&csimCommandString_p);
      }
      else
      {
        resultCode = RESULT_CODE_ERROR;
      }
      break;
    }
    case EXTENDED_QUERY:    /* AT+CSIM?  */
    case EXTENDED_ACTION:   /* AT+CSIM   */
    default:
    {
      resultCode = RESULT_CODE_ERROR;
      break;
    }
  } /* end case */

  return (resultCode);
}




/*--------------------------------------------------------------------------
*
* Function:     vgSlCRSM
*
* Parameters:   in:  commandBuffer_p - the AT command line string
*
* Returns:      ResultCode_t - status of extraction
*
* Description:  Executes the +CSRM AT command which allows the user to
*               to interact directly with the SIM - but is more
*               restrictive that the +CSIM command but easier to use.
*
*               +CRSM= <command>[,<fileid>[,<P1>,<P2>,<P3>[,<data>]]]
*
*               The set command transmits to the ME the SIM <command>
*               and its required parameters.
*
*               The response takes the following format:
*
*               +CRSM: <sw1>,<sw2>[,<response>]
*
*               or error.
*
* Note:         See GSM 07.07 for command description and GSM 11.11 for
*               details on the SIM command structure.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgSlCRSM (CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
   SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
   ResultCode_t            resultCode = RESULT_CODE_OK;
   Char                    *commandDataString_p = PNULL;
   Int16                   commandDataStringLength = 0;
   Int8                    *commandData_p = PNULL;
   Int16                   commandDataLen = 0;
   Int8                    p1;
   Int8                    p2;
   Int8                    p3;
   Int8                    cmd = 0;
   Int8                    sessionId = 0;
   SimEfId                 fileId;
   SimDirId                dirId;
   /* added for job134856 */
   Int8                    pathData[MAX_SELECTION_PATH_LEN] = {0};
   Int8                    pathLength = 0;
   SimLockContext_t        *simLockContext_p = ptrToSimLockContext (entity);
#if defined (ATCI_SLIM_DISABLE)

   FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
   switch (getExtendedOperation (commandBuffer_p))
   {
     case EXTENDED_RANGE:    /* AT+CRSM=? */
     {
       /* just output 'OK' */
       resultCode = RESULT_CODE_OK;
       break;
     }
     case EXTENDED_ASSIGN:   /* AT+CRSM=  */
     {
       if ((simLockContext_p->csimLocked == TRUE) ||(noChannelCsimLocked()))
       {
         KiAllocZeroMemory (sizeof(Char) * (VG_CRSM_MAX_COMMAND_STRING + NULL_TERMINATOR_LENGTH),
                              (void **) &commandDataString_p);
         KiAllocZeroMemory (sizeof(Int8) * (VG_CRSM_MAX_COMMAND_STRING + NULL_TERMINATOR_LENGTH),
                              (void **) &commandData_p);

         if (VG_SIM_READY == simLockGenericContext_p->simState)
         {
           /* SIM is ok, execute the command */
           resultCode = parseCommandLineForCrsmCrla (
                                         commandBuffer_p,
                                         &cmd,
                                         &fileId,
                                         &p1, &p2, &p3,
                                         commandDataString_p,
                                         VG_CRSM_MAX_COMMAND_STRING,
                                         &commandDataStringLength,
                                         /* job134856: add handling for <pathid> field */
                                         pathData,
                                         &pathLength);
           if ( RESULT_CODE_PROCEEDING == resultCode )
           {
              resultCode = buildCommandForCrsmCrla (
                            commandData_p,
                             &commandDataLen,
                              cmd, sessionId,
                               p1, p2, p3,
                                commandDataString_p,
                                 commandDataStringLength,
                                  entity);

              if (RESULT_CODE_PROCEEDING == resultCode)
              {
                setFileAndDirId (&fileId, &dirId);

                resultCode = vgSendApexSimGenAccessReq (entity,
                                                        commandDataLen,
                                                        commandData_p,
                                                        (Int16) fileId,
                                                        (Int16) dirId,
                                                        (Int16) SIM_DIR_INVALID,
                                                        /* job134856: add handling for <pathid> field */
                                                        pathData,
                                                        pathLength);
              }
           }
         }
         else
         {
           /* SIM is not ready */
           resultCode = vgGetSimCmeErrorCode ();
         }

         KiFreeMemory( (void**)&commandDataString_p);
         KiFreeMemory( (void**)&commandData_p);
       }
       else
       {
         resultCode = RESULT_CODE_ERROR;
       }
       break;
     }
     case EXTENDED_QUERY:    /* AT+CRSM?  */ /* fall through... */
     case EXTENDED_ACTION:   /* AT+CRSM   */ /* fall through... */
     default:
     {
       resultCode = RESULT_CODE_ERROR;
       break;
     }
   } /* end switch */

   return (resultCode);
}

/*--------------------------------------------------------------------------
*
* Function:     vgSlCMAR
*
* Parameters:   commandBuffer_p - pointer to command line string
*               entity          - mux channel number
*
* Returns:      Resultcode_t    - result of function
*
* Description:  Set command performs a 'master reset' of all user variables,
*               test command just returns OK.
*-------------------------------------------------------------------------*/
ResultCode_t vgSlCMAR(CommandLine_t *commandBuffer_p,
                      const VgmuxChannelNumber  entity)
{
  ResultCode_t            result = RESULT_CODE_OK;
  Char                    pin[SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH] = {0};
  Int16                   pinLen = 0;
  SimLockContext_t        *simLockContext_p = ptrToSimLockContext(entity);
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext();
  GeneralContext_t        *generalContext_p = ptrToGeneralContext (entity);
  VgSimInfo               *simInfo = &(simLockGenericContext_p->simInfo);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  switch( getExtendedOperation(commandBuffer_p) )
  {
    case EXTENDED_ASSIGN: /* AT+CMAR= */
    {
      /* Fetch pin code.... */
      if (getExtendedString(commandBuffer_p,
                            &pin[0],
                            SIM_CHV_LENGTH,
                            &pinLen) == TRUE)
      {
        /* Verify pin length is correct.... */
        if ((pinLen <= SIM_CHV_LENGTH) &&
            (pinLen != 0) &&
            (checkForAdditionalPassword (commandBuffer_p,
                                          pin,
                                           &pinLen) == TRUE))
        {
          /* Work out what to do based on current state.... */
          if (simLockGenericContext_p->simState == VG_SIM_PIN)
          {
            /* Need to send correct PIN to SIM before we can do anything.... */
            memcpy( &simLockGenericContext_p->pinCode[0],
                    &pin[0],
                    pinLen);
            memset( &simLockGenericContext_p->pinCode[pinLen],
                    UCHAR_MAX,
                    (SIM_CHV_LENGTH - pinLen));

            /* Initialise response params and send signal.... */
            simLockGenericContext_p->simBlocked = FALSE;
            simLockGenericContext_p->chvNum = SIM_CHV_1;
            result = vgChManContinueAction (entity, SIG_APEX_SIM_GET_CHV_RSP);
          }

          else if ((simLockGenericContext_p->simState == VG_SIM_READY) ||
                   (simLockGenericContext_p->simState == VG_SIM_NOT_READY))
          {
            /* Just need to verify pin entered is correct.... */
            memcpy( &generalContext_p->password[0],
                    &pin[0],
                    pinLen);
            memset( &generalContext_p->password[pinLen],
                    UCHAR_MAX,
                    (SIM_CHV_LENGTH - pinLen));

            generalContext_p->pinFunction = SIM_PIN_FUNCT_VERIFY;

            if (simInfo->cardIsUicc == TRUE)
            {
               generalContext_p->keyRef      = simInfo->pin1KeyRef;
               result = vgChManContinueAction (entity, SIG_APEX_SIM_PIN_FUNCTION_REQ);
            }
            else
            {
               generalContext_p->chvNumber   = SIM_CHV_1;
               result = vgChManContinueAction (entity, SIG_APEX_SIM_CHV_FUNCTION_REQ);
            }

           }

          else
          {
            /* Illegal state for this command.... */
            result = vgGetSimCmeErrorCode();
          }
        }

        else
        {
          /* Invalid length pin.... */
          result = VG_CME_INCORRECT_PASSWORD;
        }
      }
      else
      {
        /* Missing pin code parameter.... */
        result = VG_CME_INCORRECT_PASSWORD;
      }
      break;
    }

    case EXTENDED_RANGE:  /* AT+CMAR=? */
    {
      /* Spec defines this just to return 'OK'.... */
      break;
    }

    case EXTENDED_QUERY:  /* AT+CMAR? */
    case EXTENDED_ACTION: /* AT+CMAR */
    default:
    {
      result = RESULT_CODE_ERROR;
    }
    break;
  }

  return (result);
}


/*--------------------------------------------------------------------------
*
* Function:     vgSlMSST
*
* Parameters:   in:  commandBuffer_p - the AT command line string
*
* Returns:      ResultCode_t - status of extraction
*
* Description:  Executes the *MSST AT command which allows the user to
*               to read the SST/UST from the (U)SIM.
*
*               *MSST?   - read command
*               Reads SST/UST from (U)SIM
*
*               In the same manner the (U)SIM <response> shall be sent back
*               by the ME to the TA as it is. Format:
*
*               *MSST: <stt> [[,<sst_bitmap_allocated>,<sst_bitmap_activated>]
*                             [,<ust_bitmap_available>, <est_bitmap>]]
*               OK
*
*-------------------------------------------------------------------------*/

ResultCode_t vgSlMSST (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
  ResultCode_t            resultCode = RESULT_CODE_OK;

  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_RANGE:    /* AT*MSST=? */
    {
      /* just output ok */
      resultCode = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_QUERY:   /* AT*MSST?  */
    {
        simLockGenericContext_p->simReadParamType = SIM_READ_PARAM_SST;
        resultCode = vgChManContinueAction (entity, APEX_SIM_READ_SIM_PARAM_REQ);
        break;
    }
    case EXTENDED_ASSIGN:    /* AT*MSST= xxx  */
    case EXTENDED_ACTION:   /* AT*MSST   */
    default:
    {
      resultCode = RESULT_CODE_ERROR;
      break;
    }
  } /* end case */

  return (resultCode);
}

#if defined(FEA_TEMP_INCLUDE_SIM_EMULATION) || defined (FEA_TEMP_ESIM_EMULATION)
/*--------------------------------------------------------------------------
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     Non-standard AT command to customize function,
 *                  CUSTOM_COMMAND.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSlMEMSIM(CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t            result              = RESULT_CODE_OK;
    ExtendedOperation_t     operation           = getExtendedOperation (commandBuffer_p);
    SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext();
    ChManagerContext_t      *chManagerContext_p = ptrToChManagerContext ();
    Int32                   custSimEmulate;

    switch (operation)
    {
        case EXTENDED_ACTION: /* AT*MEMUSIM */
        {
            if( simLockGenericContext_p->simEmulate != TRUE)
            {
                simLockGenericContext_p->simEmulate = TRUE;

                /* Send the request to write the new configuration*/
                chManagerContext_p->isImmediate = TRUE;
                result = vgChManContinueAction( entity,
                                                SIG_APEX_SIM_EMUSIM_REQ);
            }
        }
        break;

        case EXTENDED_QUERY:  /* AT*MEMUSIM? */
        {
             vgPutNewLine(entity);

             /* Print out the *MEMUSIM.... */
             vgPrintf( entity,
                  (Char *)"*MEMUSIM: %d", simLockGenericContext_p->simEmulate);

             vgPutNewLine(entity);
             vgFlushBuffer(entity);
        }
        break;

        case EXTENDED_ASSIGN:   /* AT*MEMUSIM=  */
        {
          /* Get functionality parameter (mandatory) */
          if (getExtendedParameter (commandBuffer_p,
                                     &custSimEmulate,
                                      ULONG_MAX) == TRUE)
          {
            if ((custSimEmulate == VG_AT_EMULATE_SIM_OFF) ||
                (custSimEmulate == VG_AT_EMULATE_SIM_ON))
            {
              if(simLockGenericContext_p->powerUpSim == TRUE)
              {
                 /* Return error if SIM has been powered on. */
                 return VG_CME_OPERATION_NOT_ALLOWED;
              }
              else
              {
                 simLockGenericContext_p->simEmulate = (Boolean)custSimEmulate;

                 /* Send the request to write the new configuration*/
                 chManagerContext_p->isImmediate = TRUE;
                 result = vgChManContinueAction( entity,
                                                 SIG_APEX_SIM_EMUSIM_REQ);
              }
            }
            else
            {
              result = VG_CME_INVALID_INPUT_VALUE;
            }
          }
          else
          {
            result = VG_CME_INVALID_INPUT_VALUE;
          }
        }
        break;

        default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;
    }

    return (result);
}
#endif

/*--------------------------------------------------------------------------
 * Function:     vgSlCsimLock
 *
 * Parameters:   in:  commandBuffer_p - the AT command line string
 *
 * Returns:      ResultCode_t - status of extraction
 *
 * Description:  Non standard AT command to Lock or unlock the USIM Manager
 *               for external AT+CSIM operation.
 *-------------------------------------------------------------------------*/

ResultCode_t vgSlCsimLock (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber  entity)
{
    ResultCode_t            result                    = RESULT_CODE_OK;
    ExtendedOperation_t     operation                 = getExtendedOperation (commandBuffer_p);
    SimLockContext_t       *simLockContext_p          = ptrToSimLockContext(entity);
    Int32                   csimLock;
    Boolean                 otherChannelLock = FALSE;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
    switch (operation)
    {
        case EXTENDED_RANGE:  /* AT*MCSIMLOCK=? */
            vgPutNewLine(entity);
            vgPrintf(entity, (const Char*)"*MCSIMLOCK: (0-1)");
            vgPutNewLine(entity);
            break;

        case EXTENDED_QUERY:  /* AT*MCSIMLOCK?  */
            vgPutNewLine(entity);
            /* if this is the channel with the lock otherChannelLock must be false - set above */
            if ( simLockContext_p->csimLocked == FALSE)
            {
                /* this channel isn't locked so see if any others are */
                otherChannelLock = !noChannelCsimLocked();
            }
            vgPrintf( entity,
                  (Char *)"*MCSIMLOCK: %d, %d ", simLockContext_p->csimLocked, otherChannelLock);
            vgPutNewLine( entity);
            break;

        case EXTENDED_ASSIGN:/* AT*MCSIMLOCK= */
            if (getExtendedParameter (commandBuffer_p,
                                      &csimLock,
                                      ULONG_MAX) == TRUE)
            {
                if (csimLock <= 1)
                {
                    if (simLockContext_p->csimLocked == csimLock)
                    /* we are already in this state */
                    {
                        result = RESULT_CODE_OK;
                    }
                    else if (((csimLock == TRUE) && (noChannelCsimLocked() == TRUE))
                    /* we want to lock and there isn't a lock on any channel */
                     || ((csimLock == FALSE) && (simLockContext_p->csimLocked == TRUE)))
                    /* or we want to unlock and we are locked on this channel */
                    {
                        result = vgChManContinueAction (entity, SIG_APEX_SIM_CSIM_LOCK_REQ);
                    }
                    else
                    {
                        result = RESULT_CODE_ERROR;
                    }
                }
                else
                {
                    result = RESULT_CODE_ERROR;
                }
            }
            break;

        case EXTENDED_ACTION: /* AT*MCSIMLOCK   */
        default:
            result = RESULT_CODE_ERROR;
            break;
    }
    return (result);
}

/*--------------------------------------------------------------------------
*
* Function:     vgSlMGID
*
* Parameters:   in:  commandBuffer_p - the AT command line string
*
* Returns:      ResultCode_t - status of extraction
*
* Description:  Executes the *MGID AT command which allows the user to
*               to read the GID1 and GID2 from the (U)SIM.
*
*               *MGID?   - read command
*               Reads GID1 and GID2 from (U)SIM
*
*               In the same manner the (U)SIM <response> shall be sent back
*               by the ME to the TA as it is. Format:
*
*               *MGID: <length1>,<GID1>,<length2>,<GID2>
*               OK
*
*-------------------------------------------------------------------------*/

ResultCode_t vgSlMGID (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
  ResultCode_t            resultCode = RESULT_CODE_OK;

  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_RANGE:    /* AT*MGID=? */
    {
      /* just output ok */
      resultCode = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_QUERY:   /* AT*MGID?  */
    {
        simLockGenericContext_p->simReadParamType = SIM_READ_PARAM_GID;
        resultCode = vgChManContinueAction (entity, APEX_SIM_READ_SIM_PARAM_REQ);
        break;
    }
    case EXTENDED_ASSIGN:    /* AT*MGID= xxx  */
    case EXTENDED_ACTION:   /* AT*MGID   */
    default:
    {
      resultCode = RESULT_CODE_ERROR;
      break;
    }
  } /* end case */

  return (resultCode);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnCNUM
*
* Parameters:  commandBuffer_p    - pointer to command line string
*              entity             - mux channel number
*
* Returns:     Resultcode_t       - result of function
*
* Description: executes the AT+CNUM command which displays the subscriber
*              number.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnCNUM (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
  MobilityContext_t   *mobilityContext_p = ptrToMobilityContext ();

  switch (operation)
  {
    case EXTENDED_RANGE:  /* +CNUM=? */
    {
      break;
    }
    case EXTENDED_ACTION:   /* AT+CNUM  */
    {
      /* Msisdn record number starts from 1 */
      mobilityContext_p->cnumDatarecordNumber = 1;
      result = vgChManContinueAction(entity, SIG_APEX_SIM_READ_MSISDN_REQ);            
      break;
    }
    case EXTENDED_ASSIGN:   /* +CNUM=<n>  (command not supported) */
    case EXTENDED_QUERY:    /* +CNUM?     (command not supported) */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnCCHO
*
* Parameters:  commandBuffer_p    - pointer to command line string
*              entity             - mux channel number
*
* Returns:     Resultcode_t       - result of function
*
* Description: executes the AT+CCHO command for SIM logical channel access
----------------------------------------------------------------------------*/
ResultCode_t vgGnCCHO (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
  Int16               strLength = 0;
  Int16               dfnameLen = 0;
  Char                dfnameStr[SIM_LOG_CHAN_MAX_DFNAME_STR + NULL_TERMINATOR_LENGTH];
  Int8                dfname[SIM_LOG_CHAN_MAX_DFNAME_LEN];
  SimLockContext_t   *simLockContext_p = ptrToSimLockContext(entity);

  switch (operation)
  {
    case EXTENDED_RANGE:  /* +CCHO=? */
    {
      break;
    }
    case EXTENDED_ASSIGN:   /* +CCHO=<dfname> */
    {
      if (getExtendedString ( commandBuffer_p,
         dfnameStr,
         SIM_LOG_CHAN_MAX_DFNAME_STR,
         &strLength) == TRUE)
      {
         /* string length can be 2, or multiple of 2 */
         if (!((strLength >= SIM_LOG_CHAN_MIN_DFNAME_STR) &&
                (strLength == (((strLength >> 1) << 1))) &&
                (strLength <= SIM_LOG_CHAN_MAX_DFNAME_STR)))
         {
             result = VG_CME_INVALID_INPUT_VALUE;
         }
         else
         {
            /* Decode the hex string - convert to hex number */
            dfnameLen = vgMapTEToHex (dfname,
                                  SIM_LOG_CHAN_MAX_DFNAME_LEN,
                                  dfnameStr,
                                  strLength,
                                  VG_AT_CSCS_HEX);

            simLockContext_p->vgUiccLogicChannelData.dfNameLength = dfnameLen;

            memcpy(simLockContext_p->vgUiccLogicChannelData.dfName, dfname, dfnameLen);
            
            result = vgChManContinueAction (entity, APEX_SIM_OPEN_LOGICAL_CHANNEL_REQ);
          
         }
      }
      else
      {
         result = VG_CME_INVALID_INPUT_VALUE;
      }
      
      break;
    }
    case EXTENDED_ACTION:   /* AT+CCHO  */
    case EXTENDED_QUERY:    /* +CCHO?   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnCCHC
*
* Parameters:  commandBuffer_p    - pointer to command line string
*              entity             - mux channel number
*
* Returns:     Resultcode_t       - result of function
*
* Description: executes the AT+CCHC command for SIM logical channel access
----------------------------------------------------------------------------*/
ResultCode_t vgGnCCHC (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
  Int32               sessionId;
  SimLockContext_t   *simLockContext_p = ptrToSimLockContext(entity); 
  switch (operation)
  {
    case EXTENDED_RANGE:  /* +CCHC=? */
    {
      break;
    }
    case EXTENDED_ASSIGN:   /* +CCHC=<sessionid> */
    {
      /* Get <sessionid> */
      if(getExtendedParameter ( commandBuffer_p,
                                  &sessionId,
                                  ULONG_MAX) == TRUE)
      {
          if (sessionId == ULONG_MAX)
          {
              result = VG_CME_INVALID_INPUT_VALUE;
          }
      }
      else
      {
          result = VG_CME_INVALID_INPUT_VALUE;
      }
     
      if (result == RESULT_CODE_OK)
      {
          simLockContext_p->vgUiccLogicChannelData.sessionId = sessionId;
          
          result = vgChManContinueAction (entity, APEX_SIM_CLOSE_LOGICAL_CHANNEL_REQ);
      }
      break;
    }
    case EXTENDED_ACTION:   /* AT+CCHC  */
    case EXTENDED_QUERY:    /* +CCHC?   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}


/*--------------------------------------------------------------------------
*
* Function:    vgGnCGLA
*
* Parameters:  commandBuffer_p    - pointer to command line string
*              entity             - mux channel number
*
* Returns:     Resultcode_t       - result of function
*
* Description: executes the AT+CGLA command for SIM logical channel access
----------------------------------------------------------------------------*/
ResultCode_t vgGnCGLA (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
  SimLockContext_t    *simLockContext_p = ptrToSimLockContext(entity);
  Int32               sessionId;
  Int32               length;
  Int16               commandStringLen;
  Int16               commandDataLen;
  Char                commandString_p[SIM_LOG_CHAN_MAX_CMD_STR + NULL_TERMINATOR_LENGTH];
  Int8                commandData_p[SIM_LOG_CHAN_MAX_CMD_LEN];

  switch (operation)
  {
    case EXTENDED_RANGE:  /* +CGLA=? */
    {
      break;
    }
    case EXTENDED_ASSIGN:   /* +CGLA=<sessionid>,<length>,<command> */
    {

#if defined (ATCI_SLIM_DISABLE)
      FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif

      /* Get <sessionid> */
      if(getExtendedParameter ( commandBuffer_p,
                                  &sessionId,
                                  ULONG_MAX) == TRUE)
      {
          if (sessionId == ULONG_MAX)
          {
              result = VG_CME_INVALID_INPUT_VALUE;
          }
      }
      else
      {
          result = VG_CME_INVALID_INPUT_VALUE;
      }
     
      if (result == RESULT_CODE_OK)
      {
        /* Get <length> */
        if(getExtendedParameter ( commandBuffer_p,
                                    &length,
                                    ULONG_MAX) == TRUE)
        {
            if (length == ULONG_MAX)
            {
                result = VG_CME_INVALID_INPUT_VALUE;
            }
            else if (!((length > 0) &&
                  (length == (((length >> 1) << 1))) &&
                  (length <= SIM_LOG_CHAN_MAX_CMD_STR)))
            {
               result = VG_CME_INVALID_INPUT_VALUE;
            }           
        }
        else
        {
            result = VG_CME_INVALID_INPUT_VALUE;
        }
      }

      if (result == RESULT_CODE_OK)
      {
        /* Get <command> */
        if (getExtendedString (commandBuffer_p,
                               (Char*)commandString_p,
                                SIM_LOG_CHAN_MAX_CMD_STR,
                                 &commandStringLen) == TRUE)
        {
          if (!((commandStringLen > 0) &&
                (commandStringLen == (((commandStringLen >> 1) << 1))) &&
                (commandStringLen <= SIM_LOG_CHAN_MAX_CMD_STR)))
          {
             result = VG_CME_INVALID_INPUT_VALUE;
          }
          else if (length != (Int32)commandStringLen)
          {
            /* length must be twice the input number of octets size (i.e. actual
             * length of string entered
             */
            result = VG_CME_INVALID_INPUT_VALUE;
          }

          if (result == RESULT_CODE_OK)
          {
            /* Convert Char string into an array of Int8's */
            memset (commandData_p, 0, SIM_LOG_CHAN_MAX_CMD_LEN * sizeof (Int8));

            commandDataLen = vgMapTEToGsm (commandData_p,
                           SIM_LOG_CHAN_MAX_CMD_LEN,
                            commandString_p,
                             commandStringLen,
                              VG_AT_CSCS_HEX,
                               entity);

            result = vgSendApexSimLogicalChannelAccessReq (entity,
                                                           commandDataLen,
                                                           (Int8)sessionId,
                                                           commandData_p,
                                                           (Int16)SIM_EF_INVALID,
                                                           (Int16)SIM_DIR_INVALID,
                                                           (Int16)SIM_DIR_INVALID,
                                                           /* job134856: add handling for <pathid> field */
                                                           /* field not used for +CSIM command */
                                                           PNULL,
                                                           0);
          }
        }
        else /* error reading supplied parameter */
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }
      break;
    }
    case EXTENDED_ACTION:   /* AT+CGLA  */
    case EXTENDED_QUERY:    /* +CGLA?   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnCRLA
*
* Parameters:  commandBuffer_p    - pointer to command line string
*              entity             - mux channel number
*
* Returns:     Resultcode_t       - result of function
*
* Description: executes the AT+CRLA command for SIM logical channel access
----------------------------------------------------------------------------*/
ResultCode_t vgGnCRLA (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
  SimLockContext_t    *simLockContext_p = ptrToSimLockContext(entity);
  Int32               sessionId;
  Int16               commandStringLen;
  Int16               commandDataLen;
  Char                commandString_p[SIM_LOG_CHAN_MAX_CMD_STR + NULL_TERMINATOR_LENGTH];
  Int8                commandData_p[SIM_LOG_CHAN_MAX_CMD_LEN];

  Int8                p1;
  Int8                p2;
  Int8                p3;
  Int8                cmd = 0;
  SimEfId             fileId;
  SimDirId            dirId;
  Int8                pathData[MAX_SELECTION_PATH_LEN] = {0};
  Int8                pathLength = 0;

  switch (operation)
  {
    case EXTENDED_RANGE:  /* +CRLA=? */
    {
      break;
    }
    case EXTENDED_ASSIGN:  /* +CRLA= */
    {
#if defined (ATCI_SLIM_DISABLE)

      FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
      /* Get <sessionid> */
      if(getExtendedParameter ( commandBuffer_p,
                                  &sessionId,
                                  ULONG_MAX) == TRUE)
      {
          if ((sessionId == ULONG_MAX) ||
              (sessionId >= SIM_LOG_CHAN_MAX_SESSION_NUM))
          {
              result = VG_CME_INVALID_INPUT_VALUE;
          }
      }
      else
      {
          result = VG_CME_INVALID_INPUT_VALUE;
      }


      result = parseCommandLineForCrsmCrla (
                                  commandBuffer_p,
                                  &cmd,
                                  &fileId,
                                  &p1, &p2, &p3,
                                  commandString_p,
                                  VG_CRSM_MAX_COMMAND_STRING,
                                  &commandStringLen,
                                  /* job134856: add handling for <pathid> field */
                                  pathData,
                                  &pathLength);

      if ( RESULT_CODE_PROCEEDING == result )
      {
         result = buildCommandForCrsmCrla (
                       commandData_p,
                        &commandDataLen,
                         cmd, (Int8)sessionId,
                          p1, p2, p3,
                           commandString_p,
                            commandStringLen,
                             entity);

         if (RESULT_CODE_PROCEEDING == result)
         {
           setFileAndDirId (&fileId, &dirId);

           result = vgSendApexSimLogicalChannelAccessReq (entity,
                                                           commandDataLen,
                                                           (Int8)  sessionId,
                                                           commandData_p,
                                                           (Int16) fileId,
                                                           (Int16) dirId,
                                                           (Int16) SIM_DIR_INVALID,
                                                           pathData,
                                                           pathLength);
         }
      }
      break;
    }
    case EXTENDED_ACTION:   /* AT+CGLA  */
    case EXTENDED_QUERY:    /* +CGLA?   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:     viewCFUN
*
* Parameters:   entity  - mux channel number
*
* Returns:      nothing
*
* Description:  Display current system configuration info.
*
*-------------------------------------------------------------------------*/

void viewCFUN (const VgmuxChannelNumber entity)
{
  /* display +CFUN status */

  vgPutNewLine (entity);

  vgPrintf     (entity,
                (const Char*)"+CFUN: %d",
                  vgGetCFUNMode());

  vgPutNewLine (entity);
}

/*--------------------------------------------------------------------------
*
* Function:     vgSlMasterReset
*
* Scope:        Global
*
* Parameters:   entity
*
* Returns:      ResultCode_t - result of operation
*
* Description:  Performs the necessary AT commands to initiate a master
*               reset.
*-------------------------------------------------------------------------*/
ResultCode_t vgSlMasterReset(const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ScanParseContext_t  *scanParseContext_p = ptrToScanParseContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(scanParseContext_p != PNULL, entity, 0, 0);
#endif
  if (vgStrLen(cmarResetProcedure) > 0)
  {

#if defined(ATCI_ENABLE_DYN_AT_BUFF)
    /* Allocate buffer if not already done so */
    if (scanParseContext_p->nextCommand.character == PNULL)
    {
      vgAllocAtCmdBuffer(entity);
    }
#endif         
    
    /* We concatenate any commands we want to run onto the end of this
     * one such that when the command completes we execute the next....
     */
    vgStrNCpy(&scanParseContext_p->nextCommand.character[scanParseContext_p->nextCommand.position],
             cmarResetProcedure,
             (COMMAND_LINE_SIZE + NULL_TERMINATOR_LENGTH) - scanParseContext_p->nextCommand.position);
    scanParseContext_p->nextCommand.length += (Int16)(vgStrLen(cmarResetProcedure) + NULL_TERMINATOR_LENGTH);
  }

  return (result);
}

#if defined (ENABLE_DUAL_SIM_SOLUTION)
/*--------------------------------------------------------------------------
 *
 * Function:       vgSlMSIMHSEL
 * Parameters:     commandBuffer_p - pointer to command line string
 *                 entity          - mux channel number

 * Returns:        AT result code.

 *
 * Description: This function handles the non-standard AT command to select a
 * different SIM holder
 *-------------------------------------------------------------------------*/

ResultCode_t vgSlMSIMHSEL(CommandLine_t *commandBuffer_p,
                      const VgmuxChannelNumber  entity)
{
    ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
    ResultCode_t         result                      = RESULT_CODE_OK;
    SimLockContext_t     *simLockContext_p           = ptrToSimLockContext (entity);
    SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext();
    VgSimInfo            *simInfo                    = &simLockGenericContext_p->simInfo;
    Int32                newSimHolder;

    switch (operation)
    {
        case EXTENDED_RANGE:  /* AT*MSIMHSEL=? */
            {
                vgPutNewLine(entity);
                vgPrintf(entity, (const Char*)"%C: (0-1)");
                vgPutNewLine(entity);
            }
            break;

        case EXTENDED_ASSIGN: /* AT*MSIMHSEL=... */
            {
                if ( getExtendedParameter (commandBuffer_p, &newSimHolder, ULONG_MAX) != TRUE )
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else if (newSimHolder < (Int32)(simInfo->numSimHolders))
                {
                  if ( newSimHolder == simInfo->currentSimHolder )
                  /* we already have this sim holder selected */
                  {
                     result = RESULT_CODE_OK;
                  }
                  else
                  {
                    simLockContext_p->simHolderToSelect = (SimHolderType)newSimHolder;
                    /* we need to power down and select this SIM holder */
                    result = vgChManContinueAction (entity, SIG_APEX_SIM_SELECT_REQ);
                  }
                }
                else
                {
                   result = VG_CME_INVALID_INPUT_VALUE;
                }
            }
            break;

        case EXTENDED_QUERY:  /* AT*MSIMHSEL? */
            {
                vgPutNewLine(entity);
                vgPrintf(entity, (Char *)"*MSIMHSEL: %d,%d",
                        (Int16)simInfo->currentSimHolder, (Int16)simInfo->numSimHolders);
                vgPutNewLine(entity);
            }
            break;

        case EXTENDED_ACTION: /* AT*MSIMHSEL... */
        default:
            {
                result = RESULT_CODE_ERROR;
            }
            break;
    }



   return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:       vgSlCSUS
 * Parameters:     commandBuffer_p - pointer to command line string
 *                 entity          - mux channel number

 * Returns:        AT result code.

 *
 * Description: This function handles the standard AT command to select a
 * different SIM holder
 *-------------------------------------------------------------------------*/

ResultCode_t vgSlCSUS (CommandLine_t *commandBuffer_p,
                      const VgmuxChannelNumber  entity)
{
    ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
    ResultCode_t         result                      = RESULT_CODE_OK;
    SimLockContext_t     *simLockContext_p           = ptrToSimLockContext (entity);
    SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext();
    VgSimInfo            *simInfo                    = &simLockGenericContext_p->simInfo;
    Int32                newSimHolder;

    switch (operation)
    {
        case EXTENDED_RANGE:  /* AT*CSUS=? */
            {
                vgPutNewLine(entity);
                vgPrintf(entity, (const Char*)"%C: (0-1)");
                vgPutNewLine(entity);
            }
            break;

        case EXTENDED_ASSIGN: /* AT*CSUS=... */
            {
                if ( getExtendedParameter (commandBuffer_p, &newSimHolder, ULONG_MAX) != TRUE )
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else if (newSimHolder < (Int32)(simInfo->numSimHolders))
                {
                  if ( newSimHolder == simInfo->currentSimHolder )
                  /* we already have this sim holder selected */
                  {
                     result = RESULT_CODE_OK;
                  }
                  else
                  {
                    simLockContext_p->simHolderToSelect = (SimHolderType)newSimHolder;
                    /* we need to power down and select this SIM holder */
                    result = vgChManContinueAction (entity, SIG_APEX_SIM_SELECT_REQ);
                  }
                }
                else
                {
                   result = VG_CME_INVALID_INPUT_VALUE;
                }
            }
            break;

        case EXTENDED_QUERY:  /* AT*CSUS? */
            {
                vgPutNewLine(entity);
                vgPrintf(entity, (Char *)"*CSUS: %d",
                        (Int16)simInfo->currentSimHolder);
                vgPutNewLine(entity);
            }
            break;

        case EXTENDED_ACTION: /* AT*CSUS... */
        default:
            {
                result = RESULT_CODE_ERROR;
            }
            break;
    }



   return (result);
}
#endif /* ENABLE_DUAL_SIM_SOLUTION */

#if defined (SIM_EMULATION_ON)
/*--------------------------------------------------------------------------
 *
 * Function:       vgSlMUSIMEMUW
 * Parameters:     commandBuffer_p - pointer to command line string
 *                 entity          - mux channel number

 * Returns:        AT result code.

 *
 * Description: This function handles the MTK Proprietary AT command to
 *              write data in to specific files in the USIM emulator.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSlMUSIMEMUW (CommandLine_t *commandBuffer_p,
                      const VgmuxChannelNumber  entity)
{
    ExtendedOperation_t     operation                           = getExtendedOperation (commandBuffer_p);
    ResultCode_t            result                              = RESULT_CODE_OK;
    SimLockGenericContext_t *simLockGenericContext_p            = ptrToSimLockGenericContext();
    Char                    fileIdString[VG_MUSIMEMUW_FILE_ID_STR_LEN];
    Int16                   fileIdStringLen                     = 0;
    Int8                    fileIdBytes[VG_MUSIMEMUW_FILE_ID_LEN];
    Int32                   dataOffset                          = 0;
    Char                    *fileDataString_p                   = PNULL;
    Int16                   fileDataStringLen                   = 0;

    switch (operation)
    {
        case EXTENDED_QUERY:       /* AT*MUSIMEMUW? */
        case EXTENDED_RANGE:       /* AT*MUSIMEMUW=? */
        case EXTENDED_ACTION:      /* AT*MUSIMEMUW... */
        {
            result = RESULT_CODE_ERROR;
            break;
        }

        case EXTENDED_ASSIGN:      /* AT*MUSIMEMUW=... */
        {
            /* Allocate memory forthe file data string - and then free it later
             */
            KiAllocZeroMemory(   sizeof(Char)*VG_MUSIMEMUW_FILE_DATA_STR_LEN,
                                 (void **) &fileDataString_p);

            /* Get the File ID */
            if (getExtendedString (commandBuffer_p,
                                   fileIdString,
                                   VG_MUSIMEMUW_FILE_ID_LEN * 2,
                                   &fileIdStringLen) == TRUE)
            {
                if (fileIdStringLen == VG_MUSIMEMUW_FILE_ID_LEN * 2)
                {

                    /* length must be exactly twice the file ID length in bytes.
                     * Now convert it to an array of bytes
                     */
                    vgMapTEToGsm (fileIdBytes,
                                   VG_MUSIMEMUW_FILE_ID_LEN,
                                    fileIdString,
                                     fileIdStringLen,
                                      VG_AT_CSCS_HEX,
                                       entity);

                    /* Finally convert to Int16 value and put into structure
                     */
                    simLockGenericContext_p->alsiWriteUsimEmuFileReq.fileId =((((Int16)(fileIdBytes[0])) << 8) + (Int16)(fileIdBytes[1]));
                }
                else
                {
                    /* Wrong number of digits - so error */
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
            }
            else
            {
                result = VG_CME_INVALID_INPUT_VALUE;
            }

            if (result == RESULT_CODE_OK)
            {
                /* Now get the data offset */
                if (getExtendedParameter (commandBuffer_p, &dataOffset, ULONG_MAX) == TRUE)
                {
                    /* check dataOffset validity */
                    if (dataOffset == ULONG_MAX)
                    {
                        dataOffset = 0;
                    }
                    else if (dataOffset <= VG_MUSIMEMUW_FILE_DATA_LEN - 1)
                    {
                        simLockGenericContext_p->alsiWriteUsimEmuFileReq.dataOffset = (Int16)dataOffset;
                    }
                    else
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
                    }
                }
                else
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
            }

            if (result == RESULT_CODE_OK)
            {
                /* Now get the data */
                if (getExtendedString (commandBuffer_p,
                                       fileDataString_p,
                                       VG_MUSIMEMUW_FILE_DATA_LEN * 2,
                                       &fileDataStringLen) == TRUE)
                {
                    /* Check the length of the data makes sense */
                    if ((fileDataStringLen <= VG_MUSIMEMUW_FILE_DATA_LEN * 2) &&
                        (fileDataStringLen >= 2) &&
                        (fileDataStringLen/2 + dataOffset <= VG_MUSIMEMUW_FILE_DATA_LEN) &&
                        ((fileDataStringLen/2)*2 == fileDataStringLen))
                    {

                        /* Now convert to an array of bytes
                         */
                        vgMapTEToGsm (simLockGenericContext_p->alsiWriteUsimEmuFileReq.data,
                                       VG_MUSIMEMUW_FILE_DATA_LEN,
                                        fileDataString_p,
                                         fileDataStringLen,
                                          VG_AT_CSCS_HEX,
                                           entity);
                    }
                    else
                    {
                        /* Wrong number of digits - so error */
                        result = VG_CME_INVALID_INPUT_VALUE;
                    }
                }
                else
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
            }

            if (result == RESULT_CODE_OK)
            {
                /* Complete the remaining data fields prior to sending the
                 * message to the USIM manager
                 */
                simLockGenericContext_p->alsiWriteUsimEmuFileReq.dataLength = fileDataStringLen/2;
                simLockGenericContext_p->alsiWriteUsimEmuFileReq.taskId     = VG_CI_TASK_ID;

                /* Now send the message */
                result = vgChManContinueAction (entity, SIG_ALSI_WRITE_USIM_EMU_FILE_REQ);
            }

            KiFreeMemory( (void**)&fileDataString_p);

            break;
        }

        default:
        {
            result = RESULT_CODE_ERROR;
            break;
        }
        }
    return (result);
}
#endif /* SIM_EMULATION_ON */

/*--------------------------------------------------------------------------
*
* Function:    vgSlCPINR
*
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Used to display the current avaliable PIN(s) status information used
*              by the SIM for the verification of access condition level 1,
*              level 2 and alternative PIN.
*
*-------------------------------------------------------------------------*/
ResultCode_t vgSlCPINR   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber  entity)
{
  ResultCode_t             result = RESULT_CODE_OK;
  ExtendedOperation_t      operation = getExtendedOperation (commandBuffer_p);
  cpinrSimPinStatusType    code;
  Char                     *str_par = PNULL;
  Int16                    stringLength = 0;
  Boolean                  firstTime = TRUE;

#if defined (ENABLE_LONG_AT_CMD_RSP)
  KiAllocZeroMemory(    sizeof(Char) * (AT_MEDIUM_LARGE_BUFF_SIZE),
                        (void **)&str_par);
#else
  KiAllocZeroMemory(    sizeof(Char) * (COMMAND_LINE_SIZE),
                        (void **)&str_par);
#endif
  switch (operation)
  {
    case EXTENDED_RANGE:  /* *CPINR=? */
    {
      /* Do nothing */
      break;
    }

    case EXTENDED_ACTION:   /* AT*CPINR  */
    {
      /* Display all the CPINR values  */
      showAllCpinrValues (entity);
      break;
    }

    case EXTENDED_ASSIGN: /* *CPINR= */
    {
      if ((getExtendedString(commandBuffer_p,
                             str_par,
#if defined (ENABLE_LONG_AT_CMD_RSP)
                             AT_MEDIUM_LARGE_BUFF_SIZE,
#else
                             COMMAND_LINE_SIZE,
#endif                             
                             &stringLength) == TRUE))
      {
        if (stringLength != 0)
        {
          /* Get the string and see if it matches any of the strings in
           * cpinrSimPinStatusMessage array.  The str_par can contain
           * '*' and '?' wildcards.
           */
          for (code = VG_CPINR_SIM_PIN; code < NUM_CPINR_PIN_STATUS_VALUES; code++)
          {
#if defined (ENABLE_LONG_AT_CMD_RSP)
            if (vgStrWildNCmp (str_par, cpinrSimPinStatusMessage[code], AT_MEDIUM_LARGE_BUFF_SIZE-1) == 0)
#else
            if (vgStrWildNCmp (str_par, cpinrSimPinStatusMessage[code], COMMAND_LINE_SIZE-1) == 0)
#endif              
            {
              if (firstTime)
              {
                vgPutNewLine (entity);
                firstTime = FALSE;
              }
              /* We have a string match - so display it */
              showCpinrValue(entity, code);
            }
          }
          if (firstTime)
          {
            /* We didn't match a string so display CME ERROR */
            result = VG_CME_INVALID_INPUT_VALUE;
          }
        }
        else
        {
          /* Empty string */
          result = VG_CME_INVALID_INPUT_VALUE;
        }

      }
      else
      {
        /* Invalid string */
        result = VG_CME_INVALID_INPUT_VALUE;
      }
      break;
    }

    case EXTENDED_QUERY:    /* AT*CPINR?  */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  KiFreeMemory( (void **)&str_par);

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:       vgSlCardmode
 *
 * Parameters:     commandBuffer_p - pointer to command line string
 *                 entity          - mux channel number
 *
 * Returns:        AT result code.
 *
 *
 * Description: This function handles the non-standard AT command ^CARDMODE
 *              that will print the current SIM type (SIM/USIM)
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSlCardmode(  CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber  entity)
{
    ExtendedOperation_t     operation                   = getExtendedOperation (commandBuffer_p);
    ResultCode_t            result                      = RESULT_CODE_OK;
    SimLockGenericContext_t *simLockGenericContext_p    = ptrToSimLockGenericContext();
    VgSimInfo               *simInfo                    = &simLockGenericContext_p->simInfo;

    switch (operation)
    {
        case EXTENDED_RANGE:  /* AT^CARDMODE=? */
        {
            result = RESULT_CODE_ERROR;
        }
        break;

        case EXTENDED_ASSIGN: /* AT^CARDMODE=... */
        {
            result = RESULT_CODE_ERROR;
        }
        break;

        case EXTENDED_QUERY:  /* AT^CARDMODE? */
        {
            result = RESULT_CODE_ERROR;
        }
        break;

        case EXTENDED_ACTION: /* AT^CARDMODE... */
        {
            if( simLockGenericContext_p->simState == VG_SIM_NOT_READY)
            {
                result = VG_CME_SIM_NOT_INSERTED;
            }
            else
            {
                vgPutNewLine(entity);
                vgPrintf(entity, (Char *)"%C: %d",
                    (simInfo->cardIsUicc == TRUE ? VG_SL_CARDMODE_USIM : VG_SL_CARDMODE_SIM) );
                vgPutNewLine(entity);
            }
        }
        break;

        default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;
    }

   return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSlMuapp
*
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description:
*
*-------------------------------------------------------------------------*/
ResultCode_t vgSlMuapp( CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
    ResultCode_t                result                      = RESULT_CODE_OK;
    ExtendedOperation_t         operation                   = getExtendedOperation (commandBuffer_p);
    SimLockContext_t           *simLockContext_p            = ptrToSimLockContext(entity);
    SimLockGenericContext_t    *simLockGenericContext_p     = ptrToSimLockGenericContext();
    Int32                       mode                        = 0;
    Int32                       index                       = 0;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(simLockContext_p != PNULL, entity, 0, 0);

#endif
    switch (operation)
    {
        case EXTENDED_RANGE: /* AT*MUAPP=? */
        {
            /*  To know how many sim applications are present,
            *   we need to read the firsts records of the DIR file*/
            simLockContext_p->dirReqStartRecord     = 1;
            simLockContext_p->dirReqNumRecord       = 0;
            simLockContext_p->vgMuappData.muappMode = VG_MUAPP_RANGE;
            result = vgChManContinueAction (entity, SIG_APEX_SIM_READ_DIR_REQ);
        }
        break;

        case EXTENDED_QUERY: /* AT*MUAPP?  */
        {
            /* Read the first DIR records*/
            simLockContext_p->dirReqStartRecord     = 1;
            simLockContext_p->dirReqNumRecord       = 0;
            simLockContext_p->vgMuappData.muappMode = VG_MUAPP_QUERY;
            result = vgChManContinueAction (entity, SIG_APEX_SIM_READ_DIR_REQ);
        }
        break;

        case EXTENDED_ASSIGN: /* AT*MUAPP=...*/
        {
            /* Read the mode parameter*/
            if( (   (getExtendedParameter (commandBuffer_p, &mode, ULONG_MAX) != TRUE) ||
                    (mode >= VG_MUAPP_INVALID_MODE) ) )
            {
                result = VG_CME_INVALID_INPUT_VALUE;
            }
            else
            {
                simLockContext_p->vgMuappData.muappMode = (VgMuappMode)mode;
            }

            /* If need, read the index parameter*/
            if( (   (result == RESULT_CODE_OK) &&
                    (simLockContext_p->vgMuappData.muappMode == VG_MUAPP_ACTIVE_AID_SESSION) ) )
            {
                if( (getExtendedParameter (commandBuffer_p, &index, ULONG_MAX) != TRUE) ||
                    (index > VG_MAX_UINT8) ||
                    (index == 0))
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
            }

            /* Execute the command operation*/
            if( result == RESULT_CODE_OK)
            {
                switch( simLockContext_p->vgMuappData.muappMode)
                {
                    case VG_MUAPP_ACTIVE_AID_SESSION:
                    {
                        if( simLockGenericContext_p->activatedAidIndex != (Int8)index)
                        {
                            simLockContext_p->vgMuappData.index     = (Int8)index;
                            /*  To know how many sim applications are present*/
                            simLockContext_p->dirReqStartRecord     = 1;
                            simLockContext_p->dirReqNumRecord       = 0;
                            result = vgChManContinueAction (entity, SIG_APEX_SIM_READ_DIR_REQ);
                        }
                        else
                        {
                            result = RESULT_CODE_OK;
                        }
                    }
                    break;

                    case VG_MUAPP_ACTIVE_SESSION_STATE_IND:
                    {
                        result = setProfileValue(   entity,
                                                    PROF_MUAPP,
                                                    REPORTING_ENABLED);
                    }
                    break;

                    case VG_MUAPP_DEACTIVE_SESSION_STATE_IND:
                    {
                        result = setProfileValue(   entity,
                                                    PROF_MUAPP,
                                                    REPORTING_DISABLED);
                    }
                    break;

                    case VG_MUAPP_GET_SESSION_STATE_IND:
                    {
                        vgPutNewLine (entity);
                        vgPrintf(   entity,
                                    (const Char*)"%C: %d",
                                    getProfileValue( entity, PROF_MUAPP) );
                        vgPutNewLine (entity);
                    }
                    break;

                    default:
                    {
                        FatalParam( (Int8)simLockContext_p->vgMuappData.muappMode, 0, 0);
                    }
                    break;
                }
            }
        }
        break;

        case EXTENDED_ACTION: /* AT*MUAPP  */
        default:
        {
            result = RESULT_CODE_ERROR;
            break;
        }
    }

    return result;
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgMuappPrintEvent
 *
 * Parameters:  vgMuappEvent - MSMI event to print
 *
 * Returns:     Nothing
 *
 * Description: This function print the unsolicited *MUAPP events
 *-------------------------------------------------------------------------*/
void vgMuappPrintEvent( const VgMuappEvent *vgMuappEvent_p)
{
    VgmuxChannelNumber profileEntity = VGMUX_CHANNEL_INVALID;

    for ( profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
    {
        if ((isEntityActive (profileEntity)) &&
            (getProfileValue( profileEntity, PROF_MUAPP) == REPORTING_ENABLED))
        {
            vgPutNewLine( profileEntity);
            vgPrintf(   profileEntity,
                        (Char *)"*MUAPP: %d,%d,%d,",
                        vgMuappEvent_p->index,
                        (Int8)vgMuappEvent_p->aid.applicationType,
                        (Int8)vgMuappEvent_p->state);

            if( vgMuappEvent_p->aid.length != 0)
            {
                vgPutc( profileEntity, '\"');
                vgPutAlphaId(   profileEntity,
                                vgMuappEvent_p->aid.data,
                                vgMuappEvent_p->aid.length);
                vgPutc( profileEntity, '\"');
            }
            vgPutNewLine( profileEntity);
            vgSetCirmDataIndIsUrc(profileEntity, TRUE);
        }
    }
}

#if defined(FEA_SIM_IMSI_LOCK_CONTROL)
/*--------------------------------------------------------------------------
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     Non-standard AT command to customize function,
 *                  Enable/Disable the IMSI lock feature.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSlMIMSILOCK(CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t            result              = RESULT_CODE_OK;
    ExtendedOperation_t     operation           = getExtendedOperation (commandBuffer_p);
    Int32                   imsiLockState;

    switch (operation)
    {
        case EXTENDED_QUERY:  /* AT*MIMSILOCK? */
        {
             imsiLockState = (Boolean)getImsiLockState();
             vgPutNewLine(entity);

             /* Print out the *MIMSILOCK.... */
             vgPrintf( entity,
                  (Char *)"*MIMSILOCK: %d", imsiLockState);

             vgPutNewLine(entity);
             vgFlushBuffer(entity);
        }
        break;

        case EXTENDED_ASSIGN:   /* AT*MIMSILOCK=  */
        {
          /* Get functionality parameter (mandatory) */
          if (getExtendedParameter (commandBuffer_p,
                                     &imsiLockState,
                                      ULONG_MAX) == TRUE)
          {
            if ((imsiLockState == 0) ||
                (imsiLockState == 1))
            {
                setImsiLockState((Boolean)imsiLockState);
            }
            else
            {
              result = VG_CME_INVALID_INPUT_VALUE;
            }
          }
          else
          {
            result = VG_CME_INVALID_INPUT_VALUE;
          }
        }
        break;

        default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;
    }

    return (result);
}
#endif


/* END OF FILE */


