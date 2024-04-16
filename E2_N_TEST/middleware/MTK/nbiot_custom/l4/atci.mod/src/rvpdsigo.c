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
 * This file handles sending of signals to the ABPD module part of the
 * application background layer (AB).
 **************************************************************************/

#define MODULE_NAME "RVPDSIGO"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <gkisig.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvpdsigo.h>
#include <rvchman.h>
#include <rvcimxsot.h>
#include <rvgput.h>
#include <rlc_sig.h>
#include <vgmx_sig.h>
#include <abpd_sig.h>
#include <rvcimxut.h>
#include <rvcmux.h>
#include <rvstkrnat.h>
#include <rvmmut.h>
/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  ApexAbpdChannelAllocReq      apexAbpdChannelAllocReq;
  ApexAbpdChannelFreeReq       apexAbpdChannelFreeReq;

  ApexAbpdDialReq              apexAbpdDialReq;
  ApexAbpdActivateDataConnReq  apexAbpdActivateDataConnReq;
  ApexAbpdConnectRsp           apexAbpdConnectRsp;
  ApexAbpdConnectRej           apexAbpdConnectRej;
  ApexAbpdHangupReq            apexAbpdHangupReq;

#if defined (FEA_QOS_TFT)
  ApexAbpdPsdModifyReq         apexAbpdPsdModifyReq;
#endif /* FEA_QOS_TFT */

  ApexAbpdSetupRsp             apexAbpdSetupRsp;
  ApexAbpdPsdAttachReq         apexAbpdPsdAttachReq;
  ApexAbpdPsdDetachReq         apexAbpdPsdDetachReq;
#if defined (FEA_PPP)  
  ApexAbpdPppLoopbackReq       apexAbpdPppLoopbackReq;
  ApexAbpdPppConfigReq         apexAbpdPppConfigReq;
  ApexAbpdPppConfigAuthReq     apexAbpdPppConfigAuthReq;
#endif /* FEA_PPP */  
  ApexAbpdReportCounterReq     apexAbpdReportCounterReq;
  ApexAbpdResetCounterReq      apexAbpdResetCounterReq;
  ApexAbpdApnReadReq           apexAbpdApnReadReq;
  ApexAbpdApnWriteReq          apexAbpdApnWriteReq;
#if defined (FEA_UPDIR)
  ApexAbpdSetUpdirInfoReq      apexAbpdSetUpdirInfoReq;
  ApexAbpdResetUpdirInfoReq    apexAbpdResetUpdirInfoReq;
#endif
  ApexAbpdListAclReq           apexAbpdListAclReq;
  ApexAbpdWriteAclReq          apexAbpdWriteAclReq;
  ApexAbpdDeleteAclReq         apexAbpdDeleteAclReq;
  ApexAbpdSetAclReq            apexAbpdSetAclReq;
  ApexAbpdAclStatusReq         apexAbpdAclStatusReq;
  ApexAbpdWriteRelAssistReq    apexAbpdWriteRelAssistReq;
  ApexAbpdReadRelAssistReq     apexAbpdReadRelAssistReq; 

  ApexSmReadRouteReq        apexSmReadRouteReq;
  ApexSmWriteRouteReq       apexSmWriteRouteReq;

  RlcmacDataReq             rlcmacDataReq;
  RlcmacUnitDataReq         rlcmacUnitDataReq;
};


/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static VgPsdBearerInfo *vgGetPsdBearerInfo (Int8 cid, const VgmuxChannelNumber entity);
#if defined (FEA_QOS_TFT)
static TftOperationCode vgCiGetTftOpCode (TrafficFlowTemplate *requiredTft,
                                 TrafficFlowTemplate *negotiatedTft);
#endif
/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*************************************************************************
*
* Function:     vgGetPsdBearerInfo
*
* Parameters:   cid        - CID identifying the connection.
*               entity     - Entity associated with the CID
*
* Returns:      Pointer to the PSD bearer data for the given CID.
*
* Description:  This function is called following an *99# / CGDATA / CGACT connection
*               request/
*               The current PSD activation data needs to be sent to
*               the ABPD task so this returns a pointer to the PsdBearerInfo
*               applicable to the channel.
*
*************************************************************************/

static VgPsdBearerInfo *vgGetPsdBearerInfo (Int8 cid, const VgmuxChannelNumber entity)
{
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdBearerInfo      *psdBearerInfo_p      = PNULL;

  /* Lock the CID to this channel.
   */
  gprsGenericContext_p->cidOwner[cid] = entity;

  if (gprsGenericContext_p->cidUserData[cid] != PNULL)
  {
    if (gprsGenericContext_p->cidUserData[cid]->profileDefined == FALSE)
    {
     /* Set the profile reset flag so the PDP context
      * data is reset to their default states */
      gprsGenericContext_p->cidUserData[cid]->pendingUnsolicitedContextActivation = TRUE;
      gprsGenericContext_p->cidUserData[cid]->profileDefined = TRUE;
    }

    psdBearerInfo_p = &(gprsGenericContext_p->cidUserData[cid]->psdBearerInfo);
  }

  /* Return a pointer to the PSD Bearer info for this cid */
  return (psdBearerInfo_p);
}

#if defined (FEA_QOS_TFT)
/*************************************************************************
*
* Function:     vgCiGetTftOpCode
*
* Parameters:   requiredTft - required TFT
*               negotiatedTft - negotiated TFT
*
* Returns:      TftOpCode
*
* Description:
*
*************************************************************************/
static TftOperationCode vgCiGetTftOpCode (TrafficFlowTemplate *requiredTft,
                                 TrafficFlowTemplate *negotiatedTft)
{
    TftOperationCode    tftOpcode           = TFT_OPCODE_SPARE;
    PacketFilter        *reqPacketFilter    = requiredTft->packetFilterData;
    PacketFilter        *negPacketFilter    = negotiatedTft->packetFilterData;
    Int16               reqPfIndex,negPfIndex;
    Boolean             found               = FALSE;

    if (0 == negotiatedTft->numPacketFilters)
    {
      tftOpcode = TFT_OPCODE_CREATE_NEW;
    }
    else
    if (0 == requiredTft->numPacketFilters)
    {
      tftOpcode = TFT_OPCODE_DELETE_EXISTING;
    }
    else
    {
      for (reqPfIndex = 0; reqPfIndex < MAX_PFS_IN_TFT; reqPfIndex++)
      {
        found = FALSE;
        if (0 != reqPacketFilter[reqPfIndex].packetFilterId)
        {
          for (negPfIndex = 0; negPfIndex < MAX_PFS_IN_TFT; negPfIndex++)
          {
            if (reqPacketFilter[reqPfIndex].packetFilterId ==
                  negPacketFilter[negPfIndex].packetFilterId)
            {
              tftOpcode     = TFT_OPCODE_REPLACE_PACKET_FILTERS;
              found         = TRUE;
              break;
            }
          }
          /* Currently we assume the replace action is prior to add action,
             it maybe change in the future */
          if (FALSE == found && TFT_OPCODE_REPLACE_PACKET_FILTERS != tftOpcode)
          {
             tftOpcode = TFT_OPCODE_ADD_PACKET_FILTERS;
          }
        }
      }
    }

    FatalCheck(tftOpcode != TFT_OPCODE_SPARE, requiredTft->numPacketFilters, negotiatedTft->numPacketFilters, 0);

    return (tftOpcode);
}
#endif
/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*************************************************************************
*
* Function:     vgCiApnToNetworkAPN
*
* Parameters:   inAPN  - APN in AT command input/output format
*               outAPN - APN in PSD network format
*
* Returns:      nothing
*
* Description:  converts APN from type "co.cellco.co.uk" to a type
*               like "2co6cellco2co2uk"
*
*************************************************************************/

void vgCiApnToNetworkAPN        (TextualAccessPointName inAPN,
                                 AccessPointName        *outAPN)
{
  Int8    *inPtr  = inAPN.name;
  Int8    *outPtr = (outAPN->name) + 1;  /* The output pointer is offset by 1
                                          * to make room for the first number
                                          */
  Int8    *charCountPtr = outAPN->name;
  Int8    index = 0;
  Int8    charSegmentCount = 0;
  Int8    outputLength = 1;  /* Start at 1 for the first number */

  if (inAPN.length == 0)
  {
    /* No APN present so simply set the length of the network APN to 0 */
    outAPN->length = 0;
  }
  else
  {
    for (index = 0; index < inAPN.length ; index++)
    {
      if (*inPtr == DOT_CHAR)
      {
        /* We have a dot so we need to set the counter value in the output
         * string and thne move to the next one.
         */
        *charCountPtr = charSegmentCount;
        charCountPtr = outPtr;
        charSegmentCount = 0;
      }
      else
      {
        charSegmentCount++;
        *outPtr = *inPtr;
      }

      /* Increment the pointers / output length */
      outputLength++;
      outPtr++;
      inPtr++;
    }

    /* Got to the end - so now set the last number */
    *charCountPtr = charSegmentCount;
    outAPN->length = outputLength;
  }
}

/*************************************************************************
*
* Function:     vgApexSmReadRouteReq
*
* Parameters:   entity - mux channel number
*
* Returns:      nothing
*
* Description:  sends a request to read current SMS routing information
*
*************************************************************************/
void vgApexSmReadRouteReq (const VgmuxChannelNumber entity)
{
  SignalBuffer       sigBuff = kiNullBuffer;
  ApexSmReadRouteReq *request_p;

  FatalAssert (vgChManCheckHaveControl (CC_SHORT_MESSAGE_SERVICE, entity) == TRUE);

  sendSsRegistrationSignal (PACKET_DATA,
                             entity,
                              SIG_APEX_SM_READ_ROUTE_CNF);

  KiCreateZeroSignal (SIG_APEX_SM_READ_ROUTE_REQ,
                       sizeof (ApexSmReadRouteReq),
                        &sigBuff);

  request_p = (ApexSmReadRouteReq *)sigBuff.sig;
  request_p->taskId     = VG_CI_TASK_ID;
  request_p->commandRef = (Int16)entity;

  KiSendSignal (TASK_BL_ID, &sigBuff);
}


/*************************************************************************
 *
 * Function:     vgApexSmWriteRouteReq
 *
 * Parameters:   entity - mux channel number
 *
 * Returns:      nothing
 *
 * Description:  sends a request to write SMS routing information
 *
 *************************************************************************/
void vgApexSmWriteRouteReq (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  SignalBuffer          sigBuff = kiNullBuffer;
  ApexSmWriteRouteReq   *request_p;

  FatalAssert (vgChManCheckHaveControl (CC_SHORT_MESSAGE_SERVICE, entity) == TRUE);

  sendSsRegistrationSignal (PACKET_DATA,
                             entity,
                              SIG_APEX_SM_WRITE_ROUTE_CNF);

  KiCreateZeroSignal (SIG_APEX_SM_WRITE_ROUTE_REQ,
                       sizeof (ApexSmWriteRouteReq),
                        &sigBuff);

  request_p = (ApexSmWriteRouteReq *)sigBuff.sig;
  request_p->taskId     = VG_CI_TASK_ID;
  request_p->commandRef = (Int16)entity;
  request_p->smsRoute   = gprsGenericContext_p->defaultSmsRoute;

  KiSendSignal (TASK_BL_ID, &sigBuff);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdChannelAllocReq
 *
 * Parameters:  entity         entity requesting to allocate the channel.
 *
 * Returns:     Nothing
 *
 * Description: Routine to send ApexAbpdChannelAllocReq to ABPD to reserve
 *              a channel for internal atci operations
 *
 *-------------------------------------------------------------------------*/
void vgApexAbpdChannelAllocReq (const VgmuxChannelNumber entity)
{
   SignalBuffer allocSig = kiNullBuffer;


   sendSsRegistrationSignal (PACKET_DATA,
                             entity,
                             SIG_APEX_ABPD_CHANNEL_ALLOC_CNF);

   KiCreateZeroSignal(SIG_APEX_ABPD_CHANNEL_ALLOC_REQ,
                          sizeof(ApexAbpdChannelAllocReq),
                          &allocSig);

   allocSig.sig->apexAbpdChannelAllocReq.connectionLayerTaskId = UNKNOWN_TASK_ID;
   allocSig.sig->apexAbpdChannelAllocReq.sourceTaskId = VG_CI_TASK_ID;

   KiSendSignal(TASK_BL_ID, &allocSig);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdChannelFreeReq
 *
 * Parameters:  entity         entity requesting to free the channel
 *              entityToFree   entity to be released.
 *
 * Returns:     Nothing
 *
 * Description: Routine to send ApexAbpdChannelFreeReq to ABPD to free
 *              the channel reserved for internal atci operations.  It also
 *              then initiates the channel being freed within the ATCI task
 *              depending on the request.
 *
 *-------------------------------------------------------------------------*/
void vgApexAbpdChannelFreeReq (const VgmuxChannelNumber entity,
                               const VgmuxChannelNumber entityToFree)
{
  SignalBuffer            freeSig               = kiNullBuffer;
  SignalBuffer            sigBuff               = kiNullBuffer;
  StkEntityGenericData_t  *stkGenericContext_p  = ptrToStkGenericContext ();
  GprsGenericContext_t    *gprsGenericContext_p = ptrToGprsGenericContext ();
  ChannelContext_t        *channelContext_p     = ptrToChannelContext(entityToFree);
  CiMuxChannelDisabledInd *request_p;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(stkGenericContext_p != PNULL, entity, 0, 0);
  FatalCheck(gprsGenericContext_p != PNULL, entity, 0, 0);
  FatalCheck(channelContext_p != PNULL, entity, 0, 0);
#endif
  /* There is no CNF signal to this request - so just send the signal
   * and then handle the channel shutdown
   */
  KiCreateZeroSignal(SIG_APEX_ABPD_CHANNEL_FREE_REQ,
                        sizeof(ApexAbpdChannelFreeReq),
                        &freeSig);

  freeSig.sig->apexAbpdChannelFreeReq.sourceTaskId = VG_CI_TASK_ID;
  freeSig.sig->apexAbpdChannelFreeReq.channelNumber = entityToFree;

  KiSendSignal(TASK_BL_ID, &freeSig);

  /* for STK usage the atCommand buffer should be freed whether or not the MUX has
   * released the channel */
  if (stkGenericContext_p->runAtCmdState == STATE_FREEING_CHANNEL)
  {
    FatalCheck(entityToFree == stkGenericContext_p->atCommandData.cmdEntity,
               entity, 0, 0);

    /* Remember the fact that the channel is disabling locally */
    channelContext_p->channelDisablingLocally = TRUE;

    /* need to set the data so the channel is disabled */
    KiCreateZeroSignal (SIG_CIMUX_CHANNEL_DISABLED_IND,
                        sizeof (CiMuxChannelDisabledInd),
                        &sigBuff);
    request_p  = (CiMuxChannelDisabledInd *) sigBuff.sig;
    request_p->channelNumber = stkGenericContext_p->atCommandData.cmdEntity;
    KiSendSignal (VG_CI_TASK_ID, &sigBuff);

    vgStkClearStkRunAtCommandData();
  }
  else if (stkGenericContext_p->runAtCmdState == STATE_FREEING_CHANNEL_ENABLE_FAILED)
  {
    FatalCheck(entityToFree == stkGenericContext_p->atCommandData.cmdEntity,
               entity, 0, 0);
    /* Channel enable failed so we don't need to disable it.
     */
    vgStkRunAtCommandFail(entity);
  }
  else
  {
    /* Unexpected request to free a channel */
    FatalParam(entityToFree, entity, 0);
  }
}

/*************************************************************************
*
* Function:     vgApexAbpdPsdAttachReq
*
* Parameters:   entity - mux channel number
*
* Returns:      nothing
*
* Description:  Sends a PSD attach request to the AB task (ABPD module).  We
*               will get CNF signal back.
*
*************************************************************************/
void vgApexAbpdPsdAttachReq (const VgmuxChannelNumber entity)
{
  SignalBuffer           sigBuff = kiNullBuffer;
  ApexAbpdPsdAttachReq *request_p;

  FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

  sendSsRegistrationSignal (PACKET_DATA,
                             entity,
                              SIG_APEX_ABPD_PSD_ATTACH_CNF);

  KiCreateZeroSignal (SIG_APEX_ABPD_PSD_ATTACH_REQ,
                       sizeof (ApexAbpdPsdAttachReq),
                        &sigBuff);

  request_p = (ApexAbpdPsdAttachReq *)sigBuff.sig;
  request_p->sourceTaskId = VG_CI_TASK_ID;

  KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*************************************************************************
*
* Function:     vgApexAbpdPsdDetachReq
*
* Parameters:   entity - mux channel number
*
* Returns:      nothing
*
* Description:  This routine sends a PSD detach request to the AB task
*               (ABPD module) to request that the ME detaches from the PSD
*               network.
*
*************************************************************************/
void vgApexAbpdPsdDetachReq (const VgmuxChannelNumber entity)
{
  SignalBuffer          sigBuff = kiNullBuffer;
  ApexAbpdPsdDetachReq *request_p;


  FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

  sendSsRegistrationSignal (PACKET_DATA,
                             entity,
                              SIG_APEX_ABPD_PSD_DETACH_CNF);

  KiCreateZeroSignal (SIG_APEX_ABPD_PSD_DETACH_REQ,
                       sizeof (ApexAbpdPsdDetachReq),
                        &sigBuff);

  request_p = (ApexAbpdPsdDetachReq *)sigBuff.sig;
  request_p->sourceTaskId = VG_CI_TASK_ID;
  request_p->combinedDetach = FALSE;
  KiSendSignal (TASK_BL_ID, &sigBuff);
}


/*************************************************************************
*
* Function:     vgApexAbpdDialReq
*
* Parameters:   entity - mux channel number
*
* Returns:      nothing
*
* Description:  This routine sends an APEX_ABPD_DIAL_REQ signal
*               to the ABPD module part of the AB task.
*
*************************************************************************/
void vgApexAbpdDialReq          (const VgmuxChannelNumber entity)
{
  SignalBuffer            sigBuff = kiNullBuffer;
  ApexAbpdDialReq         *request_p;

  GeneralContext_t        *generalContext_p = ptrToGeneralContext (entity);
  GprsGenericContext_t    *gprsGenericContext_p = ptrToGprsGenericContext ();
  GprsContext_t           *gprsContext_p = ptrToGprsContext(entity);
  VgPsdBearerInfo         *psdBearerInfo_p = PNULL;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (generalContext_p != PNULL,                      entity, 0, 0);
  FatalCheck (gprsGenericContext_p != PNULL,                  entity, 0, 0);
  FatalCheck (gprsContext_p != PNULL,                         entity, 0, 0);
#endif
  FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

  /* no confirmation signal, so no register required.  We may get a variety
   * of responses to this signal - so we have to figure out which entity
   * it is for at that stage
   */
  KiCreateZeroSignal (SIG_APEX_ABPD_DIAL_REQ,
                       sizeof (ApexAbpdDialReq),
                        &sigBuff );

  request_p = (ApexAbpdDialReq *) sigBuff.sig;

  /* Fill in the signal fields */
  request_p->sourceTaskId = VG_CI_TASK_ID;
  request_p->channelNumber = (Int32)entity;

  /* Go get the information for the activation */
  psdBearerInfo_p = vgGetPsdBearerInfo (gprsContext_p->vgDialCid, entity);

  request_p->connType = psdBearerInfo_p->connType;
  request_p->apnPresent = psdBearerInfo_p->reqApnPresent;

  if (psdBearerInfo_p->reqApnPresent == TRUE)
  {
    request_p->apn = psdBearerInfo_p->reqApn;

  }

  request_p->reqPdnAddress = psdBearerInfo_p->reqPdnAddress;

  /* We always ask to do FDN check */
#if defined (ENABLE_ATCI_UNIT_TEST)  
  request_p->doFdnCheck = TRUE;
#else
  request_p->doFdnCheck = TRUE;
#ifdef MTK_NBIOT_TARGET_BUILD 
  request_p->doFdnCheck = FALSE;
#endif
#endif

  request_p->psdUser = psdBearerInfo_p->psdUser;

  request_p->minimumQosPresent = FALSE;    /* Never used for NB-IOT */

#if defined (FEA_QOS_TFT)
  request_p->qosPresent = psdBearerInfo_p->reqQosPresent;
  if (psdBearerInfo_p->reqQosPresent == TRUE)
  {
    request_p->qos = psdBearerInfo_p->requiredQos;
  }

  request_p->tftPresent = psdBearerInfo_p->reqTftPresent;
  if (psdBearerInfo_p->reqTftPresent == TRUE)
  {
    request_p->tft              = psdBearerInfo_p->requiredTft;
    request_p->tft.tftOpCode    = TFT_OPCODE_CREATE_NEW;
  }
#else
  request_p->qosPresent = FALSE;
  request_p->tftPresent = FALSE;
#endif /* vgPdGetPsdModifyReason */

  request_p->headerComp = psdBearerInfo_p->headerComp;
  request_p->dataComp = psdBearerInfo_p->dataComp;

  request_p->ipv4LinkMTURequest = psdBearerInfo_p->ipv4LinkMTURequest;
  request_p->nonIPLinkMTURequest = psdBearerInfo_p->nonIPLinkMTURequest;

#if defined(FEA_TEMP_REL14_ABPD_DUAL_PRIORITY)
  if(psdBearerInfo_p->nasSigPriority == VG_MS_NOT_CONFIGURED_FOR_LOW_PRIO)
  {
    request_p->deviceProperties = MS_NOT_CONFIGURED_FOR_LOW_PRIO;
  }
  else
  {
    request_p->deviceProperties = MS_CONFIGURED_FOR_LOW_PRIO;
  }
#endif
  
#if defined (FEA_DEDICATED_BEARER)
  request_p->secondaryContext = psdBearerInfo_p->secondaryContext;
  request_p->primaryConnId = psdBearerInfo_p->primaryConnId;
#else
  request_p->secondaryContext = FALSE;
  request_p->primaryConnId = INVALID_CONN_ID;
#endif /* FEA_DEDICATED_BEARER */

  /* Remember the flow control type we asked for */
  psdBearerInfo_p->flowControlType = (FlowCtrlType)getProfileValue (entity, PROF_IFC);

  /* Remember the channel we are on */
  psdBearerInfo_p->channelNumber = entity;

  /* For LTE: The flow control type of the secondary context must always match the primary.
   * This will always be the case for LTE as they will always use the same
   * channel.
   */
  request_p->flowControlType = (FlowCtrlType)getProfileValue (entity, PROF_IFC);

#if defined (FEA_PPP)
#if defined (ENABLE_PPP_RAW_LOGGING)
  request_p->rawLoggingLevel = getProfileValue (entity, PROF_MGPPPLOG);

  /* This may need to be set to something sensible */
  request_p->rawLoggerTask = UNKNOWN_TASK_ID;
#endif
#endif /* FEA_PPP */

  gprsGenericContext_p->cidUserData[gprsContext_p->vgDialCid]->pendingContextActivation = TRUE;

  KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*************************************************************************
*
* Function:     vgApexAbpdActivateDataConnReq
*
* Parameters:   entity - mux channel number
*
* Returns:      nothing
*
* Description:  This routine sends an APEX_ABPD_ACTIVATE_DATA_CONN_REQ signal
*               to the ABPD module part of the AB task.
*
*************************************************************************/
void vgApexAbpdActivateDataConnReq          (const VgmuxChannelNumber entity)
{
  SignalBuffer                   sigBuff = kiNullBuffer;
  ApexAbpdActivateDataConnReq    *request_p;

  GeneralContext_t               *generalContext_p = ptrToGeneralContext (entity);
  GprsGenericContext_t           *gprsGenericContext_p = ptrToGprsGenericContext ();
  GprsContext_t                  *gprsContext_p = ptrToGprsContext(entity);
  VgPsdBearerInfo                *psdBearerInfo_p = PNULL;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (generalContext_p != PNULL,                      entity, 0, 0);
  FatalCheck (gprsGenericContext_p != PNULL,                  entity, 0, 0);
  FatalCheck (gprsContext_p != PNULL,                         entity, 0, 0);
#endif
  FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

  /* no confirmation signal, so no register required.  We may get a variety
   * of responses to this signal - so we have to figure out which entity
   * it is for at that stage
   */
  KiCreateZeroSignal (SIG_APEX_ABPD_ACTIVATE_DATA_CONN_REQ,
                       sizeof (ApexAbpdActivateDataConnReq),
                        &sigBuff );

  request_p = (ApexAbpdActivateDataConnReq *) sigBuff.sig;

  /* Go get the information for the activation */
  psdBearerInfo_p = vgGetPsdBearerInfo (gprsContext_p->vgDialCid, entity);

  request_p->connId   = psdBearerInfo_p->connId;
  request_p->connType = psdBearerInfo_p->connType;

  gprsGenericContext_p->cidUserData[gprsContext_p->vgDialCid]->pendingDataConnectionActivation = TRUE;

  KiSendSignal (TASK_BL_ID, &sigBuff);
}


/*************************************************************************
*
* Function:     vgApexAbpdConnectRsp
*
* Parameters:   entity - mux channel number
*
* Returns:      nothing
*
* Description:  This routine sends an APEX_ABPD_CONNECT_RSP signal
*               to the ABPD module part of the AB task.
*
*************************************************************************/
void vgApexAbpdConnectRsp       (const VgmuxChannelNumber entity)
{
  SignalBuffer            sigBuff = kiNullBuffer;
  ApexAbpdConnectRsp      *request_p;
  GprsContext_t           *gprsContext_p        = ptrToGprsContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
  FatalCheck (gprsContext_p->activePsdBearerContextWithDataConn != PNULL, entity, 0, 0);
#endif
  KiCreateZeroSignal (SIG_APEX_ABPD_CONNECT_RSP,
                       sizeof (ApexAbpdConnectRsp),
                        &sigBuff );

  request_p = (ApexAbpdConnectRsp *) sigBuff.sig;

  request_p->connId = gprsContext_p->activePsdBearerContextWithDataConn->psdBearerInfo.connId;

  KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*************************************************************************
*
* Function:     vgApexAbpdConnectRej
*
* Parameters:   entity - mux channel number
*
* Returns:      nothing
*
* Description:  This routine sends an APEX_ABPD_CONNECT_REJ signal
*               to the ABPD module part of the AB task.
*
*************************************************************************/
void vgApexAbpdConnectRej       (const VgmuxChannelNumber entity)
{
  SignalBuffer            sigBuff = kiNullBuffer;
  ApexAbpdConnectRej      *request_p;
  GprsContext_t           *gprsContext_p        = ptrToGprsContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
  FatalCheck (gprsContext_p->activePsdBearerContextWithDataConn != PNULL, entity, 0, 0);
#endif
  KiCreateZeroSignal (SIG_APEX_ABPD_CONNECT_REJ,
                       sizeof (ApexAbpdConnectRej),
                        &sigBuff );

  request_p = (ApexAbpdConnectRej *) sigBuff.sig;

  request_p->connId = gprsContext_p->activePsdBearerContextWithDataConn->psdBearerInfo.connId;

  /* Cause is different depending on if we are on LTE or 2G/3G */
  if (vgIsCurrentAccessTechnologyLte())
  {
    request_p->cause  = ESM_CAUSE_INSUFFICIENT_RESOURCES;
  }
  else
  {
    request_p->cause  = SM_CAUSE_INSUFFIC_RESOURCES;
  }
  KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*************************************************************************
*
* Function:     vgApexAbpdHangupReq
*
* Parameters:   entity - mux channel number
*
* Returns:      nothing
*
* Description:  This routine sends an APEX_ABPD_HANGUP_REQ signal
*               to the ABPD module part of the AB task.
*
*************************************************************************/
void vgApexAbpdHangupReq        (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t    *gprsGenericContext_p = ptrToGprsGenericContext ();
  GprsContext_t           *gprsContext_p        = ptrToGprsContext(entity);
  SignalBuffer            sigBuff = kiNullBuffer;
  ApexAbpdHangupReq       *request_p;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsGenericContext_p != PNULL, entity, 0, 0);
  FatalCheck (gprsContext_p != PNULL,        entity, 0, 0);

  FatalCheck (gprsGenericContext_p->cidUserData[gprsContext_p->vgHangupCid] != PNULL,
              entity,
              gprsContext_p->vgHangupCid,
              0);
#endif
  FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

  KiCreateZeroSignal (SIG_APEX_ABPD_HANGUP_REQ,
                       sizeof(ApexAbpdHangupReq),
                        &sigBuff );

  request_p = (ApexAbpdHangupReq *) sigBuff.sig;

  request_p->connId = gprsGenericContext_p->cidUserData[gprsContext_p->vgHangupCid]->psdBearerInfo.connId;

  gprsGenericContext_p->cidUserData[gprsContext_p->vgHangupCid]->pendingContextDeactivation = TRUE;

  KiSendSignal (TASK_BL_ID, &sigBuff);
}

#if defined (FEA_QOS_TFT)
/*************************************************************************
*
* Function:     vgApexAbpdPsdModifyReq
*
* Parameters:   entity - mux channel number
*
* Returns:      nothing
*
* Description:  This routine sends an APEX_ABPD_PSD_MODIFY_REQ signal
*               to the ABPD module part of the AB task.
*
*************************************************************************/
void vgApexAbpdPsdModifyReq     (const VgmuxChannelNumber entity)
{
  SignalBuffer              sigBuff                 = kiNullBuffer;
  ApexAbpdPsdModifyReq      *request_p              = PNULL;
  GprsGenericContext_t      *gprsGenericContext_p   = ptrToGprsGenericContext ();
  GprsContext_t             *gprsContext_p          = ptrToGprsContext (entity);
  VgPsdStatusInfo           *cmd_p                  = PNULL;
  VgPsdBearerInfo           *psdBearerInfo_p        = PNULL;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsGenericContext_p != PNULL, entity, 0, 0);
  FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
#endif
  FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

  KiCreateZeroSignal (SIG_APEX_ABPD_PSD_MODIFY_REQ,
                      sizeof (ApexAbpdPsdModifyReq),
                      &sigBuff);

  cmd_p = gprsGenericContext_p->cidUserData [gprsContext_p->vgModifyCid];
  cmd_p->pendingContextModification = TRUE;
  psdBearerInfo_p = &cmd_p->psdBearerInfo;

  request_p = (ApexAbpdPsdModifyReq *)sigBuff.sig;

  request_p->connId = psdBearerInfo_p->connId;

  if(TRUE == psdBearerInfo_p->reqQosPresent)
  {
    request_p->qosPresent           = TRUE;
    request_p->qos                  = psdBearerInfo_p->requiredQos;
  }
  if(TRUE == psdBearerInfo_p->reqTftPresent)
  {
    request_p->tftPresent           = TRUE;
    request_p->tft                  = psdBearerInfo_p->requiredTft;
    request_p->tft.tftOpCode        = vgCiGetTftOpCode(&psdBearerInfo_p->requiredTft, &psdBearerInfo_p->negotiatedTft);
  }

  KiSendSignal (TASK_BL_ID, &sigBuff);

}
#endif /* FEA_QOS_TFT */

#if defined (FEA_MT_PDN_ACT)
/*************************************************************************
*
* Function:     vgApexAbpdSetupRsp
*
* Parameters:   entity - mux channel number
*
* Returns:      nothing
*
* Description:  This routine sends an APEX_ABPD_SETUP_RSP signal
*               to the ABPD module part of the AB task.
*
*************************************************************************/
void vgApexAbpdSetupRsp         (const VgmuxChannelNumber entity)
{
  SignalBuffer          sigBuff                 = kiNullBuffer;
  ApexAbpdSetupRsp      *request_p              = PNULL;
  GprsContext_t         *gprsContext_p          = ptrToGprsContext (entity);
  GprsGenericContext_t  *gprsGenericContext_p   = ptrToGprsGenericContext ();
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(gprsContext_p != PNULL, entity, 0, 0);
#endif
  if (gprsGenericContext_p->vgAbpdSetupRsp.connectionAccepted)
  {
    FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity));
    gprsContext_p->activePsdBearerContextWithDataConn->pendingContextActivation = TRUE;
  }
  else
  {
    /* rejecting, so the setup rsp is the last thing we do */
  }

  KiCreateZeroSignal (SIG_APEX_ABPD_SETUP_RSP,
                      sizeof (ApexAbpdSetupRsp),
                      &sigBuff );
  request_p = (ApexAbpdSetupRsp *) sigBuff.sig;
  /* we have already prepared the signal contents */
  *request_p = gprsGenericContext_p->vgAbpdSetupRsp;
  KiSendSignal (TASK_BL_ID, &sigBuff);
}
#endif /* FEA_MT_PDN_ACT */

/*************************************************************************
*
* Function:     vgApexAbpdApnReadReq
*
* Parameters:   entity - mux channel number
*
* Returns:      nothing
*
* Description:  This routine sends an APEX_ABPD_APN_READ_REQ signal
*               to the ABPD module part of the AB task.
*
*************************************************************************/
void vgApexAbpdApnReadReq       (const VgmuxChannelNumber entity)
{
  SignalBuffer            sigBuff = kiNullBuffer;
  ApexAbpdApnReadReq      *request_p;

  FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

  sendSsRegistrationSignal (PACKET_DATA,
                             entity,
                              SIG_APEX_ABPD_APN_READ_CNF);

  KiCreateZeroSignal (SIG_APEX_ABPD_APN_READ_REQ,
                      sizeof (ApexAbpdApnReadReq),
                      &sigBuff);

  request_p                 = (ApexAbpdApnReadReq *)sigBuff.sig;
  request_p->taskId         = VG_CI_TASK_ID;
  request_p->commandRef     = (Int16)entity;

  KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*************************************************************************
*
* Function:     vgApexAbpdApnWriteReq
*
* Parameters:   entity - mux channel number
*
* Returns:      nothing
*
* Description:  This routine sends an APEX_ABPD_APN_WRITE_REQ signal
*               to the ABPD module part of the AB task.
*
*************************************************************************/
void vgApexAbpdApnWriteReq      (const VgmuxChannelNumber entity)
{
  SignalBuffer            sigBuff               = kiNullBuffer;
  ApexAbpdApnWriteReq     *request_p;
  GprsGenericContext_t    *gprsGenericContext_p = ptrToGprsGenericContext ();

  FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

  sendSsRegistrationSignal (PACKET_DATA,
                             entity,
                              SIG_APEX_ABPD_APN_WRITE_CNF);

  KiCreateZeroSignal (SIG_APEX_ABPD_APN_WRITE_REQ,
                      sizeof (ApexAbpdApnWriteReq),
                      &sigBuff);

  request_p                         = (ApexAbpdApnWriteReq *)sigBuff.sig;
  request_p->taskId                 = VG_CI_TASK_ID;
  request_p->commandRef             = (Int16)entity;
  request_p->apnDesc                = gprsGenericContext_p->definedDefaultAPN;

  KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*************************************************************************
 *
 * Function:     vgApexAbpdReportCounterReq
 *
 * Parameters:   entity - mux channel number
 *
 * Returns:      nothing
 *
 * Description:  This function is called in order to modify the action of
 *               the DBM data counting function, either when an
 *               AT*MGCOUNT command is received or on context activation or
 *               deactivation if the active context has count reporting
 *               enabled.
 *
 *************************************************************************/
void vgApexAbpdReportCounterReq (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t     *gprsGenericContext_p = ptrToGprsGenericContext ();
  SignalBuffer             sigBuff = kiNullBuffer;
  ApexAbpdReportCounterReq *request_p;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsGenericContext_p != PNULL, entity, 0, 0);
  FatalCheck(gprsContext_p != PNULL,         entity, 0, 0);
#endif
  FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

  sendSsRegistrationSignal (PACKET_DATA,
                            entity,
                            SIG_APEX_ABPD_REPORT_COUNTER_CNF);
  
  KiCreateZeroSignal (SIG_APEX_ABPD_REPORT_COUNTER_REQ,
                       sizeof (ApexAbpdReportCounterReq),
                        &sigBuff);

  request_p = (ApexAbpdReportCounterReq *)sigBuff.sig;

  if(gprsGenericContext_p->vgMGCOUNTData.vgCounterCid == CID_NUMBER_UNKNOWN)
  {
    request_p->connId = DEFAULT_CONN_ID;
  }
  else
  {
    FatalCheck (gprsGenericContext_p->cidUserData[gprsGenericContext_p->vgMGCOUNTData.vgCounterCid] != PNULL,
                entity,
                gprsGenericContext_p->vgMGCOUNTData.vgCounterCid,
                0);
    request_p->connId = gprsGenericContext_p->cidUserData[gprsGenericContext_p->vgMGCOUNTData.vgCounterCid]->psdBearerInfo.connId;
  }

  KiSendSignal (TASK_BL_ID, &sigBuff);
}


/*************************************************************************
 *
 * Function:     vgApexAbpdResetCounterReq
 *
 * Parameters:   entity - mux channel number
 *
 * Returns:      nothing
 *
 * Description:  This function is called in order to reset the DBM data
 *               counters for the given PSD Bearer ID. Although the signal can
 *               support multiple PSD Bearer IDs, only one is used by the CI task.
 *
 *************************************************************************/
void vgApexAbpdResetCounterReq (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t     *gprsGenericContext_p = ptrToGprsGenericContext ();
  SignalBuffer            sigBuf = kiNullBuffer;
  ApexAbpdResetCounterReq *request_p;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsGenericContext_p != PNULL, entity, 0, 0);
#endif
  FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

  /* no confirmation signal, so no register required */

  KiCreateZeroSignal (SIG_APEX_ABPD_RESET_COUNTER_REQ,
                       sizeof (ApexAbpdResetCounterReq),
                        &sigBuf);

  request_p = (ApexAbpdResetCounterReq *)sigBuf.sig;
  
  if(gprsGenericContext_p->vgMGCOUNTData.vgCounterCid == CID_NUMBER_UNKNOWN)
  {
    request_p->connId = DEFAULT_CONN_ID;
  }
  else
  {
    FatalCheck (gprsGenericContext_p->cidUserData[gprsGenericContext_p->vgMGCOUNTData.vgCounterCid] != PNULL,
                entity,
                gprsGenericContext_p->vgMGCOUNTData.vgCounterCid,
                0);
    request_p->connId = gprsGenericContext_p->cidUserData[gprsGenericContext_p->vgMGCOUNTData.vgCounterCid]->psdBearerInfo.connId;
  }

  KiSendSignal (TASK_BL_ID, &sigBuf);
}

#if defined (FEA_UPDIR)
/*************************************************************************
 *
 * Function:     vgApexAbpdSetUpdirInfoReq
 *
 * Parameters:   entity - mux channel number
 *
 * Returns:      nothing
 *
 * Description:  This function is called in order to set UPDIR information to DBM.
 *
 *************************************************************************/
void vgApexAbpdSetUpdirInfoReq (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t     *gprsGenericContext_p = ptrToGprsGenericContext ();
  SignalBuffer             sigBuff = kiNullBuffer;
  ApexAbpdSetUpdirInfoReq  *request_p;

  FatalCheck (gprsGenericContext_p != PNULL, entity, 0, 0);

  FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

  sendSsRegistrationSignal (PACKET_DATA,
                            entity,
                            SIG_APEX_ABPD_SET_UPDIR_INFO_CNF);
  
  KiCreateZeroSignal (SIG_APEX_ABPD_SET_UPDIR_INFO_REQ,
                       sizeof (ApexAbpdSetUpdirInfoReq),
                        &sigBuff);

  request_p = (ApexAbpdSetUpdirInfoReq *)sigBuff.sig;

  request_p->srcIpAddress = gprsGenericContext_p->vgMUPDIRData.srcIpAddress;
  request_p->dstIpAddress = gprsGenericContext_p->vgMUPDIRData.dstIpAddress;
  request_p->srcPortPresent = gprsGenericContext_p->vgMUPDIRData.srcPortPresent;
  request_p->dstPortPresent = gprsGenericContext_p->vgMUPDIRData.dstPortPresent;
  request_p->srcPort = gprsGenericContext_p->vgMUPDIRData.srcPort;  
  request_p->dstPort = gprsGenericContext_p->vgMUPDIRData.dstPort;
  request_p->msgIdOffset = gprsGenericContext_p->vgMUPDIRData.msgIdOffset;
  request_p->msgIdSize = gprsGenericContext_p->vgMUPDIRData.msgIdSize;  

  KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*************************************************************************
 *
 * Function:     vgApexAbpdResetUpdirInfoReq
 *
 * Parameters:   entity - mux channel number
 *
 * Returns:      nothing
 *
 * Description:  This function is called in order to reset the UPDIR information to DBM.
 *
 *************************************************************************/
void vgApexAbpdResetUpdirInfoReq (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t      *gprsGenericContext_p = ptrToGprsGenericContext ();
  SignalBuffer              sigBuf = kiNullBuffer;
  ApexAbpdResetUpdirInfoReq *request_p;

  FatalCheck (gprsGenericContext_p != PNULL, entity, 0, 0);

  FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

  /* no confirmation signal, so no register required */
  KiCreateZeroSignal (SIG_APEX_ABPD_RESET_UPDIR_INFO_REQ,
                       sizeof (ApexAbpdResetUpdirInfoReq),
                        &sigBuf);

  request_p = (ApexAbpdResetUpdirInfoReq *)sigBuf.sig;
  
  request_p->patternId = gprsGenericContext_p->vgMUPDIRData.patternId;

  KiSendSignal (TASK_BL_ID, &sigBuf);
}

#endif

#if defined (FEA_PPP)
/*--------------------------------------------------------------------------
 * Function:    vgApexAbpdPppConfigReq
 *
 * Parameters:  None.
 *
 * Returns:     Nothing
 *
 * Description: Create, initialise and send SIG_APEX_ABPD_PPP_CONFIG_REQ
 *              signal.
 *-------------------------------------------------------------------------*/
void vgApexAbpdPppConfigReq(const VgmuxChannelNumber entity,
                             const Boolean updateMode,
                              const Int32 index,
                               const Int32 finalValue,
                                const Int32 initValue,
                                 const Int8 tries)
{
  SignalBuffer          sigBuff = kiNullBuffer;
  ApexAbpdPppConfigReq  *request_p;

  sendSsRegistrationSignal (PACKET_DATA,
                             entity,
                              SIG_APEX_ABPD_PPP_CONFIG_CNF);

  /* send configure req to PPP task */
  KiCreateZeroSignal (SIG_APEX_ABPD_PPP_CONFIG_REQ, sizeof (ApexAbpdPppConfigReq), &sigBuff);

  request_p                             = (ApexAbpdPppConfigReq *)sigBuff.sig;
  request_p->updateMode                 = updateMode;
  request_p->pppTimerConfig.index       = (PppConfigIndex) index;
  request_p->pppTimerConfig.finalValue  = finalValue;
  request_p->pppTimerConfig.initValue   = initValue;
  request_p->pppTimerConfig.tries       = tries;

  KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*--------------------------------------------------------------------------
 * Function:    vgApexAbpdPppConfigAuthReq
 *
 * Parameters:  None.
 *
 * Returns:     Nothing
 *
 * Description: Create, initialise and send SIG_APEX_ABPD_PPP_CONFIG_AUTH_REQ
 *              signal.
 *-------------------------------------------------------------------------*/
void vgApexAbpdPppConfigAuthReq(const VgmuxChannelNumber entity,
                                const Boolean updateMode,
                                 const Int32 value)
{
  SignalBuffer                    sigBuff = kiNullBuffer;
  ApexAbpdPppConfigAuthReq        *request_p;

  sendSsRegistrationSignal (PACKET_DATA,
                            entity,
                            SIG_APEX_ABPD_PPP_CONFIG_AUTH_CNF);

  /* send configure req to PPP task */
  KiCreateZeroSignal (SIG_APEX_ABPD_PPP_CONFIG_AUTH_REQ, sizeof (ApexAbpdPppConfigAuthReq), &sigBuff);

  request_p = (ApexAbpdPppConfigAuthReq *) sigBuff.sig;
  request_p->updateMode = updateMode;
  request_p->pppAuthType = (PppAuthType) value;
  KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*--------------------------------------------------------------------------
 * Function:    vgApexAbpdPppLoopbackReq
 *
 * Parameters:  None.
 *
 * Returns:     Nothing
 *
 * Description: Create, initialise and send SIG_APEX_ABPD_PPP_LOOPBACK_REQ
 *              signal.
 *-------------------------------------------------------------------------*/
void vgApexAbpdPppLoopbackReq(const VgmuxChannelNumber entity,
                               const Boolean enable)
{
  SignalBuffer                sigBuff = kiNullBuffer;
  ApexAbpdPppLoopbackReq      *request_p;
  GprsContext_t                *gprsContext_p = ptrToGprsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
#endif
  sendSsRegistrationSignal (PACKET_DATA,
                             entity,
                              SIG_APEX_ABPD_PPP_LOOPBACK_CNF);

  /* send loopback req to PPP task */
  KiCreateZeroSignal (SIG_APEX_ABPD_PPP_LOOPBACK_REQ, sizeof (ApexAbpdPppLoopbackReq), &sigBuff);

  request_p                 = (ApexAbpdPppLoopbackReq *)sigBuff.sig;
  request_p->enableLoopback = enable;
  request_p->numDlXmit      = gprsContext_p->vgPppLoopbackNumDlXmit;
  request_p->loopbackMode   = gprsContext_p->vgPppLoopbackMode;
  request_p->dlTimeout      = gprsContext_p->vgPppLoopbackDlTimeout;
  request_p->packetLength   = gprsContext_p->vgPppLoopbackPacketSize;
  request_p->totalNumDlXmit = gprsContext_p->vgPppTotalNumDlXmit;
  request_p->loopbackUlChecking = gprsContext_p->vgPppLoopbackUlCheckingEnabled;
  request_p->byteStuffingEnabled = gprsContext_p->vgPppLoopbackByteStuffingEnabled;

  request_p->channelNumber = (Int8)entity;

  KiSendSignal (TASK_BL_ID, &sigBuff);
}
#endif /* FEA_PPP */
#if defined (FEA_ACL)

/*--------------------------------------------------------------------------
 * Function:    vgApexAbpdSetAclReq
 *
 * Parameters:  entity - entity requesting signal transmission.
 *
 * Returns:     Nothing
 *
 * Description: Create, initialise and send SIG_APEX_ABPD_SET_ACL_REQ
 *-------------------------------------------------------------------------*/
void vgApexAbpdSetAclReq (const VgmuxChannelNumber entity)
{
  SignalBuffer          sigBuff  = kiNullBuffer;
  ApexAbpdSetAclReq     *request_p;
  VgCGACLData           *aclData = &(ptrToGprsGenericContext ()->vgCGACLData);

  FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

  /* Create and initialise signal */
  sendSsRegistrationSignal (PACKET_DATA,
                            entity,
                            SIG_APEX_ABPD_SET_ACL_CNF);

  KiCreateZeroSignal (SIG_APEX_ABPD_SET_ACL_REQ,
                      sizeof (ApexAbpdSetAclReq),
                      &sigBuff);

  request_p = (ApexAbpdSetAclReq *)sigBuff.sig;
  request_p->taskId     = VG_CI_TASK_ID;
  request_p->commandRef = (Int16)entity;
  request_p->enable     = aclData->setAcl;

  /* Send signal to BL for processing */
  KiSendSignal (TASK_BL_ID, &sigBuff);
}
/*--------------------------------------------------------------------------
 * Function:    vgApexAbpdListAclReq
 *
 * Parameters:  entity - entity requesting signal transmission.
 *
 * Returns:     Nothing
 *
 * Description: Create, initialise and send SIG_APEX_ABPD_LIST_ACL_REQ
 *-------------------------------------------------------------------------*/
void vgApexAbpdListAclReq (const VgmuxChannelNumber entity)
{
  SignalBuffer          sigBuff  = kiNullBuffer;
  ApexAbpdListAclReq    *request_p;
  VgCGACLData           *aclData = &(ptrToGprsGenericContext ()->vgCGACLData);

  FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

  /* Create and initialise signal */
  sendSsRegistrationSignal (PACKET_DATA,
                            entity,
                            SIG_APEX_ABPD_LIST_ACL_CNF);

  KiCreateZeroSignal (SIG_APEX_ABPD_LIST_ACL_REQ,
                      sizeof (ApexAbpdListAclReq),
                      &sigBuff);

  request_p = (ApexAbpdListAclReq *)sigBuff.sig;
  request_p->taskId     = VG_CI_TASK_ID;
  request_p->commandRef = (Int16)entity;
  request_p->startField = aclData->startField;

  /* Send signal to BL for processing */
  KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*--------------------------------------------------------------------------
 * Function:    vgApexAbpdWriteAclReq
 *
 * Parameters:  entity - entity requesting signal transmission.
 *
 * Returns:     Nothing
 *
 * Description: Create, initialise and send SIG_APEX_ABPD_WRITE_ACL_REQ
 *-------------------------------------------------------------------------*/
void vgApexAbpdWriteAclReq (const VgmuxChannelNumber entity)
{
  SignalBuffer          sigBuff = kiNullBuffer;
  ApexAbpdWriteAclReq   *request_p;
  VgCGACLData           *aclData = &(ptrToGprsGenericContext ()->vgCGACLData);

  FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

  /* Create and initialise signal */
  sendSsRegistrationSignal (PACKET_DATA,
                            entity,
                            SIG_APEX_ABPD_WRITE_ACL_CNF);

  KiCreateZeroSignal (SIG_APEX_ABPD_WRITE_ACL_REQ,
                      sizeof (ApexAbpdWriteAclReq),
                      &sigBuff);

  request_p = (ApexAbpdWriteAclReq *)sigBuff.sig;
  request_p->taskId     = VG_CI_TASK_ID;
  request_p->commandRef = (Int16)entity;
  request_p->position   = aclData->startField;
  request_p->mode       = ABPD_ACL_WRITE_ABSOLUTE;
  memcpy(&(request_p->apn), &(aclData->apn), sizeof (TextualAccessPointName));

  /* Send signal to BL for processing */
  KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*--------------------------------------------------------------------------
 * Function:    vgApexAbpdDeleteAclReq
 *
 * Parameters:  entity - entity requesting signal transmission.
 *
 * Returns:     Nothing
 *
 * Description: Create, initialise and send SIG_APEX_ABPD_DELETE_ACL_REQ
 *-------------------------------------------------------------------------*/
void vgApexAbpdDeleteAclReq (const VgmuxChannelNumber entity)
{
  SignalBuffer            sigBuff  = kiNullBuffer;
  ApexAbpdDeleteAclReq    *request_p;
  VgCGACLData             *aclData = &(ptrToGprsGenericContext ()->vgCGACLData);

  FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

  /* Create and initialise signal */
  sendSsRegistrationSignal (PACKET_DATA,
                            entity,
                            SIG_APEX_ABPD_DELETE_ACL_CNF);

  KiCreateZeroSignal (SIG_APEX_ABPD_DELETE_ACL_REQ,
                      sizeof (ApexAbpdDeleteAclReq),
                      &sigBuff);

  request_p = (ApexAbpdDeleteAclReq *)sigBuff.sig;
  request_p->taskId     = VG_CI_TASK_ID;
  request_p->commandRef = (Int16)entity;
  request_p->mode       = ABPD_ACL_DELETE_ABSOLUTE;
  request_p->position   = aclData->startField;

  /* Send signal to BL for processing */
  KiSendSignal (TASK_BL_ID, &sigBuff);
}

/*--------------------------------------------------------------------------
 * Function:    vgApexAbpdAclStatusReq
 *
 * Parameters:  entity - entity requesting signal transmission.
 *
 * Returns:     Nothing
 *
 * Description: Create, initialise and send SIG_APEX_ABPD_ACL_STATUS_REQ
 *-------------------------------------------------------------------------*/
void vgApexAbpdAclStatusReq (const VgmuxChannelNumber entity)
{
  SignalBuffer            sigBuff  = kiNullBuffer;
  ApexAbpdAclStatusReq    *request_p;

  FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

  /* Create and initialise signal */
  sendSsRegistrationSignal (PACKET_DATA,
                            entity,
                            SIG_APEX_ABPD_ACL_STATUS_CNF);

  KiCreateZeroSignal (SIG_APEX_ABPD_ACL_STATUS_REQ,
                      sizeof (ApexAbpdAclStatusReq),
                      &sigBuff);

  request_p = (ApexAbpdAclStatusReq *)sigBuff.sig;
  request_p->taskId     = VG_CI_TASK_ID;
  request_p->commandRef = (Int16)entity;

  /* Send signal to BL for processing */
  KiSendSignal (TASK_BL_ID, &sigBuff);
}
#endif

/*--------------------------------------------------------------------------
 * Function:    vgApexAbpdWriteRelAssistReq
 *
 * Parameters:  entity - entity requesting signal transmission.
 *
 * Returns:     Nothing
 *
 * Description: Create, initialise and send SIG_APEX_ABPD_WRITE_REL_ASSIST_REQ
 *-------------------------------------------------------------------------*/
void vgApexAbpdWriteRelAssistReq (const VgmuxChannelNumber entity)
{
  SignalBuffer            sigBuff  = kiNullBuffer;
  ApexAbpdWriteRelAssistReq    *request_p;
 

  FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

  /* Create and initialise signal */
  sendSsRegistrationSignal (PACKET_DATA,
                            entity,
                            SIG_APEX_ABPD_WRITE_REL_ASSIST_CNF);

  KiCreateZeroSignal (SIG_APEX_ABPD_WRITE_REL_ASSIST_REQ,
                      sizeof (ApexAbpdWriteRelAssistReq),
                      &sigBuff);

  request_p = (ApexAbpdWriteRelAssistReq *)sigBuff.sig;
  request_p->taskId     = VG_CI_TASK_ID;
  request_p->relAssistInformation = (ptrToGprsGenericContext ()->vgReqNbiotRelAssistData);
  

  /* Send signal to BL for processing */
  KiSendSignal (TASK_BL_ID, &sigBuff);

}

/*--------------------------------------------------------------------------
 * Function:    vgApexAbpdReadRelAssistReq
 *
 * Parameters:  entity - entity requesting signal transmission.
 *
 * Returns:     Nothing
 *
 * Description: Create, initialise and send SIG_APEX_ABPD_READ_REL_ASSIST_REQ
 *-------------------------------------------------------------------------*/
void vgApexAbpdReadRelAssistReq (const VgmuxChannelNumber entity)
{
    SignalBuffer                   sigBuff = kiNullBuffer;
    ApexAbpdReadRelAssistReq      *request_p;
  
    FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);
  
    sendSsRegistrationSignal (PACKET_DATA,
                               entity,
                                SIG_APEX_ABPD_READ_REL_ASSIST_CNF);
  
    KiCreateZeroSignal (SIG_APEX_ABPD_READ_REL_ASSIST_REQ,
                        sizeof (ApexAbpdReadRelAssistReq),
                        &sigBuff);
  
    request_p                 = (ApexAbpdReadRelAssistReq *)sigBuff.sig;
    request_p->taskId         = VG_CI_TASK_ID;
    KiSendSignal (TASK_BL_ID, &sigBuff);
}

 /*--------------------------------------------------------------------------
 *
 * Function:        vgApexAbpdReadApnDataTypeReq
 *
 * Parameters:      entity
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_ABPD_READ_DATA_TYPE_REQ to the
 *                  background layer 
 *-------------------------------------------------------------------------*/

void vgApexAbpdReadApnDataTypeReq (const VgmuxChannelNumber entity)
{
   SignalBuffer                   sigBuff = kiNullBuffer;
   ApexAbpdReadApnDataTypeReq     *request_p;
     
   FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);
     
   sendSsRegistrationSignal (PACKET_DATA,
                              entity,
                              SIG_APEX_ABPD_READ_APN_DATA_TYPE_CNF);
     
   KiCreateZeroSignal (SIG_APEX_ABPD_READ_APN_DATA_TYPE_REQ,
                       sizeof (ApexAbpdReadApnDataTypeReq),
                       &sigBuff);
     
   request_p                 = (ApexAbpdReadApnDataTypeReq *)sigBuff.sig;
   request_p->taskId         = VG_CI_TASK_ID;
   KiSendSignal (TASK_BL_ID, &sigBuff);
   
}
 /*--------------------------------------------------------------------------
 *
 * Function:        vgApexAbpdWriteApnDataTypeReq
 *
 * Parameters:      entity
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_ABPD_WRITE_DATA_TYPE_REQ to the
 *                  background layer
 *   
 *-------------------------------------------------------------------------*/

void vgApexAbpdWriteApnDataTypeReq (const VgmuxChannelNumber entity)
{
   SignalBuffer                   sigBuff = kiNullBuffer;
   ApexAbpdWriteApnDataTypeReq    *request_p;
   GprsGenericContext_t           *gprsGenericContext_p = ptrToGprsGenericContext ();  
   Int8                           x;
   
   
   FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);
     
   sendSsRegistrationSignal (PACKET_DATA,
                              entity,
                              SIG_APEX_ABPD_WRITE_APN_DATA_TYPE_CNF);
     
   KiCreateZeroSignal (SIG_APEX_ABPD_WRITE_APN_DATA_TYPE_REQ,
                       sizeof (ApexAbpdWriteApnDataTypeReq),
                       &sigBuff);
     
   request_p                 = (ApexAbpdWriteApnDataTypeReq *)sigBuff.sig;
   request_p->taskId         = VG_CI_TASK_ID;
   for (x = 0; x< MAX_NUM_CONN_IDS; x++)
   {
      request_p->apnDataTypeEntity[x] = gprsGenericContext_p->vgReqNbiotDtData.apnDataTypeEntity[x];
   }
   KiSendSignal (TASK_BL_ID, &sigBuff);

}

/*--------------------------------------------------------------------------
 *
 * Function:        vgApexAbpdTransmitDataInd
 *
 * Parameters:      entity
 *
 * Returns:         Nothing
 *
 * Description:     Sends a SIG_APEX_ABPD_TRANSMIT_DATA_IND to the
 *                  background layer
 *
 *-------------------------------------------------------------------------*/

void vgApexAbpdTransmitDataInd(const VgmuxChannelNumber entity)
{
   SignalBuffer                   sigBuff = kiNullBuffer;
   ApexAbpdTransmitDataInd        *request_p;
   GprsGenericContext_t           *gprsGenericContext_p = ptrToGprsGenericContext ();

   FatalAssert (vgChManCheckHaveControl (CC_GENERAL_PACKET_RADIO_SYSTEM, entity) == TRUE);

   KiCreateZeroSignal (SIG_APEX_ABPD_TRANSMIT_DATA_IND,
                       sizeof (ApexAbpdTransmitDataInd),
                       &sigBuff);

   request_p                 = (ApexAbpdTransmitDataInd *)sigBuff.sig;

   request_p->connId = gprsGenericContext_p->vgCSODCPData.connId;
   request_p->userDataLength = gprsGenericContext_p->vgCSODCPData.userDataLength;
   request_p->userData = gprsGenericContext_p->vgCSODCPData.userData;   
   request_p->relAssistInformation = gprsGenericContext_p->vgReqNbiotRelAssistData;
   request_p->ApnDataTypeEntity = gprsGenericContext_p->vgReqNbiotDtData.apnDataTypeEntity[gprsGenericContext_p->vgCSODCPData.connId];

   KiSendSignal (TASK_BL_ID, &sigBuff);
}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

