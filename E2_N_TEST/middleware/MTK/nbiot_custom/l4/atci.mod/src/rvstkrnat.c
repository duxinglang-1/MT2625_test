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
 * This module is responsible for handling the setting up and processing of
 *  the proactive command RUN AT COMMAND
 **************************************************************************/

#define MODULE_NAME "RVSTKRNAT"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#if !defined (SYSTEM_H)
#  include <system.h>
#endif

#if !defined (RVSTKRNAT_H)
#  include <rvstkrnat.h>
#endif

#if !defined (RVCIMXUT_H)
#  include <rvcimxut.h>
#endif

#if !defined (KERNEL_H)
#  include <kernel.h>
#endif

#if !defined (RVDATA_H)
#  include <rvdata.h>
#endif

#if !defined (RVSTKTP_H)
#  include <rvstktp.h>
#endif

#if !defined (AFSA_SIG_H)
#  include <afsa_sig.h>
#endif

#if !defined (RVCIMXUT_H)
#  include <rvcimxut.h>
#endif

#  include <rvpdsigo.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
   EmptySignal                  ciMuxReserveChannelReq;
   CiMuxAtDataInd               ciMuxAtDataInd;
   AfsaRunAtCommandRsp          afsaRunAtCommandRsp;
};

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

void vgStkRunAtCommandFail (const VgmuxChannelNumber entity);
/***************************************************************************
 * Type Definitions
 ***************************************************************************/


/***************************************************************************
 * Local Variables
 ***************************************************************************/


/***************************************************************************
 * Local Functions
 ***************************************************************************/
  
/*--------------------------------------------------------------------------
 *
 * Function:    vgStkRunAtCommandFail *
 * Parameters:  entity         STK entity   
 *
 * Returns:     Nothing *
 * Description: Routine to report that the proactive command RUN AT Command has 
 *              failed and clear the data stored form the command.
 *
 *-------------------------------------------------------------------------*/

void vgStkRunAtCommandFail (const VgmuxChannelNumber entity)
{
  StkEntityGenericData_t *stkGenericContext_p = ptrToStkGenericContext ();
#if defined (ATCI_SLIM_DISABLE)
  FatalCheck(stkGenericContext_p != PNULL, entity, 0, 0);
#endif
  vgStkClearStkRunAtCommandData();
  /* set the data to invalid so the response to the SIM will indicate failure */
  stkGenericContext_p->dataValid = FALSE;
  vgAfsaRunAtCommandRsp ();
}

  
/***************************************************************************
 * Global Functions
 ***************************************************************************/
/*------------------------------------------------------------------------
 *
 * Function:    vgStkSetUpChannelToRunAtCommand
 *
 * Parameters:   entity         entity for STK operations
 * 
 * Returns  Nothing
 * 
 * Description:  Enable a channel as if for communications with external entity
 * to use to send an at command.
 *-------------------------------------------------------------------------*/

void vgStkSetUpChannelToRunAtCommand (const VgmuxChannelNumber entity)
{
  StkEntityGenericData_t *stkGenericContext_p = ptrToStkGenericContext ();
  SignalBuffer            enableBuff = kiNullBuffer;
  CiMuxChannelEnableInd  *rqEnable_p;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(stkGenericContext_p != PNULL, entity, 0, 0);
#endif
  /* the MUX should have reserved a channel for the ATCI to use or we could be using 
   * an already reserved channel otherwise we shouldn'ty be trying to enable a 
   * channel */   
  if (( stkGenericContext_p->atCommandData.cmdEntity != VGMUX_CHANNEL_INVALID )
  && (stkGenericContext_p-> runAtCmdState == STATE_RESERVING_CHANNEL)) 
  {
    stkGenericContext_p-> runAtCmdState = STATE_ENABLING_CHANNEL;
    KiCreateZeroSignal (SIG_CIMUX_CHANNEL_ENABLE_IND,
                        sizeof (CiMuxChannelEnableInd),
                        &enableBuff);

    rqEnable_p                = (CiMuxChannelEnableInd *) enableBuff.sig;
    rqEnable_p->channelNumber = stkGenericContext_p->atCommandData.cmdEntity;
    KiSendSignal (VG_CI_TASK_ID, &enableBuff);
  }
  else
  {
    vgStkRunAtCommandFail (entity);
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgStkClearStkRunAtCommandData
 *
 * Parameters:  Nothing
 *  * 
 * Returns  Nothing
 * 
 * Description:  Clears data from the AtCommandData in the STK generic context
 *               freeing assigned buffers and resets the RunAtCommand state 
 *               machine.
 *-------------------------------------------------------------------------*/

void vgStkClearStkRunAtCommandData( void )
{
  StkEntityGenericData_t *stkGenericContext_p = ptrToStkGenericContext ();

  FatalAssert(stkGenericContext_p != PNULL);
  stkGenericContext_p->runAtCmdState = STATE_STK_NULL;
  stkGenericContext_p->atCommandData.cmdEntity = VGMUX_CHANNEL_INVALID;
  if ( stkGenericContext_p->atCommandData.cmdInput_p != PNULL )
  {
    KiFreeMemory( (void**)&stkGenericContext_p->atCommandData.cmdInput_p );
  }
  if ( stkGenericContext_p->atCommandData.cmdOutput_p != PNULL )
  {
    KiFreeMemory( (void**)&stkGenericContext_p->atCommandData.cmdOutput_p );
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkRequestMuxChannelToRunAtCommand
 *
 * Parameters:  
 *               entity        - mux channel number
 *
 * Returns:     Nothing *
 * Description: Send a signal to the MUX to reserve a channel so the ATCI can 
 * use it internally *
 *-------------------------------------------------------------------------*/

void vgStkRequestMuxChannelToRunAtCommand(const VgmuxChannelNumber entity)
{
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();
#if defined (ATCI_SLIM_DISABLE)  
  FatalCheck(stkGenericContext_p != PNULL, entity, 0, 0);
#endif
  vgApexAbpdChannelAllocReq    (entity);

  stkGenericContext_p->runAtCmdState = STATE_RESERVING_CHANNEL;
}

/*--------------------------------------------------------------------------
 *
 * Function:     vgStkProcessProactiveCommandRunAtCommand
 * 
 * Parameters:  none - data has been set up already 
 *
 * Returns:     none *
 * Description:  *  Now the channel has been enabled need to sent the 
 * CiRunAtCommandInd to actually perform the AT  command
 *-------------------------------------------------------------------------*/

void vgStkProcessProactiveCommandRunAtCommand ()
{
    SignalBuffer               sigBuff  = kiNullBuffer;
    StkEntityGenericData_t    *stkGenericContext_p = ptrToStkGenericContext ();
    SimatAtCommandData        *atCommand_p;
    CiMuxAtDataInd              *request_p;

    FatalAssert(stkGenericContext_p != PNULL);
    FatalAssert(stkGenericContext_p->atCommandData.cmdInput_p != PNULL);
    atCommand_p = stkGenericContext_p->atCommandData.cmdInput_p;
    KiCreateSignal(SIG_CIMUX_AT_DATA_IND,
                        sizeof (CiMuxAtDataInd), 
                        &sigBuff);
    request_p = (CiMuxAtDataInd *) sigBuff.sig;
    request_p->channelNumber = stkGenericContext_p->atCommandData.cmdEntity;
    request_p->length = atCommand_p->dataLength+1;
    memcpy(request_p->data, atCommand_p->data, atCommand_p->dataLength);
    /* add Carriage Return Char */
    request_p->data[atCommand_p->dataLength] = 
              getProfileValue (stkGenericContext_p->atCommandData.cmdEntity, PROF_S3);

    KiSendSignal(VG_CI_TASK_ID, &sigBuff);
    stkGenericContext_p->runAtCmdState = STATE_RUNNING_COMMAND;
} 

/*--------------------------------------------------------------------------
 *
 * Function:    vgAfsaRunAtCommandRsp
 *
 * Parameters:  
 *                *
 * Returns:     
 *  *
 * Description: Sends the data for the terminal response to the SIMAT task 
 *              to return to the SIM.
 *
 *-------------------------------------------------------------------------*/

void vgAfsaRunAtCommandRsp (void)
{
  SignalBuffer             sendSignal = kiNullBuffer;
  AfsaRunAtCommandRsp     *afsaRunAtCommandRsp_p;
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();


  FatalAssert(stkGenericContext_p != PNULL);
  KiCreateZeroSignal (SIG_AFSA_RUN_AT_COMMAND_RSP,
                      sizeof (AfsaRunAtCommandRsp),
                      &sendSignal);

  afsaRunAtCommandRsp_p = &sendSignal.sig->afsaRunAtCommandRsp;

  /* set the command reference number to the same as the AfsaRunAtCommandInd command. */
  afsaRunAtCommandRsp_p->simatCommandRef = stkGenericContext_p->simCommandId;

  if (stkGenericContext_p->dataValid == FALSE)
  {
    afsaRunAtCommandRsp_p->generalResult    = SIMAT_GR_ME_CANNOT_PROC_COMMAND;
    afsaRunAtCommandRsp_p->meProblemPresent = TRUE;
    afsaRunAtCommandRsp_p->meProblem        = SIMAT_INFO_NO_CAUSE_GIVEN;
  }
  else
  /* we have valid data to return  add it to the signal then clear the context data and
   * reset the state - stays enabled as channel not disabled until proactive session ends*/
  {
    if (stkGenericContext_p->atCommandData.cmdOutput_p != PNULL )
    {
      if (stkGenericContext_p->atCommandData.cmdOutput_p->dataLength != 0 )
      {
        memcpy(afsaRunAtCommandRsp_p->atResponse.data,
               stkGenericContext_p->atCommandData.cmdOutput_p->data,
               stkGenericContext_p->atCommandData.cmdOutput_p->dataLength);
        afsaRunAtCommandRsp_p->atResponse.dataLength = 
                                stkGenericContext_p->atCommandData.cmdOutput_p->dataLength;
      }
      memset(stkGenericContext_p->atCommandData.cmdOutput_p, 0, sizeof(SimatAtCommandData));
    }
    if (stkGenericContext_p->atCommandData.cmdInput_p != PNULL )
    {
      memset(stkGenericContext_p->atCommandData.cmdInput_p, 0, sizeof(SimatAtCommandData));
    }
    afsaRunAtCommandRsp_p->generalResult    = SIMAT_GR_COMM_PERF_OK;
    afsaRunAtCommandRsp_p->meProblemPresent = FALSE;

    stkGenericContext_p->runAtCmdState = STATE_CHANNEL_ENABLED;
    stkGenericContext_p->dataValid = FALSE;
  }
  KiSendSignal (SIMAT_TASK_ID, &sendSignal);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkDisableReservedEntity
 *
 * Parameters:  entity:                 Current STK entity.
 *              enableChannelFailed:    TRUE if we tried to enable the channel
 *                                      but it failed - so this is as a result
 *                                      of the enable failure.
 * Returns:     
 *  *
 * Description: Sends signal to disable the channel used internally by the atci 
 * to perfomnr the proactive command 'RUN AT COMMAND@ *
 *-------------------------------------------------------------------------*/

void vgStkDisableReservedEntity (const VgmuxChannelNumber entity,
                             Boolean enableChannelFailed)
{
  StkEntityGenericData_t *stkGenericContext_p = ptrToStkGenericContext ();

  FatalAssert(stkGenericContext_p != PNULL);
  /* now need to tell MUX that the channel is free */
  if (enableChannelFailed == TRUE)
  {
    stkGenericContext_p->runAtCmdState = STATE_FREEING_CHANNEL_ENABLE_FAILED;
  }
  else
  {
    stkGenericContext_p->runAtCmdState = STATE_FREEING_CHANNEL;
  }

  vgApexAbpdChannelFreeReq (entity, stkGenericContext_p->atCommandData.cmdEntity);

}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkAddDataToStkAtResponseBuffer
 *
 * Parameters:  
 *                *
 * Returns:     
 *  *
 * Description: Sends signal to disable the channel used internally by the atci 
 * to perfomnr the proactive command 'RUN AT COMMAND@ *
 *-------------------------------------------------------------------------*/

void vgStkAddDataToStkAtResponseBuffer(Int8 respLength, Int8 *atResponse_p)
{
  SimatAtCommandData   *stkAtResponse = PNULL;
  StkEntityGenericData_t *stkGenericContext_p = ptrToStkGenericContext ();

  FatalAssert(stkGenericContext_p != PNULL);
  if ( stkGenericContext_p->atCommandData.cmdOutput_p == PNULL)
  {
    
    KiAllocZeroMemory(sizeof(SimatAtCommandData),
            (void **)&stkGenericContext_p->atCommandData.cmdOutput_p);
             
  }
  stkAtResponse = stkGenericContext_p->atCommandData.cmdOutput_p;
  /* if the buffer isn't full yet ( if it is just dump the data we just send as much as we can )*/
  if ( stkAtResponse->dataLength < SIMAT_AT_COMM_MAX_LEN)
  {
    if ( stkAtResponse->dataLength + respLength > SIMAT_AT_COMM_MAX_LEN)
    {
      respLength = SIMAT_AT_COMM_MAX_LEN - stkAtResponse->dataLength;
    }

    if ( respLength > 0 )
    {
      /* copy the at command response string into the command response buffer appending to any
       * previous data already stored  */
      memcpy(&stkAtResponse->data[stkAtResponse->dataLength], atResponse_p, respLength);
      /* update the stored length */
      stkAtResponse->dataLength += respLength;
    }
  }
}
/* END OF FILE */

