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
 *      Mm - sim manager interface
 **************************************************************************/

#ifndef MMSI_SIG_H
#define MMSI_SIG_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#if !defined (MPSI_TYP_H)
#include     <mpsi_typ.h>
#endif

#if !defined (SIR_TYP_H)
#include     <sir_typ.h>
#endif

#if !defined (ALSI_TYP_H)
#include     <alsi_typ.h>
#endif

#if !defined (ALSI_SIG_H)
#include     <alsi_sig.h>
#endif

#if !defined (L13_TYP_H)
#include <l13_typ.h>
#endif

/** \defgroup SigSim SIM Signalling Interface
 * \ingroup Prd3gDmNas
 * \ingroup Sig
 * Interface between SIM manager and layer 3 Mobility Manager (MM).
 * This section defines the interface between GSM/3G/LTE Mobility Manager
 * (MM) and the USIM manager. This interface is known as SIM.
 *
 * The SIM interface allows MM to write data to, read data from, and
 * delete data within, the SIM/UICC, and also to request the SIM/UICC
 * to perform authentication procedures. The USIM manager sends back
 * the results of these requests to MM. This interface is for data only.
 *
 * The signals used for this interface are based on the requirements of
 * MM to read, write, and delete SIM/UICC data, and to activate authentication
 * procedures within the SIM/UICC.
 *
 * The context of the SIM interface is illustrated in the following diagram.
 *
 * \image html USIM_Manager.png
 * @{
 */
/****************************************************************************
 * Macros
 ****************************************************************************/

/****************************************************************************
 * Manifest Constants
 ***************************************************************************/

#define     DELETED_LAI_PH_1      0        /* see 04.08 10.5.1.3 */
/** If a LAI has to be deleted then all bits of the
 * location area code shall be set to one with the
 * exception of the least significant bit which shall be
 * set to zero (see 04.08 10.5.1.3)
 */
#define     DELETED_LAI_PH_2      0xFFFEU
#define     NEVER_USED_LAI        0        /* see 04.08 10.5.1.3 */

/****************************************************************************
 * Types
 ****************************************************************************/


/** Determines the outcome of the request */
typedef enum MmSiRequestStatusTag
{
   MM_SIM_REQ_OK,               /**< Valid request */
   MM_SIM_INVALID_MAC,          /**< Invalid MAC*/
   MM_SIM_SYNC_FAILURE,         /**< Synchronisation failure */
   MM_SIM_GSM_CONTEXT_NOT_SUP,  /**< GSM security context not supported */
   MM_SIM_ACCESS_DENIED,        /**< Authentication failed*/
   MM_SIM_NOT_UPDATED,          /**< SIM could not be updated*/
   MM_SIM_SERVICE_UNAVAIL,      /**< Service unavailable*/
   MM_SIM_AMF_FAILURE,          /**<if the sep bit of AMF field not 0 */
   MM_SIM_UNKNOWN_ERROR         /**< Other error*/
} MmSiRequestStatus;

/** Cipher key data element which corresponds to the data
 * stored in EF KEYS or EF KEYS PS.
 * if  UPGRADE_REL5 is defined, IK and CK will not be passed
 * in the signals as MM will not use them. USIM manager will
 * automatically update EF KEYS with the new values after
 * authentication.
 */
typedef struct CipherKeyDataTag
{
    /** corresponds to the sequence number, also called Key Set Identifier */
    KeySequence      ksi;
    /** The cipher key */
    CipherKey        ck;
    /** The integrity key */
    IntegrityKey     ik;
}
CipherKeyData;

/** Specifies MM services */
typedef enum MmSimServiceStatusTag
{
   /** Normal service */
   MM_SIM_NORMAL_SERVICE,
   /** No service */
   MM_SIM_NO_SERVICE,
   /** Emergency service */
   MM_SIM_EMERG_SERVICE
} MmSimServiceStatus;


/****************************************************************************
 * Signal types
 ****************************************************************************/
/** Informs MM that the USIM (or GSM) application has started.
 * This signal is sent from USIM manager when a SIM/UICC is inserted
 * or when a new USIM application is started for instance.
 */
typedef struct MmSimAppStartedIndTag
{
#if defined (FEA_KOR_LGUP)
    CardType                        cardType;
#endif
    /** The value of the IMSI */
    Imsi                            imsi;
    /** The access control class bits */
    AccessClass                     accessClass;
    /** Will be set to 0xff if invalid or not present*/
    SimPlmn                         actingHplmn;
    /** MS operation mode */
    MsOperationMode                 msOperationMode;
    /** Whether it is a USIM or not */
    Boolean                         usim;
    /** Length of the MNC 2 or 3 (if known else 0) */
    Int8                            mncLength;
} MmSimAppStartedInd;

/** Informs EMM that USIM has been removed.
 */
typedef struct MmSimRemovedIndTag
{
    /** If the EPS Location information has been successfully read - either from the USIM or
         *  NVRAM if the EPS files are not present on the USIM.
       \assoc PRESENT \ref epsLocInfo */
    Boolean                        epsLocInfoValid;
    /** Data read from the EF(EPS Location Info) or the equivalent in NVRAM */
    UsimEpsLocInfo                 epsLocInfo;
} MmSimRemovedInd;

/** This signal indicates the network mode for an LTE build
 *  so that the correct information is read from the USIM.
 for non-LTE builds this is an empty signal.
 */
typedef struct MmSimReadDataReqTag
{
    NetworkModes                      networkMode;
    Boolean                           readSecurityData;
    Boolean                           readEpsLociData;
    /** only needed if doing ATTACH with IMSI attach for SMS */
    Boolean                           readCsLoci;
} MmSimReadDataReq;

/** This signal is sent as a response to MmSimReadDataReq.
 * This signal is sent from USIM manager to MM to read some data
 * related to location information or security.
 *
 * If any error occurs when reading data from the SIM/UICC, or if
 * any procedural error cannot be resolved, the valid flag associated
 * with the data will be set to FALSE.
 */
typedef struct MmSimReadDataCnfTag
{
    /** Indicates that the SIM/UICC is present
\assoc PRESENT \ref gprsEnabledSim
\assoc PRESENT \ref imsi
\assoc PRESENT \ref accessClass
\assoc PRESENT \ref msOperationMode */
    Boolean                         present;
    /** Indicates whether the USIM manager successfully read all the file it tried to access */
    Boolean                         success;
    /** flag indicates whether the SIM/UICC is GPRS-enabled; if the
     * flag is set, this means that some files like EF(KCGPRS) are
     * present on the SIM/UICC */

    Boolean                         gprsEnabledSim; /* To be removed */
#if defined (UPGRADE_3G)
    /** Indicates that the startHfnCs and startHfnPs fields are valid;
     * they convey the values of START CS and START PS, as defined in
     * TS 131.102

\assoc PRESENT \ref startHfnCs
\assoc PRESENT \ref startHfnPs */
    Boolean                         startHfnValid; /* To be removed */
    /** Convey the value of START CS as defined in TS 131.102
     * (3 bytes are used; unused byte is set to FF)
     */
    Int32                           startHfnCs; /* To be removed */
    /** Convey the value of START PS as defined in TS 131.102
     * (3 bytes are used; unused byte is set to FF)
     */
    Int32                           startHfnPs; /* To be removed */
    /** Indicates that the <I> threshold </I> field is valid
\assoc PRESENT \ref threshold */
    Boolean                         thresholdValid; /* To be removed */
    /** Convey the value of the THRESHOLD, as defined in TS 131.102
     * (3 bytes are used; unused byte is set to FF)
     */
    Int32                           threshold; /* To be removed */
    /** Indicate the presence of EF(KEYS) and whether this file was read
     * successfully (This file is only present on 3G SIM cards, under
     * the ADF USIM directory)

\assoc PRESENT \ref csKey */
    Boolean                         csKeyValid; /* To be removed */
    /** Conveys the cipher and integrity keys read from EF(KEYS) */
    CipherKeyData                   csKey; /* To be removed */
    /** Indicate the presence of EF(KEYSPS) and whether this file was read
     * successfully (This file is only present on 3G SIM cards, under
     * the ADF USIM directory)

\assoc PRESENT \ref psKey */
    Boolean                         psKeyValid; /* To be removed */
    /** Conveys the cipher and integrity keys read from EF(KEYSPS) */
    CipherKeyData                   psKey; /* To be removed */
#endif
    /** If the EPS Location information has been successfully read - either from the USIM or
     *  NVRAM if the EPS files are not present on the USIM.
\assoc PRESENT \ref epsLocInfo */
    Boolean                        epsLocInfoValid;
    /** Data read from the EF(EPS Location Info) or the equivalent in NVRAM */
    UsimEpsLocInfo                 epsLocInfo;
    /** If the EPS Security Information has been successfully read - either from the USIM or
     *  NVRAM if the EPS files are not present on the USIM.
\assoc PRESENT \ref epsNasSecurityContext */
    Boolean                        epsSecurityContextValid;
    /** Data read from EF(EPS NAS Security Context) or the equivalent in NVRAM */
    UsimEpsNasSecurity             epsNasSecurityContext;
    /** If the IMSI has been successfully read - either from the USIM or
     *  NVRAM if the EPS files are not present on the USIM.
\assoc PRESENT \ref epsEmmInfoImsi */
    Boolean                        epsEmmInfoImsiValid;
    /** EMM info IMSI */
    Imsi                           epsEmmInfoImsi;
    /** Indicate the presence of EF(KC).  This file is always present
     * on GSM SIM cards. It will only be present on UICC cards if the
     * GSM Access service is enabled

\assoc PRESENT \ref kc */
    Boolean                         kcValid; /* To be removed */
    /** Conveys the cipher key read from EF(KC) */
    CipheringKeyData                kc;  /* To be removed */
    /** Indicates whether EF(LOCI) was read successfully
\assoc PRESENT \ref locInfo */
    Boolean                         locInfoValid;
    /** Is the data read from EF(LOCI) */
    LocationInformation             locInfo;
    /** IMSI */
    Imsi                            imsi;
    /** Will be set to 0xff if not present or invalid*/
    SimPlmn                         actingHplmn;
    /** Indicates the access class */
    Int16                           accessClass;
    /** MS operation mode */
    MsOperationMode                 msOperationMode;
    /** Indicate the presence of EF(KC GPRS)
\assoc PRESENT \ref gprsKc */
    Boolean                         gprsKcValid; /* To be removed */
    /** Contains the GPRS cipher key */
    CipheringKeyData                gprsKc; /* To be removed */
    /**  Indicates whether EF(LOCI GPRS), for GSM SIM cards, or
     * EF(PS LOCI), for UICC cards, was read successfully

\assoc PRESENT \ref psLocInfo */
    Boolean                         psLocInfoValid; /* To be removed */
    /** Is the data read from the EF(LOCI GPRS)and EF(PS LOCI) files  */
    RoutingInformation              psLocInfo; /* To be removed */
    /** Length of the MNC 2 or 3 (if known else 0) */
    Int8                            mncLength;
    /**  nasDataConfig parameters */
    AlsiReadNasConfigCnf            nasDataConfig;
}
MmSimReadDataCnf;

/** This signal is sent from MM to USIM manager to write some data
 * related to location information or security
 */
typedef struct MmSimWriteDataReqTag
{
#if defined (UPGRADE_3G)
    /** set to TRUE, EF(START HFN) should be updated with the new
     * values of startHfnCs and startHfnPs

\assoc PRESENT \ref startHfnCs
\assoc PRESENT \ref startHfnPs */
    Boolean                         startHfnValid; /* To be removed */
    /** New START CS value
     * (3 bytes are used; unused byte should be set to FF)
     */
    Int32                           startHfnCs; /* To be removed */
    /** New START PS value
     * (3 bytes are used; unused byte should be set to FF)
     */
    Int32                           startHfnPs; /* To be removed */
    /** Indicate that EF(KEYS) respectively should be updated;
     * this file is only present on UICC cards, under the ADF(USIM)
     * directory

\assoc PRESENT \ref csKey */
    Boolean                         csKeyValid;  /* To be removed */
    /** Conveys the value of cipher and integrity keys in the Circuit
     * Switched domain to be written to EF(KEYS)
     */
    CipherKeyData                   csKey; /* To be removed */
    /** Indicate that EF(KEYS PS) respectively should be updated;
     * this file is only present on UICC cards, under the ADF(USIM)
     * directory

\assoc PRESENT \ref psKey */
    Boolean                         psKeyValid;  /* To be removed */
    /** Conveys the value of cipher and integrity keys in the Packet
     * Switched domain to be written to EF(KEYS PS)
     */
    CipherKeyData                   psKey; /* To be removed */
#endif
    /** Indicates if the EPS Security Context should be updated.
\assoc PRESENT \ref epsNasSecurityContext */
    Boolean                         epsSecurityContextValid;
    /** Conveys the EPS Security Context to be written to EF(EPSNSC)
     * or NVRAM if the file is not present on the SIM.
     */
    UsimEpsNasSecurity             epsNasSecurityContext;
    /**  Indicates if the EPS Location Information should be updated
\assoc PRESENT \ref epsLocationInfo */
    Boolean                         epsLocationInfoValid;
    /** Conveys the EPS location information to be written to EF(EPS LOCI)
     * or NVRAM if the file is not present on the SIM. */
    UsimEpsLocInfo                  epsLocationInfo;
    /** when in LTE mode only write CsLociInfo if it is relevant - i.e. if we
    * have a combined attach
\assoc PRESENT \ref locInfo */
    Boolean                         writeCsLociInfo;
    /** IMSI associated with the EMM info */
    Imsi                            epsEmmInfoImsi;
    /** Indicate whether EF(KC) should be updated.
     * This file is always present on GSM SIM cards.
     * It will only be present on UICC cards if the GSM
     * Access service is enabled

\assoc PRESENT \ref kc */
    Boolean                         kcValid; /* To be removed */
    /** Conveys the value of the cipher key to be written to EF(KC) */
    CipheringKeyData                kc; /* To be removed */
    /** Is the location information data to write to EF(LOCI) */
    LocationInformation             locInfo;
    /** Current band of operation which is used in MNC encoding */
    Band                            currentBand; /* FR0102-9447: SIM handling of band dependant MNC encoding */
    /** Indicates whether EF KC(GPRS) should be updated
\assoc PRESENT \ref gprsKc */ /* To be removed */
    Boolean                         gprsKcValid; /* To be removed */
    /** Contains the value of the GPRS cipher key */
    CipheringKeyData                gprsKc; /* To be removed */
    /** Indicates whether EF(LOCI GPRS), for GSM SIM cards,
     * or EF(PS LOCI), for UICC cards, should be updated

\assoc PRESENT \ref psLocInfo */
    Boolean                         psLocInfoValid; /* To be removed */
    /**  Is the location information data to write to the
     * EF(LOCI GPRS)and EF(PS LOCI) files
     */
    RoutingInformation              psLocInfo; /* To be removed */
}
MmSimWriteDataReq;

/** This signal is sent from USIM manager to MM to indicate
 * whether the data was written successfully to the SIM/UICC files
 */
typedef struct MmSimWriteDataCnfTag
{
    /** Set to TRUE if data was written successfully to the SIM/UICC files */
    Boolean                         success;
}
MmSimWriteDataCnf;

/** This signal is sent from (E)MM to USIM Manager to request the USIM Manager to
 * perform the EPS AKA authentication routine.  The outputs from the USIM (CK and IK are
 * processed within the USIM Manager to generate KASME. Future releases of USIM may
 * generate this value directly.
 */
typedef struct MmSimEpsAuthenticateReqTag
{
    /** Key sequence identifier supplied by network */
    KeySequence                     ksiAsme;
    /** Serving network ID - used to derive KASME  -first 3 octets from raw tai for MCC
     * and MNC. Using the raw data means the 2 digit/ 3 digit MNC issue is already handled */
    Int8                            snId[MNC_MCC_DIGIT_LENGTH];
    /** convey the Random Challenge required by the USIM to calculate
     * the user authentication response (RES); This will generate a
     * new pair of cipher and integrity keys.
     */
    Rand                            rand;
    /** convey the Authentication Token required by the USIM to calculate
     * the user authentication response (RES); This will generate a
     * new pair of cipher and integrity keys.
     */
    Aut                             autn;
     /** Used to match the response to the request */
    Int8                            authCipherRefNum;
}
MmSimEpsAuthenticateReq;
/** This signal is the response to the request from (E)MM for an EPS AKA
 * authentication routine.  The response data, if successful, supplies the KASME
 * to (E)MM for calculating the NAS ciphering and integrity keys and KENB and NH for
 * Access Stratum.
 */
typedef struct MmSimEpsAuthenticateCnfTag
{
    /** Indicates whether or not the request was successful */
    MmSiRequestStatus              requestStatus;
    /** Whether or not the Kasme is valid.
\assoc PRESENT \ref kAsme */
    Boolean                        kAsmeValid;
    /** Kasme calculated from USIM output CK and IK */
    EutraKAsmeData                 kAsme;
    /** Contains both IK and CK*/
    CipherKeyData                  keys;
    /** If cause is synchronisation failure then AUTS is returned  */
    Aut                            auts;
    /** Response */
    Res                            res;
    /** The Random Challenge */
    Rand                           rand;
    /** Used to match the response to the request (Copied from the request signal)*/
    Int8                           authCipherRefNum;
}
MmSimEpsAuthenticateCnf;
/** This signal is sent by MM to the USIM Manager to inform the
 * USIM Manager of the current connection status when there is
 * a connection status change
 */
typedef struct MmSimConnectionIndTag
{
    /** Whether or not the connection is established */
    Boolean                         connectionEstablished;
}
MmSimConnectionInd;

/** This signal is sent by MM to the USIM Manager
 *  to inform the eDRX status.
 */
typedef struct MmSimLowPowerIndTag
{
    /** Whether or not the eDRX is enabled */
    Boolean             eDrxEnabled;
}
MmSimLowPowerInd;

/** @} */ /* End of SigSim group */
#endif   /* MMSI_SIG_H */
/* END OF FILE */





























