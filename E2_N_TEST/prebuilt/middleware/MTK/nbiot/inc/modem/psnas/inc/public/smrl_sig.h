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
 *      Sim Tl - Rl interface
 **************************************************************************/

#ifndef SMRL_SIG_H
#define SMRL_SIG_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#if !defined (TL_TYP_H)
#include     <tl_typ.h>
#endif

/* Include SMS routing info */
#if !defined (SITS_TYP_H)
#include     <sits_typ.h>
#endif

/** \defgroup SigSms SMS Signalling Interface
 * \ingroup Prd3gDmNas
 * \ingroup Sig 
 * SMRL, SMCM and SMTS signalling interfaces.
 * This section covers the SMRL, SMTS and SMCM interfaces which are respectively : <UL>
 * <LI> interface between Short Message Transfer Layer (SM-TL) and Short Message Relay Layer (SM-RL).
 * <LI> interface between Short Message Transfer Layer (SM-TL) and Application layer
 * <LI> internal SM-CM interface</UL>
 * The SMS signalling interface is illustrated in te following Figure:
 *
 * \image html MMxx_Interface.png
 * @{
 */

/** \defgroup SigSmsSmrl SMRL Signalling Interface
 * \ingroup SigSms
 * \ingroup Sig 
 * Interface between SM-TL and SM-RL.
 * The Short Message Relay Layer (SM-RL) provides a service to the Short Message 
 * Transfer Layer (SM-TL). This service enables the SM TL to send Transfer 
 * Protocol Data Units (TPDUs) to its peer entity, receive TPDUs from its peer 
 * entity and receive reports about earlier requests for TPDUs to be transferred.
 * @{
 */
/****************************************************************************
 * Macros
 ****************************************************************************/
/** Maximum amount of user data associated with an RP-DATA message (3GPP 24.011 section 7.3 )*/
#define     MAX_RP_DATA_USER_DATA       232 /* used same value as tpdu size, corrects overrun */
/** Maximum amount of user data associated with an RP-ERROR message (3GPP 24.011 section 7.3 )*/
#define     MAX_RP_ERROR_USER_DATA      232 /* used same value as tpdu size, corrects overrun */
/** Maximum amount of user data associated with an RP-ACK message (3GPP 24.011 section 7.3 )*/
#define     MAX_RP_ACK_USER_DATA_LEN    234

/****************************************************************************
 * Types
 ****************************************************************************/

/****************************************************************************
 * Signal types
 ****************************************************************************/

/** The SmrlDataReq signal is used by the SM-TL task to initiate a new SM by 
 * passing a TPDU and other control parameters to the SM-RL task
 */
typedef struct SmrlDataReqTag
{
   /** Identifies the message, this must be unique, to differentiate other
    * instances
    */
   Int8                             shortMsgId;
   /** Used for identifying SMs between the MS and the network. 
    * It is allocated by the AL (from the SIM) for MO-SM and by 
    * the network for MT-SM and then passed into SM-RL via the TS interface. 
    */
   Int8                             rpMsgRef;
   /** Holds information on the number of the Service Centre that should transfer the SM */
   RawSmsAddress                    destAddr;
   /** Holds the TPDU information */
   RpUserDataElement                userData;
   /** Indicates to SM-RL which route is to be used for sending SMS 
    * message (GPRS or GSM)
    */
   SmsRoute                         smsRoute;  
}
SmrlDataReq;

/** Indicates to SM-TL reception of RP_DATA message.
 * The SmrlDataInd signal is used to pass an incoming RP-DATA to 
 * the SM-TL task in a TPDU, it also includes additional control information
 */
typedef struct SmrlDataIndTag
{
   /** Short message identifier (SMI) which is used
    * by SM-TL to pass the SMRL_REPORT_REQ reply to the correct SMR entity
    */
   Int8                             shortMsgId;
   /** Holds the number for the Service Centre that transferred this message */  
   RawSmsAddress                    origAddr;
   /** Holds the TPDU information */
   RpUserDataElement                userData;
   /** Indicates whether the incoming SMS message was ciphered*/
   Boolean                          smsCipher;    /* route of MO SMS _CNF from network */
}
SmrlDataInd;

/** The SmrlMemAvailReq signal used to indicate to the SM-RL task that it should create an 
 * RP-SM_MEMORY_AVAILABLE and send it to its peer entity in the network.
 * This signal is sent, by the Application Software, to inform the SMS entity that
 * there is memory available in the mobile to store at least one extra short message.
 * This information is passed onto SMS-RL which is responsible for informing the
 * Network/Service centre.
 */
typedef struct SmrlMemAvailReqTag
{
   /** Identifies the message, this must be unique, to differentiate other
    * instances
    */
   Int8                             shortMsgId;
    /** Used for identifying SMs between the MS and the network. 
     * It is allocated by the AL (from the SIM) for MO-SM and by 
     * the network for MT-SM and then passed into SM-RL via the TS interface 
     */
   Int8                             rpMsgRef;
   /** Indicates to SM-RL which route is to be used for sending 
    * the RP-SM_MEMORY_AVAILABLE message (GPRS or GSM)
    */
   SmsRoute                         smsRoute;  
}
SmrlMemAvailReq;

/** The SmrlReportReq is used by the SM-TL to relay the RP-ACK or 
 * RP-ERROR message from the mobile station to the network
 */
typedef struct SmrlReportReqTag
{
   /** SMS identifier which is a reference number for the TPDU 
    * associated with the primitive 
    */
   Int8                             shortMsgId;
   /** Indicates whether the SM-RL task should build an RP-ACK 
    * (field has value TRANSFER_OK) or an RP-ERROR (field has value TRANSFER_ERROR)
    */
   SmsStatusOfReport                statusOfReport;
   /** Indicates the type of error in the case when an RP-ERROR is to be built*/
   TpFailureCause                   tpFailureCause;
   /** The length of RP-user data 
\assoc ARRAY \ref userData */
   Int16                            userDataLength;
   /** Holds the TPDU */
   Int8                             userData[MAX_RP_ACK_USER_DATA_LEN];
}
SmrlReportReq;

/** The SmrlReportInd is used to pass acknowledgement (RP-ACK) 
 * or error information (RP-ERROR) from the SM-RL task to the SM-TL task
 */
typedef struct SmrlReportIndTag
{ 
   /** Identifies the message */
   Int8                             shortMsgId;
   /** field indicates whether the signal is for an RP-ACK 
    * (field has value TRANSFER_OK) or an RP-ERROR (field has value TRANSFER_ERROR)
    */
   SmsStatusOfReport                statusOfReport;
   /** Indicates the type of relay error in the case when the signal is for an RP-ERROR */
   RpCauseElement                   rpCause;
   /** indicates whether or not the userData field is present 
\assoc PRESENT \ref userData */
   Boolean                          userDataPresent;
   /** Contains a TPDU */ 
   RpUserDataElement                userData;
   /** Indicates whether the incoming SMS report was ciphered*/
   Boolean                          smsCipher;    
   /* add for mantis 144469,Indicates whether the incoming SMS was success or failure */
   Boolean                          mtSmsFailure;

}
SmrlReportInd;

/*job100559 Signal to allow AL to signal readiness to receive*/
/*job116776 introducing moreMessagesToSend functionality*/
/** The SmrlConfigReq is used to indicate to the 
 * SM-TL task that the AL is ready to start receiving SMS
 */
typedef struct SmrlConfigReqTag
{
  /** If set to TRUE it indicates AL is ready */
  Boolean                           isReady;  
  /** Indicates if more messages need to be sent */
  Boolean                           moreMsgsToSend;
}
SmrlConfigReq;

/** @} */ /* End of SigSmrl group */
/** @} */ /* End of SigSms group */
#endif
/* END OF FILE */































