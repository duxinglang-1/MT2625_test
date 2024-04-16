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
 *      types common to mmr_sig.h (NAS) and rrc_sig.h (AS)
 **************************************************************************/

#ifndef RRCMRTYP_H
#define RRCMRTYP_H

/***************************************************************************
*   Manifest Constants
***************************************************************************/

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/
#include <uas_asn.h>

/***************************************************************************
*   Types
***************************************************************************/

/** Defines UMTS mobile equipment capability and FDD/TDD bands. */
typedef struct UmtsMobileEquipmentDataTag
{
    /** Defines UE radio access capability */
    UUE_RadioAccessCapability                   ue_RadioAccessCapability;
    /** UE radio access capability extension.*/
    UUECapabilityInformation_v370NonCriticalExt v370NonCriticalExt;

#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_FDD_MULTIBAND)
    /** Defines FDD band supported. */
    FddBandMode                                 fddBandsSupported;
    /** Defines FDD band configured. */
    FddBandMode                                 fddBandsConfigured;
#endif /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)
    /** Defines TDD128 band supported. */
    Tdd128BandMode                              tdd128BandsSupported;
    /** Defines TDD128 band configured. */
    Tdd128BandMode                              tdd128BandsConfigured;
#endif /* UPGRADE_3G_TDD128 */ 
}
UmtsMobileEquipmentData;

#if defined(UPGRADE_NVRAM_OPTIMIZE) && defined(UPGRADE_NVRAM_ENHANCED)

typedef struct UPDCP_CapabilityVarTag
{
    Boolean                                            losslessSRNS_RelocationSupport;
    T_UPDCP_Capability_supportForRfc2507               tag;
}
UPDCP_CapabilityVar;

typedef struct URF_CapabilityVarTag
{
    Boolean                                            fddRF_CapabilityPresent;
    UUE_PowerClass                                     ue_PowerClass;
}
URF_CapabilityVar;

typedef struct UUE_MultiModeRAT_CapabilityVarTag
{
    UMultiRAT_Capability                               multiRAT_CapabilityList;
}
UUE_MultiModeRAT_CapabilityVar;

typedef struct USecurityCapabilityVarTag
{
    Int16  /* 0 to 65535 */                            cipheringAlgorithmCap;
    Int16  /* 0 to 65535 */                            integrityProtectionAlgorithmCap;
}
USecurityCapabilityVar;

typedef struct UUE_Positioning_CapabilityVarTag
{
    Boolean                                            standaloneLocMethodsSupported;
    Boolean                                            ue_BasedOTDOA_Supported;
    UNetworkAssistedGPS_Supported                      networkAssistedGPS_Supported;
    Boolean                                            supportForUE_GPS_TimingOfCellFrames;
    Boolean                                            supportForIPDL;
}
UUE_Positioning_CapabilityVar;

typedef struct UUE_RadioAccessCapabBandFDDVarTag
{
    URadioFrequencyBandFDD                             radioFrequencyBandFDD;
    UUE_PowerClassExt                                  ue_PowerClass;
}
UUE_RadioAccessCapabBandFDDVar;

typedef struct UUE_RadioAccessCapabBandFDDListVarTag
{
    Int16 n; /* 1 to maxFreqBandsFDD */
    UUE_RadioAccessCapabBandFDDVar                        data [maxFreqBandsFDD];
}
UUE_RadioAccessCapabBandFDDListVar;

typedef struct UUE_PositioningCapabilityExt_v380VarTag
{
    Boolean                                            rx_tx_TimeDifferenceType2Capable;
}
UUE_PositioningCapabilityExt_v380Var;

typedef struct UUECapabilityInformation_v3a0ext_IEsVarTag
{
    Boolean                                            ue_RadioAccessCapability_v3a0extPresent;
    UUE_PositioningCapabilityExt_v3a0_valid_CellPCH_UraPCH validity_CellPCH_UraPCH;
}
UUECapabilityInformation_v3a0ext_IEsVar;

typedef struct UUECapabilityInformation_v680ext_IEsVarTag
{
    Boolean                                            supportOfHandoverToGANPresent;
    UMultiModeRAT_Capability_v680ext_supportOfHandoverToGAN supportOfHandoverToGAN;
}
UUECapabilityInformation_v680ext_IEsVar;

typedef struct UUE_RadioAccessCapability_v4b0extVarTag
{
    UPDCP_Capability_r4_ext                            pdcp_Capability_r4_ext;
    UUE_PowerClass                                     ue_PowerClass;
    URadioFrequencyBandTDDList                         radioFrequencyBandTDDList;
    Boolean                                            dlSupportOf8PSK;
    Boolean                                            ulSupportOf8PSK;

    UAccessStratumReleaseIndicator                     accessStratumReleaseIndicator;
}
UUE_RadioAccessCapability_v4b0extVar;

typedef struct UUE_RadioAccessCapability_v3g0extVarTag
{
    UUE_PositioningCapabilityExt_v3g0_sfn_sfnType2Cap  sfn_sfnType2Capability;
}
UUE_RadioAccessCapability_v3g0extVar;

typedef struct UUE_RadioAccessCapability_v590extVarTag
{
    Boolean                                            supportForRfc3095ContextRelocation;

    UHSDSCH_physical_layer_category                    fdd_hsdsch_physical_layer_category;

    T_UPhysicalChannelCapability_hspdsch_r5_tdd128_hspdsch tdd128_tag;
    UHSDSCH_physical_layer_category                    tdd128_hspdsch;

    Boolean                                            supportOfUTRAN_ToGERAN_NACC;    
}
UUE_RadioAccessCapability_v590extVar;

typedef struct UUECapabilityInformation_v5c0extVarTag
{
    UPDCP_Capability_r5_ext2_losslessDLRLC_PDUSizeChange losslessDLRLC_PDUSizeChange;
}
UUECapabilityInformation_v5c0extVar;

typedef struct UUE_RadioAccessCapability_v690extVarTag
{
    UPhysicalChannelCapability_edch_r6                 physicalchannelcapability_edch;
}
UUE_RadioAccessCapability_v690extVar;

typedef struct UInterRAT_UE_RadioAccessCapability_v690extVarTag
{
    Boolean                                            supportOfInter_RAT_PS_HandoverPresent;
    UInterRAT_UE_RadioAccessCapability_v690ext_I_RAT_PS_HO supportOfInter_RAT_PS_Handover;
}
UInterRAT_UE_RadioAccessCapability_v690extVar;

typedef struct UUE_RadioAccessCapability_v770ext_IEsVarTag
{
    Boolean                                            supportOfTwoLogicalChannel;

    Boolean                                            fddPhysChCapabilityPresent;
    UPhysicalChannelCapability_v770ext_fddPhysChCapability fddPhysChCapability;

    Boolean                                            tddPhysChCapability_128Present;
    UPhysicalChannelCapability_v770ext_tddPhysChCap_128 tddPhysChCapability_128;

    UMultiModeRAT_Capability_v770ext                   multiModeRAT_Capability;
    
    Boolean                                            mac_ehsSupportPresent;
    UUE_RadioAccessCapability_v770ext_IEs_mac_ehsSupport mac_ehsSupport;
}
UUE_RadioAccessCapability_v770ext_IEsVar;

typedef struct UUE_RadioAccessCapability_v860ext_IEsVarTag
{
    Boolean                                            tdd128RF_CapabilityPresent;
    URadioFrequencyBandTDDextList                      tdd128RF_Capability;
}
UUE_RadioAccessCapability_v860ext_IEsVar;

typedef struct UUE_CapabilityContainer_IEs_v860NonCriticalExtensionsVarTag
{
    UUE_RadioAccessCapability_v860ext_IEsVar              ue_RadioAccessCapability_v860ext;
    Boolean                                            ue_RATSpecificCapability_v860extPresent;
    UInterRAT_UE_RadioAccessCapability_v860ext         ue_RATSpecificCapability_v860ext;
}
UUE_CapabilityContainer_IEs_v860NonCriticalExtensionsVar;

typedef struct UUE_CapabilityContainer_IEsVarTag
{
    UUE_RadioAccessCapability_v690extVar                  ue_RadioAccessCapability_v690ext;

    UInterRAT_UE_RadioAccessCapability_v690extVar         ue_RATSpecificCapability_v690ext;

    UUE_RadioAccessCapability_v6b0ext_IEs               ue_RadioAccessCapability_v6b0ext;

    UUE_RadioAccessCapability_v6e0ext_IEs              ue_RadioAccessCapability_v6e0ext;

    UUE_RadioAccessCapability_v770ext_IEsVar            ue_RadioAccessCapability_v770ext;

    UUE_RadioAccessCapability_v790ext_IEs              ue_RadioAccessCapability_v790ext;

    UUE_CapabilityContainer_IEs_v860NonCriticalExtensionsVar    v860NonCriticalExtensions;
    
}
UUE_CapabilityContainer_IEsVar;

typedef struct UUECapabilityInformation_v690ext_IEsVarTag
{
    Boolean                                            ueCapabilityContainerPresent;
    UUE_CapabilityContainer_IEsVar                        ueCapabilityContainer;
}
UUECapabilityInformation_v690ext_IEsVar;

typedef struct UmtsMobileEquipmentDataVarTag
{
    UPDCP_CapabilityVar                         pdcp_Capability;
    URF_CapabilityVar                           rf_Capability;
    UUE_MultiModeRAT_CapabilityVar              ue_MultiModeRAT_Capability;
    USecurityCapabilityVar                      securityCapability;
    UUE_Positioning_CapabilityVar               ue_positioning_Capability;

    UUE_RadioAccessCapabBandFDDListVar          ue_RadioAccessCapabBandFDDList_v370;
    UUE_PositioningCapabilityExt_v380Var        ue_PositioningCapabilityExt_v380;
    UUECapabilityInformation_v3a0ext_IEsVar     ue_RadioAccessCapability_v3a0ext;
    UUECapabilityInformation_v680ext_IEsVar     ueCapabilityInformation_v680ext;
    UUE_RadioAccessCapability_v4b0extVar        ueCapabilityInformation_v4b0ext;
    UUE_RadioAccessCapability_v3g0extVar        ue_RadioAccessCapability_v3g0ext;
    UUE_RadioAccessCapability_v590extVar        ue_RadioAccessCapability_v590ext;
    UUECapabilityInformation_v5c0extVar         ueCapabilityInformation_v5c0ext;

    Boolean                                     ueCapabilityContainerPresent;
    UUE_CapabilityContainer_IEsVar              ueCapabilityContainer;
}
UmtsMobileEquipmentDataVar;
#endif

#endif  /* RRCMRTYP_H */

/* END OF FILE */
