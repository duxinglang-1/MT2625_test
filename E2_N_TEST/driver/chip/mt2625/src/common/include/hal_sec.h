/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
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
** $Log: hal_Sec.h $
**
** 01 12 2015 leo.hung
** [DVT][Crypto]
** 1. Update crypto kery from efuse KeK and usecret.
**
** 01 08 2015 leo.hung
** [DVT][Crypto]
** 1. Add key source from efuse.
**
** 12 19 2014 leo.hung
** [DVT][Crypto]
** 1. Update Crypto Ring/Double buffer control.
**
** 12 11 2014 leo.hung
** [DVT][PWM][IRDA][RTC][Crypto][WDT]
** 1. Update PWM, IRDA, RTC Crypto_AES/DES, WDT.
**
** 12 08 2014 leo.hung
** [DVT][PWM][IRDA][RTC][Crypto]
** 1. Update PWM, IRDA, RTC Crypto_AES/DES.
**
** 11 29 2014 leo.hung
** [DVT][IrRx][Crypto][RTC]
** 1. Update IrRx, Crypto, RTC.
**
**
*/

#ifndef __HAL_SEC_H__
#define __HAL_SEC_H__

/*******************************************************************************
*                         C O M P I L E R   F L A G S
********************************************************************************
*/


/*******************************************************************************
*                    E X T E R N A L   R E F E R E N C E S
********************************************************************************
*/
//#include "mt2625_cm4_hw_memmap.h"


/*******************************************************************************
*                              C O N S T A N T S
********************************************************************************
*/

#define IOT_CRYPTO_DECRYPT  (0)
#define IOT_CRYPTO_ENCRYPT  (1)

#define IOT_CRYPTO_ECB (0)
#define IOT_CRYPTO_CBC (1)

#define IOT_CRYPTO_KEY_EFUSE_0          (0) //key_in1
#define IOT_CRYPTO_KEY_EFUSE_1          (1) //key_in2
#define IOT_CRYPTO_KEY_SW               (2)
#define IOT_CRYPTO_KEY_HW_RTL           (3)  //key_rtl
#define IOT_CRYPTO_KEY_HW_CKDF_0        (4)  //key_ckdf
#define IOT_CRYPTO_KEY_HW_CKDF_1        (5)  //key_ckdf_A
#define IOT_CRYPTO_KEY_FEEDBACK         (6)

#define IOT_CRYPTO_KEY_BANK_KEK     (1)
#define IOT_CRYPTO_KEY_BANK_USECRET (2)


/*******************************************************************************
*                             D A T A   T Y P E S
********************************************************************************
*/

typedef struct _CRYPTO_KEY {
    uint32_t u4Key1;
    uint32_t u4Key2;
    uint32_t u4Key3;
    uint32_t u4Key4;
    uint32_t u4Key5;
    uint32_t u4Key6;
    uint32_t u4Key7;
    uint32_t u4Key8;
} CRYPTO_KEY_T, *P_CRYPTO_KEY_T;

typedef struct _CRYPTO_IV {
    uint32_t u4IV1;
    uint32_t u4IV2;
    uint32_t u4IV3;
    uint32_t u4IV4;
} CRYPTO_IV_T, *P_CRYPTO_IV_T;

/*******************************************************************************
*                            P U B L I C   D A T A
********************************************************************************
*/
enum CryptoEngineID {
    CryptoMD5 = 0,
    CryptoSHA1,
    CryptoSHA224,
    CryptoSHA256,
    CryptoSHA384,
    CryptoSHA512
};

/*
========================================================================
Routine Description:

Note:
========================================================================
*/
extern void halSecSetAESDESKeyBank(uint8_t ucKeyBank);
extern void halSecCryptoKick(void);
extern void halSecDMA1Config(uint8_t ucEnable, uint16_t u2LenWPPT, uint8_t * ptrAddrWPTO);
extern void halSecDMA2Config(uint8_t ucEnable, uint16_t u2LenWPPT, uint8_t * ptrAddrWPTO);
extern void halSecAESConfig(uint8_t * ptrInputText, uint8_t * ptrOuputText, uint32_t u4DataLen, CRYPTO_KEY_T cryptoKey, uint32_t u4KeyLen, uint8_t ucKeySource, uint8_t ucMode, uint8_t ucType, CRYPTO_IV_T cryptoIV);
extern void halSecAES(uint8_t * ptrInputText, uint8_t * ptrOuputText, uint32_t u4DataLen, CRYPTO_KEY_T cryptoKey, uint32_t u4KeyLen, uint8_t ucKeySource, uint8_t ucMode, uint8_t ucType, CRYPTO_IV_T cryptoIV);
extern void halSecDESConfig(uint8_t * ptrInputText, uint8_t * ptrOuputText, uint32_t u4DataLen, CRYPTO_KEY_T cryptoKey, uint32_t u4KeyLen, uint8_t ucKeySource, uint8_t ucMode, uint8_t ucType, CRYPTO_IV_T cryptoIV);
extern void halSecDES(uint8_t * ptrInputText, uint8_t * ptrOuputText, uint32_t u4DataLen, CRYPTO_KEY_T cryptoKey, uint32_t u4KeyLen, uint8_t ucKeySource, uint8_t ucMode, uint8_t ucType, CRYPTO_IV_T cryptoIV);
extern void halSecAES_DMA_Buff(uint8_t * ptrInputText, uint8_t * ptrOuputText, uint32_t u4DataLen, uint16_t u2LenWPPT, uint8_t * ptrAddrWPTO, CRYPTO_KEY_T cryptoKey, uint32_t u4KeyLen, uint8_t ucKeySource, uint8_t ucMode, uint8_t ucType, CRYPTO_IV_T cryptoIV);
extern void halSecAES_DMA_Buff_Dest(uint8_t * ptrInputText, uint8_t * ptrOuputText, uint32_t u4DataLen, uint16_t u2LenWPPT, uint8_t * ptrAddrWPTO, CRYPTO_KEY_T cryptoKey, uint32_t u4KeyLen, uint8_t ucKeySource, uint8_t ucMode, uint8_t ucType, CRYPTO_IV_T cryptoIV);
#endif //#ifndef __HAL_SEC_H__

