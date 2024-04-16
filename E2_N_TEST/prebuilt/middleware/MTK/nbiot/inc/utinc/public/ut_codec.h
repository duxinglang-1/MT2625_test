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
 ***************************************************************************
 * File Description: Type definitions for codec utility functions.
 **************************************************************************/

#ifndef UT_CODEC_H
#define UT_CODEC_H

#include <system.h>
#include <mnl3_typ.h>

/**************************************************************************
 * Nested Include Files
 **************************************************************************/

/**************************************************************************
 * Manifest Constants
 **************************************************************************/

/**************************************************************************
 * Type Definitions
 **************************************************************************/

/**
 *  Define all the available voice coding type
 */
typedef enum UtVoiceCodingTypeTag
{
    /** Prefered type: FR       Other supported types: None             */
    UT_VCT_FR                        = 0,
    /** Prefered type: EFR      Other supported types: FR               */
    UT_VCT_EFR_FR                    = 1,
    /** Prefered type: HR       Other supported types: FR               */
    UT_VCT_HR_FR                     = 2,
    /** Prefered type: FR       Other supported types: HR               */
    UT_VCT_FR_HR                     = 3,
    /** Prefered type: HR       Other supported types: EFR              */
    UT_VCT_HR_EFR                    = 4,
    /** Prefered type: EFR      Other supported types: HR               */
    UT_VCT_EFR_HR                    = 5,
    /** Prefered type: AMR-FR   Other supported types: EFR,AMR-HR       */
    UT_VCT_AMRFR_EFR_AMRHR           = 6,
    /** Prefered type: AMR-FR   Other supported types: EFR,AMR-HR,HR    */
    UT_VCT_AMRFR_EFR_AMRHR_HR        = 7,
    /** Prefered type: AMR-HR   Other supported types: HR,AMR-FR,EFR    */
    UT_VCT_AMRHR_HR_AMRFR_EFR        = 8,
    /** Prefered type: AMR-HR   Other supported types: AMR-FR,EFR       */
    UT_VCT_AMRHR_AMRFR_EFR           = 9,
    /** Prefered type: AMR-HR   Other supported types: AMR-FR,FR        */
    UT_VCT_AMRHR_AMRFR_FR            = 10,
    /** Prefered type: AMR-HR   Other supported types: HR, AMR-FR       */
    UT_VCT_AMRHR_HR_AMRFR            = 11,
    /** Prefered type: AMR-FR	Other supported types: AMR-HR           */
    UT_VCT_AMRFR_AMRHR               = 12,
    /** Prefered type: AMR-FR   Other supported types: FR,AMR-HR        */
    UT_VCT_AMRFR_FR_AMRHR            = 13,
    /** Prefered type: AMR-FR   Other supported types: FR,AMR-HR,HR     */
    UT_VCT_AMRFR_FR_AMRHR_HR         = 14,
    /**	Prefered type: AMR-FR	Other supported types: FR,AMR-HR,HR,EFR */
    UT_VCT_AMRFR_FR_AMRHR_HR_EFR     = 15,
    /** Must be the last element in the enumeration*/
    UT_VCT_FIRST_INVALID
}UtVoiceCodingType;

/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Function Prototypes
 **************************************************************************/

#if defined (UPGRADE_REL5)
void utDecodeVctSupportedCodec( UtVoiceCodingType vct, 
                                SupportedCodec *codecs_p);
#endif

void utDecodeVctSpeechVersion(  UtVoiceCodingType   vct, 
                                SpeechVersionInd   *speech_p,
                                ChannelRequirement *requir_p);

#endif /* !defined (UT_CODEC_H) */

/* END OF FILE */


