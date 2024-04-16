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
 ***************************************************************************
 * File Description:
 * Outgoing signal handlers for the Common Sub-System.
 *
 * Procedures simply send a signal. The signal is created, its expected
 * returning signal is registered, contents filled and then it is sent.
 **************************************************************************/

#define MODULE_NAME "RVMMSIGO"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <gkisig.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvchman.h>
#include <rvmmsigo.h>
#include <rvcfg.h>
#include <dmrtc_sig.h>
#include <abrpm_sig.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
    ApexMmReadPlmnSelReq    apexMmReadPlmnSelReq;
    ApexMmWritePlmnSelReq   apexMmWritePlmnSelReq;
    ApexMmPlmnListReq       apexMmPlmnListReq;
    ApexMmPlmnSelectReq     apexMmPlmnSelectReq;
    ApexEmPlmnTestReq       apexEmPlmnTestReq;
    DmRtcSetTimeZoneReq     dmRtcSetTimeZoneReq;
    DmRtcReadTimeZoneReq    dmRtcReadTimeZoneReq;
#if defined (DM_EXCLUDE_RTC_DEVICE_MANAGER)
    DmRtcSetTimeZoneCnf     dmRtcSetTimeZoneCnf;
    DmRtcReadTimeZoneCnf    dmRtcReadTimeZoneCnf;
#endif
    ApexMmDeregisterReq     apexMmDeregisterReq;
    ApexMmReadBandModeReq   apexMmReadBandModeReq;
    ApexMmSetEdrxReq        apexMmSetEdrxReq;
    ApexMmReadEdrxReq       apexMmReadEdrxReq;
    ApexWriteIotOptCfgReq   apexWriteIotOptCfgReq;
    ApexReadIotOptCfgReq    apexReadIotOptCfgReq;
    ApexWritePsmConfReq     apexWritePsmConfReq;
    ApexReadPsmConfReq      apexReadPsmConfReq;
    ApexMmReadAttachPdnCfgReq  apexMmReadAttachPdnCfgReq;
    ApexMmWriteAttachPdnCfgReq apexMmWriteAttachPdnCfgReq;
#if defined (FEA_NFM)
    ApexReadNfmReq          apexReadNfmReq;
    ApexWriteNfmReq         apexWriteNfmReq;
    ApexReadNfmConfReq      apexReadNfmConfReq;
    ApexWriteNfmConfReq     apexWriteNfmConfReq;
    ApexReadNfmStatusReq    apexReadNfmStatusReq;
#endif
    ApexMmLockArfcnReq         apexMmLockArfcnReq;
    ApexMmUeStatsReq           apexMmUeStatsReq;
    ApexMmLocCellInfoReq       apexMmLocCellInfoReq;
    ApexMmSearchBandListReq    apexMmSearchBandListReq;
    ApexMmSetEhplmnReq         apexMmSetEhplmnReq;
#if defined (FEA_RPM)
    ApexRpmReadInfoReq      apexRpmReadInfoReq;
#endif
    ApexMmDisableHplmnSearchReq apexMmDisableHplmnSearchReq;    
};

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexMmReadPlmnSelReq
 *
 * Parameters:      Int16 - index of first operator in list we want to read
 *                  Int16 - number of entries we would like returned
 *                  VgmuxChannelNumber - entity which sent request
 *                  AbmmPlmnSelector - the plmn selector
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_MM_READ_PLMN_SEL_REQ to Background layer.
 *                  It requests the list of preferred operators stored on SIM
 *************************************************************************/

void vgSigApexMmReadPlmnSelReq (Int16                    startField,
                                Int8                     numEntriesDesired,
                                AbmmPlmnSelector         plmnSelector,
                                const VgmuxChannelNumber entity)
{
    SignalBuffer         signalBuffer = kiNullBuffer;
    ApexMmReadPlmnSelReq *request_p;

    /* Note: It is not necessary to take control to read the preferred PLMNs */

    sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_MM_READ_PLMN_SEL_CNF);

    KiCreateZeroSignal (SIG_APEX_MM_READ_PLMN_SEL_REQ,
        sizeof (ApexMmReadPlmnSelReq),
        &signalBuffer);

    request_p                    = &signalBuffer.sig->apexMmReadPlmnSelReq;
    request_p->taskId            = VG_CI_TASK_ID;
    request_p->numEntriesDesired = numEntriesDesired;
    request_p->startField        = startField;
    request_p->plmnSelector      = plmnSelector;
    KiSendSignal (TASK_BL_ID, &signalBuffer);
} /* vgSigApexMmReadPlmnSelReq */

/*--------------------------------------------------------------------------
*
* Function:        vgSigApexMmWritePlmnSelReq
*
* Parameters:      VgmuxChannelNumber - entity which sent request
*
* Returns:         Nothing
*
* Description:     Sends a SIG_APEX_MM_WRITE_PLMN_SEL_REQ to the
*                  background layer to instruct it to insert/append/delete
*                  entry in SIM preferred operator list
*-------------------------------------------------------------------------*/
void vgSigApexMmWritePlmnSelReq (const VgmuxChannelNumber entity)
{
    SignalBuffer          signalBuffer       = kiNullBuffer;
    ApexMmWritePlmnSelReq *request_p;

    MobilityContext_t     *mobilityContext_p = ptrToMobilityContext ();
    VgCPOLData            *vgCPOLData        = &mobilityContext_p->vgCPOLData;

    FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_MM_WRITE_PLMN_SEL_CNF);

    KiCreateZeroSignal (SIG_APEX_MM_WRITE_PLMN_SEL_REQ,
        sizeof (ApexMmWritePlmnSelReq),
        &signalBuffer);

    request_p                    = &signalBuffer.sig->apexMmWritePlmnSelReq;

    request_p->taskId            = VG_CI_TASK_ID;
    request_p->entryNumber       = vgCPOLData->index;
    request_p->plmnNumberPresent = vgCPOLData->plmnNumberPresent;
    request_p->plmnNumber        = vgCPOLData->plmnNumber;
    request_p->threeDigitMnc     = vgCPOLData->threeDigitMnc;
    request_p->writeAction       = vgCPOLData->action;
    request_p->plmnNamePresent   = vgCPOLData->plmnNamePresent;
    request_p->plmnName          = vgCPOLData->plmnName;

    KiSendSignal (TASK_BL_ID, &signalBuffer);
} /* vgSigApexMmWritePlmnSelReq */

/*--------------------------------------------------------------------------
*
* Function:        vgSigApexMmPlmnListReq
*
* Parameters:      VgmuxChannelNumber - entity which sent request
*
* Returns:         Nothing
*
* Description:     Sends a SIG_APEX_MM_PLMNLIST_REQ to the
*                  background layer. This returns a list of valid network
*                  operators that may be selected.
*-------------------------------------------------------------------------*/

void vgSigApexMmPlmnListReq (const VgmuxChannelNumber entity)
{
    SignalBuffer      signalBuffer = kiNullBuffer;
    ApexMmPlmnListReq *request_p;



    FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

    KiCreateZeroSignal (SIG_APEX_MM_PLMNLIST_REQ,
        sizeof (ApexMmPlmnListReq),
        &signalBuffer);

    request_p         = (ApexMmPlmnListReq *) signalBuffer.sig;
    request_p->taskId = VG_CI_TASK_ID;

    request_p->networkModesToSearch = NMODE_LTE; /* set for  NB-IOT  may need to be updated*/

    if (getResultCode (entity) != RESULT_CODE_ERROR)
    {
        sendSsRegistrationSignal (MOBILITY,
            entity,
            SIG_APEX_MM_PLMNLIST_CNF);

        KiSendSignal (TASK_BL_ID, &signalBuffer);
    }
    else
    {
        /*
         * An error occured so simply destroy the signal.
         */
        KiDestroySignal (&signalBuffer);
    }

} /* vgSigApexMmPlmnListReq */

/*--------------------------------------------------------------------------
*
* Function:        vgSigApexMmAbortPlmnListReq
*
* Parameters:      VgmuxChannelNumber - entity which sent request
*
* Returns:         Nothing
*
* Description:     Sends a SIG_APEX_MM_ABORT PLMNLIST_REQ to the
*                  background layer.
*-------------------------------------------------------------------------*/

void vgSigApexMmAbortPlmnListReq (const VgmuxChannelNumber entity)
{
    SignalBuffer              signalBuffer = kiNullBuffer;
    ApexMmAbortPlmnListReq    *request_p;

    FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal (MOBILITY, entity, SIG_APEX_MM_ABORT_PLMNLIST_CNF);

    KiCreateZeroSignal (SIG_APEX_MM_ABORT_PLMNLIST_REQ,
        sizeof (ApexMmAbortPlmnListReq),
        &signalBuffer);

    request_p               = (ApexMmAbortPlmnListReq *) signalBuffer.sig;
    request_p->taskId       = VG_CI_TASK_ID;

    KiSendSignal (TASK_BL_ID, &signalBuffer);
} /* vgSigApexMmAbortPlmnListReq */

/*--------------------------------------------------------------------------
 *
 * Function:        vgApexSelectPlmnReq
 *
 * Parameters:      VgmuxChannelNumber - entity which sent request
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_MM_PLMN_SELECT_REQ to the background
 *                  layer. Initiated by the AT+COPS command after the requested
 *                  operator has been located amoung all the valid operators.
 *
 *-------------------------------------------------------------------------*/

void vgSigApexMmPlmnSelectReq (const VgmuxChannelNumber entity)
{
    SignalBuffer        signalBuffer       = kiNullBuffer;
    ApexMmPlmnSelectReq *request_p;
    MobilityContext_t   *mobilityContext_p = ptrToMobilityContext ();
    VgCOPSData          *vgCOPSData        = &mobilityContext_p->vgCOPSData;

    FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

    KiCreateZeroSignal (SIG_APEX_MM_PLMN_SELECT_REQ,
        sizeof (ApexMmPlmnSelectReq),
        &signalBuffer);

    request_p         = (ApexMmPlmnSelectReq *) signalBuffer.sig;
    request_p->taskId = VG_CI_TASK_ID;

    request_p->modeToPlmnSelect = NMODE_LTE; /* set for NB_IOT may need to be modified */

    /* if manually specified operator set signal flag appropriately */
    if (vgCOPSData->mode == VG_OP_AUTOMATIC_MODE)
    {
        vgCOPSData->autoSelectInProgress = TRUE;

        request_p->useManualMode         = FALSE;

        request_p->returnToRplmn = vgCOPSData->returnToRplmn;
    }
    else
    {
        /* LTE network mode is TDD and FDD so or the two values together - may need to change for NB-IOT */
        vgCOPSData->selectedPlmn.plmn.accessTechnology = EUTRAN_ACCESS_TECHNOLOGY_TDD | EUTRAN_ACCESS_TECHNOLOGY_FDD;
        request_p->useManualMode = TRUE;
    }

    if (getResultCode (entity) != RESULT_CODE_ERROR)
    {
        sendSsRegistrationSignal (MOBILITY,
            entity,
            SIG_APEX_MM_PLMN_SELECT_CNF);

        request_p->requestedPlmn.mcc              = vgCOPSData->selectedPlmn.plmn.mcc;
        request_p->requestedPlmn.mnc              = vgCOPSData->selectedPlmn.plmn.mnc;
        request_p->requestedPlmn.accessTechnology = vgCOPSData->selectedPlmn.plmn.accessTechnology;
        request_p->requestedPlmn.is_three_digit_mnc = vgCOPSData->threeDigitMnc;
        
        KiSendSignal (TASK_BL_ID, &signalBuffer);
    }
    else
    {
        /*
         * An error occured so simply destroy the signal.
         */
        KiDestroySignal (&signalBuffer);
    }
} /* vgSigApexMmPlmnSelectReq */

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigApexMmDeregisterReq
 *
 * Parameters:      VgmuxChannelNumber - entity which sent request
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_MM_DEREGISTER_REQ to the background
 *                  layer. Initiated by the AT+COPS command to manually
 *                  deregister from the network
 *
 *-------------------------------------------------------------------------*/

void vgSigApexMmDeregisterReq (const VgmuxChannelNumber entity)
{
    SignalBuffer        signalBuffer = kiNullBuffer;
    ApexMmDeregisterReq *request_p;

    FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_MM_DEREGISTER_CNF);

    KiCreateZeroSignal (SIG_APEX_MM_DEREGISTER_REQ,
        sizeof (ApexMmDeregisterReq),
        &signalBuffer);

    request_p         = (ApexMmDeregisterReq *) signalBuffer.sig;
    request_p->taskId = VG_CI_TASK_ID;

    KiSendSignal (cfRvAbTaskId, &signalBuffer);
} /* vgSigApexMmDeregisterReq */

/*--------------------------------------------------------------------------
*
* Function:    vgSigApexEmPlmnTestReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends a SIG_APEX_EM_PLMN_TEST_REQ to BL in
*              order to obtain a list of operator names available.

*-------------------------------------------------------------------------*/

void vgSigApexEmPlmnTestReq (const VgmuxChannelNumber entity)
{
    SignalBuffer      signalToSend       = kiNullBuffer;
    MobilityContext_t *mobilityContext_p = ptrToMobilityContext ();
    VgCOPNData        *vgCOPNData        = &(mobilityContext_p->vgCOPNData);

    FatalAssert (vgChManCheckHaveControl (CC_ENGINEERING_MODE, entity) == TRUE);

    sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_EM_PLMN_TEST_CNF);

    KiCreateZeroSignal (SIG_APEX_EM_PLMN_TEST_REQ,
        sizeof(ApexEmPlmnTestReq),
        &signalToSend);

    signalToSend.sig->apexEmPlmnTestReq.taskId            = VG_CI_TASK_ID;
    signalToSend.sig->apexEmPlmnTestReq.requestedPlmn.mcc = vgCOPNData->currentPlmn.mcc;
    signalToSend.sig->apexEmPlmnTestReq.requestedPlmn.mnc = vgCOPNData->currentPlmn.mnc;
    signalToSend.sig->apexEmPlmnTestReq.plmnOrder         = vgCOPNData->order;

    KiSendSignal (cfRvAbTaskId, &signalToSend);
} /* vgSigApexEmPlmnTestReq */

/*--------------------------------------------------------------------------
*
* Function:    vgSigAclkReadTimeZoneReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends a SIG_DM_RTC_READ_TIME_ZONE_REQ to BL in
*              order to read the current time zone

*-------------------------------------------------------------------------*/

void vgSigAclkReadTimeZoneReq (const VgmuxChannelNumber entity)
{
    SignalBuffer signalToSend = kiNullBuffer;

    sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_DM_RTC_READ_TIME_ZONE_CNF);

#if defined (DM_EXCLUDE_RTC_DEVICE_MANAGER)
  /*
   * If we have no RTC device manager - we don't want to send
   * anything to it - so just send ourselves a pretend
   * response back.
   */
    KiCreateZeroSignal (SIG_DM_RTC_READ_TIME_ZONE_CNF,
        sizeof(DmRtcReadTimeZoneCnf),
        &signalToSend);

    signalToSend.sig->dmRtcReadTimeZoneCnf.timeZone.format           = RTC_DISP_FORMAT_POS;
    signalToSend.sig->dmRtcReadTimeZoneCnf.timeZone.offset.hours    = 0;
    signalToSend.sig->dmRtcReadTimeZoneCnf.timeZone.offset.minutes  = 0;
    signalToSend.sig->dmRtcReadTimeZoneCnf.timeZone.offset.seconds  = 0;

    signalToSend.sig->dmRtcReadTimeZoneCnf.status = RTC_STATUS_OK;

    KiSendSignal (VG_CI_TASK_ID, &signalToSend);

#else /* DM_EXCLUDE_RTC_DEVICE_MANAGER */

    KiCreateZeroSignal (SIG_DM_RTC_READ_TIME_ZONE_REQ,
        sizeof(DmRtcReadTimeZoneReq),
        &signalToSend);

    signalToSend.sig->dmRtcReadTimeZoneReq.taskId = VG_CI_TASK_ID;

    KiSendSignal (DM_RTC_TASK_ID, &signalToSend);

#endif /* DM_EXCLUDE_RTC_DEVICE_MANAGER */
} /* vgSigAclkReadTimeZoneReq */

/*--------------------------------------------------------------------------
*
* Function:    vgSigAclkSetTimeZoneReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends a SIG_DM_RTC_SET_TIME_ZONE_REQ to BL in
*              order to set the current time zone

*-------------------------------------------------------------------------*/

void vgSigAclkSetTimeZoneReq (const VgmuxChannelNumber entity)
{
    SignalBuffer      signalToSend       = kiNullBuffer;

#if defined (DM_EXCLUDE_RTC_DEVICE_MANAGER)
  /*
   * If we have no RTC device manager - we don't want to send
   * anything to it - so just send ourselves a pretend
   * response back.
   */
    sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_DM_RTC_SET_TIME_ZONE_CNF);

    KiCreateZeroSignal (SIG_DM_RTC_SET_TIME_ZONE_CNF,
        sizeof(DmRtcSetTimeZoneCnf),
        &signalToSend);

    signalToSend.sig->dmRtcSetTimeZoneCnf.status = RTC_STATUS_OK;

    KiSendSignal (VG_CI_TASK_ID, &signalToSend);

#else /* DM_EXCLUDE_RTC_DEVICE_MANAGER */

    MobilityContext_t *mobilityContext_p = ptrToMobilityContext ();

    sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_DM_RTC_SET_TIME_ZONE_CNF);

    KiCreateZeroSignal (SIG_DM_RTC_SET_TIME_ZONE_REQ,
        sizeof(DmRtcSetTimeZoneReq),
        &signalToSend);

    signalToSend.sig->dmRtcSetTimeZoneReq.taskId = VG_CI_TASK_ID;

    memcpy (&signalToSend.sig->dmRtcSetTimeZoneReq.timeZone,
        &mobilityContext_p->newTimeZone,
        sizeof(RtcDisplacement));

    KiSendSignal (DM_RTC_TASK_ID, &signalToSend);

#endif /* DM_EXCLUDE_RTC_DEVICE_MANAGER */
} /* vgSigAclkSetTimeZoneReq */

/*************************************************************************
*
* Function:     vgApexMmReadBandModeReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/

void vgApexMmReadBandModeReq (const VgmuxChannelNumber entity)
{
    SignalBuffer          sigBuff = kiNullBuffer;
    ApexMmReadBandModeReq *request_p;

    FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_MM_READ_BAND_MODE_CNF);

    KiCreateZeroSignal (SIG_APEX_MM_READ_BAND_MODE_REQ,
        sizeof (ApexMmReadBandModeReq),
        &sigBuff);

    request_p         = (ApexMmReadBandModeReq *)sigBuff.sig;
    request_p->taskId = VG_CI_TASK_ID;

    KiSendSignal (TASK_BL_ID, &sigBuff);
} /* vgApexMmReadBandModeReq */

/*************************************************************************
*
* Function:     vgApexMmReadPwonOptionsReq
*
* Parameters:   entity - mux channel number
*
* Returns:      void
*
* Description:
*
*************************************************************************/

void vgApexMmReadPwonOptionsReq (const VgmuxChannelNumber entity)
{
    SignalBuffer             sigBuff = kiNullBuffer;
    ApexMmReadPwonOptionsReq *request_p;

    FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_MM_READ_PWON_OPTIONS_CNF);

    KiCreateZeroSignal (SIG_APEX_MM_READ_PWON_OPTIONS_REQ,
        sizeof (ApexMmReadPwonOptionsReq),
        &sigBuff);

    request_p         = (ApexMmReadPwonOptionsReq *)sigBuff.sig;
    request_p->taskId = VG_CI_TASK_ID;

    KiSendSignal (TASK_BL_ID, &sigBuff);
} /* vgApexMmReadPwonOptionsReq */

/*************************************************************************
*
* Function:     vgApexMmWritePwonOptionsReq
*
* Parameters:   entity - mux channel number
*
* Returns:      void
*
* Description:
*
*************************************************************************/

void vgApexMmWritePwonOptionsReq (const VgmuxChannelNumber entity)
{
    SignalBuffer              sigBuff            = kiNullBuffer;
    ApexMmWritePwonOptionsReq *request_p;
    MobilityContext_t         *mobilityContext_p = ptrToMobilityContext ();

    FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_MM_WRITE_PWON_OPTIONS_CNF);

    KiCreateZeroSignal (SIG_APEX_MM_WRITE_PWON_OPTIONS_REQ,
        sizeof (ApexMmWritePwonOptionsReq),
        &sigBuff);

    request_p           = (ApexMmWritePwonOptionsReq *)sigBuff.sig;
    request_p->taskId   = VG_CI_TASK_ID;

    request_p->gaOption = mobilityContext_p->gaOption;

    KiSendSignal (TASK_BL_ID, &sigBuff);
} /* vgApexMmWritePwonOptionsReq */

#if defined(UPGRADE_MTNET)

/*************************************************************************
*
* Function:     vgApexMmSuspendReq
*
* Parameters:   entity - mux channel number
*
* Returns:      void
*
* Description:
*
*************************************************************************/

void  vgApexMmSuspendReq (const VgmuxChannelNumber entity)
{
    SignalBuffer          sigBuff = kiNullBuffer;
    ApexMmSuspendReq      *request_p;

    FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_MM_SUSPEND_CNF);

    KiCreateZeroSignal (SIG_APEX_MM_SUSPEND_REQ,
        sizeof (ApexMmSuspendReq),
        &sigBuff);

    request_p         = (ApexMmSuspendReq *)sigBuff.sig;
    request_p->taskId = VG_CI_TASK_ID;
    request_p->silent = TRUE;

    KiSendSignal (TASK_BL_ID, &sigBuff);
} /* vgApexMmSuspendReq */

/*************************************************************************
*
* Function:     vgApexMmResumeReq
*
* Parameters:   entity - mux channel number
*
* Returns:      void
*
* Description:
*
*************************************************************************/

void  vgApexMmResumeReq (const VgmuxChannelNumber entity)
{
    SignalBuffer          sigBuff = kiNullBuffer;
    ApexMmResumeReq       *request_p;

    FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_MM_RESUME_CNF);

    KiCreateZeroSignal (SIG_APEX_MM_RESUME_REQ,
        sizeof (ApexMmResumeReq),
        &sigBuff);

    request_p         = (ApexMmResumeReq *)sigBuff.sig;
    request_p->taskId = VG_CI_TASK_ID;

    KiSendSignal (TASK_BL_ID, &sigBuff);
} /* vgApexMmSuspendReq */

#endif /* #if defined(UPGRADE_MTNET) */

/*************************************************************************
*
* Function:     vgApexMmSetEdrxReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/
void vgApexMmSetEdrxReq(const VgmuxChannelNumber entity)
{
      SignalBuffer          signalBuffer       = kiNullBuffer;
      ApexMmSetEdrxReq      *request_p;

      MobilityContext_t     *mobilityContext_p = ptrToMobilityContext ();
      VgReqEdrxData          *vgReqEdrxData    = &mobilityContext_p->vgReqEdrxData;

      FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

      sendSsRegistrationSignal (MOBILITY,
          entity,
          SIG_APEX_MM_SET_EDRX_CNF);

      KiCreateZeroSignal (SIG_APEX_MM_SET_EDRX_REQ,
          sizeof (ApexMmSetEdrxReq),
          &signalBuffer);

      request_p                    = &signalBuffer.sig->apexMmSetEdrxReq;
      request_p->taskId            = VG_CI_TASK_ID;
      request_p->userEdrxSupport   = vgReqEdrxData->userEdrxSupport;
      request_p->userEdrxValue     = vgReqEdrxData->userEdrxValue;
      request_p->userPtwPresence = vgReqEdrxData->userPtwPresence;
      request_p->userPtwValue     = vgReqEdrxData->userPtwValue;      
      if (vgReqEdrxData->mode  == VG_EDRX_MODE_DISABLE_EDRX_AND_RESET)
      {
         request_p->resetToDefaultValues = TRUE;
      }
      KiSendSignal (TASK_BL_ID, &signalBuffer);
} /* vgApexMmSetEdrxReq */

/*************************************************************************
*
* Function:     vgApexMmReadEdrxReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/
void vgApexMmReadEdrxReq(const VgmuxChannelNumber entity)
{
  SignalBuffer          signalBuffer       = kiNullBuffer;
  ApexMmReadEdrxReq      *request_p;

  FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (MOBILITY,
      entity,
      SIG_APEX_MM_READ_EDRX_CNF);

  KiCreateZeroSignal (SIG_APEX_MM_READ_EDRX_REQ,
      sizeof (ApexMmReadEdrxReq),
      &signalBuffer);
  request_p                    = &signalBuffer.sig->apexMmReadEdrxReq;
  request_p->taskId            = VG_CI_TASK_ID;
  KiSendSignal (TASK_BL_ID, &signalBuffer);

}/* vgApexMmReadEdrxReq */

/*************************************************************************
*
* Function:     vgApexWriteIotOptCfgReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/

void vgApexWriteIotOptCfgReq(const VgmuxChannelNumber entity)
{
  SignalBuffer               signalBuffer       = kiNullBuffer;
  ApexWriteIotOptCfgReq      *request_p;
  MobilityContext_t   *mobilityContext_p   = ptrToMobilityContext ();
  VgReqCciotoptData   *reqCciotoptData_p   = &(mobilityContext_p->vgReqCciotoptData);

  FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (MOBILITY,
      entity,
      SIG_APEX_WRITE_IOT_OPT_CFG_CNF);

  KiCreateZeroSignal (SIG_APEX_WRITE_IOT_OPT_CFG_REQ,
      sizeof (ApexWriteIotOptCfgReq),
      &signalBuffer);
  request_p                     = &signalBuffer.sig->apexWriteIotOptCfgReq;
  request_p->taskId             = VG_CI_TASK_ID;
  request_p->resetToDefault     = reqCciotoptData_p->resetCiotOptParams;

  request_p->ciotPreferenceValid     = reqCciotoptData_p->prefUEOptPresent;
  request_p->ciotPreference          = reqCciotoptData_p->prefUEOpt;
  request_p->supportedUeOptionsValid = reqCciotoptData_p->supportedUEOptPresent;
  request_p->supportedUeOptions      = reqCciotoptData_p->supportedUEOpt;

  KiSendSignal (TASK_BL_ID, &signalBuffer);
}


/*************************************************************************
*
* Function:     vgApexReadIotOptCfgReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/

void vgApexReadIotOptCfgReq(const VgmuxChannelNumber entity)
{
  SignalBuffer              signalBuffer       = kiNullBuffer;
  ApexReadIotOptCfgReq      *request_p;

  FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (MOBILITY,
      entity,
      SIG_APEX_READ_IOT_OPT_CFG_CNF);

  KiCreateZeroSignal (SIG_APEX_READ_IOT_OPT_CFG_REQ,
      sizeof (ApexReadIotOptCfgReq),
      &signalBuffer);
  request_p                    = &signalBuffer.sig->apexReadIotOptCfgReq;
  request_p->taskId            = VG_CI_TASK_ID;
  KiSendSignal (TASK_BL_ID, &signalBuffer);
}

/*************************************************************************
*
* Function:    vgApexReadPsmConfReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/
  void vgApexReadPsmConfReq(const VgmuxChannelNumber entity)
  {
    SignalBuffer              signalBuffer       = kiNullBuffer;
    ApexReadPsmConfReq        *request_p;

    FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_READ_PSM_CONF_CNF);

    KiCreateZeroSignal (SIG_APEX_READ_PSM_CONF_REQ,
        sizeof (ApexReadPsmConfReq),
        &signalBuffer);
    request_p                    = &signalBuffer.sig->apexReadPsmConfReq;
    request_p->taskId            = VG_CI_TASK_ID;
    KiSendSignal (TASK_BL_ID, &signalBuffer);
  }
/*************************************************************************
*
* Function:     vgApexWritePsmConfReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/

void vgApexWritePsmConfReq(const VgmuxChannelNumber entity)
{
  SignalBuffer               signalBuffer       = kiNullBuffer;
  ApexWritePsmConfReq        *request_p;
  MobilityContext_t   *mobilityContext_p   = ptrToMobilityContext ();
  VgReqCpsmsData         *reqCpsmsData_p   = &(mobilityContext_p->vgReqCpsmsData);

  FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (MOBILITY,
      entity,
      SIG_APEX_WRITE_PSM_CONF_CNF);

  KiCreateZeroSignal (SIG_APEX_WRITE_PSM_CONF_REQ,
      sizeof (ApexWritePsmConfReq),
      &signalBuffer);
  request_p = &signalBuffer.sig->apexWritePsmConfReq;
  request_p->taskId = VG_CI_TASK_ID;
  if (reqCpsmsData_p->mode == VG_CPSMS_DISABLE_OR_RESET_TO_DEFAULTS)
  {
     request_p->psm.resetToDefaults = TRUE;
     request_p->psm.psmSupport  = FALSE;
  }
  else if (reqCpsmsData_p->mode == VG_CPSMS_DISABLE)
  {
     request_p->psm.resetToDefaults = FALSE;
     request_p->psm.psmSupport  = FALSE;
  }
  else
  {
     request_p->psm.resetToDefaults = FALSE;
     request_p->psm.psmSupport  = TRUE;
     request_p->psm.psmValues.activeTime = reqCpsmsData_p->reqActiveTime;
     request_p->psm.psmValues.periodicTau = reqCpsmsData_p->reqTau;
     request_p->psm.periodicTauPresent = reqCpsmsData_p->reqTauPresent;
     request_p->psm.activeTimePresent = reqCpsmsData_p->reqActiveTimePresent;
  }
  KiSendSignal (TASK_BL_ID, &signalBuffer);
}

/*************************************************************************
*
* Function:    vgApexReadAttachPdnConfReq
*
* Parameters:   entity
*
* Returns:      void
*
* Description:
*
*************************************************************************/
void vgApexReadAttachPdnConfReq(const VgmuxChannelNumber entity)
{
  SignalBuffer              signalBuffer       = kiNullBuffer;
  ApexMmReadAttachPdnCfgReq *request_p;

  FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (MOBILITY,
      entity,
      SIG_APEX_MM_READ_ATTACH_PDN_CONF_CNF);

  KiCreateZeroSignal (SIG_APEX_MM_READ_ATTACH_PDN_CONF_REQ,
      sizeof (ApexMmReadAttachPdnCfgReq),
      &signalBuffer);
  request_p                    = &signalBuffer.sig->apexMmReadAttachPdnCfgReq;
  request_p->taskId            = VG_CI_TASK_ID;
  KiSendSignal (TASK_BL_ID, &signalBuffer);
}

/*************************************************************************
*
* Function:    vgApexWriteAttachPdnConfReq
*
* Parameters:   entity
*
* Returns:      void
*
* Description:
*
*************************************************************************/
void vgApexWriteAttachPdnConfReq(const VgmuxChannelNumber entity)
{
  SignalBuffer               signalBuffer       = kiNullBuffer;
  ApexMmWriteAttachPdnCfgReq *request_p;
  MobilityContext_t          *mobilityContext_p   = ptrToMobilityContext ();

  FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (MOBILITY,
      entity,
      SIG_APEX_MM_WRITE_ATTACH_PDN_CONF_CNF);

  KiCreateZeroSignal (SIG_APEX_MM_WRITE_ATTACH_PDN_CONF_REQ,
      sizeof (ApexMmWriteAttachPdnCfgReq),
      &signalBuffer);

  request_p = &signalBuffer.sig->apexMmWriteAttachPdnCfgReq;
  request_p->taskId = VG_CI_TASK_ID;

  if (mobilityContext_p->vgReqCipcaOpt == VG_CIPCA_ATTACH_WITH_PDN)
  {
    request_p->attachWithoutPdn = FALSE;
  }
  else
  {
    request_p->attachWithoutPdn = TRUE;
  }

  KiSendSignal (TASK_BL_ID, &signalBuffer);
}

#if defined (FEA_NFM)
/*************************************************************************
*
* Function:    vgApexReadNfmReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/
void vgApexReadNfmReq(const VgmuxChannelNumber entity)
{
   SignalBuffer          signalBuffer       = kiNullBuffer;
   ApexReadNfmReq        *request_p;

   FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

   sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_READ_NFM_CNF);

   KiCreateZeroSignal (SIG_APEX_READ_NFM_REQ,
        sizeof (ApexReadNfmReq),
        &signalBuffer);
   request_p                    = &signalBuffer.sig->apexReadNfmReq;
   request_p->taskId            = VG_CI_TASK_ID;
   KiSendSignal (TASK_BL_ID, &signalBuffer);
}

/*************************************************************************
*
* Function:     vgApexWriteNfmReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/

void vgApexWriteNfmReq(const VgmuxChannelNumber entity)
{
  SignalBuffer           signalBuffer       = kiNullBuffer;
  ApexWriteNfmReq        *request_p;
  MobilityContext_t      *mobilityContext_p  = ptrToMobilityContext ();
  VgReqNfmData           *vgReqNfmData_p    = &(mobilityContext_p->vgReqNfmData);

  FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (MOBILITY,
      entity,
      SIG_APEX_WRITE_NFM_CNF);

  KiCreateZeroSignal (SIG_APEX_WRITE_NFM_REQ,
      sizeof (ApexWriteNfmReq),
      &signalBuffer);
  request_p = &signalBuffer.sig->apexWriteNfmReq;
  request_p->taskId = VG_CI_TASK_ID;
  request_p->writeNfmReq.nfmActivePresent = TRUE;
  request_p->writeNfmReq.nfmActive = vgReqNfmData_p->reqNfmActive;
  if (vgReqNfmData_p->reqStartTimerActivePresent)
  {
     request_p->writeNfmReq.startTimerActivePresent = TRUE;
     request_p->writeNfmReq.startTimerActive  = vgReqNfmData_p->reqStartTimerActive;
  }
  else
  {
     request_p->writeNfmReq.startTimerActivePresent = FALSE;
     request_p->writeNfmReq.startTimerActive  = FALSE;
  }
  request_p->writeNfmReq.stPar = vgReqNfmData_p->reqStPar;
  request_p->writeNfmReq.stParPresent = vgReqNfmData_p->reqStParPresent;

  request_p->writeNfmReq.stTmPresent = vgReqNfmData_p->reqStTmPresent;
  request_p->writeNfmReq.stTm = vgReqNfmData_p->reqStTm;
  KiSendSignal (TASK_BL_ID, &signalBuffer);
}

/*************************************************************************
*
* Function:    vgApexReadNfmConfReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/
void vgApexReadNfmConfReq(const VgmuxChannelNumber entity)
{
   SignalBuffer          signalBuffer       = kiNullBuffer;
   ApexReadNfmConfReq    *request_p;

   FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

   sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_READ_NFM_CONF_CNF);

   KiCreateZeroSignal (SIG_APEX_READ_NFM_CONF_REQ,
        sizeof (ApexReadNfmConfReq),
        &signalBuffer);
   request_p                    = &signalBuffer.sig->apexReadNfmConfReq;
   request_p->taskId            = VG_CI_TASK_ID;
   KiSendSignal (TASK_BL_ID, &signalBuffer);
}

/*************************************************************************
*
* Function:     vgApexWriteNfmConfReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/

void vgApexWriteNfmConfReq(const VgmuxChannelNumber entity)
{
  SignalBuffer           signalBuffer        = kiNullBuffer;
  ApexWriteNfmConfReq    *request_p;
  MobilityContext_t      *mobilityContext_p  = ptrToMobilityContext ();
  VgReqNfmcData          *vgReqNfmcData_p    = &(mobilityContext_p->vgReqNfmcData);
  Int8                   index;

  FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

  sendSsRegistrationSignal (MOBILITY,
      entity,
      SIG_APEX_WRITE_NFM_CONF_CNF);

  KiCreateZeroSignal (SIG_APEX_WRITE_NFM_CONF_REQ,
      sizeof (ApexWriteNfmConfReq),
      &signalBuffer);
  request_p = &signalBuffer.sig->apexWriteNfmConfReq;
  request_p->taskId = VG_CI_TASK_ID;
  for (index = 0; index < MAX_NFM_PAR_VALUE; index++)
  {
      request_p->writeNfmConfReq.nfmPar[index] = vgReqNfmcData_p->reqNfmPar[index];
      request_p->writeNfmConfReq.nfmParPresent[index] = vgReqNfmcData_p->reqNfmParPresent[index];
  }
  request_p->writeNfmConfReq.stPar = vgReqNfmcData_p->reqStPar;
  request_p->writeNfmConfReq.stParPresent = vgReqNfmcData_p->reqStParPresent;

  KiSendSignal (TASK_BL_ID, &signalBuffer);
}

/*************************************************************************
*
* Function:    vgApexReadNfmStatusReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/
void vgApexReadNfmStatusReq(const VgmuxChannelNumber entity)
{
   SignalBuffer          signalBuffer       = kiNullBuffer;
   ApexReadNfmStatusReq  *request_p;

   FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

   sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_READ_NFM_STATUS_CNF);

   KiCreateZeroSignal (SIG_APEX_READ_NFM_STATUS_REQ,
        sizeof (ApexReadNfmStatusReq),
        &signalBuffer);
   request_p                    = &signalBuffer.sig->apexReadNfmStatusReq;
   request_p->taskId            = VG_CI_TASK_ID;
   KiSendSignal (TASK_BL_ID, &signalBuffer);
}

#endif /* FEA_NFM */



/*************************************************************************
*
* Function:     vgApexMmLockArfcnReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/

void vgApexMmLockArfcnReq (const VgmuxChannelNumber entity)
{
    SignalBuffer          sigBuff       = kiNullBuffer;
    ApexMmLockArfcnReq   *request_p;
    VgMFRCLLCKData       *vgMFRCLLCKData   = &(ptrToMobilityContext()->vgMfrcllckData);

    FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_MM_LOCK_ARFCN_CNF);

    KiCreateZeroSignal (SIG_APEX_MM_LOCK_ARFCN_REQ,
        sizeof (ApexMmLockArfcnReq),
        &sigBuff);

    request_p             = &sigBuff.sig->apexMmLockArfcnReq;
    request_p->taskId     = VG_CI_TASK_ID;
    request_p->lockArfcnCmdMode = vgMFRCLLCKData->lockArfcnCmdMode;
    
    if(vgMFRCLLCKData->lockArfcnCmdMode == MM_LOCK_ARFCN_CMD_MODE_SET)
    {
      request_p->lockStatus = vgMFRCLLCKData->lockStatus;
      
      if (VG_MFRCLLCK_LOCK_ACTIVATE_LOCK == vgMFRCLLCKData->lockStatus)
      {
          request_p->lockInfo.earfcn = vgMFRCLLCKData->earfcn;

          request_p->lockInfo.earfcnOffset        = vgMFRCLLCKData->earfcnOffset;

          if (TRUE == vgMFRCLLCKData->isPciValid)
          {
              request_p->lockInfo.pciPresent = TRUE;
              request_p->lockInfo.pci        = vgMFRCLLCKData->pci;
              request_p->lockInfo.byPassSCriteria = vgMFRCLLCKData->byPassSCriteria;
          }
          else
          {
              request_p->lockInfo.pciPresent = FALSE;
          }
          
      }
      else 
      {
          request_p->lockInfo.earfcn              = 0;
          request_p->lockInfo.pciPresent          = FALSE;
      }
    }
    else
    {
      request_p->lockStatus = FALSE;
      request_p->lockInfo.earfcn = 0;
      request_p->lockInfo.pciPresent = FALSE;
    }
    
    KiSendSignal (TASK_BL_ID, &sigBuff);
} /* vgApexMmLockArfcnReq */

/*************************************************************************
*
* Function:     vgApexMmSearchBandListReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/

void vgApexMmSearchBandListReq (const VgmuxChannelNumber entity)
{
    SignalBuffer              sigBuff       = kiNullBuffer;
    ApexMmSearchBandListReq   *request_p;
    VgMBANDSLData             *vgMANDSLData = &(ptrToMobilityContext()->vgMandSLData);
    Int8 i=0;
    FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_MM_SEARCH_BAND_LIST_CNF);

    KiCreateZeroSignal (SIG_APEX_MM_SEARCH_BAND_LIST_REQ,
        sizeof (ApexMmSearchBandListReq),
        &sigBuff);

    request_p             = &sigBuff.sig->apexMmSearchBandListReq;
    request_p->taskId     = VG_CI_TASK_ID;    
    request_p->numSearchBand = vgMANDSLData->numSearchBand;
    for(i=0;i<vgMANDSLData->numSearchBand;i++)
    {
        request_p->searchBandList[i] = vgMANDSLData->searchBandList[i];
    }
    
    KiSendSignal (TASK_BL_ID, &sigBuff);
}


/*************************************************************************
*
* Function:     vgApexMmUeStatsReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/
void vgApexMmUeStatsReq (const VgmuxChannelNumber entity)
{
    ApexMmUeStatsReq           *request_p;
    SignalBuffer                sigBuff        = kiNullBuffer;
    VgMENGINFOStatisData       *vgMengInfoData = &(ptrToMobilityContext()->vgMengInfoData);

    FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_MM_UE_STATS_CNF);

    KiCreateZeroSignal (SIG_APEX_MM_UE_STATS_REQ,
        sizeof (ApexMmUeStatsReq),
        &sigBuff);

    request_p                = &(sigBuff.sig->apexMmUeStatsReq);
    request_p->taskId        = VG_CI_TASK_ID;
    request_p->requestedInfo = (RequestedModemInfo)(vgMengInfoData->queryMode);
    
    KiSendSignal (TASK_BL_ID, &sigBuff);
}/*vgApexMmUeStatsReq*/

/*************************************************************************
*
* Function:     vgApexMmLocCellInfoReq
*
* Parameters:   void
*
* Returns:      void
*
* Description: To query Location Cell info
*
*************************************************************************/
void vgApexMmLocCellInfoReq (const VgmuxChannelNumber entity)
{
    ApexMmLocCellInfoReq        *request_p;
    SignalBuffer                sigBuff        = kiNullBuffer;

    FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_MM_LOC_CELL_INFO_CNF);

    KiCreateZeroSignal (SIG_APEX_MM_LOC_CELL_INFO_REQ,
        sizeof (ApexMmLocCellInfoReq),
        &sigBuff);

    request_p                = &(sigBuff.sig->apexMmLocCellInfoReq);
    request_p->taskId        = VG_CI_TASK_ID;
    
    KiSendSignal (TASK_BL_ID, &sigBuff);
}/*vgApexMmLocCellInfoReq*/


/*************************************************************************
*
* Function:     vgApexMmSetEhplmnReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:  Set EHPLMN list to ABMM
*
*************************************************************************/

void vgApexMmSetEhplmnReq (const VgmuxChannelNumber entity)
{
    SignalBuffer              sigBuff       = kiNullBuffer;
    ApexMmSetEhplmnReq        *request_p;
    VgMEHPLMNData             *vgMEHPLMNData = &(ptrToMobilityContext()->vgMehplmnData);
    Int8 i=0;
    FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

    KiCreateZeroSignal (SIG_APEX_MM_SET_EHPLMN_REQ,
        sizeof (ApexMmSetEhplmnReq),
        &sigBuff);

    request_p             = &sigBuff.sig->apexMmSetEhplmnReq;
    request_p->taskId     = VG_CI_TASK_ID;    
    request_p->numEhplmn  = vgMEHPLMNData->numEhplmn;
   
    for(i = 0;i<vgMEHPLMNData->numEhplmn;i++)
    {
        request_p->ehplmnList[i] = vgMEHPLMNData->ehplmnList[i];
    }
    
    KiSendSignal (TASK_BL_ID, &sigBuff);
}

#if defined (FEA_RPM)
/*************************************************************************
*
* Function:    vgApexReadNfmReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/
void vgApexReadRpmReq(const VgmuxChannelNumber entity)
{
   SignalBuffer           signalBuffer       = kiNullBuffer;
   ApexRpmReadInfoReq     *request_p;
   MobilityContext_t      *mobilityContext_p  = ptrToMobilityContext ();
   VgMrpmData             *vgMrpmData_p    = &(mobilityContext_p->vgMrpmData);

   sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_RPM_READ_INFO_CNF);

   KiCreateZeroSignal (SIG_APEX_RPM_READ_INFO_REQ,
        sizeof (ApexRpmReadInfoReq),
        &signalBuffer);
   request_p               = &signalBuffer.sig->apexRpmReadInfoReq;
   if(ABRPM_READ_AREA_SIM == vgMrpmData_p->read_area)
   {
       request_p->area  = ABRPM_READ_AREA_SIM;
   }
   else
   {
       request_p->area  = ABRPM_READ_AREA_NVRAM;
   }
   KiSendSignal (TASK_BL_ID, &signalBuffer);
}
#endif
/*************************************************************************
*
* Function:     vgApexMmSearchBandListReq
*
* Parameters:   void
*
* Returns:      void
*
* Description:
*
*************************************************************************/

void vgApexMmDisableHplmnSearchReq (const VgmuxChannelNumber entity)
{
    SignalBuffer              sigBuff       = kiNullBuffer;
    ApexMmDisableHplmnSearchReq   *request_p;
    VgMHPLMNSData             *vgMHPLMNSData = &(ptrToMobilityContext()->vgMhplmnData);
    Int8 i=0;
    FatalAssert (vgChManCheckHaveControl (CC_MOBILITY_MANAGEMENT, entity) == TRUE);

    sendSsRegistrationSignal (MOBILITY,
        entity,
        SIG_APEX_MM_DISABLE_HPLMN_SEARCH_CNF);

    KiCreateZeroSignal (SIG_APEX_MM_DISABLE_HPLMN_SEARCH_REQ,
        sizeof (ApexMmDisableHplmnSearchReq),
        &sigBuff);

    request_p             = &sigBuff.sig->apexMmDisableHplmnSearchReq;
    request_p->taskId     = VG_CI_TASK_ID;    
    request_p->isDisableHplmnSearch = vgMHPLMNSData->enable;
 
    KiSendSignal (TASK_BL_ID, &sigBuff);
}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

