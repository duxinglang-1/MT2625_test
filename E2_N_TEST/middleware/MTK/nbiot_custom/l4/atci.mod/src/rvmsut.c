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

#define MODULE_NAME "RVMSUT"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <rvdata.h>
#include <rvutil.h>
#include <rvmsss.h>
#include <rvmsprnt.h>
#include <rvprof.h>
#include <rvcrerr.h>
#include <rvmssigo.h>
#include <rvmssigi.h>
#include <rvoman.h>
#include <rvmsut.h>
#include <rvnvram.h>
#include  <smrdwr.h>
#include <smencdec.h>
#include <stdlib.h>
#include <ut_sm.h>
#include <rvchman.h>
#include <rvcrconv.h>
#include <rvmspars.h>
#include <rvcimxut.h>
#include <rvcfg.h>
#include <simdec.h>
#include <rvcimxut.h>
#include <gkimem.h>

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

#define GSM_EXTENDED_CHARS (0x20)

/***************************************************************************
 * Type Definitions
 ***************************************************************************/
#if defined (DEVELOPMENT_VERSION)
union Signal
{
  ApexSmMsgReceivedInd apexSmMsgReceivedInd;
};
#endif

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static ResultCode_t vgBuildTsCommandReq       (Int16 tpduLength, const Int8 *dataPtr, TsCommandReq *tsCommandReq);

static ResultCode_t vgBuildTsSubmitReq        (Int16 tpduLength, const Int8 *dataPtr, TsSubmitReq  *tsSubmitReq);

static ResultCode_t vgBuildTsDeliverInd       (Int16 tpduLength, const Int8 *dataPtr, TsDeliverInd *tsDeliverInd);

static ResultCode_t vgBuildTsStatusReportInd  (Int16 tpduLength, const Int8 *dataPtr, TsStatusReportInd *tsSrInd);

static ResultCode_t vgBuildTsDeliverReportReq(  Int16               tpduLength,
                                                const Int8         *dataPtr,
                                                TsDeliverReportReq *tsDrReq);

static void    codePidFieldForApexSmCommandReq(const VgmuxChannelNumber entity);

static Boolean buildSmsPduScAddress           (Int8 *data, SmsAddress *scAddr);

static ResultCode_t checkCommandSmsParams     (const VgmuxChannelNumber entity);

static ResultCode_t readAddressData (Int8 octetPosition, SmsAddress *addr,
                                     const Int8 *dataPtr,
                                     Int8 *octetsRead);

static Boolean isValidAddressDigit            (Char digit, Int8* index);

static Int8    hexToNum                       (Char ch);

/***************************************************************************
 * Variables
 ***************************************************************************/


/* Lookup tables ***********************************************************/

static const Char smsValidDigits[] = "01234567890*#ABC";
static const Char smsTimestampValidDigits[] = "01234567890";

static const struct vgSmsListTypeTag {
  const Char       *name;
  SmsSimAccessType simAccessValue;
  Int8             convertFromPduIndex;
} smsRecordTypes[] =
{
  { (const Char*)"REC READ",   SM_FIRST_READ,                1 /*map 0 to 1 (unread)  */},
  { (const Char*)"REC UNREAD", SM_FIRST_UNREAD,              0 /*map 1 to 0 (read)    */},
  { (const Char*)"STO SENT",   SM_FIRST_ORIGINATED_SENT,     3 /*map 2 to 3 (notsent) */},
  { (const Char*)"STO UNSENT", SM_FIRST_ORIGINATED_NOT_SENT, 2 /*map 3 to 2 (sent)    */},
  { (const Char*)"ALL",        SM_FIRST_ANY,                 4 /*map 4 to 4 (any)     */}
};

static const struct vgSmsStatusTypeTag {
  const Char       *name;
  SimSmRecordStatus simRecordStatus;
  Int8             convertFromPduIndex;
} smsStatusTypes[] =
{
  { (const Char*)"REC READ",   SIM_SMREC_RECEIVED_READ,       1 },
  { (const Char*)"REC UNREAD", SIM_SMREC_RECEIVED_UNREAD,     0 },
  { (const Char*)"STO SENT",   SIM_SMREC_ORIGINATED_SENT,     3 },
  { (const Char*)"STO UNSENT", SIM_SMREC_ORIGINATED_NOT_SENT, 2 },
};


/***************************************************************************
 * Macros
 ***************************************************************************/


/*****************************************
 ** Macros for SMS +CMGS/+CMGW commands **
 *****************************************/


/*
Set the field starting at the LSB specified by [bp]
within octet [op] to the value specified by [fv].
The octet being written to is assumed to have had the appropriate bits
initialised to zero (allowing logical OR operation to be used)
The value is assumed NOT to straddle an octet boundary.
The value is assumed to fit the available bit space.
See GSM 03.40 9.1.1.
*/
#define VG_WRITE_BIT_DATA(op, bp, fv, dptr) dptr[op] |= (fv << bp)

/*
Read the field starting at [bitPosition] within octet
[octetPosition] and store in [fieldValue].
The value is assumed NOT to straddle an octet boundary.
See GSM 03.40 9.1.1.
*/
#define VG_READ_BIT_DATA(octetPosition, bitPosition,        \
                          fieldSize, fieldValue, dataPtr)   \
   *(fieldValue) = dataPtr[octetPosition] >> bitPosition;   \
   *(fieldValue) &= ~(0xFF << fieldSize);

/*
Read the integer value at [octetPosition] and return in [fieldValue]
Note that it is assumed that the integer occupies 8 bits of an octet
ie does not straddle an octet boundary.
See GSM 03.40 9.1.2.1.
*/
#define VG_READ_INTEGER_DATA(octetPosition, fieldValue, dataPtr) \
   *(fieldValue) = dataPtr[octetPosition];

/* Compute memory space required to store [n] values in semi-octet form */
#define VG_SEMI_OCTET_SIZE(a)        ((a)/2 + (a)%2)

/* Calculate number of octets of 7-bit packed user data */
#define VG_7_BIT_USER_DATA_SIZE(a)  (((a)*7)/8 + ((((a)*7)%8)!=0 ? 1:0))

/*
Set the field starting at the LSB specified by [bp]
within octet [op] to the value specified by [fv].
The octet being written to is assumed to have had the appropriate bits
initialised to zero (allowing logical OR operation to be used)
The value is assumed NOT to straddle an octet boundary.
The value is assumed to fit the available bit space.
See GSM 03.40 9.1.1.
*/
#define VG_WRITE_BIT_DATA(op, bp, fv, dptr) dptr[op] |= (fv << bp)

/*
Write an item of data of type integer stored in [val] to the memory
area pointed to by [bp] and offset by [op]
Note that it is assumed that the integer occupies 8 bits of an octet
ie does not straddle an octet boundary.
See GSM 03.40 9.1.2.1.
*/
#define VG_WRITE_INTEGER_DATA(op, val, dptr) dptr[op] = val

/*
Set the field starting at the LSB specified by [bp] to the value specified by
[fv]. The octet being written to is assumed to have had the appropriate bits
initialised to zero (allowing logical OR operation to be used)
The value is assumed NOT to straddle an octet boundary.
The value is assumed to fit the available bit space.
See GSM 03.40 9.1.1.
*/
#define VG_WRITE_BIT_TO_DCS_BYTE(bp, fv, dptr) dptr |= (fv << bp)

/***************************************************************************
 * Local Functions
 ***************************************************************************/
/*--------------------------------------------------------------------------
 *
 * Function:    vgHexToNum
 *
 * Parameters:  input - variable to convert
 *
 * Returns:     Int8 representation of input
 *
 * Description: converts a hexadecimal digit to a number
 *
 *-------------------------------------------------------------------------*/
static Int8 hexToNum(Char ch)
{
  ch = (Char)toupper(ch);

  if ( isdigit(ch) )
  {
    ch -= '0';
  }
  else if ( isxdigit(ch) )
  {
    ch -= 'A';
    ch += 0x0A;
  }
  else
  {
    /* Unknown hexadecimal digit */
    FatalParam(ch, 0, 0);
  }

  return ch;
}

/*-----------------------------------------------------------------------
 *
 * Function:    vgBuildTsCommandReq
 *
 * Parameters:  (In) data - a pointer to the start of the pdu data from which
 *              to build the TS_COMMAND_REQ from..
 *              (Out) tsCommandReq - pointer to the TS_COMMAND signal to
 *              build the raw PDU Mode data into.
 *
 * Returns:     result - flag to determine if Command Request has been
 *              successfully built from the pdu:
 *
 *              VG_CMS_ERROR_INVALID_PDU_INCORRECT_UDL
 *              VG_CMS_ERROR_INVALID_PDU_ADDRESS
 *              VG_CMS_ERROR_INVALID_PDU_DIGITS_IN_ADDRESS
 *
 * Description: Creates a TS_COMMAND_REQUEST from the pdu.
 *
 *------------------------------------------------------------------------*/
static ResultCode_t vgBuildTsCommandReq(Int16 tpduLength, const Int8 *dataPtr, TsCommandReq *tsCommandReq)
{
  Int8         octetPos      = 0;
  Int8         numOctetsInAddress = 0;
  ResultCode_t resultCode = RESULT_CODE_OK;

  VG_READ_BIT_DATA(octetPos, 5, 1, (Int8 *) &tsCommandReq->statusReportReq,
                   dataPtr);                                       /* SRR */
  octetPos++;

  VG_READ_INTEGER_DATA(octetPos, (Int8 *) &tsCommandReq->msgRef, dataPtr);
                                                                   /* MR  */
  octetPos++;

  tsCommandReq->smsProtocolId.protocolMeaning = (ProtocolMeaning)(dataPtr[octetPos] & 0xe0);  /* PID */

  switch (tsCommandReq->smsProtocolId.protocolMeaning)
  {
    /*
    ** For PM_SM_AL_PROTOCOL and PM_TELEMATIC_INTERWORK need to decode
    ** 5 bits
    */
    case PM_SM_AL_PROTOCOL:
    case PM_TELEMATIC_INTERWORK:
    {
      VG_READ_BIT_DATA(octetPos, 0, 5, (Int8 *) &tsCommandReq->smsProtocolId.protocolId.data,
                       dataPtr);
      break;
    }
    /*
    ** For PM_SM_FUNCTIONS, PM_RESERVED and PM_SC_SPECIFIC and anything
    ** else decode six bits.
    */
    default:
    {
      VG_READ_BIT_DATA(octetPos, 0, 6, (Int8 *) &tsCommandReq->smsProtocolId.protocolId.data, dataPtr);
      break;
    }
  }
  octetPos++;

  VG_READ_INTEGER_DATA(octetPos, (Int8 *) &tsCommandReq->commandType, dataPtr);
                                                                   /* CT  */
  octetPos++;

  VG_READ_INTEGER_DATA(octetPos, (Int8 *) &tsCommandReq->msgNum, dataPtr);
                                                                   /* MN  */
  octetPos++;

  /* only continue if address length byte does not exceed max length allowed */
  if (dataPtr[octetPos] <= VG_SMS_MAX_ADDRESS_LENGTH)
  {
    resultCode = readAddressData (octetPos, &tsCommandReq->smeAddr, dataPtr,
                                  &numOctetsInAddress);              /* OA  */
    octetPos += numOctetsInAddress;
  }
  else
  {
      resultCode = RESULT_CODE_ERROR;
  }

  if (RESULT_CODE_OK == resultCode)
  {
    /*
    ** Only decode Data if User Data Length given in PDU is valid
    ** against the length of the TPDU given in the command AT+CMGC=xx
    **
    ** Return error if UDL byte is not correct.
    */

    /* Check number of octets of Command Data */
    if ( dataPtr[octetPos] == (tpduLength - (octetPos + 1)))
    {
      /* Decode in octet format */
      DecodeSmOctetFormat ( (Int8 *) &(dataPtr[octetPos]),
                            (Int8 *) &tsCommandReq->cmdDataLen,
                            (Int8 *) &tsCommandReq->cmdData[0],
                            SMS_MAX_PDU_DATA_LEN);
    }
    else /* expected number of User Data octets is incorrect */
    {
      resultCode = VG_CMS_ERROR_INVALID_PDU_INCORRECT_UDL;
    }
  }
  return (resultCode);
}




/*--------------------------------------------------------------------------
 *
 * Function:    codePidFieldForApexSmCommandReq
 *
 * Parameters:  SMS context entity number
 *
 * Returns:     nothing
 *
 * Description: Builds the pid value from the pidByteVal
 *
 *-------------------------------------------------------------------------*/
static void codePidFieldForApexSmCommandReq(const VgmuxChannelNumber entity)
{
  SmsContext_t  *smsContext_p = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  /* code the pid field for building into the ApexSmCommandReq */
  smsContext_p->command.pid.protocolMeaning = (ProtocolMeaning)(smsContext_p->command.pidByteVal & 0xe0);

  switch (smsContext_p->command.pid.protocolMeaning)
  {
     /*
     ** For PM_SM_AL_PROTOCOL and PM_TELEMATIC_INTERWORK need to decode
     ** 5 bits
     */
     case PM_SM_AL_PROTOCOL:
     case PM_TELEMATIC_INTERWORK:
     {
        smsContext_p->command.pid.protocolId.data = smsContext_p->command.pidByteVal & 0x1f;
        break;
     }
     /*
     ** For PM_SM_FUNCTIONS, PM_RESERVED and PM_SC_SPECIFIC and anything
     ** else decode six bits.
     */
     default:
     {
        smsContext_p->command.pid.protocolId.data = smsContext_p->command.pidByteVal & 0x3f;
        break;
     }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgBuildTsDeliverReportReq
 *
 * Parameters:  (In) dataPtr - a pointer to the start of the pdu data from which
 *              to extraxt the SC Address from.
 *              (InOut) tsDrReq - pointer to the TS_DELIVER_REPORT signal to
 *              build the SC Address information into.
 *
 * Returns:     result - flag to determine if deliver indirection has been
 *              successfully built from the pdu.
 *
 * Description: Creates a TS_DELIVER_REPORT_REQUEST from the pdu.
 *
 *------------------------------------------------------------------------*/
static ResultCode_t vgBuildTsDeliverReportReq(  Int16               tpduLength,
                                                const Int8         *dataPtr,
                                                TsDeliverReportReq *tsDrReq)
{
    Int8          octetPos             = 0;
    Int8          numUserDataOctets    = 0;
    Int8          actualUserDataOctets = 0;
    Int8          mti                  = 0;
    Int8          tmp                  = 0;
    Boolean       extensionBit         = FALSE;
    ResultCode_t  resultCode           = RESULT_CODE_OK;

    if( octetPos < tpduLength)
    {
        VG_READ_BIT_DATA(octetPos, 0, 2, (Int8*) &mti, dataPtr);                                    /* MTI  */
        FatalCheck( mti == SM_MTI_DELIVER_REPORT, mti, 0, 0);
        if( mti == SM_MTI_DELIVER_REPORT)
        {
            VG_READ_BIT_DATA(octetPos, 6, 1, (Int8*) &tsDrReq->userDataHeaderPresent, dataPtr);     /* UDHI  */
            octetPos++;
        }
        else
        {
            resultCode = VG_CMS_ERROR_INVALID_PDU_MTI;
        }
    }
    else
    {
        resultCode = VG_CMS_ERROR_INVALID_PDU_LENGTH;
    }

    if( RESULT_CODE_OK == resultCode )
    {

        if( tsDrReq->statusOfReport == TRANSFER_ERROR)
        {
            /* In this case there is an additional error cause paramater*/
            if( octetPos < tpduLength)
            {
                VG_READ_INTEGER_DATA( octetPos, &tmp, dataPtr);                 /* FCS*/
                /** Avoid Lint warning*/
                tsDrReq->tpFailureCause = (TpFailureCause)tmp;
                octetPos++;
            }
            else
            {
                resultCode = VG_CMS_ERROR_INVALID_PDU_LENGTH;
            }
        }
        else
        {
            /*  TP_NO_ERROR is implicit for this case*/
            tsDrReq->tpFailureCause = TP_NO_ERROR;
        }
    }

    if( RESULT_CODE_OK == resultCode)
    {
        if( octetPos < tpduLength)
        {
            ReadBitData (octetPos, 0, 1, (Int8 *)&tsDrReq->smsProtocolIdPresent,   dataPtr);        /* PI*/
            ReadBitData (octetPos, 1, 1, (Int8 *)&tsDrReq->smsDataCodingSchemePresent,   dataPtr);
            ReadBitData (octetPos, 2, 1, (Int8 *)&tsDrReq->udlPresent,   dataPtr);
            ReadBitData (octetPos, 7, 1, (Int8 *)&extensionBit, dataPtr);

            octetPos++;
        }
        else
        {
            resultCode = VG_CMS_ERROR_INVALID_PDU_LENGTH;
        }
    }

    if( RESULT_CODE_OK == resultCode)
    {
        if( octetPos < tpduLength)
        {
            /* Read the complete TP-PI parameter*/
            while(  (extensionBit == TRUE) &&
                    (resultCode == RESULT_CODE_OK))
            {
                ReadBitData (octetPos, 7, 1, (Int8 *)&extensionBit, dataPtr);
                octetPos++;
                if( (extensionBit == TRUE) &&
                    (octetPos >= tpduLength) )
                {
                    resultCode = VG_CMS_ERROR_INVALID_PDU_LENGTH;
                }
            }
        }
        else
        {
            resultCode = VG_CMS_ERROR_INVALID_PDU_LENGTH;
        }
    }

    if( (resultCode == RESULT_CODE_OK) &&
        (tsDrReq->smsProtocolIdPresent == TRUE) )                                           /* PID */
    {
        if( octetPos >= tpduLength)
        {
            resultCode = VG_CMS_ERROR_INVALID_PDU_LENGTH;
        }
        else
        {
            tsDrReq->smsProtocolId.protocolMeaning = (ProtocolMeaning)(dataPtr[octetPos] & 0xe0);
            switch (tsDrReq->smsProtocolId.protocolMeaning)
            {
                /*
                ** For PM_SM_AL_PROTOCOL and PM_TELEMATIC_INTERWORK need to decode
                ** 5 bits
                */
                case PM_SM_AL_PROTOCOL:
                case PM_TELEMATIC_INTERWORK:
                {
                    VG_READ_BIT_DATA(
                        octetPos, 0, 5,
                        (Int8 *) &tsDrReq->smsProtocolId.protocolId.data,
                        dataPtr);
                    break;
                }

                /*
                ** For PM_SM_FUNCTIONS, PM_RESERVED and PM_SC_SPECIFIC and anything
                ** else decode six bits.
                */
                default:
                {
                    VG_READ_BIT_DATA(
                        octetPos, 0, 6,
                        (Int8 *) &tsDrReq->smsProtocolId.protocolId.data,
                        dataPtr);
                    break;
                }
            }
            octetPos++;
        }
    }

    if( (resultCode == RESULT_CODE_OK) &&
        (tsDrReq->smsDataCodingSchemePresent == TRUE) )                                 /* DCS*/
    {
        if( octetPos >= tpduLength)
        {
            resultCode = VG_CMS_ERROR_INVALID_PDU_LENGTH;
        }
        else
        {
            tsDrReq->rawDcsValue = dataPtr[octetPos];
            DecodeSmsDataCodingScheme (dataPtr[octetPos], &(tsDrReq->smsDataCodingScheme));
            octetPos++;
        }
    }

    if( (resultCode == RESULT_CODE_OK) &&
        (tsDrReq->udlPresent == TRUE) )                                                 /* UD*/
    {
        /* Only decode Data if User Data Length given in PDU is valid*/
        actualUserDataOctets = (Int8)(tpduLength - (octetPos + 1));
        if (tsDrReq->userDataHeaderPresent == TRUE)
        {
            if (vgCheckUDHeader(dataPtr[octetPos+1], (Int8 *)&dataPtr[octetPos+2],
                                dataPtr[octetPos], (Int8 *)&dataPtr[octetPos+1]) == FALSE)
            {
                resultCode = VG_CMS_ERROR_INVALID_UDH;
            }
        }

        if (RESULT_CODE_OK == resultCode)
        {
            /* Decide the type of message decoding to perform on the SM data */
            if( (tsDrReq->smsDataCodingScheme.msgCoding == MSG_CODING_8BIT) ||
                (tsDrReq->smsDataCodingScheme.msgCoding == MSG_CODING_UCS2))
            {
                /* Calculate number of octets of 7-bit User Data */
                numUserDataOctets = dataPtr[octetPos];
                if (numUserDataOctets == actualUserDataOctets)
                {
                    /* Decode in octet format */
                    DecodeSmOctetFormat((Int8 *) &(dataPtr[octetPos]),
                                        (Int8 *) &tsDrReq->userDataLength,
                                        (Int8 *) &tsDrReq->userData[0],
                                        SMS_DELIVER_REPORT_MAX_USER_DATA_LENGTH);
                }
                else  /* expected number of User Data octets is incorrect */
                {
                    resultCode = VG_CMS_ERROR_INVALID_PDU_INCORRECT_UDL;
                }
            }
            else
            {
                /* Calculate number of octets of 7-bit User Data */
                numUserDataOctets = VG_7_BIT_USER_DATA_SIZE(dataPtr[octetPos]);
                if (numUserDataOctets == actualUserDataOctets)
                {
                    /*
                     * The Short message data is in the compressed format (see GSM 03.40 Annex 2).
                     * The data is now decompressed.
                     */
                    DecodeSmDefFormat ( (Int8 *) &(dataPtr[octetPos]),
                                        (Int8 *) &tsDrReq->userDataLength,
                                        (Int8 *) &tsDrReq->userData[0],
                                        tsDrReq->userDataHeaderPresent);
                }
                else  /* expected number of User Data octects is incorrect */
                {
                    resultCode = VG_CMS_ERROR_INVALID_PDU_INCORRECT_UDL;
                }
            }
        }
    }

    return (resultCode);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgBuildTsDeliverInd
 *
 * Parameters:  (In) data - a pointer to the start of the pdu data from which
 *              to extraxt the SC Address from.
 *              (Out) tsDeliverInd - pointer to the TS_DELIVER signal to
 *              build the SC Address information into.
 *
 * Returns:     result - flag to determine if deliver indirection has been
 *              successfully built from the pdu. result=FALSE indicates invalid
 *              PDU mode parameters.
 *
 * Description: Creates a TS_DELIVER_INDIRECTION from the pdu.
 *
 *------------------------------------------------------------------------*/
static ResultCode_t vgBuildTsDeliverInd (Int16 tpduLength, const Int8 *dataPtr, TsDeliverInd *tsDeliverInd)
{
    Int8          octetPos             = 0;
    Int8          numUserDataOctets    = 0;
    Int8          numAddressOctets     = 0;
    Int8          actualUserDataOctets = 0;
    Int8          mti;
    Int8          readDigits;
    ResultCode_t  resultCode = RESULT_CODE_OK;

    VG_READ_BIT_DATA(octetPos, 0, 2, (Int8*) &mti, dataPtr);                                    /* MTI  */
    FatalAssert( mti == SM_MTI_DELIVER);
    if( mti == SM_MTI_DELIVER)
    {
        VG_READ_BIT_DATA(octetPos, 2, 1, (Int8*) &tsDeliverInd->moreMsgsToSend, dataPtr);       /* MMS  */
        VG_READ_BIT_DATA(octetPos, 5, 1, (Int8*) &tsDeliverInd->statusReportInd, dataPtr);      /* SRI */
        VG_READ_BIT_DATA(octetPos, 6, 1, (Int8*) &tsDeliverInd->userDataHeaderPresent, dataPtr);/* UDHI  */
        VG_READ_BIT_DATA(octetPos, 7, 1, (Int8*) &tsDeliverInd->replyPath, dataPtr);            /* RP  */
        octetPos++;

        /* Inverse TP-MSS*/
        tsDeliverInd->moreMsgsToSend = !tsDeliverInd->moreMsgsToSend;

        /* only continue if address length byte does not exceed max length allowed */
        if (dataPtr[octetPos] <= VG_SMS_MAX_ADDRESS_LENGTH)
        {
            memset(&tsDeliverInd->smeAddr, 0, VG_SEMI_OCTET_SIZE(dataPtr[octetPos]));
            /* sets tpduInfoIsOk */
            resultCode = readAddressData (octetPos, &tsDeliverInd->smeAddr, dataPtr, &numAddressOctets);/* OA  */
            octetPos += numAddressOctets;
        }
        else
        {
            resultCode = VG_CMS_ERROR_INVALID_PDU_LENGTH;
        }
    }
    else
    {
        resultCode = VG_CMS_ERROR_INVALID_PDU_MTI;
    }

    if (RESULT_CODE_OK == resultCode)
    {
        tsDeliverInd->smsProtocolId.protocolMeaning = (ProtocolMeaning)(dataPtr[octetPos] & 0xe0);  /* PID */

        switch (tsDeliverInd->smsProtocolId.protocolMeaning)
        {
            /*
            ** For PM_SM_AL_PROTOCOL and PM_TELEMATIC_INTERWORK need to decode
            ** 5 bits
            */
            case PM_SM_AL_PROTOCOL:
            case PM_TELEMATIC_INTERWORK:
                {
                    VG_READ_BIT_DATA(octetPos, 0, 5, (Int8 *) &tsDeliverInd->smsProtocolId.protocolId.data, dataPtr);
                    break;
                }
            /*
            ** For PM_SM_FUNCTIONS, PM_RESERVED and PM_SC_SPECIFIC and anything
            ** else decode six bits.
            */
            default:
                {
                    VG_READ_BIT_DATA(octetPos, 0, 6, (Int8 *) &tsDeliverInd->smsProtocolId.protocolId.data, dataPtr);
                    break;
                }
        }
        octetPos++;

        tsDeliverInd->rawDcsValue = dataPtr[octetPos];
        DecodeSmsDataCodingScheme (dataPtr[octetPos], &(tsDeliverInd->smsDataCodingScheme));    /* DCS*/
        octetPos++;

        readDigits = ReadSemiOctetData( octetPos, (Int8 *) &tsDeliverInd->scTimeStamp[0],       /* SCTS*/
                                        TIMESTAMP_SIZE, dataPtr, FALSE);

        if( readDigits == TIMESTAMP_SIZE)
        {
            octetPos += (TIMESTAMP_SIZE / 2);
            /* Only decode Data if User Data Length given in PDU is valid against the
            ** length of the TPDU given in the command AT+CMG*=??
            ** Return error if UDL byte is not correct.
            */
            actualUserDataOctets = (Int8)(tpduLength - (octetPos + 1));

            if (tsDeliverInd->userDataHeaderPresent == TRUE)
            {
                if (vgCheckUDHeader(dataPtr[octetPos+1], (Int8 *)&dataPtr[octetPos+2],
                                    dataPtr[octetPos], (Int8 *)&dataPtr[octetPos+1]) == FALSE)
                {
                    resultCode = VG_CMS_ERROR_INVALID_UDH;
                }
            }
        }
        else
        {
            resultCode = RESULT_CODE_ERROR;
        }

        if (RESULT_CODE_OK == resultCode)
        {
            /* Decide the type of message decoding to perform on the SM data */
            if( (tsDeliverInd->smsDataCodingScheme.msgCoding == MSG_CODING_8BIT) ||
                (tsDeliverInd->smsDataCodingScheme.msgCoding == MSG_CODING_UCS2))
            {
                /* Calculate number of octets of 7-bit User Data */
                numUserDataOctets = dataPtr[octetPos];
                if (numUserDataOctets == actualUserDataOctets)
                {
                    /* Decode in octet format */
                    DecodeSmOctetFormat((Int8 *) &(dataPtr[octetPos]),
                                        (Int8 *) &tsDeliverInd->shortMsgLen,
                                        (Int8 *) &tsDeliverInd->shortMsgData[0],
                                        SMS_MAX_PDU_DATA_LEN);
                }
                else  /* expected number of User Data octets is incorrect */
                {
                    resultCode = VG_CMS_ERROR_INVALID_PDU_INCORRECT_UDL;
                }
            }
            else
            {
                /* Calculate number of octets of 7-bit User Data */
                numUserDataOctets = VG_7_BIT_USER_DATA_SIZE(dataPtr[octetPos]);
                if (numUserDataOctets == actualUserDataOctets)
                {
                    /*
                     * The Short message data is in the compressed format (see GSM 03.40 Annex 2).
                     * The data is now decompressed.
                     */
                    DecodeSmDefFormat ( (Int8 *) &(dataPtr[octetPos]),
                                        (Int8 *) &tsDeliverInd->shortMsgLen,
                                        (Int8 *) &tsDeliverInd->shortMsgData[0],
                                        tsDeliverInd->userDataHeaderPresent);
                }
                else  /* expected number of User Data octects is incorrect */
                {
                    resultCode = VG_CMS_ERROR_INVALID_PDU_INCORRECT_UDL;
                }
            }
        }
    }

    return (resultCode);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgBuildTsStatusReportInd
 *
 * Parameters:  (In) data - a pointer to the start of the pdu data from which
 *              to extraxt the SC Address from.
 *              (Out) tsDeliverInd - pointer to the TS_STATU_REPORT signal to
 *              build the SC Address information into.
 *
 * Returns:     result - flag to determine if deliver indirection has been
 *              successfully built from the pdu. result=FALSE indicates invalid
 *              PDU mode parameters.
 *
 * Description: Creates a TS_STATUS_REPORT_INDIRECTION from the pdu.
 *
 *------------------------------------------------------------------------*/
static ResultCode_t vgBuildTsStatusReportInd (Int16 tpduLength, const Int8 *dataPtr, TsStatusReportInd *tsSrInd)
{
    Int8          octetPos             = 0;
    Int8          numUserDataOctets    = 0;
    Int8          numAddressOctets     = 0;
    Int8          actualUserDataOctets = 0;
    Int8          mti                  = 0;
    Int8          status               = 0;
    Int8          readDigits           = 0;
    Boolean       extensionBit         = FALSE;
    ResultCode_t  resultCode           = RESULT_CODE_OK;

    VG_READ_BIT_DATA(octetPos, 0, 2, (Int8*) &mti, dataPtr);                                    /* MTI  */
    FatalAssert( mti == SM_MTI_STATUS_REPORT);
    if( mti == SM_MTI_STATUS_REPORT)
    {
        VG_READ_BIT_DATA(octetPos, 2, 1, (Int8*) &tsSrInd->moreMsgsToSend, dataPtr);            /* MMS  */
        VG_READ_BIT_DATA(octetPos, 5, 1, (Int8*) &tsSrInd->statusReportQual, dataPtr);          /* SQR */
        VG_READ_BIT_DATA(octetPos, 6, 1, (Int8*) &tsSrInd->userDataHeaderPresent, dataPtr);     /* UDHI  */
        octetPos++;

        /* Inverse TP-MSS*/
        tsSrInd->moreMsgsToSend = !tsSrInd->moreMsgsToSend;

        VG_READ_INTEGER_DATA( octetPos, &tsSrInd->msgRef, dataPtr);                             /* MR*/
        octetPos++;

        /* only continue if address length byte does not exceed max length allowed */
        if (dataPtr[octetPos] <= VG_SMS_MAX_ADDRESS_LENGTH)
        {
            memset(&tsSrInd->recipientAddr, 0, VG_SEMI_OCTET_SIZE(dataPtr[octetPos]));
            /* sets tpduInfoIsOk */
            resultCode = readAddressData (octetPos, &tsSrInd->recipientAddr, dataPtr, &numAddressOctets); /* RA  */
            octetPos += numAddressOctets;
        }
        else
        {
            resultCode = VG_CMS_ERROR_INVALID_PDU_LENGTH;
        }
    }
    else
    {
        resultCode = VG_CMS_ERROR_INVALID_PDU_MTI;
    }

    if (RESULT_CODE_OK == resultCode)
    {
        if( (tpduLength - octetPos) < (2*(TIMESTAMP_SIZE/2)+1) )
        {
            resultCode = VG_CMS_ERROR_INVALID_PDU_LENGTH;
        }
    }

    if (RESULT_CODE_OK == resultCode)
    {
        readDigits = ReadSemiOctetData( octetPos, (Int8 *) &tsSrInd->scTimeStamp[0],        /* SCTS*/
                                        TIMESTAMP_SIZE, dataPtr, FALSE);
        if( readDigits == TIMESTAMP_SIZE)
        {
            octetPos += (TIMESTAMP_SIZE / 2);
        }
        else
        {
            resultCode = RESULT_CODE_ERROR;
        }
    }

    if (RESULT_CODE_OK == resultCode)
    {
        readDigits = ReadSemiOctetData( octetPos, (Int8 *) &tsSrInd->receptionTime[0],      /* DT*/
                                        TIMESTAMP_SIZE, dataPtr, FALSE);
        if( readDigits == TIMESTAMP_SIZE)
        {
            octetPos += (TIMESTAMP_SIZE / 2);
        }
        else
        {
            resultCode = RESULT_CODE_ERROR;
        }
    }

    if (RESULT_CODE_OK == resultCode)
    {
        VG_READ_INTEGER_DATA( octetPos, &status, dataPtr);                                      /* ST*/
        octetPos++;
        tsSrInd->smsStatus = (SmsStatus)status;

        /* all the next parameters are optionals, set default value*/
        tsSrInd->dcsPresent = FALSE;
        tsSrInd->pidPresent = FALSE;
        tsSrInd->udlPresent = FALSE;
        tsSrInd->smsProtocolId.protocolMeaning = PM_SM_AL_PROTOCOL;
        tsSrInd->smsProtocolId.protocolId.data = 0x00;
        DecodeSmsDataCodingScheme( 0x00, &tsSrInd->smsDataCodingScheme);
        tsSrInd->shortMsgLen    = 0;
        tsSrInd->piLen          = 0;

        if( octetPos < tpduLength) /* Some data remains*/
        {
            ReadBitData (octetPos, 0, 1, (Int8 *)&tsSrInd->pidPresent,   dataPtr);              /* PI*/
            ReadBitData (octetPos, 1, 1, (Int8 *)&tsSrInd->dcsPresent,   dataPtr);
            ReadBitData (octetPos, 2, 1, (Int8 *)&tsSrInd->udlPresent,   dataPtr);
            ReadBitData (octetPos, 7, 1, (Int8 *)&extensionBit, dataPtr);

            tsSrInd->piData[0] = dataPtr[octetPos];
            tsSrInd->piLen = 1;
            octetPos++;

            /* Read the complete TP-PI parameter*/
            while(  (extensionBit == TRUE) &&
                    (resultCode == RESULT_CODE_OK))
            {
                tsSrInd->piData[ tsSrInd->piLen++] = dataPtr[octetPos];

                ReadBitData (octetPos, 7, 1, (Int8 *)&extensionBit, dataPtr);
                octetPos++;
                if( (extensionBit == TRUE) &&
                    (   (octetPos >= tpduLength) ||
                        (tsSrInd->piLen >= SMS_MAX_SMSR_DATA_LEN)
                    ) )
                {
                    resultCode = VG_CMS_ERROR_INVALID_PDU_LENGTH;
                }
            }

            if( (resultCode == RESULT_CODE_OK) &&
                (tsSrInd->pidPresent == TRUE) )                                                 /* PID */
            {
                if( octetPos >= tpduLength)
                {
                    resultCode = VG_CMS_ERROR_INVALID_PDU_LENGTH;
                }
                else
                {
                    tsSrInd->smsProtocolId.protocolMeaning = (ProtocolMeaning)(dataPtr[octetPos] & 0xe0);
                    switch (tsSrInd->smsProtocolId.protocolMeaning)
                    {
                        /*
                        ** For PM_SM_AL_PROTOCOL and PM_TELEMATIC_INTERWORK need to decode
                        ** 5 bits
                        */
                        case PM_SM_AL_PROTOCOL:
                        case PM_TELEMATIC_INTERWORK:
                            {
                                VG_READ_BIT_DATA(
                                    octetPos, 0, 5,
                                    (Int8 *) &tsSrInd->smsProtocolId.protocolId.data,
                                    dataPtr);
                                break;
                            }
                        /*
                        ** For PM_SM_FUNCTIONS, PM_RESERVED and PM_SC_SPECIFIC and anything
                        ** else decode six bits.
                        */
                        default:
                            {
                                VG_READ_BIT_DATA(
                                    octetPos, 0, 6,
                                    (Int8 *) &tsSrInd->smsProtocolId.protocolId.data,
                                    dataPtr);
                                break;
                            }
                    }
                    octetPos++;
                }
            }

            if( (resultCode == RESULT_CODE_OK) &&
                (tsSrInd->dcsPresent == TRUE) )                                                 /* DCS*/
            {
                if( octetPos >= tpduLength)
                {
                    resultCode = VG_CMS_ERROR_INVALID_PDU_LENGTH;
                }
                else
                {
                    DecodeSmsDataCodingScheme (dataPtr[octetPos], &(tsSrInd->smsDataCodingScheme));
                    octetPos++;
                }
            }

            if( (resultCode == RESULT_CODE_OK) &&
                (tsSrInd->udlPresent == TRUE) )                                                 /* UD*/
            {
                /* Only decode Data if User Data Length given in PDU is valid*/
                actualUserDataOctets = (Int8)(tpduLength - (octetPos + 1));
                if (tsSrInd->userDataHeaderPresent == TRUE)
                {
                    if (vgCheckUDHeader(dataPtr[octetPos+1], (Int8 *)&dataPtr[octetPos+2],
                                        dataPtr[octetPos], (Int8 *)&dataPtr[octetPos+1]) == FALSE)
                    {
                        resultCode = VG_CMS_ERROR_INVALID_UDH;
                    }
                }

                if (RESULT_CODE_OK == resultCode)
                {
                    /* Decide the type of message decoding to perform on the SM data */
                    if( (tsSrInd->smsDataCodingScheme.msgCoding == MSG_CODING_8BIT) ||
                        (tsSrInd->smsDataCodingScheme.msgCoding == MSG_CODING_UCS2))
                    {
                        /* Calculate number of octets of 7-bit User Data */
                        numUserDataOctets = dataPtr[octetPos];
                        if (numUserDataOctets == actualUserDataOctets)
                        {
                            /* Decode in octet format */
                            DecodeSmOctetFormat((Int8 *) &(dataPtr[octetPos]),
                                                (Int8 *) &tsSrInd->shortMsgLen,
                                                (Int8 *) &tsSrInd->shortMsgData[0],
                                                SMS_MAX_PDU_DATA_LEN);
                        }
                        else  /* expected number of User Data octets is incorrect */
                        {
                            resultCode = VG_CMS_ERROR_INVALID_PDU_INCORRECT_UDL;
                        }
                    }
                    else
                    {
                        /* Calculate number of octets of 7-bit User Data */
                        numUserDataOctets = VG_7_BIT_USER_DATA_SIZE(dataPtr[octetPos]);
                        if (numUserDataOctets == actualUserDataOctets)
                        {
                            /*
                             * The Short message data is in the compressed format (see GSM 03.40 Annex 2).
                             * The data is now decompressed.
                             */
                            DecodeSmDefFormat ( (Int8 *) &(dataPtr[octetPos]),
                                                (Int8 *) &tsSrInd->shortMsgLen,
                                                (Int8 *) &tsSrInd->shortMsgData[0],
                                                tsSrInd->userDataHeaderPresent);
                        }
                        else  /* expected number of User Data octects is incorrect */
                        {
                            resultCode = VG_CMS_ERROR_INVALID_PDU_INCORRECT_UDL;
                        }
                    }
                }
            }
        }
    }

    return (resultCode);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgBuildTsSubmitReq
 *
 * Parameters:  (In) data - a pointer to the start of the pdu data from which
 *              to extraxt the SC Address from.
 *              (Out) tsSubmitReq - pointer to the TS_SUBMIT signal to
 *              build the SC Address information into.
 *
 * Returns:     result - flag to determine if Submit Request has been
 *              successfully built from the pdu. result=FALSE indicates invalid
 *              PDU mode parameters.
 *
 * Description: Creates a TS_SUBMIT_REQUEST from the pdu.
 *
 *------------------------------------------------------------------------*/
static ResultCode_t vgBuildTsSubmitReq (Int16 tpduLength, const Int8 *dataPtr, TsSubmitReq *tsSubmitReq)
{
    Int8          octetPos             = 0;
    Int8          numUserDataOctets    = 0;
    Int8          numAddressOctets     = 0;
    Int8          actualUserDataOctets = 0;
    Int8          validityPeriodFormat;
    Int8          mti;
    ResultCode_t  resultCode = RESULT_CODE_OK;

    VG_READ_BIT_DATA(octetPos, 0, 2, (Int8*) &mti, dataPtr);                               /* MTI  */
    FatalAssert( mti == SM_MTI_SUBMIT);
    if( mti == SM_MTI_SUBMIT)
    {
        VG_READ_BIT_DATA(octetPos, 2, 1, (Int8*) &tsSubmitReq->rejectDuplicates, dataPtr);     /* RD  */
        /*
         * Thing to doesn't do :
         * VG_READ_BIT_DATA(octetPos, 3, 2, (Int8*) &tsSubmitReq->validityPeriodFormat, dataPtr);
         * On compiler like VS ,enum doesn't have size of 1 byte even when enum only
         * have few elements, so such code will malfunction
         */
        VG_READ_BIT_DATA(octetPos, 3, 2, (Int8*) &validityPeriodFormat, dataPtr); /* VPF */
        tsSubmitReq->validityPeriodFormat = (VpFormat)validityPeriodFormat;
        VG_READ_BIT_DATA(octetPos, 5, 1, (Int8*) &tsSubmitReq->statusReportReq, dataPtr);      /* SRR */
        VG_READ_BIT_DATA(octetPos, 6, 1, (Int8*) &tsSubmitReq->userDataHeaderPresent, dataPtr);/* UDHI  */
        VG_READ_BIT_DATA(octetPos, 7, 1, (Int8*) &tsSubmitReq->replyPath, dataPtr);            /* RP  */
        octetPos++;

        VG_READ_INTEGER_DATA(octetPos, (Int8*) &tsSubmitReq->msgRef, dataPtr);                 /* MR  */
        octetPos++;

        /* only continue if address length byte does not exceed max length allowed */
        if (dataPtr[octetPos] <= VG_SMS_MAX_ADDRESS_LENGTH)
        {
            memset(&tsSubmitReq->smeAddr, 0, VG_SEMI_OCTET_SIZE(dataPtr[octetPos]));
            /* sets tpduInfoIsOk */
            resultCode = readAddressData (octetPos, &tsSubmitReq->smeAddr, dataPtr, &numAddressOctets);/* OA  */
            octetPos += numAddressOctets;
        }
        else
        {
            resultCode = RESULT_CODE_ERROR;
        }
    }
    else
    {
        resultCode = VG_CMS_ERROR_INVALID_PDU_MTI;
    }

    if (RESULT_CODE_OK == resultCode)
    {
        tsSubmitReq->smsProtocolId.protocolMeaning = (ProtocolMeaning)(dataPtr[octetPos] & 0xe0);  /* PID */

        switch (tsSubmitReq->smsProtocolId.protocolMeaning)
        {
            /*
            ** For PM_SM_AL_PROTOCOL and PM_TELEMATIC_INTERWORK need to decode
            ** 5 bits
            */
            case PM_SM_AL_PROTOCOL:
            case PM_TELEMATIC_INTERWORK:
                {
                    VG_READ_BIT_DATA(octetPos, 0, 5, (Int8 *) &tsSubmitReq->smsProtocolId.protocolId.data, dataPtr);
                    break;
                }
            /*
            ** For PM_SM_FUNCTIONS, PM_RESERVED and PM_SC_SPECIFIC and anything
            ** else decode six bits.
            */
            default:
                {
                    VG_READ_BIT_DATA(octetPos, 0, 6, (Int8 *) &tsSubmitReq->smsProtocolId.protocolId.data, dataPtr);
                    break;
                }
        }
        octetPos++;

        tsSubmitReq->useRawDcs = FALSE;
        tsSubmitReq->rawDcs = 0;
        DecodeSmsDataCodingScheme (dataPtr[octetPos], &(tsSubmitReq->smsDataCodingScheme));

        octetPos++;

        switch (tsSubmitReq->validityPeriodFormat)
        {
            case VP_INTEGER_FORMAT:
                {
                    VG_READ_INTEGER_DATA (octetPos, (Int8 *)&tsSubmitReq->validityPeriodAsValue, dataPtr);
                    octetPos++;
                    break;
                }
            case VP_SEMI_OCTET:
            case VP_RESERVED:   /* Actually enhanced format */
                {
                    ReadSemiOctetData (octetPos, (Int8 *)tsSubmitReq->validityPeriodAsTime,
                        TIMESTAMP_SIZE, (Int8 *)dataPtr, FALSE);
                    octetPos += VG_SEMI_OCTET_SIZE(TIMESTAMP_SIZE);
                    break;
                }
            case VP_NOT_PRESENT:
                {
                    break;
                }
        }

        /* Only decode Data if User Data Length given in PDU is valid against the
        ** length of the TPDU given in the command AT+CMG*=??
        ** Return error if UDL byte is not correct.
        */
        actualUserDataOctets = (Int8)(tpduLength - (octetPos + 1));

        if (tsSubmitReq->userDataHeaderPresent == TRUE)
        {
            if (vgCheckUDHeader(dataPtr[octetPos+1], (Int8 *)&dataPtr[octetPos+2],
                                dataPtr[octetPos], (Int8 *)&dataPtr[octetPos+1]) == FALSE)
            {
                resultCode = VG_CMS_ERROR_INVALID_UDH;
            }
        }

        if (RESULT_CODE_OK == resultCode)
        {
            /* Decide the type of message decoding to perform on the SM data */
            if( (tsSubmitReq->smsDataCodingScheme.msgCoding == MSG_CODING_8BIT) ||
                (tsSubmitReq->smsDataCodingScheme.msgCoding == MSG_CODING_UCS2))
            {
                /* Calculate number of octets of 7-bit User Data */
                numUserDataOctets = dataPtr[octetPos];
                if (numUserDataOctets == actualUserDataOctets)
                {
                    /* Decode in octet format */
                    DecodeSmOctetFormat((Int8 *) &(dataPtr[octetPos]),
                                        (Int8 *) &tsSubmitReq->shortMsgLen,
                                        (Int8 *) &tsSubmitReq->shortMsgData[0],
                                        SMS_MAX_PDU_DATA_LEN);
                }
                else  /* expected number of User Data octets is incorrect */
                {
                    resultCode = VG_CMS_ERROR_INVALID_PDU_INCORRECT_UDL;
                }
            }
            else
            {
                /* Calculate number of octets of 7-bit User Data */
                numUserDataOctets = VG_7_BIT_USER_DATA_SIZE(dataPtr[octetPos]);
                if (numUserDataOctets == actualUserDataOctets)
                {
                    /*
                     * The Short message data is in the compressed format (see GSM 03.40 Annex 2).
                     * The data is now decompressed.
                     */
                    DecodeSmDefFormat ( (Int8 *) &(dataPtr[octetPos]),
                                        (Int8 *) &tsSubmitReq->shortMsgLen,
                                        (Int8 *) &tsSubmitReq->shortMsgData[0],
                                        tsSubmitReq->userDataHeaderPresent);
                }
                else  /* expected number of User Data octects is incorrect */
                {
                    resultCode = VG_CMS_ERROR_INVALID_PDU_INCORRECT_UDL;
                }
            }
        }
    }

    return (resultCode);
}

/*--------------------------------------------------------------------------
 *
 * Function:    buildSmsPduScAddress
 *
 * Parameters:  (In) data - a pointer to the start of the pdu data from which
 *              to extraxt the Service Centre Address from.
 *              (Out) scAddr - pointer to the start of the Service Centre
 *              Address info to build.
 *
 * Returns:     Boolean is address data is valid.
 *
 * Description: Builds the SC Address information from a pdu entered for all
 *              relevant MO SMS commands.
 *
 *-------------------------------------------------------------------------*/
static Boolean buildSmsPduScAddress(Int8 *data, SmsAddress *scAddr)
{
  Int8     i;
  Int8     tempScaLength = 0;
  Boolean  scAddrInfoIsOk = FALSE;

  if (*data < (VG_SMS_MAX_LEN_SCA_INFO / 2))
  {
    /* Length will be adjusted later when check for nibble padding */
    tempScaLength = 2 * (*data - 1);
    data++;

    /* decode TON/NPI byte */
    scAddr->numberingPlan = (NumberingPlan)((*data) & 0x0F);
    scAddr->typeOfNumber  = (TypeOfNumber)(((*data) >> 4) & 0x07);
    data++;

    /* decode Address digits */
    for (i=0; i<tempScaLength; i+=2)
    {
      scAddr->addressValue[i  ] = (*data) & 0x0F;
      scAddr->addressValue[i+1] = ((*data) >> 4) & 0x0F;
      data++;
    }

    /* check for padding nibble and set length accordingly */
    if (scAddr->addressValue[tempScaLength-1] == 0x0F)
    {
      scAddr->length = tempScaLength - 1;
    }
    else
    {
      scAddr->length = tempScaLength;
    }
    scAddrInfoIsOk = TRUE;
  }
  return (scAddrInfoIsOk);
}


/*==========================================================================
 *
 * Function:    getShortMessageSca
 *
 * Parameters:  (In) tpduType  - type
 *              (In) smTpdu - pointer to SimSmTpdu union
 *
 * Returns:     pointer to the scAddr
 *
 * Description: Returns pointer to the Service Centre address according
 *              to the message type.
 *
 *=========================================================================*/
static SmsAddress *getShortMessageSca (SimSmTpduType tpduType, SimSmTpdu *smTpdu)
{
  SmsAddress *addr_p = PNULL;
  switch (tpduType)
  {
    case SIM_SMTPDU_SUBMIT:
    {
      addr_p = &smTpdu->submit.scAddr;
      break;
    }
    case SIM_SMTPDU_COMMAND:
    {
      addr_p = &smTpdu->command.scAddr;
      break;
    }
    case SIM_SMTPDU_DELIVER:
    {
      addr_p = &smTpdu->deliver.scAddr;
      break;
    }
    case SIM_SMTPDU_STATUS_REPORT:
    {
      addr_p = &smTpdu->statusReport.scAddr;
      break;
    }
    default:
    {
      /* Unknown SmTpduType */
      FatalParam(tpduType, 0, 0);
      break;
    }
  }
  return (addr_p);
}

/*==========================================================================
 *
 * Function:    checkValidCharsInPduSmsMessage
 *
 * Parameters:  smsMessage
 *              smsLength
 *
 * Returns:     TRUE if ok.
 *
 * Description: Checks that the PDU sms message contains only hex
 *              characters.
 *
 *              This check is only done in development mode because we
 *              should never get any invalid chars in the message because
 *              the text entry mode (see rvcimxms.c) does not allow invalid
 *              chars.
 *
 *=========================================================================*/
static Boolean checkValidCharsInPduSmsMessage(  const Int8         *smsMessage,
                                                Int16               smsLength)
{
  Int32 i;
  Boolean ok = TRUE;

  for ( i=0; ((i<smsLength) && (ok)); i++)
  {
    if ( !isxdigit(toupper(smsMessage[i])) )
    {
      ok = FALSE;
    }
  }

  return (ok);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgCheckCommandSmsParams
 *
 * Parameters:  SMS context entity number
 *
 * Returns:     RESULT_CODE_OK if parameters are valid.
 *
 * Description: Checks TEXT Mode parameters are valid for
 *              SmsCommandInfo fields:
 *
 *              a) firstOctet
 *              b) commandType
 *              c) pid
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t checkCommandSmsParams(const VgmuxChannelNumber entity)
{
  SmsContext_t  *smsContext_p = ptrToSmsContext (entity);
  ResultCode_t  result = RESULT_CODE_OK;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  /* firstOctet parameter */
  if ((smsContext_p->command.firstOctet != 0x02) && (smsContext_p->command.firstOctet != 0x22))
  {
    /* error */
    result = VG_CMS_ERROR_SMSCMD_INVALID_FIRSTOCTET;
  }

  /* commandType parameter */
  /*lint -save -e685 Stop Lint Warning Relational operator '>' always evaluates to 'false' */
  if (smsContext_p->command.commandType > SMS_CMD_ENABLE_SRR)
  {
    /* error */
    result = VG_CMS_ERROR_SMSCMD_INVALID_CMDTYPE;
  }
  else
  {
    if (smsContext_p->command.commandType == SMS_CMD_ENQUIRY)
    {
      /* Check SRR bit is set true */
      if( DECODE_SRR(smsContext_p->command.firstOctet) != 1)
      {
        /* error */
        result = VG_CMS_ERROR_SMSCMD_SRR_BIT_NOT_SET;
      }
    }
    else
    {
      /* Check SRR bit is set FALSE for remaining command types */
      if( DECODE_SRR(smsContext_p->command.firstOctet) != 0)
      {
        /* error */
        result = VG_CMS_ERROR_SMSCMD_SRR_BIT_IS_SET;
      }
    }
  }
  /*lint -restore */

  return (result);
}


/*****************************************************************************
** SMS related functions for reading and writing bit and octet information  **
** from raw data                                                            **
*****************************************************************************/


/*--------------------------------------------------------------------------
 *
 * Function:        readAddressData
 *
 * Parameters:      see descrription
 *
 * Returns:         RESULT_CODE_OK if address data is valid.
 *
 *                  errors:
 *                  VG_CMS_ERROR_INVALID_PDU_ADDRESS
 *                  VG_CMS_ERROR_INVALID_PDU_DIGITS_IN_ADDRESS
 *
 * Description:     Read the data pointed to by [dataPtr] and offset by
 *                  [octetPosition] and store the data in the structure
 *                  pointed to by [addr].  See GSM 03.40 9.1.2.3
 *
 *                  In this format, the length byte contains the number of BCD
 *                  digits contained within the structure. The length of the
 *                  information element is deduced from this and returned.
 *
 *                  octetsRead is set to the length of the address data
 *                  information element.
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t readAddressData (Int8 octetPosition, SmsAddress *addr,
                                     const Int8 *dataPtr,
                                     Int8 *octetsRead)
{
  Int8 count      = 0;
  Int8 octetPos   = octetPosition;
  Int8 digitsRead = 0;
  ResultCode_t resultCode = RESULT_CODE_OK;

  VG_READ_INTEGER_DATA(octetPos, &addr->length, dataPtr);
  octetPos++;

  VG_READ_BIT_DATA(octetPos, 0, 4, (Int8 *) &addr->numberingPlan, dataPtr);
  VG_READ_BIT_DATA(octetPos, 4, 3, (Int8 *) &addr->typeOfNumber, dataPtr);
  octetPos++;

  digitsRead = ReadSemiOctetData (octetPos, (Int8 *) &addr->addressValue[0],
                                  addr->length, (Int8 *)/*break constness!*/dataPtr, TRUE);
  /*for mantis 151602, it is convenient to copy sms from draft mailbox to sim
     when user send SMS with AT+CMGS, if not input address, network will return error*/
  if (digitsRead > 0)
  {
    /* check address digits are in valid range */
    while ((count < digitsRead) && (RESULT_CODE_OK == resultCode))
    {
      if (addr->addressValue[count] > 0x0F)
      {
        resultCode = VG_CMS_ERROR_INVALID_PDU_DIGITS_IN_ADDRESS;
      }
      count++;
    }
  }

  /* return number of octets read */
  *octetsRead = (2 + VG_SEMI_OCTET_SIZE(digitsRead));

  return (resultCode);
}



/*==========================================================================
 *
 * Function:    isValidAddressDigit
 *
 * Parameters:  (in)  digit - the digit to test
 *              (out) index - index into the array of valid digits
 *
 * Returns:     TRUE if digit is valid
 *
 * Description: Tests a digit to see if it is in the set of valid digits
 *              for use in an SMS address.
 *
 * Currently these are the valid address digits 0123456789*#ABC
 * But CHECK in smsValidDigits to make sure they haven't changed.
 *
 *=========================================================================*/
static Boolean isValidAddressDigit(Char digit, Int8* index)
{
  Boolean validDigit = FALSE;
  Int16 numDigits = (Int16)strlen((char*)smsValidDigits);

  *index = 0;
  while ( (*index < numDigits) &&
          (digit != smsValidDigits[*index]) )
  {
    (*index)++;
  }
  if ( *index <  numDigits)
  {
    validDigit = TRUE;
  }

  return (validDigit);
}

/*==========================================================================
 *
 * Function:    isValidTimestampDigit
 *
 * Parameters:  (in)  digit - the digit to test
 *              (out) index - index into the array of valid digits
 *
 * Returns:     TRUE if digit is valid
 *
 * Description: Tests a digit to see if it is in the set of valid digits
 *              for use in an SMS timestamp.
 *
 * Currently these are the valid timestamp digits 0123456789
 *
 *=========================================================================*/
static Boolean isValidTimestampDigit(Char digit, Int8* index)
{
  Boolean validDigit = FALSE;
  Int16 numDigits = (Int16)strlen((char*)smsTimestampValidDigits);

  *index = 0;
  while ( (*index < numDigits) &&
          (digit != smsTimestampValidDigits[*index]) )
  {
    (*index)++;
  }
  if ( *index <  numDigits)
  {
    validDigit = TRUE;
  }

  return (validDigit);
}



/*--------------------------------------------------------------------------
 *
 * Function:        resetSca
 *
 * Parameters:      None
 *
 * Returns:         Nothing
 *
 * Description:     Sets default sca when no NVRAM access
 *
 *-------------------------------------------------------------------------*/

static void resetSca (void)
{
  SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext();
  Int8               *data = smsCommonContext_p->sca.addressValue;
  Char               *address = (Char*)VG_SMS_DEFAULT_SCA_NUMBER;
  Int8               len = (Int8)strlen((char*)address);

  smsCommonContext_p->sca.typeOfNumber  = NUM_INTERNATIONAL;
  smsCommonContext_p->sca.numberingPlan = PLAN_ISDN;
  smsCommonContext_p->sca.length        = len;

  while (len--)
  {
    *data++ = *address++ - (Char)'0';
  }
  memcpy (&smsCommonContext_p->vgTempSca, &smsCommonContext_p->sca, sizeof (SmsAddress));
  FatalAssert(strlen(VG_SMS_DEFAULT_SCA_ALPHAID) < VG_ARRAY_LENGTH(smsCommonContext_p->scaAlphaId.data) );
  memcpy (&smsCommonContext_p->scaAlphaId.data[0], VG_SMS_DEFAULT_SCA_ALPHAID, strlen(VG_SMS_DEFAULT_SCA_ALPHAID));
  smsCommonContext_p->scaAlphaId.length = (Int8)strlen(VG_SMS_DEFAULT_SCA_ALPHAID);
}




/*--------------------------------------------------------------------------
 *
 * Function:    putBcdMsb
 *
 * Parameters:  base - base address of the bcd array
 *              maxBaseLen - size of the array
 *              pos - index into the array
 *              val - value to write
 *
 * Returns:     Nothing
 *
 * Description: Used to pack data into a BCD array nibble by nibble
 *              by vgSmsUtilConvertSmsAddressToDialledBcdNum() function.
 *
 *-------------------------------------------------------------------------*/
static void putBcdMsb (Bcd *base, Int8 maxBaseLen, Int16 pos, Int8 val)
{
  if ((pos / 2) < maxBaseLen)
  {
    if (pos % 2 != 0)
    {
      base[pos/2] = (base[pos/2] & 0xF0) | (val & 0x0F);
    }
    else
    {
      base[pos/2] = (base[pos/2] & 0x0F) | ((val << 4) & 0xF0);
    }
  }
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

 /*--------------------------------------------------------------------------
 *
 * Function:       vgSmsGetShortMessageSmeAddress
 *
 * Parameters:     [in] sms_p - the SMS
 *
 * Returns:        Pointer to the sme address
 *
 * Description:    Returns pointer to sme address according to the type
 *                 of the smtpdu.
 *
 *-------------------------------------------------------------------------*/

const SmsAddress *vgSmsGetShortMessageSmeAddress (const VgDisplaySmsParam* sms_p)
{
    const SmsAddress *smeAddr_p = PNULL;

    switch(sms_p->tpduType)
    {
        case SIM_SMTPDU_DELIVER:
            {
                smeAddr_p = &sms_p->shortMessageTpdu.deliver.smeAddr;
                break;
            }
        case SIM_SMTPDU_SUBMIT:
            {
                smeAddr_p = &sms_p->shortMessageTpdu.submit.smeAddr;
                break;
            }
        case SIM_SMTPDU_COMMAND:
            {
                smeAddr_p = &sms_p->shortMessageTpdu.command.smeAddr;
                break;
            }
        case SIM_SMTPDU_STATUS_REPORT:
            {
                smeAddr_p = &sms_p->shortMessageTpdu.statusReport.scAddr;
                break;
            }
        default:
            {
                FatalParam (sms_p->tpduType, 0, 0);
//                smeAddr_p = PNULL;
                break;
            }
    }

    return (smeAddr_p);
}



/*--------------------------------------------------------------------------
 *
 * Function:    vgConvertSmStatusReportInd
 *
 * Parameters:  (In) src   - a pointer to a structure of type
 *              ApexSmStatusReportInd contining the
 *              SIG_APEX_SM_STATUS_REPORT_IND signal.
 *              (Out) dest - pointer to a structure of type TsStatusReportInd
 *              containing the status report recieved after sending an SM.
 *
 * Returns:     Nothing
 *
 * Description: Converts the ApexSmStatusReportInd to a TsStatusReportInd in
 *              preparation to be packed and sent to the CRM as a PDU.
 *
 *-------------------------------------------------------------------------*/
void vgSmsConvertSmStatusReportInd( const ApexSmStatusReportInd  *src,
                                    TsStatusReportInd            *dest )
{
    dest->msgRef                = src->messageRef;
    dest->moreMsgsToSend        = src->moreMsgsToSend ;
    dest->smsStatus             = src->smsStatus;
    dest->statusReportQual      = src->statusReportQual;
    dest->smsProtocolId         = src->smsProtocolId;
    dest->smsDataCodingScheme   = src->smsDataCodingScheme;
    dest->userDataHeaderPresent = src->userDataHeaderPresent;
    dest->shortMsgId            = src->messageRef;
    dest->smsCipher             = src->smsCipher;
    dest->pidPresent            = src->pidPresent;
    dest->dcsPresent            = src->dcsPresent;
    dest->udlPresent            = src->udlPresent;

    memcpy(&dest->scAddr,        &src->scAddr,        sizeof(SmsAddress)  );
    memcpy(&dest->recipientAddr, &src->recipientAddr, sizeof(SmsAddress)  );
    memcpy(dest->scTimeStamp,    src->scTimeStamp,    sizeof(SmsTimeStamp));
    memcpy(dest->receptionTime,  src->receptionTime,  sizeof(SmsTimeStamp));

    /* Copy SMS content*/
    dest->shortMsgLen = src->shortMsgLen;
    memcpy( dest->shortMsgData, src->shortMsgData, SMS_MAX_MSG_LEN * sizeof( Int8));
    dest->piLen = src->piLen;
    memcpy( dest->piData, src->piData, SMS_MAX_SMSR_DATA_LEN * sizeof( Int8));
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgConvertSmDeliveryInd
 *
 * Parameters:  (In) src   - a pointer to a structure of type ApexSmDeliveryInd
 *              contining the SIG_APEX_SM_DELIVERY_IND signal.
 *              (Out) dest - a pointer to a structure of type TsDeliverInd
 *              containing the Deliver type Short Message
 *
 * Returns:     Nothing
 *
 * Description: Converts the ApexSmDeliveryInd to a TsDeliverInd in
 *              preparation to be packed and sent to the CRM as a PDU.
 *
 *              Used by vgSmsUtilSendUnsolicitedSmDeliveryIndToCrm()
 *
 *-------------------------------------------------------------------------*/
void vgSmsConvertSmDeliveryInd (const ApexSmDeliveryInd *src, TsDeliverInd *dest)
{
  dest->statusReportInd       = FALSE;
  dest->replyPath             = src->replyPath ;
  dest->moreMsgsToSend        = src->moreMsgsToSend ;
  dest->shortMsgId            = src->shortMsgId ;
  dest->userDataHeaderPresent = src->userDataHeaderPresent ;
  dest->shortMsgLen           = src->shortMsgLen;

  memcpy(&dest->shortMsgData[0], &src->shortMsgData[0], dest->shortMsgLen) ;
  memcpy(&dest->scTimeStamp[0], &src->scTimeStamp[0], sizeof(SmsTimeStamp)) ;
  memcpy(&dest->smsProtocolId, &src->protocolId, sizeof(SmsProtocolId));
  memcpy(&dest->smeAddr, &src->smeAddr, sizeof(SmsAddress));
  memcpy(&dest->scAddr,  &src->scAddr,  sizeof(SmsAddress));
  memcpy(&dest->smsDataCodingScheme, &src->smsDataCodingScheme, sizeof(SmsDataCoding));
}



/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilConvertSmsAddressToDialledBcdNum
 *
 * Parameters:      SmsAddress
 *
 * Returns:         DialledBcdNum
 *
 * Description:     Converts an SmsAddress to a DialledBcdNum for use
 *                  with phonebook lookup that is used to convert a
 *                  telephone number into a name.
 *
 *-------------------------------------------------------------------------*/

DialledBcdNum vgSmsUtilConvertSmsAddressToDialledBcdNum(const SmsAddress *smsAddress)
{
  DialledBcdNum  bcdNum;
  Bcd            *bcdString_p = bcdNum.number;
  Int16          pos = 0;
  Int16          out = 0;
  Int8           val = 0;

  FatalAssert (NUM_ALPHANUMERIC != smsAddress->typeOfNumber);

  /* update the number plan in the output */
  bcdNum.numPlan = BCD_PLAN_E163_E164;

  /* Determine the type of number ---> Assume it is unknown
  ** if it isn't an INTERNATIONAL number.
  */
  switch (smsAddress->typeOfNumber)
  {
    case NUM_INTERNATIONAL:
    {
      bcdNum.type = NUM_TYPE_INTERNATIONAL;
      break;
    }

    case NUM_NATIONAL:
    {
      bcdNum.type = NUM_TYPE_NATIONAL;
      break;
    }

    case NUM_NETWORK_SPEC:
    {
      bcdNum.type = NUM_TYPE_NETWORK_SPEC;
      break;
    }

    case NUM_UNKNOWN:
    default:
    {
      bcdNum.type = NUM_TYPE_UNKNOWN;
      break;
    }
  }

  /* Pack the sms address digits into the BCD array */
  while ( (pos < smsAddress->length) )
  {
    val = smsAddress->addressValue[pos];
    if (val <= 9)
    {
      putBcdMsb ( bcdString_p, MAX_DIALLED_NO_LENGTH, out, val);
      ++out;
    }
    else
    {
      /* Invalid digit in smsAddress */
      WarnParam(val, pos, smsAddress->length);
    }
    ++pos;
  }

  /* Ensure that the number of digits is even by padding it with a filler character. */
  if ((out % 2) != 0 )
  {
    putBcdMsb ( bcdString_p, MAX_DIALLED_NO_LENGTH, out, DIALBCD_FILLER);
    ++out;
  }

  out /= 2;

  /* clip it to max output len */
  if (out > MAX_DIALLED_NO_LENGTH)
  {
    out = MAX_DIALLED_NO_LENGTH;
  }

  bcdNum.numberLength = (Int8) out;

  return bcdNum;
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilHasSmsStatusSimOverflowChanged
 *
 * Parameters:      none
 *
 * Returns:         Boolean
 *
 * Description:     Returns true if the SMS sim status has changed
 *                  e.g. if the SMS memory has become full
 *
 *                  Checks and updates a variable actualSmsStatusSIMOverflow
 *                  so can tell when status has changed.
 *
 *-------------------------------------------------------------------------*/
Boolean vgSmsUtilHasSmsStatusSimOverflowChanged (void)
{
  SmsCommonContext_t     *smsCommonContext_p = ptrToSmsCommonContext ();
  VgSmsStatusSIMOverflow status;
  Boolean                changed = FALSE;

  if (TRUE == smsCommonContext_p->smSimState.memCapExceeded)
  {
    status = VG_SIM_SMS_BUFFER_FULL_AND_WAIT;
  }
  else if (smsCommonContext_p->smSimState.usedRecords ==
           smsCommonContext_p->smSimState.numberOfSmsRecords)
  {
    status = VG_SIM_SMS_BUFFER_FULL;
  }
  else
  {
    status = VG_SIM_SMS_SPACE_AVAILABLE;
  }

  if( smsCommonContext_p->actualSmsStatusSIMOverflow != status )
  {
    smsCommonContext_p->actualSmsStatusSIMOverflow = status;
    changed = TRUE;
  }
  return (changed);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilTypeOfNumberToChar
 *
 * Parameters:      typeOfNumber - NUM_INTERNATIONAL or other
 *
 * Returns:         Character for the number type
 *
 * Description:     Return character depending on number type.
 *
 *-------------------------------------------------------------------------*/
Char vgSmsUtilTypeOfNumberToChar (TypeOfNumber typeOfNumber)
{
  Char retVal;

  switch (typeOfNumber)
  {
    case NUM_INTERNATIONAL:
    {
      retVal = VG_DIAL_NUMBER_INTERNATIONAL;
      break;
    }

    case NUM_NATIONAL:
    {
      retVal = VG_DIAL_NUMBER_NATIONAL;
      break;
    }
    case NUM_NETWORK_SPEC:
    {
      retVal = VG_DIAL_NUMBER_NET_SPECIFIC;
      break;
    }
    case NUM_UNKNOWN:
    default:
    {
      retVal = VG_DIAL_NUMBER_UNKNOWN;
      break;
    }
  } /* end switch */

  return retVal;
}




/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilSetSca
 *
 * Parameters:      entity - the SMS context entity number
 *                  newSca - the SCA
 *
 * Returns:         Nothing
 *
 * Description:     Sets the SMS SCA's
 *
 *-------------------------------------------------------------------------*/
void vgSmsUtilSetSca (const SimSmsParameters *smsParams)
{
  SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext();
  Int16     i;
  Boolean diff = FALSE;

  if (TRUE == smsParams->scAddrPresent)
  {
    /* set both SCA's */
    memcpy(&smsCommonContext_p->vgTempSca, &smsParams->scAddr, sizeof(SmsAddress));
    smsCommonContext_p->sca = smsCommonContext_p->vgTempSca;

    /* Detect blank alpha id's and set length to zero for them */
    memcpy(&smsCommonContext_p->scaAlphaId, &smsParams->alphaId, sizeof(smsParams->alphaId));
    for (i=1; i<smsCommonContext_p->scaAlphaId.length; i++)
    {
      if (smsCommonContext_p->scaAlphaId.data[i-1] != smsCommonContext_p->scaAlphaId.data[i])
      {
        diff = TRUE;
        break;
      }
    }
    /* If all characters in the string are the same then blank it because it
       is probably just rubbish. */
    if (FALSE == diff)
    {
      smsCommonContext_p->scaAlphaId.length = 0;
    }
  }
  else
  {
    resetSca (); /* set with default number, to avoid rubbish */
  }
}



/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsSetValidityPeriod
 *
 * Parameters:      Sim SMS parameters containing the VP
 *
 * Returns:         Nothing
 *
 * Description:     Sets the local VP
 *
 *-------------------------------------------------------------------------*/
void vgSmsSetValidityPeriod (const SimSmsParameters *smsParams)
{
  SmsCommonContext_t  *smsCommonContext_p = ptrToSmsCommonContext ();
  if (TRUE == smsParams->validityPeriodPresent)
  {
    smsCommonContext_p->validityPeriodValue = smsParams->validityPeriodAsValue;
  }
  else
  {
    smsCommonContext_p->validityPeriodValue = VG_SMS_DEFAULT_VALIDITYPERIOD;
  }

  if (TRUE == smsParams->protocolIdPresent)
  {
    memcpy (&smsCommonContext_p->protocolId,
             &smsParams->smsProtocolId,
              sizeof(SmsProtocolId));
  }
  else
  {
    smsCommonContext_p->protocolId.protocolMeaning = VG_SMS_TEXTMODE_DEFAULT_PROTOCOLMEANING;
    smsCommonContext_p->protocolId.protocolId.data = VG_SMS_TEXTMODE_DEFAULT_PROTOCOLID;
  }

  if (TRUE == smsParams->codingSchemePresent)
  {
    memcpy (&smsCommonContext_p->dataCodingScheme,
             &smsParams->smsDataCodingScheme,
              sizeof(SmsDataCoding));
  }
  else
  {
    smsCommonContext_p->dataCodingScheme.msgCoding = VG_SMS_TEXTMODE_DEFAULT_DCS_MSGCODING;
    smsCommonContext_p->dataCodingScheme.msgClass  = VG_SMS_TEXTMODE_DEFAULT_DCS_MSGCLASS;
    smsCommonContext_p->dataCodingScheme.compressedText       = VG_SMS_TEXTMODE_DEFAULT_DCS_COMPRESSEDTEXT;
    smsCommonContext_p->dataCodingScheme.msgWaitingIndPresent = VG_SMS_TEXTMODE_DEFAULT_DCS_MSGWAITINGINDPRESENT;
    smsCommonContext_p->dataCodingScheme.markedForAutomaticDeletion = VG_SMS_TEXTMODE_DEFAULT_DCS_MARKEDFORAUTOMATICDELETION;
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilGetSca
 *
 * Parameters:      None
 *
 * Returns:         Pointer to current SCA
 *
 * Description:     Gets current SCA
 *
 *-------------------------------------------------------------------------*/
void vgSmsUtilGetSca (SmsAddress *sca)
{
  SmsCommonContext_t  *smsCommonContext_p = ptrToSmsCommonContext ();
  memcpy(sca, &smsCommonContext_p->sca, sizeof(SmsAddress));
}



 /*--------------------------------------------------------------------------
 *
 * Function:        vgSmsGetValidityPeriod
 *
 * Parameters:      entity and VP output params
 *
 * Returns:         nothing
 *
 * Description:     Retrieves the validity period settings.
 *
 *-------------------------------------------------------------------------*/
void vgSmsGetValidityPeriod (VpFormat     *validityPeriodFormat,
                             Int8         *validityPeriodAsValue,
                             SmsTimeStamp  validityPeriodAsTime)
{
    SmsCommonContext_t  *smsCommonContext_p = ptrToSmsCommonContext ();

    FatalCheck(   DECODE_MTI(smsCommonContext_p->firstOctet) == SM_MTI_SUBMIT,
                  smsCommonContext_p->firstOctet, 0, 0);

    switch( DECODE_VPF(smsCommonContext_p->firstOctet))
    {
        case SM_VPF_NOT_PRESENT:
            {
                *validityPeriodFormat = VP_NOT_PRESENT;
            }
            break;

        case SM_VPF_RELATIVE:
            {
                *validityPeriodFormat = VP_INTEGER_FORMAT;
            }
            break;

        case SM_VPF_ABSOLUTE:
            {
                *validityPeriodFormat = VP_SEMI_OCTET;
            }
            break;

        case SM_VPF_ENCHANCED:
            {
                WarnFail ("Validity period format enchanted not supported");
            }
            break;

        default:
            {
                /* Invalid validity period format */
                FatalParam(DECODE_VPF(smsCommonContext_p->firstOctet), 0, 0);
            }
            break;
    }
    *validityPeriodAsValue = smsCommonContext_p->validityPeriodValue;
    memcpy( validityPeriodAsTime,
            smsCommonContext_p->validityPeriodAbsolute,
            sizeof(SmsTimeStamp));
}



/*--------------------------------------------------------------------------
*
* Function:      vgSmsIsNotBackwardCompatiblePduMode
*
* Parameters:    entity - SMS context entity number
*
* Returns:       TRUE if in backward compatible mode.
*
* Description:   +CSMS profile value = 128 forces PDU Mode operation to
*                be backward-compatible with older versions which do not
*                conform to GSM 07.05 Phase 2 output format.
*
*                Default profile value of +CSMS = 0 conforms to GSM 27.005.
*
*-------------------------------------------------------------------------*/
Boolean vgSmsIsNotBackwardCompatiblePduMode(const VgmuxChannelNumber entity)
{
  Boolean notBackwardCompatibleMode = TRUE;
  if ( VG_SMS_PDU_TPDU_ONLY == getProfileValue(entity, PROF_CSMS) )
  {
    notBackwardCompatibleMode = FALSE;
  }
  return (notBackwardCompatibleMode);
}



 /*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilIsInTextMode
 *
 * Parameters:      entity - SMS context entity number
 *
 * Returns:         Boolean true if in text mode (as opposded to PDU mode
 *
 * Description:     Whether in text mode or PDU mode
 *                  Set using the AT+CMGF command.
 *
 *-------------------------------------------------------------------------*/

Boolean vgSmsUtilIsInTextMode(const VgmuxChannelNumber entity)
{
  Boolean textMode = FALSE;
  if (getProfileValue(entity, PROF_CMGF) == PROF_CMGF_TEXT_MODE)
  {
    textMode = TRUE;
  }
  return (textMode);
}

 /*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilFormatData
 *
 * Parameters:      entity
 *
 * Returns:         TRUE if the SMS need to be concatened, FALSE otherwise
 *
 * Description:     Converts message body to correct format for transmission.
 *                  This function will convert data from the current
 *                  character set to the correct coding scheme.
 *-------------------------------------------------------------------------*/
Boolean vgSmsUtilFormatData(const VgmuxChannelNumber entity)
{
    SmsContext_t         *smsContext_p       = ptrToSmsContext (entity);
    SmsCommonContext_t   *smsCommonContext_p = ptrToSmsCommonContext();
    Int8                 *crOutputBuf_p      = getCrOutputBuffer(entity);
    Boolean               result = FALSE;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);

    FatalAssert (PNULL != crOutputBuf_p);
#endif
    if ( PNULL != crOutputBuf_p )
    {
        /* Convert entered data to correct format depending on specified DCS.
        * 'Default' coded DCS result in conversion to GSM, HEX and UCS2 result
        * in a conversion to HEX....
        */
        switch (smsCommonContext_p->dataCodingScheme.msgCoding)
        {
            case MSG_CODING_DEFAULT:
            {
                if( DECODE_UDH( smsCommonContext_p->firstOctet) == 0)
                {
                    /* Convert data from current character set to GSM.... */
                    smsContext_p->smsLength = vgMapTEToGsm( crOutputBuf_p,
                                                            VG_CR_CH_SET_OUTPUT_BUFFER_LEN,
                                                            &smsContext_p->smsMessage[0],
                                                            (Int16)smsContext_p->smsLength,
                                                            (VgCSCSMode)getProfileValue(entity, PROF_CSCS),
                                                            entity);
                }
                else /* User data header is set*/
                {
                    /* Convert data from current character set to GSM.... */
                    smsContext_p->smsLength = vgMapTEToHex( crOutputBuf_p,
                                                            VG_CR_CH_SET_OUTPUT_BUFFER_LEN,
                                                            &smsContext_p->smsMessage[0],
                                                            (Int16)smsContext_p->smsLength,
                                                            VG_AT_CSCS_HEX);
                }

                if( smsContext_p->smsLength > VG_SMS_MAX_MSG_LENGTH_GSM )
                {
                    result = TRUE;
                }
                break;
            }

            case MSG_CODING_RESERVED:
            case MSG_CODING_8BIT:
            case MSG_CODING_UCS2:
            {
                /* Convert data from current character set to GSM.... */
                smsContext_p->smsLength = vgMapTEToHex( crOutputBuf_p,
                                                        VG_CR_CH_SET_OUTPUT_BUFFER_LEN,
                                                        &smsContext_p->smsMessage[0],
                                                        (Int16)smsContext_p->smsLength,
                                                        VG_AT_CSCS_HEX);

                if( smsContext_p->smsLength > VG_SMS_MAX_MSG_LENGTH_HEX )
                {
                    result = TRUE;
                }
                break;
            }

            default:
            {
                /* Unexpected DCS coding scheme! */
                FatalParam(smsCommonContext_p->dataCodingScheme.msgCoding, 0, 0);
                break;
            }
        }

        /* Copy converted data back to structure.... */
        memcpy( &smsContext_p->smsMessage[0],
                crOutputBuf_p,
                (Int16)smsContext_p->smsLength);

        /* Free memory for buffer */
        resetCrOutputBuffer(entity);
    }
    return result;
}
 /*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilGetRecordStatusString
 *
 * Parameters:      index - index of status string
 *
 * Returns:         The status string
 *
 * Description:     Returns status string given index
 *                  e.g. "RED UNREAD"
 *
 *-------------------------------------------------------------------------*/
const Char* vgSmsUtilGetRecordStatusString(Int16 idx)
{
  FatalAssert( idx <  VG_ARRAY_LENGTH(smsRecordTypes) );
  return ((const Char*)smsRecordTypes[idx].name);
}

 /*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilGetRecordStatusStringGivenPduIndex
 *
 * Parameters:      index - index of status string
 *
 * Returns:         The status string
 *
 * Description:     Returns status string given index
 *                  e.g. "RED UNREAD"
 *                 (the pdu index swaps around index 0&1 and 2&3)
 *
 *-------------------------------------------------------------------------*/
const Char* vgSmsUtilGetRecordStatusStringGivenPduIndex(Int8 pduIdx)
{
  Int8 idx;
  FatalAssert( pduIdx < VG_ARRAY_LENGTH(smsRecordTypes) );
  idx = smsRecordTypes[pduIdx].convertFromPduIndex;
  return (smsRecordTypes[idx].name);
}

 /*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilConvertVgStatToSimSmStat
 *
 * Parameters:      index - pdu index
 *
 * Returns:         normal index
 *
 * Description:     Pdu index swaps around index 0&1 and 2&3.
 *
 *-------------------------------------------------------------------------*/
SimSmRecordStatus vgSmsUtilConvertVgStatToSimSmStat( VgSmsStatus vgStat)
{
    SimSmRecordStatus res = SIM_SMREC_RECEIVED_READ;
    switch( vgStat)
    {
        case VG_SMS_STATUS_UNREAD:
            {
                res = SIM_SMREC_RECEIVED_UNREAD;
            }
            break;

        case VG_SMS_STATUS_READ:
            {
                res = SIM_SMREC_RECEIVED_READ;
            }
            break;

        case VG_SMS_STATUS_UNSENT:
            {
                res = SIM_SMREC_ORIGINATED_NOT_SENT;
            }
            break;

        case VG_SMS_STATUS_SENT:
            {
                res = SIM_SMREC_ORIGINATED_SENT;
            }
            break;

        default:
            {
                FatalParam( vgStat, 0, 0);
            }
            break;
    }
    return res;
}

 /*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilConvertSimSmStatToVgStat
 *
 * Parameters:      index - pdu index
 *
 * Returns:         normal index
 *
 * Description:     Pdu index swaps around index 0&1 and 2&3.
 *
 *-------------------------------------------------------------------------*/
VgSmsStatus vgSmsUtilConvertSimSmStatToVgStat( SimSmRecordStatus smStat)
{
    VgSmsStatus res = VG_SMS_STATUS_READ;

    switch( smStat)
    {
        case SIM_SMREC_RECEIVED_UNREAD:
            {
                res = VG_SMS_STATUS_UNREAD;
            }
            break;

        case SIM_SMREC_RECEIVED_READ:
            {
                res = VG_SMS_STATUS_READ;
            }
            break;

        case SIM_SMREC_ORIGINATED_NOT_SENT:
            {
                res = VG_SMS_STATUS_UNSENT;
            }
            break;

        case SIM_SMREC_ORIGINATED_SENT:
            {
                res = VG_SMS_STATUS_SENT;
            }
            break;

        default:
            {
                FatalParam( smStat, 0, 0);
            }
            break;
    }
    return res;
}

 /*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilConvertPduIdxToStatusIdx
 *
 * Parameters:      index - pdu index
 *
 * Returns:         normal index
 *
 * Description:     Pdu index swaps around index 0&1 and 2&3.
 *
 *-------------------------------------------------------------------------*/
Int8 vgSmsUtilConvertPduIdxToStatusIdx(Int8 pduIdx)
{
  FatalAssert (pduIdx < VG_ARRAY_LENGTH(smsRecordTypes));
  return (smsRecordTypes[pduIdx].convertFromPduIndex);
}

 /*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilConvertPduIdxToSimAccessType
 *
 * Parameters:      index - pdu index
 *                  simAccessType - output of sim access type
 *
 * Returns:         Boolean true if ok.
 *
 * Description:     Converts from pdu index to Sim Access Type
 *
 *-------------------------------------------------------------------------*/
Boolean vgSmsUtilConvertPduIdxToSimAccessType(Int32 pduIdx, SmsSimAccessType *simAccessType)
{
  Boolean ok = FALSE;
  Int8    idx;
  if ( pduIdx < VG_ARRAY_LENGTH(smsRecordTypes) )
  {
    idx = smsRecordTypes[pduIdx].convertFromPduIndex;
    *simAccessType = smsRecordTypes[idx].simAccessValue;
    ok = TRUE;
  }
  return (ok);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilConvertStatusStringToSimAccessType
 *
 * Parameters:      index - pdu index
 *                  simAccessType - output of sim access type
 *
 * Returns:         Boolean true if ok.
 *
 * Description:     as it says!
 *
 *-------------------------------------------------------------------------*/
Boolean vgSmsUtilConvertStatusStringToSimAccessType (Char *statusString, SmsSimAccessType *simAccessType)
{
  Boolean found = FALSE;
  Int8    index = 0;

  vgConvertStringToUpper(statusString, statusString);

  while ( (FALSE == found) &&
          (index < VG_ARRAY_LENGTH(smsRecordTypes)) )
  {
    if ( 0 == strcmp((char*)smsRecordTypes[index].name, (char*)statusString) )
    {
      *simAccessType = smsRecordTypes[index].simAccessValue;
      found = TRUE;
    }
    index++;
  }
  return (found);
}


 /*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilConvertDelFlagToSimAccessType
 *
 * Parameters:      delFlag - delete flag
 *                  simAccessType - output of sim access type
 *
 * Returns:         Boolean true if ok.
 *
 * Description:     Converts from delFlag to Sim Access Type, as described
 *                  in 27.005.
 *                  <delFlag>
 *                  1 - delete all read (READ)
 *                  2 - delete all read and sent (SENT)
 *                  3 - delete all read, sent and unsent (UNSENT)
 *                  4 - delete all read, sent, unsent and unread (UNREAD)
 *
 *                  To make the implementation easier - every value of <delFlag>
 *                  will denote particular SMS type as specified above and always
 *                  all SMS' types matching that and lower <delFlag> values will be
 *                  deleted - read/delete will be run until delFlag is set to 0
 *
 *-------------------------------------------------------------------------*/
Boolean vgSmsUtilConvertDelFlagToSimAccessType (Int32 delFlag, SmsSimAccessType *simAccessType)
{
  Boolean ok = TRUE;

  switch (delFlag)
  {
    case 1:
    {
      *simAccessType = SM_NEXT_READ;
      break;
    }
    case 2:
    {
      *simAccessType = SM_NEXT_ORIGINATED_SENT;
      break;
    }
    case 3:
    {
      *simAccessType = SM_NEXT_ORIGINATED_NOT_SENT;
      break;
    }
    case 4:
    {
      *simAccessType = SM_PREVIEW_NEXT_UNREAD;
      break;
    }
    default:
    {
      ok = FALSE;
      break;
    }
  }
  return (ok);
}


/*==========================================================================
 *
 * Function:    vgSmsConvertRequestError
 *
 * Parameters:  request status
 *
 * Returns:     Nothing
 *
 * Description: Converts the supplied error code into something the
 *              CR Manager understands.
 *
 *=========================================================================*/

ResultCode_t vgSmsUtilConvertRequestError (const VgmuxChannelNumber entity,
                                            SmRequestStatus status)
{

  SimLockGenericContext_t   *simLockGenericContext_p = ptrToSimLockGenericContext();
  ResultCode_t              errorNum = VG_CMS_ERROR_UNKNOWN;

  PARAMETER_NOT_USED(entity);

  switch (status)
  {
    case SM_REQ_OK:
    {
      break;
    }
    case SM_REQ_SM_NOT_READY:
    /* job103566: implement new BL status values */
    case SM_REQ_SM_SEND_READY:
    case SM_REQ_SM_SNDRCV_READY:
    {
      /* This indicates a range of errors.  Now extrapolate the
         real error locally. */
      switch (simLockGenericContext_p->simState)
      {
        case VG_SIM_PIN:
        {
          errorNum = VG_CMS_ERROR_SIM_PIN_NECESSARY;
          break;
        }
        case VG_SIM_PUK:     /* fall through... */
        case VG_SIM_PIN2:    /* fall through... */
        case VG_SIM_PUK2:    /* fall through... */
        default:
        {
          errorNum = VG_CMS_ERROR_SIM_NOT_READY;
          break;
        }
      }
      break;
    }
    case SM_REQ_NO_FREE_RECORD:
    {
      errorNum = VG_CMS_ERROR_MEMORY_FULL;
      break;
    }
    case SM_REQ_RECORD_NOT_FOUND:
    {
      errorNum = VG_CMS_ERROR_INVALID_MEMORY_INDEX;
      break;
    }
    case SM_REQ_SIM_ERROR:
    {
      errorNum = VG_CMS_ERROR_SIM_FAILURE;
      break;
    }
    case SM_REQ_PS_ERROR:
    {
      errorNum = VG_CMS_ERROR_PS_BUSY;
      break;
    }
    case SM_REQ_INVALID_RECORD:
    {
      errorNum = VG_CMS_ERROR_INVALID_MEMORY_INDEX;
      break;
    }
    case SM_REQ_CONCAT_LINK_ERROR:
    {
      break;
    }
    case SM_REQ_NOT_SUBMIT_RECORD:
    {
      errorNum = VG_CMS_ERROR_OP_NOT_ALLOWED;
      break;
    }
    case SM_REQ_NRAM_ERROR:
    {
      errorNum = VG_CMS_ERROR_MEMORY_FAILURE;
      break;
    }
    default:
    {
      errorNum = VG_CMS_ERROR_UNKNOWN;
      break;
    }
  }

  return (errorNum);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilConvertPduIdxToSmsStatus
 *
 * Parameters:      index - pdu index
 *                  smsStatus - output of sim record status
 *
 * Returns:         Boolean true if ok.
 *
 * Description:     Converts from pdu index to Sim record status
 *
 *-------------------------------------------------------------------------*/
Boolean vgSmsUtilConvertPduIdxToSmsStatus(Int32 pduIdx, SimSmRecordStatus *smsStatus)
{
  Boolean ok = FALSE;
  Int8    idx;
  if ( pduIdx < VG_ARRAY_LENGTH(smsStatusTypes) )
  {
    idx = smsStatusTypes[pduIdx].convertFromPduIndex;
    *smsStatus = smsStatusTypes[idx].simRecordStatus;
    ok = TRUE;
  }
  return (ok);
}

 /*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilConvertStatusStringToSmsStatus
 *
 * Parameters:      index - pdu index
 *                  smsStatus - output of sim record status
 *
 * Returns:         Boolean true if ok.
 *
 * Description:     Converts from Status String to Sim record status
 *
 *-------------------------------------------------------------------------*/
Boolean vgSmsUtilConvertStatusStringToSmsStatus (Char *statusString, SimSmRecordStatus *smsStatus)
{
  Boolean found = FALSE;
  Int8    index = 0;

  vgConvertStringToUpper(statusString, statusString);

  while ( (FALSE == found) &&
          (index < VG_ARRAY_LENGTH(smsStatusTypes)) )
  {
    if ( 0 == strcmp((char*)smsStatusTypes[index].name, (char*)statusString) )
    {
      *smsStatus = smsStatusTypes[index].simRecordStatus;
      found = TRUE;
    }
    index++;
  }
  return (found);
}



/* added for job132548 */
/*==========================================================================
 *
 * Function:    vgSmsUtilConvertNetworkRequestError
 *
 * Parameters:  rpCause
 *
 * Returns:     resultCode
 *
 * Description: Converts the supplied rpCause error code into something the
 *              CR Manager understands.
 *
 *=========================================================================*/

ResultCode_t vgSmsUtilConvertNetworkRequestError (GsmCause cause)
{
  ResultCode_t              errorNum;

  switch (cause)
  {
    case CAUSE_UNASSIGNED_NO:
      errorNum = VG_CMS_ERROR_UNASSIGNED_NO;
      break;

    case CAUSE_OPER_DETERM_BARRING:
      errorNum = VG_CMS_ERROR_OPER_DETERM_BARRING;
      break;

    case CAUSE_CALL_BARRED:
      errorNum = VG_CMS_ERROR_CALL_BARRED;
      break;

    case CAUSE_NETWORK_FAILURE:
      errorNum = VG_CMS_ERROR_NETWORK_ERROR;
      break;

    case CAUSE_CALL_REJECTED:
      errorNum = VG_CMS_ERROR_REJECTED;
      break;

    case CAUSE_CONGESTION:
      errorNum = VG_CMS_ERROR_CONGESTION;
      break;

    case CAUSE_DEST_OUT_OF_ORDER:
      errorNum = VG_CMS_ERROR_DEST_OUT_OF_ORDER;
      break;

    case CAUSE_UNIDENTIFIED_SUBSCRIBER:
      errorNum = VG_CMS_ERROR_UNIDENTIFIED_SUBSCRIBER;
      break;

    case CAUSE_FACILITY_REJECTED:
      errorNum = VG_CMS_ERROR_FACILITY_REJECTED;
      break;

    case CAUSE_UNKNOWN_SUBSCRIBER:
      errorNum = VG_CMS_ERROR_UNKNOWN_SUBSCRIBER;
      break;

    case CAUSE_NET_OUT_OF_ORDER:
      errorNum = VG_CMS_ERROR_NET_OUT_OF_ORDER;
      break;

    case CAUSE_TEMP_FAILURE:
      errorNum = VG_CMS_ERROR_TEMP_FAILURE;
      break;

    case CAUSE_SWITCH_CONGESTION:
      errorNum = VG_CMS_ERROR_SWITCH_CONGESTION;
      break;

    case CAUSE_RESOURCES_UNAV:
      errorNum = VG_CMS_ERROR_RESOURCES_UNAV;
      break;

    case CAUSE_REQ_FAC_NOT_SUBSCR:
      errorNum = VG_CMS_ERROR_REQ_FAC_NOT_SUBSCR;
      break;

    case CAUSE_REQ_FACIL_NOT_IMPL:
      errorNum = VG_CMS_ERROR_REQ_FACIL_NOT_IMPL;
      break;

    case CAUSE_INVALID_SM_TRANSFER_REF:
      errorNum = VG_CMS_ERROR_INVALID_SM_TRANSFER_REF;
      break;

    case CAUSE_INVALID_MSG_SEMANTIC:
      errorNum = VG_CMS_ERROR_INVALID_MSG_SEMANTIC;
      break;

    case CAUSE_MAND_IE_ERROR:
      errorNum = VG_CMS_ERROR_MAND_IE_ERROR;
      break;

    case CAUSE_MSG_NONEXISTENT:
      errorNum = VG_CMS_ERROR_MSG_NONEXISTENT;
      break;

    case CAUSE_MSG_GEN_ERROR:
      errorNum = VG_CMS_ERROR_MSG_GEN_ERROR;
      break;

    case CAUSE_IE_NONEXISTENT:
      errorNum = VG_CMS_ERROR_IE_NONEXISTENT;
      break;

    case CAUSE_PROTOCOL_ERROR:
      errorNum = VG_CMS_ERROR_PROTOCOL_ERROR;
      break;

    case CAUSE_INTERWORKING:
      errorNum = VG_CMS_ERROR_INTERWORKING;
      break;

    case NORMAL_RELEASE:
      errorNum = VG_CMS_ERROR_PS_BUSY;
      break;

    default:
#ifdef FEA_NFM
      /* For NFM feature, should indicate NFM error and left time value to AP.
       * MD returns cause code with left time value add to 0x8000, AP shall minus 0x8000 for left time value.
       * The max vaule of time left is 15360.
       */
      if((cause >= CAUSE_NFM_ERROR)  && (cause <= CAUSE_NFM_ERROR_END))
      {
          errorNum = VG_CMS_ERROR_NFM_ERROR_START + (cause - CAUSE_NFM_ERROR);
      }
      else
#endif
      {
          errorNum = VG_CMS_ERROR_PROTOCOL_ERROR;
      }
      break;
  }

  return (errorNum);
}


/*==========================================================================
 *
 * Function:    vgSmsUtilFormatConcatSmsStruct
 *
 * Parameters:  (InOut) structure - pointer to ciapSendReq structure
 *              (same as ciapStoreReq if concatenated SMS is being used)
 *
 * Returns:     TRUE if successful.
 *
 * Description: Formats concatenated sms sequence to be sent.
 *
 *=========================================================================*/
void vgSmsUtilInitConcatSmsStruct( const VgmuxChannelNumber entity)
{
    SmsContext_t* smsContext_p = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    smsContext_p->sendRetries              = 0;
    smsContext_p->concatSms.sequenceNumber = 0;
    smsContext_p->concatSms.sequenceLength = 0;
}



/*==========================================================================
 *
 * Function:    vgSmsUtilFormatConcatSmsStruct
 *
 * Parameters:  (In)  entity number
 *              (Out) structure - pointer to ciapSendReq structure
 *              (same as ciapStoreReq if concatenated SMS is being used)
 *
 * Returns:     Nothing
 *
 * Description: Formats concatenated sms sequence to be sent.
 *
 *=========================================================================*/

void vgSmsUtilFormatConcatSmsStruct ( const VgmuxChannelNumber entity, CiapSms *structure)
{
    SmsContext_t       *smsContext_p = ptrToSmsContext (entity);
    Int32              sequenceShift;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    /* copy destination address */
    memcpy(&structure->dest, &smsContext_p->da, sizeof(SmsAddress));

    /* copy timestamp */
    memcpy(&structure->timeStamp[0], &smsContext_p->smTimeStamp[0], sizeof( SmsTimeStamp));

    structure->status = smsContext_p->smStatus;

    /* get the Service Centre address */
    vgSmsUtilGetSca(&structure->sca);

    /*
    ** Check if this should be sent as concatenated SMS if
    ** message is too big to fit in one SMS.
    */
    if(smsContext_p->smsLength <= VG_SMS_MAX_MSG_LENGTH_GSM)
    {
        structure->msgLength = (Int8)smsContext_p->smsLength;
        structure->isConcat = FALSE;
        memcpy(structure->message, &smsContext_p->smsMessage[0], (size_t)smsContext_p->smsLength);
    }
    else
    {
        FatalAssert(cfRvSmHandleConcatSms == TRUE);
        sequenceShift = (smsContext_p->concatSms.sequenceNumber * VG_SMS_CONCAT_BLOCK);

        structure->isConcat = TRUE;
        structure->concatRef = smsContext_p->concatSms.concatRef;
        structure->seqLength = smsContext_p->concatSms.sequenceLength;
        structure->seqNumber = smsContext_p->concatSms.sequenceNumber + 1;

        if( (smsContext_p->smsLength - sequenceShift) >= VG_SMS_CONCAT_BLOCK)
        {
            structure->msgLength = VG_SMS_CONCAT_BLOCK;

            /* Increment sequence number for next time */
            smsContext_p->concatSms.sequenceNumber++;
        }
        else
        {
            /* this is the last part of the concatenated SMS... */
            structure->msgLength = (Int8)(smsContext_p->smsLength - sequenceShift);

            /*
            ** Set sequenceNumber to 0 to indicate (to the Cnf handler) that this is the
            ** last part of the concatenated SMS.
            */
            smsContext_p->concatSms.sequenceNumber = 0;
        }

        /* copy part of msg into dest */
        memcpy(structure->message, &smsContext_p->smsMessage[sequenceShift], structure->msgLength);
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsUtilEncodeTimestamp
 *
 * Parameters:  (In) stringIn - a pointer to an array of Char conatining a
 *              zero terminated ASCII string containing the timestamp
 *              (In) lengthIn - Int16 indicating the length of the stringIn
 *              parameter without null character. (Error if different of 20
 *              characters)
 *              (Out) out_p - Timestamp to set.
 *
 * Returns:     Boolean which is TRUE if stringIn has been converted into
 *              timestamp without error, else FALSE.
 *
 * Description: Convert an ASCII string into a timestamp.
 *
 *-------------------------------------------------------------------------*/
Boolean vgSmsUtilEncodeTimestamp(   const Char     *stringIn,
                                    Int16           lengthIn,
                                    SmsTimeStamp    out_p)
{
    Boolean result = TRUE;
    Int32   positionIn = 0;
    Int16   positionOut = 0;
    Int8    index;
    Int32   i;

    FatalAssert( PNULL != stringIn );
    FatalAssert( PNULL != out_p );

    if( lengthIn != VG_TIMESTAMP_SIZE)
    {
        result = FALSE;
    }
    else
    {
        /* Check separator characters*/
        if( (stringIn[2]    != '/') ||
            (stringIn[5]    != '/') ||
            (stringIn[8]    != ',') ||
            (stringIn[11]   != ':') ||
            (stringIn[14]   != ':') ||
            (   (stringIn[17]   != '+') &&
                (stringIn[17]   != '-')
            ) )
        {
            result = FALSE;
        }
    }

    for(    i=0, positionIn=0, positionOut=0;
            (result == TRUE) && (i<VG_TIMESTAMP_GROUP);
            i++, positionIn++)
    {
        if ( isValidTimestampDigit( stringIn[positionIn++], &index) )
        {
            out_p[ positionOut++] = index;
            if ( isValidTimestampDigit( stringIn[positionIn++], &index) )
            {
                out_p[ positionOut++] = index;
            }
            else
            {
                result = FALSE;
            }
        }
        else
        {
            result = FALSE;
        }
    }

    /* Parse timezone sign.
    ** Warning. Spec 23.040 is conflicting about the sign bit.
    ** Sign bit can be bit 7 of the seventh octet (...the first of the two semi octets, the first bit...)
    ** Or bit 3 of the seventh octet (...bit 3 of the seventh octet ...)
    ** See section 9.2.3.11
    ** We choose to use bit 7*/
    if( (result == TRUE) &&
        (stringIn[17] == '-'))
    {
        /* activate sign bit*/
        out_p[12] |= 0x08;
    }

    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsUtilEncodeSmsDigits
 *
 * Parameters:  (In) stringIn - a pointer to an array of Char conatining a
 *              zero terminated ASCII string containing only valid sms
 *              digits.
 *              (In) lengthIn - Int16 indicating the length of the stringOut
 *              array of Int8.
 *              (Out) addrOut_p - SmsAddress to set.
 *
 * Returns:     Boolean which is TRUE if stringIn has been converted into
 *              stringOut without error, else FALSE.
 *
 * Description: Uses the validDigits array to convert the ASCII digits in the
 *              stringIn parameter Char array into their BCD equivalents,
 *              which are their indexes in the validDigits array. Any
 *              character not found in the validDigits array, other than the
 *              international prefix of the CLIR override suffic, is in error.
 *
 *-------------------------------------------------------------------------*/

Boolean vgSmsUtilEncodeSmsDigits (const Char    *stringIn,
                                  Int16         lengthIn,
                                  SmsAddress    *addrOut_p)
{
  Boolean   result = TRUE;
  Int16     position = 0;
  Int16     length   = 0;
  Int8      index;

  FatalAssert( PNULL != stringIn );
  FatalAssert( PNULL != addrOut_p );

  if ( (PNULL != stringIn) && (PNULL != addrOut_p) )
  {
    addrOut_p->numberingPlan = PLAN_ISDN;
    if (stringIn[0] == INTERNATIONAL_PREFIX)
    {
      addrOut_p->typeOfNumber = NUM_INTERNATIONAL;
      position++;
    }
    else
    {
      addrOut_p->typeOfNumber = NUM_UNKNOWN;
    }

    while ((position < lengthIn) &&
           (result == TRUE))
    {
      if ( isValidAddressDigit(stringIn[position], &index) )
      {
        addrOut_p->addressValue[length] = index;
        length++;
        position++;
        if( length ==  SMS_MAX_ADDR_LEN)
        {
          result = FALSE;
        }
      }
      else
      {
        result = FALSE;
      }
    }
    addrOut_p->length = (Int8) length;
  }
  else
  {
    result = FALSE;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsUtilSendUnsolicitedSmDeliveryIndToCrm
 *
 * Parameters:  (In) smDeliveryInd - a pointer to a structure of type
 *              ApexSmDeliveryInd contining the SIG_APEX_SM_DELIVERY_IND
 *              signal to be sent to the DS
 *              entity - a entity to send some unsolicited event.
 *
 * Returns:     Nothing
 *
 * Description: Sends the incoming Short Message to the CRM. A conversion is
 *              performed depending on whether currently in Text or PDU Mode.
 *
 *-------------------------------------------------------------------------*/

void vgSmsUtilSendUnsolicitedSmDeliveryIndToCrm (const VgmuxChannelNumber entity,
                                                 const ApexSmDeliveryInd *smDeliveryInd)
{
    SmsCommonContext_t       *smsCommonContext_p = ptrToSmsCommonContext();
#if defined (FEA_PHONEBOOK)
    const SmsAddress*        smsAddress          = &smDeliveryInd->smeAddr;
#endif
    VgmuxChannelNumber       profileEntity       =  0;
    
#if defined (FEA_PHONEBOOK)
    DialledBcdNum            dialledNum;
    ResultCode_t             resultCode;

    if( (vgSmsUtilIsAlphaIdLookupEnabled() == TRUE) &&
        (NUM_ALPHANUMERIC != smsAddress->typeOfNumber) )
    {
        dialledNum = vgSmsUtilConvertSmsAddressToDialledBcdNum (smsAddress);

        /* Copy the ApexSmReadCnf so it's preserved for use in the
        ** phonebook cnf handler.
        */
        smsCommonContext_p->savedUnsolSmDeliveryInd = *smDeliveryInd;

        /* do the lookup in the phonebook */
        resultCode = vgSmsSigOutApexLmGetAlphaReq(  entity,
                                                    VG_SMS_LMGETALPHA_UNSOLICITED_CALL_ID,
                                                    &dialledNum);

        if (resultCode != RESULT_CODE_PROCEEDING)
        {
            /* Cannot do phonebook lookup, currently just print the CMT notification
            *  without alpha. Will be upgrade later.*/
            /* FatalParam(resultCode, dialledNum.numberLength, 0);*/

            for(    profileEntity = 0;
                    profileEntity < CI_MAX_ENTITIES;
                    profileEntity++)
            {
                /* failed to send the phonebook lookup so carry on without it... */
                if( (isEntityActive (profileEntity)) && (smsCommonContext_p->printCmtOnlyOnMmi == FALSE)) /* Print on all channel*/
                {
                    vgSmsPrintUnsolicitedSmDeliveryIndWithAlphaId (profileEntity, smDeliveryInd, PNULL/*alphaId*/);
                }
            }

            smsCommonContext_p->printCmtOnlyOnMmi = FALSE;
            /* Now we need to set the unsol event to free */
            vgSmsUtilSetUnsolicitedEventHandlerFree();
        }
    }
    else
#endif /* FEA_PHONEBOOK */

    {
        for(    profileEntity = 0;
                profileEntity < CI_MAX_ENTITIES;
                profileEntity++)
        {
            /* failed to send the phonebook lookup so carry on without it... */
            if( (isEntityActive (profileEntity)) && (smsCommonContext_p->printCmtOnlyOnMmi == FALSE)) /* Print on all channel*/
            {
                vgSmsPrintUnsolicitedSmDeliveryIndWithAlphaId (profileEntity, smDeliveryInd, PNULL/*alphaId*/);
            }
        }

        smsCommonContext_p->printCmtOnlyOnMmi = FALSE;
        /* Now we need to set the unsol event to free */
        vgSmsUtilSetUnsolicitedEventHandlerFree();
    }
}

/****************************************************************************
 *
 * Function:    vgSmsUtilIsValidMessageChar
 *
 * Parameters:  ch - character to check
 *
 * Returns:     TRUE if character is valid
 *
 * Description: Returns TRUE if this is a valid character to have in an
 *              sms message as entered at the terminal.
 *
 * There are two different behaviours depending on whether we are in
 * text mode or not:
 *
 * In text mode valid characters are any printable characters AND
 * the control characters in PROF_S3 and PROF_S4 e.g. <CR>
 *
 * In PDU mode any hexadecimal digit (A ? F, a ? f, or 0 ? 9) is valid.
 *
 ****************************************************************************/
Boolean vgSmsUtilIsValidMessageChar (const VgmuxChannelNumber entity, Char ch)
{
  Boolean validChar = FALSE;

  if ( vgSmsUtilIsInTextMode(entity) )
  {
    if ( (ch == getProfileValue(entity, PROF_S3)) ||
         (ch == getProfileValue(entity, PROF_S4)) ||
         (isprint(ch)) )
    {
      validChar = TRUE;
    }

    else
    {
      if ((getProfileValue(entity, PROF_CSCS) == VG_AT_CSCS_GSM) &&
          (ch < GSM_EXTENDED_CHARS))
      {
        validChar = TRUE;
      }
    }
  }
  else
  {
    if ( isxdigit(ch) )
    {
      validChar = TRUE;
    }
  }
  return (validChar);
}



/*===========================================================================
 *
 * Function:    vgSmsUtilProcessSmsText
 *
 * Parameters:  entity - SMS context entity number
 *
 * Returns:     Nothing
 *
 * Description: Sends the SMS once user has finished entering it.
 *              Called from the cimux once user has pressed CTRL+Z
 *              to finish entering message.
 *
 *==========================================================================*/
void vgSmsUtilProcessSmsText (const VgmuxChannelNumber entity)
{
    SmsCommonContext_t *smsCommonContext_p  = ptrToSmsCommonContext ();
    SmsContext_t       *smsContext_p        = ptrToSmsContext (entity);
    CiapSms            *sms_p               = PNULL;
    SimSmTpdu          *smsTpdu_p           = PNULL;
    TsDeliverReportReq *deliverReport_p     = PNULL;
    Int8                scAddrInfoLength    = 0;
    ResultCode_t        resultCode          = RESULT_CODE_ERROR;
    Boolean             stop                = FALSE;
    SimSmTpduType       tpduType;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);

#endif

    FatalAssert(  (VG_AT_SMS_CMGS == getCommandId (entity)) ||
                  (VG_AT_SMS_CMGC == getCommandId (entity)) ||
                  (VG_AT_SMS_CMGW == getCommandId (entity)) ||
                  (VG_AT_SMS_CNMA == getCommandId (entity)));


    vgSmsUtilInitConcatSmsStruct (entity);

    if ( vgSmsUtilIsInTextMode(entity) )
    {
        if( smsContext_p->validSms == TRUE)
        {
            KiAllocZeroMemory( sizeof( CiapSms), (void **)&sms_p);

            /* Convert SMS to correct character set.... */
            /* If this is a concatenated SMS then calc how many normal messages
            ** are needed to send it.
            */
            if( vgSmsUtilFormatData(entity) == TRUE )
            {
                if( (cfRvSmHandleConcatSms == TRUE) &&
                    (smsContext_p->cpmsWriteSend == VG_CPMS_STORAGE_SM))
                {
                    /* Calculate how many chunks are needed to send the concatenated message. */
                    smsContext_p->concatSms.sequenceLength = (Int8)
                        ((smsContext_p->smsLength + VG_SMS_CONCAT_BLOCK - 1 ) / VG_SMS_CONCAT_BLOCK);

                    smsContext_p->concatSms.concatRef = (Int8)rand();
                }
                else
                {
                    resultCode = VG_CMS_ERROR_INVALID_PDU_LENGTH;
                    stop = TRUE;
                }
            }

            if( stop == FALSE)
            {
                /* Process data according to current command.... */
                vgSmsUtilFormatConcatSmsStruct(entity, sms_p);
                /* Check that UDL is OK*/
                if( DECODE_UDH( smsCommonContext_p->firstOctet) != 0x00) /* If UDL present*/
                {
                    if( sms_p->message[0] > smsContext_p->smsLength)
                    {
                        resultCode = VG_CMS_ERROR_INVALID_PDU_INCORRECT_UDL;
                        stop = TRUE;
                    }
                    else if( vgCheckUDHeader(   sms_p->message[0], (Int8 *)&sms_p->message[1],
                                                (Int8)smsContext_p->smsLength, (Int8 *)&sms_p->message[0]) == FALSE)
                    {
                        resultCode = VG_CMS_ERROR_INVALID_UDH;
                        stop = TRUE;
                    }
                }
            }

            if( stop == FALSE)
            {
                switch ( getCommandId (entity) )
                {
                    case VG_AT_SMS_CMGS: /* send SMS */
                    {
                        resultCode = vgSmsSigOutApexSendSmsNormal(entity, sms_p);
                    }
                    break;

                    case VG_AT_SMS_CMGC: /* send SMS command */
                    {
                        /*
                        ** Check the TEXT MODE PARAMETERS entered for <fo>, <ct>, <pid>.
                        ** Set success = FALSE if there is an error
                        */
                        resultCode = checkCommandSmsParams(entity);
                        if (RESULT_CODE_OK == resultCode)
                        {
                            codePidFieldForApexSmCommandReq(entity);
                            resultCode = vgSmsSigOutApexSendCommandSmsNormal(entity, sms_p);
                        }
                    }
                    break;

                    case VG_AT_SMS_CMGW: /* write SMS message to store */
                    {
                        switch( smsContext_p->cpmsWriteSend)
                        {
                            case VG_CPMS_STORAGE_SM:
                            {
                                resultCode = vgSmsSigOutApexStoreSmsNormal(entity, sms_p, 0);
                            }
                            break;

                            default:
                            {
                                FatalParam( smsContext_p->cpmsWriteSend, 0, 0);
                            }
                            break;
                        }
                    }
                    break;

                    default:
                    {
                        /* ERROR - Invalid AT command sending SMS */
                        FatalParam(getCommandId(entity), entity, 0);
//                        resultCode = RESULT_CODE_ERROR;
                    }
                    break;
                } /* END switch */
            }

            KiFreeMemory( (void**)&sms_p);
        }
        else
        {
            resultCode = VG_CMS_ERROR_INVALID_PDU_HEXCHAR;
        }
    }
    else  /* PDU MODE */
    {
        if( (smsContext_p->validSms == TRUE) &&
            ((smsContext_p->smsLength % 2) == 0) )
        {
            if( getCommandId (entity) == VG_AT_SMS_CNMA)
            {
                if (smsCommonContext_p->msgToAck == TRUE)
                {
                    KiAllocZeroMemory( sizeof( TsDeliverReportReq), (void **)&deliverReport_p);

                    deliverReport_p->shortMsgId     = smsCommonContext_p->shortMsgId;
                    deliverReport_p->statusOfReport = smsCommonContext_p->msgStatus;

                    resultCode = vgSmsFormatDeliverReportFromPdu(   entity,
                                                                    smsContext_p->smsMessage,
                                                                    &smsContext_p->smsLength,
                                                                    deliverReport_p);
                    smsContext_p->smsLength = 0;

                    if ( RESULT_CODE_OK == resultCode )
                    {
                        resultCode = vgSmsSigOutApexSmDeliveryWithReportRspTpdu( entity, deliverReport_p );
                        smsCommonContext_p->msgToAck = FALSE;
                        if( resultCode == RESULT_CODE_PROCEEDING)
                        {
                            /* there isn't any cnf signal so we need to stop the command*/
                            resultCode = RESULT_CODE_OK;
                        }
                    }
                    KiFreeMemory( (void**)&deliverReport_p);
                }
                else
                {
                    /* Message already ack on another channel*/
                    resultCode = RESULT_CODE_ERROR;
                }
            }
            else
            {
                KiAllocZeroMemory( sizeof( SimSmTpdu), (void **)&smsTpdu_p);

                /* If not in backward compatible mode (no PDU) then calc SCA length */
                if( (vgSmsIsNotBackwardCompatiblePduMode(entity) ) &&
                    (smsContext_p->smsLength > (Int32)((smsContext_p->tpduLength) * 2)) )
                {
                    scAddrInfoLength = (Int8)(smsContext_p->smsLength - (smsContext_p->tpduLength * 2));
                }

                if ( getCommandId (entity) == VG_AT_SMS_CMGC)
                {
                    resultCode = vgSmsFormatSmsStructFromPdu(   entity,
                                                                smsContext_p->smsMessage,
                                                                &smsContext_p->smsLength,
                                                                smsContext_p->tpduLength,
                                                                &tpduType,
                                                                smsTpdu_p,
                                                                scAddrInfoLength,
                                                                FALSE,
                                                                TRUE);
                    smsContext_p->smsLength = 0;

                    if( tpduType != SIM_SMTPDU_COMMAND)
                    {
                        resultCode = VG_CMS_ERROR_INVALID_PDU_MTI;
                    }
                    if ( RESULT_CODE_OK == resultCode )
                    {
                        resultCode = vgSmsSigOutApexSendCommandSmsTpdu(entity, &smsTpdu_p->command );
                    }
                }
                else /* if not VG_SMS_CMGC or VG_AT_SMS_CSCA e.g. CMGS or CMGW */
                {
                    resultCode = vgSmsFormatSmsStructFromPdu(   entity,
                                                                smsContext_p->smsMessage,
                                                                &smsContext_p->smsLength,
                                                                smsContext_p->tpduLength,
                                                                &tpduType,
                                                                smsTpdu_p,
                                                                scAddrInfoLength,
                                                                FALSE,
                                                                TRUE);
                    smsContext_p->smsLength = 0;

                    if( (tpduType != SIM_SMTPDU_SUBMIT) &&
                        (   (getCommandId (entity) == VG_AT_SMS_CMGS) || /* We cannot send a deliver SMS*/
                            (tpduType != SIM_SMTPDU_DELIVER) ) )
                    {
                        resultCode = VG_CMS_ERROR_INVALID_PDU_MTI;
                    }

                    if ( RESULT_CODE_OK == resultCode )
                    {
                        if ( getCommandId (entity) == VG_AT_SMS_CMGS)
                        {
                            resultCode = vgSmsSigOutApexSendSmsTpdu(entity, &smsTpdu_p->submit);
                        }
                        else if ( getCommandId (entity) == VG_AT_SMS_CMGW)
                        {
                            /* Check if selected status is valid for the given TPDU type*/
                            if( vgSmsSetDefaultSmStatus( tpduType, &smsContext_p->smStatus) == TRUE)
                            {
                                switch( smsContext_p->cpmsWriteSend)
                                {
                                    case VG_CPMS_STORAGE_SM:
                                    {
                                        resultCode = vgSmsSigOutApexStoreSmsTpdu(entity, tpduType, smsTpdu_p);
                                    }
                                    break;

                                    default:
                                    {
                                        FatalParam( smsContext_p->cpmsWriteSend, 0, 0);
                                    }
                                    break;
                                }
                            }
                            else
                            {
                                resultCode = VG_CMS_ERROR_INVALID_PDU_MTI;
                            }
                        }
                        else
                        {
                            /* ERROR - Invalid AT command sending SMS */
                            FatalParam(getCommandId(entity), entity, 0);
//                            resultCode = RESULT_CODE_ERROR;
                        }
                    }
                }

                KiFreeMemory( (void**)&smsTpdu_p);
            }
        }
        else
        {
            resultCode = VG_CMS_ERROR_INVALID_PDU_HEXCHAR;
        }
    }
    setResultCode (entity, resultCode);

    /* Reset context variable*/
    smsContext_p->validSms = TRUE;
}

/*===========================================================================
 *
 * Function:    vgSmsUtilCreatePduFromText
 *
 * Parameters:  (in) sms_p      - Contains some information about the sms
 *              (out) tpduType  - Will contains the PDU type
 *              (out) smsTpdu_p - SMS pdu
 *
 *
 * Returns:     Nothing
 *
 * Description: Use text parameters to create a new SMS PDU
 *
 *==========================================================================*/
void vgSmsUtilCreatePduFromText(    VgmuxChannelNumber entity,
                                    const CiapSms *sms,
                                    SimSmTpduType *tpduType,
                                    SimSmTpdu *smsTpdu)
{
    SmsCommonContext_t *smsCommonContext_p  = ptrToSmsCommonContext ();
    SmsContext_t       *smsContext_p        = ptrToSmsContext (entity);
    Int8                mti                 = DECODE_MTI( smsCommonContext_p->firstOctet);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    /* Only catch DELIVER and SUBMIT so last bit is 0 anyway*/
    *tpduType = (SimSmTpduType)(mti << 1);
    switch( *tpduType)
    {
        case SIM_SMTPDU_SUBMIT:
            {
                Boolean rejectDuplicates =
                    DECODE_RD(smsCommonContext_p->firstOctet)  ? TRUE : FALSE,
                        statusReportReq =
                    DECODE_SRR(smsCommonContext_p->firstOctet) ? TRUE : FALSE,
                        userDataHeader =
                    DECODE_UDH(smsCommonContext_p->firstOctet) ? TRUE : FALSE,
                        replyPath =
                    DECODE_RP(smsCommonContext_p->firstOctet)  ? TRUE : FALSE;

                /* job115421: use first octet values from AT+CSMP command */
                smsTpdu->submit.statusReportReq       = statusReportReq;
                smsTpdu->submit.replyPath             = replyPath;
                smsTpdu->submit.rejectDuplicates      = rejectDuplicates;
                smsTpdu->submit.userDataHeaderPresent = userDataHeader;

                smsTpdu->submit.smsProtocolId.protocolMeaning = smsCommonContext_p->protocolId.protocolMeaning;
                smsTpdu->submit.smsProtocolId.protocolId      = smsCommonContext_p->protocolId.protocolId;

                smsTpdu->submit.smsDataCodingScheme.msgCoding = smsCommonContext_p->dataCodingScheme.msgCoding;
                smsTpdu->submit.smsDataCodingScheme.msgClass  = smsCommonContext_p->dataCodingScheme.msgClass;

                smsTpdu->submit.useRawDcs = FALSE;
                smsTpdu->submit.rawDcs  = 0;
                smsTpdu->submit.msgRef  = 0;

                vgSmsGetValidityPeriod( &smsTpdu->submit.validityPeriodFormat,
                                        &smsTpdu->submit.validityPeriodAsValue,
                                        smsTpdu->submit.validityPeriodAsTime);

                smsTpdu->submit.shortMsgLen = (Int8)(sms->msgLength);
                memcpy (smsTpdu->submit.shortMsgData, sms->message, sms->msgLength);
                memcpy (&smsTpdu->submit.smeAddr, &sms->dest, sizeof(SmsAddress));
                memcpy (&smsTpdu->submit.scAddr, &sms->sca, sizeof(SmsAddress));
            }
            break;

        case SIM_SMTPDU_DELIVER:
            {
                Boolean moreMsgToSend =
                    DECODE_MMS(smsCommonContext_p->firstOctet) ? TRUE : FALSE,
                        statusReportInd =
                    DECODE_SRI(smsCommonContext_p->firstOctet) ? TRUE : FALSE,
                        userDataHeader =
                    DECODE_UDH(smsCommonContext_p->firstOctet) ? TRUE : FALSE,
                        replyPath =
                    DECODE_RP(smsCommonContext_p->firstOctet)  ? TRUE : FALSE;

                /* Inverse MMS*/
                moreMsgToSend = !moreMsgToSend;

                smsTpdu->deliver.statusReportInd       = statusReportInd;
                smsTpdu->deliver.replyPath             = replyPath;
                smsTpdu->deliver.moreMsgsToSend        = moreMsgToSend;
                smsTpdu->deliver.userDataHeaderPresent = userDataHeader;

                smsTpdu->deliver.smsProtocolId.protocolMeaning = smsCommonContext_p->protocolId.protocolMeaning;
                smsTpdu->deliver.smsProtocolId.protocolId      = smsCommonContext_p->protocolId.protocolId;

                smsTpdu->deliver.smsDataCodingScheme.msgCoding = smsCommonContext_p->dataCodingScheme.msgCoding;
                smsTpdu->deliver.smsDataCodingScheme.msgClass  = smsCommonContext_p->dataCodingScheme.msgClass;

                smsTpdu->deliver.rawDcsValue  = 0;

                smsTpdu->deliver.shortMsgLen = (Int8)(sms->msgLength);
                memcpy( smsTpdu->deliver.shortMsgData, sms->message, sms->msgLength);
                memcpy( &smsTpdu->deliver.smeAddr, &sms->dest, sizeof(SmsAddress));
                memcpy( &smsTpdu->deliver.scAddr, &sms->sca, sizeof(SmsAddress));
                memcpy( &smsTpdu->deliver.scTimeStamp[0], &sms->timeStamp[0], sizeof( SmsTimeStamp));
            }
            break;

        default:
            {
                FatalParam( *tpduType, 0, 0);
            }
            break;
    }
}

/*===========================================================================
 *
 * Function:    vgSmsUtilIsExtraInformationEnabled
 *
 * Parameters:  None
 *
 * Returns:     Boolean
 *
 * Description: Returns enabled status of the extra non-standard information.
 *              This mostly takes the form of extra unsolicated information.
 *
 *==========================================================================*/
Boolean vgSmsUtilIsExtraInformationEnabled (void)
{
  SmsCommonContext_t       *smsCommonContext_p = ptrToSmsCommonContext ();
  return (smsCommonContext_p->enableExtraInfo);
}

/*===========================================================================
 *
 * Function:    vgSmsUtilAreExtraUnsolicitedIndicationsEnabled
 *
 * Parameters:  entity - SMS context entity number
 *
 * Returns:     Boolean
 *
 * Description: Returns enabled status of the extra unsolicated indications.
 *
 *==========================================================================*/
Boolean vgSmsUtilAreExtraUnsolicitedIndicationsEnabled (const VgmuxChannelNumber entity)
{
  SmsContext_t *smsContext_p = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (smsContext_p != PNULL, entity, 0, 0);
#endif
  return (smsContext_p->enableExtraUnsol);
}

#if defined (FEA_PHONEBOOK)
/*===========================================================================
 *
 * Function:    vgSmsUtilIsExtraInformationEnabled
 *
 * Parameters:  None
 *
 * Returns:     Boolean
 *
 * Description: Returns enabled status of the extra non-standard information.
 *              This mostly takes the form of extra unsolicated information.
 *
 *==========================================================================*/
Boolean vgSmsUtilIsAlphaIdLookupEnabled (void)
{
  SmsCommonContext_t       *smsCommonContext_p = ptrToSmsCommonContext ();
  return (smsCommonContext_p->enableAlphaId);
}
#endif /* FEA_PHONEBOOK */


/********************************************
*********************************************
**                                         **
**    Serialization of the unsoliciated    **
**    indication handling.                 **
**                                         **
*********************************************
********************************************/



/*===========================================================================
 *
 * Function:    vgSmsUtilSetUnsolicitedEventHandlerFree
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 * Description: Clears the busy state of the unsolicited channel which
 *              indicates that further unsolicited indications can
 *              be processed.
 *
 *              ALSO releases control of any BL procedures.
 *
 *==========================================================================*/
void vgSmsUtilSetUnsolicitedEventHandlerFree (void)
{
    SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext ();
    smsCommonContext_p->unsolicitedChannelBusy = FALSE;

    vgExecuteDelayedAbsmIndSignals();
}

/*===========================================================================
 *
 * Function:    vgSmsUtilSetUnsolicitedEventHandlerBusy
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 * Description: Sets the busy state of the unsolicited channel which
 *              indicates that further unsolicited indiciations CANNOT
 *              be processed until the current one has finished. The
 *              indication is send back to the CI unprocessed.
 *
 *              Should only be called by VgMsss().
 *
 *==========================================================================*/
void vgSmsUtilSetUnsolicitedEventHandlerBusy (void)
{
  SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext ();
  smsCommonContext_p->unsolicitedChannelBusy = TRUE;
}

/*===========================================================================
 *
 * Function:    vgSmsUtilIsUnsolicitedChannelBusy
 *
 * Parameters:  None
 *
 * Returns:     Boolean
 *
 * Description: Returns busy state of the unsolicited channel.
 *
 *              Should only be called by VgMsss().
 *
 *==========================================================================*/
Boolean vgSmsUtilIsUnsolicitedChannelBusy (void)
{
  SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext ();
  return (smsCommonContext_p->unsolicitedChannelBusy);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilGetListOfRecordStatusStrings
 *
 * Parameters:      output - char pointer to append data to
 *
 * Returns:         updated char pointer
 *
 * Description:     provides a list of the sms record status strings
 *
 * e.g. "REC UNREAD","REC READ","STO UNSENT","STO SENT","ALL"
 *
 *-------------------------------------------------------------------------*/

void vgSmsPrintListOfRecordStatusStrings (VgmuxChannelNumber entity)
{
  Int16 index;

  /* list out the options e.g. +CMGL: "REC UNREAD","REC READ","STO UNSENT","STO SENT","ALL" */
  for(index = 0;index < VG_ARRAY_LENGTH(smsRecordTypes); index++)
  {
    vgSmsPutc(entity, '"');
    vgSmsPuts(entity, smsRecordTypes[index].name);
    vgSmsPutc(entity, '"');

     if ( index < (VG_ARRAY_LENGTH(smsRecordTypes)-1) )
     {
       vgSmsPutc(entity, ',');
     }
   }
}

/*==========================================================================
 *
 * Function:    formatSmsStructFromPdu
 *
 * Parameters:  (In) entity
 *              (InOut) smsMessage      - At entry, contains the HEX coded SMS PDU
 *                                        At output, will contain the binary SMS PDU
 *              (InOut) smsLength       - Contains the number of used byte in the SMS PDU
 *                                        (output size is alzays < to entry size)
 *              (In) argLength          - PDU size given at command parameter
 *              (Out) tpduType          - determines whether the SMS type
 *              (InOut) smTpdu          - pointer to SimSmTpdu union where the SMS TPDU
 *                                        is to be formatted
 *              (In) scAddrInfoLength   - Size for the Service Center address
 *              (In) isStatusReport     - True when PDU is a status report
 *              (In) checkCompMode      - Whether the function should do the compatibility checks
 *
 * Returns:     RESULT_CODE_OK if successful
 *
 * Description: Formats pdu from hex representation
 *
 *=========================================================================*/
ResultCode_t vgSmsFormatSmsStructFromPdu(   const VgmuxChannelNumber  entity,
                                            Int8               *smsMessage,
                                            Int16              *smsLength,
                                            Int16               argLength,
                                            SimSmTpduType      *tpduType,
                                            SimSmTpdu          *smTpdu,
                                            Int8                scAddrInfoLength,
                                            Boolean             isStatusReport,
                                            Boolean             checkCompMode)
{
    ResultCode_t  result = RESULT_CODE_OK;
    Int32         i;
    Int8         *output;
    Int8          tpduOffset = 0;
    Int32         actualLength = 0;
    Boolean       readSca = FALSE;

    /* Check no invalid characters (non-hex digits) in the message */
    WarnAssert( checkValidCharsInPduSmsMessage( smsMessage, *smsLength) );

    if (RESULT_CODE_OK == result)
    {
        /* check length of the pdu is correct... */
        if( vgSmsIsNotBackwardCompatiblePduMode(entity) ||
            (checkCompMode == FALSE))
        {
            actualLength = (argLength * 2) + scAddrInfoLength;
        }
        else /* backward compatible for +CSMS = 128 */
        {
            actualLength = (argLength * 2);
        }

        if ( *smsLength != actualLength )
        {
            result = VG_CMS_ERROR_INVALID_PDU_LENGTH;
        }
    }

    if (RESULT_CODE_OK == result)
    {
        /* Convert from hexadecimal digits pairs to bytes in place i.e.
        ** overwriting data as we convert.
        */
        output = smsMessage;
        for (i=0; i<*smsLength; i+=2)
        {
            *output++ = (   hexToNum(smsMessage[i]) << 4) +
                            hexToNum(smsMessage[i+1]);
        }

        if( vgSmsIsNotBackwardCompatiblePduMode(entity) ||
            (checkCompMode == FALSE))
        {
            /* the first octet contains the length of the SCA information,
            ** so set TPDU offset value to cater for it. 0 = no SCA info present.
            */
            if ( 0 == smsMessage[0] )
            {
                tpduOffset = 1; /* skip the SCA length octet */
                readSca = FALSE;
            }
            else
            {
                /* build the SC Address data into the required TsXXXReq signal */
                tpduOffset = smsMessage[0] + 1;
                readSca = TRUE;
            }
        }
        else /* backward compatible PDU mode - no SCA ever supplied in PDU data */
        {
            readSca = FALSE;
        }
    }

    if (RESULT_CODE_OK == result)
    {
        /*
        ** Only attempt to parse pdu if SMS TPDU has the relevant Message Type
        ** Indication (MTI) coding
        */
        switch( DECODE_MTI( smsMessage[tpduOffset]))
        {
            case SM_MTI_SUBMIT:
            {
                *tpduType = SIM_SMTPDU_SUBMIT;
            }
            break;

            case SM_MTI_DELIVER:
            {
                *tpduType = SIM_SMTPDU_DELIVER;
            }
            break;

            /* COMMAND and STATUS REPORT have same mti value*/
/*          case SM_MTI_STATUS_REPORT :*/
            case SM_MTI_COMMAND:
            {
                if( isStatusReport)
                {
                    *tpduType = SIM_SMTPDU_STATUS_REPORT;
                }
                else
                {
                    *tpduType = SIM_SMTPDU_COMMAND;
                }
            }
            break;

            default:
            {
                result = VG_CMS_ERROR_INVALID_PDU_MTI;
            }
            break;
        }
    }

    if (RESULT_CODE_OK == result)
    {
        /* Fill/read the service centre address*/
        if( readSca)
        {
            if ( FALSE == buildSmsPduScAddress(smsMessage, getShortMessageSca(*tpduType, smTpdu)))
            {
                result = VG_CMS_ERROR_INVALID_PDU_SCA_LENGTH;
            }
        }
        else
        {
            vgSmsUtilGetSca( getShortMessageSca(*tpduType, smTpdu));
        }
    }

    if (RESULT_CODE_OK == result)
    {
        /* Complete the PDU strucutre*/
        switch( *tpduType)
        {
            case SIM_SMTPDU_SUBMIT:
            {
                /* Build the SMS-SUBMIT TPDU from the pdu */
                result = vgBuildTsSubmitReq(        argLength,
                                                    &smsMessage[tpduOffset],
                                                    &smTpdu->submit);
            }
            break;

            case SIM_SMTPDU_DELIVER:
            {
                /* Build the SMS-DELIVER TPDU from the pdu */
                result = vgBuildTsDeliverInd(       argLength,
                                                    &smsMessage[tpduOffset],
                                                    &smTpdu->deliver);
            }
            break;

            case SIM_SMTPDU_STATUS_REPORT :
            {
                /* Build the SMS-STATUS-REPORT TPDU from the pdu */
                result = vgBuildTsStatusReportInd(  argLength,
                                                    &smsMessage[tpduOffset],
                                                    &smTpdu->statusReport);
            }
            break;

            case SIM_SMTPDU_COMMAND:
            {
                /* Build the SMS-COMMAND TPDU from the pdu */
                result = vgBuildTsCommandReq(       argLength,
                                                    &smsMessage[tpduOffset],
                                                    &smTpdu->command);
            }
            break;

            default:
            {
                /* Invalid enumeration value */
                FatalParam(entity, *tpduType, 0);
            }
            break;
        }
    }
    *smsLength /= 2;

    return (result);
}

/*==========================================================================
 *
 * Function:    vgSmsFormatDeliverReportFromPdu
 *
 * Parameters:  (In) entity
 *              (InOut) smsMessage      - At entry, contains the HEX coded SMS PDU
 *                                        At output, will contain the binary SMS PDU
 *              (InOut) smsLength       - Contains the number of used byte in the SMS PDU
 *                                        (output size is alzays < to entry size)
 *              (In) argLength          - PDU size given at command parameter
 *              (InOut) smTpdu          - pointer to a TsDeliverReportReq where the SMS TPDU
 *                                        is to be formatted
 *
 * Returns:     RESULT_CODE_OK if successful
 *
 * Description: Formats deliver report from hex representation
 *
 *=========================================================================*/
ResultCode_t vgSmsFormatDeliverReportFromPdu(   const VgmuxChannelNumber  entity,
                                                Int8               *smsMessage,
                                                Int16              *smsLength,
                                                TsDeliverReportReq *smTpdu)
{
    ResultCode_t  result = RESULT_CODE_OK;
    Int32         i;
    Int8         *output;
    Int8          tpduOffset = 0;

    /* Check no invalid characters (non-hex digits) in the message */
    if( checkValidCharsInPduSmsMessage( smsMessage, *smsLength) == FALSE)
    {
        result = VG_CMS_ERROR_INVALID_PDU_HEXCHAR;
    }

    if( (RESULT_CODE_OK == result) &&
        (((*smsLength) & 0x0001) == 1))
    {
        /* SMS length should be an even number*/
        result = VG_CMS_ERROR_INVALID_PDU_LENGTH;
    }

    if (RESULT_CODE_OK == result)
    {
        /* Convert from hexadecimal digits pairs to bytes in place i.e.
        ** overwriting data as we convert.
        */
        output = smsMessage;
        for (i=0; i<*smsLength; i+=2)
        {
            *output++ = (   hexToNum(smsMessage[i]) << 4) +
                            hexToNum(smsMessage[i+1]);
        }
        (*smsLength) /= 2;
    }

    if (RESULT_CODE_OK == result)
    {
        /* Build the SMS-DELIVER-REPORT TPDU from the pdu */
        result = vgBuildTsDeliverReportReq( *smsLength,
                                            &smsMessage[tpduOffset],
                                            smTpdu);
    }


    return (result);
}

 /*--------------------------------------------------------------------------
 *
 * Function:        vgSmsUtilGetListOfRecordStatusStrings
 *
 * Parameters:      tpduType        - SMS PDU type
 *                  (InOut) status  - SMS status
 *
 * Returns:         TRUE if the operation is successful
 *
 * Description:     If the current status is a valid one, check the status
 *                  is valid with the given SMS type.
 *                  Else if status is SIM_SMREC_INVALID_STATUS, assign a
 *                  default status depending of SMS type.
 *
 *-------------------------------------------------------------------------*/
Boolean vgSmsSetDefaultSmStatus( SimSmTpduType tpduType, SimSmRecordStatus *status)
{
    Boolean res = TRUE;

    switch( tpduType)
    {
        case SIM_SMTPDU_DELIVER:
            {
                if( *status == SIM_SMREC_INVALID_STATUS)
                {
                    /* Set default status*/
                    *status = SIM_SMREC_RECEIVED_UNREAD;
                }
                else if(    (*status != SIM_SMREC_RECEIVED_READ) &&
                            (*status != SIM_SMREC_RECEIVED_UNREAD))
                {
                    res = FALSE;
                }
            }
            break;

        case SIM_SMTPDU_SUBMIT:
            {
                if( *status == SIM_SMREC_INVALID_STATUS)
                {
                    /* Set default status*/
                    *status = SIM_SMREC_ORIGINATED_NOT_SENT;
                }
                else if(    (*status != SIM_SMREC_ORIGINATED_SENT) &&
                            (*status != SIM_SMREC_ORIGINATED_NOT_SENT))
                {
                   res = FALSE;
                }
            }
            break;

        case SIM_SMTPDU_STATUS_REPORT:
            {
                if( *status == SIM_SMREC_INVALID_STATUS)
                {
                    /* Set default status*/
                    *status = SIM_SMREC_RECEIVED_READ;
                }
                else if( *status != SIM_SMREC_RECEIVED_READ)
                {
                    res = FALSE;
                }
            }
            break;

        default:
            {
                res = FALSE;
            }
            break;
    }
    return res;
}

/* END OF FILE */

