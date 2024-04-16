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
 * File Description                                                      */
/** \file
 * Background General Library signals
 **************************************************************************/

#ifndef ABGL_SIG_H
#define ABGL_SIG_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#if !defined(ABGL_TYP_H)
#   include <abgl_typ.h>
#endif

#if !defined(UT_CODEC_H)
#   include <ut_codec.h>
#endif


/** \defgroup SigAbgl  ABGL Signalling Interface
 * \ingroup NasApex
 * \ingroup Sig
 * This section describes the Background General Library signals.
 * The Background General Library is a collection of functions of general use 
 * within the Background Layer Task. The following functionality is provided:<UL>
 * <LI> allocation/management of <EM>command references</EM> for use with SIM 
 * Task signals, via the ALSI interface
 * <LI> non-volatile RAM Task signals and timers
 * <LI> power down of the handset from within a Background Layer Procedure
 * <LI> generation of ApexGlErrorInd signals to the Foreground Layer Task </UL>
 * @{
 */
/***************************************************************************
 * Manifest Constants
 **************************************************************************/


/***************************************************************************
 * Typed Constants
 **************************************************************************/


/***************************************************************************
 * Type Definitions
 **************************************************************************/


/***************************************************************************
 *  Macros
 **************************************************************************/


/***************************************************************************
 *  Function Prototypes
 **************************************************************************/


/** Error indication to the Foreground Layer Task. 
 *  This signal is used to flag errors to the FL
 */
typedef struct ApexGlErrorIndTag
{
  /** The sending task ID */
  TaskId            taskId;
  /** The type of error */
  AlErrorClass      alErrorClass;
  /** The cause of error */
  AlErrorCode       alErrorCode;
  /** BL procedure that generated the error */
  ProcId            procId;       
}
ApexGlErrorInd;

/** Indirection to inform other tasks that GL status has changed
 */
typedef struct ApexGlReadyIndTag
{
    /** The sending task ID */
    TaskId                      taskId;
    /** New status*/
    AbglRequestStatus           status;
    /** The features configuration*/
    AbglFeatureConfigDataArea   featureConfig;
    /** The codecs configuration*/
    UtVoiceCodingType           codecs;
}ApexGlReadyInd;

/** Request to read the current feature configuration
 */
typedef struct ApexGlReadFeatureConfigReqTag
{
    /** The sending task ID */
    TaskId                      taskId;
    /** Command reference which is mirrored in the confirmation signal */
    Int16                       commandRef;
}ApexGlReadFeatureConfigReq;

/** Confirmation for the read features configuration request 
 */
typedef struct ApexGlReadFeatureConfigCnfTag
{
    /** The sending task ID */
    TaskId                      taskId;
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                       commandRef;
    /** Indication of whether or not the request was successful */
    AbglRequestStatus           requestStatus;
    /** The readed feature configuration*/
    AbglFeatureConfigDataArea   config;
}ApexGlReadFeatureConfigCnf;

/** Request to write in NVRAM a new configuration for features
 */
typedef struct ApexGlWriteFeatureConfigReqTag
{
    /** The sending task ID */
    TaskId                      taskId;
    /** Command reference which is mirrored in the confirmation signal */
    Int16                       commandRef;
    /** The new feature configuration to write*/
    AbglFeatureConfigDataArea   config;
}ApexGlWriteFeatureConfigReq;

/** Confirmation for the write features configuration request 
 */
typedef struct ApexGlWriteFeatureConfigCnfTag
{
    /** The sending task ID */
    TaskId                      taskId;
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                       commandRef;
    /** Indication of whether or not the request was successful */
    AbglRequestStatus           requestStatus;
}ApexGlWriteFeatureConfigCnf;

/** Request to read the current supported codecs list
 */
typedef struct ApexGlReadSupportedCodecsReqTag
{
    /** The sending task ID */
    TaskId                      taskId;
    /** Command reference which is mirrored in the confirmation signal */
    Int16                       commandRef;
}ApexGlReadSupportedCodecsReq;

/** Confirmation for the read the current supported codecs list
 */
typedef struct ApexGlReadSupportedCodecsCnfTag
{
    /** The sending task ID */
    TaskId                      taskId;
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                       commandRef;
    /** Indication of whether or not the request was successful */
    AbglRequestStatus           requestStatus;
    /** The readed codecs configuration*/
    UtVoiceCodingType           codecs;
}ApexGlReadSupportedCodecsCnf;

/** Request to write in NVRAM the current supported codecs list
 */
typedef struct ApexGlWriteSupportedCodecsReqTag
{
    /** The sending task ID */
    TaskId                      taskId;
    /** Command reference which is mirrored in the confirmation signal */
    Int16                       commandRef;
}ApexGlWriteSupportedCodecsReq;

/** Confirmation for the write supported codecs list request 
 */
typedef struct ApexGlWriteSupportedCodecsCnfTag
{
    /** The sending task ID */
    TaskId                      taskId;
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                       commandRef;
    /** Indication of whether or not the request was successful */
    AbglRequestStatus           requestStatus;
}ApexGlWriteSupportedCodecsCnf;

/** Request to set a new supported codecs list
 */
typedef struct ApexGlSetSupportedCodecsReqTag
{
    /** The sending task ID */
    TaskId                      taskId;
    /** Command reference which is mirrored in the confirmation signal */
    Int16                       commandRef;
    /** The new feature configuration to use*/
    UtVoiceCodingType           codecs;
}ApexGlSetSupportedCodecsReq;

/** Confirmation for the set supported codecs list request 
 */
typedef struct ApexGlSetSupportedCodecsCnfTag
{
    /** The sending task ID */
    TaskId                      taskId;
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                       commandRef;
    /** Indication of whether or not the request was successful */
    AbglRequestStatus           requestStatus;
}ApexGlSetSupportedCodecsCnf;

/** @} */ /* End of SigAbgl group */
#endif

/* END OF FILE */
