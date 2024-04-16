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
 * File Description
 * ----------------
 *   Application Background Radio Policy Manager
 *   Type Definitions
 **************************************************************************/

#ifndef ABRPM_TYP_H
#define ABRPM_TYP_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/
#include <system.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/
#define RPM_VERSION_IMPLEMENTED     2
#define RPM_PARAM_DEFAULT_SETTING   {RPM_FLAG_SIM_ADAPTIVE/* RPM_Flag */, 1/* N1 */, 60/* T1, in minute */, 60/* F1 */, 30/* F2 */, 60/* F3 */, 30 /* F4 */}

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/** RPM Feature Flag
 */
typedef enum RpmFlagTag {
    RPM_FLAG_DISABLED = 0,
    RPM_FLAG_ENABLED = 1,
    RPM_FLAG_SIM_ADAPTIVE = 2 /* Enable/disable the feature according to SIM HPLMN */
} RpmFlag;

/** RPM Paramter
 */
typedef struct RpmParamTag {
    RpmFlag flag;   /* RPM enabled flag */
    Int8    n1;     /* N1, max number of SW resets per hour allowed by RPM following "permanent" EMM reject */
    Int8    t1;     /* T1, average time before RPM resets modem following "permanent" EMM reject */
    Int8    f1;     /* F1, max number of PDP activation requests per hour allowed by PRM following a PDP activation ignore scenario */
    Int8    f2;     /* F2, max number of PDP activation requests per hour allowed by RPM following a "permanent" PDP activation reject */
    Int8    f3;     /* F3, max number of PDP activation requests per hour allowed by RPM following a "temporary" PDP activation reject*/
    Int8    f4;     /* F4, max number of PDP activation/deactivation requests per hour allowed by RPM */
} RpmParam;

/** RPM Operational Management Counters
 */
typedef struct RpmOmcTag {
    Int8    c_br_1; /* C-BR-1, counter related to N1 */
    Int8    c_r_1;  /* C-R-1, counter related to T1 */
    Int8    c_pdp_1;/* C-PDP-1, counter related to F1 */
    Int8    c_pdp_2;/* C-PDP-2, counter related to F2 */
    Int8    c_pdp_3;/* C-PDP-3, counter related to F3 */
    Int8    c_pdp_4;/* C-PDP-4, counter related to F4 */
} RpmOmc;

/** RPM Operational Management Counters Leak Rate
 */
typedef struct RpmOmcLeakRateTag {
    Int8    lr_1;   /* LR-1, leak rate for C-BR-1 */
    Int8    lr_2;   /* LR-2, leak rate for C-R-1 */
    Int8    lr_3;   /* LR-3, leak rate for C-PDP-1 to C-PDP-4 */
} RpmOmcLeakRate;

/** RPM Information Stored in (U)SIM card
 */
typedef struct RpmInfoInSimTag {
    RpmParam        param;
    RpmOmc          omc;
    RpmOmcLeakRate  omcLeakRate;
    Int8            verImpl; /* RPM Version Implemented */
} RpmInfoInSim;

/** State of RPM context
*/
typedef enum RpmContextStateTag {
    RPM_CONTEXT_ST_NULL,
    RPM_CONTEXT_ST_INITIALIZING,
    RPM_CONTEXT_ST_READY,
    RPM_CONTEXT_ST_OTA_UPDATING
} RpmContextState;

#endif /* ABRPM_TYP_H */

