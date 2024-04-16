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
 *      GPRS data structure typedefs for 04.60 section 12
 *      Information Elements. Also contains stuff which is
 *      generic to 2 or more of the following utility files, but which
 *      is not defined to be an Information Element in 04.60:
 *
 *          gpdecrlc:   rlc/mac downlink decode
 *          gpdecgrr:   grr downlink decode
 *          gpencrlc:   rlc/mac uplink encode
 **************************************************************************/

#ifndef GPIE_TYP_H
#define GPIE_TYP_H


/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <l3gp_typ.h>
#include <gpgentyp.h>
#include <l123_typ.h> /* for reduced frame number */

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

#define MAX_LSA_IDS                         5
#define NUM_OF_RADIO_PRIORITIES             4

#define MAX_NC_MEAS             MAX_NUM_MEASUREMENTS
#define MAX_EXT_MEAS            MAX_NUM_MEASUREMENTS

#define INVALID_CONTAINER_ID       0xff
#define MAX_CONTAINER_OCTET_LENGTH   18

/***************************************************************************
 * Types
 **************************************************************************/

typedef struct WaitIndicationTag
{
/** \assoc PRESENT \ref codedInSeconds
\assoc PRESENT \ref value */
    Boolean     present;
    Boolean     codedInSeconds;  /* if FALSE the value is coded */
                                /* in units of 20 milliseconds */
    Int8        value;
}
WaitIndication;

typedef struct GprsCellIdentificationTag
{
    Lai                         lai;
    Rac                         rac;
    CellId                      cellId;
}
GprsCellIdentification;

typedef enum DecodeResultTag
{
    /* return codes used by decode utilities: */
    GPDEC_SUCCESS_MORE_DATA_FOLLOWS       = 0,
    GPDEC_SUCCESS_NO_MORE_DATA            = 1,
    GPDEC_DISTRIBUTION_PART_ERROR         = 2,
    GPDEC_ADDRESS_INFO_PART_ERROR         = 3,
    GPDEC_NON_DISTRIBUTION_PART_ERROR     = 4,
    GPDEC_MESSAGE_ESCAPE_ERROR            = 5,
    GPDEC_IGNORE_ERROR                    = 6,
    GPDEC_ERROR_IN_TRUNCATION             = 7,
    GPDEC_MESSAGE_TOO_LONG                = 8,
    GPDEC_UNSUPPORTED_IE                  = 9,
    LAST_GPDEC_DECODE_RESULT
}
DecodeResult;

typedef enum ExtReportTypeTag
{
    TYPE_1_MEAS_REPORT                    = 0,
    TYPE_2_MEAS_REPORT                    = 1,
    TYPE_3_MEAS_REPORT                    = 2,
    RESERVED_MEAS_REPORT                  = 3
}
/*
 * Values in this enum must match the values in the EXT_REPORTING_TYPE
 * field of the Packet Measurement Report message - see 04.60 11.2.9
 */
ExtReportType;

/** Specifies the type of address used */
typedef enum AddressTypeTag
{
    /** Uplink TFI */
    GPRS_UPLINK_TFI                       = 0x01,
    /** Downlink TFI */
    GPRS_DNLINK_TFI                       = 0x02,
    /** TLLI.
     * The purpose of the TLLI information element is to provide 
     * the Temporary Logical Link Identifier.
     */
    GPRS_TLLI                             = 0x04,
    /** Temporary Queueing Identifier.
     * The Temporary Queueing Identifier (TQI) field identifies a 
     * mobile station during the queueing procedure. 
     * The contents of this field are operator defined.
     */
    GPRS_TQI                              = 0x08,
    /** Packet Request Reference.
     * The purpose of the Packet Request Reference information element is 
     * to provide the information field sent in the Packet Channel Request (
     * i.e., the PACKET CHANNEL REQUEST or EGPRS PACKET CHANNEL REQUEST message) 
     * and the frame number, FN modulo 42432, in which the Packet Channel Request 
     * was received.
     */
    GPRS_PACKET_REQUEST_REF               = 0x10,
    /** NULL ID */
    GPRS_NULL_ID                          = 0x80
}
AddressType;

/** Specifies the cause for rejecting the packet */
typedef enum PacketAccessRejectCauseTag
{
    /** The WAIT_INDICATION field is coded in units of seconds */
    WAIT_IN_SECONDS                         = 0,
    /** The WAIT_INDICATION field is coded in units of 20 milliseconds  */
    WAIT_IN_20_MILLISECONDS                 = 1
}
PacketAccessRejectCause;

/** Packet request reference elements */
typedef struct PacketReqRefTag
{
    /** Random access information */
    Int16                       randomAccessInfo;
    /** The frame number; FN modulo 42432 */
    Int16                       frameNumber;
}
PacketReqRef;

/** Defines the address used */
typedef union AddressUnionTag
{
    /** Uplink TFI */
    Int8                        uplinkTfi;
    /** Downlink TFI */
    Int8                        dnlinkTfi;
    /** TLLI.
     * The purpose of the TLLI information element is to provide 
     * the Temporary Logical Link Identifier.
     */
    Int32                       tlli;
    /** Temporary Queueing Identifier.
     * The Temporary Queueing Identifier (TQI) field identifies a 
     * mobile station during the queueing procedure. 
     * The contents of this field are operator defined.
     */
    Int16                       tqi;
     /** Packet Request Reference.
     * The purpose of the Packet Request Reference information element is 
     * to provide the information field sent in the Packet Channel Request (
     * i.e., the PACKET CHANNEL REQUEST or EGPRS PACKET CHANNEL REQUEST message) 
     * and the frame number, FN modulo 42432, in which the Packet Channel Request 
     * was received.
     */
    PacketReqRef                pktRef;
}
AddressUnion;

/** Indicates the packet addressed to which MS */
typedef struct AddressTag
{
    /** The type of address used 
\assoc UNION \ref address */
    AddressType                 addressType;
    /** Holds the address */
    AddressUnion                address;
}
Address;

typedef struct PrachControlParamTag
{
    Int16                       accessControl;
    Int8                        maxRetrans[NUM_OF_RADIO_PRIORITIES];
    Int8                        s;
    Int8                        txInt;
/** \assoc PRESENT \ref persistenceLevel */
    Boolean                     persistencePresent;
    Int8                        persistenceLevel[NUM_OF_RADIO_PRIORITIES];
}
PrachControlParam;

typedef struct PccchOrganisationTag
{
    Int8                        bsPccRel;
    Int8                        bsPbcchBlks;
    Int8                        bsPagBlksRes;
    Int8                        bsPrachBlks;
}
PccchOrganisation;

typedef struct NonGprsCellOptionsTag
{
    Boolean                     attachAllowed;
    Int8                        t3212;
    Boolean                     neci;
    Boolean                     pwrc;
    DtxIndicator                dtx;
    Int8                        radioLinkTimeout;
    Int8                        bsAgBlksRes;
    Int8                        ccchConf;
    Int8                        bsPaMfrms;
    Int8                        maxRetrans;
    Int8                        txInteger;
    Boolean                     ecBit;
    Int8                        msTxpwrMaxCcch;
    Boolean                     ecsc;
    Boolean                     ecsr3g;
}
NonGprsCellOptions;

/** Neighbour cell LSA parameters */
typedef struct NcellLsaInfoTag
{
    /** Number of LSA */
    Int8                        numLsa;
    /** Number of neighbour cell index */
    Int8                        ncellIndex [MAX_LSA_IDS];
    /** If TRUE, shortLsaId is used instead of LsaId*/
    Boolean                     shortLsaId[MAX_LSA_IDS];  
    /** The purpose of this field is to identify a LSA */
    Int32                       lsaId [MAX_LSA_IDS];
}
NcellLsaInfo;

#endif
