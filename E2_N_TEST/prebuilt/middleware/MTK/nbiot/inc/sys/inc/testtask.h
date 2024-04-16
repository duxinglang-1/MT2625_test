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
 * File Description : Protocol stack test task definitions
 **************************************************************************/

/* UPGRADE_COOLART should never be defined for a MS target build.
 * It is used exclusively for the testing of protocol stacks */
#if defined (UPGRADE_COOLART)
# include <casystsk.h>
#endif /* UPGRADE_COOLART */

#if defined (NB_PC_HW_SIM)
  KI_TASK_DEF (NB_PC_PHY_ISR_TASK_ID,              NB_PC_PHY_ISR_QUEUE_ID,        NbPcPhyISRTask,            1000,    30, NB_PC_PHY_ISR_TASK_PRIO)
  KI_TASK_DEF (NB_PC_PHY_HW_TASK_ID,               NB_PC_PHY_HW_QUEUE_ID,         NbPcPhyHwTask,             1000,    30, NB_PC_PHY_HW_TASK_PRIO)
#else /* NB_PC_HW_SIM */
  KI_RESERVED_TASK (NB_PC_PHY_ISR_TASK_ID,        NB_PC_PHY_ISR_QUEUE_ID)
  KI_RESERVED_TASK (NB_PC_PHY_HW_TASK_ID,         NB_PC_PHY_HW_QUEUE_ID)
#endif /* NB_PC_HW_SIM */

#if defined (NB_PC_L1_NET_SIM)
  KI_TASK_DEF(NB_PC_L1_NET_SIM_TASK_ID,           NB_PC_L1_NET_SIM_QUEUE_ID,      NbL1NetSimTask,            1000,     30, NB_PC_L1_NET_SIM_TASK_PRIO)
#else
  KI_RESERVED_TASK(NB_PC_L1_NET_SIM_TASK_ID,      NB_PC_L1_NET_SIM_QUEUE_ID)
#endif
