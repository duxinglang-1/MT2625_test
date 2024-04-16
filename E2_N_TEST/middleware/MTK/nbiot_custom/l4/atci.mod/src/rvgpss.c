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
 * Handler for GPRS AT Commands
 * Procedures for command execution located in vggput.c
 **************************************************************************/

#define MODULE_NAME "RVGPSS"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <rvsystem.h>
#include <cici_sig.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvgpss.h>
#include <rvgput.h>
#include <rvpdsigo.h>
#include <rvpdsigi.h>
#include <rvmmut.h>
#include <rvcimxut.h>
#include <rvgpuns.h>
#include <vgmx_sig.h>
#if !defined (USE_L4MM_ALLOC_MEMORY)
#if defined (USE_BMM_ALLOC_MEMORY)
#include <bmm_sig.h>
#else
#include <tmm_sig.h>
#endif
#endif /* USE_L4MM_ALLOC_MEMORY */

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  CiRunAtCommandInd  ciRunAtCommandInd;
};

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* GPRS command information table */

static const AtCmdControl gpAtCommandTable[] =
{
  {ATCI_CONST_CHAR_STR "+CGACT",           vgGpCGACT,          VG_AT_GP_CGACT,             AT_CMD_ACCESS_NONE},

#if defined (FEA_MT_PDN_ACT)
  {ATCI_CONST_CHAR_STR "+CGANS",           vgGpCGANS,          VG_AT_GP_CGANS,             AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MMTPDPCID",       vgGpMMTPDPCID,      VG_AT_GP_MMTPDPCID,         AT_CMD_ACCESS_NONE},
#endif /* FEA_MT_PDN_ACT */

  {ATCI_CONST_CHAR_STR "+CGATT",           vgGpCGATT,          VG_AT_GP_CGATT,             AT_CMD_ACCESS_NONE},

#if defined (FEA_QOS_TFT)
  {ATCI_CONST_CHAR_STR "+CGCMOD",          vgGpCGCMOD,         VG_AT_GP_CGCMOD,            AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CGTFT",           vgGpCGTFT,          VG_AT_GP_CGTFT,             AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CGTFTRDP",        vgGpCGTFTRDP,       VG_AT_GP_CGTFTRDP,          AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CGEQOS",          vgGpCGEQOS,         VG_AT_GP_CGEQOS,            AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CGEQOSRDP",       vgGpCGEQOSRDP,      VG_AT_GP_CGEQOSRDP,         AT_CMD_ACCESS_NONE},
#endif /* FEA_QOS_TFT */

  {ATCI_CONST_CHAR_STR "*MGCOUNT",         vgGpMGCOUNT,        VG_AT_GP_MGCOUNT,           AT_CMD_ACCESS_CFUN_1},
  {ATCI_CONST_CHAR_STR "+CGDATA",          vgGpCGDATA,         VG_AT_GP_CGDATA,            AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CGDCONT",         vgGpCGDCONT,        VG_AT_GP_CGDCONT,           AT_CMD_ACCESS_NONE},
#if defined (FEA_DEDICATED_BEARER)
  {ATCI_CONST_CHAR_STR "+CGDSCONT",        vgGpCGDSCONT,       VG_AT_GP_CGDSCONT,          AT_CMD_ACCESS_NONE},
#endif
  {ATCI_CONST_CHAR_STR "+CGPADDR",         vgGpCGPADDR,        VG_AT_GP_CGPADDR,           AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CGREG",           vgGpCGREG,          VG_AT_GP_CGREG,             AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CGSMS",           vgGpCGSMS,          VG_AT_GP_CGSMS,             AT_CMD_ACCESS_CFUN_1},
  {ATCI_CONST_CHAR_STR "*MGPRVSS",         vgGpD,              VG_AT_GP_D,                 AT_CMD_ACCESS_CFUN_1},
  {ATCI_CONST_CHAR_STR "*MGSINK",          vgGpMGSINK,         VG_AT_GP_MGSINK,            AT_CMD_ACCESS_CFUN_1},
  {ATCI_CONST_CHAR_STR "*MGTCSINK",        vgGpMGTCSINK,       VG_AT_GP_MGTCSINK,          AT_CMD_ACCESS_CFUN_1},
#if defined (FEA_PPP)
  {ATCI_CONST_CHAR_STR "*MLOOPPSD",        vgGpMLOOPPSD,       VG_AT_GP_MLOOPPSD,          AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MPPPCONFIG",      vgGpMPPPCONFIG,     VG_AT_GP_MPPPCONFIG,        AT_CMD_ACCESS_CFUN_1},
  {ATCI_CONST_CHAR_STR "*MPPPCONFIGAUTH",  vgGpMPPPCONFIGAUTH, VG_AT_GP_MPPPCONFIGAUTH,    AT_CMD_ACCESS_CFUN_1},
#endif /* FEA_PPP */
#if defined (FEA_ACL)
  {ATCI_CONST_CHAR_STR "*MSACL",           vgGpMSACL,          VG_AT_GP_MSACL,             AT_CMD_ACCESS_CFUN_1},
  {ATCI_CONST_CHAR_STR "*MLACL",           vgGpMLACL,          VG_AT_GP_MLACL,             AT_CMD_ACCESS_CFUN_1},
  {ATCI_CONST_CHAR_STR "*MWACL",           vgGpMWACL,          VG_AT_GP_MWACL,             AT_CMD_ACCESS_CFUN_1},
  {ATCI_CONST_CHAR_STR "*MDACL",           vgGpMDACL,          VG_AT_GP_MDACL,             AT_CMD_ACCESS_CFUN_1},
#endif
  {ATCI_CONST_CHAR_STR "+CGCONTRDP",       vgGpCGCONTRDP,      VG_AT_GP_CGCONTRDP,         AT_CMD_ACCESS_NONE},

#if defined (FEA_DEDICATED_BEARER)
  {ATCI_CONST_CHAR_STR "+CGSCONTRDP",      vgGpCGSCONTRDP,     VG_AT_GP_CGSCONTRDP,        AT_CMD_ACCESS_CFUN_1},
#endif

  {ATCI_CONST_CHAR_STR "*MCGDEFCONT",      vgGpMCGDEFCONT,     VG_AT_GP_MCGDEFCONT,        AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CGDEL",           vgGpCGDEL,          VG_AT_GP_CGDEL,             AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CGAUTH",          vgGpCGAUTH,         VG_AT_GP_CGAUTH,            AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CEREG",           vgGpCEREG,          VG_AT_GP_CEREG,             AT_CMD_ACCESS_NONE}, 
  {ATCI_CONST_CHAR_STR "*MDPDNP",           vgGpMDPDNP,          VG_AT_GP_MDPDNP,             AT_CMD_ACCESS_NONE},
#if defined (FEA_UPDIR)
  {ATCI_CONST_CHAR_STR "*MUPDIR",          vgGpMUPDIR,         VG_AT_GP_MUPDIR,            AT_CMD_ACCESS_NONE},
#endif

  /* AT commands for NB-IOT */

  {ATCI_CONST_CHAR_STR "*MNBIOTDT",        vgGpMNBIOTDT,       VG_AT_GP_MNBIOTDT,          AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MNBIOTRAI",       vgGpMNBIOTRAI,      VG_AT_GP_MNBIOTRAI,         AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CGAPNRC",         vgGpCGAPNRC,        VG_AT_GP_CGAPNRC,           AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CSODCP",          vgGpCSODCP,         VG_AT_GP_CSODCP,            AT_CMD_ACCESS_NONE},  
  {PNULL,                           PNULL,              VG_AT_LAST_CODE,            AT_CMD_ACCESS_NONE}
};

static const SignalCnfFunc signalFunc[] =
{
    {SIG_APEX_ABPD_CHANNEL_ALLOC_CNF,   vgApexAbpdChannelAllocCnf },

    {SIG_APEX_ABPD_PSD_ATTACH_CNF,      vgApexAbpdPsdAttachCnf    },
    {SIG_APEX_ABPD_PSD_DETACH_CNF,      vgApexAbpdPsdDetachCnf    },

    {SIG_APEX_ABPD_ATTACH_DEF_BEARER_ACT_IND,
                                        vgApexAbpdAttachDefBearerActInd },
    {SIG_APEX_ABPD_CONTEXT_IND,         vgApexAbpdContextInd      },
    {SIG_APEX_ABPD_CONNECT_IND,         vgApexAbpdConnectInd      },
    {SIG_APEX_ABPD_ERROR_IND,           vgApexAbpdErrorInd        },
    {SIG_APEX_ABPD_CONNECTING_IND,      vgApexAbpdConnectingInd   },
    {SIG_APEX_ABPD_CONNECTED_IND,       vgApexAbpdConnectedInd    },
    {SIG_APEX_ABPD_PSD_BEARER_STATUS_IND,
                                        vgApexAbpdPsdBearerStatusInd},
    {SIG_APEX_ABPD_DISCONNECTED_IND,    vgApexAbpdDisconnectedInd },
    {SIG_APEX_ABPD_BUSY_IND,            vgApexAbpdBusyInd         },
    {SIG_APEX_ABPD_ACTIVATE_DATA_CONN_CNF,
                                        vgApexAbpdActivateDataConnCnf   },
#if defined (FEA_QOS_TFT)
    {SIG_APEX_ABPD_PSD_MODIFY_CNF,      vgApexAbpdPsdModifyCnf    },
    {SIG_APEX_ABPD_PSD_MODIFY_REJ,      vgApexAbpdPsdModifyRej    },
    {SIG_APEX_ABPD_PSD_MODIFY_IND,      vgApexAbpdPsdModifyInd    },
#endif /* FEA_QOS_TFT */

#if defined (FEA_MT_PDN_ACT)
    {SIG_APEX_ABPD_SETUP_IND,           vgApexAbpdSetupInd        },
#endif /* FEA_MT_PDN_ACT */

    {SIG_APEX_ABPD_APN_READ_CNF,        vgApexAbpdApnReadCnf      },
    {SIG_APEX_ABPD_APN_WRITE_CNF,       vgApexAbpdApnWriteCnf     },


    {SIG_APEX_ABPD_REPORT_COUNTER_CNF,  vgApexAbpdReportCounterCnf},
    {SIG_APEX_ABPD_COUNTER_IND,         vgApexAbpdCounterInd      },

#if defined (FEA_UPDIR)
    {SIG_APEX_ABPD_SET_UPDIR_INFO_CNF,  vgApexAbpdSetUpdirInfoCnf },
    {SIG_APEX_ABPD_UPDIR_IND,           vgApexAbpdUpdirInd        },    
#endif

#if defined (FEA_PPP)
    {SIG_APEX_ABPD_PPP_CONFIG_CNF,      vgApexAbpdPppConfigCnf    },
    {SIG_APEX_ABPD_PPP_CONFIG_AUTH_CNF, vgApexAbpdPppConfigAuthCnf},
    {SIG_APEX_ABPD_PPP_LOOPBACK_CNF,    vgApexAbpdPppLoopbackCnf  },    
#endif /* FEA_PPP */
#if defined (FEA_ACL)
    {SIG_APEX_ABPD_SET_ACL_CNF,         vgApexAbpdSetAclCnf       },
    {SIG_APEX_ABPD_LIST_ACL_CNF,        vgApexAbpdListAclCnf      },
    {SIG_APEX_ABPD_WRITE_ACL_CNF,       vgApexAbpdWriteAclCnf     },
    {SIG_APEX_ABPD_DELETE_ACL_CNF,      vgApexAbpdDeleteAclCnf    },
    {SIG_APEX_ABPD_ACL_STATUS_CNF,      vgApexAbpdAclStatusCnf    },
#endif
    {SIG_APEX_ABPD_STK_INFO_IND,        vgApexAbpdStkInfoInd      },
    {SIG_APEX_ABPD_WRITE_REL_ASSIST_CNF,   vgApexAbpdWriteRelAssistCnf  },
    {SIG_APEX_ABPD_READ_REL_ASSIST_CNF,    vgApexAbpdReadRelAssistCnf   },
    {SIG_APEX_ABPD_WRITE_APN_DATA_TYPE_CNF,vgApexAbpdWriteApnDataTypeCnf},
    {SIG_APEX_ABPD_READ_APN_DATA_TYPE_CNF, vgApexAbpdReadApnDataTypeCnf },

    {SIG_APEX_ABPD_APN_UL_RATE_CONTROL_IND, vgApexAbpdApnUlRateControlInd},
    {SIG_APEX_ABPD_PLMN_UL_RATE_CONTROL_IND,vgApexAbpdPlmnUlRateControlInd},
    {SIG_APEX_ABPD_PACKET_DISCARD_IND,      vgApexAbpdPacketDiscardInd},
    {SIG_APEX_ABPD_MTU_IND,                 vgApexAbpdMtuInd},

    {SIG_APEX_SM_READ_ROUTE_CNF,        vgApexSmReadRouteCnf      },
    {SIG_APEX_SM_WRITE_ROUTE_CNF,       vgApexSmWriteRouteCnf     },

#if !defined (USE_L4MM_ALLOC_MEMORY)
#if !defined (USE_BMM_ALLOC_MEMORY)
    {SIG_UT_MEM_ABOVE_HWM_IND,          vgUtMemAboveHwmInd        },
    {SIG_UT_MEM_BELOW_LWM_IND,          vgUtMemBelowLwmInd        },
#endif /* USE_BMM_ALLOC_MEMORY */
#endif /* !USE_L4MM_ALLOC_MEMORY */
};

#define NUM_GP_SIG_FUNC (sizeof(signalFunc) / sizeof(SignalCnfFunc))

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static void initialiseGpssGeneric (void);

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/***************************************************************************
 * Local Variables
 ***************************************************************************/

/****************************************************************************
 *
 * Function:    initialiseGpss
 *
 * Parameters:  entity
 *
 * Returns:     none
 *
 * Description: sets gprs sub-system entity specific data
 *
 ****************************************************************************/

void initialiseGpss (const VgmuxChannelNumber entity)
{
  GprsContext_t *gprsContext_p = ptrToGprsContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(gprsContext_p != PNULL, entity, 0, 0);
#endif
  gprsContext_p->activePsdBearerContextWithDataConn = PNULL;

  gprsContext_p->pendingOpenDataConnCnf = FALSE;

  gprsContext_p->vgDialCid             = CID_NUMBER_UNKNOWN;
  gprsContext_p->vgModifyCid           = CID_NUMBER_UNKNOWN;
  gprsContext_p->vgHangupType          = VG_HANGUP_TYPE_ATH;
  gprsContext_p->vgHangupCid           = CID_NUMBER_UNKNOWN;
  gprsContext_p->disconnectionItem     = NEVER_USED_USER_CALL_ID;
  gprsContext_p->attachInProgress      = FALSE;
  gprsContext_p->attachAbortInProgress = FALSE;

  gprsContext_p->connectionActive  = FALSE;


#if defined (FEA_PPP)
  gprsContext_p->vgPppLoopbackState      = VG_LOOPBACK_DISABLED;
  gprsContext_p->vgPppLoopbackNumDlXmit  = 0;
  gprsContext_p->vgPppLoopbackMode       = 0;
  gprsContext_p->vgPppLoopbackDlTimeout  = 0;
  gprsContext_p->vgPppLoopbackPacketSize = 0;
  gprsContext_p->vgPppTotalNumDlXmit     = 0;

  gprsContext_p->vgPppLoopbackPppMode             = LOOPBACK_PPP_MODE_STANDARD;
  gprsContext_p->vgPppLoopbackFCSCheckingEnabled  = TRUE;
  gprsContext_p->vgPppLoopbackByteStuffingEnabled = TRUE;
  gprsContext_p->vgPppLoopbackUlCheckingEnabled   = TRUE;
#endif /* FEA_PPP */
}

 /****************************************************************************
 *
 * Function:    initialiseGpssGeneric
 *
 * Parameters:  none
 *
 * Returns:     none
 *
 * Description: sets gprs sub-system generic data
 *
 ****************************************************************************/

static void initialiseGpssGeneric ()
{
  /* Job137602: Add a loop flag to set the BAND */
  Int8                 i = 0;
  GprsGenericContext_t *gprsGenericContext_p;
  Int8                 profile;
  Int8                 nsapiEntry;
  Int8                 channel;

  gprsGenericContext_p = ptrToGprsGenericContext ();

  for (profile = 0; profile < MAX_NUMBER_OF_CIDS; profile++)
  {
    /* Set the pointers to PNULL */
    allocateMemToGprsContext (profile, PNULL);
  }

  for (i=0; i < ATCI_MAX_NUM_PSD_CONNECTIONS; i++)
  {
    gprsGenericContext_p->inUsePsdMemData[i].inUse = FALSE;
  }

  gprsGenericContext_p->defaultSmsRoute                     = GPRS_GSMS_ROUTE_GPRS;

#if defined (FEA_MT_PDN_ACT)
  gprsGenericContext_p->incomingPdpContextActivation = FALSE;

  memset (&gprsGenericContext_p->vgAbpdSetupRsp, 0,
          sizeof (gprsGenericContext_p->vgAbpdSetupRsp));
  memset (&gprsGenericContext_p->vgAbpdSetupInd, 0,
          sizeof (gprsGenericContext_p->vgAbpdSetupInd));
#endif /* FEA_MT_PDN_ACT */

  memset (&gprsGenericContext_p->currentDefaultAPN, 0,
          sizeof (gprsGenericContext_p->currentDefaultAPN));
  memset (&gprsGenericContext_p->definedDefaultAPN, 0,
          sizeof (gprsGenericContext_p->definedDefaultAPN));

  gprsGenericContext_p->gprsServiceState.valid = FALSE;
  gprsGenericContext_p->gprsServiceState.gprsAttached = FALSE;

  gprsGenericContext_p->gprsServiceState.abmmCurrentService = GPRS_SERVICE;

  for (nsapiEntry = 0; nsapiEntry < MAX_NUM_NSAPIS; nsapiEntry++)
  {
    gprsGenericContext_p->vgPdpContext[nsapiEntry].active = FALSE;
  }

  /* Do same for CGREG unsolicited result code */
  gprsGenericContext_p->vgCGREGData.state           = FALSE;
  gprsGenericContext_p->vgCGREGData.regStatus       = VGMNL_NOT_REGISTRATED;
  gprsGenericContext_p->vgCGREGData.lac             = 0;
  gprsGenericContext_p->vgCGREGData.cellId          = 0;
  gprsGenericContext_p->vgCGREGData.rac             = 0;

  /* Do same for CEREG unsolicited result code */
  gprsGenericContext_p->vgCEREGData.state           = FALSE;
  gprsGenericContext_p->vgCEREGData.regStatus       = VGMNL_NOT_REGISTRATED;
  gprsGenericContext_p->vgCEREGData.tac             = 0;
  gprsGenericContext_p->vgCEREGData.cellId          = 0;
  gprsGenericContext_p->vgCEREGData.rac             = 0;
  gprsGenericContext_p->vgCEREGData.rejectCause     = NO_CAUSE;
  gprsGenericContext_p->vgCEREGData.psmInfoPresent  = FALSE;

  /* set cid ownership to invalid channel */

  /* job127550: allow for transient CID */
  for (channel = 0; channel < MAX_NUMBER_OF_CIDS; channel++)
  {
    gprsGenericContext_p->cidOwner[channel] = VGMUX_CHANNEL_INVALID;
  }
  
  memset (gprsGenericContext_p->vgMDPDNPData.defaultApnFromNw.name,
          NULL_CHAR,
          sizeof (gprsGenericContext_p->vgMDPDNPData.defaultApnFromNw.name));
  gprsGenericContext_p->vgMDPDNPData.defaultApnFromNw.length = 0;

  gprsGenericContext_p->vgCSODCPData.connId = 0;
  gprsGenericContext_p->vgCSODCPData.userDataLength = 0;
  gprsGenericContext_p->vgCSODCPData.userData = PNULL;

  gprsGenericContext_p->vgMGCOUNTData.vgCounterCid = CID_NUMBER_UNKNOWN;
  gprsGenericContext_p->vgMGCOUNTData.vgReportEntity = VGMUX_CHANNEL_INVALID;
#if defined (FEA_UPDIR)
  vgInitialisePdnAddress(&gprsGenericContext_p->vgMUPDIRData.srcIpAddress);
  vgInitialisePdnAddress(&gprsGenericContext_p->vgMUPDIRData.dstIpAddress);
  gprsGenericContext_p->vgMUPDIRData.vgMupdiEntity = VGMUX_CHANNEL_INVALID; 
  gprsGenericContext_p->vgMUPDIRData.srcPortPresent = FALSE;
  gprsGenericContext_p->vgMUPDIRData.dstPortPresent = FALSE;
  gprsGenericContext_p->vgMUPDIRData.patternId = 0;  
#endif

#if defined (FEA_MT_PDN_ACT)
  gprsGenericContext_p->vgMMTPDPCIDData.enabled = FALSE;
  gprsGenericContext_p->vgMMTPDPCIDData.cid     = 0;
#endif /* FEA_MT_PDN_ACT */

  gprsGenericContext_p->vgLteAttachDefaultBearerState = LTE_ATTACH_DEFAULT_BEARER_NOT_CONNECTED;
  gprsGenericContext_p->lteAttachDefaultBearerCid     = CID_NUMBER_UNKNOWN;

  gprsGenericContext_p->hangupChannel = VGMUX_CHANNEL_INVALID;

  /* Init PLMN Rate Control info. */
  gprsGenericContext_p->plmnRateControlInfo.plmnRateControlPresent = FALSE;
  gprsGenericContext_p->plmnRateControlInfo.plmnRateControlValue = 0;

}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

 /****************************************************************************
 *
 * Function:    vgGpssInterfaceController
 *
 * Parameters:  signal_p - signal to be processed
 *              entity - mux channel number
 *
 * Returns:     none
 *
 * Description: determines action for received signals
 *
 ****************************************************************************/

Boolean vgGpssInterfaceController (const SignalBuffer *signal_p,
                                    const VgmuxChannelNumber entity)
{
  Int8                 numActiveEntities      = 0;
  VgmuxChannelNumber   profileEntity          = VGMUX_CHANNEL_INVALID;
  GprsGenericContext_t *gprsGenericContext_p  = ptrToGprsGenericContext();
  Boolean accepted                            = FALSE;
  Int8    fIndex;

  /* Signal Handler */
  switch (*signal_p->type)
  {
    case SIG_CI_RUN_AT_COMMAND_IND:
    {
      accepted = parseCommandBuffer (gpAtCommandTable, entity);
      break;
    }
    case SIG_INITIALISE:
    {
      initialiseGpssGeneric ();
      break;
    }
    case SIG_CIMUX_CHANNEL_ENABLE_IND:
    {
      initialiseGpss (entity);
      break;
    }
    case SIG_APEX_MM_NETWORK_STATE_IND:
    {
      vgApexMmNetworkStateInd (signal_p, entity);
      break;
    }

    case SIG_CI_USER_PROF_LOADED_IND:
    {
      /* Are we registered yet!
       * Can only happen for the first channel, thereafter it is not possible
       */

      /* For NB-IOT we need to check if we are allowed to activate default
       * PDN connection on attach - before doing anything (set by AT+CIPCA)
       */
      if (vgIsCurrentAccessTechnologyLte() &&
          vgPsdAttached() &&
          vgCIPCAPermitsActivateAttachDefBearer() &&
          (getProfileValue (entity, PROF_MLTEGCF) != GCF_TEST_MODE_DISABLE))

      {
        for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
        {
          if (isEntityActive (profileEntity))
          {
            numActiveEntities++;
          }
        }

        if ((numActiveEntities == 1) &&
            (gprsGenericContext_p->vgLteAttachDefaultBearerState == LTE_ATTACH_DEFAULT_BEARER_NOT_CONNECTED))
        {
          /* This must be the first activated entity.  If so then
           * we need to connect to the default bearer activated during the
           * attach procedure - only if we are not pending for it to be
           * connected in another function (crossover situation...)
           */
          vgActivateAttachDefBearerContext (entity);
        }
      }
      break;
    }
    default:
    {
      for (fIndex = 0;
           (fIndex < NUM_GP_SIG_FUNC) && (accepted == FALSE);
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

