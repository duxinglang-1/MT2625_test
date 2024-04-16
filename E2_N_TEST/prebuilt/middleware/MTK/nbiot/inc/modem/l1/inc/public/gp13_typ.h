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
/** \file
 *  Types used in GPRS Layer 1, MAC and Layer 3
 **************************************************************************/

#ifndef GP13_TYP_H
#define GP13_TYP_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

#define ALL_TIMESLOTS           8
#define MAX_SUPPORTED_TIMESLOTS 4

#define MAC_HEADER_SIZE         1

/*
** number of octects required for RLC/MAC data
*/
#define SIZE_OF_CS_1            (22 + MAC_HEADER_SIZE)
#define SIZE_OF_CS_2            (32 + MAC_HEADER_SIZE)
#define SIZE_OF_CS_3            (38 + MAC_HEADER_SIZE)
#define SIZE_OF_CS_4            (52 + MAC_HEADER_SIZE)

#if defined (UPGRADE_EDGE)
/*
** The following MCS sizes require one extra byte to store two RLC/MAC flag bits.
*/
#define SPACE_FOR_EXTRA_RLC_MAC_FLAGS (1)

#define SIZE_OF_MCS_1             (22 + SPACE_FOR_EXTRA_RLC_MAC_FLAGS)
#define SIZE_OF_MCS_2             (28 + SPACE_FOR_EXTRA_RLC_MAC_FLAGS)
#define SIZE_OF_MCS_3             (37 + SPACE_FOR_EXTRA_RLC_MAC_FLAGS)
#define SIZE_OF_MCS_4             (44 + SPACE_FOR_EXTRA_RLC_MAC_FLAGS)
#define SIZE_OF_MCS_5             (56 + SPACE_FOR_EXTRA_RLC_MAC_FLAGS)
#define SIZE_OF_MCS_6             (74 + SPACE_FOR_EXTRA_RLC_MAC_FLAGS)
#define SIZE_OF_MCS_7             (56 + SPACE_FOR_EXTRA_RLC_MAC_FLAGS)
#define SIZE_OF_MCS_8             (68 + SPACE_FOR_EXTRA_RLC_MAC_FLAGS)
#define SIZE_OF_MCS_9             (74 + SPACE_FOR_EXTRA_RLC_MAC_FLAGS)

/*
** The sizes, in octects, of the EDGE RLC?MAC headers for uplink.
*/
#define SIZE_OF_MCS_1_HDR_UL      (4)
#define SIZE_OF_MCS_2_HDR_UL      (4)
#define SIZE_OF_MCS_3_HDR_UL      (4)
#define SIZE_OF_MCS_4_HDR_UL      (4)
#define SIZE_OF_MCS_5_HDR_UL      (5)
#define SIZE_OF_MCS_6_HDR_UL      (5)
#define SIZE_OF_MCS_7_HDR_UL      (6)
#define SIZE_OF_MCS_8_HDR_UL      (6)
#define SIZE_OF_MCS_9_HDR_UL      (6)

/*
** the number of bytes required for soft decision buffers
*/
#define SIZE_OF_SDS_MCS_1         (294)
#define SIZE_OF_SDS_MCS_2         (366)
#define SIZE_OF_SDS_MCS_3         (474)
#define SIZE_OF_SDS_MCS_4         (558)
#define SIZE_OF_SDS_MCS_5         (702)
#define SIZE_OF_SDS_MCS_6         (918)
#define SIZE_OF_SDS_MCS_7         (702)
#define SIZE_OF_SDS_MCS_8         (846)
#define SIZE_OF_SDS_MCS_9         (918)

#define MAX_SIZE_MCS_RX_PAYLOAD_DATA    (SIZE_OF_MCS_9)
#define MAX_SIZE_MCS_RX_PAYLOAD_HDR     (SIZE_OF_CS_1)
#define MAX_SIZE_MCS_SDS_BUFFER         (SIZE_OF_SDS_MCS_9)
#define MAX_SIZE_MCS_TX_PAYLOAD         (SIZE_OF_MCS_9_HDR_UL + (2 * (SIZE_OF_MCS_9)))

/*
** these defines are used to loacte the positions of hdr, data1 and data2 within the
** tx payload buffer, and to reduce the number of bytes required in memcpys.
*/
#define START_OF_MCS_TX_HDR             (0)
#define START_OF_MCS_TX_DATA_1          (SIZE_OF_MCS_9_HDR_UL)
#define START_OF_MCS_TX_DATA_2          (SIZE_OF_MCS_9_HDR_UL + SIZE_OF_MCS_9)
#define SIZE_OF_MCS_HDR                 (SIZE_OF_MCS_9_HDR_UL)
#define SIZE_OF_MCS_SINGLE_DATA         (SIZE_OF_MCS_9_HDR_UL + SIZE_OF_MCS_6)
#define SIZE_OF_MCS_TWIN_DATA           (MAX_SIZE_MCS_TX_PAYLOAD)

/* endif UPGRADE_EDGE */
#endif

/*
** number of octects required for RLC/MAC data including spare bits as
** defined in GSM 04.60 section 10.
*/
#define SIZE_OF_CS_1_WITH_SPARE_BITS  (SIZE_OF_CS_1)      /* CS_1 has no spare bits */
#define SIZE_OF_CS_2_WITH_SPARE_BITS  (SIZE_OF_CS_2 + 1)  /* CS_2 requires 7 bits = 7/8 of one byte */
#define SIZE_OF_CS_3_WITH_SPARE_BITS  (SIZE_OF_CS_3 + 1)  /* CS_3 requires 3 bits = 3/8 of one byte */
#define SIZE_OF_CS_4_WITH_SPARE_BITS  (SIZE_OF_CS_4 + 1)  /* CS_4 requires 7 bits = 7/8 of one byte */

#define SIZE_OF_CS_RACH_8       1
#define SIZE_OF_CS_RACH_11      2

/* INVALID_FRAME_NUMBER
**
** Defines an invalid or not present frame number.
*/
#define INVALID_FRAME_NUMBER        0xFFFFFFFF
#define INVALID_FRAME_NUMBER_INT16  0xFFFF

/***************************************************************************
 * Types - enums etc
 **************************************************************************/

#if defined (UPGRADE_EDGE)
typedef enum PdchStructureTag
{
    PDCH_IS_GPRS       = 0,
    PDCH_IS_EDGE_GPRS  = 1
}
PdchStructure;

typedef enum LinkQualMeasModeTag
{
    NO_LINK_QUAL_MEASUREMENTS  = 0,
    LINK_QUAL_INT_MEAS_ONLY    = 1,
    LINK_QUAL_BEP_MEAS_ONLY    = 2,
    LINK_QUAL_INT_AND_BEP_MEAS = 3,
    LAST_LINK_QUAL_MEAS
}
LinkQualMeasMode;
#endif

/** \addtogroup SigGL1
 * @{
 */

/** Ciphering Algorithm exchanged between GMM, LLC and Cipher Engine.
 *  Do not change the assignments here since they are used as array indexes.
 *  Used in \ref Gl1CipherDataReq, as well as internaly in L1, MM, LLC */

typedef enum GprsCipheringAlgorithmTag
{
    GPRS_CIPHER_NONE  = 0,  /**< Ciphering not used */
    GPRS_CIPHER_GEA_1 = 1,  /**< GPRS Encryption Algorithm GEA/1 */
    GPRS_CIPHER_GEA_2 = 2,  /**< GPRS Encryption Algorithm GEA/2 */
    GPRS_CIPHER_GEA_3 = 3,  /**< GPRS Encryption Algorithm GEA/3 */
    GPRS_CIPHER_GEA_4 = 4,  /**< GPRS Encryption Algorithm GEA/4 */
    GPRS_CIPHER_GEA_5 = 5,  /**< GPRS Encryption Algorithm GEA/5 */
    GPRS_CIPHER_GEA_6 = 6,  /**< GPRS Encryption Algorithm GEA/6 */
    GPRS_CIPHER_GEA_7 = 7   /**< GPRS Encryption Algorithm GEA/7 */
}
GprsCipheringAlgorithm;

/** @} */ /* End of SigGL1 group */

/** Specifies which coding scheme should be used for data block */
typedef enum CodingSchemeTag
{   /* These values are used in the GRR encode module - DO NOT CHANGE THE VALUES */

    /** A Convolutional Half-Rate Code scheme */
    CS_1                    = 0,
    /** A Convolutional two-third-Rate Code scheme */
    CS_2                    = 1,
    /** A Convolutional three-fourth-Rate Code scheme */
    CS_3                    = 2,
    /** Uncoded coding scheme */
    CS_4                    = 3,
    CS_RACH_8               = 4,
    CS_RACH_11              = 5,

#if defined (UPGRADE_EDGE)
    /** The message delivered to the encoder has a fixed size of 209 information bits  
    * encoded with the 1/3 rate convolutional mother code (GMSK modulation)
    */
    MCS_1                   = 6,
    /** The message delivered to the encoder has a fixed size of 257 information bits
     * encoded with the 1/3 rate convolutional mother code (GMSK modulation)
     */
    MCS_2                   = 7,
    /** The message delivered to the encoder has a fixed size of 329 information bits 
     * encoded with the 1/3 rate convolutional mother code (GMSK modulation)
     */
    MCS_3                   = 8,
    /** The message delivered to the encoder has a fixed size of 385 information bits 
     * encoded with the 1/3 rate convolutional mother code (GMSK modulation)
     */
    MCS_4                   = 9,
    /** The message delivered to the encoder has a fixed size of 478 information bits 
     * encoded with the 1/3 rate convolutional mother code (8PSK modulation )
     */
    MCS_5                   = 10,
    /** The message delivered to the encoder has a fixed size of 622 information bits 
     * encoded with the 1/3 rate convolutional mother code (8PSK modulation )
     */
    MCS_6                   = 11,
    /** The message delivered to the encoder has a fixed size of 940 information bits
     * encoded with the 1/3 rate convolutional mother code (8PSK modulation )
     */
    MCS_7                   = 12,
    /** The message delivered to the encoder has a fixed size of 1132 information bits 
     * encoded with the 1/3 rate convolutional mother code (8PSK modulation )
     */
    MCS_8                   = 13,
    /** The message delivered to the encoder has a fixed size of 1228 information bits 
     * encoded with the 1/3 rate convolutional mother code (8PSK modulation )
     */
    MCS_9                   = 14,
    /** MCS 5 or MCS 7 */
    MCS_5_7                 = 15,
    /** MCS 6 or MCS 9 */
    MCS_6_9                 = 16,
/* endif UPGRADE_EDGE */
#endif
    /** The number of coding schemes */
    NUM_CODING_SCHEMES,
    /** Invalid coding scheme */
    INVALID_CODING_SCHEME   = 0xFF
}
CodingScheme;

#if defined (UPGRADE_EDGE)
/** Specifies which puncturing scheme should be used for RLC block transmissions */
typedef enum PuncturingSchemeTag
{
    /** Puncturing scheme of first data block */
    PS_1 = 0,
    /** Puncturing scheme of second data block */
    PS_2,
    /** Puncturing scheme 3 */
    PS_3,
    /** The number of puncturing schems */
    NUM_EGPRS_PUNCTURING_SCHEMES,
    /** Invalid puncturing scheme */
    INVALID_PUNCTURING_SCHEME = 0xFF
} PuncturingScheme;

typedef enum EgprsMacHeaderTypeTag
{
    MAC_HEADER_TYPE_1,
    MAC_HEADER_TYPE_2,
    MAC_HEADER_TYPE_3
} EgprsMacHeaderType;
/* endif UPGRADE_EDGE */
#endif

/** Downlink RLC/MAC control messages are received in RLC/MAC control block format.
 * The different types of messages are distinguished by the MESSAGE_TYPE field.
 */
typedef enum MessageTypeTag
{
    /*
     * GPRS encode and decode utils assume that values of this enum
     * match the values of the 6-bit MESSAGE_TYPE field as defined in
     * 04.60 v8.0.0 11.2.0.1
     */
    /** PACKET ACCESS REJECT 
     * sent on the PCCCH or PACCH by the network to the mobile station 
     * to indicate that the network has rejected the MSs access request
     */
    PACKET_ACCESS_REJECT                = 0x21,  /* 10 0001  */
    /** Packet Cell Change Order
     * is sent on the PCCCH or PACCH by the network to the mobile station to 
     * command the mobile station to leave the current cell and change to a new cell
     */
    PACKET_CELL_CHANGE_ORDER            = 0x01,  /* 00 0001  */
    /** Packet Downlink Assignment
     * sent on the PCCCH or PACCH by the network to the mobile station to assign 
     * downlink resources to the mobile station
     */
    PACKET_DOWNLINK_ASSIGNMENT          = 0x02,  /* 00 0010  */
    /** Packet Measurement Order sent on the PCCCH or PACCH by the network to a 
     * mobile station giving information for NC and EXT measurement reporting and 
     * network controlled cell reselection
     */
    PACKET_MEASUREMENT_ORDER            = 0x03,  /* 00 0011  */
    /** Packet Paging Request sent on the PCCCH by the network to trigger channel 
     * access by up to four mobile stations, for either TBF or RR connection establishment
     */
    PACKET_PAGING_REQUEST               = 0x22,  /* 10 0010  */
    /** Packet PDCH Release; the network may broadcast the PACKET PDCH RELEASE message 
     * on PACCH to indicate one or more timeslots is no longer available for packet data 
     * service 
     */
    PACKET_PDCH_RELEASE                 = 0x23,  /* 10 0011  */
    /** Packet Polling Request sent on the PCCCH or PACCH by the network to the mobile 
     * station to solicit a PACKET CONTROL ACKNOWLEDGEMENT message from the mobile station
     */
    PACKET_POLLING_REQUEST              = 0x04,  /* 00 0100  */
    /** Packet Power Control/Timing Advance sent on PACCH by the network to the mobile 
     * station in order to update the mobile station timing advance or power control parameters
     */
    PACKET_POWER_CONTROL_TIMING_ADVANCE = 0x05,  /* 00 0101  */
    /** Packet PRACH Parameters sent on the PCCCH by the network to all mobile stations 
     * within the cell to update the PRACH parameters in between Packet System Information
     * messages containing PRACH parameters
     */
    PACKET_PRACH_PARAMETERS             = 0x24,  /* 10 0100  */
    /** Packet Queueing Notification sent on the PCCCH by the network to the mobile 
     * station to notify the mobile station that it is being placed in queue
     */
    PACKET_QUEUEING_NOTIFICATION        = 0x06,  /* 00 0110  */
    /** Packet Timeslot Reconfigure sent on the PACCH by the network to the mobile 
     * station to assign uplink and downlink resources
     */
    PACKET_TIMESLOT_RECONFIGURE         = 0x07,  /* 00 0111  */
    /** Packet TBF Release sent on the PACCH by the network to the mobile station 
     * to initiate release of an uplink or downlink TBF
     */
    PACKET_TBF_RELEASE                  = 0x08,  /* 00 1000  */
    /** Packet Uplink Ack/Nack sent on the PACCH by the network to the mobile 
     * station indicate the status of the received RLC data blocks
     */
    PACKET_UPLINK_ACK_NACK              = 0x09,  /* 00 1001  */
    /** Packet Uplink Assignment sent on the PCCCH or PACCH by the network to 
     * the mobile station to assign uplink resources
     */
    PACKET_UPLINK_ASSIGNMENT            = 0x0A,  /* 00 1010  */

    /** Packet Cell Change Continue */
    PACKET_CELL_CHANGE_CONTINUE         = 0x0B,  /* 00 1011  */
    /** Packet Neighbour Cell Data */
    PACKET_NEIGHBOUR_CELL_DATA          = 0x0C,  /* 00 1100  */
    /** Packet Serving cell data */
    PACKET_SERVING_CELL_DATA            = 0x0D,  /* 00 1101  */
    PACKET_DBPSCH_ASSIGNMENT            = 0x0E,  /* 00 1110  */
    MULTIPLE_TBF_DOWNLINK_ASSIGNMENT    = 0x0F,  /* 00 1111  */
    MULTIPLE_TBF_UPLINK_ASSIGNMENT      = 0x10,  /* 01 0000  */
    MULTIPLE_TBF_RECONFIGURE            = 0x11,  /* 01 0001  */
    MBMS_MS_ID_ASSIGNMENT               = 0x13,  /* 01 0011  */
    MBMS_ASSIGNMENT_NON_DISTRIBUTION    = 0x14,  /* 01 0100  */
    PS_HANDOVER_COMMAND                 = 0x15,  /* 01 0101  */
    PACKET_PHYSICAL_INFORMATION         = 0x16,  /* 01 0110  */
    DTM_HANDOVER_COMMAND                = 0x17,  /* 01 0111  */
    
    /** Packet Downlink Dummy Control Block sent on the PCCCH or PACCH by the 
     * network to the mobile station as a fill message with either of the 
     * optional parameters PAGE_MODE and PERSISTENCE_LEVEL or with no content
     */
    PACKET_DOWNLINK_DUMMY_CONTROL_BLOCK = 0x25,  /* 10 0101  */
    /** Packet System Information Type 1 is sent by the network on the PBCCH 
     * or PACCH giving information for Cell selection, for control of the PRACH, 
     * for description of the control channel(s) and optional global power control 
     * parameters
     */
    PACKET_SYSTEM_INFORMATION_1         = 0x31,  /* 11 0001  */
    /** Packet System Information Type 2 sent by the network on PBCCH and PACCH 
     * giving information of reference frequency lists, cell allocation, GPRS mobile 
     * allocations and PCCCH descriptions being used in the cell
     */
    PACKET_SYSTEM_INFORMATION_2         = 0x32,  /* 11 0010  */
    /** Packet System Information Type 3 sent by the network on the PBCCH or PACCH 
     * giving information of the BCCH allocation (BA(GPRS)) in the neighbour cells 
     * and cell selection parameters for serving cell and non-serving cells
     */
    PACKET_SYSTEM_INFORMATION_3         = 0x33,  /* 11 0011  */
    /** Packet System Information Type 3 bis sent by the network on the PBCCH and 
     * PACCH giving information of the BCCH allocation in the neighbour cells and 
     * cell selection parameters for non-serving cells
     */
    PACKET_SYSTEM_INFORMATION_3BIS      = 0x34,  /* 11 0100  */
    /** Packet System Information Type 4 sent by the network on the PBCCH and 
     * PACCH giving information directing the mobile station to make interference 
     * measurements
     */
    PACKET_SYSTEM_INFORMATION_4         = 0x35,  /* 11 0101  */
    /** Packet System Information Type 5 sent by the network on the PBCCH giving 
     * information for measurement reporting and network controlled cell reselection
     */
    PACKET_SYSTEM_INFORMATION_5         = 0x36,  /* 11 0110  */
    /** Packet System Information Type 6 sent by the network on the PBCCH or 
     * PACCH to provide broadcast information required by non-GSM networks
     */
    PACKET_SYSTEM_INFORMATION_6         = 0x30,  /* 11 0000  */
    /** Packet System Information Type 7 sent by the network on the PBCCH or 
     * PACCH to provide broadcast information required by non-GSM networks
     */
    PACKET_SYSTEM_INFORMATION_7         = 0x38,  /* 11 1000  */
    /** Packet System Information Type 8 optionally sent by the network on 
     * the PBCCH and PACCH giving information about Cell Broadcast Channel 
     * configuration
     */
    PACKET_SYSTEM_INFORMATION_8         = 0x39,  /* 11 1001  */
    /** Packet System Information 14 provides a mobile station in dual transfer 
     * mode with GPRS access-related information
     */
    /** Packet System Information 13 provides the mobile station with GPRS cell 
     * specific access-related information
     */
    PACKET_SYSTEM_INFORMATION_13        = 0x37,  /* 11 0111  */
    PACKET_SYSTEM_INFORMATION_14        = 0x3A,  /* 11 1010  */
    /** Packet System Information Type 3 ter sent by the network on the PBCCH or 
     * PACCH giving information on additional measurement and reporting parameters
     */
    PACKET_SYSTEM_INFORMATION_3TER      = 0x3C,  /* 11 1100  */
    /** Packet System Information Type 3 quater sent by the network on the PBCCH or 
     * PACCH giving information on 3G Neighbour Cells and additional measurement and 
     * reporting parameters 
     */
    PACKET_SYSTEM_INFORMATION_3QUA      = 0x3D,  /* 11 1101  */
    /** Packet System Information 15 sent to a mobile station with UTRAN capability;
     * a mobile station with no UTRAN capability shall ignore this message.
     */
    PACKET_SYSTEM_INFORMATION_15        = 0x3E,  /* 11 1110  */
    /** Packet System Information Type 16 */
    PACKET_SYSTEM_INFORMATION_16        = 0x28,  /* 10 1000  */
    PACKET_SERVING_CELL_SI              = 0x20,  /* 10 0000  */
    PACKET_CS_COMMAND                   = 0x27,  /* 10 0111  */
    PACKET_CS_RELEASE_INDICATION        = 0x29,  /* 10 1001  */
    MBMS_ASSIGNMENT_DISTRIBUTION        = 0x2a,  /* 10 1010  */
    MBMS_NEIGHBOURING_CELL_INFORMATION  = 0x2b,  /* 10 1011  */
    PACKET_MBMS_ANNOUNCEMENT            = 0x2c,  /* 10 1100  */
    PACKET_APPLICATION_INFORMATION      = 0x2d,  /* 10 1101  */
    /** Packet with unknow message */
    PACKET_UNKNOWN_MESSAGE              = 0xFF
}
DownlinkMessageType;

/** Priority for Control ACKS and ACK/NACK */
typedef enum MacCtrlAckPriorityTag
{
    /** MAC control packet ACK with high priority */
    MAC_CTRL_ACK_HIGH_PRI,
    /** MAC control packet ACK with low priority */
    MAC_CTRL_ACK_LOW_PRI,
    /** Number of priorities of MAC control packet */
    MAC_CTRL_ACK_NUM_PRI

} MacCtrlAckPriority;

/** Indicates which packet test mode to perform */
typedef enum PacketTestModeTag
{
    /** Packet test mode is off */
    PACKET_TEST_MODE_OFF                    = 0,
    /** Packet test mode A; transmit random data */
    TEST_MODE_A_TX_RANDOM_DATA              = 1,
    /** Packet test mode B; loop back received data */
    TEST_MODE_B_LOOPBACK_RX_DATA            = 2,
#if defined (UPGRADE_EDGE)
    /** Switched symmetric loopback mode for EGPRS */
    EGPRS_SWITCHED_SYMMETRIC_LOOPBACK_MODE  = 3,
    /** Switched asymmetric loopback mode for EGPRS */
    EGPRS_SWITCHED_ASYMMETRIC_LOOPBACK_MODE = 4,
#endif
    /** Last member of PacketTestMode */
    LAST_PACKET_TEST_MODE
}
PacketTestMode;


/***************************************************************************
 * Types - Structures
 **************************************************************************/

#endif

/* END OF FILE */

