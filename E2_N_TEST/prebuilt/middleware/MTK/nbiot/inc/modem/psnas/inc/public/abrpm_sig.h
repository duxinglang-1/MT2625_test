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
 *   Definition of signals sent to/from ABRPM module within the AB task
 **************************************************************************/

#ifndef ABRPM_SIG_H
#define ABRPM_SIG_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/
#include <system.h>
#include <abrpm_typ.h>
#ifdef ENABLE_ABRPM_UNIT_TEST
#include <abrpm_fnc.h>
#endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/
typedef enum AbrpmCauseTag {
    ABRPM_CAUSE_N1_REACHED,
    ABRPM_CAUSE_F1_REACHED,
    ABRPM_CAUSE_F2_REACHED,
    ABRPM_CAUSE_F3_REACHED,
    ABRPM_CAUSE_F4_REACHED,
    ABRPM_CAUSE_T1_TIMEOUT,
    ABRPM_CAUSE_GENERAL_ERROR
} AbrpmCause;

typedef enum AbrpmReadAreaTag {
    ABRPM_READ_AREA_SIM,                /* Read RPM info from (U)SIM */
    ABRPM_READ_AREA_NVRAM               /* Read RPM info from NVRAM */
} AbrpmReadArea;

typedef enum AbrpmReadErrorTag {
    ABRPM_READ_ERR_NO_ERROR,            /* No error happens when reading RPM info */
    ABRPM_READ_ERR_SIM_NOT_READY,       /* (U)SIM is not ready */
    ABRPM_READ_ERR_NOT_PRESENT_IN_SIM,  /* RPM info is not present in (U)SIM */
    ABRPM_READ_ERR_NVRAM_ERROR          /* NVRAM error is encountered when reading RPM info */
} AbrpmReadError;

typedef struct ApexRpmIndTag {
    AbrpmCause          cause;          /* Cause to send the indication */
    Int32               time;           /* Remaining time in seconds to block an action */
} ApexRpmInd;

typedef struct ApexRpmResetChipsetIndTag {
    Int8                reserved;
} ApexRpmResetChipsetInd;

typedef struct ApexRpmReadInfoReqTag {
    AbrpmReadArea       area;
} ApexRpmReadInfoReq;

typedef struct ApexRpmReadInfoCnfTag {
    AbrpmReadError      error;
    AbrpmReadArea       area;
    RpmInfoInSim        info;
} ApexRpmReadInfoCnf;

#ifdef ENABLE_ABRPM_UNIT_TEST
typedef struct AbrpmUtPrepareForInitializeReqTag {
    RpmInitPhase        initPhase;
} AbrpmUtPrepareForInitializeReq;

typedef struct AbrpmUtInitializeReqTag {
    Boolean             rpmPresent;
    RpmInfoInSim        rpm;
} AbrpmUtInitializeReq;

typedef struct AbrpmUtCheckActionReqTag {
    RpmCheckType        actionToCheck;
    Boolean             apnPresent;
    AccessPointName     apn;
} AbrpmUtCheckActionReq;

typedef struct AbrpmUtCheckActionCnfTag {
    Boolean             actionAllowed;
} AbrpmUtCheckActionCnf;

typedef struct AbrpmUtHandleNetworkResponseReqTag {
    RpmNwRespType       nwRespType;
    Int32               cause;
    Boolean             apnPresent;
    AccessPointName     apn;
} AbrpmUtHandleNetworkResponseReq;

typedef struct AbrpmUtUpdateForOtaReqTag {
    Boolean             isRpmParamUpdated;
    Boolean             isRpmOmcLeakRateUpdated;
} AbrpmUtUpdateForOtaReq;

typedef struct AbrpmUtUpdateForSimChangeReqTag {
    Int32               reserved;
} AbrpmUtUpdateForSimChangeReq;

typedef struct AbrpmUtPrepareForSleepReqTag {
    Int32               reserved;
} AbrpmUtPrepareForSleepReq;

typedef struct AbrpmUtSetKernelTickReqTag {
    KernelTicks         tick;
} AbrpmUtSetKernelTickReq;

typedef struct AbrpmUtSetNvdmRpmParamReqTag {
    Boolean             isValid;
    RpmParam            param;
} AbrpmUtSetNvdmRpmParamReq;

typedef struct AbrpmUtSetNvdmChipResetInfoReqTag {
    Boolean             isValid;
    RpmChipResetInfo    chipResetInfo;
} AbrpmUtSetNvdmChipResetInfoReq;

typedef struct AbrpmUtSetNvdmRpmContextReqTag {
    Boolean             isValid;
    RpmContext          context;
} AbrpmUtSetNvdmRpmContextReq;

typedef struct AbrpmUtTransitContextStateIndTag {
    RpmContextState     state;
} AbrpmUtTransitContextStateInd;

typedef struct AbrpmUtResetChipsetCnfTag {
    Int32               reserved;
} AbrpmUtResetChipsetCnf;

#endif /* ENABLE_ABRPM_UNIT_TEST */

#endif /* ABRPM_SIG_H */

