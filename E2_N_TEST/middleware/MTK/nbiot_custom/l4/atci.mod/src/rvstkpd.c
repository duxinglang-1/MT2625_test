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
 *  File Description :                                                   */
/** \file
 SIM toolkit Profile download encode and decode - MMI specifics
 **************************************************************************/

#define MODULE_NAME "RVSTKPD"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#if !defined (SYSTEM_H)
#  include <system.h>
#endif


#include <gkisig.h>
#include <gkimem.h>
#if !defined (RVSTKPD_H)
#  include <rvstkpd.h>
#endif
#if !defined (AFSA_SIG_H)
#  include <afsa_sig.h>
#endif
#if !defined (ABST_SIG_H)
#  include <abst_sig.h>
#endif
#if !defined (RVUTIL_H)
#  include <rvutil.h>
#endif
#if !defined (RVDATA_H)
#  include <rvdata.h>
#endif
#if !defined (RVOMAN_H)
#  include <rvoman.h>
#endif
#if !defined (RVCRHAND_H)
#  include <rvcrhand.h>
#endif

/***************************************************************************
 * Type Definitions
 ***************************************************************************/
 /* profile Byte 1 */
#define MMI_PROFILE_DOWNLOAD_BIT      (0x01) 
#define MMI_SMS_PP_DOWNLOAD_2_BIT     (0x02)
#define MMI_CD_DOWNLOAD_BIT           (0x04)
#define MMI_MENU_SELECTION_BIT        (0x08)
#define MMI_SMS_PP_DOWNLOAD_5_BIT     (0x10)
#define MMI_TIMER_EXPIRATION_BIT      (0x20)
#define MMI_USSD_CALL_CONTROL_BIT     (0x40)
#define MMI_CALL_CONTROL_8_BIT        (0x80)

/* profile byte 2 */
#define MMI_COMMAND_RESULT_BIT        (0x01)
#define MMI_CALL_CONTROL_2_BIT        (0x02)
#define MMI_CALL_CONTROL_3_BIT        (0x04)
#define MMI_MO_SMS_CALL_CONTROL_BIT   (0x08)
#define MMI_CALL_CONTROL_5_BIT        (0x10)
#define MMI_UCS2_ENTRY_BIT            (0x20)
#define MMI_UCS2_DISPLAY_BIT          (0x40)
#define MMI_DISPLAY_TEXT_8_BIT        (0x80)

/* profile byte 3 */
#define MMI_DISPLAY_TEXT_1_BIT        (0x01)
#define MMI_GET_INKEY_2_BIT             (0x02)
#define MMI_GET_INPUT_BIT             (0x04)
#define MMI_MORE_TIME_BIT             (0x08)
#define MMI_PLAY_TONE_BIT             (0x10)
#define MMI_POLL_INTERVAL_BIT         (0x20)
#define MMI_POLLING_OFF_BIT           (0x40)  
#define MMI_REFRESH_BIT               (0x80)

/* profile byte 4 */
#define MMI_SELECT_ITEM_BIT           (0x01)
#define MMI_SEND_SMS_3GPP_BIT         (0x02)
#define MMI_SEND_SS_BIT               (0x04)
#define MMI_SEND_USSD_BIT             (0x08)
#define MMI_SETUP_CALL_BIT            (0x10)
#define MMI_SETUP_MENU_BIT            (0x20)
#define MMI_PROVIDE_LOCAL_INFO_LAI_BIT (0x40)
#define MMI_PROVIDE_LOCAL_INFO_NMR_8_BIT (0x80)

/* profile byte 5 */
#define MMI_SETUP_EVENT_LIST_BIT      (0x01)  
#define MMI_EVENT_MT_CALL_BIT         (0x02)  
#define MMI_EVENT_CALL_CONNECTED_BIT  (0x04)
#define MMI_EVENT_CALL_DISCONNECTED_BIT (0x08)
#define MMI_EVENT_LOCATION_STATUS_BIT (0x10) 
#define MMI_EVENT_USER_ACT_BIT        (0x20)
#define MMI_EVENT_IDLE_SCRN_BIT       (0x40)
#define MMI_EVENT_CARD_READER_STATUS_BIT (0x80)

/* profile byte 6 */
#define MMI_EVENT_LANGUAGE_SELECTION_BIT (0x01)
#define MMI_EVENT_BROWSER_TERMINATION_BIT (0x02)
#define MMI_EVENT_DATA_AVAILABLE_BIT  (0x04)
#define MMI_EVENT_CHANNEL_STATUS_BIT  (0x08)
#define MMI_EVENT_ACT_CHANGE_BIT      (0x10)
#define MMI_EVENT_DISPLAY_CHANGE_BIT  (0x20)
#define MMI_EVENT_LOCAL_CONN_BIT      (0x40)
#define MMI_EVENT_SRCH_MODE_CHANGE_BIT (0x80)

/* profile byte 7 */
#define MMI_POWER_ON_CARD_BIT         (0x01)
#define MMI_POWER_OFF_CARD_BIT        (0x02)
#define MMI_PERFORM_CARD_APDU_BIT     (0x04)
#define MMI_GET_READER_STATUS_BIT     (0x08)
#define MMI_GET_READER_IDENTIFIER_BIT (0x10)
#define MMI_REU_6_TO_8_MASK           (0xe0)

/* profile byte 8 */
#define MMI_TIMER_MANAGE_START_STOP_BIT (0x01)
#define MMI_TIMER_MANAGE_VALUE_BIT    (0x02)
#define MMI_PROVIDE_LOCAL_INFO_DATE_BIT (0x04)
#define MMI_GET_INKEY_4_BIT             (0x08)
#define MMI_SETUP_IDLE_MODE_TEXT_BIT    (0x10)
#define MMI_RUN_AT_COMMAND_BIT          (0x20)
#define MMI_SETUP_CALL_7_BIT            (0x40)
/* CALL CONTROL BIT is the same as the one in the byte 1 */

/* profile byte 9 */
/* DISPALY TEST BIT is the same as the one in the byte 3 */
#define MMI_SEND_DTMF_CMD_BIT         (0x02)
#define MMI_PROVIDE_LOCAL_INFO_NMR_3_BIT (0x04)
#define MMI_PROVIDE_LOCAL_INFO_LANGUAGE_BIT (0x08)
#define MMI_PROVIDE_LOCAL_INFO_TIMING_BIT   (0x10)
#define MMI_LANGUAGE_NOTIFICATION_BIT       (0x20)
#define MMI_LAUNCH_BROWSER_BIT              (0x40)
#define MMI_PROVIDE_LOCAL_INFO_ACT_BIT      (0x80)

/* profile byte 10 */
#define MMI_SLCT_ITEM_SOFTKEYS_BIT    (0x01)
#define MMI_MENU_SOFTKEYS_BIT         (0x02)
#define MMI_RFU_3_TO_8_MASK           (0xfc)  

/* profile byte 11 */
#define MMI_NUM_SOFTKEYS_AVAIL_MASK   (0xff)

/* profile byte 12 */
#define MMI_OPEN_CHANNEL_BIT          (0x01)
#define MMI_CLOSE_CHANNEL_BIT         (0x02)
#define MMI_RECEIVE_DATA_BIT          (0x04)
#define MMI_SEND_DATA_BIT             (0x08)
#define MMI_GET_CHANNEL_STATUS_BIT    (0x10)
#define MMI_SERVICE_SEARCH_BIT        (0x20)
#define MMI_GET_SERVICE_INFO_BIT      (0x40)
#define MMI_DECLARE_SERVICE_BIT       (0x80)

/* profile byte 13 */
#define MMI_CSD_SUPPORT_BIT           (0x01)
#define MMI_GPRS_SUPPORT_BIT          (0x02)
#define MMI_BLUETOOTH_SUPPORT_BIT     (0x04)
#define MMI_IRDA_SUPPORT_BIT          (0x08)
#define MMI_RS232_SUPPORT_BIT         (0x10)
#define MMI_NUM_SUPPORT_CHANNEL_MASK  (0xe0)

/* profile byte 14 */
#define MMI_CHARS_DOWN_MASK           (0x1F)
#define MMI_NO_DISPLAY_CAP_BIT        (0x20)
#define MMI_NO_KEYPAD_AVAIL_BIT       (0x40)
#define MMI_SCREEN_SIZING_BIT         (0x80)

/* profile byte 15 */
#define MMI_NUM_CHARS_ACROSS_MASK     (0x7F)
#define MMI_VARIABLE_FONTS_BIT        (0x80)

/* profile byte 16 */
#define MMI_DISPLAY_RESIZE_BIT        (0x01)
#define MMI_TEXT_WRAP_BIT             (0x02)
#define MMI_TEXT_SCROLL_BIT           (0x04)
#define MMI_TEXT_ATTRIBUTES_BIT       (0x08)
#define MMI_RFU_5_BIT                 (0x10)
#define MMI_WIDTH_REDUCTION_MASK      (0xe0)

/* profile byte 17 */
#define MMI_TCP_CLIENT_MODE_REMOTE_BIT (0x01)
#define MMI_UDP_CLIENT_MODE_REMOTE_BIT (0x02)
#define MMI_TCP_SERVER_MODE_BIT        (0x04)
#define MMI_TCP_CLIENT_MODE_LOCAL_BIT  (0x08)
#define MMI_UDP_CLIENT_MODE_LOCAL_BIT  (0x10)
#define MMI_RFU_6_BIT                  (0x20)
#define MMI_E_UTRAN_SUPPORT_BIT        (0x40)
#define MMI_HSDPA_SUPPORT_BIT          (0x80)

/* profile byte 18 */
#define MMI_DISPLAY_TIMEOUT_BIT        (0x01)
#define MMI_INKEY_HELP_BIT             (0x02)
#define MMI_USB_SUPPORT_BIT            (0x04)
#define MMI_INKEY_TIMEOUT_BIT          (0x08)
#define MMI_PROVIDE_LOCAL_INFO_ESN_BIT (0x10)
#define MMI_CALL_CONTROL_GPRS_BIT      (0x20)
#define MMI_PROVIDE_LOCAL_INFO_IMEISV_BIT (0x40)
#define MMI_PROVIDE_LOCAL_INFO_SRCH_MODE_BIT (0x80)

/* profile byte 19 */
#define MMI_PROTOCOL_VERSION_SUPPORT_MASK (0x0f)
#define MMI_RFU_5_TO_8_MASK               (0xf0)

/* profile byte 20 */
#define MMI_RESERVED_MASK                 (0xff)

/* profile byte 21 */
#define MMI_WML_SUPPORT_BIT               (0x01)
#define MMI_XHTML_SUPPORT_BIT             (0x02)
#define MMI_HTML_SUPPORT_BIT              (0x04)
#define MMI_CHTML_SUPPORT_BIT             (0x08)
/* bit 5 to bit 8 are RFU */

/* profile byte 22 - byte 27 are not supported  */

/* profile byte 28 */
#define MMI_ALIGN_LEFT_BIT            (0x01)
#define MMI_ALIGN_CENTRE_BIT          (0x02)
#define MMI_ALIGN_RIGHT_BIT           (0x04)
#define MMI_FONT_SIZE_NORMAL_BIT      (0x08)
#define MMI_FONT_SIZE_LARGE_BIT       (0x10)
#define MMI_FONT_SIZE_SMALL_BIT       (0x20)
#define MMI_RFU_7_TO_8_MASK           (0xc0)

/* profile byte 29 */
#define MMI_STYLE_NORMAL_BIT            (0x01)
#define MMI_STYLE_BOLD_BIT              (0x02)
#define MMI_STYLE_ITALIC_BIT            (0x04)
#define MMI_STYLE_UNDERLINE_BIT         (0x08)
#define MMI_STYLE_STRIKETHROUGH_BIT     (0x10)
#define MMI_FOREGROUND_COLOUR_BIT       (0x20)
#define MMI_BACKGROUND_COLOUR_BIT       (0x40)
/* the 8 bit is RFU */

#define MMI_BYTE_1     0
#define MMI_BYTE_2     1
#define MMI_BYTE_3     2
#define MMI_BYTE_4     3
#define MMI_BYTE_5     4
#define MMI_BYTE_6     5
#define MMI_BYTE_7     6
#define MMI_BYTE_8     7
#define MMI_BYTE_9     8 
#define MMI_BYTE_10    9
#define MMI_BYTE_11    10
#define MMI_BYTE_12    11
#define MMI_BYTE_13    12
#define MMI_BYTE_14    13
#define MMI_BYTE_15    14
#define MMI_BYTE_16    15
#define MMI_BYTE_17    16
#define MMI_BYTE_18    17
#define MMI_BYTE_19    18 
#define MMI_BYTE_20    19
#define MMI_BYTE_21    20
#define MMI_BYTE_22    21
#define MMI_BYTE_23    22
#define MMI_BYTE_24    23
#define MMI_BYTE_25    24
#define MMI_BYTE_26    25
#define MMI_BYTE_27    26
#define MMI_BYTE_28    27
#define MMI_BYTE_29    28 
#define MMI_BYTE_30    29 

#define VG_MAX_PROFILE_LENGTH   30
/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  AfsaMmiProfileUpdateCnf      afsaMmiProfileUpdateCnf;  
  AfsaReadMmiProfileCnf        afsaReadMmiProfileCnf;
};

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkDisplayCurrentMmiProfile *
 * 
 * Parameters:  none 
 * Returns:     result code 
 *
 * Description: this signal indicates the success or failure of the update of the MMI 
 * profile to NVRAM settings.  This profile is not the current one downloaded to the 
 * SIM/UICC until there is a re-boot unless it is as a result of an update before the 
 * profile has been downloaded * i.e. before PIN has been verified. *
 *-------------------------------------------------------------------------*/

ResultCode_t vgStkDisplayCurrentMmiProfile (const VgmuxChannelNumber entity)
{
    StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();
    ResultCode_t            result;
    Int8                    mmiByte[VG_MAX_PROFILE_LENGTH];
    Int8                    profiles;
    Int8                    profileLength = VG_MAX_PROFILE_LENGTH ;

    memset(&mmiByte[0],0,VG_MAX_PROFILE_LENGTH);
    if ( stkGenericContext_p->currentMmiProfile_p != PNULL )
    {
      if ( stkGenericContext_p->currentMmiProfile_p->stkSupported == FALSE )
      {
         profileLength = 0;
      }
      else
      {
        for ( profiles = 0; profiles < VG_MAX_PROFILE_LENGTH; profiles++ )
        {
          switch (profiles)
          {
            case  MMI_BYTE_1:
            { 
              if (stkGenericContext_p->currentMmiProfile_p->menuSelection == TRUE)
              {
                mmiByte[profiles] |= MMI_MENU_SELECTION_BIT;
              }
              break;
            }       
            case  MMI_BYTE_2:
            {    
              if (stkGenericContext_p->currentMmiProfile_p->ucs2Entry == TRUE)
              {
                mmiByte[profiles] |= MMI_UCS2_ENTRY_BIT;
              }
              if (stkGenericContext_p->currentMmiProfile_p->ucs2Display == TRUE)
              {
                mmiByte[profiles] |= MMI_UCS2_DISPLAY_BIT;
              }
              if (stkGenericContext_p->currentMmiProfile_p->displayText == TRUE)
              {
                mmiByte[profiles] |= MMI_DISPLAY_TEXT_8_BIT;
              }                                
              break;
            }
            case  MMI_BYTE_3:
            {    
              if (stkGenericContext_p->currentMmiProfile_p->displayText == TRUE)
              {
                mmiByte[profiles] |= MMI_DISPLAY_TEXT_1_BIT;
              }                                
              if (stkGenericContext_p->currentMmiProfile_p->getInkey == TRUE)
              {
                mmiByte[profiles] |= MMI_GET_INKEY_2_BIT;
              }
              if (stkGenericContext_p->currentMmiProfile_p->getInput == TRUE)
              {
                mmiByte[profiles] |= MMI_GET_INPUT_BIT;
              }        
              if (stkGenericContext_p->currentMmiProfile_p->playTone == TRUE)
              {
                mmiByte[profiles] |= MMI_PLAY_TONE_BIT;
              }         
              break;
            }

            case  MMI_BYTE_4:
            {    
              if (stkGenericContext_p->currentMmiProfile_p->selectItem == TRUE)
              {
                mmiByte[profiles] |= MMI_SELECT_ITEM_BIT;
              }
              if (stkGenericContext_p->currentMmiProfile_p->setUpMenu == TRUE)
              {
                mmiByte[profiles] |= MMI_SETUP_MENU_BIT;
              }
              break;
            }

            case MMI_BYTE_5:
            {  

              if (stkGenericContext_p->currentMmiProfile_p->userActivity == TRUE)
              {
                mmiByte[profiles] |= MMI_EVENT_USER_ACT_BIT;
              }
              if (stkGenericContext_p->currentMmiProfile_p->idleScreenAvail == TRUE)
              {
                mmiByte[profiles] |= MMI_EVENT_IDLE_SCRN_BIT;
              }
              break;
            }

            case MMI_BYTE_6:
            {         
              if (stkGenericContext_p->currentMmiProfile_p->browserTermination == TRUE)
              {
                mmiByte[profiles] |= MMI_EVENT_BROWSER_TERMINATION_BIT;
              }                                
              if (stkGenericContext_p->currentMmiProfile_p->displayParamChange == TRUE)
              {
                mmiByte[profiles] |= MMI_EVENT_DISPLAY_CHANGE_BIT;
              }
              break;
            }

            case MMI_BYTE_7:
            case MMI_BYTE_12:
            case MMI_BYTE_13:
            case MMI_BYTE_17:  
            case MMI_BYTE_19:
            case MMI_BYTE_20:
            case MMI_BYTE_21:  
            case MMI_BYTE_22:
            case MMI_BYTE_23:
            case MMI_BYTE_24:  
            case MMI_BYTE_25:
            case MMI_BYTE_26:
            case MMI_BYTE_27:      
            case MMI_BYTE_30:
            {
              /* these bytes are not supported now */
              break;
            }

            case MMI_BYTE_8:
            {  
              if (stkGenericContext_p->currentMmiProfile_p->setUpIdleModeText == TRUE)
              {
                mmiByte[profiles] |= MMI_SETUP_IDLE_MODE_TEXT_BIT;
              }        
              break;
            }

            case MMI_BYTE_9:
            {  
              if (stkGenericContext_p->currentMmiProfile_p->displayText == TRUE)
              {
                mmiByte[profiles] |= MMI_DISPLAY_TEXT_1_BIT;
              }                                
              if (stkGenericContext_p->currentMmiProfile_p->launchBrowser == TRUE)
              {
                mmiByte[profiles] |= MMI_LAUNCH_BROWSER_BIT;
              }
              break;
            }
            
            case MMI_BYTE_10:
            {  
              if (stkGenericContext_p->currentMmiProfile_p->selectItemSoftKeys == TRUE)
              {
                mmiByte[profiles] |= MMI_SLCT_ITEM_SOFTKEYS_BIT;  
              }
              if (stkGenericContext_p->currentMmiProfile_p->setUpMenuSoftKeys == TRUE)
              {
                mmiByte[profiles] |= MMI_MENU_SOFTKEYS_BIT;
              }
              break;
            }
            
            case MMI_BYTE_11:
            {  
              mmiByte[profiles] = stkGenericContext_p->currentMmiProfile_p->numberOfSoftKeys;  
              break;
            }

            case MMI_BYTE_14:
            {  
              mmiByte[profiles] = (stkGenericContext_p->currentMmiProfile_p->numberOfCharsDown & MMI_CHARS_DOWN_MASK);
              if (stkGenericContext_p->currentMmiProfile_p->screenSizing == TRUE)
              {
                mmiByte[profiles] |= MMI_SCREEN_SIZING_BIT;
              }
              break;
            }

            case MMI_BYTE_15:
            {  
              mmiByte[profiles] = (stkGenericContext_p->currentMmiProfile_p->numberOfCharsAcross & MMI_NUM_CHARS_ACROSS_MASK);
              if (stkGenericContext_p->currentMmiProfile_p->variableSizeFonts == TRUE)
              {
                mmiByte[profiles] |= MMI_VARIABLE_FONTS_BIT;
              }                                
              break;
            }

            case MMI_BYTE_16:
            {  
              mmiByte[profiles] = (stkGenericContext_p->currentMmiProfile_p->maxWidthReduction & MMI_WIDTH_REDUCTION_MASK);
              if (stkGenericContext_p->currentMmiProfile_p->displayResize == TRUE)
              {
                mmiByte[profiles] |= MMI_DISPLAY_RESIZE_BIT;
              }
              if (stkGenericContext_p->currentMmiProfile_p->textWrapping == TRUE)
              {
                mmiByte[profiles] |= MMI_TEXT_WRAP_BIT;
              }        
              if (stkGenericContext_p->currentMmiProfile_p->textScrolling == TRUE)
              {
                mmiByte[profiles] |= MMI_TEXT_SCROLL_BIT;
              }         
              if (stkGenericContext_p->currentMmiProfile_p->textAttributes == TRUE)
              {
                mmiByte[profiles] |= MMI_TEXT_ATTRIBUTES_BIT;
              }
              break;
            }

            case MMI_BYTE_18:
            {  
              if (stkGenericContext_p->currentMmiProfile_p->displayTextVariable == TRUE)
              {
                mmiByte[profiles] |= MMI_DISPLAY_TIMEOUT_BIT;  
              }
              if (stkGenericContext_p->currentMmiProfile_p->getInkeyVariable == TRUE)
              {
                mmiByte[profiles] |= MMI_INKEY_TIMEOUT_BIT;
              }
              if (stkGenericContext_p->currentMmiProfile_p->getInkeyHelp == TRUE)
              {
                mmiByte[profiles] |= MMI_INKEY_HELP_BIT;
              }
              break;
            }   

            case MMI_BYTE_28:
            {  
              if (stkGenericContext_p->currentMmiProfile_p->alignLeft == TRUE)
              {
                mmiByte[profiles] |= MMI_ALIGN_LEFT_BIT;  
              }
              if (stkGenericContext_p->currentMmiProfile_p->alignCentre == TRUE)
              {
                mmiByte[profiles] |= MMI_ALIGN_CENTRE_BIT;
              }
              if (stkGenericContext_p->currentMmiProfile_p->alignRight == TRUE)
              {
                mmiByte[profiles] |= MMI_ALIGN_RIGHT_BIT;
              }
              if (stkGenericContext_p->currentMmiProfile_p->fontNormal == TRUE)
              {
                mmiByte[profiles] |= MMI_FONT_SIZE_NORMAL_BIT;
              }                                
              if (stkGenericContext_p->currentMmiProfile_p->fontLarge == TRUE)
              {
                mmiByte[profiles] |= MMI_FONT_SIZE_LARGE_BIT;
              }
              if (stkGenericContext_p->currentMmiProfile_p->fontSmall == TRUE)
              {
                mmiByte[profiles] |= MMI_FONT_SIZE_SMALL_BIT;
              }    
              break;
            }     

            case MMI_BYTE_29:
            {  
              if (stkGenericContext_p->currentMmiProfile_p->styleNormal == TRUE)
              {
                mmiByte[profiles] |= MMI_STYLE_NORMAL_BIT;  
              }
              if (stkGenericContext_p->currentMmiProfile_p->styleBold == TRUE)
              {
                mmiByte[profiles] |= MMI_STYLE_BOLD_BIT;
              }
              if (stkGenericContext_p->currentMmiProfile_p->styleItalic == TRUE)
              {
                mmiByte[profiles] |= MMI_STYLE_ITALIC_BIT;
              }
              if (stkGenericContext_p->currentMmiProfile_p->styleUnderline == TRUE)
              {
                mmiByte[profiles] |= MMI_STYLE_UNDERLINE_BIT;
              }                                
              if (stkGenericContext_p->currentMmiProfile_p->styleStrikethrough == TRUE)
              {
                mmiByte[profiles] |= MMI_STYLE_STRIKETHROUGH_BIT;
              }
              if (stkGenericContext_p->currentMmiProfile_p->foregroundColour == TRUE)
              {
                mmiByte[profiles] |= MMI_FOREGROUND_COLOUR_BIT;
              } 
              if (stkGenericContext_p->currentMmiProfile_p->backgroundColour == TRUE)
              {
                mmiByte[profiles] |= MMI_BACKGROUND_COLOUR_BIT;
              }        
              break;
            }            
            
            default:
             break;
          }
        }
        /* need to find how many bytes have content */
        profiles = VG_MAX_PROFILE_LENGTH -1 ;
        while ( profiles >0 )
        {
          if ( mmiByte[profiles] == 0 )   /* no content */
          {
             profileLength--;
             profiles --;
          }
          else  /* there is some content so all bytes up to here are valid */
          {
            profiles = 0;
          }
        }
      } /* else - stk is supported */
       
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char *)"MSTPD: %d",profileLength);
      if ( profileLength > 0 )
      {
        vgPrintf (entity, (const Char *)",");

        for ( profiles = 0; profiles < profileLength; profiles++)
        {
          vgPut8BitHex  ( entity, (Int8) mmiByte[profiles]); 
        }
        vgPutNewLine (entity);
      }
      result = RESULT_CODE_OK;
    }
    else
    {
      result = RESULT_CODE_ERROR;
    }
    return(result);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgStkAfsaMmiProfileUpdateCnf
 *
 * Parameters:  signalBuffer  *
 * Returns:     entity
 *
 * Description: this signal indicates the success or failure of the update of the MMI 
 * profile to NVRAM settings.  This profile is not the current one downloaded to the 
 * SIM/UICC until there is a re-boot unless it is as a result of an update before the 
 * profile has been downloaded * i.e. before PIN has been verified. *
 *-------------------------------------------------------------------------*/

void vgStkAfsaMmiProfileUpdateCnf (const SignalBuffer signalBuffer,
                                    const VgmuxChannelNumber entity)
{
  StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext ();
                                            
   if (signalBuffer.sig->afsaMmiProfileUpdateCnf.profileUpdated)
   {
     setResultCode (entity, RESULT_CODE_OK);
   }
   else
   {
     /* if the profile was not updated then clear the stored profile as it isn't current  */
     if ( stkGenericContext_p->currentMmiProfile_p != PNULL )
     {
        KiFreeMemory ((void **)& stkGenericContext_p->currentMmiProfile_p);
     }
     setResultCode ( entity, VG_CME_STK_PROFILE_NOT_UPDATED);
   }
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgStkAfsaReadMmiProfileCnf
 *
 * Parameters:  signalBuffer  *
 * Returns:     entity
 *
 * Description: this signal passes back the results of the request to read the
 * MMI profile
 * -------------------------------------------------------------------------*/

void vgStkAfsaReadMmiProfileCnf (const SignalBuffer signalBuffer,
                                   const VgmuxChannelNumber entity)
{
   StkEntityGenericData_t  *stkGenericContext_p = ptrToStkGenericContext (); 
                                            
   if (signalBuffer.sig->afsaReadMmiProfileCnf.supportProfileValid)
   {
      if ( stkGenericContext_p->currentMmiProfile_p == PNULL )
      {
         /* allocate the memory as we are in dynamic mode */
         KiAllocMemory (sizeof (SimatMmiSupportProfile), 
                     (void **)&(stkGenericContext_p->currentMmiProfile_p));

      }
      memcpy (stkGenericContext_p->currentMmiProfile_p,
              &signalBuffer.sig->afsaReadMmiProfileCnf.mmiSupportProfile,
              sizeof (SimatMmiSupportProfile));

      setResultCode( entity, vgStkDisplayCurrentMmiProfile (entity));
   }
   else
   {
      setResultCode ( entity, RESULT_CODE_ERROR);
   }
}

/*--------------------------------------------------------------------------
*
* Function:    vgStkSetUpMmiProfile
*
* Parameters:  length            - length of raw data
*              rawProfileData    - raw data to use
*              mmiProfile_p  - terminal profile
*
* Returns:     Nothing*
* Description: This routine is used to decode and pass on the mmi profile for SIM Toolkit 
 * 
*-------------------------------------------------------------------------*/

void vgStkSetUpMmiProfile (const Int32            length,
                           const Int8             *rawProfileData,
                           SimatMmiSupportProfile *mmiProfile_p)
{

  Int8     profiles;
  for (profiles = 0; (Int32)profiles < length; profiles++)
  {
    switch (profiles)
    {
      /* DOWNLOAD */  
      case MMI_BYTE_1: 
      {
        if (rawProfileData [profiles] & MMI_MENU_SELECTION_BIT)
        {
          (*mmiProfile_p).menuSelection  = TRUE;
        }
        /* the left bits are not supported */
        break;
      }

      /* OTHER  */  
      case MMI_BYTE_2 :
      {
        if (rawProfileData [profiles] & MMI_UCS2_ENTRY_BIT)
        {
          (*mmiProfile_p).ucs2Entry = TRUE;

        }
        if (rawProfileData [profiles] & MMI_UCS2_DISPLAY_BIT)
        {
          (*mmiProfile_p).ucs2Display = TRUE;

        }      
        if (rawProfileData [profiles] & MMI_DISPLAY_TEXT_8_BIT)
        {
          (*mmiProfile_p).displayText = TRUE;
        }
        /* the left bits are not supported */
        break;
      }

      /* Proactive UICC */
      case MMI_BYTE_3:
      {
        if (rawProfileData [profiles] & MMI_DISPLAY_TEXT_1_BIT)
        {
          (*mmiProfile_p).displayText = TRUE;
        }
        if (rawProfileData [profiles] & MMI_GET_INKEY_2_BIT)
        {
          (*mmiProfile_p).getInkey = TRUE;
        }
        if (rawProfileData [profiles] & MMI_GET_INPUT_BIT)
        {
          (*mmiProfile_p).getInput = TRUE;
        }
        if (rawProfileData [profiles] & MMI_PLAY_TONE_BIT)
        {
          (*mmiProfile_p).playTone = TRUE;
        }
        /* the left bits are not supported */
        break;
      }

      /* Proactive UICC */
      case MMI_BYTE_4:
      {
        if (rawProfileData [profiles] & MMI_SELECT_ITEM_BIT)
        {
          (*mmiProfile_p).selectItem = TRUE;
        }
        if (rawProfileData [profiles] & MMI_SETUP_MENU_BIT)
        {
          (*mmiProfile_p).setUpMenu = TRUE;
        }
        /* the left bits are not supported */
        break;
      }

      /* Event driven information */
      case MMI_BYTE_5:
      {
        if (rawProfileData [profiles] &MMI_EVENT_USER_ACT_BIT)
        {
          (*mmiProfile_p).userActivity = TRUE;
        }
        if (rawProfileData [profiles] & MMI_EVENT_IDLE_SCRN_BIT)
        {
          (*mmiProfile_p).idleScreenAvail = TRUE;
        }
        /* the left bits are not supported */
        break;
      }

      /* (Event driven information extensions */
      case MMI_BYTE_6:
      {
        if (rawProfileData [profiles] & MMI_EVENT_BROWSER_TERMINATION_BIT)
        {
          (*mmiProfile_p).browserTermination = TRUE;
        }
        if (rawProfileData [profiles] & MMI_EVENT_DISPLAY_CHANGE_BIT)
        {
          (*mmiProfile_p).displayParamChange = TRUE;
        }         
        /* the left bits are not supported */
        break;
      }

      /* Proactive UICC */
      case MMI_BYTE_8:
      {
        if (rawProfileData [profiles] & MMI_SETUP_IDLE_MODE_TEXT_BIT)
        {
          (*mmiProfile_p).setUpIdleModeText = TRUE;
        }
        /* the left bits are not supported */
        break;
      }
      
      case MMI_BYTE_9:
      {
        if (rawProfileData [profiles] & MMI_LAUNCH_BROWSER_BIT)
        {
          (*mmiProfile_p).launchBrowser = TRUE;
        }
        if (rawProfileData [profiles] & MMI_DISPLAY_TEXT_1_BIT)
        {
          (*mmiProfile_p).displayText = TRUE;
        }
        /* the left bits are not supported */
        break;
      } 

      /* Soft keys support */
      case MMI_BYTE_10:
      {
        if (rawProfileData [profiles] & MMI_SLCT_ITEM_SOFTKEYS_BIT)
        {
          (*mmiProfile_p).selectItemSoftKeys = TRUE;

        }
        if (rawProfileData [profiles] & MMI_MENU_SOFTKEYS_BIT)
        {
          (*mmiProfile_p).setUpMenuSoftKeys = TRUE;
        }
        break;
      }

      /* Soft keys information */
      case MMI_BYTE_11:
      {
        (*mmiProfile_p).numberOfSoftKeys = rawProfileData [profiles] & MMI_NUM_SOFTKEYS_AVAIL_MASK; 
        break;
      }      

      case MMI_BYTE_7:
      case MMI_BYTE_12:
      case MMI_BYTE_13:
      case MMI_BYTE_17:  
      case MMI_BYTE_19:
      case MMI_BYTE_20:
      case MMI_BYTE_21:  
      case MMI_BYTE_22:
      case MMI_BYTE_23:
      case MMI_BYTE_24:  
      case MMI_BYTE_25:
      case MMI_BYTE_26:
      case MMI_BYTE_27:      
      case MMI_BYTE_30:
      {
        /* these bytes are not supported now */
        break;
      }
      
      /* screen height byte */
      case MMI_BYTE_14:
      {
        (*mmiProfile_p).numberOfCharsDown = rawProfileData [profiles] & MMI_CHARS_DOWN_MASK;
        /* No display capability  and No keypad available are not supported */
        if (rawProfileData [profiles] & MMI_SCREEN_SIZING_BIT)
        {
          (*mmiProfile_p).screenSizing = TRUE;
        }
        break;
      }
      
      /* screen width byte */
      case MMI_BYTE_15:
      {
        (*mmiProfile_p).numberOfCharsAcross = rawProfileData [profiles] & MMI_NUM_CHARS_ACROSS_MASK;
        if (rawProfileData [profiles] & MMI_VARIABLE_FONTS_BIT)
        {
          (*mmiProfile_p).variableSizeFonts = TRUE;
        }
        break;
      }
      
      /* screen effects byte */
      case MMI_BYTE_16:
      {
        if (rawProfileData [profiles] & MMI_DISPLAY_RESIZE_BIT)
        {
          (*mmiProfile_p).displayResize = TRUE;
        }
        if (rawProfileData [profiles] & MMI_TEXT_WRAP_BIT)
        {
          (*mmiProfile_p).textWrapping = TRUE;
        }
        if (rawProfileData [profiles] & MMI_TEXT_SCROLL_BIT)
        {
          (*mmiProfile_p).textScrolling = TRUE;
        }
        if (rawProfileData [profiles] & MMI_TEXT_ATTRIBUTES_BIT)
        {
          (*mmiProfile_p).textAttributes = TRUE;
        }
        (*mmiProfile_p).maxWidthReduction = rawProfileData [profiles] & MMI_WIDTH_REDUCTION_MASK;
        break;
      }
      
      case MMI_BYTE_18:
      {
        if (rawProfileData [profiles] & MMI_DISPLAY_TIMEOUT_BIT)
        {
          (*mmiProfile_p).displayTextVariable = TRUE;
        }
        if (rawProfileData [profiles] &  MMI_INKEY_TIMEOUT_BIT)
        {
          (*mmiProfile_p).getInkeyVariable = TRUE;
        }
        if (rawProfileData [profiles] & MMI_INKEY_HELP_BIT)
        {
          (*mmiProfile_p).getInkeyHelp = TRUE;
        }
        /* the left bits are not supported */
        break;
      }

      /* Text attributes */
      case MMI_BYTE_28:
      {
        if (rawProfileData [profiles] & MMI_ALIGN_LEFT_BIT)
        {
          (*mmiProfile_p).alignLeft = TRUE;
        }
        if (rawProfileData [profiles] & MMI_ALIGN_CENTRE_BIT)
        {
          (*mmiProfile_p).alignCentre = TRUE;
        }
        if (rawProfileData [profiles] & MMI_ALIGN_RIGHT_BIT)
        {
          (*mmiProfile_p).alignRight = TRUE;
        }
        if (rawProfileData [profiles] & MMI_FONT_SIZE_NORMAL_BIT)
        {
          (*mmiProfile_p).fontNormal = TRUE;
        }
        if (rawProfileData [profiles] & MMI_FONT_SIZE_LARGE_BIT)
        {
          (*mmiProfile_p).fontLarge = TRUE;
        }
        if (rawProfileData [profiles] & MMI_FONT_SIZE_SMALL_BIT)
        {
          (*mmiProfile_p).fontSmall = TRUE;
        }
        break;
      }

       /* Text attributes */
      case MMI_BYTE_29:
      {
        if (rawProfileData [profiles] & MMI_STYLE_NORMAL_BIT)
        {
          (*mmiProfile_p).styleNormal = TRUE;
        }
        if (rawProfileData [profiles] & MMI_STYLE_BOLD_BIT)
        {
          (*mmiProfile_p).styleBold = TRUE;
        }
        if (rawProfileData [profiles] & MMI_STYLE_ITALIC_BIT)
        {
          (*mmiProfile_p).styleItalic = TRUE;
        }
        if (rawProfileData [profiles] & MMI_STYLE_UNDERLINE_BIT)
        {
          (*mmiProfile_p).styleUnderline = TRUE;
        }
        if (rawProfileData [profiles] & MMI_STYLE_STRIKETHROUGH_BIT)
        {
          (*mmiProfile_p).styleStrikethrough = TRUE;
        }
        if (rawProfileData [profiles] & MMI_FOREGROUND_COLOUR_BIT)
        {
          (*mmiProfile_p).foregroundColour = TRUE;
        }
        if (rawProfileData [profiles] & MMI_BACKGROUND_COLOUR_BIT)
        {
          (*mmiProfile_p).backgroundColour = TRUE;
        }
        break;
      }
      
      default:
      {
        break;
      }
    } /* end switch */
  }
}

/* END OF FILE */

