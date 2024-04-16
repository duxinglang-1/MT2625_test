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
 * Procedures for Test AT command execution
 * Handler for AT commands located in vgtsss.c
 *
 * Implemented commands:
 *
 * AT*MTSOLSIG - displays list of pending solicited signals
 * AT*MTEXCALL - starts or stops an external data call
 * AT*MTFLAG   - enables CC and Change Control debug output
 **************************************************************************/

#define MODULE_NAME "RVTSUT"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <stdio.h>
#include <cici_sig.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvprof.h>
#include <rvtsut.h>
#include <rvtssigo.h>
#include <rvcimux.h>
#include <rvcimxut.h>
#include <rvoman.h>
#include <rvcrhand.h>
#include <rvcrconv.h>
#include <rvchman.h>
#include <rvsspars.h>
#include <ki_sig.h>
#include <rvcmux.h>
#include <frhsl.h>
#include <gkisig.h>
/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

typedef struct ChElementNamesTag
{
  const VgChangeControlElements element;
  const Char                    *string;
} ChElementNames;

static const ChElementNames vgChangeControlElementNames[] =
{
  { CC_GENERAL_PACKET_RADIO_SYSTEM, (const Char*)"GENERAL_PACKET_RADIO_SYSTEM"            },
  { CC_MOBILITY_MANAGEMENT,         (const Char*)"MOBILITY_MANAGEMENT"                    },
  { CC_SUBSCRIBER_IDENTITY_MODULE,  (const Char*)"SUBSCRIBER_IDENTITY_MODULE"             },
  { CC_POWER_MANAGEMENT,            (const Char*)"POWER_MANAGEMENT"                       },
#if defined (FEA_PHONEBOOK)
  { CC_LIST_MANAGEMENT,             (const Char*)"LIST_MANAGEMENT"                        },
#endif /* FEA_PHONEBOOK */
  { CC_ENGINEERING_MODE,            (const Char*)"ENGINEERING_MODE"                       },
  { CC_SHORT_MESSAGE_SERVICE,       (const Char*)"SHORT_MESSAGE_SERVICE"                  },
  { CC_GENERAL_MODULE,              (const Char*)"GENERAL MODULE"                         },
  { CC_DM_MODULE,                   (const Char*)"DM MODULE"                              },
};

#define NUM_CHANGE_CONTROL_NAMES (sizeof(vgChangeControlElementNames) / sizeof(ChElementNames))

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/
#if defined (DEVELOPMENT_VERSION)

/* job104730: GPRS function prototypes removed */

static void stripAssertFileName (Char *fileName);

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Variables
 ***************************************************************************/

/***************************************************************************
 * Macros
 ***************************************************************************/

union Signal
{
  KiDevAssertInd  kiDevAssertInd;
  KiDevCheckInd   kiDevCheckInd;
  /* job104730: remove snDataReq */
};

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
*
* Function:    stripAssertFileName
*
* Parameters:  Char * - devfail file name
*
* Returns:     nothing
*
* Description: Removes extension and path information from file name
*
*-------------------------------------------------------------------------*/

static void stripAssertFileName (Char *fileName)
{
  Boolean slashPresent = FALSE;
  Boolean dotPresent   = FALSE;
  Int16   slashIndex   = 0;
  Int16   dotIndex     = 0;
  Int16   index;

  /* find filename delimiters */
  for (index = 0; index < KI_MAX_ASSERT_FILE_NAME; index++)
  {
    if (fileName[index] == BACK_SLASH_CHAR)
    {
      slashIndex   = index;
      slashPresent = TRUE;
    }
    if (fileName[index] == DOT_CHAR)
    {
      dotIndex   = index;
      dotPresent = TRUE;
    }
  }

  /* if extension present then strip out */
  if (dotPresent == TRUE)
  {
    fileName[dotIndex] = NULL_CHAR;
  }

  /* if path present then strip out */
  if (slashPresent == TRUE)
  {
    for (index = 0; index < (KI_MAX_ASSERT_FILE_NAME - slashIndex); index++)
    {
      fileName[index] = fileName[index + slashIndex + 1];
    }
  }

  /* convert to upper case */
  vgConvertStringToUpper (fileName, fileName);

}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
*
* Function:    vgTsDoubleTypeCheck
*
* Parameters:  entity   - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Checks for cases of two solicited signals of the same type
*              on change control
*
*-------------------------------------------------------------------------*/

void vgTsDoubleTypeCheck (const VgmuxChannelNumber entity)
{
  SolicitedSignalRecord_t *solicitedSignalRecord_p = getSolicitedSignalsForEntity (entity);
  VgChangeControlElements signalElement;
  Int8 item1, item2;

  for (item1 = 1; item1 < SOLICITED_MAX_NUM_ITEMS; item1++ )
  {
    if (solicitedSignalRecord_p[item1].type != NON_SIGNAL)
    {
      for (item2 = 0; item2 < item1; item2++ )
      {
        /* two solicited signals of the same type */
        if (solicitedSignalRecord_p[item1].type ==
             solicitedSignalRecord_p[item2].type)
        {
          /* under change control */
          if (vgChManFindElement (solicitedSignalRecord_p[item1].type, &signalElement) == TRUE)
          {
            /* check that the signal found is not on exception list */
            if ((solicitedSignalRecord_p[item1].type != SIG_APEX_SM_READ_SMSP_CNF)
#if defined (FEA_PHONEBOOK)
                &&
                (solicitedSignalRecord_p[item1].type != SIG_APEX_LM_GET_ALPHA_CNF)
#endif  /* FEA_PHONEBOOK */
                )
            {
              /* if the following FatalParam has been displayed then there is a
               * possible problem. Two signals of the same type are expected by
               * the same entity. The signals are also under change control,
               * have they been handled for this eventuality? */

              /* Double signal type found */
              FatalParam (solicitedSignalRecord_p[item1].type, 0, 0);

            }
          }
        }
      }
    }
  }
}

/*----------------------------------------------------------------------------
 *
 * Function:    vgTsDevAssertInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_KI_DEV_ASSERT_IND
 *                                   SIG_KI_DEV_FAIL_IND
 *
 * Returns:     Nothing
 *
 * Description: Sends DevAssert and DevFail message to the Serial Line
 *----------------------------------------------------------------------------*/

void vgTsDevAssertInd (const SignalBuffer *signalBuffer,const VgmuxChannelNumber entity)
{
  KiDevAssertInd *sig_p = &signalBuffer->sig->kiDevAssertInd;

  Char  file[KI_MAX_ASSERT_FILE_NAME + NULL_TERMINATOR_LENGTH] = {0};
  Char  text[KI_MAX_ASSERT_TEXT + NULL_TERMINATOR_LENGTH] = {0};

  /* copy and null terminate dev text string */
  memcpy (file, sig_p->file, KI_MAX_ASSERT_FILE_NAME);
  file[KI_MAX_ASSERT_FILE_NAME] = NULL_CHAR;

  /* copy and null terminate dev text string */
  memcpy (text, sig_p->text, KI_MAX_ASSERT_TEXT);
  text[KI_MAX_ASSERT_TEXT] = NULL_CHAR;

  stripAssertFileName (file);

  vgPutNewLine  (entity);
  vgPrintf      (entity,
                 (const Char*)"*MERRMSG: file[%s] line[%d] text[%s]",
                  file,
                   sig_p->line,
                    text);
  vgPutNewLine  (entity);
  vgFlushBuffer (entity);
}

/*----------------------------------------------------------------------------
 *
 * Function:    vgTsDevCheckInd
 *
 * Parameters:  SignalBuffer       - structure containing signal:
 *                                   SIG_KI_DEV_CHECK_IND
 *
 * Returns:     Nothing
 *
 * Description: Sends DevChange message to the Serial Line
 *----------------------------------------------------------------------------*/

void vgTsDevCheckInd (const SignalBuffer *signalBuffer,const VgmuxChannelNumber entity)
{
  KiDevCheckInd *sig_p = &signalBuffer->sig->kiDevCheckInd;

  Char  file[KI_MAX_ASSERT_FILE_NAME + NULL_TERMINATOR_LENGTH] = {0};
  Char  text[KI_MAX_ASSERT_TEXT + NULL_TERMINATOR_LENGTH] = {0};

  /* copy and null terminate dev text string */
  memcpy (file, sig_p->file, KI_MAX_ASSERT_FILE_NAME);
  file[KI_MAX_ASSERT_FILE_NAME] = NULL_CHAR;

  /* copy and null terminate dev text string */
  memcpy (text, sig_p->text, KI_MAX_ASSERT_TEXT);
  text[KI_MAX_ASSERT_TEXT] = NULL_CHAR;

  stripAssertFileName (file);

  vgPutNewLine  (entity);
  vgPrintf      (entity,
                 (const Char*)"*MERRMSG: file[%s] line[%d] text[%s] p1[%d] p2[%d] p3[%d]",
                  file,
                   sig_p->line,
                    text,
                     sig_p->param1,
                      sig_p->param2,
                       sig_p->param3);
  vgPutNewLine  (entity);
  vgFlushBuffer (entity);
}

/***************************************************************************
 * Processes
 ***************************************************************************/

#endif

/*--------------------------------------------------------------------------
*
* Function:    vgTsHSLChangeControlElement
*
* Parameters:  callinfo - call information to be displayed
*              entity   - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: HSL the specified change control element in text format
*
*-------------------------------------------------------------------------*/

void vgTsHSLChangeControlElement (const VgChangeControlElements procId,
                                       const VgmuxChannelNumber entity)
{
  ChManagerContext_t      *chManagerContext_p = ptrToChManagerContext ();
  Int8 index;

  for (index = 0; index < NUM_CHANGE_CONTROL_NAMES; index++)
  {
    if (vgChangeControlElementNames[index].element == procId)
    {

      break;
    }
  }
}

/* END OF FILE */


