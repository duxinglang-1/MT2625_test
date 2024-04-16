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
 * Signals and types used for communcation from L1 to the application layer.
 **************************************************************************/

#ifndef L1AL_SIG_H
#define L1AL_SIG_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/
#include <system.h>
#include <ki_typ.h>      /* for EmptySignal */
#include <kitqid.h>      /* For TaskId type. */
#include <l1al_typ.h>
#include <dmnvp_typ.h>   /* for NvControl, NvData, RecordNumber, etc. */

#include <l1alcfg.h>
#if defined(UPGRADE_MTS)
#include <mtstyp.h>
#endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/
#define MAX_BBC_MIRROR_SIZE          64
#define MAX_BBC_HISTORY_SIZE         64

/***************************************************************************
 * Signal Definitions
 **************************************************************************/

/** \defgroup SigL1al L1AL Signaling Interface
 * \ingroup Sig
 * This interface allows applications to make low level requests that are
 * typically serviced by parts of Layer 1 that are close to the hardware.
 * @{
 */


typedef struct L1AlAuxDacSetReqTag
{
  DacReference    dac;
  Int16           value;
}
L1AlAuxDacSetReq;

typedef struct L1AlPwmStartReqTag
{
  PwmReference    pwm;
  Int8            onDuration;
  Int32           phase;
  Int32           pulseLength;
}
L1AlPwmStartReq;

typedef struct L1AlPwmStopReqTag
{
  PwmReference    pwm;
}
L1AlPwmStopReq;

typedef struct L1AlChargeAccumReqTag
{
  TaskId          taskId;
}
L1AlChargeAccumReq;

typedef struct L1AlChargeAccumIndTag
{
  Int32           txlevel;
  Int16           numTxBursts;
}
L1AlChargeAccumInd;


/********************** L1 STATUS *********************/

typedef struct L1AlStatusIndTag
{
  L1Status        l1Status;
}
L1AlStatusInd;

/********************** AUDIO **************************/

#if !defined (DISABLE_LEGACY_AUDIO_L1AL)
/** \defgroup SigL1alAudio Audio
 * Audio configuration and control.
 * @{
 */


typedef struct L1AlAudioErrorIndTag
{
    Int16           commandRef;
    L1AudioError    errorCode;    /* AUDIO_SUCCESS or an error code */
    L1AudioChannel  channel;      /* Channel ID or NO_AUDIO_CHANNEL */
}
L1AlAudioErrorInd;

typedef struct L1AlAudioChannelSetupReqTag
{
    TaskId                   taskId;
    Int16                    commandRef;
    L1AudioFormat            audioFormat;        /* Format of data on channel        */
    L1AudioDeviceList        l1AudioDeviceList;  /* Device(s) to be used for channel */
    L1AudioChannel           channel;            /* Channel ID to use or CHANNEL_REQ */
    L1AudioVolumeDescription volume;             /* 0% to 100% of the maximum volume. [0] = Mono/Left, [1] = Right. */
    L1AudioAttStereoMode     audioAttStereoMode; /* Attribute describing the encoding of the audio stream (mono/stereo)     */
    L1AudioAttRate           audioAttRate;       /* Attribute describing the rate of the audio stream (8000Hz, 44100Hz,...) */
    Boolean                  enableNrec;         /* TRUE if noise reduction and echo cancellation should enabled. */
#if defined (UPGRADE_MMAC)
    Int16                    mmacCodecId;
#endif
}
L1AlAudioChannelSetupReq;

typedef struct L1AlAudioChannelSetupCnfTag
{
    Int16                commandRef;
    L1AudioError         errorCode;    /* AUDIO_SUCCESS or an error code      */
    L1AudioChannel       channel;      /* Channel ID allocated or CHANNEL_ERR */
    L1AudioAttStereoMode audioAttStereoMode; /* Attribute describing the encoding of the audio stream (mono/stereo)     */
    L1AudioAttRate       audioAttRate;       /* Attribute describing the rate of the audio stream (8000Hz, 44100Hz,...) */
}
L1AlAudioChannelSetupCnf;

typedef struct L1AlAudioChannelDisableIndTag
{
    Int16           commandRef;
    L1AudioChannel  channel;      /* Channel ID to disable */
}
L1AlAudioChannelDisableInd;

typedef struct L1AlAudioChannelTerminateReqTag
{
    TaskId          taskId;
    Int16           commandRef;
    L1AudioChannel  channel;      /* Channel ID to terminate */
}
L1AlAudioChannelTerminateReq;

typedef struct L1AlAudioChannelTerminateCnfTag
{
    Int16           commandRef;
    L1AudioError    errorCode;    /* AUDIO_SUCCESS or an error code */
    L1AudioChannel  channel;      /* Channel ID terminated */
}
L1AlAudioChannelTerminateCnf;

typedef struct L1AlAudioVolumeReqTag
{
    TaskId                    taskId;
    Int16                     commandRef;
    L1AudioChannel            channel;   /* Channel ID to alter volume on */
    L1AudioVolumeDescription  volume;    /* New volume setting (0 - 100%) */
}
L1AlAudioVolumeReq;

typedef struct L1AlAudioVolumeCnfTag
{
    TaskId          taskId;
    Int16           commandRef;
    L1AudioChannel  channel;      /* Channel ID the volume changed. */
    L1AudioError    errorCode;    /* AUDIO_SUCCESS or an error code */
}
L1AlAudioVolumeCnf;


typedef struct L1AlAudioVolumeRampReqTag
{
    TaskId          taskId;
    Int16           commandRef;
    L1AudioChannel  channel;    /* Channel ID to ramp volume on */
    Boolean         rampOn;     /* TRUE - ramp is on, FALSE - ramp is off */
    Int8            rampStart;  /* Ramp start percentage (0 - 100% of channel volume) */
    Int8            rampEnd;    /* Ramp end   percentage (0 - 100% of channel volume) */
    Int8            rampStep;   /* Ramp step                                          */
    Int8            rampTime;   /* Ramp time between steps, in units of 20ms          */
}
L1AlAudioVolumeRampReq;

typedef struct L1AlAudioFreeBufferMemoryIndTag
{
    void           *buffer_p;
}
L1AlAudioFreeBufferMemoryInd;

typedef struct L1AlAudioDtmfReqTag
{
    TaskId          taskId;
    Int16           commandRef;
    L1AudioChannel  channel;      /* Channel ID to play tone on */
    Boolean         toneOn;       /* TRUE - Play tone number supplied, FALSE - Stops the current DTMF tone */
    L1DTMFTone      toneNumber;   /* ignored if toneOn == FALSE */
}
L1AlAudioDtmfReq;

typedef struct L1AlAudioDtmfCnfTag
{
    Int16           commandRef;
    L1AudioChannel  channel;
    L1AudioError    status;
}
L1AlAudioDtmfCnf;

typedef struct L1AlAudioCallToneReqTag
{
    TaskId          taskId;
    Int16           commandRef;
    L1AudioChannel  channel;      /* Channel ID to play tone on */
    Boolean         toneOn;        /* TRUE - Play tone number supplied, FALSE - Stops the current CALL tone */
    L1CallTone      toneNumber;    /* ignored if toneOn == FALSE */
}
L1AlAudioCallToneReq;

typedef struct L1AlAudioCallToneCnfTag
{
    Int16           commandRef;
    L1AudioChannel  channel;
    L1AudioError    status;
}
L1AlAudioCallToneCnf;


#if defined(ENABLE_LEGACY_TONE_SUPPORT)

typedef struct L1AlAudioCustomToneReqTag
{
    TaskId            taskId;
    Int16             commandRef;
    L1AudioChannel    channel;      /* Channel ID to play tone on */
    Int8              numberBlocks;
    Boolean           override;
    L1CustomToneBlock toneBlock [ MAX_NUMBER_OF_TONE_BLOCKS ];
}
L1AlAudioCustomToneReq;

typedef struct L1AlAudioCustomToneStopReqTag
{
    TaskId           taskId;
    Int16            commandRef;
    L1AudioChannel   channel;
}
L1AlAudioCustomToneStopReq;

typedef struct L1AlAudioCustomToneStopIndTag
{
    Int16            commandRef;
    L1AudioChannel   channel;
}
L1AlAudioCustomToneStopInd;

typedef struct L1AlAudioCustomToneBufferFreeIndTag
{
  L1AudioChannel   channel;
}
L1AlAudioCustomToneBufferFreeInd;

#else
typedef EmptySignal L1AlAudioCustomToneReq;
typedef EmptySignal L1AlAudioCustomToneStopReq;
typedef EmptySignal L1AlAudioCustomToneStopInd;
typedef EmptySignal L1AlAudioCustomToneBufferFreeInd;
#endif /* ENABLE_LEGACY_TONE_SUPPORT */


typedef struct L1AlAudioSpeechReqTag
{
    TaskId          taskId;
    Int16           commandRef;
    L1AudioChannel  inputChannel;
    L1AudioChannel  outputChannel;
    Boolean         speechIsOn;
}
L1AlAudioSpeechReq;

typedef struct L1AlAudioSpeechCnfTag
{
    Int16           commandRef;
    L1AudioError    errorCode;    /* AUDIO_SUCCESS or an error code */
    L1AudioChannel  channel;
    Boolean         speechIsOn;
}
L1AlAudioSpeechCnf;

typedef struct L1AlAudioMuteReqTag
{
    TaskId          taskId;
    Int16           commandRef;
    L1AudioDevice   audioDevice;  /* Device ID to mute */
    Boolean         muteOn;       /* TRUE - mutes device, FALSE - unmutes device */
}
L1AlAudioMuteReq;

typedef struct L1AlAudioMuteCnfTag
{
    TaskId          taskId;
    Int16           commandRef;
    L1AudioDevice   audioDevice;  /* Device ID to mute */
    L1AudioChannel  channel;
    Boolean         muteOn;       /* TRUE - mutes device, FALSE - unmutes device */
    L1AudioError    status;
}
L1AlAudioMuteCnf;

typedef struct L1AlAudioChannelMuteReqTag
{
    TaskId          taskId;
    Int16           commandRef;
    L1AudioChannel  channel;    /* Channel ID to mute */
    Boolean         muteOn;     /* TRUE - mutes channel, FALSE - unmutes channel */
}
L1AlAudioChannelMuteReq;

typedef struct L1AlAudioChannelMuteCnfTag
{
    Int16           commandRef;
    L1AudioError    errorCode;  /* AUDIO_SUCCESS or an error code */
    L1AudioChannel  channel;    /* Channel ID to mute */
}
L1AlAudioChannelMuteCnf;

typedef struct L1AlAudioPredefinedReqTag
{
    TaskId              taskId;
    Int16               commandRef;
    L1AudioChannel      channel;
#if !defined(UPGRADE_FSYSTEM)
/* This parameter is now only used when playing from NVRAM
 * It will be removed fron this signal completely when L1Al and L1Am are fully
 * separated */
    L1PredefinedMelody  predefinedNumber;
#else
    void*               context_p;
#endif /* UPGRADE_FSYSTEM */
}
L1AlAudioPredefinedReq;



typedef struct L1AlAudioPredefinedCnfTag
{
    Int16               commandRef;
    L1AudioChannel      channel;
    L1AudioError        status;
#if !defined(UPGRADE_FSYSTEM)
/* This parameter is now only used when playing from NVRAM
 * It will be removed fron this signal completely when L1Al and L1Am are fully
 * separated */
    Int8               *dataBuffer_p;
#endif  /* UPGRADE_FSYSTEM */
}
L1AlAudioPredefinedCnf;

typedef struct L1AlAudioPredefinedStopReqTag
{
    TaskId              taskId;
    Int16               commandRef;
    L1AudioChannel      channel;
}
L1AlAudioPredefinedStopReq;

typedef struct L1AlAudioPredefinedStopIndTag
{
    Int16               commandRef;
    L1AudioChannel      channel;
#if defined( UPGRADE_FSYSTEM )
    void*               context_p;
#endif  /* UPGRADE_FSYSTEM */
}
L1AlAudioPredefinedStopInd;

typedef struct L1AlAudioPredefinedEndOfPlaybackTag
{
    Int16               commandRef;
    L1AudioChannel      channel;
#if defined( UPGRADE_FSYSTEM )
    void*               context_p;
#endif  /* UPGRADE_FSYSTEM */
}
L1AlAudioPredefinedEndOfPlayback;

typedef struct L1AlAudioPredefinedRestartIndTag
{
   Int16               commandRef;
   L1AudioChannel      channel;
#if defined( UPGRADE_FSYSTEM )
   void*               context_p;
#endif  /* UPGRADE_FSYSTEM */
}
L1AlAudioPredefinedRestartInd;

#if defined(UPGRADE_FSYSTEM)
typedef EmptySignal L1AlAudioPredefinedStoreReq;
typedef EmptySignal L1AlAudioPredefinedStoreCnf;
#else
typedef struct L1AlAudioPredefinedStoreReqTag
{
    TaskId        taskId;
    RecordNumber  recordNumber;  /* if (recordNumber == 0xffff), store under a new record number */
    NvControl     nvControl;
    NvData        nvData;
}
L1AlAudioPredefinedStoreReq;

typedef struct L1AlAudioPredefinedStoreCnfTag
{
    L1AudioError  status;
    RecordNumber  recordNumber;  /* index of erased recording */
}
L1AlAudioPredefinedStoreCnf;
#endif /* UPGRADE_FSYSTEM */

typedef struct L1AlAudioEraseReqTag
{
    TaskId        taskId;
    RecordNumber  recordNumber;  /* index of stored recording to erase */
}
L1AlAudioEraseReq;

#if defined(UPGRADE_FSYSTEM)
typedef EmptySignal L1AlAudioPredefinedEraseCnf;
typedef EmptySignal L1AlAudioPredefinedBufferSetupReq;
#else
typedef struct L1AlAudioPredefinedEraseCnfTag
{
    L1AudioError  status;
    RecordNumber  recordNumber;  /* index of erased recording */
}
L1AlAudioPredefinedEraseCnf;

typedef struct L1AlAudioPredefinedBufferSetupReq
{
    TaskId        taskId;
    RecordNumber  maxPredefined; /* Max number of predefined tune slots available          */
    Boolean      *allocTable;    /* POINTER to a ram buffer to store the allocation table  */
}
L1AlAudioPredefinedBufferSetupReq;
#endif /* UPGRADE_FSYSTEM */

typedef struct L1AlAudioPredefinedBufferSetupCnfTag
{
    L1AudioError   status;
    RecordNumber   systemTunes;
}
L1AlAudioPredefinedBufferSetupCnf;

typedef struct L1AlAudioMelodyBufferDataRspTag
{
    L1AudioChannel          channel;
    L1GenericMelodyElement  genericToneData[L1AL_MELODY_DATA_PKT_SIZE];
}
L1AlAudioMelodyBufferDataRsp;

typedef struct L1AlAudioLoopBackReqTag
{
    TaskId          taskId;             /* Controlling task */
    Int16           commandRef;         /* CommandRef */
    Boolean         loopbackIsOn;       /* Turn loopback mode on or off */
    L1AudioChannel  inputChannel;       /* input channel (format determines loopback mode) */
    L1AudioChannel  outputChannel;      /* output channel (format determines loopback mode) */
    Int16           audioCodec;         /* AU_CODEC_xxx or NO_CODEC_OVERRIDE if default codec is to be used*/
    Int16           delayBufferLength;  /* delay in units of 20ms */
    Int16           pcmToneDuration;    /* in units of 20ms  */
}
L1AlAudioLoopBackReq;

typedef struct L1AlAudioLoopBackCnfTag
{
    Int16           commandRef;             /* CommandRef */
    L1AudioChannel  channel;                /* Always the input channel.  L1AM needs this */
    L1AudioError    inputChannelErrorCode;  /* input channel error code */
    L1AudioError    outputChannelErrorCode; /* output channel error code */
}
L1AlAudioLoopBackCnf;

typedef struct L1AlAudioLoopBackIndTag
{
    Int16           commandRef;             /* CommandRef */
    L1AudioChannel  channel;
    L1AudioError    status;
}
L1AlAudioLoopBackInd;

typedef struct L1AlAudioBufferSetupReqTag
{
  TaskId         taskId;
  Int16          commandRef;
}
L1AlAudioBufferSetupReq;

typedef struct L1AlAudioBufferSetupCnfTag
{
  Int16               commandRef;
  L1AudioSpeechStatus status;
  Int16               systemMemos; /* Number of predefined memos stored in 'ROM'  */
  DataLength          length;      /* Size of record buffer in bytes              */
  RecordNumber        maxMemos;    /* Max number memo slots available for storage */
  Boolean             allocated [ MAX_NUMBER_OF_STORED_MEMOS ];
}
L1AlAudioBufferSetupCnf;

typedef struct L1AlAudioRecordReqTag
{
  TaskId         taskId;
  Int16          commandRef;
  L1AudioChannel channel;
  L1AudioChannel channel2;
  L1AudioAttrCodecType      codecType;
}
L1AlAudioRecordReq;

typedef struct L1AlAudioRecordCnfTag
{
  Int16               commandRef;
  L1AudioSpeechStatus status;
  L1AudioChannel      channel;
  L1AudioSpeechStatus status2;
  L1AudioChannel      channel2;
}
L1AlAudioRecordCnf;

typedef struct L1AlMemoRecordStopIndTag
{
  Int8*               location_p;  /* Always a pointer to a location in Flash - so ok. */
  Int32               length;
#if defined (UPGRADE_ENHANCED_MEMO)
  L1AudioAttrCodecType  codecType;
#endif
  L1AudioChannel      channel;
}
L1AlMemoRecordStopInd;


#if defined (UPGRADE_ENHANCED_MEMO)
typedef union MemoDataArrayTag
{
  Int8           data[L1AL_MEMO_DATA_SIG_BUF_LENGTH];
  Int32          ALIGN_32;
} MemoDataArray;
#endif

typedef struct L1AlAudioPlaybackReqTag
{
  TaskId         taskId;
  Int16          commandRef;
  RecordNumber   recordNumber;  /* if ( recordNumber == 0xffff ), play back from record buffer */
  L1AudioChannel channel;
  L1AudioChannel channel2;
#if defined (UPGRADE_ENHANCED_MEMO)
  Boolean        playbackFromFile;
  MemoDataArray  memoDataArray;
  Int16          numBytes;
  Int16          numFrames;
  Boolean        endOfDataFlag;
  Boolean        newMemo;
  L1AudioFormat  playbackFormat;
  L1AudioAttrCodecType  codecType;
#endif
}
L1AlAudioPlaybackReq;

typedef struct L1AlAudioPlaybackCnfTag
{
  Int16               commandRef;
  L1AudioSpeechStatus status;
  L1AudioChannel      channel;
  L1AudioSpeechStatus status2;
  L1AudioChannel      channel2;
  Boolean             newMemo;
}
L1AlAudioPlaybackCnf;

typedef struct L1AlAudioStopReqTag
{
  TaskId         taskId;
  Int16          commandRef;
  L1AudioChannel channel;
  L1AudioChannel channel2;
#if defined( UPGRADE_ENHANCED_MEMO )
#if defined( UPGRADE_FSYSTEM )
  L1AlMemoMode   memoMode;
#endif
#endif
}
L1AlAudioStopReq;

typedef struct L1AlAudioStopCnfTag
{
  Int16               commandRef;
  L1AudioChannel      channel;
  L1AudioSpeechStatus status;
  Int16               time;
}
L1AlAudioStopCnf;

typedef struct L1AlAudioStopIndTag
{
  L1AudioChannel      channel;
  L1AudioSpeechStatus status;
}
L1AlAudioStopInd;

typedef struct L1AlAudioEraseCnfTag
{
    L1AudioSpeechStatus status;
    RecordNumber        recordNumber;  /* index of erased recording */
}
L1AlAudioEraseCnf;

typedef struct L1AlAudioStoreReqTag
{
    TaskId        taskId;
    RecordNumber  recordNumber;  /* if (recordNumber == 0xffff), store under a new record number */
}
L1AlAudioStoreReq;

typedef struct L1AlAudioStoreCnfTag
{
    L1AudioSpeechStatus status;
    RecordNumber        recordNumber;  /* number recording stored under, 0xffff if no space available */
}
L1AlAudioStoreCnf;

typedef EmptySignal L1AlMemoPlaybackBufferFreeReq;  /* Used to free the memo playback buffer when playback is complete. */

typedef struct L1AlAudioDecodeBufferFillIndTag
{
  L1AudioChannel   channel;
  L1AudioFormat    format;
} L1AlAudioDecodeBufferFillInd;

typedef struct L1AlAudioDecodeCompleteIndTag
{
  L1AudioChannel            channel;
  L1AudioFormat             format;
  L1AudioError              status;
} L1AlAudioDecodeCompleteInd;

typedef struct L1AlAutoVerificationDataIndTag
{
  L1AudioChannel            channel;
  L1AudioFormat             format;
  L1AlAutoVerificationData  verificationData;
} L1AlAutoVerificationDataInd;

/*
 * Basic channel enable request signal
 */
typedef struct L1AlAudioChannelEnableReqTag
{
  TaskId          taskId;
  Int16           commandRef;
  L1AudioChannel  channel;
}
L1AlAudioChannelEnableReq;

/*
 * Basic channel enable confirm signal
 */
typedef struct L1AlAudioChannelEnableCnfTag
{
  Int16           commandRef;
  L1AudioChannel  channel;
  Boolean         enabled;
  L1AudioError    l1AudioError;
}
L1AlAudioChannelEnableCnf;

/*
 * Basic channel disable request signal
 */
typedef struct L1AlAudioChannelDisableReqTag
{
  TaskId          taskId;
  Int16           commandRef;
  L1AudioChannel  channel;
}
L1AlAudioChannelDisableReq;

/*
 * Basic channel disable confirm signal
 */
typedef struct L1AlAudioChannelDisableCnfTag
{
  Int16           commandRef;
  L1AudioChannel  channel;
  Boolean         disabled;
}
L1AlAudioChannelDisableCnf;

#if defined (UPGRADE_MMAC)
typedef struct L1AlMmacChannelStartReqTag
{
  TaskId             taskId;
  Int16              commandRef;
  L1AudioChannel     channel;
  L1AudioFormat      audioFormat;
} L1AlMmacChannelStartReq;

typedef struct L1AlMmacChannelStartCnfTag
{
  TaskId             taskId;
  Int16              commandRef;
  L1AudioChannel     channel;
  L1AudioError       status;
} L1AlMmacChannelStartCnf;

typedef struct L1AlMmacChannelStopReqTag
{
  TaskId             taskId;
  Int16              commandRef;
  L1AudioChannel     channel;
  L1AudioFormat      audioFormat;
} L1AlMmacChannelStopReq;

typedef struct L1AlMmacChannelStopIndTag
{
  TaskId             taskId;
  Int16              commandRef;
  L1AudioChannel     channel;
  L1AudioError       status;
} L1AlMmacChannelStopInd;
#else
typedef EmptySignal  L1AlMmacChannelStartReq;
typedef EmptySignal  L1AlMmacChannelStartCnf;
typedef EmptySignal  L1AlMmacChannelStopReq;
typedef EmptySignal  L1AlMmacChannelStopInd;
#endif

typedef struct L1AlAudioBufferDelayIndTag
{
  L1AudioChannel  l1AudioChannel;
  SignedInt32     mediaOffset_ms;
} L1AlAudioBufferDelayInd;

typedef EmptySignal  L1AlPCMStreamingInputReq;
typedef EmptySignal  L1AlPCMStreamingOutputReq;
typedef EmptySignal  L1AlPCMStreamingStopReq;
typedef EmptySignal  L1AlPCMStreamingInputCnf;
typedef EmptySignal  L1AlPCMStreamingOutputCnf;
typedef EmptySignal  L1AlPCMStreamingStopCnf;

typedef struct L1AlAudioDirectControlReqTag
{
  TaskId                   taskId;
  Int16                    commandRef;
  L1AlDirectControlRdRdallWrite         readReadAllWrite;
  DirectControlWriteEntity directControlEntityArray[MAX_DIRECT_CONTROL_WRITE_ENTITIES];
} L1AlAudioDirectControlReq;

typedef struct L1AlAudioDirectControlCnfTag
{
  Int16                    commandRef;
  L1AudioError             status;
} L1AlAudioDirectControlCnf;

typedef struct L1AlAudioDirectControlIndTag
{
  Int16                    commandRef;
  Int16                    numberOfEntriesInArray;
  DirectControlReadEntity  directControlEntityArray[MAX_DIRECT_CONTROL_READ_ENTITIES];
} L1AlAudioDirectControlInd;

typedef struct L1AlAudioDirectControlCompleteIndTag
{
  Int16               commandRef;
  DlAudioDriverId     driverId;
} L1AlAudioDirectControlCompleteInd;

typedef struct L1AlAudioIoIndTag
{
  Int16               commandRef;
  DlAudioIO           dlAudioIo;
} L1AlAudioIoInd;

typedef struct L1AlAudioStatusReqTag
{
  TaskId              taskId;
  Int16               commandRef;
} L1AlAudioStatusReq;

typedef struct L1AlAudioStatusCnfTag
{
  Int16                     commandRef;
  DlAudioSlowClockVariables dlAudioSlowClockVariables;
  DlDspAudioVariables       dlDspAudioVariables;
} L1AlAudioStatusCnf;

/** @} */ /* End of SigL1alAudio group */

#endif /* !DISABLE_LEGACY_AUDIO_L1AL */

#if defined(UPGRADE_MTS)
/***************************************************************************
 * Modem Timer Service Signals
 ***************************************************************************/

/* L1AlMtsTimerStartReq
 *  Request for a modem timer to be started.
 */
typedef struct L1AlMtsTimerStartReqTag
{
    TaskId      taskId;      /* TaskId for the destination of the confirm */
    MTSTrigger  trigger;    /* The trigger point for the timer */
    SignedInt16 microSecondOffset; /* Offset from trigger to action */
    MTSAction   action;     /* The action to happen at trigger + offset */
    Boolean     isPeriodic; /* Indicates if the trigger should be periodic */
}
L1AlMtsTimerStartReq;


/* L1AlMtsTimerStartCnf
 *  The confirmation signal that a timer has been started, or that the
 *  start request failed for some reason.
 */
typedef struct L1AlMtsTimerStartCnfTag
{
    MTSErr     result;  /* MTS result code such as MTS_EOK */
    MTSEntryId ident;   /* Identity of the started timer */
}
L1AlMtsTimerStartCnf;


/* L1AlMtsTimerStopReq
 *  Request to stop a previously started modem timer.  This uses the
 *  identity previously returned in a StartTimerCnf to identify the
 *  timer that is to be stopped.
 */
typedef struct L1AlMtsTimerStopReqTag
{
    TaskId     taskId;  /* TaskId for the destination of the confirm */
    MTSEntryId ident;   /* Identity of the timer to stop */
}
L1AlMtsTimerStopReq;


/* L1AlMtsTimerStopCnf
 *  Result of a MtsTimerStopReq.  This may indicate that the timer has
 *  been stopped sucessfully, or that the stop request failed, perhaps
 *  since the timer has already expired.
 */
typedef struct L1AlMtsTimerStopCnfTag
{
    MTSErr     result;  /* MTS result code such as MTS_EOK */
    MTSEntryId ident;   /* Identity of the timer to stop */
}
L1AlMtsTimerStopCnf;

#else
typedef EmptySignal L1AlMtsTimerStartReq;
typedef EmptySignal L1AlMtsTimerStartCnf;
typedef EmptySignal L1AlMtsTimerStopReq;
typedef EmptySignal L1AlMtsTimerStopCnf;
#endif

/********************** DEVELOPMENT TOOLS ***************/

typedef EmptySignal L1AlBbcDebugReq;

typedef struct L1AlBbcDebugCnfTag
{
    Int16          mirrorLen;
    Int16          mirror[ MAX_BBC_MIRROR_SIZE ];
    Int16          historyLen;
    Int16          history[ MAX_BBC_HISTORY_SIZE ];
}
L1AlBbcDebugCnf;

/* @} */ /* End of SigL1al group */

#endif /* L1AL_SIG_H */
/* END OF FILE */
