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
 * File Description:                                                      */

/** \file
 * Eutra/Utra PS Signal Data Type definitions for the MMDBM Interface
 **************************************************************************/

#ifndef MMDBM_SIG_H
#define MMDBM_SIG_H

/***************************************************************************
* Nested Include Files
***************************************************************************/

#include <system.h>
#include <l3gp_typ.h>
#include <l3_typ.h>

/** \defgroup SigMmdbm MMDBM Signalling Interface
 * \ingroup Prd3gDmNas
 * \ingroup Sig
 * This document specifies the signal interface between Mobility Management
 * (MM) and Data Bearer Management (DBM).
 * The interface between MM and DBM is known as the MMDBM interface.
 * It describes the signals sent between these two entities in terms of data
 * structures and data values, and provides a summary of the purpose of each
 * signal, when it may be expected to occur, and how it relates to other
 * signals on the same interface.
 * This interface is designed to support 3G and LTE.
 *
 * @{
 */

/** \defgroup SigMmdbmContext MMDBM Interface Context
 * This section defines the context of the MMDBM interface.
 *
 * @{
 * The MMDBM interface is illustrated in the Figure 1 below:
 *
 * \image html mmdbm_interface_context_diagram.png "Figure 1: MMDBM Interface"
 */
/** @} */ /* End of SigMmdbmContext group */

/***************************************************************************
*   Manifest Constants
***************************************************************************/

/***************************************************************************
*   Macro Functions
***************************************************************************/

/***************************************************************************
*   Types
***************************************************************************/
/** \def MMDBM_MAX_NUM_PSD_BEARERS
 *  Defines maximum number of PSD bearers that can be established or released
 *  with one MMDBM signal */
#define MMDBM_MAX_NUM_PSD_BEARERS       16

#define MMDBM_MAX_NUM_DRBS  2

/** Determines the cause of failure for radio bearer re-establishment. */
typedef enum MmDbmCauseTag
{
    /** No cause value present, this is used only internally by EMM. */
    MMDBM_NO_CAUSE,

    MMDBM_CAUSE_CATEGORY_HOLD_START,
    /*******************************************************
     * Define HOLD category causes here.                   *
     * HOLD = DBM waits for re-establish ind to re-request *
     *******************************************************/

    /** Tells that device is in PSM or NSPS and start waking up from it */
    MMDBM_CAUSE_SEARCH_STARTED,
    /** Cannot perform Service Request at the moment as need to run another
     * EMM procedure (EMM will later send MmDbmReestablishInd if the procedure
     * is completed successfully)
     */
    MMDBM_CAUSE_EMM_PROC_NEEDED,


    MMDBM_CAUSE_CATEGORY_HOLD_END,
    MMDBM_CAUSE_CATEGORY_RETRY_START,
    /*******************************************************
     * Define RETRY category causes here.                  *
     * RETRY = DBM should retry immediately                *
     *******************************************************/

    /** Received Service Reject or no reply from network */
    MMDBM_CAUSE_SERVICE_PROC_FAILED,


    MMDBM_CAUSE_CATEGORY_RETRY_END,
    MMDBM_CAUSE_CATEGORY_FATAL_START,
    /*******************************************************
     * Define FATAL category causes here.                  *
     * FATAL = Connection establishment is not possible,   *
     * user data shall be disarded                         *
     *******************************************************/

    /** Not attached/detaching so no packet switched service */
    MMDBM_CAUSE_NO_PS_SERVICE,
    /** Not registered, or URRC or ERRC connection failed, or RAU failed */
    MMDBM_CAUSE_REESTAB_FAILED,
    /** Back-off timer running */
    MMDBM_CAUSE_BACKOFF,


    MMDBM_CAUSE_CATEGORY_FATAL_END
}
MmDbmCause;

/* Definition of EPS connection management states 23.401 4.6.3 */
typedef enum EcmStateTag
{
    ECM_IDLE,
    ECM_CONNECTED
}
EcmState;

/* Preferred CIoT behavior for the network */
typedef enum PreferredCiotNwBehaviourTag
{
    NO_ADDITIONAL_INFO,
    CP_CIOT_OPTIMIZATIONS,
    UP_CIOT_OPTIMIZATIONS
}
PreferredCiotNwBehaviour;

/* Support of CIoT EPS optimizations in the current network */
typedef struct SupportedCiotOptimizationsTag
{
    /* Control plane CIoT EPS optimization support bit */
    Boolean cpciotSupport;
    /* User plane CIoT EPS optimization support bit */
    Boolean upciotSupport;
}
SupportedCiotOptimizations;

/** \defgroup SigMmdbmSignals  MMDBM Primitives
 * Primitives exchanged between MM and DBM.
 * This section defines primitives exchanged between MM and DBM
 *
 * @{
 *
 */
/** This signal is sent from MM to DBM in 3G to indicate that the reception of
 *  the Service Request message has been acknowledged by the network.
 */
typedef struct MmDbmReestablishCnfTag
{
    /** Empty signal */
    Int8                    empty;
}
MmDbmReestablishCnf;

/** This signal is sent from MM to DBM to indicate that MM is now able to
 *  carry out the re-establishment request that has been previously rejected.
 */
typedef struct MmDbmReestablishIndTag
{
    /** Empty signal */
    Int8                    empty;
}
MmDbmReestablishInd;

typedef struct MmDbmReestablishRspTag
{
    /** Empty signal */
    Int8                    empty;
}
MmDbmReestablishRsp;

/** This signal is sent from MM to DBM to indicate a failure to
 *  re-establish radio bearers.
 */
typedef struct MmDbmReestablishRejTag
{
    /** The cause of failure */
    MmDbmCause              mmDbmCause;
}
MmDbmReestablishRej;

/** This signal is sent from DBM to MM to indicate that UL user data
 *  has been received for a PSD bearer context while the associated
 *  radio bearer has been released. MM shall initiate a service request
 *  procedure to re-establish the radio bearers by sending a SERVICE
 *  REQUEST message with service type data to the network.
 */
typedef struct MmDbmReestablishReqTag
{
    /** Indicates which PSD bearer contexts are active */
    PsdBearerContextStatus  psdBearerContextStatus;
    /** Indicates the most demanding traffic class among the PSD bearer contexts
     * set in psdBearerContextStatus parameter. This is used to determine
     * the establishment cause in URRC and ERRC messages.
     * The traffic class order from most demanding to least demanding is:
     * <UL>
     * <LI> GPRS_TRAFFIC_CLASS_CONVERSATIONAL
     * <LI> GPRS_TRAFFIC_CLASS_STREAMING
     * <LI> GPRS_TRAFFIC_CLASS_INTERACTIVE
     * <LI> GPRS_TRAFFIC_CLASS_BACKGROUND
     * <LI> GPRS_TRAFFIC_CLASS_SUBSCRIBED
     *</UL>
     */
    TrafficClass            mostDemandingTrfcClass;
    /** If this is TRUE, then MM clears follow on request flag and does NOT
     * respond with a MmDbmReestablishRej signal
     */
    Boolean                 resetFollowOn;
}
MmDbmReestablishReq;

/** This signal is sent from DBM to MM to indicate that all PSD bearer contexts
 *  have been deactivated. This causes URRC (3G) to send a Signalling Connection
 *  Release message to the network. Note that there is no PSD bearer id in this
 *  signal, because DBM needs to inform MM only when all the PSD bearer contexts
 *  have been deactivated.
 */
typedef struct MmDbmReleaseReqTag
{
    /** Empty signal */
    Int8                    empty;
}
MmDbmReleaseReq;

/** This signal is sent from MM to DBM to suspend any ongoing transmission of user
 * data when MM is recovering from certain abnormal NAS signalling conditions.
 */
typedef struct MmDbmSuspendIndTag
{
    /** Empty signal */
    Int8                    empty;
}
MmDbmSuspendInd;

/** This signal is sent from MM to DBM to inform DBM that it can restart any ongoing
* transmission of user data  following an MmDbmSuspend request.
 */
typedef struct MmDbmResumeIndTag
{
    /** Empty signal */
    Int8                    empty;
}
MmDbmResumeInd;

typedef struct MmDbmBearerStatusIndTag
{
    /* State of EPS connection */
    EcmState                ecmState;
    /* RRC suspended information. Valid only when ecmState indicates idle. */
    Boolean                 rrcConnSuspended;
    /* DrbList */
    uint8_t                 epsIdCount;
    uint8_t                 epsId[MMDBM_MAX_NUM_DRBS];
    /* for the UE test loop function */
    uint8_t                 drbId[MMDBM_MAX_NUM_DRBS];
}
MmDbmBearerStatusInd;

/** This signal is sent from MM to DBM to indicate the current CIoT configuration
 * status. DBM uses the information to conclude route for uplink user data.
 */
typedef struct MmDbmCiotConfigIndTag
{
    /* Preferred CIoT network behaviour, i.e. which optimization to use, if any */
    PreferredCiotNwBehaviour    preferredCiotNwBehaviour;

    /* Supported CIoT optimizations by the network */
    SupportedCiotOptimizations  supportedCiotOptimizations;
}
MmDbmCiotConfigInd;

/** This signal is sent from EMM to DBM to inform DBM that radio bearer re-establishment
 * is not possible and queued user data requiring radio bearer shall be disarded.
 */
typedef struct MmDbmConnFailedIndTag
{
    /** Empty signal */
    Int8                    empty;
}
MmDbmConnFailedInd;

/** This signal is sent from MM to DBM to inform DBM that all expected
 * data radio bearers have been established
 */
typedef EmptySignal MmDbmActiveServiceCompleteInd;

/** Request the UE to deactivate the test loop.
 *  This signal is sent from EMM to DBM to request DBM to deactivate the test loop. */
typedef EmptySignal MmDbmDeactivateTestModeInd;

/** Request the UE to activate a test loop in mode A mode B or mode G.
 *  This signal is sent from EMM to DBM to request DBM to activate the test loop. */
typedef struct MmDbmActivateTestModeIndTag
{
    /** Defines whether to activate mode A, B, G, H or I. */
    EutraLoopBackMode                   loopBackMode;
}
MmDbmActivateTestModeInd;

/** Request the UE to open a test loop.
 *  This signal is sent from EMM to DBM to request DBM to open the test loop. */
typedef EmptySignal MmDbmOpenUeTestLoopInd;

/** Request the UE to close a test loop in mode A, mode B, mode G or mode I.
 *  This signal is sent from EMM to DBM to request DBM to close the test loop. */
typedef struct MmDbmCloseUeTestLoopIndTag
{
    /** Defines whether the loop back mode is A, B, G or I.
     *  \assoc UNION \ref loopBackConfig
     */
    EutraLoopBackMode                   loopBackMode;

    /** Defines the loop back configuration for modes A and B. */
    EutraLoopBackConfig                 loopBackConfig;
}
MmDbmCloseUeTestLoopInd;

/** This signal is sent from DBM to MM to indicate whether user data to uplink contains
* exception data or not */
typedef struct MmDbmExceptionReportsReqTag
{
    /** Exception reports enabled or not
       * If set to TRUE:
       * RRC establishment cause for all uplink user data and signalling
       * shall be set to MO exception data */
    Boolean                 isEnabled;
}
MmDbmExceptionReportsReq;

/** This signal is sent from EMM to DBM when RRC connection reestablishment for control
 *  plane completes */
typedef EmptySignal MmDbmCpReestablishInd;

/** @} */ /* End of SigMmdbmSignals group */

/** @} */ /* End of SigMmdbm group */

#endif /* MMDBM_SIG_H */

/* END OF FILE */

