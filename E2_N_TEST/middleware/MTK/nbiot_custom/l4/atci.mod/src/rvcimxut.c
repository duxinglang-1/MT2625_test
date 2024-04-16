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
 * RVCIMXUT.C
 * Utilities for the CI MUX sub-system
 **************************************************************************/

#define MODULE_NAME "RVCIMXUT"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <vgmx_sig.h>
#include <rvsystem.h>
#include <rvutil.h>
#include <rvtssigo.h>
#include <rvcimxut.h>
#include <rvccut.h>
#if defined(DEVELOPMENT_VERSION)
#include <rvcrhand.h>
#endif
#include <rvpfsigo.h>
#include <vgmxdte.h>
#include <ciapex_sig.h>
#include <rvmmsigo.h>
#include <rvoman.h>
#include <rvchman.h>
#include <rvcmux.h>
#include <rvpdsigo.h>
#include <rvgput.h>
#include <rvstkrnat.h>
#include <gkisig.h>
#include <gkimem.h>

#if defined (MTK_NBIOT_TARGET_BUILD)
#include <serial_port.h>
#endif

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* mask which filters invalid characters */

static const Boolean mxAtDataMask[] =
{
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  TRUE,  FALSE, FALSE, FALSE, FALSE, TRUE,  FALSE, FALSE,  /* BS CR       */
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, TRUE,  TRUE,  FALSE, FALSE, FALSE, FALSE,  /* SUB ESC     */
  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,   /* SPC !"#$%&' */
  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,   /* ()*+,-./    */
  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,   /* 01235678    */
  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,   /* 9:;<=>?"    */
  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,   /* @ABCDEFG    */
  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,   /* HIJKLMNO    */
  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,   /* PQRSTUVW    */
  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,   /* XYZ[\]^_    */
  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,   /* 'abcdefg    */
  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,   /* hijklmno    */
  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,   /* pqrstuvw    */
  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  FALSE   /* xyz{|}~     */
};

#define MAX_AT_DATA_CHAR_VALUE (sizeof(mxAtDataMask)/sizeof(Boolean))

#define MAX_GSM_CHAR (0x80)

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  CiMuxAtDataReq            ciMuxAtDataReq;
  CiMuxAtoCmdReq            ciMuxAtoCmdReq;
  CiMuxChannelEnableRsp     ciMuxChannelEnableRsp;
  CiMuxCloseDataConnReq     ciMuxCloseDataConnReq;
  CiMuxConfigureChannelReq  ciMuxConfigureChannelReq;
  CiMuxOpenDataConnReq      ciMuxOpenDataConnReq;
  CiMuxRingIndicatorOffReq  ciMuxRingIndicatorOffReq;
  CiMuxRingIndicatorOnReq   ciMuxRingIndicatorOnReq;
  CiMuxChannelDisabledRsp   ciMuxChannelDisabledRsp;
};

/***************************************************************************
 * Types
 ***************************************************************************/


/***************************************************************************
 * Variables
 ***************************************************************************/


/***************************************************************************
 * Macros
 ***************************************************************************/

#define APPEND_NODE(nODEtYPE, fIRSTnODE_P, nEWnODE_P) \
{                                 \
  nODEtYPE node = fIRSTnODE_P;    \
  nODEtYPE tnode = fIRSTnODE_P;   \
                                  \
  /* find end of node */          \
  while (node)  { tnode=node; node=node->next_p; } \
  /* add node to end of list */   \
  if (tnode)                      \
    tnode->next_p = nEWnODE_P;    \
  else                            \
    fIRSTnODE_P = nEWnODE_P;      \
}

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/***************************************************************************
 * Global Functions
 ***************************************************************************/
#if defined (MTK_NBIOT_TARGET_BUILD)
extern PortSpeed connLayerGetUartPortBaudRate(Int32   channelNumber);
extern  hal_uart_flowcontrol_t connLayerGetUartPortFlowCtrl(Int32   channelNumber);
#endif

/*--------------------------------------------------------------------------
*
* Function:    applyDataMask
*
* Parameters:  dataChar - character to check against data mask
*              entity   - mux channel number
*
* Returns:     Boolean - indicating whether character is allowed
*
* Description: This function checks to see if a character is valid in the
*              current character set.  If the character is a valid printable
*              character then TRUE is returned, otherwise FALSE.
*
*-------------------------------------------------------------------------*/

Boolean applyDataMask (const Char dataChar, const VgmuxChannelNumber entity)
{
  Boolean result = FALSE;

  switch (getProfileValue (entity, PROF_CSCS))
  {
    case VG_AT_CSCS_GSM:
    {
      if (dataChar < MAX_GSM_CHAR)
      {
        result = TRUE;
      }
      break;
    }

    case VG_AT_CSCS_HEX:
    case VG_AT_CSCS_IRA:
    case VG_AT_CSCS_PCCP:
    case VG_AT_CSCS_PCDN:
    case VG_AT_CSCS_UCS2:
    case VG_AT_CSCS_8859_1:
    default:
    {
      if ((dataChar < MAX_AT_DATA_CHAR_VALUE) && (mxAtDataMask[dataChar] == TRUE))
      {
        result = TRUE;
      }
      break;
    }
  }


  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    crManAddToAtCache
*
* Parameters:  channelNumber     - mux channel number
*              targetQueue       - signal queue
*              targetQueueLength - signals in queue
*              signalBuffer_p    - incoming signal to be processed
*
* Returns:     Boolean - indicating whether character is allowed
*
* Description: Add the signals contents to the cache of data we have
*              received from the multiplexer but not yet processed
*
*-------------------------------------------------------------------------*/

void crManAddToAtCache (const VgmuxChannelNumber  channelNumber,
                            SignalBuffer             targetQueue [],
                            Int8                     *targetQueueLength,
                            SignalBuffer             *signalBuffer_p)
{
  CiMuxAtDataReq            *sig_p, *signal_p;

  Int16                     i;

  PARAMETER_NOT_USED (channelNumber);
  FatalAssert           (signalBuffer_p != PNULL);
  FatalAssert           (signalBuffer_p->type != PNULL);
  FatalAssert           (*(signalBuffer_p->type) == SIG_CIMUX_AT_DATA_REQ);

  signal_p = &signalBuffer_p->sig->ciMuxAtDataReq;



  if (*targetQueueLength == 0)
  {
    /* first queue element so just push the signal */
    targetQueue [*targetQueueLength] = kiNullBuffer;

    KiCreateSignal (SIG_CIMUX_AT_DATA_REQ,
                          sizeof(CiMuxAtDataReq),
                          &targetQueue [*targetQueueLength]);

    sig_p = &targetQueue [*targetQueueLength].sig->ciMuxAtDataReq;
    memcpy (sig_p, signal_p, sizeof (CiMuxAtDataReq));

    *targetQueueLength += 1;
  }
  else
  {
    Int32 len = signal_p->length;

    sig_p = &targetQueue [*targetQueueLength - 1].sig->ciMuxAtDataReq;

    /* enough space in last signal to add the data */

    if (len < (CIMUX_MAX_AT_DATA_LENGTH - sig_p->length))
    {
      memcpy(&sig_p->data[sig_p->length], signal_p->data, signal_p->length);
      sig_p->length += signal_p->length;
    }
    else
    {
      len = CIMUX_MAX_AT_DATA_LENGTH - sig_p->length;
      memcpy(&sig_p->data[sig_p->length], signal_p->data, len);
      sig_p->length += len;

      /*
       * append data to the last signal in the queue and then push the new
       * signal onto the Queue
       */
      if (*targetQueueLength < AT_CACHE_SIZE)
      {
        memcpy(signal_p->data, &signal_p->data[len], signal_p->length - len);
        signal_p->length = signal_p->length - len;
        for(i = signal_p->length; i < CIMUX_MAX_AT_DATA_LENGTH; i++)
        {
            signal_p->data[i] = 0;
        }
        targetQueue [*targetQueueLength] = kiNullBuffer;

        KiCreateSignal (SIG_CIMUX_AT_DATA_REQ,
                               sizeof(CiMuxAtDataReq),
                              &targetQueue [*targetQueueLength]);

        sig_p = &targetQueue [*targetQueueLength].sig->ciMuxAtDataReq;
        memcpy (sig_p, signal_p, sizeof (CiMuxAtDataReq));

        *targetQueueLength += 1;
      }
      else
      {
        /* Queue is full so ignore this signal */
        WarnParam(channelNumber, *targetQueueLength, 0);
      }
    }
  }
}

/*--------------------------------------------------------------------------
*
* Function:    popFromAtCache
*
* Parameters:  targetQueue       - signal queue
*              targetQueueLength - signals in queue
*
* Returns:     Boolean - indicating whether character is allowed
*
* Description: Remove first the signal from the at cache
*
*-------------------------------------------------------------------------*/

void popFromAtCache (SignalBuffer targetQueue [],
                       Int8 *targetQueueLength)
{
  Int8              index;

  if (targetQueue->type != PNULL)
  {
    KiDestroySignal(&targetQueue [0]);
  }

  for (index = 0; index < *targetQueueLength - 1; index++)
  {
    targetQueue[index]     = targetQueue[index + 1];
    targetQueue[index + 1] = kiNullBuffer;
  }
  *targetQueueLength -= 1;
}

/*--------------------------------------------------------------------------
*
* Function:    vgCiAbortAtCommand
*
* Parameters:  channelNumber - mux channel number
*
* Returns:     nothing
*
* Description: Add the signals contents to the cache of data we have
*              received from the multiplexer but not yet processed
*
*-------------------------------------------------------------------------*/

void vgCiAbortAtCommand (const VgmuxChannelNumber channelNumber)
{
  switch (getCommandId (channelNumber))
  {
    case VG_AT_GP_D:
    case VG_AT_CC_D:
    case VG_AT_CC_DL:
#if defined (FEA_MT_PDN_ACT)
    /* job109238: allow ATA to be aborted */
    case VG_AT_CC_A:
#endif /* FEA_MT_PDN_ACT */
    {
      vgCcAbortConnectingCall (channelNumber);
      break;
    }
    
    case VG_AT_GP_CGATT:
      /* We only abort an attach - but function vgGpAbortAttach will sort that
       * out
       */
      vgGpAbortAttach (channelNumber);
      break;

    default:
    {
      /* not a command that is abortable */
      break;
    }
  }
}


/*--------------------------------------------------------------------------
*
* Function:    setEntityState
*
* Parameters:  entity - mux channel number
*              state  - new entity state
*
* Returns:     nothing
*
* Description: Sets the specified entities state
*
*-------------------------------------------------------------------------*/

void setEntityState (const VgmuxChannelNumber entity,
                      const EntityState_t state)
{
  MuxContext_t *muxContext_p = ptrToMuxContext ();

#if defined (ENABLE_RAVEN_ENTITY_STATE_DEBUG)
  printf ("entity %d, state %d->%d", entity,
          muxContext_p->entityState[entity], state);
#endif

  muxContext_p->entityState[entity] = state;
}

/*--------------------------------------------------------------------------
*
* Function:    setEntityState
*
* Parameters:  entity - mux channel number
*
* Returns:     EntityState_t - current entity state
*
* Description: Returns the specified entities state
*
*-------------------------------------------------------------------------*/

EntityState_t getEntityState (const VgmuxChannelNumber entity)
{
  MuxContext_t  *muxContext_p = ptrToMuxContext ();
  EntityState_t entityState;

  if (entity < CI_MAX_ENTITIES)
  {
    entityState = muxContext_p->entityState[entity];
  }
  else
  {
    WarnParam (entity, 0, 0);
    entityState = ENTITY_NOT_ENABLED;
  }

  return (entityState);
}

/*--------------------------------------------------------------------------
*
* Function:    isEntityActive
*
* Parameters:  entity  - mux channel number
*
* Returns:     Boolean - channel enabled
*
* Description: Returns whether the entity is active or not
*
*-------------------------------------------------------------------------*/

Boolean isEntityActive (const VgmuxChannelNumber entity)
{
  Boolean enabled = FALSE;

  FatalCheck (entity < CI_MAX_ENTITIES, entity, 0, 0);

  /* if the entity has not been enabled then there may not be any memory */
  if (entity < CI_MAX_ENTITIES)
  {
    if (ptrToEntityContextData (entity) != PNULL)
    {
      if (getEntityState (entity) != ENTITY_NOT_ENABLED)
      {
        enabled = TRUE;
      }
    }
  }

  return (enabled);
}

/*--------------------------------------------------------------------------
*
* Function:    isEntityMmiNotUnsolicited
*
* Parameters:  entity  - mux channel number
*
* Returns:     Boolean - channel enabled
*
* Description: Returns whether the entity is MMI and another entity is
*              designated as MMI unsolicited one.
*
*-------------------------------------------------------------------------*/
Boolean isEntityMmiNotUnsolicited (const VgmuxChannelNumber entity)
{
  Boolean mmiNotUnsolicited = FALSE;

  FatalCheck (entity < CI_MAX_ENTITIES, entity, 0, 0);

  /* if the entity has not been enabled then there may not be any memory */
  if (isEntityActive (entity))
  {
    if (entity < CI_MAX_ENTITIES)
    {
      if (getProfileValueBit( entity, PROF_MROUTEMMI, PROF_BIT_MROUTEMMI_HOOK_TO_MMI)&&
          (!getProfileValueBit( entity, PROF_MROUTEMMI, PROF_BIT_MROUTEMMI_MMI_UNSOL))&&
          (vgGetMmiUnsolicitedChannel() != VGMUX_CHANNEL_INVALID ))
      {
        mmiNotUnsolicited = TRUE;
      }
    }
  }

  return (mmiNotUnsolicited);
}


/*--------------------------------------------------------------------------
*
* Function:    vgSendCiMuxChannelEnableRsp
*
* Parameters:  entity  - mux channel number
*              success - if channel was successfully enabled
*
* Returns:     nothing
*
* Description: sends a mux channel enable response
*
*-------------------------------------------------------------------------*/
void vgSendCiMuxChannelEnableRsp (const VgmuxChannelNumber entity, Boolean success)
{
  SignalBuffer signal = kiNullBuffer;
  StkEntityGenericData_t *stkGenericContext_p = ptrToStkGenericContext ();
  MuxContext_t *muxContext_p = ptrToMuxContext ();
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(stkGenericContext_p != PNULL, entity, 0, 0);
#endif
  /*
   * Only send out for real channel - not one started for AT*MCGACT command
   * or STK internal channel.
   */
  if (!((stkGenericContext_p->runAtCmdState == STATE_ENABLING_CHANNEL) && 
           (entity == stkGenericContext_p->atCommandData.cmdEntity )))
  {

    KiCreateZeroSignal (SIG_CIMUX_CHANNEL_ENABLE_RSP,
                        sizeof(CiMuxChannelEnableRsp),
                        &signal);

    signal.sig->ciMuxChannelEnableRsp.channelNumber = entity;

    if (success == TRUE)
    {
      if(TRUE == muxContext_p->isExternalChannel[entity])
      {
        vgGetUartPortBaudRate(entity);
        vgGetUartFlowControlMode(entity);
        getComPortSettings    (entity, &(signal.sig->ciMuxChannelEnableRsp.comPortSettings));
        getChannelConfOptions (entity, &(signal.sig->ciMuxChannelEnableRsp.channelConfOptions));
      }
      else
      {
        /* Set to default value when it is not external connection */
        signal.sig->ciMuxChannelEnableRsp.comPortSettings.portSpeed = PORTSPEED_115200;
        signal.sig->ciMuxChannelEnableRsp.comPortSettings.flowCtrl.downlink = FC_RTS_CTS;
        signal.sig->ciMuxChannelEnableRsp.comPortSettings.flowCtrl.uplink = FC_RTS_CTS;        
        getChannelConfOptions (entity, &(signal.sig->ciMuxChannelEnableRsp.channelConfOptions));        
      }
    }
    signal.sig->ciMuxChannelEnableRsp.channelAccept = success;

#ifdef UE_SIMULATOR
    KiSendSignal(VMMI_TASK_ID, &signal);
#else
    KiSendSignal (MUX_TASK_ID, &signal);
#endif
  }
  else if (success == FALSE)
  {
    {
      /* Must have been trying to enable a channel for STK - so now disable it.
       */
      vgStkDisableReservedEntity (entity, TRUE);
    }
  }
}

/*--------------------------------------------------------------------------
*
* Function:    vgSendCiMuxChannelDisabledRsp
*
* Parameters:  entity  - mux channel number
*              success - if channel was successfully enabled
*
* Returns:     nothing
*
* Description: sends a mux channel enable response
*
*-------------------------------------------------------------------------*/
void vgSendCiMuxChannelDisabledRsp (const VgmuxChannelNumber entity)
{
  SignalBuffer signal = kiNullBuffer;

  KiCreateZeroSignal (SIG_CIMUX_CHANNEL_DISABLED_RSP,
                      sizeof(CiMuxChannelDisabledRsp),
                      &signal);

  signal.sig->ciMuxChannelDisabledRsp.channelNumber = entity;

  KiSendSignal (MUX_TASK_ID, &signal);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSendCiMuxConfigureChannelReq
 *
 * Parameters:  mux Index
 *
 * Returns:     Nothing
 *
 * Description: send port settings and channel configuration to the Mux
 *-------------------------------------------------------------------------*/
void vgSendCiMuxConfigureChannelReq (const VgmuxChannelNumber entity,
                                     Boolean                  forceSend)
{
  CallContext_t           *callContext_p           = ptrToCallContext(entity);
#if defined (ATCI_SLIM_DISABLE)
  FatalCheck (callContext_p != PNULL, entity, 0, 0);
#endif
  if ((getEntityState (entity) == ENTITY_PROCESSING) ||
      (getEntityState (entity) == ENTITY_RUNNING) ||
      (forceSend == TRUE))
  {
    SignalBuffer signal = kiNullBuffer;

    KiCreateZeroSignal (SIG_CIMUX_CONFIGURE_CHANNEL_REQ,
                        sizeof(CiMuxConfigureChannelReq),
                        &signal);

    signal.sig->ciMuxConfigureChannelReq.channelNumber = entity;
    getComPortSettings    (entity, &(signal.sig->ciMuxConfigureChannelReq.comPortSettings));
    getChannelConfOptions (entity, &(signal.sig->ciMuxConfigureChannelReq.channelConfOptions));

    KiSendSignal(MUX_TASK_ID, &signal);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgGetUartPortBaudRate
 *
 * Parameters:  entity
 *
 * Returns:     Nothing
 *
 * Description: get port baudrate,  save and send channel configuration to the Mux if needed.
 *-------------------------------------------------------------------------*/
void vgGetUartPortBaudRate (const VgmuxChannelNumber entity)
{
#if defined (MTK_NBIOT_TARGET_BUILD)
    PortSpeed             portSpeed = connLayerGetUartPortBaudRate(entity);
#else
    PortSpeed             portSpeed;

    portSpeed = PORTSPEED_115200;
#endif
    
    setProfileValue(entity, PROF_IPR, portSpeed);        
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgGetUartFlowControlMode
 *
 * Parameters:  entity
 *
 * Returns:     Nothing
 *
 * Description: get port baudrate,  save and send channel configuration to the Mux if needed.
 *-------------------------------------------------------------------------*/
void vgGetUartFlowControlMode (const VgmuxChannelNumber entity)
{
#if defined (MTK_NBIOT_TARGET_BUILD)
    hal_uart_flowcontrol_t flow_control = connLayerGetUartPortFlowCtrl(entity);

    setProfileValue (entity, PROF_IFC + 0, flow_control);
    setProfileValue (entity, PROF_IFC + 1, flow_control);
#else
    setProfileValue (entity, PROF_IFC + 0, FC_RTS_CTS);
    setProfileValue (entity, PROF_IFC + 1, FC_RTS_CTS);
#endif
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgMuxPORTSETTINGCHANGETimerExpiry
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:     nothing
 *
 * Description: perform changes in port configuration once timer has expired.
 *              Specifically for ICF and IPR to output "OK" before the
 *              port settings are changed.
 *-------------------------------------------------------------------------*/
void vgMuxPORTSETTINGCHANGETimerExpiry (const VgmuxChannelNumber entity)
{
  ChannelContext_t    *channelContext_p     = ptrToChannelContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(channelContext_p != PNULL, entity, 0, 0);
#endif
  /*
   * Reset the timer running flag and then send the MUX configuration.
   */
  channelContext_p->iprOrIcfSettingsChange = FALSE;

  vgSendCiMuxConfigureChannelReq (entity, TRUE);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSyncWithPs
*
* Parameters:  entity  - mux channel number
*
* Returns:     nothing
*
* Description: this function should be called when we have received our first
* channel enable.  VOP needs to do some syncing with the BG layer.
*
*-------------------------------------------------------------------------*/

void vgSyncWithPs (const VgmuxChannelNumber entity)
{
  MuxContext_t *muxContext_p = ptrToMuxContext ();

  /* first channel to be enabled so now we register with the background layer */
  if (!muxContext_p->atciRegisteredWithABPRocedures)
  {
    muxContext_p->atciRegisteredWithABPRocedures = TRUE;
    vgChManInitialRegistration (entity);
    vgSigAclkReadTimeZoneReq (entity);
  }
}

/*--------------------------------------------------------------------------
*
* Function:    vgReadAbmmWritableData
*
* Parameters:  entity  - mux channel number
*
* Returns:     Boolean
*
* Description: read ABMM writable data, including CFUN status
*
*-------------------------------------------------------------------------*/

Boolean vgReadAbmmWritableData (const VgmuxChannelNumber entity)
{
  MuxContext_t *muxContext_p = ptrToMuxContext ();
  Boolean         checkForPendingChannels = TRUE;

  /* if just enabled unsolicited channel then get phone functionality
   * information */
  if (!muxContext_p->atciHaveReadPhoneFunctionality)
  {
    if (vgOpManAllocateConnection (entity, NVRAM_CONNECTION))
    {
      muxContext_p->atciHaveReadPhoneFunctionality = TRUE;
      checkForPendingChannels = FALSE;

      vgNvramDoNvramAccess (READ_REQUEST,
                             entity,
                              NRAM2_ABMM_WRITEABLE_DATA,
                               VG_NVRAM_READ_ONLY);
    }
  }

  return (checkForPendingChannels);

}


/*--------------------------------------------------------------------------
 * Function:    vgSendCiMuxOpenDataConnReq
 *
 * Parameters:  atChannel - AT channel with which the connection is associated.
 *              domain    - CS or PS connection
 *              cid       - Connection Id
 *              ppp       - whether or not this session should use ppp signals;
 *              pppTaskId - the task to which ppp signals should be sent.
 *
 * Returns:     Nothing
 *
 * Description: Send an open data connection message filling all the
 *              appropriate fields
 *
 *-------------------------------------------------------------------------*/

void vgSendCiMuxOpenDataConnReq (VgmuxChannelNumber atChannel,
                                        DataConnType dataConnType)
{
  SignalBuffer         signal = kiNullBuffer;
  CiMuxOpenDataConnReq *request_p;
#if defined (FEA_PPP)
  GprsContext_t        *gprsContext_p = ptrToGprsContext (atChannel);


  VgPsdBearerInfo      *psdBearerInfo_p;
#endif /* FEA_PPP */

  KiCreateZeroSignal (SIG_CIMUX_OPEN_DATA_CONN_REQ,
                      sizeof (CiMuxOpenDataConnReq),
                      &signal);

  request_p = (CiMuxOpenDataConnReq *) signal.sig;

  request_p->channelNumber   = atChannel;
  request_p->dataConnType    = dataConnType;

#if defined (FEA_PPP)
  if (dataConnType == PSD_PPP)
  {
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck (gprsContext_p != PNULL, atChannel, 0, 0);
#endif
    if (gprsContext_p != PNULL)
    {
      /* Set PPP options */
      if (gprsContext_p->vgPppLoopbackState == VG_LOOPBACK_ENABLED)
      {
        /* This data connection is for loopback. Get options set by AT*MLOOPPSD command. */
        if (gprsContext_p->vgPppLoopbackPppMode == LOOPBACK_PPP_MODE_STANDARD)
        {
          request_p->dataConnOptions.pppFlagDetection = TRUE;
        }
        else
        {
          request_p->dataConnOptions.pppFlagDetection = FALSE;
        }

        request_p->dataConnOptions.pppByteStuffing   = gprsContext_p->vgPppLoopbackByteStuffingEnabled;
        request_p->dataConnOptions.pppFcsChecking    = gprsContext_p->vgPppLoopbackFCSCheckingEnabled;
      }
      else
      {


#if defined (ATCI_SLIM_DISABLE)
        FatalCheck (gprsContext_p->activePsdBearerContextWithDataConn != PNULL, atChannel, 0, 0);
#endif        
        psdBearerInfo_p = &(gprsContext_p->activePsdBearerContextWithDataConn->psdBearerInfo);
#if defined (ATCI_SLIM_DISABLE)

        FatalCheck (psdBearerInfo_p != PNULL, atChannel, 0, 0);    
#endif
        /* This is a connection to transfer real data, options are based on PSD connection type. */
        switch (psdBearerInfo_p->connType)
        {
          case ABPD_CONN_TYPE_PPP:
          {
            request_p->dataConnOptions.pppFlagDetection = TRUE;
            request_p->dataConnOptions.pppByteStuffing  = TRUE;
            request_p->dataConnOptions.pppFcsChecking   = TRUE;
            break;
          }
          case ABPD_CONN_TYPE_CORE_PPP:
          {
            request_p->dataConnOptions.pppFlagDetection = FALSE;
            request_p->dataConnOptions.pppByteStuffing  = FALSE;
            request_p->dataConnOptions.pppFcsChecking   = FALSE;
            break;
          }
          default:
          {
            /* Should not come here for other type of connections (ABPD_CONN_TYPE_NONE &
             * ABPD_CONN_TYPE_PACKET_TRANSPORT)
             */
            FatalParam (psdBearerInfo_p->connType, atChannel, 0);
            break;
          }
        }
      }
    }
  }
#endif /* FEA_PPP */

#ifdef ENABLE_AP_BRIDGE_FEATURE
#if defined (FEA_PPP)
  else
#endif /* FEA_PPP */
  if( PSD_AP_BRIDGE == dataConnType )
  {
    request_p->dataConnOptions.pppFlagDetection = FALSE;
    request_p->dataConnOptions.pppByteStuffing  = FALSE;
    request_p->dataConnOptions.pppFcsChecking   = FALSE;
  }
#endif
  KiSendSignal (MUX_TASK_ID, &signal);
}

/*--------------------------------------------------------------------------
 * Function:    vgSendCiMuxCloseDataConnReq
 *
 * Parameters:  atChannel - AT channel with which the connection is associated.
 *              domain    - CS or PS connection
 *              cid       - Connection Id
 * Returns:     Nothing
 *
 * Description: Send an close data connection message
 *
 *-------------------------------------------------------------------------*/

void vgSendCiMuxCloseDataConnReq (VgmuxChannelNumber atChannel)
{
  SignalBuffer signal = kiNullBuffer;

  KiCreateZeroSignal (SIG_CIMUX_CLOSE_DATA_CONN_REQ,
                      sizeof (CiMuxCloseDataConnReq),
                      &signal);

  signal.sig->ciMuxCloseDataConnReq.channelNumber = atChannel;

  KiSendSignal (MUX_TASK_ID, &signal);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSigCimuxAtoCommandReq
 *
 * Parameters:      entity    - AT channel number
  *                 domain    - CS or PS connection
 *                  cid       - Connection Id
 * Returns:         Nothing
 *
 * Description:     Send a SIG_CIMUX_ATO_COMMAND_REQ to the mux task
 *                  which switches the specified channel to data mode
 *-------------------------------------------------------------------------*/

void vgSigCiMuxAtoCommandReq (const VgmuxChannelNumber entity)
{
  SignalBuffer        signal = kiNullBuffer;

  KiCreateZeroSignal (SIG_CIMUX_ATO_CMD_REQ,
                      sizeof (CiMuxAtoCmdReq),
                     &signal);

  signal.sig->ciMuxAtoCmdReq.channelNumber = entity;

  KiSendSignal (MUX_TASK_ID, &signal);
}


/*--------------------------------------------------------------------------
*
* Function:    vgCiMuxOpenDataConnection
*
* Parameters:  entity  - channel number
*              domain  - CS or PS connection
*              ppp     - whether or not this session should use ppp signals
*
* Returns:     Nothing
*
* Description: opens a data connection
*
*-------------------------------------------------------------------------*/
void vgCiMuxOpenDataConnection (const VgmuxChannelNumber entity,
                                const DataConnType       dataConnType)
{
  GprsContext_t *gprsContext_p = ptrToGprsContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
#endif
  if (
#if defined (FEA_PPP)    
      (dataConnType == PSD_PPP) ||
#endif /* FEA_PPP */      
      (dataConnType == PSD_PACKET_TRANSPORT))
  {
    FatalCheck (! gprsContext_p->connectionActive, entity, dataConnType, TRUE);

    vgSendCiMuxOpenDataConnReq  (entity, dataConnType);

    gprsContext_p->connectionActive = TRUE;
  }
#ifdef ENABLE_AP_BRIDGE_FEATURE
  else if(PSD_AP_BRIDGE == dataConnType)
  {
    vgSendCiMuxOpenDataConnReq  (entity, dataConnType);
  }
#endif
}

/*--------------------------------------------------------------------------
*
* Function:    vgCiMuxCloseDataConnection
*
* Parameters:  entity  - channel number
*              domain  - CS or PS connection
*
* Returns:     Nothing
*
* Description: Closes a data connection
*
*-------------------------------------------------------------------------*/
void vgCiMuxCloseDataConnection (const VgmuxChannelNumber entity)
{
  GprsContext_t *gprsContext_p = ptrToGprsContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
#endif
  if (gprsContext_p->connectionActive == TRUE)
  {
    gprsContext_p->connectionActive = FALSE;
    vgSendCiMuxCloseDataConnReq (entity);
  }

  /* Handle race condition - we may have asked the MUX to open a data channel
   * which we don't need now.
   */
  gprsContext_p->pendingOpenDataConnCnf = FALSE;

}

/*--------------------------------------------------------------------------
*
* Function:    vgSendCiMuxSetRing
*
* Parameters:  entity    - channel number
*              StartRing - TRUE to start, FALSE to stop
*
* Returns:     Nothing
*
* Description: Sends a request for Ringing to be started or stopped on
*              specific channel.
*
*-------------------------------------------------------------------------*/
void vgSendCiMuxSetRing (const VgmuxChannelNumber entity, Boolean startRing )
{
  SignalBuffer signal = kiNullBuffer;
  {
    if (startRing == TRUE)
    {
       KiCreateZeroSignal(SIG_CIMUX_RING_INDICATOR_ON_REQ,
                          sizeof(CiMuxRingIndicatorOnReq),
                          &signal);
       signal.sig->ciMuxRingIndicatorOnReq.channelNumber = entity;
    }
    else
    {
       KiCreateZeroSignal(SIG_CIMUX_RING_INDICATOR_OFF_REQ,
                          sizeof(CiMuxRingIndicatorOffReq),
                          &signal);
       signal.sig->ciMuxRingIndicatorOffReq.channelNumber = entity;
    }
    KiSendSignal(MUX_TASK_ID, &signal);
  }
} /* vgSendCiMuxSetRing() */

/* END OF FILE */

