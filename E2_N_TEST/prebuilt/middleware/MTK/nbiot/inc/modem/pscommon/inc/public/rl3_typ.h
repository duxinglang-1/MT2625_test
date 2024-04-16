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
 *      types common to mmxx_sig.h mmr_sig.h and rr_sig.h
 **************************************************************************/

#ifndef RL3_TYP_H
#define RL3_TYP_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#if !defined (SIR_TYP_H)
#include     <sir_typ.h>
#endif

#if !defined (L13_TYP_H)
#include     <l13_typ.h> /* for num_bands */
#endif

/****************************************************************************
 * Macros
 ****************************************************************************/

#define     NUM_OF_ALGOS                7
#define     MAX_MOBILE_ID_LENGTH        16
#define     SVN_LENGTH                  2
#define     IMEI_LENGTH                 15
#define     IMEISV_LENGTH               MAX_MOBILE_ID_LENGTH
#define     SN_ID_LENGTH                64
#define     MAX_UE_ID_LENGTH            SN_ID_LENGTH


/****************************************************************************
 * Types
 ****************************************************************************/

/* FR9801-1913 Band definition moved to l13_typ.h */
/** RF power capability used/supported */
typedef enum RfPowerCapabilityTag
{   /* Must keep the same values as the L3 module adds one when encoding CM3 */
    /** Class 1*/
    RF_CLASS_1 = 0,
    /** Class 2*/
    RF_CLASS_2 = 1,
    /** Class 3*/
    RF_CLASS_3 = 2,
    /** Class 4*/
    RF_CLASS_4 = 3,
    /** Class 5*/
    RF_CLASS_5 = 4,
    /** Required for new multiband classmark 3 elements */
    RF_CLASS_NULL = 5
#if defined (UPGRADE_3G)
  , RF_CLASS_IRRELEVANT = 7 /**< Class  reserved*/
#endif
}
RfPowerCapability;

/** Indicates what type of identity is used to identify the UE */
typedef enum IdTypeTag
{
    /** No ID available */
    NO_ID                       =   0,
    /** Indicates international mobile subscriber identity  */
    IMSI_ID                     =   1,
    /** Indicates international mobile equipment identity */
    IMEI_ID                     =   2,
    /** Indicates international mobile equipment identity together
     * with the software version number
     */
    IMEISV_ID                   =   3,
    /** Indicates temporary mobile subscriber identity */
    TMSI_ID                     =   4,
    /** Note: This is NOT sent on air, simply used
     * internally so that the Layer 3 module can
     * tell GMM that this is a PTMSI and not a TMSI */
    PTMSI_ID                    =   5,
#if defined (UPGRADE_LTE)
    /** Indicates GUTI */
    GUTI_ID                     =   6,
#endif
    /** The last Item in IdType */
    LAST_ID_TYPE
}
IdType;

/** Indicates the revision level */
typedef enum RevisionLevelTag
{
    RESERVED_FOR_PHASE_1        =   0x00, /**< 00 Used by GSM phase 1 ms*/
    USED_BY_PHASE_2_MS          =   0x01, /**< 01 Used by GSM phase 2 ms */
    USED_BY_R99_MS              =   0x02  /**< 10 Used by ms supporting R99 or later versions */
}
RevisionLevel;

/** The SS screening indicator is sent in the mobile station classmark 2 (2 bits length) */
typedef enum SsScreeningIndicatorTag
{
    /** Default value of phase 1*/
    PHASE1_DEFAULT                      =   0,
    /** Capability of handling of ellipsis notation and phase 2 error handling */
    ELLIPSIS_NOTATION_AND_PHASE2_ERROR  =   1
}
SsScreeningIndicator;

/** Indicates the aspects of MS */
typedef struct MsClassmarkElementTag
{
    /** Indicates if encryption algorithm A5/1 is available.
     * Used by MS Classmark 1, 2 & 3 IE's
     * This boolean array will be indexed '0 to NUM_OF_ALGOS-1',
     * The index+1 thus corresponds to the algorithm A5 number.
     */
    Boolean                         algoA5Available[NUM_OF_ALGOS];

    /** Used by MS Classmark 1 & 2 IE's */
    RevisionLevel                   revisionLevel;
    /** RF power capability. When GSM band is used the MS indicates the RF power capability
     * of the band used and when UMTS band is used the MS indicates the
     * RF power capability corresponding to the (GSM) band it supports: <UL>
     * <LI> [0] = GSM Classmark
     * <LI> [1] = DCS Classmark
     *</UL>
     */
    RfPowerCapability               rfPowerCapability[NUM_BANDS];
    /** Indicates the current band */
    Band                            currentBand;

    /* Used by MS Classmark 2 IE only */
    /** Indicates whether Mobile station supports mobile terminated
     * point to point SMS or not
     */
    Boolean                         smCapability;
    /** Indicates whether pseudo-synchronization  capability is present
     * or not
     */
    Boolean                         psCapability;
    /** The supplementary service screening indicator allows the network
     * to assess the capabilities of the MS in advance of a network initiated
     * SS activity
     */
    SsScreeningIndicator            ssScreeningIndicator;
    /** Indicates if the MS does support the E-GSM or R-GSM */
    Boolean                         frequencyCapability;
    /** This information field indicates the likely treatment by the mobile
     * station of UCS2 encoded character strings. For backward compatibility
     * reasons, if this field is not included, the value 0 shall be assumed by
     * the receiver. Table below show how to interpret this parameter.
     *
     * <TABLE>
     * <TR><TD><B> ucs2Support value </B></TD>    <TD><B> Description              </B></TD></TR>
     * <TR><TD>0   </TD>    <TD> the ME has a preference for the default alphabet
     * (defined in 3GPP TS 23.038 [8b]) over UCS2.         </TD></TR>
     * <TR><TD>1    </TD>    <TD>the ME has no preference between the use of the
     * default alphabet and the use of UCS2.       </TD></TR>
     * </TABLE>
     */
    Boolean                         ucs2Support;
    /** Indicates whether the ME supports SoLSA (Support of Localized Service Areas) or not */
    Boolean                         soLSACapability;
    /** Indicates if classmark 3 is supported */
    Boolean                         classmark3;
    /** Indicates that Controlled Early Classmark Sending option is
     * implemented in the MS
     */
    Boolean                         controlledEarlyClassmarkSending;
    /** Indicates which bands are supported by the MS*/
    BandMode                        radioSupportedBands;
#if defined (UPGRADE_EDGE)
    /** Indicates if EGPRS with 8PSK RF power capability is supported */
    Int8                            egprs8pskRfPowerCapab[NUM_BANDS];
#endif
#if defined (UPGRADE_LTE)
    /** This field is included inside the Element type for LTE, so that
     * we can call the generic 2G/3G function using this as the ctrlBitField
     * parameter. The caller must fill this based on the current capability.
     */
    Int16                           epsCtrlBitField;
#endif
}
MsClassmarkElement;

#if defined(UPGRADE_NVRAM_OPTIMIZE) && defined(UPGRADE_NVRAM_ENHANCED)
typedef struct MsClassmarkElementVarTag
{
    /** Indicates if encryption algorithm A5/1 is availabl.
     * Used by MS Classmark 1, 2 & 3 IE's
     * This boolean array will be indexed '0 to NUM_OF_ALGOS-1',
     * The index+1 thus corresponds to the algorithm A5 number.
     */
    Boolean                         algoA5_0;
    Boolean                         algoA5_1;
    Boolean                         algoA5_2;

    /** RF power capability. When GSM band is used the MS indicates the RF power capability
     * of the band used and when UMTS band is used the MS indicates the
     * RF power capability corresponding to the (GSM) band it supports: <UL>
     * <LI> [0] = GSM Classmark
     * <LI> [1] = DCS Classmark
     *</UL>
     */
    RfPowerCapability               rfPowerCapability[NUM_BANDS];

    /** Indicates the current band */
    Band                            currentBand;

    /** Indicates if the MS does support the E-GSM or R-GSM */
    Boolean                         frequencyCapability;
    /** This information field indicates the likely treatment by the mobile 
     * station of UCS2 encoded character strings. For backward compatibility 
     * reasons, if this field is not included, the value 0 shall be assumed by 
     * the receiver. Table below show how to interpret this parameter.
     *
     * <TABLE>
     * <TR><TD><B> ucs2Support value </B></TD>    <TD><B> Description              </B></TD></TR>
     * <TR><TD>0   </TD>    <TD> the ME has a preference for the default alphabet 
     * (defined in 3GPP TS 23.038 [8b]) over UCS2.         </TD></TR>
     * <TR><TD>1    </TD>    <TD>the ME has no preference between the use of the 
     * default alphabet and the use of UCS2.       </TD></TR>
     * </TABLE>
     */
    Boolean                         ucs2Support;
    /** Indicates whether the ME supports SoLSA (Support of Localized Service Areas) or not */
    Boolean                         soLSACapability;
    /** Indicates if classmark 3 is supported */
    Boolean                         classmark3;
    /** Indicates which bands are supported by the MS*/
    BandMode                        radioSupportedBands;
#if defined(UPGRADE_TWGGE_NRW)
    /** Indicates if EGPRS with 8PSK RF power capability is supported */
    Int8                            egprs8pskRfPowerCapab[NUM_BANDS];
#else
#if defined (UPGRADE_EDGE)
    /** Indicates if EGPRS with 8PSK RF power capability is supported */
    Int8                            egprs8pskRfPowerCapab[NUM_BANDS];
#endif
#endif
}
MsClassmarkElementVar;
#endif
/** SVN element */
typedef struct SvnTag
{
    /** Allows the two digit software version number to be added to the standard
     * IMEI for transmission to the network when IMEISV is requested
     */
    Int8        digit[SVN_LENGTH];
}
Svn;
/** Identifies a mobile.
 * the Mobile Identity information element is to provide either
 * the international mobile subscriber identity, IMSI, the
 * temporary mobile subscriber identity, TMSI/P-TMSI, the
 * international mobile equipment identity, IMEI, the international
 * mobile equipment identity together with the software version
 * number, IMEISV.
 */
typedef struct MobileIdTag
{
    /** Indicates type of identity  */
    IdType                          type;
    /** Represents mobile ID length 
\assoc ARRAY \ref digit */
    Int16                           digitArraySize;
    Int8                            digit[MAX_MOBILE_ID_LENGTH];  /**< Includes SVN in [14],[15] */
    /** Indicates TMSI;
     * in order to ensure subscriber identity confidentiality the VLR (Visitor Location Register)
     * and SGSN (Serving GPRS Support Node) may allocate TMSI (Temporary Mobile Subscriber Identities)
     * to visiting mobile subscribers
     */
    Tmsi                            tmsi;

#if defined (UPGRADE_LTE)
    /* This field is only used in EPS, if the type of identity is GUTI */
    Guti                            guti;
    /* This field is used to store the GUTI in raw format, to exchange with SIM */
    Int8                            rawGuti[RAW_GUTI_IE_SIZE];
#endif /* UPGRADE_LTE */
}
MobileIdElement;

typedef struct SerNumberTag
{
    /** SN, allowed saved max 64 char
     */
    Int16       digitArraySize;
    Int8        digit[SN_ID_LENGTH];
}
SernNumber;

typedef struct ImeiTag
{
    Int8       digit[MAX_MOBILE_ID_LENGTH];
}
Imei;

#endif
/* END OF FILE */
