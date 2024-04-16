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
 * Handler for Profile AT Commands
 * Procedures for command execution located in vgpfut.c
 **************************************************************************/

#define MODULE_NAME "RVPFSS"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <cici_sig.h>
#include <rvpfss.h>
#include <rvutil.h>
#include <rvdata.h>
#include <vgmx_sig.h>
#include <rvemut.h>
/***************************************************************************
 * Signal definitions
 ***************************************************************************/

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* Profile command information table */

static const AtCmdControl pfAtCommandTable[] =
{
  {ATCI_CONST_CHAR_STR "&C",            vgPfDCD,          VG_AT_PF_DCD,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "&D",            vgPfDTR,          VG_AT_PF_DTR,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "&F",            vgPfF,            VG_AT_PF_DEFAULT,  AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "&K",            vgPfKFC,          VG_AT_PF_KFC,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "&V",            vgPfV,            VG_AT_PF_VIEW,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "&W",            vgPfW,            VG_AT_PF_WRITE,    AT_CMD_ACCESS_NONE},
#if defined (FEA_PPP)    
  {ATCI_CONST_CHAR_STR "*MGPPPLOG",     vgPfMGPPPLOG,     VG_AT_PF_MGPPPLOG, AT_CMD_ACCESS_NONE},
#endif /* FEA_PPP */    
#if defined (FEA_MT_PDN_ACT)
  {ATCI_CONST_CHAR_STR "*MGMTPCACT",    vgPfMGMTPCACT,    VG_AT_PF_MGMTPCACT,AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CGAUTO",       vgPfCGAUTO,       VG_AT_PF_CGAUTO,   AT_CMD_ACCESS_NONE},
#endif
  {ATCI_CONST_CHAR_STR "+CGEREP",       vgPfCGEREP,       VG_AT_PF_CGEREP,   AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CMEE",         vgPfExtOp,        VG_AT_PF_CME,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CMGF",         vgPfExtOp,        VG_AT_PF_CMGF,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CNMI",         vgPfCNMI,         VG_AT_PF_CNMI,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CR",           vgPfExtOp,        VG_AT_PF_CR,       AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CRES",         vgPfCRES,         VG_AT_PF_CRES,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CSAS",         vgPfCSAS,         VG_AT_PF_CSAS,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CSCS",         vgPfCSCS,         VG_AT_PF_CSCS,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CSDH",         vgPfExtOp,        VG_AT_PF_CSDH,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CSMP",         vgPfCSMP,         VG_AT_PF_CSMP,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CSMS",         vgPfCSMS,         VG_AT_PF_CSMS,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CTZR",         vgPfExtOp,        VG_AT_PF_CTZR,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+CTZU",         vgPfExtOp,        VG_AT_PF_CTZU,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+DR",           vgPfExtOp,        VG_AT_PF_DR,       AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+DS",           vgPfDS,           VG_AT_PF_DS,       AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+FCLASS",       vgPfExtOp,        VG_AT_PF_FCLASS,   AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+ICF",          vgPfICF,          VG_AT_PF_ICF,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+IFC",          vgPfIFC,          VG_AT_PF_IFC,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "+IPR",          vgPfIPR,          VG_AT_PF_IPR,      AT_CMD_ACCESS_NONE},    
  {ATCI_CONST_CHAR_STR "+ILRR",         vgPfExtOp,        VG_AT_PF_ILRR,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MCEERMODE",    vgPfExtOp,        VG_AT_PF_MCEERMODE,AT_CMD_ACCESS_NONE}, 
  {ATCI_CONST_CHAR_STR "+CGPIAF",       vgPfCGPIAF,       VG_AT_PF_CGPIAF,   AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MCGEUNSOL",    vgPfExtOp,        VG_AT_PF_MCGEUNSOL,AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MLTEGCF",      vgPfExtOp,        VG_AT_PF_MLTEGCF,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MLTEGCFLOCK",  vgPfExtOp,        VG_AT_PF_MLTEGCFLOCK, AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "E",             vgPfOther,        VG_AT_PF_ECHO,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "L",             vgPfOther,        VG_AT_PF_LOUDNESS, AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "M",             vgPfOther,        VG_AT_PF_MONITOR,  AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "N1",            vgPfN1,           VG_AT_PF_N1,       AT_CMD_ACCESS_NONE},  
  {ATCI_CONST_CHAR_STR "Q",             vgPfOther,        VG_AT_PF_QUIET,    AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "S0",            vgPfSregOp,       VG_AT_PF_S0,       AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "S10",           vgPfSregOp,       VG_AT_PF_S10,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "S12",           vgPfSregOp,       VG_AT_PF_S12,      AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "S1",            vgPfS1regOp,      VG_AT_PF_S1,       AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "S25",           vgPfSregOp,       VG_AT_PF_S25,      AT_CMD_ACCESS_NONE},  
  {ATCI_CONST_CHAR_STR "S2",            vgPfSregOp,       VG_AT_PF_S2,       AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "S3",            vgPfSregOp,       VG_AT_PF_S3,       AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "S4",            vgPfSregOp,       VG_AT_PF_S4,       AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "S5",            vgPfSregOp,       VG_AT_PF_S5,       AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "S6",            vgPfSregOp,       VG_AT_PF_S6,       AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "S7",            vgPfSregOp,       VG_AT_PF_S7,       AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "S8",            vgPfSregOp,       VG_AT_PF_S8,       AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "S95",           vgPfS95,          VG_AT_PF_S95,      AT_CMD_ACCESS_NONE},  
  {ATCI_CONST_CHAR_STR "V",             vgPfOther,        VG_AT_PF_VERBOSE,  AT_CMD_ACCESS_NONE},  
  {ATCI_CONST_CHAR_STR "X",             vgPfOther,        VG_AT_PF_X,        AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "Z",             vgPfZ,            VG_AT_PF_Z,        AT_CMD_ACCESS_NONE},
#if defined (ENABLE_AT_ENG_MODE)
  {ATCI_CONST_CHAR_STR "*MEMPSET",      vgEmMParaSet,     VG_AT_MM_MEMPSET  ,AT_CMD_ACCESS_CFUN_1},
  {ATCI_CONST_CHAR_STR "*MEMPS",        vgEmMPowerSaving, VG_AT_MM_MEMPS    ,AT_CMD_ACCESS_CFUN_1},
#endif
  /* For NB-IOT */
  {ATCI_CONST_CHAR_STR "*MPDI",         vgPfExtOp,        VG_AT_PF_MPDI,     AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MPLMNURI",     vgPfMPLMNURI,     VG_AT_PF_MPLMNURI, AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MAPNURI",      vgPfMAPNURI,      VG_AT_PF_MAPNURI,  AT_CMD_ACCESS_NONE},
  {ATCI_CONST_CHAR_STR "*MATWAKEUP",    vgPfExtOp,        VG_AT_PF_MATWAKEUP,AT_CMD_ACCESS_NONE},

  {PNULL,                        PNULL,            VG_AT_LAST_CODE,   AT_CMD_ACCESS_NONE}
};

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/
/***************************************************************************
 * Global Function Prototypes
 ***************************************************************************/

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

 /****************************************************************************
 *
 * Function:    initialisePfss
 *
 * Parameters:  none
 *
 * Returns:     none
 *
 * Description: does nothing
 *
 ****************************************************************************/
void initialisePfss (const VgmuxChannelNumber entity)
{
  ProfileContext_t*        profileContext_p  = ptrToProfileContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(profileContext_p != PNULL, entity, 0, 0);
#endif
  /* initialise the profile data - when the channels are enabled these defaults
   * will be updated to either the factory defaults or those in NVRAM */
  memset (profileContext_p, 0xFF, sizeof (ProfileContext_t));
}


/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:    vgPtrToPfAtCommandTable
 *
 * Parameters:  none
 *
 * Returns:     Nothing
 *
 * Description: returns pointer to profile at command table
 *
 *-------------------------------------------------------------------------*/

AtCmdControl const *vgPtrToPfAtCommandTable (void)
{
  return ( pfAtCommandTable );
}

 /****************************************************************************
 *
 * Function:    vgPfssInterfaceController
 *
 * Parameters:  signal_p - signal to be processed
 *              entity - mux channel number
 *
 * Returns:     none
 *
 * Description: determines action for received signals
 *
 ****************************************************************************/

Boolean vgPfssInterfaceController (const SignalBuffer  *signal_p,
                                   const VgmuxChannelNumber entity)
{
  Boolean accepted = FALSE;

  /* Signal Handler */

  switch (*signal_p->type)
  {
    case SIG_CI_RUN_AT_COMMAND_IND:
    {
      accepted = parseCommandBuffer (pfAtCommandTable, entity);
      break;
    }
    case SIG_CIMUX_CHANNEL_ENABLE_IND:
    {
      initialisePfss (entity);
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

