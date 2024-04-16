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
 * File Description:                                                      */

/** \file
 * 3G PS Signal Data Type definitions for the CMAC Interface
 **************************************************************************/

#if !defined(CMAC_SIG_H)
#define       CMAC_SIG_H

/***************************************************************************
* Nested Include Files
***************************************************************************/

#include <ups_typ.h>
#include <cphy_sig.h>
#include <uphy_sig.h>

/* DEVELOPMENT_VERSION */
# if defined(ON_PC) || defined(ON_TARGET_UNIT_TEST)
#  include <umac_sig.h>
# endif /*(ON_PC) || (ON_TARGET_UNIT_TEST)*/
/*DEVELOPMENT_VERSION*/

/** \defgroup SigCmac CMAC Signalling Interface
 * \ingroup Prd3gDmProtoStack
 * \ingroup Sig
 *
 * This section defines all the primitives exchanged on the control plane of
 * RRC and MAC, hereinafter called the CMAC interface.
 * CMAC interface is the interface between MAC and RRC as shown in this
 * see \ref PSInterfaceFigure
 *
 * All the primitives have been categorised based on the common purpose for
 * which they are exchanged. For example, all the Signals used for configuration
 * of Common Channels are put into <I>Signals used for configuration
 * of Common Channels</I> category.
 *
 * \subsection SigCmacParDesc Common Parameter Description
 * <B>Common Parameter Description</B>
 *
 * Many of the configuration primitives (Req and Cnf) include the following
 * parameters:
 *
 * <TABLE>
 * <TR><TD><B>Parameter Type</B></TD>    <TD><B>Name               </B></TD></TR>
 * <TR><TD>Int8                 </TD>    <TD>configIdentifier          </TD></TR>
 * <TR><TD>Int8                 </TD>    <TD>configNoOfMessages        </TD></TR>
 * <TR><TD>Int8                 </TD>    <TD>configMessageNo           </TD></TR>
 * <TR><TD>Boolean              </TD>    <TD>waitForActivationTime      </TD></TR>
 * <TR><TD>Int8                 </TD>    <TD>activationTimeIndex        </TD></TR>
 * </TABLE>
 *
 * These are used to group configuration primitives together so that the
 * MAC knows how many messages to expect for a configuration event.
 * The \I configIdentifier is allocated by RRC and has the same value for all
 * primitives that need to be processed by the physical layer at the same time.
 * The \I configNoOfMessages tells the physical layer how many primitives there
 * are in total in this group of primitives.
 * The \I configMessageNo identifies an individual primitive in this group and
 * is in the range 1 to \I configNoOfMessages.
 * The \I waitForActivationTime indicates whether MAC should activate a configuration
 * immediately (\I waitForActivationTime = \I FALSE) or wait for L1 to indicate
 * activation of a pending configuration (\I waitForActivationTime = \I TRUE).
 * The \I activationTimeIndex is ued to check whether the correct pending
 * configuration has been activated.
 *
 * @{
 */

/***************************************************************************
*   Manifest Constants
***************************************************************************/

/** defines maximum number of logical channels mapped to one transport format */
#define     CMAC_MAX_LOG_CH_PER_TF           15

/** Defines maximum number of gaps in on radio frame in case of compressed mode */
#define     CMAC_MAX_SLOT_GAPS_PER_FRAME     7

/** Bit mask used to get radio bearer identitity */
#define     CMAC_RB_ID_MASK                  0x1F

/**Bit mask used to get logical channel indicator */
#define     CMAC_LOG_CH_MASK                 0x80

/** Macro to get radio bearer identitity */
#define     CMAC_GET_RB_ID(a)          ((a) & CMAC_RB_ID_MASK)

/** Macro to set radio bearer identitity */
#define     CMAC_SET_RB_ID(a, b)             \
    ((a) = (Int8)(((a) & ~(CMAC_RB_ID_MASK)) \
                  | ((b) & (CMAC_RB_ID_MASK))))

/** Macro to get logical channel indicator */
#define     CMAC_GET_LOG_CH_IND(a)     ((a) & CMAC_LOG_CH_MASK)

/** Macro to set logical channel indicator */
#define     CMAC_SET_LOG_CH_IND(a)     ((a) |= CMAC_LOG_CH_MASK)

/** Defines minimum number of messages employs to configure RACH.
 * Messages used for RACH configuration are:
 *
 * 1x CmacTrChConfigReq,
 *
 * 1x CmacUlRbMappingConfigReq,
 *
 *1..n x CmacRachConfigReq(if TFCS is split, >1)
 */
#define     CMAC_MIN_RACH_CONFIG_MSGS               3

/** Defines maximum number of transport channel in one CTFC subset */
#define     CMAC_MAX_TR_CH_IN_TFC_SUBSET            8

/** Defines maximum number of CTFC in one TFC subset */
#define     CMAC_MAX_CTFC_ENTRIES_IN_TFC_SUBSET     128

/** This is required for cases when transport channels and associated TFC are
 * removed by UTRAN. The TFCS is not changed and TFCI remain the same.
 * TFCI_NOT_USED indicates the TFC can no longer be selected by MAC */
#define     TFCI_NOT_USED                           0xFFFFFFFF

/** Defines number of slots used to determine state of TFC */
#define     UMAC_NUMBER_SLOTS_FOR_CTFC_STATE        (2 * UPS_SLOTS_PER_FRAME)

#if defined(UPGRADE_3G_TDD128)

/** Defines number of timeslots used to eliminate a TFC */
# define TDD128_TFC_ELIMINATION_CRITERIA_X          1

/** Defines number of subframes used to recover a TFC */
# define TDD128_TFC_RECOVERY_CRITERIA_Y             3

/** Defines number of measurements for a single ressource unit in a subframe */
# define TDD128_MEAS_PERIOD_IN_SUBFRAMES            1

/** Defines number of measurements used to determine state of TFC */
# define TDD128_TFC_OBSERVATION_PERIOD              (TDD128_TFC_RECOVERY_CRITERIA_Y * TDD128_MEAS_PERIOD_IN_SUBFRAMES)
#endif /*UPGRADE_3G_TDD128*/

#if defined(UMAC_DEBUG) && defined(ON_PC)
# define  CMAC_MAX_RAND_VALUES                      16
#endif /*UMAC_DEBUG && ON_PC*/

/** \defgroup Common variants exchanged between URRC and MAC.
 *
 * @{
 *
 */

/** These are the common variants between RRC and MAC */
#define     UMAC_DXCH_NUM_MAPPINGS_NO_CT            1
#define     UMAC_CT_FIELD_LENGTH                    4

#define     UMAC_UE_ID_TYPE_LENGTH                  2
#define     UMAC_U_RNTI_LENGTH                      32
#define     UMAC_C_RNTI_LENGTH                      16

#if defined(UPGRADE_3G_TDD128)
#define     UMAC_TDD128_RACH_TCTF_CCCH_LENGTH       2
#define     UMAC_TDD128_RACH_TCTF_DXCH_LENGTH       4
#define     UMAC_TDD128_RACH_TCTF_CCCH              0x00
#define     UMAC_TDD128_RACH_TCTF_DXCH              0x04

/** 25.321 section 9.2.1.1 (c) => TCTF(of DCCH or DTCH) + C/T + UE-Id type + UE-Id
 * (==C-RNTI for RACH) UMAC_TDD128_RACH_TCTF_DXCH_LENGTH is 4 bits,
 * UMAC_CT_FIELD_LENGTH is 4 bits, UMAC_UE_ID_TYPE_LENGTH is 2 bits, C-RNTI is 16 bits
 * In UMAC_TDD128_RACH_DXCH_MAC_HDR_LENGTH, we do not count the optional
 * UMAC_CT_FIELD_LENGTH, which is added latter (only if C/T mux)
 */
#define     UMAC_TDD128_RACH_DXCH_MAC_HDR_LENGTH \
    (UMAC_TDD128_RACH_TCTF_DXCH_LENGTH +         \
     UMAC_UE_ID_TYPE_LENGTH +                    \
     UMAC_C_RNTI_LENGTH)

/** Definitions of FACH header sizes */

/** For TDD128: UMAC_TDD128_RACH_TCTF_DXCH_LENGTH is 4 bits,
 *              UMAC_TDD128_FACH_TCTF_DXCH_LENGTH is 5 bits. */
#define     UMAC_TDD128_FACH_TCTF_CXCH_LENGTH       3
#define     UMAC_TDD128_FACH_TCTF_DXCH_LENGTH       5
#define     UMAC_TDD128_FACH_DXCH_MAC_HDR_LENGTH \
    (UMAC_TDD128_FACH_TCTF_DXCH_LENGTH +         \
     UMAC_UE_ID_TYPE_LENGTH +                    \
     UMAC_C_RNTI_LENGTH)

#if defined (UPGRADE_3G_MBMS)
#define     UMAC_TDD128_FACH_TCTF_MXCH_LENGTH       5
#define     UMAC_TDD128_MBMS_ID_LENGTH              4
#endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G_TDD128 */

#if defined(UPGRADE_3G_FDD)
#define     UMAC_FDD_RACH_TCTF_CCCH_LENGTH          2
#define     UMAC_FDD_RACH_TCTF_DXCH_LENGTH          UMAC_FDD_RACH_TCTF_CCCH_LENGTH
#define     UMAC_FDD_RACH_TCTF_CCCH                 0x00
#define     UMAC_FDD_RACH_TCTF_DXCH                 0x01

/** 25.321 section 9.2.1.1 (c) => TCTF(of DCCH or DTCH) + C/T + UE-Id type + UE-Id
 * (==C-RNTI for RACH) UMAC_FDD_RACH_TCTF_DXCH_LENGTH is 2 bits,
 * UMAC_CT_FIELD_LENGTH is 4 bits, UMAC_UE_ID_TYPE_LENGTH is 2 bits, C-RNTI is 16 bits
 * In UMAC_TDD128_RACH_DXCH_MAC_HDR_LENGTH, we do not count the optional
 * UMAC_CT_FIELD_LENGTH, which is added latter (only if C/T mux)
 */
#define     UMAC_FDD_RACH_DXCH_MAC_HDR_LENGTH \
    (UMAC_FDD_RACH_TCTF_DXCH_LENGTH +         \
     UMAC_UE_ID_TYPE_LENGTH +                 \
     UMAC_C_RNTI_LENGTH)

/** Definitions of FACH header sizes */
#define     UMAC_FDD_FACH_TCTF_CXCH_LENGTH          8
#define     UMAC_FDD_FACH_DXCH_MAC_HDR_LENGTH       UMAC_FDD_RACH_DXCH_MAC_HDR_LENGTH
#endif /* UPGRADE_3G_FDD */
/** @} */ /* End of Common Variants group */

/***************************************************************************
*   Macro Functions
***************************************************************************/
typedef enum RevelWhichCompilationSwitchIsSetTag
{
#if defined(ON_PC)
    REVEAL_ON_PC_SWITCH,
#endif
#if defined(ON_TARGET_UNIT_TEST)
    REVEAL_ON_TARGET_UNIT_TEST_SWITCH,
#endif
#if defined(UMAC_DEBUG)
    REVEAL_UMAC_DEBUG_SWITCH,
#endif
#if defined(DEVELOPMENT_VERSION)
    REVEAL_DEVELOPMENT_VERSION_SWITCH,
#endif
#if defined(UPGRADE_3G_FDD)
    REVEAL_UPGRADE_3G_FDD_SWITCH,
#endif
#if defined(UPGRADE_3G_TDD128)
    REVEAL_UPGRADE_3G_TDD128_SWITCH,
#endif
#if defined(UPGRADE_3G_HSDPA)
    REVEAL_UPGRADE_3G_HSDPA_SWITCH,
#endif
#if defined(UPGRADE_3G_HSUPA)
    REVEAL_UPGRADE_3G_HSUPA_SWITCH,
#endif
    NUM_OF_REVELATIONS
}
RevelWhichCompilationSwitchIsSet;
/***************************************************************************
*   Types
***************************************************************************/

/** Defines possible states of CTFC based on which MAC decides whether possible
 * or not to use a CTFC for data transmission */
typedef enum UmacCTFCstateTag
{
    UMAC_CTFC_STATE_SUPPORTED,    /**< CTFC can be used for data transmission */
    UMAC_CTFC_STATE_EXCESS_POWER, /**< Tx power too high (Elimination, 25.133) */
    UMAC_CTFC_STATE_BLOCKED,      /**< Tx power too high too long (Blocking) */
    UMAC_CTFC_STATE_NUM_STATES    /**< Not used: only 2 bits reserved! */
}
UmacCTFCstate;

/** Defines whether a transmission was successful or not  */
typedef enum CmacRachTxStatusTag
{
    /** Indicates unsuccessful transmission of data over
        the RACH channel.
     */
    CMAC_TRANSMISSION_UNSUCCESSFUL = 1,

    /** Data transmitted by Layer 1 successfuly */
    CMAC_TRANSMISSION_SUCCESSFUL = 2
#if defined(UPGRADE_3G_HSUPA) && defined(UPGRADE_3G_TDD128)
                                   /** Indicates a failure in transmission of data over
                                       the E-RUCCH channel.
                                    */
    , CMAC_TRANSMISSION_ERUCCH_FAILURE = 3
#endif /*(UPGRADE_3G_HSUPA) && (UPGRADE_3G_TDD128)*/
}
CmacRachTxStatus;

/** Indicates whether a hard handover, or a handover from UTRAN has
 * been initiated at URRC
 */
typedef enum CmacHandoverTypeTag
{
    CMAC_HO_HARD         = 0,            /**< Hard handover */
    CMAC_HO_GSM_WITH_RAB = 1,            /**< Hard handover to GSM with RAB configured */
    CMAC_HO_GSM_NO_RAB   = 2,            /**< Hard handover to GSM and no RAB is configured */
    CMAC_OUT_OF_SYNC  = 3                /**< This indicates L1 has gone out-of-sync. */
}
CmacHandoverType;

/* DEVELOPMENT_VERSION */

/** Indicates mode test */
typedef enum CmacTestModeCommandTag
{
    CMAC_TEST_MODE_DEBUG = 0,               /**< For debugging purpose */
#if defined(DEVELOPMENT_VERSION)
    CMAC_TEST_MODE_INJECT_CRCS = 1,         /**< Inject CRCS on downlink data */
#else
    CMAC_TEST_MODE_INJECT_CRCS_CHOICE_UNAVAILABLE = 1,
#endif
# if defined(ON_PC) || defined(ON_TARGET_UNIT_TEST)
    CMAC_TEST_MODE_UNIT_TEST = 2,           /**< For unit test */
    CMAC_TEST_MODE_FORCE_OUT_OF_TMM = 3,    /**< Toggles whether MAC-hs thinks it is out of TMM memory */
#  if defined(UMAC_DEBUG) && defined(ON_PC)
    CMAC_TEST_MODE_SET_RAND = 4,            /**< Inject values for the rand function */
#  else
    CMAC_TEST_MODE_SET_RAND_CHOICE_UNAVAILABLE = 4,
#  endif /* UMAC_DEBUG && ON_PC */
# else  /*ON_PC*/
    CMAC_TEST_MODE_UNIT_TEST_CHOICE_UNAVAILABLE = 2,
    CMAC_TEST_MODE_FORCE_OUT_OF_TMM_CHOICE_UNAVAILABLE = 3,
    CMAC_TEST_MODE_SET_RAND_CHOICE_UNAVAILABLE = 4,
# endif /*ON_PC*/
    CMAC_NUM_TEST_MODES
}
CmacTestModeCommand;
/* DEVELOPMENT_VERSION */

#if defined(UPGRADE_3G_TDD128)

/** PhyChannelAllocation contains the timeslots and physical channels allocation for a TFCS */
typedef struct PhyChannelAllocationTag
{
    /** maxNumberOfUlSlots indicates the number of uplink timeslots allocated for the TFCS */
    Int8 maxNumberOfUlSlots;

    /** phyChannelListPerSlot indicates the number of physical channels allocated in each timeslot for the TFCS  */
    UphyChannelListPerSlotTdd128 phyChannelListPerSlot[MAX_UL_TIMESLOT];
}
PhyChannelAllocation;
#endif /*UPGRADE_3G_TDD128*/

/** This parameter describes the Gain Factors for transport format combinations
 */
typedef struct MacGainFactorTag
{
#if defined(UPGRADE_3G_FDD)
    /**  Defines gain factor BetaC, as used during normal (not compressed mode)
     * transmission for this TFC (FDD only). If this value is not signalled from
     * the network, URRC shall fill it in using equations in TS 25.214 section 5.1.2.5.3.
     * In compressed mode, UMAC considers additional power constraints according to TS 25.214
     * section 5.1.2.5.4 during TFC selection. This parameter used on all TrCHs.
     */
    UGainFactor gainFactorBetaC[CPHY_MAX_CTFC_ENTRIES_PER_PRIMITIVE];
#endif /*UPGRADE_3G_FDD*/

    /** gainFactorBetaD the gain factor BetaD, as used during normal (not compressed mode)
     * transmission for this TFC. If this value is not signalled from the network, URRC
     * shall fill it in using equations in TS 25.214 section 5.1.2.5.3. In compressed mode,
     * UMAC considers additional power constraints according to TS 25.214 section 5.1.2.5.4
     * during TFC selection. This parameter used on all TrCHs.
     */
    UGainFactor      gainFactorBetaD[CPHY_MAX_CTFC_ENTRIES_PER_PRIMITIVE];
#if defined(UPGRADE_3G_FDD)
    /** Power offset Ppm as used during transmission for this TFC (FDD only used on
     * RACH (CPCH?) only)*/
    UPowerOffsetPp_m powerOffsetPp_m[CPHY_MAX_CTFC_ENTRIES_PER_PRIMITIVE];

    /** Defines power relation 10log(BetaD2 /BetaC2 )) rounded to the nearest dB (FDD only)*/
    SignedInt8       logPowerRelation[CPHY_MAX_CTFC_ENTRIES_PER_PRIMITIVE];

    /** Defines spreading factor required to use this TFC (FDD only)*/
    USpreadingFactor spreadingFactor[CPHY_MAX_CTFC_ENTRIES_PER_PRIMITIVE];

    /** Defines number of compressed mode gaps (given compressed mode by higher layer scheduling)
     * which would cause a decrease in spreading factor (FDD only). If spreadingFactor is already
     * at the minimum configured spreading factor for this TFC, MAC should not select this TFC if
     * there are this many (or more) compressed mode gaps by HL scheduling. This parameter is only
     * set by RRC if the configuration is for CELL_DCH state.
     */
    Int8 numHlSchedGapsForSfReduction[CPHY_MAX_CTFC_ENTRIES_PER_PRIMITIVE];
#endif /*UPGRADE_3G_FDD*/
#if defined(UPGRADE_3G_TDD128)
    /** numberOfChannelsOverAllUlSlotsPerTFC is the number of physical channels used each in all
        uplink timeslots of the considered TFC. Those numbers refer to the channel and timeslots
        mapping of the PhyChannelAllocation.
    */
    Int8 numberOfChannelsOverAllUlSlotsPerTFC[CPHY_MAX_CTFC_ENTRIES_PER_PRIMITIVE];
#endif /*UPGRADE_3G_TDD128*/
}
MacGainFactor;

/** This parameter describes the transport format combination set (or part thereof).
 * It may also define that certain TFCs are to be deleted or reconfigured.
 */
typedef struct MacTFCSTag
{
    /** Whether TFCs are being added, reconfigured or removed
\assoc UNION \ref explicitTfcs */
    ExplicitTfcsMode  explicitTfcsMode;

    /** Defines TFCs which are being added or reconfigured */
    ExplicitTfcsUnion explicitTfcs;

    /** Defines Gain Factors for TFCs being added or reconfigured
     * (Note: this parameter is only read by MAC when explicitTfcsMode is UTFC_COMPLETE or UTFC_ADDITION) */
    MacGainFactor     gainFactors;
#if defined(UPGRADE_3G_FDD)
    /** Indicates the power difference for compressed mode using the equation defined
     * in TS 25.214: 10log(15. Npilot,C / Nslots,C.Npilot,N)). This also takes into account the
     * change of slot format, dependent on the number of slot gaps (see TS 25.211 section 5.2.1)
     * used for FDD only
     * (Note: this parameter is only read by MAC when explicitTfcsMode is UTFC_COMPLETE) */
    SignedInt8 logPowerForCM[CMAC_MAX_SLOT_GAPS_PER_FRAME];

    /** Informs MAC of the minimum spreading factor allowed by network and UE capability,
     * used for FDD
     * (Note: this parameter is only read by MAC when explicitTfcsMode is UTFC_COMPLETE) */
    USpreadingFactor minimumSF;
#endif /*UPGRADE_3G_FDD*/
#if defined(UPGRADE_3G_TDD128)
    /** phyChannelAllocation is the timeslots and physical channels allocation for this TFCS
     * used for TDD128
     * (Note: this parameter is only read by MAC when explicitTfcsMode is UTFC_COMPLETE) */
    PhyChannelAllocation phyChannelAllocation;
#endif /*UPGRADE_3G_TDD128*/
} MacTFCS;

/** Indicates a list of TFIs (rather than RLCsizeIndex) to accompany the UL channel
 * mappings, if an explicit list of RLC size indicies is present in the UL channel
 * mappings. \note MAC has no concept of RLC size index, and RRC always translates
 * this to TFI before forwarding to MAC.
 */
typedef struct MacTfiListTag
{
    /** Indicates the number of TFIs in the following array and it is in the range
     * 0 to UPS_MAX_UL_TF-1

\assoc ARRAY \ref tfi */
    Int8 sizeOfTfiList;

    /** Carries array of Transport Format Indicators (0 to (UPS_MAX_UL_TF-1) = TFI) */
    Int8 tfi[UPS_MAX_UL_TF];
} MacTfiList;

/** Restricts the TFCS by eliminating certain transport formats from a given
 * transport channel
 */
typedef struct MacRestrictedTrChInfoTag
{
    /** Identifies the transport channel to be restricted and it is assumed transport
     * channel to be DCH
     */
    UTransportChannelIdentity restrictedTrChIdentity;

    /** Defines list of TFIs which are valid for TFC selection on this transport channel */
    MacTfiList                allowedTFIs;
} MacRestrictedTrChInfo;

/**Defines number and identities of transport channels whose transport format is
 * eliminated
 */
typedef struct MacRestrictedTrChInfoListTag
{
    /** Indicates number of transport channels in the list
\assoc ARRAY \ref data */
    Int8                  sizeOfRestrictedTrChInfo;

    /** Defines list of transport channels which may contain restricted transport formats */
    MacRestrictedTrChInfo data[CMAC_MAX_TR_CH_IN_TFC_SUBSET];
} MacRestrictedTrChInfoList;

/** Defines a list of TFCS which are allowed */
typedef struct MacTFC_ListTag
{
    /** Defines number of TFCs in the list
\assoc ARRAY \ref utfcValueArray */
    Int8       sizeOfTfcList;

    /** Defines list of TFCs, defined by TFC index into the TFCS */
    UTFC_Value utfcValueArray[CMAC_MAX_CTFC_ENTRIES_IN_TFC_SUBSET];
} MacTFC_List;

/** Indicates if the subset is aloowing or restricting some of TFCS */
typedef union MacTFC_SubsetUnionTag
{
    /** Minimum allowed TFC is a single value in the message */
    Int16       minimumAllowedTFC_Number;

    /** Restricts the TFCS to this list of TFCs */
    MacTFC_List allowedTFC_List;

    /** Restricts the TFCS by defining this list of TFCs to be barred */
    MacTFC_List non_allowedTFC_List;

    /** Restricts the TFCS by eliminating certain transport formats from
     * a given transport channel
     */
    MacRestrictedTrChInfoList restrictedTrChInfoList;
}
MacTFCsubsetUnion;

/** Contains the list of allowed or not allowed TFCs. The internal coding method
 * is indicated within the data structure
 */
typedef struct MacTFC_SubsetTag
{
    /** Indicates which union option is used
\assoc UNION \ref u_UTFC_Subset */
    T_UTFC_Subset     tag;

    /** Indicates TFC subset */
    MacTFCsubsetUnion u_UTFC_Subset;
} MacTFC_Subset;

/** Indicates both the radio bearer identity and the logical channel
 * indicator. Radio bearer identity (in the bottom 5 bits) and
 * the logical channel indicator for that radio bearer (0 or 1) in the
 * most significant bit. The next-most significant bit is unused.
 * This is formatted as follows:
 * \image html logChByRB.PNG
 */

/*
                                            0  0  0  0  0  0  0  0
                                            ULlogChInd
                                                     RadioBearerIdentity
*/

typedef struct MacLogicalChannelByRBTag
{
    /** Defines radio bearer identity and logical channel indicator.
     * Use macros CMAC_SET_RB_ID/CMAC_GET_RB_ID and CMAC_SET_LOG_CH_IND/CMAC_GET_LOG_CH_IND
     * set or extract the Radio Bearer ID and Logical Channel Indicator respectively.
     */
    Int8 rb_IdAndLogChInd;
} MacLogicalChannelByRB;

/** Lists every logical channel which is valid for the transport channel
 * indexed in the logical channel list
 */
typedef struct MacLogicalChannelList_explicitListTag
{
    /** Indicates number of logical channels present in the explicit list;
     * The default value for CMAC_MAX_LOG_CH_PER_TF is 15

\assoc ARRAY \ref data */
    Int8                  numberOfLogicalChannels;

    /** Defines an array of logical channels */
    MacLogicalChannelByRB data[CMAC_MAX_LOG_CH_PER_TF];
} MacLogicalChannelList_explicitList;

/** Contains additional dynamic information for configuration of explicit list of
 * logical channels. Carries information particular to MAC for configuration of an
 * explicit list of logical channels which are valid for a given transport format
 * (the index of the logical channel list defines the index to the transport format
 * in the dynamicTfData).
 */
typedef struct MacLogicalChannelListTag
{
    /** Indicates whether the indexed transport format is valid for the following
     * explicit list of logical channels, for all logical channels, or for logical
     * channels configured using the CmacUlRbMappingConfigReq message. The values of
     * this parameter take their values from the ASN.1 enumeration T_ULogicalChannelList.
     */
    Int8 logicalChannelListType;

    /** This parameter is valid where logicalChannellListType = T_ULogicalChannelList_explicitList.
    * It lists every logical channel which is valid for the transport channel indexed in
    * the logical channel list.
    */
    MacLogicalChannelList_explicitList logicalChannelList;
} MacLogicalChannelList;

/** Identifies the TTI andd the dynamic parts of the transport format */
typedef struct MacTransChTFSTag
{
    /** Identifies the semi-static TTI as selected by RRC (for RACH) or by the network (for DCH).
     * Dynamic TTI is not supported.
     */
    TransmissionTimeInterval tti_type;

    /** Identifies the dynamic parts of the transport format. this parameter defines
    * number of transport formats and their transport block sizes and number of transport
    * blocks.
    *\note MAC has no knowledge of RLC size, only transport block size.
    */
    DynamicTfData dynamicTfData;

    /** This parameter defines index of the logical channel list defines index to the transport
     * format in the dynamicTfData.
     * \note Index to TF in TFS within DynamicTfData is also the index of TF in logicalChannelList
     */
    MacLogicalChannelList logicalChannelList[UPS_MAX_UL_TF];
} MacTransChTFS;

/** Contains transport channel configuration information. Note that this information
 * is only given for uplink transport channels. UMAC does not need to know the downlink
 * configuration. The transport channels are assigned transport formats based on
 * transport block size information. MAC has no knowledge of RLC size
 */
typedef struct UTransportChannelConfigInfoTag
{
    /** Identifies which transport channel the following information applies to */
    UTransportChannelIdentity transportChannelIdentity;

    /** Identifies whether the UL transport channel to be configured is RACH or DCH */
    UTransportChannelType     transportChannelType;

    /** Contains set of transport formats for this transport channel */
    MacTransChTFS             transportFormatSet;
}
UTransportChannelConfigInfo;

/** This signal is sent to UMAC by URRC to inform UMAC about the current Radio
 * Network Temporary Identities. This primitive indicates  current Radio
 * Network Temporary Identities. \note Deletion of the U-RNTI is the indication
 * to MAC that the UE is entering idle mode. Similarly, indication of a valid U-RNTI
 * indicates to MAC that the UE is entering connected mode. Note that for valid transmissions
 * on RACH on logical channels other than CCCH, MAC must also have a C-RNTI.
 * Reception on all radio bearers is possible if the network uses the U-RNTI in the downlink.
 */
typedef struct CmacRNTIConfigReqTag
{
    /** Indicates whether the U-RNTI is valid, or to be deleted */
    Boolean validU_RNTI;

    /** Represents Radio Network Temporary Identity (RNTI) */
    UU_RNTI u_RNTI;

    /** Indicates whether the C-RNTI is valid, or to be deleted */
    Boolean validC_RNTI;

    /** Represents Cell Radio Network Temporary Identity */
    UC_RNTI c_RNTI;
}
CmacRNTIConfigReq;

/** Defines HFN */
typedef Int32 UMacHyperFrameNumber;

/** Contains current HFN data with the ksi that is currently used for CS
 * and PS domains. It only provides valid information if TM radio barer is configured.
 */
typedef struct CmacHfnInfoTag
{
    /** Set to TRUE if the contents of csMacdHfn and csKsi are valid
\assoc PRESENT \ref csMacdHfn
\assoc PRESENT \ref csKsi */
    Boolean csIsValid;

    /** Contains the full 24 bit HFN component of the most recent COUNT-C value.
     * If there are no TM radio bearers in the CS domain this field is not valid.
     */
    UMacHyperFrameNumber csMacdHfn;

    /** Defines ksi used in the CS domain when the HFN component was measured.
    If there are no TM radio bearers in the CS domain this field is not valid.
    */
    KeySeqId csKsi;

    /** Set to TRUE if the contents of psMacdHfn and psKsi are valid
\assoc PRESENT \ref psMacdHfn
\assoc PRESENT \ref psKsi */
    Boolean  psIsValid;

    /** Contains the full 24 bit HFN component of the most recent COUNT-C value
     * in the PS domain. If there are no TM radio bearers in the PS domain this
     * field is not valid.
     */
    UMacHyperFrameNumber psMacdHfn;

    /** Defines ksi used in the PS domain when the HFN component was measured.
    * If there are no TM radio bearers in the PS domain this field is not valid.
    */
    KeySeqId psKsi;
}
CmacHfnInfo;

/** If radio bearers for the CN domain are ciphered, the initialStartValue contains
 * the START value to initialise the HFN when the bearer is activated and this is not
 * required if ciphering is not enabled.
 * The tmCountcStartValue is set at a CFN that is exchanged between UTRAN and UE.
 * In some cases the UTRAN provides the CFN, which is then included in tmCountcStartActTime,
 * which UMAC must use and in this case no CmacHfnConfigCnf is required. In other cases,
 * when tmCountcStartActTime is not present in the message, the UMAC determines the CFN
 * (according to 25.331 section 8.6.4.3. and 8.6.6.28), which is then returned to URRC
 * in a CmacHfnConfigCnf. In either case, at the CFN agreed in this procedure the MAC-d
 * COUNT-C HFN component is set to the full 24 bit tmCountcHfn.
 */
typedef struct CmacStartValuesTag
{
    /** Indicates if the field initialHfn contains valid information
\assoc PRESENT \ref initialHfn */
    Boolean initialHfnPresent;

    /** Contains the START value that is used to initialise the HFN component of the MAC-d COUNT-C,
     * when the first transparent mode radio bearer in a CN domain is created. This initialisation is
     * only required if this is a transparent mode radio bearer and ciphering is enabled for CN domain
     * associated with the bearer. Note the HFN should not be incremented on CFN wrap.
     */
    Int32 initialHfn;

    /** Indicates the 24 bit HFN value that is used to initialise the HFN component of the MAC-d COUNT-C,
     * after a delay following the initialisation of the radio bearer. The delay is specified by a CFN
     * tmCountcStartActTime. If this is not present then the UMAC determines a CFN according to 25.331
     * section 8.6.6.28 and 8.6.4.3, which is then returned in a CmacHfnConfigCnf.
     */
    UMacHyperFrameNumber tmCountcHfn;
}
CmacStartValues;

/** These values depend on whether CountC activation time is present. \a CMAC_TM_COUNTC_ACT_TIME_APPLY_NOW is
 * used during the inter-RAT handover procedure.
 */
typedef enum CmacTmCountcStartActTimeUsageTag
{
    CMAC_TM_COUNTC_ACT_TIME_FROM_UTRAN, /**< Use the tmCountcStartActTime */
    CMAC_TM_COUNTC_ACT_TIME_FROM_UE,    /**< MAC determines tmCountcStartActTime */
    CMAC_TM_COUNTC_ACT_TIME_APPLY_NOW,  /**< Apply the HFN as soon as possible */
    CMAC_TM_COUNTC_ACT_TIME_INVALID   /**< Not valid */
}
CmacTmCountcStartActTimeUsage;

/** \defgroup SigCmacCiphering Ciphering Configuration Primitives
 * This section defines the primitives exchanged between MAC and RRC to
 * configure ciphering parameters. These primitives set ciphering configuration for
 * TM radio bearer.
 * @{
 */

/** This signal is sent to UMAC by URRC to inform UMAC about the current ciphering
 * configuration for all radio bearers in a CN domain. The ciphering configuration only applies
 * to transparent mode radio bearers. The message may be sent when the CN domain has no radio bearers.
 */
typedef struct CmacCipheringConfigReqTag
{
    /** Indicates the domain that this ciphering configuration applies to */
    UCN_DomainIdentity cnDomain;

    /** Indicates whether the ciphering should be started/restarted or stopped,
     * and the ciphering algorithm to use. Note that the HFN component of MAC-d COUNT-C
     * starts/continues incrementing at the cipheringActivationTime when ciphering is
     * started/restarted, and similarly stops incrementing when ciphering is stopped.
     */
    UCipheringModeCommand_r7 cipheringModeCommand;

    /** Indicates that the field cipheringActivationTime contains a valid value
\assoc PRESENT \ref cipheringActivationTime */
    Boolean                  cipheringActivationTimePresent;

    /** Contains the ciphering activation time for the DPCH. This is the CFN at
     * which the ciphering configuration in this message is activated in uplink
     * and downlink. If no activation time is present the ciphering configuration
     * should not be applied until an activation time is agreed with RRC using CmacHfnConfigReq.
     * <B> \note Further ciphering configurations may be sent, before activation.</B>
     */
    UActivationTime cipheringActivationTime;                     /* ASN.1 activation time type */
    /** Contains ciphering key (CK) to be used by the MAC */
    USecurityKey    ck;

    /** Contains key set identifier corresponding to the ciphering key, and extra information
     * to ensure it is unique in the UE
     */
    KeySeqId ksi;

    /** Indicates if startValue field is valid
\assoc PRESENT \ref startValue */
    Boolean  startValuePresent;

    /** Defines the top 20 bits of MAC-d HFN (24 bits), according to TS 33.102 section 6.6.4.1.
     * This should be set at cipheringActivationTime. If no startValue is given the MAC-d HFN
     * is not reset.
     */
    USTART_Value startValue;
}
CmacCipheringConfigReq;

/** @} */ /* End of SigCmacCiphering group */

/** Defines which RACH transmission control elements to use for RACHing */
typedef union RACH_TransmissionParametersUnionTag
{
#if defined(UPGRADE_3G_FDD)
    /** For FDD */
    URACH_TransmissionParameters rach_TransmissionParameters;
#endif /*UPGRADE_3G_FDD*/
#if defined(UPGRADE_3G_TDD128)
    /** Defines maximum number of synchronisation (TDD128) */
    Int8 mmax;
#endif /*UPGRADE_3G_TDD128*/
}
RACH_TransmissionParametersUnion;

/** \defgroup SigCmacTrch Transport Channel Primitives
 * This section defines the primitives exchanged between MAC and RRC for
 * establishing, re-configuring and releasing transport channels and radio
 * bearers.
  * <B> MSC below shows transactions between MAC and RRC during uplink configuration </B>
 *
 *\msc
 * msc {
 *  "URRC","UMAC";
 *  --- [label="RACH Configuration (FDD or TDD128)"];
 *  "URRC" -> "UMAC" [label="CmacRachConfigReq (id=1,noMsg=4,msgNo=1, )"];
 *  "URRC" -> "UMAC" [label="CmacUlTrChConfigReq (id=1,noMsg=4,msgNo=2, )"];
 *  "URRC" -> "UMAC" [label="CmacUlRbMappingConfigReq (id=1,noMsg=4,msgNo=3, )"];
 *  "URRC" -> "UMAC" [label="CmacUlMinTfcSetConfigReq (id=1,noMsg=4,msgNo=4, )"];
 *
 *
 *  --- [label="DCH Configuration (FDD)"];
 *  "URRC" -> "UMAC" [label="CmacUlDedicatedTfcsConfigReq (id=2,noMsg=7,msgNo=1, ) "];
 *  "URRC" -> "UMAC" [label="CmacUlTrChConfigReq (id=2,noMsg=7,msgNo=2, ) "];
 *  "URRC" -> "UMAC" [label="CmacUlRbMappingConfigReq (id=2,noMsg=7,msgNo=3, ) "];
 *  "URRC" -> "UMAC" [label="CmacUlRbMappingConfigReq (id=2,noMsg=7,msgNo=4, ) "];
 *  "URRC" -> "UMAC" [label="CmacUlTfcSubsetConfigReq (id=2,noMsg=7,msgNo=5, ) "];
 *  "URRC" -> "UMAC" [label="CmacCompressedModeConfigReq (id=2,noMsg=7,msgNo=6, ) "];
 *  "URRC" -> "UMAC" [label="CmacUlMinTfcSetConfigReq (id=2,noMsg=7,msgNo=7, ) "];
 *
 *
 *  --- [label="DCH Configuration (TDD128, first CCTrCH)"];
 *  "URRC" -> "UMAC" [label="CmacUlDedicatedTfcsConfigReq (id=2,noMsg=6,msgNo=1, ) "];
 *  "URRC" -> "UMAC" [label="CmacUlTrChConfigReq (id=2,noMsg=6,msgNo=2, ) "];
 *  "URRC" -> "UMAC" [label="CmacUlRbMappingConfigReq (id=2,noMsg=6,msgNo=3, ) "];
 *  "URRC" -> "UMAC" [label="CmacUlRbMappingConfigReq (id=2,noMsg=6,msgNo=4, ) "];
 *  "URRC" -> "UMAC" [label="CmacUlTfcSubsetConfigReq (id=2,noMsg=6,msgNo=5, ) "];
 *  "URRC" -> "UMAC" [label="CmacUlMinTfcSetConfigReq (id=2,noMsg=6,msgNo=6, ) "];
 * }
 *\endmsc
 * @{
 */

/** This signal includes various configuration information needed to inform UMAC of
 * RACH Configuration.  This message defines transport format combination sets and setting
 * RACH transmission parameters. This message <B>MUST</B> be sent before any RACH transport
 * channel configuration is sent (using CmacUlTrChConfigReq).
 */
typedef struct CmacRachConfigReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8 configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8 configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8 configMessageNo;

    /** Indicates whether MAC should activate a configuration immediately
     * (waitForActivationTIme = FALSE) or wait for L1 to indicate activation
     * of a pending configuration (waitForActivaiton = TRUE)
     */
    Boolean waitForActivationTime;

    /** Used to check whether the correct pending configuration has been activated */
    Int8    activationTimeIndex;

    /** Identifies Transport Format Combinations which are allowed in the RACH channel */
    MacTFCS prach_TFCS;

    /** Indicates wether setupASC parameter containes valid information */
    Boolean useSetupASC;

    /** Defines Access Service Class (ASC) to be used with the initial access */
    Int8    setupASC;

    /** Indicates number of Access Service Classes which has been defined */
    Int8    numberOfAccessServiceClasses;

    /** Contains dynamic persistence level value from the UTRAN. MaxPRACH is defined
    * in [3G 25.331 / 10.3.10 as having value 16]; that is, one cell can have max 16 PRACHs.
    */
    UDynamicPersistenceLevel dynamicPersistenceLevel;

    /** Indicates number of scaling factors contained in persistenceScalingFactor
\assoc ARRAY \ref persistenceScalingFactor */
    Int8                     numberOfPersistenceScalingFactors;

    /** Defines scaling factors associated with ASC 2 to ASC 7 to be applied to the
     * dynamic persistence value
     */
    UPersistenceScalingFactor persistenceScalingFactor [maxASCpersist];

    /**  Indicates for which mode (FDD or TDD128) this primitive is sent
\assoc UNION \ref mode
\assoc UNION \ref mode */
    UmtsMode                  umtsMode;

    /** Identifies Mmax(FDD or TDD128),NB01min(FDD only) and NB01max(FDD only)
     * parameters
     */
    RACH_TransmissionParametersUnion mode;
}
CmacRachConfigReq;

/** This signal is used by UMAC to inform URRC of the success status of the initial RACH
 * transmission (containing RRC Connection Request). This is implemented such that status
 * of every transmission on CCCH where there is no valid C-RNTI or U-RNTI is indicated to RRC.
 */
typedef struct CmacRachTxStatusIndTag
{
    /** Indicates whether CCCH transmission was successful or not */
    CmacRachTxStatus rachTxStatus;
}
CmacRachTxStatusInd;

/** This signal informs UMAC of uplink transport format combination sets. This message is sent
 * by RRC to MAC to configure dedicated channels. It must be the first message of a message set
 * when configuring dedicated channels for the first time. <B>In TDD, this message is sent for each
 * uplink TFCS </B>.
 */
typedef struct CmacUlDedicatedTfcsConfigReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8 configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8 configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8 configMessageNo;

    /** Indicates whether MAC should activate a configuration immediately
     * (waitForActivationTIme = FALSE) or wait for L1 to indicate activation
     * of a pending configuration (waitForActivaiton = TRUE)
     */
    Boolean waitForActivationTime;

    /** Used to check whether the correct pending configuration has been activated */
    Int8    activationTimeIndex;

    /** Used to flush the pending ciphering configuration, and to apply it now */
    Boolean applyPendingCipherConfig;

    /** Identifies Transport Format Combinations which are allowed in the uplink direction */
    MacTFCS uplinkTFCs;
#if defined(UPGRADE_3G_TDD128)
    /** Indicates whether this signal is the first signal of a set of CmacUlDedicatedTfcsConfigReq
     * signals. If the value is TRUE, then all previous transport format combination set and their
     * configurations will be removed first.
     */
    Boolean                firstSignal;

    /** Identifies Transport Format Combination Set which is to be configured (TDD only) */
    Int8                   tfcsId;

    /** Maximum allowed Tx Power of the TDD128 */
    UMaxAllowedUL_TX_Power maxAllowedUL_TX_Power;
#endif /*UPGRADE_3G_TDD128*/
}
CmacUlDedicatedTfcsConfigReq;

/** The Transport Format Combination Control procedure is used to temporarily limit the
 * set of allowed Transport Format Combinations. The UTRAN sends a set of allowed TFCs
 * in the Transport Format Combination Control message, and once the overload situation
 * has passed, it sends another message, which contains the original TFCs (or if a duration
 * time has been defined, the old set is taken into use once the duration time has passed).
 * Note that this signal only contains a set of Transport Format Combination identities. It does
 * not contain the actual Transport Format descriptions, as they are already known by the UMAC task.
 *
 * Note that this message only contains the limited set for the uplink direction. There is no need
 * to limit the allowed downlink set in the UE. In TDD, this message is sent for each uplink TFCS.
 */
typedef struct CmacUlTfcSubsetConfigReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8 configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8 configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8 configMessageNo;

    /** Indicates whether MAC should activate a configuration immediately
     * (waitForActivationTIme = FALSE) or wait for L1 to indicate activation
     * of a pending configuration (waitForActivaiton = TRUE)
     */
    Boolean waitForActivationTime;

    /** Used to check whether the correct pending configuration has been activated */
    Int8    activationTimeIndex;
#if defined(UPGRADE_3G_TDD128)
    /** Identifies the Transport Format Combination Set which is to be configured and
     * is applied for TDD only
     */
    Int8 tfcsId;
#endif /*UPGRADE_3G_TDD128*/

    /** Contains the list of allowed (or not allowed) TFCs; The internal coding method
     * is indicated within the data structure
     */
    MacTFC_Subset        tfcSubset;

    /** Indicates whether the next element has any meaningful value */
    Boolean              tfc_ControlDurationPresent;

    /** Indicates the length of the TFC limitation imposed by the tfcSubset parameter */
    UTFC_ControlDuration tfc_ControlDuration;
}
CmacUlTfcSubsetConfigReq;

/** This signal communicates the minimum TFC subset to MAC as defined in TS 25.331 section 8.6.5.2.
 * This primitive is only used for dedicated channel configurations (i.e. it is always sent in a
 * message set which includes CmacDedicatedTfcsConfigReq). In TDD, this message is sent for each
 * uplink TFCS.
 */
typedef struct CmacUlMinTfcSetConfigReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8 configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8 configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8 configMessageNo;

    /** Indicates whether MAC should activate a configuration immediately
     * (waitForActivationTIme = FALSE) or wait for L1 to indicate activation
     * of a pending configuration (waitForActivaiton = TRUE)
     */
    Boolean waitForActivationTime;

    /** Used to check whether the correct pending configuration has been activated */
    Int8    activationTimeIndex;
#if defined(UPGRADE_3G_TDD128)
    /**  Identifies the Transport Format Combination Set which is to be configured
     * and this is applied only for TDD
     */
    Int8        tfcsId;
#endif /*UPGRADE_3G_TDD128*/
       /** Indicates the minimum set of TFCs as defined in 25.331 section 8.6.5.2 */
    MacTFC_List minimumSet;
}
CmacUlMinTfcSetConfigReq;

/** This signal informs UMAC of the release of the uplink dedicated channels. This message is sent
 * by RRC to MAC to release all dedicated channels and related configurations.
 * This signal has no impact on the E-DCH channels.
 */
typedef struct CmacUlDedicatedReleaseReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8 configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8 configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8 configMessageNo;

    /** Indicates whether MAC should activate a configuration immediately
     * (waitForActivationTIme = FALSE) or wait for L1 to indicate activation
     * of a pending configuration (waitForActivaiton = TRUE)
     */
    Boolean waitForActivationTime;

    /** Used to check whether the correct pending configuration has been activated */
    Int8    activationTimeIndex;
}
CmacUlDedicatedReleaseReq;

/** @} */ /* End of SigCmacTrch group */

/** \defgroup SigCmacCiphering Ciphering Configuration Primitives
 * This section defines the primitives exchanged between MAC and RRC to
 * configure ciphering parameters. These primitives set ciphering configuration for
 * TM radio bearer.
 * @{
 */

/** This signal sent to MAC, by RRC, to setup the hyper frame number(HFN) for a CN domain.
 * There are three scenarios when this message is sent
 *
 * the first TM radio bearer for the CN domain is activated
 *
 * timing is reinitialised following a Layer 1 resynchronisation, only if ciphering is active
 *
 * a handover to UTRAN is in progress
 *
 * In the first two cases TM radio bearers have a two stage initialisation: the initial activation;
 * and then at a later CFN an HFN (or COUNT-C) activation time. In the last case, there is no HFN
 * activation time. If the radio bearer is ciphered, it is activated with ciphering algorithm running
 * and a fixed HFN.  When the HFN activation time is reached (for the first two cases) the HFN is
 * initialised with a new START value, and the HFN is incremented as normal. If the radio bearer
 * is not ciphered the HFN is only set at the HFN activation time, and it is not incremented.
 *
 * Either one or both CN domains may be configured using this message, indicated by the presence of
 * the xxStartValues field. In the case when both domains are configured together, the activation times
 * for each domain must be the same, for both initial and HFN activation.
 *
 * A <b> CmacHfnConfigCnf </b> is only required when the HFN activation time is not provided in the
 * CmacHfnConfigReq, and a HFN activation time is required (see 4.1.31).
 *
 *\note If a cipher configuration is pending, due to an activation time that is still in the future
 * when this message is received, this pending cipher configuration shall be applied by this procedure.
 */
typedef struct CmacHfnConfigReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8           configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8           configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8           configMessageNo;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Boolean        waitForActivationTime;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8           activationTimeIndex;

    /** Indicates when the new configuration should take effect */
    ActivationTime activationTime;

    /** Indicates if a valid tmCountcStartActTime is present, or whether the HFN
    * should be used immediately (in case of inter-RAT handover to UTRAN)
    */
    CmacTmCountcStartActTimeUsage tmCountcStartActTimeUsage;

    /** Indicates the CFN to reset the MAC-d HFN for the CN domain, which is set
     * to the START value tmCountcStartValue. If ciphering is active for the CN
     * domain the HFN is incremented when the CFN wraps.
     */
    UActivationTime tmCountcStartActTime;

    /** Indicates psStartValues contains valid data. This also indicates that
     * PS domain is configured by this message.

\assoc PRESENT \ref psStartValues */
    Boolean psStartValuesPresent;

    /** Contains the START information for PS domain. If this field is
     * not present the PS domain is not configured by this message.
     */
    CmacStartValues psStartValues;

    /** Indicates csStartValues contains valid data. This also indicates
     * that the CS domain is configured by this message.

\assoc PRESENT \ref csStartValues */
    Boolean csStartValuesPresent;

    /** contains the START information for the CS domain. If this field is
     * not present the CS domain is not configured by this message.
     */
    CmacStartValues csStartValues;
}
CmacHfnConfigReq;

/** This signal is sent by UMAC to URRC to indicate the activation time when the
* MAC-d HFN will be updated. It is only sent if CmacHfnConfigReq does not contain
* tmCountcStartActTime.  It is returned after the implied Layer 1 resynchronisation
* has been completed.
*/
typedef struct CmacHfnConfigCnfTag
{
    /** Contains the CFN when the MAC-d HFN is updated according to parameters in
     * CmacHfnConfigReq. If ciphering is active normal incrementing of the HFN is
     * started at this activation time. Note, if both domains are configured simultaneously
     * in the CmacHfnConfigReq the same activation time must be used.
     */
    UActivationTime tmCountcStartActTime;
}
CmacHfnConfigCnf;

/** This signal is used to abort an outstanding CmacHfnConfigReq, and return to
 * the HFN used for ciphering before the CmacHfnConfigReq was received.
 * This primitive is required when a timing reinitialised handover fails, and the
 * original configuration must be restored.
 *
 * UMAC applies the COUNT-C contained in ulHfnRevert and dlHfnRevert, when
 * synchronisation to dedicated channels is completed, instead of the values in the
 * CmacHfnConfigReq. MAC accounts for CFN wrapping during the procedure. The COUNT-C
 * for both core network domains are configured in the ulHfnRevert / dlHfnRevert,
 * but only the domains that have TM bearers present in the configuration will be used.
 *
 * This signal is not acknowledged explicitly, and any outstanding CmacHfnConfigCnf will
 * not be sent.
 *
 *\note MAC will indicate the last used COUNT-C at the activation time of the new physical
 * channel, in the UmacUlConfigChangeInd and UmacDlConfigChangeInd signals.
 *
 *\note If a cipher configuration is pending, due to an activation time that is still
 * in the future when this message is received, this pending cipher configuration shall be
 * applied by this procedure.
 */
typedef struct CmacHfnAbortReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8             configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8             configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8             configMessageNo;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Boolean          waitForActivationTime;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8             activationTimeIndex;

    /** Indicates information for the uplink on how to revert the HFN */
    UmacTmCountcInfo ulHfnRevert;

    /** Indicates information for the downlink on how to revert the HFN */
    UmacTmCountcInfo dlHfnRevert;
}
CmacHfnAbortReq;

/** @} */ /* End of SigCmacciphering group */

/** \defgroup SigCmacTrch Transport Channel Primitives
 * This section defines the primitives exchanged between MAC and RRC for
 * establishing, re-configuring and releasing transport channels and radio
 * bearers.
 *
 * @{
 */

/** This message enables UMAC to associate a number of bearers with a CS RAB.
 * This message sent to MAC, by RRC to allow MAC to map a number of bearers with a CS RAB.
 * However, this message shall not be sent by RRC to MAC if RRC has received the message
 * TiRrcActivateRbTestLoopModeReq and has not yet received the message
 * TiRrcDeactivateRbTestLoopModeReq.
 */
typedef struct CmacCsRabConfigReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8          configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8          configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8          configMessageNo;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Boolean       waitForActivationTime;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8          activationTimeIndex;

    /** Identifies radio access bearer*/
    URAB_Identity rabIdentity;

    /** Indicates whether the RAB has been set up due to a switch to UTRAN from GSM */
    Boolean       handoverActive;

    /** Defines codec type (length 4 bits) as specified in TS 26.103 section 5.4
     * (FR_AMR_CoID, HR_AMR_CoID, UMTS_AMR_CoID, UMTS_AMR2_CoID)
     */
    CsCodecType codecType;

    /** Indicates number of radio bearers in this RAB
\assoc ARRAY \ref bearerIdentity */
    Int8        numberOfRBs;

    /** Identifies radio bearer in this RAB.
    * \note If RAB is <I> downgraded </I> to a TFC which with a maximum ARR of 7.95 kbps,
    * UTRAN may remove the C subflow completely, and its associated transport channel.
    * In this case, URRC sends a second CmacRabConfigReq with new CmacUlRbMappingConfigReq
    * and CmacUlTrChConfigReq messages, which associates the RAB with only two RBs.
    */
    BearerIdentity bearerIdentity[maxRBperRAB];
}
CmacCsRabConfigReq;

/** This signal indicates to UMAC when a handover is initiated.
 * This primitive allows the DL circuit of all CS RABs to be broken before
 * synchronisation is achieved on the new frequency or RAT.
 */
typedef struct CmacHandoverReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    configMessageNo;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Boolean waitForActivationTime;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    activationTimeIndex;

    /** Indicates whether a hard handover, or a handover from UTRAN has been
     * initiated at URRC
     */
    CmacHandoverType handoverType;

    /** Valid only if handoverType is CMAC_HO_GSM_WITH_RAB; It indicates
     * RAB which persists when handing over to GSM.
     */
    URAB_Identity rabIdentity;
}
CmacHandoverReq;

/** This signal indicates to UMAC to ensure any pending
 * activations are flushed.
 */
typedef struct CmacActivationTimeResetReqTag
{
    /** Indicates whether a out of sync occured. */
    Boolean     outOfSyncOccured;
}
CmacActivationTimeResetReq;

/** This signal is sent to UMAC by URRC to setup radio bearer multiplexing information
 * for downlink bearers. This signal contains multiplexing information for one radio bearer
 * mapping option, thus URRC may have to send several signals for each active radio bearer.
 */
typedef struct CmacDlRbMappingConfigReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8           configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8           configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8           configMessageNo;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Boolean        waitForActivationTime;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8           activationTimeIndex;

    /** Identifies bearer this signal applies to */
    BearerIdentity bearerIdentity;

    /** Indicates whether this signal is the first signal of a set of CmacDlRbMappingConfigReq
     * signals. If the value is TRUE, then all previous downlink mappings for all radio bearers
     * will be removed first.
     */
    Boolean firstSignal;

    /** Indicates whether the associated bearer is in transparent mode (and therefore may
     * require ciphering) and which other possible mode (UM or AM) is configured (to allow UMAC
     * to determine URLC header size)
     */
    T_UDL_RLC_Mode_r7 bearerMode;

    /** Valid only if isTm is TRUE (TM mode). It indicates whether the bearer is in CS or PS domain.
     * This is required in order to determine whether the bearer should be ciphered using
     * CS or PS parameters.
     *\note  RB_0 never has a valid cnDomain.
     */
    UCN_DomainIdentity cnDomain;

    /** Has been removed from the standard */
    Boolean            epcEnabled;

    /** Maps downlink logical channels to transport channels (R99) or
     * MAC-d flows (HS-DSCH)
     */
    UDL_LogicalChannelMappingList_r7 dlMappingList;

    /** Indicates TTI for transport channels given in dlMappingList */
    UTransmissionTimeInterval        ttiType[maxLoCHperRLC];
}
CmacDlRbMappingConfigReq;

/** This signal is sent to the UMAC by the URRC to setup an uplink transport channel.
 * This signal contains transport channel configuration for one channel only, thus URRC
 * has to send one signal for each transport channel. In the case where RACH is used,
 * only one transport channel (and hence one message) is used.
 *
 * Due to cross-references
 * in the configuration data, these signals have to be sent before the corresponding
 * CmacRbMappingConfigReq signals. Signals in the signal set must be sent in order of
 * transport channel identity. Otherwise the mappings are considered to be done with the
 * old radio bearers.
 */
typedef struct CmacUlTrChConfigReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    configMessageNo;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Boolean waitForActivationTime;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    activationTimeIndex;

    /** Indicates whether this signal is the first signal of a set of CmacUlTrChConfigReq
     * signals. If the value is TRUE, then all previous transport channels and their
     * configurations will be removed first.
     */
    Boolean                     firstSignal;

    /** Contains transport channel configuration information */
    UTransportChannelConfigInfo transportChannelConfigInfo;
#if defined(UPGRADE_3G_TDD128)
    /** Identifies Transport Format Combination Set which shall contain this TrCH (TDD only)*/
    Int8 tfcsId;
#endif /*UPGRADE_3G_TDD128*/
}
CmacUlTrChConfigReq;

/** Used to Copy signal, as a call to StateMachine might destroy
 * CmacUlTrChConfigReq signal
 */
typedef CmacUlTrChConfigReq CmacInternalResolveRbTrChConfig;

/** This signal is sent to UMAC by URRC to setup radio bearer multiplexing
 * information for uplink channels. This signal contains multiplexing information
 * for one radio bearer mapping option, thus URRC may have to send several signals
 * for each active radio bearer. Due to cross-references in the configuration data,
 * these signals have to be sent after the corresponding CmacUlTrChConfigReq signals.
 * Otherwise the mappings are done with the old transport channels.
 */
typedef struct CmacUlRbMappingConfigReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8           configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8           configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8           configMessageNo;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Boolean        waitForActivationTime;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8           activationTimeIndex;

    /** Identifies the bearer this signal applies to */
    BearerIdentity bearerIdentity;

    /** Indicates whether this signal is the first signal of a set of
    * CmacUlRbMappingConfigReq signals. If the value is TRUE, then all
    * previous uplink mappings for all radio bearers will be removed first.
    */
    Boolean firstSignal;

    /** Indicates whether the associated bearer is in transparent mode
     * (and therefore may require ciphering) and which other possible
     * mode (UM or AM) is configured (to allow UMAC to determine URLC header size)
     */
    T_UUL_RLC_Mode bearerMode;

    /** Valid only if isTm is TRUE. It indicates whether the bearer is in the CS
     * or PS domain. This parameter is required in order to determine whether the bearer should
     * be ciphered using CS or PS parameters. Note RB_0 never has a valid cnDomain.
     */
    UCN_DomainIdentity cnDomain;

    /** Defines the number of the RLC SDUs per TTI. When TRUE, there is 1 RLC SDU per TTI,
     * else the number of RLC SDUs is equal to the number of transport blocks.
     */
    Boolean segmentedTm;

    /** Maps uplink logical channels to transport channels (R99) or MAC-d flows (E-DCH)
     *  and informs UMAC about logical channel priorities
     */
    UUL_LogicalChannelMappings_r6 ulChannelMappings;

    /** Indicates a list of TFIs (rather than RLCsizeIndex) to accompany the UL channel
     * mappings, if an explicit list of RLC size indicies is present in the UL channel
     * mappings. Note that MAC has no concept of RLC size index, and RRC always translates
     * this to TFI before forwarding to MAC.
     */
    MacTfiList explicitTfiList[maxLoCHperRLC];
}
CmacUlRbMappingConfigReq;

#if defined(UPGRADE_3G_FDD)

/** This message is used to inform the UMAC of the compressed mode configuration.
 * This primitive is sent from RRC to PHY to configure the compressed mode
 * parameters.
 */
typedef struct CmacCompressedModeConfigReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    configMessageNo;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Boolean waitForActivationTime;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    activationTimeIndex;

    /** Indicates if compressed mode is being activated by the network
     * using the RRC message MEASUREMENT CONTROL
     */
    Boolean fromMeasurementControl;

    /** Indicates the number of elements in the compressed mode gap information
     * and dimensions the compressedModeGapInfo array

\assoc ARRAY \ref activateTgps */
    Int8                    numberActiveTgps;

    /** Defines the TGPS parameters */
    UTGPSActiveConfigParams activateTgps[CPHY_MAX_ACTIVE_TGPS];

    /** Defines the number of TGPSs to be removed
\assoc ARRAY \ref deactivateTgpsi */
    Int8                    numberDeactivateTgps;

    /** Indicates the TGPSs to remove */
   UTGPSDeactiveConfigParams  deactivateTgpsi[CPHY_MAX_ACTIVE_TGPS];
}
CmacCompressedModeConfigReq;
#endif /*UPGRADE_3G_FDD*/

/** @} */ /* End of SigCmacTrch group */

/** Contains the traffic volume measurements on each radio bearer */
typedef struct CmacTrafficMeasurementQuantityTag
{
    /** Indicates the bearer for which measurements are being reported */
    BearerIdentity            bearerIdentity;

    /** Contains (if valid, see 4.1.25) the total RLC buffer occupancy for this radio bearer */
    URLC_BuffersPayload       rlcBuffersPayload;

    /**  Contains (if valid, see 4.1.25) the average RLC buffer occupancy for this radio bearer */
    UAverageRLC_BufferPayload averageRlcBufferPayload;

    /** Contains (if valid, see 4.1.25) the variance of the RLC buffer occupancy for
     * this radio bearer
     */
    UVarianceOfRLC_BufferPayload varianceOfRlcBufferPayload;
}
CmacTrafficMeasurementQuantity;

/** Defines the transport channel types and identities on which measurements should be
 * performed
 */
typedef struct CmacTransportChannelTypeTag
{
    /** Defines the type of transport channel on which measurements are reported */
    UTransportChannelType     transportChannelType;

    /** Defines the identity of the transport channel on which measurements are reported */
    UTransportChannelIdentity transportChannelIdentity;
}
CmacTransportChannelType;

/** Defines the number of events and theire parameters which trigger
 * the measurement report
 */
typedef struct CmacTrChTrafficEventTag
{
    /** Indicates whether the event(s) defined which follow are for only a single
     * transport channel or are applied to all transport channels (If TRUE, the
     * following trCh is not used)
     */
    Boolean applyEventToAllTrChs;

    /** Identifies which transport channel the following event(s) are defined (\note
     * Not used if applies to all TrChs
     */
    CmacTransportChannelType trCh;

    /** Indicates the number of events which are applicable to this transport channel
\assoc ARRAY \ref trafficVolumeEventParam */
    Int8                     numberOfEvents;

    /** Gives the event type and threshold for each of the events indicated by numberOfEvents */
    UTrafficVolumeEventParam trafficVolumeEventParam[maxMeasParEvent];
}
CmacTrChTrafficEvent;

/** Defines measurement report event criteria. It defines number of transport channel on
 * which traffic measurement is reported and defines event that triggrs the measurement traffic.
 */
typedef struct CmacTrafficCriteriaEventTag
{
    /** Defines the number of transport channels which have triggering events (
     * for the given measurement identity)

\assoc ARRAY \ref trChEvent */
    Int8                 numberOfTransportChannels;

    /** Parameterises the event which triggers the measurement report */
    CmacTrChTrafficEvent trChEvent [UPS_MAX_UL_NO_OF_TRCH];
}
CmacTrafficCriteriaEvent;

/** Indicates how MAC should report traffic measurement (Trigger/Periodic) */
typedef union CmacTrafficCriteriaTag
{
    /** Defines the criteria for each transport channel, which trigger a
     * traffic volume report
     */
    CmacTrafficCriteriaEvent eventReportCriteria;

    /** Defines the period between, and the number of measurement reports UMAC is
     * to make to URRC
     */
    UPeriodicalReportingCriteria periodicalReportCriteria;

    /** For noReporting in Genie*/
    Int8 dummy;
}
CmacTrafficCriteria;

/** contains a list of (traffic volume) measurement identities to be reported
 * when this measurement identity is reported
 */
typedef struct CmacAdditionalTrafficIdentityListTag
{
    /** Indicated number of measurement reported */
    Int8                 numberOfMeasurementRequests;

    /** Identifies each measurement reported */
    UMeasurementIdentity measurementIdentity[maxAdditionalMeas];
}
CmacAdditionalTrafficIdentityList;

/** Contains measurement data for traffic measurement. It identifies the
 * measurement and carries traffic measurement for each defines radio bearer.
 */
typedef struct CmacAdditionalTrafficMeasurementTag
{
    /**  Identifies number used by the UTRAN to identify the measurement */
    UMeasurementIdentity measurementIdentity;

    /**  Identifies number used by the UTRAN to identify the measurement */
    Int8                 sequenceNumber;                /* same as in MeasReq */

    /** Indicates which the measurement types (total buffer occupancy, average or variance)
     * are present within each measurementQuantity
     */
    UTrafficVolumeReportingQuantity validity;

    /** Defines the number of bearers for which measurements are reported (and dimensions
     * the following measurementQuantity array)

\assoc ARRAY \ref measurementQuantity */
    Int8 noBearerMeasurements;

    /** Contains the traffic volume measurements on each radio bearer */
    CmacTrafficMeasurementQuantity measurementQuantity[maxRB];
}
CmacAdditionalTrafficMeasurement;

/** Contains the measurement data for each measurement identity
 * requested for the report from UMAC
 */
typedef struct CmacAdditionalTrafficMeasurementListTag
{
    /** Defines the number of measurements in the additional measurements array */
    Int8 numberOfMeasurements;

    /** Contains the measurement data for each additional measurement report passed to URRC */
    CmacAdditionalTrafficMeasurement add[maxAdditionalMeas];
}
CmacAdditionalTrafficMeasurementList;

/** \defgroup SigCmacMeas Measurement Primitives
 * This section defines the primitives exchanged between MAC and RRC for
 * performing measurements.
 *
 * @{
 */

/** This signal is sent to the UMAC by the URRC to request (configure) traffic
 * volume measurements
 */
typedef struct CmacTrafficMeasurementReqTag
{
    /** Identifies number used by the UTRAN to identify the measurement */
    UMeasurementIdentity measurementIdentity;

    /** Defines an additional identifier used between URRC and UMAC to avoid
     * mis-synchronisation (particularly if a previously configured measurement
     * on this identity used periodic reporting)
     */
    Int8 sequenceNumber;

    /** Defines which quantities are to be measured (total buffer payload, t
     * he average or variance of the buffer payload)
     */
    UTrafficVolumeReportingQuantity trafficVolumeReportingQuantity;

    /** Defines time intervals for average and variance measurements */
    UTrafficVolumeMeasQuantity      trafficVolumeMeasQuantity;

    /** Indicates whether radio bearer buffer statistics mapped to all transport
     * channels should be reported (note if this is TRUE, the following fields
     * numberOfTransportChannels and transportChannel are not used by UMAC)
     */
    Boolean measureAllTransportChannels;

    /** Defines the number of transport channels on which measurements should be made
\assoc ARRAY \ref transportChannel */
    Int8    numberOfTransportChannels;

    /** Defines the transport channel types and identities on which measurements
     * should be made
     */
    CmacTransportChannelType transportChannel[UPS_MAX_UL_NO_OF_TRCH];

    /** Indicates whether periodical, event triggered or no reporting should be used
     * (If no reporting is selected, UMAC still measures TVM on the specified transport
     * channels, but waits for CmacGetAdditionalTrafficMeasurementReq as the trigger to
     * report measurements to URRC)

\assoc UNION \ref reportCriteria */
    T_UTrafficVolumeReportCriteria reportCriteriaType;

    /** Defines the periodicity (for periodic reporting) or the event triggers
     * (for event triggered reporting) which will cause UMAC to report measurements
     * to URRC, if <I> no reporting </I> is selected in reportCriteriaType, this parameter
     * is not used
     */
    CmacTrafficCriteria reportCriteria;

    /** Contains a list of (traffic volume) measurement identities to be reported
     * when this measurement identity is reported
     */
    CmacAdditionalTrafficIdentityList additional;
}
CmacTrafficMeasurementReq;

/** @} */ /* End of SigCmacMeas group */

/** Indicate the events and transport channels which caused this
 * measurement report
 */
typedef struct CmacTrafficVolumeEventTag
{
    /** Defines the type of transport channel which caused the traffic volume event */
    UTransportChannelType     causingTransportChannelType;

    /** Identifies of the transport channel which caused the traffic volume event */
    UTransportChannelIdentity causingTransportChannelIdentity;

    /** Defines the type of event (4a or 4b) which caused the measurement report */
    UTrafficVolumeEventType   trafficVolumeEventType;
}
CmacTrafficVolumeEvent;

/** \defgroup SigCmacMeas Measurement Primitives
 * This section defines the primitives exchanged between MAC and RRC for
 * performing measurements
 *
 * @{
 */

/** This signal is used by the UMAC to inform the URRC of traffic measurement results*/
typedef struct CmacTrafficMeasurementIndTag
{
    /** Contains the measured results */
    CmacAdditionalTrafficMeasurement meas;

    /** Indicates whether the measurement is periodical or event triggered */
    UPeriodicalOrEventTrigger        periodicalOrEventTrigger;

    /** Indicate the events on and transport channels which triggered this
     * measurement report (\note ignored if above is periodical)
     */
    CmacTrafficVolumeEvent eventResults;

    /** Contains any additional measurement results associated with the measurement
     * identity in meas
     */
    CmacAdditionalTrafficMeasurementList additional;
}
CmacTrafficMeasurementInd;

/** This signal is used by the URRC to stop a measurement. Note that each measurement
 * identity will only be for traffic volume.
 */
typedef struct CmacTrafficMeasurementStopReqTag
{
    /** Identifies number used by the UTRAN to identify the measurement */
    UMeasurementIdentity measurementIdentity;

    /** This parameter is the same as in Measurement Req */
    Int8                 sequenceNumber;
}
CmacTrafficMeasurementStopReq;

/** When a traffic volume measurement is configured with <I> noReporting </I> (
 * rather than periodic or event triggered) a UMAC measurement may need
 * to be attached to a PHY measurement. This primitive is used by URRC
 * to request a UMAC measurement report for attachment to a measurement
 * report to UTRAN.
 */
typedef struct CmacGetAdditionalTrafficMeasurementReqTag
{
    /** Identifies number used by URRC to ensure sequencing is consistent
     * between CmacGetAdditionalTrafficMeasurementReq and
     * CmacGetAdditionalTrafficMeasurementCnf
     */
    Int8 getMeasReqId;

    /** Holds an array of measurement identities which URRC is requesting
     * to be reported back to URRC
     */
    CmacAdditionalTrafficIdentityList add;
}
CmacGetAdditionalTrafficMeasurementReq;

/** This message encapsulates the UMAC measurement report for attachment in cases
 * where triggering occurred from a different measurement type
 */
typedef struct CmacGetAdditionalTrafficMeasurementCnfTag
{
    /** Defines an identity number used by URRC to ensure sequencing is consistent
     * between CmacGetAdditionalTrafficMeasurementReq and
     * CmacGetAdditionalTrafficMeasurementCnf
     */
    Int8 getMeasReqId;

    /** Contains the measurement data for each measurement identity
     * requested for the traffic volume report from UMAC
     */
    CmacAdditionalTrafficMeasurementList meas;
}
CmacGetAdditionalTrafficMeasurementCnf;

/** @} */ /* End of SigCmacMeas group */

/** \defgroup SigCmacCiphering Ciphering Configuration Primitives
 * This section defines the primitives exchanged between MAC and RRC to
 * configure ciphering parameters. These primitives set ciphering configuration for
 * TM radio bearer.
 * @{
 */

/** This message is used to indicate to URRC an HFN change. The change
 * may be caused by the CFN wrapping, or when the HFN is initialised with
 * a START value. Note, the HFN is only maintained for TM radio bearers,
 * excluding RB_0.
 */
typedef struct CmacHfnMeasurementIndTag
{
    /** Contains current HFN data with the ksi that is currently used
     * for CS and PS domains
     */
    CmacHfnInfo hfnInfo;
}
CmacHfnMeasurementInd;

/** @} */ /* End of SigCmacCiphering group */

/** \defgroup SigCmacInternal Internal Primitives
 * This section defines the primitives sent internaly to MAC
 * The following signals are internal UMAC control signals, and are not seen
 * on the external interface
 * @{
 */

/* =========================================================================
 * The following signals are internal UMAC control signals, and are not seen
 * on the external interface
 * ========================================================================= */

/** Indication of timer expiry.
 * On receving a timer expiry, the measurement identity
 * is used to retreive the measurement configuration.
 * Measurements are then made for this measId.
 */
typedef struct CmacInternalTimerExpiryTag
{
    /** Indicates measurement identity */
    UMeasurementIdentity measurementIdentity;
}
CmacInternalTimerExpiry;

/** If the DL reconfiguration has been actioned, AND the UL
 * reconfiguration has been actioned, then tell UmaDl what
 * the UL TM configuration is, and that the entire configuration
 * is complete.
 */
typedef struct CmacInternalUlTmBearersTag
{
    /** Indicates if CS has TM bearer */
    Boolean      csHasTmBearers;

    /** Defines initial start value (Valid only if TM radio bearer is configured for CS) */
    USTART_Value csInitialHfn;

    /** Defines HFN value (Valid only if TM radio bearer is configured for CS) */
    USTART_Value csTmCountcHfn;

    /** Indicates if PS has TM bearer */
    Boolean      psHasTmBearers;

    /** Defines initial start value (Valid only if TM radio bearer is configured for PS) */
    Int32        psInitialHfn;

    /** Defines HFN value (Valid only if TM radio bearer is configured for PS) */
    USTART_Value psTmCountcHfn;
}
CmacInternalUlTmBearers;

/** @} */ /* End of SigCmacInternal group */

/* DEVELOPMENT_VERSION */

/** \defgroup SigCmacInternal Internal Primitives
 * This section defines the primitives sent internaly to MAC
 * The following signals are internal UMAC control signals, and are not seen
 * on the external interface
 * @{
 */

/** This signal is logged when CmacUlTransportChannelConfigReq or
 * CmacDlRBMappingConfigReq is processed by MAC
 */
typedef struct CmacInternalReconfigTag
{
    /** Indicates if uplink or downlink transport channel is processed */
    Boolean isUplink;
}
CmacInternalReconfig;

/** Defines TFC power information for debugging purpose */
typedef struct CmacDebugTfcInfoTag
{
    /** Identifies TFCI */
    Int8          tfci;

    /** Indicates the state of the defines TFCI */
    UmacCTFCstate tfcState;

#if defined(UPGRADE_3G_FDD)
    /** Defines LogPowerRelation (10log(N * Bd^2 / Bc^2)) (FDD)*/
    SignedInt8    logPowerRelation;

    /** Defines power required by the defined TFCI (FDD) */
    SignedInt8    tfcPowerRequired;

    /** Defines number of frames on which a TFC has excess power (FDD) */
    Int8          numberFramesExcessPower;
#endif /* UPGRADE_3G_FDD */

    /** Indicates duration (Tnotify + Tmodify+ TL1_proc)
     *<ul>
     * <li>Tnotify equals 15 ms, and
     *<li>Tmodify equals MAX(Tadapt_max,TTTI), and
     *<li>TL1 proc equals 15 ms, and
     *<li>Tadapt_max equals MAX(Tadapt_1, Tadapt_2, ..., Tadapt_N)
     *</ul>
     */
    Int8        tfcTmodTnotTl1proc;
#if defined(UPGRADE_3G_TDD128)
    /** Indicates the timeslot for which the TFC has changed its state (TDD128)*/
    Int8        timeslot;

    /** Defines power required by the defined TFCI (TDD128) */
    SignedInt16 tdd128TfcPowerRequired;

    /** Defines number of frames on which a TFC has excess power (TDD128) */
    Int8        tdd128NumberFramesExcessPower;
#endif /*UPGRADE_3G_TDD128*/
}
CmacDebugTfcInfo;

/** This signal is logged when a state of a given TFC is changed.
 * This signal can help determine why MAC did not select any TFC. */
typedef struct CmacDebugTfcTag
{
    /** Indicates number of TFCs whose state has changed
\assoc ARRAY \ref tfcInfo */
    Int8                                numberOfTfcs;

    /** Defines TFC power information */
    CmacDebugTfcInfo                    tfcInfo[UPS_MAX_UL_TFC];

    /** Defines power left for two radio frames */
    SignedInt8                          powerLeft[UMAC_NUMBER_SLOTS_FOR_CTFC_STATE];
#if defined(UPGRADE_3G_TDD128)
    UphyUlPowerRefListPerSubFrameTdd128 powerRefListSubFrame[TDD128_TFC_OBSERVATION_PERIOD]; /*(P+deltaP) from L1 for the last TDD128_TFC_RECOVERY_CRITERIA_Y measurement periods*/
#endif /*UPGRADE_3G_TDD128*/
}
CmacDebugTfc;

/** @} */ /* End of SigCmacInternal group */
typedef struct TestModeDebugModeTag
{
    /** Indicates if dataReq to be logged */
    Boolean dataReqOn;

    /** Indicates if dataInd to be logged */
    Boolean dataIndOn;

    /** Indicates if macHs to be logged */
    Boolean macHsOn;
}
TestModeDebugMode;

typedef struct TestModeDummyTag
{
    Boolean param1;
}
TestModeDummy;

#if defined(DEVELOPMENT_VERSION)
typedef struct TestModeInjectCrcsTag
{
    Int16 crcEachNblocks;   /**< 0 is never inject CRCs - to turn off */
    Int16 numCrcs;          /**< Number of consecutive CRCs */
}
TestModeInjectCrcs;
#endif /* DEVELOPMENT_VERSION */

# if defined(UMAC_DEBUG) && defined(ON_PC)
typedef struct TestModeSetRandTag
{
    Int16 values [CMAC_MAX_RAND_VALUES];                       /**< values that rand function will return in the next calls*/
}
TestModeSetRand;
# endif /* UMAC_DEBUG && ON_PC */
/** Sets test mode parameters */
typedef union CmacTestModeParametersTag
{
    /** Enable signal to be logged for debugging using */
    TestModeDebugMode                   debugMode;          /**< CMAC_TEST_MODE_DEBUG */
# if defined(DEVELOPMENT_VERSION)
    TestModeInjectCrcs                  injectCrcs;         /**< CMAC_TEST_MODE_INJECT_CRCS */
# else
    TestModeDummy                       dummyInjectCrcs;
# endif
    TestModeDummy                       dummyUnitTest;      /**< CMAC_TEST_MODE_UNIT_TEST */
    TestModeDummy                       dummyForceOutOfTmm; /**< CMAC_TEST_MODE_FORCE_OUT_OF_TMM */
#  if defined(UMAC_DEBUG) && defined(ON_PC)
    TestModeSetRand                     setRand;            /**< CMAC_TEST_MODE_SET_RAND */
#  else
    TestModeDummy                       dummySetRand;
#  endif /* UMAC_DEBUG && ON_PC */
}
CmacTestModeParameters;

/** \defgroup SigCmacInternal Internal Primitives
 * This section defines the primitives sent internaly to MAC
 * The following signals are internal UMAC control signals, and are not seen
 * on the external interface
 * @{
 */

typedef struct CmacTestModeReqTag
{
    /** Defines command to set test mode
\assoc UNION \ref parameters */
    CmacTestModeCommand    command;

    /** Configure test parameters */
    CmacTestModeParameters parameters;
}
CmacTestModeReq;

/* The following are test signals used for MAC testing */

# if defined(ON_PC) || defined(ON_TARGET_UNIT_TEST)

/** This signal is internaly sent if there are UL gaps */
typedef struct CmacInternalTestCmTag
{
    /** Defines the longest configured UL TTI */
    Int8                     numFramesInTti;

    /** Defines the mode of compressed mode */
    UUL_CompressedModeMethod compressedModeMethod[UPS_LONGEST_TTI_IN_FRAMES];

    /** Indicates number of gaps */
    Int8                     numberOfSlotGaps[UPS_LONGEST_TTI_IN_FRAMES];
}
CmacInternalTestCm;
# endif /*(ON_PC) || (ON_TARGET_UNIT_TEST)*/
/** @} */ /* End of SigCmacInternal group */
/* DEVELOPMENT_VERSION */

#if defined(UPGRADE_3G_HSDPA)

/** Includes information on any newly configured MAC-hs Queues*/
typedef struct CmacHsQueueInfoTag
{
    /** Defines the MAC-hs Queue identity (0-7) */
    Int8                mac_hsQueueId;

    /** Defines the MAC-d flow identity (0-7) */
    UMAC_d_FlowIdentity mac_dFlowId;

    /** Defines the reordering release timer (10ms to 400ms) */
    UT1_ReleaseTimer    reorderingReleaseTimer;

    /** Defines the MAC-hs window size (4-32) */
    UMAC_hs_WindowSize  mac_hsWindowSize;

    /** Defines number of mac-d PDU sizes
\assoc ARRAY \ref mac_d_PDU_SizeInfo */
    Int8                numberOfMacDpduSizes;

    /** Indicates the PDU size for a given PDU index (SID) */
    UMAC_d_PDUsizeInfo  mac_d_PDU_SizeInfo[maxMAC_d_PDUsizes];
}
CmacHsQueueInfo;

/** \defgroup SigCmacHsdpa HSDPA Primitives
 * This section defines the primitives exchanged between MAC and RRC for
 * establishing, re-configuring and releasing HSDPA flows.
 *
 * @{
 */

/** This signal configures new MAC-hs queues, or reconfigures already active queues.
 * This signal shall not be sent every time a parameter changes on CPHY / CMAC interface.
 * It shall only be sent when the network explicitly configures the MAC-hs queue.
 */
typedef struct CmacHsQueueConfigReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8            configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8            configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8            configMessageNo;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Boolean         waitForActivationTime;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8            activationTimeIndex;

    /** Indicates whether the MAC-hs entity should be reset*/
    Boolean         macHsReset;

    /** Indicates number of newly configured MAC-hs Queues
\assoc ARRAY \ref newQueue */
    Int8            numberOfNewQueues;

    /** Includes information on any newly configured MAC-hs Queues*/
    CmacHsQueueInfo newQueue[maxQueueIDs];
}
CmacHsQueueConfigReq;

/** This signal removes one or more MAC-hs queues from the active queue set */
typedef struct CmacHsQueueReleaseReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    configMessageNo;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Boolean waitForActivationTime;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    activationTimeIndex;

    /** Indicates number of MAC-hs queues are to be released
\assoc ARRAY \ref macHsQueueId */
    Int8    numberOfQueuesToDelete;

    /** Indicates which MAC-hs queues are to be released */
    Int8    macHsQueueId[maxQueueIDs];
}
CmacHsQueueReleaseReq;

/** @} */ /* End of SigCmacHsdpa group */
#endif /* UPGRADE_3G_HSDPA */

#if defined(UPGRADE_3G_HSUPA)
#if defined(UPGRADE_3G_FDD)

/** Contains the added or reconfigured E-DCH MAC-d flows */
typedef struct E_DCHAddReconfMACdFlowListFDDTag
{
    /** Indicates the MAC-d flow identity which may be mapped to E-DCH HARQ processes */
    Int8 macDflowId;

    /** Indicates (as a bit mask) the other MAC-d flows that are allowed to be included
        in the same MAC-e PDU. Each of the eight bits represents a MAC-d flow
        (this MAC-d flow being ignored) as per 25.331 10.3.5.1b
    */
    Int8 multiplexingList;

    /** Indicates the power offset to be applied if this MAC-d flow is included in the
        MAC-e PDU for E-DCH (Range is 0 to  6, in dB)
    */
    Int8 macDflowPowerOffset;

    /** Indicates the maximum number of retransmissions allowed for this MAC-d flow */
    Int8 macDflowMaxRetrans;

    /** Indicates if this MAC-d flow shall be used either in non-scheduled or scheduled transmissions */
    T_UE_DCH_AddReconf_MAC_d_Flow_r7_transmissionGrantType grantType;

    /** Indicates how large the non-secheduled MAC-e PDU may be (and the HARQ processes
        on which data from this MAC-d flow can be transmitted)
        This field is only present if grantType is non-scheduled
    */
    UE_DCH_AddReconf_MAC_d_Flow_r7_fdd non_SchedTrGrantInfo;
}
E_DCHAddReconfMACdFlowListFDD;

/** Contains information about the added or reconfigured E-DCH MAC-d flows in FDD
*/
typedef struct E_DCHAddReconfTrchInfoFDDTag
{
    /** Indicates the E-DCH TTI configured:  either 2 ms or 10 ms */
    UE_DCH_TTI                    eDchTtiLength;

    /** Indicates the number of added or reconfigured E-DCH MAC-d flows */
    Int8                          numberOfEDchMacDFlows;

    /** Contains the added or reconfigured E-DCH MAC-d flows */
    E_DCHAddReconfMACdFlowListFDD macDFlowList[maxE_DCHMACdFlow];
}
E_DCHAddReconfTrchInfoFDD;
# endif /*UPGRADE_3G_FDD*/

#if defined(UPGRADE_3G_TDD128)

/** Contains the added or reconfigured E-DCH MAC-d flows
*/
typedef struct E_DCHAddReconfMACdFlowListTDD128Tag
{
    /** Indicates the MAC-d flow identity which may be mapped to E-DCH HARQ processes */
    Int8 macDflowId;

    /** Indicates (as a bit mask) the other MAC-d flows that are allowed to be included
        in the same MAC-e PDU. Each of the eight bits represents a MAC-d flow
        (this MAC-d flow being ignored) as per 25.331 10.3.5.1b
    */
    Int8 multiplexingList;

    /** Indicates the power offset to be applied if this MAC-d flow is included in the
        MAC-e PDU for E-DCH (Range is 0 to  6, in dB)
    */
    Int8 macDflowPowerOffset;

    /** Indicates the maximum number of retransmissions allowed for this MAC-d flow */
    Int8 macDflowMaxRetrans;

    /** Indicates the timer value for this MAC-d flow if NACK is received for the
        initial transmission (In ms)
    */
    Int16 macDflowRetransTimer;

    /** Indicates if this MAC-d flow shall be used either in non-scheduled or scheduled transmissions */
    T_UE_DCH_AddReconf_MAC_d_Flow_r7_transmissionGrantType grantType;
}
E_DCHAddReconfMACdFlowListTDD128;

/** Contains information about the added or reconfigured E-DCH MAC-d flows in TDD128
*/
typedef struct E_DCHAddReconfTrchInfoTDD128Tag
{
    /** Indicates the number of added or reconfigured E-DCH MAC-d flows */
    Int8 numberOfEDchMacDFlows;

    /** Contains the added or reconfigured E-DCH MAC-d flows */
    E_DCHAddReconfMACdFlowListTDD128 macDFlowList[maxE_DCHMACdFlow];
}
E_DCHAddReconfTrchInfoTDD128;
# endif /*UPGRADE_3G_TDD128*/

/** Contains Contains information about the added or reconfigured E-DCH MAC-d flows
    depending on the mode (FDD or TDD128)
 */
typedef union E_DCHAddReconfTrchInfoUnionTag
{
#if defined(UPGRADE_3G_FDD)
    /** Contains information about the added or reconfigured E-DCH MAC-d flows in  FDD */
    E_DCHAddReconfTrchInfoFDD    fdd;
# endif /*UPGRADE_3G_FDD*/
#if defined(UPGRADE_3G_TDD128)
    /** Contains information about the added or reconfigured E-DCH MAC-d flows in TDD128 */
    E_DCHAddReconfTrchInfoTDD128 tdd128;
# endif /*UPGRADE_3G_TDD128*/
}
E_DCHAddReconfTrchInfoUnion;

/** \defgroup SigCmacHsupa HSUPA Primitives
 * This section defines the primitives exchanged between MAC and RRC for
 * establishing, re-configuring and releasing HSUPA flows.
 *
 * @{
 */

/** The #CmacEdchMacDflowListConfigReq always contains the full list of active E-DCH MAC-d flows
 */
typedef struct CmacEdchMacDflowListConfigReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8                        configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8                        configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8                        configMessageNo;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Boolean                     waitForActivationTime;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8                        activationTimeIndex;

    /**  Indicates for which mode (FDD or TDD128) this primitive is sent
\assoc UNION \ref eDchAddReconfTrchInfo */
    UmtsMode                    umtsMode;

    /**  Contains information about the added or reconfigured E-DCH MAC-d flows */
    E_DCHAddReconfTrchInfoUnion eDchAddReconfTrchInfo;
}
CmacEdchMacDflowListConfigReq;

/** @} */ /* End of SigCmacHsupa group */

# define CMAC_NO_PERIODIC_SCHED_INFO_REQUIRED            0
#if defined(UPGRADE_3G_FDD)
/** Contains the E-DCH channel information in  FDD
*/
typedef struct E_DCHInfoFDDTag
{
    /** happyBitDelayCondition enables MAC to set the Happy Bit according to
     * TS 25.321 section 11.8.1.5
     */
    UHappyBit_DelayCondition            happyBitDelayCondition;

    /** schedulingInfoPeriodNoGrant and
     * schedulingInfoPeriodGrant are used by
     * MAC to determine when scheduling information should be reported
     * according to TS 25.321 section 11.8.1.6
     * These are expressed in the number of TTIs between SI.
     * Note: If equal to CMAC_NO_PERIODIC_SCHED_INFO_REQUIRED, no periodic SI is required
     */
    Int16                               schedulingInfoPeriodNoGrant;
    Int16                               schedulingInfoPeriodGrant;

    /** eDpcchDpcchPowerOffset is delta E-DPCCH as specified in table 1B in
     * section 4.2.1.3 of TS 25.213. This allows MAC to determine betaEc,
     * given betaC in a PhyEdchTtiInd.
     */
    Int8                                eDpcchDpcchPowerOffset;
    /** powerOffsetForSchedInfo is required by MAC in order to send scheduling
     * information at the correct power offset.
     */
    Int8                                powerOffsetForSchedInfo;

    /** referenceEtfcis contains the HARQ power offsets and allows MAC to
     * calculate the betaEd value (required in PhyEdchDataReq) given the
     * selected transport block size and the E-DPDCH spreading factor given
     * the formula in TS25.214 section 5.1.2.5B.2 and TS25.213 table 1B.1 in
     * section 4.2.1.3
     */
    UE_DPDCH_Reference_E_TFCIList_r7    referenceEtfcis;

    /** tfciTableIndex indicates which TFCI table is to be used (in conjunction
     * with E-DCH TTI length - TS 25.321 Annex B (0.1, 10.3.6.99 25.331)
     */
    Int8                                tfciTableIndex;

    /** minimumSetEtfci allows the definition of a set of E-TFCI's which can
     * always be selected, no matter how power constraints may limit the E-TFCS
     */
    Int8                                minimumSetEtfci;

    /** maxChannelisationCode indicates the maximum number of channelisation
     * codes allowed to be used for E-DCH.
     * This limites the E-TFCS (i.e. the maximum E-DCH transport block size).
     */
    EdchSf                              maxChannelisationCode;

    /** sfBoundaries indicates the maxTBSize to use the SF and number of channels combination
     * This allows MAC to know which SF and number of channels are used for each E-TFC,
     * and enables MAC to pass this selection to L1 in the PhyEdchDataReq.
     * RRC computes this on the basis of the puncturing limit (PLmax/PLnon-max) and maxChannelisationCode
     * received in E-DPDCH Info IE, and the calculations set out in TS 25.212 sections 4.8.1 to 4.8.4
     */
    EdchSfBoundary                      sfBoundaries[EDCH_SLOTS_PER_FRAME+1-EDCH_MIN_TX_SLOTS_PER_10MS_TTI_FDD];
    /** The maximum UL transmit power including UE capability, and UTRAN configuration */
    UMaxAllowedUL_TX_Power              maxAllowedUL_TX_Power;

    /** Scheduling Information is triggered due to a change in serving E-DCH radio
     * link (25.321 - 11.8.1.6.2) */
    Boolean                             edchServRlChangeSiTrigger;
}
E_DCHInfoFDD;
# endif /*UPGRADE_3G_FDD*/

#if defined(UPGRADE_3G_TDD128)

/** Contains the information for the E-RUCCH channel
*/
typedef struct E_RUCCHInfoTag
{
    /** Indicates the timer value for E-RUCCH transmission. If T-RUCCH expires before Grant
        is received then UE should send scheduling information again on E-RUCCH
    */
    Int16                     tRucch;

    /** Indicates the maximum number of retransmissions of scheduling information on E-RUCCH */
    Int8                      nRucch;

    /** If T-WAIT expires then UE should send scheduling information on E-RUCCH */
    Int16                     tWait;

    /** If periodic T-SI expires then UE should include scheduling information in a new MAC-e PDU */
    Int16                     tSI;

    /** Indicates the number of Access Service Classes which has been defined for E-RUCCH */
    Int8                      numberOfAccessServiceClasses;

    /** Indicates the number of scaling factors contained in persistenceScalingFactor */
    Int8                      numberOfPersistenceScalingFactors;

    /** Contains the persistence scaling factors for the ASCs */
    UPersistenceScalingFactor persistenceScalingFactor[maxASCpersist];

    /** Indicates the Maximum number of synchronisation attempts
        Note: if Mmax is not sent by the network for E-RUCCH, RRC will fallback to another
            relevant value (decline from RACH parameters, default value, ...)
    */
    Int8 Mmax;

    /** Contains dynamic persistence level value from the UTRAN.
     * Only in the case that the UEs with E-DCH and E-RUCCH configured on the
     * Primary Frequency of the multi-frequency cell, the E-DCH serving cell
     * unchanged and the UEs can obtain the dynamic persistence level N
     * from SIB7 according to the configuration of information element
     * "E-RUCCH info", the dynamic persistence value N shall be used.
     * Otherwise, dynamic persistence level N=1 shall always be used.
     */
    UDynamicPersistenceLevel dynamicPersistenceLevel;
}
E_RUCCHInfo;

/** Contains the information for the E-PUCH channel
*/
typedef struct E_PUCHInfoTag
{
    /** Indicates the Betas and Code Rates */
    UE_TFCS_Info eTfcsInfo;

    /** Indicates the minimum Code Rate. The calculations set out in TS25.224 section 4.2.2.4.1
        (Represented in 1/1000th, Range is therefore 55 to 1000 and means 0.055 to 1)
    */
    Int16 minAllowedCodeRate;

    /** Indicates the maximum code rate. The calculations set out in TS25.224 section 4.2.2.4.1
        (Represented in 1/1000th, Range is therefore 55 to 1000 and means 0.055 to 1)
    */
    Int16 maxAllowedCodeRate;

    /** Indicates the maximum number of retransmissions allowed for this MAC-d flow.
        Only used when no MAC-d PDUs are included in the same MAC-e PDU
    */
    Int8 maximumNumOfRetransSchedInfo;

    /** Indicates the timer value if NACK is received for the initial transmission (In ms).
        Only used when no MAC-d PDUs are included in the same MAC-e PDU
    */
    Int16   retransTimerForSchedInfo;

    /** Indicates if the powerOffsetForSchedInfo is present */
    Boolean powerOffsetForSchedInfoPresent;

    /** Indicates the power offset to be applied if this MAC-d flow is included in the MAC-e PDU for E-DCH.
        Only used when no MAC-d PDUs are included in the same MAC-e PDU (Range is 0 to  6, in dB)
    */
    Int8 powerOffsetForSchedInfo;

    /** Indicates if the tSiNs is present */
    Boolean             tSiNstPresent;

    /** Indicates if T-SI-NST expires then UE should include scheduling information in the next new nonscheduled
        MAC-e PDU.*/
    UNon_ScheduledTransGrantInfoTDD_ext_t_SI_nst  tSiNst;
}
E_PUCHInfo;

/** Contains the E-DCH channel information in TDD128
*/
typedef struct E_DCHInfoTDD128Tag
{
    /** Indicates the presence of the eRucchInfo field */
    Boolean                isERucchPresent;

    /** Contains the information for the E-RUCCH channel */
    E_RUCCHInfo            eRucchInfo;

    /** Contains the information for the E-PUCH channel */
    E_PUCHInfo             ePuchInfo;

    /** Indicates the Uplink Transmission Power allowed in the UE */
    UMaxAllowedUL_TX_Power maxAllowedUL_TX_Power;

    /** Indicates the E-DCH Physical Layer Category as defined in 25.306 table 5.1m */
    Int8                   tdd_edch_PhysicalLayerCategory;
}
E_DCHInfoTDD128;
# endif /*UPGRADE_3G_TDD128*/

/**  Contains the E-DCH channel information
*/
typedef union E_DCHInfoUnionTag
{
#if defined(UPGRADE_3G_FDD)
    /** Contains the E-DCH channel information in  FDD */
    E_DCHInfoFDD    fdd;
# endif /*UPGRADE_3G_FDD*/
#if defined(UPGRADE_3G_TDD128)
    /** Contains the E-DCH channel information in TDD128 */
    E_DCHInfoTDD128 tdd128;
# endif /*UPGRADE_3G_TDD128*/
}
E_DCHInfoUnion;

/** \defgroup SigCmacHsupa HSUPA Primitives
 * This section defines the primitives exchanged between MAC and RRC for
 * establishing, re-configuring and releasing HSUPA flows.
 *
 * @{
 */

/** This message is sent by RRC to MAC to configure MAC-D flows for E-DCH
 * transport channels. This signal profides UMAC information necessary to
 * perform ETFC selection.
 */
typedef struct CmacEdchConfigReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    configMessageNo;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Boolean waitForActivationTime;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    activationTimeIndex;

    /** resetMac_es_e indicates that the MAC-es/e entity should be reset
     * (TS25.321 section 11.8.1.7)
     */
    Boolean        resetMac_es_e;

    /**  Indicates for which mode (FDD or TDD128) this primitive is sent
\assoc UNION \ref eDchInfo */
    UmtsMode       umtsMode;

    /**  Contains the E-DCH channel information */
    E_DCHInfoUnion eDchInfo;

    /** Indicates if the handover have been configured */
    Boolean        handoverPresent;
}
CmacEdchConfigReq;

/** This signal informs UMAC of the release of the E-DCH channels. This message is sent
 * by RRC to MAC to release all E-DCH channels and related configurations.
 * This signal has no impact on the uplink dedicated channels.
 */
typedef struct CmacEdchReleaseReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8 configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8 configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8 configMessageNo;

    /** Indicates whether MAC should activate a configuration immediately
     * (waitForActivationTIme = FALSE) or wait for L1 to indicate activation
     * of a pending configuration (waitForActivaiton = TRUE)
     */
    Boolean waitForActivationTime;

    /** Used to check whether the correct pending configuration has been activated */
    Int8    activationTimeIndex;
}
CmacEdchReleaseReq;

/** @} */ /* End of SigCmacHsupa group */
#endif /* UPGRADE_3G_HSUPA */

#if defined (UPGRADE_3G_MBMS)
#if defined (UPGRADE_3G_TDD128)

/** Indicate the type of a logical MBMS channel.
 */
typedef enum CmacMbmsChannelTypeTag
{
    CMAC_MBMS_MCCH = 0,                  /**< The logical channel type is MCCH */
    CMAC_MBMS_MTCH = 1,                  /**< The logical channel type is MTCH */
    CMAC_MBMS_MSCH = 2                   /**< The logical channel type is MSCH */
}
CmacMbmsChannelType;

/** \defgroup SigCmacMbms MBMS Primitives
 * This section defines the primitives exchanged between MAC and RRC for
 * establishing, re-configuring and releasing MBMS mappings.
 *
 * @{
 */

/** This signal is sent to the UMAC by the URRC to setup radio bearer
 * multiplexing information for downlink MBMS bearers. This signal contains
 * multiplexing information for one radio bearer mapping option, thus URRC may
 * have to send several signals for each active MBMS radio bearer.
 */
typedef struct CmacDlMbmsRbMappingConfigReqTag
{
    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    configIdentifier;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    configNoOfMessages;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    configMessageNo;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Boolean waitForActivationTime;

    /** See the \ref SigCmacParDesc in the CMAC Signalling Interface section */
    Int8    activationTimeIndex;

    /** Identifies the MBMS bearer this signal applies to. If mbmsChannelType
     * is set to CMAC_MBMS_MCCH then bearerIdentity is set to RB_MCCH. If
     * mbmsChannelType is set to CMAC_MBMS_MTCH then bearerIdentity is in the
     * range RB_MTCH_01 to RB_MTCH_32. If mbmsChannelType is set to
     * CMAC_MBMS_MSCH then bearerIdentity is in the range RB_MSCH_01 to RB_MSCH_02.
     */
    BearerIdentity bearerIdentity;

    /** Indicates whether this signal is the first signal of a set of
     * CmacDlMbmsRbMappingConfigReq signals. If the value is TRUE, then all
     * previous MBMS mapping configurations will be removed first.
     */
    Boolean             firstSignal;

    /** Identifies the CCTRCH the MBMS channel is mapped to */
    Int8                mbmsTransportChannelId;

    /** Indicates whether the associated MBMS bearer is for MCCH, MTCH or MSCH. */
    CmacMbmsChannelType mbmsChannelType;

    /** mbmsId is only valid if mbmsChannelType is set to CMAC_MBMS_MTCH. This
     * identifier is used to distinguish different MTCH mapped on to an MBMS
     * transport channel. It is called "MBMS-Id" within the MAC header and
     * "MBMS logical channel identity" within ASN.1. The parameter has values
     * in the range of 1 to 15.
     */
    UMBMS_LogicalChIdentity mbmsId;

    /** tctfPresence is set to TRUE if UTRAN includes a TCTF field in the
     * MAC header for this channel. This parameter is always set to TRUE
     * if mbmsChannelType is set to CMAC_MBMS_MTCH.
     */
    Boolean tctfPresence;
}
CmacDlMbmsRbMappingConfigReq;

/** @} */ /* End of SigCmacMbms group */
#endif /* UPGRADE_3G_TDD128 */
#endif /* UPGRADE_3G_MBMS */

/** \defgroup SigCmacModeSelect Umts Operating Mode Primitives
 * This section defines the primitives exchanged between MAC and RRC for
 * selecting the operating mode for MAC (either FDD or TDD128).
 * <B> MSC below shows transactions between MAC and RRC during activation and deactivation</B>
 *
 *\msc
 * msc {
 *  "URRC","UMAC";
 *  --- [label="Activation in FDD mode"];
 *  "URRC" -> "UMAC" [label="CmacModeSelectReq (UMTS_FDD)"];
 *  "URRC" -> "UMAC" [label="(... any operation allowed in FDD mode ...)"];
 *  "URRC" -> "UMAC" [label="CmacDeactivateReq"];
 *
 *
 *  --- [label="Activation in TDD128 mode"];
 *  "URRC" -> "UMAC" [label="CmacModeSelectReq (UMTS_TDD128)"];
 *  "URRC" -> "UMAC" [label="(... any operation allowed in TDD128 mode ...)"];
 *  "URRC" -> "UMAC" [label="CmacDeactivateReq"];
 * }
 *\endmsc
 *
 * @{
 */

/** This primitive indicates in which mode (FDD or TDD128) MAC shall operate.
 * This signal is sent to the UMAC by the URRC to activate UMAC in a
 * specific mode (either FDD or TDD128).
 */
typedef struct CmacModeSelectReqTag
{
    /** Indicates in which mode (FDD or TDD128) MAC shall operate. (After receiving
     * this message, MAC shall consider following primitives to be of the same mode,
     * and shall ignore following primitives, that either explicitly indicates, or
     * are dedicated to the other Mode.)
     */
    UmtsMode umtsMode;
}
CmacModeSelectReq;

/** @} */ /* End of SigCmacModeSelect group */

/** @} */ /* End of SigCMAC group */
#endif /*CMAC_SIG_H*/

/* END OF FILE */
