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
 **************************************************************************/
/** \file
 *      types for application layer - sim manager interface for SIM Application
 *      Toolkit related functionality
 **************************************************************************/

#ifndef ALSA_TYP_H
#define ALSA_TYP_H

#if !defined (ALSI_TYP_H)
#include     <alsi_typ.h>
#endif

#if !defined (ALSI_SIG_H)
#include     <alsi_sig.h>
#endif

#if !defined (SIL3_TYP_H)
#include     <sil3_typ.h>
#endif

#if !defined (SITS_TYP_H)
#include     <sits_typ.h>
#endif

#if !defined (SIR_TYP_H)
#include     <sir_typ.h>
#endif

#if !defined (SML3_TYP_H)
#include     <sml3_typ.h>
#endif

#if !defined (L3_TYP_H)
#include     <l3_typ.h>
#endif

#if !defined (TS_SIG_H)
#include     <ts_sig.h>
#endif

#if !defined (MNCB_SIG_H)
#include     <mncb_sig.h>
#endif


/** used for FileList data */
#define SIMAT_MAX_NUM_FILES                 60
/** The size of IMEI */
#define SIMAT_IMEI_SIZE                     15
/* job115982: New limits imposed - allowed 240 bytes for Text String, which could be 7bit packed.*/
/** Theoretical maximum number of characters is therefore 274 */
# define SIMAT_MAX_NUM_CHARACTERS          274
/** Maximum number of 8 bit characters */
# define SIMAT_MAX_NUM_8BIT_CHARS          240
/** The size of SIMAT alpha identifier */
#define SIMAT_ALPHA_ID_SIZE                 241

/**  Maximum number of UCS2 characters */
#define SIMAT_MAX_NUM_UCS2_CHARACTERS       70
/** Maximum number of SIMAT menu items */
#define SIMAT_MAX_NUM_MENU_ITEMS            30
/** Maximum number of SIMAT items */
#define SIMAT_MAX_NUM_ITEMS                 30
/** Maximum number of characters per item */
#define SIMAT_NUM_CHARACTERS_PER_ITEM       50
/* -----------------3/1/01 12:07PM-------------------
  SAT FTA test 27.22.4.11 has a 49 digit SS string to
  handle. This is OK.
  */
/** This takes its format as per GSM 11.11 ADN file definition and
 * can therefore be greater than 40 digits in length.
 */
#define SIM_MAX_SS_STRING_LENGTH            80
/** Max ACK length to Sms-PP Download */
#define SIMAT_MAX_SMS_PP_ACK_LEN           128
/** The maximum SIMAT SS parameters */
#define SIMAT_MAX_SS_PARAMS                241

/** The maximum number of Network Access Identifier*/
#define SIMAT_MAX_NUM_NAI                   SIMAT_MAX_NUM_ITEMS
/** The maximum number of SIMAT transaction ID */
#define SIMAT_MAX_NUM_TRANS_ID              10
/** The maximum length of SIMAT USSD string */
#define SIM_MAX_USSD_STRING_LENGTH          SIM_MAX_SS_STRING_LENGTH
/** The maximum number of ARFCNs */
#define SIMAT_MAX_NUM_ARFCNS                6

/** The number of language code characters */
#define SIMAT_NUM_LANG_CODE_CHARS            2
/** max length of an AT command/response string */
#define SIMAT_AT_COMM_MAX_LEN              240

/* If the requested number of bytes is available in the buffer
* but the whole requested data cannot be included in the
* TERMINAL RESPONSE because of APDU size limits, the ME shall
* inform the SIM that the command has been successfully
* executed , using TERMINAL RESPONSE and shall indicate
* the number of bytes available remaining in the channel
* buffer (or FF if more than the maximum bytes remains) */

#define SIM_MAX_TLV_OBJ                     18    /**< Max size of the TLV object in the RECEIVE DATA RSP
                                                   * command details(5)
                                                   * + device ID(4)
                                                   * +result(3)
                                                   * +channel data length(3)
                                                   * +channel data TLV tag+length (3) */
/** Maximum channel data length */
#define SIMAT_MAX_CHANNEL_DATA_LENGTH      SIM_MAX_MSG_SIZE - SIM_REQ_DATA - SIM_MAX_TLV_OBJ - 1

/** Maximum number of channels */
#define SIMAT_MAX_NUM_CHANNELS               7
/** Maximum length for IPV4 address */
#define SIMAT_MAX_IPV4_ADDR_LENGTH           4
/** Maximum length for IPV6 address */
#define SIMAT_MAX_IPV6_ADDR_LENGTH          16
#define SIMAT_MAX_URL_LENGTH               255  /**< max value for SAT usage */
#define SIMAT_MAX_NUM_BEARERS                4  /**< max num in list for LB */

/** Check that number of UCS2 characters cannot exceed 1/2 the number
 * of 8 bit characters */
#if (SIMAT_MAX_NUM_UCS2_CHARACTERS * 2)>SIMAT_MAX_NUM_8BIT_CHARS /* job115982: new max limits */
#error
#endif

/****************************************************************************
 * Types
 ****************************************************************************/
/** This enumerated type indicates the outcome of the initial
 * proactive SIM command and forms part of the Terminal Response
 * sent to the SIM.
 */
typedef enum SimatGeneralResultTag
{
    /** Command performed successfully */
    SIMAT_GR_COMM_PERF_OK               = 0x00,
    /** Command performed with partial comprehension */
    SIMAT_GR_COMM_PART_COMP             = 0x01,
    /** Command performed, with missing information */
    SIMAT_GR_COMM_PERF_MISS_INFO        = 0x02,
    /** REFRESH performed with additional EFs read */
    SIMAT_GR_REFRESH_ADDIT_EF_READ      = 0x03,
    /** Command performed successfully, but requested icon could not be displayed */
    SIMAT_GR_COMM_PERF_OK_NO_ICON       = 0x04,
    /** Command performed OK, and type of request changed by CC*/
    SIMAT_GR_MOD_BY_CC_OK               = 0x05,
    /** Command performed successfully, limited service */
    SIMAT_GR_COMM_PERF_OK_LIM_SERV      = 0x06,
    /** Command performed with modification */
    SIMAT_GR_COMM_PERF_OK_WITH_MOD      = 0x07,
    /** The SIM wants to refresh the application that is not currently active */
    SIMAT_GR_REFRESH_APP_INACTIVE       = 0x08,
    /** Proactive SIM session terminated by the user */
    SIMAT_GR_USER_TERM_SESSION          = 0x10,
    /** Backward move in proactive SIM session requested by the user */
    SIMAT_GR_USER_REQ_BACK_MOVE         = 0x11,
    /** No response from user */
    SIMAT_GR_USER_NO_RESPONSE           = 0x12,
    /** Help info requested by user */
    SIMAT_GR_USER_REQ_HELP_INFO         = 0x13,
    /** reserved for GSM/3G */
    SIMAT_GR_USSD_SS_TRANS_USER_TERM    = 0x14,
    /** ME currently unable to process command */
    SIMAT_GR_ME_CANNOT_PROC_COMMAND     = 0x20,
    /** Network currently unable to process command */
    SIMAT_GR_NET_CANNOT_PROC_COMMAND    = 0x21,
    /** User did not accept call set-up request */
    SIMAT_GR_USER_REFUSED_CSU_REQ       = 0x22,
    /** User cleared down call before connection or network release */
    SIMAT_GR_USER_CLEAR_DOWN            = 0x23,
    /** Indicates timer is not running */
    SIMAT_GR_CONT_CURR_TIMER_STATE      = 0x24,    /* job112516 */
    /** Indicates a temporary problem with a command modified by Call Control */
    SIMAT_GR_MOD_BY_CC_TEMP_PROB        = 0x25,
    /** Browser generates error */
    SIMAT_GR_BROWSER_GEN_ERROR          = 0x26,
    /** Command beyond ME's capabilities */
    SIMAT_GR_COMM_BEYOND_ME_CAP         = 0x30,
    /** Command type not understood by ME */
    SIMAT_GR_COMM_TYPE_UNKNOWN_BY_ME    = 0x31,
    /** Command type not understood by ME */
    SIMAT_GR_COMM_DATA_UNKNOWN_BY_ME    = 0x32,
    /** Command number not known by ME */
    SIMAT_GR_COMM_NUM_UNKNOWN_BY_ME     = 0x33,
    /** SS Return Error */
    SIMAT_GR_SS_RETURN_ERROR            = 0x34,
    /** SMS RP-ERROR */
    SIMAT_GR_SMS_RP_ERROR               = 0x35,
    /** Error, required values are missing */
    SIMAT_GR_ERROR_REQ_VAL_MISSING      = 0x36,
    /** USSD return error */
    SIMAT_GR_USSD_RETURN_ERROR          = 0x37,
    /** Interaction with call control by NAA, permanent problem */
    SIMAT_GR_CC_OR_MOSM_PERM_PROB       = 0x39,
    /** Bearer Independent Protocol error */
    SIMAT_GR_BEARER_IND_PROT_ERROR      = 0x3A,
    /** Access technology unavailable */
    SIMAT_GR_ACCESS_TECHNO_UNAVAILABLE  = 0x3B
}
SimatGeneralResult;

/** Defines the status of the SIM Application Toolkit command processed.
 * Values 1 to 4 are used in the AlsaFetchFailInd signal to indicate to
 * the Application Layer the reason the command has failed.
 */
typedef enum SimatCommandStatusTag
{
    SIMAT_CS_COMM_OK,                  /**< command data OK */
    SIMAT_CS_COM_DATA_NOT_UNDERSTOOD,  /**< unknown tag, minimum data OK, no CR */
    SIMAT_CS_COM_TYPE_NOT_UNDERSTOOD,  /**< unknown Command Type in Comm Details data */
    SIMAT_CS_REQ_VALUES_MISSING,       /**< minimum set of data elements not in command */
    SIMAT_CS_UNEXPECTED_TAG            /**< Unexpected/Unknown tag */
}
SimatCommandStatus;

/** Defines which possible keys the user can enter in response to the proactive command.*/
typedef enum SimatAllowedKeysTag
{
     /** Denotes only 0-9, *, #, and + keys */
    SIMAT_INKEY_DIGITS_ONLY             = 0x00,
    /** Alphabet set */
    SIMAT_INKEY_ALPHABET_SET            = 0x01
}
SimatAllowedKeys;

/** Defines the result of the Call Control Envelope command sent to the SIM. */
typedef enum SimatCallControlResultTag
{
    /** Call control with allowed, no modification */
    SIMAT_CC_ALLOWED_NO_MODS            = 0x00,
    /** Call control with not allowed */
    SIMAT_CC_NOT_ALLOWED                = 0x01,
    /** Call control with allowed with modification */
    SIMAT_CC_ALLOWED_WITH_MODS          = 0x02
}
SimatCallControlResult;

/** Defines the type of any response data returned from the
 * SIM as a result of a Call Control Envelope command sent to the SIM.
 */
typedef enum SimatCallControlInfoTypeTag
{
    /** Call control set up */
    SIMAT_CC_CALL_SET_UP,
    /** GPRS Call control set-up */
    SIMAT_CC_GP_OPERATION,
    /** Call control with SS operation */
    SIMAT_CC_SS_OPERATION,
    /** Call control with USSD operation */
    SIMAT_CC_USSD_OPERATION,
    /** Call control with SM operation */
    SIMAT_CC_SM_OPERATION               /* needed by BL, but not SIM Mgr */
}
SimatCallControlInfoType;

/** Determines the additional information required
 * as part of the ME currently unable to process command result.
 */
typedef enum SimatMeProblemAddInfoTag
{
    /** No specific cause can be given */
    SIMAT_INFO_NO_CAUSE_GIVEN           = 0x00,
    /** Screen is busy */
    SIMAT_INFO_SCREEN_BUSY              = 0x01,
    /** ME currently busy on call */
    SIMAT_INFO_ME_BUSY_ON_CALL          = 0x02,
    /** ME currently busy on SS transaction */
    SIMAT_INFO_ME_BUSY_ON_SS            = 0x03,
    /** No service */
    SIMAT_INFO_NO_SERVICE               = 0x04,
    /** Access control class bar */
    SIMAT_INFO_ACC_CONT_CLASS_BAR       = 0x05,
    /** Radio resource not granted */
    SIMAT_INFO_RAD_RES_NOT_GRANTED      = 0x06,
    /** Not in speech call */
    SIMAT_INFO_NOT_IN_SPEECH_CALL       = 0x07,
    /** ME busy on USSD */
    SIMAT_INFO_ME_BUSY_ON_USSD          = 0x08,
    /** ME busy on DTMF */
    SIMAT_INFO_ME_BUSY_ON_DTMF          = 0x09,
    /** No application is active */
    SIMAT_INFO_ME_NO_APP_ACTIVE         = 0x0a
}
SimatMeProblemAddInfo;


/** Specifies the access technology at which the sim is */
typedef enum SimatAccessTechnologyTag
{
  /** GSM access technology */
  SIMAT_GSM_AT = 0,
  /** UTRAN access technology */
  SIMAT_UTRAN_AT = 3,
  /** E-UTRAN access technology */
  SIMAT_EUTRAN_AT = 8,
   /** Undefined access technology */
  SIMAT_UNDEFINED_AT = 0xff
} SimatAccessTechnology;

/** The display parameters element */
typedef struct SimatDisplayParamsTag
{
    /** Screen height */
    Int8        numCharDownDisplay;
    Boolean     screenSizingParamsSupported;
    /** screen width */
    Int8        numCharAcrossDisplay;
    /** Variable size and fonts supported */
    Boolean     variableSizeFontsSupported;
    /** screen effects */
    Boolean     displayResizeSupported;
    /** Text can be wrapped */
    Boolean     textWrappingSupported;
    /** Text scrolling */
    Boolean     textScrollingSupported;
    /** Width reduction */
    Int8        widthReductionInMenu;
} SimatDisplayParams;

typedef enum SimatTimeUnitTag
{
    SIMAT_TIME_UNIT_MINUTES       = 0,
    SIMAT_TIME_UNIT_SECONDS       = 1,
    SIMAT_TIME_UNIT_TENTHS        = 2

} SimatTimeUnit;

/** Defines the command qualifier for the SIMAT Provide Local Information proactive command.
 * The identifier value denotes the information content to be provided to the user.
 */
typedef enum SimatProvLocalInfoQualTag
{
    /** Consists of the MCC, MNC, LAC, and the Cell ID.*/
    SIMAT_LOCATION_INFORMATION          = 0x00,
    /** IMEI */
    SIMAT_IMEI                          = 0x01,
    /** the Network Measurement Results  */
    SIMAT_NMR                           = 0x02,
    /** the current date, time and time zone */
    SIMAT_DATE_TIME_TIMEZONE            = 0x03,
    /** the current ME language setting */
    SIMAT_LANGUAGE_SETTING              = 0x04,
    /** the Timing Advance, suitable only for GERAN */
    SIMAT_TIMING_ADVANCE                = 0x05,
    /** The current access technology */
    SIMAT_ACCESS_TECHNOLOGY             = 0x06
}
SimatProvLocalInfoQual;

/* job 102762 */
/** Specifies GMM services. */
typedef enum SimatGmmServiceStatusTag
{
   /** Normal service */
   SIMAT_GMM_NORMAL_SERVICE,
   /** No service */
   SIMAT_GMM_NO_SERVICE,
   /** Emergency service */
   SIMAT_GMM_EMERG_SERVICE
}
SimatGmmServiceStatus;

/** Defines the command qualifier for the SIMAT Refresh proactive command.*/
typedef enum SimatRefreshQualTag
{
    /** SIM Initialization and Full File Change Notification */
    SIMAT_SIM_INIT_FULL_FILE_CHA        = 0x00,
    /** File Change Notification */
    SIMAT_FILE_CHA_NOTIFICATION         = 0x01,
    /** File Change Notification */
    SIMAT_SIM_INIT_FILE_CHA_NOT         = 0x02,
    /** SIM Initialization */
    SIMAT_SIM_INIT                      = 0x03,
    /** SIM Reset */
    SIMAT_SIM_RESET                     = 0x04,
    /** Refresh of the application only*/
    SIMAT_APPLICATION_RESET             = 0x05,
    /** SIM session Reset */
    SIMAT_SESSION_RESET                 = 0x06
}
SimatRefreshQual;

/** Defines the command qualifier for the SIMAT Set-Up Call proactive command.*/
typedef enum SimatSetUpCallQualTag
{
    /** set up call, but only if not currently busy on another call */
    SIMAT_IF_NOT_BUSY                   = 0x00,
    /** set up call, but only if not currently busy on another call, with redial */
    SIMAT_IF_NOT_BUSY_WITH_REDIAL       = 0x01,
    /** set up call, putting all other calls (if any) on hold */
    SIMAT_CALLS_ON_HOLD                 = 0x02,
    /** set up call, putting all other calls (if any) on hold, with redial */
    SIMAT_CALLS_ON_HOLD_WITH_REDIAL     = 0x03,
    /** set up call, disconnecting all other calls (if any) */
    SIMAT_DISCON_CALLS                  = 0x04,
    /** set up call, disconnecting all other calls (if any), with redial */
    SIMAT_DISCON_CALLS_WITH_REDIAL      = 0x05,
    /** Reserved */
    SIMAT_RESERVED_VALUE                = 0xFF
}
SimatSetUpCallQual;

/** Specifies the type of tone */
typedef enum SimatToneTypeTag
{
    /** Dial tone */
    SIMAT_SUP_DIAL_TONE                 = 0x01,
    /** Called subscriber busy */
    SIMAT_SUP_CALLED_SUB_BUSY           = 0x02,
    /** Congestion */
    SIMAT_SUP_CONGESTION                = 0x03,
    /** Radio path acknowledge */
    SIMAT_SUP_RAD_PATH_ACK              = 0x04,
    /** Radio path not available / Call dropped */
    SIMAT_SUP_RAD_UNAV_CALL_DROP        = 0x05,
    /** Error / Special information */
    SIMAT_SUP_ERROR_SPECIAL_INFO        = 0x06,
    /** Call waiting tone */
    SIMAT_SUP_CALL_WAITING_TONE         = 0x07,
    /** Ringing tone */
    SIMAT_SUP_RINGING_TONE              = 0x08,
    /** General Beep */
    SIMAT_PROP_GENERAL_BEEP             = 0x10,
    /** Positive acknowledgement tone*/
    SIMAT_PROP_POS_ACK_TONE             = 0x11,
    /** Negative acknowledgement or error tone */
    SIMAT_PROP_NEG_ACK_OR_ERROR_TONE    = 0x12,
    /** User selected ringing tone */
    SIMAT_USER_SELECTED_RINGING_TONE    = 0x13,
    /** User selected alert tone */
    SIMAT_USER_SELECTED_ALERT_TONE      = 0x14
}
SimatToneType;

/** Contains an alpha identifier as used in the ADN, FDN, LND, MSISDN and SMSP files.*/
typedef struct SimatAlphaIdentifierTag
{
    /** The length of data
\assoc ARRAY \ref data */
    Int16                         length;
    /** Holds alpha identifier */
    Int8                          data[SIMAT_ALPHA_ID_SIZE];
}
SimatAlphaIdentifier;

/** types required for modified Alsaxxsignals */
typedef enum SimatAlphabetSetTag
{
    /** SMS default alphabet */
    SIMAT_SMS_DEF_ALPHABET              = 0x00,
    /** UCS2 alphabet format */
    SIMAT_UCS2_ALPHABET                 = 0x01
}
SimatAlphabetSet;

/** Contains the network measurement results. */
typedef struct SimatNetMeasResultsTag
{
    /** Encoded measurement results */
    Int8                  encodedMeasReport[ENCODED_MEAS_RESULTS_IE_SIZE];
    /** The number of BCCH arfcn */
    Int8                  numBcchArfcn;
    /** Lists BCCH freq */
    Arfcn                 bcchArfcn[MAX_BA_CHANNELS];
}
SimatNetMeasResults;

/** Defines the additional information for a successful command result
 * for the Send SS proactive command.
 */
typedef struct SimatSsResultsTag
{
    /** Result Operation code */
    Int8                          operationCode;
    /** The length of result parameters  */
    Int8                          paramsLength;
    /** Result Parameters */
    Int8                          params[SIMAT_MAX_SS_PARAMS];

} SimatSsResults;

/** Defines the type of additional information given in the result
 * of the Send SS proactive command.
 * It forms part of the AlsaSendSsRsp signal sent from the Application
 * Layer to the SIM.
 */
typedef enum SimatSsAdditResultTypeTag
{
    /** No additional information */
    SIMAT_AISS_NO_ADDIT_INFO,
    /** Additional information for SEND SS*/
    SIMAT_AISS_SS_RESULT,
    /** Additional information for ME problem */
    SIMAT_AISS_ME_PROBLEM,
    /** Additional information for network problem*/
    SIMAT_AISS_NET_PROBLEM,
    /** Additional information for SS problem */
    SIMAT_AISS_SS_PROBLEM
} SimatSsAdditResultType;

typedef enum SimatTextAlignTag
{
  SIMAT_ALIGN_LEFT     = 0,
  SIMAT_ALIGN_CENTRE   = 1,
  SIMAT_ALIGN_RIGHT    = 2,
  SIMAT_LANG_DEPENDENT = 3
}
SimatTextAlign;

typedef enum SimatFontSizeTag
{
  SIMAT_FONT_NORMAL = 0,
  SIMAT_FONT_LARGE  = 1,
  SIMAT_FONT_SMALL = 2,
  SIMAT_FONT_RESERVED = 3
}
SimatFontSize;

typedef enum SimatTextColourTag
{
 SIMAT_COLOUR_BLACK          = 0x00,
 SIMAT_COLOUR_DARK_GREY      = 0x01,
 SIMAT_COLOUR_DARK_RED       = 0x02,
 SIMAT_COLOUR_DARK_YELLOW    = 0x03,
 SIMAT_COLOUR_DARK_GREEN     = 0x04,
 SIMAT_COLOUR_DARK_CYAN      = 0x05,
 SIMAT_COLOUR_DARK_BLUE      = 0x06,
 SIMAT_COLOUR_DARK_MAGENTA   = 0x07,
 SIMAT_COLOUR_GREY           = 0x08,
 SIMAT_COLOUR_WHITE          = 0x09,
 SIMAT_COLOUR_BRIGHT_RED     = 0x0A,
 SIMAT_COLOUR_BRIGHT_YELLOW  = 0x0B,
 SIMAT_COLOUR_BRIGHT_GREEN   = 0x0C,
 SIMAT_COLOUR_BRIGHT_CYAN    = 0x0D,
 SIMAT_COLOUR_BRIGHT_BLUE    = 0x0E,
 SIMAT_COLOUR_BRIGHT_MAGENTA = 0x0F
}
SimatTextColour;

typedef struct SimatTextAttributesTag
{
  Int8                 startPosition;  /*Start position of the text formatting  - number of chars after text start*/
  Int8                 formatingLength;
  SimatTextAlign       alignment;
  SimatFontSize        fontSize;
  Boolean              bold;
  Boolean              italic;
  Boolean              underline;
  Boolean              strikeThrough;
  SimatTextColour      foregroundColour;
  SimatTextColour      backgroundColour;
}
SimatTextAttributes;

/** This union contains additional information on the result of the Send SS proactive command.
 * It forms part of the AlsaSendSsRsp signal sent from the Application Layer to the SIM.
 */
typedef union SimatSsAdditResultTag
{
    /** No additional information */
    Int8                          noAdditInfo;
    /** Additional information for SEND SS*/
    SimatSsResults                ssResult;
    /** Additional information for ME problem */
    SimatMeProblemAddInfo         meProblem;
    /** Additional information for network problem*/
    GsmCause                      netProblem;
    /** Additional information for SS problem */
    Int8                          ssProblem;
} SimatSsAdditResult;

/** Contains address information relating to the Set Up
 * Call and Send Short Message proactive commands.
 */
typedef struct SimatAddressTag
{
    /** Specifies the type of the number */
    BcdNumberType                 typeOfNumber;
    /** Numbering plan identification */
    BcdNumberPlan                 numberPlan;
    /** The length of the dialled string
\assoc ARRAY \ref dialString */
    Int8                          length;
    /** Dialled string */
    Bcd                           dialString[SIM_MAX_DIAL_NUMBER_LENGTH];
}
SimatAddress;

/** Defines the command qualifier for the SIMAT Display Text proactive command.*/
typedef struct SimatDisplayTextQualTag
{
    /** Determines the priority level for the text to be displayed.
     * If set to FALSE it indicates that the text is to be displayed
     * as normal priority only, and this is not displayed if the screen
      is currently busy.
      */
    Boolean                       dispHighPriorityReq;
    /** Determines when the displayed message is cleared from the display.
     * If this flag is set TRUE it indicates the displayed text is cleared
     * after an MMI user interaction.  When set to false it indicates the
     * display is cleared after a short delay (although a user MMI action
     * before the delay period times out can also clear the display).
     */
    Boolean                       userClearMessage;
}
SimatDisplayTextQual;

/** Contains a list of the paths to all the files on the SIM.
 * There are no delimiters between files, though each file path
 * begins with the Master File and ends with an Elementary File.
 */
typedef struct SimatFileListTag
{
    /** Indicates the number of files
\assoc ARRAY \ref fileList */
    Int8                          numOfFiles;
    /** List of files with their path */
    SimUiccPath                   fileList[SIMAT_MAX_NUM_FILES];
}
SimatFileList;

/** Defines the command qualifier for the SIMAT Get Inkey proactive command.*/
typedef struct SimatGetInkeyQualTag
{
    /** allowed Keys   */
    SimatAllowedKeys              allowedKeys;
    /** Specifies the alphabet format used */
    SimatAlphabetSet              alphabetSet;
    /** Determines whether help information is available for the proactive command.*/
    Boolean                       helpInfoAvailable;
    /** If set then only <B>y</B> or <B>n</B> are valid inputs */
    Boolean                       yesNoResponseOnly;
    /** Display user response */
    Boolean                       displayUserResponse;
}
SimatGetInkeyQual;

/** This structure defines the valid character set for the text and other associated parameters.*/
typedef struct SimatGetInputQualTag
{
    /** Specifies the allowed keys (either digits only (0-9, *, # and +) or
     * characters from the SMS default alphabet
     */
    SimatAllowedKeys              allowedKeys;
    /** Specifies the alphabet set */
    SimatAlphabetSet              alphabetSet;
    /** the ME may display the text string being entered by the user (the response),
     * or the ME shall hide (not display) the actual text string.
     */
    Boolean                       echoUserInputToDisplay;
    /** Whether the text in packed or unpacked format */
    Boolean                       inputInPackedFormat;
    /** Determines whether help information is available for the proactive command.*/
    Boolean                       helpInfoAvailable;
}
SimatGetInputQual;

/** Holds the IMEI required as part of the Provide Local Information proactive command.*/
typedef struct SimatImeiTag
{
    /** Holds IMEI digits */
    Int8                          imeiDigits[SIMAT_IMEI_SIZE];
}
SimatImei;

/** Stores the information required for each item contributing to the menu structures
 * provided as part of the proactive SIM functionality.
 */
typedef struct SimatItemTag
{
    /** The identifier of the selected menu item */
    Int8                          itemId;
    /** The length of menu items
\assoc ARRAY \ref itemText */
    Int8                          length;
    /** The list of menu items */
    Int8                          itemText[SIMAT_NUM_CHARACTERS_PER_ITEM];
    /** Icon Id */
    Int8                          iconIdRecordNum;
    SimatTextAttributes           textAttributes;
}
SimatItem;

/** Defines the structure of the menu items presented to
 * the ME from the SIM as part of the Set Up Menu proactive command.
 */
typedef struct SimatMenuItemsTag
{
    /** Denotes whether the ME is required to remove the existing menu from its menu structure.*/
    Boolean                       removeExistingMenu;
    /** The number of items in the menu
\assoc ARRAY \ref itemData */
    Int8                          numMenuItems;
    /** Gives the item identifier, item length, and the textual content of each item to be
     * displayed by the ME for the number of items in the menu.
     */
    SimatItem                     itemData[SIMAT_MAX_NUM_MENU_ITEMS];
    /** Indicates if icon list is present */
    Boolean                       iconListPresent;
    /** Display icon only */
    Boolean                       displayIconOnly;
    /** Whether the items in the list have text attributes */
    Boolean                       textAttsListPresent;
 }
SimatMenuItems;

/** Stores the list of items required for each menu structures
 * provided as part of the proactive SIM functionality
 */
typedef struct SimatItemListTag
{
    /** The number of items within a list
\assoc ARRAY \ref itemData */
    Int8                          numItems;
    /** Lists the items */
    SimatItem                     itemData[SIMAT_MAX_NUM_ITEMS];
    /** Indicates if icon list is present */
    Boolean                       iconListPresent;
    /** Display icon only */
    Boolean                       displayIconOnly;
    /** Whether the items in the list have text attributes */
    Boolean                       textAttsListPresent;
}
SimatItemList;

/** Defines the location information required as part of
 * the Provide Local Information proactive command.
 */
typedef struct SimatLocationInfoTag
{
    /** location area identifier*/
    LocationAreaIdElement         lai;
    /** Cell identity Value */
    Int32                         cellId;
    /** The current band (FDD /TDD128 )*/
    Band                          currentBand;
}
SimatLocationInfo;

/** This information is only available when the ME is connected to a GSM access network.*/
typedef struct SimatTimingAdvanceTag
{
    /** Indicates if ME is in idle state */
    Boolean                       meIsInIdleState;
    /** Timing Advance */
    Int16                         ta;
}
SimatTimingAdvance;

/** Union defining the local information provided,
 * the content of which depends on the value held in
 * the command qualifier in the request signal for the
 * Provide Local Information proactive command.
 * This information denotes either the IMEI or the location
 * information (MCC, MNC, LAC, and the Cell ID), or,
 * for software built using the UPGRADE_SAT97 switch,
 * the network measurement results.
 */
typedef union SimatProvLocalInformationTag
{
    /** Location information (: the mobile country code (MCC),
     * mobile network code (MNC), location area code (LAC) and
     * cell ID of the current serving cell)
     */
    SimatLocationInfo             locationInfo;
    /** The IMEI  */
    SimatImei                     imei;
    /** the Network Measurement Results  */
    SimatNetMeasResults           nmr;
    /** the current date, time and time zone */
    SmsTimeStamp                  dttz;
    /** the current ME language setting */
    Int8                          langSetting[SIMAT_NUM_LANG_CODE_CHARS];
    /** the Timing Advance, suitable only for GERAN */
    SimatTimingAdvance            timingAdvance;
    /** The access technology */
    SimatAccessTechnology         accessTechnology;
}
SimatProvLocalInformation;

/** Defines acceptable length of the response string for the Get Input proactive command */
typedef struct SimatResponseLengthTag
{
    /** the minimum acceptable lengths for the response from the user.*/
    Int8                          respMinLength;
    /** the maximum acceptable lengths for the response from the user. */
    Int8                          respMaxLength;
}
SimatResponseLength;

/** Defines the qualifier for the SIMAT Select Item proactive command.*/
typedef struct SimatSelectItemQualTag
{
    /** Determines whether help information is available for the proactive command.*/
    Boolean                       helpInfoAvailable;
    /** Whether The presentation type info is present */
    Boolean                       presentationTypeInfoPresent;
    /** Whether use navigation option is present */
    Boolean                       useNavigationOptions;
    /** Whether soft key selection is preferred */
    Boolean                       softKeySelectionPreferred;
}
SimatSelectItemQual;

/** SEND SHORT MESSAGE */
typedef struct SimatSendSmQualTag
{
    /** Indicates whether packing is required for the Short Message text.*/
    Boolean                       smsPackingRequired;
}
SimatSendSmQual;

/** Defines the qualifier for the SIMAT Set Up Menu proactive command.*/
typedef struct SimatSetUpMenuQualTag
{
    /** Determines whether help information is available for the proactive command.*/
    Boolean                       helpInfoAvailable;
    /** Whether or not the soft key selection is preferred */
    Boolean                       softKeySelectionPreferred;
}
SimatSetUpMenuQual;

/** Defines the SS string to be sent in the Send SS proactive command.*/
typedef struct SimatSsStringTag
{
    /** The type of number */
    BcdNumberType                 typeOfNumber;
    /** Number plan */
    BcdNumberPlan                 numberPlan;
    /** The length of dialled string
\assoc ARRAY \ref dialString */
    Int8                          length;
    /** Lists dialled string */
    Bcd                           dialString[SIM_MAX_SS_STRING_LENGTH];
}
SimatSsString;

/** Stores information relating to the text strings used in the
 * processing of several of the proactive commands
 */
typedef struct SimatTextStringTag
{
    /** Coding scheme to use for text */
    SmsDataCoding                 codingScheme;
    /** The length of text string
\assoc ARRAY \ref textString */
    Int16                         length;      /* job115982: up to 274 chars if 7bit packed... */
    /** Lists text string */
    Int8                          textString[SIMAT_MAX_NUM_CHARACTERS];
}
SimatTextString;

/** Defines any response data that may be returned from the
 * SIM as a result of a Call Set Up Envelope command sent to the SIM.
 */
typedef struct SimatCcCallSetUpTag
{
    /** Indicates if the address is present. */
    Boolean                       addressPresent;
    /** If the address data object is not present, then the ME shall assume the Dialling
     * number is not to be modified
     */
    SimatAddress                  address;
    /** Indicates if bearer capability is present */
    Boolean                       ccpPresent;
    /** Is only required if the SIM requests the call details to be modified,
     * if the capability configuration parameters are not present, then the ME
     * shall assume the parameters are not to be modified.
     */
    BearerCapability              ccp;
    /** Indicates if called party sub address is present */
    Boolean                       cpsPresent;
    /** Is only required if the SIM requests the call details to be
     * modified, if the called party subaddress is not present, then the ME
     * shall assume the subaddress is not to be modified
     */
    CalledPartySubaddress         cps;
    /** Indicates if alpha identifier is present */
    Boolean                       alphaIdPresent;
    /** Acts as a title for the list of menu items.*/
    SimatAlphaIdentifier          alphaId;
    /** Indicates if BC Repeat indicator is present */
    Boolean                       bcRepeatIndicatorPresent;
    /** The repeat indicator information element shall be included if bearer
     * capability 1 information element and bearer capability 2 IE are both
     * included in the message.
     */
    /*SimatBcRepeatIndicator        bcRepeatIndicator; */
    /** Indicates if bearer capability 2 is present */
    Boolean                       ccp2Present;
    /** Bearer capability 2 */
    BearerCapability              ccp2;
}
SimatCcCallSetUp;

/** Defines any response data that may be returned from the
 * SIM as a result of an SS Operation Envelope command sent to the SIM.
 */
typedef struct SimatCcSsOperationTag
{
    /** SS string */
    SimatSsString                 ssString;
}
SimatCcSsOperation;

/** It defines the USSD string to be sent in the Send USSD proactive command.*/
typedef struct SimatUssdStringTag
{
    /** Raw undecoded data coding scheme */
    Int8                          rawDCS; /* job115982: added for TC 27.22.4.12.x */
    /** Length of USSD
\assoc ARRAY \ref dialString */
    Int8                          length;
    /** String Sent in  USSD command */
    Bcd                           dialString[SIM_MAX_USSD_STRING_LENGTH];
}
SimatUssdString;

/** Defines any response data that may be returned from the
 * SIM as a result of an USSD Operation Envelope command sent to the SIM.
 */
typedef struct SimatCcUssdOperationTag
{
    /** USSD string element */
    SimatUssdString               ussdString;
}
SimatCcUssdOperation;

/** Details of the context activation parameters to be used for GPRS Call Control */
typedef struct SimatCcPdpContextInfoTag
{
#if defined (UPGRADE_NASMDL2)
    /** Identifies the service access point */
    Int8                          psdBearerId;
#else
   /** Identifies the service access point */
    Nsapi                         nsapi;
#endif /* UPGRADE_NASMDL2 */
    /** Requested quality oof service */
    QualityOfService              requestedQoS;
    /** pdpTypeNumber */
    PdpTypeNumber                 pdpTypeNumber;
    /** PDP address */
    PdpAddress                    pdpAddress;
    /** APN */
    AccessPointName               apn;
    /** whether pppConfigOptions are present */
    Boolean                       pppConfigOptsPresent;
    /** ppp Configuration options */
    ProtocolConfigOptions         pppConfigOpts;
}
SimatCcPdpContextInfo;


/** Details of the context activation parameters to be used for PDN Call Control */
typedef struct SimatCcPdnContextInfoTag
{
    /** Identifies the protocol descriminator */
    Int8                          protocolDiscriminator;
    /** Identifies the service access point */
    Int8                          epsBearerId;
    /** Identifies the PDN connectivity request message identity */
    Int8                          pdnConnReqId;
    /** Identifies the request type */
    Int8                          reqType;
    /** pdnTypeNumber */
    PdnType                       pdnTypeNumber;
    /** APN */
    AccessPointName               apn;
    /** whether pppConfigOptions are present */
    Boolean                       pppConfigOptsPresent;
    /** ppp Configuration options */
    ProtocolConfigOptions         pppConfigOpts;
}
SimatCcPdnContextInfo;

/** Required in response to a TIMER MANAGEMENT proactive command.
** New Types for Release98 implementation.
*/
typedef enum SimatTimerIdentifierTag
{
    /** Timer ID 1*/
    SIMAT_TIMER_ID_1              = 0x01,
    /** Timer ID 2*/
    SIMAT_TIMER_ID_2              = 0x02,
    /** Timer ID 3*/
    SIMAT_TIMER_ID_3              = 0x03,
    /** Timer ID 4*/
    SIMAT_TIMER_ID_4              = 0x04,
    /** Timer ID 5*/
    SIMAT_TIMER_ID_5              = 0x05,
    /** Timer ID 6*/
    SIMAT_TIMER_ID_6              = 0x06,
    /** Timer ID 7*/
    SIMAT_TIMER_ID_7              = 0x07,
    /** Timer ID 8*/
    SIMAT_TIMER_ID_8              = 0x08
}
SimatTimerIdentifier;

/** Timer qualifier element */
typedef enum SimatTimerManQualTag
{
    /** Start timer */
    SIMAT_TIMER_START             = 0x00,
    /** Deactivate timer */
    SIMAT_TIMER_DEACTIVATE        = 0x01,
    /** Get current timer value */
    SIMAT_TIMER_GET_CURRENT_VAL   = 0x02
}
SimatTimerManQual;

/** The AT Command string is structured exactly as the
 * AT Command linewhich may contain single or concatenated AT commands
 */
typedef struct SimatAtCommandDataTag
{
    /** The length of AT command */
    Int8                          dataLength;
    /** AT Command string */
    Int8                          data[SIMAT_AT_COMM_MAX_LEN];
}
SimatAtCommandData;

/** SIMAT DTMF string element */
typedef struct SimatDtmfStringTag
{
    /** The length of DTMF digits */
    Int8                          length;
    /** Contains DTMF digits */
    Bcd                           data[SIM_MAX_DIAL_NUMBER_LENGTH];
}
SimatDtmfString;

/** Holds the icon identifier. */
typedef struct SimatIconIdentifierTag
{
    /** Indicates whether or not icon is self explanatory
     * (if displayed, it shall be displayed together with the
     * corresponding name in text format)
     */
    Boolean                       displayIconOnly;
    /** The record number of the corresponding image in EFIMG */
    Int8                          recordNum;
}
SimatIconIdentifier;

/** R98 Icon Identifier support for reading Image instance data from SIM */
#define SIM_MAX_NUM_IMAGE_INSTANCES   10

/** The maximum length of image instance data */
#define SIM_MAX_IMAGE_INST_DATA_SIZE 256
/** Maximum number of image records available per SIM command */
#define SIM_MAX_NUM_IMAGE_RECS       255

/** Holds image instance data */
typedef Int8 SimImageInstData[SIM_MAX_IMAGE_INST_DATA_SIZE];

/** Image coding scheme to use */
typedef enum SimImageCodingSchemeTag
{
    /** Basic coding */
    SIM_IMAGE_BASIC_CODING             = 0x11,
    /** Colour coding */
    SIM_IMAGE_COLOUR_CODING            = 0x21
#if defined (UPGRADE_REL6)
    ,
    /** Colour coding with transparency */
    SIM_IMAGE_COLOUR_WITH_TRANSPARENCY = 0x22
#endif
}
SimImageCodingScheme;

/** SIM image instance descriptor element */
typedef struct SimImageInstDescriptorTag
{
    /** Width of image */
    Int8                          width;
    /** Height of image */
    Int8                          height;
    /** Specifies which image coding scheme to use */
    SimImageCodingScheme          codingScheme;
    /** Elementary File Id holding data */
    Int16                         efId;
    /** offset for start of data in efId */
    Int16                         offset;
    /** length of image instance data */
    Int16                         dataLength;
}
SimImageInstDescriptor;

/** Each record of this EF identifies instances of one particular graphical image,
 * which graphical image is identified by this EF's record number
 */
typedef struct SimImageDataTag
{
    /** Number of actual image instances
\assoc ARRAY \ref instance */
    Int8                          numOfInstances;
    /** Image instance data file identifier */
    SimImageInstDescriptor        instance[SIM_MAX_NUM_IMAGE_INSTANCES];
}
SimImageData;

/* New Types for Release 99 Implementation*/
/** SIMAT qualifier to launch browser */
typedef enum SimatLaunchBrowserQualTag
{
    /** launch browser if not already launched */
    SIMAT_LB_CONN_IF_NOT_LAUNCHED    = 0x00,
    /** Launch browser using the existing browser */
    SIMAT_LB_USE_EXISTING_BR         = 0x02,
    /** close the existing browser session and launch new browser session */
    SIMAT_LB_LAUN_NEW_BR_SEC_SESSION = 0x03
}
SimatLaunchBrowserQual;

/** The terminal provides additional information when there is an error launching browser */
typedef enum SimatLauBrProblemAddInfoTag
{
    /** No specific cause can be given */
    SIMAT_LB_INFO_NO_CAUSE_GIVEN     = 0x00,
    /** Bearer unavailable */
    SIMAT_LB_INFO_BEARER_NOT_AVAIL   = 0x01,
    /** Browser unavailable */
    SIMAT_LB_INFO_BROWSER_NOT_AVAIL  = 0x02,
    /** Terminal unable to read the provisioning data.*/
    SIMAT_LB_INFO_NO_PROV_DATA       = 0x03
}
SimatLauBrProblemAddInfo;

/** Additional information for Bearer Independent Protocol element */
typedef enum SimatBipProblemAddInfoTag
{
    /** No specific cause can be given */
    SIMAT_BIP_INFO_NO_CAUSE_GIVEN    = 0x00,
    /** No channel available */
    SIMAT_BIP_INFO_NO_CHAN_AVAIL     = 0x01,
    /** Channel closed */
    SIMAT_BIP_INFO_CHAN_CLOSED       = 0x02,
    /** Channel identifier not valid */
    SIMAT_BIP_INFO_CHAN_ID_INVALID   = 0x03,
    /** Requested buffer size not available */
    SIMAT_BIP_INFO_BUFF_SIZE_NOT_AV  = 0x04,
    /** Security error (unsuccessful authentication) */
    SIMAT_BIP_INFO_SECURITY_ERROR    = 0x05,
    /** Requested UICC/terminal interface transport level not available */
    SIMAT_BIP_INFO_TRANS_LEV_NOT_AV  = 0x06
}
SimatBipProblemAddInfo;

/** The terminal shall use this list to choose which bearers are allowed in order of priority.*/
typedef enum SimatBearerTag
{
    /** SMS bearer */
    SIMAT_BEARER_SMS                 = 0x00,
    /** CSD bearer */
    SIMAT_BEARER_CSD                 = 0x01,
    /** USSD bearer */
    SIMAT_BEARER_USSD                = 0x02,
    /** GPRS bearer */
    SIMAT_BEARER_GPRS                = 0x03
}
SimatBearer;

/** Release 99 Provisioning File Reference support for Launch Browser command */
/* added for job104951 */
#define SIMAT_MAX_NUM_PFR_FILES     5
#define SIMAT_MAX_FILE_PATH_LENGTH  3  /**< Number of 16-bit file identifiers in path */

/** Provisioning file reference information element */
typedef struct SimatPFRInfoTag
{
    /** The length of file identifier */
    Int8                          fileIdLength;
    /** File id ?*/
    Int16                         fileId[SIMAT_MAX_FILE_PATH_LENGTH];
} SimatPFRInfo;

/** Provisioning file reference element.
 * The file shall contain a single unambiguous set of parameters required to
 * make the connection. The content of the file shall be consistent with the
 * format defined for provisioning information for the requested type of browser.
 */
typedef struct SimatProvFileRefDataTag
{
    /** The number of Provisioning file reference files */
    Int8                          numProvFRFiles;
    /** Holds file info */
    SimatPFRInfo                  fileInfo[SIMAT_MAX_NUM_PFR_FILES];
} SimatProvFileRefData;

/** This command is issued by the UICC to request a channel opening */
typedef struct SimatOpenChannelQualTag
{
    /** Indicates if the link is established immediately */
    Boolean                       immediateLinkEstablishment;
    /** Indicates if the link is established background, if it is TRUE, immediateLinkEstablishment is ignored */
    Boolean                       backgroundLinkEstablishment;
}
SimatOpenChannelQual;

/** Command qualifier that requests the terminal to send on the specified
 * channel data provided by the UICC (if class "e" is supported)
 */
typedef struct SimatSendDataQualTag
{
    /** Whether to send data immediately or to store data in Tx buffer*/
    Boolean                       sendDataImmediately;
}
SimatSendDataQual;

/** SIMAT data rate element */
typedef enum SimatDataRateTag
{
    /** Auto-Baud */
    SIMAT_DR_AUTOBAUD             = 0x00,
    /** V.21 300 Hz */
    SIMAT_DR_V21_300              = 0x01,
    /** V.22 1.2 KHz */
    SIMAT_DR_V22_1200             = 0x02,
    /** V.22 bis 2.4 KHz */
    SIMAT_DR_V22BIS_2400          = 0x04,
    /** V.22 ter 2.4 KHz */
    SIMAT_DR_V26TER_2400          = 0x05,
    /** V.32 4.8 KHz */
    SIMAT_DR_V32_4800             = 0x06,
    /** V.32 9.6 KHz */
    SIMAT_DR_V32_9600             = 0x07,
    /** V.34 9.6 KHz */
    SIMAT_DR_V34_9600             = 0x0C,
    /** V.34 14.4 KHz */
    SIMAT_DR_V34_14400            = 0x0E,
    /** V.34 19.2 KHz */
    SIMAT_DR_V34_19200            = 0x0F,
    /** V.34 28.8 KHz */
    SIMAT_DR_V34_28800            = 0x10,
    /** V.120 1.2 KHz */
    SIMAT_DR_V120_1200            = 0x22,
    /** V.120 2.4 KHz */
    SIMAT_DR_V120_2400            = 0x24,
    /** V.120 4.8 KHz */
    SIMAT_DR_V120_4800            = 0x26,
    /** V.120 9.6 KHz */
    SIMAT_DR_V120_9600            = 0x27,
    /** V.120 14.4 KHz */
    SIMAT_DR_V120_14400           = 0x2B,
    /** V.120 19.2 KHz */
    SIMAT_DR_V120_19200           = 0x2F,
    /** V.120 28.8 KHz */
    SIMAT_DR_V120_28800           = 0x30,
    /** V.120 38.4 KHz */
    SIMAT_DR_V120_38400           = 0x31,
    /** V.120 48 KHz */
    SIMAT_DR_V120_48000           = 0x32,
    /** V.120 56 KHz */
    SIMAT_DR_V120_56000           = 0x33,
    /** V.110 300 Hz */
    SIMAT_DR_V110_300             = 0x41,
    /** V.110 1.2 KHz */
    SIMAT_DR_V110_1200            = 0x42,
    SIMAT_DR_V110_X31_2400        = 0x44,
    SIMAT_DR_V110_X31_4800        = 0x46,
    SIMAT_DR_V110_X31_9600        = 0x47,
    SIMAT_DR_V110_X31_14400       = 0x4B,
    SIMAT_DR_V110_X31_19200       = 0x4F,
    SIMAT_DR_V110_X31_28800       = 0x50,
    SIMAT_DR_V110_X31_38400       = 0x51,
    SIMAT_DR_V110_X31_48000       = 0x52,
    SIMAT_DR_V110_X31_56000       = 0x53,
    SIMAT_DR_BIT_TRANS_56000      = 0x73,
    SIMAT_DR_BIT_TRANS_64000      = 0x74
}
SimatDataRate;

/** Bearer service names */
typedef enum SimatBearerServiceTag
{
    /** Data circuit asynchronous Unrestricted Digital Information (UDI) modem */
    SIMAT_BS_UDI_3_1K_DC_ASYNC    = 0x00,
    /** Data circuit synchronous UDI modem. */
    SIMAT_BS_UDI_3_1K_DC_SYNC     = 0x01,
    /** PAD access asynchronous UDI */
    SIMAT_BS_UDI_PAD_ASYNC        = 0x02,
    /** Packet access synchronous UDI */
    SIMAT_BS_UDI_PACKET_SYNC      = 0x03,
    /** Data circuit asynchronous Restricted Digital Information (RDI).*/
    SIMAT_BS_RDI_3_1K_DC_ASYNC    = 0x04,
    /** Data circuit synchronous Restricted Digital Information (RDI).*/
    SIMAT_BS_RDI_3_1K_DC_SYNC     = 0x05,
    /** PAD access asynchronous RDI. */
    SIMAT_BS_RDI_PAD_ASYNC        = 0x06,
    /** Packet access synchronous RDI */
    SIMAT_BS_RDI_PACKET_SYNC      = 0x07
}
SimatBearerService;

/** Bearer service connection elements. */
typedef enum SimatConnElementTag
{
    /** Bearer service unknown */
    SIMAT_CE_TRANS                = 0x00,
    /** Link layer correction enabled */
    SIMAT_CE_NON_TRANS            = 0x01,
    /** No link layer correction present */
    SIMAT_CE_BOTH_TRANS_PREF      = 0x02,
    /** Both available, transparent preferred */
    SIMAT_CE_BOTH_NON_TRANS_PREF  = 0x03
}
SimatConnElement;

/** Information related to CSD bearer */
typedef struct SimatCsdBearerTag
{
    /** Data rate */
    SimatDataRate                 dataRate;
    /** bearer service */
    SimatBearerService            bearerService;
    /** Connection element */
    SimatConnElement              connElement;
}
SimatCsdBearer;
/** Holds GPRS Precedence Class element that can be assigned to an application */
typedef enum SimatGprsPrecClassTag
{
    /** GPRS Precedence Class 1 */
    SIMAT_GPC_1                   = 0x01,
    /** GPRS Precedence Class 2 */
    SIMAT_GPC_2                   = 0x02,
    /** GPRS Precedence Class 3 */
    SIMAT_GPC_3                   = 0x03
}
SimatGprsPrecClass;

/** Predictive delay classes */
typedef enum SimatGprsDelayClassTag
{
    /** GPRS predective delay class 1 */
    SIMAT_GDC_1                   = 0x01,
    /** GPRS predective delay class 2 */
    SIMAT_GDC_2                   = 0x02,
    /** GPRS predective delay class 3 */
    SIMAT_GDC_3                   = 0x03,
    /** GPRS predective delay class 4 */
    SIMAT_GDC_4                   = 0x04
}
SimatGprsDelayClass;

/** Applications can request differing levels of reliability for
 * its data depending on its tolerance to data loss.
 */
typedef enum SimatGprsRelClassTag
{
    /** GPRS relaibility class 1 */
    SIMAT_GRC_1                   = 0x01,
    /** GPRS relaibility class 2 */
    SIMAT_GRC_2                   = 0x02,
    /** GPRS relaibility class 3 */
    SIMAT_GRC_3                   = 0x03,
    /** GPRS relaibility class 4 */
    SIMAT_GRC_4                   = 0x04,
    /** GPRS relaibility class 5 */
    SIMAT_GRC_5                   = 0x05
}
SimatGprsRelClass;

/** Peak throughput class describes the throughput required for burst of
 * specified size.
 */
typedef enum SimatGprsPeakThrClassTag
{
    /** Peak throughput class 1 */
    SIMAT_GPTC_1                  = 0x01,
    /** Peak throughput class 2 */
    SIMAT_GPTC_2                  = 0x02,
    /** Peak throughput class 3 */
    SIMAT_GPTC_3                  = 0x03,
    /** Peak throughput class 4 */
    SIMAT_GPTC_4                  = 0x04,
    /** Peak throughput class 5 */
    SIMAT_GPTC_5                  = 0x05,
    /** Peak throughput class 6 */
    SIMAT_GPTC_6                  = 0x06,
    /** Peak throughput class 7 */
    SIMAT_GPTC_7                  = 0x07,
    /** Peak throughput class 8 */
    SIMAT_GPTC_8                  = 0x08,
    /** Peak throughput class 9 */
    SIMAT_GPTC_9                  = 0x09
}
SimatGprsPeakThrClass;

/** Mean throughput class is the average data transfer rate over a period of time measured in
 * octets per hour.
 */
typedef enum SimatGprsMeanThrClassTag
{
    /** Mean throughput class 1 */
    SIMAT_GMTC_1                  = 0x01,
    /** Peak throughput class 2 */
    SIMAT_GMTC_2                  = 0x02,
    /** Peak throughput class 3 */
    SIMAT_GMTC_3                  = 0x03,
    /** Peak throughput class 4 */
    SIMAT_GMTC_4                  = 0x04,
    /** Peak throughput class 5 */
    SIMAT_GMTC_5                  = 0x05,
    /** Peak throughput class 6 */
    SIMAT_GMTC_6                  = 0x06,
    /** Peak throughput class 7 */
    SIMAT_GMTC_7                  = 0x07,
    /** Peak throughput class 8 */
    SIMAT_GMTC_8                  = 0x08,
    /** Peak throughput class 9 */
    SIMAT_GMTC_9                  = 0x09,
    /** Peak throughput class 10 */
    SIMAT_GMTC_10                 = 0x0A,
    /** Peak throughput class 11 */
    SIMAT_GMTC_11                 = 0x0B,
    /** Peak throughput class 12 */
    SIMAT_GMTC_12                 = 0x0C,
    /** Peak throughput class 13 */
    SIMAT_GMTC_13                 = 0x0D,
    /** Peak throughput class 14 */
    SIMAT_GMTC_14                 = 0x0E,
    /** Peak throughput class 15 */
    SIMAT_GMTC_15                 = 0x0F,
    /** Peak throughput class 16 */
    SIMAT_GMTC_16                 = 0x10,
    /** Peak throughput class 17 */
    SIMAT_GMTC_17                 = 0x11,
    /** Peak throughput class 18 */
    SIMAT_GMTC_18                 = 0x12,
    /** Peak throughput class 31 */
    SIMAT_GMTC_31                 = 0x13
}
SimatGprsMeanThrClass;

/** Packet Data Protocol type element */
typedef enum SimatGprsPdpTypeTag
{
    /** PDP X.25 */
    SIMAT_GPDPT_X25               = 0x01,
    /** PDP IP */
    SIMAT_GPDPT_IP                = 0x02,
    /** PDP OSPIH */
    SIMAT_GPDPT_OSPIH             = 0x03,
    /** PDP PPP */
    SIMAT_GPDPT_PPP               = 0x05
}
SimatGprsPdpType;

/** SIMAT GPRS bearer element */
typedef struct SimatGprsBearerTag
{
    /** Specifies GPRS predictive delay classe */
    SimatGprsPrecClass            pc;
    /** Specifies GPRS delay classe */
    SimatGprsDelayClass           dc;
    /** Specifies GPRS relaibility class */
    SimatGprsRelClass             rc;
    /** Specifies peak throughput class */
    SimatGprsPeakThrClass         ptc;
    /** Specifies mean throughput class */
    SimatGprsMeanThrClass         mtc;
    /** Specifies PDP type*/
    SimatGprsPdpType              pdpType;
    /** Whether data is compressed */
    Boolean                       dataComp;
    /** Whether header is compressed */
    Boolean                       headerComp;
}
SimatGprsBearer;

/** Specifies the type of bearer */
typedef enum SimatBearerTypeTag
{
    /** CSD bearer */
    SIMAT_BEARER_TYPE_CSD         = 0x01,
    /** GPRS bearer */
    SIMAT_BEARER_TYPE_GPRS        = 0x02,
    /** Default bearer */
    SIMAT_BEARER_TYPE_DEFAULT     = 0x03,
    /* local link bearer */
    SIMAT_BEARER_TYPE_LOCAL_LINK  = 0x04,
    /* bluetooth bearer */
    SIMAT_BEARER_TYPE_BLUETOOTH   = 0x05,
    /* IrDA bearer */
    SIMAT_BEARER_TYPE_IRDA        = 0x06,
    /* RS232 bearer */
    SIMAT_BEARER_TYPE_RS232       = 0x07,
    /* CDMA2000 packet data bearer */
    SIMAT_BEARER_TYPE_CDMA_PS     = 0x08,
    /* UTRAN packet service with extended parameters */
    SIMAT_BEARER_TYPE_UTRAN_WITH_EXTEND_PARA  = 0x09,
    /* I-WLAN */
    SIMAT_BEARER_TYPE_I_WLAN                  = 0x0A,
    /* ETRAN mapped UTRAN packet service */
    SIMAT_BEARER_TYPE_EUTRAN_MAPPED_UTRAN     = 0x0B
}
SimatBearerType;

/** Determines bearer info based on the type of bearer */
typedef union SimatBearerInfoTag
{
    /** CSD bearer */
    SimatCsdBearer                csd;
    /** GPRS bearer */
    SimatGprsBearer               gprs;
    /** Default bearer */
    Boolean                       useDefault;
}
SimatBearerInfo;

/** Describes the bearer */
typedef struct SimatBearerDescriptionTag
{
    /** Bearer type
\assoc UNION \ref bearerInfo */
    SimatBearerType               bearerType;
    /** Bearer info */
    SimatBearerInfo               bearerInfo;
}
SimatBearerDescription;

/** SIMAT buffer information element */
typedef enum SimatBufferInfoTag
{
    /** No further information */
    SIMAT_BUFF_NO_FURTHER_INFO    = 0x00,
    /** Receiver buffer is full */
    SIMAT_BUFF_RX_BUFFER_FULL     = 0x01,
    /** Receiver buffer is empty */
    SIMAT_BUFF_RX_BUFFER_EMPTY    = 0x02,
    /** Transmitter buffer is full */
    SIMAT_BUFF_TX_BUFFER_FULL     = 0x03,
    /** Transmitter buffer is empty */
    SIMAT_BUFF_TX_BUFFER_EMPTY    = 0x04,
    /** Buffer link is dropped */
    SIMAT_BUFF_LINK_DROPPED       = 0x05
}
SimatBufferInfo;

/** Other Address type.
 * The type of address and the address shall be provided by the terminal.
*/
typedef enum SimatOtherAddressTypeTag
{
    /** IPv4 address */
    SIMAT_OTH_ADDR_TYPE_IPV4      = 0x21,
    /** IPv6 address */
    SIMAT_OTH_ADDR_TYPE_IPV6      = 0x57,  /*according to 11.14 revision 8.11.0*/
    SIMAT_OTH_ADDR_TYPE_IPV6_BIS  = 0x97   /**< according to 11.14 revision 8.6.0*/
}
SimatOtherAddressType;

/** IPV4 address data */
typedef struct SimatIpv4AddressTag
{
    /** Indicates the length of IPV4 address */
    Int8                          length;
    /** Holds the IPV4 address */
    Int8                          data[SIMAT_MAX_IPV4_ADDR_LENGTH];
}
SimatIpv4Address;

/** IPV6 address data */
typedef struct SimatIpv6AddressTag
{
    /** Indicates the length of IPV6 address */
    Int8                          length;
    /** Holds the IPV6 address */
    Int8                          data[SIMAT_MAX_IPV6_ADDR_LENGTH];
}
SimatIpv6Address;

/** Specifies the IP(V4/V6) address  */
typedef union SimatOtherAddressTag
{
    /** IPV4 address data */
    SimatIpv4Address              ipv4Addr;
    /** IPV6 address data */
    SimatIpv6Address              ipv6Addr;
}
SimatOtherAddress;

/** SIMAT channel information */
typedef struct SimatChannelInfoTag
{
    /** Identifies the channel */
    Int8                          id;
    /** Indicates if link is established */
    Boolean                       linkEstablished;
    /** Buffer information */
    SimatBufferInfo               bufferInfo;
}
SimatChannelInfo;

/** Contain the status of the channel.*/
typedef struct SimatChannelStatusTag
{
    /** Indicates the number of channels*/
    Int8                          numChannels;
    /** Holds a list status of channels */
    SimatChannelInfo              channel[SIMAT_MAX_NUM_CHANNELS];
}
SimatChannelStatus;

/** UICC/terminal interface transport level element */
typedef enum SimatTransLevTypeTag
{
    /** UDP, UICC in client mode, remote connection */
    SIMAT_TL_TYPE_UDP             = 0x01,
    /** TCP, UICC in client mode, remote connection */
    SIMAT_TL_TYPE_TCP             = 0x02
}
SimatTransLevType;

/** UICC/terminal interface transport level element */
typedef struct SimatTransportLevelTag
{
    /** Specifies the type of transport level  */
    SimatTransLevType             type;
    /** Port number */
    Int16                         portNumber;
}
SimatTransportLevel;

/** URL Information */
typedef struct SimatUrlTag
{
    /** The length of URL.
     * A null URL shall be coded with Length = '00', and no Value part.
     * In that case, the terminal shall use the default URL.
     */
    Int8                          length;
    /** Contains URL data */
    Int8                          data[SIMAT_MAX_URL_LENGTH];
}
SimatUrl;

/** Union holding any response data which may be returned
 * from the SIM as a result of a Call Control Envelope command
 * being sent to the SIM.
 */
typedef union SimatCallControlInfoTag
{
    /** Call control call set up */
    SimatCcCallSetUp              ccCallSetUp;
    /** Call control send SS */
    SimatCcSsOperation            ccSsOperation;
    /** Call control send USSD */
    SimatCcUssdOperation          ccUssdOperation;
    /** unused by the PS, but BL needs the InfoType enum for MOSM Control */
    Int8                          unusedField;
}
SimatCallControlInfo;

/**
** New definitions for SIM Application Toolkit Release 97 implementation
*/

typedef enum SimatLocationStatusTag
{
    /** Normal service */
    SIMAT_LS_NORMAL_SERVICE       = 0x00,
    /** limited service */
    SIMAT_LS_LIMITED_SERVICE      = 0x01,
    /** No service */
    SIMAT_LS_NO_SERVICE           = 0x02
}
SimatLocationStatus;

/** MO-SMS control by SIMAT element .
 * This enumeration type is only used by software built with the UPGRADE_SAT97 switch.*/
typedef enum SimatMoSmControlResultTag
{
    /** MO-SMS control with allowed with no modification */
    SIMAT_MOSM_ALLOWED_NO_MODS    = 0x00,
    /** MO-SMS control with not allowed */
    SIMAT_MOSM_NOT_ALLOWED        = 0x01,
    /** MO-SMS control with allowed with modification */
    SIMAT_MOSM_ALLOWED_WITH_MODS  = 0x02
}
SimatMoSmControlResult;

/** This structure is only used by software built with the UPGRADE_SAT97 switch. */
typedef struct SimatEventListTag
{
    /** Indicates if existing event list to be removed */
    Boolean                       removeExistingEventList;
    /** Whether a MT call */
    Boolean                       mtCall;
    /** whether or not call connected */
    Boolean                       callConnected;
    /** whether or not call disconnected */
    Boolean                       callDisconnected;
    /** Location status is present */
    Boolean                       locationStatus;
    /** User activity is present */
    Boolean                       userActivity;
    /** Screen available in idle mode */
    Boolean                       idleScreenAvailable;
    /** Langage can be selected */
    Boolean                       languageSelection;
    /* Browser has been closed */
    Boolean                       browserTermination;
    /** Data is available */
    Boolean                       dataAvailable;
    Boolean                       channelStatus;
    /** Is accesss technology changed */
    Boolean                       accessTechnologyChange;
    /** Display parameters that have been changed */
    Boolean                       displayParamsChanged;
    /** network rejection event */
    Boolean                       networkReject;
    /** data connection status change event */
    Boolean                       dataConnStatusChange;
}
SimatEventList;

/** SIMAT transaction identifier element */
typedef struct SimatTansactionIdInfoTag
{
    /** Identifier value */
    Int8                          idValue;
    /** Identifier flag */
    Boolean                       idFlag;
}
SimatTransactionIdInfo;

/** This structure is only used by software built with the UPGRADE_SAT97 switch. */
typedef struct SimatTransactionIdListTag
{
    /** The number of IDs in the list
\assoc ARRAY \ref idInfo */
    Int8                          numListIds;
    /** Lists the IDs */
    SimatTransactionIdInfo        idInfo[SIMAT_MAX_NUM_TRANS_ID];
}
SimatTransactionIdList;

/** Access Technology Change Event element.
 * When the terminal detects a change in its current access technology the terminal
 * shall inform the UICC that this has occurred.
 */
typedef struct SimatEventAccessTechnologyTag
{
    /** Contains only one event (Access Technology Change). */
    SimatEventList                eventList;
    /** Contain the current access technology that the terminal is using.*/
    SimatAccessTechnology         accessTechnology;
}
SimatEventAccessTechnology;

/** Display parameters changed event element.
 * When the screen of the terminal is resized, the terminal shall inform the UICC that
 * this has occurred
 */
typedef struct SimatDisplayParamsEventTag
{
    /** Contains only one event (display parameters changed). */
    SimatEventList                eventList;
    /** Contains the current terminal's screen parameters. */
    SimatDisplayParams            newParams;
}
SimatDisplayParamsEvent;

/** Call connection event element.
 * When the terminal receives an incoming call connect
 * message (in the case of an MT call), or when
 * the terminal sends an outgoing call connect message (in the case of an MO call),
 * the terminal shall inform the UICC that this has occurred.
 */
typedef struct SimatEventCallConnTag
{
    /** Whether connecting at the near end or connecting at the far end */
    Boolean                       isNearEndConnection;
    /** Holds only one event (call connected) */
    SimatEventList                eventList;
    /** This shall be the Transaction Identifier in the call connect message.*/
    SimatTransactionIdList        transId;
}
SimatEventCallConn;

/** Call disconnected event element.
 * While the terminal is not in the NULL state (i.e. has sent or
 * received a call setup message) and the call is disconnected,
 * the terminal shall inform the UICC that this has occurred
 */
typedef struct SimatEventCallDisconnTag
{
    /** Whether connecting at the near end or connecting at the far end */
    Boolean                       isNearEndDisconnection;
    /** Contain only one event (call disconnected) */
    SimatEventList                eventList;
    /** The transaction identifier data object shall contain a list of
     * the transaction identifiers for each of the calls being disconnected.
     */
    SimatTransactionIdList        transId;
    /** Indicates if cause is present */
    Boolean                       causePresent;
    /** Reflect the cause information element sent or received in the
     * disconnect message triggering the ENVELOPE command
     */
    Cause                         cause;  /* job115982: for encoding CC-Cause IE */
}
SimatEventCallDisconn;

/** Idle screen available event element.
 * If the idle screen available event is part of the current event list
 * then the terminal shall follow the procedure below <UL>
 * <LI> when the terminal next enters a state where it would accept rather
 * than reject a DISPLAY TEXT command of normal priority, the terminal shall
 * inform the UICC that this has occurred, by using the ENVELOPE (EVENT
 * DOWNLOAD - idle screen available) command
* <LI> as a result of sending this command to the UICC, the terminal shall
* remove the idle screen available event from its current event list.
* This is in order for the terminal to report this event only once after the event has
* been requested by the UICC.
*</UL>
*/
typedef struct SimatEventIdleScrnAvailTag
{
    /** Contain only one event (idle screen available) */
    SimatEventList                eventList;
}
SimatEventIdleScrnAvail;

/** Language selection event element.
 * When the terminal changes the currently used language, the terminal
 * shall inform the UICC that this has occurred
 */
typedef struct SimatEventLangSelectionTag
{
    /** Contains only one event (language selection) */
    SimatEventList                eventList;
    /** Contains the currently used language of the terminal.*/
    Int8                          languageCode[SIMAT_NUM_LANG_CODE_CHARS];
}
SimatEventLangSelection;

/** Location status event element .
 * When the terminal enters the idle state with the result that either
 * the Location status or Location information has been changed or updated,
 * the terminal shall inform the UICC that this has occurred.
 */
typedef struct SimatEventLocationStatusTag
{
    /** Contain only one event (location status ) */
    SimatEventList                eventList;
    /** Contain the current service state of the terminal.*/
    SimatLocationStatus           locStatus;
    /** Contains the details of the network, location area and cell that
     * have been selected (shall only be included if the Location status
     * object indicates Normal).*/
    SimatLocationInfo             locInfo;
}
SimatEventLocationStatus;

/** MT call event element.
 * When the terminal receives an incoming call setup message,
 * the terminal shall inform the UICC that this has occurred,
 * by using the ENVELOPE (EVENT DOWNLOAD - MT call)
 */
typedef struct SimatEventMtCallTag
{
    /** Contain the events */
    SimatEventList                eventList;
    /** This shall be the Transaction Identifier in the call setup message from the network.*/
    SimatTransactionIdList        transId;
    /** Indicates if address is present
\assoc PRESENT \ref address */
    Boolean                       addressPresent;
    /** Holds the Calling Party number received by the terminal in the call setup message */
    SimatAddress                  address;
    /** Indicates if called party sub address is present
\assoc PRESENT \ref cps */
    Boolean                       cpsPresent;
    /** Holds the Calling Party Subaddress as received by the terminal in
     * the call setup message.
     */
    CallingPartySubaddress        cps;       /* job 100548 calling party subaddress not called party */
}
SimatEventMtCall;

/** User activity event element.
 * If the user activity event is part of the current event list then the terminal shall
 * follow the procedure below <UL>
 * <LI> When the terminal next detects some user activity (e.g. a key-press, removal of key-lock),
 * the terminal shall inform the UICC that this has occurred, by using the ENVELOPE
 * (EVENT DOWNLOAD - user activity)command.
 *
 * <LI> As a result of sending this command to the UICC, the terminal shall remove the
 * user activity event from its current event list. This is in order for the terminal
 * to report this event only once after the event has been requested by the UICC.
 *</UL>
 */
typedef struct SimatEventUserActivityTag
{
    /** Contains only one event (user activity) */
    SimatEventList                eventList;
}
SimatEventUserActivity;

/** Provides the cause of terminating browser */
typedef enum SimatBrowserTermCauseTag
{
    /** User has terminated the browser */
    SIMAT_BT_CAUSE_USER_TERM      = 0x00,
    /** Browser has been terminating due to an error */
    SIMAT_BT_CAUSE_ERROR_TERM     = 0x01
}
SimatBrowserTermCause;

/** Browser termination event element.
 * When the browser is terminated either by the user action or by an error,
 * the terminal shall inform the UICC that this has occurred.
*/
typedef struct SimatEventBrowserTermTag
{
    /** Contains only one event (browser termination) */
    SimatEventList                eventList;
    /** Contain the browser termination cause.*/
    SimatBrowserTermCause         cause;
}
SimatEventBrowserTerm;

/** Data available event element.
 * If the targeted channel buffer is empty when new data arrives in it, the terminal
 * shall inform the UICC that this has occurred.
 */
typedef struct SimatEventDataAvailableTag
{
    /** Contains only one event (data available) */
    SimatEventList                eventList;
    /** Channel info of the channel on which the event occurred. */
    SimatChannelInfo              channelInfo;
    /** Contain the number of bytes received (available in the channel buffer) and if
     * more than 255 bytes are available, "FF" is used.
     */
    Int8                          channelDataLength;
}
SimatEventDataAvailable;

/** Channel status event element.
 */
typedef struct SimatEventChannelStatusTag
{
    /** Contains only one event (channel status) */
    SimatEventList                eventList;
    /** Channel info */
    SimatChannelInfo              channelInfo;
}
SimatEventChannelStatus;

/** update or attach type used in the registration request message.
 */
typedef enum SimatUpdateAttachTypeTag
{
  /** normal location updating in the case of Location updating request message */
  SIMAT_REG_TYPE_NORMAL_LOCATION_UPDATING    = 0x0,
  /* periodic updating in the case of Location updating request message */
  SIMAT_REG_TYPE_PERIODIC_LA_UPDATING        = 0x1,
  /* imsi attach in the case of Location updating request message */
  SIMAT_REG_TYPE_IMSI_ATTACH_                = 0x2,
  /* gprs attach in the case of GPRS ATTACH request message */
  SIMAT_REG_TYPE_GPRS_ATTACH                 = 0x3,
  /* combined gprs/imsi attach in the case of GPRS ATTACH request message */
  SIMAT_REG_TYPE_COMB_GPRS_IMSI_ATTACH       = 0x4,
  /* RA updating in the case of ROUTING area update request message */
  SIMAT_REG_TYPE_RA_UPDATING                 = 0x5,
  /* combined RA/LA updating in the case of ROUTING area update request message */
  SIMAT_REG_TYPE_COMB_RA_LA_UPDATING         = 0x6,
  /* combined RA/LA updating with IMSI attach in the case of ROUTING area update request message */
  SIMAT_REG_TYPE_COMB_RA_LA_UPDATING_WITH_IMSI_ATTACH = 0x7,
  /* periodic updating in the case of ROUTING area update request message */
  SIMAT_REG_TYPE_PERIODIC_RA_UPDATING        = 0x8,
  /* EPS attach in the case of EMM attach request message */
  SIMAT_REG_TYPE_EPS_ATTACH                  = 0x9,
  /* combined EPS/IMSI attach in the case of EMM attach request message */
  SIMAT_REG_TYPE_COMB_EPS_IMSI_ATTACH        = 0xa,
  /* TA updating in the case of EMM tracking area update request message */
  SIMAT_REG_TYPE_TA_UPDATING                 = 0xb,
  /* combined TA/LA updating in the case of EMM tracking area update request message */
  SIMAT_REG_TYPE_COMB_TA_LA_UPDATING         = 0xc,
  /* combined TA/LA updating with IMSI attach in the case of EMM tracking area update request message */
  SIMAT_REG_TYPE_COMB_TA_LA_UPDATING_WITH_IMSI_ATTACH = 0xd,
  /* periodic updating in the case of EMM tracking area update request message */
  SIMAT_REG_TYPE_PERIODIC_TA_UPDATING        = 0xe
}
SimatUpdateAttachType;

/** network reject event element.
 */
typedef struct SimatEventNetworkRejectTag
{
    /** Contains only one event (channel status) */
    SimatEventList                eventList;
    /** access technology of the rejection network */
    SimatAccessTechnology         accessTechnology;
    /** update or attach type was used in the registration request */
    SimatUpdateAttachType         updateAttachType;
    /** the identification of the rejecting network */
    Lai                           tai;
    /** the cause code received in the rejection message */
    Int8                          emmCause;
    /** extended cause code received in the rejection message or not */
    Boolean                       extEmmCausePresent;
    /** the extended cause code received in the rejection message */
    Int8                          extEmmCause;
}
SimatEventNetworkReject;

/** Data connection status values.
 */
typedef enum SimatDataConnectionStatusTag
{
  /** Data connection successful;
      succesfull means accepted by the network and completed by the device */
  SIMAT_DATA_CONNECTION_SUCCESSFUL   = 0x00,
  /** Data connection rejected */
  SIMAT_DATA_CONNECTION_REJECTED     = 0x01,
  /** Data connection dropped or deactivated */
  SIMAT_DATA_CONNECTION_DEACTIVATED  = 0x02
}
SimatDataConnectionStatus;

/** Data connection Type values.
 */
typedef enum SimatDataConnectionTypeTag
{
  /** Data connection type PDP */
  SIMAT_DATA_CONNECTION_TYPE_PDP   = 0x00,
  /** Data connection type PDN */
  SIMAT_DATA_CONNECTION_TYPE_PDN   = 0x01
}
SimatDataConnectiontype;

/** data connection status change event element.
 */
typedef struct SimatEventDataConnStatusChangeTag
{
    /** Contains only one event (channel status) */
    SimatEventList                eventList;
    /** data connection status */
    SimatDataConnectionStatus    dataConnectionStatus;
    /** data connection type */
    SimatDataConnectiontype      dataConnectionType;
    /** the identification of the rejecting network */
    SimatTransactionIdList       transId;
    /** the current date, time and time zone */
    SmsTimeStamp                 dttz;
    /** Contain the current location state of the terminal.*/
    SimatLocationStatus          locStatus;
    /** EPS PDN connection activation parameters received in the connection status message or not */
    Boolean                      pdnContextInfoPresent;
    /** EPS PDN connection activation parameters */
    SimatCcPdnContextInfo        pdnContextInfo;
}
SimatEventDataConnStatusChange;

/** A set of events for the terminal to monitor can be supplied by the
 * UICC using the proactive command SET UP EVENT LIST.
 */
typedef enum SimatEventDownloadTypeTag
{
    /** MT call event */
    SIMAT_EVENT_MT_CALL           = 0x00,
    /** Call connected event */
    SIMAT_EVENT_CALL_CONN         = 0x01,
    /** Call disconnected event */
    SIMAT_EVENT_CALL_DISCONN      = 0x02,
    /** Location status event */
    SIMAT_EVENT_LOCATION_STATUS   = 0x03,
    /** User activity event */
    SIMAT_EVENT_USER_ACTIVITY     = 0x04,
    /** Idle screen available event */
    SIMAT_EVENT_IDLE_SCRN_AVAIL   = 0x05,
    /** Language selection event */
    SIMAT_EVENT_LANG_SELECTION    = 0x07,
    /** Browser termination event*/
    SIMAT_EVENT_BROWSER_TERM      = 0x08,
    /** Data available event */
    SIMAT_EVENT_DATA_AVAILABLE    = 0x09,
    /** Channel status event */
    SIMAT_EVENT_CHANNEL_STATUS    = 0x0a,
    /** Access Technology Change Event */
    SIMAT_EVENT_ACCESS_TECHNOLOGY = 0x0b,
    /** Display parameters changed event */
    SIMAT_EVENT_DISPLAY_PARAMS    = 0x0c,
    /** Network rejection event  */
    SIMAT_EVENT_NETWORK_REJECT    = 0x12,
    /** Data Connection Status Change event */
    SIMAT_EVENT_DATA_CONN_STATUS_CHANGE = 0x1d
}
SimatEventDownloadType;

/** The event download mechanism is used to transfer details of the specific event to the
 * UICC, when it occurs
 */
typedef union SimatEventDownloadTag
{
    /** MT call event */
    SimatEventMtCall              mtCall;
    /** Call connected event */
    SimatEventCallConn            callConn;
    /** Call disconnected event */
    SimatEventCallDisconn         callDisconn;
    /** Location status event */
    SimatEventLocationStatus      locStatus;
    /** User activity event */
    SimatEventUserActivity        userAct;
    /** Idle screen available event */
    SimatEventIdleScrnAvail       idleScrn;
    /** Language selection event */
    SimatEventLangSelection       languageSelection;
    /** Browser termination event*/
    SimatEventBrowserTerm         browserTermination;
    /** Data available event */
    SimatEventDataAvailable       dataAvailable;
    /** Channel status event */
    SimatEventChannelStatus       channelStatus;
    /** Access Technology Change Event */
   SimatEventAccessTechnology     accessTechno;
   /** Display parameters changed event */
   SimatDisplayParamsEvent        displayParams;
   /** Network reject event */
   SimatEventNetworkReject        networkReject;
   /** Data connection status change event */
   SimatEventDataConnStatusChange dataConnStatusChange;
}
SimatEventDownload;

/** This structure is only used by software built with the UPGRADE_SAT97 switch.*/
typedef struct SimatNextActionIndicatorTag
{
    /** The number of NAI
\assoc ARRAY \ref nai */
    Int8                          numNai;
    /** Network Access Identifier value */
    Int8                          nai[SIMAT_MAX_NUM_NAI];
}
SimatNextActionIndicator;

/** Defines the type of additional information given in the result
 * of the Send USSD proactive command.
 */
typedef enum SimatUssdAdditResultTypeTag
{
    /** No additional information */
    SIMAT_AIUSSD_NO_ADDIT_INFO    = 0x00,
    /** Additional information for SEND USSD*/
    SIMAT_AIUSSD_USSD_RESULT      = 0x01,
    /** Additional information for ME problem */
    SIMAT_AIUSSD_ME_PROBLEM       = 0x02,
    /** Additional information for network problem*/
    SIMAT_AIUSSD_NET_PROBLEM      = 0x03,
    /** Additional information for USSD problem */
    SIMAT_AIUSSD_USSD_PROBLEM     = 0x04
}
SimatUssdAdditResultType;

/** SIMAT USSD additional result */
typedef union SimatUssdAdditResultTag
{
    /** No additional information */
    Int8                          noAdditInfo;
    /** Additional information for SEND USSD*/
    SimatTextString               ussdResult;
    /** Additional information for ME problem */
    SimatMeProblemAddInfo         meProblem;
    /** Additional information for network problem*/
    GsmCause                      netProblem;
    /** Additional information for USSD problem */
    Int8                          ussdProblem;
}
SimatUssdAdditResult;

/*
** The following types required for Terminal Response when a call related
** proactive command has been modified by Call Control by SIM.
** These are for $(TRCC)$ support for Release 97.
*/

/** Additional info for interaction with Call Control or MO SM Control */
typedef enum SimatCcProblemAddInfoTag
{
    /** No specific cause can be given */
    SIMAT_CC_INFO_NO_CAUSE_GIVEN    = 0x00,
    /** Action not allowed */
    SIMAT_CC_INFO_ACT_NOT_ALLOWED   = 0x01,
    /** The type of request has changed */
    SIMAT_CC_INFO_REQ_TYPE_CHANGED  = 0x02
}
SimatCcProblemAddInfo;

/** This structure is only used by software built with the UPGRADE_SAT97 switch. */
typedef struct SimatTrccSsAddInfoTag
{
    /** Specifies the type of additional information result
    \assoc UNION \ref additInfo */
    SimatSsAdditResultType        additInfoType;
    /** Additional information given in the result of the Send SS proactive command */
    SimatSsAdditResult            additInfo;
}
SimatTrccSsAddInfo;

/** This structure is only used by software built with the UPGRADE_SAT97 switch. */
typedef struct SimatTrccUssdAddInfoTag
{
    /** Specifies the type of additional information result
    \assoc UNION \ref additInfo */
    SimatUssdAdditResultType      additInfoType;
    /** Additional information given in the result of the Send USSD proactive command */
    SimatUssdAdditResult          additInfo;
}
SimatTrccUssdAddInfo;

/** This enumeration type is only used by software built with the UPGRADE_SAT97 switch. */
typedef enum SimatTrccResult2InfoTypeTag
{
    /** ME problem information */
    SIMAT_TRCC_RES2_ME_PROB_INFO,
    /** Network problem information */
    SIMAT_TRCC_RES2_NET_PROB_INFO,
    /** Send SS problem information */
    SIMAT_TRCC_RES2_SS_INFO,
    /** Send USSD problem information */
    SIMAT_TRCC_RES2_USSD_INFO
}
SimatTrccResult2InfoType;

/** SimatTrccResult2Info */
typedef union SimatTrccResult2InfoTag
{
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
    /** Network problem information */
    GsmCause                      netProblem;
    /** Send SS problem information */
    SimatTrccSsAddInfo            ssInfo;
    /** Send USSD problem information */
    SimatTrccUssdAddInfo          ussdInfo;
}
SimatTrccResult2Info;

/** This structure is only used by software built with the UPGRADE_SAT97 switch.*/
typedef struct SimatTrccInfoTag
{
    /** call control requsted action info (see GSM 11.14, section 12.30) */
    Boolean                       ccInfoPresent;
    /**  The type of response data returned from the SIM as a result
     * of a call control.

\assoc UNION \ref ccInfo */
    SimatCallControlInfoType      ccInfoType;
    /** The response data returned from the SIM as a result
     * of a call control.
     */
    SimatCallControlInfo          ccInfo;
    /** Indicates if alpha Id is present */
    Boolean                       alphaIdPresent;
    /** Alpha identifier for the proactive command */
    SimatAlphaIdentifier          alphaId;

    /** Additional Result Data Object 2 info (see GSM 11.14, section 6.8) */
    SimatGeneralResult            generalResult2;
    /** Indicates the type of result info
\assoc UNION \ref trccResult2Info */
    SimatTrccResult2InfoType      trccResult2InfoType;
    /** trccResult2Info */
    SimatTrccResult2Info          trccResult2Info;
}
SimatTrccInfo;

/*this structure describes the Application ID see 101 220 for coding */
/** SimatAid is of type SimUiccAid.*/
typedef SimUiccAid   SimatAid;

#endif
