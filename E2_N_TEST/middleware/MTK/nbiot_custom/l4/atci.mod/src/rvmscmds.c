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
 * SMS AT command handlers (not cell broadcast for NB-IOT)
 *
 ***************************************************************************
 *
 * Notes
 * -----
 *
 * Note 1:
 *
 * Each command functions takes the following parameters:
 *
 * (InOut) commandBuffer - a pointer to a CommandLine_t
 *         structure with the position field indicating the point in
 *         the structure's character array following this command
 *         where this command's parameters (if any) begin, and with
 *         the length field indicating the number of valid characters
 *         in the structure's character array. This function leaves
 *         the position field at either the start of the next command
 *         (if not the last command in the character the array), or
 *         equal to the length field (if the last command in the
 *         character the array).
 *
 * (In)    entity - the SMS context to use
 *
 *
 * And returns a result code:
 *
 * RESULT_CODE_OK           command executed correctly and finished.
 *
 * RESULT_CODE_ERROR        error and finished.
 * (or some other error result code)
 *
 * RESULT_CODE_PROCEEDING   command continues to execute.
 *
 *
 * Note 2:
 *
 * There are three local functions: getExtendedDecimalInt8,
 * getExtendedDecimalInt16, getExtendedDecimalInt32, that are wrappers
 * for the getExtendedParameter function in vgutil. These are used
 * instead because they provide a safer interface/usage. If using
 * getExtendedParameter it is possible to pass in a pointer to an
 * Int8 where a pointer to an Int32 was expected, the ARM compiler will not
 * warn of this and hence bad things will happen.
 **************************************************************************/

#define MODULE_NAME "RVMSCMDS"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvcrerr.h>
#include <rvoman.h>
#include <rvomut.h>
#include <rvmsss.h>
#include <rvmsprnt.h>
#include <rvmssigo.h>
#include <rvmssigi.h>
#include <rvmsut.h>
#include <rvmscmds.h>
#include <rvmsdata.h>
#include <cici_sig.h>
#include <rvchman.h>
#include <rvcrconv.h>
#include <rvpfsigo.h>
#include <rvomtime.h>
#include <rvcrhand.h>
#include <rvcimxut.h>
#include <simdec.h>
#include <gkimem.h>
#include <frhsl.h>
#include "atci_gki_trace.h"
/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static Int8  getExtendedDecimalInt8  (CommandLine_t *commandBuffer_p,
                                      Boolean *ok,
                                      Int8 defaultValue);
/*static Int16 getExtendedDecimalInt16 (CommandLine_t *commandBuffer_p,
                                      Boolean *ok,
                                      Int16 defaultValue);*/
static Int32 getExtendedDecimalInt32 (CommandLine_t *commandBuffer_p,
                                      Boolean *ok,
                                      Int32 defaultValue);

static ResultCode_t decodeSendStoreSmsParamters( CommandLine_t *commandBuffer_p,
                                                 const VgmuxChannelNumber  entity);

static Boolean decodeCmgcParametersForTextMode(CommandLine_t *commandBuffer_p,
                                                const VgmuxChannelNumber  entity);


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
 * Function:    getExtendedDecimalInt8
 *
 * Parameters:  (InOut) commandBuffer_p - a pointer to a CommandLine_t
 *              structure with the position field indicating the start of
 *              where a decimal value may be.
 *              (Out) ok - Boolean indicating the success or failure in
 *              obtaining an extended AT command's parameter value.
 *              (In) defaultValue - an Int8 value to be returned if the
 *              parameter is not supplied.
 *
 * Returns:     Int8 value
 *
 * Description: Obtains extended AT command parameter, advancing the
 *              commandBuffer pointer, *ok is set to TRUE if the returned
 *              value is ok, false otherwise.
 *
 *-------------------------------------------------------------------------*/
static Int8 getExtendedDecimalInt8 (CommandLine_t *commandBuffer_p,
                                    Boolean *ok,
                                    Int8 defaultValue)
{
  Int32 value = 0;

  *ok = getExtendedParameter(commandBuffer_p, &value, (Int32)defaultValue);
  return ((Int8)value);
}

/*--------------------------------------------------------------------------
 *
 * Function:    getExtendedDecimalInt16
 *
 * Parameters:  (InOut) commandBuffer_p - a pointer to a CommandLine_t
 *              structure with the position field indicating the start of
 *              where a decimal value may be.
 *              (Out) ok - Boolean indicating the success or failure in
 *              obtaining an extended AT command's parameter value.
 *              (In) defaultValue - an Int16 value to be returned if the
 *              parameter is not supplied.
 *
 * Returns:     Int16 value
 *
 * Description: Obtains extended AT command parameter, advancing the
 *              commandBuffer pointer, *ok is set to TRUE if the returned
 *              value is ok, false otherwise.
 *
 *-------------------------------------------------------------------------*/
/*static Int16 getExtendedDecimalInt16 (CommandLine_t *commandBuffer_p,
                                      Boolean *ok,
                                      Int16 defaultValue)
{
  Int32 value = 0;

  *ok = getExtendedParameter(commandBuffer_p, &value, (Int32)defaultValue);
  return ((Int16)value);
}*/

/*--------------------------------------------------------------------------
 *
 * Function:    getExtendedDecimalInt32
 *
 * Parameters:  (InOut) commandBuffer_p - a pointer to a CommandLine_t
 *              structure with the position field indicating the start of
 *              where a decimal value may be.
 *              (Out) ok - Boolean indicating the success or failure in
 *              obtaining an extended AT command's parameter value.
 *              (In) defaultValue - an Int32 value to be returned if the
 *              parameter is not supplied.
 *
 * Returns:     Int32 value
 *
 * Description: Obtains extended AT command parameter, advancing the
 *              commandBuffer pointer, *ok is set to TRUE if the returned
 *              value is ok, false otherwise.
 *
 *-------------------------------------------------------------------------*/
static Int32 getExtendedDecimalInt32 (CommandLine_t *commandBuffer_p,
                                      Boolean *ok,
                                      Int32 defaultValue)
{
  Int32 value = 0;

  *ok = getExtendedParameter(commandBuffer_p, &value, defaultValue);
  return (value);
}

/**
 * \brief Get the number of records in EF-SMS
 * \return Number of record
 */
Int8 getMaxSmsRecords( void )
{
    SmsCommonContext_t   *smsCommonContext_p = ptrToSmsCommonContext();

    return (smsCommonContext_p->smSimState.numberOfSmsRecords);
}




/**
 * \brief Get the number of free records in EF-SMS
 * \return Number of free record
 */
Int8 getFreeSmsRecords( void )
{
    SmsCommonContext_t   *smsCommonContext_p = ptrToSmsCommonContext();

    return (smsCommonContext_p->smSimState.numberOfSmsRecords - smsCommonContext_p->smSimState.usedRecords);
}


/**
 * \brief Get the number of Used records in EF-SMS
 * \return Number of Used record
 */
Int8 getUsedSmsRecords( void )
{
    SmsCommonContext_t   *smsCommonContext_p = ptrToSmsCommonContext();

    return (smsCommonContext_p->smSimState.usedRecords);
}

/**
 * \brief Get the maximum number of SMS-SR that can be stored
 * \return Maximum number of SMS-SR location that can be stored  (SIM and ME)
 */
Int16 getMaxSrRecords( VgmuxChannelNumber  entity )
{
    Int16               totalused;
    SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext();

    totalused = smsCommonContext_p->smSimState.numberOfSmsrRecords;

    return totalused;
}

/**
 * \brief Get the number of free space for SMS-SR storage
 * \return Number of total SMS-SR location that can still be stored (SIM and ME)
 */
Int16 getFreeSrRecords( void )
{
    SmsCommonContext_t   *smsCommonContext_p = ptrToSmsCommonContext();

    return(smsCommonContext_p->smSimState.numberOfSmsrRecords - smsCommonContext_p->smSimState.usedSmsrRecords);
}

/**
 * \brief Get the quantity of SMS-SR stored
 * \return Quantity of SMS-SR stored  (SIM and ME)
 */
Int16 getUsedSrRecords( VgmuxChannelNumber  entity )
{
    Int16               totalused;
    SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext();

    totalused = smsCommonContext_p->smSimState.usedSmsrRecords;

    return totalused;
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/


/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsCscaServiceCentreAddressSim
 *
 * Parameters:  (InOut) commandBuffer - a pointer to a CommandLine_t
 *              (In)    entity - index to the SMS context to use
 *
 * Returns:     result of operation
 *
 * Description: This function executes the AT+CSCA command.
 *              Set the Service Centre Address in the SIM.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsCscaServiceCentreAddressSim (CommandLine_t *commandBuffer_p,
                                               const VgmuxChannelNumber  entity)
{
  ResultCode_t         result = RESULT_CODE_OK;
  SmsCommonContext_t   *smsCommonContext_p = ptrToSmsCommonContext ();
  ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
  Char                 address[VG_CR_SMS_MAX_ADDR_LEN + NULL_TERMINATOR_LENGTH] = {0};
  Int16                addressLength = 0;
  Boolean              ok;
  Int8                 typeNum;

  switch (operation)
  {
    case EXTENDED_QUERY:  /* AT+CSCA? */
    {
      result = vgSmsSigOutApexReadSmspReq(entity, VG_SMSP_READ_QUERY_SCA);
      break;
    }
    case EXTENDED_RANGE:  /* AT+CSCA=? */
    {
      break;
    }
    case EXTENDED_ASSIGN: /* AT+CSCA=... */
    {

      /* <sca> parameter is mandatory */
      if ( (getExtendedString (commandBuffer_p,
                                address,
                                 VG_CR_SMS_MAX_ADDR_LEN,
                                  &addressLength) == TRUE) &&
           (addressLength !=0) )
      {
        /* Convert string from current TE set.... */
        addressLength = vgMapTEToGsm( getCrOutputBuffer(entity),
                                      VG_CR_CH_SET_OUTPUT_BUFFER_LEN,
                                      address,
                                      addressLength,
                                      (VgCSCSMode)getProfileValue(entity, PROF_CSCS),
                                      entity);

        /* Encode sms digits..... */
        if((addressLength > 0) &&
           (vgSmsUtilEncodeSmsDigits( getCrOutputBuffer(entity),
                                      addressLength,
                                      &smsCommonContext_p->vgTempSca)) &&
           (smsCommonContext_p->vgTempSca.length > 0))
        {
          result = RESULT_CODE_PROCEEDING;

          typeNum = getExtendedDecimalInt8 (
                     commandBuffer_p,
                      &ok,
                       VG_DIAL_NUMBER_UNKNOWN);

          if (TRUE == ok)
          {
            switch (typeNum)
            {
              case VG_DIAL_NUMBER_INTERNATIONAL:
              {
                smsCommonContext_p->vgTempSca.typeOfNumber = NUM_INTERNATIONAL;
                break;
              }
              case VG_DIAL_NUMBER_UNKNOWN:
              {
                /* already set in vgSmsUtilEncodeSmsDigits */
                break;
              }
              case VG_DIAL_NUMBER_NATIONAL:
              {
                smsCommonContext_p->vgTempSca.typeOfNumber = NUM_NATIONAL;
                break;
              }
              case VG_DIAL_NUMBER_NET_SPECIFIC:
              {
                smsCommonContext_p->vgTempSca.typeOfNumber = NUM_NETWORK_SPEC;
                break;
              }
              default:
              {
                result = RESULT_CODE_ERROR;
                break;
              }
            }
          }
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }

        resetCrOutputBuffer(entity);
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }

      if( RESULT_CODE_PROCEEDING == result )
      {
        result = vgSmsSigOutApexReadSmspReq(entity, VG_SMSP_READ_WRITE_SCA);
      }
      break;
    }

    case EXTENDED_ACTION: /* AT+CSCA */
    {
      result = RESULT_CODE_ERROR;
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
 * Function:    decodeCmgcParametersForTextMode
 *
 * Parameters:  (InOut) commandBuffer - a pointer to a CommandLine_t
 *              (In)    entity - index to the SMS context to use
 *
 * Returns:     TRUE if parameters ok.
 *
 * Description: This function retrieves and performs error checking on
 *              the parameters for the CMGC command when in text mode.
 *
 *-------------------------------------------------------------------------*/
static Boolean decodeCmgcParametersForTextMode(CommandLine_t *commandBuffer_p,
                                                const VgmuxChannelNumber  entity)
{
  SmsContext_t  *smsContext_p = ptrToSmsContext (entity);
  Int8          firstOctet;
  Int8          pid;
  Int8          msgNum;
  Int32         commandType;
  Int32         toda;
  Char          address[VG_CR_SMS_MAX_ADDR_LEN + NULL_TERMINATOR_LENGTH] = {0};
  Int16         addressLength = 0;
  Boolean       ok = TRUE;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  firstOctet = getExtendedDecimalInt8 (commandBuffer_p, &ok, 2);
  if (TRUE == ok)
  {
    commandType = getExtendedDecimalInt32 (commandBuffer_p, &ok, 0);
    if (TRUE == ok)
    {
      smsContext_p->command.commandType = (SmsCommand)commandType;
      smsContext_p->command.firstOctet  = (Int8)firstOctet;
    }
  }

  if (TRUE == ok)
  {
    pid = getExtendedDecimalInt8(commandBuffer_p, &ok, 0);
    if (TRUE == ok)
    {
      smsContext_p->command.pidByteVal = (Int8)pid;
    }
  }

  /* message number */
  if (TRUE == ok)
  {
    msgNum = getExtendedDecimalInt8(commandBuffer_p, &ok, 0);
    if (TRUE == ok)
    {
      smsContext_p->command.msgNum = msgNum;
    }
  }

  /* address */
  if ( (TRUE == ok) &&
       (TRUE == getExtendedString (commandBuffer_p,
                                   address,
                                   VG_CR_SMS_MAX_ADDR_LEN,
                                   &addressLength))
     )
  {
    if (0 == addressLength)
    {
      /* optional <da> not entered */
      memset(&smsContext_p->da, 0, sizeof(SmsAddress));
    }
    else
    {
      /* Convert string from current TE set.... */
      addressLength = vgMapTEToGsm( getCrOutputBuffer(entity),
                                    VG_CR_CH_SET_OUTPUT_BUFFER_LEN,
                                    address,
                                    addressLength,
                                    (VgCSCSMode)getProfileValue(entity, PROF_CSCS),
                                    entity);

      /* Encode sms digits..... */
      if( vgSmsUtilEncodeSmsDigits( getCrOutputBuffer(entity),
                                    addressLength,
                                    &smsContext_p->da) )
      {
        toda = getExtendedDecimalInt8(commandBuffer_p, &ok, VG_DIAL_NUMBER_UNKNOWN);
        if (TRUE == ok)
        {
          switch (toda)
          {
            case VG_DIAL_NUMBER_INTERNATIONAL:
            {
              smsContext_p->da.typeOfNumber = NUM_INTERNATIONAL;
              break;
            }
            case VG_DIAL_NUMBER_UNKNOWN:
            {
              /* already set in vgSmsUtilEncodeSmsDigits */
              break;
            }
            case VG_DIAL_NUMBER_NATIONAL:
            {
              smsContext_p->da.typeOfNumber = NUM_NATIONAL;
              break;
            }
            case VG_DIAL_NUMBER_NET_SPECIFIC:
            {
              smsContext_p->da.typeOfNumber = NUM_NETWORK_SPEC;
              break;
            }
            default:
            {
              ok = FALSE;
              break;
            }
          } /* end switch */
        }
      }
      else /* da entered incorrectly */
      {
        ok = FALSE;
      }

      resetCrOutputBuffer(entity);
    }
  }
  else
  {
    ok = FALSE;
  }
  return (ok);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsCmssSendFromSim
 *
 * Parameters:  (InOut) commandBuffer - a pointer to a CommandLine_t
 *              (In)    entity - index to the SMS context to use
 *
 * Returns:     result of command
 *
 * Description: Implements the CMSS command: send SMS from the SIM/ME.
 *
 * Used to send an SMS that has been previously saved to the SIM/ME
 * using the Store SMS command CMGW.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsCmssSendFromSim (CommandLine_t *commandBuffer_p,
                                   const VgmuxChannelNumber  entity)
{
    SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext ();
    Int32               index           = 0;
    Int32               toda;
    Int16               maxIndex        = 0;
    Char                address[VG_CR_SMS_MAX_ADDR_LEN + NULL_TERMINATOR_LENGTH] = {0};
    Int16               addressLength   = 0;
    SmsContext_t       *smsContext_p    = ptrToSmsContext (entity);
    ResultCode_t        result          = RESULT_CODE_OK;
    ExtendedOperation_t operation       = getExtendedOperation (commandBuffer_p);
    SmRequestStatus     requestStatus;
    Boolean             present;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    switch (operation)
    {
        case EXTENDED_QUERY:  /* AT+CMSS? */
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
                break;
            }

        case EXTENDED_RANGE:  /* AT+CMSS=? */
            {
                break;
            }

        case EXTENDED_ASSIGN: /* AT+CMSS=... */
            {
                if( result == RESULT_CODE_OK)
                {
                    /* if for any reason SIM is not ready, dont proceed */
                    requestStatus = smsCommonContext_p->smSimState.requestStatus;

                    if (requestStatus != SM_REQ_OK)
                    {
                        result = vgSmsUtilConvertRequestError (entity, requestStatus);
                    }
                    else
                    {
                        switch( smsContext_p->cpmsWriteSend)
                        {
                            case VG_CPMS_STORAGE_SM:
                                {
                                    maxIndex = getMaxSmsRecords();
                                }
                                break;
                            default:
                                {
                                    FatalParam( smsContext_p->cpmsWriteSend, 0, 0);
                                }
                                break;
                        }

                        /* <index> is mandatory parameter */
                        if( getExtendedParameter (commandBuffer_p, &index, ULONG_MAX) == FALSE)
                        {
                            result = VG_CMS_ERROR_INVALID_PARMETER;
                        }
                        else if(    (index <= 0) ||
                                    (index > maxIndex) )
                        {
                            result = VG_CMS_ERROR_INVALID_MEMORY_INDEX;
                        }
                    }
                }

                if( result == RESULT_CODE_OK)
                {
                    /* <da> is optional parameter */
                    if (FALSE == getExtendedString( commandBuffer_p,
                                                    address,
                                                    VG_CR_SMS_MAX_ADDR_LEN,
                                                    &addressLength) )
                    {
                        result = VG_CMS_ERROR_INVALID_PARMETER;
                    }
                    else
                    {
                        if (addressLength == 0)
                        {
                            smsContext_p->atCmssParams.recordNumber = (Int16)index;
                            smsContext_p->atCmssParams.useNewDest = FALSE;
                            memset(&smsContext_p->atCmssParams.dest, 0, sizeof(SmsAddress));
                        }
                        else
                        {
                            /* Convert string from current TE set.... */
                            addressLength = vgMapTEToGsm(   getCrOutputBuffer(entity),
                                                            VG_CR_CH_SET_OUTPUT_BUFFER_LEN,
                                                            address,
                                                            addressLength,
                                                            (VgCSCSMode)getProfileValue(entity, PROF_CSCS),
                                                            entity);

                            if( vgSmsUtilEncodeSmsDigits(   getCrOutputBuffer(entity),
                                                            addressLength,
                                                            &smsContext_p->da))
                            {
                                smsContext_p->atCmssParams.recordNumber = (Int16)index;
                                smsContext_p->atCmssParams.useNewDest = TRUE;
                                memcpy(&smsContext_p->atCmssParams.dest, &smsContext_p->da, sizeof(SmsAddress));
                            }
                            else
                            {
                                result = VG_CMS_ERROR_INVALID_PARMETER;
                            }

                            resetCrOutputBuffer(entity);
                        }
                    }
                }

                /* read <toda> parameter but ignore its value*/
                if( result == RESULT_CODE_OK)
                {
                    if( (getExtendedParameterPresent(   commandBuffer_p,
                                                        &toda,
                                                        VG_DIAL_NUMBER_INTERNATIONAL,
                                                        &present) == FALSE) ||
                        (toda >= VG_DIAL_NUMBER_TYPE_MAX))
                    {
                        result = VG_CMS_ERROR_INVALID_PARMETER;
                    }
                }

                if( result == RESULT_CODE_OK)
                {
                    switch( smsContext_p->cpmsWriteSend)
                    {
                        case VG_CPMS_STORAGE_SM:
                            {
                                /* parameters are retrieved from atCmssParams in smsContext */
                                result = vgSmsSigOutApexSendFromSim(entity);
                                if (result != RESULT_CODE_PROCEEDING)
                                {
                                    result = VG_CMS_ERROR_UNKNOWN;
                                }
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
            }

        case EXTENDED_ACTION: /* AT+CMSS */
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
                break;
            }

        default:
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
                break;
            }
    }
    return (result);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsCmglListSms
 *
 * Parameters:  (InOut) commandBuffer - a pointer to a CommandLine_t
 *              (In)    entity - index to the SMS context to use
 *
 * Returns:     result of command
 *
 * Description: Implements the CMGL command: list SMS's.
 *
 * For example to list SMS's in text mode do:
 *
 * AT+CMGF=1            (enable text mode)
 * AT+CMGL="ALL"        (or can specify: "REC UNREAD", "REC READ",
 *                       "STO UNSENT", "STO SENT")
 *
 * In PDU mode:
 *
 * AT+CMGF=0
 * AT+CMGL=4            (4=ALL, 0=Read, 1=Unread, 2=Sent, 3=Unset)
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsCmglListSms( CommandLine_t             *commandBuffer_p,
                               const VgmuxChannelNumber   entity )
{
    SmsContext_t           *smsContext_p        = ptrToSmsContext(entity);
    ResultCode_t            result              = RESULT_CODE_OK;

#if defined (ENABLE_LONG_AT_CMD_RSP)
    Char                    status[AT_SMALL_BUFF_SIZE + NULL_TERMINATOR_LENGTH] = {0};
#else
    Char                    status[VG_MAX_AT_DATA_IN_SIGNAL_LENGTH + NULL_TERMINATOR_LENGTH] = {0};
#endif
    Int16                   statusLength        = 0;
    Int32                   pduIdx;
    SmsSimAccessType        simAccessType       = SM_FIRST_READ;
    Boolean                 isValidCMGLCmd      = FALSE;
    ExtendedOperation_t     operation           = getExtendedOperation(commandBuffer_p);

    FatalCheck (smsContext_p != PNULL, entity, 0, 0);

    switch( operation )
    {
        case EXTENDED_QUERY:  /* AT+CMGL? */
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
                break;
            }

        case EXTENDED_RANGE:  /* AT+CMGL=? */
            {
                if ( TRUE == vgSmsUtilIsInTextMode(entity) )
                {
                    vgSmsPrintNewline(entity);
                    vgSmsPuts (entity, (const Char*)"+CMGL: (");

                    /* list out the options e.g. +CMGL: "REC UNREAD","REC READ","STO UNSENT","STO SENT","ALL" */
                    vgSmsPrintListOfRecordStatusStrings(entity);

                    vgSmsPuts (entity, (const Char*)")");

                    vgSmsPrintNewline(entity);
                    vgSmsFlush(entity);
                }
                else  /* PDU mode */
                {
                    /* 1 = Unread, 0 = Read, 3 = Stored & Unsent, 2 = Stored & Sent, 4 = All
                    ** Note how 0 & 1 and 2 & 3 are swapped around... strange, but's that the way it is!
                    **/
                    vgSmsPrintNewline(entity);
                    vgSmsPuts(entity, (const Char*)"+CMGL: (0-4)");
                    vgSmsPrintNewline(entity);
                    vgSmsFlush(entity);
                }
                break;
            }

        case EXTENDED_ASSIGN: /* AT+CMGL=... */
            {
                if( vgSmsUtilIsInTextMode(entity) )
                {
                    /* <stat> is string type in text mode so need to decode the parameter and
                    ** convert it from string "REC UNREAD", "REC READ", "STO SENT", "STO UNSENT" or "ALL"
                    ** into SmsSimAccessType. <stat> is mandatory parameter
                    **/
                    if( (TRUE == getExtendedString( commandBuffer_p,
                                                    status,
                                                    
#if defined (ENABLE_LONG_AT_CMD_RSP)
                                                    AT_SMALL_BUFF_SIZE,
#else
                                                    VG_MAX_AT_DATA_IN_SIGNAL_LENGTH,
#endif                                                    
                                                    &statusLength)) &&
                        (vgSmsUtilConvertStatusStringToSimAccessType(status, &simAccessType)) )
                    {
                        isValidCMGLCmd = TRUE;
                    }
                    else
                    {
                        result = VG_CMS_ERROR_INVALID_TEXT_MODE_PARM;
                    }
                }
                else  /* PDU mode */
                {
                    /* <stat> is integer type in PDU mode, mandatory parameter */
                    /* convert index to SmsSimAccessType */
                    if( (getExtendedParameter (commandBuffer_p, &pduIdx, ULONG_MAX) == TRUE) &&
                        (vgSmsUtilConvertPduIdxToSimAccessType(pduIdx, &simAccessType)) )
                    {
                        isValidCMGLCmd = TRUE;
                    }
                    else
                    {
                        result = VG_CMS_ERROR_INVALID_PDU_MODE_PARM;
                    }
                }

                if( isValidCMGLCmd )
                {
                    switch( smsContext_p->cpmsReadDelete )
                    {
                        case VG_CPMS_STORAGE_SM:
                            {
                                result = vgSmsSigOutApexReadSmsSetReadState( entity, 0/*index*/, simAccessType );

                                if (result != RESULT_CODE_PROCEEDING)
                                {
                                    result = VG_CMS_ERROR_UNKNOWN;
                                }
                            }
                            break;
                        case VG_CPMS_STORAGE_SR:
                            {
                                /* Start by reading all the SMS-SR from sim*/
                                result = vgSmsSigOutApexReadSmsrSetReadState(entity, 0/*index*/, simAccessType);
                                if( result != RESULT_CODE_PROCEEDING )
                                {
                                    result = VG_CMS_ERROR_UNKNOWN;
                                }
                            }
                            break;

                        default:
                            {
                                FatalParam( smsContext_p->cpmsReadDelete, 0, 0);
                            }
                            break;
                    }
                }
                break;
            }

        case EXTENDED_ACTION: /* AT+CMGL */
            {
                switch( smsContext_p->cpmsReadDelete )
                {
                    case VG_CPMS_STORAGE_SM:
                        {
                            result = vgSmsSigOutApexReadSmsSetReadState( entity, 0/*index*/, SM_FIRST_UNREAD );

                            if (result != RESULT_CODE_PROCEEDING)
                            {
                                result = VG_CMS_ERROR_UNKNOWN;
                            }
                        }
                        break;

                    case VG_CPMS_STORAGE_SR:
                        {
                            result = vgSmsSigOutApexReadSmsrSetReadState(entity, 0/*index*/, SM_FIRST_READ);
                            if( result != RESULT_CODE_PROCEEDING )
                            {
                                result = VG_CMS_ERROR_UNKNOWN;
                            }
                        }
                        break;

                    default:
                        {
                            FatalParam( smsContext_p->cpmsReadDelete, 0, 0);
                        }
                        break;
                }
                break;
            }

        default:
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
                break;
            }
    }

    return (result);
}



/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsCmgcSendSmsCommand
 *
 * Parameters:  (InOut) commandBuffer - a pointer to a CommandLine_t
 *              (In)    entity - index to the SMS context to use
 *
 * Returns:     result of command
 *
 * Description: Implements the CMGC command: send SMS command.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsCmgcSendSmsCommand (CommandLine_t *commandBuffer_p,
                                      const VgmuxChannelNumber  entity)
{
  SmsContext_t        *smsContext_p = ptrToSmsContext (entity);
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
  Int32               length;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
  memset(&smsContext_p->command, 0, sizeof(SmsCommandInfo));

  switch (operation)
  {
    case EXTENDED_QUERY:  /* AT+CMGC? */
    {
      result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
      break;
    }

    case EXTENDED_RANGE:  /* AT+CMGC=? */
    {
      break;
    }

    case EXTENDED_ASSIGN: /* AT+CMGC=... */
    {
      if ( vgSmsUtilIsInTextMode(entity) )
      {
        if ( TRUE == decodeCmgcParametersForTextMode(commandBuffer_p, entity) )
        {
          vgCiDataEntryModeInd (entity, DATA_ENTRY_SMS_MESSAGE);
          result = RESULT_CODE_PROCEEDING;
        }
        else
        {
          result = VG_CMS_ERROR_INVALID_TEXT_MODE_PARM;
        }
      }
      else /* PDU mode */
      {
        /* <length> parameter is mandatory in pdu mode */
        if ( (getExtendedParameter (commandBuffer_p, &length, ULONG_MAX)) &&
             (length > 0) &&
             (length <= VG_MAX_SMS_TDPU_SIZE) )
        {
          smsContext_p->tpduLength = (Int16) length;
          vgCiDataEntryModeInd (entity, DATA_ENTRY_SMS_MESSAGE);
          result = RESULT_CODE_PROCEEDING;
        }
        else
        {
          result = VG_CMS_ERROR_INVALID_PDU_MODE_PARM;
        }
      }
      break;
    }

    case EXTENDED_ACTION: /* AT+CMGC */
    {
      result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
      break;
    }

    default:
    {
      result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
      break;
    }
  }

  commandBuffer_p->position = commandBuffer_p->length;

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeSendStoreSmsParamters
 *
 * Parameters:  (InOut) commandBuffer - a pointer to a CommandLine_t
 *              (In)    entity - index to the SMS context to use
 *
 * Returns:     result of command
 *
 * Description: This function decodes the AT command parameters for both
 *              the CMGW (store SMS in SIM) and the CMGS (send SMS)
 *              commands.
 *
 * The parameters take the following format:
 *
 * a) in text mode: e.g. AT+CMGW="+447720598123"
 *
 * user then enters the SMS message terminated with <CTRL+Z>
 *
 * b) in PDU mode:  e.g. AT+CMGW=20
 *
 * user then enters the SMS data (including the header info etc)
 * in hex format and terminates it with <CTRL+Z>
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t decodeSendStoreSmsParamters( CommandLine_t *commandBuffer_p,
                                                 const VgmuxChannelNumber  entity)
{
    SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext ();
    SmsContext_t       *smsContext_p = ptrToSmsContext (entity);
    ResultCode_t        result = RESULT_CODE_ERROR;
    Char                address[VG_CR_SMS_MAX_ADDR_LEN + NULL_TERMINATOR_LENGTH] = {0};
    Char                timestamp[VG_CR_SMS_MAX_TIMESTAMP_LEN + NULL_TERMINATOR_LENGTH] = {0};
    Int16               addressLength = 0;
    Int16               timestampLength = 0;
    Int32               length;
    Int32               tmpInt32;
    Boolean             present;
    Int8                mti = DECODE_MTI(smsCommonContext_p->firstOctet);

#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    FatalCheck( (mti == SM_MTI_SUBMIT) || (mti == SM_MTI_DELIVER), mti, 0, 0);

    if( vgSmsUtilIsInTextMode(entity) )
    {
        if( (getExtendedString( commandBuffer_p,
                                address,
                                VG_CR_SMS_MAX_ADDR_LEN,
                                &addressLength) == TRUE) )
        {
            /* <oa/da> parameter is mandatory in text mode */
            if( addressLength != 0)
            {
                /* Convert string from current TE set.... */
                addressLength = vgMapTEToGsm(   getCrOutputBuffer(entity),
                                                VG_CR_CH_SET_OUTPUT_BUFFER_LEN,
                                                address,
                                                addressLength,
                                                (VgCSCSMode)getProfileValue(entity, PROF_CSCS),
                                                entity);

                if( (vgSmsUtilEncodeSmsDigits(  (Char *)getCrOutputBuffer(entity),
                                                addressLength,
                                                &smsContext_p->da) &&
                    (0 != smsContext_p->da.length) ) )
                {
                    result = RESULT_CODE_PROCEEDING; /* ok, otherwise use default of RESULT_CODE_ERROR */
                }

                resetCrOutputBuffer(entity);
            }
        }

        /* <tooa/toda>*/
        if( result == RESULT_CODE_PROCEEDING)
        {
            if( (getExtendedParameterPresent(   commandBuffer_p,
                                                &tmpInt32,
                                                VG_DIAL_NUMBER_UNKNOWN,
                                                &present) == FALSE) ||
                (tmpInt32 >= VG_DIAL_NUMBER_TYPE_MAX))
            {
                /* Invalid phone number type*/
                result = RESULT_CODE_ERROR;
            }
        }

        if( getCommandId (entity) == VG_AT_SMS_CMGW)
        {
            /* Set default status*/
            if( mti == SM_MTI_SUBMIT)
            {
                smsContext_p->smStatus = SIM_SMREC_ORIGINATED_NOT_SENT;
            }
            else
            {
                smsContext_p->smStatus = SIM_SMREC_RECEIVED_UNREAD;
            }
            /* Parse the SMS Status*/
            if( result == RESULT_CODE_PROCEEDING)
            {
                if( (getExtendedParameterPresent(   commandBuffer_p,
                                                    &tmpInt32,
                                                    VG_SMS_STATUS_UNREAD,
                                                    &present) == FALSE))
                {
                    /* Invalid SMS status*/
                    result = RESULT_CODE_ERROR;
                }
                else if( present == TRUE)
                {
                    switch( tmpInt32)
                    {
                        case VG_SMS_STATUS_UNREAD:
                            {
                                if( (mti == SM_MTI_SUBMIT))
                                {
                                    result = VG_CMS_ERROR_INVALID_PDU_MTI;
                                }
                                else
                                {
                                    smsContext_p->smStatus = SIM_SMREC_RECEIVED_UNREAD;
                                }
                            }
                            break;

                        case VG_SMS_STATUS_READ:
                            {
                                if( (mti == SM_MTI_SUBMIT))
                                {
                                    result = VG_CMS_ERROR_INVALID_PDU_MTI;
                                }
                                else
                                {
                                    smsContext_p->smStatus = SIM_SMREC_RECEIVED_READ;
                                }
                            }
                            break;

                        case VG_SMS_STATUS_UNSENT:
                            {
                                if( (mti == SM_MTI_DELIVER))
                                {
                                    result = VG_CMS_ERROR_INVALID_PDU_MTI;
                                }
                                else
                                {
                                    smsContext_p->smStatus = SIM_SMREC_ORIGINATED_NOT_SENT;
                                }
                            }
                            break;

                        case VG_SMS_STATUS_SENT:
                            {
                                if( (mti == SM_MTI_DELIVER))
                                {
                                    result = VG_CMS_ERROR_INVALID_PDU_MTI;
                                }
                                else
                                {
                                    smsContext_p->smStatus = SIM_SMREC_ORIGINATED_SENT;
                                }
                            }
                            break;

                        default:
                            {
                                result = RESULT_CODE_ERROR;
                            }
                            break;
                    }
                }
            }

            /* Parse the timestamp only when MTI is SMS-DELIVER*/
            if( (result == RESULT_CODE_PROCEEDING) &&
                (mti == SM_MTI_DELIVER))
            {
                if( (getExtendedString( commandBuffer_p,
                                        timestamp,
                                        VG_TIMESTAMP_SIZE,
                                        &timestampLength) == FALSE) )
                {
                    result = RESULT_CODE_ERROR;
                }
                else if( timestampLength != 0)
                {
                    /* Convert string from current TE set.... */
                    timestampLength = vgMapTEToGsm( getCrOutputBuffer(entity),
                                                    VG_CR_CH_SET_OUTPUT_BUFFER_LEN,
                                                    timestamp,
                                                    timestampLength,
                                                    (VgCSCSMode)getProfileValue(entity, PROF_CSCS),
                                                    entity);

                    if( vgSmsUtilEncodeTimestamp(   (Char *)getCrOutputBuffer(entity),
                                                    timestampLength,
                                                    &smsContext_p->smTimeStamp[0]) == FALSE )
                    {
                        result = RESULT_CODE_ERROR;
                    }

                    resetCrOutputBuffer(entity);
                }
                else
                {
                    memset( &smsContext_p->smTimeStamp[0], 0, sizeof(smsContext_p->smTimeStamp) );
                }
            }
        }
        else
        {
            /* Command id is CMSS*/
            if( mti == SM_MTI_DELIVER)
            {
                result = VG_CMS_ERROR_INVALID_PDU_MTI;
            }
        }
    }
    else /* pdu mode */
    {
        /* <length> parameter is mandatory in pdu mode */
        if( (getExtendedParameter (commandBuffer_p, &length, ULONG_MAX) == TRUE) &&
            (length > 0) &&
            (length <= VG_MAX_SMS_TDPU_SIZE) )
        {
            smsContext_p->tpduLength = (Int16) length;
            result = RESULT_CODE_PROCEEDING;
        }

        /* Parse the SMS Status (only for AT+CMGW) */
        if( (result == RESULT_CODE_PROCEEDING) &&
            (getCommandId (entity) == VG_AT_SMS_CMGW))
        {
            smsContext_p->smStatus = SIM_SMREC_INVALID_STATUS;
            if( (getExtendedParameterPresent(   commandBuffer_p,
                                                &tmpInt32,
                                                VG_SMS_MAX_STATUS,
                                                &present) == FALSE) )
            {
                /* Invalid SMS status*/
                result = RESULT_CODE_ERROR;
            }
            else if( present == TRUE)
            {
                if( (VgSmsStatus)tmpInt32 >= VG_SMS_MAX_STATUS)
                {
                    result = RESULT_CODE_ERROR;
                }
                else
                {
                    smsContext_p->smStatus = vgSmsUtilConvertVgStatToSimSmStat( (VgSmsStatus)tmpInt32);
                }
            }
        }
    }
    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsCmgsSendSms
 *
 * Parameters:  (InOut) commandBuffer - a pointer to a CommandLine_t
 *              (In)    entity - index to the SMS context to use
 *
 * Returns:     result of operation
 *
 * Description: This function executes the AT+CMGS command: send an SMS.
 *
 * In text mode it looks like this:
 *
 * AT+CMGS="+447720598123"<CR>
 * then enter messages text <CTRL+Z>
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsCmgsSendSms (CommandLine_t *commandBuffer_p,
                               const VgmuxChannelNumber  entity)
{
    ResultCode_t         result = RESULT_CODE_OK;
    ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
    SmsContext_t        *smsContext_p = ptrToSmsContext (entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    switch (operation)
    {
        case EXTENDED_QUERY:  /* AT+CMGS? */
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
                break;
            }

        case EXTENDED_RANGE:  /* AT+CMGS=? */
            {
                break;
            }

        case EXTENDED_ASSIGN: /* AT+CMGS=... */
            {
                result = decodeSendStoreSmsParamters(commandBuffer_p, entity);
                if ( RESULT_CODE_PROCEEDING == result )
                {
                    smsContext_p->validSms = TRUE;
                    vgCiDataEntryModeInd (entity, DATA_ENTRY_SMS_MESSAGE);
                }
                else
                {
                    result = VG_CMS_ERROR_INVALID_PARMETER;
                }
                break;
            }

        case EXTENDED_ACTION: /* AT+CMGS */
        {
            result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
            break;
        }

        default:
        {
            result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
            break;
        }
    }
    commandBuffer_p->position = commandBuffer_p->length;
    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsCmgwStoreSms
 *
 * Parameters:  (InOut) commandBuffer - a pointer to a CommandLine_t
 *              (In)    entity - index to the SMS context to use
 *
 * Returns:     result of command
 *
 * Description: This function executes the AT+CMGW command: store SMS in SIM
 *
 * This is just like the Send SMS command except the SMS is not actually
 * sent but stored in the SIM for later sending.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t  vgSmsCmgwStoreSms (CommandLine_t *commandBuffer_p,
                                 const VgmuxChannelNumber  entity)
{
    ResultCode_t        result        = RESULT_CODE_OK;
    ExtendedOperation_t operation     = getExtendedOperation (commandBuffer_p);
    SmsContext_t        *smsContext_p = ptrToSmsContext     (entity);
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    switch (operation)
    {
        case EXTENDED_QUERY:  /* AT+CMGW? */
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
                break;
            }

        case EXTENDED_RANGE:  /* AT+CMGW=? */
            {
                break;
            }

        case EXTENDED_ASSIGN: /* AT+CMGW=... */
            {
                // M_FrGkiPrintf0 (0xE778, GKI_ATCI_INFO, "pb improve: at+cmgw");
                GKI_TRACE0 (PB_IMPROVE_AT_CMGW, GKI_ATCI_INFO);

                result = decodeSendStoreSmsParamters( commandBuffer_p, entity);
                if ( RESULT_CODE_PROCEEDING == result )
                {
                    smsContext_p->validSms = TRUE;
                    vgCiDataEntryModeInd (entity, DATA_ENTRY_SMS_MESSAGE);
                }
                else
                {
                    result = VG_CMS_ERROR_INVALID_PARMETER;
                }
                break;
            }

        case EXTENDED_ACTION: /* AT+CMGW */
            {
                if (vgSmsUtilIsInTextMode(entity) == TRUE)
                {
                    /* User has not specified oa/da.... */
                    memset(smsContext_p->da.addressValue, 0x00, SMS_MAX_ADDR_LEN);
                    smsContext_p->da.length = 0;
                    smsContext_p->da.typeOfNumber = NUM_UNKNOWN;
                    smsContext_p->da.numberingPlan = PLAN_UNKNOWN;

                    /* Go into entry mode.... */
                    smsContext_p->validSms = TRUE;
                    vgCiDataEntryModeInd (entity, DATA_ENTRY_SMS_MESSAGE);
                    result = RESULT_CODE_PROCEEDING;
                }
                else
                {
                    result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
                }
                break;
            }

        default:
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
                break;
            }
    }
    commandBuffer_p->position = commandBuffer_p->length;
    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsCmgdDeleteSms
 *
 * Parameters:  (InOut) commandBuffer - a pointer to a CommandLine_t
 *
 * Returns:     result of operation
 *
 * Description: This function executes the AT+CMGD command which
 *              deletes the SMS with the specified index.
 *
 *              e.g. AT+CMGD=1  will deleted SMS with index 1
 *
 *-------------------------------------------------------------------------*/
ResultCode_t  vgSmsCmgdDeleteSms( CommandLine_t            *commandBuffer_p,
                                  const VgmuxChannelNumber  entity )
{
    SmsCommonContext_t  *smsCommonContext_p = ptrToSmsCommonContext();
    SmsContext_t        *smsContext_p       = ptrToSmsContext(entity);
    ResultCode_t         result             = RESULT_CODE_OK;
    ExtendedOperation_t  operation          = getExtendedOperation(commandBuffer_p);
    Int16                maxRecords         = 0;
    Int32                smsIdx = 0, delFlag;
    SmsSimAccessType     simAccessType;
    /* added for job103566 */
    SmRequestStatus      requestStatus;
    GeneralContext_t    *generalContext_p   = ptrToGeneralContext(entity);

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);
    FatalCheck(smsContext_p != PNULL, entity, 0, 0);

    switch( operation )
    {
        case EXTENDED_QUERY:  /* AT+CMGD? */
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
                break;
            }

        case EXTENDED_RANGE:  /* AT+CMGD=? */
            {
                /* job103566: test command implemented */
                /* if for any reason SIM is not ready, dont proceed */
                requestStatus = smsCommonContext_p->smSimState.requestStatus;

                if( requestStatus != SM_REQ_OK )
                {
                    result = vgSmsUtilConvertRequestError (entity, requestStatus);
                }
                else
                {
                    switch( smsContext_p->cpmsReadDelete )
                    {
                        case VG_CPMS_STORAGE_SM:
                            {
                                maxRecords = (Int16)getMaxSmsRecords( );
                            }
                            break;

                        case VG_CPMS_STORAGE_SR:
                            {
                                maxRecords = (Int16)getMaxSrRecords( entity);
                            }
                            break;

                        default:
                            {
                                /* Invalid enum value */
                                FatalParam(entity, smsContext_p->cpmsReadDelete, 0);
                            }
                            break;
                    }

                    vgSmsPrintNewline (entity);
                    vgSmsPuts(entity, (const Char *)"+CMGD: (1-");
                    vgSmsPrintInt16 (entity, maxRecords);
                    vgSmsPuts(entity, (const Char *)")");
                    vgSmsPrintNewline (entity);
                    vgSmsFlush(entity);
                }
                break;
            }

        case EXTENDED_ASSIGN: /* AT+CMGD=... */
            {
                // M_FrGkiPrintf0 (0xEB55, GKI_ATCI_INFO, "pb improve: at+cmgd");
                GKI_TRACE0 (PB_IMPROVE_AT_CMGD, GKI_ATCI_INFO);

                if( result == RESULT_CODE_OK)
                {
                    /* if for any reason SIM is not ready, dont proceed */
                    requestStatus = smsCommonContext_p->smSimState.requestStatus;

                    if( requestStatus != SM_REQ_OK )
                    {
                        result = vgSmsUtilConvertRequestError (entity, requestStatus);
                    }
                    else if( getExtendedParameter(commandBuffer_p, &smsIdx, ULONG_MAX) == FALSE )
                    {
                        result = VG_CMS_ERROR_INVALID_MEMORY_INDEX;
                    }
                }

                if( result == RESULT_CODE_OK)
                {
                    /* job118116 adds use of <delflag>, if delFlag is present and not set to 0, ignore index */
                    if( (getExtendedParameter (commandBuffer_p, &delFlag, 0) == FALSE) ||
                        (delFlag > 4))
                    {
                        result = VG_CMS_ERROR_INVALID_TEXT_MODE_PARM;
                    }
                    else if( delFlag > 0 )
                    {
                        /* convert <delflag> to SmsSimAccessType */
                        if( vgSmsUtilConvertDelFlagToSimAccessType(delFlag, &simAccessType) )
                        {
                            switch( smsContext_p->cpmsReadDelete )
                            {
                                case VG_CPMS_STORAGE_SM:
                                    {
                                        /* Remember that is multiple message delete request*/
                                        result = vgSmsSigOutApexReadSmsNoData(entity, simAccessType);

                                        if( result != RESULT_CODE_PROCEEDING )
                                        {
                                            result = VG_CMS_ERROR_UNKNOWN;
                                        }
                                        else
                                        {
                                            /* remember the delFlag value so we delete all SMS that
                                            ** are requested as per 27.005 */
                                            smsContext_p->delFlag = (Int8)delFlag;
                                        }
                                    }
                                    break;

                                case VG_CPMS_STORAGE_SR:
                                    {
                                        /* All SMS SR are assumed to have a "REC READ" Status */
                                        result = vgSmsSigOutApexReadSmsrNoData(entity, SM_NEXT_READ);

                                        if( result != RESULT_CODE_PROCEEDING )
                                        {
                                            result = VG_CMS_ERROR_UNKNOWN;
                                        }
                                        else
                                        {
                                            /* remember the delFlag value so we delete all SMS SR that
                                            ** are requested as per 27.005 */
                                            smsContext_p->delFlag = (Int8)delFlag;
                                        }
                                    }
                                    break;

                                default:
                                    {
                                        /* Invalid enum value */
                                        FatalParam(entity, smsContext_p->cpmsReadDelete, 0);
                                    }
                                    break;
                            }
                        }
                    }
                    /* delFlag is not present/relevant */
                    else
                    {
                        switch( smsContext_p->cpmsReadDelete )
                        {
                            case VG_CPMS_STORAGE_SM:
                                {
                                    /* job103566: don't allow index 0 */
                                    if( (smsIdx > 0) &&
                                        (smsIdx <= (Int32)smsCommonContext_p->smSimState.numberOfSmsRecords))
                                    {
                                        result = vgSmsSigOutApexDeleteSms( entity, (Int8)smsIdx );
                                    }
                                    else
                                    {
                                        result = VG_CMS_ERROR_INVALID_MEMORY_INDEX;
                                    }
                                }
                                break;

                            case VG_CPMS_STORAGE_SR:
                                {
                                    if( (smsIdx > 0) &&
                                        (smsIdx <= (Int32)smsCommonContext_p->smSimState.numberOfSmsrRecords))
                                    {
                                        result = vgSmsSigOutApexDeleteSmsr( entity, (Int8)smsIdx );
                                    }
                                    else
                                    {
                                        result = VG_CMS_ERROR_INVALID_MEMORY_INDEX;
                                    }
                                }
                                break;

                            default:
                                {
                                    /* Invalid enum value */
                                    FatalParam(entity, smsContext_p->cpmsReadDelete, 0);
                                }
                                break;
                        }
                    }
                }
                break;
            }

        case EXTENDED_ACTION: /* AT+CMGD */
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
                break;
            }

        default:
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
                break;
            }
    }

    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsCmgrReadSms
 *
 * Parameters:  (InOut) commandBuffer
 *
 * Returns:     result of operation
 *
 * Description: This function executes the AT+CMGR command which reads
 *              the specified SMS.
 *
 *   e.g.  AT+CMGR=1  will read the SMS with index 1
 *
 *   to get a list of SMS's then do:
 *
 *         AT+CMGF=1   to set into text mode (as opposed to PDU mode)
 *         AT+CMGL="ALL"  (or AT+CMGL=4 if in PDU mode)
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsCmgrReadSms( CommandLine_t             *commandBuffer_p,
                               const VgmuxChannelNumber   entity )
{
    SmsCommonContext_t     *smsCommonContext_p = ptrToSmsCommonContext();
    SmsContext_t           *smsContext_p       = ptrToSmsContext(entity);
    ResultCode_t            result             = RESULT_CODE_OK;
    ExtendedOperation_t     operation          = getExtendedOperation (commandBuffer_p);
    Int32                   index              = 0;
    Int16                   maxRecords         = 0;
    SmRequestStatus         requestStatus;

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);

    switch( operation )
    {
        case EXTENDED_QUERY:  /* AT+CMGR? */
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
            }
            break;

        case EXTENDED_RANGE:  /* AT+CMGR=? */
            break;

        case EXTENDED_ASSIGN: /* AT+CMGR=<index> */
        {
            switch( smsContext_p->cpmsReadDelete )
            {
                case VG_CPMS_STORAGE_SM:
                    {
                        maxRecords = (Int16)getMaxSmsRecords();
                    }
                    break;

                case VG_CPMS_STORAGE_SR:
                    {
                        /* Warning : there is SIM + ME SMS-SR records */
                        maxRecords = (Int16)getMaxSrRecords( entity);
                    }
                    break;

                default:
                    {
                        /* Invalid enum value */
                        FatalParam(entity, smsContext_p->cpmsReadDelete, 0);
                    }
                    break;
            }

            if( result == RESULT_CODE_OK )
            {
                /* if for any reason SIM is not ready, dont proceed */
                requestStatus = smsCommonContext_p->smSimState.requestStatus;

                if( requestStatus != SM_REQ_OK )
                {
                    result = vgSmsUtilConvertRequestError (entity, requestStatus);
                }
                /* <index> is mandatory parameter */
                else if( ( (getExtendedParameter (commandBuffer_p, &index, ULONG_MAX) == FALSE) ||
                           (index == 0) ||
                           (index > (Int32)maxRecords) ) )
                {
                    result = VG_CMS_ERROR_INVALID_MEMORY_INDEX;
                }
            }

            if( result == RESULT_CODE_OK )
            {
                // M_FrGkiPrintf2 (0xE4AB, GKI_ATCI_INFO, "pb improve: at+cmgr: %ld",index);
                GKI_TRACE1 (PB_IMPROVE_AT_CMGR, GKI_ATCI_INFO, index);
                switch( smsContext_p->cpmsReadDelete )
                {
                    case VG_CPMS_STORAGE_SM:
                        {
                            result = vgSmsSigOutApexReadSmsSetReadState(    entity,
                                                                            (Int8)index,
                                                                            smsContext_p->readState);
                            if( result != RESULT_CODE_PROCEEDING )
                            {
                                result = VG_CMS_ERROR_UNKNOWN;
                            }
                        }
                        break;

                    case VG_CPMS_STORAGE_SR:
                        {
                            if( index > smsCommonContext_p->smSimState.numberOfSmsrRecords )
                            {
                                /* We do not support this */
                                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
                            }
                            else
                            {
                                /* Read the SMS-SR from SIM*/
                                result = vgSmsSigOutApexReadSmsrSetReadState(   entity,
                                                                                (Int8)index,
                                                                                smsContext_p->readState);
                                if( result != RESULT_CODE_PROCEEDING )
                                {
                                    result = VG_CMS_ERROR_UNKNOWN;
                                }
                            }
                        }
                        break;

                    default:
                        {
                            /* Invalid enum value */
                            FatalParam(entity, smsContext_p->cpmsReadDelete, 0);
                        }
                        break;
                }
            }
        }
        break;

        case EXTENDED_ACTION: /* AT+CMGR */
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
            }
            break;

        default:
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
            }
            break;
    }
    return (result);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsMcmgrReadSms
 *
 * Parameters:  (InOut) commandBuffer
 *
 * Returns:     result of operation
 *
 * Description: This function executes the AT*MCMGR command which reads
 *              the specified SMS.
 *
 *   e.g.  AT*MCMGR=1  will read the SMS with index 1, but message status still be
 *          unread in SIM card.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsMcmgrReadSms( CommandLine_t             *commandBuffer_p,
                               const VgmuxChannelNumber   entity )
{
    SmsCommonContext_t     *smsCommonContext_p = ptrToSmsCommonContext();
    SmsContext_t           *smsContext_p       = ptrToSmsContext(entity);
    ResultCode_t            result             = RESULT_CODE_OK;
    ExtendedOperation_t     operation          = getExtendedOperation (commandBuffer_p);
    Int32                   index              = 0;
    Int16                   maxRecords         = 0;
    SmRequestStatus         requestStatus;

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);

    switch( operation )
    {
        case EXTENDED_QUERY:  /* AT*MCMGR? */
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
            }
            break;

        case EXTENDED_RANGE:  /* AT*MCMGR=? */
            break;

        case EXTENDED_ASSIGN: /* AT*MCMGR=<index> */
        {
            switch( smsContext_p->cpmsReadDelete )
            {
                case VG_CPMS_STORAGE_SM:
                    {
                        maxRecords = (Int16)getMaxSmsRecords();
                    }
                    break;

                case VG_CPMS_STORAGE_SR:
                    {
                        /* Warning : there is SIM + ME SMS-SR records */
                        maxRecords = (Int16)getMaxSrRecords( entity);
                    }
                    break;

                /*Not need to support BM/ME case.*/
                default:
                    {
                        /* Invalid enum value */
                        FatalParam(entity, smsContext_p->cpmsReadDelete, 0);
                    }
                    break;
            }

            if( result == RESULT_CODE_OK )
            {
                /* if for any reason SIM is not ready, dont proceed */
                requestStatus = smsCommonContext_p->smSimState.requestStatus;

                if( requestStatus != SM_REQ_OK )
                {
                    result = vgSmsUtilConvertRequestError (entity, requestStatus);
                }
                /* <index> is mandatory parameter */
                else if( ( (getExtendedParameter (commandBuffer_p, &index, ULONG_MAX) == FALSE) ||
                           (index == 0) ||
                           (index > (Int32)maxRecords) ) )
                {
                    result = VG_CMS_ERROR_INVALID_MEMORY_INDEX;
                }
            }

            if( result == RESULT_CODE_OK )
            {
                switch( smsContext_p->cpmsReadDelete )
                {
                    case VG_CPMS_STORAGE_SM:
                        {
                            result = vgSmsSigOutApexReadSmsSetReadState(    entity,
                                                                            (Int8)index,
                                                                            SM_PREVIEW_ONLY);
                            if( result != RESULT_CODE_PROCEEDING )
                            {
                                result = VG_CMS_ERROR_UNKNOWN;
                            }
                        }
                        break;

                    case VG_CPMS_STORAGE_SR:
                        {
                            if( index > smsCommonContext_p->smSimState.numberOfSmsrRecords )
                            {
                                /* Read the SMS-SR from ME*/
                                FatalParam(entity, index, smsCommonContext_p->smSimState.numberOfSmsrRecords);
                            }
                            else
                            {
                                /* Read the SMS-SR from SIM*/
                                result = vgSmsSigOutApexReadSmsrSetReadState(   entity,
                                                                                (Int8)index,
                                                                                SM_PREVIEW_ONLY);
                                if( result != RESULT_CODE_PROCEEDING )
                                {
                                    result = VG_CMS_ERROR_UNKNOWN;
                                }
                            }
                        }
                        break;

                    default:
                        {
                            /* Invalid enum value */
                            FatalParam(entity, smsContext_p->cpmsReadDelete, 0);
                        }
                        break;
                }
            }
        }
        break;

        case EXTENDED_ACTION: /* AT*MCMGR */
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
            }
            break;

        default:
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
            }
            break;
    }
    return (result);
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsCpmsPreferredSmsMessageStorage
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     Handle AT+CPMS command.
 *                  Set the preferred SMS message storage, either in the
 *                  SIM or in the phone memory.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSmsCpmsPreferredSmsMessageStorage( CommandLine_t             *commandBuffer_p,
                                                  const VgmuxChannelNumber   entity )
{
    SmsCommonContext_t     *smsCommonContext_p = ptrToSmsCommonContext ();
    SmsContext_t           *smsContext_p       = ptrToSmsContext(entity);
    ResultCode_t            result             = RESULT_CODE_OK;
    ExtendedOperation_t     operation          = getExtendedOperation (commandBuffer_p);
    VgCPMSParam_t           paramId;
    VgCPMSStorageType       storageType;
    Char                   *str_par = PNULL;
    Int16                   stringLength       = 0;
    Boolean                 firstMemType;
    SmRequestStatus         requestStatus;
    VgCPMSStorageType      *storage_p = PNULL;

    const Char * CpmsMemoryTag[] = {    (const Char *)"\"SM\"",
                                        (const Char *)"\"SR\""};

    FatalCheck (smsCommonContext_p != PNULL, entity, 0, 0);
    FatalCheck (smsContext_p != PNULL, entity, 0, 0);

#if defined (ENABLE_LONG_AT_CMD_RSP)
    KiAllocZeroMemory (sizeof(Char) * (AT_LARGE_BUFF_SIZE + NULL_TERMINATOR_LENGTH),
                        (void **)&str_par);
#else
    KiAllocZeroMemory (sizeof(Char) * (COMMAND_LINE_SIZE + NULL_TERMINATOR_LENGTH),
                        (void **)&str_par);
#endif

    switch (operation)
    {
        case EXTENDED_QUERY:       /* AT+CPMS? */
            {
                /* if for any reason SIM is not ready, dont proceed */
                requestStatus = smsCommonContext_p->smSimState.requestStatus;

                if( requestStatus != SM_REQ_OK )
                {
                    result = vgSmsUtilConvertRequestError(entity, requestStatus);
                }
                else
                {
                    vgSmsPrintNewline(entity);
                    vgSmsPuts(entity,(const Char *)("+CPMS: "));

                    for( paramId=VG_CPMS_READ_DELETE; paramId<VG_CPMS_MAX_MEM; paramId++ )
                    {
                        switch( paramId)
                        {
                            case VG_CPMS_READ_DELETE:
                                {
                                    storage_p = &smsContext_p->cpmsReadDelete;
                                }
                                break;

                            case VG_CPMS_WRITE_SEND:
                                {
                                    storage_p = &smsContext_p->cpmsWriteSend;
                                }
                                break;

                            case VG_CPMS_PREF_STORE:
                                {
                                    storage_p = &smsCommonContext_p->cpmsStore;
                                }
                                break;

                            default:
                                {
                                    /* Invalid enum value */
                                    FatalParam(entity, paramId, 0);
                                }
                                break;
                        }

                        switch( *storage_p )
                        {
                            case VG_CPMS_STORAGE_SM:
                                {
                                    vgSmsPuts(entity,(const Char *)CpmsMemoryTag[VG_CPMS_STORAGE_SM]);
                                    vgSmsPuts(entity,(const Char *)(","));
                                    vgSmsPrintInt16(entity, (Int16)getUsedSmsRecords());

                                    vgSmsPuts(entity,(const Char *)(","));
                                    vgSmsPrintInt16(entity, (Int16)getMaxSmsRecords());
                                }
                                break;

                            case VG_CPMS_STORAGE_SR:
                                {
                                    vgSmsPuts(entity,(const Char *)CpmsMemoryTag[VG_CPMS_STORAGE_SR]);
                                    vgSmsPuts(entity,(const Char *)(","));
                                    vgSmsPrintInt16(entity, (Int16)getUsedSrRecords( entity));

                                    vgSmsPuts(entity,(const Char *)(","));
                                    vgSmsPrintInt16(entity, (Int16)getMaxSrRecords( entity));
                                }
                                break;

                            default:
                                {
                                    /* Invlid enum value */
                                    FatalParam(entity, *storage_p, 0);
                                }
                                break;
                        }

                        if( paramId < (VG_CPMS_MAX_MEM-1) )
                        {
                            vgSmsPuts(entity,(const Char *)(","));
                        }
                    }
                    vgSmsPrintNewline(entity);
                    vgSmsFlush(entity);
                }
            }
            break;

        case EXTENDED_RANGE:  /* AT+CPMS=? */
            {
                vgSmsPrintNewline(entity);
                vgSmsPuts(entity,(const Char *)("+CPMS: ("));

                for( paramId=VG_CPMS_READ_DELETE; paramId<VG_CPMS_MAX_MEM; paramId++ )
                {
                    firstMemType = TRUE;

                    for( storageType=VG_CPMS_STORAGE_SM; storageType<VG_CPMS_MAX_STORAGE_TYPE; storageType++ )
                    {
                        switch( storageType )
                        {
                            case VG_CPMS_STORAGE_SM:
                                {
                                    if( getMaxSmsRecords() != 0 )
                                    {
                                        firstMemType = FALSE;
                                        vgSmsPuts(entity,(const Char *)CpmsMemoryTag[storageType]);
                                    }
                                }
                                break;

                            case VG_CPMS_STORAGE_SR:
                                {
                                    if( (getMaxSrRecords( entity) != 0) &&
                                        (paramId == VG_CPMS_READ_DELETE) )
                                    {
                                        if( firstMemType )
                                        {
                                            firstMemType = FALSE;
                                        }
                                        else
                                        {
                                            vgSmsPuts(entity,(const Char *)(","));
                                        }

                                        vgSmsPuts(entity,(const Char *)CpmsMemoryTag[storageType]);
                                    }
                                }
                                break;

                            default:
                                {
                                    /* Invalid enum value */
                                    FatalParam(entity, storageType, 0);
                                }
                                break;
                        }
                    }

                    if( paramId < (VG_CPMS_MAX_MEM-1) )
                    {
                        vgSmsPuts(entity,(const Char *)("),("));
                    }
                }

                vgSmsPuts(entity,(const Char *)(")"));
                vgSmsPrintNewline(entity);
                vgSmsFlush(entity);
            }
            break;

        case EXTENDED_ASSIGN: /* AT+CPMS=<mem1>[,<mem2>[,<mem3>]] */
            {
                /* if for any reason SIM is not ready, dont proceed */
                requestStatus = smsCommonContext_p->smSimState.requestStatus;

                // M_FrGkiPrintf0 (0xF490, GKI_ATCI_INFO, "pb improve: at+cpms");
                GKI_TRACE0 (PB_IMPROVE_AT_CPMS, GKI_ATCI_INFO);
                if( requestStatus != SM_REQ_OK )
                {
                    result = vgSmsUtilConvertRequestError(entity, requestStatus);
                }
                else
                {
                    for(    paramId = VG_CPMS_READ_DELETE;
                            (   (paramId < VG_CPMS_MAX_MEM) &&
                                (RESULT_CODE_OK == result) );
                            paramId++ )
                    {
                        if( getExtendedString(  commandBuffer_p,
                                                str_par,

#if defined (ENABLE_LONG_AT_CMD_RSP)
                                                AT_LARGE_BUFF_SIZE,
#else
                                                COMMAND_LINE_SIZE,
#endif
                                                &stringLength) == TRUE )
                        {
                            switch( paramId )
                            {
                                case VG_CPMS_READ_DELETE:
                                    {
                                        if( stringLength != 0)
                                        {
                                            if( (0 == memcmp(str_par,"SM",2)) &&
                                                (getMaxSmsRecords() != 0) )
                                            {
                                                smsContext_p->cpmsReadDelete = VG_CPMS_STORAGE_SM;
                                            }
                                            else if(    (0 == memcmp(str_par,"SR",2)) &&
                                                        (getMaxSrRecords( entity) != 0) )
                                            {
                                                smsContext_p->cpmsReadDelete = VG_CPMS_STORAGE_SR;
                                            }
                                            else
                                            {
                                                /* <mem1> parameter is mandatory, others are optional */
                                                result = VG_CMS_ERROR_INVALID_PARMETER;
                                            }
                                        }
                                        else
                                        {
                                            result = VG_CMS_ERROR_INVALID_PARMETER;
                                        }
                                    }
                                    break;

                                case VG_CPMS_WRITE_SEND:
                                    {
                                        if( stringLength != 0)
                                        {
                                            if( (0 == memcmp(str_par,"SM",2)) &&
                                                (getMaxSmsRecords() != 0) )
                                            {
                                                smsContext_p->cpmsWriteSend = VG_CPMS_STORAGE_SM;
                                            }
                                            else
                                            {
                                                result = VG_CMS_ERROR_INVALID_PARMETER;
                                            }
                                        }
                                    }
                                    break;

                                case VG_CPMS_PREF_STORE:
                                    {
                                        if( stringLength != 0)
                                        {
                                            if( (0 == memcmp(str_par,"SM",2)) &&
                                                (getMaxSmsRecords() != 0) )
                                            {
                                                smsCommonContext_p->cpmsStore = VG_CPMS_STORAGE_SM;
                                            }
                                            else
                                            {
                                                result = VG_CMS_ERROR_INVALID_PARMETER;
                                            }
                                        }
                                    }
                                    break;

                                default:
                                    {
                                        result = VG_CMS_ERROR_INVALID_PARMETER;
                                    }
                                    break;
                            }
                        }
                        else
                        {
                            result = VG_CMS_ERROR_INVALID_PARMETER;
                        }
                    }

                    if( RESULT_CODE_OK == result )
                    {
                        vgSmsPrintNewline(entity);
                        vgSmsPuts(entity, (const Char*)"+CPMS: ");

                        for( paramId = VG_CPMS_READ_DELETE; paramId < VG_CPMS_MAX_MEM; paramId++ )
                        {
                            switch( paramId)
                            {
                                case VG_CPMS_READ_DELETE:
                                    {
                                        storage_p = &smsContext_p->cpmsReadDelete;
                                    }
                                    break;

                                case VG_CPMS_WRITE_SEND:
                                    {
                                        storage_p = &smsContext_p->cpmsWriteSend;
                                    }
                                    break;

                                case VG_CPMS_PREF_STORE:
                                    {
                                        storage_p = &smsCommonContext_p->cpmsStore;
                                    }
                                    break;

                                default:
                                    {
                                        /* Invalid enum value */
                                        FatalParam(entity, paramId, 0);
                                    }
                                    break;
                            }

                            switch( *storage_p )
                            {
                                case VG_CPMS_STORAGE_SM:
                                    {
                                        vgSmsPrintInt16(entity, (Int16)getUsedSmsRecords());
                                        vgSmsPuts(entity,(const Char *)(","));
                                        vgSmsPrintInt16(entity, (Int16)getMaxSmsRecords());
                                    }
                                    break;
                                case VG_CPMS_STORAGE_SR:
                                    {
                                        vgSmsPrintInt16(entity, (Int16)getUsedSrRecords( entity));
                                        vgSmsPuts(entity,(const Char *)(","));
                                        vgSmsPrintInt16(entity, (Int16)getMaxSrRecords( entity));
                                    }
                                    break;

                                default:
                                    {
                                        /* Invalid enum value */
                                        FatalParam(entity, *storage_p, 0);
                                    }
                                    break;
                            }

                            if( paramId < (VG_CPMS_MAX_MEM-1) )
                            {
                                vgSmsPuts(entity,(const Char *)(","));
                            }
                        }

                        vgSmsPrintNewline(entity);
                        vgSmsFlush(entity);

                        // Send new parameters to AB
                        vgApexSendSmProfileChangedReq( entity);
                    }
                }
            }
            break;

        case EXTENDED_ACTION:      /* AT+CPMS */
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
            }
            break;

        default:
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
            }
            break;
    }

    KiFreeMemory( (void **)&str_par);

    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsCnmaNewMessageAcknowledgment
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     Handle AT+CNMA command.
 *                  Confirms correct reception of a new message which is
 *                  routed directly to the TE
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSmsCnmaNewMessageAcknowledgment (CommandLine_t *commandBuffer_p,
                                                const VgmuxChannelNumber  entity)
{
    ResultCode_t            result              = RESULT_CODE_OK;
    ExtendedOperation_t     operation           = getExtendedOperation (commandBuffer_p);
    SmsCommonContext_t     *smsCommonContext_p  = ptrToSmsCommonContext();
    SmsContext_t           *smsContext_p        = ptrToSmsContext (entity);
    Int32                   param               = 0;
    Int32                   length              = 0;
    Boolean                 present             = FALSE;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(smsContext_p != PNULL, entity, 0, 0);
#endif
    /* shall be used only with +CSMS parameter <service> equals 1 */
    if (VG_SMS_PDU_WITH_CNMA_ACK == getProfileValue(entity, PROF_CSMS))
    {
        switch (operation)
        {
            case EXTENDED_QUERY:       /* AT+CNMA? */
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
            }
            break;

            case EXTENDED_RANGE:  /* AT+CNMA=? */
            {
                /* In text mode OK, in PDU mode list of supported <n>s*/
                if ( FALSE == vgSmsUtilIsInTextMode(entity) )
                {
                    vgSmsPrintNewline(entity);
                    vgSmsPuts(entity, (const Char*)"+CNMA: (0-2)");
                    vgSmsPrintNewline(entity);
                    vgSmsFlush(entity);
                }
            }
            break;

            case EXTENDED_ASSIGN: /* AT+CNMA=... */
            {
                /* if there is no SMS to acknowledge, error */
                if (smsCommonContext_p->msgToAck != TRUE)
                {
                    result = VG_CMS_ERROR_OP_NOT_ALLOWED;
                }
                /* assign supported only in PDU mode */
                else if ( FALSE == vgSmsUtilIsInTextMode(entity) )
                {
                    if( (getExtendedParameter (commandBuffer_p, &param, ULONG_MAX)) &&
                        (param < 3) )
                    {
                      /* Do nothing */
                    }
                    else
                    {
                        result = VG_CMS_ERROR_INVALID_PDU_MODE_PARM;
                    }

                    if( result == RESULT_CODE_OK)
                    {
                        /* read optional <length> parameter */
                        if( (getExtendedParameterPresent(   commandBuffer_p,
                                                           &length,
                                                            ULONG_MAX,
                                                           &present) == TRUE) )
                        {
                            if( present == TRUE)
                            {
                                if( (length > 0) &&
                                    (length <= VG_MAX_SMS_TDPU_SIZE) )
                                {
                                    /* We have to read the PDU now*/
                                    smsContext_p->tpduLength = (Int16) length;
                                    result = RESULT_CODE_PROCEEDING;
                                }
                                else
                                {
                                    result = VG_CMS_ERROR_INVALID_PDU_MODE_PARM;
                                }
                            }
                            else
                            {
                                /* No PDU specified, so the default empty reply will be sent*/
                                result = RESULT_CODE_OK;
                            }
                        }
                        else
                        {
                            result = VG_CMS_ERROR_INVALID_PDU_MODE_PARM;
                        }
                    }

                    if( RESULT_CODE_PROCEEDING == result )
                    {
                        /* Read the SMS-DELIVER-REPORT pdu*/
                        smsContext_p->validSms = TRUE;
                        smsCommonContext_p->msgStatus = (param == 2 ? TRANSFER_ERROR : TRANSFER_OK);
                        vgCiDataEntryModeInd (entity, DATA_ENTRY_SMS_MESSAGE);
                    }
                    else if( result == RESULT_CODE_OK)
                    {
                        if (param == 2)
                        {
                            /* send confirmation back to the ABSM, error */
                            vgApexSendSmDeliveryRsp (smsCommonContext_p->shortMsgId, FALSE);
                            smsCommonContext_p->msgToAck = FALSE;
                        }
                        else
                        {
                            /* send confirmation back to the ABSM, ack */
                            vgApexSendSmDeliveryRsp (smsCommonContext_p->shortMsgId, TRUE);
                            smsCommonContext_p->msgToAck = FALSE;
                        }
                    }
                }
                else
                {
                    result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
                }
            }
            break;

            case EXTENDED_ACTION:      /* AT+CNMA */
            {
                /* behaviour is the same in text and pdu modes */
                /* if there is no SMS to acknowledge, error */
                if (smsCommonContext_p->msgToAck == TRUE)
                {
                    /* send confirmation back to the ABSM */
                    vgApexSendSmDeliveryRsp (smsCommonContext_p->shortMsgId, TRUE);
                    smsCommonContext_p->msgToAck = FALSE;
                }
                else
                {
                    result = VG_CMS_ERROR_OP_NOT_ALLOWED;
                }
            }
            break;

            default:
            {
                result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
            }
            break;
        }
    }
    else
    {
        result = VG_CMS_ERROR_OP_NOT_ALLOWED;
    }

    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSmsCmmsSendMoreSms
 *
 * Parameters:  (InOut) commandBuffer - a pointer to a CommandLine_t
 *              (In)    entity - index to the SMS context to use
 *
 * Returns:     result of operation
 *
 * Description: This function executes the AT+CMMS command: setting CMMS mode.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsCmmsSendMoreSms (CommandLine_t *commandBuffer_p,
                                   const VgmuxChannelNumber  entity)
{
  ResultCode_t         result = RESULT_CODE_OK;
  ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
  SmsCommonContext_t   *smsCommonContext_p = ptrToSmsCommonContext();
  Int32 param;

  switch (operation)
  {
    case EXTENDED_QUERY:  /* AT+CMMS? */
    {

      vgSmsPrintNewline(entity);
      vgSmsPuts(entity, (const Char*)"+CMMS: ");
      vgSmsPrintInt16(entity, (Int16)smsCommonContext_p->moreMsgsToSendMode);
      vgSmsPrintNewline(entity);
      vgSmsFlush(entity);
      break;
    }

    case EXTENDED_RANGE:  /* AT+CMMS=? */
    {
      vgSmsPrintNewline (entity);
      vgSmsPuts (entity, (const Char*)"+CMMS: (0-2)");
      break;
    }

    case EXTENDED_ASSIGN: /* AT+CMMS=... */
    {
      if ((getExtendedParameter (commandBuffer_p, &param, ULONG_MAX)) &&
          (param <= 2))
      {
        if (smsCommonContext_p->moreMsgsToSendMode != param)
        {
          smsCommonContext_p->moreMsgsToSendMode = (Int8)param;
          vgSigApexSmSendMoreReq (entity);
        }
        if (smsCommonContext_p->moreMsgsToSendMode == 0)
        {
          vgCiStopTimer(TIMER_TYPE_SMS_CMMS);
        }
      }
      else
      {
        result = VG_CMS_ERROR_INVALID_PDU_MODE_PARM;
      }
      break;
    }

    default:
    {
      result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
      break;
    }
  }
  /* AT commands concatenation should be allowed after this AT command */
  return (result);
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsMSmStatus
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description: This function executes the AT*MSMSTATUS command obtains the
 *              status of the SMS functionality
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsMSmStatus (CommandLine_t *commandBuffer_p,
                               const VgmuxChannelNumber  entity)
{
  ResultCode_t        result = RESULT_CODE_ERROR;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);

  switch (operation)
  {
    case EXTENDED_QUERY:  /* AT*MSMSTATUS? */
    {
      result = vgSmsSigOutApexSmStatusReq(entity);
      break;
    }
    case EXTENDED_RANGE:  /* AT*MSMSTATUS=? */
    case EXTENDED_ASSIGN: /* AT*MSMSTATUS=... */
    case EXTENDED_ACTION: /* AT*MSMSTATUS... */
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
 * Function:        vgSmsMmgrwSelLoc
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description: This function executes the AT*MMGRW command to select a SMS
 *              location in SIM for the next SMS storage
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsMmgrwSelLoc (CommandLine_t *commandBuffer_p,
                               const VgmuxChannelNumber  entity)
{
  ResultCode_t          result = RESULT_CODE_OK;
  SmsCommonContext_t   *smsCommonContext_p = ptrToSmsCommonContext ();
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  Int32                 index               = 0;
  SmRequestStatus       requestStatus;


  switch (operation)
  {
    case EXTENDED_QUERY:  /* AT*MMGRW? */
    {
      result = vgSmsSigOutApexSmStatusReq(entity);
      break;
    }

    case EXTENDED_RANGE:  /* AT*MMGRW=? */
    {
      vgSmsPrintNewline(entity);
      vgSmsPuts(entity, (const Char*)"*MMGRW: (1-");
      vgSmsPrintInt16(entity, (Int16)smsCommonContext_p->smSimState.numberOfSmsRecords);
      vgSmsPuts(entity, (const Char*)")");
      vgSmsPrintNewline(entity);
      vgSmsFlush(entity);
      break;
    }
    case EXTENDED_ASSIGN: /* AT*MMGRW=... */
    {
      /* if for any reason SIM is not ready, dont proceed */
      requestStatus = smsCommonContext_p->smSimState.requestStatus;

      if (requestStatus != SM_REQ_OK)
      {
        result = vgSmsUtilConvertRequestError (entity, requestStatus);
      }
      else if ( ((getExtendedParameter (commandBuffer_p, &index, ULONG_MAX) == FALSE) ||
                 (index > (Int32)smsCommonContext_p->smSimState.numberOfSmsRecords)) )
      {
        result = VG_CMS_ERROR_INVALID_MEMORY_INDEX;
      }

      if (result == RESULT_CODE_OK)
      {
        result = vgSmsSigOutApexStoreLocReq(entity, (Int16)index);
        if (result != RESULT_CODE_PROCEEDING)
        {
          result = VG_CMS_ERROR_UNKNOWN;
        }
      }

      break;
    }
    case EXTENDED_ACTION: /* AT*MMGRW... */
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
 * Function:        vgSmsMmgscChangeLocStatus
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description: This function executes the AT*MMGSC command to change the
 *              current SMS location status value in SIM
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsMmgscChangeLocStatus (CommandLine_t *commandBuffer_p,
                               const VgmuxChannelNumber  entity)
{
  ResultCode_t          result = RESULT_CODE_OK;
  SimSmRecordStatus     smsStatus;
  Char                 *str_par = PNULL;
  Int16                 stringLength = 0;
  Int32                 pduMode;
  Int32                 loc        = 0;
  SmsCommonContext_t   *smsCommonContext_p = ptrToSmsCommonContext ();
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);

#if defined (ENABLE_LONG_AT_CMD_RSP)
  KiAllocZeroMemory (sizeof(Char) * (AT_LARGE_BUFF_SIZE + NULL_TERMINATOR_LENGTH),
                       (void **)&str_par);
#else
  KiAllocZeroMemory (sizeof(Char) * (COMMAND_LINE_SIZE + NULL_TERMINATOR_LENGTH),
                       (void **)&str_par);
#endif

  switch (operation)
  {
    case EXTENDED_ASSIGN: /* AT*MMGSC=... */
    {
      if ( vgSmsUtilIsInTextMode(entity) ) /* TEXT mode*/
      {
        if ((getExtendedString(commandBuffer_p,
                                str_par,
                                
#if defined (ENABLE_LONG_AT_CMD_RSP)
                                AT_LARGE_BUFF_SIZE,
#else
                                COMMAND_LINE_SIZE,
#endif
                                &stringLength) == TRUE) &&
                              (stringLength != 0))
        {
          if (vgSmsUtilConvertStatusStringToSmsStatus(str_par, &smsStatus))
          {
            if ((getExtendedParameter (commandBuffer_p, &loc, ULONG_MAX) == TRUE) &&
               (loc <= smsCommonContext_p->smSimState.numberOfSmsRecords))
            {
              result = vgSmsSigOutApexSetLocStatusReq(entity, (Int16)loc, smsStatus);
              if (result != RESULT_CODE_PROCEEDING)
              {
                result = VG_CMS_ERROR_UNKNOWN;
              }
            }
            else
            {
             result = VG_CMS_ERROR_INVALID_MEMORY_INDEX;
            }
          }
          else
          {
            result = VG_CMS_ERROR_INVALID_PARMETER;
          }
        }
        else
        {
          result = VG_CMS_ERROR_INVALID_PARMETER;
        }
      }
      else /* PDU mode */
      {
        if ((getExtendedParameter (commandBuffer_p, &pduMode, ULONG_MAX) == TRUE))
        {
          if (vgSmsUtilConvertPduIdxToSmsStatus(pduMode, &smsStatus) == TRUE)
          {
            if ((getExtendedParameter (commandBuffer_p, &loc, ULONG_MAX) == TRUE) &&
               (loc <= smsCommonContext_p->smSimState.numberOfSmsRecords))
            {
              result = vgSmsSigOutApexSetLocStatusReq(entity, (Int16)loc, smsStatus);
              if (result != RESULT_CODE_PROCEEDING)
              {
                result = VG_CMS_ERROR_UNKNOWN;
              }
            }
            else
            {
             result = VG_CMS_ERROR_INVALID_MEMORY_INDEX;
            }
          }
          else
          {
            result = VG_CMS_ERROR_INVALID_PARMETER;
          }
        }

      }
      break;
    }

    case EXTENDED_QUERY:  /* AT*MMGSC? */
    case EXTENDED_RANGE:  /* AT*MMGSC=? */
    case EXTENDED_ACTION: /* AT*MMGSC... */
    default:
    {
      result = VG_CME_OPERATION_NOT_ALLOWED;
      break;
    }
  }

  KiFreeMemory( (void **)&str_par);

  return (result);
}

#if defined (FEA_PHONEBOOK)
/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsMSmAlphaId
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     Non-standard AT command to enable/disable the AlphaId
 *                  lookup for phonenumbers when displaying SMS's
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSmsMSmAlphaId (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber  entity)
{
  SmsCommonContext_t   *smsCommonContext_p = ptrToSmsCommonContext ();
  ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
  ResultCode_t         result = RESULT_CODE_OK;

  Boolean ok;
  Int8    val;

  switch (operation)
  {
    case EXTENDED_QUERY:
    {
      vgSmsPrintNewline(entity);
      vgSmsPuts  (entity, (const Char*)getCommandName(entity));
      vgSmsPuts  (entity, (const Char*)": ");
      vgSmsPrintInt16(entity, (Int16)smsCommonContext_p->enableAlphaId);
      vgSmsPrintNewline(entity);
      vgSmsFlush(entity);
      result = RESULT_CODE_OK;
      break;
    }

    case EXTENDED_RANGE:
    {
      vgSmsPrintNewline(entity);
      vgSmsPuts (entity, (const Char*)getCommandName(entity));
      vgSmsPuts (entity, (const Char*)": (0,1)");
      vgSmsPrintNewline(entity);
      vgSmsFlush(entity);
      result = RESULT_CODE_OK;
      break;
    }

    case EXTENDED_ASSIGN:
    {
      val = getExtendedDecimalInt8(commandBuffer_p, &ok,    99);
      if ( (TRUE==ok) && ((0==val) || (1==val)) )
      {
        smsCommonContext_p->enableAlphaId = (Boolean)((val==1)?TRUE:FALSE);
        result = RESULT_CODE_OK;
      }
      else
      {
        result=RESULT_CODE_ERROR;
      }
      break;
    }

    case EXTENDED_ACTION: /* fall through... */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);
}
#endif /* FEA_PHONEBOOK */

/*--------------------------------------------------------------------------
 *
 * Function:        vgSmsMSmExtraInfo
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     Non-standard AT command to enable/disable the
 *                  extra non-standard information on some commands and
 *                  messages
 *
 *             e.g. Adds an extra field onto the AT+CSCA command:
 *
 *                  +CSCA: "+447802000332",145,"BT Cellnet SMS"
 *                                             ^^^^^^^^^^^^^^^^ extra info
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSmsMSmExtraInfo (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber  entity)
{
  SmsCommonContext_t   *smsCommonContext_p = ptrToSmsCommonContext ();
  ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
  ResultCode_t         result = RESULT_CODE_OK;

  Boolean ok;
  Int8    val;

  switch (operation)
  {
    case EXTENDED_QUERY:
    {
      vgSmsPuts  (entity, (const Char*)getCommandName(entity));
      vgSmsPuts  (entity, (const Char*)": ");
      vgSmsPrintInt16(entity, (Int16)smsCommonContext_p->enableExtraInfo);
      vgSmsPrintNewline(entity);
      vgSmsFlush(entity);
      result = RESULT_CODE_OK;
      break;
    }

    case EXTENDED_RANGE:
    {
      vgSmsPrintNewline(entity);
      vgSmsPuts (entity, (const Char*)getCommandName(entity));
      vgSmsPuts (entity, (const Char*)": (0,1)");
      vgSmsPrintNewline(entity);
      vgSmsFlush(entity);
      result = RESULT_CODE_OK;
      break;
    }

    case EXTENDED_ASSIGN:
    {
      val = getExtendedDecimalInt8(commandBuffer_p, &ok,    99);
      if ( (TRUE==ok) && ((0==val) || (1==val)) )
      {
        smsCommonContext_p->enableExtraInfo = (Boolean)((val==1)?TRUE:FALSE);
        result = RESULT_CODE_OK;
      }
      else
      {
        result=RESULT_CODE_ERROR;
      }
      break;
    }

    case EXTENDED_ACTION: /* fall through... */
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
 * Function:        vgSmsMSmExtraUnsol
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     Non-standard AT command to enable/disable the
 *                  extra unsolicited messages.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgSmsMSmExtraUnsol (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber  entity)
{
  SmsContext_t         *smsContext_p = ptrToSmsContext (entity);
  ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
  ResultCode_t         result = RESULT_CODE_OK;

  Boolean ok;
  Int8    val;

  FatalCheck (smsContext_p != PNULL, entity, 0, 0);

  switch (operation)
  {
    case EXTENDED_QUERY:
    {
      vgSmsPuts  (entity, (const Char*)getCommandName(entity));
      vgSmsPuts  (entity, (const Char*)": ");
      vgSmsPrintInt16(entity, (Int16)smsContext_p->enableExtraUnsol);
      vgSmsPrintNewline(entity);
      vgSmsFlush(entity);
      result = RESULT_CODE_OK;
      break;
    }

    case EXTENDED_RANGE:
    {
      vgSmsPrintNewline(entity);
      vgSmsPuts (entity, (const Char*)getCommandName(entity));
      vgSmsPuts (entity, (const Char*)": (0,1)");
      vgSmsPrintNewline(entity);
      vgSmsFlush(entity);
      result = RESULT_CODE_OK;
      break;
    }

    case EXTENDED_ASSIGN:
    {
      val = getExtendedDecimalInt8(commandBuffer_p, &ok,    99);
      if ( (TRUE==ok) && ((0==val) || (1==val)) )
      {
        smsContext_p->enableExtraUnsol = (Boolean)((val==1)?TRUE:FALSE);
        result = RESULT_CODE_OK;
      }
      else
      {
        result=RESULT_CODE_ERROR;
      }
      break;
    }

    case EXTENDED_ACTION: /* fall through... */
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
 * Function:        vgSmsMMGI
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     Non-standard AT command to enable/disable the SMS records
 *                  access unsolicited messages.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgSmsMMGI (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber  entity)
{
    ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
    SmsCommonContext_t  *smsCommonContext_p = ptrToSmsCommonContext ();
    ResultCode_t        result = RESULT_CODE_OK;
    Int32               mode;
    Int32               event;
    VgProfileBit        profilebit;
    Int8                i;

    switch (operation)
    {
        case EXTENDED_RANGE:  /* AT*MMGI=? */
            {
                vgPutNewLine(entity);
                vgPrintf(entity, (const Char*)"%C: (0-2),(0-1)");
                vgPutNewLine(entity);
            }
            break;

        case EXTENDED_ASSIGN: /* AT*MMGI=... */
            {
                if ( getExtendedParameter (commandBuffer_p, &mode, ULONG_MAX) != TRUE )
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else if ( getExtendedParameter (commandBuffer_p, &event, ULONG_MAX) != TRUE)
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                    if(event<VG_MMGI_NB_PROF_MAX)
                    {
                        profilebit = (VgProfileBit)(1<<event);
                        switch (mode)
                        {
                            case REPORTING_DISABLED:
                                {
                                    result = setProfileValueBit (entity, PROF_MMGI, profilebit, (Int8)mode);
                                }
                                break;
                            case REPORTING_ENABLED:
                                {
                                    result = setProfileValueBit (entity, PROF_MMGI, profilebit, (Int8)mode);
                                    if(TRUE == smsCommonContext_p->smSimStateInitialized)
                                    {
                                        vgMMGIPrintEvent(VG_MMGI_SIM_ACCESS_OK);
                                    }
                                }
                                break;

                            case REPORTING_QUERY:
                                {
                                    vgPutNewLine (entity);
                                    vgPrintf(   entity,
                                                (const Char*)"%C: %d",
                                                getProfileValueBit (entity, PROF_MMGI, profilebit));
                                    vgPutNewLine (entity);
                                }
                                break;

                            default:
                                {
                                    result = VG_CME_INVALID_INPUT_VALUE;
                                }
                                break;
                        }
                    }
                    else if(event==ULONG_MAX) // no event specified
                    {
                        switch (mode)
                        {
                            case REPORTING_DISABLED:
                            case REPORTING_ENABLED:
                                {
                                    result = RESULT_CODE_OK;
                                    for (i=0; i<VG_MMGI_NB_PROF_MAX && result==RESULT_CODE_OK; i++)
                                    {
                                        profilebit= (VgProfileBit)(1<<i);
                                        result = setProfileValueBit (entity, PROF_MMGI, profilebit, (Int8)mode);
                                    }
                                }
                                break;

                            case REPORTING_QUERY:
                                {
                                    for (i=0; i<VG_MMGI_NB_PROF_MAX; i++)
                                    {
                                        profilebit= (VgProfileBit)(1<<i);
                                        vgPutNewLine (entity);
                                        vgPrintf(   entity,
                                                    (const Char*)"%C: %d, %d",
                                                    i,
                                                    getProfileValueBit (entity, PROF_MMGI, profilebit));
                                        vgPutNewLine (entity);
                                    }
                                }
                                break;

                            default:
                                {
                                    result = VG_CME_INVALID_INPUT_VALUE;
                                }
                                break;
                        }
                    }
                    else
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
                    }
                }
            }
            break;

        case EXTENDED_QUERY:  /* AT*MMGI? */
            {
                for (i=0; i<VG_MMGI_NB_PROF_MAX; i++)
                {
                    profilebit= (VgProfileBit)(1<<i);
                    vgPutNewLine (entity);
                    vgPrintf(   entity,
                                (const Char*)"%C: %d, %d",
                                i,
                                getProfileValueBit (entity, PROF_MMGI, profilebit));
                    vgPutNewLine (entity);
                }
            }
            break;

        case EXTENDED_ACTION: /* AT*MMGI... */
        default:
            {
                result = RESULT_CODE_ERROR;
            }
            break;
    }

    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgMMGIPrintEvent
 *
 * Parameters:  vgMMGIEvent - MMGI event to handle
 *
 * Returns:     Nothing
 *
 * Description: This function sends the unsolicited *MMGI events
 *-------------------------------------------------------------------------*/

void vgMMGIPrintEvent(const VgMMGIEventType vgMMGIEvent)
{
    VgProfileBit profileBit;
    VgmuxChannelNumber profileEntity = VGMUX_CHANNEL_INVALID;

    profileBit = (VgProfileBit)(1<<(int)vgMMGIEvent);
    for ( profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
    {
        if( isEntityActive (profileEntity) &&
        getProfileValueBit( profileEntity, PROF_MMGI, profileBit)==REPORTING_ENABLED)
        {
            vgPutNewLine (profileEntity);
            vgPrintf (profileEntity, (Char *)"*MMGI: %d", (int)vgMMGIEvent);
            vgPutNewLine (profileEntity);
            vgFlushBuffer (profileEntity);
        }
    }
}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

