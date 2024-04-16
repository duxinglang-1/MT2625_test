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
 * Eutra Protocol Stack Common Data Types
 *************************************************************************/

#if !defined (EPS_TYP_H)
#define       EPS_TYP_H

/***************************************************************************
* Nested Include Files
***************************************************************************/

#include <system.h>
#include <eps_asn.h>
#include <ut_mcc_mnc.h>

/** \defgroup ProtoStackAsEutra Eutra Access Stratum
 * \ingroup ProtoStackAs
 * Defines Eutra Access Stratum (AS) interface.
 */


/***************************************************************************
 *   Manifest Constants
 ***************************************************************************/

/***************************************************************************
*   Macro Functions
***************************************************************************/

/** \def EUTRA_LARGE_KEY_SIZE_OCTETS
 *  Defines the key size in Octets for the 256 bit keys. */
#define EUTRA_LARGE_KEY_SIZE_OCTETS     32

/** \def EUTRA_SMALL_KEY_SIZE_OCTETS
 *  Defines the key size in Octets for the 128 bit keys. */
#define EUTRA_SMALL_KEY_SIZE_OCTETS     16

/** \def MAX_PLMN_EUTRA_FREQ_LIST
 *  Defines the maximum number of ARFCN's in the ErrcPlmnListReq. */
#define MAX_PLMN_EUTRA_FREQ_LIST              20

/** \def MAX_NUM_PLMN_IN_SYS_INFO
 * Defines the maximum number of PLMNs in the System Info PLMN list */
#define MAX_NUM_PLMN_IN_SYS_INFO             6

/** \def MAX_ENC_NAS_MESSAGE_LENGTH
 *  Defines the maximum number of octets in the encoded NAS message received from
 * EMM */
#define MAX_ENC_NAS_MESSAGE_LENGTH      1600

/** \def MAC_I_LENGTH
 *  Defines the length of the MAC-I field appended to a control plane PDCP data PDU
 *  (36.323 sec 6.2.2)  */
#define MAC_I_LENGTH    4

/** \def MAX_AIR_INTERFACE_DEBUG_MSG_OCTETS
 *  Defines the Maximum size of an air interface PER encoded debug message in octets */
#define MAX_AIR_INTERFACE_DEBUG_MSG_OCTETS   1024

/** \def MAX_NUM_DISCARDED_EL23_MESSAGES
 * Defines the maximum number of ERRC and EMM message units in the list of rejected PDUs */
#define MAX_NUM_DISCARDED_EL23_MESSAGES      16

/** \def MAX_SUPPORTED_NBIOT_BAND_NUM
 * Defines the maximum number of supported NB-IoT frequency bands */
#define MAX_SUPPORTED_NBIOT_BAND_NUM         19

/** \def ERRC_MAX_REGULAR_FREQUENCY
 * Defines the maximum number of regular NB-IoT frequency list */
#define ERRC_MAX_REGULAR_FREQUENCY           5

/* This macro converts address to point to non-cached memory. */
#define CONVERT_TO_NON_CACHED_MEMORY(type, address) \
    (type *)((uint32_t)address & 0x0FFFFFFF)

/***************************************************************************
*   Types
***************************************************************************/

/** RSRP in dBm
 */
typedef int16_t EutraRsrp;

/** RSRQ in dB
 */
typedef int8_t EutraRsrq;

/** RSSI in dBm
 */
typedef int16_t EutraRssi;

/** Physical layer identity of the cell
 */
typedef uint16_t EutraPhysCellId;

/** EARFCN
 */
typedef uint32_t EutraArfcn;

/** Defines the operating bands of the UE
 *
 * For FDD fddBandMode: Bit 0 corresponds to band 1 up to Bit 13 for band 14,
 * and Bit 16 for band 17
 *
 * For TDD tddBandMode: Bit 0 corresponds to band 33 up to Bit 7 for band 40
 *
 * <TABLE>
 *  <TR><TD>E-UTRA band  </TD><TD>Mode  </TD><TD>Bit  </TD></TR>
 *  <TR><TD>1            </TD><TD>FDD   </TD><TD>0    </TD></TR>
 *  <TR><TD>2            </TD><TD>FDD   </TD><TD>1    </TD></TR>
 *  <TR><TD>3            </TD><TD>FDD   </TD><TD>2    </TD></TR>
 *  <TR><TD>4            </TD><TD>FDD   </TD><TD>3    </TD></TR>
 *  <TR><TD>5            </TD><TD>FDD   </TD><TD>4    </TD></TR>
 *  <TR><TD>6            </TD><TD>FDD   </TD><TD>5    </TD></TR>
 *  <TR><TD>7            </TD><TD>FDD   </TD><TD>6    </TD></TR>
 *  <TR><TD>8            </TD><TD>FDD   </TD><TD>7    </TD></TR>
 *  <TR><TD>9            </TD><TD>FDD   </TD><TD>8    </TD></TR>
 *  <TR><TD>10           </TD><TD>FDD   </TD><TD>9    </TD></TR>
 *  <TR><TD>11           </TD><TD>FDD   </TD><TD>10   </TD></TR>
 *  <TR><TD>12           </TD><TD>FDD   </TD><TD>11   </TD></TR>
 *  <TR><TD>13           </TD><TD>FDD   </TD><TD>12   </TD></TR>
 *  <TR><TD>14           </TD><TD>FDD   </TD><TD>13   </TD></TR>
 *  <TR><TD>17           </TD><TD>FDD   </TD><TD>16   </TD></TR>
 *  <TR><TD>33           </TD><TD>TDD   </TD><TD>0    </TD></TR>
 *  <TR><TD>34           </TD><TD>TDD   </TD><TD>1    </TD></TR>
 *  <TR><TD>35           </TD><TD>TDD   </TD><TD>2    </TD></TR>
 *  <TR><TD>36           </TD><TD>TDD   </TD><TD>3    </TD></TR>
 *  <TR><TD>37           </TD><TD>TDD   </TD><TD>4    </TD></TR>
 *  <TR><TD>38           </TD><TD>TDD   </TD><TD>5    </TD></TR>
 *  <TR><TD>39           </TD><TD>TDD   </TD><TD>6    </TD></TR>
 *  <TR><TD>40           </TD><TD>TDD   </TD><TD>7    </TD></TR>
 * </TABLE>
 */

typedef struct EutraBandModeTag
{
    Int32                               fddBandMode;
    Int32                               tddBandMode;
}
EutraBandMode;

/** Frequency band
 */
typedef uint16_t EutraBand;
typedef uint16_t EutraFrequencyBand;

/** Tracking area code
 */
typedef uint16_t EutraTrackingAreaCode;

/** Cell identity
 */
typedef uint32_t EutraCellIdentity;

/** Defines the UE Capability for EUTRA */
typedef struct EutraMobileEquipmentDataTag
{
    /** UE Power Class (1 to 4). Currently only 3 specified 23dBm per band. */
    Int8                                uePowerClass;
}
EutraMobileEquipmentData;

/** Defines the Tracking area identity which is made up of the PLMN + TAC. */
typedef struct EutraTrackingAreaIdentityTag
{
    /** Defines the PLMN for this tracking area. */
    Plmn                                plmn;

    EutraTrackingAreaCode               tac;

/* TODO: Remove rest of the parameters */
    /** Indicates if the MNC of #plmn 2 or 3 digits  */
    Boolean                             threeDigitMnc;

    /** Defines the TAC. */
    EutraAsn_TrackingAreaCode           trackingAreaCode;
}
EutraTrackingAreaIdentity;

/** Defines the GUMMEI of the MME where the UE is registered */
typedef struct EutraRegisteredMmeTag
{
    /** Defines the MCC and MNC for the register MME. */
    Plmn                                plmn;

    /* TODO: Remove field threeDigitMnc, because same information is in Plmn structure. */
    /** Indicates if the MNC of #plmn 2 or 3 digits  */
    Boolean                             threeDigitMnc;

    /** Provides the Group Identity of the registered MME within the #plmn. */
    Int16                               mmegi;

    /** Identifies an MME within the scope of an MME Group within the #plmn. */
    EutraAsn_MMEC                       mmec;
}
EutraRegisteredMme;

/** Indicates whether access for MO calls, Emergency Calls, MT calls
 *  or MO signalling is barred or not.*/
typedef struct EutranAccessBarredStatusTag
{
    /** If TRUE then access for MO calls are barred. */
    Boolean                             moCallsBarred;

    /** If TRUE then access for MT calls are barred. */
    Boolean                             mtCallsBarred;

    /** If TRUE then access for MO signalling is barred. */
    Boolean                             moSignallingBarred;

    /** If TRUE then access for emergency calls are barred. */
    Boolean                             emergencyCallsBarred;
}
EutranAccessBarredStatus;

/** Defines the channels on which to perform a restricted PLMN search */
typedef struct EutraArfcnListTag
{
    /** Defines the number of EARFCN's in #earfcn
\assoc ARRAY \ref earfcn */
    Int8                                numberOfEarfcn;

    /** Defines the list of channels to perform the PLMN search.
     * \see \ref SigEl1EarfcnToFrequency. */
    EutraArfcn                          earfcn [MAX_PLMN_EUTRA_FREQ_LIST];
}
EutraArfcnList;

/** EARFCN offset */
typedef enum EutraEarfcnOffsetTag
{
    EUTRAN_EARFCN_OFFSET_UNKNOWN,
    EUTRAN_EARFCN_OFFSET_MINUS_10,
    EUTRAN_EARFCN_OFFSET_MINUS_9,
    EUTRAN_EARFCN_OFFSET_MINUS_8,
    EUTRAN_EARFCN_OFFSET_MINUS_7,
    EUTRAN_EARFCN_OFFSET_MINUS_6,
    EUTRAN_EARFCN_OFFSET_MINUS_5,
    EUTRAN_EARFCN_OFFSET_MINUS_4,
    EUTRAN_EARFCN_OFFSET_MINUS_3,
    EUTRAN_EARFCN_OFFSET_MINUS_2,
    EUTRAN_EARFCN_OFFSET_MINUS_1,
    EUTRAN_EARFCN_OFFSET_MINUS_0_DOT_5,
    EUTRAN_EARFCN_OFFSET_0,
    EUTRAN_EARFCN_OFFSET_1,
    EUTRAN_EARFCN_OFFSET_2,
    EUTRAN_EARFCN_OFFSET_3,
    EUTRAN_EARFCN_OFFSET_4,
    EUTRAN_EARFCN_OFFSET_5,
    EUTRAN_EARFCN_OFFSET_6,
    EUTRAN_EARFCN_OFFSET_7,
    EUTRAN_EARFCN_OFFSET_8,
    EUTRAN_EARFCN_OFFSET_9
} EutraEarfcnOffset;

/** Carrier */
typedef struct EutraCarrierTag
{
    EutraArfcn          earfcn;

    /** Value EUTRAN_EARFCN_OFFSET_UNKNOWN indicates that field is not present */
    EutraEarfcnOffset    earfcnOffset;
} EutraCarrier;

/** Defines the Eutra DRB Identity for configuring a Eutra L2 bearer.
 *  Range: 1-32.
 */
typedef Int32 EutraDrbSrbBearerIdentity;

/** Defines the Eutra EPS Identity for configuring a Eutra L2 bearer.
 *  Range: 0-15
 */
typedef Int8 EutraEpsIdentity;

/** Defines the Eutra logical channel identity of an Eutra logical mapped to a DRB.
 *  Note. The values 0-2 are reserved for and have standard mappings to the SRBs.
 *  Range: 1-10
 */
typedef Int32 EutraLogicalChannelIdentity;

/** Defines the Eutra log channel priority for a Eutra logical channel
 *  Range: 1-16 (EUTRA_BEARER_PRIORITY_MIN - EUTRA_BEARER_PRIORITY_MAX)
 */
typedef Int32 EutraLogChPriority;

/** 24.301 9.9.3.23
 * EPS security algorithms
 * Identifiers for EPS Integrity Algorithm to be used */
typedef enum EpsIntegAlgorithmTag
{
    EPS_INTEG_EIA0     = 0,
    EPS_INTEG_128_EIA1 = 1,
    EPS_INTEG_128_EIA2 = 2,
    EPS_INTEG_128_EIA3 = 3,
    EPS_INTEG_EIA4     = 4,
    EPS_INTEG_EIA5     = 5,
    EPS_INTEG_EIA6     = 6,
    EPS_INTEG_EIA7     = 7
}
EpsIntegAlgorithm;

/** 24.301 9.9.3.23
 * EPS security algorithms
 * Identifiers for EPS Encryption Algorithm to be used */
typedef enum EpsEncryptAlgorithmTag
{
    EPS_ENCRYPT_EEA0     = 0,
    EPS_ENCRYPT_128_EEA1 = 1,
    EPS_ENCRYPT_128_EEA2 = 2,
    EPS_ENCRYPT_128_EEA3 = 3,
    EPS_ENCRYPT_EEA4     = 4,
    EPS_ENCRYPT_EEA5     = 5,
    EPS_ENCRYPT_EEA6     = 6,
    EPS_ENCRYPT_EEA7     = 7
}
EpsEncryptAlgorithm;

/** Defines the Security Key type for KeNb used by ERRC to derive the
 * integrity and cipher keys. */
typedef Int8 EutraKeNb [EUTRA_LARGE_KEY_SIZE_OCTETS];

/** Defines the NH value defined in TS 33.401 by the NAS and AS. */
typedef Int8 EutraNh [EUTRA_LARGE_KEY_SIZE_OCTETS];

/** Identifies the type of key in KeNb/NH union */
typedef enum EutraKeNbNhTypeTag
{
    /** Key type is KeNb */
    EUTRA_KEY_TYPE_KENB,
    /** Key type is NH */
    EUTRA_KEY_TYPE_NH
}
EutraKeNbNhType;

/** Union of KeNb and NH */
typedef union EutraKeNbNhUnionTag
{
    /** Defines the KeNb as defined in TS 33.401.
     *  This is used by ERRC to derive KrrcEnc, KrrcInt and KupEnc. */
    EutraKeNb   eutraKeNb;
    /** Defines the NH values as defined in TS 33.401.
     *  EMM derives this from the Kasme and #nextChainingCount. */
    EutraNh     eutraNh;
}
EutraKeNbNhUnion;

/** Defines the Security Key type for KrrcEnc for the RRC C-Plane encryption. */
typedef Int8 EutraKrrcEnc [EUTRA_SMALL_KEY_SIZE_OCTETS];

/** Defines the Security Key type for KrrcInt for the RRC C-Plane integrity protection. */
typedef Int8 EutraKrrcInt [EUTRA_SMALL_KEY_SIZE_OCTETS];

/** Defines the Security Key type for KupEnc for U-Plane encryption. */
typedef Int8 EutraKupEnc [EUTRA_SMALL_KEY_SIZE_OCTETS];

typedef struct EutraSecurityConfigTag
{
    /** 128 bits key for ciphering users data
     */
    EutraKupEnc             upEncKey;
    /** 128 bits key for ciphering RRC signalling data
     */
    EutraKrrcEnc            rrcEncKey;
    /** 128 bits key for applying integrity protection to
     *   RRC signalling data
     */
    EutraKrrcInt            rrcIntKey;
    /** Algorthim used for applying integrity protection to
     *   RRC signalling data in Eutra LTE
     */
    EpsIntegAlgorithm       integrityAlgo;
    /** Algorthim used for ciphering data in Eutra LTE
     */
    EpsEncryptAlgorithm     cipheringAlgo;
}
EutraSecurityConfig;

/** Defines the Eutra COUNT value for security.
 */
typedef uint32_t EutraCount;

/** Defines the bitmask for 3 digit MNC in #PlmnSysInfoList.
 */
typedef Int8 ThreeDigitMncBits;

/** Contains the PLMN list in the system information.
  * The first listed PLMN Identity is the primary PLMN.
  */
typedef struct PlmnSysInfoListTag
{
    /** Defines number of PLMNs included in the list.
\assoc ARRAY \ref plmn */
    Int8                            numPlmn;

    /** Identifies PLMNs in the list. */
    Plmn                            plmn[MAX_NUM_PLMN_IN_SYS_INFO];

    /** Attach without PDN connectivity support on PLMN */
    Boolean                         attachWithoutPDNConnectivitySupported[MAX_NUM_PLMN_IN_SYS_INFO];

    /** Defines if 3 digit MNC is used. Bit mask with b0 corresponding
     *  to first PLMN in list. */
    ThreeDigitMncBits               threeDigitMncEncoding;
}
PlmnSysInfoList;

/** Defines a structure to contain the encoded NAS message string for
    delivery to ERRC   */
typedef struct EutraEncNasMessageTag
{

    /** Defines length of the NAS message.
\assoc ARRAY \ref data */
    Int16      length;

    /** Defines the NAS Message. */
    Int8       data[MAX_ENC_NAS_MESSAGE_LENGTH];
}
EutraEncNasMessage;

/** Defines a structure of the NAS message container   */
typedef struct EutraEncNasMessageContainerTag
{
    /** Defines length of the NAS message container
\assoc ARRAY \ref data */
    Int16    length;

    /** Defines pointer to the data content */
    Int8    *data;
}
EutraEncNasMessageContainer;


    /** Defines the list of EL23 PDU's that have been discarded by EL2. */
typedef struct El23DiscardedMsgListTag
{
     /** Defines the number of actual discarded L23 messages in the array
 \assoc ARRAY \ref mui */
     Int8                   numDiscardedMsgs;
     /** Defines the muis which identify the EL23 message units */
     Int8                   mui[MAX_NUM_DISCARDED_EL23_MESSAGES];
}
El23DiscardedMsgList;

#endif /* EPS_TYP_H */

/* END OF FILE */
