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
 *      General Types used in GPRS
 **************************************************************************/

#ifndef GPGENTYP_H
#define GPGENTYP_H


/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <gp13_typ.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

#define NUM_TIMESLOTS           8

#define SEQUENCE_NUMBER_SPACE   128
#define WINDOW_SIZE             64

#if defined (UPGRADE_EDGE)

#define EGPRS_SEQUENCE_NUMBER_SPACE 2048

/*These defines are used to enable quick construction of the CPS field from MCS and PS's*/
#define MAC_CPS_MCS_8_BASE 0x0b  /* 0 1011 MCS_8, PS_1, PS_1*/
#define MAC_CPS_MCS_7_BASE 0x14  /* 1 0100 MCS_7, PS_1, PS_1*/
#define MAC_CPS_MCS_5_BASE 0x04  /*    100 MCS_5, PS_1*/
#define MAC_CPS_MCS_3_BASE 0x03  /*   0011 MCS_3, PS_1*/
#define MAC_CPS_MCS_2_BASE 0x09  /*   1001 MCS_2, PS_1*/
#define MAC_CPS_MCS_1_BASE 0x0b  /*   1011 MCS_1, PS_1*/

#endif

#define BITMAP_WINDOW_SIZE      WINDOW_SIZE/ALL_TIMESLOTS

#define RLC_MAC_PADDING_OCTET   0x2B    /* 0x0010 1011 04.60[11] */

#define DEFAULT_BS_CV_VALUE     15
#define INVALID_BS_CV_VALUE     0xFF

#define RLC_BLOCKS_IN_SHORT_ACCESS  8   /* number of blocks that can be sent */

#define FILL_FRAME              0x2b

#define DEFAULT_CONTROL_ACK     0x03    /* Packet Control Acknowledgement for*/
                                        /* non segmented control messages    */
                                        /* value '11' 04.60 sec 11.2.2       */

#define MAX_CVALUE              0x3f

#if defined (UPGRADE_EDGE)
#define LEN_OF_EGPRS_ACK_NACK_BITMAP        64
#endif

#define LEN_OF_ACK_NACK_BITMAP              8

#if defined (UPGRADE_EDGE)
#define LEN_OF_EGPRS_COMP_ACK_NACK_BITMAP        16
#define LEN_OF_EGPRS_UNCOMP_ACK_NACK_BITMAP      64 /* Max length of uncompressed bit map based on EGPRS window size of 512 */
#endif

/***************************************************************************
 * Typed Constants
 **************************************************************************/

typedef enum GprsLogicalChannelTag
{
    LC_PACCH,
    LC_PAGCH,
    LC_PBCCH,
    LC_PCCCH
}
GprsLogicalChannel;

typedef enum RlcModeTag
{
    RLC_ACKNOWLEDGED_MODE   = 0,
    RLC_UNACKNOWLEDGED_MODE = 1
}
RlcMode;

/** Release causes to/from Grr */
typedef enum RlcmacReleaseCauseTag
{
    /** Normal event */
    RLC_NORMAL_EVENT,

    /** Normal release */
    RLC_NORMAL_RELEASE,

    /** Abnormal release */
    RLC_ABNORMAL_RELEASE,

    
    /** A Packet UL ACK/NACK not received before T3182 timeout and the counter N3102 count
     * has not expired. GRR should send a channel request (PRACH) to re-establish the TBF 
     */
    RLC_T3182_EXP_N3102_COUNTDOWN_NOT_EXP,

    /** A Packet UL ACK/NACK not received before T3182 timeout and the counter N3102 count
     * has expired. GRR should perform cell reselection and re-establish the TBF 
     */
    RLC_T3182_EXP_N3102_COUNTDOWN_EXP,

    /** The number of blocks transmitted before a Packet UL Ack/Nack has been received to
     * complete contention resolution has exceeded the N3104 count. 
     */
    RLC_N3104_MAXED_CONTENTION_RES_FAILED,

    /** There have been no RLCMAC downlink data blocks received by MAC for the T3190 period */
    RLC_T3190_TIMED_OUT,

    /** Timer T3180 expires, the mobile station shall perform the abnormal release */
    RLC_T3180_TIMED_OUT,

    /** RLC/MAC control buffer is full */
    RLC_MAC_CTRL_BUFF_FULL,

    /* release causes from GRR */
    /** TLLI contention resolution has failed */
    RLC_TLLI_CONTENTION_RES_FAILED,
    /** 44.018 SI13 PRIORITY_ACCESS_THR; binary representation of a 
     * threshold to bar access to a cell according to the radio priority
     */
    RLC_RADIO_PRIORITY_BARRED,

    /** Access to the network is barred under the current circumstances */
    RLC_ACCESS_CLASS_BARRED,
    RLC_QUEUEING_FAILURE,

    /** Unable to send the PDU because of circuit switched operation*/
    RLC_CIRCUIT_SWITCH_OPERATION,
    /** Cell reselection is active, store the establishment until completion */
    RLC_CELL_RESELECTION,

    /** Cell not available */
    RLC_CELL_NOT_AVAILABLE,
    /** RLC T3142 still running */
    RLC_T3142_RUNNING,
    /** RAC has changed */
    RLC_RAC_HAS_CHANGED,
    /** For a frequency hopping channel the number of channels is zero */
    RLC_CELL_ALLOCATION_NOT_AVAILABLE,

    /** Packet Access Reject sent on the PCCCH or PACCH by the network to 
     * the mobile station to indicate that the network has rejected the MSs 
     * access request
     */
    RLC_PACKET_ACCESS_REJECT,

    /** RACH failure */
    RLC_RACH_FAIL,

    /** RLC T3172 is still running; during the time T3172 is running, 
     * the mobile station shall ignore all received PACKET PAGING 
     * REQUEST messages except paging request to trigger RR connection 
     * establishment
     */
    RLC_T3172_RUNNING,

    /** DL is active */
    RLC_DL_TBF_ACTIVE,

    /** Invalid service status */
    RLC_INVALID_SERVICE_STATUS,

    /** PRACH fail */
    RLC_PRACH_FAIL,

    /** RLC illegal dynamic allocation */
    RLC_ILLEGAL_ALLOCATION,


    RLC_WAITING_FOR_UPDATE,

    /** RLC bad timeslot allocation, for instance 
     * Number TS allocated = 0 || Number TS allocated > maximum slot supported 
     */
    RLC_BAD_TS_ALLOCATION,

    /** Bad frequency element has been received */
    RLC_BAD_FREQ_PARAM_ELEMENT,

    /** Frequency not implemented sent when mobile station is instructed 
     * to use a frequency that it is not capable of using 
     */
    RLC_FREQ_NOT_IMPLEMENTED,

    /** Establishment failure */
    RLC_ESTABLISHMENT_FAILURE,

    /** RLC T3192 is not running */
    RLC_T3192_NOT_RUNNING
}
RlcmacReleaseCause;

/** Uplink RLC/MAC control messages are received in the RLC/MAC control block format.
 * GPRS encode utils assume that values of this enum
 * match the values of the 6-bit MESSAGE_TYPE field as defined in
 * 04.60 v8.0.0 11.2.0.2
 */
typedef enum UplinkMessageTypeTag
{
    /** Packet Cell Change Failure message content */
    PACKET_CELL_CHANGE_FAILURE          = 0x00,  /* 00 0000  */
    /** Packet Control Acknowledgement message content  */
    PACKET_CONTROL_ACKNOWLEDGEMENT      = 0x01,  /* 00 0001  */
    /** Packet Downlink Ack/Nack message content */
    PACKET_DOWNLINK_ACK_NACK            = 0x02,  /* 00 0010  */
    /** Packet Uplink Dummy Control Block message content */
    PACKET_UPLINK_DUMMY_CONTROL_BLOCK   = 0x03,  /* 00 0011  */
    /** Packet Measurement Report message content */
    PACKET_MEASUREMENT_REPORT           = 0x04,  /* 00 0100  */
    /** Packet Resource Request message content */
    PACKET_RESOURCE_REQUEST             = 0x05,  /* 00 0101  */
    /** Packet Mobile TBF Status message content */
    PACKET_MOBILE_TBF_STATUS            = 0x06,  /* 00 0110  */
    /** Packet PSI Status message content */
    PACKET_PSI_STATUS                   = 0x07,  /* 00 0111  */
    /** Packet Enhanced Measurement Report message content  */
    PACKET_ENHANCED_MEAS_REPORT         = 0x0A,  /* 00 1010  */
    /** Packet Pause message content */
    PACKET_PAUSE                        = 0x09,  /* 00 1001  */
#if defined (UPGRADE_EDGE)
    /** EGPRS Packet Downlink Ack/Nack message content */
    EGPRS_PACKET_DOWNLINK_ACK_MESSAGE   = 0x08,  /* 00 1000  */
    /** Additional MS Radio Access Capabilities message content */
    ADDITIONAL_MS_RADIO_CAPABILITIES    = 0x0B,  /* 00 1011  */
#endif
    /** Packet change notification */
    PACKET_CELL_CHANGE_NOTIFICATION     = 0x0C,  /* 00 1100  */
    /** Packet System Information Status */
    PACKET_SI_STATUS                    = 0x0D,  /* 00 1101  */
    /** Invalid uplink packet message */
    PACKET_INVALID_UPLINK_MESSAGE       = 0xFF
}
UplinkMessageType;

/***************************************************************************
 * Types
 **************************************************************************/
/** The Ack/Nack Description information element contains the RLC parameters 
 * used to acknowledge or negatively acknowledge a group of RLC data blocks.
 */
typedef struct AckNackDescriptionTag
{
    /** Indicates whether the entire TBF is being acknowledged. If the entire 
     * TBF is being acknowledged, the SSN and RBB fields contain no information 
     * and shall be ignored.<UL>
     * <LI> 0   retransmission are requested and the TBF is incomplete
     * <LI> 1   no retransmissions are requested and this message indicates 
     * acknowledgement of all RLC data in the TBF
     *</UL>
     */
    Boolean                     finalAck;
#if defined (UPGRADE_EDGE)
    /** SSN contains the value of V(R) when this information element 
     * was transmitted. This field is encoded as the binary representation of V(R).
     * 
     * Range 0 to 127
     */
    Int16                       startingSequenceNumber;
    /** Length of the RBB */
    Int16                       rbbLength;  
    /** The RBB is a bitmap representing Block Sequence Numbers. 
     * The bitmap is indexed relative to SSN as follows:<UL>
     * <LI> BSN = (SSN - bit_number) modulo 128,    for bit_number = 1 to 64.
     * <LI> The BSN values represented range from (SSN - 1) mod 128 to (SSN - 64) mod 128.
     *</UL>
     * The value of each bit is encoded as:<UL>
     * <LI> 0   Negative acknowledgement of the RLC data block with BSN = (SSN - bit_number) mod 128
     * <LI> 1   Positive acknowledgement of the RLC data block with BSN = (SSN - bit_number) mod 128
     * </UL>
     */
    Int8                        receivedBlockBitmap[LEN_OF_EGPRS_ACK_NACK_BITMAP];
    /** Is SSN at the start of the receive window*/
    Boolean                     bow;  
    /** The RBB includes the end of the receive window in the network, ie
     * includes V(R)-1
     */
    Boolean                     eow;        
#else
    /** SSN contains the value of V(R) when this information element 
     * was transmitted. This field is encoded as the binary representation of V(R).
     * 
     * Range 0 to 127
     */
    Int8                        startingSequenceNumber;
    /** The RBB is a bitmap representing Block Sequence Numbers. 
     * The bitmap is indexed relative to SSN as follows:<UL>
     * <LI> BSN = (SSN - bit_number) modulo 128,    for bit_number = 1 to 64.
     * <LI> The BSN values represented range from (SSN - 1) mod 128 to (SSN - 64) mod 128.
     *</UL>
     * The value of each bit is encoded as:<UL>
     * <LI> 0   Negative acknowledgement of the RLC data block with BSN = (SSN - bit_number) mod 128
     * <LI> 1   Positive acknowledgement of the RLC data block with BSN = (SSN - bit_number) mod 128
     * </UL>
     */
    Int8                        receivedBlockBitmap[LEN_OF_ACK_NACK_BITMAP];
#endif
}
AckNackDescription;

#if defined (UPGRADE_EDGE)
/** Specifies the status of BEP link measurement used */
typedef enum BepLinkQualMeasStatusTag
{
    /** Link quality measurement is not valid */
    NO_VALID_LINK_QUAL_MEAS     = 0,
    /** Link quality measurement using GMSK_MEAN_BEP/GMSK_CV_BEP is valid */
    VALID_GMSK                  = 0x01,
    /** Link quality measurement using 8PSK_MEAN_BEP/8PSK_CV_BEP is valid */
    VALID_EIGHT_PSK             = 0x02,
    /** Link quality measurement using GMSK/8PSK is valid (VALID_GMSK + VALID_EIGHT_PSK) */
    VALID_GMSK_AND_EIGHT_PSK    = 0x03 
}
BepLinkQualMeasStatus;

/** EGPRS BEP Link Quality Measurements (12.5.3) */
typedef enum BepMeasTypeTag
{
    /** The mean bit error probability value calculated on 
     * timeslots 0 through 7 for 8PSK modulation 
     */
    BEP_MEAS_8PSK = 0,
    /** The mean bit error probability 
     * value calculated on timeslots 0 through 7 for GMSK modulation
     */
    BEP_MEAS_GMSK = 1
}
BepMeasType;

/** The EGPRS Bit Error Probability (BEP)Link Quality measurements IE */
typedef struct EgprsBepLinkQualMeasTag
{
    /** Specifies the status of BEP link quality measurement */
    BepLinkQualMeasStatus  status;
    /** This field contains the mean value of the Bit Error Probability of 
     * the channel averaged over all time slots in the TBF for GMSK, refer 
     * to 3GPP TS 05.08
     */
    Int8                   gmskMeanBep;
    /** This field contains the variation co-efficient for the Bit Error 
     * Probability averaged over all time slots of the TBF for GMSK, refer 
     * to 3GPP TS 05.08
     */
    Int8                   gmskCvBep;
    /** This field contains the mean value of the Bit Error Probability of the 
     * channel averaged over all time slots in the TBF for 8 PSK, refer to 3GPP TS 05.08
     */
    Int8                   eightPskMeanBep;
    /** This field contains the variation co-efficient for the Bit Error Probability 
     * averaged over all time slots of the TBF for 8 PSK, refer to 3GPP TS 05.08
     */
    Int8                   eightPskCvBep;
}
EgprsBepLinkQualMeas;

/** EGPRS Timeslot Link Quality Measurements (12.5.4) */
typedef struct EgprsTsBepLinkQualMeasTag
{
    Int8                        validBepTsMeas;             /**< Bit field; Bit 0 == TS7...bit 7 == TS0 */
    BepMeasType                 bepMeasType;                /**< Specifies if GMSK or 8PSK */
    Int8                        bepMeas[ALL_TIMESLOTS];     /**< Can contain either 8PSK or GMSK measurements */
}
EgprsTsBepLinkQualMeas;

/* EGPRS Ack/Nack Description (12.3.1) */
typedef struct EgprsAckNackDescriptionTag
{
    Boolean                     finalAck;
    Boolean                     beginningOfWindow;
    Boolean                     endOfWindow;
    Int16                       startingSequenceNumber;
    Int8                        compBmpLength;
    Boolean                     compBmpStartingColorCode;
    Int8                        compRxBlkBmp [LEN_OF_EGPRS_COMP_ACK_NACK_BITMAP]; /* Max 127 bits */
    Int16                       uncompBmpLength;
    Int8                        uncompRxBlkBmp [LEN_OF_EGPRS_UNCOMP_ACK_NACK_BITMAP]; /* Max 328 bits */
}
EgprsAckNackDescription;
#endif /* UPGRADE_EDGE */

/***************************************************************************
 * Function Prototypes
 **************************************************************************/

/*
 * Arithmetic in modulo 128
 */
/* Add ADD to BSN in modulo 128 */
#define M_MOD128_ADD( BSN, ADD)    (((BSN)  + (ADD)) & 0x7F)
/* Substract SUB from BSN in modulo 128 */
#define M_MOD128_SUB( BSN, SUB)    (((BSN) >= (SUB)) ?       ((BSN) - (SUB)) : \
                                                  (0x80 + (BSN) - (SUB)))
/* Increment BSN by INC in modulo 128 */
#define M_MOD128_INC( BSN, INC)    BSN = M_MOD128_ADD (BSN, INC)
/* Decrement BSN by DEC in modulo 128 */
#define M_MOD128_DEC( BSN, DEC)    BSN = M_MOD128_SUB (BSN, DEC)
/* Compare two BSNs in modulo 128 */
extern SignedInt16 MacMod128Cmp (Int16 n1, Int16 n2);

#if defined (UPGRADE_EDGE)
/* Add ADD to BSN in modulo 2048 */
#define M_MOD2048_ADD( BSN, ADD)    (((BSN)  + (ADD)) & (EGPRS_SEQUENCE_NUMBER_SPACE - 1))
/* Increment BSN by INC in modulo 128 */
#define M_MOD2048_INC( BSN, INC)    BSN = M_MOD2048_ADD (BSN, INC)
/* Substract SUB from BSN in modulo 128 */
#define M_MOD2048_SUB( BSN, SUB)    (((BSN) >= (SUB)) ?       ((BSN) - (SUB)) : \
                                                  (0x800 + (BSN) - (SUB)))

/* Compare two BSNs in modulo 2048 */
extern SignedInt16 MacMod2048Cmp (Int16 n1, Int16 n2);

#endif /* UPGRADE_EDGE */


#endif


/* END OF FILE */

