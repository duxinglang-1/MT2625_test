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
 *  Utility functions for voyager
 **************************************************************************/

#define MODULE_NAME "RVUTIL"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <kernel.h>
#include <string.h>
#include <ctype.h>
#include <rvsystem.h>
#include <rvutil.h>
#include <rvprof.h>
#include <rvcimxut.h>
#include <cici_sig.h>
#include <vgmx_sig.h>

#if defined (ENABLE_RAVEN_RESULT_CODE_DEBUG)
#  include <stdio.h>
#endif
#  include <rvchman.h>
#include <rvcrhand.h>

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

#define MaxInt32       (0xFFFFFFFFL)

/***************************************************************************
 * Types
 ***************************************************************************/
union Signal
{
  VgCiSsRegistrationInd       vgCiSsRegistrationInd;
  CirmDataInd                 cirmDataInd;
  CiRunAtCommandInd           ciRunAtCommandInd;
  CiDataEntryModeInd          ciDataEntryModeInd;
  CiUserProfLoadedInd         ciUserProfLoadedInd;
};

/***************************************************************************
 * Variables
 ***************************************************************************/

static const Char vgHexTable[] =
{
  '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

static const Ipr iprStr[MAX_NUM_PORTSPEED] =
{
  { "0"   }, /* To line up with PortSpeed structure for PORTSPEED_AUTO. */
  { "110"   }, /* PORTSPEED_110   */
  { "300"   }, /* PORTSPEED_300   */  
  { "1200"   }, /* PORTSPEED_1200   */  
  { "2400"   }, /* PORTSPEED_2400   */  
  { "4800"   }, /* PORTSPEED_4800   */
  { "9600"   }, /* PORTSPEED_9600   */
  { "19200"  }, /* PORTSPEED_19200  */
  { "38400"  }, /* PORTSPEED_38400  */
  { "57600"  }, /* PORTSPEED_57600  */
  { "115200" }, /* PORTSPEED_115200 */
  { "230400" }, /* PORTSPEED_230400 */
  { "460800" }, /* PORTSPEED_460800 */
  { "921600" }  /* PORTSPEED_921600 */
};

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/
static void sendSignalToCrmWithLength (const Char *string,
                                        Int16 length,
                                         Boolean prefix,
                                          Boolean postfix,
                                           TxDataClass_t txClass,
                                            ResultCode_t resultCode,
                                             const VgmuxChannelNumber sendOnChannel);
static Int8 findAlphaLength (const Char *string);
/***************************************************************************
 * Macros
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/********************************************/
/* RAM Buffer management                    */
/********************************************/
/*************************************************************************
*
* Function:     vgAllocateRamToConvBuff
*
* Parameters:   None
*
* Returns:      Nothing
*
* Description:  Assigns static RAM to pointer
*
*************************************************************************/
extern Int8 getProfileValue (const VgmuxChannelNumber entity,
                       const Int8 profId);
Int8* vgAllocateRamToConvBuff (void)
{
  CrManagerGenericContext_t   *crManagerGenericContext     = ptrToCrManagerGenericContext();

  Int8                     inUseCount;
  Int8                     *dataPtr = PNULL;
  Boolean                  found = FALSE;

  /* Search for data item not in use */
  for (inUseCount=0; !found && (inUseCount < ATCI_MAX_NUM_CONV_BUFF); inUseCount++)
  {
    if (crManagerGenericContext->inUseConvBuffDataItem[inUseCount].inUse == FALSE)
    {
      crManagerGenericContext->inUseConvBuffDataItem[inUseCount].inUse = TRUE;
      dataPtr = &(crManagerGenericContext->inUseConvBuffDataItem[inUseCount].inUseConvBuff[0]);
      found = TRUE;
    }          
  }

  /* Must always find one otherwise we have run out of memory */
  FatalAssert(found);

  return (dataPtr);
}

/*************************************************************************
*
* Function:     vgFreeRamForConvBuff
*
* Parameters:   Pointer to be freed
*
* Returns:      nothing
*
* Description:  Frees static RAM for the pointer
*
*************************************************************************/
void vgFreeRamForConvBuff             (Int8 **dataPtr)
{
  CrManagerGenericContext_t   *crManagerGenericContext     = ptrToCrManagerGenericContext();

  Int8                     inUseCount;
  Boolean                  found = FALSE;

  if (*dataPtr != PNULL)
  {

    /* Search for data item in use matching pointer */
    for (inUseCount=0; !found && (inUseCount < ATCI_MAX_NUM_CONV_BUFF); inUseCount++)
    {
      if ((crManagerGenericContext->inUseConvBuffDataItem[inUseCount].inUse == TRUE) &&
          (&(crManagerGenericContext->inUseConvBuffDataItem[inUseCount].inUseConvBuff[0]) == *dataPtr))
      {
        crManagerGenericContext->inUseConvBuffDataItem[inUseCount].inUse = FALSE;
        found = TRUE;
      }          
    }

    if (found)
    {
      *dataPtr = PNULL;
    }

    /* Must find something */
    FatalAssert(found);
  }
}

/*************************************************************************
*
* Function:     vgAllocateRamToOutputBuff
*
* Parameters:   None
*
* Returns:      Nothing
*
* Description:  Assigns static RAM to pointer
*
*************************************************************************/

Int8* vgAllocateRamToOutputBuff (void)
{
  CrManagerGenericContext_t   *crManagerGenericContext     = ptrToCrManagerGenericContext();

  Int8                     inUseCount;
  Int8                     *dataPtr = PNULL;
  Boolean                  found = FALSE;

  /* Search for data item not in use */
  for (inUseCount=0; !found && (inUseCount < ATCI_MAX_NUM_OUTPUT_BUFF); inUseCount++)
  {
    if (crManagerGenericContext->inUseOutputBuffDataItem[inUseCount].inUse == FALSE)
    {
      crManagerGenericContext->inUseOutputBuffDataItem[inUseCount].inUse = TRUE;
      dataPtr = &(crManagerGenericContext->inUseOutputBuffDataItem[inUseCount].inUseOutputBuff[0]);
      found = TRUE;
    }          
  }

  /* Must always find one otherwise we have run out of memory */
  FatalAssert(found);

  return (dataPtr);
}

/*************************************************************************
*
* Function:     vgFreeRamForOutputBuff
*
* Parameters:   Pointer to be freed
*
* Returns:      nothing
*
* Description:  Frees static RAM for the pointer
*
*************************************************************************/
void vgFreeRamForOutputBuff             (Int8 **dataPtr)
{
  CrManagerGenericContext_t   *crManagerGenericContext     = ptrToCrManagerGenericContext();

  Int8                     inUseCount;
  Boolean                  found = FALSE;

  if (*dataPtr != PNULL)
  {

    /* Search for data item in use matching pointer */
    for (inUseCount=0; !found && (inUseCount < ATCI_MAX_NUM_OUTPUT_BUFF); inUseCount++)
    {
      if ((crManagerGenericContext->inUseOutputBuffDataItem[inUseCount].inUse == TRUE) &&
          (&(crManagerGenericContext->inUseOutputBuffDataItem[inUseCount].inUseOutputBuff[0]) == *dataPtr))
      {
        crManagerGenericContext->inUseOutputBuffDataItem[inUseCount].inUse = FALSE;
        found = TRUE;
      }          
    }

    if (found)
    {
      *dataPtr = PNULL;
    }

    /* Must find something */
    FatalAssert(found);
  }
}

#if defined (FEA_PHONEBOOK)
/*************************************************************************
*
* Function:     vgAllocateRamToAlphaBuff
*
* Parameters:   None
*
* Returns:      Nothing
*
* Description:  Assigns static RAM to pointer
*
*************************************************************************/

Int8* vgAllocateRamToAlphaBuff (void)
{
  CrManagerGenericContext_t   *crManagerGenericContext     = ptrToCrManagerGenericContext();

  Int8                     inUseCount;
  Int8                     *dataPtr = PNULL;
  Boolean                  found = FALSE;

  /* Search for data item not in use */
  for (inUseCount=0; !found && (inUseCount < ATCI_MAX_NUM_ALPHA_BUFF); inUseCount++)
  {
    if (crManagerGenericContext->inUseAlphaBuffDataItem[inUseCount].inUse == FALSE)
    {
      crManagerGenericContext->inUseAlphaBuffDataItem[inUseCount].inUse = TRUE;
      dataPtr = &(crManagerGenericContext->inUseAlphaBuffDataItem[inUseCount].inUseAlphaBuff[0]);
      found = TRUE;
    }          
  }

  /* Must always find one otherwise we have run out of memory */
  FatalAssert(found);

  return (dataPtr);
}

/*************************************************************************
*
* Function:     vgFreeRamForAlphaBuff
*
* Parameters:   Pointer to be freed
*
* Returns:      nothing
*
* Description:  Frees static RAM for the pointer
*
*************************************************************************/
void vgFreeRamForAlphaBuff             (Int8 **dataPtr)
{
  CrManagerGenericContext_t   *crManagerGenericContext     = ptrToCrManagerGenericContext();

  Int8                     inUseCount;
  Boolean                  found = FALSE;

  if (*dataPtr != PNULL)
  {

    /* Search for data item in use matching pointer */
    for (inUseCount=0; !found && (inUseCount < ATCI_MAX_NUM_ALPHA_BUFF); inUseCount++)
    {
      if ((crManagerGenericContext->inUseAlphaBuffDataItem[inUseCount].inUse == TRUE) &&
          (&(crManagerGenericContext->inUseAlphaBuffDataItem[inUseCount].inUseAlphaBuff[0]) == *dataPtr))
      {
        crManagerGenericContext->inUseAlphaBuffDataItem[inUseCount].inUse = FALSE;
        found = TRUE;
      }          
    }

    if (found)
    {
      *dataPtr = PNULL;
    }

    /* Must find something */
    FatalAssert(found);
  }
}

/*************************************************************************
*
* Function:     vgAllocateRamToAlphaSearchBuff
*
* Parameters:   None
*
* Returns:      Nothing
*
* Description:  Assigns static RAM to pointer
*
*************************************************************************/

Int8* vgAllocateRamToAlphaSearchBuff (void)
{
  CrManagerGenericContext_t   *crManagerGenericContext     = ptrToCrManagerGenericContext();

  Int8                     inUseCount;
  Int8                     *dataPtr = PNULL;
  Boolean                  found = FALSE;

  /* Search for data item not in use */
  for (inUseCount=0; !found && (inUseCount < ATCI_MAX_NUM_ALPHA_SEARCH_BUFF); inUseCount++)
  {
    if (crManagerGenericContext->inUseAlphaSearchBuffDataItem[inUseCount].inUse == FALSE)
    {
      crManagerGenericContext->inUseAlphaSearchBuffDataItem[inUseCount].inUse = TRUE;
      dataPtr = &(crManagerGenericContext->inUseAlphaSearchBuffDataItem[inUseCount].inUseAlphaSearchBuff[0]);
      found = TRUE;
    }          
  }

  /* Must always find one otherwise we have run out of memory */
  FatalAssert(found);

  return (dataPtr);
}

/*************************************************************************
*
* Function:     vgFreeRamForAlphaSearchBuff
*
* Parameters:   Pointer to be freed
*
* Returns:      nothing
*
* Description:  Frees static RAM for the pointer
*
*************************************************************************/
void vgFreeRamForAlphaSearchBuff             (Int8 **dataPtr)
{
  CrManagerGenericContext_t   *crManagerGenericContext     = ptrToCrManagerGenericContext();

  Int8                     inUseCount;
  Boolean                  found = FALSE;

  if (*dataPtr != PNULL)
  {

    /* Search for data item in use matching pointer */
    for (inUseCount=0; !found && (inUseCount < ATCI_MAX_NUM_ALPHA_SEARCH_BUFF); inUseCount++)
    {
      if ((crManagerGenericContext->inUseAlphaSearchBuffDataItem[inUseCount].inUse == TRUE) &&
          (&(crManagerGenericContext->inUseAlphaSearchBuffDataItem[inUseCount].inUseAlphaSearchBuff[0]) == *dataPtr))
      {
        crManagerGenericContext->inUseAlphaSearchBuffDataItem[inUseCount].inUse = FALSE;
        found = TRUE;
      }          
    }

    if (found)
    {
      *dataPtr = PNULL;
    }

    /* Must find something */
    FatalAssert(found);
  }
}
#endif /* FEA_PHONEBOOK */

/****************************************************************************
 *
 * Function:    getIprString
 *
 * Parameters:  Int8
 *
 * Returns:     Char*
 *
 * Description:
 *
 ****************************************************************************/

const Ipr *getIprString (Int8  baudIndex)
{
  FatalCheck (baudIndex < MAX_NUM_PORTSPEED, baudIndex, MAX_NUM_PORTSPEED, 0);

  return (&iprStr [baudIndex]);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgGetBcd
 *
 * Parameters:      Bcd - pointer to Bcd string
 *                  pos - Int8 position in string
 *
 * Returns:         Int8  - Bcd digit
 *
 * Description:     gets Bcd digit from Bcd string
 *
 *-------------------------------------------------------------------------*/

Int8 vgGetBcd (const Bcd *base_p, Int16 pos)
{
  Int8 val = 0;

  if ((pos % 2) != 0)
  {
    val = base_p[pos / 2] & 0xF;
  }
  else
  {
    val = (base_p[pos / 2] >> 4) & 0xF;
  }

  return (val);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPutBcd
 *
 * Parameters:      base - pointer to Bcd string
 *                  pos - Int8 position in string
 *                  val - value to be put in string
 *
 * Returns:         nothing
 *
 * Description:     puts Bcd digit into Bcd string
 *
 *-------------------------------------------------------------------------*/

void vgPutBcd (Bcd *base, Int16 pos, Int8 val)
{
  if (pos < MAX_CALLED_BCD_NO_LENGTH)
  {
    if ((pos % 2) != 0)
    {
      base[pos / 2] = (base[pos / 2] & 0xF0) | (val & 0x0F);
    }
    else
    {
      base[pos / 2] = (base[pos / 2] & 0x0F) | ((val << 4) & 0xF0);
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgCharToBcd
 *
 * Parameters:      base - pointer to Bcd string
 *                  pos - Int8 position in string
 *                  val - value to be put in string
 *
 * Returns:         nothing
 *
 * Description:     puts Char digit into Bcd string
 *
 *-------------------------------------------------------------------------*/
void vgCharToBcd (Bcd *base, Int16 pos, Int8 val)
{
  if (pos < MAX_CALLED_BCD_NO_LENGTH)
  {
    if ((pos % 2) == 0)
    {
      base[pos / 2] = (base[pos / 2] & 0xF0) | (val & 0x0F);
    }
    else
    {
      base[pos / 2] = (base[pos / 2] & 0x0F) | ((val << 4) & 0xF0);
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgTextToBcd
 *
 * Parameters:      asciiNum_p - pointer to Char string
 *                  asciiLen - length of Char string
 *                  bcdNum_p - pointer to Bcd string
 *
 * Returns:         nothing
 *
 * Description:     puts Char digit into Bcd string
 *
 *-------------------------------------------------------------------------*/
void vgTextToBcd( const Char *asciiNum_p, Int16 asciiLen, Int8 *bcdNum_p)
{
  Int8 pos, val;

  for (pos = 0; pos < asciiLen; pos++)
  {
    switch (asciiNum_p[pos])
    {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      {
        val = asciiNum_p[pos] - '0';
        vgCharToBcd (bcdNum_p, pos, val);
        break;
      }

     default:
       vgCharToBcd( bcdNum_p, pos, 0);
       break;
    }
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:        sendSignalToToCrmWithLength
 *
 * Parameters:      string        - Pointer to string of text to be sent
 *                  length        - Int16 length of string
 *                  prefix        - Boolean determining if a <CR><LF> precedes text
 *                  postfix       - Boolean determining if a <CR><LF> postscripts text
 *                  txClass       - Class of data to transmit
 *                  resultCode    - Result code to transmit
 *                  sendOnChannel - Mux channel on which to transmit data
 *
 * Returns:         Nothing
 *
 * Description:     Sends text given in string to the Command Response
 *                  Manager. If the size of the string is too large then
 *                  multiple signals are sent.
 *
 *-------------------------------------------------------------------------*/

static void sendSignalToCrmWithLength (const Char *string,
                                        Int16 length,
                                         Boolean prefix,
                                          Boolean postfix,
                                           TxDataClass_t txClass,
                                            ResultCode_t resultCode,
                                             const VgmuxChannelNumber sendOnChannel)
{
  CirmDataInd  *request_p;
  const Char   *stringPos = string;
  Int16        strLength;
  SignalBuffer signal;
  Int16        signalPos;
  StkEntityGenericData_t       *stkGenericContext_p     = ptrToStkGenericContext ();
  CrManagerGenericContext_t    *crManGenericContext_p   = ptrToCrManagerGenericContext();
  SleepManContext_t            *sleepManContext_p       = ptrToSleepManContext();

/* calculate the number of signals required to send all of the data */
  Int8 numSignalsRequired = (Int8)(length / VG_MAX_AT_DATA_IN_SIGNAL_LENGTH) +
                            ((length % VG_MAX_AT_DATA_IN_SIGNAL_LENGTH) ? 1 : 0);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(stkGenericContext_p != PNULL, sendOnChannel, 0, 0);
#endif
  /* if we are sending a result code then the length may be zero.
   * we must still send the signal or the result code will not be sent. */
  if ((length == 0) && (txClass == RESULT_CODE))
  {
    numSignalsRequired = 1;
  }

  for (signalPos = 0;
        signalPos < numSignalsRequired;
         signalPos++, stringPos += VG_MAX_AT_DATA_IN_SIGNAL_LENGTH)
  {
    signal = kiNullBuffer;

    /* we do not need to send a registration signal as we include the entity number in the signal */
    KiCreateSignal (SIG_CIRM_DATA_IND,
                         sizeof (CirmDataInd),
                          &signal);

    request_p = &signal.sig->cirmDataInd;
    request_p->channelNumber = sendOnChannel;
    request_p->forceTransmit     = FALSE;
    request_p->isUrc             = FALSE;
    request_p->isSmsUrc          = FALSE;

    if (( sendOnChannel == stkGenericContext_p->atCommandData.cmdEntity) &&
     ((stkGenericContext_p->runAtCmdState == STATE_RUNNING_COMMAND) ||
      (stkGenericContext_p->runAtCmdState == STATE_COMMAND_COMPLETED)))
    {
       ++stkGenericContext_p->atCommandData.cirmDataIndCount;
    }
    ++crManGenericContext_p->cirmDataIndCountFlowControl;

    /* first signal, prefix included */
    if (signalPos == 0)
    {
      request_p->prefix = prefix;
    }
    else
    {
      request_p->prefix = FALSE;
    }

    /* last signal postfix included */
    if (signalPos == (numSignalsRequired - 1))
    {
      /* last signal may not be full */
      strLength = (Int16)(length % VG_MAX_AT_DATA_IN_SIGNAL_LENGTH);

      /* mantis155230: if data length just be VG_MAX_AT_DATA_IN_SIGNAL_LENGTH */
      if((strLength == 0) && (length != 0))
      {
         strLength = VG_MAX_AT_DATA_IN_SIGNAL_LENGTH;
      }
      request_p->postfix = postfix;
    }
    else
    {
      /* signal will be full */
      strLength = VG_MAX_AT_DATA_IN_SIGNAL_LENGTH;
      request_p->postfix = FALSE;
    }

    memcpy (request_p->data, stringPos, strLength);

    request_p->resultCode        = resultCode;
    request_p->length            = strLength;
    request_p->classOfData       = txClass;
    request_p->pendingConnection = getConnectionType (sendOnChannel);
    request_p->taskInitiated     = getTaskInitiated  (sendOnChannel);
    request_p->commandId         = getCommandId      (sendOnChannel);

    if (sleepManContext_p->atciInWakeupState && 0!=sendOnChannel)
    {
      KiDestroySignal ( &signal );
    }    
    else
    {
#if defined(ENABLE_CIRM_DATA_IND)
      KiSendSignal (VG_CI_TASK_ID, &signal);
#else
      vgProcessCirmData(&signal, sendOnChannel);
      KiDestroySignal ( &signal );
#endif
    }
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:        findAlphaLength
 *
 * Parameters:      string  - pointer to string
 *
 * Returns:         Int8 - alpha length
 *
 * Description:     gets the length of the first contiguous block of alpha
 *                  characters in the string
 *
 *-------------------------------------------------------------------------*/

static Int8 findAlphaLength (const Char *string)
{
  Boolean startAlpha = FALSE;
  Boolean endAlpha   = FALSE;

  Int16    index  = 0;
  Int16    length = 0;
  Int16    stringLength = (Int16)vgStrLen (string);
  for (index = 0;
        ((endAlpha == FALSE) && ((Int16)index < stringLength));
         index++)
  {
    if (startAlpha == TRUE)
    {
      if (isalpha (string[index]) != FALSE)
      {
        length++;
      }
      else
      {
        endAlpha = TRUE;
      }
    }
    else
    {
      if (isalpha (string[index]) != FALSE)
      {
        startAlpha = TRUE;
        length = 1;
      }
    }
  }

  return (length);
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/
Boolean AtIsValid(const VgmuxChannelNumber entity,CommandLine_t *commandBuffer_p)
{
    Boolean result =TRUE;
    if (commandBuffer_p->position < commandBuffer_p -> length - 1)
    {
      switch (commandBuffer_p->character[commandBuffer_p->position])
      {
        case QUERY_CHAR:  /* AT+...? */
        {
            if((';'!=commandBuffer_p->character[commandBuffer_p->position+1])
                &&('&'!=commandBuffer_p->character[commandBuffer_p->position+1])
                &&(getProfileValue (entity, PROF_S3)!=commandBuffer_p->character[commandBuffer_p->position+1]))
            {
                return FALSE;
            }
            break;
        }
        case EQUALS_CHAR:  /* AT+...= */
        {

          if ((commandBuffer_p->position+1 < commandBuffer_p->length - 1 ) &&
              (commandBuffer_p->character[commandBuffer_p->position+1] ==
                 QUERY_CHAR))
          {
                if((';'!=commandBuffer_p->character[commandBuffer_p->position+2])
                    &&('&'!=commandBuffer_p->character[commandBuffer_p->position+2])
                    &&(getProfileValue (entity, PROF_S3)!=commandBuffer_p->character[commandBuffer_p->position+2]))
                {
                    return FALSE;
                }                        
          }
           break;
        }
        default:  /* AT+... */
        {
           break;
        }
      }
    }
    return result;
}

 /****************************************************************************
 *
 * Function:    parseCommandBuffer
 *
 * Parameters:  signal_p - signal to be processed
 *              entity - mux channel number
 *
 * Returns:     none
 *
 * Description: determines action for received signals
 *
 * Warning:     the command table should be in reverse alphabetic order.
 *
 *              For example, if "+CR" comes before "+CRC" in the table then
 *              the parser will run the +CR handler even if you type in "+CRC".
 *
 ****************************************************************************/

Boolean parseCommandBuffer (AtCmdControl const *atCommandTable_p,
                             const VgmuxChannelNumber entity)
{
  Boolean            commandFound = FALSE;
  AtCmdControl       const *currCmd_p;
  Int8               commandLength = 0;
  Boolean            compare;
  Int16              commandIndex;
  CommandLine_t      *commandBuffer_p;
  ScanParseContext_t      *scanParseContext_p = ptrToScanParseContext (entity);
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
  MobilityContext_t       *mobilityContext_p = ptrToMobilityContext ();
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  AtCmdAccessMask    accessMask = AT_CMD_ACCESS_NONE;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(scanParseContext_p != PNULL, entity, 0, 0);
#endif 
  commandBuffer_p = &scanParseContext_p->nextCommand;

  if (commandBuffer_p->length > 0)
  {
    commandFound = FALSE;

    /* Build up the access mask */

    accessMask |= (AtCmdAccessMask) ((simLockGenericContext_p->simInsertedState == VG_SIM_INSERTED)  ? AT_CMD_ACCESS_SIM_PRESENT : AT_CMD_ACCESS_NONE);
    accessMask |= (AtCmdAccessMask) ((simLockGenericContext_p->simLocked)    ? AT_CMD_ACCESS_NONE : AT_CMD_ACCESS_SIM_READY);
    accessMask |= (AtCmdAccessMask) ((simLockGenericContext_p->powerUpProtoStack && simLockGenericContext_p->powerUpSim)
                                                          ? AT_CMD_ACCESS_CFUN_1 : AT_CMD_ACCESS_NONE);
    accessMask |= (AtCmdAccessMask) ((mobilityContext_p->vgCREGData.state == 1)
                                                          ? AT_CMD_ACCESS_GSM_REGISTERED : AT_CMD_ACCESS_NONE);
    accessMask |= (AtCmdAccessMask) ((gprsGenericContext_p->vgCGREGData.state == 1)
                                                       ? AT_CMD_ACCESS_GPRS_REGISTERED : AT_CMD_ACCESS_NONE);
    accessMask |= (AtCmdAccessMask) ((simLockGenericContext_p->powerUpProtoStack)
                                                          ? AT_CMD_ACCESS_CFUN_7 : AT_CMD_ACCESS_NONE);

    for (currCmd_p = atCommandTable_p;
         (commandFound == FALSE) && (currCmd_p->string != PNULL);
           currCmd_p++)
    {
      /* check if command is allowed by the access mask and by user choice */

      if (isCommandExecutable(currCmd_p->commandId) &&
          (((currCmd_p->accessMask & accessMask) & ~restrictAccessMask) ==
           (currCmd_p->accessMask & ~restrictAccessMask)))
      {

        if ((findAlphaLength (currCmd_p->string) == findAlphaLength (&commandBuffer_p->character[0])) ||
            (vgStrLen (currCmd_p->string) == 1) ||
            (commandBuffer_p->character[0] == AMPERSAND_CHAR))
        {
          commandLength = (Int8)vgStrLen (currCmd_p->string);
          compare = TRUE;
          commandIndex = 0;
          while ((commandIndex < commandLength) && (compare == TRUE))
          {
            if (toupper (currCmd_p->string[commandIndex]) !=
                 toupper (commandBuffer_p->character[commandIndex]))
            {
              compare = FALSE;
              break;
            }
            else
            {
              commandIndex++;
            }
          }
          if (compare == TRUE)
          {
            commandFound = TRUE;
            break;
          }
        }
      }
    }

    if (commandFound == TRUE)
    {

      commandBuffer_p->position += commandLength;

      /* call procedure associated with AT command */

      setCommandId   (entity, currCmd_p->commandId);
      setCommandName (entity, currCmd_p->string);
      if(AtIsValid(entity,commandBuffer_p))
      {
          setResultCode  (entity, (currCmd_p->procFunc)(commandBuffer_p, entity));
      }
      else
      {
        setResultCode (entity, RESULT_CODE_ERROR);
        return FALSE;
      }
      /* this handles the case where commands are separated by ;'s the ci mux code
       * will only gives us data upto a CR
       */
      if (commandBuffer_p->position + 1 >= commandBuffer_p->length)
      {
        /* no more data in current at command line */

        /* clear up memory */
        memset (&commandBuffer_p->character[0],
                 NULL_CHAR,
                  commandBuffer_p->length);

        commandBuffer_p->length = 0;
      }
      else
      {
        /* shuffle buffer contents along */
        memcpy (&commandBuffer_p->character[0],
                 &commandBuffer_p->character[commandBuffer_p->position],
                   commandBuffer_p->length - commandBuffer_p->position);

        /* calculate new length */
        commandBuffer_p->length = commandBuffer_p->length -
                                   commandBuffer_p->position;

        /* clear up memory */
        memset (&commandBuffer_p->character[commandBuffer_p->length],
                 NULL_CHAR,
                  commandBuffer_p->position);
      }
      commandBuffer_p->position = 0;
    }
    else
    {
      setResultCode (entity, RESULT_CODE_ERROR);
    }
  }

  return (commandFound);
}


/*--------------------------------------------------------------------------
 *
 * Function:    getExtendedOperation
 *
 * Parameters:  (InOut) commandBuffer_p - pointer to a CommandLine_t
 *              structure with the position field set to the character
 *              following the name of the extended AT command.
 *
 * Returns:     ExtendedOperation_t indicating the type of the extended
 *              AT command.
 *
 * Description: Examines the character(s) at position to determine the
 *              extended AT command operation required, leaving the position
 *              at the place following the characters used in the
 *              determination.
 *
 *-------------------------------------------------------------------------*/

ExtendedOperation_t getExtendedOperation (CommandLine_t *commandBuffer_p)
{
  ExtendedOperation_t result;

  if (commandBuffer_p->position < commandBuffer_p -> length - 1)
  {
    switch (commandBuffer_p->character[commandBuffer_p->position])
    {
      case QUERY_CHAR:  /* AT+...? */
      {
        commandBuffer_p->position++;
        result = EXTENDED_QUERY;
        break;
      }
      case EQUALS_CHAR:  /* AT+...= */
      {
        commandBuffer_p->position++;
        if ((commandBuffer_p->position < commandBuffer_p->length - 1 ) &&
            (commandBuffer_p->character[commandBuffer_p->position] ==
               QUERY_CHAR))
        {
          commandBuffer_p->position++;
          result = EXTENDED_RANGE;
        }
        else
        {
          result = EXTENDED_ASSIGN;
        }
        break;
      }
      default:  /* AT+... */
      {
        result = EXTENDED_ACTION;
        break;
      }
    }
  }
  else
  {
    result = EXTENDED_ACTION;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    getExtendedString
 *
 * Parameters:  (InOut) commandBuffer_p - a pointer to a CommandLine_t
 *              structure with the position field indicating the start of
 *              where a string value may be.
 *              (Out) outString - pointer to string obtained
 *              (Out) outStringLength - length of string returned.
 *
 * Returns:     Boolean indicating the success or failure in obtaining an
 *              AT command's string parameter.
 *
 * Description: Attempts to obtain the AT command string parameter at
 *              position, moving position to the point after the parameter
 *              and returning TRUE if the string parameter returned
 *              in outString is OK, else FALSE if it is not.
 *              The size of outString must be one larger than maxStringLength
 *              since an additional null terminator will be appended!!!
 *
 *-------------------------------------------------------------------------*/

Boolean getExtendedString (CommandLine_t *commandBuffer_p,
                            Char *outString,
                             Int16 maxStringLength,
                             Int16 *outStringLength)
{
  Boolean result = TRUE;
  Int16   length = 0;
  Int8    first, second;

  *outString = (Char)0;
  *outStringLength = 0;

  if ( commandBuffer_p->position < commandBuffer_p->length - 1 )
  {
    if (commandBuffer_p->character[commandBuffer_p->position] == COMMA_CHAR)
    {
      commandBuffer_p->position++;
    }
    else
    {
      if (commandBuffer_p->character[commandBuffer_p->position] != SEMICOLON_CHAR)
      {
        if (commandBuffer_p->character[commandBuffer_p->position] != QUOTES_CHAR)
        {
          result = FALSE;
        }
        else
        {
          commandBuffer_p->position++;
          while ((commandBuffer_p->position < commandBuffer_p->length - 1) &&
                 (commandBuffer_p->character[commandBuffer_p->position] != QUOTES_CHAR) &&
                 (length < maxStringLength) &&
                 (result == TRUE))
          {
            if (commandBuffer_p->character[commandBuffer_p->position] == '\\')
            {
              commandBuffer_p->position++;
              /* here must come two hexa digits */
              if (commandBuffer_p->position + 2 < commandBuffer_p->length - 1)
              {
                if ((getHexaValue (commandBuffer_p->character[commandBuffer_p->position++], &first)) &&
                    (getHexaValue (commandBuffer_p->character[commandBuffer_p->position++], &second)))
                {
                  *outString++ = (Char)(16 * first + second);
                  length++;
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
            else
            {
              *outString++ = (Char)(commandBuffer_p->character[commandBuffer_p->position++]);
              length++;
            }
          }

          *outStringLength = length;
          *outString = (Char)0;

          if (((commandBuffer_p->character[commandBuffer_p->position] == QUOTES_CHAR) &&
               (length <= maxStringLength)) == FALSE)
          {
            result = FALSE;
          }
          else
          {
            commandBuffer_p->position++;
            if (commandBuffer_p->character[commandBuffer_p->position] == COMMA_CHAR)
            {
              commandBuffer_p->position++;
            }
          }
        }
      }
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    getExtendedStringWithoutQuotes
 *
 * Parameters:  (InOut) commandBuffer_p - a pointer to a CommandLine_t
 *              structure with the position field indicating the start of
 *              where a string value may be.
 *              (Out) outString - pointer to string obtained
 *              (Out) outStringLength - length of string returned.
 *
 * Returns:     Boolean indicating the success or failure in obtaining an
 *              AT command's string parameter.
 *
 * Description: Attempts to obtain the AT command string parameter at
 *              position, moving position to the point after the parameter
 *              and returning TRUE if the string parameter returned
 *              in outString is OK, else FALSE if it is not.
 *
 *-------------------------------------------------------------------------*/

Boolean getExtendedStringWithoutQuotes (CommandLine_t *commandBuffer_p,
                            Char *outString,
                             Int16 maxStringLength,
                             Int16 *outStringLength)
{
  Boolean result = TRUE;
  Int16   length = 0;
  Int8    first, second;

  *outString = (Char)0;
  *outStringLength = 0;

  if (commandBuffer_p->position < commandBuffer_p->length - 1 &&
      commandBuffer_p->character[commandBuffer_p->position] != SEMICOLON_CHAR)
  {
    while ((commandBuffer_p->position < commandBuffer_p->length - 1) &&
           (commandBuffer_p->character[commandBuffer_p->position] != COMMA_CHAR) &&
           (length <= maxStringLength) &&
           (result == TRUE))
    {
      if (commandBuffer_p->character[commandBuffer_p->position] == '\\')
      {
        commandBuffer_p->position++;
        /* here must come two hexa digits */
        if (commandBuffer_p->position + 2 < commandBuffer_p->length - 1 &&
            getHexaValue (commandBuffer_p->character[commandBuffer_p->position++], &first) &&
            getHexaValue (commandBuffer_p->character[commandBuffer_p->position++], &second))
        {
          *outString++ = (Char)(16 * first + second);
          length++;
        }
        else
        {
          result = FALSE;
        }
      }
      else
      {
        *outString++ = (Char)(commandBuffer_p->character[commandBuffer_p->position++]);
        length++;
      }
    }
    *outStringLength = length;
    *outString = (Char)0;
    commandBuffer_p->position++;
  }
  else
  {
    result = FALSE;
  }

  return (result);
}


/*--------------------------------------------------------------------------
 *
 * Function:    getExtendedStringWithoutComma
 *
 * Parameters:  (InOut) commandBuffer_p - a pointer to a CommandLine_t
 *              structure with the position field indicating the start of
 *              where a string value may be.
 *              (Out) outString - pointer to string obtained
 *              (Out) outStringLength - length of string returned.
 *
 * Returns:     Boolean indicating the success or failure in obtaining an
 *              AT command's string parameter.
 *
 * Description: Attempts to obtain the AT command string parameter at
 *              position, moving position to the point after the parameter
 *              and returning TRUE if the string parameter returned
 *              in outString is OK, else FALSE if it is not.
 *
 *-------------------------------------------------------------------------*/

Boolean getExtendedStringWithoutComma (CommandLine_t *commandBuffer_p,
                                        Char *outString,
                                         Int16 maxStringLength,
                                         Int16 *outStringLength)
{
  Boolean result = TRUE;
  Int16   length = 0;
  Int8 first,second;

  *outString = (Char)0;
  *outStringLength = 0;

  if (commandBuffer_p->position < commandBuffer_p->length - 1)
  {
    if (commandBuffer_p->character[commandBuffer_p->position] != SEMICOLON_CHAR)
    {
      if (commandBuffer_p->character[commandBuffer_p->position] != QUOTES_CHAR)
      {
        result = FALSE;
      }
      else
      {
        commandBuffer_p->position++;

        while ((commandBuffer_p->position < commandBuffer_p->length - 1) &&
               (commandBuffer_p->character[commandBuffer_p->position] != QUOTES_CHAR) &&
               (length <= maxStringLength) &&
               (result == TRUE))
        {
          if (commandBuffer_p->character[commandBuffer_p->position] == '\\')
          {
            commandBuffer_p->position++;
            /* here must come two hexa digits */
            if (commandBuffer_p->position + 2 < commandBuffer_p->length - 1)
            {
              if ((getHexaValue(commandBuffer_p->character[commandBuffer_p->position++], &first)) &&
                  (getHexaValue(commandBuffer_p->character[commandBuffer_p->position++], &second)))
              {
                *outString++ = (Char)(16 * first + second);
                length++;
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
          else
          {
            *outString++ = (Char)(commandBuffer_p->character[commandBuffer_p->position++]);
            length++;
          }
        }

        *outStringLength = length;
        *outString = (Char)0;

        if (((commandBuffer_p->character[commandBuffer_p->position] == QUOTES_CHAR) &&
             (length <= maxStringLength)) == FALSE)
        {
          result = FALSE;
        }
        else
        {
          commandBuffer_p->position++;
        }
      }
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

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    getExtendedParameter
 *
 * Parameters:  (InOut) commandBuffer_p - a pointer to a CommandLine_t
 *              structure
 *
 * Returns:     Boolean indicating if there is at least one remaining parameter
 *
 * Description: Check if there are remaining parameter for the current command
 *
 *-------------------------------------------------------------------------*/
Boolean isRemainingParameter( const CommandLine_t *commandBuffer_p)
{
    Boolean   result = TRUE;

    if( commandBuffer_p->position < commandBuffer_p->length -1)
    {
        if (commandBuffer_p->character[commandBuffer_p->position] != SEMICOLON_CHAR)
        {
            /* Parameter present or empty.*/
            result = TRUE;
        }
        else
        {
            /* New command*/
            result = FALSE;
        }
    }
    else
    {
        /* End of the line*/
        result = FALSE;
    }

    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    getExtendedParameter
 *
 * Parameters:  (InOut) commandBuffer_p - a pointer to a CommandLine_t
 *              structure with the position field indicating the start of
 *              where a decimal value may be.
 *              (Out) value_p - a pointer to an Int32 value where the
 *              equivalent to the decimal value if given, or the value
 *              of the thrid parameter if not, is to be stored.
 *              (In) inValue - an Int32 value to be used if the parameter
 *              is not supplied.
 *
 * Returns:     Boolean indicating the success or failure in obtaining an
 *              extended AT command's parameter value.
 *
 * Description: Attempts to obtain the extended AT command parameter at
 *              position, moving position to the point after the parameter
 *              and returning TRUE if the extended parameter value returned
 *              in *value_p is OK, else FALSE if it is not.
 *
 *-------------------------------------------------------------------------*/

Boolean getExtendedParameter (CommandLine_t *commandBuffer_p,
                               Int32 *value_p,
                                Int32 inValue)
{
  Boolean result = TRUE;
  Boolean useInValue = TRUE;
  Int32   value;

  if (commandBuffer_p->position < commandBuffer_p->length -1)
  {
    if (commandBuffer_p->character[commandBuffer_p->position] == COMMA_CHAR)
    {
      commandBuffer_p->position++;
    }
    else
    {
      if (commandBuffer_p->character[commandBuffer_p->position] != SEMICOLON_CHAR)
      {
        if (isdigit (commandBuffer_p->character[commandBuffer_p->position]))
        {
          value = getDecimalValue (commandBuffer_p);
          *value_p = value;
          useInValue = FALSE;
          if (commandBuffer_p->position < commandBuffer_p->length - 1)
          {
            if (commandBuffer_p->character[commandBuffer_p->position] == COMMA_CHAR)
            {
              commandBuffer_p->position++;
            }
          }
        }
        else
        {
          result = FALSE;
        }
      }
    }
  }

  if (useInValue == TRUE)
  {
    *value_p = inValue;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    getExtendedParameterPresent
 *
 * Parameters:  (InOut) commandBuffer_p - a pointer to a CommandLine_t
 *              structure with the position field indicating the start of
 *              where a decimal value may be.
 *              (Out) value_p - a pointer to an Int32 value where the
 *              equivalent to the decimal value if given, or the value
 *              of the thrid parameter if not, is to be stored.
 *              (In) inValue - an Int32 value to be used if the parameter
 *              is not supplied.
 *              (Out) supplied_p - a pointer to a Boolean value to say if the
 *              parameter is supplied by user.
 *
 * Returns:     Boolean indicating the success or failure in obtaining an
 *              extended AT command's parameter value.
 *
 * Description: Attempts to obtain the extended AT command parameter at
 *              position, moving position to the point after the parameter
 *              and returning TRUE if the extended parameter value returned
 *              in *value_p is OK, else FALSE if it is not.
 *
 *-------------------------------------------------------------------------*/

Boolean getExtendedParameterPresent (CommandLine_t *commandBuffer_p,
                                      Int32 *value_p,
                                       const Int32 inValue,
                                        Boolean *supplied_p)
{
  Boolean result = TRUE;
  Boolean useInValue = TRUE;
  Int32   value;

  *supplied_p = FALSE;

  if (commandBuffer_p->position < commandBuffer_p->length -1)
  {
    if (commandBuffer_p->character[commandBuffer_p->position] == COMMA_CHAR)
    {
      commandBuffer_p->position++;
    }
    else
    {
      if (commandBuffer_p->character[commandBuffer_p->position] != SEMICOLON_CHAR)
      {
        if (isdigit (commandBuffer_p->character[commandBuffer_p->position]))
        {
          value = getDecimalValue (commandBuffer_p);
          *value_p = value;
          useInValue = FALSE;
          *supplied_p = TRUE;
          if (commandBuffer_p->position < commandBuffer_p->length - 1)
          {
            if (commandBuffer_p->character[commandBuffer_p->position] == COMMA_CHAR)
            {
              commandBuffer_p->position++;
            }
          }
        }
        else
        {
          result = FALSE;
        }
      }
    }
  }

  if (useInValue)
  {
    *value_p = inValue;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    getExtendedParameterSigned
 *
 * Parameters:  (InOut) commandBuffer_p - a pointer to a CommandLine_t
 *              structure with the position field indicating the start of
 *              where a decimal value may be.
 *              (Out) value_p - a pointer to an Int32 value where the
 *              equivalent to the decimal value if given, or the value
 *              of the thrid parameter if not, is to be stored.
 *              (In) inValue - an Int32 value to be used if the parameter
 *              is not supplied.
 *              (Out) supplied_p - a pointer to a Boolean value to say if the
 *              parameter is supplied by user.
 *
 * Returns:     Boolean indicating the success or failure in obtaining an
 *              extended AT command's parameter value.
 *
 * Description: Attempts to obtain the extended AT command parameter at
 *              position, moving position to the point after the parameter
 *              and returning TRUE if the extended parameter value returned
 *              in *value_p is OK, else FALSE if it is not.
 *              The difference between getExtendedParameter() and this function is that this
 *              function is an enhanced version of getExtendedParameter which support parsing
 *              negative symbol "-"
 *
 *-------------------------------------------------------------------------*/

Boolean getExtendedParameterSigned (CommandLine_t *commandBuffer_p,
                                    SignedInt32 *value_p,
                                    Int32 inValue)
{
  Boolean result = TRUE;
  Boolean useInValue = TRUE;
  Int32   value;
  SignedInt8 signed_flag = 1;

  signed_flag = 1;

  if (commandBuffer_p->position < commandBuffer_p->length -1)
  {
    if (commandBuffer_p->character[commandBuffer_p->position] == COMMA_CHAR)
    {
      commandBuffer_p->position++;
    }
    else
    {
      if (commandBuffer_p->character[commandBuffer_p->position] != SEMICOLON_CHAR)
      {
        if(commandBuffer_p->character[commandBuffer_p->position] == ('-'))
        {
          commandBuffer_p->position++;
          signed_flag = -1;
        }
        if (isdigit (commandBuffer_p->character[commandBuffer_p->position]))
        {
          value = getDecimalValue (commandBuffer_p);
          *value_p = (SignedInt32)value*signed_flag;
          useInValue = FALSE;
          if (commandBuffer_p->position < commandBuffer_p->length - 1)
          {
            if (commandBuffer_p->character[commandBuffer_p->position] == COMMA_CHAR)
            {
              commandBuffer_p->position++;
            }
          }
        }
        else
        {
          result = FALSE;
        }
      }
    }
  }

  if (useInValue == TRUE)
  {
    *value_p = inValue;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    checkForExtendedCommandSeparator
 *
 * Parameters:  (In) result - ResultCode_t indicating the result of the
 *              extended syntax command.
 *              (InOut) scanParseContext_p - pointer to a ScanParseContext_t
 *              structure containing the command string being scanned.
 *
 * Returns:     Nothing
 *
 * Description: If the result is "OK", and the end of the command buffer,
 *              has not been reached, and the next character is a semicolon,
 *              then the position is incremented past the semicolon.
 *
 *-------------------------------------------------------------------------*/

/* job106314: modify 2nd parameter for function call */
Boolean checkForExtendedCommandSeparator (const VgmuxChannelNumber entity,
                                           ScanParseContext_t *scanParseContext_p)
{
  Boolean result = TRUE;

  /* added for job106314 */
  CommandLine_t *commandBuffer_p = &scanParseContext_p->nextCommand;

  if (commandBuffer_p->position < commandBuffer_p->length - 1 )
  {

#if !defined (DISABLE_ATCI_IGNORE_EXTRA_AT_PARAMS)
    /*
     * If the user entered extra parameters for an extended command (+... or *...) then we need to throw them
     * away to prevent an ERROR reponse caused by ATCI trying to parse the remaining parameters as an AT command.
     * This does, however, prevent you from entering a non extended command after an extended command without a semicolon.
     * For example AT+COPS?s3=10  only the +COPS? will be executed and the S3=10 will be thrown away by this code.
     */
    if (scanParseContext_p->currentCommandType == EXTENDED_SYNTAX)
    {
      /*
       * If it is an extended command then search for the next semicolon or plus, star, hat or end of the string..
       *
       */
      while ( (commandBuffer_p->character[commandBuffer_p->position] != SEMICOLON_CHAR) &&
              (commandBuffer_p->character[commandBuffer_p->position] != PLUS_CHAR) &&
              (commandBuffer_p->character[commandBuffer_p->position] != STAR_CHAR) &&
              (commandBuffer_p->character[commandBuffer_p->position] != HAT_CHAR) &&
              (commandBuffer_p->position < commandBuffer_p->length))
      {
        commandBuffer_p->position++;
      }

      /*
       * Did this take us to the end of the line?
       */
      if (commandBuffer_p->position == commandBuffer_p->length)
      {
        result = FALSE;
      }
    }
#endif
    if (result == TRUE)
    {

      if ((commandBuffer_p->character[commandBuffer_p->position] == SEMICOLON_CHAR) ||
          (commandBuffer_p->character[commandBuffer_p->position] == SPACE_CHAR))
      {
        /* Check for white space before the semicolon.... */
        while ( (commandBuffer_p->character[commandBuffer_p->position] == SPACE_CHAR) &&
                (commandBuffer_p->position < commandBuffer_p->length))
        {
          commandBuffer_p->position++;
        }

        /* Skip past semicolon if present.... */
        if (commandBuffer_p->character[commandBuffer_p->position] == SEMICOLON_CHAR)
        {
          commandBuffer_p->position++;
        }

        /* Skip over any space following the semicolon.... */
        while ( (commandBuffer_p->character[commandBuffer_p->position] == SPACE_CHAR) &&
                (commandBuffer_p->position < commandBuffer_p->length))
        {
          commandBuffer_p->position++;
        }

        /* shuffle contents */
        memcpy (&commandBuffer_p->character[0],
                 &commandBuffer_p->character[commandBuffer_p->position],
                  commandBuffer_p->length - commandBuffer_p->position);

        /* calculate new length */
        commandBuffer_p->length = commandBuffer_p->length -
                                   commandBuffer_p->position;

        /* clear up memory */
        memset (&commandBuffer_p->character[commandBuffer_p->length],
                 NULL_CHAR,
                  commandBuffer_p->position);

        commandBuffer_p->position = 0;
      }
      else
      {
        /* extended commands must be delimited by semi */
        /* job106314: but only if they follow an extended syntax command */
        /* no semicolon required if they follow a basic syntax command */
        if (((commandBuffer_p->character[0] == PLUS_CHAR) ||
             (commandBuffer_p->character[0] == STAR_CHAR) ||
             (commandBuffer_p->character[0] == HAT_CHAR)) &&
             (scanParseContext_p->currentCommandType == EXTENDED_SYNTAX))
        {
          commandBuffer_p->position = commandBuffer_p->length;
          result = FALSE;
          setResultCode (entity, RESULT_CODE_ERROR);
        }
      }
    }
  }

  return (result);
}

 /*--------------------------------------------------------------------------
 *
 * Function:    getDecimalValue
 *
 * Parameters:  (InOut) commandBuffer_p - a pointer to a CommandLine_t
 *              structure with the position field indicating the start of
 *              where a decimal value may be.
 *
 * Returns:     Int32 containing the value equivalent to the decimal value
 *              found, or zero if no decimal value found.
 *
 * Description: Returns the value equivalent to the decimal value
 *              found at position, or zero if no decimal value found,
 *              and moves position to after the decimal value.
 *
 *-------------------------------------------------------------------------*/

Int32 getDecimalValue (CommandLine_t *commandBuffer_p)
{
  Int32 value = 0;

  while ((commandBuffer_p->position < commandBuffer_p->length - 1) &&
         (isdigit (commandBuffer_p->character[commandBuffer_p->position])))
  {
    value *= 10;
    value += (commandBuffer_p->character[commandBuffer_p->position] - '0');
    commandBuffer_p->position++;
  }

  return (value);
}

 /*--------------------------------------------------------------------------
 *
 * Function:    getDecimalValueSafe
 *
 * Parameters:  (InOut) commandBuffer_p - a pointer to a CommandLine_t
 *              structure with the position field indicating the start of
 *              where a decimal value may be.
 *
 * Returns:     Int32 containing the value equivalent to the decimal value
 *              found, or zero if no decimal value found.
 *
 * Description: Returns the value equivalent to the decimal value
 *              found at position, or zero if no decimal value found,
 *              and moves position to after the decimal value.
 *-------------------------------------------------------------------------*/

static Boolean iscomma (Char c)
{
  Boolean result = FALSE;

  if ((c == SEMICOLON_CHAR) || (c == COMMA_CHAR))
  {
    result = TRUE;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    getDecimalValueSafe
 *
 * Parameters:
 *
 * Returns:
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/

Boolean getDecimalValueSafe (CommandLine_t *commandBuffer_p, Int32 *value)
{
  Boolean ret, doloop;

  ret = (Boolean)((commandBuffer_p->position < commandBuffer_p->length -1) &&
         (((Boolean)isdigit (commandBuffer_p->character[commandBuffer_p->position])) ||
          ((Boolean)iscomma (commandBuffer_p->character[commandBuffer_p->position]))));

  doloop = (Boolean)((commandBuffer_p->position < commandBuffer_p->length -1) &&
            ((Boolean)isdigit (commandBuffer_p->character[commandBuffer_p->position])));

  (*value)=0;

  while (doloop == TRUE)
  {
    ret = (Boolean)((ret == TRUE) && ((*value) < (MaxInt32 / 10)));
    if (ret == TRUE)
    {
      (*value) *= 10;
      (*value) += (commandBuffer_p->character[commandBuffer_p->position] - '0');
      commandBuffer_p->position++;
    }
    doloop = (Boolean)((ret) && (commandBuffer_p->position < commandBuffer_p->length -1) &&
                       (isdigit (commandBuffer_p->character[commandBuffer_p->position])));
  }

  return (ret);
}


/*--------------------------------------------------------------------------
 *
 * Function:    getExtendedHexParameter
 *
 * Parameters:  (InOut) commandBuffer_p - a pointer to a CommandLine_t
 *              structure with the position field indicating the start of
 *              where a decimal value may be.
 *              (Out) value_p - a pointer to an Int32 value where the
 *              equivalent to the hexadecimal value if given, or the value
 *              of the third parameter if not, is to be stored.
 *              (In) inValue - an Int32 value to be used if the parameter
 *              is not supplied.
 *              (Out) supplied_p - a pointer to a Boolean value to say if the
 *              parameter is supplied by user.
 *
 * Returns:     Boolean indicating the success or failure in obtaining an
 *              extended AT command's parameter value.
 *
 * Description: Attempts to obtain the extended AT command parameter at
 *              position, moving position to the point after the parameter
 *              and returning TRUE if the extended parameter value returned
 *              in *value_p is OK, else FALSE if it is not.
 *
 *-------------------------------------------------------------------------*/

Boolean getExtendedHexParameter (CommandLine_t *commandBuffer_p,
                                  Int32 *value_p,
                                   const Int32 inValue,
                                    Boolean *supplied_p)
{
  Boolean result = TRUE;
  Boolean useInValue = TRUE;
  Int32   value;

  *supplied_p = FALSE;

  if (commandBuffer_p->position < commandBuffer_p->length -1)
  {
    if (commandBuffer_p->character[commandBuffer_p->position] == COMMA_CHAR)
    {
      commandBuffer_p->position++;
    }
    else
    {
      if (commandBuffer_p->character[commandBuffer_p->position] != SEMICOLON_CHAR)
      {
        if (isxdigit (commandBuffer_p->character[commandBuffer_p->position]))
        {
          value = getHexaNum (commandBuffer_p);
          *value_p = value;
          useInValue = FALSE;
          *supplied_p = TRUE;
          if (commandBuffer_p->position < commandBuffer_p->length - 1)
          {
            if (commandBuffer_p->character[commandBuffer_p->position] == COMMA_CHAR)
            {
              commandBuffer_p->position++;
            }
          }
        }
        else
        {
          result = FALSE;
        }
      }
    }
  }

  if (useInValue)
  {
    *value_p = inValue;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    getHexaValue
 *
 * Parameters:  (In) c - a character, that scould be converted to the Hexa value
 *              (Out) value - the Hexa value
 *
 * Returns:     Boolean indicating the success or failure
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/

Boolean getHexaValue (Char c, Int8 *value)
{
  Boolean result = TRUE;

  if ((c >= '0') && (c <= '9'))
  {
    *value = c - '0';
  }
  else
  {
    if ((c >= 'a') && (c <= 'f'))
    {
      *value = c - 'a' + 10;
    }
    else
    {
      if ((c >= 'A') && (c <= 'F'))
      {
        *value = c - 'A' + 10;
      }
      else
      {
        result = FALSE;
      }
    }
  }

  return (result);
}

 /*--------------------------------------------------------------------------
  *
  * Function:    getHexaValue
  *
  * Parameters:  (In) c - a character, that scould be converted to the decimal value
  *              (Out) value - the decimal value
  *
  * Returns:     Boolean indicating the success or failure
  *
  * Description:
  *
  *-------------------------------------------------------------------------*/
 
 Boolean getDeciaValue (Char c, Int8 *value)
 {
   Boolean result = TRUE;
 
   if ((c >= '0') && (c <= '9'))
   {
     *value = c - '0';
   }
   else
   {
     result = FALSE;
   }
 
   return (result);
 }

 /*--------------------------------------------------------------------------
 *
 * Function:    getHexaNum
 *
 * Parameters:  (InOut) commandBuffer_p - a pointer to a CommandLine_t
 *              structure with the position field indicating the start of
 *              where a hexadecimal value may be.
 *
 * Returns:     Int32 containing the value equivalent to the hexadecimal value
 *              found, or zero if no hexadecimal value found.
 *
 * Description: Returns the value equivalent to the hexadecimal value
 *              found at position, or zero if no hexadecimal value found,
 *              and moves position to after the hexadecimal value.
 *
 * Note:        Maximum number of characters that can be converted is 8
 *              (FFFFFFFF or Int32).
 *              It stops after converting 8 input characters.
 *
 *-------------------------------------------------------------------------*/

Int32 getHexaNum (CommandLine_t *commandBuffer_p)
{
  Int32 value = 0;
  Int8 tmpVal = 0;
  Int16 maxPosition = commandBuffer_p->position + (2 * INT8S_PER_INT32); /* 8 */

  while ((commandBuffer_p->position < commandBuffer_p->length - 1) &&
         (commandBuffer_p->position < maxPosition) &&
         (getHexaValue (commandBuffer_p->character[commandBuffer_p->position], &tmpVal)))
  {
    value <<= 4;
    value += tmpVal;
    commandBuffer_p->position++;
  }

  return (value);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgConvertStringToLower
 *
 * Parameters:  inputString - string to have case changed
 *              outputString - inputString in lower case
 *
 * Returns:     nothing
 *
 * Description: Converts inputString into lower case and puts results in
 *              outputString
 *
 *-------------------------------------------------------------------------*/

void vgConvertStringToLower (const Char *inputString, Char *outputString)
{
  Int16 index;
  Int16 length = (Int16)vgStrLen (inputString);

  for (index = 0;
        index <= length;
         index++)
  {
    outputString[index] = (Char)tolower (inputString[index]);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgConvertStringToUpper
 *
 * Parameters:  inputString - string to have case changed
 *              outputString - inputString in upper case
 *
 * Returns:     nothing
 *
 * Description: Converts inputString into upper case and puts results in
 *              outputString
 *
 *-------------------------------------------------------------------------*/

void vgConvertStringToUpper (const Char *inputString, Char *outputString)
{
  Int16 index;
  Int16 length = (Int16)vgStrLen(inputString);

  for (index = 0;
        index < length;
         index++)
  {
    outputString[index] = (Char)toupper (inputString[index]);
  }
  outputString[index] = NULL_CHAR;

}


/*--------------------------------------------------------------------------
 *
 * Function:    vgPrintLine
 *
 * Parameters:  (In)    in - string to be appended
 *              (InOut) out - output string to be printed
 *
 * Returns:     Int16 - length of input string
 *
 * Description: Concatenates in onto out
 *
 *-------------------------------------------------------------------------*/


Char *vgPrintLine(Char *out, const Char *in)
{
  while (*in)
  {
    *out++ = *in++;
  }
  *out = (Char)0;

  return (out);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgPrint32BitNum
 *
 * Parameters:  (In)    num - number to be appended
 *              (InOut) out - output string to be printed
 *
 * Returns:     Int32 - length of input string representation of num
 *
 * Description: Concatenates string representing num onto out
 *
 *-------------------------------------------------------------------------*/

Char *vgPrint32BitNum (Char *out, Int32 num)
{
  Int32 temp, i = 1;

  /* need a special case to ensure everything keeps within 32 bits */
  if (num >= 1000000000)
  {
    i = 1000000000;
  }
  else
  {
    while (num >= (i * 10))
    {
      i *= 10;
    }
  }

  do
  {
    temp = num / i;
    *out++ = ((Char)temp) + '0';
    num = num - (temp * i);
    i /= 10;
  }
  while (i > 0);

  *out = (Char)0;

  return (out);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgOp8BitHex
 *
 * Parameters:  input - variable to print
 *              output - pointer to output space
 *
 * Returns:     new pointer to next free position
 *
 * Description: outputs a hexadecimal representation of an 8 bit number
 *
 *-------------------------------------------------------------------------*/

Char *vgOp8BitHex (Int8 input, Char *output)
{
  *output++ = vgHexTable[input >> 4];
  *output++ = vgHexTable[input & 0x0f];
  *output = (Char)0;

  return (output);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgOp32BitHex
 *
 * Parameters:  input - variable to print
 *              bytes - Number of printed bytes (LSB)
 *              output - pointer to output space
 *
 * Returns:     new pointer to next free position
 *
 * Description: outputs a hexadecimal representation of an 32 bit number
 *
 *-------------------------------------------------------------------------*/

Char *vgOp32BitHex (Int32 input, Int8 bytes, Char *output)
{
  Int8 i;

  for (i = 4; i > 0; i--)
  {
    if (i <= bytes)
    {
      output = vgOp8BitHex ((Int8)((input >> ((i - 1) * 8)) & 0xFF), output);
    }
  }
  *output = (Char)0;

  return (output);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgHexaToBin
 *
 * Parameters:  input - variable to print
 *              output - pointer to output space
 *
 * Returns:     new pointer to next free position
 *
 * Description: outputs a 32bit number representation of a hexadecimal number
 *
 *-------------------------------------------------------------------------*/

Boolean vgHexaToBin (const Char *input, Int32 *output)
{
  Boolean success = TRUE;
  Int32 result = 0;
  Char i, chr, val = 0;

  if (*input == 0)
  {
    success = FALSE;
  }
  else
  {
    for (i = 0; (i < 8) && (*input != 0) && (success == TRUE); i++, input++)
    {
      chr = *input;

      /* Convert to upper case */
      if ((chr >= 'a') && (chr <= 'f'))
      {
        chr -= 'a' - 'A';
      }

      if ((chr >= '0') && (chr <= '9'))
      {
        val = chr - '0';
      }
      else
      {
        if ((chr >= 'A') && (chr <= 'F'))
        {
          val = chr - 'A' + 10;
        }
        else
        {
          success = FALSE;
        }
      }

      if (success == TRUE)
      {
        result = (result << 4) + val;
      }
    }

    if (*input != 0)
    {
      success = FALSE;
    }
  }

  *output = result;

  return (success);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgBinStrToBin
 *
 * Parameters:  input - variable to print
 *              output - pointer to output space
 *
 * Returns:     new pointer to next free position
 *
 * Description: Converts an 8 bit number in a binary string to
 *              int8
 *
 *-------------------------------------------------------------------------*/

Boolean vgBinStrToBin (const Char *input, Int8 *output)
{
  Boolean success = TRUE;
  Int32 result = 0;
  Char i, chr, val = 0;

  if (*input == 0)
  {
    success = FALSE;
  }
  else
  {
    /* Limit to 8 bits - Int8 */ 
    for (i = 0; (i < 8) && (*input != 0) && (success == TRUE); i++, input++)
    {
      chr = *input;

      if ((chr == '0') || (chr == '1'))
      {
        val = chr - '0';
      }
      else
      {
        success = FALSE;
      }

      if (success == TRUE)
      {
        result = (result << 1) + val;
      }
    }

    if (*input != 0)
    {
      success = FALSE;
    }
  }

  *output = result;

  return (success);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgInt8ToBinString
 *
 * Parameters:      input (in)   - number value
 *
 *                  output(out)  - String containing (binary) number input 
 *
 * Returns:         nothing
 *
 * Description: Converts numerical value to binary string.
 *
 *-------------------------------------------------------------------------*/


void vgInt8ToBinString( const Int8 input, Int8 num_bits, Char *output)
{
  Int8 i, x=0;
  Int8 val;

  for (i = num_bits; i > 0; i--)
  {
    x = num_bits - i;
    val = input >> (i-1)& 0x1;
    if (val == 0)
    {
      output[x] = '0';
    }
    else
    {
     output[x] = '1';
    }
  }
  output[x+1] = (Char)0;// add string null terminator
} 
  

/*--------------------------------------------------------------------------
 *
 * Function:    vgInt16ToAscii
 *
 * Parameters:      input (in)   - number value
 *
 *                  output(out)  - String containing (hex) number in
 *
 * Returns:         Int16 value of passed string.
 *
 * Description: Converts numerical value to acsii text.
 *
 *-------------------------------------------------------------------------*/

void vgInt16ToAscii (const Int16 input, Char *output)
{
  Int8 i;

  for (i = 0; i < 2; i++)
  {
    Int8 value = (Int8) (input >> (8*(1-i)) & 0x00FF);
    Int8 hvalue = value >> 4;
    Int8 lvalue = value & 0x0F;

    if ( hvalue < 10 )
    {
        output[i*2] = hvalue + '0';
    }
    else
    {
        output[i*2] = hvalue - 0x0a + 'A';
    }

    if ( lvalue < 10 )
    {
        output[i*2+1] = lvalue + '0';
    }
    else
    {
        output[i*2+1] = lvalue - 0x0a + 'A';
    }
  }
}

#if defined ( FEA_PHONEBOOK)
/*--------------------------------------------------------------------------
 *
 * Function:        vgConvBcdToTextForPhonebook
 *
 * Parameters:      (In) numType - type of Bcd number
 *                  (In) bcdData_p - pointer to Bcd string
 *                  (In) bcdDataLen - length of Bcd string
 *                  (InOut) textDialNum_p - pointer to output text dial string
 *
 * Returns:         pointer to output string
 *
 * Description:     gets Bcd digit from Bcd string and appends it to
 *                  textDialNum_p
 *
 *-------------------------------------------------------------------------*/

Char *vgConvBcdToTextForPhonebook (BcdNumberType numType,
                        const Bcd *bcdData_p,
                         Int8 bcdDataLen,
                          Char *textDialNum_p)
{
  const Bcd   *bcdString_p = bcdData_p;
  Int16 pos = 0;
  Int16 out = 0;
  Int8 val = 0;
  Int16 len = (bcdDataLen * 2);

  if (numType == NUM_TYPE_INTERNATIONAL)
  {
    *textDialNum_p++ = INTERNATIONAL_PREFIX;
    out++;
  }

  while ((pos < len) && (out < MAX_CALLED_BCD_NO_LENGTH))
  {
    val = vgGetBcd (bcdString_p, pos);
    switch (val)
    {
      case 0x0:
      case 0x1:
      case 0x2:
      case 0x3:
      case 0x4:
      case 0x5:
      case 0x6:
      case 0x7:
      case 0x8:
      case 0x9:
      {
        *textDialNum_p++ = val - 0x00 + '0';
        out++;
        break;
      }
      case 0xA:
      { /* star (can be changed) */
        *textDialNum_p++ = STAR_CHAR;
        out++;
        break;
      }
      case 0xB:
      { /* hash (can be changed) */
        *textDialNum_p++ = HASH_CHAR;
        out++;
        break;
      }
      case 0xC:
      { /* an a (can be changed) */
        *textDialNum_p++ = (Char)'p';
        out++;
        break;
      }
      case 0xD:
      { /* a b (can be changed) */
        *textDialNum_p++ = (Char)'w';
        out++;
        break;
      }
      case 0xE:
      { /* a c (can be changed) */
        *textDialNum_p++ = (Char)'r';
        out++;
        break;
      }
      case 0xF:
      { /* filler character - ignore */
        break;
      }
      default:
      {
        /*
         * Any other value - ignore...
         */
        break;
      }
    }
    pos++;
  }

  *textDialNum_p = (Char)0;

  return (textDialNum_p);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgConvBcdToText
 *
 * Parameters:      (In) numType - type of Bcd number
 *                  (In) bcdData_p - pointer to Bcd string
 *                  (In) bcdDataLen - length of Bcd string
 *                  (InOut) textDialNum_p - pointer to output text dial string
 *
 * Returns:         pointer to output string
 *
 * Description:     gets Bcd digit from Bcd string and appends it to
 *                  textDialNum_p
 *
 *-------------------------------------------------------------------------*/

Char *vgConvBcdToText (BcdNumberType numType,
                        const Bcd *bcdData_p,
                         Int8 bcdDataLen,
                          Char *textDialNum_p)
{
  const Bcd   *bcdString_p = bcdData_p;
  Int16 pos = 0;
  Int16 out = 0;
  Int8 val = 0;
  Int16 len = (bcdDataLen * 2);

  if (numType == NUM_TYPE_INTERNATIONAL)
  {
    *textDialNum_p++ = INTERNATIONAL_PREFIX;
    out++;
  }

  while ((pos < len) && (out < MAX_CALLED_BCD_NO_LENGTH))
  {
    val = vgGetBcd (bcdString_p, pos);
    switch (val)
    {
      case 0x0:
      case 0x1:
      case 0x2:
      case 0x3:
      case 0x4:
      case 0x5:
      case 0x6:
      case 0x7:
      case 0x8:
      case 0x9:
      {
        *textDialNum_p++ = val - 0x00 + '0';
        out++;
        break;
      }
      case 0xA:
      { /* star (can be changed) */
        *textDialNum_p++ = STAR_CHAR;
        out++;
        break;
      }
      case 0xB:
      { /* hash (can be changed) */
        *textDialNum_p++ = HASH_CHAR;
        out++;
        break;
      }
      case 0xC:
      { /* an a (can be changed) */
        *textDialNum_p++ = (Char)'A';
        out++;
        break;
      }
      case 0xD:
      { /* a b (can be changed) */
        *textDialNum_p++ = (Char)'B';
        out++;
        break;
      }
      case 0xE:
      { /* a c (can be changed) */
        *textDialNum_p++ = (Char)'C';
        out++;
        break;
      }
      case 0xF:
      { /* filler character - ignore */
        break;
      }
      default:
      {
        /*
         * Any other value - ignore...
         */
        break;
      }
    }
    pos++;
  }

  *textDialNum_p = (Char)0;

  return (textDialNum_p);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgConvTextToBcd
 *
 * Parameters:  (In) asciiDialNum_p - a pointer to an array of Char conatining a
 *              zero terminated ASCII string containing only valid dial
 *              digits.
 *              (In) asciiLen - Int8 indicating the length of the ascii array
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

void vgConvTextToBcd (const Char *asciiDialNum_p,
                       Int16 asciiLen,
                        BcdNumberType *numType_p,
                         BcdNumberPlan *numPlan_p,
                          Bcd *bcdData_p,
                           Int8 *bcdDataLen_p,
                            VgmuxChannelNumber entity)
{
  Int16            pos               = 0;
  Int16            out               = 0;
  Int8             val               = 0;
  Bcd              *bcdString_p      = bcdData_p;
  BcdNumberPlan    nPlan             = BCD_PLAN_E163_E164;
  GeneralContext_t *generalContext_p = ptrToGeneralContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  *numType_p = NUM_TYPE_UNKNOWN;

  if (pos < asciiLen)
  {
    if (asciiDialNum_p[pos] == CHR_PLUS)
    {
      *numType_p = NUM_TYPE_INTERNATIONAL;
      pos++;
    }
    else
    {
      if ((pos + 1) < asciiLen)
      {
        if ( (asciiDialNum_p[pos] == '0') && (asciiDialNum_p[pos + 1] == '0') )
        {
          if (((generalContext_p->vgLmData.phoneBookNumTypePresent == TRUE) &&
               (generalContext_p->vgLmData.phoneBookNumType == NUM_TYPE_INTERNATIONAL)) ||
               (generalContext_p->vgLmData.phoneBookNumTypePresent == FALSE))
          {
            *numType_p = NUM_TYPE_INTERNATIONAL;
            pos += 2;
          }
        }
      }
    }
  }

  while (pos < asciiLen)
  {
    switch (asciiDialNum_p[pos])
    {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      {
        val = asciiDialNum_p[pos] - '0';
        vgPutBcd (bcdString_p, out, val);
        out++;
        break;
      }
      case STAR_CHAR:
      {
        vgPutBcd (bcdString_p, out, 0xA);
        out++;
        break;
      }
      case HASH_CHAR:
      {
        vgPutBcd (bcdString_p, out, 0xB);
        out++;
        break;
      }
      case 'a':
      case 'A':
      {
        vgPutBcd (bcdString_p, out, 0xC);
        out++;
        break;
      }
      case 'b':
      case 'B':
      {
        vgPutBcd (bcdString_p, out, 0xD);
        out++;
        break;
      }
      case 'c':
      case 'C':
      {
        vgPutBcd (bcdString_p, out, 0xE);
        out++;
        break;
      }
      case 'p':
      case 'P':
      {
        vgPutBcd (bcdString_p, out, 0xc);
        out++;
        break;
      }
      case 'w':
      case 'W':
      {
        vgPutBcd (bcdString_p, out, 0xd);
        out++;
        break;
      }
      default:
      {
        break;
      }
    }
    pos++;
  }

  if ((out % 2) != 0)
  {
    vgPutBcd (bcdString_p, out, 0xF);
    out++;
  }

  if (bcdDataLen_p != PNULL)
  {
    if (out > MAX_CALLED_BCD_NO_LENGTH)
    {
      out = MAX_CALLED_BCD_NO_LENGTH;
    }
    out /= 2;
    *bcdDataLen_p = (Int8) out;
  }

  if (numPlan_p != PNULL)
  {
    *numPlan_p = nPlan;
  }
}
#endif /* FEA_PHONEBOOK */

/*--------------------------------------------------------------------------
 *
 * Function:    vgConvTextToSparseBcd
 *
 * Parameters:  (In) asciiDialNum_p - a pointer to an array of Char conatining a
 *              zero terminated ASCII string containing only valid dial
 *              digits.
 *              (In) asciiLen - Int8 indicating the length of the ascii array
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

void vgConvTextToSparseBcd (const Char *asciiDialNum_p,
                             Int8 asciiLen,
                              BcdNumberType *numType_p,
                               BcdNumberPlan *numPlan_p,
                                Bcd *bcdData_p,
                                 Int8 *bcdDataLen_p)
{
  Int16         pos = 0;
  Int16         out = 0;
  Bcd           *bcdString_p = bcdData_p;
  BcdNumberPlan nPlan = BCD_PLAN_E163_E164;

  *numType_p = NUM_TYPE_UNKNOWN;

  if (pos < asciiLen)
  {
    if (asciiDialNum_p[pos] == CHR_PLUS)
    {
      *numType_p = NUM_TYPE_INTERNATIONAL;
      pos++;
    }
    else
    {
      if ((pos + 1) < asciiLen)
      {
        if ( (asciiDialNum_p[pos] == '0') && (asciiDialNum_p[pos + 1] == '0') )
        {
          *numType_p = NUM_TYPE_INTERNATIONAL;
          pos += 2;
        }
      }
    }
  }

  while (pos < asciiLen)
  {
    switch (asciiDialNum_p[pos])
    {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      {
        bcdString_p[out++] = asciiDialNum_p[pos] - '0';
        break;
      }
      case STAR_CHAR:
      {
        bcdString_p[out++] = 0xA;
        break;
      }
      case HASH_CHAR:
      {
        bcdString_p[out++] = 0xB;
        break;
      }
      case 'a':
      case 'A':
      {
        bcdString_p[out++] = 0xC;
        break;
      }
      case 'b':
      case 'B':
      {
        bcdString_p[out++] = 0xD;
        out++;
        break;
      }
      case 'c':
      case 'C':
      {
        bcdString_p[out++] = 0xE;
        break;
      }
      default:
      {
        break;
      }
    }
    pos++;
  }

  if (bcdDataLen_p != PNULL)
  {
    *bcdDataLen_p = (Int8) out;
  }

  if (numPlan_p != PNULL)
  {
    *numPlan_p = nPlan;
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgConvSparseBcdToText
 *
 * Parameters:      (In) numType - type of Bcd number
 *                  (In) bcdData_p - pointer to Bcd string
 *                  (In) bcdDataLen - length of Bcd string
 *                  (InOut) textDialNum_p - pointer to output text dial string
 *
 * Returns:         pointer to output string
 *
 * Description:     gets Bcd digit from Bcd string and appends it to
 *                  textDialNum_p
 *
 *-------------------------------------------------------------------------*/

Char *vgConvSparseBcdToText (BcdNumberType numType,
                              const Bcd *bcdData_p,
                               Int8 bcdDataLen,
                                Char *textDialNum_p )
{
  Int16   pos = 0, out = 0;
  Int8 val = 0;

  static const Char numbers[0xE] =
  { '0','1','2','3','4','5','6','7','8','9','*','#','P','?' };

  if (numType == NUM_TYPE_INTERNATIONAL)
  {
    textDialNum_p[out++] = INTERNATIONAL_PREFIX;
  }

  while (pos < bcdDataLen)
  {
    val = bcdData_p[pos++];

    if (val <= 0xD)
    {
      textDialNum_p[out++] = numbers[val];
    }
    else if (val == 0xE)
    {
      /* extended value - get next digit and convert to ASCII */
      if (pos + 1 < bcdDataLen)
      {
        val = bcdData_p[pos++];

        /* value F is invalid, all others OK */
        if (val != 0xF )
        {
          textDialNum_p[out++] = val + 'A';
        }
      }
    }
  }

  textDialNum_p[out] = (Char)0;   /* don't increment out !!! */

  return (textDialNum_p + out);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgPackBcdString
 *
 * Parameters:  outBcd_p - pointer to string to store conversion in
 *                      inBcd_p - pointer to input string
 *                      inBcdLen - length of in Bcd string
 *
 * Returns:     Int16 - length of packed string
 *
 * Description: Packs Bcd string from digit per single byte to two digits per byte.
 *
 *-------------------------------------------------------------------------*/

Int16 vgPackBcdString (Bcd *outBcd_p,
                        const Bcd *inBcd_p,
                         const Int16 inBcdLen)
{
  Int16 i, j;

  for (i = 0, j = 0; i < inBcdLen; i++, j++)
  {
    outBcd_p[i] = 0x00;
    outBcd_p[i] = ((inBcd_p[j] << 4) & 0xF0);

    j++;

    if (j == inBcdLen)
    {
      outBcd_p[i] |=  0x0F;
    }

    else
    {
      outBcd_p[i] |=  (inBcd_p[j] & 0x0F);
    }
  }

  return ((inBcdLen / 2) + (inBcdLen % 2));
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgAsciiToInt16
 *
 * Parameters:      hexString_p (in)   - String containing (hex) number in
 *                                       ascii to convert NULL TERMINATED!
 *
 * Returns:         Int16 value of passed string.
 *
 * Description:     Converts acsii text to numerical value.
 *
 * Design spec:
 *
 *-------------------------------------------------------------------------*/
Int16 vgAsciiToInt16(const Int8 *hexString_p)
{
  Int16 result = 0, pos = 0;
  Int8 value;

  while ((hexString_p[pos] != '\0') && (pos < 4))
  {
    /* Check whatever's entered is valid (i.e. hex).... */
    if (isxdigit(hexString_p[pos]))
    {
      /* Numerical (0-9).... */
      if (isdigit(hexString_p[pos]))
      {
        value = hexString_p[pos] - '0';
      }

      /* Or a-f.... */
      else
      {
        value = (Int8)tolower(hexString_p[pos]);
        value -= 'a';
        value += 0x0a;
      }
    }
    else
    {
      value = 0;
    }

    /* Convert to 16-bit int.... */
    result = (result << 4) + value;
    pos++;
  }

  return result;
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgAsciiToInt8
 *
 * Parameters:      hexString_p (in)   - string containing 2-digit (hex)
 *                                       number in ascii to convert
 *                  error_p (out)      - error value set to TRUE if the
 *                                       conversion fails
 *
 * Returns:         Int8 value of passed string.
 *
 * Description:     Converts acsii text to numerical value.
 *
 * Design spec:
 *
 *-------------------------------------------------------------------------*/
Int8 vgAsciiToInt8 (const Char *hexString_p, Boolean *error_p)
{
  Int8 result = 0;
  Int8 pos = 0;
  Int8 value;

  *error_p = FALSE;

  while ((pos < 2) && !*error_p)
  {
    /* Check whatever's entered is valid (i.e. hex).... */
    if (isxdigit (hexString_p[pos]))
    {
      /* Numerical (0-9).... */
      if (isdigit (hexString_p[pos]))
      {
        value = hexString_p[pos] - '0';
      }

      /* Or a-f.... */
      else
      {
        value = tolower (hexString_p[pos]);
        value -= 'a';
        value += 0x0a;
      }

      /* Convert to 8-bit int.... */
      result = (result << 4) + value;
      pos++;
    }
    else
    {
      *error_p = TRUE;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        sendSsRegistrationSignal
 *
 * Parameters:      ssCode   - sub system code
 *                  entity   - entity to direct signal to
 *                  signalId - the signal
 *
 * Returns:         Nothing
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/
void sendSsRegistrationSignal (const VoyagerSubsystems_t ssCode,
                                const VgmuxChannelNumber entity,
                                 const SignalId signalId)
{
  SignalBuffer ssSignal = kiNullBuffer;

  KiCreateZeroSignal (SIG_VGCI_SS_REGISTRATION_IND,
                       sizeof (VgCiSsRegistrationInd),
                        &ssSignal);

  ssSignal.sig->vgCiSsRegistrationInd.signalId = signalId;
  ssSignal.sig->vgCiSsRegistrationInd.ssCode   = ssCode;
  ssSignal.sig->vgCiSsRegistrationInd.entity   = entity;

  KiSendSignal (VG_CI_TASK_ID, &ssSignal);
}

/*--------------------------------------------------------------------------
 *
 * Function:        sendDataToCrm
 *
 * Parameters:      string  - pointer to string of text to be sent
 *                  prefix  - Boolean determining if a <CR><LF> precedes text
 *                  postfix - Boolean determining if a <CR><LF> postscripts text
 *
 * Returns:         Nothing
 *
 * Description:     Sends text given in string to the Command Response
 *                  Manager. If the size of the string is too large then
 *                  multiple signals are sent.
 *
 *-------------------------------------------------------------------------*/

void sendDataToCrm (const Char *string,
                     Boolean prefix,
                      Boolean postfix,
                       const VgmuxChannelNumber sendOnChannel)
{
  sendSignalToCrmWithLength (string,
                              (Int16) vgStrLen (string),
                               prefix,
                                postfix,
                                 DATA,
                                  RESULT_CODE_PROCEEDING,
                                    sendOnChannel);
}


/*--------------------------------------------------------------------------
 *
 * Function:        sendDataWithLengthToCrm
 *
 * Parameters:      string_p - pointer to string of text to be sent
 *                  length   - length of string
 *                  prefix   - Boolean determining if a <CR><LF> precedes text
 *                  postfix  - Boolean determining if a <CR><LF> postscripts text
 *
 * Returns:         Nothing
 *
 * Description:     Sends text given in string to the Command Response
 *                  Manager. If the size of the string is too large then
 *                  multiple signals are sent.
 *
 *-------------------------------------------------------------------------*/

void sendDataWithLengthToCrm (const Char* string_p,
                               Int16 length,
                                Boolean prefix,
                                 Boolean postfix,
                                  const VgmuxChannelNumber sendOnChannel)
{
  sendSignalToCrmWithLength (string_p,
                              length,
                               prefix,
                                postfix,
                                 DATA,
                                  RESULT_CODE_PROCEEDING,
                                   sendOnChannel);
}

/*--------------------------------------------------------------------------
 *
 * Function:        sendEchoDataToCrm
 *
 * Parameters:      string  - pointer to string of text to be sent
 *                  prefix  - Boolean determining if a <CR><LF> precedes text
 *                  postfix - Boolean determining if a <CR><LF> postscripts text
 *
 * Returns:         Nothing
 *
 * Description:     Sends text given in string to the Command Response
 *                  Manager. If the size of the string is too large then
 *                  multiple signals are sent.
 *
 *-------------------------------------------------------------------------*/

void sendEchoDataToCrm (const Char *string,
                         Int16 length,
                           const VgmuxChannelNumber sendOnChannel)
{
  sendSignalToCrmWithLength (string,
                              length,
                               FALSE,
                                FALSE,
                                 ECHO_DATA,
                                  RESULT_CODE_PROCEEDING,
                                    sendOnChannel);
}


/*--------------------------------------------------------------------------
 *
 * Function:        sendResultCodeToCrm
 *
 * Parameters:      resultCode - result of the AT command just finished
 *                  entity     - channel in which command was issued
 *
 * Returns:         Nothing
 *
 * Description:     Sends result code to the Command Response Manager
 *                  for display.
 *
 *-------------------------------------------------------------------------*/

void sendResultCodeToCrm (ResultCode_t resultCode,
                           const VgmuxChannelNumber sendOnChannel)
{
  sendSignalToCrmWithLength ((const Char*)"",
                              0,
                               FALSE,
                                FALSE,
                                 RESULT_CODE,
                                  resultCode,
                                    sendOnChannel);
}

/*--------------------------------------------------------------------------
 *
 * Function:        sendIntermidateResultCodeToCrm
 *
 * Parameters:      resultCodeText_p  - pointer to intermediate result
 *                                      string
 *                  entity            - channel in which command was issued
 *
 * Returns:         Nothing
 *
 * Description:     Sends result code to the Command Response Manager
 *                  for display.  Used for intermediate codes where
 *                  there is text output and there is no difference
 *                  in the text for both verbose and non verbose modes.
 *-------------------------------------------------------------------------*/

void sendIntermidateResultCodeToCrm (const Char *resultCodeText_p,
                                      const VgmuxChannelNumber sendOnChannel)
{
  sendSignalToCrmWithLength ((const Char*)resultCodeText_p,
                              (Int16) vgStrLen(resultCodeText_p),
                               FALSE,
                                FALSE,
                                 RESULT_CODE,
                                  RESULT_CODE_INTERMEDIATE,
                                    sendOnChannel);
}

/*--------------------------------------------------------------------------
 *
 * Function:        sendUnsolicitedResultCodeToCrm
 *
 * Parameters:      resultCode - result of the AT command just finished
 *
 * Returns:         Nothing
 *
 * Description:     Sends unsolicited result code to the CRMAN for display
 *
 *-------------------------------------------------------------------------*/

void sendUnsolicitedResultCodeToCrm (const VgmuxChannelNumber entity, ResultCode_t resultCode)
{
  sendSignalToCrmWithLength ((const Char*)"",
                              0,
                               FALSE,
                                FALSE,
                                 RESULT_CODE,
                                  resultCode,
                                    entity);
}


/*--------------------------------------------------------------------------
 *
 * Function:        sendLineDataToCrm
 *
 * Parameters:      pendingConnection - type of connection required
 *                  entity            - channel to send line data on
 *
 * Returns:         Nothing
 *
 * Description:     Sends line data to the CRMAN for processing
 *
 *-------------------------------------------------------------------------*/

void sendLineDataToCrm (const ConnectionClass_t pendingConnection,
                         const VgmuxChannelNumber entity)
{
    SignalBuffer                signal = kiNullBuffer;
    CirmDataInd                *request_p;
    CrManagerGenericContext_t  *crManGenericContext_p = ptrToCrManagerGenericContext();

    ++crManGenericContext_p->cirmDataIndCountFlowControl;

    /* create new line data signal */
    KiCreateSignal( SIG_CIRM_DATA_IND,
                        sizeof (CirmDataInd),
                        &signal);

    request_p = (CirmDataInd *)signal.sig;
    request_p->channelNumber     = entity;
    request_p->prefix            = FALSE;
    request_p->postfix           = FALSE;
    request_p->data[0]           = NULL_CHAR;
    request_p->resultCode        = RESULT_CODE_OK;
    request_p->length            = 0;
    request_p->pendingConnection = pendingConnection;
    request_p->commandId         = VG_AT_NO_COMMAND;
    request_p->classOfData       = LINE_DATA;
    request_p->forceTransmit     = FALSE;

    request_p->isUrc             = FALSE;
    request_p->isSmsUrc          = FALSE;

#if defined(ENABLE_CIRM_DATA_IND)
    KiSendSignal (VG_CI_TASK_ID, &signal);
#else
    vgProcessCirmData(&signal, entity);
    KiDestroySignal ( &signal );
#endif
}

/*--------------------------------------------------------------------------
 *
 * Function:    findFirstEnabledChannel
 *
 * Parameters:  none
 *
 * Returns:     First enabled channel number
 *
 * Description: finds first enabled channel
 *
 *-------------------------------------------------------------------------*/

VgmuxChannelNumber findFirstEnabledChannel (void)
{
  Int8               channelIndex;
  VgmuxChannelNumber entity = VGMUX_CHANNEL_INVALID;

  for (channelIndex = 0;
        channelIndex < CI_MAX_ENTITIES;
         channelIndex++)
  {
    if (isEntityActive ((VgmuxChannelNumber)channelIndex) == TRUE)
    {
      entity = (VgmuxChannelNumber)channelIndex;
      break;
    }
  }

  return (entity);
}

/*--------------------------------------------------------------------------
 *
 * Function:    findFirstDisabledChannel
 *
 * Parameters:  none
 *
 * Returns:     First disabled channel.
 *
 * Description: finds first disabled channel
 *
 *-------------------------------------------------------------------------*/

VgmuxChannelNumber findFirstDisabledChannel (void)
{
  Int8               channelIndex;
  VgmuxChannelNumber entity = VGMUX_CHANNEL_INVALID;

  for (channelIndex = 0;
        channelIndex < CI_MAX_ENTITIES;
         channelIndex++)
  {
    if (isEntityActive ((VgmuxChannelNumber)channelIndex) == FALSE)
    {
      entity = (VgmuxChannelNumber)channelIndex;
      break;
    }
  }

  return (entity);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgCiRunAtCommandInd
 *
 * Parameters:  entity        - mux channel to run command on
 *              commandString - command string containing command to run
 *
 * Returns:     Nothing
 *
 * Description: runs the specified command, only used for task initiated
 *              AT commands (e.g. autoanswer)
 *
 *-------------------------------------------------------------------------*/

void vgCiRunAtCommandInd (const VgmuxChannelNumber entity,
                           const Char *commandString)
{
  SignalBuffer       sigBuff = kiNullBuffer;
  CiRunAtCommandInd  *request_p;
  ScanParseContext_t *scanParseContext_p = ptrToScanParseContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(scanParseContext_p != PNULL, entity, 0, 0);
#endif
  if (getEntityState (entity) == ENTITY_IDLE)
  {
    /* block processing new AT commands */
    setEntityState (entity, ENTITY_PROCESSING);

    /* set task initiated flag, ensure that the AT command to be run is
        * considered in the CRMAN resultCodeInterceptor procedure */
    setTaskInitiated (entity, TRUE);

    scanParseContext_p->nextCommand.position = 0;
    scanParseContext_p->nextCommand.length   = (Int16)(vgStrLen (commandString) + 1);

#if defined(ATCI_ENABLE_DYN_AT_BUFF)
    /* Allocate buffer if not already done so */
    if (scanParseContext_p->nextCommand.character == PNULL)
    {
      vgAllocAtCmdBuffer(entity);
    }
#endif         
    
    memset(scanParseContext_p->nextCommand.character, 0, sizeof(scanParseContext_p->nextCommand.character));
    vgStrNCpy (scanParseContext_p->nextCommand.character, commandString, COMMAND_LINE_SIZE);
    scanParseContext_p->nextCommand.character[vgStrLen (commandString)] = getProfileValue (entity, PROF_S3);

    KiCreateSignal (SIG_CI_RUN_AT_COMMAND_IND,
                         sizeof (CiRunAtCommandInd),
                          &sigBuff );

    request_p = (CiRunAtCommandInd *)sigBuff.sig;

    request_p->entity = entity;
    request_p->activeAtCommand.position = 0;
    request_p->activeAtCommand.length   = (Int16)(vgStrLen (commandString) + 1);
    vgStrNCpy (request_p->activeAtCommand.character, commandString, vgStrLen (commandString));
    request_p->activeAtCommand.character[vgStrLen (commandString)] = getProfileValue (entity, PROF_S3);
    KiSendSignal (VG_CI_TASK_ID, &sigBuff);
  }
  else
  {
    /* ignore the command, we're in the wrong state */
#if defined (ENABLE_RAVEN_ENTITY_STATE_DEBUG)
    printf ("vgCiRunAtCommandInd: command %s ignored, entity %d in wrong state %d",
            (const char *) commandString,
            entity, (int) getEntityState (entity));
#endif
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCiDataEntryModeInd
 *
 * Parameters:  entity - index to the SMS context to use
 *
 * Returns:     result of operation
 *
 * Description: Tells the CRM to enter SMS/CTM text mode. In this mode the
 *              CRM stops interpreting AT commands but instead puts the
 *              text entered into the an SMS message or CTM conversation.
 *
 *-------------------------------------------------------------------------*/
void vgCiDataEntryModeInd (const VgmuxChannelNumber entity,
                            const VgDataEntryMode dataEntryMode)
{
  SignalBuffer        sigBuff = kiNullBuffer;
  CiDataEntryModeInd  *request_p;

  sendSsRegistrationSignal (MUX, /* We want the MUX to receive this message */
                            entity,
                            SIG_CI_DATA_ENTRY_MODE_IND);

  KiCreateZeroSignal (SIG_CI_DATA_ENTRY_MODE_IND,
                       sizeof (CiDataEntryModeInd),
                        &sigBuff );

  request_p = (CiDataEntryModeInd *) sigBuff.sig;
  request_p->entity        = entity;
  request_p->dataEntryMode = dataEntryMode;

  KiSendSignal (VG_CI_TASK_ID, &sigBuff);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCiUserProfLoadedInd
 *
 * Parameters:  entity -  mux channel Id
 *
 * Returns:     Nothing
 *
 * Description: Notify all ATCI sub-system that User profile data has been loaded
 *              for the given entity channel.
 *
 *-------------------------------------------------------------------------*/
void vgCiUserProfLoadedInd( const VgmuxChannelNumber entity )
{
  SignalBuffer sigBuff = kiNullBuffer;

  KiCreateZeroSignal(SIG_CI_USER_PROF_LOADED_IND,
                     sizeof(CiUserProfLoadedInd),
                     &sigBuff );

  sigBuff.sig->ciUserProfLoadedInd.entity = entity;

  KiSendSignal (VG_CI_TASK_ID, &sigBuff);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgIsSmDcsGsm
 *
 * Parameters:      dcs (in)   - Data coding scheme used by SMS.
 *
 * Returns:         TRUE if encoded as GSM, FALSE otherwise
 *
 * Description:     Determines whether SM encoded using HEX or GSM scheme.
 *
 * Design spec:
 *
 *-------------------------------------------------------------------------*/
Boolean vgIsSmDcsGsm(Int8 dcs)
{
  Boolean result = TRUE;

  /*Process coding group.... */
  switch ((dcs >> 4) & 0x0F)
  {
    case 0x00:                      /* 0000 -> 0011 */
    case 0x01:
    case 0x02:
    case 0x03:
    {
      if (((dcs & 0x0C) == 0x04) || ((dcs & 0x0C) == 0x08))
      {
        result = FALSE;
      }
    }
    break;

    case 0x0E:                      /* 1110 */
    {
      /* All UCS2 coded.... */
      result = FALSE;
    }
    break;

    case 0x0F:                      /* 1111 */
    {
      if ((dcs & 0x04) == 0x04)
      {
        result = FALSE;
      }
    }
    break;

    case 0x0C:                      /* 1100 & 1101 */
    case 0x0D:
    default:                        /* 0100 - 1011 */
    {
    }
    break;
  }

  return result;
}

/*--------------------------------------------------------------------------
 *
 * Function:        setResultCode
 *
 * Parameters:      entity     - mux channel
 *                  resultCode - result code to set
 *
 * Returns:         Nothing
 *
 * Description:     sets the current result code for the specified entity
 *
 *-------------------------------------------------------------------------*/

void setResultCode (const VgmuxChannelNumber entity,
                     const ResultCode_t resultCode)
{
  CrManagerContext_t  *crManagerContext_p;

  /*
   * Only set result code if the entity is active.
   */
  if (isEntityActive ((VgmuxChannelNumber)entity) == TRUE)
  {
    crManagerContext_p = ptrToCrManagerContext (entity);

    if (crManagerContext_p != PNULL)
    {
      crManagerContext_p->resultCode = resultCode;

      /* If we are setting a result code - then we are not generating a URC - so
       * clear the setting here
       */
      vgSetCirmDataIndIsUrc(entity, FALSE);
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        getResultCode
 *
 * Parameters:      entity     - mux channel
 *
 * Returns:         Nothing
 *
 * Description:     gets the current result code for the specified entity
 *
 *-------------------------------------------------------------------------*/

ResultCode_t getResultCode (const VgmuxChannelNumber entity)
{
  CrManagerContext_t  *crManagerContext_p;
  ResultCode_t        resultCode = RESULT_CODE_OK;

  FatalAssert (entity < CI_MAX_ENTITIES);

  /*
   * Only read result code if the entity is active.
   */
  if (isEntityActive ((VgmuxChannelNumber)entity) == TRUE)
  {
    crManagerContext_p = ptrToCrManagerContext (entity);

    if (crManagerContext_p != PNULL)
    {
      resultCode = crManagerContext_p->resultCode;
    }
  }

  return (resultCode);
}

/*--------------------------------------------------------------------------
 *
 * Function:        setCommandId
 *
 * Parameters:      entity     - mux channel
 *                  commandId  - command identifier
 *
 * Returns:         Nothing
 *
 * Description:     sets the current AT command id for the specified entity
 *
 *-------------------------------------------------------------------------*/

void setCommandId (const VgmuxChannelNumber entity,
                    const CommandId_t commandId)
{
  CrManagerContext_t  *crManagerContext_p;

  FatalAssert (entity < CI_MAX_ENTITIES);

  /*
   * Only set commandId if the entity is active.
   */
  if (isEntityActive ((VgmuxChannelNumber)entity) == TRUE)
  {
    crManagerContext_p = ptrToCrManagerContext (entity);

    if (crManagerContext_p != PNULL)
    {
      ptrToCrManagerContext (entity)->currentAtCommand = commandId;
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        setCommandName
 *
 * Parameters:      entity        - mux channel
 *                  commandName_p - name of current AT command
 *
 * Returns:         Nothing
 *
 * Description:     sets the current AT command name for the specified entity
 *
 *-------------------------------------------------------------------------*/

void setCommandName (const VgmuxChannelNumber entity,
                      const Char *commandName_p)
{
  CrManagerContext_t  *crManagerContext_p;

  FatalAssert (entity < CI_MAX_ENTITIES);
  FatalAssert (commandName_p != PNULL);

  /*
   * Only set commandName if the entity is active.
   */
  if (isEntityActive ((VgmuxChannelNumber)entity) == TRUE)
  {
    crManagerContext_p = ptrToCrManagerContext (entity);

    if (crManagerContext_p != PNULL)
    {
      crManagerContext_p->currentAtCommandName_p = commandName_p;
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        getCommandId
 *
 * Parameters:      entity     - mux channel
 *
 * Returns:         Id of current AT command
 *
 * Description:     gets the current AT command id for the specified entity
 *
 *-------------------------------------------------------------------------*/

CommandId_t getCommandId (const VgmuxChannelNumber entity)
{
  CrManagerContext_t  *crManagerContext_p;
  CommandId_t         ret = VG_AT_NO_COMMAND;

  FatalAssert (entity < CI_MAX_ENTITIES);

  if (isEntityActive ((VgmuxChannelNumber)entity) == TRUE)
  {
    crManagerContext_p = ptrToCrManagerContext (entity);

    if (crManagerContext_p != PNULL)
    {
      ret = crManagerContext_p->currentAtCommand;
    }
  }
  return (ret);
}

/*--------------------------------------------------------------------------
 *
 * Function:        getCommandName
 *
 * Parameters:      entity     - mux channel
 *
 * Returns:         name of current AT command
 *
 * Description:     gets the current AT command name for the specified entity
 *
 *-------------------------------------------------------------------------*/

const Char *getCommandName (const VgmuxChannelNumber entity)
{
  const Char          *ret_p = (const Char *) "";
  CrManagerContext_t  *crManagerContext_p;

  FatalAssert (entity < CI_MAX_ENTITIES);

  if (isEntityActive ((VgmuxChannelNumber)entity) == TRUE)
  {
    crManagerContext_p = ptrToCrManagerContext (entity);

    if (crManagerContext_p != PNULL)
    {
      ret_p = crManagerContext_p->currentAtCommandName_p;
    }
  }
  return (ret_p);
}

/*--------------------------------------------------------------------------
 *
 * Function:        setConnectionType
 *
 * Parameters:      entity     - mux channel
 *                  connectionClass - type of connection just made
 *
 * Returns:         Nothing
 *
 * Description:     sets the current connection type for the specified entity
 *
 *-------------------------------------------------------------------------*/

void setConnectionType (const VgmuxChannelNumber entity,
                         const ConnectionClass_t connectionClass)
{
  CrManagerContext_t  *crManagerContext_p;

  FatalAssert (entity < CI_MAX_ENTITIES);

  if (isEntityActive ((VgmuxChannelNumber)entity) == TRUE)
  {
    crManagerContext_p = ptrToCrManagerContext (entity);

    if (crManagerContext_p != PNULL)
    {
      crManagerContext_p->connectionTypeMade = connectionClass;
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        getConnectionType
 *
 * Parameters:      entity     - mux channel
 *
 * Returns:         Nothing
 *
 * Description:     gets the current connection type for the specified entity
 *
 *-------------------------------------------------------------------------*/

ConnectionClass_t getConnectionType (const VgmuxChannelNumber entity)
{
  CrManagerContext_t *crManagerContext_p;
  ConnectionClass_t  ret = CONNECTION_TERMINATOR;

  FatalAssert (entity < CI_MAX_ENTITIES);

  if (isEntityActive ((VgmuxChannelNumber)entity) == TRUE)
  {
    crManagerContext_p = ptrToCrManagerContext (entity);

    if (crManagerContext_p != PNULL)
    {
      ret = ptrToCrManagerContext (entity)->connectionTypeMade;

    }
  }
  return (ret);
}

/*--------------------------------------------------------------------------
 *
 * Function:        setTaskInitiated
 *
 * Parameters:      entity        - mux channel
 *                  taskInitiated - whether command task initiated
 *
 * Returns:         Nothing
 *
 * Description:     sets whether the command was task initiated for the
 *                  specified entity
 *
 *-------------------------------------------------------------------------*/

void setTaskInitiated (const VgmuxChannelNumber entity,
                         const Boolean taskInitiated)
{
  CrManagerContext_t  *crManagerContext_p;

  FatalAssert (entity < CI_MAX_ENTITIES);

  if (isEntityActive ((VgmuxChannelNumber)entity) == TRUE)
  {
    crManagerContext_p = ptrToCrManagerContext (entity);

    if (crManagerContext_p != PNULL)
    {
      crManagerContext_p->taskInitiated = taskInitiated;
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        getTaskInitiated
 *
 * Parameters:      entity     - mux channel
 *
 * Returns:         Nothing
 *
 * Description:     returns whether command was task initiated for the entity
 *
 *-------------------------------------------------------------------------*/

Boolean getTaskInitiated (const VgmuxChannelNumber entity)
{
  CrManagerContext_t  *crManagerContext_p;
  Boolean             ret = FALSE;

  FatalAssert (entity < CI_MAX_ENTITIES);

  if (isEntityActive ((VgmuxChannelNumber)entity) == TRUE)
  {
    crManagerContext_p = ptrToCrManagerContext (entity);

    if (crManagerContext_p != PNULL)
    {
      ret = crManagerContext_p->taskInitiated;
    }
  }
  return (ret);
}


/*--------------------------------------------------------------------------
 *
 * Function:        getCirmDataBuffer
 *
 * Parameters:      entity     - mux channel
 *
 * Returns:         Nothing
 *
 * Description:     gets the current CirmDataBuffer pointer.
 *
 *-------------------------------------------------------------------------*/
CirmDataInd *getCirmDataBuffer( const VgmuxChannelNumber  entity)
{
  CirmDataInd         *data_p = PNULL;
  CrManagerContext_t  *crManagerContext_p = ptrToCrManagerContext(entity);
  SignalBuffer        signal;

  FatalAssert (entity < CI_MAX_ENTITIES);

  if (crManagerContext_p != PNULL)
  {
    signal = crManagerContext_p->crOutputStreamCtrl.signal;

    if (!((signal.type == kiNullBuffer.type) ||
        (signal.sig == kiNullBuffer.sig)))
    {
      data_p = (CirmDataInd *)ptrToCrManagerContext(entity)->crOutputStreamCtrl.signal.sig;
    }
  }
  return(data_p);
}


/*--------------------------------------------------------------------------
 *
 * Function:        resetCirmDataBuffer
 *
 * Parameters:      entity     - mux channel
 *
 * Returns:         Nothing
 *
 * Description:     resets the current CirmDataBuffer pointer.
 *
 *-------------------------------------------------------------------------*/
void resetCirmDataBuffer ( const VgmuxChannelNumber  entity )
{
  CrManagerContext_t  *crManagerContext_p = ptrToCrManagerContext(entity);

  if (crManagerContext_p != PNULL)
  {
    if( crManagerContext_p->crOutputStreamCtrl.signal.sig != PNULL )
    {
      KiDestroySignal ( &crManagerContext_p->crOutputStreamCtrl.signal );
    }
  }
}
/*--------------------------------------------------------------------------
 *
 * Function:        getCrConversionBuffer
 *
 * Parameters:      entity     - mux channel
 *
 * Returns:         Nothing
 *
 * Description:     gets the character set conversion buffer pointer for the
 *                  specified entity.
 *-------------------------------------------------------------------------*/
void *getCrConversionBuffer(const VgmuxChannelNumber entity)
{
  CrManagerContext_t *crManagerContext_p;
  void               *ret_p = PNULL;

  FatalAssert (entity < CI_MAX_ENTITIES);

  crManagerContext_p = ptrToCrManagerContext (entity);

  if (crManagerContext_p != PNULL)
  {
    if (crManagerContext_p->crChSetConversionBuffer_p == PNULL)
    {
      crManagerContext_p->crChSetConversionBuffer_p = vgAllocateRamToConvBuff();
    }
    ret_p = (void *)(crManagerContext_p->crChSetConversionBuffer_p);
  }
  return (ret_p);
}

/*--------------------------------------------------------------------------
 *
 * Function:        resetCrConversionBuffer
 *
 * Parameters:      entity     - mux channel
 *
 * Returns:         Nothing
 *
 * Description:     resets the character set conversion buffer pointer for the
 *                  specified entity.
 *-------------------------------------------------------------------------*/
void resetCrConversionBuffer(const VgmuxChannelNumber entity)
{
  CrManagerContext_t *crManagerContext_p;

  FatalAssert (entity < CI_MAX_ENTITIES);

  crManagerContext_p = ptrToCrManagerContext (entity);

  if (crManagerContext_p != PNULL)
  {
    if (crManagerContext_p->crChSetConversionBuffer_p != PNULL)
    {
      vgFreeRamForConvBuff (&crManagerContext_p->crChSetConversionBuffer_p);

      crManagerContext_p->crChSetConversionBuffer_p = PNULL;
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        getCrOutputBuffer
 *
 * Parameters:      entity     - mux channel
 *
 * Returns:         Nothing
 *
 * Description:     gets the character set output buffer pointer for the
 *                  specified entity.
 *-------------------------------------------------------------------------*/
Int8 *getCrOutputBuffer(const VgmuxChannelNumber entity)
{
  CrManagerContext_t *crManagerContext_p;
  Int8               *ret_p = PNULL;

  FatalAssert (entity < CI_MAX_ENTITIES);

  crManagerContext_p = ptrToCrManagerContext (entity);

  if (crManagerContext_p != PNULL)
  {
    if (crManagerContext_p->crChSetOutputBuffer_p == PNULL)
    {
      crManagerContext_p->crChSetOutputBuffer_p = vgAllocateRamToOutputBuff();
    }
    ret_p = (Int8 *)(crManagerContext_p->crChSetOutputBuffer_p);
  }
  return (ret_p);
}

/*--------------------------------------------------------------------------
 *
 * Function:        resetCrOutputBuffer
 *
 * Parameters:      entity     - mux channel
 *
 * Returns:         Nothing
 *
 * Description:     resets the character set conversion buffer pointer for the
 *                  specified entity.
 *-------------------------------------------------------------------------*/
void resetCrOutputBuffer(const VgmuxChannelNumber entity)
{
  CrManagerContext_t *crManagerContext_p;

  FatalAssert (entity < CI_MAX_ENTITIES);

  crManagerContext_p = ptrToCrManagerContext (entity);

  if (crManagerContext_p != PNULL)
  {
    if (crManagerContext_p->crChSetOutputBuffer_p != PNULL)
    {
      vgFreeRamForOutputBuff(&crManagerContext_p->crChSetOutputBuffer_p);

      crManagerContext_p->crChSetOutputBuffer_p = PNULL;
    }
  }
}

#if defined (FEA_PHONEBOOK)
/*--------------------------------------------------------------------------
 *
 * Function:        getCrAlphaStringBuffer
 *
 * Parameters:      entity     - mux channel
 *
 * Returns:         Nothing
 *
 * Description:     gets the character set alpha string pointer for the
 *                  specified entity.
 *-------------------------------------------------------------------------*/
Int8 *getCrAlphaStringBuffer(const VgmuxChannelNumber entity)
{
  CrManagerContext_t *crManagerContext_p;
  Int8               *ret_p = PNULL;

  FatalAssert (entity < CI_MAX_ENTITIES);

  crManagerContext_p = ptrToCrManagerContext (entity);

  if (crManagerContext_p != PNULL)
  {
    if (crManagerContext_p->crAlphaStringBuffer_p == PNULL)
    {
      crManagerContext_p->crAlphaStringBuffer_p = vgAllocateRamToAlphaBuff();
    }
    ret_p = (Int8 *)(crManagerContext_p->crAlphaStringBuffer_p);
  }
  return (ret_p);
}

/*--------------------------------------------------------------------------
 *
 * Function:        resetCrAlphaStringBuffer
 *
 * Parameters:      entity     - mux channel
 *
 * Returns:         Nothing
 *
 * Description:     resets the character set conversion buffer pointer for the
 *                  specified entity.
 *-------------------------------------------------------------------------*/
void resetCrAlphaStringBuffer(const VgmuxChannelNumber entity)
{
  CrManagerContext_t *crManagerContext_p;

  FatalAssert (entity < CI_MAX_ENTITIES);

  crManagerContext_p = ptrToCrManagerContext (entity);

  if (crManagerContext_p != PNULL)
  {
    if (crManagerContext_p->crAlphaStringBuffer_p != PNULL)
    {
      vgFreeRamForAlphaBuff(&crManagerContext_p->crAlphaStringBuffer_p);

      crManagerContext_p->crAlphaStringBuffer_p = PNULL;
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:        getCrAlphaSearchBuffer
 *
 * Parameters:      entity     - mux channel
 *
 * Returns:         Nothing
 *
 * Description:     gets the character set alpha search pointer for the
 *                  specified entity.
 *-------------------------------------------------------------------------*/
Int8 *getCrAlphaSearchBuffer(const VgmuxChannelNumber entity)
{
  CrManagerContext_t *crManagerContext_p;
  Int8               *ret_p = PNULL;

  FatalAssert (entity < CI_MAX_ENTITIES);

  crManagerContext_p = ptrToCrManagerContext (entity);

  if (crManagerContext_p != PNULL)
  {
    if (crManagerContext_p->crAlphaSearchBuffer_p == PNULL)
    {
      crManagerContext_p->crAlphaSearchBuffer_p = vgAllocateRamToAlphaSearchBuff();
    }
    ret_p = (Int8 *)(crManagerContext_p->crAlphaSearchBuffer_p);
  }
  return(ret_p);
}

/*--------------------------------------------------------------------------
 *
 * Function:        resetCrAlphaSearchBuffer
 *
 * Parameters:      entity     - mux channel
 *
 * Returns:         Nothing
 *
 * Description:     resets the character set conversion buffer pointer for the
 *                  specified entity.
 *-------------------------------------------------------------------------*/
void resetCrAlphaSearchBuffer(const VgmuxChannelNumber entity)
{
  CrManagerContext_t *crManagerContext_p;

  FatalAssert (entity < CI_MAX_ENTITIES);

  crManagerContext_p = ptrToCrManagerContext (entity);

  if (crManagerContext_p != PNULL)
  {
    if (crManagerContext_p->crAlphaSearchBuffer_p != PNULL)
    {
      vgFreeRamForAlphaSearchBuff(&crManagerContext_p->crAlphaSearchBuffer_p);

      crManagerContext_p->crAlphaSearchBuffer_p = PNULL;
    }
  }
}
#endif /* FEA_PHONEBOOK*/

/*--------------------------------------------------------------------------
 *
 * Function:        checkForAdditionalPassword
 *
 * Parameters:      commandBuffer_p - commandline info
 *                  otherPassword   - password to compare with
 *
 * Returns:         Boolean - whether additional password check failed
 *
 * Description:     compares additional password (if supplied) with
 *                  password given
 *-------------------------------------------------------------------------*/

Boolean checkForAdditionalPassword (CommandLine_t *commandBuffer_p,
                                     Char *otherPassword,
                                      Int16 *otherPasswordLength)
{
  Boolean result = TRUE;
  Int16   length;
  Char    password[SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH] = {0};

  FatalAssert (commandBuffer_p->position > 0);

  if (commandBuffer_p->character[commandBuffer_p->position - 1] == COMMA_CHAR)
  {
    if (commandBuffer_p->character[commandBuffer_p->position] == QUOTES_CHAR)
    {
      /* get additional password */
      if (getExtendedString (commandBuffer_p,
                              &password[0],
                               SIM_CHV_LENGTH,
                                &length) == TRUE)
      {
        /* compare passwords */
        if ((length > 0) &&
            (*otherPasswordLength == length) &&
            (memcmp (&password[0], &otherPassword[0], *otherPasswordLength) == 0))
        {
          /* use first password entered */
        }
        else
        {
          /* use second password entered */

          memcpy (&otherPassword[0], &password[0], length + NULL_TERMINATOR_LENGTH);
          *otherPasswordLength = length;
        }
      }
    }
    else
    {
      result = FALSE;
    }
  }

  return (result);
}

/* Job 109119 - prevent alpha passwords being assigned via AT */
/*--------------------------------------------------------------------------
 *
 * Function:      checkForNumericOnlyChars
 *
 * Parameters:    password   - password to check
 *
 * Returns:       Boolean - whether passwords contain alpha chars
 *
 * Description:   checks passwords given for alpha characters
 *                Since passwords are small the use of std library calls
 *                should not pose a speed issue.
 *
 *-------------------------------------------------------------------------*/

Boolean checkForNumericOnlyChars(const Char *password)
{
  Int16   passLen = 0;
  Int8    count = 0;
  Boolean result = TRUE;

  FatalAssert (strlen((const char*)password) <= SIM_CHV_LENGTH);

  passLen = (Int16)strlen((const char*)password);
  if (passLen > 0)
  {
    /* Parse each digit. Since password is small this technique shouldn't
       be to burdensome. */
    while ((result == TRUE) && (count < passLen))
    {
      if ((password[count] < '0') || (password[count] > '9'))
      {
        result = FALSE;
      }

      count++;
    }
  }

  return (result);
}
/* End of Job 109119 */

/*--------------------------------------------------------------------------
 *
 * Function:    vgCheckTextBcdString
 *
 * Parameters:  (In) asciiDialNum_p - a pointer to an array of Char
 *              containing a zero terminated ASCII string containing text
 *              vdial digits for checking.
 *              (In) asciiLen - Int8 indicating the length of the ascii
 *              array
 *
 * Returns:     Boolean which is TRUE if asciiDialNum_p is valid, FALSE
 *              otherwise.
 *
 * Description: Checks a string contains only valid dialstring characters
 *              for conversion to BCD.
 *-------------------------------------------------------------------------*/

Boolean vgCheckTextBcdString (const Char *asciiDialNum_p, Int16 asciiLen)
{
  Int16         pos = 0;
  Boolean       validString = TRUE;

  /* Check string contains only valid characters in the correct
   * position....
   */
  while ((pos < asciiLen) && (validString == TRUE))
  {
    switch (asciiDialNum_p[pos])
    {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case STAR_CHAR:
      case HASH_CHAR:
      case 'a':
      case 'A':
      case 'b':
      case 'B':
      case 'c':
      case 'C':
      case 'p':
      case 'P':
      case 'w':
      case 'W':
      {
        break;
      }
      case CHR_PLUS:
      {
        if (pos != 0)
        {
          validString = FALSE;
        }
        break;
      }
      default:
      {
        validString = FALSE;
        break;
      }
    }
    pos++;
  }

  return (validString);
}

/*--------------------------------------------------------------------------
 *
 * Function:    sendVerifyPin2Request
 *
 * Parameters:  entity - Mux channel number
 *
 * Returns:     result code.
 *
 * Description: Depending upon the type of card (GSM SIM or UICC) inserted,
 *              sends the correct request to verify PIN2.
 *-------------------------------------------------------------------------*/

ResultCode_t sendVerifyPin2Request (const VgmuxChannelNumber  entity)
{
  ResultCode_t            result;
  GeneralContext_t        *generalContext_p        = ptrToGeneralContext (entity);
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
  VgSimInfo               *simInfo                 = &simLockGenericContext_p->simInfo;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

#endif

  if (simInfo->cardIsUicc == TRUE)
  {
    generalContext_p->keyRef = simInfo->pin2KeyRef;
    result = vgChManContinueAction (entity, SIG_APEX_SIM_PIN_FUNCTION_REQ);
  }
  else
  {
    generalContext_p->chvNumber = SIM_CHV_2;
    result = vgChManContinueAction (entity, SIG_APEX_SIM_CHV_FUNCTION_REQ);
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStriNCmp
 *
 * Parameters:  string1 - a pointer to an array of Char
 *              string2 - a pointer to an array of Char
 *              num     - number of characters to compare
 *
 * Returns:     0 if strings are the same.  Non zero otherwise.
 *
 * Description: Carries out a case-insensitive comparison of two strings
 *-------------------------------------------------------------------------*/
Int32 vgStriNCmp (const Char *string1, const Char *string2, Int32 num)
{
#if defined (ENABLE_LONG_AT_CMD_RSP)
  Char tmpString1[AT_MEDIUM_BUFF_SIZE + NULL_TERMINATOR_LENGTH] = {0};
  Char tmpString2[AT_MEDIUM_BUFF_SIZE + NULL_TERMINATOR_LENGTH] = {0};
#else
  Char tmpString1[CIMUX_MAX_AT_DATA_LENGTH + NULL_TERMINATOR_LENGTH] = {0};
  Char tmpString2[CIMUX_MAX_AT_DATA_LENGTH + NULL_TERMINATOR_LENGTH] = {0};
#endif  

  vgConvertStringToUpper (string1, tmpString1);
  vgConvertStringToUpper (string2, tmpString2);

  return vgStrNCmp (tmpString1, tmpString2, num);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStrWildNCmp
 *
 * Parameters:  string1 - a pointer to an array of Char including possible
 *                        wildcard characters '*' or '?'.
 *                        '*' a string of 0-n characters of any value
 *                        '?' a string of 1 character of any value
 *              string2 - a pointer to an array of Char
 *              num     - number of characters to compare
 *
 * Returns:     0 if two strings compared are the same with wildcards in string1.
 *              1 otherwise.
 *
 * Description: Carries out a comparison of two strings where string1 can
 *              contains wildcards '*' and '?'.
 *              The string check is also done case insensitively.
 *-------------------------------------------------------------------------*/
Int32 vgStrWildNCmp (const Char *string1, const Char *string2, Int32 num)
{
  Int32      string1Index             = 0;
  Int32      string2Index             = 0;
  Boolean    compareComplete          = FALSE;
  Boolean    stringsMatched           = TRUE;
  Boolean    starCharFound            = FALSE;
  Boolean    queryCharFound           = FALSE;
  Int32      numQueryChars            = 0;
  Boolean    string1PostStarCharPoint = 0;
  Boolean    string2PostStarCharPoint = 0;
  Int16      string1Length            = (Int16)strlen((char *)string1);
  Int16      string2Length            = (Int16)strlen((char *)string2);

  /*lint -save -e661 Lint doesn't understand the code so disable the out of bounds warning */
  
  /* string1 has the wildcard, string2 is a standard string */
  /* Carry on until we have got the end of the strings or
   * we have hit the string length limit
   */
  while (!(compareComplete) && (string1Index < num) && (string2Index < num) &&
         (string2[string2Index] != NULL_CHAR))
  {
    starCharFound  = FALSE;
    queryCharFound = FALSE;
    numQueryChars  = 0;

    /* String 1 flying off the end is a special case - because we have to check
     * if we have a '*' string match fail previously and we may have to go back
     */
    if (string1[string1Index] == NULL_CHAR)
    {  
      if (string1PostStarCharPoint > 0)
      {
        /* Start the search again from the next point in string2 */
        starCharFound = TRUE;
        string1Index = string1PostStarCharPoint;
        string2Index = string2PostStarCharPoint+1;
        
      }
      else
      {
        /* We overshot the end of string 1 - but string 2 was still going.
         * So the compare failed.
         */
        compareComplete = TRUE;
        stringsMatched = FALSE;
      }
    }
    else if (string1[string1Index] == STAR_CHAR)
    {
      starCharFound = TRUE; 
      string1Index++;
    }
    else if (string1[string1Index] == QUERY_CHAR)
    {
      queryCharFound = TRUE;
      numQueryChars++;
      string1Index++;
    }
    else if (toupper(string1[string1Index]) != toupper(string2[string2Index]))
    {
      /* Check if we had a '*' character previously - if we did then
       * we will need to rewind both the string1Index and string2Index
       * to the point just after when the '*' character was first found.
       */
      if (string1PostStarCharPoint > 0)
      {
        /* We must have hit a star character previously.
         * Put string1 index back to where it was and move string2 index on 1
         * Also make it look like we got a star character again.
         */
        starCharFound = TRUE;
        string1Index = string1PostStarCharPoint;
        string2Index = string2PostStarCharPoint+1;
      }
      else
      {
        /* Strings don't match so stop now */
        compareComplete = TRUE;
        stringsMatched = FALSE;
      }
    }
    else
    {
      /* Strings must be the same - so move the pointers along 1 */
      string1Index++;
      string2Index++;
    }

    if ((starCharFound) || (queryCharFound))
    {
      /* Move along until we find the end of a string of * or ? chars */
      while ((string1Index < num) && (string1[string1Index] != NULL_CHAR) &&
             ((string1[string1Index] == STAR_CHAR) || (string1[string1Index] == QUERY_CHAR)))
      {
        if (string1[string1Index] == STAR_CHAR)
        {
          starCharFound = TRUE;
        }
        else if (string1[string1Index] == QUERY_CHAR)
        {
          /* We found another query char in a string of them - no star chars */
          numQueryChars++;
        }
        string1Index++;
      }
 
      if (((string2Index + numQueryChars) <= num) &&
          ((string2Index + numQueryChars) > string2Length))
      {
        /* The number of query chars caused us to fly off the end of the
         * string 2 but was within the compare limit for string 2 - so this means the
         * match is not OK.
         */
        stringsMatched = FALSE;
        compareComplete = TRUE;

      }
      else if (((string2Index + numQueryChars) > num) ||
               ((string2Index + numQueryChars) == string2Length))
      {
        /* We got to the exact end of string 2, or overshot the compare limit
         * with query chars - so the compare is OK
         */
        stringsMatched = TRUE;
        compareComplete = TRUE;
      }
      else if (((string1Index >= num) || (string1[string1Index] == NULL_CHAR)) &&
           (starCharFound))
      {
        /* We got to the end of the string and all we got were * or ? characters.
         * So this implicitly means that we must have got the match OK, but
         * only if there were * chars in the string of ?'s and *'s and
         * there were not too many ? chars.
         */
        /* There was a star char in the string so the match was OK */
        stringsMatched = TRUE;
        compareComplete = TRUE;
      }
      else
      {
        /* We hit something other than a * or ? character in string1 */
        if (starCharFound)
        {
          string1PostStarCharPoint = string1Index;
          string2PostStarCharPoint = string2Index;
          /* We found a star character or more with maybe some query
           * chars in there too.
           */
          /* We need to move along the string2 until we match whatever
           * comes after the * character in string 1 */
          while ((string2Index < num) && (string2Index < string2Length)
                  && ((toupper(string2[string2Index])) != (toupper(string1[string1Index]))))
          {
            string2Index++;
          }
          
          if ((string2Index >= num) || (string2Index >= string2Length))
          {
            /* String 2 got to the end of the compare limit or the end
             * of the string - so we are done and the compare was not OK
             * because there are some chars left in string1 which are not ?
             * or *
             */
            stringsMatched = FALSE;
            compareComplete = TRUE;
          }
        }
        else if (queryCharFound)
        {
          /* Only found 1 or more query char - so move along string2
           * by the number of query chars
           */
          string2Index += numQueryChars;
        }
      }
    }
  }  

  /* Catch special cases */
  if (!compareComplete)
  {
    if ((string1Index < num) && (string2Index < num))
    {
      if ((string1Index >= string1Length) && (string2Index < string2Length))
      {
        /* We flew off the end of string 1, but string 2 was still going - so this
         * is a match fail
         */
        stringsMatched = FALSE;
      }
      else if ((string2Index >= string2Length) && (string1Index < string1Length)
                && !((string1Index == string1Length-1) && (string1[string1Index] == STAR_CHAR)))
      {
        /* we flew off the end of string 2, but string 1 was still going - the 
         * '*' character case for this is handled within the loop.
         * We need to check however, if the last character in the string1 was a *
         * in which case this is still OK.
         */
        stringsMatched = FALSE;
      }
    }  
  }

  /*lint -restore */

  return (!stringsMatched);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgConvert2CharsIntoInt8
 *
 * Parameters:  string, defaultValue
 *
 * Returns:     Int8
 *
 * Description: Converts a two-char string into Int8.
 *
 *-------------------------------------------------------------------------*/

Int8 vgConvert2CharsIntoInt8 ( Int8* string, Int8 defaultValue)
{
  Int8 i;
  Int8 result = 0;
  Boolean error = FALSE;

  for (i=0; (i<2)&&(!error); i++)
  {
    switch(string[i])
    {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        result = result*16 + (string[i] - '0');
        break;

      case 'a':
      case 'A':
        result = result*16 + 10;
        break;

      case 'b':
      case 'B':
        result = result*16 + 11;
        break;

      case 'c':
      case 'C':
        result = result*16 + 12;
        break;

      case 'd':
      case 'D':
        result = result*16 + 13;
        break;

      case 'e':
      case 'E':
        result = result*16 + 14;
        break;

      case 'f':
      case 'F':
        result = result*16 + 15;
        break;

      default:
        result = defaultValue;
        error = TRUE;
        break;
    }
  }

  return result;
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgGetMmiUnsolicitedChannel
 *
 * Parameters:
 *
 * Returns:     VgmuxChannelNumber
 *
 * Description: Returns channel designated to be MMI Unsolicited channel.
 *
 *-------------------------------------------------------------------------*/
VgmuxChannelNumber vgGetMmiUnsolicitedChannel ( )
{
  VgmuxChannelNumber  profileEntity;
  VgmuxChannelNumber  retEntity = VGMUX_CHANNEL_INVALID;

  for ( profileEntity=0; profileEntity<CI_MAX_ENTITIES; profileEntity++)
  {
    if( isEntityActive(profileEntity) &&
        getProfileValueBit( profileEntity, PROF_MROUTEMMI, PROF_BIT_MROUTEMMI_MMI_UNSOL))
    {
      retEntity = profileEntity;
    }
  }

  return retEntity;
}

void writeHslString (const char *string) /**< [in] pointer to string to log */
{
#if defined (ENABLE_ATCI_STR_HSL_LOGGING)
    Int16 stringLen = 0;
#if defined (FR_PLATFORM_IS_BIG_ENDIAN)
    /* These platforms require the byte order to be swapped for endianess issues */
    Int16 buffer[(CIMUX_MAX_AT_DATA_LENGTH+16)/2+1];
    Int8 *buf_p;
    Int8 tmp;
    Int32 i;

    stringLen = strlen(string);

    /* Ensure the string length is not too long */
    if (stringLen >= (CIMUX_MAX_AT_DATA_LENGTH+16))
    {
        stringLen = (CIMUX_MAX_AT_DATA_LENGTH+16) -1;
    }

    memset (buffer, 0, sizeof(buffer));
    memcpy (buffer, string, stringLen);

    buf_p = (Int8 *)buffer;

    /* Ensure all bytes are swapped if there are an odd number */
    stringLen = (stringLen + 1) / 2;

    /* Byte swap */
    for (i=0; i < stringLen; i++)
    {
        tmp = *buf_p;
        *buf_p = *(buf_p+1);
        *(buf_p+1) = tmp;
        buf_p+=2;
    }

    /* Log the data, plus ensure log a null terminator */
#else
    stringLen = (strlen(string) + 3) / 2;
    PARAMETER_NOT_USED (stringLen);
#endif
#else /* ENABLE_ATCI_STR_HSL_LOGGING */
    PARAMETER_NOT_USED(string);
#endif /* else ENABLE_ATCI_STR_HSL_LOGGING */
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSetCirmDataIndForceTransmit
 *
 * Parameters:  VgmuxChannelNumber
 *
 * Returns:     Nothing.
 *
 * Description: Sets forceTransmit field in outgoing CimDataInd for entity.
 *
 *-------------------------------------------------------------------------*/
void vgSetCirmDataIndForceTransmit(const VgmuxChannelNumber entity)
{
  CirmDataInd            *cirmDataInd_p        = PNULL;

  cirmDataInd_p = getCirmDataBuffer(entity);

  if (cirmDataInd_p != PNULL)
  {
    cirmDataInd_p->forceTransmit = TRUE;

    /* If we are forcing transmit then this cannot be a URC */
    cirmDataInd_p->isUrc = FALSE;
    cirmDataInd_p->isSmsUrc = FALSE;
  }  
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSetCirmDataIndIsUrc
 *
 * Parameters:  VgmuxChannelNumber
 *
 * Returns:     Nothing.
 *
 * Description: Sets isUrc field in outgoing CimDataInd for entity.
 *
 *-------------------------------------------------------------------------*/
void vgSetCirmDataIndIsUrc(const VgmuxChannelNumber entity,
                         Boolean isUrc)
{
  CirmDataInd            *cirmDataInd_p        = PNULL;

  cirmDataInd_p = getCirmDataBuffer(entity);

  if (cirmDataInd_p != PNULL)
  {
    cirmDataInd_p->isUrc = isUrc;
  }  
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgGetCirmDataIndIsUrc
 *
 * Parameters:  VgmuxChannelNumber
 *
 * Returns:     Nothing.
 *
 * Description: Gets isUrc field in outgoing CimDataInd for entity.
 *
 *-------------------------------------------------------------------------*/
Boolean vgGetCirmDataIndIsUrc(const VgmuxChannelNumber entity)
{
  CirmDataInd            *cirmDataInd_p        = PNULL;
  Boolean                retVal                = FALSE;

  cirmDataInd_p = getCirmDataBuffer(entity);

  if (cirmDataInd_p != PNULL)
  {
    retVal = cirmDataInd_p->isUrc;
  }  

  return (retVal);
}

/* END OF FILE */



