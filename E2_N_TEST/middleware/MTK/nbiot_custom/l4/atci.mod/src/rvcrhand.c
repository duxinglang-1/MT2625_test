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
 * Implements handling of all data output to TE equipment.
 **************************************************************************/

#define MODULE_NAME "RVCRHAND"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvprof.h>
#include <rvcrhand.h>
#include <cici_sig.h>
#include <rvcrman.h>
#include <rvcrconv.h>
#include <rvcimxut.h>
#include <gkisig.h>
#include <rvgput.h>

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

union Signal
{
  CirmDataInd                 cirmDataInd;
};
  
/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static CirmDataInd *createCirmDataInd (const VgmuxChannelNumber   entity);
static void sendCirmDataInd           (const VgmuxChannelNumber   entity);
static void buildCirmDataString       (const VgmuxChannelNumber   entity,
                                       const Int8                 *data_p,
                                       const Int16                dataLength);
static Boolean doesItFit(const VgmuxChannelNumber entity, Int16 strLen);

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

#define VG_NUM_CONV_BUFFER      (10)

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
 * Function:        createCirmDataInd
 *
 * Parameters:      entity        - mux channel
 *
 * Returns:         CirmDataInd * - Pointer to the created buffer or PNULL.
 *
 * Description:     Creates a new CirmDataInd buffer for the specified
 *                  entity.
 *-------------------------------------------------------------------------*/
CirmDataInd *createCirmDataInd(const VgmuxChannelNumber entity)
{
  CirmDataInd         *request_p;
  CrManagerContext_t  *crManagerContext_p = ptrToCrManagerContext(entity);

  crManagerContext_p->crOutputStreamCtrl.signal = kiNullBuffer;
 
 /* Allocate buffer.... */
  KiCreateSignal (SIG_CIRM_DATA_IND,
                       sizeof (CirmDataInd),
                        &crManagerContext_p->crOutputStreamCtrl.signal);

  request_p = (CirmDataInd *)crManagerContext_p->crOutputStreamCtrl.signal.sig;

  FatalAssert(request_p != PNULL);

  /* Initialise signal.... */
  request_p->channelNumber = entity;

  request_p->prefix = FALSE;
  request_p->postfix = FALSE;

  request_p->data[0]           = NULL_CHAR;
  request_p->resultCode        = getResultCode(entity);
  request_p->length            = 0;
  request_p->pendingConnection = CONNECTION_TERMINATOR;
  request_p->commandId         = getCommandId(entity);
  request_p->classOfData       = DATA;
  request_p->forceTransmit     = FALSE;

  /* If an AT command is running then this is not a URC, otherwise
   * it is
   */
  if ((getEntityState(entity != ENTITY_IDLE)) ||
       (getResultCode(entity) == RESULT_CODE_PROCEEDING))
  {
    request_p->isUrc = FALSE;
  }
  else
  {
    request_p->isUrc = TRUE;
  }      

  request_p->isSmsUrc          = FALSE;

  return (request_p);
}

/*--------------------------------------------------------------------------
 *
 * Function:        sendCirmDataInd
 *
 * Parameters:      entity     - mux channel
 *
 * Returns:         Nothing
 *
 * Description:     Sends the current entities data buffer.
 *-------------------------------------------------------------------------*/
void sendCirmDataInd(const VgmuxChannelNumber entity)
{
  CrManagerContext_t           *crManagerContext_p      = ptrToCrManagerContext(entity);
  StkEntityGenericData_t       *stkGenericContext_p     = ptrToStkGenericContext ();
  CrManagerGenericContext_t    *crManGenericContext_p   = ptrToCrManagerGenericContext();
  SleepManContext_t            *sleepManContext_p       = ptrToSleepManContext();
  GprsContext_t                *gprsContext_p           = ptrToGprsContext(entity);
  CommandId_t                  commandId                = getCommandId(entity);
  VgPsdStatusInfo              *psdStatusInfo_p         = PNULL;
  VgPsdBearerInfo              *psdBearerInfo_p         = PNULL;
  CirmDataInd                  *cirmDataInd_p           = PNULL;

  /* If waking up OR
   * We are in data mode and therefore cannot send anything to the MUX currently
   */
  if ((gprsContext_p != PNULL) && (gprsContext_p->activePsdBearerContextWithDataConn != PNULL))
  {
    psdStatusInfo_p = gprsContext_p->activePsdBearerContextWithDataConn;
    psdBearerInfo_p = &(psdStatusInfo_p->psdBearerInfo);
  }

  cirmDataInd_p = getCirmDataBuffer(entity);
  
  if (((sleepManContext_p->atciInWakeupState)&&(0!=entity))
      || ((vgDoesEntityHaveSeparateDataChannel(entity) == FALSE) &&
      (getEntityState(entity) == ENTITY_RUNNING) &&
      (getResultCode (entity) == RESULT_CODE_PROCEEDING) &&
      (psdBearerInfo_p != PNULL) &&
      (psdBearerInfo_p->connType != ABPD_CONN_TYPE_NONE) &&
      ((commandId == VG_AT_GP_CGDATA) ||
       (commandId == VG_AT_CC_O) ||
       (commandId == VG_AT_CC_D) ||
       (commandId == VG_AT_CC_DL) ||
#if defined (FEA_PPP)      
       (commandId == VG_AT_GP_MLOOPPSD) ||
#endif /* FEA_PPP */      
#if defined (FEA_MT_PDN_ACT)
       (commandId == VG_AT_CC_A) ||
       (commandId == VG_AT_GP_CGANS) ||
#endif /* FEA_MT_PDN_ACT */
       (commandId == VG_AT_GP_D))
       && (!((cirmDataInd_p != PNULL) && (cirmDataInd_p->forceTransmit)))))
  {
    KiDestroySignal ( &crManagerContext_p->crOutputStreamCtrl.signal );
  }
  else
  {
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck(stkGenericContext_p != PNULL, entity, 0, 0);
#endif
    if (( entity == stkGenericContext_p->atCommandData.cmdEntity) &&
       ((stkGenericContext_p->runAtCmdState == STATE_RUNNING_COMMAND) ||
       (stkGenericContext_p->runAtCmdState == STATE_COMMAND_COMPLETED))) 
    {
      ++stkGenericContext_p->atCommandData.cirmDataIndCount;
    }
    ++crManGenericContext_p->cirmDataIndCountFlowControl;

#if defined(ENABLE_CIRM_DATA_IND)
    KiSendSignal (VG_CI_TASK_ID, &crManagerContext_p->crOutputStreamCtrl.signal);
#else
    vgProcessCirmData (&crManagerContext_p->crOutputStreamCtrl.signal, entity);
    KiDestroySignal ( &crManagerContext_p->crOutputStreamCtrl.signal );
#endif
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        buildCirmDataString
 *
 * Parameters:      entity      - mux channel
 *                  data_p      - data to add to the buffer
 *                  dataLength  - length of data
 *
 * Returns:         Nothing
 *
 * Description:     Adds data to the current buffer. If the buffer doesn't
 *                  exist it is created.  If the buffer is full, or fills
 *                  the buffer is sent and a new one is created.
 *-------------------------------------------------------------------------*/
static void buildCirmDataString       (const VgmuxChannelNumber   entity,
                                       const Int8                 *data_p,
                                       const Int16                dataLength)
{
  CirmDataInd  *request_p = getCirmDataBuffer(entity);
  Int16        remainingBufferSpace = 0;
  Int16        remainingData = dataLength;
  Int16        dataLengthToCopy = 0;
  const Int8   *dataPosition_p = data_p;
  Boolean      isUrc = FALSE;

  /* Check buffer status.  If PNULL then allocate a new buffer.... */
  if (request_p == PNULL)
  {
    request_p = createCirmDataInd(entity);
  }

  FatalAssert(PNULL != request_p);
  FatalAssert(PNULL != data_p);

  if ( (PNULL != request_p) && (PNULL != data_p) )
  {
    /* Build request.  If length exceeds allowed length or we fill the buffer
     * then send signal and create a new one....
     */
    if ((dataLength + request_p->length) <= VG_MAX_AT_DATA_IN_SIGNAL_LENGTH)
    {
      /* Copy user data to buffer.... */
      memcpy( &request_p->data[request_p->length],
              dataPosition_p,
              dataLength);
      request_p->length += dataLength;
    }
    else
    {
      /* Fill up buffer, send and then create and start filling new buffer.... */
      while (remainingData > 0)
      {
        remainingBufferSpace = (VG_MAX_AT_DATA_IN_SIGNAL_LENGTH - request_p->length);

        /* If anymore space left in this buffer then transmit.... */
        if (remainingBufferSpace > 0)
        {
          if (remainingBufferSpace >= remainingData)
          {
            dataLengthToCopy = remainingData;
          }

          else
          {
            dataLengthToCopy = remainingBufferSpace;
          }

          memcpy( &request_p->data[request_p->length],
                  dataPosition_p,
                  dataLengthToCopy);
          request_p->length += dataLengthToCopy;
          FatalAssert(request_p->length <= VG_MAX_AT_DATA_IN_SIGNAL_LENGTH);
          dataPosition_p += dataLengthToCopy;
          remainingData -= dataLengthToCopy;
        }

        if (request_p->length == VG_MAX_AT_DATA_IN_SIGNAL_LENGTH)
        {

          /* Save what the current message URC setting is - and then
           * copy to the next signal also
           */
          isUrc = vgGetCirmDataIndIsUrc(entity);

          /* Send current buffer & create next.... */
          sendCirmDataInd(entity);

          request_p = createCirmDataInd(entity);
          FatalAssert(PNULL != request_p);

          /* Copy URC setting to next signal */
          if (remainingData > 0)
          {
            vgSetCirmDataIndIsUrc(entity, isUrc);
          }            
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        doesItFit
 *
 * Parameters:      entity        - mux channel
 *                  strLen        - length of string to append to buffer.
 *
 * Returns:         Nothing
 *
 * Description:     Determines whether a string will fit into the buffer
 *                  without causing an overflow.
 *-------------------------------------------------------------------------*/
Boolean doesItFit(const VgmuxChannelNumber entity, Int16 strLen)
{
  CirmDataInd   *request_p = getCirmDataBuffer(entity);
  Boolean result = FALSE;

  if (request_p != PNULL)
  {
    if ((strLen + request_p->length) <= VG_MAX_AT_DATA_IN_SIGNAL_LENGTH)
    {
      result = TRUE;
    }
  }

  return result;
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:        vgPutNewLine
 *
 * Parameters:      entity        - mux channel
 *
 * Returns:         Nothing
 *
 * Description:     Puts the necessary characters to shift the data onto the
 *                  next line.
 *-------------------------------------------------------------------------*/
void vgPutNewLine(const VgmuxChannelNumber entity)
{
  CirmDataInd       *request_p = PNULL;
  Char              newLineStr[3] = {0};
  Boolean           omitLine   = FALSE;

  if (entity != VGMUX_CHANNEL_INVALID)
  {
    GeneralContext_t  *generalContext_p = ptrToGeneralContext(entity);
#if defined (ATCI_SLIM_DISABLE)
    FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
    if (generalContext_p->omitFirstNewLine == TRUE)
    {
      generalContext_p->omitFirstNewLine = FALSE;
      omitLine = TRUE;
    }
  }

  if ( omitLine == FALSE)
  {
    if (isEntityActive(entity) == TRUE)
    {
      request_p = getCirmDataBuffer(entity);

      /* Init new line string.... */
      newLineStr[0] = (Char)getProfileValue (entity, PROF_S3);
      newLineStr[1] = (Char)getProfileValue (entity, PROF_S4);
      newLineStr[2] = '\0';

      /* If request pointer is PNULL, create a new buffer.... */
      if (request_p == PNULL)
      {
        request_p = createCirmDataInd(entity);
        FatalAssert(request_p != PNULL);
      }

      /* If no signalling data, just set prefix CR flag, if we have data, we append
       * CR characters to the end of the string.  In this case we only flush the
       * buffer if the CR characters do NOT fit.  This gives us the best buffer
       * utilistation %....
       */
      if (request_p->length == 0)
      {
        request_p->prefix = TRUE;
      }
      else
      {
        if (doesItFit (entity, (Int8) vgStrLen (newLineStr)) == TRUE)
        {
          buildCirmDataString (entity, newLineStr, (Int16) vgStrLen (newLineStr));
        }
        else
        {
          request_p->postfix = TRUE;
          vgFlushBuffer(entity);
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPutc
 *
 * Parameters:      entity        - mux channel
 *                  putChar       - Char to put to the buffer.
 *
 * Returns:         Nothing
 *
 * Description:     Writes the given char to specified entities buffer.
 *-------------------------------------------------------------------------*/
void vgPutc(const VgmuxChannelNumber entity, const Char putChar)
{
  if (isEntityActive(entity) == TRUE)
  {
    buildCirmDataString(entity, &putChar, (Int16)sizeof(Char));
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPuts
 *
 * Parameters:      entity        - mux channel
 *                  putString_p   - string to write to buffer
 *
 * Returns:         Nothing
 *
 * Description:     Puts the given string to the buffer.  This has true
 *                  ASCII puts functionality - a new line is written after
 *                  the string.  Only pass in ascii characters since non-ascii
 *                  character sets usage is undefined!!
 *-------------------------------------------------------------------------*/
void vgPuts(const VgmuxChannelNumber entity, const Char *putString_p)
{
  if (isEntityActive(entity) == TRUE)
  {
    buildCirmDataString (entity, putString_p, (Int16) vgStrLen (putString_p));
    vgPutNewLine(entity);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPutsWithLength
 *
 * Parameters:      entity        - mux channel
 *                  putString_p   - string to write
 *                  length
 *
 * Returns:         Nothing
 *
 * Description:     Writes n characters to the buffer.  Will accept non-ascii
 *                  characters.  No LF will be written.
 *-------------------------------------------------------------------------*/
void vgPutsWithLength(const VgmuxChannelNumber  entity,
                      const Char                *putString_p,
                      const Int16               length)
{
  if (isEntityActive(entity) == TRUE)
  {
    buildCirmDataString(entity, putString_p, length);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPutCrSpecificData
 *
 * Parameters:      entity        - mux channel
 *                  dcs           - coding scheme for the data
 *                  crData_p      - data to write.
 *                  crDataLength  - length of data to write
 *
 * Returns:         Nothing
 *
 * Description:     Writes a set of character set specfic data to the buffer.
 *-------------------------------------------------------------------------*/
void vgPutCrSpecificData( const VgmuxChannelNumber  entity,
                          const VgDataCodingFormat  codingScheme,
                          const Int8                *crData_p,
                          const Int16               crDataLength)
{
  Int16 convLength = 0;

  if (isEntityActive(entity) == TRUE)
  {
    switch (codingScheme)
    {
      case VG_DATA_CODED_GSM:
      {
        convLength = vgMapGsmToTE(getCrOutputBuffer(entity),
                                  VG_CR_CH_SET_OUTPUT_BUFFER_LEN,
                                  crData_p,
                                  crDataLength,
                                  (VgCSCSMode)getProfileValue(entity, PROF_CSCS),
                                  entity);
        buildCirmDataString(entity,
                            getCrOutputBuffer(entity),
                            convLength);

        resetCrOutputBuffer(entity);
        break;
      }

      case VG_DATA_CODED_HEX:
      {
        convLength = vgMapHexToTE(getCrOutputBuffer(entity),
                                  VG_CR_CH_SET_OUTPUT_BUFFER_LEN,
                                  crData_p,
                                  crDataLength,
                                  (VgCSCSMode)getProfileValue(entity, PROF_CSCS));
        buildCirmDataString(entity,
                            getCrOutputBuffer(entity),
                            convLength);

        resetCrOutputBuffer(entity);
        break;
      }

      default:
      {
        FatalParam(entity, codingScheme, 0);
        break;
      }
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPutCrSpecificData
 *
 * Parameters:      entity        - mux channel
 *                  dcs           - coding scheme for the data
 *                  crData_p      - data to write.
 *                  crDataLength  - length of data to write
 *                  cscsMode      - output format
 *
 * Returns:         Nothing
 *
 * Description:     Writes a set of character set specfic data to the buffer
 *                  using a specified output format.
 *-------------------------------------------------------------------------*/
void vgPutCrSpecificDataCscs(   const VgmuxChannelNumber  entity,
                                const VgDataCodingFormat  codingScheme,
                                const Int8                *crData_p,
                                const Int16               crDataLength,
                                const VgCSCSMode          cscsMode)
{
  Int16 convLength = 0;

  if (isEntityActive(entity) == TRUE)
  {
    switch (codingScheme)
    {
      case VG_DATA_CODED_GSM:
      {
        convLength = vgMapGsmToTE(getCrOutputBuffer(entity),
                                  VG_CR_CH_SET_OUTPUT_BUFFER_LEN,
                                  crData_p,
                                  crDataLength,
                                  cscsMode,
                                  entity);
        buildCirmDataString(entity,
                            getCrOutputBuffer(entity),
                            convLength);

        resetCrOutputBuffer(entity);
        break;
      }

      case VG_DATA_CODED_HEX:
      {
        convLength = vgMapHexToTE(getCrOutputBuffer(entity),
                                  VG_CR_CH_SET_OUTPUT_BUFFER_LEN,
                                  crData_p,
                                  crDataLength,
                                  cscsMode);
        buildCirmDataString(entity,
                            getCrOutputBuffer(entity),
                            convLength);

        resetCrOutputBuffer(entity);
        break;
      }

      default:
      {
        FatalParam(entity, codingScheme, 0);
        break;
      }
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPutCrSpecificChar
 *
 * Parameters:      entity        - mux channel
 *                  dcs           - coding scheme for the data
 *                  crData        - char to write.
 *
 * Returns:         Nothing
 *
 * Description:     Writes a single character to the buffer, converting it to
 *                  the correct character set.
 *-------------------------------------------------------------------------*/
void vgPutCrSpecificChar( const VgmuxChannelNumber  entity,
                          const VgDataCodingFormat  codingScheme,
                          const Int8                crData)
{
  if (isEntityActive(entity) == TRUE)
  {
    vgPutCrSpecificData(entity, codingScheme, &crData, sizeof(Int8));
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPutAlphaId
 *
 * Parameters:      entity        - mux channel
 *                  dcs           - coding scheme for the data
 *                  alphaId_p     - data to write.
 *                  alphaIdLength - length of data to write
 *
 * Returns:         Nothing
 *
 * Description:     Writes a set of alpha data to the buffer.
 *-------------------------------------------------------------------------*/
void vgPutAlphaId( const VgmuxChannelNumber  entity,
                   const Int8                *alphaId_p,
                   const Int16               alphaIdLength)
{
  Int16 convLength = 0;

  if (isEntityActive(entity) == TRUE)
  {
    convLength = vgMapAlphaIdToTE(getCrOutputBuffer(entity),
                                  VG_CR_CH_SET_OUTPUT_BUFFER_LEN,
                                  alphaId_p,
                                  alphaIdLength,
                                  (VgCSCSMode)getProfileValue(entity, PROF_CSCS),
                                  entity);

    buildCirmDataString(entity,
                        getCrOutputBuffer(entity),
                        convLength);

    resetCrOutputBuffer(entity);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPutAlphaIdUcs2
 *
 * Parameters:      entity        - mux channel
 *                  alphaId_p     - data to write.
 *                  alphaIdLength - length of data to write
 *
 * Returns:         Nothing
 *
 * Description:     Writes a set of alpha data to the buffer using UCS2 charset.
 *-------------------------------------------------------------------------*/
void vgPutAlphaIdUcs2(  VgmuxChannelNumber  entity,
                        const Int8         *alphaId_p,
                        Int16               alphaIdLength)
{
    Int16 convLength = 0;

    if (isEntityActive(entity) == TRUE)
    {
        convLength = vgMapAlphaIdToTE(  getCrOutputBuffer(entity),
                                        VG_CR_CH_SET_OUTPUT_BUFFER_LEN,
                                        alphaId_p,
                                        alphaIdLength,
                                        VG_AT_CSCS_UCS2, /* Force UCS2*/
                                        entity);

        buildCirmDataString(    entity,
                                getCrOutputBuffer(entity),
                                convLength);

        resetCrOutputBuffer(entity);
    }
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgPrintMissingParams
 *
 * Parameters:      entity        - mux channel
 *                  numberParams  - number of parameters which are missing
 *
 * Returns:         Nothing
 *
 * Description:     Writes a comma for each parameter that is missing from displayed entry
 *-------------------------------------------------------------------------*/
void vgPrintMissingParams (const VgmuxChannelNumber  entity,
                           const Int16               numberParams)
{
  Int16 i;

  for (i = 0; i < numberParams; i++)
  {
    vgPutc (entity, ',');
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPut8BitNum
 *
 * Parameters:      entity        - mux channel
 *                  number        - number to write to the buffer
 *
 * Returns:         Nothing
 *
 * Description:     Writes a 8-bit number to the buffer.
 *-------------------------------------------------------------------------*/
void vgPut8BitNum(const VgmuxChannelNumber  entity,
                  const Int8                number)
{
  Char  conversionBuffer[VG_NUM_CONV_BUFFER] = {0};

  if (isEntityActive(entity) == TRUE)
  {
    vgPrintNum (&conversionBuffer[0], number);
    buildCirmDataString (entity, &conversionBuffer[0], (Int16) vgStrLen (conversionBuffer));
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPut16BitNum
 *
 * Parameters:      entity        - mux channel
 *                  number        - number to write to the buffer
 *
 * Returns:         Nothing
 *
 * Description:     Writes a 16-bit number to the buffer.
 *-------------------------------------------------------------------------*/
void vgPut16BitNum(const VgmuxChannelNumber  entity,
                   const Int16               number)
{
  Char  conversionBuffer[VG_NUM_CONV_BUFFER] = {0};

  if (isEntityActive(entity) == TRUE)
  {
    vgPrintNum (&conversionBuffer[0], number);
    buildCirmDataString (entity, &conversionBuffer[0], (Int16) vgStrLen (conversionBuffer));
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPut32BitNum
 *
 * Parameters:      entity        - mux channel
 *                  number        - number to write to the buffer
 *
 * Returns:         Nothing
 *
 * Description:     Writes a 32-bit number to the buffer.
 *-------------------------------------------------------------------------*/
void vgPut32BitNum(const VgmuxChannelNumber  entity,
                   const Int32               number)
{
  Char  conversionBuffer[VG_NUM_CONV_BUFFER] = {0};

  if (isEntityActive(entity) == TRUE)
  {
    vgPrint32BitNum (&conversionBuffer[0], number);
    buildCirmDataString (entity, &conversionBuffer[0], (Int16) vgStrLen (conversionBuffer));
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPut8BitHex
 *
 * Parameters:      entity        - mux channel
 *                  number        - number to write to the buffer
 *
 * Returns:         Nothing
 *
 * Description:     Writes a 8-bit number to the buffer in hex format
 *-------------------------------------------------------------------------*/
void vgPut8BitHex(const VgmuxChannelNumber  entity,
                  const Int8                number)
{
  Char  conversionBuffer[VG_NUM_CONV_BUFFER] = {0};

  if (isEntityActive(entity) == TRUE)
  {
    vgOp8BitHex (number, &conversionBuffer[0]);
    buildCirmDataString (entity, &conversionBuffer[0], (Int16) vgStrLen (conversionBuffer));
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPut16BitHex
 *
 * Parameters:      entity        - mux channel
 *                  number        - number to write to the buffer
 *
 * Returns:         Nothing
 *
 * Description:     Writes a 16-bit number to the buffer in hex format
 *-------------------------------------------------------------------------*/
void vgPut16BitHex(const VgmuxChannelNumber  entity,
                   const Int16               number)
{
  Char  conversionBuffer[VG_NUM_CONV_BUFFER] = {0};

  if (isEntityActive(entity) == TRUE)
  {
    vgOp32BitHex (number, 2, &conversionBuffer[0]);
    buildCirmDataString (entity, &conversionBuffer[0], (Int16) vgStrLen (conversionBuffer));
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPut32BitHex
 *
 * Parameters:      entity        - mux channel
 *                  number        - number to write to the buffer
 *
 * Returns:         Nothing
 *
 * Description:     Writes a 32-bit number to the buffer in hex format
 *-------------------------------------------------------------------------*/
void vgPut32BitHex(const VgmuxChannelNumber  entity,
                   const Int32               number)
{
  Char  conversionBuffer[VG_NUM_CONV_BUFFER] = {0};

  if (isEntityActive(entity) == TRUE)
  {
    vgOp32BitHex (number, 4, &conversionBuffer[0]);
    buildCirmDataString (entity, &conversionBuffer[0], (Int16) vgStrLen (conversionBuffer));
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPrintf
 *
 * Parameters:      entity        - mux channel
 *                  fmt_p         - format string
 *                  ...           - arguments
 *
 * Returns:         Nothing
 *
 * Description:     Takes a string and formats it printf style before putting
 *                  it in the buffer.
 *-------------------------------------------------------------------------*/
void vgPrintf(const VgmuxChannelNumber entity,
              const Char *fmt_p,
              ...)
{
   CrManagerGenericContext_t   *crManagerGenericContext     = ptrToCrManagerGenericContext();
   Int16                        numChars                    = 0;
   va_list                      marker;

  if (isEntityActive(entity) == TRUE)
  {
    /* Format data.... */
    va_start( marker, fmt_p);

    /* We need to check for the special %C substitution which is */
    /* only valid at the beginning of a line                     */

    if ((fmt_p[0] == '%') && (fmt_p[1] == 'C'))
    {
      vgStrNCpy(crManagerGenericContext->vgPrintfBuffer, getCommandName(entity), VG_PRINTF_CONV_BUFFER);
      numChars = (Int16) vgStrLen( crManagerGenericContext->vgPrintfBuffer);

#if defined (__arm) && !defined(__GNUC__)
      numChars += _vsprintf((char*) &crManagerGenericContext->vgPrintfBuffer[numChars],
                            (char*) &fmt_p[2],
                            marker);
#else
      numChars += vsnprintf((char*) &crManagerGenericContext->vgPrintfBuffer[numChars],
                            (VG_PRINTF_CONV_BUFFER + NULL_TERMINATOR_LENGTH) - numChars,
                           (char*) &fmt_p[2],
                           marker);
#endif
    }
    else
    {
#if defined (__arm) && !defined(__GNUC__)
      numChars = _vsprintf((char*) crManagerGenericContext->vgPrintfBuffer,
                           (char*) fmt_p,
                           marker);
#else
      numChars = (Int16)vsnprintf((char*) crManagerGenericContext->vgPrintfBuffer,
                          (VG_PRINTF_CONV_BUFFER + NULL_TERMINATOR_LENGTH),
                          (char*) fmt_p,
                          marker);
#endif
    }

    va_end( marker );

    FatalCheck( numChars <= VG_PRINTF_CONV_BUFFER, 
              numChars, 
              fmt_p[0],
              fmt_p[1]);

    /* Put string.... */
    buildCirmDataString(entity, crManagerGenericContext->vgPrintfBuffer, numChars);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgFlushBuffer
 *
 * Parameters:      entity        - mux channel
 *
 * Returns:         Nothing
 *
 * Description:     Flushes the buffer for a spcific entity.  All data
 *                  present will be dumped to the MUX.
 *-------------------------------------------------------------------------*/
void vgFlushBuffer(const VgmuxChannelNumber  entity)
{
  CirmDataInd  *request_p = PNULL;

  if (isEntityActive(entity) == TRUE)
  {
    request_p = getCirmDataBuffer(entity);

    if (request_p != PNULL)
    {
      /* We only send data if we have some to send.... */
      if ((request_p->postfix == TRUE) ||
          (request_p->prefix == TRUE)  ||
          (request_p->length > 0))
      {
        sendCirmDataInd(entity);
        request_p = createCirmDataInd(entity);
        FatalAssert(request_p != PNULL);
      }
    }
  }
}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

