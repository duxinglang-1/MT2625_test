/* Copyright Statement:
 *
 * (C) 2005-2017 MediaTek Inc. All rights reserved.
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
 * the License Agreement ("Permitted User"). If you are not a Permitted User,
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

#ifndef __TEL_CONN_MGR_FEATURES_H__
#define __TEL_CONN_MGR_FEATURES_H__


/* Doing UT with RIL simulation task enabled */
//#define TEL_CONN_MGR_UT

/* Doing IT with RIL simulation task disabled */
#define TEL_CONN_MGR_IT

/* Code added temporarily to bypass the issues when doing IT */
#define TEL_CONN_MGR_TEMP_IT

/* Notify TCPIP or NIDD task with the detailed bearer information. */
#ifndef TEL_CONN_MGR_UT
#define TEL_CONN_MGR_INFORM_TCPIP_NIDD
#endif

/* Send CEREG query before activation */
#define TEL_CONN_MGR_ENABLE_CHECK_CEREG

/* Enable INACTIVATE */
#define TEL_CONN_MGR_ENABLE_INACTIVATE

/* Enable deep sleep feature */
#define TEL_CONN_MGR_SUPPORT_DEEP_SLEEP

#ifdef TEL_CONN_MGR_SUPPORT_DEEP_SLEEP
#define TEL_CONN_MGR_DEEP_SLEEP_USE_RTC_RAM
#endif

/* Enable the AP Bridge AT CMDs for triggering the test cases */
#define TEL_CONN_MGR_APB_SUPPORT

/* Enable the callback interface */
#define TEL_CONN_MGR_SUPPORT_CALLBACK

/* Skip CGAUTH when username is null. */
//#define TEL_CONN_MGR_SKIP_CGAUTH_WHEN_NO_AUTH

/* Enable the activation guard timer. It is not supported in freertos platform. */
//#define TEL_CONN_MGR_ACTIVATION_GUARD_TIMER_SUPPORT

/* Enable the deactivation guard timer. It is not supported in freertos platform. */
//#define TEL_CONN_MGR_DEACTIVATION_GUARD_TIMER_SUPPORT

#endif /* __TEL_CONN_MGR_FEATURES_H__ */

