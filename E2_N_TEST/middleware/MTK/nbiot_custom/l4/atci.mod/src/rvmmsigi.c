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
 * Incoming signal handlers for the Mobility Management Sub-System.
 **************************************************************************/

#define MODULE_NAME "RVMMSIGI"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <gkitimer.h>

#include <stdio.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvmmsigi.h>
#include <rvmmsigo.h>
#include <rvmmut.h>
#include <rvcrhand.h>
#include <rvchman.h>
#include <rvgput.h>
#include <rvgpuns.h>
#include <rvomtime.h>
#include <dmrtc_sig.h>
#include <rvcimxut.h>
#include <uttime.h>
#include <string.h>   /* for strcat() */
#include <utbitfnc.h>
#include <rvslut.h>
#include <gkisig.h>
#include <rvpfsigo.h>
#include <rvsleep.h>
#include <hal_rtc_internal.h>
#include <abrpm_sig.h>
/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
    ApexMmReadPlmnSelCnf      apexMmReadPlmnSelCnf;
    ApexMmWritePlmnSelCnf     apexMmWritePlmnSelCnf;
    ApexMmPlmnListCnf         apexMmPlmnListCnf;
    ApexMmAbortPlmnListCnf    apexMmAbortPlmnListCnf;
    ApexMmPlmnSelectCnf       apexMmPlmnSelectCnf;
    ApexMmDeregisterCnf       apexMmDeregisterCnf;
    ApexEmPlmnTestCnf         apexEmPlmnTestCnf;
    ApexMmRssiInd             apexMmRssiInd;
    ApexMmBandInd             apexMmBandInd;
#if defined (ENABLE_AT_ENG_MODE)
    ApexMmCipherInd           apexMmCipherInd;
#endif /* (ENABLE_AT_ENG_MODE) */
    ApexMmNetworkInfoInd      apexMmNetworkInfoInd;
    ApexMmNetworkStateInd     apexMmNetworkStateInd;
    ApexMmReadBandModeCnf     apexMmReadBandModeCnf;
    ApexMmWritePwonOptionsCnf apexMmWritePwonOptionsCnf;
    ApexMmReadPwonOptionsCnf  apexMmReadPwonOptionsCnf;
    ApexMmLockArfcnCnf        apexMmLockArfcnCnf;

    DmRtcSetTimeZoneCnf       dmRtcSetTimeZoneCnf;
    DmRtcReadTimeZoneCnf      dmRtcReadTimeZoneCnf;
#if defined(UPGRADE_MTNET)
    ApexMmSuspendCnf          apexMmSuspendCnf;
    ApexMmResumeCnf           apexMmResumeCnf;
#endif
    ApexMmSetEdrxCnf             apexMmSetEdrxCnf;
    ApexMmReadEdrxCnf            apexMmReadEdrxCnf;
    ApexWriteIotOptCfgCnf        apexWriteIotOptCfgCnf;
    ApexReadIotOptCfgCnf         apexReadIotOptCfgCnf;
    ApexWritePsmConfCnf          apexWritePsmConfCnf;
    ApexReadPsmConfCnf           apexReadPsmConfCnf;
    ApexMmPsmStatusInd           apexMmPsmStatusInd;
    ApexMmOosaStatusInd          apexMmOosaStatusInd;
    ApexMmWriteAttachPdnCfgCnf   apexMmWriteAttachPdnCfgCnf;
    ApexMmReadAttachPdnCfgCnf    apexMmReadAttachPdnCfgCnf;
#if defined (FEA_NFM)
    ApexReadNfmCnf            apexReadNfmCnf;
    ApexWriteNfmCnf           apexWriteNfmCnf;
    ApexReadNfmConfCnf        apexReadNfmConfCnf;
    ApexWriteNfmConfCnf       apexWriteNfmConfCnf;
    ApexReadNfmStatusCnf      apexReadNfmStatusCnf;
#endif
    ApexMmCsconInd               apexMmCsconInd;
    ApexMmUeStatsCnf             apexMmUeStatsCnf;    
    ApexMmLocCellInfoCnf         apexMmLocCellInfoCnf;
    ApexMmSearchBandListCnf      apexMmSearchBandListCnf;
#if defined (FEA_RPM)
    ApexRpmReadInfoCnf        apexRpmReadInfoCnf;
    ApexRpmInd                apexRpmInd;
#endif
    ApexMmDisableHplmnSearchCnf  apexMmDisableHplmnSearchCnf;

};

/***************************************************************************
* Local Function Prototypes
***************************************************************************/

#define SEMI_OCTET_INT8_DEC(x) ((((x) & 0x0F) * 10) + (((x) >> 4) & 0x0F))
#if defined(HAL_RTC_MODULE_ENABLED)
static void convertToMltsTime(  RtcDateAndTime             *rtcTime_p,
                                      hal_rtc_time_t             *rtc_Time_p,
                                const ApexMmNetworkInfoInd *sig_p);
#else
static void convertToMltsTime(  RtcDateAndTime             *rtcTime_p,
                                const ApexMmNetworkInfoInd *sig_p);
#endif

static Boolean findOperatorMatch (PlmnId *selectedPlmn,
                                  const VgOpFormat format,
                                  const PlmnArrayStruct *plmnArray
                                  );

static void displayOperatorInfo ( const VgmuxChannelNumber entity,
                                  const FullPlmnDetails    *plmnDetails);

/***************************************************************************
* Type Definitions
***************************************************************************/

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/
/*----------------------------------------------------------------------------
 *
 * Function:    convertToMltsTime
 *
 * Parameters:  rtcTime_p (Out) - Time resulting after the convertion
 *              sig_p (In)      - received signal containing the time to convert
 *
 * Returns:     Nothing
 *
 * Description: Convert a network time into time waited by *MLTS command
 *
 *----------------------------------------------------------------------------*/
#if defined(HAL_RTC_MODULE_ENABLED)
static void convertToMltsTime(  RtcDateAndTime             *rtcTime_p,
                                      hal_rtc_time_t             *rtc_Time_p,
                                const ApexMmNetworkInfoInd *sig_p)
#else
static void convertToMltsTime(  RtcDateAndTime             *rtcTime_p,
                                const ApexMmNetworkInfoInd *sig_p)   
#endif
{
    FatalAssert( rtcTime_p != PNULL);
    FatalAssert( sig_p != PNULL);

    /* Fill RtcDateAndTime structure*/
    rtcTime_p->date.day     = SEMI_OCTET_INT8_DEC(sig_p->uniTimeAndLocTimeZone.day);
    rtcTime_p->date.month   = SEMI_OCTET_INT8_DEC(sig_p->uniTimeAndLocTimeZone.month);
    rtcTime_p->date.year    = SEMI_OCTET_INT8_DEC(sig_p->uniTimeAndLocTimeZone.year);
    rtcTime_p->time.hours   = SEMI_OCTET_INT8_DEC(sig_p->uniTimeAndLocTimeZone.hour);
    rtcTime_p->time.minutes = SEMI_OCTET_INT8_DEC(sig_p->uniTimeAndLocTimeZone.minute);
    rtcTime_p->time.seconds  = SEMI_OCTET_INT8_DEC(sig_p->uniTimeAndLocTimeZone.second);
    /* remove sign bit from time zone octet and change unit to minutes*/
    rtcTime_p->timeZone.offset.hours   = 0;
    rtcTime_p->timeZone.offset.minutes = SEMI_OCTET_INT8_DEC(0xF7 & sig_p->uniTimeAndLocTimeZone.locTimeZone);
    rtcTime_p->timeZone.offset.seconds = 0;
#if defined(HAL_RTC_MODULE_ENABLED)

    rtc_Time_p->rtc_year = rtcTime_p->date.year;
    rtc_Time_p->rtc_mon  = rtcTime_p->date.month;
    rtc_Time_p->rtc_day  = rtcTime_p->date.day;
    rtc_Time_p->rtc_hour = rtcTime_p->time.hours;
    rtc_Time_p->rtc_min= rtcTime_p->time.minutes;
    rtc_Time_p->rtc_sec= rtcTime_p->time.seconds;
    rtc_Time_p->rtc_week = 0;
#endif
    /*  check sign bit of time zone and do the timezone adjustment. */
    if( ((sig_p->uniTimeAndLocTimeZone.locTimeZone & 0x08) == 0) ||
        ((sig_p->uniTimeAndLocTimeZone.locTimeZone & 0xF7) == 0) )
    {
        rtcTime_p->timeZone.format  = RTC_DISP_FORMAT_POS;
    }
    else
    {
        rtcTime_p->timeZone.format  = RTC_DISP_FORMAT_NEG;
    }

    /* Day saving Time*/
    if( sig_p->networkDaylightSavingTimePresent == TRUE)
    {
        switch( sig_p->networkDaylightSavingTime)
        {
            case PLUS_ONE_HOUR_ADJUSTMENT:
            {
                rtcTime_p->daylightSaving = RTC_DAYLIGHT_SAVING_ONE_HR;
            }
            break;

            case PLUS_TWO_HOURS_ADJUSTMENT:
            {
                rtcTime_p->daylightSaving = RTC_DAYLIGHT_SAVING_TWO_HRS;
            }
            break;

            default:
            {
                rtcTime_p->daylightSaving = RTC_DAYLIGHT_SAVING_NONE;
                /* Nothing more to do*/
            }
            break;
        }
    }
    else
    {
        rtcTime_p->daylightSaving = RTC_DAYLIGHT_SAVING_NONE;
    }

}

static void GetNameworkNameString (NetworkName networkName, Char *pDst)
{
    Int8             i;
    Int16            dataLength = 0;
    Char             pTemp[MAX_NETWORK_NAME_LENGTH_IN_CBS];

    *pDst++ = ',';
    *pDst++ = '\"';
    if(networkName.networkNameCodingScheme == NETWORK_NAME_SMS_CB_CODED)
    {
        dataLength = (Int8)UtDecode7BitPackedData(pTemp,
                                            networkName.networkName,
                                            MAX_NETWORK_NAME_LENGTH_IN_CBS,
                                            networkName.networkNameLength);
        memcpy(pDst, pTemp, dataLength );
        pDst += dataLength;
        *pDst++ = '\"';

         /* <dcs>  */
        *pDst++ = ',';
        *pDst++ = '0';
    }
    else if(networkName.networkNameCodingScheme == NETWORK_NAME_UCS2_CODED)
    {
        for(i=0; i<networkName.networkNameLength; i++)
        {
            vgOp8BitHex(networkName.networkName[i], pDst);
            pDst +=2;
        }
        *pDst++ = '\"';

         /* <dcs>  */
        *pDst++ = ',';
        *pDst++ = '1';
    }
    *pDst = NULL_CHAR;
}
/*----------------------------------------------------------------------------
 *
 * Function:    findOperatorMatch
 *
 * Parameters:  PlmnId          - details of matched operator
 *              VgOpFormat      - search format
 *              PlmnArrayStruct - available operator information
 *
 * Returns:     Boolean - whether match was found
 *
 * Description: Searches an array of operator information searching for a
 *              specified network operator.
 *
 *----------------------------------------------------------------------------*/

static Boolean findOperatorMatch (PlmnId *selectedPlmn,
                                  const VgOpFormat format,
                                  const PlmnArrayStruct *plmnArray
                                  )
{
    Int32   index;
    Int16   charPos;
    Boolean operatorMatched = FALSE;

    for (index = 0;
         (index < plmnArray->numEntries) && (operatorMatched == FALSE);
         index++ )
    {
        switch (format)
        {
            case VG_OP_SHORT: /* find matching abbreviated operator name */
            {
                for (charPos = 0, operatorMatched = TRUE;
                     (charPos < (Int16)vgStrLen (plmnArray->fullPlmnDetails[index].name.abbr)) &&
                     (operatorMatched == TRUE);
                     charPos++ )
                {
                    if (toupper (plmnArray->fullPlmnDetails[index].name.abbr[charPos]) !=
                        toupper (selectedPlmn->plmnName.abbr[charPos]))
                    {
                        operatorMatched = FALSE;
                    }
                }
                break;
            }
            case VG_OP_LONG: /* find matching long operator name */
            {
                for (charPos = 0, operatorMatched = TRUE;
                     (charPos < (Int16)vgStrLen (plmnArray->fullPlmnDetails[index].name.full)) &&
                     (operatorMatched == TRUE);
                     charPos++)
                {
                    if (toupper (plmnArray->fullPlmnDetails[index].name.full[charPos]) !=
                        toupper (selectedPlmn->plmnName.full[charPos]))
                    {
                        operatorMatched = FALSE;
                    }
                }
                break;
            }
            case VG_OP_NUM: /* find matching numeric operator name */
            {
                if ((selectedPlmn->plmn.mcc ==
                     plmnArray->fullPlmnDetails[index].plmnInfo.plmn.mcc) &&
                    (selectedPlmn->plmn.mnc ==
                     plmnArray->fullPlmnDetails[index].plmnInfo.plmn.mnc))
                {
                    /* no need to record mcc & mnc of found operator because
                     * already known */
                    operatorMatched = TRUE;
                }
                break;
            }
            default:
            {
                /* Unknown COPS format */
                FatalParam (format, 0, 0);
                break;
            }
        } /* switch */

        if (operatorMatched == TRUE)
        {
            /* Access technology needs to match too */
            if (!(plmnArray->fullPlmnDetails[index].plmnInfo.plmn.accessTechnology & EUTRAN_ACCESS_TECHNOLOGY))
            {
                operatorMatched = FALSE;
            }
            if (operatorMatched == TRUE)
            {
                /* record mcc & mnc of found operator*/
                selectedPlmn->plmn.mcc =
                    plmnArray->fullPlmnDetails[index].plmnInfo.plmn.mcc;
                selectedPlmn->plmn.mnc =
                    plmnArray->fullPlmnDetails[index].plmnInfo.plmn.mnc;
            }
        }
    }

    return (operatorMatched);
} /* findOperatorMatch */

/*----------------------------------------------------------------------------
 *
 * Function:    displayOperatorInfo
 *
 * Parameters:  entity          - current mux channel
 *              FullPlmnDetails - details of operator
 *
 * Returns:     Nothing
 *
 * Description: Display operator information in COPS range format
 *
 *----------------------------------------------------------------------------*/

static void displayOperatorInfo ( const VgmuxChannelNumber entity,
                                  const FullPlmnDetails    *plmnDetails)
{
    vgPutc (entity, '(');

    if (plmnDetails->plmnInfo.isRegisteredPlmn == TRUE)

    { /* is registered */
        vgPutc (entity, '2');
    }
    else if (plmnDetails->plmnInfo.isForbiddenPlmn == TRUE)
    { /* is forbidden */
        vgPutc (entity, '3');
    }
    else
    { /* is unregistered */
        vgPutc (entity, '1');
    }

    vgPrintf (entity, (const Char *)",\"");

    if (plmnDetails->name.plmnCoding == PLMN_CODING_DEFAULT)
    {
        /* display full name of operator  */
        vgPrintf (entity, (const Char *)"%s", plmnDetails->name.full);
    }

    vgPrintf (entity, (const Char *)"\",\"");

    if (plmnDetails->name.plmnCoding == PLMN_CODING_DEFAULT)
    {
        /* display abbreviated name of operator */
        vgPrintf (entity, (const Char *)"%s", plmnDetails->name.abbr);
    }

    /* display mcc & mnc of operator */
    if (plmnDetails->plmnInfo.threeDigitMnc)
    {
        vgPrintf (entity,
            (const Char *)"\",\"%03x%03x\"",
            plmnDetails->plmnInfo.plmn.mcc,
            plmnDetails->plmnInfo.plmn.mnc);
    }
    else
    {
        vgPrintf (entity,
            (const Char *)"\",\"%03x%02x\"",
            plmnDetails->plmnInfo.plmn.mcc,
            plmnDetails->plmnInfo.plmn.mnc);
    }

       vgPrintf (entity, (const Char *)",9" );/* NBIOT */
       vgPrintf (entity, (const Char *)")");
} /* displayOperatorInfo */

/*----------------------------------------------------------------------------
 *
 * Function:    updateNetworkRegState
 *
 * Parameters:  Nothing
 *
 * Returns:     Nothing
 *
 * Description: Job 113053 If displaying network registration status, update the
 *               display using info in the last ApexMmNetworkStateInd
 *
 *----------------------------------------------------------------------------*/

static void updateNetworkRegState (void)
{
    MobilityContext_t *mobilityContext_p = ptrToMobilityContext ();
    VgCREGData        *vgCREGData        = &mobilityContext_p->vgCREGData;
    Int8              mnState;

    switch (mobilityContext_p->vgNetworkState.serviceStatus)
    {
        case PLMN_NORMAL:
            if (mobilityContext_p->vgNetworkState.currentlyRoaming)
            {
                mnState = VGMNL_REGISTRATED_ROAMING;
            }
            else
            {
                mnState = VGMNL_REGISTRATED_HOME;
            }
            /* Modify the state depending on the SMS registration status */
            if (mobilityContext_p->vgNetworkState.additionalUpdateResult == AUR_SMS_ONLY)
            {
                switch (mnState)
                {
                    case VGMNL_REGISTRATED_HOME:
                        mnState = VGMNL_REG_HOME_SMS_ONLY;
                        break;
                    case VGMNL_REGISTRATED_ROAMING:
                        mnState = VGMNL_REG_ROAMING_SMS_ONLY;
                        break;
                    default:
                        /* Don't change anything */
                        break;
                }
            }
            break;
        case PLMN_NO_SERVICE:
        case PLMN_EMERGENCY_ONLY:
        case PLMN_ACCESS_DIFFICULTY:
            if (mobilityContext_p->vgNetworkState.isSelecting)
            {
                mnState = VGMNL_SEARCHING;
            }
            else
            {
                mnState = VGMNL_NOT_REGISTRATED;
            }
            break;
        default:
            mnState = VGMNL_REGISTRATION_DENIED;
            break;
    } /* switch */
      /* check for registration state change */
    if (vgCREGData->state != mnState)
    {
        VgmuxChannelNumber profileEntity = VGMUX_CHANNEL_INVALID;

        vgCREGData->state = mnState;

        for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
        {
          if (isEntityActive (profileEntity))
          {
            Int8 profileValue = getProfileValue(profileEntity, PROF_CREG);

            if ((profileValue == VG_CREG_ENABLED) ||
                (profileValue == VG_CREG_ENABLED_WITH_LOCATION_INFO))
            {
              viewCREG (profileEntity, FALSE);
            }
          }
        }
    }
} /* updateNetworkRegState */

static ResultCode_t vgGetResultFromErrcState(ErrcStateEngInfo errcState)
{
    ResultCode_t result = RESULT_CODE_OK;

    switch (errcState)
    {
        case ERRC_STATE_NULL_ENG_INFO:
            result =  VG_CME_MENGINFO_NO_SERVICE;
            break;
        case ERRC_STATE_SEARCH_ENG_INFO:
            result =  VG_CME_MENGINFO_CELL_SEARCH;
            break;
        case ERRC_STATE_DEACTIVATING_ENG_INFO:
            result =  VG_CME_MENGINFO_ERRC_DEACTIVATED;
            break;
        case ERRC_STATE_CELL_RESEL_ENG_INFO:
            result =  VG_CME_MENGINFO_CELL_RESELECTION;
            break;
        case ERRC_STATE_L1_TEST_MODE_ENG_INFO:
            result =  VG_CME_MENGINFO_IN_L1_TEST_MODE;
            break;
        case ERRC_STATE_REESTABLISHMENT_ENG_INFO:
            result = VG_CME_MENGINFO_IN_REESTABLISHMENT_STATE;
            break;
        case ERRC_STATE_PSM_ENG_INFO:
            result =  VG_CME_MENGINFO_PSM_STATE;
            break;
        case ERRC_STATE_INVALID_REQ_ENG_INFO:
            result =  VG_CME_MENGINFO_NO_DATA_TRANSFER_INFO_IN_IDLE_STATE;
            break;
        case ERRC_STATE_BG_SEARCH_ENG_INFO:
            result = VG_CME_MENGINFO_IN_BACKGROUND_SEARCH_STATE;
            break;        

        default:
            break;
    }

    return result;
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexMmReadPlmnSelCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_MM_READ_PLMN_SEL_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Displays the preferred operator list stored on the SIM card
 *              This request is initiated by the AT+CPOL command.
 *
 *----------------------------------------------------------------------------*/

void vgSigApexMmReadPlmnSelCnf (const SignalBuffer       *signalBuffer,
                                const VgmuxChannelNumber entity)
{
    ApexMmReadPlmnSelCnf    *sig_p                   = &signalBuffer->sig->apexMmReadPlmnSelCnf;
    MobilityContext_t       *mobilityContext_p       = ptrToMobilityContext ();
    VgCPOLData              *vgCPOLData              = &mobilityContext_p->vgCPOLData;
    SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
    VgSimInfo               *simInfo                 = &(simLockGenericContext_p->simInfo);
    VgCPLSData              *vgCPLSData_p            = &mobilityContext_p->vgCPLSData;

    Int16                   cIndex;
    Int16                   index;
    Int16                   recordsScanned           = 0;
    Int16                   maxEntryRead;
    Int8                    entriesToRead;
    PlmnId                  *source_p;
    PlmnName                *plmnName_p;
    Char                    stringBuffer[PLMN_NAME_FULL_LENGTH + NULL_TERMINATOR_LENGTH] = {0};

    if (sig_p->plmnArrayIsValid == TRUE)
    {
        /* Create real strings from the PLMN names and calculate the */
        /* size of the signal data area required.                    */

        index    = sig_p->startField;
        source_p = sig_p->plmnArray;

        while (recordsScanned < sig_p->numEntriesRead)
        {
            plmnName_p = &source_p->plmnName;

            if (source_p->present == TRUE)
            {
                if (vgCPOLData->firstPlmn == TRUE)
                {
                    vgCPOLData->firstPlmn = FALSE;
                    vgPutNewLine (entity);
                }

                /* Print index and current CPOL Mode setting */
                /*The index indicates the order of preference on the SIM*/
                /*It may not match the offset on the SIM as there could be
                 * some invalid entries stuck at the middle. SIM manager only
                 * reports the valid entries...*/
                index += 1;

                if(source_p->plmn.mcc == UNUSED_MCC)
                {
                    /* skip the unused plmn */
                    recordsScanned++;
                    source_p++;
                    continue;
                }

                vgPrintf (entity,
                    (const Char *)"+CPOL: %d,%d,\"",
                    index,
                    vgCPOLData->readFormat );

                /* Get the network name as a usable string into buffer */
                switch (vgCPOLData->readFormat)
                {
                    case VG_OP_LONG:
                    {
                        /* Copy long name into stringBuffer */
                        for (cIndex = 0; cIndex < PLMN_NAME_FULL_LENGTH; cIndex++)
                        {
                            stringBuffer[cIndex] = plmnName_p->full[cIndex];
                            if (stringBuffer[cIndex] == NULL_CHAR)
                            {
                                break;
                            }
                        }
                        if (cIndex == PLMN_NAME_FULL_LENGTH)
                        {
                            stringBuffer[PLMN_NAME_FULL_LENGTH] = NULL_CHAR;
                        }
                        vgPrintf (entity, (const Char *)"%s", stringBuffer);
                        vgPutc (entity, '\"');
                        break;
                    }
                    case VG_OP_SHORT:
                    {
                        /* Copy abbreviated name into stringBuffer */
                        for (cIndex = 0; cIndex < PLMN_NAME_ABBR_LENGTH; cIndex++)
                        {
                            stringBuffer[cIndex] = plmnName_p->abbr[cIndex];
                            if (stringBuffer[cIndex] == NULL_CHAR)
                            {
                                break;
                            }
                        }
                        if (cIndex == PLMN_NAME_ABBR_LENGTH)
                        {
                            stringBuffer[PLMN_NAME_ABBR_LENGTH] = NULL_CHAR;
                        }
                        vgPrintf (entity, (const Char *)"%s", stringBuffer);
                        vgPutc (entity, '\"');
                        break;
                    }
                    case VG_OP_NUM:
                    default:
                    { /* network operator in numeric format */
                        if (source_p->threeDigitMnc)
                        {
                            vgPrintf (entity,
                                (const Char *)"%03x%03x",
                                source_p->plmn.mcc,
                                source_p->plmn.mnc);
                        }
                        else
                        {
                            vgPrintf (entity,
                                (const Char *)"%03x%02x",
                                source_p->plmn.mcc,
                                source_p->plmn.mnc);
                        }
                        vgPutc (entity, '\"');

                        if ((vgCPLSData_p->plmnSelector != ABMM_USER_SELECTOR) || (simInfo->userPlmnSelector))
                        {
                            /* TODO: For NB-IOT Change this */
                            vgPrintf (entity,
                                (const Char *)",%d,%d,%d,%d",
                                ((source_p->plmn.accessTechnology >> VG_GSM_ACT_OFFSET) & 0x01), /*gsm Act*/
                                ((source_p->plmn.accessTechnology >> VG_GSM_COMPACT_ACT_OFFSET) & 0x01), /*gsm Compact Act*/
                                ((source_p->plmn.accessTechnology >> VG_UTRAN_ACT_OFFSET) & 0x01), /*utran Act*/
                                ((source_p->plmn.accessTechnology >> VG_EUTRAN_ACT_OFFSET) & 0x01)); /*eutran Act*/
                        }
                        break;
                    }
                } /* switch */

                vgPutNewLine (entity);
            }
            recordsScanned++;
            source_p++;
        }

        maxEntryRead = (Int16)(sig_p->startField + sig_p->numEntriesRead);
        if ((maxEntryRead < sig_p->totalNumEntries) && (sig_p->numEntriesRead == PLMN_READ_LIMIT))
        {
            /* We have more entries to read */
            entriesToRead = (Int8)(sig_p->totalNumEntries - maxEntryRead);
            if (entriesToRead > PLMN_READ_LIMIT)
            {
                entriesToRead = PLMN_READ_LIMIT;
            }
            if (entriesToRead > 0)
            { /* send a request off for remaining operators in list */
                vgSigApexMmReadPlmnSelReq (maxEntryRead,
                    entriesToRead,
                    vgCPLSData_p->plmnSelector,
                    entity);
            }
        }
        else
        {
            setResultCode (entity, RESULT_CODE_OK);
        }
    }
    else
    {
        /* PLMN array is invalid. */
        setResultCode (entity, VG_CME_SIM_FAILURE);
    }
} /* vgSigApexMmReadPlmnSelCnf */

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexMmWritePlmnSelCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_MM_WRITE_PLMN_SEL_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: It returns the status of a preceding write request to the
 *              preferred operator list initiated by the AT+CPOL command.
 *----------------------------------------------------------------------------*/
void vgSigApexMmWritePlmnSelCnf (const SignalBuffer       *signalBuffer,
                                 const VgmuxChannelNumber entity)
{
    ApexMmWritePlmnSelCnf *sig_p = &signalBuffer->sig->apexMmWritePlmnSelCnf;

    switch (getCommandId (entity))
    {
        case VG_AT_MM_CPOL:
        {
            switch (sig_p->writeResult) /* set result code depending on writeResult */
            {
                case MM_REQ_OK: /* request was succesfully completed */
                {
                    setResultCode (entity, RESULT_CODE_OK);
                    break;
                }
                case MM_REQ_SIM_ERROR: /* a SIM failure has occurred */
                {
                    setResultCode (entity, VG_CME_SIM_FAILURE);
                    break;
                }
                case MM_REQ_SIM_FULL: /* there is no more space on the SIM for
                               * preferred operator list */
                {
                    setResultCode (entity, VG_CME_CPOL_SIM_FULL);
                    break;
                }
                case MM_REQ_OPERATOR_NOT_FOUND: /* the operator to be added to the
                                         * list couldn't be found */
                {
                    setResultCode (entity, VG_CME_CPOL_OPERATOR_NOT_FOUND);
                    break;
                }
                default: /* assumption is made that we do not have control of MM
                  * but this may not be the case */
                {
                    setResultCode (entity, VG_CME_CPOL_NOT_IN_CONTROL);
                    break;
                }
            } /* switch */
            break;
        }
        default: /* this case should not be reached since only the AT+CPOL command
              * may initiate a write request to the preferred operator list */
        {
            /* Unexpected command Writing Plmn Sel */
            FatalParam(entity, getCommandId (entity), 0);
            break;
        }
    } /* switch */
} /* vgSigApexMmWritePlmnSelCnf */

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexMmPlmnListCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_MM_PLMNLIST_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Returns a list of all the valid network operators that we may
 *              care to register with. The request is intiated by the AT+CPOL
 *              command.
 *----------------------------------------------------------------------------*/

void vgSigApexMmPlmnListCnf (const SignalBuffer       *signalBuffer,
                             const VgmuxChannelNumber entity)
{
    ApexMmPlmnListCnf *sig_p             = &signalBuffer->sig->apexMmPlmnListCnf;

    MobilityContext_t *mobilityContext_p = ptrToMobilityContext ();
    VgCOPSData        *vgCOPSData        = &mobilityContext_p->vgCOPSData;

    Int16             index              = 0;

    if (getCommandId(entity) != VG_AT_GN_MABORT)
    {
      /* check if the network operator list is valid */
      if (sig_p->plmnArrayIsValid == TRUE)
      {
          switch (vgCOPSData->state)
          {
              case VG_COPS_MANUAL: /* manually selecting operator */
              {
                  switch (vgCOPSData->mode)
                  {
                      case VG_OP_MANUAL_OPERATOR_SELECTION:
                      case VG_OP_MANUAL_THEN_AUTOMATIC:
                      {
                          /* if the operator was found then send a request of to select it as
                           * the desired operator */

                          if (findOperatorMatch (&vgCOPSData->selectedPlmn,
                                  vgCOPSData->format,
                                  &sig_p->plmnArray
                                  ) == TRUE)
                          {
                              vgSigApexMmPlmnSelectReq (entity);
                          }
                          else /* otherwise return error indicating operator could not be found */
                          {
                              setResultCode (entity, VG_CME_NOT_FOUND);
                              /* Job 113053 Set the reg state appropriately */
                              updateNetworkRegState ();
                          }
                          break;
                      }
                      case VG_OP_AUTOMATIC_MODE:
                      case VG_OP_SET_FORMAT:
                      case VG_OP_MANUAL_DEREGISTER:
                      default:
                      {
                          /* Unexpected COPS mode */
                          FatalParam(entity, vgCOPSData->state, vgCOPSData->mode);
                          break;
                      }
                  } /* switch */
                  break;
              }
              case VG_COPS_LIST: /* simply listing available operators */
              {
                  /* displays "+COPS" at beginning of listing */

                  vgPutNewLine (entity);

                  vgPrintf (entity, (const Char *)"+COPS: ");

                  /* move through array of operators and display entries */
                  for (index = 0; index < sig_p->plmnArray.numEntries; index++)
                  {
                      displayOperatorInfo (entity,
                                           &sig_p->plmnArray.fullPlmnDetails[index]);
                      if ( index + 1 < sig_p->plmnArray.numEntries )
                      {
                          vgPutc (entity, ',');
                      }
                  }

                  /* display other parameter ranges */
                  vgPrintf (entity, (const Char *)",,(0-4),(0-2)");
                  vgPutNewLine (entity);

                  setResultCode (entity, RESULT_CODE_OK);
                  break;
              }
              default:
              {
                  /* Invalid COPS state */
                  FatalParam(entity, vgCOPSData->state, 0);
                  break;
              }
          } /* switch */
      }
      else /* network operator list not valid */
      {
          switch (sig_p->status)
          {
              case PLMN_LIST_NOT_FOUND:
                  setResultCode (entity, VG_CME_NOT_FOUND);
                  break;

              case PLMN_LIST_SIM_NOK:
                  setResultCode (entity, VG_CME_SIM_FAILURE);
                  break;

              case PLMN_LIST_NOT_ALLOWED:
                  setResultCode (entity, VG_CME_OPERATION_NOT_ALLOWED);
                  break;

              default:
                  FatalParam(sig_p->status, 0,0);
                  break;
          } /* switch */
            /* Job 113053 Set the reg state appropriately */
          updateNetworkRegState ();
      }
    }
    /* job119076: reset COPS command state */
    vgCOPSData->state = VG_COPS_LIST;
} /* vgSigApexMmPlmnListCnf */

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexMmAbortPlmnListCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_MM_ABORT_PLMNLIST_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description:
 *----------------------------------------------------------------------------*/
void vgSigApexMmAbortPlmnListCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
    ApexMmAbortPlmnListCnf *sig_p        = &signalBuffer->sig->apexMmAbortPlmnListCnf;
    ResultCode_t           result        = RESULT_CODE_OK;
    VgCOPSData             *vgCOPSData   = &(ptrToMobilityContext()->vgCOPSData);

    vgCOPSData->state = VG_COPS_LIST;
    switch (getCommandId (entity))
    {
      case VG_AT_GN_MABORT:
      {
        if (sig_p->requestStatus == MM_REQ_OK)
        {
          result = RESULT_CODE_OK;
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
        break;
      }
      default:
      {
        /* Illegal AT command running SIG_APEX_MM_ABORT_PLMNLIST_CNF  */
        FatalParam(entity, getCommandId (entity), 0);
//        result = RESULT_CODE_ERROR;
        break;
      }
    }
    setResultCode (entity, result);
} /* vgSigApexMmAbortPlmnListCnf */

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexMmPlmnSelectCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_MM_PLMN_SELECT_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Processes result of request to select a network operator
 *----------------------------------------------------------------------------*/

void vgSigApexMmPlmnSelectCnf (const SignalBuffer       *signalBuffer,
                               const VgmuxChannelNumber entity)
{
    ApexMmPlmnSelectCnf *sig_p             = &signalBuffer->sig->apexMmPlmnSelectCnf;
    MobilityContext_t   *mobilityContext_p = ptrToMobilityContext ();
    VgCOPSData          *vgCOPSData        = &(mobilityContext_p->vgCOPSData);
#if defined(UPGRADE_MTNET)
    SignalBuffer             cResetSig          = kiNullBuffer;
    CiResetInd              *ciResetInd_p       = PNULL;
#endif


    vgCOPSData->autoSelectInProgress = FALSE;

    switch (sig_p->result)
    {
        case PLMN_SELECT_OK: /* selection went fine */
        {
          if(getCommandId(entity) == VG_AT_MM_SYSCONFIG)
          {
            setResultCode (entity, RESULT_CODE_OK);
          }
#if defined(UPGRADE_MTNET)
          else if(getCommandId(entity) == VG_AT_MM_RESET)
          {
            /* send signal internally to CI to inform other sub-systems of the modified
             * SIM state */
            KiCreateZeroSignal( SIG_CI_RESET_IND, sizeof(CiResetInd), &cResetSig);
            ciResetInd_p = (CiResetInd *)cResetSig.sig;
            ciResetInd_p->entity = entity;
            KiSendSignal (VG_CI_TASK_ID, &cResetSig);


            /* Request the Multiplexer top resend its low power vote since */
            /* layer 1 has just lost it */

            /* we need to update the CNMI state and Message Service again
              * since the protocol stack has lost it! */
            vgApexSendSmProfileChangedReq(entity);
            setResultCode (entity, RESULT_CODE_OK);
          }
#endif
          else
          {
            setResultCode (entity, RESULT_CODE_OK);
          }
          break;
        }
        case PLMN_SELECT_FAIL: /* selected failed */
        default:
        {
            /* if in manual then automatic mode and the result says use manual mode
             * then send another operator selection request off */
            if ((sig_p->useManualMode == TRUE) &&
                (vgCOPSData->mode == VG_OP_MANUAL_THEN_AUTOMATIC))
            {
                vgCOPSData->mode          = VG_OP_AUTOMATIC_MODE;
#if defined (UPGRADE_3G)
                vgCOPSData->returnToRplmn = FALSE;
#endif /* (UPGRADE_3G) */
                vgSigApexMmPlmnSelectReq (entity);
            }
            else /* no operator has been successully selected */
            {
                setResultCode (entity, VG_CME_NO_NETWORK_SERVICE);
            }
            break;
        }
    } /* switch */
} /* vgSigApexMmPlmnSelectCnf */

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexMmDeregisterCnf
 *
 * Parameters:  SignalBuffer - structure containing signal:
 *                             SIG_APEX_MM_DEREGISTER_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_MM_DEREGISTER_CNF signal received
 *              from the BL task's shell process. The signal is received
 *              in response to a SIG_APEX_MM_DEREGISTER_REQ.
 *-------------------------------------------------------------------------*/

void vgSigApexMmDeregisterCnf (const SignalBuffer       *signalBuffer,
                               const VgmuxChannelNumber entity)
{
    ApexMmDeregisterCnf *sig_p              = &signalBuffer->sig->apexMmDeregisterCnf;
    MobilityContext_t   *mobilityContext_p  = ptrToMobilityContext ();
    VgCOPSData          *vgCOPSData         = &mobilityContext_p->vgCOPSData;

    if ((getCommandId (entity) == VG_AT_MM_COPS) &&
        ((vgCOPSData->mode == VG_OP_MANUAL_DEREGISTER) ||
         (vgCOPSData->mode == VG_OP_AUTOMATIC_MODE)))
    {
        if (sig_p->requestStatus == MM_REQ_OK)
        {
            setResultCode (entity, RESULT_CODE_OK);
        }
        else
        {
            setResultCode (entity, VG_CME_OPERATION_FAILED);
        }
    }
#if defined(UPGRADE_MTNET)
    else if(getCommandId (entity) == VG_AT_MM_RESET)
    {
        /* register */
        vgCOPSData->mode = VG_OP_AUTOMATIC_MODE;
# if defined (UPGRADE_3G)
        vgCOPSData->returnToRplmn = FALSE;
# endif /* (UPGRADE_3G) */

        (void) vgChManContinueAction (entity, SIG_APEX_MM_PLMN_SELECT_REQ);
    }
#endif  /* #if defined(UPGRADE_MTNET) */
    else
    {
        /* Unexpected command / COPS mode */
        FatalParam(entity, getCommandId (entity), vgCOPSData->mode);
    }
} /* vgSigApexMmDeregisterCnf */

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexAbemPlmnTestCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_EM_PLMN_TEST_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Displays network operator information and requests the next
 *              operator information if there are any.
 *----------------------------------------------------------------------------*/
void vgSigApexAbemPlmnTestCnf (const SignalBuffer       *signalBuffer,
                               const VgmuxChannelNumber entity)
{
    ApexEmPlmnTestCnf *sig_p             = &signalBuffer->sig->apexEmPlmnTestCnf;

    MobilityContext_t *mobilityContext_p = ptrToMobilityContext ();
    VgCOPNData        *vgCOPNData        = &(mobilityContext_p->vgCOPNData);

    Boolean           printPlmn          = TRUE;
    Boolean           readNextPlmn       = FALSE;

    /* Only store if plmn is available */
    if (sig_p->plmnFound == TRUE)
    {
        /* Note current mnc and mcc */
        vgCOPNData->currentPlmn.mcc = sig_p->requestedPlmn.mcc;
        vgCOPNData->currentPlmn.mnc = sig_p->requestedPlmn.mnc;

        if (vgCOPNData->listed == 0)
        {
            /* Remember first mcc and mnc in list */
            vgCOPNData->initialPlmn.mcc = sig_p->requestedPlmn.mcc;
            vgCOPNData->initialPlmn.mnc = sig_p->requestedPlmn.mnc;

            /* Not end of list */
            readNextPlmn                = TRUE;
        }
        else
        {
            /* check if reached start of list again */
            if ((vgCOPNData->initialPlmn.mcc == sig_p->requestedPlmn.mcc) &&
                (vgCOPNData->initialPlmn.mnc == sig_p->requestedPlmn.mnc))
            {
                /* end of list found */
                printPlmn = FALSE;
            }
            else
            {
                /* Not end of list */
                readNextPlmn = TRUE;
            }
        }

        /* increment read count */
        vgCOPNData->listed += 1;

        /* Display Plmn in numeric and alpha numeric forms */
        if (printPlmn == TRUE)
        {
            if (vgCOPNData->listed == 1)
            {
                vgPutNewLine (entity);
            }
            vgPrintf (entity,
                (const Char *)"+COPN: \"%03x%02x\",\"%s\"",
                sig_p->requestedPlmn.mcc,
                sig_p->requestedPlmn.mnc,
                sig_p->selectedName.full);
            vgPutNewLine (entity);

        }

        /* request next plmn in list */
        if (readNextPlmn == TRUE)
        {
            vgCOPNData->order = EM_PLMN_GET_NEXT;
            setResultCode(  entity,
                            vgChManContinueActionFlowControl(entity, SIG_APEX_EM_PLMN_TEST_REQ) );
        }
        else
        {
            vgCOPNData->listed = 0;

            /* set result code, listing finished */
            setResultCode (entity, RESULT_CODE_OK);
        }
    }
    else
    {
        vgCOPNData->listed = 0;
        setResultCode (entity, RESULT_CODE_ERROR);
    }
} /* vgSigApexAbemPlmnTestCnf */

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexMmRssiInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_MM_RSSI_IND
 * Returns:     nothing
 *
 * Description: receives radio information about quality and level of signal
 *----------------------------------------------------------------------------*/

void vgSigApexMmRssiInd (const SignalBuffer *signalBuffer)
{
    ApexMmRssiInd     *sig_p              = &signalBuffer->sig->apexMmRssiInd;
    MobilityContext_t *mobilityContext_p  = ptrToMobilityContext ();
    VgmuxChannelNumber profileEntity      = 0;

    /* set flag which indicates that we have the receive information */
    mobilityContext_p->haveReceiveInfo    = TRUE;

    /* received signal strength indication */
    mobilityContext_p->receiveLevel       = sig_p->rssi;

    mobilityContext_p->eutraRsrp          = sig_p->rsrp;
    mobilityContext_p->eutraRsrq          = sig_p->rsrq;

    mobilityContext_p->vgdlber              = sig_p->dlber;
    mobilityContext_p->vgulber              = sig_p->ulber;    

    for (profileEntity = 0;
          profileEntity < CI_MAX_ENTITIES;
           profileEntity++)
    {
      if (isEntityActive (profileEntity))
      {
        if (isProfileValueBitEqual(profileEntity,
                                    PROF_MUNSOL,
                                    PROF_BIT_MSQN,
                                    REPORTING_ENABLED) == TRUE)
        {
            viewCESQ (profileEntity, CESQ_MSQN_UNSOLICITED);
        }

      }
    }

} /* vgSigApexMmRssiInd */

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexMmBandInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_MM_BAND_IND
 * Returns:     nothing
 *
 * Description: receives band mode change info
 *----------------------------------------------------------------------------*/

void vgSigApexMmBandInd (const SignalBuffer *signalBuffer)
{
    PARAMETER_NOT_USED(signalBuffer);

    /* Do nothing for NB-IOT */

} /* vgSigApexMmBandInd */

#if defined (ENABLE_AT_ENG_MODE)
/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexMmCipherInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_MM_CIPHER_IND
 * Returns:     nothing
 *
 * Description: receives band mode change info
 *----------------------------------------------------------------------------*/
void vgSigApexMmCipherInd (const SignalBuffer *signalBuffer)
{
    EngineeringModeContext_t *emContext_p = ptrToEngineeringModeContext();

    emContext_p->emAttributes.debugElementsBitField |= VGEM_GSM_CIPHER_BIT;

    memcpy (&(emContext_p->emAttributes.mmrCipherInd),
        &(signalBuffer->sig->apexMmCipherInd),
        sizeof (ApexMmCipherInd));
} /* vgSigApexMmCipherInd */
#endif /* (ENABLE_AT_ENG_MODE) */

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexMmNetworkStateInd
 *
 * Parameters:  SignalBuffer - structure containing signal:
 *                             SIG_APEX_MM_NETWORK_STATE_IND
 *
 * Returns:     Nothing
 *
 * Description: Updates network state information from signal contents. There
 *              may be a slight delay in the network state changing and
 *              receiving this signal.
 *-------------------------------------------------------------------------*/
void vgSigApexMmNetworkStateInd (const SignalBuffer *signalBuffer)
{
    ApexMmNetworkStateInd *sig_p                  = &signalBuffer->sig->apexMmNetworkStateInd;

    MobilityContext_t     *mobilityContext_p      = ptrToMobilityContext ();
    VgCREGData            *vgCREGData             = &mobilityContext_p->vgCREGData;

    VgCOPSData            *vgCOPSData             = &mobilityContext_p->vgCOPSData;
    VgEdrxData            *vgEdrxData_p           = &mobilityContext_p->vgEdrxData;
    VgCciotoptData        *vgCciotoptData_p       = &mobilityContext_p->vgCciotoptData;
    VgCipcaData           *vgCipcaData_p          = &mobilityContext_p->vgCipcaData;
    SleepManContext_t     *sleepManContext_p      = ptrToSleepManContext();
#if defined (FEA_NFM)
    VgNfmData             *vgNfmData_p            = &mobilityContext_p->vgNfmData;
#endif
    Boolean               stateChange             = FALSE;
    Boolean               lacAndCellIdChange      = FALSE;
    Int8                  mnState;
    VgmuxChannelNumber    profileEntity           = VGMUX_CHANNEL_INVALID;

    Boolean               sendUnsolicitedMODE     = FALSE;


    GprsGenericContext_t  *gprsGenericContext_p                = ptrToGprsGenericContext ();

    Int8                  numActiveOrPendingActivePrimContexts  = 0;
    Int8                  cid;
    VgmuxChannelNumber    entity;
    VgPsdStatusInfo       *vgPsdStatusInfo_p                   = PNULL;
    Boolean               cgactPending                         = FALSE;
    Char                  tempStr[VG_CEDRX_MAX_BIN_STR_LEN + NULL_TERMINATOR_LENGTH] = {0};

    /* For simultaneous LTE TDD/FDD support, we will still be told by ABMM whether we are on LTE-TDD or
     * LTE-FDD
     */
    /* Update the Attach PDN setting */
    vgCipcaData_p->dataValid = TRUE;

    if (sig_p->attachWithoutPdn == FALSE)
    {
       vgCipcaData_p->vgCipcaOpt = VG_CIPCA_ATTACH_WITH_PDN;
    }
    else
    {
       vgCipcaData_p->vgCipcaOpt = VG_CIPCA_ATTACH_WITHOUT_PDN;
    }

    /* job119076: don't update mode during manual PLMN selection */
    if (vgCOPSData->state != VG_COPS_MANUAL)
    {
        /* update cops mode - manual / automatic */
        if (sig_p->isSelecting == TRUE)
        {
            if (vgCOPSData->mode != VG_OP_MANUAL_THEN_AUTOMATIC)
            {
                if (sig_p->isInManualMode)
                {
                    vgCOPSData->mode = VG_OP_MANUAL_OPERATOR_SELECTION;
                }
                else
                {
                    vgCOPSData->mode = VG_OP_AUTOMATIC_MODE;
                }
            }
        }
        else
        {
            if (sig_p->serviceStatus == PLMN_NORMAL)
            {
                if (sig_p->isInManualMode == TRUE)
                {
                    vgCOPSData->mode = VG_OP_MANUAL_OPERATOR_SELECTION;
                }
                else
                {
                    vgCOPSData->mode = VG_OP_AUTOMATIC_MODE;
                }
            }
        }
    }

    /* Update network registration information */
    switch (sig_p->serviceStatus)
    {
        case PLMN_NORMAL:
        {
            mobilityContext_p->vgNetworkPresent = TRUE;
            /* Job 113053 ME may be performing a background search so
             * check for selecting in normal mode too */
            if (sig_p->currentlyRoaming == TRUE)
            {
                mnState = VGMNL_REGISTRATED_ROAMING;
            }
            else
            {
                mnState = VGMNL_REGISTRATED_HOME;
            }
            /* Modify the state depending on the SMS registration status */
            if (sig_p->additionalUpdateResult == AUR_SMS_ONLY)
            {
                switch (mnState)
                {
                    case VGMNL_REGISTRATED_HOME:
                        mnState = VGMNL_REG_HOME_SMS_ONLY;
                        break;
                    case VGMNL_REGISTRATED_ROAMING:
                        mnState = VGMNL_REG_ROAMING_SMS_ONLY;
                        break;
                    default:
                        /* Don't change anything */
                        break;
                }
            }
            break;
        }
        case PLMN_NO_SERVICE:
        /* Deliberate fall through */
        case PLMN_EMERGENCY_ONLY:
        case PLMN_ACCESS_DIFFICULTY:
        {
            mobilityContext_p->vgNetworkPresent = FALSE;
            if (sig_p->isSelecting == TRUE)
            {
                mnState = VGMNL_SEARCHING;
            }
            else
            {
                mnState = VGMNL_NOT_REGISTRATED;
            }
            break;
        }
        default:
        {
            mobilityContext_p->vgNetworkPresent = FALSE;
            mnState = VGMNL_REGISTRATION_DENIED;
            break;
        }
    } /* switch */

    /* copy network state signal */
    memcpy (&mobilityContext_p->vgNetworkState,
        sig_p,
        sizeof (ApexMmNetworkStateInd));


    if ((mnState == VGMNL_REGISTRATED_HOME) ||
        (mnState == VGMNL_REGISTRATED_ROAMING) ||
        (mnState == VGMNL_REG_HOME_SMS_ONLY) ||
        (mnState == VGMNL_REG_ROAMING_SMS_ONLY))
    {
        /* When handover, AS can't report cellId and lac to ATCI and so ATCI add this case of
          *accessTechnology change this case.
          */
        if ((sig_p->lai.lac != vgCREGData->lac) ||
            (sig_p->cellId  != vgCREGData->cellId) ||
            (sig_p->plmn.accessTechnology != vgCOPSData->selectedPlmn.plmn.accessTechnology))
        {
            lacAndCellIdChange = TRUE;
            vgCREGData->lac    = sig_p->lai.lac;
            vgCREGData->cellId = sig_p->cellId;
        }
    }

    /* check for registration state change */
    if (vgCREGData->state != mnState)
    {
        stateChange = TRUE;

        /* update location information if required */
        if ((sig_p->lai.lac != vgCREGData->lac) ||
            (sig_p->cellId  != vgCREGData->cellId))
        {
            vgCREGData->lac    = sig_p->lai.lac;
            vgCREGData->cellId = sig_p->cellId;
        }
    }

    /* Store the access technology */
    vgCOPSData->selectedPlmn.plmn.accessTechnology = sig_p->plmn.accessTechnology;

    vgCREGData->state = mnState;

    if (vgCREGData->state == VGMNL_NOT_REGISTRATED)
    {
        vgCOPSData->fplmnOnlyAvailable = sig_p->fplmnOnlyAvailable;

        for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
        {
            if ((isEntityActive (profileEntity)) &&
                (getProfileValueBit (profileEntity,
                                     PROF_MUNSOL,
                                     PROF_BIT_MFPLMN) == REPORTING_ENABLED))
            {
                vgPutNewLine (profileEntity);

                if (vgCOPSData->fplmnOnlyAvailable == TRUE)
                {
                    vgPuts (profileEntity, (const Char *)"*MFPLMN: 1");
                }
                else
                {
                    vgPuts (profileEntity, (const Char *)"*MFPLMN: 0");
                }

                vgFlushBuffer (profileEntity);
            }
        }
    }

    /*
     * Update the AT^MODE settings
     */

    /* display unsolicited registration information if enabled */
    for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
    {
        if (isEntityActive (profileEntity))
        {
            if ((getProfileValue (profileEntity, PROF_CREG) == VG_CREG_ENABLED_WITH_LOCATION_INFO) &&
                ((lacAndCellIdChange == TRUE) || (stateChange == TRUE)))
            {
                viewCREG (profileEntity,  FALSE);
            }
            else
            {
                if ((getProfileValue (profileEntity, PROF_CREG) == VG_CREG_ENABLED) &&
                    (stateChange == TRUE))
                {
                    viewCREG (profileEntity, FALSE);
                }
            }

            }
        }

#if defined (UPGRADE_SHARE_MEMORY)  || defined(UPGRADE_SHMCL_SOLUTION)
    mobilityContext_p->cregData.serviceStatus = sig_p->serviceStatus;
    writeCregDataToShareMemory();
#endif /* UPGRADE_SHARE_MEMORY|| UPGRADE_SHMCL_SOLUTION  */

    mobilityContext_p->vgSYSCONFIGData.currentParam.roamSupport = ROAMING_SUPPORT;

    if (sig_p->isInManualMode)
    {
      SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext();
      VgSimInfo                *simInfo = &(simLockGenericContext_p->simInfo);

      if((sig_p->plmn.mcc == simInfo->hplmn.plmn.mcc) && (sig_p->plmn.mnc == simInfo->hplmn.plmn.mnc))
      {
        mobilityContext_p->vgSYSCONFIGData.currentParam.roamSupport = ROAMING_NOT_SUPPORT;
      }
    }

    /* For LTE - if we have registered to a network and there are no
     * PDP contexts active and there are channels active - then we need to
     * connect to the default bearer activated during the attach procedure at
     * this point - if it isn't already
     */

    /* Find the first enabled entity */
    entity = findFirstEnabledChannel();

    /* For NB-IOT we need to check if we are allowed to activate default
     * PDN connection on attach - before doing anything (set by AT+CIPCA)
     */
    if ((entity != VGMUX_CHANNEL_INVALID) &&
        vgIsCurrentAccessTechnologyLte() &&
        vgPsdAttached() &&
        vgCIPCAPermitsActivateAttachDefBearer() &&
        (getProfileValue (entity, PROF_MLTEGCF) != GCF_TEST_MODE_DISABLE))
    {
      /* Check if there are any profiles active already - or waiting to
       * be activated - in which case we don't want to connect to the
       * default bearer activated during attach this way.
       */

      for (cid = vgGpGetMinCidValue(entity); cid < MAX_NUMBER_OF_CIDS; cid++)
      {
        if (gprsGenericContext_p->cidUserData [cid] != PNULL)
        {
          vgPsdStatusInfo_p = gprsGenericContext_p->cidUserData [cid];

          if ((vgPsdStatusInfo_p != PNULL) &&
              (vgPsdStatusInfo_p->profileDefined) &&
              ((vgPsdStatusInfo_p->isActive) || (vgPsdStatusInfo_p->pendingContextActivation))
#if defined (FEA_DEDICATED_BEARER)              
              && (!vgPsdStatusInfo_p->psdBearerInfo.secondaryContext)
#endif /* FEA_DEDICATED_BEARER */              
              )

          {
            numActiveOrPendingActivePrimContexts++;
          }
        }
      }

      /* We also need to check if any channel has a pending attach request (AT+CGATT).
       * If so then we cannot activate at this point and need to wait for the
       * attach to complete.
       */
      for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
      {
        if ((isEntityActive (profileEntity)) && (getCommandId(profileEntity) == VG_AT_GP_CGATT))
        {
          cgactPending = TRUE;
        }
      }

      if ((numActiveOrPendingActivePrimContexts == 0) &&
          (!cgactPending) &&
          (gprsGenericContext_p->vgLteAttachDefaultBearerState == LTE_ATTACH_DEFAULT_BEARER_NOT_CONNECTED))
      {
        vgActivateAttachDefBearerContext (entity);
      }
    }

/* look at the newtork EDRX settings and CIoT optimisations */

   if (sig_p->serviceStatus != PLMN_NO_SERVICE)
   {
     if (((vgEdrxData_p->userDataValid == FALSE) ||
         (vgEdrxData_p->userEdrxValue!= sig_p->edrxInfo.userEdrxValue) ||
         (vgEdrxData_p->userPagingTimeWindow != sig_p->edrxInfo.userPtw)) ||
        ((vgEdrxData_p->nwDataValid == FALSE) ||
          ((vgEdrxData_p->nwEdrxSupport != sig_p->edrxInfo.nwEdrxSupport) ||
           (vgEdrxData_p->nwEdrxValue!= sig_p->edrxInfo.nwEdrxValue) ||
           (vgEdrxData_p->nwPagingTimeWindow != sig_p->edrxInfo.pagingTimeWindow))))
     {
        vgEdrxData_p->userEdrxSupport = sig_p->edrxInfo.userEdrxSupport;
        vgEdrxData_p->userEdrxValue = sig_p->edrxInfo.userEdrxValue;
        vgEdrxData_p->userPtwPresence = sig_p->edrxInfo.userPtwPresent;
        vgEdrxData_p->userPagingTimeWindow = sig_p->edrxInfo.userPtw;
        vgEdrxData_p->userDataValid = TRUE;

        vgEdrxData_p->nwEdrxSupport = sig_p->edrxInfo.nwEdrxSupport;
        vgEdrxData_p->nwEdrxValue = sig_p->edrxInfo.nwEdrxValue;
        vgEdrxData_p->nwPagingTimeWindow = sig_p->edrxInfo.pagingTimeWindow;
        vgEdrxData_p->nwDataValid = TRUE;
        if (sig_p->edrxInfo.userEdrxSupport == TRUE)
        {
           for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
           {
              if ((isEntityActive (profileEntity)) &&
                (getProfileValue (profileEntity, PROF_CEDRXS) == VG_CEDRXP_UNSOL_ENABLE))
              {
                   vgPutNewLine (entity);
                   vgPrintf (entity, (const Char *)"+CEDRXP: %d",VG_CEDRX_ACT_NB_IOT);
                   vgPrintf (entity, (const Char*)",\"");
                   vgInt8ToBinString(mobilityContext_p->vgEdrxData.userEdrxValue, VG_CEDRX_MAX_BIN_STR_LEN, tempStr);
                   vgPrintf (entity, (const Char*)tempStr);
                   vgPrintf (entity, (const Char*)"\"");
                   if (sig_p->edrxInfo.nwEdrxSupport == TRUE)
                   {
                      vgPrintf (entity, (const Char*)",\"");
                      vgInt8ToBinString(mobilityContext_p->vgEdrxData.nwEdrxValue, VG_CEDRX_MAX_BIN_STR_LEN, tempStr);
                      vgPrintf (entity, (const Char*)tempStr);
                      vgPrintf (entity, (const Char*)"\"");

                      vgPrintf (entity, (const Char*)",\"");
                      vgInt8ToBinString(mobilityContext_p->vgEdrxData.nwPagingTimeWindow, VG_CEDRX_MAX_BIN_STR_LEN, tempStr);
                      vgPrintf (entity, (const Char*)tempStr);
                      vgPrintf (entity, (const Char*)"\"");
                   }
                   vgPutNewLine (entity);

                   vgFlushBuffer (profileEntity);
              }
           }
        }

      }
       /* handle CIoT optimisations */
      if ((vgCciotoptData_p->nwDataValid ==FALSE) || (vgCciotoptData_p->supportedNwOpt != sig_p->supportedNwOptions))
      {

         vgCciotoptData_p->supportedNwOpt = sig_p->supportedNwOptions;
         vgCciotoptData_p->nwDataValid = TRUE;
         for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
         {
            if ((isEntityActive (profileEntity)) &&
                (getProfileValue (profileEntity, PROF_CCIOTOPT) == VG_CEDRXP_UNSOL_ENABLE))
            {
                vgPutNewLine (profileEntity);
                vgPrintf (profileEntity, (const Char *)"+CCIOTOPTI: %d",sig_p->supportedNwOptions);
                vgFlushBuffer (profileEntity);
            }
         }
      }

   }
   else
   {  /* clear any edrx, psm or ciot opt data as this is not valid when in no service */
      vgEdrxData_p->userDataValid = FALSE;
      vgEdrxData_p->nwDataValid = FALSE;
      vgCciotoptData_p->nwDataValid = FALSE;
   }

#if defined (FEA_NFM)
   if ( sig_p->nfmActive &&
        ((vgNfmData_p->remainingNfmStartTimerValue != sig_p->remainingNfmStartTimerValue) ||
         (vgNfmData_p->remainingNfmBackOffTimerValue != sig_p->remainingNfmBackOffTimerValue)) )
   {
      vgNfmData_p->remainingNfmStartTimerValue = sig_p->remainingNfmStartTimerValue;
      vgNfmData_p->remainingNfmBackOffTimerValue = sig_p->remainingNfmBackOffTimerValue;

      for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
      {
         if ((isEntityActive (profileEntity)) &&
           (getProfileValue (profileEntity, PROF_NFM) == VG_NFM_UNSOL_ENABLE))
         {
              vgPutNewLine (profileEntity);
              vgPrintf (profileEntity, (const Char *)"*MNFM: %d,%d",
                         vgNfmData_p->remainingNfmStartTimerValue,
                         vgNfmData_p->remainingNfmBackOffTimerValue);
              vgPutNewLine (profileEntity);

              vgFlushBuffer (profileEntity);
         }
      }
   }
#endif

    sleepManContext_p->needNetworkStateInd = FALSE;
    RvWakeUpCompleteCheck();

} /* vgSigApexMmNetworkStateInd */

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexMmNetworkInfoInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_MM_NETWORK_INFO_IND
 *
 * Returns:     Nothing
 *
 * Description: Sets a flag to indicate that this signal has been received.
 *              The command AT*MLTS requires a time string which is constructed
 *              from the information in this signal.
 *              Encoded as TP-Service-Centre-Time-Stamp
 *              (3GPP TS 03.40 version 7.5.0 - 9.2.3.11)
 *-------------------------------------------------------------------------*/

void vgSigApexMmNetworkInfoInd (const SignalBuffer *signalBuffer,
                                 const VgmuxChannelNumber entity)
{
    ApexMmNetworkInfoInd   *sig_p             = &signalBuffer->sig->apexMmNetworkInfoInd;
    MobilityContext_t      *mobilityContext_p = ptrToMobilityContext ();
    RtcDateAndTime          rtcTime;
#if defined(HAL_RTC_MODULE_ENABLED)
    hal_rtc_time_t          rtc_time;

#endif
    Int8                    tIndex;
    Int8                    timeZone;
    Char                    networkName[MAX_NETWORK_NAME_AND_DCS_LENGTH + NULL_TERMINATOR_LENGTH];
    Char                    tempString[LSAID_STRING_LENGTH + COMMA_AND_DOUBLE_QOUTATION_LENGTH+ NULL_TERMINATOR_LENGTH];
    VgmuxChannelNumber      profileEntity = 0;
    Char                    tempTimeZone[2];
    signed char             tempTime=0;

	#if 0
    static bool sync_time = TRUE;
	if(sync_time==FALSE)
	return;
	sync_time==FALSE;
	#endif
	
    /* if time information is present then generate the CLTS string */
    if (sig_p->uniTimeAndLocTimeZonePresent == TRUE)
    {
        /* check sign bit of time zone */
        if (((sig_p->uniTimeAndLocTimeZone.locTimeZone & 0x08) == 0) ||
            ((sig_p->uniTimeAndLocTimeZone.locTimeZone & 0xF7) == 0))
        {
            mobilityContext_p->newTimeZone.format = RTC_DISP_FORMAT_POS;
        }
        else
        {
            mobilityContext_p->newTimeZone.format = RTC_DISP_FORMAT_NEG;
        }        
         /* remove sign bit from time zone octet */
        timeZone = SEMI_OCTET_INT8_DEC(0xF7 & sig_p->uniTimeAndLocTimeZone.locTimeZone)*15/60;
#if defined(HAL_RTC_MODULE_ENABLED)
        convertToMltsTime( &rtcTime, &rtc_time,sig_p);
        if(rtcTime.timeZone.format == RTC_DISP_FORMAT_POS)
        {
            tempTimeZone[0] ='+';
            tempTime = SEMI_OCTET_INT8_DEC(0xF7 & sig_p->uniTimeAndLocTimeZone.locTimeZone);
        }
        else
        {
            tempTimeZone[0] ='-';      
            tempTime = 0 - SEMI_OCTET_INT8_DEC(0xF7 & sig_p->uniTimeAndLocTimeZone.locTimeZone);
        }
        tempTimeZone[1] = timeZone;
        hal_rtc_set_quarter_hour(tempTime);
		
        hal_rtc_set_utc_time(&rtc_time);
		
        
        
        for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
        {   
            if(isEntityActive (profileEntity))
            {
                switch(getProfileValue (profileEntity, PROF_CTZR))
                {
                   case 1:
                   {
                       vgPutNewLine (profileEntity);   
                       vgPrintf (profileEntity, (const Char *)"+CTZV:%+d",tempTime); 
                       vgPutNewLine (profileEntity);  
                       vgFlushBuffer( profileEntity);
                       break;
                   }
                   case 2:
                   {
                       vgPutNewLine (profileEntity);
                       if(rtc_time.rtc_year < 100 && rtc_time.rtc_year >9)
                       {
                           vgPrintf (profileEntity, (const Char *)"+CTZE:%+d,%d,20%d/%d/%d,%d:%d:%d"
                           ,tempTime,sig_p->networkDaylightSavingTime,rtc_time.rtc_year,rtc_time.rtc_mon,rtc_time.rtc_day,rtc_time.rtc_hour,rtc_time.rtc_min,rtc_time.rtc_sec);
                       }
                       else if(rtc_time.rtc_year <= 9)
                       {
                           vgPrintf (profileEntity, (const Char *)"+CTZE:%+d,%d,200%d/%d/%d,%d:%d:%d"
                           ,tempTime,sig_p->networkDaylightSavingTime,rtc_time.rtc_year,rtc_time.rtc_mon,rtc_time.rtc_day,rtc_time.rtc_hour,rtc_time.rtc_min,rtc_time.rtc_sec);      
                       }
                       else
                       {
                           vgPrintf (profileEntity, (const Char *)"+CTZE:%+d,%d,2%d/%d/%d,%d:%d:%d"
                           ,tempTime,sig_p->networkDaylightSavingTime,rtc_time.rtc_year,rtc_time.rtc_mon,rtc_time.rtc_day,rtc_time.rtc_hour,rtc_time.rtc_min,rtc_time.rtc_sec);      
                       } 
                       vgPutNewLine (profileEntity);   
                       vgFlushBuffer( profileEntity);
                       break;
                   }
                   default:
                    break;
                }
            }
        }
#else
        convertToMltsTime( &rtcTime,sig_p);
#endif
        if( rtcTime.daylightSaving == RTC_DAYLIGHT_SAVING_NONE)
        {
            /* <dst> not present */
            snprintf( (char *)mobilityContext_p->vgMLTSString,
                (MAX_MLTS_STRING_LENGTH),
                "\"%02d/%02d/%02d,%02d:%02d:%02d%c%02d\"",
                rtcTime.date.year,
                rtcTime.date.month,
                rtcTime.date.day,
                rtcTime.time.hours,
                rtcTime.time.minutes,
                rtcTime.time.seconds,
                (rtcTime.timeZone.format == RTC_DISP_FORMAT_POS ? '+' : '-'),
                timeZone);
        }
        else
        {
            snprintf( (char *)mobilityContext_p->vgMLTSString,
                (MAX_MLTS_STRING_LENGTH),
                "\"%02d/%02d/%02d,%02d:%02d:%02d%c%02d\", \"DST +%c in use\"",
                rtcTime.date.year,
                rtcTime.date.month,
                rtcTime.date.day,
                rtcTime.time.hours,
                rtcTime.time.minutes,
                rtcTime.time.seconds,
                (rtcTime.timeZone.format == RTC_DISP_FORMAT_POS ? '+' : '-'),
                timeZone,
                (rtcTime.daylightSaving == RTC_DAYLIGHT_SAVING_ONE_HR ? '1' : '2') );
        }

        /* <Full name> */
        if(sig_p->fullNetworkNamePresent)
        {
            if(rtcTime.daylightSaving == RTC_DAYLIGHT_SAVING_NONE)
            {
                /* need to put a comma for the lack of the  previous <dst> info */
                strncat((char *)mobilityContext_p->vgMLTSString, ",",
                    strlen(","));
            }
            GetNameworkNameString(sig_p->fullNetworkName, networkName);
            strncat((char *)mobilityContext_p->vgMLTSString, (char *)networkName,
                strlen((char *)networkName));
        }

        /* <Short name> */
        if(sig_p->shortNetworkNamePresent)
        {
            if(sig_p->fullNetworkNamePresent == FALSE)
            {
                if(rtcTime.daylightSaving == RTC_DAYLIGHT_SAVING_NONE)
                {
                    /* lack of <dst> <Full name> <dcs> info */
                    strncat((char *)mobilityContext_p->vgMLTSString, ",,,",
                        strlen(",,,"));
                }
                else
                {   /* lack of <Full name> <dcs>info */
                    strncat((char *)mobilityContext_p->vgMLTSString, ",,",
                        strlen(",,"));
                }
            }

            GetNameworkNameString(sig_p->shortNetworkName, networkName);
            strncat((char *)mobilityContext_p->vgMLTSString, (char *)networkName,
                strlen((char *)networkName));
        }

        /* <Local Time Zone> */
        if(sig_p->locTimeZonePresent)
        {
            if(sig_p->shortNetworkNamePresent == FALSE)
            {
                if(sig_p->fullNetworkNamePresent == FALSE)
                {
                    if(rtcTime.daylightSaving == RTC_DAYLIGHT_SAVING_NONE)
                    {
                        /* lack of <dts>  <Full name> <dcs> <Short name> <dcs> info */
                        strncat((char *)mobilityContext_p->vgMLTSString, ",,,,,",
                            strlen(",,,,,"));
                    }
                    else
                    {
                        /* <Full name> <dcs> <Short name> <dcs> info */
                        strncat((char *)mobilityContext_p->vgMLTSString, ",,,,",
                            strlen(",,,,"));
                    }
                }
                else
                {
                    /* lack <Short name> <dcs> info */
                    strncat((char *)mobilityContext_p->vgMLTSString, ",,",
                        strlen(",,"));
                }
            }
            timeZone = SEMI_OCTET_INT8_DEC(0xF7 & sig_p->locTimeZone)*15/60;
            snprintf( (char *)tempString, 10, ",\"+%02d\"", timeZone);
            strncat((char *)mobilityContext_p->vgMLTSString, (char *)tempString,
                strlen((char *)tempString));
        }

        /* <LSA Identity> */
        if(sig_p->lsaIdentityPresent)
        {
            if(sig_p->locTimeZonePresent == FALSE)
            {
                if(sig_p->shortNetworkNamePresent == FALSE)
                {
                    if(sig_p->fullNetworkNamePresent == FALSE)
                    {
                        if(rtcTime.daylightSaving == RTC_DAYLIGHT_SAVING_NONE)
                        {
                            /* lack of <tds>  <Full name> <dcs> <Short name> <dsc>
                             * <Local Time Zone> info */
                            strncat((char *)mobilityContext_p->vgMLTSString, ",,,,,,",
                                strlen(",,,,,,"));
                        }
                        else
                        {
                            /* lack of <Full name> <dcs> <Short name> <dcs> <Local Time Zone> info */
                            strncat((char *)mobilityContext_p->vgMLTSString, ",,,,,",
                                strlen(",,,,,"));
                        }
                    }
                    else
                    {
                        /* lack of <Short name> <dcs> <Local Time Zone> info */
                        strncat((char *)mobilityContext_p->vgMLTSString, ",,,",
                            strlen(",,,"));
                    }
                }
                else
                {
                    /* lack of <Local Time Zone> info */
                    strncat((char *)mobilityContext_p->vgMLTSString, ",",
                        strlen(","));
                }
            }

            /* lsaIdentity occur 3 octets */
            snprintf( (char *)tempString, 10, ",\"");
            vgOp32BitHex(sig_p->lsaIdentity, 3, (Char*)&tempString[2]);
            tempString[8] = '\"';
            tempString[9] = NULL_CHAR;
            strncat((char *)mobilityContext_p->vgMLTSString, (char *)tempString,
                strlen((char *)tempString));
        }

        for(    profileEntity = 0;
                profileEntity < CI_MAX_ENTITIES;
                profileEntity++)
        {
            /* output MLTS string if profile is set for this entity */
            if( (isEntityActive(profileEntity) == TRUE) &&
                (getProfileValue(profileEntity, PROF_MLTS) == REPORTING_ENABLED))
            {
                vgPutNewLine( profileEntity);

                vgPrintf(   profileEntity,
                            (const Char*)"*MLTS: %s",
                            mobilityContext_p->vgMLTSString);

                vgPutNewLine( profileEntity);
                vgFlushBuffer( profileEntity);
            }
        }

        if (getProfileValue (entity, PROF_CTZU) == REPORTING_ENABLED)
        {
            /* calculate hours and minutes offset time time zone */
            mobilityContext_p->newTimeZone.offset.hours = 0;

            for (tIndex = 0; tIndex < 12; tIndex++)
            {
                if (timeZone >= VG_TIMEZONE_HOUR_DIVISOR)
                {
                    mobilityContext_p->newTimeZone.offset.hours += 1;
                    timeZone -= VG_TIMEZONE_HOUR_DIVISOR;
                }
                else
                {
                    break;
                }
            }
            /* remainder */
            mobilityContext_p->newTimeZone.offset.minutes = timeZone * VG_TIMEZONE_NB_MINUTE_PER_UNIT;
            mobilityContext_p->newTimeZone.offset.seconds = 0;

            /* need to compare NITZ time zone to local time */
            vgSigAclkReadTimeZoneReq (entity);
        }
    }
} /* vgSigApexMmNetworkInfoInd */

/*************************************************************************
*
* Function:     vgApexMmReadBandModeCnf
*
* Scope:        Global
*
* Parameters:   in: commandBuffer_p - the AT command line string
*
* Returns:      void
*
* Description:  Band mode read req confirmation handler.
*
*************************************************************************/

void vgApexMmReadBandModeCnf (const SignalBuffer       *signalBuffer,
                              const VgmuxChannelNumber entity)
{
    ApexMmReadBandModeCnf *sig_p                = &signalBuffer->sig->apexMmReadBandModeCnf;

    MobilityContext_t     *mobilityContext_p    = ptrToMobilityContext ();
    VgCOPSData            *vgCOPSData           = &(mobilityContext_p->vgCOPSData);
    SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();

    if (sig_p->listIsValid == FALSE)
    {
        setResultCode (entity, RESULT_CODE_ERROR);
    }
    else
    {
        switch (getCommandId (entity))
        {
            case VG_AT_MM_SYSCONFIG:
            {
              /* placeholder for SYSCONFIG info when defined for NB-IOT */
              VgSYSCONFIGData         *vgSYSCONFIGData_p = &mobilityContext_p->vgSYSCONFIGData;
              VgSYSCONFIGParam        *currentParam_p = &vgSYSCONFIGData_p->currentParam;

              currentParam_p->mode = SYSCONFIG_MODE_AUTO;

              /* Service type can only be PS only for NB-IOT */
              switch(sig_p->serviceType)
              {
                case GPRS_SERVICE:
                  currentParam_p->srvDomain = DOMAIN_PS_ONLY;
                  break;
                case NO_SERVICES_AVAILABLE:
                default:
                  break;
              }

              if( vgSYSCONFIGData_p->read)
              {
                vgPutNewLine (entity);
                vgPrintf(entity, (const Char *)"^SYSCONFIG: 2,0,%d,%d", currentParam_p->roamSupport, currentParam_p->srvDomain);
                vgPutNewLine (entity);
                setResultCode (entity, RESULT_CODE_OK);
              }
              else
              {
                VgSYSCONFIGParam        *newParam_p = &vgSYSCONFIGData_p->newParam;

                if((currentParam_p->roamSupport != newParam_p->roamSupport)&&
                   (newParam_p->roamSupport != ROAMING_NO_MODIFICATION))
                {
                  if(newParam_p->roamSupport)
                  {
                    vgCOPSData->mode = VG_OP_AUTOMATIC_MODE;
                  }
                  else
                  {
                    VgSimInfo                *simInfo = &(simLockGenericContext_p->simInfo);

                    vgCOPSData->mode = VG_OP_MANUAL_OPERATOR_SELECTION;
                    vgCOPSData->selectedPlmn.plmn.mcc =  simInfo->hplmn.plmn.mcc;
                    vgCOPSData->selectedPlmn.plmn.mnc =  simInfo->hplmn.plmn.mnc;
                  }
                  setResultCode (entity, vgChManContinueAction (entity, SIG_APEX_MM_PLMN_SELECT_REQ));
                }
                else
                {
                  setResultCode(entity, RESULT_CODE_OK);
                }
              }
              break;
            }

#if defined (UPGRADE_3G)
# if defined(UPGRADE_MTNET)
            case VG_AT_MM_RESET:
            {
              (void) vgChManContinueAction (entity, SIG_APEX_MM_PLMNLIST_REQ);
              break;
            }
# endif
#endif
            case VG_AT_MM_MBAND:
            {
                if (NBIOT_BAND_INVALID == sig_p->band)
                {
                    /*If no currently band, retured OK only*/
                    setResultCode(entity, RESULT_CODE_OK);
                }
                else
                {
                    vgPutNewLine (entity);
                    vgPrintf(entity, (const Char *)"*MBAND: %d", sig_p->band);
                    vgPutNewLine (entity);
                    setResultCode (entity, RESULT_CODE_OK);
                }
                    
            }
            break;
            
            case VG_AT_GN_MABORT:
            {
                // do nothing
                break;
            }
            default:
            {
              /* Illegal command for SIG_APEX_MM_READ_BAND_MODE_CNF */
              FatalParam(entity, getCommandId (entity), 0);
              break;
            }
        } /* switch */
    }
} /* vgApexMmReadBandModeCnf */

/*************************************************************************
*
* Function:     vgApexMmWritePwonOptionsCnf
*
* Scope:        Global
*
* Parameters:   signalBuffer       - structure containing signal:
*                                    SIG_APEX_MM_WRITE_PWON_OPTIONS_CNF
*               entity             - mux channel number
*
* Returns:      void
*
* Description:  Power On Options write req confirmation handler.
*
*************************************************************************/
void vgApexMmWritePwonOptionsCnf (const SignalBuffer       *signalBuffer,
                                  const VgmuxChannelNumber entity)
{
    ApexMmWritePwonOptionsCnf *sig_p = &signalBuffer->sig->apexMmWritePwonOptionsCnf;

    if (sig_p->requestStatus == MM_REQ_OK)
    {
        setResultCode (entity, RESULT_CODE_OK);
    }
    else
    {
        setResultCode (entity, RESULT_CODE_ERROR);
    }
} /* vgApexMmWritePwonOptionsCnf */

/*************************************************************************
*
* Function:     vgApexMmReadPwonOptionsCnf
*
* Scope:        Global
*
* Parameters:   signalBuffer       - structure containing signal:
*                                    SIG_APEX_MM_READ_PWON_OPTIONS_CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  Power On Options read req confirmation handler.
*
*************************************************************************/

void vgApexMmReadPwonOptionsCnf (const SignalBuffer       *signalBuffer,
                                 const VgmuxChannelNumber entity)
{
    ApexMmReadPwonOptionsCnf *sig_p = &signalBuffer->sig->apexMmReadPwonOptionsCnf;

    if ( sig_p->requestStatus == MM_REQ_OK)
    {
        vgPutNewLine (entity);
        vgPrintf (entity, (const Char *)"*MGATTCFG: %d", sig_p->gaOption);
        vgPutNewLine (entity);
        setResultCode (entity, RESULT_CODE_OK);
    }
    else
    {
        setResultCode (entity, RESULT_CODE_ERROR);
    }
} /* vgApexMmReadPwonOptionsCnf */

#if defined(UPGRADE_MTNET)

/*************************************************************************
*
* Function:     vgApexMmSuspendCnf
*
* Scope:        Global
*
* Parameters:   signalBuffer       - structure containing signal:
*                                    SIG_APEX_MM_SUSPEND_CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  .
*
*************************************************************************/

void vgApexMmSuspendCnf (const SignalBuffer       *signalBuffer,
                             const VgmuxChannelNumber entity)
{
    ApexMmSuspendCnf *sig_p = &signalBuffer->sig->apexMmSuspendCnf;
    SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();

    switch (getCommandId (entity))
    {
        case VG_AT_MM_OFF:
        {
            if(MM_REQ_OK == sig_p->requestStatus)
            {
                /* protocol stack is suspended */
                simLockGenericContext_p->powerUpProtoStack = FALSE;
                setResultCode (entity, RESULT_CODE_OK);
            }
            else
            {
                setResultCode (entity, RESULT_CODE_ERROR);
            }
            break;
        }  /* VG_AT_MM_OFF */

        case VG_AT_MM_RESET:
        {
            if(MM_REQ_OK == sig_p->requestStatus)
            {
                /* register */
                (void)vgChManContinueAction (entity, SIG_APEX_MM_RESUME_REQ);
            }
            else
            {
                setResultCode (entity, RESULT_CODE_ERROR);
            }
            break;
        } /* VG_AT_MM_RESET */

        default:
        {
            /* Illegal command for SIG_APEX_MM_SUSPEND_CNF */
            FatalParam(entity, getCommandId (entity), 0);
            break;
        }
    }

} /* vgApexMmSuspendCnf */


/*************************************************************************
*
* Function:     vgApexMmResumeCnf
*
* Scope:        Global
*
* Parameters:   signalBuffer       - structure containing signal:
*                                    SIG_APEX_MM_RESUME_CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  .
*
*************************************************************************/

void vgApexMmResumeCnf (const SignalBuffer       *signalBuffer,
                             const VgmuxChannelNumber entity)
{
    ApexMmResumeCnf *sig_p = &signalBuffer->sig->apexMmResumeCnf;
    MobilityContext_t       *mobilityContext_p  = ptrToMobilityContext ();
    SignalBuffer             cResetSig                   = kiNullBuffer;
    CiResetInd             *ciResetInd_p              = PNULL;

    FatalCheck(mobilityContext_p != PNULL, entity, 0, 0);

    if(MM_REQ_OK == sig_p->requestStatus)
    {
        setResultCode (entity, RESULT_CODE_OK);

         /* send signal internally to CI to inform other sub-systems of the modified
           * SIM state */
        KiCreateZeroSignal( SIG_CI_RESET_IND, sizeof(CiResetInd), &cResetSig);
        ciResetInd_p = (CiResetInd *)cResetSig.sig;
        ciResetInd_p->entity = entity;
        KiSendSignal (VG_CI_TASK_ID, &cResetSig);


        /* Request the Multiplexer top resend its low power vote since */
        /* layer 1 has just lost it */

        /* we need to update the CNMI state and Message Service again
          * since the protocol stack has lost it! */
        vgApexSendSmProfileChangedReq(entity);

    }
    else
    {
        setResultCode (entity, RESULT_CODE_ERROR);
    }

} /* vgApexMmResumeCnf */
#endif  /* #if defined(UPGRADE_MTNET) */

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexMmCsconInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_MM_CSCON_IND
 * Returns:     nothing
 *
 * Description: receives EMM connect status indicater
 *----------------------------------------------------------------------------*/

void vgSigApexMmCsconInd (const SignalBuffer *signalBuffer)
{
    ApexMmCsconInd    *sig_p              = &signalBuffer->sig->apexMmCsconInd;
    MobilityContext_t *mobilityContext_p  = ptrToMobilityContext ();
    VgmuxChannelNumber profileEntity     = 0;

    if(mobilityContext_p->vgCSCONData.SigConMode != sig_p->status)
    {
      /* received EMM connect status  indication */
      mobilityContext_p->vgCSCONData.SigConMode = sig_p->status;
   
      for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
      {
        if (isEntityActive (profileEntity))
        {
          if (VG_CSCON_ENABLED == mobilityContext_p->vgCSCONData.rptCfgOp)
          {
              viewCSCON(profileEntity, FALSE);
          }
        }
      }
    }
}/*vgSigApexMmCsconInd*/

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexMmPsmStatusInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_MM_PSM_STATUS_IND
 * Returns:     nothing
 *
 * Description: receives PSM status indication
 *----------------------------------------------------------------------------*/

void vgSigApexMmPsmStatusInd (const SignalBuffer *signalBuffer)
{
    ApexMmPsmStatusInd    *sig_p              = &signalBuffer->sig->apexMmPsmStatusInd;
    VgmuxChannelNumber profileEntity     = 0;

    for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
    {
      if (isEntityActive (profileEntity))
      {
        if (VG_MNBIOTEVENT_PSM_STATE == ((VG_MNBIOTEVENT_PSM_STATE) & (getProfileValue(profileEntity, PROF_MNBIOTEVENT))))
        {
          vgPutNewLine (profileEntity);

          if(sig_p->psmStatus == MM_ENTER_PSM_STATE) 
          {
            vgPrintf (profileEntity,
                (const Char *)"*MNBIOTEVENT: \"ENTER PSM\"");
          }
          else
          {
            vgPrintf (profileEntity,
                (const Char *)"*MNBIOTEVENT: \"EXIT PSM\"");
          }
          
          vgPutNewLine (profileEntity);
          vgFlushBuffer(profileEntity);
        }
      }
    }
}/*vgSigApexMmPsmStatusInd*/

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigApexMmOosaStatusInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_MM_OOSA_STATUS_IND
 * Returns:     nothing
 *
 * Description: receives OOSA status indication
 *----------------------------------------------------------------------------*/

void vgSigApexMmOosaStatusInd (const SignalBuffer *signalBuffer)
{
    ApexMmOosaStatusInd *sig_p            = &signalBuffer->sig->apexMmOosaStatusInd;
    MobilityContext_t *mobilityContext_p  = ptrToMobilityContext ();
    VgmuxChannelNumber profileEntity     = 0;

    if(mobilityContext_p->vgMoosaIndData.oosaStatus != sig_p->oosaStatus)
    {
      /* received EMM connect status  indication */
      mobilityContext_p->vgMoosaIndData.oosaStatus = sig_p->oosaStatus;
   
      for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
      {
        if ((isEntityActive (profileEntity)) && (getProfileValue(profileEntity, PROF_MOOSAIND)))
        {
            viewMOOSAIND(profileEntity, FALSE);
        }
      }
    }
}/*vgSigApexMmCsconInd*/

/*************************************************************************
*
* Function:     vgApexMmLockArfcnCnf
*
* Scope:        Global
*
* Parameters:   in: commandBuffer_p - the AT command line string
*
* Returns:      void
*
* Description:  lock frequency or cell response  handler.
*
*************************************************************************/

void vgApexMmLockArfcnCnf (const SignalBuffer       *signalBuffer,
                                   const VgmuxChannelNumber entity)
{
    ApexMmLockArfcnCnf    *sig_p = &signalBuffer->sig->apexMmLockArfcnCnf;
    VgMFRCLLCKData       *vgMFRCLLCKData   = &(ptrToMobilityContext()->vgMfrcllckData);

    if (MM_REQ_OK != sig_p->writeResult)
    {
        setResultCode (entity, RESULT_CODE_ERROR);
    }
    else
    {
        switch (getCommandId (entity))
        {
            case VG_AT_MM_MFRCLLCK:
            {
                if(vgMFRCLLCKData->lockArfcnCmdMode == MM_LOCK_ARFCN_CMD_MODE_SET)
                {
                  setResultCode (entity, RESULT_CODE_OK);
                }
                else
                {
                  vgPutNewLine (entity);

                  vgPrintf (entity,
                          (const Char *)"*MFRCLLCK: %d",
                          sig_p->lockInfoPresent);
                  
                  /* Return lock arfcn info */
                  if(sig_p->lockInfoPresent)
                  {
                    vgPrintf (entity,
                            (const Char *)",%d,%d",
                            sig_p->lockInfo.earfcn,
                            sig_p->lockInfo.earfcnOffset);

                    if(sig_p->lockInfo.pciPresent)
                    {
                      vgPrintf (entity,
                              (const Char *)",%d",
                              sig_p->lockInfo.pci);
                    }
                  }
                  vgPutNewLine (entity);
                  
                  setResultCode (entity, RESULT_CODE_OK);
                }
            }
            break;
            default:
            {
              /* Illegal command for SIG_APEX_MM_LOCK_ARFCN_CNF */
              FatalParam(entity, getCommandId (entity), 0);
              break;
            }
        } /* switch */
    }
}/*vgApexMmLockArfcnCnf*/
/*************************************************************************
*
* Function:     vgApexMmSearchBandListCnf
*
* Scope:        Global
*
* Parameters:   in: commandBuffer_p - the AT command line string
*
* Returns:      void
*
* Description: seatch Band List response  handler.
*
*************************************************************************/

void vgApexMmSearchBandListCnf (const SignalBuffer       *signalBuffer,
                                   const VgmuxChannelNumber entity)
{
    ApexMmSearchBandListCnf    *sig_p = &signalBuffer->sig->apexMmSearchBandListCnf;

    if (!sig_p->result)
    {
        setResultCode (entity, RESULT_CODE_ERROR);
    }
    else
    {
        switch (getCommandId (entity))
        {
            case VG_AT_MM_MBANDSL:
            {
                setResultCode (entity, RESULT_CODE_OK);
            }
            break;
            default:
            {
              /* Illegal command for SIG_APEX_MM_SEARCH_BAND_LIST_CNF */
              FatalParam(entity, getCommandId (entity), 0);
              break;
            }
        } /* switch */
    }
}/*vgApexMmSearchBandListCnf*/

/*************************************************************************
*
* Function:     vgApexMmUeStatsCnf
*
* Scope:        Global
*
* Parameters:   in: SignalBuffer   - structure containing signal:
*                                    SIG_APEX_MM_UE_STATS_CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  handler ue status response message.
*
*************************************************************************/

void vgApexMmUeStatsCnf (const SignalBuffer       *signalBuffer,
                              const VgmuxChannelNumber entity)
{
    ApexMmUeStatsCnf        *sig_p              = &signalBuffer->sig->apexMmUeStatsCnf;
    MobilityContext_t       *mobilityContext_p  = ptrToMobilityContext ();
    VgMENGINFOStatisData    *mengInfoData_p     = &(mobilityContext_p->vgMengInfoData);
    ResultCode_t result;
     
    if (VG_AT_MM_MENGINFO == getCommandId (entity))
    {
        /*query Mode:0, serving and neighbor cell info*/
        if (VG_MENGINFO_QUERY_MODEM_INFO_RADIO_INFO == mengInfoData_p->queryMode)
        {   /*serving cell info present*/
            if (TRUE == sig_p->cellInfoPresent)
            {   
                /*update serving cell info*/
                mengInfoData_p->curSrvCell.cellInfoValid = TRUE;

                memcpy(&(mengInfoData_p->curSrvCell.cellInfo),
                    &(sig_p->cellInfo),
                    sizeof(ServingCellRadioInfo));
            
                /*neighbour cell info present, and update info of atci*/
                if (VG_MENGINFO_NB_CELL_INFO_INVALID < sig_p->neighbourCellRadioInfoNum)
                {   
                    /*update serving cell info*/
                    mengInfoData_p->neighbourCell.neighbourCellinfoNumber = sig_p->neighbourCellRadioInfoNum;

                    memcpy(&(mengInfoData_p->neighbourCell.neighbourCellRadioInfoList),
                        &(sig_p->neighbourCellRadioInfo),
                        sizeof(NeighbourCellRadioInfo) * VG_MENGINFO_NB_CELL_NUMBER);
                }
                else
                {
                    mengInfoData_p->neighbourCell.neighbourCellinfoNumber = VG_MENGINFO_NB_CELL_INFO_INVALID;
                }
                
                /*SEND at cmd response to ap */
                viewMENGINFORadioCellInfo(entity);
                
                setResultCode (entity, RESULT_CODE_OK);
            }
            else
            {
                mengInfoData_p->curSrvCell.cellInfoValid = FALSE;
                /* Return current state in error cause */
                result = vgGetResultFromErrcState(sig_p->errcStateEngInfo);
                setResultCode(entity, result);
            }
        }
        else if (VG_MENGINFO_QUERY_MODEM_INFO_DATA_TRANSFER_INFO == mengInfoData_p->queryMode)
        {
            /*datatransfer info present, and update info of atci*/
            if (TRUE == sig_p->dataTransferInfoPresent)
            {   
                /*update serving cell info*/
                mengInfoData_p->dataTransfer.dataTransferInfoValid = TRUE;

                memcpy(&(mengInfoData_p->dataTransfer.dataTransferInfo),
                    &(sig_p->dataTransferInfo),
                    sizeof(DataTransferInfo));

                viewMENGINFODataTransferInfo(entity);  
                
                setResultCode (entity, RESULT_CODE_OK);
            }
            else
            {
                mengInfoData_p->dataTransfer.dataTransferInfoValid = FALSE;
                
                /* Return current state in error cause */
                result = vgGetResultFromErrcState(sig_p->errcStateEngInfo);
                setResultCode(entity, result);                
            }
            
        }
        else
        {
            setResultCode (entity, RESULT_CODE_OK);
        }
    }
    else
    {
        /* Illegal command for SIG_APEX_MM_CSCON_CNF */
        FatalParam(entity, getCommandId (entity), 0);
    }


}/*vgApexMmUeStatsCnf*/

/*************************************************************************
*
* Function:     vgApexMmLocCellInfoCnf
*
* Scope:        Global
*
* Parameters:   in: SignalBuffer   - structure containing signal:
*                                    SIG_APEX_MM_LOC_CELL_INFO_CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  handler ue status response message.
*
*************************************************************************/

void vgApexMmLocCellInfoCnf (const SignalBuffer       *signalBuffer,
                              const VgmuxChannelNumber entity)
{
    ApexMmLocCellInfoCnf    *sig_p              = &signalBuffer->sig->apexMmLocCellInfoCnf;
    ResultCode_t result;
    Int8         i;      

    if (VG_AT_MM_MCELLINFO == getCommandId (entity))
    {
        /*serving cell information is valild*/
        if (TRUE == sig_p->servingCellInfoPresent)
        {
            vgPutNewLine (entity);
    
            /*print: <sc_earfcn>,<sc_earfcn_offset>,<sc_pci>,<sc_rsrp>,<sc_rsrq>,<sc_rssi>,<sc_sinr> */
            vgPrintf (entity,
                    (const Char *)"*MCELLINFOSC: %d,%d,%d,%d,%d,%d,%d,",
                    sig_p->servingCellInfo.earfcn,
                    sig_p->servingCellInfo.earfcnOffset,
                    sig_p->servingCellInfo.pci,
                    sig_p->servingCellInfo.rsrp,
                    sig_p->servingCellInfo.rsrq,
                    sig_p->servingCellInfo.rssi,
                    sig_p->servingCellInfo.snr);
            /*print: <sc_mcc>   string type hex format*/
            vgPrintf (entity,
                    (const Char *)"\"%03X\",",
                    sig_p->servingCellInfo.plmn.mcc);
            
            /*print: <sc_mnc>   string type hex format*/
            if(TRUE == sig_p->servingCellInfo.plmn.is_three_digit_mnc)
            {
              vgPrintf (entity,
                      (const Char *)"\"%03X\",",
                      sig_p->servingCellInfo.plmn.mnc);
            }
            else
            {
              vgPrintf (entity,
                      (const Char *)"\"%02X\",",
                      sig_p->servingCellInfo.plmn.mnc);
            }
        
            /*print: <sc_tac>,<sc_cellid>*/
            vgPrintf (entity,
                    (const Char *)"\"%04X\",\"%X\"",
                    sig_p->servingCellInfo.tac,
                    sig_p->servingCellInfo.cellIdentity);
            
            vgPutNewLine (entity);
            
            /* continue check neighbour cell info */
            for (i = 0; i < sig_p->neighbourCellInfoNum; i++)
            {
                /* MENINFONC:<nc_earfcn>,<nc_earfcn_offset>,<nc_pci>,<nc_rsrp>,<nc_rsrq>,<nc_rssi>,<nc_sinr> */
                vgPrintf (entity,
                        (const Char *)"*MCELLINFONC: %d,%d,%d,%d,%d,%d,%d,",
                        sig_p->neighbourCellInfo[i].earfcn,
                        sig_p->neighbourCellInfo[i].earfcnOffset,
                        sig_p->neighbourCellInfo[i].pci,
                        sig_p->neighbourCellInfo[i].rsrp,
                        sig_p->neighbourCellInfo[i].rsrq,
                        sig_p->neighbourCellInfo[i].rssi,
                        sig_p->neighbourCellInfo[i].snr);
            
                /*print: <nc_mcc>   string type hex format*/
                vgPrintf (entity,
                        (const Char *)"\"%03X\",",
                        sig_p->neighbourCellInfo[i].plmn.mcc);
                
                /*print: <nc_mnc>   string type hex format*/
                if(TRUE == sig_p->neighbourCellInfo[i].plmn.is_three_digit_mnc)
                {
                  vgPrintf (entity,
                          (const Char *)"\"%03X\",",
                          sig_p->neighbourCellInfo[i].plmn.mnc);
                }
                else
                {
                  vgPrintf (entity,
                          (const Char *)"\"%02X\",",
                          sig_p->neighbourCellInfo[i].plmn.mnc);
                }
            
                /*print: <nc_tac>,<nc_cellid>*/
                vgPrintf (entity,
                        (const Char *)"\"%04X\",\"%X\"",
                        sig_p->neighbourCellInfo[i].tac,
                        sig_p->neighbourCellInfo[i].cellIdentity);
                
                vgPutNewLine (entity);
            }            
            setResultCode (entity, RESULT_CODE_OK);
        }   /*End: serving cell info valid*/
        else
        {
            /* Return current state in error cause */
            result = vgGetResultFromErrcState(sig_p->errcStateEngInfo);
            setResultCode(entity, result);
        }
    }
    else
    {
        /* Illegal command for SIG_APEX_MM_LOC_CELL_INFO_CNF */
        FatalParam(entity, getCommandId (entity), 0);
    }
}/*vgApexMmUeStatsCnf*/

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigAclkReadTimeZoneCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_DM_RTC_READ_TIME_ZONE_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Checks if time zone has changed
 *----------------------------------------------------------------------------*/
void vgSigAclkReadTimeZoneCnf (const SignalBuffer       *signalBuffer,
                               const VgmuxChannelNumber entity)
{
    DmRtcReadTimeZoneCnf *sig_p             = &signalBuffer->sig->dmRtcReadTimeZoneCnf;
    MobilityContext_t    *mobilityContext_p = ptrToMobilityContext ();

    PARAMETER_NOT_USED(entity);

    if (sig_p->status == RTC_STATUS_OK)
    {
        if (mobilityContext_p->timeZoneInitialised == TRUE)
        {
            /* check for changes to time zone */
            if ((sig_p->timeZone.format         != mobilityContext_p->newTimeZone.format) ||
                (sig_p->timeZone.offset.hours   != mobilityContext_p->newTimeZone.offset.hours) ||
                (sig_p->timeZone.offset.minutes != mobilityContext_p->newTimeZone.offset.minutes))
            {
                if (getProfileValue (entity, PROF_CTZU) == REPORTING_ENABLED)
                {
                    /* update time zone */
                    vgSigAclkSetTimeZoneReq (entity);
                }
            }
        }
        else
        {
            /* copy latest time zone information */
            memcpy (&mobilityContext_p->currentTimeZone,
                &sig_p->timeZone,
                sizeof(RtcDisplacement));

            mobilityContext_p->timeZoneInitialised = TRUE;
        }
    }
} /* vgSigAclkReadTimeZoneCnf */

/*----------------------------------------------------------------------------
 *
 * Function:    vgSigAclkSetTimeZoneCnf
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_DM_RTC_SET_TIME_ZONE_CNF
 *              VgmuxChannelNumber - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: Checks if time zone update was successful
 *----------------------------------------------------------------------------*/
void vgSigAclkSetTimeZoneCnf (const SignalBuffer       *signalBuffer,
                              const VgmuxChannelNumber entity)
{
    DmRtcSetTimeZoneCnf *sig_p = &signalBuffer->sig->dmRtcSetTimeZoneCnf;

    PARAMETER_NOT_USED(entity);

    if (sig_p->status != RTC_STATUS_OK)
    {
        /* NITZ time zone update failed */
        WarnParam(entity, 0, 0);
    }
} /* vgSigAclkSetTimeZoneCnf */

/*************************************************************************
*
* Function:     vgApexMmSetEdrxCnf
*
* Scope:        Global
*
* Parameters:   signalBuffer       - structure containing signal:
*                                    SIG_APEX_MM_SET_EDRX_CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  .
*
*************************************************************************/
void vgApexMmSetEdrxCnf (  const SignalBuffer       *signalBuffer,
                                        const VgmuxChannelNumber entity)

{
  ApexMmSetEdrxCnf *sig_p             = &signalBuffer->sig->apexMmSetEdrxCnf;
  MobilityContext_t     *mobilityContext_p = ptrToMobilityContext ();

  switch (getCommandId (entity))
  {
      case VG_AT_MM_CEDRXS:
      case VG_AT_MM_MEDRXCFG:
      {
         if (sig_p->status)
         {
           if ((!((mobilityContext_p->vgReqEdrxData.userEdrxSupport == TRUE) &&  (mobilityContext_p->vgReqEdrxData.userEdrxValue ==0)))
                && (mobilityContext_p->vgReqEdrxData.mode != VG_EDRX_MODE_DISABLE_EDRX_AND_RESET))
           {
               mobilityContext_p->vgEdrxData.userEdrxSupport = mobilityContext_p->vgReqEdrxData.userEdrxSupport;
               mobilityContext_p->vgEdrxData.userEdrxValue = mobilityContext_p->vgReqEdrxData.userEdrxValue;
               mobilityContext_p->vgEdrxData.userPtwPresence = mobilityContext_p->vgReqEdrxData.userPtwPresence;
               mobilityContext_p->vgEdrxData.userPagingTimeWindow = mobilityContext_p->vgReqEdrxData.userPtwValue;
               mobilityContext_p->vgEdrxData.mode = mobilityContext_p->vgReqEdrxData.mode;
               mobilityContext_p->vgEdrxData.userDataValid = TRUE;
               mobilityContext_p->vgEdrxData.nwDataValid = FALSE;
            }
            else
            {
                mobilityContext_p->vgEdrxData.userDataValid = FALSE;
            }
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
          /* Unexpected message. */
          FatalParam(entity, getCommandId (entity), 0);
      }
      break;
  }
}

/*************************************************************************
*
* Function:     vgApexMmReadEdrxCnf
*
* Scope:        Global
*
* Parameters:   signalBuffer       - structure containing signal:
*                                    SIG_APEX_MM_READ_EDRX_CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  .
*
*************************************************************************/
void vgApexMmReadEdrxCnf (  const SignalBuffer       *signalBuffer,
                                        const VgmuxChannelNumber entity)
{
  ApexMmReadEdrxCnf *sig_p             = &signalBuffer->sig->apexMmReadEdrxCnf;
  MobilityContext_t     *mobilityContext_p = ptrToMobilityContext ();
  Char                tempStr[VG_CEDRX_MAX_BIN_STR_LEN + NULL_TERMINATOR_LENGTH] = {0};

          mobilityContext_p->vgEdrxData.userEdrxSupport = sig_p->edrxInfo.userEdrxSupport;
          mobilityContext_p->vgEdrxData.userEdrxValue = sig_p->edrxInfo.userEdrxValue;
  mobilityContext_p->vgEdrxData.userPtwPresence = sig_p->edrxInfo.userPtwPresent;
  mobilityContext_p->vgEdrxData.userPagingTimeWindow = sig_p->edrxInfo.userPtw;
          mobilityContext_p->vgEdrxData.userDataValid = TRUE;
          mobilityContext_p->vgEdrxData.nwEdrxSupport = sig_p->edrxInfo.nwEdrxSupport;
          mobilityContext_p->vgEdrxData.nwEdrxValue = sig_p->edrxInfo.nwEdrxValue;
          mobilityContext_p->vgEdrxData.nwPagingTimeWindow = sig_p->edrxInfo.pagingTimeWindow;
          mobilityContext_p->vgEdrxData.nwDataValid = TRUE;

  switch (getCommandId (entity))
  {
      case VG_AT_MM_CEDRXS:
      {
          vgPutNewLine (entity);
          vgPrintf (entity,(const Char*)"+CEDRXS: %d",VG_CEDRX_ACT_NB_IOT);
          
          if ((mobilityContext_p->vgEdrxData.userEdrxSupport == FALSE ) &&
              (mobilityContext_p->vgEdrxData.userEdrxValue != EDRX_VALUE_0))
          {
              mobilityContext_p->vgEdrxData.userEdrxValue = EDRX_VALUE_0;
          }

          vgPrintf (entity, (const Char*)",\"");
          vgInt8ToBinString(mobilityContext_p->vgEdrxData.userEdrxValue, VG_CEDRX_MAX_BIN_STR_LEN, tempStr);
          vgPrintf (entity, (const Char*)tempStr);
          vgPrintf (entity, (const Char*)"\"");

          vgPutNewLine (entity);
          setResultCode (entity, RESULT_CODE_OK);
      }
      break;

      case VG_AT_MM_MEDRXCFG:
      {
          vgPutNewLine (entity);
          vgPrintf (entity,(const Char*)"*MEDRXCFG: %d",VG_CEDRX_ACT_NB_IOT);

          if (mobilityContext_p->vgEdrxData.userEdrxSupport == TRUE)
          {
             vgPrintf (entity, (const Char*)",\"");
             vgInt8ToBinString(mobilityContext_p->vgEdrxData.userEdrxValue, VG_CEDRX_MAX_BIN_STR_LEN, tempStr);
             vgPrintf (entity, (const Char*)tempStr);
             vgPrintf (entity, (const Char*)"\"");
         }
         if (mobilityContext_p->vgEdrxData.userPtwPresence == TRUE)
         {
            vgPrintf (entity, (const Char*)",\"");
            vgInt8ToBinString(mobilityContext_p->vgEdrxData.userPagingTimeWindow, VG_CEDRX_MAX_BIN_STR_LEN, tempStr);
            vgPrintf (entity, (const Char*)tempStr);
            vgPrintf (entity, (const Char*)"\"");
        }
         vgPutNewLine (entity);
         setResultCode (entity, RESULT_CODE_OK);
      }
      break;

      case VG_AT_MM_CEDRXRDP:
      {
          if (mobilityContext_p->vgEdrxData.nwEdrxSupport == TRUE)
          {
             vgPutNewLine (entity);
             vgPrintf (entity,(const Char*)"+CEDRXRDP: %d",VG_CEDRX_ACT_NB_IOT);

             if (mobilityContext_p->vgEdrxData.userEdrxSupport == TRUE)
             {
                vgPrintf (entity, (const Char*)",\"");
                vgInt8ToBinString(mobilityContext_p->vgEdrxData.userEdrxValue, VG_CEDRX_MAX_BIN_STR_LEN, tempStr);
                vgPrintf (entity, (const Char*)tempStr);
                vgPrintf (entity, (const Char*)"\",\"");
                vgInt8ToBinString(mobilityContext_p->vgEdrxData.nwEdrxValue, VG_CEDRX_MAX_BIN_STR_LEN, tempStr);
                vgPrintf (entity, (const Char*)tempStr);
                vgPrintf (entity, (const Char*)"\",\"");
                vgInt8ToBinString(mobilityContext_p->vgEdrxData.nwPagingTimeWindow, VG_CEDRX_MAX_BIN_STR_LEN, tempStr);
                vgPrintf (entity, (const Char*)tempStr);
                vgPrintf (entity, (const Char*)"\"");
             }
          }
          else
          { /* when nw edrx support not available we set ACT to 0 */
            vgPutNewLine (entity);
            vgPrintf (entity,(const Char*)"+CEDRXRDP: 0");
          }
          vgPutNewLine (entity);
          setResultCode (entity, RESULT_CODE_OK);

      }
      break;

     default:
     {
        /* Unexpected message. */
        FatalParam(entity, getCommandId (entity), 0);
     }
     break;
  }

}
/*************************************************************************
*
* Function:     vgApexWriteIotOptCfgCnf
*
* Scope:        Global
*
* Parameters:   signalBuffer       - structure containing signal:
*                                    SIG_APEX_WRITE_IOT_OPT_CFG_CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  .
*
*************************************************************************/

void vgApexWriteIotOptCfgCnf(  const SignalBuffer       *signalBuffer,
                                        const VgmuxChannelNumber entity)
{
  MobilityContext_t    *mobilityContext_p  = ptrToMobilityContext ();
  VgReqCciotoptData   *reqCciotoptData_p   = &(mobilityContext_p->vgReqCciotoptData);
  VgCciotoptData      *currentCciotData_p   = &(mobilityContext_p->vgCciotoptData);

  switch (getCommandId (entity))
  {
      case VG_AT_MM_CCIOTOPT:
      {

         if ((reqCciotoptData_p->prefUEOptPresent) && (reqCciotoptData_p->supportedUEOptPresent))
         {
           /* update the temporary store with this info  */
           currentCciotData_p->supportedUEOpt = reqCciotoptData_p->supportedUEOpt;
           currentCciotData_p->prefUEOpt = reqCciotoptData_p->prefUEOpt;
           currentCciotData_p->uEdataValid = TRUE;
         }
         else
         {
            /* changed one but not the other so we don't have valid data in temp store now */
            currentCciotData_p->uEdataValid = FALSE;
         }
         setResultCode (entity, RESULT_CODE_OK);
      }
      break;

      default:
      {
          /* Unexpected message. */
          FatalParam(entity, getCommandId (entity), 0);
      }
      break;
   }

}
/*************************************************************************
*
* Function:     vgApexReadIotOptCfgCnf
*
* Scope:        Global
*
* Parameters:   signalBuffer       - structure containing signal:
*                                     SIG_APEX_READ_IOT_OPT_CFG_CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  .
*
*************************************************************************/

void vgApexReadIotOptCfgCnf(  const SignalBuffer       *signalBuffer,
                                        const VgmuxChannelNumber entity)
{
    ApexReadIotOptCfgCnf *sig_p              = &signalBuffer->sig->apexReadIotOptCfgCnf;
    MobilityContext_t    *mobilityContext_p  = ptrToMobilityContext ();

    switch (getCommandId (entity))
    {
        case VG_AT_MM_CCIOTOPT:
        {
           mobilityContext_p->vgCciotoptData.prefUEOpt = sig_p->ciotPreference;
           mobilityContext_p->vgCciotoptData.supportedUEOpt = sig_p->supportedUeOptions;
           mobilityContext_p->vgCciotoptData.uEdataValid = TRUE;

           vgPutNewLine (entity);
           vgPrintf (entity,(const Char*)"+CCIOTOPT: %d,%d,%d", mobilityContext_p->vgCciotoptData.reportOpt,
           sig_p->ciotPreference,sig_p->supportedUeOptions);
           vgPutNewLine (entity);
           setResultCode (entity, RESULT_CODE_OK);
        }
        break;

        default:
        {
          /* Unexpected message. */
          FatalParam(entity, getCommandId (entity), 0);
        }
        break;
    }

}

/*************************************************************************
*
* Function:     vgApexWritePsmConfCnf
*
* Scope:        Global
*
* Parameters:   signalBuffer       - structure containing signal:
*                                     SIG_APEX_WRITE_PSM_CONF_CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  .
*
*************************************************************************/
void vgApexWritePsmConfCnf(  const SignalBuffer       *signalBuffer,
                                        const VgmuxChannelNumber entity)

{
  ApexWritePsmConfCnf *sig_p               = &signalBuffer->sig->apexWritePsmConfCnf;
  MobilityContext_t    *mobilityContext_p  = ptrToMobilityContext ();
  VgReqCpsmsData       *reqCpsmsData_p     = &(mobilityContext_p->vgReqCpsmsData);
  VgCpsmsData      *currentCpsmsData_p   = &(mobilityContext_p->vgCpsmsData);


  switch (getCommandId (entity))
  {
      case VG_AT_MM_CPSMS:
      {
         if (sig_p->success == TRUE)
         {
            currentCpsmsData_p->mode = reqCpsmsData_p->mode;
            if (currentCpsmsData_p->mode == VG_CPSMS_DISABLE_OR_RESET_TO_DEFAULTS) // disabled or restored defaults
            {
              /* clear the temp data as the defaults may have been
                         restored at lower layers and not recorded here. */
              currentCpsmsData_p->dataValid = FALSE;
              currentCpsmsData_p->mode = VG_CPSMS_DISABLE;  /* We may not be disabled - but we don't know anymore */
            }
            else if (currentCpsmsData_p->mode == VG_CPSMS_ENABLE)
            {
               if ((reqCpsmsData_p->reqActiveTimePresent) && (reqCpsmsData_p->reqTauPresent))
               {
                 /* update the temporary store with this info  */
                 currentCpsmsData_p->requestedTau =reqCpsmsData_p->reqTau;
                 currentCpsmsData_p->requestedActiveTime = reqCpsmsData_p->reqActiveTime;
                 currentCpsmsData_p->dataValid= TRUE;
               }
               else
               {
                  /* changed one but not the other so we don't have valid data in temp store now */
                  currentCpsmsData_p->dataValid= FALSE;
               }
            }
            /* Disable - nothing happens! */
            
            setResultCode (entity, RESULT_CODE_OK);
         }
         else
         {
            setResultCode (entity, VG_CME_OPERATION_NOT_ALLOWED);
         }
         reqCpsmsData_p->mode = VG_CPSMS_DISABLE;
         reqCpsmsData_p->reqActiveTimePresent = FALSE;
         reqCpsmsData_p->reqTauPresent = FALSE;
      }
      break;

      default:
      {
          /* Unexpected message. */
          FatalParam(entity, getCommandId (entity), 0);
      }
      break;
   }

}

/*************************************************************************
*
* Function:     vgApexReadPsmConfCnf
*
* Scope:        Global
*
* Parameters:   signalBuffer       - structure containing signal:
*                                     SIG_APEX_READ_PSM_CONF_CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  .
*
*************************************************************************/

void vgApexReadPsmConfCnf(  const SignalBuffer       *signalBuffer,
                                        const VgmuxChannelNumber entity)
{
  ApexReadPsmConfCnf *sig_p              = &signalBuffer->sig->apexReadPsmConfCnf;
  MobilityContext_t  *mobilityContext_p  = ptrToMobilityContext ();
  VgCpsmsData           *currentCpsmsData_p     = &(mobilityContext_p->vgCpsmsData);
  GprsGenericContext_t  *gprsGenericContext_p   = ptrToGprsGenericContext ();
  VgCEREGData           *vgCEREGData            = &gprsGenericContext_p->vgCEREGData;
  Char                reqPeriodicTauStr[VG_CPSMS_MAX_STR_LEN + NULL_TERMINATOR_LENGTH] = {0};
  Char                reqActiveTimeStr[VG_CPSMS_MAX_STR_LEN + NULL_TERMINATOR_LENGTH] = {0};


  switch (getCommandId (entity))
  {
      case VG_AT_MM_CPSMS:
      {
         vgPutNewLine (entity);
         /* disable or enable depending upon this setting */
         if (sig_p->psm.requestedPsmSupport == TRUE)
         {
           currentCpsmsData_p->mode = VG_CPSMS_ENABLE;
           currentCpsmsData_p->requestedActiveTime = sig_p->psm.requestedPsmValues.activeTime;
           currentCpsmsData_p->requestedTau = sig_p->psm.requestedPsmValues.periodicTau;
           vgPrintf (entity,(const Char*)"+CPSMS: %d",currentCpsmsData_p->mode);
           vgPrintf (entity, (const Char*)",,,\"");
           vgInt8ToBinString(currentCpsmsData_p->requestedTau, VG_CPSMS_MAX_STR_LEN, reqPeriodicTauStr);
           vgPrintf (entity, (const Char*)reqPeriodicTauStr);

           vgPrintf (entity, (const Char*)"\",\"");
           vgInt8ToBinString(currentCpsmsData_p->requestedActiveTime, VG_CPSMS_MAX_STR_LEN, reqActiveTimeStr);
           vgPrintf (entity, (const Char*)reqActiveTimeStr);
           vgPrintf (entity, (const Char*)"\"");
         }
         else
         {
           currentCpsmsData_p->mode = VG_CPSMS_DISABLE;
           currentCpsmsData_p->requestedActiveTime = 0;
           currentCpsmsData_p->requestedTau =0;
           vgPrintf (entity,(const Char*)"+CPSMS: %d", currentCpsmsData_p->mode);
         }
         currentCpsmsData_p->dataValid = TRUE;
         // update CEREG with nw settings
         vgCEREGData->psmInfoPresent = TRUE;
         vgCEREGData->activeTime = sig_p->psm.nwPsmValues.activeTime;
         vgCEREGData->periodicTau = sig_p->psm.nwPsmValues.periodicTau;

         vgPutNewLine (entity);
         setResultCode (entity, RESULT_CODE_OK);
      }
      break;

      default:
      {
        /* Unexpected message. */
        FatalParam(entity, getCommandId (entity), 0);
      }
      break;
    }
}

/*************************************************************************
*
* Function:     vgApexReadAttachPdnConfCnf
*
* Scope:        Global
*
* Parameters:   signalBuffer       - structure containing signal:
*                                     SIG_APEX_READ_ATTACH_PDN_CONF_CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  .
*
*************************************************************************/
void vgApexReadAttachPdnConfCnf(  const SignalBuffer       *signalBuffer,
                                        const VgmuxChannelNumber entity)
{
  ApexMmReadAttachPdnCfgCnf  *sig_p              = &signalBuffer->sig->apexMmReadAttachPdnCfgCnf;
  MobilityContext_t          *mobilityContext_p  = ptrToMobilityContext ();
  VgCipcaData                *currentCipcaData_p = &(mobilityContext_p->vgCipcaData);


  switch (getCommandId (entity))
  {
      case VG_AT_MM_CIPCA:
      {
         currentCipcaData_p->dataValid = TRUE;

         if (sig_p->attachWithoutPdn == FALSE)
         {
            currentCipcaData_p->vgCipcaOpt = VG_CIPCA_ATTACH_WITH_PDN;
         }
         else
         {
            currentCipcaData_p->vgCipcaOpt = VG_CIPCA_ATTACH_WITHOUT_PDN;
         }

         vgPutNewLine (entity);
         vgPrintf (entity, (const Char *)
         "+CIPCA: %d,%d", VG_CIPCA_EUTRAN_SETTING,currentCipcaData_p->vgCipcaOpt);
         vgPutNewLine (entity);

         setResultCode (entity, RESULT_CODE_OK);
      }
      break;

      default:
      {
          /* Unexpected message. */
          FatalParam(entity, getCommandId (entity), 0);
      }
      break;
   }
}

/*************************************************************************
*
* Function:     vgApexWriteAttachPdnConfCnf
*
* Scope:        Global
*
* Parameters:   signalBuffer       - structure containing signal:
*                                     SIG_APEX_WRITE_ATTACH_PDN_CONF_CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  .
*
*************************************************************************/
void vgApexWriteAttachPdnConfCnf(  const SignalBuffer       *signalBuffer,
                                        const VgmuxChannelNumber entity)
{
  ApexMmWriteAttachPdnCfgCnf *sig_p              = &signalBuffer->sig->apexMmWriteAttachPdnCfgCnf;
  MobilityContext_t          *mobilityContext_p  = ptrToMobilityContext ();
  VgCipcaData                *currentCipcaData_p = &(mobilityContext_p->vgCipcaData);


  switch (getCommandId (entity))
  {
      case VG_AT_MM_CIPCA:
      {
         if (sig_p->success == TRUE)
         {
            currentCipcaData_p->dataValid = TRUE;
            currentCipcaData_p->vgCipcaOpt = mobilityContext_p->vgReqCipcaOpt;
            setResultCode (entity, RESULT_CODE_OK);
         }
         else
         {
            setResultCode (entity, VG_CME_OPERATION_NOT_ALLOWED);
         }
      }
      break;

      default:
      {
          /* Unexpected message. */
          FatalParam(entity, getCommandId (entity), 0);
      }
      break;
   }

}

#if defined (FEA_NFM)
/*************************************************************************
*
* Function:     vgApexWriteNfmCnf
*
* Scope:        Global
*
* Parameters:   signalBuffer       - structure containing signal:
*                                     SIG_APEX_WRITE_NFM_CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  .
*
*************************************************************************/
void vgApexWriteNfmCnf(  const SignalBuffer       *signalBuffer,
                         const VgmuxChannelNumber entity)

{
  ApexWriteNfmCnf    *sig_p              = &signalBuffer->sig->apexWriteNfmCnf;
  MobilityContext_t  *mobilityContext_p  = ptrToMobilityContext ();
  VgReqNfmData       *reqNfmData_p     = &(mobilityContext_p->vgReqNfmData);
  VgNfmData      *currentNfmData_p   = &(mobilityContext_p->vgNfmData);

  switch (getCommandId (entity))
  {
      case VG_AT_MM_NFM:
      case VG_AT_MM_NFMTC:
      {
         if (sig_p->success == TRUE)
         {
            setResultCode (entity, RESULT_CODE_OK);
            currentNfmData_p->nfmActive = reqNfmData_p->reqNfmActive;
            if (reqNfmData_p->reqStartTimerActivePresent == TRUE)
            {
              currentNfmData_p->dataValid = TRUE;
              currentNfmData_p->startTimerActive = reqNfmData_p->reqStartTimerActive;
            }
         }
         else
         {
            setResultCode (entity, VG_CME_OPERATION_NOT_ALLOWED);
         }
         reqNfmData_p->reqNfmActive = FALSE;
         reqNfmData_p->reqStartTimerActivePresent = FALSE;
         reqNfmData_p->reqStartTimerActive = FALSE;
         reqNfmData_p->reqStParPresent = FALSE;
         reqNfmData_p->reqStTmPresent  = FALSE;
      }
      break;

      default:
      {
          /* Unexpected message. */
          FatalParam(entity, getCommandId (entity), 0);
      }
      break;
   }

}

/*************************************************************************
*
* Function:     vgApexReadNfmCnf
*
* Scope:        Global
*
* Parameters:   signalBuffer       - structure containing signal:
*                                     SIG_APEX_READ_NFM_CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  .
*
*************************************************************************/

void vgApexReadNfmCnf(  const SignalBuffer       *signalBuffer,
                        const VgmuxChannelNumber entity)
{
  ApexReadNfmCnf     *sig_p              = &signalBuffer->sig->apexReadNfmCnf;
  MobilityContext_t  *mobilityContext_p  = ptrToMobilityContext ();
  VgNfmData          *currentNfmData_p   = &(mobilityContext_p->vgNfmData);

  switch (getCommandId (entity))
  {
      case VG_AT_MM_NFM:
      {
         vgPutNewLine (entity);
         currentNfmData_p->dataValid = TRUE;
         currentNfmData_p->nfmActive = sig_p->readNfmCnf.nfmActive;
         currentNfmData_p->startTimerActive = sig_p->readNfmCnf.startTimerActive;

         vgPrintf (entity, (const Char*)"+NFM: %d,%d",
             currentNfmData_p->nfmActive, currentNfmData_p->startTimerActive);
         vgPutNewLine (entity);
         setResultCode (entity, RESULT_CODE_OK);
      }
      break;

      default:
      {
        /* Unexpected message. */
        FatalParam(entity, getCommandId (entity), 0);
      }
      break;
    }
}

/*************************************************************************
*
* Function:     vgApexWriteNfmConfCnf
*
* Scope:        Global
*
* Parameters:   signalBuffer       - structure containing signal:
*                                     SIG_APEX_WRITE_NFM_CONF_CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  .
*
*************************************************************************/
void vgApexWriteNfmConfCnf(  const SignalBuffer       *signalBuffer,
                             const VgmuxChannelNumber entity )

{
  ApexWriteNfmConfCnf    *sig_p            = &signalBuffer->sig->apexWriteNfmConfCnf;
  MobilityContext_t    *mobilityContext_p  = ptrToMobilityContext ();
  VgNfmcData           *vgNfmcData_p   = &(mobilityContext_p->vgNfmcData);

  switch (getCommandId (entity))
  {
      case VG_AT_MM_NFMC:
      {
         if (sig_p->success == TRUE)
         {
            setResultCode (entity, RESULT_CODE_OK);
            vgNfmcData_p->dataValid = FALSE;
         }
         else
         {
            setResultCode (entity, VG_CME_OPERATION_NOT_ALLOWED);
         }
      }
      break;

      default:
      {
          /* Unexpected message. */
          FatalParam(entity, getCommandId (entity), 0);
      }
      break;
  }

}

/*************************************************************************
*
* Function:     vgApexReadNfmConfCnf
*
* Scope:        Global
*
* Parameters:   signalBuffer       - structure containing signal:
*                                     SIG_APEX_READ_NFM_CONF_CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  .
*
*************************************************************************/

void vgApexReadNfmConfCnf(  const SignalBuffer       *signalBuffer,
                            const VgmuxChannelNumber entity)
{
  ApexReadNfmConfCnf   *sig_p              = &signalBuffer->sig->apexReadNfmConfCnf;
  MobilityContext_t    *mobilityContext_p  = ptrToMobilityContext ();
  VgNfmcData           *vgNfmcData_p   = &(mobilityContext_p->vgNfmcData);
  Int8                 index;

  switch (getCommandId (entity))
  {
      case VG_AT_MM_NFMC:
      {
         for (index = 0; index < MAX_NFM_PAR_VALUE; index++)
         {
           vgNfmcData_p->nfmPar[index] = sig_p->readNfmConfCnf.nfmPar[index];
         }
         vgNfmcData_p->stPar = sig_p->readNfmConfCnf.stPar;
         vgNfmcData_p->dataValid = TRUE;

         vgPutNewLine (entity);
         vgPrintf (entity, (const Char*)"+NFMC: %d,%d,%d,%d,%d,%d,%d,%d",
             vgNfmcData_p->nfmPar[0], vgNfmcData_p->nfmPar[1], vgNfmcData_p->nfmPar[2],
             vgNfmcData_p->nfmPar[3], vgNfmcData_p->nfmPar[4], vgNfmcData_p->nfmPar[5],
             vgNfmcData_p->nfmPar[6], vgNfmcData_p->stPar );
         vgPutNewLine (entity);
         setResultCode (entity, RESULT_CODE_OK);
      }
      break;

      default:
      {
        /* Unexpected message. */
        FatalParam(entity, getCommandId (entity), 0);
      }
      break;
  }
}

/*************************************************************************
*
* Function:     vgApexReadNfmStatusCnf
*
* Scope:        Global
*
* Parameters:   signalBuffer       - structure containing signal:
*                                     SIG_APEX_READ_NFM_STATUS_CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  .
*
*************************************************************************/

void vgApexReadNfmStatusCnf(  const SignalBuffer       *signalBuffer,
                              const VgmuxChannelNumber entity)
{
  ApexReadNfmStatusCnf  *sig_p    = &signalBuffer->sig->apexReadNfmStatusCnf;

  switch (getCommandId (entity))
  {
      case VG_AT_MM_MNFM:
      {
         vgPutNewLine (entity);
         vgPrintf (entity, (const Char*)"*MNFM: %d,%d",
             sig_p->readNfmStatusCnf.remStartTimerValue,
             sig_p->readNfmStatusCnf.remBackOffTimerValue);
         vgPutNewLine (entity);
         setResultCode (entity, RESULT_CODE_OK);
      }
      break;

      default:
      {
        /* Unexpected message. */
        FatalParam(entity, getCommandId (entity), 0);
      }
      break;
    }
}

#endif
#if defined (FEA_RPM)

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigApexMmRpmInfoInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_APEX_RPM_IND
 *
 * Returns:     Nothing
 *
 * Description: 
 *-------------------------------------------------------------------------*/
void vgSigApexMmRpmInfoInd (const SignalBuffer *signalBuffer)
{
    ApexRpmInd   *sig_p             = &signalBuffer->sig->apexRpmInd;
    VgmuxChannelNumber profileEntity     = 0;
    MobilityContext_t     *mobilityContext_p   = ptrToMobilityContext ();
    VgMrpmData            *vgMrpmData_p     = &(mobilityContext_p->vgMrpmData);
    SignalBuffer allocSig = kiNullBuffer;

    for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
    {
      if (isEntityActive (profileEntity)&&(vgMrpmData_p->enable))
      {
         switch(sig_p->cause)
         {
             case ABRPM_CAUSE_N1_REACHED:
                vgPrintf (profileEntity,
                  (const Char *)"*MRPM: ATTACH BLOCK,%d,%d",sig_p->cause,sig_p->time);                    
                vgPutNewLine (profileEntity);
                vgFlushBuffer(profileEntity); 
             break;
             case ABRPM_CAUSE_F1_REACHED:
             case ABRPM_CAUSE_F2_REACHED:
             case ABRPM_CAUSE_F3_REACHED:
             case ABRPM_CAUSE_F4_REACHED:
                 vgPrintf (profileEntity,
                   (const Char *)"*MRPM: PDN CONNECT BLOCK,%d,%d",sig_p->cause,sig_p->time);                    
                 vgPutNewLine (profileEntity);
                 vgFlushBuffer(profileEntity);
             break;
             case ABRPM_CAUSE_T1_TIMEOUT:
                 vgPrintf (profileEntity,
                   (const Char *)"*MRPM: CHIPSET RESET");                    
                 vgPutNewLine (profileEntity);
                 vgFlushBuffer(profileEntity);

                 KiCreateZeroSignal(SIG_APEX_RPM_RESET_CHIPSET_IND,
                                        sizeof(ApexRpmResetChipsetInd),
                                        &allocSig);                                 
                 KiSendSignal(TASK_BL_ID, &allocSig);           
             break;

             default:
                break;
         }
         
      }
    }
}

/*************************************************************************
*
* Function:     vgApexReadMrpmCnf
*
* Scope:        Global
*
* Parameters:   signalBuffer       - structure containing signal:
*                                     SIG_APEX_RPM_READ__CNF
*               entity             - mux channel number
* Returns:      void
*
* Description:  .
*
*************************************************************************/

void vgApexReadRpmCnf(  const SignalBuffer       *signalBuffer,
                              const VgmuxChannelNumber entity)
{
  ApexRpmReadInfoCnf  *sig_p    = &signalBuffer->sig->apexRpmReadInfoCnf;
  switch (getCommandId (entity))
  {
      case VG_AT_MM_MRPMR:
      {        
         if(sig_p->error ==ABRPM_READ_ERR_NO_ERROR)
         {
             vgPutNewLine (entity);
             vgPrintf (entity, (const Char*)"*MRPMR: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                 sig_p->area,
                 sig_p->info.param.flag,              
                 sig_p->info.param.n1,
                 sig_p->info.param.t1,
                 sig_p->info.param.f1,
                 sig_p->info.param.f2,
                 sig_p->info.param.f3,
                 sig_p->info.param.f4,
                 sig_p->info.omc.c_br_1,
                 sig_p->info.omc.c_r_1,
                 sig_p->info.omc.c_pdp_1,                
                 sig_p->info.omc.c_pdp_2,
                 sig_p->info.omc.c_pdp_3,
                 sig_p->info.omc.c_pdp_4,
                 sig_p->info.omcLeakRate.lr_1,
                 sig_p->info.omcLeakRate.lr_2,
                 sig_p->info.omcLeakRate.lr_3,
                 sig_p->info.verImpl
             );
             vgPutNewLine (entity);
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
        /* Unexpected message. */
        FatalParam(entity, getCommandId (entity), 0);
      }
      break;
    }
}
#endif
/*************************************************************************
*
* Function:     vgApexMmDisableHplmnSearchCnf
*
* Scope:        Global
*
* Parameters:   in: commandBuffer_p - the AT command line string
*
* Returns:      void
*
* Description: set  Disable Hplmn search parameter response  handler.
*
*************************************************************************/

void vgApexMmDisableHplmnSearchCnf (const SignalBuffer       *signalBuffer,
                                   const VgmuxChannelNumber entity)
{
    ApexMmDisableHplmnSearchCnf    *sig_p = &signalBuffer->sig->apexMmDisableHplmnSearchCnf;

    if (sig_p->result)
    {
        setResultCode (entity, RESULT_CODE_ERROR);
    }
    else
    {
        switch (getCommandId (entity))
        {
            case VG_AT_MM_MHPLMNS:
            {
                setResultCode (entity, RESULT_CODE_OK);
            }
            break;
            default:
            {
              /* Illegal command for SIG_APEX_MM_DISABLE_HPLMN_SEARCH_CNF */
              FatalParam(entity, getCommandId (entity), 0);
              break;
            }
        } /* switch */
    }
}/*vgApexMmSearchBandListCnf*/

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

