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
 * Incoming signal handlers for the General Sub-System.
 **************************************************************************/

#define MODULE_NAME "RVGNSIGI"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <stdio.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvchman.h>
#include <rvoman.h>
#include <rvcrerr.h>
#include <rvgnut.h>
#include <rvgncpb.h>
#include <rvgnsigi.h>
#include <rvgnsigo.h>
#include <rvccut.h>
#include <rvssdata.h>
#include <rvcimxsot.h>
#include <rvcimxut.h>
#include <rvomtime.h>
#include <rvccsigi.h>
#include <rvmssigi.h>
#include <rvcrhand.h>
#include <rvcrconv.h>
#include <rvslut.h>
#include <dmrtc_sig.h>
#include <abgl_sig.h>
#include <rvcfg.h>
#include <gkimem.h>
#if defined (COARSE_TIMER)
#include <gkisig.h>
#endif

#include <n1cd_sig.h>

#include <frhsl.h>

#include "atci_gki_trace.h"
/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{

#if defined (FEA_PHONEBOOK)  
  ApexLmDialNumStatusCnf      apexLmDialNumStatusCnf;
  ApexLmFindDialNumCnf        apexLmFindDialNumCnf;
  ApexLmReadDialNumCnf        apexLmReadDialNumCnf;
  ApexLmWriteDialNumCnf       apexLmWriteDialNumCnf;
  ApexLmDeleteDialNumCnf      apexLmDeleteDialNumCnf;
  ApexLmFixedDialFindCnf      apexLmFixedDialFindCnf;
  ApexLmFixedDialCnf          apexLmFixedDialCnf;
  ApexLmGetAlphaCnf           apexLmGetAlphaCnf;
  ApexLmBarredDialCnf         apexLmBarredDialCnf;
  ApexLmPhoneBookStatusCnf    apexLmPhoneBookStatusCnf;
  ApexLmHiddenKeyFunctionCnf  apexLmHiddenKeyFunctionCnf;
  ApexLmReadGrpCnf            apexLmReadGrpCnf;
  ApexLmReadGasCnf            apexLmReadGasCnf;
  ApexLmListGasCnf            apexLmListGasCnf;
  ApexLmWriteGasCnf           apexLmWriteGasCnf;
  ApexLmWriteGrpCnf           apexLmWriteGrpCnf;
  ApexLmWriteAnrCnf           apexLmWriteAnrCnf;
  ApexLmReadAnrCnf            apexLmReadAnrCnf;
  ApexLmWriteSneCnf           apexLmWriteSneCnf;
  ApexLmReadSneCnf            apexLmReadSneCnf;
  ApexLmWriteEmailCnf         apexLmWriteEmailCnf;
  ApexLmReadEmailCnf          apexLmReadEmailCnf;
  ApexLmDeleteGrpCnf          apexLmDeleteGrpCnf;
  ApexLmDeleteAnrCnf          apexLmDeleteAnrCnf;
  ApexLmDeleteSneCnf          apexLmDeleteSneCnf;
  ApexLmDeleteEmailCnf        apexLmDeleteEmailCnf;
  ApexLmDeleteGasCnf          apexLmDeleteGasCnf;
  ApexLmClearGasCnf           apexLmClearGasCnf;
  ApexLmGetPbSyncInfoCnf      apexLmGetPbSyncInfoCnf;
  ApexLmGetSyncStatusCnf      apexLmGetSyncStatusCnf;
  ApexLmPbSyncInfoChangeInd   apexLmPbSyncInfoChangeInd;
  ApexLmReadRecordUidCnf      apexLmReadRecordUidCnf;
  ApexLmRecordChangedInd      apexLmRecordChangedInd;
  ApexLmReadAasCnf            apexLmReadAasCnf;
  ApexLmListAasCnf            apexLmListAasCnf;
  ApexLmWriteAasCnf           apexLmWriteAasCnf;
  ApexLmDeleteAasCnf          apexLmDeleteAasCnf;
  ApexLmClearAasCnf           apexLmClearAasCnf;
  ApexLmReadyInd              apexLmReadyInd;
#endif /* FEA_PHONEBOOK */
  
  ApexSimReadSpnCnf           apexSimReadSpnCnf;
  ApexSimPinFunctionCnf       apexSimPinFunctionCnf;
  ApexSimChvFunctionCnf       apexSimChvFunctionCnf;
#if defined (FEA_SIMLOCK)  
  ApexSimMepCnf               apexSimMepCnf;
  ApexSimMepStatusCnf         apexSimMepStatusCnf;
  ApexSimWriteMepNetworkIdCnf apexSimWriteMepNetworkIdCnf;
  ApexSimReadMepNetworkIdCnf  apexSimReadMepNetworkIdCnf;
#endif /* FEA_SIMLOCK */  
  DmRtcReadDateAndTimeCnf     dmRtcReadDateAndTimeCnf;
  DmRtcSetDateAndTimeCnf      dmRtcSetDateAndTimeCnf;
  DmRtcDateAndTimeInd         dmRtcDateAndTimeInd;
  Anrm2ReadDataCnf            anrm2ReadDataCnf;
  ApexGlWriteFeatureConfigCnf apexGlWriteFeatureConfigCnf;
  ApexGlReadyInd              apexGlReadyInd;
  N1CdRfTestCnf               n1CdRfTestCnf;
  N1CdIdcTestCnf              n1CdIdcTestCnf;
  ApexMmWriteMobileIdCnf      apexMmWriteMobileIdCnf;
  ApexMmReadMobileIdCnf       apexMmReadMobileIdCnf;

};

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

#if defined (FEA_SIMLOCK)
const Char *MEPMessage[NUM_MEP_PERSONALISATIONS] =
{
  (const Char*)"PH-SIM PIN",
  (const Char*)"PH-NET PIN",
  (const Char*)"PH-NETSUB PIN",
  (const Char*)"PH-SP PIN",
  (const Char*)"PH-CORP PIN"
};
#endif /* FEA_SIMLOCK */

/***************************************************************************
 * Type Definitions
 ***************************************************************************/


/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static void vgConstructRtcString  (const DmRtcReadDateAndTimeCnf *inSig,
                                    const VgmuxChannelNumber entity);

#if defined (FEA_PHONEBOOK)
static PhoneBookInfo getPhoneBookInfo (const ApexLmPhoneBookStatusCnf *signal,
                                       const LmDialNumberFile phoneBook);
static void vgGnMUPBSYNCPrintPbResults( const VgmuxChannelNumber entity,
                                        const ApexLmGetSyncStatusCnf *sig_p);
static void vgGnMUPBSYNCPrintPbSyncData(    const VgmuxChannelNumber profileEntity,
                                            const Char *phonebook,
                                            const Ablm3gSyncInfo *syncInfo);
static void vgGnMUPBSYNCPrintUid(   const VgmuxChannelNumber entity,
                                    const ApexLmReadRecordUidCnf *sig_p);

static PhoneBookInfo getDialNumInfo (VgmuxChannelNumber entity,
                                     const ApexLmDialNumStatusCnf *signal,
                                     const LmDialNumberFile phoneBook);

static void vgListDialNum         (const VgmuxChannelNumber entity);

static void getAllEntriesInPhoneBook (const VgmuxChannelNumber entity,
                                       const PhoneBookInfo *phoneBookInfo);

static void writeEntryToPhoneBook (const VgmuxChannelNumber entity,
                                    const PhoneBookInfo *phoneBookInfo);
static void displayPhoneBookAlphaId (const VgmuxChannelNumber entity,
                                      const ApexLmGetAlphaCnf *signal);

static Boolean isPhoneBookPin2Protected  (const Int8 phoneBookIndex);

static void vgReadAssociationData(  const VgmuxChannelNumber entity,
                                    VgLmData *vgLmData_p);

static void vgReadScpbrAssociationData( const VgmuxChannelNumber entity,
                                        VgLmData *vgLmData_p);

static PhoneBookInfo getDialNumInfoByIndex( VgmuxChannelNumber entity,
                                            const ApexLmDialNumStatusCnf *signal,
                                            const Int8 index);

static void vgProcessPbStatusMupbcfg(   PhoneBookInfo *phoneBookInfo_p,
                                        VgLmData *vgLmData_p,
                                        const VgmuxChannelNumber entity);
static void vgProcessPbStatusMupbgas(   PhoneBookInfo *phoneBookInfo_p,
                                        VgLmData *vgLmData_p,
                                        const VgmuxChannelNumber entity);
static void vgProcessPbStatusMupbaas(   PhoneBookInfo *phoneBookInfo_p,
                                        VgLmData *vgLmData_p,
                                        const VgmuxChannelNumber entity);

static ResultCode_t vgGnCheckPbExtParamValid(   PhoneBookInfo *phoneBookInfo_p,
                                                VgLmData *vgLmData_p,
                                                const VgmuxChannelNumber entity);

static void vgGnPbWriteGroup(   const VgmuxChannelNumber    entity,
                                VgLmData                   *vgLmData_p,
                                Int8                        firstGasIndex);

static Boolean vgGnPbWriteNextAdNum(    const VgmuxChannelNumber    entity,
                                        VgLmData                   *vgLmData_p);

static Boolean vgGnPbWriteNextEmail(    const VgmuxChannelNumber    entity,
                                        VgLmData                   *vgLmData_p);

static void vgCpbwProcessLmListGasCnf(  ApexLmListGasCnf *sig_p,
                                        VgLmData *vgLmData_p,
                                        const VgmuxChannelNumber entity);
static void vgMupbgasProcessLmListGasCnf(   ApexLmListGasCnf *sig_p,
                                            VgLmData *vgLmData_p,
                                            const VgmuxChannelNumber entity);

static void vgCpbrProcessLmReadGasCnf(  ApexLmReadGasCnf *sig_p,
                                        VgLmData *vgLmData_p,
                                        const VgmuxChannelNumber entity);
static void vgMupbgasProcessLmReadGasCnf(   ApexLmReadGasCnf *sig_p,
                                            VgLmData *vgLmData_p,
                                            const VgmuxChannelNumber entity);

static void vgCpbwProcessLmWriteGasCnf( ApexLmWriteGasCnf *sig_p,
                                        VgLmData *vgLmData_p,
                                        const VgmuxChannelNumber entity);
static void vgMupbgasProcessLmWriteGasCnf(  ApexLmWriteGasCnf *sig_p,
                                            VgLmData *vgLmData_p,
                                            const VgmuxChannelNumber entity);

static void vgMupbgasProcessLmClearGasCnf(  ApexLmClearGasCnf *sig_p,
                                            VgLmData *vgLmData_p,
                                            const VgmuxChannelNumber entity);

static void vgMupbgasProcessLmDeleteGasCnf( ApexLmDeleteGasCnf *sig_p,
                                            VgLmData *vgLmData_p,
                                            const VgmuxChannelNumber entity);

static void vgScpbrProcessLmReadDialNum(    ApexLmReadDialNumCnf *sig_p,
                                            VgLmData *vgLmData_p,
                                            const VgmuxChannelNumber entity);
#endif /* FEA_PHONEBOOK */

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
* Function:    vgConstructRtcString
*
* Parameters:  inSig     - pointer to incoming signal AclkReadDateAndTimeCnf
*
* Returns:     Nothing
*
* Description: Constructs a time string of the form "yy/MM/dd,hh:mm:ss+-zz"
*              from the incoming signal AclkReadDateAndTimeCnf (which contains
*              the real time clock settings in a RtcDateAndTime structure.
*-------------------------------------------------------------------------*/

static void vgConstructRtcString (const DmRtcReadDateAndTimeCnf *inSig,
                                  const VgmuxChannelNumber entity)
{
  Int8  timeZone = 0;  /* offset (in 15 minute intervals) from GMT */

  /* Construct time string from signal data */
  vgPrintf (entity,
            (Char *)"+CCLK: \"%02d/%02d/%02d,%02d:%02d:%02d",
            inSig->dateAndTime.date.year%100,  /* only need last 2 digits */
            inSig->dateAndTime.date.month,
            inSig->dateAndTime.date.day,
            inSig->dateAndTime.time.hours,
            inSig->dateAndTime.time.minutes,
            inSig->dateAndTime.time.seconds);

  /* time zone */
  timeZone = (inSig->dateAndTime.timeZone.offset.hours * 4) +
             (Int8)((inSig->dateAndTime.timeZone.offset.minutes + 7) / 15);

  switch (inSig->dateAndTime.timeZone.format)
  {
    case RTC_DISP_FORMAT_POS:
    {
      vgPrintf (entity, (Char *)"+%02d", timeZone);
      break;
    }
    case RTC_DISP_FORMAT_NEG:
    {
      vgPrintf (entity, (Char *)"-%02d", timeZone);
      break;
    }
    case RTC_DISP_FORMAT_INVALID:
    {
      vgPrintf (entity, (Char *)"+00");
      break;
    }
  }
  vgPutc (entity, '\"');

}

/*--------------------------------------------------------------------------
*
* Function:        vgUpdate2GPinStatus
*
* Parameters:      ApexSimChvFunctionCnf   *sig_p
*                  Boolean                  verified
*
*
* Returns:         nothing
*
* Description:   updates the cached PIN status. Only used with 2G SIMs
*-------------------------------------------------------------------------*/

static void vgUpdate2GPinStatus(ApexSimChvFunctionCnf   *sig_p)
{
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
  if ((sig_p->requestStatus == SIM_REQ_CODE_BLOCKED) ||
      (sig_p->requestStatus == SIM_REQ_OK)||
      (sig_p->requestStatus == SIM_REQ_FILE_INVALIDATED) ||
      (sig_p->requestStatus == SIM_REQ_ACCESS_DENIED))
  {
    if (sig_p->chvNum == SIM_CHV_2)
    {
      /*update cached CHV2 status*/
      simLockGenericContext_p->simInfo.pin2NumRemainingRetrys = sig_p->chvStatus.numRemainingRetrys;
      simLockGenericContext_p->simInfo.unblockPin2NumRemainingRetrys = sig_p->unblockChvStatus.numRemainingRetrys;
      if ((sig_p->requestStatus == SIM_REQ_OK)&&(sig_p->chvFunctionSuccess))
      {
        simLockGenericContext_p->simInfo.pin2Verified = TRUE;
      }
      else if ((sig_p->requestStatus == SIM_REQ_ACCESS_DENIED) ||
               (sig_p->requestStatus == SIM_REQ_CODE_BLOCKED) )
      {
        simLockGenericContext_p->simInfo.pin2Verified = FALSE;
      }
    }
    else if (sig_p->chvNum == SIM_CHV_1)
    {
      /*update cached CHV1 status*/
      simLockGenericContext_p->simInfo.pinNumRemainingRetrys = sig_p->chvStatus.numRemainingRetrys;
      simLockGenericContext_p->simInfo.unblockPinNumRemainingRetrys = sig_p->unblockChvStatus.numRemainingRetrys;
    }
  }
}


/*--------------------------------------------------------------------------
*
* Function:        vgUpdate3GPinStatus
*
* Parameters:      ApexSimPinFunctionCnf   *sig_p
*                  Boolean                  verified
*
*
* Returns:         nothing
*
* Description:   updates the cached PIN status. Only used with 3G SIMs
*-------------------------------------------------------------------------*/

static void vgUpdate3GPinStatus(ApexSimPinFunctionCnf   *sig_p)
{
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
  if ((sig_p->requestStatus == SIM_REQ_CODE_BLOCKED) ||
      (sig_p->requestStatus == SIM_REQ_OK)||
      (sig_p->requestStatus == SIM_REQ_FILE_INVALIDATED) ||
      (sig_p->requestStatus == SIM_REQ_ACCESS_DENIED))
  {
    if (sig_p->pinKeyReference == simLockGenericContext_p->simInfo.pin2KeyRef)
    {
      /* update local PIN status */
      if ((sig_p->requestStatus == SIM_REQ_OK)&&(sig_p->pinFunctionSuccess))
      {
        simLockGenericContext_p->simInfo.pin2Verified = TRUE;
        simLockGenericContext_p->simInfo.pin2Status.verified = TRUE;
      }
      else if ((sig_p->requestStatus == SIM_REQ_ACCESS_DENIED) ||
               (sig_p->requestStatus == SIM_REQ_CODE_BLOCKED) )
      {
        simLockGenericContext_p->simInfo.pin2Verified = FALSE;
      }
      simLockGenericContext_p->simInfo.pin2NumRemainingRetrys = sig_p->pinStatus.numRemainingRetrys;
      simLockGenericContext_p->simInfo.unblockPin2NumRemainingRetrys = sig_p->unblockPinStatus.numRemainingRetrys;
      simLockGenericContext_p->simInfo.pin2Status.numRemainingRetrys = sig_p->pinStatus.numRemainingRetrys;;
      simLockGenericContext_p->simInfo.pin2Status.numRemainingRetrys = sig_p->unblockPinStatus.numRemainingRetrys;
    }
    else if ((sig_p->pinKeyReference == simLockGenericContext_p->simInfo.pin1KeyRef) ||
             (sig_p->pinKeyReference == USIM_ACCESS_UNIVERSAL_PIN))
    {
      /*update application PIN/ universal PIN status*/
      simLockGenericContext_p->simInfo.pinEnabled = sig_p->pinStatus.enabled;
      simLockGenericContext_p->simInfo.pinNumRemainingRetrys = sig_p->pinStatus.numRemainingRetrys;
      simLockGenericContext_p->simInfo.unblockPinNumRemainingRetrys = sig_p->unblockPinStatus.numRemainingRetrys;
      if (sig_p->pinKeyReference == USIM_ACCESS_UNIVERSAL_PIN)
      {
        simLockGenericContext_p->simInfo.universalPinStatus.enabled = sig_p->pinStatus.enabled;
        simLockGenericContext_p->simInfo.universalPinStatus.numRemainingRetrys= sig_p->pinStatus.numRemainingRetrys;
        simLockGenericContext_p->simInfo.unblockUniversalPinStatus.numRemainingRetrys = sig_p->unblockPinStatus.numRemainingRetrys;
      }
      else
      {
        simLockGenericContext_p->simInfo.pin1Status.enabled = sig_p->pinStatus.enabled;
        simLockGenericContext_p->simInfo.pin1Status.numRemainingRetrys = sig_p->pinStatus.numRemainingRetrys;
        simLockGenericContext_p->simInfo.unblockPin1Status.numRemainingRetrys = sig_p->unblockPinStatus.numRemainingRetrys;
      }
    }

  }
}

#if defined (FEA_PHONEBOOK)
/*--------------------------------------------------------------------------
*
* Function:     vgReadScpbrAssociationData
*
* Parameters:   const VgmuxChannelNumber   entity
*               VgLmData                  *vgLmData_p
*
*
* Returns:      nothing
*
* Description:  Reads the association data (additional number, email) if
*               supported by the USIM
*-------------------------------------------------------------------------*/
static void vgReadScpbrAssociationData( const VgmuxChannelNumber entity,
                                        VgLmData *vgLmData_p)
{
    Boolean readingData = FALSE;

    /*read the different association data, returns a boolean
    * to indicate whether any association data is supported, and
    * requires to be read */

    while(  (vgLmData_p->assocData < VG_PB_NUM_ASSOC_DATA) &&
            (readingData == FALSE))
    {
        switch ( vgLmData_p->assocData)
        {
            case VG_PB_ANR:
            {
                if (vgLmData_p->adNumInfo.anrSupported)
                {
                    /*USIM is able to store additional number info*/
                    /*read the additional number */
                    vgLmData_p->adNumInfo.anrIndex = 0;
                    vgLmData_p->aasInfo.aasIndex = 0;
                    vgLmData_p->aasInfo.adAasIndex[0] = 0;
                    setResultCode(  entity,
                                    vgChManContinueAction (entity, SIG_APEX_LM_READ_ANR_REQ));
                    readingData = TRUE;
                    vgLmData_p->assocData = VG_PB_AD_ANR;
                }
                else
                {
                    /*Type of number and number are both missing*/
                    vgLmData_p->missingParamCount += (3 * 2); /*type of number + number, so increment by 2*/
                    vgLmData_p->assocData = VG_PB_EMAIL;
                }
            }
            break;

            case VG_PB_AD_ANR:
            {
                FatalAssert( vgLmData_p->adNumInfo.anrSupported == TRUE);

                /* Continue by reading the next ANR*/
                vgLmData_p->adNumInfo.anrIndex++;
                if( vgLmData_p->adNumInfo.anrIndex < VG_SCPBW_NUM_AD_NUMBER)
                {
                    if( vgLmData_p->adNumInfo.anrIndex < vgLmData_p->adNumInfo.adNumPerRec)
                    {
                        setResultCode(  entity,
                                        vgChManContinueAction (entity, SIG_APEX_LM_READ_ANR_REQ));
                        readingData = TRUE;
                    }
                    else
                    {
                        /* For each number there is the number and the type, so x2*/
                        vgLmData_p->missingParamCount +=
                            ( (VG_SCPBW_NUM_AD_NUMBER - vgLmData_p->adNumInfo.adNumPerRec ) * 2);

                        /* All the ANRs has been read*/
                        vgLmData_p->assocData = VG_PB_EMAIL;
                    }
                }
                else
                {
                    /* All the ANRs has been read*/
                    vgLmData_p->assocData = VG_PB_EMAIL;
                }
            }
            break;

            case VG_PB_EMAIL:
            {
                if( vgLmData_p->alphaLength > 0)
                {
                    /* Need to print the alpha before continue*/
                    vgPrintMissingParams (entity, vgLmData_p->missingParamCount);
                    vgLmData_p->missingParamCount = 0;

                    vgPutc (entity, ',');
                    vgPutc (entity, '\"');
                    vgPutAlphaId(   entity,
                                    vgLmData_p->alpha,
                                    vgLmData_p->alphaLength);
                    vgPutc (entity, '\"');
                }
                else
                {
                    vgLmData_p->missingParamCount++;
                }

                /* Print alpha format*/
                vgPrintMissingParams (entity, vgLmData_p->missingParamCount);
                vgLmData_p->missingParamCount = 0;
                vgPrintf(   entity,
                            (const Char *)",%d",
                            ((VgCSCSMode)getProfileValue(entity, PROF_CSCS) == VG_AT_CSCS_GSM) ?
                                VG_SCPBW_CODING_GSM : VG_SCPBW_CODING_RAW);

                if (vgLmData_p->emailInfo.emailSupported == TRUE)
                {
                    /*USIM is able to store email info*/
                    /*Get email info */
                    vgLmData_p->emailInfo.emailIndex = 0;
                    setResultCode(  entity,
                                    vgChManContinueAction (entity, SIG_APEX_LM_READ_EMAIL_REQ));
                    readingData = TRUE;
                }
                else
                {
                    vgLmData_p->missingParamCount++;
                }
                vgLmData_p->assocData = VG_PB_COMPLETE;
            }
            break;

            case VG_PB_COMPLETE:
            {
                /*print missing parameters*/
                vgPrintMissingParams (entity, vgLmData_p->missingParamCount);
                vgPutNewLine (entity);

                if (vgLmData_p->phoneIndex1 >= vgLmData_p->phoneIndex2)
                {
                    setResultCode (entity, RESULT_CODE_OK);
                }
                else
                {
                    /*we need to get other numbers*/
                    vgListDialNum (entity);
                }
                vgLmData_p->assocData = VG_PB_NUM_ASSOC_DATA;
            }
            break;

            default:
            {
                /* assoc data doesn' t exist */
                FatalParam(entity, vgLmData_p->assocData, 0);
            }
            break;
        }
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgScpbrProcessLmReadDialNum
 *
 * Parameters:  sig_p -  Received signal
 *              vgLmData_p - LM context
 *              entity - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description:
 *-------------------------------------------------------------------------*/
static void vgScpbrProcessLmReadDialNum(    ApexLmReadDialNumCnf *sig_p,
                                            VgLmData *vgLmData_p,
                                            const VgmuxChannelNumber entity)
{
    LmDialNumber    dialNumber = sig_p->dialNumber;
    Char            conversionBuffer[MAX_CALLED_BCD_NO_LENGTH + NULL_TERMINATOR_LENGTH] = {0};

    if( vgLmData_p->firstDialNumRecordFromList == TRUE)
    {
        vgPutNewLine (entity);
        vgLmData_p->firstDialNumRecordFromList = FALSE;
    }
    vgPrintf (entity, (Char *)"^SCPBR: ");

    /* display the current phone book entry */
    vgConvBcdToText(    dialNumber.typeOfNumber,
                        dialNumber.dialString,
                        dialNumber.dialStringLength,
                        conversionBuffer);

    vgPrintf(   entity,
                (Char *)"%d,\"%s\",%d",
                sig_p->recordNumber,
                conversionBuffer,
                vgBcdNumberTypeToChar (dialNumber.typeOfNumber));

    /* Store the alpha for the moment*/
    memcpy( &vgLmData_p->alpha[0],
            &dialNumber.alphaId.data[0],
            SIM_ALPHA_ID_SIZE);
    vgLmData_p->alphaLength = dialNumber.alphaId.length;

    /* Read the next data*/
    vgLmData_p->missingParamCount   = 0;
    vgLmData_p->phoneIndex1         = sig_p->recordNumber;
    vgLmData_p->assocData           = VG_PB_ANR;
    vgReadScpbrAssociationData( entity, vgLmData_p);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgMupbgasProcessLmDeleteGasCnf
 *
 * Parameters:  sig_p -  Received signal
 *              vgLmData_p - LM context
 *              entity - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Process a received ApexLmClearGasCnf signal for a MUPBGAS operation
 *-------------------------------------------------------------------------*/
static void vgMupbgasProcessLmDeleteGasCnf( ApexLmDeleteGasCnf *sig_p,
                                            VgLmData *vgLmData_p,
                                            const VgmuxChannelNumber entity)
{
    switch(vgLmData_p->vgMupbgasContext.mupbgasMode)
    {
        case VG_MUPBGAS_MODE_WRITE: /* Means it is a indexed delete operation*/
        {
            if (sig_p->requestStatus == LM_REQ_OK)
            {
                setResultCode (entity, RESULT_CODE_OK);
            }
            else
            {
                /*something went wrong, flag an error*/
                setResultCode (entity, RESULT_CODE_ERROR);
            }
        }
        break;

        default:
        {
            DevParam( vgLmData_p->vgMupbgasContext.mupbgasMode, 0, 0);
        }
        break;
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgMupbgasProcessLmClearGasCnf
 *
 * Parameters:  sig_p -  Received signal
 *              vgLmData_p - LM context
 *              entity - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Process a received ApexLmClearGasCnf signal for a MUPBGAS operation
 *-------------------------------------------------------------------------*/
static void vgMupbgasProcessLmClearGasCnf(  ApexLmClearGasCnf *sig_p,
                                            VgLmData *vgLmData_p,
                                            const VgmuxChannelNumber entity)
{
    switch(vgLmData_p->vgMupbgasContext.mupbgasMode)
    {
        case VG_MUPBGAS_MODE_DELETE_ALL:
        {
            if (sig_p->requestStatus == LM_REQ_OK)
            {
                setResultCode (entity, RESULT_CODE_OK);
            }
            else
            {
                /*something went wrong, flag an error*/
                setResultCode (entity, RESULT_CODE_ERROR);
            }
        }
        break;

        default:
        {
            DevParam( vgLmData_p->vgMupbgasContext.mupbgasMode, 0, 0);
        }
        break;
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgMupbgasProcessLmWriteGasCnf
 *
 * Parameters:  sig_p -  Received signal
 *              vgLmData_p - LM context
 *              entity - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Process a received ApexLmWriteGasCnf signal for a MUPBGAS operation
 *-------------------------------------------------------------------------*/
static void vgMupbgasProcessLmWriteGasCnf(  ApexLmWriteGasCnf *sig_p,
                                            VgLmData *vgLmData_p,
                                            const VgmuxChannelNumber entity)
{
    if (sig_p->requestStatus == LM_REQ_OK)
    {
        setResultCode (entity, RESULT_CODE_OK);
    }
    else
    {
        /*something went wrong, flag an error*/
        setResultCode (entity, RESULT_CODE_ERROR);
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCpbwProcessLmWriteGasCnf
 *
 * Parameters:  sig_p -  Received signal
 *              vgLmData_p - LM context
 *              entity - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Process a received ApexLmWriteGasCnf signal for a CPBW operation
 *-------------------------------------------------------------------------*/
static void vgCpbwProcessLmWriteGasCnf( ApexLmWriteGasCnf *sig_p,
                                        VgLmData *vgLmData_p,
                                        const VgmuxChannelNumber entity)
{
    if (sig_p->requestStatus == LM_REQ_OK)
    {
        /* Continue by writing the GRP record*/
        vgGnPbWriteGroup(   entity,
                            vgLmData_p,
                            vgLmData_p->grpInfo.grpData.grpList[0]);
    }
    else
    {
        /*something went wrong, flag an error*/
        vgLmData_p->deleteRes = VG_CME_CPBW_CANNOT_UPDATE_GROUP;
        setResultCode(  entity,
                        vgChManContinueAction (entity, SIG_APEX_LM_DELETE_DIALNUM_REQ));
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgMupbgasProcessLmReadGasCnf
 *
 * Parameters:  sig_p -  Received signal
 *              vgLmData_p - LM context
 *              entity - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Process a received ApexLmReadGasCnf signal for a MUPBGAS operation
 *-------------------------------------------------------------------------*/
static void vgMupbgasProcessLmReadGasCnf(   ApexLmReadGasCnf *sig_p,
                                            VgLmData *vgLmData_p,
                                            const VgmuxChannelNumber entity)
{
    if( (sig_p->requestStatus == LM_REQ_OK_DATA_INVALID) ||
        (sig_p->requestStatus == LM_REQ_OK) ||
        /*print missing parameter if illegal operation or file not supported*/
        (sig_p->requestStatus == LM_REQ_ILLEGAL_OPERATION) ||
        (sig_p->requestStatus == LM_REQ_FILE_NOT_SUPPORTED))
    {
        if( sig_p->requestStatus == LM_REQ_OK)
        {
            vgPutNewLine( entity);
            vgPrintf( entity, (Char *)"*MUPBGAS: %d,\"", sig_p->gasRecordNumber);
            vgPutAlphaId(   entity,
                            sig_p->gasRecord.data,
                            sig_p->gasRecord.length);
            vgPutc( entity, '\"');
            vgPutNewLine( entity);
        }
        setResultCode (entity, RESULT_CODE_OK);
    }
    else
    {
        /*something went wrong, flag an error*/
        setResultCode (entity, RESULT_CODE_ERROR);
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCpbrProcessLmReadGasCnf
 *
 * Parameters:  sig_p -  Received signal
 *              vgLmData_p - LM context
 *              entity - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Process a received ApexLmReadGasCnf signal for a CPBR operation
 *-------------------------------------------------------------------------*/
static void vgCpbrProcessLmReadGasCnf(  ApexLmReadGasCnf *sig_p,
                                        VgLmData *vgLmData_p,
                                        const VgmuxChannelNumber entity)
{
    if (sig_p->requestStatus == LM_REQ_OK)
    {
        vgPutc( entity,',');
        vgPutc( entity, '\"');
        vgPutAlphaId(   entity,
                      sig_p->gasRecord.data,
                      sig_p->gasRecord.length);
        vgPutc (entity, '\"');
        /*read next association data */
        vgReadAssociationData (entity, vgLmData_p);
    }
    else if( sig_p->requestStatus == LM_REQ_OK_DATA_INVALID ||
            /*print missing parameter if illegal operation or file not supported*/
            (sig_p->requestStatus == LM_REQ_ILLEGAL_OPERATION) ||
            (sig_p->requestStatus == LM_REQ_FILE_NOT_SUPPORTED))
    {
        /*group info missing */
        vgLmData_p->missingParamCount ++;
        vgPrintMissingParams (entity, vgLmData_p->missingParamCount);
        vgLmData_p->missingParamCount = 0;

        /*read next association data */
        vgReadAssociationData (entity, vgLmData_p);
    }
    else
    {
        vgPutNewLine (entity);
        /*something went wrong, flag an error*/
        setResultCode (entity, VG_CME_CPBR_CANNOT_READ_GROUP);
    }
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgMupbgasProcessLmListGasCnf
 *
 * Parameters:  sig_p -  Received signal
 *              vgLmData_p - LM context
 *              entity - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Process a received ApexLmListGasCnf signal for a MUPBGAS operation
 *-------------------------------------------------------------------------*/
static void vgMupbgasProcessLmListGasCnf(   ApexLmListGasCnf *sig_p,
                                            VgLmData *vgLmData_p,
                                            const VgmuxChannelNumber entity)
{
    Int8                i;

    if (sig_p->requestStatus == LM_REQ_OK)
    {
        for (i = 0; i < sig_p->listSize; i++)
        {
            vgLmData_p->startRecord = sig_p->gasRecordNumber[i] + 1;

            /* Print the GAS*/
            vgPutNewLine( entity);
            vgPrintf( entity, (Char *)"*MUPBGAS: %d,\"", sig_p->gasRecordNumber[i]);
            vgPutAlphaId(   entity,
                            sig_p->gasRecord[i].data,
                            sig_p->gasRecord[i].length);
            vgPutc( entity, '\"');
        }
        vgPutNewLine (entity);

        if( sig_p->moreValidEntries == TRUE)
        {
            setResultCode (entity, vgChManContinueAction (entity, SIG_APEX_LM_LIST_GAS_REQ));
        }
        else
        {
            setResultCode (entity, RESULT_CODE_OK);
        }
    }
    else
    {
        /*other errors*/
        vgPutNewLine (entity);
        setResultCode (entity, VG_CME_CPBR_CANNOT_READ_GROUP);
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCpbwProcessLmListGasCnf
 *
 * Parameters:  sig_p -  Received signal
 *              vgLmData_p - LM context
 *              entity - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Process a received ApexLmListGasCnf signal for a CPBW operation
 *-------------------------------------------------------------------------*/
static void vgCpbwProcessLmListGasCnf(  ApexLmListGasCnf *sig_p,
                                        VgLmData *vgLmData_p,
                                        const VgmuxChannelNumber entity)
{
    Int8                i;
    Boolean             found = FALSE;

    if (sig_p->requestStatus == LM_REQ_OK)
    {
        for (i = 0; i < sig_p->listSize; i++)
        {
            if( vgAlphaStringSearch(    &(sig_p->gasRecord[i].data[0]),
                                        sig_p->gasRecord[i].length,
                                        &(vgLmData_p->grpInfo.grpAlphaId.data[0]),
                                        vgLmData_p->grpInfo.grpAlphaId.length,
                                        entity))
            {
                /*  found an existing group with specified AlphaId, assign the PhoneBook
                *   entry to this group */
                found = TRUE;
                vgGnPbWriteGroup( entity, vgLmData_p, sig_p->gasRecordNumber[i]);
            }
            else
            {
                if( (vgLmData_p->grpInfo.gasFreeRec == 0) &&
                    (vgLmData_p->startRecord != sig_p->gasRecordNumber[i]) )
                {
                    vgLmData_p->grpInfo.gasFreeRec = vgLmData_p->startRecord;
                }
                vgLmData_p->startRecord = sig_p->gasRecordNumber[i] + 1;
            }
        }

        if (!found)
        {
            if( sig_p->moreValidEntries == TRUE)
            {
                setResultCode (entity, vgChManContinueAction (entity, SIG_APEX_LM_LIST_GAS_REQ));
            }
            else
            {
                if( vgLmData_p->grpInfo.gasFreeRec == 0)
                {
                    /* No free records has been found in the fetched list*/

                    if( vgLmData_p->startRecord <= vgLmData_p->grpInfo.gasNumRecords)
                    {
                        /* There is at least one free record at the end*/
                        vgLmData_p->grpInfo.grpData.grpList[0] = vgLmData_p->startRecord;
                        setResultCode (entity, vgChManContinueAction (entity, SIG_APEX_LM_WRITE_GAS_REQ));
                    }
                    else
                    {
                        /* No free GAS records, stop writing record*/
                        vgPutNewLine (entity);
                        setResultCode (entity, VG_CME_CPBW_CANNOT_UPDATE_GROUP);
                    }
                }
                else
                {
                    vgLmData_p->grpInfo.grpData.grpList[0] = vgLmData_p->grpInfo.gasFreeRec;
                    setResultCode (entity, vgChManContinueAction (entity, SIG_APEX_LM_WRITE_GAS_REQ));
                }
            }
        }
    }
    else
    {
        /*other errors*/
        /*something went wrong, flag an error*/
        vgPutNewLine (entity);
        setResultCode (entity, VG_CME_CPBW_CANNOT_UPDATE_GROUP);
    }
}


/*--------------------------------------------------------------------------
*
* Function:        isPhoneBookPin2Protected
*
* Parameters:      Int8 - phone book index to check read status of
*
* Returns:         Boolean - indicating if phonebook is password protected
*
* Description:     Tests whether a phone book is password protected
*
*-------------------------------------------------------------------------*/

static Boolean isPhoneBookPin2Protected (const Int8 phoneBookIndex)

{
  Boolean result = FALSE;
  const VgLmInfo *lmInfo = getVgLmInfoRec ();

  WarnCheck (phoneBookIndex < NUMBER_OF_PHONE_BOOKS, phoneBookIndex, 0, 0 );

  if (phoneBookIndex < NUMBER_OF_PHONE_BOOKS)
  {
      result = lmInfo[phoneBookIndex].pin2Protected;
  }
  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:        vgReadAssociationData
*
* Parameters:      const VgmuxChannelNumber   entity
*                  VgLmData                  *vgLmData_p
*
*
* Returns:         nothing
*
* Description:   reads the association data (grouping info, additional number,
*                email, or second name) if supported by the USIM
*-------------------------------------------------------------------------*/
static void vgReadAssociationData (const VgmuxChannelNumber entity,
                                      VgLmData *vgLmData_p)
{
    Boolean readingData = FALSE;
    Int8    i;

    /*read the different association data, returns a boolean
    * to indicate whether any association data is supported, and
    * requires to be read */

    while(  (vgLmData_p->assocData < VG_PB_NUM_ASSOC_DATA) &&
            (!readingData))
    {
        switch ( vgLmData_p->assocData)
        {
            case VG_PB_GROUP:
            {
                if (vgLmData_p->grpInfo.groupSupported)
                {
                    /*USIM is able to store group info*/
                    /*Get group Id for the entry */
                    setResultCode(  entity,
                                    vgChManContinueAction (entity, SIG_APEX_LM_READ_GRP_REQ));
                    readingData = TRUE;
                }
                else
                {
                    vgLmData_p->missingParamCount++;
                }
                vgLmData_p->assocData++;
            }
            break;

            case VG_PB_ANR:
            {
                if (vgLmData_p->adNumInfo.anrSupported)
                {
                    /*USIM is able to store additional number info*/
                    /*read the additional number */
                    vgLmData_p->adNumInfo.anrIndex = 0;
                    vgLmData_p->aasInfo.aasIndex = 0;
                    setResultCode(  entity,
                                    vgChManContinueAction (entity, SIG_APEX_LM_READ_ANR_REQ));
                    readingData = TRUE;
                }
                else
                {
                    /*Type of number and number are both missing*/
                    vgLmData_p->missingParamCount+=2;
                }
                vgLmData_p->assocData++;
            }
            break;

            case VG_PB_SNE:
            {
                if (vgLmData_p->sneSupported)
                {
                    /*USIM is able to store second name info*/
                    /*read secon name info*/
                    setResultCode(  entity,
                                    vgChManContinueAction (entity, SIG_APEX_LM_READ_SNE_REQ));
                    readingData = TRUE;
                }
                else
                {
                    vgLmData_p->missingParamCount++;
                }
                vgLmData_p->assocData++;
            }
            break;

            case VG_PB_EMAIL:
            {
                if (vgLmData_p->emailInfo.emailSupported)
                {
                    /*USIM is able to store email info*/
                    /*Get email info */
                    vgLmData_p->emailInfo.emailIndex = 0;
                    setResultCode(  entity,
                                    vgChManContinueAction (entity, SIG_APEX_LM_READ_EMAIL_REQ));
                    readingData = TRUE;
                }
                else
                {
                    vgLmData_p->missingParamCount++;
                }
                vgLmData_p->assocData++;
            }
            break;

            case VG_PB_AD_GRP:
            {
                if( getProfileValue( entity, PROF_MUPBCFG) == VG_MUPBCFG_EXT_PARAM_ENABLE)
                {
                    if( vgLmData_p->grpInfo.groupSupported == TRUE)
                    {
                        /* Print the additionals groups*/
                        for( i=1; i<vgLmData_p->grpInfo.groupPerRec; i++)
                        {
                            if( (i < vgLmData_p->grpInfo.grpData.numOfGrp) &&
                                (vgLmData_p->grpInfo.grpData.grpList[i] != 0) )
                            {
                                /* Print missing parameters*/
                                vgPrintMissingParams (entity, vgLmData_p->missingParamCount);
                                vgLmData_p->missingParamCount = 0;

                                /* Print the group*/
                                vgPrintf(   entity,
                                            (Char *)",%d",
                                            vgLmData_p->grpInfo.grpData.grpList[i]);
                            }
                            else
                            {
                                vgLmData_p->missingParamCount++;
                            }
                        }
                        vgLmData_p->missingParamCount +=
                            VG_MUPBCFG_MAX_AD_GROUPS - (vgLmData_p->grpInfo.groupPerRec - 1);
                    }
                    else
                    {
                        vgLmData_p->missingParamCount += VG_MUPBCFG_MAX_AD_GROUPS;
                    }
                }
                vgLmData_p->assocData++;
            }
            break;

            case VG_PB_AD_ANR:
            {
                if( getProfileValue( entity, PROF_MUPBCFG) == VG_MUPBCFG_EXT_PARAM_ENABLE)
                {
                    if( vgLmData_p->adNumInfo.anrSupported == TRUE)
                    {
                        /* Continue by reading the next ANR*/
                        vgLmData_p->adNumInfo.anrIndex++;
                        vgLmData_p->aasInfo.aasIndex++;
                        if( vgLmData_p->adNumInfo.anrIndex < vgLmData_p->adNumInfo.adNumPerRec)
                        {
                            setResultCode(  entity,
                                            vgChManContinueAction (entity, SIG_APEX_LM_READ_ANR_REQ));
                            readingData = TRUE;
                        }
                        else
                        {
                            /* For each number there is the number and the type, so x2*/
                            vgLmData_p->missingParamCount +=
                                ( (VG_MUPBCFG_MAX_AD_NUMBER - (vgLmData_p->adNumInfo.adNumPerRec - 1) ) * 2);
                            /* All the ANRs has been read*/
                            vgLmData_p->assocData++;
                        }
                    }
                    else
                    {
                        /* For each number there is the number and the type, so x2*/
                        vgLmData_p->missingParamCount += VG_MUPBCFG_MAX_AD_NUMBER * 2;
                        vgLmData_p->assocData++;
                    }
                }
                else
                {
                    vgLmData_p->assocData++;
                }
            }
            break;

            case VG_PB_AD_EMAIL:
            {
                if( getProfileValue( entity, PROF_MUPBCFG) == VG_MUPBCFG_EXT_PARAM_ENABLE)
                {
                    if( vgLmData_p->emailInfo.emailSupported == TRUE)
                    {
                        /* Continue by reading the next Email*/
                        vgLmData_p->emailInfo.emailIndex++;
                        if( vgLmData_p->emailInfo.emailIndex < vgLmData_p->emailInfo.emailPerRec)
                        {
                            setResultCode(  entity,
                                            vgChManContinueAction (entity, SIG_APEX_LM_READ_EMAIL_REQ));
                            readingData = TRUE;
                        }
                        else
                        {
                            vgLmData_p->missingParamCount +=
                                (VG_MUPBCFG_MAX_AD_EMAIL - (vgLmData_p->emailInfo.emailPerRec - 1) );
                            /* All the EMAILs has been read*/
                            vgLmData_p->assocData++;
                        }
                    }
                    else
                    {
                        vgLmData_p->missingParamCount += VG_MUPBCFG_MAX_AD_EMAIL;
                        vgLmData_p->assocData++;
                    }
                }
                else
                {
                    vgLmData_p->assocData++;
                }
            }
            break;

            case VG_PB_AD_AAS:
            {
                if( getProfileValue( entity, PROF_MUPBCFG) == VG_MUPBCFG_EXT_PARAM_ENABLE)
                {
                    if( vgLmData_p->aasInfo.aasSupported == TRUE)
                    {
                        for( i=0; i<vgLmData_p->adNumInfo.adNumPerRec; i++)
                        {
                            if( vgLmData_p->aasInfo.adAasIndex[i] != 0 &&
                                vgLmData_p->aasInfo.adAasIndex[i] != 255) /* 255 also is invalid index*/
                            {
                                /* Print missing parameters*/
                                vgPrintMissingParams (entity, vgLmData_p->missingParamCount);
                                vgLmData_p->missingParamCount = 0;

                                /* Print the AAS index*/
                                vgPrintf(   entity,
                                            (Char *)",%d",
                                            vgLmData_p->aasInfo.adAasIndex[i]);
                            }
                            else
                            {
                                vgLmData_p->missingParamCount++;
                            }
                        }
                        /* Here the -1 is because we don't want a comma for the last parameter*/
                        vgLmData_p->missingParamCount +=
                            (VG_MUPBCFG_MAX_AD_AAS - vgLmData_p->adNumInfo.adNumPerRec);
                    }
                    else
                    {
                        /* Here the -1 is because we don't want a comma for the last parameter*/
                        vgLmData_p->missingParamCount += VG_MUPBCFG_MAX_AD_AAS;
                    }
                }
                vgLmData_p->assocData++;
            }
            break;

            case VG_PB_COMPLETE:
            {
                /*print missing parameters*/
                vgPrintMissingParams (entity, vgLmData_p->missingParamCount);
                vgPutNewLine (entity);

                if (vgLmData_p->phoneIndex1 >= vgLmData_p->phoneIndex2)
                {
                    vgPutNewLine (entity);
                    setResultCode (entity, RESULT_CODE_OK);
                }
                else
                {
                    /*we need to get other numbers*/
                    vgListDialNum (entity);
                }
                vgLmData_p->assocData++;
            }
            break;

            default:
            {
                /* assoc data doesn' t exist */
                FatalParam(entity, vgLmData_p->assocData, 0);
            }
            break;
        }
    }
}

/*--------------------------------------------------------------------------
*
* Function:        vgUpdateAssociationData
*
* Parameters:      const VgmuxChannelNumber   entity
*                  VgLmData                  *vgLmData_p
*
*
* Returns:         nothing
*
* Description:   updates the association data (grouping info, additional number,
*                email, or second name) if supported by the USIM
*-------------------------------------------------------------------------*/

static void vgUpdateAssociationData(    const VgmuxChannelNumber entity,
                                        VgLmData *vgLmData_p)
{
    Boolean updatingData = FALSE;
    /*read the different association data, returns a boolean
    * to indicate whether any association data is supported, and
    * requires to be read */

    while(  (vgLmData_p->assocData < VG_PB_NUM_ASSOC_DATA) &&
            (updatingData == FALSE))
    {
        switch ( vgLmData_p->assocData)
        {
            case VG_PB_GROUP:
            {
                if (vgLmData_p->grpInfo.groupSupported)
                {
                    /*group info can be stored on the USIM*/
                    if( vgLmData_p->grpInfo.grpAlphaId.length != 0)
                    {
                        /* user specified a group alpha ID entry to be assigned to a group*/
                        vgLmData_p->grpInfo.gasFreeRec = 0;

                        /*  Get the list of all existing groups to see if the Alpha Id matches
                        *   one of the existing gas */
                        vgLmData_p->startRecord = 1;
                        vgLmData_p->grpInfo.grpData.numOfGrp = 0;
                        setResultCode (entity, vgChManContinueAction (entity, SIG_APEX_LM_LIST_GAS_REQ));
                        updatingData = TRUE;
                    }
                    else if(    (getCommandId (entity) == VG_AT_GN_CPBW) &&
                                (getProfileValue( entity, PROF_MUPBCFG) == VG_MUPBCFG_EXT_PARAM_ENABLE) &&
                                (vgLmData_p->grpInfo.nbAdGas != 0))
                    {
                        vgGnPbWriteGroup( entity, vgLmData_p, 0);
                        updatingData = TRUE;
                    }
                }
                vgLmData_p->assocData++;
            }
            break;

            case VG_PB_ANR:
            {
                if (vgLmData_p->adNumInfo.anrSupported)
                {
                    /*the USIM can store additional number info*/
                    vgLmData_p->adNumInfo.anrIndex = 0;
                    if( vgLmData_p->adNumInfo.adNum.dialNumLength !=0)
                    {
                        /*the user specified an additional number...*/
                        if( getProfileValue( entity, PROF_MUPBCFG) == VG_MUPBCFG_EXT_PARAM_ENABLE)
                        {
                            vgLmData_p->aasInfo.aasIndex = vgLmData_p->aasInfo.adAasIndex[ 0];
                        }
                        setResultCode (entity, vgChManContinueAction (entity, SIG_APEX_LM_WRITE_ANR_REQ));
                        updatingData = TRUE;
                    }
                }
                vgLmData_p->assocData++;
            }
            break;

            case VG_PB_SNE:
            {
                if (vgLmData_p->sneSupported)
                {
                    /*the USIM can store Second name entry*/
                    if (vgLmData_p->secondName.length)
                    {
                        /*user specified a second name */
                        setResultCode (entity, vgChManContinueAction (entity, SIG_APEX_LM_WRITE_SNE_REQ));
                        updatingData = TRUE;
                    }
                }
                vgLmData_p->assocData++;
            }
            break;

            case VG_PB_AD_GRP:
            {
                /* Not used for writing*/
                vgLmData_p->assocData++;
            }
            break;

            case VG_PB_EMAIL:
            {
                if (vgLmData_p->emailInfo.emailSupported)
                {
                    /*USIM is able to store email info*/
                    vgLmData_p->emailInfo.emailIndex = 0;
                    if (vgLmData_p->emailInfo.email.length)
                    {
                        /*user specified an email address, store it */
                        setResultCode (entity, vgChManContinueAction (entity, SIG_APEX_LM_WRITE_EMAIL_REQ));
                        updatingData = TRUE;
                    }
                }
                vgLmData_p->assocData++;
            }
            break;

            case VG_PB_AD_ANR:
            {
                if( (getCommandId (entity) == VG_AT_GN_CPBW) &&
                    (getProfileValue( entity, PROF_MUPBCFG) == VG_MUPBCFG_EXT_PARAM_ENABLE) &&
                    (vgLmData_p->adNumInfo.anrSupported == TRUE))
                {
                    /* Process the additional numbers parameters*/
                    updatingData = vgGnPbWriteNextAdNum( entity, vgLmData_p);
                }
                else if(    (getCommandId (entity) == VG_AT_GN_SCPBW) &&
                            (vgLmData_p->adNumInfo.anrSupported == TRUE))
                {
                    /* Process the additional numbers parameters*/
                    updatingData = vgGnPbWriteNextAdNum( entity, vgLmData_p);
                }
                if( updatingData == FALSE)
                {
                    vgLmData_p->assocData++;
                }
            }
            break;

            case VG_PB_AD_EMAIL:
            {
                if( (getCommandId (entity) == VG_AT_GN_CPBW) &&
                    (getProfileValue( entity, PROF_MUPBCFG) == VG_MUPBCFG_EXT_PARAM_ENABLE) &&
                    (vgLmData_p->emailInfo.emailSupported == TRUE))
                {
                    /* Process the emails*/
                    updatingData = vgGnPbWriteNextEmail( entity, vgLmData_p);
                }

                if( updatingData == FALSE)
                {
                    vgLmData_p->assocData++;
                }
            }
            break;

            case VG_PB_AD_AAS:
            {
                /* Not used for writing*/
                vgLmData_p->assocData++;
            }
            break;

            case VG_PB_COMPLETE:
            {
                /* All the CPBW parameters has been wrote*/
                vgPutNewLine (entity);
                setResultCode (entity, RESULT_CODE_OK);
                vgLmData_p->assocData++;
            }
            break;

            default:
            {
                /* assoc data doesn' t exist */
                FatalParam(entity, vgLmData_p->assocData, 0);
            }
            break;
        }
    }
}



/*--------------------------------------------------------------------------
*
* Function:     vgGnPbWriteGroup
*
* Parameters:   entity
*               vgLmData_p      - Context for phonebook
*               firstGasIndex   - index for the first element in group
*
* Returns:      Nothing
*
* Description:  Write the group for a record
*-------------------------------------------------------------------------*/
static void vgGnPbWriteGroup(   const VgmuxChannelNumber    entity,
                                VgLmData                   *vgLmData_p,
                                Int8                        firstGasIndex)
{
    vgLmData_p->grpInfo.grpData.grpList[0] = firstGasIndex;

    if( getProfileValue( entity, PROF_MUPBCFG) == VG_MUPBCFG_EXT_PARAM_ENABLE)
    {
        /* Copy extended parametes index*/
        memcpy( &vgLmData_p->grpInfo.grpData.grpList[1],
                &vgLmData_p->grpInfo.adGasIndex[0],
                VG_MUPBCFG_MAX_AD_GROUPS);

        vgLmData_p->grpInfo.grpData.numOfGrp = vgLmData_p->grpInfo.nbAdGas + 1;
    }
    else
    {
        vgLmData_p->grpInfo.grpData.numOfGrp = 1;
    }
    setResultCode( entity, vgChManContinueAction (entity, SIG_APEX_LM_WRITE_GRP_REQ));
}


/*--------------------------------------------------------------------------
*
* Function:     vgGnPbWriteNextAdNum
*
* Parameters:   entity
*               vgLmData_p      - Context for phonebook
*
* Returns:      TRUE if we have send/try to send a signal
*
* Description:  Write the next extended ANR for a record
*-------------------------------------------------------------------------*/
static Boolean vgGnPbWriteNextAdNum(    const VgmuxChannelNumber    entity,
                                        VgLmData                   *vgLmData_p)
{
    Boolean res = FALSE;

    while(  (vgLmData_p->adNumInfo.anrIndex < VG_MUPBCFG_MAX_AD_NUMBER) &&
            (res == FALSE))
    {
        if( vgLmData_p->adNumInfo.adAdNums[ vgLmData_p->adNumInfo.anrIndex].dialNumLength != 0)
        {
            /* Copy the ANR*/
            vgLmData_p->adNumInfo.adNum = vgLmData_p->adNumInfo.adAdNums[ vgLmData_p->adNumInfo.anrIndex];
            /*  Need to increment by one now because the index 0 has already been given
            *   to standard additional number*/
            vgLmData_p->adNumInfo.anrIndex++;
            /* Copy the AAS index*/
            vgLmData_p->aasInfo.aasIndex = vgLmData_p->aasInfo.adAasIndex[ vgLmData_p->adNumInfo.anrIndex];

            /* Send the request to write the number*/
            setResultCode( entity, vgChManContinueAction (entity, SIG_APEX_LM_WRITE_ANR_REQ));
            res = TRUE;
        }
        else
        {
            vgLmData_p->adNumInfo.anrIndex++;
        }
    }

    return res;
}

/*--------------------------------------------------------------------------
*
* Function:     vgGnPbWriteNextEmail
*
* Parameters:   entity
*               vgLmData_p      - Context for phonebook
*<
* Returns:      TRUE if we have send/try to send a signal
*
* Description:  Write the next extended email for a record
*-------------------------------------------------------------------------*/
static Boolean vgGnPbWriteNextEmail(    const VgmuxChannelNumber    entity,
                                        VgLmData                   *vgLmData_p)
{
    Boolean res = FALSE;

    FatalAssert( getProfileValue( entity, PROF_MUPBCFG) == VG_MUPBCFG_EXT_PARAM_ENABLE);

    while(  (vgLmData_p->emailInfo.emailIndex < VG_MUPBCFG_MAX_AD_EMAIL) &&
            (res == FALSE))
    {
        if( vgLmData_p->emailInfo.adEmails[ vgLmData_p->emailInfo.emailIndex].length != 0)
        {
            /* Copy the ANR*/
            vgLmData_p->emailInfo.email = vgLmData_p->emailInfo.adEmails[ vgLmData_p->emailInfo.emailIndex];
            /*  Need to increment by one now because the index 0 has already been given
            *   to standard additional number*/
            vgLmData_p->emailInfo.emailIndex++;

            /* Send the request to write the number*/
            setResultCode( entity, vgChManContinueAction (entity, SIG_APEX_LM_WRITE_EMAIL_REQ));
            res = TRUE;
        }
        else
        {
            vgLmData_p->emailInfo.emailIndex++;
        }
    }

    return res;
}

/*--------------------------------------------------------------------------
*
* Function:        vgProcessPbStatusCpbr
*
* Parameters:      PhoneBookInfo     *phoneBookInfo_p
*                  VgLmData          *vgLmData_p
*                  const VgmuxChannelNumber entity
*
*
* Returns:         nothing
*
* Description:   proceeds with AT+CPBR command
*-------------------------------------------------------------------------*/
static void vgProcessPbStatusCpbr(  PhoneBookInfo *phoneBookInfo_p,
                                    VgLmData *vgLmData_p,
                                    const VgmuxChannelNumber entity)

{
    /*handles CPBR command after ApexLmDialnumStatusCnf or ApexLmPhonebookStatusCnf
    * have been received*/

    switch (vgLmData_p->phoneBookOperation)
    {
        case VG_PB_RANGE:
        {
            vgPutNewLine (entity);
            if (phoneBookInfo_p->records == 0) /* no entries in phone book */
            {
                vgPuts (entity, (Char *)"+CPBR: (0)");
                setResultCode (entity, RESULT_CODE_OK);
            }
            else if (vgLmData_p->phoneBook == DIAL_LIST_ICI)
            {
                /*cannot specify the range available for missed calls or received calls
                * as they are stored in the same file (EF_ICI)...*/
                setResultCode (entity, VG_CME_UNKNOWN);
            }
            else /* at least one entry in the phone book */
            {
                if( (vgLmData_p->phoneBook == DIAL_LIST_ADN_APP) ||
                    (vgLmData_p->phoneBook == DIAL_LIST_ADN_GLB))
                {
                    /*group, second name, email only applies to the phonebook files*/
                    /*not relevant to other files*/
                    vgPrintf(   entity,
                                (Char *)"+CPBR: (1-%d),%d,%d,%d,%d,%d",
                                phoneBookInfo_p->records,
                                phoneBookInfo_p->dial,
                                phoneBookInfo_p->alpha,
                                phoneBookInfo_p->gLength,  /* maximum length of Group id */
                                phoneBookInfo_p->sLength,  /* maximum length of SecondText */
                                phoneBookInfo_p->eLength   /* maximum length of eMail */
                    );
                    if( getProfileValue( entity, PROF_MUPBCFG) == VG_MUPBCFG_EXT_PARAM_ENABLE)
                    {
                        /* Print extended list range*/
                        if (phoneBookInfo_p->records == 0) /* no entries in phone book */
                        {
                            vgPrintf(   entity,
                                        (Char *)",0,0,0,0" );
                        }
                        else
                        {
                            vgPrintf(   entity,
                                        (Char *)",%d,%d,%d,%d",
                                        (vgLmData_p->grpInfo.groupPerRec == 0) ?
                                            0 : vgLmData_p->grpInfo.groupPerRec - 1,
                                            vgLmData_p->adNumInfo.adNumPerRec,
                                        (vgLmData_p->emailInfo.emailPerRec == 0) ?
                                            0 : vgLmData_p->emailInfo.emailPerRec - 1,
                                            vgLmData_p->aasInfo.aasNumRecord
                                        );
                        }
                    }
                    vgPutNewLine (entity);
                    setResultCode (entity, RESULT_CODE_OK);
                }
                else
                {
                    vgPrintf(   entity,
                                (Char *)"+CPBR: (1-%d),%d,%d",
                                phoneBookInfo_p->records,
                                phoneBookInfo_p->dial,
                                phoneBookInfo_p->alpha
                                );
                    vgPutNewLine (entity);
                    setResultCode (entity, RESULT_CODE_OK);
                }
            }
        }
        break;

        case VG_PB_READ:
        {
            /* check the range specified is within the phonebook limits */
            if( (phoneBookInfo_p->records > 0) &&
                (vgLmData_p->phoneIndex1 <= phoneBookInfo_p->records) &&
                (vgLmData_p->phoneIndex2 <= phoneBookInfo_p->records))
            {
                setResultCode (entity,
                vgChManContinueAction (entity, SIG_APEX_LM_READ_DIALNUM_REQ));
            }
            else
            {
                setResultCode (entity, VG_CME_INVALID_INDEX);
            }
        }
        break;

        default:
        {
            /* Invalid phonebook operation */
            WarnParam(entity, vgLmData_p->phoneBookOperation, 0);
            setResultCode (entity, RESULT_CODE_ERROR);
        }
        break;
    }
}

/*--------------------------------------------------------------------------
*
* Function:        vgProcessPbStatusScpbr
*
* Parameters:      PhoneBookInfo     *phoneBookInfo_p
*                  VgLmData          *vgLmData_p
*                  const VgmuxChannelNumber entity
*
*
* Returns:         nothing
*
* Description:   proceeds phonebook status with AT+SCPBR command
*-------------------------------------------------------------------------*/
static void vgProcessPbStatusScpbr( PhoneBookInfo *phoneBookInfo_p,
                                    VgLmData *vgLmData_p,
                                    const VgmuxChannelNumber entity)

{
    FatalCheck( (vgLmData_p->phoneBook == DIAL_LIST_ADN_APP) ||
                (vgLmData_p->phoneBook == DIAL_LIST_ADN_GLB),
                vgLmData_p->phoneBook, 0, 0);

    switch (vgLmData_p->phoneBookOperation)
    {
        case VG_PB_RANGE:
        {
            vgPutNewLine (entity);
            if (phoneBookInfo_p->records == 0) /* no entries in phone book */
            {
                vgPuts (entity, (Char *)"+SCPBR: (0)");
                setResultCode (entity, RESULT_CODE_OK);
            }
            else /* at least one entry in the phone book */
            {
                vgPrintf(   entity,
                            (Char *)"+CPBR: (1-%d),%d,%d,%d",
                            phoneBookInfo_p->records,
                            phoneBookInfo_p->dial,
                            phoneBookInfo_p->alpha,
                            phoneBookInfo_p->eLength
                            );

                vgPutNewLine (entity);
                setResultCode (entity, RESULT_CODE_OK);
            }
        }
        break;

        case VG_PB_READ:
        {
            /* check the range specified is within the phonebook limits */
            if( (phoneBookInfo_p->records > 0) &&
                (vgLmData_p->phoneIndex1 <= phoneBookInfo_p->records) &&
                (vgLmData_p->phoneIndex2 <= phoneBookInfo_p->records))
            {
                setResultCode (entity,
                vgChManContinueAction (entity, SIG_APEX_LM_READ_DIALNUM_REQ));
            }
            else
            {
                setResultCode (entity, VG_CME_INVALID_INDEX);
            }
        }
        break;

        default:
        {
            /* Invalid phonebook operation */
            WarnParam(entity, vgLmData_p->phoneBookOperation, 0);
            setResultCode (entity, RESULT_CODE_ERROR);
        }
        break;
    }
}

/*--------------------------------------------------------------------------
*
* Function:        vgProcessPbStatusCpbf
*
* Parameters:      PhoneBookInfo     *phoneBookInfo_p
*                  VgLmData          *vgLmData_p
*                  const VgmuxChannelNumber entity
*
*
* Returns:         nothing
*
* Description:   proceeds with AT+CPBF command
*-------------------------------------------------------------------------*/

static void vgProcessPbStatusCpbf(  PhoneBookInfo *phoneBookInfo_p,
                                    VgLmData *vgLmData_p,
                                    const VgmuxChannelNumber entity)
{
  /*handles CPBF command after ApexLmDialnumStatusCnf or ApexLmPhonebookStatusCnf
   * have been received*/
  switch (vgLmData_p->phoneBookOperation)
  {
    case VG_PB_RANGE:
    {
      vgPutNewLine (entity);

      if (phoneBookInfo_p->records == 0) /* no entries in phone book */
      {
        vgPuts (entity, (Char *)"+CPBF: (0)");
      }
      else /* at least one entry in the phone book */
      {
         if ((vgLmData_p->phoneBook == DIAL_LIST_ADN_APP) ||
             (vgLmData_p->phoneBook == DIAL_LIST_ADN_GLB))
         {
            /*group second text, email only applies to the phonebook files*/
            /*not relevant to other files*/
            vgPrintf (entity,
                     (Char *)"+CPBF: %d,%d,%d,%d,%d",
                     phoneBookInfo_p->dial,
                     phoneBookInfo_p->alpha,
                     phoneBookInfo_p->gLength,  /* maximum length of Group id */
                     phoneBookInfo_p->sLength,  /* maximum length of SecondText */
                     phoneBookInfo_p->eLength   /* maximum length of eMail */
                     );
            vgPutNewLine (entity);

         }
         else
         {
           vgPrintf (entity,
                    (Char *)"+CPBF: %d,%d",
                     phoneBookInfo_p->dial,
                     phoneBookInfo_p->alpha
                    );
           vgPutNewLine (entity);
         }
      }
      setResultCode (entity, RESULT_CODE_OK);
      break;
    }
    case VG_PB_FIND:
    {
      /* if search string is shorter then the biggest alpha ID then start
       * reading entries in phonebook */
      if (vgLmData_p->alphaLength <= phoneBookInfo_p->alpha )
      {
        getAllEntriesInPhoneBook (entity, phoneBookInfo_p);
      }
      else
      {
        setResultCode (entity, RESULT_CODE_ERROR);
      }
      break;
    }
    default:
    {
      /* Invalid phonebook operation */
      WarnParam(entity, vgLmData_p->phoneBookOperation, 0);
      setResultCode (entity, RESULT_CODE_ERROR);
      break;
    }
  }
}

/*--------------------------------------------------------------------------
*
* Function:        vgProcessPbStatusCpbw
*
* Parameters:      PhoneBookInfo     *phoneBookInfo_p
*                  VgLmData          *vgLmData_p
*                  const VgmuxChannelNumber entity
*
*
* Returns:         nothing
*
* Description:   proceeds with AT+CPBW command
*-------------------------------------------------------------------------*/

static void vgProcessPbStatusCpbw(  PhoneBookInfo *phoneBookInfo_p,
                                    VgLmData *vgLmData_p,
                                    const VgmuxChannelNumber entity)

{
    ResultCode_t resultCode = RESULT_CODE_OK;

    /* handles CPBW command after ApexLmDialnumStatusCnf
    * or ApexLmPhonebookStatusCnf
    * have been received*/

    switch (vgLmData_p->phoneBookOperation)
    {
        case VG_PB_RANGE:
        {
            vgPutNewLine (entity);

            if (phoneBookInfo_p->records == 0) /* no entries in phone book */
            {
                vgPuts (entity, (Char *)"+CPBW: (0)" );
            }
            else
            {
                if( (vgLmData_p->phoneBook == DIAL_LIST_ADN_APP) ||
                    (vgLmData_p->phoneBook == DIAL_LIST_ADN_GLB))
                {
                    vgPrintf(   entity,
                                (Char *)"+CPBW: (1-%d),%d,(%d,%d,%d,%d),%d,%d,%d,%d",
                                phoneBookInfo_p->records,
                                phoneBookInfo_p->dial,
                                VG_DIAL_NUMBER_UNKNOWN,
                                VG_DIAL_NUMBER_INTERNATIONAL,
                                VG_DIAL_NUMBER_NATIONAL,
                                VG_DIAL_NUMBER_NET_SPECIFIC,
                                phoneBookInfo_p->alpha,
                                phoneBookInfo_p->gLength,  /* maximum length of Group id */
                                phoneBookInfo_p->sLength,  /* maximum length of SecondText */
                                phoneBookInfo_p->eLength   /* maximum length of SecondText */
                                );
                    if( getProfileValue( entity, PROF_MUPBCFG) == VG_MUPBCFG_EXT_PARAM_ENABLE)
                    {
                        /* Print extended list range*/
                        if (phoneBookInfo_p->records == 0) /* no entries in phone book */
                        {
                            vgPrintf(   entity,
                                        (Char *)",0,0,0,0" );
                        }
                        else
                        {
                            vgPrintf(   entity,
                                        (Char *)",%d,%d,%d,%d",
                                        (vgLmData_p->grpInfo.groupPerRec == 0) ?
                                            0 : vgLmData_p->grpInfo.groupPerRec - 1,
                                        (vgLmData_p->adNumInfo.adNumPerRec == 0) ?
                                            0 : vgLmData_p->adNumInfo.adNumPerRec - 1,
                                        (vgLmData_p->emailInfo.emailPerRec == 0) ?
                                            0 : vgLmData_p->emailInfo.emailPerRec - 1,
                                        (vgLmData_p->aasInfo.aasNumRecord == 0) ?
                                            0 : (vgLmData_p->adNumInfo.adNumPerRec)
                                        );
                        }
                    }
                    vgPutNewLine (entity);
                }
                else

                {
                    vgPrintf(   entity,
                                (Char *)"+CPBW: (1-%d),%d,(%d,%d,%d,%d),%d",
                                phoneBookInfo_p->records,
                                phoneBookInfo_p->dial,
                                VG_DIAL_NUMBER_UNKNOWN,
                                VG_DIAL_NUMBER_INTERNATIONAL,
                                VG_DIAL_NUMBER_NATIONAL,
                                VG_DIAL_NUMBER_NET_SPECIFIC,
                                phoneBookInfo_p->alpha
                                );
                    vgPutNewLine (entity);
                }
            }
            setResultCode (entity, RESULT_CODE_OK);
        }
        break;

        case VG_PB_DELETE:
        {
            /* check request record is within phone book record range */
            if( (phoneBookInfo_p->records > 0) &&
                (vgLmData_p->phoneIndex1 <= phoneBookInfo_p->records))
            {
                /* deleting an entry */
                vgLmData_p->forReplace = FALSE;
                vgLmData_p->deleteRes = RESULT_CODE_OK;
                setResultCode (entity,
                    vgChManContinueAction (entity, SIG_APEX_LM_DELETE_DIALNUM_REQ));
            }
            else
            {
                setResultCode (entity, RESULT_CODE_ERROR);
            }
        }
        break;

        case VG_PB_WRITE:
        {
            /* Init some extended parameters variables*/
            vgLmData_p->grpInfo.nbAdGas = 0;
            vgLmData_p->adNumInfo.nbAdAnr = 0;
            vgLmData_p->adNumInfo.anrIndex = 0;
            vgLmData_p->emailInfo.nbAdEmails = 0;
            vgLmData_p->emailInfo.emailIndex = 0;
            vgLmData_p->aasInfo.nbAdAas = 0;
            vgLmData_p->aasInfo.aasIndex = 0;

            if((vgLmData_p->phoneBook == DIAL_LIST_ADN_GLB) ||
                (vgLmData_p->phoneBook == DIAL_LIST_ADN_APP))
            {
                if ((!vgLmData_p->grpInfo.groupSupported)&&(vgLmData_p->grpInfo.grpAlphaId.length))
                {
                    /*user specified a Group but this SIM/USIM does not support it*/
                    setResultCode (entity, VG_CME_CPBW_CANNOT_UPDATE_GROUP);
                }
                else if ((!vgLmData_p->adNumInfo.anrSupported)&&(vgLmData_p->adNumInfo.adNum.dialNumLength))
                {
                    /*user specified an additional number but this SIM/USIM does not support it*/
                    setResultCode (entity, VG_CME_CPBW_CANNOT_UPDATE_ANR);
                }
                else if ((!vgLmData_p->sneSupported)&&(vgLmData_p->secondName.length))
                {
                    /*user specified a second name but this SIM/USIM does not support it*/
                    setResultCode (entity, VG_CME_CPBW_CANNOT_UPDATE_SNE);
                }
                else if ((!vgLmData_p->emailInfo.emailSupported)&&(vgLmData_p->emailInfo.email.length))
                {
                    /*user specified an email but this SIM/USIM does not support it*/
                    setResultCode (entity, VG_CME_CPBW_CANNOT_UPDATE_EMAIL);
                }
                else if(    (getProfileValue( entity, PROF_MUPBCFG) == VG_MUPBCFG_EXT_PARAM_ENABLE) &&
                            ( (resultCode = vgGnCheckPbExtParamValid(   phoneBookInfo_p,
                                                                        vgLmData_p,
                                                                        entity)) != RESULT_CODE_OK) )
                {
                    setResultCode (entity, resultCode);
                }
                else
                {
                    writeEntryToPhoneBook (entity, phoneBookInfo_p);
                }
            }
            else
            {
                writeEntryToPhoneBook (entity, phoneBookInfo_p);
            }
        }
        break;

        default:
        {
            /* Invalid phonebook operation */
            WarnParam(entity, vgLmData_p->phoneBookOperation, 0);
            setResultCode (entity, RESULT_CODE_ERROR);
        }
        break;
    }
}

/*--------------------------------------------------------------------------
*
* Function:        vgProcessPbStatusScpbw
*
* Parameters:      PhoneBookInfo     *phoneBookInfo_p
*                  VgLmData          *vgLmData_p
*                  const VgmuxChannelNumber entity
*
*
* Returns:         nothing
*
* Description:   proceeds with AT^SCPBW command
*-------------------------------------------------------------------------*/
static void vgProcessPbStatusScpbw( PhoneBookInfo *phoneBookInfo_p,
                                    VgLmData *vgLmData_p,
                                    const VgmuxChannelNumber entity)

{
    Boolean     stop = FALSE;
    Int8        i;

    switch (vgLmData_p->phoneBookOperation)
    {
        case VG_PB_RANGE:
        {
            vgPutNewLine (entity);

            if (phoneBookInfo_p->records == 0) /* no entries in phone book */
            {
                vgPuts (entity, (Char *)"^SCPBW: (0)" );
            }
            else
            {
                vgPrintf(   entity,
                            (Char *)"^SCPBW: (1-%d),%d,(%d,%d,%d,%d),%d,%d",
                            phoneBookInfo_p->records,
                            phoneBookInfo_p->dial,
                            VG_DIAL_NUMBER_UNKNOWN,
                            VG_DIAL_NUMBER_INTERNATIONAL,
                            VG_DIAL_NUMBER_NATIONAL,
                            VG_DIAL_NUMBER_NET_SPECIFIC,
                            phoneBookInfo_p->alpha,
                            phoneBookInfo_p->eLength
                            );
            }
            vgPutNewLine (entity);
            setResultCode (entity, RESULT_CODE_OK);
        }
        break;

        case VG_PB_DELETE:
        {
            /* check request record is within phone book record range */
            if( (phoneBookInfo_p->records > 0) &&
                (vgLmData_p->phoneIndex1 <= phoneBookInfo_p->records))
            {
                /* deleting an entry */
                vgLmData_p->forReplace = FALSE;
                vgLmData_p->deleteRes = RESULT_CODE_OK;
                setResultCode (entity,
                    vgChManContinueAction (entity, SIG_APEX_LM_DELETE_DIALNUM_REQ));
            }
            else
            {
                setResultCode (entity, RESULT_CODE_ERROR);
            }
        }
        break;

        case VG_PB_WRITE:
        {
            /* Init some extended parameters variables*/
            vgLmData_p->adNumInfo.nbAdAnr = 0;
            vgLmData_p->adNumInfo.anrIndex = 0;

            vgLmData_p->grpInfo.grpAlphaId.length = 0;
            vgLmData_p->secondName.length = 0;

             /* check request record is within phone book record range */
            if( (phoneBookInfo_p->records == 0) ||
                (vgLmData_p->phoneIndex1 > phoneBookInfo_p->records) )
            {
                setResultCode (entity, RESULT_CODE_ERROR);
            }
            /* check dial string is not too long */
            else if( vgLmData_p->writeNumLength > phoneBookInfo_p->dial)
            {
                setResultCode (entity, VG_CME_LONG_DIALSTRING);
            }
            /* check alpha id is not too long */
            else if( vgLmData_p->alphaLength > phoneBookInfo_p->alpha)
            {
                setResultCode (entity, RESULT_CODE_ERROR);
            }
            else if(    (vgLmData_p->adNumInfo.anrSupported == FALSE) &&
                        (vgLmData_p->adNumInfo.adNum.dialNumLength > 0) )
            {
                /*user specified an additional number but this SIM/USIM does not support it*/
                setResultCode (entity, VG_CME_CPBW_CANNOT_UPDATE_ANR);
            }
            else if(    (vgLmData_p->emailInfo.emailSupported == FALSE) &&
                        (vgLmData_p->emailInfo.email.length > 0) )
            {
                /*user specified an email but this SIM/USIM does not support it*/
                setResultCode (entity, VG_CME_CPBW_CANNOT_UPDATE_EMAIL);
            }
            else
            {
                /* Valid additionnal numbers parameters*/
                /* -1 because we already read an additional parameter rigth before*/
                for(    i=0, vgLmData_p->adNumInfo.nbAdAnr = 0;
                        (   (i<VG_SCPBW_NUM_AD_NUMBER - 1) &&
                            (stop == FALSE) );
                        i++)
                {
                    if( vgLmData_p->adNumInfo.adAdNums[i].dialNumLength != 0)
                    {
                        vgLmData_p->adNumInfo.nbAdAnr = i + 1;
                        if( vgLmData_p->adNumInfo.adAdNums[i].dialNumLength > phoneBookInfo_p->dial)
                        {
                            /* Invalid length*/
                            setResultCode (entity, VG_CME_CPBW_CANNOT_UPDATE_ANR);
                            stop = TRUE;
                        }
                        else if( i >= (vgLmData_p->adNumInfo.adNumPerRec - 1) )
                        {
                            setResultCode (entity, VG_CME_CPBW_CANNOT_UPDATE_ANR);
                            stop = TRUE;
                        }
                    }
                }

                if( stop == FALSE)
                {
                    /* if given index is zero then write in first free index */
                    if (vgLmData_p->phoneIndex1 == 0)
                    {
                        /* writing an entry in the first available location */
                        vgLmData_p->writeMode = LM_WRITE_FIRST_FREE;
                        vgLmData_p->forReplace = FALSE;
                        setResultCode(  entity,
                                        vgChManContinueAction (entity, SIG_APEX_LM_WRITE_DIALNUM_REQ));
                    }
                    else
                    {
                        /* Start by deleting the record at the specified location*/
                        vgLmData_p->writeMode = LM_WRITE_ABSOLUTE;
                        vgLmData_p->forReplace = TRUE; /* We will determine this point with the
                                                        * delete operation result, for the moment
                                                        * assume it is a replace operation*/
                        vgLmData_p->deleteRes = RESULT_CODE_OK;
                        setResultCode (entity,
                            vgChManContinueAction (entity, SIG_APEX_LM_DELETE_DIALNUM_REQ));
                    }
                }
            }
        }
        break;

        default:
        {
            /* Invalid phonebook operation */
            WarnParam(entity, vgLmData_p->phoneBookOperation, 0);
            setResultCode (entity, RESULT_CODE_ERROR);
        }
        break;
    }
}

/*--------------------------------------------------------------------------
*
* Function:        vgGnCheckPbExtParamValid
*
* Parameters:      PhoneBookInfo     *phoneBookInfo_p
*                  VgLmData          *vgLmData_p
*                  const VgmuxChannelNumber entity
*
*
* Returns:         RESULT_CODE_OK if the parameters are valids
*
* Description:     Check the extended CPBW parameters are valid
*-------------------------------------------------------------------------*/
static ResultCode_t vgGnCheckPbExtParamValid(   PhoneBookInfo *phoneBookInfo_p,
                                                VgLmData *vgLmData_p,
                                                const VgmuxChannelNumber entity)
{
    ResultCode_t    resultCode  = RESULT_CODE_OK;
    Int8            i           = 0;

    /* Valid groups parameters*/
    for(    i=0, vgLmData_p->grpInfo.nbAdGas = 0;
            (   (i<VG_MUPBCFG_MAX_AD_GROUPS) &&
                (resultCode==RESULT_CODE_OK) );
            i++)
    {
        if( vgLmData_p->grpInfo.adGasIndex[i] != 0)
        {
            vgLmData_p->grpInfo.nbAdGas = i + 1;
            if( vgLmData_p->grpInfo.adGasIndex[i] > vgLmData_p->grpInfo.gasNumRecords)
            {
                /* Invalid index*/
                resultCode = VG_CME_CPBW_CANNOT_UPDATE_GROUP;
            }
            else if( i >= (vgLmData_p->grpInfo.groupPerRec - 1) ) /* -1 because standard parameters
                                                                  *  already contain a group*/
            {
                /* More groups than supported*/
                resultCode = VG_CME_CPBW_CANNOT_UPDATE_GROUP;
            }
        }
    }

    /* Valid additionnal numbers parameters*/
    for(    i=0, vgLmData_p->adNumInfo.nbAdAnr = 0;
            (   (i<VG_MUPBCFG_MAX_AD_NUMBER) &&
                (resultCode==RESULT_CODE_OK) );
            i++)
    {
        if( vgLmData_p->adNumInfo.adAdNums[i].dialNumLength != 0)
        {
            vgLmData_p->adNumInfo.nbAdAnr = i + 1;
            if( vgLmData_p->adNumInfo.adAdNums[i].dialNumLength > phoneBookInfo_p->dial)
            {
                /* Invalid length*/
                resultCode = VG_CME_CPBW_CANNOT_UPDATE_ANR;
            }
            else if( i >= (vgLmData_p->adNumInfo.adNumPerRec - 1) )
            {
                /* More email than supported*/
                resultCode = VG_CME_CPBW_CANNOT_UPDATE_ANR;
            }
        }
    }

    /* Valid emails parameters*/
    for(    i=0, vgLmData_p->emailInfo.nbAdEmails = 0;
            (   (i<VG_MUPBCFG_MAX_AD_EMAIL) &&
                (resultCode==RESULT_CODE_OK) );
            i++)
    {
        if( vgLmData_p->emailInfo.adEmails[i].length != 0)
        {
            vgLmData_p->emailInfo.nbAdEmails = i + 1;
            if( vgLmData_p->emailInfo.adEmails[i].length > phoneBookInfo_p->eLength)
            {
                /* Invalid length*/
                resultCode = VG_CME_CPBW_CANNOT_UPDATE_EMAIL;
            }
            else if( i >= (vgLmData_p->emailInfo.emailPerRec - 1) )
            {
                /* More email than supported*/
                resultCode = VG_CME_CPBW_CANNOT_UPDATE_EMAIL;
            }
        }
    }

    /* Valid AAS parameters*/
    for(    i=0, vgLmData_p->aasInfo.nbAdAas = 0;
            (   (i<VG_MUPBCFG_MAX_AD_AAS) &&
                (resultCode==RESULT_CODE_OK) );
            i++)
    {
        if( vgLmData_p->aasInfo.adAasIndex[i] != 0)
        {
            vgLmData_p->aasInfo.nbAdAas = i + 1;
            if( vgLmData_p->aasInfo.adAasIndex[i] > vgLmData_p->aasInfo.aasNumRecord)
            {
                /* Invalid index*/
                resultCode = VG_CME_INVALID_INDEX;
            }
            else if( i >= vgLmData_p->adNumInfo.adNumPerRec )
            {
                /* More AAS than supported*/
                resultCode = VG_CME_INVALID_INDEX;
            }
        }
    }

    return resultCode;
}

/*--------------------------------------------------------------------------
*
* Function:        vgProcessPbStatusMupbcfg
*
* Parameters:      PhoneBookInfo     *phoneBookInfo_p
*                  VgLmData          *vgLmData_p
*                  const VgmuxChannelNumber entity
*
*
* Returns:         nothing
*
* Description:     proceeds the phonebook status signal when executing a
*                  MUPBCFG command
*-------------------------------------------------------------------------*/
static void vgProcessPbStatusMupbcfg(   PhoneBookInfo *phoneBookInfo_p,
                                        VgLmData *vgLmData_p,
                                        const VgmuxChannelNumber entity)
{
    DevCheck(   (   (vgLmData_p->phoneBook == DIAL_LIST_ADN_APP) ||
                    (vgLmData_p->phoneBook == DIAL_LIST_ADN_GLB) ),
                vgLmData_p->phoneBook, 0, 0);

    vgPutNewLine (entity);
    if (phoneBookInfo_p->records == 0) /* no entries in phone book */
    {
        vgPrintf(   entity,
                    (Char *)"*MUPBCFG: (0,1),0,0,0,0" );
    }
    else
    {
        vgPrintf(   entity,
                    (Char *)"*MUPBCFG: (0,1),%d,%d,%d,%d",
                    (vgLmData_p->grpInfo.groupPerRec == 0) ?
                        0 : vgLmData_p->grpInfo.groupPerRec - 1,
                    (vgLmData_p->adNumInfo.adNumPerRec == 0) ?
                        0 : vgLmData_p->adNumInfo.adNumPerRec - 1,
                    (vgLmData_p->emailInfo.emailPerRec == 0) ?
                        0 : vgLmData_p->emailInfo.emailPerRec - 1,
                    (vgLmData_p->aasInfo.aasNumRecord == 0) ?
                        0 : (vgLmData_p->adNumInfo.adNumPerRec)
                    );
    }
    vgPutNewLine (entity);
    setResultCode (entity, RESULT_CODE_OK);
}

/*--------------------------------------------------------------------------
*
* Function:        vgProcessPbStatusMupbgas
*
* Parameters:      PhoneBookInfo     *phoneBookInfo_p
*                  VgLmData          *vgLmData_p
*                  const VgmuxChannelNumber entity
*
*
* Returns:         nothing
*
* Description:     proceeds the phonebook status signal when executing a
*                  MUPBGAS command
*-------------------------------------------------------------------------*/
static void vgProcessPbStatusMupbgas(   PhoneBookInfo *phoneBookInfo_p,
                                        VgLmData *vgLmData_p,
                                        const VgmuxChannelNumber entity)
{
    DevCheck(   (   (vgLmData_p->phoneBook == DIAL_LIST_ADN_APP) ||
                    (vgLmData_p->phoneBook == DIAL_LIST_ADN_GLB) ),
                vgLmData_p->phoneBook, 0, 0);

    if( vgLmData_p->vgMupbgasContext.mupbgasMode == VG_MUPBGAS_MODE_RANGE)
    {
        vgPutNewLine (entity);
        if( vgLmData_p->grpInfo.gasNumRecords > 0 )
        {
            vgPrintf(   entity,
                        (Char *)"*MUPBGAS: (1-3),(1-%d),%d",
                        vgLmData_p->grpInfo.gasNumRecords,
                        vgLmData_p->grpInfo.gasSize );
        }
        else
        {
            vgPrintf(   entity,
                        (Char *)"*MUPBGAS: (1-3),0,0" );
        }
        vgPutNewLine (entity);
        setResultCode (entity, RESULT_CODE_OK);
    }
    else
    {
        if( vgLmData_p->grpInfo.gasNumRecords > 0 )
        {
            switch( vgLmData_p->vgMupbgasContext.mupbgasMode)
            {
                case VG_MUPBGAS_MODE_READ:
                {
                    if( vgLmData_p->grpInfo.grpData.grpList[0] <= vgLmData_p->grpInfo.gasNumRecords)
                    {
                        /* Read the asked GAS record*/
                        setResultCode(  entity,
                                        vgChManContinueAction (entity, SIG_APEX_LM_READ_GAS_REQ));
                    }
                    else
                    {
                        setResultCode (entity, RESULT_CODE_ERROR);
                    }
                }
                break;

                case VG_MUPBGAS_MODE_WRITE:
                {
                    if( vgLmData_p->grpInfo.grpData.grpList[0] <= vgLmData_p->grpInfo.gasNumRecords)
                    {
                        if( vgLmData_p->grpInfo.grpAlphaId.length != 0)
                        {
                            /* Write the GAS at the specified index*/
                            setResultCode(  entity,
                                            vgChManContinueAction (entity, SIG_APEX_LM_WRITE_GAS_REQ));
                        }
                        else
                        {
                            /* Delete the specified GAS record*/
                            setResultCode(  entity,
                                            vgChManContinueAction (entity, SIG_APEX_LM_DELETE_GAS_REQ));
                        }
                    }
                    else
                    {
                        setResultCode (entity, RESULT_CODE_ERROR);
                    }
                }
                break;

                case VG_MUPBGAS_MODE_DELETE_ALL:
                {
                    if( vgLmData_p->grpInfo.gasNumRecords > 0)
                    {
                        setResultCode(  entity,
                                        vgChManContinueAction (entity, SIG_APEX_LM_CLEAR_GAS_REQ));
                    }
                    else
                    {
                        setResultCode (entity, RESULT_CODE_ERROR);
                    }
                }
                break;

                default:
                {
                    DevParam( vgLmData_p->vgMupbgasContext.mupbgasMode, 0, 0);
                }
                break;
            }
        }
        else
        {
            setResultCode (entity, RESULT_CODE_ERROR);
        }
    }
}

/*--------------------------------------------------------------------------
*
* Function:        vgProcessPbStatusMupbaas
*
* Parameters:      PhoneBookInfo     *phoneBookInfo_p
*                  VgLmData          *vgLmData_p
*                  const VgmuxChannelNumber entity
*
*
* Returns:         nothing
*
* Description:     proceeds the phonebook status signal when executing a
*                  MUPBAAS command
*-------------------------------------------------------------------------*/
static void vgProcessPbStatusMupbaas(   PhoneBookInfo *phoneBookInfo_p,
                                        VgLmData *vgLmData_p,
                                        const VgmuxChannelNumber entity)
{
    DevCheck(   (   (vgLmData_p->phoneBook == DIAL_LIST_ADN_APP) ||
                    (vgLmData_p->phoneBook == DIAL_LIST_ADN_GLB) ),
                vgLmData_p->phoneBook, 0, 0);

    if( vgLmData_p->vgMupbaasContext.mupbaasMode == VG_MUPBAAS_MODE_RANGE)
    {
        vgPutNewLine (entity);
        if( vgLmData_p->aasInfo.aasNumRecord > 0 )
        {
            vgPrintf(   entity,
                        (Char *)"*MUPBAAS: (1-3),(1-%d),%d",
                        vgLmData_p->aasInfo.aasNumRecord,
                        vgLmData_p->aasInfo.aasSize );
        }
        else
        {
            vgPrintf(   entity,
                        (Char *)"*MUPBAAS: (1-3),0,0" );
        }
        vgPutNewLine (entity);
        setResultCode (entity, RESULT_CODE_OK);
    }
    else
    {
        if( vgLmData_p->aasInfo.aasNumRecord > 0 )
        {
            switch( vgLmData_p->vgMupbaasContext.mupbaasMode)
            {
                case VG_MUPBAAS_MODE_READ:
                {
                    if( vgLmData_p->aasInfo.aasIndex <= vgLmData_p->aasInfo.aasNumRecord)
                    {
                        /* Read the asked GAS record*/
                        setResultCode(  entity,
                                        vgChManContinueAction (entity, SIG_APEX_LM_READ_AAS_REQ));
                    }
                    else
                    {
                        setResultCode (entity, RESULT_CODE_ERROR);
                    }
                }
                break;

                case VG_MUPBAAS_MODE_WRITE:
                {
                    if( vgLmData_p->aasInfo.aasIndex <= vgLmData_p->aasInfo.aasNumRecord)
                    {
                        if( vgLmData_p->aasInfo.aasAlphaId.length != 0)
                        {
                            /* Write the GAS at the specified index*/
                            setResultCode(  entity,
                                            vgChManContinueAction (entity, SIG_APEX_LM_WRITE_AAS_REQ));
                        }
                        else
                        {
                            /* Delete the specified GAS record*/
                            setResultCode(  entity,
                                            vgChManContinueAction (entity, SIG_APEX_LM_DELETE_AAS_REQ));
                        }
                    }
                    else
                    {
                        setResultCode (entity, RESULT_CODE_ERROR);
                    }
                }
                break;

                case VG_MUPBAAS_MODE_DELETE_ALL:
                {
                    setResultCode(  entity,
                                    vgChManContinueAction (entity, SIG_APEX_LM_CLEAR_AAS_REQ));
                }
                break;

                default:
                {
                    DevParam( vgLmData_p->vgMupbaasContext.mupbaasMode, 0, 0);
                }
                break;
            }
        }
        else
        {
            setResultCode (entity, RESULT_CODE_ERROR);
        }
    }
}

/*--------------------------------------------------------------------------
* Function:     vgGnMUPBSYNCPrintPbResults
*
* Parameters:   entity   - current entity
*               sig_p    - Signal with information to print
*
* Returns:      Nothing
*
* Description:  Print the received phonebook synchronisation status
*               information in the AT*MUPBSYNC format
*-------------------------------------------------------------------------*/
static void vgGnMUPBSYNCPrintPbResults( const VgmuxChannelNumber profileEntity,
                                        const ApexLmGetSyncStatusCnf *sig_p)
{
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (profileEntity);
    VgLmData           *vgLmData;
    const VgLmInfo     *phonebook = PNULL;
    Int8                indexPb;
    const Char         *adnGlb = PNULL;
    const Char         *adnApp = PNULL;

    FatalCheck(generalContext_p != PNULL, profileEntity, 0, 0);

    vgLmData = &generalContext_p->vgLmData;

    /* Find the abbreviation for ADN_APP and ADN_GLB*/
    phonebook = getVgLmInfoRec ();
    for(    indexPb=0;
            indexPb<NUMBER_OF_PHONE_BOOKS && (adnGlb==PNULL || adnApp==PNULL);
            indexPb++)
    {
        if( phonebook[indexPb].file == DIAL_LIST_ADN_APP)
        {
            adnApp = phonebook[indexPb].vgPhoneStore;
        }
        else if( phonebook[indexPb].file == DIAL_LIST_ADN_GLB)
        {
            adnGlb = phonebook[indexPb].vgPhoneStore;
        }
    }
    /*The phonebook information must exists*/
    FatalCheck( adnApp!=PNULL && adnGlb!=PNULL, adnApp, adnGlb, 0);

    if( sig_p->cardIsUicc )
    {
        switch( vgLmData->vgMupbsyncContext.operation)
        {
            case EXTENDED_RANGE:  /* AT*MUPBSYNC=? */
                {
                    /* Print the avalaible 3G phonebook with synchronisation data*/
                    vgPutNewLine( profileEntity);
                    vgPrintf( profileEntity, (const Char *)"%C: (1-2),(" );
                    if( sig_p->adnGlbSyncInfo.syncInfoAvalaible)
                    {
                        vgPrintf(   profileEntity,
                                    (const Char *)"\"%s\"%s",
                                    (const char*)adnGlb,
                                    sig_p->adnAppSyncInfo.syncInfoAvalaible?",":"");
                    }
                    if( sig_p->adnAppSyncInfo.syncInfoAvalaible)
                    {
                        vgPrintf(   profileEntity,
                                    (const Char *)"\"%s\"",
                                    (const char*)adnApp);
                    }
                    vgPrintf( profileEntity, (const Char *)"),(1-%d),65534", vgLmData->vgMupbsyncContext.pbCapacity);
                    vgPutNewLine( profileEntity);
                    setResultCode( profileEntity, RESULT_CODE_OK);
                }
                break;

            case EXTENDED_ASSIGN: /* AT*MUPBSYNC=... */
                {
                    /* Valid that selected phonebook supports synchronisation*/
                    if(     (   (vgLmData->phoneBook == DIAL_LIST_ADN_GLB) &&
                                (sig_p->adnGlbSyncInfo.syncInfoAvalaible)
                            ) ||
                            (   (vgLmData->phoneBook == DIAL_LIST_ADN_APP) &&
                                (sig_p->adnAppSyncInfo.syncInfoAvalaible)
                            ))
                    {
                        if( vgLmData->phoneBook == DIAL_LIST_ADN_APP)
                        {
                            vgGnMUPBSYNCPrintPbSyncData( profileEntity, adnApp, &sig_p->adnAppSyncInfo);
                        }
                        else
                        {
                            vgGnMUPBSYNCPrintPbSyncData( profileEntity, adnGlb, &sig_p->adnGlbSyncInfo);
                        }
                        setResultCode( profileEntity, RESULT_CODE_OK);
                    }
                    else
                    {
                        /* Operation not allowed*/
                        setResultCode( profileEntity, VG_CME_OPERATION_NOT_ALLOWED);
                    }
                }
                break;

            case EXTENDED_QUERY:  /* AT*MUPBSYNC? */
                {
                    if( sig_p->adnGlbSyncInfo.syncInfoAvalaible)
                    {
                        vgGnMUPBSYNCPrintPbSyncData( profileEntity, adnGlb, &sig_p->adnGlbSyncInfo);
                    }

                    if( sig_p->adnAppSyncInfo.syncInfoAvalaible)
                    {
                        vgGnMUPBSYNCPrintPbSyncData( profileEntity, adnApp, &sig_p->adnAppSyncInfo);
                    }
                    setResultCode( profileEntity, RESULT_CODE_OK);
                }
                break;

            default:
                {
                    /* Should not be here... */
                    FatalParam(profileEntity, vgLmData->vgMupbsyncContext.operation, 0);
                }
                break;
        }
    }
    else
    {
        /* Operation not allowed*/
        setResultCode( profileEntity, VG_CME_OPERATION_NOT_ALLOWED);
    }
}

/*--------------------------------------------------------------------------
* Function:     vgGnMUPBSYNCPrintPbSyncData
*
* Parameters:   phonebook   - current phonebook to print
*               syncInfo    - Phonebook's synchonisation information
*
* Returns:      Nothing
*
* Description:  Print the phonebook's synchronisation status information
*-------------------------------------------------------------------------*/
static void vgGnMUPBSYNCPrintPbSyncData(    const VgmuxChannelNumber profileEntity,
                                            const Char *phonebook,
                                            const Ablm3gSyncInfo *syncInfo)
{
    Char    textPbid[SIM_UICC_PBID_LENGTH*2+1] = {0};       /* PBID converted as text */
    Int8    i;

    /*Convert PBID in text*/
    for( i=0; i<SIM_UICC_PBID_LENGTH; i++)
    {
        snprintf( (char*)(textPbid+i*2),
                  (SIM_UICC_PBID_LENGTH*2+1) - (i*2),
                  "%02X",
                  syncInfo->phoneBookId.value[i]);
    }
    textPbid[2*SIM_UICC_PBID_LENGTH] = 0;

    vgPutNewLine( profileEntity);
    vgPrintf(   profileEntity,
                (const Char *)"%C: \"%s\",\"%s\",%d,%d",
                (const char*)phonebook,
                textPbid + (VG_MUPBSYNC_ICCID_LENGTH * 2), /* Only print the PSC, i.e the 4 last PBID bytes*/
                syncInfo->changeCounter,
                syncInfo->previousUid);
    vgPutNewLine( profileEntity);
}

/*--------------------------------------------------------------------------
* Function:     vgGnMUPBSYNCPrintUid
*
* Parameters:   entity   - current entity
*               sig_p    - Signal with information to print
*
* Returns:      Nothing
*
* Description:  Print the received record UID
*-------------------------------------------------------------------------*/
static void vgGnMUPBSYNCPrintUid(   const VgmuxChannelNumber profileEntity,
                                    const ApexLmReadRecordUidCnf *sig_p)
{
    const VgLmInfo     *phonebook = PNULL;
    Int8                indexPb;
    const Char         *adnGlb = PNULL;
    const Char         *adnApp = PNULL;

    /* The response file must be ADN_GLB or ADN_APP*/
    FatalCheck( sig_p->file==DIAL_LIST_ADN_APP || sig_p->file==DIAL_LIST_ADN_GLB, sig_p->file, 0, 0);

    /* Find the abbreviation for ADN_APP and ADN_GLB*/
    phonebook = getVgLmInfoRec ();
    for(    indexPb=0;
            indexPb<NUMBER_OF_PHONE_BOOKS && (adnGlb==PNULL || adnApp==PNULL);
            indexPb++)
    {
        if( phonebook[indexPb].file == DIAL_LIST_ADN_APP)
        {
            adnApp = phonebook[indexPb].vgPhoneStore;
        }
        else if( phonebook[indexPb].file == DIAL_LIST_ADN_GLB)
        {
            adnGlb = phonebook[indexPb].vgPhoneStore;
        }
    }
    /*The phonebook information must exists*/
    FatalCheck( adnApp!=PNULL && adnGlb!=PNULL, adnApp, adnGlb, 0);

    vgPutNewLine( profileEntity);
    vgPrintf(   profileEntity,
                (const Char *)"%C: \"%s\",%d,%d",
                sig_p->file==DIAL_LIST_ADN_APP ? (const char*)adnApp : (const char*)adnGlb,
                sig_p->recordNumber,
                sig_p->uniqueId);
    vgPutNewLine( profileEntity);
}

/*--------------------------------------------------------------------------
*
* Function:        vgContinueCallStatusRequest
*
* Parameters:      VgmuxChannelNumber - entity which sent request
*
* Returns:         nothing
*
* Description:
*-------------------------------------------------------------------------*/

static void vgContinueCallStatusRequest ( const LmDialNumber *dialInfo,
                                          const VgmuxChannelNumber entity)
{
  /* job133356: handle SS dialled string stored in memory by creating new ATD */
  /* command and passing it through the SS parser */

  CallContext_t *callContext_p = ptrToCallContext (entity);
  Char          *writePoint;
#if defined (ENABLE_LONG_AT_CMD_RSP)
  Char          outputLine[AT_MEDIUM_BUFF_SIZE] = {0};
#else
  Char          outputLine[CIMUX_MAX_AT_DATA_LENGTH] = {0};
#endif
  FatalCheck(callContext_p != PNULL, entity, 0, 0);

  writePoint = vgPrintLine (outputLine, (const Char*)"D");

  /* add the dialled digits read from memory */
  writePoint = vgConvBcdToText (dialInfo->typeOfNumber,
                                dialInfo->dialString,
                                dialInfo->dialStringLength,
                                writePoint);

  /* set entity state to IDLE because we are effectively starting a new command */
  setEntityState (entity, ENTITY_IDLE);

  /* initiate sending of SIG_CI_RUN_AT_COMMAND_IND */
  vgCiRunAtCommandInd (entity, outputLine);
}

/*--------------------------------------------------------------------------
*
* Function:        getPhoneBookInfo
*
* Parameters:      ApexLmPhoneBookStatusCnf - signal containing phonebook
*                                             information
*                  VgmuxChannelNumber - entity which sent request
*
* Returns:         PhoneBookInfo
*
* Description:     Checks if selected phonebook is available and returns
*                  the number of records allowed, the number currently used
*                  and the maximum length of the alpha id, plus the group length,
*                  second name length, and email length.
*
*
*-------------------------------------------------------------------------*/

static PhoneBookInfo getPhoneBookInfo (const ApexLmPhoneBookStatusCnf *signal,
                                        const LmDialNumberFile phoneBook)
{
  PhoneBookInfo info = { FALSE, 0, 0, 0, 0, FALSE, DIAL_LIST_NULL, 0, 0, 0 };

  FatalCheck(((phoneBook == DIAL_LIST_ADN_GLB)||(phoneBook == DIAL_LIST_ADN_APP)), phoneBook, 0, 0);

  info.phoneBook = phoneBook;
  if ((phoneBook == DIAL_LIST_ADN_GLB)||
      (phoneBook == DIAL_LIST_ADN_APP))
  {
      if (signal->adnNumRecords != 0)
      {
        info.available = TRUE;
        info.enabled   = TRUE;
        info.records   = signal->adnNumRecords;
        info.used      = signal->adnRecordsUsed;
        info.alpha     = signal->adnAlphaIdSize;
        info.dial      = (signal->ext1Available) ?
                         MAX_CALLED_BCD_NO_LENGTH :
                         SIM_NO_EXT1_MAX_DIAL_NUMBER_LENGTH;
        info.sLength   = (signal->sneAvailable) ?
                          signal->sneAlphaIdSize : 0;

        info.gLength   = signal->gasSize;
        info.eLength   = signal->emailAddressSize;

      }
  }

  return (info);
}

/*--------------------------------------------------------------------------
*
* Function:        getDialNumInfoByIndex
*
* Parameters:      ApexLmDialNumStatusCnf - signal containing phonebook
*                                           information
*                  index - index in the VgLmInfo phonebook list
*
* Returns:         PhoneBookInfo
*
* Description:     Checks if selected phonebook is available and returns
*                  the number of records allowed, the number currently used
*                  and the maximum length of the alpha id.
*
*-------------------------------------------------------------------------*/
static PhoneBookInfo getDialNumInfoByIndex( VgmuxChannelNumber entity,
                                            const ApexLmDialNumStatusCnf *signal,
                                            const Int8 index)
{
    const VgLmInfo *lmInfo = getVgLmInfoRec ();
    PhoneBookInfo   phoneBookInfo;

    switch( lmInfo[index].file)
    {
        case DIAL_LIST_LND:
            {
                if( signal->ociNumRecords != 0)
                {
                    phoneBookInfo = getDialNumInfo( entity, signal, DIAL_LIST_OCI);
                }
                else
                {
                    phoneBookInfo = getDialNumInfo( entity, signal, DIAL_LIST_LND);
                }
            }
            break;

        case DIAL_LIST_LNM:
        case DIAL_LIST_LNR:
            {
                if( signal->iciNumRecords != 0)
                {
                    phoneBookInfo = getDialNumInfo( entity, signal, DIAL_LIST_ICI);
                }
                else
                {
                    phoneBookInfo = getDialNumInfo( entity, signal, DIAL_LIST_LNR);
                }
            }
            break;

        default:
            {
                phoneBookInfo = getDialNumInfo( entity, signal, lmInfo[index].file);
            }
            break;
    }
    return phoneBookInfo;
}

/*--------------------------------------------------------------------------
*
* Function:        getDialNumInfo
*
* Parameters:      ApexLmDialNumStatusCnf - signal containing phonebook
*                                           information
*                  VgmuxChannelNumber - entity which sent request
*
* Returns:         PhoneBookInfo
*
* Description:     Checks if selected phonebook is available and returns
*                  the number of records allowed, the number currently used
*                  and the maximum length of the alpha id.
*
*
*-------------------------------------------------------------------------*/

static PhoneBookInfo getDialNumInfo (   VgmuxChannelNumber entity,
                                        const ApexLmDialNumStatusCnf *signal,
                                        const LmDialNumberFile phoneBook)
{
    GeneralGenericContext_t    *generalGenericContext_p = ptrToGeneralGenericContext();

    PhoneBookInfo               info                    = { FALSE, 0, 0, 0, 0, FALSE, DIAL_LIST_NULL, 0, 0, 0 };


    info.phoneBook = phoneBook;
    switch (phoneBook)
    {
        case DIAL_LIST_LNM: /* "MC" - ME missed (unanswered) calls list */
        {
            if (signal->lnmNumRecords != 0)
            {
                info.available = TRUE;
                info.enabled   = TRUE;
                info.records   = signal->lnmNumRecords;
                info.alpha     = signal->lnmAlphaIdSize;
                info.used = signal->lnmRecordsUsed;
                info.dial = SIM_NO_EXT1_MAX_DIAL_NUMBER_LENGTH;

            }
        }
        break;

        case DIAL_LIST_LNR: /* "RC" - ME received calls list */
        {
            if (signal->lnrNumRecords != 0)
            {
                info.available = TRUE;
                info.enabled   = TRUE;
                info.records   = signal->lnrNumRecords;
                info.alpha     = signal->lnrAlphaIdSize;

                info.used = signal->lnrRecordsUsed;

                info.dial = SIM_NO_EXT1_MAX_DIAL_NUMBER_LENGTH;

            }
        }
        break;

        case DIAL_LIST_LND: /* "DC" & "LD" - ME/SIM dialled calls list  */
        {
            if (signal->lndNumRecords != 0)
            {
                info.available = TRUE;
                info.enabled   = TRUE;
                info.records   = signal->lndNumRecords;
                info.alpha     = signal->lndAlphaIdSize;
                /* a separate extension file is used in 3G case */
                info.used = signal->lndRecordsUsed;
                info.dial = (signal->lndExtAvailable) ?
                                MAX_CALLED_BCD_NO_LENGTH :
                                SIM_NO_EXT1_MAX_DIAL_NUMBER_LENGTH;
            }
        }
        break;

        case DIAL_LIST_ICI: /* "MC" & "RC" - Missed and Received calls list  */
        {
            if (signal->iciNumRecords != 0)
            {
                info.available = TRUE;
                info.enabled   = TRUE;
                info.records   = signal->iciNumRecords;
                info.alpha     = signal->iciAlphaIdSize;
                info.used      = signal->iciRecordsUsed;
                info.dial      = (signal->iciExtAvailable) ?
                                    MAX_CALLED_BCD_NO_LENGTH :
                                    SIM_NO_EXT1_MAX_DIAL_NUMBER_LENGTH;
            }
        }
        break;
        case DIAL_LIST_OCI: /* "DC" & "LD" - ME/SIM dialled calls list  */
        {
            if (signal->ociNumRecords != 0)
            {
                info.available = TRUE;
                info.enabled   = TRUE;
                info.records   = signal->ociNumRecords;
                info.alpha     = signal->ociAlphaIdSize;
                info.used      = signal->ociRecordsUsed;
                info.dial      = (signal->ociExtAvailable == TRUE) ?
                                    MAX_CALLED_BCD_NO_LENGTH :
                                    SIM_NO_EXT1_MAX_DIAL_NUMBER_LENGTH;
            }
        }
        break;

        case DIAL_LIST_ADR: /* "ME" - ME phone list */
        {
            if (signal->nvrAdnNumRecords != 0)
            {
                info.available = TRUE;
                info.enabled   = TRUE;
                info.records   = signal->nvrAdnNumRecords;
                info.used      = signal->nvrAdnRecordsUsed;
                info.alpha     = signal->nvrAdnAlphaIdSize;
                info.dial      = MAX_CALLED_BCD_NO_LENGTH;
            }
        }
        break;

        case DIAL_LIST_ADN_GLB: /* SIM global phonebook */
        {
            if (signal->glbAdnNumRecords != 0)
            {
                info.available = TRUE;
                info.enabled   = TRUE;
                info.records   = signal->glbAdnNumRecords;
                info.used      = signal->glbAdnRecordsUsed;
                info.alpha     = signal->glbAdnAlphaIdSize;
                info.dial      = (signal->glbAdnExtAvailable == TRUE) ?
                                    MAX_CALLED_BCD_NO_LENGTH :
                                    SIM_NO_EXT1_MAX_DIAL_NUMBER_LENGTH;
            }
        }
        break;

        case DIAL_LIST_ADN_APP: /* SIM application phonebook */
        {
            if (signal->appAdnNumRecords != 0)
            {
                info.available = TRUE;
                info.enabled   = TRUE;
                info.records   = signal->appAdnNumRecords;
                info.used      = signal->appAdnRecordsUsed;
                info.alpha     = signal->appAdnAlphaIdSize;
                info.dial      = (signal->appAdnExtAvailable == TRUE) ?
                                    MAX_CALLED_BCD_NO_LENGTH :
                                    SIM_NO_EXT1_MAX_DIAL_NUMBER_LENGTH;
            }
        }
        break;

        case DIAL_LIST_FDN: /* "FD" - SIM fixdialling phonebook */
        {
            if (signal->fdnNumRecords != 0)
            {
                info.available = TRUE;
                info.enabled   = signal->fdnEnabled;
                info.records   = signal->fdnNumRecords;
                info.used      = signal->fdnRecordsUsed;
                info.alpha     = signal->fdnAlphaIdSize;
                /* a separate extension file is used in 3G case */
                info.dial      = (signal->fdnExtAvailable == TRUE) ?
                                    SIM_MAX_DIAL_NUMBER_LENGTH :
                                    SIM_NO_EXT1_MAX_DIAL_NUMBER_LENGTH;
            }
        }
        break;

        case DIAL_LIST_MSISDN: /* "ON" - SIM own numbers (MSISDNs) list */
        {
            if (signal->msisdnNumRecords != 0)
            {
                info.available = TRUE;
                info.enabled   = TRUE;
                info.records   = signal->msisdnNumRecords;
                info.used      = signal->msisdnRecordsUsed;
                info.alpha     = signal->msisdnAlphaIdSize;
                /* a separate extension file is used in 3G case */
                info.dial      = (signal->msisdnExtAvailable == TRUE) ?
                                    MAX_CALLED_BCD_NO_LENGTH :
                                    SIM_NO_EXT1_MAX_DIAL_NUMBER_LENGTH;
            }
        }
        break;

        case DIAL_LIST_SDN: /* "SD" - SIM service dial number */
        {
            if (signal->sdnNumRecords != 0)
            {
                info.available = TRUE;
                info.enabled   = TRUE;
                info.records   = signal->sdnNumRecords;
                info.used      = signal->sdnRecordsUsed;
                info.alpha     = signal->sdnAlphaIdSize;
                 /* a separate extension file is used in 3G case */
                info.dial      = (signal->sdnExtAvailable == TRUE) ?
                                    MAX_CALLED_BCD_NO_LENGTH :
                                    SIM_NO_EXT1_MAX_DIAL_NUMBER_LENGTH;
            }
        }
        break;

        case DIAL_LIST_BDN: /* "BN" - SIM barred dial numbers */
        {
            if (signal->bdnNumRecords != 0)
            {
                info.available = TRUE;
                info.enabled   = signal->bdnEnabled;
                info.records   = signal->bdnNumRecords;
                info.used      = signal->bdnRecordsUsed;
                info.alpha     = signal->bdnAlphaIdSize;
                /* a separate extension file is used in 3G case */
                info.dial      = (signal->bdnExtAvailable == TRUE) ?
                                    MAX_CALLED_BCD_NO_LENGTH :
                                    SIM_NO_EXT1_MAX_DIAL_NUMBER_LENGTH;
            }
        }
        break;

        case DIAL_LIST_CPHS_MN: /* "VM" - SIM voice mailbox number */
        {
            if (signal->cphsMnNumRecords != 0)
            {
                info.available = TRUE;
                info.enabled   = TRUE;
                info.records   = signal->cphsMnNumRecords;
                info.used      = signal->cphsMnRecordsUsed;
                info.alpha     = signal->cphsMnAlphaIdSize;
                /* a separate extension file is used in 3G case */
                info.dial      = (signal->cphsMnExtAvailable == TRUE) ?
                                    MAX_CALLED_BCD_NO_LENGTH :
                                    SIM_NO_EXT1_MAX_DIAL_NUMBER_LENGTH;
            }
        }
        break;

        case DIAL_LIST_MBDN: /* "VM" - SIM voice mailbox number */
        {
          if (signal->mbdnNumRecords != 0)
          {
            info.available = TRUE;
            info.enabled   = TRUE;
            info.records   = signal->mbdnNumRecords;
            info.used      = signal->mbdnRecordsUsed;
            info.alpha     = signal->mbdnAlphaIdSize;
            info.dial      = (signal->mbdnExtAvailable == TRUE) ?
                              MAX_CALLED_BCD_NO_LENGTH :
                              SIM_NO_EXT1_MAX_DIAL_NUMBER_LENGTH;
          }
        }
        break;
        default:
        {
            /* Unexpected phone book type */
            FatalParam(phoneBook, 0, 0);
        }
        break;
    }

    return (info);
}



/*--------------------------------------------------------------------------
*
* Function:        vgListDialNum
*
* Parameters:      VgmuxChannelNumber - entity which sent request
*
* Returns:         Nothing
*
* Description:     Continues listing dial numbers if necessary
*-------------------------------------------------------------------------*/

static void vgListDialNum (const VgmuxChannelNumber entity)
{
  GeneralContext_t *generalContext_p = ptrToGeneralContext (entity);
  VgLmData         *vgLmData;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData = &generalContext_p->vgLmData;

  switch (vgLmData->readMode)
  {
    case LM_READ_ABSOLUTE: /* not listing a range of records */
    {
      setResultCode (entity, RESULT_CODE_OK);
      break;
    }
    case LM_READ_FIRST:    /* just read the first record, now read the next */
    {
      vgLmData->readMode = LM_READ_NEXT;
      setResultCode (entity,
       vgChManContinueActionFlowControl (entity, SIG_APEX_LM_READ_DIALNUM_REQ));
      break;
    }
    case LM_READ_LAST:     /* just read the last record, now read the previous */
    {
      vgLmData->readMode = LM_READ_PREVIOUS;
      setResultCode (entity,
       vgChManContinueActionFlowControl (entity, SIG_APEX_LM_READ_DIALNUM_REQ));
      break;
    }
    case LM_READ_NEXT:     /* just read the next entry along */
    case LM_READ_PREVIOUS:
    {
      setResultCode (entity,
       vgChManContinueActionFlowControl (entity, SIG_APEX_LM_READ_DIALNUM_REQ));
      break;
    }
    default:
    {
      /* Unexpected read mode */
      FatalParam(entity, vgLmData->readMode, 0);
      setResultCode (entity, RESULT_CODE_ERROR);
      break;
    }
  }
}

/*--------------------------------------------------------------------------
* Function:    getAllEntriesInPhoneBook
*
* Parameters:  VgmuxChannelNumber  - current entity
*              PhoneBookInfo      - information on current phone book
*
* Returns:     Nothing
*
* Description: sends a request to read the first record in the current phone
*              book if there are at least one records used.
*-------------------------------------------------------------------------*/

static void getAllEntriesInPhoneBook (const VgmuxChannelNumber entity,
                                       const PhoneBookInfo *phoneBookInfo)
{
  GeneralContext_t *generalContext_p = ptrToGeneralContext (entity);

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  /* check if the phone book is available and may contain records */

  if ((phoneBookInfo->available == TRUE) &&
      (phoneBookInfo->records > 0))
  {
    generalContext_p->vgLmData.readMode    = LM_READ_FIRST;
    generalContext_p->vgLmData.phoneIndex1 = 1;
    generalContext_p->vgLmData.phoneIndex2 = phoneBookInfo->records;
    generalContext_p->vgLmData.currentLnaRecord = 0;

    setResultCode (entity,
     vgChManContinueAction (entity, SIG_APEX_LM_READ_DIALNUM_REQ));
  }
  else /* nothing to display */
  {
    setResultCode (entity, RESULT_CODE_OK);
  }
}

/*--------------------------------------------------------------------------
* Function:    writeEntryToPhoneBook
*
* Parameters:  VgmuxChannelNumber  - current entity
*              PhoneBookInfo      - information on current phone book
*
* Returns:     Nothing
*
* Description: sends a request to read the first record in the current phone
*              book if there are at least one records used.
*-------------------------------------------------------------------------*/

static void writeEntryToPhoneBook (const VgmuxChannelNumber entity,
                                    const PhoneBookInfo *phoneBookInfo)
{
    GeneralContext_t *generalContext_p = ptrToGeneralContext (entity);
    VgLmData         *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    /* check request record is within phone book record range */
    if( (phoneBookInfo->records > 0) &&
        (phoneBookInfo->records >= vgLmData_p->phoneIndex1))
    {
        /* check dial string is not too long */
        if (phoneBookInfo->dial >= vgLmData_p->writeNumLength)
        {
            /* check alpha id is not too long */
            if (phoneBookInfo->alpha >= vgLmData_p->alphaLength)
            {
                /* if given index is zero then write in first free index */
                if (vgLmData_p->phoneIndex1 == 0)
                {
                    /* writing an entry in the first available location */
                    vgLmData_p->writeMode = LM_WRITE_FIRST_FREE;
                    vgLmData_p->forReplace = FALSE;
                    setResultCode(  entity,
                                    vgChManContinueAction (entity, SIG_APEX_LM_WRITE_DIALNUM_REQ));
                }
                else
                {
                    if( (vgLmData_p->phoneBook == DIAL_LIST_ADN_APP) ||
                        (vgLmData_p->phoneBook == DIAL_LIST_ADN_GLB))
                    {
                        /* Start by deleting the record at the specified location*/
                        vgLmData_p->writeMode = LM_WRITE_ABSOLUTE;
                        vgLmData_p->forReplace = TRUE; /* We will determine this point with the
                                                        * delete operation result, for the moment
                                                        * assume it is a replace operation*/
                        vgLmData_p->deleteRes = RESULT_CODE_OK;
                        setResultCode (entity,
                            vgChManContinueAction (entity, SIG_APEX_LM_DELETE_DIALNUM_REQ));
                    }
                    else
                    {
                        /* writing an entry in a specified location */
                        vgLmData_p->writeMode = LM_WRITE_ABSOLUTE;
                        vgLmData_p->forReplace = FALSE; /* Will be automatically determined by AB*/
                        setResultCode(  entity,
                                        vgChManContinueAction (entity, SIG_APEX_LM_WRITE_DIALNUM_REQ));
                    }
                }
            }
            else
            {
                setResultCode (entity, VG_CME_LONG_TEXT);
            }
        }
        else
        {
            setResultCode (entity, VG_CME_LONG_DIALSTRING);
        }
    }
    else
    {
        setResultCode (entity, RESULT_CODE_ERROR);
    }
}


/*--------------------------------------------------------------------------
 *
 * Function:    displayPhoneBookAlphaId
 *
 * Parameters:  signal - alpha id lookup response
 *              entity - channel number
 *
 * Returns:     nothing
 *
 * Description: displays AlphaId and continues/stops display of
 *              current phonebook
 *
 *-------------------------------------------------------------------------*/

static void displayPhoneBookAlphaId (const VgmuxChannelNumber entity,
                                      const ApexLmGetAlphaCnf *signal)
{
  GeneralContext_t *generalContext_p = ptrToGeneralContext (entity);
  VgLmData         *vgLmData;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData         = &generalContext_p->vgLmData;

  FatalAssert (signal->callId == VG_PHONE_BOOK_CALL_ID);

  vgPutc (entity, '\"');

  if ((signal->requestStatus == LM_REQ_OK) &&
      (signal->alphaStringValid == TRUE))
  {
    vgPutAlphaId (entity,
                   signal->alphaId.data,
                    signal->alphaId.length);
  }

  vgPutc (entity, '\"');

  vgPutNewLine (entity);

  if (vgLmData->moreRecordsToRead == TRUE)
  {
    vgListDialNum (entity);
  }
  else
  {
    setResultCode (entity, RESULT_CODE_OK);
  }
}

#endif /* FEA_PHONEBOOK */

/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:        vgBcdNumberTypeToChar
 *
 * Parameters:      typeOfNumber - NUM_INTERNATIONAL or other
 *
 * Returns:         Character for the number type
 *
 * Description:     Return dial number type depending on number type.
 *                  eg. 145 (international), 129 (unknown)
 *
 *-------------------------------------------------------------------------*/

VgDialNumberType vgBcdNumberTypeToChar (const BcdNumberType typeOfNumber)
{
  VgDialNumberType type = VG_DIAL_NUMBER_UNKNOWN;

  switch (typeOfNumber)
  {
    case NUM_TYPE_INTERNATIONAL:
    {
      type = VG_DIAL_NUMBER_INTERNATIONAL;
      break;
    }
    case NUM_TYPE_UNKNOWN:    /* To fix BL bug! */
    {
      type = VG_DIAL_NUMBER_UNKNOWN;
      break;
    }
    case NUM_TYPE_NATIONAL:
    {
      type = VG_DIAL_NUMBER_NATIONAL;
      break;
    }
    case NUM_TYPE_NETWORK_SPEC:
    {
      type = VG_DIAL_NUMBER_NET_SPECIFIC;
      break;
    }
    case NUM_TYPE_DEDIC_PAD:
    default:
    {
      break;
    }
  }

  return (type);
}

#if defined (COARSE_TIMER)
/*--------------------------------------------------------------------------
 *
 * Function:    vgSigConModemStatusInd
 *
 * Parameters:  ModemCause       - cause value:
 *
 *
 * Returns:     Nothing
 *
 * Description: send SIG_CONN_MODEM_STATUS_IND to connection layer
 *
 *
 *-------------------------------------------------------------------------*/
void vgSigConModemStatusInd(ModemCause cause)
{
  SignalBuffer            sigBuff           = kiNullBuffer;
  ModemStatusInd          *request_p;

  KiCreateZeroSignal( SIG_CONN_MODEM_STATUS_IND,
                    sizeof (ModemStatusInd),
                    &sigBuff);

  request_p           = (ModemStatusInd *) sigBuff.sig;
  request_p->cause    = cause;

  KiSendSignal (MUXCONN_SHM_TASK_ID, &sigBuff);
}
#endif

#if defined (FEA_PHONEBOOK)
/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmDialNumStatusCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_DIALNUM_STATUS_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Extracts the information about the current phone book to
 *              get range information. Also, read, search and write requests
 *              are sent off depending on the current command.
 *-------------------------------------------------------------------------*/

void vgSigApexLmDialNumStatusCnf (const SignalBuffer *signalBuffer,
                                   const VgmuxChannelNumber entity)
{
    ApexLmDialNumStatusCnf     *sig_p                   = &signalBuffer->sig->apexLmDialNumStatusCnf;
    Int8                        index                   = 0;
    GeneralContext_t           *generalContext_p        = ptrToGeneralContext (entity);
    VgLmData                   *vgLmData;
    const VgLmInfo             *lmInfo                  = getVgLmInfoRec ();
    PhoneBookInfo               phoneBookInfo;
    SimLockGenericContext_t    *simLockGenericContext_p = ptrToSimLockGenericContext ();
    ResultCode_t                result                  = RESULT_CODE_OK;
    VgSimInfo                  *simInfo                 = &(simLockGenericContext_p->simInfo);
    SimLockContext_t           *simLockContext_p        = ptrToSimLockContext(entity);

    FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData = &generalContext_p->vgLmData;

    /* indicates first record of list */
    vgLmData->firstDialNumRecordFromList = TRUE;

    phoneBookInfo = getDialNumInfo (entity, sig_p, vgLmData->phoneBook);

    vgLmData->grpInfo.groupSupported = FALSE;
    vgLmData->sneSupported = FALSE;
    vgLmData->adNumInfo.anrSupported = FALSE;
    vgLmData->emailInfo.emailSupported = FALSE;

    if (sig_p->requestStatus == LM_REQ_OK)
    {
        switch (getCommandId (entity))
        {
            case VG_AT_GN_MUPBSYNC:
            {
                /* Store the selected phonebook capacity*/
                switch( vgLmData->phoneBook)
                {
                    case DIAL_LIST_ADN_GLB:
                    {
                        vgLmData->vgMupbsyncContext.pbCapacity = sig_p->glbAdnNumRecords;
                    }
                    break;

                    case DIAL_LIST_ADN_APP:
                    {
                        vgLmData->vgMupbsyncContext.pbCapacity = sig_p->appAdnNumRecords;
                    }
                    break;

                    default:
                    {
                        /* Oups, there is a bug here : invalid sync data request! */
                        FatalParam(entity, vgLmData->phoneBook, 0);
                    }
                    break;
                }
                /* Need reading the synchronisation status to know for which phonebook
                *  3G synchronisation is avalaible*/
                vgChManContinueAction (entity, SIG_APEX_LM_GET_SYNC_STATUS_REQ);
            }
            break;

#if 0
            case VG_AT_GN_CNUM:  /* AT+CNUM */
            {
                /* view all entries in the phonebook */
                getAllEntriesInPhoneBook (entity, &phoneBookInfo);
            }
            break;
#endif
            case VG_AT_GN_CPBR:  /* AT+CPBR */
            {
                vgProcessPbStatusCpbr (&phoneBookInfo, vgLmData, entity);
            }
            break;

            case VG_AT_GN_CPBW:  /* AT+CPBW */
            {
                vgProcessPbStatusCpbw (&phoneBookInfo, vgLmData, entity);
            }
            break;

            case VG_AT_GN_CPBF:  /* AT+CPBF */
            {
                vgProcessPbStatusCpbf (&phoneBookInfo, vgLmData, entity);
            }
            break;

            case VG_AT_GN_CPBS:  /* AT+CPBS */
            {
                switch( vgLmData->vgCpbsData.operation)
                {
                    case EXTENDED_QUERY:
                    {
                        vgPutNewLine (entity);
                        if (vgLmData->phoneBook == DIAL_LIST_ICI)
                        {
                            /*don' t print the number of records as both received calls and missed calls
                            * are in the same file, so could be confusing for the user*/
                            vgPrintf(   entity,
                                        (Char *)"+CPBS: \"%s\" ",
                                        lmInfo[vgLmData->phoneBookIndex].vgPhoneStore);
                        }
                        else
                        {
                            vgPrintf(   entity,
                                        (Char *)"+CPBS: \"%s\",%d,%d",
                                        lmInfo[vgLmData->phoneBookIndex].vgPhoneStore,
                                        phoneBookInfo.used,
                                        phoneBookInfo.records);
                        }
                        vgPutNewLine( entity);
                        setResultCode( entity, RESULT_CODE_OK);
                    }
                    break;

                    case EXTENDED_RANGE:
                    {
                        /* display all available phone book storage options */
                        vgPutNewLine( entity);
                        vgPrintf( entity, (const Char*)"+CPBS: (");
                        for (index = 0; index < NUMBER_OF_PHONE_BOOKS; index++)
                        {
                            phoneBookInfo = getDialNumInfoByIndex( entity, sig_p, index);
                            /* Only print avalaible phonebooks*/
                            if( phoneBookInfo.available && phoneBookInfo.records>0)
                            {
                                vgPrintf(   entity,
                                            (const Char*)"%s\"%s\"",
                                            index == 0 ? "" : ",",
                                            lmInfo[index].vgPhoneStore);
                            }
                        }
                        vgPrintf( entity, (const Char *)")");
                        vgPutNewLine( entity);
                        setResultCode( entity, RESULT_CODE_OK);
                    }
                    break;

                    case EXTENDED_ASSIGN:
                    {
                        /* Verify the status for the selected phonebook is OK*/
                        phoneBookInfo = getDialNumInfo( entity, sig_p, vgLmData->vgCpbsData.phoneBook);
                        if( phoneBookInfo.available && phoneBookInfo.records>0)
                        {
                            /* Update the global context fields*/
                            vgLmData->phoneBookIndex = vgLmData->vgCpbsData.phoneBookIndex;
                            vgLmData->phoneBook = vgLmData->vgCpbsData.phoneBook;
                            vgLmData->iciType   = vgLmData->vgCpbsData.iciType;
                            memcpy( generalContext_p->password,
                                    vgLmData->vgCpbsData.password,
                                    sizeof(generalContext_p->password));
                            generalContext_p->passwordLength = vgLmData->vgCpbsData.passwordLength;

                            /* this part is used for PIN2 code locked storage
                            * when password should be verified */
                            if( (isPhoneBookPin2Protected (vgLmData->phoneBookIndex)) &&
                                (simLockGenericContext_p->simInfo.pin2Verified == FALSE))
                            {
                                if( vgLmData->vgCpbsData.passwordPresent == TRUE)
                                {
                                    if( generalContext_p->passwordLength > 0)
                                    {
                                        memset( &generalContext_p->password[generalContext_p->passwordLength],
                                                UCHAR_MAX,
                                                SIM_CHV_LENGTH - generalContext_p->passwordLength);

                                        memset( &generalContext_p->newPassword[0],
                                                UCHAR_MAX,
                                                SIM_CHV_LENGTH);

                                        generalContext_p->pinFunction = SIM_PIN_FUNCT_VERIFY;
                                        result = sendVerifyPin2Request( entity);
                                    }
                                    else
                                    {
                                        /* Password required but not on input line */
                                        result = VG_CME_INCORRECT_PASSWORD;
                                    }
                                }
                                else
                                {
                                    /* Password required but not on input line */
                                    result = VG_CME_SIM_PIN2_REQUIRED;
                                }
                            }
                        }
                        else
                        {
                            result = RESULT_CODE_ERROR;
                        }
                        setResultCode (entity, result);
                    }
                    break;

                    default:
                    {
                        /* Invalid CPBS operation */
                        FatalParam(entity, vgLmData->vgCpbsData.operation, 0);
                        setResultCode (entity, RESULT_CODE_ERROR);
                    }
                    break;
                }
                vgLmData->vgCpbsData.operation = INVALID_EXTENDED_OPERATION;
            }
            break;

            default:
            {
                /* Unexpected command getting dialNum status */
                FatalParam(entity, getCommandId (entity), 0);
                setResultCode (entity, RESULT_CODE_ERROR);
            }
            break;
        }
    }
    else
    {
        /* return error code of why request failed */
        setResultCode (entity, vgGetLmCmeErrorCode (sig_p->requestStatus));
    }
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmFindDialNumCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_FIND_DIALNUM_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: if successfully found record for a phone book dial get the
 *              latest call information ready to make a call
 *-------------------------------------------------------------------------*/

void vgSigApexLmFindDialNumCnf (const SignalBuffer *signalBuffer,
                                 const VgmuxChannelNumber entity)
{
  ApexLmFindDialNumCnf *sig_p = &signalBuffer->sig->apexLmFindDialNumCnf;

  if (sig_p->requestStatus == LM_REQ_OK)
  {
    vgContinueCallStatusRequest (&sig_p->dialNumber, entity);
  }
  else
  {
    setResultCode (entity, vgGetLmCmeErrorCode (sig_p->requestStatus));
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmReadDialNumCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_READ_DIALNUM_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: If the reposnse to the phonebook read request was successful
 *              then the record is displayed or compared with a search string
 *              depending on the current command.
 *-------------------------------------------------------------------------*/

void vgSigApexLmReadDialNumCnf( const SignalBuffer *signalBuffer,
                                const VgmuxChannelNumber entity)
{
    ApexLmReadDialNumCnf     *sig_p = &signalBuffer->sig->apexLmReadDialNumCnf;
    GeneralContext_t         *generalContext_p = ptrToGeneralContext (entity);
    SimLockContext_t         *simLockContext_p = ptrToSimLockContext(entity);
    VgLmData                 *vgLmData;

    Int8                     *data_p = PNULL;
    Boolean                  alphaFound = FALSE;
    SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext();
    VgSimInfo                *simInfo = &(simLockGenericContext_p->simInfo);
    const Int8               *alphaText = sig_p->dialNumber.alphaId.data;
    Int16                    lengthTest = sig_p->dialNumber.alphaId.length;
    const LmDialNumber       *dialNumber_p = &sig_p->dialNumber;
    Int16                    lengthFind;
    Char                     *conversionBuffer_p = PNULL;
    Boolean                  pendingAlphaLookUp = FALSE;
    Boolean                  pendingGroupLookUp = FALSE;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);
    FatalCheck(simLockContext_p != PNULL, entity, 0, 0);

    KiAllocZeroMemory(  sizeof(Char)*(MAX_CALLED_BCD_NO_LENGTH + NULL_TERMINATOR_LENGTH),
                        (void **) &conversionBuffer_p);

    vgLmData   = &generalContext_p->vgLmData;
    lengthFind = vgLmData->alphaLength;

    switch (sig_p->requestStatus)
    {
        case LM_REQ_OK_DATA_INVALID:
        case LM_REQ_RECORD_NOT_FOUND:
        {
            if ((vgLmData->phoneBook == DIAL_LIST_ICI) && (vgLmData->phoneIndex2 == 0))
            {
                /*we were asked for a unique record, and it doesn' t exist...*/
                /*for missed calls and received calls, we cannot rely on the total number of
                * ICI records in order to know whether the index is out of range because
                * missed calls and received calls are stored in the same ICI file*/
                setResultCode (entity, VG_CME_INVALID_INDEX);
            }
            else
            {
                setResultCode (entity, RESULT_CODE_OK);
            }
        }
        break;

        case LM_REQ_OK:
        {
            /* phone book record out of specified range, end command */
            if( (vgLmData->readMode == LM_READ_NEXT) &&
                (vgLmData->phoneIndex2 < sig_p->recordNumber))
            {
                setResultCode (entity, RESULT_CODE_OK);
            }
            else if( getCommandId (entity) == VG_AT_GN_SCPBR)
            {
                vgScpbrProcessLmReadDialNum(    sig_p,
                                                vgLmData,
                                                entity);
            }
            else
            {
                switch (getCommandId (entity))
                {
                    case VG_AT_GN_CPBF:
                    {
                        /* match record alpha id with search alpha id */
                        if (lengthFind <= lengthTest)
                        {
                            alphaFound = vgAlphaStringSearch(   alphaText,
                                                                lengthTest,
                                                                vgLmData->alpha,
                                                                vgLmData->alphaLength,
                                                                entity);

                            if (alphaFound == TRUE)
                            {
                                if (vgLmData->firstDialNumRecordFromList == TRUE)
                                {
                                    vgPutNewLine (entity);
                                }
                                vgPrintf (entity, (Char *)"+CPBF: ");
                            }
                        }
                    }
                    break;

                    case VG_AT_GN_CPBR:
                    {
                        if (vgLmData->firstDialNumRecordFromList == TRUE)
                        {
                            vgPutNewLine (entity);
                        }
                        vgPrintf (entity, (Char *)"+CPBR: ");
                        // M_FrGkiPrintf0 (0xE50E, ATCI, "pb improve: +CPBR");
                        GKI_TRACE0 (PB_IMPROVE_AT_CPBR, GKI_ATCI_INFO);
                    }
                    break;

#if 0
                    case VG_AT_GN_CNUM:
                    {
                        if (vgLmData->firstDialNumRecordFromList == TRUE)
                        {
                            vgPutNewLine (entity);
                        }
                        vgPrintf (entity, (Char *)"+CNUM: ");
                    }
                    break;
#endif

                    default:
                    {
                    }
                    break;
                }

                if(    (getCommandId (entity) == VG_AT_GN_CPBR) ||
                            (   (getCommandId (entity) == VG_AT_GN_CPBF) &&
                                (alphaFound) ) )
                {
                    if(getCommandId (entity) == VG_AT_GN_CPBR)
                    {
                        /* display the current phone book entry */
                        vgConvBcdToTextForPhonebook(    dialNumber_p->typeOfNumber,
                                            dialNumber_p->dialString,
                                            dialNumber_p->dialStringLength,
                                            conversionBuffer_p);
                    }
                    else
                    {
                        /* display the current phone book entry */
                        vgConvBcdToText(    dialNumber_p->typeOfNumber,
                                            dialNumber_p->dialString,
                                            dialNumber_p->dialStringLength,
                                            conversionBuffer_p);
                    }


                    switch(getCommandId (entity))
                    {
                        default:
                        {
                            vgPrintf(   entity,
                                        (Char *)"%d,\"%s\",",
                                        sig_p->recordNumber,
                                        conversionBuffer_p);
                        }
                        break;
                    }

                    /* if the dial string is empty in these phonebooks assume the
                    * number was withheld by the caller and output restricted type */

                    if( (   (vgLmData->phoneBook == DIAL_LIST_LNM)  ||
                            (vgLmData->phoneBook == DIAL_LIST_ICI)  ||
                            (vgLmData->phoneBook == DIAL_LIST_MBDN) ||
                            (vgLmData->phoneBook == DIAL_LIST_LNR)
                         ) &&
                        (dialNumber_p->dialStringLength == 0) )

                    {
                        vgPrintf (entity, (Char *)"%d,", VG_DIAL_NUMBER_RESTRICTED);
                    }
                    else
                    {
                        vgPrintf (entity, (Char *)"%d,",
                        vgBcdNumberTypeToChar (dialNumber_p->typeOfNumber));
                    }

                    /* for missed, received, or dialled calls get AlphaId if not present */
                    vgPutc (entity, '\"');
                    vgPutAlphaId (entity, alphaText, lengthTest);
                    vgPutc (entity, '\"');

                    vgLmData->hiddenEntry = sig_p->hiddenEntry;
                    /* Read group info only for 3G SIMs */
                    if( (simLockGenericContext_p->simInfo.phase == SIM_PHASE_3G) &&
                    /*request to read association data should only be sent for one of the files below (global PB or local PB)*/
                    /*ALso, 2G SIMs don' t support email/group/sne/anr files, so there' s no point requesting it for 2G SIMs*/
                        (   (vgLmData->phoneBook == DIAL_LIST_ADN_GLB) ||
                            (vgLmData->phoneBook == DIAL_LIST_ADN_APP) ) )
                    {
                        /* Set missing Parameters counter to 0 */
                        vgLmData->missingParamCount = 0;
                        vgLmData->phoneIndex1 = sig_p->recordNumber;

                        vgPrintf (entity, (Char *)",%d", vgLmData->hiddenEntry);
                        if( (vgLmData->grpInfo.groupSupported) ||
                            (vgLmData->sneSupported) ||
                            (vgLmData->emailInfo.emailSupported) ||
                            (vgLmData->adNumInfo.anrSupported) )
                        {
                            pendingGroupLookUp = TRUE;
                            vgLmData->assocData = VG_PB_GROUP;
                            /*the USIM is able to store some association data, so read these*/
                            vgReadAssociationData (entity, vgLmData);
                        }
                    }
                    if ( !pendingGroupLookUp )
                    {
                        vgPutNewLine (entity);
                    }
                    
                    vgLmData->firstDialNumRecordFromList = FALSE;  /* into the list */
                }
#if 0                  
                else
                {
                    if (getCommandId (entity) == VG_AT_GN_CNUM)
                    {
                        {
                            /* display the current phone book entry */
                            vgPutc (entity, '\"');
                            vgPutAlphaId (entity, alphaText, lengthTest);
                            vgConvBcdToText(    dialNumber_p->typeOfNumber,
                                                dialNumber_p->dialString,
                                                dialNumber_p->dialStringLength,
                                                conversionBuffer_p);

                            vgPrintf(   entity,
                                        (Char *)"\",\"%s\",%d",
                                        conversionBuffer_p,
                                        vgBcdNumberTypeToChar (dialNumber_p->typeOfNumber));

                        }
                        vgPutNewLine (entity);

                        vgLmData->firstDialNumRecordFromList = FALSE;  /* into the list */
                    }
                }
#endif                
                if (vgLmData->readMode == LM_READ_NEXT)
                {
                    /* looking for an Alpha Id in LNx phonebook */
                    if( (vgLmData->alphaLength == lengthTest) &&
                        (!memcmp (&(vgLmData->alpha[0]), &(alphaText[0]), lengthTest)))
                    {
                        vgContinueCallStatusRequest (dialNumber_p, entity);
                    }
                    else
                    {
                        vgLmData->phoneIndex1 = sig_p->recordNumber;
                        setResultCode(  entity,
                                        vgChManContinueAction (entity, SIG_APEX_LM_READ_DIALNUM_REQ));
                    }
                }
                else
                {
                    vgContinueCallStatusRequest (dialNumber_p, entity);
                }
            }
        }
        break;

        case LM_REQ_SIM_ERROR:
        default:
        {
            if (getCommandId (entity)!= VG_AT_NO_COMMAND)
            {
                /* there was a SIM error, and so et the result code appropriately */
                setResultCode (entity, vgGetLmCmeErrorCode (sig_p->requestStatus));
            }
        }
        break;
    }

    vgFlushBuffer (entity);

    KiFreeMemory( (void**)&conversionBuffer_p);
}


/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmWriteDialNumCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_WRITE_DIALNUM_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: If the write request to the phone book was successful then
 *              return (result) code okay, otherwise return error code
 *----------------------------------------------------------------------------*/

void vgSigApexLmWriteDialNumCnf (const SignalBuffer *signalBuffer,
                                  const VgmuxChannelNumber entity)
{
    ApexLmWriteDialNumCnf      *sig_p = &signalBuffer->sig->apexLmWriteDialNumCnf;
    GeneralContext_t           *generalContext_p = ptrToGeneralContext (entity);
    VgLmData                   *vgLmData_p;
    SimLockGenericContext_t    *simLockGenericContext_p = ptrToSimLockGenericContext();
    VgSimInfo                  *simInfo = &(simLockGenericContext_p->simInfo);
    SimLockContext_t           *simLockContext_p = ptrToSimLockContext(entity);

    FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p         = &generalContext_p->vgLmData;

    switch( getCommandId (entity))
    {
        case VG_AT_GN_SCPBW:
        case VG_AT_GN_CPBW:
        {
            /* set the result code depending the the success of the write request */
            if( sig_p->requestStatus == LM_REQ_OK)
            {
                if( (   (vgLmData_p->phoneBook == DIAL_LIST_ADN_GLB) ||
                        (vgLmData_p->phoneBook == DIAL_LIST_ADN_APP) ) &&
                    (simLockGenericContext_p->simInfo.phase == SIM_PHASE_3G) )
                {
                    /*this grouping info may only be valid on 3G SIMs for the global phonebook
                    * or the local one. */
                    vgLmData_p->assocData = VG_PB_GROUP;

                    /* Update the record number to avoid problem with CPBW without index parameter*/
                    vgLmData_p->phoneIndex1 = sig_p->recordNumber;

                    vgUpdateAssociationData(entity, vgLmData_p);
                }
                else
                {
                    setResultCode (entity, RESULT_CODE_OK);
                }
            }
            else
            {
                setResultCode (entity, vgGetLmCmeErrorCode (sig_p->requestStatus));
            }
        }
        break;

        default:
        {
            FatalParam( getCommandId (entity), 0, 0);
        }
        break;
    }


}


 /*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmDeleteDialNumCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_DELETE_DIALNUM_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: If the delete record request was successful then return
 *              result code okay, otherwise return the error code
 *-------------------------------------------------------------------------*/

void vgSigApexLmDeleteDialNumCnf (const SignalBuffer *signalBuffer,
                                   const VgmuxChannelNumber entity)
{
    ApexLmDeleteDialNumCnf *sig_p = &signalBuffer->sig->apexLmDeleteDialNumCnf;

    VgLmData               *vgLmData_p;
    GeneralContext_t       *generalContext_p = ptrToGeneralContext (entity);

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    switch( getCommandId (entity))
    {
        case VG_AT_GN_SCPBW:
        case VG_AT_GN_CPBW:
        {
            switch (vgLmData_p->phoneBookOperation)
            {
                case VG_PB_DELETE:
                {
                    /* set the result code depending the the success of the delete request */
                    if( (sig_p->requestStatus == LM_REQ_OK) ||
                        (sig_p->requestStatus == LM_REQ_RECORD_NOT_FOUND) )
                    {
                        setResultCode (entity, RESULT_CODE_OK);
                    }
                    else
                    {
                        setResultCode (entity, vgGetLmCmeErrorCode( sig_p->requestStatus));
                    }
                }
                break;

                case VG_PB_WRITE:
                {
                    if( vgLmData_p->deleteRes == RESULT_CODE_OK) /* Case of delete before a write operation */
                    {
                         /* If problem with the record, it means it doesn't exists, so is not a replace operation*/
                        vgLmData_p->forReplace = (sig_p->requestStatus == LM_REQ_OK);

                        /* writing the entry in a specified location */
                        vgLmData_p->writeMode = LM_WRITE_ABSOLUTE;
                        setResultCode(  entity,
                                        vgChManContinueAction (entity, SIG_APEX_LM_WRITE_DIALNUM_REQ));
                    }
                    else /* Case of delete because write operation fail */
                    {
                        setResultCode(  entity, vgLmData_p->deleteRes);
                    }
                }
                break;

                default:
                {
                    DevParam( (Int8)vgLmData_p->phoneBookOperation, 0, 0);
                }
                break;
            }
        }
        break;

        default:
        {
            DevParam( getCommandId (entity), 0, 0);
        }
        break;
    }
}

#endif /* FEA_PHONEBOOK */

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimReadSpnCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal.
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Displays the service provider name returned from the SIM
 *----------------------------------------------------------------------------*/

void vgSigApexSimReadSpnCnf (const SignalBuffer *signalBuffer,
                              const VgmuxChannelNumber entity)
{
  ApexSimReadSpnCnf *sig_p = &signalBuffer->sig->apexSimReadSpnCnf;

  Char spnDisplay[SIM_MAX_SPN_SIZE + NULL_TERMINATOR_LENGTH] = {0};

  /* Deconstuct Signal and fill in vgNetworkName variable */
  switch(getCommandId (entity))
  {
    case VG_AT_GN_MSPN:
      if (sig_p->requestStatus == SIM_REQ_OK)
      {
        FatalAssert (sig_p->spnData.spnSize <= SIM_MAX_SPN_SIZE);

        memcpy (spnDisplay, sig_p->spnData.spn, sig_p->spnData.spnSize);
        spnDisplay[sig_p->spnData.spnSize] = NULL_CHAR;

        vgPutNewLine (entity);

        vgPrintf (entity, (Char *)"%C: \"");

        vgPutAlphaId(entity, spnDisplay, sig_p->spnData.spnSize);

        vgPrintf (entity, (Char *)"\",%d", sig_p->spnData.displayCond);

        vgPutNewLine (entity);

        setResultCode (entity, RESULT_CODE_OK);
      }
      else
      {
        setResultCode (entity, vgGetSimReqCmeErrorCode (sig_p->requestStatus));
      }

      break;

    case VG_AT_GN_SPN:
      if (sig_p->requestStatus == SIM_REQ_OK)
      {
        FatalAssert (sig_p->spnData.spnSize <= SIM_MAX_SPN_SIZE);

        memcpy (spnDisplay, sig_p->spnData.spn, sig_p->spnData.spnSize);
        spnDisplay[sig_p->spnData.spnSize] = NULL_CHAR;

        vgPutNewLine (entity);

        vgPrintf (entity, (Char *)"%C: %d,0,\"", sig_p->spnData.displayCond);

        vgPutAlphaId(entity, spnDisplay, sig_p->spnData.spnSize);

        vgPrintf(entity, (Char *)"\"");

        vgPutNewLine (entity);

        setResultCode (entity, RESULT_CODE_OK);
      }
      else
      {
        setResultCode (entity, vgGetSimReqCmeErrorCode (sig_p->requestStatus));
      }
      break;

    default:
      /* Unexpected command readin SPN */
      FatalParam(entity, getCommandId (entity), 0);
      break;
  }
}

 /*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimPinFunctionCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_SIM_PIN_FUNCTION_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: If the passcode was correct then continue with the delayed
 *              access function calls.
 *----------------------------------------------------------------------------*/

void vgSigApexSimPinFunctionCnf (const SignalBuffer *signalBuffer,
                                 const VgmuxChannelNumber entity)
{
  ApexSimPinFunctionCnf   *sig_p = &signalBuffer->sig->apexSimPinFunctionCnf;
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
  SupplementaryContext_t  *supplementaryContext_p = ptrToSupplementaryContext (entity);
  GeneralContext_t        *generalContext_p       = ptrToGeneralContext       (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(supplementaryContext_p != PNULL, entity, 0, 0);
  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  vgUpdate3GPinStatus (sig_p);

  /* decide what to do next based on curent command and request success */
  switch (sig_p->pinFunction)
  {
    case SIM_PIN_FUNCT_VERIFY:
    {
      if ((sig_p->requestStatus == SIM_REQ_OK) &&
          (sig_p->pinFunctionSuccess))
      {
        switch (getCommandId (entity))
        {

#if defined (FEA_PHONEBOOK)          
          case VG_AT_SS_CLCK:
          {            
            if (generalContext_p->updatingBdn == TRUE)
            {
              setResultCode (entity,
               vgChManContinueAction (entity, SIG_APEX_LM_BARRED_DIAL_REQ));
            }
            else
            {
              setResultCode (entity,
               vgChManContinueAction (entity, SIG_APEX_LM_FIXED_DIAL_REQ));
            }
            break;
          }
#endif /* FEA_PHONEBOOK */

          case VG_AT_SL_CPIN:
          case VG_AT_SL_MUPIN:
          {
            vgSetSimState (VG_SIM_READY);
            setResultCode (entity, RESULT_CODE_OK);
            break;
          }
          case VG_AT_SL_CMAR:
          {
            setResultCode(entity, vgSlMasterReset(entity));
            break;
          }
#if defined (FEA_PHONEBOOK)          
          case VG_AT_GN_CPBS:
          {
            setResultCode (entity, RESULT_CODE_OK);
            break;
          }
#endif /* FEA_PHONEBOOK */
#if defined (FEA_ACL)

          case VG_AT_GP_MSACL:
          {
            setResultCode (entity,
            vgChManContinueAction (entity, SIG_APEX_ABPD_SET_ACL_REQ));
            break;
          }
          case VG_AT_GP_MDACL:
          {
            setResultCode (entity,
            vgChManContinueAction (entity, SIG_APEX_ABPD_DELETE_ACL_REQ));
            break;
          }
          case VG_AT_GP_MWACL:
          {
            setResultCode (entity,
            vgChManContinueAction (entity, SIG_APEX_ABPD_WRITE_ACL_REQ));
            break;
          }
#endif          
          default:
          {
            /* Unexpected command using PIN function */
            FatalParam(entity, getCommandId (entity), 0);
            break;
          }
        }
      }
      else
      {
        setResultCode (entity,
        vgGetSimPinCmeErrorCode (sig_p->requestStatus,
                                 sig_p->pinKeyReference));
      }
      break;
    }

    case SIM_PIN_FUNCT_CHANGE:
    {
      switch (getCommandId (entity))
      {
        case VG_AT_SS_CLCK:
        case VG_AT_SS_CPWD:
        case VG_AT_SL_MUPIN:
        {
          /* Check status.... */
          if (sig_p->requestStatus == SIM_REQ_OK)
          {
            vgSetSimState (VG_SIM_READY);
            setResultCode (entity, RESULT_CODE_OK);
          }
          else
          {
            setResultCode (entity,
                            vgGetSimPinCmeErrorCode (sig_p->requestStatus,
                                 sig_p->pinKeyReference));
          }
          break;
        }
        default:
        {
          /* Unknown AT command response */
          FatalParam(entity, getCommandId (entity), 0);
//          setResultCode (entity, RESULT_CODE_ERROR);
          break;
        }
      }
      break;
    }

    case SIM_PIN_FUNCT_ENABLE:
    {
      switch (getCommandId (entity))
      {
        case VG_AT_SS_CLCK:
        case VG_AT_SL_MUPIN:
        {
          if (sig_p->requestStatus == SIM_REQ_OK)
          {
            setResultCode (entity, RESULT_CODE_OK);

            /* Set pin code status.... */
            if ((supplementaryContext_p->ssParams.currentFac == VG_AT_SS_FAC_SC) ||
                 (getCommandId (entity) == VG_AT_SL_MUPIN))
            {
              simLockGenericContext_p->simInfo.pinEnabled = TRUE;
              /* job126452: need to update SIM state */
              vgSetSimState (VG_SIM_READY);
            }
            else
            {
              /* Unhandled response */
              FatalParam(entity, getCommandId (entity), supplementaryContext_p->ssParams.currentFac);
//              setResultCode (entity, RESULT_CODE_ERROR);
            }
          }
          else
          {
            setResultCode (entity,
                           vgGetSimPinCmeErrorCode (sig_p->requestStatus,
                           sig_p->pinKeyReference));
          }
          break;
        }
        default:
        {
          /* Unknown AT command response */
          FatalParam(entity, getCommandId (entity), 0);
         // setResultCode (entity, RESULT_CODE_ERROR);
          break;
        }
      }
      break;
    }

    case SIM_PIN_FUNCT_DISABLE:
    {
      switch (getCommandId (entity))
      {
        case VG_AT_SS_CLCK:
        case VG_AT_SL_MUPIN:
        {
          if (sig_p->requestStatus == SIM_REQ_OK)
          {
            setResultCode (entity, RESULT_CODE_OK);

            /* Set pin code status.... */
            if ((supplementaryContext_p->ssParams.currentFac == VG_AT_SS_FAC_SC) ||
                 (getCommandId (entity) == VG_AT_SL_MUPIN))
            {
              simLockGenericContext_p->simInfo.pinEnabled = FALSE;
              /* job126452: need to update SIM state */
              vgSetSimState (VG_SIM_READY);
            }
            else
            {
              /* Unhandled response */
              FatalParam(entity, getCommandId (entity), supplementaryContext_p->ssParams.currentFac);
              setResultCode (entity, RESULT_CODE_ERROR);
            }
          }
          else
          {
            setResultCode (entity,
                            vgGetSimPinCmeErrorCode (sig_p->requestStatus,
                                 sig_p->pinKeyReference));
          }
          break;
        }
        default:
        {
          /* Unknown AT command response */
          FatalParam(entity, getCommandId (entity), 0);
//          setResultCode (entity, RESULT_CODE_ERROR);
          break;
        }
      }
      break;
    }

    case SIM_PIN_FUNCT_UNBLOCK:
    {

      if ((sig_p->requestStatus == SIM_REQ_OK) &&
          (sig_p->pinFunctionSuccess))
      {
        switch (getCommandId (entity))
        {
          case VG_AT_SL_CPIN:
          case VG_AT_SL_MUPIN:
          {
            vgSetSimState (VG_SIM_READY);

            setResultCode (entity, RESULT_CODE_OK);
            break;
          }
          default:
          {
            /* Unexpected command using PIN function */
            FatalParam(entity, getCommandId (entity), 0);
            break;
          }
        }
      }
      else
      {
        setResultCode (entity,
        vgGetSimPinCmeErrorCode (sig_p->requestStatus,
                                 sig_p->pinKeyReference));
      }
      break;
    }

    default:
    {
      /* Unexpected PIN function request */
      FatalParam(entity, sig_p->pinFunction, 0);
      break;
    }
  }


}

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimChvFunctionCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_SIM_CHV_FUNCTION_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: If the passcode was correct then continue with the delayed
 *              access function calls.
 *----------------------------------------------------------------------------*/

void vgSigApexSimChvFunctionCnf (const SignalBuffer *signalBuffer,
                                  const VgmuxChannelNumber entity)
{
  ApexSimChvFunctionCnf   *sig_p = &signalBuffer->sig->apexSimChvFunctionCnf;
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
  SupplementaryContext_t  *supplementaryContext_p = ptrToSupplementaryContext (entity);
  GeneralContext_t        *generalContext_p       = ptrToGeneralContext       (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(supplementaryContext_p != PNULL, entity, 0, 0);
  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  vgUpdate2GPinStatus (sig_p);
  /* decide what to do next based on curent command and request success */
  switch (sig_p->chvFunction)
  {
    case SIM_PIN_FUNCT_VERIFY:
    {
      if ((sig_p->requestStatus == SIM_REQ_OK) &&
          (sig_p->chvFunctionSuccess == TRUE))
      {
        if (sig_p->chvNum == SIM_CHV_2)
        {
          /* password (chv2) verified */
          simLockGenericContext_p->simInfo.pin2Verified = TRUE;
        }
        switch (getCommandId (entity))
        {
#if defined (FEA_PHONEBOOK)          
          case VG_AT_SS_CLCK:
          {
            if (generalContext_p->updatingBdn == TRUE)
            {
              setResultCode (entity,
               vgChManContinueAction (entity, SIG_APEX_LM_BARRED_DIAL_REQ));
            }
            else
            {
              setResultCode (entity,
               vgChManContinueAction (entity, SIG_APEX_LM_FIXED_DIAL_REQ));
            }
            break;
          }
#endif /* FEA_PHONEBOOK */          
          case VG_AT_SL_CPIN:
          case VG_AT_SL_MUPIN:
          {
            vgSetSimState (VG_SIM_READY);

            setResultCode (entity, RESULT_CODE_OK);
            break;
          }
          case VG_AT_SL_CMAR:
          {
            setResultCode(entity, vgSlMasterReset(entity));
            break;
          }
#if defined (FEA_PHONEBOOK)          
          case VG_AT_GN_CPBS:
          {
            setResultCode (entity, RESULT_CODE_OK);
            break;
          }
#endif /* FEA_PHONEBOOK */          
#if defined (FEA_ACL)
          case VG_AT_GP_MSACL:
          {
            setResultCode (entity,
            vgChManContinueAction (entity, SIG_APEX_ABPD_SET_ACL_REQ));
            break;
          }
          
          case VG_AT_GP_MDACL:
          {
            setResultCode (entity,
            vgChManContinueAction (entity, SIG_APEX_ABPD_DELETE_ACL_REQ));
            break;
          }
          case VG_AT_GP_MWACL:
          {
            setResultCode (entity,
            vgChManContinueAction (entity, SIG_APEX_ABPD_WRITE_ACL_REQ));
            break;
          }
#endif          
          default:
          {
            /* Unexpected command using Chv function */
            FatalParam(entity, getCommandId (entity), 0);
            break;
          }
        }
      }
      else
      {

        setResultCode (entity,
        vgGetSimChvCmeErrorCode (sig_p->requestStatus,
                                   sig_p->chvNum));
      }
      break;
    }
    case SIM_PIN_FUNCT_CHANGE:
    {
      switch (getCommandId (entity))
      {
        case VG_AT_SS_CLCK:
        case VG_AT_SS_CPWD:
        case VG_AT_SL_MUPIN:
        {
          /* Check status.... */
          if (sig_p->requestStatus == SIM_REQ_OK)
          {
            if (sig_p->chvNum == SIM_CHV_2)
            {
              /* password (chv2) verified */
              simLockGenericContext_p->simInfo.pin2Verified = TRUE;
            }
            vgSetSimState (VG_SIM_READY);
            setResultCode (entity, RESULT_CODE_OK);
          }

          else
          {
            setResultCode (entity,
                            vgGetSimChvCmeErrorCode (sig_p->requestStatus,
                                                      sig_p->chvNum));
          }
          break;
        }
        default:
        {
          /* Unknown AT command response */
          FatalParam(entity, getCommandId (entity), 0);
//          setResultCode (entity, RESULT_CODE_ERROR);
          break;
        }
      }
      break;
    }


    case SIM_PIN_FUNCT_ENABLE:
    {
      switch (getCommandId (entity))
      {
        case VG_AT_SS_CLCK:
        case VG_AT_SL_MUPIN:
        {
          if (sig_p->requestStatus == SIM_REQ_OK)
          {
            setResultCode (entity, RESULT_CODE_OK);


            /* Set pin code status.... */
            if ((supplementaryContext_p->ssParams.currentFac == VG_AT_SS_FAC_SC) ||
                 (getCommandId (entity) == VG_AT_SL_MUPIN))
            {
              simLockGenericContext_p->simInfo.pinEnabled = TRUE;
              vgSetSimState (VG_SIM_READY);
            }
            else
            {
              /* Unhandled response */
              FatalParam(entity, getCommandId (entity), supplementaryContext_p->ssParams.currentFac);
//              setResultCode (entity, RESULT_CODE_ERROR);
            }
          }

          else
          {
            setResultCode (entity,
                            vgGetSimChvCmeErrorCode (sig_p->requestStatus,
                                                      sig_p->chvNum));
          }
          break;
        }
        default:
        {
          /* Unknown AT command response */
          FatalParam(entity, getCommandId (entity), 0);
//          setResultCode (entity, RESULT_CODE_ERROR);
          break;
        }
      }
      break;
    }

    case SIM_PIN_FUNCT_DISABLE:
    {
      switch (getCommandId (entity))
      {
        case VG_AT_SS_CLCK:
        case VG_AT_SL_MUPIN:
        {
          if (sig_p->requestStatus == SIM_REQ_OK)
          {
            setResultCode (entity, RESULT_CODE_OK);

            /* Set pin code status.... */
            if ((supplementaryContext_p->ssParams.currentFac == VG_AT_SS_FAC_SC) ||
                 (getCommandId (entity) == VG_AT_SL_MUPIN))
            {
              simLockGenericContext_p->simInfo.pinEnabled = FALSE;
              vgSetSimState (VG_SIM_READY);
            }
            else
            {
              /* Unhandled response */
              FatalParam(entity, getCommandId (entity), supplementaryContext_p->ssParams.currentFac);
//              setResultCode (entity, RESULT_CODE_ERROR);
            }
          }
          else
          {
            setResultCode (entity,
                            vgGetSimChvCmeErrorCode (sig_p->requestStatus,
                                                      sig_p->chvNum));
          }
          break;
        }
        default:
        {
          FatalParam(entity, getCommandId (entity), 0);
  //        setResultCode (entity, RESULT_CODE_ERROR);
          break;
        }
      }
      break;
    }
    case SIM_PIN_FUNCT_UNBLOCK:
    {
      if ((sig_p->requestStatus == SIM_REQ_OK) &&
          (sig_p->chvFunctionSuccess == TRUE))
      {
        switch (getCommandId (entity))
        {
          case VG_AT_SL_CPIN:
          case VG_AT_SL_MUPIN:
          {
            vgSetSimState (VG_SIM_READY);

            setResultCode (entity, RESULT_CODE_OK);
            break;
          }
          default:
          {
            /* Unexpected command using Chv function */
            FatalParam(entity, getCommandId (entity), 0);
            break;
          }
        }
      }
      else
      {
        setResultCode (entity,
         vgGetSimChvCmeErrorCode (sig_p->requestStatus,
                                   sig_p->chvNum));
      }
      break;
    }

    default:
    {
      /* Unexpected Chv function request */
      FatalParam(entity, sig_p->chvFunction, getCommandId (entity));
      break;
    }
  }

}

#if defined (FEA_SIMLOCK)
/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimMepCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_SIM_MEP_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Handles the return of the Mobile Equipment Personalisation
 *              signal (which enables/disables features).
 *----------------------------------------------------------------------------*/

void vgSigApexSimMepCnf (const SignalBuffer *signalBuffer,
                         const VgmuxChannelNumber entity)
{
  ApexSimMepCnf *sig_p = &signalBuffer->sig->apexSimMepCnf;

  switch (getCommandId (entity))
  {
    case VG_AT_SS_CLCK:
    case VG_AT_SS_CPWD:
    {
      if (sig_p->result == PERSONALISATION_OK)
      {
        /* Everything worked.... */
        setResultCode (entity, RESULT_CODE_OK);
      }
      else if (sig_p->result == PERSONALISATION_INVALID_LOCK_CODE)
      {
        /* Wrong PIN code.... */
        setResultCode (entity, VG_CME_INCORRECT_PASSWORD);
      }
      else
      {
        /* Error, not allowed.... */
        setResultCode (entity, VG_CME_OPERATION_NOT_ALLOWED);
      }
      break;
    }
    default:
    {
      /* Unexpected response */
      FatalParam(entity, getCommandId (entity), 0);
//      setResultCode (entity, RESULT_CODE_ERROR);
      break;
    }
  }
}

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimMepStatusCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_SIM_MEP_STATUS_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Handles the return of the Mobile Equipment Personalisation
 *              status signal.
 *----------------------------------------------------------------------------*/

void vgSigApexSimMepStatusCnf (const SignalBuffer *signalBuffer,
                         const VgmuxChannelNumber entity)
{
  SupplementaryContext_t  *supplementaryContext_p = ptrToSupplementaryContext (entity);
  ApexSimMepStatusCnf     *sig_p = &signalBuffer->sig->apexSimMepStatusCnf;
  Int8        index = 0;
  Boolean     found = FALSE;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(supplementaryContext_p != PNULL, entity, 0, 0);
#endif
  switch (getCommandId (entity))
  {
    case VG_AT_SS_CLCK:
    {
      if (((supplementaryContext_p->ssParams.currentFac ) >= VG_AT_SS_FAC_PS) &&
          ((supplementaryContext_p->ssParams.currentFac  - VG_AT_SS_FAC_PS) < NUM_MEP_PERSONALISATIONS ) &&
          (sig_p->lockStates[supplementaryContext_p->ssParams.currentFac  - VG_AT_SS_FAC_PS] < MEP_LOCK_DISABLED))
      {
        /* Print out status of current FAC MEP element.... */
        vgPutNewLine (entity);
        vgPrintf (entity,
                  (Char *)"+CLCK: %d",
                   sig_p->lockStates[supplementaryContext_p->ssParams.currentFac  - VG_AT_SS_FAC_PS]);
        vgPutNewLine (entity);

        setResultCode (entity, RESULT_CODE_OK);
      }
      else
      {
        setResultCode (entity, VG_CME_OPERATION_NOT_ALLOWED);
      }
      break;
    }

    /* This CNF is request by CPIN because CPIN sim status is VG_CME_SIM_WRONG */
    /* It will response the reason of SIM_WRONG */
    case VG_AT_SL_CPIN:
    {
      while ((index < NUM_MEP_PERSONALISATIONS) && (found != TRUE))
      {
        if (MEP_LOCK_ACTIVE == sig_p->lockStates[index])
        {
          vgPutNewLine (entity);
          vgPrintf (entity, (const Char*)"+CPIN: %s", MEPMessage[index]);
          vgPutNewLine (entity);
          setResultCode (entity, RESULT_CODE_OK);
          found = TRUE;
        }
        index++;
      }
      if (FALSE == found)
      {
        /* Unexpected MEP error */
        FatalParam(entity, 0, 0);
      }
    }
    break;

    default:
    {
      FatalParam(entity, getCommandId (entity), 0);
//      setResultCode (entity, RESULT_CODE_ERROR);
      break;
    }
  }

}

#if defined(FEA_NOT_SLIM_SIM_CODE)
/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimWriteMepNetworkIdCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_SIM_WRITE_MEP_NETWORK_ID_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Handles the return of the Write Mep Network Id signal
 *----------------------------------------------------------------------------*/

void vgSigApexSimWriteMepNetworkIdCnf (const SignalBuffer *signalBuffer,
                                       const VgmuxChannelNumber entity)
{
  PARAMETER_NOT_USED (signalBuffer);
  PARAMETER_NOT_USED (entity);
}

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexSimReadMepNetworkIdCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_SIM_READ_MEP_NETWORK_ID_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Handles the return of the Read Mep Network Id signal
 *----------------------------------------------------------------------------*/

void vgSigApexSimReadMepNetworkIdCnf (const SignalBuffer *signalBuffer,
                                      const VgmuxChannelNumber entity)
{
  PARAMETER_NOT_USED (signalBuffer);
  PARAMETER_NOT_USED (entity);
}
#endif
#endif /* FEA_SIMLOCK */

#if defined (FEA_PHONEBOOK)
/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmFixedDialCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_FIXED_DIAL_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Status of request which deterines whether provided number
 *              is allowed for operation (i.e. has entry in FDN phonebook).
 *-------------------------------------------------------------------------*/
void vgSigApexLmFixedDialCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  ApexLmFixedDialCnf       *sig_p = &signalBuffer->sig->apexLmFixedDialCnf;
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
  GeneralContext_t         *generalContext_p        = ptrToGeneralContext  (entity);

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  simLockGenericContext_p->simInfo.fdnIsEnabled = sig_p->fdnEnabled;

  /* check if request was successful */
  if ((sig_p->requestStatus == LM_REQ_OK) &&
      (sig_p->fdnEnabled    == generalContext_p->enableFdn))
  {
    setResultCode (entity, RESULT_CODE_OK);
  }
  else
  {
    setResultCode (entity, RESULT_CODE_ERROR);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmBarredDialCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_BARRED_DIAL_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Status of request which deterines whether provided number
 *              is allowed for operation (i.e. has entry in BDN phonebook).
 *-------------------------------------------------------------------------*/
void vgSigApexLmBarredDialCnf (const SignalBuffer *signalBuffer,
                                const VgmuxChannelNumber entity)
{
  ApexLmBarredDialCnf      *sig_p = &signalBuffer->sig->apexLmBarredDialCnf;
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
  GeneralContext_t         *generalContext_p = ptrToGeneralContext (entity);

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  simLockGenericContext_p->simInfo.bdnIsEnabled = sig_p->bdnEnabled;

  /* check if request was successful */
  if ((sig_p->requestStatus == LM_REQ_OK) &&
      (sig_p->bdnEnabled    == generalContext_p->enableBdn))
  {
    setResultCode (entity, RESULT_CODE_OK);
  }
  else
  {
    setResultCode (entity, RESULT_CODE_ERROR);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmGetAlphaCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_GET_ALPHA_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to dial number-alpha lookup in phonebook.
 *-------------------------------------------------------------------------*/
void vgSigApexLmGetAlphaCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
  ApexLmGetAlphaCnf      *sig_p = &signalBuffer->sig->apexLmGetAlphaCnf;
  GeneralContext_t       *generalContext_p = ptrToGeneralContext (entity);
  VgAlphaLookup          *vgAlphaLookup_p = PNULL;
  SsCallRelatedContext_t *ssCallRelatedContext_p = ptrToSsCallRelatedContext ();
  VgSsCallerIdData       *vgSsCallerIdData_p     = PNULL;

#if defined (ENABLE_LONG_AT_CMD_RSP)
  Char                   conversionBuffer[AT_MEDIUM_BUFF_SIZE] = {0};
#else  
  Char                   conversionBuffer[CIMUX_MAX_AT_DATA_LENGTH] = {0};
#endif

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  if ((sig_p->callId <=  MAX_USER_CALL_ID)&&(sig_p->callId >0))
  {
    vgSsCallerIdData_p = &(ssCallRelatedContext_p->vgSsCallerIdData[sig_p->callId - 1]);
  }
  if ((sig_p->callId <= VG_MAX_USER_CALL_ID)&&(sig_p->callId>0))
  {
    vgAlphaLookup_p = &(generalContext_p->vgAlphaLookup[sig_p->callId - 1]);
    vgAlphaLookup_p->active = FALSE;
  }

  generalContext_p->pendingAlphaReq--;

  switch (sig_p->callId)
  {
    case VG_PHONE_BOOK_CALL_ID:
    {
      displayPhoneBookAlphaId (entity, sig_p);
      break;
    }
    case VG_SMS_LMGETALPHA_COMMAND_CALL_ID:
    case VG_SMS_LMGETALPHA_UNSOLICITED_CALL_ID:
    {
      vgSmsSigInApexLmGetAlphaCnf (entity, sig_p);
      break;
    }
    default:
    {
      /* Do nothing - for SS not supported */
      break;
    }
  }
}

#endif /* FEA_PHONEBOOK */

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigAclkDateAndTimeInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_DM_RTC_DATE_AND_TIME_IND
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Checks if time zone update was successful
 *----------------------------------------------------------------------------*/
void vgSigAclkDateAndTimeInd (const SignalBuffer *signalBuffer)
{
  DmRtcDateAndTimeInd *sig_p = &signalBuffer->sig->dmRtcDateAndTimeInd;
  MobilityContext_t  *mobilityContext_p = ptrToMobilityContext ();
  Int8 timeZone;
  VgmuxChannelNumber profileEntity =  VGMUX_CHANNEL_INVALID;

  for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
  {
    if ((isEntityActive (profileEntity)) &&
        (getProfileValue (profileEntity, PROF_CTZR) == REPORTING_ENABLED))
    {
      /* check if time one has changed */
      if ((sig_p->dateAndTime.timeZone.format         != mobilityContext_p->currentTimeZone.format        ) ||
          (sig_p->dateAndTime.timeZone.offset.hours   != mobilityContext_p->currentTimeZone.offset.hours  ) ||
          (sig_p->dateAndTime.timeZone.offset.minutes != mobilityContext_p->currentTimeZone.offset.minutes))
      {
        /* display time zone */
        vgPutNewLine (profileEntity);

        timeZone = ((sig_p->dateAndTime.timeZone.offset.hours * 4) +
                    (sig_p->dateAndTime.timeZone.offset.minutes / 15));

        if (sig_p->dateAndTime.timeZone.format == RTC_DISP_FORMAT_POS)
        {
          vgPrintf (profileEntity,
                    (Char *)"+CTZV: \"+%02d\"", timeZone);
        }
        else
        {
          vgPrintf (profileEntity,
                    (Char *)"+CTZV: \"-%02d\"", timeZone);
        }

        vgPutNewLine (profileEntity);
        vgFlushBuffer(profileEntity);
      }
    }
  }
  /* copy latest time zone information */
  memcpy (&mobilityContext_p->currentTimeZone,
           &sig_p->dateAndTime.timeZone,
            sizeof(RtcDisplacement));

  mobilityContext_p->timeZoneInitialised = TRUE;
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexGlWriteFeatureConfigCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEXGL_WRITE_FEATURE_CONFIG_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to a write feature configuration request
 *-------------------------------------------------------------------------*/
void vgSigApexGlWriteFeatureConfigCnf(  const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity)
{
    ApexGlWriteFeatureConfigCnf *sig_p = &signalBuffer->sig->apexGlWriteFeatureConfigCnf;

    switch (getCommandId (entity))
    {
        case VG_AT_GN_MFTRCFG:
            {
                if (sig_p->requestStatus == GL_REQUEST_OK)
                {
                    setResultCode (entity, RESULT_CODE_OK);
                }
                else
                {
                    setResultCode (entity, RESULT_CODE_ERROR);
                }
            }
            break;

        default:
            {
                /* Invalid activation of vgSigApexGlWriteFeatureConfigCnf! */
                FatalParam(entity, getCommandId (entity), 0);
            }
            break;
    }
}

#if defined (FEA_PHONEBOOK)
/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmPhoneBookStatusCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_PHONEBOOK_STATUS_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Phonebook Status Request
 *-------------------------------------------------------------------------*/
void vgSigApexLmPhoneBookStatusCnf (const SignalBuffer *signalBuffer,
                                     const VgmuxChannelNumber entity)
{
    ApexLmPhoneBookStatusCnf   *sig_p                       = &signalBuffer->sig->apexLmPhoneBookStatusCnf;
    GeneralContext_t           *generalContext_p            = ptrToGeneralContext (entity);
    VgLmData                   *vgLmData_p;
    PhoneBookInfo               phoneBookInfo;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    phoneBookInfo = getPhoneBookInfo (sig_p, vgLmData_p->phoneBook);

    vgLmData_p->grpInfo.groupSupported      = FALSE;
    vgLmData_p->grpInfo.groupPerRec         = 0;
    vgLmData_p->grpInfo.gasNumRecords       = 0;
    vgLmData_p->grpInfo.gasSize             = 0;
    vgLmData_p->sneSupported                = FALSE;
    vgLmData_p->adNumInfo.anrSupported      = FALSE;
    vgLmData_p->adNumInfo.adNumPerRec       = 0;
    vgLmData_p->emailInfo.emailSupported    = (sig_p->emailEntriesPerAdn >0)? TRUE:FALSE;
    vgLmData_p->emailInfo.emailPerRec       = sig_p->emailEntriesPerAdn;
    vgLmData_p->aasInfo.aasSupported        = FALSE;
    vgLmData_p->aasInfo.aasNumRecord        = 0;
    vgLmData_p->aasInfo.aasSize             = 0;

    /* Limit the number of associated records to the maximum supported records*/
    if( vgLmData_p->grpInfo.groupPerRec > VG_MUPBCFG_MAX_AD_GROUPS + 1)
    {
        vgLmData_p->grpInfo.groupPerRec = VG_MUPBCFG_MAX_AD_GROUPS + 1;
    }
    if( vgLmData_p->adNumInfo.adNumPerRec > VG_MUPBCFG_MAX_AD_NUMBER + 1)
    {
        vgLmData_p->adNumInfo.adNumPerRec = VG_MUPBCFG_MAX_AD_NUMBER + 1;
    }
    if( vgLmData_p->emailInfo.emailPerRec > VG_MUPBCFG_MAX_AD_EMAIL + 1)
    {
        vgLmData_p->emailInfo.emailPerRec = VG_MUPBCFG_MAX_AD_EMAIL + 1;
    }

    /* job103099: indicate first record of list */
    vgLmData_p->firstDialNumRecordFromList = TRUE;

    switch (getCommandId (entity))
    {
        case VG_AT_GN_CPBR:
        {
            vgProcessPbStatusCpbr(  &phoneBookInfo,
                                    vgLmData_p,
                                    entity);
        }
        break;

        case VG_AT_GN_SCPBR:
        {
            vgProcessPbStatusScpbr( &phoneBookInfo,
                                    vgLmData_p,
                                    entity);
        }
        break;

        case VG_AT_GN_CPBW:
        {
            vgProcessPbStatusCpbw(  &phoneBookInfo,
                                    vgLmData_p,
                                    entity);
        }
        break;

        case VG_AT_GN_SCPBW:
        {
            vgProcessPbStatusScpbw( &phoneBookInfo,
                                    vgLmData_p,
                                    entity);
        }
        break;

        case VG_AT_GN_CPBF:
        {
            vgProcessPbStatusCpbf(  &phoneBookInfo,
                                    vgLmData_p,
                                    entity);
        }
        break;

        case VG_AT_GN_MUPBCFG:
        {
            vgProcessPbStatusMupbcfg(   &phoneBookInfo,
                                        vgLmData_p,
                                        entity);
        }
        break;

        case VG_AT_GN_MUPBGAS:
        {
            vgProcessPbStatusMupbgas(   &phoneBookInfo,
                                        vgLmData_p,
                                        entity);
        }
        break;

        case VG_AT_GN_MUPBAAS:
        {
            vgProcessPbStatusMupbaas(   &phoneBookInfo,
                                        vgLmData_p,
                                        entity);
        }
        break;

        default:
        {
        }
        break;
    }
}

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmHiddenKeyFunctionCnf(ApexSimGenAccessCnf *inSig);
 *
 * Parameters:  Pointer to a incoming ApexSimGenAccessCnf data structure
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_LM_HIDDEN_KEY_FUNCTION_CNF signal received
 *              from the background layer.
 *----------------------------------------------------------------------------*/
void vgSigApexLmHiddenKeyFunctionCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  const ApexLmHiddenKeyFunctionCnf *inSig = &(signalBuffer->sig->apexLmHiddenKeyFunctionCnf);

  if (inSig->requestStatus == LM_REQ_OK)
  {
    setResultCode (entity, RESULT_CODE_OK);
  }
  else
  {
    setResultCode (entity, VG_CME_INCORRECT_PASSWORD);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmReadGroupCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_READ_GROUP_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Read Group Request
 *-------------------------------------------------------------------------*/
void vgSigApexLmReadGrpCnf (const SignalBuffer *signalBuffer,
                            const VgmuxChannelNumber entity)
{
    ApexLmReadGrpCnf *sig_p = &signalBuffer->sig->apexLmReadGrpCnf;
    GeneralContext_t *generalContext_p = ptrToGeneralContext (entity);
    VgLmData         *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    /*  Right now, it is only called as a part of AT+CPBR handler. Other cases may follow,
    *   in which case some sort of FSM will be needed */

    if( sig_p->requestStatus == LM_REQ_OK)
    {
        memcpy (&vgLmData_p->grpInfo.grpData, &sig_p->groupingInfo, sizeof (LmGrpData));

        if( (sig_p->groupingInfo.numOfGrp != 0) &&
            (vgLmData_p->grpInfo.grpData.grpList[0] != 0) )
        {
            setResultCode(  entity,
                            vgChManContinueAction (entity, SIG_APEX_LM_READ_GAS_REQ));
        }
        else
        {
            /* does not belong to any group */
            vgLmData_p->missingParamCount++;
            /* read the next association data */
            vgReadAssociationData (entity, vgLmData_p);
        }
    }
    else if( sig_p->requestStatus == LM_REQ_OK_DATA_INVALID ||
            /*print missing parameter if illegal operation or file not supported*/
            (sig_p->requestStatus == LM_REQ_ILLEGAL_OPERATION) ||
            (sig_p->requestStatus == LM_REQ_FILE_NOT_SUPPORTED))
    {
        vgLmData_p->grpInfo.grpData.numOfGrp = 0;

        /* does not belong to any group */
        vgLmData_p->missingParamCount++;
        /* read the next association data */
        vgReadAssociationData (entity, vgLmData_p);
    }
    else
    {
        /*something went wrong, flag an error*/
        setResultCode (entity, VG_CME_CPBR_CANNOT_READ_GROUP);
    }
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmReadGasCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_READ_GAS_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Read Group Alpha String request
 *-------------------------------------------------------------------------*/
void vgSigApexLmReadGasCnf (const SignalBuffer *signalBuffer,
                            const VgmuxChannelNumber entity)
{
    ApexLmReadGasCnf *sig_p = &signalBuffer->sig->apexLmReadGasCnf;
    GeneralContext_t *generalContext_p = ptrToGeneralContext (entity);
    VgLmData         *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    switch( getCommandId( entity) )
    {
        case VG_AT_GN_CPBF:
        case VG_AT_GN_CPBR:
        {
            vgCpbrProcessLmReadGasCnf( sig_p, vgLmData_p, entity);
        }
        break;

        case VG_AT_GN_MUPBGAS:
        {
            vgMupbgasProcessLmReadGasCnf( sig_p, vgLmData_p, entity);
        }
        break;

        default:
        {
            DevParam( getCommandId( entity), 0, 0);
        }
        break;
    }
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmReadAnrCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_READ_ANR_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Read Anr request
 *-------------------------------------------------------------------------*/
void vgSigApexLmReadAnrCnf (const SignalBuffer *signalBuffer,
                            const VgmuxChannelNumber entity)
{
    ApexLmReadAnrCnf *sig_p = &signalBuffer->sig->apexLmReadAnrCnf;
    GeneralContext_t *generalContext_p = ptrToGeneralContext (entity);
    VgLmData         *vgLmData_p;
    Char             conversionBuffer[MAX_CALLED_BCD_NO_LENGTH + NULL_TERMINATOR_LENGTH] = {0};

    FatalCheck( generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p         = &generalContext_p->vgLmData;

    /*  Right now, it is only called as a part of AT+CPBR handler. Other cases
    *   may follow, in which case some sort of state mashine will be needed */

    if( (sig_p->requestStatus == LM_REQ_OK)&&
        (sig_p->anrDialNum.dialStringLength))
    {
        /* Save AAS index*/
        vgLmData_p->aasInfo.adAasIndex[ vgLmData_p->aasInfo.aasIndex] = sig_p->aasRecordNumber;

        vgPrintMissingParams (entity, vgLmData_p->missingParamCount);
        vgLmData_p->missingParamCount = 0;

        vgConvBcdToText(    sig_p->anrDialNum.typeOfNumber,
                            sig_p->anrDialNum.dialString,
                            sig_p->anrDialNum.dialStringLength,
                            conversionBuffer);

        vgPrintf(   entity,
                    (Char *)",\"%s\"",
                    conversionBuffer);

        vgPrintf (entity, (Char *)",%d",
        vgBcdNumberTypeToChar (sig_p->anrDialNum.typeOfNumber));
        if( getCommandId( entity) == VG_AT_GN_SCPBR)
        {
            vgReadScpbrAssociationData( entity, vgLmData_p);
        }
        else
        {
            /*read next association data */
            vgReadAssociationData (entity, vgLmData_p);
        }

    }
    else if ((sig_p->requestStatus == LM_REQ_OK_DATA_INVALID) ||
            (sig_p->requestStatus == LM_REQ_OK) ||
            /*print missing parameter if illegal operation or file not supported*/
            (sig_p->requestStatus == LM_REQ_ILLEGAL_OPERATION) ||
            (sig_p->requestStatus == LM_REQ_FILE_NOT_SUPPORTED))
    {
        vgLmData_p->aasInfo.adAasIndex[ vgLmData_p->aasInfo.aasIndex] = 0;

        /*additional number missing*/
        vgLmData_p->missingParamCount += 2; /*type of number + number, so increment by 2*/
        vgPrintMissingParams (entity, vgLmData_p->missingParamCount);
        vgLmData_p->missingParamCount = 0;
        if( getCommandId( entity) == VG_AT_GN_SCPBR)
        {
            vgReadScpbrAssociationData( entity, vgLmData_p);
        }
        else
        {
            /*read next association data */
            vgReadAssociationData (entity, vgLmData_p);
        }
    }
    else
    {
        /*something went wrong, flag an error*/
        setResultCode (entity, VG_CME_CPBR_CANNOT_READ_ANR);
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmReadSneCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_READ_SNE_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Read SNE request
 *-------------------------------------------------------------------------*/
void vgSigApexLmReadSneCnf (const SignalBuffer *signalBuffer,
                            const VgmuxChannelNumber entity)
{
  ApexLmReadSneCnf *sig_p = &signalBuffer->sig->apexLmReadSneCnf;
  GeneralContext_t *generalContext_p = ptrToGeneralContext (entity);
  VgLmData         *vgLmData_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  vgLmData_p         = &generalContext_p->vgLmData;

  /* Right now, it is only called as a part of AT+CPBR handler. Other cases
     may follow, in which case some sort of state mashine will be needed */

  if ((sig_p->requestStatus == LM_REQ_OK ) &&
      (sig_p->secondName.length))
  {
    vgPrintMissingParams (entity, vgLmData_p->missingParamCount);

    vgLmData_p->missingParamCount = 0;

    vgPutc (entity,',');
    vgPutc (entity, '\"');
    vgPutAlphaId (entity,
                   sig_p->secondName.data,
                    sig_p->secondName.length);
    vgPutc (entity, '\"');
    /*read next association data*/
    vgReadAssociationData (entity, vgLmData_p);

  }
  else if ((sig_p->requestStatus == LM_REQ_OK_DATA_INVALID) ||
            (sig_p->requestStatus == LM_REQ_OK) ||
            /*print missing parameter if illegal operation or file not supported*/
            (sig_p->requestStatus == LM_REQ_ILLEGAL_OPERATION) ||
            (sig_p->requestStatus == LM_REQ_FILE_NOT_SUPPORTED))
  {
    /*second name missing*/
    vgLmData_p->missingParamCount ++;
    vgPrintMissingParams (entity, vgLmData_p->missingParamCount);
    vgLmData_p->missingParamCount = 0;
    /*read next association data*/
    vgReadAssociationData (entity, vgLmData_p);
  }
  else
  {
    /*something went wrong, flag an error*/
    setResultCode (entity, VG_CME_CPBR_CANNOT_READ_SNE);
  }


}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmReadEmailCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_READ_EMAIL_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Read Email request
 *-------------------------------------------------------------------------*/
void vgSigApexLmReadEmailCnf (const SignalBuffer *signalBuffer,
                              const VgmuxChannelNumber entity)
{
    ApexLmReadEmailCnf *sig_p = &signalBuffer->sig->apexLmReadEmailCnf;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    /*  Right now, it is only called as a part of AT+CPBR handler. Other cases
    *   may follow, in which case some sort of state mashine will be needed */

    if( (sig_p->requestStatus == LM_REQ_OK) &&
        (sig_p->emailAddress.length ))
    {
        vgPrintMissingParams (entity, vgLmData_p->missingParamCount);

        vgLmData_p->missingParamCount = 0;

        vgPutc( entity,',');
        vgPutc( entity, '\"');
        vgPutAlphaId(   entity,
                        sig_p->emailAddress.data,
                        sig_p->emailAddress.length);
        vgPutc( entity, '\"');

        if( getCommandId( entity) == VG_AT_GN_SCPBR)
        {
            vgReadScpbrAssociationData( entity, vgLmData_p);
        }
        else
        {
            /*read next association data */
            vgReadAssociationData (entity, vgLmData_p);
        }
    }
    else if(    (sig_p->requestStatus == LM_REQ_OK) ||
                (sig_p->requestStatus == LM_REQ_OK_DATA_INVALID) ||
                /*print missing parameter if illegal operation or file not supported*/
                (sig_p->requestStatus == LM_REQ_ILLEGAL_OPERATION) ||
                (sig_p->requestStatus == LM_REQ_FILE_NOT_SUPPORTED))
    {
        /*email missing*/
        vgLmData_p->missingParamCount++;
        vgPrintMissingParams (entity, vgLmData_p->missingParamCount);
        vgLmData_p->missingParamCount = 0;

        if( getCommandId( entity) == VG_AT_GN_SCPBR)
        {
            vgReadScpbrAssociationData( entity, vgLmData_p);
        }
        else
        {
            /*read next association data */
            vgReadAssociationData (entity, vgLmData_p);
        }
    }
    else
    {
        /*something went wrong, flag an error*/
        setResultCode (entity, VG_CME_CPBR_CANNOT_READ_EMAIL);
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmListGasCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_LIST_GAS_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to List GAS request
 *-------------------------------------------------------------------------*/
void vgSigApexLmListGasCnf (const SignalBuffer *signalBuffer,
                            const VgmuxChannelNumber entity)
{
    ApexLmListGasCnf   *sig_p = &signalBuffer->sig->apexLmListGasCnf;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p;

    FatalCheck( generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    switch( getCommandId( entity) )
    {
        case VG_AT_GN_CPBW:
        {
            vgCpbwProcessLmListGasCnf( sig_p, vgLmData_p, entity);
        }
        break;

        case VG_AT_GN_MUPBGAS:
        {
            vgMupbgasProcessLmListGasCnf( sig_p, vgLmData_p, entity);
        }
        break;


        default:
        {
            DevParam( getCommandId( entity), 0, 0);
        }
        break;
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmWriteGasCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_WRITE_GAS_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Write GAS request
 *-------------------------------------------------------------------------*/
void vgSigApexLmWriteGasCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
    ApexLmWriteGasCnf  *sig_p = &signalBuffer->sig->apexLmWriteGasCnf;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p;

    FatalCheck( generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    switch( getCommandId( entity) )
    {
        case VG_AT_GN_CPBW:
        {
            vgCpbwProcessLmWriteGasCnf( sig_p, vgLmData_p, entity);
        }
        break;

        case VG_AT_GN_MUPBGAS:
        {
            vgMupbgasProcessLmWriteGasCnf( sig_p, vgLmData_p, entity);
        }
        break;

        default:
        {
            DevParam( getCommandId( entity), 0, 0);
        }
        break;
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmWriteGrpCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_WRITE_GRP_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Write Group request
 *-------------------------------------------------------------------------*/
void vgSigApexLmWriteGrpCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
    ApexLmWriteGrpCnf *sig_p = &signalBuffer->sig->apexLmWriteGrpCnf;
    GeneralContext_t  *generalContext_p = ptrToGeneralContext (entity);
    VgLmData          *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    if (sig_p->requestStatus == LM_REQ_OK)
    {
        vgUpdateAssociationData(entity, vgLmData_p);
    }
    else
    {
        /*something went wrong, flag an error*/
        vgLmData_p->deleteRes = VG_CME_CPBW_CANNOT_UPDATE_GROUP;
        setResultCode(  entity,
                        vgChManContinueAction (entity, SIG_APEX_LM_DELETE_DIALNUM_REQ));
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmWriteAnrCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_WRITE_ANR_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Write Anr Request
 *-------------------------------------------------------------------------*/
void vgSigApexLmWriteAnrCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
    ApexLmWriteAnrCnf *sig_p = &signalBuffer->sig->apexLmWriteAnrCnf;
    GeneralContext_t  *generalContext_p = ptrToGeneralContext (entity);
    VgLmData          *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    if (sig_p->requestStatus == LM_REQ_OK)
    {
        /*update next association data */
        vgUpdateAssociationData(entity, vgLmData_p);
    }
    else
    {
        /*something went wrong, flag an error*/
        vgLmData_p->deleteRes = VG_CME_CPBW_CANNOT_UPDATE_ANR;
        setResultCode(  entity,
                        vgChManContinueAction (entity, SIG_APEX_LM_DELETE_DIALNUM_REQ));
    }
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmWriteSneCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_WRITE_SNE_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Write Sne Request
 *-------------------------------------------------------------------------*/
void vgSigApexLmWriteSneCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
    ApexLmWriteSneCnf *sig_p = &signalBuffer->sig->apexLmWriteSneCnf;
    GeneralContext_t  *generalContext_p = ptrToGeneralContext (entity);
    VgLmData          *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    if (sig_p->requestStatus == LM_REQ_OK)
    {
        vgUpdateAssociationData (entity, vgLmData_p);
    }
    else
    {
        /*something went wrong, flag an error*/
        vgLmData_p->deleteRes = VG_CME_CPBW_CANNOT_UPDATE_SNE;
        setResultCode(  entity,
                        vgChManContinueAction (entity, SIG_APEX_LM_DELETE_DIALNUM_REQ));
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmWriteEmailCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_WRITE_EMAIL_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Write Email Request
 *-------------------------------------------------------------------------*/
void vgSigApexLmWriteEmailCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
    ApexLmWriteEmailCnf *sig_p = &signalBuffer->sig->apexLmWriteEmailCnf;
    GeneralContext_t  *generalContext_p = ptrToGeneralContext (entity);
    VgLmData          *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    if (sig_p->requestStatus == LM_REQ_OK)
    {
        /* Continue by dealing with extended parameters if need*/
        vgUpdateAssociationData (entity, vgLmData_p);
    }
    else if (sig_p->requestStatus == LM_REQ_INCOMPLETE_WRITE)
    {
        switch( getCommandId (entity))
        {
            case VG_AT_GN_SCPBW:
            case VG_AT_GN_CPBW:
                setResultCode(entity, VG_CME_CPBW_NO_FREE_EMAIL_NUMBER);
            break;

            default:
                FatalParam(getCommandId (entity), 0, 0);
            break;

        }
    }
    else
    {
        /*something went wrong, flag an error*/
        vgLmData_p->deleteRes = VG_CME_CPBW_CANNOT_UPDATE_EMAIL;
        setResultCode(  entity,
                        vgChManContinueAction (entity, SIG_APEX_LM_DELETE_DIALNUM_REQ));
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmDeleteGrpCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_DELETE_GRP_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Delete Grp Request
 *-------------------------------------------------------------------------*/
void vgSigApexLmDeleteGrpCnf (const SignalBuffer *signalBuffer,
                              const VgmuxChannelNumber entity)
{
    ApexLmDeleteGrpCnf *sig_p = &signalBuffer->sig->apexLmDeleteGrpCnf;

    if( (sig_p->requestStatus == LM_REQ_OK)||
        (sig_p->requestStatus == LM_REQ_OK_DATA_INVALID))
    {
        /* Not used anymore actually but still can be usefull*/
        DevFail( "Nothing to do here");
    }
    else
    {
        DevFail( "Nothing to do here");
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmDeleteAnrCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_DELETE_ANR_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Delete Anr Request
 *-------------------------------------------------------------------------*/
void vgSigApexLmDeleteAnrCnf (const SignalBuffer *signalBuffer,
                              const VgmuxChannelNumber entity)
{
    ApexLmDeleteAnrCnf *sig_p = &signalBuffer->sig->apexLmDeleteAnrCnf;

    if( (sig_p->requestStatus == LM_REQ_OK)||
        (sig_p->requestStatus == LM_REQ_OK_DATA_INVALID))
    {
        /* Not used anymore actually but still can be usefull*/
        DevFail( "Nothing to do here");
    }
    else
    {
        DevFail( "Nothing to do here");
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmDeleteSneCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_DELETE_SNE_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Delete Sne Request
 *-------------------------------------------------------------------------*/
void vgSigApexLmDeleteSneCnf (const SignalBuffer *signalBuffer,
                              const VgmuxChannelNumber entity)
{
    ApexLmDeleteSneCnf *sig_p = &signalBuffer->sig->apexLmDeleteSneCnf;

    if( (sig_p->requestStatus == LM_REQ_OK)||
        (sig_p->requestStatus == LM_REQ_OK_DATA_INVALID))
    {
        /* Not used anymore actually but still can be usefull*/
        DevFail( "Nothing to do here");
    }
    else
    {
        DevFail( "Nothing to do here");
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmDeleteEmailCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_DELETE_EMAIL_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Delete Email Request
 *-------------------------------------------------------------------------*/
void vgSigApexLmDeleteEmailCnf (const SignalBuffer *signalBuffer,
                                const VgmuxChannelNumber entity)
{
    ApexLmDeleteEmailCnf *sig_p = &signalBuffer->sig->apexLmDeleteEmailCnf;

    if( (sig_p->requestStatus == LM_REQ_OK)||
        (sig_p->requestStatus == LM_REQ_OK_DATA_INVALID))
    {
        /* Not used anymore actually but still can be usefull*/
        DevFail( "Nothing to do here");
    }
    else
    {
        DevFail( "Nothing to do here");
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmDeleteGasCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_DELETE_GAS_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Delete GAS request
 *-------------------------------------------------------------------------*/
void vgSigApexLmDeleteGasCnf(   const SignalBuffer *signalBuffer,
                                const VgmuxChannelNumber entity)
{
    ApexLmDeleteGasCnf *sig_p = &signalBuffer->sig->apexLmDeleteGasCnf;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p;

    FatalCheck( generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    switch( getCommandId( entity) )
    {
        case VG_AT_GN_MUPBGAS:
        {
            vgMupbgasProcessLmDeleteGasCnf( sig_p, vgLmData_p, entity);
        }
        break;

        default:
        {
            DevParam( getCommandId( entity), 0, 0);
        }
        break;
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmClearGasCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_CLEAR_GAS_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Clear GAS file request
 *-------------------------------------------------------------------------*/
void vgSigApexLmClearGasCnf(    const SignalBuffer *signalBuffer,
                                const VgmuxChannelNumber entity)
{
    ApexLmClearGasCnf  *sig_p = &signalBuffer->sig->apexLmClearGasCnf;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p;

    FatalCheck( generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    switch( getCommandId( entity) )
    {
        case VG_AT_GN_MUPBGAS:
        {
            vgMupbgasProcessLmClearGasCnf( sig_p, vgLmData_p, entity);
        }
        break;

        default:
        {
            DevParam( getCommandId( entity), 0, 0);
        }
        break;
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmGetSyncStatusCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_GET_SYNC_STATUS_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Get synchonization status Request
 *-------------------------------------------------------------------------*/
void vgSigApexLmGetSyncStatusCnf (const SignalBuffer *signalBuffer,
                                  const VgmuxChannelNumber entity)
{
    GeneralContext_t           *generalContext_p    = ptrToGeneralContext (entity);
    VgLmData                   *vgLmData_p;
    ApexLmGetSyncStatusCnf     *sig_p               = &signalBuffer->sig->apexLmGetSyncStatusCnf;
    Boolean                     printComma          = FALSE;
    const VgLmInfo             *lmInfo_p            = getVgLmInfoRec ();

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p   = &generalContext_p->vgLmData;

    switch (getCommandId (entity))
    {
        case VG_AT_GN_MUPBSYNC:
        {
            if (sig_p->requestStatus == LM_REQ_OK)
            {
                /*Print the synchronisation information*/
                vgGnMUPBSYNCPrintPbResults( entity, sig_p);
            }
            else
            {
                setResultCode (entity, RESULT_CODE_ERROR);
            }
        }
        break;

        default:
        {
            /* Invalid activation of vgSigApexLmGetSyncStatusCnf! */
            FatalParam(entity, getCommandId (entity), 0);
        }
        break;
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmReadRecordUidCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_READ_RECORD_UID_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to an read UID request
 *-------------------------------------------------------------------------*/
void vgSigApexLmReadRecordUidCnf(   const SignalBuffer *signalBuffer,
                                    const VgmuxChannelNumber entity)
{
    ApexLmReadRecordUidCnf *sig_p = &signalBuffer->sig->apexLmReadRecordUidCnf;
    GeneralContext_t       *generalContext_p = ptrToGeneralContext (entity);
    VgLmData               *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p   = &generalContext_p->vgLmData;

    switch (getCommandId (entity))
    {
        case VG_AT_GN_MUPBSYNC:
            {
                switch( sig_p->requestStatus)
                {
                    case LM_REQ_OK:
                        {
                            /* Print the record*/
                            vgGnMUPBSYNCPrintUid( entity, sig_p);

                            if( vgLmData_p->vgMupbsyncContext.uidReadMode == LM_READ_ABSOLUTE)
                            {
                                setResultCode (entity, RESULT_CODE_OK);
                            }
                            else
                            {
                                /* Ask for next record*/
                                generalContext_p->vgLmData.vgMupbsyncContext.uidReadMode = LM_READ_NEXT;
                                generalContext_p->vgLmData.vgMupbsyncContext.uidIndex    = sig_p->recordNumber;
                                vgChManContinueAction (entity, SIG_APEX_LM_READ_RECORD_UID_REQ);
                            }
                        }
                        break;

                    case LM_REQ_OK_DATA_INVALID:
                    case LM_REQ_RECORD_NOT_FOUND:
                        {
                            if( vgLmData_p->vgMupbsyncContext.uidReadMode == LM_READ_ABSOLUTE)
                            {
                                /* User at request a precise record but it don't exists*/
                                setResultCode (entity, vgGetLmCmeErrorCode (LM_REQ_RECORD_NOT_FOUND));
                            }
                            else
                            {
                                /* All avalaible records has been read*/
                                setResultCode (entity, RESULT_CODE_OK);
                            }
                        }
                        break;

                    case LM_REQ_SIM_ERROR:
                    default:
                        {
                            setResultCode (entity, vgGetLmCmeErrorCode (sig_p->requestStatus));
                        }
                        break;
                }
            }
            break;

        default:
            {
                /* Invalid activation of vgSigApexLmReadRecordUidCnf! */
                FatalParam(entity, getCommandId (entity), 0);
            }
            break;
    }
}

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmReadyIndLocal
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_READY_IND
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: ABLM is now ready, initialise local data
 *----------------------------------------------------------------------------*/
void vgSigApexLmReadyIndLocal(  const SignalBuffer *signalBuffer,
                                const VgmuxChannelNumber entity)
{
    ApexLmReadyInd      *sig_p = &signalBuffer->sig->apexLmReadyInd;
    GeneralContext_t    *generalContext_p;
    VgmuxChannelNumber   i;
    const VgLmInfo      *lmInfo = getVgLmInfoRec();
    Int8                 index;
    VgLmData           *vgLmData;

    /* Look for SM phonebook*/
    for(    index = 0;
            (   (index < NUMBER_OF_PHONE_BOOKS) &&
                (vgStrCmp(lmInfo[index].vgPhoneStore, "SM") != 0) );
            index++)
    {
    }
    FatalAssert( index < NUMBER_OF_PHONE_BOOKS );

    /* Reset selected phonebook to default SM for all entities*/
    /* if we don't want such thing, we need to manage cases like selected phonebook
     * are LNR and user insert a USIM with ICI files*/
    for ( i = 0; i < CI_MAX_ENTITIES; i++)
    {
        if(     isEntityActive (i) &&
                (generalContext_p = ptrToGeneralContext(i)) != PNULL)
        {
            vgLmData = &generalContext_p->vgLmData;

            vgLmData->phoneBook                 = lmInfo[index].file;
            vgLmData->phoneBookIndex            = index;
            vgLmData->vgCpbsData.phoneBook      = lmInfo[index].file;
            vgLmData->vgCpbsData.phoneBookIndex = index;
        }
    }
}

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmRecordChangedInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_RECORD_CHANGED_IND
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: A sim record has been modified
 *----------------------------------------------------------------------------*/
void vgSigApexLmRecordChangedInd(   const SignalBuffer *signalBuffer,
                                    const VgmuxChannelNumber entity)
{
    PARAMETER_NOT_USED( entity);

    /* Do nothing */
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmReadAasCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_READ_AAS_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Read additional number Alpha String request
 *-------------------------------------------------------------------------*/
void vgSigApexLmReadAasCnf (const SignalBuffer *signalBuffer,
                            const VgmuxChannelNumber entity)
{
    ApexLmReadAasCnf   *sig_p             = &signalBuffer->sig->apexLmReadAasCnf;
    GeneralContext_t   *generalContext_p  = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    switch( getCommandId( entity) )
    {
        case VG_AT_GN_MUPBAAS:
        {
            switch(vgLmData_p->vgMupbaasContext.mupbaasMode)
            {
                case VG_MUPBAAS_MODE_READ:
                {
                    if( (sig_p->requestStatus == LM_REQ_OK_DATA_INVALID) ||
                        (sig_p->requestStatus == LM_REQ_OK))
                    {
                        if( sig_p->requestStatus == LM_REQ_OK)
                        {
                            vgPutNewLine( entity);
                            vgPrintf( entity, (Char *)"*MUPBAAS: %d,\"", sig_p->aasRecordNumber);
                            vgPutAlphaId(   entity,
                                            sig_p->aasRecord.data,
                                            sig_p->aasRecord.length);
                            vgPutc( entity, '\"');
                        }
                        vgPutNewLine (entity);
                        setResultCode (entity, RESULT_CODE_OK);
                    }
                    else
                    {
                        /*something went wrong, flag an error*/
                        setResultCode (entity, RESULT_CODE_ERROR);
                    }
                }
                break;

                default:
                {
                    FatalParam( vgLmData_p->vgMupbaasContext.mupbaasMode, 0, 0);
                }
                break;
            }
        }
        break;

        default:
        {
            FatalParam( getCommandId( entity), 0, 0);
        }
        break;
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmWriteAasCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_WRITE_AAS_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Write AAS request
 *-------------------------------------------------------------------------*/
void vgSigApexLmWriteAasCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
    ApexLmWriteAasCnf  *sig_p = &signalBuffer->sig->apexLmWriteAasCnf;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p;

    FatalCheck( generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    switch( getCommandId( entity) )
    {
        case VG_AT_GN_MUPBAAS:
        {
            switch(vgLmData_p->vgMupbaasContext.mupbaasMode)
            {
                case VG_MUPBAAS_MODE_WRITE:
                {
                    if (sig_p->requestStatus == LM_REQ_OK)
                    {
                        setResultCode (entity, RESULT_CODE_OK);
                    }
                    else
                    {
                        /*something went wrong, flag an error*/
                        setResultCode (entity, RESULT_CODE_ERROR);
                    }
                }
                break;

                default:
                {
                    FatalParam( vgLmData_p->vgMupbaasContext.mupbaasMode, 0, 0);
                }
                break;
            }
        }
        break;

        default:
        {
            FatalParam( getCommandId( entity), 0, 0);
        }
        break;
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmDeleteAasCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_DELETE_AAS_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Delete AAS request
 *-------------------------------------------------------------------------*/
void vgSigApexLmDeleteAasCnf(   const SignalBuffer *signalBuffer,
                                const VgmuxChannelNumber entity)
{
    ApexLmDeleteAasCnf *sig_p = &signalBuffer->sig->apexLmDeleteAasCnf;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p;

    FatalCheck( generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    switch( getCommandId( entity) )
    {
        case VG_AT_GN_MUPBAAS:
        {
            switch(vgLmData_p->vgMupbaasContext.mupbaasMode)
            {
                case VG_MUPBAAS_MODE_WRITE: /* Means it is a indexed delete operation*/
                {
                    if (sig_p->requestStatus == LM_REQ_OK)
                    {
                        setResultCode (entity, RESULT_CODE_OK);
                    }
                    else
                    {
                        /*something went wrong, flag an error*/
                        setResultCode (entity, RESULT_CODE_ERROR);
                    }
                }
                break;

                default:
                {
                    FatalParam( vgLmData_p->vgMupbaasContext.mupbaasMode, 0, 0);
                }
                break;
            }
        }
        break;

        default:
        {
            FatalParam( getCommandId( entity), 0, 0);
        }
        break;
    }
}
/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmListAasCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_LIST_AAS_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to List AAS request
 *-------------------------------------------------------------------------*/
void vgSigApexLmListAasCnf (const SignalBuffer *signalBuffer,
                            const VgmuxChannelNumber entity)
{
    ApexLmListAasCnf   *sig_p = &signalBuffer->sig->apexLmListAasCnf;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p;
    Int8                i;

    FatalCheck( generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    switch( getCommandId( entity) )
    {
        case VG_AT_GN_MUPBAAS:
        {
            switch(vgLmData_p->vgMupbaasContext.mupbaasMode)
            {
                case VG_MUPBAAS_MODE_LIST:
                {
                    if (sig_p->requestStatus == LM_REQ_OK)
                    {
                        for (i = 0; i < sig_p->listSize; i++)
                        {
                            vgLmData_p->startRecord = sig_p->aasRecordNumber[i] + 1;

                            /* Print the AAS*/
                            vgPutNewLine( entity);
                            vgPrintf( entity, (Char *)"*MUPBAAS: %d,\"", sig_p->aasRecordNumber[i]);
                            vgPutAlphaId(   entity,
                                            sig_p->aasRecord[i].data,
                                            sig_p->aasRecord[i].length);
                            vgPutc( entity, '\"');
                        }
                        vgPutNewLine (entity);

                        if( sig_p->moreValidEntries == TRUE)
                        {
                            setResultCode (entity, vgChManContinueAction (entity, SIG_APEX_LM_LIST_AAS_REQ));
                        }
                        else
                        {
                            setResultCode (entity, RESULT_CODE_OK);
                        }
                    }
                    else
                    {
                        /*other errors*/
                        setResultCode (entity, RESULT_CODE_ERROR);
                    }
                }
                break;

                default:
                {
                    FatalParam( vgLmData_p->vgMupbaasContext.mupbaasMode, 0, 0);
                }
                break;
            }
        }
        break;

        default:
        {
            FatalParam( getCommandId( entity), 0, 0);
        }
        break;
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexLmClearAasCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_LM_CLEAR_AAS_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to Clear AAS file request
 *-------------------------------------------------------------------------*/
void vgSigApexLmClearAasCnf(    const SignalBuffer *signalBuffer,
                                const VgmuxChannelNumber entity)
{
    ApexLmClearAasCnf  *sig_p = &signalBuffer->sig->apexLmClearAasCnf;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p;

    FatalCheck( generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    switch( getCommandId( entity) )
    {
        case VG_AT_GN_MUPBAAS:
        {
            switch(vgLmData_p->vgMupbaasContext.mupbaasMode)
            {
                case VG_MUPBAAS_MODE_DELETE_ALL:
                {
                    if (sig_p->requestStatus == LM_REQ_OK)
                    {
                        setResultCode (entity, RESULT_CODE_OK);
                    }
                    else
                    {
                        /*something went wrong, flag an error*/
                        setResultCode (entity, RESULT_CODE_ERROR);
                    }
                }
                break;

                default:
                {
                    FatalParam( vgLmData_p->vgMupbaasContext.mupbaasMode, 0, 0);
                }
                break;
            }
        }
        break;

        default:
        {
            FatalParam( getCommandId( entity), 0, 0);
        }
        break;
    }
}

#endif /* FEA_PHONEBOOK */

/*************************************************************************
*
* Function:     vgSigApexMmWriteMobileIdReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/
void vgSigApexMmWriteMobileIdCnf( const SignalBuffer *signalBuffer,
                                    const VgmuxChannelNumber entity)
{
    ApexMmWriteMobileIdCnf       *sig_p;

    sig_p = &signalBuffer->sig->apexMmWriteMobileIdCnf; 

    switch( getCommandId(entity) )
    {
    case VG_AT_GN_MCGSN:
        {
            if (MM_REQ_OK == sig_p->requestStatus)
            {
                setResultCode (entity, RESULT_CODE_OK);
            }
            else
            {
                setResultCode (entity, RESULT_CODE_ERROR);
            }
        }
        break;

    default:
        {
            FatalParam( getCommandId( entity), 0, 0);
        }
        break;
    }
}

/*************************************************************************
*
* Function:     vgSigApexMmReadMobileIdCnf
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/
void vgSigApexMmReadMobileIdCnf( const SignalBuffer *signalBuffer,
                                    const VgmuxChannelNumber entity)
{
    ApexMmReadMobileIdCnf       *sig_p;
    Int8    index;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);

    sig_p = &signalBuffer->sig->apexMmReadMobileIdCnf; 

    switch( getCommandId(entity) )
    {
    case VG_AT_GN_GSN:
        {
          vgPutNewLine (entity);
        
          for (index = 0;
                index < (sig_p->mobileID.imei.digitArraySize);
                 index++)
          {
            vgPrintf (entity, (const Char*)"%d",
                       sig_p->mobileID.imei.digit[index]);
          }
          vgPutNewLine (entity);
        
          setResultCode (entity, RESULT_CODE_OK);
        }

        break;
    case VG_AT_GN_CGSN:
        {
          vgPutNewLine (entity);
        
          switch (generalContext_p->cgsnSnt)
          {
            case VG_CGSN_SNT_SN:
            {
              for (index = 0;
                    index < (sig_p->mobileID.SnId.digitArraySize);
                     index++)
              {
                vgPrintf (entity, (const Char*)"%c",
                           sig_p->mobileID.SnId.digit[index]);
              }
              break;
            }
            case VG_CGSN_SNT_IMEI:
            {
              vgPrintf(entity, (const Char*)"+CGSN: ");
              for (index = 0;
                    index < (sig_p->mobileID.imei.digitArraySize);
                     index++)
              {
                vgPrintf (entity, (const Char*)"%d",
                           sig_p->mobileID.imei.digit[index]);
              }
              break;
            }
            case VG_CGSN_SNT_IMEISV:
            {
              vgPrintf(entity, (const Char*)"+CGSN: ");
              for (index = 0;
                    index < (sig_p->mobileID.imei.digitArraySize-1);
                     index++)
              {
                vgPrintf (entity, (const Char*)"%d",
                           sig_p->mobileID.imei.digit[index]);
              }
              for (index = 0;
                  index < SVN_LENGTH;
                  index++)
              {
                vgPrintf (entity, (const Char*)"%d",
                           sig_p->mobileID.svn.digit[index]);
              }    
              break;
            }
            case VG_CGSN_SNT_SVN:
            {
              vgPrintf(entity, (const Char*)"+CGSN: ");
              for (index = 0;
                  index < SVN_LENGTH;
                  index++)
              {
                vgPrintf (entity, (const Char*)"%d",
                           sig_p->mobileID.svn.digit[index]);
              }    
              break;
            }
            default:
            {
              FatalParam(entity, generalContext_p->cgsnSnt, 0);
              break;
            }
          }
          
          vgPutNewLine (entity);
          setResultCode (entity, RESULT_CODE_OK);
        break;
        }  
    default:
        {
            FatalParam( getCommandId( entity), 0, 0);
        }
        break;
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigN1CdEnterCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_N1CD_ENTER_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to *MCALDEV command.
 *-------------------------------------------------------------------------*/
void vgSigN1CdEnterCnf                  (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity)
{
  /* Ignore signal content */
  switch( getCommandId( entity) )
  {
      case VG_AT_GN_MCALDEV:
      {
          setResultCode (entity, RESULT_CODE_OK);
      }
      break;
  
      default:
      {
          FatalParam( getCommandId( entity), 0, 0);
      }
      break;
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigN1CdExitCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_N1CD_EXIT_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to *MCALDEV command.
 *-------------------------------------------------------------------------*/
void vgSigN1CdExitCnf                  (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity)
{
  /* Ignore signal content */
  switch( getCommandId( entity) )
  {
      case VG_AT_GN_MCALDEV:
      {
          setResultCode (entity, RESULT_CODE_OK);
      }
      break;
  
      default:
      {
          FatalParam( getCommandId( entity), 0, 0);
      }
      break;
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigN1CdRfTestCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_N1CD_NRF_TEST_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to *MCAL command.
 *-------------------------------------------------------------------------*/
void vgSigN1CdRfTestCnf                     (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity)
{
  N1CdRfTestCnf           *sig_p                    = &signalBuffer->sig->n1CdRfTestCnf;
  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  VgMcalContext           *mcalContext_p            = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMcalContext);
  Int16                   str_length;

  switch( getCommandId( entity) )
  {
      case VG_AT_GN_MCAL:
      {
        /* Make sure length > 0 & <= MAX_MCAL_DATA_SIZE_BYTES - otherwise don't print anything */
        if ((sig_p->length > 0) && (sig_p->length <= MAX_MCAL_DATA_SIZE_BYTES) && (sig_p->length <= MAX_N1CD_RF_TEST_CNF_PARAM_LENGTH))
        {
          str_length = vgMapHexToTE(mcalContext_p->data_str,
                            MAX_MCAL_DATA_STR_LEN,
                            sig_p->param,
                            sig_p->length,
                            VG_AT_CSCS_HEX);

          /* Display the text now */
          vgPutNewLine (entity);
          vgPrintf (entity,
                   (const Char *)"*MCAL: \"NRF\",");

          vgPrintf(entity, (const Char *)"%d,%d,%d,\"",
                   sig_p->token,
                   sig_p->cmd,
                   sig_p->length);

          if (sig_p->length > 1)
          {
            vgPutc(entity, MCAL_LITTLE_ENDIAN_CHAR);
          }

          /* Add the tailing quite then output the string */
          mcalContext_p->data_str[str_length] = '\"';
          mcalContext_p->data_str[str_length+1] = 0;
          vgPuts(entity,(const Char *)mcalContext_p->data_str);
          vgFlushBuffer(entity);
 
          setResultCode (entity, RESULT_CODE_OK);
        }
        else
        {
          setResultCode (entity, RESULT_CODE_ERROR);
        }
      }
      break;
  
      default:
      {
          FatalParam( getCommandId( entity), 0, 0);
      }
      break;
  }

}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigN1CdIdcTestCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_N1CD_IDC_TEST_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Response to +IDCTEST command.
 *-------------------------------------------------------------------------*/
void vgSigN1CdIdcTestCnf                     (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity)
{
  N1CdIdcTestCnf          *sig_p                    = &signalBuffer->sig->n1CdIdcTestCnf;
  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  VgMcalContext           *mcalContext_p            = &(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion.vgMcalContext);
  Int16                   str_length;

  switch( getCommandId( entity) )
  {
      case VG_AT_GN_IDCTEST:
      {
        /* Make sure length > 0 & <= MAX_MCAL_DATA_SIZE_BYTES - otherwise don't print anything */
        if ((sig_p->length > 0) && (sig_p->length <= MAX_MCAL_DATA_SIZE_BYTES) && (sig_p->length <= MAX_N1CD_IDC_TEST_CNF_PARAM_LENGTH))
        {
          str_length = vgMapHexToTE(mcalContext_p->data_str,
                            MAX_MCAL_DATA_STR_LEN,
                            sig_p->param,
                            sig_p->length,
                            VG_AT_CSCS_HEX);

          /* Display the text now */
          vgPutNewLine (entity);
          vgPrintf (entity,
                   (const Char *)"+IDCTEST: ");

          vgPrintf(entity, (const Char *)"%d,%d,%d,\"",
                   sig_p->token,
                   sig_p->cmd,
                   sig_p->length);

          /* Add the tailing quite then output the string */
          mcalContext_p->data_str[str_length] = '\"';
          mcalContext_p->data_str[str_length+1] = 0;
          vgPuts(entity,(const Char *)mcalContext_p->data_str);
          vgFlushBuffer(entity);
 
          setResultCode (entity, RESULT_CODE_OK);
        }
        else
        {
          setResultCode (entity, RESULT_CODE_ERROR);
        }
      }
      break;
  
      default:
      {
          FatalParam( getCommandId( entity), 0, 0);
      }
      break;
  }

}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSendNextMnvmgetDataItemToMux
 *
 * Parameters:  
 *              VgmuxChannelNumber - entity which got the ciMuxAtDataCnf signal
 *
 * Returns:     Nothing
 *
 * Description: Send next data item to MUX for the AT*MNVMGET command.
 *-------------------------------------------------------------------------*/
void vgSendNextMnvmgetDataItemToMux   (const VgmuxChannelNumber entity)
{
  GeneralGenericContext_t *generalGenericContext_p  = ptrToGeneralGenericContext();
  VgMnvmMcalContext       *vgNvmMcalContext_p = &(generalGenericContext_p->vgMnvmMcalContext);

#if defined (MTK_NVDM_MODEM_ENABLE) 
  VgNvmAreaInfo                 areaInfo;
  nvdm_modem_data_item_info_t   *current_info_list_p = PNULL;
  Int32                         counter;
#endif

  switch( getCommandId( entity) )
  {
      case VG_AT_GN_MNVMGET:
      {
#if defined (MTK_NVDM_MODEM_ENABLE) 
        /* First check we have the right channel and we have something to send */
        if ((vgNvmMcalContext_p->totalDataItemsToSend > 0) &&
            ((vgNvmMcalContext_p->info_list_p_normal != PNULL)||(vgNvmMcalContext_p->info_list_p_protected != PNULL))  &&
            (vgNvmMcalContext_p->currentNvmChannel == entity))
        { 
          if ((vgNvmMcalContext_p->normalDataItems >0)  &&(vgNvmMcalContext_p->nextDataItemNumberToSend < vgNvmMcalContext_p->normalDataItems))
          {
            current_info_list_p = vgNvmMcalContext_p->info_list_p_normal;
            counter = vgNvmMcalContext_p->nextDataItemNumberToSend;
          }
          else if (vgNvmMcalContext_p->protectedDataItems >0) 
          {
            current_info_list_p = vgNvmMcalContext_p->info_list_p_protected; 
            counter = vgNvmMcalContext_p->nextDataItemNumberToSend - (vgNvmMcalContext_p->normalDataItems);
          }  

          if (current_info_list_p != PNULL)
          {
            switch (current_info_list_p[counter].area)
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
                FatalParam(entity, current_info_list_p[counter].area, 0);
                break;
              }
            }
          
            vgPutNewLine (entity);
            vgPrintf (entity,
                   (const Char *)"*MNVMGET: %d,\"%s\",\"%s\"",areaInfo,
                    current_info_list_p[counter].group_name,
                    current_info_list_p[counter].data_item_name);
            vgFlushBuffer(entity);

            vgNvmMcalContext_p->nextDataItemNumberToSend++;
            if (vgNvmMcalContext_p->nextDataItemNumberToSend >= vgNvmMcalContext_p->totalDataItemsToSend)
            {
              /* This was the last one - so finish off */
              vgPutNewLine (entity);

              setResultCode (entity, RESULT_CODE_OK);

              /* Clear everything now */
              if (generalGenericContext_p->vgMnvmMcalContext.info_list_p_normal != PNULL)
              {
                  KiFreeMemory ((void **) &generalGenericContext_p->vgMnvmMcalContext.info_list_p_normal);
              }
              if (generalGenericContext_p->vgMnvmMcalContext.info_list_p_protected != PNULL) 
              {
                  KiFreeMemory ((void **) &generalGenericContext_p->vgMnvmMcalContext.info_list_p_protected);
              }
              generalGenericContext_p->vgMnvmMcalContext.info_list_p_normal = PNULL;
              generalGenericContext_p->vgMnvmMcalContext.info_list_p_protected = PNULL;
              generalGenericContext_p->vgMnvmMcalContext.nextDataItemNumberToSend = 0;
              generalGenericContext_p->vgMnvmMcalContext.totalDataItemsToSend = 0;
              generalGenericContext_p->vgMnvmMcalContext.normalDataItems = 0;
              generalGenericContext_p->vgMnvmMcalContext.protectedDataItems = 0;
              generalGenericContext_p->vgMnvmMcalContext.currentNvmChannel = VGMUX_CHANNEL_INVALID;
            }
          }
          else
          {
             FatalAssert("MNVMGET: no list to read from "); 
          }
        
       }
#endif
      break;
     }
 
      default:
      {
        /* Do nothing */
      }
      break;
  }   
}


/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */


