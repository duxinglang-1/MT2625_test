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
 * Procedures for General AT command execution
 *
 * Contains implementations of the following AT commands
 *
 * AT+CEER    - displays an extended report of the reason for the last call release
 * AT+CIMI    - displays the international mobile subscriber identity (IMSI)
 * AT*MLTS    - displays the local time stamp
 * AT+CPAS    - displays phone activity status
 * AT*MSPN    - displays service provider name from the SIM
 * AT+GCAP    - displays complete capabilities listing
 * AT+GMI     - displays the manufacturer identification
 * AT+GMM     - displays the model identification
 * AT+GMR     - displays the revision identification
 * AT+GOI     - displays the global object identification
 * AT+GSN     - displays serial number identification (IMEI)
 * AT*MUNSOL  - configures proprietary unsolicated indications
 * AT*MABORT  - abort AT command
 * ATI        - displays product identification information
 * ATP        - does nothing
 * ATT        - does nothing
 **************************************************************************/

#define MODULE_NAME "RVGNUT"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <gkisig.h>
#include <gkimem.h>
#include <ki_sig.h>

#include <rvutil.h>
#include <rvdata.h>
#include <rvprof.h>
#include <rvmmss.h>
#include <rvchman.h>
#include <rvcrerr.h>
#include <rvmmsigo.h>
#include <rvpfut.h>
#include <rvcfg.h>
#include <rvoman.h>
#include <rvgnut.h>
#include <rvgnsigo.h>
#include <rvgnsigi.h>
#include <rvcrhand.h>
#include <rvcrman.h>
#include <rvmmut.h>
#include <rvccsigi.h>
#include <rvcimxut.h>
#include <rvcrconv.h>
#include <n1cd_sig.h>
#include <n1tst_sig.h>
#include <tool_authentication.h>
#include <ut_mcc_mnc.h>

#include <frhsl.h>
#if defined (MTK_NVDM_MODEM_ENABLE)&& defined (MTK_NBIOT_TARGET_BUILD)
#include <nvdm_modem.h>
#endif

/***************************************************************************
 * For NB-IOT Project
 ***************************************************************************/
/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* call release text descriptions */
extern const AtIdentificationInfo atIdentificationInfoDefaultInit;

static const Char *vgApexCallReleaseInfo[NUM_OF_APEX_CAUSES] =
{
  (const Char*)"No cause was given",                      /* APEX_CAUSE_OK */
  (const Char*)"The called number was busy",              /* APEX_CAUSE_SUBSCRIBER_BUSY */
  (const Char*)"The network was congested",               /* APEX_CAUSE_CONGESTION */
  (const Char*)"There was no radio path available",       /* APEX_CAUSE_RADIO_PATH_UNAVAIL */
  (const Char*)"There was an error",                      /* APEX_CAUSE_ERROR */
  (const Char*)"The called number was unobtainable",      /* APEX_CAUSE_NUM_UNOBTAINABLE */
  (const Char*)"There was an authentication failure",     /* APEX_CAUSE_AUTH_FAILURE */
  (const Char*)"No number was given for the call",        /* APEX_CAUSE_NO_NUM_PRESENT */
  (const Char*)"There was a call control failure",        /* APEX_CAUSE_BL_CC_FAILURE */
  (const Char*)"There was a mobility management failure", /* APEX_CAUSE_BL_MM_FAILURE */
  (const Char*)"The requested channel was busy",          /* APEX_CAUSE_CHANNEL_BUSY */
  (const Char*)"FDN Mismatch",                            /* APEX_CAUSE_FDN_MISMATCH */
  (const Char*)"Bearer capability fail",                  /* APEX_CAUSE_BC_FAIL  */
  (const Char*)"EMERGENCY_CALLS_ONLY",                    /* APEX_CAUSE_EMERGENCY_CALLS_ONLY */
  (const Char*)"ACM limit exceeded",                      /* APEX_CAUSE_ACM_LIMIT_EXCEEDED */
  (const Char*)"Hold error",                              /* APEX_CAUSE_HOLD_ERROR */
  (const Char*)"Busy processing Request",                 /* APEX_CAUSE_BUSY_PROCESSING_REQUEST */
  (const Char*)"Active channel unavailable",              /* APEX_CAUSE_ACTIVE_CHANNEL_UNAVAILABLE */
  (const Char*)"Outgoing calls barred",                   /* APEX_CAUSE_OUTGOING_CALL_BARRED */
  (const Char*)"Number has been blacklisted",             /* APEX_CAUSE_NUMBER_BLACKLISTED */
  (const Char*)"Blacklist full",                          /* APEX_CAUSE_BLACKLIST_FULL */
  (const Char*)"Redial timer still running",              /* APEX_CAUSE_REDIAL_NOT_TIMED_OUT */
  (const Char*)"Channel Mode Modify fail",                /* APEX_CAUSE_MODIFY_FAIL */
  (const Char*)"Not in control",                          /* APEX_CAUSE_NOT_IN_CONTROL */
  (const Char*)"No cause was given",                      /* APEX_CAUSE_NO_CAUSE */
  (const Char*)"Reestablishment barred",                  /* APEX_CAUSE_ERROR_REESTABLISHMENT_BARRED */
  (const Char*)"STK call is emergency",                   /* APEX_CAUSE_SIMTOOLKIT_CALL_IS_EMERGENCY */
  (const Char*)"Number barred by SIM",                    /* APEX_CAUSE_BARRED_BY_SIM */
};

static const Char *vgTimerCallReleaseInfo[NUM_OF_TIMER_ERROR] =
{
  (const Char*)"Connection Timer Expired"          /* CI_CONNECT_TIMER_EXPIRED */
};

#define MAX_MUNSOL_ID_LEN (2)

typedef struct MunsolInfoTag
{
  Char         vgMunsolStr[MAX_MUNSOL_ID_LEN + NULL_TERMINATOR_LENGTH];
  VgProfileBit profileBit;
}
MunsolInfo;

static const MunsolInfo munsolInfo[] =
{
  { "SQ",  PROF_BIT_MSQN    },
  { "FN",  PROF_BIT_MFPLMN  },
  { "SM",  PROF_BIT_SMSINFO },
  { "XX",  END_OF_MUNSOL_BITS }
};

typedef enum VgRouteMmiOptionIdTag
{
  VG_ROUTE_MMI_OPTION_HOOK_TO_MMI = 0,
  VG_ROUTE_MMI_OPTION_UNSOL = 1,
  NUM_OF_MROUTEMMI_BITS
}VgRouteMmiOptionId;

static const VgProfileBit MRouteMmiProfiles[] =
{
    PROF_BIT_MROUTEMMI_HOOK_TO_MMI,
    PROF_BIT_MROUTEMMI_MMI_UNSOL,
    END_OF_MROUTEMMI_BITS,
};

#define MNVMQ_CHIP_NAME ("2625")

typedef enum VgMnvmqAuthStatusTag
{
  MNVMQ_NO_AUTH_REQ = 0,
  MNVMQ_AUTH_REQ = 1
}VgMnvmqAuthStatus;

typedef enum VgMnvmauthResultTag
{
  MNVMAUTH_AUTH_FAILED = 0,
  MNVMAUTH_AUTH_PASSED = 1
}VgMnvmauthResult;
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_1   (0x00000001 << 0)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_2   (0x00000001 << 1)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_3   (0x00000001 << 2)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_5   (0x00000001 << 3)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_8   (0x00000001 << 4)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_11  (0x00000001 << 5)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_12  (0x00000001 << 6)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_13  (0x00000001 << 7)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_17  (0x00000001 << 8)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_18  (0x00000001 << 9)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_19  (0x00000001 << 10)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_20  (0x00000001 << 11)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_25  (0x00000001 << 12)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_26  (0x00000001 << 13)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_28  (0x00000001 << 14)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_31  (0x00000001 << 15)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_66  (0x00000001 << 16)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_70  (0x00000001 << 17)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_21  (0x00000001 << 18)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_4   (0x00000001 << 19)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_71  (0x00000001 << 20)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_85  (0x00000001 << 21)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_14  (0x00000001 << 22)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_72  (0x00000001 << 23)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_73  (0x00000001 << 24)
#define MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_74  (0x00000001 << 25)


/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static ResultCode_t vgGnMFtrCfgVarCallInfoStorage(  Int32    newValue,
                                                    Boolean *modification_p);
static ResultCode_t vgGnMFtrCfgVarHandleConcatSms(  Int32    newValue,
                                                    Boolean *modification_p);
static ResultCode_t vgGnMFtrCfgVarMeLocation(   Int32    newValue,
                                                Boolean *modification_p);
static ResultCode_t vgGnMFtrCfgVarModemMode(    Int32    newValue,
                                                Boolean *modification_p);
static void vgGnMFtrCfgPrintAllVariablesValue( const VgmuxChannelNumber entity);
static ResultCode_t vgGnMFtrCfgPrintVariableValue( const VgmuxChannelNumber entity,
                                                   Int32 variable);
static void vgGnRouteMMIPrintOption(    Int8 option,
                                        const VgmuxChannelNumber entity);



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
 * Local Functions
 ***************************************************************************/
#if defined (MTK_NVDM_MODEM_ENABLE)
/*--------------------------------------------------------------------------
*
* Function:    vgConvertNvramStatus
*
* Parameters:
*              entity          - mux channel number
*              status          - Status returned from NVRAM function
*
* Returns:     VgNvmStatus     - Status to display
*
* Description: Converts NVRAM status to result generated by AT command.
*-------------------------------------------------------------------------*/
static VgNvmStatus vgConvertNvramStatus  (const VgmuxChannelNumber entity, nvdm_modem_status_t status)
{
  VgNvmStatus vgNvmStatus;

  switch (status)
  {                  
    case NVDM_MODEM_STATUS_NO_BACKUP:
    {
      vgNvmStatus = VG_NVM_STATUS_OTHER_FAILURE;
      break;
    }
    case NVDM_MODEM_STATUS_INVALID_PARAMETER:
    {
      vgNvmStatus = VG_NVM_STATUS_OTHER_FAILURE;
      break;
    }
    case NVDM_MODEM_STATUS_ITEM_NOT_FOUND:
    {
      vgNvmStatus = VG_NVM_STATUS_OTHER_FAILURE;
      break;
    }
    case NVDM_MODEM_STATUS_INSUFFICIENT_SPACE:
    {
      vgNvmStatus = VG_NVM_STATUS_OTHER_FAILURE;
      break;
    }
    case NVDM_MODEM_STATUS_INCORRECT_CHECKSUM:
    {
      vgNvmStatus = VG_NVM_STATUS_NVRAM_CORRUPT;
      break;
    }
    case NVDM_MODEM_STATUS_ERROR:
    {
      vgNvmStatus = VG_NVM_STATUS_OTHER_FAILURE;
      break;
    }
    case NVDM_MODEM_STATUS_OK:
    {
      vgNvmStatus = VG_NVM_STATUS_OK;
      break;                      
    }
    default:
    {
      FatalParam(entity, status, 0);
      break;
    }
  }        

  return (vgNvmStatus);
}
#endif /* MTK_NVDM_MODEM_ENABLE */

/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
*
* Function:    vgGnMFASSERT
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT*MFASSERT AT command.  It will
*              FatalFail when the command it entered.
*-------------------------------------------------------------------------*/

ResultCode_t vgGnMFASSERT (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_ERROR;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);

  switch (operation)
  {
    case EXTENDED_ACTION:   /* AT*MFASSERT   */
    {
      /* Just Assert */
      FatalFail ("ATCI Modem Fatal Assert");
      break;
    }
    case EXTENDED_RANGE:    /* AT*MFASSERT=? */
    case EXTENDED_ASSIGN:   /* AT*MFASSERT=  */
    case EXTENDED_QUERY:    /* AT*MFASSERT?  */
    default:
    {
      /* Just generate the error */
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnCLTS
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT*MLTS (Get Local Time Stamp) command
*              which displays local time stamp information (passed up in
*              an ApexMmNetworkInfoInd signal).
*-------------------------------------------------------------------------*/

ResultCode_t vgGnMLTS (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_ERROR;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
  MobilityContext_t   *mobilityContext_p = ptrToMobilityContext ();
  Int32               tmpVar;

  switch (operation)
  {
    case EXTENDED_RANGE:    /* *MLTS=? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"%C: (0,1),\"yy/MM/dd,hh:mm:ss+/-zz\"");
      vgPutNewLine (entity);
      result = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_ACTION:   /* AT*MLTS   */
    {
      vgPutNewLine (entity);
      vgPrintf (entity,
                (const Char*)"%C: %s",
                 mobilityContext_p->vgMLTSString);
      vgPutNewLine (entity);
      result = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_ASSIGN:   /* AT*MLTS=   */

      if (getExtendedParameter (commandBuffer_p, &tmpVar, ULONG_MAX))
      {
        if (tmpVar > REPORTING_ENABLED)
        {
          /* no parameter or out of range */
          result = VG_CME_INVALID_INPUT_VALUE;
        }
        else
        {
          setProfileValue(entity, PROF_MLTS, (Int8)tmpVar);
          result = RESULT_CODE_OK;
        }
      }
      else
      {
        result = VG_CME_INVALID_INPUT_VALUE;
      }
      break;
    case EXTENDED_QUERY:    /* AT*MLTS?  */
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"%C: %d", getProfileValue(entity, PROF_MLTS));
      vgPutNewLine (entity);
      result = RESULT_CODE_OK;
      break;
    default:
    {
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnGOI
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+GOI and AT+CGOI commands which
*              displays the global object identification code.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnGOI (CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);

  switch (operation)
  {
    case EXTENDED_RANGE:  /* AT+GOI=? */
    {
      break;
    }
    case EXTENDED_ACTION: /* AT+GOI */
    {
      /* see if we can get an NVRAM connection */
      vgPutNewLine (entity);
      vgPuts (entity, atIdentificationInfoDefaultInit.atIdentificationText.globalId);

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


/*--------------------------------------------------------------------------
*
* Function:    vgGnGSN
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+GSN command which displays the
*              TA serial number identification (IMEI)
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnGSN ( CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t          result    = RESULT_CODE_ERROR;
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  ChManagerContext_t    *chManagerContext_p = ptrToChManagerContext ();
  MobilityContext_t   	*mobilityContext_p = ptrToMobilityContext ();

  switch (operation)
  {
    case EXTENDED_RANGE:  /* AT+GSN=? */
    {
      result = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_ACTION: /* AT+GSN */
    {

      /* Always sent request to AB even if it looks like nothing has changed */
      chManagerContext_p->isImmediate = TRUE;
#if defined(USE_ABAPP)
      mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
#endif
      result = vgChManContinueAction (entity, SIG_APEX_MM_READ_MOBILE_ID_REQ);

      break;
    }
    default:
    {
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnCGSN
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+CGSN command which displays the
*              TA serial number identification (IMEI), SVN or both.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnCGSN ( CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t          result    = RESULT_CODE_ERROR;
  GeneralContext_t      *generalContext_p = ptrToGeneralContext(entity);
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);

  Int32                 tmpVar;    
  ChManagerContext_t    *chManagerContext_p = ptrToChManagerContext ();
  MobilityContext_t     *mobilityContext_p = ptrToMobilityContext ();
#if defined (ATCI_SLIM_DISABLE)


  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  switch (operation)
  {
    case EXTENDED_RANGE:  /* AT+CGSN=? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"%C: (0-3)");
      vgPutNewLine (entity);
      result = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_ASSIGN: /* AT+CGSN= */
    {
      /* Get <snt> parameter */
      if (getExtendedParameter (commandBuffer_p, &tmpVar, ULONG_MAX))
      {
        if (tmpVar >= VG_CGSN_INVALID_SNT)
        {
          /* no parameter or out of range */
          result = VG_CME_INVALID_INPUT_VALUE;
        }
        else
        {
          generalContext_p->cgsnSnt = (VgCgsnSnt)tmpVar;
          result = RESULT_CODE_OK;
        }
      }
      else
      {
        result = VG_CME_INVALID_INPUT_VALUE;
      }

      if (result == RESULT_CODE_OK)
      {
          /* Always sent request to AB even if it looks like nothing has changed */
          chManagerContext_p->isImmediate = TRUE;
#if defined(USE_ABAPP)
          mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
#endif
          result = vgChManContinueAction (entity, SIG_APEX_MM_READ_MOBILE_ID_REQ);
      }
      break;
    }
    case EXTENDED_ACTION: /* AT+CGSN */
    {
      generalContext_p->cgsnSnt = VG_CGSN_SNT_SN;

      result = vgChManContinueAction (entity, SIG_APEX_MM_READ_MOBILE_ID_REQ);

      break;
    }
    default:
    {
      break;
    }
  }

  return (result);
}


/*--------------------------------------------------------------------------
*
* Function:    vgGnMCGSN
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT*MCGSN command which write the
*              TA serial number identification (IMEI), SN.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnMCGSN ( CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t          result             = RESULT_CODE_ERROR;
  GeneralContext_t     *generalContext_p   = ptrToGeneralContext(entity);
  ExtendedOperation_t   operation          = getExtendedOperation (commandBuffer_p);
  
  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  Boolean                  vgNvmAccessAuthorised = (generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised);
  
  Int32                 tmpVar;
  Int16                 strLength;
  Int32                 index;
  Char                  tmpString[MAX_UE_ID_LENGTH + 1];

#if defined (MTK_NVDM_MODEM_ENABLE)
  nvdm_modem_status_t   status;
  VgNvmStatus              vgNvmStatus;
#endif

#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)
  tool_auth_response_t     auth_response = TOOL_AUTH_REQUIRED;
#endif

  MobileID           tempMobileId;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  memset(&tempMobileId,0,sizeof(MobileID));


  switch (operation)
  {
    case EXTENDED_QUERY:    /*AT*MCGSN?*/
    case EXTENDED_RANGE:    /* AT*MCGSN=? */
    {
      result = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_ASSIGN: /* AT*MCGSN= */
    {
      /* Get <sn_id_type> parameter */
      if (getExtendedParameter (commandBuffer_p, &tmpVar, ULONG_MAX))
      {
        if (tmpVar >= VG_MCGSN_INVALID_SNT)
        {
          /* no parameter or out of range */
          result = VG_CME_INVALID_INPUT_VALUE;
        }
        else
        {
            /* Only allow access to protected area if authorisation occurred */
            if (vgNvmAccessAuthorised)
            {
                generalContext_p->mcgsnData.mcgsnSnt = (VgMcgsnSnt)tmpVar;
                result = RESULT_CODE_OK;
            }
#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)
            /* To check if needs tool authentication */
            else if(tool_auth_is_required(&auth_response) == TOOL_AUTH_STATUS_OK)
            {
                switch(auth_response)
                {
                    case TOOL_AUTH_NOT_REQUIRED:
                    {
                      generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised = TRUE;
                      result = RESULT_CODE_OK;
                      break;
                    }
                    case TOOL_AUTH_REQUIRED:
                    {
                      result = VG_CME_OPERATION_NOT_ALLOWED;
                      break;
                    }
                    default:
                    {
                      FatalFail("ATCI NVDM: Illegal auth response");
                      break;  
                    }
                }
             }
#endif
            else
            {
              result = VG_CME_OPERATION_NOT_ALLOWED;
            }
        }
      }
      else
      {
        result = VG_CME_INVALID_INPUT_VALUE;
      }
      
      /*Get <id_value> string type*/
      if (result == RESULT_CODE_OK)
      {
        if (getExtendedString ( commandBuffer_p,
               &tmpString[0],
                MAX_UE_ID_LENGTH,
                &strLength) == TRUE)
        {
            if (VG_MCGSN_SNT_SN == tmpVar)
            {
                    generalContext_p->mcgsnData.mcgsnSnt           = (VgMcgsnSnt)tmpVar;
                    generalContext_p->mcgsnData.digitImeiArraySize = 0;
                    generalContext_p->mcgsnData.digitSNArraySize   = strLength;

                    memcpy(generalContext_p->mcgsnData.digitMobileId, tmpString, strLength);

            }
            else if((VG_MCGSN_SNT_IMEI == tmpVar) && (IMEI_LENGTH == strLength))
            {/*Get IMEI ID VALUE*/
                for(index = 0; index < IMEI_LENGTH; index++)
                {
                    if(FALSE == getDeciaValue(tmpString[index],&(generalContext_p->mcgsnData.digitMobileId[index])))
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
                        break;
                    }
                }
                
                if (result == RESULT_CODE_OK)
                {
                    generalContext_p->mcgsnData.mcgsnSnt           = (VgMcgsnSnt)tmpVar;
                    generalContext_p->mcgsnData.digitImeiArraySize = IMEI_LENGTH;
                    generalContext_p->mcgsnData.digitSNArraySize   = 0;
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
      
      if (result == RESULT_CODE_OK)
      {
        //chManagerContext_p->isImmediate         = TRUE;
        result = vgChManContinueAction (entity, SIG_APEX_MM_WRITE_MOBILE_ID_REQ);
      }
      break;
    }
    
    default:
    {
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnMCGHWN
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT*MCGHWN command which quert the name of hardware.
*
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMCGHWN ( CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity)
{
  ResultCode_t          result             = RESULT_CODE_ERROR;
  GeneralContext_t     *generalContext_p   = ptrToGeneralContext(entity);
  ExtendedOperation_t   operation          = getExtendedOperation (commandBuffer_p); 

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  switch (operation)
  {
    case EXTENDED_QUERY:    /*AT*MCGHWN?*/
    case EXTENDED_RANGE:    /* AT*MCGHWN=? */
    {
      result = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_ACTION: /* AT*MCGHWN */
    {

      vgPutNewLine (entity);
      vgPrintf (entity,
                (const Char*)"+MCGHWN: MT2625");
      vgPutNewLine (entity);
      result = RESULT_CODE_OK;
      break;
    }    
    
    default:
    {
      break;
    }
  }

  return (result);
}


/*--------------------------------------------------------------------------
*
* Function:    vgGnMOPTLOCK
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT*MOPTLOCK command which lock the opt item.
*
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMOPTLOCK ( CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity)
{
  ResultCode_t          result             = RESULT_CODE_ERROR;
  GeneralContext_t     *generalContext_p   = ptrToGeneralContext(entity);
  ExtendedOperation_t   operation          = getExtendedOperation (commandBuffer_p);
  


#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  switch (operation)
  {
    case EXTENDED_QUERY:    /*AT*MOPTLOCK?*/
    case EXTENDED_RANGE:    /* AT*MOPTLOCK=? */
    {
      result = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_ACTION: /* AT*MOPTLOCK */
    {
#if defined (MTK_NVDM_MODEM_ENABLE)&& defined (MTK_NBIOT_TARGET_BUILD)
      nvdm_modem_lock_otp_area();
#endif
      break;
    }
    
    default:
    {
      break;
    }
  }

  return (result);
}


/*--------------------------------------------------------------------------
*
* Function:    vgGnCIMI
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT+CIMI which displays the international
*              mobile subscriber identity (IMSI)
*-------------------------------------------------------------------------*/

ResultCode_t vgGnCIMI (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t             result = RESULT_CODE_OK;
  ExtendedOperation_t      operation = getExtendedOperation (commandBuffer_p);
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
  Int8                     digit;
  Int8                     displayDigit;
  Int8                     val8 = 0;


  switch (operation)
  {
    case EXTENDED_QUERY:       /* AT+CIMI? */
    case EXTENDED_ASSIGN:      /* AT+CIMI= */
    {
      result = RESULT_CODE_ERROR;
      break;
    }
    case EXTENDED_RANGE:       /* AT+CIMI=? */
    {
      break;
    }
    case EXTENDED_ACTION:      /* AT+CIMI */
    {
      /* if SIM is not ready so return reason */
      if (simLockGenericContext_p->simState != VG_SIM_READY)
      {
        result = vgGetSimCmeErrorCode ();
      }
      else
      {
        vgPutNewLine (entity);

        val8 = simLockGenericContext_p->simInfo.imsi.contents[0];
        displayDigit = (val8 >> 4) & 0x0F;
        if (displayDigit != 0x0F)
        {
          vgPutc (entity, (Char) (displayDigit + '0'));
        }
        for ( digit = 1; digit < simLockGenericContext_p->simInfo.imsi.length ; digit++)
        {
          val8 = simLockGenericContext_p->simInfo.imsi.contents[digit];
          displayDigit = val8 & 0x0F;
          if (displayDigit != 0x0F)
          {
            vgPutc (entity, (Char) (displayDigit + '0'));
          }

          displayDigit = (val8 >> 4) & 0x0F;
          if (displayDigit != 0x0F)
          {
             vgPutc (entity, (Char) (displayDigit + '0'));
          }
        }
        vgPutNewLine (entity);
      }
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


/*--------------------------------------------------------------------------
*
* Function:    vgGnP
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the ATP command which does nothing
*              because in GSM there is no Pulse dial.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnP (CommandLine_t *commandBuffer_p,
                     const VgmuxChannelNumber entity)
{
  ResultCode_t result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);

  PARAMETER_NOT_USED(entity);

  switch (operation)
  {
    case EXTENDED_ACTION:
    {
      break;
    }
    case EXTENDED_QUERY:
    case EXTENDED_ASSIGN:
    case EXTENDED_RANGE:
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
* Function:    vgGnI
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the ATI command which displays
*              product identification information
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnI (CommandLine_t *commandBuffer_p,
                     const VgmuxChannelNumber entity)
{
  Int32         value;

  getDecimalValueSafe (commandBuffer_p, &value);
  if(value>10)
  {
      return RESULT_CODE_ERROR;
  }
  else
  {
    vgPutNewLine (entity);
    vgPuts (entity, atIdentificationInfoDefaultInit.atIdentificationText.manufacturerId);
    vgPuts (entity, atIdentificationInfoDefaultInit.atIdentificationText.modelId);
    vgPuts (entity, atIdentificationInfoDefaultInit.atIdentificationText.swRevision);
    vgPuts (entity, atIdentificationInfoDefaultInit.atIdentificationText.hwRevision);
      
    return RESULT_CODE_OK;
    
 
  }    

}
/*--------------------------------------------------------------------------
*
* Function:    vgGnT
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the ATT command which does nothing
*              because in GSM is no Tone dial.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnT (CommandLine_t *commandBuffer_p,
                     const VgmuxChannelNumber entity)
{
  ResultCode_t result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);

  PARAMETER_NOT_USED(entity);

  switch (operation)
  {
    case EXTENDED_ACTION:
    {
      break;
    }
    case EXTENDED_QUERY:
    case EXTENDED_ASSIGN:
    case EXTENDED_RANGE:
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
* Function:    vgGnGMR
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+GMR, AT+FMR, AT+CGMR commands
*              which all display the revision identification
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnGMR (CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);

  switch (operation)
  {
    case EXTENDED_RANGE:  /* AT+GMR=? */
    {
      break;
    }
    case EXTENDED_ACTION: /* AT+GMR... */
    {
      /* see if we can get an NVRAM connection */
      vgPutNewLine (entity);
      vgPuts (entity, atIdentificationInfoDefaultInit.atIdentificationText.swRevision);        

      break;
    }
    case EXTENDED_ASSIGN: /* AT+GMR=... */
    case EXTENDED_QUERY:  /* AT+GMR? */
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
* Function:    vgGnGMM
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+GMM, AT+FMR, AT+CGMR commands
*              which all display the model identification
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnGMM (CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);

  switch (operation)
  {
    case EXTENDED_RANGE:  /* AT+GMM=? */
    {
      break;
    }
    case EXTENDED_ACTION: /* AT+GMM... */
    {
      /* see if we can get an NVRAM connection */
      vgPutNewLine (entity);
      vgPuts (entity, atIdentificationInfoDefaultInit.atIdentificationText.modelId);


      break;
    }
    case EXTENDED_QUERY:  /* AT+GMM? */
    case EXTENDED_ASSIGN: /* AT+GMM=... */
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
* Function:    vgGnGCAP
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+GCAP command which displays
*              complete capabilities listing
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnGCAP (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);

  switch (operation)
  {
    case EXTENDED_RANGE:  /* AT+GCAP=? */
    {
      break;
    }
    case EXTENDED_ACTION: /* AT+GCAP... */
    {
      vgPutNewLine (entity);
      vgPuts (entity, (const Char*)"+GCAP: +CGSM");
      break;
    }
    case EXTENDED_QUERY:  /* AT+GCAP? */
    case EXTENDED_ASSIGN: /* AT+GCAP=... */
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
* Function:    vgGnCEER
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+CEER command which displays an
*              extended report of the reason for the last call release
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnCEER (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t         result = RESULT_CODE_OK;
  ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
  CallContext_t        *callContext_p   = ptrToCallContext (entity);

  FatalCheck(callContext_p != PNULL, entity, 0, 0);

  switch (operation)
  {
    case EXTENDED_RANGE:  /* AT+CEER=? */
    {
      break;
    }
    case EXTENDED_ACTION: /* AT+CEER... */
    {
      if (getProfileValue(entity, PROF_MCEERMODE) == 0)
      /* We are in textual response mode.. */
      {
        switch (callContext_p->vgErrorType)
        {
          case CI_CALL_RELEASE_ERROR_APEX:
          {
            vgPutNewLine (entity);
            vgPrintf (
             entity,
              (const Char*)"+CEER: %s",
               vgApexCallReleaseInfo[callContext_p->vgApexCallReleaseError]);
            vgPutNewLine (entity);
            break;
          }
          case CI_CALL_RELEASE_ERROR_TIMER:
          {
            vgPutNewLine (entity);
            vgPrintf (
             entity,
              (const Char*)"+CEER: %s",
               vgTimerCallReleaseInfo[callContext_p->vgTimerCallReleaseError]);
            vgPutNewLine (entity);
            break;
          }
          case CI_CALL_RELEASE_ERROR_GSM_CAUSE:
          {
#  if defined(UPGRADE_RAVEN_NO_VERBOSE)
            /* This option not supported in this build. */
            result = VG_CME_OPERATION_NOT_SUPPORTED;
#  else
            vgPutNewLine (entity);
            vgPrintf (
             entity,
              (const Char*)"+CEER: %s",
                vgGetVerboseResultString (
                 vgGetGsmCauseCmeErrorCode (
                  callContext_p->vgGsmCauseCallReleaseError )));
            vgPutNewLine (entity);
#  endif
            break;
          }
          default:
          {
            /* invalid error type */
            FatalParam (callContext_p->vgErrorType, entity, 0);
            /* reset error information to default */
            vgSetApexGsmCallReleaseError (APEX_CAUSE_OK, TRUE, CAUSE_NORMAL_CLEARING, PD_CC, entity);
            /* indicate an error has ocurred */
            result = RESULT_CODE_ERROR;
            break;
          }
        }
      }
      else
      /* We are in numeric response mode.. */
      {
        switch (callContext_p->vgErrorType)
        {
          case CI_CALL_RELEASE_ERROR_APEX:
          case CI_CALL_RELEASE_ERROR_GSM_CAUSE:
          {
           /* For APEX we also set the GSM cause for the numeric case. */
            vgPutNewLine (entity);
            vgPrintf (
             entity,
              (const Char*)"+CEER: %d",
               /* For LTE we need to make sure we mask off the LTE cause value
                * bit before we display the cause
                */
               (callContext_p->vgGsmCauseCallReleaseError) & ~(LTE_CAUSE_BASE));
            vgPutNewLine (entity);
            break;
          }
          case CI_CALL_RELEASE_ERROR_TIMER:
          {
            /* For timer timeout - assume no user response */
            vgPutNewLine (entity);
            vgPrintf (
             entity,
              (const Char*)"+CEER: %d",
               CAUSE_NO_USER_RESPONDING);
            vgPutNewLine (entity);
            break;
          }
          default:
          {
            /* invalid error type */
            FatalParam (callContext_p->vgErrorType, entity, 0);
            /* reset error information to default */
            vgSetApexGsmCallReleaseError (APEX_CAUSE_OK, TRUE, CAUSE_NORMAL_CLEARING, PD_CC, entity);
            /* indicate an error has ocurred */
            result = RESULT_CODE_ERROR;
            break;
          }
        }
      }
      break;
    }
    case EXTENDED_QUERY:  /* AT+CEER? */
    case EXTENDED_ASSIGN: /* AT+CEER=... */
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
* Function:    vgGnGMI
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+GMI, AT+FMI, AT+CGMI commands
*              which all display the manufacturer identification
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnGMI (CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);

  switch (operation)
  {
    case EXTENDED_RANGE:  /* AT+GMI=? */
    {
      break;
    }
    case EXTENDED_ACTION: /* AT+GMI... */
    {
      /* see if we can get an NVRAM connection */
      vgPutNewLine (entity);
      vgPuts (entity, atIdentificationInfoDefaultInit.atIdentificationText.manufacturerId);

      break;
    }
    case EXTENDED_QUERY:  /* AT+GMI? */
    case EXTENDED_ASSIGN: /* AT+GMI=... */
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
* Function:    vgGnMSPN
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT*MSPN command which retrieves
*              the service provider name from the SIM
*-------------------------------------------------------------------------*/

ResultCode_t vgGnMSPN (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t            result = RESULT_CODE_OK;
  ExtendedOperation_t     operation = getExtendedOperation (commandBuffer_p);

  switch (operation)
  {
    case EXTENDED_QUERY:       /* AT*MSPN? */
    {
      /* request service provider name */
      result = vgChManContinueAction (entity, APEX_SIM_READ_SPN_REQ);
      break;
    }
    case EXTENDED_RANGE:       /* AT*MSPN=? */
    case EXTENDED_ASSIGN:      /* AT*MSPN= */
    case EXTENDED_ACTION:      /* AT*MSPN */
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
* Function:    vgGnSPN
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT^SPN command which retrieves
*              the service provider name from the SIM
*-------------------------------------------------------------------------*/

ResultCode_t vgGnSPN (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t            result = RESULT_CODE_OK;
  ExtendedOperation_t     operation = getExtendedOperation (commandBuffer_p);
  Int32 paramValue = 0;

  switch (operation)
  {
    case EXTENDED_RANGE:       /* AT^SPN=? */
      vgPutNewLine(entity);
      vgPrintf( entity, (Char *)"^SPN: (0-1)");
      vgPutNewLine(entity);
      break;

    case EXTENDED_ASSIGN:      /* AT^SPN= */
    {
      if ((getExtendedParameter ( commandBuffer_p,
                                     &paramValue,
                                      1) == TRUE)&&
            ((paramValue == 1) || (paramValue == 0)) )
        {
          /* parameter spn_type is ignored - we always go for the current SPN */
          /* request service provider name */
          result = vgChManContinueAction (entity, APEX_SIM_READ_SPN_REQ);
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
       break;
    }

    case EXTENDED_QUERY:       /* AT^SPN? */
    case EXTENDED_ACTION:      /* AT^SPN */
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
* Function:    vgGnMUNSOL
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT*MUNSOL commands
*              which enable/disable proprietary unsolicated indications
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnMUNSOL (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);

  Int32    index;
  Char    unsolId[MAX_MUNSOL_ID_LEN + NULL_TERMINATOR_LENGTH] = {0};
  Int16   unsolIdLength;
  Boolean matchFound = FALSE;
  Int32   mode;

  switch (operation)
  {
    case EXTENDED_RANGE:  /* AT*MUNSOL=? */
    {
      vgPutNewLine (entity);
      vgPrintf     (entity, (Char *)"%C: (");

      index = 0;
      while (munsolInfo[index].profileBit != END_OF_MUNSOL_BITS)
      {
        if (index > 0)
        {
          vgPutc (entity, ',');
        }
        vgPrintf (entity, (Char *)"\"%s\"", munsolInfo[index].vgMunsolStr);

        index++;
      }
      vgPuts (entity, (Char *)")");
      break;
    }
    case EXTENDED_ASSIGN: /* AT*MUNSOL=... */
    {
      if ((getExtendedString (commandBuffer_p,
                               &unsolId[0],
                                MAX_MUNSOL_ID_LEN,
                                 &unsolIdLength) == TRUE) &&
          (getExtendedParameter (commandBuffer_p,
                                  &mode,
                                   ULONG_MAX) == TRUE))
      {
        index = 0;
        while ((munsolInfo[index].profileBit != END_OF_MUNSOL_BITS) &&
               (matchFound == FALSE))
        {
          if (memcmp(&munsolInfo[index].vgMunsolStr[0],
                      &unsolId[0],
                       unsolIdLength) == 0)
          {
            matchFound = TRUE;
          }
          else
          {
            index++;
          }
        }

        if (matchFound == TRUE)
        {
          switch (mode)
          {
            case REPORTING_DISABLED:
            {
              result = setProfileValueBit (entity,
                                            PROF_MUNSOL,
                                             munsolInfo[index].profileBit,
                                              REPORTING_DISABLED);
              break;
            }
            case REPORTING_ENABLED:
            {
              result = setProfileValueBit (entity,
                                            PROF_MUNSOL,
                                             munsolInfo[index].profileBit,
                                              REPORTING_ENABLED);

              /* display signal stength indication immediately after
               * indication presentation is enabled */
              if ((munsolInfo[index].profileBit == PROF_BIT_MSQN) &&
                  (result == RESULT_CODE_OK))
              {
                viewCESQ (entity, CESQ_MSQN_UNSOLICITED);
              }
              break;
            }
            case REPORTING_QUERY:
            {
              vgPutNewLine (entity);
              vgPrintf (entity,
                        (const Char*)"%C: %d",
                          getProfileValueBit (entity,
                                               PROF_MUNSOL,
                                                munsolInfo[index].profileBit));
              vgPutNewLine (entity);
              break;
            }
            default:
            {
              result = VG_CME_INVALID_INPUT_VALUE;
              break;
            }
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
      break;
    }
    case EXTENDED_ACTION: /* AT*MUNSOL... */
    case EXTENDED_QUERY:  /* AT*MUNSOL? */
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
* Function:    vgGnMABORT
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: abort AT command
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnMABORT (CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
  ResultCode_t              result                      = RESULT_CODE_OK;
  ExtendedOperation_t       operation                   = getExtendedOperation (commandBuffer_p);
  ChannelContext_t          *channelContext_p           = ptrToChannelContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (channelContext_p != PNULL, entity, 0, 0);
#endif
  switch (operation)
  {
    case EXTENDED_ACTION: /* AT*MABORT... */
    {
      /* Only run abort command if we have a PLMN list Req ongoing - otherwise
       * just generate OK string
       */
      if (channelContext_p->canRunMabortCmd)
      {
        channelContext_p->canRunMabortCmd = FALSE;
        result = vgChManContinueAction (entity, SIG_APEX_MM_ABORT_PLMNLIST_REQ);
      }  
      break;
    }

    case EXTENDED_QUERY:  /* AT*MABORT? */
    case EXTENDED_RANGE:  /* AT*MABORT=? */
    case EXTENDED_ASSIGN: /* AT*MABORT=... */
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
* Function:    vgGnHVER
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Request Hardware Version
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnHVER (CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);

  switch (operation)
  {
    case EXTENDED_ACTION: /* AT^HVER... */
    {
      /* see if we can get an NVRAM connection */
      vgPutNewLine (entity);
      vgPrintf(entity, (const Char*)"%C: ");
      vgPuts (entity, atIdentificationInfoDefaultInit.atIdentificationText.hwRevision);

      break;
    }

    case EXTENDED_RANGE:  /* AT^HVER=? */
    case EXTENDED_QUERY:  /* AT^HVER? */
    case EXTENDED_ASSIGN: /* AT^HVER=... */
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
 * Function:        vgGnMFtrCfg
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     Non-standard AT command to configure some global features
 *                  like modem mode or ME storage
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgGnMFtrCfg(CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ExtendedOperation_t         operation = getExtendedOperation (commandBuffer_p);
    GeneralGenericContext_t    *generalGenericContext_p  = ptrToGeneralGenericContext();
    ResultCode_t                result = RESULT_CODE_OK;
    Int32                       mode;
    Int32                       variable;
    Int32                       value;
    Boolean                     present = FALSE;
    Boolean                     modification = FALSE;

    if( generalGenericContext_p->vgMFtrCfgData.initialised)
    {
        switch (operation)
        {
            case EXTENDED_RANGE:  /* AT*MFTRCFG=? */
                {
                    vgPutNewLine(entity);
                    vgPrintf(entity, (const Char*)"%C: (1-2),(0-3),(0-1),(0-1)");
                    vgPutNewLine(entity);
                }
                break;

            case EXTENDED_ASSIGN: /* AT*MFTRCFG=... */
                {
                    if( getExtendedParameter (commandBuffer_p, &mode, ULONG_MAX) != TRUE)
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
                    }
                    else if( getExtendedParameter (commandBuffer_p, &variable, ULONG_MAX) != TRUE)
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
                    }
                    else
                    {
                        if( mode == VG_MFTRCFG_MODE_WRITE)
                        {
                            /* Read the variable value*/
                            if( getExtendedParameterPresent(commandBuffer_p,
                                                            &value,
                                                            ULONG_MAX,
                                                            &present) != TRUE ||
                                present == FALSE)
                            {
                                result = VG_CME_INVALID_INPUT_VALUE;
                            }
                            else
                            {
                                /* Valid variable value*/
                                switch (variable)
                                {
                                    case VG_MFTRCFG_VAR_MODEM_MODE:
                                        {
                                            result = vgGnMFtrCfgVarModemMode( value, &modification);
                                        }
                                        break;

                                    case VG_MFTRCFG_VAR_ME_LOCATION:
                                        {
                                            result = vgGnMFtrCfgVarMeLocation( value, &modification);
                                        }
                                        break;

                                    case VG_MFTRCFG_VAR_SM_HANDLE_CONCAT_SMS:
                                        {
                                            result = vgGnMFtrCfgVarHandleConcatSms( value, &modification);
                                        }
                                        break;

                                    case VG_MFTRCFG_VAR_LM_CALL_INFO_STORAGE:
                                        {
                                            result = vgGnMFtrCfgVarCallInfoStorage( value, &modification);
                                        }
                                        break;

                                    default:
                                        {
                                            result = VG_CME_INVALID_INPUT_VALUE;
                                        }
                                        break;
                                }

                                if( result == RESULT_CODE_OK && modification)
                                {
                                    /* Send the request to write the new configuration*/
                                    result = vgChManContinueAction( entity,
                                                                    SIG_APEXGL_WRITE_FEATURE_CONFIG_REQ);
                                }
                            }
                        }
                        else if( mode == VG_MFTRCFG_MODE_READ)
                        {
                            result = vgGnMFtrCfgPrintVariableValue( entity, variable);
                        }
                        else
                        {
                            result = VG_CME_INVALID_INPUT_VALUE;
                        }
                    }
                }
                break;

            case EXTENDED_QUERY:  /* AT*MFTRCFG? */
                {
                    vgGnMFtrCfgPrintAllVariablesValue( entity);
                }
                break;

            case EXTENDED_ACTION: /* AT*MFTRCFG... */
            default:
                {
                    result = RESULT_CODE_ERROR;
                }
                break;
        }
    }
    else
    {
        /* We need to have received the current feature configuration*/
        result = RESULT_CODE_ERROR;
    }

    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgGnMFtrCfgVarCallInfoStorage
 *
 * Parameters:      newValue        - New value to give
 *                  modification_p  - Set to TRUE is the value has been modified
 *
 * Returns:         AT result code.
 *
 * Description:     Valids and sets the new value for the "AblmCallInfoStorage"
 *                  configuration variable
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t vgGnMFtrCfgVarCallInfoStorage(  Int32    newValue,
                                                    Boolean *modification_p)
{
    GeneralGenericContext_t    *generalGenericContext_p  = ptrToGeneralGenericContext();
    AbglFeatureConfigDataArea  *nvramCfgVar;
    AbglFeatureConfigDataArea  *currentCfgVar;
    VgMFtrCfgRebootInfo        *rebootInfo;
    ResultCode_t                result = RESULT_CODE_OK;

    nvramCfgVar     = &generalGenericContext_p->vgMFtrCfgData.nvramCfgVar;
    currentCfgVar   = &generalGenericContext_p->vgMFtrCfgData.currentCfgVar;
    rebootInfo      = &generalGenericContext_p->vgMFtrCfgData.rebootInfo;

    *modification_p = FALSE;
    if(newValue <= VG_MFTRCFG_MAX_BOOLEAN_VALUE)
    {
        if(nvramCfgVar->callInfoStorageMode != (AbglFeatureCallInfoStorage)newValue)
        {
            *modification_p = TRUE;
            nvramCfgVar->callInfoStorageMode = (AbglFeatureCallInfoStorage)newValue;
            /* Determine if a T1 reboot is needed*/
            if( nvramCfgVar->callInfoStorageMode ==  currentCfgVar->callInfoStorageMode)
            {
                rebootInfo->callInfoStorageModeNR = FALSE;
            }
            else
            {
                rebootInfo->callInfoStorageModeNR = TRUE;
            }
        }
    }
    else
    {
        result = VG_CME_INVALID_INPUT_VALUE;
    }

    return result;
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgGnMFtrCfgVarHandleConcatSms
 *
 * Parameters:      newValue        - New value to give
 *                  modification_p  - Set to TRUE is the value has been modified
 *
 * Returns:         AT result code.
 *
 * Description:     Valids and sets the new value for the "smHandleConcatSms"
 *                  configuration variable
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t vgGnMFtrCfgVarHandleConcatSms(  Int32    newValue,
                                                    Boolean *modification_p)
{
    GeneralGenericContext_t    *generalGenericContext_p  = ptrToGeneralGenericContext();
    AbglFeatureConfigDataArea  *nvramCfgVar;
    AbglFeatureConfigDataArea  *currentCfgVar;
    VgMFtrCfgRebootInfo        *rebootInfo;
    ResultCode_t                result = RESULT_CODE_OK;

    nvramCfgVar     = &generalGenericContext_p->vgMFtrCfgData.nvramCfgVar;
    currentCfgVar   = &generalGenericContext_p->vgMFtrCfgData.currentCfgVar;
    rebootInfo      = &generalGenericContext_p->vgMFtrCfgData.rebootInfo;

    *modification_p = FALSE;
    if(newValue <= VG_MFTRCFG_MAX_BOOLEAN_VALUE)
    {
        if(nvramCfgVar->smHandleConcatSms != (Boolean)newValue)
        {
            *modification_p = TRUE;
            nvramCfgVar->smHandleConcatSms = (Boolean)newValue;
            cfRvSmHandleConcatSms = (Boolean)newValue;
            /* Determine if a T1 reboot is needed*/
            if( nvramCfgVar->smHandleConcatSms == currentCfgVar->smHandleConcatSms)
            {
                rebootInfo->smHandleConcatSmsNR = FALSE;
            }
            else
            {
                rebootInfo->smHandleConcatSmsNR = TRUE;
            }
        }
    }
    else
    {
        result = VG_CME_INVALID_INPUT_VALUE;
    }

    return result;
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgGnMFtrCfgVarMeLocation
 *
 * Parameters:      newValue        - New value to give
 *                  modification_p  - Set to TRUE is the value has been modified
 *
 * Returns:         AT result code.
 *
 * Description:     Valids and sets the new value for the "MeLocation"
 *                  configuration variable
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t vgGnMFtrCfgVarMeLocation(   Int32    newValue,
                                                Boolean *modification_p)
{
    GeneralGenericContext_t    *generalGenericContext_p  = ptrToGeneralGenericContext();
    AbglFeatureConfigDataArea  *nvramCfgVar;
    AbglFeatureConfigDataArea  *currentCfgVar;
    VgMFtrCfgRebootInfo        *rebootInfo;
    ResultCode_t                result = RESULT_CODE_OK;

    nvramCfgVar     = &generalGenericContext_p->vgMFtrCfgData.nvramCfgVar;
    currentCfgVar   = &generalGenericContext_p->vgMFtrCfgData.currentCfgVar;
    rebootInfo      = &generalGenericContext_p->vgMFtrCfgData.rebootInfo;

    *modification_p = FALSE;
    if (newValue <= (Int32)GL_FEATURE_ME_LOCATION_NVRAM)
    {
        if(nvramCfgVar->meLocation != (AbglFeatureMeLocation)newValue)
        {
            *modification_p = TRUE;
            nvramCfgVar->meLocation = (AbglFeatureMeLocation)newValue;
            /* Determine if a T1 reboot is needed*/
            if( nvramCfgVar->meLocation == currentCfgVar->meLocation)
            {
                rebootInfo->meLocationNR = FALSE;
            }
            else
            {
                rebootInfo->meLocationNR = TRUE;
            }
        }
    }
    else
    {
        result = VG_CME_INVALID_INPUT_VALUE;
    }

    return result;
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgGnMFtrCfgVarModemMode
 *
 * Parameters:      newValue        - New value to give
 *                  modification_p  - Set to TRUE is the value has been modified
 *
 * Returns:         AT result code.
 *
 * Description:     Valids and sets the new value for the "ModemMode"
 *                  configuration variable
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t vgGnMFtrCfgVarModemMode(    Int32    newValue,
                                                Boolean *modification_p)
{
    GeneralGenericContext_t    *generalGenericContext_p  = ptrToGeneralGenericContext();
    AbglFeatureConfigDataArea  *nvramCfgVar;
    AbglFeatureConfigDataArea  *currentCfgVar;
    VgMFtrCfgRebootInfo        *rebootInfo;
    ResultCode_t                result = RESULT_CODE_OK;

    nvramCfgVar     = &generalGenericContext_p->vgMFtrCfgData.nvramCfgVar;
    currentCfgVar   = &generalGenericContext_p->vgMFtrCfgData.currentCfgVar;
    rebootInfo      = &generalGenericContext_p->vgMFtrCfgData.rebootInfo;

    *modification_p = FALSE;
    /* Cannot set standalone mode if ME in MMI*/
    if (newValue <= (Int32)GL_FEATURE_MODEM_MODE_MMI)
    {
        if( nvramCfgVar->modemMode != (AbglFeatureModemMode)newValue)
        {
            *modification_p = TRUE;
            nvramCfgVar->modemMode = (AbglFeatureModemMode)newValue;
            /* Determine if a T1 reboot is needed*/
            if( nvramCfgVar->modemMode == currentCfgVar->modemMode)
            {
                rebootInfo->modemModeNR = FALSE;
            }
            else
            {
                rebootInfo->modemModeNR = TRUE;
            }
        }
    }
    else
    {
        result = VG_CME_INVALID_INPUT_VALUE;
    }

    return result;
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgGnMFtrCfgPrintAllVariablesValue
 *
 * Parameters:      entity  - mux channel number
 *
 * Returns:         Nothing
 *
 * Description:     Print the current NVRAM value for all the configuration
 *                  variables.
 *
 *-------------------------------------------------------------------------*/
static void vgGnMFtrCfgPrintAllVariablesValue( const VgmuxChannelNumber entity)
{
    GeneralGenericContext_t    *generalGenericContext_p  = ptrToGeneralGenericContext();
    AbglFeatureConfigDataArea  *nvramCfgVar;
    VgMFtrCfgRebootInfo        *rebootInfo;

    nvramCfgVar     = &generalGenericContext_p->vgMFtrCfgData.nvramCfgVar;
    rebootInfo      = &generalGenericContext_p->vgMFtrCfgData.rebootInfo;

    /* Print value for each variable*/
    vgPutNewLine(entity);
    vgPrintf(   entity,
                (const Char*)"%C: %d,%d,%d",
                (Int32)VG_MFTRCFG_VAR_MODEM_MODE,
                (Int32)nvramCfgVar->modemMode,
                (Int32)rebootInfo->modemModeNR);
    vgPutNewLine(entity);
    vgPrintf(   entity,
                (const Char*)"%C: %d,%d,%d",
                (Int32)VG_MFTRCFG_VAR_ME_LOCATION,
                (Int32)nvramCfgVar->meLocation,
                (Int32)rebootInfo->meLocationNR);
    vgPutNewLine(entity);
    vgPrintf(   entity,
                (const Char*)"%C: %d,%d,%d",
                (Int32)VG_MFTRCFG_VAR_SM_HANDLE_CONCAT_SMS,
                (Int32)nvramCfgVar->smHandleConcatSms,
                (Int32)rebootInfo->smHandleConcatSmsNR);
    vgPutNewLine(entity);
    vgPrintf(   entity,
                (const Char*)"%C: %d,%d,%d",
                (Int32)VG_MFTRCFG_VAR_LM_CALL_INFO_STORAGE,
                (Int32)nvramCfgVar->callInfoStorageMode,
                (Int32)rebootInfo->callInfoStorageModeNR);
    vgPutNewLine(entity);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgGnMFtrCfgPrintVariableValue
 *
 * Parameters:      entity      - mux channel number
 *                  variable    - Variable to print
 *
 * Returns:         AT result code.
 *
 * Description:     Print the current NVRAM value for a given variable.
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t vgGnMFtrCfgPrintVariableValue( const VgmuxChannelNumber entity, Int32 variable)
{
    GeneralGenericContext_t    *generalGenericContext_p  = ptrToGeneralGenericContext();
    AbglFeatureConfigDataArea  *nvramCfgVar;
    VgMFtrCfgRebootInfo        *rebootInfo;
    Int32                       value = 0;
    Boolean                     needReboot = FALSE;
    ResultCode_t                result = RESULT_CODE_OK;

    nvramCfgVar     = &generalGenericContext_p->vgMFtrCfgData.nvramCfgVar;
    rebootInfo      = &generalGenericContext_p->vgMFtrCfgData.rebootInfo;

    switch (variable)
    {
        case VG_MFTRCFG_VAR_MODEM_MODE:
            {
                value = (Int32)nvramCfgVar->modemMode;
                needReboot = rebootInfo->modemModeNR;
            }
            break;

        case VG_MFTRCFG_VAR_ME_LOCATION:
            {
                value = (Int32)nvramCfgVar->meLocation;
                needReboot = rebootInfo->meLocationNR;
            }
            break;

        case VG_MFTRCFG_VAR_SM_HANDLE_CONCAT_SMS:
            {
                value = (Int32)nvramCfgVar->smHandleConcatSms;
                needReboot = rebootInfo->smHandleConcatSmsNR;
            }
            break;

        case VG_MFTRCFG_VAR_LM_CALL_INFO_STORAGE:
            {
                value = (Int32)nvramCfgVar->callInfoStorageMode;
                needReboot = rebootInfo->callInfoStorageModeNR;
            }
            break;

        default:
            {
                result = VG_CME_INVALID_INPUT_VALUE;
            }
            break;
    }

    if( result == RESULT_CODE_OK)
    {
        vgPutNewLine(entity);
        vgPrintf(   entity,
                    (const Char*)"%C: %d,%d,%d",
                    variable,
                    value,
                    (Int32)needReboot);
        vgPutNewLine(entity);
    }

    return result;
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgGnRouteMMI
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     Non-standard AT command to inform the modem that an AT
 *                  channel (where this command is sent) is hooked to MMI use.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgGnRouteMMI(CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ExtendedOperation_t     operation = getExtendedOperation (commandBuffer_p);
    ResultCode_t            result = RESULT_CODE_OK;
    Int32                   mode;
    Int32                   option = 0;
    Int32                   value;
    Int8                    i;
    GeneralContext_t        *generalContext_p = ptrToGeneralContext(entity);
    SmsContext_t            *smsContext_p  = ptrToSmsContext(entity);
    Boolean                 optionPresent = FALSE;

    FatalCheck( entity<CI_MAX_ENTITIES, entity, CI_MAX_ENTITIES, 0);

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);
    FatalCheck(smsContext_p != PNULL, entity, 0, 0);

    switch (operation)
    {

        case EXTENDED_RANGE:  /* AT*MROUTEMMI=? */
            {
                vgPutNewLine(entity);
                vgPrintf(entity, (const Char*)"%C: (1-2),(0-1),(0-1)");
                vgPutNewLine(entity);
            }
            break;

        case EXTENDED_ASSIGN: /* AT*MROUTEMMI=... */
            {
                /* Is it a read or a write ?*/
                if ( getExtendedParameter( commandBuffer_p, &mode, ULONG_MAX) != TRUE )
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else if( mode > VG_MROUTEMMI_MODE_WRITE )
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }

                if( result == RESULT_CODE_OK)
                {
                    /* which option to configure ?*/
                    if ( getExtendedParameterPresent(   commandBuffer_p,
                                                        &option,
                                                        0,
                                                        &optionPresent) != TRUE)
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
                    }
                    else if( option >= NUM_OF_MROUTEMMI_BITS)
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
                    }
                }

                if( result == RESULT_CODE_OK)
                {
                    switch( mode)
                    {
                        case VG_MROUTEMMI_MODE_WRITE:
                            {
                                /* Write the new value for option*/
                                if ( getExtendedParameter( commandBuffer_p, &value, ULONG_MAX) != TRUE )
                                {
                                    result = VG_CME_INVALID_INPUT_VALUE;
                                }
                                else if( value > VG_MROUTEMMI_MAX_OPTION_VALUE )
                                {
                                    result = VG_CME_INVALID_INPUT_VALUE;
                                }
                                else /* Everything is OK*/
                                {
                                    /* Check that no attempt is made to designate more than one channel as MMI Unsolicited channel */
                                    if( ((!optionPresent)||(option == PROF_BIT_MROUTEMMI_MMI_UNSOL))&&
                                        (value == 1) )
                                    {
                                      VgmuxChannelNumber unsolMmiEntity = vgGetMmiUnsolicitedChannel();
                                      if((unsolMmiEntity != entity)&&(unsolMmiEntity != VGMUX_CHANNEL_INVALID))
                                      {
                                        result = VG_CME_OPERATION_NOT_ALLOWED;
                                      }
                                    }


                                    if( result == RESULT_CODE_OK)
                                    {

                                      if ((optionPresent) && (option < NUM_OF_MROUTEMMI_BITS))
                                      {



                                          /* Set the value for option*/
                                          setProfileValueBit( entity,
                                                              PROF_MROUTEMMI,
                                                              MRouteMmiProfiles[option],
                                                              (Int8)value);

                                      }
                                      else
                                      {
                                        /* Set value for all the options*/
                                        for(    i=0; i < NUM_OF_MROUTEMMI_BITS; i++)
                                        {
                                            setProfileValueBit( entity,
                                                                PROF_MROUTEMMI,
                                                                MRouteMmiProfiles[i],
                                                                (Int8)value);
                                        }
                                      }
                                    }
                                }
                            }
                            break;

                        case VG_MROUTEMMI_MODE_READ:
                            {
                                if( optionPresent)
                                {
                                    /* Only print the desired option*/
                                    vgGnRouteMMIPrintOption( (Int8)option, entity);
                                }
                                else
                                {
                                    /* Print all options*/
                                    for(    i=0; i < NUM_OF_MROUTEMMI_BITS; i++)
                                    {
                                        vgGnRouteMMIPrintOption( i, entity);
                                    }
                                }
                            }
                            break;

                        default:
                            {
                                 result = VG_CME_INVALID_INPUT_VALUE; 
                            }
                            break;
                    }
                }
            }
            break;

        case EXTENDED_QUERY:  /* AT*MROUTEMMI? */
            {
                /* Print all options*/
                for(    i=0; i < NUM_OF_MROUTEMMI_BITS; i++)
                {
                    vgGnRouteMMIPrintOption( i, entity);
                }
            }
            break;

        case EXTENDED_ACTION: /* AT*MROUTEMMI... */
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
 * Function:    vgGnRouteMMIPrintOption
 *
 * Parameters:  (in) option - Option to print
 *              (in) entity - channel entity
 *
 * Returns:     Nothing
 *
 * Description: Print the list of channels for which the option is set
 *-------------------------------------------------------------------------*/
static void vgGnRouteMMIPrintOption(    Int8 option,
                                        const VgmuxChannelNumber entity)
{
    VgmuxChannelNumber      channel;
    Boolean                 comma;

    FatalCheck( option < NUM_OF_MROUTEMMI_BITS, option, 0, 0);

    vgPutNewLine (entity);
    vgPrintf(   entity,
                (const Char*)"%C: %d,%s,(",
                option,
                getProfileValueBit( entity, PROF_MROUTEMMI, MRouteMmiProfiles[option]) ? "1" : "0" );
    for (   comma = FALSE, channel=0;
            channel<CI_MAX_ENTITIES;
            channel++)
    {
        if( (isEntityActive(channel)) &&
            (getProfileValueBit( channel, PROF_MROUTEMMI, MRouteMmiProfiles[option])))
        {
            if( comma)
            {
                vgPrintf( entity, (const Char*)",%d", channel+1);
            }
            else
            {
                vgPrintf( entity, (const Char*)"%d", channel+1);
                comma = TRUE;
            }
        }
    }
    vgPrintf(   entity, (const Char*)")");
}

/***************************************************************************
 * AT Commands for NB-IOT Project
 ***************************************************************************/
/*--------------------------------------------------------------------------
*
* Function:    vgGnMNVMQ
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MNVMQ which checks modem NVRAM status
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMNVMQ  (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  VgMnvmDataContext       *vgNvmDataContext_p = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMnvmDataContext);
  Boolean                 *vgNvmAccessAuthorised_p = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised);
  ResultCode_t             result = RESULT_CODE_OK;

#if defined (MTK_NVDM_MODEM_ENABLE)
  ExtendedOperation_t      operation = getExtendedOperation (commandBuffer_p);    
  Int16                    str_length;
#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)
  tool_auth_response_t     auth_response = TOOL_AUTH_REQUIRED;
  VgMnvmqAuthStatus        mnvmqAuthStatus;

#endif

  memset((Int8*)(vgNvmDataContext_p), 0 , sizeof (VgMnvmDataContext));
  
  switch (operation)
  {
    case EXTENDED_ASSIGN:      /* AT*MNVMQ= */
    {
      /* Get Tool Version  */
      if (getExtendedString ( commandBuffer_p,
          vgNvmDataContext_p->tool_version,
          MAX_NVRAM_TOOL_VERSION_LEN,
          &str_length) == TRUE)
      {
          if (str_length == 0)
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
        /* If access already authorised - then just issue immediate response
         * that authorisation is not required
         */
        if (*vgNvmAccessAuthorised_p)
        {
          vgPutNewLine (entity);
          vgPrintf (entity,
                   (const Char *)"*MNVMQ: \"%s\",%d",MNVMQ_CHIP_NAME, MNVMQ_NO_AUTH_REQ);
          vgPutNewLine (entity);        
        }
#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)         
        else if(tool_auth_is_required(&auth_response) == TOOL_AUTH_STATUS_OK)
        {
          switch(auth_response)
          {
            case TOOL_AUTH_NOT_REQUIRED:
            {
              mnvmqAuthStatus = MNVMQ_NO_AUTH_REQ;
              *vgNvmAccessAuthorised_p = TRUE;
              break;
            }
            case TOOL_AUTH_REQUIRED:
            {
              mnvmqAuthStatus = MNVMQ_AUTH_REQ;
              *vgNvmAccessAuthorised_p = FALSE;
              break;
            }
            default:
            {
              FatalFail("ATCI NVDM: Illegal auth response");
              break;  
            }
          }

          vgPutNewLine (entity);
          vgPrintf (entity,
                   (const Char *)"*MNVMQ: \"%s\",%d",MNVMQ_CHIP_NAME, mnvmqAuthStatus);
          vgPutNewLine (entity);         
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
#else
        *vgNvmAccessAuthorised_p = TRUE;
#endif
      }
      break;
    }
    case EXTENDED_QUERY:       /* AT*MNVMQ? */
    case EXTENDED_RANGE:       /* AT*MNVMQ=? */
    {
      /* Do nothing */
      break;
    }

    case EXTENDED_ACTION:      /* AT*MNVMQ */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }    
  }
#endif /* MTK_NVDM_MODEM_ENABLE */
  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnMNVMAUTH
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MNVMAUTH which authenticates the
*              config tool.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMNVMAUTH  (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity)
{
  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  VgMnvmDataContext       *vgNvmDataContext_p = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMnvmDataContext);
#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)         
  Boolean                 *vgNvmAccessAuthorised_p = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised);
#endif
  ResultCode_t             result = RESULT_CODE_OK;

#if defined (MTK_NVDM_MODEM_ENABLE)
  ExtendedOperation_t      operation = getExtendedOperation (commandBuffer_p);    
  Int16                    str_length;
  Int32                    cert_data_length;

#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)         
  tool_auth_verify_result_t  verify_result = TOOL_AUTH_VERIFY_FAIL;
  VgMnvmauthResult           mnvmauthResult;
#endif

  memset((Int8*)(vgNvmDataContext_p), 0 , sizeof (VgMnvmDataContext));
  
  switch (operation)
  {
    case EXTENDED_ASSIGN:      /* AT*MNVMAUTH= */
    {

      /* Get certificate length */
      if(getExtendedParameter ( commandBuffer_p,
                                  &cert_data_length,
                                  ULONG_MAX) == TRUE)
      {
          if (cert_data_length != ULONG_MAX)
          {
              if ((cert_data_length == 0) || (cert_data_length > MAX_NVRAM_CERTIFICATE_SIZE_BYTES))
              {
                  result = VG_CME_INVALID_INPUT_VALUE;
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
     
      if (result == RESULT_CODE_OK)
      {
        /* Get certificate:  Note we are re-using the NVRAM data structure to
         * save memory
         */
        if (getExtendedString ( commandBuffer_p,
           vgNvmDataContext_p->data_str,
           MAX_NVRAM_CERTIFICATE_STR_LEN,
           &str_length) == TRUE)
        {
           /* Length cannot be 0 or not multiple of 2 */
           if ((str_length == 0) || (((str_length>>1)<<1) != str_length))
           {
               result = VG_CME_INVALID_INPUT_VALUE;
           }
           else
           {
               if (str_length != (cert_data_length*2))
               {
                 result = VG_CME_INVALID_INPUT_VALUE;
               }
               else
               {
                 /* Decode the hex string - convert to hex number */
                 (void)vgMapTEToHex (vgNvmDataContext_p->data,
                                cert_data_length,
                                 vgNvmDataContext_p->data_str,
                                  str_length,
                                   VG_AT_CSCS_HEX);
#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)         
                 if (tool_auth_verify_certificate((uint8_t *)(vgNvmDataContext_p->data),
                                                              (uint16_t)cert_data_length,
                                                              &verify_result) == TOOL_AUTH_STATUS_OK)
                 {
                   switch(verify_result)
                   {
                     case TOOL_AUTH_VERIFY_FAIL:
                     {
                       mnvmauthResult = MNVMAUTH_AUTH_FAILED;
                       *vgNvmAccessAuthorised_p = FALSE;
                       break;
                     }
                     case TOOL_AUTH_VERIFY_PASS:
                     {
                       mnvmauthResult = MNVMAUTH_AUTH_PASSED;
                       *vgNvmAccessAuthorised_p = TRUE;
                       break;
                     }
                     default:
                     {
                       FatalFail("ATCI NVDM: Illegal auth response");
                       break;  
                     }
                   }
                   
                   vgPutNewLine (entity);
                   vgPrintf (entity,
                            (const Char *)"*MNVMAUTH: %d", mnvmauthResult);
                   vgPutNewLine (entity);
                 }
                 else
                 {
                   result = RESULT_CODE_ERROR;
                 }
#endif
               }
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
      break;
    }
    case EXTENDED_QUERY:       /* AT*MNVMAUTH? */
    case EXTENDED_RANGE:       /* AT*MNVMAUTH=? */
    {
      /* Do nothing */
      break;
    }

    case EXTENDED_ACTION:      /* AT*MNVMAUTH */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }    
  }
#endif /* MTK_NVDM_MODEM_ENABLE */
  return (result);
}
  
/*--------------------------------------------------------------------------
*
* Function:    vgGnMNVMW
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MNVMW which writes direct to NVRAM.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMNVMW  (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  VgMnvmDataContext       *vgNvmDataContext_p = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMnvmDataContext);
  Boolean                  vgNvmAccessAuthorised = (generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised);
  ResultCode_t             result = RESULT_CODE_OK;

#if defined (MTK_NVDM_MODEM_ENABLE)
  ExtendedOperation_t      operation = getExtendedOperation (commandBuffer_p);    
  Int16                    str_length;
  Int32                    param;
  VgNvmAreaInfo            areaInfo;
  VgNvmDataType            dataType;
  Int32                    length;
  nvdm_modem_status_t      status;
  VgNvmStatus              vgNvmStatus;
  Boolean                  protected_area = FALSE;
  nvdm_modem_attr_enum_t   attr = NVDM_MODEM_ATTR_AVERAGE;
  nvdm_modem_data_item_type_t   data_item_type = NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA;  

#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)
  tool_auth_response_t     auth_response = TOOL_AUTH_REQUIRED;
#endif

  memset((Int8*)(vgNvmDataContext_p), 0 , sizeof (VgMnvmDataContext));
  
  switch (operation)
  {
    case EXTENDED_ASSIGN:      /* AT*MNVMW= */
    {

      /* Get Area Info */
      if(getExtendedParameter ( commandBuffer_p,
                                  &param,
                                  ULONG_MAX) == TRUE)
      {
          if (param != ULONG_MAX)
          {
              if (param >= VG_NVM_NUM_AREAS)
              {
                  result = VG_CME_INVALID_INPUT_VALUE;
              }
              else
              {
                  areaInfo = (VgNvmAreaInfo)param;

                  if ((areaInfo == VG_NVM_PROTECTED_AREA) || (areaInfo == VG_NVM_PROTECTED_AREA_WITH_BACKUP))
                  {
                    /* Only allow access to protected area if authorisation occurred */
                    if (vgNvmAccessAuthorised)
                    {
                      protected_area = TRUE;
                    }
#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)
                    /* To check if needs tool authentication */
                    else if(tool_auth_is_required(&auth_response) == TOOL_AUTH_STATUS_OK)
                    {
                        switch(auth_response)
                        {
                            case TOOL_AUTH_NOT_REQUIRED:
                            {
                              generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised = TRUE;
                              protected_area = TRUE;
                              break;
                            }
                            case TOOL_AUTH_REQUIRED:
                            {
                              result = VG_CME_OPERATION_NOT_ALLOWED;
                              break;
                            }
                            default:
                            {
                              FatalFail("ATCI NVDM: Illegal auth response");
                              break;  
                            }
                        }
                     }
#endif
                    else
                    {
                      result = VG_CME_OPERATION_NOT_ALLOWED;
                    }
                  }
                  
                  if ((areaInfo == VG_NVM_NORMAL_AREA_WITH_BACKUP) || (areaInfo == VG_NVM_PROTECTED_AREA_WITH_BACKUP))
                  {
                    attr = NVDM_MODEM_ATTR_BACKUP;
                  }
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

      if (result == RESULT_CODE_OK)
      {
        /* Get group ID */
        if (getExtendedString ( commandBuffer_p,
            vgNvmDataContext_p->group_id,
            MAX_NVRAM_GROUP_ID_LEN,
            &str_length) == TRUE)
        {
            if (str_length == 0)
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
        /* Get data item ID */
        if (getExtendedString ( commandBuffer_p,
            vgNvmDataContext_p->data_item_id,
            MAX_NVRAM_DATA_ITEM_ID_LEN,
            &str_length) == TRUE)
        {
            if (str_length == 0)
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
        /* Get data type */
        if(getExtendedParameter ( commandBuffer_p,
                                    &param,
                                    ULONG_MAX) == TRUE)
        {
            if (param != ULONG_MAX)
            {
                if (param >= VG_NVM_NUM_DATA_TYPES)
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                    dataType = (VgNvmDataType)param;

                    if (dataType == VG_NVM_DATA_TYPE_STRING)
                    {
                      data_item_type = NVDM_MODEM_DATA_ITEM_TYPE_STRING;
                    }
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

      if (result == RESULT_CODE_OK)
      {
        /* Get length */
        if(getExtendedParameter ( commandBuffer_p,
                                    &length,
                                    ULONG_MAX) == TRUE)
        {
            if (length != ULONG_MAX)
            {
                if ((length == 0) || (length > MAX_NVRAM_DATA_ITEM_SIZE_BYTES))
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
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
       
      if (result == RESULT_CODE_OK)
      {
        /* Get data item */
        if (getExtendedString ( commandBuffer_p,
           vgNvmDataContext_p->data_str,
           MAX_NVRAM_DATA_ITEM_STR_LEN,
           &str_length) == TRUE)
        {
           /* Length cannot be 0 or not multiple of 2 */
           if ((str_length == 0) || (((str_length>>1)<<1) != str_length))
           {
               result = VG_CME_INVALID_INPUT_VALUE;
           }
           else
           {
               if (str_length != (length*2))
               {
                 result = VG_CME_INVALID_INPUT_VALUE;
               }
               else
               {
                 /* Decode the hex string - convert to hex number */
                 (void)vgMapTEToHex (vgNvmDataContext_p->data,
                                length,
                                 vgNvmDataContext_p->data_str,
                                  str_length,
                                   VG_AT_CSCS_HEX);
                 
                 /* Now send request to NVDM */
                 
                 if (protected_area)
                 {
                   status = nvdm_modem_write_protected_data_item((const char *)vgNvmDataContext_p->group_id,
                                                 (const char *)vgNvmDataContext_p->data_item_id,
                                                 data_item_type,
                                                 (const uint8_t *)vgNvmDataContext_p->data,
                                                 length,
                                                 attr);
                 }
                 else
                 {
                   status = nvdm_modem_write_normal_data_item((const char *)vgNvmDataContext_p->group_id,
                                                 (const char *)vgNvmDataContext_p->data_item_id,
                                                 data_item_type,
                                                 (const uint8_t *)vgNvmDataContext_p->data,
                                                 length,
                                                 attr);
                 }
                 vgNvmStatus = vgConvertNvramStatus (entity, status);
                 
                 vgPutNewLine (entity);
                 vgPrintf (entity,
                          (const Char *)"*MNVMW: %d",vgNvmStatus);
                 vgPutNewLine (entity);
               }
           }
        }
        else
        {
           result = VG_CME_INVALID_INPUT_VALUE;
        }
      }
      break;
    }  
    case EXTENDED_QUERY:       /* AT*MNVMW? */
    case EXTENDED_RANGE:       /* AT*MNVMW=? */
    {
      /* Do nothing */
      break;
    }

    case EXTENDED_ACTION:      /* AT*MNVMW */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
#endif /* MTK_NVDM_MODEM_ENABLE */
  return (result);
  
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnMNVMOTPW
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MNVMOTPW which writes direct to NVRAM.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMNVMOTPW  (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  VgMnvmDataContext       *vgNvmDataContext_p = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMnvmDataContext);
  ResultCode_t             result = RESULT_CODE_OK;

#if defined (MTK_NVDM_MODEM_ENABLE)&& defined (MTK_NBIOT_TARGET_BUILD)
  ExtendedOperation_t      operation = getExtendedOperation (commandBuffer_p);    
  Int16                    str_length;
  Int32                    length;
  nvdm_modem_status_t      status;
  VgNvmStatus              vgNvmStatus;
  nvdm_modem_data_item_type_t   data_item_type = NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA;  

  memset((Int8*)(vgNvmDataContext_p), 0 , sizeof (VgMnvmDataContext));
  
  switch (operation)
  {
    case EXTENDED_ASSIGN:      /* AT*MNVMOTPW= */
    {            
      if (result == RESULT_CODE_OK)
      {
        /* Get data item ID */
        if (getExtendedString ( commandBuffer_p,
            vgNvmDataContext_p->data_item_id,
            MAX_NVRAM_OTP_DATA_ITEM_ID_LEN,
            &str_length) == TRUE)
        {
            if (str_length == 0 ||str_length >7)
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
        /* Get length */
        if(getExtendedParameter ( commandBuffer_p,
                                    &length,
                                    ULONG_MAX) == TRUE)
        {
            if (length != ULONG_MAX)
            {
                if ((length == 0) || (length > MAX_NVRAM_DATA_ITEM_SIZE_BYTES))
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
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
       
      if (result == RESULT_CODE_OK)
      {
        /* Get data item */
        if (getExtendedString ( commandBuffer_p,
           vgNvmDataContext_p->data_str,
           MAX_NVRAM_DATA_ITEM_STR_LEN,
           &str_length) == TRUE)
        {
           /* Length cannot be 0 or not multiple of 2 */
           if ((str_length == 0) || (((str_length>>1)<<1) != str_length))
           {
               result = VG_CME_INVALID_INPUT_VALUE;
           }
           else
           {
               if (str_length != (length*2))
               {
                 result = VG_CME_INVALID_INPUT_VALUE;
               }
               else
               {
                 /* Decode the hex string - convert to hex number */
                 (void)vgMapTEToHex (vgNvmDataContext_p->data,
                                length,
                                 vgNvmDataContext_p->data_str,
                                  str_length,
                                   VG_AT_CSCS_HEX);
                 
                 /* Now send request to NVDM */
                 
                   status = nvdm_modem_write_otp_data_item((const char *)vgNvmDataContext_p->data_item_id,                                              
                                                 (const uint8_t *)vgNvmDataContext_p->data,
                                                 length);                              
                 vgNvmStatus = vgConvertNvramStatus (entity, status);                 
                 vgPutNewLine (entity);
                 vgPrintf (entity,
                          (const Char *)"*MNVMOTPW: %d",vgNvmStatus);
                 vgPutNewLine (entity);
               }
           }
        }
        else
        {
           result = VG_CME_INVALID_INPUT_VALUE;
        }
      }
      break;
    }  
    case EXTENDED_QUERY:       /* AT*MNVMW? */
    case EXTENDED_RANGE:       /* AT*MNVMW=? */
    {
      /* Do nothing */
      break;
    }

    case EXTENDED_ACTION:      /* AT*MNVMW */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
#endif /* MTK_NVDM_MODEM_ENABLE */
  return (result);
  
}
/*--------------------------------------------------------------------------
*
* Function:    vgGnMNVMR
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MNVMR which reads direct from NVRAM.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMNVMR  (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  VgMnvmDataContext       *vgNvmDataContext_p = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMnvmDataContext);
  Boolean                  vgNvmAccessAuthorised = (generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised);
  ResultCode_t             result = RESULT_CODE_OK;

#if defined (MTK_NVDM_MODEM_ENABLE)
  ExtendedOperation_t      operation = getExtendedOperation (commandBuffer_p);
  Int32                    param;
  VgNvmAreaInfo            areaInfo;
  VgNvmDataType            dataType = VG_NVM_DATA_TYPE_RAW;
  Int16                    str_length;
  Int32                    buff_length, actual_data_length;
  nvdm_modem_status_t      status;
  VgNvmStatus              vgNvmStatus;
  Boolean                  protected_area = FALSE;
  nvdm_modem_area_t        area;
  nvdm_modem_data_item_type_t   data_item_type;  
#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)
  tool_auth_response_t     auth_response = TOOL_AUTH_REQUIRED;
#endif

  memset((Int8*)(vgNvmDataContext_p), 0 , sizeof (VgMnvmDataContext));

  switch (operation)
  {
    case EXTENDED_ASSIGN:      /* AT*MNVMR= */
    {
      /* Get Area Info */
      if(getExtendedParameter ( commandBuffer_p,
                                  &param,
                                  ULONG_MAX) == TRUE)
      {
          if (param != ULONG_MAX)
          {
              if (param >= VG_NVM_NUM_AREAS)
              {
                  result = VG_CME_INVALID_INPUT_VALUE;
              }
              else
              {
                  areaInfo = (VgNvmAreaInfo)param;

                  if ((areaInfo == VG_NVM_PROTECTED_AREA) || (areaInfo == VG_NVM_PROTECTED_AREA_WITH_BACKUP))
                  {
                    /* Only allow access to protected area if authorisation occurred */
                    if (vgNvmAccessAuthorised)
                    {
                      protected_area = TRUE;
                    }
#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)
                    /* To check if needs tool authentication */
                    else if(tool_auth_is_required(&auth_response) == TOOL_AUTH_STATUS_OK)
                    {
                        switch(auth_response)
                        {
                            case TOOL_AUTH_NOT_REQUIRED:
                            {
                              generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised = TRUE;
                              protected_area = TRUE;
                              break;
                            }
                            case TOOL_AUTH_REQUIRED:
                            {
                              result = VG_CME_OPERATION_NOT_ALLOWED;
                              break;
                            }
                            default:
                            {
                              FatalFail("ATCI NVDM: Illegal auth response");
                              break;  
                            }
                        }
                     }
#endif
                    else
                    {
                      result = VG_CME_OPERATION_NOT_ALLOWED;
                    }
                  }  
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

      if (result == RESULT_CODE_OK)
      {
        /* Get group ID */
        if (getExtendedString ( commandBuffer_p,
            vgNvmDataContext_p->group_id,
            MAX_NVRAM_GROUP_ID_LEN,
            &str_length) == TRUE)
        {
            if (str_length == 0)
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
        /* Get data item ID */
        if (getExtendedString ( commandBuffer_p,
            vgNvmDataContext_p->data_item_id,
            MAX_NVRAM_DATA_ITEM_ID_LEN,
            &str_length) == TRUE)
        {
            if (str_length == 0)
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
        /* Get length */
        if(getExtendedParameter ( commandBuffer_p,
                                    &buff_length,
                                    ULONG_MAX) == TRUE)
        {
            if (buff_length != ULONG_MAX)
            {
                if ((buff_length == 0) || (buff_length > MAX_NVRAM_DATA_ITEM_SIZE_BYTES))
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
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
       
      if (result == RESULT_CODE_OK)
      {
        actual_data_length = buff_length;
        
        /* Now read data from NVRAM
         */

        if (protected_area)
        {
          status = nvdm_modem_read_protected_data_item((const char *)vgNvmDataContext_p->group_id,
                                        (const char *)vgNvmDataContext_p->data_item_id,
                                        &data_item_type,
                                        (uint8_t *)vgNvmDataContext_p->data,
                                        &actual_data_length);
        }
        else
        {
          status = nvdm_modem_read_normal_data_item((const char *)vgNvmDataContext_p->group_id,
                                        (const char *)vgNvmDataContext_p->data_item_id,
                                        &data_item_type,
                                        (uint8_t *)vgNvmDataContext_p->data,
                                        &actual_data_length);
        }

        if ((actual_data_length == 0) || (actual_data_length > buff_length))
        {
          /* User specified too small buffer length value */
          result = VG_CME_INVALID_INPUT_VALUE;
        }

        if (result == RESULT_CODE_OK)
        {
          vgNvmStatus = vgConvertNvramStatus (entity, status);

          /* Get the information about whether data is backed up or not */
          if (vgNvmStatus == VG_NVM_STATUS_OK)
          {
            /* Convert data type we got */
            if (data_item_type == NVDM_MODEM_DATA_ITEM_TYPE_STRING)
            {
              dataType = VG_NVM_DATA_TYPE_STRING;
            }
          
            status = nvdm_modem_query_data_item_area((const char *)vgNvmDataContext_p->group_id,
                                                     (const char *)vgNvmDataContext_p->data_item_id,
                                                     &area);
            /* Status here should always be OK because the read was OK */
            FatalCheck(status == NVDM_MODEM_STATUS_OK, entity, status, area);
            FatalCheck(area != NVDM_MODEM_AREA_NONE, entity, status, area);

            switch (area)
            {
              case (NVDM_MODEM_AREA_NORMAL):
              {
                areaInfo = VG_NVM_NORMAL_AREA;
                break;
              }
              case (NVDM_MODEM_AREA_PROTECTED):
              {
                areaInfo = VG_NVM_PROTECTED_AREA;
                break;
              }
              case (NVDM_MODEM_AREA_NORMAL | NVDM_MODEM_AREA_BACKUP):
              {
                areaInfo = VG_NVM_NORMAL_AREA_WITH_BACKUP;
                break;
              }
              case (NVDM_MODEM_AREA_PROTECTED | NVDM_MODEM_AREA_BACKUP):
              {
                areaInfo = VG_NVM_PROTECTED_AREA_WITH_BACKUP;
                break;
              }
              default:
              {
                FatalParam(entity, status, area);
                break;
              }
            }
          }

          if (vgNvmStatus == VG_NVM_STATUS_OK)
          {
            /* Convert to hex string
             *
             */
            str_length = vgMapHexToTE(vgNvmDataContext_p->data_str,
                              MAX_NVRAM_DATA_ITEM_STR_LEN,
                              vgNvmDataContext_p->data,
                              actual_data_length,
                              VG_AT_CSCS_HEX);

            /* Display the text now */
            vgPutNewLine (entity);
            vgPrintf (entity,
                     (const Char *)"*MNVMR: ");

            vgPrintf(entity, (const Char *)"%d,%d,\"%s\",\"%s\",%d,%d,\"",
                    vgNvmStatus,
                    areaInfo,
                    vgNvmDataContext_p->group_id,
                    vgNvmDataContext_p->data_item_id,
                    dataType,
                    actual_data_length);

            vgNvmDataContext_p->data_str[str_length] = '\"';
            vgNvmDataContext_p->data_str[str_length+1] = 0;
            vgPuts(entity,(const Char *)vgNvmDataContext_p->data_str);
            vgFlushBuffer(entity);
          }
          else
          {
            vgPutNewLine (entity);
            vgPrintf (entity,
                     (const Char *)"*MNVMR: %d",vgNvmStatus);
            vgPutNewLine (entity);
          }
        }
      }
      break;
    }  
    
    case EXTENDED_QUERY:       /* AT*MNVMR? */
    case EXTENDED_RANGE:       /* AT*MNVMR=? */
    {
      /* Do nothing */
      break;
    }

    case EXTENDED_ACTION:      /* AT*MNVMR */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
#endif /* MTK_NVDM_MODEM_ENABLE */

  return (result);  
}

Int32 handleSupportBand(Int32 param,Int32  support_band)
{
    switch(param)
    {
      case 1:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_1| support_band;
      case 2:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_2| support_band;
      case 3:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_3| support_band;
      case 4:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_4| support_band;
      case 5:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_5| support_band;
      case 8:  
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_8| support_band;
      case 11:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_11| support_band;
      case 12:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_12| support_band;
      case 13:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_13| support_band;
      case 17:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_17| support_band;
      case 18:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_18| support_band;
      case 19:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_19| support_band;
      case 20:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_20| support_band;
      case 25:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_25| support_band;
      case 26:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_26| support_band;
      case 28:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_28| support_band;
      case 31:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_31| support_band;
      case 66:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_66| support_band;
      case 70:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_70| support_band;
      case 71:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_71| support_band;
      case 85:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_85| support_band;
      case 21:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_21| support_band;
      case 14:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_14| support_band;      
      case 72:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_72| support_band;
      case 73:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_73| support_band;
      case 74:
        return MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_74| support_band;
      default: 
        return support_band;
    
    }

}

void readSupportBand(Int32  bandSupportBmp,Int8 *band_list)
{
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_1 & bandSupportBmp )
    {
        band_list[0]=1;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_2 & bandSupportBmp )
    {
        band_list[1]=2;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_3 & bandSupportBmp )
    {
        band_list[2]=3;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_4 & bandSupportBmp)
    {
        band_list[3]=4;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_5 & bandSupportBmp )
    {
        band_list[4]=5;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_8 & bandSupportBmp )
    {
        band_list[5]=8;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_11 & bandSupportBmp )
    {
        band_list[6]=11;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_12 & bandSupportBmp )
    {
        band_list[7]=12;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_13 & bandSupportBmp )
    {
        band_list[8]=13;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_17 & bandSupportBmp )
    {
        band_list[9]=17;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_18 & bandSupportBmp )
    {
        band_list[10]=18;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_19 & bandSupportBmp )
    {
        band_list[11]=19;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_20 & bandSupportBmp )
    {
        band_list[12]=20;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_21 & bandSupportBmp )
    {
        band_list[13]=21;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_25 & bandSupportBmp )
    {
        band_list[14]=25;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_26 & bandSupportBmp )
    {
        band_list[15]=26;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_28 & bandSupportBmp )
    {
        band_list[16]=28;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_31 & bandSupportBmp )
    {
        band_list[17]=31;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_66 & bandSupportBmp )
    {
        band_list[18]=66;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_70 & bandSupportBmp )
    {
        band_list[19]=70;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_71 & bandSupportBmp )
    {
        band_list[20]=71;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_85 & bandSupportBmp )
    {
        band_list[21]=85;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_14 & bandSupportBmp )
    {
        band_list[22]=14;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_72 & bandSupportBmp )
    {
        band_list[23]=72;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_73 & bandSupportBmp )
    {
        band_list[24]=73;
    }
    if( MODEM_CFG_BAND_SUPPORT_ERRC_BANDS_RF_BAND_74 & bandSupportBmp )
    {
        band_list[25]=74;
    }


}
#include "hal_rtc.h"

void n1_mqtt_create();
void n1_mqtt_connect();
void n1_mqtt_publish(char *data);
void n1_mqtt_subscribe();
void sbit_m2m_ct_send_massege(int msg,char *str,int len,bool isr);
void n1_mqtt_disconnect(uint32_t mqtt_id);
void WIFI_driver_init(void);
extern unsigned char g_dbg_output;
extern int g_mqtt_id;
extern hal_rtc_status_t hal_rtc_enter_forced_reset_mode(void);

extern void m2m_mqttrec_analysis(char *temp);


ResultCode_t CH_AT_TEST (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  ResultCode_t             result = RESULT_CODE_OK;
  ExtendedOperation_t      operation = getExtendedOperation (commandBuffer_p);
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
  VgSimInfo                *simInfo = &(simLockGenericContext_p->simInfo);

  PlmnName                 plmnName;

	//dbg_print("CH_AT_TEST:%d",operation);
  switch (operation)
  {
    case EXTENDED_ACTION:      /* AT+CH */
    {
      break;
    }
    case EXTENDED_ASSIGN:      /* AT+CH= *///AT+CH=DBGPRINT,1#
	{
		char tmp_buf[200]={0};
		short strLength;
		char *ptr=NULL;
		result = RESULT_CODE_OK;
		
		//		+CH=TESTHAHA,13
		
		//		+CH=TESTHAHA,13
		ptr = strstr(commandBuffer_p->character,"DBGPRINT,");
		if(ptr)
		{
			ptr += 9;
			g_dbg_output = ptr[0]-'0';
		}

		ptr = strstr(commandBuffer_p->character,"TEST,");
		if(ptr)
		{
			ctiot_lwm2m_client_send_response(ptr);//TEST,22
			ptr += 5;
			char *Str="{1:1:0:0:111100000119952:S15:-32,20221204,195033}";
			switch(atoi(ptr))
			{
				case 0:
					n1_mqtt_create();
					break;
				case 1:
					n1_mqtt_connect();
					break;
				case 2:
					n1_mqtt_subscribe();
					break;
				case 3:
					n1_mqtt_publish("123456");
					break;
				case 4:
					n1_mqtt_disconnect(g_mqtt_id);
				case 5:
					hal_rtc_chuhuiset_utc_time(1636789219,0);
					break;
				case 6:
					m2m_mqttrec_analysis(Str);
					break;
				case 7:
					hal_rtc_enter_forced_reset_mode();
					break;
				case 20:
					m2m_test_message_handler();
					break;
				case 21:
					m2m_test_adress_bag();
					break;
				case 22:
					m2m_test_adress_bag2();
					break;
				default:
					break;
			}
		}

		
		ptr = strstr(commandBuffer_p->character,"SEND,");
		if(ptr)
		{
			int i= atoi(ptr+5);
			sbit_m2m_ct_send_massege(10+i,NULL,0,0);//M2M_CT_total_data_T50
		}
      break;
    }
    case EXTENDED_QUERY:       /* AT+CH? */
    case EXTENDED_RANGE:       /* AT+CH=? */
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
* Function:    vgGnMBSC
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MBSC which write direct to NVRAM.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMBSC(CommandLine_t *commandBuffer_p,
                                  const VgmuxChannelNumber entity)

{

    Int32               param;
    Int8 pIndex =0;
    ResultCode_t        result = RESULT_CODE_OK;
    uint32_t bandSupport = 0x00000000;
    uint8_t* bandSupportP =(uint8_t*) &bandSupport;
    uint32_t writeBuffer = 0x0;
#if defined (MTK_NVDM_MODEM_ENABLE)
    nvdm_modem_status_t status;
    VgNvmStatus vgNvmStatus =VG_NVM_STATUS_OK;
        Int8 band_list[22];
    nvdm_modem_data_item_type_t type;
    uint8_t                     readBuffer[4];
    uint32_t                    returnValue;
        Boolean                     first_band = TRUE;
    ExtendedOperation_t      operation = getExtendedOperation (commandBuffer_p);
    switch (operation)
    {
      case EXTENDED_ASSIGN:      /* AT*MBSC= */
      {
        /* Get Area Info */
        if(getExtendedParameter ( commandBuffer_p,
                                    &param,
                                    ULONG_MAX) == TRUE)
        {
            if(0 == param)
            {
                    bandSupport = 0x03FFFFF;
            }
                else if(param <=22)
            {       
                pIndex = param;
                for (; pIndex > 0; pIndex--)
                {
                   if (getExtendedParameter (commandBuffer_p, &param,ULONG_MAX) == TRUE)
                   {
                       if (param == ULONG_MAX)
                       {
                           return VG_CME_OPERATION_NOT_ALLOWED;
                       }
                       else
                       {
                         bandSupport = handleSupportBand(param,bandSupport);
                       }
                   }
                   else
                   {
                       return VG_CME_INVALID_INPUT_VALUE;
                   }
                }
        
            }
            else
            {
                return VG_CME_INVALID_INPUT_VALUE;
            }         
            writeBuffer = (bandSupportP[0]<<24) +
                          (bandSupportP[1]<<16) +
                          (bandSupportP[2]<<8)  +
                          (bandSupportP[3]);

            status = nvdm_modem_write_protected_data_item(
                 "NVDM_MODEM_CFG",
                 "BAND_SUPPORT",
                 NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                 (uint8_t*) &writeBuffer,
                 sizeof(uint32_t),
                 NVDM_MODEM_ATTR_AVERAGE);
                 vgNvmStatus = vgConvertNvramStatus (entity, status);
                 
                 vgPutNewLine (entity);
                 vgPrintf (entity,
                          (const Char *)"*MBSC: %d",vgNvmStatus);
                 vgPutNewLine (entity);           
        }
        else
        {
            result = VG_CME_OPERATION_NOT_SUPPORTED;
        }
        break;
     }
     case EXTENDED_QUERY: 
            memset((Int8*)(band_list), 0 ,22*sizeof (Int8));
        writeBuffer = 4;
        status = nvdm_modem_read_protected_data_item(
            "NVDM_MODEM_CFG",
            "BAND_SUPPORT",
            &type,
            &readBuffer[0],
            &writeBuffer);
        vgNvmStatus = vgConvertNvramStatus (entity, status);
        if(VG_NVM_STATUS_OK!=vgNvmStatus)
        {
            result = VG_CME_NVRAM_ITEM_READ_FAILED;
        }
        else
        {
            returnValue = 0;
            for (pIndex = 0; pIndex < writeBuffer; pIndex++)
            {
            /* NVDM data is in big endian format */
                returnValue |= ((uint32_t)readBuffer[pIndex] << (8 * (writeBuffer - pIndex - 1)));
            }
            readSupportBand(returnValue,band_list);
            vgPutNewLine (entity);
            vgPrintf (entity,(const Char *)"*MBSC: ");

                for(pIndex=0;pIndex<22;pIndex++)
            {
                if(0!=band_list[pIndex])
                {
                       if(first_band)
                       {
                    vgPrintf (entity,
                                     (const Char *)"%d",band_list[pIndex]);
                            first_band = FALSE;
                       }
                       else
                       {
                            vgPrintf (entity,
                                     (const Char *)",%d",band_list[pIndex]);                   
                       }
                }
            }
            vgPutNewLine (entity);
        }

        
        break;
     default:
        result = VG_CME_OPERATION_NOT_SUPPORTED;
        break;
  }
#endif
    return result;
    }

/*--------------------------------------------------------------------------
*
* Function:    vgGnMNVMOTPR
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MNVMOTPR which reads direct from NVRAM.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMNVMOTPR  (CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity)
{
  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  VgMnvmDataContext       *vgNvmDataContext_p = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMnvmDataContext);
  ResultCode_t             result = RESULT_CODE_OK;

#if defined (MTK_NVDM_MODEM_ENABLE)&& defined (MTK_NBIOT_TARGET_BUILD)

  ExtendedOperation_t      operation = getExtendedOperation (commandBuffer_p);
  Int16                    str_length;
  Int32                    buff_length, actual_data_length;
  nvdm_modem_status_t      status;
  VgNvmStatus              vgNvmStatus;

  memset((Int8*)(vgNvmDataContext_p), 0 , sizeof (VgMnvmDataContext));

  switch (operation)
  {
    case EXTENDED_ASSIGN:      /* AT*MNVMOTPR= */
    {

      if (result == RESULT_CODE_OK)
      {
        /* Get data item ID */
        if (getExtendedString ( commandBuffer_p,
            vgNvmDataContext_p->data_item_id,
            MAX_NVRAM_OTP_DATA_ITEM_ID_LEN,
            &str_length) == TRUE)
        {
            if (str_length == 0)
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
        /* Get length */
        if(getExtendedParameter ( commandBuffer_p,
                                    &buff_length,
                                    ULONG_MAX) == TRUE)
        {
            if (buff_length != ULONG_MAX)
            {
                if ((buff_length == 0) || (buff_length > MAX_NVRAM_DATA_ITEM_SIZE_BYTES))
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
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
       
      if (result == RESULT_CODE_OK)
      {
        actual_data_length = buff_length;
        
        /* Now read data from NVRAM
         */
        status = nvdm_modem_read_otp_data_item((const char *)vgNvmDataContext_p->data_item_id,
                                        (uint8_t *)vgNvmDataContext_p->data,
                                        &actual_data_length);

        if ((actual_data_length == 0) || (actual_data_length > buff_length))
        {
          /* User specified too small buffer length value */
          result = VG_CME_INVALID_INPUT_VALUE;
        }

        if (result == RESULT_CODE_OK)
        {
          vgNvmStatus = vgConvertNvramStatus (entity, status);

          /* Get the information about whether data is backed up or not */
          if (vgNvmStatus == VG_NVM_STATUS_OK)
          {
            /* Convert to hex string
             *
             */
            str_length = vgMapHexToTE(vgNvmDataContext_p->data_str,
                              MAX_NVRAM_DATA_ITEM_STR_LEN,
                              vgNvmDataContext_p->data,
                              actual_data_length,
                              VG_AT_CSCS_HEX);

            /* Display the text now */
            vgPutNewLine (entity);
            vgPrintf (entity,
                     (const Char *)"*MNVMOTPR: ");

            vgPrintf(entity, (const Char *)"%d,\"%s\",%d,\"",
                    vgNvmStatus,                    
                    vgNvmDataContext_p->data_item_id,
                    actual_data_length);

            vgNvmDataContext_p->data_str[str_length] = '\"';
            vgNvmDataContext_p->data_str[str_length+1] = 0;
            vgPuts(entity,(const Char *)vgNvmDataContext_p->data_str);
            vgFlushBuffer(entity);
          }
          else
          {
            vgPutNewLine (entity);
            vgPrintf (entity,
                     (const Char *)"*MNVMOTPR: %d",vgNvmStatus);
            vgPutNewLine (entity);
          }
        }
      }
      break;
    }  
    
    case EXTENDED_QUERY:       /* AT*MNVMR? */
    case EXTENDED_RANGE:       /* AT*MNVMR=? */
    {
      /* Do nothing */
      break;
    }

    case EXTENDED_ACTION:      /* AT*MNVMR */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
#endif /* MTK_NVDM_MODEM_ENABLE */

  return (result);  
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnMNVMGET
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MNVMGET which accesses list of
*              currently stored NVRAM data items.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMNVMGET  (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity)
{
  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  VgMnvmDataContext       *vgNvmDataContext_p = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMnvmDataContext);
  VgMnvmMcalContext       *vgNvmMcalContext_p = &(generalGenericContext_p->vgMnvmMcalContext);
  Boolean                  vgNvmAccessAuthorised = (generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised);
  ResultCode_t             result = RESULT_CODE_OK;

#if defined (MTK_NVDM_MODEM_ENABLE)
  ExtendedOperation_t           operation = getExtendedOperation (commandBuffer_p);    
  nvdm_modem_status_t           status;
  VgNvmStatus                   vgNvmStatus;
#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)
  tool_auth_response_t     auth_response = TOOL_AUTH_REQUIRED;
#endif

  memset((Int8*)(vgNvmDataContext_p), 0 , sizeof (VgMnvmDataContext));
  
  switch (operation)
  {
    case EXTENDED_ACTION:      /* AT*MNVMGET */
    {
 
      /* First get normal the data items */
      status = nvdm_modem_query_data_item_number(NVDM_MODEM_AREA_NORMAL,&vgNvmMcalContext_p->normalDataItems);
      
      /* Only allow access to protected area if authorisation occurred */
      if (vgNvmAccessAuthorised)
      {
         status = nvdm_modem_query_data_item_number(NVDM_MODEM_AREA_PROTECTED,&vgNvmMcalContext_p->protectedDataItems);
      }
#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)
      /* To check if needs tool authentication */
      else if(tool_auth_is_required(&auth_response) == TOOL_AUTH_STATUS_OK)
      {
          switch(auth_response)
          {
              case TOOL_AUTH_NOT_REQUIRED:
              {
                generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised = TRUE;
                status = nvdm_modem_query_data_item_number(NVDM_MODEM_AREA_PROTECTED,&vgNvmMcalContext_p->protectedDataItems);
                break;
              }
              case TOOL_AUTH_REQUIRED:
              {
                break;
              }
              default:
              {
                FatalFail("ATCI NVDM: Illegal auth response");
                break;  
              }
          }
       }
      else
      {
          /* Get authentication status fail, do nothing */
      }
#endif
 
      vgNvmMcalContext_p->totalDataItemsToSend = vgNvmMcalContext_p->normalDataItems + vgNvmMcalContext_p->protectedDataItems;
      

      if ((status == NVDM_MODEM_STATUS_OK) && (vgNvmMcalContext_p->totalDataItemsToSend > 0))
      {
        /* Store all data items now */
        if ( vgNvmMcalContext_p->normalDataItems >0 )
        {
          KiAllocZeroMemory ((sizeof(nvdm_modem_data_item_info_t) *vgNvmMcalContext_p->normalDataItems), (void **) &vgNvmMcalContext_p->info_list_p_normal);
        
          if (vgNvmMcalContext_p->info_list_p_normal != PNULL)
          {
            status = nvdm_modem_query_all_data_item_info (NVDM_MODEM_AREA_NORMAL,
                                                      vgNvmMcalContext_p->info_list_p_normal,
                                                      vgNvmMcalContext_p->normalDataItems);
          }
          else
          {       
            FatalFail ("ATCI Modem Fatal Assert, no memory");
          }            
        }
        
        if ((status == NVDM_MODEM_STATUS_OK) && ( vgNvmMcalContext_p->protectedDataItems> 0))
        {
          KiAllocZeroMemory ((sizeof(nvdm_modem_data_item_info_t) *vgNvmMcalContext_p->protectedDataItems), (void **) &vgNvmMcalContext_p->info_list_p_protected);
        
          if (vgNvmMcalContext_p->info_list_p_protected != PNULL)
          {
            status = nvdm_modem_query_all_data_item_info (NVDM_MODEM_AREA_PROTECTED,
                                                        vgNvmMcalContext_p->info_list_p_protected,
                                                        vgNvmMcalContext_p->protectedDataItems);
          }
          else
          {       
               FatalFail ("ATCI Modem Fatal Assert, no memory");
          }
        }
    
            
        if (status == NVDM_MODEM_STATUS_OK)
        {
          if (vgNvmMcalContext_p->totalDataItemsToSend > 0)
          {
            vgNvmMcalContext_p->nextDataItemNumberToSend = 0;
            vgNvmMcalContext_p->currentNvmChannel = entity;

            /* Now send the next data item */
            vgSendNextMnvmgetDataItemToMux(entity);

            /* Must have been the one and only item */
            if (vgNvmMcalContext_p->totalDataItemsToSend == 0)
            {
              result = RESULT_CODE_OK;
            }
            else
            {
              result = RESULT_CODE_PROCEEDING;
            }
          }          
          else
          {
          
            vgNvmStatus = vgConvertNvramStatus (entity, status);
            vgPutNewLine (entity);
            vgPrintf (entity,
                     (const Char *)"*MNVMGETST: %d", vgNvmStatus);
            vgPutNewLine (entity);
          }
        }
      }          
      else
      {
        vgNvmStatus = vgConvertNvramStatus (entity, status);
        vgPutNewLine (entity);
        vgPrintf (entity,
                 (const Char *)"*MNVMGETST: %d", vgNvmStatus);
        vgPutNewLine (entity);
      }
      break;
    }
    case EXTENDED_QUERY:       /* AT*MNVMGET? */
    case EXTENDED_RANGE:       /* AT*MNVMGET=? */
    {
      /* Do nothing */
      break;
    }

    case EXTENDED_ASSIGN:      /* AT*MNVMGET= */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }    
  }
#endif /* MTK_NVDM_MODEM_ENABLE */
  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnMNVMIVD
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MNVMIVD which invalidated a location
*              in NVRAM.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMNVMIVD  (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  VgMnvmDataContext       *vgNvmDataContext_p = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMnvmDataContext);
  Boolean                  vgNvmAccessAuthorised = (generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised);
  ResultCode_t             result = RESULT_CODE_OK;

#if defined (MTK_NVDM_MODEM_ENABLE)
  ExtendedOperation_t      operation = getExtendedOperation (commandBuffer_p);
  Int32                    param;
  VgNvmAreaInfo            areaInfo;
  Int16                    str_length;
  nvdm_modem_status_t      status;
  VgNvmStatus              vgNvmStatus;
  Boolean                  protected_area = FALSE;
#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)
  tool_auth_response_t     auth_response = TOOL_AUTH_REQUIRED;
#endif

  memset((Int8*)(vgNvmDataContext_p), 0 , sizeof (VgMnvmDataContext));

  switch (operation)
  {
    case EXTENDED_ASSIGN:      /* AT*MNVMIVD= */
    {
      /* Get Area Info */
      if(getExtendedParameter ( commandBuffer_p,
                                  &param,
                                  ULONG_MAX) == TRUE)
      {
          if (param != ULONG_MAX)
          {
              if (param >= VG_NVM_NUM_AREAS)
              {
                  result = VG_CME_INVALID_INPUT_VALUE;
              }
              else
              {
                  areaInfo = (VgNvmAreaInfo)param;

                  if (areaInfo == VG_NVM_PROTECTED_AREA)
                  {
                    /* Only allow access to protected area if authorisation occurred */
                    if (vgNvmAccessAuthorised)
                    {
                      protected_area = TRUE;
                    }
#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)
                    /* To check if needs tool authentication */
                    else if(tool_auth_is_required(&auth_response) == TOOL_AUTH_STATUS_OK)
                    {
                        switch(auth_response)
                        {
                            case TOOL_AUTH_NOT_REQUIRED:
                            {
                              generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised = TRUE;
                              protected_area = TRUE;
                              break;
                            }
                            case TOOL_AUTH_REQUIRED:
                            {
                              result = VG_CME_OPERATION_NOT_ALLOWED;
                              break;
                            }
                            default:
                            {
                              FatalFail("ATCI NVDM: Illegal auth response");
                              break;  
                            }
                        }
                     }
#endif
                    else
                    {
                      result = VG_CME_OPERATION_NOT_ALLOWED;
                    }
                  }
                  else if (areaInfo != VG_NVM_NORMAL_AREA)
                  {
                    result = VG_CME_INVALID_INPUT_VALUE;
                  }
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

      if (result == RESULT_CODE_OK)
      {
        /* Get group ID */
        if (getExtendedString ( commandBuffer_p,
            vgNvmDataContext_p->group_id,
            MAX_NVRAM_GROUP_ID_LEN,
            &str_length) == TRUE)
        {
            if (str_length == 0)
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
        /* Get data item ID */
        if (getExtendedString ( commandBuffer_p,
            vgNvmDataContext_p->data_item_id,
            MAX_NVRAM_DATA_ITEM_ID_LEN,
            &str_length) == TRUE)
        {
            if (str_length == 0)
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
        if (protected_area)
        {
          status = nvdm_modem_invalidate_protected_data_item((const char *)vgNvmDataContext_p->group_id,
                                                             (const char *)vgNvmDataContext_p->data_item_id);
        }
        else
        {
          status = nvdm_modem_invalidate_normal_data_item((const char *)vgNvmDataContext_p->group_id,
                                                             (const char *)vgNvmDataContext_p->data_item_id);
        }
       
        vgNvmStatus = vgConvertNvramStatus (entity, status);
        
        vgPutNewLine (entity);
        vgPrintf (entity,
                 (const Char *)"*MNVMIVD: %d",vgNvmStatus);
        vgPutNewLine (entity);
      }
      break;  
    }  
    case EXTENDED_QUERY:       /* AT*MNVMIVD? */
    case EXTENDED_RANGE:       /* AT*MNVMIVD=? */
    {
      /* Do nothing */
      break;
    }

    case EXTENDED_ACTION:      /* AT*MNVMIVD */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
#endif /* MTK_NVDM_MODEM_ENABLE */

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnMNVMRSTONE
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MNVMRSTONE which resets a data item
*              to factory defaults.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMNVMRSTONE  (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  VgMnvmDataContext       *vgNvmDataContext_p = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMnvmDataContext);
  Boolean                  vgNvmAccessAuthorised = (generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised);
  ResultCode_t             result = RESULT_CODE_OK;

#if defined (MTK_NVDM_MODEM_ENABLE)
  ExtendedOperation_t      operation = getExtendedOperation (commandBuffer_p);
  Int32                    param;
  VgNvmAreaInfo            areaInfo;
  Int16                    str_length;
  nvdm_modem_status_t      status;
  VgNvmStatus              vgNvmStatus;
  Boolean                  protected_area = FALSE;
#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)
  tool_auth_response_t     auth_response = TOOL_AUTH_REQUIRED;
#endif

  memset((Int8*)(vgNvmDataContext_p), 0 , sizeof (VgMnvmDataContext));

  switch (operation)
  {
    case EXTENDED_ASSIGN:      /* AT*MNVMRSTONE= */
    {
      /* Get Area Info */
      if(getExtendedParameter ( commandBuffer_p,
                                  &param,
                                  ULONG_MAX) == TRUE)
      {
          if (param != ULONG_MAX)
          {
              if (param >= VG_NVM_NUM_AREAS)
              {
                  result = VG_CME_INVALID_INPUT_VALUE;
              }
              else
              {
                  areaInfo = (VgNvmAreaInfo)param;

                  if (areaInfo == VG_NVM_PROTECTED_AREA)
                  {

                    /* Only allow access to protected area if authorisation occurred */
                    if (vgNvmAccessAuthorised)
                    {
                      protected_area = TRUE;
                    }
                    
#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)
                    /* To check if needs tool authentication */
                    else if(tool_auth_is_required(&auth_response) == TOOL_AUTH_STATUS_OK)
                    {
                        switch(auth_response)
                        {
                            case TOOL_AUTH_NOT_REQUIRED:
                            {
                              generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised = TRUE;
                              protected_area = TRUE;
                              break;
                            }
                            case TOOL_AUTH_REQUIRED:
                            {
                              result = VG_CME_OPERATION_NOT_ALLOWED;
                              break;
                            }
                            default:
                            {
                              FatalFail("ATCI NVDM: Illegal auth response");
                              break;  
                            }
                        }
                     }
#endif
                    else
                    {
                      result = VG_CME_OPERATION_NOT_ALLOWED;
                    }
                  }
                  else if (areaInfo != VG_NVM_NORMAL_AREA)
                  {
                    result = VG_CME_INVALID_INPUT_VALUE;
                  }
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

      if (result == RESULT_CODE_OK)
      {
        /* Get group ID */
        if (getExtendedString ( commandBuffer_p,
            vgNvmDataContext_p->group_id,
            MAX_NVRAM_GROUP_ID_LEN,
            &str_length) == TRUE)
        {
            if (str_length == 0)
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
        /* Get data item ID */
        if (getExtendedString ( commandBuffer_p,
            vgNvmDataContext_p->data_item_id,
            MAX_NVRAM_DATA_ITEM_ID_LEN,
            &str_length) == TRUE)
        {
            if (str_length == 0)
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
        if (protected_area)
        {
          status = nvdm_modem_reset_protected_data_item((const char *)vgNvmDataContext_p->group_id,
                                                             (const char *)vgNvmDataContext_p->data_item_id);
        }
        else
        {
          status = nvdm_modem_reset_normal_data_item((const char *)vgNvmDataContext_p->group_id,
                                                             (const char *)vgNvmDataContext_p->data_item_id);
        }

        vgNvmStatus = vgConvertNvramStatus (entity, status);

        vgPutNewLine (entity);
        vgPrintf (entity,
                 (const Char *)"*MNVMRSTONE: %d",vgNvmStatus);
        vgPutNewLine (entity);
      }
      break;
    }  
    case EXTENDED_QUERY:       /* AT*MNVMRSTONE? */
    case EXTENDED_RANGE:       /* AT*MNVMRSTONE=? */
    {
      /* Do nothing */
      break;
    }

    case EXTENDED_ACTION:      /* AT*MNVMRSTONE */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
#endif /* MTK_NVDM_MODEM_ENABLE */

  return (result);
  
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnMNVMRST
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MNVMRST which resets a data item
*              to factory defaults.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMNVMRST  (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{

  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  VgMnvmDataContext       *vgNvmDataContext_p = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMnvmDataContext);
  Boolean                  vgNvmAccessAuthorised = (generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised);
  ResultCode_t             result = RESULT_CODE_OK;

#if defined (MTK_NVDM_MODEM_ENABLE)
  ExtendedOperation_t           operation = getExtendedOperation (commandBuffer_p);
  nvdm_modem_status_t           status;
  VgNvmStatus                   vgNvmStatus;
  nvdm_modem_data_item_info_t   *info_list_p = PNULL;   /* Pointer to structure containing all data items */
  Int32                         number_of_data_items;
  Int32                         i;
#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)
  tool_auth_response_t     auth_response = TOOL_AUTH_REQUIRED;
#endif

  memset((Int8*)(vgNvmDataContext_p), 0 , sizeof (VgMnvmDataContext));

  switch (operation)
  {
    case EXTENDED_ACTION:      /* AT*MNVMRST */
    {

      /* First get all the normal data items */
      status = nvdm_modem_query_data_item_number(NVDM_MODEM_AREA_NORMAL,
                                                 &number_of_data_items);

      if ((status == NVDM_MODEM_STATUS_OK)&&(number_of_data_items >0))
      {
        /* Store all normal data items now */

        KiAllocZeroMemory ((sizeof(nvdm_modem_data_item_info_t) * number_of_data_items), (void **) &info_list_p);

        if (info_list_p != PNULL)
        {
          status = nvdm_modem_query_all_data_item_info (NVDM_MODEM_AREA_NORMAL,
                                                        info_list_p,
                                                        number_of_data_items);
          if (status == NVDM_MODEM_STATUS_OK)
          {
            /* Now loop around resetting all data items */
            for (i=0 ; (i < number_of_data_items) && (status == NVDM_MODEM_STATUS_OK); i++)
            {
               status = nvdm_modem_reset_normal_data_item((const char *)info_list_p[i].group_name,
                                                              (const char *)info_list_p[i].data_item_name);
               /* For nvdm_modem_reset_normal_data_item, NO_BACKUP can be regarded as OK */
               if(status == NVDM_MODEM_STATUS_NO_BACKUP)
               {
                  status = NVDM_MODEM_STATUS_OK;
               }
            }
          }
          /* now reset the loop variables */
          KiFreeMemory ((void **) &info_list_p);
          number_of_data_items = 0;
          info_list_p = PNULL;

#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)
          /* To check if needs tool authentication */
          if(tool_auth_is_required(&auth_response) == TOOL_AUTH_STATUS_OK)
          {
              switch(auth_response)
              {
                  case TOOL_AUTH_NOT_REQUIRED:
                  {
                    generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised = TRUE;
                    vgNvmAccessAuthorised = TRUE;
                    break;
                  }
                  case TOOL_AUTH_REQUIRED:
                  {
                    generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised = FALSE;
                    vgNvmAccessAuthorised = FALSE;
                    break;
                  }
                  default:
                  {
                    FatalFail("ATCI NVDM: Illegal auth response");
                    break;
                  }
              }
           }
#endif

          /* now handle the protected area items */
          if ((vgNvmAccessAuthorised) && (status == NVDM_MODEM_STATUS_OK))
          {
             status = nvdm_modem_query_data_item_number(NVDM_MODEM_AREA_PROTECTED,
                                                 &number_of_data_items);

             if ((status == NVDM_MODEM_STATUS_OK)&&(number_of_data_items >0))
             {
               /* Store all protected data items now */

               KiAllocZeroMemory ((sizeof(nvdm_modem_data_item_info_t) * number_of_data_items), (void **) &info_list_p);

               if (info_list_p != PNULL)
               {
                 status = nvdm_modem_query_all_data_item_info (NVDM_MODEM_AREA_PROTECTED,
                                                        info_list_p,
                                                        number_of_data_items);
                 if (status == NVDM_MODEM_STATUS_OK)
                 {
                    /* Now loop around resetting all data items */
                    for (i=0 ; (i < number_of_data_items) && (status == NVDM_MODEM_STATUS_OK); i++)
                    {
                          status = nvdm_modem_reset_protected_data_item((const char *)info_list_p[i].group_name,
                                                              (const char *)info_list_p[i].data_item_name);

                          /* For nvdm_modem_reset_protected_data_item, NO_BACKUP can be regarded as OK */
                          if(status == NVDM_MODEM_STATUS_NO_BACKUP)
                          {
                             status = NVDM_MODEM_STATUS_OK;
                          }
                    }
                 }
                 KiFreeMemory ((void **) &info_list_p);
              }
              else
              {
                 FatalFail ("ATCI Modem Fatal Assert, no memory");
              }
            }
          }
        }
        else
        {
          FatalFail ("ATCI Modem Fatal Assert, no memory");
        }
      }

      vgNvmStatus = vgConvertNvramStatus (entity, status);

      vgPutNewLine (entity);
      vgPrintf (entity,
               (const Char *)"*MNVMRST: %d",vgNvmStatus);
      vgPutNewLine (entity);

      break;
    }
    case EXTENDED_QUERY:       /* AT*MNVMRST? */
    case EXTENDED_RANGE:       /* AT*MNVMRST=? */
    {
      /* Do nothing */
      break;
    }

    case EXTENDED_ASSIGN:      /* AT*MNVMRST= */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
#endif /* MTK_NVDM_MODEM_ENABLE */

  return (result);

}

/****************************/
/* NVRAM mini dump commands */
/****************************/
#if defined (FEA_MINI_DUMP)

/*--------------------------------------------------------------------------
*
* Function:    vgGnMNVMMDNQ
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MNVMMDNQ for NVRAM mini-dump.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMNVMMDNQ   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  ResultCode_t             result = RESULT_CODE_OK;

#if defined (MTK_NVDM_MODEM_ENABLE)
  ExtendedOperation_t           operation = getExtendedOperation (commandBuffer_p);
  nvdm_modem_status_t           status;
  VgNvmStatus                   vgNvmStatus;
  Int8                          mini_dump_num;
  

  switch (operation)
  {
    case EXTENDED_ACTION:      /* AT*MNVMMDNQ */
    {

      status = nvdm_modem_query_mini_dump_number(&mini_dump_num);

      vgNvmStatus = vgConvertNvramStatus (entity, status);

      vgPutNewLine (entity);
      vgPrintf (entity,
               (const Char *)"*MNVMMDNQ: %d",vgNvmStatus);
      
      if (vgNvmStatus == VG_NVM_STATUS_OK)
      {
        vgPrintf (entity,
                 (const Char *)",%d",mini_dump_num);
      }
      
      vgPutNewLine (entity);

      break;
    }
    case EXTENDED_QUERY:       /* AT*MNVMMDNQ? */
    case EXTENDED_RANGE:       /* AT*MNVMMDNQ=? */
    {
      /* Do nothing */
      break;
    }

    case EXTENDED_ASSIGN:      /* AT*MNVMMDNQ= */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
#endif /* MTK_NVDM_MODEM_ENABLE */

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnMNVMMDR
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MNVMMDPR for NVRAM mini-dump.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMNVMMDR   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  VgMnvmDataContext       *vgNvmDataContext_p = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMnvmDataContext);
  ResultCode_t             result = RESULT_CODE_OK;

#if defined (MTK_NVDM_MODEM_ENABLE)
  ExtendedOperation_t           operation = getExtendedOperation (commandBuffer_p);
  nvdm_modem_status_t           status;
  VgNvmStatus                   vgNvmStatus;
  Int32                         param;
  Int8                          mini_dump_idx;
  Int16                         offset;
  Int16                         data_length;
  Int16                         str_length = 0;

  memset((Int8*)(vgNvmDataContext_p), 0 , sizeof (VgMnvmDataContext));
  
  switch (operation)
  {
    case EXTENDED_ASSIGN:      /* AT*MNVMMDR= */
    {
      /* Get Mini Dump Index */
      if(getExtendedParameter ( commandBuffer_p,
                                  &param,
                                  ULONG_MAX) == TRUE)
      {
          if (param != ULONG_MAX)
          {
              if (param > MAX_NVRAM_MINI_DUMP_INDEX)
              {
                  result = VG_CME_INVALID_INPUT_VALUE;
              }
              else
              {
                  mini_dump_idx = (Int8)param;
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

      if (result == RESULT_CODE_OK)
      {
        /* Get Offset */
        if(getExtendedParameter ( commandBuffer_p,
                                    &param,
                                    ULONG_MAX) == TRUE)
        {
            if (param != ULONG_MAX)
            {
                if (param > MAX_NVRAM_MINI_DUMP_OFFSET)
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                    offset = (Int16)param;
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

      if (result == RESULT_CODE_OK)
      {
        /* Get Data length */
        if(getExtendedParameter ( commandBuffer_p,
                                    &param,
                                    ULONG_MAX) == TRUE)
        {
            if (param != ULONG_MAX)
            {
                if ((param == 0) || (param > MAX_NVRAM_MINI_DUMP_READ_LENGTH))
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                    data_length = (Int16)param;
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

      if (result == RESULT_CODE_OK)
      {

        status = nvdm_modem_read_mini_dump_data(mini_dump_idx, 
                                      offset,
                                      vgNvmDataContext_p->data,
                                      &data_length);

        vgNvmStatus = vgConvertNvramStatus (entity, status);

        if ((vgNvmStatus == VG_NVM_STATUS_OK) && (data_length <= MAX_NVRAM_MINI_DUMP_READ_LENGTH))
        {
          if (data_length > 0)
          {
            /* Convert to hex string
             *
             */
            str_length = vgMapHexToTE(vgNvmDataContext_p->data_str,
                              MAX_NVRAM_DATA_ITEM_STR_LEN,
                              vgNvmDataContext_p->data,
                              data_length,
                              VG_AT_CSCS_HEX);
          }
        
          /* Display the text now */
          vgPutNewLine (entity);
          vgPrintf (entity,
                   (const Char *)"*MNVMMDR: ");

          vgPrintf(entity, (const Char *)"%d,%d,%d,%d",
                  vgNvmStatus,
                  mini_dump_idx,
                  offset,
                  data_length);

          if (data_length > 0)
          {
            vgPrintf(entity, (const Char *)",\"");
            vgNvmDataContext_p->data_str[str_length] = '\"';
            vgNvmDataContext_p->data_str[str_length+1] = 0;
            vgPuts(entity,(const Char *)vgNvmDataContext_p->data_str);
          }
          else
          {
            vgPutNewLine (entity);
          }
            
          vgFlushBuffer(entity);
        }
        else
        {
          vgPutNewLine (entity);
          vgPrintf (entity,
                   (const Char *)"*MNVMMDR: %d",vgNvmStatus);
          vgPutNewLine (entity);
        }
      }
      break;
    }
    case EXTENDED_QUERY:       /* AT*MNVMMDR? */
    case EXTENDED_RANGE:       /* AT*MNVMMDR=? */
    {
      /* Do nothing */
      break;
    }

    case EXTENDED_ACTION:      /* AT*MNVMMDR */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
#endif /* MTK_NVDM_MODEM_ENABLE */

  return (result);  
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnMNVMMDC
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MNVMMDC for NVRAM mini-dump.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMNVMMDC   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  ResultCode_t             result = RESULT_CODE_OK;

#if defined (MTK_NVDM_MODEM_ENABLE)
  ExtendedOperation_t           operation = getExtendedOperation (commandBuffer_p);
  nvdm_modem_status_t           status;
  VgNvmStatus                   vgNvmStatus;
  Int32                         param;
  Int8                          mini_dump_idx;
  
  switch (operation)
  {
    case EXTENDED_ASSIGN:      /* AT*MNVMMDC= */
    {
      /* Get Mini Dump Index */
      if(getExtendedParameter ( commandBuffer_p,
                                  &param,
                                  ULONG_MAX) == TRUE)
      {
          if (param != ULONG_MAX)
          {
              if (param > MAX_NVRAM_MINI_DUMP_INDEX)
              {
                  result = VG_CME_INVALID_INPUT_VALUE;
              }
              else
              {
                  mini_dump_idx = (Int8)param;
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

      if (result == RESULT_CODE_OK)
      {
        status = nvdm_modem_clean_mini_dump_data(mini_dump_idx);

        vgNvmStatus = vgConvertNvramStatus (entity, status);

        vgPutNewLine (entity);
        vgPrintf (entity,
                 (const Char *)"*MNVMMDC: %d",vgNvmStatus);
        
        if (vgNvmStatus == VG_NVM_STATUS_OK)
        {
          vgPrintf (entity,
                   (const Char *)",%d",mini_dump_idx);
        }
        
        vgPutNewLine (entity);
      }          
      break;
    }
    case EXTENDED_QUERY:       /* AT*MNVMMDC? */
    case EXTENDED_RANGE:       /* AT*MNVMMDC=? */
    {
      /* Do nothing */
      break;
    }

    case EXTENDED_ACTION:      /* AT*MNVMMDC */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
#endif /* MTK_NVDM_MODEM_ENABLE */

  return (result);

}
#endif
/*--------------------------------------------------------------------------
*
* Function:    vgGnMNVUID
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MNVUID for read UID value.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMNVUID   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  ResultCode_t             result = RESULT_CODE_OK;

#if defined (MTK_TOOL_AUTHENTICATION_ENABLE)
  ExtendedOperation_t           operation = getExtendedOperation (commandBuffer_p);
  VgNvmStatus             vgNvmStatus;
  uint8_t               vgUidStr[MAX_NVRAM_UID_STR_LEN + NULL_TERMINATOR_LENGTH];
  Int8              data_str[MAX_NVRAM_UID_STR_LEN * 2 + NULL_TERMINATOR_LENGTH * 2];
  Int16             str_length;
  
  switch (operation)
  {
    case EXTENDED_ACTION:      /* AT*MNVUID */
    {
    memset(vgUidStr, 0 , sizeof (vgUidStr));
    memset(data_str, 0 , sizeof (data_str));    
    
    vgNvmStatus = tool_auth_get_uid(vgUidStr, MAX_NVRAM_UID_STR_LEN);   
    
    if(VG_NVM_STATUS_OK == vgNvmStatus)
    {
      /* Convert to hex string
       *
       */
        str_length = vgMapHexToTE(data_str,
                        MAX_NVRAM_UID_STR_LEN * 2,
                        vgUidStr,
                        MAX_NVRAM_UID_STR_LEN,
                        VG_AT_CSCS_HEX);

        /* Display the text now */
        vgPutNewLine (entity);
        vgPrintf (entity,
                 (const Char *)"*MNVUID: ");
        
        vgPrintf(entity, (const Char *)",\"");
        data_str[str_length] = '\"';
        data_str[str_length+1] = 0;
        vgPuts(entity,(const Char *)data_str);
      
        vgFlushBuffer(entity);
    }
    else
    {
        result = VG_CME_OPERATION_NOT_ALLOWED;
    }
      break;
    }
    case EXTENDED_QUERY:       /* AT*MNVUID? */
    case EXTENDED_RANGE:       /* AT*MNVUID=? */
    {
      /* Do nothing */
      break;
    }

    case EXTENDED_ASSIGN:      /* AT*MNVUID= */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
#endif /* MTK_NVDM_MODEM_ENABLE */

  return (result);

}

  
/****************************/
/* IDC RF Control commands  */  
/****************************/
#if defined (ATCI_IDC_ENABLE)

/*--------------------------------------------------------------------------
*
* Function:    vgGnIDCFREQ
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*IDCFREQ for IDC RF control.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnIDCFREQ   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  ResultCode_t                  result = RESULT_CODE_OK;
  VgIdcRfFreqRangeList          idcRfFreqRangeList = {{0,0},{0,0},{0,0}};
  Int16                         freqRangeIndex = 0;

  ExtendedOperation_t           operation = getExtendedOperation (commandBuffer_p);
  Int32                         param;
  Boolean                       rangeListNotComplete = TRUE;

  switch (operation)
  {
    case EXTENDED_ASSIGN:      /* AT+IDCFREQ= */
    {
      while ((result == RESULT_CODE_OK) && rangeListNotComplete)
      {
        /* Get frequency range start */
        if(getExtendedParameter ( commandBuffer_p,
                                    &param,
                                    ULONG_MAX) == TRUE)
        {
            if (param != ULONG_MAX)
            {
                if (param > IDC_RF_MAX_FREQ)
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                    idcRfFreqRangeList[freqRangeIndex].freq_start = (Int16)param;
                    if ((freqRangeIndex == 0) && (param == IDC_RF_FREQ_STOP_URC))
                    {
                      rangeListNotComplete = FALSE;
                    }
                }
            }
            else
            {
                /* Must have at least one freq range */
                if (freqRangeIndex == 0)
                {
                  result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                  /* Not more params so end here */
                  rangeListNotComplete = FALSE;
                }
            }
        }
        else
        {
            result = VG_CME_INVALID_INPUT_VALUE;
        }
        
        if ((result == RESULT_CODE_OK) && (rangeListNotComplete))
        {
          /* Get frequency range stop */
          if(getExtendedParameter ( commandBuffer_p,
                                      &param,
                                      ULONG_MAX) == TRUE)
          {
              if (param != ULONG_MAX)
              {
                  if (param > IDC_RF_MAX_FREQ)
                  {
                      result = VG_CME_INVALID_INPUT_VALUE;
                  }
                  else
                  {
                      idcRfFreqRangeList[freqRangeIndex].freq_stop = (Int16)param;
                      freqRangeIndex++;
                      if (freqRangeIndex >= IDC_RF_MAX_NUM_FREQ_RANGES)
                      {
                        /* We have all 3 ranges now so stop */
                        rangeListNotComplete = FALSE;
                      }
                  }
              }
              else
              {
                  /* Must always have stop for start freq */
                  result = VG_CME_INVALID_INPUT_VALUE;
              }
          }
          else
          {
              result = VG_CME_INVALID_INPUT_VALUE;
          }
        }
      }

      if (result == RESULT_CODE_OK)
      {
        /* TODO: When API available */
#if 0
        /* Send Freq ranges to IDC RF */
#endif
        /* TODO: Temporary until AT command implemented - to stop compiler warnings */
        PARAMETER_NOT_USED(idcRfFreqRangeList);
      }
      break;
    }
    case EXTENDED_QUERY:       /* AT+IDCFREQ? */
    case EXTENDED_RANGE:       /* AT+IDCFREQ=? */
    {
      /* Do nothing */
      break;
    }

    case EXTENDED_ACTION:      /* AT+IDCFREQ */
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
* Function:    vgGnIDCPWRBACKOFF
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*IDCPWRBACKOFF for IDC RF control.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnIDCPWRBACKOFF   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  ResultCode_t                  result = RESULT_CODE_OK;
  Int16                         attenuation_power;

  ExtendedOperation_t           operation = getExtendedOperation (commandBuffer_p);
  Int32                         param;

  switch (operation)
  {
    case EXTENDED_ASSIGN:      /* AT+IDCPWRBACKOFF= */
    {
      /* Get attenuation power */
      if(getExtendedParameter ( commandBuffer_p,
                                  &param,
                                  ULONG_MAX) == TRUE)
      {
          if (param != ULONG_MAX)
          {
              if (param > IDC_RF_MAX_ATTENUATION_POWER)
              {
                  result = VG_CME_INVALID_INPUT_VALUE;
              }
              else
              {
                  attenuation_power = (Int16)param;
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

      if (result == RESULT_CODE_OK)
      {
        /* TODO: When API available */
#if 0
        /* Send attenuation power to IDC RF */
#endif
        /* TODO: Temporary until AT command implemented - to stop compiler warnings */
        PARAMETER_NOT_USED(attenuation_power);
  
      }
      break;
    }
    case EXTENDED_QUERY:       /* AT+IDCPWRBACKOFF? */
    case EXTENDED_RANGE:       /* AT+IDCPWRBACKOFF=? */
    {
      /* Do nothing */
      break;
    }

    case EXTENDED_ACTION:      /* AT+IDCPWRBACKOFF */
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
* Function:    vgGnIDCTX2GPS
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*IDCTX2GPS for IDC RF control.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnIDCTX2GPS   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  ResultCode_t                  result = RESULT_CODE_OK;
  VgIdcTx2GpsMode               mode;
  Int16                         freq = 0;
  Int16                         tx_pwr = 0;
  Int16                         period = 0;

  ExtendedOperation_t           operation = getExtendedOperation (commandBuffer_p);
  Int32                         param;
  
  switch (operation)
  {
    case EXTENDED_ASSIGN:      /* AT+IDCTX2GPS= */
    {
      /* Get mode */
      if(getExtendedParameter ( commandBuffer_p,
                                  &param,
                                  ULONG_MAX) == TRUE)
      {
          if (param != ULONG_MAX)
          {
              if (param >= IDC_TX2GPS_NUM_MODES)
              {
                  result = VG_CME_INVALID_INPUT_VALUE;
              }
              else
              {
                  mode = (VgIdcTx2GpsMode)param;
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

      /* Get remaining params if mode not set to STOP_TX */
      if ((mode != IDC_TX2GPS_MODE_STOP_TX) && (result == RESULT_CODE_OK))
      {
        /* Get freq */
        if(getExtendedParameter ( commandBuffer_p,
                                    &param,
                                    ULONG_MAX) == TRUE)
        {
            if (param != ULONG_MAX)
            {
                if (param > IDC_RF_MAX_FREQ)
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                    freq = (Int16)param;
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

        if (result == RESULT_CODE_OK)
        {
          /* Get tx_pwr */
          if(getExtendedParameter ( commandBuffer_p,
                                      &param,
                                      ULONG_MAX) == TRUE)
          {
              if (param != ULONG_MAX)
              {
                  if (param > IDC_RF_MAX_TX_PWR)
                  {
                      result = VG_CME_INVALID_INPUT_VALUE;
                  }
                  else
                  {
                      tx_pwr = (Int16)param;
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
        
        if (result == RESULT_CODE_OK)
        {
          /* Get period */
          if(getExtendedParameter ( commandBuffer_p,
                                      &param,
                                      ULONG_MAX) == TRUE)
          {
              if (param != ULONG_MAX)
              {
                  if ((param < IDC_RF_MIN_PERIOD) || (param > IDC_RF_MAX_PERIOD))
                  {
                      result = VG_CME_INVALID_INPUT_VALUE;
                  }
                  else
                  {
                      period = (Int16)param;
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
      }

      if (result == RESULT_CODE_OK)
      {
        /* TODO: When API available */
#if 0
        /* Send request to IDC */
#endif
        /* TODO: Temporary until AT command implemented - to stop compiler warnings */
        PARAMETER_NOT_USED(freq);
        PARAMETER_NOT_USED(tx_pwr);
        PARAMETER_NOT_USED(period);

      }
      break;
    }
    case EXTENDED_QUERY:       /* AT+IDCTX2GPS? */
    case EXTENDED_RANGE:       /* AT+IDCTX2GPS=? */
    {
      /* Do nothing */
      break;
    }

    case EXTENDED_ACTION:      /* AT+IDCTX2GPS */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}
#endif
/*--------------------------------------------------------------------------
 *
 * Function:        vgGnIDCTEST
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     This executes command AT*IDCTEST for IDC RF testing.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgGnIDCTEST(CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t            result                       = RESULT_CODE_OK;
    GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
    VgMcalContext           *mcalContext_p            = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMcalContext);
    Int16                   strLength;
    Char                    *data_ptr;

    switch( getExtendedOperation(commandBuffer_p) )
    {
      case EXTENDED_ASSIGN: /* AT*IDCTEST=*/
      {
        /* If not in CALDEV state then not permitted */
        if (generalGenericContext_p->vgMnvmMcalContext.vgCaldevStatus == VG_CALDEV_ENABLED)
        {
          initMcalContextData(mcalContext_p);

          /* Get token */
          if(getExtendedParameter ( commandBuffer_p,
                                      &mcalContext_p->token,
                                      ULONG_MAX) == TRUE)
          {
              if (mcalContext_p->token != ULONG_MAX)
                
              {
                  if (mcalContext_p->token > MCAL_MAX_TOKEN)
                  {
                      result = VG_CME_INVALID_INPUT_VALUE;
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

          if (result == RESULT_CODE_OK)
          {
            /* Get command */
            if(getExtendedParameter ( commandBuffer_p,
                                        &mcalContext_p->command,
                                        ULONG_MAX) == TRUE)
            {
                if (mcalContext_p->command != ULONG_MAX)
                  
                {
                    if (mcalContext_p->command > MCAL_MAX_COMMAND)
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
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

          if (result == RESULT_CODE_OK)
          {
            /* Get length */
            if(getExtendedParameter ( commandBuffer_p,
                                        &mcalContext_p->length,
                                        ULONG_MAX) == TRUE)
            {
                if (mcalContext_p->length != ULONG_MAX)
                  
                {
                    if ((mcalContext_p->length > MAX_MCAL_DATA_SIZE_BYTES) || (mcalContext_p->length > MAX_N1CD_IDC_TEST_REQ_PARAM_LENGTH))
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
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

          if (result == RESULT_CODE_OK)
          {
            /* Get parameter (data to send to L1 */
            
            if (getExtendedString ( commandBuffer_p,
               mcalContext_p->data_str,
               MAX_MCAL_DATA_STR_LEN,
               &strLength) == TRUE)
            {
               /* string length can be 0, or multiple of 2 */
               if (!((strLength == 0) ||
                      (strLength == ((strLength >> 1) << 1))))
               {
                   result = VG_CME_INVALID_INPUT_VALUE;
               }
               else
               {
                   data_ptr = mcalContext_p->data_str;
               }
 
               if (result == RESULT_CODE_OK)
               {
                 /* Decode the hex string - convert to hex number */
                 (void)vgMapTEToHex (mcalContext_p->data,
                                     mcalContext_p->length,
                                     data_ptr,
                                     strLength,
                                     VG_AT_CSCS_HEX);

                  /* Send request to IDC */
                  vgSigN1CdIdcTestReq (entity);

                  result = RESULT_CODE_PROCEEDING;                
               }
            }
            else
            {
               result = VG_CME_INVALID_INPUT_VALUE;
            }
          }
        }
        else
        {
          result = VG_CME_OPERATION_NOT_ALLOWED;
        }
        
        break;
      }

      case EXTENDED_QUERY:  /* AT*IDCTEST? */
      case EXTENDED_RANGE:  /* AT*IDCTEST=? */
      {
        /* Do nothing */
        break;
      }

      case EXTENDED_ACTION: /* AT*IDCTEST */
      default:
      {
        result = RESULT_CODE_ERROR;
        break;
      }
    }
    return (result);
}


/****************************/
/* RF calibration commands  */
/****************************/

/*--------------------------------------------------------------------------
 *
 * Function:        vgGnMCALDEV
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     This proprietary command allows the PC to perform
 *                  calibration of L1 for NB-IOT by putting the modem in to
 *                  or out of CALDEV mode.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgGnMCALDEV(CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t            result                    = RESULT_CODE_OK;
    GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
    VgMnvmMcalContext       *vgMnvmMcalContext_p      = &(generalGenericContext_p->vgMnvmMcalContext);
    Int32                   param;
    SimLockContext_t        *simLockContext_p        = ptrToSimLockContext (entity);
    VgCFUNData       *vgCFUNData_p;
    
    FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
    
    vgCFUNData_p = &(simLockContext_p->vgCFUNData);

    switch( getExtendedOperation(commandBuffer_p) )
    {
      case EXTENDED_RANGE:  /* AT*MCALDEV=? */
      {
        vgPutNewLine (entity);
        vgPrintf (entity, (const Char*)"*MCALDEV: (%d-%d)",
                          VG_CALDEV_DISABLED,
                          VG_CALDEV_ENABLED);
        vgPutNewLine (entity);
        break;
      }

      case EXTENDED_QUERY:  /* AT*MCALDEV? */
      {
        vgPutNewLine (entity);
        vgPrintf (entity, (const Char*)"*MCALDEV: %d",
                          vgMnvmMcalContext_p->vgCaldevStatus);
        vgPutNewLine (entity);
        break;
      }
        
      case EXTENDED_ASSIGN: /* AT*MCALDEV=*/
      {
        /* Get State */
        if(getExtendedParameter ( commandBuffer_p,
                                    &param,
                                    ULONG_MAX) == TRUE)
        {
            if ((param != ULONG_MAX) && (param < VG_NUM_CALDEV_STATUS))
              
            {
#if defined (MTK_NBIOT_TARGET_BUILD)
                if(vgCFUNData_p->powerUpProtoStack)
                {
                     return VG_CME_OPERATION_NOT_ALLOWED;
                }
#endif
                if(vgMnvmMcalContext_p->vgCaldevStatus != param)
                {
                    vgMnvmMcalContext_p->vgCaldevStatus = (VgCaldevStatus)param;
                    result = RESULT_CODE_OK;
                }
                else
                {
                    return RESULT_CODE_OK;
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

        if (result == RESULT_CODE_OK)
        {
          if (vgMnvmMcalContext_p->vgCaldevStatus == VG_CALDEV_ENABLED)
          {
            /* Send signal To L1 */
            vgSigN1CdEnterReq (entity);
            result = RESULT_CODE_PROCEEDING;

          }
          else
          {
            /* Send signal To L1 */
            vgSigN1CdExitReq (entity);
            result = RESULT_CODE_PROCEEDING;
          }


        }
        break;
      }

      case EXTENDED_ACTION: /* AT*MCALDEV */
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
 * Function:        vgGnMCAL
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     This proprietary command allows the PC to perform
 *                  calibration of L1 for NB-IOT
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgGnMCAL(CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t            result                       = RESULT_CODE_OK;
    GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
    VgMcalContext           *mcalContext_p            = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMcalContext);
    Char                    nrfStr[MCAL_MAX_NRF_STR_SIZE + NULL_TERMINATOR_LENGTH] = {0};
    Int16                   strLength;
    Char                    *data_ptr;

    switch( getExtendedOperation(commandBuffer_p) )
    {
      case EXTENDED_RANGE:  /* AT*MCAL=? */
      {
        vgPutNewLine (entity);
        vgPrintf (entity, (const Char*)"*MCAL: (\"%s\"),(%d-%d),(%d-%d),(%d-%d)",
                          MCAL_NRF_STR,
                          MCAL_MIN_TOKEN,
                          MCAL_MAX_TOKEN,
                          MCAL_MIN_COMMAND,
                          MCAL_MAX_COMMAND,
                          MCAL_MIN_LENGTH,
                          MAX_MCAL_DATA_SIZE_BYTES);
        vgPutNewLine (entity);
        break;
      }

      case EXTENDED_ASSIGN: /* AT*MCAL=*/
      {
        /* If not in CALDEV state then not permitted */
        if (generalGenericContext_p->vgMnvmMcalContext.vgCaldevStatus == VG_CALDEV_ENABLED)
        {
          initMcalContextData(mcalContext_p);

          /* Get "NRF" string - not actually used */
          if (getExtendedString ( commandBuffer_p,
              nrfStr,
              MCAL_MAX_NRF_STR_SIZE,
              &strLength) == TRUE)
          {
              if ((strLength != MCAL_MAX_NRF_STR_SIZE) || (strcmp((char *)nrfStr,MCAL_NRF_STR) != 0))
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
            /* Get token */
            if(getExtendedParameter ( commandBuffer_p,
                                        &mcalContext_p->token,
                                        ULONG_MAX) == TRUE)
            {
                if (mcalContext_p->token != ULONG_MAX)
                  
                {
                    if (mcalContext_p->token > MCAL_MAX_TOKEN)
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
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

          if (result == RESULT_CODE_OK)
          {
            /* Get command */
            if(getExtendedParameter ( commandBuffer_p,
                                        &mcalContext_p->command,
                                        ULONG_MAX) == TRUE)
            {
                if (mcalContext_p->command != ULONG_MAX)
                  
                {
                    if (mcalContext_p->command > MCAL_MAX_COMMAND)
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
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

          if (result == RESULT_CODE_OK)
          {
            /* Get length */
            if(getExtendedParameter ( commandBuffer_p,
                                        &mcalContext_p->length,
                                        ULONG_MAX) == TRUE)
            {
                if (mcalContext_p->length != ULONG_MAX)
                  
                {
                    if ((mcalContext_p->length > MAX_MCAL_DATA_SIZE_BYTES) || (mcalContext_p->length > MAX_N1CD_RF_TEST_REQ_PARAM_LENGTH))
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
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

          if (result == RESULT_CODE_OK)
          {
            /* Get parameter (data to send to L1 */
            
            if (getExtendedString ( commandBuffer_p,
               mcalContext_p->data_str,
               MAX_MCAL_DATA_STR_LEN,
               &strLength) == TRUE)
            {
               /* string length can be 0,2, or multiple of 2+1 */
               if (!((strLength == 0) ||
                      (strLength == 2) ||
                      (strLength == (((strLength >> 1) << 1) + 1))))
               {
                   result = VG_CME_INVALID_INPUT_VALUE;
               }
               else
               {
                   if ((strLength != 2) &&
                       (strLength != ((mcalContext_p->length*2) + 1)))
                   {
                     result = VG_CME_INVALID_INPUT_VALUE;
                   }
                   else
                   {
                     /* Check if we have a first char of endian setting */
                     if (strLength == 2)
                     {
                       data_ptr = mcalContext_p->data_str;
                     }
                     else if (*mcalContext_p->data_str == MCAL_LITTLE_ENDIAN_CHAR)
                     {
                       data_ptr = mcalContext_p->data_str + 1;
                       strLength--;
                     }
                     else
                     {
                       result = VG_CME_INVALID_INPUT_VALUE;
                     }
                   }
                }

                if (result == RESULT_CODE_OK)
                {
                  /* Decode the hex string - convert to hex number */
                  (void)vgMapTEToHex (mcalContext_p->data,
                                      mcalContext_p->length,
                                      data_ptr,
                                      strLength,
                                      VG_AT_CSCS_HEX);

                  vgSigN1CdRfTestReq (entity);

                  result = RESULT_CODE_PROCEEDING;
                
               }
            }
            else
            {
               result = VG_CME_INVALID_INPUT_VALUE;
            }
          }
        }
        else
        {
          result = VG_CME_OPERATION_NOT_ALLOWED;
        }
        
        break;
      }

      case EXTENDED_QUERY:  /* AT*MCAL? */
      case EXTENDED_ACTION: /* AT*MCAL */
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
* Function:    vgGnMICCID
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MICCID which displays the ICCID.
*-------------------------------------------------------------------------*/

ResultCode_t vgGnMICCID (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t             result = RESULT_CODE_OK;
  ExtendedOperation_t      operation = getExtendedOperation (commandBuffer_p);
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
  VgSimInfo                *simInfo = &(simLockGenericContext_p->simInfo);
  Int8                     index;
  Int8                     displayDigit;
  Int8                     val8 = 0;

  switch (operation)
  {
    case EXTENDED_QUERY:       /* AT*MIDDID? */
    case EXTENDED_ASSIGN:      /* AT*MIDDID= */
    case EXTENDED_RANGE:       /* AT*MIDDID=? */
    {
      result = RESULT_CODE_ERROR;
      break;
    }
    case EXTENDED_ACTION:      /* AT*MICCID */
    {
      /* if SIM is not ready so return reason */
      if (simLockGenericContext_p->simState != VG_SIM_READY)
      {
        result = vgGetSimCmeErrorCode ();
      }
      else
      {
        vgPutNewLine (entity);

        vgPrintf (entity,
                  (const Char *)"*MICCID: ");

        for ( index = 0; index < SIM_ICCID_LEN; index++)
        {
          val8 = simInfo->iccid.data[index];

//          printf ("Index: %d, Value %d", index, val8);

          displayDigit = val8 & 0x0F;
          if (displayDigit < 0x0A)
          {
            vgPutc (entity, (Char) (displayDigit + '0'));
          }
          else
          {
            vgPutc (entity, (Char) (displayDigit + '7'));
          }

          displayDigit = (val8 >> 4) & 0x0F;
          if (displayDigit < 0x0A)
          {
            vgPutc (entity, (Char) (displayDigit + '0'));
          }
          else
          {
            vgPutc (entity, (Char) (displayDigit + '7'));
          }
        }
        vgPutNewLine (entity);
      }
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

/*--------------------------------------------------------------------------
*
* Function:    vgGnMHOMENW
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MHOMENW which displays the home
*              network information extracted from the IMSI.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMHOMENW (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  ResultCode_t             result = RESULT_CODE_OK;
  ExtendedOperation_t      operation = getExtendedOperation (commandBuffer_p);
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
  VgSimInfo                *simInfo = &(simLockGenericContext_p->simInfo);

  PlmnName                 plmnName;
  
  switch (operation)
  {
    case EXTENDED_ACTION:      /* AT*MHOMENW */
    {
      /* if SIM is not ready so return reason */
      if (simLockGenericContext_p->simState != VG_SIM_READY)
      {
        result = vgGetSimCmeErrorCode ();
      }
      else
      {
        /* Lookup the network name information using the MNC/MCC */
        utmmLookupPlmnName(&simInfo->hplmn, &plmnName);

        vgPutNewLine (entity);

        vgPrintf (entity,
                  (const Char *)"*MHOMENW: ");

        /* Long alphabetic name */
        vgPutc (entity, '\"');
        if ( plmnName.plmnCoding == PLMN_CODING_DEFAULT)
        {
            vgPrintf (entity,
                      (const Char *)"%s",
                      plmnName.full);
        }
        vgPutc (entity, '\"');
        vgPutc (entity, ',');
        
        /* Short alphabetic name */
        vgPutc(entity, '\"');
        if (plmnName.plmnCoding == PLMN_CODING_DEFAULT)
        {
            vgPrintf (entity,
                      (const Char *)"%s",
                      plmnName.abbr);
        }
        vgPutc (entity, '\"');

        /* MCC & MNC operator code */
        /* check if MNC is 2 or 3 digits long */
        if (simInfo->hplmn.mncThreeDigitsDecoding)
        {
            vgPrintf (entity,
                      (const Char *)",\"%03x%03x\"",
                      simInfo->hplmn.plmn.mcc,
                      simInfo->hplmn.plmn.mnc);
        }
        else
        {
            vgPrintf (entity,
                      (const Char *)",\"%03x%02x\"",
                      simInfo->hplmn.plmn.mcc,
                      simInfo->hplmn.plmn.mnc);
        }

        vgPutNewLine (entity);
      }
      break;
    }
    case EXTENDED_QUERY:       /* AT*MHOMENW? */
    case EXTENDED_ASSIGN:      /* AT*MHOMENW= */
    case EXTENDED_RANGE:       /* AT*MHOMENW=? */
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
* Function:    vgGnMSPCHSC
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT*MSPCHSC command is used to select new or old 
*                   scrambling code for NPCSCH.
*
*-------------------------------------------------------------------------*/
ResultCode_t vgGnMSPCHSC  (CommandLine_t* commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
    ResultCode_t        result    = RESULT_CODE_OK;
    ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
    Int32               param;
    Boolean             curNpcschSymbolRotationMode;

    switch (operation)
    {
    case EXTENDED_QUERY:       /* AT*MSPCHSC? */
        {
            /*querying scrambling algorithm for NPCSCH*/
            curNpcschSymbolRotationMode = vgCiReadSymbolRotationModeNvram();

            vgPutNewLine(entity);
            vgPrintf(entity, (const Char*)"*MSPCHSC: %d", curNpcschSymbolRotationMode);
            vgPutNewLine(entity);
            setResultCode (entity, RESULT_CODE_OK);
        }
        break;
    case EXTENDED_RANGE:       /* AT*MSPCHSC=? */
        {
          vgPutNewLine (entity);
          vgPrintf (entity, (const Char*)"*MSPCHSC: (%d-%d)",
                            PROF_MSPCHSC_OLD_SCRAMBLE, PROF_MSPCHSC_NEW_SCRAMBLE);
          vgPutNewLine (entity);
          setResultCode (entity, RESULT_CODE_OK);
        }
        break;
    case EXTENDED_ASSIGN:      /* AT*MSPCHSC= */
        {
            if (FALSE == getExtendedParameter (commandBuffer_p, &param, ULONG_MAX))
            {
              result = RESULT_CODE_ERROR;
            }
            else if ((PROF_MSPCHSC_NEW_SCRAMBLE == param) || (PROF_MSPCHSC_OLD_SCRAMBLE == param))
            {
                /*updated NVRAM, NPCSCH should select new or old scrambling*/
                if (FALSE == vgCiWriteSymbolRotationModeNvram(param))
                {
                    result = RESULT_CODE_ERROR;
                }
                else
                {
                    result = RESULT_CODE_OK;
                }
            }
            else
            {
              result = RESULT_CODE_ERROR;
            }
            setResultCode (entity, result);
        }
        break;
    case EXTENDED_ACTION:      /* AT*MSPCHSC */
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
 * Function:        vgGnMN1DEBUG
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     This proprietary command allows testing of L1
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgGnMN1DEBUG(CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t            result                       = RESULT_CODE_OK;
    GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
    VgMcalContext           *mcalContext_p            = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMcalContext);
    Int16                   strLength;
    Char                    *data_ptr;

    switch( getExtendedOperation(commandBuffer_p) )
    {
      case EXTENDED_RANGE:  /* AT*MN1DEBUG=? */
      case EXTENDED_QUERY:  /* AT*MN1DEBUG? */
      {
        /* Do nothing - just display OK */
        break;
      }

      case EXTENDED_ASSIGN: /* AT*MN1DEBUG=*/
      {
        initMcalContextData(mcalContext_p);

        /* Get token */
        if(getExtendedParameter ( commandBuffer_p,
                                    &mcalContext_p->token,
                                    ULONG_MAX) == TRUE)
        {
            if (mcalContext_p->token != ULONG_MAX)
              
            {
                if (mcalContext_p->token > MCAL_MAX_TOKEN)
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
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

        if (result == RESULT_CODE_OK)
        {
          /* Get command */
          if(getExtendedParameter ( commandBuffer_p,
                                      &mcalContext_p->command,
                                      ULONG_MAX) == TRUE)
          {
              if (mcalContext_p->command != ULONG_MAX)
                
              {
                  if (mcalContext_p->command > MCAL_MAX_COMMAND)
                  {
                      result = VG_CME_INVALID_INPUT_VALUE;
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

        if (result == RESULT_CODE_OK)
        {
          /* Get length */
          if(getExtendedParameter ( commandBuffer_p,
                                      &mcalContext_p->length,
                                      ULONG_MAX) == TRUE)
          {
              if (mcalContext_p->length != ULONG_MAX)
                
              {
                  if ((mcalContext_p->length > MAX_MCAL_DATA_SIZE_BYTES) || (mcalContext_p->length > MAX_N1TST_ALG_TUNING_SET_REQ_PARAM_LENGTH))
                  {
                      result = VG_CME_INVALID_INPUT_VALUE;
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

        if (result == RESULT_CODE_OK)
        {
          /* Get parameter (data to send to L1 */
          
          if (getExtendedString ( commandBuffer_p,
             mcalContext_p->data_str,
             MAX_N1TST_ALG_TUNING_SET_REQ_PARAM_LENGTH*2,
             &strLength) == TRUE)
          {
             /* string length can be 0, or multiple of 2 */
             if (!((strLength == 0) ||
                    (strLength == ((strLength >> 1) << 1))))
             {
                 result = VG_CME_INVALID_INPUT_VALUE;
             }
             else
             {
               data_ptr = mcalContext_p->data_str;
             }
 
             if (result == RESULT_CODE_OK)
             {
               /* Decode the hex string - convert to hex number */
               (void)vgMapTEToHex (mcalContext_p->data,
                                   mcalContext_p->length,
                                   data_ptr,
                                   strLength,
                                   VG_AT_CSCS_HEX);
 
               vgSigN1TstAlgTuningSetReq (entity);
 
               result = RESULT_CODE_OK;
             
            }
          }
          else
          {
             result = VG_CME_INVALID_INPUT_VALUE;
          }
        }
        
        break;
      }

      case EXTENDED_ACTION: /* AT*MN1DEBUG */
      default:
      {
        result = RESULT_CODE_ERROR;
        break;
      }
    }
    return (result);
}

#if defined (MTK_TOOL_AUTHENTICATION_ENABLE) && defined (ENABLE_ATCI_UNIT_TEST)
/***********************************************************************/
/* For unit testing ATCI NVRAM Authentication                          */
/***********************************************************************/
/**
 * @brief     This function is used to check the tool authentication required in this connection.
 * @param[out] response   is the response after checking.
 * @return
 *                #TOOL_AUTH_STATUS_OK, if the operation completed successfully. \n
 *                #TOOL_AUTH_STATUS_ERROR, if an unknown error occurred. \n
 */
tool_auth_status_t tool_auth_is_required(tool_auth_response_t *response)
{
  *response = TOOL_AUTH_REQUIRED;

  return (TOOL_AUTH_STATUS_OK);
}

/**
 * @brief     This function is used to verify the correctness of certificate passed from tool side.
 * @param[in] certificate_body   is the data of this certificate.
 * @param[in] certificate_length   is the length of this certificate.
 * @param[out] verify_result   is the result after the certificate checking.
 * @return
 *                #TOOL_AUTH_STATUS_OK, if the operation completed successfully. \n
 *                #TOOL_AUTH_STATUS_ERROR, if an unknown error occurred. \n
 */
tool_auth_status_t tool_auth_verify_certificate(uint8_t *certificate_body,
        uint16_t certificate_length,
        tool_auth_verify_result_t *verify_result)
{
  uint16_t i;
  Boolean error_detected = FALSE;

  printf ("DMNV Authentication req: %d; %d,%d,%d,%d", certificate_length,
          certificate_body[0],
          certificate_body[1],
          certificate_body[2],
          certificate_body[3]);

  for (i=0; i<certificate_length; i++)
  { 
    if (certificate_body[i] != (uint8_t)i)
    {
      error_detected = TRUE;
      break;
    }
  }

  if (error_detected)
  {
    *verify_result = TOOL_AUTH_VERIFY_FAIL;
  }
  else
  {
    *verify_result = TOOL_AUTH_VERIFY_PASS;
  }  

  return (TOOL_AUTH_STATUS_OK);
}

tool_auth_status_t tool_auth_get_uid(uint8_t *uid, uint32_t uid_len)
{
    uid[0] = 0;
	
    return TOOL_AUTH_STATUS_OK;
}

#endif
/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

