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
 * File Description                                                      */
/** \file
 * Background Short Messages type definitions & constants
 * Include file which defines types used in the Sms / foreground layer
 * interface apexSM signals not used in any other signal
 **************************************************************************/

#ifndef ABSM_TYP_H
#define ABSM_TYP_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <applayer.h>
#include <sits_typ.h>
#include <sitl_typ.h>
#include <abst_typ.h>
#include <alsa_typ.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/
#define ABSM_SMSP_STORE_SIZE 4     /*Size of stored SMS parameter blocks*/

/* support SMS Class 0, 1, 2 and 3 */
#define NUM_SMS_CLASSES_SUPPORTED    4
/* Max number number of records in a concatenated SMS reported by send from SIM */
#define MAX_LENGTH_CONCATENATED_SMS  32
/* Maximum SIM file records we record, Int8 - 255 is max possible value*/
#define MAX_SMS_RECORD               SIM_MAX_FREE_SM_LIST
/*  */
#define SMS_BIT_MAP_SIZE             SIM_SMS_BIT_MAP_SIZE
/*Largest short msg ID that can be used by ABSM to send a SMS*/
#define MAX_OUTWARD_MSG_ID           7

#define ABSM_INVALID_PORT_NUM         0xffff  /* job 103293 */

#define MAX_CONCATENATED_SMS_MSG (SMS_MAX_MSG_LEN - 8)


/***************************************************************************
 * Typed Constants
 **************************************************************************/
typedef enum AbsmStateTag
{
  ABSM_INIT,        /*Initial state */
  ABSM_PARTIAL_INIT,
  ABSM_RETRIEVE,    /*Reading all parameters from NVRAM and SIM */
  ABSM_SEND,        /*ABSM is able to send SMS messages */
  ABSM_SEND_RECV,   /*ABSM can send or receive message */
  ABSM_READY        /*Full access to stored messages, sending receiving possible*/
} AbsmState;

/** The differents available operation on SMS storage*/
typedef enum AbsmOperationTypeTag
{
    /** Add a record in the storage*/
    ABSM_OP_ADD,
    /** Remove a record from the storage*/
    ABSM_OP_DEL
} AbsmOperationType;

/*CNMI values set up for routing mesage classes to terminal*/
typedef struct AbsmProfileTag
{
  Boolean   mtCnmiVal [NUM_SMS_CLASSES_SUPPORTED];
  Int8      dsCnmiVal;
  Int8      msgService;
  /** Whether the new received SMS should be stored in ME*/
  Boolean   storeInMe;
} AbsmProfile;

/** Memory type area type for message store */
typedef enum AbsmMemAreaTypeTag
{
  ABSM_STORAGE_NONE,                    /**< Not stored (only status report indication) */
  ABSM_STORAGE_SM,                      /**< SM storage */
  ABSM_STORAGE_ME,                      /**< ME storage */
  ABSM_SM_MEMOVER_STORAGE_ME,           /**< ME storage because SM memory is full */

} AbsmMemAreaType;

/** SMS/SMSP read/write mode, relative to specified record number */
typedef enum SmsSimAccessTypeTag
{
  SM_FIRST_UNREAD,                      /**< Read first unread SMS record */
  SM_NEXT_UNREAD,                       /**< Read next unread SMS record */
  SM_FIRST_READ,                        /**< Read first read SMS record */
  SM_NEXT_READ,                         /**< Read next read SMS record */
  SM_FIRST_ORIGINATED_NOT_SENT,         /**< Read first stored unsent SMS record */
  SM_NEXT_ORIGINATED_NOT_SENT,          /**< Read next stored unsent SMS record */
  SM_FIRST_ORIGINATED_SENT,             /**< Read first stored sent SMS record */
  SM_NEXT_ORIGINATED_SENT,              /**< Read next stored unsent SMS record */
  SM_FIRST_ANY,                         /**< Read first used SMS record */
  SM_NEXT_ANY,                          /**< Read next used SMS record */
  SM_PREVIEW_FIRST_ANY,                 /**< Read first SMS record but do not mark as read */
  SM_PREVIEW_NEXT_ANY,                  /**< Read next SMS record but do not mark as read */
  SM_RW_ABSOLUTE,                       /**< Read/Write specified SMS/SMSP record */
  SM_WRITE_FREE,                        /**< Write specified SMS/SMSP record */
  SM_PREVIEW_ONLY,                      /**< Read specified SMS record but do not mark as read */
  /* added for 0211-21790 */
  SM_FIRST_ORIGINATED,                  /**< Read first stored SMS record, sent or unsent */
  SM_NEXT_ORIGINATED,                   /**< Read next stored SMS record, sent or unsent */
  /*added for job 106290*/
  SM_PREVIEW_FIRST_UNREAD,              /**< Read first unread SMS record but do not mark as read */
  SM_PREVIEW_NEXT_UNREAD                /**< Read next unread SMS record but do not mark as read */
}
SmsSimAccessType;

/** Determines the outcome of the request */
typedef enum SmRequestStatusTag
{
  SM_REQ_OK,                 /**< Request successful */
  SM_REQ_SM_NOT_READY,       /**< Short message procedure is not ready */
  SM_REQ_SM_SEND_READY,      /**< Short message procedure ready to send */
  SM_REQ_SM_SNDRCV_READY,    /**< Short message procedure is ready to send and receive */
  SM_REQ_RECORD_NOT_FOUND,   /**< Requested record not found or invalid */
  SM_REQ_RECORD_EMPTY_FOUND, /**< Requested record found but empty */
  SM_REQ_SIM_ERROR,          /**< SIM has reported error */
  SM_REQ_PS_ERROR,           /**< Protocol Stack has reported error */
  SM_REQ_NO_MSG_ID,          /**< Out of message IDs */
  SM_REQ_INVALID_RECORD,     /**< Record number to delete or to send from SIM was 
                              * not the start of a concatenated SMS
                              */
  SM_REQ_CONCAT_LINK_ERROR,  /**< Out of sequence when storing a concatenated SMS 
                              * to SIM or space has already been allocated 
                              */
  SM_REQ_NOT_SUBMIT_RECORD,  /**< A record asked to be sent from SIM        
                              * was not of submit type                    
                              */
  SM_REQ_RECORD_BUSY,        /**< A transaction is already being performed on this record */
  SM_REQ_NRAM_ERROR,         /**< ANRM has reported error */
  SM_REQ_INVALID_COMMAND,    /**< Command too long etc 20460 */
  /** FDN failure */
  SM_REQ_FDN_FAILURE,        /*job100892*/
  SM_REQ_NO_FREE_RECORD,     /**< There are no more records available */
  /** Facility rejected */
  SM_REQ_FAC_REJECT,         /* GSM04.11 Annex E-2 value 29 (AT+CMS error) */
  /** Unidentified subscriber */
  SM_REQ_UNIDENT_SUBSC,      /* GSM04.11 Annex E-2 value 28 (AT+CMS error) */
  /** Requested facility not implemented */
  SM_REQ_FAC_NOT_IMPL,       /* GSM04.11 Annex E-2 value 69 (AT+CMS error) */
  /** Invalid message, unspecified */
  SM_REQ_INVALID_SM,         /* GSM04.11 Annex E-2 value 95 (AT+CMS error) */
  /** Unknown subscriber */
  SM_REQ_UNKNOWN_SUBSC,      /* GSM04.11 Annex E-2 value 30 (AT+CMS error) */
  /** Memory full */
  SM_REQ_MEM_FULL,           /* GSM07.05 3.2.5 value 322 (AT+CMS error) */
  /** Barred by SIM */
  SM_REQ_BARRED_BY_SIM,      /* call control on SIM  - barred by SIM  */
  /** Referenced SMS was not found  */
  SM_REQ_REF_SMS_NOT_FOUND,
  /** We don't reveive a response from ME storage*/
  SM_REQ_NO_ME_RESPONSE
}
SmRequestStatus;

/** SMS parameters find mode element.
 * For an SMSP_FIND_NEXT and SMSP_FIND_PREVIOUS requests, the returned 
 * parameters are those relative to the supplied index in the request. 
 * SMSP_FIND_FIRST will find the first set of parameters in the Background
 * Layer's list, and similarly SMSP_FIND_LAST will find the last ones.
 */
typedef enum SmspFindModeTag
{
  SMSP_FIND_FIRST,     /**< Return first SMS parameters in BL list */
  SMSP_FIND_LAST,      /**< Return last SMS parameters in BL list */
  SMSP_FIND_NEXT,      /**< Return SMS parameters after index in BL list*/
  SMSP_FIND_PREVIOUS,  /**< Return SMS parameters before index in BL list */
/*9902-3727 The find modes not currently implemeted, may be for the future
  SMSP_FIND_EXACT,          Return SMS parameters whose name matches alphaId
  SMSP_FIND_PARTIAL,        Return SMS parameters whose name partially matches alphaId*/
  SMSP_FIND_DEFAULT    /**< Return preferred SMSP parameters */
} SmspFindMode;

/** Describes the source of SMS parameters. */
typedef enum SmspEntryTypeTag
{
  SMSP_SIM_ENTRY,   /**< SMS parameters on the SIM */
  SMSP_MEM_ENTRY,   /**< SMS parameters in NVRAM */
  SMSP_ANY          /**< SIM or NVRAM SMS parameters */
} SmspEntryType;

typedef enum SmspEntryFile
{
  SM_SIM_SMSP_ENTRY,  /*SMS parameters on SIM */
  SM_NVRAM_SMSP_ENTRY /*SMS parameters in NVRAM */
} SmspEntryFile;

/* The User Datagram Header Format is described in GSM:0340 and WAP(WDP) 1.1 */

typedef enum SmsFixedAddressTypeTag
{
  SMS_8BIT_PORT_ADDRESS,
  SMS_16BIT_PORT_ADDRESS
} SmsFixedAddressType;

/***************************************************************************
 * Type Definitions
 **************************************************************************/
/** Message Waiting Indication Information element */
typedef struct SmsMwiCombinedTag
{
    /** Flag indicating the presence of UDH MWI information in SMS message. */
    Boolean     udhPresent;
    /** Flag indicating the presence of CPHS MWI information. */
    Boolean     cphsPresent;
    /** Flag indicating the presence of DCS MWI information. */
    Boolean     dcsPresent;
    /** Flag indicating the presence of PID MWI information. */
    Boolean     pidPresent;
    /** Is the message to be stored or discarded. */
    Boolean     storeMessage;    /* Defined in UDH, CPHS, DCS case */
    Boolean     indActive;       /**< Defined in UDH, CPHS, DCS case */
    /** MWI type. */
    SmsIndType  indType;         /* Defined in UDH, CPHS, DCS case */
    /** Number of messages waiting (This field is valid 
     * only when UDH is present).
     */
    Int8        numMsgsWaiting; 
    /* CPHS line 1 (Defined in CPHS case) */
    Boolean     cphsLine1;    
    /* CPHS line 2 (Defined in CPHS case) */
    Boolean     cphsLine2;      
}
SmsMwiCombined;


/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

#endif

/* END OF FILE */
