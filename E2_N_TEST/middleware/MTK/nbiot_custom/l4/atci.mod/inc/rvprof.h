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
 * File Description
 * ----------------
 *
 * Profile enumerated type
 * Enumerated types and constants which match up to profile entries
 **************************************************************************/

#ifndef RVPROF_H
#define RVPROF_H

#ifndef GEN_NVDM_DATABASE_FOR_PC
#include <ablm_typ.h>
#endif
#include <rvsystem.h>
#include <cimux_typ.h>
#include <rvcrdata.h>

/* Some profile entries require multiple bytes, the size of these are */
/* defined below                                                      */

#define NUM_IFC                   (2)
#define NUM_ICF                   (2)
#define NUM_CNMI                  (5)
#define NUM_DS                    (5)
#define NUM_CHECKSUM              (8)

#define NUM_CGPIAF                (4)

/* Profile Enumerated Type */

/* Every profile entry has a corresponding handle below */
/* Some entries require multiple bytes (see above)      */

typedef enum ProfIdTag
{
  PROF_ECHO   = 0,
  PROF_LOUDNESS,
  PROF_MONITOR,
  PROF_QUIET,
  PROF_VERBOSE,
  PROF_X,
  PROF_DCD,
  PROF_DTR,
  PROF_S0,
  PROF_S1,
  PROF_S2,
  PROF_S3,
  PROF_S4,
  PROF_S5,
  PROF_S6,
  PROF_S7,
  PROF_S8,
  PROF_S10,
  PROF_S12,
  PROF_S25,
  PROF_CR,
  PROF_FCLASS,
  PROF_IFC,
  PROF_ICF  = PROF_IFC + NUM_IFC,
  PROF_DR  = PROF_ICF + NUM_ICF,
  PROF_DS,
  PROF_CMGF = PROF_DS + NUM_DS,
  PROF_CSDH,
  PROF_IPR,
  PROF_ILRR,
  PROF_MSIMINS,
  PROF_CSCS,
  PROF_CME,
  PROF_CREG,
  PROF_CGREG,
  PROF_CEREG,
  PROF_MUX,
  PROF_MUNSOL,
  PROF_CTZR,
  PROF_CDIP,
  PROF_MODE,
#if defined (FEA_MT_PDN_ACT)
  PROF_CGAUTO,
#endif  
  PROF_CGEREP,
#if defined (FEA_MT_PDN_ACT)
  PROF_MGMTPCACT,
#endif
  PROF_MSTMODE,

#if defined (FEA_PPP)  
  PROF_MGPPPLOG,
#endif /* FEA_PPP */

  PROF_MMGI,
  PROF_MROUTEMMI,
  PROF_MUPBCFG,
  PROF_MCEERMODE,
  PROF_CGPIAF,
  PROF_MCGEUNSOL = PROF_CGPIAF + NUM_CGPIAF,
  PROF_MLTS,
  PROF_MUAPP,
#if defined (ENABLE_AT_TRACE)
  PROF_TRACE,
#endif
  PROF_MLTEGCFLOCK,

  /* Profile values for NB-IOT */
  PROF_CEDRXS,
  PROF_CCIOTOPT,
#if defined (FEA_NFM)
  PROF_NFM,
#endif
  PROF_MPDI,
  PROF_MPLMNURI,
  PROF_MAPNURI,
  PROF_MATWAKEUP,
  PROF_MDPDNP,
  PROF_MNBIOTEVENT,
  PROF_MOOSAIND,
  NUM_OF_PROF_SPECIFIC,
  PROF_CSMS = NUM_OF_PROF_SPECIFIC,
  PROF_CNMI,
  PROF_CTZU = PROF_CNMI + NUM_CNMI,
  PROF_MLTEGCF,
  NUM_OF_PROF
}
ProfId;

/* Extra 8 spaces in profile for NVRAM codeword */

#define NUM_OF_PROF_SPECIFIC_ENTRIES     (NUM_OF_PROF_SPECIFIC + NUM_CHECKSUM)
#define NUM_OF_PROF_GENERIC              (NUM_OF_PROF - NUM_OF_PROF_SPECIFIC)
#define NUM_OF_PROF_GENERIC_ENTRIES      (NUM_OF_PROF_GENERIC + NUM_CHECKSUM)

/* Type definition for profile structure */

/* N.B. If the number of elements in the profile is to be altered, apart from
 * modifying the above list, you must also change the Factory Defaults to
 * match (cicode/rvctcfg.c).
 */

typedef struct ProfileTag
{
  Int8  value[NUM_OF_PROF_SPECIFIC_ENTRIES];
}
ProfileContext_t;

typedef struct ProfileGenericTag
{
  Int8  value[NUM_OF_PROF_GENERIC_ENTRIES];
}
ProfileGenericContext_t;

/* Bit masks for profile values are defined below */

typedef enum VgProfileBitTag
{
    PROF_BIT_ONE        = 0x01,
    PROF_BIT_TWO        = 0x02,
    PROF_BIT_THREE      = 0x04,
    PROF_BIT_FOUR       = 0x08,
    PROF_BIT_FIVE       = 0x10,
    PROF_BIT_SIX        = 0x20,
    PROF_BIT_SEVEN      = 0x40,
    PROF_BIT_EIGHT      = 0x80,

    /* PROF_MUNSOL */

    PROF_BIT_MSQN       = PROF_BIT_ONE,
    PROF_BIT_MFPLMN     = PROF_BIT_TWO,
    PROF_BIT_SMSINFO    = PROF_BIT_THREE,
    PROF_BIT_MMWT       = PROF_BIT_FOUR,
    END_OF_MUNSOL_BITS,

    /* PROF_MROUTEMMI */

    /** Whether the channel is hooked to MMI.*/
    PROF_BIT_MROUTEMMI_HOOK_TO_MMI  = PROF_BIT_ONE,
    /** Whether the channel is designated as MMI Unsolicited channel */
    PROF_BIT_MROUTEMMI_MMI_UNSOL    = PROF_BIT_TWO,
    END_OF_MROUTEMMI_BITS,

    END_OF_PROF_BITS
} VgProfileBit;

/* Profile Constants */
#define NUMBER_OF_PHONE_BOOKS       (12)

/* Reporting flags */

#define REPORTING_DISABLED          (0)
#define REPORTING_ENABLED           (1)
#define REPORTING_QUERY             (2)
#define REPORTING_EXTENDED_ENABLED  (2) /* For MTK proprietary AT+CR reporting */


/* Command &C     */

#define AMPERC_DCD_SET_ON     (0)       /* DCD line is always ON        */
#define AMPERC_DCD_FOLLOWS    (1)       /* DCD line is ON only in the
                                           presence of data carrier     */

/* Command &D     */

#define AMPERD_NO_ACTION      (0)       /* TA ignores status on DTR     */
#define AMPERD_COMMAND_STATE  (1)       /* Change to command mode with
                                           remaining the connected call */
#define AMPERD_DROP_CALL      (2)       /* Disconnect call, change to
                                           command mode                 */

/* Command &K */

#define AMPERK_FC_NONE        (0)       /* No flow control */
#define AMPERK_FC_RTS_CTS     (3)       /* H/W flow control */
#define AMPERK_FC_XON_XOFF    (4)       /* S/W flow control */

/* Command +CMGF  */

#define PROF_CMGF_TEXT_MODE   (1)       /* Text mode                    */

#define PROF_ENABLE    (1)
#define PROF_DISABLE   (0)

#if defined (ENABLE_AT_TRACE)
#define AT_TRACE_ENABLE  (1)
#define AT_TRACE_DISABLE (0)
#endif

/* Command +CSCS  */

typedef enum VgCscsModeTag
{
  VG_AT_CSCS_GSM = 0,  /* Standard GSM format                         */
  VG_AT_CSCS_HEX,      /* Hexadecimal                                 */
  VG_AT_CSCS_IRA,      /* International reference alphabet            */
  VG_AT_CSCS_PCCP,     /* PC character set Code Page (437)            */
  VG_AT_CSCS_PCDN,     /* PC Danish/Norwegian char set                */
  VG_AT_CSCS_UCS2,     /* 16-bit universal multiple-octet coded chars */
  VG_AT_CSCS_8859_1,   /* ISO 8859 Latin (1) char set                 */
  VG_AT_CSCS_MAX_VAL   /* Number of possible +CSCS state values       */
} VgCSCSMode;

/* Command +CMEE   */

typedef enum VgCmeSettingTag
{
  VG_CME_OFF  = 0,     /* Disable result code                       */
  VG_CME_NUMERIC,      /* Enable result code and use numeric values */
  VG_CME_VERBOSE,      /* Enable result code and use verbose values */
  NUM_OF_VG_CME        /* Number of possible ATV state values       */
}
VgCmeSetting;

#ifndef GEN_NVDM_DATABASE_FOR_PC
typedef struct VgLmInfo
{
  const Char             *vgPhoneStore;
  const LmDialNumberFile file;
  const Boolean          readOnly;
  const Boolean          pin2Protected;
  const Boolean          hiddenEntries;
}
VgLmInfo;
#endif

typedef enum VgIcfFormatTag
{
  VG_ICF_FORMAT_AUTO_DETECT,                /* not supported */
  VG_ICF_FORMAT_8_DATA_0_PARITY_2_STOP,
  VG_ICF_FORMAT_8_DATA_1_PARITY_1_STOP,
  VG_ICF_FORMAT_8_DATA_0_PARITY_1_STOP,
  VG_ICF_FORMAT_7_DATA_0_PARITY_2_STOP,
  VG_ICF_FORMAT_7_DATA_1_PARITY_1_STOP,
  VG_ICF_FORMAT_7_DATA_0_PARITY_1_STOP,
  VG_ICF_NUMBER_OF_FORMATS
} VgIcfFormat;

typedef enum VgIcfParityTag
{
  VG_ICF_PARITY_ODD,
  VG_ICF_PARITY_EVEN,
  VG_ICF_PARITY_MARK,
  VG_ICF_PARITY_SPACE,
  VG_ICF_NUMBER_OF_PARITYS
} VgIcfParity;

typedef enum VgConnectResultTag
{
  VG_CONNECT_ONLY,
  VG_CONNECT_TEXT_ONLY,
  VG_CONNECT_TEXT_WITH_DIAL_TONE_DETECTION,
  VG_CONNECT_TEXT_WITH_BUSY_DETECTION,
  VG_CONNECT_TEXT_WITH_DIAL_TONE_AND_BUSY_DETECTION
} VgConnectResult;

/* Command +CGPIAF  */
#define PROF_CGPIAF_DISABLE               (0)
#define PROF_CGPIAF_ENABLE                (1)

#define PROF_CGPIAF_IPV6_ADDR_FORMAT      (0)
#define PROF_CGPIAF_IPV6_SUBNET_NOTATION  (1)
#define PROF_CGPIAF_IPV6_LEADING_ZEROS    (2)
#define PROF_CGPIAF_IPV6_COMPRESS_ZEROS   (3)

#define PROF_CEN_DISABLE                  (0)
#define PROF_CEN_ENABLE                   (1)

#define PROF_MPLMNAPNURI_DISABLE          (0)
#define PROF_MPLMNAPNURI_ENABLE           (1)

#define PROF_MPDI_DISABLE                 (0)
#define PROF_MPDI_ENABLE                  (1)

#define PROF_MATWAKEUP_DISABLE            (0)
#define PROF_MATWAKEUP_ENABLE             (1)

#define PROF_MSPCHSC_NEW_SCRAMBLE         (1)   /*New scrambling code*/
#define PROF_MSPCHSC_OLD_SCRAMBLE         (0)   /*old scrambling code*/

/* For NB-IOT */

/*Command *MLTEGCF */
typedef enum VtLteGcfModeTag
{
  GCF_TEST_MODE_DISABLE = 0,
  GCF_TEST_MODE_ENABLE = 1,
  GCF_TEST_CID0_ENABLE = 2
}
VgLteGcfTestMode;

/************************************
 * Modem ID information             *
 ************************************/
#define MAX_MANUFACTURE_ID_LENGTH         (30)
#define MAX_MODEL_ID_LENGTH               (30)
#define MAX_SW_REVISION_LENGTH            (30)
#define MAX_HW_REVISION_LENGTH            (30)
#define MAX_GLOBAL_ID_LENGTH              (30)

/* configurable identification command responses */
typedef struct AtIdentificationTextTag
{
  Char manufacturerId[MAX_MANUFACTURE_ID_LENGTH];
  Char modelId       [MAX_MODEL_ID_LENGTH];
  Char swRevision    [MAX_SW_REVISION_LENGTH];
  Char hwRevision    [MAX_HW_REVISION_LENGTH];
  Char globalId      [MAX_GLOBAL_ID_LENGTH];  
} AtIdentificationText;

/***************************************************************
 *              Profile API
 * *************************************************************/
#ifndef GEN_NVDM_DATABASE_FOR_PC
ResultCode_t setProfileValue (const VgmuxChannelNumber entity,
                               const Int8 profId,
                                const Int8 value);

ResultCode_t setProfileValueBit (const VgmuxChannelNumber entity,
                                  const Int8 profId,
                                   const VgProfileBit profBit,
                                    const Int8 value);

Int8 getProfileValue (const VgmuxChannelNumber entity,
                       const Int8 profId);

Int8 getProfileValueBit (const VgmuxChannelNumber entity,
                          const Int8 profId,
                           const VgProfileBit profBit);

Boolean isProfileValueEqual (const VgmuxChannelNumber entity,
                              const Int8 profId,
                               const Int8 value);

Boolean isProfileValueBitEqual (const VgmuxChannelNumber entity,
                                 const Int8 profId,
                                  const VgProfileBit profBit,
                                   const Int8 value);

ResultCode_t setDefaultSpecificProfile (const VgmuxChannelNumber entity,
                                 const Boolean powerUp );
ResultCode_t setDefaultGenericProfile (const VgmuxChannelNumber entity,
                                 const Boolean powerUp );

ResultCode_t setStoredSpecificProfile (const VgmuxChannelNumber entity,
                                const ProfileContext_t *source_p);
ResultCode_t setStoredGenericProfile (const VgmuxChannelNumber entity,
                                const ProfileGenericContext_t *source_p);

void copySpecificProfile (ProfileContext_t *destination_p,
                   const ProfileContext_t *source_p);

void copyGenericProfile (ProfileGenericContext_t *destination_p,
                   const ProfileGenericContext_t *source_p);

ResultCode_t setCresSpecificProfile (const VgmuxChannelNumber entity,
                              const ProfileContext_t *source_p);
ResultCode_t setCresGenericProfile (const VgmuxChannelNumber entity,
                              const ProfileGenericContext_t *source_p);

Boolean isSpecificProfileCodeWordValid (const ProfileContext_t *profile_p);
Boolean isGenericProfileCodeWordValid (const ProfileGenericContext_t *profile_p);

const Char              *getCscsString      (VgCSCSMode cscsIndex);
const VgLmInfo          *getVgLmInfoRec     (void);

void getChannelConfOptions (const VgmuxChannelNumber entity, ChannelConfOptions* confOptions_p);
void getComPortSettings    (const VgmuxChannelNumber entity, ComPortSettings*    comPortSettings_p);
#endif
#endif  /* RVPROF_H */
