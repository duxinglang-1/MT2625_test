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
 *************************************************************************/
/** \file
 * 3G L1 Primitive Data Type definitions for the UPHY Interface
 *************************************************************************/

#if !defined (UPHY_SIG_H)
#define       UPHY_SIG_H

/***************************************************************************
* Nested Include Files
***************************************************************************/

#include <ups_typ.h>

/***************************************************************************
*   Manifest Constants
***************************************************************************/

/** \defgroup SigUphy UPHY Signalling Interface
 * \ingroup Layer13G
 * \ingroup Sig
 *
 * This section defines all the primitives exchanged on the data plane of MAC
 * and 3G L1, hereinafter called the UPHY interface, to enable transmission and
 * reception of data and for synchronisation on the uplink and the downlink.
 *
 * The UPHY interface is illustrated in the figure below:
 *
 * \image html prd3gdualmodeinterface.png "Fig: Dual Mode Architecture"
 * \image latex prd3gdualmodeinterface.eps "Dual Mode Architecture" width=12cm
 *
 * All the primitive definitions are based on the recommendations defined in
 * <tt>[3G TS 25.302 section 10]</tt> with additions to fill any areas not
 * covered by these recommendations.
 *
 * The timing of the primitives in downlink and uplink is as explained below.
 *
 * <B> Timing in Downlink </B>
 *
 * The following figure illustrates how the #PhyDataInd reports the actual
 * frame number of when the BCCH data was received.
 *
 * \image html prd3gdownlinktiming.png "Fig: Timing of downlink primitives"
 * \image latex prd3gdownlinktiming.eps "Timing of downlink primitives" width=12cm
 *
 * The position of the #PhyDataInd relative to the received data will be
 * physical layer dependent.
 *
 * The timing in dedicated mode is exactly the same except that frame numbers
 * are reported using the CFN.
 *
 * <B> Timing in Uplink </B>
 *
 * The following figure illustrates the timing relationship for data
 * transmission.
 *
 * \image html prd3guplinktiming.png "Fig: Timing of uplink primitives"
 * \image latex prd3guplinktiming.eps "Timing of uplink primitives" width=12cm
 *
 * The timing of the #PhyFrameInd is such that the protocol stack can respond
 * with the #PhyDataReq before the end of the current frame. The above indicates
 * a typical physical layer solution where the data is encoded in the next
 * frame and transmitted in the following frame. The frame delay from receipt
 * of the #PhyDataReq and transmitting the data will be physical layer
 * dependent. The protocol stack must know the actual delay, as the MAC needs
 * to know the transmission frame number for ciphering. In the above example
 * the MAC ciphers the above data with CFN=3.
 *
 *
 * <B> List of signals in UPHY interface </B>
 *
 * <TABLE>
 * <TR><TD> #PhyAccessReq   </TD></TR>
 * <TR><TD> #PhyAccessCnf   </TD></TR>
 * <TR><TD> #PhyDataInd     </TD></TR>
 * <TR><TD> #PhyFrameInd    </TD></TR>
 * <TR><TD> #PhyDataReq     </TD></TR>
 * <TR><TD> #PhyHsDataInd   </TD></TR>
 * <TR><TD> #PhyEdchTtiInd  </TD></TR>
 * <TR><TD> #PhyEdchDataReq </TD></TR>
 * <TR><TD> #PhyStateInd    </TD></TR>
 * <TR><TD> #PhyStatusInd   </TD></TR>
 * </TABLE>
 * @{
 */

/** Power left indicated in a #PhyFrameInd primitive for a slot where there
 * is a compressed mode gap.
 */
#define UPHY_POWER_LEFT_TX_OFF              SCHAR_MIN

/** Length of data shown in #PhyDebugDataInfoInd and #PhyDebugDataInfoReq
 * primitives.
 */
#define UPHY_DEBUG_DATA_LENGTH              10

/* Used in PHY_DATA_IND and PHY_FRAME_IND */
#define INVALID_ACTIVATION_TIME_INDEX     0xFF

/* default value for undefined or assgined spreading factor */
#define INVALID_SF 0xFFFF

#if defined(UPGRADE_3G_HSDPA)
/** Define the maximum assigned HS pointers by UMAC at the same time.
 */
#define UPHY_MAXIMUM_ASSIGNED_HS_POINTERS             3
#endif

/***************************************************************************
*   Macro Functions
***************************************************************************/

/***************************************************************************
*   Types
***************************************************************************/

/** Indicates the various states in which the PHY can reside.
 * \c PHY_STATE_IDLE also encompasses the RRC states CELL_PCH and URA_PCH.
 */
typedef enum UPhyStateTag
{
    /** PHY is in NULL state. */
    PHY_STATE_NULL                          = 0,

    /** PHY is in IDLE state. */
    PHY_STATE_IDLE                          = 1,

    /** PHY is in CELL_FACH state. */
    PHY_STATE_FACH                          = 2,

    /** PHY is in CELL_DCH state. */
    PHY_STATE_DCH                           = 3

} UPhyState;

/** Indicates the result (success/failure) of the #PhyAccessReq primitive.
 */
typedef enum URachResultTag
{
    /** Data was sent successfully. */
    UPHY_RACH_RESULT_DATA_SENT_SUCCESSFULLY = 0,

    /** Maximum power reached during ramping cycles. */
    UPHY_RACH_RESULT_REACHED_MAX_POWER      = 1,

    /** NACK received on AICH for PRACH preamble, and is only used in FDD. */
    UPHY_RACH_RESULT_NACK_ON_AICH_RECEIVED  = 2,

    /** Timer when no acknowledgment is received. */
    UPHY_RACH_RESULT_TIMEOUT                = 3,

    /** RACH procedure was aborted by MAC with \c PhyAccessReq primitive. */
    UPHY_RACH_RESULT_ABORT                  = 4,

    /** This value is used only to allow <tt> GCF test case 7.1.2.4a [3G TS
     * 34.123] </tt> pass.
     */
    UPHY_RACH_RESULT_NO_SUBCHANNEL_ASSIGNED = 5
}
URachResult;


/** Indicates the event occurred in the physical layer, which is to be notified
 * to the higher layers.
 */
typedef enum UEventValueTag
{
    /** Failure in hardware */
    PHY_EV_HARDWARE_FAILURE                 = 0,

    /** Failure to decode BCH */
    PHY_EV_BCH_DECODE_FAILURE               = 1,

    /** Failure to detect SFN */
    PHY_EV_SFN_DETECT_FAILURE               = 2,

    /** Failure to decode PCH */
    PHY_EV_PCH_DECODE_FAILURE               = 3,

    /** Failure to decode FACH */
    PHY_EV_FACH_DECODE_FAILURE              = 4,

    /** Failure to decode DCH */
    PHY_EV_DCH_DECODE_FAILURE               = 5
}
UEventValue;


/** Indicates layer 1 status in #PhyDataInd primitive.
 */
typedef enum UphChStatusTag
{
    /** Status is OK */
    UPHCH_STATUS_OK                         = 0,

    /** Failure to decode TFCI */
    UPHCH_STATUS_TFCI_DECODE_FAIL           = 1,

    /** Failure to decode in BTFD */
    UPHCH_STATUS_BTFD_FAIL                  = 2
}
UphChStatus;



#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** FDD specific info for AccessReq.
 */
typedef struct UphyAccessReqFDDTag
{
    /** Indicates the Transport Format Combination Index to be used for sending
     * the data. Range 0 to (#UPS_MAX_UL_TFC - 1).
     */
    Int16                                   tfci;

    /** Indicates the gain factor
     * <em style='font-family:Symbol'>b</em><em><sub><small>c</small></sub></em>
     * to be used in weighing the control part of PRACH for the selected TFC.
     */
    UGainFactor                             gainFactorBetaC;

    /** Indicates the gain factor
     * <em style='font-family:Symbol'>b</em><em><sub><small>d</small></sub></em>
     * to be used in weighing the data part of PRACH for the selected TFC.
     */
    UGainFactor                             gainFactorBetaD;

    /** Indicates the spreading factor used in the physical layer, and it is in
     * the range of 32 to 256.
     */
    USpreadingFactor                        spreadingFactor;

    /** Indicates the power offset between the last transmitted preamble and
     * the control part of the message. This is generally added to the
     * preamable power to receive the power of the message control part.
     */
    UPowerOffsetPp_m                        powerOffsetPp_m;
    
}
UphyAccessReqFDD;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/** Unionised info for AccessReq.*/
typedef union UphyAccessReqUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** parameters in fdd */
    UphyAccessReqFDD    fdd;
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** dummy : no parameters in tdd128 since gainFactorBeta=1 and 
     * and SF to be used is only in L1 once FPACH ACK received */
    Int8                 tdd128;
#endif
} UphyAccessReqUnion;


/** Access to a RACH transport channel request or to an E-RUCCH physical channel request.
 * This primitive is used by the MAC to request access to a RACH transport
 * channel from the physical layer or to a E-RUCCH (only in TDD128 when HSUPA
 * Scheduled Transmission exists), and to send data to the physical layer to
 * be sent in the PRACH channel or in the E-RUCCH channel if required.
 * This primitive differs from that in <tt>[3G TS 25.302 section 10]</tt> in 
 * including data to be sent in the RACH or in the E-RUCCH, so that it is available
 * to the physical layer at the beginning of the random access procedure. 
 * This primitive initiates the random access procedure.
 * \note For RACH:Unlike #PhyDataReq data, the TMM data block passed to the physical
 * layer in #PhyAccessReq is not de-allocated by the interface adapter, as MAC
 * may attempt retransmission of the same data (due to back-off or timeout
 * waiting for response on AICH (FDD) and FPACH(TDD128)).
 *
 * \note For E-RUCCH (HSUPA TDD128): The E-RUCCH data may be changed for retransmission, 
 * thus the interface handler does not perform similar handling as for RACH data.
 * The E-RUCCH TB is of fixed length 39 bits: 23 bits for SI and 16 bits for E-RNTI. 
 * (This parameter is kept in L1 only -no need in MAC- and L1 inserts it in the last 16 bits 
 * of the E-RUCCH TB).
 */
typedef struct PhyAccessReqTag
{
    /** Indicates the Access Service Class to be employed in the physical layer
     * during random access. Range 0 to 7.
     */
    Int8                                    accessServiceClassIndex;

    /** umtsMode is used to determine which mode the unionised information 
     * elements refer to either FDD or TDD128.   
\assoc UNION \ref mode */
    UmtsMode                                umtsMode; 

    /** mode specific info for AccessReq.*/
    UphyAccessReqUnion                      mode;
    
#if defined(DATA_IN_SIGNAL)

    /** Indicates all of the transport blocks for this TTI. And it is used for
     * testing with genie to provide visibility of the data in the primitive. */
    UUlPduList                              pduList;
#else
    /** Indicates a reference to a TMM data block, the protocol stack is
     * responsible for allocation of static memory during initialisation. One
     * static \c UUlPduList is required per CCTrCh, but the same \c UUlPduList
     * may also be used for #PhyDataReq. L1 shall not modify any of the data
     * referenced by \c pduList_p except the parameter \c isConsumed.
     */
    UUlPduList                            * pduList_p;
#endif /* DATA_IN_SIGNAL */
}
PhyAccessReq;


/** Access to a RACH transport channel confirmation or to an E-RUCCH physical channel
 * confirmation.
 * This primitive is used to confirm that data has been sent in the RACH
 * transmission or in the E-RUCCH transmission (only in TDD128 when HSUPA 
 * Scheduled Transmission exists)if data was sent using the PhyAccessReq primitive.
 * This primitive is always sent, whether transmission is successful, unsuccessful
 * (NACK on AICH in FDD, or timeout waiting for response on AICH in FDD, or timeout 
 * waiting for response on FPACH in TDD128).
 */
typedef struct PhyAccessCnfTag
{
    /** Indicates the result of the random access attempt.*/
    URachResult                             rachResult;
}
PhyAccessCnf;



#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** FDD specific info for DataReq.*/
typedef struct UphyDataReqFddTag
{
    /** Indicates the TFCI in use is for null data rate. */
    Boolean                                 isTfciNoData;

    /** Indicates the Transport Format Combination Index to be used for sending
     * the data. Range 0 to (#UPS_MAX_UL_TFC - 1).
     */
    Int16                                   tfci;

    /** Indicates the gain factor
     * <em style='font-family:Symbol'>b</em><em><sub><small>c</small></sub></em>
     * used for weighing the DPCCH.
     * This gain factor is always the gain factor used with no compressed mode,
     * even during compressed frames.
     */
    UGainFactor                             gainFactorBetaC;

    /** Indicates the gain factor
     * <em style='font-family:Symbol'>b</em><em><sub><small>d</small></sub></em>
     * used for weighing the DPDCHs.
     * This gain factor is always the gain factor used with no compressed mode,
     * even during compressed frames.
     */
    UGainFactor                             gainFactorBetaD;

    /** Indicates the spreading factor for the selected TFC. MAC ensures that
     * the TFC selected is not lower than the minimum configured spreading
     * factor. 
     *
     * For non compressed mode:
     *       MAC sets compressedSpreadingFactor = INVALID_SF
     *
     * For compressed mode Mode by spreading factor reduction (SF/2) :
     *       MAC always sets compressedSpreadingFactor = nonCompressedSpreadingFactor/2
     * 
     * For compressed mode Mode by higher layer scheduling:
     *      MAC will either set
     *        compressedSpreadingFactor = nonCompressedSpreadingFactor/2 or
     *        compressedSpreadingFactor = nonCompressedSpreadingFactor depending on the TFCI selected and 
     *                                                     number of compress mode slots 
     */
    USpreadingFactor                        nonCompressedSpreadingFactor;
    USpreadingFactor                        compressedSpreadingFactor;
  
#if defined(UPS_CFG_UL_DATA_CLASS_768KBPS)
    Int8                                    numberOfDpdchs;
#endif /* UPS_CFG_UL_DATA_CLASS_768KBPS */

} UphyDataReqFdd;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** UphyChannelListPerSlotTdd128 contains the physical channels allocation for a timeslot */
typedef struct UphyChannelListPerSlotTdd128Tag
{
    /** timeslotPosition indicates the uplink timeslot considered. */
    Int8                        timeslotPosition;
    
    /** numberOfChannels indicates the max number of physical channels in the timeslot. */
    Int8                        numberOfChannels;
    
    /** channelCodes is an array containing the spreading factors and codes of each physical channel in the considered Timeslot, in the order of the channels.*/
    UUL_TS_ChannelisationCode   channelCodeList[UUL_CHANNELS_PER_SLOT_FOR_TDD128];
} 
UphyChannelListPerSlotTdd128;

/** UphyCcTrChInfoList contains all information related to the selected TFC by MAC for a CCTrCH.
 */
typedef struct UphyCcTrChInfoListTag
{
    /** Identity of the CCTrCh*/
    Int8                                    tfcsId;

    /** Indicates the Transport Format Combination Index to be used for sending
     * the data. Range 0 to (#UPS_MAX_UL_TFC - 1).
     */
    Int16                                   tfci;

    /** Indicates the gain factor for the selected TFC
     */
    UGainFactor                             gainFactorBeta;

    /** numberOfUlSlots indicates the number of uplink timeslots used for the selected TFC. */
    Int8                                    numberOfUlSlots;
    
    /** ChannelListPerSlot indicates the number of channels used for the selected TFC. */
    UphyChannelListPerSlotTdd128            phyChannelListPerSlot[MAX_UL_TIMESLOT];

} 
UphyCcTrChInfoList;

/** TDD128 specific info for DataReq.
 */
typedef struct UphyDataReqTdd128Tag
{
    /** Info related to the selected TFC for the CCTrCH.*/
    UphyCcTrChInfoList                     ccTrChInfoList;
}
UphyDataReqTdd128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/** Unionised info for DataReq.
 */
typedef union UphyDataReqUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
  UphyDataReqFdd    fdd;
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
  UphyDataReqTdd128 tdd128;
#endif
} 
UphyDataReqUnion;

/** Transmit data on physical channel request.
 * This primitive is used to send data from the MAC to the physical layer for
 * transmission. One #PhyDataReq primitive is sent every shortest configured
 * transmission time interval. One #PhyDataReq primitive is used for all
 * configured transport channels. There is one #PhyDataReq primitive per coded
 * composite transport channel (DPCHs).
 */
typedef struct PhyDataReqTag
{
    /** umtsMode is used to determine which mode the unionised information 
     * elements refer to either FDD or TDD128.   
\assoc UNION \ref mode */
    UmtsMode                                 umtsMode; 

    /** mode specific info for DataReq.*/
    UphyDataReqUnion                         mode;

#if defined (DATA_IN_SIGNAL)
    /** Indicates all of the transport blocks for this TTI. And it is used for
     * testing with genie to provide visibility of the data in the primitive.
     */
    UUlPduList                              pduList;
#else /* DATA_IN_SIGNAL */
    /** Indicates a reference to a TMM data block, the protocol stack is
     * responsible for allocation of static memory during initialisation. One
     * static \c UUlPduList is required per CCTrCh, but the same \c UUlPduList
     * may also be used for #PhyAccessReq.
     */
    UUlPduList                            * pduList_p;
#endif /* DATA_IN_SIGNAL */

    /** Indicates the CFN at which the data should be sent on air. */
    Int8                                    ulCfn;
}
PhyDataReq;



/** Data received indication.
 * This primitive is used to send data from the physical layer to the MAC.
 * One #PhyDataInd primitive is sent every shortest configured transmission
 * time interval for all transport channels in CELL_DCH and CELL_FACH. There is
 * one #PhyDataInd primitive per coded composite transport channel (P-CCPH,
 * S-CCPCH, DPCHs). This primitive is also used to synchronise new downlink
 * configurations (with respect to activation time).
 */
typedef struct PhyDataIndTag
{
    /** Indicates the downlink frequency of the cell on which data is being
     * indicated to RRC. Together with \c primaryScramblingCode RRC will
     * determine if the BCH data is from the serving or a neighbour cell.
     */
    UUARFCN                                 uarfcn_DL;
    
    /** umtsMode is used to determine which mode the unionised information 
     * elements refer to either FDD or TDD128.   
\assoc UNION \ref cellIdentification */
    UmtsMode                                umtsMode;
    /** mode specific for CellIdentification.*/
    CellIdentificationUnion                 cellIdentification; 
    
    /* Activation Time Index */
    Int8                                    activationTimeIndex;    

    /** Indicates either the SFN or CFN in which this data was received. Note
     * that in the case the transport channel was longer than a 10ms TTI, the
     * last frame occupied by this TTI defines the frame number. This parameter
     * is used by MAC (and passed on to RLC and RRC) for synchronisation of
     * new downlink configurations with respect to activation time.
     */
    UFrameNumber                            dlFrameNumber;

    /** Indicates if the status is <tt> ok/TFCI failure/BTFD failure</tt>. */
    UphChStatus                             phChStatus;
#if defined (DATA_IN_SIGNAL)
    /** Indicates all of the transport blocks for this TTI. And it is used for
     * testing with genie to provide visibility of the data in the primitive.
     */
    UDlPduList                              pduList;
#else /* DATA_IN_SIGNAL */
    /** Indicates a reference to a TMM data block. L1 is responsible for
     * allocation of static memory during initialisation. One static
     * UDlPduList is required per CcTrCh.
     */
    UDlPduList                            * pduList_p;
#endif /* DATA_IN_SIGNAL */
}
PhyDataInd;

#if defined(UPLANE_INTEGRATION_TEST)
/** #PhyDataDisInd is a DATA_IN_SIGNAL only version of #PhyDataInd.
 *  It is used for user plane integration testing when TMM memory is used
 *  and the user wants to allow phyDataInd siganls to be generated from
 *  genie test scripts (which only support DATA_IN_SIGNAL).  This signal
 *  is handled by a stubs task which converts the signal into a TMM equivalent 
 *  PhyDataInd signal.
 */
typedef struct PhyDataDisIndTag
{
    /** Indicates the downlink frequency of the cell on which data is being
     * indicated to RRC. Together with \c primaryScramblingCode RRC will
     * determine if the BCH data is from the serving or a neighbour cell.
     */
    UUARFCN                                 uarfcn_DL;
    
    /** umtsMode is used to determine which mode the unionised information 
     * elements refer to either FDD or TDD128.   
\assoc UNION \ref cellIdentification */
    UmtsMode                                umtsMode;

    /** mode specific info for CellIdentification.*/
    CellIdentificationUnion                 cellIdentification; 
    
    /* Activation Time Index */
    Int8                                    activationTimeIndex;    

    /** Indicates either the SFN or CFN in which this data was received. Note
     * that in the case the transport channel was longer than a 10ms TTI, the
     * last frame occupied by this TTI defines the frame number. This parameter
     * is used by MAC (and passed on to RLC and RRC) for synchronisation of
     * new downlink configurations with respect to activation time.
     */
    UFrameNumber                            dlFrameNumber;

    /** Indicates if the status is <tt> ok/TFCI failure/BTFD failure</tt>. */
    UphChStatus                             phChStatus;

    /** Indicates all of the transport blocks for this TTI. And it is used for
     * testing with genie to provide visibility of the data in the primitive.
     */
    UDlPduList                              pduList;
}
PhyDataDisInd;
#endif /* UPLANE_INTEGRATION_TEST */

/* DEVELOPMENT_VERSION */

/** Log #PhyDataInd primitive.
 * Allows logging of the #PhyDataInd primitive sent by L1 to the protocol
 * stack. This primitive is only logged if the number of transport blocks
 * received in a given TTI is greater than zero. Each primitive logged contains
 * the first <tt> UPHY_DEBUG_DATA_LENGTH </tt> bytes of the data. Logging is
 * turned on using \c RLCdec by selecting <tt> Filter > MAC > PhyDataInd </tt>.
 */
typedef struct PhyDebugDataInfoIndTag
{
    UUARFCN                                 uarfcn_DL;
/** \assoc UNION \ref cellIdentification */
    UmtsMode                                umtsMode;
    CellIdentificationUnion                 cellIdentification;
    UFrameNumber                            dlFrameNumber;
    UphChStatus                             phChStatus;
/** \assoc ARRAY \ref pduListInfo */
    Int8                                    numberOfTransportBlocks;
    UDlPduListInfo                          pduListInfo[UPS_MAX_DL_NO_OF_TB_PER_TTI];
/** \assoc ARRAY \ref data */
    Int16                                   byteLength;
    Int8                                    data[UPHY_DEBUG_DATA_LENGTH];
}
PhyDebugDataInfoInd;


/** Log #PhyDataReq primitive.
 * Allows logging of the #PhyDataReq primitive sent to L1 by the protocol
 * stack. This primitive is only logged if the number of transport blocks
 * transmitted in a given TTI is greater than zero. Each primitive logged
 * contains the first <tt> UPHY_DEBUG_DATA_LENGTH </tt> bytes of the data.
 * Logging is turned on using \c RLCdec by selecting <tt> Filter > MAC >
 * PhyDataReq </tt>.
 */
typedef struct PhyDebugDataInfoReqTag
{
    Int8                                    cfn;
/** \assoc UNION \ref mode */
    UmtsMode                                umtsMode; 
    UphyDataReqUnion                        mode;
/** \assoc ARRAY \ref pduListInfo */
    Int8                                    numberOfTransportBlocks;
    UUlPduListInfo                          pduListInfo[UPS_MAX_UL_NO_OF_TB_PER_TTI];
/** \assoc ARRAY \ref data */
    Int16                                   byteLength;
    Int8                                    data[UPHY_DEBUG_DATA_LENGTH];
}
PhyDebugDataInfoReq;
/* DEVELOPMENT_VERSION */

/* DEVELOPMENT_VERSION */
typedef struct PhyDebugDlPduListTag
{
    UDlPduList                            pduList;
} 
PhyDebugDlPduList;

typedef struct PhyDebugUlPduListTag
{
    UUlPduList                            pduList;
} 
PhyDebugUlPduList;
/* DEVELOPMENT_VERSION */

/** Status indication to MAC.
 * This primitive is used by the physical layer to indicate to the MAC that
 * an event has occurred.
 */
typedef struct PhyStatusIndTag
{
    /** Indicates an enumeration constant #UEventValue, which describes the
     * event occurred.
     */
    UEventValue                             eventValue;
}
PhyStatusInd;

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** UphyUlPowerRefListPerSubFrameTdd128 contains power for the 2 subframes of a radio frame.*/
typedef struct UphyUlPowerRefListPerSubFrameTdd128Tag 
{
    /** (P+deltaP) values for each timeslot of the subframe*/
    SignedInt16                             Pref[MAX_UL_TIMESLOT];
}
UphyUlPowerRefListPerSubFrameTdd128;

/** UphyCcTrChPowerList contains all information related to the transmission of a CCTrCH
 * over a 10 ms radio frame.
 */
typedef struct UphyCcTrChPowerListTag 
{
    /** Identity of the CCTrCh*/
    Int8                                    tfcsId;
    /** Channel is ready to send */
    Boolean                                 dchReadyToSend;
    /** Power for the 2 subframes in a frame*/
    UphyUlPowerRefListPerSubFrameTdd128     powerRefListPerSubFrame[TDD128_SUBFRAME_PER_FRAME];
}
UphyCcTrChPowerList;

/** Tick from L1 as a complement to #PhyFrameInd
 * for TDD128 radio subframe (5ms).
 */
typedef struct PhySubFrameIndTag 
{
    /** Indicates the uplink CFN used for synchronisation of uplink
     * configurations (using activation time) in MAC (and subsequently RLC and
     * RRC). It also indicates that the physical layer is requesting data for
     * CFN \c ulCfn (to which MAC replies with a #PhyDataReq, if a new TTI has
     * started).
     */
    Int8                                    ulCfn;
}
PhySubFrameInd;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** FDD specific info for FrameInd.
 */
typedef struct UphyFrameIndFddTag 
{
    /** Indicates whether synchronisation has been achieved on dedicated
     * channels, and the SRB delay is complete. Once it is set TRUE, MAC starts
     * transmitting on the DCH transport channels. If synchronisation is lost,
     * it is set FALSE and MAC stops requesting more data to be sent.
     */
    Boolean                                 dchReadyToSend;

    /** Indicates the power difference (in dB) between the maximum allowed
     * power and the power required to transmit the control part in the
     * previous radio frame. In CELL_DCH it indicates the calculated power
     * remaining on DPCCH prior to clipping in each of the slots in the
     * previous frame as
     * <em>P<sub><small>max</small></sub> -
     * P<sub><small>DPCCH</small></sub></em>.
     * In CELL_FACH it indicates
     * the same value for each slot as
     * <em>P<sub><small>max</small></sub> -
     * P<sub><small>reamble_Initial_Power</small></sub></em>.
     * <tt> [3G TS 25.331 section 8.5.7]</tt>.
     * <em>P<sub><small>max</small></sub></em> is the minimum of
     * the maximum allowed power and the maximum UE transmitter power. The value
     * range is between the minimum and maximum power output of the UE <tt> [3G
     * TS 25.101 sections 6.2.1 and 6.4.3.1] </tt> i.e. less than -50 dBm to up
     * to 33 dBm. If the transmitter was off due to compressed mode gaps in the
     * previous frame, L1 should use the special value <tt>
     * UPHY_POWER_LEFT_TX_OFF </tt> in those slots. Where HS-DPCCH may also be
     * transmitted, layer 1 must include the difference these transmissions make
     * on the powerLeft calculation, i.e. the calculation for each slot becomes
     * <em>P<sub><small>max</small></sub> -
     * P<sub><small>DPCCH</small></sub> -
     * P<sub><small>HS-DPCCH</small></sub></em>.
     */
    SignedInt8                              powerLeft[UPS_SLOTS_PER_FRAME];
}
UphyFrameIndFdd;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** TDD128 specific info for FrameInd.
 */
typedef struct UphyFrameIndTdd128Tag
{
    /** Power used in each TS of a radio frame for a CCTrCH.*/
    UphyCcTrChPowerList                     ccTrChPowerList;
}
UphyFrameIndTdd128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/** Unionised info for FrameInd.
 */
typedef union UphyFrameIndUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    UphyFrameIndFdd                          fdd;
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    UphyFrameIndTdd128                       tdd128;
#endif
}
UphyFrameIndUnion;

/** Tick from L1 sent every radio frame (10ms).
 * This primitive is sent by the physical layer to the MAC to request data
 * for transmission at a specific radio frame.  It is also used by the protocol
 * stack to handle uplink reconfigurations, which are to be processed at a
 * network specified <tt>Activation time</tt>.
 *
 * One #PhyFrameInd primitive is sent every radio frame (10ms) in RRC states
 * CELL_FACH and CELL_DCH. \note #PhyFrameInd primitive is not sent during
 * initial FACH, hence an initial RACH access cannot rely on receiving
 * #PhyFrameInd primitive. During power saving #PhyFrameInd primitive is also
 * not sent. During these times the MCU uses internal 10ms signals for timing
 */
typedef struct PhyFrameIndTag
{
    /** Indicates the uplink CFN used for synchronisation of uplink
     * configurations (using activation time) in MAC (and subsequently RLC and
     * RRC). It also indicates that the physical layer is requesting data for
     * CFN \c ulCfn (to which MAC replies with a #PhyDataReq, if a new TTI has
     * started).
     */
    Int8                                    ulCfn;

    /* Activation Time Index */
    Int8                                    activationTimeIndex;

    /** umtsMode is used to determine which mode the unionised information 
     * elements refer to either FDD or TDD128.   
\assoc UNION \ref mode */
    UmtsMode                                umtsMode; 

    /** mode specific info for FrameInd.*/
    UphyFrameIndUnion                       mode;

}
PhyFrameInd;


/** PHY state change indication.
 * This primitive is sent by the physical layer to the MAC to indicate PHY
 * state changes.
 */
typedef struct PhyStateIndTag
{
    /** Indicates the new PHY state following an internal PHY state change. */
    UPhyState                               phyState;

    /** Indicates the SFN at which the state change takes into effect. */
    Int16                                   sfn;

} PhyStateInd;


# if defined(UPGRADE_3G_HSDPA)
/** Data received on HS-PDSCH indication.
 * This primitive is sent by the physical layer to the MAC to send data from
 * layer 1 on HS-DSCH. It may be sent up to every 2ms (i.e. every HS-TTI) when
 * HS-DSCH is configured. It is not necessary for this message to be sent if no
 * data is received on HS-DSCH.
 */
typedef struct PhyHsDataIndTag
{
    /** Indicates the current system frame number. */
    Int8                                    dlSfn;

    /** Indicates the 2ms TTI within the \c dlSfn in which data was received.
     * It is in the range 0 to 4.
     */
    Int8                                    subframe;

    /** Indicates the length of the buffer in \c umacHsData or referred to by
     * \c umacHsData_p.
     */
    Int32                                   bitLength;
#  if !defined(DATA_IN_SIGNAL)
    /** Indicates the pointer to the buffer allocated by MAC. If this buffer
     * was not allocated by MAC or MAC has already moved this buffer to its
     * reorder entity then MAC will assert an error. Note: This pointer is
     * aligned to 64-bit boundary.
     */
    Int8                                    *umacHsData_p;
#  else
    /** The length of the data in bytes to be passed up to MAC */
    Int16                                   byteLength;

    /** If DATA_IN_SIGNAL is defined then this parameter points to the data
     * sent by layer 1.
     */
    Int8                                    umacHsData[BITS_TO_INT8S(UPS_HS_DSCH_MAX_BITS_PER_TTI_FOR_PS)];
#  endif /* !DATA_IN_SIGNAL */
}
PhyHsDataInd;

#if defined(UPLANE_INTEGRATION_TEST)
/** #PhyHsDataDisInd is a DATA_IN_SIGNAL only version of #PhyHsDataInd.
 *  It is used for user plane integration testing when TMM memory is used
 *  and the user wants to allow phyDataInd siganls to be generated from
 *  genie test scripts (which only support DATA_IN_SIGNAL).  This signal
 *  is handled by a stubs task which converts the signal into a TMM equivalent 
 *  PhyHsDataInd signal.
 */
typedef struct PhyHsDataDisIndTag
{
    /** Indicates the current system frame number. */
    Int8                                    dlSfn;

    /** Indicates the 2ms TTI within the \c dlSfn in which data was received.
     * It is in the range 0 to 4.
     */
    Int8                                    subframe;

    /** Indicates the length of the buffer in \c umacHsData or referred to by
     * \c umacHsData_p.
     */
    Int16                                   bitLength;

    /** The length of the data in bytes to be passed up to MAC */
    Int16                                   byteLength;

    /** If DATA_IN_SIGNAL is defined then this parameter points to the data
     * sent by layer 1.
     */
    UmacHsData                              umacHsData;
}
PhyHsDataDisInd;
#endif /* UPLANE_INTEGRATION_TEST */

/** This primitive is sent by MAC to pass a memory reference to layer 1 when
 * ON_PC is defined.
 */
typedef struct PhyHsAssignPtrReqTag
{
    /** Indicates the pointer to the buffer allocated by MAC. */
    Int8 *umacHsData_p;
}
PhyHsAssignPtrReq;
# endif /* UPGRADE_3G_HSDPA */

#if defined (UPGRADE_3G_HSUPA)
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)

/** The accuracy of the PDPCCH estimate used for E-TFC selection is +/-2dB (according to TS 25.133).
 * Moreover power control granularity is 1dB.
 * Hence, this correction shall ensure the E-TFC will always get the worst case for our current implementation.
 * This signed power correction is given in 64 of dB.
 * for instance a If +2 dB correction (represented (2*64)) is used:
 * -The maximum power level at which a ETFCI is restricted would be just the nominal value.
 * -The minimum power level at which a ETFCI is restricted would be below the nominal value by 4dB.
 * If no correction (represented (0)) is used
 * -The maximum power level at which a ETFCI is restricted would be above the nominal value by 2dB.
 * -The minimum power level at which a ETFCI is restricted would be below the nominal value by 2dB.
 */
#define UPHY_DPCCH_POWER_CORRECTION_FOR_ETFC_SELECTION (SignedInt16)(2*64)

/** Define the E-HICH result(FDD). */
typedef enum UphyEhichResultFddTag
{
  /** implicitly from the serving cell. */
  UPHY_E_HICH_ACK_FDD      = 0,
  /** from a cell not in the RLS containing the serving cell. */
  UPHY_E_HICH_ACK_NS_FDD   = 1,
  /** from all cells. */
  UPHY_E_HICH_NACK_FDD     = 2 
}
UphyEhichResultFdd;

/** FDD specific info for EdchTtiInd.
 */
typedef struct UphyEdchTtiIndFddTag
{
    /** harqId (0..7 for 2ms TTI, 0..3 for 10ms TTI) indicates
     * the HARQ process identity chosen by L1 as the current HARQ process (chosen
     * by L1 according to <tt>[3G TS 25.321 section 11.8.1.4]</tt>).
     */
    Int8                                    harqId;
    /** Supends the E-DCH transmission in next TTI. 
     * When set to TRUE, MAC shall for the subsequent PhyEdchDataReq:
     * - Not add any HL data (neither from scheduled MAC-d flows nor from non-scheduled) in MAC-e PDU
     * - Prohibit SI reporting
     * - Not allow any retransmission
     * as specified in <tt>[3G TS 25.331 section 8.6.5.16]</tt>
     */
    Boolean                                 suspendTx;
    /** servingGrant (for this process) expressed as an index in table 
     * <tt>[3G TS 25.321 table 9.2.5.2.1.1]</tt>
     * indicates the maximum power allowed by the currently
     * active absolute and relative grants as maintained by L1,
     * and is the output of the Serving Grant Update function
     * <tt>[3G TS 25.321 section 11.8.1.3]</tt>.
     * Note that there is two special values that are not index the SG-table
     * servingGrant may set to "EDCH_ZERO_GRANT" and handled as defined in <tt>[3G TS 25.321] sections 11.8.1.4 and 11.8.1.6</tt>.
     * Also if the harq process indicated by harqId is deactivated by an absolute grant command,
     * servingGrant shall be set to "EDCH_HARQ_INACTIVE" and handled as defined in <tt>[3G TS 25.321]</tt>.
     */
    Int8                                    servingGrant;
    /** The UE transmission power headroom indicates the power headroom to report to the network via SI
     * as specified in <tt>[3G TS 25.133] section 9.1.13</tt>, the reporting range is from 0 ...+31 dB 
     * <tt>[3G TS 25.133] Table 9.34A</tt> defines the mapping 
     */
    Int8                                    uph;

    /** The maximum UE power defined by the UE power category.in 64 of dB
        Does not take into account any MPR nor network maximum allowed power */
    SignedInt16                             maxUePowerDbx64;

    /** The power used to transmit the DPCCH in the last slot in 64th of dB */
    SignedInt16                             dpcchPowerDbx64;

    /** Squared Amplitude ratio for DPDCH = BetaD^2/BetaC^2.
        The Beta value used are the Beta value that will be
        configured during the transmission of the next EDCH TTI.
        The compressed mode correction are not applied */
    Int16                                   squaredAmplRatioBetaDx225;

    /** Squared Amplitude ratio for HS-DPCCH = Ahs.
        This is the maximum Ahs used during the next TTI.
        The compressed mode correction are not applied */
    Int16                                   squaredAmplRatioBetaHsx225;

    /** Squared Correction factor for the next frame = NpilotC * 64 / NpilotD */
    Int8                                    squaredCmCorrectionx64;

    /** The number of transmit slot which is:
     * For a 10ms TTI: 
     *      - 15 (no gap) 
     *      - less than 15 in case (down to 8) of CM gap in current frame.
     * For a 2ms TTI: 
     *      - 15 (no gap)
     *      - less than 15 (down to 8) in case of CM gap in current frame but not in current subframe
     *      - 0 in case of CM gap in current subframe. 
     */
    Int8                                    nc;
    /** number of Active Harq processes allowed for the Scheduled Transmissions 
     * To be used by MAC for the Happy Bit Setting as defined in <tt>[3G TS 25.321 section 11.8.1.5]</tt>.
     * If 0 it means the L1 has received a command to deactivate all HARQ processes on the E-AGCH.
     */    
    Int8                                    numActiveHarqProcesses;
    /** eHichResult indicates the transmission status of an E-DCH TTI (ACK or NACK),
     * this is received on E-HICH.
     */
    UphyEhichResultFdd                      eHichResult;
} UphyEdchTtiIndFdd;

#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)

/** Indicates the grant type (TDD128).*/
typedef enum UphyGrantTypeTdd128Tag
{
   UPHY_NO_GRANT_TDD128                             = 0,
   UPHY_GRANT_SCHEDULED_TRANSMISSION_TDD128         = 1,
   UPHY_GRANT_NON_SCHEDULED_TRANSMISSION_TDD128     = 2
} UphyGrantTypeTdd128;

/** UphyGrantInfoTdd128 contains the grant info.
 */
typedef struct UphyGrantInfoTdd128Tag
{
   /** grantType indicates if there is no grant, a sched grant or a  non-sched grant. */
    UphyGrantTypeTdd128                                grantType;
   
   /** nEucch indicates the nb of E_UCCH instance in the granted E-DCH TTI.
    *  It ranges from 1 to 8. */
    Int8                                               nEucch;
    
   /** timeslotResourceRelatedInfo indicates the bitmap of allocated TS for E-DCH Tx.
    * LSB_b0 corresponds to TS5 and MSB_b4 corresponds to TS1. */
    Int8                                               timeslotResourceRelatedInfo;

   /** spreadingFactor indicates the SF to be used among allocated TS for E-DCH Tx */   
    Int8                                               spreadingFactor;

   /** powerResourceRelatedInfo (also called AbsoluteGrant(Power)Value in 3GPP spec) indicates the maximum E-PUCH
    * transmission power E-DCH traffic to reference power ratio per TDD resource unit 
    * that the UE is allowed to use on the E-DCH resources associated with the grant.
    * It ranges from -12 to 19 dB in steps of 1 dB. */
    SignedInt8                                         powerResourceRelatedInfo;

   /** isOnlyQpskModulationAllowed indicates if only QPSK is allowed for the current granted TTI.
    * This is the case when an E-PUCH coexists with another UL PhyCh (either DPCH, or E-RUCCH or HS-SICH)
    * in at least one of the timeslots within the timeslotResourceRelatedInfo. */
    Boolean                                            isOnlyQpskModulationAllowed;

   /** numberOfGrantsInExtentedEstimationWindow indicates the number of expected grants
    * (including schedued grant and non-scheduled grant) in the ExtendedEstimationWindow.
    * ExtendedEstimationWindow may be provided in #CphyEdchConfigReq when scheduled 
    * transmission is configured.
    * Default value is 0xFF when no ExtendedEstimationWindow configured.*/
    Int8                                               numberOfGrantsInExtentedEstimationWindow;

   /** isOnlySiSending indicates if the current grant TTI only contains SI.
    * This i.e. is only used in the case of non-scheduled HSUPA without UL DPCH,
    * UE sends SI-only E-PUCH when ul-synchronization of Cell_DCH. Default is False. */
    Boolean                                            isOnlySiSending;
   
} UphyGrantInfoTdd128;

/** Indicates the E-HICH result (TDD128).*/
typedef enum UphyEhichResultTdd128Tag
{
    UPHY_NO_E_HICH_TDD128     = 0,
    UPHY_E_HICH_ACK_TDD128    = 1,
    UPHY_E_HICH_NACK_TDD128   = 2
} UphyEhichResultTdd128;

/** UphyAckNackResultTdd128 contains the E-HICH indication info.
 */
typedef struct UphyAckNackResultTdd128Tag
{
    /** eHichResult indicates if no E-HICH, an ACK or a NACK has been received. */
    UphyEhichResultTdd128                    eHichResult;
   
    /** harqProcessIdentity indicates to which HARQ process the received E-HICH is linked to. */
    Int8                                     harqProcessIdentity;
   
} UphyAckNackResultTdd128;

/** TDD128 specific info for EdchTtiInd.
 */
typedef struct UphyEdchTtiIndTdd128Tag
{
    /** grantInfo contains the grant related info. */
    UphyGrantInfoTdd128                      grantInfo;

    /** Number of valid E-HICH(s) ACK/NACK result(s),
     *  also is E-HICH' existent number in this Subframe.
     * refer to the array ackNackResult[maxEHICH].
     */
    Int8                                     numOfEhichResult;

    /** ackNackResult contains info related to ACK/NACK result 
     * to indicate the transmission status of an E-DCH TTI, this is received on E-HICH.
     */
    UphyAckNackResultTdd128                  ackNackResult[maxNumE_HICH];
 
    /** Serving and Neighbour Cell Pathloss metric computed by L1
     * depending on SNPL reporting type (type1 or type2) in #CphyEdchConfigReq. 
     * It is computed according to <tt>[3G TS 25.224 section 4.13.2]</tt> 
     * and ranges from 0 to 31.
     * NOTE: if snpl is 0xFF, it indicates MAC-e assembles MAC-d PDU prior to SI even T-SI-NST expired.
     */
    Int8                                     snpl;
 
    /** Pref is the E-PUCH power used in the timeslots of the previous subframe (E-DCH TTI).
     * It is reported in steps of 1/8 dBm and it is computed as: Pe-base + L
     * (i.e: initial E-DCH power + TPCs + Pathloss).
     */
    SignedInt16                              Pref;

    /** Premaining is the remaining power which could be used to transmit E-PUCH in next subframe (E-DCH TTI).
     * It is reported in steps of 1/8 dBm and it is computed as: Pmax - Psich(if posible) - Perucch(if possible)
     * The power of DPCH(if posible) is powerRefListPerSubFrame in the msg:UphyFrameIndTdd128.
     */
    SignedInt16                              Premaining[maxTS_LCR_1];
        
} UphyEdchTtiIndTdd128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/** Unionised info for EdchTtiInd.
 */
typedef union UphyEdchTtiIndUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    UphyEdchTtiIndFdd       fdd;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    UphyEdchTtiIndTdd128    tdd128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */   
} UphyEdchTtiIndUnion;

/** This signal is sent by L1 to MAC every E-DCH TTI whenever E-DCH is
 * configured, even if there is no relative (FDD) nor absolute grant assigned 
 * (FDD/TDD128) to this UE.
 * MAC always replies to this signal with #PhyEdchDataReq, even if no data and
 * no scheduling information is passed to L1. 
 * If the current E-DCH TTI is on a TTI boundary where R99 (FDD) or R4 (TDD128) 
 * UL data should also be scheduled, L1 shall first send a #PhyFrameInd, and only
 * send #PhyEdchTtiInd after reception of the #PhyDataReq message. 
 * \note For TDD128: In actual design, #PhyhEdchTtiInd is sent to MAC at the end of TS2.
 */
typedef struct PhyEdchTtiIndTag
{
    /** ulCfn indicates the current UL CFN */
    Int8                                    ulCfn;
    
    /** For FDD: subFrame (0..4) indicates the current subframe (2ms TTI) - always
     * zero for 10ms TTI.
     * For TDD128: subFrame (0..1) indicates the current subframe (5ms TTI).
     */
    Int8                                    subFrame;

    /** umtsMode is used to determine which mode the unionised information 
     * elements refer to either FDD or TDD128.   
\assoc UNION \ref mode */
    UmtsMode                                umtsMode;

    /** mode specifc info for EdchTtiInd.*/
    UphyEdchTtiIndUnion                     mode;

    /** E-DCH encoder adress (PLP side) to write data of next #PhyEdchDataReq.*/
    Int8                                   *data_p;
}
PhyEdchTtiInd;

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** FDD specific info for EdchDataReq.
 */
typedef struct UphyEdchDataReqFddTag
{
    /** amplitudeEc is (betaEc/betaC) specified in <tt>[3G TS 25.214 section 5.1.2.5B.1]</tt>.
     * Note 1: This value is unquantized, exluding compressed factor related to pilot bits: root(Npilot,C/Npilot,N),
     *      and compressed factor related to DTX slots for a 10 ms EDCH TTI: root(15/Nslots,C)
     *      (see <tt>[3G TS 25.214 section 5.1.2.5B.1]</tt>.
     * Note 2: only numerator of linear value is given, hence real value = (amplitudeEc/15)
     */
    Int16                                   amplitudeEc;

    /** amplitudeEd,k for the first Physical Channel k is (BetaEd,k/betaC) specified in <tt>[3G TS 25.214 section 5.1.2.5B.2]</tt>.
     * This is computed by MAC using reference E-TFCI power offsets ("delta HARQ").
     * The value is used as follow:
     * Value is for the single channel among EDCH_SF_256, EDCH_SF_128, EDCH_SF_64, EDCH_SF_32, EDCH_SF_16, EDCH_SF_8, EDCH_SF_4,
     * Value for the two channels among EDCH_SF_4x2
     * Value expressed in equivalent of SF4 (e.g. not counting the sqrt(2) factor) for the two channels among EDCH_SF_2x2
     * Value expressed for sf = SF4 (e.g. not counting the sqrt(2) factor for the sf=SF2 channels), for the four channels of EDCH_SF_4x2_2x2
     * Note 1: This value is unquantized, exluding compressed factor related to pilot bits: sqrt(Npilot,C/Npilot,N),
     *      and compressed factor related to DTX slots for a 10 ms EDCH TTI: sqrt(15/Nslots,C)
     *      (see <tt>[3G TS 25.214 section 5.1.2.5B.2.4]</tt>.
     * Note 2: only numerator of linear value is given, hence real value = (amplitudeEd/15)
     */
    Int16                                   amplitudeEd;

    /** The spreadingFactorAndNoEdpdch defines the spreading factor and the
     * number of E-DPDCH physical channels required to transmit this MAC-e PDU.
     * (see SET0 in <tt>[3G TS 25.212 section 4.8.4.1]</tt>).
     */
    EdchSf                                  spreadingFactorAndNoEdpdch;
    /** The eTfci is selected by MAC for the E-DPDCH transmission and is in the
     * range 0 to 127.
     */
    Int8                                    eTfci;
    /** The happyBit indicates according to <tt>[3G TS 25.321 section 11.8.1.5]</tt>
     * whether the UE is "happy" or "unhappy".
     */
    Boolean                                 happyBit;
    /**Retransmission Sequence Number (RSN) the L1 shall use to derive the 
     * Redundancy Version (RV) of the HARQ transmission
     */
    Int8                                    rsn;
    /** Indicates if the current transmission includes a Scheduled Transmission
     *  Note that transmission of Scheduling Information only is not considered as a Scheduled Transmission
     */
    Boolean                                 scheduledTransmission;
    /**sg_LUPR is the index corresponding to the lowest power ratio in the configured SG-table that is equal or higher to the reference_ETPR
    * reference_ETPR being the E-DPDCH to DPCCH power ratio to be used as reference for the Relative Grant commands.
    * This is only valid if scheduledTransmission == TRUE.
    */
    Int8                                    sg_LUPR;

# if defined(DISABLE_UP_TO_HIGHER_THROUGHPUT)
    Int8                                    reservedForUptoHigherThrougputFeature;
# else
    /** indexStepsForUp indicates the minimum index steps to add to the sg_LUPR to reach an higher throughput.
     *  The Serving Grant Update shall take this into account when processing an "UP" Serving Relative Grant command,
     *  as follow:
     *      Serving_Grant = SG[ MIN( MAX(sg_LUPR + indexStepsForUp, sg_LUPR + indexStepsFromThresholds), 37) ]
     *  Where indexStepsFromThresholds is either 1,2,or 3, depending on the 2-index-step and 3-index-step thresholds.
     *  Range is [1..37].
     *  This is only valid if scheduledTransmission == TRUE.
     *  Note: this parameter is implementation specific
     */
    Int8                                    indexStepsForUp;
# endif /*DISABLE_UP_TO_HIGHER_THROUGHPUT*/
    /* payload excluding padding bits */
    Int16                                   payload;   
} UphyEdchDataReqFdd;

#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Indicates the moduation type for E-PUCH transmission (TDD128).*/
typedef enum UphyEdchModulationTypeTdd128Tag
{
    UPHY_E_DCH_MODULATION_QPSK_TDD128    =0,
    UPHY_E_DCH_MODULATION_16QAM_TDD128   =1
} UphyEdchModulationTypeTdd128;

/** TDD128 specific info for EdchDataReq.
 */
typedef struct UphyEdchDataReqTdd128Tag
{
    /** betaE is the gain factor for the select E-TFC transport block size, the
     * E-PUCH physical ressources, the modulation type and HARQ power offset.
     * Computed according to formula in <tt>[3G TS 25.224 section 5.1.1.6]</tt>
     *     betaE = beta0_E + alphaE + delta_Harq.
     * \note:For information: Pe-puch = Pe-base + beta_e + L, where 
     *       => Pe-puch is the Tx power on E-PUCH code
     *       => Pe-base is maintained in L1 according to desired Rx power of E-PUCH at 
     *           node-B side including TPCs received.
     *       => L is the pathloss also maintained in L1.
     *       => Beta_e is computed by MAC and provided to L1.
     */
    SignedInt8                              betaE;

    /** E-UCCH content: 10 bits
     * E-TFCI :  6 bits (MSB_b9,b8,b7,b6,b5,b4) 
     *           => eTfci selected by MAC for this E-DCH TTI, range: 0..63
     * RSN    :  2 bits (b3,b2)
     *           => to be used by L1 to perform Incremental Redundancy, range: 0..3
     * HARQ_ID:  2 bits (b1,b0_LSB)
     *           => ID of the HARQ process
     */
    Int16                                   e_UCCH;   

    /* payload bits exlude padding */
    Int16                                   payload;   

    /** modulation type to be used for E-DCH Tx : QPSK or 16QAM */
    UphyEdchModulationTypeTdd128            modulationType;

} UphyEdchDataReqTdd128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/** Unionised info for EdchDataReq.
 */
typedef union UphyEdchDataReqUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
   UphyEdchDataReqFdd      fdd;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
   UphyEdchDataReqTdd128   tdd128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */
} UphyEdchDataReqUnion;


/** This signal is always sent by MAC to L1 in response to the #PhyEdchTtiInd message. 
 * It is sent even if no data and no scheduling information is to be sent.
 */
typedef struct PhyEdchDataReqTag
{
    /** ulCfn indicates the current UL CFN */
    Int8                                    ulCfn;
    
    /** For FDD: subFrame (0..4) indicates the current subframe (2ms TTI) - always
     * zero for 10ms TTI.
     * For TDD128: subFrame (0..1) indicates the current subframe (5ms TTI).
     */
    Int8                                    subFrame;

    /** umtsMode is used to determine which mode the unionised information 
     * elements refer to either FDD or TDD128. 
\assoc UNION \ref mode */
    UmtsMode                                umtsMode;
    
    /** mode specific info for EdchDataReq.*/
    UphyEdchDataReqUnion                    mode;
    
    /** numberOfBits indicates the size in bits :
     * FYI: location of the MAC-e PDU was indicated in corresponding previous #PhyEdchTtiInd.*/
    Int16                                   numberOfBits;
}
PhyEdchDataReq;

#if defined(UPGRADE_3G_FDD) || defined(UPGRADE_3G_DOUBLE_DUAL_MODE)
/** This signal is sent by L1 to MAC when the E-DCH encoder is ready for the data transfer in FDD. 
 * It is only sent if data needs to be transfered (numberOfBits > 0) as told in previous #PhyEdchDataReq.
 */
typedef struct PhyEdchEncoderReadyIndTag
{
    /** harqId (0..7 for 2ms TTI, 0..3 for 10ms TTI) indicates
     * the HARQ process identity chosen by L1 as the current HARQ process (chosen
     * by L1 according to <tt>[3G TS 25.321 section 11.8.1.4]</tt>. */
    Int8                                    harqId;
}
PhyEdchEncoderReadyInd;

/*Functional call to MAC handling of PhyEdchEncoderReadyInd */
void FirstParsePhyEdchEncoderReadyInd (PhyEdchEncoderReadyInd * phyEdchEncoderReadyInd_p);
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */

#endif /* UPGRADE_3G_HSUPA */

#if defined(T1_UPHY_STUB_ENABLE)   
typedef enum UphyStubCaseIDTag
{
    ULPDU_1K7_CASE_ID,
    ULPDU_3K4_CASE_ID,
    ULPDU_3K4_TS2_CASE_ID,
    ULPDU_3K4_TS3_CASE_ID,
    ULPDU_3K4_TS4_CASE_ID,
    ULPDU_12K2_CASE_ID,
    ULPDU_12K2_TS5_CASE_ID,
    ULPDU_12K2_TS2_CASE_ID,
    ULPDU_RMC12K2_CASE_ID,
    ULPDU_PS64K_I_CASE_ID,
    ULPDU_13K6_CASE_ID,
    ULPDU_CS64K_CASE_ID,
    ULPDU_CS32K_CASE_ID,
    ULPDU_CS57K6_CASE_ID,
    ULPDU_CS28K8_CASE_ID,
    ULPDU_CS14K4_CASE_ID,
    ULPDU_CS10K2_CASE_ID,
    ULPDU_CS7K95_CASE_ID,
    ULPDU_CS7K4_CASE_ID,
    ULPDU_CS6K7_CASE_ID,
    ULPDU_CS5K9_CASE_ID,
    ULPDU_CS5K15_CASE_ID,
    ULPDU_CS4K75_CASE_ID,
    ULPDU_PS8K_CASE_ID,
    ULPDU_PS16K_CASE_ID,
    ULPDU_PS64K_CASE_ID,
    ULPDU_PS128K_CASE_ID,
    ULPDU_PS128K_TS23_CASE_ID,
    ULPDU_PS144K_CASE_ID, 
    ULPDU_CS12K2_PS384K_CASE_ID,
    ULPDU_CS12K2_PS64K_CASE_ID,
    ULPDU_CS64K_PS256K_CASE_ID,
    ULPDU_CS12K2_CASE_ID,
    ULPDU_RMC_CS12K2_CASE_ID,
    ULPDU_12K2_HO_CASE_ID,
    ULPDU_MAX_ULSLOTS_CASE_ID,
    ULPDU_TL1_MOC_MTC_01_DCH05_CASE_ID,        
    ULPDU_TL1_MOC_MTC_01_DCH06_CASE_ID,        
    ULPDU_TL1_MOC_MTC_01_DCH07_CASE_ID,        
    ULPDU_CS32K_DTX_CASE_ID,    //Modify by SYO for DL DTX test. Add one new test case because of time slot position limitation from arbiter
    ULPDU_DPA_PS32K_CASE_ID,    //Added by SYO for uplink PS32k in HSDPA integration test
    ULPDU_DPA_PS32K_TS2_CASE_ID,  
    ULPDU_DPA_PS32K_TS3_CASE_ID,
    ULPDU_DPA_PHY_PFM_CASE_ID,
#if defined (UPGRADE_3G_HSUPA) && defined (T1_UPHY_R7_STUB_ENABLE)
    ULPDU_UPA_NO_UL_DPCH,
    ULPDU_UPA_2_CODE_DPCH,
    ULPDU_UPA_EDCH_TB_A,
    ULPDU_UPA_EDCH_TB_B,
    ULPDU_UPA_EDCH_TB_C,
    ULPDU_UPA_EDCH_TB_D,
    ULPDU_UPA_EDCH_TB_E,
    ULPDU_UPA_EDCH_TB_F,
    ULPDU_UPA_EDCH_TB_G,
    ULPDU_UPA_EDCH_TB_H,
    ULPDU_UPA_EDCH_TB_I,
    ULPDU_UPA_EDCH_TB_J,
    ULPDU_UPA_EDCH_TB_K,
    ULPDU_UPA_EDCH_TB_L,
    ULPDU_UPA_EDCH_TB_WITH_ULDPCH_SCHEDULED_01, 
    ULPDU_UPA_EDCH_TB_WITH_ULDPCH_SCHEDULED_02, 
    ULPDU_UPA_EDCH_TB_WITH_ULDPCH_SCHEDULED_03, 
    ULPDU_UPA_EDCH_TB_WITHOUT_ULDPCH_SCHEDULED_01,
    ULPDU_UPA_EDCH_TB_WITHOUT_ULDPCH_NONSCHEDULE_AND_SCHEDULE_01,
    ULPDU_UPA_EDCH_TB_WITHOUT_ULDPCH_NONSCHEDULE_AND_SCHEDULE_02,
    ULPDU_UPA_EDCH_TB_WITHOUT_ULDPCH_NONSCHEDULE_AND_SCHEDULE_03,
    ULPDU_UPA_EDCH_TB_WITHOUT_ULDPCH_NONSCHEDULE_AND_SCHEDULE_04,


#endif /* UPGRADE_3G_HSUPA && T1_UPHY_R7_STUB_ENABLE */
    ULPDU_MAX_CASE_NUMBER        
}UphyStubCaseID;


typedef enum UphyStubTestTxModeTag
{
    UphyStubTestTxModeSpecialBurst,
    UphyStubTestTxModeFullDataRage
}UphyStubTestTxMode;
typedef enum UphyStubTestTxDataPtnTag
{
    UphyStubTestTxDataAllZero,
    UphyStubTestTxDataAllOne,
    UphyStubTestTxDataIntlv01,
    UphyStubTestTxDataIntlv10,
    UphyStubTestTxDataVector
}UphyStubTestTxDataPtn;

typedef struct UphyStubTestReqTag
{
    /** specify which test case is carry on */
    UphyStubCaseID caseId;
    /**specify how data is tansmitted
     * 0: tranmit special burst
     * 1: transmit full data rate*/
    UphyStubTestTxMode txMode;

    /** defines the mode of data generation
     * 0: all zero data,
     * 1: all 1 data,
     * 2: 010101...data,
     * 3, 101010...data,
     */
    UphyStubTestTxDataPtn dataPattern;
}
UphyStubTestReq;

typedef Int8 UphyStubTestCnf;

#endif /*T1_UPHY_STUB_ENABLE*/


/** @} */ /* End of SigUphy group */

#endif

/* END OF FILE */
