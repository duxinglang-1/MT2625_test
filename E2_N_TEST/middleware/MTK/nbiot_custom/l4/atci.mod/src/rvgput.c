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
 * Procedures for GPRS AT command execution
 *
 * Contains implementations of the following AT commands
 *
 * AT+CGACT        - activates a PDP context
 * AT+CGANS        - accept request from network to activate pdp context
 * AT+CGATT        - attaches or detaches from the network
 * AT+CGAUTO       - moved to rvpfut.c (like ATS0, for PDP contexts)
 * AT*MGCOUNT      - controls the reporting of SNDCP data counters
 * AT+CGDATA       - activates a PDP context but does not enter data state
 * AT+CGDCONT      - configures a PDP context
 * AT+CGPADDR      - displays configured PDP context profiles
 * AT+CGTFT        - specifies a Packet Filter for a Traffic Flow Template
 * AT+CGREG        - configures notification of registration information changes
 * AT+CGSMS        - configures default route taken by Short messages (CS or GPRS)
 * ATD*            - attempts to make a GPRS data connection
 * AT*MLOOPPSD     - configures PSD in loopback mode
 * AT*MPPPCONFIG   - configures GPRS PPP timers and counters
 * AT*MSACL        - enables/disables ACL functionality
 * AT*MLACL        - displays ACL list
 * AT*MWACL        - writes an ACL entry
 * AT*MDACL        - deletes an ACL entry
 * AT+CGCONTRDP    - Displays parameters assigned by the network for the PDP
 *                   context
 * AT+CGSCONTRDP   - parameters assigned by the network for the PDP
 *                   context
 * AT+CGTFTRDP     - Displays TFT parameters assigned by the network for the
 *                   PDP context
 * AT*MCGDEFCONT   - Set default PSD connection settings
 * AT+CGDEL        - Delete non-active PDP Context(s)
 * AT+CGAUTH       - Set username and password for PDN Connection (3GPPP 27.007
 *                   release 11)
 * For LTE/NB-IOT:
 * AT+CEREG        - Displays EPS network registration status
 * AT+CGEQOS       - Set LTE specific QoS parameters
 * AT+CGEQOSRDP    - Displays LTE specific QoS parameters assigned by the
 *                   network for the PDP context
 **************************************************************************/

#define MODULE_NAME "RVGPUT"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <gkisig.h>
#include <gkimem.h>
#include <gkitimer.h>
#include <ki_typ.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvprof.h>
#include <rvchman.h>
#include <rvgpuns.h>
#include <rvpdsigo.h>
#include <rvpdsigi.h>
#include <rvmmut.h>
#include <rvgput.h>
#include <rvoman.h>
#include <rvcrhand.h>
#include <rvccut.h>
#include <rvccsigi.h>
#include <rvcimxut.h>
#include <rvcimux.h>

#if defined (USE_L4MM_ALLOC_MEMORY)
#include <l4mm_api.h>
#else /* USE_L4MM_ALLOC_MEMORY */
#if defined (USE_BMM_ALLOC_MEMORY)
#include <bmm.h>
#else
#include <tmm.h>
#endif
#endif /* USE_L4MM_ALLOC_MEMORY */

#include <sn_sig.h>
#include <stdlib.h>

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

#define GPRS_MIN_DIAL_STRING_SIZE   (3)
#define NUM_GPRS_SC_DIGITS          (2)
#define GPRS_SC_DIG_1               ('9')
#define GPRS_SC_DIG_2               GPRS_SC_DIG_1
#define DIAL_STRING_OFFSET          (3)
#if defined (FEA_PPP)
#define GPRS_L2P_PPP                ('1')
#endif /* FEA_PPP */
#define GPRS_ATTACH_REQ             (1)
#define GPRS_DETACH_REQ             (0)
#define GPRS_COMBINED_DETACH_REQ    (2)
#define LOOPPSD_ENABLE              (1)
#define LOOPPSD_DISABLE             (0)
#define LOOPPSD_MAX_DL_XMIT         (100)
#define LOOPPSD_DEFAULT_DL_XMIT     (1)
#define LOOPPSD_TWO_WAY_MODE        (0)
#define LOOPPSD_DL_MODE             (1)
#define LOOPPSD_UL_MODE             (2)
uint32_t LOOPPSD_MIN_TIMEOUT = (TICKS_TO_MILLISECONDS(1)==0) ? 1 : TICKS_TO_MILLISECONDS(1);
#define LOOPPSD_DEFAULT_PCKT_SIZE   (1500)

#define GPRS_PARAM_NOT_SUPPLIED     (0xFFFF)

#define VG_MAX_CPDATA_LENGTH        (750)

/* added for job104730 */
#define IP_MTU    (1500) /* Maximum Transmission Unit over GPRS */
#define IP_LEN    (20)   /* IP  header Length */
#define UDP_LEN   (8)    /* UDP header Length */
#define TCP_LEN   (20)   /* TCP header Length */
#if !defined (min)
# define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

static const SupportedPDNTypesMap supportedPDNTypesMap[] =
{
  {(const Char*)"IP",       PDN_TYPE_IPV4,          TRUE , 0 },
  {(const Char*)"IPV6",     PDN_TYPE_IPV6,          TRUE,  1 },
  {(const Char*)"IPV4V6",   PDN_TYPE_IPV4V6,        TRUE,  2 },
  {(const Char*)"Non-IP",   PDN_TYPE_NONIP,         TRUE,  3 },
  {TERMINATOR_STRING,       PDN_TYPE_UNUSED,        FALSE, UCHAR_MAX }
};

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static void viewActivationState        (Char const *prompt,
                                         const VgmuxChannelNumber entity);
static void viewCGDCONTRange           (const VgmuxChannelNumber entity);

static void viewCGDCONTCidInfo (const VgmuxChannelNumber entity, Int8 cid, PdnType pdnType);

void vgIPV6SubnetMaskIntToDisplayStr (PdnAddress inAddr,
                                      TextualPdnAddress *outAddr,
                                      const VgmuxChannelNumber entity);

static Boolean vgPDNStrToEnum          (Char *string,
                                         PdnType *enumValue);
static void viewCGDCONTAttributes      (const VgmuxChannelNumber entity);
static void viewPDNAddr                (PdnType pdpTypeNumber,
                                        const PdnAddress *cmd_p,
                                         const Int8 cid,
                                          const VgmuxChannelNumber entity);

static void viewPDNAddrGeneric (PdnType pdnType,
                                const PdnAddress *address,
                                const VgmuxChannelNumber entity);

static void vgCheckProfileUndefined (VgPsdStatusInfo *ptr, Int32 thisCid);

#if defined (FEA_QOS_TFT)
static void vgDisplayCGTFTRDPInfo             (const VgmuxChannelNumber entity, Int8 cid);
static void vgDisplayAllCGTFTRDPInfo          (const VgmuxChannelNumber entity);
static void vgInitialiseCgtftData             (VgPsdStatusInfo *ptr);
static void vgInitialiseCgeqosData            (VgPsdStatusInfo *ptr);
static void viewEqosAttributes                (const VgmuxChannelNumber entity);
static void vgDisplayCGEQOSRDPInfo            (const VgmuxChannelNumber entity, Int8 cid);
static void vgDisplayAllCGEQOSRDPInfo         (const VgmuxChannelNumber entity);
static void vgView3gTftAttributes             (const VgmuxChannelNumber entity);
static void vgView3gTftRange                  (const VgmuxChannelNumber entity);

/* Functions for convertion of textual TFT values to numeric and back to textual
 */
static Boolean vgConvertDotSeparatedTextualRemoteAddressSubnetMaskToNumeric (RemoteAddrSubnetMask *input);
static Boolean vgConvertColonSeparatedTextualRemoteAddressSubnetMaskToNumeric (RemoteAddrSubnetMask *input);
static Boolean vgConvertTextualRemoteAddressSubnetMaskToNumeric (RemoteAddrSubnetMask *input,
                                                                 const VgmuxChannelNumber entity);
static Boolean vgConvertTextualPortRangeToNumeric (PortRange *input);
static Boolean vgConvertTextualTosTrfcClassToNumeric (TosTrfcClass *input);
static Boolean vgConvertNumericRemoteAddressSubnetMaskToTextual (RemoteAddrSubnetMask *input,
                                                                 const VgmuxChannelNumber entity);
static Boolean vgConvertNumericPortRangeToTextual (PortRange *input);
static Boolean vgConvertNumericTosTrfcClassToTextual (TosTrfcClass *input);
#endif /* FEA_QOS_TFT */

static Boolean vgDisplayUsernamePasswordInfo  (const VgmuxChannelNumber entity, Int8 cid,
                                               Boolean firstTime);
static void vgDisplayAllUsernamePasswordInfo  (const VgmuxChannelNumber entity);

static void vgInitialiseCgdcontData  (VgPsdStatusInfo *ptr);
#if defined (FEA_DEDICATED_BEARER)
static void vgInitialiseCgdscontData (VgPsdStatusInfo *ptr);
#endif /* FEA_DEDICATED_BEARER */
static void vgInitialisePsdUserData  (VgPsdStatusInfo *ptr);

static void vgDisplayCGCONTRDPBasicInfo  (const VgmuxChannelNumber entity, Int8 cid);
static void vgDisplayCGCONTRDPAddrInfo   (const VgmuxChannelNumber entity, Int8 cid, PdnType pdnType);
static void vgDisplayCGCONTRDPInfo       (const VgmuxChannelNumber entity, Int8 cid);
static void vgDisplayAllCGCONTRDPInfo    (const VgmuxChannelNumber entity);

#if defined (FEA_DEDICATED_BEARER)
static void vgDisplayCGSCONTRDPInfo      (const VgmuxChannelNumber entity, Int8 cid);
static void vgDisplayAllCGSCONTRDPInfo   (const VgmuxChannelNumber entity);
#endif

static void vgDeleteNonActiveContext     (const VgmuxChannelNumber entity, Int8 cid, Boolean firstTime);
static void vgDeleteAllNonActiveContexts (const VgmuxChannelNumber entity);

static Boolean vgCheckCidAvailable (Int8 cid,
                             const VgmuxChannelNumber entity);
static Boolean vgGpGetCidActiveStatus (Int8 cid);

#if defined (FEA_DEDICATED_BEARER)
static void viewCGDSCONTRange          (const VgmuxChannelNumber entity);
static void viewCGDSCONTAttributes     (const VgmuxChannelNumber entity);
#endif

static void vgPrintActiveCids          (Char const *prompt_p,
                                         const VgmuxChannelNumber entity,
                                          Boolean primaryContexts,
                                           Boolean secondaryContexts);

static ResultCode_t vgProcMgSinkExtAssign (CommandLine_t *commandBuffer_p,
                                           const VgmuxChannelNumber entity);

static void vgProcMgSinkTxOnePduOnNsapi (VgMGSINKData *vgMGSINKData_p);

static ResultCode_t vgProcMgtcSinkExtAssign (CommandLine_t *commandBuffer_p,
                                             const VgmuxChannelNumber entity);

static void vgProcMgtcSinkTxOnePduOnNsapi (VgMGSINKData *vgMGSINKData_p);

static void vgGpDtAddConnIdFromActiveCid( Int8 cidNum, AbpdApnDataType dataType,
                                                   Boolean *isValid_p);

static void vgGpDisplayApnExceptionReportsInfo( Int8 cidNum, const VgmuxChannelNumber entity);


/***************************************************************************
 * Variables
 ***************************************************************************/

/***************************************************************************
 * Macros
 ***************************************************************************/

/* added for job104730 */
union Signal
{
  SnDataReq                     snDataReq;
  ApexAbpdWriteApnDataTypeReq   apexAbpdWriteApnDataTypeReq;
  ApexAbpdReadApnDataTypeReq    apexAbpdReadApnDataTypeReq;
};

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*************************************************************************
*
* Function:     vgDeactivateContext
*
* Parameters:   cid    - the pdp context to be deactivated
*               entity - mux channel number
*
* Returns:      ResultCode_t - success of deactivation
*
* Description:  Deactivates the context identified by the given 'cid'
*
*************************************************************************/

static ResultCode_t vgDeactivateContext (Int32 cid,
                                          const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  ResultCode_t          result;

  VgPsdStatusInfo       *ptr           = gprsGenericContext_p->cidUserData[cid];
  VgmuxChannelNumber    entityLinkedToCid;
  GprsContext_t         *gprsContext_p;
  OpmanContext_t        *opManContext_p;


  entityLinkedToCid = vgFindEntityLinkedToCidWithoutCheckEntity(cid);

  if ((entityLinkedToCid != VGMUX_CHANNEL_INVALID) &&
      (entityLinkedToCid != entity))
  {
    gprsContext_p  = ptrToGprsContext(entityLinkedToCid);
    opManContext_p = ptrToOpManContext (entityLinkedToCid);
  }
  else
  {
    gprsContext_p  = ptrToGprsContext(entity);
    opManContext_p = ptrToOpManContext (entity);
  }

  FatalAssert (gprsContext_p != PNULL);

  FatalAssert (cid < MAX_NUMBER_OF_CIDS);

  /* check profile is defined */
  if (ptr->profileDefined)
  {
    /* if profile is active send a hangup signal */
      if (ptr->isActive)
      {
        gprsContext_p->vgHangupType = VG_HANGUP_TYPE_DTR_DROPPED;
        gprsContext_p->vgHangupCid  = (Int8)cid;

        /* A different channel from the one that owns the CID is attempting to
         * disconnected it.  This is allowed but we have to remember the channel
         */
        if (entityLinkedToCid != entity)
        {
          /* Also set the hangupCid for this channel - may be different from
           * the one which "owns" the CID.  Makes handling hangup easier!
           */
          gprsGenericContext_p->hangupChannel = entity;
          ptrToGprsContext(entity)->vgHangupCid  = (Int8)cid;
        }
        /* Otherwise set hangup channel to invalid to be safe */
        else
        {
          gprsGenericContext_p->hangupChannel = VGMUX_CHANNEL_INVALID;
        }

        /* Make sure we set the disconnectionItem correctly */
        gprsContext_p->disconnectionItem = opManContext_p->callInfo.vgIdent;

        result = vgChManContinueAction (entity, SIG_APEX_ABPD_HANGUP_REQ);

      }
      else
      {
        /* Context not active anyway so should return error */
        result = RESULT_CODE_ERROR;
      }
  }
  else
  {
    /* the profile is not defined */
    result = VG_CME_PROFILE_NOT_DEFINED;
  }

  return (result);
}


/*************************************************************************
*
* Function:     viewActivitationState
*
* Parameters:   prompt - header for AT output
*               entity - mux channel number
*
* Returns:      nothing
*
* Description:  outputs the activation state of all defined contexts,
*               together with their cid values.
*
*************************************************************************/
static void viewActivationState (Char const *prompt,
                                  const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo       *cmd_p;
  Int8                  profile;
  Boolean               firstProfile = TRUE;

  /* go through all profiles */
  for (profile = vgGpGetMinCidValue(entity); profile < MAX_NUMBER_OF_CIDS; profile++)
  {
    /* check user data is present */
    if (gprsGenericContext_p->cidUserData [profile] != PNULL)
    {
      cmd_p = gprsGenericContext_p->cidUserData [profile];

      /* if the profile is defined then display activation state */
      if ((cmd_p->profileDefined) &&
          ((cmd_p->cgdcontDefined)
           || (cmd_p->cgdscontDefined)
           ))
      {
        if (firstProfile)
        {
          vgPutNewLine (entity);
          firstProfile = FALSE;
        }

        /* Profile is defined */
        vgPrintf (entity, (const Char*)"%s%d", prompt, profile);
        if (cmd_p->isActive == TRUE)
        {
          vgPrintf (entity, (const Char*)",1");
        }
        else
        {
          vgPrintf (entity, (const Char*)",0");
        }
        vgPutNewLine (entity);
      }
    }
  }
}

/*************************************************************************
*
* Function:     viewCGDCONTRange
*
* Parameters:   prompt - header to be displayed
*               entity - mux channel number
*
* Returns:      nothing
*
* Description:  Outputs the ranges for the PDP context attributes
*
*************************************************************************/

static void viewCGDCONTRange (const VgmuxChannelNumber entity)
{
  const SupportedPDNTypesMap *map = supportedPDNTypesMap;

  /* added for job106759 */
  /* job132261: print correct CID value */
  volatile Int8 maxCids = MAX_NUMBER_OF_CIDS - 1;

  while (map->arrIndx != UCHAR_MAX)
  {
    vgPutNewLine (entity);

    /* go through each pdp type displaying supported types */
    if (map->supported)
    {
      /* job106759: allow for variable number of PDP context records */
      vgPrintf (entity, (const Char*)"+CGDCONT: ");
      if (maxCids == 1)
      {
#if defined(FEA_TEMP_REL14_ABPD_DUAL_PRIORITY)
        vgPrintf (entity,
                  (const Char*)"(1),\"%s\",,,(%d-%d),(%d-%d),(0),,,,(%d-%d),,(%d-%d),,(%d-%d)",
                   map->str,
                   (Int32)DATA_COMP_OFF,
                   (Int32)DATA_COMP_V42BIS,
                   (Int32)HEADER_COMP_OFF,
                   (Int32)HEADER_COMP_RFC3095,
                   (Int32)VG_MS_CONFIGURED_FOR_LOW_PRIO,
                   (Int32)VG_MS_NOT_CONFIGURED_FOR_LOW_PRIO,
                   (Int32)MTU_SIZE_NOT_REQUESTED,
                   (Int32)MTU_SIZE_REQUESTED,
                   (Int32)MTU_SIZE_NOT_REQUESTED,
                   (Int32)MTU_SIZE_REQUESTED);
#else
      vgPrintf (entity,
                (const Char*)"(1),\"%s\",,,(%d-%d),(%d-%d),(0),,,,,,(%d-%d),,(%d-%d)",
                 map->str,
                 (Int32)DATA_COMP_OFF,
                 (Int32)DATA_COMP_V42BIS,
                 (Int32)HEADER_COMP_OFF,
                 (Int32)HEADER_COMP_RFC3095,
                 (Int32)MTU_SIZE_NOT_REQUESTED,
                 (Int32)MTU_SIZE_REQUESTED,
                 (Int32)MTU_SIZE_NOT_REQUESTED,
                 (Int32)MTU_SIZE_REQUESTED);
#endif
      }
      else
      {
#if defined(FEA_TEMP_REL14_ABPD_DUAL_PRIORITY)
        vgPrintf (entity,
                  (const Char*)"(1-%d),\"%s\",,,(%d-%d),(%d-%d),(0),,,,(%d-%d),,(%d-%d),,(%d-%d)",
                   maxCids,
                   map->str,
                   (Int32)DATA_COMP_OFF,
                   (Int32)DATA_COMP_V42BIS,
                   (Int32)HEADER_COMP_OFF,
                   (Int32)HEADER_COMP_RFC3095,
                   (Int32)VG_MS_CONFIGURED_FOR_LOW_PRIO,
                   (Int32)VG_MS_NOT_CONFIGURED_FOR_LOW_PRIO,
                   (Int32)MTU_SIZE_NOT_REQUESTED,
                   (Int32)MTU_SIZE_REQUESTED,
                   (Int32)MTU_SIZE_NOT_REQUESTED,
                   (Int32)MTU_SIZE_REQUESTED);
#else
      vgPrintf (entity,
                (const Char*)"(1-%d),\"%s\",,,(%d-%d),(%d-%d),(0),,,,,,(%d-%d),,(%d-%d)",
                 maxCids,
                 map->str,
                 (Int32)DATA_COMP_OFF,
                 (Int32)DATA_COMP_V42BIS,
                 (Int32)HEADER_COMP_OFF,
                 (Int32)HEADER_COMP_RFC3095,
                 (Int32)MTU_SIZE_NOT_REQUESTED,
                 (Int32)MTU_SIZE_REQUESTED,
                 (Int32)MTU_SIZE_NOT_REQUESTED,
                 (Int32)MTU_SIZE_REQUESTED,
#endif
      }
    }
    map++;
  }

  vgPutNewLine (entity);
}

/*************************************************************************
*
* Function:     viewCGDCONTCidInfo
*
* Parameters:   entity - mux channel number
*                     cid - cid number
*                     pdnType - PDN type
*
* Returns:      nothing
*
* Description:  Outputs the CGDCONT cid related context attributes
*
*************************************************************************/

static void viewCGDCONTCidInfo (const VgmuxChannelNumber entity, Int8 cid, PdnType pdnType)
{
    GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
    VgPsdStatusInfo       *cmd_p = PNULL;
    Boolean              enumFound = FALSE;
    Int8                 arrIndx;
    Int8                 apnChar;

    cmd_p = gprsGenericContext_p->cidUserData [cid];

    vgPutNewLine (entity);
    vgPrintf (entity, (const Char*)"+CGDCONT: ");
    vgPrintf (entity, (const Char*)"%d,\"", cid);

    /* Display negotiated PDN address */
    enumFound = vgPDNTypeToIndx (cmd_p->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress.pdnType, &arrIndx);

    if (enumFound == TRUE)
    {
      vgPrintf (entity, (const Char*)"%s", supportedPDNTypesMap[arrIndx].str);
    }
    vgPrintf (entity, (const Char*)"\",\"");

    /* display negotiated APN if present */
    if ((cmd_p->psdBearerInfo.negApnPresent == TRUE) && (vgOpManCidActive(cid)))
    {
      for (apnChar = 0;
            apnChar < cmd_p->psdBearerInfo.negTextualApn.length;
             apnChar++)
      {
        vgPutc (entity, cmd_p->psdBearerInfo.negTextualApn.name[apnChar]);
      }
    }
    else if (cmd_p->psdBearerInfo.reqApnPresent == TRUE)
    {
      for (apnChar = 0;
            apnChar < cmd_p->psdBearerInfo.reqTextualApn.length;
             apnChar++)
      {
        vgPutc (entity, cmd_p->psdBearerInfo.reqTextualApn.name[apnChar]);
      }
    }
    vgPrintf (entity, (const Char*)"\",\"");

    /* display PDN address if present */
    if (cmd_p->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress.addressPresent)
    {
      TextualPdnAddress convertedPdnAddress;
      /* PdnAddress has to be converted from type 0000 to type
       * 000.000.000.000 or FF FF FF FF to 255.255.255.255 (IPV4) or whatever
       * format is set by AT+CGPIAF (IPV6) to display for +CGDCONT */

      if (PDN_TYPE_IPV4 == pdnType)
      {
          vgPDNAddrIntToDisplayStr (pdnType,
                                                    cmd_p->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress,
                                                    &convertedPdnAddress, entity);
          vgPrintf (entity, (const Char*)"%.*s",
                    convertedPdnAddress.length, convertedPdnAddress.address);
      }
      else
      if (PDN_TYPE_IPV6 == pdnType)
      {
        vgPDNAddrIntToDisplayStr (pdnType,
                                                  cmd_p->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress,
                                                  &convertedPdnAddress, entity);
          vgPrintf (entity, (const Char*)"%.*s",
                    convertedPdnAddress.length, convertedPdnAddress.address);
      }
      /* For non-IP we don't print anything */
    }
    else if (cmd_p->psdBearerInfo.reqPdnAddress.addressPresent)
    {
      TextualPdnAddress convertedPdnAddress;
      /* PdnAddress has to be converted from type 0000 to type
       * 000.000.000.000 or FF FF FF FF to 255.255.255.255 (IPV4) or whatever
       * format is set by AT+CGPIAF (IPV6) to display for +CGDCONT */
      vgPDNAddrIntToDisplayStr (cmd_p->psdBearerInfo.reqPdnAddress.pdnType,
                                      cmd_p->psdBearerInfo.reqPdnAddress,
                                      &convertedPdnAddress,
                                      entity);
      vgPrintf (entity, (const Char*)"%.*s",
                convertedPdnAddress.length, convertedPdnAddress.address);
    }

#if defined(FEA_TEMP_REL14_ABPD_DUAL_PRIORITY)
    vgPrintf (entity,
              (const Char*)"\",%d,%d,0,,,,%d,,%d,,%d",
               cmd_p->psdBearerInfo.dataComp,
                cmd_p->psdBearerInfo.headerComp,
                cmd_p->psdBearerInfo.nasSigPriority,
                cmd_p->psdBearerInfo.ipv4LinkMTURequest,
                cmd_p->psdBearerInfo.nonIPLinkMTURequest);
#else
    vgPrintf (entity,
              (const Char*)"\",%d,%d,0,,,,,,%d,,%d",
               cmd_p->psdBearerInfo.dataComp,
                cmd_p->psdBearerInfo.headerComp,
                cmd_p->psdBearerInfo.ipv4LinkMTURequest,
                cmd_p->psdBearerInfo.nonIPLinkMTURequest);
#endif
}

/*************************************************************************
*
* Function:     vgIPV6SubnetMaskIntToDisplayStr
*
* Parameters:   inAddr  - address, in binary
*               outAddr - address, as a human-readable string
*               entity  - the entity on which the data will be displayed.
*
* Returns:      nothing
*
* Description:  Converts IVP6 subnetmask from numeric format to textual.
*               This format depends on the CGPIAF setting.  This is appended
*               to the source address string by the calling function.
*
*************************************************************************/
void vgIPV6SubnetMaskIntToDisplayStr (PdnAddress inAddr, TextualPdnAddress *outAddr,
                                      const VgmuxChannelNumber entity)
{
  Int8 subnetMaskCount;
  Int8 bitMask;
  Int8 subnetBitCount = 0;
  Boolean done;

  if (getProfileValue (entity, PROF_CGPIAF + PROF_CGPIAF_IPV6_ADDR_FORMAT) == PROF_CGPIAF_ENABLE)
  {
    if (getProfileValue (entity, PROF_CGPIAF + PROF_CGPIAF_IPV6_SUBNET_NOTATION) == PROF_CGPIAF_ENABLE)
    {
      /* Subnet notation is the "/x" format */
      /* Count the number of bits in the subnet mask set to 1 */
      for (subnetMaskCount = 0; subnetMaskCount < MAX_IPV6_ADDR_LEN; subnetMaskCount++)
      {
        bitMask = 0x80;
        done = FALSE;

        while (!done)
        {
          if ((inAddr.ipv6Address[subnetMaskCount] & bitMask) == bitMask)
          {
            subnetBitCount++;
          }

          if (bitMask == 1)
          {
            done = TRUE;
          }
          else
          {
            bitMask >>= 1;
          }
        }
      }

      /* String can be up to 4 characters long ("/128") - but buffer size
       * set to 5 for the trailing '\0'
       */
      snprintf((char *)outAddr->address, 5,
                "/%d",
                subnetBitCount);
      outAddr->length = strlen ((char *)outAddr->address);
    }
    else
    {
      /* Add a space to the string to which the subnet mask string is being written
       * to
       */
      outAddr->address[0] = ' ';
      vgIpv6PDNAddrIntToStr (inAddr, outAddr, 1, entity);
    }
  }
  else
  {
    /* Original dot separated format */
    /* Add a "." to the string to which the subnet mask string is being written
     * to
     */
    outAddr->address[0] = '.';
    vgIpv6PDNAddrIntToStr (inAddr, outAddr, 1, entity);
  }
}

/*************************************************************************
*
* Function:     vgPDNStrToEnum
*
* Parameters:   string    - the string of the enum to extract
*               enumValue - PDN type enum associated with string
*
* Returns:      Boolean   - success of string lookup
*
* Description:  Iterates through map until string is found and then
*               returns the enum value
*
*************************************************************************/

static Boolean vgPDNStrToEnum (Char *string, PdnType *enumValue)
{
  const SupportedPDNTypesMap  *ptr = supportedPDNTypesMap;
  Boolean                     entryFound = FALSE;

  if (ptr->arrIndx != UCHAR_MAX)
  {
    /* go through each pdp type comparing supported types */
    do
    {
      /* job120348: allow lower case PDP type string */
      /* String length to be compared */
      if ((vgStrLen (string) == vgStrLen(ptr->str)) && (vgStriNCmp (string, ptr->str, vgStrLen (string)) == 0))
      {
        *enumValue = ptr->PDNType;
        entryFound = TRUE;
      }
      ptr++;
    }
    while ((ptr->arrIndx != UCHAR_MAX) && (entryFound == FALSE));
  }

  return (entryFound);
}

/*************************************************************************
*
* Function:     viewCGDCONTAttributes
*
* Parameters:   prompt - header for AT output
*               entity - mux channel number
*
* Returns:      nothing
*
* Description:  outputs the current values for each of the context profiles.
*
*************************************************************************/

static void viewCGDCONTAttributes (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo      *cmd_p;
  Int8                 profile;

  /* go through all profiles */
  for (profile = vgGpGetMinCidValue(entity); profile < MAX_NUMBER_OF_CIDS; profile++)
  {
    /* check user data is present */
    if (gprsGenericContext_p->cidUserData[profile] != PNULL)
    {
      cmd_p = gprsGenericContext_p->cidUserData [profile];

      /* if the profile is defined then display attributes */
      if ((cmd_p->profileDefined) &&
          (cmd_p->cgdcontDefined)
#if defined (FEA_DEDICATED_BEARER)
          && (!cmd_p->psdBearerInfo.secondaryContext)
#endif
          )
      {
        if((cmd_p->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress.pdnType == PDN_TYPE_IPV4V6) &&
          (cmd_p->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress.addressPresent == TRUE))
        {
            viewCGDCONTCidInfo(entity, profile, PDN_TYPE_IPV4);
            viewCGDCONTCidInfo(entity, profile, PDN_TYPE_IPV6);
        }
        else
        {
            viewCGDCONTCidInfo(entity, profile, cmd_p->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress.pdnType);
        }
        }
      }
    }
  vgPutNewLine (entity);
}

/*************************************************************************
*
* Function:     viewPDNAddr
*
* Parameters:   pdnType - type of address
*               address - PDN address in a context profile.
*               cid     - the PSD profile to be displayed with the address
*               entity  - mux channel number
*
* Returns:      void
*
* Description:  outputs the given cid value and address if present
*               Note that this is for AT+CGPADDR command only
*
*************************************************************************/

static void viewPDNAddr (PdnType pdnType,
                         const PdnAddress *address,
                         const Int8 cid,
                         const VgmuxChannelNumber entity)
{
  /* cid is printed even if the PDP address is not available */
  vgPrintf (entity, (const Char*)"+CGPADDR: %d", cid);

  if (address->addressPresent)
  {
    TextualPdnAddress convertedPdnAddress;
    /* PdnAddress has to be converted from type 0000 to type
     * 000.000.000.000 or FF FF FF FF to 255.255.255.255 (IPV4) or whatever
     * format is set by AT+CGPIAF (IPV6) to display for +CGPADDR */

    if (PDN_TYPE_IPV4 == pdnType)
    {
        vgPDNAddrIntToDisplayStr (pdnType, *address, &convertedPdnAddress, entity);
        vgPrintf (entity, (const Char*)",\"%.*s\"",
                  convertedPdnAddress.length, convertedPdnAddress.address);
    }
    else
    if (PDN_TYPE_IPV6 == pdnType)
    {
        vgPDNAddrIntToDisplayStr (pdnType, *address, &convertedPdnAddress, entity);
        vgPrintf (entity, (const Char*)",\"%.*s\"",
                  convertedPdnAddress.length, convertedPdnAddress.address);
    }
    else
    if (PDN_TYPE_IPV4V6 == pdnType)
    {
        vgPDNAddrIntToDisplayStr (PDN_TYPE_IPV4, *address, &convertedPdnAddress, entity);
        vgPrintf (entity, (const Char*)",\"%.*s\"",
                  convertedPdnAddress.length, convertedPdnAddress.address);
        vgPDNAddrIntToDisplayStr (PDN_TYPE_IPV6, *address, &convertedPdnAddress, entity);
        vgPrintf (entity, (const Char*)",\"%.*s\"",
                  convertedPdnAddress.length, convertedPdnAddress.address);
    }
    /* For non-IP we don't print anything */
  }

  vgPutNewLine (entity);
}

/*************************************************************************
*
* Function:     viewPDNAddrGeneric
*
* Parameters:   pdnType - type of address
*               address - PDN address in a context profile.
*               entity  - mux channel number
*
* Returns:      void
*
* Description:  outputs the given cid value and address if present
*               This is generic for any command requiring display of
*               an IP address.  Note that it is up to the caller to
*               call this for IPV4 and IPV6 addresses separately.
*
*************************************************************************/
static void viewPDNAddrGeneric (PdnType pdnType,
                                const PdnAddress *address,
                                const VgmuxChannelNumber entity)
{
  if (address->addressPresent)
  {
    TextualPdnAddress convertedPdnAddress;
    /* PdnAddress has to be converted from type 0000 to type
     * 000.000.000.000 or FF FF FF FF to 255.255.255.255 (IPV4) or whatever
     * format is set by AT+CGPIAF (IPV6) to display for +CGPADDR */

    vgPDNAddrIntToDisplayStr (pdnType, *address, &convertedPdnAddress, entity);
    vgPrintf (entity, (const Char*)"\"%.*s\"",
              convertedPdnAddress.length, convertedPdnAddress.address);
  }
}

/*************************************************************************
*
* Function:     vgPrintActiveCids
*
* Parameters:   prompt_p - header for AT output
*               entity - mux channel number
*               primaryContexts - display the primary contexts
*               secondaryContexts - display the secondary contexts
*
* Returns:      void
*
* Description:  prints the cids associated with active contexts
*
*************************************************************************/
static void vgPrintActiveCids (Char const *prompt_p,
                               const VgmuxChannelNumber entity,
                               Boolean primaryContexts,
                               Boolean secondaryContexts)
{

  VgPsdStatusInfo       *cmd_p = PNULL;

  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  Int8                  profile;
  Boolean               firstEntry = TRUE;

  Boolean               foundAnEntry = FALSE;

  /* First see if we have any entries.  If we have at least one then we carry
   * on display - otherwise display nothing.
   */
  for (profile = vgGpGetMinCidValue(entity); profile < MAX_NUMBER_OF_CIDS; profile++)
  {
    /* check user data is present */
    if (gprsGenericContext_p->cidUserData [profile] != PNULL)
    {
      cmd_p = gprsGenericContext_p->cidUserData [profile];

      /* if the profile is defined and active, and it is primary or secondary
       * depending on what we want to display then  we found one
       */
      if ((cmd_p->profileDefined) && (cmd_p->isActive)
#if defined (FEA_DEDICATED_BEARER)
           && ((!(cmd_p->psdBearerInfo.secondaryContext) && primaryContexts) ||
           ((cmd_p->psdBearerInfo.secondaryContext) && secondaryContexts)))
#else
           && primaryContexts)
#endif
      {
        foundAnEntry = TRUE;
        break;
      }
    }
  }

  if (foundAnEntry)
  {
    vgPutNewLine (entity);
    vgPrintf (entity, (const Char*)"%s", prompt_p);

    /* go through all profiles */
    for (profile = vgGpGetMinCidValue(entity); profile < MAX_NUMBER_OF_CIDS; profile++)
    {
      /* check user data is present */
      if (gprsGenericContext_p->cidUserData [profile] != PNULL)
      {
        cmd_p = gprsGenericContext_p->cidUserData [profile];

        /* if the profile is defined and active, and it is primary or secondary
         * depending on what we want to display then display the cid */
        if ((cmd_p->profileDefined) && (cmd_p->isActive) &&
#if defined (FEA_DEDICATED_BEARER)
           ((!(cmd_p->psdBearerInfo.secondaryContext) && primaryContexts) ||
           ((cmd_p->psdBearerInfo.secondaryContext) && secondaryContexts)))
#else
           primaryContexts)
#endif
        {
          if (firstEntry)
          {
            vgPutc (entity, ' ');
            vgPutc (entity, '(');
            firstEntry = FALSE;
          }
          else
          {
            vgPutc (entity, ',');
          }
          vgPrintf (entity, (const Char*)"%d", profile);
        }
      }
    }

    if (firstEntry == FALSE)
    {
      /* there's at least one entry in the list */
      vgPutc (entity, ')');
    }
    vgPutNewLine (entity);
  }
}
#if defined (FEA_ACL)

/*--------------------------------------------------------------------------
*
* Function:    readAndCheckPin2
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Reads and checks PIN2. If already confirmed, returns OK.
*-------------------------------------------------------------------------*/
static ResultCode_t readAndCheckPin2 (CommandLine_t *commandBuffer_p,
                                      const VgmuxChannelNumber entity )
{
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
  GeneralContext_t         *generalContext_p        = ptrToGeneralContext (entity);
  Boolean                  passwdPresent;
  ResultCode_t             result                   = RESULT_CODE_OK;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  /* get password parameter */
  passwdPresent = getExtendedString (commandBuffer_p,
                                     generalContext_p->password,
                                     SIM_CHV_LENGTH,
                                     &generalContext_p->passwordLength);

  /* if PIN2 has not been verified yet, do it now */
  if (simLockGenericContext_p->simInfo.pin2Verified == FALSE)
  {
    if (passwdPresent == TRUE)
    {
      if (generalContext_p->passwordLength > 0)
      {
        memset (&generalContext_p->password[generalContext_p->passwordLength],
                UCHAR_MAX,
                SIM_CHV_LENGTH - generalContext_p->passwordLength);

        memset (&generalContext_p->newPassword[0],
                UCHAR_MAX,
                SIM_CHV_LENGTH);

        /* send off request to check password */
        generalContext_p->pinFunction = SIM_PIN_FUNCT_VERIFY;
        result =  sendVerifyPin2Request (entity);
      }
      else
      {
        result = VG_CME_INCORRECT_PASSWORD;
      }
    }
    else
    {
      /* password required but not in command */
      result = VG_CME_SIM_PIN2_REQUIRED;
    }
  }
  return (result);
}
#endif
#if defined (FEA_QOS_TFT)
/*************************************************************************
*
* Function:     vgView3gTftAttributes
*
* Parameters:   entity - mux channel number
*
* Returns:      nothing
*
* Description:  This function outputs the current 3g Tft values for each
*               of the context profiles.
*
*************************************************************************/
static void vgView3gTftAttributes (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo       *cmd_p;
  Int8                  profile, pfind;
  Boolean               firstProfile = TRUE;
  TrafficFlowTemplate   *tft_p =PNULL;
  PacketFilter          *pf_p = PNULL;
  PacketFilter          *tempPf_p = PNULL;

  /* Allocate memory */
  KiAllocMemory (sizeof(PacketFilter), (void **)&tempPf_p);

  /* go through all profiles */
  for (profile = vgGpGetMinCidValue(entity); profile < MAX_NUMBER_OF_CIDS; profile++)
  {
    /* check cid user values exist */
    if (gprsGenericContext_p->cidUserData[profile] != PNULL)
    {
      cmd_p = gprsGenericContext_p->cidUserData [profile];

      if ((cmd_p->psdBearerInfo.reqTftPresent) &&
          (cmd_p->cgtftDefined))
      {
        if (firstProfile)
        {
          vgPutNewLine (entity);
          firstProfile = FALSE;
        }

        tft_p = &cmd_p->psdBearerInfo.requiredTft;

        for (pfind = 0; pfind < MAX_PFS_IN_TFT; pfind++)
        {
          pf_p = &tft_p->packetFilterData[pfind];

          /* Copy the packet filter so we can convert numeric values to text
           * if necessary.
           */
          *tempPf_p = *pf_p;

          if (tempPf_p->packetFilterId != 0)
          {
            vgPrintf (entity, (const Char*)"+CGTFT: %d", profile);
            vgPutc (entity, ',');

            vgPrintf (entity, (const Char*)"%d,%d", tempPf_p->packetFilterId, tempPf_p->evalPrecedenceIndex);
            vgPutc (entity, ',');

            if (tempPf_p->remoteAddrSubnetMask.present)
            {
              /* Convert the addresses to text first */
              if (vgConvertNumericRemoteAddressSubnetMaskToTextual (&tempPf_p->remoteAddrSubnetMask, entity))
              {
                vgPrintf (entity, (const Char*)"\"%s\"", tempPf_p->remoteAddrSubnetMask.val);
              }
            }
            vgPutc (entity, ',');

            if (tempPf_p->protocolNumNextHdr.present)
            {
              vgPrintf (entity, (const Char*)"%d", tempPf_p->protocolNumNextHdr.val);
            }
            vgPutc (entity, ',');

            if (tempPf_p->localPortRange.present)
            {
              if (vgConvertNumericPortRangeToTextual (&tempPf_p->localPortRange))
              {
                vgPrintf (entity, (const Char*)"\"%s\"", tempPf_p->localPortRange.val);
              }
            }
            vgPutc (entity, ',');

            if (tempPf_p->remotePortRange.present)
            {
              if (vgConvertNumericPortRangeToTextual (&tempPf_p->remotePortRange))
              {
                vgPrintf (entity, (const Char*)"\"%s\"", tempPf_p->remotePortRange.val);
              }
            }
            vgPutc (entity, ',');

            if (tempPf_p->ipsecSpi.present)
            {
              vgPrintf (entity, (const Char*)"%lX", tempPf_p->ipsecSpi.val);
            }
            vgPutc (entity, ',');

            if (tempPf_p->tosTrfcClass.present)
            {
              if (vgConvertNumericTosTrfcClassToTextual (&tempPf_p->tosTrfcClass))
              {
                vgPrintf (entity, (const Char*)"\"%s\"", tempPf_p->tosTrfcClass.val);
              }
            }
            vgPutc (entity, ',');

            if (tempPf_p->flowLabel.present)
            {
              vgPrintf (entity, (const Char*)"%X", tempPf_p->flowLabel.val);
            }

            /* the coding scheme for AT command and NAS signalling is not the same */

            if (PF_DIR_DOWNLINK_ONLY == tempPf_p->packetFilterDirection)
            {
              vgPrintf (entity, (const Char*)",2");
            }
            else if (PF_DIR_UPLINK_ONLY == tempPf_p->packetFilterDirection)
            {
              vgPrintf (entity, (const Char*)",1");
            }
            else
            {

              vgPrintf (entity, (const Char*)",%d", tempPf_p->packetFilterDirection);
            }
            vgPutNewLine (entity);
          }
        }
      }
    }
  }

  /* Free the memory for temporary packet filter */
  KiFreeMemory ((void **)&tempPf_p);
}

/*************************************************************************
*
* Function:     vgView3gTftRange
*
* Parameters:   entity - mux channel number
*
* Returns:      nothing
*
* Description:  This function outputs the range of 3g Tft values
*
*************************************************************************/
static void vgView3gTftRange (const VgmuxChannelNumber entity)
{
  const SupportedPDNTypesMap *map_p = supportedPDNTypesMap;

  while (map_p->arrIndx != UCHAR_MAX)
  {
    vgPutNewLine (entity);

    /* go through each pdp type displaying supported types */
    if (map_p->supported)
    {
      /* Print PDP type */
      vgPrintf (entity, (const Char*)"+CGTFT:\"%s\",", map_p->str);
      /* PFI range */
      vgPrintf (entity, (const Char*)"(%d-%d),", MIN_PFI, MAX_PFI);
      /* Evaluation precedence index */
      vgPrintf (entity, (const Char*)"(%d-%d),", 0, MAX_EVAL_PREC_IND);

      /* Src address & subnet mask */
      if (vgStrCmp(map_p->str, "IP") == 0)
      {
        vgPrintf (entity, (const Char*)"\"(0.0.0.0.0.0.0.0-%d.%d.%d.%d.%d.%d.%d.%d)\",",
          UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX,
          UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX);
      }
      /* For IPV4V6 we show the IPV6 address ranges */
      else if ((vgStrCmp(map_p->str, "IPV6") == 0) ||
               (vgStrCmp(map_p->str, "IPV4V6") == 0))
      {
        /* The range of TFT entry is different depending on the CGPIAF setting
                */
        if (getProfileValue (entity, PROF_CGPIAF + PROF_CGPIAF_IPV6_ADDR_FORMAT) == PROF_CGPIAF_ENABLE)
        {
          if (getProfileValue (entity, PROF_CGPIAF + PROF_CGPIAF_IPV6_SUBNET_NOTATION) == PROF_CGPIAF_ENABLE)
          {
            if (getProfileValue (entity, PROF_CGPIAF + PROF_CGPIAF_IPV6_LEADING_ZEROS) == PROF_CGPIAF_ENABLE)
            {
              if (getProfileValue (entity, PROF_CGPIAF + PROF_CGPIAF_IPV6_COMPRESS_ZEROS) == PROF_CGPIAF_ENABLE)
              {
                vgPrintf (entity, (const Char*)"\"(::/0-%X:%X:%X:%X:%X:%X:%X:%X/%d)\",",
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  MAX_IPV6_ADDR_LEN * BITS_PER_INT8);
              }
              else
              {
                vgPrintf (entity, (const Char*)"\"(0000:0000:0000:0000:0000:0000:0000:0000/0-%X:%X:%X:%X:%X:%X:%X:%X/%d)\",",
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  MAX_IPV6_ADDR_LEN * BITS_PER_INT8);
              }
            }
            else
            {
              if (getProfileValue (entity, PROF_CGPIAF + PROF_CGPIAF_IPV6_COMPRESS_ZEROS) == PROF_CGPIAF_ENABLE)
              {
                vgPrintf (entity, (const Char*)"\"(::/0-%X:%X:%X:%X:%X:%X:%X:%X/%d)\",",
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  MAX_IPV6_ADDR_LEN * BITS_PER_INT8);
              }
              else
              {
                vgPrintf (entity, (const Char*)"\"(0:0:0:0:0:0:0:0/0-%X:%X:%X:%X:%X:%X:%X:%X/%d)\",",
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  MAX_IPV6_ADDR_LEN * BITS_PER_INT8);
              }
            }
          }
          else
          {
            if (getProfileValue (entity, PROF_CGPIAF + PROF_CGPIAF_IPV6_LEADING_ZEROS) == PROF_CGPIAF_ENABLE)
            {
              if (getProfileValue (entity, PROF_CGPIAF + PROF_CGPIAF_IPV6_COMPRESS_ZEROS) == PROF_CGPIAF_ENABLE)
              {
                vgPrintf (entity, (const Char*)"\"(:: ::-%X:%X:%X:%X:%X:%X:%X:%X %X:%X:%X:%X:%X:%X:%X:%X)\",",
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX);
              }
              else
              {
                vgPrintf (entity, (const Char*)"\"(0000:0000:0000:0000:0000:0000:0000:0000 0000:0000:0000:0000:0000:0000:0000:0000-%X:%X:%X:%X:%X:%X:%X:%X %X:%X:%X:%X:%X:%X:%X:%X)\",",
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX);
              }
            }
            else
            {
              if (getProfileValue (entity, PROF_CGPIAF + PROF_CGPIAF_IPV6_COMPRESS_ZEROS) == PROF_CGPIAF_ENABLE)
              {
                vgPrintf (entity, (const Char*)"\"(:: ::-%X:%X:%X:%X:%X:%X:%X:%X %X:%X:%X:%X:%X:%X:%X:%X)\",",
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX);
              }
              else
              {
                vgPrintf (entity, (const Char*)"\"(0:0:0:0:0:0:0:0 0:0:0:0:0:0:0:0-%X:%X:%X:%X:%X:%X:%X:%X %X:%X:%X:%X:%X:%X:%X:%X)\",",
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                  USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX);
              }
            }
          }
        }
        else
        {
          /* Just use the dot separated format */

          vgPrintf (entity, (const Char*)"\"(0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0-%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d)\",",
            UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX,
            UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX,
            UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX,
            UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX,
            UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX,
            UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX,
            UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX,
            UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX);
        }
      }

      /* Protocol number / next header */
      vgPrintf (entity, (const Char*)"(%d-%d),", 0, MAX_PROTO_NUM);
      /* Destination port range */
      vgPrintf (entity, (const Char*)"\"(0.0-%d.%d)\",", USHRT_MAX, USHRT_MAX);
      /* Source port range */
      vgPrintf (entity, (const Char*)"\"(0.0-%d.%d)\",", USHRT_MAX, USHRT_MAX);
      /* Ipsec index */
      vgPrintf (entity, (const Char*)"(0-%lX),", ULONG_MAX);
      /* Type of service / traffic class */
      vgPrintf (entity, (const Char*)"\"(0.0-%d.%d)\",", UCHAR_MAX, UCHAR_MAX);

      /* Flow label */
      /* For IPV4V6 we show the flow label */
      if ((vgStrCmp(map_p->str, "IPV6") == 0)
          || (vgStrCmp(map_p->str, "IPV4V6") == 0))
      {
         vgPrintf (entity, (const Char*)"(0-%X)", MAX_FLOW_LABEL);
      }
      vgPrintf (entity, (const Char*)",(%d-%d)", PF_DIR_PRE_REL7, PF_DIR_BIDIRECTIONAL);
    }
    map_p++;
  }
  vgPutNewLine (entity);
}

/* We need functions which convert textual TFT values to
 * numeric for sending to ABPD.  In addition, for displaying the current values
 * we need to convert from numeric back to textual format.
 */

/*--------------------------------------------------------------------------
 *
 * Function:    vgConvertDotSeparatedTextualRemoteAddressSubnetMaskToNumeric
 *
 * Parameters:  input      textual remote address & subnet mask to convert
 *
 * Returns:     TRUE  if conversion successful
 *              FALSE otherwise
 *
 * Description:
 *
 * Converts remote (source) address and subnet mask from string
 * in the form of "a1.a2.a3.a4.m1.m2.m3.m4" for IPV4 and
 * "a1.a2.a3.a4.a5.a6.a7.a8.a9.a10.a11.a12.a13.a14.a15.a16.
 *  m1.m2.m3.m4.m5.m6.m7.m8.m9.m10.m11.m12.m13.m14.m15.m16" for IPV6
 *  where a,m are between 0 and 255 (27.007 10.1.3) to, for IPV4:
 *  An array of 8 octets where:
 *   octet 0 = a1
 *   octet 1 = a2
 *   octet 2 = a3
 *   octet 3 = a4
 *   octet 4 = m1
 *   octet 5 = m2
 *   octet 6 = m3
 *   octet 7 = m4
 *
 * For IPV6 - it is 32 octets:
 *   octet 0-15:  a1-a16
 *   octet 16-31: m1-m16
 *
 * The conversion is done using the same structure for source and destination,
 * so if you want to preserve the original, the "input" must be a copy.
 *
 * Supports IPV4, IPV6 and IPV4V6 addresses.  Basically - it converts what it
 * is given - if there are 32 values then it assumes IPV6, 8 values IPV4.
 *
 * Returns TRUE  if conversion successful
 *         FALSE otherwise
 *-------------------------------------------------------------------------*/
static Boolean vgConvertDotSeparatedTextualRemoteAddressSubnetMaskToNumeric (RemoteAddrSubnetMask *input)
{
  Int8 *start_p;
  Int8 *end_p;
  Int8 addrCounter;
  Int8 i;
  Int32 *tmpVal_p = PNULL;
  Boolean success = FALSE;

  KiAllocZeroMemory ((IPV6_REMOTE_ADDR_LEN * sizeof (Int32)), (void **)&tmpVal_p);

  start_p = input->val;
  end_p = input->val;

  /* Step through each value and convert in to the output numeric value.
   * Assume IPV6 address length - but we will drop out early if it is IPV4
   * anyway.
   */
  addrCounter = 0;

  while ((addrCounter < IPV6_REMOTE_ADDR_LEN) && (success == FALSE))
  {
    tmpVal_p[addrCounter] = strtoul ((const char *)start_p, (char **)&end_p, 10);

    if (tmpVal_p[addrCounter] > UCHAR_MAX)
    {
      break;
    }

    if (((addrCounter == (IPV4_REMOTE_ADDR_LEN - 1)) || (addrCounter == (IPV6_REMOTE_ADDR_LEN - 1))) &&
        (*end_p == '\0'))
    {
      success = TRUE;
    }
    else if (*end_p != '.')
    {
      break;
    }
    else
    {
      start_p = end_p + 1;
      addrCounter++;
    }
  }

  if (success)
  {
    memset (input->val, 0, MAX_REMOTE_ADDR_AND_SUBNETMASK);
    for (i = 0; i < (addrCounter + 1); i++)
    {
      input->val[i] = (Int8)tmpVal_p[i];
      input->length = (addrCounter + 1);
      /* Set the present field to be sure */
      input->present = TRUE;
    }
  }
  else
  {
    input->present = FALSE;
  }

  KiFreeMemory ((void **)&tmpVal_p);

  return (success);

}

/*--------------------------------------------------------------------------
 *
 * Function:    vgConvertColonSeparatedTextualRemoteAddressSubnetMaskToNumeric
 *
 * Parameters:  input      textual remote address & subnet mask to convert
 *
 * Returns:     TRUE  if conversion successful
 *              FALSE otherwise
 *
 * Description:
 *
 * Converts IPV6 remote (source) address and subnet mask from string
 * in the form of:
 * "ssss:tttt:uuuu:vvvv:wwww:xxxx:yyyy:zzzz aaaa:bbbb:cccc:dddd:eeee:ffff:gggg:hhhh"
 *
 * where sssss to zzzz and aaaa to hhhh are hexadecimal numbers,
 * to 32 octets:
 *
 *   ssss->zzzz: octet 0-15:  a1-a16 (remote (source) address)
 *   aaaa->hhhh: octet 16-31: m1-m16 (subnet mast)
 *
 * The format can compress groups of zeros and also miss out leading zeros.
 * In addition the subnet mask can be represented as a single number bit count
 * value using slash notation.
 *
 * This function deals with all these different combinations and does not need
 * to know what that the AT+CGPIAF setting is to do this.
 *
 * The conversion is done using the same structure for source and destination,
 * so if you want to preserve the original, the "input" must be a copy.
 *
 * Returns TRUE  if conversion successful
 *         FALSE otherwise
 *-------------------------------------------------------------------------*/
static Boolean vgConvertColonSeparatedTextualRemoteAddressSubnetMaskToNumeric (RemoteAddrSubnetMask *input)
{
  Boolean           success    = TRUE;
  Int8              i;
  Int8              subnetOffset = VG_MAX_UINT8;
  Boolean           subnetIsSlashFormat = FALSE;
  TextualPdnAddress *tempTextualPdnAddress_p = PNULL;
  PdnAddress        *tempPdnAddress_p = PNULL;
  Int8              *tempVal_p = PNULL;
  Int32             subnetMaskBits = 0;
  Char              *endPtr;
  Int8              subnetMaskBit;

  KiAllocZeroMemory (sizeof(TextualPdnAddress), (void **) &tempTextualPdnAddress_p);
  KiAllocZeroMemory (sizeof(PdnAddress), (void **) &tempPdnAddress_p);
  KiAllocZeroMemory (MAX_REMOTE_ADDR_AND_SUBNETMASK, (void **) &tempVal_p);

  tempTextualPdnAddress_p->addressPresent = TRUE;

  /* Find the separating space or "/" character between the remote (source)
   * address and the subnet mask
   */
  for (i = 0; i < input->length; i++)
  {
    /* we found a slash character */
    if (input->val[i] == FWD_SLASH_CHAR)
    {
      /* Did we already find one? */
      if (subnetIsSlashFormat)
      {
        /* Invalid format */
        success = FALSE;
      }
      else
      {
        subnetIsSlashFormat = TRUE;
        subnetOffset = i;
      }
    }
    /* we found a space character between the address and subnet mask */
    else if (input->val[i] == SPACE_CHAR)
    {
      /* Did we already find a slash? */
      if (subnetIsSlashFormat)
      {
        /* Invalid format */
        success = FALSE;
      }
      /* Check that the offset is not already assigned to something */
      else if (subnetOffset != VG_MAX_UINT8)
      {
        success = FALSE;
      }
      else
      {
        subnetOffset = i;
      }
    }
  }

  /* Check the validity of the separator between the address and subnet
   * mask
   */
  if ((subnetOffset == 0) || (subnetOffset >= MAX_TEXTUAL_PDN_ADDR) || (subnetOffset >= input->length - 1))
  {
    success = FALSE;
  }
  else
  {
    /* Separate out the remote (source) address first */
    tempTextualPdnAddress_p->length = subnetOffset;

    memset (tempTextualPdnAddress_p->address, 0, MAX_TEXTUAL_PDN_ADDR+1);

    memcpy (tempTextualPdnAddress_p->address, input->val, subnetOffset);

    if (vgColonHexStringToIpv6PDPAddr(tempTextualPdnAddress_p, tempPdnAddress_p, FALSE) != RESULT_CODE_OK)
    {
      /* The conversion failed for some reason */
      success = FALSE;
    }
    else
    {
      /* Copy the data in to the first 16 bytes of the tempVal field */
      if (tempPdnAddress_p->addressPresent)
      {
        memcpy (tempVal_p, tempPdnAddress_p->ipv6Address, MAX_IPV6_ADDR_LEN);
      }
      else
      {
        /* Just set the address to all zeros */
        memset (tempVal_p, 0, MAX_IPV6_ADDR_LEN);
      }
    }
  }

  if (success)
  {
    /* We already got the remote (source) address, now get the subnet mask */
    if (subnetIsSlashFormat)
    {
      /* We have a slash format subnet mask so convert the number we got */
      subnetMaskBits = strtoul ((const char *)&(input->val[subnetOffset+1]), (char **)&endPtr, 10);

      if (subnetMaskBits > MAX_IPV6_SUBNET_MASK_BITS)
      {
        /* Converted value out of range */
        success = FALSE;
      }
      else
      {
        subnetMaskBit = 0x80;

        /* Convert the subnet mask bits to actual bits in the subnet mask */
        for (i=0; i< subnetMaskBits; i++)
        {
          tempVal_p[MAX_IPV6_ADDR_LEN + (i/BITS_PER_INT8)] |= subnetMaskBit;
          if (subnetMaskBit != 0x01)
          {
            /* Shift down 1 bit */
            subnetMaskBit >>= 1;
          }
          else
          {
            /* Start at the top bit again */
            subnetMaskBit = 0x80;
          }
        }
      }
    }
    else
    {
      /* Convert the subnet mask as a normal IPV6 address */
      tempTextualPdnAddress_p->length = input->length - (subnetOffset + 1);

      /* Make sure we aren't going to overrun the array */
      if (tempTextualPdnAddress_p->length >= MAX_TEXTUAL_PDN_ADDR)
      {
        success = FALSE;
      }
      else
      {
        memset (tempTextualPdnAddress_p->address, 0, MAX_TEXTUAL_PDN_ADDR+1);

        /* Copy the value after the remote (source) address */
        memcpy (tempTextualPdnAddress_p->address, &input->val[subnetOffset + 1], tempTextualPdnAddress_p->length);

        if (vgColonHexStringToIpv6PDPAddr(tempTextualPdnAddress_p, tempPdnAddress_p, TRUE) != RESULT_CODE_OK)
        {
          /* The conversion failed for some reason */
          success = FALSE;
        }
        else
        {
          /* Copy the data in to the first 16 bytes of the tempVal field */
          if (tempPdnAddress_p->addressPresent)
          {
            memcpy (&tempVal_p[MAX_IPV6_ADDR_LEN], tempPdnAddress_p->ipv6Address, MAX_IPV6_ADDR_LEN);
          }
          else
          {
            /* Just set the address to all zeros */
            memset (&tempVal_p[MAX_IPV6_ADDR_LEN], 0, MAX_IPV6_ADDR_LEN);
          }
        }
      }
    }
  }

  if (success)
  {
    /* Convertion of all values OK - so now copy the data back in to the
     * input
     */
    input->present = TRUE;
    input->length = IPV6_REMOTE_ADDR_LEN;
    memcpy (input->val, tempVal_p, IPV6_REMOTE_ADDR_LEN);
  }
  else
  {
    input->present = FALSE;
  }

  KiFreeMemory ((void **) &tempTextualPdnAddress_p);
  KiFreeMemory ((void **) &tempPdnAddress_p);
  KiFreeMemory ((void **) &tempVal_p);

  return (success);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgConvertTextualRemoteAddressSubnetMaskToNumeric
 *
 * Parameters:  input      textual remote address & subnet mask to convert
 *              entity     Entity which is requesting to convert the entered
 *                         address and subnet mask in to numeric
 *
 * Returns:     TRUE  if conversion successful
 *              FALSE otherwise
 *
 * Description:
 *
 * Converts remote (source) address and subnet mask from string format in to
 * numeric.  The way the number is converted depends on address type and format:
 *
 * If the format has ":" chars in it then it must be a colon separated IPV6
 * address.  If it has "." chars in it then it can be a dot separated IPV4 or
 * IPV6 address.  So this function does not need to know the AT+CGPIAF setting
 * in order to work out what format the user has set.
 *
 * The conversion is done using the same structure for source and destination,
 * so if you want to preserve the original, the "input" must be a copy.
 *
 * Returns TRUE  if conversion successful
 *         FALSE otherwise
 *-------------------------------------------------------------------------*/
static Boolean vgConvertTextualRemoteAddressSubnetMaskToNumeric (RemoteAddrSubnetMask *input,
                                                                 const VgmuxChannelNumber entity)
{
  Boolean success    = FALSE;

  Int8    index;
  Boolean colonFound = FALSE;
  Boolean dotFound   = FALSE;

  /* Scan the input for ":"s and "."s */

  for (index = 0; (index < MAX_REMOTE_ADDR_AND_SUBNETMASK) && (input->val[index] != '\0'); index++)
  {
    if (input->val[index] == DOT_CHAR)
    {
      dotFound = TRUE;
    }

    if (input->val[index] == COLON_CHAR)
    {
      colonFound = TRUE;
    }
  }

  if (dotFound && !colonFound)
  {
    /* Must be IPV4 or IPV6 dot separated address */
    success = vgConvertDotSeparatedTextualRemoteAddressSubnetMaskToNumeric (input);
  }
  else if (colonFound && !dotFound)
  {
    /* Must be colon separated IPV6 address */
    success = vgConvertColonSeparatedTextualRemoteAddressSubnetMaskToNumeric (input);
  }

  /* Anything else is an error */
  return (success);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgConvertTextualPortRangeToNumeric
 *
 * Parameters:  input      textual port range to convert
 *
 * Returns:     TRUE  if conversion successful
 *              FALSE otherwise
 *
 * Description:
 *
 * Converts port range from string in the form of
 * "f.t" where f,t are between 0 and 65535 (27.007 10.1.3) into
 * an array of 4 octets where:
 * octet 0 = most significant octet of f
 * octet 1 = least significant octet of f
 * octet 2 = most significant octet of t
 * octet 3 = least significant octet of t
 *
 * The conversion is done using the same structure for source and destination,
 * so if you want to preserve the original, the "input" must be a copy.
 *
 *-------------------------------------------------------------------------*/
static Boolean vgConvertTextualPortRangeToNumeric (PortRange *input)
{
  Int8 *start_p, *end_p;
  Int32 tmpVal1, tmpVal2;
  Boolean success = FALSE;

  start_p = input->val;
  end_p = input->val;

  /* Convert the f octet first */
  tmpVal1 = strtoul ((const char *)start_p, (char **)&end_p, 10);
  if (*end_p == '.')
  {
    start_p = end_p + 1;
    /* Convert the t octet */
    tmpVal2 = strtoul ((const char *)start_p, (char **)&end_p, 10);
    if ((*end_p == '\0') &&
        (tmpVal1 <= USHRT_MAX) &&
        (tmpVal2 <= USHRT_MAX))
    {
      /* only overwrite input if both values are present and within range */
      memset (input->val, 0, MAX_PORT_RANGE);
      input->val[0] = (Int8) (tmpVal1 >> BITS_PER_INT8);
      input->val[1] = (Int8) (tmpVal1);
      input->val[2] = (Int8) (tmpVal2 >> BITS_PER_INT8);
      input->val[3] = (Int8) (tmpVal2);
      input->length = PORT_RANGE_LEN;

      /* Set present field to be sure */
      input->present = TRUE;

      success = TRUE;
    }
  }
  return (success);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgConvertTextualTosTrfcClassToNumeric
 *
 * Parameters:  input      textual type of service / traffic class to convert
 *
 * Returns:     TRUE  if conversion successful
 *              FALSE otherwise
 *
 * Description:
 *
 * Converts type of service /traffic class from string
 * in the form of "t.m" where t,m are between 0 and 255 (27.007 10.1.3) into
 * an array of 2 octets where:
 * octet 0 = t
 * octet 1 = m
 *
 * The conversion is done using the same structure for source and destination,
 * so if you want to preserve the original, the "input" must be a copy.
 *
 *-------------------------------------------------------------------------*/
static Boolean vgConvertTextualTosTrfcClassToNumeric (TosTrfcClass *input)
{
  Int8 *start_p, *end_p;
  Int32 tmpVal1, tmpVal2;
  Boolean success = FALSE;

  start_p = input->val;
  end_p = input->val;

  /* Convert t value first */
  tmpVal1 = strtoul ((const char *)start_p, (char **)&end_p, 10);
  if (*end_p == '.')
  {
    start_p = end_p + 1;

    /* Convert m value */
    tmpVal2 = strtoul ((const char *)start_p, (char **)&end_p, 10);
    if ((*end_p == '\0') &&
        (tmpVal1 <= UCHAR_MAX) &&
        (tmpVal2 <= UCHAR_MAX))
    {
      /* only overwrite input if both values are present and within range
       */
      memset (input->val, 0, MAX_TOS_TRFCCLSS);
      input->val[0] = (Int8) (tmpVal1);
      input->val[1] = (Int8) (tmpVal2);
      input->length = TOS_TRFCCLSS_LEN;

      /* Set present field to be sure */
      input->present = TRUE;

      success = TRUE;
    }
  }
  return (success);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgConvertNumericRemoteAddressSubnetMaskToTextual
 *
 * Parameters:  input      numeric remote address & subnet mask to convert
 *              entity     Channel on which the data is to be displayed
 *
 * Returns:     TRUE  if conversion successful
 *              FALSE otherwise
 *
 * Description:
 *
 * Converts remote address and subnet mask from numeric (8 octets for IPV4,
 * 32 octets for IPV6) to string type
 * in the form of "a1.a2.a3.a4.m1.m2.m3.m4" for IPV4 and
 * "a1.a2.a3.a4.a5.a6.a7.a8.a9.a10.a11.a12.a13.a14.a15.a16.
 *  m1.m2.m3.m4.m5.m6.m7.m8.m9.m10.m11.m12.m13.m14.m15.m16" for IPV6
 *  or the specific format as specified by the AT+CGPIAF command.
 *
 *  where a,m are between 0 and 255 (27.007 10.1.3) for IPV4:
 *   octet 0 = a1
 *   octet 1 = a2
 *   octet 2 = a3
 *   octet 3 = a4
 *   octet 4 = m1
 *   octet 5 = m2
 *   octet 6 = m3
 *   octet 7 = m4
 *
 * For IPV6:
 *   octet 0-15:  a1-a16
 *   octet 16-31: m1-m16
 *
 * The conversion is done using the same structure for source and destination,
 * so if you want to preserve the original, the "input" must be a copy.
 *
 * Note that in order to support IPV4V6 this function works out from the
 * length of the numeric data if it is an IPV4 or IPV6 it does not use the setting
 * for the PDP context (CGDCONT) which may not have been defined yet anyway.
 *
 * Returns TRUE  if conversion successful
 *         FALSE otherwise
 *-------------------------------------------------------------------------*/
static Boolean vgConvertNumericRemoteAddressSubnetMaskToTextual (RemoteAddrSubnetMask *input,
                                                                 const VgmuxChannelNumber entity)
{

  Int8              *tempRemoteAddrSubnetMaskStr_p = PNULL;
  PdnAddress        *tempPdnAddress_p = PNULL;
  TextualPdnAddress *tempTextualPdnAddress_p = PNULL;
  Int8              textualSourceAddrLen;

  Boolean success = FALSE;

  if ((input->present) &&
      ((input->length == IPV4_REMOTE_ADDR_LEN) || (input->length == IPV6_REMOTE_ADDR_LEN)))
  {

    KiAllocMemory (MAX_REMOTE_ADDR_AND_SUBNETMASK, (void **)&tempRemoteAddrSubnetMaskStr_p);
    KiAllocMemory (sizeof (PdnAddress), (void **)&tempPdnAddress_p);
    KiAllocMemory (sizeof (TextualPdnAddress), (void **)&tempTextualPdnAddress_p);

    if (input->length == IPV4_REMOTE_ADDR_LEN)
    {
      /* Must be IPV4 address and subnet mask */
      snprintf((char *)tempRemoteAddrSubnetMaskStr_p, MAX_REMOTE_ADDR_AND_SUBNETMASK-1,
                 "%d.%d.%d.%d.%d.%d.%d.%d",
                input->val[0],
                input->val[1],
                input->val[2],
                input->val[3],
                input->val[4],
                input->val[5],
                input->val[6],
                input->val[7]);
    }
    else
    {
      /* Must be IPV6 address and subnet mask */
      /* How the address is displayed depends on the AT+CGPIAF setting */
      tempPdnAddress_p->pdnType = PDN_TYPE_IPV6;
      tempPdnAddress_p->addressPresent = TRUE;
      tempPdnAddress_p->ipv6AddressType = IPV6_ADDRESS_FULL;

      /* Copy the source address part in to the tempPdnAddress structure */
      memcpy (tempPdnAddress_p->ipv6Address, input->val, MAX_IPV6_ADDR_LEN);

      /* Now convert the address */
      vgPDNAddrIntToDisplayStr (PDN_TYPE_IPV6, *tempPdnAddress_p, tempTextualPdnAddress_p, entity);

      textualSourceAddrLen = tempTextualPdnAddress_p->length;

      /* Copy the string we get back in to the tempRemoteAddrSubnetMaskStr_p */
      memcpy (tempRemoteAddrSubnetMaskStr_p, tempTextualPdnAddress_p->address, textualSourceAddrLen);

      /* Now copy the subnet mask part in to the tempPdnAddress structure */
      memcpy (tempPdnAddress_p->ipv6Address, &(input->val[MAX_IPV6_ADDR_LEN]), MAX_IPV6_ADDR_LEN);

      /* Now convert the address */
      vgIPV6SubnetMaskIntToDisplayStr (*tempPdnAddress_p, tempTextualPdnAddress_p, entity);

      /* Make sure we don't overshoot the length of the display array */
      FatalCheck (textualSourceAddrLen + tempTextualPdnAddress_p->length <= (MAX_REMOTE_ADDR_AND_SUBNETMASK-1),
                  textualSourceAddrLen,
                  tempTextualPdnAddress_p->length,
                  entity);

      /* Copy the string we get back in to the tempRemoteAddrSubnetMaskStr_p */
      memcpy (tempRemoteAddrSubnetMaskStr_p + textualSourceAddrLen, tempTextualPdnAddress_p->address, tempTextualPdnAddress_p->length);

      /* stick a trailing 0 at the end of the string */
      tempRemoteAddrSubnetMaskStr_p [textualSourceAddrLen + tempTextualPdnAddress_p->length] = 0;
    }

    input->length = strlen((char *)tempRemoteAddrSubnetMaskStr_p);
    /* also copy the trailing 0 */
    memcpy (input->val, tempRemoteAddrSubnetMaskStr_p, input->length + 1);

    KiFreeMemory ((void **)&tempRemoteAddrSubnetMaskStr_p);
    KiFreeMemory ((void **)&tempPdnAddress_p);
    KiFreeMemory ((void **)&tempTextualPdnAddress_p);

    success = TRUE;
  }
  return (success);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgConvertNumericPortRangeToTextual
 *
 * Parameters:  input      numeric port range to convert
 *
 * Returns:     TRUE  if conversion successful
 *              FALSE otherwise
 *
 * Description:
 *
 * Converts port range from numeric (4 octets) to string type in the form of
 * "f.t" where f,t are between 0 and 65535 (27.007 10.1.3)
 *
 * octet 0 = most significant octet of f
 * octet 1 = least significant octet of f
 * octet 2 = most significant octet of t
 * octet 3 = least significant octet of t
 *
 * The conversion is done using the same structure for source and destination,
 * so if you want to preserve the original, the "input" must be a copy.
 *
 *-------------------------------------------------------------------------*/
static Boolean vgConvertNumericPortRangeToTextual (PortRange *input)
{
  Int8    tempPortRangeStr[MAX_PORT_RANGE] = {0};
  Boolean success = FALSE;

  if ((input->present) && (input->length == PORT_RANGE_LEN))
  {
    snprintf( (char *)tempPortRangeStr, MAX_PORT_RANGE-1, "%d.%d",
              (((Int16)input->val[0]) << BITS_PER_INT8) + (Int16)input->val[1],
              (((Int16)input->val[2]) << BITS_PER_INT8) + (Int16)input->val[3]);
    input->length = strlen((char *)tempPortRangeStr);
    /* also copy the trailing 0 */
    memcpy (input->val, (Char *)tempPortRangeStr, input->length + 1);
    success = TRUE;
  }

  return (success);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgConvertNumericTosTrfcClassToTextual
 *
 * Parameters:  input      numeric type of service / traffic class to convert
 *
 * Returns:     TRUE  if conversion successful
 *              FALSE otherwise
 *
 * Description:
 *
 * Converts type of service /traffic class from numeric (2 octets) to string type
 * in the form of "t.m" where t,m are between 0 and 255 (27.007 10.1.3)
 *
 * octet 0 = t
 * octet 1 = m
 *
 * The conversion is done using the same structure for source and destination,
 * so if you want to preserve the original, the "input" must be a copy.
 *
 *-------------------------------------------------------------------------*/
static Boolean vgConvertNumericTosTrfcClassToTextual (TosTrfcClass *input)
{
  Int8    tempTosTrfcClass[MAX_TOS_TRFCCLSS] = {0};
  Boolean success = FALSE;

  if ((input->present) && (input->length == TOS_TRFCCLSS_LEN))
  {
    snprintf( (char *)tempTosTrfcClass, MAX_TOS_TRFCCLSS-1, "%d.%d",
              input->val[0],
              input->val[1]);
    input->length = strlen((char *)tempTosTrfcClass);
    /* also copy the trailing 0 */
    memcpy (input->val, (Char *)tempTosTrfcClass, input->length + 1);
    success = TRUE;
  }

  return (success);
}
#endif /* FEA_QOS_TFT */

/***************************************************************************
 *
 * Function:    vgProcMgSinkTxOnePduOnNsapi
 *
 * Parameters:
 *
 * Returns:
 *
 * Description:
 *
 ***************************************************************************/

static void vgProcMgSinkTxOnePduOnNsapi (VgMGSINKData *vgMGSINKData_p)
{
    Int8                  psdBearerIdEntry   = 0;

    SignalBuffer          sigBuf        = kiNullBuffer;
    VgCGEREPPdpContext    *pdpContext   = PNULL;
    GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
    Boolean               success       = TRUE;
    Boolean               seriousFailure = FALSE;
    /* Store some parameters locally and update at exit only if successful */
    Int16                 packetCount   = vgMGSINKData_p->packetCount;
    Int16                 packetId      = vgMGSINKData_p->packetId;
    Int16                 packet        = vgMGSINKData_p->packet;
    Int16                 segment       = vgMGSINKData_p->segment;

    /* Find out if there is context for this NSAPI */
    while ( ( psdBearerIdEntry < MAX_NUM_PSD_BEARERS ) &&
            !( ( gprsGenericContext_p->vgPdpContext[psdBearerIdEntry].active == TRUE ) &&
               ( gprsGenericContext_p->vgPdpContext[psdBearerIdEntry].psdBearerId == vgMGSINKData_p->psdBearerId ) ) )
    {
        psdBearerIdEntry++;
    }

    if (psdBearerIdEntry == MAX_NUM_PSD_BEARERS)
    {
        success         = FALSE;
        seriousFailure  = TRUE;
    }
    else
    {
        pdpContext = &(gprsGenericContext_p->vgPdpContext[psdBearerIdEntry]);
    }

    if ((pdpContext != PNULL) &&
        (success == TRUE))
    {
        if (pdpContext->reliabilityClass <= GPRS_RELIAB_CLASS_2)
        {   /* Acknowledged LLC mode */
            KiCreateSignal (SIG_SN_DATA_REQ, sizeof (SnDataReq), &sigBuf);
        }
        else
        {   /* Unacknowledged LLC mode */
            KiCreateSignal (SIG_SN_UNIT_DATA_REQ, sizeof (SnUnitDataReq), &sigBuf);
        }

        /* IP Segmentation */
        if (packet == 0)
        {   /* The first segment */
            segment = min (IP_MTU-IP_LEN-UDP_LEN, vgMGSINKData_p->packetSize);
            packet  = vgMGSINKData_p->packetSize - segment;
            segment += IP_LEN+UDP_LEN;
            packetId++;
            /* Decrement packet count */
            packetCount--;
        }
        else
        {
            segment = min (IP_MTU-IP_LEN, packet);
            packet  -= segment;
            segment += IP_LEN;
        }

        sigBuf.sig->snDataReq.npdu.totalLength  = segment;
        sigBuf.sig->snDataReq.npdu.npduData     = PNULL;

#if defined (USE_L4MM_ALLOC_MEMORY)
        /* Allocate TMM memory for the data to send */
        L4mmAlloc (L4MM_UL_POOL,
                        segment,
                        &sigBuf.sig->snDataReq.npdu.npduData);
#else
        /* Allocate TMM memory for the data to send */
        TmmAllocMemory (TMM_DEFAULT_POOL_UL,
                        segment,
                        &sigBuf.sig->snDataReq.npdu.npduData);
#endif

        /* Need to check whether we managed to get memory from TMM */
        if (sigBuf.sig->snDataReq.npdu.npduData == PNULL)
        {
            KiDestroySignal (&sigBuf);
            success = FALSE;
        }
        else
        {
            Int16 i;
            Int32 cs16 = 0;
            Int8 *data = sigBuf.sig->snDataReq.npdu.npduData;

            PdnAddress   tmpPdnAddress;

            memset (data, 0x00, segment);

            if ((pdpContext != PNULL) &&
                (pdpContext->pdnAddress.pdnType == PDN_TYPE_IPV4))
            {    /* IP +  Version=4     |     IHL=5      +  45
                       |  Routine   |D=0|T=0|R=0| 0 | 0  |  00
                       |  Total Length=XXXX              |  XX XX
                       |  Identification=XXXX            |  XX XX
                       |  DF=0 MF=X Fragment Offset=XX   |  00 XX
                       |  Time to Live=15                |  0F
                       |  Protocol=UDP                   |  11
                       |  Header Checksum      OK        |  77 EF
                       |  Source        018.052.086.120  |  12 34 56 78
                       |  Destination   192.168.010.010  |  C0 A8 0A 0A */
                data[0] = 0x45; data[1] = 0x00;
                data[2] = (Int8)(segment  >> 8) ; data[3] = (Int8)(segment  & 0x00ff);
                data[4] = (Int8)(packetId >> 8) ; data[5] = (Int8)(packetId & 0x00ff);
                /* Fragment Offset (13 bits, units of 8 octets) */
                data[6] = (Int8)(((UDP_LEN + vgMGSINKData_p->packetSize - packet + IP_LEN - segment)/8) >> 8) ;
                data[7] = (Int8)(((UDP_LEN + vgMGSINKData_p->packetSize - packet + IP_LEN - segment)/8) & 0x00ff);
                if (packet) /* MF=1 */
                    data[6] |= 0x20;
                data[8]  = 0x0F; data[9]  = 0x11; /* TtL=15, UDP */
                data[10] = 0x00; data[11] = 0x00;

                /* Copy Source address */
                tmpPdnAddress = pdpContext->pdnAddress;

                memcpy (&data[12], tmpPdnAddress.ipv4Address, 4);
                /* Copy Destination address */
                if (vgMGSINKData_p->pdnAddress.addressPresent == FALSE)
                {    /* Case "" - Destination = Source */
                    memcpy (&data[16], &data[12], 4);
                }
                else
                {
                    if (vgStringToPDNAddr (PDN_TYPE_IPV4, &vgMGSINKData_p->pdnAddress, &tmpPdnAddress) == RESULT_CODE_OK)
                    {
                        if (tmpPdnAddress.addressPresent == 0)
                        {    /* Case "0.0.0.0" - Destination = Source */
                            memcpy (&data[16], &data[12], 4);
                        }
                        else
                        {
                            memcpy (&data[16], tmpPdnAddress.ipv4Address, 4);
                        }
                    }
                    else
                    {
                        success         = FALSE;
                        seriousFailure  = TRUE;
                    }
                }

                if (success == TRUE)
                {
                    /* IP Header Checksum */
                    for (i = 0; i < 20; i += 2)
                    {
                        cs16 += ((data[i]<<8) | data[i+1]);
                    }
                    cs16 = ( cs16  & 0xFFFF) + (cs16 >> 16);
                    cs16 = ( cs16  & 0xFFFF) + (cs16 >> 16);
                    cs16 = (~cs16) & 0xFFFF;
                    data[10] = (Int8)(cs16 >> 8);
                    data[11] = (Int8)(cs16  & 0x00ff);
                    if (packet + segment >= vgMGSINKData_p->packetSize) /* The first segment */
                    {
                    /* UDP +  Source=0      | Dest=9         +  00 00 00 09
                           |  Length=828    | Checksum  none |  03 3C 00 00 */
                        data[20] = 0x00; data[21] = 0x00;   /* Source (optional) */
                        data[22] = (Int8)(vgMGSINKData_p->port >> 8); data[23] = (Int8)(vgMGSINKData_p->port & 0x00ff);  /* Dest */
                        data[24] = (Int8)((UDP_LEN + vgMGSINKData_p->packetSize) >> 8) ;       /* Length */
                        data[25] = (Int8)((UDP_LEN + vgMGSINKData_p->packetSize) & 0x00ff);
                        data[26] = 0x00; data[27] = 0x00; /* Checksum (optional) */
                    }

                    sigBuf.sig->snDataReq.nsapi             = (Nsapi)(vgMGSINKData_p->psdBearerId);
                    sigBuf.sig->snDataReq.npdu.dataOffset   = 0;
                    sigBuf.sig->snDataReq.npdu.length       = segment;
                    sigBuf.sig->snDataReq.pdpPacketTypeHdr  = GPRS_PCOMP_UNCOMPRESSED;
                    sigBuf.sig->snDataReq.pdpPacketTypeData = GPRS_DCOMP_UNCOMPRESSED;

                    /* Send the signal to SNDCP to transmit */
                    KiSendSignal (GP_SNDCP_TASK_ID, &sigBuf);
                }
            }
        }
    }

    if (success == TRUE)
    {
        /* Store updated paramaters in MGSINK data */
        vgMGSINKData_p->packetCount = packetCount;
        vgMGSINKData_p->packetId    = packetId;
        vgMGSINKData_p->packet      = packet;
        vgMGSINKData_p->segment     = segment;
    }

    if ((seriousFailure == TRUE) ||                       /* Give up */
        ((vgMGSINKData_p->packet == 0) &&
         (vgMGSINKData_p->packetCount == 0)))
    {
        /* Finished */
        /* Check if there is a pending request */
        if (vgMGSINKData_p->next != PNULL)
        {
            /* There is a pending request. Copy it into current one. */
            VgMGSINKData *newVgMGSINKData_p = vgMGSINKData_p->next;
            memcpy (vgMGSINKData_p, newVgMGSINKData_p, sizeof(VgMGSINKData));
            KiFreeMemory ((void **)&newVgMGSINKData_p);
        }
        else
        {
            vgMGSINKData_p->psdBearerId  = PSD_BEARER_ID_UNASSIGNED;
            vgMGSINKData_p->mgsinkActive = FALSE;

#if defined (USE_L4MM_ALLOC_MEMORY)
            /* De-register from L4MM UL flow control indications.
            */
            /* TODO:TmmRemoveFlowControlTask (TMM_DEFAULT_POOL_UL, VG_CI_TASK_ID); */

#else /* USE_L4MM_ALLOC_MEMORY */
            /* Remove ATCI as flow controller for TMM.
             * Note that this may be just decrementing instances of ATCI task
             * in flow control data in TMM.
             */
#if !defined (USE_BMM_ALLOC_MEMORY)
            TmmRemoveFlowControlTask (TMM_DEFAULT_POOL_UL, VG_CI_TASK_ID);
#endif
#endif /* USE_L4MM_ALLOC_MEMORY */
        }
    }

    return;
}

/***************************************************************************
 *
 * Function:    vgProcMgSinkTxPdu
 *
 * Parameters:
 *
 * Returns:
 *
 * Description:
 *
 ***************************************************************************/

void vgProcMgSinkTxPdu (void)
{
    Int8                  psdBearerIdEntry   = 0;
    GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
    Boolean               sendMore = TRUE;

    /* Send UL NPDUs alternately on NSAPIs running MGSINK */
    while (sendMore == TRUE)
    {
        /* Find out if there is a PSD Bearer running MGSINK */
        sendMore = FALSE;
        for (psdBearerIdEntry = 0; psdBearerIdEntry < MAX_NUM_PSD_BEARERS; psdBearerIdEntry++)
        {
            if (gprsGenericContext_p->vgMGSINKData[psdBearerIdEntry].mgsinkActive == TRUE)
            {
                /* Found an NSAPI that is running MGSINK */
                sendMore = TRUE;
                break;
            }
        }

        /* There is at least one PSD Bearer still expecting to send UL data.
         * Loop through all PSD Bearers and send 1 UL packet on all waiting if possible. */
        if (sendMore == TRUE)
        {
            for (psdBearerIdEntry = 0; psdBearerIdEntry < MAX_NUM_PSD_BEARERS; psdBearerIdEntry++)
            {
                if (gprsGenericContext_p->vgMGSINKData[psdBearerIdEntry].mgsinkActive == TRUE)
                {
                    /* Found a PSD Beaerer ID that is running MGSINK.
                     * Try to send one UL packet on it.
                     */
                    vgProcMgSinkTxOnePduOnNsapi (&gprsGenericContext_p->vgMGSINKData[psdBearerIdEntry]);
                }
            }

#if defined (USE_L4MM_ALLOC_MEMORY)
            if (L4mmIsFlowControlLevelReached(L4MM_UL_POOL) == TRUE)
#else
            if (TmmCanAllocFromPool(TMM_DEFAULT_POOL_UL) == FALSE)
#endif
            {
                /* TMM HWM reached, stop here and wait for LWM to continue */
                sendMore = FALSE;
            }
        }
    }

    return;
}

/***************************************************************************
 *
 * Function:    vgProcMgtcSinkTxOnePduOnNsapi
 *
 * Parameters:
 *
 * Returns:
 *
 * Description:
 *
 ***************************************************************************/
static void vgProcMgtcSinkTxOnePduOnNsapi (VgMGSINKData *vgMGSINKData_p)
{
    Int8                  psdBearerIdEntry     = 0;
    SignalBuffer          sigBuf         = kiNullBuffer;
    VgCGEREPPdpContext    *pdpContext    = PNULL;
    GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
    Boolean               success        = TRUE;
    Boolean               seriousFailure = FALSE;
    /* Store some parameters locally and update at exit only if successful */
    Int16                 packetCount    = vgMGSINKData_p->packetCount;
    Int16                 packetId       = vgMGSINKData_p->packetId;
    Int32                 sequenceNumber = vgMGSINKData_p->sequenceNumber;
    Int16                 packet         = vgMGSINKData_p->packet;
    Int16                 segment        = vgMGSINKData_p->segment;

    /* Find out if there is context for this PSD Bearer ID */
    while ( ( psdBearerIdEntry < MAX_NUM_PSD_BEARERS ) &&
            !( ( gprsGenericContext_p->vgPdpContext[psdBearerIdEntry].active == TRUE ) &&
               ( gprsGenericContext_p->vgPdpContext[psdBearerIdEntry].psdBearerId == vgMGSINKData_p->psdBearerId ) ) )
    {
        psdBearerIdEntry++;
    }

    if (psdBearerIdEntry == MAX_NUM_PSD_BEARERS)
    {
        success         = FALSE;
        seriousFailure  = TRUE;
    }
    else
    {
        pdpContext = &(gprsGenericContext_p->vgPdpContext[psdBearerIdEntry]);
    }

    /* if packetSize is 0, just reset the packetId and sequence number fields */
    if (vgMGSINKData_p->packetSize == 0)
    {
        vgMGSINKData_p->packetId        = 0;
        vgMGSINKData_p->sequenceNumber  = 0;
        vgMGSINKData_p->packetCount     = 0;
        success = FALSE; /* To return */
    }

    if ((pdpContext != PNULL) &&
        (success == TRUE))
    {
        if (pdpContext->reliabilityClass <= GPRS_RELIAB_CLASS_2)
        {   /* Acknowledged LLC mode */
            KiCreateSignal (SIG_SN_DATA_REQ, sizeof (SnDataReq), &sigBuf);
        }
        else
        {   /* Unacknowledged LLC mode */
            KiCreateSignal (SIG_SN_UNIT_DATA_REQ, sizeof (SnUnitDataReq), &sigBuf);
        }
        /* IP Segmentation */
        if (packet == 0)
        {   /* The first segment */
            segment = min (IP_MTU-IP_LEN-TCP_LEN, vgMGSINKData_p->packetSize);
            packet  = vgMGSINKData_p->packetSize - segment;
            segment += IP_LEN+TCP_LEN;
            packetId++;
            /* Decrement packet count */
            packetCount--;
        }
        else
        {
            segment = min (IP_MTU-IP_LEN, packet);
            packet -= segment;
            segment += IP_LEN;
        }

        sigBuf.sig->snDataReq.npdu.totalLength  = segment;
        sigBuf.sig->snDataReq.npdu.npduData     = PNULL;

#if defined (USE_L4MM_ALLOC_MEMORY)
        L4mmAlloc (L4MM_UL_POOL,
                        segment,
                        &sigBuf.sig->snDataReq.npdu.npduData);
#else
        /* Allocate TMM memory for the data to send */
        TmmAllocMemory (TMM_DEFAULT_POOL_UL,
                        segment,
                        &sigBuf.sig->snDataReq.npdu.npduData);
#endif

        /* Need to check whether we managed to get memory from TMM */
        if (sigBuf.sig->snDataReq.npdu.npduData == PNULL)
        {
            KiDestroySignal (&sigBuf);
            success = FALSE;
        }
        else
        {
            Int16 i;
            Int32 cs16 = 0;
            Int8 *data = sigBuf.sig->snDataReq.npdu.npduData;

            PdnAddress   tmpPdnAddress;

            memset (data, 0x00, segment);

            if ( (pdpContext != PNULL) &&
                (pdpContext->pdnAddress.pdnType == PDN_TYPE_IPV4))

            {    /* IP +  Version=4     |     IHL=5      +  45
                       |  Routine   |D=0|T=0|R=0| 0 | 0  |  00
                       |  Total Length=XXXX              |  XX XX
                       |  Identification=XXXX            |  XX XX
                       |  DF=0 MF=X Fragment Offset=XX   |  00 XX
                       |  Time to Live=1                 |  01
                       |  Protocol=TCP                   |  06
                       |  Header Checksum      OK        |  77 EF
                       |  Source        018.052.086.120  |  12 34 56 78
                       |  Destination   192.168.010.010  |  C0 A8 0A 0A */
                data[0] = 0x45; data[1] = 0x00;
                data[2] = (Int8)(segment  >> 8) ; data[3] = (Int8)(segment  & 0x00ff);
                data[4] = (Int8)(packetId >> 8) ; data[5] = (Int8)(packetId & 0x00ff);
                /* Fragment Offset (13 bits, units of 8 octets) */
                data[6] = (Int8)(((TCP_LEN + vgMGSINKData_p->packetSize - packet + IP_LEN - segment)/8) >> 8) ;
                data[7] = (Int8)(((TCP_LEN + vgMGSINKData_p->packetSize - packet + IP_LEN - segment)/8) & 0x00ff);
                if (packet) /* MF=1 */
                    data[6] |= 0x20;
                data[8]  = 0x01; data[9]  = 0x06; /* TtL=1, TCP */
                data[10] = 0x00; data[11] = 0x00;
                /* Copy Source address */

                tmpPdnAddress = pdpContext->pdnAddress;
                memcpy (&data[12], tmpPdnAddress.ipv4Address, 4);
                /* Copy Destination address */
                if (vgMGSINKData_p->pdnAddress.addressPresent == FALSE)
                {    /* Case "" - Destination = Source */
                    memcpy (&data[16], &data[12], 4);
                }
                else
                {
                    if (vgStringToPDNAddr (PDN_TYPE_IPV4, &vgMGSINKData_p->pdnAddress, &tmpPdnAddress) == RESULT_CODE_OK)
                    {
                        if (tmpPdnAddress.addressPresent == FALSE)
                        {    /* Case "0.0.0.0" - Destination = Source */
                            memcpy (&data[16], &data[12], 4);
                        }
                        else
                        {
                            memcpy (&data[16], tmpPdnAddress.ipv4Address, 4);
                        }
                    }
                    else
                    {
                        success         = FALSE;
                        seriousFailure  = TRUE;
                    }
                }

                if (success == TRUE)
                {
                    /* IP Header Checksum */
                    for (i = 0; i < 20; i += 2)
                    {
                        cs16 += ((data[i]<<8) | data[i+1]);
                    }
                    cs16 = ( cs16  & 0xFFFF) + (cs16 >> 16);
                    cs16 = ( cs16  & 0xFFFF) + (cs16 >> 16);
                    cs16 = (~cs16) & 0xFFFF;
                    data[10] = (Int8)(cs16 >> 8);
                    data[11] = (Int8)(cs16  & 0x00ff);

                    if (packet + segment >= vgMGSINKData_p->packetSize) /* The first segment */
                    {
                    /* TCP +  Source=0      | Dest=9         +  00 00 00 09
                           |  Sequence number                |  XX XX XX XX
                           |  Acknowledgement number         |  XX XX XX XX
                           |  Off | R | CTR | Window         |  XX XX XX XX
                           |  Chksum        | Urg            |  XX XX XX XX */
                        data[20] = 0x00; data[21] = 0x00;                /* Source (optional) */
                        data[22] = (Int8)(vgMGSINKData_p->port >> 8); data[23] = (Int8)(vgMGSINKData_p->port & 0x00ff);  /* Dest */
                        data[24] = (Int8) (sequenceNumber >> 24);       /* seqnum */
                        data[25] = (Int8) (sequenceNumber >> 16);       /* seqnum */
                        data[26] = (Int8) (sequenceNumber >> 8);        /* seqnum */
                        data[27] = (Int8) (sequenceNumber) ;             /* seqnum */
                        sequenceNumber += vgMGSINKData_p->packetSize;
                        data[28] = 0 ;                                   /* acknum */
                        data[29] = 0 ;                                   /* acknum */
                        data[30] = 0 ;                                   /* acknum */
                        data[31] = 0 ;                                   /* acknum */
                        data[32] = (Int8) (5 << 4) ; /* data offset + reserved */
                        data[33] = 0x10 ; /* reserved + control - ack bit set to trigger compression*/
                        data[34] = 0xFF ; /* window */
                        data[35] = 0xFF ; /* window */

                        /* TCP Checksum - not important for compression */
                        data[36] = 0;
                        data[37] = 0;

                        data[38] = 0; /* urg */
                        data[39] = 0; /* urg */
                        i = 40;
                    }
                    else
                    {
                        i = 20;
                    }

                    /* Fill the NPDU with some 'random' data */
                    for (; i < segment; i++)
                    {
                        data[i] = (Int8) (vgMGSINKData_p->packetId + sequenceNumber + i);
                    }
                }

                sigBuf.sig->snDataReq.nsapi             = (Nsapi)(vgMGSINKData_p->psdBearerId);
                sigBuf.sig->snDataReq.npdu.dataOffset   = 0;
                sigBuf.sig->snDataReq.npdu.length       = segment;
                sigBuf.sig->snDataReq.pdpPacketTypeHdr  = GPRS_PCOMP_UNCOMPRESSED;
                sigBuf.sig->snDataReq.pdpPacketTypeData = GPRS_DCOMP_UNCOMPRESSED;

                /* Send the signal to SNDCP to transmit */
                KiSendSignal (GP_SNDCP_TASK_ID, &sigBuf);
            }
        }
    }

    if (success == TRUE)
    {
        /* Store updated paramaters in MGSINK data */
        vgMGSINKData_p->packetCount     = packetCount;
        vgMGSINKData_p->packetId        = packetId;
        vgMGSINKData_p->sequenceNumber  = sequenceNumber;
        vgMGSINKData_p->packet          = packet;
        vgMGSINKData_p->segment         = segment;
    }

    if ((seriousFailure == TRUE) ||                       /* Give up */
        ((vgMGSINKData_p->packet == 0) &&
         (vgMGSINKData_p->packetCount == 0)))
    {
        /* Finished */
        /* Check if there is a pending request */
        if (vgMGSINKData_p->next != PNULL)
        {
            /* There is a pending request. Copy it into current one. */
            VgMGSINKData *newVgMGSINKData_p = vgMGSINKData_p->next;
            memcpy (vgMGSINKData_p, newVgMGSINKData_p, sizeof(VgMGSINKData));
            KiFreeMemory ((void **)&newVgMGSINKData_p);
        }
        else
        {

            vgMGSINKData_p->psdBearerId     = PSD_BEARER_ID_5;

            vgMGSINKData_p->mgtcsinkActive  = FALSE;

#if defined (USE_L4MM_ALLOC_MEMORY)
            /* De-register from L4MM UL flow control indications.
            */
            /* TODO:TmmRemoveFlowControlTask (TMM_DEFAULT_POOL_UL, VG_CI_TASK_ID); */

#else /* USE_L4MM_ALLOC_MEMORY */
            /* Remove ATCI as flow controller for TMM.
             * Note that this may be just decrementing instances of ATCI task
             * in flow control data in TMM.
             */
#if !defined (USE_BMM_ALLOC_MEMORY)
            TmmRemoveFlowControlTask (TMM_DEFAULT_POOL_UL, VG_CI_TASK_ID);
#endif
#endif /* USE_L4MM_ALLOC_MEMORY */

        }
    }

    return;
}

/***************************************************************************
 *
 * Function:    vgProcMgtcSinkTxPdu
 *
 * Parameters:
 *
 * Returns:
 *
 * Description:
 *
 ***************************************************************************/
void vgProcMgtcSinkTxPdu (void)
{
    Int8                  psdBearerIdEntry     = 0;

    GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
    Boolean               sendMore = TRUE;

    /* Send UL NPDUs alternately on NSAPIs running MGTCSINK */
    while (sendMore == TRUE)
    {
        /* Find out if there is an PSD Bearer running MGTCSINK */
        sendMore = FALSE;
        for (psdBearerIdEntry = 0; psdBearerIdEntry < MAX_NUM_PSD_BEARERS; psdBearerIdEntry++)
        {
            if (gprsGenericContext_p->vgMGSINKData[psdBearerIdEntry].mgtcsinkActive == TRUE)
            {
                /* Found an NSAPI that is running MGSINK */
                sendMore = TRUE;
                break;
            }
        }

        /* There is at least one PSD Bearer IDs still expecting to send UL data.
         * Loop through all NSAPIs and send 1 UL packet on all waiting if possible. */
        if (sendMore == TRUE)
        {
            for (psdBearerIdEntry = 0; psdBearerIdEntry < MAX_NUM_PSD_BEARERS; psdBearerIdEntry++)
            {
                if (gprsGenericContext_p->vgMGSINKData[psdBearerIdEntry].mgtcsinkActive == TRUE)
                {
                    /* Found an NSAPI that is running MGTCSINK.
                     * Try to send one UL packet on it.
                     */
                    vgProcMgtcSinkTxOnePduOnNsapi (&gprsGenericContext_p->vgMGSINKData[psdBearerIdEntry]);
                }
            }

#if defined (USE_L4MM_ALLOC_MEMORY)
            if (L4mmIsFlowControlLevelReached(L4MM_UL_POOL) == TRUE)
#else
            if (TmmCanAllocFromPool(TMM_DEFAULT_POOL_UL) == FALSE)
#endif

            {
                /* TMM HWM reached, stop here and wait for LWM to continue */
                sendMore = FALSE;
            }
        }
    }

    return;
}

/***************************************************************************
 *
 * Function:    vgProcMgSinkExtAssign
 *
 * Parameters:
 *
 * Returns:
 *
 * Description: Extracts the parameters from the command line
 *     *MGSINK = [NSAPI], [PacketSize], [PacketCount], [Address], [Port]
 ***************************************************************************/
static ResultCode_t  vgProcMgSinkExtAssign (CommandLine_t *commandBuffer_p,
                                            const VgmuxChannelNumber entity)
{
    Int32              parameter;

    Int8               psdBearerId             = PSD_BEARER_ID_5;
    TextualPdnAddress  pdnAddress              = {0};
    Int8               psdBearerIdEntry        = 0;
    Int16              packetSize   = IP_MTU-IP_LEN-UDP_LEN; /* MTU packet */
    Int16              packetCount  = 1;
    /* allow more space for the string size so we capture user error */
    Char               tmpString [STRING_LENGTH_40 + NULL_TERMINATOR_LENGTH] = {0};
    Int16              tmpStringLen;
    Int16              port         = 9;  /* sink */
    ResultCode_t       result       = RESULT_CODE_OK;
    VgMGSINKData       *vgMGSINKData_p = PNULL;
    Boolean            found        = FALSE;
    GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
    Boolean            requestSaved = FALSE;

    /* PSD Bearer ID */
    if ( ( result == RESULT_CODE_OK ) &&
         ( getExtendedParameter (commandBuffer_p, &parameter, PSD_BEARER_ID_5) ) &&
         ( parameter >= PSD_BEARER_ID_5 && parameter <= PSD_BEARER_ID_15 ) ) /* Check the nsapi */
    {
        psdBearerId = (Int8) parameter;
    }
    else
    {
        result = RESULT_CODE_ERROR;
    }

    /* PacketSize (Number of octets to send) */
    if ( ( result == RESULT_CODE_OK ) &&
         ( getExtendedParameter ( (CommandLine_t*)commandBuffer_p,
                                   &parameter,
                                    IP_MTU - IP_LEN - UDP_LEN) ) &&
         ( parameter <= 10000 ) ) /* Check the PacketSize */
    {
        packetSize = (Int16) parameter;
    }
    else
    {
        result = RESULT_CODE_ERROR;
    }

    /* PacketCount (Number of packets to send) */
    if ( ( result == RESULT_CODE_OK ) &&
         ( getExtendedParameter ( (CommandLine_t*)commandBuffer_p, &parameter, 1) )  &&
         ( parameter > 0 && parameter <= 20 ) ) /* Check the packetCount */
    {
        packetCount = (Int16) parameter;
    }
    else
    {
        result = RESULT_CODE_ERROR;
    }

    /* Pdn address ("255.255.255.255") */
    if ((result == RESULT_CODE_OK) &&
        (getExtendedString (commandBuffer_p,
                            tmpString,
                            STRING_LENGTH_40,
                            &tmpStringLen)))
    {
        if (tmpStringLen == 0)
        {
            /* no parameter so undefine it */
            pdnAddress.addressPresent = FALSE;
            pdnAddress.length = 0;
        }
        else
        {
            if (tmpStringLen <= MAX_TEXTUAL_PDN_ADDR &&
                tmpStringLen <= STRING_LENGTH_40)
            {
                /* reset the value to 0 */
                memset (pdnAddress.address, 0, MAX_TEXTUAL_PDN_ADDR);
                memcpy (pdnAddress.address, tmpString, tmpStringLen);
                pdnAddress.length = (Int8) tmpStringLen;
                pdnAddress.addressPresent = TRUE;
            }
            else
            {
                pdnAddress.addressPresent = FALSE;
                pdnAddress.length = 0;
                result = RESULT_CODE_ERROR;
            }
        }
    }
    else /* error reading supplied parameter */
    {
        result = RESULT_CODE_ERROR;
    }

    /* UDP Port address (7 echo, 9 discard/sink/null or any user defined) */
    if ( ( result == RESULT_CODE_OK ) &&
         ( getExtendedParameter ((CommandLine_t*)commandBuffer_p, &parameter, 9)) )
    {
        port = (Int16) parameter;
    }
    else
    {
        result = RESULT_CODE_ERROR;
    }

    /* Process the parameters */
    if (result == RESULT_CODE_OK)
    {

        for (psdBearerIdEntry = 0 ; (psdBearerIdEntry < MAX_NUM_PSD_BEARERS && !found); psdBearerIdEntry++)
        {
            vgMGSINKData_p = &(gprsGenericContext_p->vgMGSINKData[psdBearerIdEntry]);

            if ((vgMGSINKData_p->psdBearerId == psdBearerId) &&
                ((vgMGSINKData_p->mgsinkActive == TRUE) ||
                 (vgMGSINKData_p->mgtcsinkActive == TRUE)))
            {
                /* There is an MGSINK or MGTCSINK command already running on this NSAPI.
                 * Store the new request to retrieve and process later when the current
                 * one is finished.
                 */
                VgMGSINKData    *newVgMGSINKData_p = PNULL;

                /* Allocate memory */
                KiAllocMemory (sizeof(VgMGSINKData), (void **)&newVgMGSINKData_p);

                /* Find the last element in the linked list */
                while (vgMGSINKData_p->next != PNULL)
                {
                    vgMGSINKData_p = vgMGSINKData_p->next;
                }

                vgMGSINKData_p->next = newVgMGSINKData_p;

                /* Store MGSINK information and set active */
                newVgMGSINKData_p->psdBearerId  = psdBearerId;
                newVgMGSINKData_p->mgsinkActive = TRUE;
                newVgMGSINKData_p->packetSize   = packetSize;
                newVgMGSINKData_p->packetCount  = packetCount;
                memcpy (&newVgMGSINKData_p->pdnAddress, &pdnAddress, sizeof(TextualPdnAddress));
                newVgMGSINKData_p->port         = port;
                newVgMGSINKData_p->packetId     = 0;            /* Initial value */
                newVgMGSINKData_p->packet       = 0;            /* Initial value */
                newVgMGSINKData_p->segment      = 0;            /* Initial value */
                newVgMGSINKData_p->next         = PNULL;

                requestSaved    = TRUE;
                found           = TRUE;     /* to get out of loop */
            }

            if ((requestSaved == FALSE) &&
                (vgMGSINKData_p->mgsinkActive == FALSE) &&
                (vgMGSINKData_p->mgtcsinkActive == FALSE))
            {
                /* Store MGSINK information and set active */
                vgMGSINKData_p->psdBearerId     = psdBearerId;
                vgMGSINKData_p->mgsinkActive    = TRUE;
                vgMGSINKData_p->packetSize      = packetSize;
                vgMGSINKData_p->packetCount     = packetCount;
                memcpy (&vgMGSINKData_p->pdnAddress, &pdnAddress, sizeof(TextualPdnAddress));
                vgMGSINKData_p->port            = port;
                vgMGSINKData_p->packetId        = 0;            /* Initial value */
                vgMGSINKData_p->packet          = 0;            /* Initial value */
                vgMGSINKData_p->segment         = 0;            /* Initial value */
                vgMGSINKData_p->next            = PNULL;

                found = TRUE;
            }
        }
    }

    if (found == FALSE)
    {
        result = RESULT_CODE_ERROR;
    }

    if ((result == RESULT_CODE_OK) &&
        (requestSaved == FALSE))
    {
#if defined (USE_L4MM_ALLOC_MEMORY)
        /* TODO: Replace PNULL with callback the function*/
        L4mmRegCallback(PNULL, L4MM_UL_POOL);
#else /* USE_L4MM_ALLOC_MEMORY */
        /* Add ATCI as flow controller for TMM.
         * Note that this may be just incrementing instances of ATCI task
         * in flow control data in TMM.
         */
#if !defined (USE_BMM_ALLOC_MEMORY)
        TmmAddFlowControlTask (TMM_DEFAULT_POOL_UL, VG_CI_TASK_ID);
#endif
#endif /* USE_L4MM_ALLOC_MEMORY */

        vgProcMgSinkTxPdu ();
    }

    return (result);
}

/***************************************************************************
 *
 * Function:    vgProcMgtcSinkExtAssign
 *
 * Parameters:
 *
 * Returns:
 *
 * Description: Extracts the parameters from the command line
 *     *MGTCSINK = [NSAPI], [PacketSize], [PacketCount], [Address], [Port]
 ***************************************************************************/
static ResultCode_t  vgProcMgtcSinkExtAssign (CommandLine_t *commandBuffer_p,
                                              const VgmuxChannelNumber entity)

{
    Int32              parameter;

    Int8               psdBearerId             = PSD_BEARER_ID_5;
    TextualPdnAddress  pdnAddress              = {0};
    Int8               psdBearerIdEntry        = 0;
    Int16              packetSize   = IP_MTU-IP_LEN-TCP_LEN; /* MTU packet */
    Int16              packetCount  = 1;
    /* allow more space for the string size so we capture user error */
    Char               tmpString [STRING_LENGTH_40] = {0};
    Int16              tmpStringLen;
    Int16              port         = 9;  /* sink */
    ResultCode_t       result       = RESULT_CODE_OK;
    VgMGSINKData       *vgMGSINKData_p = PNULL;
    Boolean            found        = FALSE;
    GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
    Boolean            requestSaved = FALSE;

    /* PSD Bearer ID */
    if ( ( result == RESULT_CODE_OK ) &&
         ( getExtendedParameter (commandBuffer_p, &parameter, PSD_BEARER_ID_5) ) &&
         ( parameter >= PSD_BEARER_ID_5 && parameter <= PSD_BEARER_ID_15 ) ) /* Check the nsapi */
    {
        psdBearerId = (Int8) parameter;
    }
    else
    {
        result = RESULT_CODE_ERROR;
    }

    /* PacketSize (Number of octets to send) */
    if ( ( result == RESULT_CODE_OK ) &&
         ( getExtendedParameter ( (CommandLine_t*)commandBuffer_p,
                                   &parameter,
                                    IP_MTU - IP_LEN - TCP_LEN) ) &&
         ( parameter <= 10000 ) ) /* Check the PacketSize */
    {
        packetSize = (Int16) parameter;
    }
    else
    {
        result = RESULT_CODE_ERROR;
    }

    /* PacketCount (Number of packets to send) */
    if ( ( result == RESULT_CODE_OK ) &&
         ( getExtendedParameter ( (CommandLine_t*)commandBuffer_p, &parameter, 1) )  &&
         ( parameter > 0 && parameter <= 20 ) ) /* Check the packetCount */
    {
        packetCount = (Int16) parameter;
    }
    else
    {
        result = RESULT_CODE_ERROR;
    }

    /* Pdn address ("255.255.255.255") */
    if ((result == RESULT_CODE_OK) &&
        (getExtendedString (commandBuffer_p,
                            tmpString,
                            STRING_LENGTH_40,
                            &tmpStringLen)))
    {
        if (tmpStringLen == 0)
        {
            /* no parameter so undefine it */
            pdnAddress.addressPresent = FALSE;
            pdnAddress.length = 0;
        }
        else
        {
            if (tmpStringLen <= MAX_TEXTUAL_PDN_ADDR &&
                tmpStringLen <= STRING_LENGTH_40)
            {
                /* reset the value to 0 */
                memset (pdnAddress.address, 0, MAX_TEXTUAL_PDN_ADDR);
                memcpy (pdnAddress.address, tmpString, tmpStringLen);
                pdnAddress.length = (Int8) tmpStringLen;
                pdnAddress.addressPresent = TRUE;
            }
            else
            {
                pdnAddress.addressPresent = FALSE;
                pdnAddress.length = 0;
                result = RESULT_CODE_ERROR;
            }
        }
    }
    else /* error reading supplied parameter */
    {
        result = RESULT_CODE_ERROR;
    }

    /* TCP Port address (7 echo, 9 discard/sink/null or any user defined) */
    if ( ( result == RESULT_CODE_OK ) &&
         ( getExtendedParameter ((CommandLine_t*)commandBuffer_p, &parameter, 9)) )
    {
        port = (Int16) parameter;
    }
    else
    {
        result = RESULT_CODE_ERROR;
    }

    /* Process the parameters */
    if (result == RESULT_CODE_OK)
    {
        for (psdBearerIdEntry = 0 ; (psdBearerIdEntry < MAX_NUM_PSD_BEARERS && !found); psdBearerIdEntry++)
        {
            vgMGSINKData_p = &(gprsGenericContext_p->vgMGSINKData[psdBearerIdEntry]);

            if ((vgMGSINKData_p->psdBearerId == psdBearerId) &&
                ((vgMGSINKData_p->mgsinkActive == TRUE) ||
                 (vgMGSINKData_p->mgtcsinkActive == TRUE)))
            {
                /* There is an MGSINK or MGTCSINK command already running on this NSAPI.
                 * Store the new request to retrieve and process later when the current
                 * one is finished.
                 */
                VgMGSINKData    *newVgMGSINKData_p = PNULL;

                /* Allocate memory */
                KiAllocMemory (sizeof(VgMGSINKData), (void **)&newVgMGSINKData_p);

                /* Find the last element in the linked list */
                while (vgMGSINKData_p->next != PNULL)
                {
                    vgMGSINKData_p = vgMGSINKData_p->next;
                }

                vgMGSINKData_p->next = newVgMGSINKData_p;

                /* Store MGSINK information and set active */
                newVgMGSINKData_p->psdBearerId      = psdBearerId;
                newVgMGSINKData_p->mgtcsinkActive   = TRUE;
                newVgMGSINKData_p->packetSize       = packetSize;
                newVgMGSINKData_p->packetCount      = packetCount;
                memcpy (&newVgMGSINKData_p->pdnAddress, &pdnAddress, sizeof(TextualPdnAddress));
                newVgMGSINKData_p->port             = port;
                newVgMGSINKData_p->packetId         = 0;            /* Initial value */
                vgMGSINKData_p->sequenceNumber      = 0;            /* Initial value */
                newVgMGSINKData_p->packet           = 0;            /* Initial value */
                newVgMGSINKData_p->segment          = 0;            /* Initial value */
                newVgMGSINKData_p->next             = PNULL;

                requestSaved    = TRUE;
                found           = TRUE;     /* to get out of loop */
            }

            if ((requestSaved == FALSE) &&
                (vgMGSINKData_p->mgsinkActive == FALSE) &&
                (vgMGSINKData_p->mgtcsinkActive == FALSE))
            {
                /* Store MGTCSINK information and set active */
                vgMGSINKData_p->psdBearerId     = psdBearerId;
                vgMGSINKData_p->mgtcsinkActive  = TRUE;
                vgMGSINKData_p->packetSize      = packetSize;
                vgMGSINKData_p->packetCount     = packetCount;
                memcpy (&vgMGSINKData_p->pdnAddress, &pdnAddress, sizeof(TextualPdnAddress));
                vgMGSINKData_p->port            = port;
                vgMGSINKData_p->packetId        = 0;            /* Initial value */
                vgMGSINKData_p->sequenceNumber  = 0;            /* Initial value */
                vgMGSINKData_p->packet          = 0;            /* Initial value */
                vgMGSINKData_p->segment         = 0;            /* Initial value */
                vgMGSINKData_p->next            = PNULL;

                found = TRUE;
            }
        }
    }
    if (found == FALSE)
    {
        result = RESULT_CODE_ERROR;
    }

    if ((result == RESULT_CODE_OK) &&
        (requestSaved == FALSE))
    {
#if defined (USE_L4MM_ALLOC_MEMORY)
        /* TODO: Replace PNULL with callback the function*/
        L4mmRegCallback(PNULL, L4MM_UL_POOL);
#else /* USE_L4MM_ALLOC_MEMORY */
        /* Add ATCI as flow controller for TMM.
         * Note that this may be just incrementing instances of ATCI task
         * in flow control data in TMM.
         */
#if !defined (USE_BMM_ALLOC_MEMORY)
        TmmAddFlowControlTask (TMM_DEFAULT_POOL_UL, VG_CI_TASK_ID);
#endif
#endif /* USE_L4MM_ALLOC_MEMORY */
        vgProcMgtcSinkTxPdu ();
    }

    return (result);
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*************************************************************************
*
* Function:     vgAllocateRamToCid
*
* Parameters:   cid - pdp context to allocate memory for
*
* Returns:      TRUE if pointer assigned successfully
*
* Description:  Assigns static RAM to the specified PDP context
*
*************************************************************************/

Boolean vgAllocateRamToCid (const Int32 cid)
{

  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo       *dPtr = PNULL;
  Int8                  inUsePsdDataItemCount;
  Boolean               success = FALSE;

  /* allocate some memory if we need to as we are in dynamic mode */
  if (gprsGenericContext_p->cidUserData [cid] == PNULL)
  {
    /* Search for PSD data item not in use and assign CID point to it */
    for (inUsePsdDataItemCount=0; !success && (inUsePsdDataItemCount < ATCI_MAX_NUM_PSD_CONNECTIONS); inUsePsdDataItemCount++)
    {
      if (gprsGenericContext_p->inUsePsdMemData[inUsePsdDataItemCount].inUse == FALSE)
      {
        gprsGenericContext_p->inUsePsdMemData[inUsePsdDataItemCount].inUse = TRUE;
        dPtr = &(gprsGenericContext_p->inUsePsdMemData[inUsePsdDataItemCount].vgPsdStatusInfo);
        allocateMemToGprsContext (cid, (void *)dPtr);
        success = TRUE;
      }
    }

    if ( PNULL != dPtr )
    {
      /* Initialise the cid's data */
      vgInitialiseCidData (gprsGenericContext_p->cidUserData[cid], cid);
    }
  }
  else
  {
    /* Memory already assigned for CID - so say success already */
    success = TRUE;
  }

  return(success);
}

/*************************************************************************
*
* Function:     vgFreeRamForCid
*
* Parameters:   cid - pdp context to allocate memory for
*
* Returns:      nothing
*
* Description:  Frees static RAM for the specified PDP context
*
*************************************************************************/
void vgFreeRamForCid             (const Int32 cid )
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  Int8                  inUsePsdDataItemCount;
  Boolean               found = FALSE;

  if (gprsGenericContext_p->cidUserData [cid] != PNULL)
  {
    /* Search for PSD data item assigned to cid */
    for (inUsePsdDataItemCount=0; !found && (inUsePsdDataItemCount < ATCI_MAX_NUM_PSD_CONNECTIONS); inUsePsdDataItemCount++)
    {
      if ((gprsGenericContext_p->inUsePsdMemData[inUsePsdDataItemCount].inUse == TRUE) &&
         (&(gprsGenericContext_p->inUsePsdMemData[inUsePsdDataItemCount].vgPsdStatusInfo) ==
          gprsGenericContext_p->cidUserData [cid]))
      {
        gprsGenericContext_p->inUsePsdMemData[inUsePsdDataItemCount].inUse = FALSE;
        gprsGenericContext_p->cidUserData [cid] = PNULL;
        found = TRUE;
      }
    }
  }
}

/*--------------------------------------------------------------------------
*
* Function:     vgAssignNewActiveContext
*
* Parameters:  cid    - context identifier
*              entity - mux channel number
*
* Returns:     Boolean - whether assignment successful
*
* Description: changes the active context if cid specified is valid.
*              Note this is only called for connections
*              requiring a data connection - so always assigns the
*              activePsdBearerContextWithDataConn to the appropriate cidUserData.
#endif
*
*-------------------------------------------------------------------------*/

Boolean vgAssignNewActiveContext (const Int8 cid,
                                   const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  GprsContext_t         *gprsContext_p = ptrToGprsContext(entity);
  Boolean               success = FALSE;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(gprsContext_p != PNULL, entity, 0, 0);
#endif
  /* check cid is within range */
  /* job127550: allow for transient CID */
  if ((cid >= vgGpGetMinCidValue(entity)) && (cid < MAX_NUMBER_OF_CIDS))
  {
    success = vgAllocateRamToCid (cid);

    if (success)
    {
      /* set up active context pointers */
      gprsContext_p->activePsdBearerContextWithDataConn =  gprsGenericContext_p->cidUserData[cid];
    }
  }

  return (success);
}

/*--------------------------------------------------------------------------
*
* Function:     vgInitialiseCidData
*
* Parameters:   ptr     - cid profile to be initialised
*               thisCid - cid number for given context
*
* Returns:      nothing
*
* Description:  Initialises the PSD status data for the requested cid
*
*-------------------------------------------------------------------------*/

void vgInitialiseCidData (VgPsdStatusInfo *ptr, Int32 thisCid)
{

  ptr->cid = (Int8)thisCid;

  /* cgdcont data */
  vgInitialiseCgdcontData (ptr);

#if defined (FEA_DEDICATED_BEARER)
  /* cgdscont data */
  vgInitialiseCgdscontData (ptr);
#endif /* FEA_DEDICATED_BEARER */

#if defined (FEA_QOS_TFT)
  /* Initialise the eqos data */
  vgInitialiseCgeqosData (ptr);
  /* cgtft data */
  vgInitialiseCgtftData (ptr);
#endif

  ptr->profileDefined        = FALSE;
  ptr->isActive              = FALSE;

  ptr->pendingContextActivation = FALSE;
  ptr->pendingDataConnectionActivation = FALSE;

#if defined (FEA_QOS_TFT)
  ptr->pendingContextModification = FALSE;
  ptr->modifyPending = FALSE;
  ptr->vgModifyChannel = VGMUX_CHANNEL_INVALID;
#endif /* FEA_QOS_TFT */

  ptr->pendingContextDeactivation = FALSE;
  ptr->pendingUnsolicitedContextActivation = FALSE;

#if defined (FEA_DEDICATED_BEARER)
  ptr->secondaryContextCidPendingActivation = CID_NUMBER_UNKNOWN;
#endif /* FEA_DEDICATED_BEARER */

  /* SNDCP data counters */
  ptr->countReportType   = ONCE;
  ptr->countReportPeriod = 0;

  ptr->psdBearerInfo.channelNumber = VGMUX_CHANNEL_INVALID;
  ptr->psdBearerInfo.connType = ABPD_CONN_TYPE_NONE;
  ptr->psdBearerInfo.connId = INVALID_CONN_ID;
  ptr->psdBearerInfo.psdBearerId = PSD_BEARER_ID_UNASSIGNED;

  vgInitialisePsdUserData (ptr);

  ptr->psdBearerInfo.modifiedBySim = FALSE;
  ptr->psdBearerInfo.simMods = 0;

  ptr->psdBearerInfo.apnAmbrPresent = FALSE;
  ptr->psdBearerInfo.apnAmbr.apnAmbrDownlink = 0;
  ptr->psdBearerInfo.apnAmbr.apnAmbrUplink = 0;

  /* Initialise negotiated APN and textual APN */
  ptr->psdBearerInfo.negApnPresent = FALSE;
  ptr->psdBearerInfo.negApn.length = 0;
  memset (ptr->psdBearerInfo.negApn.name,
          NULL_CHAR,
          sizeof (ptr->psdBearerInfo.negApn.name));
  ptr->psdBearerInfo.negTextualApn.length = 0;
  memset (ptr->psdBearerInfo.negTextualApn.name,
          NULL_CHAR,
          sizeof (ptr->psdBearerInfo.negTextualApn.name));

  /* Initialise  APN Rate Control Parameters */
  ptr->apnUplinkRateControlInfo.apnRateControlPresent = FALSE;
  ptr->apnUplinkRateControlInfo.apnRateControlAdditionalExceptionReportsAllowed = FALSE;
  ptr->apnUplinkRateControlInfo.apnRateControlMaxUplinkRate = 0;
  ptr->apnUplinkRateControlInfo.apnRateControluplinkTimeUnit = 0;

#if defined (FEA_QOS_TFT)
  /* Initialise negotiated QOS */
  ptr->psdBearerInfo.negQosPresent = FALSE;
  ptr->psdBearerInfo.negotiatedQos.epsQualityOfServicePresent = FALSE;
  ptr->psdBearerInfo.negotiatedQos.epsBitRatesPresent = FALSE;

  /* Initialise negotiated TFT */
  ptr->psdBearerInfo.negTftPresent = FALSE;
  memset (&ptr->psdBearerInfo.negotiatedTft, 0, sizeof(TrafficFlowTemplate));
#endif /* FEA_QOS_TFT */

  ptr->psdBearerInfo.flowControlType = FC_NONE;

  ptr->psdBearerInfo.ipv4MtuSizePresent = FALSE;
  ptr->psdBearerInfo.ipv4LinkMTU = 0;
  ptr->psdBearerInfo.nonIPMtuSizePresent = FALSE;
  ptr->psdBearerInfo.nonIPLinkMTU = 0;
}

/*--------------------------------------------------------------------------
*
* Function:     vgCheckProfileUndefined
*
* Parameters:   ptr     - cid profile to be initialised
*               thisCid - cid number for given context (note: =actual cid-1)
*
* Returns:      nothing
*
* Description:  If the cid has no CGxxx command data undefine it
*
*-------------------------------------------------------------------------*/

static void vgCheckProfileUndefined (VgPsdStatusInfo *ptr,    Int32 thisCid)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();

  if (!ptr->cgdcontDefined
      && !ptr->cgdscontDefined
#if defined (FEA_QOS_TFT)
      && !ptr->cgtftDefined
      && !ptr->cgeqosDefined
#endif /* (FEA_QOS_TFT)  */
     )
  {
    ptr->profileDefined = FALSE;
    gprsGenericContext_p->cidOwner[thisCid] = VGMUX_CHANNEL_INVALID;
  }
}

/*--------------------------------------------------------------------------
*
* Function:     vgInitialisePdnAddress
*
* Parameters:   pdnAddress_p   - pdnAddress to be initialised
*
* Returns:      nothing
*
* Description:  Initialises ths given PDN address information
*
*-------------------------------------------------------------------------*/
void vgInitialisePdnAddress (PdnAddress *pdnAddress_p)
{
  pdnAddress_p->addressPresent = FALSE;
  pdnAddress_p->pdnType = PDN_TYPE_IPV4;

  memset (pdnAddress_p->ipv4Address,
          0,
          IPV4_ADDR_LEN);

  pdnAddress_p->ipv6AddressType = IPV6_ADDRESS_FULL;

  memset (pdnAddress_p->ipv6Address,
          0,
          MAX_IPV6_ADDR_LEN);
}

/*--------------------------------------------------------------------------
*
* Function:     vgInitialiseCgdcontData
*
* Parameters:   ptr     - cid profile to be initialised
*
* Returns:      nothing
*
* Description:  Initialises the context CGDCONT profile data
*
*-------------------------------------------------------------------------*/
static void vgInitialiseCgdcontData (VgPsdStatusInfo *ptr)
{
  ptr->cgdcontDefined        = FALSE;

  /* cgdcont data */
  ptr->psdBearerInfo.reqPdnAddress.addressPresent = FALSE;
  ptr->psdBearerInfo.reqPdnAddress.pdnType  = PDN_TYPE_IPV4;

  /* Initialise required APN and textual APN */
  memset (ptr->psdBearerInfo.reqApn.name,
          NULL_CHAR,
          sizeof (ptr->psdBearerInfo.reqApn.name));
  ptr->psdBearerInfo.reqApn.length = 0;
  ptr->psdBearerInfo.reqApnPresent = FALSE;

  memset (ptr->psdBearerInfo.reqTextualApn.name,
          NULL_CHAR,
          sizeof (ptr->psdBearerInfo.reqTextualApn.name));
  ptr->psdBearerInfo.reqTextualApn.length = 0;

  /* Clear all the negotiated connection address information we will get from
   * the network.
   */

  vgInitialisePdnAddress (&ptr->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress);
  vgInitialisePdnAddress (&ptr->psdBearerInfo.pdnConnectionAddressInfo.primaryDnsAddress);
  vgInitialisePdnAddress (&ptr->psdBearerInfo.pdnConnectionAddressInfo.secondaryDnsAddress);
  vgInitialisePdnAddress (&ptr->psdBearerInfo.pdnConnectionAddressInfo.gatewayAddress);
  vgInitialisePdnAddress (&ptr->psdBearerInfo.pdnConnectionAddressInfo.subnetMask);

  /* Clear requested IP address */
  vgInitialisePdnAddress (&ptr->psdBearerInfo.reqPdnAddress);

  ptr->psdBearerInfo.headerComp = HEADER_COMP_OFF;
  ptr->psdBearerInfo.dataComp = DATA_COMP_OFF;

  ptr->psdBearerInfo.ipv4LinkMTURequest  = MTU_SIZE_NOT_REQUESTED;
  ptr->psdBearerInfo.nonIPLinkMTURequest = MTU_SIZE_NOT_REQUESTED;

#if defined(FEA_TEMP_REL14_ABPD_DUAL_PRIORITY)
  ptr->psdBearerInfo.nasSigPriority = VG_MS_CONFIGURED_FOR_LOW_PRIO;
#endif
}

#if defined (FEA_DEDICATED_BEARER)
/*--------------------------------------------------------------------------
*
* Function:     vgInitialiseCgdscontData
*
* Parameters:   ptr     - cid profile to be initialised
*
* Returns:      nothing
*
* Description:  Initialises the pdp CGDSCONT profile data
*
*-------------------------------------------------------------------------*/

static void vgInitialiseCgdscontData (VgPsdStatusInfo *ptr)
{
  ptr->cgdscontDefined        = FALSE;

#if defined (FEA_MT_PDN_ACT)
  ptr->mtActivatedDedicatedBearer = FALSE;
#endif

  /* cgdscont data */
  ptr->psdBearerInfo.secondaryContext = FALSE;
  ptr->psdBearerInfo.primaryConnId = INVALID_CONN_ID;
  ptr->psdBearerInfo.primaryCid = 0;
  ptr->psdBearerInfo.primaryPsdBearerId = PSD_BEARER_ID_UNASSIGNED;

  ptr->psdBearerInfo.headerComp = HEADER_COMP_OFF;
  ptr->psdBearerInfo.dataComp = DATA_COMP_OFF;
}
#endif /* FEA_DEDICATED_BEARER */


#if defined (FEA_QOS_TFT)
/*--------------------------------------------------------------------------
*
* Function:     vgInitialiseCgtftData
*
* Parameters:   ptr     - cid profile to be initialised
*
* Returns:      nothing
*
* Description:  Initialises the pdp CGTFT profile data
*
*-------------------------------------------------------------------------*/

static void vgInitialiseCgtftData (VgPsdStatusInfo *ptr)
{
  ptr->cgtftDefined        = FALSE;

  /* Initialise required TFT data */
  ptr->psdBearerInfo.reqTftPresent = FALSE;
  memset (&ptr->psdBearerInfo.requiredTft, 0, sizeof(TrafficFlowTemplate));
}
#endif /* FEA_QOS_TFT */

/*--------------------------------------------------------------------------
*
* Function:     vgInitialisePsdUserData
*
* Parameters:   ptr     - cid profile to be initialised
*
* Returns:      nothing
*
* Description:  Initialises the username and password profile data
*
*-------------------------------------------------------------------------*/
static void vgInitialisePsdUserData  (VgPsdStatusInfo *ptr)
{
  ptr->psdBearerInfo.psdUser.usernamePresent = FALSE;
  ptr->psdBearerInfo.psdUser.usernameLength  = 0;
  memset (ptr->psdBearerInfo.psdUser.username, 0, MAX_PSD_USERNAME_LENGTH);

  ptr->psdBearerInfo.psdUser.passwdPresent = FALSE;
  ptr->psdBearerInfo.psdUser.passwdLength  = 0;
  memset (ptr->psdBearerInfo.psdUser.passwd, 0, MAX_PSD_PWD_LENGTH);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgActivateContext
 *
 * Parameters:  cid    - context identifier whose parameters are to be used
 *                       in the context activation
 *              entity - mux channel number
 *              connectionType - Connection type. Can be PPP, core PPP or
 *                               Packet Transport.
 *
 * Returns:     ResultCode_t  - result after initiating context activation
 *
 * Description: Attempts to activate a context described by 'cid'
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgActivateContext (Int8 cid,
                                const VgmuxChannelNumber entity,
                                AbpdPdpConnType connectionType)
{
  ResultCode_t         result                = RESULT_CODE_OK;
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  GprsContext_t        *gprsContext_p        = ptrToGprsContext(entity);
  VgPsdStatusInfo      *ptr;
#if defined (FEA_DEDICATED_BEARER)
  VgPsdStatusInfo      *primaryPtr;
#endif
  ConnectionClass_t    connectionClass;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(gprsContext_p != PNULL, entity, cid, 0);
#endif
  /* Point to context */
  ptr = gprsGenericContext_p->cidUserData[cid];
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (ptr != PNULL, entity, cid, 0);
#endif
  /* Assign to activePsdBearerContextWithDataConn if PT or PPP mode */
  if (connectionType != ABPD_CONN_TYPE_NONE)
  {
    gprsContext_p->activePsdBearerContextWithDataConn = gprsGenericContext_p->cidUserData[cid];
  }

  /* test to see if PSD connection is permissible.
   */

  switch (connectionType)
  {
#if defined (FEA_PPP)
    case ABPD_CONN_TYPE_PPP:
    case ABPD_CONN_TYPE_CORE_PPP:
      connectionClass = PPP_CONNECTION;
      break;
#endif /* FEA_PPP */
    case ABPD_CONN_TYPE_PACKET_TRANSPORT:
      connectionClass = PT_CONNECTION;
      break;
    case ABPD_CONN_TYPE_NONE:
      /* For no connection type - when using AT+CGACT then connectionClass is
       * not valid and not used.  Set to something to shut lint up.
       */
      connectionClass = PT_CONNECTION;
      break;
    default:
      FatalParam (entity, cid, connectionType);
      break;
  }

#if defined (FEA_DEDICATED_BEARER)
  if (ptr->psdBearerInfo.secondaryContext == TRUE)
  {
    /* This is a secondary context / dedicate bearer - so we need to check if
     * the primary context / default bearer is active and has a connId - if
     * it doesn't then for NASMDL2 - we will activate the primary connection
     * first and then activate the secondary.  The primary must have been
     * defined already - AT+CGDSCONT takes care of this.
     *
     * For LTE, the channel we are on MUST be the same as that of primary context.
     * For LTE we cannot allow a secondary context connection with a
     * data connection (PPP or PT) but that will be checked with call to
     * vgOpManAllocateConnection later in this function as we will be on the same
     * channel as the primary context we are associated with.
     */
    primaryPtr = (gprsGenericContext_p->cidUserData[ptr->psdBearerInfo.primaryCid]);

    if ((primaryPtr->psdBearerInfo.secondaryContext == FALSE) &&
        (vgGpGetCidActiveStatus(ptr->psdBearerInfo.primaryCid) == TRUE)
        && (primaryPtr->psdBearerInfo.channelNumber == entity))
    {
      /* copy the relevent information in to the secondary context information
       */
      ptr->psdBearerInfo.primaryConnId = primaryPtr->psdBearerInfo.connId;
      ptr->psdBearerInfo.primaryPsdBearerId = primaryPtr->psdBearerInfo.psdBearerId;
    }
    else
    {
      result = VG_CME_OPERATION_NOT_ALLOWED;
    }
  }
#endif /* FEA_DEDICATED_BEARER */

  if (result == RESULT_CODE_OK)
  {
    if (ptr->isActive == TRUE)
    {
      /* We allow special case of activating a data connection already active
       * when it was previously activated with ABPD_CONN_TYPE_NONE and now
       * the user want to connect a data connection (i.e. Packet Transport
       * or PPP).  The signal sent to ABPD is SIG_APEX_ABPD_ACTIVATE_DATA_CONN_REQ
       * rather than SIG_APEX_ABPD_DIAL_REQ.
       */
      if ((connectionType != ABPD_CONN_TYPE_NONE) &&
           (ptr->psdBearerInfo.connType == ABPD_CONN_TYPE_NONE))
      {
        if (vgOpManAllocateConnection (entity, connectionClass) == TRUE)
        {
          /* reset last call release error */
          vgSetApexGsmCallReleaseError (APEX_CAUSE_OK, TRUE, CAUSE_NORMAL_CLEARING, PD_SM, entity);
          ptr->psdBearerInfo.connType = connectionType;
          gprsContext_p->vgDialCid = cid;
          result = vgChManContinueAction (entity, SIG_APEX_ABPD_ACTIVATE_DATA_CONN_REQ);
        }
        else
        {
          result = VG_CME_PHONE_LINK_RESERVED;
        }
      }
      /* Trying to activate the connection but with a different connection
       * type this time - so error case
       */
      else if (connectionType != ptr->psdBearerInfo.connType)
      {
        result = VG_CME_OPERATION_NOT_ALLOWED;
      }
      else
      {
        /* The context with this CID is already active so just say OK
         */
        result = RESULT_CODE_OK;
      }
    }
    /* We don't check if the connection is permitted for ABPD_CONN_TYPE_NONE
     * we only check for PT or PPP as activations with AT+CGACT=1 are always
     * permitted on a channel as long as the CID is not already active
     */
    else if ((connectionType == ABPD_CONN_TYPE_NONE) ||
             (vgOpManAllocateConnection (entity, connectionClass) == TRUE))
    {
      /* reset last call release error */
      vgSetApexGsmCallReleaseError (APEX_CAUSE_OK, TRUE, CAUSE_NORMAL_CLEARING, PD_SM, entity);
      ptr->psdBearerInfo.connType = connectionType;
      gprsContext_p->vgDialCid = cid;
      result = vgChManContinueAction (entity, SIG_APEX_ABPD_DIAL_REQ);

    }
    else
    {
      result = VG_CME_PHONE_LINK_RESERVED;
    }
  }

  return (result);
}

#if defined (FEA_MT_PDN_ACT)
/*--------------------------------------------------------------------------
 *
 * Function:    vgActivateMtContext
 *
 * Parameters:  cid    - context identifier whose parameters are to be used
 *                       in the context activation
 *              entity - mux channel number
 *              connectionType - Connection type -  can be PPP, Core PPP or
 *                               Packet Transport
 *
 * Returns:     ResultCode_t  - result after initiating context activation
 *
 * Description: Attempts to activate or reject a context described by 'cid', in
 * response to a request from the network
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgActivateMtContext (Int8 cid,
                                  const VgmuxChannelNumber entity,
                                  AbpdPdpConnType connectionType)
{
  ResultCode_t          result = RESULT_CODE_PROCEEDING;
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  GprsContext_t         *gprsContext_p = ptrToGprsContext(entity);
  VgPsdStatusInfo       *ptr;
  ConnectionClass_t     connectionClass;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(gprsContext_p != PNULL, entity, 0, 0);
#endif
  /* Point to context */
  ptr = gprsGenericContext_p->cidUserData[cid];
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (ptr!=PNULL, cid, entity, connectionType);
#endif
  /* Assign to activePsdBearerContextWithDataConn if PT or PPP mode */
  if (connectionType != ABPD_CONN_TYPE_NONE)
  {
    gprsContext_p->activePsdBearerContextWithDataConn =  gprsGenericContext_p->cidUserData[cid];
  }
  else
  {
    /* Cannot have a connection type of none for MT connections */
    FatalParam(cid, entity, connectionType);
  }

  /* save the connection type, connID and Channel Number for the step after next... */
  gprsGenericContext_p->vgAbpdSetupRsp.channelNumber    = entity;
  gprsGenericContext_p->vgAbpdSetupRsp.connType         = connectionType;
  gprsGenericContext_p->vgAbpdSetupRsp.connId           = ptr->psdBearerInfo.connId;

  /* Store the connection type in GPRS context data.
   * This will be checked when sending OpenDataConnReq to MUX. */
  ptr->psdBearerInfo.connType = connectionType;

  gprsGenericContext_p->vgAbpdSetupRsp.connectionAccepted = TRUE;

  switch (connectionType)
  {
#if defined (FEA_PPP)
    case ABPD_CONN_TYPE_PPP:
    case ABPD_CONN_TYPE_CORE_PPP:
      connectionClass = PPP_CONNECTION;
      break;
#endif /* FEA_PPP */
    case ABPD_CONN_TYPE_PACKET_TRANSPORT:
      connectionClass = PT_CONNECTION;
      break;
    default:
      FatalParam (entity, cid, connectionType);
      break;
  }

  if ( !ptr->isActive)
  {
    /* test to see if PSD connection is permissible.  All MT PDP Contexts
     * require a data connection as they are 2G/3G only
     */
    if (vgOpManAllocateConnection (entity, connectionClass))
    {
      result = vgChManContinueAction (entity, SIG_APEX_ABPD_SETUP_RSP);
    }
    else
    {
      result = VG_CME_PHONE_LINK_RESERVED;
    }
  }

  if (result != RESULT_CODE_PROCEEDING)
  {
    /* The answer failed - so reset the various data items */
    gprsContext_p->activePsdBearerContextWithDataConn       = PNULL;

    gprsGenericContext_p->vgAbpdSetupRsp.channelNumber      = VGMUX_CHANNEL_INVALID;
    gprsGenericContext_p->vgAbpdSetupRsp.connType           = ABPD_CONN_TYPE_NONE;
    gprsGenericContext_p->vgAbpdSetupRsp.connId             = INVALID_CONN_ID;
    gprsGenericContext_p->vgAbpdSetupRsp.connectionAccepted = FALSE;

    ptr->psdBearerInfo.connType                             = ABPD_CONN_TYPE_NONE;
  }
  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPdRespondToMtpca
*
* Parameters:  entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: accept mobile-terminated pdp context activation -- do
* the actual work
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPdRespondToMtpca (const VgmuxChannelNumber entity,
                                 Int32 cid,
                                 AbpdPdpConnType connectionType)
{
  ResultCode_t          result                = RESULT_CODE_OK;
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdBearerInfo       *psdBearerInfo_p     = PNULL;
  Boolean               profileWasDefined;

  /* When PSDMode is LTE and we are on 2G/3G - we cannot permit the MT activation
   * of a secondary context.
   * In LTE mode we don't permit any MT context activations - this should not
   * have happened anyway.
   */
  if (vgIsCurrentAccessTechnologyLte())
  {
    result = VG_CME_OPERATION_NOT_ALLOWED;
  }
  else if (vgAssignNewActiveContext ((Int8) cid, entity) == FALSE)
  {
    result = RESULT_CODE_ERROR;
  }
  else
  /* Connection is permitted - so continue */
  {
    /* copy the parameters received in the abgp-setup-ind into the
     * context.  This will then be used to make the AbpdDialReq request
     * to ABPD
     */
    FatalCheck (gprsGenericContext_p->cidUserData [cid],
              cid, gprsGenericContext_p->vgAbpdSetupInd.psdBearerId, 0);

    psdBearerInfo_p = &gprsGenericContext_p->cidUserData[cid]->psdBearerInfo;

    psdBearerInfo_p->connId = gprsGenericContext_p->vgAbpdSetupInd.connId;
    psdBearerInfo_p->psdBearerId = gprsGenericContext_p->vgAbpdSetupInd.psdBearerId;
    psdBearerInfo_p->pdnConnectionAddressInfo.pdnAddress = gprsGenericContext_p->vgAbpdSetupInd.pdnAddress;
    psdBearerInfo_p->negApnPresent = gprsGenericContext_p->vgAbpdSetupInd.apnPresent;
    psdBearerInfo_p->negApn = gprsGenericContext_p->vgAbpdSetupInd.apn;

    if (psdBearerInfo_p->negApnPresent == TRUE)
    {
      TextualAccessPointName convertedApn;

      /* AccessPointName: Convert the apn to textual
       * form so that we can query it later if required.
       */
      vgNetworkAPNToCiAPN (gprsGenericContext_p->vgAbpdSetupInd.apn, &convertedApn);
      memcpy (&psdBearerInfo_p->negTextualApn, &convertedApn, sizeof (AccessPointName));
    }
    else
    {
      psdBearerInfo_p->negApn.length = 0;
    }
    /* make sure that the AB task ABPD module takes some notice of the parameters
       set.  */
    /* Copy it first in case we need to undo it */
    profileWasDefined = gprsGenericContext_p->cidUserData[cid]->profileDefined;

    gprsGenericContext_p->cidUserData[cid]->profileDefined = TRUE;

    /* From here on it should behave similarly to the MO PDP context
     * activation.
     * Note that this whole procedure is 2G/3G specific - and cannot happen
     * for LTE
     */
    result = vgActivateMtContext ((Int8) cid, entity, connectionType);
    /* we no longer have an outstanding incoming request for pdp
       context activation */
    gprsGenericContext_p->incomingPdpContextActivation = FALSE;

    if (result != RESULT_CODE_PROCEEDING)
    {
      /* Something went wrong - so we need to reset associated data structures
       */
      psdBearerInfo_p->connId = INVALID_CONN_ID;
      psdBearerInfo_p->psdBearerId = PSD_BEARER_ID_UNASSIGNED;
      psdBearerInfo_p->pdnConnectionAddressInfo.pdnAddress.addressPresent = FALSE;
      psdBearerInfo_p->negApnPresent = FALSE;
      gprsGenericContext_p->cidUserData[cid]->profileDefined = profileWasDefined;
    }
  }
  return (result);
}
#endif /* FEA_MT_PDN_ACT */

/*--------------------------------------------------------------------------
*
* Function:     vgPDNTypeToIndx
*
* Parameters:   pdnType       - pdn type to find
*               arrayIndex    - pdn type info table record number
*
* Returns:      Boolean       - whether pdn type information was found
*
* Description:  Looks through supported pdn types to find match to
*               pdn type parameter and sets the record index if found.
*
*-------------------------------------------------------------------------*/

Boolean vgPDNTypeToIndx (PdnType pdnType,
                          Int8 *arrayIndex)
{
  const SupportedPDNTypesMap *map = supportedPDNTypesMap;
  Boolean                    entryFound = FALSE;

  if (map->arrIndx != UCHAR_MAX)
  {
    /* go through each pdn type comparing with specified type number */
    do
    {
      if (map->PDNType == pdnType)
      {
        *arrayIndex = map->arrIndx;
        entryFound = TRUE;
      }
      map++;
    }
    while ((map->arrIndx != UCHAR_MAX) && (entryFound == FALSE));
  }

  return (entryFound);
}

/*--------------------------------------------------------------------------
 *
 * Function:    getSupportedPDNTypesMap
 *
 * Parameters:  void
 * Returns:     SupportedPDPTypesMap    - result of function
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/

const SupportedPDNTypesMap *getSupportedPDNTypesMap (void)
{
  return (supportedPDNTypesMap);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgDoesEntityHaveSeparateDataChannel
 *
 * Parameters:  entity                  - Entity to check
 *
 * Returns:     TRUE if the entity currently has a separate data channel.
 *              This currently always returns FALSE as MUX switches between
 *              AT command and data mode and does not have separate command
 *              and data channels for NB-IOT solution.
 *
 *              This replaces some calls that used to be to
 *              vgIsEntityInPacketTransportMode which used to check this.
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/
Boolean vgDoesEntityHaveSeparateDataChannel    (const VgmuxChannelNumber entity)
{
   return (FALSE);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgIsEntityInPacketTransportMode
 *
 * Parameters:  entity                  - Entity to check
 *
 * Returns:     TRUE if the entity is currently in a packet transport mode
 *              connection.
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/
Boolean vgIsEntityInPacketTransportMode    (const VgmuxChannelNumber entity)
{
  GprsContext_t         *gprsContext_p = ptrToGprsContext(entity);
  Boolean               retVal         = FALSE;

  if ((gprsContext_p != PNULL) &&
      (gprsContext_p->activePsdBearerContextWithDataConn != PNULL) &&
      (gprsContext_p->activePsdBearerContextWithDataConn->psdBearerInfo.connType
                                          == ABPD_CONN_TYPE_PACKET_TRANSPORT))
  {
    /* This channel is in a packet transport connection so return TRUE */
    retVal = TRUE;
  }

  return (retVal);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgFindEntityLinkedToCid
 *
 * Parameters:  entity                  - Entity to check
 *
 * Returns:     Entity number which owns this cid or VGMUX_CHANNEL_INVALID if not
 *              found.
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/
VgmuxChannelNumber vgFindEntityLinkedToCid (Int8 cid)
{
  GprsGenericContext_t        *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgmuxChannelNumber          entity                = VGMUX_CHANNEL_INVALID;

  FatalAssert (gprsGenericContext_p != PNULL);

  entity = gprsGenericContext_p->cidOwner[cid];

  FatalCheck (isEntityActive(entity), cid, entity, 0);

  return (entity);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgFindEntityLinkedToCidWithoutCheckEntity
 *
 * Parameters:  entity                  - Entity to check
 *
 * Returns:     Entity number which owns this cid or VGMUX_CHANNEL_INVALID if not
 *              found.
 *
 * Description:Find the entity linked to CID
 *                 And if find invalid entity, do not assert
 *
 *-------------------------------------------------------------------------*/
VgmuxChannelNumber vgFindEntityLinkedToCidWithoutCheckEntity (Int8 cid)
{
  GprsGenericContext_t        *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgmuxChannelNumber          entity                = VGMUX_CHANNEL_INVALID;

  FatalAssert (gprsGenericContext_p != PNULL);

  entity = gprsGenericContext_p->cidOwner[cid];

  return (entity);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgFindCidLinkedToConnId
 *
 * Parameters:  connId                  - connId to check
 *
 * Returns:     CID for PSD Bearer context which is associated with the connId
 *              or CID_NUMBER_UNKNOWN if not found.
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/
Int8 vgFindCidLinkedToConnId (Int8 connId)
{
  GprsGenericContext_t        *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo             *cidUserData_p;
  Int8                        cid                   = CID_NUMBER_UNKNOWN;
  Int8                        loopCid;

  FatalAssert (gprsGenericContext_p != PNULL);

  for (loopCid = 0; loopCid < MAX_NUMBER_OF_CIDS; loopCid++)
  {
    cidUserData_p = gprsGenericContext_p->cidUserData[loopCid];
    if (cidUserData_p != PNULL)
    {
      if (cidUserData_p->psdBearerInfo.connId == connId)
      {
        cid = loopCid;
      }
    }
  }

  return (cid);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgDeactivateNextContextOwnedByEntity
 *
 * Parameters:  entity                  - entity to check
 *
 * Returns:     Boolean if we found another context which is active associated
 *              with this entity and triggered a deactivation.
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/
Boolean vgDeactivateNextContextOwnedByEntity (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t     *gprsGenericContext_p  = ptrToGprsGenericContext ();
  GprsContext_t            *gprsContext_p         = ptrToGprsContext (entity);
  OpmanContext_t           *opManContext_p        = ptrToOpManContext (entity);
  Int8                     cidCount, cid          = CID_NUMBER_UNKNOWN;
  Boolean                  cidFound               = FALSE;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck  (opManContext_p != PNULL, entity, 0, 0);
  FatalAssert (gprsGenericContext_p != PNULL);
  FatalAssert (gprsContext_p != PNULL);
#endif
  for (cidCount = vgGpGetMinCidValue(entity); (cidCount < MAX_NUMBER_OF_CIDS) && (cid == CID_NUMBER_UNKNOWN); cidCount++)
  {
    if ((gprsGenericContext_p->cidOwner[cidCount] == entity) &&
        (vgOpManCidActive(cidCount) == TRUE))
    {
      /* Note that we need to handle secondary contexts for 2G/3G support.  However, for
       * LTE once we disconnect a primary context - then all the secondaries associated with it
       * will also be disconnected
       */
      cid = cidCount;
    }
  }

  /* We found another CID associated with this channel - so we need to disconnect it
   */
  if (cid != CID_NUMBER_UNKNOWN)
  {
    cidFound = TRUE;

    if ((gprsGenericContext_p->cidUserData[cid] != PNULL) &&
        (gprsGenericContext_p->cidUserData[cid]->psdBearerInfo.connType != ABPD_CONN_TYPE_NONE))
    {
      /* Disconnect the data session for PT or PPP connnections only.  We look for the
       * PT or PPP connection linked to this entity - as it MUST be this connection as you can
       * only have one of these types per channel
       */
      if (opManContext_p->numberOfCallConnections > 0)
      {
        switch (opManContext_p->callInfo.vgClass)
        {
#if defined (FEA_PPP)
          case PPP_CONNECTION:
#endif /* FEA_PPP */
          case PT_CONNECTION:
          {
            switch (opManContext_p->callInfo.vgState)
            {
              case CONNECTION_DIALLING:
              case CONNECTION_CONNECTING:
              case CONNECTION_ON_LINE:
              {
                vgOpManSetConnectionStatus (entity,
                                             opManContext_p->callInfo.vgIdent,
                                              CONNECTION_DISCONNECTING);

                terminateDataSession (entity, opManContext_p->callInfo.vgClass);
                break;
              }
              default:
              {
                /* already disconnecting or offline */
                break;
              }
            }
          }
          default:
          {
            /* Don't do anything as it is a voice or CSD call so leave it alone here. */
            break;
          }
        }
      }
    }

    /* Now disconnect the PSD connection */
    gprsContext_p->vgHangupType = VG_HANGUP_TYPE_ATH;
    gprsContext_p->vgHangupCid  = cid;

    vgChManContinueAction (entity, SIG_APEX_ABPD_HANGUP_REQ);
  }
  return (cidFound);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgFindCidWithDataConnLinkedToEntity
 *
 * Parameters:  entity                  - entity to check
 *
 * Returns:     The CID linked to the entity which is in Packet Transport or
 *              PPP mode.  There will be only one of them.
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/
Int8 vgFindCidWithDataConnLinkedToEntity (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t     *gprsGenericContext_p  = ptrToGprsGenericContext ();
  Int8                     cid                    = CID_NUMBER_UNKNOWN;
  Int8                     cidCount;

  FatalAssert (gprsGenericContext_p != PNULL);

  for (cidCount = vgGpGetMinCidValue(entity); (cidCount < MAX_NUMBER_OF_CIDS) && (cid == CID_NUMBER_UNKNOWN); cidCount++)
  {
    if ((gprsGenericContext_p->cidOwner[cidCount] == entity) &&
        (gprsGenericContext_p->cidUserData[cidCount] != PNULL) &&
        (gprsGenericContext_p->cidUserData[cidCount]->psdBearerInfo.connType != ABPD_CONN_TYPE_NONE))
    {
      /* We found a CID which is associated with this entity and it is set for PT or PPP mode
       */
      cid = cidCount;
    }
  }
  return (cid);
}

#if defined (FEA_DEDICATED_BEARER)
/*--------------------------------------------------------------------------
 *
 * Function:    vgFindCidOfNextActiveSecondaryContextLinkedToPrimaryCid
 *
 * Parameters:  cid            CID of primary context
 *
 * Returns:     The next secondary context CID linked to the primary
 *              context.  If nothing is found then returns CID_NUMBER_UNKNOWN.
 *
 * Description: See above.
 *
 *-------------------------------------------------------------------------*/
Int8 vgFindCidOfNextActiveSecondaryContextLinkedToPrimaryCid (Int8 cid)
{
  GprsGenericContext_t     *gprsGenericContext_p  = ptrToGprsGenericContext ();
  Int8                     secondaryCid           = CID_NUMBER_UNKNOWN;
  Int8                     cidCount;

  FatalAssert (gprsGenericContext_p != PNULL);

  for (cidCount = MIN_CID_VALUE_CID1; (cidCount < MAX_NUMBER_OF_CIDS) && (secondaryCid == CID_NUMBER_UNKNOWN); cidCount++)
  {
    if ((gprsGenericContext_p->cidUserData[cidCount] != PNULL) &&
        (gprsGenericContext_p->cidUserData[cidCount]->isActive) &&
        (gprsGenericContext_p->cidUserData[cidCount]->psdBearerInfo.secondaryContext) &&
        (gprsGenericContext_p->cidUserData[cidCount]->psdBearerInfo.primaryCid == cid))
    {
      /* We found a secondary CID which is associated with the primary CID and it is still
       * active.
       */
      secondaryCid = cidCount;
    }
  }
  return (secondaryCid);
}
#endif /* FEA_DEDICATED_BEARER */

#if defined (ATCI_SLIM_DISABLE)
/*--------------------------------------------------------------------------
*
* Function:     vgCheckCidAvailable
*
* Parameters:   cid    - context id
*               entity - mux channel number
*
* Returns:      Boolean - whether cid can be accessed
*
* Description:  Checks for which entity can modify the CID
*               packet counter.
*
*-------------------------------------------------------------------------*/
static Boolean vgCheckCidAvailable (Int8 cid,
                             const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  Boolean               result = FALSE;

  if ((gprsGenericContext_p->cidUserData[cid]->reportEntity == entity) ||
      (gprsGenericContext_p->cidUserData[cid]->reportEntity == VGMUX_CHANNEL_INVALID))
  {
    result = TRUE;
  }
  return (result);
}
#endif

/*--------------------------------------------------------------------------
*
* Function:     vgGpGetCidActiveStatus
*
* Parameters:   cid    - context id
*
* Returns:      Boolean - whether cid has been activated
*
* Description:  Checks for if the cid has been activated.
*
*-------------------------------------------------------------------------*/
static Boolean vgGpGetCidActiveStatus (Int8 cid)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  Boolean               result = FALSE;

  if(gprsGenericContext_p->cidUserData[cid] != PNULL)
  {
    result = gprsGenericContext_p->cidUserData[cid]->isActive;
  }

  return (result);
}

/******************************************
 * Static functions                                                *
 ******************************************/

/*--------------------------------------------------------------------------
*
* Function:    vgDisplayCGCONTRDPBasicInfo
*
* Parameters:  entity          - mux channel number
*              cid             - CID to display
*
* Returns:     Nothing
*
* Description: Displays initial list of attributes excluding any IP addresses
*              for AT*CGCONTRDP command.
*
*-------------------------------------------------------------------------*/
static void vgDisplayCGCONTRDPBasicInfo(const VgmuxChannelNumber entity, Int8 cid)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo       *cmd_p = PNULL;
  Int8                  apnIndex;

  cmd_p = gprsGenericContext_p->cidUserData [cid];

  FatalAssert ((cmd_p != PNULL) &&
            (cmd_p->profileDefined == TRUE) &&
            (cmd_p->isActive == TRUE));

  vgPrintf (entity,
          (const Char*)"+CGCONTRDP: %d,%d",
           cid,
           cmd_p->psdBearerInfo.psdBearerId);

  vgPrintf (entity, (const Char*)",\"");


  /* display APN if present */
  if (cmd_p->psdBearerInfo.negApnPresent == TRUE)
  {
    for (apnIndex = 0;
          apnIndex < cmd_p->psdBearerInfo.negTextualApn.length;
           apnIndex++)
    {
      vgPutc (entity, cmd_p->psdBearerInfo.negTextualApn.name[apnIndex]);
    }
  }
  vgPrintf (entity, (const Char*)"\"");
}

/*--------------------------------------------------------------------------
*
* Function:    vgDisplayCGCONTRDPAddrInfo
*
* Parameters:  entity          - mux channel number
*              cid             - CID to display
*              pdnType         - PDN Type to display (IPV4 or IPV6)
*
* Returns:     Nothing
*
* Description: Displays the IP Address information for CGCONTRDP command.
*              Can be either for IPV4 or IPV6.
*
*-------------------------------------------------------------------------*/
static void vgDisplayCGCONTRDPAddrInfo (const VgmuxChannelNumber entity, Int8 cid, PdnType pdnType)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo       *cmd_p = PNULL;
  Boolean               pdnAddressPresent;
  Boolean               subnetMaskPresent;
  Boolean               gatewayAddressPresent;
  Boolean               primaryDnsAddressPresent;
  Boolean               secondaryDnsAddressPresent;
  Boolean               plmnRateControlPresent;
  Boolean               ipv4MtuSizePresent;
  Boolean               nonIPMtuSizePresent;
  TextualPdnAddress     convertedPdnAddress;

  cmd_p = gprsGenericContext_p->cidUserData [cid];

  FatalAssert ((cmd_p != PNULL) &&
            (cmd_p->profileDefined == TRUE) &&
            (cmd_p->isActive == TRUE));

  FatalCheck ((pdnType == PDN_TYPE_IPV4) || (pdnType == PDN_TYPE_IPV6) || (pdnType == PDN_TYPE_NONIP), entity, cid, pdnType);
  pdnAddressPresent = cmd_p->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress.addressPresent;
  subnetMaskPresent = cmd_p->psdBearerInfo.pdnConnectionAddressInfo.subnetMask.addressPresent;
  gatewayAddressPresent = cmd_p->psdBearerInfo.pdnConnectionAddressInfo.gatewayAddress.addressPresent;
  primaryDnsAddressPresent = cmd_p->psdBearerInfo.pdnConnectionAddressInfo.primaryDnsAddress.addressPresent;
  secondaryDnsAddressPresent = cmd_p->psdBearerInfo.pdnConnectionAddressInfo.secondaryDnsAddress.addressPresent;
  plmnRateControlPresent = gprsGenericContext_p->plmnRateControlInfo.plmnRateControlPresent;
  ipv4MtuSizePresent = cmd_p->psdBearerInfo.ipv4MtuSizePresent;
  nonIPMtuSizePresent = cmd_p->psdBearerInfo.nonIPMtuSizePresent;

  if ((pdnAddressPresent == TRUE) ||
      (subnetMaskPresent == TRUE) ||
      (gatewayAddressPresent == TRUE) ||
      (primaryDnsAddressPresent == TRUE) ||
      (secondaryDnsAddressPresent == TRUE) ||
      (plmnRateControlPresent == TRUE) ||
      (ipv4MtuSizePresent == TRUE) ||
      (nonIPMtuSizePresent == TRUE))

  {
    vgPrintf (entity, (const Char*)",");

    if (pdnAddressPresent || subnetMaskPresent)
    {
      /* How this information is displayed depends on the setting of AT+CGPIAF */

      /* IP Address */
      if (pdnAddressPresent)
      {
        vgPDNAddrIntToDisplayStr (pdnType, cmd_p->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress, &convertedPdnAddress, entity);
        vgPrintf (entity, (const Char*)"\"%.*s",
                  convertedPdnAddress.length, convertedPdnAddress.address);
      }
      else
      {
        vgPrintf (entity, (const Char*)"\"%s", vgGetDefaultSourceAddress(pdnType));
      }
      /* Subnet Mask */
      if (subnetMaskPresent)
      {
        vgPDNAddrIntToDisplayStr (pdnType, cmd_p->psdBearerInfo.pdnConnectionAddressInfo.subnetMask, &convertedPdnAddress, entity);
        vgPrintf (entity, (const Char*)".%.*s\"",
                  convertedPdnAddress.length, convertedPdnAddress.address);
      }
      else
      {
        vgPrintf (entity, (const Char*)".%s\"", vgGetDefaultSubnetMask(pdnType));
      }
    }
  }

  if ((gatewayAddressPresent == TRUE) ||
      (primaryDnsAddressPresent == TRUE) ||
      (secondaryDnsAddressPresent == TRUE) ||
      (plmnRateControlPresent == TRUE) ||
      (ipv4MtuSizePresent == TRUE) ||
      (nonIPMtuSizePresent == TRUE))

  {
    vgPrintf (entity, (const Char*)",");

    if (gatewayAddressPresent)
    {
      /* Gateway Address */
      viewPDNAddrGeneric(pdnType,
                         &cmd_p->psdBearerInfo.pdnConnectionAddressInfo.gatewayAddress,
                         entity);
    }
  }

  if ((primaryDnsAddressPresent == TRUE) ||
      (secondaryDnsAddressPresent == TRUE) ||
      (plmnRateControlPresent == TRUE) ||
      (ipv4MtuSizePresent == TRUE) ||
      (nonIPMtuSizePresent == TRUE))
  {
    vgPrintf (entity, (const Char*)",");

    if (primaryDnsAddressPresent)
    {
      /* Primary DNS Address */
      viewPDNAddrGeneric(pdnType,
                         &cmd_p->psdBearerInfo.pdnConnectionAddressInfo.primaryDnsAddress,
                         entity);
    }
  }

  if ((secondaryDnsAddressPresent == TRUE) ||
      (plmnRateControlPresent == TRUE) ||
      (ipv4MtuSizePresent == TRUE) ||
      (nonIPMtuSizePresent == TRUE))
  {
    vgPrintf (entity, (const Char*)",");
    if (secondaryDnsAddressPresent)
    {
      /* Secondary DNS Address */
      viewPDNAddrGeneric(pdnType,
                         &cmd_p->psdBearerInfo.pdnConnectionAddressInfo.secondaryDnsAddress,
                         entity);
    }
  }
}

/*--------------------------------------------------------------------------
*
* Function:    vgDisplayMtuSizePlmnRateControlInfo
*
* Parameters:  entity          - mux channel number
*
* Returns:     Nothing
*
* Description: Displays the PLMN Rate Contrl information for NB-IOT
*
*-------------------------------------------------------------------------*/
static void vgDisplayMtuSizePlmnRateControlInfo (const VgmuxChannelNumber entity, Int8 cid)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo       *cmd_p = PNULL;

  cmd_p = gprsGenericContext_p->cidUserData [cid];

  FatalAssert ((cmd_p != PNULL) &&
            (cmd_p->profileDefined == TRUE) &&
            (cmd_p->isActive == TRUE));

  if (cmd_p->psdBearerInfo.ipv4MtuSizePresent)
  {
    vgPrintf (entity, (const Char*)",,,,,%d",cmd_p->psdBearerInfo.ipv4LinkMTU);

    if (cmd_p->psdBearerInfo.nonIPMtuSizePresent)
    {
      vgPrintf (entity, (const Char*)",,,%d",cmd_p->psdBearerInfo.nonIPLinkMTU);
    }
    else if (gprsGenericContext_p->plmnRateControlInfo.plmnRateControlPresent)
    {
      vgPrintf (entity, (const Char*)",,,");
    }
  }
  else if (cmd_p->psdBearerInfo.nonIPMtuSizePresent)
  {
    vgPrintf (entity, (const Char*)",,,,,,,,%d",cmd_p->psdBearerInfo.nonIPLinkMTU);
  }
  else if (gprsGenericContext_p->plmnRateControlInfo.plmnRateControlPresent)
  {
    vgPrintf (entity, (const Char*)",,,,,,,,");
  }

  if (gprsGenericContext_p->plmnRateControlInfo.plmnRateControlPresent)
  {
    vgPrintf (entity, (const Char*)",%d",gprsGenericContext_p->plmnRateControlInfo.plmnRateControlValue);
  }
}

/*--------------------------------------------------------------------------
*
* Function:    vgDisplayCGCONTRDPInfo
*
* Parameters:  entity          - mux channel number
*              cid             - CID to display
*
* Returns:     Nothing
*
* Description: Displays number of attributes which have been assigned to a
*              CID when the context was activated, such as PSD Bearer ID
*              (EPS Bearer ID/NSAPI), APN, IP address, DNS addresses.
*              The function assumes the CID is valid. If not then
*              it will assert.
*
*-------------------------------------------------------------------------*/
static void vgDisplayCGCONTRDPInfo (const VgmuxChannelNumber entity, Int8 cid)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo       *cmd_p = PNULL;
  PdnType               pdnType;

  cmd_p = gprsGenericContext_p->cidUserData [cid];

  FatalAssert ((cmd_p != PNULL) &&
            (cmd_p->profileDefined == TRUE) &&
            (cmd_p->isActive == TRUE));

  pdnType = cmd_p->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress.pdnType;

  /* Display IPV4 address */
  if ((PDN_TYPE_IPV4 == pdnType) || (PDN_TYPE_IPV6 == pdnType) || (PDN_TYPE_NONIP == pdnType))
  {
    /* Display the start string */
    vgDisplayCGCONTRDPBasicInfo(entity, cid);

    /* Now display all the IP address info */
    vgDisplayCGCONTRDPAddrInfo(entity, cid, pdnType);

    /* Display PLMN rate control info for NB-IOT - if present */
    vgDisplayMtuSizePlmnRateControlInfo (entity, cid);

    vgPutNewLine (entity);
  }
  else
  if (PDN_TYPE_IPV4V6 == pdnType)
  {
    vgDisplayCGCONTRDPBasicInfo(entity, cid);
    vgDisplayCGCONTRDPAddrInfo(entity, cid, PDN_TYPE_IPV4);
    vgDisplayMtuSizePlmnRateControlInfo (entity, cid);
    vgPutNewLine (entity);

    vgDisplayCGCONTRDPBasicInfo(entity, cid);
    vgDisplayCGCONTRDPAddrInfo(entity, cid, PDN_TYPE_IPV6);
    vgDisplayMtuSizePlmnRateControlInfo (entity, cid);
    vgPutNewLine (entity);
  }

}

/*--------------------------------------------------------------------------
*
* Function:    vgDisplayAllCGCONTRDPInfo
*
* Parameters:  entity          - mux channel number
*              cid             - CID to display
*
* Returns:     Nothing
*
* Description: Displays number of attributes which have been assigned to
*              all active CIDs when the contexts were activated, such as PSD
*              Bearer ID (EPS Bearer ID/NSAPI), APN, IP address, DNS addresses.
*
*-------------------------------------------------------------------------*/
static void vgDisplayAllCGCONTRDPInfo (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo       *cmd_p = PNULL;
  Int32                 cid;
  Boolean               firstTime = TRUE;

  for (cid = vgGpGetMinCidValue(entity); cid < MAX_NUMBER_OF_CIDS; cid++)
  {
    if (gprsGenericContext_p->cidUserData [cid] != PNULL)
    {
      cmd_p = gprsGenericContext_p->cidUserData [cid];

      /* if the profile is defined and active, and it is not secondary,
       * display cid info */
      if ((cmd_p->profileDefined) && (cmd_p->isActive)
#if defined (FEA_DEDICATED_BEARER)
          && !(cmd_p->psdBearerInfo.secondaryContext)
#endif /* FEA_DEDICATED_BEARER */
          )
      {
        if (firstTime)
        {
          firstTime = FALSE;
          vgPutNewLine (entity);
        }

        vgDisplayCGCONTRDPInfo(entity, (Int8)cid);
      }
    }
  }
}

#if defined (FEA_DEDICATED_BEARER)
/*--------------------------------------------------------------------------
*
* Function:    vgDisplayCGSCONTRDPInfo
*
* Parameters:  entity          - mux channel number
*
* Returns:     Nothing
*
* Description: Displays number of attributes which have been assigned to a
*              secondary CID when the context was activated, such as PSD Bearer
*              ID, primary and secondary CID values.
*
*-------------------------------------------------------------------------*/
static void vgDisplayCGSCONTRDPInfo (const VgmuxChannelNumber entity, Int8 cid)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo       *cmd_p = PNULL;

  cmd_p = gprsGenericContext_p->cidUserData [cid];

  FatalAssert ((cmd_p != PNULL) &&
            (cmd_p->profileDefined == TRUE) &&
            (cmd_p->isActive == TRUE) &&
            (cmd_p->psdBearerInfo.secondaryContext == TRUE));

  vgPrintf (entity,
          (const Char*)"+CGSCONTRDP: %d,%d,%d",
           cid,
           cmd_p->psdBearerInfo.primaryCid,
           cmd_p->psdBearerInfo.psdBearerId);

  vgPutNewLine (entity);
}

/*--------------------------------------------------------------------------
*
* Function:    vgDisplayAllCGSCONTRDPInfo
*
* Parameters:  entity          - mux channel number
*
* Returns:     Nothing
*
* Description: Displays number of attributes which have been assigned to
*              all active secondary CID when the contexts were activated, such
*              as PSD Bearer ID, primary and secondary CID values.
*
*-------------------------------------------------------------------------*/
static void vgDisplayAllCGSCONTRDPInfo (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo       *cmd_p = PNULL;
  Int32                 cid;
  Boolean               firstTime = TRUE;

  for (cid = vgGpGetMinCidValue(entity); cid < MAX_NUMBER_OF_CIDS; cid++)
  {
    if (gprsGenericContext_p->cidUserData [cid] != PNULL)
    {
      cmd_p = gprsGenericContext_p->cidUserData [cid];

      /* if the profile is defined and active, and it is secondary,
       * display cid info */
      if ((cmd_p->profileDefined) && (cmd_p->isActive) &&
          (cmd_p->psdBearerInfo.secondaryContext))

      {
        if (firstTime)
        {
          firstTime = FALSE;
          vgPutNewLine (entity);
        }
        vgDisplayCGSCONTRDPInfo(entity, (Int8)cid);
      }
    }
  }
}
#endif /* FEA_DEDICATED_BEARER */

#if defined(FEA_QOS_TFT)
/*--------------------------------------------------------------------------
*
* Function:    vgDisplayCGTFTRDPInfo
*
* Parameters:  entity          - mux channel number
*              cid             - CID to display
*
* Returns:     Nothing
*
* Description: Displays traffic flow template parameters which have been
*              assigned to a CID when the context was activated. The
*              function assumes the CID is valid. If not then it will
*              assert.
*
*-------------------------------------------------------------------------*/
static void vgDisplayCGTFTRDPInfo (const VgmuxChannelNumber entity, Int8 cid)
{
    GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
    VgPsdStatusInfo       *cmd_p                = PNULL;
    TrafficFlowTemplate   *tft_p                = PNULL;
    PacketFilter          *tempPf_p             = PNULL;
    Int8                  pfIndex;

    cmd_p = gprsGenericContext_p->cidUserData [cid];

    FatalAssert ((cmd_p != PNULL) &&
                 (cmd_p->profileDefined == TRUE) &&
                 (cmd_p->isActive == TRUE));

    vgPrintf (entity, (const Char*)"+CGTFTRDP: %d", cid);
    if(TRUE == cmd_p->psdBearerInfo.negTftPresent)
    {
      KiAllocMemory (sizeof(PacketFilter), (void **)&tempPf_p);
      tft_p = &(cmd_p->psdBearerInfo.negotiatedTft);
      for (pfIndex = 0; pfIndex < MAX_PFS_IN_TFT; pfIndex++)
      {
        *tempPf_p = tft_p->packetFilterData[pfIndex];
        if (tempPf_p->packetFilterId != 0)
        {
          vgPrintf (entity, (const Char*)",%d,%d,",
                    tempPf_p->packetFilterId,
                    tempPf_p->evalPrecedenceIndex);

          if (tempPf_p->remoteAddrSubnetMask.present)
          {
            /* Convert the addresses to text first */
            if (vgConvertNumericRemoteAddressSubnetMaskToTextual (&tempPf_p->remoteAddrSubnetMask, entity))
            {
              vgPrintf (entity, (const Char*)"\"%s\"", tempPf_p->remoteAddrSubnetMask.val);
            }
          }
          vgPutc (entity, ',');
          if (tempPf_p->protocolNumNextHdr.present)
          {
            vgPrintf (entity, (const Char*)"%d", tempPf_p->protocolNumNextHdr.val);
          }
          vgPutc (entity, ',');

          if (tempPf_p->localPortRange.present)
          {
            if (vgConvertNumericPortRangeToTextual (&tempPf_p->localPortRange))
            {
              vgPrintf (entity, (const Char*)"\"%s\"", tempPf_p->localPortRange.val);
            }
          }
          vgPutc (entity, ',');

          if (tempPf_p->remotePortRange.present)
          {
            if (vgConvertNumericPortRangeToTextual (&tempPf_p->remotePortRange))
            {
              vgPrintf (entity, (const Char*)"\"%s\"", tempPf_p->remotePortRange.val);
            }
          }
          vgPutc (entity, ',');

          if (tempPf_p->ipsecSpi.present)
          {
            vgPrintf (entity, (const Char*)"%lX", tempPf_p->ipsecSpi.val);
          }
          vgPutc (entity, ',');

          if (tempPf_p->tosTrfcClass.present)
          {
            if (vgConvertNumericTosTrfcClassToTextual (&tempPf_p->tosTrfcClass))
            {
              vgPrintf (entity, (const Char*)"\"%s\"", tempPf_p->tosTrfcClass.val);
            }
          }
          vgPutc (entity, ',');

          if (tempPf_p->flowLabel.present)
          {
            vgPrintf (entity, (const Char*)"%X", tempPf_p->flowLabel.val);
          }

          /* the coding scheme for AT command and NAS signalling is not the same */

          if (PF_DIR_DOWNLINK_ONLY == tempPf_p->packetFilterDirection)
          {
            vgPrintf (entity, (const Char*)",2");
          }
          else if (PF_DIR_UPLINK_ONLY == tempPf_p->packetFilterDirection)
          {
            vgPrintf (entity, (const Char*)",1");
          }
          else
          {

            vgPrintf (entity, (const Char*)",%d", tempPf_p->packetFilterDirection);
          }
          vgPutNewLine (entity);
          /* Free the memory for temporary packet filter */
        }
      }
      KiFreeMemory ((void **)&tempPf_p);
    }
}

/*--------------------------------------------------------------------------
*
* Function:    vgDisplayAllCGTFTRDPInfo
*
* Parameters:  entity          - mux channel number
*              cid             - CID to display
*
* Returns:     Nothing
*
* Description: Displays traffic flow template parameters which have been assigned to
*              all active CIDs when the contexts were activated.
*-------------------------------------------------------------------------*/
static void vgDisplayAllCGTFTRDPInfo (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p   = ptrToGprsGenericContext ();
  VgPsdStatusInfo       *cmd_p                  = PNULL;
  Int32                 cid;
  Boolean               firstTime               = TRUE;

  for (cid = vgGpGetMinCidValue(entity); cid < MAX_NUMBER_OF_CIDS; cid++)
  {
    if (gprsGenericContext_p->cidUserData [cid] != PNULL)
    {
      cmd_p = gprsGenericContext_p->cidUserData [cid];

      /* if the profile is active, display cid info */
      if (cmd_p->isActive && cmd_p->profileDefined)
      {
        if (firstTime)
        {
          vgPutNewLine (entity);
          firstTime = FALSE;
        }
        vgDisplayCGTFTRDPInfo(entity, (Int8)cid);
      }
    }
  }
}
#endif /* FEA_QOS_TFT */

/*--------------------------------------------------------------------------
*
* Function:    vgDisplayUsernamePasswordInfo
*
* Parameters:  entity          - mux channel number
*              cid             - CID to display
*              firstTime       - If the function was called multiple times then
*                                this is the first time it was called
*
* Returns:     TRUE if something was displayed
*
* Description: Displays username and password for PDN Connection which
*              have been assigned to a primary CID when the context was
*              defined using AT+CGDCONT. The function assumes the CID is
*              valid. If not then it will assert.
*-------------------------------------------------------------------------*/
static Boolean vgDisplayUsernamePasswordInfo (const VgmuxChannelNumber entity, Int8 cid,
                                              Boolean firstTime)
{
    GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
    VgPsdStatusInfo       *cmd_p                = PNULL;
    Boolean               textDisplayed         = FALSE;

    cmd_p = gprsGenericContext_p->cidUserData [cid];

    FatalAssert (cmd_p != PNULL);

    if ((cmd_p->profileDefined) &&
#if defined (FEA_DEDICATED_BEARER)
        (((cmd_p->cgdcontDefined) &&
          (!cmd_p->psdBearerInfo.secondaryContext)) ||
         ((cmd_p->cgdscontDefined) &&
          (cmd_p->psdBearerInfo.secondaryContext))))
#else
        (cmd_p->cgdcontDefined))
#endif

    {
        /* We only display something if there is a username present
         * otherwise display nothing
         */

        if (cmd_p->psdBearerInfo.psdUser.usernamePresent)
        {
             textDisplayed = TRUE;

             /* If this was the first time a line of text was displayed then
              * output a newline
              */
             if (firstTime)
             {
                 vgPutNewLine (entity);
             }

             /* Now print the string - it is slightly different depending on the
              * AT command
              */
             vgPrintf (entity, (const Char*)"%C: %d", cid);

             if (getCommandId(entity) == VG_AT_GP_CGAUTH)
             {
                 /* If the username is defined then we only ever used PAP
                  * so just hard coded here
                  */
                 vgPrintf (entity, (const Char*)",%d", (Int8) PPP_AUTH_PAP);
             }
             vgPrintf (entity, (const Char*)",\"%s\"",
                       cmd_p->psdBearerInfo.psdUser.username);

             if (cmd_p->psdBearerInfo.psdUser.passwdPresent)
             {
                 vgPrintf (entity, (const Char*)",\"%s\"",
                           cmd_p->psdBearerInfo.psdUser.passwd);
             }
             vgPutNewLine (entity);
        }
    }

    return (textDisplayed);
}

/*--------------------------------------------------------------------------
*
* Function:    vgDisplayAllUsernamePasswordInfo
*
* Parameters:  entity          - mux channel number
*              cid             - CID to display
*
* Returns:     Nothing
*
* Description: Displays username and password for PDN Connection which
*              have been assigned to all primary CIDs when the context
*              was defined using AT+CGDCONT.
*
*-------------------------------------------------------------------------*/
static void vgDisplayAllUsernamePasswordInfo (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p   = ptrToGprsGenericContext ();
  Int32                 cid;
  Boolean               firstTime               = TRUE;

  for (cid = vgGpGetMinCidValue(entity); cid < MAX_NUMBER_OF_CIDS; cid++)
  {
    if (gprsGenericContext_p->cidUserData [cid] != PNULL)
    {
      /* If we displayed something then we need to clear the firstTime flag */
      if (vgDisplayUsernamePasswordInfo(entity, (Int8)cid, firstTime) && firstTime)
      {
        firstTime = FALSE;
      }
    }
  }
}

/*--------------------------------------------------------------------------
*
* Function:    vgDeleteNonActiveContext
*
* Parameters:  entity          - mux channel number
*              cid             - CID to delete
*              firstTime       - Indicates the first time this has been
*                                called as part of the AT+CGDEL command
*                                execution.
*
* Returns:     Nothing
*
* Description: Deletes a non active PDP context specified by cid and any
*              associated secondary contexts if it is a primary context.
*
*-------------------------------------------------------------------------*/
static void vgDeleteNonActiveContext     (const VgmuxChannelNumber entity, Int8 cid, Boolean firstTime)
{
  GprsGenericContext_t  *gprsGenericContext_p   = ptrToGprsGenericContext ();
#if defined (FEA_DEDICATED_BEARER)
  Int32                 cidCount;
  Boolean               isPrimaryCid = FALSE;
#endif /* FEA_DEDICATED_BEARER */

  /* This function should not be called with a cid which is not there or active
   */
  FatalCheck (gprsGenericContext_p->cidUserData [cid] != PNULL, entity, cid, 0);
  FatalCheck (!(gprsGenericContext_p->cidUserData [cid]->isActive), entity, cid, 0);

#if defined (FEA_DEDICATED_BEARER)
  isPrimaryCid = !(gprsGenericContext_p->cidUserData [cid]->psdBearerInfo.secondaryContext);
#endif /* FEA_DEDICATED_BEARER */

  vgFreeRamForCid(cid);

  if (firstTime)
  {
    vgPutNewLine (entity);
    vgPrintf (entity, (const Char*)"+CGDEL: %d", cid);
  }
  else
  {
   vgPrintf (entity, (const Char*)",%d", cid);
  }
#if defined (FEA_DEDICATED_BEARER)
  if (isPrimaryCid)
  {
    /* Check if there are any associated secondary contexts and delete them too */
    for (cidCount = vgGpGetMinCidValue(entity); cidCount < MAX_NUMBER_OF_CIDS; cidCount++)
    {
      if ((gprsGenericContext_p->cidUserData [cidCount] != PNULL) &&
          (gprsGenericContext_p->cidUserData [cidCount]->psdBearerInfo.secondaryContext)  &&
          (gprsGenericContext_p->cidUserData [cidCount]->psdBearerInfo.primaryCid == cid))
      {
        /* We found an associated secondary context. It must already be inactive to have
         * got here.
         */
        FatalCheck (!(gprsGenericContext_p->cidUserData [cidCount]->isActive), entity, cid, cidCount);

        vgFreeRamForCid(cidCount);

        vgPrintf (entity, (const Char*)",%d", cidCount);
      }
    }
  }
#endif /* FEA_DEDICATED_BEARER */
}

/*--------------------------------------------------------------------------
*
* Function:    vgDeleteAllNonActiveContexts
*
* Parameters:  entity          - mux channel number
*
* Returns:     Nothing
*
* Description: Deletes all non active PDP contexts.
*
*-------------------------------------------------------------------------*/
static void vgDeleteAllNonActiveContexts (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p   = ptrToGprsGenericContext ();
  Int32                 cidCount;
  Boolean               firstTime = TRUE;

  for (cidCount = vgGpGetMinCidValue(entity); cidCount < MAX_NUMBER_OF_CIDS; cidCount++)
  {
    if ((gprsGenericContext_p->cidUserData [cidCount] != PNULL) &&
        !(gprsGenericContext_p->cidUserData [cidCount]->isActive))
    {
      /* We found a non active CID - so delete the context data
       */
      if (firstTime)
      {
        vgPutNewLine (entity);
      }
      vgDeleteNonActiveContext (entity, cidCount, firstTime);
      firstTime = FALSE;
    }
  }

  if (!firstTime)
  {
    /* We deleted something - so put a newline at the end of the cid list */
    vgPutNewLine (entity);
  }
}

#if defined (FEA_QOS_TFT)
/*--------------------------------------------------------------------------
*
* Function:     vgInitialiseCgeqosData
*
* Parameters:   ptr     - cid profile to be initialised
*
* Returns:      nothing
*
* Description:  Initialises the pdp CGEQOS profile data
*
*-------------------------------------------------------------------------*/

static void vgInitialiseCgeqosData (VgPsdStatusInfo *ptr)
{
  ptr->cgeqosDefined        = FALSE;

  /* required qos data */
  ptr->psdBearerInfo.requiredQos.epsQualityOfServicePresent = FALSE;
  ptr->psdBearerInfo.requiredQos.epsQci = 0;
  ptr->psdBearerInfo.requiredQos.epsBitRatesPresent = FALSE;
  ptr->psdBearerInfo.requiredQos.epsMaxBitRateUplink = 0;
  ptr->psdBearerInfo.requiredQos.epsMaxBitRateDownlink = 0;
  ptr->psdBearerInfo.requiredQos.epsGuaranteedBitRateUplink = 0;
  ptr->psdBearerInfo.requiredQos.epsGuaranteedBitRateDownlink = 0;
}

/*************************************************************************
*
* Function:     viewEqosAttributes
*
* Parameters:   entity - mux channel number
*
* Returns:      nothing
*
* Description:  Outputs the current LTE (CGEQOS) attributes.
*
*************************************************************************/
static void viewEqosAttributes (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();

  VgPsdStatusInfo      *cmd_p;
  Int8                  profile;
  Boolean               firstPass = TRUE;

  /* go through all profiles */
  for (profile = vgGpGetMinCidValue(entity); profile < MAX_NUMBER_OF_CIDS; profile++)
  {
    /* check CID EPS values exist */
    if ((gprsGenericContext_p->cidUserData [profile] != PNULL) &&
        (gprsGenericContext_p->cidUserData [profile]->profileDefined) &&
        (gprsGenericContext_p->cidUserData [profile]->cgeqosDefined) &&
        (gprsGenericContext_p->cidUserData [profile]->psdBearerInfo.requiredQos.epsQualityOfServicePresent))
    {
      cmd_p = gprsGenericContext_p->cidUserData [profile];

      if (firstPass)
      {
        vgPutNewLine (entity);
        firstPass = FALSE;
      }

      /* Display AT+CGEQOS attributes */
      vgPrintf (entity, (const Char*)"+CGEQOS: %d,%d",
                profile,
                cmd_p->psdBearerInfo.requiredQos.epsQci);

      /* Display the bit rates only if they are present */
      if (cmd_p->psdBearerInfo.requiredQos.epsBitRatesPresent == TRUE)
      {
        vgPrintf (entity, (const Char*)",%d,%d,%d,%d",
                  cmd_p->psdBearerInfo.requiredQos.epsGuaranteedBitRateDownlink,
                  cmd_p->psdBearerInfo.requiredQos.epsGuaranteedBitRateUplink,
                  cmd_p->psdBearerInfo.requiredQos.epsMaxBitRateDownlink,
                  cmd_p->psdBearerInfo.requiredQos.epsMaxBitRateUplink);
      }
      vgPutNewLine (entity);
    }
  }
}

/*--------------------------------------------------------------------------
*
* Function:    vgDisplayCGEQOSRDPInfo
*
* Parameters:  entity          - mux channel number
*              cid             - CID to display
*
* Returns:     Nothing
*
* Description: Displays EPS Quality of Service which have been assigned to
*              a CID when the context was activated.
*-------------------------------------------------------------------------*/
static void vgDisplayCGEQOSRDPInfo (const VgmuxChannelNumber entity, Int8 cid)
{
    GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
    VgPsdStatusInfo       *cmd_p                = PNULL;

    cmd_p = gprsGenericContext_p->cidUserData [cid];

    FatalAssert ((cmd_p != PNULL) && (cmd_p->isActive == TRUE));

    if ((cmd_p->profileDefined) &&
        (cmd_p->psdBearerInfo.negotiatedQos.epsQualityOfServicePresent))
    {
        vgPrintf (entity, (const Char*)"+CGEQOSRDP: %d,%d",
                  cid,
                  cmd_p->psdBearerInfo.negotiatedQos.epsQci);

        /* Display the bit rates only if they are present */
        if (cmd_p->psdBearerInfo.negotiatedQos.epsBitRatesPresent == TRUE)
        {
          vgPrintf (entity, (const Char*)",%d,%d,%d,%d",
                    cmd_p->psdBearerInfo.negotiatedQos.epsGuaranteedBitRateDownlink,
                    cmd_p->psdBearerInfo.negotiatedQos.epsGuaranteedBitRateUplink,
                    cmd_p->psdBearerInfo.negotiatedQos.epsMaxBitRateDownlink,
                    cmd_p->psdBearerInfo.negotiatedQos.epsMaxBitRateUplink);
        }
        vgPutNewLine (entity);
    }
}

/*--------------------------------------------------------------------------
*
* Function:    vgDisplayAllCGEQOSRDPInfo
*
* Parameters:  entity          - mux channel number
*
* Returns:     Nothing
*
* Description: Displays EPS Quality of Service which have been assigned to
*              all active CIDs when the contexts were activated.
*-------------------------------------------------------------------------*/
static void vgDisplayAllCGEQOSRDPInfo (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p   = ptrToGprsGenericContext ();
  VgPsdStatusInfo       *cmd_p                  = PNULL;
  Int32                 cid;
  Boolean               firstTime               = TRUE;

  for (cid = vgGpGetMinCidValue(entity); cid < MAX_NUMBER_OF_CIDS; cid++)
  {
    if (gprsGenericContext_p->cidUserData [cid] != PNULL)
    {
      cmd_p = gprsGenericContext_p->cidUserData [cid];

      /* if the profile is active, display cid info */
      if (cmd_p->isActive && cmd_p->profileDefined)
      {
        if (firstTime)
        {
          vgPutNewLine (entity);
          firstTime = FALSE;
        }
        vgDisplayCGEQOSRDPInfo(entity, (Int8)cid);
      }
    }
  }
}

#endif /* FEA_QOS_TFT */

/*--------------------------------------------------------------------------
*
* Function:     vgGetFreeCid
*
* Parameters:   cidValue    - return a free cid
*               entity      - mux channel number
*
* Returns:      Boolean     - whether cid can be gotten
*
* Description:  Checks whether a free cid can be gotten.
*
*-------------------------------------------------------------------------*/

Boolean vgGetFreeCid (Int32 *cidValue,
                             const VgmuxChannelNumber entity)
{
#if defined (FEA_MT_PDN_ACT)
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
#endif
  Boolean               result                = FALSE;
  Boolean               found                 = FALSE;
  Boolean               firstCidFlag          = FALSE;
  Int8                  cidIndex;


  /* Check whether a free cid can be used */
  for (cidIndex = vgGpGetMinCidValue(entity); cidIndex < MAX_NUMBER_OF_CIDS; cidIndex++ )
  {
#if defined (FEA_MT_PDN_ACT)
      /* check whether cid is defined for MT PDP */
      if ((!gprsGenericContext_p->vgMMTPDPCIDData.enabled) ||
          (gprsGenericContext_p->vgMMTPDPCIDData.enabled    &&
          (gprsGenericContext_p->vgMMTPDPCIDData.cid !=cidIndex)))
#endif /* FEA_MT_PDN_ACT */
      {
        if (vgOpManCidDefined (cidIndex))
        {
          /* Check whether cid is in use by another entity or
             has been activated in current entity*/
          if (vgOpManCidAvailable (cidIndex, entity) &&
              !vgGpGetCidActiveStatus (cidIndex))
          {
            *cidValue = (Int32)cidIndex;
            found     = TRUE;
            break;
          }
        }
        else if( firstCidFlag == FALSE )
        {
          /* record the first free cid */
          *cidValue     = (Int32)cidIndex;
          firstCidFlag  = TRUE;
        }
      }
   }

  if((found == TRUE) || (firstCidFlag == TRUE))
  {
    result = TRUE;
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:     vgGetFreeCidForDataConn
*
* Parameters:   cidValue    - return a free cid
*               entity      - mux channel number
*
* Returns:      Boolean     - whether cid can be gotten
*
* Description:  Checks whether a free cid can be gotten... The CID
*               is also regarded as free in this function if the
*               CID is active BUT with the data connection type set to none.
*               This allows ATD*99# for example to grab a CID which was
*               previously activated with AT+CGACT
*
*-------------------------------------------------------------------------*/

Boolean vgGetFreeCidForDataConn (Int32 *cidValue,
                                 const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  Boolean               result                = FALSE;
  Boolean               found                 = FALSE;
  Boolean               firstCidFlag          = FALSE;
  Int8                  cidIndex;


  /* Check whether a free cid can be used */
  for (cidIndex = vgGpGetMinCidValue(entity); cidIndex < MAX_NUMBER_OF_CIDS; cidIndex++ )
  {
#if defined (FEA_MT_PDN_ACT)
      /* check whether cid is defined for MT PDP */
      if ((!gprsGenericContext_p->vgMMTPDPCIDData.enabled) ||
          (gprsGenericContext_p->vgMMTPDPCIDData.enabled    &&
          (gprsGenericContext_p->vgMMTPDPCIDData.cid !=cidIndex)))
#endif /* FEA_MT_PDN_ACT */
      {
        if (vgOpManCidDefined (cidIndex))
        {
          /* Check whether cid is in use by another entity or
           * has been activated in current entity.  It is OK to be activated
           * as long as the data conn type is ABPD_CONN_TYPE_NONE
           */
          if (vgOpManCidAvailable (cidIndex, entity) &&
              ((!vgGpGetCidActiveStatus (cidIndex)) ||
               (vgGpGetCidActiveStatus (cidIndex) &&
                (gprsGenericContext_p->cidUserData[cidIndex]->psdBearerInfo.connType == ABPD_CONN_TYPE_NONE))))
          {
            *cidValue = (Int32)cidIndex;
            found     = TRUE;
            break;
          }
        }
        else if( firstCidFlag == FALSE )
        {
          /* record the first free cid */
          *cidValue     = (Int32)cidIndex;
          firstCidFlag  = TRUE;
        }
      }
   }

  if((found == TRUE) || (firstCidFlag == TRUE))
  {
    result = TRUE;
  }

  return (result);
}

#if defined (FEA_MT_PDN_ACT)
/*--------------------------------------------------------------------------
*
* Function:    vgGpCGANS
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: handle the command to accept or reject
* mobile-terminated pdp context activation
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGpCGANS (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t          result               = RESULT_CODE_OK;
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  Boolean               response             = FALSE;
  Char                  linkType[STRING_LENGTH_40 + NULL_TERMINATOR_LENGTH] = {0};
  Int16                 linkTypeLength       = 0;

  AbpdPdpConnType       connectionType       = ABPD_CONN_TYPE_PACKET_TRANSPORT;
  Int32                 cid                  = 0;
  Int32                 param                = 0;
  ExtendedOperation_t   operation            = getExtendedOperation (commandBuffer_p);

  PARAMETER_NOT_USED(entity);

  switch (operation)
  {
    case EXTENDED_ASSIGN:
    {
      linkType[0] = '\0';
      /* the parameters are: 0/1 reject/accept; "PPP" or "M-CPPP"; cid.  the
         defaults are 0,"PPP",1 */
      if (result == RESULT_CODE_OK &&
          getExtendedParameter (commandBuffer_p, &param, ULONG_MAX))
      {
        response = (Boolean) param;
      }

      /* the remaining parameters are irrelevant if the response is 0,
         so don't both looking at them */
      if (result == RESULT_CODE_OK && response &&
          ! gprsGenericContext_p->incomingPdpContextActivation)
      {
        result = VG_CME_OPERATION_NOT_ALLOWED;
      }
      if (result == RESULT_CODE_OK && response)
      {
          if (getExtendedString (commandBuffer_p,
                                 linkType,
                                 STRING_LENGTH_40,
                                 &linkTypeLength) &&
              linkTypeLength != 0)
          {
#if defined (FEA_PPP)
              if (vgStrCmp (linkType, "PPP") == 0)
              {
                  /* connection type remains as ABPD_CONN_TYPE_PPP */
              }
              else if (vgStrCmp (linkType, "M-CPPP") == 0)
              {
                  connectionType = ABPD_CONN_TYPE_CORE_PPP;
              }
              else
#endif /* FEA_PPP */
              if (vgStrCmp (linkType, "M-PT") == 0)
              {
                  connectionType = ABPD_CONN_TYPE_PACKET_TRANSPORT;
              }
              else
              {
                  /* Invalid L2P text entered */
                  result = VG_CME_INVALID_TEXT_CHARS;
              }
          }
          else
          {
              /* Get connection type from profile value */

              connectionType = (AbpdPdpConnType)getProfileValue(entity, PROF_MGMTPCACT);

          }
      }

      /* get the CID */
      if (result == RESULT_CODE_OK && response)
      {
        if (getExtendedParameter (commandBuffer_p, &cid, ULONG_MAX))
        {
          /* job127550: if CID not supplied use transient CID */
          if (cid == ULONG_MAX)
          {
            /* Check if a cid is configured by MMTPDPCID to be the default cid for
             * MT PDP contexts. If one is configured, use it. Otherwise use transient cid.
             */
            if (gprsGenericContext_p->vgMMTPDPCIDData.enabled)
            {
              cid = gprsGenericContext_p->vgMMTPDPCIDData.cid;
            }
            else
            {
              /* get a first free cid */
              if(vgGetFreeCid(&cid, entity) == FALSE)
              {
                result = VG_CME_INSUFFIC_RESOURCES;
              }
            }
          }
          else
          {
            if ((!(cid >= vgGpGetMinCidValue(entity))) || (cid >= MAX_NUMBER_OF_CIDS))
            {
              result = VG_CME_INVALID_CID_VALUE;
            }
          }
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
      }
      if (result == RESULT_CODE_OK)
      {
        if (response)
        {
          result = vgPdAnswer (entity, cid, connectionType);
        }
        else
        {
          /* send a call setup response rejection only if an incoming call setup
           * indication has been received */
          if (gprsGenericContext_p->incomingPdpContextActivation)
          {
            result = vgPdReject (entity);
          }
        }
      }
      break;
    }
    case EXTENDED_RANGE:
    {
      vgPutNewLine (entity);
      /* output the list of supported responses (0/1), and the list of
         supported l2ps (PPP, M-CPPP, or M-PT) */
#if define (FEA_PPP)
      vgPrintf (entity, (const Char*)"+CGANS: (0-1),(\"PPP\",\"M-CPPP\",\"M-PT\")");
#else
      vgPrintf (entity, (const Char*)"+CGANS: (0-1),(\"M-PT\")");
#endif /* FEA_PPP */
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_ACTION:
    case EXTENDED_QUERY:
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  /* 07.07: other commands are not allowed to be concatenated on the
     end of the line */
  commandBuffer_p->position = commandBuffer_p->length;
  return (result);
}
#endif /* FEA_MT_PDN_ACT */

/*--------------------------------------------------------------------------
*
* Function:    vgGpEnsureAttached
*
* Parameters:  entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Ensure that we are attached to the gprs network
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGpEnsureAttached (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  ResultCode_t          result               = RESULT_CODE_OK;

  if ( ! gprsGenericContext_p->gprsServiceState.gprsAttached)
  {
    result = vgChManContinueAction (entity, SIG_APEX_ABPD_PSD_ATTACH_REQ);
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGpCGATT
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+CGATT command which attaches
*              or detaches the ME from the network.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGpCGATT (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation            = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  ResultCode_t          result               = RESULT_CODE_OK;
  Int32                 cgatt;

  GprsContext_t *gprsContext_p = ptrToGprsContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(gprsContext_p != PNULL, entity, 0, 0);
#endif
  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT+CGATT=? */
    {
      vgPutNewLine (entity);
      vgPuts       (entity, (const Char*)"+CGATT: (0-1)");
      break;
    }
    case EXTENDED_ASSIGN:   /* AT+CGATT=  */
    {
      if (getExtendedParameter (commandBuffer_p, &cgatt, ULONG_MAX))
      {
        if (cgatt == ULONG_MAX)
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }

      if (result == RESULT_CODE_OK)
      {
        switch (cgatt)
        {
          case GPRS_ATTACH_REQ: /* perform GPRS attach */
          {
            if (gprsGenericContext_p->gprsServiceState.gprsAttached == FALSE)
            {
              gprsContext_p->attachInProgress = TRUE;
              result = vgChManContinueAction (entity, SIG_APEX_ABPD_PSD_ATTACH_REQ);
            }
            else
            {
              result = RESULT_CODE_OK;
            }
            break;
          }
          case GPRS_DETACH_REQ: /* perform GPRS detach */
          {
            /* For LTE we allow a detach to be sent even if we are not attach..
             * this is to allow for GCF test cases which try to abort during the
             * power-on attach procedure
             */
            result = vgChManContinueAction (entity, SIG_APEX_ABPD_PSD_DETACH_REQ);
            break;
          }

          default:
          {
            result = VG_CME_INVALID_INPUT_VALUE;
            break;
          }
        }
      }
      break;
    }
    case EXTENDED_QUERY:    /* AT+CGATT?  */
    {

#if 0
      /* Useful for memory size measurements */
      printf ("Entity size: %d, PSD part: %d", sizeof(Entity_t),sizeof(VgPsdStatusInfo));
#endif

      if (gprsGenericContext_p->gprsServiceState.valid == TRUE)
      {
        vgPutNewLine (entity);
        vgPrintf (entity,
                  (const Char*)"+CGATT: %d",
                  (Int8)gprsGenericContext_p->gprsServiceState.gprsAttached);
        vgPutNewLine (entity);
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    case EXTENDED_ACTION:   /* AT+CGATT   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:     vgGpCGACT
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description:  Allows TE to activate a PDP context previously defined
*               and stored from/in CI task. This context profile is
*               then sent to the PPP task together with the ATD*99#
*               dial string.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGpCGACT (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  ResultCode_t         result    = RESULT_CODE_OK;
  Int32                cid       = 0;
  Int32                tmpCid;    /* Dummy cid value */
  Int32                state;
  Int16                loopCid   = MAX_NUMBER_OF_CIDS;

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT+CGACT=? */
    {
      vgPutNewLine (entity);
      vgPuts       (entity, (const Char*)"+CGACT: (0-1)");
      break;
    }
    case EXTENDED_ASSIGN:   /* AT+CGACT=  */
    {
      /* activation state */
      if (getExtendedParameter (commandBuffer_p, &state, ULONG_MAX))
      {
        if (((state == 0) || (state == 1)) == FALSE)
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }

      /* get the CID */
      if (result == RESULT_CODE_OK)
      {
        if (getExtendedParameter (commandBuffer_p, &cid, ULONG_MAX))
        {
          /* job106759: if there's only one cid, then allow <cid> parameter */
          /* to be omitted, and activate/deactivate the sole cid - NEEDS REWORK! */
          if (cid == ULONG_MAX)
          {
            /* get a free cid to use */
            if ((state ==1) && (vgGetFreeCid (&cid, entity) != TRUE))
            {
              /* send error to user and stop */
              result = VG_CME_NO_FURTHER_CIDS_SUPPORTED;
            }
            else if(state == 0)
            {
              for(loopCid = 0; loopCid < MAX_NUMBER_OF_CIDS; loopCid++)
              {
                if(gprsGenericContext_p->cidOwner[loopCid] == entity)
                {
                    /* Find a cid. */
                    cid = loopCid;
                    break;
                }
              }
            }
          }
          /* AT+CGACT=x,0 is not possible. CID 0 is automatically
           * activated during attach and deactivated on detach
           */
          if ((!(cid >= 1))||(cid >= MAX_NUMBER_OF_CIDS))
          {
            result = VG_CME_INVALID_CID_VALUE;
          }
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
      }

      if (result == RESULT_CODE_OK)
      {
        /* This checks case where user may have entered multiple cid values.
         * This is not supported at present */
        if (getExtendedParameter (commandBuffer_p, &tmpCid, ULONG_MAX))
        {
          if (tmpCid != ULONG_MAX)
          {
            /* send error to user and stop */
            result = VG_CME_NO_FURTHER_CIDS_SUPPORTED;
          }
        }
      }

      if (result == RESULT_CODE_OK)
      {
        if (vgAllocateRamToCid (cid))
        {
          /* Check whether cid has been created yet */
          if (vgOpManCidDefined ((Int8)cid))
          {
            /* Check whether cid is in use by another entity.
             * Only check this for activation.  We can deactivate
             * a cid from any channel
             */
            if ((state == 1) &&(!vgOpManCidAvailable ((Int8)cid, entity)))
            {
              result = VG_CME_CID_NOT_AVAILABLE;
            }
          }
          else
          {
            result = VG_CME_PROFILE_NOT_DEFINED;
          }
        }
        else
        {
          /* No more CID memory available */
          result = VG_CME_INSUFFIC_RESOURCES;
        }
      }

      if (result == RESULT_CODE_OK)
      {
        if (state == 1)
        {
          /* Activate context */
          if(result == RESULT_CODE_OK)
          {
            result = vgActivateContext ((Int8)cid, entity, ABPD_CONN_TYPE_NONE);
          }
        }
        else
        {
          /* Deactivate context */
          result = vgDeactivateContext (cid, entity);
        }
      }
      break;
    }
    case EXTENDED_QUERY:    /* AT+CGACT?  */
    {
      viewActivationState ((const Char*)"+CGACT: ", entity);
      break;
    }
    case EXTENDED_ACTION:   /* AT+CGACT   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgGpMGCOUNT
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: Executes command AT*MGCOUNT AT command, used to control the
 *              reporting of SNDCP data counters to the ME.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgGpMGCOUNT (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  ResultCode_t          result = RESULT_CODE_OK;
  Int8                  cid;
  Int32                 tempVar;
  CountAction           action;

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT*MGCOUNT=? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"%C: (0-1),(1-%d)", MAX_NUMBER_OF_CIDS-1);
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_ASSIGN:   /* AT*MGCOUNT=  */
    {
      /* get action */
      if (getExtendedParameter (commandBuffer_p, &tempVar, ULONG_MAX))
      {
        if (tempVar <= READ_SNDCP_COUNTER)
        {
          action = (CountAction)tempVar;
          /* get CID */
          if (getExtendedParameter (commandBuffer_p, &tempVar, ULONG_MAX))
          {
            if ((tempVar >= vgGpGetMinCidValue(entity)) && (tempVar < MAX_NUMBER_OF_CIDS))
            {
              cid = (Int8)tempVar;

              if (FALSE == vgOpManCidActive(cid))
              {
                /* CID is not active */
                result = VG_CME_INVALID_ACTIVATION_STATE;
              }
            }
            /* If cid is not inputted, get Tx/Rx data for all active PDN connections */
            else if(tempVar == ULONG_MAX)
            {
              cid = CID_NUMBER_UNKNOWN;
            }
            else
            {
              /* supplied CID is out of range */
              result = VG_CME_INVALID_CID_VALUE;
            }

            if(result == RESULT_CODE_OK)
            {
              gprsGenericContext_p->vgMGCOUNTData.vgReportEntity = entity;
              gprsGenericContext_p->vgMGCOUNTData.vgCounterCid = cid;

              switch (action)
              {
                case RESET_SNDCP_COUNTER:
                {
                  result = vgChManContinueAction (entity, SIG_APEX_ABPD_RESET_COUNTER_REQ);

                  if(result = RESULT_CODE_PROCEEDING)
                  {
                    result = RESULT_CODE_OK;
                  }
                  break;
                }
                case READ_SNDCP_COUNTER:
                {
                  result = vgChManContinueAction (entity, SIG_APEX_ABPD_REPORT_COUNTER_REQ);
                  break;
                }
                /* Other modes do not support */
                default:
                {
                  result = VG_CME_UNSUPPORTED_MODE;
                  break;
                }
              }
            }
          }
          else
          {
            /* error reading parameter (CID) */
            result = RESULT_CODE_ERROR;
          }
        }
        else
        {
          /* invalid action value */
          result = VG_CME_UNSUPPORTED_MODE;
        }
      }
      else
      {
        /* error reading parameter (action) */
        result = RESULT_CODE_ERROR;
      }
      break;
    }

    case EXTENDED_QUERY:    /* AT*MGCOUNT?  */
    {
      result = RESULT_CODE_OK;
      break;
    }
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgGpCGDATA
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: Executes command AT+CGDATA AT command (see 07.07)
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgGpCGDATA (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  ResultCode_t          result = RESULT_CODE_OK;
  Int32                 tmpCid;
  Int8                  cids[MAX_NUMBER_OF_CIDS] = {0};
  Int8                  numCids = 0;
  Int8                  index;
  Char                  linkType[STRING_LENGTH_40 + NULL_TERMINATOR_LENGTH] = {0};
  Int16                 linkTypeLength = 0;

  AbpdPdpConnType       connectionType        = ABPD_CONN_TYPE_PACKET_TRANSPORT;

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT+CGDATA=? */
    {
      vgPutNewLine (entity);
#if defined (FEA_PPP)
      vgPuts (entity, (const Char*)"+CGDATA: (\"PPP\",\"M-CPPP\",\"M-PT\")");
#else
      vgPuts (entity, (const Char*)"+CGDATA: (\"M-PT\")");
#endif /* FEA_PPP */
      break;
    }
    case EXTENDED_ASSIGN:   /* AT+CGDATA=  */
    {
      /* get protocol */
      if (getExtendedString (commandBuffer_p,
                             linkType,
                             STRING_LENGTH_40,
                             &linkTypeLength))
      {
        if (linkTypeLength != 0)
        {
#if defined (FEA_PPP)
          if (vgStrCmp (linkType, "PPP") == 0)
          {
            connectionType = ABPD_CONN_TYPE_PPP;
          }
          else if (vgStrCmp (linkType, "M-CPPP") == 0)
          {
            connectionType = ABPD_CONN_TYPE_CORE_PPP;
          }
          else
#endif /* FEA_PPP */
          if (vgStrCmp (linkType, "M-PT") == 0)
          {
            connectionType = ABPD_CONN_TYPE_PACKET_TRANSPORT;
          }
          else
          {
            /* Invalid L2P text entered */
            result = VG_CME_INVALID_TEXT_CHARS;
          }
        }

        if (result == RESULT_CODE_OK)
        {
          for (index = 0;
               ((index < MAX_NUMBER_OF_CIDS) &&
                (result == RESULT_CODE_OK));
               index++)
          {
            /* get context identifier */
            if (getExtendedParameter (commandBuffer_p,
                                      &tmpCid,
                                      ULONG_MAX))
            {
              if ((!(tmpCid >= vgGpGetMinCidValue(entity))) ||
                  ((tmpCid != ULONG_MAX) && (tmpCid >= MAX_NUMBER_OF_CIDS)))
              {
                result = VG_CME_INVALID_CID_VALUE;
              }
              else
              {
                if (tmpCid != ULONG_MAX)
                {
                  cids[numCids] = (Int8)tmpCid;
                  numCids += 1;
                  if (numCids > 1)
                  {
                    /* too many cids supplied */
                    result = VG_CME_NO_FURTHER_CIDS_SUPPORTED;
                  }
                }
              }
            }
            else
            {
              result = VG_CME_INVALID_CID_VALUE;
            }
          }

          if ((numCids == 0) && (result == RESULT_CODE_OK))
          {
            /* must have one cid supplied */
            result = VG_CME_INVALID_CID_VALUE;
          }
        }
      }
      else /* no protocol specified */
      {
        result = VG_CME_INVALID_TEXT_CHARS;
      }

      if (result == RESULT_CODE_OK)
      {

        if (vgAllocateRamToCid (cids[0]))
        {
          /* Check whether cid is in use by another entity */
          if (vgOpManCidAvailable ((Int8)cids[0], entity) == FALSE)
          {
            result = VG_CME_CID_NOT_AVAILABLE;
          }
        }
        else
        {
          /* Not data for CID */
          result = VG_CME_INSUFFIC_RESOURCES;
        }
      }

      if (result == RESULT_CODE_OK)
      {

        /* For NASMDL2 we leave the attach procedure up to ABPD - so we
         * don't need to explicitly try to attach here
         */
        {
          /* Only activate first context in the CID list */
          result = vgActivateContext ((Int8)cids[0], entity, connectionType);
        }
      }
      break;
    }
    case EXTENDED_QUERY:    /* AT+CGDATA?  */
    case EXTENDED_ACTION:   /* AT+CGDATA   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}


/*--------------------------------------------------------------------------
*
* Function:    vgGpCGDCONT
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Allows the user to amend the CGQCONT attributes for a
*              given cid. The cid must be supplied.  Other parameters are
*              optional. If parameters are not included then that
*              attribute will become undefined.  If all attributes are
*              not supplied then the entire profile becomes undefined.
*              A command with valid parmeters will define a profile.
*              The profile may be undefined by this command.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGpCGDCONT (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  ResultCode_t          result = RESULT_CODE_OK;
  Int32                 tmpVar;
  Int32                 cid;
  Boolean               defd = FALSE;
  Boolean               strFound = FALSE;
  Char                  tmpString [MAX_APN_NAME + NULL_TERMINATOR_LENGTH] = {0};
  Int16                 tmpStringLen = 0;

  VgPsdStatusInfo       *cmd_p = PNULL;
  PdnType               pdnType = PDN_TYPE_IPV4;

#if defined (FEA_DEDICATED_BEARER)
  Int32                 cid_count;
  VgPsdStatusInfo       *secondary_context_p = PNULL;
#endif

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT+CGDCONT=? */
    {
      viewCGDCONTRange (entity);
      break;
    }
    case EXTENDED_ASSIGN:   /* AT+CGDCONT=  */
    {
      /* get the CID */
      if (getExtendedParameter (commandBuffer_p, &cid, ULONG_MAX))
      {
        /* if the cid is invalid or does not exist; tmpVar = default
           value of ULONG_MAX then error. */
        if ((cid >=vgGpGetMinCidValue(entity)) &&(cid < MAX_NUMBER_OF_CIDS))
        {
          if (vgAllocateRamToCid (cid))
          {
            cmd_p = gprsGenericContext_p->cidUserData [cid];

            /* save old values to reinstate later if change unsuccessful */
            memcpy( &(gprsGenericContext_p->oldPsdBearerInfo), &cmd_p->psdBearerInfo, sizeof(VgPsdBearerInfo));
          }
          else
          {
            /* No more memory for CID data */
            result = VG_CME_INSUFFIC_RESOURCES;
          }
        }
        else
        {
          result = VG_CME_INVALID_CID_VALUE;
        }
      }
      else
      {
        /* must supply at least one cid */
        result = RESULT_CODE_ERROR;
      }

      if (result == RESULT_CODE_OK)
      {
        /* Check whether cid is in use by another entity */
        if (vgOpManCidAvailable ((Int8)cid, entity) == FALSE)
        {
          result = VG_CME_CID_NOT_AVAILABLE;
        }
      }

      if (result == RESULT_CODE_OK)
      {
        /* Check that there isn't already an active context */
        if (vgOpManCidActive ((Int8)cid))
        {
          result = VG_CME_CID_ALREADY_ACTIVE;
        }
      }

      if (result == RESULT_CODE_OK)
      {
         FatalAssert (cmd_p != PNULL);

        /* Check that the context has not been defined by cgdscont */
        if (cmd_p->cgdscontDefined)
        {
          result = RESULT_CODE_ERROR;
        }
      }

      /* now get the PDP_TYPE */
      if (result == RESULT_CODE_OK)
      {
        FatalAssert (cmd_p != PNULL);

        if (getExtendedString (commandBuffer_p,
                               tmpString,
                               STRING_LENGTH_40,
                               &tmpStringLen))
        {

          if (tmpStringLen == 0)
          {
            /* no parameter so undefine it */
            cmd_p->psdBearerInfo.reqPdnAddress.addressPresent = FALSE;
          }
          else
          {
            strFound = vgPDNStrToEnum (tmpString, &pdnType);
            if (strFound == FALSE)
            {
              result = RESULT_CODE_ERROR;
            }
            else
            {
              if ((pdnType == PDN_TYPE_IPV4)  ||
                  (pdnType == PDN_TYPE_IPV6)  ||
                  (pdnType == PDN_TYPE_IPV4V6) ||
                  (pdnType == PDN_TYPE_NONIP))
              {
                cmd_p->psdBearerInfo.reqPdnAddress.pdnType = pdnType;
                defd = TRUE;
              }
              else
              {
                result = VG_CME_PDP_TYPE_NOT_SUPPORTED;
              }
            }
          }
        }
        else /* error reading supplied parameter */
        {
          result = RESULT_CODE_ERROR;
        }
        memset (tmpString, NULL_CHAR, sizeof(tmpString));
      }

      /* now get the APN */
      if (result == RESULT_CODE_OK)
      {
        FatalAssert (cmd_p != PNULL);

        if (getExtendedString (commandBuffer_p,
                               tmpString,
                               MAX_APN_NAME,
                               &tmpStringLen))
        {

          if (tmpStringLen == 0)
          {
            /* no parameter so undefine it */
            cmd_p->psdBearerInfo.reqApnPresent = FALSE;
          }
          else
          {
            if (tmpStringLen <= sizeof(cmd_p->psdBearerInfo.reqTextualApn.name))
            {
              memcpy (cmd_p->psdBearerInfo.reqTextualApn.name, tmpString, tmpStringLen);
              cmd_p->psdBearerInfo.reqTextualApn.length = (Int8) tmpStringLen;

              /* Generate the network version of the APN which is needed by ABPD */
              vgCiApnToNetworkAPN(cmd_p->psdBearerInfo.reqTextualApn, &(cmd_p->psdBearerInfo.reqApn));
              cmd_p->psdBearerInfo.reqApnPresent = TRUE;
              defd = TRUE;
            }
            else
            {
              cmd_p->psdBearerInfo.reqTextualApn.length = 0;
              cmd_p->psdBearerInfo.reqApn.length = 0;
              cmd_p->psdBearerInfo.reqApnPresent = FALSE;
              result = RESULT_CODE_ERROR;
            }
          }
        }
        else /* error reading supplied parameter */
        {
          result = RESULT_CODE_ERROR;
        }
        memset (tmpString, NULL_CHAR, sizeof(tmpString));
      }

      /* now get the PDP ADDR.
       * Should not be present for IP type IPV4V6 and Non-IP
       */
      if (result == RESULT_CODE_OK)
      {
        FatalAssert (cmd_p != PNULL);

        if (getExtendedString (commandBuffer_p,
                               tmpString,
                               MAX_TEXTUAL_PDN_ADDR,
                               &tmpStringLen))
        {

          if (tmpStringLen == 0)
          {
            /* no parameter so undefine it */
            cmd_p->psdBearerInfo.reqPdnAddress.addressPresent = FALSE;
          }
          else
          {
            TextualPdnAddress textualPdnAddress;

            textualPdnAddress.addressPresent  = TRUE;
            textualPdnAddress.length = (Int8)tmpStringLen;
            memcpy (textualPdnAddress.address, tmpString, tmpStringLen);
            textualPdnAddress.address[tmpStringLen] = '\0';

            switch (pdnType)
            {
              case PDN_TYPE_IPV4:
                result = vgStringToPDNAddr (pdnType,
                                            &textualPdnAddress,
                                            &cmd_p->psdBearerInfo.reqPdnAddress);
                break;
              case PDN_TYPE_IPV6:
                result = vgStringToPDNAddr (pdnType,
                                            &textualPdnAddress,
                                            &cmd_p->psdBearerInfo.reqPdnAddress);
                break;
              case PDN_TYPE_IPV4V6:
                result = RESULT_CODE_ERROR;
                break;
              case PDN_TYPE_NONIP:
                result = RESULT_CODE_ERROR;
                break;
              default:
                /* No other types permitted at the moment */
                FatalParam (entity, pdnType, 0);
                break;
            }

            if (result == RESULT_CODE_OK)
            {
              defd = TRUE;
            }
            else
            {
              cmd_p->psdBearerInfo.reqPdnAddress.addressPresent = FALSE;
              /* we use the result code returned by vgStringToPDNAddr */
            }
          }
          memset (tmpString, NULL_CHAR, sizeof (tmpString));
        }
        else /* error reading supplied parameter */
        {
          result = RESULT_CODE_ERROR;
        }
      }

      /* now get the D_COMP */
      if (result == RESULT_CODE_OK)
      {
        FatalAssert (cmd_p != PNULL);

        if (getExtendedParameter (commandBuffer_p, &tmpVar, ULONG_MAX))
        {

          if (tmpVar == ULONG_MAX)
          {
            /* no parameter so undefine it */
            cmd_p->psdBearerInfo.dataComp = DATA_COMP_OFF;
          }
          else
          {
            if (tmpVar <= (Int32)DATA_COMP_V42BIS)
            {
              cmd_p->psdBearerInfo.dataComp = (DataCompType)tmpVar;
              defd = TRUE;
            }
            else
            {
              result = VG_CME_INVALID_INPUT_VALUE;
            }
          }
        }
        else /* error reading supplied parameter */
        {
          result = RESULT_CODE_ERROR;
        }
      }

      /* get the P_COMP */
      if (result == RESULT_CODE_OK)
      {
        FatalAssert (cmd_p != PNULL);

        if (getExtendedParameter (commandBuffer_p, &tmpVar, ULONG_MAX))
        {

          if (tmpVar == ULONG_MAX)
          {
            /* no parameter so undefine it */
            cmd_p->psdBearerInfo.headerComp = HEADER_COMP_OFF;
          }
          else
          {
            if (tmpVar <= (Int32)HEADER_COMP_RFC3095)
            {
              cmd_p->psdBearerInfo.headerComp = (HeaderCompType)tmpVar;
              defd = TRUE;
            }
            else
            {
              result = VG_CME_INVALID_INPUT_VALUE;
            }
          }
        }
        else /* error reading supplied parameter */
        {
          result = RESULT_CODE_ERROR;
        }
      }

      /* get the IPv4AddrAlloc which is actually not supported now */
      if (result == RESULT_CODE_OK)
      {
        FatalAssert (cmd_p != PNULL);
        if (getExtendedParameter (commandBuffer_p, &tmpVar, ULONG_MAX))
        {
          if (!(0 == tmpVar || ULONG_MAX == tmpVar))
          {
            result = VG_CME_INVALID_INPUT_VALUE;
          }
        }
        else /* error reading supplied parameter */
        {
          result = RESULT_CODE_ERROR;
        }
      }

      /* Get the next 5 parameters - all of which are ignored */
      if (result == RESULT_CODE_OK)
      {
        (void) (getExtendedParameter (commandBuffer_p, &tmpVar, ULONG_MAX));
      }
      if (result == RESULT_CODE_OK)
      {
        (void) (getExtendedParameter (commandBuffer_p, &tmpVar, ULONG_MAX));
      }
      if (result == RESULT_CODE_OK)
      {
        (void) (getExtendedParameter (commandBuffer_p, &tmpVar, ULONG_MAX));
      }

#if defined(FEA_TEMP_REL14_ABPD_DUAL_PRIORITY)
      /* now get the NSLPI */
      if (result == RESULT_CODE_OK)
      {
        FatalAssert (cmd_p != PNULL);
        if (getExtendedParameter (commandBuffer_p, &tmpVar, VG_MS_CONFIGURED_FOR_LOW_PRIO))
        {
          if (tmpVar > VG_MS_NOT_CONFIGURED_FOR_LOW_PRIO)
          {
            result = VG_CME_INVALID_INPUT_VALUE;
          }
          else
          {
            cmd_p->psdBearerInfo.nasSigPriority = (VgNasSigPrioType)tmpVar;
          }
        }
        else /* error reading supplied parameter */
        {
          result = RESULT_CODE_ERROR;
        }
      }
#else
      if (result == RESULT_CODE_OK)
      {
        (void) (getExtendedParameter (commandBuffer_p, &tmpVar, ULONG_MAX));
      }
#endif
      /* Get the next 1 parameter - ignore the parameter */
      if (result == RESULT_CODE_OK)
      {
        (void) (getExtendedParameter (commandBuffer_p, &tmpVar, ULONG_MAX));
      }

      /* Get IPV4 MTU discovery */
      if (result == RESULT_CODE_OK)
      {
        FatalAssert (cmd_p != PNULL);
        if (getExtendedParameter (commandBuffer_p, &tmpVar, ULONG_MAX))
        {
          if (!((MTU_SIZE_NOT_REQUESTED == tmpVar) || (MTU_SIZE_NOT_REQUESTED == tmpVar) || (ULONG_MAX == tmpVar)))
          {
            result = VG_CME_INVALID_INPUT_VALUE;
          }
          else if ((tmpVar==ULONG_MAX) || (tmpVar==MTU_SIZE_NOT_REQUESTED))
          {
            cmd_p->psdBearerInfo.ipv4LinkMTURequest = MTU_SIZE_NOT_REQUESTED;
          }
          else
          {
            cmd_p->psdBearerInfo.ipv4LinkMTURequest = MTU_SIZE_REQUESTED;
          }
        }
        else /* error reading supplied parameter */
        {
          result = RESULT_CODE_ERROR;
        }
      }

      /* Get the next 1 parameter - ignored */
      if (result == RESULT_CODE_OK)
      {
        (void) (getExtendedParameter (commandBuffer_p, &tmpVar, ULONG_MAX));
      }

      /* Get Non-IP MTU discovery */
      if (result == RESULT_CODE_OK)
      {
        FatalAssert (cmd_p != PNULL);
        if (getExtendedParameter (commandBuffer_p, &tmpVar, ULONG_MAX))
        {
          if (!((MTU_SIZE_NOT_REQUESTED == tmpVar) || (MTU_SIZE_NOT_REQUESTED == tmpVar) || (ULONG_MAX == tmpVar)))
          {
            result = VG_CME_INVALID_INPUT_VALUE;
          }
          else if ((tmpVar==ULONG_MAX) || (tmpVar==MTU_SIZE_NOT_REQUESTED))
          {
            cmd_p->psdBearerInfo.nonIPLinkMTURequest = MTU_SIZE_NOT_REQUESTED;
          }
          else
          {
            cmd_p->psdBearerInfo.nonIPLinkMTURequest = MTU_SIZE_REQUESTED;
          }
        }
        else /* error reading supplied parameter */
        {
          result = RESULT_CODE_ERROR;
        }
      }

      if (result == RESULT_CODE_OK)
      {
        FatalAssert (cmd_p != PNULL);

        if (defd)
        {

          /* update CGPADDR information, if static IP address is present cgpaddr will keep that information */
          cmd_p->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress = cmd_p->psdBearerInfo.reqPdnAddress;

          /* this AT command defines a profile */
          cmd_p->profileDefined = TRUE;
          cmd_p->cgdcontDefined = TRUE;

#if defined (FEA_DEDICATED_BEARER)
          /* We need to update information in any secondary contexts associated with this
           * primary context.
           */
          for (cid_count = vgGpGetMinCidValue(entity); cid_count < MAX_NUMBER_OF_CIDS; cid_count++)
          {
            secondary_context_p = gprsGenericContext_p->cidUserData [cid_count];

            if ((secondary_context_p != PNULL) &&
                (secondary_context_p->profileDefined) &&
                (secondary_context_p->cgdscontDefined) &&
                (secondary_context_p->psdBearerInfo.secondaryContext) &&
                (secondary_context_p->psdBearerInfo.primaryCid == cid))
            {
              /* We have found a secondary CID which is associated with this
               * primary CID.  So update accordingly
               */
              secondary_context_p->psdBearerInfo.reqApnPresent = cmd_p->psdBearerInfo.reqApnPresent;
              memcpy (&secondary_context_p->psdBearerInfo.reqApn, &cmd_p->psdBearerInfo.reqApn, sizeof (AccessPointName));
              memcpy (&secondary_context_p->psdBearerInfo.reqTextualApn, &cmd_p->psdBearerInfo.reqTextualApn, sizeof (TextualAccessPointName));

              secondary_context_p->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress.pdnType =
                                             cmd_p->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress.pdnType;

              secondary_context_p->psdBearerInfo.flowControlType = cmd_p->psdBearerInfo.flowControlType;
            }
          }
#endif /* FEA_DEDICATED_BEARER */

        }
        else
        {
          /* Undefine cgdcont entry */
          if (cmd_p->cgdcontDefined)
          {
            vgInitialiseCgdcontData (cmd_p);
            /* Check if the profile needs to be undefined */
            vgCheckProfileUndefined (cmd_p, cid);

#if defined (FEA_DEDICATED_BEARER)
            /* We need to undefine any secondary contexts associated with this
             * context.
             */
            for (cid_count = vgGpGetMinCidValue(entity); cid_count < MAX_NUMBER_OF_CIDS; cid_count++)
            {
              secondary_context_p = gprsGenericContext_p->cidUserData [cid_count];

              if ((secondary_context_p != PNULL) &&
                  (secondary_context_p->profileDefined) &&
                  (secondary_context_p->cgdscontDefined) &&
                  (secondary_context_p->psdBearerInfo.secondaryContext) &&
                  (secondary_context_p->psdBearerInfo.primaryCid == cid))
              {
                vgInitialiseCgdscontData (secondary_context_p);
                vgInitialiseCgdcontData (secondary_context_p);
                /* Check if the profile needs to be undefined */
                vgCheckProfileUndefined (secondary_context_p, cid_count);
              }
            }
#endif /* FEA_DEDICATED_BEARER */
          }
        }
      }
      else
      {
        if (cmd_p != PNULL)
        {
          memcpy(&cmd_p->psdBearerInfo, &(gprsGenericContext_p->oldPsdBearerInfo), sizeof(VgPsdBearerInfo));
        }
      }
      break;
    }
    case EXTENDED_QUERY:    /* AT+CGDCONT?  */
    {
      viewCGDCONTAttributes (entity);
      break;
    }
    case EXTENDED_ACTION:   /* AT+CGDCONT   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);
}




/*--------------------------------------------------------------------------
*
* Function:    vgGpCGPADDR
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This functions processes the CGPADDR AT command.  The
*              outputs the address and the cid numbers that are
*              defined in the context activation profiles.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGpCGPADDR (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{

  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  ResultCode_t          result = RESULT_CODE_OK;

  PdnAddress            *cmd_p;
  Int8                  profile;
  Int32                 tmpVar;
  Boolean               firstEntry = TRUE;

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT+CGPADDR=? */
    {
      vgPrintActiveCids ((const Char*)"+CGPADDR:", entity, TRUE, TRUE);
      break;
    }
    case EXTENDED_ASSIGN:   /* AT+CGPADDR=  */
    {
      if (getExtendedParameter (commandBuffer_p, &tmpVar, ULONG_MAX))
      {
        if (tmpVar == ULONG_MAX)
        {
          /* no cids supplied, so special case, display all addr's */
          for (profile = 0; profile < MAX_NUMBER_OF_CIDS; profile++)
          {
            if ((gprsGenericContext_p->cidUserData[profile] != PNULL) &&
                (gprsGenericContext_p->cidUserData[profile]->profileDefined))
            {
              if (firstEntry)
              {
                vgPutNewLine (entity);
                firstEntry = FALSE;
              }

              viewPDNAddr (gprsGenericContext_p->cidUserData[profile]->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress.pdnType,
                           &(gprsGenericContext_p->cidUserData[profile]->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress),
                           (Int8) profile,
                           entity);
            }
          }
        }
        else if ((tmpVar >= vgGpGetMinCidValue(entity) ) &&(tmpVar < MAX_NUMBER_OF_CIDS))
        {

          if (vgAllocateRamToCid (tmpVar))
          {
            /* output the addr for the cid */
            if ((gprsGenericContext_p->cidUserData[tmpVar] != PNULL) &&
                (gprsGenericContext_p->cidUserData[tmpVar]->profileDefined))
            {
              cmd_p = &(gprsGenericContext_p->cidUserData[tmpVar]->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress);

              if (firstEntry)
              {
                vgPutNewLine (entity);
                firstEntry = FALSE;
              }
              viewPDNAddr (gprsGenericContext_p->cidUserData[tmpVar]->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress.pdnType,
                           cmd_p, (Int8)tmpVar, entity);
            }
          }
          else
          {
            result = VG_CME_INSUFFIC_RESOURCES;
          }
        }
        else
        {
          result = VG_CME_INVALID_CID_VALUE;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    case EXTENDED_QUERY:    /* AT+CGPADDR?  */
    case EXTENDED_ACTION:   /* AT+CGPADDR   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgGpCGREG
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: Executes command AT+CGREG, displaying PLMN state
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgGpCGREG (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  ResultCode_t          result = RESULT_CODE_OK;
  Int32                 param;

  switch (operation)
  {
    case EXTENDED_QUERY:     /* +CGREG? */
    {
      vgSendCGREGData (entity, TRUE, (VgCGREGMode)getProfileValue (entity, PROF_CGREG));
      break;
    }
    case EXTENDED_RANGE:     /* +CGREG=? */
    {
      vgPutNewLine (entity);
      vgPuts (entity, (const Char*)"+CGREG: (0-2)");
      break;
    }
    case EXTENDED_ASSIGN:    /* +CGREG= */
    {
      if (getExtendedParameter (commandBuffer_p,
              &param,
              VG_CGREG_NUMBER_OF_SETTINGS) == TRUE)
      {
          if (param < VG_CGREG_NUMBER_OF_SETTINGS)
          {
              result = setProfileValue (entity, PROF_CGREG, (Int8)param);
#if 0
                /* For AP do not send this */
                if(param != VG_CGREG_DISABLED)
                  vgSendCGREGData (entity, FALSE, (VgCGREGMode)param);
#endif
          }
          else
          {
              result = RESULT_CODE_ERROR;
          }
      }
      else
      {
          result = RESULT_CODE_ERROR;
      }
      break;
    }
    case EXTENDED_ACTION:
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*************************************************************************
*
* Function:     vgGpCGSMS
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description:  Allows TE to modify the default route taken by Short
*               messages (Circuit Switched or GPRS). Allowed (set)
*               values are:
*
*               0   -   GPRS_GSMS_ROUTE_GPRS        (GPRS only)
*               1   -   GPRS_GSMS_ROUTE_CS          (CS only)
*               2   -   GPRS_GSMS_ROUTE_GPRS_PREF   (GPRS if available)
*               3   -   GPRS_GSMS_ROUTE_CS_PREF     (CS if available)
*
*************************************************************************/

ResultCode_t vgGpCGSMS (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  ResultCode_t          result = RESULT_CODE_OK;
  Int32                 param;

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT+CGSMS=? */
    {
      vgPutNewLine (entity);
      vgPuts       (entity, (const Char*)"+CGSMS: (0-3)");
      break;
    }
    case EXTENDED_ASSIGN:   /* AT+CGSMS=  */
    {
      if (getExtendedParameter (commandBuffer_p, &param, GPRS_GSMS_ROUTE_CS_PREF))
      {
        /* Ensure SMS route is valid */
        if ((param == GPRS_GSMS_ROUTE_GPRS)      ||
            (param == GPRS_GSMS_ROUTE_CS)        ||
            (param == GPRS_GSMS_ROUTE_GPRS_PREF) ||
            (param == GPRS_GSMS_ROUTE_CS_PREF))
        {
          gprsGenericContext_p->defaultSmsRoute = (SmsRoute)param;
          result = vgChManContinueAction (entity, SIG_APEX_SM_WRITE_ROUTE_REQ);
        }
        else
        {
          result = VG_CME_INVALID_SMS_SERVICE_PREF;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    case EXTENDED_QUERY:    /* AT+CGSMS?  */
    {
      result = vgChManContinueAction (entity, SIG_APEX_SM_READ_ROUTE_REQ);
      break;
    }
    case EXTENDED_ACTION:   /* AT+CGSMS   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    atCommandGprsDial
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: This function executes the ATD* command.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgGpD (CommandLine_t *commandBuffer_p,
                    const VgmuxChannelNumber entity)
{
  GprsContext_t         *gprsContext_p          = ptrToGprsContext (entity);
  GeneralContext_t      *generalContext_p       = ptrToGeneralContext (entity);
  GprsGenericContext_t  *gprsGenericContext_p   = ptrToGprsGenericContext ();
  ConnectionClass_t     connectionClass;
  VgPsdStatusInfo       *psdBearerContext_p;
  AbpdPdpConnType       connType                = ABPD_CONN_TYPE_NONE;
  Char                  *dialDigits_p           = PNULL;
  Int16                 dialDigitsLen;
  Char                  *gprsDigits_p           = PNULL;
  Int16                 gprsDigitsLen;
  Int32                 cidValue;
  Int32                 l2pParam;
  ResultCode_t          result                  = RESULT_CODE_OK;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
  FatalCheck (generalContext_p != PNULL, entity, 0, 0);
#endif
  KiAllocZeroMemory (sizeof(Char)*(MAX_DIAL_STRING_SIZE + NULL_TERMINATOR_LENGTH),
                       (void **) &dialDigits_p);
  KiAllocZeroMemory (sizeof(Char)*(MAX_DIAL_STRING_SIZE + NULL_TERMINATOR_LENGTH),
                       (void **) &gprsDigits_p);

  switch (getExtendedOperation(commandBuffer_p))
  {
    case EXTENDED_ASSIGN:   /* AT*MGPRVSS=  */
    {
      /* The parser passes us commands in the format:
       *  AT*MGPRVSS="dial num",l2p,cid
       */
      if (getExtendedString(commandBuffer_p,
                            dialDigits_p,
                            MAX_DIAL_STRING_SIZE,
                            &dialDigitsLen))
      {
        (void)getExtendedParameter(commandBuffer_p, &l2pParam, GPRS_PARAM_NOT_SUPPLIED);
        (void)getExtendedParameter(commandBuffer_p, &cidValue, GPRS_PARAM_NOT_SUPPLIED);

        /* Attempt to activate CID.... */
        if (cidValue != GPRS_PARAM_NOT_SUPPLIED)
        {
          if (vgAssignNewActiveContext((Int8)cidValue, entity) == FALSE)
          {
            result = RESULT_CODE_ERROR;
          }
        }
        else
        {
          /* get a free cid to use.  We can use one which has been
           * activated with AT+CGACT too and then we will activate
           * a data connection for it below
           */
          if (vgGetFreeCidForDataConn (&cidValue, entity) == TRUE)
          {
            if (vgAssignNewActiveContext((Int8)cidValue, entity) == FALSE)
            {
              result = RESULT_CODE_ERROR;
            }
            else
            {
              result = RESULT_CODE_OK;
            }
          }
          else
          {
            result = VG_CME_NO_FURTHER_CIDS_SUPPORTED;
          }
        }

        if (getExtendedString(commandBuffer_p,
                              &gprsDigits_p[0],
                              MAX_DIAL_STRING_SIZE,
                              &gprsDigitsLen))
        {
          /* Do  nothing */
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
        if (result == RESULT_CODE_OK)
        {
          /* All PSD dial strings have at least a '#' character.... */
          if (dialDigitsLen > 0)
          {
            switch (l2pParam)
            {
#if defined (FEA_PPP)
              case VG_SS_L2P_CORE_PPP_PROTOCOL:
              case VG_SS_L2P_PPP_PROTOCOL:
                connectionClass = PPP_CONNECTION;
                break;
#endif /* FEA_PPP*/
              case VG_SS_L2P_PACKET_TRANSPORT_PROTOCOL:
                connectionClass = PT_CONNECTION;
                break;
              default:
#if defined (FEA_PPP)
                /* Just set it to PPP as default */
                l2pParam = VG_SS_L2P_PPP_PROTOCOL;
                connectionClass = PPP_CONNECTION;
#else
                l2pParam = VG_SS_L2P_PACKET_TRANSPORT_PROTOCOL;
                connectionClass = PT_CONNECTION;
#endif /* FEA_PPP*/
                break;
            }

            /* Note that activePsdBearerContextWithDataConn pointer was already asssigned
             * in the call to vgAssignNewActiveContext above
             */
            psdBearerContext_p = gprsContext_p->activePsdBearerContextWithDataConn;
#if defined (ATCI_SLIM_DISABLE)

            FatalCheck (psdBearerContext_p != PNULL, entity, cidValue, 0);
#endif
#if defined (FEA_DEDICATED_BEARER)
            if (psdBearerContext_p->psdBearerInfo.secondaryContext == TRUE)
            {
              /* This is a secondary context - so we need to check if
               * the primary context / default bearer is active and has a connId - if
               * it doesn't then we cannot proceed.
               * In addition This is applicable ONLY to 2G/3G because we cannot dial
               * a dedicated bearer for LTE this way - we can only use AT+CGACT.
               * We do this by checking the current psdMode.  If it is legacy - then this
               * is permitted.
               */
              result = VG_CME_OPERATION_NOT_ALLOWED;
            }
            else
#endif /* FEA_DEDICATED_BEARER */
            if(!psdBearerContext_p->profileDefined && !psdBearerContext_p->cgdcontDefined)
            {
                psdBearerContext_p->psdBearerInfo.reqApnPresent = gprsGenericContext_p->currentDefaultAPN.apnPresent;
                psdBearerContext_p->psdBearerInfo.reqApn        = gprsGenericContext_p->currentDefaultAPN.apn;
                psdBearerContext_p->psdBearerInfo.reqTextualApn = gprsGenericContext_p->currentDefaultAPN.textualApn;
                psdBearerContext_p->psdBearerInfo.reqPdnAddress.pdnType
                                      = gprsGenericContext_p->currentDefaultAPN.pdnType;
                psdBearerContext_p->psdBearerInfo.psdUser       = gprsGenericContext_p->currentDefaultAPN.psdUser;
            }

            /* test to see if PSD connection is permissable */
            /* Allow connection if a connection can be allocated */
            /* (i.e. there is no existing PSDconnection or nothing that */
            /* prevents one) */
            /* we may not be RESULT_CODE_OK because of a failed secondary
             * context.  (ATD can only be done for connections with a
             * connection type requiring a data connection: PT or PPP).
             */
            if (result == RESULT_CODE_OK)
            {
              switch (l2pParam)
              {
#if defined (FEA_PPP)
                case VG_SS_L2P_CORE_PPP_PROTOCOL:
                  /* Core-PPP */
                 connType  = ABPD_CONN_TYPE_CORE_PPP;
                  break;
#endif /* FEA_PPP */
                case VG_SS_L2P_PACKET_TRANSPORT_PROTOCOL:
                  /* Packet transport */
                  connType  = ABPD_CONN_TYPE_PACKET_TRANSPORT;
                  break;
                default:
#if defined (FEA_PPP)
                  /* Standard PPP */
                  connType  = ABPD_CONN_TYPE_PPP;
#else
                  /* Packet transport */
                  connType  = ABPD_CONN_TYPE_PACKET_TRANSPORT;
#endif /* FEA_PPP */

                  break;
              }

              if (psdBearerContext_p->isActive == TRUE)
              {
                /* We allow special case of activating a data connection already active
                 * when it was previously activated with ABPD_CONN_TYPE_NONE and now
                 * the user want to connect a data connection (i.e. Packet Transport
                 * or PPP).  The signal sent to ABPD is SIG_APEX_ABPD_ACTIVATE_DATA_CONN_REQ
                 * rather than SIG_APEX_ABPD_DIAL_REQ.
                 */
                if ((connType != ABPD_CONN_TYPE_NONE) &&
                     (psdBearerContext_p->psdBearerInfo.connType == ABPD_CONN_TYPE_NONE))
                {
                  if (vgOpManAllocateConnection (entity, connectionClass))
                  {
                    /* reset last call release error */
                    vgSetApexGsmCallReleaseError (APEX_CAUSE_OK, TRUE, CAUSE_NORMAL_CLEARING, PD_SM, entity);
                    psdBearerContext_p->psdBearerInfo.connType = connType;
                    gprsContext_p->vgDialCid = (Int8)cidValue;
                    result = vgChManContinueAction (entity, SIG_APEX_ABPD_ACTIVATE_DATA_CONN_REQ);
                  }
                  else
                  {
                    result = VG_CME_PHONE_LINK_RESERVED;
                  }
                }
                /* Trying to connect when already connected with a different
                 * connection type - so error case.
                 */
                else if (connType != psdBearerContext_p->psdBearerInfo.connType)
                {
                  result = VG_CME_OPERATION_NOT_ALLOWED;
                }
                else
                {
                  /* The context with this CID is already active so just say OK
                   */
                  result = RESULT_CODE_OK;
                }
              }
              /* We don't check if the connection is permitted for ABPD_CONN_TYPE_NONE
               * we only check for PT or PPP as activations with AT+CGACT=1 are always
               * permitted on a channel as long as the CID is not already active
               */
              else if (vgOpManAllocateConnection (entity, connectionClass))
              {
                /* reset last call release error */
                vgSetApexGsmCallReleaseError (APEX_CAUSE_OK, TRUE, CAUSE_NORMAL_CLEARING, PD_SM, entity);
                psdBearerContext_p->psdBearerInfo.connType = connType;
                gprsContext_p->vgDialCid = (Int8)cidValue;
                result = vgChManContinueAction (entity, SIG_APEX_ABPD_DIAL_REQ);
              }
              else
              {
                result = VG_CME_PHONE_LINK_RESERVED;
              }
            }
          }
          else
          {
            result = VG_CME_INVALID_DIALSTRING_CHARS;
          }
        }
      }
      else
      {
        result = VG_CME_INVALID_DIALSTRING_CHARS;
      }
      break;
    }

    case EXTENDED_QUERY:    /* AT*MGPRVSS?  */
    case EXTENDED_RANGE:    /* AT*MGPRVSS=? */
    case EXTENDED_ACTION:   /* AT*MGPRVSS   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  KiFreeMemory( (void**)&dialDigits_p);
  KiFreeMemory( (void**)&gprsDigits_p);

  return (result);
}

#if defined (FEA_QOS_TFT)
/*--------------------------------------------------------------------------
 *
 * Function:    vgGpCGTFT
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: Executes command AT+CGTFT
 *
 * Note:        The 'source address and subnet mask', 'destination port range',
 *              and 'source port range' referred to in 27.007 spec
 *              correspond to 'Remote Address and Subnet Mask', 'Local Port Range',
 *              and 'Remote Port Range' respectively in the type definitions - this
 *              is the convention in 23.060 and 24.008 specs in Rel-7 and later versions
 *              and has been adapted for both Pre Rel-7 and Post Rel-7 code.
 *
 *     Pre Rel-7                                Post (including) Rel-7 (default usage)
 *     =========                                ======================================
 *     Source Address and Subnet Mask           Remote Address and Subnet Mask
 *     Destination Port Range                   Local Port Range
 *     Source Port Range                        Remote Port Range
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgGpCGTFT (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t          result                                  = RESULT_CODE_OK;
  Int32                 cid, tmpVal;

  VgPsdStatusInfo       *cmd_p                                  = PNULL;
  TrafficFlowTemplate   *oldTft_p                               = PNULL;
  Boolean               found                                   = FALSE;
  Boolean               packetFiltersPresent                    = FALSE;
  GprsGenericContext_t  *gprsGenericContext_p                   = ptrToGprsGenericContext ();
  GprsContext_t         *gprsContext_p                          = ptrToGprsContext (entity);
  Char                  tmpString [MAX_REMOTE_ADDR_AND_SUBNETMASK + NULL_TERMINATOR_LENGTH]  = {0}; /* Use this as it is long enough */
  Int16                 tmpStringLen                            = 0;
  Boolean               tmpBool                                 = FALSE;
  PacketFilter          *packetFilter_p                         = PNULL;
  TrafficFlowTemplate   *newTft_p                               = PNULL;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
#endif
  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_ASSIGN:    /* +CGTFT= */
    {
      /* get the cid */
      if (getExtendedParameter (commandBuffer_p, &cid, ULONG_MAX))
      {
        /* if no cid found then cid will = ulong_max */
        /* job132261: correct CID range limit */
        if ((cid >= vgGpGetMinCidValue(entity)) && (cid < MAX_NUMBER_OF_CIDS))
        {
          if (vgAllocateRamToCid (cid))
          {
            cmd_p = gprsGenericContext_p->cidUserData [cid];

            KiAllocMemory (sizeof(TrafficFlowTemplate), (void **)&oldTft_p);

            /* save old values to reinstate later if change unsuccessful */
            FatalAssert (cmd_p != PNULL);
            memcpy(oldTft_p, &cmd_p->psdBearerInfo.requiredTft, sizeof(TrafficFlowTemplate));
            newTft_p = &cmd_p->psdBearerInfo.requiredTft;
            cmd_p->psdBearerInfo.reqTftPresent = TRUE;
            newTft_p->tftOpCode = TFT_OPCODE_SPARE;
          }
          else
          {
            result = VG_CME_INSUFFIC_RESOURCES;
          }
        }
        else
        {
          result = VG_CME_INVALID_CID_VALUE;
        }
      }
      else
      {
        /* you must supply a cid */
        result = RESULT_CODE_ERROR;
      }

      /* now get the TFT data */
      /* get packet filter identifier */
      if (result == RESULT_CODE_OK)
      {
        FatalAssert (cmd_p != PNULL);
        FatalAssert (newTft_p != PNULL);

        if (getExtendedParameterPresent (commandBuffer_p, &tmpVal, ULONG_MAX, &tmpBool))
        {
          /* TODO: the tftOpCode will be dynamically determined when needed, is it
                   need to set here? */
          if (tmpBool)
          {
            if ((tmpVal >= MIN_PFI) && (tmpVal <= MAX_PFI))
            {
              Int8 i;
              /* Check if PFI has been defined before */
              for (i = 0; i < MAX_PFS_IN_TFT; i++)
              {
                if (newTft_p->packetFilterData[i].packetFilterId == (Int8) tmpVal)
                {
                  /* Existing PFI. */
                  packetFilter_p = &newTft_p->packetFilterData[i];
                  memset (packetFilter_p, 0, sizeof(PacketFilter));
                  packetFilter_p->packetFilterId = (Int8) tmpVal;
                  newTft_p->tftOpCode = TFT_OPCODE_REPLACE_PACKET_FILTERS; /* For now */
                  break;
                }
              }

              if (packetFilter_p == PNULL)
              {
                /* PFI has not been defined before. Check if there is room for a new one. */
                for (i = 0; i < MAX_PFS_IN_TFT; i++)
                {
                  if (newTft_p->packetFilterData[i].packetFilterId == 0)
                  {
                    /* Check that we didn't already assign it to a slot */
                    if (!found)
                    {
                      /* Found an available slot. Use it. */
                      packetFilter_p = &newTft_p->packetFilterData[i];
                      memset (packetFilter_p, 0, sizeof(PacketFilter)); /* To be sure */
                      packetFilter_p->packetFilterId = (Int8) tmpVal;

                      /* Increment the number of PFIs we have.
                       * This never gets decrements - we can only delete all of them
                       * using AT.
                       */
                      newTft_p->numPacketFilters++;

                      newTft_p->tftOpCode = TFT_OPCODE_ADD_PACKET_FILTERS;
                      found = TRUE;
                    }
                  }
                  else
                  {
                    packetFiltersPresent = TRUE;
                  }
                }
                /* There were no other packet filters - so this is a new TFT */
                if (!packetFiltersPresent)
                {
                  newTft_p->tftOpCode = TFT_OPCODE_CREATE_NEW;
                }
              }
              if (packetFilter_p == PNULL)
              {
                /* Trying to define more than MAX_PFS_IN_TFT pf's */
                result = RESULT_CODE_ERROR;
              }
            }
            else
            {
              result = VG_CME_INVALID_INPUT_VALUE;
            }
          }
          else
          {
            /* No pfi means delete tft */
            newTft_p->tftOpCode = TFT_OPCODE_DELETE_EXISTING;
            memset (newTft_p->packetFilterData, 0, sizeof (newTft_p->packetFilterData));
            newTft_p->numPacketFilters = 0;
          }
        }
        else
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }

      /* Now get additional parameters for NASMDL2 */
      /* get evaluation precedence index */
      if ((newTft_p != PNULL) && (newTft_p->tftOpCode != TFT_OPCODE_DELETE_EXISTING) && (result == RESULT_CODE_OK))
      {
        FatalAssert (newTft_p != PNULL);

        if (getExtendedParameterPresent (commandBuffer_p, &tmpVal, ULONG_MAX, &tmpBool))
        {
          if (tmpBool)
          {
            if (tmpVal <= MAX_EVAL_PREC_IND)
            {
              Int8 i;
              FatalAssert (packetFilter_p != PNULL);
              for (i = 0; i < MAX_PFS_IN_TFT; i++)
              {
                /* Check if another pfi is using the same eval. prec. index */
                if ((newTft_p->packetFilterData[i].packetFilterId != 0) &&
                    (newTft_p->packetFilterData[i].packetFilterId != packetFilter_p->packetFilterId) &&
                    (newTft_p->packetFilterData[i].evalPrecedenceIndex == (Int8) tmpVal))
                {
                  result = RESULT_CODE_ERROR;
                  break;
                }
              }

              if (result == RESULT_CODE_OK)
              {
                packetFilter_p->evalPrecedenceIndex = (Int8) tmpVal;
              }
            }
            else
            {
              result = VG_CME_INVALID_INPUT_VALUE;
            }
          }
          else
          {
            /* If PFI is present then we must have the evaluation precedence
             * index present too
             */
            result = VG_CME_INVALID_INPUT_VALUE;
          }
          /* For NASMDL2/LTE - we do not allow the special case of deleting a specific
           * packet filter as we need to use the packet filter ID of 0 in the stack
           */
        }
        else
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }

      /* get the source address and subnet mask */
      if ((newTft_p != PNULL) && (newTft_p->tftOpCode != TFT_OPCODE_DELETE_EXISTING) && (result == RESULT_CODE_OK))
      {
        if (getExtendedString (commandBuffer_p,
                               tmpString,
                               MAX_REMOTE_ADDR_AND_SUBNETMASK,
                               &tmpStringLen))
        {
          if (tmpStringLen > 0)
          {
            FatalAssert (packetFilter_p != PNULL);
            memcpy (packetFilter_p->remoteAddrSubnetMask.val,
                    tmpString,
                    tmpStringLen);
            /* Make sure we put a \0 at the end */
            packetFilter_p->remoteAddrSubnetMask.val[tmpStringLen] = '\0';
            packetFilter_p->remoteAddrSubnetMask.length = (Int8) tmpStringLen;
            packetFilter_p->remoteAddrSubnetMask.present = TRUE;

            /* Now convert to numeric */
            if (!vgConvertTextualRemoteAddressSubnetMaskToNumeric(&packetFilter_p->remoteAddrSubnetMask, entity))
            {
              /* Didn't convert so something is wrong with the input string */
              result = VG_CME_INVALID_INPUT_VALUE;
            }
          }
          memset (tmpString, NULL_CHAR, sizeof (tmpString));
        }
        else /* error reading supplied parameter */
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }

      /* get protocol number / next header */
      if ((newTft_p != PNULL) && (newTft_p->tftOpCode != TFT_OPCODE_DELETE_EXISTING) && (result == RESULT_CODE_OK))
      {
        if (getExtendedParameterPresent (commandBuffer_p, &tmpVal, ULONG_MAX, &tmpBool))
        {
          if (tmpBool)
          {
            FatalAssert (packetFilter_p != PNULL);
            if (tmpVal <= MAX_PROTO_NUM)
            {
              packetFilter_p->protocolNumNextHdr.val = (Int8) tmpVal;
              packetFilter_p->protocolNumNextHdr.present = TRUE;
            }
            else
            {
              result = VG_CME_INVALID_INPUT_VALUE;
            }
          }
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
      }

      /* get destination port range */
      if ((newTft_p != PNULL) && (newTft_p->tftOpCode != TFT_OPCODE_DELETE_EXISTING) && (result == RESULT_CODE_OK))
      {
        if (getExtendedString (commandBuffer_p,
                               tmpString,
                               MAX_PORT_RANGE,
                               &tmpStringLen))
        {
          if (tmpStringLen > 0)
          {
            FatalAssert (packetFilter_p != PNULL);
            memcpy (packetFilter_p->localPortRange.val,
                    tmpString,
                    tmpStringLen);
            /* Make sure we put a \0 at the end */
            packetFilter_p->localPortRange.val[tmpStringLen] = '\0';
            packetFilter_p->localPortRange.length = (Int8) tmpStringLen;
            packetFilter_p->localPortRange.present = TRUE;
            /* Now convert to numeric */
            if (!vgConvertTextualPortRangeToNumeric(&packetFilter_p->localPortRange))
            {
              /* Didn't convert so something is wrong with the input string */
              result = VG_CME_INVALID_INPUT_VALUE;
            }
          }
          memset (tmpString, NULL_CHAR, sizeof (tmpString));
        }
        else /* error reading supplied parameter */
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }

      /* get source port range */
      if ((newTft_p != PNULL) && (newTft_p->tftOpCode != TFT_OPCODE_DELETE_EXISTING) && (result == RESULT_CODE_OK))
      {
        if (getExtendedString (commandBuffer_p,
                               tmpString,
                               MAX_PORT_RANGE,
                               &tmpStringLen))
        {
          if (tmpStringLen > 0)
          {
            FatalAssert (packetFilter_p != PNULL);
            memcpy (packetFilter_p->remotePortRange.val,
                    tmpString,
                    tmpStringLen);
            /* Make sure we put a \0 at the end */
            packetFilter_p->remotePortRange.val[tmpStringLen] = '\0';
            packetFilter_p->remotePortRange.length = (Int8) tmpStringLen;
            packetFilter_p->remotePortRange.present = TRUE;

            /* Now convert to numeric */
            if (!vgConvertTextualPortRangeToNumeric(&packetFilter_p->remotePortRange))
            {
              /* Didn't convert so something is wrong with the input string */
              result = VG_CME_INVALID_INPUT_VALUE;
            }
          }
          memset (tmpString, NULL_CHAR, sizeof (tmpString));
        }
        else /* error reading supplied parameter */
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }

      /* get ipsec spi */
      if ((newTft_p != PNULL) && (newTft_p->tftOpCode != TFT_OPCODE_DELETE_EXISTING) && (result == RESULT_CODE_OK))
      {
        if (getExtendedHexParameter (commandBuffer_p, &tmpVal, ULONG_MAX, &tmpBool))
        {
          if (tmpBool)
          {
            FatalAssert (packetFilter_p != PNULL);

            packetFilter_p->ipsecSpi.val = tmpVal;
            packetFilter_p->ipsecSpi.present = TRUE;
          }
        }
        else
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }

      /* get type of service / traffic class */
      if ((newTft_p != PNULL) && (newTft_p->tftOpCode != TFT_OPCODE_DELETE_EXISTING) && (result == RESULT_CODE_OK))
      {
        if (getExtendedString (commandBuffer_p,
                               tmpString,
                               MAX_TOS_TRFCCLSS,
                               &tmpStringLen))
        {
          if (tmpStringLen > 0)
          {
            FatalAssert (packetFilter_p != PNULL);
            memcpy (packetFilter_p->tosTrfcClass.val,
                    tmpString,
                    tmpStringLen);
            /* Make sure we put a \0 at the end */
            packetFilter_p->tosTrfcClass.val[tmpStringLen] = '\0';
            packetFilter_p->tosTrfcClass.length = (Int8) tmpStringLen;
            packetFilter_p->tosTrfcClass.present = TRUE;

            /* Now convert to numeric */
            if (!vgConvertTextualTosTrfcClassToNumeric(&packetFilter_p->tosTrfcClass))
            {
              /* Didn't convert so something is wrong with the input string */
              result = VG_CME_INVALID_INPUT_VALUE;
            }

          }
          memset (tmpString, NULL_CHAR, sizeof (tmpString));
        }
        else /* error reading supplied parameter */
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }

      /* get flow label */
      if ((newTft_p != PNULL) && (newTft_p->tftOpCode != TFT_OPCODE_DELETE_EXISTING) && (result == RESULT_CODE_OK))
      {
        if (getExtendedHexParameter (commandBuffer_p, &tmpVal, ULONG_MAX, &tmpBool))
        {
          if (tmpBool)
          {
            if (tmpVal <= MAX_FLOW_LABEL)
            {
              FatalAssert (packetFilter_p != PNULL);
              packetFilter_p->flowLabel.val = tmpVal;
              packetFilter_p->flowLabel.present = TRUE;
            }
            else
            {
              result = VG_CME_INVALID_INPUT_VALUE;
            }
          }
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
      }

      /* get direction */
      if ((newTft_p != PNULL) && (newTft_p->tftOpCode != TFT_OPCODE_DELETE_EXISTING) && (result == RESULT_CODE_OK))
      {
        if (getExtendedParameterPresent(commandBuffer_p, &tmpVal, ULONG_MAX, &tmpBool))
        {
          if (tmpBool)
          {
            if (tmpVal <= PF_DIR_BIDIRECTIONAL)
            {
              FatalAssert (packetFilter_p != PNULL);
              /* the coding scheme for AT command and NAS signalling is not the same */
              if (1 == tmpVal)
              {
                packetFilter_p->packetFilterDirection = PF_DIR_UPLINK_ONLY;
              }
              else if (2 == tmpVal)
              {
                packetFilter_p->packetFilterDirection = PF_DIR_DOWNLINK_ONLY;
              }
              else
              {

                packetFilter_p->packetFilterDirection = (PacketFilterDirection)tmpVal;
              }
            }
            else
            {
              result = VG_CME_INVALID_INPUT_VALUE;
            }
          }
          /* else it has not been supplied by the user - default is bi-directional */
          else if (packetFilter_p != PNULL)
          {
            packetFilter_p->packetFilterDirection = PF_DIR_BIDIRECTIONAL;
          }
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
      }

      /* Check the combination of packet filters according to 23.060 15.3.2 */
      if ((result == RESULT_CODE_OK) && (packetFilter_p != PNULL))
      {
        FatalAssert (newTft_p != PNULL);

        if ((packetFilter_p->localPortRange.present) ||
            (packetFilter_p->remotePortRange.present))
        {
          /* Combination I */
          if ((packetFilter_p->ipsecSpi.present) ||
              (packetFilter_p->flowLabel.present))
          {
            result = RESULT_CODE_ERROR;
          }
        }
        else if (packetFilter_p->ipsecSpi.present)
        {
          /* Combination II */
          if (packetFilter_p->flowLabel.present)
          {
            result = RESULT_CODE_ERROR;
          }
        }
        else if (packetFilter_p->flowLabel.present)
        {
          /* Combination III */
          if (packetFilter_p->protocolNumNextHdr.present)
          {
            result = RESULT_CODE_ERROR;
          }
        }
        else
        {
          /* Could be any combination. Something must be present. */
          if ((!packetFilter_p->remoteAddrSubnetMask.present) &&
              (!packetFilter_p->protocolNumNextHdr.present) &&
              (!packetFilter_p->tosTrfcClass.present))
          {
            /* Except for delete operations */
            if ((newTft_p->tftOpCode != TFT_OPCODE_DELETE_EXISTING) &&
                (newTft_p->tftOpCode != TFT_OPCODE_DELETE_PACKET_FILTERS))
            {
              result = RESULT_CODE_ERROR;
            }
          }
        }
      }

      /*
        At this point the opcode must have been determined. Otherwise it is
        a programming error.
      */
      if (result == RESULT_CODE_OK)
      {
        FatalAssert (newTft_p != PNULL);

        if (newTft_p->tftOpCode == TFT_OPCODE_SPARE)
        {
          /* If it is still spare, it must have failed */
          /* Opcode not determined */
          WarnParam(entity, newTft_p->tftOpCode, 0);
          result = RESULT_CODE_ERROR;
        }
      }

      if (result == RESULT_CODE_OK)
      {
        FatalAssert (cmd_p != PNULL);
        FatalAssert (newTft_p != PNULL);

        /* this AT command defines a profile */
        cmd_p->profileDefined = TRUE;
        cmd_p->cgtftDefined = TRUE;

        if((vgOpManCidActive((Int8)cid)) && (cmd_p->modifyPending == FALSE))
        {
           FatalAssert (oldTft_p != PNULL);
           cmd_p->modifyPending = TRUE;
        }
      }
      else
      {
        /* Restore previous profile values */
        if (cmd_p != PNULL)
        {
          FatalAssert (oldTft_p != PNULL);
          memcpy(&cmd_p->psdBearerInfo.requiredTft, oldTft_p, sizeof(TrafficFlowTemplate));
        }
      }

      if (oldTft_p != PNULL)
      {
         KiFreeMemory ((void**)&oldTft_p);
      }
      break;
    }
    case EXTENDED_RANGE:    /* AT+CGTFT=? */
    {
      vgView3gTftRange(entity);
      break;
    }
    case EXTENDED_QUERY:    /* AT+CGTFT?  */
    {
      vgView3gTftAttributes(entity);
      break;
    }
    case EXTENDED_ACTION:   /* AT+CGTFT */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgGpCGCMOD
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: Executes command AT+CGCMOD
 *-------------------------------------------------------------------------*/
ResultCode_t vgGpCGCMOD (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
  ResultCode_t          result  = RESULT_CODE_OK;
  Int32                 cid     = CID_NUMBER_UNKNOWN;

  Int32                 cidCount;
  VgPsdStatusInfo       *cmd_p  = PNULL;

  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  GprsContext_t         *gprsContext_p = ptrToGprsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsGenericContext_p != PNULL, entity, 0, 0);
  FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
#endif
  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_ASSIGN:    /* +CGCMOD= */
    {

        /* get the cid */
        if (getExtendedParameter (commandBuffer_p, &cid, ULONG_MAX))
        {
          if (cid == ULONG_MAX)
          {

            /* If we have a cid which is in data mode use that */
            if (gprsContext_p->activePsdBearerContextWithDataConn != PNULL)
            {
              cid = gprsContext_p->activePsdBearerContextWithDataConn->cid;
            }
            else
            {
              /* otherwise pick the first available CID which is assigned to this channel */
              for (cidCount = vgGpGetMinCidValue(entity); (cidCount < MAX_NUMBER_OF_CIDS) && (cid == ULONG_MAX); cidCount++)
              {
                if ((gprsGenericContext_p->cidOwner[cidCount] == entity) &&
                    (vgOpManCidActive((Int8)cidCount) == TRUE))
                {
                  cid = cidCount;
                }
              }
            }
          }

          /* if no cid found then cid will = ulong_max */
          /* job132261: correct CID range limit */
          if ((cid >= vgGpGetMinCidValue(entity)) && (cid < MAX_NUMBER_OF_CIDS))
          {

            if (vgAllocateRamToCid ( cid ))
            {
              cmd_p = gprsGenericContext_p->cidUserData [cid];

              /* We only trigger a modification if something has actually changed.  Otherwise
               * we do nothing and send an error.  For LTE also - we only trigger
               * a modification for a secondary context
               */
              if (cmd_p->profileDefined && vgOpManCidActive((Int8)cid))
              {
                cmd_p->modifyPending = FALSE;

                /* Make sure we record the fact that we are starting a modification
                 * procedure
                 */
                cmd_p->vgModifyChannel = entity;
                gprsContext_p->vgModifyCid = (Int8)cid;
                result = vgChManContinueAction (entity, SIG_APEX_ABPD_PSD_MODIFY_REQ);
              }
              else
              {
                result = RESULT_CODE_ERROR;
              }
            }
            else
            {
              result = VG_CME_INSUFFIC_RESOURCES;
            }
          }
          else
          {
            result = VG_CME_INVALID_CID_VALUE;
          }
        }
        else
        {
          /* you must supply a valid cid */
          result = RESULT_CODE_ERROR;
        }

        break;
    }
    case EXTENDED_RANGE:     /* +CGCMOD=? */
    {
      vgPrintActiveCids ((const Char*)"+CGCMOD: ", entity, TRUE, TRUE);
      break;
    }
    case EXTENDED_ACTION:    /* +CGCMOD */
    case EXTENDED_QUERY:     /* +CGCMOD? */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);
}

#endif /* FEA_QOS_TFT */

#if defined (FEA_DEDICATED_BEARER)
/*************************************************************************
*
* Function:     viewCGDSCONTRange
*
* Parameters:   prompt_p - pointer to the header to be displayed
*               entity   - mux channel number
*
* Returns:      nothing
*
* Description:  Outputs the ranges for the PDP secondary context attributes
*
*************************************************************************/

static void viewCGDSCONTRange (const VgmuxChannelNumber entity)
{

  VgPsdStatusInfo            *cmd_p;
  GprsGenericContext_t       *gprsGenericContext_p = ptrToGprsGenericContext ();
  Int8                       profile;
  Boolean                    firstPrimaryPdp = TRUE;

      vgPutNewLine (entity);
      /* Start printing: range of cids supported */
      vgPrintf (entity, (const Char*)"+CGDSCONT: ");
      /* job132261: print correct CID value */
      vgPrintf (entity,
                (const Char*)"(%d-%d),(",
                 vgGpGetMinCidValue(entity),
                 (MAX_NUMBER_OF_CIDS - 1));

      /* Print primary PDP contexts - note: even if inactive */
      /* job132261: CID index starts at 1 */
      for (profile = vgGpGetMinCidValue(entity); profile < MAX_NUMBER_OF_CIDS; profile++)
      {
        /* check user data is present */
        if (gprsGenericContext_p->cidUserData [profile] != PNULL)
        {
          cmd_p = gprsGenericContext_p->cidUserData [profile];

          if ((cmd_p->profileDefined) &&
              (!cmd_p->psdBearerInfo.secondaryContext == TRUE)) /* Primary */
          {
            if (firstPrimaryPdp)
            {
              /* job132261: print correct CID value */
              vgPrintf (entity, (const Char*)"%d", profile);
              firstPrimaryPdp = FALSE;
            }
            else
            {
              /* job132261: print correct CID value */
              vgPrintf (entity, (const Char*)",%d", profile);
            }
          }
        }
      }

      /* PDP type and d_comp, h_comp */
      vgPrintf (entity,
                (const Char*)"),(%d-%d),(%d-%d)",
                (Int32)DATA_COMP_OFF,
                (Int32)DATA_COMP_V42BIS,
                (Int32)HEADER_COMP_OFF,
                (Int32)HEADER_COMP_RFC3095);
  vgPutNewLine (entity);
}

/*************************************************************************
*
* Function:     viewCGDSCONTAttributes
*
* Parameters:   prompt_p - pointer to the header for AT output
*               entity   - mux channel number
*
* Returns:      nothing
*
* Description:  outputs the current values for each of the secondary
*               context profiles.
*
*************************************************************************/

static void viewCGDSCONTAttributes (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();

  VgPsdStatusInfo      *cmd_p;
  Int8                 profile;

  /* go through all profiles */
  /* job132261: CID index starts at 1 */
  for (profile = vgGpGetMinCidValue(entity); profile < MAX_NUMBER_OF_CIDS; profile++)
  {
    /* check user data is present */
    if (gprsGenericContext_p->cidUserData[profile] != PNULL)
    {
      cmd_p = gprsGenericContext_p->cidUserData [profile];

      /* if the profile is defined then display attributes */
      if ((cmd_p->profileDefined) &&
          (cmd_p->cgdscontDefined) &&
          (cmd_p->psdBearerInfo.secondaryContext))
      {
        vgPutNewLine (entity);
        vgPrintf (entity, (const Char*)"+CGDSCONT: ");

        vgPrintf (entity,
                  (const Char*)"%d,%d,%d,%d",
                    /* job132261: print correct CID value */
                    profile,
                     cmd_p->psdBearerInfo.primaryCid,
                      cmd_p->psdBearerInfo.dataComp,
                       cmd_p->psdBearerInfo.headerComp);
      }
    }
  }
  vgPutNewLine (entity);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGpCGDSCONT
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Defines a secondary PDP context
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGpCGDSCONT (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  ResultCode_t          result = RESULT_CODE_OK;

  VgPsdStatusInfo       *cmd_p = PNULL;
  VgPsdStatusInfo       *p_cmd_p = PNULL;
  Int32                 tmpVar;
  Boolean               tmpBool = FALSE;
  Int32                 cid, p_cid;
  Boolean               defd = FALSE;

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT+CGDSCONT=? */
    {
      viewCGDSCONTRange (entity);
      break;
    }
    case EXTENDED_ASSIGN:   /* AT+CGDSCONT=  */
    {
      /* get the CID */
      if (getExtendedParameter (commandBuffer_p, &cid, ULONG_MAX))
      {
        /* if the cid is invalid or does not exist; tmpVar = default
           value of ULONG_MAX then error. */
        /* job132261: correct upper CID range value */
        if ((cid >= vgGpGetMinCidValue(entity)) && (cid < MAX_NUMBER_OF_CIDS))
        {
          if (vgAllocateRamToCid (cid))
          {
            cmd_p = gprsGenericContext_p->cidUserData [cid];

            /* save old values to reinstate later if change unsuccessful */
            memcpy (&(gprsGenericContext_p->oldPsdBearerInfo), &cmd_p->psdBearerInfo, sizeof(VgPsdBearerInfo));
          }
          else
          {
            result = VG_CME_INSUFFIC_RESOURCES;
          }
        }
        else
        {
          result = VG_CME_INVALID_CID_VALUE;
        }
      }
      else
      {
        /* must supply at least one cid */
          result = RESULT_CODE_ERROR;
      }

      if (result == RESULT_CODE_OK)
      {
        /* Check whether cid is in use by another entity */
        if (vgOpManCidAvailable ((Int8)cid, entity) == FALSE)
        {
          result = VG_CME_CID_NOT_AVAILABLE;
        }
      }

      if (result == RESULT_CODE_OK)
      {
        /* Check that there isn't already an active context */
        if (vgOpManCidActive ((Int8)cid))
        {
          /* Just check if the Primary PSD bearer it is associated with
           * is what is already stored for this connection
           */

          /* For NB-IOT we need to check if we are allowed to activate default
           * PDN connection on attach - before doing anything (set by AT+CIPCA)
           */
          if ((getProfileValue(entity, PROF_MLTEGCF) != GCF_TEST_MODE_DISABLE) &&
               vgCIPCAPermitsActivateAttachDefBearer() &&
              (getExtendedParameterPresent (commandBuffer_p, &p_cid, ULONG_MAX, &tmpBool)))
          {
            if (((tmpBool) && (cmd_p != PNULL) && (p_cid != cmd_p->psdBearerInfo.primaryCid)) ||
                (!tmpBool))
            {
              result = VG_CME_CID_ALREADY_ACTIVE;
            }
          }
          else
          {
            result = VG_CME_CID_ALREADY_ACTIVE;
          }
        }
      }

      if ((result == RESULT_CODE_OK) && (cmd_p !=PNULL))
      {
        /* Check that the context has not been defined by cgdcont */
        if (cmd_p->cgdcontDefined)
        {
          result = RESULT_CODE_ERROR;
        }
      }

      /* get the p_CID */
      if ((result == RESULT_CODE_OK) && (!vgOpManCidActive ((Int8)cid)))
      {
        FatalAssert (cmd_p != PNULL);

        if (getExtendedParameterPresent (commandBuffer_p, &p_cid, ULONG_MAX, &tmpBool))
        {
          if (tmpBool)
          {
            /* if the p_cid is invalid then error. */
            /* job132261: correct upper CID range value */
            if ((p_cid >= vgGpGetMinCidValue(entity)) && (p_cid < MAX_NUMBER_OF_CIDS) && (p_cid != cid))
            {
              /* Check if primary context is defined and it is primary */
              p_cmd_p = gprsGenericContext_p->cidUserData [p_cid];

              /* Special case for NASMDL2 - if the primary CID is not present
               * then create it now in the CID structure as if the user had
               * entered the AT+CGDCONT command for it.
               *
               */
              if (p_cmd_p == PNULL)
              {
                if (vgAllocateRamToCid (p_cid))
                {
                  p_cmd_p = gprsGenericContext_p->cidUserData [p_cid];

                  if (p_cmd_p != PNULL)
                  {
                    /* Give the primary connection a default setting - in this case
                     * IPV4 for the PDN Type..
                     * TODO: May have to configure this for when we have IPV6 or
                     * IPV4V6.
                     * Maybe use the default bearer setting (MCGDEFCONT).
                     */
                    p_cmd_p->psdBearerInfo.reqPdnAddress.addressPresent = FALSE;
                    p_cmd_p->psdBearerInfo.reqPdnAddress.pdnType = PDN_TYPE_IPV4;

                    p_cmd_p->profileDefined = TRUE;
                    p_cmd_p->cgdcontDefined = TRUE;
                  }
                }
                else
                {
                  result = VG_CME_INSUFFIC_RESOURCES;
                }
              }
              else if (p_cmd_p->profileDefined == FALSE)
              {
                /* We can use this CID because the profile is not set - so set it as defined
                 */
                vgInitialiseCidData (p_cmd_p, p_cid);

                p_cmd_p->psdBearerInfo.reqPdnAddress.addressPresent = FALSE;
                p_cmd_p->psdBearerInfo.reqPdnAddress.pdnType = PDN_TYPE_IPV4;

                p_cmd_p->profileDefined = TRUE;
                p_cmd_p->cgdcontDefined = TRUE;
              }

              if (result == RESULT_CODE_OK)
              {
                if ((p_cmd_p != PNULL) &&
                    (p_cmd_p->profileDefined) &&
                    (!p_cmd_p->psdBearerInfo.secondaryContext))
                {
                  /* We don't want to copy everything from the primary connection
                   * just the APN, PDN address type and the flow control type.
                   * Note that if the primary CID is changed for these parameters,
                   * then any associated secondary CIDs must be updated in line when this occurs.
                   */
                  cmd_p->psdBearerInfo.reqApnPresent = p_cmd_p->psdBearerInfo.reqApnPresent;
                  memcpy (&cmd_p->psdBearerInfo.reqApn, &p_cmd_p->psdBearerInfo.reqApn, sizeof (AccessPointName));
                  memcpy (&cmd_p->psdBearerInfo.reqTextualApn, &p_cmd_p->psdBearerInfo.reqTextualApn, sizeof (TextualAccessPointName));

                  cmd_p->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress.pdnType =
                          p_cmd_p->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress.pdnType;

                  cmd_p->psdBearerInfo.flowControlType = p_cmd_p->psdBearerInfo.flowControlType;

                  cmd_p->psdBearerInfo.secondaryContext = TRUE;
                  cmd_p->psdBearerInfo.primaryCid = (Int8) p_cid;

                  /* Also copy the APN Rate Control information if present */
                  memcpy(&cmd_p->apnUplinkRateControlInfo, &p_cmd_p->apnUplinkRateControlInfo, sizeof(VgApnUplinkRateControlInfo));

                  defd = TRUE;
                }
                else
                {
                  result = VG_CME_INVALID_CID_VALUE;
                }
              }
            }
            else
            {
              result = VG_CME_INVALID_CID_VALUE;
            }
          }
        }
        else
        {
          result = VG_CME_INVALID_CID_VALUE;
        }
      }

      /* now get the D_COMP */
      if ((result == RESULT_CODE_OK) && (defd) && (!vgOpManCidActive ((Int8)cid)))
      {
        FatalAssert (cmd_p != PNULL);

        if (getExtendedParameter (commandBuffer_p, &tmpVar, ULONG_MAX))
        {
          if (tmpVar == ULONG_MAX)
          {
            /* no parameter so undefine it */
            cmd_p->psdBearerInfo.dataComp = DATA_COMP_OFF;
          }
          else
          {
            if (tmpVar <= (Int32)DATA_COMP_V42BIS)
            {
              cmd_p->psdBearerInfo.dataComp = (DataCompType)tmpVar;
              defd = TRUE;
            }
            else
            {
              result = VG_CME_INVALID_INPUT_VALUE;
            }
          }
        }
        else /* error reading supplied parameter */
        {
          result = RESULT_CODE_ERROR;
        }
      }

      /* get the P_COMP */
      if ((result == RESULT_CODE_OK) && (defd) && (!vgOpManCidActive ((Int8)cid)))
      {
        FatalAssert (cmd_p != PNULL);

        if (getExtendedParameter (commandBuffer_p, &tmpVar, ULONG_MAX))
        {
          if (tmpVar == ULONG_MAX)
          {
            /* no parameter so undefine it */
            cmd_p->psdBearerInfo.headerComp = HEADER_COMP_OFF;
          }
          else
          {
            if (tmpVar <= (Int32)HEADER_COMP_RFC3095)
            {
              cmd_p->psdBearerInfo.headerComp = (HeaderCompType)tmpVar;
              defd = TRUE;
            }
            else
            {
              result = VG_CME_INVALID_INPUT_VALUE;
            }
          }
        }
        else /* error reading supplied parameter */
        {
          result = RESULT_CODE_ERROR;
        }
      }

      if ((result == RESULT_CODE_OK) && (!vgOpManCidActive ((Int8)cid)))
      {
        FatalAssert (cmd_p != PNULL);

        /* if this is a reset then reset local cid data as well */
        if (defd)
        {
          /* update CGPADDR information, if static IP address is present cgpaddr will keep that information */
          cmd_p->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress = cmd_p->psdBearerInfo.reqPdnAddress;

          /* this AT command defines a profile */
          cmd_p->profileDefined = TRUE;
          cmd_p->cgdscontDefined = TRUE;
        }
        else
        {
          vgInitialiseCidData (cmd_p, cid);
          vgCheckProfileUndefined (cmd_p, cid);
        }
      }
      else if ((cid < MAX_NUMBER_OF_CIDS) && (!vgOpManCidActive ((Int8)cid)))
      {
        if (cmd_p != PNULL)
        {
          memcpy( &cmd_p->psdBearerInfo, &(gprsGenericContext_p->oldPsdBearerInfo), sizeof(VgPsdBearerInfo));
        }
      }
      break;
    }
    case EXTENDED_QUERY:    /* AT+CGDSCONT?  */
    {
      viewCGDSCONTAttributes (entity);
      break;
    }
    case EXTENDED_ACTION:   /* AT+CGDSCONT   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);
}
#endif /* FEA_DEDICATED_BEARER) */

/*--------------------------------------------------------------------------
*
* Function:    vgGpMGSINK
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Sends an IP/UDP packet to Discard (sink null) service on port 9.
*              Note that this command can only be executed on one AT channel
*              at any one time.
*-------------------------------------------------------------------------*/

ResultCode_t vgGpMGSINK (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{

  ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
  ResultCode_t         result = RESULT_CODE_ERROR;

  switch (operation)
  {
    case EXTENDED_QUERY: /* *MGSINK? */
    {
      break;
    }
    case EXTENDED_RANGE: /* *MGSINK=? */
    {
      vgPutNewLine (entity);
      vgPrintf     (entity, (const Char*)"%C = [NSAPI],[PacketSize 0-10000],[PacketCount 1-20],[Address],[Port]");
      vgPutNewLine (entity);
      result = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_ASSIGN: /* *MGSINK=XX,XX,XX,"xxx.xxx.xxx.xxx",XX */
    {
      result = vgProcMgSinkExtAssign (commandBuffer_p, entity);
      break;
    }
    case EXTENDED_ACTION:
    {
      break;
    }
    default:
    {
      break;
    }
  }

  return (result);

}

/*--------------------------------------------------------------------------
*
* Function:    vgGpMGTCSINK
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Sends an IP/TCP packet
*-------------------------------------------------------------------------*/

ResultCode_t vgGpMGTCSINK (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity)
{

  ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
  ResultCode_t         result = RESULT_CODE_ERROR;

  switch (operation)
  {
    case EXTENDED_QUERY: /* *MGTCSINK? */
    {
      break;
    }
    case EXTENDED_RANGE: /* *MGTCSINK=? */
    {
      vgPutNewLine (entity);
      vgPrintf     (entity, (const Char*)"%C = [NSAPI],[PacketSize 0-10000],[PacketCount 1-20],[Address],[Port]");
      vgPutNewLine (entity);
      result = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_ASSIGN: /* *MGTCSINK=XX,XX,XX,"xxx.xxx.xxx.xxx",XX */
    {
      result = vgProcMgtcSinkExtAssign (commandBuffer_p, entity);
      break;
    }
    case EXTENDED_ACTION:
    {
      break;
    }
    default:
    {
      break;
    }
  }

  return (result);

}

#if defined (FEA_PPP)
/*--------------------------------------------------------------------------
*
* Function:    vgGpMLOOPPSD
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT*MLOOPPSD command which activates
*              or deactivates loopback mode on PPP.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGpMLOOPPSD (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  GprsContext_t         *gprsContext_p = ptrToGprsContext (entity);
  ResultCode_t          result = RESULT_CODE_OK;
  Int32                 looppsd;
  Int32                 numDlXmits = 0;
  Int32                 loopMode = 0;
  Int32                 dlTimeout = 0;
  Int32                 packetSize = 0;
  Int32                 totalNumDlXmits = 0;
  Int32                 pppMode = 0;
  Int32                 pppFCSChecking = 0;
  Int32                 pppByteStuffing = 0;
  Int32                 pppUlChecking = 0;

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT*MLOOPPSD=? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity,
                (const Char*)"%C: (0-1),(0-100),(0-2),(0,%d-65535),(1-65535),(0-65535),(0-1),(0-1),(0-1),(0-1)",
                LOOPPSD_MIN_TIMEOUT);
      break;
    }
    case EXTENDED_ASSIGN:   /* AT*MLOOPPSD=  */
    {
      if (getExtendedParameter (commandBuffer_p, &looppsd, ULONG_MAX))
      {
        if (looppsd == ULONG_MAX)
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }

      if (result == RESULT_CODE_OK)
      {
        switch (looppsd)
        {
          case LOOPPSD_ENABLE:
          {
            if (vgOpManAllocateConnection (entity, PPP_CONNECTION) == TRUE)
            {
#if defined (ATCI_SLIM_DISABLE)

              FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
#endif
              /* Enable request */
              switch (gprsContext_p->vgPppLoopbackState)
              {
                case VG_LOOPBACK_DISABLED:
                /* Carry on even if enabled to enter data mode */
                case VG_LOOPBACK_ENABLED:
                {
                  if (getExtendedParameter (commandBuffer_p,
                                            &numDlXmits,
                                            LOOPPSD_DEFAULT_DL_XMIT))
                  {
                    if (numDlXmits > LOOPPSD_MAX_DL_XMIT)
                    {
                      result = VG_CME_INVALID_INPUT_VALUE;
                    }
                  }
                  else
                  {
                    result = RESULT_CODE_ERROR;
                  }

                  if (result == RESULT_CODE_OK)
                  {
                    if (getExtendedParameter (commandBuffer_p,
                                              &loopMode,
                                              LOOPPSD_TWO_WAY_MODE))
                    {
                      switch (loopMode)
                      {
                        case LOOPPSD_TWO_WAY_MODE:
                        case LOOPPSD_DL_MODE:
                        case LOOPPSD_UL_MODE:
                        {
                          break;
                        }
                        default:
                        {
                          result = VG_CME_INVALID_INPUT_VALUE;
                          break;
                        }
                      }
                    }
                    else
                    {
                      result = RESULT_CODE_ERROR;
                    }
                  }

                  if (result == RESULT_CODE_OK)
                  {
                    if (getExtendedParameter (commandBuffer_p,
                                              &dlTimeout,
                                              0))
                    {
                      if ((dlTimeout != 0) &&
                          ((dlTimeout < LOOPPSD_MIN_TIMEOUT) || (dlTimeout > USHRT_MAX)))
                      {
                        result = VG_CME_INVALID_INPUT_VALUE;
                      }
                    }
                    else
                    {
                      result = RESULT_CODE_ERROR;
                    }
                  }

                  if (result == RESULT_CODE_OK)
                  {
                    if (getExtendedParameter (commandBuffer_p,
                                              &packetSize,
                                              LOOPPSD_DEFAULT_PCKT_SIZE))
                    {
                      if ((packetSize == 0) || (packetSize > USHRT_MAX))
                      {
                        result = VG_CME_INVALID_INPUT_VALUE;
                      }
                    }
                    else
                    {
                      result = RESULT_CODE_ERROR;
                    }
                  }

                  if (result == RESULT_CODE_OK)
                  {
                    if (getExtendedParameter (commandBuffer_p, &totalNumDlXmits, 0))
                    {
                      if (totalNumDlXmits > USHRT_MAX)
                      {
                        result = VG_CME_INVALID_INPUT_VALUE;
                      }
                    }
                    else
                    {
                      result = RESULT_CODE_ERROR;
                    }
                  }

                  if (result == RESULT_CODE_OK)
                  {
                    if (getExtendedParameter (commandBuffer_p, &pppMode, LOOPBACK_PPP_MODE_STANDARD))
                    {
                      switch (pppMode)
                      {
                        case LOOPBACK_PPP_MODE_STANDARD:
                        case LOOPBACK_PPP_MODE_CORE:
                        {
                          break;
                        }
                        default:
                        {
                          result = VG_CME_INVALID_INPUT_VALUE;
                          break;
                        }
                      }
                    }
                    else
                    {
                      result = RESULT_CODE_ERROR;
                    }
                  }

                  if (result == RESULT_CODE_OK)
                  {
                    if (getExtendedParameter (commandBuffer_p, &pppByteStuffing, TRUE))
                    {
                      if ((pppByteStuffing != TRUE) &&
                          (pppByteStuffing != FALSE))
                      {
                        result = VG_CME_INVALID_INPUT_VALUE;
                      }
                    }
                    else
                    {
                      result = RESULT_CODE_ERROR;
                    }
                  }

                  if (result == RESULT_CODE_OK)
                  {
                    if (getExtendedParameter (commandBuffer_p, &pppFCSChecking, TRUE))
                    {
                      if ((pppFCSChecking != TRUE) &&
                          (pppFCSChecking != FALSE))
                      {
                        result = VG_CME_INVALID_INPUT_VALUE;
                      }
                    }
                    else
                    {
                      result = RESULT_CODE_ERROR;
                    }
                  }

                  if (result == RESULT_CODE_OK)
                  {
                    if (getExtendedParameter (commandBuffer_p, &pppUlChecking, TRUE))
                    {
                      if ((pppUlChecking != TRUE) &&
                          (pppUlChecking != FALSE))
                      {
                        result = VG_CME_INVALID_INPUT_VALUE;
                      }
                    }
                    else
                    {
                      result = RESULT_CODE_ERROR;
                    }
                  }

                  if (result == RESULT_CODE_OK)
                  {
                    gprsContext_p->vgPppLoopbackState       = VG_LOOPBACK_ENABLE_PENDING;
                    gprsContext_p->vgPppLoopbackNumDlXmit   = (Int8) numDlXmits;
                    gprsContext_p->vgPppLoopbackMode        = (Int8) loopMode;
                    gprsContext_p->vgPppLoopbackDlTimeout   = (Int16) dlTimeout;
                    gprsContext_p->vgPppLoopbackPacketSize  = (Int16) packetSize;
                    gprsContext_p->vgPppTotalNumDlXmit      = (Int16) totalNumDlXmits;

                    gprsContext_p->vgPppLoopbackPppMode             = (VgLoopbackPppMode) pppMode;
                    gprsContext_p->vgPppLoopbackFCSCheckingEnabled  = (Boolean) pppFCSChecking;
                    gprsContext_p->vgPppLoopbackByteStuffingEnabled = (Boolean) pppByteStuffing;
                    gprsContext_p->vgPppLoopbackUlCheckingEnabled   = (Boolean) pppUlChecking;

                    vgApexAbpdPppLoopbackReq (entity, TRUE);
                    result = RESULT_CODE_PROCEEDING;
                  }
                  else
                  {
                    vgOpManDropConnection (entity, PPP_CONNECTION);
                  }
                  break;
                }

                default:
                {
                  vgOpManDropConnection (entity, PPP_CONNECTION);
                  result = RESULT_CODE_ERROR;
                  break;
                }
              }
            }
            else
            {
              result = RESULT_CODE_ERROR;
            }
            break;
          }

          case LOOPPSD_DISABLE:
          {
#if defined (ATCI_SLIM_DISABLE)

            /* Disable request */
            FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
#endif
            switch (gprsContext_p->vgPppLoopbackState)
            {
              case VG_LOOPBACK_ENABLED:
              {

                vgOpManDropConnection (entity, PPP_CONNECTION);
                gprsContext_p->vgPppLoopbackState       = VG_LOOPBACK_DISABLE_PENDING;
                gprsContext_p->vgPppLoopbackNumDlXmit   = 0;
                gprsContext_p->vgPppLoopbackMode        = 0;
                gprsContext_p->vgPppLoopbackDlTimeout   = 0;
                gprsContext_p->vgPppLoopbackPacketSize  = 0;
                gprsContext_p->vgPppTotalNumDlXmit      = 0;

                gprsContext_p->vgPppLoopbackPppMode             = LOOPBACK_PPP_MODE_STANDARD;
                gprsContext_p->vgPppLoopbackFCSCheckingEnabled  = TRUE;
                gprsContext_p->vgPppLoopbackByteStuffingEnabled = TRUE;
                gprsContext_p->vgPppLoopbackUlCheckingEnabled   = TRUE;

                vgApexAbpdPppLoopbackReq (entity, FALSE);
                result = RESULT_CODE_PROCEEDING;
                break;
              }

              case VG_LOOPBACK_DISABLED:
              {
                result = RESULT_CODE_OK;
                break;
              }

              default:
              {
                result = RESULT_CODE_ERROR;
                break;
              }
            }
            break;
          }

          default:
          {
            result = VG_CME_INVALID_INPUT_VALUE;
            break;
          }
        }
      }
      break;
    }
    case EXTENDED_QUERY:    /* AT*MLOOPPSD?  */
    {
#if defined (ATCI_SLIM_DISABLE)

      FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
#endif
      vgPutNewLine (entity);
      vgPrintf (entity,
                (const Char*)"%C: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                (Int8)gprsContext_p->vgPppLoopbackState,
                (Int8)gprsContext_p->vgPppLoopbackNumDlXmit,
                gprsContext_p->vgPppLoopbackMode,
                gprsContext_p->vgPppLoopbackDlTimeout,
                gprsContext_p->vgPppLoopbackPacketSize,
                gprsContext_p->vgPppTotalNumDlXmit,
                gprsContext_p->vgPppLoopbackPppMode,
                gprsContext_p->vgPppLoopbackFCSCheckingEnabled,
                gprsContext_p->vgPppLoopbackByteStuffingEnabled,
                gprsContext_p->vgPppLoopbackUlCheckingEnabled);
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_ACTION:   /* AT*MLOOPPSD   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGpMPPPCONFIG
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function configures the PPP timers and counters for GPRS
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGpMPPPCONFIG (CommandLine_t *commandBuffer_p,
                             const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  ResultCode_t          result = RESULT_CODE_OK;
  Int32                 tmpVal, index = 0, finalValue = 0, initValue = 0;
  Int8                  tries = 0;
  Boolean               present;

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT*MPPPCONFIG=? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity,
                (const Char*)"%C: (0-%d),(0-%lu),(0-%lu),(0-%d)",
                PPP_CFG_INDEX_NUM - 1, ULONG_MAX, ULONG_MAX, UCHAR_MAX);
      break;
    }
    case EXTENDED_ASSIGN:   /* AT*MPPPCONFIG=  */
    {
      if (getExtendedParameter (commandBuffer_p, &tmpVal, ULONG_MAX))
      {
        if (tmpVal < PPP_CFG_INDEX_NUM)
        {
          index = tmpVal;
        }
        else
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }

      if (result == RESULT_CODE_OK)
      {
        if (getExtendedParameterPresent (commandBuffer_p,
                                         &tmpVal,
                                         ULONG_MAX,
                                         &present))
        {
          if (present)
          {
            finalValue = tmpVal;
          }
          else
          {
            /* Parameter must always be present */
            result = RESULT_CODE_ERROR;
          }
        }
        else
        {
          /* Invalid parameter */
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }

      if (result == RESULT_CODE_OK)
      {
        if (getExtendedParameterPresent (commandBuffer_p,
                                         &tmpVal,
                                         ULONG_MAX,
                                         &present))
        {
          if (present)
          {
            initValue = tmpVal;
          }
          else
          {
            /* Not present, send 0 */
            initValue = 0;
          }
        }
        else
        {
          /* Invalid parameter */
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }

      if (result == RESULT_CODE_OK)
      {
        if (getExtendedParameterPresent (commandBuffer_p,
                                         &tmpVal,
                                         ULONG_MAX,
                                         &present))
        {
          if (present)
          {
            if (tmpVal <= UCHAR_MAX)
            {
              tries = (Int8)tmpVal;
            }
            else
            {
              result = VG_CME_INVALID_INPUT_VALUE;
            }
          }
          else
          {
            /* Not present, send 0 */
            tries = 0;
          }
        }
        else
        {
          /* Invalid parameter */
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }

      if (result == RESULT_CODE_OK)
      {

        vgApexAbpdPppConfigReq (entity, TRUE, index, finalValue, initValue, tries);
        result = RESULT_CODE_PROCEEDING;
      }
      break;
    }
    case EXTENDED_QUERY:    /* AT*MPPPCONFIG?  */
    {

      vgApexAbpdPppConfigReq (entity, FALSE, 0, 0, 0, 0);
      result = RESULT_CODE_PROCEEDING;
      break;
    }
    case EXTENDED_ACTION:   /* AT*MPPPCONFIG   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGpMPPPCONFIGAUTH
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function configures the PPP auth mode for GPRS
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGpMPPPCONFIGAUTH (CommandLine_t *commandBuffer_p,
                                 const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  ResultCode_t          result = RESULT_CODE_OK;
  Int32                 newValue = 0;
  Boolean               present;

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT*MPPPCONFIGAUTH=? */
    {
      vgPutNewLine (entity);
      /* at present we only support none, pap, chap, and ms-chap-v1 */
      vgPrintf (entity,
                (const Char*)"%C: (%d-%d)", PPP_AUTH_NONE, PPP_AUTH_MS_CHAP_V1);
      break;
    }
    case EXTENDED_ASSIGN:   /* AT*MPPPCONFIGAUTH=  */
    {
      if (result == RESULT_CODE_OK)
      {
        if (getExtendedParameterPresent (commandBuffer_p,
                                         &newValue,
                                         ULONG_MAX,
                                         &present))
        {
          if ( ! present)
          {
            result = RESULT_CODE_ERROR;
          }
        }
        else
        {
          /* Invalid parameter */
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }
      /* leave further validation to the ppp task */
      if (result == RESULT_CODE_OK)
      {
        vgApexAbpdPppConfigAuthReq (entity, TRUE, newValue);
        result = RESULT_CODE_PROCEEDING;
      }
      break;
    }
    case EXTENDED_QUERY:    /* AT*MPPPCONFIG?  */
    {

      vgApexAbpdPppConfigAuthReq (entity, FALSE, 0);
      result = RESULT_CODE_PROCEEDING;
      break;
    }
    case EXTENDED_ACTION:   /* AT*MPPPCONFIG   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}
#endif /* FEA_PPP */
#if defined (FEA_ACL)
/*--------------------------------------------------------------------------
*
* Function:    vgGpMSACL
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Handles the AT*MSACL command, which enables/disables ACL feature.
*-------------------------------------------------------------------------------*/
ResultCode_t vgGpMSACL (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
  VgCGACLData          *aclData  = &(ptrToGprsGenericContext ()->vgCGACLData);
  ResultCode_t         result    = RESULT_CODE_ERROR;
  Int32                sacl      = 0;

  switch (operation)
  {
    case EXTENDED_QUERY: /* *MSACL? */
    {
      result = vgChManContinueAction (entity, SIG_APEX_ABPD_ACL_STATUS_REQ);
      break;
    }
    case EXTENDED_RANGE: /* *MSACL=? */
    {
      vgPutNewLine (entity);
      vgPrintf     (entity, (const Char*)"*MSACL: (0-1)");
      vgPutNewLine (entity);
      result = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_ASSIGN: /* *MSACL= */
    {
      if (getExtendedParameter (commandBuffer_p, &sacl, ULONG_MAX))
      {
        switch(sacl)
        {
           case 0:
           case 1:
             aclData->setAcl = (Boolean)sacl;
             result =  readAndCheckPin2 ( commandBuffer_p, entity);
             if (result == RESULT_CODE_OK)
             {
               result = vgChManContinueAction (entity, SIG_APEX_ABPD_SET_ACL_REQ);
             }
             break;
           default:
             break;
        }
      }
      break;
    }
    case EXTENDED_ACTION: /* *MSACL */
    {
      break;
    }
    default:
    {
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGpMLACL
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Handles the AT*MLACL command, which displays ACL list
*-------------------------------------------------------------------------*/

ResultCode_t vgGpMLACL (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
  VgCGACLData          *aclData = &(ptrToGprsGenericContext ()->vgCGACLData);
  ResultCode_t         result = RESULT_CODE_ERROR;
  Int32                startField = 0;
  Int32                endField  = 255;

  switch (operation)
  {
    case EXTENDED_QUERY: /* *MLACL? */
    {
      break;
    }
    case EXTENDED_RANGE: /* *MLACL=? */
    {
      vgPutNewLine (entity);
      vgPrintf     (entity, (const Char*)"*MLACL: (0-255),(0-255)");
      vgPutNewLine (entity);
      result = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_ASSIGN: /* *MLACL= */
    {
      if (getExtendedParameter (commandBuffer_p, &startField, ULONG_MAX))
      {
        if ( startField < ULONG_MAX)
        {
          aclData->startField = (Int8)startField;
          if (getExtendedParameter (commandBuffer_p, &endField, ULONG_MAX))
          {
            if (endField >= startField)
            {
              if (endField < ULONG_MAX)
              {
                aclData->endField = (Int8)endField;
              }
              else
              {
                aclData->endField = 255;
              }
              result = vgChManContinueAction (entity, SIG_APEX_ABPD_LIST_ACL_REQ);
            }
          }
        }
      }
      break;
    }
    case EXTENDED_ACTION: /* *MLACL */
    {
      break;
    }
    default:
    {
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGpMWACL
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Handles the AT*MWACL command, which writes ACL entry.
*-------------------------------------------------------------------------*/

ResultCode_t vgGpMWACL (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity)
{

  ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
  VgCGACLData *aclData = &(ptrToGprsGenericContext ()->vgCGACLData);
  ResultCode_t         result = RESULT_CODE_ERROR;
  Int32 position;
  Boolean apnPresent;
  Int16 length;

  switch (operation)
  {
    case EXTENDED_QUERY: /* *MWACL? */
    {
      break;
    }
    case EXTENDED_RANGE: /* *MWACL=? */
    {
      vgPutNewLine (entity);
      vgPrintf     (entity, (const Char*)"*MWACL: (0-255)");
      vgPutNewLine (entity);
      result = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_ASSIGN: /* *MWACL= */
    {
      if (getExtendedParameter (commandBuffer_p, &position, ULONG_MAX))
      {
        if ( position < ULONG_MAX)
        {
          aclData->startField = (Int8)position;
          apnPresent = getExtendedString (commandBuffer_p,
                                          aclData->apn.name,
                                          MAX_APN_NAME,
                                          &length);
          if (apnPresent)
          {
            aclData->apn.length = (Int8)length;
            result =  readAndCheckPin2 (commandBuffer_p, entity);
            if (result == RESULT_CODE_OK)
            {
              result = vgChManContinueAction (entity, SIG_APEX_ABPD_WRITE_ACL_REQ);
            }
          }
        }
      }
      break;
    }
    case EXTENDED_ACTION: /* *MWACL */
    {
      break;
    }
    default:
    {
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGpMDACL
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Handles the AT*MDACL command, which deletes an ACL entry.
*-------------------------------------------------------------------------*/

ResultCode_t vgGpMDACL (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{

  ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
  VgCGACLData          *aclData  = &(ptrToGprsGenericContext ()->vgCGACLData);
  ResultCode_t         result    = RESULT_CODE_ERROR;
  Int32                index;

  switch (operation)
  {
    case EXTENDED_QUERY: /* *MDACL? */
    {
      break;
    }
    case EXTENDED_RANGE: /* *MDACL=? */
    {
      vgPutNewLine (entity);
      vgPrintf     (entity, (const Char*)"*MDACL: (0-255)");
      vgPutNewLine (entity);
      result = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_ASSIGN: /* *MDACL= */
    {
      if (getExtendedParameter (commandBuffer_p, &index, ULONG_MAX))
      {
        if (index < 255)
        {
          aclData->startField = (Int8)index;
          result =  readAndCheckPin2 ( commandBuffer_p, entity);
          if (result == RESULT_CODE_OK)
          {
            result = vgChManContinueAction (entity, SIG_APEX_ABPD_DELETE_ACL_REQ);
          }
        }
      }
      break;
    }
    case EXTENDED_ACTION: /* *MDACL */
    {
      break;
    }
    default:
    {
      break;
    }
  }

  return (result);
}
#endif

#if defined (FEA_MT_PDN_ACT)
/*--------------------------------------------------------------------------
*
* Function:    vgGpMMTPDPCID
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Set CID for MT PDP activations.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGpMMTPDPCID (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity)
{
  ResultCode_t          result    = RESULT_CODE_OK;
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  Int32                 val = 0;

  PARAMETER_NOT_USED(entity);

  /* 2 parameters: mode + CID */
  switch (operation)
  {
    case EXTENDED_ASSIGN:  /* AT*MMTPDPCID=  */
    {
      if (getExtendedParameter (commandBuffer_p, &val, ULONG_MAX)
          && (val < 2))
      {
        if (val == 0)
        {
          gprsGenericContext_p->vgMMTPDPCIDData.enabled = FALSE;
          gprsGenericContext_p->vgMMTPDPCIDData.cid     = 0;
        }
        else /* val == 1 */
        {
          /* Get the next parameter (cid) */
          if (getExtendedParameter (commandBuffer_p, &val, ULONG_MAX)
              && (val < MAX_NUMBER_OF_CIDS) && (val >= vgGpGetMinCidValue(entity)))
          {
            /* Set the value */
            gprsGenericContext_p->vgMMTPDPCIDData.enabled = TRUE;
            gprsGenericContext_p->vgMMTPDPCIDData.cid     = (Int8) val;
          }
          else
          {
            result = VG_CME_INVALID_INPUT_VALUE;
          }
        }
      }
      else
      {
        result = VG_CME_INVALID_INPUT_VALUE;
      }
      break;
    }
    case EXTENDED_RANGE: /* AT*MMTPDPCID=?  */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"*MMTPDPCID: (0-1),(1-%d)", (MAX_NUMBER_OF_CIDS - 1));
      break;
    }
    case EXTENDED_QUERY: /* AT*MMTPDPCID?  */
    {
      vgPutNewLine (entity);

      if (gprsGenericContext_p->vgMMTPDPCIDData.enabled)
      {
        vgPrintf (entity,
                  (const Char*)"*MMTPDPCID: 1,%d", gprsGenericContext_p->vgMMTPDPCIDData.cid);
      }
      else
      {
        vgPrintf (entity,
                  (const Char*)"*MMTPDPCID: 0");
      }
      break;
    }
    case EXTENDED_ACTION: /* AT*MMTPDPCID  */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}
#endif /* FEA_MT_PDN_ACT */

/*--------------------------------------------------------------------------
*
* Function:     vgGetUnusedCid
*
* Parameters:   entity      - the entity which wants the new CID
*               cidValue    - return an unused cid
*               psdBearerId - Current PSD bearer ID.
*
* Returns:      Boolean     - whether there was a free CID
*
* Description:  Checks if there is an unused CID, and responds with that
*               CID value.  For LTE if we have enabled LTE GCF test mode
*               we first try to assign the CID value to the same value as the
*               PSD bearer ID.  If that is not possible, then we just get the
*               next free one.
*
*-------------------------------------------------------------------------*/

Boolean vgGetUnusedCid (const VgmuxChannelNumber entity,
                        Int8 psdBearerId,
                        Int8 *cidValue)
{
  Boolean               found                 = FALSE;
  Int8                  cidIndex;

  /* Are we in LTE mode, is the LTE GCF test mode enabled
   * and is the CID actually free to use.
   */

  /* For NB-IOT we need to check if we are allowed to activate default
   * PDN connection on attach - before doing anything (set by AT+CIPCA)
   */
  if ((psdBearerId != PSD_BEARER_ID_UNASSIGNED) &&
      vgIsCurrentAccessTechnologyLte() &&
      vgCIPCAPermitsActivateAttachDefBearer() &&
      (getProfileValue (entity, PROF_MLTEGCF) == GCF_TEST_MODE_ENABLE) &&
      (!vgOpManCidDefined (psdBearerId)))
  {
    *cidValue = psdBearerId;
    found = TRUE;
  }
  else if ((psdBearerId == PSD_BEARER_ID_UNASSIGNED) &&
  	   vgIsCurrentAccessTechnologyLte() &&
       vgCIPCAPermitsActivateAttachDefBearer() &&
      (getProfileValue (entity, PROF_MLTEGCF) == GCF_TEST_CID0_ENABLE))
	/* Must be set to use CID 0.  It doesn't matter if the CID is defined or not
	 * unlike in above case for GCF tests.
	 */
  {
    *cidValue = 0;
    found = TRUE;
  }
  else
  {
    /* Check whether a free cid can be used.
     * Must always start from 1 for all remaining CIDs
     * as not allowed to use CID 0 apart from default bearer activated during attach
     */
    for (cidIndex = 1; cidIndex < MAX_NUMBER_OF_CIDS; cidIndex++ )
    {
      if ((!vgOpManCidDefined (cidIndex)) && found == FALSE)
      {
        /* record the unused cid */
        *cidValue     = cidIndex;
        found = TRUE;
        break;
      }
    }
  }
  return (found);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGpCGCONTRDP
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Allows the user to read a number of attributes which have
*              been assigned to a CID when the context was activated, such
*              as PSD Bearer ID (EPS Bearer ID/NSAPI), APN, IP address, DNS
*              addresses.
*
*-------------------------------------------------------------------------*/
ResultCode_t vgGpCGCONTRDP (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation               = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t  *gprsGenericContext_p   = ptrToGprsGenericContext ();
  ResultCode_t          result                  = RESULT_CODE_OK;
  Int32                 cid;

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT+CGCONTRDP=? */
    {
      /* Just display a list of the active CIDs */
      vgPrintActiveCids ((const Char*)"+CGCONTRDP:", entity, TRUE, FALSE);
      break;
    }
    case EXTENDED_ASSIGN:   /* AT+CGCONTRDP=  */
    {
      /* get the CID */
      (void)getExtendedParameter (commandBuffer_p, &cid, ULONG_MAX);
      /* No CID provided so we have to go through and display the
       * CGCONTRDP values for all active primary CIDs
       */
      if (cid == ULONG_MAX)
      {
        vgDisplayAllCGCONTRDPInfo (entity);
      }
      /* If the cid is invalid or not primary or does not exist then error. */
      else if ((cid >= vgGpGetMinCidValue(entity)) &&(cid < MAX_NUMBER_OF_CIDS))
      {
        if ((gprsGenericContext_p->cidUserData [cid] != PNULL) &&
            (gprsGenericContext_p->cidUserData [cid]->profileDefined) &&
            (gprsGenericContext_p->cidUserData [cid]->isActive)
#if defined (FEA_DEDICATED_BEARER)
            && !(gprsGenericContext_p->cidUserData [cid]->psdBearerInfo.secondaryContext)
#endif /* FEA_DEDICATED_BEARER */
            )
        {
          vgPutNewLine (entity);
          vgDisplayCGCONTRDPInfo(entity, (Int8)cid);
        }
        else
        {
          result = VG_CME_INVALID_CID_VALUE;
        }
      }
      else
      {
        result = VG_CME_INVALID_CID_VALUE;
      }
      break;
    }
    case EXTENDED_ACTION:   /* AT+CGCONTRDP   */
      /* No CID provided so we have to go through and display the
       * CGCONTRDP values for all active primary CIDs
       */
      vgDisplayAllCGCONTRDPInfo (entity);
      break;
    /* Query does nothing */
    case EXTENDED_QUERY:    /* AT+CGCONTRDP?  */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

#if defined (FEA_DEDICATED_BEARER)
/*--------------------------------------------------------------------------
*
* Function:    vgGpCGSCONTRDP
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Allows the user to read a number of attributes which have
*              been assigned to a CID when the context was activated, for
*              a secondary context only.  It basically just displays the
*              CID, Primary CID and PSD Bearer ID (EPS Bearer ID/NSAPI).
*-------------------------------------------------------------------------*/
ResultCode_t vgGpCGSCONTRDP (CommandLine_t *commandBuffer_p,
                             const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  ResultCode_t          result = RESULT_CODE_OK;
  Int32                 cid;

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT+CGSCONTRDP=? */
    {
      /* Just display a list of the active CIDs */
      vgPrintActiveCids ((const Char*)"+CGSCONTRDP:", entity, FALSE, TRUE);
      break;
    }
    case EXTENDED_ASSIGN:   /* AT+CGSCONTRDP=  */
    {
      /* get the CID */
      (void)getExtendedParameter (commandBuffer_p, &cid, ULONG_MAX);
      /* No CID provided so we have to go through and display the
       * CGSCONTRDP values for all active secondary CIDs
       */
      if (cid == ULONG_MAX)
      {
        vgDisplayAllCGSCONTRDPInfo (entity);
      }
      /* If the cid is invalid or not secondary or does not exist then error. */
      else if ((cid >= vgGpGetMinCidValue(entity)) &&(cid < MAX_NUMBER_OF_CIDS))
      {
        if ((gprsGenericContext_p->cidUserData [cid] != PNULL) &&
            (gprsGenericContext_p->cidUserData [cid]->profileDefined) &&
            (gprsGenericContext_p->cidUserData [cid]->isActive)
#if defined (FEA_DEDICATED_BEARER)
             && (gprsGenericContext_p->cidUserData [cid]->psdBearerInfo.secondaryContext)
#endif /* FEA_DEDICATED_BEARER */
             )

        {
          vgPutNewLine (entity);
          vgDisplayCGSCONTRDPInfo(entity, (Int8)cid);
        }
        else
        {
          result = VG_CME_INVALID_CID_VALUE;
        }
      }
      else
      {
        result = VG_CME_INVALID_CID_VALUE;
      }
      break;
    }
    case EXTENDED_ACTION:   /* AT+CGSCONTRDP   */
      /* Action displays CGSCONTRDP values for all active secondary CIDs */
      vgDisplayAllCGSCONTRDPInfo (entity);
      break;
    /* Query does nothing */
    case EXTENDED_QUERY:    /* AT+CGSCONTRDP?  */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}
#endif /* FEA_DEDICATED_BEARER */

#if defined (FEA_QOS_TFT)
/*--------------------------------------------------------------------------
*
* Function:    vgGpCGTFTRDP
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Allows the user to read parameters of traffic flow template
*              which have been assigned to a CID when the context was
*              activated.
*
* Note:        The 'source address and subnet mask', 'destination port range',
*              and 'source port range' referred to in 27.007 spec
*              correspond to 'Remote Address and Subnet Mask', 'Local Port Range',
*              and 'Remote Port Range' respectively in the type definitions - this
*              is the convention in 23.060 and 24.008 specs in Rel-7 and later versions
*              and has been adapted for both Pre Rel-7 and Post Rel-7 code.
*
*     Pre Rel-7                                Post (including) Rel-7 (default usage)
*     =========                                ======================================
*     Source Address and Subnet Mask           Remote Address and Subnet Mask
*     Destination Port Range                   Local Port Range
*     Source Port Range                        Remote Port Range
*
*-------------------------------------------------------------------------*/
ResultCode_t vgGpCGTFTRDP (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation               = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t  *gprsGenericContext_p   = ptrToGprsGenericContext ();
  ResultCode_t          result                  = RESULT_CODE_OK;
  Int32                 cid;

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT+CGTFTRDP=? */
    {
      vgPrintActiveCids ((const Char*)"+CGTFTRDP:", entity, TRUE, TRUE);
      break;
    }

    case EXTENDED_ASSIGN:   /* AT+CGTFTRDP=  */
    {
      /* get the CID */
      (void)getExtendedParameter (commandBuffer_p, &cid, ULONG_MAX);
      /* No CID provided so we have to go through and display the
       * CGSCONTRDP values for all active secondary CIDs
       */
      if (cid == ULONG_MAX)
      {
         vgDisplayAllCGTFTRDPInfo (entity);
      }
      /* If the cid is invalid or does not exist then error. */
      else if ((cid >= vgGpGetMinCidValue(entity)) && (cid < MAX_NUMBER_OF_CIDS))
      {
        if ((gprsGenericContext_p->cidUserData [cid] != PNULL) &&
            (gprsGenericContext_p->cidUserData [cid]->profileDefined) &&
            (gprsGenericContext_p->cidUserData [cid]->isActive))

        {
          vgPutNewLine (entity);
          vgDisplayCGTFTRDPInfo(entity, (Int8)cid);
        }
        else
        {
          result = VG_CME_INVALID_CID_VALUE;
        }
      }
      else
      {
        result = VG_CME_INVALID_CID_VALUE;
      }
      break;
    }

    case EXTENDED_ACTION:   /* AT+CGTFTRDP   */
      /* No CID provided so we have to go through and display the
       * CGEQOSRDP values for all active CIDs
       */
      vgDisplayAllCGTFTRDPInfo (entity);
      break;

    /* Query does nothing */
    case EXTENDED_QUERY:    /* AT+CGTFTRDP?  */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}
#endif /* FEA_QOS_TFT */

/*--------------------------------------------------------------------------
 *
 * Function:    vgGpMCGDEFCONT
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: Executes command AT*MCGDEFCONT which sets default PSD
 *              Connection Settings
 *-------------------------------------------------------------------------*/

ResultCode_t vgGpMCGDEFCONT (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
  GprsGenericContext_t          *gprsGenericContext_p       = ptrToGprsGenericContext();
  AbpdApn                       *definedDefaultAPN_p        = &(gprsGenericContext_p->definedDefaultAPN);
  ResultCode_t                  result                      = RESULT_CODE_OK;
  const SupportedPDNTypesMap    *map                        = getSupportedPDNTypesMap();
  Boolean                       firstEntry                  = TRUE;
  Boolean                       strFound                    = FALSE;
  Char                          tmpString[STRING_LENGTH_40];
  Int16                         tmpStringLen;
  PdnType                       pdnType;

  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_RANGE:     /* *MCGDEFCONT=? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"*MCGDEFCONT: (");

      while (map->arrIndx != UCHAR_MAX)
      {
        if (TRUE == map->supported)
        {
          if (firstEntry)
          {
            firstEntry = FALSE;
            vgPrintf (entity, (const Char*)"\"%s\"", map->str);
          }
          else
          {
            vgPrintf (entity, (const Char*)",\"%s\"", map->str);
          }
        }
        map++;
      }
      vgPrintf (entity, (const Char*)")");
      vgPutNewLine (entity);
      break;
    }

    case EXTENDED_ASSIGN:    /* *MCGDEFCONT= */
    {

      memset (definedDefaultAPN_p, 0, sizeof(AbpdApn));
      if (getExtendedString (commandBuffer_p,
                             tmpString,
                             STRING_LENGTH_40,
                             &tmpStringLen))
      {
        if (tmpStringLen != 0)
        {
          strFound = vgPDNStrToEnum (tmpString, &pdnType);
          if (strFound == FALSE)
          {
            result = RESULT_CODE_ERROR;
          }
          else
          {
            definedDefaultAPN_p->pdnType = pdnType;
          }
        }
        else
        {
         /* When PDN type is not inputted, return error */
          result = RESULT_CODE_ERROR;
        }
      }
      else
      {
            result = RESULT_CODE_ERROR;
      }

      if (RESULT_CODE_OK == result)
      {

        if (getExtendedString (commandBuffer_p,
                               definedDefaultAPN_p->textualApn.name,
                               MAX_APN_NAME,
                               &tmpStringLen) &&
            tmpStringLen != 0)
        {
          definedDefaultAPN_p->apnPresent = TRUE;
          definedDefaultAPN_p->textualApn.length = (Int8)tmpStringLen;
          vgCiApnToNetworkAPN(definedDefaultAPN_p->textualApn, &definedDefaultAPN_p->apn);

          if (getExtendedString (commandBuffer_p,
                                 definedDefaultAPN_p->psdUser.username,
                                 MAX_PSD_USERNAME_LENGTH,
                                 &tmpStringLen) &&
              tmpStringLen != 0)
          {
            definedDefaultAPN_p->psdUser.usernamePresent = TRUE;
            definedDefaultAPN_p->psdUser.usernameLength = (Int8)tmpStringLen;

            if (getExtendedString (commandBuffer_p,
                                   definedDefaultAPN_p->psdUser.passwd,
                                   MAX_PSD_PWD_LENGTH,
                                   &tmpStringLen) &&
                tmpStringLen != 0)
            {
                definedDefaultAPN_p->psdUser.passwdPresent = TRUE;
                definedDefaultAPN_p->psdUser.passwdLength = (Int8)tmpStringLen;
            }
          }
        }

        result = vgChManContinueAction (entity, SIG_APEX_ABPD_APN_WRITE_REQ);

      }

      break;
    }

    case EXTENDED_QUERY:     /* *MCGDEFCONT? */
    {
      result = vgChManContinueAction (entity, SIG_APEX_ABPD_APN_READ_REQ);
      break;
    }

    case EXTENDED_ACTION:    /* *MCGDEFCONT */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }

  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGpCGDEL
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Allows the user to delete a non active PDP context.
*              This is useful in order to save memory.
*
*-------------------------------------------------------------------------*/
ResultCode_t vgGpCGDEL (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation               = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t  *gprsGenericContext_p   = ptrToGprsGenericContext ();
  ResultCode_t          result                  = RESULT_CODE_OK;
  Int32                 cid;

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT+CGDEL=? */
    {
      /* Do nothing - leave result code as OK */
      break;
    }
    case EXTENDED_ASSIGN:   /* AT+CGDEL=  */
    {
      /* get the CID */
      (void)getExtendedParameter (commandBuffer_p, &cid, ULONG_MAX);

      if (cid == ULONG_MAX)
      {
        /* No CID so delete all non-active PDP contexts */
        vgDeleteAllNonActiveContexts (entity);
      }
      /* If the cid is invalid or does not exist then error. */
      else if ((cid >= vgGpGetMinCidValue(entity)) &&(cid < MAX_NUMBER_OF_CIDS))
      {
        if ((gprsGenericContext_p->cidUserData [cid] != PNULL) &&
            !(gprsGenericContext_p->cidUserData [cid]->isActive))
        {

          /* Delete CID data plus any associated secondaries */
          vgPutNewLine (entity);
          vgDeleteNonActiveContext (entity, cid, TRUE);
          vgPutNewLine (entity);
        }
        else
        {
          result = VG_CME_INVALID_CID_VALUE;
        }
      }
      else
      {
        result = VG_CME_INVALID_CID_VALUE;
      }
      break;
    }
    case EXTENDED_ACTION:   /* AT+CGDEL   */
      /* No CID so delete all non-active PDP contexts */
      vgDeleteAllNonActiveContexts (entity);
      break;
    /* Query does nothing */
    case EXTENDED_QUERY:    /* AT+CGDEL?  */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGpCGAUTH
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Allows the user to set the username and password associated
*              with a particular PDP context/PDN connection (CID).
*-------------------------------------------------------------------------*/
ResultCode_t vgGpCGAUTH    (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation                                   = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t  *gprsGenericContext_p                       = ptrToGprsGenericContext ();
  VgPsdStatusInfo       *cmd_p                                      = PNULL;
  ResultCode_t          result                                      = RESULT_CODE_OK;
  PsdUser               *psdUser                                    = PNULL;
  Int32                 cid;
  Int32                 authProt                                    = PPP_AUTH_PAP;
  Char                  tmpUsernameString[MAX_PSD_USERNAME_LENGTH]  = {0};
  Char                  tmpPasswdString[MAX_PSD_PWD_LENGTH]         = {0};
  Int16                 tmpUsernameLen                              = 0;
  Int16                 tmpPasswdLen                                = 0;

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT*CGAUTH=? */
    {
      /* Display the range of input values */
      vgPutNewLine (entity);
#if (MAX_NUMBER_OF_CIDS == 2)
      vgPrintf (entity, (const Char*)"%C: (1),(%d-%d),(\"\"),(\"\")",
                        PPP_AUTH_NONE, PPP_AUTH_PAP);
#else
      vgPrintf (entity, (const Char*)"%C: (1-%d),(%d-%d),(\"\"),(\"\")",
                        (MAX_NUMBER_OF_CIDS - 1),
                        PPP_AUTH_NONE, PPP_AUTH_PAP);
#endif
      vgPutNewLine (entity);
      break;
    }

    case EXTENDED_QUERY:    /* AT+CGAUTH?  */
    {
      vgDisplayAllUsernamePasswordInfo(entity);
      break;
    }

    case EXTENDED_ASSIGN:   /* AT*CGAUTH=  */
    {
      /* get the cid */
      if (getExtendedParameter (commandBuffer_p, &cid, ULONG_MAX))
      {
        /* if no cid found then cid will = ulong_max */
        if ((cid >= vgGpGetMinCidValue(entity)) && (cid < MAX_NUMBER_OF_CIDS))
        {
          if(((cmd_p = gprsGenericContext_p->cidUserData [cid]) != PNULL) &&
              (cmd_p->profileDefined))

          {
            psdUser = &(cmd_p->psdBearerInfo.psdUser);
          }
          else
          {
            result = VG_CME_INVALID_CID_VALUE;
          }
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
      }
      else
      {
        /* you must supply a valid cid */
        result = RESULT_CODE_ERROR;
      }

      if (result == RESULT_CODE_OK)
      {
        /* Now get the auth protocol */
        if (getExtendedParameter (commandBuffer_p, &authProt, ULONG_MAX))
        {
          /* if no auth protocol found then auth protocol will = ulong_max,
           * otherwise we only permit PAP or none. */
          if (authProt == ULONG_MAX)
          {
            /* Do nothing */
          }
          else if ((authProt == PPP_AUTH_NONE) ||
                   (authProt == PPP_AUTH_PAP))
          {
            /* Do nothing */
          }
          else
          {
            result = RESULT_CODE_ERROR;
          }
        }
        else
        {
          /* not a valid auth protocol number */
          result = RESULT_CODE_ERROR;
        }
      }

      /* The user wants to delete the username and password for this
       * cid - if it isn't clear already */
      if (authProt == PPP_AUTH_NONE)
      {
        vgInitialisePsdUserData (cmd_p);
      }
      else
      {
        if (RESULT_CODE_OK == result)
        {
          /* now get the username data */
          if (getExtendedString(commandBuffer_p,
                                tmpUsernameString,
                                MAX_PSD_USERNAME_LENGTH,
                                &tmpUsernameLen))
          {
#if defined (ATCI_SLIM_DISABLE)

            FatalCheck(psdUser != PNULL, 0, 0, 0);
#endif
            if (tmpUsernameLen != 0)
            {
              psdUser->usernamePresent = TRUE;
              memcpy (psdUser->username, tmpUsernameString, tmpUsernameLen);
              psdUser->usernameLength = (Int8)tmpUsernameLen;
              psdUser->username[psdUser->usernameLength] = (Char)0;
            }
          }
          else
          {
            result = RESULT_CODE_ERROR;
          }
        }

        if (RESULT_CODE_OK == result)
        {
          /* now get the password data */
          if (getExtendedString(commandBuffer_p,
                                tmpPasswdString,
                                MAX_PSD_PWD_LENGTH,
                                &tmpPasswdLen))
          {
#if defined (ATCI_SLIM_DISABLE)

            FatalCheck(psdUser != PNULL, 0, 0, 0);
#endif
            if (tmpPasswdLen != 0)
            {
              psdUser->passwdPresent = TRUE;
              memcpy (psdUser->passwd, tmpPasswdString, tmpPasswdLen);
              psdUser->passwdLength = (Int8)tmpPasswdLen;
              psdUser->passwd[psdUser->passwdLength] = (Char)0;
            }
          }
          else
          {
            result = RESULT_CODE_ERROR;
          }
        }

        /* only the CID was supplied - so display the authProt, username and
         * password for this CID.
         */
        if ((result == RESULT_CODE_OK) &&
            (tmpUsernameLen == 0))
        {
          if (tmpPasswdLen == 0)
          {
            if (authProt == ULONG_MAX)
            {
              vgDisplayUsernamePasswordInfo(entity, cid, TRUE);
            }
            else
            {
              /* The auth protocol was present - but there was no username or
               * password. This is not allowed.
               */
              result = RESULT_CODE_ERROR;
            }
          }
          else
          {
            /* Can't have a blank username if there was a password */
            result = RESULT_CODE_ERROR;
          }
        }
      }
      break;
    }

    case EXTENDED_ACTION:   /* AT*CGAUTH   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgGpCEREG
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: Executes command AT+CEREG, displaying PLMN state
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgGpCEREG (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  ResultCode_t          result = RESULT_CODE_OK;
  Int32                 param;

  switch (operation)
  {
    case EXTENDED_QUERY:     /* +CEREG? */
    {
      vgSendCEREGData (entity, TRUE, (VgCEREGMode)getProfileValue (entity, PROF_CEREG));
      break;
    }
    case EXTENDED_RANGE:     /* +CEREG=? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char *)"+CEREG: (%d-%d)",VG_CEREG_DISABLED,
        VG_CEREG_ENABLED_PSM_INFO_AND_CAUSE );
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_ASSIGN:    /* +CEREG= */
    {
        if (getExtendedParameter (commandBuffer_p,
                &param,
                VG_CEREG_NUMBER_OF_SETTINGS) == TRUE)
        {
            if (param < VG_CEREG_NUMBER_OF_SETTINGS)
            {
                result = setProfileValue (entity, PROF_CEREG, (Int8)param);
#if 0
                /* For AP do not send this */
                if(param != VG_CEREG_DISABLED)
                    vgSendCEREGData (entity, FALSE, (VgCEREGMode)param);
#endif
            }
            else
            {
                result = RESULT_CODE_ERROR;
            }
        }
        else
        {
            result = RESULT_CODE_ERROR;
        }
        break;
    }
    case EXTENDED_ACTION:
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgGpMDPDNP
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: Executes command AT+CEREG, displaying PLMN state
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgGpMDPDNP (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t      *gprsGenericContext_p   = ptrToGprsGenericContext ();
  MobilityContext_t     *mobilityContext_p      = ptrToMobilityContext ();
  VgCGREGData          *vgCGREGData             = &(gprsGenericContext_p->vgCGREGData);
  VgMDPDNPData              *vgMdpdnpData         = &(gprsGenericContext_p->vgMDPDNPData);
  VgCipcaData           *vgCipcaData_p          = &mobilityContext_p->vgCipcaData;
  ResultCode_t          result = RESULT_CODE_OK;
  Int32                 param;

  switch (operation)
  {
    case EXTENDED_QUERY:     /* *MDPDNP? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char *)"*MDPDNP: %d",getProfileValue(entity, PROF_MDPDNP));
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_RANGE:     /* *MDPDNP=? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char *)"*MDPDNP: (%d-%d)",VG_MDPDNP_DISABLED,
        VG_MDPDNP_ENABLED);
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_ASSIGN:    /* *MDPDNP= */
    {
        if (getExtendedParameter (commandBuffer_p,
                &param,
                VG_MDPDNP_NUMBER_OF_SETTINGS) == TRUE)
        {
            if (param < VG_MDPDNP_NUMBER_OF_SETTINGS)
            {
                result = setProfileValue (entity, PROF_MDPDNP, (Int8)param);
                if((param == VG_MDPDNP_ENABLED) && (vgCipcaData_p->vgCipcaOpt == VG_CIPCA_ATTACH_WITH_PDN) &&
                  ((vgCGREGData->regStatus == VGMNL_REGISTRATED_HOME) || (vgCGREGData->regStatus == VGMNL_REGISTRATED_ROAMING) ||
                  (vgCGREGData->regStatus == VGMNL_REG_HOME_SMS_ONLY) || (vgCGREGData->regStatus == VGMNL_REG_ROAMING_SMS_ONLY)))
                {
                  vgSendMDPDNPData(entity, &(vgMdpdnpData->defaultApnFromNw),vgMdpdnpData->defaultPdnTypeFromNw);
                }
            }
            else
            {
                result = RESULT_CODE_ERROR;
            }
        }
        else
        {
            result = RESULT_CODE_ERROR;
        }
        break;
    }
    case EXTENDED_ACTION:
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

#if defined (FEA_UPDIR)
/*--------------------------------------------------------------------------
 *
 * Function:    vgGpMUPDIR
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: Executes command AT*MUPDIR, setting UDP packet delivery indication
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgGpMUPDIR (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgMUPDIRData          *vgMupdirData = &(gprsGenericContext_p->vgMUPDIRData);
  ResultCode_t          result = RESULT_CODE_OK;
  Int32                 param;
  TextualPdnAddress     textualPdnAddress;
  Char                  tmpString [MAX_TEXTUAL_PDN_ADDR + NULL_TERMINATOR_LENGTH] = {0};
  Int16                 tmpStringLen = 0;
  PdnType               pdnType = PDN_TYPE_IPV4;
  Boolean               strFound = FALSE;

  switch (operation)
  {
    case EXTENDED_RANGE:     /* *MUPDIR=? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char *)"*MUPDIR: (%d-%d)", VG_MUPDIR_DISABLED, VG_MUPDIR_ENABLED);
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_ASSIGN:    /* *MUPDIR= */
    {
      /* Get mode */
      if (getExtendedParameter (commandBuffer_p,
              &param,
              VG_MUPDIR_NUMBER_OF_SETTINGS) == TRUE)
      {
        if (param >= VG_MUPDIR_NUMBER_OF_SETTINGS)
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }

        if(result == RESULT_CODE_OK)
        {
          /* Disable track of packets */
          if(param ==  VG_MUPDIR_DISABLED)
          {
            /* Get pattern id */
            if (getExtendedParameter (commandBuffer_p,
                            &param,
                            ULONG_MAX) == TRUE)
            {
              if(param == ULONG_MAX)
              {
                result = VG_CME_INVALID_INPUT_VALUE;
              }
              else
              {
                vgMupdirData->patternId = (Int8)param;
              }
            }
            else
            {
              result = VG_CME_INVALID_INPUT_VALUE;
            }

            if(result == RESULT_CODE_OK)
            {
              result = vgChManContinueAction(entity, SIG_APEX_ABPD_RESET_UPDIR_INFO_REQ);

              if(result == RESULT_CODE_PROCEEDING)
              {
                result = RESULT_CODE_OK;
              }
            }
          }
          /* Enable track of packets */
          else
          {
            /* Get IP type */
            if (getExtendedString (commandBuffer_p,
                                   tmpString,
                                   STRING_LENGTH_40,
                                   &tmpStringLen))
            {
              if (tmpStringLen == 0)
              {
                result = VG_CME_PDP_TYPE_NOT_SUPPORTED;
              }
              else
              {
                strFound = vgPDNStrToEnum (tmpString, &pdnType);
                if (strFound == FALSE)
                {
                  result = RESULT_CODE_ERROR;
                }
                else
                {
                  if ((pdnType == PDN_TYPE_IPV4) ||
                      (pdnType == PDN_TYPE_IPV6))
                  {
                    vgMupdirData->srcIpAddress.pdnType = pdnType;
                    vgMupdirData->dstIpAddress.pdnType = pdnType;
                  }
                  else
                  {
                    result = VG_CME_PDP_TYPE_NOT_SUPPORTED;
                  }
                }
              }
              memset (tmpString, NULL_CHAR, sizeof(tmpString));
            }
            else
            {
              result = RESULT_CODE_ERROR;
            }

            /* Get Source IP address */
            if(result == RESULT_CODE_OK)
            {
              if (getExtendedString (commandBuffer_p,
                                     tmpString,
                                     MAX_TEXTUAL_PDN_ADDR,
                                     &tmpStringLen))
              {
                if (tmpStringLen == 0)
                {
                  vgMupdirData->srcIpAddress.addressPresent = FALSE;
                }
                else
                {
                  textualPdnAddress.addressPresent  = TRUE;
                  textualPdnAddress.length = (Int8)tmpStringLen;
                  memcpy (textualPdnAddress.address, tmpString, tmpStringLen);
                  textualPdnAddress.address[tmpStringLen] = '\0';

                  switch (pdnType)
                  {
                    case PDN_TYPE_IPV4:
                      result = vgStringToPDNAddr (pdnType,
                                                  &textualPdnAddress,
                                                  &vgMupdirData->srcIpAddress);
                      break;
                    case PDN_TYPE_IPV6:
                      result = vgStringToPDNAddr (pdnType,
                                                  &textualPdnAddress,
                                                  &vgMupdirData->srcIpAddress);
                      break;
                    default:
                      result = RESULT_CODE_ERROR;
                      break;
                  }
                }
                memset (tmpString, NULL_CHAR, sizeof (tmpString));
              }
              else
              {
                result = VG_CME_INVALID_INPUT_VALUE;
              }
            }

            /* Get Source UDP Port */
            if(result == RESULT_CODE_OK)
            {
              if (getExtendedParameter (commandBuffer_p,
                              &param,
                              ULONG_MAX) == TRUE)
              {
                if(param == ULONG_MAX)
                {
                  vgMupdirData->srcPortPresent = FALSE;
                }
                else if(param > 65535)
                {
                  result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                  vgMupdirData->srcPortPresent = TRUE;
                  vgMupdirData->srcPort = (Int16)param;
                }
              }
              else
              {
                result = VG_CME_INVALID_INPUT_VALUE;
              }
            }

            /* Get destination IP address */
            if(result == RESULT_CODE_OK)
            {
              if (getExtendedString (commandBuffer_p,
                                     tmpString,
                                     MAX_TEXTUAL_PDN_ADDR,
                                     &tmpStringLen))
              {
                if (tmpStringLen == 0)
                {
                  vgMupdirData->dstIpAddress.addressPresent = FALSE;
                }
                else
                {
                  textualPdnAddress.addressPresent  = TRUE;
                  textualPdnAddress.length = (Int8)tmpStringLen;
                  memcpy (textualPdnAddress.address, tmpString, tmpStringLen);
                  textualPdnAddress.address[tmpStringLen] = '\0';

                  switch (pdnType)
                  {
                    case PDN_TYPE_IPV4:
                      result = vgStringToPDNAddr (pdnType,
                                                  &textualPdnAddress,
                                                  &vgMupdirData->dstIpAddress);
                      break;
                    case PDN_TYPE_IPV6:
                      result = vgStringToPDNAddr (pdnType,
                                                  &textualPdnAddress,
                                                  &vgMupdirData->dstIpAddress);
                      break;
                    default:
                      result = RESULT_CODE_ERROR;
                      break;
                  }
                }
                memset (tmpString, NULL_CHAR, sizeof (tmpString));
              }
              else
              {
                result = VG_CME_INVALID_INPUT_VALUE;
              }
            }

            /* Get Destination UDP Port */
            if(result == RESULT_CODE_OK)
            {
              if (getExtendedParameter (commandBuffer_p,
                              &param,
                              ULONG_MAX) == TRUE)
              {
                if(param == ULONG_MAX)
                {
                  vgMupdirData->dstPortPresent = FALSE;
                }
                else if(param > 65535)
                {
                  result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                  vgMupdirData->dstPortPresent = TRUE;
                  vgMupdirData->dstPort = (Int16)param;
                }
              }
              else
              {
                result = VG_CME_INVALID_INPUT_VALUE;
              }
            }

            /* Get Message ID offset */
            if(result == RESULT_CODE_OK)
            {
              if (getExtendedParameter (commandBuffer_p,
                              &param,
                              ULONG_MAX) == TRUE)
              {
                if((param == ULONG_MAX) || (param > 1500))
                {
                  result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                  vgMupdirData->msgIdOffset = (Int16)param;
                }
              }
              else
              {
                result = VG_CME_INVALID_INPUT_VALUE;
              }
            }

            /* Get Message ID size */
            if(result == RESULT_CODE_OK)
            {
              if (getExtendedParameter (commandBuffer_p,
                              &param,
                              ULONG_MAX) == TRUE)
              {
                if((param == ULONG_MAX) ||
                   (param == 0) ||
                   (param > 2))
                {
                  result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                  vgMupdirData->msgIdSize = (Int8)param;
                }
              }
              else
              {
                result = VG_CME_INVALID_INPUT_VALUE;
              }
            }

            if(result == RESULT_CODE_OK)
            {
              vgMupdirData->vgMupdiEntity = entity;
              result = vgChManContinueAction(entity, SIG_APEX_ABPD_SET_UPDIR_INFO_REQ);
            }
          }
        }
      }
      else
      {
        result = VG_CME_INVALID_INPUT_VALUE;
      }
      break;
    }
    case EXTENDED_QUERY:     /* *MUPDIR? */
    {
      break;
    }
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}
#endif

#if defined (FEA_QOS_TFT)
/*--------------------------------------------------------------------------
*
* Function:    vgGpCGEQOS
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Allows the user to set LTE specific QOS parameters.  It cannot
*              read back what was assigned by the network.  AT command
*              AT+CGEQOSRDP is used for that.
*
*-------------------------------------------------------------------------*/
ResultCode_t vgGpCGEQOS (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo       *cmd_p = PNULL;
  ResultCode_t          result = RESULT_CODE_OK;
  QualityOfService      qos = {0};
  Int32                 cid;
  Int32                 tmpVal;
  Boolean               qciDefd = FALSE;

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT+CGEQOS=? */
    {
      vgPutNewLine (entity);

      /* Display AT+CGEQOS range */
      vgPrintf (entity, (const Char*)"+CGEQOS: (%d-%d),(%d-%d),(0-%d),(0-%d),(0-%d),(0-%d)",
                vgGpGetMinCidValue(entity),
                MAX_NUMBER_OF_CIDS,
                MIN_QCI,
                MAX_QCI,
                MAX_DL_GBR,
                MAX_UL_GBR,
                MAX_DL_MBR,
                MAX_UL_MBR);
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_QUERY:    /* AT+CGEQOS?  */
    {
      viewEqosAttributes (entity);
      break;
    }

    case EXTENDED_ASSIGN:   /* AT+CGEQOS=  */
    {

      /* get the cid */
      if (getExtendedParameter (commandBuffer_p, &cid, ULONG_MAX))
      {
        /* if no cid found then cid will = ulong_max */
        if ((cid >= vgGpGetMinCidValue(entity)) && (cid < MAX_NUMBER_OF_CIDS))
        {

          if (vgAllocateRamToCid ( cid ))
          {
            cmd_p = gprsGenericContext_p->cidUserData [cid];
#if defined (ATCI_SLIM_DISABLE)

            FatalCheck(cmd_p!=PNULL, entity, cid, 0);
#endif
          }
          else
          {
            result = VG_CME_INSUFFIC_RESOURCES;
          }
        }
        else
        {
          result = VG_CME_INVALID_CID_VALUE;
        }
      }
      else
      {
        /* you must supply a valid cid */
        result = RESULT_CODE_ERROR;
      }

      /* now get the required EQOS data */
      if (result == RESULT_CODE_OK)
      {
#if defined (ATCI_SLIM_DISABLE)

        FatalCheck(cmd_p!=PNULL, entity, cid, 0);
#endif
        /* Take a copy of the QoS parameters */
        qos = cmd_p->psdBearerInfo.requiredQos;

        /* Get the QCI */
        if (getExtendedParameter (commandBuffer_p, &tmpVal, ULONG_MAX))
        {
          /* if no QCI found then QCI will = ulong_max */
          if (tmpVal <= MAX_QCI)
          {
            qos.epsQci = (Int8) tmpVal;
            qciDefd = TRUE;

          }
          /* It is OK not to supply the QCI, as long as all the other parameters
           * are not present - then it means the QoS information is to be reset
           */
          else if (tmpVal != ULONG_MAX)
          {
            result = VG_CME_INVALID_INPUT_VALUE;
          }
        }
        else
        {
          /* you must supply a valid QCI */
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }

      if (result == RESULT_CODE_OK)
      {
        /* The bit rates are set as not present initially */
        qos.epsBitRatesPresent = FALSE;

        /* NOTE: If some (but not all) GBR or MBR are missing then they are set to
         * 0.  If they are all missing then we set them as all 0 and present field is FALSE
         */

        /* Get the DL_GBR */
        if (getExtendedParameter (commandBuffer_p, &tmpVal, ULONG_MAX))
        {
          /* If the parameter is valid - QCI must also be present and set to
           * something other than 0
           */
          if ((tmpVal <= MAX_DL_GBR) && (qciDefd == TRUE) && (qos.epsQci != MIN_QCI))
          {
            /* we have at least one parameter for Bit rates so set the epsBitRatesPresent field to TRUE */
            qos.epsBitRatesPresent = TRUE;
            qos.epsGuaranteedBitRateDownlink = tmpVal;

          }
          /* It's OK to have this parameter missing
           *
           */
          else if (tmpVal != ULONG_MAX)
          {
            result = VG_CME_INVALID_INPUT_VALUE;
          }
          else
          {
            /* No parameter present to set it to 0 */
            qos.epsGuaranteedBitRateDownlink = 0;
          }
        }
        else
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }

      if (result == RESULT_CODE_OK)
      {
        /* Get the UL_GBR */
        if (getExtendedParameter (commandBuffer_p, &tmpVal, ULONG_MAX))
        {
          /* If the parameter is valid - QCI must also be present and set to
           * something other than 0
           */
          if ((tmpVal <= MAX_UL_GBR) && (qciDefd == TRUE) && (qos.epsQci != MIN_QCI))
          {
            /* we have at least one parameter for Bit rates so set the epsBitRatesPresent field to TRUE */
            qos.epsBitRatesPresent = TRUE;
            qos.epsGuaranteedBitRateUplink = tmpVal;

          }
          else if (tmpVal != ULONG_MAX)
          {
            result = VG_CME_INVALID_INPUT_VALUE;
          }
          else
          {
            /* No parameter present to set it to 0 */
            qos.epsGuaranteedBitRateUplink = 0;
          }
        }
        else
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }

      if (result == RESULT_CODE_OK)
      {
        /* Get the DL_MBR */
        if (getExtendedParameter (commandBuffer_p, &tmpVal, ULONG_MAX))
        {
          /* If the parameter is valid - QCI must also be present and set to
           * something other than 0
           */
          if ((tmpVal <= MAX_DL_MBR) && (qciDefd == TRUE) && (qos.epsQci != MIN_QCI))
          {
            /* we have at least one parameter for Bit rates so set the epsBitRatesPresent field to TRUE */
            qos.epsBitRatesPresent = TRUE;
            qos.epsMaxBitRateDownlink = tmpVal;

          }
          else if (tmpVal != ULONG_MAX)
          {
            result = VG_CME_INVALID_INPUT_VALUE;
          }
          else
          {
            /* No parameter present to set it to 0 */
            qos.epsMaxBitRateDownlink = 0;
          }
        }
        else
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }

      }

      if (result == RESULT_CODE_OK)
      {
        /* Get the UL_MBR */
        if (getExtendedParameter (commandBuffer_p, &tmpVal, ULONG_MAX))
        {
          /* If the parameter is valid - QCI must also be present and set to
           * something other than 0
           */
          if ((tmpVal <= MAX_UL_MBR) && (qciDefd == TRUE) && (qos.epsQci != MIN_QCI))
          {
            /* we have at least one parameter for Bit rates so set the epsBitRatesPresent field to TRUE */
            qos.epsBitRatesPresent = TRUE;
            qos.epsMaxBitRateUplink = tmpVal;

          }
          else if (tmpVal != ULONG_MAX)
          {
            result = VG_CME_INVALID_INPUT_VALUE;
          }
          else
          {
            /* No parameter present to set it to 0 */
            qos.epsMaxBitRateUplink = 0;
          }
        }
        else
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }

      }

      /* We now have all the parameters */
      if ( result == RESULT_CODE_OK)
      {
        if (vgOpManCidActive((Int8)cid))
        {
#if defined (ATCI_SLIM_DISABLE)

          FatalCheck(cmd_p!=PNULL, entity, cid, 0);
#endif
          if (cmd_p->modifyPending == FALSE)
          {
            cmd_p->modifyPending = TRUE;
          }
        }
      }

      if (result == RESULT_CODE_OK)
      {
#if defined (ATCI_SLIM_DISABLE)

        FatalCheck(cmd_p!=PNULL, entity, cid, 0);
#endif
        /* this AT command can define a new profile, only if everything is Ok */
        if (qciDefd)
        {
          /* If none of the bit rates were present then we reset them all to 0
           * otherwise leave them alone.
           */
          if (qos.epsBitRatesPresent == FALSE)
          {
            qos.epsGuaranteedBitRateDownlink = 0;
            qos.epsGuaranteedBitRateUplink   = 0;
            qos.epsMaxBitRateDownlink        = 0;
            qos.epsMaxBitRateUplink          = 0;
          }

          cmd_p->profileDefined = TRUE;
          cmd_p->cgeqosDefined = TRUE;
          cmd_p->psdBearerInfo.requiredQos = qos;


        }
        else
        {
          /* Undefine cgeqos entry */
          if (cmd_p->cgeqosDefined)
          {
            vgInitialiseCgeqosData (cmd_p);
            /* Check if the profile needs to be undefined */
            vgCheckProfileUndefined (cmd_p, cid);
          }
        }

        /* There is no need to inform AB task ABPD module about req EQoS values at this point
         * during context activation all context parameters will be supplied */
        /* If the QoS profile is not defined in the command line,
         * the requested profile for context number <cid> will become undefined.*/
        cmd_p->psdBearerInfo.requiredQos.epsQualityOfServicePresent = qciDefd;
        cmd_p->psdBearerInfo.reqQosPresent = qciDefd;
      }
      break;
    }
    /* Action do nothing */
    case EXTENDED_ACTION:   /* AT+CGEQOS   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgGpCGEQOSRDP
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Allows the user to view LTE specific QOS parameters assigned by
*              the network.
*
*-------------------------------------------------------------------------*/
ResultCode_t vgGpCGEQOSRDP (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation               = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t  *gprsGenericContext_p   = ptrToGprsGenericContext ();
  ResultCode_t          result                  = RESULT_CODE_OK;
  Int32                 cid;

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT+CGEQOSRDP=? */
    {
      vgPrintActiveCids ((const Char*)"+CGEQOSRDP:", entity, TRUE, TRUE);
      break;
    }

    case EXTENDED_ASSIGN:   /* AT+CGEQOSRDP=  */
    {
      /* get the CID */
      (void)getExtendedParameter (commandBuffer_p, &cid, ULONG_MAX);
      /* No CID provided so we have to go through and display the
       * CGSCONTRDP values for all active secondary CIDs
       */
      if (cid == ULONG_MAX)
      {
         vgDisplayAllCGEQOSRDPInfo (entity);
      }
      /* If the cid is invalid or not secondary or does not exist then error. */
      else if ((cid >= vgGpGetMinCidValue(entity)) &&(cid < MAX_NUMBER_OF_CIDS))
      {
        if ((gprsGenericContext_p->cidUserData [cid] != PNULL) &&
            (gprsGenericContext_p->cidUserData [cid]->profileDefined) &&
            (gprsGenericContext_p->cidUserData [cid]->isActive))

        {
          vgPutNewLine (entity);
          vgDisplayCGEQOSRDPInfo(entity, (Int8)cid);
        }
        else
        {
          result = VG_CME_INVALID_CID_VALUE;
        }
      }
      else
      {
        result = VG_CME_INVALID_CID_VALUE;
      }
      break;
    }
    case EXTENDED_ACTION:   /* AT+CGEQOSRDP   */
      /* No CID provided so we have to go through and display the
       * CGEQOSRDP values for all active CIDs
       */
      vgDisplayAllCGEQOSRDPInfo (entity);
      break;
    /* Query does nothing */
    case EXTENDED_QUERY:    /* AT+CGEQOSRDP?  */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}
#endif /* FEA_QOS_TFT */

/*--------------------------------------------------------------------------
*
* Function:     vgActivateAttachDefBearerContext
*
* Parameters:   entity      - the entity which wants to connect to the
*
* Returns:      Boolean     - Nothing
*
* Description:  Initiates a PDP context activation to connect to the default
*               bearer which was activated during the attach procedure.  This
*               is done during ATCI startup - or when we register to the LTE
*               network. This is not triggered by any AT command, and will only
*               occur if AT*MLTEGCF is set to 1 or 2.
*               This function will also overwrite the entity number if
*               AT*MLTEGCFLOCK is set on a particular channel which is
*               already enabled.
*
*-------------------------------------------------------------------------*/
void    vgActivateAttachDefBearerContext (const VgmuxChannelNumber entity)
{
  GeneralGenericContext_t       *generalGenericContext_p    = ptrToGeneralGenericContext();
  GprsGenericContext_t          *gprsGenericContext_p       = ptrToGprsGenericContext();
  Int8                          cid;
  VgPsdStatusInfo               *vgPsdStatusInfo_p          = PNULL;
  VgmuxChannelNumber            profileEntity;
  VgmuxChannelNumber            entityToUse;
  Boolean                       found                       = FALSE;

  /* Should not already be connected */
  FatalCheck (gprsGenericContext_p->vgLteAttachDefaultBearerState == LTE_ATTACH_DEFAULT_BEARER_NOT_CONNECTED,
              entity,
              gprsGenericContext_p->lteAttachDefaultBearerCid,
              gprsGenericContext_p->vgLteAttachDefaultBearerState);

  FatalCheck (entity < CI_MAX_ENTITIES,
              entity,
              gprsGenericContext_p->lteAttachDefaultBearerCid,
              gprsGenericContext_p->vgLteAttachDefaultBearerState);

  entityToUse = entity;

  /* Check if any channel has LTEGCFLOCK enabled.  If so, then we must use
   * that channel rather than "entity" passed to this function (although they may
   * well be the same number).
   * NOTE: There can only ever be one channel with LTEGCFLOCK enabled.
   * If we cannot find anything then we will continue to use the channel
   * we have been given.
   */
  for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
  {
    if ((isEntityActive (profileEntity)) && (getProfileValue(profileEntity, PROF_MLTEGCFLOCK)))
    {
      FatalCheck (found == FALSE, entity, entityToUse, profileEntity);
      entityToUse = profileEntity;
      found = TRUE;
    }
  }

  if (generalGenericContext_p->initDataFromABPDState != READY)
  {
    /* We can only activate the context if we have loaded in the
     * data from ABPD.  Otherwise - we will need to delay it until
     * the data is received from ABPD.
     */
    gprsGenericContext_p->vgLteAttachDefaultBearerState = LTE_ATTACH_DEFAULT_BEARER_PENDING_ABPD_DATA;
  }
  else
  {
    /* Firstly get the first free CID.
     * Note that for LTEGCF test mode = 1 this CID number will get
     * changed once the bearer is activated in order to align the CID to the
     * PSD Bearer ID as per the GCF requirements.
     * For LTEGCF test mode = 2, then we pick up CID 0 here and it will not
     * need to be re-assigned later.  We will ALWAYS get CID 0 no matter if
     * the CID was assigned or not previously using AT+CGDCONT  (would not
     * recommend doing that anyway..)
     */

    /* If we cannot get any CID - then something has gone wrong */
    if (!vgGetUnusedCid (entityToUse, PSD_BEARER_ID_UNASSIGNED, &cid))
    {
      FatalParam (entity, 0, 0);
    }

    if (vgAllocateRamToCid (cid))
    {
      vgPsdStatusInfo_p = gprsGenericContext_p->cidUserData [cid];

      if (vgPsdStatusInfo_p != PNULL)
      {
        /* Give the primary connection a default setting - in this case
         * IPV4 for the PDN Type..
         */
        vgPsdStatusInfo_p->psdBearerInfo.reqPdnAddress.addressPresent = FALSE;

        /* Get the PDN type from the default APN information read from ABPD.
         */
        vgPsdStatusInfo_p->psdBearerInfo.reqPdnAddress.pdnType = gprsGenericContext_p->currentDefaultAPN.pdnType;

        vgPsdStatusInfo_p->profileDefined = TRUE;
        vgPsdStatusInfo_p->cgdcontDefined = TRUE;
        vgPsdStatusInfo_p->psdBearerInfo.channelNumber = entityToUse;

        gprsGenericContext_p->vgLteAttachDefaultBearerState = LTE_ATTACH_DEFAULT_BEARER_PENDING_CONNECTION;
        gprsGenericContext_p->lteAttachDefaultBearerCid = cid;

        /* This is all internal - so the user will not see if this was OK
         * or failed.
         * It is always ABPD_CONN_TYPE_NONE.
         */
        (void)vgActivateContext (cid, entityToUse, ABPD_CONN_TYPE_NONE);
      }
      else
      {
        FatalParam (entity, cid, 0);
      }
    }
    else
    {
      FatalParam (entity, cid, 0);
    }
  }
}


/*--------------------------------------------------------------------------
*
* Function:     vgGpAbortAttach
*
* Parameters:   entity      - the entity which wants to connect to the
*
* Returns:      Boolean     - Nothing
*
* Description:  If we were trying to attach using AT+CGATT then
*               now abort that (i.e. initiated a detach).
*
*-------------------------------------------------------------------------*/
void    vgGpAbortAttach (const VgmuxChannelNumber entity)
{
  GprsContext_t *gprsContext_p = ptrToGprsContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(gprsContext_p != PNULL, entity, 0, 0);
#endif
  if (gprsContext_p->attachInProgress)
  {
    /* Only abort the attach */
    gprsContext_p->attachInProgress = FALSE;
    gprsContext_p->attachAbortInProgress = TRUE;

    /* Stop waiting for the ATTACH_CNF - NOTE: Maybe a crossover here. If we get
     * the signal after - then just throw it away.
     */
    deleteSolicitedSignalRecordForEntity (entity);

    (void)vgChManContinueAction (entity, SIG_APEX_ABPD_PSD_DETACH_REQ);
  }
}

/*--------------------------------------------------------------------------
*
* Function:     vgGpGetMinCidValue
*
* Parameters:   entity

* Returns:      Int8     - Current minimum CID value
*
* Description:  Returns current minimum CID value - which is dependent on the
*               value of AT*MLTEGCF
*
*-------------------------------------------------------------------------*/
Int8    vgGpGetMinCidValue (const VgmuxChannelNumber entity)
{
  Int8 retVal = MIN_CID_VALUE_CID1;

  /* For NB-IOT we need to check if we are allowed to activate default
   * PDN connection on attach - before doing anything (set by AT+CIPCA)
   */
  if ((getProfileValue (entity, PROF_MLTEGCF) == GCF_TEST_CID0_ENABLE) &&
       vgCIPCAPermitsActivateAttachDefBearer())
  {
    retVal = MIN_CID_VALUE_CID0;
  }

  return (retVal);
}

/*--------------------------------------------------------------------------
*
* Function:   vgGpDtAddConnIdFromActiveCid
*
* Parameters:  cidNum
*
* Returns:     Resultcode_t    - result of function
*
* Description: Find if the cid is active and if it is what is the associated connId then add that to the
info for *MNBIOTDT extended assign
*
*-------------------------------------------------------------------------*/

void vgGpDtAddConnIdFromActiveCid( Int8 cidNum, AbpdApnDataType dataType, Boolean *isValid_p)
{
    VgPsdStatusInfo       *cidUserData_p;
    GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();

    cidUserData_p = gprsGenericContext_p->cidUserData[cidNum];

    if ((cidUserData_p != PNULL) && (cidUserData_p->isActive) &&
                  (cidUserData_p->psdBearerInfo.connId < MAX_NUM_CONN_IDS ))
    {
       gprsGenericContext_p->vgReqNbiotDtData.apnDataTypeEntity[cidUserData_p->psdBearerInfo.connId].abpdApnDataTypeValidity = TRUE;
       gprsGenericContext_p->vgReqNbiotDtData.apnDataTypeEntity[cidUserData_p->psdBearerInfo.connId].abpdApnDataType = dataType;
       *isValid_p = TRUE;
    }
}

/*--------------------------------------------------------------------------
*
* Function:    vgGpMNBIOTDT
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Allows the user to set the NB-IOT data type per APN (Normal or Exceptional data)
*
*-------------------------------------------------------------------------*/
ResultCode_t vgGpMNBIOTDT    (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);

  ResultCode_t          result = RESULT_CODE_OK;
  Int32                 param1, param2, param3     = 0;
  Int8                  x;
  Boolean               validData = FALSE;
  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT*MNBIOTDT=? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"*MNBIOTDT: 0,1" );
      vgPutNewLine (entity);
      break;
    }

    case EXTENDED_ASSIGN:   /* AT*MNBIOTDT=  */
    {
       if( (getExtendedParameter(  commandBuffer_p,
                                            &param1,
                                            ULONG_MAX) == FALSE) ||
                    (param1 > ABPD_APN_DATA_TYPE_EXCEPTIONAL) )// get data type
       {
          result = VG_CME_INVALID_INPUT_VALUE;
       }
       else
       {
          if( result == RESULT_CODE_OK)
          {
             if (getExtendedParameter(  commandBuffer_p,
                                         &param2,
                                         ULONG_MAX) == FALSE)
             {
                result = VG_CME_INVALID_INPUT_VALUE;
             }
             else if(param2 == ULONG_MAX) // if no specific cids are included
             {
                //find connIds for all active cids
                for (x = 0; x < MAX_NUMBER_OF_CIDS; x++)
                {
                   vgGpDtAddConnIdFromActiveCid( x, param1, &validData);
                }
             }
             else if (param2 >= MAX_NUMBER_OF_CIDS)
             {
                result = VG_CME_INVALID_INPUT_VALUE;
             }
             else
             {  // see what connId we have then read the rest
               vgGpDtAddConnIdFromActiveCid( param2, param1, &validData);
               while ((param3 != ULONG_MAX) && (result == RESULT_CODE_OK))
               {
                   if ((getExtendedParameter(  commandBuffer_p,
                                         &param3,
                                         ULONG_MAX) == FALSE))
                   {
                       result = VG_CME_INVALID_INPUT_VALUE;
                   }
                   if (param3 <  ULONG_MAX)
                   {
                      if (param3 >=MAX_NUMBER_OF_CIDS)
                      {
                         result = VG_CME_INVALID_INPUT_VALUE;
                      }
                      else
                      {
                         vgGpDtAddConnIdFromActiveCid( param3, param1, &validData);
                      }
                   }
               }
             }
             if ((validData) && (result == RESULT_CODE_OK))  // whole input string needs to be ok and there needs to be some valid data to send
             {
                result = vgChManContinueAction(entity, SIG_APEX_ABPD_WRITE_APN_DATA_TYPE_REQ);
             }
          }
        }
       break;
    }

    case EXTENDED_QUERY:    /* AT*MNBIOTDT?  */
    {
        result = vgChManContinueAction (entity, SIG_APEX_ABPD_READ_APN_DATA_TYPE_REQ);
      break;
    }
    case EXTENDED_ACTION:   /* AT*MNBIOTDT   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);
}
/*--------------------------------------------------------------------------
*
* Function:    vgGpMNBIOTRAI
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Allows the user to set
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGpMNBIOTRAI (CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();

  ResultCode_t          result = RESULT_CODE_OK;
  Int32                 param   = 0;

   switch (operation)
  {
    case EXTENDED_RANGE:    /* AT*MNBIOTRAI=? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"*MNBIOTRAI: (%d-%d)", ABPD_RAI_NO_INFO, ABPD_RAI_UL_AND_DL);
      vgPutNewLine (entity);
      break;
    }

    case EXTENDED_ASSIGN:   /* AT*MNBIOTRAI=  */
    {
       if( (getExtendedParameter(  commandBuffer_p,
                                            &param,
                                            ULONG_MAX) == FALSE) ||
                    (param > ABPD_RAI_UL_AND_DL) )// get data type
       {
          result = VG_CME_INVALID_INPUT_VALUE;
       }
       else
       {
          if( result == RESULT_CODE_OK)
          {
             gprsGenericContext_p->vgReqNbiotRelAssistData = (AbpdRelAssistInformation)param;
             result = vgChManContinueAction(entity,SIG_APEX_ABPD_WRITE_REL_ASSIST_REQ);
          }
       }
       break;
    }

    case EXTENDED_QUERY:    /* AT*MNBIOTRAI?  */
    {
        result = vgChManContinueAction (entity, SIG_APEX_ABPD_READ_REL_ASSIST_REQ);
      break;
    }
    case EXTENDED_ACTION:   /* AT*MNBIOTRAI   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);

}
/*--------------------------------------------------------------------------
*
* Function:    vgGpCGAPNRC
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Allows the user to get APN rate control parameters
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGpCGAPNRC(CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  ResultCode_t          result = RESULT_CODE_OK;
  Int32                 param   = 0;
  Int8                  cidCounter;
  VgPsdStatusInfo      *psdStatusInfo_p;
  Boolean               first = TRUE;
  Int8                  cidNum;

   switch (operation)
  {
    case EXTENDED_RANGE:    /* AT+CGAPNRC=? */
    {

      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"+CGAPNRC: (");
      for (cidCounter = 0; cidCounter < MAX_NUMBER_OF_CIDS; cidCounter++)
      {
        psdStatusInfo_p = gprsGenericContext_p->cidUserData[cidCounter];
        if ((psdStatusInfo_p != PNULL)&&(psdStatusInfo_p->isActive))
         {
           if (first == TRUE)
           {
              first = FALSE;
           }
           else
           {
              vgPrintf(entity, (const Char*)",");
           }
           vgPrintf (entity, (const Char*)"%d", cidCounter);
         }
      }
      vgPrintf(entity,(const Char*)")");
      vgPutNewLine (entity);
      break;
    }

    case EXTENDED_ASSIGN:   /* AT+CGAPNRC=  */
    {
       if ((getExtendedParameter(  commandBuffer_p,
            &param, ULONG_MAX) == FALSE) || ((param >= MAX_NUMBER_OF_CIDS) && (param < ULONG_MAX )))
       {
        result = VG_CME_INVALID_INPUT_VALUE;
       }
       else if (param == ULONG_MAX)
       {
          // display all valid CID info
          for (cidNum = 0; cidNum < MAX_NUMBER_OF_CIDS; cidNum++)
          {
             vgGpDisplayApnExceptionReportsInfo(cidNum, entity);
          }
       }
       else
       {
          cidNum = param;
          vgGpDisplayApnExceptionReportsInfo(cidNum, entity);
        }
        vgPutNewLine (entity);
       break;

    }

    case EXTENDED_QUERY:    /* AT+CGAPNRC?  */
    {
      result = RESULT_CODE_ERROR;
      break;
    }

    case EXTENDED_ACTION:   /* AT+CGAPNRC   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);

}

/*--------------------------------------------------------------------------
*
* Function:     vgGpDisplayApnExceptionReportsInfo
*
* Parameters:   cidNum for next print

* Returns:      nothing
*
* Description:  Display the Apn rate control info associated with the passed CID
*
*-------------------------------------------------------------------------*/

void vgGpDisplayApnExceptionReportsInfo( Int8 cidNum, const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo       *psdStatusInfo_p;

  psdStatusInfo_p = gprsGenericContext_p->cidUserData[cidNum];    // find data related to the input CID
  if ((psdStatusInfo_p != PNULL) && (psdStatusInfo_p->isActive))
  {
     vgPutNewLine (entity);
     vgPrintf (entity, (const Char*)"+CGAPNRC:%d",cidNum);
     if (psdStatusInfo_p->apnUplinkRateControlInfo.apnRateControlPresent)
     {
        vgPrintf(entity, (const Char*)",%d,%d,%d",
          psdStatusInfo_p->apnUplinkRateControlInfo.apnRateControlAdditionalExceptionReportsAllowed,
          psdStatusInfo_p->apnUplinkRateControlInfo.apnRateControluplinkTimeUnit,
          psdStatusInfo_p->apnUplinkRateControlInfo.apnRateControlMaxUplinkRate);
     }
  }
}

/*--------------------------------------------------------------------------
*
* Function:    vgGpCSODCP
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Send originating data via the control plane
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGpCSODCP(CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  ResultCode_t          result = RESULT_CODE_OK;
  Int32                 param = 0;
  Int32                 cid;
  Int32                 dataLength;
  Int16                 tmpStringLen;
  Char                  *data_ptr;
  Char                  tmpDataStr[VG_MAX_CPDATA_LENGTH*2+1];
  Char                  data[VG_MAX_CPDATA_LENGTH+1];
  VgPsdStatusInfo       *cmd_p = PNULL;
  Boolean               validData = FALSE;
  memset((Char*)(tmpDataStr), 0 , sizeof(Char)*(VG_MAX_CPDATA_LENGTH*2+1));
  memset((Char*)(data), 0 , sizeof(Char)*(VG_MAX_CPDATA_LENGTH+1));

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT+CSODCP=? */
    {
      vgPutNewLine (entity);
#if (MAX_NUMBER_OF_CIDS == 2)
      vgPrintf (entity, (const Char*)"%C: (1),(%d),(%d-%d),(%d-%d)",
                        VG_MAX_CPDATA_LENGTH, ABPD_RAI_NO_INFO, ABPD_RAI_UL_AND_DL,
                        ABPD_APN_DATA_TYPE_NORMAL, ABPD_APN_DATA_TYPE_EXCEPTIONAL);
#else
      vgPrintf (entity, (const Char*)"%C: (1-%d),(%d),(%d-%d),(%d-%d)",
                        (MAX_NUMBER_OF_CIDS - 1),VG_MAX_CPDATA_LENGTH, ABPD_RAI_NO_INFO, ABPD_RAI_UL_AND_DL,
                        ABPD_APN_DATA_TYPE_NORMAL, ABPD_APN_DATA_TYPE_EXCEPTIONAL);
#endif
      vgPutNewLine (entity);
      break;
    }

    case EXTENDED_ASSIGN:   /* AT+CSODCP=  */
    {
      /* get the CID */
      if (getExtendedParameter (commandBuffer_p, &cid, ULONG_MAX))
      {
        /* if the cid is invalid or does not exist; tmpVar = default
           value of ULONG_MAX then error. */
        if ((cid >= vgGpGetMinCidValue(entity)) &&(cid < MAX_NUMBER_OF_CIDS))
        {
          if (vgOpManCidActive((Int8)cid))
          {
            cmd_p = gprsGenericContext_p->cidUserData [cid];
            gprsGenericContext_p->vgCSODCPData.connId = cmd_p->psdBearerInfo.connId;
          }
          else
          {
            /* The cid has not been activated */
            result = VG_CME_INVALID_CID_VALUE;
          }
        }
        else
        {
          result = VG_CME_INVALID_CID_VALUE;
        }
      }
      else
      {
        /* must supply at least one cid */
        result = RESULT_CODE_ERROR;
      }

      /* now get the cpdata_length */
      if (result == RESULT_CODE_OK)
      {
        if (getExtendedParameter (commandBuffer_p, &dataLength, ULONG_MAX))
        {
            if((dataLength == ULONG_MAX) || (dataLength > VG_MAX_CPDATA_LENGTH))
            {
              result = VG_CME_INVALID_INPUT_VALUE;
            }
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
      }

      /* now get the CPDATA */
      if (result == RESULT_CODE_OK)
      {
        if (getExtendedString (commandBuffer_p,
                               tmpDataStr,
                               VG_MAX_CPDATA_LENGTH * 2,
                               &tmpStringLen))
        {
          /* string length can be multiple of 2 */
          if (!(tmpStringLen == (Int16)dataLength * 2))
          {
            result = VG_CME_INVALID_INPUT_VALUE;
          }
          else if(tmpStringLen != 0)
          {
            /* Decode the hex string - convert to hex number */
            (void)vgMapTEToHex (data,
                                (Int16)dataLength,
                                tmpDataStr,
                                tmpStringLen,
                                VG_AT_CSCS_HEX);
          }
        }
        else /* error reading supplied parameter */
        {
          result = RESULT_CODE_ERROR;
        }
      }

      /* now get the RAI */
      if (result == RESULT_CODE_OK)
      {
        if((getExtendedParameter(commandBuffer_p,
                                 &param,
                                 ABPD_RAI_NO_INFO) == TRUE) &&
                     (param <= ABPD_RAI_UL_AND_DL) )
        {
          gprsGenericContext_p->vgReqNbiotRelAssistData = (AbpdRelAssistInformation)param;
        }
        else
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }

      /* now get the datatype. */
      if (result == RESULT_CODE_OK)
      {
       if( (getExtendedParameter(  commandBuffer_p,
                                    &param,
                                    ABPD_APN_DATA_TYPE_NORMAL) == TRUE) &&
                    (param <= ABPD_APN_DATA_TYPE_EXCEPTIONAL) )
       {
         vgGpDtAddConnIdFromActiveCid(cid, param, &validData);
       }
       else
       {
         result = VG_CME_INVALID_INPUT_VALUE;
       }
      }

      if ((result == RESULT_CODE_OK) && (validData == TRUE))
      {
        if(tmpStringLen != 0)
        {

          /* Allocate L4MM memory */
          if(TRUE == L4mmAlloc(L4MM_UL_POOL, dataLength, &(gprsGenericContext_p->vgCSODCPData.userData)))
          {
            gprsGenericContext_p->vgCSODCPData.userDataLength = dataLength;
            memcpy(gprsGenericContext_p->vgCSODCPData.userData, data, dataLength);

            result = vgChManContinueAction (entity, SIG_APEX_ABPD_TRANSMIT_DATA_IND);

            if(result == RESULT_CODE_PROCEEDING)
            {
              /* Return OK */
              result = RESULT_CODE_OK;
            }
            else
            {
              L4mmFree(&(gprsGenericContext_p->vgCSODCPData.userData),dataLength);
            }
          }
          else
          {
            result = VG_CME_INSUFFIC_RESOURCES;
          }
        }
      }
      break;
    }
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);

}

/***************************************************************************
 * Processes
 ***************************************************************************/


/* END OF FILE */



