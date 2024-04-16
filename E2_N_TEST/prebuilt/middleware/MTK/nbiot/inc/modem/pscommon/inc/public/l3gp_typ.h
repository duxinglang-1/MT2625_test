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
 *
 *  GPRS (and some GSM) types used in encoding/decoding of Layer 3 messages
 **************************************************************************/

#ifndef L3GP_TYP_H
#define L3GP_TYP_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#if !defined (L13_TYP_H)
#include     <l13_typ.h>
#endif

#if !defined (SIR_TYP_H)
#include     <sir_typ.h>
#endif

#if !defined (RL3_TYP_H)
#include <rl3_typ.h>  /* for idtype */
#endif

#if !defined (SML3_TYP_H)
#include <sml3_typ.h> /* for gsmcause */
#endif

#if !defined (GP13_TYP_H)
#include     <gp13_typ.h>
#endif

#if !defined (PDP_TYP_H)
#include <pdp_typ.h>    /* for pdptype */
#endif

#if !defined (GPSAPI_H)
#include <gpsapi.h>
#endif

#if !defined (GPNSAPI_H)
#include <gpnsapi.h>
#endif

#if !defined (GPQOS_H)
#include <gpqos.h>
#endif

#if !defined (GPGENTYP_H)
#include <gpgentyp.h>
#endif

#if !defined (GRRMRTYP_H)
#include <grrmrtyp.h>
#endif

#if !defined (L33G_TYP_H)
#include <l33g_typ.h>
#endif

#if defined (UPGRADE_REL5)
#include <sil3_typ.h>
#include <mnl3_typ.h>
#endif

/****************************************************************************
 * Macros exported to GPRS
 ****************************************************************************/
#define MAX_NUM_W                              64

#define  MAX_DIAGNOSTIC_SIZE                   28

#define  MAX_NETWORK_NAME_LENGTH               64

#define  MAX_BA_CA_CHANNELS                    64

/*****************************************************************
 * Macros for GPRS only
 *****************************************************************/

#define BIT_ZERO                                (Boolean) 0
#define BIT_ONE                                 (Boolean) 1

/* Downlink msg lengths: */
/* Octet-based lengths: */
#define MAX_LEN_OF_MSG_BLOCK                    22
#define MIN_LEN_OF_MSG_BLOCK                    20
#define MAX_VALID_MESSAGE_LEN                  (MAX_LEN_OF_MSG_BLOCK * 2)
#define MIN_VALID_MESSAGE_LEN                   MIN_LEN_OF_MSG_BLOCK
#define MAX_LEN_OF_PSI_MESSAGE                  MAX_LEN_OF_MSG_BLOCK

/* Bit-based lengths: */
#define MAX_VALID_MSG_LEN_BITS       (MAX_VALID_MESSAGE_LEN * LEN_OF_OCTET)
#define MAX_LEN_OF_PSI_MSG_BITS      (MAX_LEN_OF_PSI_MESSAGE * LEN_OF_OCTET)

/* Uplink msg lengths: */
/* Octet-based lengths: */
#define LEN_OF_UPLINK_MSG_BLOCK                 22
#define LEN_OF_UPLINK_MSG_BITS       (LEN_OF_UPLINK_MSG_BLOCK * LEN_OF_OCTET)


/* Field lengths, etc: */
#define LEN_MESSAGE_TYPE                        6
#define LEN_PAGE_MODE                           2

#define MAX_ALLOC_BITMAP_LEN               32   /* 32 Octets, = 256 bits */
#define MAX_SIZE_MAFL_CONTENTS             18

#define LEN_NSAPI                4   /* Used in NPDU Number List encode */
#define LEN_NPDU_NUMBER_VALUE    8
#define SIZE_NPDU_NUMBER_LIST    GPRS_NSAPI_15 - GPRS_NSAPI_5 + 1
#define NULL_NSAPI_VALUE         0 /* Used to terminate an N-PDU number list,
                                    * see 24.008 section 10.5.5.11 */

#define MAX_GPRS_EXTENSION_IE_LENGTH  8

#define MAX_CAPABILITIES                    4  /* One for GSM P, E or R,
                                                  one for GSM 1800,
                                                  one for GSM 1900,
                                                  and one spare  */

#define NUM_OF_GPRS_ALGOS        7  /* Number of GPRS ciphering algorithms */

#define EDRX_NOT_SUPPORTED       0

#define GPRS_TIMER_NOT_USED      0
#define GPRS_TIMER_DEACTIVATED   0xE0

/*****************************************************************
 *** Macros for GPRS only that are used on CCCH
 *****************************************************************/

#define MAX_ARFCN_INDEX_SIZE               64
#define MAX_NUM_RFL_NUMBERS                 4
#define MAX_RAW_MA_SIZE                     8
#define IA_ALPHA_NOT_PRESENT              255
#define IA_TIMING_ADV_INDEX_NOT_PRESENT   255
#define IA_TBF_STARTING_TIME_NOT_PRESENT  0xffff
#define IA_TMA_BIT                        0x04
#define IA_DOWNLINK_ASSIGNMENT_BIT        0x02
#define IA_TBF_ASSIGNMENT_BIT             0x01
#define IA_TBF_BITS                       0x07
#define IA_CHANGE_MARK_1_NOT_PRESENT      0xff
#define IA_EXTENDED_RA_NOT_PRESENT        0xff
#define IA_BEP_PERIOD2_NOT_PRESENT        0xff
#define MAX_SIZE_IA_FIXED_ALLOC_BITMAP    0x04

#define SI13_MA_NUMBER                    14

#define MAX_NUM_ACCESS_TECHNOLOGY         10 /* Allow a few spare entries */

#if defined (UPGRADE_REL5)
#define MAX_NUM_EMERGENCY_LIST           10   /* 24.008 4.4.4.6 */
#define NETWORK_FEATURE_LCS_MOLR_BIT     0x08 /* Bit 4 LCS_MOLR */

/* PS LCS Capabilities according to 24.008 10.5.5.22 */
#define PC_LCS_CAPABILITIES_NOT_PRESENT  0x00
#define GPS_C                            0x01 /* Bit 1 */
#define GPS_B                            0x02 /* Bit 2 */
#define GPS_A                            0x04 /* Bit 3 */
#define OTD_B                            0x08 /* Bit 4 */
#define OTD_A                            0x10 /* Bit 5 */
#endif

#if defined (ENABLE_GTTP_MESSAGING)
/* 3GPP TS 44.006 - The number of octets in a L3-message passed to layer 2 for
** transmission in acknowledged mode shall not exceed 251 octets
** due to corresponding limits in 3GPP TS 48.056. */
#define MAX_LLC_PDU_CONTAINER_SIZE 251
#endif

#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL7)
#define REQUESTED_MS_INFORMATION_BIT     0x08 /* Bit 4 - 24.008 10.5.5.25 Requested MS information */
#endif /* (UPGRADE_3G_FDD) && (UPGRADE_REL7) */

/****************************************************************************
 * Types exported to GPRS
 ****************************************************************************/

/** Network name coding scheme element */
typedef enum NetworkNameCodingSchemeTag
{
    /** Cell Broadcast data coding scheme  */
    NETWORK_NAME_SMS_CB_CODED   =   0,
    /** UCS2 */
    NETWORK_NAME_UCS2_CODED     =  1
}
NetworkNameCodingScheme;

/** Indicates whether MS shall or shall not use the Discontinuous Transmission DTX */
typedef enum DtxIndicatorTag
{
    /** MS may use Discontinuous Transmission */
    MS_MAY_USE_DTX,
    /** MS shall use Discontinuous Transmission */
    MS_SHALL_USE_DTX,
    /** MS shall not use Discontinuous Transmission */
    MS_SHALL_NOT_USE_DTX
}
DtxIndicator;

/** The different formats are distinguished by the FormatId field.   */
typedef enum FormatIdTag
{
    /** bit map 0  */
    BIT_MAP_0_FORMAT            =   0x00,
    /** 1024 range */
    RANGE_1024_FORMAT           =   0x80,
    /** 512 range */
    RANGE_512_FORMAT            =   0x88,
    /** 256 range */
    RANGE_256_FORMAT            =   0x8A,
    /** 128 range */
    RANGE_128_FORMAT            =   0x8C,
    /** variable bit map  */
    VARIABLE_BIT_MAP_FORMAT     =   0x8E
}
FormatId;

/** Specifies the type of measurement report
 * (Note that this <I> inverts </I> the sense in the L3/RLC signalling
 * where <I> 0 </I> indicates enhanced measurements)
 */
typedef enum MeasReportTypeTag
{
    /** Normal measurements  */
    USE_NORMAL_MEAS_REPORTING   = 0,
    /** Enhanced measurements  */
    USE_ENHANCED_MEAS_REPORTING = 1
}
MeasReportType;

typedef enum MeasReportRateTag
{
    NORMAL_RATE_REPORTING_ONLY      = 0,
    REDUCED_RATE_REPORTING_ALLOWED  = 1
}
MeasReportRate;

/*****************************************************************
 * L3 Message elements exported to GPRS
 *****************************************************************/
/** Type of AuthParamRandElement */
typedef Rand
AuthParamRandElement;


/** UMTS authentication challenge */
typedef Aut
AuthParamAutnElement;

/** Authentication Response parameter */
typedef Res
AuthParamSresElement;


/** Ciphering Key Sequence Number element.
 * The ciphering key sequence number is allocated by the network and sent
 * with the AUTHENTICATION REQUEST message to the mobile station where it
 * is stored together with the calculated ciphering key Kc.
 */
typedef struct CipheringKeySeqNumElementTag
{
    /** Key sequence */
    KeySequence                     keySequence;
}
CipheringKeySeqNumElement;

/** Identity type element */
typedef struct IdTypeElementTag
{
    /** Specifies which identity is requested.*/
    IdType                          type;
}
IdTypeElement;

/** Network Name information element */
typedef struct NetworkNameTag
{
    /** Specifies the coding scheme */
    NetworkNameCodingScheme         networkNameCodingScheme;
    /** Whether or not the MS should add the letters for the
     * Country's Initials to the text string.
     */
    Boolean                         addCountryInitials;
    /** Length of Network Name contents */
    Int8                            networkNameLength;
    /** Carries information about the number of spare bits in octet */
    Int8                            numberOfSpareBitsInLastOctet;
    /** Text String */
    Int8                            networkName[MAX_NETWORK_NAME_LENGTH];
}
NetworkName;

/** Local time zone */
typedef Int8
LocTimeZone;

/** Time Zone and Time information element */
typedef struct UniTimeAndLocTimeZoneTag
{
    /** This field uses the same format as the Year field used in the TP-Service-Centre-Time-Stamp,
     * which is defined in GSM 03.40, and its value shall be set as defined in GSM 02.42.
     */
    Int8                            year;
    /** This field uses the same format as the Month field used in the TP-Service-Centre-Time-Stamp,
     * which is defined in GSM 03.40, and its value shall be set as defined in GSM 02.42.
     */
    Int8                            month;
    /** This field uses the same format as the Day field used in the TP-Service-Centre-Time-Stamp,
     * which is defined in GSM 03.40, and its value shall be set as defined in GSM 02.42.
     */
    Int8                            day;
    /** This field uses the same format as the Hour field used in the TP-Service-Centre-Time-Stamp,
     * which is defined in GSM 03.40, and its value shall be set as defined in GSM 02.42.
     */
    Int8                            hour;
    /** This field uses the same format as the Minut field used in the TP-Service-Centre-Time-Stamp,
     * which is defined in GSM 03.40, and its value shall be set as defined in GSM 02.42.
     */
    Int8                            minute;
    /** This field uses the same format as the Second field used in the TP-Service-Centre-Time-Stamp,
     * which is defined in GSM 03.40, and its value shall be set as defined in GSM 02.42.
     */
    Int8                            second;
    /** This field uses the same format as the Time Zone field used in the TP-Service-Centre-Time-Stamp,
     * which is defined in GSM 03.40, and its value shall be set as defined in GSM 02.42.
     */
    LocTimeZone                     locTimeZone;
}
UniTimeAndLocTimeZone;

/** Daylight Saving Time information element */
typedef enum NetworkDaylightSavingTimeTag
{
    /** No adjustment for Daylight Saving Time */
    NO_ADJUSTMENT  = 0,
    /** +1 hour adjustment for Daylight Saving Time */
    PLUS_ONE_HOUR_ADJUSTMENT = 1,
    /** +2 hours adjustment for Daylight Saving Time */
    PLUS_TWO_HOURS_ADJUSTMENT = 2
}
NetworkDaylightSavingTime;

typedef struct CellDescrElementTag
{
    Int16                           ncc;
    Int16                           bcc;
    Arfcn                           bcchArfcnNumber;
}
CellDescrElement;

/** The purpose of the Cell Channel Description information element is to
 * provide the reference frequency list to be used to decode the mobile
 * allocation information element.
 */
typedef struct FrequencyListElementTag
{
    /** Format Identifier */
    FormatId                        formatId;
    /** The number of entries in the arfcn array
\assoc ARRAY \ref arfcn */
    Int16                           numArfcn;
    /** Absolute RF Channel.
     * For  a  RF channel with ARFCN = N belonging to the
     * frequency list the ARFCN N bit is coded with a
     * "1"; N = 1, 2, .. , 124.
     *
     * For  a  RF channel with ARFCN = N not belonging to
     * the frequency list the ARFCN N bit is coded
     * with a "0"; N = 1, 2 .. , 124.
     */
    Arfcn                           arfcn [MAX_BA_CA_CHANNELS];
    /** extension indication; this variable is for NeighbourCellsDescrElement only.
     * If received in System Information 2, 2bis, 5 or 5bis this bit indicates whether
     * the information element carries the complete information of a BCCH channel
     * sub list or whether a complementary information element is sent in another message.
     */
    Boolean                         extensionInd;
    /** BCCH allocation sequence number indication; this variable is for
     * NeighbourCellsDescrElement only.
     * The BA-IND is needed to allow the network to discriminate measurements
     * results related to different BAs (e.g. BA(BCCH) and BA(SACCH)) sent to the
     * MS.
     */
    Int8                            baInd;
    /** Each non null W(k) allows to compute, together with
     * some previous W(i) the ARFCN F(k) of a frequency in
     * the set.
     */
    Int16                           w [MAX_NUM_W];
}
CellChannelDescrElement,
/** The purpose of the Frequency List information element is to provide the
 * list of the absolute radio frequency channel numbers used in a frequency
 * hopping sequence.
 */
FrequencyListElement,
/** The purpose of the Neighbour Cells Description information element is to
 * provide the absolute radio frequency channel numbers of the BCCH carriers
 * to be monitored by the mobile stations in the cell.
 */
NeighbourCellsDescrElement;

/** The purpose of the Mobile Allocation information element is to provide that part
 * of the RF channels belonging to the cell allocation (coded with a "1" in the cell
 * channel description information element) which is used in the mobile hopping sequence.
 * The decoded form of this message is an array of numbers, which correspond to indexes
 * into the cell allocation table (caArfcn in the cell channel description information
 * element) of the channels used for hopping, and a count of the RF channels used.
 *
 * To get the nth ARFCN used in the the hopping sequence use:<UL>
 * <LI> caArfcn[ mac[ n ] ] </U:>
 * \note During message decode the MobileAllocationElements are decoded
 * without knowledge of the current cell allocation table, therefore
 * it is possible for mac[n] to be larger than numCaArfcn
 */
typedef struct MobileAllocationElementTag
{
    /** The number of RF channel used
\assoc ARRAY \ref mac */
    Int16                           numRfChannelsUsed;
    /** Mobile  allocation RF channel i.
     * The MA C i bit indicates whether or not the Mobile
     * allocation frequency list includes the  i'th
     * frequency in the cell allocation frequency list.
     */
    Int8                            mac[MAX_MA_CHANNELS];
}
MobileAllocationElement;

/** Channel Description element.
 * Provides a description of an allocable channel together with its SACCH
 */
typedef struct ChannelDescrElementTag
{
    /** Channel type (channelType and tdmaOffset fields are derived from
     * the same raw field)
     */
    ChannelType                     channelType;
    /** TDMA offset */
    Int8                            tdmaOffset;
    /** The Timeslot number */
    Int8                            timeslotNumber;
    /** Training Sequence Code (Range: 0 to 7)*/
    Int8                            trainingSequenceCode;
    /** Whether Single RF channel or RF hopping channel */
    Boolean                         hoppingChannel;
    /** Coded as the binary representation of the absolute RF channel number
     * (SINGLE_RF_CHANNEL)
     */
    Arfcn                           arfcn;
    /** Coded as the binary representation of the mobile allocation index offse
     * (RF_HOPPING_CHANNEL)
     */
    Int8                            maio;
    /** Hopping sequence number */
    Int8                            hsn;
}
ChannelDescrElement;

/** The purpose of the Starting Time information element is to provide the start
 * TDMA frame number, FN modulo 42432.
 */
typedef struct StartingTimeElementTag
{
    /** The start frame number */
    Int16                           frameNumber;
}
StartingTimeElement;

typedef struct MeasurementParametersTag
{
    Boolean                         multibandReportPresent;
    Boolean                         scBandReportPresent;
    Int8                            multibandReport;
    Int8                            scBandReport;
    Int8                            scaleOrd;

    MeasReportingOffsetThreshold    report900Param;
    MeasReportingOffsetThreshold    report1800Param;
    MeasReportingOffsetThreshold    report1900Param;
    MeasReportingOffsetThreshold    report850Param;
#if defined (UPGRADE_GSM400)
    MeasReportingOffsetThreshold    report400Param;
#endif
}
MeasurementParameters;

/*****************************************************************
 * Types for GPRS that are used on CCCH
 *****************************************************************/
/** Power reduction mode */
typedef enum PowerReductionModeTag
{
    /** PR mode A : for one addressed MS */
    POWER_REDUCTION_MODE_A    = 0x00,
    /** PR mode B : for all MS */
    POWER_REDUCTION_MODE_B    = 0x01
}
PowerReductionMode;

/** Packet page indicator element */
typedef enum PacketPageIndicatorTag
{
    /** RR connection paging */
    RR_CONNECTION_PAGING = 0,
    /** Packet paging */
    PACKET_PAGING        = 1
}
PacketPageIndicator;

/** The channel decription element */
typedef struct ChanReqDescriptionTag
{
    /** Indicates if channel decription is present
\assoc PRESENT \ref peakThroughputClass
\assoc PRESENT \ref radioPriority
\assoc PRESENT \ref rlcUnackMode
\assoc PRESENT \ref notAckSack
\assoc PRESENT \ref rlcOctetCount */
    Boolean                     present;
    /** Specifies a QoS parameter */
    PeakThroughput              peakThroughputClass;
    /** Indicates what priority level to use */
    RadioPriorityLevel          radioPriority;
    /** Whether or not RLC is in unacknowledge mode */
    Boolean                     rlcUnackMode;
    /** Indicates that the frame is not contain neither ACK nor
     * SACK acknowledgement
     */
    Boolean                     notAckSack;
    /** Indicates the number of RLC data octets, plus the number of RLC data block
     * length octets, that the mobile station wishes to transfer.
     */
    Int16                       rlcOctetCount;
    /** Packet Flow Identifier indicates the Packet Flow Identifier for a Packet
     * Flow Context; Oxff indicates not_present
     */
    Int8                        pfi;
}
ChanReqDescription;

typedef struct GprsArfcnIndexListTag
{
/** \assoc ARRAY \ref index */
    Int8            numEntries;
    Int8            index[MAX_ARFCN_INDEX_SIZE];
}
GprsArfcnIndexList;

/** GPRS raw mobile allocation element */
typedef struct GprsRawMaTag
{
    /** The number of entries in the list
\assoc ARRAY \ref list */
    Int8            numEntries;
    /** Holds the list of raw MA */
    Int8            list[MAX_RAW_MA_SIZE];
}
GprsRawMa;

typedef struct GprsRflNumberTag
{
/** \assoc ARRAY \ref number */
    Int8            numEntries;
    Int8            number[MAX_NUM_RFL_NUMBERS];
}
GprsRflNumber;

/** The parameter NETWORK_CONTROL_ORDER may have one of the values
 * NC0, NC1, NC2 or RESET
 */
typedef enum NetworkControlTag
{
    /** Network control order 0,
     * in this mode the MS performs autonomous cell reselection, but does not
     * send measurement reports to the network
     */
    NC_0         = 0,
    /** Network control order 1,
     * in this mode MS performs autonomous cell reselection and periodically sends
     * measurement reports to the network
     */
    NC_1         = 1,
    /** Network control order 2,
     * In this mode, the MS (Mobile Station) sends measurement reports to the network
     * but does not perform autonomous reselection
     */
    NC_2         = 2,
    /** RESET */
    NC_RESET     = 3
}
NetworkControl;

typedef enum PcMeasurementChannelTag
{
    MEASUREMENTS_ON_BCCH = 0,
    MEASUREMENTS_ON_PDCH = 1
}
PcMeasurementChannel;

typedef enum IaAllocationParamTypeTag
{
    IA_UPLINK_FIXED_ALLOCATION   = 0,
    IA_UPLINK_DYNAMIC_ALLOCATION = 1,
    IA_UPLINK_SINGLE_BLOCK       = 2,
    IA_DOWNLINK_SINGLE_BLOCK     = 3,
    IA_DOWNLINK                  = 4,
#if defined (UPGRADE_EDGE)
    IA_UPLINK_MULTI_BLOCK        = 5,
#endif
    IA_INVALID_ASSIGNMENT
}
IaAllocationParamType;

/** Packet assignement type element used in IA Rest Octets IE */
typedef enum IaPacketAssignmentTypeTag
{
    /** Packet Uplink Assignment */
    IA_PACKET_UPLINK_ASSIGNMENT   = 0,
    /** Packet Downlink Assignment  */
    IA_PACKET_DOWNLINK_ASSIGNMENT = 1,
    /** No packet ssignment */
    IA_NO_PACKET_ASSIGNMENT       = 2,
    /** Second Part Packet Assignment */
    IA_SECOND_PART_PACKET_IA      = 3
}
IaPacketAssignmentType;

typedef enum GprsPwrCtrlParamTypeTag
{
    SYS_INFO_PWR_CTRL_SUBSET = 0,
    GLOBAL_PWR_CTRL          = 1
}
GprsPwrCtrlParamType;

typedef enum AccessBurstTypeTag
{
    EIGHT_BIT_ACCESS_BURST  = 0,
    ELEVEN_BIT_ACCESS_BURST = 1,
    ACCESS_BURST_NOT_DEFINED = 2
}
AccessBurstType;

/** BTS output power parameters */
typedef struct BtsOutputPowerParamTag
{
    /** If present the power control is used; otherwise, if P0 is not present, the power control is not used
     * (ranges from 0dB to 30 dB in steps of 2)
     */
    Int8                    p0;
    /** Indicates the power control mode used (Mode A if False; Mode B if TRUE) */
    Boolean                 btsPowerControlModeB;
    /** Specifies the mode of power reduction */
    PowerReductionMode      prMode;
}
BtsOutputPowerParam;

typedef enum AccessTechnologyTag
{
    GSM_P           = 0,
    GSM_E           = 1,
    GSM_R           = 2,
    GSM_1800        = 3,
    GSM_1900        = 4,
    GSM_450         = 5,
    GSM_480         = 6,
    GSM_850         = 7,
    LAST_ACC_TECH
}
AccessTechnology;

typedef struct IaTbfParamsTag
{
    IaAllocationParamType   type;
    Int8                    alpha;
    Int8                    gamma;
    Boolean                 tfiAssignmentPresent;
    Int8                    tfiAssignment;
    Int8                    timingAdvIndex;
    Int16                   tbfStartingTime;
    Boolean                 btsOutputPowerParamPresent;
    BtsOutputPowerParam     btsOutputPowerParam;
    Boolean                 polling;

    /* For Downlink only */
    Int32                   tlli;
    Boolean                 rlcUnackMode;
    Boolean                 taValid;

    /* For Uplink only */
    Int8                    usf;
    Boolean                 usfGranularity;
#if defined (INCLUDE_GPRS_FIXED_ALLOC_IN_L3)
    Int8                    allocationLength;
    /* Max Size of allocationlength is 5 bits ==> 4 octets */
    Int8                    allocationBitmap[MAX_SIZE_IA_FIXED_ALLOC_BITMAP];
#endif
    CodingScheme            codingScheme;
    CodingScheme            tlliCodingScheme;
#if defined (UPGRADE_EDGE)
    PdchStructure           pdchStructure;
    Int8                    extendedRa;
    Int8                    numAccessTechnology;
    AccessTechnology        accessTechnologyRequest[MAX_NUM_ACCESS_TECHNOLOGY];
    Int8                    bepPeriod2;
    Boolean                 resegment;
    Int8                    egprsWindowSize;
    Int8                    numRadioBlocksAllocated;
    Int8                    linkQualMeasMode;
#endif
}
IaTbfParams;

typedef struct IaPacketChannelDescrTag
{
    Int8        timeslot;
    Int8        tsc;
    Boolean     hopping;
    Arfcn       arfcn;
    Boolean     directEncoding;
    Int8        maio;
    Int8        maNumber;
    Int8        changeMark1;
    Int8        hsn;
}
IaPacketChannelDescr;

/*****************************************************************
 * L3 Message Elements for GPRS that are used on CCCH
 *****************************************************************/

typedef struct GlobalPwrCtrlParamTag
{   /* Based on section 12.9 in GSM 04.60. */
    Int8                        alpha;
                    /* where 0 = 0.0, 1 = 0.1, .... 9 = 0.9, 10 = 1.0 */
    Int8                        tAvgW;
    Int8                        tAvgT;
    Int8                        pB;
                    /* where 0 = 0dB, 1 = -2dB, .... 15 = -30dB */
    PcMeasurementChannel        pcChannel;
    Boolean                     psi4Available;
    Int8                        nAvgI;
}
GlobalPwrCtrlParam;

typedef struct GprsPowerCtrlParamTag
{   /* Sent in PSI13, SI13, DTM Assignment Command */
    Int8                        alpha;            /* where 0 = 0.0, 1 = 0.1, .... 9 = 0.9, 10 = 1.0 */
    Int8                        tAvgW;
    Int8                        tAvgT;
    PcMeasurementChannel        pcChannel;
    Int8                        nAvgI;
}
GprsPowerCtrlParam;

typedef struct ExtBitsTag
{
    Int8        extLength;
    Int8        extBits[MAX_GPRS_EXTENSION_IE_LENGTH];
}
ExtBits;

typedef enum AllocationTypeTag
{
    FIXED_ALLOCATION                        = 3,
    SINGLE_BLOCK_ALLOCATION                 = 2,
    DYNAMIC_ALLOCATION                      = 1,
    FUTURE_EXTENSION                        = 0
}
AllocationType;

/** Specifes the type of frequency parameters available */
typedef enum FreqParamTypeTag
{
    /** ARFCN; representation of the absolute radio frequency channel number
     * (ARFCN) defined in 3GPP TS 05.05; range 0 to 1023
     */
    FP_ARFCN                              = 0,
    /** Indirect encode */
    FP_INDIRECT                           = 1,
    /** Direct encode 1 */
    FP_DIRECT_1                           = 2,
    /** Direct encode 2 */
    FP_DIRECT_2                           = 3
}
FreqParamType;

typedef struct GprsMobileAllocationTag
{
    Int8                hsn;
    Int8                maNumber;
    GprsRflNumber       rfl;
    GprsRawMa           rawMa;
    GprsArfcnIndexList  arfcnIndex;
}
GprsMobileAllocation;

/** Indirect encode element */
typedef struct IndirectEncodeTag
{
    /** Representation of the mobile allocation index offset (MAIO),
     * see 3GPP TS 05.02; range 0 to 63
     */
    Int8                        maio;
    /** This field is the binary reference to a GPRS mobile allocation received
     * in either the PSI2 information, the SI13/PSI13 information or a previous
     * assignment message, range: 0 to 15
     */
    Int8                        maNumber;
    /** Indicates if changeMark1 contains valid information
\assoc PRESENT \ref changeMark1 */
    Boolean                     changeMark1Present;
    /** These fields are the binary representations of the allowed values for the PSI
     * or SI change mark associated with the GPRS mobile allocation that the
     * maNumber field refers to; range: 0 to 3
     */
    Int8                        changeMark1;
    /** Indicates if changeMark2 contains valid information
\assoc PRESENT \ref changeMark2 */
    Boolean                     changeMark2Present;
    /** These fields are the binary representations of the allowed values for the PSI
     * or SI change mark associated with the GPRS mobile allocation that the
     * maNumber field refers to; range: 0 to 3
     */
    Int8                        changeMark2;
}
IndirectEncode;

typedef struct DirectEncode1Tag
{
    Int8                        maio;
    GprsMobileAllocation        ma;
}
DirectEncode1;

typedef struct DirectEncode2Tag
{
    Int8                        maio;
    Int8                        hsn;
/** \assoc ARRAY \ref octet */
    Int8                        maflLength;
    Int8                        octet[MAX_SIZE_MAFL_CONTENTS];
}
DirectEncode2;

/** Defines the frequency parameters */
typedef union  FreqParamUnionTag
{
    /** ARFCN; representation of the absolute radio frequency channel number
     * (ARFCN) defined in 3GPP TS 05.05; range 0 to 1023
     */
    Arfcn                       arfcn;
    /** Indirect encode */
    IndirectEncode              indirectEncode;
    /** Direct encode 1 */
    DirectEncode1               directEncode1;
    /** Direct encode 2 */
    DirectEncode2               directEncode2;
}
FreqParamUnion;

/** The Frequency Parameters information element defines frequency parameters
 * and a training sequence code (TSC), which may be allocated to a mobile station
 * to define its channel configuration.
 */
typedef struct FrequencyParametersTag
{
    /** Representation of the training sequence code; Range: 0 to 7 */
    Int8                        tsc;
    /** Indicates the type of frequency parameters available
\assoc UNION \ref freqParam */
    FreqParamType               freqParamType;
    /** Defines the frequency parameters used */
    FreqParamUnion              freqParam;
}
FrequencyParameters;

/** Packet Timing Advance information elements.
 * describes the timing advance mode and timing advance value assigned
 * to the mobile station.
 */
typedef struct PacketTimingAdvanceTag
{
    /** Indicates if timing advance is present
\assoc PRESENT \ref value */
    Boolean                     valuePresent;
    /** If the timing advance value is present, the mobile station shall use
     * the value contained therein after time defined in 3GPP TS 05.10; if the
     * timing advance value is not present the mobile station shall not change
     * its timing advance value
     */
    Int8                        value;
    /** Indicates if iming advance index is present
\assoc PRESENT \ref index
\assoc PRESENT \ref indexTimeslot */
    Boolean                     indexPresent;
    /** If present the mobile station shall begin operation of the Continuous
     * Timing Advance procedure at the point in time denoted by the TBF starting
     * time if present, otherwise after the reaction time specified in 3GPP TS 05.10
     */
    Int8                        index;
    /** This field indicates the timeslot assigned for Continuous Timing Advance operation on the PTCCH */
    Int8                        indexTimeslot;
}
PacketTimingAdvance;

/** Specifies which medium access mode to use */
typedef enum MacModeTag
{
    /** Dynamic Allocation */
    MAC_DYNAMIC_ALLOCATION                  = 0,
    /** Extended Dynamic Allocation */
    MAC_EXTENDED_DYNAMIC_ALLOCATION         = 1,
    /** Fixed Allocation, not half duplex mode */
    MAC_FIXED_ALLOCATION_NOT_HALF_DUPLEX    = 2,
    /** Fixed Allocation, half duplex mode */
    MAC_FIXED_ALLOCATION_HALF_DUPLEX        = 3,
    /** Unkown mode */
    MAC_UNKNOWN_ALLOCATION_MODE             = 0xFF
}
MacMode;

/** The Power Control parameters information element contains parameters the mobile
 * station shall use to determine its TX power level
 */
typedef struct PowerControlParamsTag
{
    Int8                        gammaTimeslotAllocation; /**< Bit field indicating which gammTn[] are present */
    /** This field is the binary representation of the parameter alpha for MS output power
     * control in units of 0.1, see 3GPP TS 05.08; range: 0 to 10
     */
    Int8                        alpha;
    /** The GAMMA_TN0 to 7 fields are the binary representation of the parameter ?CH for
     * MS output power control in units of 2 dB, see 3GPP TS 05.08. GAMMA_TN0 contains the
     * gamma value for timeslot number 0, GAMMA_TN1 contains the gamma value for timeslot
     * number 1, etc. If this information element is also used to determine the timeslot
     * allocation for an uplink TBF, for each timeslot, the presence of the GAMMA value
     * indicates that the timeslot is assigned, and the absence of the GAMMA value
     * indicates that the timeslot is not assigned.
     */
    Int8                        gammaTn[ALL_TIMESLOTS];
}
PowerControlParams;

/** Starting Frame Number Description.
 * There are two types of encoding for this IE : Relative Frame Number or Absolute Frame Number.
 */
typedef struct StartFrameNumberTag
{
    /** Indicates if start frame number is present
\assoc PRESENT \ref absoluteFnEncoding
\assoc PRESENT \ref frameNumber */
    Boolean                     startFnPresent;
    /** Whether Absolute Frame Number Encoding or Relative Frame Number Encoding */
    Boolean                     absoluteFnEncoding;
    /** If absoluteFnEncoding is TRUE, frameNumber stores the
     * absolute frame number; otherwise it stores the frame number of
     * the first frame of the RLCMAC block where the command was received,
     * this is used with 'k' to calculate the relative start time.
     */
    Int16                       frameNumber;
    /**  Integer k, from which the offset to be applied to N (tdma frame number)can be derived */
    Int16                       k;
}
StartFrameNumber;

/** Measurement Mapping elements */
typedef struct MeasMapTag
{
    /** Whether or not Measurement Mapping is present
\assoc PRESENT \ref measStartTime
\assoc PRESENT \ref measInterval
\assoc PRESENT \ref measBitMap */
    Boolean                     present;
    /** Measurement start time */
    StartFrameNumber            measStartTime;
    /** Indicates the number of block periods from the start of one
     * assigned measurement period to the beginning of the next measurement period
     */
    Int8                        measInterval;
    /** Indicates the timeslots assigned for use during measurement periods */
    Int8                        measBitMap;
}
MeasMap;

typedef struct DynamicAllocTag
{
    Boolean                     extendedDynamicAlloc;
    Boolean                     granularity4Burst;
/** \assoc PRESENT \ref uplinkTfi */
    Boolean                     uplinkTfiPresent;
    Int8                        uplinkTfi;
/** \assoc PRESENT \ref blocksGranted */
    Boolean                     blocksGrantedPresent;
    Int8                        blocksGranted;
    StartFrameNumber            tbfStartTime;
    Int8                        usf[ALL_TIMESLOTS];
/** \assoc PRESENT \ref powerCtrl */
    Boolean                     powerCtrlPresent;
    PowerControlParams          powerCtrl;
/** \assoc PRESENT \ref btsOutputPowerParam */
    Boolean                     btsOutputPowerParamPresent;
    BtsOutputPowerParam         btsOutputPowerParam;

    /*  Notes re dynamicAlloc:
     *
     * 1. blocksGranted holds the raw decoded value, without +9 offset.
     * 2. An out of range (0xFF) value of any entry in usf indicates
     *    that no allocation was received for that timeslot
     */
}
DynamicAlloc;

typedef struct SingleBlockAllocTag
{
    Int8                        timeslotNumber;
/** \assoc PRESENT \ref powerCtrl */
    Boolean                     powerCtrlPresent;
    PowerControlParams          powerCtrl;
/** \assoc PRESENT \ref btsOutputPowerParam */
    Boolean                     btsOutputPowerParamPresent;
    BtsOutputPowerParam         btsOutputPowerParam;
    StartFrameNumber            tbfStartTime;
    /*  Notes re singleBlockAlloc:
     *
     * 1. If ALPHA & GAMMA are present in the msg, the GAMMA value is
     *    stored in powerCtrl.Gamma[timeslotNumber]; otherwise,
     *    powerCtrl.present is FALSE.
     */
#if defined (UPGRADE_EDGE)
    /* This is the EDGE parameter which determines whether the SB is actually two blocks !! */
    Int8                        numberOfRadioBlocksAlloc;
#endif
}
SingleBlockAlloc;

#if defined (INCLUDE_GPRS_FIXED_ALLOC_IN_L3)
typedef struct FixedAllocTag
{
/** \assoc PRESENT \ref uplinkTfi */
    Boolean                     uplinkTfiPresent;
    Int8                        uplinkTfi;
    Boolean                     finalAllocation;
    Boolean                     repeatAllocation;
    Int8                        tsOverride;
    Int8                        dnlinkTimeslot;
    Boolean                     btsOutputPowerParamPresent;
    BtsOutputPowerParam         btsOutputPowerParam;
/** \assoc PRESENT \ref timeslotAllocation */
    Boolean                     timeslotAllocationPresent;
    Int8                        timeslotAllocation;
/** \assoc PRESENT \ref powerCtrl */
    Boolean                     powerCtrlPresent;
    PowerControlParams          powerCtrl;
    Boolean                     halfDuplexMode;
    MeasMap                     measMap;
    StartFrameNumber            tbfStartTime;
    Boolean                     allocBitmapIsPeriods;
    Int16                       allocBitmapLength;
    Int8                        allocBitmap[MAX_ALLOC_BITMAP_LEN];
    /*  Notes re fixedAlloc:
     *
     * 1. allocBitmapLength is always set by decode, regardless of whether
     *    it's present in the message.
     * 2. The above structure is used for the fixed allocation structures
     *    in each of the following msgs:
     *          Packet Uplink Assignment,
     *          Packet Uplink Ack/Nack msgs,
     *          Packet Timeslot Reconfigure.
     *
     *    These messages structures differ slightly.  In each case, there are
     *    some fields in the above data structure which are redundant, i.e.
     *    which have no corresponding field in the msg.  These are set up as
     *    follows in each case:
     *
     *        Packet Uplink Assignment:
     *              repeatAllocation = FALSE;
     *              tsOverride unchanged;
     *              measMap.present = FALSE;
     *              timeslotAllocationPresent = TRUE.
     *
     *        Packet Uplink Ack/Nack
     *              uplinkTfi and uplinkTfiPresent unchanged;
     *              dnlinkTimeslot unchanged;
     *              p0 & btsPowerControlModeA unchanged;
     *              powerCtrl unchanged;
     *              halfDuplexMode unchanged;
     *              measMap.present = FALSE.
     *
     *        Packet Timeslot Reconfigure.
     *              uplinkTfi and uplinkTfiPresent unchanged;
     *              repeatAllocation = FALSE;
     *              tsOverride unchanged;
     *              halfDuplexMode unchanged;
     *              timeslotAllocationPresent = TRUE.

     */
}
FixedAlloc;
#endif

typedef union  AllocationUnionTag
{
#if defined (INCLUDE_GPRS_FIXED_ALLOC_IN_L3)
    FixedAlloc                  fixedAlloc;
#else
    Int8                        fixedAlloc;
#endif
    SingleBlockAlloc            singleBlockAlloc;
    DynamicAlloc                dynamicAlloc;
    Int8                        extension;    /* For future extension */
}
AllocationUnion;

typedef struct GprsCellOptionsTag
{
    GprsNetworkMode    gprsNetworkMode;
    Int8               t3168;
    Int8               t3192;
    Int8               drxTimerMax;   /* DRX max is calculated by:- 2^(drxTimerMax - 1) seconds */
    AccessBurstType    accessBurstType;
    Boolean            ackIsRlcMacFormat;
    Int8               bsCvMax;
/** \assoc PRESENT \ref panDec
\assoc PRESENT \ref panInc
\assoc PRESENT \ref panMaxN3102 */
    Boolean            panPresent;
    Int8               panDec;
    Int8               panInc;
    Int8               panMaxN3102;  /* in units if 4 i.e. 0 = 4, 1 = 8,.... 7 = 32 */
    Boolean            egprsSupported;

#if defined (UPGRADE_EDGE)
    Boolean            eprachSupported;
    Int8               bepPeriod;
#endif
    Boolean            pfcFeatureModeSupported;
    Boolean            dtmSupported;
    Boolean            bssPagingCoordination;

    /* Release 4 extensions */
    Boolean            ccnActive;
    Boolean            nwExtUtbf;

#if defined (ENABLE_REL_6_EXTENSIONS)
    /* Release 6 extensions */
    Boolean            multipleTbfSupport;
    Boolean            extUlTbfNoData;
    Boolean            dtmEnhancementsSupport;
    Boolean            mbmsSupported;
    Boolean            mbmsNotificationSupport;
    Boolean            mnciSupport;
#endif
}
GprsCellOptions;

typedef struct PbcchDescriptionTag
{
    Int8                    pb;
    Int8                    tsc;
    Int8                    timeslot;
    Boolean                 useBcchArfcn;
    Boolean                 pbcchHopping;
    Arfcn                   pbcchArfcn;
    Int8                    pbcchMaio;
    Boolean                 psi14FreqParamPresent;
    FrequencyParameters     psi14FreqParam;
}
PbcchDescription;

/*****************************************************************
 * Types used in GMM and SM messages
 *****************************************************************/

#if defined (UPGRADE_REL5)
/** Emergency number element */
typedef struct EmergencyNumberTag
{
    /** The service category */
    Int8                            serviceCategory;
    /** The length of number
\assoc ARRAY \ref number */
    Int8                            numberLength;
    /** List the number */
    Bcd                             number[MAX_CALLED_BCD_NO_LENGTH];
}
EmergencyNumber;

/** Emergency number list element.
 * This IE may be sent by the network. If this IE is sent,
 * the contents of this IE indicates a list of emergency
 * numbers valid within the same MCC as in the cell on
 * which this IE is received.
 */
typedef struct EmergencyNumberListTag
{
    /** The number of emergency number in the list
\assoc ARRAY \ref emergencyNumber */
    Int8                            numEmergencyList;
    /** Emergency number list*/
    EmergencyNumber                 emergencyNumber[MAX_NUM_EMERGENCY_LIST];
}
EmergencyNumberListElement,
/** Emergency number list element.*/
EmergencyNumberList;
#endif

/** Timer multiplier element. */
typedef enum TimerMultiplierTag
{
    /** value is incremented in multiples of 2 seconds */
    TIMER_TWO_SECONDS = 0,
    /** value is incremented in multiples of 1 minute  */
    TIMER_MINUTES = 1,
    /** value is incremented in multiples of decihours */
    TIMER_DECIHOURS = 2,
    /** value indicates that the timer is deactivated. */
    TIMER_DEACTIVATED = 7
}
TimerMultiplier;

/** Timer multiplier element. */
typedef enum Timer3MultiplierTag
{
    /** value is incremented in multiples of 10 minutes */
    TIMER3_TEN_MINUTES = 0,
    /** value is incremented in multiples of 1 hour  */
    TIMER3_ONE_HOUR = 1,
    /** value is incremented in multiples 10 hours */
    TIMER3_TEN_HOURS = 2,
    /** value is incremented in multiples 2 seconds */
    TIMER3_TWO_SECONDS = 3,
    /** value is incremented in multiples 30 seconds */
    TIMER3_30_SECONDS = 4,
    /** value is incremented in multiples 1 minute */
    TIMER3_ONE_MINUTE = 5,
    /** value is incremented in multiples 320 hours */
    TIMER3_320_HOURS = 6,
    /** value indicates that the timer is deactivated. */
    TIMER3_DEACTIVATED = 7
}
Timer3Multiplier;

/** GPRS Timer information element.
* The purpose of the GPRS timer information element is to
* specify GPRS specific timer values, e.g. for the READY timer.
*
* The GPRS timer is a type 3 information element with 2 octets length.
*/
typedef struct GprsTimerTag
{
    /** Timer value */
    Int8             timerBaseValue;
    /** Defines the timer value unit for the GPRS */
    TimerMultiplier  timerMultiplier;
}
GprsTimer,
/** Specify GPRS specific timer values */
GprsTimerElement;

/** GPRS Timer3 information element.
* The purpose of the GPRS timer information element is to
* specify GPRS specific timer values.
*
* The GPRS timer is a type 3 information element with 2 octets length.
*/
typedef struct GprsTimer3ElementTag
{
    /** Timer value */
    Int8             timerBaseValue;
    /** Defines the timer value unit for the GPRS */
    Timer3Multiplier timer3Multiplier;
}
GprsTimer3Element;

#if defined (UPGRADE_NASMDL2)
/** Request type element.
 *  Ref: 24.008/10.5.6.17 and 24.301/9.9.4.14 */
typedef enum RequestTypeTag
{
    /** Initial request */
    REQ_TYPE_INITIAL_REQUEST    = 1,
    /** Handover */
    REQ_TYPE_HANDOVER           = 2,
    /** Unused, if received, the network shall interpret this as
     * Initial request
     */
    REQ_TYPE_UNUSED             = 3
}
RequestType;
#endif /* UPGRADE_NASMDL2 */

/*****************************************************************
 * GPRS SM messages
 *****************************************************************/
/** This message is sent by the MS to the network to request activation of a PDP context. */
typedef struct GprsActPdpContextRequestMessageTag
{
    /** Requested NSAPI */
    Nsapi                   requestedNsapi;
    /** Requested LLC SAPI */
    LlcSapi                 requestedSapi;
    /** Requested quality of service */
    QualityOfService        requestedQos;
#if defined (UPGRADE_3G)
    /** Highest currently active traffic class */
    TrafficClass            highestTrafficClass;
#endif
    /** Requested PDP type number */
    PdpTypeNumber           requestedPdpTypeNumber;
    /** Requested PDP address */
    PdpAddress              requestedPdpAddress;
    /** Whether or not APN is present
\assoc PRESENT \ref apn */
    Boolean                 apnPresent;
    /** Included in the message when the MS selects a specific
     * external network to be connected to
     */
    AccessPointName         apn;
    /** Indicates if protocol configuration options are present
\assoc PRESENT \ref pppConfigOpts */
    Boolean                 pppConfigOptsPresent;
    /** Included if the MS supports Network Requested Bearer Control procedures.
     * This parameter is included in the message when the MS wishes to transmit
     * (protocol) data (e.g. configuration parameters, error codes or messages/events)
     * to the network.
     */
    ProtocolConfigOptions   pppConfigOpts;
}
GprsActPdpContextRequestMessage;

/** This message is sent by the network to the MS to acknowledge
 * activation of a PDP context.
 */
typedef struct GprsActPdpContextAcceptMessageTag
{
    /** Negotiated SAPI */
    LlcSapi                 negotiatedSapi;
    /** Negotiated QoS */
    QualityOfService        negotiatedQos;
    /** Specifies the radio priority level */
    RadioPriorityLevel      rpl;
    /** Indicates if dynamic PDP address is present
\assoc PRESENT \ref pdpTypeNumber
\assoc PRESENT \ref dynamicPdpAddress */
    Boolean                 dynamicPdpAddressPresent;
    /** Indicates the type of PDP number */
    PdpTypeNumber           pdpTypeNumber;
    /** Shall be included by the network if the MS has requested the activation
     * of a PDP context with the PDP type IPv4 or IPv6 and dynamic addressing.
     */
    PdpAddress              dynamicPdpAddress;
    /** Indicates if PPP configuration optionsare  present
\assoc PRESENT \ref pppConfigOpts */
    Boolean                 pppConfigOptsPresent;
    /** Options are included to indicate the selected Bearer Control Mode to be applied
     * for all active PDP contexts sharing the same PDP Address and APN.
     */
    ProtocolConfigOptions   pppConfigOpts;
    /** Whether or not the packet flow identifier is present
\assoc PRESENT \ref packetFlowId */
    Boolean                 packetFlowIdPresent;
    /** Included if the network wants to indicate the Packet Flow
     * Identifier associated to the PDP context.
     */
    PacketFlowId            packetFlowId;
}
GprsActPdpContextAcceptMessage;

/** This message is sent by the network to the MS to reject activation of a PDP context.*/
typedef struct GprsActPdpContextRejectMessageTag
{
    /** Indicates the reason why a session management request is rejected.*/
    GsmCause                smCause;
    /** Indicates if PPP configuration options are present.
\assoc PRESENT \ref pppConfigOpts */
    Boolean                 pppConfigOptsPresent;
    /** Included in the message when the network wishes to transmit (protocol)
     * data (for instance configuration parameters, error codes or messages/events)
     * to the MS.
     */
    ProtocolConfigOptions   pppConfigOpts;
}
GprsActPdpContextRejectMessage;

/** This message is sent by the network to the MS to initiate
 * activation of a PDP context.   */
typedef struct GprsRequestPdpContextActMessageTag
{
    /** PDP type number */
    PdpTypeNumber           pdpTypeNumber;
    /** Offered PDP address */
    PdpAddress              offeredPdpAddress;
    /** Indicates if APN is present
\assoc PRESENT \ref apn */
    Boolean                 apnPresent;
    /** APN. The APN consists of two parts; the Network ID, which identifies the external
     * service requested by a user of the GPRS service and the Operator ID which specifies
     * routing information.
     */
    AccessPointName         apn;
}
GprsRequestPdpContextActMessage;

/** This message is sent by the MS to the network to
 * reject initiation of a PDP context activation.
 */
typedef struct GprsRequestPdpContextActRejMessageTag
{
    /** Indicates the reason why a session management request is rejected.*/
    GsmCause                smCause;
}
GprsRequestPdpContextActRejMessage;

/** This message requests modification of an active PDP context.
 */
typedef struct GprsModifyPdpContextRequestMessageTag
{
    /** Indicates the radio priority level */
    RadioPriorityLevel      rpl;
    /** Included in the message to request a new LLC SAPI if a new QoS is requested. */
    LlcSapi                 newSapi;
    /** Included in the message to request a modification of the QoS. */
    QualityOfService        newQos;
    /** Whether or not external PDN address is present
\assoc PRESENT \ref pdpTypeNumber
\assoc PRESENT \ref externalPdpAddress */
    Boolean                 externalPdpAddressPresent;
    /** The type of PDP number */
    PdpTypeNumber           pdpTypeNumber;
    /** Requested external PDN address */
    PdpAddress              externalPdpAddress;
    /** Whether or not packet flow identifier is present
\assoc PRESENT \ref packetFlowId */
    Boolean                 packetFlowIdPresent;
    /** Indicates the Packet Flow Identifier associated to the PDP context */
    PacketFlowId            packetFlowId;
}
GprsModifyPdpContextRequestMessage;

/** This message is sent to request deactivation of an active PDP context. */
typedef struct GprsDeactPdpContextRequestMessageTag
{
    /** Indicates the reason of deactivation.*/
    GsmCause                smCause;
    /** Whether or not the tear down indicator is present
\assoc PRESENT \ref tdiFlag */
    Boolean                 tdiPresent;
    /** Indicates whether only the PDP context associated with this
     * specific TI or all active PDP contexts sharing the same PDP
     * address and APN as the PDP context associated with this specific
     * TI shall be deactivated.
     */
    Boolean                 tdiFlag;
}
GprsDeactPdpContextRequestMessage;

/** This message requests modification of an active PDP context.
 */
typedef struct GprsMoModifyPdpContextRequestMessageTag
{
    /** Indicates if SAPI is present
\assoc PRESENT \ref requestedSapi */
    Boolean                 sapiPresent;
    /** Included in the message to request a new LLC SAPI if a new QoS is requested.*/
    LlcSapi                 requestedSapi;
    /** Whether or not the QoS is present
\assoc PRESENT \ref requestedQos */
    Boolean                 qosPresent;
    /** Included in the message to request a modification of the QoS.*/
    QualityOfService        requestedQos;
    /** Indicates if TFT is included
\assoc PRESENT \ref newTft */
    Boolean                 tftPresent;
    /** Included in the message to request a new TFT or
     * modification of an existing TFT or transfer extra
     * parameters to the network.
     */
    TrafficFlowTemplate     newTft;
}
GprsMoModifyPdpContextRequestMessage;

/** This message is sent to acknowledge the modification of an active PDP context. */
typedef struct GprsMoModifyPdpContextAcceptMessageTag
{
    /** Indicates if QoS is present
\assoc PRESENT \ref negotiatedQos */
    Boolean                 qosPresent;
    /** Included in the message if the network assigns a new QoS.*/
    QualityOfService        negotiatedQos;
    /** Indicates if SAPI is included
\assoc PRESENT \ref negotiatedSapi */
    Boolean                 sapiPresent;
    /** Included in the message if the network assigns a new LLC SAPI.*/
    LlcSapi                 negotiatedSapi;
    /** Indicates if radio priority level is present
\assoc PRESENT \ref newRpl */
    Boolean                 rplPresent;
    /** Included in the message only if the network modifies the radio priority */
    RadioPriorityLevel      newRpl;
    /** Whether or not packet flow identifier is present
\assoc PRESENT \ref packetFlowId */
    Boolean                 packetFlowIdPresent;
    /** Included if the network wants to indicate the Packet Flow Identifier
     * associated to the PDP context
     */
    PacketFlowId            packetFlowId;
}
GprsMoModifyPdpContextAcceptMessage;

/** This message is sent to reject a modification of an active PDP context. */
typedef struct GprsMoModifyPdpContextRejectMessageTag
{
    /** Indicates the cause */
    GsmCause                smCause;
}
GprsMoModifyPdpContextRejectMessage;

/** Activate Secondary PDP Context Request element.
 * This message is sent by the MS to the network to request
 * activation of an additional PDP context associated with
 * the same PDP address and APN as an already active PDP context.
 */
typedef struct GprsActSecondaryPdpContextRequestTag
{
    /** NSAPI associated with PDP context to activate */
    Nsapi                   requestedNsapi;
    /** Requested LLC SAPI */
    LlcSapi                 requestedSapi;
    /** Requested QoS */
    QualityOfService        requestedQos;
#if defined (UPGRADE_3G)
    /** Highest traffic class */
    TrafficClass            highestTrafficClass;
#endif
    /** Linked TI */
    LinkedTi                linkedTi;
    /** Indicates if TFT is present */
    Boolean                 tftPresent;
    /** Included if a linked PDP context without TFT has already been activated. */
    TrafficFlowTemplate     tft;
}
GprsActSecondaryPdpContextRequestMessage;

/** Activate Secondary PDP Context Accept element.
 * This message is sent by the network to the MS to acknowledge
 * activation of an additional PDP context associated with the
 * same PDP address and APN as an already active PDP context.
 */
typedef struct GprsActSecondaryPdpContextAcceptTag
{
    /** Negotiated SAPI */
    LlcSapi                 negotiatedSapi;
    /** Negotiated QoS */
    QualityOfService        negotiatedQos;
    /** Radio priority level */
    RadioPriorityLevel      rpl;
    /** Indicates if packet flow identifier is present
\assoc PRESENT \ref packetFlowId */
    Boolean                 packetFlowIdPresent;
    /** Included if the network wants to indicate the Packet
     * Flow Identifier associated to the PDP context.
     */
    PacketFlowId            packetFlowId;
}
GprsActSecondaryPdpContextAcceptMessage;

/** This message is sent by the network to the UE to reject activation of an
 * additional PDP context associated with the same PDP address and APN as an
 * already active PDP context.
 */
typedef struct GprsActSecondaryPdpContextRejectTag
{
    /** Indicates the cause for rejecting activation of an
     * additional PDP context.
     */
    GsmCause                smCause;
}
GprsActSecondaryPdpContextRejectMessage;

/** This message is sent by the network or the MS to pass information
 *  on the status of the indicated context and report certain error conditions.
 */
typedef struct GprsSmStatusMessageTag
{
    /** Indicates the cause value for the status message */
    GsmCause                smCause;
}
GprsSmStatusMessage;

/** Device properties 24.008 chapter 10.5.7.8 */
typedef enum DevicePropertiesElementTag
{
    /** MS is not configured for NAS signalling low priority */
    MS_NOT_CONFIGURED_FOR_LOW_PRIO      = 0,
    /** MS is configured for NAS signalling low priority */
    MS_CONFIGURED_FOR_LOW_PRIO          = 1
}
DeviceProperties,
/** Device Properties IE */
DevicePropertiesElement;

/** Old GUTI type 24.301 chapter 9.9.3.45 */
typedef enum OldGutiTypeElementTag
{
    NATIVE_GUTI          = 0,
    MAPPED_GUTI          = 1
}
OldGutiTypeElement;

/** MS Network Feature Support 24.008 chapter 10.5.1.15 */
typedef enum MsNetworkFeatureSupportElementTag
{
    EXTENDED_PERIODIC_TIMERS_NOT_SUPPORTED   = 0,
    EXTENDED_PERIODIC_TIMERS_SUPPORTED       = 1
}
MsNetworkFeatureSupportElement;


/* Definitions how values are used is in TS 24.008 10.5.5.32 */
typedef enum EdrxValueTag
{
    /** EDRX value 0 is acceptable in paging time window in NB-IOT  */
    EDRX_VALUE_0                       = 0,
    /** EDRX value 1 in E-UTRAN */
    EDRX_VALUE_1                       = 1,
    /** EDRX value 2 in E-UTRAN */
    EDRX_VALUE_2                       = 2,
    /** EDRX value 3 in E-UTRAN */
    EDRX_VALUE_3                       = 3,
    /** EDRX value 4 in E-UTRAN */
    EDRX_VALUE_4                       = 4,
    /** EDRX value 5 in E-UTRAN */
    EDRX_VALUE_5                       = 5,
    /** EDRX value 6 in E-UTRAN */
    EDRX_VALUE_6                       = 6,
    /** EDRX value 7 in E-UTRAN */
    EDRX_VALUE_7                       = 7,
    /** EDRX value 8 in E-UTRAN */
    EDRX_VALUE_8                       = 8,
    /** EDRX value 9 in E-UTRAN */
    EDRX_VALUE_9                       = 9,
    /** EDRX value 10 in E-UTRAN */
    EDRX_VALUE_10                      = 10,
    /** EDRX value 11 in E-UTRAN */
    EDRX_VALUE_11                      = 11,
    /** EDRX value 12 in E-UTRAN */
    EDRX_VALUE_12                      = 12,
    /** EDRX value 13 in E-UTRAN */
    EDRX_VALUE_13                      = 13,
    /** EDRX value 14 in E-UTRAN */
    EDRX_VALUE_14                      = 14,
    /** EDRX value 15 in E-UTRAN */
    EDRX_VALUE_15                      = 15
}
EdrxValue;

/** 24.008 10.5.5.32 eDRX structure
 */
typedef struct EdrxElementTag
{
    /**  */
    EdrxValue                        pagingTimeWindow;
    EdrxValue                        edrxValue;
}
EdrxElement;


/*****************************************************************
 * Types used in GPRS GMM messages
 *****************************************************************/

/* start: IMPORTANT IMPORTANT IMPORTANT */
/* MsNetwork Capability and MsNetworkCapabilityElement are
 * different structures.  MsNetworkCapabilityElement includes
 * the SsScreeningIndicator but MsNetworkCapability does NOT
 * include SsScreeningIndicator since this is already included
 * in the MsClassmark structure.  It is the responsibility
 * of the task sending the signal (GMM) to copy the
 * SsScreeningIndicator into the MsNetworkCapabilityElement
 * The same applies to the ucs2Support and soLSACapability */

/** Provides the network with information concerning aspects of
 * the mobile station related to GPRSIndicates general mobile station
 * characteristics and it shall therefore, except for fields explicitly
 * indicated, be independent of the frequency band of the channel
 * it is sent on.
 */

typedef struct MsNetworkCapabilityTag
{
    /**
    This boolean array will be indexed '0 to NUM_OF_GPRS_ALGOS',
    where the zeroth element is GEA_1, so to get the item in the
    enumeration gprsCipheringAlgorithm, subtract 1 (i.e.
    if algoGprsAvailable[GPRS_CIPHER_GEA_1 - 1] is TRUE then
    GPRS_CIPHER_GEA_1 is supported, etc.)
    */
    Boolean                  algoGprsAvailable[NUM_OF_GPRS_ALGOS];
    /** Indicates if Mobile station supports mobile terminated point
     * to point SMS via CS domain
     */
    Boolean                  smsCapabViaDedicatedChannels;
    /** Indicates if Mobile station supports mobile terminated point
     * to point SMS via PS domain
     */
    Boolean                  smsCapabViaGprsChannels;
    /** Indicates if used by a mobile station supporting\not supporting
     * R99 or later versions of the protocol.
     * Table below shows hoe to interpret the value of revisionLevelInd.
     *
     * <TABLE>
     * <TR><TD><B> revisionLevelInd</B></TD>    <TD><B> Description           </B></TD></TR>
     * <TR><TD>0               </TD>    <TD>used by a mobile station not supporting R99
     * or later versions of the protocol         </TD></TR>
     * <TR><TD>1               </TD>    <TD>used by a mobile station supporting R99 or
     * later versions of the protocol        </TD></TR>
     * </TABLE>
     */
    Boolean                  revisionLevelInd;
    /** Indicates if Mobile station does support BSS packet flow procedures. */
    Boolean                  pfcFeatureMode;

#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
    /** LCS VA capability */
    Boolean lcsVaCapabSupport;

    /** PS inter-RAT HO to UTRAN Iu mode capability */
    Boolean psInterRatHoToUtranIuCapabSupport;
#endif /* (UPGRADE_3G_FDD) && (UPGRADE_REL6) */
}
MsNetworkCapability;

#if defined(UPGRADE_NVRAM_OPTIMIZE) && defined(UPGRADE_NVRAM_ENHANCED)
typedef struct MsNetworkCapabilityVarTag
{
    /**
    This boolean array will be indexed '0 to NUM_OF_GPRS_ALGOS',
    where the zeroth element is GEA_1, so to get the item in the
    enumeration gprsCipheringAlgorithm, subtract 1 (i.e.
    if algoGprsAvailable[GPRS_CIPHER_GEA_1 - 1] is TRUE then
    GPRS_CIPHER_GEA_1 is supported, etc.)
    */
    Boolean                  algoGprsAvailable_0;
    Boolean                  algoGprsAvailable_1;
    Boolean                  algoGprsAvailable_2;
    /** Indicates if Mobile station supports mobile terminated point
     * to point SMS via CS domain
     */
    Boolean                  smsCapabViaDedicatedChannels;
    /** Indicates if Mobile station supports mobile terminated point
     * to point SMS via PS domain
     */
    Boolean                  smsCapabViaGprsChannels;
    /** Indicates if used by a mobile station supporting\not supporting
     * R99 or later versions of the protocol.
     * Table below shows hoe to interpret the value of revisionLevelInd.
     *
     * <TABLE>
     * <TR><TD><B> revisionLevelInd</B></TD>    <TD><B> Description           </B></TD></TR>
     * <TR><TD>0               </TD>    <TD>used by a mobile station not supporting R99
     * or later versions of the protocol         </TD></TR>
     * <TR><TD>1               </TD>    <TD>used by a mobile station supporting R99 or
     * later versions of the protocol        </TD></TR>
     * </TABLE>
     */
    Boolean                  revisionLevelInd;
    /** Indicates if Mobile station does support BSS packet flow procedures. */
    Boolean                  pfcFeatureMode;
#if defined(UPGRADE_TWGGE_NRW)

    /** LCS VA capability */
    Boolean lcsVaCapabSupport;

    /** PS inter-RAT HO to UTRAN Iu mode capability */
    Boolean psInterRatHoToUtranIuCapabSupport;

#else
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
    /** LCS VA capability */
    Boolean lcsVaCapabSupport;

    /** PS inter-RAT HO to UTRAN Iu mode capability */
    Boolean psInterRatHoToUtranIuCapabSupport;
#endif /* (UPGRADE_3G_FDD) && (UPGRADE_REL6) */
#endif
}
MsNetworkCapabilityVar;
#endif
/** MS network capability element.
 * provide the network with information concerning aspects of
 * the mobile station related to GPRS. The contents might affect
 * the manner in which the network handles the operation of
 * the mobile station. The MS network capability information
 * indicates general mobile station characteristics and it shall
 * therefore, except for fields explicitly indicated, be independent
 * of the frequency band of the channel it is sent on.
 */
typedef struct MsNetworkCapabilityElementTag
{
    /** Whether or not GPRS algorithm is available */
    Boolean                  algoGprsAvailable[NUM_OF_GPRS_ALGOS];
    /** Whether or not Mobile station supports mobile terminated point to
     * point SMS via CS domain.
     */
    Boolean                  smsCapabViaDedicatedChannels;
    /** Whether or not Mobile station supports mobile terminated point
     * to point SMS via PS domain.
     */
    Boolean                  smsCapabViaGprsChannels;
    /** Whether or not the ME has preference between the use of the default alphabet and the
     * use of UCS2.
     */
    Boolean                  ucs2Support;
    /** SS Screening Indicator */
    SsScreeningIndicator     ssScreeningIndicator;
    /** Whether or not the ME supports SoLSA (Support of Localized Service Areas) */
    Boolean                  soLSACapability;
    /** used by a mobile station supporting/not supporting  R99 or later versions
     * of the protocol
     */
    Boolean                  revisionLevelInd;
    /** Mobile station does support BSS packet flow procedures */
    Boolean                  pfcFeatureMode;

#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
    /** LCS VA capability */
    Boolean lcsVaCapabSupport;

    /** PS inter-RAT HO to UTRAN Iu mode capability */
    Boolean psInterRatHoToUtranIuCapabSupport;
#endif /* (UPGRADE_3G_FDD) && (UPGRADE_REL6) */

#if defined (UPGRADE_LTE)
    /** PS inter-RAT HO to UTRAN Iu mode capability */
    Boolean                  psInterRatHoToUtranIuMode;
    /** PS inter-RAT HO to E-UTRAN S1 mode capability */
    Boolean                  psInterRatHoToEutranS1Mode;
    /** ISR support */
    Boolean                  isrSupport;
    /** SRVCC to GERAN/UTRAN capability */
    Boolean                  srvccToGeranUtranCapability;
    /** EPC capability */
    Boolean                  epcCapability;
    /** CSFB capability */
    Boolean                  csfbCapability;
#endif /* UPGRADE_LTE */

}
MsNetworkCapabilityElement;

/* end: IMPORTANT IMPORTANT IMPORTANT */

/** Indicates whether a valid TMSI is available in the MS or not. */
typedef enum TmsiStatusElementTag
{
    /** No valid TMSI available */
    TMSI_STATUS_NOT_VALID = 0,
    /** Valid TMSI available */
    TMSI_STATUS_VALID     = 1
}
TmsiStatusElement;

#ifdef FEA_COMBINED_PROCEDURES
typedef struct TmsiBasedNriContainerElementTag
{
    Int8  nriContainerValue23to16;
    Int8  nriContainerValue15to14;
}
TmsiBasedNriContainerElement;
#endif /* FEA_COMBINED_PROCEDURES */

typedef struct MsRadAccCapabilityTag
{
/** \assoc ARRAY \ref accessTechnology
\assoc ARRAY \ref rfPowerCapability
\assoc ARRAY \ref egprs8pskRfPowerCapab \opt CONDITIONAL */
    Int8                        numOfAccessTechnologies;
    AccessTechnology            accessTechnology [MAX_CAPABILITIES];
    Int8                        rfPowerCapability [MAX_CAPABILITIES];
    Int8                        a5Bits;
    Boolean                     earlyClassmarkSend;
    Boolean                     pseudoSynchronisation;
    Boolean                     voiceGroupCallService;
    Boolean                     voiceBroadcastService;
    Int8                        hscsdMultislotClass;    /* 0xFF if absent */
    Int8                        gprsMultislotClass;     /* 0xFF if absent */
    Boolean                     gprsExtDynamicAlloc;
    Int8                        smsValue;               /* 0xFF if absent */
    Int8                        smValue;
#if defined(UPGRADE_EDGE)
    Int8                        egprsMultislotClass;     /* 0xFF if absent */
    Boolean                     egprsExtDynamicAlloc;
    Boolean                     pskOnUpLinkSupported;
    Int8                        egprs8pskRfPowerCapab [MAX_CAPABILITIES];
#endif
    Boolean                     geran4Fp1SupportEnabled;
#if defined(UPGRADE_EDGE)
    Boolean                     egprsSupportEnabled;
#endif
#if defined (UPGRADE_3G_FDD)
    Boolean                     fddSupportedEnabled;
#endif /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)
    Boolean                     tdd128SupportedEnabled;
#endif /* UPGRADE_3G_TDD128 */
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
    /** PS Handover Capability */
    Boolean                     psHoCapabSupportedEnabled;
#endif /* (UPGRADE_3G_FDD) && (UPGRADE_REL6) */
}
/**  Provides the radio part of the network with information concerning
 * radio aspects of the mobile station.
 *
 * The 04.08 v7.1.0 10.5.5.12a definition of this IE permits all fields to take distinct
 * values for each different Access Technology.  However the structure
 * above supports a more restricted implementation in which only
 * accessTechnolgy itself and rfPowerCapability vary per Access
 * Technology.  This allows us always to omit (i.e. default) A5 bits
 * and Multislot capability for 2nd and subsequent Access Technology.
 *
 * This shortens the message lengths considerably, and
 * reduces the risk of message overflow in the Packet Resource Request
 * message.  But note that overflow may still occur if there are
 * more than 2 Access Technologies.
 *
 * Length  field is calculated by the encode util, and is not part
 * of this data structure.
 * a5Bits:
 *          (if bit 7 = ms bit, and bit 0 = ls bit then) bit 6 gives the
 *          value for A5/1, bit 5 = A5/2 ...
 */
MsRadAccCapability,
/** Provides the radio part of the network with information concerning
 * radio aspects of the mobile station. The contents might affect the
 * manner in which the network handles the operation of the mobile
 * station.
 */
MsRadAccCapabilityElement;

#if defined (UPGRADE_3G)
/** Attach type information element (3GPP TS 24.008). */
typedef enum AttachTypeTag
{
    /** GPRS attach */
    GP_GPRS_ONLY_ATTACH = 1,
    /** Not used. This value was allocated in earlier versions of the protocol */
    GP_GPRS_ATTACH_WHILE_IMSI_ATTACHED = 2,
    /** Combined GPRS/IMSI attach */
    GP_COMBINED_IMSI_ATTACH = 3
}
AttachType;
/** Attach type element.
 * The purpose of the attach type information element is to indicate
 * the type of the requested attach, i.e. whether the MS wants to
 * perform a GPRS or combined GPRS attach.
 *
 * The attach type is a type 1 information element.
 */
typedef struct AttachTypeElementTag
{
    /** Type of attach */
    AttachType attachType;
    /** Whether or not follow-on request pending */
    Boolean    followOnRequest;
}
AttachTypeElement;

typedef enum UpdateTypeTag
{
    GP_RA_ONLY_UPDATING = 0,
    GP_COMBINED_UPDATING = 1,
    GP_COMBINED_UPDATING_WITH_IMSI_ATTACH = 2,
    GP_PERIODIC_UPDATING = 3
}
UpdateType;

typedef struct UpdateTypeElementTag
{
    UpdateType updateType;
    Boolean    followOnRequest;
}
UpdateTypeElement;

#else

typedef enum AttachTypeTag
{
    GP_GPRS_ONLY_ATTACH = 1,
    GP_GPRS_ATTACH_WHILE_IMSI_ATTACHED = 2,
    GP_COMBINED_IMSI_ATTACH = 3
}
AttachType,
AttachTypeElement;

typedef enum UpdateTypeTag
{
    GP_RA_ONLY_UPDATING = 0,
    GP_COMBINED_UPDATING = 1,
    GP_COMBINED_UPDATING_WITH_IMSI_ATTACH = 2,
    GP_PERIODIC_UPDATING = 3
}
UpdateType,
UpdateTypeElement;

#endif

/** P-TMSI signature identifies a GMM context and it is 4 octets length */
typedef Int32 PtmsiSignature, PtmsiSignatureElement;

/** Specifies the result of a GPRS attach proceed */
typedef enum AttachResultTag
{
    /** GPRS only attached */
    GP_GPRS_ONLY_ATTACHED = 1,
    /** Combined GPRS/IMSI attached */
    GP_COMBINED_ATTACHED = 3
}
AttachResult,
AttachResultElement;

#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
/** GSM 24.008: 10.5.5.1 Attach result */
typedef enum FollowOnProceedTag
{
    FOLLOW_ON_PROCEED = 0,
    NO_FOLLOW_ON_PROCEED = 1
}
FollowOnProceed;

/* CR-0882 Follow-on proceed for the PS domain
** Rec 24.008 10.5.5.1 Attach Result
** |xxxx|FOP|Result of attach|
** FOP = 1 bit Follow-on prodeed
** Attach Result : 3 bits
*/
typedef struct AttachResultInformationElementTag
{
    AttachResultElement attachResult;
    FollowOnProceed     followOnProceed;
}
AttachResultInformationElement;
#endif /* (UPGRADE_3G_FDD) && (UPGRADE_REL6) */

typedef enum UpdateResultTag
{
    GP_GPRS_UPDATE = 0,
    GP_COMBINED_UPDATE = 1
}
UpdateResult,
UpdateResultElement;

#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
/* CR-0882 Follow-on proceed for the PS domain
** Rec 24.008 10.5.5.17 Update Result
** |xxxx|FOP|Update result value|
** FOP = 1 bit Follow-on prodeed
** Update result value : 3 bits
*/
typedef struct UpdateResultInformationElementTag
{
    UpdateResult        updateResult;
    FollowOnProceed     followOnProceed;
}
UpdateResultInformationElement;
#endif /* (UPGRADE_3G_FDD) && (UPGRADE_REL6) */

/** Type of detach element. */
typedef enum MoDetachTag
{
    /** GPRS detach */
    GP_MO_GPRS_DETACH = 0x1,
    /** IMSI detach */
    GP_MO_IMSI_DETACH = 0x2,
    /** Combined GPRS/IMSI detach */
    GP_MO_COMBINED_DETACH = 0x3
}
MoDetach;

/** Detach type element.
 * Indicate which type of detach is requested by the MS.
 * In the network to MS direction the detach type information
 * element is used to indicate the reason why a detach request is sent.
 *
 * The detach type is a type 1 information element.
 */
typedef struct MoDetachTypeTag
{
    /** Power switched off (Not used in network to MS direction)*/
    Boolean              powerOff;
    /** Type of detach */
    MoDetach             moDetach;
}
MoDetachTypeElement,
MoDetachType;

/** Indicates the reason why a detach request is sent.*/
typedef enum MtDetachTypeTag
{
    /** Re-attach required */
    GP_MT_REATTACH_REQUIRED = 0x1,
    /** Re-attach not required */
    GP_MT_REATTACH_NOT_REQUIRED = 0x2,
    /** IMSI detach (after VLR failure) */
    GP_MT_IMSI_DETACH = 0x3
}
MtDetachTypeElement,
MtDetachType;

typedef Int32 Ptmsi, PtmsiElement;

/** Authentication and ciphering reference number is of type Int8 */
typedef Int8 AuthCipherRefNum,
/** Authentication and ciphering reference number is of type Int8 */
AuthCipherRefNumElement;

/** NPDU number element. */
typedef struct NpduNumberTag
{
    /** The Network Service Access Point Identifier is an index to the PDP context that
     * is using the services provided by the lower layer (SNDCP)
     */
    Nsapi            nsapi;
    /** NPDU number value (Int16 so that out of range is 0xffff) */
    Int16            npduNumberValue;
}
NpduNumber;

/** N-PDU Number list element. */
typedef struct NpduNumberListTag
{
    /** The number of N-PDU numbers
\assoc ARRAY \ref npduNumber */
    Int8       numNpduNumber;
    /** List of Receive N PDU Numbers (shall be included if the
     * routing area update accept message contained this IE)
     */
    NpduNumber npduNumber[SIZE_NPDU_NUMBER_LIST];
}
NpduNumberList,
/** N-PDU Number list element. */
NpduNumberListElement;

/** Ciphering algorithm element. */
typedef GprsCipheringAlgorithm CipheringAlgorithmElement;
typedef RadioPriorityLevel     RadioPriorityLevelElement;

/** PDP context status of type Int16.
 * \image html PdpContextStatus.png
 * NSAPI(x) shall be coded as shown in the table below.
 * <TABLE>
 * <TR><TD><B> Index        </B></TD>    <TD><B> Description           </B></TD></TR>
 * <TR><TD> NSAPI(0) to NSAPI(4)               </TD>    <TD> are coded as 0 and shall be treated as
 * spare in this version of the protocol.         </TD></TR>
 * <TR><TD> NSAPI(5) to NSAPI(15)                 </TD>    <TD> 0 indicates that the SM state of the
 * corresponding PDP context is PDP-INACTIVE.
 *
 * 1 indicates that the SM state of the corresponding PDP context is not PDP-INACTIVE.</TD></TR>
 * </TABLE>
 */
typedef Int16 PdpContextStatus;
#if defined (UPGRADE_REL7) && defined (UPGRADE_3G_FDD)
/** Uplink Data Status of type Int16.
 * \image html UplinkDataStatus.png
 * NSAPI(x) shall be coded as shown in the table below.
 * <TABLE>
 * <TR><TD><B> Index        </B></TD>    <TD><B> Description           </B></TD></TR>
 * <TR><TD> NSAPI(0) to NSAPI(4)               </TD>    <TD> are coded as 0 and shall be treated as
 * spare in this version of the protocol.         </TD></TR>
 * <TR><TD> NSAPI(5) to NSAPI(15)                 </TD>    <TD> 0 indicates that the SM state of the
 * corresponding PDP context is PDP-INACTIVE.
 *
 * 1 indicates that the SM state of the corresponding PDP context is PENDING.</TD></TR>
 * </TABLE>
 */
typedef Int16 UplinkDataStatus;
#endif /* (UPGRADE_REL7) && (UPGRADE_3G_FDD) */

#if defined (UPGRADE_NASMDL2)
/** PSD bearer context status is a generic name for status of
 *  PDP Contexts or EPS Bearer Contexts depending on the current RAT.
 * \image html psd_bearer_context_status_diagram.png
 * PSD bearer id (x) shall be coded as shown in the table below.
 * <TABLE>
 * <TR>
 * <TD><B> Index </B></TD>
 * <TD><B> Description </B></TD>
 * </TR>
 * <TR>
 * <TD> PSD bearer id (0) to PSD bearer id (4) </TD>
 * <TD> are spare and shall be coded as zero. </TD>
 * </TR>
 * <TR>
 * <TD> PSD bearer id (5) to PSD bearer id (15) </TD>
 * <TD> 0 indicates that the SM state of the corresponding PSD bearer context is INACTIVE.
 *
 * 1 indicates that the SM state of the corresponding PSD bearer context is ACTIVE. </TD>
 * </TR>
 * </TABLE>
 */
typedef Int16 PsdBearerContextStatus;
#endif /* UPGRADE_NASMDL2 */

typedef Int16 HeaderCompressionStatus;

#if defined (UPGRADE_3G)
typedef enum GmmServiceTypeTag
{
    SERVICE_TYPE_SIGNALLING      =   0,
    SERVICE_TYPE_DATA            =   1,
    SERVICE_TYPE_PAGING_RESPONSE =   2
}
GmmServiceType;
#endif

/*****************************************************************
 * GPRS GMM messages
 *****************************************************************/
/** This message is sent by the MS to the network in order to
 * perform a GPRS or combined GPRS attach.
 */
typedef struct GprsGmmAttachRequestTag
{
    /** MS network capability (GSM 04.08,  10.5.5.12)*/
    MsNetworkCapabilityElement msNetworkCapability;
    /** Attach type (GSM 04.08,  10.5.5.2 ) */
    AttachTypeElement          attachType;
    /** Ciphering key sequence number (GSM 04.08,  10.5.1.2) */
    CipheringKeySeqNumElement  gprsCksn;
    /** DRX parameters */
    GprsDrxParametersElement   gprsDrxParameters;
    /** Mobile identity (P-TMSI or IMSI)*/
    MobileIdElement            mobileId;
    /** Old routing area identification */
    RoutingAreaIdElement       oldRai;
    /** MS Radio Access capability (GSM 04.08, 10.5.5.12a ) */
    MsRadAccCapabilityElement  msRadAccCapability;
    /** Indicates if old P-TMSI signature is present
\assoc PRESENT \ref oldPtmsiSignature */
    Boolean                    oldPtmsiSignaturePresent;
    /**  Included if a valid P-TMSI and P-TMSI signature
     * are stored in the MS (GSM 04.08,  10.5.5.8 )
     */
    PtmsiSignatureElement      oldPtmsiSignature;
    /** Indicates if requested ready timer value is present
\assoc PRESENT \ref requestedReadyTimer */
    Boolean                    requestedReadyTimerPresent;
    /** Included if the MS wants to indicate a preferred value
     * for the READY timer (GSM 04.08, 10.5.7.3)
     */
    GprsTimerElement           requestedReadyTimer;
    /** Indicates if TMSI status is present */
    Boolean                    tmsiStatusPresent;
    /** Included if the MS performs a combined GPRS attach
     * and no valid TMSI is available (GSM 24.008, 10.5.5.4)
     */
    TmsiStatusElement          tmsiStatus;
#if defined (UPGRADE_REL5)
    /** Included if the MS supports at least one positioning
     * method for the provision of location services (LCS)
     * via the PS domain in Gb-mode(GSM 24.008 10.5.5.22)
     */
    Int8                       psLcsCapability;
#endif
}
GprsGmmAttachRequest;

/** Attach accept message element.
 * This message is sent by the network to the MS to indicate that
 * the corresponding attach request has been accepted.
 */
typedef struct GprsGmmAttachAcceptTag
{
    /** The result of attach (GSM 04.08,  10.5.5.1) */
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
    AttachResultInformationElement attachResultInformation;
#else /* !(UPGRADE_3G_FDD) || !(UPGRADE_REL6) */
    AttachResultElement       attachResult;
#endif /* !(UPGRADE_3G_FDD) || !(UPGRADE_REL6) */
    /** Whether or not force to standby is indicated (GSM 04.08,  10.5.5.7)*/
    Boolean                   forceToStandby;
    /** Periodic RA update timer */
    GprsTimerElement          periodicRAUpdateTimer;
    /** Radio priority for SMS */
    RadioPriorityLevelElement rplForSms;
    /** Routing area identification */
    RoutingAreaIdElement      rai;
    /** Indicates if P-TMSI signature is present.
\assoc PRESENT \ref ptmsiSignature */
    Boolean                   ptmsiSignaturePresent;
    /** Assigns an identity to the MS's GMM context (GSM 04.08,  10.5.5.8) */
    PtmsiSignatureElement     ptmsiSignature;
    /** Indicates if Negotiated READY timer is present.
\assoc PRESENT \ref negotiatedReadyTimer */
    Boolean                   negotiatedReadyTimerPresent;
    /** Indicates a value for the READY timer. */
    GprsTimerElement          negotiatedReadyTimer;
    /** Indicates if allocated P-TMSI is present.
\assoc PRESENT \ref allocatedPtmsi */
    Boolean                   allocatedPtmsiPresent;
    /** Assign a P-TMSI to an MS in case of a GPRS or
     * combined GPRS attach (GSM 04.08,  10.5.1.4).
     */
    MobileIdElement           allocatedPtmsi;
    /** Indicates if MS identity is present.
\assoc PRESENT \ref mobileId */
    Boolean                   mobileIdPresent;
    /** Assigns or unassigns a TMSI to an MS in case of
     * a combined GPRS attach (GSM 04.08,  10.5.1.4).
     */
    MobileIdElement           mobileId;
    /** Included when IMSI attach for non-GPRS
     * services was not successful during a combined
     * GPRS attach procedure (GSM 04.08,  10.5.5.14).
     */
    GsmCause                  gmmCause;
    /** Indicates if T3302 value is present
\assoc PRESENT \ref t3302Timer */
    Boolean                   t3302TimerPresent;
    /** Indicates a value for the T3302 timer.*/
    GprsTimerElement          t3302Timer;
    /** Indicates the ability to support the Cell Notification */
    Boolean                   cellNotificationSupported;
    /** Included if the network wants to inform the mobile
     * station of equivalent PLMNs.
     */
    EquivalentPlmnListElement equivalentPlmnList;
#if defined (UPGRADE_REL5)
    /** GSM 24.008, 10.5.5.23 Network Feature Support*/
    Boolean                   networkFeatureLcsMolrSupported;
    /** Indicates if a list of emergency numbers is present */
    Boolean                   emergencyNumberListPresent;
    /** Indicates a list of emergency numbers valid within the same
     * MCC as in the cell on which this IE is received (GSM 24.008, 10.5.3.13).
     */
    EmergencyNumberList       emergencyNumberList;
#endif
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL7)
    /** GSM 24.008 10.5.5.25 Requested MS information
    ** PS HandOver not supported.
    ** Not coded yet
    \assoc PRESENT \ref interRatInfoContainerRequested */
    Boolean                   interRatInfoContainerRequestedPresent;
    Boolean                   interRatInfoContainerRequested;
    /** GSM 24.008 GPRS Timer 2
    ** Indicates if T3319 value is present
\assoc PRESENT \ref t3319Timer */
    Boolean                   t3319TimerPresent;
    /** Indicates a value for the T3302 timer.*/
    GprsTimerElement          t3319Timer;
#endif /* (UPGRADE_3G_FDD) && (UPGRADE_REL7) */
}
GprsGmmAttachAccept;

/** Attach reject message element.
 * This message is sent by the network to the MS to indicate
 * that the corresponding attach request has been rejected.
 */
typedef struct GprsGmmAttachRejectTag
{
    /** The cause for rejecting the attach request */
    GsmCause           rejectCause;
    /** Indicates if T3302 value is present
\assoc PRESENT \ref t3302Timer */
    Boolean            t3302TimerPresent;
    /** Indicate a value for the T3302 timer */
    GprsTimerElement   t3302Timer;
}
GprsGmmAttachReject;

/** This message is sent by the MS to request the release of a GMM context */
typedef struct GprsGmmMoDetachRequestTag
{
    /** Detach type */
    MoDetachTypeElement       moDetachType;
    /** Indicates if PTMSI is present */
    Boolean                   ptmsiPresent;
    /** P-TMSI ( GSM 04.08,  10.5.1.4) */
    MobileIdElement           ptmsi;
    /** Indicates if P-TMSI signature is present */
    Boolean                   ptmsiSignaturePresent;
    /** Included if the MS has a valid P-TMSI signature (GSM 04.08,  10.5.5.8) */
    PtmsiSignatureElement     ptmsiSignature;
}
GprsGmmMoDetachRequest;

/** This message is sent by the network to request the release of a GMM context. */
typedef struct GprsGmmMtDetachRequestTag
{
    /** Detach type */
    MtDetachTypeElement mtDetachType;
    /** Indicates if force to standby indicated */
    Boolean             forceToStandby;
    /** Included in case the detach reason has to be indicated
     * to the MS for instance due to a failed IMEI check.
     */
    GsmCause            gmmCause;
}
GprsGmmMtDetachRequest;

/** This message is sent by the network to indicate that
 * the detach procedure has been completed.
 */
typedef struct GprsGmmMoDetachAcceptTag
{
    /** Indicates if force to standby indicated */
    Boolean        forceToStandby;
}
GprsGmmMoDetachAccept;

/** This message is sent by the network to the MS to reallocate a P-TMSI. */
typedef struct GprsGmmPtmsiReallocCommandTag
{
    /** Allocated PTMSI */
    MobileIdElement       allocatedPtmsi;
    /** Routing area identification */
    RoutingAreaIdElement  rai;
    /** Whether or not force to standby is indicated (GSM 04.08,  10.5.5.7)*/
    Boolean               forceToStandby;
    /** Indicates if P-TMSI signature is present.
\assoc PRESENT \ref ptmsiSignature */
    Boolean               ptmsiSignaturePresent;
    /** Assigns an identity to the MS's GMM context (GSM 04.08,  10.5.5.8) */
    PtmsiSignatureElement ptmsiSignature;
}
GprsGmmPtmsiReallocCommand;

/** Authentication and ciphering request element.
 * This message is sent by the network to the MS to initiate
 * authentication of the MS identity. Additionally, the ciphering
 * mode is set, indicating whether ciphering will be performed or not.
 */
typedef struct GprsGmmAuthCipherRequestTag
{
    /** Ciphering algorithm (GSM 04.08,  10.5.5.3 )*/
    CipheringAlgorithmElement cipheringAlgorithm;
    /** Whether or not IMEISV is requested (GSM 04.08,  10.5.5.10)*/
    Boolean                   imeisvRequest;
    /** Whether or not force to standby is indicated (GSM 04.08,  10.5.5.7)*/
    Boolean                   forceToStandby;
    /**  Included if and only if the Authentication parameter
     * RAND is contained in the message (GSM 04.08,  10.5.5.19).
     */
    AuthCipherRefNumElement   authCipherRefNum;
    /** Indicates if GPRS authentication parameters are present
\assoc PRESENT \ref gprsAuthParamRand
\assoc PRESENT \ref gprsCksn */
    Boolean                   gprsAuthParamsPresent;
    /** GPRS authentication parameter RAND (GSM 04.08,  10.5.3.1) */
    AuthParamRandElement      gprsAuthParamRand;
    /** Ciphering Key Sequence Number (GSM 04.08,  10.5.1.2)  */
    CipheringKeySeqNumElement gprsCksn;
    /** Indicates if GPRS authentication parameter AUTN is present */
    Boolean                   gprsAuthParamsAutnPresent;
    /** GPRS authentication parameter UTN (GSM 24.008,  10.5.3.1.1) */
    AuthParamAutnElement      gprsAuthParamAutn;
}
GprsGmmAuthCipherRequest;

/** This message is sent by the MS to the network in response to an
 * Authentication and ciphering request message.
 */
typedef struct GprsGmmAuthCipherResponseTag
{
    /** Authentication and ciphering reference number (GSM 04.08,  10.5.5.19) */
    AuthCipherRefNumElement authCipherRefNum;
    /** Indicates if Authentication Response parameter is present.
\assoc PRESENT \ref gprsAuthParamSres */
    Boolean                 gprsAuthParamSresPresent;
    /** Contains the SRES, if the authentication challenge was for
     * GSM or the RES (all or just the 4 most significant octets of)
     * if it is a UMTS authentication challenge (GSM 04.08,  10.5.3.2).
     */
    AuthParamSresElement    gprsAuthParamSres;
    /** Indicates if IMEISV is present.
\assoc PRESENT \ref imeisv */
    Boolean                 imeisvPresent;
    /** Included if requested within the corresponding authentication
     * and ciphering request message (GSM 04.08,  10.5.1.4).
     */
    MobileIdElement         imeisv;
}
GprsGmmAuthCipherResponse;

/** This message is sent by the mobile station to the network to
 * indicate that authentication of the network has failed.
 */
typedef struct GprsGmmAuthCipherFailureTag
{
    /** The cause of failure. */
    GsmCause        gmmCause;
    /** Whether or not authentication failure parameter is present. */
    Boolean         authFailureParameterPresent;
    /** Shall be sent if and only if the GMM cause was Synch failure. */
    Aut             authFailureParameter;
}
GprsGmmAuthCipherFailure;
/** Identity request message element.
 * This message is sent by the network to the mobile station to
 * request a mobile station to submit the specified identity to
 * the network.
 */
typedef struct GprsGmmIdentityRequestTag
{
    /** Identity type (GSM 04.08,  10.5.5.9) */
    IdTypeElement   idType;
    /** Whether or not force to standby is indicated (GSM 04.08,  10.5.5.7)*/
    Boolean         forceToStandby;
}
GprsGmmIdentityRequest;

/** Identity response message element.
 * This message is sent by the mobile station to the network in
 * response to an IDENTITY REQUEST message providing the requested
 * identity.
 */
typedef struct GprsGmmIdentityResponseTag
{
    /** Mobile identity (GSM 04.08,  10.5.1.4) */
    MobileIdElement    mobileId;
}
GprsGmmIdentityResponse;

/**
 * This message is sent by the MS to the network either to request
 * an update of its location file or to request an IMSI attach for
 * non-GPRS services.
 */
typedef struct GprsGmmRaUpdateRequestTag
{
    /** Update type (GSM 04.08,  10.5.5.18)*/
    UpdateTypeElement         updateType;
    /** Ciphering key sequence number (GSM 04.08,  10.5.1.2) */
    CipheringKeySeqNumElement gprsCksn;
    /** Old routing area identification (GSM 04.08,  10.5.5.12a) */
    RoutingAreaIdElement      oldRai;
    /** MS Radio Access capability */
    MsRadAccCapabilityElement msRadAccCapability;
    /** Indicates if old P-TMSI signature is present.
\assoc PRESENT \ref oldPtmsiSignature */
    Boolean                   oldPtmsiSignaturePresent;
    /** Included by the MS if it was received from the network in an ATTACH ACCEPT
     * or ROUTING AREA UPDATE ACCEPT message (GSM 04.08,  10.5.5.8).
     */
    PtmsiSignatureElement     oldPtmsiSignature;
    /** Indicates if Requested READY timer value is present.
\assoc PRESENT \ref requestedReadyTimer */
    Boolean                   requestedReadyTimerPresent;
    /**  Included if the MS wants to indicate a preferred value
     * for the READY timer *GSM 04.08,  10.5.7.3) */
    GprsTimerElement          requestedReadyTimer;
    /** Indicates if DRX parameter is present.
\assoc PRESENT \ref gprsDrxParameters */
    Boolean                   gprsDrxParametersPresent;
    /** Included if the MS changes the access network from GSM to UMTS, or the
     * MS wants to indicate new DRX parameters to the network.
     */
    GprsDrxParametersElement  gprsDrxParameters;
    /** Indicates if TMSI status is present. */
    Boolean                    tmsiStatusPresent;
    /** Included if the MS performs a combined routing area
     * update and no valid TMSI is available (GSM 24.008, 10.5.5.4).
     */
    TmsiStatusElement          tmsiStatus;
    /** Indicates if PTMSI is present. */
    Boolean                    ptmsiPresent;
    /** P-TMSI (GSM 04.08,  10.5.1.4) */
    MobileIdElement            ptmsi;
    /** Indicates if MS network capability is present. */
    Boolean                    msNetworkCapabilityPresent;
    /** Included by the MS to indicate its capabilities to the network
     * (GSM 24.008, 10.5.5.12) */
    MsNetworkCapabilityElement msNetworkCapability;
    /** Indicates if PDP context status is present. */
    Boolean                    pdpContextStatusPresent;
    /** PDP context status (GSM 24.008, 10.5.7.1) */
    PdpContextStatus           pdpContextStatus;
#if defined (UPGRADE_REL5)
    /** Included if the MS supports at least one positioning method
     * for the provision of location services (LCS) via the PS domain
     * in Gb-mode (GSM 24.008 10.5.5.22).
     */
    Int8                       psLcsCapability;
#endif
}
GprsGmmRaUpdateRequest;

/** This message is sent by the network to the MS to provide the MS
 * with GPRS mobility management related data in response to a routing
 * area update request message.
 */
typedef struct GprsGmmRaUpdateAcceptTag
{
    /** Whether or not force to standby is indicated (GSM 04.08,  10.5.5.7)*/
    Boolean               forceToStandby;
    /** Update result (GSM 04.08,  10.5.5.17) */
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
    UpdateResultInformationElement   updateResultInformation;
#else /* !(UPGRADE_3G_FDD) || !(UPGRADE_REL6) */
    UpdateResultElement   updateResult;
#endif /* !(UPGRADE_3G_FDD) || !(UPGRADE_REL6) */
    /** Periodic RA update timer (GSM 04.08,  10.5.7.3) */
    GprsTimerElement      periodicRAUpdateTimer;
    /** Routing area identification */
    RoutingAreaIdElement  rai;
    /** Indicates if P-TMSI signature is present.
\assoc PRESENT \ref ptmsiSignature */
    Boolean               ptmsiSignaturePresent;
    /** Assigns an identity to the MS's GMM context (GSM 04.08,  10.5.5.8 ).*/
    PtmsiSignatureElement ptmsiSignature;
    /** Indicates if allocated P-TMSI is present.
\assoc PRESENT \ref allocatedPtmsi */
    Boolean               allocatedPtmsiPresent;
    /** Assigns a P-TMSI to an MS in case of a GPRS or combined routing
     * area updating procedure (GSM 04.08,  10.5.1.4).
     */
    MobileIdElement       allocatedPtmsi;
    /** Indicates if MS identity is present.
\assoc PRESENT \ref mobileId */
    Boolean               mobileIdPresent;
    /** Assigns or unassigns a TMSI to a MS in case of a combined routing
     * area updating procedure (GSM 04.08,  10.5.1.4).
     */
    MobileIdElement       mobileId;
    /** Indicates if npduNumberList holds valid information
\assoc PRESENT \ref npduNumberList */
    Boolean               npduNumberListPresent;
    /**  Included in case of an inter SGSN routing area updating from
     * A/Gb mode to A/Gb mode, or inter SGSN routing area updating from
     * Iu mode to A/Gb mode, or intra SGSN routing area updating from
     * Iu mode to A/Gb mode, if there are PDP contexts that have been
     * activated in LLC acknowledged transfer mode (GSM 04.08,  10.5.5.11).
     */
    NpduNumberListElement npduNumberList;
    /** Indicates if Negotiated READY timer is present.
\assoc PRESENT \ref negotiatedReadyTimer */
    Boolean               negotiatedReadyTimerPresent;
    /** Indicates a value for the READY timer (GSM 04.08,  10.5.5.14).*/
    GprsTimerElement      negotiatedReadyTimer;
    /** Included if the combined GPRS routing area updating procedure was
     * successful for GPRS services only.
     */
    GsmCause              gmmCause;
    /** Indicates if T3302 value is present
\assoc PRESENT \ref t3302Timer */
    Boolean               t3302TimerPresent;
    /** Indicates a value for the T3302 timer */
    GprsTimerElement      t3302Timer;
    /** Indicates the ability to support the Cell Notification.*/
    Boolean               cellNotificationSupported;
    /** Included if the network wants to inform the mobile station of equivalent PLMNs.*/
    EquivalentPlmnListElement equivalentPlmnList;
#if defined (UPGRADE_REL5)
    /** Indicates if PDP context status is present */
    Boolean                    pdpContextStatusPresent;
    /** Included by the NW (GSM 24.008, 10.5.7.1) */
    PdpContextStatus           pdpContextStatus;
    /** GSM 24.008, 10.5.5.23 Network Feature Support*/
    Boolean                    networkFeatureLcsMolrSupported;
    /** GSM 24.008, 10.5.5.23 Network Feature Support*/
    Boolean                    lcsMolrSupported;
    /** Indicates if a list of emergency numbers is present */
    Boolean                    emergencyNumberListPresent;
    /** Indicates a list of emergency numbers valid within the same MCC
     * as in the cell on which this IE is received (GSM 24.008, 10.5.3.13).
     */
    EmergencyNumberList        emergencyNumberList;
#endif
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL7)
    /** GSM 24.008 10.5.5.25 Requested MS information
    ** PS HandOver not supported.
    ** Not coded yet
    \assoc PRESENT \ref interRatInfoContainerRequested */
    Boolean                   interRatInfoContainerRequestedPresent;
    Boolean                   interRatInfoContainerRequested;
    /** GSM 24.008 GPRS Timer 2
    ** Indicates if T3319 value is present
\assoc PRESENT \ref t3319Timer */
    Boolean                   t3319TimerPresent;
    /** Indicates a value for the T3302 timer.*/
    GprsTimerElement          t3319Timer;
#endif /* (UPGRADE_3G_FDD) && (UPGRADE_REL7) */
}
GprsGmmRaUpdateAccept;

/** Routing area update complete message element.
 * This message shall be sent by the MS to the network in response to a routing
 * area update accept message if at least one of the following conditions is fulfilled:<UL>
 * <LI> a P-TMSI and/or a TMSI has been assigned;
 * <LI> there are established LLC connections; or
 * <LI> the network has requested the MS to provide feature-related information.
 * <UL>
 */
typedef struct GprsGmmRaUpdateCompleteTag
{
    /** Whether or not list of received N-PDU numbers is present.
\assoc PRESENT \ref npduNumberList */
    Boolean                npduNumberListPresent;
    /** Receive N PDU Number list (GSM 04.08,  10.5.5.11) */
    NpduNumberListElement  npduNumberList;
}
GprsGmmRaUpdateComplete;

/** This message is sent by the network to the MS in order to
 * reject the routing area update procedure.
 */
typedef struct GprsGmmRaUpdateRejectTag
{
    /** The cause for rejecting the routing area update
     * procedure (GSM 04.08,  10.5.5.14) */
    GsmCause           gmmCause;
    /** Whether or not force to standby is indicated (GSM 04.08,  10.5.5.7)*/
    Boolean            forceToStandby;
    /** Indicates if T3302 value is present
\assoc PRESENT \ref t3302Timer */
    Boolean            t3302TimerPresent;
    /** Indicates a value for the T3302 timer */
    GprsTimerElement   t3302Timer;
}
GprsGmmRaUpdateReject;

#if defined (UPGRADE_3G)
/** Service request message element.
 * This message is sent by the MS to transfer to establish
 * logical association between the MS and the network.
 */
typedef struct GprsGmmServiceRequestTag
{
    /** Ciphering key sequence number */
    CipheringKeySeqNumElement cksn;
    /** Service type */
    GmmServiceType            serviceType;
    /** P-TMSI */
    MobileIdElement           ptmsi;
    /** Indicates if PDP context status is present. */
    Boolean                   pdpContextStatusPresent;
    /** Included by the MS (GSM 24.008, 10.5.7.1). */
    PdpContextStatus          pdpContextStatus;
#if defined (UPGRADE_REL7) && defined (UPGRADE_3G_FDD)
    /** Indicates if Uplink Data Status is present
\assoc PRESENT \ref uplinkDataStatus */
    Boolean                   uplinkDataStatusPresent;
    /** 24.008 10.5.7.7. This IE may be included by the MS when the
    ** Service Type is set to 'data' */
    UplinkDataStatus          uplinkDataStatus;
#endif /* (UPGRADE_REL7) && (UPGRADE_3G_FDD) */
}
GprsGmmServiceRequest;

/** This message is sent by the network to the UE in order to reject
 * the service request procedure.
 */
typedef struct GprsGmmServiceRejectTag
{
    /** The cause for rejecting the Service request procedure. */
    GsmCause           gmmCause;
}
GprsGmmServiceReject;
#if defined (UPGRADE_REL5)
/** This message is sent by the network in response to a Service Request message. */
typedef struct GprsGmmServiceAcceptTag
{
    /** Indicates PDP context status is present. */
    Boolean                    pdpContextStatusPresent;
    /** Included by the NW. */
    PdpContextStatus           pdpContextStatus;
}
GprsGmmServiceAccept;
#endif
#endif

/** This message is sent by the MS or by the network at any
 * time to report certain error conditions.
 */
typedef struct GprsGmmStatusMessageTag
{
    /** Reported error conditions (GSM 04.08,  10.5.5.14) */
    GsmCause        gmmCause;
}
GprsGmmStatusMessage;


/** This message is sent by the network at any time to sent certain information to the MS.
 * \note This type is the same as the MmInformationMessage used in circuit switched. When we
 * receive a GprsGmmInformation message in GPRS it is read out of the L3Message union as an
 * MmInformationMessage as this is the type used in the MmrNetworkInfoInd signal. Consequently
 * GprsGmmInformation and MmInformationMessage must be maintained with the same contents
 */
typedef struct GprsGmmInformationTag
{
    /** Whether or not full network name is present
\assoc PRESENT \ref fullNetworkName */
    Boolean                    fullNetworkNamePresent;
    /** Indicates full network name information (GSM 04.08,  10.5.3.5a ) */
    NetworkName                fullNetworkName;
    /** Indicates the abbreviated name of the network
\assoc PRESENT \ref shortNetworkName */
    Boolean                    shortNetworkNamePresent;
    /** Indicates short network name information (GSM 04.08,  10.5.3.5a) */
    NetworkName                shortNetworkName;
    /** Indicates if location time zone is present
\assoc PRESENT \ref locTimeZone */
    Boolean                    locTimeZonePresent;
    /** The purpose of this parameter is to encode the
     * offset between universal time and local time in steps
     * of 15 minutes (GSM 04.08,  10.5.3.8) .
     */
    LocTimeZone                locTimeZone;
    /** Whether the universal time and local time zone is present
\assoc PRESENT \ref uniTimeAndLocTimeZone */
    Boolean                    uniTimeAndLocTimeZonePresent;
    /** The purpose of the timezone is to encode the offset between universal
     * time and local time in steps of 15 minutes and the purpose of the time
     * part of this information element is to encode the universal time at which
     * this information element may have been sent by the network (GSM 04.08,  10.5.3.9)
     */
    UniTimeAndLocTimeZone      uniTimeAndLocTimeZone;
    /** Indicates if daylight saving time is present
\assoc PRESENT \ref networkDaylightSavingTime */
    Boolean                    networkDaylightSavingTimePresent;
    /** Indicates the value that has been used to adjust the local
     * time zone (GSM 24.008, 10.5.3.12) */
    NetworkDaylightSavingTime  networkDaylightSavingTime;
    /** Whether LSA is present
\assoc PRESENT \ref lsaIdentity */
    Boolean                    lsaIdentityPresent;
    /** Uniquely identifies a LSA (GSM 24.008, 10.5.3.11).*/
    Int32                      lsaIdentity;
}
GprsGmmInformation;

typedef enum GprsTestModeTag
{
    GPRS_TEST_MODE_TX_RANDOM_DATA     = 0,
    GPRS_TEST_MODE_LOOPBACK_RX_DATA   = 1
}
GprsTestMode;

/** This message is only sent in the direction SS to MS.*/
typedef struct GprsTestModeCommandTag
{
    /** GPRS test mode (See GSM 04.14,  8.13) */
    GprsTestMode     gprsTestMode;
    /** Indicates if PDU count is present. */
    Boolean          pduCountPresent;
    /** PDU count*/
    Int16            pduCount;
    /** Time slot offset for downlink */
    Int8             downlinkTimeslotOffset;
}
GprsTestModeCommand;

#if defined (UPGRADE_EDGE)
typedef enum EgprsSwitchedLoopbackModeTag
{
    EGPRS_SYM_RAD_BLOCK_LOOPBACK_MODE = 0,
    EGPRS_ASYM_RAD_BLOCK_LOOPBACK_MODE = 1
}
EgprsSwitchedLoopbackMode;

typedef struct EgprsStartLoopbackCmdTag
{
    /* See GSM 04.14,  8.14 */
    EgprsSwitchedLoopbackMode egprsSwitchedLoopbackMode;
    Int8                      downlinkTimeslotOffset;
}
EgprsStartLoopbackCmd;
#endif

/** This message is sent on the PCCCH or PACCH by the network to
 * the mobile station to assign downlink resources to the mobile station
 */
typedef struct PacketDnlinkAssignmentTag
{
    /** Indicates the medium access method to be used during the TBF */
    MacMode                     macMode;
    /** Set to TRUE if RLC unacknowledged mode and set to FALSE if RLC acknowledged mode */
    Boolean                     rlcUnackMode;
    /** This field shall be set to '1' if the network establishes a new downlink TBF
     * for the mobile station whose timer T3192 is running. Otherwise this field shall
     * be set to '0'
     */
    Boolean                     controlAck;
    /** Indicates the timeslots for use during a TBF or the timeslots carrying a PCCCH */
    Int8                        timeslotAllocation;
    /** describes the timing advance mode and timing advance value assigned to the mobile station.*/
    PacketTimingAdvance         packetTa;
    /** Indicates if BTS output power parameters are present
\assoc PRESENT \ref btsOutputPowerParam */
    Boolean                     btsOutputPowerParamPresent;
    /** BTS output power parameters */
    BtsOutputPowerParam         btsOutputPowerParam;
    /** Indicates if frequency parameters are present
\assoc PRESENT \ref freqParam */
    Boolean                     freqParamPresent;
    /** Defines frequency parameters and a training sequence code  */
    FrequencyParameters         freqParam;
    /** Indicates if downlink TFI is present
\assoc PRESENT \ref dnlinkTfi */
    Boolean                     dnlinkTfiPresent;
    /** This information element, if present, assigns the TFI to the mobile station
     * to identify the downlink TBF described by this message
     */
    Int8                        dnlinkTfi;
    /** Indicates if power control is present
\assoc PRESENT \ref powerCtrl */
    Boolean                     powerCtrlPresent;
    /** Contains parameters the mobile station shall use to determine its TX power level. */
    PowerControlParams          powerCtrl;
    /** Contains a starting time that indicates the TDMA frame number during which the
     * assigned TBF may start.
     */
    StartFrameNumber            tbfStartTime;
    /** Measurement mapping */
    MeasMap                     measMap;
    /** Indicates if Packet Extended Timing Advance is present */
    Boolean                     packetExtTaPresent;
    /** Packet Extended Timing Advance.
     * The Packet Extended Timing Advance field is a 2 bit field used to
     * support Extended Timing Advance. These two bits represent the two
     * most significant bits of the timing advance value to be applied by
     * the mobile station.
     */
    Int8                        extendedTa;
#if defined (UPGRADE_EDGE)
    PdchStructure               pdchStructure;
    /** Indicates if EGPRS parameters are present */
    Boolean                     egprsParamPresent;
    /** This information element defines the window size to be used in an EGPRS TBF.
     * The network sets the window size according to the number of timeslots allocated
     * in the direction of the TBF.
     */
    Int8                        egprsWindowSize;
    /** This field determines the measurements to be included within the EGPRS Timeslot
     * Link Quality Measurements IE
     */
    Int8                        linkQualMeasMode;
    /** A constant which is used for filtering channel quality measurements in EGPRS;
     * set to 0xFF when not present
     */
    Int8                        bepPeriod2;
#endif
}
PacketDnlinkAssignment;


typedef union PdchAllocationUnionTag
{
    AllocationUnion             uplink;
    PacketDnlinkAssignment      dnLink;
}
PdchAllocationUnion;

typedef struct DcchTbfAllocationTag
{
    CodingScheme                codingScheme;
    CodingScheme                tlliScheme;
    Boolean                     isItDownlink;
    PacketTimingAdvance         packetTa;
/** \assoc UNION \ref alloc */
    AllocationType              allocType;
    PdchAllocationUnion         alloc;
}
DcchTbfAllocation;

/** GPRS SUSPENSION REQUEST message content.
 * Used to request a suspension of GPRS services.
 */
typedef struct GprsSuspensionRequestMsgTag
{
    /** The purpose of the TLLI information element is to provide the Temporary Logical Link Identifier.*/
    Int32   tlli;
    /** Routeing Area Identification */
    Rai     rai;
    /** The purpose of the Suspension Cause information element is to provide the reason for the establishment
     * of the dedicated circuit which generated the GPRS suspension.
     */
    Int8    suspensionCause;
}
GprsSuspensionRequestMsg;

typedef struct PsiCbchChanDescriptionTag
{
    Int8                            chanTypeAndTdmaOffset;
    Int8                            timeslot;
    FrequencyParameters             freqParam;
}
PsiCbchChanDescription;

/** Network control measurement parameters */
typedef struct NcMeasurementParamTag
{
    /** Network Control */
    NetworkControl          nc;
    /** Reporting period in non-DRX mode */
    Int8                    ncNonDrxPeriod;
    /** Reporting period when in packet idle mode */
    Int8                    ncReportingPeriodI;
    /** Reporting period when in packet transfer mode */
    Int8                    ncReportingPeriodT;
    /** Enhanced or Normal measurement report */
    MeasReportType          reportType;
}
NcMeasurementParam;

typedef struct EnhMeasurementParamTag
{
    Boolean                     pmoInd;
    Boolean                     isChangeMark; /* TRUE then use the psi3ChangeMark field. FALSE then use baIndParam field */
    Boolean                     baInd;
    Boolean                     baInd3g;
    Int8                        psi3ChangeMark;

    MeasReportType              reportType;
    MeasReportRate              reportRate;
    Boolean                     invalidBsicReporting;

    Boolean                     nccPermittedPresent;
    Int8                        nccPermitted;

    Boolean                     gprsMeasParamPresent;

    ReportPriorityParam         reportPriority;
    MeasurementParameters       gprsMeasParam;
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    Boolean                     ncell3gDescrPresent;
    Boolean                     gprs3gMeasParamPresent;
    Boolean                     rem3gNcellDescrPresent;

#if defined (UPGRADE_3G_FDD)
    Boolean                     utranFddNcellParamPresent;
    UtranFddNcellParam          utranFddNcellParam;
#endif /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)
    Boolean                     utranTdd128NcellParamPresent;
    UtranTdd128NcellParam       utranTdd128NcellParam;
#endif /* UPGRADE_3G_TDD128 */

    /* The CDMA 2000 description is skipped over so there is no storage for the associated parameters */
    Removed3gNcellDescription   rem3gNcellDescr;     /* End of 3G ncell descr */
    Gprs3gMeasurementParam      gprs3gMeasParam;
#endif
}
EnhMeasurementParam;

typedef struct Psi5EnhMeasurementParamTag
{
    Boolean                     valid;
    MeasReportType              reportType;
    MeasReportRate              reportRate;
    Boolean                     invalidBsicReporting;

    Boolean                     nccPermittedPresent;
    Int8                        nccPermitted;

    Boolean                     gprsMeasParamPresent;
    MeasurementParameters       gprsMeasParam;

#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    Boolean                     gprs3gMeasParamPresent;
    Gprs3gMeasurementParam      gprs3gMeasParam;
#endif
}
Psi5EnhMeasurementParam;

#endif /* L3GP_TYP_H */
/* END OF FILE */

