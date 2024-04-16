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

/*---------------------------------------------------------------------------
 | Filename:
 | ---------
 |   frgkicfg.h
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

#ifndef __FRGKICFG_H__
#define __FRGKICFG_H__

#if defined (FR_HSL_2_GKI)
#include <ki_typ.h>

/*******************************************************************************
** Macros
*******************************************************************************/
#ifdef OPTIMIZED_GKI_LOG_ENABLE
typedef enum KiLogChannelTag
{
    KI_LOG_CMD_CHANNEL,
#ifdef MTK_NBIOT_ICD_ENABLE
    KI_LOG_ICD_CHANNEL,
#endif
    KI_LOG_GKI_MD_CHANNEL,
    KI_LOG_GKI_AP_CHANNEL,
    //KI_LOG_HSL_CHANNEL, /* reserved for HSL */
    KI_LOG_IDLE_CHANNEL,
    KI_LOG_MAX_CHANNEL = KI_LOG_IDLE_CHANNEL,
} KiLogChannel_t;

typedef enum
{
    KI_LOG_FLUSH_CMD,
#ifdef MTK_NBIOT_ICD_ENABLE
    KI_LOG_FLUSH_ICD,
#endif
    KI_LOG_FLUSH_MD,
    KI_LOG_FLUSH_ALL,
    KI_LOG_FLUSH_IN_CB,
} ki_log_flush_t;
#endif /* OPTIMIZED_GKI_LOG_ENABLE */

/** Define header msg
*/
#define GKI2HSL_END    0xF0
#define GKI2HSL_BODY   0xB0
#define GKI2HSL_START  0x70
#define GKI2HSL_SINGLE 0x30

#define GKI2HSL_WRITE(pATERN,dATA)                   \
    {                                                \
        *datap++ = pATERN;                           \
        *datap++=  dATA&0xFF;                        \
        *datap++= (dATA&0xFF00)>>8;                  \
        *datap++= (dATA&0xFF0000)>>16;               \
        *datap++= (dATA&0xFF000000)>>24;             \
    }

#define GKI2HSL_WRITE_HEADER(pATERN,dATA)            \
    {                                                \
        *datap++ = pATERN;                           \
        *datap++ = 0;                                \
        *datap++ = 0;                                \
        *datap++=  dATA&0xFF;                        \
        *datap++= (dATA&0xFF00)>>8;                  \
        *datap++= (dATA&0xFF0000)>>16;               \
        *datap++= (dATA&0xFF000000)>>24;             \
    }

/** Writes to the GKI SWMsg Last Word register.
*  Writes the last word of a multi-word message to this register.
*/

# define M_FrGkiWriteSwMsgLastWord(vALUE, vALUE2)   GKI2HSL_WRITE(GKI2HSL_END, (((Int32)((vALUE2)<<16))|(vALUE)))

/** Writes to the GKI SWMsg Body register.
*  Writes the middle words (but not the first or last) of a multi-word message to
*  this register.
*/
#  define M_FrGkiWriteSwMsgBody(vALUE, vALUE2)      GKI2HSL_WRITE(GKI2HSL_BODY, (((Int32)(vALUE2<<16))|(vALUE)) )

/** Writes to the GKI SWMsg Header register.
*  Writes the first word of a multi-word message to this register.
*/
# define M_FrGkiWriteSwMsgHeader(vALUE, len)        GKI2HSL_WRITE_HEADER(GKI2HSL_START, (((Int32)(vALUE<<16))|(len)))

/** Writes to the GKI SWMsg Single register.
*  Writes single-word logpoints to this register.  Note that the LG_SINGLE_WORD
*  bit (0x2000) must be set in the header value written.  This is essential
*  to allow the message nesting scheme to operate correctly.
*/
# define M_FrGkiWriteSwMsgSingle(vALUE)             GKI2HSL_WRITE_HEADER(GKI2HSL_SINGLE, ((Int32)(vALUE)))

# define  TIMESTAMP_LENGTH                          2
# define  M_GetRawLen(aRGNum)                      (TIMESTAMP_LENGTH + 5 * ((3 + aRGNum) >> 1))
# define  M_GetDataLen(aRGNum)                     (M_GetRawLen(aRGNum) + 2)

#   define M_FrGki0ByteMsg(hDR)                     \
    {                                               \
        M_FrGkiWriteSwMsgSingle(hDR);               \
    }

#   define M_FrGki1ByteMsg(hDR, aRG1)               \
    {                                               \
        M_FrGkiWriteSwMsgHeader(hDR, 1);            \
        M_FrGkiWriteSwMsgLastWord(aRG1, 0);         \
    }
#   define M_FrGki2ByteMsg(hDR, aRG1, aRG2)         \
    {                                               \
        M_FrGkiWriteSwMsgHeader(hDR, 2);            \
        M_FrGkiWriteSwMsgLastWord(aRG1, aRG2);      \
    }
#   define M_FrGki3ByteMsg(hDR, aRG1, aRG2, aRG3)   \
    {                                               \
        M_FrGkiWriteSwMsgHeader(hDR, 3);            \
        M_FrGkiWriteSwMsgBody(aRG1, aRG2);          \
        M_FrGkiWriteSwMsgLastWord(aRG3, 0);         \
    }

#   define M_FrGki4ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4)    \
    {                                                      \
        M_FrGkiWriteSwMsgHeader(hDR, 4);                   \
        M_FrGkiWriteSwMsgBody(aRG1, aRG2);                 \
        M_FrGkiWriteSwMsgLastWord(aRG3, aRG4);             \
    }

#   define M_FrGki5ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4, aRG5)    \
    {                                                            \
        M_FrGkiWriteSwMsgHeader(hDR, 5);                         \
        M_FrGkiWriteSwMsgBody(aRG1, aRG2);                       \
        M_FrGkiWriteSwMsgBody(aRG3, aRG4);                       \
        M_FrGkiWriteSwMsgLastWord(aRG5, 0);                      \
    }

#   define M_FrGki6ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6)        \
    {                                                                      \
        M_FrGkiWriteSwMsgHeader(hDR, 6);                                   \
        M_FrGkiWriteSwMsgBody(aRG1, aRG2);                                 \
        M_FrGkiWriteSwMsgBody(aRG3, aRG4);                                 \
        M_FrGkiWriteSwMsgLastWord(aRG5, aRG6);                             \
    }

#   define M_FrGki7ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7)    \
    {                                                                        \
        M_FrGkiWriteSwMsgHeader(hDR, 7);                                     \
        M_FrGkiWriteSwMsgBody(aRG1, aRG2);                                   \
        M_FrGkiWriteSwMsgBody(aRG3, aRG4);                                   \
        M_FrGkiWriteSwMsgBody(aRG5, aRG6);                                   \
        M_FrGkiWriteSwMsgLastWord(aRG7, 0);                                  \
    }


#   define M_FrGki8ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8)    \
    {                                                                              \
        M_FrGkiWriteSwMsgHeader(hDR, 8);                                           \
        M_FrGkiWriteSwMsgBody(aRG1, aRG2);                                         \
        M_FrGkiWriteSwMsgBody(aRG3, aRG4);                                         \
        M_FrGkiWriteSwMsgBody(aRG5, aRG6);                                         \
        M_FrGkiWriteSwMsgLastWord(aRG7, aRG8);                                     \
    }


#   define M_FrGki9ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9)    \
    {                                                                                    \
        M_FrGkiWriteSwMsgHeader(hDR, 9);                                                 \
        M_FrGkiWriteSwMsgBody(aRG1, aRG2);                                               \
        M_FrGkiWriteSwMsgBody(aRG3, aRG4);                                               \
        M_FrGkiWriteSwMsgBody(aRG5, aRG6);                                               \
        M_FrGkiWriteSwMsgBody(aRG7, aRG8);                                               \
        M_FrGkiWriteSwMsgLastWord(aRG9, 0);                                              \
    }

#   define M_FrGki10ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10)    \
    {                                                                                            \
        M_FrGkiWriteSwMsgHeader(hDR, 10);                                                        \
        M_FrGkiWriteSwMsgBody(aRG1, aRG2);                                                       \
        M_FrGkiWriteSwMsgBody(aRG3, aRG4);                                                       \
        M_FrGkiWriteSwMsgBody(aRG5, aRG6);                                                       \
        M_FrGkiWriteSwMsgBody(aRG7, aRG8);                                                       \
        M_FrGkiWriteSwMsgLastWord(aRG9, aRG10);                                                  \
    }

#   define M_FrGki11ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10, aRG11)        \
    {                                                                                                       \
        M_FrGkiWriteSwMsgHeader(hDR, 11);                                                                   \
        M_FrGkiWriteSwMsgBody(aRG1, aRG2);                                                                  \
        M_FrGkiWriteSwMsgBody(aRG3, aRG4);                                                                  \
        M_FrGkiWriteSwMsgBody(aRG5, aRG6);                                                                  \
        M_FrGkiWriteSwMsgBody(aRG7, aRG8);                                                                  \
        M_FrGkiWriteSwMsgBody(aRG9, aRG10);                                                                 \
        M_FrGkiWriteSwMsgLastWord(aRG11, 0);                                                                \
    }

#   define M_FrGki12ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10, aRG11, aRG12)    \
    {                                                                                                          \
        M_FrGkiWriteSwMsgHeader(hDR, 12);                                                                      \
        M_FrGkiWriteSwMsgBody(aRG1, aRG2);                                                                     \
        M_FrGkiWriteSwMsgBody(aRG3, aRG4);                                                                     \
        M_FrGkiWriteSwMsgBody(aRG5, aRG6);                                                                     \
        M_FrGkiWriteSwMsgBody(aRG7, aRG8);                                                                     \
        M_FrGkiWriteSwMsgBody(aRG9, aRG10);                                                                    \
        M_FrGkiWriteSwMsgLastWord(aRG11, aRG12);                                                               \
    }

#   define M_FrGki13ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10, aRG11, aRG12, aRG13)    \
    {                                                                                                                 \
        M_FrGkiWriteSwMsgHeader(hDR, 13);                                                                             \
        M_FrGkiWriteSwMsgBody(aRG1, aRG2);                                                                            \
        M_FrGkiWriteSwMsgBody(aRG3, aRG4);                                                                            \
        M_FrGkiWriteSwMsgBody(aRG5, aRG6);                                                                            \
        M_FrGkiWriteSwMsgBody(aRG7, aRG8);                                                                            \
        M_FrGkiWriteSwMsgBody(aRG9, aRG10);                                                                           \
        M_FrGkiWriteSwMsgBody(aRG11, aRG12);                                                                          \
        M_FrGkiWriteSwMsgLastWord(aRG13, 0);                                                                          \
    }

#   define M_FrGki14ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10, aRG11, aRG12, aRG13, aRG14)    \
    {                                                                                                                        \
        M_FrGkiWriteSwMsgHeader(hDR, 14);                                                                                    \
        M_FrGkiWriteSwMsgBody(aRG1, aRG2);                                                                                   \
        M_FrGkiWriteSwMsgBody(aRG3, aRG4);                                                                                   \
        M_FrGkiWriteSwMsgBody(aRG5, aRG6);                                                                                   \
        M_FrGkiWriteSwMsgBody(aRG7, aRG8);                                                                                   \
        M_FrGkiWriteSwMsgBody(aRG9, aRG10);                                                                                  \
        M_FrGkiWriteSwMsgBody(aRG11, aRG12);                                                                                 \
        M_FrGkiWriteSwMsgLastWord(aRG13, aRG14);                                                                             \
    }

#   define M_FrGki15ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15)    \
    {                                                                                                                               \
        M_FrGkiWriteSwMsgHeader(hDR, 15);                                                                                           \
        M_FrGkiWriteSwMsgBody(aRG1, aRG2);                                                                                          \
        M_FrGkiWriteSwMsgBody(aRG3, aRG4);                                                                                          \
        M_FrGkiWriteSwMsgBody(aRG5, aRG6);                                                                                          \
        M_FrGkiWriteSwMsgBody(aRG7, aRG8);                                                                                          \
        M_FrGkiWriteSwMsgBody(aRG9, aRG10);                                                                                         \
        M_FrGkiWriteSwMsgBody(aRG11, aRG12);                                                                                        \
        M_FrGkiWriteSwMsgBody(aRG13, aRG14);                                                                                        \
        M_FrGkiWriteSwMsgLastWord(aRG15, 0);                                                                                        \
    }

#   define M_FrGki16ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15, aRG16)    \
    {                                                                                                                                      \
        M_FrGkiWriteSwMsgHeader(hDR, 16);                                                                                                  \
        M_FrGkiWriteSwMsgBody(aRG1, aRG2);                                                                                                 \
        M_FrGkiWriteSwMsgBody(aRG3, aRG4);                                                                                                 \
        M_FrGkiWriteSwMsgBody(aRG5, aRG6);                                                                                                 \
        M_FrGkiWriteSwMsgBody(aRG7, aRG8);                                                                                                 \
        M_FrGkiWriteSwMsgBody(aRG9, aRG10);                                                                                                \
        M_FrGkiWriteSwMsgBody(aRG11, aRG12);                                                                                               \
        M_FrGkiWriteSwMsgBody(aRG13, aRG14);                                                                                               \
        M_FrGkiWriteSwMsgLastWord(aRG15, aRG16);                                                                                           \
    }

# define M_FrGkiPrintf0(hDR,mDFlOGiD,fORMATsPECIFER)                                                              \
    {                                                                                                             \
        uint8_t* datap = kiHslPrintf_##mDFlOGiD##_Ind.rawData;                                                    \
        kiHslPrintf_##mDFlOGiD##_Ind.length = M_GetRawLen(0);                                                     \
        M_FrGki0ByteMsg(hDR)                                                                                      \
        KiLogSignalStruct(SIG_KI_HSL_PRINTF_##mDFlOGiD##_IND, M_GetDataLen(0),&kiHslPrintf_##mDFlOGiD##_Ind);     \
    }

# define M_FrGkiPrintf1(hDR,mDFlOGiD,fORMATsPECIFER, aRG1)                                                        \
    {                                                                                                             \
        uint8_t* datap = kiHslPrintf_##mDFlOGiD##_Ind.rawData;                                                    \
        kiHslPrintf_##mDFlOGiD##_Ind.length = M_GetRawLen(1);                                                     \
        M_FrGki1ByteMsg(hDR, aRG1)                                                                                \
        KiLogSignalStruct(SIG_KI_HSL_PRINTF_##mDFlOGiD##_IND, M_GetDataLen(1), &kiHslPrintf_##mDFlOGiD##_Ind);    \
    }

# define M_FrGkiPrintf2(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2)                                                  \
    {                                                                                                             \
        uint8_t* datap = kiHslPrintf_##mDFlOGiD##_Ind.rawData;                                                    \
        kiHslPrintf_##mDFlOGiD##_Ind.length = M_GetRawLen(2);                                                     \
        M_FrGki2ByteMsg(hDR, aRG1, aRG2)                                                                          \
        KiLogSignalStruct(SIG_KI_HSL_PRINTF_##mDFlOGiD##_IND, M_GetDataLen(2), &kiHslPrintf_##mDFlOGiD##_Ind);    \
    }

# define M_FrGkiPrintf3(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3)                                            \
    {                                                                                                             \
        uint8_t* datap = kiHslPrintf_##mDFlOGiD##_Ind.rawData;                                                    \
        kiHslPrintf_##mDFlOGiD##_Ind.length = M_GetRawLen(3);                                                     \
        M_FrGki3ByteMsg(hDR, aRG1,aRG2, aRG3)                                                                     \
        KiLogSignalStruct(SIG_KI_HSL_PRINTF_##mDFlOGiD##_IND, M_GetDataLen(3), &kiHslPrintf_##mDFlOGiD##_Ind);    \
    }

# define M_FrGkiPrintf4(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4)                                      \
    {                                                                                                             \
        uint8_t* datap = kiHslPrintf_##mDFlOGiD##_Ind.rawData;                                                    \
        kiHslPrintf_##mDFlOGiD##_Ind.length = M_GetRawLen(4);                                                     \
        M_FrGki4ByteMsg(hDR, aRG1,aRG2, aRG3, aRG4)                                                               \
        KiLogSignalStruct(SIG_KI_HSL_PRINTF_##mDFlOGiD##_IND, M_GetDataLen(4), &kiHslPrintf_##mDFlOGiD##_Ind);    \
    }
# define M_FrGkiPrintf5(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5)                                \
    {                                                                                                             \
        uint8_t* datap = kiHslPrintf_##mDFlOGiD##_Ind.rawData;                                                    \
        kiHslPrintf_##mDFlOGiD##_Ind.length = M_GetRawLen(5);                                                     \
        M_FrGki5ByteMsg(hDR, aRG1,aRG2, aRG3, aRG4, aRG5)                                                         \
        KiLogSignalStruct(SIG_KI_HSL_PRINTF_##mDFlOGiD##_IND, M_GetDataLen(5), &kiHslPrintf_##mDFlOGiD##_Ind);    \
    }

# define M_FrGkiPrintf6(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6)                          \
    {                                                                                                             \
        uint8_t* datap = kiHslPrintf_##mDFlOGiD##_Ind.rawData;                                                    \
        kiHslPrintf_##mDFlOGiD##_Ind.length = M_GetRawLen(6);                                                     \
        M_FrGki6ByteMsg(hDR, aRG1,aRG2, aRG3, aRG4, aRG5, aRG6)                                                   \
        KiLogSignalStruct(SIG_KI_HSL_PRINTF_##mDFlOGiD##_IND, M_GetRawLen(6), &kiHslPrintf_##mDFlOGiD##_Ind);     \
    }

# define M_FrGkiPrintf7(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7)                    \
    {                                                                                                             \
        uint8_t* datap = kiHslPrintf_##mDFlOGiD##_Ind.rawData;                                                    \
        kiHslPrintf_##mDFlOGiD##_Ind.length = M_GetRawLen(7);                                                     \
        M_FrGki7ByteMsg(hDR, aRG1,aRG2, aRG3, aRG4, aRG5, aRG6, aRG7)                                             \
        KiLogSignalStruct(SIG_KI_HSL_PRINTF_##mDFlOGiD##_IND, M_GetRawLen(7), &kiHslPrintf_##mDFlOGiD##_Ind);     \
    }

# define M_FrGkiPrintf8(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8)              \
    {                                                                                                             \
        uint8_t* datap = kiHslPrintf_##mDFlOGiD##_Ind.rawData;                                                    \
        kiHslPrintf_##mDFlOGiD##_Ind.length = M_GetRawLen(8);                                                     \
        M_FrGki8ByteMsg(hDR, aRG1,aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8)                                       \
        KiLogSignalStruct(SIG_KI_HSL_PRINTF_##mDFlOGiD##_IND, M_GetRawLen(8), &kiHslPrintf_##mDFlOGiD##_Ind);     \
    }
# define M_FrGkiPrintf9(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9)        \
    {                                                                                                             \
        uint8_t* datap = kiHslPrintf_##mDFlOGiD##_Ind.rawData;                                                    \
        kiHslPrintf_##mDFlOGiD##_Ind.length = M_GetRawLen(9);                                                     \
        M_FrGki9ByteMsg(hDR, aRG1,aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9)                                 \
        KiLogSignalStruct(SIG_KI_HSL_PRINTF_##mDFlOGiD##_IND, M_GetDataLen(9), &kiHslPrintf_##mDFlOGiD##_Ind);    \
    }

# define M_FrGkiPrintf10(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10)                                     \
    {                                                                                                                                                  \
        uint8_t* datap = kiHslPrintf_##mDFlOGiD##_Ind.rawData;                                                                                         \
        kiHslPrintf_##mDFlOGiD##_Ind.length = M_GetRawLen(10);                                                                                         \
        M_FrGki10ByteMsg(hDR, aRG1,aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10)                                                              \
        KiLogSignalStruct(SIG_KI_HSL_PRINTF_##mDFlOGiD##_IND, M_GetRawLen(10), &kiHslPrintf_##mDFlOGiD##_Ind);                                         \
    }
# define M_FrGkiPrintf11(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10,aRG11)                               \
    {                                                                                                                                                  \
        uint8_t* datap = kiHslPrintf_##mDFlOGiD##_Ind.rawData;                                                                                         \
        kiHslPrintf_##mDFlOGiD##_Ind.length = M_GetRawLen(11);                                                                                         \
        M_FrGki11ByteMsg(hDR, aRG1,aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10,aRG11)                                                        \
        KiLogSignalStruct(SIG_KI_HSL_PRINTF_##mDFlOGiD##_IND, M_GetDataLen(11), &kiHslPrintf_##mDFlOGiD##_Ind);                                        \
    }

# define M_FrGkiPrintf12(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10,aRG11,aRG12)                         \
    {                                                                                                                                                  \
        uint8_t* datap = kiHslPrintf_##mDFlOGiD##_Ind.rawData;                                                                                         \
        kiHslPrintf_##mDFlOGiD##_Ind.length = M_GetRawLen(12);                                                                                         \
        M_FrGki12ByteMsg(hDR, aRG1,aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10,aRG11,aRG12)                                                  \
        KiLogSignalStruct(SIG_KI_HSL_PRINTF_##mDFlOGiD##_IND, M_GetRawLen(12), &kiHslPrintf_##mDFlOGiD##_Ind);                                         \
    }

# define M_FrGkiPrintf13(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10,aRG11,aRG12,aRG13)                   \
    {                                                                                                                                                  \
        uint8_t* datap = kiHslPrintf_##mDFlOGiD##_Ind.rawData;                                                                                         \
        kiHslPrintf_##mDFlOGiD##_Ind.length = M_GetRawLen(13);                                                                                         \
        M_FrGki13ByteMsg(hDR, aRG1,aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10,aRG11,aRG12,aRG13)                                            \
        KiLogSignalStruct(SIG_KI_HSL_PRINTF_##mDFlOGiD##_IND, M_GetDataLen(13), &kiHslPrintf_##mDFlOGiD##_Ind);                                        \
    }
# define M_FrGkiPrintf14(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10,aRG11,aRG12,aRG13,aRG14)             \
    {                                                                                                                                                  \
        uint8_t* datap = kiHslPrintf_##mDFlOGiD##_Ind.rawData;                                                                                         \
        kiHslPrintf_##mDFlOGiD##_Ind.length = M_GetRawLen(14);                                                                                         \
        M_FrGki14ByteMsg(hDR, aRG1,aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10,aRG11,aRG12,aRG13,aRG14)                                      \
        KiLogSignalStruct(SIG_KI_HSL_PRINTF_##mDFlOGiD##_IND, M_GetDataLen(14), &kiHslPrintf_##mDFlOGiD##_Ind);                                        \
    }
# define M_FrGkiPrintf15(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10,aRG11,aRG12,aRG13,aRG14,aRG15)       \
    {                                                                                                                                                  \
        uint8_t* datap = kiHslPrintf_##mDFlOGiD##_Ind.rawData;                                                                                         \
        kiHslPrintf_##mDFlOGiD##_Ind.length = M_GetRawLen(15);                                                                                         \
        M_FrGki15ByteMsg(hDR, aRG1,aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10,aRG11,aRG12,aRG13,aRG14,aRG15)                                \
        KiLogSignalStruct(SIG_KI_HSL_PRINTF_##mDFlOGiD##_IND, M_GetDataLen(15), &kiHslPrintf_##mDFlOGiD##_Ind);                                        \
    }

# define M_FrGkiPrintf16(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10,aRG11,aRG12,aRG13,aRG14,aRG15,aRG16) \
    {                                                                                                                                                  \
        uint8_t* datap = kiHslPrintf_##mDFlOGiD##_Ind.rawData;                                                                                         \
        kiHslPrintf_##mDFlOGiD##_Ind.length = M_GetRawLen(16);                                                                                         \
        M_FrGki16ByteMsg(hDR, aRG1,aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10,aRG11,aRG12,aRG13,aRG14,aRG15,aRG16)                          \
        KiLogSignalStruct(SIG_KI_HSL_PRINTF_##mDFlOGiD##_IND, M_GetDataLen(16), &kiHslPrintf_##mDFlOGiD##_Ind);                                        \
    }
#else
# define M_FrGkiPrintf0(hDR, mDFlOGiD,fORMATsPECIFER)
# define M_FrGkiPrintf1(hDR, mDFlOGiD,fORMATsPECIFER, aRG1)
# define M_FrGkiPrintf2(hDR, mDFlOGiD,fORMATsPECIFER, aRG1, aRG2)
# define M_FrGkiPrintf3(hDR, mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3)
# define M_FrGkiPrintf4(hDR, mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4)
# define M_FrGkiPrintf5(hDR, mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5)
# define M_FrGkiPrintf6(hDR, mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6)
# define M_FrGkiPrintf7(hDR, mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7)
# define M_FrGkiPrintf8(hDR, mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8)
# define M_FrGkiPrintf9(hDR, mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9)
# define M_FrGkiPrintf10(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10)
# define M_FrGkiPrintf11(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10,aRG11)
# define M_FrGkiPrintf12(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10,aRG11,aRG12)
# define M_FrGkiPrintf13(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10,aRG11,aRG12,aRG13)
# define M_FrGkiPrintf14(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10,aRG11,aRG12,aRG13,aRG14)
# define M_FrGkiPrintf15(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10,aRG11,aRG12,aRG13,aRG14,aRG15)
# define M_FrGkiPrintf16(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, aRG9, aRG10,aRG11,aRG12,aRG13,aRG14,aRG15,aRG16)
#endif

/*******************************************************************************
** extern
*******************************************************************************/
#if defined (FR_HSL_2_GKI)
extern KiHslPrintfAtciInd   kiHslPrintf_ATCI_Ind;
extern KiHslPrintfLevel4Ind kiHslPrintf_LEVEL4_Ind;
#endif

#endif //__FRGKICFG_H__




