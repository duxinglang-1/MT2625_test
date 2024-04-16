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
 * Types common to dl_sig.h mph_sig.h ph_sig.h rr_sig.h and mmxx_typ.h.
 **************************************************************************/

#ifndef L123_TYP_H
#define L123_TYP_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#if !defined (SYSTEM_H)
#include     <system.h>
#endif

/****************************************************************************
 * Macros
 ****************************************************************************/

/** Invalid reduced frame number.
 * Reduced frame numbers are optionally used in L1 configuration requests to
 * synchronise network and mobile changes. The \a NO_STARTING_TIME value is
 * used to indicate that the facility is not being used in a configuration
 * request
 */
#define     NO_STARTING_TIME            ((ReducedFrameNumber)0xFFFF)


#define     MAX_REDUCED_FRAME_NUMBER    ((ReducedFrameNumber)(32 * 51 * 26))

/** Maximum number of Arfcns in a mobile allocation.
 * This is used to dimension the #MobileAllocation type, and can hold
 * the maximum required channels for a frequency definition.
 */
#define     MAX_MA_CHANNELS             64

/****************************************************************************
 * Types
 ****************************************************************************/

/**
 * The value is 0xffff for non PGSM and 0 for PGSM
 * \todo Remove NO_ARFCN and replace all instances with INVALID_ARFCN
 */
#define     NO_ARFCN                    INVALID_ARFCN

/** Absolute Radio Frequency Channel Number.
 * An ARFCN uniquely identifies a channel in a band, where a channel consists
 * of both an uplink and downlink frequency component (GSM is Frequency
 * Division Duplex).
 *
 * Protocol stack ARFCNs are numbered 1 to #NUM_RF_CHANNELS. This is the
 * same as the GSM definition for non-extended GSM. The extra channels in
 * E-GSM are defined to be 975 to 1023 and 0 in the appropriate TS, however
 * in to the Protocol Stack (and interface to L1) they are numbered 1-174.
 * This allows smaller types to be used to hold ARFCNs, and they can be used
 * to index tables without any processing.
 */
typedef Int16 Arfcn;


/** Reduced Frame Number.
 * Reduced frame numbers are optionally used in L1 configuration requests to
 * synchronise network and mobile changes.  A reduced frame number is in the
 * range of 0 to (26 * 51 * 32) - 1.
 */
typedef Int16 ReducedFrameNumber;


/** Base Station Identity Code.
 * A code that together with the C0 Arfcn of a cell uniquely identifies some
 * cell.  This is decoded from the SCH, a sub-channel of the BCCH.
 *
 * The special value #INVALID_BSIC is used to indicate an unknown or invalid
 * BSIC.  #INCONCLUSIVE_BSIC may also be used to indicate a failure to decode
 * a BSIC accurately.
 */
typedef Int8 Bsic;

/** Training Sequence Code
 * Bit pattern inserted into bursts to aid equalisation.  The special value
 * #NO_TSC is used to indicate an unknown or invalid training sequence.
 */
typedef Int8 Tsc;

/** No Training Sequence Code
 * Special value for a training sequence code to indicate unknown or invalid
 * training sequence.  Normally a GSM training sequence is in the range 0 to
 * 7.
 */
#define     NO_TSC     0xFF


#endif
/* END OF FILE */































