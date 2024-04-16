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
 * Utility functions to support SMS commands
 **************************************************************************/

#define MODULE_NAME "RVMSPRNT"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <rvmsprnt.h>
#include <system.h>
#include <rvdata.h>
#include <rvutil.h>
#include <rvprof.h>
#include <rvcrerr.h>
#include <rvoman.h>
#include <rvmsut.h>
#include <smrdwr.h>
#include <smencdec.h>
#include <stdlib.h>
#include <ut_sm.h>
#include <rvcrhand.h>
#include <rvcimxut.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <rvcfg.h>
#include <simdec.h>
#include <gkimem.h>


/***************************************************************************
 * Manifest Constants
 ***************************************************************************/


/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

/***************************************************************************
 * Type Definitions
 ***************************************************************************/


/***************************************************************************
 * Variables
 ***************************************************************************/


/***************************************************************************
 * Local Functions
 ***************************************************************************/








/***************************************************************************
 * Global Functions
 ***************************************************************************/


/*==========================================================================
 *
 * Function:    vgSmsFlush
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description: Print function
 *
 *=========================================================================*/
void vgSmsFlush (const VgmuxChannelNumber entity)
{
  CirmDataInd  *request_p = PNULL;

  if (isEntityActive(entity) == TRUE)
  {
    request_p = getCirmDataBuffer(entity);

    request_p->isSmsUrc = TRUE;
  }  
  
  vgFlushBuffer(entity);
}

/*==========================================================================
 *
 * Function:    vgSmsPutc
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description: Print function
 *
 *=========================================================================*/
void vgSmsPutc(const VgmuxChannelNumber entity, Char c)
{
  vgPutc(entity, c);
}

/*==========================================================================
 *
 * Function:    vgSmsFlush
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description: Print function
 *
 *=========================================================================*/

void vgSmsPutBcdDigit(const VgmuxChannelNumber entity, Int8 val)
{
  vgSmsPutc(entity, (Int8) ('0' + val));
}

/*==========================================================================
 *
 * Function:    vgSmsPrintInt16
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description: Print function
 *
 *=========================================================================*/

void vgSmsPrintInt16(const VgmuxChannelNumber entity, Int16 val)
{
  vgPut16BitNum(entity, val);
}

/*==========================================================================
 *
 * Function:    vgSmsPuts
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description: Print function
 *
 *=========================================================================*/

void vgSmsPuts(const VgmuxChannelNumber entity, const Char *s)
{
  vgPutsWithLength(entity, s, (Int16) vgStrLen(s));
}




/*==========================================================================
 *
 * Function:    vgSmsPrintf
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description: Print function
 *
 *=========================================================================*/

void vgSmsPrintf(const VgmuxChannelNumber entity, Char* fmt, ... )
{
  va_list marker;

  va_start(marker, fmt);
  /*lint -e(437) stops LINT complaining about va_list being sent as
   * a parameter to vgPrintf
   */
  vgPrintf(entity, fmt, marker);
  va_end(marker);
}



/*==========================================================================
 *
 * Function:    vgSmsPutsWithLen
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description: Print function
 *
 *=========================================================================*/
void vgSmsPutsWithLen(const VgmuxChannelNumber entity, const Char *s, Int16 len)
{
  vgPutsWithLength(entity, s, len);
}

/*==========================================================================
 *
 * Function:    vgSmsPrintAlphaIdTextMode
 *
 * Parameters:  string to concat onto and the alpha id
 *
 * Returns:     updated string pointer
 *
 * Description: Concatenated an alpha id onto string
 *
 *=========================================================================*/
void vgSmsPrintNewline(const VgmuxChannelNumber entity)
{
  vgPutNewLine(entity);
}

/*==========================================================================
 *
 * Function:    vgSmsPrintAlphaIdTextMode
 *
 * Parameters:  string to concat onto and the alpha id
 *
 * Returns:     updated string pointer
 *
 * Description: Concatenated an alpha id onto string
 *
 *=========================================================================*/
void vgSmsPrint8BitHex(const VgmuxChannelNumber entity, Int8 val)
{
  vgPut8BitHex(entity, val);
}

/*==========================================================================
 *
 * Function:    vgSmsPrintAlphaIdTextMode
 *
 * Parameters:  string to concat onto and the alpha id
 *
 * Returns:     updated string pointer
 *
 * Description: Concatenated an alpha id onto string
 *
 *=========================================================================*/
void vgSmsPrintAlphaIdTextMode(const VgmuxChannelNumber entity, const SimAlphaIdentifier *alphaId_p)
{
  if (PNULL != alphaId_p)
  {
    vgPutAlphaId(entity, alphaId_p->data, alphaId_p->length);
  }
}

/*==========================================================================
 *
 * Function:    vgSmsPrintAlphaIdPduMode
 *
 * Parameters:  string to concat onto and the alpha id
 *
 * Returns:     updated string pointer
 *
 * Description: Concatenated an alpha id onto string in PDU mode
 *
 *=========================================================================*/
void vgSmsPrintAlphaIdPduMode(const VgmuxChannelNumber entity, const SimAlphaIdentifier *alphaId_p)
{
  Int8 i;
  Int8  length;
  
  if (PNULL != alphaId_p)
  {
    if (alphaId_p->length > 0)
      vgSmsPutc(entity, '"');

    if(alphaId_p->length > SIM_ALPHA_ID_SIZE)
    {
      length = SIM_ALPHA_ID_SIZE;
    }
    else
    {
      length = alphaId_p->length;
    }
    
    for (i=0; i<length; i++)
    {
      vgSmsPrint8BitHex(entity, alphaId_p->data[i]);
    }

    if (alphaId_p->length > 0)
      vgSmsPutc(entity, '"');
  }
}




/*==========================================================================
 *
 * Function:    vgSmsUtilOutputSmsAddressTextMode
 *
 * Parameters:  (InOut) address - a pointer to an SmsAddress
 *              structure containing the input.
 *
 * Returns:     updated Char pointer
 *
 * Description: Converts the address into ASCII digits (if it is
 *              no alphanumeric) and preprends the international
 *              dialing prefix if it is international number.
 *              If the address is alphanumeric it is just copied into
 *              the destination.
 *
 * An example address will look like this:
 *
 * +447720123456
 *
 * Note: the '+' is only prepended for international numbers.
 *
 *=========================================================================*/

void vgSmsPrintSmsAddressTextMode (const VgmuxChannelNumber entity, const SmsAddress *scAddr)
{
  const Int8 *data = scAddr->addressValue;
  Int8       len   = scAddr->length;

  if(len > SMS_MAX_ADDR_LEN)
  {
      len = SMS_MAX_ADDR_LEN;
  }

  if ( scAddr->typeOfNumber == NUM_ALPHANUMERIC)
  {
    vgPutCrSpecificData(entity, VG_DATA_CODED_GSM, data, len);
  }
  else
  {
    /* add the '+' on the front if international number */
    if (scAddr->typeOfNumber == NUM_INTERNATIONAL)
    {
      vgPutCrSpecificChar(entity, VG_DATA_CODED_GSM, INTERNATIONAL_PREFIX);
    }
    /* convert to ASCII(GSM) number digits */
    while (len-- > 0)
    {
      vgPutCrSpecificChar(entity, VG_DATA_CODED_GSM, (Int8)((Char)'0' + *data++));
    }
  }
}



/*--------------------------------------------------------------------------
*
* Function:      vgSmsPrintSmsAddressInPdu
*
* Parameters:    entity
*                scAddr - pointer to the SmsAddress to output
*
* Returns:       new output string pointer
*
* Description:   Outputs the SMS address in hex/PDU format.
*
*                The first two hex digits are the length of the address
*                in bytes.
*
*                If the address is a number (normally) then the rest of
*                the digits are the address data rounded up to bytes
*                i.e. if the address is an odd number of nibbles in length
*                the last nibble is padded to 0xF.
*                If the address is AlphaNumeric then it is just output
*                as hex bytes.
*
*-------------------------------------------------------------------------*/
void vgSmsPrintSmsAddressPduMode (const VgmuxChannelNumber entity, const SmsAddress *scAddr)
{
  Int8 offset;
  Int8 scaLen = 0;
  Int8 scaByte = 0;

  if ( 0 != scAddr->length )
  {
    if ( NUM_ALPHANUMERIC == scAddr->typeOfNumber )
    {
      scaLen = scAddr->length;
    }
    else
    {
      /* calc length of nibbles */
      scaLen = ( (scAddr->length + 1) / 2) + 1;
    }
  }

  /* output length of SC Address information bytes */
  vgSmsPrint8BitHex(entity, scaLen);

  /*
  ** if SC Address information is not present, TPDU starts immediately after
  ** output of SC Address length byte
  */
  if (0 != scAddr->length)
  {
    /* output TON/NPI byte - encode it first from the TON and NPI info */
    scaByte = (Int8)(scAddr->numberingPlan & 0x0F);
    scaByte |= ((scAddr->typeOfNumber & 0x07 ) << 4);
    scaByte |= 0x80;             /* top bit always set */
    vgSmsPrint8BitHex(entity, scaByte);
    if ( NUM_ALPHANUMERIC == scAddr->typeOfNumber )
    {
      /* output AlphaNumeric SC address */
      for (offset = 0; offset < (scAddr->length); offset++)
      {
        vgSmsPrint8BitHex(entity, scAddr->addressValue[offset]);
      }
    }
    else
    {
      /* output the SC Address - coding two digits from SC Address into one byte */
      for (offset = 0; offset < (scAddr->length); offset+=2)
      {
        /* do the low nibble */
        scaByte = scAddr->addressValue[offset] & 0x0F;

        /* do the high nibble */
        if (offset == (scAddr->length - 1))
        {
          scaByte |= 0xF0; /* padding 'F' in last high nibble */
        }
        else
        {
          scaByte |= (scAddr->addressValue[offset+1] & 0x0F) << 4;
        }
        vgSmsPrint8BitHex(entity, scaByte);
      }
    }
  }
}



/*==========================================================================
 *
 * Function:    vgSmsPrintSca
 *
 * Parameters:  entity
 *
 * Returns:     Nothing
 *
 * Description: outputs sms service centre address string
 *
 * e.g. +CSCA: "+44785123456",145
 *
 * or if in extra info mode adds the alphaId supplied from
 * the network (this is non-standard):
 *
 * e.g. +CSCA: "+44785123456",145,"BT Cellnet SMS"
 *
 *=========================================================================*/

void vgSmsPrintSca (const VgmuxChannelNumber  entity)
{
  SmsCommonContext_t  *smsCommonContext_p = ptrToSmsCommonContext ();

  vgSmsPuts(entity, (const Char*)"+CSCA: ");
  vgSmsPutc(entity, '"');
  vgSmsPrintSmsAddressTextMode (entity, &smsCommonContext_p->sca);
  vgSmsPutc(entity, '"');
  vgSmsPutc(entity, ',');
  vgSmsPrintInt16 (entity, vgSmsUtilTypeOfNumberToChar (smsCommonContext_p->sca.typeOfNumber));

  if (vgSmsUtilIsExtraInformationEnabled())
  {
    vgSmsPutc(entity, ',');
    vgSmsPutc(entity, '"');
    vgSmsPrintAlphaIdTextMode (entity, &smsCommonContext_p->scaAlphaId);
    vgSmsPutc(entity, '"');
  }
  vgSmsPrintNewline(entity);
}


/*==========================================================================
 *
 * Function:    vgSmsPrintValidityPeriod
 *
 * Parameters:  entity
 *
 * Returns:     Nothing
 *
 * Description: outputs sms validity period
 *
 *=========================================================================*/

void vgSmsPrintValidityPeriod (const VgmuxChannelNumber  entity)
{
    SmsCommonContext_t  *smsCommonContext_p = ptrToSmsCommonContext ();
    Int8 pid, dcs;

    pid = (Int8)(smsCommonContext_p->protocolId.protocolMeaning +
        smsCommonContext_p->protocolId.protocolId.data);

    EncodeSmsDataCodingScheme (&dcs, &smsCommonContext_p->dataCodingScheme);

    vgSmsPuts         (entity, (const Char*)"+CSMP: ");
    /* job115421: first octet can now be modified by AT+CSMP command */
    vgSmsPrintInt16   (entity, smsCommonContext_p->firstOctet);
    vgSmsPutc         (entity, (Char)',');
    if( (DECODE_MTI(smsCommonContext_p->firstOctet) == SM_MTI_SUBMIT) &&
        (DECODE_VPF(smsCommonContext_p->firstOctet) == SM_VPF_ABSOLUTE))
    {
        vgSmsPrintSmsTimestamp( entity, smsCommonContext_p->validityPeriodAbsolute);
    }
    else
    {
        vgSmsPrintInt16( entity, smsCommonContext_p->validityPeriodValue);
    }
    vgSmsPutc         (entity, (Char)',');
    vgSmsPrintInt16   (entity, (Int16)pid);
    vgSmsPutc         (entity, (Char)',');
    vgSmsPrintInt16   (entity, (Int16)dcs);
    vgSmsPrintNewline (entity);
}


 /*--------------------------------------------------------------------------
 *
 * Function:        vgSmsPrintPdu
 *
 * Parameters:      entity - SMS context entity number
 *                  tpduData - pointer to SMS data
 *                  tpduLength - length of the data
 *                  scAddr - the SMS address
 *
 * Returns:         Nothing
 *
 * Description:     Outputs SMS message TPDU to user in PDU (hex) format.
 *
 *-------------------------------------------------------------------------*/
void vgSmsPrintPdu(const VgmuxChannelNumber entity, const Int8 tpduData[], Int16 tpduLength, const SmsAddress *scAddr)
{
  Int16 i=0;

  /* If not in backward compatible mode then output the address */
  if ( vgSmsIsNotBackwardCompatiblePduMode(entity) )
  {
    vgSmsPrintSmsAddressPduMode( entity, scAddr );
  }

  /* Now output the SMS message  */
  for (i=0; i<tpduLength; i++)
  {
    vgSmsPrint8BitHex(entity, tpduData[i]);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsPrintSmsTimestamp
 *
 * Parameters:  (InOut) data - a pointer to the first element in the
 *              structure containing the Time Stamp info
 *              (InOut) output - a pointer to a Char array that is to
 *              contain the time stamp info extracted from the SmsTimeStamp
 *              structure.
 *
 * Returns:     pointer to end of string
 *
 * Description: From the incoming signal copy the time stamp digits into the
 *              output buffer with appropiate formatting.
 *
 *              Looks like this for 25th Dec 2002:
 *              "02/12/25,12:45:05+00"
 *
 *-------------------------------------------------------------------------*/
void vgSmsPrintSmsTimestamp (const VgmuxChannelNumber entity, const Int8 *data)
{
    /* Print SCTS.... */
    vgSmsPutc(entity, '"');
    vgSmsPutBcdDigit(entity, *data++);
    vgSmsPutBcdDigit(entity, *data++);
    vgSmsPutc(entity, '/');
    vgSmsPutBcdDigit(entity, *data++);
    vgSmsPutBcdDigit(entity, *data++);
    vgSmsPutc(entity, '/');
    vgSmsPutBcdDigit(entity, *data++);
    vgSmsPutBcdDigit(entity, *data++);
    vgSmsPutc(entity, ',');
    vgSmsPutBcdDigit(entity, *data++);
    vgSmsPutBcdDigit(entity, *data++);
    vgSmsPutc(entity, ':');
    vgSmsPutBcdDigit(entity, *data++);
    vgSmsPutBcdDigit(entity, *data++);
    vgSmsPutc(entity, ':');
    vgSmsPutBcdDigit(entity, *data++);
    vgSmsPutBcdDigit(entity, *data++);

    /* check sign bit -
    ** Warning. Spec 23.040 is conflicting about the sign bit.
    ** Sign bit can be bit 7 of the seventh octet (...the first of the two semi octets, the first bit...)
    ** or bit 3 of the seventh octet (...bit 3 of the seventh octet ...)
    ** See section 9.2.3.11
    ** We choose to use bit 7*/
    if( (((*data) & 0x08) == 0) || /* Bit sign is set*/
        (   (((*data) & 0x07) == 0) && /* Or all the remaining timezone is 0*/
            (((*(data+1)) & 0x0F) == 0)
        ))
    {
        vgSmsPutc(entity, '+');
    }
    else
    {
        vgSmsPutc(entity, '-');
    }

    /* remove sign bit and display BCD digit */
    vgSmsPutBcdDigit(entity, (Int8) ((*data++) & 0x07));
    vgSmsPutBcdDigit(entity, *data++);
    vgSmsPutc(entity, '"');
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsPrintUnsolicitedSmDeliveryIndWithAlphaId
 *
 * Parameters:  (In) smDeliveryInd - a pointer to a structure of type
 *              ApexSmDeliveryInd contining the SIG_APEX_SM_DELIVERY_IND
 *              signal to be sent to the CRM
 *              (In) alphaId - the optional alpha id (PNULL if empty)
 *
 * Returns:     Nothing
 *
 * Description: Sends the incoming Short Message to the CRM. A conversion is
 *              performed depending on whether currently in Text or PDU Mode.
 *
 *-------------------------------------------------------------------------*/
void vgSmsPrintUnsolicitedSmDeliveryIndWithAlphaId (const VgmuxChannelNumber entity, const ApexSmDeliveryInd *smDeliveryInd, const VgSmsAlphaId *alphaId)
{
    Int8                     dcsByte = 0;
    Int8                     *tpdu_p = PNULL;
    Int16                    tpduLength;
    const Int8               *smsMessage_p;
    Int8                     smsMessageLength;
    Int16                    udhLength;
    TsDeliverInd             *tsDeliverInd_p = PNULL;
    Int8                     firstOctet;

    if(!isEntityMmiNotUnsolicited(entity))
    {
      vgSmsPrintNewline(entity);
      vgSmsPuts(entity, (const Char*)"+CMT: ");

      if ( vgSmsUtilIsInTextMode(entity) )
      {
        vgSmsPutc(entity, '"');
        vgSmsPrintSmsAddressTextMode(entity, &smDeliveryInd->smeAddr);
        vgSmsPutc(entity, '"');
        vgSmsPutc(entity, ',');
        /* optional <alpha> field */
        if (PNULL != alphaId)
        {
            vgSmsPutc(entity, '"');
            vgSmsPrintAlphaIdTextMode (entity, alphaId);
            vgSmsPutc(entity, '"');
        }
        vgSmsPutc(entity, ',');

        vgSmsPrintSmsTimestamp (entity, smDeliveryInd->scTimeStamp);

        EncodeSmsDataCodingScheme ( &dcsByte, (SmsDataCoding*)&(smDeliveryInd->smsDataCodingScheme));

        if( getProfileValue(entity, PROF_CSDH) == 1)
        {
            vgSmsPutc(entity, ',');
            vgSmsPrintInt16(entity, vgSmsUtilTypeOfNumberToChar (smDeliveryInd->smeAddr.typeOfNumber) );
            vgSmsPutc(entity, ',');

            /* first octet */
            firstOctet  = SM_MTI_DELIVER;
            firstOctet |= ENCODE_MMS( smDeliveryInd->moreMsgsToSend ? 0 : 1);
            firstOctet |= ENCODE_SRI( smDeliveryInd->statusReportInd);
            firstOctet |= ENCODE_UDH( smDeliveryInd->userDataHeaderPresent);
            firstOctet |= ENCODE_RP ( smDeliveryInd->replyPath);
            vgSmsPrintInt16(entity, firstOctet);

            /* protocol */
            vgSmsPutc(entity, ',');
            vgSmsPrintInt16(entity,(Int16)smDeliveryInd->protocolId.protocolMeaning);

            /* DCS */
            vgSmsPutc(entity, ',');

            vgSmsPrintInt16(entity, dcsByte);
            vgSmsPutc(entity, ',');

            /* Address */
            vgSmsPutc(entity, '"');
            vgSmsPrintSmsAddressTextMode(entity, &smDeliveryInd->scAddr);
            vgSmsPutc(entity, '"');
            vgSmsPutc(entity, ',');

            vgSmsPrintInt16(entity, vgSmsUtilTypeOfNumberToChar(smDeliveryInd->scAddr.typeOfNumber));

            vgSmsPutc(entity, ',');
            vgSmsPrintInt16(entity, smDeliveryInd->shortMsgLen);
        }

        smsMessage_p     = &smDeliveryInd->shortMsgData[0];
        smsMessageLength = smDeliveryInd->shortMsgLen;

        /* Strip off the UDH if present */
        if( (0 != smsMessageLength) &&
            (TRUE == smDeliveryInd->userDataHeaderPresent) )
        {
            udhLength = smsMessageLength - utsmGetUserMessageLen((Int8*)smsMessage_p, smsMessageLength);
            smsMessage_p     += udhLength;
            smsMessageLength -= udhLength;
        }

        /* output the message data */
        vgSmsPrintNewline(entity);

        if( (   (cfRvSmHandleConcatSms==TRUE) || 
                (smDeliveryInd->userDataHeaderPresent==FALSE)) && 
            (vgIsSmDcsGsm(dcsByte) == TRUE))
        {
            vgPutCrSpecificData(        entity, 
                                        VG_DATA_CODED_GSM, 
                                        smsMessage_p, 
                                        (Int16)(smsMessageLength));
        }
        else
        {
            vgPutCrSpecificDataCscs(    entity, 
                                        VG_DATA_CODED_HEX, 
                                        smsMessage_p, 
                                        (Int16)(smsMessageLength), 
                                        VG_AT_CSCS_HEX); /*Force output to HEX*/
        }
      }
      else /* pdu mode */
      {
        KiAllocZeroMemory(  sizeof(Int8)*(VG_SMS_PDU_TPDU_SIZE),
                            (void **) &tpdu_p);
        KiAllocZeroMemory(  sizeof(TsDeliverInd),
                            (void **) &tsDeliverInd_p);

        vgSmsConvertSmDeliveryInd (smDeliveryInd, tsDeliverInd_p);

        tpduLength = PackSmsDeliver (tsDeliverInd_p, tpdu_p, VG_SMS_PDU_TPDU_SIZE, FALSE);

        /* optional <alpha> field */
        if (PNULL != alphaId)
        {
            vgSmsPrintAlphaIdPduMode(entity, alphaId);
        }
        vgSmsPutc(entity, ',');

        vgSmsPrintInt16(entity, tpduLength);

        vgSmsPrintNewline(entity);

        vgSmsPrintPdu(entity, tpdu_p, tpduLength, &smDeliveryInd->scAddr);
        
        KiFreeMemory( (void**)&tpdu_p);
        KiFreeMemory( (void**)&tsDeliverInd_p);
      }

      vgSmsPrintNewline(entity);
      vgSmsFlush(entity);
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsPrintUnsolicitedSmReadyInd
 *
 * Parameters:  (In) smSimState_p - a pointer to a structure of type
 *              ApexSmReadyInd.
 *
 * Returns:     Nothing
 *
 * Description: This is extra information i.e. it is non-standard.
 *
 *              Provides user with indication that the SM service
 *              is ready to use, and with the number of records available
 *              etc
 *
 *-------------------------------------------------------------------------*/

void vgSmsPrintUnsolicitedSmReadyInd (const VgmuxChannelNumber entity, 
                                      const ApexSmReadyInd *smSimState_p )
{
  FatalAssert( vgSmsUtilAreExtraUnsolicitedIndicationsEnabled(entity) );

  vgPutNewLine(entity);
  vgPrintf( entity,
            (const Char*)"+SMREADY: %d,%d,%d,%d",
            smSimState_p->unreadRecords,
            smSimState_p->usedRecords,
            (smSimState_p->memCapExceeded ? 1 : 0),
            smSimState_p->numberOfSmsRecords);
  vgPutNewLine(entity);
  vgSmsFlush(entity);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsPrintUnsolicitedSmStatusReport
 *
 * Parameters:  (In) entity - SMS context entity number
 *
 *              (In) smStatusReportInd - a pointer to a structure of type
 *              ApexSmStatusReportInd contining the signal
 *              SIG_APEX_SM_STATUS_REPORT_IND to be sent to the DS
 *
 * Returns:     Nothing
 *
 * Description: Sends the incoming Status Report to the CRM. A conversion is
 *              performed depending on whether currently in Text or PDU Mode.
 *
 *-------------------------------------------------------------------------*/
void vgSmsPrintUnsolicitedSmStatusReport (const VgmuxChannelNumber entity, const ApexSmStatusReportInd *smStatusReport)
{
  Int8               fo = 0;  /* first octet of the Status Report */
  Int8              *tpdu_p = PNULL;
  Int16              tpduLength;
  TsStatusReportInd *tsStatusReportInd_p = PNULL;

  if(!isEntityMmiNotUnsolicited(entity))
  {
    vgSmsPrintNewline(entity);
    vgSmsPuts(entity, (const Char*)"+CDS: ");

    if ( vgSmsUtilIsInTextMode(entity) )
    {
      /* encode the first octet value */
      fo = SM_MTI_STATUS_REPORT;
      fo |= ENCODE_MMS(smStatusReport->moreMsgsToSend ? 0 : 1);
      fo |= (Int8)ENCODE_SRQ(smStatusReport->statusReportQual);
      vgSmsPrintInt16(entity, (Int16)fo);  /* FO */
      vgSmsPutc(entity, ',');

      vgSmsPrintInt16(entity, (Int16)smStatusReport->messageRef);  /* MR */
      vgSmsPutc(entity, ',');

      vgSmsPutc(entity, '"');
      vgSmsPrintSmsAddressTextMode (entity, &smStatusReport->recipientAddr);
      vgSmsPutc(entity, '"');
      vgSmsPutc(entity, ',');

      /* TORA - type of recipient address */
      vgSmsPrintInt16(entity, vgSmsUtilTypeOfNumberToChar(smStatusReport->recipientAddr.typeOfNumber));
      vgSmsPutc(entity, ',');

      /* Service Centre Time Stamp info */
      vgSmsPrintSmsTimestamp(entity, smStatusReport->scTimeStamp);
      vgSmsPutc(entity, ',');

      /* Reception Time info - identical format to Time Stamp digits */
      vgSmsPrintSmsTimestamp(entity, smStatusReport->receptionTime);
      vgSmsPutc(entity, ',');

      vgSmsPrintInt16(entity, (Int16)smStatusReport->smsStatus);  /* ST */
    }
    else /* pdu mode */
    {
      KiAllocZeroMemory(    sizeof(Char)*(VG_SMS_PDU_TPDU_SIZE),
                            (void **) &tpdu_p);
      KiAllocZeroMemory(    sizeof(TsStatusReportInd),
                            (void **) &tsStatusReportInd_p);

      vgSmsConvertSmStatusReportInd (smStatusReport, tsStatusReportInd_p);
      tpduLength = PackSmsStatusReport (tsStatusReportInd_p, tpdu_p, VG_SMS_PDU_TPDU_SIZE);
      vgSmsPrintInt16(entity, tpduLength);
      vgSmsPrintNewline(entity);
      vgSmsPrintPdu(entity, tpdu_p, tpduLength, &smStatusReport->scAddr);

      KiFreeMemory( (void**)&tpdu_p);
      KiFreeMemory( (void**)&tsStatusReportInd_p);
    }

    vgSmsPrintNewline(entity);
    vgSmsFlush(entity);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsPrintUnsolicitedSmStatusReportInd
 *
 * Parameters:  recordNumber - the record number the SMSR is stored at.
 *
 * Returns:     Nothing
 *
 * Description: Sends the CDSI Status Report Indication to the CRM.
 *
 *-------------------------------------------------------------------------*/
void vgSmsPrintUnsolicitedSmStatusReportInd (const VgmuxChannelNumber entity, Int16 recordNumber)
{
  if(!isEntityMmiNotUnsolicited(entity))
  { 
    vgSmsPrintNewline(entity);
    vgSmsPuts(entity, (const Char*)"+CDSI: \"SR\",");
    vgSmsPrintInt16(entity, recordNumber);  /* SMSR Location */
    vgSmsPrintNewline(entity);
    vgSmsFlush(entity);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsPrintUnsolicitedSmMsgReceivedInd
 *
 * Parameters:  recordNumber - the record number the SMS is stored at.
 *
 * Returns:     Nothing
 *
 * Description: Sends the CMTI SMS message received message  to the CRM.
 *
 *-------------------------------------------------------------------------*/
void vgSmsPrintUnsolicitedSmMsgReceivedInd( const VgmuxChannelNumber entity, 
                                            AbsmMemAreaType messageLoc, 
                                            Int16 recordNumber)
{
    if(!isEntityMmiNotUnsolicited(entity))
    {
      vgSmsPrintNewline(entity);
      switch( messageLoc)
      {
        case ABSM_STORAGE_SM:
            {
                vgSmsPuts(entity, (const Char*)"+CMTI: \"SM\",");
            }
            break;

        case ABSM_STORAGE_ME:
        case ABSM_SM_MEMOVER_STORAGE_ME:
            {
                vgSmsPuts(entity, (const Char*)"+CMTI: \"ME\",");
            }
            break;

        default :
            {
                FatalParam( (Int16)messageLoc, 0, 0);
            }
            break;
      }
      vgSmsPrintInt16 (entity, recordNumber);
      vgSmsPrintNewline(entity);
      vgSmsFlush(entity);
    } 
}

 /*--------------------------------------------------------------------------
 *
 * Function:       outputSmsInTextMode
 *
 * Parameters:     (All parameters are input only)
 *                 entity - SMS entity context
 *                 submitOrDeliver - whether this is a SimSmtpduSubmit
 *                                   or SimSmtpduDeliver
 *                 [in] sms_p - the SMS to display
 *                 alphaId - alpha string for phone number (optional)
 *
 * Returns:        Nothing
 *
 * Description:    Ouputs the SMS message to the user in text mode.
 *
 *                 The alphaId is optional and is only output if != PNULL
 *
 *   Example for CMGL:
 *
 *   <status>     <address>       <alphaId>   <timestamp>
 *
 *   "REC UNREAD","+447720123456","Mr. Smith","02/10/25,12:45:05+00"
 *   This is the SMS message text.
 *
 *-------------------------------------------------------------------------*/
void vgSmsPrintSmsInTextMode( const VgmuxChannelNumber    entity,
                              SimSmtpduSubmitDeliverType  submitOrDeliver,
                              const VgDisplaySmsParam    *sms_p,
                              const VgSmsAlphaId         *alphaId)
{
    SmsContext_t *smsContext_p = ptrToSmsContext (entity);
    TypeOfNumber  typeOfNumber;
    Int8          dcsByte;
    Int16         statusIdx;
    Int16         pid;
    Boolean       udhPresent = FALSE;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    if ( VG_SIM_SMTPDU_DELIVER == submitOrDeliver )
    {
        EncodeSmsDataCodingScheme ( &dcsByte, (SmsDataCoding*)&(sms_p->shortMessageTpdu.deliver.smsDataCodingScheme));
        pid = (Int16)sms_p->shortMessageTpdu.deliver.smsProtocolId.protocolMeaning;
        udhPresent = sms_p->shortMessageTpdu.deliver.userDataHeaderPresent;
    }
    else
    {
        EncodeSmsDataCodingScheme ( &dcsByte, (SmsDataCoding*)&(sms_p->shortMessageTpdu.submit.smsDataCodingScheme));
        pid = (Int16)sms_p->shortMessageTpdu.submit.smsProtocolId.protocolMeaning;
        udhPresent = sms_p->shortMessageTpdu.submit.userDataHeaderPresent;
    }

    /* convert SmRecordStatus to status idx */
    statusIdx = (((Int8)(sms_p->recordStatus)) - 1) >> 1;

    /* status of the sms e.g. "UNREAD", */
    vgSmsPutc(entity, '"');
    vgSmsPuts(entity, vgSmsUtilGetRecordStatusString(statusIdx) );
    vgSmsPuts(entity, (const Char*)"\",");

    /* the dialed number e.g. "+447720123456", */
    vgSmsPutc(entity, '"');
    vgSmsPrintSmsAddressTextMode( entity, vgSmsGetShortMessageSmeAddress( sms_p));
    vgSmsPuts(entity, (const Char*)"\",");

    /* optional <alpha> field */
    if (PNULL != alphaId)
    {
        vgSmsPutc(entity, '"');
        vgSmsPrintAlphaIdTextMode(entity, alphaId);
        vgSmsPutc(entity, '"');
    }
    vgSmsPutc(entity, ',');

    if ( VG_SIM_SMTPDU_DELIVER == submitOrDeliver )
    {
        /* date/time e.g. "02/12/25,12:45:05+00" */
        vgSmsPrintSmsTimestamp(entity, sms_p->shortMessageTpdu.deliver.scTimeStamp);
    }

    if (1 == getProfileValue(entity, PROF_CSDH))
    {
        /* extra information... */

        vgSmsPutc(entity, ',');
        typeOfNumber = vgSmsGetShortMessageSmeAddress(sms_p)->typeOfNumber;

        /* Number prefix */
        vgSmsPrintInt16(entity, vgSmsUtilTypeOfNumberToChar(typeOfNumber));

        if( (smsContext_p->readState == SM_RW_ABSOLUTE)||(smsContext_p->readState == SM_PREVIEW_ONLY) )
        {
            FatalAssert ((getCommandId(entity) == VG_AT_SMS_CMGR) ||
                         (getCommandId(entity) == VG_AT_SMS_MCMGR));

            vgSmsPutc(entity, ',');
            vgSmsPrintInt16(entity, SmEncodeFirstOctet( sms_p->tpduType, &sms_p->shortMessageTpdu));
            vgSmsPutc(entity, ',');
            vgSmsPrintInt16(entity, pid);
            vgSmsPutc(entity, ',');
            vgSmsPrintInt16(entity, dcsByte);
            vgSmsPutc(entity, ',');

            if ( VG_SIM_SMTPDU_SUBMIT == submitOrDeliver)
            {
                /* validity period */
                switch( sms_p->shortMessageTpdu.submit.validityPeriodFormat)
                {
                    case VP_INTEGER_FORMAT:
                        {
                            vgSmsPrintInt16(entity,(Int16)sms_p->shortMessageTpdu.submit.validityPeriodAsValue);
                            vgSmsPutc(entity, ',');
                        }
                        break;

                    case VP_SEMI_OCTET:
                        {
                            vgSmsPrintSmsTimestamp( 
                                entity, (Int8*)sms_p->shortMessageTpdu.submit.validityPeriodAsTime);
                            vgSmsPutc(entity, ',');
                        }
                        break;

                    default:
                        {
                            /* Print nothing*/
                        }
                        break;
                }
            }

            /* the SMS service centre address (phone number) */
            vgSmsPutc(entity, '"');
            vgSmsPrintSmsAddressTextMode(entity, &sms_p->scAddr);
            vgSmsPutc(entity, '"');
            vgSmsPutc(entity, ',');

            /* type of number prefix */
            vgSmsPrintInt16(entity, vgSmsUtilTypeOfNumberToChar(sms_p->scAddr.typeOfNumber));

        } /* end of (smsContext_p->readState == SM_RW_ABSOLUTE) */

        vgSmsPutc(entity, ',');

        /* the length */
        if(smsContext_p->smsLength > VG_SMS_MAX_TEXT_LENGTH)
        {
            smsContext_p->smsLength = VG_SMS_MAX_TEXT_LENGTH;
        }
        vgSmsPrintInt16(entity, (Int16)smsContext_p->smsLength);
    }

    /* display the rest... */
    vgSmsPrintNewline(entity);

    /* display the message text */
    if( (   (cfRvSmHandleConcatSms==TRUE) || 
            (udhPresent == FALSE)) && 
        (vgIsSmDcsGsm(dcsByte) == TRUE))
    {
        vgPutCrSpecificData(    entity, 
                                VG_DATA_CODED_GSM, 
                                smsContext_p->smsMessage, 
                                (Int16)(smsContext_p->smsLength));
    }
    else
    {
        vgPutCrSpecificDataCscs(    entity, 
                                    VG_DATA_CODED_HEX, 
                                    smsContext_p->smsMessage, 
                                    (Int16)(smsContext_p->smsLength), 
                                    VG_AT_CSCS_HEX); /*Force output to HEX*/
    }

    vgSmsPrintNewline(entity);
}

/*--------------------------------------------------------------------------
*
* Function:       vgSmsPrintSmsInPduMode
*
* Parameters:     (All parameters are input only)
*                 entity - SMS entity context
*                 submitOrDeliver - whether this is a SimSmtpduSubmit
*                                   or SimSmtpduDeliver
*                 output - string to append to then output
*                 [in] sms_p - the SMS to display
*                 alphaId - alpha string for phone number (optional)
*
* Returns:        Nothing
*
* Description:    Ouputs the SMS message to the user in PDU mode
*
* e.g. It will look something like this:
*
* +CMGL: 3,1,,0
* 0791448720003023840C9144770295381800002020705104640004D4E2940A
*
*-------------------------------------------------------------------------*/

void vgSmsPrintSmsInPduMode( const VgmuxChannelNumber    entity,
                             SimSmtpduSubmitDeliverType  submitOrDeliver,
                             const VgDisplaySmsParam    *sms_p,
                             const VgSmsAlphaId         *alphaId )
{
    Char                *tpdu_p = PNULL;
    Int16               tpduLength;
    Int8                pduIdx;
    VgDisplaySmsParam   *paramCopy_p = PNULL;
    GeneralContext_t    *generalContext_p    = ptrToGeneralContext(entity);
    Boolean             saveOmitFirstNewLine;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
    KiAllocZeroMemory(  sizeof(Char)*(VG_SMS_PDU_TPDU_SIZE),
                        (void **) &tpdu_p);
    KiAllocZeroMemory(  sizeof(VgDisplaySmsParam),
                        (void **) &paramCopy_p);

    saveOmitFirstNewLine = generalContext_p->omitFirstNewLine;

    /* Some modification on signal attributs may be needed, so make a copy*/
    memcpy( paramCopy_p, sms_p, sizeof(VgDisplaySmsParam));

    /* Pack various info fields from bytes to bits into
       tpdu according to GSM spec 03.40 9.2.2.2.
    */
    if ( VG_SIM_SMTPDU_SUBMIT == submitOrDeliver )
    {

        /* If userDataHeaderPresent == TRUE, then check the length for illegal coding.
         * If illegal, then mark header as not present....
         */
        if( (paramCopy_p->shortMessageTpdu.submit.userDataHeaderPresent == TRUE) &&
            (paramCopy_p->shortMessageTpdu.submit.shortMsgData[0] > paramCopy_p->shortMessageTpdu.submit.shortMsgLen) )
        {
            paramCopy_p->shortMessageTpdu.submit.userDataHeaderPresent = FALSE;
        }
        
        if(paramCopy_p->shortMessageTpdu.submit.smeAddr.length>SMS_MAX_ADDR_LEN)
        {
            paramCopy_p->shortMessageTpdu.submit.smeAddr.length = SMS_MAX_ADDR_LEN;
        }

        if(paramCopy_p->shortMessageTpdu.submit.scAddr.length>SMS_MAX_ADDR_LEN)
        {
            paramCopy_p->shortMessageTpdu.submit.scAddr.length = SMS_MAX_ADDR_LEN;
        }
        
        tpduLength = PackSmsSubmit ((TsSubmitReq*)&paramCopy_p->shortMessageTpdu.submit, tpdu_p, VG_SMS_PDU_TPDU_SIZE);
    }
    else
    {
        if( (paramCopy_p->shortMessageTpdu.deliver.userDataHeaderPresent == TRUE) &&
            (paramCopy_p->shortMessageTpdu.deliver.shortMsgData[0] > paramCopy_p->shortMessageTpdu.deliver.shortMsgLen) )
        {
            paramCopy_p->shortMessageTpdu.deliver.userDataHeaderPresent = FALSE;
        }

        if(paramCopy_p->shortMessageTpdu.deliver.smeAddr.length > SMS_MAX_ADDR_LEN)
        {
             paramCopy_p->shortMessageTpdu.deliver.smeAddr.length = SMS_MAX_ADDR_LEN;
        }

        if(paramCopy_p->shortMessageTpdu.deliver.scAddr.length > SMS_MAX_ADDR_LEN)
        {
             paramCopy_p->shortMessageTpdu.deliver.scAddr.length = SMS_MAX_ADDR_LEN;
        }
        
        tpduLength = PackSmsDeliver ((TsDeliverInd*)&paramCopy_p->shortMessageTpdu.deliver, tpdu_p, VG_SMS_PDU_TPDU_SIZE, FALSE);
    }

    /* first output the header info:
    **
    ** t,,nn
    ** ^ ^ ^
    ** | | the length of the tpdu data in decimal
    ** | the "alpha" field which is optional
    ** the status of the SMS message e.g. 0 = read, 1 = unread etc
    */
    pduIdx = (((Int8)(paramCopy_p->recordStatus)) - 1) >> 1;
    vgSmsPrintInt16(entity, vgSmsUtilConvertPduIdxToStatusIdx(pduIdx));
    vgSmsPutc(entity, ',');

    /* optional <alpha> field */
    vgSmsPrintAlphaIdPduMode (entity, alphaId);
    vgSmsPutc(entity, ',');

    /* output the length */
    if(tpduLength > VG_SMS_PDU_TPDU_SIZE)
    {
        tpduLength = VG_SMS_PDU_TPDU_SIZE;
    }
    vgSmsPrintInt16(entity, tpduLength);
    /*In this case, a new line is necessary.*/
    generalContext_p->omitFirstNewLine = FALSE;
    vgSmsPrintNewline(entity);
    /*Restore the value.*/
    generalContext_p->omitFirstNewLine = saveOmitFirstNewLine;

    /* output the tpdu and address in PDU (hex) format:
    **
    ** llnnnnnnnnxxxxxxxxxxxxxxxxxxxxxxxx
    ** ^ ^       ^
    ** | |       the tpdu data
    ** | the address
    ** two hex digits for the length of the address in bytes
    **
    */
    vgSmsPrintPdu(entity, tpdu_p, tpduLength, &paramCopy_p->scAddr);
    /*In this case, a new line is necessary.*/
    generalContext_p->omitFirstNewLine = FALSE;
    vgSmsPrintNewline(entity);
    /*Restore the value.*/
    generalContext_p->omitFirstNewLine = saveOmitFirstNewLine;

    KiFreeMemory( (void**)&tpdu_p);
    KiFreeMemory( (void**)&paramCopy_p);
}

 /*--------------------------------------------------------------------------
 *
 * Function:       outputSmsrInTextMode
 *
 * Parameters:     (All parameters are input only)
 *                 entity - SMS entity context
 *                 output - string to append to then output
 *                 signal - pointer to ApexSmReadCnf structure
 *
 * Returns:        Nothing
 *
 * Description:    Ouputs the SMS SR message to the user in text mode.
 *
 *
 *   Example for CMGL:
 *
 *   <status>,<fo>,<mr>,[<ra>],[<tora>],<scts>,<dt>,<st>
 *
 *   "REC READ",6,160,"+33612345678",129,"09/05/01,17:29:09+00",0
 *
 *-------------------------------------------------------------------------*/

void vgSmsPrintSmsrInTextMode( const VgmuxChannelNumber  entity,
                               const VgDisplaySmsSrParam *display_p )
{
    Int8   fo = 0;  /* first octet of the Status Report */
    Int16  statusIdx;

    /* convert "REC READ" Status to status idx in smsRecordTypes[] table */
    statusIdx = 0;

    /* status of the SMS-SR is always "READ", */
    vgSmsPutc(entity, '"');
    vgSmsPuts(entity, vgSmsUtilGetRecordStatusString(statusIdx) );
    vgSmsPuts(entity, (const Char*)"\",");

    /* encode the first octet value */
    fo  = SM_MTI_STATUS_REPORT;
    fo |= ENCODE_MMS(display_p->smStatusReport.moreMsgsToSend ? 0 : 1);
    fo |= ENCODE_SRQ(display_p->smStatusReport.statusReportQual);
    fo |= ENCODE_UDH(display_p->smStatusReport.userDataHeaderPresent ? 0 : 1);
    vgSmsPrintInt16(entity, (Int16)fo);                             /* FO */
    vgSmsPutc(entity, ',');

    vgSmsPrintInt16(entity, (Int16)display_p->smStatusReport.msgRef);  /* MR */
    vgSmsPutc(entity, ',');

    vgSmsPutc(entity, '"');
    vgSmsPrintSmsAddressTextMode (entity, &display_p->smStatusReport.recipientAddr);  /* RA */
    vgSmsPutc(entity, '"');
    vgSmsPutc(entity, ',');

    /* TORA - type of recipient address */                          /* TORA */
    vgSmsPrintInt16(entity, vgSmsUtilTypeOfNumberToChar(display_p->smStatusReport.recipientAddr.typeOfNumber));
    vgSmsPutc(entity, ',');

    /* Service Centre Time Stamp info */                            /* SCTS */
    vgSmsPrintSmsTimestamp(entity, display_p->smStatusReport.scTimeStamp);
    vgSmsPutc(entity, ',');

    /* Reception Time info - Discharge Time */                      /* DT */
    vgSmsPrintSmsTimestamp(entity, display_p->smStatusReport.receptionTime);
    vgSmsPutc(entity, ',');

    /* Status of the previously SMS submitted or command */         /* ST */
    vgSmsPrintInt16(entity, (Int16)display_p->smStatusReport.smsStatus);

    vgSmsPrintNewline(entity);
}

/*--------------------------------------------------------------------------
*
* Function:       vgSmsPrintSmsrInPduMode
*
* Parameters:     (All parameters are input only)
*                 entity - SMS entity context
*                 submitOrDeliver - whether this is a SimSmtpduSubmit
*                                   or SimSmtpduDeliver
*                 output - string to append to then output
*                 signal - pointer to ApexSmReadCnf structure
*                 alphaId - alpha string for phone number (optional)
*
* Returns:        Nothing
*
* Description:    Ouputs the SMS message to the user in PDU mode
*
* e.g. It will look something like this:
*
* +CMGL: 3,1,,0
* 0791448720003023840C9144770295381800002020705104640004D4E2940A
*
*-------------------------------------------------------------------------*/

void vgSmsPrintSmsrInPduMode( const VgmuxChannelNumber  entity,
                              const VgDisplaySmsSrParam *display_p )
{
    Char              *tpdu_p = PNULL;
    Int16              tpduLength;

    KiAllocZeroMemory(  sizeof(Char)*(VG_SMS_PDU_TPDU_SIZE),
                        (void **) &tpdu_p);

    tpduLength = PackSmsStatusReport(&display_p->smStatusReport, tpdu_p, VG_SMS_PDU_TPDU_SIZE);

    /* output the length */
    vgSmsPrintInt16(entity, tpduLength);
    vgSmsPrintNewline(entity);

    /* output the tpdu and address in PDU (hex) format:
    **
    ** llnnnnnnnnxxxxxxxxxxxxxxxxxxxxxxxx
    ** ^ ^       ^
    ** | |       the tpdu data
    ** | the address
    ** two hex digits for the length of the address in bytes
    **
    */
    vgSmsPrintPdu(entity, tpdu_p, tpduLength, &display_p->smStatusReport.scAddr);
    vgSmsPrintNewline(entity);

    KiFreeMemory( (void**)&tpdu_p);
}


/* END OF FILE */

