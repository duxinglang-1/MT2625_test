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
 * Handler for Sim Lock AT Commands and related signals
 **************************************************************************/

#define MODULE_NAME "RVSLSS"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <cici_sig.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvslss.h>
#include <rvslut.h>
#include <rvslsigi.h>
#include <rvslsigo.h>
#include <rvcrerr.h>
#include <vgmx_sig.h>
#include <rvsspars.h>
#include <rvnvram.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  CiRunAtCommandInd ciRunAtCommandInd;
  Anrm2ReadDataCnf  anrm2ReadDataCnf;
};

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* AT command lookup table */
static const AtCmdControl slAtCommandTable[] =
{
    { ATCI_CONST_CHAR_STR "+CFUN",     vgSlCFUN,           VG_AT_SL_CFUN,      AT_CMD_ACCESS_NONE},
    { ATCI_CONST_CHAR_STR "+CMAR",     vgSlCMAR,           VG_AT_SL_CMAR,      AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
    { ATCI_CONST_CHAR_STR "+CPIN",     vgSlCPIN,           VG_AT_SL_CPIN,      AT_CMD_ACCESS_NONE},
    { ATCI_CONST_CHAR_STR "*MSIMINS",  vgSlMSIMINS,        VG_AT_SL_MSIMINS,   AT_CMD_ACCESS_NONE},
#if defined (ENABLE_DUAL_SIM_SOLUTION)
    { ATCI_CONST_CHAR_STR "*MSIMHSEL", vgSlMSIMHSEL,       VG_AT_SL_MSIMHSEL,  AT_CMD_ACCESS_NONE},
    { ATCI_CONST_CHAR_STR "+CSUS",     vgSlCSUS,           VG_AT_SL_CSUS,      AT_CMD_ACCESS_NONE},
#endif /* ENABLE_DUAL_SIM_SOLUTION */
    { ATCI_CONST_CHAR_STR "+CSIM",     vgSlCSIM,           VG_AT_SL_CSIM,      AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
    { ATCI_CONST_CHAR_STR "+CRSM",     vgSlCRSM,           VG_AT_SL_CRSM,      AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
    { ATCI_CONST_CHAR_STR "*MUPIN",    vgSlMUPIN,          VG_AT_SL_MUPIN,     AT_CMD_ACCESS_SIM_PRESENT},
    { ATCI_CONST_CHAR_STR "*MUAPP",    vgSlMuapp,          VG_AT_SL_MUAPP,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
    { ATCI_CONST_CHAR_STR "^CARDMODE", vgSlCardmode,       VG_AT_SL_CARDMODE,  AT_CMD_ACCESS_SIM_PRESENT},
    { ATCI_CONST_CHAR_STR "DP",        vgParseSsString,    VG_AT_CC_D,         AT_CMD_ACCESS_CFUN_7},
    { ATCI_CONST_CHAR_STR "DT",        vgParseSsString,    VG_AT_CC_D,         AT_CMD_ACCESS_CFUN_7},
    { ATCI_CONST_CHAR_STR "D",         vgParseSsString,    VG_AT_CC_D,         AT_CMD_ACCESS_CFUN_7},
    { ATCI_CONST_CHAR_STR "*MSST",     vgSlMSST,           VG_AT_SL_MSST,      AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
    { ATCI_CONST_CHAR_STR "*MGID",     vgSlMGID,           VG_AT_SL_MGID,      AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
#if defined(FEA_TEMP_INCLUDE_SIM_EMULATION) || defined (FEA_TEMP_ESIM_EMULATION)
    { ATCI_CONST_CHAR_STR "*MEMUSIM",  vgSlMEMSIM,         VG_AT_GN_MEMUSIM,   AT_CMD_ACCESS_NONE},
#endif
    { ATCI_CONST_CHAR_STR "*MCSIMLOCK",vgSlCsimLock,       VG_AT_SL_MCSIMLOCK, AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
#if defined (SIM_EMULATION_ON)
    { ATCI_CONST_CHAR_STR "*MUSIMEMUW",vgSlMUSIMEMUW,      VG_AT_SL_MUSIMEMUW, AT_CMD_ACCESS_NONE},
#endif /* SIM_EMULATION_ON */
    { ATCI_CONST_CHAR_STR "+CPINR",    vgSlCPINR,          VG_AT_SL_CPINR,     AT_CMD_ACCESS_SIM_PRESENT},
    { ATCI_CONST_CHAR_STR "+CNUM",     vgGnCNUM,           VG_AT_GN_CNUM,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
    { ATCI_CONST_CHAR_STR "+CCHO",     vgGnCCHO,           VG_AT_GN_CCHO,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
    { ATCI_CONST_CHAR_STR "+CCHC",     vgGnCCHC,           VG_AT_GN_CCHC,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
    { ATCI_CONST_CHAR_STR "+CGLA",     vgGnCGLA,           VG_AT_GN_CGLA,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
    { ATCI_CONST_CHAR_STR "+CRLA",     vgGnCRLA,           VG_AT_GN_CRLA,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
#if defined(FEA_SIM_IMSI_LOCK_CONTROL)
    { ATCI_CONST_CHAR_STR "*MIMSILOCK",  vgSlMIMSILOCK,    VG_AT_SL_MIMSILOCK,   AT_CMD_ACCESS_NONE},
#endif

    { PNULL,                    PNULL,              VG_AT_LAST_CODE,    AT_CMD_ACCESS_NONE}
};

/* Signal handler lookup table..... */
static const SignalCnfFunc signalFunc[] =
{
  { SIG_APEX_SIM_APP_STARTED_IND,       vgSigApexSimAppStartedInd    },
  { SIG_APEX_SIM_APP_STOPPED_IND,       vgSigApexSimAppStoppedInd    },
#if defined(FEA_TEMP_INCLUDE_SIM_EMULATION) || defined (FEA_TEMP_ESIM_EMULATION)
  { SIG_APEX_SIM_EMUSIM_CNF,            vgSigApexEmuSimCnf           },
#endif
  { APEX_SIM_LIST_PNN_CNF,              vgSigApexListPnnCnf          },
  { APEX_SIM_LIST_OPL_CNF,              vgSigApexListOplCnf          },
  { SIG_APEX_SIM_CHV_FUNCTION_IND,      vgSigApexSimChvFunctionInd   },
  { SIG_APEX_PM_MODE_CHANGE_CNF,        vgSigApexPmModeChangeCnf     },
#if defined (ENABLE_DUAL_SIM_SOLUTION)
  { SIG_APEX_SIM_SELECT_CNF,            vgSigApexSimSelectCnf        },
#endif
  { SIG_APEX_SIM_CSIM_LOCK_CNF,         vgSigApexSimCsimLockCnf      },
  { SIG_APEX_SIM_CSIM_LOCK_IND,         vgSigApexSimCsimLockInd      },
  { SIG_APEX_SIM_READ_DIR_CNF,          vgSigApexSimReadDirCnf       },
  { SIG_APEX_SIM_USIM_APP_START_CNF,    vgSigApexSimUsimAppStartCnf  },
  { SIG_APEX_SIM_GEN_ACCESS_CNF,        vgSigApexSimGenAccessCnf     },
  { APEX_SIM_LOGICAL_CHANNEL_ACCESS_CNF,vgSigApexSimLogicalChannelAccessCnf},
  { SIG_APEX_SIM_RAW_APDU_ACCESS_CNF,   vgSigApexSimRawApduAccessCnf },
  { APEX_SIM_READ_SIM_PARAM_CNF,        vgSigApexSimReadSimParamCnf  },
  { SIG_APEX_SIM_READ_MSISDN_CNF,       vgSigApexSimReadMsisdnCnf    }
#if defined (SIM_EMULATION_ON)
 ,{ SIG_ALSI_WRITE_USIM_EMU_FILE_CNF,   vgSigAlsiWriteUsimEmuFileCnf }
#endif /* SIM_EMULATION_ON */
 ,{ APEX_SIM_OPEN_LOGICAL_CHANNEL_CNF,  vgSigApexSimOpenLogicalChannelCnf}
 ,{ APEX_SIM_CLOSE_LOGICAL_CHANNEL_CNF, vgSigApexSimCloseLogicalChannelCnf}

};

#define NUM_SS_SIG_FUNC (sizeof(signalFunc) / sizeof(SignalCnfFunc))

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static void initialiseSlssGenericData (void);

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

 /****************************************************************************
 *
 * Function:    initialiseSlssGenericData
 *
 * Parameters:  none
 *
 * Returns:     Nothing
 *
 * Description: initiates sim lock sub-system entity Generic data
 *
 ****************************************************************************/

static void initialiseSlssGenericData (void)
{
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();

  /* SIM information */
  simLockGenericContext_p->simLocked = TRUE;
  simLockGenericContext_p->simEmulate = FALSE;
  simLockGenericContext_p->simBlocked = FALSE;
  simLockGenericContext_p->simInfo.pinEnabled                       = FALSE;
  simLockGenericContext_p->simInfo.pinNumRemainingRetrys            = 0;
  simLockGenericContext_p->simInfo.pin2NumRemainingRetrys           = 0;
  simLockGenericContext_p->simInfo.unblockPinNumRemainingRetrys     = 0;
  simLockGenericContext_p->simInfo.unblockPin2NumRemainingRetrys    = 0;
  simLockGenericContext_p->simInfo.pin2Verified                     = FALSE;
  simLockGenericContext_p->simInfo.phase                            = SIM_PHASE_UNKNOWN;
  memset(&(simLockGenericContext_p->simInfo.pin1Status), 0x00, sizeof(SimUiccPinStatus));
  memset(&(simLockGenericContext_p->simInfo.pin2Status), 0x00, sizeof(SimUiccPinStatus));
  memset(&(simLockGenericContext_p->simInfo.universalPinStatus), 0x00, sizeof(SimUiccPinStatus));
  memset(&(simLockGenericContext_p->simInfo.unblockPin1Status), 0x00, sizeof(SimUiccPinStatus));
  memset(&(simLockGenericContext_p->simInfo.unblockPin2Status), 0x00, sizeof(SimUiccPinStatus));
  memset(&(simLockGenericContext_p->simInfo.unblockUniversalPinStatus), 0x00, sizeof(SimUiccPinStatus));
  memset (&simLockGenericContext_p->simInfo.imsi, 0, sizeof(Imsi));

  simLockGenericContext_p->simInfo.iccid.data[0]                    = NULL_CHAR;

#if defined (FEA_PHONEBOOK)
  simLockGenericContext_p->simInfo.fdnIsEnabled                     = FALSE;
  simLockGenericContext_p->simInfo.bdnIsEnabled                     = FALSE;
#endif
  simLockGenericContext_p->simInfo.disablePin1Allowed               = FALSE;
#if defined (UPGRADE_3G)
  simLockGenericContext_p->simInfo.plmnModeBitSetToZero             = FALSE;
#endif
#if defined (ENABLE_DUAL_SIM_SOLUTION)
  simLockGenericContext_p->simInfo.numSimHolders                    = 0;
  simLockGenericContext_p->simInfo.currentSimHolder                 = SIM_0;
#endif
  simLockGenericContext_p->activatedAidIndex                        = 0;
  simLockGenericContext_p->simState                                 = VG_SIM_NOT_READY;
  simLockGenericContext_p->simInsertedState                         = VG_SIM_INSERTED_STATE_UNKNOWN;
  simLockGenericContext_p->simRejected                              = FALSE;
  simLockGenericContext_p->simWrong                                 = FALSE;
  simLockGenericContext_p->simPoweredUp                             = FALSE;
  simLockGenericContext_p->awaitingChvRsp                           = FALSE;
  simLockGenericContext_p->chvNum                                   = SIM_CHV_1;

  /* Default +CFUN settings to FULL functionality.... */
  simLockGenericContext_p->powerUpProtoStack                        = FALSE;
  simLockGenericContext_p->powerUpSim                               = FALSE;

  memset (simLockGenericContext_p->pinCode,
           NULL_CHAR,
            SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH);

  memset (simLockGenericContext_p->pukCode,
           NULL_CHAR,
            SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH);
}

 /****************************************************************************
 *
 * Function:    initialiseSlssEntitySpecificData
 *
 * Parameters:  none
 *
 * Returns:     Nothing
 *
 * Description: initiates sim lock sub-system entity specific
 *              data
 *
 ****************************************************************************/

void initialiseSlssEntitySpecificData (const VgmuxChannelNumber entity)
{
  SimLockContext_t        *simLockContext_p;

  /* initialise general context data for all entities */
  simLockContext_p = ptrToSimLockContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
  simLockContext_p->vgCFUNData.powerUpSim        = TRUE;
  simLockContext_p->vgCFUNData.powerUpProtoStack = TRUE;
  simLockContext_p->vgCFUNData.resetNow          = TRUE;
  simLockContext_p->vgCFUNData.application       = VG_AT_CFUN_APPLY_NOW;
#if defined (COARSE_TIMER)
  simLockContext_p->vgCFUNData.functionality     = 0xFF;
#endif

  simLockContext_p->simGenAccess.length     = 0;
  simLockContext_p->simGenAccess.efId       = SIM_EF_INVALID;
  simLockContext_p->simGenAccess.dirId      = SIM_DIR_INVALID;
  simLockContext_p->simGenAccess.rootDirId  = SIM_DIR_INVALID;
  simLockContext_p->simGenAccess.commandRef = VGMUX_CHANNEL_INVALID;

  simLockContext_p->csimLocked = FALSE;

  simLockContext_p->vgUiccLogicChannelData.dfNameLength = 0;
  simLockContext_p->vgUiccLogicChannelData.sessionId    = 0;

  simLockContext_p->simLogicalChannelAccess.length = 0;
  simLockContext_p->simLogicalChannelAccess.sessionId = 0;
}


/***************************************************************************
 * Global Functions
 ***************************************************************************/

 /****************************************************************************
 *
 * Function:    vgSlssInterfaceController
 *
 * Parameters:  SignalBuffer       - structure containing incoming signal
 *              VgmuxChannelNumber - mux channel number
 *
 * Returns:     Boolean - indicates whether the sub-system has recognised and
 *                        procssed the signal given.
 *
 * Description: determines action for received signals
 *
 ****************************************************************************/

Boolean vgSlssInterfaceController (const SignalBuffer *signal_p,
                                    const VgmuxChannelNumber entity)
{
  Boolean                   accepted = FALSE;
  Int8                      aloop;

  /* Signal Handler */

  switch (*signal_p->type)
  {
    case SIG_APEX_SIM_OK_IND:
    {
      vgSigApexSimOkInd (signal_p, entity);
      break;
      /* Not accepted since another sub-system also processes the signal */
    }
    case SIG_APEX_SIM_NOK_IND:
    {
      vgSigApexSimNokInd (signal_p, entity);
      /* Not accepted since another sub-system also processes the signal */
      break;
    }
    case SIG_APEX_SIM_APP_STARTED_IND:
    {
      vgSigApexSimAppStartedInd (signal_p, entity);
      /* Not accepted since another sub-system also processes the signal */
      break;
    }
    case SIG_APEX_SIM_GET_PIN_IND:
    {
      vgSigApexSimGetPinInd (signal_p, entity);
      /* Not accepted since another sub-system also processes the signal */
      break;
    }
    case SIG_APEX_SIM_GET_CHV_IND:
    {
      vgSigApexSimGetChvInd (signal_p, entity);
      /* Not accepted since another sub-system also processes the signal */
      break;
    }
    case SIG_APEX_SIM_FAULT_IND:
    {
      vgSigApexSimFaultInd (signal_p);
      accepted = TRUE;
      break;
    }
    case SIG_APEX_PM_POWER_GOING_DOWN_IND:
    {
      vgSigApexPmPowerGoingDownRsp ();
      accepted = TRUE;
      break;
    }
    case SIG_CI_RUN_AT_COMMAND_IND:
    {
      accepted = parseCommandBuffer (slAtCommandTable, entity);
      break;
    }
    case SIG_INITIALISE:
    {
      initialiseSlssGenericData ();
      break;
    }

    case SIG_CIMUX_CHANNEL_ENABLE_IND:
    {
      initialiseSlssEntitySpecificData (entity);
      break;
    }

    default:
    {
      for (aloop = 0; (aloop < NUM_SS_SIG_FUNC) && (accepted == FALSE); aloop++)
      {
        if (signalFunc[aloop].signalId == (*signal_p->type))
        {
          (signalFunc[aloop].procFunc)(signal_p, entity);
          accepted = TRUE;
        }
      }
      break;
    }
  }

  return (accepted);

}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

