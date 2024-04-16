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
 * Handler for General AT Commands and related signals
 **************************************************************************/

#define MODULE_NAME "RVGNSS"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <cici_sig.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvgnss.h>
#include <rvgnut.h>
#include <rvgncclk.h>
#include <rvgncpb.h>
#include <rvgnsigi.h>
#include <vgmx_sig.h>
#include <abgl_sig.h>
#include <nvdm_modem.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  CiRunAtCommandInd      ciRunAtCommandInd;
};

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* General command information table */

static const AtCmdControl gnAtCommandTable[] =
{
  {ATCI_CONST_CHAR_STR "+CCLK",     vgGnCCLK,     VG_AT_GN_CCLK,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CEER",     vgGnCEER,     VG_AT_GN_CEER,     AT_CMD_ACCESS_CFUN_1},
  {ATCI_CONST_CHAR_STR "*MFTRCFG",  vgGnMFtrCfg,  VG_AT_GN_MFTRCFG,  AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CGMI",     vgGnGMI,      VG_AT_GN_GMI,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CGMM",     vgGnGMM,      VG_AT_GN_GMM,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CGMR",     vgGnGMR,      VG_AT_GN_GMR,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CGOI",     vgGnGOI,      VG_AT_GN_GOI,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CGSN",     vgGnCGSN,     VG_AT_GN_CGSN,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CIMI",     vgGnCIMI,     VG_AT_GN_CIMI,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
  {ATCI_CONST_CHAR_STR "*MLTS",     vgGnMLTS,     VG_AT_GN_MLTS,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MCGHWN",   vgGnMCGHWN,   VG_AT_GN_MCGHWN,   AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MCGSN",    vgGnMCGSN,    VG_AT_GN_MCGSN,    AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MOPTLOCK", vgGnMOPTLOCK, VG_AT_GN_MOPTLOCK, AT_CMD_ACCESS_NONE},
#if defined (FEA_PHONEBOOK)    
#if 0
  {ATCI_CONST_CHAR_STR "+CNUM",     vgGnCNUM,     VG_AT_GN_CNUM,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
#endif
  {ATCI_CONST_CHAR_STR "+CPBF",     vgGnCPBF,     VG_AT_GN_CPBF,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
  {ATCI_CONST_CHAR_STR "+CPBR",     vgGnCPBR,     VG_AT_GN_CPBR,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
  {ATCI_CONST_CHAR_STR "^SCPBR",    vgGnSCPBR,    VG_AT_GN_SCPBR,    AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
  {ATCI_CONST_CHAR_STR "+CPBS",     vgGnCPBS,     VG_AT_GN_CPBS,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
  {ATCI_CONST_CHAR_STR "+CPBW",     vgGnCPBW,     VG_AT_GN_CPBW,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
  {ATCI_CONST_CHAR_STR "^SCPBW",    vgGnSCPBW,    VG_AT_GN_SCPBW,    AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
  {ATCI_CONST_CHAR_STR "*MUPBSYNC", vgGnMUPBSYNC, VG_AT_GN_MUPBSYNC, AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
  {ATCI_CONST_CHAR_STR "*MUPBHKEY", vgMUpbhKey,   VG_AT_GN_MUPBHKEY, AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
  {ATCI_CONST_CHAR_STR "*MUPBCFG",  vgGnMupbcfg,  VG_AT_GN_MUPBCFG,  AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MUPBGAS",  vgGnMupbgas,  VG_AT_GN_MUPBGAS,  AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
  {ATCI_CONST_CHAR_STR "*MUPBAAS",  vgGnMupbaas,  VG_AT_GN_MUPBAAS,  AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
#endif /* FEA_PHONEBOOK */

  {ATCI_CONST_CHAR_STR "*MROUTEMMI",vgGnRouteMMI, VG_AT_GN_MROUTEMMI,AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MSPN",     vgGnMSPN,     VG_AT_GN_MSPN,     AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
  {ATCI_CONST_CHAR_STR "^SPN",      vgGnSPN,      VG_AT_GN_SPN,      AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
  {ATCI_CONST_CHAR_STR "*MUNSOL",   vgGnMUNSOL,   VG_AT_GN_MUNSOL,   AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MFASSERT", vgGnMFASSERT, VG_AT_GN_MFASSERT, AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MABORT",   vgGnMABORT,   VG_AT_GN_MABORT,   AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+FMI",      vgGnGMI,      VG_AT_GN_GMI,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+FMM",      vgGnGMM,      VG_AT_GN_GMM,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+FMR",      vgGnGMR,      VG_AT_GN_GMR,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+GCAP",     vgGnGCAP,     VG_AT_GN_GCAP,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+GMI",      vgGnGMI,      VG_AT_GN_GMI,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+GMM",      vgGnGMM,      VG_AT_GN_GMM,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+GMR",      vgGnGMR,      VG_AT_GN_GMR,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+GOI",      vgGnGOI,      VG_AT_GN_GOI,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+GSN",      vgGnGSN,      VG_AT_GN_GSN,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "^HVER",     vgGnHVER,     VG_AT_GN_HVER,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "I",         vgGnI,        VG_AT_GN_I,        AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "P",         vgGnP,        VG_AT_GN_P,        AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "T",         vgGnT,        VG_AT_GN_T,        AT_CMD_ACCESS_NONE},

/**********************************/
/* AT commands for NB-IOT project */
/**********************************/
/* NVRAM access */
  {ATCI_CONST_CHAR_STR "*MNVMQ",     vgGnMNVMQ,      VG_AT_GN_MNVMQ,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MNVMAUTH",  vgGnMNVMAUTH,   VG_AT_GN_MNVMAUTH,   AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MNVMW",     vgGnMNVMW,      VG_AT_GN_MNVMW,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MNVMR",     vgGnMNVMR,      VG_AT_GN_MNVMR,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MNVMGET",   vgGnMNVMGET,    VG_AT_GN_MNVMGET,    AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MNVMIVD",   vgGnMNVMIVD,    VG_AT_GN_MNVMIVD,    AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MNVMRSTONE",vgGnMNVMRSTONE, VG_AT_GN_MNVMRSTONE, AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MNVMRST",   vgGnMNVMRST,    VG_AT_GN_MNVMRST,    AT_CMD_ACCESS_NONE},
#if defined (FEA_MINI_DUMP)
  {ATCI_CONST_CHAR_STR "*MNVMMDNQ",  vgGnMNVMMDNQ,   VG_AT_GN_MNVMMDNQ,   AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MNVMMDR",   vgGnMNVMMDR,    VG_AT_GN_MNVMMDR,    AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MNVMMDC",   vgGnMNVMMDC,    VG_AT_GN_MNVMMDC,    AT_CMD_ACCESS_NONE},
#endif
  {ATCI_CONST_CHAR_STR "*MNVUID",   vgGnMNVUID,    VG_AT_GN_MNVUID,    AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MNVMOTPW",  vgGnMNVMOTPW,    VG_AT_GN_MNVMOTPW,    AT_CMD_ACCESS_NONE},    
  {ATCI_CONST_CHAR_STR "*MNVMOTPR",  vgGnMNVMOTPR,    VG_AT_GN_MNVMOTPR,    AT_CMD_ACCESS_NONE}, 

/* IDC RF Control */  
#if defined (ATCI_IDC_ENABLE)
  {ATCI_CONST_CHAR_STR "+IDCFREQ",      vgGnIDCFREQ,      VG_AT_GN_IDCFREQ,         AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+IDCPWRBACKOFF",vgGnIDCPWRBACKOFF,VG_AT_GN_IDCPWRBACKOFF,   AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+IDCTX2GPS",    vgGnIDCTX2GPS,    VG_AT_GN_IDCTX2GPS,       AT_CMD_ACCESS_NONE},
#endif
  {ATCI_CONST_CHAR_STR "+IDCTEST",      vgGnIDCTEST,      VG_AT_GN_IDCTEST,         AT_CMD_ACCESS_NONE},

  {ATCI_CONST_CHAR_STR "*MCAL",         vgGnMCAL,       VG_AT_GN_MCAL,       AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MCALDEV",      vgGnMCALDEV,    VG_AT_GN_MCALDEV,    AT_CMD_ACCESS_NONE},
    
  {ATCI_CONST_CHAR_STR "*MICCID",       vgGnMICCID,     VG_AT_GN_MICCID,     AT_CMD_ACCESS_NONE},   
  {ATCI_CONST_CHAR_STR "*MHOMENW",      vgGnMHOMENW,    VG_AT_GN_MHOMENW,    AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MSPCHSC",      vgGnMSPCHSC,    VG_AT_GN_MSPCHSC,    AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MN1DEBUG",     vgGnMN1DEBUG,   VG_AT_GN_MN1DEBUG,   AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MBSC",         vgGnMBSC,       VG_AT_GN_MBSC,       AT_CMD_ACCESS_NONE},
	{ATCI_CONST_CHAR_STR "+CH", 		CH_AT_TEST, 	VG_AT_CH_TEST,		AT_CMD_ACCESS_NONE},
  {PNULL,                    PNULL,        VG_AT_LAST_CODE,   AT_CMD_ACCESS_NONE}
 };

/* Signal type to handling procedure table */

static const SignalCnfFunc signalFunc[] =
{
#if defined (FEA_PHONEBOOK)  
    { SIG_APEX_LM_DIALNUM_STATUS_CNF,        vgSigApexLmDialNumStatusCnf      },
    { SIG_APEX_LM_FIND_DIALNUM_CNF,          vgSigApexLmFindDialNumCnf        },
    { SIG_APEX_LM_READ_DIALNUM_CNF,          vgSigApexLmReadDialNumCnf        },
    { SIG_APEX_LM_WRITE_DIALNUM_CNF,         vgSigApexLmWriteDialNumCnf       },
    { SIG_APEX_LM_DELETE_DIALNUM_CNF,        vgSigApexLmDeleteDialNumCnf      },
    { SIG_APEX_LM_FIXED_DIAL_CNF,            vgSigApexLmFixedDialCnf          },
    { SIG_APEX_LM_BARRED_DIAL_CNF,           vgSigApexLmBarredDialCnf         },
    { SIG_APEX_LM_PHONEBOOK_STATUS_CNF,      vgSigApexLmPhoneBookStatusCnf    },
    { SIG_APEX_LM_HIDDEN_KEY_FUNCTION_CNF,   vgSigApexLmHiddenKeyFunctionCnf  },
    { SIG_APEX_LM_READ_GRP_CNF,              vgSigApexLmReadGrpCnf            },
    { SIG_APEX_LM_READ_GAS_CNF,              vgSigApexLmReadGasCnf            },
    { SIG_APEX_LM_LIST_GAS_CNF,              vgSigApexLmListGasCnf            },
    { SIG_APEX_LM_WRITE_GAS_CNF,             vgSigApexLmWriteGasCnf           },
    { SIG_APEX_LM_WRITE_GRP_CNF,             vgSigApexLmWriteGrpCnf           },
    { SIG_APEX_LM_READ_ANR_CNF,              vgSigApexLmReadAnrCnf            },
    { SIG_APEX_LM_WRITE_ANR_CNF,             vgSigApexLmWriteAnrCnf           },
    { SIG_APEX_LM_READ_SNE_CNF,              vgSigApexLmReadSneCnf            },
    { SIG_APEX_LM_WRITE_SNE_CNF,             vgSigApexLmWriteSneCnf           },
    { SIG_APEX_LM_READ_EMAIL_CNF,            vgSigApexLmReadEmailCnf          },
    { SIG_APEX_LM_WRITE_EMAIL_CNF,           vgSigApexLmWriteEmailCnf         },
    { SIG_APEX_LM_DELETE_GRP_CNF,            vgSigApexLmDeleteGrpCnf          },
    { SIG_APEX_LM_DELETE_ANR_CNF,            vgSigApexLmDeleteAnrCnf          },
    { SIG_APEX_LM_DELETE_SNE_CNF,            vgSigApexLmDeleteSneCnf          },
    { SIG_APEX_LM_DELETE_EMAIL_CNF,          vgSigApexLmDeleteEmailCnf        },
    { SIG_APEX_LM_DELETE_GAS_CNF,            vgSigApexLmDeleteGasCnf          },
    { SIG_APEX_LM_CLEAR_GAS_CNF,             vgSigApexLmClearGasCnf           },
    { SIG_APEX_LM_GET_SYNC_STATUS_CNF,       vgSigApexLmGetSyncStatusCnf      },
    { SIG_APEX_LM_READ_RECORD_UID_CNF,       vgSigApexLmReadRecordUidCnf      },
    { SIG_APEX_LM_READY_IND,                 vgSigApexLmReadyIndLocal         },
    { SIG_APEX_LM_RECORD_CHANGED_IND,        vgSigApexLmRecordChangedInd      },
    { SIG_APEX_LM_READ_AAS_CNF,              vgSigApexLmReadAasCnf            },
    { SIG_APEX_LM_LIST_AAS_CNF,              vgSigApexLmListAasCnf            },
    { SIG_APEX_LM_WRITE_AAS_CNF,             vgSigApexLmWriteAasCnf           },
    { SIG_APEX_LM_DELETE_AAS_CNF,            vgSigApexLmDeleteAasCnf          },
    { SIG_APEX_LM_CLEAR_AAS_CNF,             vgSigApexLmClearAasCnf           },
    { SIG_APEX_LM_GET_ALPHA_CNF,             vgSigApexLmGetAlphaCnf           },
#endif /* FEA_PHONEBOOK */

    { SIG_APEX_SIM_CHV_FUNCTION_CNF,         vgSigApexSimChvFunctionCnf       },

#if defined (FEA_SIMLOCK)      
    { APEX_SIM_MEP_CNF,                      vgSigApexSimMepCnf               },
    { APEX_SIM_MEP_STATUS_CNF,               vgSigApexSimMepStatusCnf         },
#if defined(FEA_NOT_SLIM_SIM_CODE)
    { SIG_APEX_SIM_WRITE_MEP_NETWORK_ID_CNF, vgSigApexSimWriteMepNetworkIdCnf },
    { SIG_APEX_SIM_READ_MEP_NETWORK_ID_CNF,  vgSigApexSimReadMepNetworkIdCnf  },
#endif
#endif /* FEA_SIMLOCK */
    { SIG_APEX_MM_WRITE_MOBILE_ID_CNF,       vgSigApexMmWriteMobileIdCnf      },
    { SIG_APEX_MM_READ_MOBILE_ID_CNF,        vgSigApexMmReadMobileIdCnf       },
    { APEX_SIM_READ_SPN_CNF,                 vgSigApexSimReadSpnCnf           },
    { SIG_APEX_SIM_PIN_FUNCTION_CNF,         vgSigApexSimPinFunctionCnf       },
    { SIG_APEXGL_WRITE_FEATURE_CONFIG_CNF,   vgSigApexGlWriteFeatureConfigCnf },
    { SIG_N1CD_ENTER_CNF,                    vgSigN1CdEnterCnf                },
    { SIG_N1CD_EXIT_CNF,                     vgSigN1CdExitCnf                 },
    { SIG_N1CD_NRF_TEST_CNF,                 vgSigN1CdRfTestCnf               },
    { SIG_N1CD_IDC_TEST_CNF,                 vgSigN1CdIdcTestCnf              }
};

#define NUM_GN_SIG_FUNC (sizeof(signalFunc) / sizeof(SignalCnfFunc))

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static void initialiseGnssGenericData (void);

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

 /****************************************************************************
 *
 * Function:    initialiseGnss
 *
 * Parameters:  none
 *
 * Returns:     Nothing
 *
 * Description: initiates general sub-system entity specific data
 *
 ****************************************************************************/
void initialiseGnss (const VgmuxChannelNumber entity)
{
  GeneralContext_t  *generalContext_p;

#if defined (FEA_PHONEBOOK)
  Int8              callIndex;
#endif /* FEA_PHONEBOOK */

  /* initialise general context data for all entities */
  generalContext_p = ptrToGeneralContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
#if defined (FEA_PHONEBOOK)
  generalContext_p->vgLmData.phoneBookOperation         = VG_PB_INVALID;
  generalContext_p->vgLmData.phoneIndex1                = NULL_CHAR;
  generalContext_p->vgLmData.phoneIndex2                = NULL_CHAR;
  generalContext_p->vgLmData.phoneBookNumType           = NUM_TYPE_NATIONAL;
  generalContext_p->vgLmData.phoneBookNumTypePresent    = TRUE;
  generalContext_p->vgLmData.readMode                   = LM_READ_ABSOLUTE;
  generalContext_p->vgLmData.writeMode                  = LM_WRITE_ABSOLUTE;
  generalContext_p->vgLmData.phoneBook                  = DIAL_LIST_ADN_GLB;
  generalContext_p->vgLmData.oldPhoneBook               = DIAL_LIST_ADN_GLB;
  generalContext_p->vgLmData.phoneBookIndex             = 5;  /* must match index of phoneBook in vgLmInfo table */
  generalContext_p->vgLmData.hiddenEntry                = FALSE;
  generalContext_p->vgLmData.secondName.length          = 0;
  generalContext_p->vgLmData.emailInfo.email.length        = 0;
  generalContext_p->vgLmData.startRecord                = 0;
  generalContext_p->vgLmData.adNumInfo.adNum.dialNum[0] = NULL_CHAR;
  generalContext_p->vgLmData.adNumInfo.adNum.dialNumLength  = 0;
  generalContext_p->vgLmData.adNumInfo.adNum.dialNumType    = NUM_TYPE_NATIONAL;
  generalContext_p->vgLmData.missingParamCount          = 0;
  generalContext_p->vgLmData.grpInfo.grpData.numOfGrp   = 0;
  generalContext_p->vgLmData.grpInfo.grpAlphaId.length  = 0;
  generalContext_p->vgLmData.grpInfo.groupSupported     = FALSE;
  generalContext_p->vgLmData.grpInfo.gasFreeRec         = 0;
  generalContext_p->vgLmData.grpInfo.gasNumRecords      = 0;
  generalContext_p->vgLmData.needOldPhoneBook           = FALSE;
  generalContext_p->vgLmData.firstDialNumRecordFromList = TRUE;
  generalContext_p->vgLmData.alphaLength                = 0;
  generalContext_p->vgLmData.fixedDialNumLength         = 0;
  generalContext_p->vgLmData.fixedDialNumType           = NUM_TYPE_UNKNOWN;
  generalContext_p->vgLmData.writeNumLength             = 0;
  generalContext_p->vgLmData.moreRecordsToRead          = FALSE;

  generalContext_p->vgLmData.vgCpbsData.operation       = INVALID_EXTENDED_OPERATION;
  generalContext_p->vgLmData.vgCpbsData.phoneBook       = DIAL_LIST_NULL;
  generalContext_p->vgLmData.vgCpbsData.phoneBookIndex  = 0;
  generalContext_p->vgLmData.vgCpbsData.passwordPresent = FALSE;
  generalContext_p->vgLmData.vgCpbsData.passwordLength  = 0;
  generalContext_p->vgLmData.vgCpbsData.password[0]     = NULL_CHAR;
  generalContext_p->vgLmData.vgCpbsData.iciType         = LM_ICI_ALL;
  generalContext_p->enableFdn        = TRUE;
  generalContext_p->enableBdn        = FALSE;
  generalContext_p->updatingBdn      = FALSE;

  /* Initialise alpha lookup structure.... */
  for (callIndex = 0; callIndex < VG_MAX_USER_CALL_ID; callIndex++)
  {
    generalContext_p->vgAlphaLookup[callIndex].active = FALSE;
  }

  generalContext_p->pendingAlphaReq     = 0;

  generalContext_p->vgLmData.vgMupbsyncContext.operation    = EXTENDED_RANGE;
  generalContext_p->vgLmData.vgMupbsyncContext.uidReadMode  = LM_READ_ABSOLUTE;
  generalContext_p->vgLmData.vgMupbsyncContext.uidIndex     = 0;
  generalContext_p->vgLmData.vgMupbsyncContext.pbCapacity   = 0;

#endif /* FEA_PHONEBOOK */

  generalContext_p->password[0]      = NULL_CHAR;

  generalContext_p->cgsnSnt = VG_CGSN_SNT_SN;
  
  generalContext_p->mcgsnData.mcgsnSnt           = VG_MCGSN_SNT_SN;
  generalContext_p->mcgsnData.digitImeiArraySize = 0;
  generalContext_p->mcgsnData.digitSNArraySize   = 0;
  
  generalContext_p->omitFirstNewLine    = FALSE;
}

 /****************************************************************************
 *
 * Function:    initialiseGnssGenericData
 *
 * Parameters:  none
 *
 * Returns:     Nothing
 *
 * Description: initiates general sub-system entity Generic data
 *
 ****************************************************************************/
static void initialiseGnssGenericData (void)
{
    GeneralGenericContext_t    *generalGenericContext_p  = ptrToGeneralGenericContext();

    
    /* MFTRCFG command*/
    generalGenericContext_p->vgMFtrCfgData.initialised                          = FALSE;
    generalGenericContext_p->vgMFtrCfgData.rebootInfo.modemModeNR               = FALSE;
    generalGenericContext_p->vgMFtrCfgData.rebootInfo.meLocationNR              = FALSE;
    memset( &generalGenericContext_p->vgMFtrCfgData.nvramCfgVar,
            0, sizeof( AbglFeatureConfigDataArea));
    memset( &generalGenericContext_p->vgMFtrCfgData.currentCfgVar,
            0, sizeof( AbglFeatureConfigDataArea));

    generalGenericContext_p->initDataFromABPDState      = WAITING_FOR_DEFAULT_APN;

    /*******************************/
    /* Context data for NB-IOT     */
    /*******************************/
    generalGenericContext_p->vgMnvmMcalContext.vgCaldevStatus = VG_CALDEV_DISABLED;

#if defined (MTK_NVDM_MODEM_ENABLE)
    generalGenericContext_p->vgMnvmMcalContext.info_list_p_normal = PNULL;
    generalGenericContext_p->vgMnvmMcalContext.info_list_p_protected = PNULL;
#endif
#if !defined (MTK_TOOL_AUTHENTICATION_ENABLE) && !defined (ENABLE_ATCI_UNIT_TEST)
    generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised = TRUE;
#else
    generalGenericContext_p->vgMnvmMcalContext.vgMnvmAccessAuthorised = FALSE;
#endif
    generalGenericContext_p->vgMnvmMcalContext.nextDataItemNumberToSend = 0;
    generalGenericContext_p->vgMnvmMcalContext.totalDataItemsToSend = 0;
    generalGenericContext_p->vgMnvmMcalContext.normalDataItems = 0; 
    generalGenericContext_p->vgMnvmMcalContext.protectedDataItems = 0;    
    generalGenericContext_p->vgMnvmMcalContext.currentNvmChannel = VGMUX_CHANNEL_INVALID;
    memset((Int8*)&(generalGenericContext_p->vgMnvmMcalContext.vgMnvmMcalContextUnion), 0 , sizeof (VgMnvmMcalContextUnion));

    /* *MSPCHSC command*/
   generalGenericContext_p->vgNvmMspchscContext.curNpbchSymbolRotationMode      = PROF_MSPCHSC_NEW_SCRAMBLE;
   generalGenericContext_p->vgNvmMspchscContext.NpbchSymbolRotationModeInitDone = FALSE;
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

 /****************************************************************************
 *
 * Function:    vgGnssInterfaceController
 *
 * Parameters:  SignalBuffer       - structure containing incoming signal
 *              VgmuxChannelNumber - mux channel number
 *
 * Returns:     Boolean - indicates whether the sub-system has recognised and
 *                        procssed the signal given.
 *
 * Description: determines action for received signals
 *
 ****************************************************************************/

Boolean vgGnssInterfaceController (const SignalBuffer *signal_p,
                                    const VgmuxChannelNumber entity)
{
  Boolean accepted = FALSE;
  Int8    fIndex;


  /* Signal Handler */

  switch (*signal_p->type)
  {
    case SIG_CI_RUN_AT_COMMAND_IND:
    {
      accepted = parseCommandBuffer (gnAtCommandTable, entity);
      break;
    }
    case SIG_CIMUX_CHANNEL_ENABLE_IND:
    {
      initialiseGnss (entity);
      break;
    }
    case SIG_INITIALISE:
    {

      initialiseGnssGenericData ();
      break;
    }
    case SIG_DM_RTC_DATE_AND_TIME_IND:
    {
      vgSigAclkDateAndTimeInd (signal_p);
      accepted = TRUE;
      break;
    }
    
    case SIG_CIMUX_AT_DATA_CNF:
    {
      vgSendNextMnvmgetDataItemToMux (entity);
      /* Do not accept here - as CRMAN also needs it
       * accepted = TRUE;
       */
      break;
    }
    default:
    {
      for (fIndex = 0;
           (fIndex < NUM_GN_SIG_FUNC) && (accepted == FALSE);
             fIndex++)
      {
        if (signalFunc[fIndex].signalId == (*signal_p->type))
        {
          (signalFunc[fIndex].procFunc)(signal_p, entity);
          accepted = TRUE;
        }
      }
      break;
    }
  }
  return (accepted);

}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

