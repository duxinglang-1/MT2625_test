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
 * File Description:                                                     */
/** \file
 * Definition of PSD counter signals and types
 **************************************************************************/

#ifndef GPCNTR_H
#define GPCNTR_H

/**** NESTED INCLUDE FILES *************************************************/
#ifndef GEN_NVDM_DATABASE_FOR_PC
#include <gkitask.h>
#endif
#include <system.h>
#include <gpnsapi.h>
#if defined (UPGRADE_NASMDL2)
#include <psdbearerdef.h>
#endif /* UPGRADE_NASMDL2 */

/**** CONSTANTS ************************************************************/


/**** TYPEDEFS *************************************************************/
/** Report type element */
typedef enum ReportTypeTag
{
    /** Report counters once */
    ONCE = 0,
    /** Start reporting the counters periodically */
    START_PERIODIC,
    /** Report the counters when a context is deactivated */
    ON_CONTEXT_DEACTIVATION,
    /** Cancel previous START_PERIODIC and ON_CONTEXT_DEACTIVATION requests */
    CANCEL
}
ReportType;

/** The counter data in reports */
typedef struct CounterDataTag
{
    /** Number of octets (compressed) in the uplink direction */
    Int32   upLinkCompressedOctets;
    /** Number of octets (uncompressed) in the uplink direction */
    Int32   upLinkUncompressedOctets;
    /** Number of packets in the uplink direction */
    Int32   upLinkPackets;
    /** Number of octets (compressed) in the downlink direction */
    Int32   downLinkCompressedOctets;
    /** Number of octets (uncompressed) in the downlink direction */
    Int32   downLinkUncompressedOctets;
    /** Number of packets in the downlink direction */
    Int32   downLinkPackets;
}
CounterData;

/** Structure that contains the counter data and the PSD bearer id */
typedef struct CounterValTag
{
#if defined (UPGRADE_NASMDL2)
    /** EPS bearer id or NSAPI */
    Int8            psdBearerId;
#endif /* UPGRADE_NASMDL2 */
    /* TODO: Remove NSAPI under #else part of UPGRADE_NASMDL2 */
    /** NSAPI */
    Nsapi           nsapi;
    /** Counter data */
    CounterData     counterData;
}
CounterVal;

/** Request to report or stop reporting the data volume counters.
 * Request the PSD stack to report or stop reporting the data volume counters.
 * If the reportType is START_PERIODIC, the signal contains the timeInterval parameter.
 * It is an Int16 value containing the time interval between successive periodic counter
 * reports in seconds. The minimum value for the timeInterval is 1 second. If it is 0,
 * 1 second is used as the time interval. A new START_PERIODIC request overrides any
 * previously received START_PERIODIC requests and the new timeInterval value becomes
 * effective. A report will also be sent when a context has been deactivated during
 * a periodic report phase.
 *
 * If reportType is ON_CONTEXT_DEACTIVATION, a report will be sent when a context
 * is deactivated. PSD stack sends a spontaneous CounterInd to confirm the request.
 *
 * If reportType is CANCEL, previous START_PERIODIC and ON_CONTEXT_DEACTIVATION
 * requests will be cancelled. PSD stack sends a spontaneous CounterInd to confirm the request.
 */
typedef struct ReportCounterReqTag
{
    /* TODO: Move taskId under #if !defined UPGRADE_NASMDL2 */
    /** The TaskId of the task sending the request */
    TaskId      sourceTaskId;
    /** Indicates the reason for sending the signal */
    ReportType  reportType;
    /** Contains the time interval between successive periodic counter
     * reports in seconds
     */
    Int16       timeInterval;
}
ReportCounterReq;

/** This signal is sent in order to request the PSD stack to reset the counters */
typedef struct ResetCounterReqTag
{
#if defined (UPGRADE_NASMDL2)
    /** Indicates the number of PSD Bearers affected */
    Int8    numPsdBearers;
    /** Holds the information about which PSD Bearers the reset request applies to */
    Int8    psdBearerList[MAX_NUM_PSD_BEARERS];
#endif /* UPGRADE_NASMDL2 */
    /* TODO: Move following code under #else part of UPGRADE_NASMDL2 */
    /** Holds the TaskId of the task sending the request */
    TaskId  sourceTaskId;
    /** Indicates the number of NSAPIs affected */
    Int8    numNsapis;
    /** Holds the information about which NSAPIs the reset request applies to */
    Nsapi   nsapiList[MAX_NUM_NSAPIS];
}
ResetCounterReq;

/** This signal is used to report the current counter values.*/
typedef struct CounterIndTag
{
#if defined (UPGRADE_NASMDL2)
    /** Holds the uplink/downlink data/packet counter values for all the PSD Bearers */
    CounterVal  psdCounterValList[MAX_NUM_PSD_BEARERS];
    /** Released PSD Bearer ID.
     * The field releasedPsdBearerId will be coded as follows:
     * <UL>
     * <LI> If the CounterInd is because of a PDP context / EPS Bearer
     * deactivation (a PSD Bearer ID (NSAPI / EPS Bearer ID) has been released),
     * the releasedPsdBearerId field will be set as the value of the PSD Bearer
     * ID that has just been released
     * <LI> Otherwise, if the CounterInd is sent as a response to a report
     * or a reset request or as a periodic counter report, the field releasedPsdBearerId
     * will be set as PSD_BEARER_ID_UNASSIGNED
     * </UL>
     */
    Int8        releasedPsdBearerId;
#endif /* UPGRADE_NASMDL2 */
    /* TODO: Move following code under #else part of UPGRADE_NASMDL2 */
    /** Holds the TaskId of the task to send the indication to */
    TaskId      destTaskId;
    /** Holds the uplink/downlink data/packet counter values for all the NSAPIs */
    CounterVal  counterValList[MAX_NUM_NSAPIS];
    /** Released NSAPI.
     * The field releasedNsapi will be coded as follows:
     * <UL>
     * <LI> If the CounterInd is because of a PDP context deactivation
     * (an NSAPI has been released), the releasedNsapi field will be set as the
     * value of the NSAPI that has just been released
     * <LI> Otherwise, if the CounterInd is sent as a response to a report
     * or a reset request or as a periodic counter report, the field releasedNsapi
     * will be set as GPRS_NSAPI_UNALLOCATED
     * </UL>
     */
    Nsapi       releasedNsapi;
}
CounterInd;

#endif /* GPCNTR_H */

/* END OF FILE */
