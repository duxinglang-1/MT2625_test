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
 *  File Description :
 *
 *      GPRS data structure typedefs for RLC/MAC messages.  Covers both
 *      downlink (decode) and uplink (encode) messages.
 *
 **************************************************************************/

#if !defined (GPRLCTYP_H)
#define       GPRLCTYP_H


/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <gpie_typ.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

#define OUT_OF_RANGE_INT8                  0xFF
#define INVALID_GAMMA                      OUT_OF_RANGE_INT8
#define INVALID_USF                        OUT_OF_RANGE_INT8

#define MAX_PSI_MSG_LST                     5

/* Assumes msg type field is always 6 bits: */
#define NUM_OF_MSG_TYPES                   64

#if defined (UPGRADE_EDGE)
#define LEN_EDGE_ACK_NACK_RBB              16
#define LEN_EDGE_ACK_NACK_URBB             64
#endif

/*
 * Max no of rejects in a Packet Access Reject msg:
 * 16 is the most that will fit in a single-block message.
 */
#define MAX_REJECTS                         16

/*
** GSM 04.60 states that up to 4 mobile stations can be addressed in a pkt paging message.
*/

#define MAX_REPEATED_PAGE_INFO              4

#if defined (UPGRADE_EDGE)
#define MAX_COMPACT_RED_MA_BITMAP_LEN       16 /* 04.60 ss 12.9, max compact reduced MA bitmap length */
/* UPGRADE_EDGE */
#endif


/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Types
 **************************************************************************/

typedef enum ControlAckFormatTag
{
    FORMAT_RLC_MAC                          = 0,
    FORMAT_11_BIT                           = 1,
    FORMAT_8_BIT                            = 2
}
ControlAckFormat;

typedef enum AccessTypeTag
{
    TWO_PHASE_ACCESS_RQ                     = 0,
    PAGE_RESPONSE                           = 1,
    CELL_UPDATE                             = 2,
    MM_PROCEDURE                            = 3,
    NO_ACCESS_TYPE                          = 0xFF
}
AccessType;

typedef enum TbfCauseTag
{
    TBF_NORMAL                              = 0,
    TBF_STATUS_UNSPECIFIED                  = 1,
    TBF_NON_DISTRIB_ERROR                   = 2,
    TBF_MESSAGE_ESCAPE_ERROR                = 3,
    TBF_MESSAGE_INCOMPATIBLE_WITH_STATE     = 4
}
/* Values in this enum must be equal to the corresponding values of the
 * 3-bit TBF_CAUSE field in the Packet Mobile TBF Status msg -
 * see 04.60 11.2.9c
 */
TbfCause;

typedef struct PsiMsgListTag
{
    DownlinkMessageType         psiMsgType;
    Boolean                     psiMsgReceived;
    Int8                        psixChangeMark;
    Boolean                     psixCountPresent;
    Int8                        psixCount;
    Int16                       psixInstanceBitmap;
    /*
     * The number of significant bits in psixInstanceBitmap is psixCount + 1;
     * the significant bits are assumed to be right-justified within this
     * Int16.  The ls bit refers to the PSI instance with INDEX = 0.
     */
}
PsiMsgList;

typedef struct UnknownPsiMsgListTag
{
    Int8                    unknownPsiMsgCount;     /* 0 if none received */
    Int8                    newerUnknownPsiMsgType; /* 0xFF if none newer */
    Int8                    olderUnknownPsiMsgType; /* 0xFF if none older */
}
UnknownPsiMsgList;

/** Reject information */
typedef struct RejectTag
{
    /** The address; the mobile station shall only accept the
     * first Reject struct addressed to it and ignore all other Reject structs
     */
    Address                     address;
    /** Indicates the cause for rejecting the packet access */
    PacketAccessRejectCause     rejectCause;
    /** Prohibit packet access in the cell after Packet Access
     * Reject message has been received
     */
    Int8                        t3172;
}
Reject;

typedef struct RepeatedPageInfoTag
{
    Boolean                     pageForRrConnEstab;
    MobileIdElement             pageId;
    Int8                        channelNeeded;
    Boolean                     emlppPresent;
    Int8                        emlpp;
}
RepeatedPageInfo;

typedef struct ChanQualReportTag
{
    Int8                        cValue;
    Int8                        rxQual;
    Int8                        signVar;
    Int8                        measChannelMap;
    Int8                        iLevel[ALL_TIMESLOTS];  /* 0xFF if absent */
}
ChanQualReport;

/** Generic message header, used for most Rlc/Mac messages */

typedef struct RlcMacMsgHeaderTag
{
    /** Controls the action of the mobile station belonging to the paging
     * subgroup corresponding to the paging subchannel
     */
    PageMode                    pageMode;
    /** Indicates if persistence is present */
    Boolean                     persistencePresent;
    /** The peristence level indicates the values of the access persistence level
     * P(i) for each Radio Priority i (i = 1 to 4) where Radio Priority 1 represents the
     * highest Radio Priority of an LLC PDU to be transmitted
     */
    Int8                        persistenceLevel[NUM_OF_RADIO_PRIORITIES];
    /** Address used */
    Address                     address;
}
RlcMacMsgHeader;


/** This message is sent on the PCCCH or PACCH by the network to the mobile
 * station to indicate that the network has rejected the MSs access request.
 * This message may contain fields addressing more than one mobile station.
 */
typedef struct PacketAccessRejectTag
{
    /** Controls the action of the mobile station belonging to the paging
     * subgroup corresponding to the paging subchannel
     */
    PageMode                    pageMode;
    /** The number of rejects */
    Int8                        numOfRejects;
    /** Reject information for each packet rejected */
    Reject                      reject[MAX_REJECTS];
}
PacketAccessReject;


/* Packet Control Ackowledgement (11.2.2):   */

typedef struct PacketControlAckTag
{
    ControlAckFormat            controlAckFormat;
    Int8                        controlAck;
    Int32                       tlli;
}
PacketControlAck;


/* Packet Cell Change Failure (11.2.3):   */

/*
 * Not defined here: uses the RcGrrCellChangeFailureReq
 * structure defined in rcgrrsig.h
 */


/* Packet Downlink Ack/Nack (11.2.6):   */
typedef struct PacketDnlinkAckNackTag
{
    Int8                        dnlinkTfi;
    AckNackDescription          ackNackDesc;
    ChanReqDescription          chanReqDescription;
    ChanQualReport              chanQualReport;
    Int8                        pfi; /* 0xFF not present */
}
PacketDnlinkAckNack;

/* Packet Dnlink Dummy Control Block (11.2.8): */

typedef struct PacketDnlinkDummyCtrlBlockTag
{
    PageMode                    pageMode;
    Boolean                     persistencePresent;
    Int8                        persistenceLevel[NUM_OF_RADIO_PRIORITIES];
}
PacketDnlinkDummyCtrlBlock;


/* Packet Uplink Dummy Control Block (11.2.8b): */

typedef struct PacketUplinkDummyCtrlBlockTag
{
    Int32                       tlli;
}
PacketUplinkDummyCtrlBlock;


/* Packet Measurement report (11.2.9):   */
/*
 * Not defined here: uses the RcGrrNcMeasReportReq &
 * RcGrrExtMeasReportReq structures defined in rcgrrsig.h
 */


/* Packet Mobile TBF Status (11.2.9c): */

typedef struct PacketMobileTbfStatusTag
{
    Address                     address;    /* uplink TFI or downlink TFI */
    TbfCause                    tbfCause;
    Boolean                     messageTypePresent;
    DownlinkMessageType         messageType;
}
PacketMobileTbfStatus;


/* Packet Paging Request (11.2.10): */
/* The complete message is decoded into the following structure: */

typedef struct PacketPagingRequestTag
{
    PageMode                    pageMode;
    Boolean                     persistencePresent;
    Int8                        persistenceLevel[NUM_OF_RADIO_PRIORITIES];
    Boolean                     nlnPresent;
    Int8                        notificationListNumber;
    Int8                        numOfRptPageInfo;
    RepeatedPageInfo            repeatedPageInfo[MAX_REPEATED_PAGE_INFO];
}
PacketPagingRequest;


/* Packet PDCH Release (11.2.11): */
/* The complete message is decoded into the following structure: */

typedef struct PacketPdchReleaseTag
{
    PageMode                    pageMode;
    Boolean                     timeslotsAvailablePresent;
    Int8                        timeslotsAvailable;
}
PacketPdchRelease;


/* Packet Polling Request (11.2.12): */
/* The complete message is decoded into the following structure: */

typedef struct PacketPollingRequestTag
{
    PageMode                    pageMode;
    Address                     address;
    Boolean                     ackIsRlcMacFormat;
}
PacketPollingRequest;

/*
 * Packet Power Control / Timing Advance (11.2.13):
 * 'Header info' in this msg is first decoded into a RlcMacMsgHeader struct;
 * if required, the body of the message is then decoded into the following:
 */
typedef struct PacketPwrCtrlTaTag
{
/** \assoc PRESENT \ref globalPwrCtrlParam */
    Boolean                     globalPwrCtrlParamPresent;
    GlobalPwrCtrlParam          globalPwrCtrlParam;
/** \assoc PRESENT \ref powerCtrl */
    Boolean                     powerCtrlPresent;
    PowerControlParams          powerCtrl;
    Boolean                     globalPktTaPresent;
    PacketTimingAdvance         uplinkTa;
    PacketTimingAdvance         dnlinkTa;
    Boolean                     packetExtTaPresent;
    Int8                        extendedTa;
}
PacketPwrCtrlTa;


/* Packet Queueing Notification (11.2.15): */
/* The complete message is decoded into the following structure: */
typedef struct PacketQueueingNotificationTag
{
    PageMode                    pageMode;
    Address                     address;
    Int16                       tqi;
}
/*
 * Notes: the address info in this message is always a Pkt Req Ref;
 * but we'll still use the generic Address structure, so we can use the
 * generic function GpDecAddressInfo to check and extract it.
 */
PacketQueueingNotification;


/* Packet Resource Request (11.2.16): */
typedef struct PacketResourceRequestTag
{
    AccessType                  accessType;      /* 0xFF if field absent */
    Address                     address;
    /* MsRadAccCapability and ChanReqDescription have been deleted from here as GRR already has a copy
     * of these parameters. Adding parameters to this structure may cause the stack to blow if not adjusted */
    Int8                        changeMark;      /* 0xFF if field absent */
    Int8                        cValue;
    Int8                        signVar;              /* 0xFF if field absent */
    Int8                        iLevel[ALL_TIMESLOTS]; /* 0xFF if absent */
    Int8                        numMsRadAccEncoded;
#if defined (UPGRADE_EDGE)
    /* Additional MS RAC Information available - Filled in by encode module */
    Boolean                     retransPrr;
    PdchStructure               pdchStructure;
    Boolean                     aracRequired;
    EgprsBepLinkQualMeas        bepLinkQualMeas;
    EgprsTsBepLinkQualMeas      tsBepLinkQualMeas;
#endif
    Boolean                     rel99Ntwk;
    Int8                        pfi; /* 0xFF not present */
}
PacketResourceRequest;


/* Packet PSI Status (11.2.17): */

/*
 * (Structures for this message may well need to be transferred to
 * rcgrrsig.h or gpgrrtyp.h in due course)
 */
typedef struct PacketPsiStatusTag
{
    Address                     address;        /* for Global TFI */
    Int8                        pbcchChangeMark;
    PsiMsgList                  psiMsgList[MAX_PSI_MSG_LST];
    Int8                        newestUnknownPsiMsgLink;
    UnknownPsiMsgList           unknownPsiMsgList[NUM_OF_MSG_TYPES];
}
/*
 * The encode utils assume that entries in the psiMsgList array are listed
 * in descending order of priority.  Encode may omit trailing entries from
 * the Packet PSI Status message if there is insufficient room in the message.
 * Priority order is currently defined by 04.60 to be:
 *
 *   PACKET_SYSTEM_INFORMATION_2 : highest priority = element 0
 *   PACKET_SYSTEM_INFORMATION_3
 *   PACKET_SYSTEM_INFORMATION_3BIS
 *   PACKET_SYSTEM_INFORMATION_4
 *   PACKET_SYSTEM_INFORMATION_5 : lowest priority = element 4
 *
 * The psiMsgReceived flag within the PsiMsgList struct must be used to
 * indicate which PSI msgs have actually been received.
 *
 * The unknownPsiMsgList is managed as a doubly-linked list.  The array is
 * indexed by message type (0-63); the list is linked in order of latest-
 * receipt of each unknown message type on the PBCCH.  This approach is
 * intended to allow easy maintenance of the list when a new unknown msg
 * is received, and to allow the encode utility to generate the Packet PSI
 * Status msg easily.
 */
PacketPsiStatus;

/* Packet TBF Release (11.2.26): */
/* The complete message is decoded into the following structure: */
typedef struct PacketTbfReleaseTag
{
    PageMode                    pageMode;
    Address                     address;
    Boolean                     releaseUplink;
    Boolean                     releaseDnlink;
    Int8                        tbfReleaseCause;
}
PacketTbfRelease;

#if defined (UPGRADE_EDGE)
#if defined (ENABLE_EDGE_UL_ACK_NACK_LOGGING)
typedef struct AckNackDescriptionUnionTag
#else
typedef union AckNackDescriptionUnionTag
#endif
{
    AckNackDescription          ackNackDesc;
    EgprsAckNackDescription     egprsAckNackDesc;
}
AckNackDescriptionUnion;
#endif

typedef struct PacketUplinkAckNackTag
{
    CodingScheme                codingScheme;
#if defined (UPGRADE_EDGE)
    AckNackDescriptionUnion     ackNackDescUnion;
#else
    AckNackDescription          ackNackDesc;
#endif
    Boolean                     tlliPresent;
    Int32                       contentionResolutionTlli;
    Boolean                     packetTaPresent;
    PacketTimingAdvance         packetTa;
    Boolean                     powerCtrlPresent;
    PowerControlParams          powerCtrl;
#if defined (ENABLE_GPRS_EXT_BITS)
    ExtBits                     ext;
#endif

#if defined (UPGRADE_EDGE)
    PdchStructure               pdchStructure;
    Boolean                     resegment;
    Boolean                     preEmptTrans;
    Boolean                     prrReTransReq;
    Boolean                     aracReTransReq;
#endif
    Boolean                     tbfEst;
    Boolean                     packetExtTaPresent;
    Int8                        extendedTa;
#if defined (INCLUDE_GPRS_FIXED_ALLOC_IN_L3)
    Boolean                     fixedAllocPresent;
    FixedAlloc                  fixedAlloc;
#endif
#if defined (UPGRADE_EDGE)
    Int8                        uncompRxBlkBmp[LEN_EDGE_ACK_NACK_URBB];
#endif
}
PacketUplinkAckNack;


/* Packet Uplink Assignment (11.2.29): */
typedef struct PacketUplinkAssignmentTag
{
    CodingScheme                codingScheme;
    CodingScheme                tlliScheme;
    PacketTimingAdvance         packetTa;
/** \assoc PRESENT \ref freqParam */
    Boolean                     freqParamPresent;
    FrequencyParameters         freqParam;

#if defined (UPGRADE_EDGE)
    PdchStructure               pdchStructure;
    Boolean                     contResTlliPresent;
    Int32                       contResTlli;

    Boolean                     resegment;
    Int8                        egprsWindowSize;
/** \assoc ARRAY \ref accessTechnologyRequest */
    Int8                        numAtr;
    AccessTechnology            accessTechnologyRequest[MAX_NUM_ACCESS_TECHNOLOGY];
    Boolean                     aracRetransReq;
    Int8                        bepPeriod2; /* 0xFF = not present */
#endif
/** \assoc PRESENT \ref extendedTa */
    Boolean                     packetExtTaPresent;
    Int8                        extendedTa;
/** \assoc UNION \ref alloc */
    AllocationType              allocType;
    AllocationUnion             alloc;
}
PacketUplinkAssignment;

/* Packet Timeslot Reconfigure (11.2.31): */
typedef struct PacketTimeslotRecfgTag
{
    CodingScheme                codingScheme;
    PacketTimingAdvance         uplinkTa;
    PacketTimingAdvance         dnlinkTa;
    Boolean                     rlcUnackMode;
    Boolean                     controlAck;
    Boolean                     dnlinkTfiPresent;
    Int8                        dnlinkTfi;
    Boolean                     uplinkTfiPresent;
    Int8                        uplinkTfi;
    Int8                        dlTimeslotAllocation;
    Boolean                     freqParamPresent;
    FrequencyParameters         freqParam;

#if defined (UPGRADE_EDGE)
    PdchStructure               pdchStructure;
    Boolean                     resegment;
    Int8                        dlEgprsWindowSize; /* 0xFF = not present */
    Int8                        ulEgprsWindowSize; /* 0xFF = not present */
    Int8                        linkQualMeasMode;
#endif
    Boolean                     packetExtTaPresent;
    Int8                        extendedTa;
/** \assoc UNION \ref alloc */
    AllocationType              allocType;
    AllocationUnion             alloc;
}
PacketTimeslotRecfg;

#endif
/* END OF FILE */
