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
 *  File Description :
 *      L1AL signal definitions
 *
 ****************************************************************************
 * Please, please, please do not make any additions to this file wrapped in
 * #if defined() #endif.  New signals should still be added next to the most
 * appropriate group as before.
 **************************************************************************/

#if !defined (EXCLUDE_L1AL)
    SIG_DEF( SIG_L1AL_DUMMY  = L1AL_SIGNAL_BASE,   EmptySignal                 l1AlDummy)

    SIG_DEF( SIG_L1AL_MTS_TIMER_START_REQ,         L1AlMtsTimerStartReq        l1AlMtsTimerStartReq)
    SIG_DEF( SIG_L1AL_MTS_TIMER_START_CNF,         L1AlMtsTimerStartCnf        l1AlMtsTimerStartCnf)
    SIG_DEF( SIG_L1AL_MTS_TIMER_STOP_REQ,          L1AlMtsTimerStopReq         l1AlMtsTimerStopReq)
    SIG_DEF( SIG_L1AL_MTS_TIMER_STOP_CNF,          L1AlMtsTimerStopCnf         l1AlMtsTimerStopCnf)
    SIG_DEF( SIG_L1AL_AUX_DAC_SET_REQ,             L1AlAuxDacSetReq            l1AlAuxDacSetReq)
    SIG_DEF( SIG_L1AL_PWM_START_REQ,               L1AlPwmStartReq             l1AlPwmStartReq)
    SIG_DEF( SIG_L1AL_PWM_STOP_REQ,                L1AlPwmStopReq              l1AlPwmStopReq)
    SIG_DEF( SIG_L1AL_DUMMY1,                      EmptySignal                 l1alDummy1EmptySig)
    SIG_DEF( SIG_L1AL_DUMMY2,                      EmptySignal                 l1alDummy2EmptySig)
    SIG_DEF( SIG_L1AL_STATUS_IND,                  L1AlStatusInd               l1AlStatusInd)
    SIG_DEF( SIG_L1AL_DUMMY3,                      EmptySignal                 l1alDummy3EmptySig)
    /* Internal signal within Layer 1 */
    SIG_DEF( SIG_L1AL_START_ALARM_TIMER_IND,       EmptySignal                 l1alStartAlarmTimerInd)
    SIG_DEF( SIG_L1AL_DUMMY4,                      EmptySignal                 l1alDummy4EmptySig)
    SIG_DEF( SIG_L1AL_DUMMY5,                      EmptySignal                 l1alDummy5EmptySig)

#if !defined (DISABLE_LEGACY_AUDIO_L1AL)
/************** Application Layer <-> Layer 1 Audio Signals **********************/

    SIG_DEF( SIG_L1AL_AUDIO_ERROR_IND,                      L1AlAudioErrorInd                  l1AlAudioErrorInd                 )
    SIG_DEF( SIG_L1AL_AUDIO_CHANNEL_SETUP_REQ,              L1AlAudioChannelSetupReq           l1AlAudioChannelSetupReq          )
    SIG_DEF( SIG_L1AL_AUDIO_CHANNEL_DISABLE_IND,            L1AlAudioChannelDisableInd         l1AlAudioChannelDisableInd        )
    SIG_DEF( SIG_L1AL_AUDIO_CHANNEL_TERMINATE_REQ,          L1AlAudioChannelTerminateReq       l1AlAudioChannelTerminateReq      )
    SIG_DEF( SIG_L1AL_AUDIO_CHANNEL_TERMINATE_CNF,          L1AlAudioChannelTerminateCnf       l1AlAudioChannelTerminateCnf      )
    SIG_DEF( SIG_L1AL_AUDIO_CHANNEL_SETUP_CNF,              L1AlAudioChannelSetupCnf           l1AlAudioChannelSetupCnf          )
    SIG_DEF( SIG_L1AL_AUDIO_VOLUME_REQ,                     L1AlAudioVolumeReq                 l1AlAudioVolumeReq                )
    SIG_DEF( SIG_L1AL_AUDIO_VOLUME_CNF,                     L1AlAudioVolumeCnf                 l1AlAudioVolumeCnf                )
    SIG_DEF( SIG_L1AL_AUDIO_VOLUME_RAMP_REQ,                L1AlAudioVolumeRampReq             l1AlAudioVolumeRampReq            )
    SIG_DEF( SIG_L1AL_AUDIO_CHANNEL_MUTE_REQ,               L1AlAudioChannelMuteReq            l1AlAudioChannelMuteReq           )
    SIG_DEF( SIG_L1AL_AUDIO_CHANNEL_MUTE_CNF,               L1AlAudioChannelMuteCnf            l1AlAudioChannelMuteCnf           )
    SIG_DEF( SIG_L1AL_AUDIO_MUTE_REQ,                       L1AlAudioMuteReq                   l1AlAudioMuteReq                  )
    SIG_DEF( SIG_L1AL_AUDIO_MUTE_CNF,                       L1AlAudioMuteCnf                   l1AlAudioMuteCnf                  )
    SIG_DEF( SIG_L1AL_AUDIO_SPEECH_REQ,                     L1AlAudioSpeechReq                 l1AlAudioSpeechReq                )
    SIG_DEF( SIG_L1AL_AUDIO_SPEECH_CNF,                     L1AlAudioSpeechCnf                 l1AlAudioSpeechCnf                )

/************** Application Layer <-> Layer 1  Audio tone interface Signals **********************/

    SIG_DEF( SIG_L1AL_AUDIO_CALL_TONE_REQ,                  L1AlAudioCallToneReq               l1AlAudioCallToneReq              )
    SIG_DEF( SIG_L1AL_AUDIO_CALL_TONE_CNF,                  L1AlAudioCallToneCnf               l1AlAudioCallToneCnf              )
    SIG_DEF( SIG_L1AL_AUDIO_DTMF_REQ,                       L1AlAudioDtmfReq                   l1AlAudioDtmfReq                  )
    SIG_DEF( SIG_L1AL_AUDIO_DTMF_CNF,                       L1AlAudioDtmfCnf                   l1AlAudioDtmfCnf                  )
    SIG_DEF( SIG_L1AL_AUDIO_CUSTOM_TONE_REQ,                L1AlAudioCustomToneReq             l1AlAudioCustomToneReq            )
    SIG_DEF( SIG_L1AL_AUDIO_CUSTOM_TONE_BUFFER_FREE_IND,    L1AlAudioCustomToneBufferFreeInd   l1AlAudioCustomToneBufferFreeInd  )
    SIG_DEF( SIG_L1AL_AUDIO_CUSTOM_TONE_STOP_REQ,           L1AlAudioCustomToneStopReq         l1AlAudioCustomToneStopReq        )
    SIG_DEF( SIG_L1AL_AUDIO_CUSTOM_TONE_STOP_IND,           L1AlAudioCustomToneStopInd         l1AlAudioCustomToneStopInd        )
/************** Application Layer <-> Layer 1  Audio predefined melody interface Signals **********************/

    SIG_DEF( SIG_L1AL_AUDIO_PREDEFINED_REQ,                 L1AlAudioPredefinedReq             l1AlAudioPredefinedReq            )
    SIG_DEF( SIG_L1AL_AUDIO_PREDEFINED_CNF,                 L1AlAudioPredefinedCnf             l1AlAudioPredefinedCnf            )
    SIG_DEF( SIG_L1AL_AUDIO_PREDEFINED_STOP_REQ,            L1AlAudioPredefinedStopReq         l1AlAudioPredefinedStopReq        )
    SIG_DEF( SIG_L1AL_AUDIO_PREDEFINED_STOP_IND,            L1AlAudioPredefinedStopInd         l1AlAudioPredefinedStopInd        )
    SIG_DEF( SIG_L1AL_AUDIO_PREDEFINED_RESTART_IND,         L1AlAudioPredefinedRestartInd      l1AlAudioPredefinedRestartInd     )
    SIG_DEF( SIG_L1AL_AUDIO_PREDEFINED_STORE_REQ,           L1AlAudioPredefinedStoreReq        l1AlAudioPredefinedStoreReq       )
    SIG_DEF( SIG_L1AL_AUDIO_PREDEFINED_STORE_CNF,           L1AlAudioPredefinedStoreCnf        l1AlAudioPredefinedStoreCnf       )
    SIG_DEF( SIG_L1AL_AUDIO_PREDEFINED_ERASE_REQ,           L1AlAudioEraseReq                  l1AlAudioPredefinedEraseReq       )
    SIG_DEF( SIG_L1AL_AUDIO_PREDEFINED_ERASE_CNF,           L1AlAudioPredefinedEraseCnf        l1AlAudioPredefinedEraseCnf       )
    SIG_DEF( SIG_L1AL_AUDIO_PREDEFINED_BUFFER_SETUP_REQ,    L1AlAudioPredefinedBufferSetupReq  l1AlAudioPredefinedBufferSetupReq )
    SIG_DEF( SIG_L1AL_AUDIO_PREDEFINED_BUFFER_SETUP_CNF,    L1AlAudioPredefinedBufferSetupCnf  l1AlAudioPredefinedBufferSetupCnf )
/************** Application Layer <-> Layer 1  Audio record/playback interface Signals **********************/
    SIG_DEF( SIG_L1AL_AUDIO_LOOPBACK_REQ,                   L1AlAudioLoopBackReq               l1AlAudioLoopBackReq              )
    SIG_DEF( SIG_L1AL_AUDIO_LOOPBACK_IND,                   L1AlAudioLoopBackInd               l1AlAudioLoopBackInd              )
    SIG_DEF( SIG_L1AL_AUDIO_LOOPBACK_CNF,                   L1AlAudioLoopBackCnf               l1AlAudioLoopBackCnf              )
    SIG_DEF( SIG_L1AL_AUDIO_RECORD_REQ,                     L1AlAudioRecordReq                 l1AlAudioRecordReq                )
    SIG_DEF( SIG_L1AL_AUDIO_RECORD_CNF,                     L1AlAudioRecordCnf                 l1AlAudioRecordCnf                )
    SIG_DEF( SIG_L1AL_MEMO_RECORD_STOP_IND,                 L1AlMemoRecordStopInd              l1AlMemoRecordStopInd             )
    SIG_DEF( SIG_L1AL_AUDIO_PLAYBACK_REQ,                   L1AlAudioPlaybackReq               l1AlAudioPlaybackReq              )
    SIG_DEF( SIG_L1AL_AUDIO_PLAYBACK_CNF,                   L1AlAudioPlaybackCnf               l1AlAudioPlaybackCnf              )
    SIG_DEF( SIG_L1AL_AUDIO_STOP_REQ,                       L1AlAudioStopReq                   l1AlAudioStopReq                  )
    SIG_DEF( SIG_L1AL_AUDIO_STOP_CNF,                       L1AlAudioStopCnf                   l1AlAudioStopCnf                  )
    SIG_DEF( SIG_L1AL_AUDIO_STOP_IND,                       L1AlAudioStopInd                   l1AlAudioStopInd                  )
    SIG_DEF( SIG_L1AL_AUDIO_STORE_REQ,                      L1AlAudioStoreReq                  l1AlAudioStoreReq                 )
    SIG_DEF( SIG_L1AL_AUDIO_STORE_CNF,                      L1AlAudioStoreCnf                  l1AlAudioStoreCnf                 )
    SIG_DEF( SIG_L1AL_AUDIO_ERASE_REQ,                      L1AlAudioEraseReq                  l1AlAudioEraseReq                 )
    SIG_DEF( SIG_L1AL_AUDIO_ERASE_CNF,                      L1AlAudioEraseCnf                  l1AlAudioEraseCnf                 )
    SIG_DEF( SIG_L1AL_AUDIO_BUFFER_SETUP_REQ,               L1AlAudioBufferSetupReq            l1AlAudioBufferSetupReq           )
    SIG_DEF( SIG_L1AL_AUDIO_BUFFER_SETUP_CNF,               L1AlAudioBufferSetupCnf            l1AlAudioBufferSetupCnf           )
    SIG_DEF( SIG_L1AL_MEMO_PLAYBACK_BUFFER_FREE_REQ,        L1AlMemoPlaybackBufferFreeReq      l1AlMemoPlaybackBufferFreeReq     )

/************** Application Layer <-> Layer 1  PCM Streaming input/output/stop interface Signals **********************/
    SIG_DEF( SIG_L1AL_PCM_STREAMING_INPUT_REQ,              L1AlPCMStreamingInputReq           l1AlPCMStreamingInputReq          )
    SIG_DEF( SIG_L1AL_PCM_STREAMING_OUTPUT_REQ,             L1AlPCMStreamingOutputReq          l1AlPCMStreamingOutputReq         )
    SIG_DEF( SIG_L1AL_PCM_STREAMING_STOP_REQ,               L1AlPCMStreamingStopReq            l1AlPCMStreamingStopReq           )
    SIG_DEF( SIG_L1AL_PCM_STREAMING_INPUT_CNF,              L1AlPCMStreamingInputCnf           l1AlPCMStreamingInputCnf          )
    SIG_DEF( SIG_L1AL_PCM_STREAMING_OUTPUT_CNF,             L1AlPCMStreamingOutputCnf          l1AlPCMStreamingOutputCnf         )
    SIG_DEF( SIG_L1AL_PCM_STREAMING_STOP_CNF,               L1AlPCMStreamingStopCnf            l1AlPCMStreamingStopCnf           )

    SIG_DEF( SIG_L1AL_MMAC_CHANNEL_START_REQ,               L1AlMmacChannelStartReq            l1AlMmacChannelStartReq           )
    SIG_DEF( SIG_L1AL_MMAC_CHANNEL_START_CNF,               L1AlMmacChannelStartCnf            l1AlMmacChannelStartCnf           )
    SIG_DEF( SIG_L1AL_MMAC_CHANNEL_STOP_REQ,                L1AlMmacChannelStopReq             l1AlMmacChannelStopReq            )
    SIG_DEF( SIG_L1AL_MMAC_CHANNEL_STOP_IND,                L1AlMmacChannelStopInd             l1AlMmacChannelStopInd            )

    SIG_DEF( SIG_L1AL_AUDIO_PRINT_CHANNEL_DEFS,             EmptySignal                      l1AlAudioPrintChannelDefs         )
/*******************************************************************************************/
    SIG_DEF( SIG_L1AL_AUDIO_DECODE_BUFFER_FILL_IND,         L1AlAudioDecodeBufferFillInd       l1AlAudioDecodeBufferFillInd      )
    SIG_DEF( SIG_L1AL_AUDIO_DECODE_COMPLETE_IND,            L1AlAudioDecodeCompleteInd         l1AlAudioDecodeCompleteInd        )
    SIG_DEF( SIG_L1AL_AUDIO_MELODY_BUFFER_DATA_RSP,         L1AlAudioMelodyBufferDataRsp       l1AlAudioMelodyBufferDataRsp      )
    SIG_DEF( SIG_L1AL_AUDIO_FREE_BUFFER_MEMORY_IND,         L1AlAudioFreeBufferMemoryInd       l1AlAudioFreeBufferMemoryInd      )
/*******************************************************************************************/
    SIG_DEF( SIG_L1AL_AUDIO_CHANNEL_ENABLE_REQ,             L1AlAudioChannelEnableReq          l1AlAudioChannelEnableReq         )
    SIG_DEF( SIG_L1AL_AUDIO_CHANNEL_ENABLE_CNF,             L1AlAudioChannelEnableCnf          l1AlAudioChannelEnableCnf         )
    SIG_DEF( SIG_L1AL_AUDIO_CHANNEL_DISABLE_REQ,            L1AlAudioChannelDisableReq         l1AlAudioChannelDisableReq        )
    SIG_DEF( SIG_L1AL_AUDIO_CHANNEL_DISABLE_CNF,            L1AlAudioChannelDisableCnf         l1AlAudioChannelDisableCnf        )

    SIG_DEF( SIG_L1AL_AUDIO_BUFFER_DELAY_IND,               L1AlAudioBufferDelayInd            l1AlAudioBufferDelayInd)
#endif /* !DISABLE_LEGACY_AUDIO_L1AL */

    SIG_DEF( SIG_L1AL_BBC_DEBUG_REQ,                        L1AlBbcDebugReq                    l1AlBbcDebugReq)
    SIG_DEF( SIG_L1AL_BBC_DEBUG_CNF,                        L1AlBbcDebugCnf                    l1AlBbcDebugCnf)

#if !defined (DISABLE_LEGACY_AUDIO_L1AL)
    SIG_DEF( SIG_L1AL_AUTO_VERIFICATION_DATA_IND,           L1AlAutoVerificationDataInd        l1AlAutoVerificationDataInd)

/************** Application Layer <-> Layer 1  Direct Control of Driver/Hardware interface Signals **********************/
    SIG_DEF( SIG_L1AL_AUDIO_DIRECT_CONTROL_REQ,             L1AlAudioDirectControlReq          l1AlAudioDirectControlReq         )
    SIG_DEF( SIG_L1AL_AUDIO_DIRECT_CONTROL_CNF,             L1AlAudioDirectControlCnf          l1AlAudioDirectControlCnf         )
    SIG_DEF( SIG_L1AL_AUDIO_DIRECT_CONTROL_IND,             L1AlAudioDirectControlInd          l1AlAudioDirectControlInd         )
    SIG_DEF( SIG_L1AL_AUDIO_DIRECT_CONTROL_COMPLETE_IND,    L1AlAudioDirectControlCompleteInd  l1AlAudioDirectControlCompleteInd )
    SIG_DEF( SIG_L1AL_AUDIO_IO_IND,                         L1AlAudioIoInd                     l1AlAudioIoInd                    )
    SIG_DEF( SIG_L1AL_AUDIO_STATUS_REQ,                     L1AlAudioStatusReq                 l1AlAudioStatusReq                )
    SIG_DEF( SIG_L1AL_AUDIO_STATUS_CNF,                     L1AlAudioStatusCnf                 l1AlAudioStatusCnf                )
    SIG_DEF( SIG_L1AL_AUDIO_PREDEFINED_END_OF_PLAYBACK,     L1AlAudioPredefinedEndOfPlayback   l1AlAudioPredefinedEndOfPlayback  )
#endif /* !DISABLE_LEGACY_AUDIO_L1AL */

# endif  /* EXCLUDE_L1AL */

/* END OF FILE */
