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
 * Types used in Mph primitives.
 **************************************************************************/

#ifndef MPH_TYP_H
#define MPH_TYP_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include  <system.h>
#include  <stddef.h>
#include  <mpsi_typ.h>
#include  <l13_typ.h>


/****************************************************************************
 * Manifest Constants
 ****************************************************************************/

/** Invalid BSIC value.
 * Special value for BSIC to be used in #MphBsicDecodeCnf when a neighbour
 * cell BSIC cannot be decoded
 */
#define INVALID_BSIC      0xFF

/***************************************************************************
 * Macros
 **************************************************************************/

#if !defined(DISABLE_FAST_MA_COPYING)
/** Fast copy of a populated #FrequencyDefinition structure.
 * Macro to efficiently copy one \a FrequencyDefinition to another.
 * This will only copy the used portion of the ma list, and hence
 * saves copying operations for \a ma lists that are shorter than
 * #MAX_MA_CHANNELS.
 *
 * \param dEST  Pointer to structure to populate with copied values.
 * \param sRC   Pointer to source structure from which to copy values.
 *
 * \note Where possible this function should be used in place of an assignment
 *       between \a FrequencyDefinition structures.
 */
#define M_CopyFrequencyDefinition(dEST,sRC)         \
{                                                   \
    DevAssert((sRC)->maSize <= MAX_MA_CHANNELS);    \
    memcpy(dEST,                                    \
           sRC,                                     \
           offsetof(FrequencyDefinition, ma) +      \
           (sizeof(Arfcn) * (sRC)->maSize) );       \
}
#else
#define M_CopyFrequencyDefinition(dEST,sRC)         \
    memcpy(dEST, sRC, sizeof(FrequencyDefinition))
#endif

/***************************************************************************
 * Types
 **************************************************************************/

/** Recoverable error reporting.
 * This enumerator is used during channel assignment or handover to report
 * either that the operation was successful, or if it was unsuccessful which
 * error caused it to fail.
 */
typedef enum L1ErrorTag
{
    /**
     * No error.  The request was handled correctly.
     */
    L1_SUCCESS,

    /**
     * A #MphBcchDecodeReq was received but reception cannot be started.  This
     * may occur because Layer 1 either has no valid timing information for the
     * requested cell, or the control bitmap was incorrectly formatted.
     */
    L1_CANT_DECODE_BCCH,

    /**
     * Generally used to indicate that Layer 1 is not in the correct state to
     * handle some request.
     */
    L1_INTERNAL_ERROR,

    /**
     * BCCH decoding was attempted, but the received blocks could not be decoded.
     *  This is commonly seen in an #MphErrorInd in poor signal conditions.
     */
    L1_BCCH_DECODE_ERROR,

    /**
     * Frequency definition contains ARFCNs not supported by the current band mode.
     */
    L1_FREQ_NOT_IMPLEMENTED,

    /**
     * Frequency definition contains ARFCNs not supported by the current band
     * mode, and a starting time has elapsed.  Maybe used in #MphHandoverCnf.
     */
    L1_FREQ_NOT_IMPLEMENTED_ELAPSED,

    /**
     * A specified or calculated timing advance value was out of range.
     * Timing advance is normally specified in bits, and can range from
     * 0 to 63.
     *
     * For handover, this may indicated that the timing advance calculated
     * for finely or pseudo-synchronised handover is out of range and the
     * parameter ignoreOutOfRangeTa was not set.
     */
    L1_TA_OUT_OF_RANGE,

    /**
     * Layer 1 has no synchronisation information for the specified cell.
     */
    L1_NO_TIMING_INFORMATION,

    /* Development Only */
    L1_CCCH_DECODE_ERROR,
    L1_SACCH_DECODE_ERROR,
    L1_SDCCH_DECODE_ERROR,
    L1_FACCH_DECODE_ERROR,
    L1_PCH_DECODE_ERROR,

    /* GPRS */
    L1_PTCCHD_DECODE_ERROR,
    L1_PDTCH_DECODE_ERROR,

    L1_SBLOCK_START_ERROR,
    L1_NO_MORE_SBLOCK_BUFFS,
    L1_NO_SBLOCK_CHANNEL_INFO,
    L1_SBLOCK_BAD_FREQ_DEF,

    L1_CANT_DECODE_PBCCH,
    L1_PBCCH_DECODE_ERROR,
    L1_TBF_NO_SLOTS,
    L1_TBF_TOO_MANY_TX_SLOTS,
    L1_TBF_TOO_MANY_RX_SLOTS,
    L1_TBF_TOO_MANY_TOTAL_SLOTS,
    L1_TBF_TX_AND_RX_SLOTS_CLASH,
    L1_PCCCH_CONFIG_ERROR,
    L1_PCCCH_DECODE_ERROR,

    /* AMR  */
    L1_AMR_UNKNOWN,
    L1_AMR_RATSCCH_DECODE_ERROR,
    L1_AMR_BAD_THRESH_ORDER,
    L1_AMR_BAD_HYST_ORDER,
    L1_AMR_BAD_UNUSED_THRESH,
    L1_AMR_BAD_ACS_SIZE,
    L1_AMR_INVALID_HR_CODEC,
    L1_AMR_ICM_OUT_OF_RANGE,
    L1_AMR_NO_CONFIG,
    L1_AMR_ACK,

    L1_ILLEGAL_ALLOCATION,

    /*
    ** error code returned if an attempt is made to initiate an
    ** un-supported EGPRS Switched Radio Block Loopback configuration.
    */
    L1_EGPRS_LOOPBACK_NOT_SUPPORTED,

    /* Dual Mode */
    L1_CANT_DECODE_SIB,

    /* Release 99 */
    L1_FAILURE_TO_SYNC,

    L1_LAST_ERROR
}
L1Error;

/** Neighbour cell operation priority.
 * This enumerator is used to set decoding priority for neighbour cells in
 * respect of BCCH and BSIC information
 */
typedef enum NcellPriorityTag
{
    NCELL_LOW_PRIORITY          = 0,
    NCELL_HIGH_PRIORITY         = 1,
/* value 2 - NCELL_CURSOR_PRIORITY removed when cursor code removed  */
#if defined (UPGRADE_ENHANCED_QUAD_BAND)
    NCELL_PLMN_SEARCH_PRIORITY  = 3,
#endif
    LAST_NCELL_PRIORITY
}
NcellPriority;

/** Status of a cells CBCH.
 * This enumerator is used during cell allocation to report whether the
 * selected cells broadcast channel is enabled, disabled or not present.
 */
typedef enum CbchDataStatusTag
{
    BCH_CB_ENABLED,         /**< CBCH is enabled */
    BCH_CB_DISABLED,        /**< CBCH is disabled */
    BCH_CB_NOT_PRESENT,     /**< CBCH is not supported on the cell */
#if defined (ENABLE_EXT_CBCH)
    BCH_EXT_CB_ENABLED,     /**< Extended CBCH is enabled */
    BCH_EXT_CB_DISABLED,    /**< Extended CBCH is disabled */
#endif
    LAST_CB_STATUS
}
CbchDataStatus;


/** Ciphering parameters.
 * This enumerator is used during dedicated channel setup, or to enable or
 * disable ciphering on an already dedicated channel. It specifies whether
 * or not to use ciphering and if used, which cipher algorithm and key.
 */
typedef struct CipheringParamTag
{
    Boolean                         useCiphering; /**< TRUE is ciphering enabled 
\assoc PRESENT \ref algorithm
\assoc PRESENT \ref key */
    CipherAlgorithm                 algorithm;    /**< Algorithm to use */
    CipheringKey                    key;          /**< Key to use */
}
CipheringParam;


/** Frequency parameters.
 * This structure specifies the frequency-hopping parameters to use on a
 * dedicated channel or a broadcast channel when receiving cell broadcast.
 * For a non-hopping channel maSize is 1 and the ARFCN is given in ma[0].
 * For a hopping channel maSize is the number of channels used contains
 * the ARFCNs for the channels used.
 */
typedef struct FrequencyDefinitionTag
{
    /** Number of Arfcns in hop sequence (1 = not hopping) 
\assoc ARRAY \ref ma */
    Int8                            maSize;

    /** Mobile Allocation Index Offset (0-(N-1), where N is the number of
     * frequencies in the ma array)
     */
    Int8                            maio;

    /** Hopping Sequence (generator) number (0-63) */
    Int8                            hsn;

    /** Bit mask used in hop calculation.  This is constant for a given ma
     * so is calculates in the background task */
    Int8                            nBinMask;

    /** Training sequence code. Range 0->7 */
    Int8                            tsc;

    /** Frequencies to use.
     * Only the first element is used when not hopping.
     * \note This must be the last element in the structure for
     *       M_CopyFrequencyDefinition() to function correctly,
     *       so do not re-arrange this structure.
     */
    Arfcn                           ma[MAX_MA_CHANNELS];
}
FrequencyDefinition;


/** Serving cell dedicated measurements.
 * This structure contains measurements made on the serving cell, which are
 * typically accumulated over a GSM reporting period.
 */
typedef struct ScellDedMeasurementsTag
{
    /** Full RX level for the serving cell.
     * This value can be added to -110 to get the dBm figure that is the RX
     * level observed for all received blocks on the serving cell.
     */
    Int8                            rxLevFull;

    /** Sub RX level for the serving cell.
     * This is the same as \a rxLevFull, but only averages measurements made
     * on the mandatory blocks that are defined in 05.08, section 8.3.
     * If the network is using Discontinuous Transmission on the downlink,
     * this value will accurately indicate the actual receive level while
     * \a rxLevFull will include results from blocks on which the network
     * may not have transmitted.
     */
    Int8                            rxLevSub;

    /** Full receive quality for the serving cell.
     * The receive quality is a value between 0 and 7 and indicates a
     * quantised bit error rate, as according to the following table,
     * reproduced from 05.08, section 8.2.4.
     *
     * <TABLE>
     * <TR><TD>RXQUAL_0</TD><TD>     </TD><TD>    BER&lt;</TD><TD> 0.2%</TD><TD>Assumed value = 0.14%</TD></TR>
     * <TR><TD>RXQUAL_1</TD><TD> 0.2%</TD><TD>&lt;BER&lt;</TD><TD> 0.4%</TD><TD>Assumed value = 0.28%</TD></TR>
     * <TR><TD>RXQUAL_2</TD><TD> 0.4%</TD><TD>&lt;BER&lt;</TD><TD> 0.8%</TD><TD>Assumed value = 0.57%</TD></TR>
     * <TR><TD>RXQUAL_3</TD><TD> 0.8%</TD><TD>&lt;BER&lt;</TD><TD> 1.6%</TD><TD>Assumed value = 1.13%</TD></TR>
     * <TR><TD>RXQUAL_4</TD><TD> 1.6%</TD><TD>&lt;BER&lt;</TD><TD> 3.2%</TD><TD>Assumed value = 2.26%</TD></TR>
     * <TR><TD>RXQUAL_5</TD><TD> 3.2%</TD><TD>&lt;BER&lt;</TD><TD> 6.4%</TD><TD>Assumed value = 4.53%</TD></TR>
     * <TR><TD>RXQUAL_6</TD><TD> 6.4%</TD><TD>&lt;BER&lt;</TD><TD>12.8%</TD><TD>Assumed value = 9.05%</TD></TR>
     * <TR><TD>RXQUAL_7</TD><TD>12.8%</TD><TD>&lt;BER    </TD><TD>     </TD><TD>Assumed value =18.10%</TD></TR>
     * </TABLE>
     *
     * The full receive quality includes all received blocks, so tends to be
     * high (i.e. of poor quality) during periods where the network is utilising
     * discontinuous transmission on the downlink.
     */
    Int8                            rxQualFull;

    /** Sub-set receive quality for the serving cell.
     * This is similar to the \a rxQualFull, but only contains the measurements
     * of the mandatory sub-set of radio blocks.  It is similar to the
     * \a rxLevSub parameter, but for Bit Error Rate in place of RX level.
     */
    Int8                            rxQualSub;

    /** #TRUE if DTX was used on the uplink during the last measurement period. */
    Boolean                         dtxUsed;

    /** The timing advance value used during the last reporting period. */
    Int8                            timingAdv;

    /** Current transmit Power Control Level.
     * This value is a GSM PCL, as would be supplied to L1CfgGetTxHalfRamp().
     */
    Int8                            txPowerLevel;

    /** Mean Bit Error Probability (BEP)
     * Enhanced Measurement Reporting (EMR)
     */
    Int8                            meanBep;
    /** Coefficient of Variation of the Bit Error Probability
     * Enhanced Measurement Reporting (EMR)
     */

    Int8                            cvBep;
    /**  The number of correctly decoded blocks, as defined for RXLEV_VAL */
    Int8                            nbrRcvdBlks;

    /**  Received level Value
     *  RXLEV measured on SACCH frames and on the 4 last time slots
     *  of each fully received and correctly decoded data block,
     * whether the DTX was used in downlink or not
     */

    Int8                            rxLevVal;
}
ScellDedMeasurements;

typedef struct NcellDedMeasurementsTag
{
    Int8                            baInd;
    Int8                            rxLev[MAX_BA_CHANNELS];
/** \assoc ARRAY \ref sortedIndexList */
    Int8                            lengthSortedList;
    Int8                            sortedIndexList[MAX_BA_CHANNELS];
}
NcellDedMeasurements;

typedef struct ScellAdditionalMeasTag
{   /* For DEVELOPMENT_VERSION only */
    Int32                       bitErrQualityFull;
    Int32                       bitErrQualitySubset;
}
ScellAdditionalMeas;

/** Description of a Common Control Channel.
 */
typedef struct CcchDescriptionTag
{
    /* Arfcn of C0 carrier of serving cell. */
    Arfcn                  arfcn;

    /* Defines if CCCH is combined with SDCCH + SACCH. */
    Boolean                combinedCcch;

    /** CCCH group of the MS. Timeslot = { 0, 2, 4, 6 }. */
    Int8                   timeslot;

    /** Initial value for the downlink signalling failure counter.
     * Range 10 - 45.
     */
    Int8                   dscInitValue;

    /** Page group of the MS. Range 0- 81. */
    Int8                   pagingGroup;

    /** Number of blocks per 51-frame multiframe not used for paging
     * on CCCH channel. Range 0-7.
     */
    Int8                   bsAgBlksRes;

    /** Page period in 51-frame multiframes.
     * The Mobile Station MS should receive 1 paging block during this period.
     * Range 2 - 9.
     */
    Int8                   bsPaMfrms;

    /** Circuit switched paging mode. */
    PageMode               pageMode;

    /** Band mode for operations.
     * This is used in the interpretation of #Arfcn values; since PCS and DCS
     * overlap, the \a bandMode is needed to resolve and ambiguity.
     */
    BandMode               bandMode;

    /** Whether CBCH is on, off, or not available. */
    CbchDataStatus         cbchDataStatus;

    /** Channel type for the cell broadcast. */
    ChannelType            cbchType;

    /** Timeslot on the carrier in which CBCH is present. */
    Int8                   cbchTimeslot;

    /** Frequency configuration for CBCH.
     * \note The training sequence field of this structure (present only with
     *       GPRS) is not used.  Instead the TSC for the CBCH is stored in
     *       \a CcchDescription.cbchTsc.
     */
    FrequencyDefinition    cbchFreqDef;

    /** Training sequence code for CBCH channel. */
    Int8                   cbchTsc;
}
CcchDescription;

#if defined (UPGRADE_AMR) || defined (UPGRADE_AMR_PS)
/** AMR phase information.
 * \note Values of enum tags reflect the CMIP bit in the CMI_PHASE_REQ message.
 */
typedef enum AmrPhaseTag
{
  AMR_PHASE_EVEN = 0,
  AMR_PHASE_ODD  = 1
}
AmrPhase;

/** AMR codec rates.
 */
typedef enum AmrCodecRateTag
{
  AMR_MR_475 = 0,                               /**< AMR codec rate enumeration for 4.75 kbit/s codec rate. */
  AMR_MR_515 = 1,                               /**< AMR codec rate enumeration for 5.15 kbit/s codec rate. */
  AMR_MR_590 = 2,                               /**< AMR codec rate enumeration for 5.9  kbit/s codec rate. */
  AMR_MR_670 = 3,                               /**< AMR codec rate enumeration for 6.7  kbit/s codec rate. */
  AMR_MR_740 = 4,                               /**< AMR codec rate enumeration for 7.4  kbit/s codec rate. */
  AMR_MR_795 = 5,                               /**< AMR codec rate enumeration for 7.95 kbit/s codec rate. */
  AMR_MR_102 = 6,                               /**< AMR codec rate enumeration for 10.2 kbit/s codec rate. */
  AMR_MR_122 = 7                                /**< AMR codec rate enumeration for 12.2 kbit/s codec rate. */
}
AmrCodecRate;

/** AMR configuration data.
 */
typedef struct AmrConfigTag
{
    Int8             sizeOfAcs;                     /**< Number of rates in the active codec set. 
\assoc ARRAY \ref acs */
    Int8             initialCodecMode;              /**< Initial codec mode to use. */
    AmrCodecRate     acs[MAX_AMR_ACS_SIZE];         /**< Set of codec rates to use as active codec set. */
    Boolean          threshPresent;                 /**< If TRUE, codec rate adaptation thresholds are present. 
\assoc PRESENT \ref thresh */
    Int16            thresh[MAX_AMR_THRESHOLD_SIZE];/**< Codec rate adaption thresholds for switching between different rates of active codec set. */
    Boolean          phasePresent;                  /**< If TRUE, \a phase is valid 
\assoc PRESENT \ref phase */
    AmrPhase         phase;                         /**< Initial phase for downlink in-band data (CMI/CMC) selection. */

    /** Noise Suppression Control Bit.
     * \internal For future use: noise suppression not currently supported.
     */
    Boolean          nscb;
}
AmrConfig;
#endif  /* UPGRADE_AMR_PS */

/** Initiate GSM Handover procedure.
 * This is sent by RR to initiate handover. For non-hopping channels the
 * dedicated channel ARFCN is contained in element 0 of the
 * #FrequencyDefinition mobile allocation list.
 */
typedef struct L1GsmHandoverReqTag
{
    /** Type of handover to execute.
     * This changes how the Timing Advance is calculated and the when
     * Access Bursts are sent on the new channel.
     */
    Synchronisation                 synchronisation;

    /** Type for the new physical channel */
    ChannelType                     type;

    /** Mode for the new channel */
    ChannelMode                     mode;

    /** Sub-channel.
     * If the #ChannelType supports sub-channels, this identifies the
     * sub-channel on which to start access.
     */
    Int8                            subchannel;

    /** Timeslot number. 0 to 7 */
    Int8                            timeslot;

    /** Training sequence code. 0 to 7 */
    Int8                            tsc;

    /** Starting time for procedure */
    ReducedFrameNumber              startingTime;

    /** Arfcn or hopping parameters.
     * Non-hopping ARFCN stored in element [0] of ma array.
     */
    FrequencyDefinition             freqDef;

    /** Initial power control level */
    Int8                            powerLevel;

    /** Whether Discontinuous Transmission should be used on new channel */
    Boolean                         applyDtx;

    /** Radio link timeout value 0, 4, 8 ... 64 */
    Int8                            radioLinkTimeout;

    /** Timing advance value.
     * Specified for pre-synchronised handover types.
     */
    Int8                            timingAdvance;

    /** Whether to include sCell in measurements.
     * If TRUE C0 not included in serving-cell measurements.
     */
    Boolean                         pwrc;

    /** Ciphering on/off, algorithm and key */
    CipheringParam                  cipherParam;

    /** Handover reference to send in access burst */
    Int8                            reference;

    /** If FALSE then transmit RACH after handover */
    Boolean                         atc;

    /** Control ignoring of out-of-range TA values.
     * If TRUE then out-of-range TA values calculated for finely and
     * pseudo-synchronised handovers can be ignored.
     */
    Boolean                         ignoreOutOfRangeTa;

    /** Used to calculate TA for pseudo-synchronised handover */
    Int8                            realTimeDifference;

    /** New serving cell BCCH ARFCN */
    Arfcn                           c0Arfcn;

    /** New serving cell BSIC */
    Bsic                            bsic;

    /** Frame to start using new frequency definition */
    ReducedFrameNumber              redefStartingTime;

    /** New frequency definition */
    FrequencyDefinition             redefFreqDef;

#if defined (UPGRADE_AMR_PS)
    /** If TRUE, #amrConfig is valid 
\assoc PRESENT \ref amrConfig */
    Boolean                         amrConfigPresent;

    /** AMR specific configuration options */
    AmrConfig                       amrConfig;
#endif
}
L1GsmHandoverReq;

#endif /* !defined MPH_TYP_H */

/* END OF FILE */
