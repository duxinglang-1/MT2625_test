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
 * File Description:                                                     */
/** \file
 * SNDCP data definition - this contains state information for each NSAPI
 * as well as global SNDCP variables
 **************************************************************************/

#ifndef GPQOS_H
#define GPQOS_H

/**** NESTED INCLUDE FILES *************************************************/

/**** CONSTANTS ************************************************************/

#if defined (UPGRADE_LTE)
#define MIN_QCI        0
#define MAX_QCI        9

/* Maximum bits rates permissible. 
  */
#if defined (LTE_CATEGORY_4)
/* Category 4 settings */
#define MAX_DL_GBR     150000   /* In Kbps */
#define MAX_UL_GBR     50000    /* In Kbps */
#define MAX_DL_MBR     150000   /* In Kbps */
#define MAX_UL_MBR     50000    /* In Kbps */
#else
/* Default to category 3 */
#define MAX_DL_GBR     100000   /* In Kbps */
#define MAX_UL_GBR     50000    /* In Kbps */
#define MAX_DL_MBR     100000   /* In Kbps */
#define MAX_UL_MBR     50000    /* In Kbps */
#endif

#endif /* UPGRADE_LTE */

/**** TYPEDEFS *************************************************************/

/** Delay class element(GSM 04.08/10.5.6.5); see also GSM 02.60
*/
typedef enum DelayClassTag
{
    /** Subscribed delay class */
    GPRS_DELAY_CLASS_SUBSCRIBED = 0,
    /** Delay class 1 (highest quality)*/
    GPRS_DELAY_CLASS_1 = 1,
    /** Delay class 2*/
    GPRS_DELAY_CLASS_2 = 2,
    /** Delay class 3 (lowest  quality)*/
    GPRS_DELAY_CLASS_3 = 3,
    /** Delay class 4 (best effort)*/
    GPRS_DELAY_CLASS_4 = 4,
    /** Reserved */
    GPRS_DELAY_CLASS_RESERVED = 7
}
DelayClass;


/** Reliability class element.
 *(ref. GSM 04.08/10.5.6.5).
 * Data reliability is defined in terms of the residual error rates for the
 * following cases (see GSM 02.60):<UL>
 * <LI> probability of data loss;
 * <LI> probability of data delivered out of sequence;
 * <LI> probability of duplicate data delivery; and
 * <LI> probability of corrupted data. </UL>
*/

typedef enum ReliabilityClassTag                        /*  acked  acked  acked  prot  */
{                                                       /*   GTP    LLC    RLC    LLC  */
    /** Subscribed reliability class */
    GPRS_RELIAB_CLASS_SUBSCRIBED = 0,
    /** Acknowledged GTP, LLC, and RLC;
      * Protected data */
    GPRS_RELIAB_CLASS_1 = 1,                            /*    y      y      y      y   */
    /** Unacknowledged GTP; Acknowledged
      * LLC and RLC, Protected data */
    GPRS_RELIAB_CLASS_2 = 2,                            /*    n      y      y      y   */
    /** Unacknowledged GTP and LLC; Acknowledged
      * RLC, Protected data */
    GPRS_RELIAB_CLASS_3 = 3,                            /*    n      n      y      y   */
    /** Unacknowledged GTP, LLC, and RLC,
     * Protected data */
    GPRS_RELIAB_CLASS_4 = 4,                            /*    n      n      n      y   */
    /** Unacknowledged GTP, LLC, and RLC,
      * Unprotected data */
    GPRS_RELIAB_CLASS_5 = 5,                            /*    n      n      n      n   */
    /** Reserved */
    GPRS_RELIAB_CLASS_RESERVED = 7
}
ReliabilityClass;


/** The peak throughput is measured at the Gi and R reference points in units of
 * octets per second. It specifies the maximum rate at which data is expected
 * to be transferred across the network for an individual PDP context. There is
 * no guarantee that this peak rate can be achieved or sustained for any time
 * period. In the MS this value may correspond to the serial data rate from a PC.
 *(ref. GSM 04.08/10.5.6.5)
*/
typedef enum PeakThroughputTag
{
    /** Subscribed peak throughput */
    GPRS_PEAK_THRPT_SUBSCRIBED = 0,
    /** Up to 1 000 octet/s */
    GPRS_PEAK_THRPT_UPTO_1KOCT = 1,
    /** Up to 2 000 octet/s */
    GPRS_PEAK_THRPT_UPTO_2KOCT = 2,
    /** Up to 4 000 octet/s */
    GPRS_PEAK_THRPT_UPTO_4KOCT = 3,
    /** Up to 8 000 octet/s */
    GPRS_PEAK_THRPT_UPTO_8KOCT = 4,
    /** Up to 16 000 octet/s */
    GPRS_PEAK_THRPT_UPTO_16KOCT = 5,
    /** Up to 32 000 octet/s */
    GPRS_PEAK_THRPT_UPTO_32KOCT = 6,
    /** Up to 64 000 octet/s */
    GPRS_PEAK_THRPT_UPTO_64KOCT = 7,
    /** Up to 128 000 octet/s */
    GPRS_PEAK_THRPT_UPTO_128KOCT = 8,
    /** Up to 256 000 octet/s */
    GPRS_PEAK_THRPT_UPTO_256KOCT = 9,
    /** Reserved */
    GPRS_PEAK_THRPT_RESERVED = 0x0f,
    /** The highest peak throughput */
    GPRS_PEAK_THRPT_HIGHEST = GPRS_PEAK_THRPT_UPTO_256KOCT
}
PeakThroughput;


/** Indicates the relative priority of maintaining the service.
 * The precedence class indicates the relative importance of maintaining the
 * service commitments under abnormal conditions, for example which packets are
 * discarded in the event of problems such as limited resources or network
 * congestion. (ref. GSM 04.08/10.5.6.5)
*/
typedef enum PrecedenceClassTag
{
    /** Subscribed precedence */
    GPRS_PRECED_CLASS_SUBSCRIBED = 0,
    /** High priority */
    GPRS_PRECED_CLASS_1 = 1,
    /** Normal priority */
    GPRS_PRECED_CLASS_2 = 2,
    /** Low priority */
    GPRS_PRECED_CLASS_3 = 3,
    /** Reserved */
    GPRS_PRECED_CLASS_RESERVED = 7
}
PrecedenceClass;


/** The mean throughput is measured at the R reference point in units of octets
 * per hour. It specifies the average rate at which data is expected to be
 * transferred across the GPRS network during the remaining lifetime of an
 * activated PDP context.(ref. GSM 04.08/10.5.6.5)
 */

typedef enum MeanThroughputTag
{
    /** Subscribed mean throughput */
    GPRS_MEAN_THRPT_SUBSCRIBED  = 0,
    /** 100 octet/h */
    GPRS_MEAN_THRPT_100_OPH     = 1,
    /** 200 octet/h */
    GPRS_MEAN_THRPT_200_OPH     = 2,
    /** 500 octet/h */
    GPRS_MEAN_THRPT_500_OPH     = 3,
    /** 1 000 octet/h */
    GPRS_MEAN_THRPT_1K_OPH      = 4,
    /** 2 000 octet/h */
    GPRS_MEAN_THRPT_2K_OPH      = 5,
    /** 5 000 octet/h */
    GPRS_MEAN_THRPT_5K_OPH      = 6,
    /** 10 000 octet/h */
    GPRS_MEAN_THRPT_10K_OPH     = 7,
    /** 20 000 octet/h */
    GPRS_MEAN_THRPT_20K_OPH     = 8,
    /** 50 000 octet/h */
    GPRS_MEAN_THRPT_50K_OPH     = 9,
    /** 100 000 octet/h */
    GPRS_MEAN_THRPT_100K_OPH    = 10,
    /** 200 000 octet/h */
    GPRS_MEAN_THRPT_200K_OPH    = 11,
    /** 500 000 octet/h */
    GPRS_MEAN_THRPT_500K_OPH    = 12,
    /** 1 000 000 octet/h */
    GPRS_MEAN_THRPT_1M_OPH      = 13,
    /** 2 000 000 octet/h */
    GPRS_MEAN_THRPT_2M_OPH      = 14,
    /** 5 000 000 octet/h */
    GPRS_MEAN_THRPT_5M_OPH      = 15,
    /** 10 000 000 octet/h */
    GPRS_MEAN_THRPT_10M_OPH     = 16,
    /** 20 000 000 octet/h */
    GPRS_MEAN_THRPT_20M_OPH     = 17,
    /** 50 000 000 octet/h */
    GPRS_MEAN_THRPT_50M_OPH     = 18,
    /** Reserved */
    GPRS_MEAN_THRPT_RESERVED    = 30,
    /** Best effort */
    GPRS_MEAN_THRPT_BEST_EFFORT = 31
}
MeanThroughput;


/** Delivery of erroneous SDUs element.
 * Used to decide whether error detection is needed and whether
 * frames with detected errors shall be forwarded or not(ref. 24.008/10.5.6.5).
 * \note <B> yes </B> implies that error detection is employed and that erroneous
 * SDUs are delivered together with an error indication, <B> no </B> implies that
 * error detection is employed and that erroneous SDUs are discarded, and <B> - </B>
 * implies that SDUs are delivered without considering error detection.
 */
typedef enum DeliveryOfErroneousSduTag
{
    /** Subscribed delivery of erroneous SDUs */
    GPRS_DLV_ERROR_SDU_SUBSCRIBED  = 0,
    /** No detect (-)  */
    GPRS_DLV_ERROR_SDU_NODETECT    = 1,
    /** Erroneous SDUs are delivered (yes) */
    GPRS_DLV_ERROR_SDU_YES         = 2,
    /** Erroneous SDUs are not delivered (no) */
    GPRS_DLV_ERROR_SDU_NO          = 3,
    /** Reserved */
    GPRS_DLV_ERROR_SDU_RESERVED    = 7
}
DeliveryOfErroneousSdu;

/** Delivery order element.
 * Indicates whether the UMTS bearer shall provide in-sequence SDU delivery or not.
 */
typedef enum DeliveryOrderTag
{
    /** Subscribed delivery order */
    GPRS_DLV_ORDER_SUBSCRIBED  = 0,
    /** With delivery order (yes)*/
    GPRS_DLV_ORDER_WITH        = 1,
    /** Without delivery order (no)*/
    GPRS_DLV_ORDER_WITHOUT     = 2,
    /** Reserved */
    GPRS_DLV_ORDER_RESERVED    = 3
}
DeliveryOrder;

/** Traffic class element */
typedef enum TrafficClassTag
{
    /** Subscribed class */
    GPRS_TRAFFIC_CLASS_SUBSCRIBED       = 0,
    /** Conversational class */
    GPRS_TRAFFIC_CLASS_CONVERSATIONAL   = 1,
    /** Streaming Class */
    GPRS_TRAFFIC_CLASS_STREAMING        = 2,
    /** Interactive Class */
    GPRS_TRAFFIC_CLASS_INTERACTIVE      = 3,
    /** Background Class */
    GPRS_TRAFFIC_CLASS_BACKGROUND       = 4,
    /** Reserved Class */
    GPRS_TRAFFIC_CLASS_RESERVED         = 7
}
TrafficClass;

#define GPRS_MAX_SDU_SIZE_SUBSCRIBED    0
#define GPRS_MAX_SDU_SIZE_RESERVED      0xFF

/*
Used for maxBitRateUplink, maxBitRateDownlink, guaranteedBitRateUplink, and
guaranteedBitRateDownlink.
*/
#define GPRS_BIT_RATE_SUBSCRIBED        0


/** Residual Bit Error Rate (BER)element.
 * Indicates the undetected bit error ratio in the delivered SDUs.
 * If no error detection is requested, Residual bit error ratio indicates
 * the bit error ratio in the delivered SDUs.
 */
typedef enum ResidualBERTag
{
    /** Subscribed residual BER */
    GPRS_RESIDUAL_BER_SUBSCRIBED  = 0,
    GPRS_RESIDUAL_BER_5102        = 1,    /**< 5 * (10 POW (-2)) */
    GPRS_RESIDUAL_BER_1102        = 2,    /**< 1 * (10 POW (-2)) */
    GPRS_RESIDUAL_BER_5103        = 3,    /**< 5 * (10 POW (-3)) */
    GPRS_RESIDUAL_BER_4103        = 4,    /**< 4 * (10 POW (-3)) */
    GPRS_RESIDUAL_BER_1103        = 5,    /**< 1 * (10 POW (-3)) */
    GPRS_RESIDUAL_BER_1104        = 6,    /**< 1 * (10 POW (-4)) */
    GPRS_RESIDUAL_BER_1105        = 7,    /**< 1 * (10 POW (-5)) */
    GPRS_RESIDUAL_BER_1106        = 8,    /**< 1 * (10 POW (-6)) */
    GPRS_RESIDUAL_BER_6108        = 9,    /**< 6 * (10 POW (-8)) */
    /** Reserved */
    GPRS_RESIDUAL_BER_RESERVED    = 15
}
ResidualBER;

/** Indicates the fraction of SDUs lost or detected as erroneous
 * (SDU error ratio is defined only for conforming traffic).
 */
typedef enum SduErrorRatioTag
{
    /** Subscribed SDU error ratio */
    GPRS_SDU_ERROR_RATIO_SUBSCRIBED  = 0,
    /** 1 * (10 POW (-2)) */
    GPRS_SDU_ERROR_RATIO_1102        = 1,
    /** 7 * (10 POW (-3)) */
    GPRS_SDU_ERROR_RATIO_7103        = 2,
    /** 1 * (10 POW (-3)) */
    GPRS_SDU_ERROR_RATIO_1103        = 3,
    /** 1 * (10 POW (-4)) */
    GPRS_SDU_ERROR_RATIO_1104        = 4,
    /** 1 * (10 POW (-5)) */
    GPRS_SDU_ERROR_RATIO_1105        = 5,
    /** 1 * (10 POW (-6)) */
    GPRS_SDU_ERROR_RATIO_1106        = 6,
    /** 1 * (10 POW (-1)) */
    GPRS_SDU_ERROR_RATIO_1101        = 7,
    /** Reserved */
    GPRS_SDU_ERROR_RATIO_RESERVED    = 15
}
SduErrorRatio;

/** Traffic handling priority element.
 * Within the interactive class, there is a definite need to differentiate
 * between bearer qualities. This is handled by using the traffic handling
 * priority attribute, to allow UMTS to schedule traffic accordingly.
 * By definition, priority is an alternative to absolute guarantees, and
 * thus these two attribute types cannot be used together for a single bearer
 */
typedef enum TrafficHandlingPriorityTag
{
    /** Subscribed traffic handling priority */
    GPRS_TRAFFIC_PRIORITY_LEVEL_SUBSCRIBED  = 0,
    /** Priority level 1 */
    GPRS_TRAFFIC_PRIORITY_LEVEL_1           = 1,
    /** Priority level 2 */
    GPRS_TRAFFIC_PRIORITY_LEVEL_2           = 2,
    /** Priority level 3 */
    GPRS_TRAFFIC_PRIORITY_LEVEL_3           = 3
}
TrafficHandlingPriority;

#define GPRS_TRANSFER_DELAY_SUBSCRIBED  0
#define GPRS_TRANSFER_DELAY_RESERVED    0x3F


#if defined (UPGRADE_REL5)
/** Specifies characteristics of the source of submitted SDUs.*/
typedef enum SourceStatisticDiscriptorTag
{
    /** unknown */
    SOURCE_STATISTIC_UNKNOWN   = 0,
    /** speech */
    SOURCE_STATISTIC_SPEECH    = 1
}
SourceStatisticDescriptor;
#endif

/** Specify the QoS parameters for a PDP context.
 * This structure specifies the requested or negotiated quality of service for a
 * SAPI at the LLC layer.
 */
typedef struct QualityOfServiceTag
{
    /** This parameter notifies that R97 parameters of the QoS
     * structure have values different than subscribed
\assoc PRESENT \ref delayClass
\assoc PRESENT \ref reliabilityClass
\assoc PRESENT \ref peakThroughput
\assoc PRESENT \ref precedenceClass
\assoc PRESENT \ref meanThroughput */
    Boolean r97QualityOfServicePresent;
    /** defines the end-to-end transfer delay
     * incurred in the transmission of SDUs through the GPRS network(s).
     */
    DelayClass delayClass;
    /** Defines the probability of loss of, duplication of,
      * mis-sequencing of  or corruption of SDUs.
      */
    ReliabilityClass reliabilityClass;
    /** Maximum user data throughput */
    PeakThroughput peakThroughput;
    /** Indicates the relative priority of maintaining the service*/
    PrecedenceClass precedenceClass;
    /** Includes, for example for bursty transmissions, the periods
      * in which no data is transmitted.
      */
    MeanThroughput meanThroughput;
    /** This parameter notifies that R99 parameters of the QoS
     * structure have values different than subscribed
\assoc PRESENT \ref trafficClass
\assoc PRESENT \ref deliveryOrder
\assoc PRESENT \ref deliveryOfErrSdu
\assoc PRESENT \ref maxSduSize
\assoc PRESENT \ref maxBitRateUplink
\assoc PRESENT \ref maxBitRateDownlink
\assoc PRESENT \ref residualBER
\assoc PRESENT \ref sduErrorRatio
\assoc PRESENT \ref transferDelay
\assoc PRESENT \ref trafficPriority
\assoc PRESENT \ref guaranteedBitRateUplink
\assoc PRESENT \ref guaranteedBitRateDownlink */
    Boolean r99QualityOfServicePresent;
    /** Traffic class */
    TrafficClass  trafficClass;
    /** Specifies if out-of-sequence SDUs are acceptable or not. */
    DeliveryOrder deliveryOrder;
    /** Indicates whether SDUs detected as erroneous shall be delivered or discarded.*/
    DeliveryOfErroneousSdu deliveryOfErrSdu;
    /** Maximum Size of an SDU */
    Int16 maxSduSize;
    /** Maximum bit rate for uplink */
    Int16 maxBitRateUplink;
    /** Maximum bit rate for downlink */
    Int16 maxBitRateDownlink;
    /** Indicates the undetected bit error ratio in the delivered SDUs. */
    ResidualBER residualBER;
    /** Indicates the fraction of SDUs lost or detected as erroneous */
    SduErrorRatio sduErrorRatio;
    /** Indicates maximum delay for 95th percentile of the distribution
     * of delay for all delivered SDUs during the lifetime of a bearer
     * service, where delay for an SDU is defined as the time from a
     * request to transfer an SDU at one SAP to its delivery at the other SAP.
     */
    Int16 transferDelay;
    /** specifies the relative importance for handling of all
     * SDUs belonging to the UMTS bearer compared to the SDUs
     * of other bearers.
     */
    TrafficHandlingPriority trafficPriority;
    /** Guaranteed bit rate for uplink */
    Int16 guaranteedBitRateUplink;
    /** Guaranteed bit rate for downlink */
    Int16 guaranteedBitRateDownlink;
#if defined (UPGRADE_REL5)
    /** Indicates if release 5 QoS is present */
    Boolean rel5QualityOfServicePresent;
    /** Specifies characteristics of the source of submitted SDUs.*/
    SourceStatisticDescriptor sourceStatisticDescriptor;
    /** Indicates if PDP is optimised for signalling */
    Boolean pdpOptimisedForSignalling;
#endif
#if defined (UPGRADE_LTE)
    /** This parameter indicates if the EPS QoS is present. */
    Boolean     epsQualityOfServicePresent;
    /** Quality of Service Class Identifier (QCI) */
    Int8        epsQci;
    /** This parameter indicates if the bit rate fields are present. */
    Boolean     epsBitRatesPresent;
    /** Maximum bit rate for uplink in kbps (for EPS) */
    Int32       epsMaxBitRateUplink;
    /** Maximum bit rate for downlink in kbps (for EPS) */
    Int32       epsMaxBitRateDownlink;
    /** Guaranteed bit rate for uplink in kbps (for EPS) */
    Int32       epsGuaranteedBitRateUplink;
    /** Guaranteed bit rate for downlink in kbps (for EPS) */
    Int32       epsGuaranteedBitRateDownlink;
#endif
}
QualityOfService;

/**

The radio priority level indicates to RLC/MAC what priority shall be used for a
particular NSAPI. Level 1 is the highest priority.

Although not part of the QoS profile, radio priority level is closely
associated with QoS. (ref. GSM 04.08/10.5.7.2)
*/

typedef enum RadioPriorityLevelTag
{
    /** Radio priority level 1*/
    GPRS_RPL_1 = 1,
    /** Radio priority level 2*/
    GPRS_RPL_2 = 2,
    /** Radio priority level 3*/
    GPRS_RPL_3 = 3,
    /** Radio priority level 4*/
    GPRS_RPL_4 = 4
}
RadioPriorityLevel;

/** The Packet Flow Identifier (PFI) indicates the Packet Flow Identifier
 * for a Packet Flow Context used in the network; although not part of the
 * QoS profile, Packet Flow Identifier is closely associated with QoS
 * (ref 3GPP 24.008/10.5.6.11).
 */
typedef Int8 PacketFlowId;

#define GPRS_PFI_BEST_EFFORT 0
#define GPRS_PFI_SIGNALING   1
#define GPRS_PFI_SMS         2
          /* PFI values from 3 to   7 are reserved and
           * the values from 8 to 127 are dynamically assigned */
#define GPRS_PFI_NOT_PRESENT   0xff


#if defined (UPGRADE_REL5)
/* max/guaranteed bit rate for downlink (extended) ie. 16000 kbps in 100 kbps increments*/
/* section 10.5.6.5 of 24.008 */
#define MAX_BIT_RATE_DOWNLINK_EXTENDED   74     /* 0 1 0 0 1 0 1 0 */
#define MAX_DOWNLINK_BIT_RATE            0xFE   /* 1 1 1 1 1 1 1 0 */
#endif

#if defined (UPGRADE_LTE)
/** APN aggregate maximum bit rate */
typedef struct ApnAmbrTag
{
    /** APN-AMBR for downlink */
    Int32   apnAmbrDownlink;
    /** APN-AMBR for uplink */
    Int32   apnAmbrUplink;
}
ApnAmbr;
#endif

#endif

/* END OF FILE */
