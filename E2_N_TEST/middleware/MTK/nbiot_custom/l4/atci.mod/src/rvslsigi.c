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
 * Incoming signal handlers for the Sim lock Services Sub-System.
 **************************************************************************/

#define MODULE_NAME "RVSLSIGI"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <stdio.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvslsigi.h>
#include <rvslsigo.h>
#include <absi_sig.h>
#include <rvchman.h>
#include <dmpm_sig.h>
#include <rvcrhand.h>
#include <rvcrerr.h>
#include <rvslut.h>
#include <rvpfsigo.h>
#include <rvmmut.h>
#include <rvmspars.h>
#include <rvmmsigo.h>
#include <rvcimxut.h>
#include <rvgnsigi.h>
#include <rvgnut.h>
#include <rvsleep.h>
#include <gkisig.h>
#include <gkimem.h>
#include <psc_api.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
    ApexSimAppStartedInd               apexSimAppStartedInd;
    ApexSimAppStoppedInd               apexSimAppStoppedInd;
    ApexSimGetPinInd                   apexSimGetPinInd;
    ApexSimResetAccCallTimerCnf        apexSimResetAccCallTimerCnf;
    ApexSimAccCallTimerChangedInd      apexSimAccCallTimerChangedInd;
    ApexSimGetChvInd                   apexSimGetChvInd;
    ApexSimChvFunctionInd              apexSimChvFunctionInd;
    ApexSimOkInd                       apexSimOkInd;
    ApexSimNokInd                      apexSimNokInd;
    ApexSimFaultInd                    apexSimFaultInd;
    ApexPmModeChangeCnf                apexPmModeChangeCnf;
    ApexSimGenAccessCnf                apexSimGenAccessCnf;
    ApexSimLogicalChannelAccessCnf     apexSimLogicalChannelAccessCnf;
    ApexSimRawApduAccessCnf            apexSimRawApduAccessCnf;
    ApexSimReadSimParamCnf             apexSimReadSimParamCnf;
    CiCfunSetInd                       ciCfunSetInd;
    ApexSimReadMbiCnf                  apexSimReadMbiCnf;
    ApexSimWriteMbiCnf                 apexSimWriteMbiCnf;
    ApexSimListPnnCnf                  apexSimListPnnCnf;
    ApexSimListOplCnf                  apexSimListOplCnf;
#if defined (ENABLE_DUAL_SIM_SOLUTION)
    ApexSimSelectCnf                   apexSimSelectCnf;
#endif
    ApexSimCsimLockCnf                 apexSimCsimLockCnf;
    ApexSimCsimLockInd                 apexSimCsimLockInd;
    ApexSimReadDirCnf                  apexSimReadDirCnf;
    ApexTerminateSessionCnf            apexTerminateSessionCnf;
    ApexSimUsimAppStartCnf             apexSimUsimAppStartCnf;
    ApexSimReadMsisdnCnf               apexSimReadMsisdnCnf;
#if defined (SIM_EMULATION_ON)
    AlsiWriteUsimEmuFileCnf            alsiWriteUsimEmuFileCnf;
#endif /* SIM_EMULATION_ON */
    ApexSimOpenLogicalChannelCnf       apexSimOpenLogicalChannelCnf;
    ApexSimCloseLogicalChannelCnf      apexSimCloseLogicalChannelCnf;
};


/***************************************************************************
 * Manifest Constants
 ***************************************************************************/
extern  const Mcc                 utmm3DigitsMncCountries[];

#define UICC_FCP_TEMPLATE_TAG                    0x62
#define UICC_FILE_SIZE_OBJECT_TAG                0x80
#define UICC_FILE_SIZE_OBJECT_SIZE               2
#define MAX_PLMN_SEL_RECORD_LENGTH               5  /*EF PLMNwACT , EF OPLMNwACT, EF HPLMNwACT*/
#define MIN_PLMN_SEL_RECORD_LENGTH               3  /*EF PLMN SEL only*/

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static void outputCommandDataInHex (const VgmuxChannelNumber entity,
                                   const Int8 *data, Int16 length);

static void vgMuappProcessSimReadDirCnf (    const ApexSimReadDirCnf *sig_p,
                                            SimLockContext_t        *simLockContext_p,
                                            const VgmuxChannelNumber entity);

static void vgMuappProcessUsimAppStartCnf (  const ApexSimUsimAppStartCnf    *sig_p,
                                            SimLockContext_t                *simLockContext_p,
                                            const VgmuxChannelNumber        entity);


/***************************************************************************
 * Type Definitions
 ***************************************************************************/



/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*----------------------------------------------------------------------------
 *
 * Function:    vgDecodeTLVDataLength
 *
 * Parameters:  Int8 *dataLgth
 *              Int8 *data
 *
 * Returns:     on how many bytes the length is encoded, can be 1 or 2
 *
 * Description: returns on how many bytes the length is encoded in the TLV object,
 *
 *----------------------------------------------------------------------------*/

static Int8 vgDecodeTLVDataLength ( Int8 *dataLgth,
                                    Int8 *data)
{
    Int8    lengthSize = 0;

    if ( (*data) <= 0x7F)
    {
        *dataLgth = *data;
        lengthSize = 1;
    }
    else
    {
        *dataLgth = 0;
        if ( (*data) == 0x81)
        {
            if ( *(data+1) >= 0x80 )
            {
                *dataLgth = *(data+1);  /*this is the length of the data to follow in the TLV*/
            }

            lengthSize = 2;
        }
    }

    /* returns the number of bytes the length is stored on  */
    return (lengthSize);
}

/*----------------------------------------------------------------------------
 *
 * Function:    vgGetFileSizeFromFcpTemplate
 *
 * Parameters:  pointer to ApexSimGenAccessCnf signal
 *
 * Returns:     the file Size
 *
 * Description: extracts the file size from the FCP template returned by the USIM.
 *              See 102.221 for decoding of the FCP template
 *
 *----------------------------------------------------------------------------*/
static Int16 vgGetFileSizeFromFcpTemplate (ApexSimGenAccessCnf *apexSimGenAccessCnf_p)
{
    Int8 fcpLength =0;                   /* can be one or two bytes */
    Int8 offset = 0;
    Boolean exit = FALSE;
    Int8 fcpOffset;
    Int8 tvlLength, tvlOffset;
    Int16 fileSize = 0;

    /*------------------------------------------------------------
    * Decode Fcp template  (see TS 102 221 section 11.1.1.3.1)
    * ----------------------------------------------------------*/
    if ( apexSimGenAccessCnf_p->command[offset++] == UICC_FCP_TEMPLATE_TAG)          /* Fcp template tag */
    {
      /*work out whether the FCP length is encoded on 1 or 2 bytes*/
      fcpOffset = vgDecodeTLVDataLength (&fcpLength, &apexSimGenAccessCnf_p->command[offset]);

      /*fcp offset can be 1 or 2*/
      if (fcpOffset != 0)
      {
        offset += fcpOffset;
        while ((!exit)&& (offset <= (fcpLength + fcpOffset)) )
        {
          if (apexSimGenAccessCnf_p->command[offset] == UICC_FILE_SIZE_OBJECT_TAG)
          {
             if (apexSimGenAccessCnf_p->command[offset+1] == UICC_FILE_SIZE_OBJECT_SIZE) /*should always be 2 bytes*/
             {
                 fileSize = ((((Int16)apexSimGenAccessCnf_p->command[offset+2]) << 8) | (apexSimGenAccessCnf_p->command[offset+3]));
             }
             exit = TRUE;
          }
          else
          {
             /* Just ignore TLV object we don't know*/
             offset++;
             tvlOffset =
             vgDecodeTLVDataLength (&tvlLength, &apexSimGenAccessCnf_p->command[offset]);
             /* length of tvl object can be coded on 1 or 2 bytes */
             offset += tvlOffset + tvlLength;
          }
        }
      }
    }
    return (fileSize);
}

/*----------------------------------------------------------------------------
 *
 * Function:    outputCommandDataInHex
 *
 * Parameters:  entity, pointer to int8 array and length
 *
 * Returns:     Nothing
 *
 * Description: Used by vgSigApexSimGenAccessCnf to display the command
 *              data response from the SIM.
 *
 *----------------------------------------------------------------------------*/
static void outputCommandDataInHex (const VgmuxChannelNumber entity,
                                   const Int8 *data, Int16 length)
{
  Int16 loop;
  vgPrintf (entity, (const Char*)"\"");
  for (loop = 0; loop < length; loop++)
  {
    vgPrintf (entity, (const Char*)"%02X", data[loop]);
  }
  vgPrintf (entity, (const Char*)"\"");
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimAppStartedInd
 *
 * Parameters:  inSig - Pointer to signal of type ApexAppStartedInd
 *
 * Returns:     Nothing
 *
 * Description: Indicates that a GSM or USIM application has been started, can expect GET CHV/PIN,
 *              SIM OK and SIM NOK Ind signals next
 *-------------------------------------------------------------------------*/

void vgSigApexSimAppStartedInd (const SignalBuffer *signalBuffer,
                                const VgmuxChannelNumber entity)
{
    ApexSimAppStartedInd    *sig_p = &signalBuffer->sig->apexSimAppStartedInd;
    SimLockGenericContext_t *simLockGenericContext_p   = ptrToSimLockGenericContext ();
    VgSimInfo               *simInfo = &(simLockGenericContext_p->simInfo);
    VgMuappEvent            muappEvent;
    PARAMETER_NOT_USED (entity);

    if (sig_p->status == SIM_INSERTED_OK)
    {
        vgSetSimInsertionState (VG_SIM_INSERTED);
    }

    simLockGenericContext_p->activatedAidIndex  = sig_p->aidRecordNum;
    simLockGenericContext_p->powerUpSim         = TRUE;

    simInfo->plmnModeBitSetToZero           = FALSE;
    simInfo->cardIsUicc                     = sig_p->cardIsUicc;
    simInfo->phase                          = sig_p->phase;
    simInfo->pin1KeyRef                     = sig_p->pin1KeyRef;
    simInfo->pin2KeyRef                     = sig_p->pin2KeyRef;
    simInfo->verifyUniversalPin             = sig_p->verifyUniversalPin;
    simInfo->universalPinSupportedByCard    = sig_p->universalPinSupportedByCard;
    simInfo->oci                            = FALSE;
    simInfo->ici                            = FALSE;
    simInfo->userPlmnSelector               = FALSE;
    simInfo->hplmnSelector                  = FALSE;
    simInfo->operatorPlmnSelector           = FALSE;
    simInfo->plmnSelector                   = FALSE;
    muappEvent.index    = sig_p->aidRecordNum;
    muappEvent.state    = VG_MUAPP_APP_RUNNING;
    muappEvent.aid      = sig_p->applicationLab;
    vgMuappPrintEvent ( &muappEvent);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexGetPinInd
 *
 * Parameters:  inSig - Pointer to signal of type ApexGetPinInd
 *
 * Returns:     Nothing
 *
 * Description: A request from the Background for a PIN code.  We set the
 *              system up such that no commands can run until a valid PIN
 *              has been entered using the CPIN command.
 *-------------------------------------------------------------------------*/
void vgSigApexSimGetPinInd (const SignalBuffer *signalBuffer,
                            const VgmuxChannelNumber entity)
{
  ApexSimGetPinInd        *sig_p = &signalBuffer->sig->apexSimGetPinInd;
  SimLockGenericContext_t *simLockGenericContext_p   = ptrToSimLockGenericContext ();
  VgSimInfo               *simInfo = &(simLockGenericContext_p->simInfo);


  if ((getCommandId (entity) == VG_AT_SL_CPIN) ||
      (getCommandId (entity) == VG_AT_SL_CMAR) ||
      (getCommandId (entity) == VG_AT_SL_MUPIN))
  {
    /* user entered wrong password */
    setResultCode (entity, VG_CME_INCORRECT_PASSWORD);
  }

  /*only expect to receive this for checking the application PIN or the universal PIN*/
  FatalCheck(((sig_p->pinKeyReference == simInfo->pin1KeyRef) ||
           (sig_p->pinKeyReference == USIM_ACCESS_UNIVERSAL_PIN )),
           sig_p->pinKeyReference,
           simInfo->pin1KeyRef, 0 );

  if (sig_p->pinBlocked == TRUE)
  {
    /* application PIN (PIN1) or universal PIN is blocked */
    vgSetSimState (VG_SIM_PUK);
  }
  else
  {
    /* application PIN (PIN1) or universal PIN is required */
    vgSetSimState (VG_SIM_PIN);
  }

  simLockGenericContext_p->awaitingChvRsp = TRUE;
  simLockGenericContext_p->simPoweredUp   = TRUE;

  /* copy SIM information over */
  simInfo->pinEnabled = sig_p->pinStatus.enabled;
  simInfo->pinNumRemainingRetrys = sig_p->pinStatus.numRemainingRetrys;
  simInfo->unblockPinNumRemainingRetrys = sig_p->unblockPinStatus.numRemainingRetrys;

  if (sig_p->pinKeyReference == USIM_ACCESS_UNIVERSAL_PIN)
  {
    simInfo->universalPinStatus = sig_p->pinStatus;
    simInfo->unblockUniversalPinStatus = sig_p->unblockPinStatus;
  }
  else
  {
    simInfo->pin1Status = sig_p->pinStatus;
    simInfo->unblockPin1Status = sig_p->unblockPinStatus;
  }

}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexGetChvInd
 *
 * Parameters:  inSig - Pointer to signal of type ApexGetChvInd
 *
 * Returns:     Nothing
 *
 * Description: A request from the Background for a PIN code.  We set the
 *              system up such that no commands can run until a valid PIN
 *              has been entered using the CPIN command.
 *-------------------------------------------------------------------------*/
void vgSigApexSimGetChvInd (const SignalBuffer *signalBuffer,
                            const VgmuxChannelNumber entity)
{
  ApexSimGetChvInd        *sig_p = &signalBuffer->sig->apexSimGetChvInd;
  SimLockGenericContext_t *simLockGenericContext_p   = ptrToSimLockGenericContext ();
  VgSimInfo               *simInfo = &(simLockGenericContext_p->simInfo);


  if ((getCommandId (entity) == VG_AT_SL_CPIN) ||
      (getCommandId (entity) == VG_AT_SL_CMAR) ||
      (getCommandId (entity) == VG_AT_SL_MUPIN))
  {
    /* user entered wrong password */
    setResultCode (entity, VG_CME_INCORRECT_PASSWORD);
  }

  /* only ever expect to get requests for CHV1 */
  FatalAssert (sig_p->chvNum == SIM_CHV_1);

  if (sig_p->chvBlocked == TRUE)
  {
    /* CHV1 is blocked */
    vgSetSimState (VG_SIM_PUK);
  }
  else
  {
    /* CHV1 requires password */
    vgSetSimState (VG_SIM_PIN);
  }

  simLockGenericContext_p->awaitingChvRsp = TRUE;
  simLockGenericContext_p->simPoweredUp   = TRUE;

  /* copy SIM information over */

  simInfo->pinEnabled = sig_p->chv1Enabled;

  simInfo->pinNumRemainingRetrys = sig_p->chv1Status.numRemainingRetrys;
  simInfo->pin2NumRemainingRetrys = sig_p->chv2Status.numRemainingRetrys;

  simInfo->unblockPinNumRemainingRetrys = sig_p->unblockChv1Status.numRemainingRetrys;
  simInfo->unblockPin2NumRemainingRetrys = sig_p->unblockChv2Status.numRemainingRetrys;


}

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimChvFunctionInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_SIM_CHV_FUNCTION_IND
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Update information on remaining retries for pin/puk codes
 *----------------------------------------------------------------------------*/

void vgSigApexSimChvFunctionInd (const SignalBuffer *signalBuffer,
                                  const VgmuxChannelNumber entity)
{
  ApexSimChvFunctionInd   *sig_p = &signalBuffer->sig->apexSimChvFunctionInd;
  SimLockGenericContext_t *simLockGenericContext_p  = ptrToSimLockGenericContext ();

  PARAMETER_NOT_USED (entity);

  if (sig_p->chvNum == SIM_CHV_1)
  {
    simLockGenericContext_p->simInfo.pinNumRemainingRetrys        = sig_p->chvStatus.numRemainingRetrys;
    simLockGenericContext_p->simInfo.unblockPinNumRemainingRetrys = sig_p->unblockChvStatus.numRemainingRetrys;
    if (simLockGenericContext_p->simInfo.unblockPinNumRemainingRetrys == 0)
    {
      vgSetSimState (VG_SIM_NOT_READY);
    }
  }
  else
  {
    simLockGenericContext_p->simInfo.pin2NumRemainingRetrys        = sig_p->chvStatus.numRemainingRetrys;
    simLockGenericContext_p->simInfo.unblockPin2NumRemainingRetrys = sig_p->unblockChvStatus.numRemainingRetrys;
    if (sig_p->chvFunctionSuccess == TRUE)
    {
      /* only go to READY state if previous state was PIN2/PUK2 */
      if ((simLockGenericContext_p->simState == VG_SIM_PIN2) ||
          (simLockGenericContext_p->simState == VG_SIM_PUK2))
      {
        vgSetSimState (VG_SIM_READY);
      }
    }
  }
}


/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexPmModeChangeCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_PM_MODE_CHANGE_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Checks to see if mode change was success, initiated by the
 *              AT+CFUN and AT+COPS commands. In the case of of AT+COPS
 *              command it may re-register to the network.
 *----------------------------------------------------------------------------*/

void vgSigApexPmModeChangeCnf (const SignalBuffer *signalBuffer,
                                const VgmuxChannelNumber entity)
{
  ApexPmModeChangeCnf     *sig_p                    = &signalBuffer->sig->apexPmModeChangeCnf;
  SignalBuffer             cfunInfoSig               = kiNullBuffer;
  CiCfunSetInd             *ciCfunSetInd_p           = PNULL;
  SimLockGenericContext_t *simLockGenericContext_p  = ptrToSimLockGenericContext ();
  SimLockContext_t        *simLockContext_p         = ptrToSimLockContext (entity);
  VgCFUNData              *vgCFUNData_p             = &(simLockContext_p->vgCFUNData);
#if defined (UPGRADE_3G)
  MobilityContext_t       *mobilityContext_p         = ptrToMobilityContext ();
  VgCOPSData              *vgCOPSData_p              = &mobilityContext_p->vgCOPSData;
  ChManagerContext_t      *chManagerContext_p        =  ptrToChManagerContext ();
#endif

  EntityMobilityContext_t *entityMobilityContext_p   = PNULL;
  VgmuxChannelNumber      profileEntity              = 0;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (simLockContext_p != PNULL, entity, 0, 0);
#endif
  switch (getCommandId (entity))
  {
    case VG_AT_SL_CFUN:
    {
      if (sig_p->result == TRUE)
      {
        /* check if further processing required */
        switch (vgCFUNData_p->application)
        {
          case VG_AT_CFUN_APPLY_ON_RESET:
          {
            setResultCode (entity, RESULT_CODE_OK);
            break;
          }
          case VG_AT_CFUN_APPLY_ON_RESET_AND_NOW:
          {
            /* stored new settings, need to apply now also */
            vgCFUNData_p->application = VG_AT_CFUN_APPLY_NOW;
            vgCFUNData_p->resetNow    = TRUE;

            setResultCode (entity,
                vgChManContinueAction (entity, SIG_APEX_PM_MODE_CHANGE_REQ));
            break;
          }
          case VG_AT_CFUN_APPLY_NOW:
          {
            if ((TRUE == sig_p->postPowerUpSim) || (TRUE == sig_p->postPowerUpStack))
            {
              vgChManContinueAction (entity, SIG_APEX_PM_MODE_CHANGE_REQ);
            }
            else
            {
              /* update current settings */
              simLockGenericContext_p->powerUpSim
                       = vgCFUNData_p->powerUpSim;
              simLockGenericContext_p->powerUpProtoStack
                       = vgCFUNData_p->powerUpProtoStack;

              if(simLockGenericContext_p->powerUpSim == FALSE && 
                 simLockGenericContext_p->powerUpProtoStack == FALSE)
              {
                psc_set_power_state(PSC_PWR_STATE_INACTIVE,TRUE);  
              }

              /* send signal internally to CI to inform other sub-systems of the modified
                 * SIM state */
              KiCreateZeroSignal ( SIG_CI_CFUN_SET_IND, sizeof (CiCfunSetInd), &cfunInfoSig);
              ciCfunSetInd_p = (CiCfunSetInd *)cfunInfoSig.sig;
              ciCfunSetInd_p->entity = entity;
              ciCfunSetInd_p->cfunMode = vgGetCFUNMode ();
              KiSendSignal (VG_CI_TASK_ID, &cfunInfoSig);

              /* force sim state to not ready since we've just powered down
               * the SIM */
              if (!simLockGenericContext_p->powerUpSim)
              {
                vgSetSimState (VG_SIM_NOT_READY);
              }
              /* Request the Multiplexer top resend its low power vote since */
              /* layer 1 has just lost it */

              /* we need to update the CNMI state and Message Service again
               * since the protocol stack has lost it! */
              vgApexSendSmProfileChangedReq (entity);

              /* set final result code */
              setResultCode (entity, RESULT_CODE_OK);

#if defined  (WL1_UPHY_STUB)
extern void U1IhCfgApplicationLayerPsUnplugged(Boolean psUnplugged );
              if(ciCfunSetInd_p->cfunMode == 0 )
              {
                U1IhCfgApplicationLayerPsUnplugged( TRUE );
              }
              else
              {
                 U1IhCfgApplicationLayerPsUnplugged( FALSE );

              }
#endif  /* WL1_UPHY_STUB */
              if(ciCfunSetInd_p->cfunMode == 0)
              {
                mobilityContext_p->vgEdrxData.userDataValid = FALSE;
                mobilityContext_p->vgEdrxData.nwDataValid = FALSE;
              }


#if defined (UPGRADE_3G)
              /*ENS requirement*/
              if  ((simLockGenericContext_p->simInfo.plmnModeBitSetToZero) &&
                   (vgCOPSData_p->mode == VG_OP_MANUAL_OPERATOR_SELECTION))
              {
                 vgCOPSData_p->returnToRplmn = TRUE;
                 /*force change control*/
                 chManagerContext_p->isImmediate = TRUE;
#if defined (USE_ABAPP)
                 mobilityContext_p->isImmediate = chManagerContext_p->isImmediate;
#endif
                 vgCOPSData_p->mode = VG_OP_AUTOMATIC_MODE;

                 /* If PLMN mode bit set to 0, and the ME is in manual mode,
                  * return to automatic mode */
                 vgChManContinueAction (entity,
                                        SIG_APEX_MM_PLMN_SELECT_REQ);
              }
#endif
            }
            break;
          }
          default:
          {
            /* Invalid application value */
            FatalParam(entity, getCommandId (entity), vgCFUNData_p->application);
//            setResultCode (entity, RESULT_CODE_ERROR);
            break;
          }
        }
      }
      else
      {
        setResultCode (entity, RESULT_CODE_ERROR);
      }
      break;
    }
    default:
    {
      /*lint -save -e666 disables warning relating to passing a function to macro */
      /* It is ok to disable this warning as 'getCommandId' does not alter value */
      /* Unexpected command with Mode Change */
     // WarnParam(entity, getCommandId (entity), 0);
      /*lint -restore */
      break;
    }
  }

  /* fixed bug: G1 Mantis123324 */
  if(sig_p->protoStackState ==FALSE)
  {
     for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
     {
       if (isEntityActive (profileEntity))
       {
             entityMobilityContext_p  = ptrToEntityMobilityContext (profileEntity);
             if(entityMobilityContext_p != PNULL)
             {
               entityMobilityContext_p->msqnState.lastSentLevel    = VG_CESQ_INVALID_RXLEV;
               entityMobilityContext_p->msqnState.lastSentRsrq      = VG_CESQ_INVALID_RSRQ;
               entityMobilityContext_p->msqnState.lastSentRsrp      = VG_CESQ_INVALID_RSRP;
             }
       }
     }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimOkInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_SIM_OK_IND
 *
 * Returns:     Nothing
 *
 * Description: Updates the SIM insertion status i.e. it is inserted
 *-------------------------------------------------------------------------*/

void vgSigApexSimOkInd (const SignalBuffer *signalBuffer,
                         const VgmuxChannelNumber entity)
{
    ApexSimOkInd             *sig_p = &signalBuffer->sig->apexSimOkInd;
    SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
    VgSimInfo                *simInfo = &(simLockGenericContext_p->simInfo);
    SleepManContext_t        *sleepManContext_p  = ptrToSleepManContext();


    /* if CPIN is running set result code */
    if( (getCommandId (entity) == VG_AT_SL_CPIN) ||
        (getCommandId (entity) == VG_AT_SL_MUPIN))
    {
        setResultCode(entity, RESULT_CODE_OK);
    }

    /* If CMAR command is running perform master reset.... */
    else if (getCommandId (entity) == VG_AT_SL_CMAR)
    {
        setResultCode(entity, vgSlMasterReset(entity));
    }

    /* Update local copy of SIM parameters.... */
    vgSetSimState (VG_SIM_READY);
    simLockGenericContext_p->simInfo.phase = sig_p->phase;
    simLockGenericContext_p->awaitingChvRsp = FALSE;
    simLockGenericContext_p->simRejected    = FALSE;
    simLockGenericContext_p->simWrong       = FALSE;
    simLockGenericContext_p->simPoweredUp   = TRUE;

#if defined (FEA_PHONEBOOK)
    simInfo->fdnIsEnabled       = sig_p->fdnIsEnabled;
    simInfo->bdnIsEnabled       = sig_p->bdnIsEnabled;
#endif
#if defined (ENABLE_DUAL_SIM_SOLUTION)
    simInfo->currentSimHolder   = sig_p->simHolder;
    simInfo->numSimHolders      = sig_p->numSimHolders;
#endif

    simInfo->pinEnabled        = sig_p->pin1Status.enabled;
    simInfo->pinNumRemainingRetrys = sig_p->pin1Status.numRemainingRetrys;
    simInfo->pin2NumRemainingRetrys = sig_p->pin2Status.numRemainingRetrys;
    simInfo->unblockPinNumRemainingRetrys = sig_p->unblockPin1Status.numRemainingRetrys;
    simInfo->unblockPin2NumRemainingRetrys = sig_p->unblockPin2Status.numRemainingRetrys;
    simInfo->pin1KeyRef = sig_p->pin1KeyReference;
    simInfo->pin2KeyRef = sig_p->pin2KeyReference;

    if (sig_p->phase == SIM_PHASE_3G)
    {
        simInfo->disablePin1Allowed = TRUE;     /*always allowed on UICCs*/
    }
    else
    {
        simInfo->disablePin1Allowed = sig_p->serviceTable.simService.chv1Disable.activated;
    }
    simInfo->pin1Status = sig_p->pin1Status;
    simInfo->pin2Status = sig_p->pin2Status;
    simInfo->universalPinStatus = sig_p->universalPinStatus;
    simInfo->unblockPin1Status = sig_p->unblockPin1Status;
    simInfo->unblockPin2Status = sig_p->unblockPin2Status;
    simInfo->unblockUniversalPinStatus = sig_p->unblockUniversalPinStatus;
    simInfo->imsi = sig_p->imsi;
    simInfo->hplmn.plmn.mcc = vgMmGetMccFromImsi(&sig_p->imsi);
    {
        Int8    i = 0;
        Boolean mncThreeDigitsDecoding = FALSE;

        while (( mncThreeDigitsDecoding == FALSE) && (utmm3DigitsMncCountries[i] != 0))
        {
            if (simInfo->hplmn.plmn.mcc == utmm3DigitsMncCountries[i])
            {
                mncThreeDigitsDecoding = TRUE;
            }
            else
            {
                i++;
            }
        }

        simInfo->hplmn.plmn.mnc = vgMmGetMncFromImsi(mncThreeDigitsDecoding,&sig_p->imsi);

        /* Remember that is is a 3 digit MNC */
        simInfo->hplmn.mncThreeDigitsDecoding = mncThreeDigitsDecoding;
    }

    simInfo->iccid.data[0]      = sig_p->iccid.data[0];
    simInfo->iccid.data[1]      = sig_p->iccid.data[1];
    simInfo->iccid.data[2]      = sig_p->iccid.data[2];
    simInfo->iccid.data[3]      = sig_p->iccid.data[3];
    simInfo->iccid.data[4]      = sig_p->iccid.data[4];
    simInfo->iccid.data[5]      = sig_p->iccid.data[5];
    simInfo->iccid.data[6]      = sig_p->iccid.data[6];
    simInfo->iccid.data[7]      = sig_p->iccid.data[7];
    simInfo->iccid.data[8]      = sig_p->iccid.data[8];
    simInfo->iccid.data[9]      = sig_p->iccid.data[9];

    if (sig_p->phase == SIM_PHASE_3G)
    {
        /*3G SIMs*/
        simInfo->userPlmnSelector     = sig_p->serviceTable.usimService.userPlmnSelector;
        simInfo->hplmnSelector        = sig_p->serviceTable.usimService.hplmnSelector;
        simInfo->operatorPlmnSelector = sig_p->serviceTable.usimService.operatorPlmnSelector;
        /*USIMs don' t have PLMN SEL file so simInfo->plmnSelector should always be FALSE*/
        simInfo->oci                  = sig_p->serviceTable.usimService.oci;
        simInfo->ici                  = sig_p->serviceTable.usimService.ici;
    }
    else
    {
        /*2G SIMs*/
        /*EF PLMN SEL*/
        if( ( sig_p->serviceTable.simService.plmnSelector.activated) &&
            ( sig_p->serviceTable.simService.plmnSelector.allocated))
        {
            simInfo->plmnSelector  = TRUE;
        }
        /*EF PLMN W ACT*/
        if( ( sig_p->serviceTable.simService.userPlmnSelector.activated) &&
            ( sig_p->serviceTable.simService.userPlmnSelector.allocated))
        {
            simInfo->userPlmnSelector  = TRUE;
        }
        /*EF HPLMN W ACT*/
        if( ( sig_p->serviceTable.simService.hplmnSelector.activated) &&
            ( sig_p->serviceTable.simService.hplmnSelector.allocated))
        {
            simInfo->hplmnSelector  = TRUE;
        }
        /*EF OPLMN W ACT*/
        if( ( sig_p->serviceTable.simService.operatorPlmnSelector.activated) &&
            ( sig_p->serviceTable.simService.operatorPlmnSelector.allocated))
        {
            simInfo->operatorPlmnSelector  = TRUE;
        }
    }
    simInfo->serviceTableType = sig_p->serviceTableType;
    memcpy(&(simInfo->serviceTable), &(sig_p->serviceTable), sizeof(ServiceTable));

    sleepManContext_p->needSimOk = FALSE;
    RvWakeUpCompleteCheck();
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimNokInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_SIM_NOK_IND
 *
 * Returns:     Nothing
 *
 * Description: Updates the SIM insertion status i.e. it is not inserted
 *-------------------------------------------------------------------------*/

void vgSigApexSimNokInd (const SignalBuffer *signalBuffer,
                          const VgmuxChannelNumber entity)
{
  ApexSimNokInd           *sig_p                    = &signalBuffer->sig->apexSimNokInd;
  SimLockGenericContext_t *simLockGenericContext_p  = ptrToSimLockGenericContext ();
  StkEntityGenericData_t  *stkGenericContext_p      = ptrToStkGenericContext ();
  VgSimInfo               *simInfo                  = &(simLockGenericContext_p->simInfo);
  Boolean pinCodeRemoved  = FALSE;
  VgSimState newSimState  = VG_SIM_NOT_READY;
  SleepManContext_t        *sleepManContext_p  = ptrToSleepManContext();

  switch (sig_p->reason)
  {
    case SIM_NOK_INSERTED_ERROR:
    case SIM_NOK_REMOVED:
    case SIM_NOK_INVALID_STATUS_RSP:
    {
      vgSetSimInsertionState (VG_SIM_NOT_INSERTED);
      break;
    }
    case SIM_NOK_POWER_OFF:
    {
      simLockGenericContext_p->simPoweredUp = FALSE;
      vgSetSimInsertionState (VG_SIM_INSERTED_STATE_UNKNOWN);
      break;
    }
    /* Job # 121081 Allow STK and Phone Book operations in case of Network reject */
    case SIM_NOK_NETWORK_REJECT:
    case SIM_NOK_IMSI_FAILED:
    {
      newSimState = VG_SIM_READY;
      simLockGenericContext_p->simRejected = TRUE;
      break;
    }
    /* job108530: handle SIM_NOK_CHV1_UNBLOCK_BLOCKED */
    case SIM_NOK_CHV1_UNBLOCK_BLOCKED:
    {
      simLockGenericContext_p->simRejected = TRUE;
      vgIndicatePukBlocked();
      break;
    }
    case SIM_NOK_MEP_CHECK_FAILED:
    {
      simLockGenericContext_p->simWrong    = TRUE;
      pinCodeRemoved = TRUE;
      vgSetSimInsertionState (VG_SIM_INSERTED);
      break;
    }
    case SIM_NOK_CHV1_BLOCKED:
    {
      newSimState = VG_SIM_PUK;
      break;
    }
    case SIM_NOK_INITIALISE_FAILED:
    case SIM_NOK_UNKNOWN:
    case SIM_NOK_GENERAL_FAULT:
    case SIM_NOK_SM_FAULT:
# if !defined (UPGRADE_3G)
    case SIM_NOK_GSM_ACCESS_UNAVAIL:
# endif
    case SIM_NOK_NOT_VALID_FOR_LTE:
    {
      vgSetSimInsertionState (VG_SIM_INSERTED_STATE_UNKNOWN);
      /* expected reasons which have no special processing */
      break;
    }
    case SIM_NO_APPLICATION_RUNNING:
    {
        if(simLockGenericContext_p->simPoweredUp != FALSE)
        {
            vgSetSimInsertionState (VG_SIM_INSERTED);
            newSimState = VG_SIM_NOT_READY;
        }
        break;
    }
    case SIM_NOK_FILE_NOT_FOUND:
        /*sim file not found is not a sever issue. should not assert. just continue...*/
        break;
    case SIM_NOK_REFRESH:
    case SIM_NOK_CHV2_BLOCKED:
    case SIM_NOK_CHV2_UNBLOCK_BLOCKED:
    default:
    {
      /* reasons that should never be received */
      FatalParam(entity, 0, 0);
      break;
    }
  }

  if ((sig_p->reason != SIM_NOK_NETWORK_REJECT) && (sig_p->reason != SIM_NO_APPLICATION_RUNNING))
  {
    stkGenericContext_p->dataValid = FALSE;
  }

#if defined (ENABLE_DUAL_SIM_SOLUTION)
  simInfo->currentSimHolder   = sig_p->simHolder;
  simInfo->numSimHolders      = sig_p->numSimHolders;
#endif

  simLockGenericContext_p->awaitingChvRsp  = FALSE;

  simInfo->plmnSelector = FALSE;
  simInfo->userPlmnSelector = FALSE;
  simInfo->operatorPlmnSelector = FALSE;
  simInfo->hplmnSelector = FALSE;
#if defined (UPGRADE_3G)
  simInfo->plmnModeBitSetToZero = FALSE;
#endif

  vgSetSimState (newSimState);

  /* Check if CPIN or CMAR is running */
  if ((getCommandId (entity) == VG_AT_SL_CPIN)  ||
      (getCommandId (entity) == VG_AT_SL_CMAR))
  {
    /* a PIN code was succesfully entered, but MEP check failed. */
    if (pinCodeRemoved == TRUE)
    {
      setResultCode (entity, RESULT_CODE_OK);
    }
    else
    {
      setResultCode (entity, vgGetSimCmeErrorCode());
    }
  }

  sleepManContext_p->needSimNok = FALSE;
  RvWakeUpCompleteCheck();
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimFaultInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_SIM_FAULT_IND
 * Returns:     Nothing
 *
 * Description: Handles SIM faults reported.
 *
 *-------------------------------------------------------------------------*/

void vgSigApexSimFaultInd (const SignalBuffer *signalBuffer)
{
  ApexSimFaultInd *sig_p = &signalBuffer->sig->apexSimFaultInd;

  if ((sig_p->simFault == SIM_REQ_MEMORY_PROBLEM) ||
      (sig_p->simFault == SIM_REQ_SIM_GENERAL_FAULT) ||
      (sig_p->simFault == SIM_REQ_ALLOC_ERROR) ||
      (sig_p->simFault == SIM_REQ_SM_FAULT))
  {
    /* a fatal error has occurred, so indicate SIM is not ready
     * and not inserted */
    vgSetSimInsertionState (VG_SIM_INSERTED_STATE_UNKNOWN);
    vgSetSimState (VG_SIM_NOT_READY);
  }
}



/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimGenAccessCnf(ApexSimGenAccessCnf *inSig);
 *
 * Parameters:  Pointer to a incoming ApexSimGenAccessCnf data structure
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SIM_GEN_ACCESS_CNF signal received
 *              from the background layer.
 *----------------------------------------------------------------------------*/
void vgSigApexSimGenAccessCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  const ApexSimGenAccessCnf *inSig = &(signalBuffer->sig->apexSimGenAccessCnf);
  Int16                     sw1;
  Int16                     sw2;
  Int16                     numRecords = 0;
  Int8                      recordLength;
  MobilityContext_t         *mobilityContext_p         = ptrToMobilityContext ();
  SimLockGenericContext_t   *simLockGenericContext_p   = ptrToSimLockGenericContext ();
  VgSimInfo                 *simInfo                   = &(simLockGenericContext_p->simInfo);
  Int16                      fileSize;
  VgCPLSData                *vgCPLSData_p              = &mobilityContext_p->vgCPLSData;

  if (vgMsSMSIsSimResponse (inSig->commandRef) == TRUE)
  {
    vgMsSMSMsgProcess (&inSig->command[0], (Int8)(inSig->commandRef & 0x00FF));

    vgChManReleaseControlOfElement (CC_SUBSCRIBER_IDENTITY_MODULE, entity);
  }
  else
  {
    switch (getCommandId (entity))
    {
      case VG_AT_SL_CSIM: /* Response to Voyager +CSIM command */
      {
        if (inSig->length >= STATUS_BYTES_LENGTH)
        {
          vgPutNewLine (entity);
          {
             vgPrintf (entity, (const Char*)"+CSIM: %d,", inSig->length * 2);
             outputCommandDataInHex (entity, &inSig->command[0], inSig->length);
          }

          vgPutNewLine (entity);

          if(inSig->success == TRUE)
          {
            setResultCode (entity, RESULT_CODE_OK);
          }
          else
          {
            setResultCode (entity, RESULT_CODE_ERROR);
          }
        }
        else
        {
          setResultCode (entity, RESULT_CODE_ERROR);
        }
        break;
      }

      case VG_AT_SL_CRSM: /* Response to Voyager +CRSM command */
      {
        /* Command data must contain at least two bytes; for sw1 and sw2 */
        if (inSig->length >= STATUS_BYTES_LENGTH)
        {
          /* the two status bytes are stored at the end of the command data */
          sw1 = inSig->command[inSig->length - 2];
          sw2 = inSig->command[inSig->length - 1];

          vgPutNewLine (entity);
          vgPrintf (entity, (const Char*)"+CRSM: %d,%d", sw1, sw2);

          if (inSig->length > 2)
          {
            vgPutc (entity, ',');
            outputCommandDataInHex (entity,
                                     &inSig->command[0],
                                      (Int16) (inSig->length - 2));
          }

          vgPutNewLine (entity);

          if(inSig->success == TRUE)
          {
            setResultCode (entity, RESULT_CODE_OK);
          }
          else
          {
            setResultCode (entity, RESULT_CODE_ERROR);
          }
        }
        else
        {
          /* No data returned, should be at least 2 bytes */
          setResultCode (entity, RESULT_CODE_ERROR);
        }
        break;
      }

      case VG_AT_MM_CPOL: /* Response to Voyager +CPOL command */
      {
        if ((inSig->success == TRUE) &&
            (inSig->length > STATUS_BYTES_LENGTH)) /*not just status bytes*/
        {
            /*plmns are coded using 3 bytes in EF PLMN SEL, but on 5 bytes on other preferred PLMN files */
            /*the 2 extra bytes cater for the access technology field*/
            if ((vgCPLSData_p->plmnSelector == ABMM_USER_SELECTOR) &&
                (simInfo->plmnSelector) &&
                (!simInfo->userPlmnSelector))
            {
                recordLength = MIN_PLMN_SEL_RECORD_LENGTH;
            }
            else
            {
                recordLength = MAX_PLMN_SEL_RECORD_LENGTH;
            }

            if (simInfo->cardIsUicc)
            {
              /*3G SIMs, we need to decode the FCP template */
              fileSize = vgGetFileSizeFromFcpTemplate (&signalBuffer->sig->apexSimGenAccessCnf);
              numRecords =  fileSize / recordLength;
              vgPutNewLine (entity);
              vgPrintf     (entity, (const Char*)"+CPOL: (1-%d),(0-2)", numRecords);
              vgPutNewLine (entity);
              setResultCode (entity, RESULT_CODE_OK);
            }
            else
            {
              /* response data must contain at least 15 bytes; for mandatory info */
              FatalCheck(inSig->length >= 15, inSig->length, 0, 0);

              /*GSM SIM: decode the response to the GET RESPONSE, see GSM 11.11, section 9.2.1*/
              /* read 16 bit file length */
              fileSize = ((inSig->command[2] << 8) + inSig->command[3]);
              numRecords = fileSize / recordLength;


              vgPutNewLine (entity);
              vgPrintf     (entity, (const Char*)"+CPOL: (1-%d),(0-2)", numRecords);
              vgPutNewLine (entity);

              setResultCode (entity, RESULT_CODE_OK);
            }
        }
        else
        {
          /* No data returned, should be at least 15 bytes */
          setResultCode (entity, RESULT_CODE_ERROR);
        }
      }
      break;
      default:
      {
        /*lint -save -e666 disables warning relating to passing a function to macro */
        /* It is ok to disable this warning as 'getCommandId' does not alter value */
        /* Ignore signal */
        WarnParam(entity, getCommandId (entity), 0);
        /*lint -restore */
        break;
      }
    }
  }
}

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimLogicalChannelAccessCnf(ApexSimGenAccessCnf *inSig);
 *
 * Parameters:  Pointer to a incoming ApexSimLogicalChannelAccessCnf data structure
 *
 * Returns:     Nothing
 *
 * Description: Handles the APEX_SIM_LOGICAL_CHANNEL_ACCESS_CNF signal received
 *              from the background layer.
 *----------------------------------------------------------------------------*/
void vgSigApexSimLogicalChannelAccessCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  Int16                     sw1;
  Int16                     sw2;
  const ApexSimLogicalChannelAccessCnf *inSig = &(signalBuffer->sig->apexSimLogicalChannelAccessCnf);

  switch (getCommandId (entity))
  {
    case VG_AT_GN_CGLA: /* Response to Voyager +CGLA command */
    {
      if(inSig->requestStatus == SIM_REQ_OK)
      {
        if (inSig->length >= STATUS_BYTES_LENGTH)
        {
          vgPutNewLine (entity);
          {
             vgPrintf (entity, (const Char*)"+CGLA: %d,", inSig->length * 2);
             outputCommandDataInHex (entity, &inSig->response[0], inSig->length);
          }

          vgPutNewLine (entity);

          setResultCode (entity, RESULT_CODE_OK);
        }
        else
        {
          setResultCode (entity, RESULT_CODE_ERROR);
        }
      }
      else
      {
        setResultCode (entity, RESULT_CODE_ERROR);
      }
      break;
    }

    case VG_AT_GN_CRLA: /* Response to Voyager +CRLA command */
    {
      if(inSig->requestStatus == SIM_REQ_OK)
      {
        /* Command data must contain at least two bytes; for sw1 and sw2 */
        if (inSig->length >= STATUS_BYTES_LENGTH)
        {
          /* the two status bytes are stored at the end of the command data */
          sw1 = inSig->response[inSig->length - 2];
          sw2 = inSig->response[inSig->length - 1];

          vgPutNewLine (entity);
          vgPrintf (entity, (const Char*)"+CRLA: %d,%d", sw1, sw2);

          if (inSig->length > 2)
          {
            vgPutc (entity, ',');
            outputCommandDataInHex (entity,
                                     &inSig->response[0],
                                      (Int16) (inSig->length - 2));
          }

          vgPutNewLine (entity);

          setResultCode (entity, RESULT_CODE_OK);
        }
        else
        {
          /* No data returned, should be at least 2 bytes */
          setResultCode (entity, RESULT_CODE_ERROR);
        }
      }
      else
      {
        setResultCode (entity, RESULT_CODE_ERROR);
      }
      break;
    }

    break;
    default:
    {
      /*lint -save -e666 disables warning relating to passing a function to macro */
      /* It is ok to disable this warning as 'getCommandId' does not alter value */
      /* Ignore signal */
      WarnParam(entity, getCommandId (entity), 0);
      /*lint -restore */
      break;
    }
  }

}

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimRawApduAccessCnf(pexSimSelectCnf *inSig);
 *
 * Parameters:  Pointer to a incoming ApexSimRawApduAccessCnf data structure
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SIM_RAW_APDU_ACCESS_CNF signal received
 *              from the background layer.
 *----------------------------------------------------------------------------*/
void vgSigApexSimRawApduAccessCnf (const SignalBuffer *signalBuffer,
    const VgmuxChannelNumber entity)
{
    ApexSimRawApduAccessCnf *inSig = &(signalBuffer->sig->apexSimRawApduAccessCnf);
    response rsp;

    if ((inSig->success == TRUE) && (inSig->length >= 2))
    {
        rsp.flag = 1;
        rsp.sw = (inSig->command[inSig->length - 2] << 8)
            + (inSig->command[inSig->length - 1]);
        rsp.length = inSig->length - 2;
        memcpy(rsp.res_data, inSig->command, rsp.length);
    }
    else
    {
        rsp.flag = 0;
    }
}

#if defined (ENABLE_DUAL_SIM_SOLUTION)
/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimSelectCnf(pexSimSelectCnf *inSig);
 *
 * Parameters:  Pointer to a incoming ApexSimGenAccessCnf data structure
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SIM_GEN_ACCESS_CNF signal received
 *              from the background layer.
 *----------------------------------------------------------------------------*/

void vgSigApexSimSelectCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  const ApexSimSelectCnf *inSig = &(signalBuffer->sig->apexSimSelectCnf);
  SimLockContext_t           *simLockContext_p         = ptrToSimLockContext (entity);
  SimLockGenericContext_t   *simLockGenericContext_p   = ptrToSimLockGenericContext ();
  VgSimInfo                 *simInfo                   = &(simLockGenericContext_p->simInfo);

#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (simLockContext_p != PNULL, entity, 0, 0);
#endif
  switch (getCommandId (entity))
  {
    case VG_AT_SL_MSIMHSEL: /* Response to SELECT SIM Holder command AT*MSIMHSEL */
    case VG_AT_SL_CSUS: /* Response to SELECT SIM Holder command AT+CSUS */
    {
      if ( inSig-> success != TRUE )
      {
        setResultCode (entity, RESULT_CODE_ERROR);
      }
      else
      {
        /* this should have been updated by the new ApexSimOkInd */
        if ( simInfo->currentSimHolder != simLockContext_p->simHolderToSelect)
        {
          simInfo->currentSimHolder = simLockContext_p->simHolderToSelect;
        }
        simLockGenericContext_p->powerUpSim = inSig-> powerUpSim;
        simLockGenericContext_p->powerUpProtoStack = inSig-> powerUpProtoStack;
        setResultCode (entity, RESULT_CODE_OK);
      }
      break;
    }
    default:
    {
      /* Unexpected command with ApexSimSelectCnf */
      FatalParam(entity, getCommandId (entity), 0);
      break;
    }
  }
  simLockContext_p->simHolderToSelect = SIM_UNSELECTED;
}
#endif /* ENABLE_DUAL_SIM_SOLUTION */

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimCsimLockCnf
 *
 * Parameters:  Pointer to a incoming ApexSimCsimLockCnf data structure
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SIM_CSIM_LOCK_CNF signal received
 *              from the background layer.
 *----------------------------------------------------------------------------*/

void  vgSigApexSimCsimLockCnf (const SignalBuffer *signalBuffer,
                              const VgmuxChannelNumber entity)
{
  const ApexSimCsimLockCnf  *inSig = &(signalBuffer->sig->apexSimCsimLockCnf);
  SimLockContext_t          *simLockContext_p = ptrToSimLockContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
  if ( inSig-> success != TRUE )
  {
    setResultCode (entity, RESULT_CODE_ERROR);
  }
  else
  {
    setResultCode (entity, RESULT_CODE_OK);
    simLockContext_p->csimLocked = !simLockContext_p->csimLocked;
  }
}

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimCsimLockInd
 *
 * Parameters:  Pointer to a incoming ApexSimCsimLockInd data structure
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SIM_CSIM_LOCK_IND signal received
 *              from the background layer.
 *----------------------------------------------------------------------------*/


void  vgSigApexSimCsimLockInd (const SignalBuffer *signalBuffer,
                              const VgmuxChannelNumber entity)
{
  const ApexSimCsimLockInd  *inSig = &(signalBuffer->sig->apexSimCsimLockInd);
  SimLockContext_t        *simLockContext_p = PNULL;
  VgmuxChannelNumber         cIndex    = 0;
  Boolean                    found = FALSE;

  /* find the entity that applied the lock and send the unsolicited to that */
  while ((cIndex < CI_MAX_ENTITIES) && (found == FALSE))
  {
      simLockContext_p = ptrToSimLockContext(cIndex);
      if ((simLockContext_p != PNULL) &&(simLockContext_p->csimLocked == TRUE))
      {
        found = TRUE;
      }
      else
      {
        cIndex++;
      }
  }

  FatalAssert( found == TRUE);

  FatalAssert(simLockContext_p != PNULL);

  simLockContext_p->csimLocked = inSig->locked;

  {
     if ((TRUE == isEntityActive (cIndex))
        &&(!isEntityMmiNotUnsolicited(cIndex)))
     {
       /* display unsolicited CSIMLock state message */
       vgPutNewLine (cIndex);
       vgPrintf (cIndex,
                (const Char*)"*MCSIMLOCK: %d",
                 simLockContext_p->csimLocked);
       vgPutNewLine (cIndex);
       vgFlushBuffer (cIndex);
     }
  }
}


/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimReadSimParamCnf
 *
 * Parameters:  Pointer to a incoming ApexSimReadSimParamCnf data structure
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SIM_READ_SIM_PARAM_CNF signal received
 *              from the background layer.
 *----------------------------------------------------------------------------*/
void vgSigApexSimReadSimParamCnf (const SignalBuffer *signalBuffer,
                                  const VgmuxChannelNumber entity)
{
    const ApexSimReadSimParamCnf   *inSig                   = &(signalBuffer->sig->apexSimReadSimParamCnf);

    Int8                            j                       = 0;
    SimLockGenericContext_t        *simLockGenericContext_p = ptrToSimLockGenericContext ();

    if (inSig->simParamResult == SIM_PARAM_OK)
    {
        switch (simLockGenericContext_p->simReadParamType)
        {
            case SIM_READ_PARAM_GID:
            {
                vgPutNewLine (entity);
                vgPrintf ( entity,
                          (Char *)"*MGID: %d,\"%s\",%d,\"%s\"",
                          inSig->simParam.simGid.length1,
                          inSig->simParam.simGid.gid1,
                          inSig->simParam.simGid.length2,
                          inSig->simParam.simGid.gid2);
                vgPutNewLine (entity);
                setResultCode (entity, RESULT_CODE_OK);
            }
            break;

            case SIM_READ_PARAM_SST:
            {
                vgPutNewLine (entity);
                vgPrintf ( entity, (Char *)"*MSST: %d", inSig->simParam.simSst.serviceTableType);
                if (inSig->simParam.simSst.serviceTableType == SIM_SERVICE_TABLE)
                {
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.mms.allocated;
                    j |= inSig->simParam.simSst.serviceTable.simService.extension8.allocated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.mmsUserConnectivityParam.allocated<<2;
                    vgPrintf ( entity, (Char *)",\"0%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.MExE.allocated;
                    j |= inSig->simParam.simSst.serviceTable.simService.rplmnLastUsedTechnology.allocated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.plmnNetworkName.allocated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.operatorPlmnList.allocated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.mailboxDiallingNumber.allocated;
                    j |= inSig->simParam.simSst.serviceTable.simService.messageWaitingIndStatus.allocated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.callForwardingIndStatus.allocated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.serviceProviderDisplayInfo.allocated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.ussdInCc.allocated;
                    j |= inSig->simParam.simSst.serviceTable.simService.runAtCommand.allocated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.userPlmnSelector.allocated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.operatorPlmnSelector.allocated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.hplmnSelector.allocated;
                    j |= inSig->simParam.simSst.serviceTable.simService.cpbcchInfo.allocated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.investigationScan.allocated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.eccp.allocated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.dck.allocated;
                    j |= inSig->simParam.simSst.serviceTable.simService.cnl.allocated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.smsr.allocated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.nia.allocated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.moSms.allocated;
                    j |= inSig->simParam.simSst.serviceTable.simService.gprs.allocated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.image.allocated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.solsa.allocated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.ddSmsCb.allocated;
                    j |= inSig->simParam.simSst.serviceTable.simService.ddSmsPp.allocated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.menuSelection.allocated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.cc.allocated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.proactiveSim.allocated;
                    j |= inSig->simParam.simSst.serviceTable.simService.cbmir.allocated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.bdn.allocated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.extension4.allocated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.spn.allocated;
                    j |= inSig->simParam.simSst.serviceTable.simService.sdn.allocated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.extension3.allocated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.rfu.allocated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.vgcsIdList.allocated;
                    j |= inSig->simParam.simSst.serviceTable.simService.vbsIdList.allocated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.emlpp.allocated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.aaEmlpp.allocated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.msisdn.allocated;
                    j |= inSig->simParam.simSst.serviceTable.simService.extension1.allocated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.extension2.allocated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.smsParameters.allocated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.lnd.allocated;
                    j |= inSig->simParam.simSst.serviceTable.simService.cbmi.allocated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.gid1.allocated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.gid2.allocated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.msisdn.allocated;
                    j |= inSig->simParam.simSst.serviceTable.simService.extension1.allocated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.extension2.allocated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.smsParameters.allocated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.lnd.allocated;
                    j |= inSig->simParam.simSst.serviceTable.simService.cbmi.allocated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.gid1.allocated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.gid2.allocated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.chv1Disable.allocated;
                    j |= inSig->simParam.simSst.serviceTable.simService.adn.allocated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.fdn.allocated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.smsStorage.allocated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.adviceOfCharge.allocated;
                    j |= inSig->simParam.simSst.serviceTable.simService.capabilityConfig.allocated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.plmnSelector.allocated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.calledPartySubaddress.allocated<<3;
                    vgPrintf ( entity, (Char *)"%x\"", j );

                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.mms.activated;
                    j |= inSig->simParam.simSst.serviceTable.simService.extension8.activated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.mmsUserConnectivityParam.activated<<2;
                    vgPrintf ( entity, (Char *)",\"0%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.MExE.activated;
                    j |= inSig->simParam.simSst.serviceTable.simService.rplmnLastUsedTechnology.activated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.plmnNetworkName.activated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.operatorPlmnList.activated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.mailboxDiallingNumber.activated;
                    j |= inSig->simParam.simSst.serviceTable.simService.messageWaitingIndStatus.activated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.callForwardingIndStatus.activated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.serviceProviderDisplayInfo.activated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.ussdInCc.activated;
                    j |= inSig->simParam.simSst.serviceTable.simService.runAtCommand.activated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.userPlmnSelector.activated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.operatorPlmnSelector.activated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.hplmnSelector.activated;
                    j |= inSig->simParam.simSst.serviceTable.simService.cpbcchInfo.activated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.investigationScan.activated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.eccp.activated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.dck.activated;
                    j |= inSig->simParam.simSst.serviceTable.simService.cnl.activated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.smsr.activated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.nia.activated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.moSms.activated;
                    j |= inSig->simParam.simSst.serviceTable.simService.gprs.activated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.image.activated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.solsa.activated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.ddSmsCb.activated;
                    j |= inSig->simParam.simSst.serviceTable.simService.ddSmsPp.activated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.menuSelection.activated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.cc.activated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.proactiveSim.activated;
                    j |= inSig->simParam.simSst.serviceTable.simService.cbmir.activated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.bdn.activated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.extension4.activated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.spn.activated;
                    j |= inSig->simParam.simSst.serviceTable.simService.sdn.activated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.extension3.activated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.rfu.activated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.vgcsIdList.activated;
                    j |= inSig->simParam.simSst.serviceTable.simService.vbsIdList.activated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.emlpp.activated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.aaEmlpp.activated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.msisdn.activated;
                    j |= inSig->simParam.simSst.serviceTable.simService.extension1.activated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.extension2.activated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.smsParameters.activated<<3;
                    j |= inSig->simParam.simSst.serviceTable.simService.lnd.activated<<4;
                    j |= inSig->simParam.simSst.serviceTable.simService.cbmi.activated<<5;
                    j |= inSig->simParam.simSst.serviceTable.simService.gid1.activated<<6;
                    j |= inSig->simParam.simSst.serviceTable.simService.gid2.activated<<7;
                    vgPrintf ( entity, (Char *)"%2x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.msisdn.activated;
                    j |= inSig->simParam.simSst.serviceTable.simService.extension1.activated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.extension2.activated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.smsParameters.activated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.lnd.activated;
                    j |= inSig->simParam.simSst.serviceTable.simService.cbmi.activated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.gid1.activated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.gid2.activated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.chv1Disable.activated;
                    j |= inSig->simParam.simSst.serviceTable.simService.adn.activated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.fdn.activated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.smsStorage.activated<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.simService.adviceOfCharge.activated;
                    j |= inSig->simParam.simSst.serviceTable.simService.capabilityConfig.activated<<1;
                    j |= inSig->simParam.simSst.serviceTable.simService.plmnSelector.activated<<2;
                    j |= inSig->simParam.simSst.serviceTable.simService.calledPartySubaddress.activated<<3;
                    vgPrintf ( entity, (Char *)"%x\"", j );
                }
                else
                {
                    j = 0;
#if defined (ENABLE_EHPLMN)
                    j |= inSig->simParam.simSst.serviceTable.usimService.terminalApps;
                    vgPrintf (entity, (Char *)",\"%x", j);
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.usimService.ehplmnpi;
                    j |= inSig->simParam.simSst.serviceTable.usimService.lastRplmnSelectionInd<<1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.reserved2<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.gbaLocalKey<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.usimService.vbsSecurity;
                    j |= inSig->simParam.simSst.serviceTable.usimService.wlanReauthIdent<<1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.mmsStorage<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.gba<<3;
                    vgPrintf ( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.usimService.mbmsSecurity;
                    j |= inSig->simParam.simSst.serviceTable.usimService.dataDownloadByUssd<<1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.ehplmn<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.additTermProfile<<3;
                    vgPrintf (entity, (Char *)"%x", j);
                    j = 0;
#endif /* (ENABLE_EHPLMN) */
                    j |= inSig->simParam.simSst.serviceTable.usimService.vgcsIdList;
                    j |= inSig->simParam.simSst.serviceTable.usimService.vbsIdList<<1;
#if defined (ENABLE_EHPLMN)
                    j |= inSig->simParam.simSst.serviceTable.usimService.pseudonym<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.uplmnForWlan<<3;
                    vgPrintf( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.usimService.oplmnForWlan;
                    j |= inSig->simParam.simSst.serviceTable.usimService.userWsidList<<1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.operatrWsidList<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.vgcsSecurity<<3;
#endif /* (ENABLE_EHPLMN) */
#if defined (ENABLE_EHPLMN)
                    vgPrintf(entity, (Char *)"%x", j);
#else
                    vgPrintf(entity, (Char *)"\"0%x", j);
#endif
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.usimService.cfis;
                    j |= inSig->simParam.simSst.serviceTable.usimService.reserved<<1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.serviceProviderDisplayInfo<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.mms<<3;
                    vgPrintf( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.usimService.extension8;
                    j |= inSig->simParam.simSst.serviceTable.usimService.ccByGprs<<1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.mmsUserConnectivityParam<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.nia<<3;
                    vgPrintf(entity, (Char *)"%x", j);
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.usimService.MExE;
                    j |= inSig->simParam.simSst.serviceTable.usimService.operatorPlmnSelector<<1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.hplmnSelector<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.extension5<<3;
                    vgPrintf( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.usimService.plmnNetworkName;
                    j |= inSig->simParam.simSst.serviceTable.usimService.operatorPlmnList<<1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.mbdn<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.mwis<<3;
                    vgPrintf(entity, (Char *)"%x", j);
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.usimService.notUsed;
                    j |= inSig->simParam.simSst.serviceTable.usimService.est<<1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.apn<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.dck<<3;
                    vgPrintf( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.usimService.cnl;
                    j |= inSig->simParam.simSst.serviceTable.usimService.gsmSecurityContext<<1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.cpbcchInfo<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.investigationScan<<3;
                    vgPrintf(entity, (Char *)"%x", j);
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.usimService.aaEmlpp;
                    j |= inSig->simParam.simSst.serviceTable.usimService.rfu<<1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.gsmAccess<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.ddSmsPp<<3;
                    vgPrintf( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.usimService.ddSmsCb;
                    j |= inSig->simParam.simSst.serviceTable.usimService.cc<<1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.moSms<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.runAtCommand<<3;
                    vgPrintf(entity, (Char *)"%x", j);
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.usimService.gid1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.gid2<<1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.spn<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.userPlmnSelector<<3;
                    vgPrintf( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.usimService.msisdn;
                    j |= inSig->simParam.simSst.serviceTable.usimService.img<<1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.solsa<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.emlpp<<3;
                    vgPrintf(entity, (Char *)"%x", j);
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.usimService.ici;
                    j |= inSig->simParam.simSst.serviceTable.usimService.smsStorage<<1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.smsr<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.smsParameters<<3;
                    vgPrintf( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.usimService.adviceOfCharge;
                    j |= inSig->simParam.simSst.serviceTable.usimService.capabilityConfig<<1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.cbmi<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.cbmir<<3;
                    vgPrintf(entity, (Char *)"%x", j);
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.usimService.localPhoneBook;
                    j |= inSig->simParam.simSst.serviceTable.usimService.fdn<<1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.extension2<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.sdn<<3;
                    vgPrintf( entity, (Char *)"%x", j );
                    j = 0;
                    j |= inSig->simParam.simSst.serviceTable.usimService.extension3;
                    j |= inSig->simParam.simSst.serviceTable.usimService.bdn<<1;
                    j |= inSig->simParam.simSst.serviceTable.usimService.extension4<<2;
                    j |= inSig->simParam.simSst.serviceTable.usimService.oci<<3;
                    vgPrintf(entity, (Char *)"%x\"", j);

                    j = 0;
                    j |= inSig->simParam.simSst.fdnEnabled;
                    j |= inSig->simParam.simSst.bdnEnabled<<1;
                    j |= inSig->simParam.simSst.aclEnabled<<2;
                    vgPrintf(entity, (Char *)",\"%x\"", j);
                }
                vgPutNewLine(entity);
                setResultCode (entity, RESULT_CODE_OK);
            }
            break;

            default:
            {
                /*lint -save -e666 disables warning relating to passing a function to macro */
                /* It is ok to disable this warning as 'getCommandId' does not alter value */
                /* Unexpected command */
                WarnParam(entity, simLockGenericContext_p->simReadParamType, 0);
                /*lint -restore */
            }
            break;
        }
    }
    else
    {
        switch (inSig->simParamResult)
        {
            case SIM_PARAM_GID_NOT_AVAIL:
                setResultCode (entity, VG_CME_MGID_NO_VALID_GID);
                break;
            case SIM_PARAM_INVALID_PARAM:
                setResultCode (entity, RESULT_CODE_ERROR);
                break;
            default:
                setResultCode (entity, RESULT_CODE_ERROR);
                break;
        }
    }
}

#if defined(FEA_TEMP_INCLUDE_SIM_EMULATION) || defined (FEA_TEMP_ESIM_EMULATION)
/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexEmuSimCnf
 *
 * Parameters:  Pointer to a incoming ApexSimEmuSimCnf data structure
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SIM_EMUSIM_CNF signal received
 *              from the background layer.
 *----------------------------------------------------------------------------*/
void vgSigApexEmuSimCnf (const SignalBuffer *signalBuffer,
                                 const VgmuxChannelNumber entity)
{
  setResultCode (entity, RESULT_CODE_OK);
}
#endif

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexListPnnCnf
 *
 * Parameters:  Pointer to a incoming ApexListPnnCnf data structure
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SIM_LIST_PNN_CNF signal received
 *              from the background layer.
 *----------------------------------------------------------------------------*/
void vgSigApexListPnnCnf (const SignalBuffer *signalBuffer,
                                  const VgmuxChannelNumber entity)
{
    SimLockContext_t            *simLockContext_p   = ptrToSimLockContext (entity);
    const ApexSimListPnnCnf     *inSig              = &(signalBuffer->sig->apexSimListPnnCnf);
    Int8                        i                   = 0;
    Boolean                     doPrintLoop         = FALSE;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck ( simLockContext_p != PNULL, entity, 0, 0);
#endif
    FatalCheck ( getCommandId ( entity) == VG_AT_MM_MNON,
                getCommandId ( entity), VG_AT_MM_MNON, 0);

    if (inSig->requestStatus == SIM_REQ_OK)
    {
        switch ( simLockContext_p->vgMnonData.operation)
        {
            case VG_MNON_OP_READ_ALL:
            {
                if ( simLockContext_p->startField == 1)
                {
                    vgPutNewLine (entity);
                }

                for (i=0; i< inSig->pnnData.numPnns; i++)
                {
                    vgPrintf (   entity,
                                (Char *)"*MNON: %d,\"%s\",\"%s\"",
                                inSig->pnnData.pnn[i].indexNumber,
                                inSig->pnnData.pnn[i].name.full.name,
                                inSig->pnnData.pnn[i].name.abbr.name);

                    vgPutNewLine (entity);
                }

                if ( (inSig->pnnData.numPnns < SIM_PNN_LIST_SIZE) ||
                    (simLockContext_p->startField + inSig->pnnData.numPnns > inSig->totalEntries))
                {
                    setResultCode (entity, RESULT_CODE_OK);
                }
                else
                {
                    simLockContext_p->startField = inSig->nextStartField;
                    vgChManContinueAction (entity,APEX_SIM_LIST_PNN_REQ);
                }
            }
            break;

            case VG_MNON_OP_READ_INDEX:
            {
                if ( simLockContext_p->startField > inSig->totalEntries)
                {
                    setResultCode (entity, RESULT_CODE_ERROR);
                }
                else if ( (inSig->pnnData.numPnns == 0) ||
                            (simLockContext_p->startField != inSig->pnnData.pnn[0].indexNumber))
                {
                    setResultCode (entity, RESULT_CODE_ERROR);
                }
                else
                {
                    /* Everything is OK*/
                    vgPutNewLine(entity);

                    vgPrintf( entity, (Char *)"*MNON: %d,\"%s\",\"%s\"", inSig->pnnData.pnn[0].indexNumber,
                    inSig->pnnData.pnn[0].name.full.name, inSig->pnnData.pnn[0].name.abbr.name);

                    vgPutNewLine(entity);
                    setResultCode (entity, RESULT_CODE_OK);
                }
            }
            break;

            case VG_MNON_OP_TEST:
            {
                if( simLockContext_p->startField == 1)
                {
                    /* Initialise the data*/
                    vgPutNewLine(entity);
                    if( (inSig->totalEntries == 0) ||
                        (inSig->pnnData.numPnns == 0) )
                    {
                        vgPrintf(   entity,
                                    (Char *)"*MNON: (0)");
                        vgPutNewLine(entity);
                        setResultCode (entity, RESULT_CODE_OK);
                        doPrintLoop = FALSE;
                    }
                    else
                    {
                        simLockContext_p->vgMnonData.startIntIndex = inSig->pnnData.pnn[0].indexNumber;
                        simLockContext_p->vgMnonData.stopIntIndex = inSig->pnnData.pnn[0].indexNumber;
                        simLockContext_p->vgMnonData.intPrinted = FALSE;
                        i=1;

                        vgPrintf(   entity,
                                    (Char *)"*MNON: (");
                        doPrintLoop = TRUE;
                    }
                }
                else
                {
                    doPrintLoop = TRUE;
                    i=0;
                }

                if( doPrintLoop == TRUE)
                {
                    /* i has been initialised before*/
                    for( ; i<inSig->pnnData.numPnns; i++)
                    {
                        if( simLockContext_p->vgMnonData.stopIntIndex+1 !=
                                    inSig->pnnData.pnn[i].indexNumber)
                        {
                            /* Print the comma if need*/
                            if( simLockContext_p->vgMnonData.intPrinted == TRUE)
                            {
                                vgPrintf(   entity,
                                            (Char *)",");
                            }
                            else
                            {
                                simLockContext_p->vgMnonData.intPrinted = TRUE;
                            }

                            /* Print the interval*/
                            if( simLockContext_p->vgMnonData.startIntIndex ==
                                        simLockContext_p->vgMnonData.stopIntIndex)
                            {
                                vgPrintf(   entity,
                                            (Char *)"%d",
                                            simLockContext_p->vgMnonData.startIntIndex);
                            }
                            else
                            {
                                vgPrintf(   entity,
                                            (Char *)"%d-%d",
                                            simLockContext_p->vgMnonData.startIntIndex,
                                            simLockContext_p->vgMnonData.stopIntIndex);
                            }
                            simLockContext_p->vgMnonData.startIntIndex = inSig->pnnData.pnn[i].indexNumber;
                            simLockContext_p->vgMnonData.stopIntIndex = inSig->pnnData.pnn[i].indexNumber;
                        }
                        else
                        {
                            /* Increase the interval*/
                            simLockContext_p->vgMnonData.stopIntIndex = inSig->pnnData.pnn[i].indexNumber;
                        }
                    }

                    if( (inSig->pnnData.numPnns < SIM_PNN_LIST_SIZE) ||
                        (simLockContext_p->startField + inSig->pnnData.numPnns > inSig->totalEntries))
                    {
                        /* Print the final interval*/
                        if( simLockContext_p->vgMnonData.intPrinted == TRUE)
                        {
                            vgPrintf(   entity,
                                        (Char *)",");
                        }
                        if( simLockContext_p->vgMnonData.startIntIndex ==
                                    simLockContext_p->vgMnonData.stopIntIndex)
                        {
                            vgPrintf(   entity,
                                        (Char *)"%d)",
                                        simLockContext_p->vgMnonData.startIntIndex);
                        }
                        else
                        {
                            vgPrintf(   entity,
                                        (Char *)"%d-%d)",
                                        simLockContext_p->vgMnonData.startIntIndex,
                                        simLockContext_p->vgMnonData.stopIntIndex);
                        }

                        /* Command is complete*/
                        vgPutNewLine(entity);
                        setResultCode (entity, RESULT_CODE_OK);
                    }
                    else
                    {
                        /* Ask for the next data*/
                        simLockContext_p->startField = inSig->nextStartField;
                        vgChManContinueAction (entity,APEX_SIM_LIST_PNN_REQ);
                    }
                }
            }
            break;

            default:
            {
                FatalParam( simLockContext_p->vgMnonData.operation, 0, 0);
            }
            break;
        }
    }
    else
    {
        setResultCode (entity, RESULT_CODE_ERROR);
    }
}

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexListOplCnf
 *
 * Parameters:  Pointer to a incoming ApexListOplCnf data structure
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SIM_READ_OPL_CNF signal received
 *              from the background layer.
 *----------------------------------------------------------------------------*/
void vgSigApexListOplCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
  SimLockContext_t          *simLockContext_p = ptrToSimLockContext (entity);
  const ApexSimListOplCnf   *inSig = &(signalBuffer->sig->apexSimListOplCnf);
  Int8                      i;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (simLockContext_p != PNULL, entity, 0, 0);
#endif
  if (inSig->requestStatus == SIM_REQ_OK)
  {
    if ( simLockContext_p->commandField == 0)
    {
      if (simLockContext_p->startField == 1)
      {
        vgPutNewLine (entity);
      }

      for (i=0; i< inSig->oplData.numOpls; i++)
      {
        vgPrintf ( entity, (Char *)"*MOPL: %d,\"%03x%03x\",\"%04x%04x\",%d",simLockContext_p->startField+i,
                  inSig->oplData.opl[i].simPlmn.plmn.mcc, inSig->oplData.opl[i].simPlmn.plmn.mcc,
                  inSig->oplData.opl[i].lac1 , inSig->oplData.opl[i].lac2, inSig->oplData.opl[i].pnnId);

        vgPutNewLine (entity);
      }

      if (simLockContext_p->startField + inSig->oplData.numOpls > inSig->totalEntries)
      {
        setResultCode (entity, RESULT_CODE_OK);
      }
      else
      {
        simLockContext_p->startField = inSig->nextStartField;
        vgChManContinueAction (entity,APEX_SIM_LIST_OPL_REQ);
      }
    }
    else
    {
      vgPutNewLine (entity);

      vgPrintf ( entity, (Char *)"*MOPL: %d,\"%03x%03x\",\"%04x%04x\",%d",simLockContext_p->startField,
                inSig->oplData.opl[0].simPlmn.plmn.mcc, inSig->oplData.opl[0].simPlmn.plmn.mcc,
                inSig->oplData.opl[0].lac1 , inSig->oplData.opl[0].lac2, inSig->oplData.opl[0].pnnId);

      vgPutNewLine (entity);

      setResultCode (entity, RESULT_CODE_OK);
    }
  }
  else
  {
    setResultCode (entity, RESULT_CODE_ERROR);
  }
}



/*--------------------------------------------------------------------------
 *
 * Function:    vgMuappProcessSimReadDirCnf
 *
 * Parameters:  sig_p -  Received signal
                simLockContext_p - Sim lock context
 *              entity - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Process a received ApexSimReadDirCnf signal for a MUAPP command
 *-------------------------------------------------------------------------*/
static void vgMuappProcessSimReadDirCnf (    const ApexSimReadDirCnf *sig_p,
                                            SimLockContext_t        *simLockContext_p,
                                            const VgmuxChannelNumber entity)
{
    SimLockGenericContext_t    *simLockGenericContext_p   = ptrToSimLockGenericContext ();
    const SimUiccAidLabel      *appInfo_p;
    Int8                        i;

    if ( sig_p->requestStatus == SIM_REQ_OK)
    {
        switch ( simLockContext_p->vgMuappData.muappMode)
        {
            case VG_MUAPP_RANGE:
            {
                vgPutNewLine ( entity);
                if ( sig_p->totalEntries > 1)
                {
                    vgPrintf (   entity,
                                (Char *)"*MUAPP: (0-3),(1-%d),(0-9),(0-1)",
                                sig_p->totalEntries );
                }
                else // 0 or 1
                {
                    vgPrintf (   entity,
                                (Char *)"*MUAPP: (0-3),(%d),(0-9),(0-1)",
                                sig_p->totalEntries );
                }
                vgPutNewLine ( entity);
                setResultCode (entity, RESULT_CODE_OK);
            }
            break;

            case VG_MUAPP_QUERY:
            {
                for (i = 0; i < sig_p->applicationList.numUsimApplications; i++)
                {
                    appInfo_p = &sig_p->applicationList.applicationLabel[i];

                    /* Print the AAS*/
                    vgPutNewLine( entity);
                    vgPrintf(   entity,
                                (Char *)"*MUAPP: %d,%d,%d,",
                                sig_p->recordNum + i,
                                appInfo_p->applicationType,
                                (simLockGenericContext_p->activatedAidIndex == (sig_p->recordNum + i)) ? 1 : 0);

                    if( appInfo_p->length != 0)
                    {
                        vgPutc( entity, '\"');
                        vgPutAlphaId(   entity,
                                        appInfo_p->data,
                                        appInfo_p->length);
                        vgPutc( entity, '\"');
                    }
                    vgPutNewLine( entity);
                }

                if( (sig_p->recordNum + sig_p->applicationList.numUsimApplications) <= sig_p->totalEntries)
                {
                    /* Continue with the next data*/
                    simLockContext_p->dirReqStartRecord
                        = sig_p->recordNum + sig_p->applicationList.numUsimApplications;
                    simLockContext_p->dirReqNumRecord = 0;
                    setResultCode (entity, vgChManContinueAction (entity, SIG_APEX_SIM_READ_DIR_REQ));
                }
                else
                {
                    setResultCode (entity, RESULT_CODE_OK);
                }
            }
            break;

            case VG_MUAPP_ACTIVE_AID_SESSION:
            {
                if( simLockContext_p->vgMuappData.index > sig_p->totalEntries)
                {
                    vgPutNewLine( entity);
                    setResultCode (entity, VG_CME_INVALID_INPUT_VALUE);
                }
                else
                {
                    /* Execute the request*/
                    simLockContext_p->dirReqStartRecord = simLockContext_p->vgMuappData.index;
                    setResultCode(  entity,
                                    vgChManContinueAction(  entity,
                                                            SIG_APEX_SIM_USIM_APP_START_REQ));
                }
            }
            break;

            default:
            {
                FatalParam( simLockContext_p->vgMuappData.muappMode, 0, 0);
            }
            break;
        }
    }
    else
    {
        vgPutNewLine( entity);
        setResultCode (entity, RESULT_CODE_ERROR);
    }
}

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimReadDirCnf
 *
 * Parameters:  Pointer to a incoming ApexSimReadDirCnf data structure
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SIM_READ_DIR_CNF signal received
 *              from the background layer.
 *----------------------------------------------------------------------------*/
void vgSigApexSimReadDirCnf (    const SignalBuffer *signalBuffer,
                                const VgmuxChannelNumber entity)
{
    const ApexSimReadDirCnf *inSig              = &(signalBuffer->sig->apexSimReadDirCnf);
    SimLockContext_t        *simLockContext_p   = ptrToSimLockContext (entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck (simLockContext_p != PNULL, entity, 0, 0);
#endif
    switch ( getCommandId ( entity) )
    {
        case VG_AT_SL_MUAPP:
        {
            vgMuappProcessSimReadDirCnf ( inSig, simLockContext_p, entity);
        }
        break;

        default:
        {
            FatalParam ( getCommandId ( entity), 0, 0);
        }
        break;
    }
}
/*--------------------------------------------------------------------------
 *
 * Function:        vgConvBcdToText
 *
 * Parameters:      (In) numType - type of Bcd number
 *                  (In) bcdData_p - pointer to Bcd string
 *                  (In) bcdDataLen - length of Bcd string
 *                  (InOut) textDialNum_p - pointer to output text dial string
 *
 * Returns:         pointer to output string
 *
 * Description:     gets Bcd digit from Bcd string and appends it to
 *                  textDialNum_p
 *
 *-------------------------------------------------------------------------*/

Char *vgConvFromBcdToText (BcdNumberType numType,
                        const Bcd *bcdData_p,
                         Int8 bcdDataLen,
                          Char *textDialNum_p)
{
  const Bcd   *bcdString_p = bcdData_p;
  Int16 pos = 0;
  Int32 out = 0;
  Int8 val = 0;
  Int16 len = (bcdDataLen * 2);

  if (numType == NUM_TYPE_INTERNATIONAL)
  {
    *textDialNum_p++ = INTERNATIONAL_PREFIX;
    out++;
  }

  while ((pos < len) && (out < MAX_CALLED_BCD_NO_LENGTH))
  {
    val = vgGetBcd (bcdString_p, pos);
    switch (val)
    {
      case 0x0:
      case 0x1:
      case 0x2:
      case 0x3:
      case 0x4:
      case 0x5:
      case 0x6:
      case 0x7:
      case 0x8:
      case 0x9:
      {
        *textDialNum_p++ = val - 0x00 + '0';
        out++;
        break;
      }
      case 0xA:
      { /* star (can be changed) */
        *textDialNum_p++ = STAR_CHAR;
        out++;
        break;
      }
      case 0xB:
      { /* hash (can be changed) */
        *textDialNum_p++ = HASH_CHAR;
        out++;
        break;
      }
      case 0xC:
      { /* an a (can be changed) */
        *textDialNum_p++ = (Char)'A';
        out++;
        break;
      }
      case 0xD:
      { /* a b (can be changed) */
        *textDialNum_p++ = (Char)'B';
        out++;
        break;
      }
      case 0xE:
      { /* a c (can be changed) */
        *textDialNum_p++ = (Char)'C';
        out++;
        break;
      }
      case 0xF:
      { /* filler character - ignore */
        break;
      }
      default:
      {
        /*
         * Any other value - ignore...
         */
        break;
      }
    }
    pos++;
  }

  *textDialNum_p = (Char)0;

  return (textDialNum_p);
}

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimReadMsisdnCnf
 *
 * Parameters:  Pointer to a incoming ApexSimReadMsisdnCnf data structure
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SIM_READ_MSISDN_CNF signal received
 *              from the background layer.
 *----------------------------------------------------------------------------*/
void vgSigApexSimReadMsisdnCnf (    const SignalBuffer *signalBuffer,
                                const VgmuxChannelNumber entity)
{
    const ApexSimReadMsisdnCnf *inSig              = &(signalBuffer->sig->apexSimReadMsisdnCnf);
    const Int8                 *alphaText;
    Int16                      lengthText;

    Char *conversionBuffer_p = NULL;
    KiAllocMemory((sizeof(Char)*(MAX_CALLED_BCD_NO_LENGTH + NULL_TERMINATOR_LENGTH)), (void **)&conversionBuffer_p);
    if ( inSig->requestStatus == SIM_REQ_OK)
    {
        alphaText = &(inSig->dialNumber.alphaId.data[0]);
        lengthText = inSig->dialNumber.alphaId.length;
        MobilityContext_t   *mobilityContext_p = ptrToMobilityContext ();

        vgPutNewLine( entity);
        vgPrintf (entity, (Char *)"+CNUM: ");
        vgPutAlphaId (entity, alphaText, lengthText);
        vgConvFromBcdToText(inSig->dialNumber.typeOfNumber,
                  inSig->dialNumber.dialString,
                  inSig->dialNumber.dialStringLength,
                  conversionBuffer_p);
        vgPrintf(entity,
                  (Char *)",\"%s\",%d",
                  conversionBuffer_p,
                  vgBcdNumberTypeToChar(inSig->dialNumber.typeOfNumber));
        vgPutNewLine( entity);
        vgFlushBuffer(entity);
        if (inSig->more)
        {
            mobilityContext_p->cnumDatarecordNumber = inSig->recordNumber +1;
            vgChManContinueAction(entity,SIG_APEX_SIM_READ_MSISDN_REQ);
        }
        else
        {
            setResultCode (entity, RESULT_CODE_OK);
        }
    }
    else
    {
        MobilityContext_t   *mobilityContext_p = ptrToMobilityContext ();

        /* More than one record has been read out */
        if(mobilityContext_p->cnumDatarecordNumber > 1)
        {
            setResultCode (entity, RESULT_CODE_OK);
        }
        else
        {
            setResultCode (entity, RESULT_CODE_ERROR);    
        }
    }
    KiFreeMemory((void **)&conversionBuffer_p);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgMuappProcessUsimAppStartCnf
 *
 * Parameters:  sig_p -  Received signal
                simLockContext_p - Sim lock context
 *              entity - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Process a received ApexSimUsimAppStartCnf signal for a MUAPP command
 *-------------------------------------------------------------------------*/
static void vgMuappProcessUsimAppStartCnf (  const ApexSimUsimAppStartCnf    *sig_p,
                                            SimLockContext_t                *simLockContext_p,
                                            const VgmuxChannelNumber        entity)
{
    if ( sig_p->requestStatus == SIM_REQ_OK)
    {
        switch ( simLockContext_p->vgMuappData.muappMode)
        {
            case VG_MUAPP_ACTIVE_AID_SESSION:
            {
                vgPutNewLine ( entity);
                setResultCode (entity, RESULT_CODE_OK);
            }
            break;

            default:
            {
                FatalParam ( simLockContext_p->vgMuappData.muappMode, 0, 0);
            }
            break;
        }
    }
    else
    {
        vgPutNewLine ( entity);
        setResultCode (entity, RESULT_CODE_ERROR);
    }
}

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimUsimAppStartCnf
 *
 * Parameters:  Pointer to a incoming ApexSimUsimAppStartCnf data structure
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_SIM_USIM_APP_START_CNF signal received
 *              from the background layer.
 *----------------------------------------------------------------------------*/
void vgSigApexSimUsimAppStartCnf (   const SignalBuffer *signalBuffer,
                                    const VgmuxChannelNumber entity)
{
    const ApexSimUsimAppStartCnf   *inSig               = &(signalBuffer->sig->apexSimUsimAppStartCnf);
    SimLockContext_t               *simLockContext_p    = ptrToSimLockContext (entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck (simLockContext_p != PNULL, entity, 0, 0);
#endif
    switch ( getCommandId ( entity) )
    {
        case VG_AT_SL_MUAPP:
        {
            vgMuappProcessUsimAppStartCnf ( inSig, simLockContext_p, entity);
        }
        break;

        default:
        {
            FatalParam ( getCommandId ( entity), 0, 0);
        }
        break;
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimAppStoppedInd
 *
 * Parameters:  inSig - Pointer to signal of type ApexAppStoppedInd
 *
 * Returns:     Nothing
 *
 * Description: Indicates that the SIM application has been stopped
 *-------------------------------------------------------------------------*/
void vgSigApexSimAppStoppedInd (const SignalBuffer *signalBuffer,
                                const VgmuxChannelNumber entity)
{
    ApexSimAppStoppedInd    *sig_p = &signalBuffer->sig->apexSimAppStoppedInd;
    VgMuappEvent            muappEvent;
    SimLockGenericContext_t *simLockGenericContext_p   = ptrToSimLockGenericContext ();

    muappEvent.index    = sig_p->recordNum;
    muappEvent.state    = VG_MUAPP_APP_STOPPED;
    muappEvent.aid      = sig_p->applicationInfo;
    vgMuappPrintEvent ( &muappEvent);

    /* No more application running*/
    simLockGenericContext_p->activatedAidIndex = 0;
}

#if defined (SIM_EMULATION_ON)
/*--------------------------------------------------------------------------
 *
 * Function:    vgSigAlsiWriteUsimEmuFileCnf
 *
 * Parameters:  inSig - Pointer to signal of type AlsiWriteUsimEmuFileCnf
 *
 * Returns:     Nothing
 *
 * Description: Received as response to a SIG_ALSI_WRITE_USIM_EMU_FILE_REQ
 *              sent to the USIM Manager.
 *-------------------------------------------------------------------------*/
void vgSigAlsiWriteUsimEmuFileCnf(  const SignalBuffer *signalBuffer,
                                    const VgmuxChannelNumber entity)
{
    AlsiWriteUsimEmuFileCnf    *sig_p = &signalBuffer->sig->alsiWriteUsimEmuFileCnf;

    FatalCheck (getCommandId(entity) == VG_AT_SL_MUSIMEMUW, entity, 0, 0);

    if (sig_p->success)
    {
        setResultCode (entity, RESULT_CODE_OK);
    }
    else
    {
        setResultCode (entity, RESULT_CODE_ERROR);
    }
}
#endif /* SIM_EMULATION_ON */

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimOpenLogicalChannelCnf
 *
 * Parameters:  Pointer to a incoming ApexSimOpenLogicalChannelCnf data structure
 *
 * Returns:     Nothing
 *
 * Description: Handles the APEX_SIM_OPEN_LOGICAL_CHANNEL_CNF signal received
 *              from the background layer.
 *----------------------------------------------------------------------------*/
void vgSigApexSimOpenLogicalChannelCnf (   const SignalBuffer *signalBuffer,
                                    const VgmuxChannelNumber entity)
{
    ApexSimOpenLogicalChannelCnf    *inSig               = &(signalBuffer->sig->apexSimOpenLogicalChannelCnf);
    SimLockContext_t                *simLockContext_p    = ptrToSimLockContext (entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck (simLockContext_p != PNULL, entity, 0, 0);
#endif
    switch ( getCommandId ( entity) )
    {
        case VG_AT_GN_CCHO:
        {
            if (SIM_REQ_OK == inSig->requestStatus)
            {
                simLockContext_p->vgUiccLogicChannelData.sessionId = inSig->sessionId;

                vgPutNewLine (entity);
                vgPrintf (entity,(const Char *)"%d", inSig->sessionId);
                vgPutNewLine (entity);

                setResultCode (entity, RESULT_CODE_OK);
            }
            else
            {
                setResultCode (entity, VG_CME_SIM_FAILURE);
            }
        }
        break;

        default:
        {
            FatalParam ( getCommandId ( entity), 0, 0);
        }
        break;
    }
} /*vgSigApexSimOpenLogicalChannelCnf*/

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimCloseLogicalChannelCnf
 *
 * Parameters:  Pointer to a incoming ApexSimOpenLogicalChannelCnf data structure
 *
 * Returns:     Nothing
 *
 * Description: Handles the APEX_SIM_CLOSE_LOGICAL_CHANNEL_CNF signal received
 *              from the background layer.
 *----------------------------------------------------------------------------*/
void vgSigApexSimCloseLogicalChannelCnf ( const SignalBuffer *signalBuffer,
                                    const VgmuxChannelNumber entity)
{
    ApexSimCloseLogicalChannelCnf  *inSig               = &(signalBuffer->sig->apexSimCloseLogicalChannelCnf);
    SimLockContext_t               *simLockContext_p    = ptrToSimLockContext (entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck (simLockContext_p != PNULL, entity, 0, 0);
#endif
    switch ( getCommandId ( entity) )
    {
        case VG_AT_GN_CCHC:
        {
            if (SIM_REQ_OK == inSig->requestStatus)
            {
                simLockContext_p->vgUiccLogicChannelData.sessionId = 0;

                vgPutNewLine (entity);
                vgPrintf (entity,(const Char *)"CCHC");
                vgPutNewLine (entity);

                setResultCode (entity, RESULT_CODE_OK);
            }
            else
            {
                setResultCode (entity, VG_CME_SIM_FAILURE);
            }
        }
        break;

        default:
        {
            FatalParam ( getCommandId ( entity), 0, 0);
        }
        break;
    }
} //vgSigApexSimCloseLogicalChannelCnf





/* END OF FILE */





