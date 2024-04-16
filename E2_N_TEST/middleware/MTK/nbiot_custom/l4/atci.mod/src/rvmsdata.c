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
 * Initialisation and access of SMS data.
 **************************************************************************/

#define MODULE_NAME "RVMSDATA"

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <rvsystem.h>
#include <rvmsdata.h>
#include <rvdata.h>
#include <rvmsut.h>

static void initEntitySpecificData (SmsContext_t *smsContext_p);

 /****************************************************************************
 *
 * Function:    initialiseMsss
 *
 * Parameters:  smsContext_p - pointer to entity specific sms data
 *
 * Returns:     nothing
 *
 * Description: sets general sub-system entity specific data
 *
 ****************************************************************************/

static void initEntitySpecificData (SmsContext_t *smsContext_p)
{
    memset( smsContext_p->smsMessage, 0, sizeof(smsContext_p->smsMessage) );

    smsContext_p->smsLength  = 0;
    smsContext_p->tpduLength = 0;
    smsContext_p->maxSmsMessageLength = 0;
    smsContext_p->validSms = TRUE;

    smsContext_p->concatSms.sequenceNumber = 0;
    smsContext_p->concatSms.sequenceLength = 0;
    smsContext_p->concatSms.concatRef = 0;

    smsContext_p->cmglRecordNumberToSearchFrom = VG_SMS_INVALID_RECORD_NUMBER;

    memset( &smsContext_p->command,   0, sizeof(smsContext_p->command) );
    memset( &smsContext_p->da,        0, sizeof(smsContext_p->da) );

    smsContext_p->readState = SM_RW_ABSOLUTE;

    /* job118116 adds use of <delflag> */
    smsContext_p->delFlag = 0;

    /* Extra unsolicited messages are enabled by default */
    smsContext_p->enableExtraUnsol = FALSE;  
    smsContext_p->smsStoreLoc = 0;

    /* Init CPMS Param with "SM" type by default */
    smsContext_p->cpmsReadDelete = VG_CPMS_STORAGE_SM;
    smsContext_p->cpmsWriteSend  = VG_CPMS_STORAGE_SM;

    memset( &smsContext_p->atCmssParams, 0, sizeof(smsContext_p->atCmssParams) );
    memset( &smsContext_p->smTimeStamp[0], 0, sizeof(smsContext_p->smTimeStamp) );
}


 /****************************************************************************
 *
 * Function:    initialiseMsss
 *
 * Parameters:  none
 *
 * Returns:     nothing
 *
 * Description: Initialises all SMS data.
 *
 ****************************************************************************/

void vgSmsInitialiseData (const VgmuxChannelNumber entity)
{
  SmsContext_t *smsContext_p = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  initEntitySpecificData (smsContext_p);
}

 /****************************************************************************
 *
 * Function:    initCommonEntityData
 *
 * Parameters:  none
 *
 * Returns:     nothing
 *
 * Description: Initialises the SMS data that is common to all entities
 *
 ****************************************************************************/

void initCommonEntityData( void )
{
    SmsCommonContext_t  *smsCommonContext_p = ptrToSmsCommonContext();

    memset( &smsCommonContext_p->sca,       0, sizeof(smsCommonContext_p->sca) );
    memset( &smsCommonContext_p->vgTempSca, 0, sizeof(smsCommonContext_p->vgTempSca) );

    smsCommonContext_p->validityPeriodValue = VG_SMS_DEFAULT_VALIDITYPERIOD;
    memset( &smsCommonContext_p->validityPeriodAbsolute[0], 0, sizeof( SmsTimeStamp) );

    smsCommonContext_p->protocolId.protocolMeaning = VG_SMS_TEXTMODE_DEFAULT_PROTOCOLMEANING;
    smsCommonContext_p->protocolId.protocolId.data = VG_SMS_TEXTMODE_DEFAULT_PROTOCOLID;

    smsCommonContext_p->dataCodingScheme.msgCoding            = VG_SMS_TEXTMODE_DEFAULT_DCS_MSGCODING;
    smsCommonContext_p->dataCodingScheme.msgClass             = VG_SMS_TEXTMODE_DEFAULT_DCS_MSGCLASS;
    smsCommonContext_p->dataCodingScheme.compressedText       = VG_SMS_TEXTMODE_DEFAULT_DCS_COMPRESSEDTEXT;
    smsCommonContext_p->dataCodingScheme.msgWaitingIndPresent = VG_SMS_TEXTMODE_DEFAULT_DCS_MSGWAITINGINDPRESENT;
    smsCommonContext_p->dataCodingScheme.markedForAutomaticDeletion = VG_SMS_TEXTMODE_DEFAULT_DCS_MARKEDFORAUTOMATICDELETION;

    smsCommonContext_p->smSimStateInitialized      = FALSE;
    smsCommonContext_p->smSimState.requestStatus   = SM_REQ_SM_NOT_READY;
    smsCommonContext_p->actualSmsStatusSIMOverflow = VG_SIM_SMS_SPACE_AVAILABLE;
    smsCommonContext_p->unsolicitedChannelBusy     = FALSE;

    /* job115421: remove compile switch */
    smsCommonContext_p->firstOctet = VG_SMS_TEXTMODE_DEFAULT_FIRSTOCTET;

    /* Flag used to determine whether new SMS message needs acknowledging */
    smsCommonContext_p->msgToAck = FALSE;

    /* initialise SM capacity */
    smsCommonContext_p->smSimState.unreadRecords      = 0;
    smsCommonContext_p->smSimState.unreadMemRecords   = 0;
    smsCommonContext_p->smSimState.usedRecords        = 0;
    smsCommonContext_p->smSimState.numberOfSmsRecords = 0;
    smsCommonContext_p->smSimState.firstFreeRecord    = 0;

    /* initialise SR capacity SIM only */
    smsCommonContext_p->smSimState.usedSmsrRecords     = 0;
    smsCommonContext_p->smSimState.numberOfSmsrRecords = 0;

    /** initialise number of SMSP storage locations. */
    smsCommonContext_p->smSimState.numberOfSmspRecords = 0;;

    /********************************************************************
    ***********          USER CONFIGURABLE VALUES            ************
    ********************************************************************/

#if defined (FEA_PHONEBOOK)
    /* AlphaId lookup for phone numbers is enabled by default */
    smsCommonContext_p->enableAlphaId         = FALSE;
#endif /* FEA_PHONEBOOK */

    /* Extra information on certain commands/messages is disabled by default */
    smsCommonContext_p->enableExtraInfo       = FALSE;

    /* job116776 Adding Send More Messages functionality as defined in 27.007 */
    smsCommonContext_p->moreMsgsToSendMode    = 0;

    /* Default storage for newly received messages is SIM*/
    smsCommonContext_p->cpmsStore             = VG_CPMS_STORAGE_SM;

    smsCommonContext_p->printCmtOnlyOnMmi     = FALSE;
}

/* END OF FILE */

