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
 **************************************************************************/
/** file description
 * This module handles Call Control on SIM functionality in the Raven SIK
 * sub-systsem.
  **************************************************************************/

#define MODULE_NAME "RVSTKCC"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#if !defined (SYSTEM_H)
#  include <system.h>
#endif

#if !defined (RVSTKCC_H)
#  include <rvstkcc.h>
#endif

#if !defined (RVCHMAN_H)
#  include <rvchman.h>
#endif

#if !defined (RVOMAN_H)
#  include <rvoman.h>
#endif

#include <stdio.h>

#if !defined (ABST_SIG_H)
#  include <abst_sig.h>
#endif

 #if !defined (ABCC_TYP_H)
#  include <abcc_typ.h>
#endif

#if !defined (RVCRHAND_H)
#  include <rvcrhand.h>
#endif

#include <rvcimxut.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  ApexStCcStatusInd        apexStCcStatusInd;
};

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/


/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/
  void vgStkDisplayCallControlMessage(SimatCallControlInfoType  ccType,
                                      SimatCcStatus   ccStatus, 
                                      Boolean         alphaIdPresent,
                                      SimatAlphaIdentifier *alphaId,
                                      const VgmuxChannelNumber entity)
  {
    VgMCCSTType           mCcStType   = VG_MCCST_SMS;
    VgMCCSTEventType      mCStEvent   = VG_MCCST_ALLOWED;
    VgmuxChannelNumber    printEntity = VGMUX_CHANNEL_INVALID; 
    switch ( ccType)
    {
     case SIMAT_CC_SM_OPERATION :
        mCcStType = VG_MCCST_SMS ;
        switch ( ccStatus)
        {
          case SIMAT_CC_SM_BARRED :
            mCStEvent = VG_MCCST_BARRED;
          break; 
          case SIMAT_CC_SM_CHANGED :
            mCStEvent = VG_MCCST_MODIFIED;
          break;
          case SIMAT_CC_SM_OK :
            mCStEvent = VG_MCCST_ALLOWED;
          break;
          default :
            FatalParam (ccType, ccStatus, 0);
            break;   
        }
        break;

     case SIMAT_CC_GP_OPERATION:
        mCcStType =  VG_MCCST_GPRS_CONTEXT;
        switch ( ccStatus )
        {
          case SIMAT_CC_GP_CALL_BARRED :
            mCStEvent = VG_MCCST_BARRED;
            break;
          case SIMAT_CC_GP_PDP_CHANGED :
            mCStEvent = VG_MCCST_MODIFIED;
            break;
          case SIMAT_CC_GP_NO_CHANGE: 
            mCStEvent = VG_MCCST_ALLOWED;
            break;
          default :
            FatalParam (ccType, ccStatus, 0);
            break;  
        }
        break;     

      default:
        FatalParam (ccType, ccStatus, 0);
       break;
    }     
          
    if(isEntityMmiNotUnsolicited(entity))
    {
      printEntity = vgGetMmiUnsolicitedChannel(); 
    }

    if (printEntity != VGMUX_CHANNEL_INVALID)
    {
      vgPrintf (printEntity, (Char *)"*MCCST: %d,%d", (int) mCcStType, (int) mCStEvent);
  
      if ( alphaIdPresent )
      {
        vgPutNewLine (printEntity); 
        vgPrintf (printEntity, (Char *)",\"");
        if (( alphaId->length >0 ) && (alphaId->length < SIMAT_ALPHA_ID_SIZE)) 
        {
          alphaId->data[alphaId->length] = NULL_CHAR;
          vgPrintf (printEntity, (Char *)"%s", (Char *)alphaId->data);
        } 
        vgPrintf (printEntity, (Char *)"\"");
      }
      vgPutNewLine (printEntity);
      vgFlushBuffer (printEntity);    
    }
  }  
/***************************************************************************
 * 
 * Global Functions
 ***************************************************************************/
/***************************************************************************
 * Function:     vgStkCheckCallControlStatus
 *
 * Parameters:  signalBuffer- Pointer to the incoming signal
 *
 * Returns:     None
 *
 * Description: This function checks the incoming signal for call status information
 * for a call being set up as a result of a SET UP CALL proactive command.
 
 ***************************************************************************/

void vgStkCheckCallControlStatus (const SignalBuffer signalBuffer,
                                 const VgmuxChannelNumber entity)
{
  ApexStCcStatusInd        *ccStatusSignal = &signalBuffer.sig->apexStCcStatusInd;

  switch (ccStatusSignal->messageBody.type)
  {
    case SIMAT_CC_SM_OPERATION:
      switch (ccStatusSignal->messageBody.status)
      {
        case SIMAT_CC_SM_CHANGED:
        /* deliberate fallthrough on case statement */  
        case SIMAT_CC_SM_BARRED:
        /* deliberate fallthrough on case statement */  
        case SIMAT_CC_SM_OK:
         vgStkDisplayCallControlMessage(ccStatusSignal->messageBody.type, 
         ccStatusSignal->messageBody.status, ccStatusSignal->alphaIdPresent,
         &ccStatusSignal->alphaId, entity);
        break;

        default :
          WarnParam(ccStatusSignal->messageBody.type, ccStatusSignal->messageBody.status, 0);
         break;
      }
      break;
 
    case SIMAT_CC_GP_OPERATION :
      switch (ccStatusSignal->messageBody.status)
      {
        case SIMAT_CC_GP_NO_CHANGE :
        /* deliberate fallthrough on case statement */       
        case SIMAT_CC_GP_PDP_CHANGED:
        /* deliberate fallthrough on case statement */     
        case SIMAT_CC_GP_CALL_BARRED: 
         vgStkDisplayCallControlMessage(ccStatusSignal->messageBody.type, 
         ccStatusSignal->messageBody.status, ccStatusSignal->alphaIdPresent,
         &ccStatusSignal->alphaId, entity);
         break;
                 
        default :
          WarnParam(ccStatusSignal->messageBody.type, ccStatusSignal->messageBody.status, 0);
         break;
      }
      break;
    default:
      WarnParam(ccStatusSignal->messageBody.type, ccStatusSignal->messageBody.status, 0);
      break;
  }
}

/* END OF FILE */

