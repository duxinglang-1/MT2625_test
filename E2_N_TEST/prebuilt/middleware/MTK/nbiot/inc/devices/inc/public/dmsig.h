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
 * File Description:   DM Signals
 **************************************************************************/

    SIG_DEF( SIG_DM_DUMMY = DM_SIGNAL_BASE,      EmptySignal                dmDummy                     )

#if defined (EXCLUDE_DM)
#if defined (MTK_NBIOT_TARGET_BUILD) || defined (UT_TARGET_CONFIG)
  /* TODO see if these can be removed */
    SIG_DEF( SIG_DM_RTC_READ_DATE_AND_TIME_REQ,    EmptySignal                dmRtcReadDateAndTimeReq  )
    SIG_DEF( SIG_DM_RTC_READ_DATE_AND_TIME_CNF,    EmptySignal                dmRtcReadDateAndTimeCnf  )
    SIG_DEF( SIG_DM_RTC_SET_DATE_AND_TIME_REQ,     EmptySignal                dmRtcSetDateAndTimeReq   )
    SIG_DEF( SIG_DM_RTC_SET_DATE_AND_TIME_CNF,     EmptySignal                dmRtcSetDateAndTimeCnf   )
    SIG_DEF( SIG_DM_RTC_SET_TIME_ZONE_REQ,         EmptySignal                dmRtcSetTimeZoneReq      )
    SIG_DEF( SIG_DM_RTC_SET_TIME_ZONE_CNF,         EmptySignal                dmRtcSetTimeZoneCnf      )
    SIG_DEF( SIG_DM_RTC_READ_TIME_ZONE_REQ,        EmptySignal                dmRtcReadTimeZoneReq     )
    SIG_DEF( SIG_DM_RTC_READ_TIME_ZONE_CNF,        EmptySignal                dmRtcReadTimeZoneCnf     )
    SIG_DEF( SIG_DM_RTC_DATE_AND_TIME_IND,         EmptySignal                dmRtcDateAndTimeInd      )

  /* Power Control Module */
    SIG_DEF( SIG_DM_POWER_CONTROL_REQ,             EmptySignal                dmPowerControlReq        )

  /* NVRAM Processor task signals. */
#   include <dmsig_nvp.h>
#endif
#else

  /* GPIO module */
    SIG_DEF( SIG_DM_GPIO_RECONFIGURE_REQ,          DmGpioReconfigureReq       dmGpioReconfigureReq      )
    SIG_DEF( SIG_DM_GPIO_ASSERT_REQ,               DmGpioAssertReq            dmGpioAssertReq           )
    SIG_DEF( SIG_DM_GPIO_STATUS_REQ,               DmGpioStatusReq            dmGpioStatusReq           )
    SIG_DEF( SIG_DM_GPIO_STATUS_CNF,               DmGpioStatusCnf            dmGpioStatusCnf           )
    SIG_DEF( SIG_DM_GPIO_INTERRUPT_IND,            DmGpioInterruptInd         dmGpioInterruptInd        )
    SIG_DEF( SIG_DM_GPIO_START_INT_TIMER_REQ,      PdGpioStartIntTimerReq     pdGpioStartIntTimerReq    )

  /* Power Control Module */
    SIG_DEF( SIG_DM_POWER_CONTROL_REQ,             DmPowerControlReq          dmPowerControlReq         )

    /* NVRAM Processor task signals. */
#   include <dmsig_nvp.h>

    SIG_DEF ( SIG_MPD_DTE_PROCESS_RX,              EmptySignal                mpdDteProcessRx           )
    SIG_DEF ( SIG_MPD_DTE_TXREADY_INT,             EmptySignal                mpdDteTxReadyInt          )
    SIG_DEF ( SIG_MPD_DTE_DTR_EVENT,               EmptySignal                mpdDteDtrEvent            )
    SIG_DEF ( SIG_MPD_DTE_RTS_EVENT,               EmptySignal                mpdDteRtsEvent            )
    SIG_DEF ( SIG_MPD_DTE_FRAME_ERROR_EVENT,       EmptySignal                mpdDteFrameErrEvent       )
    SIG_DEF ( SIG_MPD_DTE_PARITY_ERROR_EVENT,      EmptySignal                mpdDteParityErrEvent      )
    SIG_DEF ( SIG_MPD_DTE_BREAK_ERROR_EVENT,       EmptySignal                mpdDteBreakErrEvent       )
#endif
/* END OF FILE */

