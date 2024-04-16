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
*******************************************************************************/
/***************************************************************************//**
*  Includes
*******************************************************************************/
//Standard headers
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <system.h>
#include <afnv_typ.h>
#include <abpd_sig.h>
#include <errc_typ.h>
#include <dmnvmcfg.h>
#include <alsi_typ.h>
#include "nvdm_modem_default_config.h"
#include "nvdm_modem_editor.h"
//Other modules
#ifdef MTK_NVDM_OTA_SUPPORT
#include "nvdm_verno.h"
#endif
//Own header

/******************************************************************************
*  PRIVATE CONSTANTS
*******************************************************************************/

/******************************************************************************
*  PRIVATE MACROS
*******************************************************************************/


/******************************************************************************
*  PRIVATE DATA TYPES
*******************************************************************************/


/******************************************************************************
*  PRIVATE VARIABLES
*******************************************************************************/
/*CFUN Configure nvram Item Set Default value*/
const MePersonalisationData mepDataInit =
{
    /* mepData
     * personalisationsActivated*/ 0x00,

    /* personalisationsEnabled           */ 0x1F,
    { /* personalisationKeys */
        { /* personalisationKeys[0]
           * length */         0x0A,
                           { /* number
                                        * number[0]    */ 0x31,
                                 /* number[1]    */ 0x35,
                                 /* number[2]    */ 0x39,
                                 /* number[3]    */ 0x38,
                                 /* number[4]    */ 0x37,
                                 /* number[5]    */ 0x35,
                                 /* number[6]    */ 0x33,
                                 /* number[7]    */ 0x39,
                                 /* number[8]    */ 0x37,
                                 /* number[9]    */ 0x31,
                                 /* number[10]   */ 0x00,
                                 /* number[11]   */ 0x00
                           },
        },

        { /* personalisationKeys[1]
           * length */           0x0A,
                             { /* number
                                        * number[0]    */ 0x31,
                                 /* number[1]    */ 0x35,
                                 /* number[2]    */ 0x39,
                                 /* number[3]    */ 0x38,
                                 /* number[4]    */ 0x37,
                                 /* number[5]    */ 0x35,
                                 /* number[6]    */ 0x33,
                                 /* number[7]    */ 0x39,
                                 /* number[8]    */ 0x37,
                                 /* number[9]    */ 0x31,
                                 /* number[10]   */ 0x00,
                                 /* number[11]   */ 0x00
                             },
        },

        { /* personalisationKeys[2]
           * length */           0x0A,
                             { /* number
                                        * number[0]    */ 0x31,
                                 /* number[1]    */ 0x35,
                                 /* number[2]    */ 0x39,
                                 /* number[3]    */ 0x38,
                                 /* number[4]    */ 0x37,
                                 /* number[5]    */ 0x35,
                                 /* number[6]    */ 0x33,
                                 /* number[7]    */ 0x39,
                                 /* number[8]    */ 0x37,
                                 /* number[9]    */ 0x31,
                                 /* number[10]   */ 0x00,
                                 /* number[11]   */ 0x00
                             },
        },

        { /* personalisationKeys[3]
           * length */           0x0A,
                             { /* number
                                        * number[0]    */ 0x31,
                                 /* number[1]    */ 0x35,
                                 /* number[2]    */ 0x39,
                                 /* number[3]    */ 0x38,
                                 /* number[4]    */ 0x37,
                                 /* number[5]    */ 0x35,
                                 /* number[6]    */ 0x33,
                                 /* number[7]    */ 0x39,
                                 /* number[8]    */ 0x37,
                                 /* number[9]    */ 0x31,
                                 /* number[10]   */ 0x00,
                                 /* number[11]   */ 0x00
                             },
        },

        { /* personalisationKeys[4]
           * length*/            0x0A,
                             { /* number
                                        * number[0]    */ 0x31,
                                 /* number[1]    */ 0x35,
                                 /* number[2]    */ 0x39,
                                 /* number[3]    */ 0x38,
                                 /* number[4]    */ 0x37,
                                 /* number[5]    */ 0x35,
                                 /* number[6]    */ 0x33,
                                 /* number[7]    */ 0x39,
                                 /* number[8]    */ 0x37,
                                 /* number[9]    */ 0x31,
                                 /* number[10]   */ 0x00,
                                 /* number[11]   */ 0x00
                             },
        },
    },

    { /* keyRetries
       * keyRetries[0]                       */ 0x0A,
     /* keyRetries[1]                       */ 0x0A,
     /* keyRetries[2]                       */ 0x0A,
     /* keyRetries[3]                       */ 0x0A,
     /* keyRetries[4]                       */ 0x0A
    },

    { /* networkSubsetId
       * networkSubsetId[0]                  */ 0xFF,    /*MSISN: digit 1, digit 2*/
    },

    /* serviceproviderId                    */ 0xFF,

    /* corporateId                          */ 0xFF,
    { /* simId */ /*we now store the raw Imsi as encoded on the SIM card
                   * length*/  0,
                       { /*contents*/
                           0xFF,
                           0xFF,
                           0xFF,
                           0xFF,
                           0xFF,
                           0xFF,
                           0xFF,
                           0xFF,
                       },
    },
    { /* networkIds */
        {
            { /* networkIds[0]
               * mcc */                         0xFFFF,
            /* mnc */                        0xFFFF,
            /* Is MNC 3 digit */               0x00,
            /* access technology */            GSM_ACCESS_TECHNOLOGY
            },

            /* Is three digit encoding used */   0x00,         /* 0 = FALSE, 1 = TRUE */
        },
        {
            { /* networkIds[1]
                  * mcc */                      0xFFFF,
               /* mnc */                     0xFFFF,
               /* Is MNC 3 digit */            0x00,
               /* access technology */         GSM_ACCESS_TECHNOLOGY
            },
           
           /* Is three digit encoding used */    0x00,         /* 0 = FALSE, 1 = TRUE */
        },
        {
            { /* networkIds[2]
               * mcc */                         0xFFFF,
            /* mnc */                        0xFFFF,
            /* Is MNC 3 digit */               0x00,
            /* access technology */            GSM_ACCESS_TECHNOLOGY
            },

            /* s three digit encoding used */    0x00,         /* 0 = FALSE, 1 = TRUE */
        }
    },
};


/* Customized default APN info valid flag:
 * TRUE - content of customizedDefaultApnInfo is valid and will be stored in NVRAM;
 * FALSE - otherwise
 */
const Boolean isCustomizedDefaultApnInfoValid = FALSE;

/*
 * Customized default settings for APN, username and password used for
 * activation of PDN connection during attached procedure.
 *
 * Note that the APN name that the network uses is of the form:
 * "3wap2o22co2uk" (refer to 3GPP TS 23.003, 9.1), and the textual
 * equivalent (textualApn in this case) is "wap.o2.co.uk"
 * Note that this a default setting and should be overwritten with
 * valid settings for the target network.
 */
const AbpdApn customizedDefaultApnInfo = /* NOTE: Valid only if isCustomizedDefaultApnInfoValid is set to TRUE */
{
    /* apnPresent */ TRUE,
    { /* apn */
        /* length */ 9,
        {  /* name
            * name [0] */       8,
           /* name [1] */       'i',
           /* name [2] */       'n',
           /* name [3] */       't',
           /* name [4] */       'e',
           /* name [5] */       'r',
           /* name [6] */       'n',
           /* name [7] */       'e',
           /* name [8] */       't'
        }
    },
    { /* textualApn */
        /* length */ 8,
        {  /* name
            * name [0] */       'i',
           /* name [1] */       'n',
           /* name [2] */       't',
           /* name [3] */       'e',
           /* name [4] */       'r',
           /* name [5] */       'n',
           /* name [6] */       'e',
           /* name [7] */       't'
        }
    },
    /* pdnType */ PDN_TYPE_IPV4V6,
    { /* psdUser */
        /* usernamePresent */ TRUE,
        /* usernameLength */  3,
        {  /* username
            * username [0]  */  'w',
           /* username [1] */   'e',
           /* username [2] */   'b'
        },
        /* passwdPresent */ TRUE,
        /* passwdLength */  8,
        {  /* passwd
            * passwd [0]  */    'p',
           /* passwd [1]  */    'a',
           /* passwd [2]  */    's',
           /* passwd [3]  */    's',
           /* passwd [4]  */    'w',
           /* passwd [5]  */    'o',
           /* passwd [6]  */    'r',
           /* passwd [7]  */    'd'
        }
    }
};
const uint16_t MODEM_CFG_RECOVERY_SEARCH_STEP_1 = MODEM_CFG_RECOVERY_SEARCH_STEP_1_DEFAULT;
const uint16_t MODEM_CFG_RECOVERY_SEARCH_STEP_2 = MODEM_CFG_RECOVERY_SEARCH_STEP_2_DEFAULT;
const uint16_t MODEM_CFG_RECOVERY_SEARCH_STEP_3 = MODEM_CFG_RECOVERY_SEARCH_STEP_3_DEFAULT;
const uint16_t MODEM_CFG_RECOVERY_SEARCH_STEP_4 = MODEM_CFG_RECOVERY_SEARCH_STEP_4_DEFAULT;
const uint16_t MODEM_CFG_RECOVERY_SEARCH_STEP_5 = MODEM_CFG_RECOVERY_SEARCH_STEP_5_DEFAULT;
const uint16_t MODEM_CFG_RECOVERY_SEARCH_STEP_6 = MODEM_CFG_RECOVERY_SEARCH_STEP_6_DEFAULT;

/*The svn range from 0~99*/
const Svn MODEM_CFG_DEFAULT_SVN = {
    6,5
};


/* New MibSib Scrambling Mode default vaule */
const Boolean NEW_ROTATION_MODE_ACTIVE_DEFAULT_VALUE = TRUE;  /*1 : new algorithm (default value)*/
const Boolean NPHY_SIG_NPDCCH_SCRAMBLING_INIT_CR265_DEFAULT = FALSE;


/* Band support default value */
const uint32_t MODEM_CFG_BAND_SUPPORT_DEFAULT = 0x003BFFFF;

/* psm mode support or  not */
const Boolean MODEM_CFG_PSM_SUPPORT_DEFAULT = TRUE;

/* default t3324/t3412 value */
const Int8 MODEM_CFG_PSM_DEFAULT_T3324_VALUE_DEFAULT = 0x22;
const Int8 MODEM_CFG_PSM_DEFAULT_T3412_VALUE_DEFAULT = 0x23;


/* eDRX support or not */
const Boolean MODEM_CFG_EDRX_SUPPORT_DEFAULT = TRUE;
/* eDRX parameters */
const uint8_t MODEM_CFG_EDRX_DEFAULT_PARAMS_DEFAULT = 0x35;

/* TRUE: search all support band; FALSE: only search preferred band */
const Boolean MODEM_CFG_SEARCH_ALL_SUPPORT_BAND_DEFAULT = TRUE;

/* Combined attach support or not */
const Boolean MODEM_CFG_COMBINED_ATTACH_SUPPORT_DEFAULT = FALSE;


/* EPCO is enabled or not */
const Boolean MODEM_CFG_EPCO_SUPPORT_DEFAULT = TRUE;

/* Software and hardware information */
const AtIdentificationInfo atIdentificationInfoDefaultInit =
{ /* atIdentificationInfoInit */
    { /* AtIdentificationText */
      "Mediatek",                     /* manufacturer id */
      "NB-IOT Mobile Station",        /* model id        */
      "geneva-mp3-nb.V1.9.0",            /* S/W revision id */
      "MT2625_V01",                  /* H/W revision id */
      "Null ID"                       /* global id       */
    }
};

/* power up setting */
const Boolean powerUpProtoStackDefConfig = TRUE;      /* whether or not power up protocol stack
                                                  * TRUE: Power up Protocol stack,
                                                  * FALSE:No Power up protocol stack;
                                                  */
const Boolean powerUpSimDefConfig        = TRUE;      /* whether or not SIM card
                                                  * TRUE: Power up SIM card,
                                                  * FALSE:No Power up SIM card;
                                                  */

/* deepsleep enter criterial value */
const uint32_t PscEdrxDeepSleepCriterial = 82000 ;
const uint32_t PscDeepSleepCriterial = 11000 ;


/* stored location information */
const UsimEpsLocInfo epsLocInfoInit =
{
    {   /* rawGuti */
        /* rawGuti[0] */    0xFF,
        /* rawGuti[1] */    0xFF,
        /* rawGuti[2] */    0xFF,
        /* rawGuti[3] */    0xFF,
        /* rawGuti[4] */    0xFF,
        /* rawGuti[5] */    0xFF,
        /* rawGuti[6] */    0xFF,
        /* rawGuti[7] */    0xFF,
        /* rawGuti[8] */    0xFF,
        /* rawGuti[9] */    0xFF,
        /* rawGuti[10] */   0xFF,
        /* rawGuti[11] */   0xFF
    },
    {   /* lastRegTai */
        /* mcc */    0xFFFF,
        /* mnc */    0xFFFF,
        /* lac */    0xFFFF,
        {   /* rawIe */
            /* rawIe[0] */   0xFF,
            /* rawIe[1] */   0xFF,
            /* rawIe[2] */   0xFF,
            /* rawIe[3] */   0xFF,
            /* rawIe[4] */   0xFF
        }
    },
    /* epsUpdateStatus */   EU2_EPS_NOT_UPDATED
};

/*******************************************************************************
* Operator Prefer Information Table,it include operatot PLMN, prefer band and APN
* Operator prefer information entity format
*   {
*
*       {
*           MCC,
*           MNC,
*           is_tre_digit_mnc, // FALSE,two digit mnc, TRUE,three digit mnc
*           ACCESS_TECHNOLOGY //access technology
*       },//PLMN
*
*       "APN name string", // anp name string
*
*       (supportBandsByOperator), //bit map format, see file modem_conf_nvram_def.h for definition
*
*        * prefer band list of operatot, the max number of the list is MAX_NUMBER_OF_PREFERRED_BANDS(4)
*        * the frist band was the heghest priority band, it should priority
*        * been searched while perform PLMN find procedure.
*        * it's type is enmu NbiotBand;
*       {first priority band, second band, third band, ...},
*
*   }
*
********************************************************************************/

const NasOperatorPrefInfoNvData OperatorPreferredInfoDefTbl =
{
    /*operatorPrefInfoTable[MAX_NUM_OF_OPT_PREFERRED_INFO]*/
    {
    /*** CMCC PLMN ***/
        {
            /*PLMN 46000*/
            CMCC_PLMN_46000,
            /*Prefer APN name*/
            "",
            /*support bands by operator(bit map format)*/
            (MODEM_CFG_BAND_SUPPORT_B3_MASK | MODEM_CFG_BAND_SUPPORT_B8_MASK),
            /*prefer band list */
            {NBIOT_BAND_8,NBIOT_BAND_3}
        },

    /*** CU PLMN ***/
        {
            /*PLMN 46001*/
            CU_PLMN_46001,
            /*Prefer APN name*/
            "",
            /*support bands by operator(bit map format)*/
            (MODEM_CFG_BAND_SUPPORT_B3_MASK | MODEM_CFG_BAND_SUPPORT_B8_MASK),
            /*prefer band list*/
            {NBIOT_BAND_8,NBIOT_BAND_3}
        },

    /*** CT PLMN ***/
        {
            /*PLMN 46011*/
            CT_PLMN_46011,
            /*Prefer APN name*/
            "ctnb",
            /*support bands by operator(bit map format)*/
            (MODEM_CFG_BAND_SUPPORT_B5_MASK),
            /*prefer band list, band5*/
            {NBIOT_BAND_5}
        },

    /*** SoftBank PLMN ***/
        {
            /*PLMN 44020*/
            SOFTBANK_PLMN_44020,
            /*Prefer APN name*/
            "",
            /*support bands by operator(bit map format)*/
            (MODEM_CFG_BAND_SUPPORT_B1_MASK | MODEM_CFG_BAND_SUPPORT_B8_MASK),
            /*prefer band list*/
            {NBIOT_BAND_1,NBIOT_BAND_8}
        },
        
     /** Thailand's operator*/
        {
             /*PLMN 52004*/
             THAI_TRUE_PLMN_52004,
            /*Prefer APN name*/
            "",
             /*support bands by operator(bit map format)*/
             (MODEM_CFG_BAND_SUPPORT_B8_MASK),
             /*prefer band list, band5*/
             {NBIOT_BAND_8}
        },

        {
             /*PLMN 52003*/
             THAI_AIS_PLMN_52003,
            /*Prefer APN name*/
            "",
             /*support bands by operator(bit map format)*/
             (MODEM_CFG_BAND_SUPPORT_B8_MASK),
             /*prefer band list, band5*/
             {NBIOT_BAND_8}
        },
    
        /* Korea LGU+ */
        {
            /*PLMN 45003*/
            LGUP_PLMN_45003,
            /*Prefer APN name*/
            "",
            /*support bands by operator(bit map format)*/
            (MODEM_CFG_BAND_SUPPORT_B5_MASK),
            /*prefer band list, band5*/
            {NBIOT_BAND_5}
        },
        
    /* RJIO PLMN */
        {
            /*PLMN 45003*/
            RJIO_PLMN_405840,
            /*Prefer APN name*/
            "",
            /*support bands by operator(bit map format)*/
            (MODEM_CFG_BAND_SUPPORT_B3_MASK | MODEM_CFG_BAND_SUPPORT_B5_MASK),
            /*prefer band list, band5*/
            {NBIOT_BAND_3,NBIOT_BAND_5}
        },

         /* TMO */
        {
            /*PLMN 310240*/
            TMO_PLMN_310240,
            /*Prefer APN name*/
            "iot.nb",
            /*support bands by operator(bit map format)*/
            (MODEM_CFG_BAND_SUPPORT_B2_MASK  | MODEM_CFG_BAND_SUPPORT_B4_MASK  | MODEM_CFG_BAND_SUPPORT_B12_MASK |
            MODEM_CFG_BAND_SUPPORT_B66_MASK | MODEM_CFG_BAND_SUPPORT_B71_MASK | MODEM_CFG_BAND_SUPPORT_B85_MASK),
            /*prefer band list*/
            {NBIOT_BAND_4, NBIOT_BAND_2, NBIOT_BAND_71, NBIOT_BAND_12, NBIOT_BAND_66, NBIOT_BAND_85},
        },
		/* Arkessa.com */
	   {
		   /*PLMN 901288*/
		   ARK_PLMN_901288,
		   /*Prefer APN name*/
		   "iot.nb",
		   /*support bands by operator(bit map format)*/
		   (MODEM_CFG_BAND_SUPPORT_B5_MASK	| MODEM_CFG_BAND_SUPPORT_B20_MASK),
		   /*prefer band list*/
		   {NBIOT_BAND_5, NBIOT_BAND_20},
	   },
		/* 2-1   NCE*/
	   {
		   /*PLMN 901288*/
		   NCE_PLMN_90140,
		   /*Prefer APN name*/
		   "iot.nb",
		   /*support bands by operator(bit map format)*/
		   (MODEM_CFG_BAND_SUPPORT_B5_MASK	| MODEM_CFG_BAND_SUPPORT_B20_MASK),
		   /*prefer band list*/
		   {NBIOT_BAND_5, NBIOT_BAND_20},
	   }	
    }
};

/* NAS NSPS recoverage search period configure default value. */
const nasNspsSearchCfgData  nspsPeriodSearchCfgDefalutData = 
{
    /* the default number of search steps for LGUP */
    LGUP_RECOVERY_SEARCH_NUM_OF_STEPS_DEFAULT,

    {   /*{search type,                 repeat times,           search peroid(secends)}*/
        {
            EUTRA_SEARCH_TYPE_SHALLOW,  NAS_NSPS_STEP_REPEAT_1,     (NSPS_SEARCH_1_SECOND_PERIOD * 2)
        },/*from in service goto out service, nsps search start step index*/
        {
            EUTRA_SEARCH_TYPE_DEEP,     NAS_NSPS_STEP_REPEAT_1,     (NSPS_SEARCH_1_SECOND_PERIOD * 4)
        },
        {
            EUTRA_SEARCH_TYPE_DEEP,     NAS_NSPS_STEP_REPEAT_10,    (NSPS_SEARCH_1_SECOND_PERIOD * 10)
        },
        
        {
            EUTRA_SEARCH_TYPE_DEEP,     LGUP_PH1_RECOVERY_DEFAULT_SEARCH_RPEATS_TIMES, (NSPS_SEARCH_1_SECOND_PERIOD * 30)
        }, /*out of service start step index*/
        {
            EUTRA_SEARCH_TYPE_DEEP,     LGUP_PH1_RECOVERY_DEFAULT_SEARCH_RPEATS_TIMES, (NSPS_SEARCH_1_SECOND_PERIOD * 45)
        },
       {
            EUTRA_SEARCH_TYPE_DEEP,     LGUP_PH2_RECOVERY_DEFAULT_SEARCH_RPEATS_TIMES, (NSPS_SEARCH_1_MINUTE_PERIOD * 10)
        },
        {
            EUTRA_SEARCH_TYPE_DEEP,     LGUP_PH2_RECOVERY_DEFAULT_SEARCH_RPEATS_TIMES, (NSPS_SEARCH_1_MINUTE_PERIOD * 20)
        },
        {
            EUTRA_SEARCH_TYPE_DEEP,     LGUP_PH2_RECOVERY_DEFAULT_SEARCH_RPEATS_TIMES, (NSPS_SEARCH_1_MINUTE_PERIOD * 40)
         },
        {
            EUTRA_SEARCH_TYPE_DEEP,     LGUP_PH2_RECOVERY_DEFAULT_SEARCH_RPEATS_TIMES, (NSPS_SEARCH_1_HOUR_PERIOD)
         },
         {
            EUTRA_SEARCH_TYPE_DEEP,     LGUP_PH3_RECOVERY_DEFAULT_SEARCH_RPEATS_TIMES, (NSPS_SEARCH_1_HOUR_PERIOD * 24)
        },
    }

};
/* default carrier list for plmn
 * only support 6 carriers for each plmn
 */
const DefaultFrequencyForPlmn defaultFrequencyListForPlmn[] =
{
    /* MCC, MNC, is_three_digit_mnc, earfcn, earfcnOffset */
    /* earfcnOffset:
       0:    offset of -2
       1:    offset of -1
       2:    offset of -0.5
       3:    offset of 0
       4:    offset of 1
     */
      {0x0460, 0x0000,  FALSE,  3738,  2}, /* CMCC */
      {0x0460, 0x0000,  FALSE,  3736,  2}, /* CMCC */
      {0x0460, 0x0000,  FALSE,  3734,  2}, /* CMCC */
      {0x0460, 0x0002,  FALSE,  3739,  2}, /* CMCC */

      {0x0460, 0x0001,  FALSE,  3797,  2}, /* CU   */
      {0x0460, 0x0009,  FALSE,  3797,  2}, /* CU   */
      {0x0460, 0x0001,  FALSE,  1452,  2}, /* CU   */

      {0x0460, 0x0011,  FALSE,  2504,  2}, /* CT   */
      {0x0460, 0x0011,  FALSE,  2505,  2}, /* CT   */
      {0x0460, 0x0011,  FALSE,  2506,  2}, /* CT   */
      {0x0460, 0x0011,  FALSE,  2507,  2}, /* CT   */
      {0x0460, 0x0011,  FALSE,  2508,  2}, /* CT   */
      {0x0460, 0x0011,  FALSE,  2509,  2}, /* CT   */
};

/********************  feature switch  ******************/
const Boolean otpEnabled = FALSE;/* feature switch for OTP feature, defaulty set to FALSE */

#ifdef MTK_NVDM_OTA_SUPPORT

#undef NVDM_MODEM_ITEM_DEF
#undef NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION

#define NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION(NV_NAME, structure, GROUP_NAME, ITEM_NAME, area, default, USE_VER, verno) NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION_##USE_VER(GROUP_NAME, ITEM_NAME, verno)
#define NVDM_MODEM_ITEM_DEF(NV_NAME, structure, GROUP_NAME, ITEM_NAME, area, default, USE_VER, verno) NVDM_MODEM_ITEM_DEF_##USE_VER(GROUP_NAME, ITEM_NAME, verno)

#define NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION_NO_VER(GROUP_NAME, ITEM_NAME, verno)
#define NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION_USE_VER(GROUP_NAME, ITEM_NAME, verno) {GROUP_NAME, ITEM_NAME, verno},
#define NVDM_MODEM_ITEM_DEF_NO_VER(GROUP_NAME, ITEM_NAME, verno)
#define NVDM_MODEM_ITEM_DEF_USE_VER(GROUP_NAME, ITEM_NAME, verno) {GROUP_NAME, ITEM_NAME, verno},

static const nvdm_item_verno_info_t nvdm_modem_item_verno_table[] =
{
     #include "nvdm_modem_data_item_table.h"
     {"", "", 0xFF}
};
static uint16_t nvdm_modem_item_name_value[sizeof(nvdm_modem_item_verno_table)/sizeof(nvdm_item_verno_info_t)];
static nvdm_item_verno_table_info_t nvdm_modem_item_verno_table_info =
{
    &nvdm_modem_item_verno_table[0],
    (sizeof(nvdm_modem_item_verno_table)/sizeof(nvdm_item_verno_info_t) - 1),
    &nvdm_modem_item_name_value[0],
    false
};
#endif

/******************************************************************************
*  PRIVATE FUNCTIONS
*******************************************************************************/

uint8_t nvdm_get_default_frequency_list_num()
{
    return (sizeof(defaultFrequencyListForPlmn) / sizeof(DefaultFrequencyForPlmn));
}

/******************************************************************************
*  INTERFACE FUNCTIONS
*******************************************************************************/
#ifdef MTK_NVDM_OTA_SUPPORT
extern nvdm_item_verno_table_info_t nvdm_modem_internal_item_verno_table_info;

uint8_t nvdm_get_modem_item_verno(char* group_name, char* item_name)
{
    uint8_t verno;
    verno = nvdm_get_item_verno(&nvdm_modem_item_verno_table_info, group_name, item_name);
    if(verno == 0xFF)
        verno = nvdm_get_item_verno(&nvdm_modem_internal_item_verno_table_info, group_name, item_name);
    return verno;
}

uint8_t nvdm_get_item_verno(nvdm_item_verno_table_info_t* table_info, char* group_name, char* item_name)
{
      uint32_t i, j;
      uint16_t value = 0;
      const nvdm_item_verno_info_t * p;
      uint8_t verno = 0xFF;

      if(table_info->init_flag == false)
      {
          for(i = 0; i < table_info->item_num; i++)
          {
              value = 0;
              p = &table_info->table[i];
              for(j = 0; j < 32 && p->group_name[j] !='\0';j++)
                 value += (uint8_t)p->group_name[j];

              for(j = 0; j < 16 && p->item_name[j] != '\0'; j++)
                 value += (uint8_t)p->item_name[j];

              table_info->name_value[i] = value;
          }
          table_info->init_flag = true;
      }

      if(table_info->item_num == 0)
         return verno;

      value = 0;
      //calculate item name value;
      for(j = 0; j < 32 && group_name[j] != '\0'; j++)
         value += group_name[j];

      for(j = 0; j < 16 && item_name[j] != '\0'; j++)
         value += item_name[j];
      
      for(i = 0; i < table_info->item_num; i++)
      {
         if(value == table_info->name_value[i])
         {
             //compare group name
             p = &table_info->table[i];
             if (strcmp(group_name, p->group_name)){
                continue;
             }

             //compare item name
             if(0 == strcmp(item_name, p->item_name)){
                verno = p->version;
                break;
             }
         }
      }
      return verno;
}
#endif

/* end of file */
