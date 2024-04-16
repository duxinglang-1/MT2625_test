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
 * Handles Packet Data related signals coming from the ABPD module within
 * the application background (AB).
 **************************************************************************/

#define MODULE_NAME "RVPDSIGI"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvgput.h>
#include <rvgpuns.h>
#include <rvpdsigi.h>
#include <rvpdsigo.h>
#include <rvccsigi.h>
#include <rvoman.h>
#include <rvcimxsot.h>
#include <abpd_sig.h>
#include <rvchman.h>
#include <rvcrhand.h>
#include <rvcrerr.h>
#include <rvccut.h>
#include <rvmmut.h>
#include <rvomtime.h>
#include <rvccsigi.h>
#include <rvcmux.h>
#include <rvcimxut.h>
#if !defined (USE_L4MM_ALLOC_MEMORY)
#if defined (USE_BMM_ALLOC_MEMORY)
#include <bmm_sig.h>
#else /* USE_BMM_ALLOC_MEMORY */
#include <tmm_sig.h>
#endif  /* USE_BMM_ALLOC_MEMORY */
#endif  /* !USE_L4MM_ALLOC_MEMORY */
#include <rvutv6ad.h>
#include <rvstkcc.h>
#include <rvstkrnat.h>
#include <rvsleep.h>
#include <gkisig.h>
#include <gkimem.h>


/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  ApexAbpdChannelAllocCnf      apexAbpdChannelAllocCnf;

  ApexAbpdContextInd           apexAbpdContextInd;
  ApexAbpdConnectInd           apexAbpdConnectInd;
  ApexAbpdErrorInd             apexAbpdErrorInd;
  ApexAbpdConnectingInd        apexAbpdConnectingInd;
  ApexAbpdConnectedInd         apexAbpdConnectedInd;
  ApexAbpdPsdBearerStatusInd   apexAbpdPsdBearerStatusInd;
  ApexAbpdDisconnectedInd      apexAbpdDisconnectedInd;
  ApexAbpdBusyInd              apexAbpdBusyInd;

#if defined (FEA_QOS_TFT)
  ApexAbpdPsdModifyCnf         apexAbpdPsdModifyCnf;
  ApexAbpdPsdModifyRej         apexAbpdPsdModifyRej;
  ApexAbpdPsdModifyInd         apexAbpdPsdModifyInd;
#endif

  ApexAbpdActivateDataConnCnf  apexAbpdActivateDataConnCnf;
  ApexAbpdSetupInd             apexAbpdSetupInd;
  ApexAbpdPsdAttachCnf         apexAbpdPsdAttachCnf;
  ApexAbpdPsdDetachCnf         apexAbpdPsdDetachCnf;
#if defined (FEA_PPP)
  ApexAbpdPppLoopbackCnf       apexAbpdPppLoopbackCnf;
  ApexAbpdPppConfigCnf         apexAbpdPppConfigCnf;
  ApexAbpdPppConfigAuthCnf     apexAbpdPppConfigAuthCnf;
#endif /* FEA_PPP */
  ApexAbpdReportCounterCnf     apexAbpdReportCounterCnf;
  ApexAbpdCounterInd           apexAbpdCounterInd;
  ApexAbpdApnReadCnf           apexAbpdApnReadCnf;
  ApexAbpdApnWriteCnf          apexAbpdApnWriteCnf;
#if defined (FEA_UPDIR)
  ApexAbpdSetUpdirInfoCnf      apexAbpdSetUpdirInfoCnf;      
  ApexAbpdUpdirInd             apexAbpdUpdirInd;
#endif
  ApexAbpdListAclCnf           apexAbpdListAclCnf;
  ApexAbpdWriteAclCnf          apexAbpdWriteAclCnf;
  ApexAbpdDeleteAclCnf         apexAbpdDeleteAclCnf;
  ApexAbpdSetAclCnf            apexAbpdSetAclCnf;
  ApexAbpdAclStatusCnf         apexAbpdAclStatusCnf;
  ApexAbpdStkInfoInd           apexAbpdStkInfoInd;
  ApexAbpdWriteRelAssistCnf    apexAbpdWriteRelAssistCnf;
  ApexAbpdReadRelAssistCnf     apexAbpdReadRelAssistCnf;
  ApexAbpdReadApnDataTypeCnf   apexAbpdReadApnDataTypeCnf;
  ApexAbpdWriteApnDataTypeCnf  apexAbpdWriteApnDataTypeCnf;
  ApexAbpdApnUlRateControlInd  apexAbpdApnUlRateControlInd;
  ApexAbpdPlmnUlRateControlInd apexAbpdPlmnUlRateControlInd;
  ApexAbpdPacketDiscardInd     apexAbpdPacketDiscardInd;
  ApexAbpdMtuInd               apexAbpdMtuInd;
    
  ApexSmReadRouteCnf           apexSmReadRouteCnf;
  ApexSmWriteRouteCnf          apexSmWriteRouteCnf;
#if !defined (USE_L4MM_ALLOC_MEMORY)
  UtMemAboveHwmInd             utMemAboveHwmInd;
  UtMemBelowLwmInd             utMemBelowLwmInd;
#endif /* !USE_L4MM_ALLOC_MEMORY */
};

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/


static void vgUpdateErepPdpContextData (ApexAbpdConnectedInd *ptr,
                                         Int8 cid,
                                          const VgmuxChannelNumber entity);

static void vgUpdateErepPdpContextDataFromPsdBearerStatusInd (ApexAbpdPsdBearerStatusInd *ptr,
                                         Int8 cid,
                                          const VgmuxChannelNumber entity);


static void vgUpdateContextActivationStatus (Int8 cid, const VgmuxChannelNumber entity);

static Boolean vgGetErepPdpContextData (Int8 psdBearerId,
                                         PdnAddress *pdnAddress,
                                          Int8 *cid);

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*************************************************************************
*
* Function:     vgIpv4PDNAddrIntToStr
*
* Parameters:   inAddr  - address, type 0000
*               outAddr - address, type 000.000.000.000
*
* Returns:      nothing
*
* Description:  converts IPv4 PDN address from one format to another
*
*************************************************************************/
static void
vgIpv4PDNAddrIntToStr (PdnAddress inAddr, TextualPdnAddress *outAddr)
{
  if (inAddr.addressPresent == TRUE)
  {
    /* We always assume the address length is IPV4_ADDR_LEN */
    outAddr->addressPresent = TRUE;
    outAddr->length =
      (Int8)snprintf ((char *)outAddr->address,
                      MAX_TEXTUAL_PDP_ADDR,
                      "%d.%d.%d.%d",
                      inAddr.ipv4Address[0],
                      inAddr.ipv4Address[1],
                      inAddr.ipv4Address[2],
                      inAddr.ipv4Address[3]);
    FatalCheck (outAddr->length <= MAX_TEXTUAL_PDN_ADDR,
              outAddr->length, MAX_TEXTUAL_PDN_ADDR, 0);
  }
  else
  {
    /* Assume PdpAddress is not present at all */
    outAddr->addressPresent = FALSE;
    outAddr->length         = 0;
    outAddr->address[0]     = NULL_CHAR;
  }
}

/*************************************************************************
*
* Function:     vgDottedDecimalStringToIpv4Ipv6PDPAddr
*
* Parameters:   pdnType - pdnType of IP Address (IPV4 or IPV6)
*               inAddr  - address, type 000.000.000.000
*               outAddr - address, type 0000
*
* Returns:      nothing
*
* Converts PDN address (send by CI task) from format
*
* For IPV4 address:
*
*      255.002.000.032       (format as entered by CI user)
*   or 255.2.00.32
* to
*      0xFF 0x02 0x00 0x20   (format used in PSD stack)
*
* For IPV6 address the number of bytes is 16 rather than 4 but the
* format is the same.
*
* Returns RESULT_CODE_OK  if conversion successful and address in range
*         ERROR_CODE otherwise
*************************************************************************/
static ResultCode_t
vgDottedDecimalStringToIpv4Ipv6PDPAddr (PdnType pdnType, const TextualPdnAddress *inAddr, PdnAddress *outAddr)
{
  Int8          digitStore[MAX_FIELD_DIGITS_IN_DOT_SEPARATED_IP_ADDRESS] = {0};
  Int32          destIndex = 0;
  /* Need to define larger array size so we capture overflow > 255 */
  Int16         tmpPdnAddress[MAX_IPV6_ADDR_LEN] = {0};
  Int8          tmpIndex = 0;
  Int8          index = 0;
  Int8          newInt = 0;
  Int32          j;
  Int8          mult[MAX_FIELD_DIGITS_IN_DOT_SEPARATED_IP_ADDRESS] = { 1 , 10 , 100 };  /* Multiplication factor lookup table */
  ResultCode_t  result = RESULT_CODE_OK;
  Boolean       addressAllZeros = TRUE;

  /* Scan all characters in PDP ADDRESS it will finish with zero */
  while ((result == RESULT_CODE_OK) &&
         (index <inAddr->length) &&
         (inAddr->address[index] !=0))
  {
    newInt = inAddr->address[index];
    /* If not the full stop, process the char, 0x2E is an ASCII '.' */
    if (newInt != DOT_CHAR)
    {
      /* Store character as integer */
      if (newInt >= ZERO_CHAR && newInt <= NINE_CHAR)
      {
        /* Error in input pdpAddress (string) */
        if (destIndex >= MAX_FIELD_DIGITS_IN_DOT_SEPARATED_IP_ADDRESS)
        {
          result = VG_CME_OOR_ADDRESS_ELEMENT;
        }
        else
        {
          digitStore[destIndex] = newInt - ZERO_CHAR;
          destIndex++;
        }
      }
      else
      {
          result = VG_CME_INVALID_CHAR_IN_ADDRESS_STRING;
      }
    }
    else   /*  Found an ASCII '.'  */
    {
      /* Add to converted Pdp Address */
      tmpPdnAddress[tmpIndex] = 0;

      if (destIndex > 0)
      {
        for (j = destIndex; j>0; j--)
        {
          tmpPdnAddress[tmpIndex] += digitStore[j-1] * mult[destIndex - j];
          if (tmpPdnAddress[tmpIndex] > 255)
          {
            result = VG_CME_OOR_ADDRESS_ELEMENT;
          }
        }
        destIndex = 0;
        tmpIndex++;
      }
      else
      {
        /* There were no digits between the dots */
        result = VG_CME_OOR_ADDRESS_ELEMENT;
      }
    }
    index++;
  }

  /* Assign last number of Pdp Address */
  if ((result == RESULT_CODE_OK) && (newInt != DOT_CHAR))
  {
    /* Add to converted Pdp Address */
    tmpPdnAddress[tmpIndex] = 0;

    for (j = destIndex; j>0; j--)
    {
      FatalCheck (destIndex > 0, destIndex, 0, 0);
      tmpPdnAddress[tmpIndex] += digitStore[j-1] * mult[destIndex - j];
    }
    if (tmpPdnAddress[tmpIndex] > 255)
    {
      result = VG_CME_OOR_ADDRESS_ELEMENT;
    }
    tmpIndex++;
  }

  /* Validate converted PdpAddress */
  if (result == RESULT_CODE_OK)
  {
    if (pdnType == PDN_TYPE_IPV4)
    {
      if (tmpIndex != IPV4_ADDR_LEN)
      {
        result = VG_CME_INVALID_ADDRESS_LENGTH;
      }
      else
      {
        outAddr->addressPresent = TRUE;
        for (j = 0; j < IPV4_ADDR_LEN; j++)
        {
            outAddr->ipv4Address [j] = (Int8) tmpPdnAddress [j];
        }
      }

      /* make sure we consider the case when the address is zero */
      if ((outAddr->ipv4Address[0] == 0) &&
          (outAddr->ipv4Address[1] == 0) &&
          (outAddr->ipv4Address[2] == 0) &&
          (outAddr->ipv4Address[3] == 0))
      {
        outAddr->addressPresent = FALSE;
      }
    }
    else if (pdnType == PDN_TYPE_IPV6)
    {
      if (tmpIndex != MAX_IPV6_ADDR_LEN)
      {
        result = VG_CME_INVALID_ADDRESS_LENGTH;
      }
      else
      {
        outAddr->addressPresent = TRUE;
        for (j = 0; j < MAX_IPV6_ADDR_LEN; j++)
        {
            outAddr->ipv6Address [j] = (Int8) tmpPdnAddress [j];
        }
      }

      /* make sure we consider the case when the address is zero */
      for (j = 0; j < MAX_IPV6_ADDR_LEN; j++)
      {
        if (outAddr->ipv6Address[j] != 0)
        {
          addressAllZeros = FALSE;
        }
      }

      if (addressAllZeros)
      {
        outAddr->addressPresent = FALSE;
      }
    }
    else
    {
      /* Not a legal PDN address type for this function. */
      FatalParam (pdnType, 0, 0);
    }
  }
  return (result);
}

#if defined (FEA_MT_PDN_ACT)
/*--------------------------------------------------------------------------
 *
 * Function:    vgPdStartRinging
 *
 * Parameters:  entity        - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description: starts ring timer and associated events
 *-------------------------------------------------------------------------*/

static void vgPdStartRinging (const VgmuxChannelNumber entity)
{
  vgStartRinging (entity);
}
#endif /* FEA_MT_PDN_ACT */

/*************************************************************************
*
* Function:     vgPdGetPsdModifyReason
*
* Parameters:   originalQos
*               currentQos
*               originalTft
*               currentTft
*
* Returns:      modifyReason
*
* Description:
*
*************************************************************************/

VgEREPModifyReason vgPdGetPsdModifyReason (QualityOfService *originalQos,
                                    QualityOfService *currentQos,
                                    TrafficFlowTemplate *originalTft,
                                    TrafficFlowTemplate *currentTft)
{
    Boolean             QosChanged      = FALSE;
    Boolean             TftChanged      = FALSE;
    VgEREPModifyReason  modifyReason    = NUM_OF_EREP_MODIFY_REASON;

    if ((PNULL == originalQos && PNULL != currentQos) ||
        (PNULL != originalQos && PNULL == currentQos))
    {
        QosChanged = TRUE;
    }
    else
    if (PNULL == originalQos && PNULL == currentQos)
    {
        QosChanged = FALSE;
    }
    else
    if (PNULL != originalQos && PNULL != currentQos)
    {

        QosChanged = memcmp(originalQos, currentQos, sizeof(QualityOfService));
    }

    if ((PNULL == originalTft && PNULL != currentTft) ||
        (PNULL != originalTft && PNULL == currentTft))
    {
        TftChanged = TRUE;
    }
    else
    if (PNULL == originalTft && PNULL == currentTft)
    {
        TftChanged = FALSE;
    }
    else
    if (PNULL != originalTft && PNULL != currentTft)
    {
        TftChanged = memcmp(originalTft, currentTft, sizeof(TrafficFlowTemplate));
    }

    if (QosChanged && TftChanged)
    {
        modifyReason    = EREP_REASON_TFT_AND_QOS_CHANGED;
    }
    else
    if (QosChanged)
    {
        modifyReason    = EREP_REASON_QOS_CHANGED;
    }
    else
    if (TftChanged)
    {
        modifyReason    = EREP_REASON_TFT_CHANGED;
    }

    WarnCheck(modifyReason != NUM_OF_EREP_MODIFY_REASON, modifyReason, QosChanged, TftChanged);

    return (modifyReason);
}

#if defined (FEA_MT_PDN_ACT)
/*--------------------------------------------------------------------------
 *
 * Function:    vgPdDisplayRING
 *
 * Parameters:  entity        - entity which sent request
 *
 * Returns:     Nothing
 *
 * Description:
 *-------------------------------------------------------------------------*/

static void vgPdDisplayRING (const VgmuxChannelNumber entity,
                           PdnType pdnType,
                           PdnAddress pdnAddress,
                           AccessPointName apn)
{
  char                  *extTypeStr_p = PNULL;
  Int8                   arrIndx;
  TextualPdnAddress      textualPdnAddress;
  Boolean                enumFound;

#if defined (ENABLE_LONG_AT_CMD_RSP)
  KiAllocZeroMemory(sizeof(Char)*AT_MEDIUM_LARGE_BUFF_SIZE,
                    (void **) &extTypeStr_p);
#else
  KiAllocZeroMemory(sizeof(Char)*VG_PRINTF_CONV_BUFFER,
                    (void **) &extTypeStr_p);
#endif

  enumFound = vgPDNTypeToIndx (pdnType, &arrIndx);
  vgPDNAddrIntToDisplayStr (pdnType, pdnAddress, &textualPdnAddress, entity);
  if (pdnType == PDN_TYPE_NONIP)
  {
    /* No IP address information present */
    if(apn.length > 0)
    {
      TextualAccessPointName textualApn;

      vgNetworkAPNToCiAPN (apn, &textualApn);
      /* see 27.007 6.11 for the format */
      snprintf (extTypeStr_p,
#if defined (ENABLE_LONG_AT_CMD_RSP)
               AT_MEDIUM_LARGE_BUFF_SIZE,
#else
               VG_PRINTF_CONV_BUFFER,
#endif
               "GPRS \"%s\", , \"PPP\", \"%.*s\"",
                (enumFound ?
                (const char *) getSupportedPDNTypesMap()[arrIndx].str :
                "UNKNOWN PDP TYPE"),
               textualApn.length,
               (const char *) textualApn.name);
    }
    else
    {
      /* see 27.007 6.11 for the format - omit APN from the string if it isn't present */
      snprintf (extTypeStr_p,
#if defined (ENABLE_LONG_AT_CMD_RSP)
                AT_MEDIUM_LARGE_BUFF_SIZE,
#else
                VG_PRINTF_CONV_BUFFER,
#endif
                "GPRS \"%s\", , \"PPP\"",
                 (enumFound ?
                 (const char *) getSupportedPDNTypesMap()[arrIndx].str :
                 "UNKNOWN PDP TYPE"));
    }
  }
  else
  {
    if(apn.length > 0)
    {
      TextualAccessPointName textualApn;

      vgNetworkAPNToCiAPN (apn, &textualApn);
      /* see 27.007 6.11 for the format */
      snprintf (extTypeStr_p,
#if defined (ENABLE_LONG_AT_CMD_RSP)
               AT_MEDIUM_LARGE_BUFF_SIZE,
#else
               VG_PRINTF_CONV_BUFFER,
#endif
               "GPRS \"%s\", \"%.*s\", \"PPP\", \"%.*s\"",
                (enumFound ?
                (const char *) getSupportedPDNTypesMap()[arrIndx].str :
                "UNKNOWN PDP TYPE"),
               textualPdnAddress.length,
               (const char *) textualPdnAddress.address,
               textualApn.length,
               (const char *) textualApn.name);
    }
    else
    {
      /* see 27.007 6.11 for the format - omit APN from the string if it isn't present */
      snprintf (extTypeStr_p,
#if defined (ENABLE_LONG_AT_CMD_RSP)
                AT_MEDIUM_LARGE_BUFF_SIZE,
#else
                VG_PRINTF_CONV_BUFFER,
#endif
                "GPRS \"%s\", \"%.*s\", \"PPP\"",
                 (enumFound ?
                 (const char *) getSupportedPDNTypesMap()[arrIndx].str :
                 "UNKNOWN PDP TYPE"),
                textualPdnAddress.length,
                (const char *) textualPdnAddress.address);
    }
  }
  vgDisplayRING (entity, extTypeStr_p, extTypeStr_p);

  KiFreeMemory( (void**)&extTypeStr_p);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgPdAutoAnswer
 *
 * Parameters:  entity - mux channel number
 *
 * Returns: whether we should auto-answer network requests for PDN
 * context activation.
 *
 * Description: returns the same
 *
 *-------------------------------------------------------------------------*/

static Boolean vgPdAutoAnswer (const VgmuxChannelNumber entity, VgmuxChannelNumber* runEntity)
{
  Boolean autoAnswer = FALSE;
  VgmuxChannelNumber profileEntity = 0;

  PARAMETER_NOT_USED (entity);

  while ((!autoAnswer) && (profileEntity < CI_MAX_ENTITIES))
  {
    if (isEntityActive (profileEntity))
    {
      /* The spec isn't quite clear on the condition here: are we meant to
         wait for S1 rings in the case CGAUTO=1?  I assume not: the text
         for +CGAUTO hints that we should answer after the _first_ ring,
         and hence the 1 below. */
      autoAnswer =
         (getProfileValue (profileEntity, PROF_CGAUTO) == CGAUTO_MODE_PS_AUTOANSWER &&
          getProfileValue (profileEntity, PROF_S1) >= 1)
         ||
         ((getProfileValue (profileEntity, PROF_CGAUTO) == CGAUTO_MODE_MODEM_COMPATIBILITY_PS_ONLY ||
           getProfileValue (profileEntity, PROF_CGAUTO) == CGAUTO_MODE_MODEM_COMPATIBILITY_PS_AND_CS)
         &&
          getProfileValue (profileEntity, PROF_S0) > 0  &&
          getProfileValue (profileEntity, PROF_S1) >= getProfileValue (profileEntity, PROF_S0));

    }

    if (autoAnswer)
    {
      *runEntity = profileEntity;
    }

    profileEntity++;
  }

  return (autoAnswer);
}
#endif /* FEA_MT_PDN_ACT */

/*--------------------------------------------------------------------------
 *
 * Function:    translateApnStatusToResult
 *
 * Parameters:  requestStatus - as received in ApexAbgp signal.
 *
 * Returns:     ResultCode_t - result of the AT operation.
 *
 * Description: Translates APN Status to Result Code.
 *-------------------------------------------------------------------------*/
static ResultCode_t translateApnStatusToResult (AbpdRequestStatus       requestStatus)
{
  ResultCode_t        result = RESULT_CODE_ERROR;

  switch (requestStatus)
  {
    case ABPD_REQ_OK:
      result = RESULT_CODE_OK;
      break;

    case ABPD_REQ_ACCESS_DENIED:
      result = VG_CME_SIM_PIN2_REQUIRED;
      break;

    case ABPD_REQ_FILE_NOT_FOUND:
    case ABPD_REQ_RECORD_NOT_FOUND:
      result = RESULT_CODE_ERROR;
      break;

    case ABPD_REQ_INVALID_PARAMS:
      result = VG_CME_INVALID_INDEX;
      break;

    case ABPD_REQ_SIM_ERROR:
      result = VG_CME_SIM_FAILURE;
      break;

    case ABPD_REQ_POWERING_DOWN:
      result = VG_CME_SIM_POWERED_DOWN;
      break;

    case ABPD_REQ_MEMORY_PROBLEM:
      result = VG_CME_MEMORY_FULL;
      break;

    case ABPD_REQ_SERVICE_NOT_AVAILABLE:
      result = VG_CME_FEATURE_NOT_SUPPORTED;
      break;

    case ABPD_REQ_NOT_ALLOWED:
      result = VG_CME_OPERATION_NOT_ALLOWED;
      break;

    default:
      FatalParam( requestStatus, 0,0);
      break;
  }

  return (result);
}

/*************************************************************************
*
* Function:     vgUpdateErepPdpContextData
*
* Parameters:   ptr    - received ApexAbpdConnectedInd signal
*               cid    - pdp context / EPS Bearer CID value
*               entity - mux channel number
*
* Returns:      void
*
* Description:  Stores psdBearerId, pdnType, pdnAddress contained within
*               an ApexAbpdConnectedInd signal and marks that psdBearer as
*               active. If the PSD Bearer becomes deactivated at a later
*               stage (indicated within an ApexAbpdDisconnectedInd signal)
*               ATCI has knowledge of its pdnType and PdnAddress
*               which have to be reported to the CI user if unsolicited
*               events need to be buffered and/or displayed to CI user.
*
*************************************************************************/
static void vgUpdateErepPdpContextData (ApexAbpdConnectedInd *ptr,
                                         Int8 cid,
                                          const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  Boolean               found                 = FALSE;
  VgCGEREPPdpContext    *pdpContext;
  Int8                  psdBearerIdEntry;

  /* Is PSD Bearer already active. If so update context
   * or WarnParam for development */
  for (psdBearerIdEntry = 0 ; psdBearerIdEntry < MAX_NUM_PSD_BEARERS; psdBearerIdEntry++)
  {
    pdpContext = &(gprsGenericContext_p->vgPdpContext[psdBearerIdEntry]);

    if ((pdpContext != PNULL) && (pdpContext->active == TRUE))
    {
      if (pdpContext->psdBearerId == ptr->psdBearerId)
      {
        /* Already active */
        /* Two ConnectedInd's received for same PSD Bearer ID */
        WarnParam(entity, cid, ptr->psdBearerId);

        pdpContext->pdnAddress       = ptr->pdnConnectionAddressInfo.pdnAddress;
        pdpContext->cid              = cid;
        pdpContext->reliabilityClass = ptr->qos.reliabilityClass;
        found = TRUE;
      }
    }
  }

  /* Is a new PSD Bearer ID now connected ? */
  if (found == FALSE)
  {
    for (psdBearerIdEntry = 0 ; (psdBearerIdEntry < MAX_NUM_PSD_BEARERS && !found); psdBearerIdEntry++)
    {
      pdpContext = &(gprsGenericContext_p->vgPdpContext[psdBearerIdEntry]);

      if (pdpContext->active == FALSE)
      {
        /* Store PSD Bearer information and set active */
        pdpContext->active           = TRUE;
        pdpContext->psdBearerId      = ptr->psdBearerId;
        pdpContext->pdnAddress       = ptr->pdnConnectionAddressInfo.pdnAddress;
        pdpContext->cid              = cid;
        pdpContext->reliabilityClass = ptr->qos.reliabilityClass;

        found = TRUE;
      }
    }
  }

  if (found == FALSE)
  {
    /* Not enough storage for erepPdpContexts */
    FatalParam (entity, cid, 0);
  }
}

/*************************************************************************
*
* Function:     vgUpdateErepPdpContextDataFromPsdBearerStatusInd
*
* Parameters:   ptr    - received ApexAbpdConnectedInd signal
*               cid    - pdp context / EPS Bearer CID value
*               entity - mux channel number
*
* Returns:      void
*
* Description:  Stores psdBearerId, pdnType, pdnAddress contained within
*               an ApexAbpdPsdBearerStatusInd signal and marks that psdBearer as
*               active. If the PSD Bearer becomes deactivated at a later
*               stage (indicated within an ApexAbpdDisconnectedInd signal)
*               ATCI has knowledge of its pdnType and PdnAddress
*               which have to be reported to the CI user if unsolicited
*               events need to be buffered and/or displayed to CI user.
*
*************************************************************************/
static void vgUpdateErepPdpContextDataFromPsdBearerStatusInd (ApexAbpdPsdBearerStatusInd *ptr,
                                         Int8 cid,
                                          const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  Boolean               found                 = FALSE;
  VgCGEREPPdpContext    *pdpContext;
  Int8                  psdBearerIdEntry;

  for (psdBearerIdEntry = 0 ; (psdBearerIdEntry < MAX_NUM_PSD_BEARERS && !found); psdBearerIdEntry++)
  {
    pdpContext = &(gprsGenericContext_p->vgPdpContext[psdBearerIdEntry]);

    if (pdpContext->active == FALSE)
    {
      /* Store PSD Bearer information and set active */
      pdpContext->active           = TRUE;
      pdpContext->psdBearerId      = ptr->psdBearerId;
      pdpContext->pdnAddress       = ptr->pdnConnectionAddressInfo.pdnAddress;
      pdpContext->cid              = cid;
      pdpContext->reliabilityClass = ptr->qos.reliabilityClass;

      found = TRUE;
    }
  }
}

/*--------------------------------------------------------------------------
*
* Function:    vgUpdateContextActivationStatus
*
* Parameters:  cid    - The cid for the connection
*              entity - mux channel number
*
* Returns:     nothing
*
* Description: Checks to see if the a DisconnectedInd is associated with
*              a CI task PDP context which is thought to be active. If it
*              is, the 'isActive' flag is set FALSE to indicate its new state.
*-------------------------------------------------------------------------*/

static void vgUpdateContextActivationStatus (Int8 cid, const VgmuxChannelNumber entity)
{
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext();
  GprsContext_t         *gprsContext_p        = ptrToGprsContext(entity);
  VgPsdStatusInfo       *ptr;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(gprsGenericContext_p != PNULL, cid, entity, 0);
  FatalCheck(gprsContext_p != PNULL, cid, entity, 0);
#endif
  ptr = gprsGenericContext_p->cidUserData[cid];

  FatalCheck(gprsGenericContext_p->cidUserData[cid] != PNULL, cid, entity, 0);

  /* PDP context disconnected */
  ptr->isActive = FALSE;

 /* We should never reset PDP address information, as specified by 27.007 in +CGPADDR:
  * For a static address, it will be the one set by the +CGDCONT and +CGDSCONT commands
  * when the context was defined. For a dynamic address it will be the one assigned
  * during the last PDP context activation*/
#if defined (ATCI_SLIM_DISABLE)

  /* stop reporting periodic SNDCP counters when context deactivated */
  if (ptr->countReportType == START_PERIODIC)
  {
    ptr->countReportType   = CANCEL;
    ptr->countReportPeriod = 0;
    gprsContext_p->vgCounterCid = cid;
    vgChManContinueAction (entity, SIG_APEX_ABPD_REPORT_COUNTER_REQ);
  }
#endif
}

/*--------------------------------------------------------------------------
*
* Function:    vgGetErepPdpContextData
*
* Parameters:  IN:
*              psdBearerId   - PSD Bearer ID of the disconnected bearer
*              OUT:
*              PdnAddress    - PdnAddress associated with PSD Bearer being
*                              disconnected
*              cid           - context identifier
*              entity        - mux channel number
*
* Returns:     TRUE  if pdp context entry was found
*              FALSE if pdp context entry was not found
*
* Description: Fetches pdnAddress associated with a PSD Bearer ID
*              which has just been disconnected. ATCI has a record of
*              these parameters because they were stored when the associated
*              ApexAbpdConnectedInd signal was received for the same PSD Bearer
*              ID.
*-------------------------------------------------------------------------*/

static Boolean vgGetErepPdpContextData (Int8 psdBearerId,
                                        PdnAddress *pdnAddress,
                                          Int8 *cid)
{
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  Boolean              found = FALSE;
  Int8                 psdBearerIdEntry  = 0;

  VgCGEREPPdpContext *pdpContext;

  /* Search through all active PSD Bearer ID entries */
  for (psdBearerIdEntry = 0 ; psdBearerIdEntry < MAX_NUM_PSD_BEARERS; psdBearerIdEntry++)
  {
    pdpContext = &(gprsGenericContext_p->vgPdpContext[psdBearerIdEntry]);

    if (pdpContext->active == TRUE)
    {

      if (pdpContext->psdBearerId == psdBearerId)
      {
        /* Delete record of PSD Bearer ID entry now that it is disconnected */
        pdpContext->active = FALSE;

        /* Record pdbAddress for this PSD Beaerer ID */
        *pdnAddress      = pdpContext->pdnAddress;
        *cid             = pdpContext->cid;
        found = TRUE;
      }
    }
  }

  return (found);
}


/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*************************************************************************
*
* Function:     vgPDNAddrIntToDisplayStr
*
* Parameters:   inAddr  - address, in binary
*               outAddr - address, as a human-readable string
*               entity  - the entity on which the data will be displayed.
*
* Returns:      nothing
*
* Description:  Converts PDN address from one format to another.
*               For IPV6 this format depends on the CGPIAF setting.
*
*************************************************************************/
void vgPDNAddrIntToDisplayStr (PdnType pdnType,
                        PdnAddress inAddr, TextualPdnAddress *outAddr,
                        const VgmuxChannelNumber entity)
{
  outAddr->length = 0;

  switch (pdnType)
  {
    case PDN_TYPE_IPV4:
      vgIpv4PDNAddrIntToStr (inAddr, outAddr);
      break;
    case PDN_TYPE_IPV6:
      /* Offset is alwyays 0 for this call because this function is only
       * used for IP addresses and not subnet mask with offset character.
       */
      vgIpv6PDNAddrIntToStr (inAddr, outAddr, 0, entity);
      break;
    case PDN_TYPE_IPV4V6:
    case PDN_TYPE_NONIP:
    default:
      FatalParam (pdnType, 0, 0);
      break;
  }
}

/*************************************************************************
*
* Function:     vgIpv6PDNAddrIntToStr
*
* Parameters:   inAddr  - address, type numeric
*               outAddr - address, type textual.  note that this
*                         string may have already had test put in it so
*                         we need to take its length field in to account.
*               offset  - the offset from the start of the outAddr in to
*                         which to start writing the textual IP address.
*               entity  - The entity we are going to display the test on.
*
* Returns:      nothing
*
* Description:  converts IPv6 PDN address from an array of
*               integers to a string.  The AT command AT+CGPIAF
*               has an effect on how this is displayed.
*
*************************************************************************/
void vgIpv6PDNAddrIntToStr (PdnAddress inAddr,
                       TextualPdnAddress *outAddr,
                       Int8  offset,
                       const VgmuxChannelNumber entity)
{
  VgUtv6adConvErr err;

  if (inAddr.addressPresent == TRUE)
  {
    outAddr->addressPresent = TRUE;

    /* If GCPIAF is set to use the : notation then do something different
     */
    if (getProfileValue(entity, PROF_CGPIAF + PROF_CGPIAF_IPV6_ADDR_FORMAT) == PROF_CGPIAF_ENABLE)
    {
        err = vgUtv6adIpv6BinToColonHexText (inAddr.ipv6Address,
                                             (MAX_TEXTUAL_PDN_ADDR + 1) - offset,
                                             (Char *) outAddr->address + offset,
                                             (getProfileValue(entity, PROF_CGPIAF + PROF_CGPIAF_IPV6_LEADING_ZEROS) == PROF_CGPIAF_ENABLE),
                                             (getProfileValue(entity, PROF_CGPIAF + PROF_CGPIAF_IPV6_COMPRESS_ZEROS) == PROF_CGPIAF_ENABLE));
    }
    else
    {
        err = vgUtv6adIpv6BinToDottedDecimalText (inAddr.ipv6Address,
                                                  (MAX_TEXTUAL_PDN_ADDR + 1) - offset,
                                                  (Char *) outAddr->address + offset);
    }

    FatalCheck (err == VG_UTV6AD_ERR_OK,
              err, outAddr->length, 0);
    if (err == VG_UTV6AD_ERR_OK)
    {
      outAddr->length = (Int8)strlen ((char *)outAddr->address);
    }
    else
    {
      outAddr->length = 0;
      outAddr->addressPresent = FALSE;
    }

    /* We need to check if we overshot the array.  We needed to have room
     * for one extra character for the spacer between remote (source) address and
     * subnet mask
     */
    FatalCheck (outAddr->length <= (MAX_TEXTUAL_PDN_ADDR+1),
              outAddr->length, MAX_TEXTUAL_PDN_ADDR, 0);
  }
  else
  {
    /* Assume PdpAddress is not present at all */
    outAddr->addressPresent = FALSE;
    outAddr->length         = 0;
    outAddr->address[0]     = NULL_CHAR;
  }
}


/*************************************************************************
*
* Function:     vgStringToPDNAddr
*
* Parameters:   pdnType - type of address, currently only IPv4 and v6
*               inAddr  - address, as string
*               outAddr - address, in binary
*
* Returns:
* Returns RESULT_CODE_OK  if conversion successful and address in range
*         ERROR_CODE otherwise
*************************************************************************/
ResultCode_t vgStringToPDNAddr (PdnType      pdnType,
                               const TextualPdnAddress *inAddr,
                               PdnAddress *outAddr)

{
  ResultCode_t result     = RESULT_CODE_OK;
  Int8         index;
  Boolean      colonFound = FALSE;
  Boolean      dotFound   = FALSE;

  switch (pdnType)
  {
    case PDN_TYPE_IPV4:
      result = vgDottedDecimalStringToIpv4Ipv6PDPAddr (pdnType, inAddr, outAddr);
      break;
    case PDN_TYPE_IPV6:
      /* Scan the input for ":"s and "."s so we don't need to use the AT+CGPIAF setting
       * for the address input
       */
      for (index = 0; (index < inAddr->length) && (inAddr->address[index] != '\0'); index++)
      {
        if (inAddr->address[index] == DOT_CHAR)
        {
          dotFound = TRUE;
        }

        if (inAddr->address[index] == COLON_CHAR)
        {
          colonFound = TRUE;
        }
      }

      if (dotFound && !colonFound)
      {
        /* Must be IPV4 or IPV6 dot separated address */
        result = vgDottedDecimalStringToIpv4Ipv6PDPAddr (pdnType, inAddr, outAddr);
      }
      else if (colonFound && !dotFound)
      {
        /* Must be colon separated IPV6 address */
        result = vgColonHexStringToIpv6PDPAddr (inAddr, outAddr, TRUE);
      }
      else
      {
        /* Cannot have ":" and "." in the same string unless we handle
         * embedded IPV6 addresses - which we are not for now
         */
        result = VG_CME_UTV6AD_IPV6_ADDRESS_TOO_SHORT_OR_BAD_CHARS;
      }
      break;
    case PDN_TYPE_IPV4V6:
    case PDN_TYPE_NONIP:
      FatalParam (pdnType, 0, 0);
      break;
    default:
      result = VG_CME_PDP_TYPE_NOT_SUPPORTED;
      break;
  }
  return (result);
}

/*************************************************************************
*
* Function:     vgColonHexStringToIpv6PDPAddr
*
* Parameters:   inAddr  - address, type ::
*               outAddr - address, type 0000
*
* Converts PDN address (send by CI task) from the textual format
* to binary, as used in PSD stack.  The IP address is always and IPV6 address
* of the colon separated type.  The way the string is decoded
*
* Returns RESULT_CODE_OK  if conversion successful and address in range
*         ERROR_CODE otherwise
*************************************************************************/
ResultCode_t
vgColonHexStringToIpv6PDPAddr (const TextualPdnAddress *inAddr, PdnAddress *outAddr,
                            Boolean checkTrailingJunk)

{
  VgUtv6adConvErr err = VG_UTV6AD_ERR_OK;
  Char *endptr;
  outAddr->addressPresent = FALSE;

  if (inAddr->addressPresent)
  {
    err = vgUtv6adIpv6TextToBin ((const Char *) inAddr->address,
                                 &endptr,
                                 outAddr->ipv6Address,
                                 FALSE, /* allow3gppDottedDecimal */
                                 FALSE); /* allowNonfinalEmbeddedIpv4 */
    if ((checkTrailingJunk) && (err == VG_UTV6AD_ERR_OK) && (*endptr != '\0'))
    {
      err = VG_UTV6AD_ERR_TRAILING_JUNK;
    }

    if (err == VG_UTV6AD_ERR_OK)
    {
      outAddr->addressPresent = TRUE;

      if (TRUE == vgIsCurrentAccessTechnologyLte())
      {
        outAddr->ipv6AddressType = IPV6_ADDRESS_INTERFACE_IDENTIFIER;
      }
      else
      {
        outAddr->ipv6AddressType = IPV6_ADDRESS_FULL;
      }
    }
  }
  /* the conversion error codes are each set to a result code value in
     their enumeration */
  return ((ResultCode_t) err);
}

/*************************************************************************
*
* Function:     vgNetworkAPNToCiAPN
*
* Parameters:   inAPN  - APN in PSD network format
*               outAPN - APN in AT command output format
*
* Returns:      nothing
*
* Description:  converts APN from type "2co6cellco2co2uk" to a type
*               like "co.cellco.co.uk"
*
*************************************************************************/

void vgNetworkAPNToCiAPN (AccessPointName inAPN,
                          TextualAccessPointName *outAPN)
{
  Int8    *inPtr  = inAPN.name;
  Int8    *outPtr = outAPN->name;
  Int8    index = 0;
  Int8    len = 0;
  Int8    parseLength = 0;
  Boolean finished = FALSE;

  /* copy APN contents whilst converting characters */
  do
  {
    parseLength = *inPtr;

    FatalCheck(MAX_APN_NAME > parseLength + len, parseLength, MAX_APN_NAME, len);
    inPtr++;
    len++;

    for (index = 0; index < parseLength; index++)
    {
      *(outPtr++) = *(inPtr++);
      len++;
    }

    if (len >= (inAPN.length - 1))
    {
      finished = TRUE;
    }
    else
    {
      *(outPtr++) = DOT_CHAR;
    }
  }
  while (finished == FALSE);

  outAPN->length = inAPN.length - 1;
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSetGsmCauseCallReleaseError
 *
 * Parameters:  errorCode - the gsm cause call error code
 *              entity    - channel on which call was lost
 *
 * Returns:     nothing
 *
 * Description: Sets the current call release error to errorCode and the
 *              error type to gsm cause
 *-------------------------------------------------------------------------*/

void vgSetGsmCauseCallReleaseError (const GsmCause errorCode,
                                     const VgmuxChannelNumber entity)
{
  CallContext_t *callContext_p = ptrToCallContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(callContext_p != PNULL, entity, 0, 0);
#endif
  /* sets the call release error code and error code type */
  callContext_p->vgErrorType = CI_CALL_RELEASE_ERROR_GSM_CAUSE;
  callContext_p->vgGsmCauseCallReleaseError = errorCode;
}

#if defined (FEA_MT_PDN_ACT)
/*--------------------------------------------------------------------------
 *
 * Function:    vgPdMayAnswerWithA
 *
 * Parameters:  entity - mux channel number
 *
 * Returns: whether we may answer network requests for PDN connection
 * activation using ATA, or ATH to reject.
 *
 * Description: returns the same
 *
 *-------------------------------------------------------------------------*/

Boolean vgPdMayAnswerWithA (const VgmuxChannelNumber entity)
{
  Boolean manualAnswer;

  manualAnswer =
    (getProfileValue (entity, PROF_CGAUTO) == CGAUTO_MODE_MODEM_COMPATIBILITY_PS_ONLY ||
     getProfileValue (entity, PROF_CGAUTO) == CGAUTO_MODE_MODEM_COMPATIBILITY_PS_AND_CS);

  return (manualAnswer);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgPdIncomingPdpContextActivationIncoming
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:     nothing
 *
 * Description: say whether we have an outstanding mt pdp context activation
 *
 *-------------------------------------------------------------------------*/

Boolean vgPdIncomingPdpContextActivation (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();

  PARAMETER_NOT_USED (entity);

  return (gprsGenericContext_p->incomingPdpContextActivation);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgPdAnswer
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:     nothing
 *
 * Description: accepts an mt pdp context activation request,
 * after finding an appropriate channel
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgPdAnswer (const VgmuxChannelNumber entity,
                         Int32 cid,
                         AbpdPdpConnType connectionType)
{
  ResultCode_t result = RESULT_CODE_PROCEEDING;
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();

  gprsGenericContext_p->cidOwner[cid] = entity;

  /* accept */
  result = vgPdRespondToMtpca (entity, cid, connectionType);

  if (result != RESULT_CODE_PROCEEDING)
  {
    /* The activation failed so we need to clear the cidOwner */
    gprsGenericContext_p->cidOwner[cid] = VGMUX_CHANNEL_INVALID;

  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgPdReject
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:     nothing
 *
 * Description: rejects an mt pdp context activation request
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgPdReject (const VgmuxChannelNumber entity)
{

  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  ResultCode_t result = RESULT_CODE_OK;

  gprsGenericContext_p->vgAbpdSetupRsp.connId = gprsGenericContext_p->vgAbpdSetupInd.connId;
  gprsGenericContext_p->vgAbpdSetupRsp.connectionAccepted = FALSE;
  vgApexAbpdSetupRsp (entity);
  gprsGenericContext_p->incomingPdpContextActivation = FALSE;

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgPdRINGINGTimerExpiry
 *
 * Parameters:  entity - mux channel number
 *
 * Returns:     nothing
 *
 * Description: displays the RING unsolicited event notification
 *
 *-------------------------------------------------------------------------*/

void vgPdRINGINGTimerExpiry (const VgmuxChannelNumber entity)
{
  GprsGenericContext_t *gprsGenericContext_p    = ptrToGprsGenericContext ();
  VgmuxChannelNumber    runEntity               = VGMUX_CHANNEL_INVALID;
  AccessPointName       apn;
  PdnType               pdnType;

  /* the check that there is still an mtpca has already been done */

  if (gprsGenericContext_p->vgAbpdSetupInd.apnPresent == TRUE)
  {
    apn = gprsGenericContext_p->vgAbpdSetupInd.apn;
  }
  else
  {
    apn.name[0] = 0;
    apn.length = 0;
  }

  pdnType = gprsGenericContext_p->vgAbpdSetupInd.pdnAddress.pdnType;

  if ((PDN_TYPE_IPV4 == pdnType) || (PDN_TYPE_IPV6 == pdnType))
  {
        vgPdDisplayRING (entity,
                         gprsGenericContext_p->vgAbpdSetupInd.pdnAddress.pdnType,
                         gprsGenericContext_p->vgAbpdSetupInd.pdnAddress,
                         apn);
  }
  else if (PDN_TYPE_IPV4V6 == pdnType)
  {
        vgPdDisplayRING (entity,
                         PDN_TYPE_IPV4,
                         gprsGenericContext_p->vgAbpdSetupInd.pdnAddress,
                         apn);
        vgPdDisplayRING (entity,
                         PDN_TYPE_IPV6,
                         gprsGenericContext_p->vgAbpdSetupInd.pdnAddress,
                         apn);
  }
  else /* Must be non IP */
  {
        vgPdDisplayRING (entity,
                         PDN_TYPE_NONIP,
                         gprsGenericContext_p->vgAbpdSetupInd.pdnAddress,
                         apn);
  }

  /* check if auto-answer is on and the required number of rings has
     been reached */
  if (vgPdAutoAnswer (entity, &runEntity))
  {
    if (runEntity == VGMUX_CHANNEL_INVALID)
    {
      /* nothing we can do, reject */
      vgCiRunAtCommandInd (entity, (Char *)"+CGANS=0");
    }
    else
    {
      /* run the ANS command on the appropriate channel */
      vgCiRunAtCommandInd (runEntity, (Char *)"+CGANS=1");
    }
  }
}
#endif /* FEA_MT_PDN_ACT */

/*************************************************************************
*
* Function:     vgApexSmReadRouteCnf
*
* Parameters:   signalBuffer - the signal sent from ABGP
*               entity       - mux channel number
*
* Returns:      nothing
*
* Description:  SMS read routing parameter confirmation handler
*
*************************************************************************/

void vgApexSmReadRouteCnf (const SignalBuffer *signalBuffer,
                           const VgmuxChannelNumber entity)
{
  ApexSmReadRouteCnf *sig_p = &signalBuffer->sig->apexSmReadRouteCnf;

  switch (getCommandId (entity))
  {
    case VG_AT_GP_CGSMS:
    {
      if (sig_p->success == TRUE)
      {
        vgPutNewLine (entity);
        vgPrintf (entity, (const Char*)"+CGSMS: %d", (Int16)sig_p->smsRoute);
        vgPutNewLine (entity);

        setResultCode (entity, RESULT_CODE_OK);
      }
      else
      {
        setResultCode (entity, RESULT_CODE_ERROR);
      }
      break;
    }
    default:
    {
      /* Illegal AT command running SIG_APEX_SM_READ_ROUTE_CNF */
      FatalParam(entity, getCommandId (entity), 0);

//      setResultCode (entity, RESULT_CODE_ERROR);
      break;
    }
  }
}

/*************************************************************************
*
* Function:     vgApexSmWriteRouteCnf
*
* Parameters:   signalBuffer - the signal sent from ABGP
*               entity       - mux channel number
*
* Returns:      nothing
*
* Description:  SMS write routing parameter confirmation handler.
*
*************************************************************************/

void vgApexSmWriteRouteCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
  ApexSmWriteRouteCnf *sig_p = &signalBuffer->sig->apexSmWriteRouteCnf;

  switch (getCommandId (entity))
  {
    case VG_AT_GP_CGSMS:
    {
      if (sig_p->success == TRUE)
      {
        setResultCode (entity, RESULT_CODE_OK);
      }
      else
      {
        setResultCode (entity, RESULT_CODE_ERROR);
      }
      break;
    }
    default:
    {
      /* Illegal AT command running SIG_APEX_SM_WRITE_ROUTE_CNF */
      FatalParam(entity, getCommandId (entity), 0);

//      setResultCode (entity, RESULT_CODE_ERROR);
      break;
    }
  }
}


/****************************************************************************
 *
 * Function:    vgApexAbpdChannelAllocCnf
 *
 * Parameters:  VgmuxChannelNumber  channelNumber
 *              CiMuxReserveChannelCnf  * signal with reservation info
 *
 * Returns:     Nothing
 *
 * Description:  Handling of the signal which indicates if a channel has been
 *               allocated by ABPD for ATCI internal use and if successful
 *               and the channel was allocated for STK proactive command RUN AT
 *               COMMAND then set up the channel to run it.
 *
***************************************************************************/

void vgApexAbpdChannelAllocCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  ApexAbpdChannelAllocCnf   *apexAbpdChannelAllocCnf_p = &signalBuffer->sig->apexAbpdChannelAllocCnf;
  StkEntityGenericData_t *stkGenericContext_p = ptrToStkGenericContext ();
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(stkGenericContext_p != PNULL, entity, 0, 0);
#endif
  if ( apexAbpdChannelAllocCnf_p->success == TRUE)
  /* if the channel was successfully reserved */
  {
    if (( entity == stkGenericContext_p->registeredStkEntity) &&
     (stkGenericContext_p->runAtCmdState == STATE_RESERVING_CHANNEL))
     /* if this was a reservation for STK operations */
    {
      stkGenericContext_p->atCommandData.cmdEntity = apexAbpdChannelAllocCnf_p->channelNumber;
      vgStkSetUpChannelToRunAtCommand (entity);
    }
    else
    {
      /* Unexpected signal */
      FatalParam(apexAbpdChannelAllocCnf_p->channelNumber, entity, 0);
    }
  }
  else
  /* couldn't reserve a channel */
  {
    if (( entity == stkGenericContext_p->registeredStkEntity) &&
    (stkGenericContext_p->runAtCmdState == STATE_RESERVING_CHANNEL))
    /* if STK was waiting for a channel then we must report to SIM that the
     * proactive command RUN AT COMMAND cannot be processed */
    {
      vgStkRunAtCommandFail (entity);
    }
    else
    {
      /* Unexpected signal */
      FatalParam(apexAbpdChannelAllocCnf_p->channelNumber, entity, 0);
    }
  }
}

/*************************************************************************
*
* Function:     vgApexAbpdPsdAttachCnf
*
* Parameters:   signalBuffer - the signal sent from ABPD
*               entity       - mux channel number
*
* Returns:      nothing
*
* Description:  processes the confirmation that is returned from ABPD
*               after the CI Task has sent the PSD attach req.
*
*************************************************************************/

void vgApexAbpdPsdAttachCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  ApexAbpdPsdAttachCnf *sig_p = &signalBuffer->sig->apexAbpdPsdAttachCnf;
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();

  GprsContext_t         *gprsContext_p        = ptrToGprsContext(entity);
  VgmuxChannelNumber    attachDefBearerEntity;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(gprsContext_p != PNULL, entity, 0, 0);
#endif
  switch (getCommandId (entity))
  {
    case VG_AT_GP_CGATT:
#if defined (FEA_MT_PDN_ACT)
    case VG_AT_PF_CGAUTO:
#endif /* FEA_MT_PDN_ACT */
    {
      /* Clear down the attach request flag if it was set */
      gprsContext_p->attachInProgress = FALSE;

      if (gprsContext_p->attachAbortInProgress)
      {
        /* We had a crossover with an abort of the attach - so just generate
         * a warning and ignore this signal.
         */
      }
      else
      {
        if (sig_p->success == TRUE)
        {
          gprsGenericContext_p->gprsServiceState.gprsAttached = TRUE;
          setResultCode (entity, RESULT_CODE_OK);
          /* For LTE - if we have just completed the attach to a network - then we need to
           * connect to the default bearer activated during the attach procedure at
           * this point - if it isn't already
           */
          /* Find the first enabled entity */
          attachDefBearerEntity = findFirstEnabledChannel();

          /* For NB-IOT we need to check if we are allowed to activate default
           * PDN connection on attach - before doing anything (set by AT+CIPCA)
           */
          if ((attachDefBearerEntity != VGMUX_CHANNEL_INVALID) &&
              vgIsCurrentAccessTechnologyLte() &&
              vgPsdAttached() &&
              vgCIPCAPermitsActivateAttachDefBearer() &&
              (getProfileValue (attachDefBearerEntity, PROF_MLTEGCF) != GCF_TEST_MODE_DISABLE))
          {
            if (gprsGenericContext_p->vgLteAttachDefaultBearerState == LTE_ATTACH_DEFAULT_BEARER_NOT_CONNECTED)
            {
              /* Make sure control of PSD actions is released form the channel which
               * generated the AT+CGATT before we go on as otherwise this will
               * not work
               */
              vgChManReleaseControlOfElement(CC_GENERAL_PACKET_RADIO_SYSTEM, entity);

              vgActivateAttachDefBearerContext (attachDefBearerEntity);
            }
          }
        }
        else
        {
          setResultCode (entity, VG_CME_OPERATION_FAILED);
        }
      }
      break;
    }
    default:
    {
      /* Illegal AT command running SIG_APEX_ABGP_GPRS_ATTACH_CNF */
      FatalParam(entity, getCommandId (entity), 0);
//      setResultCode (entity, RESULT_CODE_ERROR);
      break;
    }
  }
}


/*************************************************************************
*
* Function:     vgApexAbpdPsdDetachCnf
*
* Parameters:   signalBuffer - the signal sent from ABPD
*               entity       - mux channel number
*
* Returns:      nothing
*
* Description:  processes the confirmation that is returned from ABPD
*               after the CI Task has sent the PSD detach req.
*
*************************************************************************/

void vgApexAbpdPsdDetachCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  ApexAbpdPsdDetachCnf *sig_p                = &signalBuffer->sig->apexAbpdPsdDetachCnf;
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();

  GprsContext_t         *gprsContext_p        = ptrToGprsContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(gprsContext_p != PNULL, entity, 0, 0);
#endif
  switch (getCommandId (entity))
  {
    case VG_AT_GP_CGATT:
    {
      if (sig_p->success == TRUE)
      {
        /* We may have been in the middle of aborting an attach - so
         * clear the flag
         */
        gprsContext_p->attachAbortInProgress = FALSE;
        gprsContext_p->attachInProgress      = FALSE;

        gprsGenericContext_p->gprsServiceState.gprsAttached = FALSE;
        setResultCode (entity, RESULT_CODE_OK);
      }
      else
      {
        setResultCode (entity, VG_CME_OPERATION_FAILED);
      }
      break;
    }
    default:
    {
      /* Illegal AT command running SIG_APEX_ABGP_GPRS_DETACH_CNF */
      FatalParam(entity, getCommandId (entity), 0);

//      setResultCode (entity, RESULT_CODE_ERROR);
      break;
    }
  }
}

/*************************************************************************
*
* Function:     vgApexAbpdAttachDefBearerActInd
*
* Parameters:   signalBuffer - the signal sent from ABPD
*               entity       - mux channel number
*
* Returns:      nothing
*
* Description:  Process indication from ABPD that the default bearer
*               activated during the attach is now activated.
*               ATCI will do nothing about this if it is not in the correct
*               state or AT*MLTEGCF is not set.  Otherwise it may trigger
*               connection to this bearer by ATCI automatically.
*
*************************************************************************/
void vgApexAbpdAttachDefBearerActInd (const SignalBuffer *signalBuffer,
                                   const VgmuxChannelNumber entity)
{

  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgmuxChannelNumber    attachDefBearerEntity;

  /* Ignore the entity and find the appropriate context from the channel number
   * in the signal
   */
  PARAMETER_NOT_USED(entity);

  attachDefBearerEntity = findFirstEnabledChannel();

  /* If we have a spare channel, and we are registered on LTE and
   * the default bearer activated during attach is not already connected
   * at ATCI level and the MLTEGCF mode is set - then connect to it now
   */
  /* For NB-IOT we need to check if we are allowed to activate default
   * PDN connection on attach - before doing anything (set by AT+CIPCA)
   */
  if ((attachDefBearerEntity != VGMUX_CHANNEL_INVALID) &&
      vgIsCurrentAccessTechnologyLte() &&
      vgPsdAttached() &&
      vgCIPCAPermitsActivateAttachDefBearer() &&
      (getProfileValue (attachDefBearerEntity, PROF_MLTEGCF) != GCF_TEST_MODE_DISABLE))
  {
    if (gprsGenericContext_p->vgLteAttachDefaultBearerState == LTE_ATTACH_DEFAULT_BEARER_NOT_CONNECTED)
    {
      vgActivateAttachDefBearerContext (attachDefBearerEntity);
    }
  }
}


/*************************************************************************
*
* Function:     vgApexAbpdContextInd
*
* Parameters:   signalBuffer - the signal sent from ABPD
*               entity       - mux channel number
*
* Returns:      nothing
*
* Description:  Process context indication from ABPD.  This will contain
*               the connId assigned to the PSD connection.
*
*
*************************************************************************/
void vgApexAbpdContextInd (const SignalBuffer *signalBuffer,
                          const VgmuxChannelNumber entity)
{
  ApexAbpdContextInd     *sig_p                = &signalBuffer->sig->apexAbpdContextInd;
  GprsGenericContext_t   *gprsGenericContext_p = ptrToGprsGenericContext ();
  Int8                   cid                   = CID_NUMBER_UNKNOWN;
  Int8                   cidCount;

  /* Ignore the entity and find the appropriate context from the channel number
   * in the signal
   */
  PARAMETER_NOT_USED(entity);

  FatalCheck (sig_p->channelNumber < MAX_NUM_MDL_CHANNELS, entity, sig_p->channelNumber, 0);

  if (isEntityActive(sig_p->channelNumber))
  {
    /* Find the CID associated with this connection */
    for (cidCount = vgGpGetMinCidValue(entity); cidCount < MAX_NUMBER_OF_CIDS; cidCount++)
    {
      if ((gprsGenericContext_p->cidOwner[cidCount] == sig_p->channelNumber) &&
          (gprsGenericContext_p->cidUserData[cidCount] != PNULL) &&
          (gprsGenericContext_p->cidUserData[cidCount]->pendingContextActivation == TRUE))
      {
        cid = cidCount;
      }
    }
  }

  if (cid != CID_NUMBER_UNKNOWN)
  {
    /* Copy the connId from the signal in to the CID structure */
    gprsGenericContext_p->cidUserData[cid]->psdBearerInfo.connId = sig_p->connId;
  }
  else
  {
    FatalAssert ("Unexpected AbpdContextInd");
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdConnectInd
 *
 * Parameters:  signalBuffer - the signal sent from ABGP
 *              entity       - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Handles the SIG_APEX_ABPD_CONNECT_IND signal received
 *              from AB task ABPD module.
 *
 *-------------------------------------------------------------------------*/

void vgApexAbpdConnectInd (const SignalBuffer *signalBuffer,
                         const VgmuxChannelNumber entity)
{
  ApexAbpdConnectInd     *sig_p                = &signalBuffer->sig->apexAbpdConnectInd;
  GprsGenericContext_t   *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo        *activePsdContext_p   = PNULL;

#if defined (ENABLE_LONG_AT_CMD_RSP)
  Char                   resultText [AT_SMALL_BUFF_SIZE] = {0};
#else
  Char                   resultText [VG_MAX_AT_DATA_IN_SIGNAL_LENGTH] = {0};
#endif

  Int8                   cid;
  VgmuxChannelNumber     channel               = VGMUX_CHANNEL_INVALID;
  ConnectionClass_t      connectionClass       = PT_CONNECTION;

  /* Ignore the entity */
  PARAMETER_NOT_USED(entity);

  cid = vgFindCidLinkedToConnId(sig_p->connId);

  if (cid != CID_NUMBER_UNKNOWN)
  {
    channel = vgFindEntityLinkedToCid(cid);
  }

  if ((cid < MAX_NUMBER_OF_CIDS) && (channel != VGMUX_CHANNEL_INVALID))
  {
    activePsdContext_p = gprsGenericContext_p->cidUserData[cid];

    WarnAssert (activePsdContext_p != PNULL);

    /* Check we were pending an activation at this point */
    if ((activePsdContext_p != PNULL) &&
        ((activePsdContext_p->pendingContextActivation == TRUE) ||
         (activePsdContext_p->pendingDataConnectionActivation == TRUE)))
    {
      /* display service report */
      if ((getProfileValue (channel, PROF_CR) == REPORTING_ENABLED) ||
          (getProfileValue (channel, PROF_CR) == REPORTING_EXTENDED_ENABLED))
      {
        switch (activePsdContext_p->psdBearerInfo.connType)
        {

#if defined (FEA_PPP)
          case ABPD_CONN_TYPE_PPP:
            snprintf((char *)resultText,
#if defined (ENABLE_LONG_AT_CMD_RSP)
                    AT_SMALL_BUFF_SIZE,
#else
                    VG_MAX_AT_DATA_IN_SIGNAL_LENGTH,
#endif
                    (char *)"+CR: GPRS \"PPP\"");
            break;
          case ABPD_CONN_TYPE_CORE_PPP:
            snprintf((char *)resultText,
#if defined (ENABLE_LONG_AT_CMD_RSP)
                    AT_SMALL_BUFF_SIZE,
#else
                    VG_MAX_AT_DATA_IN_SIGNAL_LENGTH,
#endif
                    (char *)"+CR: GPRS \"M-CPPP\"");
            break;
#endif /* FEA_PPP */

          case ABPD_CONN_TYPE_PACKET_TRANSPORT:
            snprintf((char *)resultText,
#if defined (ENABLE_LONG_AT_CMD_RSP)
                    AT_SMALL_BUFF_SIZE,
#else
                    VG_MAX_AT_DATA_IN_SIGNAL_LENGTH,
#endif
                    (char *)"+CR: GPRS \"M-PT\"");
            break;
          default:
            /* We should not get here as we should not have received the CONNECT_IND
             * for ABPD_CONN_TYPE_NONE
             */
            FatalParam (activePsdContext_p->psdBearerInfo.connType, channel, cid);
            break;
        }

        sendIntermidateResultCodeToCrm (resultText, channel);
      }

      /* set connection state to on line
       * user call id hard coded to MIN_PSD_USER_CALL_ID as only one PSD
       * connection with connection to PC allowed on one channel.
       */
      vgOpManSetConnectionStatus (channel,
                                   MIN_PSD_USER_CALL_ID,
                                    CONNECTION_ON_LINE);

      /* set connection type to correct value */
      switch (activePsdContext_p->psdBearerInfo.connType)
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
          /* We should not get here as we should not have received the CONNECT_IND
           * for ABPD_CONN_TYPE_NONE
           */
          FatalParam (activePsdContext_p->psdBearerInfo.connType, channel, cid);
          break;
      }
      setConnectionType (channel, connectionClass);

      sendResultCodeToCrm (RESULT_CODE_CONNECT, channel);

      /* reset connection type
       */
      setConnectionType (channel, CONNECTION_TERMINATOR);

      /* We are only waiting for Connection to be established now, no need to keep the control
       * of the GPRS module
       */
      vgChManReleaseControlOfElement(CC_GENERAL_PACKET_RADIO_SYSTEM, entity);

      /* further processing occurs when the CONNECT string has been sent to the
       * mux, see rvcrman.c */
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdErrorInd
 *
 * Parameters:  signalBuffer - the signal sent from ABGP
 *              entity       - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Handles the SIG_APEX_ABPD_ERROR_IND signal received
 *              from AB task ABPD module.
 *-------------------------------------------------------------------------*/
void vgApexAbpdErrorInd (const SignalBuffer *signalBuffer,
                        const VgmuxChannelNumber entity)
{
  ApexAbpdErrorInd       *sig_p                 = &signalBuffer->sig->apexAbpdErrorInd;
  GprsGenericContext_t   *gprsGenericContext_p  = ptrToGprsGenericContext ();
  OpmanGenericContext_t  *opManGenericContext_p = ptrToOpManGenericContext ();
  GprsContext_t          *gprsContext_p         = PNULL;
  VgPsdStatusInfo        *activePsdContext_p    = PNULL;
  Int8                   cid                    = CID_NUMBER_UNKNOWN;
  Int8                   cidCount;
  VgmuxChannelNumber     channel                = VGMUX_CHANNEL_INVALID;
  ConnectionClass_t      connectionClass        = PT_CONNECTION;

  /* Ignore the entity */
  PARAMETER_NOT_USED(entity);

  if (sig_p->connId == INVALID_CONN_ID)
  {
    /* If the connId was not valid then we need to find the connection
     * by seeing which was pending connection or modification
     */
    for (cidCount = vgGpGetMinCidValue(entity); cidCount < MAX_NUMBER_OF_CIDS; cidCount++)
    {
      /* We must assume that as this was pending activation or modification - it must have been the one
       * which the ErrorInd is for.
       */
      if ((gprsGenericContext_p->cidUserData[cidCount] != PNULL) &&
          ((gprsGenericContext_p->cidUserData[cidCount]->pendingContextActivation == TRUE) ||
           (gprsGenericContext_p->cidUserData[cidCount]->pendingDataConnectionActivation == TRUE)
#if defined (FEA_QOS_TFT)
           || (gprsGenericContext_p->cidUserData[cidCount]->pendingContextModification == TRUE)
#endif
           ))
      {
        cid = cidCount;
      }
    }
  }
  else
  {
    cid = vgFindCidLinkedToConnId(sig_p->connId);
  }

  if (cid != CID_NUMBER_UNKNOWN)
  {
    channel = vgFindEntityLinkedToCid(cid);
  }

  if ((cid < MAX_NUMBER_OF_CIDS) && (channel != VGMUX_CHANNEL_INVALID))
  {

    activePsdContext_p = gprsGenericContext_p->cidUserData[cid];

    WarnAssert (activePsdContext_p != PNULL);

    if (activePsdContext_p != PNULL)
    {

      if ((TRUE == activePsdContext_p->pendingContextActivation) ||
          (TRUE == activePsdContext_p->pendingDataConnectionActivation))
      {
        vgSendUnsMCGCONTSTAT(FALSE, cid,
#if defined (FEA_DEDICATED_BEARER)
                             activePsdContext_p->psdBearerInfo.secondaryContext,
#else
                             FALSE,
#endif /* FEA_DEDICATED_BEARER */
                             sig_p->abpdCause,
                             sig_p->causePresent,
                             sig_p->cause);
      }
#if defined (FEA_QOS_TFT)
      else
      if (TRUE == activePsdContext_p->pendingContextModification)
      {
        vgSendUnsMCGCMOD(cid,
                          sig_p->abpdCause,
                          sig_p->causePresent,
                          sig_p->cause);
      }
#endif /* FEA_QOS_TFT */

      /* Only disconnect if we were pending an activation - for a modify we only send
       * out the error code
       */
      if ((activePsdContext_p->pendingContextActivation == TRUE) ||
          (activePsdContext_p->pendingDataConnectionActivation == TRUE))
      {
        switch (activePsdContext_p->psdBearerInfo.connType)
        {
#if defined (FEA_PPP)
          case ABPD_CONN_TYPE_PPP:
            connectionClass = PPP_CONNECTION;
            break;
          case ABPD_CONN_TYPE_CORE_PPP:
            connectionClass = PPP_CONNECTION;
            break;
#endif /* FEA_PPP */
          case ABPD_CONN_TYPE_PACKET_TRANSPORT:
            connectionClass = PT_CONNECTION;
            break;
          default:
            /* For ABPD_CONN_TYPE_NONE we have no valid connection class
             * Set to something to shut lint up.
             */
            connectionClass = PT_CONNECTION;
            break;
        }
      }

      if (activePsdContext_p->pendingContextActivation == TRUE)
      {
        /* We might have got an ErrorInd for a pending modification
         * so we need to handle that differently - i.e. we don't want to
         * simply disconnect the context here!
         */
        if (activePsdContext_p->psdBearerInfo.connType != ABPD_CONN_TYPE_NONE)
        {
          /* The operations manager connection status
           * is only updated for connection with a data connection (i.e. PT or PPP)
           */
          vgOpManSetConnectionStatus (channel,
                                      MIN_PSD_USER_CALL_ID,
                                      CONNECTION_OFF_LINE);
          vgOpManDropConnection (channel, connectionClass);

          vgCiMuxCloseDataConnection (channel);
        }
      }
      else if (activePsdContext_p->pendingDataConnectionActivation == TRUE)
      {
        /* If we were just trying to activate a data connection to an already
         * active CID (previously activated with no data connection), then
         * we just reset the connType to NONE and clear down the data
         * connection status.
         */
        vgOpManSetConnectionStatus (channel,
                                    MIN_PSD_USER_CALL_ID,
                                    CONNECTION_OFF_LINE);
        vgOpManDropConnection (channel, connectionClass);

        activePsdContext_p->psdBearerInfo.connType = ABPD_CONN_TYPE_NONE;
      }


      if (sig_p->causePresent == TRUE)
      {
        vgSetGsmCauseCallReleaseError (sig_p->cause, channel);
      }
      else
      {
        /* Cause is different depending on if we are on LTE or 2G/3G */
        if (vgIsCurrentAccessTechnologyLte())
        {
          vgSetGsmCauseCallReleaseError (ESM_CAUSE_NO_CAUSE_SET, channel);
        }
        else
        {
          vgSetGsmCauseCallReleaseError (SM_CAUSE_NO_CAUSE_SET, channel);
        }
      }

      /* We may have been attempting to deactivate the channel in which case
       * we don't want to set the result code.
       * For LTE - if we were trying to connect to the default bearer
       * activated during the attach - then we also don't generate the result
       * code.
       */
      if ((opManGenericContext_p->channelState [channel].isDisconnecting == FALSE)
           && (!((cid == gprsGenericContext_p->lteAttachDefaultBearerCid) &&
                 (gprsGenericContext_p->vgLteAttachDefaultBearerState == LTE_ATTACH_DEFAULT_BEARER_PENDING_CONNECTION)))
          )
      {
        if ((gprsGenericContext_p->hangupChannel != VGMUX_CHANNEL_INVALID) &&
            (ptrToGprsContext(gprsGenericContext_p->hangupChannel) != PNULL) &&
            (ptrToGprsContext(gprsGenericContext_p->hangupChannel)->vgHangupCid == cid))
        {
          /* We were attempting to disconnect from another channel - so send
           * ERROR on that channel too.
           * TODO: May need to have a hangup channel for each cid - as this
           * error could have been for a different channel.
           * This would be an extremely unlikely event!
           */
          setResultCode ( gprsGenericContext_p->hangupChannel, RESULT_CODE_ERROR);

        }
        setResultCode (channel, RESULT_CODE_ERROR);
      }

      if (activePsdContext_p->pendingContextActivation == TRUE)
      {
        gprsContext_p = ptrToGprsContext(channel);

        /* ensure active context is reset */
        if ((gprsContext_p != PNULL) &&
            (gprsContext_p->activePsdBearerContextWithDataConn != PNULL) &&
            (gprsContext_p->activePsdBearerContextWithDataConn->cid == cid))
        {
          /* This CID was associated with a data connection on this channel
           * - so free up the pointer now as it is disconnected.
           */
          gprsContext_p->activePsdBearerContextWithDataConn = PNULL;
        }
        /*
         * Free up the CID for someone else to use.
         */
        vgOpManMakeCidAvailable (cid, channel);

        /* Make sure we clear down some other crucial settings to allow someone else
         * to use the CID.
         */
        activePsdContext_p->psdBearerInfo.channelNumber = VGMUX_CHANNEL_INVALID;
        activePsdContext_p->psdBearerInfo.connType = ABPD_CONN_TYPE_NONE;
        activePsdContext_p->psdBearerInfo.connId = INVALID_CONN_ID;
        activePsdContext_p->psdBearerInfo.psdBearerId = PSD_BEARER_ID_UNASSIGNED;
        activePsdContext_p->psdBearerInfo.modifiedBySim = FALSE;
        activePsdContext_p->psdBearerInfo.simMods = 0;
        activePsdContext_p->psdBearerInfo.flowControlType = FC_NONE;
      }

      /* Reset flags for the context. */
      activePsdContext_p->pendingContextActivation = FALSE;
      activePsdContext_p->pendingDataConnectionActivation = FALSE;

#if defined (FEA_QOS_TFT)
      activePsdContext_p->pendingContextModification = FALSE;
#endif

      activePsdContext_p->pendingContextDeactivation = FALSE;
      activePsdContext_p->pendingUnsolicitedContextActivation = FALSE;

#if defined (FEA_DEDICATED_BEARER)
      activePsdContext_p->secondaryContextCidPendingActivation = CID_NUMBER_UNKNOWN;
#endif /* FEA_DEDICATED_BEARER */

      gprsContext_p = ptrToGprsContext(channel);

      if ((gprsContext_p != PNULL) && (gprsContext_p->vgHangupCid == cid))
      {
        /* Reset hangupCid */
        gprsContext_p->vgHangupCid = CID_NUMBER_UNKNOWN;
      }

      /* Clear down UL rate control flags */
      activePsdContext_p->apnUplinkRateControlInfo.apnRateControlPresent = FALSE;
      activePsdContext_p->apnUplinkRateControlInfo.apnRateControlAdditionalExceptionReportsAllowed = FALSE;
      activePsdContext_p->apnUplinkRateControlInfo.apnRateControlMaxUplinkRate = 0;
      activePsdContext_p->apnUplinkRateControlInfo.apnRateControluplinkTimeUnit = 0;

      if ((cid == gprsGenericContext_p->lteAttachDefaultBearerCid) &&
          ((gprsGenericContext_p->vgLteAttachDefaultBearerState == LTE_ATTACH_DEFAULT_BEARER_PENDING_CONNECTION) ||
          (gprsGenericContext_p->vgLteAttachDefaultBearerState == LTE_ATTACH_DEFAULT_BEARER_CONNECTED)))
      {
        /* We were trying to connect to the default bearer activated during the
         * attach - but something went wrong (maybe we detached for example
         * - so we will have to clear all the settings now and retry when we
         * attach again.
         * Or we simply got an error whilst it was already connected - so we need to free it then too.
         */
        gprsGenericContext_p->lteAttachDefaultBearerCid     = CID_NUMBER_UNKNOWN;
        gprsGenericContext_p->vgLteAttachDefaultBearerState = LTE_ATTACH_DEFAULT_BEARER_NOT_CONNECTED;

        /* Clear the CID so someone else can use it */
        vgInitialiseCidData (activePsdContext_p, cid);
      }
    }

    if ((gprsGenericContext_p->hangupChannel != VGMUX_CHANNEL_INVALID) &&
        (ptrToGprsContext(gprsGenericContext_p->hangupChannel) != PNULL) &&
        (ptrToGprsContext(gprsGenericContext_p->hangupChannel)->vgHangupCid == cid))
    {
      /* reset the hangupCid for this channel too */
      ptrToGprsContext(gprsGenericContext_p->hangupChannel)->vgHangupCid = CID_NUMBER_UNKNOWN;

      gprsGenericContext_p->hangupChannel = VGMUX_CHANNEL_INVALID;
    }

    /* We need to check if we were trying to disable the channel even though
     * we got an ErrorInd.
     * If we were disabling the channel then check if all channels are disabled
     * or if we need to diabled further contexts associated with this channel
     */
    vgCheckDisconnectAllProgress (channel);

  }
  else
  {
    /* We got an unknown errorInd.  This may have been a crossover, so
     * just generate a warning.
     */
    WarnParam (sig_p->connId, sig_p->abpdCause, sig_p->cause);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdConnectingInd
 *
 * Parameters:  signalBuffer - the signal sent from ABGP
 *              entity       - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Handles the SIG_APEX_ABPD_CONNECTING_IND signal received
 *              from AB task ABPD module.
 *-------------------------------------------------------------------------*/
void vgApexAbpdConnectingInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  ApexAbpdConnectingInd  *sig_p                = &signalBuffer->sig->apexAbpdConnectingInd;
  GprsGenericContext_t   *gprsGenericContext_p = ptrToGprsGenericContext ();
#if defined (FEA_MT_PDN_ACT)
  GprsContext_t          *gprsContext_p        = PNULL;
#endif
  VgPsdStatusInfo        *activePsdContext_p   = PNULL;
  Int8                   cid;
  VgmuxChannelNumber     channel = VGMUX_CHANNEL_INVALID;

  /* Ignore the entity */
  PARAMETER_NOT_USED(entity);

  cid = vgFindCidLinkedToConnId(sig_p->connId);

  if (cid != CID_NUMBER_UNKNOWN)
  {
    channel = vgFindEntityLinkedToCid(cid);
  }

  if ((cid < MAX_NUMBER_OF_CIDS) && (channel != VGMUX_CHANNEL_INVALID))
  {

    activePsdContext_p = gprsGenericContext_p->cidUserData[cid];

    WarnAssert (activePsdContext_p != PNULL);

    /* Check we were pending an activation at this point */
    if ((activePsdContext_p != PNULL) &&
        (activePsdContext_p->pendingContextActivation == TRUE))
    {
      switch (getCommandId (channel))
      {

#if defined (FEA_MT_PDN_ACT)
        case VG_AT_GP_CGANS:
        case VG_AT_CC_A:
          /* We acceped an incoming PSD connection - 2G/3G only -
           * check connId is matching the setupRsp we sent.
           */
          gprsContext_p = ptrToGprsContext(channel);
#if defined (ATCI_SLIM_DISABLE)

          FatalCheck (gprsContext_p != PNULL, channel, sig_p->connId, cid);
#endif
          FatalCheck (gprsGenericContext_p->vgAbpdSetupRsp.connId == sig_p->connId,
                      channel, sig_p->connId, cid);
          break;
#endif /* FEA_MT_PDN_ACT */

        case VG_AT_GP_D:
        case VG_AT_GP_CGACT:
        case VG_AT_GP_CGDATA:
          /* We get this signal during mopca, but for mopca we don't need
             to do anything.  Leave the result code alone. */
          break;
        default:
          /* Just ignore otherwise */
          break;
      }
    }
    else
    {
      /* We are in the wrong state */
      FatalParam (entity, sig_p->connId, cid);
    }

  }
  else
  {
    /* Erronious data from ABPD */
    FatalParam (entity, sig_p->connId, cid);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdConnectedInd
 *
 * Parameters:  signalBuffer - the signal sent from ABGP
 *              entity       - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Handles the SIG_APEX_ABPD_CONNECTED_IND signal received
 *              from AB task ABPD module.
 *-------------------------------------------------------------------------*/
void vgApexAbpdConnectedInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{

  ApexAbpdConnectedInd   *sig_p                = &signalBuffer->sig->apexAbpdConnectedInd;
  GprsGenericContext_t   *gprsGenericContext_p = ptrToGprsGenericContext ();
  GprsContext_t          *gprsContext_p        = PNULL;
  VgPsdStatusInfo        *activePsdContext_p   = PNULL;
#if defined (FEA_DEDICATED_BEARER)
  VgPsdStatusInfo        *secondaryContext_p   = PNULL;
  Int8                   psdBearerId           = PSD_BEARER_ID_UNASSIGNED;
#endif /* FEA_DEDICATED_BEARER */
  Int8                   cid                   = CID_NUMBER_UNKNOWN;
  VgmuxChannelNumber     channel               = VGMUX_CHANNEL_INVALID;
  Boolean                connectionOk          = FALSE;
  TextualAccessPointName convertedAPN;
  Boolean                isUEInitiated         = FALSE;
 
#if defined (FEA_MT_PDN_ACT)
  Int8                   primaryCid;
#endif

  Int8                   newCid                = CID_NUMBER_UNKNOWN;
  VgPsdStatusInfo        *newPsdContext_p      = PNULL;

  /* Ignore the entity */
  PARAMETER_NOT_USED (entity);

  /* Check if this is unsolicited - this can happen when we have an LTE
   * dedicated bearer set up by the network - which can happen for a number of
   * reasons
   */
  if (sig_p->abpdCause == ABPD_CAUSE_UE_ACTION_OK)
  {
    /* UE initiated action - so we must have caused this.  In which case
     * we need to deal with it accordingly
     */
    cid = vgFindCidLinkedToConnId(sig_p->connId);

    if (cid != CID_NUMBER_UNKNOWN)
    {
      channel = vgFindEntityLinkedToCid(cid);
    }

    if ((cid < MAX_NUMBER_OF_CIDS) && (channel != VGMUX_CHANNEL_INVALID))
    {

      activePsdContext_p = gprsGenericContext_p->cidUserData[cid];

      WarnAssert (activePsdContext_p != PNULL);

      /* Check we were pending an activation at this point */
      if ((activePsdContext_p != PNULL) &&
          (activePsdContext_p->pendingContextActivation == TRUE))
      {
        /* Some fields must be the same as the ones we asked for - otherwise
         * there is a coding error
         */
#if defined (FEA_DEDICATED_BEARER)
        FatalAssert (activePsdContext_p->psdBearerInfo.secondaryContext == sig_p->secondaryContext);
        FatalAssert (activePsdContext_p->psdBearerInfo.primaryConnId == sig_p->primaryConnId);
#endif /* FEA_DEDICATED_BEARER */

        FatalAssert (activePsdContext_p->psdBearerInfo.flowControlType == sig_p->flowControlType);

        /* For UE initiated ABPD_CONN_TYPE_NONE connection - we need to set the
         * result code to OK at this point.. but only if this was not a primary
         * connection activated as a result of a secondary activation triggering
         * a primary activation first.
         */
        if((getCommandId(channel) == VG_AT_GP_CGACT)
#if defined (FEA_DEDICATED_BEARER)
            && (activePsdContext_p->secondaryContextCidPendingActivation == CID_NUMBER_UNKNOWN)
#endif
            )
        {
          setResultCode(channel, RESULT_CODE_OK);
        }

        gprsContext_p = ptrToGprsContext (channel);
        
#if defined (ATCI_SLIM_DISABLE)
        FatalCheck (gprsContext_p != PNULL, cid, channel, 0);

        /* If DBM counting is enabled for this context, kick off reports */
        if (activePsdContext_p->countReportType == START_PERIODIC)
        {
          gprsContext_p->vgCounterCid = cid;
          setResultCode (channel, vgChManContinueAction (channel, SIG_APEX_ABPD_REPORT_COUNTER_REQ));
        }
        else
        {
          if (activePsdContext_p->countReportType == ON_CONTEXT_DEACTIVATION)
          {
            gprsContext_p->vgCounterCid = cid;
            setResultCode (channel, vgChManContinueAction (channel,
                                                      SIG_APEX_ABPD_REPORT_COUNTER_REQ));
          }
        }
#endif
        connectionOk = TRUE;
      }
      else
      {
        /* We didn't expect the signal */
        FatalParam (channel, sig_p->connId, cid);
      }
    }
    else
    {
      /* We didn't expect the signal */
      FatalParam (channel, sig_p->connId, cid);
    }
  }

#if defined (FEA_MT_PDN_ACT)
  else if (sig_p->abpdCause == ABPD_CAUSE_NW_ACTION_OK)
  {
    /* Network initiated action.  Must be secondary context. */

    connectionOk = FALSE;  /* Will set to TRUE within the following checks */

#if defined (FEA_DEDICATED_BEARER)
    /* This can only happen for a secondary context and the primary context must be active */
    if ((sig_p->secondaryContext)
#if !defined (ENABLE_ATCI_UNIT_TEST)
         && (vgIsCurrentAccessTechnologyLte() == TRUE)
#endif
       )
    {
      primaryCid = vgFindCidLinkedToConnId(sig_p->primaryConnId);

      /* Get the channel we should be on for this MT connection */
      if (primaryCid != CID_NUMBER_UNKNOWN)
      {
        channel = vgFindEntityLinkedToCid(primaryCid);
      }

      /* Channel OK, the primary CID is active and not a secondary context? */
      if ((channel != VGMUX_CHANNEL_INVALID) && (primaryCid >= vgGpGetMinCidValue(channel)) &&
          (primaryCid < MAX_NUMBER_OF_CIDS) &&
          (gprsGenericContext_p->cidUserData[primaryCid] != PNULL) &&
          (gprsGenericContext_p->cidUserData[primaryCid]->isActive) &&
          (!gprsGenericContext_p->cidUserData[primaryCid]->psdBearerInfo.secondaryContext))
      {
        /* get an unused cid to use.  Note this is different to vgGetFreeCid used
         * For 2G/3G legacy MT PDP context activations.
         */
        /* For NB-IOT we need to check if we are allowed to activate default
         * PDN connection on attach - before doing anything (set by AT+CIPCA)
         */
        if ((getProfileValue(channel, PROF_MLTEGCF) != GCF_TEST_MODE_DISABLE) && vgCIPCAPermitsActivateAttachDefBearer())
        {
          psdBearerId = sig_p->psdBearerId;
        }

        if (vgGetUnusedCid (channel, psdBearerId, &cid) == TRUE)
        {
          /* We have allocated a new CID */
          if (vgAllocateRamToCid (cid))
          {
            gprsGenericContext_p->cidOwner[cid] = channel;

            activePsdContext_p = gprsGenericContext_p->cidUserData[cid];

            if (activePsdContext_p != PNULL)
            {
              connectionOk = TRUE;

              /* Make sure we set the fact that the profile is defined */
              activePsdContext_p->profileDefined = TRUE;
              activePsdContext_p->cgdscontDefined = TRUE;

              /* Indicate this was activated by the network */
              activePsdContext_p->mtActivatedDedicatedBearer = TRUE;

              /* Assign values received in the signal to the new CID */
              activePsdContext_p->cid = cid;
              activePsdContext_p->psdBearerInfo.channelNumber = channel;

              /* Always ABPD_CONN_TYPE_NONE for LTE dedicated bearers */
              activePsdContext_p->psdBearerInfo.connType = ABPD_CONN_TYPE_NONE;

              activePsdContext_p->psdBearerInfo.connId = sig_p->connId;
              activePsdContext_p->psdBearerInfo.secondaryContext = TRUE;
              activePsdContext_p->psdBearerInfo.primaryCid = primaryCid;
              activePsdContext_p->psdBearerInfo.primaryConnId = sig_p->primaryConnId;
              activePsdContext_p->psdBearerInfo.primaryPsdBearerId = gprsGenericContext_p->cidUserData[primaryCid]->psdBearerInfo.psdBearerId;

              activePsdContext_p->psdBearerInfo.flowControlType = sig_p->flowControlType;
            }
          }
          else
          {
            /* No extra room for MT CID - cannot happen - so assert */
            FatalParam(channel,psdBearerId, cid);
          }
        }
      }
    }
    else if ((sig_p->secondaryContext == FALSE) && (vgIsCurrentAccessTechnologyLte() == FALSE))
#else /* FEA_DEDICATED_BEARER */
    if (vgIsCurrentAccessTechnologyLte() == FALSE)
#endif /* FEA_DEDICATED_BEARER */
    {

        /* TODO: Code here won't be executed - can be deleted for NB-IOT
         */
        cid = vgFindCidLinkedToConnId(sig_p->connId);
        channel = vgFindEntityLinkedToCid(cid);
        if ((channel != VGMUX_CHANNEL_INVALID) && (cid >= vgGpGetMinCidValue(channel)) &&
            (cid < MAX_NUMBER_OF_CIDS) &&
            (gprsGenericContext_p->cidUserData[cid] != PNULL))
        {
          activePsdContext_p = gprsGenericContext_p->cidUserData[cid];

          connectionOk = TRUE;

          /* Make sure we set the fact that the profile is defined */
          activePsdContext_p->profileDefined = TRUE;
          activePsdContext_p->cgdcontDefined = TRUE;

          activePsdContext_p->pendingUnsolicitedContextActivation = FALSE;

          /* Assign values received in the signal to the new CID */
          activePsdContext_p->cid                           = cid;
          activePsdContext_p->psdBearerInfo.channelNumber   = channel;
          activePsdContext_p->psdBearerInfo.flowControlType = sig_p->flowControlType;

          /* Check below for ABPD_CONN_TYPE_PACKET_TRANSPORT should really be for
           * vgDoesEntityHaveSeparateDataChannel(channel) - but that breaks the unit tests
           * and this code will all be deleted soon anyway
           */

          if ((activePsdContext_p->psdBearerInfo.connType == ABPD_CONN_TYPE_NONE) ||
              (activePsdContext_p->psdBearerInfo.connType == ABPD_CONN_TYPE_PACKET_TRANSPORT))
          {
            setResultCode (channel, RESULT_CODE_OK);
          }
        }
    }
  }
#endif /* FEA_MT_PDN_ACT */

  else
  {
    /* We don't expect any other cause values for this signal */
    FatalParam(sig_p->abpdCause, sig_p->connId, sig_p->psdBearerId);
  }

  if ((activePsdContext_p != PNULL) && (connectionOk == TRUE))
  {
    /* Perform common actions for MO and MT activations */

    /* Now copy all the information from the ConnectedInd signal into
     * the psdContext structure
     */
    activePsdContext_p->psdBearerInfo.psdBearerId = sig_p->psdBearerId;
    activePsdContext_p->psdBearerInfo.modifiedBySim = sig_p->modifiedBySim;
    activePsdContext_p->psdBearerInfo.simMods = sig_p->simMods;

    /* The APN may have been modified by the SIM - so copy it back in to the structure */
    activePsdContext_p->psdBearerInfo.negApnPresent = sig_p->apnPresent;
    /* If APN is length 0 set APN present to FALSE */
    if (activePsdContext_p->psdBearerInfo.negApnPresent && (sig_p->apn.length == 0))
    {
      activePsdContext_p->psdBearerInfo.negApnPresent = FALSE;
    }
    /* APN has to be converted from that used by the GPRS network e.g. of
     * type 2co6cellco2co2uk to a type like "co.cellco.co.uk" for
     * displaying with the +CGDCONT, or +CGDCONTRDP AT command */
    if (activePsdContext_p->psdBearerInfo.negApnPresent == TRUE)
    {
      activePsdContext_p->psdBearerInfo.negApn = sig_p->apn;
      vgNetworkAPNToCiAPN (activePsdContext_p->psdBearerInfo.negApn, &convertedAPN);
      memcpy (&activePsdContext_p->psdBearerInfo.negTextualApn,
              &convertedAPN, sizeof (AccessPointName));
    }

    /* PdpAddress has to be converted from type 0000 to type
     * 000.000.000.000 or FF FF FF FF to 255.255.255.255 etc for
     * displaying with +CGPADDR; however, we do this when we process
     * CGPADDR, not here where we just save it in the binary form. */
    activePsdContext_p->psdBearerInfo.pdnConnectionAddressInfo = sig_p->pdnConnectionAddressInfo;

#if defined (FEA_QOS_TFT)
    activePsdContext_p->psdBearerInfo.negQosPresent = sig_p->qosPresent;

    if (sig_p->qosPresent == TRUE)
    {
      activePsdContext_p->psdBearerInfo.negotiatedQos = sig_p->qos;
    }
    else
    {
      memset (&activePsdContext_p->psdBearerInfo.negotiatedQos, 0, sizeof(QualityOfService));
    }

    activePsdContext_p->psdBearerInfo.negTftPresent = sig_p->tftPresent;
    if (sig_p->tftPresent == TRUE)
    {
      activePsdContext_p->psdBearerInfo.negotiatedTft = sig_p->tft;
    }
    else
    {
      /* Make sure all the data is cleared - just to be sure in case the
       * network cleared the TFT we just set
       */
      memset (&activePsdContext_p->psdBearerInfo.negotiatedTft, 0, sizeof(TrafficFlowTemplate));
    }
#endif /* FEA_QOS_TFT */

    activePsdContext_p->psdBearerInfo.apnAmbrPresent = sig_p->apnAmbrPresent;
    if (sig_p->apnAmbrPresent == TRUE)
    {
      activePsdContext_p->psdBearerInfo.apnAmbr = sig_p->apnAmbr;
    }

    /* Reset pending flag if it was set before */
    activePsdContext_p->pendingContextActivation = FALSE;

    /* Mark context as active and record nsapi */
    activePsdContext_p->isActive = TRUE;

    /* For LTE - if this CID was pending connection to the default bearer
     * activated during the attach - we now need to move this CID to a
     * CID with the same value as the PSD bearer ID (if possible).
     * ..and update the gprsGenericContext settings
     */
    if ((gprsGenericContext_p->lteAttachDefaultBearerCid == cid) &&
        (gprsGenericContext_p->vgLteAttachDefaultBearerState == LTE_ATTACH_DEFAULT_BEARER_PENDING_CONNECTION))
    {
      /* If the CID is already the same as the PSD Bearer ID then we don't need to
       * copy this CID data to a new one.
       */

      /* Only do this for LTE GCF test mode - NOT CID 0 */

      /* For NB-IOT we need to check if we are allowed to activate default
       * PDN connection on attach - before doing anything (set by AT+CIPCA)
       */
      if ((getProfileValue(channel, PROF_MLTEGCF) == GCF_TEST_MODE_ENABLE) &&
           vgCIPCAPermitsActivateAttachDefBearer() &&
           (cid != sig_p->psdBearerId) &&
           (vgGetUnusedCid (channel, sig_p->psdBearerId, &newCid) == TRUE))
      {
        /* The CID is already defined? - but this might be OK - we may just need to
         * re-use it.  This can happen, for example when we change from
         * AT+CFUN=1->0->1
         */
        if (newCid != sig_p->psdBearerId)
        {
          if (vgOpManCidAvailable (sig_p->psdBearerId, channel) &&
              (!gprsGenericContext_p->cidUserData[sig_p->psdBearerId]->isActive))
          {
            newCid = sig_p->psdBearerId;
          }
        }

        if (newCid == sig_p->psdBearerId)
        {
          /* We got the cid we wanted - otherwise don't bother moving it
           */
          if (vgAllocateRamToCid (newCid))
          {

            newPsdContext_p = gprsGenericContext_p->cidUserData[newCid];

            if (newPsdContext_p != PNULL)
            {
              /* Make sure the CID data is initialised in case we are re-using
               * a CID that was already there
               */
              vgInitialiseCidData (newPsdContext_p, newCid);
              gprsGenericContext_p->cidOwner[newCid] = channel;

              *newPsdContext_p = *activePsdContext_p;

              newPsdContext_p->cid = newCid;

              /* Clear down the old CID now */
              vgUpdateContextActivationStatus (cid, channel);
              vgOpManMakeCidAvailable (cid, channel);

              /* Check cid is in range to keep lint happy */
              if (cid < MAX_NUMBER_OF_CIDS)
              {
                /* ..and now free up the cid */
                vgFreeRamForCid(cid);
              }

              gprsGenericContext_p->lteAttachDefaultBearerCid = newCid;

              /* Make this new CID the current CID for the operations
               * below.
               */
              activePsdContext_p = newPsdContext_p;
              cid = newCid;
            }
          }
          else
          {
            FatalParam(cid,newCid,channel);
          }
        }
      }

      gprsGenericContext_p->vgLteAttachDefaultBearerState = LTE_ATTACH_DEFAULT_BEARER_CONNECTED;
    }

    vgUpdateErepPdpContextData (sig_p,
                                cid,
                                channel);

    /* Don't generate result code for network activation or if this was a primary
     * context activated because of a secondary context activation.
     */
    if ((sig_p->abpdCause != ABPD_CAUSE_NW_ACTION_OK)
#if defined (FEA_DEDICATED_BEARER)
        && (activePsdContext_p->secondaryContextCidPendingActivation == CID_NUMBER_UNKNOWN)
#endif /* FEA_DEDICATED_BEARER */
        )
    {
      /* For connections not requiring a data connection we need to set the
       * result code at this point in order to complete the AT command.
       */
      if (activePsdContext_p->psdBearerInfo.connType == ABPD_CONN_TYPE_NONE)
      {
        setResultCode (channel, RESULT_CODE_OK);
      }
      /* For entities where the data channel is separate from the AT command channel
       * we need to send an OK back to the PC now to indicate further AT commands can be entered
       */
      else if (vgDoesEntityHaveSeparateDataChannel(channel))
      {
        setResultCode (channel, RESULT_CODE_OK);
      }
    }

    if (sig_p->abpdCause == ABPD_CAUSE_UE_ACTION_OK)

    {
        isUEInitiated = TRUE;
    }
    else if (sig_p->abpdCause == ABPD_CAUSE_NW_ACTION_OK)

    {
        isUEInitiated = FALSE;
    }

    if ((activePsdContext_p->psdBearerInfo.connType == ABPD_CONN_TYPE_PACKET_TRANSPORT) ||
        (activePsdContext_p->psdBearerInfo.connType == ABPD_CONN_TYPE_NONE))
    {
#if defined (FEA_DEDICATED_BEARER)
      if (TRUE == activePsdContext_p->psdBearerInfo.secondaryContext)
      {
        vgSendUnsSecActDeactData(isUEInitiated, cid, EREP_EVENT_INFORMATIONAL, TRUE);
      }
      else
#endif /* FEA_DEDICATED_BEARER */

      {
        vgSendUnsPrimActDeactData(isUEInitiated, cid, TRUE, sig_p->causePresent, sig_p->cause);
      }
      vgSendUnsMCGCONTSTAT(TRUE,
                           cid,
#if defined (FEA_DEDICATED_BEARER)
                           sig_p->secondaryContext,
#else
                           FALSE,
#endif
                           sig_p->abpdCause,
                           FALSE,
                           CAUSE_NOT_APPLICABLE);
    }


#if defined (FEA_DEDICATED_BEARER)
    /* Now this CID is activated - check if it was a primary connection activated
     * as a result of the user attempting a secondary context activation without
     * the primary having been activated first
     */
    if (activePsdContext_p->secondaryContextCidPendingActivation != CID_NUMBER_UNKNOWN)
    {
      /* Check that this is not a secondary context */
      FatalCheck (!activePsdContext_p->psdBearerInfo.secondaryContext,
                  cid,
                  sig_p->abpdCause,
                  activePsdContext_p->secondaryContextCidPendingActivation);

      secondaryContext_p = gprsGenericContext_p->cidUserData[activePsdContext_p->secondaryContextCidPendingActivation];
#if defined (ATCI_SLIM_DISABLE)

      FatalCheck (secondaryContext_p != PNULL,
                  cid,
                  sig_p->abpdCause,
                  activePsdContext_p->secondaryContextCidPendingActivation);
#endif
      /* Initialise the secondary data now we have it from the primary */
      secondaryContext_p->psdBearerInfo.primaryConnId = secondaryContext_p->psdBearerInfo.connId;
      secondaryContext_p->psdBearerInfo.primaryPsdBearerId = secondaryContext_p->psdBearerInfo.psdBearerId;
      /* May need to copy additional parameters for secondary context ..
       * connId, etc.  Currently no more data needed to be copied.
       */

      /* Now activate the secondary CID. Must have been CONN_TYPE_NONE to get here
       */
      vgActivateContext (activePsdContext_p->secondaryContextCidPendingActivation,
                         channel,
                         ABPD_CONN_TYPE_NONE);

      activePsdContext_p->secondaryContextCidPendingActivation = CID_NUMBER_UNKNOWN;
    }
#endif /* FEA_DEDICATED_BEARER */
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdPsdBearerStatusInd
 *
 * Parameters:  signalBuffer - the signal sent from ABPD
 *              entity       - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Handles the SIG_APEX_ABPD_PSD_BEARER_STATUS_IND signal received
 *              from AB task ABPD module.
 *-------------------------------------------------------------------------*/
void vgApexAbpdPsdBearerStatusInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{

  ApexAbpdPsdBearerStatusInd   *sig_p                = &signalBuffer->sig->apexAbpdPsdBearerStatusInd;
  GprsGenericContext_t         *gprsGenericContext_p = ptrToGprsGenericContext ();
  GprsContext_t                *gprsContext_p;
  SleepManContext_t            *sleepManContext_p    = ptrToSleepManContext();
  VgPsdStatusInfo              *activePsdContext_p   = PNULL;
  Int8                         cid                   = CID_NUMBER_UNKNOWN;
  VgmuxChannelNumber           channel               = VGMUX_CHANNEL_INVALID;
  TextualAccessPointName       convertedAPN;
  ConnectionClass_t            connectionClass       = PT_CONNECTION;
  OpmanContext_t               *opManContext_p;

#if !defined(ATCI_ENABLE_DYN_AT_BUFF)
  /* Ignore the entity */
  PARAMETER_NOT_USED (entity);
#endif

  /********************************************************/
  /* For each bearer, we tick off when we receive this signal
   * When all recevied then we can say we are woken up, assuming
   * we got the NETWORK_STATE_IND and the SIM_OK_IND
   */

  /* TBD: Currently we do not copy TFT or QoS information in to
   * Requested QoS or set that the TFT and QoS commands were used.
   * This is unlikely to be needed for wake-up
   */

  /* For this function to work, on wake-up the ATCI Should have:
   * 1. Restored the PSD bearer pointers for all previously active
   *    PSD bearers.
   * 2. Set the connId in the data structure so we can look up the CID
   *    in this function
   * NOTE: All non-active PSD bearer information will be lost after
   *       wake-up.  TBD: Is this an issue - do we need to save
   *       bearer info that was set but not activated.
   */


  if (sig_p->abpdCause == ABPD_CAUSE_UE_ACTION_OK)
  {
    cid = vgFindCidLinkedToConnId(sig_p->connId);

    if (cid != CID_NUMBER_UNKNOWN)
    {
      channel = vgFindEntityLinkedToCid(cid);
    }

    if ((cid < MAX_NUMBER_OF_CIDS) && (channel != VGMUX_CHANNEL_INVALID))
    {

      activePsdContext_p = gprsGenericContext_p->cidUserData[cid];

      FatalAssert (activePsdContext_p != PNULL);

      activePsdContext_p->psdBearerInfo.connType = sig_p->connType;

      activePsdContext_p->psdBearerInfo.psdBearerId = sig_p->psdBearerId;
      activePsdContext_p->psdBearerInfo.modifiedBySim = sig_p->modifiedBySim;
      activePsdContext_p->psdBearerInfo.simMods = sig_p->simMods;

      activePsdContext_p->psdBearerInfo.negApnPresent = sig_p->apnPresent;

      if (activePsdContext_p->psdBearerInfo.negApnPresent == TRUE)
      {
        activePsdContext_p->psdBearerInfo.negApn = sig_p->apn;
        vgNetworkAPNToCiAPN (activePsdContext_p->psdBearerInfo.negApn, &convertedAPN);
        memcpy (&activePsdContext_p->psdBearerInfo.negTextualApn,
                &convertedAPN, sizeof (AccessPointName));
      }

#if defined (FEA_DEDICATED_BEARER)
      activePsdContext_p->psdBearerInfo.secondaryContext = sig_p->secondaryContext;

      if (activePsdContext_p->psdBearerInfo.secondaryContext)
      {
        activePsdContext_p->psdBearerInfo.primaryConnId = sig_p->primaryConnId;
        activePsdContext_p->psdBearerInfo.primaryCid = vgFindCidLinkedToConnId(sig_p->primaryConnId);
      }
#endif /* FEA_DEDICATED_BEARER */

      activePsdContext_p->psdBearerInfo.flowControlType = sig_p->flowControlType;

      activePsdContext_p->psdBearerInfo.pdnConnectionAddressInfo = sig_p->pdnConnectionAddressInfo;

#if defined (FEA_QOS_TFT)
      activePsdContext_p->psdBearerInfo.negQosPresent = sig_p->qosPresent;

      if (sig_p->qosPresent == TRUE)
      {
        activePsdContext_p->psdBearerInfo.negotiatedQos = sig_p->qos;
      }
      else
      {
        memset (&activePsdContext_p->psdBearerInfo.negotiatedQos, 0, sizeof(QualityOfService));
      }

      activePsdContext_p->psdBearerInfo.negTftPresent = sig_p->tftPresent;
      if (sig_p->tftPresent == TRUE)
      {
        activePsdContext_p->psdBearerInfo.negotiatedTft = sig_p->tft;
      }
      else
      {
        /* Make sure all the data is cleared - just to be sure in case the
         * network cleared the TFT we just set
         */
        memset (&activePsdContext_p->psdBearerInfo.negotiatedTft, 0, sizeof(TrafficFlowTemplate));
      }
#endif /* FEA_QOS_TFT */

      activePsdContext_p->psdBearerInfo.apnAmbrPresent = sig_p->apnAmbrPresent;
      if (sig_p->apnAmbrPresent == TRUE)
      {
        activePsdContext_p->psdBearerInfo.apnAmbr = sig_p->apnAmbr;
      }

      /* Mark context as active and record nsapi */
      activePsdContext_p->isActive = TRUE;

      /*** Now store PLMN and APN Rate Control Values ***/
      gprsGenericContext_p->plmnRateControlInfo.plmnRateControlPresent = sig_p->plmnRateControlPresent;

      if (sig_p->plmnRateControlPresent)
      {
        gprsGenericContext_p->plmnRateControlInfo.plmnRateControlValue = sig_p->plmnRateControlValue;
      }

      activePsdContext_p->apnUplinkRateControlInfo.apnRateControlPresent = sig_p->apnRateControlPresent;
      if (sig_p->apnRateControlPresent)
      {
        activePsdContext_p->apnUplinkRateControlInfo.apnRateControlAdditionalExceptionReportsAllowed =
                sig_p->apnRateControlAdditionalExceptionReportsAllowed;
        activePsdContext_p->apnUplinkRateControlInfo.apnRateControlMaxUplinkRate  = sig_p->apnRateControlMaxUplinkRate;
        activePsdContext_p->apnUplinkRateControlInfo.apnRateControluplinkTimeUnit = sig_p->apnRateControluplinkTimeUnit;
      }

      /* Set MTU Sizes if present */
      activePsdContext_p->psdBearerInfo.ipv4MtuSizePresent = sig_p->ipv4MTUPresent;
      activePsdContext_p->psdBearerInfo.ipv4LinkMTU = sig_p->ipv4LinkMTU;
      activePsdContext_p->psdBearerInfo.nonIPMtuSizePresent = sig_p->nonIPMTUPresent;
      activePsdContext_p->psdBearerInfo.nonIPLinkMTU = sig_p->nonIPLinkMTU;

      /*****************************************************************************/
      /* For wake-up must also copy some of this data in to AT command "requested" */
      /* information                                                               */
      /*****************************************************************************/
      activePsdContext_p->profileDefined = TRUE;

#if defined (FEA_DEDICATED_BEARER)
      /* Set either primary or secondary information */
      if (activePsdContext_p->psdBearerInfo.secondaryContext)
      {
        activePsdContext_p->cgdscontDefined = TRUE;
        activePsdContext_p->psdBearerInfo.primaryPsdBearerId = gprsGenericContext_p->cidUserData[activePsdContext_p->psdBearerInfo.primaryCid]->psdBearerInfo.psdBearerId;
      }
      else
#endif /* FEA_DEDICATED_BEARER */
      {
        activePsdContext_p->cgdcontDefined = TRUE;

        activePsdContext_p->psdBearerInfo.reqPdnAddress.pdnType = activePsdContext_p->psdBearerInfo.pdnConnectionAddressInfo.pdnAddress.pdnType;

        /* TBD: Do we copy the IP address also in to the reqPdnAddress.
         * Currently assume not..
         */
        if (activePsdContext_p->psdBearerInfo.negApnPresent)
        {

          activePsdContext_p->psdBearerInfo.reqApnPresent = TRUE;
          activePsdContext_p->psdBearerInfo.reqApn = activePsdContext_p->psdBearerInfo.negApn;
          /* APN was converted earlier on in this function */
          memcpy (&activePsdContext_p->psdBearerInfo.reqTextualApn,
                  &convertedAPN, sizeof (AccessPointName));

        }
      }

      activePsdContext_p->psdBearerInfo.psdUser = sig_p->psdUser;

      if (gprsGenericContext_p->lteAttachDefaultBearerCid == cid)
      {
        gprsGenericContext_p->vgLteAttachDefaultBearerState = LTE_ATTACH_DEFAULT_BEARER_CONNECTED;
      }

      /* Update CGEREP info. */
      vgUpdateErepPdpContextDataFromPsdBearerStatusInd (sig_p,
                                                        cid,
                                                        channel);


      /* For PPP and PT we need to set the correct connection status */
      if (activePsdContext_p->psdBearerInfo.connType != ABPD_CONN_TYPE_NONE)
      {
        /* set connection type to correct value */
        switch (activePsdContext_p->psdBearerInfo.connType)
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
          default:
            /* We should not get here
             */
            FatalParam (activePsdContext_p->psdBearerInfo.connType, channel, cid);
            break;
        }

        gprsContext_p = ptrToGprsContext(channel);

        FatalAssert (gprsContext_p != PNULL);

        opManContext_p = ptrToOpManContext (channel);
#if defined (ATCI_SLIM_DISABLE)

        FatalCheck(opManContext_p != PNULL, channel, 0, 0);
#endif
        opManContext_p->currentCcontrolState = PSD_ONLY;
        opManContext_p->numberOfCallConnections += 1;
        opManContext_p->vgLastCallConnectionType = connectionClass;
        opManContext_p->callInfo.vgClass = connectionClass;
        opManContext_p->callInfo.vgState = CONNECTION_ON_LINE;
        opManContext_p->callInfo.vgIdent = MIN_PSD_USER_CALL_ID;

        /* set connection state to on line
         * user call id hard coded to MIN_PSD_USER_CALL_ID as only one PSD
         * connection with connection to PC allowed on one channel.
         * NOTE: This line is probably not needed as we set it in the lines
         * above.
         */
        vgOpManSetConnectionStatus (channel,
                                     MIN_PSD_USER_CALL_ID,
                                      CONNECTION_ON_LINE);

        /* Re-assign activePsdBearerContextWithDataConn for the channel */
        gprsContext_p->activePsdBearerContextWithDataConn = activePsdContext_p;
        setConnectionType (channel, connectionClass);
#if 0
        /* TBD: May need to send this after wake-up */
        sendResultCodeToCrm (RESULT_CODE_CONNECT, channel);
#endif
        /* reset connection type
         */
        setConnectionType (channel, CONNECTION_TERMINATOR);

        /* If entity was in running state (read from NVRAM in wake-up
         * then we need to restore the channel to locked out state
         * Assume AT commands was AT+CGDATA.  May have been ATD or ATO
         * but doesn't make any different.
         * TODO: Maybe store the current AT command for the channel to get this
         * exact if needed.
         */
        if (getEntityState(channel) == ENTITY_RUNNING)
        {
          /* In order to have been in this state we must have been in the middle
           * of running an AT command that puts us in data mode - so set to
           * AT+CGDATA
           */
          setCommandId (channel, VG_AT_GP_CGDATA);

#if defined(ATCI_ENABLE_DYN_AT_BUFF)
          vgAllocAtCmdBuffer(entity);// reassign a buffer for the running AT command
#endif
          /* Lock up the channel now as it is in data mode */
          setResultCode (channel, RESULT_CODE_PROCEEDING);

#if defined (ENABLE_ATCI_UNIT_TEST)
          printf("ATCI: Setting channel %d to locked out state", channel);
#endif
        }
      }

      if (sleepManContext_p->atciInWakeupState)
      {
        FatalAssert(sleepManContext_p->numPsdBearerStatusIndsNeeded > 0);

        sleepManContext_p->numPsdBearerStatusIndsNeeded--;
        RvWakeUpCompleteCheck();
      }
      else
      {
        /* We shouldn't have even got this if we weren't waking up */
        FatalFail("ATCI: Unexpected PSD_BEARER_STATUS_IND");
      }
    }
    else
    {
      /* We didn't expect the signal */
      FatalParam (channel, sig_p->connId, cid);
    }
  }
  else
  {
    /* We don't expect any other cause values for this signal */
    FatalParam(sig_p->abpdCause, sig_p->connId, sig_p->psdBearerId);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdDisconnectedInd
 *
 * Parameters:  signalBuffer - the signal sent from ABGP
 *              entity       - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Handles the SIG_APEX_ABPD_DISCONNECTED_IND signal received
 *              from AB task ABPD module.
 *-------------------------------------------------------------------------*/
void vgApexAbpdDisconnectedInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  ApexAbpdDisconnectedInd *sig_p                 = &signalBuffer->sig->apexAbpdDisconnectedInd;
  GprsGenericContext_t    *gprsGenericContext_p  = ptrToGprsGenericContext ();
  OpmanGenericContext_t   *opManGenericContext_p = ptrToOpManGenericContext ();
  GprsContext_t           *gprsContext_p         = PNULL;
  VgPsdStatusInfo         *activePsdContext_p    = PNULL;
  Int8                    cid, tempCid;
  VgmuxChannelNumber      channel                = (VgmuxChannelNumber) sig_p->connId;
  ConnectionClass_t       connectionClass;
  Boolean                 psdBearerIdEntryFound  = FALSE;
  PdnAddress              pdnAddress;
  VgPsdStatusInfo         *tempPsdContext_p      = PNULL;
  Int8                    numActivePrimContexts  = 0;
  VgmuxChannelNumber      entityIndex;
  Boolean                 cgattOnOtherChannel    = FALSE;

  /* Check if any channel is attempting AT+CGATT=0 */
  for (entityIndex = 0; entityIndex < CI_MAX_ENTITIES; entityIndex++)
  {
    if(isEntityActive(entityIndex) && (getCommandId (entityIndex) == VG_AT_GP_CGATT))
    {
      cgattOnOtherChannel = TRUE;
    }
  }

#if defined (FEA_MT_PDN_ACT)

  /* If this was a pending MT PDP context for 2G/3G only (but we have
   * not yet answered it) then the CID will not be valid - so we just need to
   * send out the NO CARRIER indication on all active channels as we were sending
   * RING indication.
   */
  if ((gprsGenericContext_p->incomingPdpContextActivation) &&
      (gprsGenericContext_p->vgAbpdSetupInd.connId == sig_p->connId))
  {
    /* Note also this does not behave like this for LTE as the incoming
     * PDP context - is only seen when we get a connectedInd and it is only
     * for secondary context (dedicated bearer)
     */
    gprsGenericContext_p->incomingPdpContextActivation = FALSE;

    /* As any active channel was ringing - we now need to set all active
     * channels to stop ringing and generate NO CARRIER.
     */
    for (entityIndex = 0; entityIndex < CI_MAX_ENTITIES; entityIndex++)
    {
      if(isEntityActive(entityIndex) && (getEntityState(entityIndex) == ENTITY_IDLE)
         &&(!isEntityMmiNotUnsolicited(entityIndex)))
      {
        sendResultCodeToCrm (RESULT_CODE_NO_CARRIER, entityIndex);
      }
    }
  }
  else
#endif /* FEA_MT_PDN_ACT */
  {
    cid = vgFindCidLinkedToConnId(sig_p->connId);

    if (cid != CID_NUMBER_UNKNOWN)
    {
      channel = vgFindEntityLinkedToCid(cid);
    }

    if ((cid < MAX_NUMBER_OF_CIDS) && (channel != VGMUX_CHANNEL_INVALID))
    {
      activePsdContext_p = gprsGenericContext_p->cidUserData[cid];

      WarnAssert (activePsdContext_p != PNULL);

      if (activePsdContext_p != PNULL)
      {

#if defined (FEA_DEDICATED_BEARER)
        /* Check here if we have been asked to disconnect a primary context before
         * all its linked secondary contexts have been disconnected.  The stack should
         * have handled this situation so assert.
         */
        if (!activePsdContext_p->psdBearerInfo.secondaryContext)
        {
          tempCid = vgFindCidOfNextActiveSecondaryContextLinkedToPrimaryCid(cid);

          if (tempCid != CID_NUMBER_UNKNOWN)
          {
            FatalParam (entity, cid, tempCid);
          }
        }
#endif /* FEA_DEDICATED_BEARER */

        /* keep note of the disconnect cause */
        vgSetGsmCauseCallReleaseError (sig_p->cause, channel);

        /* if dialling then must have aborted connection or
         * we have been stuck in RESULT_CODE_PROCEEDING as the
         * channel has been in data state and the AT interface
         * was locked out.  Setting to RESULT_CODE_NO_CARRIER
         * will re-activate the AT interface
         */
        if ((getCommandId (channel) == VG_AT_GP_D) ||
                 (getCommandId (channel) == VG_AT_CC_O) ||
                 (getCommandId (channel) == VG_AT_CC_DL) ||
                 (getCommandId (channel) == VG_AT_GP_CGDATA)
#if defined (FEA_MT_PDN_ACT)
                 || (getCommandId (channel) == VG_AT_CC_A)
#endif /* FEA_MT_PDN_ACT */
                  )
        {
          setResultCode (channel, RESULT_CODE_NO_CARRIER);

          /* Special case - a different channel may have been trying to deactivate
           * the connection which is why one of the above commands
           * were in the middle of execution (i.e. it was
           * in data mode at this point
           *
           * - so we must make sure we send an OK on that channel
           */
          if ((gprsGenericContext_p->hangupChannel != VGMUX_CHANNEL_INVALID) &&
             (ptrToGprsContext(gprsGenericContext_p->hangupChannel) != PNULL) &&
             (ptrToGprsContext(gprsGenericContext_p->hangupChannel)->vgHangupCid == cid))
          {
            setResultCode (gprsGenericContext_p->hangupChannel, RESULT_CODE_OK);
          }
        }
#if defined (FEA_MT_PDN_ACT)
        else if (getCommandId (channel) == VG_AT_GP_CGANS)
        {
          /* Used to display NO CARRIER correctly when doing CGANS command
          *  activated by CGAUTO.
          */
          setTaskInitiated  (entity, FALSE);
          setResultCode (channel, RESULT_CODE_NO_CARRIER);
        }
#endif /* FEA_MT_PDN_ACT */
        else
        {
          if (sig_p->abpdCause == ABPD_CAUSE_UE_ACTION_OK)
          {
            if (getCommandId (channel) == VG_AT_CC_H)
            {
              setResultCode (channel, RESULT_CODE_OK);
            }
            /* else this might be in response to e.g. CGACT=0,x command
             * on this channel
             */
            else if(getCommandId (channel) == VG_AT_GP_CGACT)
            {
              if ((ptrToGprsContext(channel) != PNULL) &&
                  (ptrToGprsContext(channel)->vgHangupCid == cid))
              {
                setResultCode (channel, RESULT_CODE_OK);
              }
            }
            else if(getCommandId (channel) == VG_AT_GP_CGATT)
            {
              /* Detaching - ABPD may be informing us of PDN connections
               * being deactivated
               */
              /* Do nothing here */
            }
            else if ((opManGenericContext_p->channelState [channel].isDisconnecting == FALSE) &&
                     (cgattOnOtherChannel))
            {
              /* This channel was not attempted detach - there was no
               * AT command running - but another one
               * was and this channel is not being disabled
               * - if this is the case we need to send a NO_CARRIER
               */
              sendResultCodeToCrm (RESULT_CODE_NO_CARRIER, channel);
            }
            /* No command was running - but we initiated the disconnect so
             * we must have dropped the call from a CiMuxClosedDataConnInd or
             * from the channel being disabled.
             */
            else
            {
              if (opManGenericContext_p->channelState [channel].isDisconnecting == FALSE)
              {
                /* If the channel was not being disabled then we set the result code to OK.
                  */
                if (vgDoesEntityHaveSeparateDataChannel(channel))
                {
                  /* For entities where the AT channel is separate from the data channel in the MUX
                   * we need to send out "NO CARRIER" rather than OK as the AT channel separate from the data channel.
                   */
                  if(isEntityMmiNotUnsolicited(channel))
                  {
                    sendResultCodeToCrm (RESULT_CODE_NO_CARRIER, vgGetMmiUnsolicitedChannel());
                  }
                  else
                  {
                    sendResultCodeToCrm (RESULT_CODE_NO_CARRIER, channel);
                  }
                }
                else
                {
                  /* Special case - a different channel may have been trying to deactivate
                   * the connection which is why no command was running on the
                   * channel that owns the CID
                   * - so we must make sure we send an OK on that channel.
                   */
                  if ((gprsGenericContext_p->hangupChannel != VGMUX_CHANNEL_INVALID) &&
                     (ptrToGprsContext(gprsGenericContext_p->hangupChannel) != PNULL) &&
                     (ptrToGprsContext(gprsGenericContext_p->hangupChannel)->vgHangupCid == cid))
                  {
                    setResultCode (gprsGenericContext_p->hangupChannel, RESULT_CODE_OK);

                    /* No need to send OK on original channel because we have already done so
                     * on the hangup channel */

                    sendResultCodeToCrm (RESULT_CODE_NO_CARRIER, channel);
                  }
                  else
                  {
                    /* Send out the result code and then set it */
                    /* TODO: Only need to send result code as should already
                     * be set to OK.
                     */
                    sendResultCodeToCrm (RESULT_CODE_OK, channel);
                    setResultCode (channel, RESULT_CODE_OK);
                  }
                }

                /* Special case - a different channel may have been trying to deactivate
                 * the connection which is why no command was running on the
                 * channel that owns the CID
                 * - so we must make sure we send an OK on that channel
                 */
                if ((gprsGenericContext_p->hangupChannel != VGMUX_CHANNEL_INVALID) &&
                    (ptrToGprsContext(gprsGenericContext_p->hangupChannel) != PNULL) &&
                    (ptrToGprsContext(gprsGenericContext_p->hangupChannel)->vgHangupCid == cid))
                {
                  setResultCode (gprsGenericContext_p->hangupChannel, RESULT_CODE_OK);
                }
              }
            }
          }
          else if ((getCommandId (channel) == VG_AT_GP_CGACT) &&
                    (TRUE == activePsdContext_p->pendingContextDeactivation) &&
                    (ABPD_CAUSE_NW_ACTION_OK == sig_p->abpdCause))
          {
            /* a race condition between the UE deactivating and the network
               deactivating the connection  */
            setResultCode (channel, RESULT_CODE_NO_CARRIER);

            /* Also check hangup channel if different */
            if ((gprsGenericContext_p->hangupChannel != VGMUX_CHANNEL_INVALID) &&
                (ptrToGprsContext(gprsGenericContext_p->hangupChannel) != PNULL) &&
                (ptrToGprsContext(gprsGenericContext_p->hangupChannel)->vgHangupCid == cid))
            {
              setResultCode (gprsGenericContext_p->hangupChannel, RESULT_CODE_NO_CARRIER);
            }
          }
          else if ((getCommandId (channel) == VG_AT_GP_CGACT) &&
                    (TRUE == activePsdContext_p->pendingContextActivation) &&
                    (ABPD_CAUSE_UE_ACTION_FAIL == sig_p->abpdCause))
          {
            /* if dialling then must have aborted connection */
            setResultCode (channel, RESULT_CODE_NO_CARRIER);
          }
          else if ((getCommandId (channel) == VG_AT_GP_CGACT) &&
                    (TRUE == activePsdContext_p->pendingContextActivation) &&
                    (ABPD_CAUSE_DEDB_ACT_FAIL_NW_MOD_RESP == sig_p->abpdCause))
          {
            setResultCode (channel, RESULT_CODE_OK);
          }
          /* This may be an error - but we need to make sure that we are not
           * deactivating dedicated bearers as a result of an MO default bearer
           * deactivation - which is permitted
           */
          else if ((getCommandId (channel) == VG_AT_GP_CGACT) &&
                    (sig_p->abpdCause != ABPD_CAUSE_DED_B_DEACT_BY_DEF_B_DEACT) &&
                    (sig_p->abpdCause != ABPD_CAUSE_DED_B_DEACT_LOCAL) &&
                    (sig_p->abpdCause != ABPD_CAUSE_NW_ACTION_OK))
          {
            /* Not normal termination and CGACT: means e.g. CGACT=1,1 did not
             * succeed, return ERROR */
            setResultCode (channel, RESULT_CODE_ERROR);
          }

#if defined (FEA_QOS_TFT)
          else if ((getCommandId (channel) == VG_AT_GP_CGCMOD) &&
                    (TRUE == activePsdContext_p->pendingContextModification))
          {
            setResultCode (channel, RESULT_CODE_NO_CARRIER);
          }
#endif /* FEA_QOS_TFT */

          else
          {
            if(isEntityMmiNotUnsolicited(channel))
            {
              sendResultCodeToCrm (RESULT_CODE_NO_CARRIER, vgGetMmiUnsolicitedChannel());
            }
            else
            {
              sendResultCodeToCrm (RESULT_CODE_NO_CARRIER, channel);
            }
          }
        }
        switch (activePsdContext_p->psdBearerInfo.connType)
        {
#if defined (FEA_PPP)
          case ABPD_CONN_TYPE_PPP:
            connectionClass = PPP_CONNECTION;
            break;
          case ABPD_CONN_TYPE_CORE_PPP:
            connectionClass = PPP_CONNECTION;
            break;
#endif /* FEA_PPP */
          case ABPD_CONN_TYPE_PACKET_TRANSPORT:
            connectionClass = PT_CONNECTION;
            break;
          default:
            /*
             * For ABPD_CONN_TYPE_NONE set the connection class is
             * a special value for the MDRIND command
             */
            connectionClass = PSD_NONE_CONNECTION;
            break;
        }

         /* Fetch pdnAddress, cid
         * for unsolicited event reporting */

        psdBearerIdEntryFound = vgGetErepPdpContextData (
                                 activePsdContext_p->psdBearerInfo.psdBearerId,
                                  &pdnAddress,
                                   &tempCid);

        if (psdBearerIdEntryFound == TRUE)
        {
          /* Record that PDN connection is now inactive */
          vgUpdateContextActivationStatus (tempCid, channel);

#if defined (FEA_DEDICATED_BEARER)
          if (TRUE == activePsdContext_p->psdBearerInfo.secondaryContext)
          {
              vgSendUnsSecActDeactData(sig_p->isUEInitiated, tempCid, EREP_EVENT_INFORMATIONAL, FALSE);
          }
          else
#endif /* FEA_DEDICATED_BEARER */
          {
              vgSendUnsPrimActDeactData(sig_p->isUEInitiated, tempCid, FALSE, sig_p->causePresent, sig_p->cause);
          }
        }

        vgSendUnsMCGCONTSTAT(FALSE,
                             cid,
#if defined (FEA_DEDICATED_BEARER)
                             activePsdContext_p->psdBearerInfo.secondaryContext,
#else
                             FALSE,
#endif
                             sig_p->abpdCause,
                             sig_p->causePresent,
                             sig_p->cause);

        if (activePsdContext_p->psdBearerInfo.connType != ABPD_CONN_TYPE_NONE)
        {
          /* terminate the PSD data session and set the connection status
           * accordingly only for PT or PPP connections
           */
          terminateDataSession (channel, connectionClass);
          vgOpManSetConnectionStatus (channel,
                                      MIN_PSD_USER_CALL_ID,
                                      CONNECTION_OFF_LINE);
          vgOpManDropConnection (channel, connectionClass);

          /* Clear the link to the channel for the data connection now */
          gprsContext_p = ptrToGprsContext(channel);

          if ((gprsContext_p != PNULL) &&
              (gprsContext_p->activePsdBearerContextWithDataConn != PNULL))
          {
            gprsContext_p->activePsdBearerContextWithDataConn = PNULL;
          }
        }
        /* Clear any flags associated with the CID */
        activePsdContext_p->pendingContextActivation = FALSE;
        activePsdContext_p->pendingDataConnectionActivation = FALSE;

#if defined (FEA_QOS_TFT)
        activePsdContext_p->pendingContextModification = FALSE;
#endif

        activePsdContext_p->pendingContextDeactivation = FALSE;
        activePsdContext_p->pendingUnsolicitedContextActivation = FALSE;

#if defined (FEA_DEDICATED_BEARER)
        activePsdContext_p->secondaryContextCidPendingActivation = CID_NUMBER_UNKNOWN;
#endif /* FEA_DEDICATED_BEARER */

        /* Reset hangupCid */
        /* Clear the link to the channel for the data connection now */
        gprsContext_p = ptrToGprsContext(channel);

        if ((gprsContext_p != PNULL))
        { 
            if (gprsContext_p->vgHangupCid == cid)
            {
              gprsContext_p->vgHangupCid = CID_NUMBER_UNKNOWN;
            }
        }
        /* Clear down UL rate control flags */
        activePsdContext_p->apnUplinkRateControlInfo.apnRateControlPresent = FALSE;
        activePsdContext_p->apnUplinkRateControlInfo.apnRateControlAdditionalExceptionReportsAllowed = FALSE;
        activePsdContext_p->apnUplinkRateControlInfo.apnRateControlMaxUplinkRate = 0;
        activePsdContext_p->apnUplinkRateControlInfo.apnRateControluplinkTimeUnit = 0;

        if ((cid == gprsGenericContext_p->lteAttachDefaultBearerCid) &&
            ((gprsGenericContext_p->vgLteAttachDefaultBearerState == LTE_ATTACH_DEFAULT_BEARER_PENDING_CONNECTION) ||
            (gprsGenericContext_p->vgLteAttachDefaultBearerState == LTE_ATTACH_DEFAULT_BEARER_CONNECTED)))
        {
          /* We were trying to connect to the default bearer activated during the
           * attach - but something went wrong (maybe we detached for example
           * - so we will have to clear all the settings now and retry when we
           * attach again.
           * or the bearer got deactivated during detach...
           */
          gprsGenericContext_p->lteAttachDefaultBearerCid     = CID_NUMBER_UNKNOWN;
          gprsGenericContext_p->vgLteAttachDefaultBearerState = LTE_ATTACH_DEFAULT_BEARER_NOT_CONNECTED;

          /* Clear the CID so someone else can use it */
          vgInitialiseCidData (activePsdContext_p, cid);

        }
        else
        {
          /* We need to check if this was the last primary connection.  If so then
           * we need to clear down the lteAttachDefaultBearerCid.
           */
          for (tempCid = vgGpGetMinCidValue(channel); tempCid < MAX_NUMBER_OF_CIDS; tempCid++)
          {
            if (gprsGenericContext_p->cidUserData [tempCid] != PNULL)
            {
              tempPsdContext_p = gprsGenericContext_p->cidUserData [tempCid];

              /* if the profile is defined and active, and not secondary then
               * increment the counter
               */
              if ((tempPsdContext_p->profileDefined) && (tempPsdContext_p->isActive)
#if defined (FEA_DEDICATED_BEARER)
                  && (!tempPsdContext_p->psdBearerInfo.secondaryContext)
#endif /* FEA_DEDICATED_BEARER */
                  )

              {
                numActivePrimContexts++;
              }
            }
          }

          if (numActivePrimContexts == 0)
          {
            /* No primary connections left so we need to clear down the attach
             * default bearer settings at this point
             */
            gprsGenericContext_p->lteAttachDefaultBearerCid     = CID_NUMBER_UNKNOWN;
            gprsGenericContext_p->vgLteAttachDefaultBearerState = LTE_ATTACH_DEFAULT_BEARER_NOT_CONNECTED;
          }
        }

        /*
         * Free up the CID for someone else to use.
         */
        vgOpManMakeCidAvailable (cid, channel);

        /* Make sure we clear down some other crucial settings to allow someone else
         * to use the CID.
         */
        activePsdContext_p->psdBearerInfo.channelNumber = VGMUX_CHANNEL_INVALID;
        activePsdContext_p->psdBearerInfo.connType = ABPD_CONN_TYPE_NONE;
        activePsdContext_p->psdBearerInfo.connId = INVALID_CONN_ID;
        activePsdContext_p->psdBearerInfo.psdBearerId = PSD_BEARER_ID_UNASSIGNED;
        activePsdContext_p->psdBearerInfo.modifiedBySim = FALSE;
        activePsdContext_p->psdBearerInfo.simMods = 0;
        activePsdContext_p->psdBearerInfo.flowControlType = FC_NONE;


#if defined (FEA_MT_PDN_ACT)
        /* If the CID was activated by the network (MT dedicated bearer) - then
         * we need to clear down the CID
         */
        if (activePsdContext_p->mtActivatedDedicatedBearer)
        {
          vgInitialiseCidData (activePsdContext_p, cid);
        }
#endif

        if ((gprsGenericContext_p->hangupChannel != VGMUX_CHANNEL_INVALID) &&
            (ptrToGprsContext(gprsGenericContext_p->hangupChannel) != PNULL) &&
            (ptrToGprsContext(gprsGenericContext_p->hangupChannel)->vgHangupCid == cid))
        {
          /* reset the hangupCid for this channel too */
          ptrToGprsContext(gprsGenericContext_p->hangupChannel)->vgHangupCid = CID_NUMBER_UNKNOWN;

          gprsGenericContext_p->hangupChannel = VGMUX_CHANNEL_INVALID;
        }

        /* If we were disabling the channel then check if all channels are disabled
         * or if we need to diabled further contexts associated with this channel
         */
        vgCheckDisconnectAllProgress (channel);

      }
      else
      {
        FatalParam (sig_p->connId, cid, channel);
      }
    }
    else
    {
      FatalParam (sig_p->connId, cid, channel);
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdBusyInd
 *
 * Parameters:  signalBuffer - the signal sent from ABGP
 *              entity       - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Handles the SIG_APEX_ABPD_BUSY_IND signal received
 *              from AB task ABPD module.
 *-------------------------------------------------------------------------*/
void vgApexAbpdBusyInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  PARAMETER_NOT_USED (signalBuffer);
  PARAMETER_NOT_USED (entity);

  /*-----------------28/06/2012 15:53-----------------
   * This function  is not needed for NASMDL2
   * --------------------------------------------------*/
}

#if defined (FEA_QOS_TFT)
/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdPsdModifyCnf
 *
 * Parameters:  signalBuffer - the signal sent from ABGP
 *              entity       - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Handles the SIG_APEX_ABPD_PSD_MODIFY_CNF signal received
 *              from AB task ABPD module.
 *-------------------------------------------------------------------------*/
void vgApexAbpdPsdModifyCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{

  ApexAbpdPsdModifyCnf      *sig_p                  = &signalBuffer->sig->apexAbpdPsdModifyCnf;
  GprsGenericContext_t      *gprsGenericContext_p   = ptrToGprsGenericContext ();
  GprsContext_t             *gprsContext_p          = PNULL;
  VgPsdStatusInfo           *cmd_p                  = PNULL;
  VgPsdBearerInfo           *psdBearerInfo_p        = PNULL;
  Int8                      cid                     = CID_NUMBER_UNKNOWN;
  VgEREPModifyReason        modifyReason            = NUM_OF_EREP_MODIFY_REASON;
  VgmuxChannelNumber        channel                 = VGMUX_CHANNEL_INVALID;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsGenericContext_p != PNULL, entity, 0, 0);
#endif
  cid = vgFindCidLinkedToConnId(sig_p->connId);

  if (cid != CID_NUMBER_UNKNOWN)
  {
    cmd_p = gprsGenericContext_p->cidUserData [cid];
  }

  if ((cid < MAX_NUMBER_OF_CIDS) && (cmd_p != PNULL))
  {
    channel = cmd_p->vgModifyChannel;

    WarnCheck(channel != VGMUX_CHANNEL_INVALID, cid, 0, 0);

    gprsContext_p = ptrToGprsContext (channel);

    WarnCheck (gprsContext_p != PNULL, channel, 0, 0);

    if (gprsContext_p != PNULL)
    {
      WarnCheck(cid == gprsContext_p->vgModifyCid, channel, 0, 0);
    }

    psdBearerInfo_p = &cmd_p->psdBearerInfo;

    switch (getCommandId (channel))
    {
      case VG_AT_GP_CGCMOD:
      {
        setResultCode (channel, RESULT_CODE_OK);

        modifyReason = vgPdGetPsdModifyReason(
                       (psdBearerInfo_p->negQosPresent)?(&psdBearerInfo_p->negotiatedQos):(PNULL),
                       (sig_p->qosPresent)?(&sig_p->qos):(PNULL),
                       (psdBearerInfo_p->negTftPresent)?(&psdBearerInfo_p->negotiatedTft):(PNULL),
                       (sig_p->tftPresent)?(&sig_p->tft):(PNULL));

        if (TRUE == sig_p->qosPresent)
        {
          psdBearerInfo_p->negQosPresent  = TRUE;
          psdBearerInfo_p->negotiatedQos  = sig_p->qos;
        }
        else
        {
          psdBearerInfo_p->negQosPresent  = FALSE;
          memset (&psdBearerInfo_p->negotiatedQos, 0, sizeof(QualityOfService));
        }

        if (TRUE == sig_p->tftPresent)
        {
          psdBearerInfo_p->negTftPresent     = TRUE;
          psdBearerInfo_p->negotiatedTft        = sig_p->tft;
        }
        else
        {
          psdBearerInfo_p->negTftPresent      = FALSE;
          memset (&psdBearerInfo_p->negotiatedTft, 0, sizeof(TrafficFlowTemplate));
        }

        cmd_p->pendingContextModification = FALSE;

        if (modifyReason != NUM_OF_EREP_MODIFY_REASON)
        {
          vgSendUnsContextModifyData(TRUE, cid, modifyReason, EREP_EVENT_INFORMATIONAL);
        }
        vgSendUnsMCGCMOD(cid, ABPD_CAUSE_UE_ACTION_OK, FALSE, CAUSE_NOT_APPLICABLE);

        break;
      }

      default:
      {
        /* Illegal AT command running SIG_APEX_ABPD_PSD_MODIFY_CNF.
         * Just generate warning in case of crossover situation
         */
        WarnParam(channel, 0, 0);
        break;
      }
    }
  }
  else
  {
    FatalParam (sig_p->connId, cid, channel);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdPsdModifyRej
 *
 * Parameters:  signalBuffer - the signal sent from ABGP
 *              entity       - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Handles the SIG_APEX_ABPD_PSD_MODIFY_REJ signal received
 *              from AB task ABPD module.
 *-------------------------------------------------------------------------*/
void vgApexAbpdPsdModifyRej (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  ApexAbpdPsdModifyRej      *sig_p                  = &signalBuffer->sig->apexAbpdPsdModifyRej;
  GprsGenericContext_t      *gprsGenericContext_p   = ptrToGprsGenericContext ();
  GprsContext_t             *gprsContext_p          = PNULL;
  VgPsdStatusInfo           *cmd_p                  = PNULL;
  Int8                      cid                     = CID_NUMBER_UNKNOWN;
  VgmuxChannelNumber        channel                 = VGMUX_CHANNEL_INVALID;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsGenericContext_p != PNULL, entity, 0, 0);
#endif
  cid = vgFindCidLinkedToConnId(sig_p->connId);

  if (cid != CID_NUMBER_UNKNOWN)
  {
    cmd_p = gprsGenericContext_p->cidUserData [cid];
  }
  
  if ((cid < MAX_NUMBER_OF_CIDS) && (cmd_p != PNULL))
  {
    channel = cmd_p->vgModifyChannel;

    WarnCheck(channel != VGMUX_CHANNEL_INVALID, cid, 0, 0);

    gprsContext_p = ptrToGprsContext (channel);

    WarnCheck (gprsContext_p != PNULL, channel, 0, 0);

    if (gprsContext_p != PNULL)
    {
      WarnCheck(cid == gprsContext_p->vgModifyCid, channel, 0, 0);
    }

    switch (getCommandId (channel))
    {
      case VG_AT_GP_CGCMOD:
      {
        setResultCode (channel, RESULT_CODE_ERROR);
        cmd_p->pendingContextModification = FALSE;
        vgSendUnsMCGCMOD(cid,
                         sig_p->abpdCause,
                         sig_p->causePresent,
                         sig_p->cause);
        break;
      }

      default:
      {
        /* Illegal AT command running SIG_APEX_ABPD_PSD_MODIFY_CNF.
         * Just generate warning in case of crossover situation
         */
        WarnParam(channel, 0, 0);
        break;
      }
    }
  }
  else
  {
    FatalParam (sig_p->connId, cid, channel);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdPsdModifyInd
 *
 * Parameters:  signalBuffer - the signal sent from ABGP
 *              entity       - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Handles the SIG_APEX_ABPD_PSD_MODIFY_IND signal received
 *              from AB task ABPD module.
 *-------------------------------------------------------------------------*/
void vgApexAbpdPsdModifyInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  ApexAbpdPsdModifyInd      *sig_p                  = &signalBuffer->sig->apexAbpdPsdModifyInd;
  GprsGenericContext_t      *gprsGenericContext_p   = ptrToGprsGenericContext ();
  VgPsdStatusInfo           *cmd_p                  = PNULL;
  VgPsdBearerInfo           *psdBearerInfo_p        = PNULL;
  Int8                      cid                     = CID_NUMBER_UNKNOWN;
  VgEREPModifyReason        modifyReason            = NUM_OF_EREP_MODIFY_REASON;
  VgmuxChannelNumber        channel                 = VGMUX_CHANNEL_INVALID;

#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsGenericContext_p != PNULL, entity, 0, 0);
#endif
  cid = vgFindCidLinkedToConnId(sig_p->connId);

  if (cid != CID_NUMBER_UNKNOWN)
  {
    channel = vgFindEntityLinkedToCid(cid);
  }

  if ((cid < MAX_NUMBER_OF_CIDS) && (channel != VGMUX_CHANNEL_INVALID))
  {
    cmd_p = gprsGenericContext_p->cidUserData [cid];

    if (cmd_p != PNULL)
    {
      psdBearerInfo_p = &cmd_p->psdBearerInfo;

      modifyReason = vgPdGetPsdModifyReason(
                     (psdBearerInfo_p->negQosPresent)?(&psdBearerInfo_p->negotiatedQos):(PNULL),
                     (sig_p->qosPresent)?(&sig_p->qos):(PNULL),
                     (psdBearerInfo_p->negTftPresent)?(&psdBearerInfo_p->negotiatedTft):(PNULL),
                     (sig_p->tftPresent)?(&sig_p->tft):(PNULL));

      if (TRUE == sig_p->qosPresent)
      {
        psdBearerInfo_p->negQosPresent  = TRUE;
        psdBearerInfo_p->negotiatedQos  = sig_p->qos;
      }
      else
      {
        psdBearerInfo_p->negQosPresent  = FALSE;
        memset (&psdBearerInfo_p->negotiatedQos, 0, sizeof(QualityOfService));
      }

      if (TRUE == sig_p->tftPresent)
      {
        psdBearerInfo_p->negTftPresent     = TRUE;
        psdBearerInfo_p->negotiatedTft        = sig_p->tft;
      }
      else
      {
        psdBearerInfo_p->negTftPresent      = FALSE;
        memset (&psdBearerInfo_p->negotiatedTft, 0, sizeof(TrafficFlowTemplate));
      }

      if (modifyReason != NUM_OF_EREP_MODIFY_REASON)
      {
        vgSendUnsContextModifyData(FALSE, cid, modifyReason, EREP_EVENT_INFORMATIONAL);
      }

      vgSendUnsMCGCMOD(cid, ABPD_CAUSE_NW_ACTION_OK, FALSE, CAUSE_NOT_APPLICABLE);
    }
    else
    {
      /* Just generate warning in case of crossover situation
       */
      WarnParam(sig_p->connId, cid, channel);
    }
  }
  else
  {
    FatalParam (sig_p->connId, cid, channel);
  }
}
#endif /* FEA_QOS_TFT */

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdActivateDataConnCnf
 *
 * Parameters:  signalBuffer - the signal sent from ABGP
 *              entity       - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Handles the SIG_APEX_ABPD_ACTIVATE_DATA_CONN_CNF signal received
 *              from AB task ABPD module.
 *-------------------------------------------------------------------------*/
void vgApexAbpdActivateDataConnCnf (const SignalBuffer *signalBuffer,
                                   const VgmuxChannelNumber entity)
{
  ApexAbpdActivateDataConnCnf  *sig_p                  = &signalBuffer->sig->apexAbpdActivateDataConnCnf;
  GprsGenericContext_t         *gprsGenericContext_p   = ptrToGprsGenericContext ();
  GprsContext_t                *gprsContext_p          = PNULL;
  VgPsdStatusInfo              *activePsdContext_p     = PNULL;
  Int8                         cid                     = CID_NUMBER_UNKNOWN;
  VgmuxChannelNumber           channel                 = VGMUX_CHANNEL_INVALID;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsGenericContext_p != PNULL, entity, 0, 0);
#endif
  cid = vgFindCidLinkedToConnId(sig_p->connId);

  if (cid != CID_NUMBER_UNKNOWN)
  {
    channel = vgFindEntityLinkedToCid(cid);
  }

  if ((cid < MAX_NUMBER_OF_CIDS) && (channel != VGMUX_CHANNEL_INVALID))
  {

    gprsContext_p = ptrToGprsContext (channel);

    WarnCheck (gprsContext_p != PNULL, channel, 0, 0);

    if (gprsContext_p != PNULL)
    {
      WarnCheck (cid == gprsContext_p->vgDialCid, channel, 0, 0);
    }

    activePsdContext_p = gprsGenericContext_p->cidUserData [cid];

    if (activePsdContext_p != PNULL)
    {
      WarnCheck (activePsdContext_p->pendingDataConnectionActivation, channel, cid, 0);

      activePsdContext_p->pendingDataConnectionActivation = FALSE;

      /* Where AT channel is separate from data channel - we set the result code to OK
       * here.
       */
      if (vgDoesEntityHaveSeparateDataChannel(channel))
      {
        setResultCode (channel, RESULT_CODE_OK);
      }

      vgSendUnsMCGCONTSTAT(TRUE,
                           cid,
                           FALSE,
                           ABPD_CAUSE_UE_ACTION_OK,
                           FALSE,
                           CAUSE_NOT_APPLICABLE);

    }
  }
  else
  {
    FatalParam (sig_p->connId, cid, channel);
  }
}

#if defined (FEA_MT_PDN_ACT)
/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdSetupInd
 *
 * Parameters:  signalBuffer - the signal sent from ABGP
 *              entity       - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Handles the SIG_APEX_ABPD_SETUP_IND signal received
 *              from AB task ABPD module.  This is for MT PDP contexts
 *              and is only applicable to 2G/3G.
 *-------------------------------------------------------------------------*/
void vgApexAbpdSetupInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  ApexAbpdSetupInd      *sig_p                    = &signalBuffer->sig->apexAbpdSetupInd;
  GprsGenericContext_t  *gprsGenericContext_p     = ptrToGprsGenericContext ();

  /* Ignore incoming activation if we have one pending already. */
  if (FALSE == gprsGenericContext_p->incomingPdpContextActivation)
  {
    /* stash away the signal params for future reference */
    gprsGenericContext_p->vgAbpdSetupInd                = *sig_p;
    gprsGenericContext_p->incomingPdpContextActivation  = TRUE;
    /* Send RING to AT i-f. It will kick off the timer, to repeat RING.
       We handle auto-answer after the appropriate number (S1) of rings
       -- see vgPdRINGINGTimerExpiry. */
    vgPdStartRinging (entity);
  }
  else
  {
    /* Just ignore the signal completely.  It may be picked up by
       another fg entity, or it may time out at a layer below */
  }

}
#endif

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdApnReadCnf
 *
 * Parameters:  signalBuffer - the signal sent from ABGP
 *              entity       - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Handles the SIG_APEX_ABPD_APN_READ_CNF signal received
 *              from AB task ABPD module.
 *-------------------------------------------------------------------------*/
void vgApexAbpdApnReadCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  ApexAbpdApnReadCnf            *sig_p                  = &signalBuffer->sig->apexAbpdApnReadCnf;
  GprsGenericContext_t          *gprsGenericContext_p   = ptrToGprsGenericContext ();
  GeneralGenericContext_t       *generalGenericContext_p= ptrToGeneralGenericContext ();
  const SupportedPDNTypesMap    *supportedPDNTypesMap   = getSupportedPDNTypesMap();
  AbpdApn                       *apnDesc_p              = &(sig_p->apnDesc);
  Int8                          arrIndx;
  Boolean                       enumFound               = FALSE;
  ResultCode_t                  result                  = RESULT_CODE_OK;

  if (READY == generalGenericContext_p->initDataFromABPDState)
  {
    switch (getCommandId (entity))
    {
      case VG_AT_GP_MCGDEFCONT:
      {
        if (ABPD_REQ_OK == sig_p->requestStatus)
        {

          vgPutNewLine(entity);
          vgPrintf(entity, (const Char *)"*MCGDEFCONT: ");
          enumFound = vgPDNTypeToIndx (apnDesc_p->pdnType, &arrIndx);
          if (TRUE == enumFound)
          {
            vgPrintf (entity, (const Char*)"\"%s\"", supportedPDNTypesMap[arrIndx].str);
          }
          else
          {
            vgPrintf (entity, (const Char*)"\"UNKNOWN PDP TYPE\"");
          }
          if (apnDesc_p->apnPresent)
          {
            vgPrintf (entity, (const Char*)",\"%s\"", apnDesc_p->textualApn.name);
            if (apnDesc_p->psdUser.usernamePresent)
            {
              vgPrintf (entity, (const Char*)",\"%s\"", apnDesc_p->psdUser.username);
              if (apnDesc_p->psdUser.passwdPresent)
              {
                vgPrintf (entity, (const Char*)",\"%s\"", apnDesc_p->psdUser.passwd);
              }
            }
          }
          vgPutNewLine(entity);

          gprsGenericContext_p->currentDefaultAPN = sig_p->apnDesc;
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
        break;
      }

      default:
      {
        /* Illegal AT command running SIG_APEX_ABPD_APN_READ_CNF */
        FatalParam(entity, getCommandId (entity), 0);
//        result = RESULT_CODE_ERROR;
        break;
      }
    }
    setResultCode (entity, result);
  }
  else
  if (WAITING_FOR_DEFAULT_APN == generalGenericContext_p->initDataFromABPDState)
  {
    FatalCheck (sig_p->requestStatus == ABPD_REQ_OK, sig_p->requestStatus, entity, 0);

    generalGenericContext_p->initDataFromABPDState = READY;
    gprsGenericContext_p->currentDefaultAPN = sig_p->apnDesc;
    /* Check if we were waiting for this information before initiating a connection
     * to the default bearer activated during the attach
     */
    if (gprsGenericContext_p->vgLteAttachDefaultBearerState == LTE_ATTACH_DEFAULT_BEARER_PENDING_ABPD_DATA)
    {
      gprsGenericContext_p->vgLteAttachDefaultBearerState = LTE_ATTACH_DEFAULT_BEARER_NOT_CONNECTED;

       /* If CIPCA is set to NOT activate default PDN on attach & MLTEGCF is not set to GCF or CID 0 mode
        * - then it should never get here. But check in case.
        */
      if (vgIsCurrentAccessTechnologyLte() &&
          vgPsdAttached() &&
          vgCIPCAPermitsActivateAttachDefBearer() &&
          (getProfileValue (entity, PROF_MLTEGCF) != GCF_TEST_MODE_DISABLE))
      {
        /* Only do this if we are registered to LTE now - if we are not then
         * we need to wait until we are registered - which is handled in
         * rvmmsigi.c
         */
        vgActivateAttachDefBearerContext(entity);
      }
    }
  }
  else
  {
    FatalParam(generalGenericContext_p->initDataFromABPDState, 0, 0);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdApnWriteCnf
 *
 * Parameters:  signalBuffer - the signal sent from ABGP
 *              entity       - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Handles the SIG_APEX_ABPD_APN_WRITE_CNF signal received
 *              from AB task ABPD module.
 *-------------------------------------------------------------------------*/
void vgApexAbpdApnWriteCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  ApexAbpdApnWriteCnf     *sig_p                    = &signalBuffer->sig->apexAbpdApnWriteCnf;
  ResultCode_t             result                   = RESULT_CODE_OK;
  GprsGenericContext_t    *gprsGenericContext_p     = ptrToGprsGenericContext ();

  switch (getCommandId (entity))
  {
    case VG_AT_GP_MCGDEFCONT:
    {
      if (VG_AT_GP_MCGDEFCONT == getCommandId (entity))
      {
        if (ABPD_REQ_OK == sig_p->requestStatus)
        {
          gprsGenericContext_p->currentDefaultAPN = gprsGenericContext_p->definedDefaultAPN;
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
      }
      break;
    }

    default:
    {
      /* Illegal AT command running SIG_APEX_ABPD_APN_WRITE_CNF */
      FatalParam(entity, getCommandId (entity), 0);
//      result = RESULT_CODE_ERROR;
      break;
    }
  }

  setResultCode (entity, result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgApexAbpdReportCounterCnf
*
* Parameters:  SignalBuffer       - structure containing signal:
*                                   SIG_APEX_ABPD_REPORT_COUNTER_CNF
*              VgmuxChannelNumber - entity which sent request
*
* Returns:     nothing
*
* Description: This function is called when a SIG_APEX_ABPD_REPORT_COUNTER_CNF
*              signal has been received. The signal originates from DBM
*              and contains the cid and result. 
*-------------------------------------------------------------------------*/

void vgApexAbpdReportCounterCnf (const SignalBuffer *signalBuffer,
                            const VgmuxChannelNumber entity)
{
  ApexAbpdReportCounterCnf *sig_p = &signalBuffer->sig->apexAbpdReportCounterCnf;
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo      *psdStatusInfo_p;
  Int8                 index, cidCounter;
  CounterData          *countPtr = PNULL;
  ResultCode_t         result = RESULT_CODE_OK;

  switch (getCommandId (entity))
  {
    case VG_AT_GP_MGCOUNT:
    {
      if (TRUE == sig_p->isActive)
      {
        result = RESULT_CODE_OK;
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }

    default:
    {
      /* Illegal AT command running SIG_APEX_ABPD_REPORT_COUNTER_CNF */
      FatalParam(entity, getCommandId (entity), 0);
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  setResultCode (entity, result);
}


#if defined (FEA_UPDIR)
/*--------------------------------------------------------------------------
*
* Function:    vgApexAbpdSetUpdirInfoCnf
*
* Parameters:  SignalBuffer       - structure containing signal:
*                                   SIG_APEX_ABPD_SET_UPDIR_INFO_CNF
*              VgmuxChannelNumber - entity which sent request
*
* Returns:     nothing
*
* Description: This function is called when a SIG_APEX_ABPD_SET_UPDIR_INFO_CNF
*              signal has been received. The signal originates from DBM
*              and contains the patternId and result. 
*-------------------------------------------------------------------------*/

void vgApexAbpdSetUpdirInfoCnf (const SignalBuffer *signalBuffer,
                            const VgmuxChannelNumber entity)
{
  ApexAbpdSetUpdirInfoCnf *sig_p = &signalBuffer->sig->apexAbpdSetUpdirInfoCnf;
  ResultCode_t         result = RESULT_CODE_OK;

  switch (getCommandId (entity))
  {
    case VG_AT_GP_MUPDIR:
    {
      if (TRUE == sig_p->isSuccess)
      {
        setResultCode (entity, RESULT_CODE_OK);

        /* Report pattern id of *MUPDIR URC */
        vgPutNewLine (entity);
        
        vgPrintf (entity,
            (const Char *)"*MUPDIR: %d",
            sig_p->patternId);
        
        vgPutNewLine (entity);
      }
      else
      {
        setResultCode (entity, RESULT_CODE_ERROR);
      }
      break;
    }

    default:
    {
      /* Illegal AT command running SIG_APEX_ABPD_SET_UPDIR_INFO_CNF */
      FatalParam(entity, getCommandId (entity), 0);
      setResultCode (entity, RESULT_CODE_ERROR);
      break;
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdUpdirInd
 *
 * Parameters:  signalBuffer - the signal sent from ABPD
 *              entity       - mux channel number
 *
 * Returns:     nothing
 *
 * Description: Handles the SIG_APEX_ABPD_UPDIR_IND signal received
 *              from AB task ABPD module.
 *-------------------------------------------------------------------------*/
  void vgApexAbpdUpdirInd (const SignalBuffer *signalBuffer,
                                 const VgmuxChannelNumber entity)
{
  ApexAbpdUpdirInd      *sig_p                  = &signalBuffer->sig->apexAbpdUpdirInd;
  GprsGenericContext_t  *gprsGenericContext_p   = ptrToGprsGenericContext ();
  VgmuxChannelNumber    channel                 = VGMUX_CHANNEL_INVALID;
  Int8                  index;

  channel = gprsGenericContext_p->vgMUPDIRData.vgMupdiEntity;

  if((isEntityActive(channel)) && (sig_p->msgIdNum != 0) && (sig_p->msgIdNum <= ABPD_MAX_NUM_UPDIR_PACKET_ID_REPORT))
  {
    vgPutNewLine (channel);
    vgPrintf (channel, (const Char *)"*MUPDI: %d,%d,%d",
              sig_p->patternId,
              sig_p->status,
              sig_p->msgIdNum);
    for(index = 0; index < sig_p->msgIdNum; index++)
    {
      vgPrintf (channel, (const Char *)",%u",
                sig_p->msgId[index]);
    }
    vgPutNewLine (channel);
    vgFlushBuffer (channel);
  }
}

#endif
#if defined (FEA_PPP)
/*************************************************************************
*
* Function:     vgApexAbpdPppConfigCnf
*
* Parameters:   signalBuffer - the signal sent from AB task ABPD module
*               entity       - mux channel number
*
* Returns:      nothing
*
* Description:  This signal is received following a ConfigReq being
*               sent to the AB task ABPD module.
*
*************************************************************************/

void vgApexAbpdPppConfigCnf (const SignalBuffer *signalBuffer,
                            const VgmuxChannelNumber entity)
{
  ApexAbpdPppConfigCnf  *sig_p = (ApexAbpdPppConfigCnf *) signalBuffer->sig;
  Int8            i;

  switch (getCommandId (entity))
  {
    case VG_AT_GP_MPPPCONFIG:
    {
      if (sig_p->success)
      {
        if (sig_p->updateMode)
        {
          setResultCode (entity, RESULT_CODE_OK);
        }
        else
        {
          /* Output the values received from PPP */
          vgPutNewLine (entity);
          vgPrintf (entity, (const Char*) "%C: ");
          vgPutNewLine (entity);

          for (i = 0; i < PPP_CFG_INDEX_NUM; i++)
          {
            vgPrintf (entity, (const Char*) "%d,%lu,%lu,%d",
                      i,
                      sig_p->pppTimerConfig[i].finalValue,
                      sig_p->pppTimerConfig[i].initValue,
                      sig_p->pppTimerConfig[i].tries);
            vgPutNewLine (entity);
          }

          setResultCode (entity, RESULT_CODE_OK);
        }
      }
      else
      {
        setResultCode (entity, RESULT_CODE_ERROR);
      }
      break;
    }
    default:
    {
      /* Illegal AT command running SIG_APEX_ABPD_PPP_CONFIG_CNF */
      FatalParam(entity, getCommandId (entity), 0);

      setResultCode (entity, RESULT_CODE_ERROR);
      break;
    }
  }
}

/*************************************************************************
*
* Function:     vgApexAbpdPppConfigAuthCnf
*
* Parameters:   signalBuffer - the signal sent from AB task ABPD module
*               entity       - mux channel number
*
* Returns:      nothing
*
* Description:  This signal is received following a ConfigAuthReq being
*               sent to the AB task ABPD module
*
*************************************************************************/

void vgApexAbpdPppConfigAuthCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  ApexAbpdPppConfigAuthCnf  *sig_p = (ApexAbpdPppConfigAuthCnf *) signalBuffer->sig;

  switch (getCommandId (entity))
  {
    case VG_AT_GP_MPPPCONFIGAUTH:
    {
      if (sig_p->success)
      {
        if (sig_p->updateMode)
        {
          setResultCode (entity, RESULT_CODE_OK);
        }
        else
        {
          /* Output the values received from PPP */
          vgPutNewLine (entity);
          vgPrintf (entity, (const Char*) "%C: %u", (unsigned) sig_p->pppAuthType);
          vgPutNewLine (entity);

          setResultCode (entity, RESULT_CODE_OK);
        }
      }
      else
      {
        setResultCode (entity, RESULT_CODE_ERROR);
      }
      break;
    }
    default:
    {
      /* Illegal AT command running SIG_APEX_ABPD_PPP_CONFIG_AUTH_CNF */
      FatalParam(entity, getCommandId (entity), 0);

      setResultCode (entity, RESULT_CODE_ERROR);
      break;
    }
  }
}


/*************************************************************************
*
* Function:     vgApexAbpdPppLoopbackCnf
*
* Parameters:   signalBuffer - the signal sent from AB ABPD module
*               entity       - mux channel number
*
* Returns:      nothing
*
* Description:  This signal is received following a LoopbackReq being
*               sent to the ABPD task.
*
*************************************************************************/

void vgApexAbpdPppLoopbackCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity)
{
  ApexAbpdPppLoopbackCnf  *sig_p = &signalBuffer->sig->apexAbpdPppLoopbackCnf;
  GprsContext_t           *gprsContext_p = ptrToGprsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
#endif
  switch (getCommandId (entity))
  {
    case VG_AT_GP_MLOOPPSD:
    {
      if (sig_p->success == TRUE)
      {
        switch (gprsContext_p->vgPppLoopbackState)
        {
          case VG_LOOPBACK_ENABLE_PENDING:
          {
            /* Make sure the operations manager knows this channel is now
             * online
             */
            vgOpManSetConnectionStatus (entity,
                                        MIN_PSD_USER_CALL_ID,
                                        CONNECTION_ON_LINE);

            sendResultCodeToCrm (RESULT_CODE_CONNECT, entity);
            vgCiStartTimer (TIMER_TYPE_PSD_LOOPBACK, entity);
            break;
          }
          case VG_LOOPBACK_DISABLE_PENDING:
          {
            gprsContext_p->vgPppLoopbackState = VG_LOOPBACK_DISABLED;
            setResultCode (entity, RESULT_CODE_OK);
            vgCiMuxCloseDataConnection (entity);
            break;
          }
          default:
          {
            FatalParam (entity, gprsContext_p->vgPppLoopbackState, 0);
//            setResultCode (entity, RESULT_CODE_ERROR);
            break;
          }
        }
      }
      else
      {
        switch (gprsContext_p->vgPppLoopbackState)
        {
          case VG_LOOPBACK_ENABLE_PENDING:
          {
            gprsContext_p->vgPppLoopbackState = VG_LOOPBACK_DISABLED;
            break;
          }
          case VG_LOOPBACK_DISABLE_PENDING:
          {
            gprsContext_p->vgPppLoopbackState = VG_LOOPBACK_ENABLED;
            break;
          }
          default:
          {
            FatalParam (entity, gprsContext_p->vgPppLoopbackState, 0);
            break;
          }
        }
        setResultCode (entity, RESULT_CODE_ERROR);
      }
      break;
    }
    default:
    {
      /* Illegal AT command running SIG_CI_PPP_LOOPBACK_CNF */
      FatalParam(entity, getCommandId (entity), 0);

  //    setResultCode (entity, RESULT_CODE_ERROR);
      break;
    }
  }
}

/*************************************************************************
*
* Function:     vgPdLoopbackTimerExpiry
*
* Parameters:   entity - mux channel number
*
* Returns:      nothing
*
* Description:  Handles PSD loopback timer expiry, to change into data mode.
*
*************************************************************************/
void vgPdLoopbackTimerExpiry (const VgmuxChannelNumber entity)
{
  GprsContext_t        *gprsContext_p = ptrToGprsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (gprsContext_p != PNULL, entity, 0, 0);
#endif
  switch (getCommandId (entity))
  {
    case VG_AT_GP_MLOOPPSD:
    {
      switch (gprsContext_p->vgPppLoopbackState)
      {
        case VG_LOOPBACK_ENABLE_PENDING:
        {
          gprsContext_p->vgPppLoopbackState = VG_LOOPBACK_ENABLED;
          setResultCode (entity, RESULT_CODE_PROCEEDING);
          vgCiMuxOpenDataConnection (entity, PSD_PPP);

          /* once a connect has been displayed the command is no longer considered
           * task initiated to allow further result codes to be displayed */
          setTaskInitiated  (entity, FALSE);
          break;
        }
        default:
        {
          FatalParam (entity, gprsContext_p->vgPppLoopbackState, 0);
//          setResultCode (entity, RESULT_CODE_ERROR);
          break;
        }
      }
      break;
    }
    default:
    {
      /* Illegal AT command running GPRS loopback timer */
      FatalParam (entity,
                  getCommandId (entity),
                  gprsContext_p->vgPppLoopbackState);
//      setResultCode (entity, RESULT_CODE_ERROR);
      break;
    }
  }
}
#endif /* FEA_PPP */
#if defined (FEA_ACL)

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdSetAclCnf
 *
 * Parameters:  (In) signalBuffer - a pointer to the SignalBuffer which
 *                                  will contain a ApexAbpdSetAclCnf
 *                                  structure.
 *              (In) entity       - entity which is handling this signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_ABPD_SET_ACL_CNF signal
 *-------------------------------------------------------------------------*/

void vgApexAbpdSetAclCnf (const SignalBuffer *signalBuffer,
                          const VgmuxChannelNumber entity)
{
  ApexAbpdSetAclCnf *sig_p = &signalBuffer->sig->apexAbpdSetAclCnf;
  ResultCode_t      result = translateApnStatusToResult(sig_p->requestStatus);

  /* Set result code.... */
  setResultCode (entity, result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdListAclCnf
 *
 * Parameters:  (In) signalBuffer - a pointer to the SignalBuffer which
 *                                  will contain a ApexAbpdListAclCnf
 *                                  structure.
 *              (In) entity       - entity which is handling this signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_ABPD_LIST_ACL_CNF signal
 *-------------------------------------------------------------------------*/

void vgApexAbpdListAclCnf (const SignalBuffer *signalBuffer,
                           const VgmuxChannelNumber entity)
{
  ApexAbpdListAclCnf *sig_p        = &signalBuffer->sig->apexAbpdListAclCnf;
  VgCGACLData        *aclData      = &(ptrToGprsGenericContext ()->vgCGACLData);
  ResultCode_t       result        = translateApnStatusToResult(sig_p->requestStatus);
  Int8               index;
  Int8               apnsToDisplay = sig_p->apnList.numApns;

  if (result == RESULT_CODE_OK)
  {
    if ( sig_p->startField == 0)
    {
      vgPutNewLine (entity);
    }

    if (sig_p->startField +  sig_p->apnList.numApns > aclData->endField )
    {
      apnsToDisplay = aclData->endField - sig_p->startField + 1;
    }

    if (apnsToDisplay > 0)
    {
      vgPrintf(entity, (const Char*)"*MLACL: ");
      for (index = 0; index < apnsToDisplay; index++)
      {
        vgPut8BitNum(entity, index + sig_p->startField);
        vgPutc(entity, ',');
        if (sig_p->apnList.apn[index].length == 0)
        {
          vgPuts(entity, (const Char*) "Network provided APN");
        }
        else
        {
          vgPutc(entity, '\"');
          vgPutsWithLength (entity,
                            sig_p->apnList.apn[index].name,
                            sig_p->apnList.apn[index].length);
          vgPutc(entity, '\"');
          vgPutNewLine (entity);
        }
      }
    }

    if ((sig_p->startField + apnsToDisplay < aclData->endField) &&
        (sig_p->startField + sig_p->apnList.numApns < sig_p->apnList.totalNumApns ))
    {
      aclData->startField =  sig_p->startField + sig_p->apnList.numApns;
      result = vgChManContinueAction (entity, SIG_APEX_ABPD_LIST_ACL_REQ);
    }

  }

  /* Set result code.... */
  setResultCode (entity, result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdWriteAclCnf
 *
 * Parameters:  (In) signalBuffer - a pointer to the SignalBuffer which
 *                                  will contain a ApexAbpdWriteAclCnf
 *                                  structure.
 *              (In) entity       - entity which is handling this signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_ABPD_WRITE_ACL_CNF signal
 *-------------------------------------------------------------------------*/

void vgApexAbpdWriteAclCnf (const SignalBuffer *signalBuffer,
                            const VgmuxChannelNumber entity)
{
  ApexAbpdWriteAclCnf *sig_p = &signalBuffer->sig->apexAbpdWriteAclCnf;
  ResultCode_t        result = translateApnStatusToResult(sig_p->requestStatus);

  /* Set result code.... */
  setResultCode (entity, result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdDeleteAclCnf
 *
 * Parameters:  (In) signalBuffer - a pointer to the SignalBuffer which
 *                                  will contain a ApexAbpdDeleteAclCnf
 *                                  structure.
 *              (In) entity       - entity which is handling this signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_ABPD_DELETE_ACL_CNF signal
 *-------------------------------------------------------------------------*/

void vgApexAbpdDeleteAclCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
  ApexAbpdDeleteAclCnf *sig_p = &signalBuffer->sig->apexAbpdDeleteAclCnf;
  ResultCode_t         result = translateApnStatusToResult(sig_p->requestStatus);

  /* Set result code.... */
  setResultCode (entity, result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdAclStatusCnf
 *
 * Parameters:  (In) signalBuffer - a pointer to the SignalBuffer which
 *                                  will contain a ApexAbpdAclStatusCnf
 *                                  structure.
 *              (In) entity       - entity which is handling this signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_ABPD_ACL_STATUS_CNF signal
 *-------------------------------------------------------------------------*/

void vgApexAbpdAclStatusCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
  ApexAbpdAclStatusCnf *sig_p = &signalBuffer->sig->apexAbpdAclStatusCnf;

  vgPutNewLine (entity);
  vgPrintf     (entity, (const Char*)"*MSACL: %d,%d", sig_p->available, sig_p->enabled);
  vgPutNewLine (entity);

  /* Set result code.... */
  setResultCode (entity, RESULT_CODE_OK);
}
#endif

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdStkInfoInd
 * Parameters:  (In) signalBuffer - a pointer to the SignalBuffer which
 *                                  will contain a ApexAbpdStkInfoInd
 *                                  structure.
 *              (In) entity       - entity which is handling this signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_ABPD_STK_INFO_IND
 * ------------------------------------------------------------------------*/
void vgApexAbpdStkInfoInd   (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  ApexAbpdStkInfoInd   *sig_p = &signalBuffer->sig->apexAbpdStkInfoInd;
  Boolean              found = FALSE;
  Int32                entityIndex;
  Int8                 cidCount;


  PARAMETER_NOT_USED (entity);

  /* Find the entity which is pending a context activation for a particular CID
   */
  for (entityIndex = 0; (entityIndex < CI_MAX_ENTITIES) && (found == FALSE); entityIndex++)
  {
    if (isEntityActive ((VgmuxChannelNumber)entityIndex) == TRUE)
    {
      /* Find the CID associated with this connection */
      for (cidCount = vgGpGetMinCidValue(entityIndex); (cidCount < MAX_NUMBER_OF_CIDS) && (found == FALSE); cidCount++)
      {
        if ((gprsGenericContext_p->cidOwner[cidCount] == entityIndex) &&
            (gprsGenericContext_p->cidUserData[cidCount] != PNULL) &&
            (gprsGenericContext_p->cidUserData[cidCount]->pendingContextActivation == TRUE))
        {
           /* We found the CID which is pending a context activation on this channel
            */
           found = TRUE;
           vgStkDisplayCallControlMessage(SIMAT_CC_GP_OPERATION, sig_p->ccStatus,
           sig_p->alphaIdPresent, &sig_p->alphaId, (VgmuxChannelNumber)entityIndex);
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdWriteRelAssistCnf

 * Parameters:  (In) signalBuffer - a pointer to the SignalBuffer.

 *              (In) entity       - entity which is handling this signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_ABPD_WRITE_REL_ASSIST_CNF
 * ------------------------------------------------------------------------*/

void vgApexAbpdWriteRelAssistCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
  /* Set result code.... no failure action - the request was only a copy to ABPD */
  setResultCode (entity, RESULT_CODE_OK);
}
/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdReadRelAssistCnf

 * Parameters:  (In) signalBuffer - a pointer to the SignalBuffer which
 *                                  will contain an ApexAbpdReadRelAssistCnf
 *                                  structure.
 *              (In) entity       - entity which is handling this signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_ABPD_WRITE_REL_ASSIST_CNF
 * ------------------------------------------------------------------------*/

void vgApexAbpdReadRelAssistCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
    ApexAbpdReadRelAssistCnf  *sig_p = &signalBuffer->sig->apexAbpdReadRelAssistCnf ;

    vgPutNewLine (entity);
    vgPrintf     (entity, (const Char*)"*MNBIOTRAI: %d", sig_p->relAssistInformation);
    vgPutNewLine (entity);

    /* Set result code.... */
    setResultCode (entity, RESULT_CODE_OK);
}
/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdReadApnDataTypeCnf

 * Parameters:  (In) signalBuffer - a pointer to the SignalBuffer which
 *                                  will contain an ApexAbpdReadApnDataTypeCnf
 *                                  structure.
 *                     (In) entity        - entity which is handling this signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_ABPD_READ_APN_DATA_TYPE_CNF
 * ------------------------------------------------------------------------*/

void vgApexAbpdReadApnDataTypeCnf (const SignalBuffer *signalBuffer,
                                             const VgmuxChannelNumber entity)
{
  ApexAbpdReadApnDataTypeCnf  *sig_p = &signalBuffer->sig->apexAbpdReadApnDataTypeCnf ;
  VgPsdStatusInfo             *cidUserData_p;
  GprsGenericContext_t        *gprsGenericContext_p = ptrToGprsGenericContext ();
  Int8                         cidNum;

  vgPutNewLine (entity);
  for (cidNum=0; cidNum < MAX_NUMBER_OF_CIDS; cidNum++)
  {
     cidUserData_p = gprsGenericContext_p->cidUserData[cidNum];

     if (cidUserData_p != PNULL)
     {

       if((cidUserData_p->isActive == TRUE) && (cidUserData_p->psdBearerInfo.connId < MAX_NUM_CONN_IDS))
        {
          if (sig_p->apnDataTypeEntity [cidUserData_p->psdBearerInfo.connId].abpdApnDataTypeValidity == TRUE)
          {
             vgPrintf     (entity, (const Char*)"*MNBIOTDT:%d,%d",cidNum,
                           sig_p->apnDataTypeEntity [cidUserData_p->psdBearerInfo.connId].abpdApnDataType);
             vgPutNewLine (entity);
          }
       }
     }
  }
  /* Set result code.... */
  setResultCode (entity, RESULT_CODE_OK);

}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdWriteApnDataTypeCnf

 * Parameters:  (In) entity       - entity which is handling this signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_ABPD_WRITE_APN_DATA_TYPE_CNF
 * ------------------------------------------------------------------------*/

void vgApexAbpdWriteApnDataTypeCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
  /* Set result code.... no failure action - the request was only a copy to ABPD */
  /* reset the temporary data store */
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  Int8    x;

  for(x =0; x < MAX_NUM_CONN_IDS; x++)
  {
     gprsGenericContext_p->vgReqNbiotDtData.apnDataTypeEntity[x].abpdApnDataTypeValidity = FALSE;
     gprsGenericContext_p->vgReqNbiotDtData.apnDataTypeEntity[x].abpdApnDataType = ABPD_APN_DATA_TYPE_NORMAL;
  }
  setResultCode (entity, RESULT_CODE_OK);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdApnUlRateControlInd
 *
 * Parameters:  (In) entity       - entity which is handling this signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_ABPD_APN_UL_RATE_CONTROL_IND
 * ------------------------------------------------------------------------*/
void vgApexAbpdApnUlRateControlInd (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
  ApexAbpdApnUlRateControlInd  *sig_p = &signalBuffer->sig->apexAbpdApnUlRateControlInd ;
  VgPsdStatusInfo             *activePsdContext_p = PNULL;
  GprsGenericContext_t        *gprsGenericContext_p = ptrToGprsGenericContext ();
  Int8                        cid;

#if defined (FEA_DEDICATED_BEARER)
  Int8                        cidCounter;
  VgPsdStatusInfo             *dedicatedPsdContext_p = PNULL;
#endif

  cid = vgFindCidLinkedToConnId(sig_p->connId);

  if ((cid != CID_NUMBER_UNKNOWN) && (cid < MAX_NUMBER_OF_CIDS))
  {
    activePsdContext_p = gprsGenericContext_p->cidUserData[cid];

    WarnAssert (activePsdContext_p != PNULL);

    /* Check for valid connId */
    if (activePsdContext_p != PNULL)
    {
      activePsdContext_p->apnUplinkRateControlInfo.apnRateControlPresent = sig_p->apnRateControlPresent;
      if (sig_p->apnRateControlPresent)
      {
        activePsdContext_p->apnUplinkRateControlInfo.apnRateControlAdditionalExceptionReportsAllowed =
                sig_p->apnRateControlAdditionalExceptionReportsAllowed;
        activePsdContext_p->apnUplinkRateControlInfo.apnRateControlMaxUplinkRate  = sig_p->apnRateControlMaxUplinkRate;
        activePsdContext_p->apnUplinkRateControlInfo.apnRateControluplinkTimeUnit = sig_p->apnRateControluplinkTimeUnit;
      }

      /* Generate URC */
      vgSendUnsMAPNURI(cid);

#if defined (FEA_DEDICATED_BEARER)
      /* Now check for any dedicated bearers (secondary contexts) assodicated with this connId as they also need
       * to have their APN URC information updated
       */
      for (cidCounter = 0; cidCounter < MAX_NUMBER_OF_CIDS; cidCounter++)
      {
        dedicatedPsdContext_p = gprsGenericContext_p->cidUserData[cid];

        if (dedicatedPsdContext_p != PNULL)
        {
          if ((dedicatedPsdContext_p->psdBearerInfo.secondaryContext) &&
              (dedicatedPsdContext_p->psdBearerInfo.primaryCid == cid))
          {
            /* We have a dedicated bearer whos APN info has also now changed
             * NOTE: We don't know if it is active at this point
             */
            memcpy(&(dedicatedPsdContext_p->apnUplinkRateControlInfo),
                   &(activePsdContext_p->apnUplinkRateControlInfo),
                   sizeof(VgApnUplinkRateControlInfo));

            /* Only send URC if the dedicated bearer is active */
            if (dedicatedPsdContext_p->isActive)
            {
              vgSendUnsMAPNURI(cidCounter);
            }
          }
        }
      }
#endif /* FEA_DEDICATED_BEARER */

    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdPlmnUlRateControlInd
 *
 * Parameters:  (In) entity       - entity which is handling this signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_ABPD_APN_UL_RATE_CONTROL_IND
 * ------------------------------------------------------------------------*/
void vgApexAbpdPlmnUlRateControlInd (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
  ApexAbpdPlmnUlRateControlInd  *sig_p = &signalBuffer->sig->apexAbpdPlmnUlRateControlInd ;
  GprsGenericContext_t          *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgmuxChannelNumber            profileEntity         = 0;

  gprsGenericContext_p->plmnRateControlInfo.plmnRateControlPresent = sig_p->plmnRateControlPresent;

  if (sig_p->plmnRateControlPresent)
  {
    gprsGenericContext_p->plmnRateControlInfo.plmnRateControlValue = sig_p->plmnRateControlValue;
  }

  /* Now generate URC */
  for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
  {
    if (isEntityActive(profileEntity) &&
        (getProfileValue (profileEntity, PROF_MPLMNURI) == PROF_MPLMNAPNURI_ENABLE))
    {
      vgPutNewLine (profileEntity);
      vgPrintf (profileEntity, (const Char *)"*MPLMNURI: %d",
                gprsGenericContext_p->plmnRateControlInfo.plmnRateControlPresent?1:0);

      if (sig_p->plmnRateControlPresent)
      {
        vgPrintf (profileEntity, (const Char *)",%d",
                  sig_p->plmnRateControlValue);
      }
      vgPutNewLine (profileEntity);
      vgFlushBuffer (profileEntity);
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdPacketDiscardInd
 *
 * Parameters:  (In) entity       - entity which is handling this signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_ABPD_APN_UL_RATE_CONTROL_IND
 * ------------------------------------------------------------------------*/
void vgApexAbpdPacketDiscardInd (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
  ApexAbpdPacketDiscardInd      *sig_p = &signalBuffer->sig->apexAbpdPacketDiscardInd ;
  VgmuxChannelNumber            profileEntity         = 0;
  Int8                          cid;

  cid = vgFindCidLinkedToConnId(sig_p->connId);

  if ((cid != CID_NUMBER_UNKNOWN) && (cid < MAX_NUMBER_OF_CIDS))
  {

    /* Now generate URC */
    for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
    {
      if (isEntityActive(profileEntity) &&
          (getProfileValue (profileEntity, PROF_MPDI) == PROF_MPDI_ENABLE))
      {
        vgPutNewLine (profileEntity);
        vgPrintf (profileEntity, (const Char *)"*MPDI: %d,%d,%d",
                  sig_p->status?1:0,
                  cid,
                  sig_p->discardTime);
        vgPutNewLine (profileEntity);
        vgFlushBuffer (profileEntity);
      }
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgApexAbpdMtuInd
 *
 * Parameters:  (In) entity       - entity which is handling this signal.
 *
 * Returns:     Nothing
 *
 * Description: Handles the SIG_APEX_ABPD_MTU_IND
 * ------------------------------------------------------------------------*/
void vgApexAbpdMtuInd (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity)
{
  ApexAbpdMtuInd                *sig_p = &signalBuffer->sig->apexAbpdMtuInd;

  Int8                          cid;
  GprsGenericContext_t          *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo               *activePsdContext_p   = PNULL;

  cid = vgFindCidLinkedToConnId(sig_p->connId);

  if ((cid != CID_NUMBER_UNKNOWN) && (cid < MAX_NUMBER_OF_CIDS))
  {
    activePsdContext_p = gprsGenericContext_p->cidUserData[cid];

    WarnAssert (activePsdContext_p != PNULL);

    if (activePsdContext_p->isActive)
    {
      /* Set the parameters in the signalinfo */
      activePsdContext_p->psdBearerInfo.ipv4MtuSizePresent = sig_p->ipv4MTUPresent;
      activePsdContext_p->psdBearerInfo.ipv4LinkMTU = sig_p->ipv4LinkMTU;
      activePsdContext_p->psdBearerInfo.nonIPMtuSizePresent = sig_p->nonIPMTUPresent;
      activePsdContext_p->psdBearerInfo.nonIPLinkMTU = sig_p->nonIPLinkMTU;
    }
  }
}


#if !defined (USE_L4MM_ALLOC_MEMORY)
#if !defined (USE_BMM_ALLOC_MEMORY)
/*************************************************************************
*
* Function:     vgUtMemAboveHwmInd
*
* Parameters:   signalBuffer - the signal sent from ABGP
*               entity       - mux channel number
*
* Returns:      nothing
*
* Description:  This signal is received when the usage of TMM memory has reached
*               High Water Mark, which means no more allocations from specific pool
*               should be possible.
*************************************************************************/
void vgUtMemAboveHwmInd (const SignalBuffer *signalBuffer,
                         const VgmuxChannelNumber entity)
{
  /* Currently no action is taken for this signal */
  PARAMETER_NOT_USED (signalBuffer);
  PARAMETER_NOT_USED (entity);
}

/*************************************************************************
*
* Function:     vgUtMemBelowLwmInd
*
* Parameters:   signalBuffer - the signal sent from ABGP
*               entity       - mux channel number
*
* Returns:      nothing
*
* Description:  This signal is received when the usage of TMM memory has reached
*               Low Water Mark, which means allocations from specific pool
*               are allowed.
*************************************************************************/
void vgUtMemBelowLwmInd (const SignalBuffer *signalBuffer,
                         const VgmuxChannelNumber entity)
{
  UtMemBelowLwmInd     *sig_p = &signalBuffer->sig->utMemBelowLwmInd;

  PARAMETER_NOT_USED (entity);

  if (sig_p->poolId == TMM_DEFAULT_POOL_UL)
  {
    /* If there are any MGSINK or MGTCSINK commands waiting for UL pool to become
     * available, continue processing them.
     */
    vgProcMgSinkTxPdu();
    vgProcMgtcSinkTxPdu ();
  }
}
#endif /* !USE_BMM_ALLOC_MEMORY */
#endif /* !USE_L4MM_ALLOC_MEMORY */

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */
