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
 * RVCHMAN.C
 * Change control manager interface controller module
 **************************************************************************/

#define MODULE_NAME "RVCHMAN"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <rvsystem.h>
#include <rvchman.h>
#include <rvchsigo.h>
#include <rvchsigi.h>
#include <rvgnsigo.h>
#include <rvpdsigo.h>
#include <rvmmsigo.h>
#if defined (ENABLE_AT_ENG_MODE)
#include <rvemsigo.h>
#endif
#include <rvmssigo.h>
#include <rvoman.h>
#include <rvslsigo.h>
#include <rvtsut.h>
#include <rvomtime.h>
#include <rvcimxsot.h>
#include <rvccsigi.h>
#include <rvgncpb.h>
#include <rvcrhand.h>
#include <rvcimxut.h>
#include <rvgnsigi.h>
#include <rvchcut.h>
#include <pssignal.h>
#include <rvgput.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static ResultCode_t vgPostApexSimGetChvRsp          (const VgmuxChannelNumber entity,
                                                      const Boolean successful);
static ResultCode_t vgPostApexMmPlmnSelectReq       (const VgmuxChannelNumber entity,
                                                      const Boolean successful);
#if defined (FEA_PHONEBOOK)
static ResultCode_t vgPostApexLmGetAlphaReq         (const VgmuxChannelNumber entity,
                                                      const Boolean successful);
#endif
static ResultCode_t vgPostApexAbpdResetCounterReq   (const VgmuxChannelNumber entity,
                                                      const Boolean successful);
static ResultCode_t vgPostApexAbpdReportCounterReq  (const VgmuxChannelNumber entity,
                                                      const Boolean successful);
static ResultCode_t vgPostApexAbpdHangupReq         (const VgmuxChannelNumber entity,
                                                      const Boolean successful);
static ResultCode_t vgPostApexAbpdDialReq           (const VgmuxChannelNumber entity,
                                                      const Boolean successful);
static ResultCode_t vgPostApexAbpdActivateDataConnReq (const VgmuxChannelNumber entity,
                                                        const Boolean successful);

/* added for job119076 */
static ResultCode_t vgPostApexMmPlmnListReq         (const VgmuxChannelNumber entity,
                                                      const Boolean successful);


static void initialiseChss (void);

/***************************************************************************
 * Types
 ***************************************************************************/

typedef void (*VgChSigReq)(const VgmuxChannelNumber  entity);

typedef struct ContinueActionTag
{
  SignalId                signalId;
  VgChSigReq              procFunc;
} ContinueAction;

typedef ResultCode_t (*VgChPostSigReq)(const VgmuxChannelNumber entity, const Boolean successful);

typedef struct PostActionTag
{
  SignalId                signalId;
  VgChPostSigReq          procFunc;
} PostAction;

typedef struct SignalBaseToElementTag
{
  SignalInterfaceBases    base;
  VgChangeControlElements element;
} SignalBaseToElement;

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* the table below matches up the signal type to the relevant target background
 * layer procedure and the procedure to call to send the signal when control
 * has been given */

static const ContinueAction continueAction[] =
{

  /* CC_CALL_CONTROL */
  
  /* None */

  /* CC_MOBILITY_MANAGEMENT */
  { SIG_APEX_MM_WRITE_PLMN_SEL_REQ,      vgSigApexMmWritePlmnSelReq     },
  { SIG_APEX_MM_PLMNLIST_REQ,            vgSigApexMmPlmnListReq         },
  { SIG_APEX_MM_ABORT_PLMNLIST_REQ,      vgSigApexMmAbortPlmnListReq    },
  { SIG_APEX_MM_PLMN_SELECT_REQ,         vgSigApexMmPlmnSelectReq       },
  { SIG_APEX_MM_DEREGISTER_REQ,          vgSigApexMmDeregisterReq       },
  { SIG_APEX_MM_READ_BAND_MODE_REQ,      vgApexMmReadBandModeReq        },
  { SIG_APEX_MM_WRITE_PWON_OPTIONS_REQ,  vgApexMmWritePwonOptionsReq    },
  { SIG_APEX_MM_READ_PWON_OPTIONS_REQ,   vgApexMmReadPwonOptionsReq     },
#if defined(UPGRADE_MTNET)
  { SIG_APEX_MM_SUSPEND_REQ,             vgApexMmSuspendReq             },
  { SIG_APEX_MM_RESUME_REQ,              vgApexMmResumeReq              },
#endif
  { SIG_APEX_MM_LOCK_ARFCN_REQ,          vgApexMmLockArfcnReq     },
  { SIG_APEX_MM_SEARCH_BAND_LIST_REQ,     vgApexMmSearchBandListReq},
  { SIG_APEX_MM_UE_STATS_REQ,             vgApexMmUeStatsReq     },
  { SIG_APEX_MM_LOC_CELL_INFO_REQ,        vgApexMmLocCellInfoReq         },
  { SIG_APEX_MM_SET_EHPLMN_REQ,           vgApexMmSetEhplmnReq           },  
  { SIG_APEX_MM_DISABLE_HPLMN_SEARCH_REQ, vgApexMmDisableHplmnSearchReq  },  
/* NB-IOT SPECIFICS */
  { SIG_APEX_MM_SET_EDRX_REQ,             vgApexMmSetEdrxReq             },
  { SIG_APEX_MM_READ_EDRX_REQ,            vgApexMmReadEdrxReq            },
  { SIG_APEX_WRITE_IOT_OPT_CFG_REQ,       vgApexWriteIotOptCfgReq        },
  { SIG_APEX_READ_IOT_OPT_CFG_REQ,        vgApexReadIotOptCfgReq         },
  { SIG_APEX_READ_PSM_CONF_REQ,           vgApexReadPsmConfReq           },
  { SIG_APEX_WRITE_PSM_CONF_REQ,          vgApexWritePsmConfReq          },
  { SIG_APEX_MM_READ_ATTACH_PDN_CONF_REQ, vgApexReadAttachPdnConfReq     },
  { SIG_APEX_MM_WRITE_ATTACH_PDN_CONF_REQ,vgApexWriteAttachPdnConfReq    },
#if defined (FEA_NFM)
  { SIG_APEX_READ_NFM_REQ,                vgApexReadNfmReq               },
  { SIG_APEX_WRITE_NFM_REQ,               vgApexWriteNfmReq              },
  { SIG_APEX_READ_NFM_CONF_REQ,           vgApexReadNfmConfReq           },
  { SIG_APEX_WRITE_NFM_CONF_REQ,          vgApexWriteNfmConfReq          },
  { SIG_APEX_READ_NFM_STATUS_REQ,         vgApexReadNfmStatusReq         },
#endif
#if defined (FEA_RPM)
  { SIG_APEX_RPM_READ_INFO_REQ,           vgApexReadRpmReq               }, 
#endif
  { SIG_APEX_MM_READ_MOBILE_ID_REQ,       vgSigApexMmReadMobileIdReq    },
  { SIG_APEX_MM_WRITE_MOBILE_ID_REQ,      vgSigApexMmWriteMobileIdReq    },
  /* CC_ENGINEERING_MODE */
#if defined (ENABLE_AT_ENG_MODE)
  { SIG_APEX_EM_INFO_REQ,                vgApexEmInfoReq                },
  { SIG_APEX_EM_FORCE_CMD_REQ,           vgApexEmForceCmdReq            },
#endif
  /* CC_POWER_MANAGEMENT */

  { SIG_APEX_PM_MODE_CHANGE_REQ,      vgSigApexPmModeChangeReq },

  /* CC_SUBSCRIBER_IDENTITY_MODULE */

  { SIG_APEX_SIM_CHV_FUNCTION_REQ,    vgSigApexSimChvFunctionReq },
  { SIG_APEX_SIM_PIN_FUNCTION_REQ,    vgSigApexSimPinFunctionReq },
  { APEX_SIM_READ_SPN_REQ,            vgSigApexSimReadSpnReq     },

#if defined (FEA_SIMLOCK)    
  { APEX_SIM_MEP_REQ,                 vgSigApexSimMepReq         },
  { APEX_SIM_MEP_STATUS_REQ,          vgSigApexSimMepStatusReq   },
#endif /* FEA_SIMLOCK */

  { SIG_APEX_SIM_GET_CHV_RSP,         vgSigApexSimGetChvRsp      },
  { SIG_APEX_SIM_GET_PIN_RSP,         vgSigApexSimGetPinRsp      },
  { SIG_APEX_SIM_GEN_ACCESS_REQ,      vgSigApexSimGenAccessReq   },
  { APEX_SIM_LOGICAL_CHANNEL_ACCESS_REQ,vgSigApexSimLogicalChannelAccessReq},
  { SIG_APEX_SIM_READ_DIR_REQ,        vgSigApexSimReadDirReq     },
  { SIG_APEX_SIM_USIM_APP_START_REQ,  vgSigApexSimUsimAppStartReq},
  { SIG_APEX_SIM_CSIM_LOCK_REQ,             vgSigApexSimCsimLockReq          },
#if defined (ENABLE_DUAL_SIM_SOLUTION)
  { SIG_APEX_SIM_SELECT_REQ,                vgSigApexSimSelectReq },
#endif
  { APEX_SIM_LIST_PNN_REQ,                  vgSigApexListPnnReq      },
  { APEX_SIM_LIST_OPL_REQ,                  vgSigApexListOplReq      },
  { APEX_SIM_READ_SIM_PARAM_REQ,            vgSigApexReadSimParamReq        },
#if defined(FEA_TEMP_INCLUDE_SIM_EMULATION) || defined (FEA_TEMP_ESIM_EMULATION)
  { SIG_APEX_SIM_EMUSIM_REQ,                vgSigApexEmuSimCfgReq    },
#endif
#if defined (SIM_EMULATION_ON)
  { SIG_ALSI_WRITE_USIM_EMU_FILE_REQ,       vgSigAlsiWriteUsimEmuFileReq },
#endif /* SIM_EMULATION_ON */

  { APEX_SIM_OPEN_LOGICAL_CHANNEL_REQ,        vgSigApexSimOpenLogicalChannelReq },
  { APEX_SIM_CLOSE_LOGICAL_CHANNEL_REQ,       vgSigApexSimCloseLogicalChannelReq },
  { SIG_APEX_SIM_READ_MSISDN_REQ,    vgSigApexSimReadMsisdnReq },


#if defined (FEA_PHONEBOOK)
  /* CC_LIST_MANAGEMENT */

  { SIG_APEX_LM_DIALNUM_STATUS_REQ,     vgSigApexLmDialNumStatusReq },
  { SIG_APEX_LM_READ_DIALNUM_REQ,       vgSigApexLmReadDialNumReq   },
  { SIG_APEX_LM_WRITE_DIALNUM_REQ,      vgSigApexLmWriteDialNumReq  },
  { SIG_APEX_LM_DELETE_DIALNUM_REQ,     vgSigApexLmDeleteDialNumReq },
  { SIG_APEX_LM_FIND_DIALNUM_REQ,       vgSigApexLmFindDialNumReq   },
  { SIG_APEX_LM_FIXED_DIAL_REQ,         vgSigApexLmFixedDialReq     },
  { SIG_APEX_LM_BARRED_DIAL_REQ,        vgSigApexLmBarredDialReq    },
  { SIG_APEX_LM_GET_ALPHA_REQ,          vgSigApexLmGetAlphaReq      },
  { SIG_APEX_LM_PHONEBOOK_STATUS_REQ,   vgSigApexLmPhoneBookStatusReq   },
  { SIG_APEX_LM_HIDDEN_KEY_FUNCTION_REQ,vgSigApexLmHiddenKeyFunctionReq },
  { SIG_APEX_LM_READ_GRP_REQ,           vgSigApexLmReadGrpReq         },
  { SIG_APEX_LM_READ_GAS_REQ,           vgSigApexLmReadGasReq         },
  { SIG_APEX_LM_READ_ANR_REQ,           vgSigApexLmReadAnrReq         },
  { SIG_APEX_LM_LIST_GAS_REQ,           vgSigApexLmListGasReq         },
  { SIG_APEX_LM_WRITE_GAS_REQ,          vgSigApexLmWriteGasReq        },
  { SIG_APEX_LM_WRITE_GRP_REQ,          vgSigApexLmWriteGrpReq        },
  { SIG_APEX_LM_WRITE_ANR_REQ,          vgSigApexLmWriteAnrReq        },
  { SIG_APEX_LM_WRITE_SNE_REQ,          vgSigApexLmWriteSneReq        },
  { SIG_APEX_LM_READ_SNE_REQ,           vgSigApexLmReadSneReq         },
  { SIG_APEX_LM_WRITE_EMAIL_REQ,        vgSigApexLmWriteEmailReq      },
  { SIG_APEX_LM_READ_EMAIL_REQ,         vgSigApexLmReadEmailReq       },
  { SIG_APEX_LM_DELETE_GRP_REQ,         vgSigApexLmDeleteGrpReq       },
  { SIG_APEX_LM_DELETE_GAS_REQ,         vgSigApexLmDeleteGasReq       },
  { SIG_APEX_LM_CLEAR_GAS_REQ,          vgSigApexLmClearGasReq        },
  { SIG_APEX_LM_DELETE_ANR_REQ,         vgSigApexLmDeleteAnrReq       },
  { SIG_APEX_LM_DELETE_SNE_REQ,         vgSigApexLmDeleteSneReq       },
  { SIG_APEX_LM_DELETE_EMAIL_REQ,       vgSigApexLmDeleteEmailReq     },
  { SIG_APEX_LM_GET_PB_SYNC_INFO_REQ,   vgSigApexLmGetPbSyncInfoReq   },
  { SIG_APEX_LM_GET_SYNC_STATUS_REQ,    vgSigApexLmGetSyncStatusReq   },
  { SIG_APEX_LM_READ_RECORD_UID_REQ,    vgSigApexLmReadRecordUidReq   },
  { SIG_APEX_LM_READ_AAS_REQ,           vgSigApexLmReadAasReq         },
  { SIG_APEX_LM_WRITE_AAS_REQ,          vgSigApexLmWriteAasReq        },
  { SIG_APEX_LM_DELETE_AAS_REQ,         vgSigApexLmDeleteAasReq       },
  { SIG_APEX_LM_LIST_AAS_REQ,           vgSigApexLmListAasReq         },
  { SIG_APEX_LM_CLEAR_AAS_REQ,          vgSigApexLmClearAasReq        },
#endif /* FEA_PHONEBOOK */

  /* CC_ENGINEERING_MODE */

  { SIG_APEX_EM_PLMN_TEST_REQ,          vgSigApexEmPlmnTestReq        },

  /* CC_GENERAL */
  { SIG_APEXGL_WRITE_FEATURE_CONFIG_REQ,    vgSigApexGlWriteFeatureConfigReq    },

  /* CC_GENERAL_PACKET_RADIO_SYSTEM */

  { SIG_APEX_ABPD_PSD_ATTACH_REQ,     vgApexAbpdPsdAttachReq     },
  { SIG_APEX_ABPD_PSD_DETACH_REQ,     vgApexAbpdPsdDetachReq     },

  { SIG_APEX_ABPD_DIAL_REQ,           vgApexAbpdDialReq          },
  { SIG_APEX_ABPD_ACTIVATE_DATA_CONN_REQ,
                                      vgApexAbpdActivateDataConnReq },
  { SIG_APEX_ABPD_CONNECT_RSP,        vgApexAbpdConnectRsp       },
  { SIG_APEX_ABPD_CONNECT_REJ,        vgApexAbpdConnectRej       },
  { SIG_APEX_ABPD_HANGUP_REQ,         vgApexAbpdHangupReq        },

#if defined (FEA_QOS_TFT)
  { SIG_APEX_ABPD_PSD_MODIFY_REQ,     vgApexAbpdPsdModifyReq     },
#endif

#if defined (FEA_MT_PDN_ACT)
  { SIG_APEX_ABPD_SETUP_RSP,          vgApexAbpdSetupRsp         },
#endif /* FEA_MT_PDN_ACT */

  { SIG_APEX_ABPD_APN_READ_REQ,       vgApexAbpdApnReadReq       },
  { SIG_APEX_ABPD_APN_WRITE_REQ,      vgApexAbpdApnWriteReq      },


  { SIG_APEX_ABPD_REPORT_COUNTER_REQ, vgApexAbpdReportCounterReq },
  { SIG_APEX_ABPD_RESET_COUNTER_REQ,  vgApexAbpdResetCounterReq  },

#if defined (FEA_UPDIR)
  { SIG_APEX_ABPD_SET_UPDIR_INFO_REQ, vgApexAbpdSetUpdirInfoReq  },
  { SIG_APEX_ABPD_RESET_UPDIR_INFO_REQ,vgApexAbpdResetUpdirInfoReq},
#endif

#if defined (FEA_ACL)
  { SIG_APEX_ABPD_SET_ACL_REQ,        vgApexAbpdSetAclReq        },
  { SIG_APEX_ABPD_LIST_ACL_REQ,       vgApexAbpdListAclReq       },
  { SIG_APEX_ABPD_WRITE_ACL_REQ,      vgApexAbpdWriteAclReq      },
  { SIG_APEX_ABPD_DELETE_ACL_REQ,     vgApexAbpdDeleteAclReq     },
  { SIG_APEX_ABPD_ACL_STATUS_REQ,     vgApexAbpdAclStatusReq     },
#endif
  { SIG_APEX_ABPD_WRITE_REL_ASSIST_REQ, vgApexAbpdWriteRelAssistReq},
  { SIG_APEX_ABPD_READ_REL_ASSIST_REQ, vgApexAbpdReadRelAssistReq}, 
  { SIG_APEX_ABPD_READ_APN_DATA_TYPE_REQ, vgApexAbpdReadApnDataTypeReq},
  { SIG_APEX_ABPD_WRITE_APN_DATA_TYPE_REQ,vgApexAbpdWriteApnDataTypeReq},
  { SIG_APEX_ABPD_TRANSMIT_DATA_IND,  vgApexAbpdTransmitDataInd},

  /* CC_SHORT_MESSAGE_SERVICE */

  { SIG_APEX_SM_READ_ROUTE_REQ,       vgApexSmReadRouteReq         },
  { SIG_APEX_SM_WRITE_ROUTE_REQ,      vgApexSmWriteRouteReq        },
  { SIG_APEX_SM_DELETE_REQ,           vgSigApexSmDeleteReq         },
  { SIG_APEX_SM_DELETE_SMSR_REQ,      vgSigApexSmDeleteSmsrReq     },
  { SIG_APEX_SM_READ_REQ,             vgSigApexSmReadReq           },
  { SIG_APEX_SM_READ_SMSR_REQ,        vgSigApexSmReadSmsrReq       },
  { SIG_APEX_SM_READ_SMSP_REQ,        vgSigApexSmReadSmspReq       },
  { SIG_APEX_SM_WRITE_SMSP_REQ,       vgSigApexSmWriteSmspReq      },
  { SIG_APEX_SM_SEND_REQ,             vgSigApexSmSendReq           },
  { SIG_APEX_SM_STORE_REQ,            vgSigApexSmStoreReq          },
  { SIG_APEX_SM_DELIVERY_WITH_REPORT_RSP, vgApexSendSmDeliveryWithReportRsp},
  { SIG_APEX_SM_SEND_FROM_SIM_REQ,    vgSigApexSmSendFromSimReq    },
  { SIG_APEX_SM_SEND_MORE_REQ,        vgSigApexSmSendMoreReq       },
  { SIG_APEX_SM_STATUS_REQ,           vgSigApexSmStatusReq         },
  { SIG_APEX_SM_COMMAND_REQ,          vgSigApexSmCommandReq        },
  { SIG_APEX_SM_READ_NODATA_REQ,      vgSigApexSmReadNoDataReq     },
  { SIG_APEX_SM_READ_SMSR_NODATA_REQ, vgSigApexSmReadSmsrNoDataReq },
  { SIG_APEX_SM_STORE_LOC_REQ,        vgSigApexSmStoreLocReq       },
  { SIG_APEX_SM_SET_LOC_STATUS_REQ,   vgSigApexSmSetLocStatusReq   },
  { SIG_APEX_SM_SMSR_STORE_RSP,       vgSigApexSmSmsrStoreRsp      },

  /* CC_SUPPLEMENTARY_SERVICES */
  /* No call related SS for NB-IOT */
};

#define NUM_CONTINUE_ACTIONS (sizeof(continueAction) / sizeof(ContinueAction))

/* the table below matches the signal type to a procedure which is run after
   the signal has been sent, or when we know we can't get control */

static const PostAction postAction[] =
{
  { SIG_APEX_SIM_GET_CHV_RSP,         vgPostApexSimGetChvRsp         },
  { SIG_APEX_SIM_GET_PIN_RSP,         vgPostApexSimGetChvRsp         },
  { SIG_APEX_MM_PLMN_SELECT_REQ,      vgPostApexMmPlmnSelectReq      },

#if defined (FEA_PHONEBOOK)    
  { SIG_APEX_LM_GET_ALPHA_REQ,        vgPostApexLmGetAlphaReq        },
#endif /* FEA_PHONEBOOK */
#if defined (ATCI_SLIM_DISABLE)
  { SIG_APEX_ABPD_RESET_COUNTER_REQ,  vgPostApexAbpdResetCounterReq  },
  { SIG_APEX_ABPD_REPORT_COUNTER_REQ, vgPostApexAbpdReportCounterReq },
#endif  
  { SIG_APEX_ABPD_DIAL_REQ,           vgPostApexAbpdDialReq          },
  { SIG_APEX_ABPD_ACTIVATE_DATA_CONN_REQ,
                                      vgPostApexAbpdActivateDataConnReq },
  { SIG_APEX_ABPD_HANGUP_REQ,         vgPostApexAbpdHangupReq        },

  /* No call related SS for NB-IOT */

  { SIG_APEX_MM_PLMNLIST_REQ,         vgPostApexMmPlmnListReq        }
};

#define NUM_POST_ACTIONS (sizeof(postAction) / sizeof(PostAction))

/* the table below matches the signal base of a signal to the corresponding
 * change control element */

static const SignalBaseToElement baseToElement[] =
{
  {ABPM_SIGNAL_BASE,    CC_POWER_MANAGEMENT            },
  {ABMM_SIGNAL_BASE,    CC_MOBILITY_MANAGEMENT         },
  {ABSM_SIGNAL_BASE,    CC_SHORT_MESSAGE_SERVICE       },
  {DM_SIGNAL_BASE,      CC_DM_MODULE                   },
  {ABSI_SIGNAL_BASE,    CC_SUBSCRIBER_IDENTITY_MODULE  },
#if defined (SIM_EMULATION_ON)
  {ALSI_EX_SIGNAL_BASE, CC_SUBSCRIBER_IDENTITY_MODULE  },
#endif /* SIM_EMULATION_ON */

#if defined (FEA_PHONEBOOK)
  {ABLM_SIGNAL_BASE,    CC_LIST_MANAGEMENT             },
#endif /* FEA_PHONEBOOK */

  {ABGL_SIGNAL_BASE,    CC_GENERAL_MODULE              },
  {ABEM_SIGNAL_BASE,    CC_ENGINEERING_MODE            },
  {ABPD_SIGNAL_BASE,    CC_GENERAL_PACKET_RADIO_SYSTEM },
#if defined(FEA_RPM)
  {ABRPM_SIGNAL_BASE,   CC_GENERAL_MODULE },
#endif

};

#define NUM_OF_ELEMENT_SIGNAL_BASES (sizeof(baseToElement) / sizeof(SignalBaseToElement))

/***************************************************************************
 * Variables
 ***************************************************************************/

/***************************************************************************
 * Macros
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:        vgPostApexSimGetChvRsp
 *
 * Parameters:      entity     - channel sending signal
 *                  successful - whether signal was sent or not
 *
 * Returns:         ResultCode_t - result code
 *
 * Description:     Ensures control is released before another ABSI indication
 *                  is received from BL.
 *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgPostApexSimGetChvRsp (const VgmuxChannelNumber entity,
                                            const Boolean successful)
{
  ResultCode_t result = RESULT_CODE_PROCEEDING;

  if (successful)
  {
    /* we must explicitly release control immediately after sending this signal
     * otherwise the state machine can be updated by a signal from ABSI before
     * we've had chance to release control  */
    vgChManReleaseControlOfElement (CC_SUBSCRIBER_IDENTITY_MODULE, entity);
  }

  return (result);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgPostApexMmPlmnSelectReq
 *
 * Parameters:  entity     - channel sending signal
 *              successful - TRUE if the signal was sent
 *
 * Returns:     Nothing
 *
 * Description: Allows the AT+COPS=0 auto selection of network to return
 *              a result code immediately rather than waiting for the
 *              network selection to take place.
 *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgPostApexMmPlmnSelectReq (const VgmuxChannelNumber entity,
                                                const Boolean successful)
{
  PARAMETER_NOT_USED (entity);
  PARAMETER_NOT_USED (successful);

  return (RESULT_CODE_PROCEEDING);
}

#if defined (FEA_PHONEBOOK)
/*--------------------------------------------------------------------------
 *
 * Function:        vgPostApexLmGetAlphaReq
 *
 * Parameters:      entity   - channel sending signal
 *
 * Returns:         Nothing
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgPostApexLmGetAlphaReq (const VgmuxChannelNumber entity,
                                              const Boolean successful)
{
  ResultCode_t            result = RESULT_CODE_PROCEEDING;
  SsCallRelatedContext_t  *ssCallRelatedContext_p = ptrToSsCallRelatedContext ();
  VgSsCallerIdData        *vgSsCallerIdData_p;
  Int8                    index = 0;

#if defined (ENABLE_LONG_AT_CMD_RSP)
  Char                    conversionBuffer[AT_MEDIUM_BUFF_SIZE] = {0};
#else
  Char                    conversionBuffer[CIMUX_MAX_AT_DATA_LENGTH] = {0};
#endif
  GeneralContext_t        *generalContext_p = ptrToGeneralContext (entity);
  VgAlphaLookup           *vgAlphaLookup_p;

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  if (successful == FALSE)
  {
    /* Find all pending alpha requests and cancel for this entity.  Note that
     * we deliberately search only over the first MAX_USER_CALL_ID elements
     * here....
     */
    for (index = 0; index < MAX_USER_CALL_ID; index++)
    {
      vgAlphaLookup_p = &generalContext_p->vgAlphaLookup[index];

      if (vgAlphaLookup_p->active == TRUE)
      {
        /* callId = (index+1) */
        vgAlphaLookup_p->active = FALSE;
        vgSsCallerIdData_p = &ssCallRelatedContext_p->vgSsCallerIdData[index];
        vgSsCallerIdData_p->alphaIdValid = FALSE;
      }
    }

    /* check for pending phonebook alphaId lookup */

    vgAlphaLookup_p = &(generalContext_p->vgAlphaLookup[VG_PHONE_BOOK_CALL_ID - 1]);

    if (vgAlphaLookup_p->active == TRUE)
    {
      vgAlphaLookup_p->active = FALSE;

      /* display empty quotes */
      vgPutc (entity, '\"');
      vgPutc (entity, '\"');
      vgPutNewLine (entity);
    }
  }

  return (result);
}
#endif /* FEA_PHONEBOOK */
#if defined (ATCI_SLIM_DISABLE)

/*--------------------------------------------------------------------------
 *
 * Function:        vgPostApexAbpdResetCounterReq
 *
 * Parameters:      entity   - channel sending signal
 *
 * Returns:
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t vgPostApexAbpdResetCounterReq (const VgmuxChannelNumber entity,
                                                   const Boolean successful)
{
  ResultCode_t  result = RESULT_CODE_OK;

  PARAMETER_NOT_USED(entity);
  PARAMETER_NOT_USED(successful);

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPostApexAbpdReportCounterReq
 *
 * Parameters:      entity   - channel sending signal
 *
 * Returns:
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgPostApexAbpdReportCounterReq (const VgmuxChannelNumber entity,
                                                    const Boolean successful)
{
  ResultCode_t  result = RESULT_CODE_OK;

  PARAMETER_NOT_USED(entity);
  PARAMETER_NOT_USED(successful);

  return (result);
}
#endif
/*--------------------------------------------------------------------------
 *
 * Function:        vgPostApexAbpdHangupReq
 *
 * Parameters:      entity   - channel sending signal
 *
 * Returns:
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgPostApexAbpdHangupReq (const VgmuxChannelNumber entity,
                                             const Boolean successful)
{
  ResultCode_t  result = RESULT_CODE_PROCEEDING;

  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  GprsContext_t        *gprsContext_p        = ptrToGprsContext (entity);
  VgmuxChannelNumber   entityLinkedToCid     = VGMUX_CHANNEL_INVALID;

  if (successful == TRUE)
  {
    /* Only update Operations Manager for PT or PPP connections */
    if ((gprsGenericContext_p != PNULL) && (gprsContext_p != PNULL) &&
        (gprsGenericContext_p->cidUserData[gprsContext_p->vgHangupCid]->psdBearerInfo.connType != ABPD_CONN_TYPE_NONE))
    {

      entityLinkedToCid = vgFindEntityLinkedToCid(gprsContext_p->vgHangupCid);
      
      /* Entity on which hangup request occurred may be different to the
       * channel which "owns" the CID so we need to check for that
       */
      if ((gprsGenericContext_p->hangupChannel != VGMUX_CHANNEL_INVALID) &&
          (gprsGenericContext_p->hangupChannel != entityLinkedToCid))
      {

        if (entityLinkedToCid != VGMUX_CHANNEL_INVALID)
        {
          vgOpManSetConnectionStatus (entityLinkedToCid,
                                       ptrToGprsContext(entityLinkedToCid)->disconnectionItem,
                                        CONNECTION_DISCONNECTING);
        }
        else
        {
          FatalFail("Cannot find entity linked to CID");
        }
      }  
      else
      {
        vgOpManSetConnectionStatus (entity,
                            gprsContext_p->disconnectionItem,
                              CONNECTION_DISCONNECTING);

      }
    }
  }

  return (result);
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgPostApexAbpdDialReq
 *
 * Parameters:      entity   - channel sending signal
 *
 * Returns:
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgPostApexAbpdDialReq (const VgmuxChannelNumber entity,
                                           const Boolean successful)
{
  ResultCode_t  result = RESULT_CODE_PROCEEDING;

  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  GprsContext_t        *gprsContext_p        = ptrToGprsContext (entity);


  if (successful == TRUE)
  {
    /* Only update Operations Manager for PT or PPP connections */
    if ((gprsGenericContext_p != PNULL) && (gprsContext_p != PNULL) &&
        (gprsGenericContext_p->cidUserData[gprsContext_p->vgDialCid]->psdBearerInfo.connType != ABPD_CONN_TYPE_NONE))
    {
      vgOpManSetConnectionStatus (entity,
                                   MIN_PSD_USER_CALL_ID,
                                    CONNECTION_DIALLING);
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPostApexAbpdActivateDataConnReq
 *
 * Parameters:      entity   - channel sending signal
 *
 * Returns:
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgPostApexAbpdActivateDataConnReq (const VgmuxChannelNumber entity,
                                                     const Boolean successful)
{
  ResultCode_t  result = RESULT_CODE_PROCEEDING;

  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  GprsContext_t        *gprsContext_p        = ptrToGprsContext (entity);


  if (successful == TRUE)
  {
    /* Only update Operations Manager for PT or PPP connections */
    if ((gprsGenericContext_p != PNULL) && (gprsContext_p != PNULL) &&
        (gprsGenericContext_p->cidUserData[gprsContext_p->vgDialCid]->psdBearerInfo.connType != ABPD_CONN_TYPE_NONE))
    {
      vgOpManSetConnectionStatus (entity,
                                   MIN_PSD_USER_CALL_ID,
                                    CONNECTION_DIALLING);
    }
  }

  return (result);
}

/* function added for job119076 */
/*--------------------------------------------------------------------------
 *
 * Function:        vgPostApexMmPlmnListReq
 *
 * Parameters:      entity   - channel sending signal
 *
 * Returns:         Nothing
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/

static ResultCode_t vgPostApexMmPlmnListReq (const VgmuxChannelNumber entity,
                                              const Boolean successful)
{
  MobilityContext_t   *mobilityContext_p = ptrToMobilityContext ();
  VgCOPSData          *vgCOPSData        = &mobilityContext_p->vgCOPSData;

  ResultCode_t  result = RESULT_CODE_PROCEEDING;

  PARAMETER_NOT_USED(entity);

  if (!successful)
  {
    vgCOPSData->state = VG_COPS_LIST;
  }

  return (result);
}

 /****************************************************************************
 *
 * Function:    initialiseChss
 *
 * Parameters:  none
 *
 * Returns:     nothing
 *
 * Description: initiates common sub-system entity generic data to default
 *              state, initiates timer utilities and registers interest in
 *              background layer procedures to enable change control requests.
 *
 ****************************************************************************/

static void initialiseChss (void)
{
  ChManagerContext_t *chManagerContext_p = ptrToChManagerContext ();
  Int32              index;

  /* job 117612: initialized to FALSE; will only be changed to TRUE by cicode modules
     which wish ApexShChangeControlReq to be sent with isImmediate set to TRUE */
  chManagerContext_p->isImmediate = FALSE;

  /* set change control state machine up */

  for (index = 0; index < NUMBER_CHANGE_CONTROL_ELEMENTS; index++ )
  {
    chManagerContext_p->vgControl[index].state             = UNREGISTERED;
    chManagerContext_p->vgControl[index].entity            = VGMUX_CHANNEL_INVALID;
    chManagerContext_p->vgControl[index].isSingleUser      = FALSE;
    chManagerContext_p->vgControl[index].sendsReadyInd     = FALSE;
    chManagerContext_p->vgControl[index].pendingSignal     = NON_SIGNAL;
    chManagerContext_p->vgControl[index].pendingCompletion = FALSE;
    chManagerContext_p->vgControl[index].pendingCommand    = FALSE;
    /* added for job108826 */
    chManagerContext_p->vgControl[index].previousEntity    = VGMUX_CHANNEL_INVALID;
  }

  /* control elements which require signals to be sent to get control */

  chManagerContext_p->vgControl[CC_MOBILITY_MANAGEMENT       ].isSingleUser = TRUE;
  chManagerContext_p->vgControl[CC_SUBSCRIBER_IDENTITY_MODULE].isSingleUser = TRUE;
  chManagerContext_p->vgControl[CC_ENGINEERING_MODE          ].isSingleUser = TRUE;

  /* control elements which send ready indication signals */

#if defined (FEA_PHONEBOOK)
  chManagerContext_p->vgControl[CC_LIST_MANAGEMENT           ].sendsReadyInd = TRUE;
#endif /* FEA_PHONEBOOK */

  chManagerContext_p->vgControl[CC_SUBSCRIBER_IDENTITY_MODULE].sendsReadyInd = TRUE;
  chManagerContext_p->vgControl[CC_ENGINEERING_MODE          ].sendsReadyInd = TRUE;
  chManagerContext_p->vgControl[CC_SHORT_MESSAGE_SERVICE     ].sendsReadyInd = TRUE;
  chManagerContext_p->vgControl[CC_GENERAL_MODULE            ].sendsReadyInd = TRUE;

}

/***************************************************************************
 * Global Functions
 ***************************************************************************/


/*--------------------------------------------------------------------------
 *
 * Function:        vgChManContinueActionFlowControl
 *
 * Parameters:      entity   - channel wanting to send signal
 *                  signalId - type of signal to be sent to BL
 *
 * Returns:         a result code set to RESULT_CODE_PROCEEDING if control is
 *                  being requested or a signal has been sent to the BL.
 *                  if control cannot be obtained the result code is set
 *                  to RESULT_CODE_ERROR
 *
 * Description:     This function should be used with command like cpbr
 *                  that print lot of data. It will delay the request for
 *                  the next element until AT sent to MUX all its cached data.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgChManContinueActionFlowControl(  const VgmuxChannelNumber entity,
                                                const SignalId signalId)
{
    ResultCode_t                result                  = RESULT_CODE_PROCEEDING;
    ChannelContext_t           *channelContext_p        = ptrToChannelContext(entity);
    CrManagerGenericContext_t  *crManGenericContext_p   = ptrToCrManagerGenericContext();
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck( channelContext_p != PNULL, entity, 0, 0);
#endif
    if((crManGenericContext_p->cirmDataIndCountFlowControl == 0) &&
       (channelContext_p->at.waitingForCnf == FALSE))
    {
        /*  In the case we are not expecting a MUX response, we have to
        *   send the signal immediatly.*/
        result = vgChManContinueAction( entity, signalId);
    }
    else
    {
        /* For the moment only one signal can be delayed at the same time on a same channel*/
        FatalCheck( channelContext_p->delayedSignalId == NON_SIGNAL, entity, 0, 0);

        /* The signal will be sent later (noramally when the mux will have ack the data)*/
        channelContext_p->delayedSignalId = signalId;
    }

    return result;
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgChManContinueAction
 *
 * Parameters:      entity   - channel wanting to send signal
 *                  signalId - type of signal to be sent to BL
 *
 * Returns:         a result code set to RESULT_CODE_PROCEEDING if control is
 *                  being requested or a signal has been sent to the BL.
 *                  if control cannot be obtained the result code is set
 *                  to RESULT_CODE_ERROR
 *
 * Description:     attempts to send a signal to the background after
 *                  ascertaining whether voyager has control of the relevant
 *                  BL procedure
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgChManContinueAction (const VgmuxChannelNumber entity,
                                     const SignalId signalId)
{
  Int8            aIndex;
  Boolean         matched = FALSE;
  ResultCode_t    result  = RESULT_CODE_PROCEEDING;
  Boolean         commandRunning = FALSE;
  ChManagerContext_t      *chManagerContext_p = ptrToChManagerContext ();
  VgChangeControlElements element;

  /* Check the customer ch table first, if matched is false then process our own */
  matched = continueActionsCustomerList (entity, signalId, &result);

  if ( matched == FALSE )
  {
    /* loops through continueAction table searching for requested signal
     * information */

    for (aIndex = 0;
         (aIndex < NUM_CONTINUE_ACTIONS) && (matched == FALSE);
           aIndex++ )
    {
      if (continueAction[aIndex].signalId == signalId)
      {
        /* the signal has been matched, perform checks on control */
        matched = TRUE;

        if (vgChManFindElement (signalId, &element) == FALSE)
        {
          FatalParam (entity, signalId, 0);
        }
        else
        {
          /* if no pending signal then check entity state to determine if
           * command is running */
          if (chManagerContext_p->vgControl[element].pendingSignal == NON_SIGNAL)
          {
            /* check if we are running a command */
            if (getEntityState (entity) == ENTITY_RUNNING)
            {
              commandRunning = TRUE;
            }
          }
          else
          {
            /* a signal is pending, use pendingCommand flag */
            commandRunning = chManagerContext_p->vgControl[element].pendingCommand;
          }

          if (vgChManCheckHaveControl (element, entity) == TRUE)
          {
            if (chManagerContext_p->vgControl[element].pendingCompletion == FALSE)
            {
              /* job 117612: The default value of isImmediate flag is set for future calls
                 of vgChManGetControl */
              chManagerContext_p->isImmediate = FALSE;
              /* send signal requested */
              (continueAction[aIndex].procFunc) (entity);
              /* run post action action */
              result = vgChManPostAction (signalId, entity, TRUE);
              /* if no command running then release control after sending signal */

              if (commandRunning == FALSE)
              {
                vgChManReleaseControlOfElement ((VgChangeControlElements)element, entity);
              }
              /* reset pending signal */
              chManagerContext_p->vgControl[element].pendingSignal  = NON_SIGNAL;
              chManagerContext_p->vgControl[element].pendingCommand = FALSE;
            }
            else /* entity has control but an operation is currently running already */
            {
              vgTsHSLChangeControlElement (element, entity);
              result = VG_CME_UNABLE_TO_GET_CONTROL;

              /* run post action action */
              vgChManPostAction (signalId, entity, FALSE);
            }
          }
          else
          {
            if (chManagerContext_p->vgControl[element].isSingleUser == TRUE)
            { /* single user procs */
              /* change control signals must be sent to BL */

              if (vgChManGetControl (element, entity) == TRUE)
              {
                chManagerContext_p->vgControl[element].pendingSignal  = signalId;
                chManagerContext_p->vgControl[element].pendingCommand = commandRunning;
              }
              else
              {
                vgTsHSLChangeControlElement (element, entity);
                result = VG_CME_UNABLE_TO_GET_CONTROL;
                /* run post action action */
                vgChManPostAction (signalId, entity, FALSE);
              }
            }
            else
            { /* multi user procs */
              /* may assume control straight away and send signals to BL */
              if (chManagerContext_p->vgControl[element].state == CONTROL_NOT)
              {
                chManagerContext_p->vgControl[element].state          = CONTROL_GOT;
                chManagerContext_p->vgControl[element].entity         = entity;
                chManagerContext_p->vgControl[element].pendingSignal  = NON_SIGNAL;
                chManagerContext_p->vgControl[element].pendingCommand = FALSE;
                /* send requested signal */
                (continueAction[aIndex].procFunc) (entity);
                /* run post action action */
                result = vgChManPostAction (signalId, entity, TRUE);
                /* if no command running then release control after sending signal */
                if (commandRunning == FALSE)
                {
                  vgChManReleaseControlOfElement ((VgChangeControlElements)element, entity);
                }
              }
              else
              {
                if (vgChManGetControl (element, entity) == TRUE)
                { /* maybe unregistered */
                  chManagerContext_p->vgControl[element].pendingSignal  = signalId;
                  chManagerContext_p->vgControl[element].pendingCommand = commandRunning;
                }
                else
                {
                  vgTsHSLChangeControlElement (element, entity);
                  result = VG_CME_UNABLE_TO_GET_CONTROL;
                  /* run post action action */
                  vgChManPostAction (signalId, entity, FALSE);
                }
              }
            }
          }
        }
      }
    }

    /* if the requested signal does not have an entry in the continueAction
     * table an error is returned */

    if (matched == FALSE)
    {
      /* Signal not matched */
      FatalParam (entity, signalId, 0);
    }

  } /* customer match found */

  return (result);

}


/*--------------------------------------------------------------------------
 *
 * Function:        vgChManFindElement
 *
 * Parameters:      signalId - signal id of signal to check
 *                  element  - pointer to a change control element
 *
 * Returns:         nothing
 *
 * Description:     searches for the associated change control element
 *                  by using the signal base of the given signal
 *-------------------------------------------------------------------------*/

Boolean vgChManFindElement (const SignalId signalId,
                             VgChangeControlElements *element)
{
  Int8    index;
  Boolean matchFound = FALSE;
  SignalInterfaceBases signalBase;

  *element = NUMBER_CHANGE_CONTROL_ELEMENTS;

  /* get signal base */
  signalBase = (SignalInterfaceBases)KI_SIGNAL_BASE_FROM_SIGNALID (signalId);

  /* find matching change control element */
  for (index = 0;
       ((index < NUM_OF_ELEMENT_SIGNAL_BASES) && (matchFound == FALSE));
        index++)
  {
    if (baseToElement[index].base == signalBase)
    {
      *element = baseToElement[index].element;
      matchFound = TRUE;
    }
  }

  return (matchFound);
}



/*--------------------------------------------------------------------------
 *
 * Function:        vgChManCheckHaveControl
 *
 * Parameters:      element - procedure to check control status of
 *                  entity  - channel number
 *
 * Returns:         a boolean indicating whether voyager has control of the
 *                  specified BL procedure
 *
 * Description:     checks the current control state of the specified BL
 *                  procedure
 *
 *-------------------------------------------------------------------------*/

Boolean vgChManCheckHaveControl (const VgChangeControlElements element,
                                  const VgmuxChannelNumber entity)
{
  ChManagerContext_t *chManagerContext_p = ptrToChManagerContext ();
  Boolean result;

  if ((chManagerContext_p->vgControl[element].state  == CONTROL_GOT) &&
      (chManagerContext_p->vgControl[element].entity == entity))
  {
    result = TRUE;
  }
  else
  {
    result = FALSE;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgChManGetControl
 *
 * Parameters:      element - procedure to get control of
 *                  entity  - channel number
 *
 * Returns:         a boolean indicating whether control is being sought
 *
 * Description:     checks the control status of the specified BL layer
 *                  procedure. If voyager does not have control or in the
 *                  process of releasing control then it attempts to get
 *                  control.
 *
 *-------------------------------------------------------------------------*/


Boolean vgChManGetControl (const VgChangeControlElements element,
                             const VgmuxChannelNumber entity)
{
  ChManagerContext_t *chManagerContext_p = ptrToChManagerContext ();
  Boolean result = FALSE;

  switch (chManagerContext_p->vgControl[element].state)
  {
    case UNREGISTERED:
    {
      /* attempt to register interest in procedure, and then get control */
      if (vgChManRegister (element, entity) == TRUE)
      {
        chManagerContext_p->vgControl[element].entity = entity;
        result = TRUE;
      }
      break;
    }
    case NOT_READY:            /* the BL layer procedure has not sent ready
                                * indication */
    case PENDING_REGISTRATION: /* registering interest */
    {
      /* if no signal is already pending then can set pending signal */
      if (chManagerContext_p->vgControl[element].pendingSignal == NON_SIGNAL)
      {
        result = TRUE;
        chManagerContext_p->vgControl[element].entity = entity;
      }
      break;
    }
    case CONTROL_GOT:         /* an entity already has control of the specified
                               * procedure */
    case PENDING_CONTROL:     /* another request for control has already been
                               * recieved */
    case SUSPENDED:           /* an error has occurred */
    {
      break;
    }
    case CONTROL_NOT:
    case PENDING_RELEASE:
    {
      result = TRUE;
      /* send change control request signal to background layer */
      chManagerContext_p->vgControl[element].state  = PENDING_CONTROL;
      /* job108826: save current entity for possible future re-use */
      chManagerContext_p->vgControl[element].previousEntity =
        chManagerContext_p->vgControl[element].entity;
      chManagerContext_p->vgControl[element].entity = entity;
      vgSigApexShChangeControlReq (VG_CI_TASK_ID, (ProcId)element,
        chManagerContext_p->isImmediate, entity);

      /* job 117612: The default value of isImmediate flag is set for future calls
               of vgChManGetControl */
      chManagerContext_p->isImmediate = FALSE;
      break;
    }
    default:
    {
      /* Invalid change control state */
      FatalParam (entity, chManagerContext_p->vgControl[element].state, 0);
      break;
    }
  }

  return (result);
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgChManReleaseControl
 *
 * Parameters:      entity - channel number
 *
 * Returns:         nothing
 *
 * Description:     releases control of all background layer procedures
 *                  it is called after an AT command has finished
 *                  executing
 *
 *-------------------------------------------------------------------------*/

void vgChManReleaseControl (const VgmuxChannelNumber entity)
{
  Int8 eIndex;

  for (eIndex = 0; eIndex < NUMBER_CHANGE_CONTROL_ELEMENTS; eIndex++ )
  {
    vgChManReleaseControlOfElement ((VgChangeControlElements)eIndex, entity);
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgChManReleaseControlOfElement
 *
 * Parameters:      entity - channel number
 *
 * Returns:         nothing
 *
 * Description:     releases control of specified change control element
 *
 *-------------------------------------------------------------------------*/

void vgChManReleaseControlOfElement (const VgChangeControlElements element,
                                      const VgmuxChannelNumber entity)
{
  ChManagerContext_t *chManagerContext_p = ptrToChManagerContext ();

  /* if entity has control then release it */
  if ((chManagerContext_p->vgControl[element].state == CONTROL_GOT) &&
      (chManagerContext_p->vgControl[element].entity == entity))
  {
    if (chManagerContext_p->vgControl[element].pendingCompletion == FALSE)
    {
      chManagerContext_p->vgControl[element].pendingSignal  = NON_SIGNAL;
      chManagerContext_p->vgControl[element].pendingCommand = FALSE;
      /* don't reset entity, this ensures the last entity to have control is remembered */

      /* if BL procedure is single user then send signal to release control */
      if (chManagerContext_p->vgControl[element].isSingleUser == TRUE)
      {
        switch (element)
        {
#if defined (USE_ABAPP)
          case CC_MOBILITY_MANAGEMENT:
          {
            MobilityContext_t       *mobilityContext_p       = ptrToMobilityContext ();
            /*force change control*/
            chManagerContext_p->vgControl[element].state = PENDING_RELEASE;
            vgSigApexShChangeControlReq (TASK_FL_ID, (ProcId)element, mobilityContext_p->isImmediate, entity);
            mobilityContext_p->isImmediate = FALSE;
            break;
          }
#endif
          default:
          {
#if defined (USE_ABAPP)
            chManagerContext_p->vgControl[element].state = PENDING_RELEASE;
            vgSigApexShChangeControlReq (TASK_FL_ID, (ProcId)element, FALSE, entity);
#else
            /* The FG layer does not handle change control handovers correctly
             * so we wait until the BG takes it from us */
            chManagerContext_p->vgControl[element].state = CONTROL_NOT;
#endif
            break;
          }
        }
      }
      else /* if multi-user then set the control flag directly */
      {
        chManagerContext_p->vgControl[element].state = CONTROL_NOT;
      }
    }
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgChManRegister
 *
 * Parameters:      entity - channel number
 *
 * Returns:         nothing
 *
 * Description:     releases control of all background layer procedures
 *                  it is called after an AT command has finished
 *                  executing
 *
 *-------------------------------------------------------------------------*/

Boolean vgChManRegister (const VgChangeControlElements element,
                           const VgmuxChannelNumber entity)
{
  Boolean result = FALSE;
  ChManagerContext_t *chManagerContext_p = ptrToChManagerContext ();

  if (chManagerContext_p->vgControl[element].state == UNREGISTERED)
  {
    chManagerContext_p->vgControl[element].state = PENDING_REGISTRATION;
    result = TRUE;

    /* send registration request signal to background layer */
    vgSigApexShRegisterTaskReq ((ProcId)element, entity);
  }

  return (result);
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgSetPendingCompletion
 *
 * Parameters:      element - the change control element to be considered
 *                  pending - pending state to set
 *                  entity  - channel number
 *
 * Returns:         nothing
 *
 * Description:     sets the pending completion flag, this allows an entity
 *                  to remain in control after an AT command has finished
 *                  executing
 *-------------------------------------------------------------------------*/

void vgChManSetPendingCompletion (const VgChangeControlElements element,
                                   const Boolean pending,
                                    const VgmuxChannelNumber entity)
{
  ChManagerContext_t *chManagerContext_p = ptrToChManagerContext ();

  if ((chManagerContext_p->vgControl[element].entity == entity) &&
      (chManagerContext_p->vgControl[element].state  == CONTROL_GOT))
  {
    chManagerContext_p->vgControl[element].pendingCompletion = pending;

    /* if no longer wanting to postpone losing control, then release control */
    if (pending == FALSE)
    {
      vgChManReleaseControlOfElement (element, entity);
    }
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgChManPostAction
 *
 * Parameters:      signalId - signal id of signal to check
 *                  entity   - channel number
 *
 * Returns:         nothing
 *
 * Description:     determines if any post action action is required and
 *                  runs it
 *-------------------------------------------------------------------------*/

ResultCode_t vgChManPostAction (const SignalId signalId,
                         const VgmuxChannelNumber entity,
                          const Boolean successful)
{
  Int8    index;
  Boolean found = FALSE;
  ResultCode_t result = RESULT_CODE_PROCEEDING;

  for (index = 0;
        (index < NUM_POST_ACTIONS) &&
        (found == FALSE);
         index ++)
  {
    if (postAction[index].signalId == signalId)
    {
      result = (postAction[index].procFunc) (entity, successful);
      found = TRUE;
    }
  }
  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgChManCheckSignalDirection
 *
 * Parameters:      signalId - signal id of signal to check
 *                  entity   - channel number
 *
 * Returns:         nothing
 *
 * Description:     determines of incoming unregistered indication signal
 *                  is from a change control element - it then returns
 *                  the current entity which has, or has just lost control
 *-------------------------------------------------------------------------*/

Boolean vgChManCheckSignalDirection (const SignalId signalId,
                                      VgmuxChannelNumber *entity)
{
  ChManagerContext_t      *chManagerContext_p = ptrToChManagerContext ();
  VgChangeControlElements element;
  Boolean result = FALSE;

  *entity = VGMUX_CHANNEL_INVALID;

  if (vgChManFindElement (signalId, &element) == TRUE)
  {
    if (chManagerContext_p->vgControl[element].entity != VGMUX_CHANNEL_INVALID)
    {
      if (isEntityActive(chManagerContext_p->vgControl[element].entity) == TRUE)
      {
        *entity = chManagerContext_p->vgControl[element].entity;
        result = TRUE;
      }
    }

    if (result == FALSE)
    {
      *entity = findFirstEnabledChannel ();

      if (*entity != VGMUX_CHANNEL_INVALID)
      {
        result = TRUE;
      }
    }
  }

  return (result);
}

/* added for job108826 */
/*--------------------------------------------------------------------------
 *
 * Function:        vgChManRestoreVgControlEntity
 *
 * Parameters:      element - BL procedure
 *
 * Returns:         nothing
 *
 * Description:     restores control of the specified BL procedure to the
 *                  previously-stored entity
 *
 *-------------------------------------------------------------------------*/

void vgChManRestoreVgControlEntity (const VgChangeControlElements element)
{
  ChManagerContext_t *chManagerContext_p = ptrToChManagerContext ();

  chManagerContext_p->vgControl[element].entity =
    chManagerContext_p->vgControl[element].previousEntity;
}

/* end of job108826 addition */


/*--------------------------------------------------------------------------
 *
 * Function:        vgChManProcessPendingSignal
 *
 * Parameters:      element - change control element which may have a signal
 *                            pending
 *
 * Returns:         Nothing
 *
 * Description:     considers whether to ask the change control manager to
 *                  send a signal
 *-------------------------------------------------------------------------*/

void vgChManProcessPendingSignal (const VgChangeControlElements element)
{
  ChManagerContext_t *chManagerContext_p = ptrToChManagerContext ();
  ResultCode_t       result;
  Boolean            useResult;

  FatalAssert (element < NUMBER_CHANGE_CONTROL_ELEMENTS);

  /* check there is a signal waiting to be sent */
  if ((element < NUMBER_CHANGE_CONTROL_ELEMENTS) &&
      (chManagerContext_p->vgControl[element].pendingSignal != NON_SIGNAL))
  {
    useResult = chManagerContext_p->vgControl[element].pendingCommand;

    /* call change control manager */
    result = vgChManContinueAction (
              chManagerContext_p->vgControl[element].entity,
               chManagerContext_p->vgControl[element].pendingSignal);

    /* only set result code if a command is running */
    if (useResult == TRUE)
    {
      setResultCode (chManagerContext_p->vgControl[element].entity, result);
    }
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgChManInitialRegistration
 *
 * Parameters:      none
 *
 * Returns:         nothing
 *
 * Description:     registers interest in various background layer
 *                  procedures required when the first channel is enabled
 *-------------------------------------------------------------------------*/

void vgChManInitialRegistration (const VgmuxChannelNumber entity)
{
  /* register interest in background layer procedures */
  vgChManRegister (CC_SHORT_MESSAGE_SERVICE,       entity);
  vgChManRegister (CC_GENERAL_PACKET_RADIO_SYSTEM, entity);
  vgChManRegister (CC_MOBILITY_MANAGEMENT,         entity);
  vgChManRegister (CC_SUBSCRIBER_IDENTITY_MODULE,  entity);

#if defined (FEA_PHONEBOOK)
  vgChManRegister (CC_LIST_MANAGEMENT,             entity);
#endif /* FEA_PHONEBOOK */

  vgChManRegister (CC_POWER_MANAGEMENT,            entity);
  vgChManRegister (CC_GENERAL_MODULE,              entity);
  vgChManRegister (CC_DM_MODULE,                   entity);
#if defined (ENABLE_AT_ENG_MODE)
  vgChManRegister (CC_ENGINEERING_MODE,            entity);
#endif
  vgChManRegister (CC_SIMAT_BIP,                   entity);
}

 /****************************************************************************
 *
 * Function:    vgChManInterfaceController
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

Boolean vgChManInterfaceController (const SignalBuffer *signal_p,
                                     const VgmuxChannelNumber entity)
{
  Boolean accepted = FALSE;

  PARAMETER_NOT_USED(entity);

  switch (*signal_p->type)
  {
    case SIG_INITIALISE:
    {
      initialiseChss ();
      break;
    }
    case SIG_APEX_SH_REGISTER_TASK_CNF:
    {
      vgSigApexShRegisterTaskCnf (signal_p);
      accepted = TRUE;
      break;
    }
    case SIG_APEX_SH_CHANGE_CONTROL_IND:
    {
      vgSigApexShChangeControlInd (signal_p);
      accepted = TRUE;
      break;
    }
    case SIG_APEX_SH_CHANGE_CONTROL_CNF:
    {
      vgSigApexShChangeControlCnf (signal_p);
      accepted = TRUE;
      break;
    }
#if defined (FEA_PHONEBOOK)    
    case SIG_APEX_LM_READY_IND:
    {
      vgSigApexLmReadyInd (signal_p);
      /* we cannot accept this signal as it is needed by the LM sub-system */
      break;
    }
#endif /* FEA_PHONEBOOK */
    case SIG_APEX_SM_READY_IND:
    {
      vgSigApexSmReadyInd (signal_p);
      /* we cannot accept this signal as it is needed by the SMS sub-system */
      break;
    }
    case SIG_APEX_EM_READY_IND:
    {
      vgSigApexEmReadyInd (signal_p);
      accepted = TRUE;
      break;
    }
    case SIG_APEXGL_READY_IND:
    {
      vgSigApexGlReadyInd( signal_p);
      accepted = TRUE;
      break;
    }
    case SIG_APEX_SIM_OK_IND:
    case SIG_APEX_SIM_NOK_IND:

    case SIG_APEX_SIM_GET_PIN_IND: /* uses the same handler for all since
                                    * the signal content is not used */
    case SIG_APEX_SIM_GET_CHV_IND: /* uses the same handler for all since
                                    * the signal content is not used */
    {
      vgSigApexSimInd (signal_p);
      /* not accepted since another sub-systems also processes the signal */
      break;
    }
    default:
    {
      break;
    }
  }
  return (accepted);
}


/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */





