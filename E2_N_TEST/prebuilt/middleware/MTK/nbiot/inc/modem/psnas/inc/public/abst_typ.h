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
 *  File Description :                                                   */
/** \file
 SIM toolkit ABST Types
 **************************************************************************/

#ifndef ABST_TYP_H
#define ABST_TYP_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/



/****************************************************************************
 * Macros
 ****************************************************************************/


/****************************************************************************
 * Types
 ****************************************************************************/
/** Specifies the result of checking terminal profile */ 
typedef enum SimatTerminalProfileCheckTag
{
  /** Terminal profile check is OK */
  SIMAT_TP_OK                    =    0x00,
   /** Error in check of terminal profile */
  SIMAT_TP_ERROR                 =    0x01

} SimatTerminalProfileCheck;

/** Defines the result of Call Control by the SIM */
typedef enum SimatCcStatusTag
{
  /** Call control is not changed */
  SIMAT_CC_NO_CHANGE             = 0x00,
  /** SIM/UICC has modified a call request */
  SIMAT_CC_CALL_CHANGED          = 0x01,
  /** SIM/UICC  has barred a call request */
  SIMAT_CC_CALL_BARRED           = 0x02,
  /** SIM/UICC has replaced a call request by a SS operation */
  SIMAT_CC_CALL_REPLACED_BY_SS   = 0x03,
  /** SIM/UICC has modified a SS operation */
  SIMAT_CC_SS_CHANGED            = 0x04,
  /** SIM/UICC has barred a SS operation */
  SIMAT_CC_SS_BARRED             = 0x05,
  /** SIM/UICC has replaced a SS operation by a call request */ 
  SIMAT_CC_SS_REPLACED_BY_CALL   = 0x06,
  /** SIM/UICC  SS operation is failed */
  SIMAT_CC_SS_FAILED             = 0x07,
  /** SIM/UICC call request is failed */
  SIMAT_CC_CALL_FAILED           = 0x08,
  /** SIM/UICC SS operation is OK */
  SIMAT_CC_SS_OK                 = 0x09,
  /** SIMAT CC call request is OK */
  SIMAT_CC_CALL_OK               = 0x0A,
  /** SIM/UICC has modified a SM */
  SIMAT_CC_SM_CHANGED            = 0x0B,
  /** SIM/UICC has barred a SM */
  SIMAT_CC_SM_BARRED             = 0x0C,
  /** Call control SMS is OK */
  SIMAT_CC_SM_OK                 = 0x0D,
  /** SIM/UICC USSD operation is failed */
  SIMAT_CC_USSD_FAILED           = 0x0E,
  /** SIM/UICC USSD operation is OK */
  SIMAT_CC_USSD_OK               = 0x0F,
  /** SIM/UICC has replaced a call request by a USSD operation */
  SIMAT_CC_CALL_REPLACED_BY_USSD = 0X10,
  /** SIM/UICC has replaced a SS operation by a USSD operation */ 
  SIMAT_CC_SS_REPLACED_BY_USSD   = 0x11,
  /** SIM/UICC has modified a USSD */
  SIMAT_CC_USSD_CHANGED          = 0x12,
  /** SIM/UICC has replaced a USSD operation by a call request */ 
  SIMAT_CC_USSD_REPLACED_BY_CALL = 0x13,
  /** SIM/UICC has replaced a USSD operation by an SS */ 
  SIMAT_CC_USSD_REPLACED_BY_SS   = 0x14,
  /** USSD Operation barred */ 
  SIMAT_CC_USSD_BARRED           = 0x15  ,
  /**GPRS_CALL_CONTROL_States*/
  /** PDP context is not changed */
  SIMAT_CC_GP_NO_CHANGE          = 0x16,
  /** SIM/UICC has modified a call request */
  SIMAT_CC_GP_PDP_CHANGED        = 0x17,
  /** SIM/UICC  has barred a call request */
  SIMAT_CC_GP_CALL_BARRED        = 0x18
}
SimatCcStatus;

/** Used to determine whether Call Control was successful or if it failed.*/
typedef enum SimatCcResultTag
{
  /** Call control failed */
  SIMAT_CC_FAIL                  = 0x00,
  /** Call control was successful */
  SIMAT_CC_OK                    = 0x01
}
SimatCcResult;

/** Defines the type of data download error which occurred */
typedef enum SimatDownloadErrorTag
{
  /** No error */
  SIMAT_NO_ERROR                 = 0x00,
  /** Error occured when downloading data via SMS-PP */
  SIMAT_SMS_PP_ERROR             = 0x01,
  /** Error occured when downloading data via SMS-CB */
  SIMAT_SMS_CB_ERROR             = 0x02
} SimatDownloadError;

/** Specifies the origin of the alpha ID,for instance from a SMS 
 * or SS SAT proactive command.
 */
typedef enum SimatAlphaIdTypeTag
{
  /** Open channel */
  SIMAT_OP_ALPHA_ID , 
  /** Send data */
  SIMAT_SE_ALPHA_ID , 
  /** Receive data */
  SIMAT_RE_ALPHA_ID ,  
  /** Close channel */
  SIMAT_CL_ALPHA_ID ,  
  /** Suplementary service alpha ID */
  SIMAT_SS_ALPHA_ID ,
  /** USSD alpha ID */
  SIMAT_USSD_ALPHA_ID,
  /** SMS alpha ID */
  SIMAT_SM_ALPHA_ID ,
  /** Call control */
  SIMAT_CC_ALPHA_ID ,
  /**GPRS Call Control */
  SIMAT_GP_CC_ALPHA_ID,
  /** Send DTMF */
  SIMAT_SD_ALPHA_ID ,  
  /** Call control second alpha ID */
  SIMAT_CC_SECOND_ALPHA_ID,
  /** count of enumeration */
  SIMAT_ALPHA_ID_NUM
} SimatAlphaIdType;

/** Specifies the user feedback */
typedef enum SimatUserFeedbackTag
{
   /** User has accepted the call set-up */
   SIMAT_USER_ACCEPT_CALL,
   /** User has not accepted the call set-up */
   SIMAT_USER_REFUSE_CALL,
   /** User has terminated the session */
   SIMAT_USER_TERM_SESSION
} SimatUserFeedback;

/** Used to determine whether MO-SMS was successful or if it failed.*/
typedef enum SimatMoSmResultTag
{
  /** MO-SMS failed */
  SIMAT_MO_SM_FAIL                  = 0x00,
  /** MO-SMS was successful */
  SIMAT_MO_SM_OK                    = 0x01
}SimatMoSmResult;

/* used to decide what action is required when the user can cancel */
typedef enum SimatCommandCancelTypeTag
{
  /** do not cancel */
  SIMAT_NO_CANCEL                 = 0x00,
  /** cancel command */
  SIMAT_CANCEL_COMMAND            = 0x01,
  /** cancel proactive session */
  SIMAT_CANCEL_SESSION            = 0x02
}SimatCommandCancelType;

#endif
/* END OF FILE */
