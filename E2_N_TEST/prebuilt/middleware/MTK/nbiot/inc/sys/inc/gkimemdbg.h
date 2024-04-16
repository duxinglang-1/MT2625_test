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
#ifndef  GKIMEMDBG_H
# define GKIMEMDBG_H


typedef enum GxKiMemDbgAllocTypeTag
{
    GX_KMD_MEM_ALLOC,
    GX_KMD_MEM_ALLOC_ZERO,
    GX_KMD_MEM_REQUEST,
    GX_KMD_MEM_REALLOC,
    GX_KMD_MEM_POLL,
    GX_KMD_MEM_SIGNAL_ALLOC,
    GX_KMD_MEM_SIGNAL_REQUEST,
    GX_KMD_MEM_BMM_INC,
    GX_KMD_NUM_ALLOC_TYPES

} GxKiMemDbgAllocType;

typedef enum GxKiMemDbgFreeTypeTag
{
    GX_KMD_MEM_FREE,
    GX_KMD_MEM_SIGNAL_DESTROY,
    GX_KMD_MEM_BMM_DEC,
    GX_KMD_NUM_FREE_TYPES

} GxKiMemDbgFreeType;

#if defined (KI_MEMORY_DEBUG)

#if defined (ON_PC) || defined (GX2)
# define M_GxKiMemDbgLogStart() \
                GxKiMemDbgLogStart()
# define M_GxKiMemDbgMemAlloc(aLLOCtYPE, nUMbYTES, bUFFER, fILE, lINE) \
                GxKiMemDbgMemAlloc(aLLOCtYPE, nUMbYTES, bUFFER, fILE, lINE)
# define M_GxKiMemDbgMemFree(fREEtYPE, bUFFER, fILE, lINE)         \
                GxKiMemDbgMemFree(fREEtYPE, bUFFER, fILE, lINE)
# define M_GxKiMemDbgReallocCheck(rEQsIZE, bUFsIZE)      \
                GxKiMemDbgReallocCheck(rEQsIZE, bUFsIZE)

void GxKiMemDbgLogStart(void);
void GxKiMemDbgMemAlloc(GxKiMemDbgAllocType allocType, Int32 numBytes, void *buffer, const char *file, int line);
void GxKiMemDbgMemFree(GxKiMemDbgFreeType freeType, void *buffer, const char *file, int line);
void GxKiMemDbgReallocCheck(Int32 requiredSize, Int32 bufferSize);

# define KI_MEM_DBG_PC_EXTRA_ARGS_DEF       , const char *file, int line
# define KI_MEM_DBG_PC_EXTRA_ARGS_USE       , MODULE_NAME, __LINE__
# define KI_MEM_DBG_PC_EXTRA_ARGS_PASS      , file, line

#else 
# define M_GxKiMemDbgLogStart()
# define M_GxKiMemDbgMemAlloc(aLLOCtYPE, nUMbYTES, bUFFER, fILE, lINE)
# define M_GxKiMemDbgMemFree(fREEtYPE, bUFFER, fILE, lINE)
# define M_GxKiMemDbgReallocCheck(rEQsIZE, bUFsIZE)

# define KI_MEM_DBG_PC_EXTRA_ARGS_DEF
# define KI_MEM_DBG_PC_EXTRA_ARGS_USE
# define KI_MEM_DBG_PC_EXTRA_ARGS_PASS

#endif 

# define GX_MEM_DBG_EXTRA_PARAMS     , const char *file, int line
# define GX_MEM_DBG_EXTRA_ARGS       , file, line

# define KI_MEM_DBG_EXTRA_ARGS_DEF       , const char *file, int line
# define KI_MEM_DBG_EXTRA_ARGS_USE       , MODULE_NAME, __LINE__
# define KI_MEM_DBG_EXTRA_ARGS_PASS      , file, line

#else

# define GX_MEM_DBG_EXTRA_PARAMS
# define GX_MEM_DBG_EXTRA_ARGS

# define M_GxKiMemDbgLogStart()
# define M_GxKiMemDbgMemAlloc(aLLOCtYPE, nUMbYTES, bUFFER, fILE, lINE)
# define M_GxKiMemDbgMemFree(fREEtYPE, bUFFER, fILE, lINE)
# define M_GxKiMemDbgReallocCheck(rEQsIZE, bUFsIZE)

# define KI_MEM_DBG_EXTRA_ARGS_DEF
# define KI_MEM_DBG_EXTRA_ARGS_USE
# define KI_MEM_DBG_EXTRA_ARGS_PASS

# define KI_MEM_DBG_PC_EXTRA_ARGS_DEF
# define KI_MEM_DBG_PC_EXTRA_ARGS_USE
# define KI_MEM_DBG_PC_EXTRA_ARGS_PASS

#endif

#endif /* !GKIMEMDBG_H */
/* END OF FILE */
