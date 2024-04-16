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
 SIM toolkit AFSA signals
 **************************************************************************/

#if !defined (AFSA_SIG_H)
#define       AFSA_SIG_H
#endif

/***************************************************************************
 * Nested Include Files
 **************************************************************************/
#include <alsi_sig.h>
#include <alsa_sig.h>

/** \defgroup SigAfsa  AFSA Signalling Interface
 * \ingroup Prd3gDmNas
 * \ingroup Sig
 * Application Foreground Layer to SAT Application Layer interface.
 * The interface between the SIMAT and the Foreground Layer is known as AFSA interface. 
 * This interface deals with two peer-to-peer entities and is principally 
 * concerned with modification of the display and its behaviour in response 
 * to commands issued by the SIM. 
 *
 * <B> The context of the AFSA interface is illustrated in the following diagram.</B>
 *
 * \image html SAT_Software_Interfaces.png
 * @{
 */

/****************************************************************************
 * Macros
 ****************************************************************************/
/** The maximum length of image instance data */
#define AFSA_IMAGE_INST_DATA_SIZE SIM_MAX_IMAGE_INST_DATA_SIZE

/** Size of the signal carrying the Image instance data, this can be variable 
 * depending upon how much data is stored in the signal   
 * (The signal size is based upon the equivalent ALSA signal size).
 */
#define AFSA_READ_IMAGE_INST_DATA_CNF_SIZE(length)    \
              (sizeof(AfsaReadImageInstDataCnf)       \
               - (SIM_MAX_IMAGE_INST_DATA_SIZE)       \
               + (length))


/***************************************************************************
 * Types
 **************************************************************************/
/** MMI State element. */
typedef enum SimatMMIStateTag
{
  /** MMI is in idle mode */
  SIMAT_MMI_IDLE               = 0x00,
  /** MMI is busy */
  SIMAT_MMI_BUSY               = 0x01
}
SimatMMIState;

typedef struct SimatDispParamsValidityTag
{
    Boolean     numCharDownValid;
    Boolean     screenSizingValid;
    Boolean     numCharAcrossValid;
    Boolean     varSizeFontsValid;
    Boolean     displayResizeValid;
    Boolean     textWrappingValid;
    Boolean     textScrollingValid;
    Boolean     widthReductionValid;
  
}
SimatDispParamsValidity;

/***************************************************************************
 * Signal Definitions
 **************************************************************************/

/** Indication by SIMAT Task to peer level application foreground 
 * of changes to the SIM configuration. 
 */
typedef struct AfsaRefreshIndTag
{
    /** SIMAT Command reference */
    Int16                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Defines which refresh mode is required. */
    SimatRefreshQual              commandQual;
    /** The list of files to be refreshed for Refresh Command 
     * types File Change Notification and SIM Initialisation 
     * and File Change Notification 
     */
    SimatFileList                 fileInfo;
     /** Application ID of the application that should be refreshed. */
    SimatAid                      aid;
}
AfsaRefreshInd;

/** Returns the response data for the Refresh proactive SIM command.*/
typedef struct AfsaRefreshRspTag
{
    /** Command reference copied from AfsaRefreshInd */
    Int16                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Indicates whether a problem was encountered with the ME in 
     * attempting to perform the proactive command.
     */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command. */
    SimatMeProblemAddInfo         meProblem;
}
AfsaRefreshRsp;

/** This signal returns the state of the MMI. */
typedef struct AfsaDisplayStatusIndTag
{
  /** The status of the handset. */
  SimatMMIState                  simatMMIState;
}
AfsaDisplayStatusInd;

/** Indication regarding the activity on the user interface. */
typedef struct AfsaUserActivityIndTag
{
  /** The activity on the user interface */
  Int16                  userActivity;
}
AfsaUserActivityInd;

/** indication from MMI that display parameters have changed */
typedef struct AfsaDisplayParamsChangedIndTag
{
    SimatDispParamsValidity paramValidity;
    /** the new display parameters received from the MMI */ 
    SimatDisplayParams     newDispParams;
}
AfsaDisplayParamsChangedInd;

/** Indication of language selection.
 * Soon after the ME powers on, the FL should send an AfsaLanguageSelectionInd 
 * to the SIMAT task. The ME will use the value set in the AfsaLanguageSelectionInd 
 * to get the currently used language, and will return this value to the SIM manager 
 * in the AlsaProvideLocalInfoRsp.
 */
typedef struct AfsaLanguageSelectionIndTag
{
  /** Indicates the selected language. */
  Int8          languageCode[SIMAT_NUM_LANG_CODE_CHARS];
}
AfsaLanguageSelectionInd;

/* job800120: Made available for general SAT99 usage */
/** This signal is sent to the SIMAT task when the WAP browser is closed.
 * When the WAP browser is closed, the FL sends an AfsaCloseBrowserInd to the SIMAT task. 
 */
typedef struct AfsaCloseBrowserIndTag
{
  /** Indicates whether or not the browser was closed by the user.*/
  Boolean userTermination;
}
AfsaCloseBrowserInd;

/** Requestes to register/deregister with SIMAT task. */
typedef struct AfsaStRegisterTaskReqTag
{
    /** Task ID of the requesting task */
    TaskId  taskId;
    /** Indicates if task to be registered/not registered */
    Boolean isRegistered;
}
AfsaStRegisterTaskReq;

/** Confirmation of the corresponding request. */
typedef struct AfsaStRegisterTaskCnfTag
{
    /** Id of registering/deregistering task */
    TaskId  taskId;
    /** Indicates if task is registered/not registered */
    Boolean isRegistered;
}
AfsaStRegisterTaskCnf;


/* icon data fetching... */

/** This signal allows an MMI task to request a list of image 
 * instance information for a particular image record number.
 * The Foreground layer signals which icon record number it 
 * requires by sending an AfsaReadImageRecReq to the SAT Task 
 * which will query the SIM Manager and return an AfsaReadImageRecCnf 
 * signal. 
 */
typedef struct AfsaReadImageRecReqTag
{
  /** Task ID of the requesting task (Foreground Layer where to send result) */
  TaskId                        taskId; 
  /** Unique command reference provided by the Foreground Layer task.*/
  Int16                         simatCommandRef;
  /** Record of IMG LF file containing image instance data */
  Int8                          recordNum;  
}
AfsaReadImageRecReq;

/** The AfsaReadImageRecCnf signal returns a list of image 
 * instance information to the requesting MMI task.
 * This signal lists the width, height, coding scheme and file 
 * location for each of the available instances for the requested icon.
 */
typedef struct AfsaReadImageRecCnfTag
{
  /** Command reference of the corresponding request.*/
  Int16                         simatCommandRef;
  /** Status of the request as defined by the ALSA interface.*/
  SimRequestStatus              requestStatus;
  /** Record number of the requested icon.*/
  Int8                          recordNum;
  /** List of image instance information as defined by the ALSA interface.*/
  SimImageData                  image;
}
AfsaReadImageRecCnf;

/** The Foreground Layer selects which image instance data it requires 
 * by sending an AfsaReadImageInstDataReq detailing the file location of 
 * the image data it requires. 
 * The AfsaReadImageInstDataReq signal allows an MMI 
 * task to request the image data for a specific image instance 
 * using the instance data location information
 *  from an AfsaReadImageRecCnf signal.
 */
typedef struct AfsaReadImageInstDataReqTag
{
  /** Where to send result */
  TaskId                        taskId; 
  /** Unique command reference provided by the Foreground Layer task.*/
  Int16                         simatCommandRef;
  /** ID of the SIM Elementary File containing the image data */
  Int16                         efId;
  /** Start point of data in efId */
  Int16                         offset;  
  /** Amount of data to read (max 256 bytes per efId)*/
  Int16                         length; 
}
AfsaReadImageInstDataReq;

/** This signal returns the actual image instance data to the requesting task.
 * The SAT Task requests this information from the SIM Manager and returns 
 * it via an AfsaReadImageInstDataCnf signal. As the SIM can only return a 
 * maximum of 256 bytes in any one read, the SAT Task may have to make 
 * several reads from the SIM to fetch all of the image data. The SAT Task 
 * will pack the data into a single signal for the Foreground Layer. As the
 * size of the icon data may vary widely, the AfsaReadImageInstDataCnf signal 
 * is resizable.
 */
typedef struct AfsaReadImageInstDataCnfTag
{
  /** Command reference of the corresponding request. */
  Int16                         simatCommandRef;
  /** Status of the request as defined by the ALSA interface. */
  SimRequestStatus              requestStatus;
  /** Length of data read 
\assoc ARRAY \ref data */
  Int16                         dataLength;  
  /** The image instance data (Variable sized field of 
   * minimum size 256 bytes). 
   */
  Int8                          data[AFSA_IMAGE_INST_DATA_SIZE];
          /* alloc signal bigger than this*/
          /* Use macro AFSA_READ_IMAGE_INST_DATA_CNF_SIZE(length) */
}
AfsaReadImageInstDataCnf;

/** Lists all the instances of the Icon Image  from SIM card */
typedef struct AfsaListImageRecReqTag
{
  /** Where to send result */
  TaskId                        taskId;  
  /** Unique command reference provided by the Foreground Layer task.*/
  Int16                         simatCommandRef;
}
AfsaListImageRecReq;

/** This signal returns the image records. This primitive is sent as a response to AfsaListImageRecReq */
typedef AlsaListImageRecCnf AfsaListImageRecCnf;



/** Indication that the ME is to display a text message on the screen. 
 * This signal indicates both the text string format and the priority of the message. 
 */
typedef   AlsaDisplayTextInd     AfsaDisplayTextInd;

/** Returns the response data for the Display Text proactive SIM command.*/
typedef   AlsaDisplayTextRsp     AfsaDisplayTextRsp;

/** Indication that text is to be displayed and the user is to 
 * enter a single character in response to the text message.  
 * The signal determines whether help information is requested 
 * by the user, and specifies the valid character set for user 
 * input as part of the command qualifier data.
 */
typedef   AlsaGetInkeyInd    AfsaGetInkeyInd;

/** Returns the response data for the Get Inkey proactive SIM command.*/
typedef   AlsaGetInkeyRsp    AfsaGetInkeyRsp;


/** Indication that text is to be displayed and the user is to enter a response string.
 * The commandQual structure holds the command qualifier details for the proactive 
 * command, indicating the availability of help information, the valid character 
 * set for the text, the packing format, and level of visibility of the response 
 * string to the user.
 */
typedef   AlsaGetInputInd    AfsaGetInputInd;

/** Returns the response data for the Get Input proactive SIM command.*/
typedef   AlsaGetInputRsp    AfsaGetInputRsp;

/** This signal indicates that an audio tone is to be played. 
 * The tone, alpha identifier and duration information may not 
 * be provided by the SIM, and the relevant Boolean types indicate
 * if the information is present or not. 
 */
typedef   AlsaPlayToneInd    AfsaPlayToneInd;

/** Returns the response data for the Play Tone proactive SIM command.*/
typedef   AlsaPlayToneRsp    AfsaPlayToneRsp;

/** Indication that a set of menu items has been supplied to 
 * the application layer by the SIM for user navigation. 
 * It may alternatively indicate that the menu is to be 
 * removed from the menu structure of the ME, depending 
 * on the length and value of the first item data object 
 * in the Simat Structure.  Each menu item contains an identifier and a text string.
 */
typedef   AlsaSetUpMenuInd   AfsaSetUpMenuInd;

/** Returns the response data for the Set Up Menu proactive SIM command.*/
typedef   AlsaSetUpMenuRsp   AfsaSetUpMenuRsp;

/** Indication that the user is required to select an item from 
 * the list of text stings provided by the SIM
 */
typedef   AlsaSelectItemInd      AfsaSelectItemInd;

/** Returns the response data for the Select Item proactive SIM command.*/
typedef   AlsaSelectItemRsp      AfsaSelectItemRsp;

/** Request to select an item from the Menu provided by the proactive SIM */
typedef  AlsaMenuSelectionReq    AfsaMenuSelectionReq;

/** Confirmation or not that the SIM has been informed of the menu 
 * selection made by the user from the menu supplied by the proactive 
 * SIM. This includes an indication of whether this has resulted in a 
 * proactive  session being initiated on the SIM.
 */
typedef  AlsaMenuSelectionCnf    AfsaMenuSelectionCnf;


/** Indicates to set up idle mode text. */
typedef   AlsaSetUpIdleModeTextInd   AfsaSetUpIdleModeTextInd;

/** Confirmation to the corresponding request */
typedef   AlsaSetUpIdleModeTextRsp   AfsaSetUpIdleModeTextRsp;

/** Used by the SIM to send an AT command to the ME, as though 
 * initiated by an attached terminal.
 */
typedef   AlsaRunAtCommandInd        AfsaRunAtCommandInd;

/** This signal is sent as response to AfsaRunAtCommandInd.
 * Returns the response data for the Run AT Command Proactive SIM command.
 */
typedef   AlsaRunAtCommandRsp        AfsaRunAtCommandRsp;

/** Decoded information from the Language Notification proactive command. */
typedef   AlsaLanguageNotificationInd AfsaLanguageNotificationInd;

/** Confirmation to the corresponding request */
typedef   AlsaLanguageNotificationRsp AfsaLanguageNotificationRsp;

/** Requests a browser inside a browser enabled terminal to interpret the content
 * corresponding to an URL
 */
typedef   AlsaLaunchBrowserInd   AfsaLaunchBrowserInd;

/** The TERMINAL RESPONSE for the LAUNCH BROWSER command  */
typedef   AlsaLaunchBrowserRsp   AfsaLaunchBrowserRsp;

/** This signal  supplies the list of events to monitor. 
 * These are included in the SimatEventList structure. 
 */
typedef   AlsaSetUpEventListInd  AfsaSetUpEventListInd;

/** Returns the response data for the Setup Event List proactive SIM command.
 */
typedef   AlsaSetUpEventListRsp  AfsaSetUpEventListRsp;


/** update of MMI support items for the termimal profile download */
/** note These options will only be implemented following a power down and up
 * i.e. at the next profile download */
typedef struct AfsaMmiProfileUpdateReqTag
{
  /** Id of task supplying the MMI support options update */
  TaskId                         taskId;
  /** what MMI specific STK terminal profile options are supported in MMI */
  SimatMmiSupportProfile         mmiSupportProfile;
}
AfsaMmiProfileUpdateReq;

typedef struct AfsaMmiProfileUpdateCnfTag
{
  /** Id of task supplying the MMI support options update */
  TaskId                         taskId;
  /** Indicates if update was successful or not */
  Boolean                        profileUpdated;
}
AfsaMmiProfileUpdateCnf;

/** details of the  MMI support items for the termimal profile download */
typedef struct AfsaReadMmiProfileReqTag
{
  /** Id of task asking for the MMI support options */
  TaskId                         taskId;
}
AfsaReadMmiProfileReq;


typedef struct AfsaReadMmiProfileCnfTag
{
  /** Id of task requesting the MMI support options */
  TaskId                         taskId;
  Boolean                        supportProfileValid;
  /** what MMI specific STK terminal profile options are supported in MMI */
  SimatMmiSupportProfile         mmiSupportProfile;
}
AfsaReadMmiProfileCnf;

/** @} */ /* End of SigAfsa group */
/* END OF FILE */



