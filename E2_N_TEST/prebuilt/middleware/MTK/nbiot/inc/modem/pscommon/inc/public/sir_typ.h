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
 *      types common to mmxx_sig.h rr_sig.h mmr_sig.h and
 *                      mmsi_sig.h and alsi_sig.h
 **************************************************************************/

#ifndef SIR_TYP_H
#define SIR_TYP_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#include <system.h>

# if !defined (L13_TYP_H)
#include     <l13_typ.h>
# endif /* !(L13_TYP_H) */

# if !defined (UT_MCC_MNC_H)
#include <ut_mcc_mnc.h>
# endif /* !(UT_MCC_MNC_H) */

# if !defined (MPSI_TYP_H)
#include     <mpsi_typ.h>
# endif /* !(MPSI_TYP_H) */

/****************************************************************************
 * Macros
 ****************************************************************************/
#if defined(UPGRADE_TWGGE_NRW)
#define MAX_EQUIVALENT_PLMNS  16
#else
# if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
/* CR-816 ePLMN extension
** Rec 24.008 rel6 9.2.13 Location updating accept
**
** ((MAX_EQUIVALENT_PLMNS_IE_SIZE/3)+1)
*/
#define MAX_EQUIVALENT_PLMNS  16  /* max transmitted + 1 for current PLMN */
# else
#define MAX_EQUIVALENT_PLMNS  6   /* max transmitted + 1 for current PLMN */
# endif
#endif
#define RAW_LAI_IE_SIZE       5

#if defined(UPGRADE_TWGGE_NRW)
#define MAX_NUM_EHPLMN_LIST_IN_NVRAM (3)
#else
#if defined(ENABLE_EHPLMN) && defined(ENABLE_CMCC_FEATURE)
#define MAX_NUM_EHPLMN_LIST_IN_NVRAM (3)
#endif
#endif

#if defined(ENABLE_EHPLMN)
#define MAX_EHPLMNS           MAX_EQUIVALENT_PLMNS   /* Size of the HPLMN array which is forwarded to lower layer.
                                                      * Not specified in Rec */
#endif /* (ENABLE_EHPLMN) */

#define BA_LIST_SIZE                16

#define BA_ARRAY_SIZE               128
#define SI_2_BA_ARRAY_SIZE          BA_LIST_SIZE  /* FR9710-1539: set to 16 = (124 ARFCNs/8) +1) */

/*Access technology field.
 * For coding please refer to TS 131 102,
 * this is defined under the section describing EF PLMNwACT */
#define MM_GSM_ACT_OFFSET               (07)
#define MM_GSM_COMPACT_ACT_OFFSET       (06)
#define MM_UTRAN_ACT_OFFSET             (15)
#define GSM_ACCESS_TECHNOLOGY          (0x0001 << MM_GSM_ACT_OFFSET)         /* = 0x0080 */
#define GSM_COMPACT_ACCESS_TECHNOLOGY  (0x0001 << MM_GSM_COMPACT_ACT_OFFSET) /* = 0x0040 */
#define UTRAN_ACCESS_TECHNOLOGY        (0x0001 << MM_UTRAN_ACT_OFFSET)       /* = 0x8000 */

/* ABMM internal: ABMM needs to know the UMTS mode */
#if defined (UPGRADE_3G_HSDPA)
#define UTRAN_ACCESS_TECHNOLOGY_HSDPA_BIT 0x0400
#endif
#if defined (UPGRADE_3G_HSUPA)
#define UTRAN_ACCESS_TECHNOLOGY_HSUPA_BIT 0x0800
#endif

#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_MBMS)
#  if defined (UPGRADE_3G_TDD128)
#define UTRAN_ACCESS_TECHNOLOGY_MBMS_BIT  0x1000
  #  endif /*UPGRADE_3G_TDD128*/
# endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G */

#  if defined (UPGRADE_3G_FDD)
#define UTRAN_ACCESS_TECHNOLOGY_FDD_BIT  0x0100
#define UTRAN_ACCESS_TECHNOLOGY_FDD      (UTRAN_ACCESS_TECHNOLOGY | UTRAN_ACCESS_TECHNOLOGY_FDD_BIT)
#  endif /* (UPGRADE_3G_FDD) */
#  if defined (UPGRADE_3G_TDD128)
#define UTRAN_ACCESS_TECHNOLOGY_TDD128_BIT  0x0200
#define UTRAN_ACCESS_TECHNOLOGY_TDD128   (UTRAN_ACCESS_TECHNOLOGY | UTRAN_ACCESS_TECHNOLOGY_TDD128_BIT)
#  endif /* (UPGRADE_3G_TDD128) */

#if defined (UPGRADE_LTE)
#define EUTRAN_ACCESS_TECHNOLOGY_BIT      0x4000
#define EUTRAN_TDD_ACCESS_TECHNOLOGY_BIT  0x0001
#define EUTRAN_FDD_ACCESS_TECHNOLOGY_BIT  0x0002
#define MM_EUTRAN_ACT_OFFSET              (14)
#define EUTRAN_ACCESS_TECHNOLOGY          (0x0001 << MM_EUTRAN_ACT_OFFSET)      /* = 0x4000 */
#define EUTRAN_ACCESS_TECHNOLOGY_TDD      (EUTRAN_ACCESS_TECHNOLOGY_BIT | EUTRAN_TDD_ACCESS_TECHNOLOGY_BIT)
#define EUTRAN_ACCESS_TECHNOLOGY_FDD      (EUTRAN_ACCESS_TECHNOLOGY_BIT | EUTRAN_FDD_ACCESS_TECHNOLOGY_BIT)

/** Length of raw GUTI format to exchange the data with SIM.
 * Excludes the IEI field but includes the length field.
 * 24.301 Figure 9.9.3.12.1
 */
#define RAW_GUTI_IE_SIZE                  12

/** Offset of the start of the MCC/MNC field (octet 4 in 24.301 Figure 9.9.3.12.1) in raw GUTI
 * (raw GUTI starts from octet 2 in Figure 9.9.3.12.1, i.e. from the length field)
 */
#define MNC_MCC_GUTI_OFFSET               2

#endif


/****************************************************************************
 * Types
 ****************************************************************************/

#define     RAND_SIZE               16
#define     AUT_SIZE                      16
#define     RES_SIZE                      16

#define ENCODED_MEAS_RESULTS_IE_SIZE 16

/****************************************************************************
 * Types
 ****************************************************************************/

/** Expected user response (SRES) element */
typedef struct SresTag
{
    /** SRES value used to determine if the authentication is successful or not */
    Int32                           value;
}
Sres;

/** Authentication parameter RAND */
typedef struct RandTag
{
    /** RAND value
    * A number selected from a known set of numbers in such a way that each
    * number in the set has the same probability of occurrence. This is typically
    * used in the authentication and ciphering process.
    */
    /** The length of RAND value
\assoc ARRAY \ref value */
    Int8                            length;
    /** RAND data */
    Int8                            value[RAND_SIZE];
}
Rand;

/** UMTS authentication challenge.
 * Provides the MS with a means of authenticating the network.
 * For coding see TS 31.103, section 6.3.2 and section 6.3.7
 */
typedef struct AutTag
{
    /** AUTN value
    * The Authentication Token is sent in the Authentication Request message to the
    * mobile along with a random number and KSI (Key Set Identifier). The variables
    * within the AUTN are used by the mobile in the authentication process that it
    * conducts with the network.
    */
    /** Length of AUTN contents
\assoc ARRAY \ref data */
    Int8 length;
    /** AUTN data */
    Int8 data[AUT_SIZE];
}
Aut;

/** Authentication Response parameter element.
 * to provide the network with the authentication response
 * calculated in the SIM/USIM.
 */
typedef struct ResTag
{
  /** The length of authentication response data
\assoc ARRAY \ref data */
  Int8 length;
  /** Authentication response data */
  Int8 data[RES_SIZE];
}
Res;

/** Bit string 32 */
typedef Int32
Tmsi;

/** Access class of type Int16 */
typedef Int16
AccessClass;

/** Bit string(16)*/
typedef Int16
Lac;

/** Coded as an 8 bit number contained in the first octet */
typedef Int8
Rac;

/** CellId of type Int16 */
typedef Int16
CellId;

typedef Int8
TmsiTime;

/** Defines the MS operation mode and it forms part of the SIM
 * administration data
 */
typedef enum MsOperationModeTag
{
    MS_OPERN_NORMAL             =   0x00, /**< Normal operation */
    MS_OPERN_TA                 =   0x80, /**< Type approval operation */
    MS_OPERN_NORMAL_PLUS_SPECIFIC = 0x01, /**< Normal + specific operation */
    MS_OPERN_TA_PLUS_SPECIFIC   =   0x81, /**< Type approval + specific operation */
    MS_OPERN_MAINTENANCE        =   0x02, /**< Maintenance operation */
    MS_OPERN_CELL_TEST          =   0x04  /**< Cell test operation */
}
MsOperationMode;

/** Key sequence element.
 * The ciphering key sequence number is a type 1 information element. See
 * (GPP TS 24.008) */
typedef enum KeySequenceTag
{
    /** Key sequence 0 */
    KEY_SEQUENCE_0                  = 0,
    /** Key sequence 1 */
    KEY_SEQUENCE_1                  = 1,
    /** Key sequence 2 */
    KEY_SEQUENCE_2                  = 2,
    /** Key sequence 3 */
    KEY_SEQUENCE_3                  = 3,
    /** Key sequence 4 */
    KEY_SEQUENCE_4                  = 4,
    /** Key sequence 5 */
    KEY_SEQUENCE_5                  = 5,
    /** Key sequence 6 */
    KEY_SEQUENCE_6                  = 6,
    /** Key sequence 7 */
    NO_KEY_SEQUENCE_AVAILABLE       = 7
}
KeySequence;

/** Ciphering key data element */
typedef struct CipheringKeyDataTag
{
    /** Is the sequence number; this corresponds to the data
     * stored in EF(KC) or EF(KC GPRS)
     */
    KeySequence                     cksn;
    /** Ciphering key */
    CipheringKey                    kc;
}
CipheringKeyData;

/** Location Area ID element.
 * The purpose of the Location Area Identification information element
 * is to provide an unambiguous identification of location areas within
 * the area covered by the 3GPP system.
 */
typedef struct LocationAreaIdTag
{
    /** Contains the Mobile County Code of the location area which represents the PLMN */
    Mcc                             mcc;
    /** Contains the Mobile Network Code of the location area  */
    Mnc                             mnc;
    /** Contains the Location Area Code and this uniquely identifies a geographic
     * location area within a PLMN
     */
    Lac                             lac;
    /** Raw IE */
    Int8                            rawIe[RAW_LAI_IE_SIZE];
}
LocationAreaIdElement,
#if defined (UPGRADE_LTE)
/** Location Area ID.
 * When used in LTE mode operation this Lai type may be used to identfy and map onto
 * a Tracking Area ID  - in which case Lac represents the Tracking Area Code - which is also held as an
 * Int16 value. This is not uniquely a Tracking Area ID in LTE though as when the UE is combined EPS
 * and IMSI attached a Location area ID is identifed for the IMSI attach in addition to Tracking Area ID.
 */
#else /* UPGRADE_LTE */
/** Location Area ID */
#endif /* UPGRADE_LTE */
Lai;

/** Routing area identification (RAI)element.
 * The purpose of the routing area identification (RAI) information
 * element is to provide an unambiguous identification of routing
 * areas within the GPRS coverage area.
 */
typedef struct RoutingAreaIdTag
{
    /** The Mobile Country Code is a three digit number uniquely identifying a given country */
    Mcc                             mcc;
    /** The Mobile Network Code is either a two or three digit number used to
     * uniquely identify a given network from within a specified country
     * (MCC (Mobile Country Code)) */
    Mnc                             mnc;
    /** The Location Area Code uniquely identifies a LA (Location Area) within a PLMN  */
    Lac                             lac;
    /** Routing Area Code is a fixed length of 1 octet and identifies a routing area within a
     * location area; the RAC is part of the RAI (Routing Area Identity)
     */
    Rac                             rac;
    /** Raw IE */
    Int8                            rawIe[RAW_LAI_IE_SIZE];
}
RoutingAreaIdElement,
/** The purpose of the routing area identification (RAI) information
 * element is to provide an unambiguous identification of routing
 * areas within the GPRS coverage area.
 */
Rai;

/** Equivalent PLMN element */
typedef struct EquivalentPlmnTag
{
    /** MCC for the PLMN;
     * The Mobile Country Code is a three digit number uniquely identifying a given country */
    Mcc     mcc;
    /** MNC for the PLMN;
     * The Mobile Network Code is either a two or three digit number used to
     * uniquely identify a given network from within a specified country
     * (MCC (Mobile Country Code))
     */
    Mnc     mnc;
#if defined (UPGRADE_NASMDL2)
    /** identifies if the mnc is two or three digit as established from the decode of the list */
    Boolean threeDigitMnc;
#endif /* UPGRADE_NASMDL2 */
}
EquivalentPlmn;

/** Defines equivalent PLMN list element.
 * The Equivalent PLMNs information element is included if
 * the network wants to inform the mobile station of equivalent
 * PLMNs.
 */
typedef struct EquivalentPlmnListTag
{
    /** Indicates number of PLMN in the list
\assoc ARRAY \ref plmn */
    Int8                            numPlmn;
    /** Lists equivalent PLMN */
    EquivalentPlmn                  plmn[MAX_EQUIVALENT_PLMNS];
}
EquivalentPlmnListElement,
/** Defines equivalent PLMN list.
 * The Equivalent PLMNs information element is included if
 * the network wants to inform the mobile station of equivalent
 * PLMNs.
 */
EquivalentPlmnList;

/** BCCH allocation list (BA list).
 * BCCH channels carry information detailing the frequencies on which neighbour
 * cell BCCH channels are broadcast (belonging to the same PLMN). This data is
 * called the BCCH allocation list (shortened to BA list). This list can be used
 * by an MS to speed up the camping procedure by initially attempting to synchronise
 * to those frequencies indicated in the list.
 *
 * For the RPLMN, the MS stores the BA list in the BCCH file on the SIM. Every time
 * the application software receives an MmrRegCnf signal or an MmrRegInd signal, it
 * should write the BaList contained within these signals to the BCCH file on the SIM.
 *
 * The SIM only has storage for the RPLMN BA list. Other BA lists may be stored locally
 * within the MS. This may speed up the initial camping procedure when no SIM is present.
 */
typedef struct ChannelBitMapTag
{
    /** If the parameter valid is set false, the channel data should not be used
\assoc PRESENT \ref channelData */
    Boolean                         valid;
    /** Is a bit array where each bit represents the presence (or not) of a particular channel */
    Int8                            channelData[BA_ARRAY_SIZE];
}
BaList;

/* FR9710-1539 : */
/** Used to transparently pass the BA list recieved in SI2 up the PS */
typedef struct Si2BaListTag
{
    /** Whether or not the data is valid
\assoc PRESENT \ref channelData */
    Boolean                         valid;
    /** BCCH Allocation list.
     * This is a list of frequencies supported on the neighbouring cells.
     * This parameter is broadcast on the BCCH (Broadcast Control Channel)
     * giving mobiles the frequencies of the BCCH carriers on neighbouring cells.
     * It is typically used by the MS (Mobile Station) in the cell selection and re-selection processes.
     */
    Int8                            channelData[SI_2_BA_ARRAY_SIZE];
}
Si2BaList;

/** Local information information element */
typedef struct LocalLocationInfoTag
{
    /** The Location Area Identity uniquely identifies a LA (Location Area)
     * within any PLMN (Public Land Mobile Network); it is comprised of the
     * MCC (Mobile Country Code), MNC (Mobile Network Code) and the LAC
     * (Location Area Code)
     */
    Lai         lai;
    /** Identifies a cell within a location area */
    Int32       cellId;
    /** Current band to use*/
    Band        currentBand;
}
LocalLocationInfo;

#if defined (UPGRADE_LTE)

/** 24.301 9.9.3.12
 * MME group id */
typedef Int16
MmeGroupId;

/** 24.301 9.9.3.12
 * MME code */
typedef Int8
MmeCode;

/** 24.301 9.9.3.12
 * MME Allocated TMSI for EPS */
typedef Int32
Mtmsi;

/** 24.301 9.9.3.12
 * GUMMEI */
typedef struct GummeiTag
{
    /** MCC */
    Mcc             mcc;
    /** MNC */
    Mnc             mnc;
    /** Is MNC three digits */
    Boolean         threeDigitsMnc;
    /** MME group id */
    MmeGroupId      mmeGroupId;
    /** MME code */
    MmeCode         mmeCode;
}
Gummei;

/** 24.301 9.9.3.12
 * GUTI */
typedef struct GutiTag
{
    /** GUMMEI */
    Gummei          gummei;
    /** MTMSI */
    Mtmsi           mtmsi;
}
Guti;

#endif /* UPGRADE_LTE */

#endif /* !(SIR_TYP_H) */
/* END OF FILE */

