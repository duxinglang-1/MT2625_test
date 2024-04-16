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
 * File Description:                                                      */
/** \file
 *    Definitions for various LLC interfaces.
 **************************************************************************/

#ifndef GPLLC_H
#define GPLLC_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <ki_sigbuf.h>

#if defined (DEVELOPMENT_VERSION  ) /* During development    */
#include <llheader.h>       /* llc header test trace */
#endif


/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/* Max length of LLC header (Address + Control fields) */
#define MIN_LL_HEADER_LEN        (1 + 1)  /* U frame */
#define MAX_LL_HEADER_LEN        (1 + 36) /* I frame SACK format */

#define LLC_SAPI1_MIN_N201_U      400
#define LLC_SAPI7_MIN_N201_U      270
#define LLC_MIN_N201              140
#define LLC_MAX_N201             1520

#define LLC_SAPI1_DEFAULT_N201_U  400 /* GMM */
#define LLC_SAPI7_DEFAULT_N201_U  270 /* SMS */
#define LLC_USERS_DEFAULT_N201_U  500 /* User Data 3, 5, 9 and 11 */
#define LLC_USERS_DEFAULT_N201_I 1503 /* User Data 3, 5, 9 and 11 */

#define LLC_FCS_LEN                 3

#define MAX_LL_FRAME_OFFSET       128 /* Space for RFC1144 header */
/* Max length of LLC frame    (Header + Information + Fcs fields) */
#define MAX_LL_FRAME_LEN   (MAX_LL_HEADER_LEN + LLC_MAX_N201 + LLC_FCS_LEN)

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/** TriggerCause exchanged between GMM, LLC and RLC/MAC;
 * Only Page Response and Cell Update are applicable to LlgmmTriggerReq
 * whereas all the values are used as TBF establish causes in RLC/MAC
 */
typedef enum TriggerCauseTag
{
    /* Causes in LlgmmTriggerReq */
    /** Trigger from GMM (Paging Response) */
    GPRS_PAGE_RESPONSE,  
    /** Trigger from GMM     (Cell Update) */
    GPRS_CELL_UPDATE,    
    /** CR A142r2: (Optimized Cell Update) */
    GPRS_CELL_NOTIFICATION,  

    /* Causes according to origin of Llc[Unit]DataReq              */
    /** GMM request PDUs */
    GPRS_GMM_REQ_PDU,
    /** GMM response PDUs */
    GPRS_GMM_RSP_PDU,
    /** GMM complete PDUs */
    GPRS_GMM_COMP_PDU,
    /** GMM status PDUs */
    GPRS_GMM_STAT_PDU,
    /** GMM accept PDUs */
    GPRS_GMM_ACC_PDU,
    /** SM PDUs */
    GPRS_SM_PDU,
    /** LLC-PDU from LLC      (Signalling) */
    GPRS_LLC_PROCEDURE,   
    /** User data from SNDCP  (Data)       */
    GPRS_USER_DATA,
    /** Dummy LLC PDU from RD              */
    GPRS_DUMMY_PDU           
}
TriggerCause;



/** Logical Link Release Cause exchanged between LLC, SNDCP and SM */
typedef enum LlcReleaseCauseTag
{
    /** DM received */
    LLC_CAUSE_DM_RECEIVED          = 0,
    /** No peer response */
    LLC_CAUSE_NO_PEER_RESPONSE     = 1,
    /** Invalid XID response */
    LLC_CAUSE_INVALID_XID_RESPONSE = 2,
    /** Normal release */
    LLC_CAUSE_NORMAL_RELEASE       = 3,
    /** Invalid XID command */
    LLC_CAUSE_INVALID_XID_COMMAND  = 4 /* A cause raised by SNDCP */
}
LlcReleaseCause;


/** Indicates how the memory should be handled and freed by the receiving task. 
 * DataUsage indicates if receiving task is responsible for de-allocation
 * and the source of the dynamic memory block allocated for the frame data.
 */
typedef enum DataUsageTag
{
    /** Data must be preserved (for instance segmented, constant or static) */
    DU_PRESERVED, 
    /** Data is from Kernel */
    DU_KERNEL, 
    /** Data is from traffic memory manager */
    DU_TMM        
}
DataUsage;


/***************************************************************************
 * Types
 **************************************************************************/

/** This structure contains a LLC frame for both Downlink and Uplink directions.
 * 
 * This is the basic unit of data handled by RLC/MAC at the GRR-SAP interface.
 * The frame consists of header, data and Frame Check Sequence (FCS).
 */
typedef struct LlPduTag
{
    /** the number of bytes of memory in data before
     * the actual LLC frame data appears. The offset allows in-place
     * TCP/IP header decompression to be done in SNDCP (downlink direction).
     * The offset is not included in the data length.
     */
    Int16       offset;
    /** Indicates how many bytes are relevant in the data field */
    Int16       length;
    /** Indicates if receiving task is responsible for de-allocation
     * and the source of the dynamic memory block allocated for the frame data
     */
    DataUsage   dataUsage;
#if defined (DEVELOPMENT_VERSION              ) /* During development:              */
    /** LLC header test trace          */
    LlcHeader   header;                 
#endif
#if defined (DATA_IN_SIGNAL)
    /** For showing only relevant data 
\assoc ARRAY \ref data */
    Int16       lengthPlusOffset;    
    /** An array of bytes containing an LL-PDU (LLC frame)
     * consisting of LLC Header, Information and Frame Check Sequence (FCS) fields
     */
    Int8        data[MAX_LL_FRAME_OFFSET +
                     MAX_LL_FRAME_LEN]; /* - Data in the signal body        */
#else
    /** Points to LLC PDU */
    Int8       *data;
#endif
}
LlPdu;



/*
The (private) LLC variables embedded in LlcDataReq and LlcDataInd signals.

The next field:
A pointer to the next frame in the LLC transmit/receive window
implemented as single linked list of signals.
This field must be the very first field of the signal!

The state field:
State of the frame in transmit/receive window.

The ns field (Send sequence number):
At the time that an in-sequence I frame is designated for transmission,
the value of N(S) is set equal to the value of the send state variable V(S).

The tsn field  (transmission sequence number):
Each LLC stores the history of the transmitted I frames, i.e., the LLE will
remember the I-frame transmission sequence. The history is used to decide
which I frames to retransmit. Due to retransmission, the history is not
necessarily an in-order sequence.
The tsn is updated with a new highest number on each (re-)transmission.
An unacknowledged I frame that was (re-)transmitted prior to an acknowledged
I frame (ie tsn1 < tsn2) will be considered lost and marked for retransmission.

The rc field (retransmission count variable):
The LLE maintains an internal retransmission count variable
for each transmitted I frame.
The LLE increments by 1 the RC of retransmitted I frame
- associated with T201 on timer T201 expiry (but not on peer receiver busy)
- on receipt of acknowledgement (after analysing transmission sequence history)
*/

/* A frame within the transmit window is either: */
typedef enum LlcFrameTxStateTag
{                          /* The frame has been... */
    FRAME_NOT_TRANSMITTED, /* not yet been transmitted */
    FRAME_TRANSMITTED,     /* (re-)transmitted, not known if received by peer */
    FRAME_ACKNOWLEDGED,    /* acknowledged by the peer LLE */
    FRAME_RETRANSMIT       /* marked for retransmission */
}
LlcFrameTxState;

/* A frame within the receive window is either: */
typedef enum LlcFrameRxStateTag
{
    FRAME_RECEIVED,        /* The frame has been correctly received */
    FRAME_NOT_RECEIVED     /* The frame has not been correctly received */
}
LlcFrameRxState;


typedef struct LlcFrameTxVariableTag
{
    SignalBuffer    next;  /* Next frame in the LLC transmit window */
    LlcFrameTxState state; /* State of the frame in transmit window */
    Int16           ns;    /* Send sequence number N(S), N(S)=V(S)  */
    Int16           tsn;   /* Transmission sequence number (The tx history) */
    Int8            rc;    /* Retransmission count variable for the I frame */
}
LlcFrameTxVariable;

typedef struct LlcFrameRxVariableTag
{
    SignalBuffer    next;  /* Next frame in the LLC transmit window */
    /* LlcFrameRxState state;  State of the frame in receive window */
    Int16           ns;    /* Send sequence number N(S), LLC header */
}
LlcFrameRxVariable;


/***************************************************************************
 * Functions
 **************************************************************************/

#endif

/* END OF FILE */
