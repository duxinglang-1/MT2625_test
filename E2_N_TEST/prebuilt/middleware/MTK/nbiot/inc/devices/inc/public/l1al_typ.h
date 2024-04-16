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
 *   Type definitions for signals used for communcation from L1 to the
 *   application layer.
 **************************************************************************/
#if !defined (L1AL_TYP_H)
# define      L1AL_TYP_H

# if defined( UPGRADE_AMR_MEMO )
#  if !defined( UPGRADE_ENHANCED_MEMO )
#   error "UPGRADE_AMR_MEMO cannot be compiled without UPGRADE_ENHANCED_MEMO"
#  endif
# endif

/***************************************************************************
 * Nested Include Files
 **************************************************************************/
#include <system.h>
#include <kitqid.h>      /* For TaskId type. */

#include <uttime.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/
# define MAX_NUMBER_TONE_COMPONENTS          4

# define MAX_NUMBER_OF_STORED_MEMOS          (10 )
# define MEMO_BUFFER_RECORD_NUMBER           (0xffff)
# if defined(UPGRADE_MUSIC)
#  define MUSIC_BUFFER_RECORD_NUMBER         (0xfffe)
# endif

/* Frame Type enumerations used by RTP Payload Format ToC. */
# define VRE_AMR_MR_475                          0  /* AMR 475 kbit/s           */
# define VRE_AMR_MR_515                          1  /* AMR 515 kbit/s           */
# define VRE_AMR_MR_59                           2  /* AMR 590 kbit/s           */
# define VRE_AMR_MR_67                           3  /* AMR 670 kbit/s (PDC-EFR) */
# define VRE_AMR_MR_74                           4  /* AMR 740 kbit/s (TDMA-EFR)*/
# define VRE_AMR_MR_795                          5  /* AMR 795 kbit/s           */
# define VRE_AMR_MR_102                          6  /* AMR 102 kbit/s           */
# define VRE_AMR_MR_122                          7  /* AMR 122 kbit/s (GSM-EFR) */
# define VRE_AMR_MR_DTX                          8  /* AMR SID                  */
# define VRE_AMR_MR_GSM_EFR_SID                  9  /* GSM-EFR SID              */
# define VRE_AMR_MR_TDMA_EFR_SID                10  /* TDMA-EFR SID             */
# define VRE_AMR_MR_PDC_EFR_SID                 11  /* PDC-EFR SID              */
# define VRE_AMR_MR_FOR_FUTURE_USE_1            12  /* For future use           */
# define VRE_AMR_MR_FOR_FUTURE_USE_2            13  /* For future use           */
# define VRE_AMR_MR_FOR_FUTURE_USE_3            14  /* For future use           */
# define VRE_AMR_MR_NO_DATA                     15  /* No Data                  */

# if defined( UPGRADE_ENHANCED_MEMO )
   /* This defined the size of the signal buffer for memo playback from file. */
#  define  L1AL_MEMO_DATA_SIG_BUF_LENGTH    1280
   /* Defines the number of bytes that are transferred in the memo playback signal
    * depending on the format of the playback. */
#  define  L1AM_PCM_MEMO_BUF_LENGTH         1280
#  define  L1AM_FR_MEMO_BUF_LENGTH          990
#  define  L1AL_SM_SPEECH_BUFFER_SIZE_BYTES (sizeof(EncSpeechBuffer))

#  if defined( UPGRADE_AMR_MEMO )
#   define  MEMO_AMR_MAX_FRAME_SIZE_BYTES      31
#   define  MEMO_AMR_FRAME_HEADER_SIZE_BYTES   1
#  endif
# endif

/** Parameters for L1Al's local melody buffer **/
/* How many melodyElements in the melody data FIFO */
# define L1AL_MELODY_BUFFER_SIZE            21
/* How many elements left in the FIFO when a req for more data is sent */
# define L1AL_MELODY_BUFFER_LOW_WATER_MARK  10
/* The number of melodyElements sent in the L1AlAudioMelodyBufferDataRsp
 * signal from the audio manager */
# define L1AL_MELODY_DATA_PKT_SIZE          10
/* The number of melody data pkts that L1Am will initially send to L1Al, upon
 * starting a new melody, to prime L1al's melody data FIFO */
# define L1AL_MELODY_BUFFER_PRIME_PKT_COUNT 2


# if defined(ENA_L1AL_REGRESSION_TEST_HARNESS)
/* Control word fields for l1AlAutoVerificationCtrlWord. */
#  define  ENA_L1AL_TEST_COMPUTE_CHKSUM      0x0001
# endif

#define MAX_DIRECT_CONTROL_WRITE_ENTITIES 32
#define MAX_DIRECT_CONTROL_READ_ENTITIES  16

#define NUM_L1AM_DEVICE_PRIORITIES         3  /* Max number of l1al audio devices in the priority list. */

#define MAX_NUMBER_OF_TONE_BLOCKS          5

/***************************************************************************
 * Type Definitions
 **************************************************************************/


/********************** L1 STATUS *********************/
typedef enum L1StatusTag
{
  L1_NUM_STATUS
} L1Status;

/********************** AUDIO **************************/

#if !defined (DISABLE_LEGACY_AUDIO_L1AL)

typedef enum L1AudioChannelStateTag
{
  L1_AUD_CHAN_STATE_NOT_DEFINED
, L1_AUD_CHAN_STATE_SETUP
, L1_AUD_CHAN_STATE_ENABLE_IN_PROGRESS
, L1_AUD_CHAN_STATE_ENABLED
, L1_AUD_CHAN_STATE_DISABLE_IN_PROGRESS
, L1_AUD_CHAN_STATE_DISABLED
, L1_AUD_CHAN_STATE_DEVICE_CHANGE_IN_PROGRESS
} L1AudioChannelState;


typedef Int8  L1AudioVolume;

typedef struct L1AudioVolumeDescriptionTag
{
  L1AudioVolume     MonoOrLeft;
  L1AudioVolume     Right;
} L1AudioVolumeDescription;

typedef Int8  L1AudioChannel;
typedef Int16 L1AudioFrequency;

/*
 * Ordering of enumerated type L1DTMFTone MUST match
 * the values in l1AlDTMFToneParameters[] within l1aldtmf.c
 */
typedef enum L1DTMFToneTag
{
  TONE_DTMF_1,
  TONE_DTMF_2,
  TONE_DTMF_3,
  TONE_DTMF_A,
  TONE_DTMF_4,
  TONE_DTMF_5,
  TONE_DTMF_6,
  TONE_DTMF_B,
  TONE_DTMF_7,
  TONE_DTMF_8,
  TONE_DTMF_9,
  TONE_DTMF_C,
  TONE_DTMF_STAR,
  TONE_DTMF_0,
  TONE_DTMF_HASH,
  TONE_DTMF_D,
  TOTAL_NUMBER_DTMF_TONES
} L1DTMFTone;

/*
 * Ordering of enumerated type L1CallTone MUST match
 * the values in l1AlCallToneParameters[] within l1aldtmf.c
 */
typedef enum L1CallToneTag
{
  TONE_UK_RING_TONE
, TONE_AUTHENTICATION_TONE
, TONE_GSM_DCS_RINGING_TONE
, TONE_PCS_RINGING_TONE
, TONE_GSM_DCS_SUB_BUSY_TONE
, TONE_PCS_SUB_BUSY_TONE
, TONE_GSM_DCS_CONGESTION_TONE
, TONE_PCS_CONGESTION_TONE
, TONE_CALL_DROP_TONE
, TONE_GSM_DCS_DIAL_TONE
, TONE_PCS_DIAL_TONE
, TONE_RADIO_ACK_TONE
, TONE_GSM_DCS_CALL_WAITING_TONE
, TONE_PCS_CALL_WAITING_TONE
, TONE_ATTENTION_TONE
/*
 * Define here any new Call Tones. Ensure that a
 * new definition appears in l1AlCallToneParameters[]
 */
, TOTAL_NUMBER_CALL_TONES
/*
 * Define here any Call Tones which are identical
 * to any of the Call Tones defined above
 */
, TONE_UNOBTAINABLE_TONE = TONE_AUTHENTICATION_TONE
} L1CallTone;

typedef enum L1AudioDeviceTag
{/* Some of the following audio devices will not be supported without
  * appropriate hardware. N.B. Output devices and input devices are each
  * grouped together in the definition of this enum, which allows various
  * constants to be defined and applied in the rest of the code. */

/* The device list in this table must be kept the same as the device list
 *      in dlaudio.c: dlAudioManagerTable_p [ NUMBER_AUDIO_DEVICES ] = {...};
 * Also the volumeTable array in L1AuCal.c is referenced via this list. */

# define GET_AUDIO_DEVICE_FROM_AUDIO_DEF
# include <l1al_audio_devices.h>
# undef  GET_AUDIO_DEVICE_FROM_AUDIO_DEF
} L1AudioDevice;

/* The preference list works as follows: Device[0] is preferred over Device[1], over Device[2]. So
 * device [0] must play the audio, but if possible Device[1] and maybe Device[2] will also play. This
 * depends on the capabilities of the low-level audio system and the hardware capability. */
typedef L1AudioDevice L1AudioDeviceList[NUM_L1AM_DEVICE_PRIORITIES];

/* In priority, highest first */
typedef enum L1AudioFormatTag
{/*Some of the following audio formats may not be
  * supported without appropriate upgrades */
  CALL_TONE = 0
, PRE_DEFINED
, CUSTOM_TONE
, DTMF
, AUDIO_DATA
, PCM_DATA_PLAYBACK
, PCM_DATA_RECORD
, SPEECH_OUT
, SPEECH_IN
, ENCODED_DATA_PLAYBACK
, ENCODED_DATA_RECORD
, VOICE_RECOG_ENCODED_PLAYBACK
, VOICE_RECOG_ENCODED_RECORD
, I2S_8KHZ
, I2S_11_025KHZ
, I2S_12KHZ
, I2S_16KHZ
, I2S_22_05KHZ
, I2S_24KHZ
, I2S_32KHZ
, I2S_44_1KHZ
, I2S_48KHZ
, MIDI                      /* Used for hardware midi support via external chipset. */
, MMAC                      /* Generic multimedia audio codec engine.               */
, EXTERNAL_AUDIO_SOURCE     /* Allows mixing of an external audio source.           */
, AMV2_AUDIO
, AUDIO_LOOPBACK

, NUMBER_AUDIO_FORMATS
, NOT_IN_USE
, AUDIO_FIRST_FORMAT = 0    /* Used as a limit within 'for' loops.                  */
} L1AudioFormat;

/* L1AudioAttrCodecType: Used by the legacy voice memo (VRE).
 */
typedef enum L1AudioAttrCodecTypeTag
{
  L1_AUDIO_CODEC_FR
, L1_AUDIO_CODEC_AMR
, L1_AUDIO_CODEC_INVALID
} L1AudioAttrCodecType;

typedef enum L1AudioAttStereoModeTag
{
    L1_AUDIO_ATT_MODE_MONO
,   L1_AUDIO_ATT_MODE_STEREO
,   L1_AUDIO_ATT_MODE_AUTO_SELECT
,   L1_AUDIO_ATT_STEREO_MODE_NUMBER_OF_MODES
,   L1_AUDIO_ATT_MODE_UNDEFINED
} L1AudioAttStereoMode;

typedef enum L1AudioAttRateTag
{
    L1_AUDIO_ATT_RATE_8000Hz
,   L1_AUDIO_ATT_RATE_11025Hz
,   L1_AUDIO_ATT_RATE_12000Hz
,   L1_AUDIO_ATT_RATE_16000Hz
,   L1_AUDIO_ATT_RATE_22050Hz
,   L1_AUDIO_ATT_RATE_24000Hz
,   L1_AUDIO_ATT_RATE_32000Hz
,   L1_AUDIO_ATT_RATE_44100Hz
,   L1_AUDIO_ATT_RATE_48000Hz
,   L1_AUDIO_ATT_RATE_96000Hz
,   L1_AUDIO_ATT_RATE_AUTO_SELECT
,   L1_AUDIO_ATT_RATE_NUMBER_OF_RATES
,   L1_AUDIO_ATT_RATE_UNDEFINED
} L1AudioAttRate;

typedef enum L1AudioErrorTag
{
  AUDIO_SUCCESS,
  AUDIO_FAILED,
  AUDIO_UNSUPPORTED_FORMAT,
  AUDIO_CHANNEL_DOESNT_EXIST,
  AUDIO_ILLEGAL_REDEF,
  AUDIO_NO_FREE_CHANNEL,
  AUDIO_INV_SPEECH_STATE,
  AUDIO_INV_AUDIO_DATA,
  AUDIO_UNKNOWN_FORMAT,
  AUDIO_UNKNOWN_DEVICE,
  AUDIO_NO_FREE_MEMORY,
  AUDIO_TIMEOUT,
  AUDIO_BUFFER_UNDERRUN
} L1AudioError;

typedef struct L1ToneComponentTag
{
  L1AudioFrequency   freq;
  Int16              delay;
  Int16              periodOn;
  Int16              periodOff;
  L1AudioVolume      amplitude;
} L1ToneComponent;

typedef struct L1CustomToneBlockTag
{
  Int8             numComponents;
  Int8             repeat;        /* 0x00 = repeat until stopped */
  L1ToneComponent  component [ MAX_NUMBER_TONE_COMPONENTS ];
} L1CustomToneBlock;

typedef enum L1AudioSpeechStatusTag
{
  SPEECH_UNINITIALISED,
  SPEECH_IDLE,
  SPEECH_LOOPING,
  SPEECH_PLAYING,
  SPEECH_STOPPED,
  SPEECH_RECORDING,
  SPEECH_ERASING,
  SPEECH_STORED,
  SPEECH_FAILED,
  SPEECH_INPUT_CHANNEL_FAIL,
  SPEECH_OUTPUT_CHANNEL_FAIL,
  SPEECH_NVRAM_FULL,
  SPEECH_NVRAM_FAIL,
  SPEECH_OK,
  SPEECH_BUSY
} L1AudioSpeechStatus;

# if defined(UPGRADE_ENHANCED_MEMO) && defined (UPGRADE_FSYSTEM)
/* L1AlMemoMode: Used by the legacy voice memo (VRE). This is used by the
 * audio manager to indicate what mode the memo system is in when it sends a
 * memo stop request. */
typedef enum L1AlMemoModeTag
{
  L1AL_MEMO_PLAYBACK,
  L1AL_MEMO_RECORDING,
  L1AL_MEMO_INVALID_MODE
} L1AlMemoMode;
# endif

typedef struct
{
  Int16 frequency;
  Int16 amplitude;
} GenericToneComponent;

typedef struct L1GenericMelodyElementTag
{
  GenericToneComponent c[MAX_NUMBER_TONE_COMPONENTS];
  Int16                flags;    /* on/offs for each tone component */
  Int8                 repeat;   /* number of repeats of this element */
  Int8                 control;  /* control codes */
} L1GenericMelodyElement;

/* Values for L1GenericMelodyElement.control control codes */
# define GENRIC_MELODY_CONTROL_NO_ACTION     0x00
# define GENRIC_MELODY_CONTROL_LAST_ELEMENT  0x01

#endif /* !DISABLE_LEGACY_AUDIO_L1AL */

typedef struct RtpPlTocTag
{
  Int16  FT;         /* (4 bits): Frame type index, indicating either the AMR or
                      * AMR-WB speech coding mode or comfort noise (SID) mode of the
                      * corresponding frame carried in this payload.
                      */

  Int16  F;          /* (1 bit): If set to 1, indicates that this frame is followed by
                      * another speech frame in this payload; if set to 0, indicates that
                      * this frame is the last frame in this payload.
                      */

  Int16  Q;          /* (1 bit): Frame quality indicator. If set to 0, indicates the
                      * corresponding frame is a bad frame.
                      */
} RtpPlToc;          /* RTP Payload Table of Contents. */

typedef struct
{
  Int32  stage1Data;
  Int32  stage2Data;
} L1AlAutoVerificationData;

/* Used for direct control of driver/hardware */
typedef enum DlAudioEntityIdTag
{
  DLAU_NULL = 0
, DLAU_DRIVER_ID

  /* Typhoon registers and functions */
, DLAU_BBC_65XX_AUD_MODE_FIELD
, DLAU_BBC_65XX_AUD_CNTRL_1_REG
, DLAU_BBC_65XX_AUD_DAC_OS_FIELD
, DLAU_BBC_65XX_AUD_DAC_IS_FIELD
, DLAU_BBC_65XX_AUD_CNTRL_2_REG
, DLAU_BBC_65XX_AUD_OG1_FIELD
, DLAU_BBC_65XX_AUD_OG2_FIELD
, DLAU_BBC_65XX_AUDOUT_12_CNTRL_REG
, DLAU_BBC_65XX_AUD_OG_LEFT_FIELD
, DLAU_BBC_65XX_AUD_OG_RIGHT_FIELD
, DLAU_BBC_65XX_AUDOUT_LR_CNTRL_REG
, DLAU_BBC_65XX_AUD_OG_3_FIELD
, DLAU_BBC_65XX_AUDOUT_3_CNTRL_REG
, DLAU_BBC_65XX_AUD_MIC1_BIAS_FIELD
, DLAU_BBC_65XX_AUD_MIC1_GAIN_FIELD
, DLAU_BBC_65XX_AUD_MIC1_CFG_FIELD
, DLAU_BBC_65XX_AUD_MIC_1_CNTRL_REG
, DLAU_BBC_65XX_AUD_MIC2_BIAS_FIELD
, DLAU_BBC_65XX_AUD_MIC2_GAIN_FIELD
, DLAU_BBC_65XX_AUD_MIC2_CFG_FIELD
, DLAU_BBC_65XX_AUD_MIC_2_CNTRL_REG
, DLAU_BBC_65XX_AUD_IN_GN_1_FIELD
, DLAU_BBC_65XX_AUD_IN_GN_2_FIELD
, DLAU_BBC_65XX_AUD_IN_CNTRL_REG
, DLAU_BBC_65XX_AUD_IN_1_MUTE_CTRL_FIELD
, DLAU_BBC_65XX_AUD_IN_2_MUTE_CTRL_FIELD
, DLAU_BBC_65XX_AUD_IN_3L_MUTE_CTRL_FIELD
, DLAU_BBC_65XX_AUD_IN_3R_MUTE_CTRL_FIELD
, DLAU_BBC_65XX_AUD_MUTE_CNTRL_REG
, DLAU_BBC_65XX_PWM_DIV8_DIV64_FIELD
, DLAU_BBC_65XX_TONE1_ENA_DIS_FIELD
, DLAU_BBC_65XX_TONE1_VOL_FIELD
, DLAU_BBC_65XX_TONE_CNTRL_REG
, DLAU_BBC_65XX_TONE2_ENA_DIS_FIELD
, DLAU_BBC_65XX_TONE2_VOL_FIELD
, DLAU_BBC_65XX_TONE_CNTRL_2_REG
, DLAU_BBC_65XX_AUD3_ASPORT_FIELD
, DLAU_BBC_65XX_AUD_IN3_FIELD
, DLAU_BBC_65XX_AUD3_OUTPUT_AUTOCAL_FIELD
, DLAU_BBC_65XX_AUD3_GAIN_CH_FIELD
, DLAU_BBC_65XX_AUD3_PREAMP_GAIN_FIELD
, DLAU_BBC_65XX_AUD_CNTRL_3_REG
, DLAU_BBC_65XX_LDO_VABB_FIELD
, DLAU_BBC_65XX_LDO_VMIC_FIELD
, DLAU_BBC_65XX_LDO_CNTRL_REG

  /* DSP register identities */
, DLAU_SHM_UL_UL_GAIN
, DLAU_SHM_UL_VOX_GAIN
, DLAU_SHM_UL_MIN_MIC_ENERGY
, DLAU_SHM_UL_SAMP_SLNCE_PRD
, DLAU_SHM_UL_FILTER_00
, DLAU_SHM_UL_FILTER_01
, DLAU_SHM_UL_FILTER_02
, DLAU_SHM_UL_FILTER_03
, DLAU_SHM_UL_FILTER_04
, DLAU_SHM_UL_FILTER_05
, DLAU_SHM_UL_FILTER_06
, DLAU_SHM_UL_FILTER_07
, DLAU_SHM_UL_FILTER_08
, DLAU_SHM_UL_FILTER_09
, DLAU_SHM_UL_FILTER_10
, DLAU_SHM_UL_FILTER_11
, DLAU_SHM_UL_FILTER_12
, DLAU_SHM_UL_FILTER_13
, DLAU_SHM_UL_FILTER_14
, DLAU_SHM_UL_FILTER_15
, DLAU_SHM_UL_FILTER_16
, DLAU_SHM_UL_FILTER_17
, DLAU_SHM_UL_FILTER_18
, DLAU_SHM_UL_FILTER_19
, DLAU_SHM_UL_FILTER_20
, DLAU_SHM_UL_FILTER_21
, DLAU_SHM_UL_FILTER_22
, DLAU_SHM_UL_FILTER_23
, DLAU_SHM_UL_FILTER_24
, DLAU_SHM_UL_FILTER_25
, DLAU_SHM_UL_FILTER_26
, DLAU_SHM_UL_FILTER_27
, DLAU_SHM_UL_FILTER_28
, DLAU_SHM_UL_FILTER_29
, DLAU_SHM_UL_FILTER_30
, DLAU_SHM_UL_FILTER_31
, DLAU_SHM_UL_AGC_GAIN
, DLAU_SHM_UL_ECHO_MIC_THRESHOLD
, DLAU_SHM_UL_ECHO_RESIDUAL_THRESHOLD
, DLAU_SHM_UL_ECHO_MAX_STEP_SIZE
, DLAU_SHM_UL_ECHO_ENABLE_COEF_UPDATE
, DLAU_SHM_UL_ECHO_ENABLE_NLP
, DLAU_SHM_UL_ECHO_NLP_ATTENUATION
, DLAU_SHM_UL_ECHO_TAIL_00
, DLAU_SHM_UL_ECHO_TAIL_01
, DLAU_SHM_UL_ECHO_TAIL_02
, DLAU_SHM_UL_ECHO_TAIL_03
, DLAU_SHM_UL_ECHO_TAIL_04
, DLAU_SHM_UL_ECHO_TAIL_05
, DLAU_SHM_UL_ECHO_TAIL_06
, DLAU_SHM_UL_ECHO_TAIL_07
, DLAU_SHM_UL_ECHO_TAIL_08
, DLAU_SHM_UL_ECHO_TAIL_09
, DLAU_SHM_UL_ECHO_TAIL_10
, DLAU_SHM_UL_ECHO_TAIL_11
, DLAU_SHM_UL_ECHO_TAIL_12
, DLAU_SHM_UL_ECHO_TAIL_13
, DLAU_SHM_UL_ECHO_TAIL_14
, DLAU_SHM_UL_ECHO_TAIL_15
, DLAU_SHM_UL_ECHO_TAIL_16
, DLAU_SHM_UL_ECHO_TAIL_17
, DLAU_SHM_UL_ECHO_TAIL_18
, DLAU_SHM_UL_ECHO_TAIL_19
, DLAU_SHM_UL_ECHO_TAIL_20
, DLAU_SHM_UL_ECHO_TAIL_21
, DLAU_SHM_UL_ECHO_TAIL_22
, DLAU_SHM_UL_ECHO_TAIL_23
, DLAU_SHM_UL_ECHO_TAIL_24
, DLAU_SHM_UL_ECHO_TAIL_25
, DLAU_SHM_UL_ECHO_TAIL_26
, DLAU_SHM_UL_ECHO_TAIL_27
, DLAU_SHM_UL_ECHO_TAIL_28
, DLAU_SHM_UL_ECHO_TAIL_29
, DLAU_SHM_UL_ECHO_TAIL_30
, DLAU_SHM_UL_ECHO_TAIL_31
, DLAU_SHM_UL_ECHO_TAIL_32
, DLAU_SHM_UL_ECHO_TAIL_33
, DLAU_SHM_UL_ECHO_TAIL_34
, DLAU_SHM_UL_ECHO_TAIL_35
, DLAU_SHM_UL_ECHO_ENABLE_COMFORT_NOISE
, DLAU_SHM_UL_ECHO_DL_UL_DELAY
, DLAU_SHM_UL_ECHO_FAR_END_SPEECH
, DLAU_SHM_UL_ECHO_NEAR_END_SPEECH
, DLAU_SHM_UL_ECHO_DOUBLETALK_DETECTED
, DLAU_SHM_UL_ECHO_RETURN_LOSS
, DLAU_SHM_UL_ECHO_UL_TAIL_GAIN
, DLAU_SHM_UL_ECHO_DL_TAIL_GAIN

, DLAU_SHM_FAR_END_SIGNAL
, DLAU_SHM_FAR_END_PAUSE
, DLAU_SHM_NEAR_END_SIGNAL
, DLAU_SHM_NEAR_END_PAUSE
, DLAU_SHM_COMFORT_NOISE_ACTIVE
, DLAU_SHM_FAR_END_LEVEL
, DLAU_SHM_FAR_END_NOISE_FLOOR
, DLAU_SHM_NEAR_END_LEVEL
, DLAU_SHM_NEAR_END_NOISE_FLOOR
, DLAU_SHM_NOISE_REDUCTION_OUTPUT_LEVEL

, DLAU_SHM_DL_DL_GAIN
, DLAU_SHM_DL_DL_VOLUME
, DLAU_SHM_DL_SIDE_TONE_GAIN_00
, DLAU_SHM_DL_SIDE_TONE_GAIN_01
, DLAU_SHM_DL_SIDE_TONE_GAIN_02
, DLAU_SHM_DL_SIDE_TONE_GAIN_03
, DLAU_SHM_DL_SIDE_TONE_GAIN_04
, DLAU_SHM_DL_SIDE_TONE_GAIN_05
, DLAU_SHM_DL_SIDE_TONE_GAIN_06
, DLAU_SHM_DL_SIDE_TONE_GAIN_07
, DLAU_SHM_DL_SIDE_TONE_GAIN_08
, DLAU_SHM_DL_SIDE_TONE_GAIN_09
, DLAU_SHM_DL_SIDE_TONE_GAIN_10
, DLAU_SHM_DL_SIDE_TONE_GAIN_11
, DLAU_SHM_DL_SIDE_TONE_GAIN_12
, DLAU_SHM_DL_SIDE_TONE_GAIN_13
, DLAU_SHM_DL_SIDE_TONE_GAIN_14
, DLAU_SHM_DL_SIDE_TONE_GAIN_15
, DLAU_SHM_DL_SIDE_TONE_GAIN_16
, DLAU_SHM_DL_SIDE_TONE_GAIN_17
, DLAU_SHM_DL_SIDE_TONE_GAIN_18
, DLAU_SHM_DL_SIDE_TONE_GAIN_19
, DLAU_SHM_DL_SIDE_TONE_GAIN_20
, DLAU_SHM_DL_SIDE_TONE_GAIN_21
, DLAU_SHM_DL_FILTER_00
, DLAU_SHM_DL_FILTER_01
, DLAU_SHM_DL_FILTER_02
, DLAU_SHM_DL_FILTER_03
, DLAU_SHM_DL_FILTER_04
, DLAU_SHM_DL_FILTER_05
, DLAU_SHM_DL_FILTER_06
, DLAU_SHM_DL_FILTER_07
, DLAU_SHM_DL_FILTER_08
, DLAU_SHM_DL_FILTER_09
, DLAU_SHM_DL_FILTER_10
, DLAU_SHM_DL_FILTER_11
, DLAU_SHM_DL_FILTER_12
, DLAU_SHM_DL_FILTER_13
, DLAU_SHM_DL_FILTER_14
, DLAU_SHM_DL_FILTER_15
, DLAU_SHM_DL_FILTER_16
, DLAU_SHM_DL_FILTER_17
, DLAU_SHM_DL_FILTER_18
, DLAU_SHM_DL_FILTER_19
, DLAU_SHM_DL_FILTER_20
, DLAU_SHM_DL_FILTER_21
, DLAU_SHM_DL_FILTER_22
, DLAU_SHM_DL_FILTER_23
, DLAU_SHM_DL_FILTER_24
, DLAU_SHM_DL_FILTER_25
, DLAU_SHM_DL_FILTER_26
, DLAU_SHM_DL_FILTER_27
, DLAU_SHM_DL_FILTER_28
, DLAU_SHM_DL_FILTER_29
, DLAU_SHM_DL_FILTER_30
, DLAU_SHM_DL_FILTER_31
, DLAU_SHM_DL_VOLUME_SETTINGS_00
, DLAU_SHM_DL_VOLUME_SETTINGS_01
, DLAU_SHM_DL_VOLUME_SETTINGS_02
, DLAU_SHM_DL_VOLUME_SETTINGS_03
, DLAU_SHM_DL_VOLUME_SETTINGS_04
, DLAU_SHM_DL_VOLUME_SETTINGS_05
, DLAU_SHM_DL_VOLUME_SETTINGS_06
, DLAU_SHM_DL_VOLUME_SETTINGS_07
, DLAU_SHM_DL_VOLUME_SETTINGS_08
, DLAU_SHM_DL_VOLUME_SETTINGS_09
, DLAU_SHM_DL_VOLUME_SETTINGS_10
, DLAU_SHM_DL_VOLUME_SETTINGS_11
, DLAU_SHM_DL_VOLUME_SETTINGS_12
, DLAU_SHM_DL_VOLUME_SETTINGS_13
, DLAU_SHM_DL_VOLUME_SETTINGS_14
, DLAU_SHM_DL_VOLUME_SETTINGS_15
, DLAU_SHM_DL_VOLUME_SETTINGS_16
, DLAU_SHM_DL_VOLUME_SETTINGS_17
, DLAU_SHM_DL_VOLUME_SETTINGS_18
, DLAU_SHM_DL_VOLUME_SETTINGS_19
, DLAU_SHM_DL_VOLUME_SETTINGS_20
, DLAU_SHM_DL_VOLUME_SETTINGS_21
, DLAU_SHM_DL_PEAK_LIMITER_GAIN

  /* Generic representations */
, DLAU_UL_MIC_GAIN
, DLAU_UL_MIC_BIAS
, DLAU_UL_MIC_CFG
, DLAU_UL_MIC_ADC
, DLAU_UL_MIC_PGA
, DLAU_UL_MIC_ENABLE_PREAMP
, DLAU_UL_MIC_MUTE
, DLAU_UL_MIC_VCC_STATE
, DLAU_DL_PGA_GAIN_00
, DLAU_DL_PGA_GAIN_01
, DLAU_DL_PGA_GAIN_02
, DLAU_DL_PGA_GAIN_03
, DLAU_DL_PGA_GAIN_04
, DLAU_DL_SIDETONE_FACTOR
, DLAU_DL_SAMPLING_RATE
, DLAU_DL_STEREO_MODE
, DLAU_DAI_MODE
, DLAU_RESET_TO_DEFAULT_VALUES

, DLAU_NUMBER_OF_ENTITIES
} DlAudioEntityId;

typedef enum DlAudioDriverIdTag
{
  DLAU_DRIVER_ID_DLDSPAUDIO
, DLAU_DRIVER_ID_DLAU6521
, DLAU_DRIVER_ID_DLAU6522
, DLAU_DRIVER_ID_DLAU6528
, DLAU_DRIVER_ID_DLAU6532
, DLAU_DRIVER_ID_DLAU65XX
, DLAU_DRIVER_ID_DLAUBT
, DLAU_DRIVER_ID_DLAUCHC
, DLAU_DRIVER_ID_NUMBER_OF_IDS
} DlAudioDriverId;

typedef enum DlAudioIOTag
{
  /* BBC (e.g. AD6521/AD6535/AD6555) Inputs and Outputs */
  DL_AUD_IO_BBC_INPUT_DISABLED
, DL_AUD_IO_BBC_INPUT_1_ENABLED
, DL_AUD_IO_BBC_INPUT_2_ENABLED
, DL_AUD_IO_BBC_OUTPUT_DISABLED
, DL_AUD_IO_BBC_OUTPUT_1_ENABLED
, DL_AUD_IO_BBC_OUTPUT_2_ENABLED
, DL_AUD_IO_BBC_OUTPUT_3_ENABLED
, DL_AUD_IO_BBC_OUTPUT_RESERVED

  /* MCU Inputs and Outputs */
, DL_AUD_IO_MCU_OUTPUT_BUZZER_DISABLED
, DL_AUD_IO_MCU_OUTPUT_BUZZER_ENABLED

  /* Bluetooth Inputs and Outputs */
, DL_AUD_IO_BT_INPUT_DISABLED
, DL_AUD_IO_BT_INPUT_ENABLED
, DL_AUD_IO_BT_OUTPUT_DISABLED
, DL_AUD_IO_BT_OUTPUT_ENABLED

   /* Any other Inputs and Outputs to be added here */
} DlAudioIO;

typedef struct DirectControlWriteEntityTag
{
  DlAudioEntityId dlAudioEntityId;
  Int16           newValue;
} DirectControlWriteEntity;

typedef struct DirectControlReadEntityTag
{
  DlAudioEntityId dlAudioEntityId;
  Int16           minRange;
  Int16           maxRange;
  Int16           currentValue;
} DirectControlReadEntity;

typedef struct L1AlAudioDirectControlCommandTag
{
  TaskId              taskId;
  Int16               commandRef;
  DirectControlWriteEntity directControlEntityArray[MAX_DIRECT_CONTROL_WRITE_ENTITIES];
} L1AlAudioDirectControlCommand;

typedef enum L1AlDirectControlRdRdallWriteTag
{
  L1AL_DCC_READ,
  L1AL_DCC_READ_ALL,
  L1AL_DCC_WRITE
} L1AlDirectControlRdRdallWrite;


#if !defined (DISABLE_LEGACY_AUDIO_L1AL)

typedef struct DlAudioSlowClockVariablesTag
{
  Boolean             dlAudioCanSlowClock;
#if defined (UPGRADE_AD6535) || defined (UPGRADE_AD6555) || defined (UPGRADE_AD6852) || defined (UPGRADE_AD6855)
  Boolean             dlAu65xxCanSlowClock;
#endif
#if defined (UPGRADE_AD6521)
  Boolean             dlAu6521CanSlowClock;
#endif
#if defined (UPGRADE_AD6522)
  Boolean             dlAu6522CanSlowClock;
#endif
#if defined (UPGRADE_AD6526)
  Boolean             dlAu6528CanSlowClock;
#endif
#if defined (UPGRADE_AD6528)
  Boolean             dlAu6528CanSlowClock;
#endif
#if defined (UPGRADE_AD6532)
  Boolean             dlAu6532CanSlowClock;
#endif
  Boolean             dlAuDspAudioCanSlowClock;
/* For future expansion. New mixed signal devices go here with UPGRADE_xxx switches
 *  Boolean             dlAuNewMixedSignal1CanSlowClock;
 *  Boolean             dlAuNewMixedSignal2CanSlowClock;
 *  Boolean             dlAuNewMixedSignal3CanSlowClock;         */
  Boolean             m_ChannelAnyMarkAsEnabled;
  Int32               enabledChannels;
} DlAudioSlowClockVariables;

typedef struct DlDspAudioVariablesTag
{
  Boolean             dspAudioIsActive;
  Int16               auDataAcqDeviceId;
  Boolean             dlDspEnableDac;
  L1AudioChannel      dlDspUplinkChannel;
  L1AudioChannel      dlDspDownlinkChannel;
  L1AudioChannel      dlDspToneSrcChannel;
} DlDspAudioVariables;

#endif /* !DISABLE_LEGACY_AUDIO_L1AL */

#endif /* L1AL_TYP_H */
/* END OF FILE */
