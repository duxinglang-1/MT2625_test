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
 * This module is responsible for all the decoding of the indications and
 * setting up the data string.  The data will be sent to the accessory
 * when the *MSTCR AT command is recived from the accessory or sent
 * unsolicited.
 **************************************************************************/

#define MODULE_NAME "RVSTKRD"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#if !defined (SYSTEM_H)
#  include <system.h>
#endif

#include <rvsystem.h>

#if !defined (RVSTKRD_H)
#  include <rvstkrd.h>
#endif
#if ! defined (RVSTKRNAT_H)
#  include <rvstkrnat.h>
#endif
#if !defined (RVUTIL_H)
#  include <rvutil.h>
#endif
#if !defined (SITS_TYP_H)
#  include <sits_type.h>
#endif
#if !defined (AFSA_SIG_H)
#  include <afsa_sig.h>
#endif
#if !defined (ABST_SIG_H)
#  include <abst_sig.h>
#endif
#if !defined (KERNEL_H)
#  include <kernel.h>
#endif
#if !defined (RVDATA_H)
#  include <rvdata.h>
#endif
#if !defined (RVCIMUX_H)
#  include <rvcimux.h>
#endif
#if !defined (UTBITFNC_H)
#  include <utbitfnc.h>
#endif
#include <rvcimxut.h>
#include <rvgnsigi.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
   AfsaDisplayTextInd           afsaDisplayTextInd;
   AfsaRefreshInd               afsaRefreshInd;
   AfsaSelectItemInd            afsaSelectItemInd;
   AfsaSetUpMenuInd             afsaSetUpMenuInd;
   ApexStDisplayAlphaIdInd      apexStDisplayAlphaIdInd;
   AfsaGetInkeyInd              afsaGetInkeyInd;
   AfsaGetInputInd              afsaGetInputInd;
   AfsaPlayToneInd              afsaPlayToneInd;
   ApexStSetUpUssdInd           apexStSetUpUssdInd;
   ApexStCallSetupGetAckInd     apexStCallSetupGetAckInd;
   AfsaSetUpEventListInd        afsaSetUpEventListInd;
   AfsaRunAtCommandInd          afsaRunAtCommandInd;
   AfsaSetUpIdleModeTextInd     afsaSetUpIdleModeTextInd;
   AfsaLaunchBrowserInd         afsaLaunchBrowserInd;
   AfsaLanguageNotificationInd  afsaLanguageNotificationInd;
};

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static Char* vgStkCopyString (Char* writePoint_p,
                              const Char* stringToCopy_p,
                              Int16 stringLength,
                              Int16 bufferIndex);
static void vgStkEraseMenuStore (void);
static void vgStkPopulateMenuStore (const SimatItem menu []);
static void vgStkCreateMenuStore (const Int8 menuItems, const SimatItem menu []);

static void vgStkCreateBuffer (const Int8 lineToAllocate);
static void vgGetDcsParameter (const MsgCoding dcsValue,
                                Char  **writePoint,
                                 Boolean  *result);

static void vgWriteTextParameters (Char **writePoint,
                                   SimatTextAttributes textAttributes);

static void vgStkUpdateBufferLength (Int16 index, Char* point);

static void vgStkResizeBuffer (const Int8 lineToAllocate);


static Boolean vgSigAfsaDecodeDisplayTextInd (const SignalBuffer signalEntity,
                                               const VgmuxChannelNumber entity);
static Boolean vgAfsaRefreshInd              (const SignalBuffer signalEntity,
                                               const VgmuxChannelNumber entity);
static Boolean vgSigAfsaDecodeSelectItemInd  (const SignalBuffer signalEntity,
                                               const VgmuxChannelNumber entity);
static Boolean vgSigAfsaDecodeSetUpMenuInd   (const SignalBuffer signalEntity,
                                               const VgmuxChannelNumber entity);
static Boolean vgApexStDisplayAlphaIdInd     (const SignalBuffer signalEntity,
                                               const VgmuxChannelNumber entity);
static Boolean vgSigAfsaDecodeInkeyInd       (const SignalBuffer signalEntity,
                                               const VgmuxChannelNumber entity);
static Boolean vgSigAfsaDecodeGetInputInd    (const SignalBuffer signalEntity,
                                               const VgmuxChannelNumber entity);
static Boolean vgSigAfsaDecodeToneInd        (const SignalBuffer signalEntity,
                                               const VgmuxChannelNumber entity);
static Boolean vgAfsaSetUpEventListInd       (const SignalBuffer signalEntity,
                                               const VgmuxChannelNumber entity);
static Boolean vgAfsaSetUpIdleModeTextInd    (const SignalBuffer signalEntity,
                                               const VgmuxChannelNumber entity);
static Boolean vgAfsaLaunchBrowserInd        (const SignalBuffer signalEntity,
                                               const VgmuxChannelNumber entity);
static Boolean vgAfsaLanguageNotificationInd (const SignalBuffer signalEntity,
                                               const VgmuxChannelNumber entity);

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

typedef Boolean (*StkDecodeProcFunc)(const SignalBuffer signalBuffer,
                                      const VgmuxChannelNumber entity);

struct StkDecodeSignalEntityTag
{
   SignalId           signalType;
   StkDecodeProcFunc  stkDecodeProcFunc;
};
typedef struct StkDecodeSignalEntityTag StkDecodeSignalEntityControl;

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* The following is appropriate because *MSTGC are only produced in response */
/* to the *MSTGC command not in other circumstances                          */
#define MSTGC_PROMPT "*MSTGC: "
/* the following is approppriate because STUD is an unsolicited notification */
#define MSTUD_PROMPT "*MSTUD: "

/* mapping of IND signal to give its, hex string and the decode routine to
   setup the data string */

static const StkDecodeSignalEntityControl stkDecodeSignalEntityTable[] =
{
  /* commands that need a response */
  {SIG_AFSA_DISPLAY_TEXT_IND,          vgSigAfsaDecodeDisplayTextInd},
  {SIG_AFSA_REFRESH_IND,               vgAfsaRefreshInd},
  {SIG_AFSA_SELECT_ITEM_IND,           vgSigAfsaDecodeSelectItemInd},
  {SIG_AFSA_SET_UP_MENU_IND,           vgSigAfsaDecodeSetUpMenuInd},
  {SIG_APEX_ST_DISPLAY_ALPHA_ID_IND,   vgApexStDisplayAlphaIdInd},
  {SIG_AFSA_GET_INKEY_IND,             vgSigAfsaDecodeInkeyInd},
  {SIG_AFSA_GET_INPUT_IND,             vgSigAfsaDecodeGetInputInd},
  {SIG_AFSA_PLAY_TONE_IND,             vgSigAfsaDecodeToneInd},
  {SIG_AFSA_SET_UP_IDLE_MODE_TEXT_IND, vgAfsaSetUpIdleModeTextInd},
  {SIG_AFSA_SET_UP_EVENT_LIST_IND,     vgAfsaSetUpEventListInd},
  {SIG_AFSA_LAUNCH_BROWSER_IND,        vgAfsaLaunchBrowserInd},
  {SIG_AFSA_LANGUAGE_NOTIFICATION_IND, vgAfsaLanguageNotificationInd},
  {SIG_SYS_DUMMY,                      PNULL}
};

/* this structure is holds the list of menu items in the current menu.*/
struct StkMenuItemsTag
{
  struct StkMenuItemsTag  *next_p;
  Int8   menuItem;
};
typedef struct StkMenuItemsTag StkMenuItems_t;

#define STK_UPDATE_BUFFER_LENGTH(lINEnUMBER, eNDpOINTER)                             \
{                                                                                    \
  FatalAssert (stkGenericContext_p->stkCommandBuffer[lINEnUMBER] != PNULL);            \
  FatalAssert (eNDpOINTER >= stkGenericContext_p->stkCommandBuffer[lINEnUMBER]->data); \
  if ((stkGenericContext_p->stkCommandBuffer[lINEnUMBER] != PNULL) &&                \
      (eNDpOINTER >= stkGenericContext_p->stkCommandBuffer[lINEnUMBER]->data))       \
  {                                                                                  \
    stkGenericContext_p->stkCommandBuffer[lINEnUMBER]->dataLength      = (Int16)     \
    (eNDpOINTER - stkGenericContext_p->stkCommandBuffer[lINEnUMBER]->data);          \
  }                                                                                  \
}

/***************************************************************************
 * Local Variables
 ***************************************************************************/

static StkMenuItems_t*  stkMenuItemList_p = PNULL;

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkCopyString
 *
 * Parameters:  writePoint_p   - destination address
 *              stringToCopy_p - source string
 *              stringLength   - length of string to copy
 *              bufferIndex    - STK buffer copying string to
 *
 * Returns:     Char* - updated destination address
 *
 * Description: copies a string to a line of the STK buffer (checking there is
 *              sufficient memory to copy the data).
 *
 *-------------------------------------------------------------------------*/

static Char* vgStkCopyString (Char* writePoint_p,
                              const Char* stringToCopy_p,
                              Int16 stringLength,
                              Int16 bufferIndex)
{
  StkEntityGenericData_t* stkGenericContext_p = ptrToStkGenericContext ();

  FatalAssert (writePoint_p != PNULL);
  FatalAssert (stringToCopy_p != PNULL);
  FatalAssert (stkGenericContext_p->stkCommandBuffer[bufferIndex] != PNULL);

  if ((writePoint_p != PNULL) &&
      (stringToCopy_p != PNULL) &&
      (stkGenericContext_p->stkCommandBuffer[bufferIndex] != PNULL))
  {
    /* check writePoint points to part of the STK buffer */
    FatalAssert (writePoint_p >= stkGenericContext_p->stkCommandBuffer[bufferIndex]->data);
    FatalAssert ((writePoint_p + stringLength) <= stkGenericContext_p->stkCommandBuffer[bufferIndex]->data + MAX_DATA_MESSAGE_SIZE);

    /* copy the string (if the space is available) */
    if ((writePoint_p >= stkGenericContext_p->stkCommandBuffer[bufferIndex]->data) &&
        ((writePoint_p + stringLength) <= stkGenericContext_p->stkCommandBuffer[bufferIndex]->data + MAX_DATA_MESSAGE_SIZE))
    {
      memcpy ((char*)writePoint_p, (char*)stringToCopy_p, stringLength);
      writePoint_p += stringLength;
    }
  }

  return (writePoint_p);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkEraseMenuStore
 *
 * Parameters:  Nothing
 *
 * Returns:     nothing
 *
 * Description: de-allocates RAM from the menu list
 *
 *-------------------------------------------------------------------------*/

static void vgStkEraseMenuStore ()
{
  StkMenuItems_t  *list_p = stkMenuItemList_p;
  StkMenuItems_t  *temp_p = stkMenuItemList_p;

  if ( list_p != PNULL )
  {
    while ( list_p->next_p != PNULL )
    {
      list_p = list_p->next_p;
      KiFreeMemory ((void **)&temp_p);
      temp_p = list_p;
    }
    KiFreeMemory ((void **)&list_p);
    stkMenuItemList_p = PNULL;
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkPopulateMenuStore
 *
 * Parameters:  menu: list of menu items
 *
 * Returns:     nothing
 *
 * Description: populates the recorded menu list given a list of menu items
 * Assumption here is that the menu list is all ready created using the
 * create menu function.
 *
 *-------------------------------------------------------------------------*/

static void vgStkPopulateMenuStore (const SimatItem menu [])
{
  StkMenuItems_t*  temp_p = stkMenuItemList_p;
  Int8             count = 0;

  if ( menu != PNULL )
  {
    temp_p->menuItem = menu[count++].itemId;
    while ( temp_p->next_p != PNULL )
    {
      temp_p = temp_p->next_p;
      temp_p->menuItem = menu[count++].itemId;
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkCreateMenuStore
 *
 * Parameters:  menuItems: the number of menu items
 *              menu: list of menu items from the SIM
 *
 * Returns:     nothing
 *
 * Description: allocates RAM for the menu list
 *
 *-------------------------------------------------------------------------*/

static void vgStkCreateMenuStore (const Int8 menuItems, const SimatItem menu [])
{

  StkMenuItems_t  *temp_p = PNULL;
  Int8            menuItemsCount = 0;

  /* creating a new menu so erase the existing menu items */
  vgStkEraseMenuStore ();

  if ( ( menuItems > 0 ) && (stkMenuItemList_p == PNULL) )
  {
    KiAllocMemory (sizeof (StkMenuItems_t), (void **)&temp_p);

    if (PNULL != temp_p)
    {
      temp_p->menuItem = 0;
      temp_p->next_p = PNULL;
    }

    if ( menuItems > 1 )
    {
      for (menuItemsCount = 1; menuItemsCount < menuItems; menuItemsCount++)
      {
        stkMenuItemList_p = PNULL;
        KiAllocMemory (sizeof (StkMenuItems_t), (void **)&stkMenuItemList_p);

        if (PNULL != stkMenuItemList_p)
        {
          stkMenuItemList_p->menuItem = 0;
          stkMenuItemList_p->next_p = temp_p;
          temp_p = stkMenuItemList_p;
        }
      }
    }
    else
    {
      stkMenuItemList_p = temp_p;
    }

    vgStkPopulateMenuStore (menu);

  }
  else
  {
    /* No menu items to allocate or menu store is null */
    WarnParam(menuItems, 0, 0);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkCreateBuffer
 *
 * Parameters:  lineToAllocate - line of response data, start from 0
 *
 * Returns:     nothing
 *
 * Description: Allocates memory for the number of lines required for
 *              storage of the response data.
 *
 *-------------------------------------------------------------------------*/

static void vgStkCreateBuffer (const Int8 lineToAllocate)
{
  StkEntityGenericData_t *stkGenericContext_p = ptrToStkGenericContext ();

  if (lineToAllocate <= SIMAT_MAX_NUM_ITEMS)
  {
      if (stkGenericContext_p->stkCommandBuffer[lineToAllocate] == PNULL)
      {
        KiAllocMemory ((Int16)sizeof(StkBuffer_t),
        (void **)&stkGenericContext_p->stkCommandBuffer[lineToAllocate]);
      }
      else
      {
        stkGenericContext_p->stkCommandBuffer[lineToAllocate]->dataLength = MAX_DATA_MESSAGE_SIZE;
        vgStkResizeBuffer (lineToAllocate);
      }

      memset (stkGenericContext_p->stkCommandBuffer[lineToAllocate]->data,
              NULL_CHAR,
              sizeof(stkGenericContext_p->stkCommandBuffer[lineToAllocate]->data));

      stkGenericContext_p->stkCommandBuffer[lineToAllocate]->dataLength = 0;
  }
  else
  {
    /* too many lines for stkCommandBuffer */
    FatalParam(lineToAllocate, 0, 0);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkResizeBuffer
 *
 * Parameters:  lineToAllocate - line of response data, start from 0
 *
 * Returns:     nothing
 *
 * Description: Resize memory for saving memory.
 *
 *-------------------------------------------------------------------------*/

static void vgStkResizeBuffer (const Int8 lineToAllocate)
{
  StkEntityGenericData_t *stkGenericContext_p = ptrToStkGenericContext ();
  Int16 newSize;

  if (lineToAllocate <= SIMAT_MAX_NUM_ITEMS)
  {
      if (stkGenericContext_p->stkCommandBuffer[lineToAllocate] != PNULL)
      {
        newSize = stkGenericContext_p->stkCommandBuffer[lineToAllocate]->dataLength + sizeof(Int16);
        KiReallocMemory (newSize,
        (void **)&stkGenericContext_p->stkCommandBuffer[lineToAllocate]);
      }
  }
  else
  {
    /* too many lines for stkCommandBuffer */
    FatalParam(lineToAllocate, 0, 0);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgGetDcsParameter
 *
 * Parameters:  dcsValue     - coding scheme
 *              writePoint   - string to output coding scheme
 *              result       - validity of coding scheme
 *
 * Returns:     nothing
 *
 * Description: displays coding scheme
 *
 *-------------------------------------------------------------------------*/

static void vgGetDcsParameter (const MsgCoding  dcsValue,
                                Char  **writePoint,
                                 Boolean  *result)
{
  *result = TRUE;

  switch (dcsValue)
  {
    case MSG_CODING_DEFAULT:
    {
      *writePoint = vgPrintNum (*writePoint, STK_DCS_DEFAULT);
      break;
    }
    case MSG_CODING_8BIT:
    {
      *writePoint = vgPrintNum (*writePoint, STK_DCS_EIGHT_BIT);
      break;
    }
    case MSG_CODING_UCS2:
    {
      *writePoint = vgPrintNum (*writePoint, STK_DCS_UCS2);
      break;
    }
    default:
    {
      *result = FALSE;
      break;
    }
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgWriteTextParameters

 * Parameters:  writePoint                - string to output text params
 *              SimatTextAttributes       - text attributes
 *
 * Returns:     nothing
 *
 * Description: string text attributes *
 *-------------------------------------------------------------------------*/

static void vgWriteTextParameters (Char **writePoint, SimatTextAttributes textAttributes)
{
  *writePoint = vgPrintNum  (*writePoint, (Int16)textAttributes.startPosition);
  *writePoint = vgPrintLine (*writePoint, (const Char*)",");
  *writePoint = vgPrintNum  (*writePoint, (Int16)textAttributes.formatingLength);
  *writePoint = vgPrintLine (*writePoint, (const Char*)",");
  *writePoint = vgPrintNum  (*writePoint, (Int16)textAttributes.alignment);
  *writePoint = vgPrintLine (*writePoint, (const Char*)",");
  *writePoint = vgPrintNum  (*writePoint, (Int16)textAttributes.fontSize);
  *writePoint = vgPrintLine (*writePoint, (const Char*)",");
  *writePoint = vgPrintNum  (*writePoint, (Int16)textAttributes.bold);
  *writePoint = vgPrintLine (*writePoint, (const Char*)",");
  *writePoint = vgPrintNum  (*writePoint, (Int16)textAttributes.italic);
  *writePoint = vgPrintLine (*writePoint, (const Char*)",");
  *writePoint = vgPrintNum  (*writePoint, (Int16)textAttributes.underline);
  *writePoint = vgPrintLine (*writePoint, (const Char*)",");
  *writePoint = vgPrintNum  (*writePoint, (Int16)textAttributes.strikeThrough);
  *writePoint = vgPrintLine (*writePoint, (const Char*)",");
  *writePoint = vgPrintNum  (*writePoint, (Int16)textAttributes.foregroundColour);
  *writePoint = vgPrintLine (*writePoint, (const Char*)",");
  *writePoint = vgPrintNum (*writePoint, (Int16)textAttributes.backgroundColour);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSigAfsaDecodeDisplayTextInd
 *
 * Parameters:  signalEntity  - structure containing signal:
 *                              SIG_AFSA_DISPLAY_TEXT_IND
 *              entity        - entity which sent request
 *
 * Returns:     Boolean - validaity of signal data
 *
 * Description: Decodes the display text ind signal and sets up the data str.
 *
 *-------------------------------------------------------------------------*/

static Boolean vgSigAfsaDecodeDisplayTextInd (const SignalBuffer signalEntity,
                                               const VgmuxChannelNumber entity)
{
  Char                    *writePoint;
  Int16                   i;   /* job115982: new max limit for 7bit packed characters */
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();
  Int16                   dataLength;
  Int8                    *packedBuffer_p = PNULL;

  stkGenericContext_p->simCommandId = signalEntity.sig->afsaDisplayTextInd.simatCommandRef;

  /* we can assume that the data is valid to start off with */
  stkGenericContext_p->dataValid = TRUE;
  /* allocate some memory for the sting */
  vgStkCreateBuffer (0);

  /* header */
  writePoint = vgPrintLine (stkGenericContext_p->stkCommandBuffer[0]->data,
                            (const Char*)MSTGC_PROMPT);
  writePoint = vgPrintNum (writePoint, stkGenericContext_p->cmdId);
  writePoint = vgPrintLine (writePoint, (const Char*)",");
  /* dcs */
  vgGetDcsParameter (signalEntity.sig->afsaDisplayTextInd.textString.
                       codingScheme.msgCoding,
                     &writePoint,
                       &stkGenericContext_p->dataValid);
  if (stkGenericContext_p->dataValid == TRUE)
  {
    /* text */
    writePoint = vgPrintLine (writePoint, (const Char*)",");

    writePoint = vgPrintLine (writePoint, (const Char*)"\"");
    if (vgStkOutputInTextMode (entity))
    {
      writePoint = vgStkCopyString (writePoint,
        signalEntity.sig->afsaDisplayTextInd.textString.textString,
        signalEntity.sig->afsaDisplayTextInd.textString.length,
        0);
    }
    else
    {
      if ( signalEntity.sig->afsaDisplayTextInd.textString.codingScheme.msgCoding == MSG_CODING_DEFAULT)
      {
        KiAllocZeroMemory(  sizeof(Int8)*(MAX_DATA_MESSAGE_SIZE),
                            (void **) &packedBuffer_p);

        /* 7-bit decode data, so we need to re-encode it and then convert into HEX 7-bit */
        dataLength = UtEncode7BitPackedData(&packedBuffer_p[0],
                                             signalEntity.sig->afsaDisplayTextInd.textString.length,
                                              &signalEntity.sig->afsaDisplayTextInd.textString.textString[0],
                                               MAX_DATA_MESSAGE_SIZE);
        for (i=0; i < dataLength; i++)
        {
          writePoint = vgOp8BitHex (packedBuffer_p[i], writePoint);
        }

        KiFreeMemory( (void**)&packedBuffer_p);
      }
      else
      {
        for (i=0; i < signalEntity.sig->afsaDisplayTextInd.textString.length; i++)
        {
          writePoint = vgOp8BitHex(signalEntity.sig->afsaDisplayTextInd.
                                     textString.textString[i], writePoint);
        }
      }
    }
    writePoint = vgPrintLine (writePoint, (const Char*)"\",");
    /* priority */
    writePoint = vgPrintNum (writePoint,
                              signalEntity.sig->afsaDisplayTextInd.commandQual.
                               dispHighPriorityReq);
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    /* clear */
    writePoint = vgPrintNum (writePoint,
                              signalEntity.sig->afsaDisplayTextInd.commandQual.
                               userClearMessage);
    /* icon Id and disp mode are optional */
    if (signalEntity.sig->afsaDisplayTextInd.iconIdPresent == TRUE)
    {
      writePoint = vgPrintLine (writePoint, (const Char*)",");
      /* icon id */
      writePoint = vgPrintNum (writePoint,
                                (Int16)signalEntity.sig->afsaDisplayTextInd.
                                  iconId.recordNum);
      writePoint = vgPrintLine (writePoint, (const Char*)",");
      /* disp Mode */
      writePoint = vgPrintNum (writePoint,
                                (Int16) (signalEntity.sig->afsaDisplayTextInd.
                                 iconId.displayIconOnly == TRUE?0:1));
    }
    else
    {
       /* need to put a comma for the lack of the  previous icon info */
       writePoint = vgPrintLine (writePoint, (const Char*)",");
       /* need to put a comma for the lack of the  previous disp Mode info */
       writePoint = vgPrintLine (writePoint, (const Char*)",");
    }
    /* immediate response ? */
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    writePoint = vgPrintNum (writePoint,
                                (Int16) (signalEntity.sig->afsaDisplayTextInd.
                                 immediateResponse == TRUE?1:0));

    if (signalEntity.sig->afsaDisplayTextInd.durationPresent == TRUE)
    {
      writePoint = vgPrintLine (writePoint, (const Char*)",");
      writePoint = vgPrintNum (writePoint,
                   (Int16)signalEntity.sig->afsaDisplayTextInd.durationUnit);
      writePoint = vgPrintLine (writePoint, (const Char*)",");
      writePoint = vgPrintNum (writePoint,
                   (Int16)signalEntity.sig->afsaDisplayTextInd.durationValue);
    }

    if (signalEntity.sig->afsaDisplayTextInd.textAttsPresent == TRUE)
    {
      if ( signalEntity.sig->afsaDisplayTextInd.durationPresent == FALSE)
      {
        /* need to put a comma for the lack of the  previous duration unit info */
        writePoint = vgPrintLine (writePoint, (const Char*)",");
        /* need to put a comma for the lack of the  previous duration value info */
        writePoint = vgPrintLine (writePoint, (const Char*)",");
      }
      writePoint = vgPrintLine (writePoint, (const Char*)",");
      vgWriteTextParameters (&writePoint, signalEntity.sig->afsaDisplayTextInd.textAttributes);
    }
  }
  /* calculate length of stk buffer line */
  vgStkUpdateBufferLength (0, writePoint);

  return (stkGenericContext_p->dataValid);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgAfsaRefreshInd
 *
 * Parameters:  signalEntity  - structure containing signal:
 *                              SIG_AFSA_REFRESH_IND
 *              entity        - mux channel number
 *
 * Returns:     Boolean - validaity of signal data
 *
 * Description: received a refresh indication and we need to decode the
 *              signal set up the local data string.  The indication requires
 *              an unsolicited response to be sent to the accessory.
 *              The MMI will report whether the Refresh can be carried out
 *               or whether the screen is busy
 *
 *-------------------------------------------------------------------------*/

static Boolean vgAfsaRefreshInd (const SignalBuffer signalEntity,
                                  const VgmuxChannelNumber entity)
{
  Char                    *writePoint;
  Int8                    allFiles;
  Int16                   fileId;
  SimUiccPath            *filePath_p = PNULL;
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  stkGenericContext_p->dataValid = TRUE;

  stkGenericContext_p->simCommandId = signalEntity.sig->afsaRefreshInd.simatCommandRef;

  vgStkCreateBuffer (0);

  writePoint = vgPrintLine (stkGenericContext_p->stkCommandBuffer[0]->data,
                            (const Char*)MSTUD_PROMPT);

  /* cmd id for refresh is 1 so hex notation 01 */
  if (stkGenericContext_p->cmdId <= 9)
  {
    writePoint = vgPrintNum (writePoint, 0);
    writePoint = vgPrintNum (writePoint, stkGenericContext_p->cmdId);
  }
  else
  {
    writePoint = vgPrintNum (writePoint,
                              stkGenericContext_p->cmdId);
  }

  /* commandQual */
  writePoint = vgPrintLine (writePoint, (const Char*)",");
  writePoint = vgPrintNum (writePoint, (Int16) signalEntity.sig ->
                           afsaRefreshInd.commandQual);

  /* num of files */
  writePoint = vgPrintLine (writePoint, (const Char*)",");
  writePoint = vgPrintNum (writePoint, signalEntity.sig ->
                           afsaRefreshInd.fileInfo.numOfFiles);

  /* file list */
  if (signalEntity.sig -> afsaRefreshInd.fileInfo.numOfFiles > 0)
  {
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    writePoint = vgPrintLine (writePoint, (const Char*)"\"");
    if (vgStkOutputInTextMode (entity))
    {
      for (allFiles = 0; allFiles < signalEntity.sig -> afsaRefreshInd.
            fileInfo.numOfFiles; allFiles++)
      {
           if (allFiles != 0)
           {
              writePoint = vgPrintLine (writePoint, (const Char*)",");
           }
           filePath_p = &signalEntity.sig -> afsaRefreshInd.fileInfo.fileList[allFiles];
           FatalCheck(filePath_p->length >= 2, filePath_p->length, 0, 0);
           /* extract the file ID */
           fileId =  (Int16) (((Int16)(filePath_p->data[filePath_p->length - 2] <<8)) |
                               filePath_p->data[filePath_p->length - 1]);

           writePoint = vgOp32BitHex ((Int32)fileId, 2, writePoint);
      }
    }
    else
    {
      for (allFiles = 0; allFiles < signalEntity.sig -> afsaRefreshInd.
            fileInfo.numOfFiles; allFiles++)
      {
        if (allFiles != 0)
        {
          writePoint = vgOp8BitHex (0x2C, writePoint);
        }

        filePath_p = &signalEntity.sig -> afsaRefreshInd.fileInfo.fileList[allFiles];
        FatalCheck(filePath_p->length >= 2, filePath_p->length, 0, 0);

        /* extract the file ID */
        fileId = (Int16) (((Int16)(filePath_p->data[filePath_p->length - 2] <<8)) |
                                   filePath_p->data[filePath_p->length - 1]);

        writePoint = vgOp32BitHex ((Int32)fileId, 2, writePoint);
      }
    }
    writePoint = vgPrintLine (writePoint, (const Char*)"\"");
  }

  /* calculate length of stk buffer line */
  vgStkUpdateBufferLength (0, writePoint);

  /* now send the unsolicited result */
  if (stkGenericContext_p->dataValid == TRUE)
  {
    if(isEntityMmiNotUnsolicited(entity))
    {
      sendDataWithLengthToCrm (
        stkGenericContext_p->stkCommandBuffer[0]->data,
        stkGenericContext_p->stkCommandBuffer[0]->dataLength, TRUE, TRUE,  vgGetMmiUnsolicitedChannel());
    }
    else
    {
      sendDataWithLengthToCrm (
        stkGenericContext_p->stkCommandBuffer[0]->data,
        stkGenericContext_p->stkCommandBuffer[0]->dataLength, TRUE, TRUE, entity);
    }
  }

  return (stkGenericContext_p->dataValid);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigAfsaDecodeSelectItemInd
 *
 * Parameters:  signalEntity  - structure containing signal:
 *                              SIG_AFSA_DECODE_SELECT_ITEM_IND
 *              entity        - mux channel number
 *
 * Returns:     Boolean - validaity of signal data
 *
 * Description: received a set up call indication and we need to decode the
 *              signal set up the local data string.  The indication requires
 *              an unsolicited response to be sent to the accessory.
 *
 *-------------------------------------------------------------------------*/

static Boolean vgSigAfsaDecodeSelectItemInd (const SignalBuffer signalEntity,
                                              const VgmuxChannelNumber entity)
{
  Char                    *writePoint;
  Int8                    menuItems;
  Int8                    i;
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  /* we can assume that the data is valid to start off with */
  stkGenericContext_p->dataValid = TRUE;

  stkGenericContext_p->simCommandId = signalEntity.sig->afsaSelectItemInd.simatCommandRef;

  /* allocate some memory for the sting */
  vgStkCreateBuffer (0);

  /* header */
  writePoint = vgPrintLine (stkGenericContext_p->stkCommandBuffer[0]->data,
                            (const Char*)MSTGC_PROMPT);
  writePoint = vgPrintNum (writePoint, stkGenericContext_p->cmdId);
  writePoint = vgPrintLine (writePoint, (const Char*)",");
  /* num items */
  writePoint = vgPrintNum (writePoint, signalEntity.sig->afsaSelectItemInd.
  itemList.numItems);
  writePoint = vgPrintLine (writePoint, (const Char*)",");
  /* default item id */
  if ( signalEntity.sig->afsaSelectItemInd.defaultItemIdPresent )
  {
    writePoint = vgPrintNum (writePoint, signalEntity.sig->afsaSelectItemInd.
    defaultItemId);
  }
  writePoint = vgPrintLine (writePoint, (const Char*)",");
  /* selection */
  writePoint = vgPrintNum (writePoint, signalEntity.sig->afsaSelectItemInd.
  commandQual.softKeySelectionPreferred);
  writePoint = vgPrintLine (writePoint, (const Char*)",");
  /* help info */
  writePoint = vgPrintNum (writePoint, (Int16) (signalEntity.sig ->
  afsaSelectItemInd.commandQual.helpInfoAvailable == TRUE?1:0));
  /* alpha id */

  if (signalEntity.sig->afsaSelectItemInd.alphaIdPresent == TRUE)
  {
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    writePoint = vgPrintLine (writePoint, (const Char*)"\"");
    if (vgStkOutputInTextMode (entity))
    {
      writePoint = vgStkCopyString (writePoint,
        signalEntity.sig->afsaSelectItemInd.alphaId.data,
        signalEntity.sig->afsaSelectItemInd.alphaId.length,
        0);
    }
    else
    {
      for (i=0;
            i < signalEntity.sig->afsaSelectItemInd.alphaId.length;
             i++)
      {
        writePoint = vgOp8BitHex(signalEntity.sig->afsaSelectItemInd.
                                  alphaId.data[i], writePoint);
      }
    }
    writePoint = vgPrintLine (writePoint, (const Char*)"\"");
  }

 /* icon and disp */
  if (signalEntity.sig->afsaSelectItemInd.iconIdPresent == TRUE)
  {
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    /* icon id */
    writePoint = vgPrintNum (writePoint,
                             (Int16)signalEntity.sig->afsaSelectItemInd.
                              iconId.recordNum);
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    /* disp Mode */
    writePoint = vgPrintNum (writePoint,
                             (Int16) (signalEntity.sig->afsaSelectItemInd.
                               iconId.displayIconOnly == TRUE?0:1));
  }
  if (signalEntity.sig->afsaSelectItemInd.textAttsPresent == TRUE)
  {
    if (signalEntity.sig->afsaSelectItemInd.iconIdPresent == FALSE)
    {
      /* need to put a comma for the lack of the  previous icon info */
      writePoint = vgPrintLine (writePoint, (const Char*)",");
      /* need to put a comma for the lack of the  previous disp Mode info */
      writePoint = vgPrintLine (writePoint, (const Char*)",");
    }
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    vgWriteTextParameters (&writePoint, signalEntity.sig->afsaSelectItemInd.textAttributes);
  }
  
  writePoint = vgPrintLine (writePoint, (const Char*)"\r\n");

  /* calculate length of stk buffer line */
  vgStkUpdateBufferLength (0, writePoint);

  /* item id and item text */
  for (menuItems = 0;
        ((menuItems < signalEntity.sig->afsaSelectItemInd.itemList.numItems) &&
         (stkGenericContext_p->dataValid == TRUE));
         menuItems++)
  {
    vgStkCreateBuffer ((Int8) (menuItems + 1));

    /* header - write to a new line in the buffer for each item*/
    writePoint = vgPrintLine (stkGenericContext_p->stkCommandBuffer[menuItems + 1]->data,
                              (const Char*)MSTGC_PROMPT);
    /* item id */
    writePoint = vgPrintNum (writePoint,
                              signalEntity.sig->afsaSelectItemInd.itemList.
                               itemData [menuItems].itemId);
    writePoint = vgPrintLine (writePoint, (const Char*)",");

    /* item text */
    writePoint = vgPrintLine (writePoint, (const Char*)"\"");
    if (vgStkOutputInTextMode (entity))
    {
      writePoint = vgStkCopyString (writePoint,
        signalEntity.sig->afsaSelectItemInd.itemList.itemData [menuItems].itemText,
        signalEntity.sig->afsaSelectItemInd.itemList.itemData [menuItems].length,
        menuItems + 1);
    }
    else
    {
      for (i=0;
            i < signalEntity.sig->afsaSelectItemInd.itemList.
                   itemData [menuItems].length;
             i++)
      {
        writePoint = vgOp8BitHex(signalEntity.sig->afsaSelectItemInd.itemList.
         itemData [menuItems].itemText[i], writePoint);
      }
    }
    writePoint = vgPrintLine (writePoint, (const Char*)"\"");

    /* icon id and disp mode */
    if (signalEntity.sig->afsaSelectItemInd.itemList.iconListPresent == TRUE)
    {
      writePoint = vgPrintLine (writePoint, (const Char*)",");
      /* icon id */
      writePoint = vgPrintNum (writePoint,
                                (Int16)signalEntity.sig->afsaSelectItemInd.
                                  itemList.itemData [menuItems].iconIdRecordNum);
      writePoint = vgPrintLine (writePoint, (const Char*)",");
      /* disp mode */
      writePoint = vgPrintNum (writePoint,
                                (Int16) (signalEntity.sig->afsaSelectItemInd.itemList.
                                 displayIconOnly == TRUE?0:1));
    }

    if (signalEntity.sig->afsaSelectItemInd.naiPresent == TRUE)
    {
       writePoint = vgPrintLine (writePoint, (const Char*)",");
       if (signalEntity.sig->afsaSelectItemInd.itemList.iconListPresent == FALSE)
       {
         /* need to put a comma for the lack of the  previous icon info */
         writePoint = vgPrintLine (writePoint, (const Char*)",");
         /* need to put a comma for the lack of the  previous disp Mode info */
         writePoint = vgPrintLine (writePoint, (const Char*)",");
       }
       if ( menuItems <signalEntity.sig->afsaSelectItemInd.nai.numNai )
       {
         writePoint = vgOp8BitHex(signalEntity.sig->afsaSelectItemInd.nai.nai[menuItems],
                                     writePoint);
       }
       else
       {
         writePoint = vgPrintLine (writePoint, (const Char*)",");
       }
    }
    if (signalEntity.sig->afsaSelectItemInd.itemList.textAttsListPresent == TRUE)
    {
      if (signalEntity.sig->afsaSelectItemInd.naiPresent == FALSE)
      {
        writePoint = vgPrintLine (writePoint, (const Char*)",");
        /* if both Nai and icon list are not present and textattributes are present
         * we need to put commas for both the empty entries */
        if (signalEntity.sig->afsaSelectItemInd.itemList.iconListPresent == FALSE)
        {
          /* need to put a comma for the lack of the  previous icon info */
          writePoint = vgPrintLine (writePoint, (const Char*)",");
          /* need to put a comma for the lack of the  previous disp Mode info */
          writePoint = vgPrintLine (writePoint, (const Char*)",");
        }
      }

      writePoint = vgPrintLine (writePoint, (const Char*)",");
      vgWriteTextParameters (&writePoint,
            signalEntity.sig->afsaSelectItemInd.itemList.itemData[menuItems].textAttributes);
    }
    
    writePoint = vgPrintLine (writePoint, (const Char*)"\r\n");

    /* calculate length of stk buffer line */
    vgStkUpdateBufferLength (menuItems + 1, writePoint);
  }

  return (stkGenericContext_p->dataValid);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgSigAfsaDecodeSetUpMenuInd
 *
 * Parameters:  signalEntity  - structure containing signal:
 *                              SIG_APEX_ST_SET+UP_SS_IND
 *              entity        - mux channel number
 *
 * Returns:     Boolean - validaity of signal data
 *
 * Description: decodes the set up menu indication and sets up the data str.
 *
 *-------------------------------------------------------------------------*/

static Boolean vgSigAfsaDecodeSetUpMenuInd (const SignalBuffer signalEntity,
                                             const VgmuxChannelNumber entity)
{
  Char                    *writePoint;
  Int8                    menuItems;
  Int8                    i;
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  /* we can assume that the data is valid to start off with */
  stkGenericContext_p->dataValid = TRUE;

  /* set up some globals for the *MSTMS command to use */
  stkGenericContext_p->currentMenuItems = signalEntity.sig->afsaSetUpMenuInd.menuItems.numMenuItems;
  stkGenericContext_p->menuHasBeenRemoved = signalEntity.sig->afsaSetUpMenuInd.menuItems.removeExistingMenu;
  /* job102163: keep record of first menu item id for *MSTMS=? range check */
  stkGenericContext_p->firstMenuItemId = signalEntity.sig->afsaSetUpMenuInd.menuItems.itemData[0].itemId;

  stkGenericContext_p->simCommandId = signalEntity.sig->afsaSetUpMenuInd.simatCommandRef;

  /* job120397: remove existing menu, so erase store here as we don't need to allocate any RAM */
  if (stkGenericContext_p->menuHasBeenRemoved == TRUE)
  {
    /* SIM requests removal of SAT menu so erase any existing menu items */
    vgStkEraseMenuStore ();
  }
  else
  {
    vgStkCreateMenuStore (signalEntity.sig->afsaSetUpMenuInd.menuItems.numMenuItems,
                          signalEntity.sig->afsaSetUpMenuInd.menuItems.itemData);
  }

  /* allocate some memory for the sting */
  vgStkCreateBuffer (0);

  /* header */
  writePoint = vgPrintLine (stkGenericContext_p->stkCommandBuffer[0]->data,
                            (const Char*)MSTGC_PROMPT);
  writePoint = vgPrintNum (writePoint, stkGenericContext_p->cmdId);
  writePoint = vgPrintLine (writePoint, (const Char*)",");
  /* num items */
  writePoint = vgPrintNum (writePoint, signalEntity.sig->afsaSetUpMenuInd.
  menuItems.numMenuItems);
  writePoint = vgPrintLine (writePoint, (const Char*)",");
  /* selection */
  writePoint = vgPrintNum (writePoint,
                           signalEntity.sig->afsaSetUpMenuInd.commandQual.
                           softKeySelectionPreferred);
  writePoint = vgPrintLine (writePoint, (const Char*)",");
  /* remove menu */
  writePoint = vgPrintNum (writePoint,
                           (Int16) (signalEntity.sig->afsaSetUpMenuInd.
                            menuItems.removeExistingMenu == TRUE?1:0));
  writePoint = vgPrintLine (writePoint, (const Char*)",");
  /* help info */
  writePoint = vgPrintNum (writePoint, (Int16) (signalEntity.sig ->
  afsaSetUpMenuInd.commandQual.helpInfoAvailable == TRUE?1:0));

  /* Other data items will not be present if menu has been removed */
  if (stkGenericContext_p->menuHasBeenRemoved != TRUE)
  {
    writePoint = vgPrintLine (writePoint, (const Char*)",");

    /* alpha id */
    if (signalEntity.sig->afsaSetUpMenuInd.alphaId.length > 0)
    {
      /* Need to check length field is correct such that alphaId string does
      ** not contain 'padding' 0xFF bytes - some SIMs (well known to us!) code
      ** the length field incorrectly. We cannot just truncate the alphaId as
      ** it can contain UCS2 coding (as per GSM 11.11, Annex B) with 0xFF as a
      ** valid byte of a character code.
      ** Function already exists in PS to do this check/adjustment.
      */
  /*     SimGetAdnAlphaIdLength(signalEntity.sig->afsaSetUpMenuInd.alphaId.data,
                              &signalEntity.sig->afsaSetUpMenuInd.alphaId.length);    NEEDS UPDATING   */

      writePoint = vgPrintLine (writePoint, (const Char*)"\"");
      if (vgStkOutputInTextMode (entity))
      {
        writePoint = vgStkCopyString (writePoint,
          signalEntity.sig->afsaSetUpMenuInd.alphaId.data,
          signalEntity.sig->afsaSetUpMenuInd.alphaId.length,
          0);
      }
      else
      {
        for (i=0;
              i < (Int8)(signalEntity.sig->afsaSetUpMenuInd.alphaId.length);
               i++)
        {
          writePoint = vgOp8BitHex(signalEntity.sig->afsaSetUpMenuInd.
                                     alphaId.data[i], writePoint);
        }
      }
      writePoint = vgPrintLine (writePoint, (const Char*)"\"");
    }
    else
    {
      stkGenericContext_p->dataValid = FALSE;
    }
    /* optional bit */
    if (stkGenericContext_p->dataValid == TRUE)
    {
      if (signalEntity.sig->afsaSetUpMenuInd.iconIdPresent == TRUE)
      {
        writePoint = vgPrintLine (writePoint, (const Char*)",");
        /* icon id */
        writePoint = vgPrintNum (writePoint,
                                  (Int16)signalEntity.sig->
                                    afsaSetUpMenuInd.iconId.recordNum);
        writePoint = vgPrintLine (writePoint, (const Char*)",");
        /* disp Mode */
        writePoint = vgPrintNum (writePoint,
                                  (Int16) (signalEntity.sig->afsaSetUpMenuInd.
                                   iconId.displayIconOnly == TRUE?0:1));
      }

      if (signalEntity.sig->afsaSetUpMenuInd.titleTextAttsPresent == TRUE)
      {
        if (signalEntity.sig->afsaSetUpMenuInd.iconIdPresent == FALSE)
        {
          /* need to put a comma for the lack of the  previous icon info */
          writePoint = vgPrintLine (writePoint, (const Char*)",");
          /* need to put a comma for the lack of the  previous disp Mode info */
          writePoint = vgPrintLine (writePoint, (const Char*)",");
        }
        writePoint = vgPrintLine (writePoint, (const Char*)",");
        vgWriteTextParameters (&writePoint, signalEntity.sig->afsaSetUpMenuInd.titleTextAttributes);
      }
    }

    writePoint = vgPrintLine (writePoint, (const Char*)"\r\n");
    
    /* calculate length of stk buffer line */
    vgStkUpdateBufferLength (0, writePoint);

    /* item id and item text */
    for (menuItems = 0; ((menuItems < signalEntity.sig->afsaSetUpMenuInd.
          menuItems.numMenuItems) && (stkGenericContext_p->dataValid == TRUE)); menuItems++)
    {
      vgStkCreateBuffer ((Int8) (menuItems + 1));

      /* header - write to a new line in the buffer for each item */
      writePoint = vgPrintLine (stkGenericContext_p->stkCommandBuffer[menuItems + 1]->data,
                                (const Char*)MSTGC_PROMPT);
      /* item id */
      writePoint = vgPrintNum (writePoint,
                                signalEntity.sig->afsaSetUpMenuInd.menuItems.
                                 itemData [menuItems].itemId);
      /* job102163: check for lowest menu item id for *MSTMS=? range check as
       * may not be in ascending order. Essential for TCs 27.22.4.8/27.22.5.3 */
      if (stkGenericContext_p->firstMenuItemId > signalEntity.sig->afsaSetUpMenuInd.menuItems.itemData[menuItems].itemId)
      {
        stkGenericContext_p->firstMenuItemId = signalEntity.sig->afsaSetUpMenuInd.menuItems.itemData[menuItems].itemId;
      }

      /* item text */
      writePoint = vgPrintLine (writePoint, (const Char*)",");
      writePoint = vgPrintLine (writePoint, (const Char*)"\"");
      if (vgStkOutputInTextMode (entity))
      {
         writePoint = vgStkCopyString (writePoint,
           signalEntity.sig->afsaSetUpMenuInd.menuItems.itemData[menuItems].itemText,
           signalEntity.sig->afsaSetUpMenuInd.menuItems.itemData[menuItems].length,
           menuItems + 1);
      }
      else
      {
        for (i=0;
               i < signalEntity.sig->afsaSetUpMenuInd.menuItems.
                 itemData [menuItems].length;
                i++)
         {
            writePoint = vgOp8BitHex(signalEntity.sig->afsaSetUpMenuInd.menuItems.
                                      itemData [menuItems].itemText[i], writePoint);
         }

      }
      writePoint = vgPrintLine (writePoint, (const Char*)"\"");


      /* icon id and disp mode */
      if (signalEntity.sig->afsaSetUpMenuInd.menuItems.iconListPresent == TRUE)
      {
         writePoint = vgPrintLine (writePoint, (const Char*)",");
         /* icon id */
         writePoint = vgPrintNum (writePoint,
                                   (Int16)signalEntity.sig->afsaSetUpMenuInd.
                                     menuItems.itemData [menuItems].iconIdRecordNum);
         writePoint = vgPrintLine (writePoint, (const Char*)",");
         /* disp mode */
         writePoint = vgPrintNum (writePoint,
                                   (Int16) (signalEntity.sig->afsaSetUpMenuInd.menuItems.
                                    displayIconOnly == TRUE?0:1));
      }

      if (signalEntity.sig->afsaSetUpMenuInd.naiPresent == TRUE)
      {

         writePoint = vgPrintLine (writePoint, (const Char*)",");
         if (signalEntity.sig->afsaSetUpMenuInd.menuItems.iconListPresent == FALSE)
         {
            /* need to put a comma for the lack of the  previous icon info */
            writePoint = vgPrintLine (writePoint, (const Char*)",");
            /* need to put a comma for the lack of the  previous disp Mode info */
            writePoint = vgPrintLine (writePoint, (const Char*)",");
         }

         if ( menuItems <signalEntity.sig->afsaSetUpMenuInd.nai.numNai )
         {
           writePoint = vgOp8BitHex(signalEntity.sig->afsaSetUpMenuInd.nai.nai[menuItems],
                                     writePoint);
         }
         else
         {
            writePoint = vgPrintLine (writePoint, (const Char*)",");
         }
      }

      if (signalEntity.sig->afsaSetUpMenuInd.menuItems.textAttsListPresent == TRUE)
      {
         if (signalEntity.sig->afsaSetUpMenuInd.naiPresent == FALSE)
         {
            writePoint = vgPrintLine (writePoint, (const Char*)",");
            /* if both Nai and icon list are not present and textattributes are present
             * we need to put commas for both the empty entries */
            if (signalEntity.sig->afsaSetUpMenuInd.menuItems.iconListPresent == FALSE)
            {
               /* need to put a comma for the lack of the  previous icon info */
               writePoint = vgPrintLine (writePoint, (const Char*)",");
               /* need to put a comma for the lack of the  previous disp Mode info */
               writePoint = vgPrintLine (writePoint, (const Char*)",");
            }
         }
         writePoint = vgPrintLine (writePoint, (const Char*)",");
         vgWriteTextParameters (&writePoint,
                signalEntity.sig->afsaSetUpMenuInd.menuItems.itemData [menuItems].textAttributes);
      }
      writePoint = vgPrintLine (writePoint, (const Char*)"\r\n");
      /* calculate length of stk buffer line */
      vgStkUpdateBufferLength (menuItems + 1, writePoint);
    }
  }
  else
  {
    writePoint = vgPrintLine (writePoint, (const Char*)"\r\n");
    /* job120397: SAT Menu has been removed.
     * Some header info still present so calculate length of stk buffer line */
    vgStkUpdateBufferLength (0, writePoint);
  }

  return (stkGenericContext_p->dataValid);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgApexStDisplayAlphaIdInd
 *
 * Parameters:  signalEntity  - structure containing signal:
 *                              SIG_APEX_ST_DISPLAY_APLHA_ID_IND
 *              entity        - mux channel number
 *
 * Returns:     Boolean - validaity of signal data
 *
 * Description: Signal used to display info to the user like ME is
 *              sending DTMF or ME is about to open a channel.  The cmd id needs
 *              to be alterted to relect the different commands packed into this
 *              signal.
 *
 *-------------------------------------------------------------------------*/

static Boolean vgApexStDisplayAlphaIdInd (const SignalBuffer signalEntity,
                                           const VgmuxChannelNumber entity)
{
  Char                    *writePoint;
  ApexStDisplayAlphaIdInd signalData = signalEntity.sig->apexStDisplayAlphaIdInd;
  Int8                    i;
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  /* we can assume that the data is valid to start off with */
  stkGenericContext_p->dataValid = TRUE;


  /* allocate some memory for the string */
  vgStkCreateBuffer (0);
   if ((signalData.type == SIMAT_OP_ALPHA_ID ) ||(signalData.type == SIMAT_SD_ALPHA_ID)
   ||  (signalData.type == SIMAT_SE_ALPHA_ID)  ||  (signalData.type == SIMAT_RE_ALPHA_ID))
   {
        writePoint = vgPrintLine (stkGenericContext_p->stkCommandBuffer[0]->data,
                            (const Char*)MSTGC_PROMPT);
   }
   else
   {
       writePoint = vgPrintLine (stkGenericContext_p->stkCommandBuffer[0]->data,
                            (const Char*)MSTUD_PROMPT);
   }
  writePoint = vgPrintNum (writePoint, stkGenericContext_p->cmdId);

  /* alpha id */
  /* job 137614 MSTUD 10 needs to indicate what the call phase is */
  if (( signalData.type == SIMAT_CC_ALPHA_ID) ||
      ( signalData.type == SIMAT_CC_SECOND_ALPHA_ID))
  {
     writePoint = vgPrintLine (writePoint, (const Char*)",");
     if ( signalData.type == SIMAT_CC_ALPHA_ID)
     {
          writePoint = vgPrintNum (writePoint, 0);
     }
     else
     {
          writePoint = vgPrintNum (writePoint, 1);
     }
  }
  writePoint = vgPrintLine (writePoint, (const Char*)",");
  if (signalData.alphaIdPresent)
  {
    if (signalData.alphaId.length != 0) /* not NULL alpha ID */
    {
      writePoint = vgPrintLine (writePoint, (const Char*)"\"");
      if (vgStkOutputInTextMode (entity))
      {
        writePoint = vgStkCopyString (writePoint,
                                      signalData.alphaId.data,
                                      signalData.alphaId.length,
                                      0);
      }
      else
      {
        for (i=0;
              i < signalData.alphaId.length;
               i++)
        {
          writePoint = vgOp8BitHex(signalData.alphaId.data[i], writePoint);
        }
      }
      writePoint = vgPrintLine (writePoint, (const Char*)"\"");
    }
    else
    {
      writePoint = vgPrintLine (writePoint, (const Char*)"\"\"");
    }
  }
  /* icon id */
  if (signalData.iconIdPresent == TRUE)
  {
     writePoint = vgPrintLine (writePoint, (const Char*)",");
     /* icon id */
     writePoint = vgPrintNum (writePoint,
                               (Int16)signalData.iconId.recordNum);
     writePoint = vgPrintLine (writePoint, (const Char*)",");
     /* disp Mode */
     writePoint = vgPrintNum (writePoint,
                               (Int16) (signalData.iconId.
                                displayIconOnly == TRUE?0:1));
  }
   /* text attributes */
  if ( signalData.textAttsPresent)
  {
     if (signalData.iconIdPresent == FALSE)
     {
       /* need to put a comma for the lack of the  previous icon info */
       writePoint = vgPrintLine (writePoint, (const Char*)",");
       /* need to put a comma for the lack of the  previous disp Mode info */
       writePoint = vgPrintLine (writePoint, (const Char*)",");
     }
     writePoint = vgPrintLine (writePoint, (const Char*)",");
     vgWriteTextParameters (&writePoint, signalData.textAttributes);
  }


      /* calculate length of stk buffer line */
  vgStkUpdateBufferLength (0, writePoint);

  /* simat op and sd are protocol requests so no need to send unsol req */
  if ((signalData.type != SIMAT_OP_ALPHA_ID) &&
      (signalData.type != SIMAT_SD_ALPHA_ID) &&
      (signalData.type != SIMAT_SE_ALPHA_ID) &&
      (signalData.type != SIMAT_RE_ALPHA_ID))
  {
    /* now send the unsolicited result, alpha id, etc are optional */
    if (stkGenericContext_p->dataValid == TRUE)
    {
      if(isEntityMmiNotUnsolicited(entity))
      {
        sendDataWithLengthToCrm (
          stkGenericContext_p->stkCommandBuffer[0]->data,
          stkGenericContext_p->stkCommandBuffer[0]->dataLength, TRUE, TRUE, vgGetMmiUnsolicitedChannel());
      }
      else
      {
        sendDataWithLengthToCrm (
          stkGenericContext_p->stkCommandBuffer[0]->data,
          stkGenericContext_p->stkCommandBuffer[0]->dataLength, TRUE, TRUE, entity);
      }
    }
  }

  return (stkGenericContext_p->dataValid);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigAfsaDecodeInkeyInd
 *
 * Parameters:  signalEntity  - structure containing signal:
 *                              SIG_AFSA_DECODE_INKEY_IND
 *              entity        - mux channel number
 *
 * Returns:     Boolean - validaity of signal data
 *
 * Description: Decodes the decode inkey indication and sets up the data str.
 *
 *-------------------------------------------------------------------------*/

static Boolean vgSigAfsaDecodeInkeyInd (const SignalBuffer signalEntity,
                                         const VgmuxChannelNumber entity)
{

#define DIGITS_ONLY (0)
#define SMS_ALPHA (1)
#define UCS2_ALPHA (2)
#define YN_ONLY (3)

  Char                    *writePoint;
  Int16                   i;   /* job115982: new max limit for 7bit packed characters */
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  /* we can assume that the data is valid to start off with */
  stkGenericContext_p->dataValid = TRUE;

  stkGenericContext_p->simCommandId = signalEntity.sig->afsaGetInkeyInd.simatCommandRef;

  /* allocate some memory for the sting */
  vgStkCreateBuffer (0);

  /* header */
  writePoint = vgPrintLine (stkGenericContext_p->stkCommandBuffer[0]->data,
                            (const Char*)MSTGC_PROMPT);
  writePoint = vgPrintNum (writePoint, stkGenericContext_p->cmdId);
  writePoint = vgPrintLine (writePoint, (const Char*)",");
  /* dcs */
  vgGetDcsParameter (signalEntity.sig->afsaGetInkeyInd.textString.
                       codingScheme.msgCoding,
                     &writePoint,
                       &stkGenericContext_p->dataValid);
  if (stkGenericContext_p->dataValid == TRUE)
  {
    /* text */
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    writePoint = vgPrintLine (writePoint, (const Char*)"\"");
    if (vgStkOutputInTextMode (entity))
    {
       writePoint = vgStkCopyString (writePoint,
         signalEntity.sig->afsaGetInkeyInd.textString.textString,
         signalEntity.sig->afsaGetInkeyInd.textString.length,
         0);
    }
    else
    {
       for (i=0;
             i < signalEntity.sig->afsaGetInkeyInd.textString.length;
              i++)
       {
          writePoint = vgOp8BitHex(signalEntity.sig->afsaGetInkeyInd.
                                    textString.textString[i], writePoint);
       }
    }
    writePoint = vgPrintLine (writePoint, (const Char*)"\",");
/* P4 Job # 117877 Check if Command qualify for Yes/No response is set first */
    if (signalEntity.sig->afsaGetInkeyInd.commandQual.
         yesNoResponseOnly == TRUE)
    {
       writePoint = vgPrintNum (writePoint, YN_ONLY);
    }
    else
    {
       /* response digits only */
       if (signalEntity.sig->afsaGetInkeyInd.commandQual.
                                  allowedKeys == SIMAT_INKEY_DIGITS_ONLY)
       {
          writePoint = vgPrintNum (writePoint, DIGITS_ONLY);
       } /* SMS alphabet */
       else if (signalEntity.sig->afsaGetInkeyInd.commandQual.
                                  alphabetSet == SIMAT_SMS_DEF_ALPHABET)
       {
          writePoint = vgPrintNum (writePoint, SMS_ALPHA);
       } /* ucs2 alphabet */
       else if (signalEntity.sig->afsaGetInkeyInd.commandQual.
                                  alphabetSet == SIMAT_UCS2_ALPHABET)
       {
          writePoint = vgPrintNum (writePoint, UCS2_ALPHA);
       }
       else
       {
          stkGenericContext_p->dataValid = FALSE;
       }
    }

    if (stkGenericContext_p->dataValid == TRUE)
    /* whether to display user response */
    {
       writePoint = vgPrintLine (writePoint, (const Char*)",");
       writePoint = vgPrintNum (writePoint, (Int16) (signalEntity.sig ->
       afsaGetInkeyInd.commandQual.displayUserResponse == TRUE?0:1));
    }

    /* help info */
    if (stkGenericContext_p->dataValid == TRUE)
    {
       writePoint = vgPrintLine (writePoint, (const Char*)",");
       writePoint = vgPrintNum (writePoint, (Int16) (signalEntity.sig ->
       afsaGetInkeyInd.commandQual.helpInfoAvailable == TRUE?1:0));
    }

    if (stkGenericContext_p->dataValid == TRUE)
    {
       /* icon Id and disp mode are optional */
       if (signalEntity.sig->afsaGetInkeyInd.iconIdPresent == TRUE)
       {
          writePoint = vgPrintLine (writePoint, (const Char*)",");
          /* icon id */
          writePoint = vgPrintNum (writePoint,
                              (Int16)signalEntity.sig->afsaGetInkeyInd.
                              iconId.recordNum);
          writePoint = vgPrintLine (writePoint, (const Char*)",");
          /* disp Mode */
          writePoint = vgPrintNum (writePoint,
                              (Int16) (signalEntity.sig->afsaGetInkeyInd.
                                  iconId.displayIconOnly == TRUE?0:1));
       }
    }
    if ( stkGenericContext_p->dataValid == TRUE)
    {
      if (signalEntity.sig->afsaGetInkeyInd.durationPresent == TRUE)
      {
        if (signalEntity.sig->afsaGetInkeyInd.iconIdPresent == FALSE)
        {
            /* need to put a comma for the lack of the  previous icon info */
            writePoint = vgPrintLine (writePoint, (const Char*)",");
            /* need to put a comma for the lack of the  previous disp Mode info */
            writePoint = vgPrintLine (writePoint, (const Char*)",");
        }
        writePoint = vgPrintLine (writePoint, (const Char*)",");
        writePoint = vgPrintNum (writePoint,
                     (Int16)signalEntity.sig->afsaGetInkeyInd.durationUnit);
        writePoint = vgPrintLine (writePoint, (const Char*)",");
        writePoint = vgPrintNum (writePoint,
                     (Int16)signalEntity.sig->afsaGetInkeyInd.durationValue);
      }
    }

    if ( stkGenericContext_p->dataValid == TRUE)
    {
      if (signalEntity.sig->afsaGetInkeyInd.textAttsPresent == TRUE)
      {
        if (signalEntity.sig->afsaGetInkeyInd.durationPresent == FALSE)
        {
            /* need to put a comma for the lack of the  previous duration unit info */
            writePoint = vgPrintLine (writePoint, (const Char*)",");
            /* need to put a comma for the lack of the  previous duration value info */
            writePoint = vgPrintLine (writePoint, (const Char*)",");
        }
        writePoint = vgPrintLine (writePoint, (const Char*)",");
        vgWriteTextParameters (&writePoint, signalEntity.sig->afsaGetInkeyInd.textAttributes);
      }
    }
  }

  /* calculate length of stk buffer line */
  vgStkUpdateBufferLength (0, writePoint);

  return (stkGenericContext_p->dataValid);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigAfsaDecodeGetInputInd
 *
 * Parameters:  signalEntity  - structure containing signal:
 *                              SIG_AFSA_DECODE_GET_INPUT_IND
 *              entity        - mux channel number
 *
 * Returns:     Boolean - validaity of signal data
 *
 * Description: Decodes the decode inkey indication and sets up the data str.
 *
 *-------------------------------------------------------------------------*/

static Boolean vgSigAfsaDecodeGetInputInd (const SignalBuffer signalEntity,
                                            const VgmuxChannelNumber entity)
{
#define UNPACKED_DIGITS_ONLY (1)
#define PACKED_DIGITS_ONLY (2)
#define UCS2_DIGITS_ONLY (3)
#define UNPACKED_SMS_DEFAULT (4)
#define PACKED_SMS_DEFAULT (5)
#define UCS2_APLHA (6)

  Char                    *writePoint;
  Int16                   i;   /* job115982: new max limit for 7bit packed characters */
  Int16                   dataLength;
  Int8                    *packedBuffer_p = PNULL;
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  /* we can assume that the data is valid to start off with */
  stkGenericContext_p->dataValid = TRUE;

  stkGenericContext_p->simCommandId = signalEntity.sig->afsaGetInputInd.simatCommandRef;

  /* allocate some memory for the sting */
  vgStkCreateBuffer (0);
  /* header */
  writePoint = vgPrintLine (stkGenericContext_p->stkCommandBuffer[0]->data,
                            (const Char*)MSTGC_PROMPT);
  writePoint = vgPrintNum (writePoint, stkGenericContext_p->cmdId);
  writePoint = vgPrintLine (writePoint, (const Char*)",");
  /* dcs */
  vgGetDcsParameter (signalEntity.sig->afsaGetInputInd.textString.
                       codingScheme.msgCoding,
                     &writePoint,
                       &stkGenericContext_p->dataValid);
  if (stkGenericContext_p->dataValid == TRUE)
  {
    /* text */
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    writePoint = vgPrintLine (writePoint, (const Char*)"\"");
    if (vgStkOutputInTextMode (entity))
    {
       writePoint = vgStkCopyString (writePoint,
         signalEntity.sig->afsaGetInputInd.textString.textString,
         signalEntity.sig->afsaGetInputInd.textString.length,
         0);
    }
    else
    {
      if ( signalEntity.sig->afsaGetInputInd.textString.codingScheme.msgCoding == MSG_CODING_DEFAULT)
      {
        KiAllocZeroMemory(  sizeof(Int8)*(MAX_DATA_MESSAGE_SIZE),
                            (void **) &packedBuffer_p);

        /* 7-bit decode data, so we need to re-encode it and then convert into HEX 7-bit */
        dataLength = UtEncode7BitPackedData(&packedBuffer_p[0],
                                             signalEntity.sig->afsaGetInputInd.textString.length,
                                              &signalEntity.sig->afsaGetInputInd.textString.textString[0],
                                               MAX_DATA_MESSAGE_SIZE);
        for (i=0; i < dataLength; i++)
        {
          writePoint = vgOp8BitHex (packedBuffer_p[i], writePoint);
        }

        KiFreeMemory( (void**)&packedBuffer_p);
      }
      else
      {
         for (i=0; i < signalEntity.sig->afsaGetInputInd.textString.length; i++)
         {
            writePoint = vgOp8BitHex(signalEntity.sig->afsaGetInputInd.
                                      textString.textString[i], writePoint);
         }
      }
    }
    writePoint = vgPrintLine (writePoint, (const Char*)"\"");

    /* response *
    * digits only from SMS alphabet and unpacked */
    if ((signalEntity.sig->afsaGetInputInd.commandQual.
         allowedKeys == SIMAT_INKEY_DIGITS_ONLY) &&
         (signalEntity.sig->afsaGetInputInd.commandQual.
         alphabetSet == SIMAT_SMS_DEF_ALPHABET) &&
         (signalEntity.sig->afsaGetInputInd.commandQual.
         inputInPackedFormat == FALSE))
    {
       writePoint = vgPrintLine (writePoint, (const Char*)",");
       writePoint = vgPrintNum (writePoint, UNPACKED_DIGITS_ONLY);
    } /* digits only from SMS alphabet and packed */
    else if ((signalEntity.sig->afsaGetInputInd.commandQual.
         allowedKeys == SIMAT_INKEY_DIGITS_ONLY) &&
         (signalEntity.sig->afsaGetInputInd.commandQual.
         alphabetSet == SIMAT_SMS_DEF_ALPHABET) &&
         (signalEntity.sig->afsaGetInputInd.commandQual.
         inputInPackedFormat == TRUE))
    {
       writePoint = vgPrintLine (writePoint, (const Char*)",");
       writePoint = vgPrintNum (writePoint, PACKED_DIGITS_ONLY);
    } /* digits from ucs2 */
    else if ((signalEntity.sig->afsaGetInputInd.commandQual.
         allowedKeys == SIMAT_INKEY_DIGITS_ONLY) &&
         (signalEntity.sig->afsaGetInputInd.commandQual.
         alphabetSet == SIMAT_UCS2_ALPHABET))
    {
       writePoint = vgPrintLine (writePoint, (const Char*)",");
       writePoint = vgPrintNum (writePoint, UCS2_DIGITS_ONLY);
    } /* sms alphabet and unpacked */
    else if ((signalEntity.sig->afsaGetInputInd.commandQual.
         alphabetSet == SIMAT_SMS_DEF_ALPHABET) &&
         (signalEntity.sig->afsaGetInputInd.commandQual.
         inputInPackedFormat == FALSE))
    {
       writePoint = vgPrintLine (writePoint, (const Char*)",");
       writePoint = vgPrintNum (writePoint, UNPACKED_SMS_DEFAULT);
    } /* sms alphabet and packed */
    else if ((signalEntity.sig->afsaGetInputInd.commandQual.
         alphabetSet == SIMAT_SMS_DEF_ALPHABET) &&
         (signalEntity.sig->afsaGetInputInd.commandQual.
         inputInPackedFormat == TRUE))
    {
       writePoint = vgPrintLine (writePoint, (const Char*)",");
       writePoint = vgPrintNum (writePoint, PACKED_SMS_DEFAULT);
    } /* ucs2 alphabet */
    else if (signalEntity.sig->afsaGetInputInd.commandQual.
         alphabetSet == SIMAT_UCS2_ALPHABET)
    {
       writePoint = vgPrintLine (writePoint, (const Char*)",");
       writePoint = vgPrintNum (writePoint, UCS2_APLHA);
    }
    else
    {
       stkGenericContext_p->dataValid = FALSE;
    }
    if (stkGenericContext_p->dataValid == TRUE)
    {
       /* echo */
       writePoint = vgPrintLine (writePoint, (const Char*)",");
       writePoint = vgPrintNum (writePoint, (Int16) (signalEntity.sig ->
        afsaGetInputInd.commandQual.echoUserInputToDisplay == TRUE?0:1));/*reverse the value so it is same to doc*/
       writePoint = vgPrintLine (writePoint, (const Char*)",");
       /* help info */
       writePoint = vgPrintNum (writePoint, (Int16) (signalEntity.sig ->
        afsaGetInputInd.commandQual.helpInfoAvailable == TRUE?1:0));
       writePoint = vgPrintLine (writePoint, (const Char*)",");
       /* min length */
       writePoint = vgPrintNum (writePoint, signalEntity.
        sig -> afsaGetInputInd.responseLength.respMinLength);
       writePoint = vgPrintLine (writePoint, (const Char*)",");
       stkGenericContext_p->minInputRange = signalEntity.
          sig -> afsaGetInputInd.responseLength.respMinLength;
       /* max length */
       writePoint = vgPrintNum (writePoint, signalEntity.
        sig -> afsaGetInputInd.responseLength.respMaxLength);
       stkGenericContext_p->maxInputRange = signalEntity.
          sig -> afsaGetInputInd.responseLength.respMaxLength;
       /* optional dcs */
       if (signalEntity.sig->afsaGetInputInd.defaultTextStringPresent == TRUE)
       {
          writePoint = vgPrintLine (writePoint, (const Char*)",");
          vgGetDcsParameter (signalEntity.sig->afsaGetInputInd.defaultTextString.
                               codingScheme.msgCoding,
                               &writePoint,
                               &stkGenericContext_p->dataValid);
          if (stkGenericContext_p->dataValid == TRUE)
          {
             /* text */
             writePoint = vgPrintLine (writePoint, (const Char*)",");
             writePoint = vgPrintLine (writePoint, (const Char*)"\"");
             if (vgStkOutputInTextMode (entity))
             {
                writePoint = vgStkCopyString (writePoint,
                  signalEntity.sig->afsaGetInputInd.defaultTextString.textString,
                  signalEntity.sig->afsaGetInputInd.defaultTextString.length,
                  0);
             }
             else
             {
                if ( signalEntity.sig->afsaGetInputInd.defaultTextString.codingScheme.msgCoding == MSG_CODING_DEFAULT)
                {
                  KiAllocZeroMemory(    sizeof(Int8)*(MAX_DATA_MESSAGE_SIZE),
                                        (void **) &packedBuffer_p);

                  /* 7-bit decode data, so we need to re-encode it and then convert into HEX 7-bit */
                  dataLength = UtEncode7BitPackedData(&packedBuffer_p[0],
                                                        signalEntity.sig->afsaGetInputInd.defaultTextString.length,
                                                         &signalEntity.sig->afsaGetInputInd.defaultTextString.textString[0],
                                                          MAX_DATA_MESSAGE_SIZE);
                  for (i=0; i < dataLength; i++)
                  {
                    writePoint = vgOp8BitHex (packedBuffer_p[i], writePoint);
                  }

                  KiFreeMemory( (void**)&packedBuffer_p);
                }
                else
                {
                  for (i=0; i < signalEntity.sig->afsaGetInputInd.defaultTextString.length; i++)
                  {
                     writePoint = vgOp8BitHex(signalEntity.sig->afsaGetInputInd.
                                                defaultTextString.textString[i], writePoint);
                  }
                }
             }
             writePoint = vgPrintLine (writePoint, (const Char*)"\"");

             /* icon Id and disp mode are optional */
             if (signalEntity.sig->afsaGetInputInd.iconIdPresent == TRUE)
             {
                writePoint = vgPrintLine (writePoint, (const Char*)",");
                /* icon id */
                writePoint = vgPrintNum (writePoint,
                                          (Int16)signalEntity.sig->
                                            afsaGetInputInd.iconId.recordNum);
                writePoint = vgPrintLine (writePoint, (const Char*)",");
                /* disp Mode */
                writePoint = vgPrintNum (writePoint,
                                          (Int16) (signalEntity.sig->afsaGetInputInd.
                                           iconId.displayIconOnly == TRUE?0:1));
             }
             if (signalEntity.sig->afsaGetInputInd.textAttsPresent == TRUE)
             {
                if (signalEntity.sig->afsaGetInputInd.iconIdPresent == FALSE)
                {
                   /* need to put a comma for the lack of the  previous icon info */
                   writePoint = vgPrintLine (writePoint, (const Char*)",");
                   /* need to put a comma for the lack of the  previous disp Mode info */
                   writePoint = vgPrintLine (writePoint, (const Char*)",");
                }
                writePoint = vgPrintLine (writePoint, (const Char*)",");
                vgWriteTextParameters (&writePoint, signalEntity.sig->afsaGetInputInd.textAttributes);
             }
          }
       }
       else
       {
       }/* ok no more data to setup */
    }
  }

  /* calculate length of stk buffer line */
  vgStkUpdateBufferLength (0, writePoint);

  return (stkGenericContext_p->dataValid);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgSigAfsaDecodeToneInd
 *
 * Parameters:  signalEntity  - structure containing signal:
 *                              SIG_AFSA_DECODE_TONE_IND
 *              entity        - mux channel number
 *
 * Returns:     Boolean - validaity of signal data
 *
 * Description: Decodes the decode inkey indication and sets up the data str.
 *
 *-------------------------------------------------------------------------*/

static Boolean vgSigAfsaDecodeToneInd (const SignalBuffer signalEntity,
                                        const VgmuxChannelNumber entity)
{
  Char                    *writePoint;
  Int8                    i;
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  /* we can assume that the data is valid to start off with */
  stkGenericContext_p->dataValid = TRUE;

  stkGenericContext_p->simCommandId = signalEntity.sig->afsaPlayToneInd.simatCommandRef;

  /* allocate some memory for the sting */
  vgStkCreateBuffer (0);

  /* header */
  writePoint = vgPrintLine (stkGenericContext_p->stkCommandBuffer[0]->data,
                            (const Char*)MSTGC_PROMPT);
  writePoint = vgPrintNum (writePoint, stkGenericContext_p->cmdId);
  /* alpha id */
  if (signalEntity.sig->afsaPlayToneInd.alphaIdPresent == TRUE)
  {
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    if ( signalEntity.sig->afsaPlayToneInd.alphaId.length > 0)
    {
      writePoint = vgPrintLine (writePoint, (const Char*)"\"");
      if (vgStkOutputInTextMode (entity))
      {
        writePoint = vgStkCopyString (writePoint,
          signalEntity.sig->afsaPlayToneInd.alphaId.data,
          signalEntity.sig->afsaPlayToneInd.alphaId.length,
          0);
      }
      else
      {
        for (i=0;
              i < signalEntity.sig->afsaPlayToneInd.alphaId.length;
              i++)
        {
          writePoint = vgOp8BitHex(signalEntity.sig->afsaPlayToneInd.
                                   alphaId.data[i],
                                   writePoint);
        }
      }
      writePoint = vgPrintLine (writePoint, (const Char*)"\"");
    }
    else  /* NULL alpha ID */
    {
      writePoint = vgPrintLine (writePoint, (const Char*)"\"\"");
    }
  } /* alpha id present */
  else
  {
    if ((signalEntity.sig->afsaPlayToneInd.tonePresent == TRUE) ||
      (signalEntity.sig->afsaPlayToneInd.durationPresent == TRUE))
      /* if there is further information we need a comma for this alphaid */
    {
       writePoint = vgPrintLine (writePoint, (const Char*)",");
    }
  }
  /* tone */
  if (signalEntity.sig->afsaPlayToneInd.tonePresent == TRUE)
  {
     writePoint = vgPrintLine (writePoint, (const Char*)",");
     writePoint = vgPrintNum (writePoint, (Int16)signalEntity.sig->
                              afsaPlayToneInd.tone);
  }
  else
  {
     /* if there is no tone but there is a duration we need to add a comma */
     if (signalEntity.sig->afsaPlayToneInd.durationPresent == TRUE)
     {
       writePoint = vgPrintLine (writePoint, (const Char*)",");
     }
  }

  /* duration */
  if (signalEntity.sig->afsaPlayToneInd.durationPresent == TRUE)
  {
     writePoint = vgPrintLine (writePoint, (const Char*)",");
     writePoint = vgPrintNum (writePoint, (Int16)signalEntity.sig->
                              afsaPlayToneInd.durationUnit);
     writePoint = vgPrintLine (writePoint, (const Char*)",");
     writePoint = vgPrintNum (writePoint, (Int16)signalEntity.sig->
                              afsaPlayToneInd.durationValue);
  }
  if ((signalEntity.sig->afsaPlayToneInd.alphaIdPresent == TRUE) &&
     (signalEntity.sig->afsaPlayToneInd.alphaId.length > 0))
  {
     /* icon Id and disp mode are optional */
     if (signalEntity.sig->afsaPlayToneInd.iconIdPresent == TRUE)
     {
        if (signalEntity.sig->afsaPlayToneInd.durationPresent == FALSE)
        {
           /* need to put a comma for the lack of the  previous duration unit info */
           writePoint = vgPrintLine (writePoint, (const Char*)",");
           /* need to put a comma for the lack of the  previous duration value info */
           writePoint = vgPrintLine (writePoint, (const Char*)",");
        }
        writePoint = vgPrintLine (writePoint, (const Char*)",");
        /* icon id */
        writePoint = vgPrintNum (writePoint,
                                 (Int16)signalEntity.sig->
                                        afsaPlayToneInd.iconId.recordNum);
        writePoint = vgPrintLine (writePoint, (const Char*)",");
        /* disp Mode */
        writePoint = vgPrintNum (writePoint,
                                 (Int16) (signalEntity.sig->afsaPlayToneInd.
                                          iconId.displayIconOnly == TRUE?0:1));
     }
     if (signalEntity.sig->afsaPlayToneInd.textAttsPresent == TRUE)
     {
        if (signalEntity.sig->afsaPlayToneInd.iconIdPresent == FALSE)
        {
           /* need to put a comma for the lack of the  previous icon info */
           writePoint = vgPrintLine (writePoint, (const Char*)",");
           /* need to put a comma for the lack of the  previous disp Mode info */
           writePoint = vgPrintLine (writePoint, (const Char*)",");
        }
        writePoint = vgPrintLine (writePoint, (const Char*)",");
        vgWriteTextParameters (&writePoint, signalEntity.sig->afsaPlayToneInd.textAttributes);
     }
  }
  /* calculate length of stk buffer line */
  vgStkUpdateBufferLength (0, writePoint);

  return (stkGenericContext_p->dataValid);
}




/*--------------------------------------------------------------------------
 *
 * Function:    vgAfsaSetUpIdleModeTextInd
 *
 * Parameters:  signalEntity  - structure containing signal:
 *                              SIG_AFSA_SETUP_IDLE_TEXT_MODE_IND
 *              entity        - mux channel number
 *
 * Returns:     Boolean - validaity of signal data
 *
 * Description: Received a idle mode text indication from the SIM AT task.
 *              Decodes the signal and sets up the data string.
 *
 *-------------------------------------------------------------------------*/

static Boolean vgAfsaSetUpIdleModeTextInd (const SignalBuffer signalEntity,
                                            const VgmuxChannelNumber entity)
{
  Char                             *writePoint;
  const AfsaSetUpIdleModeTextInd   *signalData_p = &signalEntity.sig->afsaSetUpIdleModeTextInd;
  Int16                             i;   /* job115982: new max limit for 7bit packed characters */
  Int16                             dataLength;
  Int8                             *packedBuffer_p = PNULL;
  StkEntityGenericData_t           *stkGenericContext_p = ptrToStkGenericContext ();

  KiAllocZeroMemory(  sizeof(Int8)*MAX_DATA_MESSAGE_SIZE,
                      (void **) &packedBuffer_p);

  /* we can assume that the data is valid to start off with */
  stkGenericContext_p->dataValid = TRUE;

  stkGenericContext_p->simCommandId = signalData_p->simatCommandRef;

  /* allocate some memory for the string */
  vgStkCreateBuffer (0);
  /* header */
  writePoint = vgPrintLine (stkGenericContext_p->stkCommandBuffer[0]->data,
                            (const Char*)MSTGC_PROMPT);
  writePoint = vgPrintNum (writePoint, stkGenericContext_p->cmdId);
  writePoint = vgPrintLine (writePoint, (const Char*)",");  /* job115982: added missing comma */

  /* dcs */
  vgGetDcsParameter (signalData_p->textString.codingScheme.msgCoding,
                     &writePoint,
                      &stkGenericContext_p->dataValid);

  if (stkGenericContext_p->dataValid == TRUE)
  {
    /* text */
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    writePoint = vgPrintLine (writePoint, (const Char*)"\"");
    if (vgStkOutputInTextMode (entity))
    {
       writePoint = vgStkCopyString (writePoint,
         signalData_p->textString.textString,
         signalData_p->textString.length,
         0);
    }
    else
    {
      if ( signalData_p->textString.codingScheme.msgCoding == MSG_CODING_DEFAULT)
      {
        /* 7-bit decode data, so we need to re-encode it and then convert into HEX 7-bit */
        dataLength = UtEncode7BitPackedData(&packedBuffer_p[0],
                                             signalData_p->textString.length,
                                              &signalData_p->textString.textString[0],
                                               MAX_DATA_MESSAGE_SIZE);
        for (i=0; i < dataLength; i++)
        {
          writePoint = vgOp8BitHex (packedBuffer_p[i], writePoint);
        }
      }
      else
      {
        for (i=0; i < signalData_p->textString.length; i++)
        {
           writePoint = vgOp8BitHex(signalData_p->textString.textString[i], writePoint);
        }
      }
    }
    writePoint = vgPrintLine (writePoint, (const Char*)"\"");

    /* icon and disp mode */
    if (signalData_p->iconIdPresent == TRUE)
    {
       writePoint = vgPrintLine (writePoint, (const Char*)",");
       /* icon id */
       writePoint = vgPrintNum (writePoint,
                                 (Int16)signalData_p->iconId.recordNum);
       writePoint = vgPrintLine (writePoint, (const Char*)",");
       /* disp Mode */
       writePoint = vgPrintNum (writePoint,
                                 (Int16) (signalData_p->iconId.
                                  displayIconOnly == TRUE?0:1));
    }
    /* text attributes */
    if (signalData_p->textAttsPresent == TRUE)
    {
       if (signalData_p->iconIdPresent == FALSE)
       {
          /* need to put a comma for the lack of the  previous icon info */
          writePoint = vgPrintLine (writePoint, (const Char*)",");
          /* need to put a comma for the lack of the  previous disp Mode info */
          writePoint = vgPrintLine (writePoint, (const Char*)",");
       }
       writePoint = vgPrintLine (writePoint, (const Char*)",");
       vgWriteTextParameters (&writePoint, signalData_p->textAttributes);
    }
  }

  /* calculate length of stk buffer line */
  vgStkUpdateBufferLength (0, writePoint);

  KiFreeMemory( (void**)&packedBuffer_p);

  return (stkGenericContext_p->dataValid);
}



/*--------------------------------------------------------------------------
 *
 * Function:    vgAfsaSetUpEventListInd
 *
 * Parameters:  signalEntity  - structure containing signal:
 *                              SIG_AFSA_SETUP_EVENT_LIST_IND
 *              entity        - mux channel number
 *
 * Returns:     Boolean - validaity of signal data
 *
 * Description: Received a setup event list indication from the SIM AT task.
 *              Decodes the signal and setsup the data string.
 *
 *-------------------------------------------------------------------------*/

static Boolean vgAfsaSetUpEventListInd (const SignalBuffer signalEntity,
                                         const VgmuxChannelNumber entity)
{
  Char                    *writePoint;
  AfsaSetUpEventListInd   signalData = signalEntity.sig->afsaSetUpEventListInd;
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  PARAMETER_NOT_USED(entity);

  /* we can assume that the data is valid to start off with */
  stkGenericContext_p->dataValid = FALSE;

  stkGenericContext_p->simCommandId = signalEntity.sig->afsaSetUpEventListInd.simatCommandRef;

  /* allocate some memory for the string */
  vgStkCreateBuffer (0);
  /* header */
  writePoint = vgPrintLine (stkGenericContext_p->stkCommandBuffer[0]->data,
                            (const Char*)MSTGC_PROMPT);
  /* cmd id for event list is 5 so hex notation 05 */
  if (stkGenericContext_p->cmdId <= (Int16)9)
  {
    writePoint = vgPrintNum (writePoint, 0);
  }
  writePoint = vgPrintNum (writePoint, stkGenericContext_p->cmdId);

  /* clear the currently mionitored list - receipt of a new list - even an empty one
   * or one with items not monitored by the MMI always clears the previous list*/
  memset(&stkGenericContext_p->currentEventList, FALSE, sizeof(StkMmiEventList));

  if (signalData.monitorEvent.removeExistingEventList == TRUE)
  {
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    writePoint = vgPrintLine (writePoint, (const Char*)"FF");
    stkGenericContext_p->dataValid = TRUE;
  }
  if (signalData.monitorEvent.userActivity == TRUE)
  {
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    writePoint = vgPrintLine (writePoint, (const Char*)"05");
    stkGenericContext_p->dataValid = TRUE;
    stkGenericContext_p->currentEventList.userActivity = TRUE;
  }
  if (signalData.monitorEvent.idleScreenAvailable == TRUE)
  {
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    writePoint = vgPrintLine (writePoint, (const Char*)"06");
    stkGenericContext_p->dataValid = TRUE;
    stkGenericContext_p->currentEventList.idleScreenAvailable = TRUE;
  }
  if (signalData.monitorEvent.languageSelection == TRUE)
  {
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    writePoint = vgPrintLine (writePoint, (const Char*)"08");
    stkGenericContext_p->dataValid = TRUE;
    stkGenericContext_p->currentEventList.languageSelection = TRUE;
  }
  if (signalData.monitorEvent.browserTermination == TRUE)
  {
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    writePoint = vgPrintLine (writePoint, (const Char*)"09");
    stkGenericContext_p->dataValid = TRUE;
    stkGenericContext_p->currentEventList.browserTermination = TRUE;
  }
  if (signalData.monitorEvent.displayParamsChanged == TRUE)
  {
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    writePoint = vgPrintLine (writePoint, (const Char*)"0C");
    stkGenericContext_p->dataValid = TRUE;
    stkGenericContext_p->currentEventList.displayParamsChanged = TRUE;
  }
  /* calculate length of stk buffer line */
  vgStkUpdateBufferLength (0, writePoint);

  return (stkGenericContext_p->dataValid);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgAfsaLanguageNotificationInd
 *
 * Parameters:  signalEntity  - structure containing signal:
 *                              SIG_AFSA_LANGUAGE_NOTIFICATION_IND
 *              entity        - mux channel number
 *
 * Returns:     Boolean - validaity of signal data
 *
 * Description: Received a lang notification indication from the SIM AT task.
 *              Decodes the signal and setsup the data string.
 *
 *-------------------------------------------------------------------------*/

static Boolean vgAfsaLanguageNotificationInd (const SignalBuffer signalEntity,
                                               const VgmuxChannelNumber entity)
{
  Char                         *writePoint;
  AfsaLanguageNotificationInd  signalData = signalEntity.
  sig->afsaLanguageNotificationInd;
  StkEntityGenericData_t       *stkGenericContext_p = ptrToStkGenericContext ();

  PARAMETER_NOT_USED(entity);

  /* we can assume that the data is valid to start off with */
  stkGenericContext_p->dataValid = TRUE;

  stkGenericContext_p->simCommandId = signalEntity.sig->afsaLanguageNotificationInd.simatCommandRef;

  /* allocate some memory for the sting */
  vgStkCreateBuffer (0);
  /* header */
  writePoint = vgPrintLine (stkGenericContext_p->stkCommandBuffer[0]->data,
                            (const Char*)MSTUD_PROMPT);
  writePoint = vgPrintNum (writePoint, stkGenericContext_p->cmdId);

  /* language */
  if (signalData.languageCodePresent == TRUE)
  {
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    writePoint = vgPrintLine (writePoint, (const Char*)"\"");
    if (vgStkOutputInTextMode (entity))
    {
      writePoint = vgStkCopyString (writePoint,
                                  (Char*)&signalData.languageCode[0],
                                  2,
                                  0);
    }
    else
    {
      writePoint = vgPrintNum (writePoint,(Int16)signalData.languageCode [0]);
      writePoint = vgPrintNum (writePoint,(Int16)signalData.languageCode [1]);
    }
    writePoint = vgPrintLine (writePoint, (const Char*)"\"");
  }
  else
  {
    /* optional so nothing to do */
  }

  /* calculate length of stk buffer line */
  vgStkUpdateBufferLength (0, writePoint);

  /* now send the unsolicited result */
  if (stkGenericContext_p->dataValid == TRUE)
  {
    if(isEntityMmiNotUnsolicited(entity))
    {
      sendDataWithLengthToCrm (
        stkGenericContext_p->stkCommandBuffer[0]->data,
        stkGenericContext_p->stkCommandBuffer[0]->dataLength, TRUE, TRUE,  vgGetMmiUnsolicitedChannel());
    }
    else
    {
      sendDataWithLengthToCrm (
        stkGenericContext_p->stkCommandBuffer[0]->data,
        stkGenericContext_p->stkCommandBuffer[0]->dataLength, TRUE, TRUE, entity);
    }
  }

  return (stkGenericContext_p->dataValid);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgAfsaLaunchBrowserInd
 *
 * Parameters:  signalEntity  - structure containing signal:
 *                              SIG_AFSA_LAUNCH_BROWSER_IND
 *              entity        - mux channel number
 *
 * Returns:     Boolean - validaity of signal data
 *
 * Description: Received a launcH browser indication from the SIM AT task.
 *              Decodes the signal and sets up the data string.
 *
 *-------------------------------------------------------------------------*/

static Boolean vgAfsaLaunchBrowserInd (const SignalBuffer signalEntity,
                                        const VgmuxChannelNumber entity)
{
  Char                    *writePoint;
  Int16                   allFr, fileIdCount;
  AfsaLaunchBrowserInd    *sig_p = &signalEntity.sig->afsaLaunchBrowserInd;
  Int8                    i;
  Int16                   j;   /* job115982: new max limit for 7bit packed characters */
  Int16                   dataLength;
  Int8                    *packedBuffer_p = PNULL;
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  /* we can assume that the data is valid to start off with */
  stkGenericContext_p->dataValid = TRUE;

  stkGenericContext_p->simCommandId = sig_p->simatCommandRef;

  /* allocate some memory for the string */
  vgStkCreateBuffer (0);
  /* header */
  writePoint = vgPrintLine (stkGenericContext_p->stkCommandBuffer[0]->data,
                            (const Char*)MSTGC_PROMPT);
  writePoint = vgPrintNum (writePoint, stkGenericContext_p->cmdId);
  /* comQual */
  writePoint = vgPrintLine (writePoint, (const Char*)",");
  writePoint = vgPrintNum (writePoint, (Int16) sig_p->commandQual);
  /* url */
  writePoint = vgPrintLine (writePoint, (const Char*)",");
  writePoint = vgPrintLine (writePoint, (const Char*)"\"");
  if (vgStkOutputInTextMode (entity))
  {
    writePoint = vgStkCopyString (writePoint,
                                  sig_p->url.data,
                                  sig_p->url.length,
                                  0);
  }
  else
  {
   for (i=0; i < sig_p->url.length; i++)
   {
      writePoint = vgOp8BitHex(sig_p->url.data[i],
                                writePoint);
   }
  }
  writePoint = vgPrintLine (writePoint, (const Char*)"\"");

  if (sig_p->browserIdPresent == TRUE)
  {
    /* browser id */
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    writePoint = vgPrintNum (writePoint, sig_p->browserId);
  }
  if (sig_p->bearerPresent == TRUE)
  {
     /* bearer */
     writePoint = vgPrintLine (writePoint, (const Char*)",");
     writePoint = vgPrintNum (writePoint, (Int16) sig_p->bearer[0]);
  }
  /* added for job104951 */
  if (sig_p->provFRDataPresent == TRUE)
  {
     /* provisioning file reference data */
     writePoint = vgPrintLine (writePoint, (const Char*)",");
     writePoint = vgPrintNum (writePoint, sig_p->provFRData.numProvFRFiles);
     for (allFr = 0; allFr < sig_p->provFRData.numProvFRFiles; allFr++)
     {
        writePoint = vgPrintLine (writePoint, (const Char*)",\"");
        for (fileIdCount = 0;
             fileIdCount < (Int16)(sig_p->provFRData.fileInfo[allFr].fileIdLength);
             fileIdCount++)
        {
           writePoint = vgPrintNum (writePoint, sig_p->provFRData.
              fileInfo[allFr].fileId[fileIdCount]);
        }
        writePoint = vgPrintLine (writePoint, (const Char*)"\"");
     }
  }
  if (sig_p->textStringPresent == TRUE)
  {
     vgGetDcsParameter (sig_p->textString.codingScheme.msgCoding,
                        &writePoint,
                        &stkGenericContext_p->dataValid);
     if (stkGenericContext_p->dataValid == TRUE)
     {
        /* gateway text */
        writePoint = vgPrintLine (writePoint, (const Char*)",");
        writePoint = vgPrintLine (writePoint, (const Char*)"\"");
        if (vgStkOutputInTextMode (entity))
        {
           writePoint = vgStkCopyString (writePoint,
                                         sig_p->textString.textString,
                                         sig_p->textString.length,
                                         0);
        }
        else
        {
           if ( sig_p->textString.codingScheme.msgCoding == MSG_CODING_DEFAULT)
           {
              KiAllocZeroMemory(    sizeof(Int8)*(MAX_DATA_MESSAGE_SIZE),
                                    (void **) &packedBuffer_p);

              /* 7-bit decode data, so we need to re-encode it and then convert into HEX 7-bit */
              dataLength = UtEncode7BitPackedData(&packedBuffer_p[0],
                                                  sig_p->textString.length,
                                                  &sig_p->textString.textString[0],
                                                  MAX_DATA_MESSAGE_SIZE);
               /* job115982: cater for new max 16bit length  */
               for (j=0; j < dataLength; j++)
               {
                   writePoint = vgOp8BitHex (packedBuffer_p[j], writePoint);
               }

               KiFreeMemory( (void**)&packedBuffer_p);
           }
           else
           {
               /* job115982: cater for new max 16bit length  */
               for (j=0; j < sig_p->textString.length; j++)
               {
                  writePoint = vgOp8BitHex(sig_p->textString.textString[j], writePoint);
               }
           }
        }
        writePoint = vgPrintLine (writePoint, (const Char*)"\"");
     }
  }
  /* alpha id */
  if (sig_p->alphaIdPresent == TRUE)
  {
     writePoint = vgPrintLine (writePoint, (const Char*)",");
     if (sig_p->alphaId.length > 0 )
     {
        writePoint = vgPrintLine (writePoint, (const Char*)"\"");
        if (vgStkOutputInTextMode (entity))
        {
           writePoint = vgStkCopyString (writePoint,
                                         sig_p->alphaId.data,
                                         sig_p->alphaId.length,
                                         0);
        }
        else
        {
           for (i=0; i < sig_p->alphaId.length; i++)
           {
              writePoint = vgOp8BitHex(sig_p->alphaId.data[i],
                                       writePoint);
           }
        }
        writePoint = vgPrintLine (writePoint, (const Char*)"\"");
        /* icon and disp mode */
        if (sig_p->iconIdPresent == TRUE)
        {
           writePoint = vgPrintLine (writePoint, (const Char*)",");
           /* icon id */
           writePoint = vgPrintNum (writePoint,
                        (Int16)sig_p->iconId.recordNum);
           writePoint = vgPrintLine (writePoint, (const Char*)",");
           /* disp Mode */
           writePoint = vgPrintNum (writePoint,
                        (Int16) (sig_p->iconId.
                        displayIconOnly == TRUE?0:1));
        }
        /* text attributes */
        if (sig_p->textAttsPresent == TRUE)
        {
           if (sig_p->iconIdPresent == FALSE)
           {
              /* need to put a comma for the lack of the  previous icon info */
              writePoint = vgPrintLine (writePoint, (const Char*)",");
              /* need to put a comma for the lack of the  previous disp Mode info */
              writePoint = vgPrintLine (writePoint, (const Char*)",");
           }
           writePoint = vgPrintLine (writePoint, (const Char*)",");
           vgWriteTextParameters (&writePoint, sig_p->textAttributes);
        }
     }
     else
     {
        writePoint = vgPrintLine (writePoint, (const Char*)"\"\"");
     } /* null alpha ID */
  } /* alpha id  present*/

  /* calculate length of stk buffer line */
  vgStkUpdateBufferLength (0, writePoint);

  return (stkGenericContext_p->dataValid);
}




/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:    vgInitialiseStkData
 *
 * Parameters:  in:  Void
 *
 * Returns:     Void
 * Description: Initialises the data.
 *
 * Design spec:
 *
*-------------------------------------------------------------------------*/
void vgInitialiseStkData (void)
{
  Int8                    mIndex;
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  for (mIndex = 0; mIndex < SIMAT_MAX_NUM_ITEMS + 1; mIndex++)
  {
    stkGenericContext_p->stkCommandBuffer[mIndex] = PNULL;
  }

  stkGenericContext_p->cmdId = 0;
  stkGenericContext_p->menuHasBeenRemoved = TRUE;
  stkGenericContext_p->dataValid = FALSE;
  stkGenericContext_p->runAtCmdState = STATE_STK_NULL;
  stkGenericContext_p->atCommandData.cmdEntity = VGMUX_CHANNEL_INVALID;
  stkGenericContext_p->atCommandData.cmdInput_p = PNULL;
  stkGenericContext_p->atCommandData.cmdOutput_p = PNULL;
  stkGenericContext_p->atCommandData.cirmDataIndCount = 0;
  if ( stkGenericContext_p->currentMmiProfile_p != PNULL )
  {
     KiFreeMemory ((void **)& stkGenericContext_p->currentMmiProfile_p);

  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkSetUpData
 *
 * Parameters:  signalEntity  - structure containing signal:
 *              commandId     - current STK command
 *              entity        - mux channel number
 *
 * Returns:     Boolean - validatity of signal data
 *
 * Description: handler that compairs the suplied signal type against the
 *              indications in the map and then calls the approprriate function.
 *
 *-------------------------------------------------------------------------*/

Boolean vgStkSetUpData (const SignalBuffer signalEntity,
                         const StkCommandId commandId,
                          const VgmuxChannelNumber entity)
{
  const StkDecodeSignalEntityControl  *signalTable = &stkDecodeSignalEntityTable[0];
  StkEntityGenericData_t              *stkGenericContext_p = ptrToStkGenericContext ();
  Boolean foundType = FALSE;
  Boolean result    = FALSE;

  /* set up the cmd id variable */
  stkGenericContext_p->cmdId         = commandId;
  stkGenericContext_p->currentSignal = *signalEntity.type;

  /* search through STK signal indications for a type match */
  while ((signalTable->signalType != SIG_SYS_DUMMY) &&
         (foundType == FALSE))
  {
    if (signalTable->signalType == *(signalEntity.type))
    {
      foundType = TRUE;
      result    = signalTable->stkDecodeProcFunc (signalEntity, entity);
    }
    else
    {
      signalTable++;
    }
  }

  return (result);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgStkSendData
 *
 * Parameters:  none
 *
 * Returns:     nothing
 *
 * Description: Loops through stkBuffer and for each line that is not Null
 *              sends the data to the datas stack.
 *
 *-------------------------------------------------------------------------*/

void vgStkSendData (void)
{
  StkEntityGenericData_t* stkGenericContext_p = ptrToStkGenericContext ();
  Int8                    allLines;
  Boolean                 cReturn = TRUE;

  for (allLines = 0;
       (allLines <= SIMAT_MAX_NUM_ITEMS);
         allLines++)
  {
    /* check line allocated and has length greater than zero */
    if ((stkGenericContext_p->stkCommandBuffer[allLines] != PNULL) &&
        (stkGenericContext_p->stkCommandBuffer[allLines]->dataLength > 0))
    {
      sendDataWithLengthToCrm (
        stkGenericContext_p->stkCommandBuffer[allLines]->data,
        stkGenericContext_p->stkCommandBuffer[allLines]->dataLength,
        cReturn,
        TRUE,
        stkGenericContext_p->registeredStkEntity);
      cReturn = FALSE;
    }
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgStkDestroyBuffer
 *
 * Parameters:  none
 *
 * Returns:     nothing
 *
 * Description: Called to claim back the memory allocated to allow
 *              storage of the command lines.
 *
 *-------------------------------------------------------------------------*/

void vgStkDestroyBuffer (void)
{
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();
  Int8                    lines = 0;

  while ((stkGenericContext_p->stkCommandBuffer[lines] != PNULL) &&
         (lines < SIMAT_MAX_NUM_ITEMS))
  {
    KiFreeMemory ((void **)&stkGenericContext_p->stkCommandBuffer[lines]);
    stkGenericContext_p->stkCommandBuffer[lines] = PNULL;
    lines++;
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkGetCurrentSignal
 *
 * Parameters:  none
 *
 * Returns:     SignalId - current signal
 *
 * Description: returns the current STK signal from generic context
 *
 *-------------------------------------------------------------------------*/

SignalId vgStkGetCurrentSignal (void)
{
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  return (stkGenericContext_p->currentSignal);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkGetSimCmd
 *
 * Parameters:  none
 *
 * Returns:     Int16 - command refernce used in stk sim signalling
 *
 * Description: returns the STK signal reference
 *
 *-------------------------------------------------------------------------*/

Int32 vgStkGetSimCmd (void)
{
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  return (stkGenericContext_p->simCommandId);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkGetCmdId
 *
 * Parameters:  none
 *
 * Returns:     Int16 - command refernce used in stk signalling
 *
 * Description: returns the recorded command id.  this will be set up when
 *              we receive an Indication from the SIM AT task.
 *
 *-------------------------------------------------------------------------*/

Int16 vgStkGetCmdId (void)
{
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  return (stkGenericContext_p->cmdId);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkIsDataValid
 *
 * Parameters:  none
 *
 * Returns:     Boolean - validity flag
 *
 * Description: returns a flag indicating whether the data is valid.
 *
 *-------------------------------------------------------------------------*/

Boolean vgStkIsDataValid (void)
{
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  return (stkGenericContext_p->dataValid);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkSetDataToInValid
 *
 * Parameters:  none
 *
 * Returns:     nothing
 *
 * Description: sets the data valid flag to invalid.  this function is
 *              called when the stk timer expires. any subsequent AT
 *              commands check the state if the flag.
 *
 *-------------------------------------------------------------------------*/

void vgStkSetDataToInValid (void)
{
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  stkGenericContext_p->dataValid = FALSE;
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkMenuItemPresent
 *
 * Parameters:  thisItem : the menu item under test
 *
 * Returns:     Boolean : the menu iten is/not in the menu list
 *
 * Description: Checks the current menu list to see if it is present
 *
 *-------------------------------------------------------------------------*/

Boolean vgStkMenuItemPresent (Int32 thisItem)
{
  Boolean          found =  FALSE;
  StkMenuItems_t*  menu_p = stkMenuItemList_p;

  if (menu_p != PNULL)
  {
    do
    {
      if ((Int32)menu_p->menuItem == thisItem)
      {
        found = TRUE;
      }
      else
      {
        menu_p = menu_p->next_p;
      }
    }
    while ( (menu_p != PNULL ) && ( !found ));
  }

  return (found);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkOutputInTextMode
 *
 * Parameters:  entity  - mux channel number
 *
 * Returns:     Boolean - whether we are in text mode
 *
 * Description: Determines whether STK output should be formatted in PDU or
 *              Text Mode depending on profile values
 *
 *-------------------------------------------------------------------------*/

Boolean vgStkOutputInTextMode (const VgmuxChannelNumber entity)
{
  Boolean inTextMode;

  /* use MSTMODE setting to determine whether we are in text mode */
  switch (getProfileValue (entity, PROF_MSTMODE))
  {
    case STK_MODE_TEXT:
    {
      inTextMode = TRUE;
      break;
    }
    case STK_MODE_PDU:
    {
      inTextMode = FALSE;
      break;
    }
    case STK_MODE_CMGF:
    {
      inTextMode = (getProfileValue (entity, PROF_CMGF) == PROF_CMGF_TEXT_MODE);
      break;
    }
    default:
    {
      /* an unexpected STK Mode has been encountered */
      FatalParam (getProfileValue (entity, PROF_MSTMODE),
                  getProfileValue (entity, PROF_CMGF),
                  entity);
      /* default to PDU mode */
      inTextMode = FALSE;
      break;
    }
  }

  return (inTextMode);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgAfsaRunAtCommandInd
 *
 * Parameters:  signalEntity  - structure containing signal:
 *                              SIG_AFSA_RUN_AT_COMMAND_IND
 *              entity        - mux channel number
 *
 * Returns:     Boolean - validaity of signal data
 *
 * Description: Received a run at indication from the SIM AT task.
 *              Decodes the signal and setsup the data string.
 *
 *-------------------------------------------------------------------------*/

void vgAfsaRunAtCommandInd (const SignalBuffer signalEntity,
                            const VgmuxChannelNumber entity)
{
  Char                    *writePoint;
  Int8                    i;
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(stkGenericContext_p != PNULL, entity, 0, 0);
#endif
  /* we can assume that the data is valid to start off with */
  stkGenericContext_p->dataValid = TRUE;

  stkGenericContext_p->cmdId = STK_CMD_SETUP_RUN_AT_COMMAND;
  stkGenericContext_p->simCommandId  = signalEntity.sig->afsaRunAtCommandInd.simatCommandRef;
  stkGenericContext_p->currentSignal = *signalEntity.type;

  /* allocate some memory for the sting */
  vgStkCreateBuffer (0);
  /* header */
  writePoint = vgPrintLine (stkGenericContext_p->stkCommandBuffer[0]->data,
                            (const Char*)MSTUD_PROMPT);
  writePoint = vgPrintNum (writePoint, stkGenericContext_p->cmdId);

  /* alpha id */

  if (signalEntity.sig->afsaRunAtCommandInd.alphaIdPresent == TRUE)
  {
    writePoint = vgPrintLine (writePoint, (const Char*)",");
    if ( signalEntity.sig->afsaRunAtCommandInd.alphaId.length != 0)
    {
       writePoint = vgPrintLine (writePoint, (const Char*)"\"");
       if (vgStkOutputInTextMode (entity))
       {
          writePoint = vgStkCopyString (writePoint,
            signalEntity.sig->afsaRunAtCommandInd.alphaId.data,
            signalEntity.sig->afsaRunAtCommandInd.alphaId.length,
            0);
       }
       else
       {
          for (i=0;
                i < signalEntity.sig->afsaRunAtCommandInd.alphaId.length;
                 i++)
          {
             writePoint = vgOp8BitHex(signalEntity.sig->afsaRunAtCommandInd.
                                       alphaId.data[i],
                                        writePoint);
          }
       }
       writePoint = vgPrintLine (writePoint, (const Char*)"\"");

       /* icon and display mode */
       if (signalEntity.sig->afsaRunAtCommandInd.iconIdPresent == TRUE)
       {
          writePoint = vgPrintLine (writePoint, (const Char*)",");
          /* icon id */
          writePoint = vgPrintNum (writePoint,(Int16)signalEntity.sig->
           afsaRunAtCommandInd.iconId.recordNum);
          writePoint = vgPrintLine (writePoint, (const Char*)",");
          /* disp Mode */
          writePoint = vgPrintNum (writePoint,
                                    (Int16) (signalEntity.sig->afsaRunAtCommandInd.
                                     iconId.displayIconOnly == TRUE?0:1));
       }
       /* text attributes */
       if (signalEntity.sig->afsaRunAtCommandInd.textAttsPresent == TRUE)
       {
          if (signalEntity.sig->afsaRunAtCommandInd.iconIdPresent == FALSE)
          {
             /* need to put a comma for the lack of the  previous icon info */
             writePoint = vgPrintLine (writePoint, (const Char*)",");
             /* need to put a comma for the lack of the  previous disp Mode info */
             writePoint = vgPrintLine (writePoint, (const Char*)",");
          }
          writePoint = vgPrintLine (writePoint, (const Char*)",");
          vgWriteTextParameters (&writePoint, signalEntity.sig->afsaRunAtCommandInd.textAttributes);
       }
    }
    else
    { /* NULL alpha ID so put empty string */
       writePoint = vgPrintLine (writePoint, (const Char*)"\"\"");
    }
  }
  else
  {
    /* optional so nothing to do */
  }

  /* calculate length of stk buffer line */
  vgStkUpdateBufferLength (0, writePoint);

  if (stkGenericContext_p->dataValid == TRUE)
  {
    if(isEntityMmiNotUnsolicited(entity))
    {
      sendDataWithLengthToCrm (
        stkGenericContext_p->stkCommandBuffer[0]->data,
        stkGenericContext_p->stkCommandBuffer[0]->dataLength, TRUE, TRUE, vgGetMmiUnsolicitedChannel());
    }
    else
    {
      sendDataWithLengthToCrm (
        stkGenericContext_p->stkCommandBuffer[0]->data,
        stkGenericContext_p->stkCommandBuffer[0]->dataLength, TRUE, TRUE, entity);
    }
  }

  /* now store the AT command data so that when the channel is properly set-up the
   * data can be sent to run the at command */
  if ( stkGenericContext_p->atCommandData.cmdInput_p == PNULL)
  {
    KiAllocMemory(sizeof(SimatAtCommandData),
                  (void **)&stkGenericContext_p->atCommandData.cmdInput_p);
  }

  /* copy the at command content from the signal into the storage */
  memcpy(stkGenericContext_p->atCommandData.cmdInput_p,
         &signalEntity.sig->afsaRunAtCommandInd.atCommand, sizeof(SimatAtCommandData));

  switch ( stkGenericContext_p->runAtCmdState)
  {
    case STATE_STK_NULL :
    /*now enable the channel so we have an entity to send the at command on */
      vgStkRequestMuxChannelToRunAtCommand(entity);
    break;

    case STATE_CHANNEL_ENABLED:
      vgStkProcessProactiveCommandRunAtCommand();
    break;

    default:
      FatalParam (entity, stkGenericContext_p->runAtCmdState, 0);
    break;
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkUpdateBufferLength
 *
 * Parameters:  none
 *
 * Returns:     Boolean - validity flag
 *
 * Description: returns a flag indicating whether the data is valid.
 *
 *-------------------------------------------------------------------------*/
static void vgStkUpdateBufferLength (Int16 index, Char* point)
{
  StkEntityGenericData_t           *stkGenericContext_p = ptrToStkGenericContext ();

  FatalAssert (stkGenericContext_p->stkCommandBuffer[index] != PNULL);
  FatalAssert (point >= stkGenericContext_p->stkCommandBuffer[index]->data);
  if ((stkGenericContext_p->stkCommandBuffer[index] != PNULL) &&
      (point >= stkGenericContext_p->stkCommandBuffer[index]->data))
  {
    stkGenericContext_p->stkCommandBuffer[index]->dataLength      = (Int16)
    (point - stkGenericContext_p->stkCommandBuffer[index]->data);

    FatalAssert (stkGenericContext_p->stkCommandBuffer[index]->dataLength <= MAX_DATA_MESSAGE_SIZE);
  }

  vgStkResizeBuffer(index);
}
/* END OF FILE */
