/* Copyright Statement:
*
* (C) 2005-2018  MediaTek Inc. All rights reserved.
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


/*---------------------------------------------------------------------------
 | Filename:
 | ---------
 |   gki_trace.h
 |
 | Project:
 | --------
 |   2625
 |
 | Description:
 | ------------
 |   GKI log extern customer
 |
 | Author:
 | -------
 |
 |
 |---------------------------------------------------------------------------
 | $Log$
 |
 ---------------------------------------------------------------------------*/


#ifndef __GKI_TRACE_H__
#define __GKI_TRACE_H__


/* user define, the mod define sequence should match with  related sig define in kisig.h */

/* (module id, max trace num, trace define header file path) */

#define GKI_TRACE_MOD_DEFINE \
        GKI_TRACE_MOD_IDS(CUST,              500,     "middleware\MTK\nbiot_custom\gkiprintf\cust_gki_trace.h"                  )  \
        GKI_TRACE_MOD_IDS(ATCI,              500,     "middleware\MTK\nbiot_custom\l4\atci.mod\inc\atci_gki_trace.h"            )


#define MASK_GKITRACE
/**************************************internal define*******************************/
/*
         flogID:         |31----16|15---8|8---0|
                              | reserved |   filter  |    ID   |
 */
#define FILTER_OFFSET                 8
#define GKITRACE_DEBUG                0x00
#define GKITRACE_INFO                 0x01
#define GKITRACE_WARNING              0x02
#define GKITRACE_ERROR                0x03
#define GKITRACE_NONE                 0x04
#define GKITRACE_COMPT_FLAG           0x8000

#define DEFAULT_TRACE_LEVEL           GKITRACE_INFO
#define GKITRACE_FILTER_CHECK(ID)     (((ID) >> FILTER_OFFSET & 0x7F) >=  DEFAULT_TRACE_LEVEL)

#define GKI_TRACE_MOD_IDS(MODULE, NUMBER, PATH)         GKITRACE_##MODULE##_MOD,
#define GKITRACE_MOD_LIST                              enum{GKI_TRACE_MOD_DEFINE};
GKITRACE_MOD_LIST
#undef  GKI_TRACE_MOD_IDS


#define GKI_TRACE_MOD_IDS(MODULE, NUMBER, PATH)           GKI_##MODULE##_DEBUG   = GKITRACE_##MODULE##_MOD | (GKITRACE_DEBUG   << FILTER_OFFSET), \
                                                          GKI_##MODULE##_INFO    = GKITRACE_##MODULE##_MOD | (GKITRACE_INFO    << FILTER_OFFSET), \
                                                          GKI_##MODULE##_WARNING = GKITRACE_##MODULE##_MOD | (GKITRACE_WARNING << FILTER_OFFSET), \
                                                          GKI_##MODULE##_ERROR   = GKITRACE_##MODULE##_MOD | (GKITRACE_ERROR   << FILTER_OFFSET), \
                                                    COMPT_GKI_##MODULE##_DEBUG   = GKITRACE_##MODULE##_MOD | (GKITRACE_DEBUG   << FILTER_OFFSET) | GKITRACE_COMPT_FLAG, \
                                                    COMPT_GKI_##MODULE##_INFO    = GKITRACE_##MODULE##_MOD | (GKITRACE_INFO    << FILTER_OFFSET) | GKITRACE_COMPT_FLAG, \
                                                    COMPT_GKI_##MODULE##_WARNING = GKITRACE_##MODULE##_MOD | (GKITRACE_WARNING << FILTER_OFFSET) | GKITRACE_COMPT_FLAG, \
                                                    COMPT_GKI_##MODULE##_ERROR   = GKITRACE_##MODULE##_MOD | (GKITRACE_ERROR   << FILTER_OFFSET) | GKITRACE_COMPT_FLAG,

#define GKITRACE_MOD_TAG                           enum{GKI_TRACE_MOD_DEFINE};
GKITRACE_MOD_TAG
#undef  GKI_TRACE_MOD_IDS

#define GKI_TRACE_MOD_IDS(MODULE, NUMBER, PATH)     \
    MODULE##_START,                                 \
    MODULE##_END = MODULE##_START + NUMBER - 1,

/* GKI trace ID base, reserved for tools*/
#define GKI_TRACE_IDS_BASE            0x0011

#define GKI_TRACE_MOD_ENUM                          \
    enum{                                           \
    GKI_TRACE_MOD_START = GKI_TRACE_IDS_BASE,       \
    GKI_TRACE_MOD_DEFINE};

GKI_TRACE_MOD_ENUM


#define GKI_TRACE_DEF(TRACE_ID, STRING)             \
    TRACE_ID,

#define GKI_BEGIN_IDS(MODULE)                       \
    enum MODULE##_ENUM{                             \
    MODULE##_begin=MODULE##_START,

#define GKI_END_IDS(MODULE)                         \
    MODULE##_end};

#undef  GKI_TRACE_MOD_IDS


#if defined(MASK_GKITRACE) || !defined(MTK_NBIOT_TARGET_BUILD)

#define GKI_TRACE0(...)
#define GKI_TRACE1(...)
#define GKI_TRACE2(...)
#define GKI_TRACE3(...)
#define GKI_TRACE4(...)
#define GKI_TRACE5(...)
#define GKI_TRACE6(...)
#define GKI_TRACE7(...)
#define GKI_TRACE8(...)
#define GKI_TRACE9(...)
#define GKI_TRACE10(...)
#define GKI_STRING(...)

#else

# define GKI_TRACE0(hDR, mDFlOGiD)  \
do{if(GKITRACE_FILTER_CHECK(mDFlOGiD)) FrGkiPrintf(0, hDR, mDFlOGiD);}while(0)

# define GKI_TRACE1(hDR, mDFlOGiD, aRG1)  \
do{if(GKITRACE_FILTER_CHECK(mDFlOGiD)) FrGkiPrintf(1, hDR, mDFlOGiD, aRG1);}while(0)

# define GKI_TRACE2(hDR, mDFlOGiD, aRG1, aRG2)  \
do{if(GKITRACE_FILTER_CHECK(mDFlOGiD)) FrGkiPrintf(2, hDR, mDFlOGiD, aRG1, aRG2);}while(0)

# define GKI_TRACE3(hDR, mDFlOGiD, aRG1, aRG2, aRG3)  \
do{if(GKITRACE_FILTER_CHECK(mDFlOGiD)) FrGkiPrintf(3, hDR, mDFlOGiD, aRG1, aRG2, aRG3);}while(0)

# define GKI_TRACE4(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4)  \
do{if(GKITRACE_FILTER_CHECK(mDFlOGiD)) FrGkiPrintf(4, hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4);}while(0)

# define GKI_TRACE5(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5)  \
do{if(GKITRACE_FILTER_CHECK(mDFlOGiD)) FrGkiPrintf(5, hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5);}while(0)

# define GKI_TRACE6(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6)  \
do{if(GKITRACE_FILTER_CHECK(mDFlOGiD)) FrGkiPrintf(6, hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6);}while(0)

# define GKI_TRACE7(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7)  \
do{if(GKITRACE_FILTER_CHECK(mDFlOGiD)) FrGkiPrintf(7, hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7);}while(0)

# define GKI_TRACE8(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8)  \
do{if(GKITRACE_FILTER_CHECK(mDFlOGiD)) FrGkiPrintf(8, hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8);}while(0)

# define GKI_TRACE9(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9)  \
do{if(GKITRACE_FILTER_CHECK(mDFlOGiD)) FrGkiPrintf(9, hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9);}while(0)

# define GKI_TRACE10(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10)  \
do{if(GKITRACE_FILTER_CHECK(mDFlOGiD)) FrGkiPrintf(10, hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10);}while(0)


# define GKI_STRING(hDR, mDFlOGiD, sTRINGpTR)  \
do{if(GKITRACE_FILTER_CHECK(mDFlOGiD)) FrGkiString(hDR, mDFlOGiD, sTRINGpTR);}while(0)


#endif
extern void FrGkiPrintf(uint16_t num, uint16_t header, uint32_t flogId, ...);
extern void FrGkiString (uint16_t header, uint32_t flogId, uint8_t* s);

#endif

