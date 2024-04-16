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
  * This file implements the BMM (Buffer Memory Manager) and provides
  * prototypes for the functions that replace TMM.
  */

#ifndef BMM_H
#define BMM_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>

#if !defined (FR_PLP_BUILD) || defined (UPGRADE_SINGLE_CORE_MODEM)
#include <ki_typ.h>
#endif

#include <bmm_typ.h>

#if !defined (FR_PLP_BUILD) || defined (UPGRADE_SINGLE_CORE_MODEM)
/***************************************************************************
 * Type Definitions
 **************************************************************************/

typedef enum TmmPoolIDTag
{
   TMM_DEFAULT_POOL_UL,
   TMM_DEFAULT_POOL_DL,
   TMM_DEFAULT_POOL_RLC,
   TMM_DEFAULT_POOL_CTL,
   TMM_DEFAULT_POOL_UL_C,
   TMM_DEFAULT_POOL_DL_C,
   TMM_POOL_BIP,
   TMM_POOL_AP_BRIDGE,
   TMM_POOL_SOFTSIM,
   TMM_POOL_NVRAM
} TmmPoolId;

#if defined (KI_MEMORY_DEBUG)
# define KI_MEM_DBG_EXTRA_ARGS_DEF       , const char *file, int line
# define KI_MEM_DBG_EXTRA_ARGS_USE       , MODULE_NAME, __LINE__
# define KI_MEM_DBG_EXTRA_ARGS_PASS      , file, line
#else
# define KI_MEM_DBG_EXTRA_ARGS_DEF
# define KI_MEM_DBG_EXTRA_ARGS_USE
# define KI_MEM_DBG_EXTRA_ARGS_PASS
#endif

/***************************************************************************
* Macro       : M_BMM_GET_BLOCK_START_ADDRESS
***************************************************************************/
/** Obtain the BMM block start address for a given memory address by simply masking the block size.
* \Param[in]     mEM_P           Memory address.
*/
#define M_BMM_GET_BLOCK_START_ADDRESS( mEM_P ) ((Int32) (mEM_P & (~(BMM_BUFFER_SIZE-1)))

/*deinfe BMM function alise for purpose of memory leakage check*/
#define BmmAllocMemory(P,S,M)              BmmCfgAllocMemory (P,S,M KI_MEM_DBG_EXTRA_ARGS_USE)
#define BmmAllocBestMemoryAvail(P,S,B,M)   BmmCfgAllocBestMemoryAvail (P,S,B,M KI_MEM_DBG_EXTRA_ARGS_USE)
#define BmmIncBlockRef(M,C)                BmmCfgIncBlockRef (M, C KI_MEM_DBG_EXTRA_ARGS_USE)
#define BmmDecBlockRef(M,C)                BmmCfgDecBlockRef (M,C KI_MEM_DBG_EXTRA_ARGS_USE)
#define BmmFreeMemory(M)                   BmmCfgFreeMemory (M KI_MEM_DBG_EXTRA_ARGS_USE)

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

extern Boolean   TmmAllocMemory (TmmPoolId tmmPoolId, Int32 requestedBytes, Int8 **mem_pp);
extern Int32     TmmAllocBestMemoryAvail (TmmPoolId poolId, Int32 requestedBytes, Int32 minBytes, Int8 **mem_pp);
extern void      TmmFreeMemory (Int8 **mem_pp);
extern void      TmmFreeTail (Int32 len, Int8 **mem_pp);
extern void      TmmAddFlowControlTask (TmmPoolId poolId, TaskId taskId);
extern void      TmmRemoveFlowControlTask (TmmPoolId poolId, TaskId taskId);
extern void      TmmInit (void);
extern Int32     TmmGetBlockLength (Int8 *mem_p);
extern Boolean   TmmCanAllocFromPool (TmmPoolId poolId);

extern void      BmmCfgAllocMemory (BmmPoolId bmmPoolId, Int32 requestedBytes, Int8 **mem_pp KI_MEM_DBG_EXTRA_ARGS_DEF);
extern Int32     BmmCfgAllocBestMemoryAvail (BmmPoolId bmmPoolId, Int32 requestedBytes, Int32 minBytes, Int8 **mem_pp KI_MEM_DBG_EXTRA_ARGS_DEF);
extern Int8      BmmCfgFreeMemory (Int8 *free_p KI_MEM_DBG_EXTRA_ARGS_DEF);
extern Int32     BmmReserveBestMemoryAvail (BmmPoolId bmmPoolId, Int32 requestedBytes, Int32 minBytes,Int8 **reserve_pp);
extern void      BmmReserveMemory (BmmPoolId bmmPoolId, Int32 requestedBytes, Int8 **reserve_pp);
extern Boolean   BmmUpdateReserveMemory (Int8 *reserve_p);
extern Int8      BmmCfgIncBlockRef (Int8 *mem_p, Int8 count KI_MEM_DBG_EXTRA_ARGS_DEF);
extern Int8      BmmCfgDecBlockRef (Int8 *mem_p, Int8 count KI_MEM_DBG_EXTRA_ARGS_DEF);
extern void      BmmAddFlowControlTask (BmmPoolId bmmPoolId, TaskId taskId);
extern void      BmmRemoveFlowControlTask (BmmPoolId bmmPoolId, TaskId taskId);
extern Boolean   BmmCanAllocFromPool (BmmPoolId bmmPoolId);
extern void      BmmGetBufferInfo (Int32 bufferId, BmmBuffer **bmmBuffer_pp, Int8 ** refArray_pp);
extern void      TmmIncrementBlockRef (Int8 *mem_p);
extern Int32     TmmGetBlockRefs (Int8 *mem_p);
extern TmmPoolId TmmGetBlockPoolId (Int8 *mem_p);

#if defined (ENABLE_BMM_MEM_OP_DEBUG)
extern void      BmmReportBufferStatus (void);
#endif /* ENABLE_BMM_MEM_OP_DEBUG */

extern void      BmmInit (void);
extern Boolean   BmmIncPlpReserveDlMemory (Int8 *plpReserveStart_p, Int8 *plpReserveEnd_p);

#endif  /* #if !defined (FR_PLP_BUILD) || defined (UPGRADE_SINGLE_CORE_MODEM)*/

#if defined (FR_PLP_BUILD) || defined (UPGRADE_SINGLE_CORE_MODEM)
extern Int32     BmmReservePlpDlMemory (Int32 requestedBytes, Int8 **mem_pp);

#endif /* #if defined (FR_PLP_BUILD) || defined (UPGRADE_SINGLE_CORE_MODEM) */
#endif
/* END OF FILE */
