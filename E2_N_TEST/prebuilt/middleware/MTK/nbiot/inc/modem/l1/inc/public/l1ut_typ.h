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
 * File Description: Layer 1 utility signal types. These are types for signal
 *                       that are notdirectly related to modem functions but may
 *                       be needed by higher layers. This must only contain
 *                       functionality that is not mode specific.
 **************************************************************************/

#ifndef L1UT_TYP_H
#define L1UT_TYP_H

/*******************************************************************************
** Includes
*******************************************************************************/

#include <system.h>

/*******************************************************************************
** Defines
*******************************************************************************/

#define MAX_DSP_LOGGER_CONFIG_LENGTH 17

/*******************************************************************************
** Types
*******************************************************************************/

typedef enum SleepReqSourceModuleTag
{
    SIM_DRIVER_MODULE,
    VOYAGER_MODULE,
    GPRS_CIPHER_MODULE,
    BATMAN_MODULE,
    BT_DRIVER_MODULE,
    USB_DRIVER_MODULE,
    SLEEP_REQ_IRDA_MODULE,
    CME_AUDIO_MODULE,
    EMMI_MODULE,
    DMA_DRIVER_MODULE,
    /* Added it for Dedicated DFE DMA module after G2P */
    DFEDMA_DRIVER_MODULE,
    OPTIONAL_GAP_MODULE,
    U1CD_MODULE,
/*
 *  Sleep should be disabled during startup. There are 2 main reasons for this
 *  - For builds with USE_PC_SCRIPT or ENABLE_SPLIT_SYSTEM defined we need to keep serial clocks running
 *  - If we go to sleep in this phase the hardware resources we need to wake up may not have been initialised
 */
    STARTUP_MODULE,
    NUM_SLEEP_REQ_MODULES
}
SleepReqSourceModule;

#endif
/* END OF FILE */

