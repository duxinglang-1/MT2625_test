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
 * GKI header file that defines the SignalId type (and other types
 * associated with the signal id).
 **************************************************************************/

#ifndef PSSIGNAL_H
#define PSSIGNAL_H

/** \addtogroup PrdGki
 * @{
 */

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

/*
 * Kisystsk.h has a number of conditional defines under !UPGRADE_GGE.
 * When building the signal data base in Genie these conditional defines are
 * not set because they are not included in tvsignal.h and this results in a
 * mistmatch between the number of signal IDs and the the number of signals
 * in the Signal Union. This is also a problem for EXCLUDE_GSM_PS and
 * EXCLUDE_GPRS_PS. Therefore include kitqid.h to ensure that these
 * conditional defines are set when buidling the Genie signal database.
 *
 * THIS SHOULD BE TIDIED UP, THERE SHOULD NOT BE THIS FILE DEPENDENCY. TODO/TBD.
 */
#include <kitqid.h>

/****************************************************************************
 * Types
 ****************************************************************************/

# define KI_SIGSET_START(sETnAME) KI_SIGNAL_SET_ID_##sETnAME,

/** Enumeration used to calculate signal set indexes.
 * The KiSignalSets enumeration is used to create the signal set indexes from
 * the kisysbas.h file. These are then used to create the signal base numbers
 * in the #KiSignalBaseIndex enumeration. */
typedef enum KiSignalSetsTag
{
#   include <kisysbas.h>

    KI_NUM_SIGNAL_SET_IDS

} KiSignalSets;

/* Define KI_BASE_DEF to allow extraction of base indexes */
#define KI_BASE_DEF(bASEnAME)   KI_BASE_INDEX_##bASEnAME,

#define KI_SIGSET_START(sETnAME) KI_SET_BASE_INDEX_##sETnAME = KI_SIGNAL_SET_ID_##sETnAME << 8,

/** Enumeration used to calculate signal base indexes.
 * The KiSignalBaseIndex enumeration is used to generate a signal base id. This
 * enumeration is a 1 based index of a base within it's signal set. These values
 * (minus one) are then used in the #KiSignalBaseId enumeration to calculate the
 * actual base values. */
typedef enum KiSignalBaseIndexTag
{
#   include <kisysbas.h>

    /* The following constant is used by the test tools to dimension
    ** the Normal or Compact filter matrix signal. */
    NUM_SIGNAL_BASES

} KiSignalBaseIndex;

/* Define KI_BASE_DEF to allow calculation of the signal bases */
#define KI_BASE_DEF(bASEnAME)   bASEnAME##_SIGNAL_BASE = (KI_BASE_INDEX_##bASEnAME - 1) * 0x0100,

#define KI_SIGSET_START(sETnAME)    KI_SIGNAL_SET_START_##sETnAME = KI_SIGNAL_SET_ID_##sETnAME << 16,

/** Enumeration containing all Signal base ids.
 * The KiSignalBaseId enumeration defines all the signal bases in the system.
 * The definitions are extracted from the KISYSBAS.H file. */
typedef enum SignalInterfaceBasesTag
{
#   include <kisysbas.h>

    KI_SIGNALBASE_ID_END
}
SignalInterfaceBases, KiSignalBaseId;

/** The maximum value for a SignalId will always be less than or
 * equal to this value. */
#define KI_MAX_SIGNAL_ID   (((NUM_SIGNAL_BASES + 1) << 8) - 1)

/*
** Any one base must have less than MAX_NUM_SIGS_IN_BASE signals
** to allow logging of signals with Normal or Compact filter matrices.
** Signals beyond MAX_NUM_SIGS_IN_BASE can still be logged but only with
** a Tiny filter matrix.
**
** For Normal Filter Matrix (MAX_NUM_SIGS_IN_BASE+1) * NUM_SIGNAL_BASES
**        must be less than the size of the largest memory block
**
** For Compact Filter Matrix ((MAX_NUM_SIGS_IN_BASE / 8) + 1) * NUM_SIGNAL_BASES
**        must be less than the size of the largest memory block
**
** For Tiny Filter Matrix (Number of signals in system / 8) + (1.5 * NUM_SIGNAL_BASES)
**        must be less than the size of the largest memory block
*/
#define     MAX_NUM_SIGS_IN_BASE    100

/* Macro for determining signal base from signal id */
#define M_KiGetSignalBaseId(sIGNALiD)       ((KiSignalBaseId)((sIGNALiD) & 0xFFFFFF00))
#define KI_SIGNAL_BASE_FROM_SIGNALID(sIGiD) M_KiGetSignalBaseId(sIGiD)

/* Macro for determining signal base index from signal id */
#define M_KiGetSignalBaseIndex(sIGNALiD)    ((sIGNALiD) >> 8)

/** Macro for determining signal offset within a signal base from a signal id. */
#define M_KiGetSignalOffset(sIGNALiD)       ((sIGNALiD) & 0x00FF)

/* This include causes the definition of the SIG_DEF macro to be changed
** such that it will generate entries in the SignalId enumeration. */
#define SIGNAL PSSIGNAL
#include <kisigdef.h>

/** Enumeration containing all signal ids.
 * The SignalId enumeration defines all the signal identifiers in the system.
 * This enumeration is generated from the use of the SIG_DEF() macro in signal
 * definition files e.g. kisig.h. This enumeration has to have exactly the same
 * number of members as the Signal union in KISIGUNI.H (the members must also
 * be in the same order). */
typedef enum SignalIdTag
{
#   include <kisiginc.h>

    NON_SIGNAL                      = 0,
    NoMoreSignals
}
SignalId;

/** @} */ /* End of PrdGki group */

#endif
/* END OF FILE */
