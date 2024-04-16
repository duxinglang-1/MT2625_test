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
 * 3G Protocol Stack Configuration Definitions
 *************************************************************************/

#if !defined (UPS_CFG_H)
#define       UPS_CFG_H

#if defined (UPGRADE_3G)

/***************************************************************************
* Nested Include Files
***************************************************************************/

#include <uas_asn.h>
#include <system.h>
#include <ubnd_cfg.h>
#include <ut_mcc_mnc.h>

/***************************************************************************
 * External Global Variables
 ***************************************************************************/

/** Defines the applicable threshold, which is used during RSSI scan.
 * During RSSI scan on a frequency, the frequency won't be considered if the
 * reported RSSI value is lower than the applicable threshold. Threshold
 * applicability is described below:
 *     -# Due to internal scaling, the value "X" of the thresholds below must
 *        be such that <tt> -4095 dBm <= X <= +4095 dBm. </tt>
 *     -# Setting these thresholds is at the discretion of individual
 *        manufacturers. In doing so, they accept sole responsibility.
 */
extern const SignedInt16 upsCfgRssiScanThreshold;

/** If a frequency is not detected it will only be scanned again
 * if its RSSI reading has increased by upsCfgRssiScanHysteresis (in 1/8dB).
 */
extern const SignedInt16 upsCfgRssiScanHysteresis;

/** If an RSSI scan is for a frequency range the Rssi scan threshold is
 * calculated dynamically from the lowest RSSI reading found (i.e. noise
 * floor plus upsCfgNoiseFloorThreshold (in 1/8dB).
 */
extern const SignedInt16 upsCfgNoiseFloorThreshold;

/** The following variable configures the Ec/No value that is deemed to give
 * enough confidence that the S criterion is satisfied for cells for which the
 * measured Ec/No is such that:
 *         measured Ec/No >= upsCfgSCriterionValidationLevel
 * during 3G PLMN searches.
 * This threshold is used in order to minimise the sib3 reading activity during
 * 3G PLMN searches in RRC IDLE. If the Ec/No for a cell satisfies the above
 * inequality, then its SIB3 is not read, beause it is very likely that this
 * cell satisfies S without having to read the relevant SIB3 parameters. This
 * speeds up the searches whilst giving confidence that the reported cells
 * still satisfy S.
 * \note Setting this threshold is at the discretion of individual
 * manufacturers. In doing so, they accept sole responsibility.
 */
extern const SignedInt16 upsCfgSCriterionValidationLevel;

/** The minimum of the dynamic receiver range in dBm x 8 units for
 * Event 6E.
 */
extern const SignedInt16 upsCfgGetMinDynamicReceiverRange;

/** The maximum of the dynamic receiver range in dBm x 8 units for
 * Event 6E.
 */
extern const SignedInt16 upsCfgGetMaxDynamicReceiverRange;

extern const SignedInt16 upsCfgRssiFullScanThreshold;

/* If there is a malfunction in the lower layers, and it becomes
 * impossible to send data then the RRC connection needs to be abandoned.
 * The best way of detecting this is if there are a large number of AM data
 * not acknowledged by the UTRAN. */
extern const SignedInt16 upsCfgMaxNumberOfAmDataReqQueuedInRlc;

/* The following structure and table is used to associate a given country
 * with a given FDD BAND combination (FddRegion). Overrides default band
 * combinations. */
typedef struct FddRegionExemptionTableTag
{
    FddRegion fddRegion;
    Mcc       mcc;
}
FddRegionExemptionTable;

#ifdef UPGRADE_3G_TDD128

#define TDD_REGION_MASK_NULL     (0x0)
#define TDD_REGION_MASK_ALL      (0xffff)

#endif /* UPGRADE_3G_TDD128 */

#define FDD_REGION_MASK_NULL     (0x0)
#define FDD_REGION_MASK_ALL      (0xffff)

extern const FddRegionExemptionTable upsCfgFddRegionExemptionTable [];

/***************************************************************************
 * Global Function Prototypes
 ***************************************************************************/

/* Function prototypes to allow a 3G platform to know when data received
 * by the PS in PhyDataInd has completed processing */

/** @} */ /* End of Cfg3gPs group */

/** \defgroup ApiPdu PDU Management API
 * \ingroup Cfg3gPs

 * \ingroup Api
 * This section defines API to allow a 3G platform to know if the data received
 * in #PhyDataInd primitive has been completely processed or not.
 *
 * @{
 */

/** Defines how large the data area should be
 * allocated for the UL PDU list. \note TMM memory should always be used
 * for these allocations for flow control to work correctly.
 */
Int8 *UpsCfgAllocUlPduList (void);

/** Defines a deallocation function for the
 * above allocation function. \note TMM memory should always be used for
 * these allocations for flow control to work correctly.
 */
void UpsCfgFreeUlPduList (Int8 * oldPduList_p);

/* add variable 'clearLastResult' to indicate when power off occurred  
*clear some relative variable values.
*/ 
Boolean UpsCfgCheckRssiToBeSent (Int16 signalLevel,
                                 Int16 signalQuality,
                                 Boolean clearLastResult);

/* Check flag 'isConsumed' which indicates if the DL PDU list 
* is no longer in use.
*/ 
void UpsCfgDlPduListIsConsumed (UDlPduList * pduList_p);

#if defined(UPGRADE_3G_HSDPA)

/** Defines how large the data area should be
 * allocated for the MAC-hs PDU. \note TMM memory should always be used
 * for these allocations for flow control to work correctly.
 */
Int8 *UpsCfgAllocHsPdu (Int32 size);

/** Defines a deallocation function for the
 * above allocation function. \note TMM memory should always be used for
 * these allocations for flow control to work correctly.
 */
void UpsCfgFreeHsPdu (Int8 * oldHsPdu_p);

/** Defines a partial deallocation function for
 * the above allocation function. \note TMM memory should always be used
 * for these allocations for flow control to work correctly.
 */
void UpsCfgFreeTailHsPdu (Int32 newLength, Int8 * hsPdu_p);

/** Defines a function to send PhyHsAssignPointerReq to the 
 * lower layer.
 */
void UpsCfgSendPhyHsAssignPointerReq(Int8 *umacHsData_p);
#endif /* UPGRADE_3G_HSDPA */

/** @} */ /* End of ApiPdu group */

#endif /* UPGRADE_3G */
#endif /* UPS_CFG_H */
/* END OF FILE */
