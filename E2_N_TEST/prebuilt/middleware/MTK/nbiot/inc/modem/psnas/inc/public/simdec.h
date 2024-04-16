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
 *  Header file for encode and decode procedures.
 **************************************************************************/

#ifndef SIMDEC_H
#define SIMDEC_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#if !defined (SIMDATA_H)
#include <simdata.h>
#endif

#if !defined (TS_SIG_H)
#include <ts_sig.h>
#endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

#define SW1_NORMAL                  0x90
#define SW1_LENGTH                  0x9f
#define SW1_MEM_ERROR               0x92
#define SW1_REFERENCE_ERROR         0x94
#define SW1_SECURITY_ERROR          0x98
#define SW1_BAD_P3                  0x67
#define SW1_BAD_P1_OR_P2            0x6b
#define SW1_BAD_INSTRUCTION         0x6d
#define SW1_BAD_CLASS               0x6e
#define SW1_UNDEFINED_ERROR         0x6f
/* phase 2+ definitions for SIM Toolkit use */
#define SW1_NORMAL_WITH_SIM_INFO   0x91
#define SW1_SIMAT_IS_BUSY          0x93
/* Data Download Error SW response for data length to retrieve for RP-ERROR */
#define SW1_DDL_ERROR_LENGTH       0x9e
/*  3G  status codes */
#define SW1_EXPECTED_LENGTH         0x6C
#define SW1_RESPONSE                0x61
#define SW1_WARNING1                0x62
#define SW1_WARNING2                0x63
#define SW1_NO_INFO_GIVEN_MEM_UN    0x64
#define SW1_EXEC_ERROR              0x65
#define SW1_CLA_NOT_SUP             0x68
#define SW1_COM_NOT_ALLOWED         0x69
#define SW1_WRONG_PARAM             0x6a
#define SW1_APP_ERROR               0x98
#define SW2_NORMAL                  0x00
#define SW2_MEM_ERROR               0x40
#define SW2_NO_EF                   0x00
#define SW2_OUT_OF_RANGE            0x02
#define SW2_FILE_ID_NOT_FOUND       0x04
#define SW2_PATTERN_NOT_FOUND       0x04
#define SW2_INCONSISTENCY           0x08
#define SW2_NO_CHV_INITIALISED      0x02
#define SW2_AUTHENTICATION_ERROR    0x04
#define SW2_ERROR_WITH_CHV          0x08
#define SW2_ERROR_WITH_INV_STATUS   0x10
#define SW2_NO_ATTEMPTS_LEFT        0x40
#define SW2_CANT_INCREMENT          0x80

#define SW2_NO_INFO_GIVEN                   0x00
#define SW2_INCREASE_CANNOT_BE_PERF         0x50
#define SW2_AUTH_ERROR                      0x62
#define SW2_INCORRECT_DATA_FIELD            0x80
#define SW2_DATA_CORRUPTED                  0x81
#define SW2_MEM_PROBLEM                     0x81
#define SW2_LOGICAL_CHAN_NOT_SUP            0x81
#define SW2_COM_INC_WITH_FILE_STRUCT        0x81
#define SW2_FUNC_NOT_SUPPORTED              0x81
#define SW2_EOF_REACHED_BEFORE_READING_LE   0x82
#define SW2_SEC_MSG_NOT_SUP                 0x82
#define SW2_SEC_STATUS_NOT_SATISFIED        0x82
#define SW2_FILE_NOT_FOUND                  0x82
#define SW2_PIN_BLOCKED                     0x83
#define SW2_REC_NOT_FOUND                   0x83
#define SW2_REF_DATA_INVALIDATED            0x84
#define SW2_COND_OF_USED_NOT_SATISFIED      0x85
#define SW2_NO_EF_SELECTED                  0x86
#define SW2_INCORRECT_P1_P2                 0x86
#define SW2_INCONSISTENT_LC                 0x87
#define SW2_REF_DATA_NOT_FOUND              0x88
#define SW2_PIN_METHOD_BLOCKED              0xc0

#define MF_FILETYPE                 1
#define DF_FILETYPE                 2
#define EF_FILETYPE                 4

#define EF_INCREASE_MASK            0x40
#define EF_INCREASE                 0x40

#define ACCESS_ALWAYS               0x00
#define ACCESS_CHV1                 0x01
#define ACCESS_CHV2                 0x02
#define ACCESS_RFU                  0x03
#define ACCESS_NEVER                0x0f

#define EF_INVALID_MASK             0x01
#define EF_NOT_INVALID              0x01
#define EF_RDDWR_INVALID_MASK       0x04

#define EF_STRUCTURE_T              0
#define EF_STRUCTURE_LF             1
#define EF_STRUCTURE_C              3

/*
** Defines for the file characteristics field of the GSM dir status.
**
** See simdec.c for details.
*/
#define DF_CLOCK_STOP_MASK          0x0d
#define DF_CLOCK_FREQ_MASK          0x02
#define DF_VOLTAGE_MASK             0x70
#define DF_CHV1_ENA_MASK            0x80
#define DF_CHV1_ENABLED             0x00

/**/
#define DF_CHV_REMAIN_MASK          0x0f
#define DF_CHV_INIT_MASK            0x80
#define DF_CHV_INITIALISED          0x80

/* Positions of parameters in EF status data  */
#define EF_STATUS_FILE_SIZE         2
#define EF_STATUS_FILE_ID           4
#define EF_STATUS_FILE_TYPE         6
#define EF_STATUS_INCREASE          7
#define EF_STATUS_ACCESS_RD_UP      8
#define EF_STATUS_ACCESS_INC        9
#define EF_STATUS_ACCESS_INV        10
#define EF_STATUS_FILE_STATUS       11
#define EF_STATUS_STRUCTURE         13
#define EF_STATUS_RECORD_LENGTH     14

/* Positions of parameters in MF or DF status data */
#define DF_FREE_MEM                 2
#define DF_FILE_ID                  4
#define DF_FILE_TYPE                6
#define DF_SPEC_DATA_LEN            12
#define DF_FILE_CHARS               13
#define DF_NUM_CHILD_DFS            14
#define DF_NUM_CHILD_EFS            15
#define DF_NUM_CHVS_ETC             16
#define DF_CHV1_STATUS              18
#define DF_UNBLOCK_CHV1             19
#define DF_CHV2_STATUS              20
#define DF_UNBLOCK_CHV2             21

#define PLMN_ELEMENT_SIZE              3
#define ACCESS_TECHNOLOGY_ELEMENT_SIZE 2
#define SIM_ACCESS_TECHNOLOGY_OFFSET   3
#define CBMI_ELEMENT_SIZE              2
#define MAX_FPLMNS                     4
#define KC_SEQ_NUM                     8
#define LI_ELEMENT_SIZE                2

/* New definitions for Phase 2+ */
#define CBMID_ELEMENT_SIZE             2
#define CBMIR_ELEMENT_SIZE             4
#define SIM_VGCS_VBS_GID_SIZE          4
#define SIM_ELP_CODE_SIZE              2
#define CNL_ELEMENT_SIZE               6
#define SIM_NUM_PRIOR_LEVS_PER_BYTE    7  /* used for eMLPP and AAeM features */
#define SIM_NUM_DCKS                   4
/* position of parameters in the CNL EF */
#define CNL_MCC                        0
#define CNL_MNC                        1
#define CNL_NS                         3
#define CNL_SP                         4
#define CNL_CORP                       5

#define SST_ALLOCATED               0x01
#define SST_ACTIVATED               0x02

/* Definitions for extracting currency information */
#define CURRENCY_SIGN               0x10
#define EXP_MASK                    0xe0
#define EXP_SHIFT                   5

/* Position of parameters in the Loci EF */
#define LOCI_TMSI                   0
#define LOCI_LAI                    4
#define LOCI_TMSI_TIME              9
#define LOCI_STATUS                 10

/* Position of parameters in the GPRS Loci EF */
#define GLOCI_PTMSI             0
#define GLOCI_PTMSI_SIG         4
#define GLOCI_RAI               7
#define GLOCI_RA_STATUS         13
#define MAX_LOCI_GPRS_FILE_LENGTH   14
#define MIN_LOCI_GPRS_FILE_LENGTH   12   /*0209-20504*/
/* 14 is the size of EF GPRS LOCI coded according to latest revisions of 11.11 and 51.011                           */
/* If EF GPRS LOCI file is coded according to 11.11 revision 6.1.0 , then its size will be set to 12                */
/* Below is the offset of the routing area update status for GPRS LOCI file coded according to 11.11 revision 6.1.0 */
/* The value of this offset is 13 for files coded according to the latest revisions of GSM 11.11                    */
#define GLOCI_ROUTING_AREA_STATUS   11
#define GLOCI_ROUTING_AREA_INFO     4

/* definitions for extracting Type of number and numbering plans */
#define TON_MASK                    0x70
#define TON_SHIFT                   4
#define NPI_MASK                    0x0f

/* Length of a dialing number (in bytes. Equivilent to 20 bcd digits) */
#define DIAL_NUM_LEN                10
#define CPS_SECTION                 11

/* Default hplmn time for phase 1 SIMs is 30 seconds (5 * 6) */
#define DEF_PHASE1_HPLMN            5

/* Short message service definitions */
#define SM_MTI_DELIVER              0x00
#define SM_MTI_DELIVER_REPORT       0x00
#define SM_MTI_STATUS_REPORT        0x02
#define SM_MTI_COMMAND              0x02
#define SM_MTI_SUBMIT               0x01
#define SM_MTI_SUBMIT_REPORT        0x01
#define SM_MTI_RESERVED             0x03

#define SM_VPF_NOT_PRESENT          0x00
#define SM_VPF_RELATIVE             0x02
#define SM_VPF_ENCHANCED            0x01
#define SM_VPF_ABSOLUTE             0x03

#define SIM_SMS_DATA_LEN            176
#define CPS_LENGTH                  0
#define CPS_TYPE                    1
#define CPS_ODDEVEN                 1
#define CPS_INFO                    2
#define SIM_CPS_DATA_LEN            22
#define SIM_SMSR_REC_LENGTH         30    /*job 100781*/


#define SIM_EXT_LENGTH              11

#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
/* CPHS related definitions */
#define CPHS_ONS_MAX_SIZE               10
#define CPHS_NUM_VMW_FLAGS_PER_BYTE     2
#define CPHS_VMW_FLAGS_BIT_SHIFT        4
#define CPHS_MSG_WAITING                0x0a
#define CPHS_NO_MSG_WAITING             0x05
#define CPHS_CALL_FWD_UNCOND_ACTIVE     0x0a
#define CPHS_CALL_FWD_UNCOND_NOT_ACTIVE 0x05

/* CPHS indexes into Information Number data */
#define SIM_CPHS_INFO_NUM_ALPHA_LENGTH     0
#define SIM_CPHS_INFO_NUM_ENTRY_STATUS     1
#define SIM_CPHS_INFO_NUM_ALPHA_ID         2
#define M_SimCphsInfoNumDialLength(aLPHAiD)  (aLPHAiD + 2)
#define M_SimCphsInfoNumTonNpi(aLPHAiD)      (aLPHAiD + 3)
#define M_SimCphsInfoNumDialString(aLPHAiD)  (aLPHAiD + 4)

#define SIM_CPHS_INFO_NUM_LEVEL_MASK       0x0F
#define SIM_CPHS_INFO_NUM_NETWORK_MASK     0x20
#define SIM_CPHS_INFO_NUM_PREMIUM_MASK     0x10
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/* Max data that we want to get back from the SIM */
#define MAX_RESPONSE_DATA_LENGTH       255

#define TAG_ID_OFFSET  0
#define LENGTH_OFFSET  1
#define VALUE_START_OFFSET 2

#define SIM_PATH_MIN_LENGTH  2

#define SIM_CCP_RECORD_SIZE 14

#if defined (UPGRADE_LTE)
#define SINGLE_BYTE_TLV_LENGTH 1
#endif


/***************************************************************************
 * Typed Constants
 **************************************************************************/

typedef enum SimExtRecordTypeTag
{
    SIM_EXT_CPS_RECORD      = 1,
    SIM_EXT_OVF_RECORD      = 2,
    SIM_EXT_UNUSED_RECORD   = 0xff /* See GSM 11.11, v4.13.0, 11.5.1 */
                                   /*     Paragraph on purge         */
}
SimExtRecordType;

/***************************************************************************
 * Type Definitions
 **************************************************************************/

typedef struct SimExtRecordTag
{
    Int8    numParents;
    Int8    recordType;
    Int8    dataLength;
    Int8    data[SIM_EXT_LENGTH];
    Int8    nextRecord;

} SimExtRecord;

/***************************************************************************
 *  Macros
 **************************************************************************/

#define RECORD_NUM_VALID( rECORDNUM )    (Boolean) ((rECORDNUM != 0) && (rECORDNUM != 0xff))
#define TLV_OBJECT_OFFSET_ADVANCE( oFFSETpARAM, oBJdATAlENGTH ) \
                        oFFSETpARAM = oFFSETpARAM + 2 + oBJdATAlENGTH;

#define SMALLER_OF(c,d)                 ( ((c) < (d)) ? (c) : (d) )


#define ENCODE_NPI(c)                   (Int8)( (c) & 0x0f)
#define ENCODE_TON(c)                   (Int8)(((c) & 0x07) << 4)

#define ENCODE_MMS(c)                   (((c) & 0x01) << 2)
#define ENCODE_RP(c)                    (((c) & 0x01) << 7)
#define ENCODE_SRI(c)                   (((c) & 0x01) << 5)
#define ENCODE_VPF(c)                   (((c) & 0x03) << 3)
#define ENCODE_SRQ(c)                   (((c) & 0x01) << 5)
#define ENCODE_SRR(c)                   (((c) & 0x01) << 5)
#define ENCODE_UDH(c)                   (((c) & 0x01) << 6)
#define ENCODE_MTI(c)                   (Int8)(((c) >> 1) & 0x03)

#define ENCODE_PID_MEANING(c)           (Int8)((c) & 0xe0)
#define ENCODE_PID_DATA(c)              (Int8)(((c) & 0x3f) )
#define ENCODE_DCS_GROUP(c)             (((c) & 0x0f) << 4)
#define ENCODE_DCS_CODING(c)            (((c) & 0x01) << 2)
#define ENCODE_DCS_CLASS(c)             (((c) & 0x03) )
#define ENCODE_DCS_ALPHAIND(c)          (((c) & 0x0f) )

#define ENCODE_RADIO_CH_REQ(c)          (((c) & 0x03) << 5)
#define ENCODE_TRANSFER_MODE(c)         (((c) & 0x01) << 3)
#define ENCODE_TRANSFER_CAP(c)          (((c) & 0x07) )
#define ENCODE_DATA_STRUCTURE(c)        (((c) & 0x03) << 4)
#define ENCODE_DUPLEX_MODE(c)           (((c) & 0x01) << 3)
#define ENCODE_RATE_ADAPTION(c)         (((c) & 0x03) << 3)
#define ENCODE_SIGNAL_AP(c)             (((c) & 0x07) )
#define ENCODE_IS_SYNC(c)               (((c) & 0x01) )
#define ENCODE_USER_RATE(c)             (((c) & 0x0f) )
#define ENCODE_NUM_BITS(c)              (((c) & 0x01) << 4)
#define ENCODE_NUM_STOP_BITS(c)         (((c) & 0x01) << 6)
#define ENCODE_PARITY(c)                (((c) & 0x07) )
#define ENCODE_NIC_ON_RX(c)             (((c) & 0x01) << 3)
#define ENCODE_NIC_ON_TX(c)             (((c) & 0x01) << 4)
#define ENCODE_INTERMED_RATE(c)         (((c) & 0x03) << 5)
#define ENCODE_MODEM_TYPE(c)            (((c) & 0x1f) )
#define ENCODE_CONNECT_ELEM(c)          (((c) & 0x03) << 5)
#define ENCODE_USER_INFO(c)             (((c) & 0x1f) )

#define ENCODE_SMSP_DEST_ADDR(c)        (((c) & 0x01) )
#define ENCODE_SMSP_SC_ADDR(c)          (((c) & 0x01) << 1)
#define ENCODE_SMSP_PROT_ID(c)          (((c) & 0x01) << 2)
#define ENCODE_SMSP_DATA_CODING(c)      (((c) & 0x01) << 3)
#define ENCODE_SMSP_VALID_PERIOD(c)     (((c) & 0x01) << 4)

#define ENCODE_SMSS_MEM_EXCEEDED(c)     (Int8)(~((c) & 0x01) )

#define ENCODE_CPS_TYPE(c)              (Int8)(((c) & 0x07) << 4)
#define ENCODE_CPS_ODD(c)               (Int8)(((c) & 0x01) << 3)

/* Macro to decode an Int16 value from two Int8`s */

#define COMBINE_TWO_BYTES(high,low)     (Int16)((((Int32)high) << 8) | (Int32)(low))

/* Macros to decode access codes from a given byte */
#define DECODE_CPS_TYPE(c)              (((c) >> 4) & 0x07)
#define DECODE_CPS_ODD(c)               (((c) >> 3) & 0x01)

#define DECODE_ACCESS_UPDATE(c)         ((c) & 0x0f)
#define DECODE_ACCESS_READSEEK(c)       (((c) >> 4) & 0x0f)
#define DECODE_ACCESS_INCREASE(c)       (((c) >> 4) & 0x0f)
#define DECODE_ACCESS_INVALID(c)        ((c) & 0x0f)
#define DECODE_ACCESS_REHAB(c)          (((c) >> 4) & 0x0f)

#define DECODE_NPI(c)                   ((c) & 0x0f)
#define DECODE_TON(c)                   (((c) >> 4) & 0x07)

/* Macros to decode SMS parameters */
#define DECODE_MTI(c)                   ((c) & 0x03)
#define DECODE_MMS(c)                   (((c) >> 2) & 0x01)
#define DECODE_RD(c)                    (((c) >> 2) & 0x01)
#define DECODE_VPF(c)                   (((c) >> 3) & 0x03)
#define DECODE_SRI(c)                   (((c) >> 5) & 0x01)
#define DECODE_SRR(c)                   (((c) >> 5) & 0x01)
#define DECODE_UDH(c)                   (((c) >> 6) & 0x01)
#define DECODE_RP(c)                    (((c) >> 7) & 0x01)
#define DECODE_DCS_GROUP(c)             (((c) >> 4) & 0x0f)
#define DECODE_DCS_CODING(c)            (((c) >> 2) & 0x01)
#define DECODE_DCS_CLASS(c)             ((c) & 0x03)
#define DECODE_DCS_ALPHAIND(c)          ((c) & 0x0f)

#define DECODE_RADIO_CH_REQ(c)          (((c) >> 5) & 0x03)
#define DECODE_CODING_STD(c)            (((c) >> 4) & 0x01)
#define DECODE_TRANSFER_MODE(c)         (((c) >> 3) & 0x01)
#define DECODE_TRANSFER_CAP(c)          (((c) & 0x07))
#define DECODE_DATA_STRUCTURE(c)        (((c) >> 4) & 0x03)
#define DECODE_DUPLEX_MODE(c)           (((c) >> 3) & 0x01)
#define DECODE_RATE_ADAPTION(c)         (((c) >> 3) & 0x03)
#define DECODE_SIGNAL_AP(c)             (((c) & 0x07))
#define DECODE_IS_SYNC(c)               (((c) & 0x01))
#define DECODE_EXTENSION(c)             (((c) >> 7) & 0x01)
#define DECODE_USER_RATE(c)             (((c) & 0x0f))
#define DECODE_NUM_BITS(c)              (((c) >> 4) & 0x01)
#define DECODE_NUM_STOP_BITS(c)         (((c) >> 6) & 0x01)
#define DECODE_PARITY(c)                (((c) & 0x07) )
#define DECODE_NIC_ON_RX(c)             (((c) >> 3) & 0x01)
#define DECODE_NIC_ON_TX(c)             (((c) >> 4) & 0x01)
#define DECODE_INTERMED_RATE(c)         (((c) >> 5) & 0x03)
#define DECODE_MODEM_TYPE(c)            (((c) & 0x1f))
#define DECODE_CONNECT_ELEM(c)          (((c) >> 5) & 0x03)
#define DECODE_USER_INFO(c)             (((c) & 0x1f))

#define DECODE_SMSP_DEST_ADDR(c)        (((c) & 0x01))
#define DECODE_SMSP_SC_ADDR(c)          (((c) >> 1) & 0x01)
#define DECODE_SMSP_PROT_ID(c)          (((c) >> 2) & 0x01)
#define DECODE_SMSP_DATA_CODING(c)      (((c) >> 3) & 0x01)
#define DECODE_SMSP_VALID_PERIOD(c)     (((c) >> 4) & 0x01)

#define SMSP_PRESENT                    0
#define SMSP_ABSENT                     1
#define SMSP_ADDR_LENGTH                12

#define DECODE_SMSS_MEM_EXCEEDED(c)     ((c) & 0x01)

/*EONS defines*/
#define  OPL_MCC                        0
#define  OPL_MNC                        1
#define  OPL_LAC1                       3
#define  OPL_LAC2                       5
#define  OPL_PNNID                      7
#define  PNN_FULL_NAME_IEI_POS          0
#define  PNN_FULL_NAME_LENGTH           1
#define  PNN_FULL_NAME_CODING           2
#define  PNN_FULL_NAME                  3
#define  PNN_FULL_NAME_IEI              0x43
#define  PNN_SHORT_NAME_IEI             0x45

/* Compute memory space required to store [n] values in semi-octet form */
#define SemiOctetSize(n)                ((n)/2 + (n)%2)



/* Macros to encode SMS parameters */

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
Int8 SimDecodeTLVLength ( Int16 *dataLgth,
                          Int8  *data);

Boolean SimDecodeCfis   ( SimDataBlock *dataBlock,
                          SimCfisData  *cfis_p,
                          Int8         *ccpRecord_p,
                          Int8         *extRecord_p);


SimSwStatusCode SimDecodeSwStatus      ( SimDataBlock             *dataBlock,
                                         SimManagerData           *simData);

void SimDecodeEfStatusData  (   SimDataBlock             *dataBlock,
                                SimUiccAppData           *uiccData,
                                SimManagerData           *efStatus );

void SimDecodeDirStatusData (   SimDataBlock             *dataBlock,
                                SimDirStatusData         *dirStatus );

void SimDecodeIccid         (   SimDataBlock             *dataBlock,
                                SimIccid                 *iccid );

void SimDecodeLp            (   SimDataBlock             *dataBlock,
                                Int16                    startField,
                                Int8                     numEntriesDesired,
                                Int16                    *totalEntries,
                                LanguagePrefList         *lpList );

Boolean SimDecodeImsi        (   SimDataBlock             *dataBlock,
                                 Imsi                     *imsi );

void SimDecodePlmn ( SimDataBlock *dataBlock,
                     Int16         startField,
                     Int16         numEntriesDesired,
                     Int16        *totalEntries,
                     SimPlmnList  *plmnList,
                     Boolean       accessTechnologyPresent,
                     Boolean       checkPlmn);

Boolean SimCompactPlmn      (   SimDataBlock             *dataBlock,
                                Boolean                  accessTechnologyPresent,
                                Int16                    *numValidEntries  );

#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
void SimDecodeAcmMax        (   SimDataBlock             *dataBlock,
                                Int32                    *acmMax );
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

void SimDecodeHplmn         (   SimDataBlock             *dataBlock,
                                SimManagerData           *simData,
                                Int16                    *hPlmn );

#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
void SimDecodeSst           (   SimDataBlock             *dataBlock,
                                SimManagerData           *simData,
                                SimServiceTable          *serviceTable );

void SimDecodeAcm           (   SimDataBlock             *dataBlock,
                                Int32                    *acm );

void SimDecodePuct          (   SimDataBlock             *dataBlock,
                                SimPuctData              *puctData );

void SimDecodeCbmi          (   SimDataBlock             *dataBlock,
                                Int8                     startField,
                                Int8                     numEntriesDesired,
                                Int8                     *totalEntries,
                                CbmiList                 *cbmiList );

void SimDecodeBcch          (   SimDataBlock             *dataBlock,
                                SimBaList                *baList );
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

AccessClass SimDecodeAccessClass (SimDataBlock           *dataBlock);


void SimDecodeLoci          (   SimDataBlock             *dataBlock,
                                SimManagerData           *simData,
                                LocationInformation      *loci );

void SimDecodeAdmin         (   SimDataBlock             *dataBlock,
                                SimAdminData             *adminData );

SimPhase SimDecodePhase     (   SimDataBlock             *dataBlock);

Boolean SimDecodeAdnFdn     (   SimDataBlock             *dataBlock,
                                SimAbbreviatedDialNumber *adn,
                                Int8                     *ccpRecord,
                                Int8                     *extRecord,
                                Int16 dnFile);

void SimDecodeSms           (   SimDataBlock             *dataBlock,
                                SimSmTpdu                *smTpdu,
                                SimSmTpduType            *tpduType,
                                SimSmRecordStatus        *status,
                                SmsAddress               *scAddr );

Int16 SimDecodeCcp          (   SimDataBlock             *dataBlock,
                                BearerCapability         *capability );

void SimDecodeSmsp          (   SimDataBlock             *dataBlock,
                                SimSmsParameters         *smsp );

void SimDecodeSmss          (   SimDataBlock             *dataBlock,
                                Int8                     *msgRef,
                                Boolean                  *memoryCapExceeded );

void SimDecodeExt           (   SimDataBlock             *dataBlock,
                                SimExtRecord             *extRecord );


Boolean SimDecodeExtensionData (Int8                      extRecord,
                                Int16                     extId,
                                Bcd                      *dialString_p,
                                Int8                     *dialStringLength_p,
                                CalledPartySubaddress    *calledPartySubaddress_p,
                                SimManagerData           *simData,
                                SimDataBlock             *dataBlock);


void SimDecodeCps           (   SimDataBlock             *dataBlock,
                                CalledPartySubaddress    *cps );

void SimDecodeCpsValue      (   Int8                     *dataOffset,
                                SimDataBlock             *dataBlock,
                                CalledPartySubaddress    *cps );
#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
void SimDecodeGid           (   SimDataBlock             *dataBlock,
                                SimGroupIdData           *gids );
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
void SimDecodeSpn           (   SimDataBlock             *dataBlock,
                                SimSpnData               *spn,
                                Boolean                   displayCond2Present);

#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
void SimDecodeSpdi          (Int16        dataLength,
                             Int8        *data_p,
                             SimSpdiList *spdiList_p,
                             Int16        startField,
                             Int16       *numValidEntries_p );
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

void SimDecodeRequestStatus ( SimUiccAppData           *uiccData,
                              SimRequestStatus         *requestStatus );


Boolean SimDecodeOpl           (   SimDataBlock               *dataBlock,
                                   SimOperatorList            *opl );

Boolean SimDecodePnn           (   SimDataBlock               *dataBlock,
                                   SimNetworkNameList         *pnn,
                                   Int8                        recordNumber);

void DecodeLongMnc          (   Int16         *mnc,
                                Int8          *data );

void SimDecodeElp           (   SimDataBlock              *dataBlock,
                                Int16                      startField,
                                Int8                       numEntriesDesired,
                                Int16                     *totalEntries,
                                SimExtLangPrefList        *elpList );

#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
void SimDecodeVgcsVbs       (   SimDataBlock              *dataBlock,
                                SimVoiceServiceData       *vsData );

void SimDecodeVgcsVbsStatus (   SimDataBlock              *dataBlock,
                                SimVoiceServiceStatusData *vsStatusData );

void SimDecodeEmlpp         (   SimDataBlock              *dataBlock,
                                SimEmlppData              *emlppData );

void SimDecodeAaem          (   SimDataBlock              *dataBlock,
                                SimAaemData               *aaemData );

void SimDecodeCbmid         (   SimDataBlock              *dataBlock,
                                Int16                     startField,
                                Int16                     numEntriesDesired,
                                Int16                     *totalEntries,
                                SimCbmidList              *cbmidList );

void SimDecodeCbmir         (   SimDataBlock              *dataBlock,
                                Int16                     startField,
                                Int8                      numEntriesDesired,
                                Int16                     *totalEntries,
                                SimCbmirList              *cbmirList );
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

void SimDecodeEcc           (   SimDataBlock              *dataBlock,
                                Int8                      *totalEntries,
                                SimEccList                *eccList );
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
void SimDecodeDck           (   SimDataBlock              *dataBlock,
                                SimDckData                *dckData );
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
void SimDecodeCnl           (   SimDataBlock              *dataBlock,
                                SimCnlData                *cnlData );

#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
void SimDecodeNia           (   SimDataBlock              *dataBlock,
                                SimInformativeText        *infoText,
                                SimAlertingCategory       *alertingCat );
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

void SimDecodeSmsr          (   SimDataBlock              *dataBlock,
                                Int8                      *smsRecordNum,    /*job 100781*/
                                TsStatusReportInd         *smStatusReport );

void SimDecodeActHplmn      (   SimDataBlock              *dataBlock,
                                SimPlmn                   *actHplmn,
                                Boolean                   *actHplmnValid ); /* job111915 */

void SimEncodeTerminalSupportFeatures ( SimEnhNtwkTermSuppProfile     *enhNtwkTermSuppProfile,
                                        SimDataBlock                  *dataBlock );

#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
/* Decode prototypes for CPHS related functions */
void SimCphsDecodeCphsInfo  ( SimDataBlock         *dataBlock,
                              SimManagerData       *simData,
                              SimCphsPhase         *cphsPhase,
                              SimCphsServiceTable  *cphsService );

void SimCphsDecodeCff       ( SimDataBlock                 *dataBlock,
                              SimCphsCallForwardFlags      *cphsCff );

void SimCphsDecodeVmwFlags  ( SimDataBlock                 *dataBlock,
                              SimCphsVoiceMsgWaitingFlags  *cphsVmwFlags );

void SimCphsDecodeOn        ( SimDataBlock                 *dataBlock,
                              SimCphsOperatorName          *cphsOpName );

void SimCphsDecodeOns       ( SimDataBlock                 *dataBlock,
                              SimCphsOperatorNameShort     *cphsOpNameShortForm );

void SimCphsDecodeCsp       ( SimDataBlock                 *dataBlock,
                              SimCphsCustServProfile       *cphsCsp );

Boolean SimCphsDecodeInfoNum ( SimDataBlock                 *dataBlock,
                               SimCphsInfoNumEntry          *infoNum,
                               Int8                         *extRecord,
                               Int8                          recordLength,
                               Int8                         *maxAlphaLength);

Boolean SimCphsIsInfoNumUsed ( SimDataBlock                *dataBlock );

Int8 SimCphsGetMaxInfoNumDialString ( SimDataBlock         *dataBlock,
                                      Int8                  recordLength );
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
void SimHZDecodeHZParameters ( SimDataBlock             *dataBlock,
                               SimHomeZoneEFParameters  *hzParameters );

void SimHZDecodeHZCache ( SimDataBlock             *dataBlock,
                          SimHomeZoneEFCacheEntry  *cacheEntry);

void SimEncodeHzCellCache( SimHomeZoneEFCacheEntry *cache,
                           SimDataBlock            *dataBlock );

void SimDecodeMsgWaitingInfo ( SimDataBlock            *dataBlock,
                               SimMBMsgWaitingInfo     *msgWaitingInfo
#if defined (UPGRADE_REL6)
                              ,Boolean                  videoMailInfoPresent
#endif
                             );

void SimEncodeMsgWaitingInfo ( SimMBMsgWaitingInfo *msgWaitingInfo,
                               SimDataBlock        *dataBlock
#if defined (UPGRADE_REL6)
                              ,Int8                 numInfoSets
#endif
                             );


void SimDecodeMailboxInfo ( SimDataBlock            *dataBlock,
                            SimMailboxInfo          *mailboxInfo
#if defined (UPGRADE_REL6)
                           ,Boolean                  videoMailInfoPresent
#endif
                          );

void SimEncodeMailboxInfo ( SimMailboxInfo          *mailboxInfo,
                            SimDataBlock            *dataBlock
#if defined (UPGRADE_REL6)
                           ,Int8                     numInfoSets
#endif
                          );
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

void SimEncodeLp          ( LanguagePrefList        *lpList,
                            SimDataBlock            *dataBlock );

void SimEncodePlmn ( SimPlmnList  *plmnList,
                     SimDataBlock *dataBlock,
                     Boolean accessTechnologyPresent);

#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
void SimEncodeAcmMax      ( Int32                    acmMax ,
                            SimDataBlock            *dataBlock );

void SimEncodeAcm         ( Int32                    acm,
                            SimDataBlock            *dataBlock );

void SimEncodePuct        ( SimPuctData             *puctData,
                            SimDataBlock            *dataBlock );

void SimEncodeCbmi        ( CbmiList                *cbmiList,
                            SimDataBlock            *dataBlock );

void SimEncodeBcch        ( SimBaList               *baList,
                            SimDataBlock            *dataBlock );
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

void SimEncodeAccessClass ( AccessClass             *accessClass,
                            SimDataBlock            *dataBlock );

void SimEncodeLoci        ( LocationInformation     *loci ,
                            SimManagerData          *simData,
                            SimDataBlock            *dataBlock );

void SimEncodeAdnFdn     ( SimAbbreviatedDialNumber *adn,
                           Int8                      ccpRecord,
                           Int8                      extRecord ,
                           SimDataBlock             *dataBlock ,
                           Int8                      adnRecLen,
                           SimDialNumberFile         dnFile);

void SimEncodeSms        ( SimSmTpdu                *smTpdu,
                           SimSmTpduType             tpduType,
                           SimSmRecordStatus         status,
                           SmsAddress               *address,
                           SimDataBlock             *dataBlock );

Int16 SimEncodeCcp       ( BearerCapability         *capability ,
                           Int8                     *encData,
                           Int8                      ccpRecordLength );

void SimEncodeSmsp       ( SimSmsParameters         *smsp ,
                           SimDataBlock             *dataBlock,
                           Int8                      smspRecLen );

void SimEncodeSmss       ( Int8                     *msgRef,
                           Boolean                  *memoryCapExceeded ,
                           SimDataBlock             *dataBlock );

void SimEncodeExt        ( SimExtRecord             *extRecord ,
                           SimDataBlock             *dataBlock );

void SimEncodeCps        ( CalledPartySubaddress    *cps,
                           SimDataBlock             *dataBlock );

void SimEncodeRawSms     ( SimSmRecordStatus         smRecordStatus,
                           RawSmsAddress            *scAddr,
                           RpUserDataElement        *tpdu,
                           SimDataBlock             *dataBlock );

void SimEncodeElp             ( SimExtLangPrefList      *elpList,
                                SimDataBlock            *dataBlock );

void SimEncodeAaem            ( SimAaemData             *aaemData,
                                SimDataBlock            *dataBlock );

void SimEncodeCbmir           ( SimCbmirList            *cbmirList,
                                SimDataBlock            *dataBlock );

#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
void SimEncodeDck             ( SimDataBlock            *dataBlock );
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

void SimEncodeSmsr            ( TsStatusReportInd       *smsrList,        /*job 100781*/
                                Int8                     smsRecord,
                                SimDataBlock            *dataBlock);

void SimEncodeCfis            ( SimCfisData             *cfisData_p,
                                Int8                     ccpRecord,
                                Int8                     extRecord ,
                                SimDataBlock            *dataBlock ,
                                Int8                     cfisRecordLength);


void  SimDecodeImageInstData(  SimDataBlock     *dataBlock,
                               Int16            *dataLength,
                               SimImageInstData *data,
                               Int16            desiredLength);

void  SimDecodeImageRec(  SimDataBlock  *dataBlock,
                          SimImageData  *image );


void SimEncodeCpsValue ( CalledPartySubaddress *cps,
                         Int8                  *dataOffset,
                         SimDataBlock          *dataBlock );

void Encode3DigitMcc ( Int16 mcc, Int16 mnc, Int8 *data );
void Encode3DigitMnc ( Int16 mnc, Int8 *data );
void EncodeMnc ( Int16 mnc, Int8 *data );
void EncodeMcc ( Int16 mcc, Int8 *data );



#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
/* Encode prototypes for CPHS related functions */
void SimCphsEncodeCff       ( SimCphsCallForwardFlags   *cphsCff,
                              SimDataBlock              *dataBlock );

void SimCphsEncodeVmwf      ( SimCphsVoiceMsgWaitingFlags  *cphsVmwf,
                              SimDataBlock                 *dataBlock );

void SimCphsEncodeCspEntry  ( SimCphsCspEntry              *cspEntry,
                              SimDataBlock                 *dataBlock );

void SimCphsEncodeInfoNum   ( SimCphsInfoNumEntry          *infoNum,
                              SimDataBlock                 *dataBlock,
                              Int8                          recordLength,
                              Int8                          maxAlphaIdLength);
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

void DecodeMcc (Int16 *mcc, Int8 *data);
Boolean DecodeMnc ( Int16 *mnc, Int8 *data);

void SimDecodeNasConfig ( SimDataBlock *dataBlock,
                          SimManagerData *simData,
                          AlsiReadNasConfigCnf *respData );
#endif

/* END OF FILE */
