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
 *      types common to mnxx_sig.h mmxx_sig.h rr_sig.h ts_sig.h and smrl_sig.h
 **************************************************************************/

#ifndef SML3_TYP_H
#define SML3_TYP_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

/****************************************************************************
 * Macros
 ****************************************************************************/

/****************************************************************************
 * Types
 ****************************************************************************/
/** This is a set of causes combined from GSM CC,
 * (G)MM and RR messages, and internal protocol stack causes
 */
typedef enum GsmCauseTag
/*
From 04.08 10.5.4.11
And 04.08 10.5.3.6
*/
{
    /* SM cause values
    ** (ref. GSM 24.008/10.5.6.6).
    ** Note previously contained in smcause.h */
    /** LLC or SNDCP failure (A/Gb mode only) */
    SM_CAUSE_LLC_OR_SNDCP_FAILURE = 0x19,
    /** Insufficient resources */
    SM_CAUSE_INSUFFIC_RESOURCES = 0x1A,
    /** Missing or unknown APN */
    SM_CAUSE_MISSING_OR_UNKNOWN_APN = 0x1B,
    /** Unknown PDP address or PDP type */
    SM_CAUSE_UNKNOWN_PDP_ADDR_OR_TYPE = 0x1C,
    /** User authentication failed */
    SM_CAUSE_USER_AUTH_FAILED = 0x1D,
#if defined (UPGRADE_LTE)
    /** Activation rejected by GGSN, Serving GW or PDN GW */
#else /* UPGRADE_LTE */
    /**Activation rejected by GGSN */
#endif /* UPGRADE_LTE */
    SM_CAUSE_ACTIV_REJ_BY_GGSN = 0x1E,
    /** Activation rejected, unspecified */
    SM_CAUSE_ACTIV_REJ_UNSPECIFIED = 0x1F,
    /** Service option not supported */
    SM_CAUSE_SERVICE_OPT_NOT_SUPPORTED = 0x20,
    /** Requested service option not subscribed */
    SM_CAUSE_SERVICE_OPT_NOT_SUBSCRIBED = 0x21,
    /** Service option temporarily out of order */
    SM_CAUSE_SERVICE_OPT_TEMP_OUT_OF_ORDER = 0x22,
    /** NSAPI already used (not sent) */
    SM_CAUSE_NSAPI_ALREADY_USED = 0x23,
    /** Regular deactivation */
    SM_CAUSE_REGULAR_DEACTIVATION = 0x24,
    /** QoS not accepted */
    SM_CAUSE_QOS_NOT_ACCEPTED = 0x25,
    /** Network failure */
    SM_CAUSE_NETWORK_FAILURE = 0x26,
    /** Reactivation required */
    SM_CAUSE_REACTIVATION_REQUIRED = 0x27,
    /** Feature not supported */
    SM_CAUSE_FEATURE_NOT_SUPPORTED = 0x28,    /* Added for 0111-13748 */
    /** Semantic error in the TFT operation */
    SM_CAUSE_SEMANTIC_ERROR_IN_TFT_OPERATION = 0x29,
    /** Syntactical error in the TFT operation */
    SM_CAUSE_SYNTACTICAL_ERROR_IN_TFT_OPERATION = 0x2A,
    /** Unknown PDP context */
    SM_CAUSE_UNKNOWN_PDP_CONTEXT = 0x2B,
    /** Semantic errors in packet filter(s) */
    SM_CAUSE_SEMANTIC_ERRORS_IN_PACKET_FILTER = 0x2C,
    /** Syntactical errors in packet filter(s) */
    SM_CAUSE_SYNTACTICAL_ERRORS_IN_PACKET_FILTER = 0x2D,
    /** PDP context without TFT already activated */
    SM_CAUSE_PDP_CONTEXT_WITHOUT_TFT_ALREADY_ACTIVATED = 0x2E,
#if defined (UPGRADE_NASMDL2)
    /** PDP type IPv4 only allowed */
    SM_CAUSE_PDN_TYPE_IPV4_ONLY_ALLOWED = 0x32,
    /** PDP type IPv6 only allowed */
    SM_CAUSE_PDN_TYPE_IPV6_ONLY_ALLOWED = 0x33,
    /** Single address bearers only allowed */
    SM_CAUSE_SINGLE_ADDRESS_BEARERS_ONLY_ALLOWED = 0x34,
#endif /* UPGRADE_NASMDL2 */
    /** Invalid transaction identifier value */
    SM_CAUSE_INVALID_TI_VALUE = 0x51,
    /** Semantically incorrect message */
    SM_CAUSE_SEMANTICALLY_INCORRECT_MSG = 0x5F,
    /** Invalid mandatory information */
    SM_CAUSE_INVALID_MAND_INFORMATION = 0x60,
    /** Message type non-existent or not implemented */
    SM_CAUSE_MSG_TYPE_NONEXIST_OR_NOT_IMP = 0x61,
    /** Message type not compatible with the protocol state */
    SM_CAUSE_MSG_TYPE_INCOMPAT_WITH_STATE = 0x62,
    /** Information element non-existent or not implemented */
    SM_CAUSE_IE_NONEXIST_OR_NOT_IMP = 0x63,
    /** Conditional IE error */
    SM_CAUSE_CONDITIONAL_IE_ERROR = 0x64,
    /** Message not compatible with the protocol state */
    SM_CAUSE_MSG_INCOMPAT_WITH_STATE = 0x65,
    /** Protocol error, unspecified */
    SM_CAUSE_PROTOCOL_ERROR_UNSPEC = 0x6F,
#if defined (UPGRADE_NASMDL2)
    /** APN restriction value incompatible with active PDP context */
    SM_CAUSE_APN_RESTRICTION_VALUE_INCOMPATIBLE = 0x70,
#endif /* UPGRADE_NASMDL2 */
    /* all internal errors removed as not needed */
    /** No SM cause */
    SM_CAUSE_NO_CAUSE_SET  = 0x3400,
    /* END SM cause values */

#if defined (UPGRADE_LTE)
    /* All LTE Cause Values have the top bit set to 1.  This is not the
     * the same as the ETSI specifications so the L23 Enc/Dec functions must
     * remove/add the bit as appropriate.
     * This is so that, when logging, we can tell the difference between
     * LTE cause values and other values which, in the specifications, have
     * the same value
     */
    LTE_CAUSE_BASE = 0x8000,
    /* ESM cause values
    ** (ref. GSM 24.301/9.9.4.4). */
    /** Operator Determined Barring */
    ESM_CAUSE_OPERATOR_DETERMINED_BARRING = LTE_CAUSE_BASE + 0x08,
    /** Insufficient resources */
    ESM_CAUSE_INSUFFICIENT_RESOURCES = LTE_CAUSE_BASE + 0x1A,
    /** Unknown or missing APN */
    ESM_CAUSE_UNKNOWN_MISSING_APN = LTE_CAUSE_BASE + 0x1B,
    /** Unknown PDN type */
    ESM_CAUSE_UNKNOWN_PDN_TYPE = LTE_CAUSE_BASE + 0x1C,
    /** User authentication failed */
    ESM_CAUSE_USER_AUTHENTICATION_FAILED = LTE_CAUSE_BASE + 0x1D,
    /** Request rejected by Serving GW or PDN GW */
    ESM_CAUSE_REQ_REJECTED_BY_SERVING_GW_PDNGW = LTE_CAUSE_BASE + 0x1E,
    /** Request rejected, unspecified */
    ESM_CAUSE_REQ_REJECTED_UNSPECIFIED = LTE_CAUSE_BASE + 0x1F,
    /** Service option not supported */
    ESM_CAUSE_SERVICE_OPTION_NOT_SUPPORTED = LTE_CAUSE_BASE + 0x20,
    /** Requested service option not subscribed */
    ESM_CAUSE_REQ_SERVICE_OPTION_NOT_SUBSCRIBED = LTE_CAUSE_BASE + 0x21,
    /** Service option temporarily out of order */
    ESM_CAUSE_SERVICE_OPT_TEMP_OUT_OF_ORDER = LTE_CAUSE_BASE + 0x22,
    /** PTI already in use */
    ESM_CAUSE_PTI_ALREADY_IN_USE = LTE_CAUSE_BASE + 0x23,
    /** Regular deactivation */
    ESM_CAUSE_REGULAR_DEACTIVATION = LTE_CAUSE_BASE + 0x24,
    /** EPS QoS not accepted */
    ESM_CAUSE_EPS_QOS_NOT_ACCEPTED = LTE_CAUSE_BASE + 0x25,
    /** Network failure */
    ESM_CAUSE_NETWORK_FAILURE = LTE_CAUSE_BASE + 0x26,
#if defined (UPGRADE_REL9)
    /** Reactivation requested */
    ESM_CAUSE_REACTIVATION_REQUESTED = LTE_CAUSE_BASE + 0x27,
#endif /* UPGRADE_REL9 */
    /** Semantic error in the TFT operation */
    ESM_CAUSE_SEMANTIC_ERROR_IN_TFT_OP = LTE_CAUSE_BASE + 0x29,
    /** Syntactical error in the TFT operation */
    ESM_CAUSE_SYNTACTICAL_ERROR_IN_TFT_OP = LTE_CAUSE_BASE + 0x2A,
    /** Invalid EPS bearer identity */
    ESM_CAUSE_INVALID_EPS_BEARER_ID = LTE_CAUSE_BASE + 0x2B,
    /** Semantic errors in packet filter(s) */
    ESM_CAUSE_SEMANTIC_ERRORS_IN_PACKET_FILTERS = LTE_CAUSE_BASE + 0x2C,
    /** Syntactical errors in packet filter(s) */
    ESM_CAUSE_SYNTACTICAL_ERRORS_IN_PACKET_FILTERS = LTE_CAUSE_BASE + 0x2D,
    /** EPS bearer context without TFT already activated */
    ESM_CAUSE_EPS_B_CONTEXT_WITHOUT_TFT_ALREADY_ACTIVATED = LTE_CAUSE_BASE + 0x2E,
    /** PTI mismatch */
    ESM_CAUSE_PTI_MISMATCH = LTE_CAUSE_BASE + 0x2F,
    /** Last PDN disconnection not allowed */
    ESM_CAUSE_LAST_PDN_DISCONNECTION_NOT_ALLOWED = LTE_CAUSE_BASE + 0x31,
    /** PDN type IPv4 only allowed */
    ESM_CAUSE_PDN_TYPE_IPV4_ONLY_ALLOWED = LTE_CAUSE_BASE + 0x32,
    /** PDN type IPv6 only allowed */
    ESM_CAUSE_PDN_TYPE_IPV6_ONLY_ALLOWED = LTE_CAUSE_BASE + 0x33,
    /** Single address bearers only allowed */
    ESM_CAUSE_SINGLE_ADDRESS_BEARERS_ONLY_ALLOWED = LTE_CAUSE_BASE + 0x34,
    /** ESM information not received */
    ESM_CAUSE_ESM_INFORMATION_NOT_RECEIVED = LTE_CAUSE_BASE + 0x35,
    /** PDN connection does not exist */
    ESM_CAUSE_PDN_CONNECTION_DOES_NOT_EXIST = LTE_CAUSE_BASE + 0x36,
    /** Multiple PDN connections for a given APN not allowed */
    ESM_CAUSE_MULTIPLE_PDN_CONN_NOT_ALLOWED_FOR_ONE_APN = LTE_CAUSE_BASE + 0x37,
    /** Collision with network initiated request */
    ESM_CAUSE_COLLISION_WITH_NW_INITIATED_REQUEST = LTE_CAUSE_BASE + 0x38,
    /** PDN type IPv4v6 only allowed */
    ESM_CAUSE_PDN_TYPE_IPV4V6_ONLY_ALLOWED = LTE_CAUSE_BASE + 0x39,
    /** PDN type non IP only allowed */
    ESM_CAUSE_PDN_TYPE_NON_IP_ONLY_ALLOWED = LTE_CAUSE_BASE + 0x3A,
    /** Unsupported QCI value */
    ESM_CAUSE_UNSUPPORTED_QCI_VALUE = LTE_CAUSE_BASE + 0x3B,
    /** Bearer handling not supported */
    ESM_CAUSE_BEARER_HANDLING_NOT_SUPPORTED = LTE_CAUSE_BASE + 0x3C,
    /** Maximum number of EPS bearers reached */
    ESM_CAUSE_MAXIMUM_NUMBER_OF_EPS_BEARERS_REACHED = LTE_CAUSE_BASE + 0x41,
    /** Requested APN not supported in current RAT and PLMN combination */
    ESM_CAUSE_REQUESTED_APN_NOT_SUPPORTED_IN_CURRENT_RAT_AND_PLMN_COMBIN = LTE_CAUSE_BASE + 0x42,
    /** Invalid PTI value */
    ESM_CAUSE_INVALID_PTI_VALUE = LTE_CAUSE_BASE + 0x51,
    /** Semantically incorrect message */
    ESM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE = LTE_CAUSE_BASE + 0x5F,
    /** Invalid mandatory information */
    ESM_CAUSE_INVALID_MANDATORY_INFORMATION = LTE_CAUSE_BASE + 0x60,
    /** Message type non-existent or not implemented */
    ESM_CAUSE_MESSAGE_TYPE_NON_EXIST_OR_NOT_IMP = LTE_CAUSE_BASE + 0x61,
    /** Message type not compatible with the protocol state */
    ESM_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_STATE = LTE_CAUSE_BASE + 0x62,
    /** Information element non-existent or not implemented */
    ESM_CAUSE_IE_NON_EXIST_OR_NOT_IMP = LTE_CAUSE_BASE + 0x63,
    /** Conditional IE error */
    ESM_CAUSE_CONDITIONAL_IE_ERROR = LTE_CAUSE_BASE + 0x64,
    /** Message not compatible with the protocol state */
    ESM_CAUSE_MESSAGE_NOT_COMPAT_WITH_STATE = LTE_CAUSE_BASE + 0x65,
    /** Protocol error, unspecified */
    ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED = LTE_CAUSE_BASE + 0x6F,
    /** APN restriction value incompatible with active EPS bearer context */
    ESM_CAUSE_APN_RESTRICTION_VALUE_INCOMPATIBLE = LTE_CAUSE_BASE + 0x70,
    /** Multiple accesses to a PDN connection not allowed */
    ESM_CAUSE_MULTIPLE_ACCESSES_TO_PDN_CONN_NOT_ALLOWED = LTE_CAUSE_BASE + 0x71,
    /** message is not acked by NW */
    ESM_CAUSE_MESSAGE_NOT_ACKED = LTE_CAUSE_BASE + 0x101,
    /** No ESM cause */
    ESM_CAUSE_NO_CAUSE_SET  = LTE_CAUSE_BASE + 0x3400,

    /* EMM cause values
    ** (ref. GSM 24.301/9.9.3.9). */
    /** IMSI unknown in HSS */
    EMM_CAUSE_IMSI_UNKNOWN_IN_HSS = LTE_CAUSE_BASE + 0x02,
    /** Illegal UE */
    EMM_CAUSE_ILLEGAL_UE = LTE_CAUSE_BASE + 0x03,
    /** Illegal ME */
    EMM_CAUSE_ILLEGAL_ME = LTE_CAUSE_BASE + 0x06,
    /** EPS services not allowed */
    EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED = LTE_CAUSE_BASE + 0x07,
    /** EPS services and non-EPS services not allowed */
    EMM_CAUSE_EPS_AND_NON_EPS_SERVICES_NOT_ALLOWED = LTE_CAUSE_BASE + 0x08,
    /** UE identity cannot be derived by the network */
    EMM_CAUSE_UE_ID_CANNOT_BE_DERIVED = LTE_CAUSE_BASE + 0x09,
    /** Implicitly detached */
    EMM_CAUSE_IMPLICITLY_DETACHED = LTE_CAUSE_BASE + 0x0A,
    /** PLMN not allowed */
    EMM_CAUSE_PLMN_NOT_ALLOWED = LTE_CAUSE_BASE + 0x0B,
    /** Tracking Area not allowed */
    EMM_CAUSE_TA_NOT_ALLOWED = LTE_CAUSE_BASE + 0x0C,
    /** Roaming not allowed in this tracking area */
    EMM_CAUSE_ROAMING_NOT_ALLOWED_IN_TA = LTE_CAUSE_BASE + 0x0D,
    /** EPS services not allowed in this PLMN */
    EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED_IN_PLMN = LTE_CAUSE_BASE + 0x0E,
    /** No Suitable Cells In tracking area */
    EMM_CAUSE_NO_SUITABLE_CELLS_IN_TA = LTE_CAUSE_BASE + 0x0F,
    /** MSC temporarily not reachable */
    EMM_CAUSE_MSC_TEMP_NOT_REACHABLE = LTE_CAUSE_BASE + 0x10,
    /** Network failure */
    EMM_CAUSE_NETWORK_FAILURE = LTE_CAUSE_BASE + 0x11,
    /** CS domain not available */
    EMM_CAUSE_CS_DOMAIN_NOT_AVAILABLE = LTE_CAUSE_BASE + 0x12,
    /** ESM failure */
    EMM_CAUSE_ESM_FAILURE = LTE_CAUSE_BASE + 0x13,
    /** MAC failure */
    EMM_CAUSE_MAC_FAILURE = LTE_CAUSE_BASE + 0x14,
    /** Synch failure */
    EMM_CAUSE_SYNCH_FAILURE = LTE_CAUSE_BASE + 0x15,
    /** Congestion */
    EMM_CAUSE_CONGESTION = LTE_CAUSE_BASE + 0x16,
    /** UE security capabilities mismatch */
    EMM_CAUSE_UE_SECURITY_CAP_MISMATCH = LTE_CAUSE_BASE + 0x17,
    /** Security mode rejected, unspecified */
    EMM_CAUSE_SECURITY_MODE_REJ_UNSPEC = LTE_CAUSE_BASE + 0x18,
    /** Not authorized for this CSG */
    EMM_CAUSE_NOT_AUTHORIZED_FOR_CSG = LTE_CAUSE_BASE + 0x19,
    /** Requested service not authorized in this PLMN */
    EMM_CAUSE_REQUESTED_SERVICE_NOT_AUTHORIZED_IN_THIS_PLMN = LTE_CAUSE_BASE + 0x23,
    /** Non-EPS authentication unacceptable */
    EMM_CAUSE_NON_EPS_AUTH_UNACCEPTABLE = LTE_CAUSE_BASE + 0x1A,
    /** CS domain temporarily not available */
    EMM_CAUSE_CS_DOMAIN_TEMP_UNAVAILABLE = LTE_CAUSE_BASE + 0x27,
    /** No EPS bearer context activated */
    EMM_CAUSE_NO_EPS_BEARER_CONTEXT_ACTIVATED = LTE_CAUSE_BASE + 0x28,
    /** Severe network failure */
    EMM_CAUSE_SEVERE_NETWORK_FAILURE = LTE_CAUSE_BASE + 0x2A,
    /** Semantically incorrect message */
    EMM_CAUSE_SEMANTICALLY_INCORRECT_MSG = LTE_CAUSE_BASE + 0x5F,
    /** Invalid mandatory information */
    EMM_CAUSE_INVALID_MANDATORY_INFO = LTE_CAUSE_BASE + 0x60,
    /** Message type non-existent or not implemented */
    EMM_CAUSE_MESSAGE_TYPE_NON_EXIST_OR_NOT_IMP = LTE_CAUSE_BASE + 0x61,
    /** Message type not compatible with the protocol state */
    EMM_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_STATE = LTE_CAUSE_BASE + 0x62,
    /** Information element non-existent or not implemented */
    EMM_CAUSE_IE_NON_EXIST_OR_NOT_IMP = LTE_CAUSE_BASE + 0x63,
    /** Conditional IE error */
    EMM_CAUSE_CONDITIONAL_IE_ERROR = LTE_CAUSE_BASE + 0x64,
    /** Message not compatible with the protocol state */
    EMM_CAUSE_MESSAGE_NOT_COMPAT_WITH_STATE = LTE_CAUSE_BASE + 0x65,
    /** Protocol error, unspecified */
    EMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED = LTE_CAUSE_BASE + 0x6F,
    /** No ESM cause */
    EMM_CAUSE_NO_CAUSE_SET  = LTE_CAUSE_BASE + 0x3400,

#endif /* UPGRADE_LTE */

    /* GMM cause values
    ** (ref. GSM 24.008/10.5.5.14).
    ** Note previously contained in gmmcause.h */

    /** GPRS services not allowed */
    CAUSE_GMM_GPRS_SERVICES_NOT_ALLOWED      = 0x07,
    /** GPRS services and non-GPRS services not allowed */
    CAUSE_GMM_COMBINED_SERVICES_NOT_ALLOWED  = 0x08,
    /** MS identity cannot be derived by the network */
    CAUSE_GMM_MS_ID_NOT_IN_NETWORK           = 0x09,
    /** Implicitly detached */
    CAUSE_GMM_IMPLICITLY_DETACHED            = 0x0a,
    /** GPRS services not allowed in this PLMN */
    CAUSE_GPRS_NOT_ALLOWED_IN_PLMN           = 0x0e,
    /** MSC temporarily not reachable */
    CAUSE_GMM_MSC_TEMP_NOT_REACHABLE         = 0x10,
    /** No PDP context activated */
    CAUSE_NO_PDP_CONTEXT_ACTIVATED           = 0x28,
    /** GMM no cause */
    CAUSE_GMM_NO_CAUSE                       = 0xffff,
    /* END GMM CAUSES */

    /* GSM Cause Values Phase 2 */
    /** unallocated number */
    CAUSE_UNASSIGNED_NO             =   1,
    /** No route to destination */
    CAUSE_NO_ROUTE_TO_DEST          =   3,
    /** Channel unacceptable        */
    CAUSE_CHAN_UNACCEPTABLE         =   6,
    /** Operator determined barring */
    CAUSE_OPER_DETERM_BARRING       =   8,
    /** Normal call clearing        */
    CAUSE_NORMAL_CLEARING           =   16,
    /** User busy   */
    CAUSE_USER_BUSY                 =   17,
    /** No user responding */
    CAUSE_NO_USER_RESPONDING        =   18,
    /** User alerting, no answer */
    CAUSE_ALERTING_NO_ANSWER        =   19,
    /** Call rejected */
    CAUSE_CALL_REJECTED             =   21,
    /** Number changed */
    CAUSE_NUMBER_CHANGED            =   22,
    /** Pre-emption */
    CAUSE_PREEMPTION                =   25,
    /** Non selected user clearing  */
    CAUSE_NONSEL_USER_CLRING        =   26,
    /** Destination out of order     */
    CAUSE_DEST_OUT_OF_ORDER         =   27,
    /** Invalid number format (in complete number) */
    CAUSE_INVALID_NO_FORMAT         =   28,
    /** Facility rejected */
    CAUSE_FACILITY_REJECTED         =   29,
    /** Response to STATUS ENQUIRY  */
    CAUSE_RSP_TO_STATUS_ENQ         =   30,
    /** Normal, unspecified */
    CAUSE_NORMAL_UNSPECIFIED        =   31,
    /** No circuit/channel available */
    CAUSE_NO_CIRC_CHAN_AV           =   34,
    /** Network out of order */
    CAUSE_NET_OUT_OF_ORDER          =   38,
    /** Temporary failure */
    CAUSE_TEMP_FAILURE              =   41,
    /** Switching equipment congestion */
    CAUSE_SWITCH_CONGESTION         =   42,
    /** Access information discarded */
    CAUSE_ACC_INFO_DISCARDED        =   43,
    /** requested circuit/channel not available */
    CAUSE_REQ_CIRC_CHAN_UNAV        =   44,
    /** Resources unavailable, unspecified */
    CAUSE_RESOURCES_UNAV            =   47,
    /** Quality of service unavailable */
    CAUSE_QOS_UNAV                  =   49,
    /** Requested facility not subscribed */
    CAUSE_REQ_FAC_NOT_SUBSCR        =   50,
    /** Incoming calls barred within the CUG */
    CAUSE_CUG_INCOMING_BARRED       =   55,
    /** Bearer capability not authorized */
    CAUSE_BEAR_CAP_NOT_AUTH         =   57,
    /** Bearer capability not presently available */
    CAUSE_BEAR_CAP_UNAV             =   58,
    /** Service or option not available, unspecified */
    CAUSE_SERV_OPT_UNAV             =   63,
    /** Bearer service not implemented */
    CAUSE_BEAR_SVC_NOT_IMPL         =   65,
    /** ACM equal to or greater than ACMmax */
    CAUSE_ACM_EQ_OR_GT_ACMMAX       =   68, /* FR9608-0481 */
    /** Requested facility not implemented */
    CAUSE_REQ_FACIL_NOT_IMPL        =   69,
    /** Only restricted digital information bearer
     * capability is available
     */
    CAUSE_ONLY_RESTRIC_DIG_AV       =   70,
    /** Service or option not implemented, unspecified */
    CAUSE_SVC_OPT_NOT_IMPL          =   79,
    /** Invalid transaction identifier value */
    CAUSE_INVALID_TI                =   81,
    /** User not member of CUG  */
    CAUSE_USER_NOT_IN_CUG           =   87,
    /** Incompatible destination   */
    CAUSE_INCOMPAT_DEST             =   88,
    /** Invalid transit network selection */
    CAUSE_INVALID_TRANSIT_NET       =   91,
    /** Semantically incorrect message */
    CAUSE_INVALID_MSG_SEMANTIC      =   95,
    /** Invalid mandatory information */
    CAUSE_MAND_IE_ERROR             =   96,
    /** Message type non-existent or not implemented */
    CAUSE_MSG_NONEXISTENT           =   97,
    /** Message type not compatible with protocol state  */
    CAUSE_MSG_GEN_ERROR             =   98,
    /** Information element non-existent or not implemented */
    CAUSE_IE_NONEXISTENT            =   99,
    /** Conditional IE error  */
    CAUSE_INVALID_CONDITION_IE      =   100,
    /** Message not compatible with protocol state */
    CAUSE_MSG_INCOMPAT_STATE        =   101,
    /** Recovery on timer expiry   */
    CAUSE_RECOV_ON_TIMER_EXP        =   102,
    /** Protocol error, unspecified */
    CAUSE_PROTOCOL_ERROR            =   111,
    /** Interworking, unspecified  */
    CAUSE_INTERWORKING              =   127,

    /** GSM Cause Values Phase 1 */
    CAUSE_PH1_INVALID_IE_CONTENTS   =   100,
    /*
    reject causes (10.5.3.6) sent in CM Service Reject, Abort, MM-Status
    and Location Updating Reject messages to MM from the network
    and some Attach Rej cause , and some RAU Rej
    */
    /** Unallocated TMSI */
    CAUSE_UNALLOCATED_TMSI          =   1,
    /** IMSI unknown in HLR */
    CAUSE_IMSI_UNKNOWN_IN_HLR,
    /** Illegal MS */
    CAUSE_ILLEGAL_MS,
    /** IMSI unknown in VLR */
    CAUSE_IMSI_UNKNOWN_IN_VLR,
    /** IMEI not accepted */
    CAUSE_IMEI_NOT_ACCEPTED,
    /** Illegal ME */
    CAUSE_ILLEGAL_ME,
    /** PLMN not allowed */
    CAUSE_PLMN_NOT_ALLOWED          =   11,
    /** Location Area not allowed */
    CAUSE_LA_NOT_ALLOWED,
    /** Roaming not allowed in this location area */
    CAUSE_NATIONAL_ROAMING_NOT_ALLOWED,
    /** No Suitable Cells In Location Area */
    CAUSE_NO_SUITABLE_CELLS_IN_LA   =   15,
    /** Network failure */
    CAUSE_NETWORK_FAILURE           =   17,
    /** MAC failure */
    CAUSE_MAC_FAILURE               =   20,
    /** Synch failure */
    CAUSE_SYNCH_FAILURE             =   21,
    /** Congestion */
    CAUSE_CONGESTION                =   22,
    /** GSM authentication unacceptable */
    CAUSE_GSM_AUTH_UNACCEPTABLE     =   23,
    /** Service option not supported */
    CAUSE_SERV_OPT_NOT_SUPPORTED    =   32,
    /** Requested service option not subscribed */
    CAUSE_SERV_OPT_NOT_SUBSCRIBED,
    /** Service option temporarily out of order */
    CAUSE_SERV_OPT_TEMP_OUT_OF_ORDER,
    /** Call cannot be identified */
    CAUSE_CALL_CANNOT_BE_IDENTIFIED =   38,
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
    /** Semantically incorrect message */
    CAUSE_SEMANTICALLY_INCORRECT_MSG = 0x5F,    /*  95 */
    /** Invalid mandatory information */
    CAUSE_INVALID_MAND_INFORMATION = 0x60,      /*  96 */
    /** Message type non-existent or not implemented */
    CAUSE_MSG_TYPE_NONEXIST_OR_NOT_IMP = 0x61,  /*  97 */
    /** Message type not compatible with the protocol state */
    CAUSE_MSG_TYPE_INCOMPAT_WITH_STATE = 0x62,  /*  98 */
    /** Information element non-existent or not implemented */
    CAUSE_IE_NONEXIST_OR_NOT_IMP = 0x63,        /*  99 */
    /** Conditional IE error */
    CAUSE_CONDITIONAL_IE_ERROR = 0x64,          /* 100 */
    /** Message not compatible with the protocol state */
    CAUSE_MSG_INCOMPAT_WITH_STATE = 0x65,       /* 101 */
    /** Protocol error, unspecified */
    CAUSE_PROTOCOL_ERROR_UNSPEC = 0x6F,         /* 111 */
#endif /* (UPGRADE_3G_FDD) && (UPGRADE_REL6) */
    /** Retry upon entry into a new cell */
    CAUSE_RETRY_IN_NEW_CELL_LOW     =   192,
    /** Retry upon entry into a new cell */
    CAUSE_RETRY_IN_NEW_CELL_HIGH    =   207,

    /*
    RR cause values
    */
    /** Normal event */
    RRC_NORMAL_EVENT                =   0,
    /** Abnormal release, unspecified */
    RRC_ABNORMAL_REL_UNSPECIFIED    =   1,
    /** Abnormal release, channel unacceptable */
    RRC_ABNORMAL_REL_CHAN_UNACCEPT  =   2,
    /** Abnormal release, timer expired */
    RRC_ABNORMAL_REL_TIMER_EXPIRY   =   3,
    /** Abnormal release, no activity on the radio path */
    RRC_ABNORMAL_REL_NO_RF_ACTIVITY =   4,
    /** Preemptive release */
    RRC_PREEMPTIVE_RELEASE          =   5,
    /** Handover impossible, timing advance out of range */
    RRC_TIMING_ADVANCE_OUT_OF_RANGE =   8,
    /** Channel mode unacceptable */
    RRC_CHAN_MODE_UNACCEPTABLE      =   9,
    /**Frequency not implemented */
    RRC_FREQUENCEY_NOT_IMPLEMENTED  =   10,
    /** Call already cleared */
    RRC_CALL_ALREADY_CLEARED        =   65,
    /** Semantically incorrect message */
    RRC_SEMANTICALLY_INCORRECT_MSG  =   95,
    /** Invalid mandatory information */
    RRC_MANDATORY_IE_ERROR          =   96,
    /** Message type non-existent or not implemented */
    RRC_MSG_NOT_IMPLEMENTED         =   97,
    /** Message not compatible with state */
    RRC_MSG_NOT_COMPAT_WITH_STATE   =   98,
    /** Conditional IE error */
    RRC_CONDITIONAL_IE_ERROR        =   100,
    /**No cell allocation available */
    RRC_NO_CA_AVAILABLE             =   101,
    /** No cell allocation available */
    RRC_PROTOCOL_ERROR_UNSPECIFIED  =   111,

    /* Internal Cause Values */

    /*
    SMRL causes not covered by 04.08 cause values
    */
    /** The PLMN rejects the short message TPDU due to barring of the MS */
    CAUSE_CALL_BARRED               =   10,
    /** Call reserved */
    CAUSE_RESERVED                  =   11,
    /** No memory capacity available to store the message */
    CAUSE_MEMORY_EXCEEDED           =   22,
    /** Unidentified subscriber */
    CAUSE_UNIDENTIFIED_SUBSCRIBER   =   28,
    /** Unknown subscriber */
    CAUSE_UNKNOWN_SUBSCRIBER        =   30,
    /** Invalid SM transfer refference */
    CAUSE_INVALID_SM_TRANSFER_REF   =   81,

    /** Corrupt Message Cause Values */
    CAUSE_INVALID_IE_CONTENTS       =   0x1000,

    /*
    cause values in a RR_REL_IND - these form part of this set because MM
    has to relay these causes to CM in addition to air interface causes
    */
    /** Normal release */
    NORMAL_RELEASE                  =   0x3100,  /* RR is 1st layer in L3 */
    /** The answer to random access is an IMMEDIATE ASSIGNMENT REJECT message */
    IMMEDIATE_ASSIGN_REJECT,
    /** T3122 is still running */
    T3122_STILL_RUNNING,
    /** Random access failure  */
    RACH_FAIL,
    /** Cell selection in progress */
    CELL_SELECTION_IN_PROGRESS,
    /** Establishment barred */
    REESTABLISHMENT_BARRED,
    /** Access class barred */
    ACCESS_CLASS_BARRED,
    /** Responding to paging */
    RESPONDING_TO_PAGING,
    /**Failure in lower layer */
    LOWER_LAYER_FAILURE,
    /** Abnormal release */
    ABNORMAL_RELEASE,
    /** LAC has changed */
    LOCATION_AREA_CODE_HAS_CHANGED,
    /** CS not available */
    CIRCUIT_SWITCHED_NOT_AVAILABLE,
    /** T309 has expired */
    CCO_T309_EXPIRED,

    /* MM causes */
    /** Authentication failure */
    CAUSE_MM_AUTH_FAILURE           =   0x3200,  /* MM is 2nd layer in L3 */
    /** MM connection has failed */
    CAUSE_MM_CONNECTION_FAIL,
    /** Too many connections */
    CAUSE_MAX_NUM_CM_CONNS_EXCEEDED,
    /**No CS service */
    CAUSE_MM_NO_CS_SERVICE,
    /** MM not registered */
    CAUSE_MM_NOT_REGISTERED,
    /** Cell selection */
    CAUSE_MM_CELL_SELECTION,
    /** SIM is removed */
    CAUSE_MM_SIM_REMOVED,
    /** MM is detachin */
    CAUSE_MM_DETACHING,
    /** incompatible MM state */
    CAUSE_INCOMPATIBLE_MM_STATE,

    /* CC-AL causes */
    /** CC-AL  no cause */
    CAUSE_CC_NO_CAUSE               =   0x3310,  /* CM is 3rd layer in L3 */
    /** Insufficient tasks */
    CAUSE_INSUFFICIENT_TASKS,

    /* SM-CM causes */
    /** CP-ACK not received */
    CAUSE_CP_ACK_NOT_RECEIVED       =   0x3320,
    /** Service out of order */
    CAUSE_CP_SERVICE_OPT_TEMP_OUT_OF_ORDER,
    /** CP allocation has failed */
    CAUSE_CP_ENTITY_ALLOC_FAIL,
    /** CP no error */
    CAUSE_CP_NO_ERROR,

    /* SM-RL causes */
    /** SMRL timer expiry signifying failure to receive RP_ACK/RP_ERROR response */
    CAUSE_RP_TIMEOUT_ERROR          =   0x4000,  /* RL is Layer 4 */
    /** Memory allocation error */
    CAUSE_RP_ENTITY_ALLOC_FAIL,
    /** No free MO entities */
    CAUSE_RP_ENTITY_NONE_FREE,
    /** RP no error */
    CAUSE_RP_NO_ERROR,

    /* PPP cause values
     * Note previously contained in ppcause.h
     * ppp doesn't really fit into layers as above */
    /** Start of PPP cause values */
    PPP_CAUSE_BASE = 0x5000,
    /** Cause not set */
    PPP_CAUSE_NOT_SET = PPP_CAUSE_BASE,
    /** Normal termination due to hang up */
    PPP_CAUSE_NORMAL_TERMINATION,
    /** Vague ncp-close cause */
    PPP_CAUSE_NCP_CLOSE = PPP_CAUSE_NORMAL_TERMINATION,
    /** Error in loopback */
    PPP_CAUSE_LOOPBACK_ERROR,
    /** Unacceptable MRU value */
    PPP_CAUSE_PEER_REFUSES_OUR_MRU,
    /** Peer rejects our ACCM */
    PPP_CAUSE_PEER_REFUSES_OUR_ACCM,
    /** Peer rejects our IP address */
    PPP_CAUSE_PEER_REFUSES_OUR_IP_ADDR,
    /** Peer re-requests CHAP */
    PPP_CAUSE_PEER_REREQUESTED_CHAP,
    /** Opened state in LCP not reached yet this could be a
     * failure during re-negotiation
     */
    PPP_CAUSE_LCP_REQ_NEGOTIATION_TIMEOUT,
    /** LCP terminate negotiation timeout */
    PPP_CAUSE_LCP_TERM_NEGOTIATION_TIMEOUT,
    /** Opened state in IPCP not reached yet this could be a
     * failure during re-negotiation
     */
    PPP_CAUSE_IPCP_NEGOTIATION_TIMEOUT,
    /** PAP closed */
    PPP_CAUSE_PAP_CLOSE,
    /** CHAP closed */
    PPP_CAUSE_CHAP_CLOSE,
    /** Reject because of bad code or protocol */
    PPP_CAUSE_BAD_CODE_OR_PROTOCOL_REJ,
    /** Other side has not sent us any echo replies for a while */
    PPP_CAUSE_NO_ECHO_REPLY,
    /** SM parameters (for example IP address) have changed */
    PPP_CAUSE_CANT_MODIFY_ADDRESS,
    /** Too many rejects */
    PPP_CAUSE_TOO_MANY_RXJS,
    /* 0206-18266: new cause values for PDP context activation failure */
    /** Invalid Dialing string length */
    PPP_CAUSE_INVALID_DIALSTRING_LENGTH,
    /** Invalid PDP type */
    PPP_CAUSE_INVALID_PDP_TYPE,
    /** Invalid character in address string */
    PPP_CAUSE_INVALID_CHAR_IN_ADDRESS_STRING,
    /** Out of order address element */
    PPP_CAUSE_OOR_ADDRESS_ELEMENT,
    /** The length of address is invalid */
    PPP_CAUSE_INVALID_ADDRESS_LENGTH,
    /** Failure to obtain NSAPI */
    PPP_CAUSE_NO_FREE_NSAPIS,
    /** GPRS service not available */
    PPP_CAUSE_GPRS_SERVICE_NOT_AVAILABLE,
    /** Powering down */
    PPP_CAUSE_POWERING_DOWN,
    /** job100813 FDN checking of GPRS activation */
    PPP_CAUSE_FDN_FAILURE,
    /** GPRS Call Control - Barred by SIM */
    PPP_CAUSE_CONTEXT_ACT_BARRED_BY_SIM,
    /** APN is not part of the ACL */
    PPP_CAUSE_RESTRICTED_APN_DESTINATION,
    /** Invalid Pdp context */
    PPP_CAUSE_BAD_CONTEXT_PARAMS,
    /** PPP already exist */
    PPP_CAUSE_ALREADY_ACTIVE,
    /** No memory allocated for the entity */
    PPP_CAUSE_NO_MEMORY_FOR_ENTITY,
    /** IPV6CP negotiation timeout */
    PPP_CAUSE_IPV6CP_NEGOTIATION_TIMEOUT,
    /** IPCP not supported with IPV6CP context */
    PPP_CAUSE_IPCP_NOT_SUPPORTED_WITH_IPV6CP_CONTEXT,
    /** IPV6CP not supported with IPCP context */
    PPP_CAUSE_IPV6CP_NOT_SUPPORTED_WITH_IPCP_CONTEXT,
    /** All the NCPS are not active */
    PPP_CAUSE_NO_AVAILABLE_NCPS,
#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_MBMS)
#  if defined (UPGRADE_3G_TDD128)
    /** MBMS session stop*/
    PPP_CAUSE_MBMS_NETWORK_NORMAL_RELEASE,
    /** No free SAPI is allocated for MBMS session */
    PPP_CAUSE_MBMS_NO_FREE_LSAPI,
    /** Resource conflict */
    PPP_CAUSE_MBMS_RESOURCE_CONFLICT,
    /** Cell not supported */
    PPP_CAUSE_MBMS_CELL_NOT_SUPPORTED,
    /** out of MBMS service coverage in RAN */
    PPP_CAUSE_MBMS_OUT_OF_MBMS_SERVICE_COVERAGE,
    /** Network release MBMS session since abnormal reason */
    PPP_CAUSE_MBMS_NETWORK_ABNORMAL_RELEASE,
    /** UE normal release */
    PPP_CAUSE_MBMS_LOCAL_NORMAL_RELEASE,
    /** Network switch */
    PPP_CAUSE_MBMS_NETWORK_SWITCH,
    /** MBMS Abnormal release, unspecified */
    PPP_CAUSE_MBMS_ABNORMAL_REL_UNSPECIFIED,
#  endif /* (UPGRADE_3G) */
# endif /* (UPGRADE_3G_MBMS) */
#endif /* (UPGRADE_3G_TDD128) */
    /* END PPP CAUSES */

#if defined (UPGRADE_NASMDL2)
    /** Generic PSD cause values */
    PSD_CAUSE_BASE = 0x6000,
    /** Cause not set */
    PSD_CAUSE_NOT_SET = PSD_CAUSE_BASE,
    /** Normal termination due to hang up */
    PSD_CAUSE_NORMAL_TERMINATION,
    /** PSD mode change not permitted with current usermode settings */
    PSD_CAUSE_PSD_MODE_NOT_POSSIBLE,
    /** SM parameters (for example IP address) have changed */
    PSD_CAUSE_CANT_MODIFY_ADDRESS,
    /** Invalid PDP type */
    PSD_CAUSE_INVALID_PDP_TYPE,
    /** The length of address is invalid */
    PSD_CAUSE_INVALID_ADDRESS_LENGTH,
    /** The connection type is invalid */
    PSD_CAUSE_INVALID_CONN_TYPE,
    /** Failure to obtain PSD Bearer ID (NSAPI for 2G/3G, EPS Bearer ID for LTE) */
    PSD_CAUSE_NO_FREE_PSD_BEARER_IDS,
    /** Failure to obtain PTI */
    PSD_CAUSE_NO_FREE_PTIS,
    /** PSD service not available */
    PSD_CAUSE_PSD_SERVICE_NOT_AVAILABLE,
    /** Powering down */
    PSD_CAUSE_POWERING_DOWN,
    /** FDN checking of PSD activation failed*/
    PSD_CAUSE_FDN_FAILURE,
    /** PSD Call Control (SIM Toolkit) - Barred by SIM */
    PSD_CAUSE_CONTEXT_ACT_BARRED_BY_SIM,
    /** APN is not part of the ACL */
    PSD_CAUSE_RESTRICTED_APN_DESTINATION,
    /** Invalid PSD Connection activation parameters */
    PSD_CAUSE_BAD_PSD_CONN_PARAMS,
    /** PSD Connection Already active (Conn ID) */
    PSD_CAUSE_ALREADY_ACTIVE,
    /** ATCI as unable to open a data connection when requested to do so by ABPD module */
    PSD_CAUSE_UNABLE_TO_OPEN_DATA_CONN,
    /** The username/password combination are incorrect compared to the ones used for the EPS connection during attach procedure */
    PSD_CAUSE_INCORRECT_USERNAME_PASSWD,
    /** Failed locally - invalid PSD bearer id */
    PSD_CAUSE_LOCAL_FAIL_INV_PSD_BEARER_ID,
    /** Failed locally - invalid PTI */
    PSD_CAUSE_LOCAL_FAIL_INV_PTI,
    /** Failed locally - invalid linked PSD bearer id */
    PSD_CAUSE_LOCAL_FAIL_INV_LINKED_PSD_BEARER_ID,
    /** Failed locally - incompatible state */
    PSD_CAUSE_LOCAL_FAIL_INCOMPAT_STATE,
    /** Failed locally - TFT error */
    PSD_CAUSE_LOCAL_FAIL_TFT_ERROR,
#endif

    /** value used if cause is passed to AL and nothing is relevant */
    CAUSE_NOT_APPLICABLE            =   0x7000,

#ifdef FEA_NFM
    /* For NFM feature, should indicate NFM error and left time value to AP.
     * MD returns cause code with left time value add to 0x8000, AP shall minus 0x8000 for left time value.
     * The max vaule of time left is 15360.
     */
    CAUSE_NFM_ERROR = 0x8000,
    CAUSE_NFM_ERROR_END = 0xbc00,
#endif
    /** No more causes. */
    noMoreGsmCauses
}
GsmCause;

#endif
/* END OF FILE */































