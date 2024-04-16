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
 *      types common to mmxx_sig.h and rr_sig.h
 **************************************************************************/

#ifndef L3_TYP_H
#define L3_TYP_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#include     <stddef.h>

#if !defined(L3GP_TYP_H)
# include     <l3gp_typ.h>
#endif  /* !(L3GP_TYP_H) */

#if defined (UPGRADE_LTE)
#include <l3eps_typ.h>
#endif /* UPGRADE_LTE */

#if !defined(MNL3_TYP_H)
# include     <mnl3_typ.h>
#endif  /* !(MNL3_TYP_H) */

#if !defined(RL3_TYP_H)
# include     <rl3_typ.h>
#endif  /* !(RL3_TYP_H) */

#if !defined(TIL3_TYP_H)
# include     <til3_typ.h>
#endif  /* !(TIL3_TYP_H) */

#if !defined(ERR_TYP_H)
# include     <err_typ.h>
#endif  /* !(ERR_TYP_H) */

#if !defined(L33G_TYP_H)
# include <l33g_typ.h>
#endif  /* !(L33G_TYP_H) */

/****************************************************************************
 * Macros
 ****************************************************************************/

#define     NUM_REJECT_PER_MSG        4
#define     NUM_MEAS_RECORDS          6
#define     MAX_RPPDU_INFO_LENGTH     248
#define     MAX_USER_TO_USER_LENGTH   128
#define     MAX_BA_RANGES             10
/* (Theoretical max = (98 Ranges / 2) * 5 = 245 Octets */

#define MSG_NO_BODY                   offsetof (L3Message, body)

#define MSG_SIZE(m) (MSG_NO_BODY + sizeof (m ## Message))

#define     MAX_BCCH_SCHEDULING_INFO  12
#define     MAX_BCCH_OCCURENCE        16

/*
** Transaction Values and Transaction Identifiers in Layer 3 messages
*/
#define     MAX_NUM_TVS               7                   /* 24.007 11.2.3 */
#define     NUMBER_OF_TFS             2                   /* myTv and !myTv */
#define     MAX_NUM_TIS               (MAX_NUM_TVS * NUMBER_OF_TFS)

/*
** Transaction Values and Transaction Identifiers in Layer 3 messages
** when the extension mechanism is used (required in GPRS SM
** signalling to address more than 7 NSAPIs in Release 99 and later).
*/
#define     MAX_NUM_EXT_TVS           128                  /* 24.007 11.2.3 */

#define LEN_OF_OCTET                  8

#define MAX_NUM_BSIC_DESCR            32
#define MAX_NUM_RTD_DESCR             32
#define MAX_NUM_BSIC_PER_ARFCN        5
#define MAX_NUM_RTD_PER_ARFCN         3
#define MAX_NUM_RTD12                 32
#define MAX_NUM_RTD6                  32
#define NUM_NCELL_RTD                 3       /* Limit of 3 RTD per ncell */
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
/* UTRAN Freq List contained in the channel release message */
# define UTRAN_FREQ_LIST_ELEMENT_SIZE 8
#endif  /* UPGRADE_3G && UPGRADE_GGE */

#define SI2_TER_REST_OCTET_OFFSET     19
#define SI2_TER_REST_OCTET_SIZE       4

#define CM3_ENC_CTRL_R99_BIT          0x0001
#define CM3_ENC_CTRL_GERAN4_FP1_BIT   0x0002
#define CM3_ENC_CTRL_FDD_CAPAB_BIT    0x0004
#define CM3_ENC_CTRL_TDD128_CAPAB_BIT 0x0010
#define CM3_ENC_CTRL_NO_IEI_OR_LEN    0x0008

/* Used by both the L3 decode module and it's calling modules. */
#define MAX_BEARER_CAPABILITY_IE_SIZE 16

#define LOCATION_AREA_ID_IE_SIZE      5

#define OCTET_SIZE                    8

/* Transaction Identifier */
#define     TV_RESERVED               7

#define MAX_NUM_GSM_DESCR_CELLS       6

/* Used in the decode of IEs containing UTRA FDD cell information. Not under an UPGRADE
** as also required skipping over 3G IEs contained in 2GAS messages */
#define MAX_NR_OF_FDD_CELLS           17

/* This value is needed for TDD128 and non-dual mode  */
#define MAX_NR_OF_TDD128_CELLS        21

#if defined (UPGRADE_LTE)
/** \def MAX_MODEA_LB_ENTITIES
 *  Defines the maximum number of loop back enitities for Mode A. */
#define MAX_MODEA_LB_ENTITIES           2
#endif /* UPGRADE_LTE */

/****************************************************************************
 * Types
 ****************************************************************************/

typedef enum NibbleTag
{
    HIGH_NIBBLE,
    LOW_NIBBLE
}
Nibble;

typedef enum ElementTypeTag
{
    MANDATORY,
    OPTIONAL
}
ElementType;

typedef enum IeCodesetTag
{
    IE_CODESET_0 = 0x000,
    IE_CODESET_1 = 0x100,
    IE_CODESET_2 = 0x200,
    IE_CODESET_3 = 0x300,
    IE_CODESET_4 = 0x400,
    IE_CODESET_5 = 0x500,
    IE_CODESET_6 = 0x600,
    IE_CODESET_7 = 0x700
}
IeCodeset;

typedef enum IeiTag
{
    /* Common Information Element Identifiers */
    CELL_ID_IE = 0x11,
    CIPHERING_KEY_SEQ_NUM_IE             = 0xF0,
    LOCATION_AREA_ID_IE                  = 0x13,
    MOBILE_ID_IE                         = 0x17,
    MS_CLASSMARK1_IE                     = 0x18,
    MS_CLASSMARK2_IE                     = 0x1F,
    MS_CLASSMARK3_IE                     = 0x20,
#if defined(UPGRADE_3G)
    MS_CLASSMARK2_UMTS_IE                = 0x33,
#endif  /* (UPGRADE_3G) */
    ESM_DEVICE_PROPERTIES_IE             = 0xC0,
    EMM_DEVICE_PROPERTIES_IE             = 0xD0,
    EDRX_IE                              = 0x6E,
    /* RR Information Element Identifiers */
    BA_RANGE_IE                          = 0x73,
    CELL_CHANNEL_DESCR_IE                = 0x62,
    CELL_DESCR_IE                        = 0x61,
    CELL_OPTIONS_IE                      = 0x6B,
    CELL_SELECTION_PARAM_IE              = 0x6C,
    CHANNEL_DESCR_IE                     = 0x64,
    CHAN_DESC1_BEFORE_TIME_IE            = 0x1c,
    CHAN_DESC2_BEFORE_TIME_IE            = 0x1d,
    CHANNEL_MODE_IE                      = 0x63,
    CHANNEL_MODE2_IE                     = 0x66,
    CHANNEL_NEEDED_IE                    = 0xE0,
    CIPHER_MODE_SETTING_IE               = 0x90,
    CIPHER_RESPONSE_IE                   = 0xA0,
    CONTROL_CHAN_DESCR_IE                = 0x68,
    FREQUENCY_CHAN_SEQ_IE                = 0x69,
    FREQ_CHAN_SEQ_BEFORE_TIME_IE         = 0x1e,
    FREQUENCY_LIST_IE                    = 0x05,
    FREQUENCY_SHORT_LIST_IE              = 0x02,
    FREQ_LIST_BEFORE_TIME_IE             = 0x19,
    FREQ_SHORT_LIST_BEFORE_TIME_IE       = 0x12,
    IA_REST_OCTETS_IE                    = 0x00, /* Value is not relevant */
    HANDOVER_REFERENCE_IE                = 0x6A,
    MEASUREMENT_RESULTS_IE               = 0x71,
    MOBILE_ALLOCATION_IE                 = 0x72,
    MOBILE_ALLOC_BEFORE_TIME_IE          = 0x21,
    MOBILE_TIME_DIFFERENCE_IE            = 0x77,
    NEIGHBOUR_CELLS_DESCR_IE             = 0x74,
    PAGE_MODE_IE                         = 0xB0,
    P2_REST_OCTETS_IE                    = 0x0, /* Value is not relevant */
    P3_REST_OCTETS_IE                    = 0x0, /* Value is not relevant */
    NCC_PERMITTED_IE                     = 0x76,
    POWER_COMMAND_IE                     = 0x75,
    RACH_CONTROL_PARAM_IE                = 0x78,
    REQUEST_REFERENCE_IE                 = 0x79,
    RR_CAUSE_IE                          = 0x7A,
    SI3_REST_OCTETS_IE                   = 0x0, /* Value is not relevant */
    SI4_REST_OCTETS_IE                   = 0x0, /* Value is not relevant */
    SI7_REST_OCTETS_IE                   = 0x0, /* Value is not relevant */
    SI8_REST_OCTETS_IE                   = 0x0, /* Value is not relevant */
    STARTING_TIME_IE                     = 0x7C,
    SYNC_INDICATION_IE                   = 0xD0,
    TIMING_ADVANCE_IE                    = 0x7D,
    TIME_DIFFERENCE_IE                   = 0x7B,
    TMSI_IE                              = 0x7E,
    WAIT_INDICATION_IE                   = 0x7F,
    GROUP_CHANNEL_DESCRIPTION_IE         = 0x74,
    GROUP_CIPHER_KEY_NUMBER_IE           = 0x80,
    GPRS_RESUMPTION_IE                   = 0xc0,
    BA_LIST_PREF_IE                      = 0x75,
    UTRAN_FREQ_LIST_IE                   = 0x76,
    VGCS_TARGET_MODE_INDIC_IE            = 0x01,
    MULTIRATE_CONFIGURATION_IE           = 0x03,
    CLASSMARK_ENQ_MASK_IE                = 0x10,
    CELL_SELECTION_INDIC_IE              = 0x77,

    /* MM Information Element Identifiers */
    AUTH_PARAM_RAND_IE                   = 0x41,
    AUTH_PARAM_SRES_IE                   = 0x42,
    AUTH_PARAM_AUTN_IE                   = 0x20,
    AUTH_PARAM_SRES_EXT_IE               = 0x21,
    AUTH_PARAM_FAILURE_IE                = 0x22,
    CM_SERVICE_TYPE_IE                   = 0x90,
    FOLLOW_ON_PROCEED_IE                 = 0xA1,
    ID_TYPE_IE                           = 0xC0,
    LOCATION_UPDATING_TYPE_IE            = 0xE0,
    REJECT_CAUSE_IE                      = 0x44,
    FULL_NETWORK_NAME_IE                 = 0x43,
    SHORT_NETWORK_NAME_IE                = 0x45,
    LOC_TIME_ZONE_IE                     = 0x46,
    UNI_TIME_AND_LOC_TIME_ZONE_IE        = 0x47,
    LSA_IDENTITY_IE                      = 0x48,
    NETWORK_DAYLIGHT_SAVING_TIME_IE      = 0x49,
    EQUIVALENT_PLMNS_IE                  = 0x4A,
    CTS_PERMISSION_IE                    = 0xA2,

    /* CC Information Element Identifiers */
    AUXILIARY_STATES_IE                  = 0x24,
    BEARER_CAPABILITY_IE                 = 0x04,
#if defined(UPGRADE_REL5)
    BACKUP_BEARER_CAPABILITY_IE          = 0x41,
#endif  /* (UPGRADE_REL5) */
    CALL_STATE_IE                        = 0x14,
    CALLING_PARTY_BCD_NUM_IE             = 0x5C,
    CALLING_PARTY_SUBADDRESS_IE          = 0x5D,
    CALLED_PARTY_BCD_NUM_IE              = 0x5E,
    CALLED_PARTY_SUBADDRESS_IE           = 0x6D,
    CAUSE_IE                             = 0x08,
    CAUSE_OF_NO_CLI_IE                   = 0x3A,
    CLIR_SUPPRESSION_IE                  = 0xA1,
    CLIR_INVOCATION_IE                   = 0xA2,
    CONGESTION_LEVEL_IE                  = 0xB0,
    CONNECTED_NUMBER_IE                  = 0x4C,
    CONNECTED_SUBADDRESS_IE              = 0x4D,
    FACILITY_IE                          = 0x1C, /* Issue dlm002 */
    HIGH_LAYER_COMPAT_IE                 = 0x7D,
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
    /* CR-860 Correction of the network initiated in-call modification
    ** Rec 24.008 9.3.13 Modify
    ** The field immediateModificationIndicatorPresent does not exist anymore
    */
#else /* !(UPGRADE_3G_FDD) || !(UPGRADE_REL6) */
    IMMEDIATE_MODIFICATION_INDICATOR_IE  = 0xA4,
#endif /* !(UPGRADE_3G_FDD) || !(UPGRADE_REL6) */
    KEYPAD_FACILITY_IE                   = 0x2C,
    LOW_LAYER_COMPAT_IE                  = 0x7C,
    MORE_DATA_IE                         = 0xA0,
    NETWORK_CALL_CONTROL_CAPABILITIES_IE = 0x2F,
    NOTIFICATION_INDICATOR_IE            = 0x27,
    PROGRESS_INDICATOR_IE                = 0x1E,
    REPEAT_INDICATOR_IE                  = 0xD0,
    SHIFT_IE                             = 0x90,
    SIGNAL_IE                            = 0x34,
    SS_VERSION_INDICATOR_IE              = 0x7F,
    STREAM_IDENTIFIER_IE                 = 0x2D,
#if defined(UPGRADE_REL5)
    SUPPORTED_CODEC_LIST_IE              = 0x40,
    EMERGENCY_SERVICE_CATEGORY_IE        = 0x2E,
    PS_LCS_CAPABILITY_IE                 = 0x33,
    EMERGENCY_NUMBER_LIST_IE             = 0x34,
    NETWORK_FEATURE_SUPPORT_IE           = 0xB0,
#endif  /* (UPGRADE_REL5) */
    REVERSE_CALL_SETUP_DIRECTION_IE      = 0xA3,
    CC_CAPABILITIES_IE                   = 0x15,
    REDIRECTING_PARTY_BCD_NUMBER_IE      = 0x74,
    REDIRECTING_PARTY_SUBADDRESS_IE      = 0x75,
    PRIORITY_LEVEL_IE                    = 0x80,
    ALERTING_PATTERN_IE                  = 0x19,
    USER_USER_IE                         = 0x7E
    ,
    UNKNOWN_GPRS_IE                      = 0,    /* IEs for which no IEI is defined */

    /* For GRR */
    RR_PACKET_UPLINK_ASSIGNMENT          = 0x22,
    RR_PACKET_DNLINK_ASSIGNMENT          = 0x23,

    /* For GMM */
    GPRS_GMM_DRX_PARAMETERS_IE           = 0x27,
    GPRS_GMM_NPDU_NUMBER_LIST_IE         = 0x26,
    GPRS_GMM_CAUSE_IE                    = 0x25,
    GPRS_GMM_MOBILE_ID_IE                = 0x23,
    GPRS_GMM_AUTH_SRES_IE                = 0x22,
    GPRS_GMM_AUTH_RAND_IE                = 0x21,
    GPRS_GMM_AUTH_AUTN_IE                = 0x28,
    GPRS_GMM_AUTH_SRES_EXT_IE            = 0x29,
    GPRS_GMM_AUTH_FAILURE_IE             = 0x30,
    GPRS_GMM_PTMSI_SIGNATURE_IE          = 0x19,
    GPRS_GMM_PTMSI_IE                    = 0x18,
    GPRS_GMM_TIMER_IE                    = 0x17,
    GPRS_GMM_CIPHER_KEY_SEQ_NUM_IE       = 0x80, /* IEI in upper nibble */
    GPRS_GMM_TMSI_STATUS_IE              = 0x90, /* IEI in upper nibble */
    GPRS_GMM_T3302_TIMER_IE              = 0x2A,
    GPRS_GMM_CELL_NOTIFICATION_IE        = 0x8C,
    GPRS_GMM_MS_NETWORK_CAPABILITY_IE    = 0x31,
    GPRS_GMM_PDP_CONTEXT_STATUS_IE       = 0x32,
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL7)
    GPRS_GMM_REQUESTED_MS_INFORMATION_IE = 0xA0,
    GPRS_GMM_UPLINK_DATA_STATUS_IE       = 0x36,
    GPRS_GMM_T3319_VALUE_IE              = 0x37,
#endif /* (UPGRADE_3G_FDD) && (UPGRADE_REL7) */

    /* For SM */
    GPRS_SM_LLC_SAPI_IE                  = 0x32,
    GPRS_SM_QUALITY_OF_SERVICE_IE        = 0x30,
    GPRS_SM_PDP_ADDRESS_IE               = 0x2B,
    GPRS_SM_TIMER_IE                     = 0x29,
    GPRS_SM_ACCESS_POINT_NAME_IE         = 0x28,
    GPRS_SM_TFT_IE                       = 0x36,
    GPRS_SM_NEW_TFT_IE                   = 0x31,
    GPRS_SM_PACKET_FLOW_IDENTIFIER_IE    = 0x34,
    GPRS_SM_TEAR_DOWN_INDICATOR_IE       = 0x90,
    GPRS_SM_PROTOCOL_RADIO_PRIORITY_IE   = 0x80, /* IEI in upper nibble */
    GPRS_SM_PROTOCOL_CONFIG_OPTIONS_IE   = 0x27
#if defined (UPGRADE_NASMDL2)
    ,
    GPRS_SM_REQUEST_TYPE_IE              = 0xA0  /* IEI in upper nibble */
#endif /* UPGRADE_NASMDL2 */
#if defined (UPGRADE_LTE)
    ,
    /* For EMM - some IEI values are same as GPRS GMM */
    EPS_EMM_TMSI_BASED_NRI_CONTAINER_IE  = 0x10,
    EPS_EMM_CLASSMARK2_IE                = 0x11,
    EPS_EMM_HASH_MME_IE                  = 0x4F,
    EPS_EMM_REPLAYED_NAS_MSG_IE          = 0x79,
    EPS_EMM_EPS_MOBILE_IDENTITY_IE       = 0x50,
    EPS_EMM_TRACKING_AREA_IDENTITY_IE    = 0x52,
    EPS_EMM_CAUSE_IE                     = 0x53,
    EPS_EMM_TRACKING_AREA_LIST_IE        = 0x54,
    EPS_EMM_NONCE_UE_IE                  = 0x55,
    EPS_EMM_NONCE_MME_UE                 = 0x56,
    EPS_EMM_EPS_BEARER_CONTEXT_STATUS_IE = 0x57,
    EPS_EMM_UE_NETWORK_CAPABILITY_IE     = 0x58,
    EPS_EMM_T3423_TIMER_IE               = 0x59,
    EPS_EMM_T3412_TIMER_IE               = 0x5A,
    EPS_EMM_T3442_TIMER_IE               = 0x5B,
    EPS_EMM_DRX_PARAMETERS_IE            = 0x5C,
    EPS_EMM_T3412_EXTENDED_IE            = 0x5E,
    EPS_EMM_T3346_TIMER_IE               = 0x5F,
    EPS_EMM_T3324_TIMER_IE               = 0x6A,
    EPS_EMM_T3448_TIMER_IE               = 0x6B,
    EPS_EMM_NETWORK_FEATURE_SUPPORT      = 0x64,
    EPS_EMM_HEADER_CONF_STATUS_IE        = 0x68,
    EPS_EMM_ESM_MESSAGE_CONTAINER_IE     = 0x78,
    EPS_EMM_CLI_IE                       = 0x60,
    EPS_EMM_SS_CODE_IE                   = 0x61,
    EPS_EMM_NAS_MESSAGE_CONTAINER        = 0x67,
    EPS_EMM_UE_RADIO_CAP_INFO_NEEDED_IE  = 0xA0, /* IEI in upper nibble */
    EPS_EMM_EXTENDED_EMM_CAUSE_IE        = 0xA0, /* IEI in upper nibble */
    EPS_EMM_CSFB_RESPONSE_IE             = 0xB0, /* IEI in upper nibble */
    EPS_EMM_NAS_KSI_IE                   = 0xB0, /* IEI in upper nibble */
    EPS_EMM_IMEISV_REQUEST_IE            = 0xC0, /* IEI in upper nibble */
    EPS_EMM_MS_NETWORK_FEATURE_IE        = 0xC0, /* IEI in upper nibble */
    EPS_EMM_NON3GPP_NW_PROVIDED_POL_IE   = 0xD0, /* IEI in upper nibble */
    EPS_EMM_ADDITIONAL_UPDATE_RESULT_IE  = 0xF0, /* IEI in upper nibble */
    EPS_EMM_ADDITIONAL_UPDATE_TYPE_IE    = 0xF0, /* IEI in upper nibble */
    EPS_EMM_OLD_GUTI_IE                  = 0xE0, /* IEI in upper nibble */
    EPS_EMM_T3402_TIMER_IE               = 0x16,
    EPS_EMM_SMS_SERVICES_STATUS_IE       = 0xE0, /* IEI in upper nibble */
    EPS_EMM_DCN_ID_IE                    = 0x65,
    /* For ESM - some IEI values are same as GPRS SM */
    EPS_ESM_PROTOCOL_CONFIG_OPTIONS_IE   = 0x7B, /* Was 0x27, changed to 0x7B for ePCO  */
    EPS_ESM_LEGACY_PCO_IE                = 0x27, /* Legacy IE to be ignored in decoding */
    EPS_ESM_APN_IE                       = 0x28,
    EPS_ESM_NEGOTIATED_QOS_IE            = 0x30,
    EPS_ESM_NEGOTIATED_LLC_SAPI_IE       = 0x32,
    EPS_ESM_PACKET_FLOW_IDENTIFIER_IE    = 0x34,
    EPS_ESM_TFT_IE                       = 0x36,
    EPS_ESM_APN_AMBR_IE                  = 0x5E,
    EPS_ESM_CAUSE_IE                     = 0x58,
    EPS_ESM_EPS_QOS_IE                   = 0x5B,
    EPS_ESM_TRANSACTION_ID_IE            = 0x5D,
    EPS_ESM_SERVING_PLMN_RATE_CONTROL_IE = 0x6E,
    EPS_ESM_RADIO_PRIORITY_IE            = 0x80,  /* IEI in upper nibble */
    EPS_ESM_CONTROL_PLANE_ONLY_IND_IE    = 0x90,
    EPS_ESM_INFO_TRANSFER_IE             = 0xD0,  /* IEI in upper nibble */
    EPS_ESM_RELEASE_ASSISTANCE_IND_IE    = 0xF0,
    EPS_ESM_T3396_TIMER_IE               = 0x37,
    EPS_ESM_BACK_OFF_TIMER_IE            = 0x37,
    EPS_ESM_HEADER_COMPRESS_CONFIG_IE    = 0x66,
    EPS_ESM_RE_ATTEMPT_INDICATOR_IE      = 0x6B,

    UNKNOWN_EPS_IE                       = 0      /* EPS IEs for which no IEI is defined */
#endif /* UPGRADE_LTE */
}
Iei;
#if defined(UPGRADE_3G)
typedef enum CustomConfigBitsTag
{
#if defined(UPGRADE_3G_TDD128)
    PS_COT_BIT = 0,
#endif
    USIM_COT_BIT = 3,
    REALNW_TESTNW_CONFLICT = 8,
    INVALID_BIT = 0xff
}CustomConfigBits;
#endif
/** Specifies the reason for establishment */
typedef enum EstablishmentReasonTag
{
    /** Location update */
    ER_LOCATION_UPDATE,
    /** Re-establishment */
    ER_REESTABLISHMENT,
    /** Ask for a SDCCH */
    ER_SDCCH_REQUIRED,
    /** Speech transport channel full rate required */
    ER_SPEECH_TCH_F_REQUIRED,
    /** Data transport channel full rate required */
    ER_DATA_TCH_F_REQUIRED,
    /** Data transport channel half rate required */
    ER_DATA_TCH_H_REQUIRED,
    /** Speech transport channel half rate required */
    ER_SPEECH_TCH_H_REQUIRED,
    /** Emergency call */
    ER_EMERGENCY_CALL,
    /** Respond to paging */
    ER_RESPOND_TO_PAGING,
    ER_PACKET,       /**< IMPORTANT: All Circuit switched establishment causes must come before ER_PACKET. GRR uses
                      * assumes the first GPRS cause is ER_TBF_ESTABLISHMENT */
    /** One phase access */
    ER_ONE_PHASE_ACCESS,
    /** Short access */
    ER_SHORT_ACCESS,
    /** Two phase access */
    ER_TWO_PHASE_ACCESS,
    /** Packet page response */
    ER_PACKET_PAGE_RESPONSE,
    /** Cell update */
    ER_CELL_UPDATE,
    /** GMM procedure */
    ER_GMM_PROCEDURE,
    /** L1 does stop sending RACH bursts on receipt of the packet channel config.*/
    ER_SINGLE_BLOCK_NO_TBF,
    /** Last establishment reason */
    ER_LAST_ESTAB_REASON
}
EstablishmentReason;

/** Normal cell indication element */
typedef enum NormalCellIndTag
{
    /** Out of range timing advance is ignored */
    OUT_OF_RANGE_TA_IGNORED,
    /** Out of range timing advance shall trigger a handover failure procedure   */
    OUT_OF_RANGE_TA_TRIGGERS
}
NormalCellInd;

typedef enum ChannelNeededTag
{
    ANY_CHANNEL   =   0,
    SDCCH         =   1,                /* FR9805-2401: Corrected spelling mistake! */
    TCH_FULL_RATE =   2,
    TCH_DUAL_RATE =   3
}
ChannelNeeded;

/** CCCH Configuration */
typedef enum CcchConfigurationTag
{
    /** 1 CCCH channel */
    CCCH_1_CHANNEL            =   0,
    /** 1 CCCH channel with SDCCH */
    CCCH_1_CHANNEL_WITH_SDCCH =   1,
    /** 2 CCCH channels */
    CCCH_2_CHANNELS           =   2,
    /** 3 CCCH channels */
    CCCH_3_CHANNELS           =   4,
    /** 4 CCCH channels */
    CCCH_4_CHANNELS           =   6
}
CcchConfiguration;

/** Specifies the type of service */
typedef enum CmServiceTypeTag
{
    /** MO call establishment */
    MO_CALL_ESTABLISHMENT        =   1,
    /** Emergency call establishment */
    EMERGENCY_CALL_ESTABLISHMENT =  2,
    /** SMS transfer */
    SHORT_MESSAGE_TRANSFER       =   4,
    /** Supplmentary service */
    SUPPLEMENTARY_SERVICE_ACT    =   8
}
CmServiceType;

/** Specifies if a normal updating, a periodic updating or an IMSI attach is wanted */
typedef enum LocationUpdatingTypeTag
{
    /** Normal location updating */
    NORMAL_LOCATION_UPDATING =   0,
    /** Periodic updating */
    PERIODIC_UPDATING        =   1,
    /** IMSI attach */
    IMSI_ATTACH              =   2
}
LocationUpdatingType;

/** Specifies whether or not the receiver is ready */
typedef enum CongestionLevelTag
{
    /** receiver ready */
    RECEIVER_READY     =   0x00,
    /** receiver not ready */
    RECEIVER_NOT_READY =   0x0F
}
CongestionLevel;

typedef enum FreqListEncodedFormatTag
{
    CELL_CHANNEL_DESCR_FORMAT = 0,
    NCELL2_DESCR_FORMAT
}
FreqListEncodedFormat;

typedef enum CellSelectionIndicCellTypeTag
{
    CELL_INDIC_TYPE_GSM         = 0,
#if defined (UPGRADE_3G_FDD)
    CELL_INDIC_TYPE_UTRA_FDD    = 1,
#endif /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)
    CELL_INDIC_TYPE_UTRA_TDD128 = 2,
#endif /* UPGRADE_3G_TDD128 */
    CELL_INDIC_TYPE_NOT_PRESENT = 3
}
CellSelectionIndicCellType;

/*****************************************************************
 *** L3 Message elements
 *****************************************************************/
/** Measurement record elements */
typedef struct MeasurementRecordTag
{
    /** Received signal strength of the neighbour cell */
    Int16 rxLev;
    /** BCCH carrier frequency of the neighbour cell */
    Int16 bcchFreq;
    /** Base Station Identity Code of the neighbour cell */
    Int16 bsic;
}
MeasurementRecord;

typedef struct CellIdElementTag
{
    CellId value;
}
CellIdElement;

/** Cell Options (BCCH) information element.
 * The purpose of the Cell Options (BCCH) information element is to provide
 * a variety of information about a cell.
 */
typedef struct CellOptionsElementTag
{
    /** Indicates if power control is set or not */
    Boolean      pwrc;
    /** DTX indicator */
    DtxIndicator dtx;
    /** Radio link timeout from 4 to 64 step by 4 */
    Int8         radioLinkTimeout;
    /** Dynamic ARFCN mapping */
    Boolean      dnInd;
}
CellOptionsElement;

/** Parameters used for cell selection */
typedef struct CellSelectionParamTag
{
    /** Indicates the Hysteresis which applies in Ready state for cells in same RA;
     * it ranges from 0 to 14 dB in steps of 2 dB.
     * \note Cell  reselection hysteresis is translated from raw value using
     * formula:  2 * raw value
    */
    Int16   cellReselectHysteresis;
    /** The maximum allowed power for transmission on CCH in the service cell */
    Int8    msTxpwrMaxCch;
    /** Minimum access level for serving cell */
    Int8    rxlevAccessMin;
    /** Additional reselect param indication.
     * In System Information type 3 message:  <UL>
     * <LI> 0   The SI 4 rest octets, if present, and SI 7
     * and SI 8 rest octets, if so indicated in the
     * SI 4 rest octets shall be used to derive the
     * value of PI and possibly C2 parameters and/or
     * other parameters
     * <LI> 1    System information type 16 and 17 are
     * broadcast on the BCCH.
     * A mobile station which does not support System
     * information type 16 and 17 may consider this bit
     * as "0".  </UL>
     *
     * In System Information type 4 message:<UL>
     * <LI>  0   The SI 4 rest octets, if present, shall be
     *  used to derive the value of PI and possibly
     * C2 parameters and/or other parameters
     * <LI> 1   The value of PI and possibly C2 parameters
     * and/or other parameters in a System
     * information type 7 or type 8 message shall
     *  be used
     * </UL>
     */
    Boolean additionalCellParams;
    /** Whether new establishment cause are supported or not.
     * 0   New establishment causes are not supported
     *
     * 1   New establishment causes are supported
     */
    Boolean newEstabCausesSupported;
}
CellSelectionParamElement;

/** Mode of a dedicated channel.
 * This enumerator is used during dedicated channel setup to specify channel
 * mode (signaling-only, speech or data), mode version and data rate.
 *
 * \see #ChannelModeTag
 */
typedef enum ChannelMode2Tag
{
    CHM2_SIGNALLING            =   0x00,
    CHM2_SPEECH_HALF_RATE      =   0x05,
    CHM2_DATA_4800_HALF        =   0x0F,
    CHM2_SPEECH_HALF_RATE_VER2 =   0x25,
    CHM2_SPEECH_HALF_RATE_VER3 =   0x45,
    CHM2_DATA_2400_HALF        =   0x17
}
ChannelMode2;

typedef struct ChannelDescriptorTag
{
    ChannelType type;
    ChannelMode mode;
    Int8        subchannel;
    Int8        timeslot;
    Int8        tsc;
    Boolean     hopping;
    Int8        maio;
    Int8        hsn;
    Arfcn       arfcn;
}
ChannelDescriptor;

/** Channel Mode information element */
typedef struct ChannelModeElementTag
{
    /** Indicates the channel mode */
    ChannelMode mode;
}
ChannelModeElement;

/** Channel Mode 2 information element */
typedef struct ChannelMode2ElementTag
{  /* FR9802-2042: Separated out channel mode 1 and channel mode 2. */
    /** Gives information of the mode of coding/decoding and transcoding */
    ChannelMode2 mode;
}
ChannelMode2Element;

/** The purpose of the Cipher Mode Setting information element is to indicate whether stream
 * ciphering shall be started or not and if it is to be started, which algorithm to use.
 */
typedef struct CipherModeSettingElementTag
{
    /** Whether or not to start ciphering */
    Boolean         startCiphering;
    /** algorithm identifier */
    CipherAlgorithm algorithm;
}
CipherModeSettingElement;

/** The Cipher Response information element is used by the network to indicate
 * to the mobile station which information the mobile station has to include
 * in the CIPHERING MODE COMPLETE message.
 */
typedef struct CipherResponseElementTag
{
    /** Indicates if IMEI shall be included or not */
    Boolean imeiIncluded;
}
CipherResponseElement;

typedef struct ControlChannelDescrElementTag
{
    Boolean           attachDetachAllowed;
    Int8              numAccessGrantBlocks;
    CcchConfiguration ccchConf;
    /* raw value + 2 */
    Int8              bsPaMfrms;
    Int8              t3212;
    Boolean           mscR99OrLater;                /* Release 99 addition */
}
ControlChannelDescrElement;

typedef struct HandoverReferenceElementTag
{
    Int8 value;
}
HandoverReferenceElement;

/** Measurement results elements.
 * The purpose of the Measurement Results information element is to provide
 * the results of the measurements made by the mobile station on the serving
 * cell and the neighbour cells.
 *
 * The Measurement Results is a type 3 information element with 17 octets length.
 */
typedef struct MeasurementResultsElementTag
{
    /** BCCH coding type */
    Int8              baUsed;
    /** Indicating whether the mobile equipment uses DTX (DTX=Discontinuous transmission mechanism) or not,
     * i.e a power level transmission which is not continuous in the previous measurement period
     */
    Boolean           dtxUsed;
    /** Received full signal strengths on serving cell;
     * intensity of the signal received from the serving cell, measured on all slots
     */
    Int16             rxlevFullServingCell;
    /** Received subset signal strengths on serving cell;
     * intensity of the signal received from the serving cell, measured on a subset of slots
     */
    Int16             rxlevSubServingCell;
    /** Indicates the validity of serving cell masurements */
    Boolean           scMeasValid;
    /** Indicates the validity of Ncell masurements */
    Boolean           ncMeasValid;
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    /** Indicates if 3G BA is used */
    Boolean           ba3gUsed;
#endif  /* UPGRADE_3G && UPGRADE_GGE */
    /** Received full signal qualities on serving cell;
     * quality of the signal received from the serving cell measured on all slots
     */
    Int16             rxqualFullServingCell;
    /** Received subset signal qualities on serving cell;
     * quality of the signal received from the serving cell measured on a subset of slots
     */
    Int16             rxqualSubServingCell;
    /** Number of measurements of neighboring cells */
    Int16             numNcellMeasurements;
    /** Holds a list of measurement records for each Neighbouring Cell */
    MeasurementRecord ncell[NUM_MEAS_RECORDS];
}
MeasurementResultsElement;

/** NCC Permitted information element.
 * The purpose of the NCC Permitted information element is to provide a
 * definition of the allowed NCCs on the BCCH carriers to be reported in
 * the MEASUREMENT REPORT message by the mobile stations in the cell.
 */
typedef struct NccPermittedElementTag
{
    /** NCC permitted.
     * The  NCC permitted field is coded as a bit map, i.e
     * bit N is coded with a "0" if the BCCH carrier with
     * NCC = N-1 is not permitted for monitoring and with a
     * "1" if the BCCH carrier with NCC = N-1 is permitted
     * for monitoring; N = 1,2,..,8.
     */
    Int8 nccBitMap;
}
NccPermittedElement;

/** The purpose of the Page Mode information element is to control the action
 * of the mobile station belonging to the paging subgroup corresponding to the
 * paging subchannel.
 */
typedef struct PageModeElementTag
{
    /** Specifies the mode of paging */
    PageMode mode;
}
PageModeElement;

typedef struct PowerCommandElementTag
{
    Int8    powerLevel;
    Boolean atc;
}
PowerCommandElement;

/** RACH Control Parameters information element.
 * The purpose of the RACH Control Parameters information element is to
 * provide parameters used to control the RACH utilization. This information
 * element is broadcast to mobile stations in SYSTEM INFORMATION TYPE 1, 2, 2bis, 3,
 * and 4 messages.
 */
typedef struct RachControlParamElementTag
{
    /** Maximum  number  of retransmissions   */
    Int16       maxRetrans;
    /** Number of slots to spread transmission   */
    Int16       txInteger;
    /** Cell Barred for Access; indicates whether or not The cell is barred */
    Boolean     cellBarAccess;
    /** Call reestablishment allowed.
     * This field indicates:<UL>
     * <LI> 0  Call Reestablishment allowed in the cell
     * <LI> 1  Call Reestablishment not allowed in the cell
     *</UL>
     */
    Boolean     reestabNotAllowed;
    /** Access Control Class */
    AccessClass accessClass;
}
RachControlParamElement;

/** The purpose of the Request Reference information element is to provide the random
 * access information used in the channel request and the frame number, FN modulo 42432
 * in which the channel request was received.
 */
typedef struct RequestReferenceElementTag
{
    /** Random Access Information; this is an unformatted 8 bit field */
    Int16 ra;
    /** The frame number */
    Int16 frameNumber;
}
RequestReferenceElement;

/** The purpose of the RR Cause information element is to provide
 * the reason for release or the reason for completion of an
 * assignment or handover.
 */
typedef struct RrCauseElementTag
{
    /** Cause value */
    GsmCause value;
}
RrCauseElement;

typedef struct BaRangeTag
{
    Int16 higherRange;
    Int16 lowerRange;
}
BaRange;

typedef struct BaRangeElementTag
{
    Int8    numberOfRanges;
    BaRange arfcnRange[MAX_BA_RANGES];
}
BaRangeElement;

#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
typedef struct UtranFreqListElementTag
{
#if defined (UPGRADE_3G_FDD)
    Int16 numFddArfcn;
    Int16 fddArfcn[UTRAN_FREQ_LIST_ELEMENT_SIZE];
#endif /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)
    Int16   numTdd128Arfcn;
    Int16   tdd128Arfcn[UTRAN_FREQ_LIST_ELEMENT_SIZE];
#endif /* UPGRADE_3G_TDD128 */

}
UtranFreqListElement;
#endif  /* UPGRADE_3G && UPGRADE_GGE */

typedef struct IaRestOctetsElementTag
{
    /** Indicates if mobile allocation before time is present */
    Boolean                 maBeforeTimePresent;
    /** Mobile allocation index offset; 6 bit field range: 0 to 63 */
    Int8                    maio;
    /** The Mobile Allocation contains a bitmap referring to the Cell Channel
     * Description IE in SI 1 message
     */
    MobileAllocationElement ma;
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    /** Indicates if compressed INTER-RAT information is allowed */
    Boolean                 compressedInterRatInfoAllowed;
#endif  /* UPGRADE_3G && UPGRADE_GGE */
}
IaRestOctetsElement;

typedef struct Si4RestOctetsElementTag
{
    Boolean     c2ParamsPresent;
    Boolean     cellBarQualify;
    SignedInt16 cellReselectOffset;
    Int16       temporaryOffset;
    Int16       penaltyTime;
    Boolean     si2TerAvailable;
    Boolean     ecsControl;
    Boolean     powerOffsetPresent;
    Int8        dcsPowerOffset;
    Boolean     gprsIndicatorPresent;
    Boolean     whereIndicatorPresent;
    Int8        si9Location;
    Int8        raColour;
    Boolean     si13IsInBcchExt;
    Boolean     cellIdPresent;
    CellId      cellId;
}
Si4RestOctetsElement,
Si7RestOctetsElement,
Si8RestOctetsElement;

typedef struct Si3RestOctetsElementTag
{
    Boolean     c2ParamsPresent;
    Boolean     cellBarQualify;
    SignedInt16 cellReselectOffset;
    Int16       temporaryOffset;
    Int16       penaltyTime;
    Boolean     si2TerAvailable;
    Boolean     ecsControl;
    Boolean     powerOffsetPresent;
    Int8        dcsPowerOffset;
    Boolean     gprsIndicatorPresent;
    Boolean     whereIndicatorPresent;
    Int8        si9Location;
    Int8        raColour;
    Boolean     si13IsInBcchExt;
    Boolean     ecsControl3g;
    Boolean     si2QuaterAvailable;
    Boolean     si2QuaterInBcchExt;
}
Si3RestOctetsElement;

/** SI 1 Rest Octets information element.
 * Is a type 5 information element with 1 octet length
 * contains the position about the NCH and band indicator bits.
 */
typedef struct Si1RestOctetsElementTag
{
    /** Position on the CCCH.
     * The values in the NCH Position field indicates the block
     * number of the CCCH block which is used for the
     * first NCH block and the number of blocks used
     * for the NCH.
     */
    Int8  nchPosition;
    /** The band indicator */
    Int16 bandIndicator;
}
Si1RestOctetsElement;

typedef struct PchNchInfoTag
{
    Boolean pagingChannelRestructured;
    Int8    nlnSacch;
    Int8    callPriority;
    Boolean nlnStatus;
}
PchNchInfo;

typedef struct Si6RestOctetsElementTag
{
    Boolean    pchNchInfoPresent;
    PchNchInfo pchNchInfo;

    /* VBS and VGCS options not supported */

    /* Now DTM additions .... */
    Boolean    dtmSupported;
#if defined(ENABLE_REL6_EXTENSIONS)
    Int8       rac;
    Int8       maxLapdm;
#endif  /* (ENABLE_REL6_EXTENSIONS) */

    /* Finally  the band indicator */
    Int16      bandIndicator;
}
Si6RestOctetsElement;

typedef enum SchedulingInfoType5Tag
{
    SYS_INFORMATION_TYPE_1    = 0x00,
    SYS_INFORMATION_TYPE_2    = 0x01,
    SYS_INFORMATION_TYPE_2bis = 0x02,
    SYS_INFORMATION_TYPE_2ter = 0x03,
    SYS_INFORMATION_TYPE_3    = 0x04,
    SYS_INFORMATION_TYPE_4    = 0x05,
    SYS_INFORMATION_TYPE_7    = 0x06,
    SYS_INFORMATION_TYPE_8    = 0x07,
    SYS_INFORMATION_TYPE_9    = 0x08,
    SYS_INFORMATION_TYPE_13   = 0x09,
    LAST_SCEHDULING_INFO      = 0x0a
}
SchedulingInfoType5;

/** BCCH scheduling information element */
typedef struct BcchSchedulingInfoTag
{
    /** Contains a binary encoded non-negative integer number assigned
     * to a type of information sent on the BCCH
     */
    SchedulingInfoType5 schedInfoType5;
    /** Encodes the position modulus */
    Int8                modulus[MAX_BCCH_OCCURENCE];
    Boolean             bcchExt[MAX_BCCH_OCCURENCE];
    /** Contains the N+1 bit binary encoding of a non-negative integer number < 2N+1. */
    Int16               relPosition[MAX_BCCH_OCCURENCE];
}
BcchSchedulingInfo;

/** SI 9 Rest Octets element.
 * The SI 9 Rest Octets information element contains information about
 * scheduling of some or all of the information on the BCCH.
 *
 * The SI 9 Rest Octets information element is a type 5 information
 * element with 17 octets length.
 */
typedef struct Si9RestOctetsElementTag
{
    /** Indicates how many entries in the schedInfo */
    Int8               numSchedulingInfo;
    /** Indicates one or more information types (in info type)
     * together with their positions
     */
    BcchSchedulingInfo schedInfo[MAX_BCCH_SCHEDULING_INFO];
}
Si9RestOctetsElement;

/** Synchronization Indication information element.
 * It specifies which type of handover is to be performed.
 */
typedef struct SyncIndicationElementTag
{
    /** Indicates what kind of synchronization to perform */
    Synchronisation synchronisation;
    /** Normal cell indication */
    NormalCellInd   nci;
    /** Report Observed Time Difference.
     * This field is coded as follows: <UL>
     * <LI> 0    Mobile Time Difference IE shall not be included
     * in the HANDOVER COMPLETE message
     * <LI> 1    Mobile Time Difference IE shall be included in
     * the HANDOVER COMPLETE message
     *</UL>
     */
    Boolean         rot;
}
SyncIndicationElement;

typedef struct TimeDifferenceElementTag
{
    Int8 value;
}
TimeDifferenceElement;

/** The purpose of the Timing Advance information element is to provide the timing advance value. */
typedef struct TimingAdvanceElementTag
{
    /** Timing advance value.
     *  The  coding of the timing advance value field is the binary  representation
     * of the timing advance in bit periods.
     */
    Int8 value;
}
TimingAdvanceElement;

typedef struct WaitIndicationElementTag
{
    Int16 t3122;
}
WaitIndicationElement;

/** CM Service Type information element.
 * The CM Service Type is a type 1 information element (3GPP TS 24.008).
 */
typedef struct CmServiceTypeElementTag
{
    /** Specifies which service is requested from the network */
    CmServiceType type;
}
CmServiceTypeElement;

/** Location updating type information element.
 * to indicate whether a normal updating, a periodic updating
 * or an IMSI attach is wanted. It may also indicate that a
 * follow-on request has been received from the mobile station CM layer.
 */
typedef struct LocationUpdatingTypeElementTag
{
    /** Location Updating Type */
    LocationUpdatingType lut;
    /** Indicates if No follow-on request pending or follow-on request pending */
    Boolean              foRequestPending;
}
LocationUpdatingTypeElement;

typedef struct RejectCauseElementTag
{
    GsmCause rejectCause;
}
RejectCauseElement;

typedef struct AuxiliaryStatesElementTag
{
    HoldAuxiliaryState holdState;
    MptyAuxiliaryState mptyState;
}
AuxiliaryStatesElement;

/** Call state element.
 * The call state information element is coded as defined in TS 24.008
 */
typedef struct CallStateElementTag
{
    CodingStandard codingStandard;
    /** Describes the current status of a call */
    CallState      value;
}
CallStateElement;

/** Congestion level information element */
typedef struct CongestionLevelElementTag
{
    /** Describes the congestion status of the call.*/
    CongestionLevel level;
}
CongestionLevelElement;

/** Keypad facility element.
 * The purpose of the keypad facility information element is to convey
 * IA5 characters, e.g. entered by means of a terminal keypad.
 */
typedef struct KeypadFacilityElementTag
{
    /** DTMF character */
    DtmfCharacter keypadInformation;
}
KeypadFacilityElement;

/** Notification indicator element.*/
typedef struct NotificationIndicatorElementTag
{
    /** Indicate information pertaining to a call.*/
    NotificationDescrip descr;
}
NotificationIndicatorElement;

/** Repeat indicator element */
typedef struct RepeatIndicatorElementTag
{
    /** Indicator */
    RepeatIndication indication;
}
RepeatIndicatorElement;

/** Signal element */
typedef struct SignalElementTag
{
    /** Holds signal information */
    SignalInfo value;
}
SignalElement;

/** User-user Element.
 * The purpose of the user-user information element is to convey
 * information between the mobile station and the remote ISDN user.
 */
typedef struct UserToUserTag
{
    /** Length of user-user contents
\assoc ARRAY \ref info */
    Int16               infoLength;
    /** User-user protocol discriminator */
    UserProtocolDiscrim protocolDisc;
    /** Lists User-user information */
    Int8                info[MAX_USER_TO_USER_LENGTH];
}
UserUserElement;

/** CP-Cause element.
 * This element is included in the CP-ERROR message.
 */
typedef struct CpCauseElementTag
{
    /** Cause value */
    GsmCause cause;
}
CpCauseElement;

/** CP-User data element layout.
 * The CP-User data element is used to carry the RPDU.
 * It has an information element identifier, a length
 * indicator and a data field. The data field will contain
 * the RPDUs. The maximum length of the data field is 248 octets.
 */
typedef struct CpUserDataElementTag
{
    /** Length indicator
\assoc ARRAY \ref rpduInformation */
    Int16 rpduInfoLength;
    /** The RPDUs*/
    Int8  rpduInformation[MAX_RPPDU_INFO_LENGTH];
}
CpUserDataElement;

/** Authenetication failure parameter of type AUT */
typedef Aut AuthParamFailureElement;

/* StreamIdentifier, CauseOfNoCli, NetworkCallControlCapabilities have
   moved to mnl3_typ.h */

#if defined(UPGRADE_AMR_PS)
/** Specifies the version of multirate */
typedef enum MultirateVersionTag
{
    /** Multirate speech version 1 */
    ADAPTIVE_MR_VERSION_1 = 0x01,
    /** Multirate speech version 2 */
    ADAPTIVE_MR_VERSION_2 = 0x02,
    /** Reserved */
    ADAPTIVE_MR_RESERVED
}
MultirateVersion;

/** MultiRate configuration information element */
typedef struct MultirateConfigElementTag
{
    /** The number of codec sets */
    Int8             numCodecSets;
    /** Multirate speech version */
    MultirateVersion mrVersion;
    /** Noise Suppression Control Bit.
     * \internal For future use: noise suppression not currently supported.
     */
    Boolean          nscb;
    /** Initial multirate codec indicator.
     * This field is coded as follows: <UL>
     * <LI> 0   The initial codec mode is defined by the implicit rule
     * <LI> 1   The initial codec mode is defined by the Start Mode field
     *</UL>
     */
    Boolean          imci;
    /** The initial codec mode is coded as in GSM 05.09  */
    Int8             startMode;
    /** The number of set of AMR codec  modes*/
    Int8             setOfCodecModes;
    /** The threshold of C/I in dB, as defined in GSM 05.09 */
    Int8             threshold[3];
    /** The threshold in dB as defined in GSM 05.09 */
    Int8             hysteresis[3];
}
MultirateConfigElement;
#endif  /* (UPGRADE_AMR_PS) */

/** Lists the frequencies that are allocated to the mobile.
 */
typedef struct MobileAllocationTag
{
    Int8  numChannels;                   /**< Number of valid elements in \a array[]
\assoc ARRAY \ref array */
    /** List of frequencies allocated to the mobile.*/
    Arfcn array[MAX_MA_CHANNELS];
}
MobileAllocation;

/*****************************************************************
 *** L3 Messages
 *****************************************************************/
/** Additional assignment message content */
typedef struct AdditionalAssignmentMessageTag
{
    /** Channel description; provides a description of an allocable channel */
    ChannelDescrElement     channelDescr;
    /** Indicates if mobileAllocation contains relevant information
\assoc PRESENT \ref mobileAllocation */
    Boolean                 mobileAllocationPresent;
    /** Mobile Allocation.
     * This information element shall appear if the Channel Description
     * information element indicates frequency hopping.
     *
     * If the Channel Description IE does not indicate frequency hopping and
     * the information element is present it shall be considered as an IE unnecessary in the message.
     */
    MobileAllocationElement mobileAllocation;
    /** Indicates if startingTime contains relevant information
\assoc PRESENT \ref startingTime */
    Boolean                 startingTimePresent;
    /** This information element appears in particular if e.g, a change of frequency is planned */
    StartingTimeElement     startingTime;
}
AdditionalAssignmentMessage;

/** Immediate assignment message content */
typedef struct ImmediateAssignmentMessageTag
{
    /** Indicates the mode of paging */
    PageModeElement         pageMode;
    /** If the dedicatedModeOrTbf indicates that the message assigns a
     * dedicated mode resource, the mobile station shall consider this information
     * element  present in the message.
     */
    ChannelDescrElement     channelDescr[2];
    /** If this message is used in an assignment of a downlink TBF, the network
     * shall code this information element, e.g. by using a suitably offset frame
     * number, such that the resource reference cannot be confused with any
     * CHANNEL REQUEST message sent by a mobile station.
     */
    RequestReferenceElement requestReference[2];
    /** If the iaRestOctets indicates that this message is the second message
     * of a two-message assignment of an uplink or downlink TBF, the mobile station
     * shall ignore the contents of this information element and regard it as an
     * unnecessary IE.
     */
    TimingAdvanceElement    timingAdvance[2];
    /** If this message assigns a dedicated mode resource and the Channel
     * Description IE does not indicate frequency hopping , the length
     * indicator of this information element shall be set to zero, and the
     * mobile station shall consider the IE as an unnecessary IE.
     */
    MobileAllocationElement mobileAllocation;
    /** Whether or not startingTime is present
\assoc PRESENT \ref startingTime */
    Boolean                 startingTimePresent;
    /** This information element appears if e.g a frequency change is in progress */
    StartingTimeElement     startingTime;
    /** If the starting time IE is present but not the frequency parameters, before
     * time construction, the mobile stations must wait until the starting time before
     * accessing the channel.
     */
    IaRestOctetsElement     iaRestOctets;
    Int8                    dedicatedModeOrTbfMask;         /**< Used to filter out unwanted TBF assignments in the IA */
    /** A mobile station not supporting GPRS may ignore the contents
     * of this information element and regard it as an unnecessary IE.
     * Such mobile station shall assume that this message assigns a
     * dedicated mode resource.
     */
    Int8                    dedicatedModeOrTbf;
    /** Indicates the type of assignment */
    IaPacketAssignmentType  packetAssignmentType;
    IaTbfParams             param;
    /** If the Dedicated mode or TBF IE indicates that the message assigns a
     * Temporary Block Flow (TBF), the mobile station shall consider this
     * information element  present in the message. If the Dedicated mode or
     * TBF IE indicates that this message is the first of two in a two-message
     * assignment of an uplink or downlink TBF, the mobile station shall ignore
     * the contents of this information element and regard it as an unnecessary IE.
     */
    IaPacketChannelDescr    packetChannelDescr;
    /** Contains raw mobile allocation (MA) */
    GprsRawMa               rawMa;
}
ImmediateAssignmentMessage,
/** Immediate assignment extended message content. */
ImmediateAssignmentExtMessage;

/** Immediate assignment reject message content.
 * This message is sent to change the channel configurations of MS
 * to different dedicated configurations while they stay in the same cell.
 */
typedef struct ImmediateAssignmentRejMessageTag

{
    /** Specifies the type of paging */
    PageModeElement         pageMode;
    /** Request reference */
    RequestReferenceElement requestReference[NUM_REJECT_PER_MSG];
    /** When immediate assignment reject message is for RR connection establisment
     * then this IE contains timeout value for T3122. If  immediate assignment reject
     * message is for TBF establisment for GPRS MS then this IE contain timeout value for T3142.
     */
    WaitIndicationElement   waitIndication[NUM_REJECT_PER_MSG];
#if defined(UPGRADE_EDGE)
    Int8                    extendedRa[NUM_REJECT_PER_MSG];
#endif  /* (UPGRADE_EDGE) */
}
ImmediateAssignmentRejMessage;

/** Ciphering mode command message content */
typedef struct CipheringModeCommandMessageTag
{
    /** Sets the ciphering algorithm */
    CipherModeSettingElement cipherModeSetting;
    /** Defines the information included by MS in the Cipher Response */
    CipherResponseElement    cipherResponse;
}
CipheringModeCommandMessage;

/** Ciphering mode complete message content */
typedef struct CipheringModeCompleteMessageTag
{
    /** Indicates if mobile ID is present
\assoc PRESENT \ref mobileId */
    Boolean         mobileIdPresent;
    /** This information element is included if and only if the mobile station
     * shall include its IMEISV
     */
    MobileIdElement mobileId;
}
CipheringModeCompleteMessage;

/** Assignment command message content.
 * This message is sent on the main DCCH by the network to the mobile station
 * to change the channel configuration to another independent dedicated channel
 * configuration, when no timing adjustment is needed.
 */
typedef struct HandoverCommandMessageTag
{
    /* mandatory fields */
    /** Channel Description; not present in assignment */
    CellDescrElement         cellDescr;
    /** Channel description; provides a description of an allocable channel */
    ChannelDescrElement      channelDescr;
    /** Handover reference; not present in assignment */
    HandoverReferenceElement handoverReference;
    /** Provides the power level to be used by the mobile station */
    PowerCommandElement      powerCommand;
    /** Indicates if frequencyList is present */
    Boolean                  frequencyListPresent;
    /** Provides the list of the absolute radio frequency channel
     * numbers used in a frequency hopping sequence.
     */
    FrequencyListElement     frequencyList;
    /** Indicates if Frequency Short List is present.
     * The purpose of the Frequency Short List information element
     * is to provide the list of the absolute radio frequency channel
     * numbers used in a frequency hopping sequence, in a small fixed
     * length information element to obtain when possible the HANDOVER COMMAND
     * message in a single block.
     */
    Boolean                  frequencyShortListPresent;         /* after time */
    /** Indicates if Frequency Channel Sequence is present.
     * The purpose of the Frequency Channel Sequence information element
     * is to provide the absolute radio frequency channel numbers used
     * in the mobile hopping sequence. This information element shall
     * only be used for radio frequency channels in the primary GSM band
     */
    Boolean                  freqChannelSequencePresent;        /* after time */
    /** Indicates if frequencyListBeforeTime contains valid information */
    Boolean                  freqListBeforeTimePresent;
    FrequencyListElement     frequencyListBeforeTime;

    /** Whether or not mobileAllocationBeforeTime contains valid information */
    Boolean                  mobileAllocBeforeTimePresent;
    /** Indicates if mobileAllocationBeforeTime contains valid information */
    MobileAllocationElement  mobileAllocationBeforeTime;
    /** Indicates if channelDescrBeforeTime is present */
    Boolean                  channelDescrBeforeTimePresent;
    /** If the channelDescrBeforeTime is not present,
     * the channel description to apply for before the time, if needed, is given
     * by the description of the first channel, after time IE.
     */
    ChannelDescrElement      channelDescrBeforeTime;

    /** Indicates ifchannelDescr2BeforeTime contains valid information */
    Boolean                  channelDescr2BeforeTimePresent;
    ChannelDescrElement      channelDescr2BeforeTime;
    /** Indicates if cellChannelDescr is present
\assoc PRESENT \ref cellChannelDescr */
    Boolean                  cellChannelDescrPresent;
    /** If present, this information element shall be used to decode the
     * Mobile Allocation IE in the same message and in subsequent messages.
     */
    CellChannelDescrElement  cellChannelDescr;
    /** Whether or not channel mode is present
\assoc PRESENT \ref channelMode */
    Boolean                  channelModePresent;
    /** Gives information of the mode on coding/decoding and transcoding.
     * The exact mode is determined by the contents of this IE and the channel type.
     */
    ChannelModeElement       channelMode;
    /** Indicates if channelDescr2 is present
\assoc PRESENT \ref channelDescr2 */
    Boolean                  channelDescr2Present;
    /** Description of the Second Channel.
     * These information element appear if the mobile station carries two connections
     * (on two dedicated channels, for the TCH/H+TCH/H configuration).
     */
    ChannelDescrElement      channelDescr2;
    /** Indicates if channelMode2 is present
\assoc PRESENT \ref channelMode2 */
    Boolean                  channelMode2Present;
    /** Mode of the Second Channel.
     * This element appears at least when the channel mode is changed for
     * the channel defined in the Description of the Second Channel information element.
     */
    ChannelMode2Element      channelMode2;
/** \assoc PRESENT \ref mobileAllocation */
    Boolean                  mobileAllocationPresent;
    /** The purpose of the mobileAllocation is to provide that part of the RF
     * channels belonging to the cell allocation (coded with a "1" in the cell
     * channel description information element) which is used in the mobile
     * hopping sequence.
     */
    MobileAllocationElement  mobileAllocation;         /* after time MA */
    /** Indicates if startingTime is present
\assoc PRESENT \ref startingTime */
    Boolean                  startingTimePresent;
    /** Starting Time.
     * The starting time information element is included when the network wants
     * the mobile station to change the frequency parameters of the channels more
     * or less at the moment a change of channel occurs. In this case a number of
     * information elements may be included to give the frequency parameters to be
     * used before the starting time.
     */
    StartingTimeElement      startingTime;
    /** Indicates if cipherModeSetting is present
\assoc PRESENT \ref cipherModeSetting */
    Boolean                  cipherModeSettingPresent;
    /** If this information element is omitted, the mode of ciphering is not changed
     * after the mobile station has switched to the assigned channel.
     */
    CipherModeSettingElement cipherModeSetting;

    /*
    the following optional fileds are only ever present in
    handover command message
    */
    /** Indicates if timeDifference contains valid information
\assoc PRESENT \ref timeDifference */
    Boolean                  timeDifferencePresent;
    /**  Provides information about the synchronization difference
     * between the time bases of two Base Stations.
     */
    TimeDifferenceElement    timeDifference;
    /** Indicates if timingAdvance contains valid information
\assoc PRESENT \ref timingAdvance */
    Boolean                  timingAdvancePresent;
    /** This information element shall appear if the syncIndication
     * indicates a presynchronized handover. If not included for a presynchronized handover,
     * then the default value as defined in GSM 05.10 shall be used. For other types of
     * handover it shall be considered as an unnecessary information element.
     */
    TimingAdvanceElement     timingAdvance;
    /** Indicates if Synchronization Indication is present
\assoc PRESENT \ref syncIndication */
    Boolean                  syncIndicationPresent;
    /** Indicates which type of handover is to be performed */
    SyncIndicationElement    syncIndication;
#if defined(UPGRADE_AMR_PS)
    /** Indicates if multirateConfig is present */
    Boolean                  multirateConfigPresent;
    /** This information element appears if the Mode of the First Channel
     * indicates a multi-rate speech codec, and if the assigned configuration
     * is new, i.e it is different from the MultiRateconfiguration of a
     * previously allocated channel in the cell.
     */
    MultirateConfigElement   multirateConfig;
#endif  /* (UPGRADE_AMR_PS) */
}
AssignmentCommandMessage,
/** Handover command message content.
 * This message is sent on the main DCCH by the network to the mobile
 * station to change the dedicated channel configuration, timing adjustment
 *  needed.
 */
HandoverCommandMessage;

/** Handover complete message content */
typedef struct HandoverCompleteMessageTag
{
    /** The reason for completion of an handover*/
    RrCauseElement rrCause;
    /** Indicates if otd contains relevant information
\assoc PRESENT \ref otd */
    Boolean        timeDifferencePresent;
    /** This information element is included if and only if the
     * Synchronization Indication IE in the HANDOVER COMMAND message requests
     * it to be sent.
     */
    Otd            otd;
}
HandoverCompleteMessage;

/** Assignment complete message content.
 * This message is sent on the main DCCH from the mobile station to the network
 * to indicate that the mobile station has established the main signalling link
 * successfully.
 */
typedef struct AssignmentCompleteMessageTag
{
    /** The reason that triggers sending this message */
    RrCauseElement rrCause;
}
AssignmentCompleteMessage,
/** Assignment failure message content.
 * Indicates that the mobile station has failed to seize the new channel.
 */
AssignmentFailureMessage,
/** Handover failure message content.
 * Indicates that the mobile station has failed to seize the new channel.
 */
HandoverFailureMessage;

typedef struct GsmCellDescrTag
{
    Int16 numCells;
    Int16 bandIndicator[MAX_NUM_GSM_DESCR_CELLS];
    Arfcn arfcn[MAX_NUM_GSM_DESCR_CELLS];
    Bsic  bsic[MAX_NUM_GSM_DESCR_CELLS];
}
GsmCellDescr;

typedef struct CellSelectionIndicIeTag
{
/** \assoc UNION \ref cellList \opt CONDITIONAL */
    CellSelectionIndicCellType cellType;
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    union
    {
        GsmCellDescr gsmDescr;
#if defined (UPGRADE_3G_FDD)
        FddCellDescr fddDescr;
#endif
#if defined (UPGRADE_3G_TDD128)
        Tdd128CellDescr    tdd128Descr;
#endif /* UPGRADE_3G_TDD128 */
    }
    cellList;
#else  /* UPGRADE_3G && UPGRADE_GGE */
    GsmCellDescr               gsmDescr;
#endif  /* UPGRADE_3G && UPGRADE_GGE */
}
CellSelectionIndicIe;

typedef struct ChannelReleaseMessageTag
{
    RrCauseElement       rrCause;
    BaRangeElement       baRange;
    Boolean              gprsResumptionAcknowledged;
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    UtranFreqListElement utranFreqList;
#endif  /* UPGRADE_3G && UPGRADE_GGE */
    CellSelectionIndicIe cellSelectionIndic;
}
ChannelReleaseMessage;

/** Partial release message content.
 * Used to deactivate part of the dedicated channels in use.
 */
typedef struct PartialReleaseMessageTag
{
    /** Channel description; provides a description of an allocable channel */
    ChannelDescrElement channelDescr;
}
PartialReleaseMessage;

/** Physical information message content.*/
typedef struct PhysicalInformationMessageTag
{
    /** provides the timing advance value */
    TimingAdvanceElement timingAdvance;
}
PhysicalInformationMessage;

/** Paging request message content */
typedef struct PagingRequestMessageTag
{
    /** The paging mode */
    PageModeElement     pageMode;
    /** Indicates the number of entries in the mobileId[] */
    Int16               numMobileIds;
    /** The Mobile Identity 1 and 2 IEs shall not refer to IMEI */
    MobileIdElement     mobileId[4];
    /** The first CHANNEL field of Channel Needed IE is associated with
     * Mobile Identity 1. The second CHANNEL field of Channel Needed IE
     * is associated with Mobile Identity 2.
     */
    ChannelNeeded       channelNeeded[4];
    /** Packet page indicator */
    PacketPageIndicator packetPageIndic[3];
}
PagingRequestMessage;

/** This message is sent on the main DCCH by the mobile station to
 * the network in connection with establishment of the main signalling
 * link as a response to the paging request message.
 */
typedef struct PagingResponseMessageTag
{
    /** Ciphering key sequence number */
    CipheringKeySeqNumElement cipheringKeySeqNum;
    /** Mobile station classmark;
     * provides the network with information concerning aspects of the mobile station equipment
     */
    MsClassmarkElement        msClassmark;
    /** Mobile identity */
    MobileIdElement           mobileId;
}
PagingResponseMessage;

/** SYSTEM INFORMATION TYPE 1 message content.
 * Gives information of control of the RACH and of the cell allocation.
 */
typedef struct SystemInformationType1MessageTag
{
    /** Cell Channel Description */
    CellChannelDescrElement cellChannelDescr;
    /** Provides parameters used to control the RACH utilization */
    RachControlParamElement rachControlParam;
    /** SI 1 Rest Octets; contains the position about the NCH and band indicator bits */
    Si1RestOctetsElement    si1RestOctets;
}
SystemInformationType1Message;

/** System information type 2 message content.
 * Gives information of control of the RACH and of the BCCH allocation in the neighbour cells.
 */
typedef struct SystemInformationType2MessageTag
{
    /** Neighbour Cell Description */
    NeighbourCellsDescrElement neighbourCellsDescr;
    /** NCC permitted */
    NccPermittedElement        nccPermitted;
    /** Provides parameters used to control the RACH utilization */
    RachControlParamElement    rachControlParam;
    /** Additional GSM network parameters;
     * Added to ensure that AL always has SI2 BA list
     */
    Si2BaList                  si2BaList;
}
SystemInformationType2Message;

/** System information type 2bis message content.
 * Gives information on control of the RACH and of the extension
 * of the BCCH allocation in the neighbour cells.
 */
typedef struct SystemInformationType2bisMessageTag
{
    /** provide the absolute radio frequency channel numbers
     * of the BCCH carriers to be monitored by the mobile stations in the cell
     */
    NeighbourCellsDescrElement neighbourCellsDescr;
    /** Provides parameters used to control the RACH utilization */
    RachControlParamElement    rachControlParam;
}
SystemInformationType2bisMessage;

/** System information type 3message content.
 * Gives information of control on the RACH, the location area identification,
 * the cell identity and various other information about the cell.
 */
typedef struct SystemInformationType3MessageTag
{
    /** Cell Identity */
    CellIdElement              cellId;
    /** Location Area Identification */
    LocationAreaIdElement      locationAreaId;
    /** Control Channel description */
    ControlChannelDescrElement controlChannelDescr;
    /** Cell Options (BCCH); provides a variety of information about a cell */
    CellOptionsElement         cellOptions;
    /** Cell Selection Parameters */
    CellSelectionParamElement  cellSelectionParam;
    /** Provides parameters used to control the RACH utilization */
    RachControlParamElement    rachControlParam;
    /** SI 3 Rest Octets */
    Si3RestOctetsElement       si3RestOctets;
}
SystemInformationType3Message;

/** System information type 4 message content.
 * Gives information on control of the RACH, the location area identification,
 * the cell identity and various other information about the cell.
 */
typedef struct SystemInformationType4MessageTag
{
    /** Location Area Identification */
    LocationAreaIdElement     locationAreaId;
    /** Cell Selection Parameters */
    CellSelectionParamElement cellSelectionParam;
    /** Provides parameters used to control the RACH utilization */
    RachControlParamElement   rachControlParam;
    /** Indicates if cbchChannelDescr contains relevant information
\assoc PRESENT \ref cbchChannelDescr */
    Boolean                   cbchChannelDescrPresent;
    /** This information element is present if SMSCB is active in the
     * cell and indicates (together with the CBCH Mobile Allocation IE)
     * where to find the CBCH.
     */
    ChannelDescrElement       cbchChannelDescr;
    /** Indicates if cbchMobileAllocation contains relevant information
\assoc PRESENT \ref cbchMobileAllocation */
    Boolean                   cbchMobileAllocationPresent;
    /** If the CBCH Channel Description Information Element indicates
     * frequency hopping, the CBCH Mobile Allocation IE shall be present.
     * If the CBCH Channel Description does not indicate frequency hopping,
     * the CBCH Mobile Allocation IE shall be considered as an unnecessary
     * IE in the message.
     */
    MobileAllocationElement   cbchMobileAllocation;
    /** The sum of the length of this IE and the L2 pseudo length of the message equals 22 */
    Si4RestOctetsElement      si4RestOctets;
}
SystemInformationType4Message;

/** System information type 5 message content.
 * Gives information on the BCCH allocation in the neighbour cells.
 */
typedef struct SystemInformationType5MessageTag
{
    /** The neighbour cells description */
    NeighbourCellsDescrElement neighbourCellsDescr;
}
SystemInformationType5Message,
/** System information type 5bis message content.
 * giving information on the extension of the BCCH allocation in the neighbour cells.
 */
SystemInformationType5bisMessage;

typedef struct SystemInformationType2TerMessageTag
{
    NeighbourCellsDescrElement neighbourCellsDescr;
    Int8 multiBandNcellReports;
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    Boolean                    si2TerRestOctetsPresent;
    Int8 si2TerIndex;
    Int8 si2TerCount;
    Int8 mpChangeMark;
    Int8 changeMark3g;
    Int8 restOctets[SI2_TER_REST_OCTET_SIZE];
#endif  /* UPGRADE_3G && UPGRADE_GGE */
}
SystemInformationType2TerMessage;

/** System information type 5ter message content.
 * Gives information on the extension of the BCCH allocation in the neighbour cells.
 */
typedef struct SystemInformationType5TerMessageTag
{
    /** The neighbour cells description */
    NeighbourCellsDescrElement neighbourCellsDescr;
    /** Multiband neighbour cell reports */
    Int8 multiBandNcellReports;
}
SystemInformationType5TerMessage;

/** SYSTEM INFORMATION TYPE 13 message content.
 * \note A mobile station not supporting GPRS shall treat this message
 * as an unknown message type.
 */
typedef struct SystemInformationType13Tag
{
    /** Indicates the status of the information on BCCH.
     * The value of this field may be changed when information on BCCH is changed.
     */
    Int8                 bcchChangeMark;
    /** This field is the binary representation of which information was changed
     * at the last indication in bcchChangeMark. Range 0 to 15:<UL>
     * <LI> 0     Update of unspecified SI message or SI messages;
     * <LI>   1   Update of SI1 message;
   * <LI> 2   Update of SI2, SI2 bis or SI2 ter message;
   * <LI> 3   Update of SI3, SI4, SI7 or SI8 message;
   * <LI> 4   Update of SI9 message;
     *</UL>
     * All other values shall be interpreted as 'update of unknown SI message type'.
     */
    Int8                 siChangeField;
    /** Indicates if GPRS Mobile Allocation is present */
    Boolean              gprsMaPresent;
    /** This field is the binary representation of the SI change mark identifying
     * the GPRS Mobile Allocation provided in SI13 and PSI13 messages;
     * range: 0 to 3
     */
    Int8                 si13ChangeMark;
    /** This information element shall refer to the cell allocation defined for the cell in SI1 or PSI2 */
    GprsMobileAllocation gprsMa;
    /** Indicates if Pbcch is present */
    Boolean              pbcchPresent;

    /* If PBCCH is not present */
    /** Routing Area Code is a fixed length of 1 octet and identifies a routing area within a
     * location area; the RAC is part of the RAI (Routing Area Identity)
     */
    Rac                  rac;
    /** whether the MS supports the split pg cycle option on CCCH */
    Boolean              splitPgCycleSupported;
    /** Indicates packet access is allowed for which priority level */
    Int8                 priorityAccessThreshold;
    /** Network control order */
    NetworkControl       nc;
    /** Used to control a set of cell options related to GPRS */
    GprsCellOptions      gprsCellOptions;
    /** GPRS Power Control Parameters */
    GprsPowerCtrlParam   pwrCtrl;
    /* Note that SI15_IND is not included as it should have been removed */

    /* If PBCCH is present */
    /** PSI1 repeat period (0-15, see 04.18 for format).
     * The field is coded according to the following table:
     * <TABLE>
     *  <TR><TD>Bits Band</TD><TD> Definition           </TD></TR>
     *  <TR><TD> 0000 </TD><TD> PSI1 repeat period = 1 multiframe </TD></TR>
     *  <TR><TD> 0001 </TD><TD> PSI1 repeat period = 2 multiframe </TD></TR>
     *  <TR><TD> 0002 </TD><TD> PSI1 repeat period = 3 multiframe </TD></TR>
     *  <TR><TD> 0003 </TD><TD> PSI1 repeat period = 4 multiframe </TD></TR>
     *  <TR><TD> 0004 </TD><TD> PSI1 repeat period = 5 multiframe </TD></TR>
     *  <TR><TD> 0005 </TD><TD> PSI1 repeat period = 6 multiframe </TD></TR>
     *  <TR><TD> 0006 </TD><TD> PSI1 repeat period = 7 multiframe </TD></TR>
     *  <TR><TD> 0007 </TD><TD> PSI1 repeat period = 8 multiframe </TD></TR>
     *  <TR><TD> 0008 </TD><TD> PSI1 repeat period = 9 multiframe </TD></TR>
     *  <TR><TD> 0009 </TD><TD> PSI1 repeat period = 10 multiframe </TD></TR>
     *  <TR><TD> 0010 </TD><TD> PSI1 repeat period = 11 multiframe </TD></TR>
     *  <TR><TD> 0011 </TD><TD> PSI1 repeat period = 12 multiframe </TD></TR>
     *  <TR><TD> 0012 </TD><TD> PSI1 repeat period = 13 multiframe </TD></TR>
     *  <TR><TD> 0013 </TD><TD> PSI1 repeat period = 14 multiframe </TD></TR>
     *  <TR><TD> 0014 </TD><TD> PSI1 repeat period = 15 multiframe </TD></TR>
     *  <TR><TD> 0015 </TD><TD> PSI1 repeat period = 16 multiframe </TD></TR>
     * </Table>
     */
    Int8                 psi1RepeatPeriod;
    /** Provides the channel description for the PBCCH */
    PbcchDescription     pbcchDesc;
    /** TRUE if SGSN is R99+, FALSE if R98- */
    Boolean              sgsnR99OrLater;
    /** TRUE = SI status supported */
    Boolean              siStatusSupported;
}
SystemInformationType13;

/** System information type 9 message content.
 * Gives some, but not necessarily all information on the
 * scheduling of information on the BCCH.
 */
typedef struct SystemInformationType9Tag
{
    /** Provides parameters used to control the RACH utilization */
    RachControlParamElement rachControlParam;
#if defined(ENABLE_SI_SCHED_INFO)
    /** SI 9 Rest Octets;
     * contains information about scheduling of some or all of the information on the BCCH.
     */
    Si9RestOctetsElement    restOctets;
#endif  /* (ENABLE_SI_SCHED_INFO) */
}
SystemInformationType9;

typedef struct MeasurementParameterDescrTag
{
    MeasReportType reportType;
    Int8           servingBandReporting;
}
MeasurementParameterDescr;

typedef struct Rtd6EntryTag
{
    Int8 numRtd;
    Int8 rtd[NUM_NCELL_RTD];
}
Rtd6Entry;

typedef struct Rtd12EntryTag
{
    Int8  numRtd;
    Int16 rtd[NUM_NCELL_RTD];
}
Rtd12Entry;

typedef struct Rtd6Tag
{
    Int8      baIndexStartRtd;
    Int8      numEntries;
    Rtd6Entry rtdEntry[MAX_NUM_RTD6];
}
Rtd6;

typedef struct Rtd12Tag
{
    Int8       baIndexStartRtd;
    Int8       numEntries;
    Rtd12Entry rtdEntry[MAX_NUM_RTD12];
}
Rtd12;

typedef struct RtdDescriptionTag
{
    Rtd6  rtd6;
    Rtd12 rtd12;
}
RtdDescription;

typedef struct BsicDescriptionTag
{
    Int8  baIndexStartBsic;
    Int16 numBsic;
    Int32 freqScrolling;
    Int8  bsic[MAX_NUM_BSIC_DESCR];
}
BsicDescription;

typedef struct SystemInformationType2QuaTag
{
    Int8                        baInd;
    Int8                        baInd3g;
    Int8                        mpChangeMark;
    Int8                        si2QuaCount;
    Int8                        si2QuaIndex;
    Boolean                     measParamDescrPresent;
    MeasurementParameterDescr   measParamDescr;

    RtdDescription              rtdDescr;

    Boolean                     bsicDescrPresent;
    BsicDescription             bsicDescr;

    ReportPriorityParam         reportPriority;

    Boolean                     gprsMeasParamDescrPresent;
    MeasReportType              reportType;
    MeasReportRate              reportRate;
    Boolean                     invalidBsicReporting;
    MeasurementParameters       gprsMeasParam;

    Boolean                     ncMeasurementParamPresent;
    NcMeasurementParam          ncMeasurementParam;

    Boolean                     ccnSupportPresent;
    Int32                       ccn;

#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    Boolean                     ncell3gDescrPresent;
    Boolean                     measParam3gDescrPresent;
    Boolean                     gprs3gMeasParamPresent;

    Boolean                     release6ParamPresent;
    Boolean                     ccn3GActive;

#if defined (UPGRADE_3G_FDD)
    Boolean                     utranFddNcellParamPresent;
    UtranFddNcellParam          utranFddNcellParam;
    Addit3GMeasParam            addit3GMeasParam; /* Only used by FDD */
    Addit3GMeasParam2           addit3GMeasParam2; /* Only used by FDD */
#endif /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)
    Boolean                     utranTdd128NcellParamPresent;
    UtranTdd128NcellParam       utranTdd128NcellParam;
#endif /* UPGRADE_3G_TDD128 */

    MeasurementParameter3gDescr measParam3gDescr;
    Gprs3gMeasurementParam      gprs3gMeasParam;
#endif  /* UPGRADE_3G && UPGRADE_GGE */
}
SystemInformationType2Qua;

typedef struct SystemInformationType6MessageTag
{
    CellIdElement         cellId;
    LocationAreaIdElement locationAreaId;
    CellOptionsElement    cellOptions;
    NccPermittedElement   nccPermitted;
    Si6RestOctetsElement  si6RestOctets;
}
SystemInformationType6Message;

/** System information type 7 message content.
 * Gives information about cell reselection parameters to be used in that cell.
 */
typedef struct SystemInformationType7MessageTag
{
    /** Includes parameters which are used by the mobile station for
     * cell selection and reselection purposes
     */
    Si7RestOctetsElement si7RestOctets;
}
SystemInformationType7Message;

/** System information type 8 message content.
 * Gives information about cell reselection parameters to be used in that cell.
 */
typedef struct SystemInformationType8MessageTag
{
    /**  Includes parameters which are used by the mobile station for cell
     * selection and reselection purposes
     */
    Si8RestOctetsElement si8RestOctets;
}
SystemInformationType8Message;

/** This message is sent by the network to the mobile station to reallocate
 * or delete a TMSI.
 */
typedef struct TmsiReallocationCommandMessageTag
{
    /** Location area identification */
    LocationAreaIdElement locationAreaId;
    /** Mobile identity */
    MobileIdElement       mobileId;
}
TmsiReallocationCommandMessage;

/** CM service request element
 * This message is sent by the mobile station to the network to
 * request a service for the connection management sublayer entities,
 * e.g. circuit switched connection establishment, supplementary
 * services activation, short message transfer, location services
 */
typedef struct CmServiceRequestMessageTag
{
    /** CM service type */
    CmServiceTypeElement      cmServiceType;
    /** Ciphering key sequence number */
    CipheringKeySeqNumElement cipheringKeySeqNum;
    /** Mobile station classmark;
     * provides the network with information concerning aspects of the mobile station equipment
     */
    MsClassmarkElement        msClassmark;
    /** Mobile identity */
    MobileIdElement           mobileId;
}
CmServiceRequestMessage;

/** CM Re-establishment request element.
 * This message is sent by the mobile station to the network to request
 * re-establishment of a connection if the previous one has failed.
 */
typedef struct CMReestablishmentRequestMessageTag
{
    /** Ciphering key sequence number */
    CipheringKeySeqNumElement cipheringKeySeqNum;
    /** Mobile station classmark;
     * provides the network with information concerning aspects of the mobile station equipment
     */
    MsClassmarkElement        msClassmark;
    /** Mobile identity */
    MobileIdElement           mobileId;
    /** Indicates if location area identification is present
\assoc PRESENT \ref locationAreaId */
    Boolean                   locationAreaIdPresent;
    /** Location area identification*/
    LocationAreaIdElement     locationAreaId;
}
CmReestablishmentRequestMessage;

#if defined(UPGRADE_AMR_PS)
/** Channel mode modify message content.
 * This message is sent on the main DCCH by the network to the mobile
 * station to request the setting of the mode for the indicated channel(s).
 * \note The message can be used to change the channel mode of a Multislot
 * Configuration which only contains one channel set.
 */
typedef struct ChannelModeModifyMessageTag
{
    /** This is sufficient to identify the channel in the case of a TCH/H + TCH/H
     * configuration. If used for a multislot configuration, the IE shall describe
     * the present channel configuration with TN indicating the main channel.
     * The IE shall not indicate a new channel configuration when included in the
     * Channel Mode Modify message.
     */
    ChannelDescrElement    channelDescr;
    /** Channel Mode; gives information of the mode on coding/decoding and transcoding */
    ChannelModeElement     channelMode;
    /** Indicates if MultiRate configuration is present */
    Boolean                multirateConfigPresent;
    /** This information element appears if the Channel Mode IE indicates a
     * multi-rate speech codec.
     */
    MultirateConfigElement multirateConfig;
}
ChannelModeModifyMessage;

/** Channel mode modify acknowledge message content.
 * Indicates the successful or unsuccessful execution of a channel mode modify request.
 */
typedef struct ChannelModeModifyAckMessageTag
{
    /** Channel description; provides a description of an allocable channel */
    ChannelDescrElement channelDescr;
    /** Gives information of the mode on coding/decoding and transcoding.
     * The exact mode is determined by the contents of this IE and the channel type.
     */
    ChannelModeElement  channelMode;
}
ChannelModeModifyAckMessage;

#else  /* !(UPGRADE_AMR_PS) */
/* Non-AMR structs */
/** Channel mode modify message content.
 * This message is sent on the main DCCH by the network to the mobile
 * station to request the setting of the mode for the indicated channel(s).
 * \note The message can be used to change the channel mode of a Multislot
 * Configuration which only contains one channel set.
 */
typedef struct ChannelModeModifyMessageTag
{
    /** Channel description; provides a description of an allocable channel */
    ChannelDescrElement channelDescr;
    /** Channel Mode; gives information of the mode on coding/decoding and transcoding */
    ChannelModeElement  channelMode;
}
ChannelModeModifyMessage,
/** Channel mode modify acknowledge message content.
 * Indicates the successful or unsuccessful execution of a channel mode modify request.
 */
ChannelModeModifyAckMessage;
#endif  /* !(UPGRADE_AMR_PS) */

/** RR STATUS message content.
 * This message is used to report certain error conditions.
 */
typedef struct RrStatusMessageTag
{
    /** Indicates the cause of error */
    RrCauseElement rrCause;
    /*
    Note an optional control state element may be added here
    at a later date. (see section 9.1.29/GSM 04.08)
    */
}
RrStatusMessage;

/** FREQUENCY REDEFINITION message content.
 * Indicates that the frequencies and the hopping sequence of the
 * allocated channels shall be changed.
 */
typedef struct FreqRedefinitionMessageTag
{
    /** If it does not appear, the cell channel description is assumed to be unchanged */
    ChannelDescrElement     channelDescr;
    /** Mobile Allocation */
    MobileAllocationElement mobileAllocation;
    /** Starting Time */
    StartingTimeElement     startingTime;
    /** Indicates if cellChannelDescr contains relevant information
\assoc PRESENT \ref cellChannelDescr */
    Boolean                 cellChannelDescrPresent;
    /** Cell Channel Description */
    CellChannelDescrElement cellChannelDescr;
}
FreqRedefinitionMessage;

/** Measurement report message content.
 * This message is used to report measurement results about the dedicated channel
 * and about neighbour cells.
 */
typedef struct MeasurementReportMessageTag
{
    /** Provide the results of the measurements made by the mobile station on
     * the serving cell and the neighbour cells
     */
    MeasurementResultsElement measurementResults;
}
MeasurementReportMessage;

/** Classmark enquiry message content.
 * This message is used to request classmark information.
 */
typedef struct ClassmarkEnquiryMessageTag
{
    /** Indicates if mask is present */
    Boolean maskPresent;
    /** Classmark mask */
    Int8    mask;
}
ClassmarkEnquiryMessage;

/** Extended measurement order message content.
 * orders the mobile station to send one extended measurement report.
 */
typedef struct ExtendedMeasurementOrderMsgTag
{
    /** Cell channel description */
    CellChannelDescrElement cellChannelDescr;
}
ExtendedMeasurementOrderMsg;

/** Extended measurement report message content.
 * This message is used to report extended measurement results about the
 * signal strength on specified carriers.
 */
typedef struct ExtendedMeasurementReportMsgTag
{
    /** Indicates the value of the SEQ-CODE in the extended measurement frequency
     * list information element used for defining the list of frequencies reported on
     */
    Int8    scUsed;
    /** Indicates  whether or not the mobile station used DTX during the previous
    * measurement period.
    */
    Boolean dtxUsed;
    /** The received signal strength on carrier N */
    Int8    rxLev[MAX_BA_CHANNELS];
}
ExtendedMeasurementReportMsg;

/** Classmark change message content.
 * This message indicates a classmark change or as a response to a classmark enquiry.
 */
typedef struct ClassmarkChangeMessageTag
{
    /** This IE shall include for multiband MS the Classmark 2 corresponding to
     * the frequency band in use
     */
    MsClassmarkElement msClassmark;
}
ClassmarkChangeMessage;

/** This message is sent by the mobile station to the network
 * to set a deactivation indication in the network.
 */
typedef struct ImsiDetachIndicationMessageTag
{
    /** Mobile station classmark;
     * provides the network with information concerning aspects of the mobile station equipment
     */
    MsClassmarkElement msClassmark;
    /** Mobile identity */
    MobileIdElement    mobileId;
}
ImsiDetachIndicationMessage;

/** This message is sent by the network to the mobile station to indicate
 * that updating or IMSI attach in the network has been completed.
 */
typedef struct LocationUpdatingAcceptMessageTag
{
    /** Location area identification */
    LocationAreaIdElement     locationAreaId;
    /** Indicates if mobile identity is present
\assoc PRESENT \ref mobileId */
    Boolean                   mobileIdPresent;
    /** Mobile identity */
    MobileIdElement           mobileId;
    /** Indicates if follow on proceed is present */
    Boolean                   followOnProceedPresent;
    /** Holds a list of equivalent PLMNs */
    EquivalentPlmnListElement equivalentPlmnList;
#if defined(UPGRADE_REL5)
    /** Indicates if emergency number list is present */
    Boolean                   emergencyNumberListPresent;
    /** Emergency number list */
    EmergencyNumberList       emergencyNumberList;
#endif  /* (UPGRADE_REL5) */
}
LocationUpdatingAcceptMessage;

/** This message is sent by the network to the mobile station to
 * indicate that the requested service cannot be provided.
 */
typedef struct CmServiceRejectMessageTag
{
    /** Indicates the reason for reject */
    GsmCause rejectCause;
}
CmServiceRejectMessage,
/** This message is sent by the network to the mobile
 * station to initiate the abortion of all MM connections
 * and to indicate the reason for the abortion.
 */
AbortMessage,
/** This message is sent by the network to the mobile
 * station to initiate the abort of all MM connections
 * and to indicate the reason for abort.
 */
MmStatusMessage;

/** This message is sent by the network to the mobile
 * station to provide the mobile station with subscriber
 * specific information.
 */
typedef struct MmInformationMessageTag
{
    /** Whether or not full network name is present
\assoc PRESENT \ref fullNetworkName */
    Boolean                   fullNetworkNamePresent;
    /** Indicates full network name information */
    NetworkName               fullNetworkName;
    /** Indicates the abbreviated name of the network
\assoc PRESENT \ref shortNetworkName */
    Boolean                   shortNetworkNamePresent;
    /** Indicates short network name information */
    NetworkName               shortNetworkName;
    /** Indicates if location time zone is present
\assoc PRESENT \ref locTimeZone */
    Boolean                   locTimeZonePresent;
    /** The purpose of this parameter is to encode the
     * offset between universal time and local time in steps
     * of 15 minutes.
     */
    LocTimeZone               locTimeZone;
    /** whether the universal time and local time zone is present
\assoc PRESENT \ref uniTimeAndLocTimeZone */
    Boolean                   uniTimeAndLocTimeZonePresent;
    /** The purpose of the timezone is to encode the offset between universal
     * time and local time in steps of 15 minutes and the purpose of the time
     * part of this information element is to encode the universal time at which
     * this information element may have been sent by the network
     */
    UniTimeAndLocTimeZone     uniTimeAndLocTimeZone;
    /** Indicates if daylight saving time is present
\assoc PRESENT \ref networkDaylightSavingTime */
    Boolean                   networkDaylightSavingTimePresent;
    /** Indicates the value that has been used to adjust the local time zone */
    NetworkDaylightSavingTime networkDaylightSavingTime;
    /** Whether LSA is present
\assoc PRESENT \ref lsaIdentity */
    Boolean                   lsaIdentityPresent;
    /** uniquely identifies a LSA.*/
    Int32                     lsaIdentity;
}
MmInformationMessage;

/** Location updating reject element.
 * This message is sent by the network to the mobile station
 * to indicate that updating or IMSI attach has failed.
 */
typedef struct LocationUpdatingRejectMessageTag
{
    /** Indicates the cause for reject */
    GsmCause rejectCause;
}
LocationUpdatingRejectMessage;

/** Location updating request element.
 * This message is sent by the mobile station to the network
 * either to request update of its location file (normal
 * updating or periodic updating) or to request IMSI attach
 */
typedef struct LocationUpdatingRequestMessageTag
{
    /** Location updating type */
    LocationUpdatingTypeElement locationUpdatingType;
    /** Ciphering key sequence number */
    CipheringKeySeqNumElement   cipheringKeySeqNum;
    /** Location area identification */
    LocationAreaIdElement       locationAreaId;
    /** Mobile station classmark;
     * provides the network with information concerning aspects of the mobile station equipment
     */
    MsClassmarkElement          msClassmark;
    /** Mobile identity */
    MobileIdElement             mobileId;
#if defined(UPGRADE_3G)
    /** Whether it is UMTS network */
    Boolean                     umtsNetwork;
#endif  /* (UPGRADE_3G) */
}
LocationUpdatingRequestMessage;

/** Authentication request information element.
 * This message is sent by the network to the mobile station to
 * initiate authentication of the mobile station identity.
 */
typedef struct AuthenticationRequestMessageTag
{
    /** Ciphering key sequence number */
    CipheringKeySeqNumElement cipheringKeySeqNum;
    /** Authentication parameter RAND is a non-predictable number to be used
     * for authentication.
     */
    AuthParamRandElement      authParamRand;
    /** Indicates if authentication parameter AUTN is present */
    Boolean                   authParamAutnPresent;
    /** UMTS authentication challenge  */
    AuthParamAutnElement      authParamAutn;
}
AuthenticationRequestMessage;

/** This message is sent by the mobile station to the network
 * to deliver a calculated response to the network.
 */
typedef struct AuthenticationResponseMessageTag
{
    /** Authentication Response parameter */
    AuthParamSresElement authParamSres;
}
AuthenticationResponseMessage;

/** Autentication failure message element */
typedef struct AuthenticationFailureMessageTag
{
    /** Indicates the cause of failure */
    GsmCause                rejectCause;
    /** Whether or not authentication failure parameter is present */
    Boolean                 authFailureParameterPresent;
    /** Authentication failure parameter */
    AuthParamFailureElement authFailureParameter;
}
AuthenticationFailureMessage;

/** This message is sent by the network to the mobile station to
 * request a mobile station to submit the specified identity to the network.
 */
typedef struct IdentityRequestMessageTag
{
    /** Identity type */
    IdTypeElement idType;
}
IdentityRequestMessage;

/** This message is sent by the mobile station to the network in
 * response to an IDENTITY REQUEST message providing the requested identity.
 */
typedef struct IdentityResponseMessageTag
{
    /** Mobile identity */
    MobileIdElement mobileId;
}
IdentityResponseMessage;

/** This message is sent from the network to the mobile station
 * (or from the mobile station to the network )to
 * indicates that the network (the MS)has released the transaction identifier
 * and that the mobile station (Network)shall release the transaction identifier.
 */
typedef struct ReleaseCompleteMessageTag
{
    /** Indicates if cause is present
\assoc PRESENT \ref cause */
    Boolean                   causePresent;
    /** Cause of release (shall be included if the message is used to
     * initiate call clearing).
     */
    CauseElement              cause;
    /** Indicates if facility is present
\assoc PRESENT \ref facility */
    Boolean                   facilityPresent;
    /** Included for functional operation of supplementary services.*/
    FacilityElement           facility;
    /** Indicates if user-user information is present
\assoc PRESENT \ref userUser */
    Boolean                   userUserPresent;
    /** Included in the network to mobile station direction, when the
     * RELEASE COMPLETE message is used to initiate call clearing,
     * in order to transport user-user information from the remote user.
     */
    UserUserElement           userUser;
    /** Whether or not SS version is included
\assoc PRESENT \ref ssVersionIndicator */
    Boolean                   ssVersionIndicatorPresent;
    /** Shall not be included if the facility information element
     * is not present in this message.
     */
    SsVersionIndicatorElement ssVersionIndicator;
}
ReleaseCompleteMessage;

/** The CP-DATA message is sent between an MSC and an MS, in both directions.
 * The message contains the user data to be relayed between the CM-users, and
 * associated parameters.
 */
typedef struct CpDataMessageTag
{
    /** Used to carry the RPDU.*/
    CpUserDataElement cpUserData;
}
CpDataMessage;

/** CP-ERROR element.
 * The CP-ERROR message is sent between an MSC and an MS,
 * in both directions, and used to convey error information.
 */
typedef struct CpErrorMessageTag
{
    /** Indicates the cause of error. */
    CpCauseElement cpCause;
}
CpErrorMessage;

/** This message is sent by the network to the calling mobile station
 * (or by the called mobile station to the network )
 * to indicate that the called user alerting has been initiated.
 */
typedef struct AlertingMessageTag
{
    /** Whether or not facility is present
\assoc PRESENT \ref facility */
    Boolean                   facilityPresent;
    /** Transports supplementary service related information */
    FacilityElement           facility;
    /** Indicates if progress indicator is present
\assoc PRESENT \ref progressIndicator */
    Boolean                   progressIndicatorPresent;
    /** Progress indicator */
    ProgressIndicatorElement  progressIndicator;
    /** Indicates if user to user is present
\assoc PRESENT \ref userUser */
    Boolean                   userUserPresent;
    /** User to user */
    UserUserElement           userUser;
    /** Indicates if the SS version indicator is present
\assoc PRESENT \ref ssVersionIndicator */
    Boolean                   ssVersionIndicatorPresent;
    /** The SS version indicator */
    SsVersionIndicatorElement ssVersionIndicator;
}
AlertingMessage;

/** Call confirmed message element.
 * This message is sent by the called mobile station to confirm an
 * incoming call request.
 */
typedef struct CallConfirmedMessageTag
{
    /** Repeat Indicator shall be included if bearer capability 1 information element
     * and bearer capability 2 IE are both included in the message
     */
    RepeatIndicatorElement  repeatIndicator;
    /** Indicates if bearer capability1 element is present
\assoc PRESENT \ref bearerCapability1 */
    Boolean                 bearerCapability1Present;
    /** Bearer capability1 */
    BearerCapabilityElement bearerCapability1;
    /** Indicates if bearer capability2 element is present
\assoc PRESENT \ref bearerCapability2 */
    Boolean                 bearerCapability2Present;
    /** Bearer capability2 */
    BearerCapabilityElement bearerCapability2;
    /** Indicates if the Cause is present
\assoc PRESENT \ref cause */
    Boolean                 causePresent;
    /** Indicates the reason for sending this message;
     * included if the mobile station is compatible but the
     * user is busy.
     */
    CauseElement            cause;
    /** Whether or not call control capabilities are present */
    Boolean                 ccCapabilitiesPresent;
    /** Indicates the mobile station  call control capabilities */
    CcCapabilitiesElement   ccCapabilities;
    /** Indicates if streamIdentifier contain valid information */
    Boolean                 streamIdentifierPresent;
    /** Stream Identifier shall be included by the mobile station
     * supporting multicall.
     */
    StreamIdentifier        streamIdentifier;
#if defined(UPGRADE_REL5)
    /** Whether or not supported codec list is present */
    Boolean                 supportedCodecListPresent;
    /** Supported codec list shall be included for speech calls,
     * if the mobile station supports UMTS radio access.
     */
    SupportedCodecList      supportedCodecList;
#endif  /* (UPGRADE_REL5) */
}
CallConfirmedMessage;

/** Call proceeding message element.
 * This message is sent by the network to the calling mobile station to
 * indicate that the requested call establishment information has been
 * received, and no more call establishment information will be accepted.
 */
typedef struct CallProceedingMessageTag
{
    /** Repeat indicator is included if and only if bearer capability 1
     * IE and bearer capability 2 IE are both contained in the message.
     */
    RepeatIndicatorElement         repeatIndicator;
    /** Indicates if bearer capability1 is present
\assoc PRESENT \ref bearerCapability1 */
    Boolean                        bearerCapability1Present;
    /** Bearer capability1 */
    BearerCapabilityElement        bearerCapability1;
    /** Indicates if bearer capability2 is present
\assoc PRESENT \ref bearerCapability2 */
    Boolean                        bearerCapability2Present;
    /** Bearer capability2 */
    BearerCapabilityElement        bearerCapability2;
    /** Indicates if facility is present
\assoc PRESENT \ref facility */
    Boolean                        facilityPresent;
    /** Used for functional operation of supplementary services.*/
    FacilityElement                facility;
    /** Indicates if progress indicator is present
\assoc PRESENT \ref progressIndicator */
    Boolean                        progressIndicatorPresent;
    /** Included in order to pass information about the call in
     * progress for instance in the event of interworking; and/or
     * to make the MS attach the user connection for speech.
     */
    ProgressIndicatorElement       progressIndicator;
    /** Indicates if priority granted is present
\assoc PRESENT \ref priorityGranted */
    Boolean                        priorityGrantedPresent;
    /** Provided by the network in the case that eMLPP is supported. */
    PriorityLevel                  priorityGranted;
    /** Whether or not network call control capabilities are present
\assoc PRESENT \ref networkCallControlCapabilities */
    Boolean                        networkCallControlCapabilitiesPresent;
    /** be included by the network to indicate its call control capabilities
     * if the network supports multicall.and there are no other ongoing calls
     * to the MS.
     */
    NetworkCallControlCapabilities networkCallControlCapabilities;
}
CallProceedingMessage;

/** This message is sent by the network to indicate the establishment
 * or termination of flow control on the transmission of USER INFORMATION
 * messages.
 */
typedef struct CongestionControlMessageTag
{
    /** Congestion level */
    CongestionLevelElement congestionLevel;
    /** Indicates if cause is present
\assoc PRESENT \ref cause */
    Boolean                causePresent;
    /** Included if the user to user information has been discarded
     * as a result of the congestion situation.
     */
    CauseElement           cause;
}
CongestionControlMessage;

/** Indicates call acceptance by the called user.
 */
typedef struct ConnectMessageTag
{
    /** Indicates if facility is present
\assoc PRESENT \ref facility */
    Boolean                    facilityPresent;
    /** Used for functional operation of supplementary services. */
    FacilityElement            facility;
    /** Indicates if progress indicator is present
\assoc PRESENT \ref progressIndicator */
    Boolean                    progressIndicatorPresent;
    /** Included in order to pass information about the call in
     * progress for instance in the event of interworking; and/or
     * to make the MS attach the user connection for speech.
     */
    ProgressIndicatorElement   progressIndicator;
    /** Indicates if connected number is present
\assoc PRESENT \ref connectedNumber */
    Boolean                    connectedNumberPresent;
    /** Identifies the connected party of a call. */
    ConnectedNumberElement     connectedNumber;
    /** Indicates if connected subaddress is present
\assoc PRESENT \ref connectedSubaddress */
    Boolean                    connectedSubaddressPresent;
    /** Identify a subaddress associated with the connected party of a call.*/
    ConnectedSubaddressElement connectedSubaddress;
    /** Indicates if user-user information is present
\assoc PRESENT \ref userUser */
    Boolean                    userUserPresent;
    /** User-user information */
    UserUserElement            userUser;
    /** Whether or not SS version indicator is present
\assoc PRESENT \ref ssVersionIndicator */
    Boolean                    ssVersionIndicatorPresent;
    /** SS version indicator */
    SsVersionIndicatorElement  ssVersionIndicator;
    /** The streamIdentifier is only for MS-originated calls
\assoc PRESENT \ref streamIdentifier */
    Boolean                    streamIdentifierPresent;
    /** Stream identifier */
    StreamIdentifier           streamIdentifier;
}
ConnectMessage;

/** This message is sent by the network to indicate that the
 * end-to-end connection is cleared.
 */
typedef struct DisconnectMessageTag
{
    /** The cause of disconnection */
    CauseElement              cause;
    /** Whether or not facility is present
\assoc PRESENT \ref facility */
    Boolean                   facilityPresent;
    /** This message is sent by the network to indicate that the
     * end-to-end connection is cleared.
     */
    FacilityElement           facility;
    /** Indicates if progress indicator is present
\assoc PRESENT \ref progressIndicator */
    Boolean                   progressIndicatorPresent;
    /** Included by the network to make the MS attach the user
     * connection for speech and react in a specific way during call clearing
     */
    ProgressIndicatorElement  progressIndicator;
    /** Indicates if user-user is present
\assoc PRESENT \ref userUser */
    Boolean                   userUserPresent;
    /** Included by the network when the remote user initiates call clearing
     * and included a user-user information element in the DISCONNECT message.
     */
    UserUserElement           userUser;
    /** Indicates if SS version indicator is present
\assoc PRESENT \ref ssVersionIndicator */
    Boolean                   ssVersionIndicatorPresent;
    /** SS version indicator */
    SsVersionIndicatorElement ssVersionIndicator;
}
DisconnectMessage;

typedef struct EmergencySetupMessageTag
{
/** \assoc PRESENT \ref bearerCapability */
    Boolean                 bearerCapabilityPresent;
    BearerCapabilityElement bearerCapability;
/** \assoc PRESENT \ref streamIdentifier */
    Boolean                 streamIdentifierPresent;
    StreamIdentifier        streamIdentifier;
#if defined(UPGRADE_REL5)
/** \assoc PRESENT \ref supportedCodecList */
    Boolean                 supportedCodecListPresent;
    SupportedCodecList      supportedCodecList;
/** \assoc PRESENT \ref emergencyServiceCategory */
    Boolean                 emergencyServiceCategoryPresent;
    Int8                    emergencyServiceCategory;
#endif  /* (UPGRADE_REL5) */
}
EmergencySetupMessage;

/** Facility message element. This message is sent to request
 * or acknowledge a supplementary service. The supplementary service to
 * be invoked and its associated parameters are specified in the facility
 * information element.
 */
typedef struct FacilityMessageTag
{
    /** Facility information */
    FacilityElement           facility;
    /** Indicates if SS version is present
\assoc PRESENT \ref ssVersionIndicator */
    Boolean                   ssVersionIndicatorPresent;
    /** SS version */
    SsVersionIndicatorElement ssVersionIndicator;
}
FacilityMessage,
/** Register message element. */
SsRegisterMessage;

/** This message is sent by the network to indicate the denial of a request to hold a call.*/
typedef struct HoldRejectMessageTag
{
    /** Indicates the cause for holding a call */
    CauseElement cause;
}
HoldRejectMessage,
/** This message is sent by the network to indicate the inability to
 * perform the requested retrieve function.
 */
RetrieveRejectMessage;

/** This message is sent by the mobile station to the network or
 * by the network to the mobile station to request a change in
 * bearer capability for a call.
 */
typedef struct ModifyMessageTag
{
    /** Bearer capability */
    BearerCapabilityElement       bearerCapability;
    /** Indicates if low layer compatibility is present
\assoc PRESENT \ref lowLayerCompatibility */
    Boolean                       lowLayerCompatibilityPresent;
    /** Low layer compatibility shall be included if it was included
     * in the initial SETUP message.*/
    LowLayerCompatibilityElement  lowLayerCompatibility;
    /** Indicates if high layer compatibility is present
\assoc PRESENT \ref highLayerCompatibility */
    Boolean                       highLayerCompatibilityPresent;
    /** This shall be included if it was included in the initial SETUP message.*/
    HighLayerCompatibilityElement highLayerCompatibility;
    /** Reverse call setup direction (is included or omitted in the mobile to
     * network direction according to the rules defined in TS 24.008 )
     */
    Boolean                       reverseCallSetupDirectionPresent;
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
    /* CR-860 Correction of the network initiated in-call modification
    ** Rec 24.008 9.3.13 Modify
    ** The field immediateModificationIndicatorPresent does not exist anymore
    */
#else /* !(UPGRADE_3G_FDD) || !(UPGRADE_REL6) */
    /** Whether or not immediate modification indicator is present */
    Boolean                       immediateModificationIndicatorPresent;
#endif /* !(UPGRADE_3G_FDD) || !(UPGRADE_REL6) */
}
ModifyMessage;

/** This message is sent by the mobile station to the network or by
 * the network to the mobile station to indicate completion of a
 * request to change bearer capability for a call.
 */
typedef struct ModifyCompleteMessageTag
{
    /** Bearer capability */
    BearerCapabilityElement       bearerCapability;
    /** Indicates if low layer compatibility is present
\assoc PRESENT \ref lowLayerCompatibility */
    Boolean                       lowLayerCompatibilityPresent;
    /** Shall be included if it was included in the initial SETUP message.*/
    LowLayerCompatibilityElement  lowLayerCompatibility;
    /** Indicates if high layer compatibility is present
\assoc PRESENT \ref highLayerCompatibility */
    Boolean                       highLayerCompatibilityPresent;
    /** Shall be included if it was included in the initial SETUP message. */
    HighLayerCompatibilityElement highLayerCompatibility;
    /** Indicates if reverse call setup direction is included */
    Boolean                       reverseCallSetupDirectionPresent;
}
ModifyCompleteMessage;

/** This message is sent by the mobile station to the network or by the
 * network to the mobile station to indicate failure of a request to
 * change the bearer capability for a call.
 */
typedef struct ModifyRejectMessageTag
{
    /** Bearer capability */
    BearerCapabilityElement       bearerCapability;
    /** Indicates the cause for reject */
    CauseElement                  cause;
    /** Indicates if low layer compatibility is present
\assoc PRESENT \ref lowLayerCompatibility */
    Boolean                       lowLayerCompatibilityPresent;
    /** Shall be included if it was included in the initial SETUP message.*/
    LowLayerCompatibilityElement  lowLayerCompatibility;
    /** Indicates if high layer compatibility is present
\assoc PRESENT \ref highLayerCompatibility */
    Boolean                       highLayerCompatibilityPresent;
    /** Shall be included if it was included in the initial SETUP message.*/
    HighLayerCompatibilityElement highLayerCompatibility;
}
ModifyRejectMessage;

/** User information element.
 * This message is sent by the mobile station to the network to transfer
 * information to the remote user. This message is also sent by the
 * network to the mobile station to deliver information transferred
 * from the remote user. This message is used if the user-to-user
 * transfer is part of an allowed information transfer.
 */
typedef struct UserInformationMessageTag
{
    /** User-user information.
     * Some networks may only support a maximum length of 35 octets.
     * R98 and earlier versions of this protocol specified a minimum
     * length of 3 octets for this information element (not counting the IEI).
     * To avoid interworking problems with mobile stations supporting
     * only R98 or earlier versions of the protocol, the network shall
     * deliver the User information message to these mobile stations
     * only if the length of the User-user IE is greater or equal to
     * 3 octets (not counting the IEI).
     */
    UserUserElement userUser;
    /** Indicates whether or not another USER INFORMATION message
     * pertaining to the same message block will follow.
     */
    Boolean         moreDataPresent;
    /* the more data element contains no data */
}
UserInformationMessage;
/** This message is sent either from the mobile station or from
 * the network to indicate information pertaining to a call, such as user suspended.
 */
typedef struct NotifyMessageTag
{
    /** Notification indicator */
    NotificationIndicatorElement notificationIndication;
}
NotifyMessage;

/** Status message element.
 * This message is sent by the mobile station or the network at any
 * time during a call to report certain error conditions.
 * It shall also be sent in response to a STATUS ENQUIRY message.
 */
typedef struct StatusMessageTag
{
    /** The cause of sending this message */
    CauseElement           cause;
    /** Describe the current status of a call */
    CallStateElement       callState;
    /** Indicates if auxiliaryStates contains valid information
\assoc PRESENT \ref auxiliaryStates */
    Boolean                auxiliaryStatesPresent;
    /** Included if and only if the call state is <B>active</B> or
     * <B>mobile originating modify</B> and any auxiliary state is
     * different from <B>idle</B>.
     */
    AuxiliaryStatesElement auxiliaryStates;
}
StatusMessage;

/** Prgogress message element.
 * This message is sent from the network to the mobile station to
 * indicate the progress of a call in the event of interworking or
 * in connection with the provision of in-band information/patterns.
 */
typedef struct ProgressMessageTag
{
    /** Progress indicator */
    ProgressIndicatorElement progressIndicator;
    /** Indicates if user-user is present
\assoc PRESENT \ref userUser */
    Boolean                  userUserPresent;
    /** This information element is included when the PROGRESS
     * message is sent by the network when the call has been
     * cleared by the remote user before it reached the active
     * state to indicate that the remote user wants to pass user
     * information at call clearing time.
     */
    UserUserElement          userUser;
}
ProgressMessage;

/** Release message element.
 * This message is sent, from the network to the mobile station (or
 * from the mobile station to the network) to indicate that the
 * network (the mobile station )intends to release the transaction identifier, and that
 * the receiving equipment shall release the transaction identifier after
 * sending RELEASE COMPLETE.
 */
typedef struct ReleaseMessageTag
{
    /** Indicates if cause1 holds valid information
\assoc PRESENT \ref cause1 */
    Boolean                   cause1Present;
    /** Shall be included if this message is used to initiate call clearing.*/
    CauseElement              cause1;
    /** Indicates if cause2 holds valid information
\assoc PRESENT \ref cause2 */
    Boolean                   cause2Present;
    /** Indicates abnormal cases for release */
    CauseElement              cause2;
    /** Indicates if facility is present
\assoc PRESENT \ref facility */
    Boolean                   facilityPresent;
    /** Included for functional operation of supplementary services. */
    FacilityElement           facility;
    /** Indicates if user-user information is present
\assoc PRESENT \ref userUser */
    Boolean                   userUserPresent;
    /** Included in the network to mobile station direction, when the
     * RELEASE message is used to initiate call clearing, in order to
     * transport user-user information from the remote user.
     */
    UserUserElement           userUser;
    /** Indicates if SS version is present
\assoc PRESENT \ref ssVersionIndicator */
    Boolean                   ssVersionIndicatorPresent;
    /** Shall not be included if the facility information element
     * is not present in this message.
     */
    SsVersionIndicatorElement ssVersionIndicator;
}
ReleaseMessage;

/** This message is sent by the network to the mobile station to
 * initiate a mobile terminated call establishment.
 */
typedef struct SetupMessageTag
{
    /** Included if and only if bearer capability 1 information
     * element and bearer capability 2 IE are both present in the message.
     */
    RepeatIndicatorElement         repeatIndicator;
    /** Indicates if bearer capability1 is present
\assoc PRESENT \ref bearerCapability1 */
    Boolean                        bearerCapability1Present;
    /** Bearer capability1 (element may be omitted in the case where the mobile subscriber
     * is allocated only one directory number for all services) */
    BearerCapabilityElement        bearerCapability1;
    /** Indicates if bearer capability2 is present
\assoc PRESENT \ref bearerCapability2 */
    Boolean                        bearerCapability2Present;
    /** Bearer capability2 */
    BearerCapabilityElement        bearerCapability2;
    /** Indicates if facility is present
\assoc PRESENT \ref facility */
    Boolean                        facilityPresent;
    /** Included for functional operation of supplementary services. */
    FacilityElement                facility;
    /** Indicates if progress indicator is present
\assoc PRESENT \ref progressIndicator */
    Boolean                        progressIndicatorPresent;
    /** Included in order to pass information about the call in
     * progress for instance in the event of interworking; and/or
     * to make the MS attach the user connection for speech.
     */
    ProgressIndicatorElement       progressIndicator;
    /** Whether or not signal is present
\assoc PRESENT \ref signal */
    Boolean                        signalPresent;
    /** Signal */
    SignalElement                  signal;
    /** Indicates if callingPartyBcdNum holds valid information
\assoc PRESENT \ref callingPartyBcdNum */
    Boolean                        callingPartyBcdNumPresent;
    /** Calling party BCD number information */
    CallingPartyBcdNumElement      callingPartyBcdNum;
    /** Indicates if callingPartySubaddress holds valid information
\assoc PRESENT \ref callingPartySubaddress */
    Boolean                        callingPartySubaddressPresent;
    /** Calling party subaddress information */
    CallingPartySubaddressElement  callingPartySubaddress;
    /** Indicates if calledPartyBcdNum holds valid information
\assoc PRESENT \ref calledPartyBcdNum */
    Boolean                        calledPartyBcdNumPresent;
    /** For all bands except for PCS1900, the maximum length of
     * this IE sent by the network shall be 13 octets.
     */
    CalledPartyBcdNumElement       calledPartyBcdNum;
    /** Indicates if called party sub address is present
\assoc PRESENT \ref calledPartySubaddress */
    Boolean                        calledPartySubaddressPresent;
    /** Included in the Network-to-mobile station direction if the calling
     * user includes a called party subaddress information element in the
     * SETUP message.
     */
    CalledPartySubaddressElement   calledPartySubaddress;
    /** Whether or not low layer compatibility1 is present
\assoc PRESENT \ref lowLayerCompatibility1 */
    Boolean                        lowLayerCompatibility1Present;
    /** Included in the network-to-mobile station direction if the calling
     * user specified a low layer compatibility.
     */
    LowLayerCompatibilityElement   lowLayerCompatibility1;
    /** Whether or not low layer compatibility2 is present
\assoc PRESENT \ref lowLayerCompatibility2 */
    Boolean                        lowLayerCompatibility2Present;
    /** Included if and only if the LLC repeat indicator information
     * element is contained in the message.
     */
    LowLayerCompatibilityElement   lowLayerCompatibility2;
    /** Indicates if high layer compatibility 1 is present
\assoc PRESENT \ref highLayerCompatibility1 */
    Boolean                        highLayerCompatibility1Present;
    /** Included in the network-to-mobile station direction if the calling
     * user specified a high layer compatibility
     */
    HighLayerCompatibilityElement  highLayerCompatibility1;
    /** Indicates if high layer compatibility 2 is present
\assoc PRESENT \ref highLayerCompatibility2 */
    Boolean                        highLayerCompatibility2Present;
    /** Included if and only if the HLC repeat indicator information
     * element is contained in the message.
     */
    HighLayerCompatibilityElement  highLayerCompatibility2;
    /** Indicates if user-user information is present
\assoc PRESENT \ref userUser */
    Boolean                        userUserPresent;
    /** Included in the network to called mobile station direction when
     * the calling remote user included a user-user information element
     * in the SETUP message.
     */
    SmallUserUserElement           userUser;
    /** Whether or not SS version indicator is present
\assoc PRESENT \ref ssVersionIndicator */
    Boolean                        ssVersionIndicatorPresent;
    /** SS version indicator */
    SsVersionIndicatorElement      ssVersionIndicator;
    /** Whether or not CLIR suppression is present */
    Boolean                        clirSuppressionPresent;
    /* CLIR Suppression Element contains no data */
    /** Indicates if CLIR invocation is present. */
    Boolean                        clirInvocationPresent;
    /* CLIR Invocation Element contains no data */
    /** Indicates if call control capabilities are present.
\assoc PRESENT \ref ccCapabilities */
    Boolean                        ccCapabilitiesPresent;
    /** Call control capabilities */
    CcCapabilitiesElement          ccCapabilities;
    /** Indicates if redirecting party BCD number is present
\assoc PRESENT \ref redirectingPartyBcdNum */
    Boolean                        redirectingPartyBcdNumPresent;
    /** Included in the network to called mobile station direction when
     * the call has been redirected.
     */
    RedirectingPartyBcdNum         redirectingPartyBcdNum;
    /** Indicates if redirectingPartySubAddress contains valid information
\assoc PRESENT \ref redirectingPartySubAddress */
    Boolean                        redirectingPartySubAddrPresent;
    /** Included in the network to called mobile station direction when
     * the calling remote user included a called party subaddress in
     * the SETUP message and the call has been redirected.
     */
    RedirectingPartySubAddress     redirectingPartySubAddress;
    /** Indicates if priority level is included
\assoc PRESENT \ref priorityLevel */
    Boolean                        priorityLevelPresent;
    /** Indicates the priority of the incoming call if eMLPP is used.*/
    PriorityLevel                  priorityLevel;
    /** Whether or not alerting pattern is present
\assoc PRESENT \ref alertingPattern */
    Boolean                        alertingPatternPresent;
    /** Included by the network to give some indication about
     * alerting (category or level).
     */
    AlertingPattern                alertingPattern;
    /* NetworkCallControlCapabilities and CauseOfNoCli are only in
       SetupInd (MS-terminated calls) */
    /** Indicates if networkCallControlCapabilities holds valid information
\assoc PRESENT \ref networkCallControlCapabilities */
    Boolean                        networkCallControlCapabilitiesPresent;
    /** Included by the network to indicate its call control capabilities
     * if the network supports multicall.and there are no other ongoing
     * calls to the MS.
     */
    NetworkCallControlCapabilities networkCallControlCapabilities;
    /** Whether or not cause of No CLI is present
\assoc PRESENT \ref causeOfNoCli */
    Boolean                        causeOfNoCliPresent;
    /** Cause of No CLI */
    CauseOfNoCli                   causeOfNoCli;
    /** StreamIdentifier is only for SetupReq (MS-originated calls)
\assoc PRESENT \ref streamIdentifier */
    Boolean                        streamIdentifierPresent;
    /** Stream identifier */
    StreamIdentifier               streamIdentifier;
#if defined(UPGRADE_REL5)
    /** Indicates if supported codec list is present */
    Boolean                        supportedCodecListPresent;
    /** Supported codec list */
    SupportedCodecList             supportedCodecList;
    /** Indicates if backup bearer capability is present */
    Boolean                        backupBearerCapabilityPresent;
    /** Included by the network only if there are no bearer capability
     * IEs contained in the message.
     */
    BackupBearerCapabilityElement  backupBearerCapability;
#endif  /* (UPGRADE_REL5) */
}
SetupMessage;

/**  Start DTMF message element.
 * This message is sent by the mobile station to the network and contains
 * the digit the network should reconvert back into a DTMF tone which is
 * then applied towards the remote user.
 */
typedef struct StartDtmfMessageTag
{
    /** Keypad facility */
    KeypadFacilityElement keypadFacility;
}
StartDtmfMessage,
StartDtmfAcknowledgeMessage;

/** This message is sent by the network to the mobile station,
 * if the network can not accept the START DTMF message.
 */
typedef struct StartDtmfRejectMessageTag
{
    /** Indicates the reason for rejecting the START DTMF message.*/
    CauseElement cause;
}
StartDtmfRejectMessage;

/******************************************************************
 *** Test Messages
 ******************************************************************/
/** This message is only sent in the direction SS to MS. */
typedef struct CloseTchLoopCmdMessageTag
{
    /** Identifies a subchannel */
    Int8            subchannel;
    /** Specifies test loop */
    TchLoopbackMode loopbackMode;
}
CloseTchLoopCmdMessage;

typedef struct OpenLoopCmdMessageTag
{
    Boolean                            acknowledge;
}
OpenLoopCmdMessage, OpenLoopAckMessage;

typedef struct TestedDeviceElementTag
{
    TestedDevice device;
}
TestedDeviceElement;

/* CloseTchLoopAckMessage - no data */
/* OpenLoopCmdMessage - no data */
/* ActEmmiCmdMessage - no data */
/* ActEmmiAckMessage - no data */
/* DeactEmmiMessage - no data */

typedef struct TestInterfaceMessageTag
{
    TestedDeviceElement testedDevice;
}
TestInterfaceMessage;

#if defined(UPGRADE_3G)
/** Defines loopback modes.
 * The UE test loop function can be operated in mode 1, mode 2 or mode 3.
 */
typedef enum TcUmtsTestLoopModeTag
{
    /** Loop mode 1.
     * For UE test loop mode 1 the loopback point is located above Layer 2.
     * Depending on the actual radio bearer setup loopback is performed of
     * RLC SDUs or PDCP SDUs.
     */
    TC_UMTS_TEST_LOOP_MODE_1 = 0,
    /** Loop mode 2.
     * For UE test loop mode 2 both data and CRC are looped back. UE test loop
     * mode 2 is intended for BLER testing of DL 12.2 kbps reference measurement
     * channel for which loopback of downlink CRC is required.
     */
    TC_UMTS_TEST_LOOP_MODE_2 = 1
#if defined (UPGRADE_3G)
#if defined (UPGRADE_3G_MBMS)
#if defined (UPGRADE_3G_TDD128)
    ,
    /** Loop mode 3.
     * UE test loop mode 3 provides counting of successfully received RLC SDUs
     * on a given MTCH.
     */
    TC_UMTS_TEST_LOOP_MODE_3 = 2
#endif /* UPGRADE_3G_TDD128 */
#endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G */
} TcUmtsTestLoopMode;

/**  Defines radio bearer identity to be looped back and
 * UL SDU sizes for each looped back RB
 */
typedef struct TcUmtsBearerInfoTag
{
    /** Radio bearer to be looped back */
    BearerIdentity bearerIdentity;                              /* 5-32 */
    /** Defines an array of uplink SDU sizes for a bearer which shall be looped back
     * (if the bearer is not using test loop mode 1 at the top of RLC, this
     * parameter is set to 0)
     */
    Int16          rlcSduBits;                                  /* 0-65535 */
} TcUmtsBearerInfo;

/** This signal is used to close the UE test loop mode 1, mode 2 or mode 3.
 * Close UE test loop procedure is used to start the UE Test Loop
 * function in the UE
 */
typedef struct TcUmtsCloseTestLoopModeCmdTag
{
    /** Specifies loopback mode; mode 1, mode 2 or mode 3 */
    TcUmtsTestLoopMode mode;
#if defined (UPGRADE_3G)
#if defined (UPGRADE_3G_MBMS)
#if defined (UPGRADE_3G_TDD128)
    /** Specifies a short identity of the MBMS transmission identity, which
     * concerns a session of a specific service.
     */
    UMBMS_ShortTransmissionID  shortTransmissionID;
#endif /* UPGRADE_3G_TDD128 */
#endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G */
    /** Defines how many radio bearers are in bearerInfo
\assoc ARRAY \ref bearerInfo */
    Int8               numberOfBearers;                         /* 1- 4 */
    /** An array of structures defining radio bearer identity to be looped back and
     * (if applicable) detemines UL SDU sizes for each defined RB
     */
    TcUmtsBearerInfo   bearerInfo [UPS_MAX_LOOPBACK_BEARERS];
} TcUmtsCloseTestLoopModeCmd;

#if defined (UPGRADE_3G_MBMS)
# if defined (UPGRADE_3G_TDD128)
/** This signal is used to answer the request of RLC
 *  SDU counter
 * function in the UE
 */
typedef struct TcUmtsTestLoopMode3SduCounterRequestAckTag
{
    Int32              rlcSduCounterValue;
} TcUmtsTestLoopMode3SduCounterRequestAck;
# endif /* UPGRADE_3G_TDD128 */
#endif /* UPGRADE_3G_MBMS */

#endif  /* (UPGRADE_3G) */

#if defined(ENABLE_GTTP_MESSAGING)
typedef struct GprsInformationMessageTag
{
    Int32 tlli;
    Int8  lengthLlcPdu;
    Int8  llcPdu[MAX_LLC_PDU_CONTAINER_SIZE];
}
GprsInformationMessage;
#endif  /* (ENABLE_GTTP_MESSAGING) */


#if defined (UPGRADE_LTE)
/** Defines if the loop back mode is mode A, mode B, Mode G, mode H or mode I.*/
typedef enum EutraLoopBackModeTag
{
    EUTRA_LOOPBACK_MODE_A = 0,
    EUTRA_LOOPBACK_MODE_B = 1,
    EUTRA_LOOPBACK_MODE_G = 6,
    EUTRA_LOOPBACK_MODE_H = 7,
    EUTRA_LOOPBACK_MODE_I = 8,
    NO_LOOPBACK
}
EutraLoopBackMode;

/** Defines the loopBack configuration for a DRB. */
typedef struct EutraLoopBackSetupTag
{
     /** Defines the identiy of the DRB to loop back. */
     EutraAsn_DRB_Identity              drbIdentity;

     /** Defines the size of the uplink PDCP SDU to loop back.
      *  Range is 0 to 12160 bits */
     Int16                              pdcpSduSize;
}
EutraLoopBackSetup;

/** Defines the loop back mode configuration for mode A. */
typedef struct EutraLoopBackModeATag
{
    /** Defines the number of DRB to loop back.
    \assoc ARRAY \ref loopBackSetup */
    Int8                                numberOfDrb;

     /** Defines the loop back mode configuration for each DRB. */
    EutraLoopBackSetup                  loopBackSetup [MAX_MODEA_LB_ENTITIES];
}
EutraLoopBackModeA;

/** Defines the loop back mode configuration for mode G */
typedef struct EutraLoopBackModeGTag
{
    /** Defines uplink loopback operation mode */
    Int8                                ulOpMode;
    /** Defines the number of repetitions */
    Int8                                numRepeat;
    /** Define uplink data delay, unit is seconds */
    Int8                                ulDataDelay;
}
EutraLoopBackModeG,
EutraLoopBackModeH;

/** Defines the loop back mode configuration for mode I */
typedef struct EutraLoopBackModeITag
{
    /** Defines uplink loopback operation mode */
    Int8                     ulOpMode;
}
EutraLoopBackModeI;

/** Defines the loop back mode configuration for mode A, B, G, H or I. */
typedef union EutraLoopBackConfigTag
{
     /** Defines the configuration for loop back mode A. */
     EutraLoopBackModeA                 loopBackModeA;

    /** Defines the configuration for loop back mode B.
     *  This is the time in seconds in the range 0 to 255 to delay the loop back. */
    Int8                                tDelayModeB;

    /** Defines the configuration for loop back mode G and H. */
    EutraLoopBackModeG                  loopBackModeG;

    /** Defines the configuration for loop back mode I. */
    EutraLoopBackModeI                  loopBackModeI;
}
EutraLoopBackConfig;

/** This signal is used to close the UE test loop mode in the UE.
 */
typedef struct TcEutraCloseUeTestLoopMessageTag
{
    /** Specifies loopback mode; mode A or B
            \assoc UNION \ref loopBackConfig */
    EutraLoopBackMode                   loopBackMode;

    /** Defines the loop back configuration for modes A and B. */
    EutraLoopBackConfig                 loopBackConfig;
}
TcEutraCloseUeTestLoopMessage;

/** This signal is used to activate the LTE test mode.
 */
typedef struct TcEutraActivateTestModeMessageTag
{
    /** Defines whether to activate mode A or B. */
    EutraLoopBackMode                   loopBackMode;
}
TcEutraActivateTestModeMessage;

#endif /* UPGRADE_LTE */

/*****************************************************************
 *** The L3 Message Structure
 *****************************************************************/

/** The status of L3 Message */
typedef struct MessageStatusTag
{
    /** Cause of error */
    Err   errorCode;
    /** The length of the diagnostic message
\assoc ARRAY \ref diagnostic */
    Int16 diagnosticSize;
    /** Holds the message for diagnostic. */
    Int8  diagnostic[MAX_DIAGNOSTIC_SIZE];
}
MessageStatus;

/** The header of L3 Message */
typedef struct MessageHeaderTag
{
    /** Used to indicate what CM sub-layer the signal is from
     * (in the MS) and for which layer in the network it is.
     */
    ProtocolDiscriminator pd;
    /** Holds a transaction value that is used to
    * identify this action within the CM sub-layer on both the
    * Mobile Station (MS) and Network side.
    */
    Int8                  tv;
    /** Indicates whether the MS or network initiated the transaction.
    * If it is set to FALSE the network initiated the transaction;
    * otherwise the MS did
    */
    Boolean               myTv;
    /** The L3 message type */
    L3MessageType         messageType;
    /** Send sequence number N(SD)*/
    Int8                  nsd;
    /** This flag indicates whether MSC/SGSN is Rel99 capable so we
     * can include Rel99 dependent fields when encoding messages
     */
    Boolean               r99Encode;
    /** PCS band flag */
    Boolean               pcsBand;
#if defined (UPGRADE_LTE)
    /** LTE only field: Security Header Type (24.301 Fig. 9.1.1 and 9.1.2)*/
    EpsSecurityHeaderType securityHeaderType;
   /** LTE only field: MessageAuthentication Code (24.301 Fig. 9.1.2) */
    Int32                 MessageAuthCode;
   /** LTE only field: Sequence number (24.301 Fig. 9.1.2) */
    Int8                  sequenceNumber;
    /** LTE only field: EPS bearer identity (24.301 Fig. 9.1.1) */
    Int8                  epsBearerId;
    /** LTE only field: Procedure transaction identity (24.301 Fig. 9.1.1) */
    Int8                  pti;
#endif /* UPGRADE_LTE */
    /** Header offset */
    Int16                 hdrOffset;
}
MessageHeader;

/** This union is reserved for Non Access Stratum messages only
 * (CC, SMS, MM SS,etc).
 */
typedef union MessageBodyTag
{
    /* SMS Messages */
    /** CP-DATA */
    CpDataMessage                            cpData;
    /** CP-ACK; used to acknowledge the reception of a CP-DATA message.*/
    EmptyMemberOfUnion                       cpAck;
    /** CP-ERROR */
    CpErrorMessage                           cpError;

    /* Test messages */
    /** Close TCH Loop command */
    CloseTchLoopCmdMessage                   closeTchLoopCmd;
    /** Close TCH Loop ACKNOWLEDGE */
    EmptyMemberOfUnion                       closeTchLoopAck;
    /** OPEN LOOP COMMAND */
    OpenLoopCmdMessage                       openLoopCmd;
    /** OPEN LOOP ACKNOWLEDGE */
    OpenLoopAckMessage                       openLoopAck;
    /** ACTIVATE EMMI COMMAND */
    EmptyMemberOfUnion                       actEmmiCmd;
    /** ACTIVATE EMMI ACKNOWLEDGE */
    EmptyMemberOfUnion                       actEmmiAck;
    /** DEACTIVATE EMMI */
    EmptyMemberOfUnion                       deactEmmi;
    /** TEST INTERFACE */
    TestInterfaceMessage                     testInterface;
#if defined(UPGRADE_3G)
    /** UMTS CLOSE TEST LOOP MODE COMMAND */
    TcUmtsCloseTestLoopModeCmd               tcUmtsCloseTestLoopModeCmd;
    /** UMTS CLOSE TEST LOOP MODE ACKNOWLEDGE */
    EmptyMemberOfUnion                       tcUmtsCloseTestLoopModeAck;
    /** UMTS OPEN TEST LOOP MODE COMMAND */
    EmptyMemberOfUnion                       tcUmtsOpenTestLoopModeCmd;
    /** UMTS OPEN TEST LOOP MODE ACKNOWLEDGE */
    EmptyMemberOfUnion                       tcUmtsOpenTestLoopModeAck;
    /** UMTS ACTIVATE RADIO BEARER TEST LOOP MODE COMMAND */
    EmptyMemberOfUnion                       tcUmtsActivateRbTestLoopModeCmd;
    /** UMTS ACTIVATE RADIO BEARER TEST LOOP MODE ACKNOWLEDGE  */
    EmptyMemberOfUnion                       tcUmtsActivateRbTestLoopModeAck;
    /** UMTS DEACTIVATE RADIO BEARER TEST LOOP MODE COMMAND */
    EmptyMemberOfUnion                       tcUmtsDeactivateRbTestLoopModeCmd;
    /** UMTS DEACTIVATE RADIO BEARER TEST LOOP MODE ACKNOWLEDGE  */
    EmptyMemberOfUnion                       tcUmtsDeactivateRbTestLoopModeAck;
#if defined (UPGRADE_3G_MBMS)
# if defined (UPGRADE_3G_TDD128)
    /** UMTS CLOSE TEST LOOP MODE COMMAND */
    EmptyMemberOfUnion                       tcUmtsTestLoopMode3SduCounterRequestCmd;
    /** UMTS CLOSE TEST LOOP MODE ACKNOWLEDGE */
    TcUmtsTestLoopMode3SduCounterRequestAck  tcUmtsTestLoopMode3SduCounterRequestAck;
# endif /* UPGRADE_3G_TDD128 */
#endif /* UPGRADE_3G_MBMS */
#endif  /* (UPGRADE_3G) */


#if defined (UPGRADE_LTE)
    /* EPS EMM Messages */
    /** EPS Attach Request    */
    EpsAttachRequest                         epsAttachRequest;
    /** EPS Attach Accept     */
    EpsAttachAccept                          epsAttachAccept;
    /** EPS Attach Complete   */
    EpsAttachComplete                        epsAttachComplete;
    /** EPS Attach Reject     */
    EpsAttachReject                          epsAttachReject;
    /** EPS MO/MT Detach Request (This structure should never be visible in Genie,
     * instead the internal MO/MT detach types below should be visible, but this is
     * still needed here to keep L3MessageType and MessageBody aligned)
     */
    Int8                                     epsDetachRequest;
    /** EPS Detach Accept     */
    EpsDetachAccept                          epsDetachAccept;
    /** EPS Tracking Area Update Request */
    EpsTrackingAreaUpdateRequest             epsTrackingAreaUpdateRequest;
    /** EPS Tracking Area Update Accept  */
    EpsTrackingAreaUpdateAccept              epsTrackingAreaUpdateAccept;
    /** EPS Tracking Area Update Complete*/
    EpsTrackingAreaUpdateComplete            epsTrackingAreaUpdateComplete;
    /** EPS Tracking Area Update Reject  */
    EpsTrackingAreaUpdateReject              epsTrackingAreaUpdateReject;

    Int8                                     epsExtendedServiceRequest;
    /** EPS Control Plane Service Request */
    EpsCpServiceRequest                      epsCpServiceRequest;
    /** EPS Service Reject    */
    EpsServiceReject                         epsServiceReject;
    /** EPS Service Accept */
    EpsServiceAccept                         epsServiceAccept;
    /** EpsGutiReallocationCommand */
    EpsGutiReallocationCommand               epsGutiReallocationCommand;
    /** EpsGutiReallocationComplete */
    EpsGutiReallocationComplete              epsGutiReallocationComplete;
    /** EPS Authentication Request */
    EpsAuthenticationRequest                 epsAuthenticationRequest;
    /** EPS Authentication Response */
    EpsAuthenticationResponse                epsAuthenticationResponse;
    /** EPS Authentication Reject */
    EpsAuthenticationReject                  epsAuthenticationReject;
    /** EPS Authentication Failure */
    EpsAuthenticationFailure                 epsAuthenticationFailure;
    /** EPS Identity Request  */
    EpsIdentityRequest                       epsIdentityRequest;
    /** EPS Identity Response */
    EpsIdentityResponse                      epsIdentityResponse;
    /** EPS Security Mode Command */
    EpsSecurityModeCommand                   epsSecurityModeCommand;
    /** EPS Security Mode Complete */
    EpsSecurityModeComplete                  epsSecurityModeComplete;
    /** EPS Security Mode Reject */
    EpsSecurityModeReject                    epsSecurityModeReject;
    /** EPS EMM Status  */
    EpsEmmStatus                             epsEmmStatus;
    /** EPS Emm Information */
    EpsEmmInformation                        epsEmmInformation;
    /** EPS Downlink NAS Transport */
    EpsDownlinkNasTransport                  epsDownlinkNasTransport;
    /** EPS Uplink NAS Transport */
    EpsUplinkNasTransport                    epsUplinkNasTransport;
    Int8                                     epsCsServiceNotification;
    /** not yet used */
    EpsGenericNasTransport                   epsDownlinkGenericNasTransport;
    /** not yet used */
    EpsGenericNasTransport                   epsUplinkGenericNasTransport;
    /** EPS MO Detach Request */
    EpsMoDetachRequest                       epsMoDetachRequest;
    /** EPS MT Detach Request */
    EpsMtDetachRequest                       epsMtDetachRequest;
    /** EPS plain NAS message (may be ciphered) inside a security protected NAS message */
    EutraEncNasMessage                       epsPlainNasMesssage;

    /* EPS ESM Messages */
    /** Activate Default EPS Bearer Context Request */
    EpsActDefaultEpsBearerContextRequestMessage         epsActDefaultEpsBearerContextRequest;
    /** Activate Default EPS Bearer Context Accept */
    EpsActDefaultEpsBearerContextAcceptMessage          epsActDefaultEpsBearerContextAccept;
    /** Activate Default EPS Bearer Context Reject */
    EpsActDefaultEpsBearerContextRejectMessage          epsActDefaultEpsBearerContextReject;
    /** Activate Dedicated EPS Bearer Context Request */
    EpsActDedicatedEpsBearerContextRequestMessage       epsActDedicatedEpsBearerContextRequest;
    /** Activate Dedicated EPS Bearer Context Accept */
    EpsActDedicatedEpsBearerContextAcceptMessage        epsActDedicatedEpsBearerContextAccept;
    /** Activate Dedicated EPS Bearer Context Reject */
    EpsActDedicatedEpsBearerContextRejectMessage        epsActDedicatedEpsBearerContextReject;
    /** Modify EPS Bearer Context Request */
    EpsModifyEpsBearerContextRequestMessage             epsModifyEpsBearerContextRequest;
    /** Modify EPS Bearer Context Accept */
    EpsModifyEpsBearerContextAcceptMessage              epsModifyEpsBearerContextAccept;
    /** Modify EPS Bearer Context Reject */
    EpsModifyEpsBearerContextRejectMessage              epsModifyEpsBearerContextReject;
    /** Deactivate EPS Bearer Context Request */
    EpsDeactEpsBearerContextRequestMessage              epsDeactEpsBearerContextRequest;
    /** Deactivate EPS Bearer Context Accept */
    EpsDeactEpsBearerContextAcceptMessage               epsDeactEpsBearerContextAccept;
    /** PDN Connectivity Request */
    EpsPdnConnectivityRequestMessage                    epsPdnConnectivityRequest;
    /** PDN Connectivity Reject */
    EpsPdnConnectivityRejectMessage                     epsPdnConnectivityReject;
    /** PDN Disconnect Request */
    EpsPdnDisconnectRequestMessage                      epsPdnDisconnectRequest;
    /** PDN Disconnect Reject */
    EpsPdnDisconnectRejectMessage                       epsPdnDisconnectReject;
    /** Bearer Resource Allocation Request */
    EpsBearerResourceAllocRequestMessage                epsBearerResourceAllocRequest;
    /** Bearer Resource Allocation Reject */
    EpsBearerResourceAllocRejectMessage                 epsBearerResourceAllocReject;
    /** Bearer Resource Modification Request */
    EpsBearerResourceModifyRequestMessage               epsBearerResourceModifyRequest;
    /** Bearer Resource Modification Reject */
    EpsBearerResourceModifyRejectMessage                epsBearerResourceModifyReject;
    /** ESM Information Request */
    EpsEsmInfoRequestMessage                            epsEsmInfoRequest;
    /** ESM Information Response */
    EpsEsmInfoResponseMessage                           epsEsmInfoResponse;
    /** ESM Dummy message */
    EpsEsmDummyMessage                                  epsEsmDummyMessage;
    /** ESM Status */
    EpsEsmStatusMessage                                 epsEsmStatus;
    /** ESM Data Transport */
    EpsEsmDataTransportMessage                          epsEsmDataTransport;
    /** Eutra Close UE Test Loop */
    TcEutraCloseUeTestLoopMessage                       tcEutraCloseUeTestLoop;
    /** Eutra Close UE Test Loop Complete*/
    Int8                                                tcEutraCloseUeTestLoopComplete;
    /** Eutra Open UE Test Loop */
    Int8                                                tcEutraOpenUeTestLoop;
    /** Eutra Open UE Test Loop Complete*/
    Int8                                                tcEutraOpenUeTestLoopComplete;
    /** Eutra Activate Test Mode */
    TcEutraActivateTestModeMessage                      tcEutraActivateTestMode;
    /** Eutra Activate Test Mode Complete */
    Int8                                                tcEutraActivateTestModeComplete;
    /** Eutra Deactivate Test Mode */
    Int8                                                tcEutraDeactivateTestMode;
    /** Eutra Deactivate Test Mode Complete */
    Int8                                                tcEutraDeactivateTestModeComplete;
#endif /* UPGRADE_LTE */

}
MessageBody;

/** The L3 message element. */
typedef struct L3MessageTag
{
    /** The status of the L3 message */
    MessageStatus status;
    /** The header of the L3 message */
    MessageHeader header;
    /** The body of the L3 message */
    MessageBody   body;
}
L3Message;

/** L3EstDataRequest element */
typedef struct L3EstDataRequestTag
{
    /** Indicates the reason for eastablishment (only in RR_EST_REQ) */
    EstablishmentReason reason;
    Lai                 lai;                   /**< only in RR_EST_REQ */
    /** old LAC */
    Lac                 oldLac;
    /** old RAC */
    Rac                 oldRac;
    /** Determines L3 message */
    L3Message           l3Message;
}
L3EstDataRequest;

typedef struct MeasurementInformationTag
{
    Int8                     baInd;
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    Int8                     baInd3g;
#endif  /* UPGRADE_3G && UPGRADE_GGE */
    Int8                     mpChangeMark;
    Boolean                  pwrc;
    Boolean                  invalidBsicReporting;
    MeasReportType           reportType;
    MeasReportRate           reportRate;
    RtdDescription           rtdDescr;

    BsicDescription          bsicDescr;

    ReportPriorityParam      reportPriority;

    MeasurementParameters    measParam;
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
#if defined (UPGRADE_3G_FDD)
    UtranFddNcellParam       utranFddNcellParam;
    Addit3GMeasParam2        addit3GMeasParam2; /* Only used by FDD */
#endif /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)
    UtranTdd128NcellParam    utranTdd128NcellParam;
#endif /* UPGRADE_3G_TDD128 */

    MiMeasurementParameter3g measParam3g;
#endif  /* UPGRADE_3G && UPGRADE_GGE */
}
MeasurementInformation;

/** System information type 10.
 * They are  not  standard layer 3 messages. They shall be transferred using
 * the short header format for SACCH messages sent in unacknowledged mode specified
 */
typedef struct SystemInformationType10Tag
{
    /** Dummy variable */
    Int8 dummy;
}
SystemInformationType10;

/** Notification/FACCH message content */
typedef struct NotificationFacchTag
{
    /** Dummy variable */
    Int8 dummy;
}
NotificationFacch;

/** Uplink free.
 * The message indicates the uplink as free unless the Uplink Access
 * Request field indicates the uplink as not free.
 */
typedef struct UplinkFreeTag
{
    /** Dummy variable */
    Int8 dummy;
}
UplinkFree;

/** Enhanced Measurement Report message content */
typedef struct EnhancedMeasurementReportTag
{
    /** Dummy variable */
    Int8 dummy;
}
EnhancedMeasurementReport;

/** RR short protocol discriminator message type */
typedef enum ShortPdMessageTypeTag
{
    /** SI type 10;
     * Messages of this message type are optionally sent by the network
     * in unacknowledged mode on the SACCH. SYSTEM INFORMATION TYPE 10
     * messages contain information about neighbour cells
     */
    SYSTEM_INFORMATION_TYPE_10  = 0,
    /** This message is sent on the main DCCH, in unacknowledged mode using
     * the RR short protocol discriminator by the network to notify the mobile
     * stations in dedicated mode or in on-going voice broadcast calls or
     * voice group calls on other voice broadcast calls or voice group calls
     * in that cell
     */
    NOTIFICATION_FACCH          = 1,
    /** This message is sent on the main DCCH, in unacknowledged mode using the
     * RR short protocol discriminator by the network to inform the mobile station
     * of the uplink status of the voice group call channel
     */
    UPLINK_FREE                 = 2,
     /** Extended measurement reports.
     * This message is sent on the SACCH by the mobile station to the network
     * to report extended measurement results about the signal strength on specified
     * carriers
     */
    ENHANCED_MEASUREMENT_REPORT = 4,
    /** Measurement information;
     * These messages contain measurement results
     * about reception characteristics from the current cell and
     * from neighbour cells
     */
    MEASUREMENT_INFORMATION     = 5
}
ShortPdMessageType;

/** RR short protocol discriminator message body */
typedef union L3ShortPdMessageBodyTag
{
    /** SI type 10;
     * Messages of this message type are optionally sent by the network
     * in unacknowledged mode on the SACCH. SYSTEM INFORMATION TYPE 10
     * messages contain information about neighbour cells
     */
    SystemInformationType10   systemInformationType10;
    /** This message is sent on the main DCCH, in unacknowledged mode using
     * the RR short protocol discriminator by the network to notify the mobile
     * stations in dedicated mode or in on-going voice broadcast calls or
     * voice group calls on other voice broadcast calls or voice group calls
     * in that cell
     */
    NotificationFacch         notificationFacch;
    /** This message is sent on the main DCCH, in unacknowledged mode using the
     * RR short protocol discriminator by the network to inform the mobile station
     * of the uplink status of the voice group call channel
     */
    UplinkFree                uplinkFree;
    /** Extended measurement reports.
     * This message is sent on the SACCH by the mobile station to the network
     * to report extended measurement results about the signal strength on specified
     * carriers
     */
    EnhancedMeasurementReport enhancedMeasurementReport;
    /** Measurement information;
     * These messages contain measurement results
     * about reception characteristics from the current cell and
     * from neighbour cells
     */
    MeasurementInformation    measurementInfo;
}
L3ShortPdMessageBody;

/** RR short protocol discriminator message element */
typedef struct L3ShortPdMessageTag
{
    /** Cause of error */
    Err                  errorCode;
    Int32                diagnostic;
    /** Determines the type of message
\assoc UNION \ref body */
    ShortPdMessageType   type;
    /** Holds the body of the message */
    L3ShortPdMessageBody body;
}
L3ShortPdMessage;

 /** This enum is reserved for Access Stratum messages only
  * i.e GRR
  */
typedef enum AccStratumMessageTypeTag
{
    /** Additional assignment.
     * This message is sent on the main DCCH by the network to the mobile station
     * to allocate an additional dedicated channel while keeping the previously
     * allocated channels
     */
    ADDITIONAL_ASSIGNMENT        = 0x3B,
    /** Immediate assignment
     * This message is sent on the CCCH by the network to the mobile station
     * in idle mode to change the channel configuration to a dedicated configuration
     * while staying in the same cell or to the mobile station in packet idle mode
     * to change the channel configuration to either an uplink or a downlink packet
     * data channel configuration in the cell.
     */
    IMMEDIATE_ASSIGNMENT         = 0x3F,
    /** Immediate assignment extended.
     * This message is sent on the CCCH by the network to two mobile stations
     * in idle mode to change their channel configurations to different dedicated
     * configurations while they stay in the same cell.
     */
    IMMEDIATE_ASSIGNMENT_EXT     = 0x39,
    /** Immediate assignment reject.
     * This message is sent on the CCCH by the network to up to four  mobile stations
     * to indicate that no channel is available for assignment.
     */
    IMMEDIATE_ASSIGNMENT_REJ     = 0x3A,

    /** Ciphering mode command.
     * This message is sent on the main DCCH from the network to the mobile station
     * to indicate that the network has started deciphering and that enciphering
     * and deciphering shall be started in the mobile station, or to indicate that
     * ciphering will not be performed.
     */
    CIPHERING_MODE_COMMAND       = 0x35,
    /** Ciphering mode complete.
     * This message is sent on the main DCCH from the mobile station to the network
     * to indicate that enciphering and deciphering has been started in the MS.
     */
    CIPHERING_MODE_COMPLETE      = 0x32,

    /** Assignment command.
     * This message is sent on the main DCCH by the network to the mobile station
     * to change the channel configuration to another independent dedicated channel
     * configuration, when no timing adjustment is needed.
     */
    ASSIGNMENT_COMMAND           = 0x2E,
    /** Assignment complete.
     * This message is sent on the main DCCH from the mobile station to the network
     * to indicate that the mobile station has established the main signalling
     * link successfully.
     */
    ASSIGNMENT_COMPLETE          = 0x29,
    /** Assignment failure.
     * This message is sent on the main DCCH on the old channel from the mobile
     * station to the network to indicate that the mobile station has failed to
     * seize the new channel.
     */
    ASSIGNMENT_FAILURE           = 0x2F,
    /** Handover command.
     * This message is sent on the main DCCH by the network to the mobile station to
     * change the dedicated channel configuration, timing adjustment needed.
     */
    HANDOVER_COMMAND             = 0x2B,
    /** Handover complete.
     * This message is sent on the main DCCH from the mobile station to the
     * network to indicate that the mobile station has established the main
     * signalling link successfully.
     */
    HANDOVER_COMPLETE            = 0x2C,
    /** Handover failure.
     * This message is sent on the main DCCH on the old channel from the mobile
     * station to the network to indicate that the mobile station has failed to
     * seize the new channel.
     */
    HANDOVER_FAILURE             = 0x28,
    /** Physical information.
     * This message is sent on the main DCCH by the network to the mobile station
     * to stop the sending of access bursts from the mobile station.
     */
    PHYSICAL_INFORMATION         = 0x2D,

    /** Channel release; the data link layer is released explicitly */
    CHANNEL_RELEASE              = 0x0D,
    /** Partial release.
     * This message is sent on the main DCCH by the network to the mobile
     * station to deactivate part of the dedicated channels in use.
     */
    PARTIAL_RELEASE              = 0x0A,
    /** Partial release complete.
     * This message is sent on the main DCCH by the mobile station to the
     * network to indicate that a part of the dedicated channels has been
     * deactivated.
     */
    PARTIAL_RELEASE_COMPLETE     = 0x0F,

    /** Paging request type 1.
     * This message is sent on the CCCH by the network to up to two mobile
     * stations. It may be sent to a mobile station in idle mode to trigger
     * channel access.
     */
    PAGING_REQUEST_TYPE1         = 0x21,
    /** Paging request type 2.
     * This message is sent on the CCCH by the network to two or three mobile stations.
     * It may be sent to a mobile station in idle mode to trigger channel access. It may
     * be sent to a mobile station in packet idle mode to transfer MM information
     * (i.e. trigger of cell update procedure).
     */
    PAGING_REQUEST_TYPE2         = 0x22,
    /** Paging request type 3.
     * This message is sent on the CCCH by the network to four mobile stations.
     * It may be sent to a mobile station in idle mode to trigger channel access.
     * It may be sent to a mobile station in packet idle mode to transfer MM
     * information (i.e. trigger of cell update procedure).
     */
    PAGING_REQUEST_TYPE3         = 0x24,
    /** Paging response.
     * This message is sent on the main DCCH by the mobile station to the network
     * in connection with establishment of the main signalling link as a response
     * to the paging request message.
     */
    PAGING_RESPONSE              = 0x27,

    /** System information type 8.
     * This message is sent on the BCCH by the network giving information about
     * cell reselection parameters to be used in that cell.
     */
    SYSTEM_INFORMATION_TYPE8     = 0x18,
    /** System information Type 1.
     * This message is sent on the BCCH by the network to all mobile stations within
     * the cell giving information of control of the RACH and of the cell allocation.
     */
    SYSTEM_INFORMATION_TYPE1     = 0x19,
    /** System information type 2.
     * This message is sent on the BCCH by the network to all mobile stations
     * within the cell giving information of control of the RACH and of the BCCH
     * allocation in the neighbour cells
     */
    SYSTEM_INFORMATION_TYPE2     = 0x1A,
    /** System information type 3.
     * This message is sent on the BCCH by the network giving information of
     * control on the RACH, the location area identification, the cell identity
     * and various other information about the cell.
     */
    SYSTEM_INFORMATION_TYPE3     = 0x1B,
    /** System information type 4.
     * This message is sent on the BCCH by the network giving information on control
     * of the RACH, the location area identification, the cell identity and various
     * other information about the cell.
     */
    SYSTEM_INFORMATION_TYPE4     = 0x1C,
    /** System information type 5.
     * This message is sent on the SACCH by the network to mobile stations
     * within the cell giving information on the BCCH allocation in the neighbour cells.
     */
    SYSTEM_INFORMATION_TYPE5     = 0x1D,
    /** System information type 6.
     * This message is sent on the SACCH by the network to mobile stations
     * within the cell giving information of location area identification,
     * of cell identity and various other information.
     */
    SYSTEM_INFORMATION_TYPE6     = 0x1E,
    /** System information type 7.
     * This message is sent on the BCCH by the network giving information about cell
     * reselection parameters to be used in that cell.
     */
    SYSTEM_INFORMATION_TYPE7     = 0x1F,
    /** System information type 2bis.
     * This message is sent optionally on the BCCH by the network to all mobile
     * stations within the cell giving information on control of the RACH and of
     * the extension of the BCCH allocation in the neighbour cells.
     */
    SYSTEM_INFORMATION_TYPE2BIS  = 0x02,
    /** System information type 5bis.
     * This message is sent optionally on the SACCH by the network to mobile stations
     * within the cell giving information on the extension of the BCCH allocation in
     * the neighbour cells.
     */
    SYSTEM_INFORMATION_TYPE5BIS  = 0x05,
    /** System information type 2ter.
     * This message is sent optionally on the BCCH by the network to all mobile
     * stations within the cell giving information on the extension of the BCCH
     * allocation in the neighbour cells.
     */
    SYSTEM_INFORMATION_TYPE2TER  = 0x03,
    /** System information type 5ter.
     * This message is sent optionally on the SACCH by the network to mobile
     * stations within the cell giving information on the extension of the BCCH
     * allocation in the neighbour cells.
     */
    SYSTEM_INFORMATION_TYPE5TER  = 0x06,
    /** System information Type 9.
     * This message is sent on the BCCH by the network to all mobile stations
     * within the cell giving some, but not necessarily all information on the
     * scheduling of information on the BCCH.
     */
    SYSTEM_INFORMATION_TYPE9     = 0x04,
    /** System information Type 13.
     * This message is sent on the BCCH if indicated in at least one of the
     * SYSTEM INFORMATION TYPE 3, 4, 7 or 8 messages. The message is sent by
     * the network to provide information related to GPRS in the cell.
     */
    SYSTEM_INFORMATION_TYPE13    = 0x00,

    /** System information Type 2Qua */
    SYSTEM_INFORMATION_TYPE2QUA  = 0x07,
    /** System information Type 14.
     * Contains parameters for common and dedicated physical channel uplink
     * outer loop power control information for both idle and connected modes.
     */
    SYSTEM_INFORMATION_TYPE14    = 0x01,
    /** System information Type 15.
     * Contains information useful for UE-based or UE-assisted positioning methods.
     */
    SYSTEM_INFORMATION_TYPE15    = 0x44,
    /** System information Type 16.
     * Contains radio bearer, transport channel and physical channel parameters to be stored
     * by UE in idle and connected mode.
     */
    SYSTEM_INFORMATION_TYPE16    = 0x3d,
    /** System information Type 17.
     * Contains fast changing parameters for the configuration of the shared physical
     * channels to be used in connected mode.
     */
    SYSTEM_INFORMATION_TYPE17    = 0x3e,
    /** System information Type 18.
     * Contains IEs that list the PLMN identities of neigbouring cells.
     */
    SYSTEM_INFORMATION_TYPE18    = 0x40,
    /** System information Type 19 */
    SYSTEM_INFORMATION_TYPE19    = 0x41,
    /** System information Type 20 */
    SYSTEM_INFORMATION_TYPE20    = 0x42,

    /** Channel mode modify.
     * This message is sent on the main DCCH by the network to the mobile station
     * to request the setting of the mode for the indicated channel(s).
     */
    CHANNEL_MODE_MODIFY          = 0x10,
    /** RR Status.
     * This message is sent by the mobile station or the network at any time
     * to report certain error conditions.
     */
    RR_STATUS                    = 0x12,
    /** Channel mode modify acknowledge.
     * This message is sent on the main DCCH by the mobile station to the network
     * to indicate the successful or unsuccessful execution of a channel mode modify
     * request.
     */
    CHANNEL_MODE_MODIFY_ACK      = 0x17,
    /** Frequency redefinition.
     * This message is sent on the main DCCH from the network to the MS to indicate
     * that the frequencies and the hopping sequence of the allocated channels shall
     * be changed.
     */
    FREQUENCY_REDEFINITION       = 0x14,
    /** Classmark change.
     * This message is sent on the main DCCH by the mobile station to the network
     * to indicate a classmark change or as a response to a classmark enquiry.
     */
    CLASSMARK_CHANGE             = 0x16,
    /** Measurement report.
     * This message is sent on the SACCH by the mobile station to the network
     * to report measurement results about the dedicated channel and about neighbour cells.
     */
    MEASUREMENT_REPORT           = 0x15,
    /** Classmark enquiry.
     * This message is sent on the main DCCH by the network to the
     * mobile station to request classmark information.
     */
    CLASSMARK_ENQUIRY            = 0x13,
    /** Extended measurement order.
     * This message is sent on the SACCH by the network to the mobile
     * station, to order the mobile station to send one extended measurement report.
     */
    EXTENDED_MEASUREMENT_ORDER   = 0x37,
    /** Extended measurement report.
     * This message is sent on the SACCH by the mobile station to the network to
     * report extended measurement results about the signal strength on specified
     * carriers.
     */
    EXTENDED_MEASUREMENT_REPORT  = 0x36,
    /** GPRS suspension request.
     * This message is sent on the main DCCH by the mobile station to the network
     * to request a suspension of GPRS services.
     */
    GPRS_SUSPENSION_REQUEST      = 0x34,

#if defined(UPGRADE_APP_INFO)
    /** RR application information */
    RR_APPLICATION_INFORMATION   = 0x38,
#endif  /* (UPGRADE_APP_INFO) */

    /* DTM messaging would be included here as follows:-
    ** DTM_ASSIGNMENT_FAILURE        = 0x48,
    ** DTM_REJECT                    = 0x49,
    ** DTM_REQUEST                   = 0x4a,
    ** PACKET_ASSIGNMENT             = 0x4b,
    ** DTM_ASSIGNMENT_COMMAND        = 0x4c,
    ** DTM_INFORMATION               = 0x4d,
    ** PACKET_NOTIFICATION           = 0x4e,
    */

#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    /** UTRAN classmark change.
     * This message is sent on the main DCCH by the mobile station to the network to
     * indicate a UTRAN classmark change or as a response to a UTRAN classmark enquiry
     */
    UTRAN_CLASSMARK_CHANGE       = 0x60,
    /** CDMA2000 classmark change.
     * This message is sent on the main DCCH by the mobile station to the network to
     * indicate a CDMA2000 classmark change or as a response to a CDMA2000 classmark enquiry
     */
    CDMA2000_CLASSMARK_CHANGE    = 0x62,
    /** Inter system to UTRAN handover command */
    INTER_SYS_TO_UTRAN_HO_CMD    = 0x63,
    /** Inter system to CDMA2000 handover command */
    INTER_SYS_TO_CDMA2000_HO_CMD = 0x64,
    /** GERAN classmark change */
    GERAN_IU_CLASSMARK_CHANGE    = 0x65,
#endif  /* UPGRADE_3G && UPGRADE_GGE */
    /** Last access stratum message type */
    LAST_ACC_STRATUM_MSG_TYPE    = 0xff
}
AccStratumMessageType;

typedef union AccStratumMessageUnionTag
{
    /** Additional assignment.
     * This message is sent on the main DCCH by the network to the mobile station
     * to allocate an additional dedicated channel while keeping the previously
     * allocated channels
     */
    AdditionalAssignmentMessage      additionalAssignment;
    /** Immediate assignment
     * This message is sent on the CCCH by the network to the mobile station
     * in idle mode to change the channel configuration to a dedicated configuration
     * while staying in the same cell or to the mobile station in packet idle mode
     * to change the channel configuration to either an uplink or a downlink packet
     * data channel configuration in the cell.
     */
    ImmediateAssignmentMessage       immediateAssignment;
    /** Immediate assignment extended.
     * This message is sent on the CCCH by the network to two mobile stations
     * in idle mode to change their channel configurations to different dedicated
     * configurations while they stay in the same cell.
     */
    ImmediateAssignmentExtMessage    immediateAssignmentExt;
    /** Immediate assignment reject.
     * This message is sent on the CCCH by the network to up to four  mobile stations
     * to indicate that no channel is available for assignment.
     */
    ImmediateAssignmentRejMessage    immediateAssignmentRej;

    /** Ciphering mode command.
     * This message is sent on the main DCCH from the network to the mobile station
     * to indicate that the network has started deciphering and that enciphering
     * and deciphering shall be started in the mobile station, or to indicate that
     * ciphering will not be performed.
     */
    CipheringModeCommandMessage      cipheringModeCommand;
    /** Ciphering mode complete.
     * This message is sent on the main DCCH from the mobile station to the network
     * to indicate that enciphering and deciphering has been started in the MS.
     */
    CipheringModeCompleteMessage     cipheringModeComplete;

    /** Assignment command.
     * This message is sent on the main DCCH by the network to the mobile station
     * to change the channel configuration to another independent dedicated channel
     * configuration, when no timing adjustment is needed.
     */
    AssignmentCommandMessage         assignmentCommand;
    /** Assignment complete.
     * This message is sent on the main DCCH from the mobile station to the network
     * to indicate that the mobile station has established the main signalling
     * link successfully.
     */
    AssignmentCompleteMessage        assignmentComplete;
    /** Assignment failure.
     * This message is sent on the main DCCH on the old channel from the mobile
     * station to the network to indicate that the mobile station has failed to
     * seize the new channel.
     */
    AssignmentFailureMessage         assignmentFailure;
    /** Handover command.
     * This message is sent on the main DCCH by the network to the mobile station to
     * change the dedicated channel configuration, timing adjustment needed.
     */
    HandoverCommandMessage           handoverCommand;
    /** Handover complete.
     * This message is sent on the main DCCH from the mobile station to the
     * network to indicate that the mobile station has established the main
     * signalling link successfully.
     */
    HandoverCompleteMessage          handoverComplete;
    /** Handover failure.
     * This message is sent on the main DCCH on the old channel from the mobile
     * station to the network to indicate that the mobile station has failed to
     * seize the new channel.
     */
    HandoverFailureMessage           handoverFailure;
    /** Physical information.
     * This message is sent on the main DCCH by the network to the mobile station
     * to stop the sending of access bursts from the mobile station.
     */
    PhysicalInformationMessage       physicalInformation;

    /** Channel release; the data link layer is released explicitly */
    ChannelReleaseMessage            channelRelease;
    /** Partial release.
     * This message is sent on the main DCCH by the network to the mobile
     * station to deactivate part of the dedicated channels in use.
     */
    PartialReleaseMessage            partialRelease;
    /** Partial release complete.
     * This message is sent on the main DCCH by the mobile station to the
     * network to indicate that a part of the dedicated channels has been
     * deactivated.
     */
    EmptyMemberOfUnion               partialReleaseComplete;

    /** Paging request type 1.
     * This message is sent on the CCCH by the network to up to two mobile
     * stations. It may be sent to a mobile station in idle mode to trigger
     * channel access.
     */
    PagingRequestMessage             pagingRequest;
    /** Paging request type 2.
     * This message is sent on the CCCH by the network to two or three mobile stations.
     * It may be sent to a mobile station in idle mode to trigger channel access. It may
     * be sent to a mobile station in packet idle mode to transfer MM information
     * (i.e. trigger of cell update procedure).
     */
    PagingRequestMessage             pagingRequest2;
    /** Paging request type 3.
     * This message is sent on the CCCH by the network to four mobile stations.
     * It may be sent to a mobile station in idle mode to trigger channel access.
     * It may be sent to a mobile station in packet idle mode to transfer MM
     * information (i.e. trigger of cell update procedure).
     */
    PagingRequestMessage             pagingRequest3;
    /** Paging response.
     * This message is sent on the main DCCH by the mobile station to the network
     * in connection with establishment of the main signalling link as a response
     * to the paging request message.
     */
    PagingResponseMessage            pagingResponse;

    /** System information type 8.
     * This message is sent on the BCCH by the network giving information about
     * cell reselection parameters to be used in that cell.
     */
    SystemInformationType8Message    systemInformationType8;
    /** System information Type 1.
     * This message is sent on the BCCH by the network to all mobile stations within
     * the cell giving information of control of the RACH and of the cell allocation.
     */
    SystemInformationType1Message    systemInformationType1;
    /** System information type 2.
     * This message is sent on the BCCH by the network to all mobile stations
     * within the cell giving information of control of the RACH and of the BCCH
     * allocation in the neighbour cells
     */
    SystemInformationType2Message    systemInformationType2;
     /** System information type 3.
     * This message is sent on the BCCH by the network giving information of
     * control on the RACH, the location area identification, the cell identity
     * and various other information about the cell.
     */
    SystemInformationType3Message    systemInformationType3;
    /** System information type 4.
     * This message is sent on the BCCH by the network giving information on control
     * of the RACH, the location area identification, the cell identity and various
     * other information about the cell.
     */
    SystemInformationType4Message    systemInformationType4;
    /** System information type 5.
     * This message is sent on the SACCH by the network to mobile stations
     * within the cell giving information on the BCCH allocation in the neighbour cells.
     */
    SystemInformationType5Message    systemInformationType5;
    /** System information type 6.
     * This message is sent on the SACCH by the network to mobile stations
     * within the cell giving information of location area identification,
     * of cell identity and various other information.
     */
    SystemInformationType6Message    systemInformationType6;
    /** System information type 7.
     * This message is sent on the BCCH by the network giving information about cell
     * reselection parameters to be used in that cell.
     */
    SystemInformationType7Message    systemInformationType7;

    /** System information type 2bis.
     * This message is sent optionally on the BCCH by the network to all mobile
     * stations within the cell giving information on control of the RACH and of
     * the extension of the BCCH allocation in the neighbour cells.
     */
    SystemInformationType2bisMessage systemInformationType2bis;
     /** System information type 5bis.
     * This message is sent optionally on the SACCH by the network to mobile stations
     * within the cell giving information on the extension of the BCCH allocation in
     * the neighbour cells.
     */
    SystemInformationType5bisMessage systemInformationType5bis;

    /** System information type 2ter.
     * This message is sent optionally on the BCCH by the network to all mobile
     * stations within the cell giving information on the extension of the BCCH
     * allocation in the neighbour cells.
     */
    SystemInformationType2TerMessage systemInformationType2Ter;
    /** System information type 5ter.
     * This message is sent optionally on the SACCH by the network to mobile
     * stations within the cell giving information on the extension of the BCCH
     * allocation in the neighbour cells.
     */
    SystemInformationType5TerMessage systemInformationType5Ter;
    /** System information Type 9.
     * This message is sent on the BCCH by the network to all mobile stations
     * within the cell giving some, but not necessarily all information on the
     * scheduling of information on the BCCH.
     */
    SystemInformationType9           systemInformationType9;
    /** System information Type 13.
     * This message is sent on the BCCH if indicated in at least one of the
     * SYSTEM INFORMATION TYPE 3, 4, 7 or 8 messages. The message is sent by
     * the network to provide information related to GPRS in the cell.
     */
    SystemInformationType13          systemInformationType13;

    /* R99 / Release 4 additions */
    /** System information Type 2Qua */
    SystemInformationType2Qua        systemInformationType2Qua;
    /** System information Type 14.
     * Contains parameters for common and dedicated physical channel uplink
     * outer loop power control information for both idle and connected modes.
     */
    EmptyMemberOfUnion               systemInformationType14;
    /** System information Type 15.
     * Contains information useful for UE-based or UE-assisted positioning methods.
     */
    EmptyMemberOfUnion               systemInformationType15;
    /** System information Type 16.
     * Contains radio bearer, transport channel and physical channel parameters to be stored
     * by UE in idle and connected mode.
     */
    EmptyMemberOfUnion               systemInformationType16;
    /** System information Type 17.
     * Contains fast changing parameters for the configuration of the shared physical
     * channels to be used in connected mode.
     */
    EmptyMemberOfUnion               systemInformationType17;
    /** System information Type 18.
     * Contains IEs that list the PLMN identities of neigbouring cells.
     */
    EmptyMemberOfUnion               systemInformationType18;
    /** System information Type 19 */
    EmptyMemberOfUnion               systemInformationType19;
    /** System information Type 20 */
    EmptyMemberOfUnion               systemInformationType20;

    /** Channel mode modify.
     * This message is sent on the main DCCH by the network to the mobile station
     * to request the setting of the mode for the indicated channel(s).
     */
    ChannelModeModifyMessage         channelModeModify;
    /** RR Status.
     * This message is sent by the mobile station or the network at any time
     * to report certain error conditions.
     */
    RrStatusMessage                  rrStatus;
    /** Channel mode modify acknowledge.
     * This message is sent on the main DCCH by the mobile station to the network
     * to indicate the successful or unsuccessful execution of a channel mode modify
     * request.
     */
    ChannelModeModifyAckMessage      channelModeModifyAck;
    /** Frequency redefinition.
     * This message is sent on the main DCCH from the network to the MS to indicate
     * that the frequencies and the hopping sequence of the allocated channels shall
     * be changed.
     */
    FreqRedefinitionMessage          freqRedefinition;
    /** Classmark change.
     * This message is sent on the main DCCH by the mobile station to the network
     * to indicate a classmark change or as a response to a classmark enquiry.
     */
    ClassmarkChangeMessage           classmarkChange;
    /** Measurement report.
     * This message is sent on the SACCH by the mobile station to the network
     * to report measurement results about the dedicated channel and about neighbour cells.
     */
    MeasurementReportMessage         measurementReport;
    /** Classmark enquiry.
     * This message is sent on the main DCCH by the network to the
     * mobile station to request classmark information.
     */
    ClassmarkEnquiryMessage          classmarkEnquiry;
    /** Extended measurement order.
     * This message is sent on the SACCH by the network to the mobile
     * station, to order the mobile station to send one extended measurement report.
     */
    ExtendedMeasurementOrderMsg      extendedMeasurementOrder;
    /** Extended measurement report.
     * This message is sent on the SACCH by the mobile station to the network to
     * report extended measurement results about the signal strength on specified
     * carriers.
     */
    ExtendedMeasurementReportMsg     extendedMeasurementReport;
    /** GPRS suspension request.
     * This message is sent on the main DCCH by the mobile station to the network
     * to request a suspension of GPRS services.
     */
    GprsSuspensionRequestMsg         gprsSuspensionRequest;

#if defined(UPGRADE_APP_INFO)
    /** RR application information */
    EmptyMemberOfUnion               rrApplicationInformation;
#endif  /* (UPGRADE_APP_INFO) */

    /* DTM messaging would be included here as follows:-
    ** EmptyMemberOfUnion                  dtmAssignmentFailure;
    ** EmptyMemberOfUnion                  dtmReject;
    ** EmptyMemberOfUnion                  dtmRequest;
    ** EmptyMemberOfUnion                  dtmPacketAssignment;
    ** EmptyMemberOfUnion                  dtmAssignmentCommand;
    ** EmptyMemberOfUnion                  dtmInformation;
    ** EmptyMemberOfUnion                  dtmPacketNotification;
    */

#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    /** UTRAN classmark change.
     * This message is sent on the main DCCH by the mobile station to the network to
     * indicate a UTRAN classmark change or as a response to a UTRAN classmark enquiry
     */
    EmptyMemberOfUnion utranClassmarkChange;
    /** CDMA2000 classmark change.
     * This message is sent on the main DCCH by the mobile station to the network to
     * indicate a CDMA2000 classmark change or as a response to a CDMA2000 classmark enquiry
     */
    EmptyMemberOfUnion cdma2000ClassmarkChange;
    /** Inter system to UTRAN handover command */
    EmptyMemberOfUnion interSysToUtranHoCmd;
    /** Inter system to CDMA2000 handover command */
    EmptyMemberOfUnion interSysToCdma2000HoCmd;
    /** GERAN classmark change */
    EmptyMemberOfUnion geranIuClassmarkChange;
#endif  /* UPGRADE_3G && UPGRADE_GGE */
}
AccStratumMessageUnion;

/** Access Stratum message element */
typedef struct AccessStratumMessageTag
{
    /** Indicates the status of the message */
    MessageStatus          status;
    /** Used to indicate what CM sub-layer the signal is from
     * (in the MS) and for which layer in the network it is.
     */
    ProtocolDiscriminator  pd;
    /** Specifies the type of AS message
\assoc UNION \ref body */
    AccStratumMessageType  messageType;
    /** The contents of the message */
    AccStratumMessageUnion body;
}
AccessStratumMessage;

#endif /* !(L3_TYP_H) */
/* END OF FILE */
