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
 * GKI header file that defines the maximum number of timers in the system.
 **************************************************************************/

#ifndef KISYSTIM_H
#define KISYSTIM_H

/****************************************************************************
 * Defines
 ****************************************************************************/

/*
** The total number of timers used simultaneously in each unit
*/
#define     NUMBER_OF_SMS_TIMERS     35 /* 14 concurrent transctions */
#define     NUMBER_OF_CB_TIMERS      1
#define     NUMBER_OF_SS_TIMERS      1
#define     NUMBER_OF_CC_TIMERS      2
#define     NUMBER_OF_MM_TIMERS      2
#define     NUMBER_OF_RR_TIMERS      26
#define     NUMBER_OF_L1_TIMERS      8
#define     NUMBER_OF_L2_TIMERS      2

#define     NUMBER_OF_AL_TIMERS      10

# define     NUMBER_OF_VG_TIMERS     60  /* Maximum SREJs */

#if defined(UPGRADE_BT_BASEBAND)
# define     NUMBER_OF_LM_TIMERS     1
#else
# define     NUMBER_OF_LM_TIMERS     0
#endif

#if defined(UPGRADE_BT)
# if defined(UPGRADE_BT_BASEBAND)
#  define     NUMBER_OF_HL_TIMERS    20
# else
#  define     NUMBER_OF_HL_TIMERS    3
# endif
#else
# define  NUMBER_OF_HL_TIMERS        0
#endif

#if defined(UPGRADE_COOLART)
# define NUMBER_OF_CA_TIMERS         50
#else
# define NUMBER_OF_CA_TIMERS         0
#endif

/** \ingroup PrdGki
 * The maximum number of active GKI timers at any one time in the system. */
#define  KI_TIM_MAX_NUMBER_OF_TIMERS    (NUMBER_OF_AL_TIMERS  +       \
                                         NUMBER_OF_VG_TIMERS  +       \
                                         NUMBER_OF_SMS_TIMERS +       \
                                         NUMBER_OF_CB_TIMERS  +       \
                                         NUMBER_OF_SS_TIMERS  +       \
                                         NUMBER_OF_CC_TIMERS  +       \
                                         NUMBER_OF_MM_TIMERS  +       \
                                         NUMBER_OF_RR_TIMERS  +       \
                                         NUMBER_OF_L2_TIMERS  +       \
                                         NUMBER_OF_L1_TIMERS  +       \
                                         NUMBER_OF_LM_TIMERS  +       \
                                         NUMBER_OF_HL_TIMERS  +       \
                                         NUMBER_OF_CA_TIMERS)

#endif /* KISYSTIM_H */

/* END OF FILE */
