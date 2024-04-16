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
 *  File Description :
 *
 *  types for sim emulator
 **************************************************************************/

#if !defined (USIMEMU_H)
#define USIMEMU_H

#if defined(FEA_TEMP_INCLUDE_SIM_EMULATION) || defined (FEA_TEMP_ESIM_EMULATION)
/***************************************************************************
 * Nested Include Files
 **************************************************************************/
#if !defined (SYSTEM_H)
#include "system.h"
#endif

#if !defined(SIMDATA_H)
#include "simdata.h"
#endif

#if !defined(L1SI_SIG_H)
#include "l1si_sig.h"
#endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/* Number of records for LF and C type files      */
/* All of these values may change from SIM to SIM */

#if defined (MULTI_USIM_APPLICATIONS)
#define SIM_EMU_NUM_DIR_RECS           3
#else
#define SIM_EMU_NUM_DIR_RECS           2
#endif

#define SIM_MAX_DATA_SIZE             255

#define SIM_EMU_NUM_USIM_ARR_RECS      6

#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_MBMS)
# if defined (UPGRADE_3G_TDD128)
#define SIM_EMU_NUM_USIM_MSK_RECS              2
#define SIM_EMU_NUM_USIM_MUK_RECS              2
#define SIM_EMU_NUM_USIM_GBANL_RECS            2
#  endif /* UPGRADE_3G_TDD128 */
# endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G */

#if defined (USIM_EMU_USE_EXTENDED_ECC_CODES)
/* testing for extended ECC codes */
#define SIM_EMU_NUM_USIM_ECC_RECS     11
#else
#define SIM_EMU_NUM_USIM_ECC_RECS      3
#endif

#define SIM_EMU_NUM_ICI_RECS           1
#define SIM_EMU_NUM_OCI_RECS           1
#define SIM_EMU_NUM_USIM_BDN_RECS      20
#define SIM_EMU_NUM_USIM_ACM_RECS      2
#define SIM_EMU_NUM_USIM_FDN_RECS      3
#define SIM_EMU_NUM_USIM_SMS_RECS      5
#define SIM_EMU_NUM_USIM_MMSN_RECS     3
#define SIM_EMU_NUM_USIM_MMSUP_RECS    2
#define SIM_EMU_NUM_USIM_SMSR_RECS     10   /*job 100781*/
#define SIM_EMU_NUM_USIM_SMSP_RECS     5


#define SIM_EMU_NUM_IMG_RECS           5


#define SIM_EMU_NUM_USIM_ICI_RECS      7
#define SIM_EMU_NUM_USIM_OCI_RECS      6
#define SIM_EMU_NUM_USIM_MSISDN_RECS   3
#define SIM_EMU_NUM_USIM_EXT3_RECS     12
#define SIM_EMU_NUM_USIM_EXT4_RECS     12
#define SIM_EMU_NUM_USIM_EXT5_RECS     12
#define SIM_EMU_NUM_USIM_EXT6_RECS     10
#define SIM_EMU_NUM_USIM_EXT7_RECS     12
#define SIM_EMU_NUM_USIM_EXT8_RECS     4
#define SIM_EMU_NUM_USIM_CFIS_RECS     2
#define SIM_EMU_NUM_USIM_MBDN_RECS     4
#define SIM_EMU_NUM_USIM_MWIS_RECS     2
#define SIM_EMU_NUM_USIM_MBI_RECS      2
#define SIM_EMU_NUM_USIM_CCP2_RECS     4
#define SIM_EMU_NUM_USIM_PNN_RECS      12
#define SIM_EMU_NUM_USIM_OPL_RECS      15
#define SIM_EMU_NUM_USIM_ORPK_RECS     2
#define SIM_EMU_NUM_USIM_ARPK_RECS     2
#define SIM_EMU_NUM_USIM_TPRPK_RECS    2

/* Size of records for LF and C type files */

#define SIM_EMU_NUM_USIM_EXT2_RECS     11
#define SIM_EMU_NUM_USIM_SDN_RECS      8
#define SIM_EMU_SIZE_USIM_SMSR_RECS    10     /*job 100781*/
#define SIM_EMU_SIZE_USIM_KEYS_FILE    33
#define SIM_EMU_SIZE_USIM_AAS_FILE     12
#define SIM_EMU_SIZE_USIM_CCP2_FILE    15
#define SIM_EMU_SIZE_USIM_OPL_FILE     8
#define SIM_EMU_SIZE_USIM_PNN_FILE     32
#define SIM_EMU_SIZE_USIM_ARR_FILE      37      /* Variable    */
#define SIM_EMU_SIZE_USIM_ECC_FILE      10
#define SIM_EMU_SIZE_USIM_BDN_FILE      46
#define SIM_EMU_SIZE_USIM_FDN_FILE      46
#define SIM_EMU_SIZE_USIM_SMSS_FILE      2
#define SIM_EMU_SIZE_USIM_SMS_FILE      176
#define SIM_EMU_SIZE_USIM_MMSN_FILE     52
#define SIM_EMU_SIZE_USIM_MMSUP_FILE     64
#define SIM_EMU_SIZE_USIM_SMSP_FILE     31
#define SIM_EMU_SIZE_USIM_ICI_FILE         59
#define SIM_EMU_SIZE_USIM_OCI_FILE         58
#define SIM_EMU_SIZE_USIM_OCT_FILE         3
#define SIM_EMU_SIZE_USIM_ICT_FILE         3
#define SIM_EMU_SIZE_USIM_MSISDN_FILE      46
#define SIM_EMU_SIZE_USIM_EXT3_FILE        13
#define SIM_EMU_SIZE_USIM_EXT4_FILE        13
#define SIM_EMU_SIZE_USIM_EXT5_FILE        13
#define SIM_EMU_SIZE_USIM_EXT6_FILE        13
#define SIM_EMU_SIZE_USIM_EXT7_FILE        13
#define SIM_EMU_SIZE_USIM_EXT8_FILE        23
#define SIM_EMU_SIZE_USIM_CFIS_FILE        16
#define SIM_EMU_SIZE_USIM_MBDN_FILE        45
#define SIM_EMU_SIZE_USIM_GID1_FILE        20
#define SIM_EMU_SIZE_USIM_GID2_FILE        20
#define SIM_EMU_SIZE_USIM_EXT1_FILE        13
#define SIM_EMU_SIZE_USIM_EXT2_FILE        13
#define SIM_EMU_SIZE_USIM_SDN_FILE         46
/* CPHS specific - for Mailbox Number file */
#define SIM_EMU_SIZE_CPHS_MN_RECS       46      /* Variable as per ADN */
#define SIM_EMU_SIZE_CPHS_INFO_NUM_RECS 35      /* Variable */


/* Size of T type files */
#define SIM_EMU_SIZE_ICCID_FILE        10
#define SIM_EMU_SIZE_LP_FILE            5
#define SIM_EMU_SIZE_KC_FILE            9
#define SIM_EMU_SIZE_TST_FILE           8
#define SIM_EMU_SIZE_ACTING_HPLMN_FILE  3
#define SIM_EMU_SIZE_CSP_FILE           16
#define SIM_EMU_SIZE_GID1_FILE         20
#define SIM_EMU_SIZE_GID2_FILE         20
#define SIM_EMU_SIZE_USIM_SPDI_FILE         19

/* CPHS specific - for CPHS_INFO, ON, ONS, CFF and VMWF */
#define SIM_EMU_SIZE_CPHS_INFO_FILE     3      /* Fixed */
#define SIM_EMU_SIZE_CPHS_ON_FILE      20      /* Variable */
#define SIM_EMU_SIZE_CPHS_ONS_FILE     10      /* Fixed */
#define SIM_EMU_SIZE_CPHS_CFF_FILE      2      /* Variable */
#define SIM_EMU_SIZE_CPHS_VMWF_FILE     2      /* Variable */
#define SIM_EMU_SIZE_CPHS_CSP_FILE     16      /* Variable */
#define SIM_EMU_SIZE_KC_GPRS_FILE      9    /* F */
#define SIM_EMU_SIZE_LOCI_GPRS_FILE    14   /* F */
#define SIM_EMU_SIZE_PL_FILE                10
#define SIM_EMU_SIZE_USIM_LI_FILE           22

#define SIM_EMU_SIZE_DIR_FILE               38
#define SIM_EMU_SIZE_IMG_FILE               19
#if defined (UPGRADE_REL8_USIM)
#define SIM_EMU_SIZE_USIM_UST_FILE          12
#else
# if defined (ENABLE_EHPLMN)||defined (UPGRADE_3G_MBMS)
 #define SIM_EMU_SIZE_USIM_UST_FILE         10
# else
 #define SIM_EMU_SIZE_USIM_UST_FILE          7
# endif
#endif /* !UPGRADE_REL8_USIM */
#define SIM_EMU_SIZE_USIM_EST_FILE          1
#define SIM_EMU_SIZE_USIM_KEYS_FILE         33
#define SIM_EMU_SIZE_USIM_FPLMN_FILE        12
#define SIM_EMU_SIZE_USIM_CNL_FILE          24
#define SIM_EMU_SIZE_USER_PLMNWACT_FILE     60
#define SIM_EMU_SIZE_OPERATOR_PLMNWACT_FILE 40
#define SIM_EMU_SIZE_HPLMNWACT_FILE         40
#define SIM_EMU_SIZE_USIM_DCK_FILE          16
#define SIM_EMU_SIZE_USIM_IMSI_FILE         9
#define SIM_EMU_SIZE_USIM_ACC_FILE          2
#define SIM_EMU_SIZE_USIM_LOCI_FILE         11
#define SIM_EMU_SIZE_USIM_HPLMN_FILE        1
#define SIM_EMU_SIZE_USIM_CBMID_FILE        10
#define SIM_EMU_SIZE_USIM_HPLMN_FILE        1
#if defined (SIM_EMU_DISABLE_MNC_LENGTH)
#define SIM_EMU_SIZE_USIM_AD_FILE           3
#else
#define SIM_EMU_SIZE_USIM_AD_FILE           4
#endif
#define SIM_EMU_SIZE_USIM_ACM_FILE          3
#define SIM_EMU_SIZE_USIM_ACM_MAX_FILE      3
#define SIM_EMU_SIZE_USIM_HPLMN_FILE        1
#define SIM_EMU_SIZE_USIM_PUCT_FILE         5
#define SIM_EMU_SIZE_USIM_EST_FILE          1
#define SIM_EMU_SIZE_USIM_DCK_FILE         16
#define SIM_EMU_SIZE_USIM_CBMID_FILE       10
#define SIM_EMU_SIZE_USIM_CBMI_FILE        10


#define SIM_EMU_SIZE_USIM_SPN_FILE         17
#define SIM_EMU_SIZE_USIM_ICI_FILE         59
#define SIM_EMU_SIZE_USIM_OCI_FILE         58
#define SIM_EMU_SIZE_USIM_OCT_FILE         3
#define SIM_EMU_SIZE_USIM_ICT_FILE         3
#define SIM_EMU_SIZE_USIM_MSISDN_FILE      46
#define SIM_EMU_SIZE_USIM_EXT3_FILE        13
#define SIM_EMU_SIZE_USIM_EXT4_FILE        13
#define SIM_EMU_SIZE_USIM_EXT5_FILE        13
#define SIM_EMU_SIZE_USIM_EXT6_FILE        13
#define SIM_EMU_SIZE_USIM_EXT7_FILE        13
#define SIM_EMU_SIZE_USIM_MBDN_FILE        45
#define SIM_EMU_SIZE_USIM_MWIS_FILE        5
#define SIM_EMU_SIZE_USIM_MBI_FILE         4
#define SIM_EMU_SIZE_USIM_GID1_FILE        20
#define SIM_EMU_SIZE_USIM_GID2_FILE        20
#define SIM_EMU_SIZE_USIM_EXT1_FILE        13
#define SIM_EMU_SIZE_USIM_EXT2_FILE        13
#define SIM_EMU_SIZE_USIM_SDN_FILE         46
#define SIM_EMU_SIZE_USIM_KC_FILE          9
#define SIM_EMU_SIZE_USIM_LOCI_PS_FILE     14
#define SIM_EMU_SIZE_USIM_THRESHOLD_FILE   3
#define SIM_EMU_SIZE_USIM_NETPAR_FILE      74
#if defined  (SIM_EMU_BIG_ACL_FILE)
#define SIM_EMU_SIZE_USIM_ACL_FILE         187
#else
#define SIM_EMU_SIZE_USIM_ACL_FILE         64
#endif
#define SIM_EMU_SIZE_USIM_START_HFN_FILE    6

#define SIM_EMU_SIZE_USIM_SMSR_FILE        30       /*job 100781*/
#define SIM_EMU_SIZE_USIM_HIDDEN_KEY_FILE   4
#define SIM_EMU_SIZE_USIM_EHPLMN_FILE      12
#define SIM_EMU_SIZE_USIM_EHPLMNPI_FILE     1

#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_MBMS)
# if defined (UPGRADE_3G_TDD128)
#define SIM_EMU_SIZE_GBABP_FILE            40
#define SIM_EMU_SIZE_MSK_FILE              30
#define SIM_EMU_SIZE_MUK_FILE              30
#define SIM_EMU_SIZE_GBANL_FILE            20
#  endif /* UPGRADE_3G_TDD128 */
# endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G */

#if defined (UPGRADE_REL8_USIM)
#define SIM_EMU_SIZE_EPS_LOCI_FILE      18
#define SIM_EMU_SIZE_NAS_SECURITY_FILE  54
#define SIM_EMU_NUM_NAS_SECURITY_RECS    1
#endif

#define SIM_EMU_SIZE_NAS_CONFIG_FILE   39 /*(13*3)*/

#define SIM_EMU_SIZE_NCPIP_FILE        39
#define SIM_EMU_NUM_NCPIP_RECS         1

#define CHV_ATTEMPTS                    3
#define CHV_UNBLOCK_ATTEMPTS           10

#define PIN_1_VALUE          "1111"
#define PIN_2_VALUE          "2222"    /* PIN2 = 1 1 1 1*/

#define UNBLOCK_PIN_1_VALUE          "11111111"
#define UNBLOCK_PIN_2_VALUE          "22222222"

#define UNIVERSAL_PIN_VALUE          "1234"
#define UNBLOCK_UNIVERSAL_PIN_VALUE  "12345678"

/*
** SIM command codes
*/
#define SIM_CMD_SELECT               0xa4
#define SIM_CMD_STATUS               0xf2
#define SIM_CMD_READ_BINARY          0xb0
#define SIM_CMD_UPDATE_BINARY        0xd6
#define SIM_CMD_READ_RECORD          0xb2
#define SIM_CMD_UPDATE_RECORD        0xdc
#define SIM_CMD_SEEK                 0xa2
#define SIM_CMD_INCREASE             0x32
#define SIM_CMD_VERIFY_PIN           0x20
#define SIM_CMD_CHANGE_PIN           0x24
#define SIM_CMD_DISABLE_PIN          0x26
#define SIM_CMD_ENABLE_PIN           0x28
#define SIM_CMD_UNBLOCK_PIN          0x2c
#define SIM_CMD_DEACTIVATE           0x04
#define SIM_CMD_ACTIVATE             0x44
#define SIM_CMD_AUTHENTICATE         0x88
#define SIM_CMD_GET_RESPONSE         0xc0
#define SIM_CMD_TERMINAL_PROFILE     0x10
#define SIM_CMD_ENVELOPE             0xC2
#define SIM_CMD_FETCH                0x12
#define SIM_CMD_TERMINAL_RESPONSE    0x14
#define SIM_CMD_MANAGE_CHANNEL       0x70
#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_MBMS)
# if defined (UPGRADE_3G_TDD128)
#define SIM_COMM_ODD_INS_CODE        0x89
#  endif /* UPGRADE_3G_TDD128 */
# endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G */


/***************************************************************************
 * Type Definitions
 **************************************************************************/
 typedef struct SiCmdMsgTag
{
    Int8     class;
    Int8     instruction;
    Int8     p1;
    Int8     p2;
    Int8     lc;
    Int8     data[SIM_MAX_DATA_SIZE];
    Int8     le;
    Boolean  lePresent;
    Boolean  lcPresent;
}
SiCmdMsg;

#if !defined (FEA_TEMP_ESIM_EMULATION)
typedef enum SimUiccFileIdTag
{
    SIM_UICC_DIR_ID =0,
    SIM_UICC_ECC_ID,
    SIM_UICC_PL_ID,
    SIM_UICC_LI_ID,
    SIM_UICC_IMSI_ID,
    SIM_UICC_ACC_ID,
    SIM_UICC_HPLMN_ID,
    SIM_UICC_LOCI_ID,
    SIM_UICC_FPLMN_ID,
    SIM_UICC_CNL_ID,
    SIM_UICC_CBMID_ID,
    SIM_UICC_DCK_ID,
    SIM_UICC_BDN_ID,
    SIM_UICC_IMG_ID,
    SIM_UICC_IMGINST1_ID,
    SIM_UICC_IMGINST2_ID,
    SIM_UICC_IMGINST3_ID,
    SIM_UICC_IMGINST4_ID,
    SIM_UICC_IMGINST5_ID,
    SIM_UICC_ICCID_ID,
    SIM_UICC_CSP_ID,
    SIM_UICC_TST_ID,
    SIM_UICC_ACTING_HPLMN_ID,
    SIM_UICC_AD_ID,
    SIM_UICC_ACM_ID,
    SIM_UICC_PUCT_ID,
    SIM_UICC_UST_ID,
    SIM_UICC_EST_ID,
    SIM_UICC_ACM_MAX_ID,
    SIM_UICC_FDN_ID,
    SIM_UICC_CBMI_ID,
    SIM_UICC_SMSR_ID,      /*job 100781*/
    SIM_UICC_SMSS_ID,
    SIM_UICC_SMS_ID,
    SIM_UICC_MMSN_ID,
    SIM_UICC_MMSUP_ID,
    SIM_UICC_MMSUCP_ID,
    SIM_UICC_MMSICP_ID,
    SIM_UICC_SMSP_ID,
    SIM_UICC_SPN_ID,
    SIM_UICC_OCI_ID,
    SIM_UICC_ICI_ID,
    SIM_UICC_ICT_ID,
    SIM_UICC_OCT_ID,
    SIM_UICC_MSISDN_ID,
    SIM_UICC_EXT3_ID,
    SIM_UICC_EXT4_ID,
    SIM_UICC_EXT5_ID,
    SIM_UICC_EXT6_ID,
    SIM_UICC_EXT7_ID,
    SIM_UICC_EXT8_ID,
    SIM_UICC_CFIS_ID,
    SIM_UICC_MBDN_ID,
    SIM_UICC_MWIS_ID,
    SIM_UICC_MBI_ID,
    SIM_UICC_GID1_ID,
    SIM_UICC_GID2_ID,
    SIM_UICC_EXT2_ID,
    SIM_UICC_SDN_ID,
    SIM_UICC_KC_ID,
    SIM_UICC_KC_GPRS_ID,
    SIM_UICC_KEYS_ID,
    SIM_UICC_KEYS_PS_ID,
    SIM_UICC_LOCI_PS_ID,
    SIM_UICC_START_HFN_ID,
    SIM_UICC_THRESHOLD_ID,
    SIM_UICC_NETPAR_ID,
    SIM_UICC_ACL_ID,
    SIM_UICC_USER_PLMN_SEL_ID,
    SIM_UICC_OPL_ID,
    SIM_UICC_PNN_ID,
    SIM_UICC_OPERATOR_PLMN_SEL_ID,
    SIM_UICC_HPLMN_SEL_ID,
    SIM_UICC_ARR_ID,
    SIM_UICC_HIDDEN_KEY_ID,
    SIM_UICC_CCP2_ID,
    SIM_UICC_SPDI_ID,
    SIM_UICC_EHPLMN_ID,
    SIM_UICC_EHPLMNPI_ID,
#if defined (UPGRADE_3G_MBMS)
# if defined (UPGRADE_3G_TDD128)
    SIM_UICC_GBABP_ID,
    SIM_UICC_MSK_ID,
    SIM_UICC_MUK_ID,
    SIM_UICC_GBANL_ID,
# endif
#endif
#if defined (UPGRADE_REL8_USIM)
    SIM_UICC_NAS_SECURITY_ID,
    SIM_UICC_EPS_LOCI_ID,
#endif
    SIM_UICC_NAS_CONFIG_ID,
    SIM_UICC_NCPIP_ID,
/*LOCAL PHONEBOOK files*/
    SIM_UICC_EXT1_ID,
    SIM_UICC_PBC_ID,
    SIM_UICC_PBC1_ID,
    SIM_UICC_AAS_ID,
    SIM_UICC_PBR_ID,
    SIM_UICC_ADN_ID,
    SIM_UICC_IAP_ID,
    SIM_UICC_IAP1_ID,
    SIM_UICC_ADN1_ID,
    SIM_UICC_CCP1_ID,
    SIM_UICC_GRP_ID,
    SIM_UICC_GRP1_ID,
    SIM_UICC_PSC_ID,
    SIM_UICC_PUID_ID,
    SIM_UICC_CC_ID,
    SIM_UICC_UID_ID,
    SIM_UICC_UID1_ID,
    SIM_UICC_EMAILA_ID,
    SIM_UICC_EMAILB_ID,
    SIM_UICC_EMAILA1_ID,
    SIM_UICC_EMAILB1_ID,
    SIM_UICC_GAS_ID,
    SIM_UICC_SNE_ID,
    SIM_UICC_SNE1_ID,
    SIM_UICC_ANRA_ID,
    SIM_UICC_ANRB_ID,
    SIM_UICC_ANRC_ID,
    SIM_UICC_ANRA1_ID,
    SIM_UICC_ANRB1_ID,
    SIM_UICC_ANRC1_ID,

/*GLOBAL PHONEBOOK files*/
    SIM_UICC_GB_EXT1_ID,
    SIM_UICC_GB_PBC_ID,
    SIM_UICC_GB_PBC1_ID,
    SIM_UICC_GB_AAS_ID,
    SIM_UICC_GB_PBR_ID,
    SIM_UICC_GB_ADN_ID,
    SIM_UICC_GB_ADN1_ID,
    SIM_UICC_GB_CCP1_ID,
    SIM_UICC_GB_GRP_ID,
    SIM_UICC_GB_GRP1_ID,
    SIM_UICC_GB_PSC_ID,
    SIM_UICC_GB_PUID_ID,
    SIM_UICC_GB_CC_ID,
    SIM_UICC_GB_UID_ID,
    SIM_UICC_GB_UID1_ID,
    SIM_UICC_GB_EMAILA_ID,
    SIM_UICC_GB_EMAILB_ID,
    SIM_UICC_GB_EMAILA1_ID,
    SIM_UICC_GB_EMAILB1_ID,
    SIM_UICC_GB_GAS_ID,
    SIM_UICC_GB_SNE_ID,
    SIM_UICC_GB_SNE1_ID,
    SIM_UICC_GB_ANRA_ID,
    SIM_UICC_GB_ANRB_ID,
    SIM_UICC_GB_ANRC_ID,
    SIM_UICC_GB_ANRA1_ID,
    SIM_UICC_GB_ANRB1_ID,
    SIM_UICC_GB_ANRC1_ID,

    NUM_EMU_EF_FILES
}
SimUiccFileId;
#else
typedef enum SimUiccFileIdTag
{
    SIM_UICC_DIR_ID =0,
    SIM_UICC_IMSI_ID,
    SIM_UICC_ACC_ID,
    SIM_UICC_HPLMN_ID,
    SIM_UICC_FPLMN_ID,
    SIM_UICC_ICCID_ID,
    SIM_UICC_AD_ID,
    SIM_UICC_UST_ID,
    SIM_UICC_USER_PLMN_SEL_ID,
    SIM_UICC_OPL_ID,
    SIM_UICC_OPERATOR_PLMN_SEL_ID,
    SIM_UICC_HPLMN_SEL_ID,
    SIM_UICC_EHPLMN_ID,
    SIM_UICC_NAS_SECURITY_ID,
    SIM_UICC_EPS_LOCI_ID,
    SIM_UICC_NAS_CONFIG_ID,
    NUM_EMU_EF_FILES
}
SimUiccFileId;
#endif

typedef enum SimEmuEfIdTag
{
    SIM_EMU_EF_DIR  = 0x2f00,
    SIM_EMU_EF_ECC  = 0x6fb7,
    SIM_EMU_EF_PL   = 0x2f05,
    SIM_EMU_EF_LI   = 0x6f05,
    SIM_EMU_EF_IMSI  = 0x6f07,
    SIM_EMU_EF_ACC  = 0x6f78,
    SIM_EMU_EF_HPLMN  = 0x6f31,
    SIM_EMU_EF_LOCI  = 0x6f7e,
    SIM_EMU_EF_FPLMN  = 0x6f7b,
    SIM_EMU_EF_CNL   = 0x6f32,
    SIM_EMU_EF_CBMID  = 0x6f48,
    SIM_EMU_EF_DCK  = 0x6f2c,
    SIM_EMU_EF_BDN  = 0x6f4d,
    SIM_EMU_EF_IMG  = 0x4f20,
    SIM_EMU_EF_IMGINST1 = 0x4f01,
    SIM_EMU_EF_IMGINST2 = 0x4f02,
    SIM_EMU_EF_IMGINST3 = 0x4f03,
    SIM_EMU_EF_IMGINST4 = 0x4f04,
    SIM_EMU_EF_IMGINST5 = 0x4f05,
    SIM_EMU_EF_ICCID  = 0x2fe2,
    SIM_EMU_EF_TST = 0x6fd2,
    SIM_EMU_EF_CSP = 0x6f15,
    SIM_EMU_EF_AD  = 0x6fad,
    SIM_EMU_EF_ACM  = 0x6f39,
    SIM_EMU_EF_PUCT  = 0x6f41,
    SIM_EMU_EF_UST  = 0x6f38,
    SIM_EMU_EF_EST  = 0x6f56,
    SIM_EMU_EF_ACM_MAX  = 0x6f37,
    SIM_EMU_EF_FDN  = 0x6f3b,
    SIM_EMU_EF_CBMI  = 0x6f45,
    SIM_EMU_EF_SMSS  = 0x6f43,
    SIM_EMU_EF_SMSR  = 0x6f47,
    SIM_EMU_EF_SMS  = 0x6f3c,
    SIM_EMU_EF_MMSN = 0x6fce,
    SIM_EMU_EF_SMSP = 0x6f42,
    SIM_EMU_EF_PBR  = 0x4f30,
    SIM_EMU_EF_ACT_HPLMN = 0x4f34,
    SIM_EMU_EF_ADN  = 0x4f3a,
    SIM_EMU_EF_ADN1 = 0x4f3b,
    SIM_EMU_EF_IAP  = 0x4f97,
    SIM_EMU_EF_IAP1 = 0x4f98,
    SIM_EMU_EF_CCP1 = 0x4f99,
    SIM_EMU_EF_GRP  = 0x4f83,
    SIM_EMU_EF_GRP1 = 0x4f84,
    SIM_EMU_EF_PSC  = 0x4f22,
    SIM_EMU_EF_CC   = 0x4f23,
    SIM_EMU_EF_PUID = 0x4f24,
    SIM_EMU_EF_UID  = 0x4f81,
    SIM_EMU_EF_UID1 = 0x4f82,
    SIM_EMU_EF_EMAILA = 0x4f50,
    SIM_EMU_EF_EMAILB = 0x4f52,
    SIM_EMU_EF_EMAILA1 = 0x4f51,
    SIM_EMU_EF_EMAILB1 = 0x4f53,
    SIM_EMU_EF_GAS = 0x4f4c,
    SIM_EMU_EF_SNE  = 0x4f19,
    SIM_EMU_EF_SNE1  = 0x4f1a,
    SIM_EMU_EF_ANRA = 0x4f11,
    SIM_EMU_EF_ANRB = 0x4f13,
    SIM_EMU_EF_ANRC = 0x4f15,
    SIM_EMU_EF_ANRA1 = 0x4f12,
    SIM_EMU_EF_ANRB1 = 0x4f14,
    SIM_EMU_EF_ANRC1 = 0x4f16,
    SIM_EMU_EF_PBC   = 0x4f09,
    SIM_EMU_EF_PBC1  = 0x4f0A,
    SIM_EMU_EF_SPN = 0x6f46,
    SIM_EMU_EF_OCI  = 0x6f81,
    SIM_EMU_EF_ICI  = 0x6f80,
    SIM_EMU_EF_ICT  = 0x6f82,
    SIM_EMU_EF_OCT  = 0x6f83,
    SIM_EMU_EF_MSISDN  = 0x6f40,
    SIM_EMU_EF_EXT3  = 0x6f4c,
    SIM_EMU_EF_EXT4  = 0x6f55,
    SIM_EMU_EF_EXT5  = 0x6f4e,
    SIM_EMU_EF_EXT6 = 0x6fc8,
    SIM_EMU_EF_CFIS = 0x6fcb,
    SIM_EMU_EF_EXT7 = 0x6fcc,
    SIM_EMU_EF_EXT8 = 0x6fcf,
    SIM_EMU_EF_MBDN = 0x6fc7,
    SIM_EMU_EF_MWIS = 0x6fca,
    SIM_EMU_EF_MBI = 0x6fc9,
    SIM_EMU_EF_GID1 = 0x6f3e,
    SIM_EMU_EF_GID2 =0x6f3f,
    SIM_EMU_EF_EXT1 =0x4f4a,
    SIM_EMU_EF_EXT2 =0x6f4b,
    SIM_EMU_EF_SDN = 0x6f49,
    SIM_EMU_EF_KC =0x4f20,
    SIM_EMU_EF_KC_GPRS =0x4f52,
    SIM_EMU_EF_KEYS =0x6f08,
    SIM_EMU_EF_KEYS_PS =0x6f09,
    SIM_EMU_EF_PS_LOCI =0x6f73,
    SIM_EMU_EF_THRESHOLD = 0x6f5c,
    SIM_EMU_EF_NETPAR  = 0x6fc4,
    SIM_EMU_EF_ACL = 0x6f57,
    SIM_EMU_EF_START_HFN = 0x6f5b,
    SIM_EMU_EF_USER_PLMN_SEL = 0x6f30,
    SIM_EMU_EF_OPLMNW_ACT = 0x6f61,
    SIM_EMU_EF_HPLMNW_ACT = 0x6f62,
    SIM_EMU_EF_PLMNW_ACT = 0x6F60,
    SIM_EMU_EF_ARR       = 0x6f06,
    SIM_EMU_EF_AAS       = 0x4f4b,
    SIM_EMU_EF_HIDDEN_KEY = 0x6fc3,
    SIM_EMU_EF_CCP2      = 0x6f4f,
    SIM_EMU_EF_PNN        = 0x6fc5,
    SIM_EMU_EF_OPL        = 0x6fc6,
    SIM_EMU_EF_SPDI       = 0x6fcd,
    SIM_EMU_EF_MMSUP      = 0x6fd1,
    SIM_EMU_EF_MMSUCP     = 0x6fd2,
    SIM_EMU_EF_MMSICP     = 0x6fd0,
    SIM_EMU_EF_EHPLMN     = 0x6fd9,
    SIM_EMU_EF_EHPLMNPI   = 0x6fdb,
# if defined (UPGRADE_3G_MBMS)
# if defined (UPGRADE_3G_TDD128)
    SIM_EMU_EF_GBABP      = 0x6fd6,
    SIM_EMU_EF_MSK        = 0x6fd7,
    SIM_EMU_EF_MUK        = 0x6fd8,
    SIM_EMU_EF_GBANL      = 0x6fda,
# endif
#endif
# if defined (UPGRADE_REL8_USIM)
    SIM_EMU_EF_EPS_LOCI   = 0x6fe3,
    SIM_EMU_NAS_SECURITY  = 0x6fe4,
# endif
    SIM_EMU_EF_NAS_CONFIG = 0x6fe8,
    SIM_EMU_EF_NCPIP      = 0x6fe2,
    SIM_EMU_EF_INVALID = 0

}
SimEmuEfId;


/*-----------------12/11/01 10:34-------------------------------------------------------
 * This is used to define access conditions for a Dir file, see ISO 7816-9 for details
 * -------------------------------------------------------------------------------------*/
typedef struct SimUiccDirCompactFormatTag
{
    SimUiccAccessCondition    deleteSelf;   /* delete self */
    SimUiccAccessCondition    terminate;
    SimUiccAccessCondition    activate;
    SimUiccAccessCondition    deactivate;
    SimUiccAccessCondition    createEf;
    SimUiccAccessCondition    createDf;
    SimUiccAccessCondition    deleteChild;  /* delete child file */
} SimUiccDirCompactFormat;


typedef struct SimMfDfDataTag
{
    SimDirId                dirId;
    SimDirId                parentId;
    Int16                   totalMemAvailable;
    SimFileType             fileType;
    SimUiccState            fileState;
    SimClockStopMode        clockMode;
    Int8                    appMinClockFreq;
    SimVoltageCapab         simVoltageCapab;
    SimUiccDirCompactFormat accessData;              /* Security attributes for Directories */
    Int8                    appPowerConsumption;
    Int8                    refFrequency;
    SimUiccKeyRefValue      applicationPin;                /*PIN2: local PIN*/
    SimUiccKeyRefValue      localPin;                /*PIN2: local PIN*/
    SimUiccPinStatus        localPinStatus;
    SimUiccPinValue         localPinValue;
    SimUiccPinStatus        unblockLocalPinStatus;
    SimUiccPinValue         unblockLocalPinValue;
    SimUiccAid              aid;                     /* Only used for ADF_USIM */
   }
SimMfDfData;

typedef struct SimEfDataTag
{
    SimEmuEfId              fileId;
    SimEfStructure          fileStructure;
    SimDirId                dirId;
    Int16                   numRecords;  /*If the file is transparent, numRecords = 1*/
    Int16                   recordLength; /*If the file is transparent, recordLength = size of file*/
    Int16                   currentRecord;
    Int16                   cyclicRecOne;
    SimUiccEfCompactFormat  accessData;
    SimUiccState            fileState;
    Boolean                 sfiSupported;
    Int8                    sfi;        /*Short File Indentifier: b1-b5*/
    Boolean                 underUsim;  /*Is the file under USIM dir or not...*/
}
SimEfData;

#if defined (FEA_TEMP_ESIM_EMULATION)
typedef struct SimEmuEfTableTag
{
    SimEfData   *efHeader;
    Int8        *efData;
}
SimEmuEfTable;

typedef struct SimImageTag
{
    SimDirId                     currentDir;
    SimDirId                     dfInUse;
    SimEmuEfId                   currentEf;
    SimEfData                   *currentEfPtr;
    Int8                        *currentEfDataPtr;
    SimMfDfData                 *currentDirPtr;
    SimUiccAid                  *currentAid;
     Int16                       adfUsimId;
    Boolean                      started;

    /*Application Pin */
    SimUiccKeyRefValue           applicationPin;
    SimUiccPinStatus             applicationPinStatus;
    SimUiccPinValue              applicationPinValue;
    SimUiccPinStatus             unblockAppPinStatus;
    SimUiccPinValue              unblockAppPinValue;

    /* Master File (Root Directory) structure */
    SimMfDfData                  mfData;

    /*Security environment */
    SimUiccSecEnvironment        secEnv;
    SimMfDfData                  adfUsimData;

    /* Elementary Files structures */
    /* All files starting with 'u' are located under the USIM directory */
    SimEfData    dirEfData;
    Int8         dirData [SIM_EMU_SIZE_DIR_FILE * SIM_EMU_NUM_DIR_RECS];
    SimEfData    uImsiEfData;
    Int8         uImsiData [SIM_EMU_SIZE_USIM_IMSI_FILE];
    SimEfData    uAccEfData;
    Int8         uAccData [SIM_EMU_SIZE_USIM_ACC_FILE];
    SimEfData    uHplmnEfData;
    Int8         uHplmnData [SIM_EMU_SIZE_USIM_HPLMN_FILE];
    SimEfData    uFplmnEfData;
    Int8         uFplmnData [SIM_EMU_SIZE_USIM_FPLMN_FILE];
    SimEfData    iccidEfData;
    Int8         iccidData [SIM_EMU_SIZE_ICCID_FILE];
    SimEfData    uAdEfData;
    Int8         uAdData [SIM_EMU_SIZE_USIM_AD_FILE];
    SimEfData    uUstEfData;
    Int8         uUstData [SIM_EMU_SIZE_USIM_UST_FILE];
    SimEfData    uUserPlmnSelEfData;
    Int8         uUserPlmnSelData [SIM_EMU_SIZE_USER_PLMNWACT_FILE];
    SimEfData    uOperatorPlmnSelEfData;
    Int8         uOperatorPlmnSelData [SIM_EMU_SIZE_OPERATOR_PLMNWACT_FILE];
    SimEfData    uHplmnSelEfData;
    Int8         uHplmnSelData [SIM_EMU_SIZE_HPLMNWACT_FILE];
    SimEfData    uOplEfData;
    Int8         uOplData [SIM_EMU_SIZE_USIM_OPL_FILE * SIM_EMU_NUM_USIM_OPL_RECS];
    SimEfData    uEhplmnEfData;
    Int8         uEhplmnData [SIM_EMU_SIZE_USIM_EHPLMN_FILE];
    SimEfData    epsLociEfData;
    Int8         epsLociData[SIM_EMU_SIZE_EPS_LOCI_FILE];
    SimEfData    nasSecurityEfData;
    Int8         nasSecurityData[SIM_EMU_SIZE_NAS_SECURITY_FILE * SIM_EMU_NUM_NAS_SECURITY_RECS];
    SimEfData    nasConfigEfData;
    Int8         nasConfigData[SIM_EMU_SIZE_NAS_CONFIG_FILE];
}
SimImage;

typedef struct SiResponseTag
{
    Int8     data[SIM_MAX_DATA_SIZE];
    Boolean  dataIsValid;
    Int8     dataLength;
    Int8     sw1;
    Int8     sw2;
}
SiResponse;
#endif

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
void SimEmuHandleL1siResetReq ( SignalBuffer  *signal);
void SimEmuHandleL1siConfigReq (SignalBuffer  *signal);
void SimEmuHandleL1siDataReq (SignalBuffer  *signal,
                                 SignalBuffer  *respSignal);
void SimEmuHandleL1siRejectReq ( SignalBuffer  *signal);

#if defined (ENABLE_DUAL_SIM_SOLUTION)
void SimEmuHandleL1siSelectSimReq (SignalBuffer  *signal);
void SimEmuHandleL1siShutdownReq ( SignalBuffer  *signal);
#endif

Boolean SimEmuHandleNullCardRemoved (TaskId myTaskId);
void SimEmuSetImageFile (void);
void SimEmuSaveImage (void);
void SimEmuCloseImageFile (void);
void SimEmuCloseImageFile (void);
Boolean SimEmuConfigureModifiableEfFile( Int16 fileId,     Int16  dataOffset,
                                         Int16 dataLength, Int8  *data);

#endif /* FEA_TEMP_INCLUDE_SIM_EMULATION */

#endif /* USIMEMU_H */
/* END OF FILE */





