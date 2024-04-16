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
 * File Description:
 *
 *    NB-IoT trace definitions
 *
 **************************************************************************/

#if !defined (NBIOT_TRACE_H)
#define       NBIOT_TRACE_H

/***************************************************************************
* Include Files
***************************************************************************/
#include <uls_api.h>
#include <frhsl.h>

/***************************************************************************
 * Constants
 ***************************************************************************/

/* Action values to direct execution of nbiot_trace_group_filter_set() function */
#if defined(NBIOT_TRACE_FILTER)
#define HSL_FILTERACTION_CURRENT_SET    (0)
#define HSL_FILTERACTION_SET_NEW        (1)
#define HSL_FILTERACTION_SET_ALL        (2)
#define HSL_FILTERACTION_CLEAR_ALL      (3)
#define HSL_FILTERACTION_STARTUP        (4)
#endif

#define ERRC_TRACE      54000
#define ESM_TRACE       54200
#define DSP_TRACE       54400
#define SYS_TRACE       54500
#define N1_TRACE        54900
#define USIM_TRACE      55700
#define EL2_TRACE       56100
#define SIMA_TRACE      56900
#define NBMM_TRACE      56910
#define ABMM_TRACE      57000
#define EMM_TRACE       57200
#define SIMDRV_TRACE    57700
#define L4MM_TRACE      58100
#define DBM_TRACE       58120
#define MUX_TRACE       58300
#define ABPD_TRACE      58500
#define ABRPM_TRACE     58800

#define CONN_LAYER_TRACE    60000
#ifdef ENABLE_AP_BRIDGE_FEATURE
#define AP_BRIDGE_MD_TRACE  60300
#endif

#define SECDRV_TRACE    60500
#define ROHC_TRACE      60900
#define PSC_TRACE       61400
#define AS_TRACE        61600
#define LPP_TRACE       62000
#define LBS_TRACE       63000

#define APMDFAKESERVICE_TRACE  64000

#if defined (NBIOT_TRACE_FILTER)
typedef enum NbiotTraceGroupIdTag
{
    NBIOT_LEGACY_SUPPORT1 = 0,          /* legacy LG_DEBUG_1 definition */
    NBIOT_LEGACY_SUPPORT2,              /* legacy DEFAULT_GROUP definition */
    HSL_SYS_LOG_ERROR,                  /* System level traces for Fail cases */
    HSL_SYS_LOG_WARNING,                /* System level traces for User attention */
    HSL_SYS_LOG_BASIC,                  /* System level traces needed to follow system SW functionality */
    HSL_SYS_LOG_EXTENDED,               /* System level more detailed information */
    HSL_SYS_LOG_DEBUG,                  /* System level SW debugging traces */
    NBIOT_USIM_TRACE,                   /* Sim Manager trace */
    HSL_N1_AFC_BASIC,                   /* N1 AFC Basic traces */
    HSL_N1_AFC_EXTENDED,                /* N1 AFC Extended traces */
    HSL_N1_NE_BASIC,                    /* N1 NE Basic traces */
    HSL_N1_NE_EXTENDED,                 /* N1 NE Extended traces */
    HSL_N1_MD_BASIC,                    /* N1 MD Basic traces */
    HSL_N1_MD_EXTENDED,                 /* N1 MD Extended traces */
    HSL_N1_FWS_BASIC,                   /* N1 FWS Basic traces */
    HSL_N1_FWS_EXTENDED,                /* N1 FWS Extended traces */
    HSL_N1_WARMUP_BASIC,                /* N1 Warmup sequencer basic traces */
    HSL_N1_CS_BASIC,                    /* N1 CS Basic traces */
    HSL_N1_CS_EXTENDED,                 /* N1 CS Extended traces */
    HSL_N1_MIB_BASIC,                   /* N1 MIB sequencer basic traces */
    HSL_N1_SIB1_BASIC,                  /* N1 SIB1 sequencer basic traces */
    HSL_N1_SIBN_BASIC,                  /* N1 SIBN Basic traces */
    HSL_N1_SIBN_EXTENDED,               /* N1 SIBN Extended traces */
    HSL_N1_NPDCCH_BASIC,                /* N1 NPDCCH Basic traces */
    HSL_N1_NPDCCH_EXTENDED,             /* N1 NPDCCH Extended traces */
    HSL_N1_NPDSCH_BASIC,                /* N1 NPDSCH Basic traces */
    HSL_N1_NPDSCH_EXTENDED,             /* N1 NPDSCH Extended traces */
    HSL_N1_NPUSCH_BASIC,                /* N1 NPUSCH Basic traces */
    HSL_N1_NPUSCH_EXTENDED,             /* N1 NPUSCH Extended traces */
    HSL_N1_NPRACH_BASIC,                /* N1 NPRACH Basic traces */
    HSL_N1_NPRACH_EXTENDED,             /* N1 NPRACH Extended traces */
    HSL_N1_DSP_BOOT,                    /* N1 DSP CTRL Boot traces */
    HSL_N1_DSP_MEMORY_DUMP,             /* N1 DSP CTRL Memory Dump traces */
    HSL_N1_DSP_MSG_BASIC,               /* N1 DSP CTRL Msg Basic traces */
    HSL_N1_DSP_MSG_EXTENDED,            /* N1 DSP CTRL Msg Extended traces */
    HSL_N1_DSP_RPT_BASIC,               /* N1 DSP CTRL RPT Basic traces */
    HSL_N1_SYS_TIME_BASIC,              /* N1 SYS Time Basic traces */
    HSL_N1_SYS_MAIN_BASIC,              /* N1 SYS Main Basic traces */
    HSL_N1_SYS_FSE_BASIC,               /* N1 SYS FSE Basic traces */
    HSL_N1_SYS_FSE_EXTENDED,            /* N1 SYS FSE Extended traces */
    HSL_N1_SYS_FSE_FRAMEWORK,           /* N1 SYS FSE Framework traces */
    HSL_N1_SYS_SQ_BASIC,                /* N1 SYS SQ Basic traces */
    HSL_N1_SYS_SQ_EXTENDED,             /* N1 SYS SQ Extended traces */
    HSL_N1_SYS_SIG_INT_Q_FRAMEWORK,     /* N1 SYS Signal interrupt queue traces */
    HSL_N1_STH_MAIN,                    /* N1 STH Main traces */
    HSL_N1_STH_SS,                      /* N1 STH SS traces */
    HSL_N1_STH_FRAMEWORK,               /* N1 STH Framework traces */
    HSL_N1_STH_BG_PLMN_SEARCH_BASIC,    /* N1 STH BG PLMN search Basic traces */
    HSL_N1_STH_BG_PLMN_SEARCH_EXTENDED, /* N1 STH BG PLMN search Extended traces */
    HSL_N1_NPDXXH_BASIC,                /* N1 NPDXXH Basic traces */
    HSL_N1_NPDXXH_EXTENDED,             /* N1 NPDXXH Extended traces */
    HSL_N1_MODEM_BUS_ERROR,             /* N1 MISC Modem Bus Error */
    HSL_N1_HW_TEST,                     /* N1 MISC HW Test */
    HSL_N1_NVDM_ERROR,                  /* N1 MISC NVDM Access Error */
    HSL_N1_SIGNAL_LEVEL_IND,            /* N1 MISC Signal Level Indications */
    HSL_N1_INFO_CNF,                    /* N1 MISC Info Confirmations */
    HSL_N1_ECID_MEAS_CNF,               /* N1 MISC E-CID Meas Cnf */
    HSL_N1_AGC_FREEZE,                  /* N1 MISC AGC Freeze with L1 Test Message */
    HSL_N1_ML1S,                        /* N1 MISC ML1S */
    HSL_N1_CDS,                         /* N1 MISC CDS */
    HSL_N1_SCM_BASIC,                   /* N1 SCM Basic traces */
    HSL_N1_SLP_DRV,                     /* N1 SLP DRV traces */
    HSL_N1_SLP_DRV_EXT,                 /* N1 SLP DRV Extended traces */
    HSL_N1_SLP_DRV_DEBUG,               /* N1 SLP DRV Debug traces */
    HSL_N1_SCM_FRAMEWORK,               /* N1 SCM Framework traces */
    HSL_N1_HW_BRP_BASIC,                /* N1 HW BRP Basic traces */
    HSL_N1_HW_CLOCK_CTRL_BASIC,         /* N1 HW CLOCK CTRL Basic traces */
    HSL_N1_HW_CLOCK_CTRL_DEBUG,         /* N1 HW CLOCK CTRL Debug traces */
    HSL_N1_HW_PHY_TIMER_BASIC,          /* N1 HW PHY Timer Basic traces */
    HSL_N1_HW_PHY_TIMER_EXTENDED,       /* N1 HW PHY Timer Extended traces */
    HSL_N1_HW_T2F_F2T_BASIC,            /* N1 HW T2F F2T Basic traces */
    HSL_N1_HW_REGISTERS_DUMP,           /* N1 HW registers dump */
    HSL_N1_TX_MAN_MAIN,                 /* N1 TX_MAN Main traces */
    HSL_N1_TX_MAN_PWR,                  /* N1 TX_MAN PWR traces */
    HSL_N1_TX_MAN_FRAMEWORK,            /* N1 TX_MAN Framework traces */
    HSL_N1_RXM_BASIC,                   /* N1 RX manager basic traces */
    HSL_N1_RF_MAN_BASIC,                /* N1 RF_MAN Basic traces */
    HSL_N1_RF_FHC,                      /* N1 RF FHC traces */
    HSL_N1_RF_AGC,                      /* N1 RF AGC traces */
    HSL_N1_RF_DFE,                      /* N1 RF DFE traces */
    HSL_N1_RF_SEQUENCER,                /* N1 RF Sequencer traces */
    HSL_N1_RF_SEQUENCER_RX,             /* N1 RF Sequencer_RX traces */
    HSL_N1_RF_SEQUENCER_TX,             /* N1 RF Sequencer_TX traces */
    HSL_N1_RF_MAN_EXTENDED,             /* N1 RF Extended traces */
    HSL_N1_RF_PORT_LOGGER,              /* N1 RF Port Logger traces */
    HSL_N1_RF_MAN_FRAMEWORK,            /* N1 RF_MAN Framework traces */
    HSL_N1_MSG_RX_BASIC,                /* N1 MSG RX Basic traces */
    HSL_N1_MSG_RX_EXTENDED,             /* N1 MSG RX Extended traces */
    HSL_N1_RX_DH_BASIC,                 /* N1 RX_DH Basic traces */
    HSL_N1_RX_DH_EXTENDED,              /* N1 RX_DH Extended traces */
    HSL_N1_SFH_BASIC,                   /* N1 SFH Basic traces */
    HSL_N1_SFH_FRAMEWORK,               /* N1 SFH Framework traces */
    HSL_N1_COSIM_BASIC,                 /* N1 COSIM Basic traces */
    HSL_N1_COSIM_EXTENDED,              /* N1 COSIM Extended traces */
    HSL_N1_CELLDB_BASIC,                /* N1 Cell database basic traces */
    HSL_N1_CELLDB_EXTENDED,             /* N1 Cell database Extended traces */
    ABMM_TRACE_GROUP,                   /* L4 PLMN selection traces */
    ABPD_TRACE_GROUP,                   /* L4 Packet Data module traces */
    LPP_TRACE_GROUP,                    /* LPP Information traces */
    LBS_TRACE_GROUP,                    /* LBS Information traces */
    ESM_TRACE_GROUP,                    /* EPS Session management traces */
    EMM_TRACE_GROUP,                    /* EPS Mobility management traces */
    EMM_TRACE_GROUP_EXTERNAL,           /* EPS Mobility management traces, which can be delivered to customers */
    ESM_TRACE_GROUP_EXTERNAL,           /* EPS Session management traces, which can be delivered to customers */
    DBM_TRACE_GROUP,                    /* DBM Data Bearer Management traces */
    NBIOT_SIMDRV_TRACE,                 /* L1 SIM Driver trace group */
    ERRC_INFO,                          /* ERRC information traces */
    EL2_INFO,                           /* EL2 information traces */
    ROHC_INFO,                          /* ROHC information traces */
    MUX_TRACE_GROUP,                    /* MUX trace group */
    L4MM_TRACE_GROUP,                   /* L4MM trace group */
    N1_TRACE_GRP_CM_SQ,                 /* CM sequencer basic traces */
    N1_TRACE_GRP_CM_ALG,                /* CM algorithm traces */
    N1_TRACE_GRP_CM_ALG_EXTENDED,       /* CM algorithm extended traces */
    N1_TRACE_GRP_RLM_SQ,                /* RLM sequencer basic traces */
    N1_TRACE_GRP_RLM_ALG,               /* RLM algorithm traces */
    N1_TRACE_GRP_TEMP_DEBUG,            /* Temporary debug traces */
    HSL_N1_L1TST_BASIC,                 /* N1 L1TST/Calibration Test Mode Traces */
    HSL_N1_OTDOA_BASIC,                 /* OTDOA sequencer basic traces */
    HSL_N1_OTDOA_EXTENDED,              /* OTDOA sequencer extended traces */
    HSL_N1_IDC_BASIC,                   /* N1 IDC Test Mode Traces */
    PSC_INFO,                           /* PSC information traces */
    DSP_TRACE_GRP_EVENT,                /* DSP event traces. Please keep DSP trace groups sequential */
    DSP_TRACE_GRP_HW,                   /* DSP HW related traces */
    DSP_TRACE_GRP_CM,                   /* DSP Cell Measurement traces */
    DSP_TRACE_GRP_OTDOA,                /* DSP OTDOA traces */
    DSP_TRACE_GRP_ALG,                  /* DSP general algorithm traces */
    DSP_TRACE_GRP_DFE,                  /* DSP DFE traces */
    DSP_TRACE_GRP_TD_BUF,               /* DSP TD Buffer traces */
    DSP_TRACE_GRP_NBI,                  /* DSP Narrow Band Interference algorithm traces */
    CONN_LAYER_TRACE_GROUP,             /* All conn layer traces except data traces */
    CONN_LAYER_DATA_TRACE_GROUP,        /* Only conn layer data traces */
    NBMM_INFO,                          /* NBMM information traces */
    SIMA_TRACE_GROUP,                   /* SIMA trace group */
    USER_DATA_TRACE_GROUP,              /* User data traces */
    NBIOT_SECDRV_INFO,                  /* NBIOT security driver traces */
    APB_INFO,                           /* AP Bridge traces. */
    ABRPM_TRACE_GROUP,                  /* L4 Radio Policy Manager traces */
    NBIOT_TRACE_MAX_GROUP_VALUE
} NbiotTraceGroupId;
#endif

typedef enum
{
    TRACE_STATUS_UNINITIALIZED  = 0,
    TRACE_STATUS_DISCONNECT     = 1,       /* disconnection  */
    TRACE_STATUS_OFF            = 2,       /* power off & buffer flushed */
    TRACE_STATUS_INACTIVE       = 3,       /* clk off or power off */
    TRACE_STATUS_IDLE           = 4,
    TRACE_STATUS_FLUSHING       = 5,       /* flushing all data */
    TRACE_STATUS_MAX,
} trace_status_t;


/***************************************************************************
*  Macros
***************************************************************************/

#define BEGIN_TRACE_IDS(MODULE) \
    enum MODULE##_ENUM{ \
    MODULE##_begin=MODULE,

#define END_TRACE_IDS(MODULE) \
    MODULE##__end};

#define TRACE_DEF(TRACE_ID,TRACE_GROUP, STRING) \
    TRACE_ID,

#define H_WORD(Data)               (Data >> 16)
#define L_WORD(Data)               (Data & 0xFFFF)


#if !defined (NBIOT_TRACE_FILTER)

#define NBIOT_TRACE_STRING(TRACE_ID, TRACE_GROUP, STRINGPTR) \
    M_FrHslString((TRACE_ID), (TRACE_GROUP), "", (STRINGPTR))

#define NBIOT_TRACE0(TRACE_ID, TRACE_GROUP) \
    M_FrHslPrintf0((TRACE_ID), (TRACE_GROUP), "")

#define NBIOT_TRACE1(TRACE_ID, TRACE_GROUP, ARG1) \
    M_FrHslPrintf1((TRACE_ID), (TRACE_GROUP), "", (ARG1))

#define NBIOT_TRACE2(TRACE_ID, TRACE_GROUP, ARG1, ARG2) \
    M_FrHslPrintf2((TRACE_ID), (TRACE_GROUP), "", (ARG1), (ARG2))

#define NBIOT_TRACE3(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3) \
    M_FrHslPrintf3((TRACE_ID), (TRACE_GROUP), "", (ARG1), (ARG2), (ARG3))

#define NBIOT_TRACE4(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4) \
    M_FrHslPrintf4((TRACE_ID), (TRACE_GROUP), "", (ARG1), (ARG2), (ARG3), (ARG4))

#define NBIOT_TRACE5(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5) \
    M_FrHslPrintf5((TRACE_ID), (TRACE_GROUP), "", (ARG1), (ARG2), (ARG3), (ARG4), (ARG5))

#define NBIOT_TRACE6(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
    M_FrHslPrintf6((TRACE_ID), (TRACE_GROUP), "", (ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6))

#define NBIOT_TRACE7(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7) \
    M_FrHslPrintf7((TRACE_ID), (TRACE_GROUP), "", (ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6), (ARG7))

#define NBIOT_TRACE8(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8) \
    M_FrHslPrintf8((TRACE_ID), (TRACE_GROUP), "", (ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6), (ARG7), (ARG8))

#define NBIOT_TRACE9(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9) \
    M_FrHslPrintf9((TRACE_ID), (TRACE_GROUP), "", (ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6), (ARG7), (ARG8), (ARG9))

#define NBIOT_TRACE10(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10) \
    M_FrHslPrintf10((TRACE_ID), (TRACE_GROUP), "", (ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6), (ARG7), (ARG8), (ARG9), (ARG10))

#define NBIOT_TRACE11(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11) \
    M_FrHslPrintf11((TRACE_ID), (TRACE_GROUP), "", (ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6), (ARG7), (ARG8), (ARG9), (ARG10), (ARG11))

#define NBIOT_TRACE12(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12) \
    M_FrHslPrintf12((TRACE_ID), (TRACE_GROUP), "", (ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6), (ARG7), (ARG8), (ARG9), (ARG10), (ARG11), (ARG12))

#define NBIOT_TRACE13(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13) \
    M_FrHslPrintf13((TRACE_ID), (TRACE_GROUP), "", (ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6), (ARG7), (ARG8), (ARG9), (ARG10), (ARG11), (ARG12), (ARG13))

#define NBIOT_TRACE14(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14) \
    M_FrHslPrintf14((TRACE_ID), (TRACE_GROUP), "", (ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6), (ARG7), (ARG8), (ARG9), (ARG10), (ARG11), (ARG12), (ARG13), (ARG14))

#define NBIOT_TRACE15(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15) \
    M_FrHslPrintf15((TRACE_ID), (TRACE_GROUP), "", (ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6), (ARG7), (ARG8), (ARG9), (ARG10), (ARG11), (ARG12), (ARG13), (ARG14), (ARG15))

#define NBIOT_TRACE16(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15, ARG16) \
    M_FrHslPrintf16((TRACE_ID), (TRACE_GROUP), "", (ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6), (ARG7), (ARG8), (ARG9), (ARG10), (ARG11), (ARG12), (ARG13), (ARG14), (ARG15), (ARG16))

#define NBIOT_TRACE17(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15, ARG16, ARG17) \
    M_FrHslPrintf17((TRACE_ID), (TRACE_GROUP), "", (ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6), (ARG7), (ARG8), (ARG9), (ARG10), (ARG11), (ARG12), (ARG13), (ARG14), (ARG15), (ARG16), (ARG17))

#define NBIOT_TRACE18(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15, ARG16, ARG17, ARG18) \
    M_FrHslPrintf18((TRACE_ID), (TRACE_GROUP), "", (ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6), (ARG7), (ARG8), (ARG9), (ARG10), (ARG11), (ARG12), (ARG13), (ARG14), (ARG15), (ARG16), (ARG17), (ARG18))

#define NBIOT_TRACE19(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15, ARG16, ARG17, ARG18, ARG19) \
    M_FrHslPrintf19((TRACE_ID), (TRACE_GROUP), "", (ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6), (ARG7), (ARG8), (ARG9), (ARG10), (ARG11), (ARG12), (ARG13), (ARG14), (ARG15), (ARG16), (ARG17), (ARG18), (ARG19))

#define NBIOT_GROUP_ACTIVE(GrpId)   1

#else
/** New ones with group id use ******************************************************************/
extern uint8_t ULS_GroupFilter[];

/* -------- Macros to manipulate filters ------------- */
#define NBIOT_GROUP_ALLOW_TRACES(GrpNbr)                            \
        ( ULS_GroupFilter[(GrpNbr/8)] |= (1<<(GrpNbr%8)) )

#define NBIOT_GROUP_DISABLE_TRACES(GrpNbr)                          \
        ( ULS_GroupFilter[(GrpNbr/8)] &= ~(1<<(GrpNbr%8)) )

#define NBIOT_GROUP_ALLOW_ALL                                       \
        for(int c = NBIOT_TRACE_MAX_GROUP_VALUE/8; c >= 0; c--)     \
        {                                                           \
            ULS_GroupFilter[c] = 0xFF;                              \
        }

#define NBIOT_GROUP_DISABLE_ALL                                     \
        for(int c = NBIOT_TRACE_MAX_GROUP_VALUE/8; c >= 0; c--)     \
        {                                                           \
            ULS_GroupFilter[c] = 0x00;                              \
        }

#define TRACE_POWER_STATE()  (trace_status < TRACE_STATUS_IDLE ? trace_status_request(TRACE_STATUS_IDLE) :TRUE)

#define NBIOT_GROUP_ACTIVE(GrpId)                                                    \
        ((ULS_GroupFilter[(GrpId & 0xff)/8] & (1<<((GrpId & 0xff)%8))) != 0)
#ifdef USE_FILTER_PRECHECK_FUNCTION
#define NBIOT_TRACE_PRE_CHECK(GrpId)   nbiot_trace_pre_check(GrpId)
#else
#define NBIOT_TRACE_PRE_CHECK(GrpId)   (NBIOT_GROUP_ACTIVE(GrpId) && TRACE_POWER_STATE())
#endif //USE_FILTER_PRECHECK_FUNCTION


/* -------- Macros to write traces ------------- */
#if defined(ULS_FUNCTION_PROFILER)
#define NBIOT_TRACE_STRING(TRACE_GROUP, FORMAT, ...)
#define NBIOT_TRACE_ARRAY16(TRACE_ID, TRACE_GROUP, LENGTH, DATA)
#define NBIOT_TRACE_ARRAY32(TRACE_ID, TRACE_GROUP, LENGTH, DATA)
#define NBIOT_TRACE0(TRACE_ID, TRACE_GROUP)
#define NBIOT_TRACE1(TRACE_ID, TRACE_GROUP, ARG1)
#define NBIOT_TRACE2(TRACE_ID, TRACE_GROUP, ARG1, ARG2)
#define NBIOT_TRACE3(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3)
#define NBIOT_TRACE4(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4)
#define NBIOT_TRACE5(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5)
#define NBIOT_TRACE6(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6)
#define NBIOT_TRACE7(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7)
#define NBIOT_TRACE8(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8)
#define NBIOT_TRACE9(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9)
#define NBIOT_TRACE10(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10)
#define NBIOT_TRACE11(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11)
#define NBIOT_TRACE12(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12)
#define NBIOT_TRACE13(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13)
#define NBIOT_TRACE14(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14)
#define NBIOT_TRACE15(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15)
#define NBIOT_TRACE16(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15, ARG16)
#define NBIOT_TRACE17(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15, ARG16, ARG17)
#define NBIOT_TRACE18(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15, ARG16, ARG17, ARG18)
#define NBIOT_TRACE19(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15, ARG16, ARG17, ARG18, ARG19)

#else

#if 0
#define NBIOT_TRACE_STRING(TRACE_GROUP, FORMAT, args...) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            char buffer[50];  \
            snprintf(buffer, 50, FORMAT, ##args); \
            FrHslString(0x000A, buffer);  \
        }
#else
#define NBIOT_TRACE_STRING(TRACE_GROUP, FORMAT, ...) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            char buffer[50];  \
            snprintf(buffer, 50, FORMAT, __VA_ARGS__); \
            FrHslString(0x000A, buffer);  \
        }
#endif
#define NBIOT_TRACE_ARRAY16(TRACE_ID, TRACE_GROUP, LENGTH, DATA) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            M_FrHslInt16ArrayMsg(TRACE_ID, LENGTH, DATA ); \
        }
#define NBIOT_TRACE_ARRAY32(TRACE_ID, TRACE_GROUP, LENGTH, DATA) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            M_FrHslInt32ArrayMsg(TRACE_ID, LENGTH, DATA ); \
        }
#define NBIOT_TRACE0(TRACE_ID, TRACE_GROUP) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_1_DATA( TRACE_ID); \
        }

#define NBIOT_TRACE1(TRACE_ID, TRACE_GROUP, ARG1) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_2_DATA( TRACE_ID, ARG1); \
        }

#define NBIOT_TRACE2(TRACE_ID, TRACE_GROUP, ARG1, ARG2) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_3_DATA( TRACE_ID, ARG1, ARG2); \
        }

#define NBIOT_TRACE3(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_4_DATA( TRACE_ID, ARG1, ARG2, ARG3); \
        }

#define NBIOT_TRACE4(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_5_DATA( TRACE_ID, ARG1, ARG2, ARG3, ARG4); \
        }

#define NBIOT_TRACE5(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_6_DATA( TRACE_ID, ARG1, ARG2, ARG3, ARG4, ARG5);  \
        }

#define NBIOT_TRACE6(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_7_DATA( TRACE_ID, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6); \
        }

#define NBIOT_TRACE7(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_8_DATA( TRACE_ID, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7); \
        }

#define NBIOT_TRACE8(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_9_DATA( TRACE_ID, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8); \
        }

#define NBIOT_TRACE9(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
        ULS_LOG_10_DATA( TRACE_ID, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9); \
        }

#define NBIOT_TRACE10(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_11_DATA( TRACE_ID, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10); \
        }

#define NBIOT_TRACE11(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_12_DATA( TRACE_ID, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11); \
        }

#define NBIOT_TRACE12(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_13_DATA( TRACE_ID, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12); \
        }

#define NBIOT_TRACE13(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_14_DATA( TRACE_ID, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13); \
        }

#define NBIOT_TRACE14(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_15_DATA( TRACE_ID, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14); \
        }

#define NBIOT_TRACE15(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_16_DATA( TRACE_ID, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15); \
        }

#define NBIOT_TRACE16(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15, ARG16) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_17_DATA( TRACE_ID, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15, ARG16); \
        }

#define NBIOT_TRACE17(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15, ARG16, ARG17) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_18_DATA( TRACE_ID, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15, ARG16, ARG17); \
        }

#define NBIOT_TRACE18(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15, ARG16, ARG17, ARG18) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_19_DATA( TRACE_ID, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15, ARG16, ARG17, ARG18); \
        }

#define NBIOT_TRACE19(TRACE_ID, TRACE_GROUP, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15, ARG16, ARG17, ARG18, ARG19) \
        if( NBIOT_TRACE_PRE_CHECK(TRACE_GROUP) ) \
        { \
            ULS_LOG_20_DATA( TRACE_ID, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13, ARG14, ARG15, ARG16, ARG17, ARG18, ARG19); \
        }
#endif /* NBIOT_TRACE_FILTER */
#endif //ULS_FUNCTION_PROFILER

/***************************************************************************
 * Types
 ***************************************************************************/

/***************************************************************************
 * Typed Constants
 ***************************************************************************/

/***************************************************************************
 * Global Function Prototypes
 ***************************************************************************/

/***************************************************************************
 * Extern global varible or function
 ***************************************************************************/
extern volatile trace_status_t trace_status;

extern bool     is_mdsys_off_pd_ctrl_owner(bool flag);
extern uint32_t trace_status_request(trace_status_t next_state);

#endif /* NBIOT_TRACE_H */

/* END OF FILE */
