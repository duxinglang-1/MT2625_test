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
 * GKI header file that defines the task definition macros. These are used
 * to extract the RTOS independent information from the task definitions.
 * RTOS dependant information is extracted using the macros in kitskdefos.h.
 **************************************************************************/

/***************************************************************************
 * Make sure no macros are already defined
 **************************************************************************/

#if defined (KI_TASK_DEF)
#   undef KI_TASK_DEF
#endif
#if defined (KI_TASKEX_DEF)
#   undef KI_TASKEX_DEF
#endif
#if defined (KI_ALIAS_DEF)
#   undef KI_ALIAS_DEF
#endif
#if defined (KI_START_ALIAS)
#   undef KI_START_ALIAS
#endif
#if defined (KI_STOP_ALIAS)
#   undef KI_STOP_ALIAS
#endif
#if defined (KI_RESERVED_TASK)
#   undef KI_RESERVED_TASK
#endif
#if defined (KI_TASK_SET_START)
#   undef KI_TASK_SET_START
#endif
#if defined (KI_TASK_SET_STOP)
#   undef KI_TASK_SET_STOP
#endif

/* Make sure no stack pool macros are defined */
#if defined (KI_TASK_STACK_POOL_0_DEF)
#   undef KI_TASK_STACK_POOL_0_DEF
#endif
#if defined (KI_TASK_STACK_POOL_1_DEF)
#   undef KI_TASK_STACK_POOL_1_DEF
#endif
#if defined (KI_TASK_STACK_POOL_2_DEF)
#   undef KI_TASK_STACK_POOL_2_DEF
#endif
#if defined (KI_TASK_STACK_POOL_3_DEF)
#   undef KI_TASK_STACK_POOL_3_DEF
#endif

/*
** KI_TASK_DEF_TYPE should be defined to be one of the defined
** values below by the including file
*/
/*
** This file deiberately does not have a
** #if !defined KITSKDEF_H
** as it can be legitimately included twice
*/

#define KI_TASK_IDS                  1
#define KI_QUEUE_IDS                 2
#define KI_REAL_TASKS                3
#define KI_TASK_SET_INDEX_TO_INIT    4
#define KI_TASK_SETS                 5
#define KI_TASK_SET_INDEX_CALC       6
#define KI_TASK_SET_INDEX_USE        7
#define KI_TASK_SET_INDEX_TO_ID      8
#define KI_TASK_NAMES				 9
#define KI_TASK_CFG_UNUSED         200

/***************************************************************************
 * OS indepentant macros
 **************************************************************************/

#if KI_TASK_DEF_TYPE == KI_TASK_IDS
#   define KI_TASK_DEF(TASK_ID,QUEUE_ID,ENTRY_POINT,STACK_SIZE,QUEUE_SIZE,PRIORITY) TASK_ID,
#   define KI_ALIAS_DEF(TASK_ID, ALIAS_TASK_ID, ALIAS_QUEUE_ID)                     ALIAS_TASK_ID = TASK_ID,
#   define KI_START_ALIAS(ALIAS_ID)           START_##ALIAS_ID,
#   define KI_STOP_ALIAS(ALIAS_ID)            STOP_##ALIAS_ID = START_##ALIAS_ID - 1,
#   define KI_RESERVED_TASK(TASK_ID,QUEUE_ID) TASK_ID,
#   define KI_TASK_SET_START(SET_NUM)         KI_TASK_SET_START_##SET_NUM = (SET_NUM << KI_TASK_SET_SIZE_POWER_OF_2),
#   define KI_TASK_SET_STOP(SET_NUM)          KI_TASK_SET_STOP_##SET_NUM,
#endif

#if KI_TASK_DEF_TYPE == KI_QUEUE_IDS
#   define KI_TASK_DEF(TASK_ID,QUEUE_ID,ENTRY_POINT,STACK_SIZE,QUEUE_SIZE,PRIORITY) QUEUE_ID = TASK_ID,
#   define KI_ALIAS_DEF(TASK_ID, ALIAS_TASK_ID, ALIAS_QUEUE_ID)                     ALIAS_QUEUE_ID = TASK_ID,
#   define KI_RESERVED_TASK(TASK_ID,QUEUE_ID)                                       QUEUE_ID = TASK_ID,
#endif

#if KI_TASK_DEF_TYPE == KI_REAL_TASKS
#   define KI_TASK_DEF(TASK_ID,QUEUE_ID,ENTRY_POINT,STACK_SIZE,QUEUE_SIZE,PRIORITY) KI_REAL_##TASK_ID,
#endif

#if KI_TASK_DEF_TYPE == KI_TASK_SET_INDEX_TO_INIT
#   define KI_TASK_DEF(TASK_ID,QUEUE_ID,ENTRY_POINT,STACK_SIZE,QUEUE_SIZE,PRIORITY) KI_REAL_##TASK_ID,
#   define KI_RESERVED_TASK(TASK_ID,QUEUE_ID)                                       KI_NUM_OF_REAL_TASKS,
#   define KI_TASK_SET_START(SET_NUM)                                               KI_NUM_OF_REAL_TASKS,
#endif

#if KI_TASK_DEF_TYPE == KI_TASK_SETS
#   define KI_TASK_SET_START(SET_NUM)             KI_TASK_SET_##SET_NUM##_ID = SET_NUM,
#endif

#if KI_TASK_DEF_TYPE == KI_TASK_SET_INDEX_CALC
#   define KI_TASK_DEF(TASK_ID,QUEUE_ID,ENTRY_POINT,STACK_SIZE,QUEUE_SIZE,PRIORITY) KI_TASK_SET_INDEX_##TASK_ID,
#   define KI_RESERVED_TASK(TASK_ID,QUEUE_ID)                                       KI_TASK_SET_INDEX_##TASK_ID,
#   define KI_TASK_SET_START(SET_NUM)                                               KI_TASK_SET_INDEX_##SET_NUM,
#endif

#if KI_TASK_DEF_TYPE == KI_TASK_SET_INDEX_USE
#   define KI_TASK_SET_START(SET_NUM)             KI_TASK_SET_INDEX_##SET_NUM,
#endif

#if KI_TASK_DEF_TYPE == KI_TASK_SET_INDEX_TO_ID
#   define KI_TASK_DEF(TASK_ID,QUEUE_ID,ENTRY_POINT,STACK_SIZE,QUEUE_SIZE,PRIORITY) TASK_ID,
#   define KI_RESERVED_TASK(TASK_ID,QUEUE_ID)                                       TASK_ID,
#   define KI_TASK_SET_START(SET_NUM)                                               KI_TASK_SET_START_##SET_NUM,
#endif

#if KI_TASK_DEF_TYPE == KI_TASK_NAMES
#   define KI_TASK_DEF(TASK_ID,QUEUE_ID,ENTRY_POINT,STACK_SIZE,QUEUE_SIZE,PRIORITY) {#TASK_ID},
#endif

/*
** Example task specification
**
**              Task Id      Queue Id      Entry Point     Stack  Priority
** KI_TASK_DEF (SIM_TASK_ID, SIM_QUEUE_ID, SimManagerTask, 1024,  8)
**
** use KI_RESERVED_TASK to reserve places in the task table without defining
** an entry point.
**
** KI_RESERVED_TASK (ALPB_TASK_ID, ALPB_QUEUE_ID)
**
** Example alias definition
**
**               Real Task Id  Alias Task Id   Alias Queue Id
** KI_START_ALIAS (EX_ALIASES)
** KI_ALIAS_DEF (AL_TASK_ID,   ALSM_TASK_ID,   ALSM_QUEUE_ID)
** KI_ALIAS_DEF (AL_TASK_ID,   ALSS_TASK_ID,   ALSS_QUEUE_ID)
** KI_STOP_ALIAS (EX_ALIASES)
**
** WARNING: When using aliases the value in the enumeration must be stored
**          first (using the KI_START_ALIAS macro) and then re-instated after
**          (using the KI_END_ALIAS macro) the alias definition.
*/

/***************************************************************************
 * Define defaults
 **************************************************************************/

#if !defined (KI_TASK_DEF)
#   define KI_TASK_DEF(TASK_ID,QUEUE_ID,ENTRY_POINT,STACK_SIZE,QUEUE_SIZE,PRIORITY)
#endif
#if !defined (KI_TASKEX_DEF)
#   define KI_TASKEX_DEF(TASK_ID,QUEUE_ID,ENTRY_POINT,STACK_SIZE,QUEUE_SIZE,PRIORITY,STACKPOOL) KI_TASK_DEF(TASK_ID,QUEUE_ID,ENTRY_POINT,STACK_SIZE,QUEUE_SIZE,PRIORITY)
#endif
#if !defined (KI_ALIAS_DEF)
#   define KI_ALIAS_DEF(TASK_ID, ALIAS_TASK_ID, ALIAS_QUEUE_ID)
#endif
#if !defined (KI_START_ALIAS)
#   define KI_START_ALIAS(ALIAS_ID)
#endif
#if !defined (KI_STOP_ALIAS)
#   define KI_STOP_ALIAS(ALIAS_ID)
#endif
#if !defined (KI_RESERVED_TASK)
#   define KI_RESERVED_TASK(TASK_ID,QUEUE_ID)
#endif
#if !defined (KI_TASK_SET_START)
#   define KI_TASK_SET_START(SET_NUM)
#endif
#if !defined (KI_TASK_SET_STOP)
#   define KI_TASK_SET_STOP(SET_NUM)
#endif

#undef KI_TASK_DEF_TYPE

/* END OF FILE */
