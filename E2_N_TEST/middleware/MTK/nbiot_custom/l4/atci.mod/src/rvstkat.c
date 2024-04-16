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
 * Module contains the AT command handlers for the sim ci stk.
 **************************************************************************/

#define MODULE_NAME "RVSTKAT"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#if !defined (SYSTEM_H)
#  include <system.h>
#endif


#include <gkisig.h>
#include <gkimem.h>

#if !defined (AFSA_SIG_H)
#  include <afsa_sig.h>
#endif
#if !defined (ABST_SIG_H)
#  include <abst_sig.h>
#endif
#if !defined (RVSTKAT_H)
#  include <rvstkat.h>
#endif
#if !defined (RVSTKTP_H)
#  include <rvstktp.h>
#endif

#if !defined (RVSTKPD_H)
#  include <rvstkpd.h>
#endif
#if !defined (RVUTIL_H)
#  include <rvutil.h>
#endif
#if !defined (RVSTKRD_H)
#  include <rvstkrd.h>
#endif
#if !defined (RVDATA_H)
#  include <rvdata.h>
#endif
#if !defined (ALSI_SIG_H)
#  include <alsi_sig.h>
#endif
#if !defined (ALSA_SIG_H)
#  include <alsa_sig.h>
#endif
#if !defined (RVCIMUX_H)
#  include <rvcimux.h>
#endif
#if !defined (RVOMTIME_H)
#  include <rvomtime.h>
#endif
#if !defined (RVSTKSOT_H)
#  include <rvstksot.h>
#endif
#if !defined (RVOMAN_H)
#  include <rvoman.h>
#endif
#if !defined (RVOMTIME_H)
#  include <rvomtime.h>
#endif
#if !defined (RVCRHAND_H)
#  include <rvcrhand.h>
#endif

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static void vgStkProcessMmiProfile (CommandLine_t *commandBuffer_p,
                                      SimatMmiSupportProfile *mmiProfile_p,
                                       Boolean *result);

static ResultCode_t vgStkReadDisplayParameters (CommandLine_t *commandBuffer_p,
                                       SignalBuffer   *signalBuffer);
/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  AfsaDisplayStatusInd      afsaDisplayStatusInd;
  AfsaLanguageSelectionInd  afsaLanguageSelectionInd;
  AfsaMmiProfileUpdateReq   afsaMmiProfileUpdateReq;
  AfsaReadMmiProfileReq     afsaReadMmiProfileReq;
  AfsaCloseBrowserInd       afsaCloseBrowserInd;
  AfsaSetUpEventListInd     afsaSetUpEventListInd;
  AfsaMenuSelectionReq      afsaMenuSelectionReq;
  EmptySignal               afsaUserActivityInd;
  AfsaListImageRecReq       afsaListImageRecReq;
  AfsaReadImageRecReq       afsaReadImageRecReq;
  AfsaReadImageInstDataReq  afsaReadImageInstDataReq;
  AfsaDisplayParamsChangedInd afsaDisplayParamsChangedInd;
};


/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

#define VG_STK_RESP_TIMER_MAX_VAL     (3600)
#define MAX_PROFILE_LENGTH            (30)
#define MAX_STK_COMMAND_RANGE         (0xFE)
#define TIMER_TYPE_STK_TONE_DEF_VALUE (500)
#define IMAGE_REC_MAX_VALUE           (255)
#define MAX_FILE_OFFSET               (255)

#define MAX_CHARS_DOWN_DISPLAY         (31)
#define MAX_CHARS_ACROSS_DISPLAY       (127)
#define MAX_MENU_WIDTH_REDUCTION       (7)
/***************************************************************************
 * Local Functions
 ***************************************************************************/


/*--------------------------------------------------------------------------
*
* Function:    vgStkProcessMmiProfile
*
* Parameters:  commandBuffer_p  - pointer to command line string
*              mmiProfile_p     - MMI STK support profile
*              result           - new profile accepted
*
* Returns:     nothing*
* Description: This routine decodes the MMI profile data.  This data is a string
*              of hex digits of length profileLength.
*
*-------------------------------------------------------------------------*/

static void vgStkProcessMmiProfile (CommandLine_t          *commandBuffer_p,
                                    SimatMmiSupportProfile *mmiProfile_p,
                                    Boolean                *result)
{


  Int32    profileLength = 0;
  Int8     profiles;
  Int8     rawProfileData [MAX_PROFILE_LENGTH];
  Int8     lsbHexValue;
  Int8     msbHexValue;

  *result = TRUE;

  for (profiles = 0; profiles < MAX_PROFILE_LENGTH ; profiles++)
  {
    rawProfileData [profiles] = 0x00;
  }

  if (getExtendedParameter (commandBuffer_p, &profileLength, ULONG_MAX) == TRUE)
  {
    if ( profileLength > MAX_PROFILE_LENGTH)
    {
      profileLength = MAX_PROFILE_LENGTH;
    }
    if (profileLength > 0)     
    {
      for (profiles = 0; (Int32)profiles < profileLength; profiles++)
      {
        /* parse byte at a time */
        if (commandBuffer_p->position + 2 < commandBuffer_p->length)
        {
          if ((getHexaValue (commandBuffer_p->character[commandBuffer_p->position++], &msbHexValue)) &&
              (getHexaValue (commandBuffer_p->character[commandBuffer_p->position++], &lsbHexValue)))
          {
            rawProfileData [profiles] = (16 * msbHexValue) + lsbHexValue;
          }
        } /* ignore any missing or invalid data */
      }
    } /* if the profile length was 0 then the profile should be empty  - i.e stk not to be supported */
  }
  else
  {
    *result = FALSE;
  }

  /* extracted data from the command string so now setup the data rec */
  if (*result == TRUE)
  {
    /* now set up the mmiProfile_p struct */
    memset (mmiProfile_p, 0, sizeof(SimatMmiSupportProfile));
    if ( profileLength >0 )
    {
       mmiProfile_p->stkSupported = TRUE;
       vgStkSetUpMmiProfile (profileLength, rawProfileData, mmiProfile_p);
    }
  }
}


/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
*
* Function:    vgStkMSTGC
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This command is received from the accessory and is an
*              indication to the user that the accessory is interested in the
*              procactive indication generated by the sim.  If the command id
*              supplied by the sim is ok and the data is valid then the data
*              is sent to the accessory.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgStkMSTGC (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{

  ResultCode_t  result = RESULT_CODE_ERROR;
  Int32         cmdId;

  if (vgOpManCheckStkRegistration (entity) == TRUE)
  {
    if (vgOpManStkOnLine (entity) == TRUE)
    {
      switch (getExtendedOperation (commandBuffer_p))
      {
        case EXTENDED_ASSIGN:   /* AT*MSTGC=  */
        {
          if (getExtendedParameter (commandBuffer_p, &cmdId, ULONG_MAX) == TRUE)
          {
            if ((cmdId > 0) &&
                (cmdId <= MAX_STK_COMMAND_RANGE))
            {
              if ((cmdId == (Int32)vgStkGetCmdId ()) &&
                  (vgStkIsDataValid () == TRUE))
              {
                vgStkSendData ();
                result = RESULT_CODE_OK;
              }
              else
              {
                result = VG_CME_STK_INVALID_COMMAND_ID;
              }
            }
          }
          break;
        }
        default:
        {
          break;
        }
      }
    }
    else
    {
      result = VG_CME_STK_NOT_ENABLED;
    }
  }
  else
  {
    result = VG_CME_STK_ALREADY_IN_USE;
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgStkMSTCR
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This command is received from the accessory and is an
*              indication that the accessory has received the data string.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgStkMSTCR (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
  ResultCode_t  result = RESULT_CODE_ERROR;
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  if (vgOpManCheckStkRegistration (entity) == TRUE)
  {
    if (vgOpManStkOnLine (entity) == TRUE)
    {
      if (vgStkIsDataValid () == TRUE)
      {
        result = vgStkCreateSendRspSignal (commandBuffer_p, FALSE, entity);

        if (result == RESULT_CODE_OK)
        {
          vgCiStopTimer (TIMER_TYPE_STK_CNF_CONFIRM);
          vgStkSetDataToInValid ();
          stkGenericContext_p->cmdId = 0; /* reset the command id as this is finished now */
        }
        /* otherwise the error will be reported and the correct command with format should be sent */
      }
    }
    else
    {
      result = VG_CME_STK_NOT_ENABLED;
    }
  }
  else
  {
    result = VG_CME_STK_ALREADY_IN_USE;
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgStkMSTPD
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: MMI profile download.  This setting does not take effect until there
has been a reset or if this is sent prior to PIN being entered.
*-------------------------------------------------------------------------*/

ResultCode_t vgStkMSTPD (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();
  ResultCode_t                   result = RESULT_CODE_ERROR;
  Boolean                        decodeStatus = FALSE;
  SimatMmiSupportProfile         mmiProfile;
  SignalBuffer                   sigToSend = kiNullBuffer;
  AfsaMmiProfileUpdateReq        *sig_p;
  
  if (vgOpManCheckStkRegistration (entity) == TRUE)
  {
    if (vgOpManStkOnLine (entity) == TRUE)
    {
      switch (getExtendedOperation (commandBuffer_p))
      {
        case EXTENDED_ASSIGN:   /* AT*MSTPD=  */
        {
          vgStkProcessMmiProfile (commandBuffer_p, &mmiProfile,
                                  &decodeStatus);
          if (decodeStatus == FALSE)
          {
            result = RESULT_CODE_ERROR;
          }
          else
          {
            sendSsRegistrationSignal (SIM_TOOLKIT, entity,
                                      SIG_AFSA_MMI_PROFILE_UPDATE_CNF);
            /* send signal to SIMAT task containing this information */
            KiCreateZeroSignal( SIG_AFSA_MMI_PROFILE_UPDATE_REQ,  
                                sizeof (AfsaMmiProfileUpdateReq), &sigToSend); 
            sig_p = (AfsaMmiProfileUpdateReq *)sigToSend.sig;
            sig_p->taskId = VG_CI_TASK_ID;
            sig_p->mmiSupportProfile = mmiProfile; 
            /* only update the current profile which might have been stored in the CI 
             * with this profile if it is before the PIN has been  entered */
            if ( stkGenericContext_p->simHasPassedPinCheck == FALSE ) 
            {
              if (stkGenericContext_p->currentMmiProfile_p == PNULL)
              {
                KiAllocMemory (sizeof (SimatMmiSupportProfile), 
                              (void **)&(stkGenericContext_p->currentMmiProfile_p));
              }
              memcpy (stkGenericContext_p->currentMmiProfile_p, &mmiProfile, 
                      sizeof(SimatMmiSupportProfile));
            }
            KiSendSignal (SIMAT_TASK_ID, &sigToSend);
            result = RESULT_CODE_PROCEEDING;                          
          }
          break;
        }
        case EXTENDED_QUERY:  /* AT*MSTPD? */
         /* if we haven't already read this send a request to read the current MMi Profile as downloaded to the SIM */
        {
          if (stkGenericContext_p->currentMmiProfile_p == PNULL)
          {
            sendSsRegistrationSignal (SIM_TOOLKIT, entity,
                                      SIG_AFSA_READ_MMI_PROFILE_CNF);
            /* send signal to SIMAT task containing this information */
            KiCreateZeroSignal( SIG_AFSA_READ_MMI_PROFILE_REQ,  
                                sizeof (AfsaReadMmiProfileReq), &sigToSend); 
            sigToSend.sig->afsaReadMmiProfileReq.taskId = VG_CI_TASK_ID;
            KiSendSignal (SIMAT_TASK_ID, &sigToSend);
            result = RESULT_CODE_PROCEEDING;   
          }
          else
          {
            result = vgStkDisplayCurrentMmiProfile ( entity );
          }
          break;
        }

        default:
        {
          result = RESULT_CODE_ERROR;
          break;
        }
      }
    }
  } /* entity cross check */
  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgStkMSTEV
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This command is received from the accessory and is an
*              indication to voyager that the accessory has performed an
*              MMI event.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgStkMSTEV (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
  enum PossibleEventsTag
  {
    USER_ACTIVITY_EVENT       = 0x05,
    IDLE_SCREEN_EVENT         = 0x06,
    LANGUAGE_SELECTION_EVENT  = 0x08,
    BROWSER_TERMINATION_EVENT = 0x09,
    DISPLAY_PARAMS_EVENT      = 0x0c,
  };
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();
  ResultCode_t  result = RESULT_CODE_OK;
  Int8          thisEvent;
  Int8          langBuffer [SIMAT_NUM_LANG_CODE_CHARS + NULL_TERMINATOR_LENGTH] = {0};
  Int16         extendedStrLen = 0;
  SignalBuffer  eventSignal = kiNullBuffer;
  Int8          lsbHexValue = 0;
  Int8          msbHexValue = 0;

  if (vgOpManCheckStkRegistration (entity) == TRUE)
  {
    if (vgOpManStkOnLine (entity) == TRUE)
    {
      switch (getExtendedOperation (commandBuffer_p))
      {
        case EXTENDED_RANGE:  /* *MSTEV=?  */
        {
          vgPutNewLine (entity);

          vgPrintf (entity, (const Char*)"%C= <05> - User Activity Event");
          vgPutNewLine (entity);
          vgPrintf (entity, (const Char*)"%C= <06> - Idle Screen Event");
          vgPutNewLine (entity);
          vgPrintf (entity, (const Char*)"%C= <08>,<00-99> - Language Selection Event");
          vgPutNewLine (entity);
          vgPrintf (entity, (const Char*)"%C= <09> - Browser Termination Event");
          vgPutNewLine (entity);
          vgPrintf (entity, (const Char*)"%C= <0C> - Display Parameters Changed");
          vgPutNewLine (entity);
          break;
        }
        case EXTENDED_ASSIGN: /* *MSTEV=   */
        {
          if ((commandBuffer_p->position < commandBuffer_p->length) &&
              (getHexaValue (commandBuffer_p->character[commandBuffer_p->position++], &msbHexValue)))
          {
            if ((commandBuffer_p->position < commandBuffer_p->length) &&
                (getHexaValue (commandBuffer_p->character[commandBuffer_p->position++], &lsbHexValue)))
            {
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

          if (result == RESULT_CODE_OK)
          {
            thisEvent = (16 * msbHexValue) + lsbHexValue;

            switch (thisEvent)
            {
              case USER_ACTIVITY_EVENT:
              {
                if ( stkGenericContext_p->currentEventList.userActivity == FALSE )
                {
                  result = RESULT_CODE_ERROR;

                }
                else
                {
                  KiCreateZeroSignal (SIG_AFSA_USER_ACTIVITY_IND,
                                      sizeof (EmptySignal),
                                      &eventSignal);

                  KiSendSignal (SIMAT_TASK_ID, &eventSignal);
                  /* this event should only be monitored for once */
                  stkGenericContext_p->currentEventList.userActivity = FALSE;
                }
                break;
              }
              case IDLE_SCREEN_EVENT:
              {
                if ( stkGenericContext_p->currentEventList.idleScreenAvailable == FALSE )
                {
                  result = RESULT_CODE_ERROR;
                }
                else
                {                
                  KiCreateZeroSignal (SIG_AFSA_DISPLAY_STATUS_IND,
                                      sizeof (AfsaDisplayStatusInd),
                                      &eventSignal);

                  eventSignal.sig -> afsaDisplayStatusInd.simatMMIState = SIMAT_MMI_IDLE;
                  /* this event should only be monitored for once */
                  stkGenericContext_p->currentEventList.idleScreenAvailable = FALSE;
                  KiSendSignal (SIMAT_TASK_ID, &eventSignal);
                }
                break;
              }
              case LANGUAGE_SELECTION_EVENT:
              {
                if ( stkGenericContext_p->currentEventList.languageSelection == FALSE )
                {
                  result = RESULT_CODE_ERROR;
                }
                else
                {                
                  commandBuffer_p->position++;
                  if (getExtendedString (commandBuffer_p,
                                         &langBuffer[0],
                                         SIMAT_NUM_LANG_CODE_CHARS,
                                         &extendedStrLen) == TRUE)
                  { 
                    if (strncmp ((char*)langBuffer, "", strlen ((char*)langBuffer)) == 0)
                    {
                      result = VG_CME_INVALID_TEXT_CHARS;
                    }
                    else
                    {
                      KiCreateZeroSignal (SIG_AFSA_LANGUAGE_SELECTION_IND,
                                          sizeof (AfsaLanguageSelectionInd),
                                          &eventSignal);

                      memcpy(eventSignal.sig -> afsaLanguageSelectionInd.languageCode,
                             langBuffer,                                  
                             sizeof(Int8) * SIMAT_NUM_LANG_CODE_CHARS);   

                      KiSendSignal (SIMAT_TASK_ID, &eventSignal);
                    } 
                  }
                  else
                  {
                    result = VG_CME_INVALID_TEXT_CHARS;
                  } 
                }
                break;
              }
              case BROWSER_TERMINATION_EVENT:
              {
                if ( stkGenericContext_p->currentEventList.browserTermination == FALSE )
                {
                  result = RESULT_CODE_ERROR;
                }
                else
                {                
                  KiCreateZeroSignal (SIG_AFSA_CLOSE_BROWSER_IND,
                                      sizeof (AfsaCloseBrowserInd),
                                      &eventSignal);

                  eventSignal.sig -> afsaCloseBrowserInd.userTermination = TRUE;

                  KiSendSignal (SIMAT_TASK_ID, &eventSignal);
                }
                break;
              }
              case DISPLAY_PARAMS_EVENT:
              {
                if ( stkGenericContext_p->currentEventList.displayParamsChanged == FALSE )
                {
                  result = RESULT_CODE_ERROR;
                }
                else
                {                
                  KiCreateZeroSignal (SIG_AFSA_DISPLAY_PARAMS_CHANGED_IND,
                                      sizeof (AfsaDisplayParamsChangedInd),
                                      &eventSignal);
                  /* need to get the display parameters from the input string*/
                  result = vgStkReadDisplayParameters (commandBuffer_p, &eventSignal);
                  if ( result == RESULT_CODE_OK )
                  {
                    KiSendSignal (SIMAT_TASK_ID, &eventSignal);
                  }
                  else
                  {
                    KiDestroySignal(&eventSignal);
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
          }
          break;
        }
        default:
        {
          result = RESULT_CODE_ERROR;
          break;
        }
      }
    }
    else
    {
      result = VG_CME_STK_NOT_ENABLED;
    }
  }
  else
  {
    result = VG_CME_STK_ALREADY_IN_USE;
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgStkMSTMS
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This command is received from the accessory and is an
*              indication of the menu item selected from the main menu.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgStkMSTMS (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
  ResultCode_t            result = RESULT_CODE_OK;
  Int32                   itemId;
  Int32                   helpItemId = (Int32)0;
  SignalBuffer            sendSignal = kiNullBuffer;
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  if (vgOpManCheckStkRegistration (entity) == TRUE)
  {
    if (vgOpManStkOnLine (entity) == TRUE)
    {
      switch (getExtendedOperation (commandBuffer_p))
      {
        case EXTENDED_RANGE:  /* *MSTMS=?  */
        {
          if (stkGenericContext_p->currentMenuItems == 0)
          {
            result = VG_CME_STK_NO_MENU_AVAILABLE;
          }
          else
          {
            vgPutNewLine (entity);
            /* job102163: update Item Id in *MSTMS=? range check */
            vgPrintf     (entity, (const Char*)"%C: (%u-%u),(0-1)",
                           stkGenericContext_p->firstMenuItemId,
                           stkGenericContext_p->firstMenuItemId +
                             (stkGenericContext_p->currentMenuItems - 1) );
            vgPutNewLine (entity);
          }
          break;
        }
        case EXTENDED_ASSIGN: /* *MSTMS=<i>,<h> */
        {
          if (stkGenericContext_p->menuHasBeenRemoved == TRUE)
          { /* extracted from the set up menu indication in vgstkrd.c
               the set up menu indication may ask for a menu to be removed */
            result = VG_CME_STK_NO_MENU_AVAILABLE;
          }
          else if (stkGenericContext_p->proactiveSessionStarted == TRUE)
          { /* shouldn't try to start another session when one is already running */
            result = VG_CME_STK_ALREADY_IN_USE;
          }
          else  
          {
            /* get choosen item */
            if (getExtendedParameter (commandBuffer_p,
                                       &itemId,
                                        ULONG_MAX) == TRUE)
            {
              /* job102163: modify check on item Id value - for TC 27.22.5.3 */
              if ( ( itemId == ULONG_MAX ) || ( !vgStkMenuItemPresent (itemId) ) )
              {
                result = VG_CME_INVALID_INDEX;
              }

              /* get help info */
              if (getExtendedParameter (commandBuffer_p,
                                         &helpItemId,
                                          ULONG_MAX) == TRUE)
              {
                if ((helpItemId > 1) &&
                    (helpItemId != ULONG_MAX))
                {
                  result = VG_CME_INVALID_INDEX;
                }
              }

              /* if all ok then set up signal data and send it */
              if (result == RESULT_CODE_OK)
              {
                KiCreateZeroSignal (SIG_AFSA_MENU_SELECTION_REQ,
                                     sizeof(AfsaMenuSelectionReq),
                                      &sendSignal);
                /* ChosenItem*/
                sendSignal.sig->afsaMenuSelectionReq.chosenItemId = (Int8)itemId;
                /* help info */
                sendSignal.sig->afsaMenuSelectionReq.helpInfoRequired =
                       (Boolean)(helpItemId == 1?TRUE:FALSE);

                /* Now send it */
                KiSendSignal (SIMAT_TASK_ID, &sendSignal);
              }
            }
            else
            {
              result = VG_CME_INVALID_TEXT_CHARS;
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
    }
    else
    {
      result = VG_CME_STK_NOT_ENABLED;
    }
  }
  else
  {
    result = VG_CME_STK_ALREADY_IN_USE;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkMSTICREC
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: Executes the +MSTICREC AT command which allows the MMI to request 
 * information concerning the Icon image records.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgStkMSTICREC (CommandLine_t * commandBuffer_p,
                            const VgmuxChannelNumber entity)
{
  ResultCode_t            result = RESULT_CODE_OK;
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();
  SignalBuffer            sendSignal = kiNullBuffer; 
  Int32                   recordNum; 

  if (vgOpManCheckStkRegistration (entity) == TRUE)
  {
    if (vgOpManStkOnLine (entity) == TRUE)
    {
      switch (getExtendedOperation (commandBuffer_p))
      {
        case EXTENDED_QUERY:    /* AT*MSTICREC? */
        {
          /* send the registration request off */
          sendSsRegistrationSignal (SIM_TOOLKIT,  /* just the stk sub-sys */
                                    stkGenericContext_p->registeredStkEntity,
                                    SIG_AFSA_LIST_IMAGE_REC_CNF);

          KiCreateZeroSignal (SIG_AFSA_LIST_IMAGE_REC_REQ,
                              sizeof(AfsaListImageRecReq),
                                      &sendSignal);
          sendSignal.sig->afsaListImageRecReq.taskId = VG_CI_TASK_ID;
          /* Now send it */
          KiSendSignal (SIMAT_TASK_ID, &sendSignal);
          result = RESULT_CODE_PROCEEDING;
          break;
        }
        case EXTENDED_ASSIGN:   /* AT*MSTICREC=  */ 
        {
          /* parse buffer for which record needs to be read */
          /* send the registration request off */
          if (getExtendedParameter (commandBuffer_p, &recordNum, IMAGE_REC_MAX_VALUE) == FALSE)
          {
            result = VG_CME_INVALID_INPUT_VALUE;
          }
          else
          {
            sendSsRegistrationSignal (SIM_TOOLKIT,  /* just the stk sub-sys */
                                      stkGenericContext_p->registeredStkEntity,
                                      SIG_AFSA_READ_IMAGE_REC_CNF);

            KiCreateZeroSignal (SIG_AFSA_READ_IMAGE_REC_REQ,
                                sizeof(AfsaReadImageRecReq),
                                       &sendSignal);
            sendSignal.sig->afsaReadImageRecReq.taskId = VG_CI_TASK_ID;
            sendSignal.sig->afsaReadImageRecReq.recordNum = (Int8)recordNum;
            /* Now send it */
            KiSendSignal (SIMAT_TASK_ID, &sendSignal);
            result = RESULT_CODE_PROCEEDING;
          }
          break;
        }
        default:
        {
          result = RESULT_CODE_ERROR;
          break;
        }
      }
    }
    else
    {
      result = VG_CME_STK_NOT_ENABLED;
    }
  }
  else
  {
    result = VG_CME_STK_ALREADY_IN_USE;
  }
  return(result);
}
/*--------------------------------------------------------------------------
 *
 * Function:    vgStkMSTICIMG
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: Executes the +MSTICIMG AT command which allows the MMI to request 
 * information concerning the Icon image instance data.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgStkMSTICIMG (CommandLine_t * commandBuffer_p,
                            const VgmuxChannelNumber entity)
{
  ResultCode_t            result = RESULT_CODE_OK;
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();
  SignalBuffer            sendSignal = kiNullBuffer; 
  Int16                   efId = 0;
  Int32                   offset = 0;
  Int32                   length = 0;
  Boolean                 tmpBool = FALSE;
  Int32                   tmpVal;
  if (vgOpManCheckStkRegistration (entity) == TRUE)
  {
    if (vgOpManStkOnLine (entity) == TRUE)
    {
      switch (getExtendedOperation (commandBuffer_p))
      {
        case EXTENDED_ASSIGN:   /* AT*MSTICIMG=  */ 
        {
          
          if (getExtendedHexParameter (commandBuffer_p, &tmpVal, ULONG_MAX, &tmpBool))
          {
            if (tmpBool)
            {
              efId = (Int16)tmpVal;
            }
            else
            {
              result = VG_CME_INVALID_INPUT_VALUE;
            }

            if (result == RESULT_CODE_OK)
            {
              if ((getExtendedParameter (commandBuffer_p,
                                          &offset, MAX_FILE_OFFSET) == TRUE) && 
                (getExtendedParameter (commandBuffer_p, &length, 0) == TRUE))
              { 
                sendSsRegistrationSignal (SIM_TOOLKIT,  /* just the stk sub-sys */
                                          stkGenericContext_p->registeredStkEntity,
                                          SIG_AFSA_READ_IMAGE_INST_DATA_CNF);

                KiCreateZeroSignal (SIG_AFSA_READ_IMAGE_INST_DATA_REQ,
                                    sizeof(AfsaReadImageInstDataReq),
                                    &sendSignal);
                sendSignal.sig->afsaReadImageInstDataReq.taskId = VG_CI_TASK_ID;
                sendSignal.sig->afsaReadImageInstDataReq.efId = efId;
                sendSignal.sig->afsaReadImageInstDataReq.offset = (Int16)offset;
                sendSignal.sig->afsaReadImageInstDataReq.length = (Int16)length;
                /* Now send it */
                KiSendSignal (SIMAT_TASK_ID, &sendSignal);
                result = RESULT_CODE_PROCEEDING;
              }
              else
              {
                result = RESULT_CODE_ERROR;
              }   
            }
          }
          else
          {
            result = RESULT_CODE_ERROR;
          }
          break;
        }    

        default:
        {
          result = RESULT_CODE_ERROR;
          break;
        }
      }
    }
    else
    {
       result = VG_CME_STK_NOT_ENABLED;
    }

  }
  else
  {
    result = VG_CME_STK_ALREADY_IN_USE;
  }
  return(result);
}    
/*--------------------------------------------------------------------------
 *
 * Function:    vgStkMSTMODE
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: Executes the +MSTMODE AT command which allows the user to
 *              configure STK output formatting:
 *              0 - PDU Mode
 *              1 - Text Mode
 *              2 - Use AT+CMGF Mode
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgStkMSTMODE (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity)
{
  ResultCode_t result  = RESULT_CODE_OK;
  Int32        newMode = STK_MODE_TEXT;

  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_RANGE:    /* AT*MSTMODE=? */
    {
      vgPutNewLine (entity);
      vgPrintf     (entity, (const Char*)"%C: (0-2)");
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_QUERY:    /* AT*MSTMODE?  */
    {
      vgPutNewLine (entity);
      vgPrintf     (entity, (const Char*)"%C: %u",
                    getProfileValue (entity, PROF_MSTMODE));
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_ASSIGN:   /* AT*MSTMODE=  */
    {
      /* extract STK mode parameter */
      if (getExtendedParameter (commandBuffer_p,
                                &newMode,
                                getProfileValue (entity, PROF_MSTMODE)))
      {
        /* check parameter has valid value */
        if ((newMode == STK_MODE_PDU)  ||
            (newMode == STK_MODE_TEXT) ||
            (newMode == STK_MODE_CMGF))
        {
          /* set up the active profile with the new STK mode */
          result = setProfileValue (entity, PROF_MSTMODE, (Int8)newMode);
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
    case EXTENDED_ACTION:   /* AT*MSTMODE   */
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
 * Function:    vgInitialiseStkAt
 *
 * Parameters:  none
 *
 * Returns:     nothing
 *
 * Description: Sets generic context to startup values
 *
 *-------------------------------------------------------------------------*/
void vgInitialiseStkAt (void)
{
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();

  stkGenericContext_p->vgToneIsCallTone = FALSE;
}

/*--------------------------------------------------------------------------
*
* Function:    vgStkMSTLOCK

*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This command is received from the accessory and locks SIM toolkit 
 * activity  - including unsolicited data to this Entity*
*-------------------------------------------------------------------------*/

ResultCode_t vgStkMSTLOCK (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity)
{
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();
  ResultCode_t             result              = RESULT_CODE_OK;
  Int32                  lockState             = 0;
  Int32                  timerPeriod           = 0;
  Boolean                restartTimer          = FALSE; 
       
  if (vgOpManCheckStkRegistration (entity) == TRUE)
  {
    switch (getExtendedOperation (commandBuffer_p))
    {
      case EXTENDED_RANGE:    /* AT*MSTLOCK=? */
      {
        vgPutNewLine (entity);
        vgPrintf     (entity, (const Char*)"%C: (0 - 1), ( 0 - %d )",(Int16)VG_STK_RESP_TIMER_MAX_VAL);
        vgPutNewLine (entity);
        break;
      }
      case EXTENDED_QUERY:    /* AT*MSTLOCK?  */
      {
        vgPutNewLine (entity);
        vgPrintf (entity, (const Char*) "*MSTLOCK: %d",
                  (Int16)stkGenericContext_p->registeredForStk[entity]);
        if ( stkGenericContext_p->responseTimerEnabled )
        {
          timerPeriod = getTimeOutPeriod (TIMER_TYPE_STK_CNF_CONFIRM);
          vgPrintf (entity, (const Char*) ", %d", (Int32)(timerPeriod /1000) );
        }
        else
        {
          vgPrintf (entity, (const Char*) ", 0");  
        }
        vgPutNewLine (entity);
        break;
      }
      case EXTENDED_ASSIGN:   /* AT*MSTLOCK=  */
      {
        if (getExtendedParameter (commandBuffer_p,
                                       &lockState,
                                        ULONG_MAX) == TRUE)
        {
          if (( lockState != TRUE ) && ( lockState != FALSE ))
          {
            result = VG_CME_INVALID_INPUT_VALUE;
          }

          /* if all ok then set up signal data and send it */
          if (result == RESULT_CODE_OK) 
          {
            result = vgStkHandleAccessoryStateChange (entity,
                                                          (Boolean)lockState);
          }
        }
        else
        {
          result = VG_CME_INVALID_TEXT_CHARS;
        }

        /* now check for any timer info */
        if (( result == RESULT_CODE_OK) || (result == RESULT_CODE_PROCEEDING))
        {
          if (getExtendedParameter (commandBuffer_p, &timerPeriod, 0) == TRUE)
          {
            if (timerPeriod > 0)  /* we are setting the timer to run */           
            {
              if ( timerPeriod <= VG_STK_RESP_TIMER_MAX_VAL )
              {
                if ( isTimerRunning (TIMER_TYPE_STK_CNF_CONFIRM) == TRUE )
                {
                  restartTimer = TRUE;
                  vgCiStopTimer (TIMER_TYPE_STK_CNF_CONFIRM);
                }
                setTimeOutPeriod ((Int32)timerPeriod * 1000, TIMER_TYPE_STK_CNF_CONFIRM);
                stkGenericContext_p->responseTimerEnabled = TRUE;
                if (( restartTimer ) || ( stkGenericContext_p->cmdId != 0))
                /* there is a command that is being processed so it should be timed */
                {
                  vgCiStartTimer (TIMER_TYPE_STK_CNF_CONFIRM, entity);
                }
              }
              else
              {
                result = RESULT_CODE_ERROR;
              }
            }
            else /* we are switching the timer off  - including the default of not making any value*/
            {
               /* if the timer is running then stop it as we aren't going to use it */
              if ( isTimerRunning (TIMER_TYPE_STK_CNF_CONFIRM) == TRUE )
              {
                vgCiStopTimer (TIMER_TYPE_STK_CNF_CONFIRM);
                
              }
              stkGenericContext_p->responseTimerEnabled = FALSE;
            }
          }
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
  }
  else
  {
    result = VG_CME_STK_ALREADY_IN_USE;
  }
  return (result);
}
/*--------------------------------------------------------------------------
 *
 * Function:    vgStkReadDisplayParameters
 *
 * Parameters:  CommandLine_t *commandBuffer_p
 *              SignalBuffer   *signalBuffer
 * Returns:     whether reading the display parameters was successful
 * Description: reads the Display parameters input when a DisplayParametersChanged
 * event has occurred.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgStkReadDisplayParameters (CommandLine_t *commandBuffer_p,
                                         SignalBuffer  *signalBuffer)
{
   ResultCode_t                   result              = RESULT_CODE_OK;
   AfsaDisplayParamsChangedInd    *sendSig_p          = PNULL;
   Int32                          charsDown           = 0;
   Int32                          charsAcross         = 0;
   Int32                          screenSizeSupport   = 0;
   Int32                          variableSizeFonts   = 0;
   Int32                          displayResize       = 0;
   Int32                          textWrapping        = 0;
   Int32                          textScrolling       = 0;
   Int32                          widthReduction      = 0;
   Boolean                        present             = FALSE;

   FatalAssert (*signalBuffer->type == SIG_AFSA_DISPLAY_PARAMS_CHANGED_IND);

   sendSig_p = &signalBuffer->sig->afsaDisplayParamsChangedInd;
   /* move past the event identifier in the command string */
   commandBuffer_p->position++;

   if((getExtendedParameterPresent( commandBuffer_p, &charsDown, ULONG_MAX, 
                 &present) == FALSE) || 
                 (( present == TRUE) && (charsDown > MAX_CHARS_DOWN_DISPLAY)))
   {
     result = VG_CME_INVALID_INPUT_VALUE;
   }
   else if( present == TRUE)
   {
     sendSig_p->paramValidity.numCharDownValid = TRUE;
     sendSig_p->newDispParams.numCharDownDisplay = (Int8)charsDown;
   }
 
   if ( result == RESULT_CODE_OK )
   {
     if((getExtendedParameterPresent( commandBuffer_p, &screenSizeSupport, ULONG_MAX, 
                 &present) == FALSE) || 
                 (( present == TRUE) && (screenSizeSupport > TRUE)))
     {
       result = VG_CME_INVALID_INPUT_VALUE;
     }
     else if( present == TRUE)
     {
       sendSig_p->paramValidity.screenSizingValid = TRUE;
       sendSig_p->newDispParams.screenSizingParamsSupported = (Boolean)screenSizeSupport;
     }
   }

   if ( result == RESULT_CODE_OK )
   {
     if((getExtendedParameterPresent( commandBuffer_p, &charsAcross, ULONG_MAX, 
                 &present) == FALSE) || 
                 (( present == TRUE) && (charsAcross > MAX_CHARS_ACROSS_DISPLAY)))
     {
       result = VG_CME_INVALID_INPUT_VALUE;
     }
     else if( present == TRUE)
     {
       sendSig_p->paramValidity.numCharAcrossValid = TRUE;
       sendSig_p->newDispParams.numCharAcrossDisplay = (Int8)charsAcross;
     }
   }   

   if ( result == RESULT_CODE_OK )
   {
     if((getExtendedParameterPresent( commandBuffer_p, &variableSizeFonts, ULONG_MAX, 
                 &present) == FALSE) || 
                 (( present == TRUE) &&(variableSizeFonts > TRUE)))
     {
       result = VG_CME_INVALID_INPUT_VALUE;
     }
     else if( present == TRUE)
     {
        sendSig_p->paramValidity.varSizeFontsValid = TRUE;
        sendSig_p->newDispParams.variableSizeFontsSupported = (Boolean)variableSizeFonts;
     }
   }

   if ( result == RESULT_CODE_OK )
   {
     if((getExtendedParameterPresent( commandBuffer_p, &displayResize, ULONG_MAX, 
                 &present) == FALSE) || 
                 (( present == TRUE) && (displayResize > TRUE)))
     {
       result = VG_CME_INVALID_INPUT_VALUE;
     }
     else if( present == TRUE)
     {
        sendSig_p->paramValidity.displayResizeValid = TRUE;
        sendSig_p->newDispParams.displayResizeSupported = (Boolean)displayResize;
     }
   }

   if ( result == RESULT_CODE_OK )
   {
     if((getExtendedParameterPresent( commandBuffer_p, &textWrapping, ULONG_MAX, 
                 &present) == FALSE) || 
                 (( present == TRUE) && (textWrapping > TRUE)))
     {
       result = VG_CME_INVALID_INPUT_VALUE;
     }
     else if( present == TRUE)
     {
        sendSig_p->paramValidity.textWrappingValid = TRUE;
        sendSig_p->newDispParams.textWrappingSupported = (Boolean)textWrapping;
     }
   }


   if ( result == RESULT_CODE_OK )
   {
     if((getExtendedParameterPresent( commandBuffer_p, &textScrolling, ULONG_MAX, 
                 &present) == FALSE) || 
                 (( present == TRUE) && (textScrolling > TRUE)))
     {
       result = VG_CME_INVALID_INPUT_VALUE;
     }
     else if( present == TRUE)
     {
        sendSig_p->paramValidity.textScrollingValid = TRUE;
        sendSig_p->newDispParams.textScrollingSupported = (Boolean)textScrolling;
     }
   }

   if ( result == RESULT_CODE_OK )
   {
     if((getExtendedParameterPresent( commandBuffer_p, &widthReduction, ULONG_MAX, 
                 &present) == FALSE) || 
                 (( present == TRUE) && (widthReduction > MAX_MENU_WIDTH_REDUCTION )))
     {
       result = VG_CME_INVALID_INPUT_VALUE;
     }
     else if( present == TRUE)
     {
       sendSig_p->paramValidity.widthReductionValid = TRUE;
       sendSig_p->newDispParams.widthReductionInMenu = (Int8)widthReduction;
     }
   } 
   return (result); 
}

/* END OF FILE */

