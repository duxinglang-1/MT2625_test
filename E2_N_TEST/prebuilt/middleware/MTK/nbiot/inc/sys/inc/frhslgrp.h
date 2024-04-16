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

#ifndef FRHSLGRP_H_
#define FRHSLGRP_H_

/** Filter ID DESCRIPTION **/

/* ---------------------------------------------------------- */
/*  MODULE ID: bit[15:8]  | PRIORITY LEVEL: bit[7:0],bitmap   */
/* ---------------------------------------------------------- */

/** Group definition (Modules) */
#define FR_HSL_FILTER_MODULE_BIT            (8)

#define FR_HSL_FILTER_GROUP_MASK            0xFF00
#define FR_HSL_FILTER_SUB_GROUP_MASK        0x00FF

/** Macro used to obtain a group id
 * \param Int8 gROUPnB Group number (0 to 255)
 */
#define M_FrHslGroupId(gROUPnB) ((gROUPnB) << FR_HSL_FILTER_MODULE_BIT)

/** Macro used to define a priority
 * \param Int8 vALUE Priority (0 to 7)
 */
#define M_FrHslPriorityDefine(vALUE) (1 << vALUE)

/** Macro used to define a group
 * \param HslModule_E bASE Base id of the module the group belongs to
 * \param Int8 oFFSET Offset from base
 */
#define M_FrHslGroupDefine(bASE, oFFSET) ( (Int16)(bASE) + (Int16)(M_FrHslGroupId( (oFFSET) )) )

/** Macro used to define a sub-group
 * \param Int16 gROUP Group which the sub-group belongs to
 * \param HslPriority_E pRIORITY Group priority (used for filtering and/or QoS )
 */
#define M_FrHslSubGroupDefine(gROUP, pRIORITY) ( (gROUP) | (pRIORITY) )

/** Macro used to provide a mask to filter priority given and below.
 * \param HslPriority_E HSL Priority num from HslPriority_0 to HslPriority_7
 */
#define M_FrHslFilterPriorityMaskFromBitmap(pRIORITY) (Int16)( ((pRIORITY) << 1) - 1 )


/** Macro used to provide a mask to filter priority given and below.
 * (Int32) casting added to prevent warnings. Macro warned if input value was 222.
 * \param Int16 Priority num from 0 to 7
 * NOTE! It seems that input value can be much bigger than 7.
 */
#define M_FrHslFilterPriorityMaskFromNum(pRIORITY) (Int16)( (Int32)( 1 << ( ((pRIORITY) + 1) & 0x1F /* Avoid warning if priority is > 15 */ ) ) - 1 )

/** Macro used to know if a log point group is in the special filter group range (HSL_FILTER_*)
 * \return Boolean
 */
#define M_FrHslIsSpecialFilterGrp(gROUP)                                                \
    (                                                                                   \
        ( ((gROUP) & FR_HSL_FILTER_GROUP_MASK) >= HSL_FILTER_PRIORITY_0) &&             \
        ( ((gROUP) & FR_HSL_FILTER_GROUP_MASK) <= HSL_FILTER_PRIORITY_7)                \
    )

/** Macro used to retrieve priority number from the special filter group range (HSL_FILTER_*)
 * \return Boolean
 */
#define M_FrHslPriorityNumFromSpecialFilterGrp(gROUP) ( ((gROUP) - HSL_FILTER) >> FR_HSL_FILTER_MODULE_BIT )


/** Default HSL filtering */
#if !defined(HSL_FILTER_GLOBAL_PRIORITY)
# define HSL_FILTER_GLOBAL_PRIORITY HslPriority_3
#endif

/**
 * \defgroup HSL Priorities
 */

/** Sub-group definition (Priority) */
typedef enum HslPriorityTag
{
    HslPriority_0 = M_FrHslPriorityDefine(0),   /* the highest priority for filter: always on */
    HslPriority_1 = M_FrHslPriorityDefine(1),
    HslPriority_2 = M_FrHslPriorityDefine(2),
    HslPriority_3 = M_FrHslPriorityDefine(3),
    HslPriority_4 = M_FrHslPriorityDefine(4),
    HslPriority_5 = M_FrHslPriorityDefine(5),
    HslPriority_6 = M_FrHslPriorityDefine(6),
    HslPriority_7 = M_FrHslPriorityDefine(7)    /* the lowest priority for filter */
} HslPriority_E;

/**@}*//* HSL Priorities */

/**
 * \defgroup HSL Groups
 */

/** Modules: Group bases **/
typedef enum HslModuleTag
{
    /* 0 is used for legacy log points */
    HSL_LEGACY = M_FrHslGroupId(0), //!< HSL_LEGACY

    /* 1-9 is reserved for HSL Filtering */
    HSL_FILTER = M_FrHslGroupId(1), //!< HSL_FILTER

    /* 10-63 is reserved for GGE */
    HSL_GGE = M_FrHslGroupId(10), //!< HSL_GGE

    /* 64-127 is reserved for TDD */
    HSL_TL1 = M_FrHslGroupId(64), //!< HSL_TL1
    HSL_TPH = M_FrHslGroupId(80), //!< HSL_TPH

    /* 128-181 is reserved for FDD */
    HSL_WL1 = M_FrHslGroupId(128), //!< HSL_WL1
    HSL_UPH = M_FrHslGroupId(150), //!< HSL_UPH

    /* 182-191 is reserved for common FDD/TDD */
    HSL_U1 = M_FrHslGroupId(182), //!< HSL_UL1
    HSL_XPH = M_FrHslGroupId(184), //!< HSL_XPH

    /* 192-199 is reserved for SYS (OS/Driver) */
    HSL_SYS = M_FrHslGroupId(192), //!< HSL_SYS

    /* 200-220 is reserved for Audio */
    HSL_AUD = M_FrHslGroupId(200), //!< HSL_AUD

    /* 221-245 NB-IOT L1 */
    HSL_N1 = M_FrHslGroupId(221), //!< HSL_N1

    /* 246-400 is reserved NAS */
    /* 246-259 is reserved Uart Connectivity Layer */
    HSL_CL = M_FrHslGroupId(246), //!< HSL_CL

//!< HSL_FILTER

} HslModule_E;


/**
 * \defgroup HSL_LEGACY_GROUP
 * \brief Special group for legacy support
 * Usage is to redirect values of log points that do not follow the standard group|subgroup definition
 * in that group.
 * @{
 */

/** HSL legacy group */
#define     HSL_LEGACY_LOG                      M_FrHslGroupDefine(HSL_LEGACY, 0x00)
/** Sub-group for mdhi3g.h */
#define     HSL_LEGAGY_LOG_3G                   M_FrHslSubGroupDefine(HSL_LEGACY_LOG, HslPriority_0)

/**@}*//* HSL_LEGACY_GROUP */

/**
 * \defgroup HSL_FILTER_GROUP
 * \brief Special group for filtering.
 * When a group is externally defined, for instance 'HSL_xxx_yyy_PRIO', the group value has to be one
 * of the following option.
 * Redirection should NOT be used if ENABLE_HSL_FILTER is not defined.
 * @{
 */

/** HSL Group Filter for priority 0 */
#define     HSL_FILTER_PRIORITY_0               M_FrHslGroupDefine(HSL_FILTER, 0x00)

/** HSL Group Filter for priority 1 and lower */
#define     HSL_FILTER_PRIORITY_1               M_FrHslGroupDefine(HSL_FILTER, 0x01)

/** HSL Group Filter for priority 2 and lower */
#define     HSL_FILTER_PRIORITY_2               M_FrHslGroupDefine(HSL_FILTER, 0x02)

/** HSL Group Filter for priority 3 and lower */
#define     HSL_FILTER_PRIORITY_3               M_FrHslGroupDefine(HSL_FILTER, 0x03)

/** HSL Group Filter for priority 4 and lower */
#define     HSL_FILTER_PRIORITY_4               M_FrHslGroupDefine(HSL_FILTER, 0x04)

/** HSL Group Filter for priority 5 and lower */
#define     HSL_FILTER_PRIORITY_5               M_FrHslGroupDefine(HSL_FILTER, 0x05)

/** HSL Group Filter for priority 6 and lower */
#define     HSL_FILTER_PRIORITY_6               M_FrHslGroupDefine(HSL_FILTER, 0x06)

/** HSL Group Filter for priority 7 and lower */
#define     HSL_FILTER_PRIORITY_7               M_FrHslGroupDefine(HSL_FILTER, 0x07)

/**@}*//* HSL_FILTER_GROUP */

/**
 * \defgroup HSL_SYS_GROUP
 * @{
 */

/** Global system logging */
#ifndef     HSL_SYS_LOG
# define    HSL_SYS_LOG                         M_FrHslGroupDefine(HSL_SYS, 0x01)
#endif
#if !defined (NBIOT_TRACE_FILTER)
#define     HSL_SYS_LOG_ERROR                   M_FrHslSubGroupDefine(HSL_SYS_LOG, HslPriority_0)
#define     HSL_SYS_LOG_WARNING                 M_FrHslSubGroupDefine(HSL_SYS_LOG, HslPriority_1)
# if defined(MTK_NBIOT_COSIM_BUILD)
#define     HSL_SYS_LOG_BASIC                   M_FrHslSubGroupDefine(HSL_SYS_LOG, HslPriority_4)
#define     HSL_SYS_LOG_EXTENDED                M_FrHslSubGroupDefine(HSL_SYS_LOG, HslPriority_5)
#define     HSL_SYS_LOG_DEBUG                   M_FrHslSubGroupDefine(HSL_SYS_LOG, HslPriority_6)
# else
#define     HSL_SYS_LOG_BASIC                   M_FrHslSubGroupDefine(HSL_SYS_LOG, HslPriority_2)
#define     HSL_SYS_LOG_EXTENDED                M_FrHslSubGroupDefine(HSL_SYS_LOG, HslPriority_4)
#define     HSL_SYS_LOG_DEBUG                   M_FrHslSubGroupDefine(HSL_SYS_LOG, HslPriority_5)
# endif
#endif

/** Usim */
#ifndef     HSL_SYS_USIM
# define    HSL_SYS_USIM                        M_FrHslGroupDefine(HSL_SYS, 0x02)
#endif
#define     HSL_SYS_USIM_DRV                    M_FrHslSubGroupDefine(HSL_SYS_USIM, HslPriority_1)

#ifndef     HSL_SYS_IRQ
# define    HSL_SYS_IRQ                         M_FrHslGroupDefine(HSL_SYS, 0x03)
#endif
#define     HSL_SYS_IRQ_ESSENTIAL               M_FrHslSubGroupDefine(HSL_SYS_IRQ, HslPriority_0)

/**@}*//* HSL_SYS_GROUP */

/**
 * \defgroup HSL_AUD_GROUP
 * @{
 */

/** Global audio logging */
#ifndef     HSL_AUD_LOG
# define    HSL_AUD_LOG                         M_FrHslGroupDefine(HSL_AUD, 0x01)
#endif

#ifndef     HSL_AUD_LOG_MAX
# define    HSL_AUD_LOG_MAX                     M_FrHslGroupDefine(HSL_AUD, 0x14)
#endif

/**@}*//* HSL_AUD_GROUP */

/**
 * \defgroup HSL_GGE_GROUP
 */

#if defined(ENABLE_NEW_MDFID)
/* For new mechanism */

#ifndef     LG_GGE_RX_AFC
# define    LG_GGE_RX_AFC                       M_FrHslGroupDefine(HSL_GGE, 0x01)
#endif
#define     LG_PP_AFC_STT                       M_FrHslSubGroupDefine(LG_GGE_RX_AFC, HslPriority_0)
#define     LG_PP_AFC_END                       M_FrHslSubGroupDefine(LG_GGE_RX_AFC, HslPriority_0)

#ifndef     LG_GGE_RX_MON
# define    LG_GGE_RX_MON                       M_FrHslGroupDefine(HSL_GGE, 0x02)
#endif
#define     LG_PP_MONITOR_STT                   M_FrHslSubGroupDefine(LG_GGE_RX_MON, HslPriority_0)
#define     LG_PP_MONITOR_END                   M_FrHslSubGroupDefine(LG_GGE_RX_MON, HslPriority_0)
#define     LG_MON_TDS_CONTENTS                 M_FrHslSubGroupDefine(LG_GGE_RX_MON, HslPriority_1)
#define     LG_T1_GENERIC_RX_MON_IQ             M_FrHslSubGroupDefine(LG_GGE_RX_MON, HslPriority_2)
#define     LG_RSSI_RES                         M_FrHslSubGroupDefine(LG_GGE_RX_MON, HslPriority_3)

#ifndef     LG_GGE_RX_PRP
# define    LG_GGE_RX_PRP                       M_FrHslGroupDefine(HSL_GGE, 0x03)
#endif
#define     LG_PP_POWERRAMP_STT                 M_FrHslSubGroupDefine(LG_GGE_RX_PRP, HslPriority_0)
#define     LG_PP_POWERRAMP_END                 M_FrHslSubGroupDefine(LG_GGE_RX_PRP, HslPriority_0)
#define     LG_PRP_TDS_CONTENTS                 M_FrHslSubGroupDefine(LG_GGE_RX_PRP, HslPriority_1)
#define     LG_PP_TX_GAIN_STT                   M_FrHslSubGroupDefine(LG_GGE_RX_PRP, HslPriority_2)
#define     LG_PP_TX_GAIN_END                   M_FrHslSubGroupDefine(LG_GGE_RX_PRP, HslPriority_3)

#ifndef     LG_GGE_RX_SYNC
# define    LG_GGE_RX_SYNC                      M_FrHslGroupDefine(HSL_GGE, 0x04)
#endif
#define     LG_PP_SYNTHCONST_STT                M_FrHslSubGroupDefine(LG_GGE_RX_SYNC, HslPriority_0)
#define     LG_PP_SYNTHCONST_END                M_FrHslSubGroupDefine(LG_GGE_RX_SYNC, HslPriority_0)

#ifndef     LG_GGE_RX_SETGAIN
# define    LG_GGE_RX_SETGAIN                   M_FrHslGroupDefine(HSL_GGE, 0x05)
#endif
#define     LG_PP_SETGAIN_STT                   M_FrHslSubGroupDefine(LG_GGE_RX_SETGAIN, HslPriority_0)
#define     LG_PP_SETGAIN_END                   M_FrHslSubGroupDefine(LG_GGE_RX_SETGAIN, HslPriority_0)
#define     LG_SETGAIN_TDS_CONTENTS             M_FrHslSubGroupDefine(LG_GGE_RX_SETGAIN, HslPriority_1)
#define     LG_SET_RADIO_GAIN                   M_FrHslSubGroupDefine(LG_GGE_RX_SETGAIN, HslPriority_2)

#ifndef     LG_GGE_RX_FASTAGC
# define    LG_GGE_RX_FASTAGC                   M_FrHslGroupDefine(HSL_GGE, 0x06)
#endif
#define     LG_PP_FASTAGC_STT                   M_FrHslSubGroupDefine(LG_GGE_RX_FASTAGC, HslPriority_0)
#define     LG_PP_FASTAGC_END                   M_FrHslSubGroupDefine(LG_GGE_RX_FASTAGC, HslPriority_0)
#define     LG_FASTAGC_TDS_CONTENTS             M_FrHslSubGroupDefine(LG_GGE_RX_FASTAGC, HslPriority_1)
#define     LG_CHAR                             M_FrHslSubGroupDefine(LG_GGE_RX_FASTAGC, HslPriority_3)
#define     LG_CALC_GAIN                        M_FrHslSubGroupDefine(LG_GGE_RX_FASTAGC, HslPriority_4)

#ifndef     LG_GGE_RX_PRIAGC
# define    LG_GGE_RX_PRIAGC                    M_FrHslGroupDefine(HSL_GGE, 0x07)
#endif
#define     LG_PP_PRIMEAGC_STT                  M_FrHslSubGroupDefine(LG_GGE_RX_PRIAGC, HslPriority_0)
#define     LG_PP_PRIMEAGC_END                  M_FrHslSubGroupDefine(LG_GGE_RX_PRIAGC, HslPriority_0)
#define     LG_PRIAGC_TDS_CONTENTS              M_FrHslSubGroupDefine(LG_GGE_RX_PRIAGC, HslPriority_1)
#define     LG_AGC_PRIMEVAL                     M_FrHslSubGroupDefine(LG_GGE_RX_PRIAGC, HslPriority_2)
#define     LG_CHAR_PRIAGC                      M_FrHslSubGroupDefine(LG_GGE_RX_PRIAGC, HslPriority_3)

#ifndef     LG_GGE_RX_RCSYN
# define     LG_GGE_RX_RCSYN                     M_FrHslGroupDefine(HSL_GGE, 0x08)
#endif
#define     LG_PP_RCSYNTH_STT                   M_FrHslSubGroupDefine(LG_GGE_RX_RCSYN, HslPriority_0)
#define     LG_PP_RCSYNTH_END                   M_FrHslSubGroupDefine(LG_GGE_RX_RCSYN, HslPriority_0)
#define     LG_SYNTH_DATA                       M_FrHslSubGroupDefine(LG_GGE_RX_RCSYN, HslPriority_1)

#ifndef     LG_GGE_RX_RFINIT
# define    LG_GGE_RX_RFINIT                    M_FrHslGroupDefine(HSL_GGE, 0x09)
#endif
#define     LG_PP_RFINIT_STT                    M_FrHslSubGroupDefine(LG_GGE_RX_RFINIT, HslPriority_0)
#define     LG_PP_RFINIT_END                    M_FrHslSubGroupDefine(LG_GGE_RX_RFINIT, HslPriority_0)

#ifndef     LG_GGE_RX_SNIFF
# define    LG_GGE_RX_SNIFF                     M_FrHslGroupDefine(HSL_GGE, 0x0A)
#endif
#define     LG_PP_SNIFF_STT                     M_FrHslSubGroupDefine(LG_GGE_RX_SNIFF, HslPriority_0)
#define     LG_PP_SNIFF_END                     M_FrHslSubGroupDefine(LG_GGE_RX_SNIFF, HslPriority_0)
#define     LG_SNIFF_TDS_CONTENTS               M_FrHslSubGroupDefine(LG_GGE_RX_SNIFF, HslPriority_1)
#define     LG_T1_GENERIC_RX_SNIFF_IQ           M_FrHslSubGroupDefine(LG_GGE_RX_SNIFF, HslPriority_2)

#ifndef     LG_GGE_RX_START
# define    LG_GGE_RX_START                     M_FrHslGroupDefine(HSL_GGE, 0x0B)
#endif
#define     LG_PP_STARTRX_STT                   M_FrHslSubGroupDefine(LG_GGE_RX_START, HslPriority_0)
#define     LG_PP_STARTRX_END                   M_FrHslSubGroupDefine(LG_GGE_RX_START, HslPriority_0)
#define     LG_RX_TDS_CONTENTS                  M_FrHslSubGroupDefine(LG_GGE_RX_START, HslPriority_1)

#ifndef     LG_GGE_RX_STOP
# define    LG_GGE_RX_STOP                      M_FrHslGroupDefine(HSL_GGE, 0x0C)
#endif
#define     LG_PP_STOPRX_STT                    M_FrHslSubGroupDefine(LG_GGE_RX_STOP, HslPriority_0)
#define     LG_PP_STOPRX_END                    M_FrHslSubGroupDefine(LG_GGE_RX_STOP, HslPriority_0)

#ifndef     LG_GGE_TX_START
# define    LG_GGE_TX_START                     M_FrHslGroupDefine(HSL_GGE, 0x0D)
#endif
#define     LG_PP_STARTTX_STT                   M_FrHslSubGroupDefine(LG_GGE_TX_START, HslPriority_0)
#define     LG_PP_STARTTX_END                   M_FrHslSubGroupDefine(LG_GGE_TX_START, HslPriority_0)
#define     LG_TX_TDS_CONTENTS                  M_FrHslSubGroupDefine(LG_GGE_TX_START, HslPriority_1)

#ifndef     LG_GGE_TX_STOP
# define    LG_GGE_TX_STOP                      M_FrHslGroupDefine(HSL_GGE, 0x0E)
#endif
#define     LG_PP_STOPTX_STT                    M_FrHslSubGroupDefine(LG_GGE_TX_STOP, HslPriority_0)
#define     LG_PP_STOPTX_END                    M_FrHslSubGroupDefine(LG_GGE_TX_STOP, HslPriority_0)

#ifndef     LG_GGE_MC_CIPHER
# define    LG_GGE_MC_CIPHER                    M_FrHslGroupDefine(HSL_GGE, 0x0F)
#endif
#define     LG_CIPLD_TDS_CONTENTS               M_FrHslSubGroupDefine(LG_GGE_MC_CIPHER, HslPriority_0)
#define     LG_CIPGO_TDS_CONTENTS               M_FrHslSubGroupDefine(LG_GGE_MC_CIPHER, HslPriority_1)

#ifndef     LG_GGE_MC_SDEC
# define    LG_GGE_MC_SDEC                      M_FrHslGroupDefine(HSL_GGE, 0x10)
#endif
#define     LG_PP_DECODESB_STT                  M_FrHslSubGroupDefine(LG_GGE_MC_SDEC, HslPriority_0)
#define     LG_PP_DECODESB_END                  M_FrHslSubGroupDefine(LG_GGE_MC_SDEC, HslPriority_0)
#define     LG_SDC_TDS_CONTENTS                 M_FrHslSubGroupDefine(LG_GGE_MC_SDEC, HslPriority_1)
#define     LG_T1_GENERIC_RX_SDEC_IQ            M_FrHslSubGroupDefine(LG_GGE_MC_SDEC, HslPriority_2)
#define     LG_SB_DEC_RES                       M_FrHslSubGroupDefine(LG_GGE_MC_SDEC, HslPriority_3)
#define     LG_SB_DEC_CNT                       M_FrHslSubGroupDefine(LG_GGE_MC_SDEC, HslPriority_3)

#ifndef     LG_GGE_MC_DEC
# define    LG_GGE_MC_DEC                       M_FrHslGroupDefine(HSL_GGE, 0x11)
#endif
#define     LG_PP_DECODE_STT                    M_FrHslSubGroupDefine(LG_GGE_MC_DEC, HslPriority_0)
#define     LG_PP_DECODE_END                    M_FrHslSubGroupDefine(LG_GGE_MC_DEC, HslPriority_0)
#define     LG_DEC_TDS_CONTENTS                 M_FrHslSubGroupDefine(LG_GGE_MC_DEC, HslPriority_1)
#define     LG_DEC_CHANNEL_TYPE                 M_FrHslSubGroupDefine(LG_GGE_MC_DEC, HslPriority_2)
#define     LG_NB_DEC_RES                       M_FrHslSubGroupDefine(LG_GGE_MC_DEC, HslPriority_3)
#define     LG_CC_DECODED_DATA                  M_FrHslSubGroupDefine(LG_GGE_MC_DEC, HslPriority_4)
#define     LG_CC_VITERBI_IN                    M_FrHslSubGroupDefine(LG_GGE_MC_DEC, HslPriority_5)
#define     LG_CC_VITERBI_OUT                   M_FrHslSubGroupDefine(LG_GGE_MC_DEC, HslPriority_6)

#ifndef     LG_GGE_MC_UDEC
# define    LG_GGE_MC_UDEC                      M_FrHslGroupDefine(HSL_GGE, 0x12)
#endif
#define     LG_PP_USFDECODE_STT                 M_FrHslSubGroupDefine(LG_GGE_MC_UDEC, HslPriority_0)
#define     LG_UDEC_TDS_CONTENTS                M_FrHslSubGroupDefine(LG_GGE_MC_UDEC, HslPriority_1)
#define     LG_UDEC_CHANNEL_TYPE                M_FrHslSubGroupDefine(LG_GGE_MC_UDEC, HslPriority_2)
#define     LG_CC_USF_STATE                     M_FrHslSubGroupDefine(LG_GGE_MC_UDEC, HslPriority_3)

#ifndef     LG_GGE_MC_ENC
# define    LG_GGE_MC_ENC                       M_FrHslGroupDefine(HSL_GGE, 0x13)
#endif
#define     LG_PP_ENCODE_STT                    M_FrHslSubGroupDefine(LG_GGE_MC_ENC, HslPriority_0)
#define     LG_PP_ENCODE_END                    M_FrHslSubGroupDefine(LG_GGE_MC_ENC, HslPriority_0)
#define     LG_ENC_TDS_CONTENTS                 M_FrHslSubGroupDefine(LG_GGE_MC_ENC, HslPriority_1)
#define     LG_ENC_CHANNEL_TYPE                 M_FrHslSubGroupDefine(LG_GGE_MC_ENC, HslPriority_2)
#define     LG_CC_ILEAVE                        M_FrHslSubGroupDefine(LG_GGE_MC_ENC, HslPriority_3)
#define     LG_PP_SPC_REQ                       M_FrHslSubGroupDefine(LG_GGE_MC_ENC, HslPriority_4)
#define     LG_PP_SPC_SILENCE                   M_FrHslSubGroupDefine(LG_GGE_MC_ENC, HslPriority_5)
#define     LG_CC_ENC_BUFY                      M_FrHslSubGroupDefine(LG_GGE_MC_ENC, HslPriority_6)

#ifndef     LG_GGE_MC_FMTNB
# define    LG_GGE_MC_FMTNB                     M_FrHslGroupDefine(HSL_GGE, 0x14)
#endif
#define     LG_PP_FORMATNB_STT                  M_FrHslSubGroupDefine(LG_GGE_MC_FMTNB, HslPriority_0)
#define     LG_PP_FORMATNB_END                  M_FrHslSubGroupDefine(LG_GGE_MC_FMTNB, HslPriority_0)
#define     LG_FMTNB_CHANNEL_TYPE               M_FrHslSubGroupDefine(LG_GGE_MC_FMTNB, HslPriority_1)
#define     LG_CC_ILEAVE_FMT                    M_FrHslSubGroupDefine(LG_GGE_MC_FMTNB, HslPriority_2)
#define     LG_CC_BURST                         M_FrHslSubGroupDefine(LG_GGE_MC_FMTNB, HslPriority_3)

#ifndef     LG_GGE_MC_FMTNBE
# define    LG_GGE_MC_FMTNBE                    M_FrHslGroupDefine(HSL_GGE, 0x15)
#endif
#define     LG_PP_FORMATNBE_STT                 M_FrHslSubGroupDefine(LG_GGE_MC_FMTNBE, HslPriority_0)
#define     LG_PP_FORMATNBE_END                 M_FrHslSubGroupDefine(LG_GGE_MC_FMTNBE, HslPriority_0)
#define     LG_FMTNBE_TDS_CONTENTS              M_FrHslSubGroupDefine(LG_GGE_MC_FMTNBE, HslPriority_1)
#define     LG_FMTNBE_CHANNEL_TYPE              M_FrHslSubGroupDefine(LG_GGE_MC_FMTNBE, HslPriority_2)
#define     LG_CHETDS_IL_HANDLE                 M_FrHslSubGroupDefine(LG_GGE_MC_FMTNBE, HslPriority_3)
#define     LG_CHETDS_TX_HANDLE                 M_FrHslSubGroupDefine(LG_GGE_MC_FMTNBE, HslPriority_4)

#ifndef     LG_GGE_MC_RACH
# define    LG_GGE_MC_RACH                      M_FrHslGroupDefine(HSL_GGE, 0x16)
#endif
#define     LG_PP_RACH_STT                      M_FrHslSubGroupDefine(LG_GGE_MC_RACH, HslPriority_0)
#define     LG_PP_RACH_END                      M_FrHslSubGroupDefine(LG_GGE_MC_RACH, HslPriority_0)
#define     LG_CHETDS_TX_HANDLE_RACH            M_FrHslSubGroupDefine(LG_GGE_MC_RACH, HslPriority_1)
#define     LG_RACH_CHANNEL_TYPE                M_FrHslSubGroupDefine(LG_GGE_MC_RACH, HslPriority_2)
#define     LG_RACH_PAYLOAD                     M_FrHslSubGroupDefine(LG_GGE_MC_RACH, HslPriority_3)
#define     LG_TX_BURST                         M_FrHslSubGroupDefine(LG_GGE_MC_RACH, HslPriority_4)

#ifndef     LG_GGE_MC_DECNB
# define    LG_GGE_MC_DECNB                     M_FrHslGroupDefine(HSL_GGE, 0x17)
#endif
#define     LG_PP_DECODENB_STT                  M_FrHslSubGroupDefine(LG_GGE_MC_DECNB, HslPriority_0)
#define     LG_PP_DECODENB_END                  M_FrHslSubGroupDefine(LG_GGE_MC_DECNB, HslPriority_0)
#define     LG_DECNB_TDS_CONTENTS               M_FrHslSubGroupDefine(LG_GGE_MC_DECNB, HslPriority_1)
#define     LG_CHDTDS_DI_HANDLE                 M_FrHslSubGroupDefine(LG_GGE_MC_DECNB, HslPriority_2)
#define     LG_DECNB_CHANNEL_TYPE               M_FrHslSubGroupDefine(LG_GGE_MC_DECNB, HslPriority_3)
#define     LG_CHDTDS_SD_BUFFER                 M_FrHslSubGroupDefine(LG_GGE_MC_DECNB, HslPriority_4)
#define     LG_CHDTDS_PL_BUFFER                 M_FrHslSubGroupDefine(LG_GGE_MC_DECNB, HslPriority_5)

#ifndef     LG_GGE_MC_DECNBE
# define    LG_GGE_MC_DECNBE                    M_FrHslGroupDefine(HSL_GGE, 0x18)
#endif
#define     LG_PP_DECODENBE_STT                 M_FrHslSubGroupDefine(LG_GGE_MC_DECNBE, HslPriority_0)
#define     LG_PP_DECODENBE_END                 M_FrHslSubGroupDefine(LG_GGE_MC_DECNBE, HslPriority_0)
#define     LG_DECNBE_TDS_CONTENTS              M_FrHslSubGroupDefine(LG_GGE_MC_DECNBE, HslPriority_1)
#define     LG_DECNBE_CHANNEL_TYPE              M_FrHslSubGroupDefine(LG_GGE_MC_DECNBE, HslPriority_2)

#ifndef     LG_GGE_MC_IRCNB
# define    LG_GGE_MC_IRCNB                     M_FrHslGroupDefine(HSL_GGE, 0x19)
#endif
#define     LG_PP_IRCOMBINENBE_STT              M_FrHslSubGroupDefine(LG_GGE_MC_IRCNB, HslPriority_0)
#define     LG_PP_IRCOMBINENBE_END              M_FrHslSubGroupDefine(LG_GGE_MC_IRCNB, HslPriority_0)
#define     LG_IRCNB_TDS_CONTENTS               M_FrHslSubGroupDefine(LG_GGE_MC_IRCNB, HslPriority_1)
#define     LG_IRCNB_CHANNEL_TYPE               M_FrHslSubGroupDefine(LG_GGE_MC_IRCNB, HslPriority_2)
#define     LG_CHITDS_SD_HANDLE                 M_FrHslSubGroupDefine(LG_GGE_MC_IRCNB, HslPriority_3)
#define     LG_CHITDS_SD_BUFFER                 M_FrHslSubGroupDefine(LG_GGE_MC_IRCNB, HslPriority_4)

#ifndef     LG_GGE_MC_SEGNB
# define    LG_GGE_MC_SEGNB                     M_FrHslGroupDefine(HSL_GGE, 0x1A)
#endif
#define     LG_PP_SEGMENTNBE_STT                M_FrHslSubGroupDefine(LG_GGE_MC_SEGNB, HslPriority_0)
#define     LG_PP_SEGMENTNBE_END                M_FrHslSubGroupDefine(LG_GGE_MC_SEGNB, HslPriority_0)
#define     LG_SEGNB_TDS_CONTENTS               M_FrHslSubGroupDefine(LG_GGE_MC_SEGNB, HslPriority_1)
#define     LG_SEGNB_CHANNEL_TYPE               M_FrHslSubGroupDefine(LG_GGE_MC_SEGNB, HslPriority_2)
#define     LG_CHGTDS_DI_HANDLE                 M_FrHslSubGroupDefine(LG_GGE_MC_SEGNB, HslPriority_3)
#define     LG_CHGTDS_DI_HANDLE_1               M_FrHslSubGroupDefine(LG_GGE_MC_SEGNB, HslPriority_4)
#define     LG_CHGTDS_DI_HANDLE_2               M_FrHslSubGroupDefine(LG_GGE_MC_SEGNB, HslPriority_5)
#define     LG_CHGTDS_SD_HANDLE_1               M_FrHslSubGroupDefine(LG_GGE_MC_SEGNB, HslPriority_6)
#define     LG_CHGTDS_SD_HANDLE_2               M_FrHslSubGroupDefine(LG_GGE_MC_SEGNB, HslPriority_7)

#ifndef     LG_GGE_MC_ENCNBE
# define    LG_GGE_MC_ENCNBE                    M_FrHslGroupDefine(HSL_GGE, 0x1B)
#endif
#define     LG_PP_ENCNBE_STT                    M_FrHslSubGroupDefine(LG_GGE_MC_ENCNBE, HslPriority_0)
#define     LG_PP_ENCNBE_END                    M_FrHslSubGroupDefine(LG_GGE_MC_ENCNBE, HslPriority_0)
#define     LG_ENCNBE_TDS_CONTENTS              M_FrHslSubGroupDefine(LG_GGE_MC_ENCNBE, HslPriority_1)
#define     LG_ENCNBE_CHANNEL_TYPE              M_FrHslSubGroupDefine(LG_GGE_MC_ENCNBE, HslPriority_2)
#define     LG_CHETDS_PL_BUFFER                 M_FrHslSubGroupDefine(LG_GGE_MC_ENCNBE, HslPriority_3)
#define     LG_CHETDS_IL_HANDLE_1               M_FrHslSubGroupDefine(LG_GGE_MC_ENCNBE, HslPriority_4)

#ifndef     LG_GGE_MC_AMR
# define    LG_GGE_MC_AMR                       M_FrHslGroupDefine(HSL_GGE, 0x1C)
#endif
#define     LG_NB_AMR_DEC_RES                   M_FrHslSubGroupDefine(LG_GGE_MC_AMR, HslPriority_0)
#define     LG_MC_AMR_TDS_CONTENTS              M_FrHslSubGroupDefine(LG_GGE_MC_AMR, HslPriority_1)
#define     LG_CC_UPLINK_RATE                   M_FrHslSubGroupDefine(LG_GGE_MC_AMR, HslPriority_2)
#define     LG_CC_DOWNLINK_RATE                 M_FrHslSubGroupDefine(LG_GGE_MC_AMR, HslPriority_3)
#define     LG_CC_ENCODE_DUMMY_SPCH_FRAME       M_FrHslSubGroupDefine(LG_GGE_MC_AMR, HslPriority_4)
#define     LG_AMR_ENC_CODEC_RATE_MISMATCH      M_FrHslSubGroupDefine(LG_GGE_MC_AMR, HslPriority_5)
#define     LG_AMR_ENC_CODEC_RATE_ERROR         M_FrHslSubGroupDefine(LG_GGE_MC_AMR, HslPriority_6)

#ifndef     LG_GGE_MC_AMR_CC
# define    LG_GGE_MC_AMR_CC                    M_FrHslGroupDefine(HSL_GGE, 0x1D)
#endif
#define     LG_CC_INPUT_DATA                    M_FrHslSubGroupDefine(LG_GGE_MC_AMR_CC, HslPriority_0)
#define     LG_CC_CRC_DATA                      M_FrHslSubGroupDefine(LG_GGE_MC_AMR_CC, HslPriority_1)
#define     LG_CC_CONVOLVE_INPUT                M_FrHslSubGroupDefine(LG_GGE_MC_AMR_CC, HslPriority_2)
#define     LG_CC_IDMARK                        M_FrHslSubGroupDefine(LG_GGE_MC_AMR_CC, HslPriority_3)
#define     LG_CC_CONVOLVE_OUTPUT               M_FrHslSubGroupDefine(LG_GGE_MC_AMR_CC, HslPriority_4)
#define     LG_CC_DEILEAVE                      M_FrHslSubGroupDefine(LG_GGE_MC_AMR_CC, HslPriority_5)
#define     LG_CC_AMR_CHANNEL_TYPE              M_FrHslSubGroupDefine(LG_GGE_MC_AMR_CC, HslPriority_6)

#ifndef     LG_GGE_MC_AMR_LP
# define    LG_GGE_MC_AMR_LP                    M_FrHslGroupDefine(HSL_GGE, 0x1E)
#endif
#define     LG_AMR_UL_ACS                       M_FrHslSubGroupDefine(LG_GGE_MC_AMR_LP, HslPriority_0)
#define     LG_AMR_UL_ICM                       M_FrHslSubGroupDefine(LG_GGE_MC_AMR_LP, HslPriority_1)
#define     LG_AMR_UL_CMI                       M_FrHslSubGroupDefine(LG_GGE_MC_AMR_LP, HslPriority_2)
#define     LG_AMR_DL_ACS                       M_FrHslSubGroupDefine(LG_GGE_MC_AMR_LP, HslPriority_3)
#define     LG_AMR_DL_ICM                       M_FrHslSubGroupDefine(LG_GGE_MC_AMR_LP, HslPriority_4)
#define     LG_AMR_DL_CMI                       M_FrHslSubGroupDefine(LG_GGE_MC_AMR_LP, HslPriority_5)
#define     LG_AMR_CMR                          M_FrHslSubGroupDefine(LG_GGE_MC_AMR_LP, HslPriority_6)
#define     LG_AMR_CMC                          M_FrHslSubGroupDefine(LG_GGE_MC_AMR_LP, HslPriority_7)

#ifndef     LG_GGE_MC_AMR_LP_EX
# define    LG_GGE_MC_AMR_LP_EX                 M_FrHslGroupDefine(HSL_GGE, 0x1F)
#endif
#define     LG_AMR_RAU_THRESHOLDS               M_FrHslSubGroupDefine(LG_GGE_MC_AMR_LP_EX, HslPriority_0)
#define     LG_AMR_RAU_HYSTERESIS               M_FrHslSubGroupDefine(LG_GGE_MC_AMR_LP_EX, HslPriority_1)
#define     LG_AMR_RAU_QI_AND_CMR               M_FrHslSubGroupDefine(LG_GGE_MC_AMR_LP_EX, HslPriority_2)
#define     LG_PP_AMR_RAU_INH                   M_FrHslSubGroupDefine(LG_GGE_MC_AMR_LP_EX, HslPriority_3)
#define     LG_AMR_INVALID_DL_CMI               M_FrHslSubGroupDefine(LG_GGE_MC_AMR_LP_EX, HslPriority_4)
#define     LG_AMR_INVALID_CMC                  M_FrHslSubGroupDefine(LG_GGE_MC_AMR_LP_EX, HslPriority_5)
#define     LG_AMR_IB_LOOPBACK_DATA             M_FrHslSubGroupDefine(LG_GGE_MC_AMR_LP_EX, HslPriority_6)

/* EQ */
#ifndef     LG_GGE_EQ_GSM
# define    LG_GGE_EQ_GSM                       M_FrHslGroupDefine(HSL_GGE, 0x20)
#endif
#define     LG_PP_EQ_STT                        M_FrHslSubGroupDefine(LG_GGE_EQ_GSM, HslPriority_0)
#define     LG_PP_EQ_END                        M_FrHslSubGroupDefine(LG_GGE_EQ_GSM, HslPriority_0)
#define     LG_EQ_TDS_CONTENTS                  M_FrHslSubGroupDefine(LG_GGE_EQ_GSM, HslPriority_1)
#define     LG_T1_GENERIC_RX_EQ_IQ              M_FrHslSubGroupDefine(LG_GGE_EQ_GSM, HslPriority_2)
#define     LG_PP_EQ_SETUP                      M_FrHslSubGroupDefine(LG_GGE_EQ_GSM, HslPriority_3)
#define     LG_GMSK_MEAN_BEP                    M_FrHslSubGroupDefine(LG_GGE_EQ_GSM, HslPriority_4)
#define     LG_T1_DEROTATED_CHANNEL             M_FrHslSubGroupDefine(LG_GGE_EQ_GSM, HslPriority_5)
#define     LG_T1_MIN_PHASE_CHANNEL             M_FrHslSubGroupDefine(LG_GGE_EQ_GSM, HslPriority_6)
#define     LG_T1_MIN_PHASE_HALF_IQ             M_FrHslSubGroupDefine(LG_GGE_EQ_GSM, HslPriority_7)

#define     LG_T1_GENERIC_RX_EQ_IQ_NEW          M_FrHslSubGroupDefine(LG_GGE_EQ_GSM, HslPriority_1)


#ifndef     LG_GGE_EQ_GMSK
# define    LG_GGE_EQ_GMSK                      M_FrHslGroupDefine(HSL_GGE, 0x21)
#endif
#define     LG_PP_EQ_GMSK                       M_FrHslSubGroupDefine(LG_GGE_EQ_GMSK, HslPriority_0)
#define     LG_PP_EQ_ESTIMATE_CHANNEL           M_FrHslSubGroupDefine(LG_GGE_EQ_GMSK, HslPriority_1)
#define     LG_PP_EQ_COPY_FREQ_DATA             M_FrHslSubGroupDefine(LG_GGE_EQ_GMSK, HslPriority_2)
#define     LG_PP_EQ_CALC_FILTERS               M_FrHslSubGroupDefine(LG_GGE_EQ_GMSK, HslPriority_3)
#define     LG_CC_EQ_CI                         M_FrHslSubGroupDefine(LG_GGE_EQ_GMSK, HslPriority_4)
#define     LG_PP_EQ_PHASE_FILTER               M_FrHslSubGroupDefine(LG_GGE_EQ_GMSK, HslPriority_5)
#define     LG_T1_SELECTED_CHANNEL              M_FrHslSubGroupDefine(LG_GGE_EQ_GMSK, HslPriority_6)
#define     LG_T1_COMPLETE_CHANNEL              M_FrHslSubGroupDefine(LG_GGE_EQ_GMSK, HslPriority_7)

#ifndef     LG_GGE_EQ_8PSK
# define    LG_GGE_EQ_8PSK                      M_FrHslGroupDefine(HSL_GGE, 0x22)
#endif
#define     LG_PP_EQ_8PSK                       M_FrHslSubGroupDefine(LG_GGE_EQ_8PSK, HslPriority_0)
#define     LG_CC_8PSK_USF_SCORES               M_FrHslSubGroupDefine(LG_GGE_EQ_8PSK, HslPriority_1)
#define     LG_T2_REMOD_BITS                    M_FrHslSubGroupDefine(LG_GGE_EQ_8PSK, HslPriority_2)
#define     LG_T1_DFSEV_OUTPUT                  M_FrHslSubGroupDefine(LG_GGE_EQ_8PSK, HslPriority_3)
#define     LG_PP_EQ_CALC_DFSEV                 M_FrHslSubGroupDefine(LG_GGE_EQ_8PSK, HslPriority_4)
#define     LG_PP_EQ_PHASE_FILTER_8PSK          M_FrHslSubGroupDefine(LG_GGE_EQ_8PSK, HslPriority_5)
#define     LG_T1_SELECTED_CHANNEL_8PSK         M_FrHslSubGroupDefine(LG_GGE_EQ_8PSK, HslPriority_6)
#define     LG_T1_COMPLETE_CHANNEL_8PSK         M_FrHslSubGroupDefine(LG_GGE_EQ_8PSK, HslPriority_7)

#ifndef     LG_GGE_EQ_BURST0
# define    LG_GGE_EQ_BURST0                    M_FrHslGroupDefine(HSL_GGE, 0x23)
#endif
#define     LG_PP_EQ_BST_STT                    M_FrHslSubGroupDefine(LG_GGE_EQ_BURST0, HslPriority_0)
#define     LG_PP_EQ_BST_END                    M_FrHslSubGroupDefine(LG_GGE_EQ_BURST0, HslPriority_0)
#define     LG_T1_GENERIC_RX_BST_IQ             M_FrHslSubGroupDefine(LG_GGE_EQ_BURST0, HslPriority_1)
#define     LG_T1_RECEIVED_IQ                   M_FrHslSubGroupDefine(LG_GGE_EQ_BURST0, HslPriority_2)
#define     LG_T1_SCALED_IQ                     M_FrHslSubGroupDefine(LG_GGE_EQ_BURST0, HslPriority_3)
#define     LG_T1_SCALED_IQ_8PSK                M_FrHslSubGroupDefine(LG_GGE_EQ_BURST0, HslPriority_4)
#define     LG_T1_MATCHED_FILTER                M_FrHslSubGroupDefine(LG_GGE_EQ_BURST0, HslPriority_5)
#define     LG_T1_OVERALL_FILTER                M_FrHslSubGroupDefine(LG_GGE_EQ_BURST0, HslPriority_6)
#define     LG_T1_DEROTATED_IQ                  M_FrHslSubGroupDefine(LG_GGE_EQ_BURST0, HslPriority_7)

#ifndef     LG_GGE_EQ_BURST1
# define    LG_GGE_EQ_BURST1                    M_FrHslGroupDefine(HSL_GGE, 0x24)
#endif
#define     LG_PP_EQ_RSSI_STT                   M_FrHslSubGroupDefine(LG_GGE_EQ_BURST1, HslPriority_0)
#define     LG_PP_EQ_RSSI_END                   M_FrHslSubGroupDefine(LG_GGE_EQ_BURST1, HslPriority_0)
#define     LG_EQTDS_DI_HANDLE                  M_FrHslSubGroupDefine(LG_GGE_EQ_BURST1, HslPriority_1)
#define     LG_PP_EQ_CALC_FREQ_OFFSET           M_FrHslSubGroupDefine(LG_GGE_EQ_BURST1, HslPriority_2)
#define     LG_T2_FILTERED_BITS                 M_FrHslSubGroupDefine(LG_GGE_EQ_BURST1, HslPriority_3)
#define     LG_T2_COMPLEX_DIFFERENCE            M_FrHslSubGroupDefine(LG_GGE_EQ_BURST1, HslPriority_4)
#define     LG_T2_PHASE_DIFFERENCE              M_FrHslSubGroupDefine(LG_GGE_EQ_BURST1, HslPriority_5)
#define     LG_EQ_RES                           M_FrHslSubGroupDefine(LG_GGE_EQ_BURST1, HslPriority_6)
#define     LG_PP_EQ_1R7M                       M_FrHslSubGroupDefine(LG_GGE_EQ_BURST1, HslPriority_7)

#ifndef     LG_GGE_EQ_BURST2
# define    LG_GGE_EQ_BURST2                    M_FrHslGroupDefine(HSL_GGE, 0x25)
#endif
#define     LG_PP_EQ_BST2_STT                   M_FrHslSubGroupDefine(LG_GGE_EQ_BURST2, HslPriority_0)
#define     LG_PP_EQ_BST2_END                   M_FrHslSubGroupDefine(LG_GGE_EQ_BURST2, HslPriority_0)
#define     LG_T1_MAX_PHASE_CHANNEL             M_FrHslSubGroupDefine(LG_GGE_EQ_BURST2, HslPriority_1)
#define     LG_T1_MAX_PHASE_HALF_IQ             M_FrHslSubGroupDefine(LG_GGE_EQ_BURST2, HslPriority_2)
#define     LG_T1_DFSEV_OUTPUT_0                M_FrHslSubGroupDefine(LG_GGE_EQ_BURST2, HslPriority_3)
#define     LG_T1_DFSEV_OUTPUT_1                M_FrHslSubGroupDefine(LG_GGE_EQ_BURST2, HslPriority_4)
#define     LG_T1_DEROTATED_IQ_1                M_FrHslSubGroupDefine(LG_GGE_EQ_BURST2, HslPriority_7)

#ifndef     LG_GGE_EQ_BURST3
# define    LG_GGE_EQ_BURST3                    M_FrHslGroupDefine(HSL_GGE, 0x26)
#endif
#define     LG_PP_EQ_BST3_STT                   M_FrHslSubGroupDefine(LG_GGE_EQ_BURST3, HslPriority_0)
#define     LG_PP_EQ_BST3_END                   M_FrHslSubGroupDefine(LG_GGE_EQ_BURST3, HslPriority_0)
#define     LG_EQTDS_DI_HANDLE_1                M_FrHslSubGroupDefine(LG_GGE_EQ_BURST3, HslPriority_1)
#define     LG_PP_EQ_CALC_FREQ_OFFSET_1         M_FrHslSubGroupDefine(LG_GGE_EQ_BURST3, HslPriority_2)
#define     LG_T2_FILTERED_BITS_1               M_FrHslSubGroupDefine(LG_GGE_EQ_BURST3, HslPriority_3)
#define     LG_T2_COMPLEX_DIFFERENCE_1          M_FrHslSubGroupDefine(LG_GGE_EQ_BURST3, HslPriority_4)
#define     LG_T2_PHASE_DIFFERENCE_1            M_FrHslSubGroupDefine(LG_GGE_EQ_BURST3, HslPriority_5)
#define     LG_EQ_RES_1                         M_FrHslSubGroupDefine(LG_GGE_EQ_BURST3, HslPriority_6)

#ifndef     LG_GGE_EQ_VIT_SD
# define    LG_GGE_EQ_VIT_SD                    M_FrHslGroupDefine(HSL_GGE, 0x27)
#endif
#define     LG_PP_EQ_VITERBI                    M_FrHslSubGroupDefine(LG_GGE_EQ_VIT_SD, HslPriority_0)
#define     LG_T1_VITERBI_INPUT                 M_FrHslSubGroupDefine(LG_GGE_EQ_VIT_SD, HslPriority_1)
#define     LG_T1_VITERBI_OUTPUT                M_FrHslSubGroupDefine(LG_GGE_EQ_VIT_SD, HslPriority_2)
#define     LG_T1_VITERBI_TABLE                 M_FrHslSubGroupDefine(LG_GGE_EQ_VIT_SD, HslPriority_3)
#define     LG_PP_EQ_CALC_SOFT_DECISION         M_FrHslSubGroupDefine(LG_GGE_EQ_VIT_SD, HslPriority_4)
#define     LG_T1_SOFT_DECISION_BITS0           M_FrHslSubGroupDefine(LG_GGE_EQ_VIT_SD, HslPriority_5)
#define     LG_T1_SOFT_DECISION_BITS1           M_FrHslSubGroupDefine(LG_GGE_EQ_VIT_SD, HslPriority_6)
#define     LG_T1_SOFT_DECISION_BITS2           M_FrHslSubGroupDefine(LG_GGE_EQ_VIT_SD, HslPriority_7)

/* FBS: */
#ifndef     LG_GGE_FBS
# define    LG_GGE_FBS                          M_FrHslGroupDefine(HSL_GGE, 0x28)
#endif
#define     LG_PP_FBSEARCH_STT                  M_FrHslSubGroupDefine(LG_GGE_FBS, HslPriority_0)
#define     LG_PP_FBSEARCH_END                  M_FrHslSubGroupDefine(LG_GGE_FBS, HslPriority_0)
#define     LG_FBS_TDS_CONTENTS                 M_FrHslSubGroupDefine(LG_GGE_FBS, HslPriority_1)
#define     LG_PP_TIM_TDQ_ENTRY_FBS             M_FrHslSubGroupDefine(LG_GGE_FBS, HslPriority_2)
#define     LG_T1_GENERIC_FB_IQ                 M_FrHslSubGroupDefine(LG_GGE_FBS, HslPriority_3)
#define     LG_T2_FB_RECEIVED_IQ                M_FrHslSubGroupDefine(LG_GGE_FBS, HslPriority_4)
#define     LG_T2_FB_RECEIVED_IQ_NO_DC          M_FrHslSubGroupDefine(LG_GGE_FBS, HslPriority_5)
#define     LG_F_BURST_DATA                     M_FrHslSubGroupDefine(LG_GGE_FBS, HslPriority_6)
#define     LG_PP_DMA_PTR                       M_FrHslSubGroupDefine(LG_GGE_FBS, HslPriority_7)

#ifndef     LG_GGE_FBS1
# define    LG_GGE_FBS1                         M_FrHslGroupDefine(HSL_GGE, 0x29)
#endif
#define     LG_T2_FB_DETECT_ENERGY_IN           M_FrHslSubGroupDefine(LG_GGE_FBS1, HslPriority_0)
#define     LG_T2_FB_DETECT_ENERGY_OUT          M_FrHslSubGroupDefine(LG_GGE_FBS1, HslPriority_1)
#define     LG_T2_FB_DETECT_ENERGY_RATIO        M_FrHslSubGroupDefine(LG_GGE_FBS1, HslPriority_2)
#define     LG_FB_OUT_OF_CPU_RESOURCE           M_FrHslSubGroupDefine(LG_GGE_FBS1, HslPriority_3)
#define     LG_T2_FB_PHASE                      M_FrHslSubGroupDefine(LG_GGE_FBS1, HslPriority_4)
#define     LG_T2_FB_UNWRAPPED_PHASE            M_FrHslSubGroupDefine(LG_GGE_FBS1, HslPriority_5)
#define     LG_FREQ_EST                         M_FrHslSubGroupDefine(LG_GGE_FBS1, HslPriority_6)
#define     LG_PP_FBS_ABORT                     M_FrHslSubGroupDefine(LG_GGE_FBS1, HslPriority_7)

#ifndef     LG_GGE_FBS_MISC
# define    LG_GGE_FBS_MISC                     M_FrHslGroupDefine(HSL_GGE, 0x2A)
#endif
#define     LG_GGE_FBS_MISC_INVsOKE             M_FrHslSubGroupDefine(LG_GGE_FBS_MISC, HslPriority_0)
#define     LG_GGE_FBS_MISC_SAMPLE              M_FrHslSubGroupDefine(LG_GGE_FBS_MISC, HslPriority_1)
#define     LG_GGE_FBS_MISC_FREQ_AVG            M_FrHslSubGroupDefine(LG_GGE_FBS_MISC, HslPriority_2)
#define     LG_GGE_FBS_MISC_STD_DEV             M_FrHslSubGroupDefine(LG_GGE_FBS_MISC, HslPriority_3)

/* MISC */
#ifndef     LG_GGE_MISC
# define    LG_GGE_MISC                         M_FrHslGroupDefine(HSL_GGE, 0x2B)
#endif
#define     LG_PP_MISC_VLIF_BIT                 M_FrHslSubGroupDefine(LG_GGE_MISC, HslPriority_0)
#define     LG_PP_MISC_FULL_SCALE               M_FrHslSubGroupDefine(LG_GGE_MISC, HslPriority_1)
#define     LG_PP_MISC_INTER_DIR                M_FrHslSubGroupDefine(LG_GGE_MISC, HslPriority_2)
#define     LG_PP_MISC_DEINTER                  M_FrHslSubGroupDefine(LG_GGE_MISC, HslPriority_3)
#define     LG_PP_MISC_AMR_CONV                 M_FrHslSubGroupDefine(LG_GGE_MISC, HslPriority_4)
#define     LG_PP_MISC_AMR_CONV_OUT             M_FrHslSubGroupDefine(LG_GGE_MISC, HslPriority_5)
#define     LG_L1_DEVCHECK                      M_FrHslSubGroupDefine(LG_GGE_MISC, HslPriority_7)

#if defined(UPGRADE_SB_SEARCHER)
/* SBSearcher: */
#ifndef     LG_GGE_SBS
# define    LG_GGE_SBS                          M_FrHslGroupDefine(HSL_GGE, 0x2C)
#endif
#define     LG_PP_SBSEARCH_STT                  M_FrHslSubGroupDefine(LG_GGE_SBS, HslPriority_0)
#define     LG_PP_SBSEARCH_END                  M_FrHslSubGroupDefine(LG_GGE_SBS, HslPriority_0)
#define     LG_SBS_TDS_CONTENTS                 M_FrHslSubGroupDefine(LG_GGE_SBS, HslPriority_1)
#define     LG_SBS_ENERGY                       M_FrHslSubGroupDefine(LG_GGE_SBS, HslPriority_2)
#define     LG_SBS_DMA_DEBUG                    M_FrHslSubGroupDefine(LG_GGE_SBS, HslPriority_3)
#define     LG_SBS_MISC                         M_FrHslSubGroupDefine(LG_GGE_SBS, HslPriority_4)
#define     LG_SBS_TEST_MODE                    M_FrHslSubGroupDefine(LG_GGE_SBS, HslPriority_5)
#define     LG_SBS_RECEIVED_IQ_NO_DC            M_FrHslSubGroupDefine(LG_GGE_SBS, HslPriority_6)
#endif /* UPGRADE_SB_SEARCHER */


/*SAIC*/
#ifndef     LG_GGE_SAIC
# define    LG_GGE_SAIC                          M_FrHslGroupDefine(HSL_GGE, 0x2D)
#endif

#define     LG_GG_SAIC_INFO                      M_FrHslSubGroupDefine(LG_GGE_SAIC, HslPriority_0)
#define     LG_GG_SAIC_COMPFTER_2X               M_FrHslSubGroupDefine(LG_GGE_SAIC, HslPriority_1)
#define     LG_GG_SAIC_DEBUG                     M_FrHslSubGroupDefine(LG_GGE_SAIC, HslPriority_2)
#define     LG_GG_SAIC_PROFILE                   M_FrHslSubGroupDefine(LG_GGE_SAIC, HslPriority_3)




#if defined(UPGRADE_FBS)
/* New FBS */
#ifndef     LG_GGE_UPGRADE_FBS
# define    LG_GGE_UPGRADE_FBS                  M_FrHslGroupDefine(HSL_GGE, 0x1A)
#endif
#define     LG_FBS_ENERGY                       M_FrHslSubGroupDefine(LG_GGE_UPGRADE_FBS, HslPriority_2)
#define     LG_FBS_DMA_DEBUG                    M_FrHslSubGroupDefine(LG_GGE_UPGRADE_FBS, HslPriority_3)
#define     LG_FBS_MISC                         M_FrHslSubGroupDefine(LG_GGE_UPGRADE_FBS, HslPriority_4)
#define     LG_FBS_TEST_MODE                    M_FrHslSubGroupDefine(LG_GGE_UPGRADE_FBS, HslPriority_5)
#endif /*UPGRADE_FBS*/

#if defined (FR_ABP_CLK_GATING_GGE)
#ifndef     LG_GGE_UPGRADE_GGEABP
# define    LG_GGE_UPGRADE_GGEABP               M_FrHslGroupDefine(HSL_GGE, 0x2E)
#endif
#define     LG_GGEABP_LOG                       M_FrHslSubGroupDefine(LG_GGE_UPGRADE_GGEABP, HslPriority_2)
#endif

#endif  /* ENABLE_NEW_MDFID */

/**@}*//* HSL_GGE_GROUP */

/**
 * \defgroup HSL_TL1_GROUP
 * @{
 */

/** L1 framework, including sub frame handler, slot handler, sorter, SHM signal message */
#ifndef     HSL_TL1_FRW
# define    HSL_TL1_FRW                         M_FrHslGroupDefine(HSL_TL1, 0x01)
#endif

/** Interface with protocol stack, including CPHY, UPHY */
#ifndef     HSL_TL1_ITF
# define    HSL_TL1_ITF                         M_FrHslGroupDefine(HSL_TL1, 0x02)
#endif

/** Sub state handler running in sub frame interrupt */
#ifndef     HSL_TL1_SSH
# define    HSL_TL1_SSH                         M_FrHslGroupDefine(HSL_TL1, 0x03)
#endif

/** Sequencer */
#ifndef     HSL_TL1_SEQ
# define    HSL_TL1_SEQ                         M_FrHslGroupDefine(HSL_TL1, 0x04)
#endif

/** ARBITER */
#ifndef     HSL_TL1_ARB
# define    HSL_TL1_ARB                         M_FrHslGroupDefine(HSL_TL1, 0x05)
#endif

/** RXDH, TXDH */
#ifndef     HSL_TL1_DH
# define    HSL_TL1_DH                          M_FrHslGroupDefine(HSL_TL1, 0x06)
#endif

/** CSM */
#ifndef     HSL_TL1_CSM
# define    HSL_TL1_CSM                         M_FrHslGroupDefine(HSL_TL1, 0x07)
#endif

/** IFIC */
#ifndef     HSL_TL1_IFIC
# define    HSL_TL1_IFIC                        M_FrHslGroupDefine(HSL_TL1, 0x08)
#endif

/** LOOP: AGC, AFC, RX TIMING, TA, Power Control */
#ifndef     HSL_TL1_LP
# define    HSL_TL1_LP                          M_FrHslGroupDefine(HSL_TL1, 0x09)
#endif

/** Dual Mode */
#ifndef     HSL_TL1_DM
# define    HSL_TL1_DM                          M_FrHslGroupDefine(HSL_TL1, 0x0A)
#endif

/** RF Driver */
#ifndef     HSL_TL1_RFD
# define    HSL_TL1_RFD                         M_FrHslGroupDefine(HSL_TL1, 0x0B)
#endif

/** Slow Clock */
#ifndef     HSL_TL1_SLC
# define    HSL_TL1_SLC                         M_FrHslGroupDefine(HSL_TL1, 0x0C)
#endif

/** RRM */
#ifndef     HSL_TL1_RRM
# define    HSL_TL1_RRM                         M_FrHslGroupDefine(HSL_TL1, 0x0D)
#endif

/**@}*//* HSL_TL1_GROUP */

/**
 * \defgroup HSL_TPH_GROUP
 * @{
 */

/** Uplink BRP */
#ifndef     HSL_TPH_DPUL
# define    HSL_TPH_DPUL                        M_FrHslGroupDefine(HSL_TPH, 0x01)
#endif

/** Txm */
#ifndef     HSL_TPH_ULCRP
# define    HSL_TPH_ULCRP                       M_FrHslGroupDefine(HSL_TPH, 0x02)
#endif

/** Keystone adapter */
#ifndef     HSL_TPH_KAP
# define    HSL_TPH_KAP                         M_FrHslGroupDefine(HSL_TPH, 0x03)
#endif

/** DFE */
#ifndef     HSL_TPH_DFE
# define    HSL_TPH_DFE                         M_FrHslGroupDefine(HSL_TPH, 0x04)
#endif

/** DL CRP */
#ifndef     HSL_TPH_DLCRP
# define    HSL_TPH_DLCRP                       M_FrHslGroupDefine(HSL_TPH, 0x05)
#endif

/** Rxd */
#ifndef     HSL_TPH_DPDL
# define    HSL_TPH_DPDL                        M_FrHslGroupDefine(HSL_TPH, 0x06)
#endif

/** CSM */
#ifndef     HSL_TPH_CSM
# define    HSL_TPH_CSM                         M_FrHslGroupDefine(HSL_TPH, 0x07)
#endif

/** ARRY */
#ifndef     HSL_TPH_ARY
# define    HSL_TPH_ARY                         M_FrHslGroupDefine(HSL_TPH, 0x08)
#endif

/** UT */
#ifndef     HSL_TPH_UT
# define    HSL_TPH_UT                          M_FrHslGroupDefine(HSL_TPH, 0x09)
#endif

/** LOOP: ATC, AFC, ULPC, DLPC, CQI */
#ifndef     HSL_TPH_LP
# define    HSL_TPH_LP                          M_FrHslGroupDefine(HSL_TPH, 0x0A)
#endif

/**@}*//* HSL_TPH_GROUP */

/**
 * \defgroup HSL_WL1_GROUP
 * @{
 */

/** DCH */
#ifndef     HSL_WL1_DCH
# define    HSL_WL1_DCH                         M_FrHslGroupDefine(HSL_WL1, 0x01)
#endif
#define     HSL_WL1_DCH_ESSENTIAL               M_FrHslSubGroupDefine(HSL_WL1_DCH, HslPriority_0)
#define     HSL_WL1_DCH_TPC                     M_FrHslSubGroupDefine(HSL_WL1_DCH, HslPriority_1)
#define     HSL_WL1_DCH_TPC_MISC                M_FrHslSubGroupDefine(HSL_WL1_DCH, HslPriority_7)
#define     HSL_WL1_DCH_TIMER                   M_FrHslSubGroupDefine(HSL_WL1_DCH, HslPriority_2)
#define     HSL_WL1_DCH_BLER                    M_FrHslSubGroupDefine(HSL_WL1_DCH, HslPriority_2)
#define     HSL_WL1_DCH_CM                      M_FrHslSubGroupDefine(HSL_WL1_DCH, HslPriority_2)
#define     HSL_WL1_DCH_TX                      M_FrHslSubGroupDefine(HSL_WL1_DCH, HslPriority_0)
#define     HSL_WL1_DCH_RX                      M_FrHslSubGroupDefine(HSL_WL1_DCH, HslPriority_0)
#define     HSL_WL1_DCH_MISC                    M_FrHslSubGroupDefine(HSL_WL1_DCH, HslPriority_7)
#define     HSL_WL1_RX_DH                       M_FrHslSubGroupDefine(HSL_WL1_DCH, HslPriority_2)
#define     HSL_WL1_SQ_CM_FRAME_INFO            M_FrHslSubGroupDefine(HSL_WL1_DCH, HslPriority_2)
#define     HSL_WL1_SQ_CM_UL_NEXT_TTI_DATA      M_FrHslSubGroupDefine(HSL_WL1_DCH, HslPriority_2)

/** RACH */
#ifndef     HSL_WL1_RACH
# define    HSL_WL1_RACH                        M_FrHslGroupDefine(HSL_WL1, 0x02)
#endif
#define     HSL_WL1_RACH_ESSENTIAL              M_FrHslSubGroupDefine(HSL_WL1_RACH, HslPriority_0)
#define     HSL_WL1_RACH_TIMING                 M_FrHslSubGroupDefine(HSL_WL1_RACH, HslPriority_2)

/** FACH */
#ifndef     HSL_WL1_FACH
# define    HSL_WL1_FACH                        M_FrHslGroupDefine(HSL_WL1, 0x03)
#endif
#define     HSL_WL1_FACH_ESSENTIAL              M_FrHslSubGroupDefine(HSL_WL1_FACH, HslPriority_0)
#define     HSL_WL1_FACH_MEASUREMENT            M_FrHslSubGroupDefine(HSL_WL1_FACH, HslPriority_2)

/** SLCLK */
#ifndef     HSL_WL1_SLCLK
# define    HSL_WL1_SLCLK                       M_FrHslGroupDefine(HSL_WL1, 0x04)
#endif
#define     HSL_WL1_SLCLK_ESSENTIAL             M_FrHslSubGroupDefine(HSL_WL1_SLCLK, HslPriority_0)
#define     HSL_WL1_SLCLK_MISC                  M_FrHslSubGroupDefine(HSL_WL1_SLCLK, HslPriority_7)

/** RAKE */
#ifndef     HSL_WL1_RAKE
# define    HSL_WL1_RAKE                        M_FrHslGroupDefine(HSL_WL1, 0x05)
#endif
#define     HSL_WL1_RAKE_ESSENTIAL              M_FrHslSubGroupDefine(HSL_WL1_RAKE, HslPriority_0)
#define     HSL_WL1_RAKE_RPA                    M_FrHslSubGroupDefine(HSL_WL1_RAKE, HslPriority_2)
#define     HSL_WL1_RAKE_RPA_CM                 M_FrHslSubGroupDefine(HSL_WL1_RAKE, HslPriority_0)
#define     HSL_WL1_RAKE_PAMCMDR                M_FrHslSubGroupDefine(HSL_WL1_RAKE, HslPriority_2)
#define     HSL_WL1_RAKE_MISC                   M_FrHslSubGroupDefine(HSL_WL1_RAKE, HslPriority_7)
#define     HSL_WL1_RAKE_TST_SQ                 M_FrHslSubGroupDefine(HSL_WL1_RAKE, HslPriority_7)


/** PCH */
#ifndef     HSL_WL1_PCH
# define    HSL_WL1_PCH                         M_FrHslGroupDefine(HSL_WL1, 0x06)
#endif
#define     HSL_WL1_PCH_ESSENTIAL               M_FrHslSubGroupDefine(HSL_WL1_PCH, HslPriority_0)
#define     HSL_WL1_PCH_SCHEDULING              M_FrHslSubGroupDefine(HSL_WL1_PCH, HslPriority_2)
#define     HSL_WL1_PCH_SCHEDULER               M_FrHslSubGroupDefine(HSL_WL1_PCH, HslPriority_2)
#define     HSL_WL1_PCH_MISC                    M_FrHslSubGroupDefine(HSL_WL1_PCH, HslPriority_7)

/** MEAS */
#ifndef     HSL_WL1_MEAS
# define    HSL_WL1_MEAS                        M_FrHslGroupDefine(HSL_WL1, 0x07)
#endif
#define     HSL_WL1_MEAS_ESSENTIAL              M_FrHslSubGroupDefine(HSL_WL1_MEAS, HslPriority_0)
#define     HSL_WL1_MEAS_CELL_STATUS            M_FrHslSubGroupDefine(HSL_WL1_MEAS, HslPriority_2)
#define     HSL_WL1_MEAS_CELL_LIST              M_FrHslSubGroupDefine(HSL_WL1_MEAS, HslPriority_2)

/** FCS */
#ifndef     HSL_WL1_FCS
# define    HSL_WL1_FCS                         M_FrHslGroupDefine(HSL_WL1, 0x08)
#endif
#define     HSL_WL1_FCS_ESSENTIAL               M_FrHslSubGroupDefine(HSL_WL1_FCS, HslPriority_0)
#define     HSL_WL1_FCS_CALLBACK_FUNC           M_FrHslSubGroupDefine(HSL_WL1_FCS, HslPriority_2)
#define     HSL_WL1_FCS_FCS_FUNC                M_FrHslSubGroupDefine(HSL_WL1_FCS, HslPriority_2)
#define     HSL_WL1_FCS_IT                      M_FrHslSubGroupDefine(HSL_WL1_FCS, HslPriority_7)
#define     HSL_WL1_FCS_MISC                    M_FrHslSubGroupDefine(HSL_WL1_FCS, HslPriority_7)

/** BCH */
#ifndef     HSL_WL1_BCH
# define    HSL_WL1_BCH                         M_FrHslGroupDefine(HSL_WL1, 0x09)
#endif
#define     HSL_WL1_BCH_ESSENTIAL               M_FrHslSubGroupDefine(HSL_WL1_BCH, HslPriority_0)
#define     HSL_WL1_BCH_SCHEDULING              M_FrHslSubGroupDefine(HSL_WL1_BCH, HslPriority_2)
#define     HSL_WL1_BCH_MISC                    M_FrHslSubGroupDefine(HSL_WL1_BCH, HslPriority_7)

/** HSDPA */
#ifndef     HSL_WL1_HSDPA
# define    HSL_WL1_HSDPA                       M_FrHslGroupDefine(HSL_WL1, 0x0A)
#endif
#define     HSL_WL1_HSDPA_ESSENTIAL             M_FrHslSubGroupDefine(HSL_WL1_HSDPA, HslPriority_0)
#define     HSL_WL1_HSDPA_SCHEDULING            M_FrHslSubGroupDefine(HSL_WL1_HSDPA, HslPriority_1)
#define     HSL_WL1_HSDPA_DMA                   M_FrHslSubGroupDefine(HSL_WL1_HSDPA, HslPriority_1)
#define     HSL_WL1_HSDPA_UL                    M_FrHslSubGroupDefine(HSL_WL1_HSDPA, HslPriority_1)

/** MSG */
#ifndef     HSL_WL1_MSG
# define    HSL_WL1_MSG                         M_FrHslGroupDefine(HSL_WL1, 0x0B)
#endif
#define     HSL_WL1_MSG_ESSENTIAL               M_FrHslSubGroupDefine(HSL_WL1_MSG, HslPriority_0)
#define     HSL_WL1_MSG_UPHY_SIG                M_FrHslSubGroupDefine(HSL_WL1_MSG, HslPriority_2)
#define     HSL_WL1_MSG_MISC                    M_FrHslSubGroupDefine(HSL_WL1_MSG, HslPriority_7)

/** CSM */
#ifndef     HSL_WL1_CSM
# define    HSL_WL1_CSM                         M_FrHslGroupDefine(HSL_WL1, 0x0C)
#endif
#define     HSL_WL1_CSM_ESSENTIAL               M_FrHslSubGroupDefine(HSL_WL1_CSM, HslPriority_0)
#define     HSL_WL1_CSM_MPS                     M_FrHslSubGroupDefine(HSL_WL1_CSM, HslPriority_2)
#define     HSL_WL1_CSM_CPICH                   M_FrHslSubGroupDefine(HSL_WL1_CSM, HslPriority_2)
#define     HSL_WL1_CSM_PSCH                    M_FrHslSubGroupDefine(HSL_WL1_CSM, HslPriority_2)
#define     HSL_WL1_CSM_SSCH                    M_FrHslSubGroupDefine(HSL_WL1_CSM, HslPriority_2)
#define     HSL_WL1_CSM_RSSI                    M_FrHslSubGroupDefine(HSL_WL1_CSM, HslPriority_2)

/** SYNC */
#ifndef     HSL_WL1_SYNC
# define    HSL_WL1_SYNC                        M_FrHslGroupDefine(HSL_WL1, 0x0D)
#endif
#define     HSL_WL1_SYNC_ESSENTIAL              M_FrHslSubGroupDefine(HSL_WL1_SYNC, HslPriority_0)
#define     HSL_WL1_SYNC_MISC                   M_FrHslSubGroupDefine(HSL_WL1_SYNC, HslPriority_7)

/** AFC */
#ifndef     HSL_WL1_AFC
# define    HSL_WL1_AFC                         M_FrHslGroupDefine(HSL_WL1, 0x0E)
#endif
#define     HSL_WL1_AFC_ESSENTIAL               M_FrHslSubGroupDefine(HSL_WL1_AFC, HslPriority_0)
#define     HSL_WL1_AFC_MISC                    M_FrHslSubGroupDefine(HSL_WL1_AFC, HslPriority_7)

/** SYS */
#ifndef     HSL_WL1_SYS
# define    HSL_WL1_SYS                         M_FrHslGroupDefine(HSL_WL1, 0x0F)
#endif
#define     HSL_WL1_SYS_ESSENTIAL               M_FrHslSubGroupDefine(HSL_WL1_SYS, HslPriority_0)
#define     HSL_WL1_SYS_TIME                    M_FrHslSubGroupDefine(HSL_WL1_SYS, HslPriority_1)
#define     HSL_WL1_SYS_SEFN                    M_FrHslSubGroupDefine(HSL_WL1_SYS, HslPriority_2)
#define     HSL_WL1_SYS_MISC                    M_FrHslSubGroupDefine(HSL_WL1_SYS, HslPriority_7)
#define     HSL_WL1_SYS_STATE                   M_FrHslSubGroupDefine(HSL_WL1_SYS, HslPriority_2)
#define     HSL_WL1_SYS_ACT_TIME                M_FrHslSubGroupDefine(HSL_WL1_SYS, HslPriority_0)
#define     HSL_WL1_SYS_ALARM                   M_FrHslSubGroupDefine(HSL_WL1_SYS, HslPriority_0)

/** GIW */
#ifndef     HSL_WL1_GIW
# define    HSL_WL1_GIW                         M_FrHslGroupDefine(HSL_WL1, 0x10)
#endif
#define     HSL_WL1_GIW_ESSENTIAL               M_FrHslSubGroupDefine(HSL_WL1_GIW, HslPriority_0)
#define     HSL_WL1_GIW_DCHFACH                 M_FrHslSubGroupDefine(HSL_WL1_GIW, HslPriority_2)
#define     HSL_WL1_GIW_PCH                     M_FrHslSubGroupDefine(HSL_WL1_GIW, HslPriority_2)
#define     HSL_WL1_GIW_SCHEDULER               M_FrHslSubGroupDefine(HSL_WL1_GIW, HslPriority_2)
#define     HSL_WL1_GIW_MODECHANGE              M_FrHslSubGroupDefine(HSL_WL1_GIW, HslPriority_2)
#define     HSL_WL1_GIW_SEQ                     M_FrHslSubGroupDefine(HSL_WL1_GIW, HslPriority_2)
#define     HSL_WL1_GIW_MISC                    M_FrHslSubGroupDefine(HSL_WL1_GIW, HslPriority_7)

/** WIG */
#ifndef     HSL_WL1_WIG
# define    HSL_WL1_WIG                         M_FrHslGroupDefine(HSL_WL1, 0x11)
#endif
#define     HSL_WL1_WIG_ESSENTIAL               M_FrHslSubGroupDefine(HSL_WL1_WIG, HslPriority_0)
#define     HSL_WL1_WIG_CONNECTED               M_FrHslSubGroupDefine(HSL_WL1_WIG, HslPriority_2)
#define     HSL_WL1_WIG_IDLE                    M_FrHslSubGroupDefine(HSL_WL1_WIG, HslPriority_2)
#define     HSL_WL1_WIG_SUB_SM                  M_FrHslSubGroupDefine(HSL_WL1_WIG, HslPriority_2)
#define     HSL_WL1_WIG_WIG_SI                  M_FrHslSubGroupDefine(HSL_WL1_WIG, HslPriority_2)
#define     HSL_WL1_WIG_TIMING                  M_FrHslSubGroupDefine(HSL_WL1_WIG, HslPriority_2)
#define     HSL_WL1_WIG_RX_CTRL                 M_FrHslSubGroupDefine(HSL_WL1_WIG, HslPriority_2)
#define     HSL_WL1_WIG_MISC                    M_FrHslSubGroupDefine(HSL_WL1_WIG, HslPriority_7)

/** RF */
#ifndef     HSL_WL1_RF
# define    HSL_WL1_RF                          M_FrHslGroupDefine(HSL_WL1, 0x12)
#endif
#define     HSL_WL1_RF_ESSENTIAL                M_FrHslSubGroupDefine(HSL_WL1_RF, HslPriority_0)
#define     HSL_WL1_RF_RX                       M_FrHslSubGroupDefine(HSL_WL1_RF, HslPriority_2)
#define     HSL_WL1_RF_TX                       M_FrHslSubGroupDefine(HSL_WL1_RF, HslPriority_2)
#define     HSL_WL1_RF_PWR                      M_FrHslSubGroupDefine(HSL_WL1_RF, HslPriority_2)
#define     HSL_WL1_RF_MODECHANGE               M_FrHslSubGroupDefine(HSL_WL1_RF, HslPriority_2)
#define     HSL_WL1_RF_CAL                      M_FrHslSubGroupDefine(HSL_WL1_RF, HslPriority_2)
#define     HSL_WL1_RF_AFC                      M_FrHslSubGroupDefine(HSL_WL1_RF, HslPriority_2)
#define     HSL_WL1_RF_MISC                     M_FrHslSubGroupDefine(HSL_WL1_RF, HslPriority_7)
#define     HSL_WL1_RF_TX_DEBUG                 M_FrHslSubGroupDefine(HSL_WL1_RF, HslPriority_7)


/** AGC */
#ifndef     HSL_WL1_AGC
# define    HSL_WL1_AGC                          M_FrHslGroupDefine(HSL_WL1, 0x13)
#endif
#define     HSL_WL1_AGC_ESSENTIAL                M_FrHslSubGroupDefine(HSL_WL1_RF, HslPriority_0)
#define     HSL_WL1_AGC_INIT                     M_FrHslSubGroupDefine(HSL_WL1_RF, HslPriority_2)
#define     HSL_WL1_AGC_ALLOCATE                 M_FrHslSubGroupDefine(HSL_WL1_RF, HslPriority_1)
#define     HSL_WL1_AGC_RETRIEVE                 M_FrHslSubGroupDefine(HSL_WL1_RF, HslPriority_0)
#define     HSL_WL1_AGC_AGE                      M_FrHslSubGroupDefine(HSL_WL1_RF, HslPriority_0)
#define     HSL_WL1_AGC_MISC                     M_FrHslSubGroupDefine(HSL_WL1_RF, HslPriority_2)
#define     HSL_WL1_AGC_CHECK                    M_FrHslSubGroupDefine(HSL_WL1_RF, HslPriority_2)

/** RX DH */
#ifndef     HSL_WL1_RX_DH
# define    HSL_WL1_RX_DH                         M_FrHslGroupDefine(HSL_WL1, 0x14)
#endif

#define     HSL_WL1_RX_DH_ESSENTIAL             M_FrHslSubGroupDefine(HSL_WL1_RX_DH, HslPriority_0)
#define     HSL_WL1_RX_DH_ALLOC                 M_FrHslSubGroupDefine(HSL_WL1_RX_DH, HslPriority_2)

/** HSUPA */
#ifndef     HSL_WL1_HSUPA
# define    HSL_WL1_HSUPA                       M_FrHslGroupDefine(HSL_WL1, 0x15)
#endif
#define     HSL_WL1_HSUPA_ESSENTIAL             M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_0)
#define     HSL_WL1_HSUPA_SCHEDULING            M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_1)
#define     HSL_WL1_HSUPA_MEAS                  M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_1)
#define     HSL_WL1_HSUPA_RPA                   M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_2)
#define     HSL_WL1_HSUPA_MISC                  M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_7)
#define     HSL_WL1_HSUPA_EDCH_ESSENTIAL        M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_1)
#define     HSL_WL1_HSUPA_EDCH_MEAS             M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_2)
#define     HSL_WL1_HSUPA_EDCH_MISC             M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_3)
#define     HSL_WL1_HSUPA_RXSQ_ESSENTIAL        M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_1)
#define     HSL_WL1_HSUPA_RXSQ_MEAS             M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_2)
#define     HSL_WL1_HSUPA_RXSQ_MISC             M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_4)
#define     HSL_WL1_HSUPA_EHICH_ESSENTIAL       M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_1)
#define     HSL_WL1_HSUPA_EHICH_MEAS            M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_2)
#define     HSL_WL1_HSUPA_EHICH_RESULTS         M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_2)
#define     HSL_WL1_HSUPA_EHICH_MISC            M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_3)
#define     HSL_WL1_HSUPA_EAGCH_ESSENTIAL       M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_1)
#define     HSL_WL1_HSUPA_EAGCH_MEAS            M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_2)
#define     HSL_WL1_HSUPA_EAGCH_RESULTS         M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_2)
#define     HSL_WL1_HSUPA_EAGCH_MISC            M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_3)
#define     HSL_WL1_HSUPA_ERGCH_ESSENTIAL       M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_1)
#define     HSL_WL1_HSUPA_ERGCH_MEAS            M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_2)
#define     HSL_WL1_HSUPA_ERGCH_RESULTS         M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_2)
#define     HSL_WL1_HSUPA_ERGCH_MISC            M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_3)
#define     HSL_WL1_HSUPA_ERGCH_NS_ESSENTIAL    M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_1)
#define     HSL_WL1_HSUPA_ERGCH_NS_MEAS         M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_2)
#define     HSL_WL1_HSUPA_ERGCH_NS_RESULTS      M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_2)
#define     HSL_WL1_HSUPA_ERGCH_NS_MISC         M_FrHslSubGroupDefine(HSL_WL1_HSUPA, HslPriority_3)

/**@}*//* HSL_WL1_GROUP */

/**
 * \defgroup HSL_UPH_GROUP
 * @{
 */

/**** group HSL_UPH_RXM ****/
#ifndef     HSL_UPH_RXM
# define    HSL_UPH_RXM                         M_FrHslGroupDefine(HSL_UPH, 0x01)
#endif
#define     HSL_UPH_RXM_ESSENTIAL               M_FrHslSubGroupDefine(HSL_UPH_RXM, HslPriority_0)
#define     HSL_UPH_RXM_MISC                    M_FrHslSubGroupDefine(HSL_UPH_RXM, HslPriority_7)

/**** group HSL_UPH_CPS ****/
#ifndef     HSL_UPH_CPS
# define    HSL_UPH_CPS                         M_FrHslGroupDefine(HSL_UPH, 0x02)
#endif
#define     HSL_UPH_CPS_ESSENTIAL               M_FrHslSubGroupDefine(HSL_UPH_CPS, HslPriority_0)
#define     HSL_UPH_CPS_MISC                    M_FrHslSubGroupDefine(HSL_UPH_CPS, HslPriority_7)

/**** group HSL_UPH_DLC_SCALE ****/
#ifndef     HSL_UPH_DLC_SCALE
# define    HSL_UPH_DLC_SCALE                   M_FrHslGroupDefine(HSL_UPH, 0x03)
#endif
#define     HSL_UPH_DLC_SCALE_ESSENTIAL         M_FrHslSubGroupDefine(HSL_UPH_DLC_SCALE, HslPriority_0)
#define     HSL_UPH_DLC_SCALE_SCALE_MISC        M_FrHslSubGroupDefine(HSL_UPH_DLC_SCALE, HslPriority_7)

/**** group HSL_UPH_TXM ****/
#ifndef     HSL_UPH_TXM
# define    HSL_UPH_TXM                         M_FrHslGroupDefine(HSL_UPH, 0x04)
#endif
#define     HSL_UPH_TXM_ESSENTIAL               M_FrHslSubGroupDefine(HSL_UPH_TXM, HslPriority_0)
#define     HSL_UPH_TXM_MISC                    M_FrHslSubGroupDefine(HSL_UPH_TXM, HslPriority_7)

/**** group HSL_UPH_DPDL ****/
#ifndef     HSL_UPH_DPDL
# define    HSL_UPH_DPDL                        M_FrHslGroupDefine(HSL_UPH, 0x05)
#endif
#define     HSL_UPH_DPDL_ESSENTIAL              M_FrHslSubGroupDefine(HSL_UPH_DPDL, HslPriority_0)
#define     HSL_UPH_DPDL_DATA                   M_FrHslSubGroupDefine(HSL_UPH_DPDL, HslPriority_2)
#define     HSL_UPH_DPDL_MISC                   M_FrHslSubGroupDefine(HSL_UPH_DPDL, HslPriority_7)

/**** group HSL_UPH_TRXC ****/
#ifndef     HSL_UPH_TRXC
# define    HSL_UPH_TRXC                        M_FrHslGroupDefine(HSL_UPH, 0x06)
#endif
#define     HSL_UPH_TRXC_ESSENTIAL              M_FrHslSubGroupDefine(HSL_UPH_TRXC, HslPriority_0)
#define     HSL_UPH_TRXC_CM                     M_FrHslSubGroupDefine(HSL_UPH_TRXC, HslPriority_3)
#define     HSL_UPH_TRXC_HSD                    M_FrHslSubGroupDefine(HSL_UPH_TRXC, HslPriority_2)
#define     HSL_UPH_TRXC_MISC                   M_FrHslSubGroupDefine(HSL_UPH_TRXC, HslPriority_7)

/**** group HSL_UPH_DLC ****/
#ifndef     HSL_UPH_DLC
# define    HSL_UPH_DLC                         M_FrHslGroupDefine(HSL_UPH, 0x07)
#endif
#define     HSL_UPH_DLC_ESSENTIAL               M_FrHslSubGroupDefine(HSL_UPH_DLC, HslPriority_0)
#define     HSL_UPH_DLC_AICH                    M_FrHslSubGroupDefine(HSL_UPH_DLC, HslPriority_2)
#define     HSL_UPH_DLC_SCCPCH                  M_FrHslSubGroupDefine(HSL_UPH_DLC, HslPriority_2)
#define     HSL_UPH_DLC_COMBINER                M_FrHslSubGroupDefine(HSL_UPH_DLC, HslPriority_2)
#define     HSL_UPH_DLC_DPDH_STT                M_FrHslSubGroupDefine(HSL_UPH_DLC, HslPriority_2)
#define     HSL_UPH_DLC_MISC                    M_FrHslSubGroupDefine(HSL_UPH_DLC, HslPriority_7)

/**** group HSL_UPH_DPUL ****/
#ifndef     HSL_UPH_DPUL
# define    HSL_UPH_DPUL                        M_FrHslGroupDefine(HSL_UPH, 0x08)
#endif
#define     HSL_UPH_DPUL_ESSENTIAL              M_FrHslSubGroupDefine(HSL_UPH_DPUL, HslPriority_0)
#define     HSL_UPH_DPUL_MISC                   M_FrHslSubGroupDefine(HSL_UPH_DPUL, HslPriority_7)

/**** group HSL_UPH_PTH ****/
#ifndef     HSL_UPH_PTH
# define    HSL_UPH_PTH                         M_FrHslGroupDefine(HSL_UPH, 0x09)
#endif
#define     HSL_UPH_PTH_ESSENTIAL               M_FrHslSubGroupDefine(HSL_UPH_PTH, HslPriority_0)
#define     HSL_UPH_PTH_MISC                    M_FrHslSubGroupDefine(HSL_UPH_PTH, HslPriority_7)

/**** group HSL_UPH_FCS ****/
#ifndef     HSL_UPH_FCS
# define    HSL_UPH_FCS                         M_FrHslGroupDefine(HSL_UPH, 0x0A)
#endif
#define     HSL_UPH_FCS_ESSENTIAL               M_FrHslSubGroupDefine(HSL_UPH_FCS, HslPriority_0)
#define     HSL_UPH_FCS_CPICH_VE                M_FrHslSubGroupDefine(HSL_UPH_FCS, HslPriority_2)
#define     HSL_UPH_FCS_MISC                    M_FrHslSubGroupDefine(HSL_UPH_FCS, HslPriority_7)

/**** group HSL_UPH_HSD ****/
#ifndef     HSL_UPH_HSD
# define    HSL_UPH_HSD                         M_FrHslGroupDefine(HSL_UPH, 0x0B)
#endif
#define     HSL_UPH_HSD_ESSENTIAL               M_FrHslSubGroupDefine(HSL_UPH_HSD, HslPriority_0)
#define     HSL_UPH_HSD_DMA                     M_FrHslSubGroupDefine(HSL_UPH_HSD, HslPriority_1)
#define     HSL_UPH_HSD_UL                      M_FrHslSubGroupDefine(HSL_UPH_HSD, HslPriority_1)
#define     HSL_UPH_HSD_DL                      M_FrHslSubGroupDefine(HSL_UPH_HSD, HslPriority_1)
#define     HSL_UPH_HSD_MISC                    M_FrHslSubGroupDefine(HSL_UPH_HSD, HslPriority_1)

/**** group HSL_UPH_MISC ****/
#ifndef     HSL_UPH_MISC
# define    HSL_UPH_MISC                        M_FrHslGroupDefine(HSL_UPH, 0x0C)
#endif
#define     HSL_UPH_MISC_MISC                   M_FrHslSubGroupDefine(HSL_UPH_MISC, HslPriority_7)

/**** group HSL_UPH_HSU ****/
#ifndef     HSL_UPH_HSU
# define    HSL_UPH_HSU                         M_FrHslGroupDefine(HSL_UPH, 0x0D)
#endif

#define     HSL_UPH_HSU_ESSENTIAL               M_FrHslSubGroupDefine(HSL_UPH_HSU, HslPriority_0)
#define     HSL_UPH_HSU_VERBOSE                 M_FrHslSubGroupDefine(HSL_UPH_HSU, HslPriority_1)
#define     HSL_UPH_HSU_BRIEF                   M_FrHslSubGroupDefine(HSL_UPH_HSU, HslPriority_3)
#define     HSL_UPH_HSU_MISC                    M_FrHslSubGroupDefine(HSL_UPH_HSU, HslPriority_7)

/**** group HSL_UPH_MARGIN ****/
#ifndef     HSL_UPH_MARGIN
# define    HSL_UPH_MARGIN                      M_FrHslGroupDefine(HSL_UPH, 0x0E)
#endif

#define     HSL_UPH_MARGIN_ESSENTIAL            M_FrHslSubGroupDefine(HSL_UPH_MARGIN, HslPriority_0)
#define     HSL_UPH_MARGIN_MISC                 M_FrHslSubGroupDefine(HSL_UPH_MARGIN, HslPriority_7)

/**@}*//* HSL_UPH_GROUP */

/**
 * \defgroup HSL_UL1_GROUP
 * @{
 */

/** U1 RF */
#ifndef     HSL_UL1_RF
# define    HSL_UL1_RF                          M_FrHslGroupDefine(HSL_U1, 0x01)
#endif
#define     HSL_UL1_RF_ESSENTIAL                M_FrHslSubGroupDefine(HSL_UL1_RF, HslPriority_0)
#define     HSL_UL1_RF_MODE_CHANGE              M_FrHslSubGroupDefine(HSL_UL1_RF, HslPriority_2)

/** U1 MSG */
#ifndef     HSL_U1_MSG
# define    HSL_U1_MSG                         M_FrHslGroupDefine(HSL_U1, 0x02)
#endif
#define     HSL_U1_MSG_ESSENTIAL               M_FrHslSubGroupDefine(HSL_U1_MSG, HslPriority_0)
#define     HSL_U1_MSG_UPHY_SIG                M_FrHslSubGroupDefine(HSL_U1_MSG, HslPriority_7)
#define     HSL_U1_MSG_MISC                    M_FrHslSubGroupDefine(HSL_U1_MSG, HslPriority_7)

/**@}*//* HSL_UL1_GROUP */

/**
 * \defgroup HSL_CL_GROUP
 * @{
 */

/** CL UART */
#ifndef     HSL_CL_UART
# define    HSL_CL_UART                        M_FrHslGroupDefine(HSL_CL, 0x01)
#endif

#define     HSL_CL_UART_CONFIG                 M_FrHslSubGroupDefine(HSL_CL_UART, HslPriority_3)
#define     HSL_CL_UART_RX                     M_FrHslSubGroupDefine(HSL_CL_UART, HslPriority_3)
#define     HSL_CL_UART_TX                     M_FrHslSubGroupDefine(HSL_CL_UART, HslPriority_3)
#define     HSL_CL_UART_DATA_COMMON            M_FrHslSubGroupDefine(HSL_CL_UART, HslPriority_3)

/** CL MAIN */
#ifndef     HSL_CL_MAIN
# define    HSL_CL_MAIN                        M_FrHslGroupDefine(HSL_CL, 0x02)
#endif

#define     HSL_CL_MAIN_MSG_REC                M_FrHslSubGroupDefine(HSL_CL_MAIN, HslPriority_3)
#define     HSL_CL_MAIN_CTRL                   M_FrHslSubGroupDefine(HSL_CL_MAIN, HslPriority_3)

/**@}*//* HSL_CL_GROUP */

/**
 * \defgroup HSL_N1_GROUP
 * @{
 */
#if !defined (NBIOT_TRACE_FILTER)
/**** group HSL_N1_AFC ****/
#ifndef     HSL_N1_AFC
# define    HSL_N1_AFC                          M_FrHslGroupDefine(HSL_N1, 0x01)
#endif
#define     HSL_N1_AFC_BASIC                    M_FrHslSubGroupDefine(HSL_N1_AFC, HslPriority_1)
#define     HSL_N1_AFC_EXTENDED                 M_FrHslSubGroupDefine(HSL_N1_AFC, HslPriority_5)

/**** group HSL_N1_NE ****/
#ifndef     HSL_N1_NE
# define    HSL_N1_NE                           M_FrHslGroupDefine(HSL_N1, 0x02)
#endif
#define     HSL_N1_NE_BASIC                     M_FrHslSubGroupDefine(HSL_N1_NE, HslPriority_1)
#define     HSL_N1_NE_EXTENDED                  M_FrHslSubGroupDefine(HSL_N1_NE, HslPriority_5)

/**** group HSL_N1_MD ****/
#ifndef     HSL_N1_MD
# define    HSL_N1_MD                           M_FrHslGroupDefine(HSL_N1, 0x03)
#endif
#define     HSL_N1_MD_BASIC                     M_FrHslSubGroupDefine(HSL_N1_MD, HslPriority_1)
#define     HSL_N1_MD_EXTENDED                  M_FrHslSubGroupDefine(HSL_N1_MD, HslPriority_5)

/**** group HSL_N1_FWS ****/
#ifndef     HSL_N1_FWS
# define    HSL_N1_FWS                          M_FrHslGroupDefine(HSL_N1, 0x04)
#endif
#define     HSL_N1_FWS_BASIC                    M_FrHslSubGroupDefine(HSL_N1_FWS, HslPriority_1)
#define     HSL_N1_FWS_EXTENDED                 M_FrHslSubGroupDefine(HSL_N1_FWS, HslPriority_5)

/**** group HSL_N1_CS ****/
#ifndef     HSL_N1_CS
# define    HSL_N1_CS                           M_FrHslGroupDefine(HSL_N1, 0x05)
#endif
#define     HSL_N1_CS_BASIC                     M_FrHslSubGroupDefine(HSL_N1_CS, HslPriority_1)
#define     HSL_N1_CS_EXTENDED                  M_FrHslSubGroupDefine(HSL_N1_CS, HslPriority_5)

/**** group HSL_N1_MIB ****/
#ifndef     HSL_N1_MIB
# define    HSL_N1_MIB                          M_FrHslGroupDefine(HSL_N1, 0x06)
#endif
#define     HSL_N1_MIB_BASIC                    M_FrHslSubGroupDefine(HSL_N1_MIB, HslPriority_1)

/**** group HSL_N1_SIB1 ****/
#ifndef     HSL_N1_SIB1
# define    HSL_N1_SIB1                         M_FrHslGroupDefine(HSL_N1, 0x07)
#endif
#define     HSL_N1_SIB1_BASIC                   M_FrHslSubGroupDefine(HSL_N1_SIB1, HslPriority_1)

/**** group HSL_N1_SIBN ****/
#ifndef     HSL_N1_SIBN
# define    HSL_N1_SIBN                         M_FrHslGroupDefine(HSL_N1, 0x08)
#endif
#define     HSL_N1_SIBN_BASIC                   M_FrHslSubGroupDefine(HSL_N1_SIBN, HslPriority_1)
#define    	HSL_N1_SIBN_EXTENDED                M_FrHslSubGroupDefine(HSL_N1_SIBN, HslPriority_1)

/**** group HSL_N1_NPDCCH ****/
#ifndef     HSL_N1_NPDCCH
# define    HSL_N1_NPDCCH                       M_FrHslGroupDefine(HSL_N1, 0x09)
#endif
#define     HSL_N1_NPDCCH_BASIC                 M_FrHslSubGroupDefine(HSL_N1_NPDCCH, HslPriority_1)
#define     HSL_N1_NPDCCH_EXTENDED              M_FrHslSubGroupDefine(HSL_N1_NPDCCH, HslPriority_1)

/**** group HSL_N1_NPDSCH ****/
#ifndef     HSL_N1_NPDSCH
# define    HSL_N1_NPDSCH                       M_FrHslGroupDefine(HSL_N1, 0x0A)
#endif
#define     HSL_N1_NPDSCH_BASIC                 M_FrHslSubGroupDefine(HSL_N1_NPDSCH, HslPriority_1)
#define     HSL_N1_NPDSCH_EXTENDED              M_FrHslSubGroupDefine(HSL_N1_NPDSCH, HslPriority_1)

/**** group HSL_N1_CM ****/
#ifndef     HSL_N1_CM
# define    HSL_N1_CM                           M_FrHslGroupDefine(HSL_N1, 0x0B)
#endif
#define     N1_TRACE_GRP_CM_SQ                  M_FrHslSubGroupDefine(HSL_N1_CM, HslPriority_1)
#define     N1_TRACE_GRP_CM_ALG                 M_FrHslSubGroupDefine(HSL_N1_CM, HslPriority_1)
#define     N1_TRACE_GRP_CM_ALG_EXTENDED        M_FrHslSubGroupDefine(HSL_N1_CM, HslPriority_7)

/**** group HSL_N1_DSP_CTRL ****/
#ifndef     HSL_N1_DSP_CTRL
# define    HSL_N1_DSP_CTRL                     M_FrHslGroupDefine(HSL_N1, 0x0C)
#endif
#define     HSL_N1_DSP_BOOT                     M_FrHslSubGroupDefine(HSL_N1_DSP_CTRL, HslPriority_1)
#define     HSL_N1_DSP_MEMORY_DUMP              M_FrHslSubGroupDefine(HSL_N1_DSP_CTRL, HslPriority_1)
#define     HSL_N1_DSP_MSG_BASIC                M_FrHslSubGroupDefine(HSL_N1_DSP_CTRL, HslPriority_1)
#define     HSL_N1_DSP_MSG_EXTENDED             M_FrHslSubGroupDefine(HSL_N1_DSP_CTRL, HslPriority_7)
#define     HSL_N1_DSP_RPT_BASIC                M_FrHslSubGroupDefine(HSL_N1_DSP_CTRL, HslPriority_1)
#define     DSP_TRACE_GRP_EVENT                 M_FrHslSubGroupDefine(HSL_N1_DSP_CTRL, HslPriority_1)
#define     DSP_TRACE_GRP_HW                    M_FrHslSubGroupDefine(HSL_N1_DSP_CTRL, HslPriority_1)
#define     DSP_TRACE_GRP_CM                    M_FrHslSubGroupDefine(HSL_N1_DSP_CTRL, HslPriority_1)
#define     DSP_TRACE_GRP_OTDOA                 M_FrHslSubGroupDefine(HSL_N1_DSP_CTRL, HslPriority_1)
#define     DSP_TRACE_GRP_ALG                   M_FrHslSubGroupDefine(HSL_N1_DSP_CTRL, HslPriority_1)
#define     DSP_TRACE_GRP_DFE                   M_FrHslSubGroupDefine(HSL_N1_DSP_CTRL, HslPriority_1)
#define     DSP_TRACE_GRP_TD_BUF                M_FrHslSubGroupDefine(HSL_N1_DSP_CTRL, HslPriority_1)
#define     DSP_TRACE_GRP_NBI                   M_FrHslSubGroupDefine(HSL_N1_DSP_CTRL, HslPriority_1)

/**** group HSL_N1_SYS ****/
#ifndef     HSL_N1_SYS
# define    HSL_N1_SYS                          M_FrHslGroupDefine(HSL_N1, 0x0D)
#endif
#define     HSL_N1_SYS_TIME_BASIC               M_FrHslSubGroupDefine(HSL_N1_SYS, HslPriority_1)
#define     HSL_N1_SYS_MAIN_BASIC               M_FrHslSubGroupDefine(HSL_N1_SYS, HslPriority_1)
#define     HSL_N1_SYS_FSE_BASIC                M_FrHslSubGroupDefine(HSL_N1_SYS, HslPriority_1)
#define     HSL_N1_SYS_FSE_EXTENDED             M_FrHslSubGroupDefine(HSL_N1_SYS, HslPriority_5)
#define     HSL_N1_SYS_FSE_FRAMEWORK            M_FrHslSubGroupDefine(HSL_N1_SYS, HslPriority_5)
#define     HSL_N1_SYS_SQ_BASIC                 M_FrHslSubGroupDefine(HSL_N1_SYS, HslPriority_1)
#define     HSL_N1_SYS_SQ_EXTENDED              M_FrHslSubGroupDefine(HSL_N1_SYS, HslPriority_5)
#define     HSL_N1_SYS_SIG_INT_Q_FRAMEWORK      M_FrHslSubGroupDefine(HSL_N1_SYS, HslPriority_5)

/**** group HSL_N1_STH ****/
#ifndef     HSL_N1_STH
# define    HSL_N1_STH                          M_FrHslGroupDefine(HSL_N1, 0x0E)
#endif
#define     HSL_N1_STH_MAIN                     M_FrHslSubGroupDefine(HSL_N1_STH, HslPriority_1)
#define     HSL_N1_STH_SS                       M_FrHslSubGroupDefine(HSL_N1_STH, HslPriority_1)
#define     HSL_N1_STH_FRAMEWORK                M_FrHslSubGroupDefine(HSL_N1_STH, HslPriority_5)
#define     HSL_N1_STH_BG_PLMN_SEARCH_BASIC     M_FrHslSubGroupDefine(HSL_N1_STH, HslPriority_1)
#define     HSL_N1_STH_BG_PLMN_SEARCH_EXTENDED  M_FrHslSubGroupDefine(HSL_N1_STH, HslPriority_5)

/**** group HSL_N1_NPDXXH ****/
#ifndef     HSL_N1_NPDXXH
# define    HSL_N1_NPDXXH                       M_FrHslGroupDefine(HSL_N1, 0x0F)
#endif
#define     HSL_N1_NPDXXH_BASIC                 M_FrHslSubGroupDefine(HSL_N1_NPDXXH, HslPriority_1)
#define     HSL_N1_NPDXXH_EXTENDED              M_FrHslSubGroupDefine(HSL_N1_NPDXXH, HslPriority_5)

/**** group HSL_N1_MISC ****/
#ifndef     HSL_N1_MISC
# define    HSL_N1_MISC                         M_FrHslGroupDefine(HSL_N1, 0x10)
#endif
#define     HSL_N1_MODEM_BUS_ERROR              M_FrHslSubGroupDefine(HSL_N1_MISC, HslPriority_0)
#define     HSL_N1_HW_TEST                      M_FrHslSubGroupDefine(HSL_N1_MISC, HslPriority_0)
#define     HSL_N1_NVDM_ERROR                   M_FrHslSubGroupDefine(HSL_N1_MISC, HslPriority_0)
#define     HSL_N1_SIGNAL_LEVEL_IND             M_FrHslSubGroupDefine(HSL_N1_MISC, HslPriority_0)
#define     HSL_N1_INFO_CNF                     M_FrHslSubGroupDefine(HSL_N1_MISC, HslPriority_0)
#define     HSL_N1_ECID_MEAS_CNF                M_FrHslSubGroupDefine(HSL_N1_MISC, HslPriority_0)
#define     HSL_N1_AGC_FREEZE                   M_FrHslSubGroupDefine(HSL_N1_MISC, HslPriority_0)
#define     HSL_N1_ML1S                         M_FrHslSubGroupDefine(HSL_N1_MISC, HslPriority_0)
#define     HSL_N1_CDS                          M_FrHslSubGroupDefine(HSL_N1_MISC, HslPriority_0)

/**** group HSL_N1_SCM ****/
#ifndef     HSL_N1_SCM
# define    HSL_N1_SCM                          M_FrHslGroupDefine(HSL_N1, 0x11)
#endif
#define     HSL_N1_SCM_BASIC                    M_FrHslSubGroupDefine(HSL_N1_SCM, HslPriority_1)
#define     HSL_N1_SLP_DRV                      M_FrHslSubGroupDefine(HSL_N1_SCM, HslPriority_1)
#define     HSL_N1_SLP_DRV_EXT                  M_FrHslSubGroupDefine(HSL_N1_SCM, HslPriority_5)
#define     HSL_N1_SLP_DRV_DEBUG                M_FrHslSubGroupDefine(HSL_N1_SCM, HslPriority_5)
#define     HSL_N1_SCM_FRAMEWORK                M_FrHslSubGroupDefine(HSL_N1_SCM, HslPriority_5)

/**** group HSL_N1_HW_DRV ****/
#ifndef     HSL_N1_HW_DRV
# define    HSL_N1_HW_DRV                       M_FrHslGroupDefine(HSL_N1, 0x12)
#endif
#define     HSL_N1_HW_BRP_BASIC                 M_FrHslSubGroupDefine(HSL_N1_HW_DRV, HslPriority_1)
#define     HSL_N1_HW_CLOCK_CTRL_BASIC          M_FrHslSubGroupDefine(HSL_N1_HW_DRV, HslPriority_1)
#define     HSL_N1_HW_CLOCK_CTRL_DEBUG          M_FrHslSubGroupDefine(HSL_N1_HW_DRV, HslPriority_5)
#define     HSL_N1_HW_PHY_TIMER_BASIC           M_FrHslSubGroupDefine(HSL_N1_HW_DRV, HslPriority_1)
#define     HSL_N1_HW_PHY_TIMER_EXTENDED        M_FrHslSubGroupDefine(HSL_N1_HW_DRV, HslPriority_1)
#define     HSL_N1_HW_T2F_F2T_BASIC             M_FrHslSubGroupDefine(HSL_N1_HW_DRV, HslPriority_1)
#define     HSL_N1_HW_REGISTERS_DUMP            M_FrHslSubGroupDefine(HSL_N1_HW_DRV, HslPriority_1)

/**** group HSL_N1_TX_MAN ****/
#ifndef     HSL_N1_TX_MAN
# define    HSL_N1_TX_MAN                      M_FrHslGroupDefine(HSL_N1, 0x13)
#endif
#define     HSL_N1_TX_MAN_MAIN                 M_FrHslSubGroupDefine(HSL_N1_TX_MAN, HslPriority_1)
#define     HSL_N1_TX_MAN_PWR                  M_FrHslSubGroupDefine(HSL_N1_TX_MAN, HslPriority_1)
#define     HSL_N1_TX_MAN_FRAMEWORK            M_FrHslSubGroupDefine(HSL_N1_TX_MAN, HslPriority_5)

/**** group HSL_N1_RF_MAN ****/
#ifndef     HSL_N1_RF_MAN
# define    HSL_N1_RF_MAN                       M_FrHslGroupDefine(HSL_N1, 0x14)
#endif
#define     HSL_N1_RF_MAN_BASIC                 M_FrHslSubGroupDefine(HSL_N1_RF_MAN, HslPriority_1)
#define     HSL_N1_RF_FHC                       M_FrHslSubGroupDefine(HSL_N1_RF_MAN, HslPriority_1)
#define     HSL_N1_RF_AGC                       M_FrHslSubGroupDefine(HSL_N1_RF_MAN, HslPriority_1)
#define     HSL_N1_RF_DFE                       M_FrHslSubGroupDefine(HSL_N1_RF_MAN, HslPriority_1)
#define     HSL_N1_RF_SEQUENCER                 M_FrHslSubGroupDefine(HSL_N1_RF_MAN, HslPriority_1)
#define     HSL_N1_RF_SEQUENCER_RX              M_FrHslSubGroupDefine(HSL_N1_RF_MAN, HslPriority_1)
#define     HSL_N1_RF_SEQUENCER_TX              M_FrHslSubGroupDefine(HSL_N1_RF_MAN, HslPriority_1)
#define     HSL_N1_RF_PORT_LOGGER               M_FrHslSubGroupDefine(HSL_N1_RF_MAN, HslPriority_1)
#define     HSL_N1_RF_MAN_FRAMEWORK             M_FrHslSubGroupDefine(HSL_N1_RF_MAN, HslPriority_5)
#define     HSL_N1_RF_MAN_EXTENDED              M_FrHslSubGroupDefine(HSL_N1_RF_MAN, HslPriority_5)

/**** group HSL_N1_MSG ****/
#ifndef     HSL_N1_MSG
# define    HSL_N1_MSG                          M_FrHslGroupDefine(HSL_N1, 0x15)
#endif
#define     HSL_N1_MSG_RX_BASIC                 M_FrHslSubGroupDefine(HSL_N1_MSG, HslPriority_1)
#define     HSL_N1_MSG_RX_EXTENDED              M_FrHslSubGroupDefine(HSL_N1_MSG, HslPriority_5)

/**** group HSL_N1_RX_DH ****/
#ifndef     HSL_N1_RX_DH
# define    HSL_N1_RX_DH                        M_FrHslGroupDefine(HSL_N1, 0x16)
#endif
#define     HSL_N1_RX_DH_BASIC                  M_FrHslSubGroupDefine(HSL_N1_RX_DH, HslPriority_1)
#define     HSL_N1_RX_DH_EXTENDED               M_FrHslSubGroupDefine(HSL_N1_RX_DH, HslPriority_5)

/**** group HSL_N1_SFH ****/
#ifndef     HSL_N1_SFH
# define    HSL_N1_SFH                          M_FrHslGroupDefine(HSL_N1, 0x17)
#endif
#define     HSL_N1_SFH_BASIC                    M_FrHslSubGroupDefine(HSL_N1_SFH, HslPriority_1)
#define     HSL_N1_SFH_FRAMEWORK                M_FrHslSubGroupDefine(HSL_N1_SFH, HslPriority_5)

/**** group HSL_N1_COSIM ****/
#ifndef     HSL_N1_COSIM
# define    HSL_N1_COSIM                        M_FrHslGroupDefine(HSL_N1, 0x18)
#endif
#define     HSL_N1_COSIM_BASIC                  M_FrHslSubGroupDefine(HSL_N1_COSIM, HslPriority_1)
#define     HSL_N1_COSIM_EXTENDED               M_FrHslSubGroupDefine(HSL_N1_COSIM, HslPriority_5)

/**** group HSL_N1_RLM ****/
#ifndef     HSL_N1_RLM
# define    HSL_N1_RLM                          M_FrHslGroupDefine(HSL_N1, 0x19)
#endif
#define     N1_TRACE_GRP_RLM_SQ                 M_FrHslSubGroupDefine(HSL_N1_RLM, HslPriority_1)
#define     N1_TRACE_GRP_RLM_ALG                M_FrHslSubGroupDefine(HSL_N1_RLM, HslPriority_1)

/**** group HSL_N1_NPUSCH ****/
#ifndef     HSL_N1_NPUSCH
# define    HSL_N1_NPUSCH                       M_FrHslGroupDefine(HSL_N1, 0x1A)
#endif
#define     HSL_N1_NPUSCH_BASIC                 M_FrHslSubGroupDefine(HSL_N1_NPUSCH, HslPriority_1)
#define     HSL_N1_NPUSCH_EXTENDED              M_FrHslSubGroupDefine(HSL_N1_NPUSCH, HslPriority_5)

/**** group HSL_N1_NPRACH ****/
#ifndef     HSL_N1_NPRACH
# define    HSL_N1_NPRACH                       M_FrHslGroupDefine(HSL_N1, 0x1B)
#endif
#define     HSL_N1_NPRACH_BASIC                 M_FrHslSubGroupDefine(HSL_N1_NPRACH, HslPriority_1)
#define     HSL_N1_NPRACH_EXTENDED              M_FrHslSubGroupDefine(HSL_N1_NPRACH, HslPriority_5)

/**** group HSL_N1_WARMUP ****/
#ifndef     HSL_N1_WARMUP
# define    HSL_N1_WARMUP                       M_FrHslGroupDefine(HSL_N1, 0x1C)
#endif
#define     HSL_N1_WARMUP_BASIC                 M_FrHslSubGroupDefine(HSL_N1_WARMUP, HslPriority_1)

/**** group HSL_N1_TEMP ****/
#ifndef     N1_TRACE_GRP_TEMP
# define    N1_TRACE_GRP_TEMP                   M_FrHslGroupDefine(HSL_N1, 0x1D)
#endif
#define     N1_TRACE_GRP_TEMP_DEBUG             M_FrHslSubGroupDefine(N1_TRACE_GRP_TEMP, HslPriority_1)

/**** group HSL_N1_CELLDB ****/
#ifndef     HSL_N1_CELLDB
# define    HSL_N1_CELLDB                       M_FrHslGroupDefine(HSL_N1, 0x1E)
#endif
#define     HSL_N1_CELLDB_BASIC                 M_FrHslSubGroupDefine(HSL_N1_CELLDB, HslPriority_1)
#define     HSL_N1_CELLDB_EXTENDED              M_FrHslSubGroupDefine(HSL_N1_CELLDB, HslPriority_5)

/**** group HSL_N1_RXM ****/
#ifndef     HSL_N1_RXM
# define    HSL_N1_RXM                          M_FrHslGroupDefine(HSL_N1, 0x1F)
#endif
#define     HSL_N1_RXM_BASIC                    M_FrHslSubGroupDefine(HSL_N1_RXM, HslPriority_1)

/**** group HSL_N1_L1TST ****/
#ifndef     HSL_N1_L1TST
# define    HSL_N1_L1TST                        M_FrHslGroupDefine(HSL_N1, 0x20)
#endif
#define     HSL_N1_L1TST_BASIC                  M_FrHslSubGroupDefine(HSL_N1_L1TST, HslPriority_1)

/**** group HSL_N1_OTDOA ****/
#ifndef     HSL_N1_OTDOA
# define    HSL_N1_OTDOA                        M_FrHslGroupDefine(HSL_N1, 0x21)
#endif
#define     HSL_N1_OTDOA_BASIC                  M_FrHslSubGroupDefine(HSL_N1_OTDOA, HslPriority_1)
#define     HSL_N1_OTDOA_EXTENDED               M_FrHslSubGroupDefine(HSL_N1_OTDOA, HslPriority_5)

/**** group HSL_N1_IDC ****/
#ifndef     HSL_N1_IDC
# define    HSL_N1_IDC                          M_FrHslGroupDefine(HSL_N1, 0x22)
#endif
#define     HSL_N1_IDC_BASIC                    M_FrHslSubGroupDefine(HSL_N1_IDC, HslPriority_1)

#endif /* NBIOT_TRACE_FILTER */

/**@}*//* HSL_N1_GROUP */


/**@}*//* HSL Groups */

#endif /* FRHSLGRP_H_ */
