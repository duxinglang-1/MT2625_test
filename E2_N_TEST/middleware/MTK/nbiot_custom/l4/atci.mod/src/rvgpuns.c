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
 * GKI-APEX unsolicited signal converter for the GPRS Sub-System.
 **************************************************************************/

#define MODULE_NAME "RVGPUNS"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvmmut.h>
#include <rvgput.h>
#include <rvgpuns.h>
#include <rvpdsigo.h>
#include <rvpdsigi.h>
#include <rvcrhand.h>
#include <rvchman.h>
#include <rvoman.h>
#include <rvccut.h>
#include <rvcimxut.h>

#if defined (UPGRADE_SHARE_MEMORY)
#include <r2_hal.h>
#include <t1muxshmdrv.h>
#endif /* UPGRADE_SHARE_MEMORY */
/* For new SHMCL */
#if defined(UPGRADE_SHMCL_SOLUTION)
#include <muxconn_at.h>
#endif
/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  ApexMmNetworkStateInd   apexMmNetworkStateInd;
  ApexAbpdCounterInd      apexAbpdCounterInd;
};

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

typedef struct ErepPromptMapTag
{
  const Char *string;
}
ErepPromptMap;

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

static const ErepPromptMap erepPromptMap[] =
{
  { (const Char *)"+CGEV: NW DETACH"   },
  { (const Char *)"+CGEV: ME DETACH"   },
  { (const Char *)"+CGEV: NW CLASS"    },  /* Not used */
  { (const Char *)"+CGEV: ME CLASS"    },  /* Not used */
  { (const Char *)"+CGEV: NW PDN ACT"  },
  { (const Char *)"+CGEV: ME PDN ACT"  },
  { (const Char *)"+CGEV: NW ACT"      },
  { (const Char *)"+CGEV: ME ACT"      }, 
  { (const Char *)"+CGEV: NW PDN DEACT"},
  { (const Char *)"+CGEV: ME PDN DEACT"},
  { (const Char *)"+CGEV: NW DEACT"    },
  { (const Char *)"+CGEV: ME DEACT"    },
  { (const Char *)"+CGEV: NW MODIFY"   },
  { (const Char *)"+CGEV: ME MODIFY"   },
  { (const Char *)"+CGEV: REJECT"      },
  { (const Char *)"+CGEV: NW REACT"    }
};
#define ACT_NBIOT                    9 
#define INT8_BIN_STRING_LENGTH       9


/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static void vgSendUnsDetachData (const VgEREPPromptRef promptRef);
static void vgSendUnsMGCOUNT    (const Int8 cid, uint64_t ulDataVolumeCounters, uint64_t dlDataVolumeCounters, PdnType pdntype);

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
*
* Function:    vgSendUnsDetachData
*
* Parameters:  promptRef - prompt map index
*
* Returns:     nothing
*
* Description: Constructs a sentence making up the unsolicited event
*              report for mobile and network initiated GPRS detaches
*
*-------------------------------------------------------------------------*/

static void vgSendUnsDetachData (const VgEREPPromptRef promptRef)
{
  VgmuxChannelNumber profileEntity = 0;

  for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
  {
    if (isEntityActive(profileEntity) && 
        (getProfileValue (profileEntity, PROF_CGEREP) == EREP_MODE_1))
    {
      vgPutNewLine (profileEntity);
      vgPuts (profileEntity, erepPromptMap[promptRef].string);
      vgSetCirmDataIndIsUrc(profileEntity, TRUE);
      vgFlushBuffer (profileEntity);
    }
  }
}

/*--------------------------------------------------------------------------
*
* Function:    vgSendUnsMGCOUNT
*
* Parameters:  cid           - CID associated with the counters
*              counterData   - counter information
*
* Returns:     nothing
*
* Description: This function sends an unsolicited *MGCOUNT message.
*
*-------------------------------------------------------------------------*/

static void vgSendUnsMGCOUNT (const Int8 cid, uint64_t ulDataVolumeCounters, uint64_t dlDataVolumeCounters, PdnType pdntype)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgmuxChannelNumber    channel;
  uint32_t              ulRollCounter;
  uint32_t              ulDataCounters;
  uint32_t              dlRollCounter;
  uint32_t              dlDataCounters;

  channel = gprsGenericContext_p->vgMGCOUNTData.vgReportEntity;
  ulRollCounter = ulDataVolumeCounters >> 32;
  ulDataCounters = ulDataVolumeCounters & 0xFFFFFFFF;
  dlRollCounter = dlDataVolumeCounters >> 32;
  dlDataCounters = dlDataVolumeCounters & 0xFFFFFFFF;

  vgPutNewLine (channel);

  vgPrintf (channel,
           (const Char *)"*MGCOUNT: %d,%u,%u,%u,%u",
            /* job132261: print correct CID value */
            cid,
            ulRollCounter,
            ulDataCounters,
            dlRollCounter,
            dlDataCounters);

  if(pdntype == PDN_TYPE_NONIP)
  {
    vgPrintf (channel, (const Char*)",\"Non-IP\"");
  }
  else
  {
    vgPrintf (channel, (const Char*)",\"IP\"");
  }
  
  vgPutNewLine (channel);
  vgSetCirmDataIndIsUrc(channel, TRUE);
  vgFlushBuffer (channel);

}

/*--------------------------------------------------------------------------
*
* Function:    vgSendUnsPrimActDeactData
*
* Parameters:  isUEInitiated - if the UE initiated the PDP context
*              cid           - context id
*              isActive      - if the PDP context is active
*              causePresent  - if there was a cause received from SM task
*              cause         - the cause (if any) from SM task
*
* Returns:     nothing
*
* Description: Constructs a sentence making up the unsolicited event
*              report for mobile and network initiated context
*              activations and deactivations for default bearer
*
*-------------------------------------------------------------------------*/

void vgSendUnsPrimActDeactData (const Boolean   isUEInitiated,
                               const Int8      cid,
                               const Boolean   isActive,
                               const Boolean   causePresent,
                               const GsmCause  cause)
{
  VgmuxChannelNumber     profileEntity         = 0;
  VgmuxChannelNumber     cidEntity             = VGMUX_CHANNEL_INVALID;
  VgEREPPromptRef        erepPromptRef;
  GprsGenericContext_t   *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo        *activePsdContext_p   = PNULL;
  VgEREPMePrimBActReason erepActReason         = NUM_OF_EREP_PRIM_B_ACT_REASON;

  if (cid != CID_NUMBER_UNKNOWN)
  {
    cidEntity = vgFindEntityLinkedToCid(cid);
  }
    
  if (TRUE == isActive)
  {
     erepPromptRef = (isUEInitiated)?(EREP_PROMPT_ME_PRIM_B_ACT):(EREP_PROMPT_NW_PRIM_B_ACT);
  }
  else
  {
     erepPromptRef = (isUEInitiated)?(EREP_PROMPT_ME_PRIM_B_DEACT):(EREP_PROMPT_NW_PRIM_B_DEACT); 
  }

  for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
  {
    if (isEntityActive(profileEntity) && 
        (getProfileValue (profileEntity, PROF_CGEREP) == EREP_MODE_1))
    {
      vgPutNewLine (profileEntity);
      vgPrintf (profileEntity, (const Char *)"%s %d",
                erepPromptMap[erepPromptRef].string,
                cid);

      /* To display the <reason> field we need to check if this was an
       * ME activated context and that the PDN address type we got from the 
       * network was not the same as the one requested (if we requested IPV4V6
       * addressing
       */
      if (erepPromptRef == EREP_PROMPT_ME_PRIM_B_ACT)
      {
        activePsdContext_p = gprsGenericContext_p->cidUserData[cid];

        if ((activePsdContext_p->psdBearerInfo.reqPdnAddress.pdnType == PDN_TYPE_IPV4V6) &&
            (activePsdContext_p->psdBearerInfo.reqPdnAddress.pdnType !=
                                   activePsdContext_p->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress.pdnType))
        {
          /* We requested PDN type IPV4V6 - but we got something else - so display whatever
           * cause value we got from ESM - as we should have got something sensible
           */

          if (causePresent)
          {
            switch (cause)
            {
              /* Only ESM causes.
               */
              case ESM_CAUSE_PDN_TYPE_IPV4_ONLY_ALLOWED:
                erepActReason = EREP_REASON_IPV4_ONLY_ALLOWED;
                break;
              case ESM_CAUSE_PDN_TYPE_IPV6_ONLY_ALLOWED:
                erepActReason = EREP_REASON_IPV6_ONLY_ALLOWED;
                break;
              case ESM_CAUSE_SINGLE_ADDRESS_BEARERS_ONLY_ALLOWED:
                erepActReason = EREP_REASON_SINGLE_ADDR_BEARERS_ONLY_ALLOWED;
                break;
              default:
                /* Do nothing - we don't support any other types here */
                break;
            }
          }
          /* Display the <reason> field if there is one */
          if (erepActReason != NUM_OF_EREP_PRIM_B_ACT_REASON)
          {
            vgPrintf (profileEntity, (const Char *) ",%d",
                      (Int8)erepActReason);
          }
        }
      }
      vgPutNewLine (profileEntity);

      /* Check if we need to force output of this URC.
       * Only force output if this channel is the same as the one
       * being activated/deactivated
       */
      if ((cidEntity != VGMUX_CHANNEL_INVALID) && (cidEntity == profileEntity))
      {
        vgSetCirmDataIndForceTransmit(profileEntity);
      }

      vgSetCirmDataIndIsUrc(profileEntity, TRUE);      
      vgFlushBuffer (profileEntity);
    }
  }
}

#if defined (FEA_DEDICATED_BEARER)
/*--------------------------------------------------------------------------
*
* Function:    vgSendUnsSecActDeactData
*
* Parameters:  isUEInitiated
*              cid           - context id
*              eventType
*              isActive    
*
* Returns:     nothing
*
* Description: Constructs a sentence making up the unsolicited event
*              report for mobile and network initiated context
*              activations and deactivations for dedicated bearer
*
*-------------------------------------------------------------------------*/
void vgSendUnsSecActDeactData (const Boolean isUEInitiated,
                             const Int8 cid,
                             const VgEREPEventType eventType, 
                             const Boolean isActive)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo       *psdStatusInfo        = gprsGenericContext_p->cidUserData[cid];
  VgmuxChannelNumber    profileEntity         = 0;
  VgEREPPromptRef       erepPromptRef;  

  if (TRUE == isActive)
  {
     erepPromptRef = (isUEInitiated)?(EREP_PROMPT_ME_SEC_B_ACT):(EREP_PROMPT_NW_SEC_B_ACT);
  }
  else
  {
     erepPromptRef = (isUEInitiated)?(EREP_PROMPT_ME_SEC_B_DEACT):(EREP_PROMPT_NW_SEC_B_DEACT); 
  }

  FatalCheck(TRUE == psdStatusInfo->psdBearerInfo.secondaryContext, cid, 0, 0);
  
  for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
  {
    if (isEntityActive(profileEntity) && 
        (getProfileValue (profileEntity, PROF_CGEREP) == EREP_MODE_1))
    {
      vgPutNewLine (profileEntity);
      vgPrintf (profileEntity, (const Char *)"%s %d,%d,%d",
                erepPromptMap[erepPromptRef].string,
                psdStatusInfo->psdBearerInfo.primaryCid,
                cid,
                (Int16)eventType);
      vgPutNewLine (profileEntity);
      vgSetCirmDataIndIsUrc(profileEntity, TRUE);      
      vgFlushBuffer (profileEntity);
    }
  }
}
#endif /* FEA_DEDICATED_BEARER */

/*--------------------------------------------------------------------------
*
* Function:    vgSendUnsContextRejectReactData
*
* Parameters:  erepPromptRef
*              pdnAddress
*              entity   
*              cid
*
* Returns:     nothing
*
* Description: Constructs a sentence making up the unsolicited event
*              report for reacted or automatically rejected pdp context
*
*-------------------------------------------------------------------------*/
void vgSendUnsContextRejectReactData (const VgEREPPromptRef erepPromptRef,
                                  const PdnAddress pdnAddress,
                                  const VgmuxChannelNumber entity,
                                  const Int8 cid)
{
  const SupportedPDNTypesMap    *supportedPDNTypesMap = getSupportedPDNTypesMap();    
  VgmuxChannelNumber            profileEntity         = 0;
  Int8                          index;  

  for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
  {
    if (isEntityActive(profileEntity) && 
        (getProfileValue (profileEntity, PROF_CGEREP) == EREP_MODE_1))
    {
      vgPutNewLine (profileEntity);
      vgPrintf (profileEntity, (const Char *)"%s ",
                erepPromptMap[erepPromptRef].string);

      vgPutc(profileEntity, '\"');

      if (vgPDNTypeToIndx (pdnAddress.pdnType, &index) == TRUE)
      {
        vgPrintf (profileEntity,
                  (const Char *)"%s",
                  supportedPDNTypesMap[index].str);
      }

      vgPrintf (profileEntity, (const Char *)"\",\"");

      if (pdnAddress.addressPresent == TRUE)
      {
        TextualPdnAddress convertedPdnAddress;
        vgPDNAddrIntToDisplayStr (pdnAddress.pdnType, pdnAddress, &convertedPdnAddress, entity);
        vgPrintf (profileEntity,
                  (const Char *)"%.*s",
                  convertedPdnAddress.length,
                  convertedPdnAddress.address);
      }

      vgPrintf (profileEntity, (const Char *)"\"");

      if ((erepPromptRef == EREP_PROMPT_REACT) && (cid != CID_NUMBER_UNKNOWN))
      {
        vgPrintf (profileEntity, (const Char *)",%d", cid);
      }
      
      vgPutNewLine (profileEntity);
      vgSetCirmDataIndIsUrc(profileEntity, TRUE);      
      vgFlushBuffer (profileEntity);
    }
  }
}

#if defined (FEA_QOS_TFT)
/*--------------------------------------------------------------------------
*
* Function:    vgSendUnsContextModifyData
*
* Parameters:  isUEInitiated
*              cid           - context id
*              modifyReason
*              eventType    
*
* Returns:     nothing
*
* Description: Constructs a sentence making up the unsolicited event
*              report for mobile and network initiated context
*              modifications
*
*-------------------------------------------------------------------------*/
void vgSendUnsContextModifyData (const Boolean isUEInitiated,
                             const Int8 cid,
                             const VgEREPModifyReason modifyReason,
                             const VgEREPEventType eventType)
{
  VgmuxChannelNumber    profileEntity         = 0;
  VgEREPPromptRef       erepPromptRef;
  
  erepPromptRef = (isUEInitiated)?(EREP_PROMPT_ME_MODIFY):(EREP_PROMPT_NW_MODIFY);

  for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
  {
    if (isEntityActive(profileEntity) && 
        (getProfileValue (profileEntity, PROF_CGEREP) == EREP_MODE_1))
    {
      vgPutNewLine (profileEntity);
      vgPrintf (profileEntity, (const Char *)"%s %d,%d,%d",
                erepPromptMap[erepPromptRef].string,
                cid,
                (Int16)modifyReason,
                (Int16)eventType);
      vgPutNewLine (profileEntity);
      vgSetCirmDataIndIsUrc(profileEntity, TRUE);      
      vgFlushBuffer (profileEntity);
    }
  }
}
#endif /* FEA_QOS_TFT */

/***************************************************************************
 * Global Functions
 ***************************************************************************/

#if defined (UPGRADE_SHARE_MEMORY)  || defined(UPGRADE_SHMCL_SOLUTION)
/*--------------------------------------------------------------------------
*
* Function:    writeCgregDataToShareMemory
*
* Parameters:  none
*              
*
* Returns:     nothing
*
* Description: place stat, lac and cellId information about GPRS connection
*              to share memory and reocord it locally.
*-------------------------------------------------------------------------*/
void writeCgregDataToShareMemory(void)
{
  AtDataType           atDataType;
  Boolean              showRegInfo           = FALSE;
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();  
  VgCGREGData          *vgCGREGData          = &(gprsGenericContext_p->vgCGREGData);  
  MobilityContext_t    *mobilityContext_p    = ptrToMobilityContext ();
  Int8                 accessTechnology      = ACT_NBIOT;

  if ((vgCGREGData->state == TRUE) &&
      ((vgCGREGData->lac != 0) || 
       (vgCGREGData->cellId != 0)))
  {
    showRegInfo = TRUE;
  }

  mobilityContext_p->cgregData.state              = (Int8)vgCGREGData->regStatus;
  if(showRegInfo == TRUE)
  {
    mobilityContext_p->cgregData.accessTechnology = accessTechnology;
    mobilityContext_p->cgregData.cellId           = vgCGREGData->cellId;
    mobilityContext_p->cgregData.lac              = vgCGREGData->lac;
  }
  else
  {
    mobilityContext_p->cgregData.accessTechnology = 0;
    mobilityContext_p->cgregData.cellId           = 0;
    mobilityContext_p->cgregData.lac              = 0;   
  }
  
  atDataType.cmdType = CGREG_CMD;
  atDataType.atData.cgregData.accessTechnology    = mobilityContext_p->cgregData.accessTechnology;
  atDataType.atData.cgregData.cellId              = mobilityContext_p->cgregData.cellId;
  atDataType.atData.cgregData.lac                 = mobilityContext_p->cgregData.lac;
  atDataType.atData.cgregData.state               = mobilityContext_p->cgregData.state;  
  if(DriWriteUnsolictedInd(&atDataType) == TRUE)
  {
    mobilityContext_p->cgregUpdated = FALSE;
  }
  else
  {
    mobilityContext_p->cgregUpdated = TRUE;
  }
}
#endif /* UPGRADE_SHARE_MEMORY || UPGRADE_SHMCL_SOLUTION */


/*--------------------------------------------------------------------------
*
* Function:    vgSendCGREGData
*
* Parameters:  entity  - mux channel number
*              atQuery - AT command / unsolicited indication
*
* Returns:     nothing
*
* Description: Send stat, lac and cellId information about GPRS connection
*-------------------------------------------------------------------------*/

void vgSendCGREGData (const VgmuxChannelNumber entity,
                      const Boolean atQuery ,
                      VgCGREGMode cgregMode )
{
  GprsGenericContext_t *gprsGenericContext_p    = ptrToGprsGenericContext ();
  VgCGREGData          *vgCGREGData             = &(gprsGenericContext_p->vgCGREGData);
  Int8                 accessTechnology         = ACT_NBIOT;

  vgPutNewLine (entity);

  if (atQuery == TRUE)
  {
    vgPrintf (entity,
              (const Char *)"+CGREG: %d,%d",
              (Int16)cgregMode,
              vgCGREGData->regStatus);
  }
  else
  {
    vgPrintf (entity,
              (const Char *)"+CGREG: %d",
              vgCGREGData->regStatus);
    vgSetCirmDataIndIsUrc(entity, TRUE);      
  }

  if ((vgCGREGData->state == TRUE) &&
      ((vgCGREGData->lac    != 0) ||
       (vgCGREGData->rac    != 0) ||
       (vgCGREGData->cellId != 0)) &&
      (cgregMode == VG_CGREG_ENABLED_WITH_LOCATION_INFO))

  {
    vgPrintf (entity,
              (const Char *)",\"%04X\",\"%08X\",%d",
              (Int32)(vgCGREGData->lac),
              vgCGREGData->cellId,
              accessTechnology);
    vgPrintf (entity,
              (const Char *)",\"%02X\"",
              vgCGREGData->rac);
  }

  vgPutNewLine (entity);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSendMDPDNPData
*
* Parameters:  entity  - mux channel number
*              atQuery - AT command / unsolicited indication
*
* Returns:     nothing
*
* Description: Send stat, lac and cellId information about GPRS connection
*-------------------------------------------------------------------------*/

void vgSendMDPDNPData (const VgmuxChannelNumber entity,
                      TextualAccessPointName *defaultApn,
                      PdnType defaultPdnType)
{
  Boolean            enumFound;
  Int8                 arrIndx;
  Int8                 apnChar;  
  const SupportedPDNTypesMap    *supportedPDNTypesMap = getSupportedPDNTypesMap();    

  vgPutNewLine(entity);

  vgPrintf (entity, (const Char*)"*MDPDNP: \"");

  /* Display default APN */
  for (apnChar = 0;
        apnChar < defaultApn->length;
         apnChar++)
  {
    vgPutc (entity, defaultApn->name[apnChar]);
  }

  vgPrintf (entity, (const Char*)"\",\"");

  /* Display default PDN type */
  enumFound = vgPDNTypeToIndx (defaultPdnType, &arrIndx);
  
  if (enumFound == TRUE)
  {
    vgPrintf (entity,
              (const Char *)"%s",
              supportedPDNTypesMap[arrIndx].str);
  }
  vgPrintf (entity, (const Char*)"\"");
  
  vgSetCirmDataIndIsUrc(entity, TRUE);      
  
  vgPutNewLine (entity);
}

/*--------------------------------------------------------------------------
*
* Function:     vgApexMmNetworkStateInd
*
* Parameters:   signalBuffer - the signal sent from ABGP
*               entity       - mux channel number
*
* Returns:      nothing
*
* Description:  This routine processes the network ind signal that is sent
*               to all FG layers after the ME has changed states.  If the
*               service state is either ABMM_GPRS or COMBINED then the
*               ME must be GPRS attached.
*-------------------------------------------------------------------------*/

void vgApexMmNetworkStateInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  ApexMmNetworkStateInd     *sig_p                  = &signalBuffer->sig->apexMmNetworkStateInd;
  GprsGenericContext_t      *gprsGenericContext_p   = ptrToGprsGenericContext ();
  VgCGREGData               *vgCGREGData            = &(gprsGenericContext_p->vgCGREGData);
  VgCGREGMode               cgregMode;
  VgCEREGData               *vgCEREGData            = &(gprsGenericContext_p->vgCEREGData); 
  VgCEREGMode               ceregMode;  
  VgMDPDNPData              *vgMdpdnpData         = &(gprsGenericContext_p->vgMDPDNPData);
  VgMDPDNPMode             mdpdnpMode;
  Boolean                   regStatusChange         = FALSE;
  Boolean                   locationInfoChange      = FALSE;
  Boolean                   psmInfoChange           = FALSE;
  Boolean                   rejectCauseChange       = FALSE;
  VgmuxChannelNumber        profileEntity;
  VgMnLocation              newRegStatus;
  PARAMETER_NOT_USED(entity);

  gprsGenericContext_p->gprsServiceState.abmmCurrentService = sig_p->serviceType;
  if(sig_p->defaultApnPresent == TRUE)
  {
    vgMdpdnpData->defaultApnFromNw = sig_p->defaultApnFromNw;
    vgMdpdnpData->defaultPdnTypeFromNw = sig_p->defaultPdnTypeFromNw;
  }
  
  /* Update detach status */
  if (gprsGenericContext_p->gprsServiceState.abmmCurrentService == GPRS_SERVICE)
  {
    gprsGenericContext_p->gprsServiceState.gprsAttached = TRUE;
  }
  else
  {
    /*
     * If we were attached and we have lost the network then keep the setting that we are
     * attached until a detach is triggered.
     */
    if (gprsGenericContext_p->gprsServiceState.gprsAttached == TRUE)
    {
      if ((gprsGenericContext_p->gprsServiceState.abmmCurrentService == NO_SERVICES_AVAILABLE) &&
          (sig_p->abmmDetachTrig == GPRS_NO_DETACH_TRIG))
      {
        /*
         * We are out of service - but we need to check if we should say we are attached or not depending on the
         * serviceStatus
         */
        switch (sig_p->serviceStatus)
        {
          case PLMN_NORMAL:    
          case PLMN_EMERGENCY_ONLY:    
          case PLMN_NO_SERVICE:    
          case PLMN_ACCESS_DIFFICULTY:
            /*
             * Leave us as attached.
             */
            break;
          default:
            /*
             * Access forbidden or not allowed for some reason - so set as detached.
             */
            gprsGenericContext_p->gprsServiceState.gprsAttached = FALSE;   
            break;
        }

      }  
      else
      {
        gprsGenericContext_p->gprsServiceState.gprsAttached = FALSE;   
      }          
    }
  }
  gprsGenericContext_p->gprsServiceState.valid = TRUE;

  /* If there has been an ME or network
   * initiated detach, send unsolicited report */
  switch (sig_p->abmmDetachTrig)
  {
    case GPRS_NO_DETACH_TRIG:
    { /* Ignore. Nothing to report */
      break;
    }
    case GPRS_ME_DETACH_TRIG:
    {
      /* Mobile initiated detach */
      vgSendUnsDetachData (EREP_PROMPT_ME_DETACH);
      break;
    }
    case GPRS_NW_DETACH_TRIG:
    {
      /* Network initiated detach */
      vgSendUnsDetachData (EREP_PROMPT_NW_DETACH);
      break;
    }
    default:
    {
      /* Invalid value of inSig->abmmDetachTrig */
      FatalParam(entity, sig_p->abmmDetachTrig, 0);
      break;
    }
  }

  if (sig_p->serviceType == GPRS_SERVICE)
  {
      vgCGREGData->state = TRUE;
      vgCEREGData->state = TRUE;
  }
  else
  {
      vgCGREGData->state = FALSE;
      vgCEREGData->state = FALSE; 
  }
  

  /* Examine NetworkStateInd signal for changes in
   * registration status and/or location info */
  newRegStatus = (VgMnLocation) sig_p->gprsRegStatus;
  /* For LTE we modify the status depending on SMS only registration information */
#if 0
  if (sig_p->additionalUpdateResult == AUR_SMS_ONLY)
#else
  /* TODO: Temporary hack for allow AP testing - need to confirm what we do
   * for SMS OK and also PDN OK
   */
  if ((sig_p->additionalUpdateResult == AUR_SMS_ONLY) && !(sig_p->serviceStatus == PLMN_NORMAL))
#endif
  {
      switch (newRegStatus)
      {
          case VGMNL_REGISTRATED_HOME:
              newRegStatus = VGMNL_REG_HOME_SMS_ONLY;
              break;
          case VGMNL_REGISTRATED_ROAMING:
              newRegStatus = VGMNL_REG_ROAMING_SMS_ONLY;
              break;
          default:
              /* Don't change anything */
              break;
      }
  }

  if (newRegStatus != vgCGREGData->regStatus)
  {
    regStatusChange = TRUE;
    vgCGREGData->regStatus = newRegStatus;
    vgCEREGData->regStatus = newRegStatus;   
  }

  if ((sig_p->lai.lac != vgCGREGData->lac) ||
      (sig_p->cellId  != vgCGREGData->cellId) ||
      (sig_p->gprsCellInfo.rac != vgCGREGData->rac) ||
      (sig_p->plmn.accessTechnology != vgCGREGData->accessTechnology))
  {
    locationInfoChange = TRUE;
    vgCGREGData->lac    = sig_p->lai.lac;
    vgCGREGData->cellId = sig_p->cellId;
    vgCGREGData->accessTechnology = sig_p->plmn.accessTechnology;
    vgCGREGData->rac    = sig_p->gprsCellInfo.rac;
  }

  if ((sig_p->lai.lac != vgCEREGData->tac) ||
      (sig_p->cellId  != vgCEREGData->cellId) ||
      (sig_p->plmn.accessTechnology != vgCEREGData->accessTechnology) ||
      (sig_p->gprsCellInfo.rac != vgCEREGData->rac))
  {
    locationInfoChange = TRUE;
    vgCEREGData->tac    = sig_p->lai.lac;
    vgCEREGData->cellId = sig_p->cellId;
    vgCEREGData->accessTechnology = sig_p->plmn.accessTechnology;
    vgCEREGData->rac    = sig_p->gprsCellInfo.rac;
  }  

  if (vgCEREGData->rejectCause != sig_p->emmCause)
  { 
    vgCEREGData->causeType = sig_p->causeType;
    if (sig_p->causeType == REJECT_CAUSE_TYPE_EMM)
    {
       vgCEREGData->rejectCause = sig_p->emmCause;
       rejectCauseChange = TRUE;
    }
    else
    {
       if (vgCEREGData->rejectCause != NO_CAUSE)
       {
          rejectCauseChange = TRUE;
          vgCEREGData->rejectCause = NO_CAUSE;
       }
    }
 }
 if ((sig_p->nwPsm.activeTime != vgCEREGData->activeTime) ||
      (sig_p->nwPsm.periodicTau != vgCEREGData->periodicTau))
 {
    psmInfoChange = TRUE;
    /* update CEREG info */
    vgCEREGData->psmInfoPresent = TRUE; 
    vgCEREGData->activeTime = sig_p->nwPsm.activeTime;
    vgCEREGData->periodicTau = sig_p->nwPsm.periodicTau;
  }


  for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
  {
    if ((isEntityActive (profileEntity)) && (ptrToGprsContext (profileEntity) != PNULL))
    {
      /* Send Default PDN parameters to AP */
      mdpdnpMode = (VgMDPDNPMode) getProfileValue (profileEntity, PROF_MDPDNP);
      if((sig_p->attachWithoutPdn == FALSE) && (sig_p->defaultApnPresent == TRUE) && (regStatusChange) && (mdpdnpMode == VG_MDPDNP_ENABLED) &&
        ((newRegStatus == VGMNL_REGISTRATED_HOME) || (newRegStatus == VGMNL_REGISTRATED_ROAMING) ||
        (newRegStatus == VGMNL_REG_HOME_SMS_ONLY) || (newRegStatus == VGMNL_REG_ROAMING_SMS_ONLY) ))
      {
        vgSendMDPDNPData(profileEntity, &(sig_p->defaultApnFromNw),sig_p->defaultPdnTypeFromNw);
      }

      cgregMode = (VgCGREGMode) getProfileValue (profileEntity, PROF_CGREG);
      if (((regStatusChange) && (cgregMode == VG_CGREG_ENABLED)) ||
          ((regStatusChange || locationInfoChange) && 
           (cgregMode == VG_CGREG_ENABLED_WITH_LOCATION_INFO)))
      {
        vgSendCGREGData (profileEntity, FALSE, cgregMode);
      }

      ceregMode = (VgCEREGMode) getProfileValue (profileEntity, PROF_CEREG);
      if (((regStatusChange) && (ceregMode == VG_CEREG_ENABLED)) ||
          ((regStatusChange || locationInfoChange) && 
           (ceregMode == VG_CEREG_ENABLED_WITH_LOCATION_INFO))
           || ((regStatusChange || locationInfoChange || rejectCauseChange) && 
           (ceregMode == VG_CEREG_ENABLED_LOC_INFO_AND_CAUSE))
           || ((regStatusChange || locationInfoChange || psmInfoChange ) && 
           (ceregMode == VG_CEREG_ENABLED_INFO_FOR_PSM))  
           || ((regStatusChange || locationInfoChange || psmInfoChange || rejectCauseChange ) && 
           (ceregMode == VG_CEREG_ENABLED_PSM_INFO_AND_CAUSE)))
      {
        vgSendCEREGData (profileEntity, FALSE, ceregMode);
      } 
    }
  }
  
#if defined (UPGRADE_SHARE_MEMORY) || defined(UPGRADE_SHMCL_SOLUTION)
  writeCgregDataToShareMemory();
#endif /* UPGRADE_SHARE_MEMORY || UPGRADE_SHMCL_SOLUTION */
}

/*--------------------------------------------------------------------------
*
* Function:    vgApexAbpdCounterInd
*
* Parameters:  SignalBuffer       - structure containing signal:
*                                   SIG_APEX_ABPD_COUNTER_IND
*              VgmuxChannelNumber - entity which sent request
*
* Returns:     nothing
*
* Description: This function is called when a SIG_APEX_ABPD_COUNTER_IND
*              signal has been received. The signal originates from DBM
*              and contains the values of the data and packet counters
*              for each PSD Bearer ID. The values for the PSD Bearer ID relevant
*              to the current context ID are passed to the ME using a *MGCOUNT
*              report.
*-------------------------------------------------------------------------*/

void vgApexAbpdCounterInd (const SignalBuffer *signalBuffer,
                            const VgmuxChannelNumber entity)
{
  ApexAbpdCounterInd   *sig_p = &signalBuffer->sig->apexAbpdCounterInd;
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo      *psdStatusInfo_p;
  Int8                 cidCounter;

  PARAMETER_NOT_USED(entity);

  /* Search for the PSD Bearer that corresponds to the active */
  /* (or recently deactivated) cid */

  for (cidCounter = 0; cidCounter < MAX_NUMBER_OF_CIDS; cidCounter++)
  {
    psdStatusInfo_p = gprsGenericContext_p->cidUserData[cidCounter];

    if ((psdStatusInfo_p != PNULL) &&
        (psdStatusInfo_p->psdBearerInfo.connId == sig_p->connId))
    {
      /* compile and send the (unsolicited) report */
      vgSendUnsMGCOUNT (cidCounter, sig_p->ulDataVolumeCounters, sig_p->dlDataVolumeCounters, sig_p->pdnType);
      break;
    }
  }
}

/*--------------------------------------------------------------------------
*
* Function:    vgSendUnsMCGCONTSTAT
*
* Parameters:  
*                        
* Returns:     nothing
*
* Description: This function sends an unsolicited *MCGCONTSTAT message.
*
*-------------------------------------------------------------------------*/
void vgSendUnsMCGCONTSTAT   (const Boolean isActive,
                                const Int8 cid,
                                const Boolean secondaryContext,
                                const AbpdPsdCause abpdCause,
                                const Boolean causePresent,                                
                                const GsmCause cause)
{
  VgmuxChannelNumber profileEntity = VGMUX_CHANNEL_INVALID;
  
  for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
  {
    if((isEntityActive(profileEntity)) &&
       (REPORTING_ENABLED == getProfileValue(profileEntity, PROF_MCGEUNSOL)))
    {
      vgPutNewLine (profileEntity);
      vgPrintf (profileEntity, (Char *)"*MCGCONTSTAT: %d,%d,%d,", (Int8)isActive, cid, (Int8)secondaryContext);
      switch (abpdCause)
      {
        case ABPD_CAUSE_UE_ACTION_OK:
          vgPrintf (profileEntity, (Char *)"%d", 1);
          break;
        case ABPD_CAUSE_NW_ACTION_OK:
          vgPrintf (profileEntity, (Char *)"%d", 2);
          break;
        case ABPD_CAUSE_UE_ACTION_FAIL:
          vgPrintf (profileEntity, (Char *)"%d", 3);
          break;
        case ABPD_CAUSE_DEF_B_DEACT_OK_NW_REJ:
          vgPrintf (profileEntity, (Char *)"%d", 5);
          break;

#if defined (FEA_DEDICATED_BEARER)
        case ABPD_CAUSE_DEDB_ACT_FAIL_NW_MOD_RESP:
          vgPrintf (profileEntity, (Char *)"%d", 4);
          break;
        case ABPD_CAUSE_DED_B_DEACT_LOCAL:
          vgPrintf (profileEntity, (Char *)"%d", 6);
          break;
        case ABPD_CAUSE_DED_B_DEACT_BY_DEF_B_DEACT:
          vgPrintf (profileEntity, (Char *)"%d", 7);
          break;    
#endif /* FEA_DEDICATED_BEARER */

        default:
          vgPrintf (profileEntity, (Char *)"%d", 0);
          break;                    
      }
      if (TRUE == causePresent)
      {
        vgPrintf (profileEntity, (Char *)",%d", cause);        
      }
      vgPutNewLine (profileEntity);
      vgSetCirmDataIndIsUrc(profileEntity, TRUE);      
      vgFlushBuffer (profileEntity);
    }
  }
}

#if defined (FEA_QOS_TFT)
/*--------------------------------------------------------------------------
*
* Function:    vgSendUnsMCGCMOD
*
* Parameters:  
*                        
* Returns:     nothing
*
* Description: This function sends an unsolicited *MCGCMOD message.
*
*-------------------------------------------------------------------------*/
void vgSendUnsMCGCMOD   (const Int8 cid,
                                const AbpdPsdCause abpdCause,
                                const Boolean causePresent,
                                const GsmCause cause)
{
  VgmuxChannelNumber profileEntity = VGMUX_CHANNEL_INVALID;
  
  for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
  {
    if((isEntityActive(profileEntity)) &&
       (REPORTING_ENABLED == getProfileValue(profileEntity, PROF_MCGEUNSOL)))
    {
      vgPutNewLine (profileEntity);
      vgPrintf (profileEntity, (Char *)"*MCGCMOD: %d,", cid);
      switch (abpdCause)
      {
        case ABPD_CAUSE_UE_ACTION_OK:
          vgPrintf (profileEntity, (Char *)"%d", 1);
          break;
        case ABPD_CAUSE_NW_ACTION_OK:
          vgPrintf (profileEntity, (Char *)"%d", 2);
          break;
        case ABPD_CAUSE_UE_ACTION_FAIL:
          vgPrintf (profileEntity, (Char *)"%d", 3);
          break;
        case ABPD_CAUSE_DEDB_ACT_FAIL_NW_MOD_RESP:
          vgPrintf (profileEntity, (Char *)"%d", 4);
          break;
        case ABPD_CAUSE_DEF_B_DEACT_OK_NW_REJ:
          vgPrintf (profileEntity, (Char *)"%d", 5);
          break;
        case ABPD_CAUSE_DED_B_DEACT_LOCAL:
          vgPrintf (profileEntity, (Char *)"%d", 6);
          break;
        case ABPD_CAUSE_DED_B_DEACT_BY_DEF_B_DEACT:
          vgPrintf (profileEntity, (Char *)"%d", 7);
          break;    
        default:
          vgPrintf (profileEntity, (Char *)"%d", 0);
          break;                    
      }
      if (TRUE == causePresent)
      {
        vgPrintf (profileEntity, (Char *)",%d", cause);        
      }
      vgPutNewLine (profileEntity);
      vgSetCirmDataIndIsUrc(profileEntity, TRUE);      
      vgFlushBuffer (profileEntity);
    }
  }    
}
#endif /* FEA_QOS_TFT */

/*--------------------------------------------------------------------------
*
* Function:    vgSendCEREGData
*
* Parameters:  entity  - mux channel number
*              atQuery - AT command / unsolicited indication
*
* Returns:     nothing
*
* Description: Send stat, tac and cellId information about GPRS connection
*-------------------------------------------------------------------------*/

void vgSendCEREGData (const VgmuxChannelNumber entity,
                      const Boolean atQuery ,
                      VgCEREGMode ceregMode )
{
  GprsGenericContext_t *gprsGenericContext_p    = ptrToGprsGenericContext ();
  VgCEREGData          *vgCEREGData             = &(gprsGenericContext_p->vgCEREGData);
  Int8                 accessTechnology         = ACT_NBIOT;
  Char                 activeTimeStr[INT8_BIN_STRING_LENGTH] = {0};
  Char                 periodicTauStr[INT8_BIN_STRING_LENGTH] = {0};

  vgPutNewLine (entity);

  if (atQuery == TRUE)
  {
    vgPrintf (entity,
              (const Char *)"+CEREG: %d,%d",
              (Int16)ceregMode,
              vgCEREGData->regStatus);
  }
  else
  {
    vgPrintf (entity,
              (const Char *)"+CEREG: %d",
              vgCEREGData->regStatus);
    vgSetCirmDataIndIsUrc(entity, TRUE);      
  }
 
  if ((vgCEREGData->state == TRUE) &&
      ((vgCEREGData->tac    != 0) ||
       (vgCEREGData->cellId != 0) ||
       (vgCEREGData->rac    != 0)) &&
      ((ceregMode > VG_CEREG_ENABLED) && (ceregMode < VG_CEREG_NUMBER_OF_SETTINGS)))
  {
    vgPrintf (entity,
              (const Char *)",\"%04X\",\"%08X\",%d,\"%02X\"",
              (Int32)(vgCEREGData->tac),
              vgCEREGData->cellId,
              accessTechnology,
              vgCEREGData->rac);
  }

  else if((ceregMode == VG_CEREG_ENABLED_LOC_INFO_AND_CAUSE) || (ceregMode == VG_CEREG_ENABLED_PSM_INFO_AND_CAUSE) ||
            ((vgCEREGData->psmInfoPresent)&& (ceregMode == VG_CEREG_ENABLED_INFO_FOR_PSM)))
  {  
      vgPrintf (entity,
                (const Char *)",,,,");
  }
    
  if ((vgCEREGData->psmInfoPresent)&&
      ((ceregMode ==VG_CEREG_ENABLED_INFO_FOR_PSM) || (ceregMode == VG_CEREG_ENABLED_PSM_INFO_AND_CAUSE)))
  {
      vgInt8ToBinString(vgCEREGData->activeTime, 8, activeTimeStr);
      vgInt8ToBinString(vgCEREGData->periodicTau, 8, periodicTauStr);
  }

  switch (ceregMode)
  {
      case VG_CEREG_ENABLED_WITH_LOCATION_INFO:
      break; //nothing more to do

      case VG_CEREG_ENABLED_LOC_INFO_AND_CAUSE:
      {
          vgPrintf(entity,
              (const Char *)",0,%d", vgCEREGData->rejectCause);// force to EMM cause - this will be no cause if not EMM type
      break;
      }

      case VG_CEREG_ENABLED_PSM_INFO_AND_CAUSE:
      {          
          vgPrintf(entity,
              (const Char *)",0,%d", vgCEREGData->rejectCause);

          if (vgCEREGData->psmInfoPresent)// both cases for PSM info valid
          {
            vgPrintf(entity,
                (const Char *)",\"%s\",\"%s\"", (const char*) activeTimeStr,(const char*) periodicTauStr );
          }
          break;
      }

      case VG_CEREG_ENABLED_INFO_FOR_PSM:
      {
          if (vgCEREGData->psmInfoPresent)
          {
            vgPrintf (entity, (const Char *)",,"); // need to exclude reject cause display
            vgPrintf(entity,
                (const Char *)",\"%s\",\"%s\"", (const char*)activeTimeStr,(const char*)periodicTauStr );
          }
          break;
      }

      default:
      break;
  }  
  vgPutNewLine (entity);
}

/*--------------------------------------------------------------------------
*
* Function:    vgSendUnsMAPNURI
*
* Parameters:  cid: Associated CID for which APN Uplink Rate Control
*                   info is to be displayed.
*
* Returns:     nothing
*
* Description: Send *MAPNURI URC
*-------------------------------------------------------------------------*/
void vgSendUnsMAPNURI (const Int8 cid)
{
  GprsGenericContext_t        *gprsGenericContext_p = ptrToGprsGenericContext (); 
  VgPsdStatusInfo             *activePsdContext_p   = PNULL;
  VgmuxChannelNumber          profileEntity         = 0;

  if ((cid != CID_NUMBER_UNKNOWN) && (cid < MAX_NUMBER_OF_CIDS))
  {
    activePsdContext_p = gprsGenericContext_p->cidUserData[cid];

    /* Generate URC */
    for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
    {
      if (isEntityActive(profileEntity) && 
          (getProfileValue (profileEntity, PROF_MAPNURI) == PROF_MPLMNAPNURI_ENABLE))
      {
        vgPutNewLine (profileEntity);
        vgPrintf (profileEntity, (const Char *)"*MAPNURI: %d,%d",
                  cid,
                  activePsdContext_p->apnUplinkRateControlInfo.apnRateControlPresent?1:0);

        if (activePsdContext_p->apnUplinkRateControlInfo.apnRateControlPresent)
        {
          vgPrintf (profileEntity, (const Char *)",%d,%d,%d",
                    activePsdContext_p->apnUplinkRateControlInfo.apnRateControlAdditionalExceptionReportsAllowed,
                    activePsdContext_p->apnUplinkRateControlInfo.apnRateControluplinkTimeUnit,
                    activePsdContext_p->apnUplinkRateControlInfo.apnRateControlMaxUplinkRate); 
        }
        
        vgPutNewLine (profileEntity);
        vgSetCirmDataIndIsUrc(profileEntity, TRUE);      
        vgFlushBuffer (profileEntity);
      }
    }
  }
}

/***************************************************************************
 * Processes
 ***************************************************************************/


/* END OF FILE */



