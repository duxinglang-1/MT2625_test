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
 *  File definition for the SIM emulator
 **************************************************************************/

#if !defined (USIMCUSTDEF_H)
#define USIMCUSTDEF_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#if !defined (ALSI_SIG_H)
#include "alsi_sig.h"
#endif

#if !defined (SIMDATA_H)
#include "simdata.h"
#endif

#if !defined (USIMEMU_H)
#include "usimemu.h"
#endif

#include "nbiot_modem_common_config.h"

#if defined (FEA_TEMP_ESIM_EMULATION)
/*---------------------------------------------------
 * EF ICCID     2fe2
 * --------------------------------------------------*/
Int8    defIccidData[] = {0x21,0x43,0x65,0x87,0x09,0x89,0x67,0x45,0x23,0xF1};

SimEfData defIccidEfData =
{
    SIM_EMU_EF_ICCID,
    SIM_EFSTRUCT_T,
    SIM_DIR_MF,
    1,
    SIM_EMU_SIZE_ICCID_FILE,      /*record length*/
    (0),
    (0),
    { USIM_ACCESS_NEVER,          /* delete file */
      USIM_ACCESS_NEVER,          /* terminate */
      USIM_ACCESS_PIN,            /* activate */
      USIM_ACCESS_PIN,            /* deactivate */
      USIM_ACCESS_NEVER,          /* write */
      USIM_ACCESS_NEVER,          /* update */
      USIM_ACCESS_PIN             /* read */
    },
    SIM_UICC_ACTIVATED_STATE,     /* file State*/
    FALSE,                        /* sfi supported ?*/
    0,
    FALSE                         /* Under USIM*/
};

/*----------------------------------------------------------------------------
 * Ef DIR is used to store the AID of the different appliactions on the UICC
 * ---------------------------------------------------------------------------*/
Int8    defDirData[] =
{
   /*---------------------------------------------------
    * first application: UICC app
    * --------------------------------------------------*/
    0x61,                               /* app template tag */
    0x26,                              /* length of the app template */
    0x4f,                              /* application identifier tag */
    0x10,                              /* AID length , this is variable*/
 /*RID*/0xa0,0x00,0x00,0x00,0x87, /*3G app code */ 0x10, 0x02, /*PIX*/ 0x32, 0xf4, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  /* AID  value */
    0x50,                            /* application label tag  */
    0x10,                            /* application label value */
    'F','I','R','S','T',0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,  /* application label value  */
};

/*---------------------------------------------------
 * EF DIR this EF points to USIM applications  2f00
 * --------------------------------------------------*/
SimEfData    defDirEfData =
{
    SIM_EMU_EF_DIR,              /* Id           */
    SIM_EFSTRUCT_LF,         /* Type         */
    SIM_DIR_MF,              /* Dir          */
    SIM_EMU_NUM_DIR_RECS,   /*  Num recs      */
    SIM_EMU_SIZE_DIR_FILE,   /* Rec Len     */
    (0),                     /* curr Rec     */
    (0),
  {   USIM_ACCESS_NEVER,    /* delete file*/
      USIM_ACCESS_NEVER,    /* terminate*/
      USIM_ACCESS_ADM,      /* activate */
      USIM_ACCESS_ADM,      /* deactivate */
      USIM_ACCESS_NEVER,    /* write */
      USIM_ACCESS_ADM,      /* update*/
      USIM_ACCESS_ALWAYS    /* read*/
  },
    SIM_UICC_ACTIVATED_STATE, /*file State*/
    TRUE,                     /*sfi supported  */
    0x00,                     /*sfi*/
    FALSE                      /*Under USIM*/
};

 /*----------------------------------------------------
  * EF ACC :     6f78
  * Access control class
  * please refer to 131 102 section 4.2.15
  * --------------------------------------------------*/
Int8    defAccUsimData[] = { 0x02, 0x00 };

SimEfData    defAccUsimEfData =
{
    SIM_EMU_EF_ACC,         /* Id           */
    SIM_EFSTRUCT_T,         /* Type         */
    SIM_DIR_ADF_USIM,       /* Dir          */
    1,                      /* Num recs      */
    SIM_EMU_SIZE_USIM_ACC_FILE,  /* Rec len     */
    (0),                    /* curr Rec     */
    (0),
    {
    USIM_ACCESS_NEVER,    /* delete file*/
    USIM_ACCESS_NEVER,    /* terminate*/
    USIM_ACCESS_ADM,      /* activate */
    USIM_ACCESS_ADM,      /* deactivate */
    USIM_ACCESS_NEVER,    /* write */
    USIM_ACCESS_ADM,      /* update*/
    USIM_ACCESS_PIN       /* read*/
    },
    SIM_UICC_ACTIVATED_STATE, /*file State*/
    TRUE,                     /*sfi supported  */
    0x06,                     /*sfi*/
    TRUE                      /*Under USIM*/
};


#if defined (TEST_SP8315)
/** Equivalent Home PLMN list */
Int8   defEhplmnUsimData[]  = {
                                0x64, 0xf0, 0x10,      /*EHPLMN 1  */
                                0xff, 0xff, 0xff,      /*EHPLMN 2. */
                                0xff, 0xff, 0xff,      /*EHPLMN 3  */
                                0xff, 0xff, 0xff       /*EHPLMN 4  */
                               };

SimEfData defEhplmnUsimEfData =
{
  SIM_EMU_EF_EHPLMN,          /* Id           */
  SIM_EFSTRUCT_T,             /* Type         */
  SIM_DIR_ADF_USIM,
  1,                              /* one record because transparent  */
  SIM_EMU_SIZE_USIM_EHPLMN_FILE,  /* total file size                 */
    (0),                          /* curr Rec                        */
    (0),
  {
     USIM_ACCESS_NEVER,    /* delete file */
     USIM_ACCESS_NEVER,    /* terminate   */
     USIM_ACCESS_ADM,      /* activate    */
     USIM_ACCESS_ADM,      /* deactivate  */
     USIM_ACCESS_NEVER,    /* write       */
     USIM_ACCESS_ADM,      /* update      */
     USIM_ACCESS_PIN       /* read        */
   },
   SIM_UICC_ACTIVATED_STATE, /*file State     */
   TRUE,                     /*sfi supported  */
   0x1d,                     /*sfi            */
   TRUE                      /*Under USIM     */

 };
/** Equivalent Home PLMN presentation Indication */
Int8   defNasSecurityData[]  = { 0xa0 /* EPS NAS SC Tag    */, 0x34, /* length of rest of TLV    */
                                 0x80 /*KSI ASME Tag       */, 0x01, /* 1 byte long TLV          */
                                 0x07 /*KSI ASME valid 0-6 */,
                                 0x81 /* KASME TAG         */, 0x20, /* rel 8 K asme is 32 bytes */
                                 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                 0x82 /* UpL NAS count Tag */, 0x04, /* coded on 4 bytes          */
                                 0xff, 0xff, 0xff, 0xff,             /* max valid entry -0xFFFFFF */
                                 0x83 /* DL NAS count Tag  */, 0x04,  /* coded on 4 bytes          */
                                 0xff, 0xff, 0xff, 0xff,             /* max valid entry -0xFFFFFF */
                                 0x84 /* EPS alg ident Tag */, 0x01,
                                 0xff/* bit 5-7 tOA ciphering - 000 = ciphering not used
                                      * bit 1-3 001 integrity - 001 = EPS INTEG 128-EIA1 */
                                 };

SimEfData defNasSecurityEfData =
{
  SIM_EMU_NAS_SECURITY,        /* Id           */
  SIM_EFSTRUCT_LF,            /* Type         */
  SIM_DIR_ADF_USIM,
  SIM_EMU_NUM_NAS_SECURITY_RECS,  /* num recs                */
  SIM_EMU_SIZE_NAS_SECURITY_FILE, /* rec length              */
    (0),                      /* curr Rec                        */
    (0),  {
     USIM_ACCESS_NEVER,    /* delete file */
     USIM_ACCESS_NEVER,    /* terminate   */
     USIM_ACCESS_ADM,      /* activate    */
     USIM_ACCESS_ADM,      /* deactivate  */
     USIM_ACCESS_NEVER,    /* write       */
     USIM_ACCESS_PIN,      /* update      */
     USIM_ACCESS_PIN       /* read        */
   },
   SIM_UICC_ACTIVATED_STATE, /*file State     */
   TRUE,                     /*sfi supported  */
   0x18,                     /*sfi            */
   TRUE                      /*Under USIM     */

 };

Int8   defEpsLociData[]  = { 0x0b, 0xf6, 0x00, 0xf1, 0x10, 0x80,  /* GUTI, LRTAI and EPS update status */
                             0x01, 0x01, 0x12, 0x34, 0x56, 0x78,
                             0x00, 0xF1, 0x10, 0x00, 0x01, 0x00}; /* not updated */

SimEfData defEpsLociEfData =
{
  SIM_EMU_EF_EPS_LOCI,        /* Id                          */
  SIM_EFSTRUCT_T,            /* Type - transparent           */
  SIM_DIR_ADF_USIM,
  1,                          /* one record because transparent  */
  SIM_EMU_SIZE_EPS_LOCI_FILE, /* total file size                 */
    (0),                          /* curr Rec                */
    (0),  {
     USIM_ACCESS_NEVER,    /* delete file */
     USIM_ACCESS_NEVER,    /* terminate   */
     USIM_ACCESS_ADM,      /* activate    */
     USIM_ACCESS_ADM,      /* deactivate  */
     USIM_ACCESS_NEVER,    /* write       */
     USIM_ACCESS_PIN,      /* update      */
     USIM_ACCESS_PIN       /* read        */
   },
   SIM_UICC_ACTIVATED_STATE, /*file State     */
   TRUE,                     /*sfi supported  */
   0x1e,                     /*sfi            */
   TRUE                      /*Under USIM     */

 };

/*-----------------18/10/01 08:40-------------------
 *  Ef IMSI:  Language indication
 *  For coding please refer to 131 102 section 4.2.2
 * --------------------------------------------------*/
Int8    defImsiUsimData[] = { 0x08,    /*length of IMSI*/
                              0x49, 0x06, 0x10, 0x10, 0x32, 0x54, 0x76, 0x98 };    /*IMSI*/

SimEfData    defImsiUsimEfData =
{
    SIM_EMU_EF_IMSI,        /* Id           */
    SIM_EFSTRUCT_T,         /* Type         */
    SIM_DIR_ADF_USIM,       /* Dir          */
    1,                      /* Num recs      */
    SIM_EMU_SIZE_USIM_IMSI_FILE, /* Rec len     */
    (0),                    /* curr Rec     */
    (0),
    {
    USIM_ACCESS_NEVER,    /* delete file*/
    USIM_ACCESS_NEVER,    /* terminate*/
    USIM_ACCESS_ADM,      /* activate */
    USIM_ACCESS_ADM,      /* deactivate */
    USIM_ACCESS_NEVER,    /* write */
    USIM_ACCESS_PIN,      /* update*/
    USIM_ACCESS_PIN    /* read*/
    },
    SIM_UICC_ACTIVATED_STATE, /*file State*/
    TRUE,                     /*sfi supported  */
    0x07,                     /*sfi*/
    TRUE                      /*Under USIM*/
 };

/*-----------------17/07/2003 16:15-----------------
 * EF HPLMN: this file is located under DF GSM ACCESS Dir
 * For coding see 131 102 section 4.2.6  6f31
 * --------------------------------------------------*/

Int8    defHplmnUsimData[] = { 0x05 };

SimEfData    defHplmnUsimEfData =
{
    SIM_EMU_EF_HPLMN,           /* Id           */
    SIM_EFSTRUCT_T,         /* Type         */
    SIM_DIR_ADF_USIM,       /* Dir          */
    1,                      /* Num rec      */
    SIM_EMU_SIZE_USIM_HPLMN_FILE,/* Rec len  */
    (0),                    /* curr Rec     */
    (0),
    { USIM_ACCESS_NEVER,    /* delete file*/
      USIM_ACCESS_NEVER,    /* terminate*/
      USIM_ACCESS_ADM,      /* activate */
      USIM_ACCESS_ADM,      /* deactivate */
      USIM_ACCESS_NEVER,    /* write */
      USIM_ACCESS_PIN,      /* update*/
      USIM_ACCESS_PIN       /* read*/
    },
    SIM_UICC_ACTIVATED_STATE, /*file State*/
    TRUE,                     /*sfi supported  */
    0x12,                     /*sfi*/
    TRUE                     /*Under USIM*/
};

/*-----------------18/10/01 10:39-------------------
 * Ef UST: USIM service table
 * For coding please refer to 131 102 section 4.2.8
 * --------------------------------------------------*/
                                        /*   bit 8       bit7      bit6    bit5     bit4      bit3    bit2       bit1    */
Int8 defUstUsimData[] = { 0x86,         /*  OCI/OCT      EXT4      BDN     EXT3     SDN       EXT2    FDN        local PB */
                          0x7f,         /*  CBMIR        CBMI      CCP     AoC      SMSP      SMSR    SMS        ICI    */
                          0x1f,         /*  eMLPP        unused    IMG     MSISDN   UserPLMN  SPN     GID2       GID1*/
                          0x18,         /*  run AT       mo-sms    cc      sms-cb   sms-pp    GSM a.  RFU        automatic answer*/
                          0x23,         /*   inv. scan   CPBCCH   GSM sec.  CNL      dck       apn     est        1  */
                          0x0e,         /*   MWIS     Mailboxdn OperatorPLMN PLMNname ext5   HPLMNwAct OPLMNwAct  MExE*/
                          0x00,         /*  N/A      MMS user c. cc on GPRS  ext8     MMS        SPD      RPLMN      CFIS*/
                          0x00,
                          0x40,
                          0x00,          /* 73-80, Service 80; Conn. Param. for USIM IP connection, service 73; Equivalent HPLMN  */
                          0x50,          /* 81-88, bit 5 not set for EMM info */
                          0x00,          /* 89-96 */
                          0x00,          /* 97-104 */
                          0x00,          /* 105-112 */
                          0x00           /* 113-120, bit 8 set for XCAP Service 120 */
                        };

SimEfData defUstUsimEfData =
{
  SIM_EMU_EF_UST,              /* Id           */
  SIM_EFSTRUCT_T,              /* Type         */
  SIM_DIR_ADF_USIM,            /* Dir          */
  1,                           /* Num recs      */
  SIM_EMU_SIZE_USIM_UST_FILE,  /* Rec Len     */
  (0),                         /* curr Rec     */
  (0),
  {
     USIM_ACCESS_NEVER,    /* delete file*/
     USIM_ACCESS_NEVER,    /* terminate*/
     USIM_ACCESS_ADM,      /* activate */
     USIM_ACCESS_ADM,      /* deactivate */
     USIM_ACCESS_NEVER,    /* write */
     USIM_ACCESS_PIN,      /* update*/
     USIM_ACCESS_PIN       /* read*/
  },
  SIM_UICC_ACTIVATED_STATE, /*file State*/
  TRUE,                    /*sfi supported  */
  0x04,                     /*sfi*/
  TRUE                      /*Under USIM*/
};

/*-----------------17/07/2003 16:37-----------------*
 * EF PLMN SEL: Preferred PLMN 6f60                 *
 * -------------------------------------------------*/

Int8    defUserPlmnSelUsimData[] = { 0x64, 0xf0, 0x10, 0x40, 0x00,
                                     0x64, 0xf0, 0x10, 0x80, 0x00,
                                     0x64, 0xf0, 0x10, 0x00, 0x80,
                                     0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00 };

SimEfData    defUserPlmnSelUsimEfData =
{
    SIM_EMU_EF_PLMNW_ACT,               /* Id           */
    SIM_EFSTRUCT_T,                 /* Type         */
    SIM_DIR_ADF_USIM,               /* Dir          */
    1,                              /* Num recs      */
    SIM_EMU_SIZE_USER_PLMNWACT_FILE,   /* Rec len    */
    (0),                            /* curr Rec     */
    (0),
   {
    USIM_ACCESS_NEVER,    /* delete file*/
    USIM_ACCESS_NEVER,    /* terminate*/
    USIM_ACCESS_ADM,      /* activate */
    USIM_ACCESS_ADM,      /* deactivate */
    USIM_ACCESS_NEVER,    /* write */
    USIM_ACCESS_PIN,      /* update*/
    USIM_ACCESS_PIN       /* read*/
  },
    SIM_UICC_ACTIVATED_STATE, /*file State*/
    TRUE,                     /*sfi supported  */
    0x0d,                     /*sfi*/
    TRUE                      /*Under USIM*/
};

/*-----------------17/07/2003 16:37-----------------*
 * EF OPLMNwAct: Operator controlled PLMN selector  *
 * with access technology      6f61                 *
 * -------------------------------------------------*/

Int8    defOperatorPlmnSelUsimData[] = { 0x64, 0xf0, 0x10, 0x40, 0x00,
                                         0x64, 0xf0, 0x10, 0x80, 0x00,
                                         0x64, 0xf0, 0x10, 0x00, 0x80,
                                         0xff, 0xff, 0xff, 0x00, 0x00,
                                         0xff, 0xff, 0xff, 0x00, 0x00,
                                         0xff, 0xff, 0xff, 0x00, 0x00,
                                         0xff, 0xff, 0xff, 0x00, 0x00,
                                         0xff, 0xff, 0xff, 0x00, 0x00 };

SimEfData    defOperatorPlmnSelUsimEfData =
{
    SIM_EMU_EF_OPLMNW_ACT,               /* Id           */
    SIM_EFSTRUCT_T,                 /* Type         */
    SIM_DIR_ADF_USIM,               /* Dir          */
    1,                              /* Num recs      */
    SIM_EMU_SIZE_OPERATOR_PLMNWACT_FILE,   /* Rec len    */
    (0),                            /* curr Rec     */
    (0),
   {
    USIM_ACCESS_NEVER,    /* delete file*/
    USIM_ACCESS_NEVER,    /* terminate*/
    USIM_ACCESS_ADM,      /* activate */
    USIM_ACCESS_ADM,      /* deactivate */
    USIM_ACCESS_NEVER,    /* write */
    USIM_ACCESS_PIN,      /* update*/
    USIM_ACCESS_PIN       /* read*/
  },
    SIM_UICC_ACTIVATED_STATE, /*file State*/
    TRUE,                     /*sfi supported  */
    0x11,                     /*sfi*/
    TRUE                      /*Under USIM*/
};
/*-----------------17/07/2003 16:37-----------------*
 * EF HPLMN SELECTOR WITH ACCESS TECHNOLOGY         *
 * See TS 131 102 section 4.2.54         6f62       *
 * -------------------------------------------------*/

Int8    defHplmnSelUsimData[] = { 0x64, 0xf0, 0x10, 0x40, 0x00,
                                  0x64, 0xf0, 0x10, 0x80, 0x00,
                                  0x64, 0xf0, 0x10, 0x00, 0x80,
                                  0xff, 0xff, 0xff, 0x00, 0x00,
                                  0xff, 0xff, 0xff, 0x00, 0x00,
                                  0xff, 0xff, 0xff, 0x00, 0x00,
                                  0xff, 0xff, 0xff, 0x00, 0x00,
                                  0xff, 0xff, 0xff, 0x00, 0x00 };

SimEfData    defHplmnSelUsimEfData =
{
    SIM_EMU_EF_HPLMNW_ACT,               /* Id           */
    SIM_EFSTRUCT_T,                 /* Type         */
    SIM_DIR_ADF_USIM,               /* Dir          */
    1,                              /* Num recs      */
    SIM_EMU_SIZE_HPLMNWACT_FILE,   /* Rec len    */
    (0),                            /* curr Rec     */
    (0),
   {
    USIM_ACCESS_NEVER,    /* delete file*/
    USIM_ACCESS_NEVER,    /* terminate*/
    USIM_ACCESS_ADM,      /* activate */
    USIM_ACCESS_ADM,      /* deactivate */
    USIM_ACCESS_NEVER,    /* write */
    USIM_ACCESS_PIN,      /* update*/
    USIM_ACCESS_PIN       /* read*/
  },
    SIM_UICC_ACTIVATED_STATE, /*file State*/
    TRUE,                     /*sfi supported  */
    0x13,                     /*sfi*/
    TRUE                      /*Under USIM*/
};
/*-----------------18/07/2003 13:17-----------------
 * EF PS LOCI (packet switched location information)
 * See TS 131 102, section 4.2.23 6f73
 * --------------------------------------------------*/
Int8    defPsLociUsimData[] = { 0xff, 0xff, 0xff, 0xff,
                                0xff, 0xff, 0xff,
                                0x00, 0xf1, 0x10, 0xff, 0xfe, 0xff,
                                0x02 };

SimEfData    defPsLociUsimEfData =
{
    SIM_EMU_EF_PS_LOCI,                 /* Id           */
    SIM_EFSTRUCT_T,                  /* Type         */
    SIM_DIR_ADF_USIM,                /* Dir          */
    1,                               /* Num recs      */
    SIM_EMU_SIZE_USIM_LOCI_PS_FILE,  /* Rec len     */
    (0),                             /* curr Rec     */
    (0),
   {
     USIM_ACCESS_NEVER,    /* delete file */
     USIM_ACCESS_NEVER,    /* terminate */
     USIM_ACCESS_ADM,      /* activate */
     USIM_ACCESS_ADM,      /* deactivate */
     USIM_ACCESS_NEVER,    /* write */
     USIM_ACCESS_PIN,      /* update */
     USIM_ACCESS_PIN       /* read */
   },
   SIM_UICC_ACTIVATED_STATE, /*file State */
   TRUE,                     /*sfi supported  */
   0x0c,                     /*sfi */
   TRUE                      /*Under USIM */
};

/*-----------------17/07/2003 16:37-----------------
 * EF FPLMN: Forbidden PLMN 6f7b
 *
 * see section 4.2.16  for coding (TS 131 102)
 * --------------------------------------------------*/
Int8    defFplmnUsimData[] = { 0x64, 0xf0, 0x00,      /*FPLMN 1.   (MCC = 234 MNC = 15) */
                               0xff, 0xff, 0xff,      /*FPLMN 2 */
                               0xff, 0xff, 0xff,      /*FPLMN 3 */
                               0xff, 0xff, 0xff };    /*FPLMN 4 */

SimEfData    defFplmnUsimEfData =
{
    SIM_EMU_EF_FPLMN,                   /* Id           */
    SIM_EFSTRUCT_T,                 /* Type         */
    SIM_DIR_ADF_USIM,               /* Dir          */
    1,                              /* Num recs      */
    SIM_EMU_SIZE_USIM_FPLMN_FILE,   /* Rec len    */
    (0),                            /* curr Rec     */
    (0),
   {
    USIM_ACCESS_NEVER,    /* delete file*/
    USIM_ACCESS_NEVER,    /* terminate*/
    USIM_ACCESS_ADM,      /* activate */
    USIM_ACCESS_ADM,      /* deactivate */
    USIM_ACCESS_NEVER,    /* write */
    USIM_ACCESS_PIN,      /* update*/
    USIM_ACCESS_PIN       /* read*/
  },
    SIM_UICC_ACTIVATED_STATE, /*file State*/
    TRUE,                     /*sfi supported  */
    0x0d,                     /*sfi*/
    TRUE                      /*Under USIM*/
};
/*-----------------17/07/2003 16:32-----------------
  * EF LOCI : 6f7e
  * Location information
  * please refer to 131 102 section 4.2.17
  * --------------------------------------------------*/
Int8    defLociUsimData[] = { 0x0f, 0x2b, 0x41, 0x75,    /*TMSI*/
                              0x00, 0xf1, 0x10, 0x00, 0x01,  /* LAI*/
                              0x00, 0x00  };   /*RFU + location update status*/

SimEfData    defLociUsimEfData =
{
    SIM_EMU_EF_LOCI,            /* Id           */
    SIM_EFSTRUCT_T,         /* Type         */
    SIM_DIR_ADF_USIM,       /* Dir          */
    1,                      /* Num recs     */
    SIM_EMU_SIZE_USIM_LOCI_FILE, /* Rec Len */
    (0),                    /* curr Rec     */
    (0),
   {
    USIM_ACCESS_NEVER,    /* delete file*/
    USIM_ACCESS_NEVER,    /* terminate*/
    USIM_ACCESS_ADM,      /* activate */
    USIM_ACCESS_ADM,      /* deactivate */
    USIM_ACCESS_NEVER,    /* write */
    USIM_ACCESS_PIN,      /* update*/
    USIM_ACCESS_PIN       /* read*/
  },
    SIM_UICC_ACTIVATED_STATE, /*file State*/
    TRUE,                     /*sfi supported  */
    0x0b,                     /*sfi*/
    TRUE                      /*Under USIM*/
};

/*-----------------17/07/2003 16:44-----------------
 * EF AD  Administrative Data
 * for coding please refer to 131 102 section 4.2.18
 * --------------------------------------------------*/

Int8    defAdUsimData[] =  { MS_OPERN_TA_PLUS_SPECIFIC,  /*UE operation mode*/
                             0, 0,  /*additional info*/
                             2};

SimEfData    defAdUsimEfData =
{
    SIM_EMU_EF_AD,              /* Id           */
    SIM_EFSTRUCT_T,             /* Type         */
    SIM_DIR_ADF_USIM,           /* Dir          */
    1,                          /* Num recs      */
    SIM_EMU_SIZE_USIM_AD_FILE,  /* Rec len     */
    (0),                        /* curr Rec     */
    (0),
    {
     USIM_ACCESS_NEVER,    /* delete file*/
     USIM_ACCESS_NEVER,    /* terminate*/
     USIM_ACCESS_ADM,      /* activate */
     USIM_ACCESS_ADM,      /* deactivate */
     USIM_ACCESS_NEVER,    /* write */
     USIM_ACCESS_PIN,      /* update*/
     USIM_ACCESS_ALWAYS    /* read*/
    },
    SIM_UICC_ACTIVATED_STATE, /*file State*/
    TRUE,                     /*sfi supported  */
    0x03,                     /*sfi*/
    TRUE                      /*Under USIM*/
};

/*EF OPL: this is part of EONS. see 51.011 for coding 6fc6*/

Int8    defOplUsimData[] = {
                             /* First record   */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record  2 */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record  3 */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record  4 */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record  5 */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record  6 */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record  7 */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record  8 */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record 9  */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record 10  */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record 11  */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record 12  */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record 13  */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record 14  */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record 15  */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
                             };
SimEfData    defOplUsimEfData =
{
    SIM_EMU_EF_OPL,                  /* Id           */
    SIM_EFSTRUCT_LF,                 /* Type         */
    SIM_DIR_ADF_USIM,                /* Dir          */
    SIM_EMU_NUM_USIM_OPL_RECS,       /* Num recs      */
    SIM_EMU_SIZE_USIM_OPL_FILE,      /* Rec len     */
    (0),                             /* curr Rec     */
    (0),
   {
     USIM_ACCESS_NEVER,    /* delete file */
     USIM_ACCESS_NEVER,    /* terminate */
     USIM_ACCESS_ADM,      /* activate */
     USIM_ACCESS_ADM,      /* deactivate */
     USIM_ACCESS_NEVER,    /* write */
     USIM_ACCESS_PIN,      /* update */
     USIM_ACCESS_PIN       /* read */
   },
   SIM_UICC_ACTIVATED_STATE, /*file State */
   TRUE,                     /*sfi supported  */
   0x1a,                     /*sfi */
   TRUE                      /*Under USIM */
};

/** NAS Config Information  (Transparent) 6fe8*/
/* no EF NAS Config*/
                            /* Tag   Len   Val    */
Int8   defNasConfigData[]  = { 0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff};

SimEfData defNasConfigEfData =
{
  SIM_EMU_EF_NAS_CONFIG,        /* Id                          */
  SIM_EFSTRUCT_T,            /* Type - transparent           */
  SIM_DIR_ADF_USIM,
  1,                          /* one record because transparent  */
  SIM_EMU_SIZE_NAS_CONFIG_FILE, /* total file size                 */
    (0),                          /* curr Rec                */
    (0),  {
     USIM_ACCESS_NEVER,    /* delete file */
     USIM_ACCESS_NEVER,    /* terminate   */
     USIM_ACCESS_ADM,      /* activate    */
     USIM_ACCESS_ADM,      /* deactivate  */
     USIM_ACCESS_NEVER,    /* write       */
     USIM_ACCESS_PIN,      /* update      */
     USIM_ACCESS_PIN       /* read        */
   },
   SIM_UICC_ACTIVATED_STATE, /*file State     */
   FALSE,                    /*sfi supported  */
   0,                        /*sfi            */
   TRUE                      /*Under USIM     */

 };

/** auth  */
/*fixed values for response based upon what is expected by the tester */
Int8    defAuthSres[8] = {0xa3,0xdf,0x0e,0x6e,0x32,0x3b,0x36,0xc4};
Int8    defAuthCk[16]  = {0xdf,0x0e,0x6e,0x32,0x3b,0x36,0xc4,0x6c,0xad,0x0d,0x84,0x17,0xf5,0xdb,0x78,0xa3};
Int8    defAuthIk[16]  = {0x0e,0x6e,0x32,0x3b,0x36,0xc4,0x6c,0xad,0x0d,0x84,0x17,0xf5,0xdb,0x78,0xa3,0xdf};

#else /* TEST_M8821C */
/** Equivalent Home PLMN list   6fd9  */
Int8   defEhplmnUsimData[]  = {
                                0x00, 0xf1, 0x01,      /*EHPLMN 1  */
                                0x00, 0xf1, 0x10,      /*EHPLMN 2. */
                                0x64, 0xf0, 0x20,      /*EHPLMN 3  */
                                0x00, 0xf2, 0x11       /*EHPLMN 4  */
                               };

SimEfData defEhplmnUsimEfData =
{
  SIM_EMU_EF_EHPLMN,          /* Id           */
  SIM_EFSTRUCT_T,             /* Type         */
  SIM_DIR_ADF_USIM,
  1,                              /* one record because transparent  */
  SIM_EMU_SIZE_USIM_EHPLMN_FILE,  /* total file size                 */
    (0),                          /* curr Rec                        */
    (0),
  {
     USIM_ACCESS_NEVER,    /* delete file */
     USIM_ACCESS_NEVER,    /* terminate   */
     USIM_ACCESS_ADM,      /* activate    */
     USIM_ACCESS_ADM,      /* deactivate  */
     USIM_ACCESS_NEVER,    /* write       */
     USIM_ACCESS_ADM,      /* update      */
     USIM_ACCESS_PIN       /* read        */
   },
   SIM_UICC_ACTIVATED_STATE, /*file State     */
   TRUE,                     /*sfi supported  */
   0x1d,                     /*sfi            */
   TRUE                      /*Under USIM     */
};

/*--------------------------------------------------*
 * EF EPS NAS Security Context         6fe4         *
 * -------------------------------------------------*/
Int8   defNasSecurityData[]  = { 0xa0 /* EPS NAS SC Tag    */, 0x34, /* length of rest of TLV    */
                                 0x80 /*KSI ASME Tag       */, 0x01, /* 1 byte long TLV          */
                                 0x06 /*KSI ASME valid 0-6 */,
                                 0x81 /* KASME TAG         */, 0x20, /* rel 8 K asme is 32 bytes */
                                 0x21, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
                                 0x31, 0x32, 0x03, 0x34, 0x35, 0x36, 0x37, 0x38,
                                 0x41, 0x42, 0x43, 0x04, 0x45, 0x46, 0x47, 0x48,
                                 0x51, 0x52, 0x53, 0x54, 0x05, 0x56, 0x57, 0x58,
                                 0x82 /* UpL NAS count Tag */, 0x04, /* coded on 4 bytes          */
                                 0x00, 0x00, 0x12, 0xFF,             /* max valid entry -0xFFFFFF */
                                 0x83 /* DL NAS count Tag  */, 0x04,  /* coded on 4 bytes          */
                                 0x00, 0x00, 0x15, 0xF4,             /* max valid entry -0xFFFFFF */
                                 0x84 /* EPS alg ident Tag */, 0x01,
                                 0x01/* bit 5-7 tOA ciphering - 000 = ciphering not used
                                      * bit 1-3 001 integrity - 001 = EPS INTEG 128-EIA1 */
                                 };

SimEfData defNasSecurityEfData =
{
  SIM_EMU_NAS_SECURITY,        /* Id           */
  SIM_EFSTRUCT_LF,            /* Type         */
  SIM_DIR_ADF_USIM,
  SIM_EMU_NUM_NAS_SECURITY_RECS,  /* num recs                */
  SIM_EMU_SIZE_NAS_SECURITY_FILE, /* rec length              */
    (0),                      /* curr Rec                        */
    (0),  {
     USIM_ACCESS_NEVER,    /* delete file */
     USIM_ACCESS_NEVER,    /* terminate   */
     USIM_ACCESS_ADM,      /* activate    */
     USIM_ACCESS_ADM,      /* deactivate  */
     USIM_ACCESS_NEVER,    /* write       */
     USIM_ACCESS_PIN,      /* update      */
     USIM_ACCESS_PIN       /* read        */
   },
   SIM_UICC_ACTIVATED_STATE, /*file State     */
   TRUE,                     /*sfi supported  */
   0x18,                     /*sfi            */
   TRUE                      /*Under USIM     */

 };

/*--------------------------------------------------*
 * EF EPS Loci                 6fe3                 *
 * -------------------------------------------------*/
Int8   defEpsLociData[]  = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  /* GUTI, LRTAI and EPS update status */
                             0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                             0x42, 0xF6, 0x18, 0xFF, 0xFE, 0x01}; /* not updated */

SimEfData defEpsLociEfData =
{
  SIM_EMU_EF_EPS_LOCI,        /* Id                          */
  SIM_EFSTRUCT_T,            /* Type - transparent           */
  SIM_DIR_ADF_USIM,
  1,                          /* one record because transparent  */
  SIM_EMU_SIZE_EPS_LOCI_FILE, /* total file size                 */
    (0),                          /* curr Rec                */
    (0),  {
     USIM_ACCESS_NEVER,    /* delete file */
     USIM_ACCESS_NEVER,    /* terminate   */
     USIM_ACCESS_ADM,      /* activate    */
     USIM_ACCESS_ADM,      /* deactivate  */
     USIM_ACCESS_NEVER,    /* write       */
     USIM_ACCESS_PIN,      /* update      */
     USIM_ACCESS_PIN       /* read        */
   },
   SIM_UICC_ACTIVATED_STATE, /*file State     */
   TRUE,                     /*sfi supported  */
   0x1e,                     /*sfi            */
   TRUE                      /*Under USIM     */

 };

/*---------------------------------------------------
 *  Ef IMSI:  Language indication    6f07
 *  For coding please refer to 131 102 section 4.2.2
 * --------------------------------------------------*/
Int8    defImsiUsimData[] = { 0x08,    /*length of IMSI*/
                              0x09, 0x10, 0x10, 0x10, 0x32, 0x54, 0x76, 0x98 };    /*IMSI*/

SimEfData    defImsiUsimEfData =
{
    SIM_EMU_EF_IMSI,        /* Id           */
    SIM_EFSTRUCT_T,         /* Type         */
    SIM_DIR_ADF_USIM,       /* Dir          */
    1,                      /* Num recs      */
    SIM_EMU_SIZE_USIM_IMSI_FILE, /* Rec len     */
    (0),                    /* curr Rec     */
    (0),
    {
    USIM_ACCESS_NEVER,    /* delete file*/
    USIM_ACCESS_NEVER,    /* terminate*/
    USIM_ACCESS_ADM,      /* activate */
    USIM_ACCESS_ADM,      /* deactivate */
    USIM_ACCESS_NEVER,    /* write */
    USIM_ACCESS_PIN,      /* update*/
    USIM_ACCESS_PIN    /* read*/
    },
    SIM_UICC_ACTIVATED_STATE, /*file State*/
    TRUE,                     /*sfi supported  */
    0x07,                     /*sfi*/
    TRUE                      /*Under USIM*/
 };

/*----------------------------------------------------
 * EF HPLMN: this file is located under DF GSM ACCESS Dir
 * For coding see 131 102 section 4.2.6  6f31
 * --------------------------------------------------*/
Int8    defHplmnUsimData[] = { 0x00 };

SimEfData    defHplmnUsimEfData =
{
    SIM_EMU_EF_HPLMN,           /* Id           */
    SIM_EFSTRUCT_T,         /* Type         */
    SIM_DIR_ADF_USIM,       /* Dir          */
    1,                      /* Num rec      */
    SIM_EMU_SIZE_USIM_HPLMN_FILE,/* Rec len  */
    (0),                    /* curr Rec     */
    (0),
    { USIM_ACCESS_NEVER,    /* delete file*/
      USIM_ACCESS_NEVER,    /* terminate*/
      USIM_ACCESS_ADM,      /* activate */
      USIM_ACCESS_ADM,      /* deactivate */
      USIM_ACCESS_NEVER,    /* write */
      USIM_ACCESS_PIN,      /* update*/
      USIM_ACCESS_PIN       /* read*/
    },
    SIM_UICC_ACTIVATED_STATE, /*file State*/
    TRUE,                     /*sfi supported  */
    0x12,                     /*sfi*/
    TRUE                     /*Under USIM*/
};

/*----------------------------------------------------
 * Ef UST: USIM service table      6f38
 * For coding please refer to 131 102 section 4.2.8
 * --------------------------------------------------*/
                                        /*   bit 8       bit7      bit6    bit5     bit4      bit3    bit2       bit1    */
Int8 defUstUsimData[] = { 0x9e,         /*  OCI/OCT      EXT4      BDN     EXT3     SDN       EXT2    FDN        local PB */
                          0xff,         /*  CBMIR        CBMI      CCP     AoC      SMSP      SMSR    SMS        ICI    */
                          0xbf,         /*  eMLPP        unused    IMG     MSISDN   UserPLMN  SPN     GID2       GID1*/
                          0xfd,         /*  run AT       mo-sms    cc      sms-cb   sms-pp    GSM a.  RFU        automatic answer*/
                          0xff,         /*   inv. scan   CPBCCH   GSM sec.  CNL      dck       apn     est        1  */
                          0xfe,         /*   MWIS     Mailboxdn OperatorPLMN PLMNname ext5   HPLMNwAct OPLMNwAct  MExE*/
                          0x01,         /*  N/A      MMS user c. cc on GPRS  ext8     MMS        SPD      RPLMN      CFIS*/
                          0x00,
                          0x00,
                          0x00,          /* 73-80, Service 80; Conn. Param. for USIM IP connection, service 73; Equivalent HPLMN  */
                          0x30,          /* 81-88, bit 5 not set for EMM info */
                          0xff,          /* 89-96 */
                          0x00,          /* 97-104 */
                          0x00,          /* 105-112 */
                          0x00           /* 113-120, bit 8 set for XCAP Service 120 */
                        };

SimEfData defUstUsimEfData =
{
  SIM_EMU_EF_UST,              /* Id           */
  SIM_EFSTRUCT_T,              /* Type         */
  SIM_DIR_ADF_USIM,            /* Dir          */
  1,                           /* Num recs      */
  SIM_EMU_SIZE_USIM_UST_FILE,  /* Rec Len     */
  (0),                         /* curr Rec     */
  (0),
  {
     USIM_ACCESS_NEVER,    /* delete file*/
     USIM_ACCESS_NEVER,    /* terminate*/
     USIM_ACCESS_ADM,      /* activate */
     USIM_ACCESS_ADM,      /* deactivate */
     USIM_ACCESS_NEVER,    /* write */
     USIM_ACCESS_PIN,      /* update*/
     USIM_ACCESS_PIN       /* read*/
  },
  SIM_UICC_ACTIVATED_STATE, /*file State*/
  TRUE,                    /*sfi supported  */
  0x04,                     /*sfi*/
  TRUE                      /*Under USIM*/
};

/*--------------------------------------------------*
 * EF PLMN SEL: Preferred PLMN 6f60                 *
 * -------------------------------------------------*/
Int8    defUserPlmnSelUsimData[] = { 0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00,
                                     0xff, 0xff, 0xff, 0x00, 0x00 };

SimEfData    defUserPlmnSelUsimEfData =
{
    SIM_EMU_EF_PLMNW_ACT,               /* Id           */
    SIM_EFSTRUCT_T,                 /* Type         */
    SIM_DIR_ADF_USIM,               /* Dir          */
    1,                              /* Num recs      */
    SIM_EMU_SIZE_USER_PLMNWACT_FILE,   /* Rec len    */
    (0),                            /* curr Rec     */
    (0),
   {
    USIM_ACCESS_NEVER,    /* delete file*/
    USIM_ACCESS_NEVER,    /* terminate*/
    USIM_ACCESS_ADM,      /* activate */
    USIM_ACCESS_ADM,      /* deactivate */
    USIM_ACCESS_NEVER,    /* write */
    USIM_ACCESS_PIN,      /* update*/
    USIM_ACCESS_PIN       /* read*/
  },
    SIM_UICC_ACTIVATED_STATE, /*file State*/
    TRUE,                     /*sfi supported  */
    0x0d,                     /*sfi*/
    TRUE                      /*Under USIM*/
};

/*--------------------------------------------------*
 * EF OPLMNwAct: Operator controlled PLMN selector  *
 * with access technology      6f61                 *
 * -------------------------------------------------*/
Int8    defOperatorPlmnSelUsimData[] = { 0xff, 0xff, 0xff, 0x00, 0x00,
                                         0xff, 0xff, 0xff, 0x00, 0x00,
                                         0xff, 0xff, 0xff, 0x00, 0x00,
                                         0xff, 0xff, 0xff, 0x00, 0x00,
                                         0xff, 0xff, 0xff, 0x00, 0x00,
                                         0xff, 0xff, 0xff, 0x00, 0x00,
                                         0xff, 0xff, 0xff, 0x00, 0x00,
                                         0xff, 0xff, 0xff, 0x00, 0x00 };

SimEfData    defOperatorPlmnSelUsimEfData =
{
    SIM_EMU_EF_OPLMNW_ACT,               /* Id           */
    SIM_EFSTRUCT_T,                 /* Type         */
    SIM_DIR_ADF_USIM,               /* Dir          */
    1,                              /* Num recs      */
    SIM_EMU_SIZE_OPERATOR_PLMNWACT_FILE,   /* Rec len    */
    (0),                            /* curr Rec     */
    (0),
   {
    USIM_ACCESS_NEVER,    /* delete file*/
    USIM_ACCESS_NEVER,    /* terminate*/
    USIM_ACCESS_ADM,      /* activate */
    USIM_ACCESS_ADM,      /* deactivate */
    USIM_ACCESS_NEVER,    /* write */
    USIM_ACCESS_PIN,      /* update*/
    USIM_ACCESS_PIN       /* read*/
  },
    SIM_UICC_ACTIVATED_STATE, /*file State*/
    TRUE,                     /*sfi supported  */
    0x11,                     /*sfi*/
    TRUE                      /*Under USIM*/
};

/*--------------------------------------------------*
 * EF HPLMN SELECTOR WITH ACCESS TECHNOLOGY         *
 * See TS 131 102 section 4.2.54         6f62       *
 * -------------------------------------------------*/
Int8    defHplmnSelUsimData[] = { 0xff, 0xff, 0xff, 0x00, 0x00,
                                  0xff, 0xff, 0xff, 0x00, 0x00,
                                  0xff, 0xff, 0xff, 0x00, 0x00,
                                  0xff, 0xff, 0xff, 0x00, 0x00,
                                  0xff, 0xff, 0xff, 0x00, 0x00,
                                  0xff, 0xff, 0xff, 0x00, 0x00,
                                  0xff, 0xff, 0xff, 0x00, 0x00,
                                  0xff, 0xff, 0xff, 0x00, 0x00 };

SimEfData    defHplmnSelUsimEfData =
{
    SIM_EMU_EF_HPLMNW_ACT,               /* Id           */
    SIM_EFSTRUCT_T,                 /* Type         */
    SIM_DIR_ADF_USIM,               /* Dir          */
    1,                              /* Num recs      */
    SIM_EMU_SIZE_HPLMNWACT_FILE,   /* Rec len    */
    (0),                            /* curr Rec     */
    (0),
   {
    USIM_ACCESS_NEVER,    /* delete file*/
    USIM_ACCESS_NEVER,    /* terminate*/
    USIM_ACCESS_ADM,      /* activate */
    USIM_ACCESS_ADM,      /* deactivate */
    USIM_ACCESS_NEVER,    /* write */
    USIM_ACCESS_PIN,      /* update*/
    USIM_ACCESS_PIN       /* read*/
  },
    SIM_UICC_ACTIVATED_STATE, /*file State*/
    TRUE,                     /*sfi supported  */
    0x13,                     /*sfi*/
    TRUE                      /*Under USIM*/
};

/*----------------------------------------------------
 * EF FPLMN: Forbidden PLMN 6f7b
 *
 * see section 4.2.16  for coding (TS 131 102)
 * --------------------------------------------------*/
Int8    defFplmnUsimData[] = { 0x64, 0xf0, 0x00,      /*FPLMN 1.   (MCC = 234 MNC = 15) */
                               0xff, 0xff, 0xff,      /*FPLMN 2 */
                               0xff, 0xff, 0xff,      /*FPLMN 3 */
                               0xff, 0xff, 0xff };    /*FPLMN 4 */

SimEfData    defFplmnUsimEfData =
{
    SIM_EMU_EF_FPLMN,                   /* Id           */
    SIM_EFSTRUCT_T,                 /* Type         */
    SIM_DIR_ADF_USIM,               /* Dir          */
    1,                              /* Num recs      */
    SIM_EMU_SIZE_USIM_FPLMN_FILE,   /* Rec len    */
    (0),                            /* curr Rec     */
    (0),
   {
    USIM_ACCESS_NEVER,    /* delete file*/
    USIM_ACCESS_NEVER,    /* terminate*/
    USIM_ACCESS_ADM,      /* activate */
    USIM_ACCESS_ADM,      /* deactivate */
    USIM_ACCESS_NEVER,    /* write */
    USIM_ACCESS_PIN,      /* update*/
    USIM_ACCESS_PIN       /* read*/
  },
    SIM_UICC_ACTIVATED_STATE, /*file State*/
    TRUE,                     /*sfi supported  */
    0x0d,                     /*sfi*/
    TRUE                      /*Under USIM*/
};

/*---------------------------------------------------
 * EF AD  Administrative Data       6fad
 * for coding please refer to 131 102 section 4.2.18
 * --------------------------------------------------*/
Int8    defAdUsimData[] =  { MS_OPERN_TA,  /*UE operation mode*/
                             0, 0,  /*additional info*/
                             2};

SimEfData    defAdUsimEfData =
{
    SIM_EMU_EF_AD,              /* Id           */
    SIM_EFSTRUCT_T,             /* Type         */
    SIM_DIR_ADF_USIM,           /* Dir          */
    1,                          /* Num recs      */
    SIM_EMU_SIZE_USIM_AD_FILE,  /* Rec len     */
    (0),                        /* curr Rec     */
    (0),
    {
     USIM_ACCESS_NEVER,    /* delete file*/
     USIM_ACCESS_NEVER,    /* terminate*/
     USIM_ACCESS_ADM,      /* activate */
     USIM_ACCESS_ADM,      /* deactivate */
     USIM_ACCESS_NEVER,    /* write */
     USIM_ACCESS_PIN,      /* update*/
     USIM_ACCESS_ALWAYS    /* read*/
    },
    SIM_UICC_ACTIVATED_STATE, /*file State*/
    TRUE,                     /*sfi supported  */
    0x03,                     /*sfi*/
    TRUE                      /*Under USIM*/
};

/*EF OPL: this is part of EONS. see 51.011 for coding 6fc6*/
Int8    defOplUsimData[] = {
                             /* First record   */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record  2 */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record  3 */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record  4 */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record  5 */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record  6 */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record  7 */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record  8 */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record 9  */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record 10  */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record 11  */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record 12  */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record 13  */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record 14  */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             /* record 15  */
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
                             };

SimEfData    defOplUsimEfData =
{
    SIM_EMU_EF_OPL,                  /* Id           */
    SIM_EFSTRUCT_LF,                 /* Type         */
    SIM_DIR_ADF_USIM,                /* Dir          */
    SIM_EMU_NUM_USIM_OPL_RECS,       /* Num recs      */
    SIM_EMU_SIZE_USIM_OPL_FILE,      /* Rec len     */
    (0),                             /* curr Rec     */
    (0),
   {
     USIM_ACCESS_NEVER,    /* delete file */
     USIM_ACCESS_NEVER,    /* terminate */
     USIM_ACCESS_ADM,      /* activate */
     USIM_ACCESS_ADM,      /* deactivate */
     USIM_ACCESS_NEVER,    /* write */
     USIM_ACCESS_PIN,      /* update */
     USIM_ACCESS_PIN       /* read */
   },
   SIM_UICC_ACTIVATED_STATE, /*file State */
   TRUE,                     /*sfi supported  */
   0x1a,                     /*sfi */
   TRUE                      /*Under USIM */
};

/** NAS Config Information  (Transparent) 6fe8*/
                             /* Tag   Len   Val    */
Int8   defNasConfigData[]  = { 0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff,
                               0xff, 0xff, 0xff};

SimEfData defNasConfigEfData =
{
  SIM_EMU_EF_NAS_CONFIG,        /* Id                          */
  SIM_EFSTRUCT_T,            /* Type - transparent           */
  SIM_DIR_ADF_USIM,
  1,                          /* one record because transparent  */
  SIM_EMU_SIZE_NAS_CONFIG_FILE, /* total file size                 */
    (0),                          /* curr Rec                */
    (0),  {
     USIM_ACCESS_NEVER,    /* delete file */
     USIM_ACCESS_NEVER,    /* terminate   */
     USIM_ACCESS_ADM,      /* activate    */
     USIM_ACCESS_ADM,      /* deactivate  */
     USIM_ACCESS_NEVER,    /* write       */
     USIM_ACCESS_PIN,      /* update      */
     USIM_ACCESS_PIN       /* read        */
   },
   SIM_UICC_ACTIVATED_STATE, /*file State     */
   FALSE,                    /*sfi supported  */
   0,                        /*sfi            */
   TRUE                      /*Under USIM     */
};

/** auth  */
/*fixed values for response based upon what is expected by the tester */
Int8    defAuthSres[] = {0x01,0x22,0x47,0x64,0x8d,0xae,0xcb,0xe8};
Int8    defAuthCk[]   = {0x22,0x47,0x64,0x8d,0xae,0xcb,0xe8,0x09,0x2a,0x4f,0x6c,0x85,0xa6,0xc3,0xe0,0x01};
Int8    defAuthIk[]   = {0x47,0x64,0x8d,0xae,0xcb,0xe8,0x09,0x2a,0x4f,0x6c,0x85,0xa6,0xc3,0xe0,0x01,0x22};
#endif

#endif


#endif
/* END OF FILE */
