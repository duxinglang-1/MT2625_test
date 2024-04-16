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
 * File Description :                                                    */
/** \file
 *
 * Contains definitions of GSM MM data common to several other header files
 **************************************************************************/


#ifndef MM_COMM_H
#define MM_COMM_H


/** Describe to AL why we have sent a MMR_ERROR_IND */
typedef enum MmErrorCauseTag
{
    /** Failed to camp */
    MM_ERROR_UNEX_CAMP_REQ,
    /** Failed to register */
    MM_ERROR_UNEX_REG_REQ,
    /** Failure to suspend.*/
    MM_ERROR_UNEX_NREG_REQ,
    /** Bounce a second PLMN list request back to AL instead of AS discarding it.*/
    MM_ERROR_UNEX_PLMN_LIST_REQ,
    /** SIM problem */
    MM_ERROR_SIM_PROBLEM
    /** GSM SIM so cannot support LTE */
    ,MM_ERROR_SIM_NO_LTE_SUPPORT
    /** Number of items in MmErrorCause enum */
    , MM_ERROR_MAX_NUMBER
    /** Init value */
    , MM_ERROR_NULL
}
MmErrorCause;

/** MM state */
typedef enum MmStateTag  /* defined in 4.1.2.1.1 */
{
                                                          /*     GRR         */
                                                          /* connection y/n */
    /** The mobile station is inactive */
    M0_MM_NULL = 0,                                                   /* n  */
    /** A location updating procedure has been started and
     * the MM awaits a response from the network */
    M3_WAIT_FOR_LOC_UPDATE_RESPONSE = 3,                              /* y  */
    /**The MM connection establishment has been started,
     * and the MM awaits a response from the network. */
    M5_WAIT_FOR_OUTGOING_MM_CONNECTION = 5,                           /* y  */
    /** The MM sublayer has a RR connection to its peer
     * entity on the network side and one or more MM
     * connections are active.*/
    M6_MM_CONNECTION_ACTIVE = 6,                                      /* y  */
    /**The IMSI detach procedure has been started. */
    M7_IMSI_DETACH_INITIATED = 7,                                     /* y  */
    /** The mobile station is passive, awaiting further
     * commands from the network */
    M9_WAIT_FOR_NETWORK_COMMAND = 9,                                  /* y  */
    /** A location updating procedure has been rejected
     * and RR connection release is awaited */
    M10_LOCATION_UPDATE_REJECTED = 10,                                /* y  */
    /** The MM sublayer has requested RR
     * connection establishment for starting the
     * location updating procedure. */
    M13_RR_CONN_PENDING_LOC_UPDATING = 13,                            /* n  */
    /** The MM sublayer has requested RR connection
     * establishment for dedicated mode for starting
     * the MM connection establishment*/
    M14_RR_CONN_PENDING_MM_CONNECTION = 14,                           /* n  */
    /** The MM sublayer has requested RR connection
     * establishment for starting the IMSI detach procedure*/
    M15_RR_CONN_PENDING_IMSI_DETACH = 15,                             /* n  */
    /** A lower layer failure has occurred and re-establishment
     * may be performed from the disturbed CM layer entities.*/
    M17_WAIT_FOR_CM_REESTABLISH = 17,                                 /* n  */
    /** The MM sublayer has requested activation of the RR sublayer.*/
    M18_WAIT_FOR_RR_ACTIVE = 18,                                      /* n  */
    /** The MM connection establishment for an additional MM
     * connection has been started, and the MM awaits response
     * from the network.*/
    M20_WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION = 20,              /* y  */
    /** A location updating has been started using
     * the combined GPRS routing area updating procedure.*/
    M23_LOCATION_UPDATE_PENDING = 23,
    /** An IMSI detach for non-GPRS services has been
     * started using the combined GPRS detach procedure
     * at not switching off.*/
    M24_IMSI_DETACH_PENDING = 24,
    /** all requests from the CM layers are treated normally */
    M19_MM_IDLE_NORMAL_SERVICE = 191,             /* state 19.1 */    /* n  */
    /** Emergency call requests are treated
     * normally, otherwise the request triggers
     * first a location updating attempt in the
     * selected cell*/
    M19_MM_IDLE_ATTEMPTING_TO_UPDATE = 192,       /* state 19.2 */    /* n  */
    /** Only emergency services are offered
     * (cell not able to provide normal service).*/
    M19_MM_IDLE_LIMITED_SERVICE = 193,            /* state 19.3 */    /* n  */
    /** No valid subscriber data and a cell is selected */
    M19_MM_IDLE_NO_IMSI = 194,                    /* state 19.4 */    /* n  */
    /** No cell can be selected */
    M19_MM_IDLE_NO_CELL_AVAILABLE = 195,          /* state 19.5 */    /* n  */
    /** A location updating must be done as soon as possible. */
    M19_MM_IDLE_LOC_UPDATE_NEEDED = 196,          /* state 19.6 */    /* n  */
    /** Other MM states are not implemented; they are insufficient to
     * describe behaviour.
     */
    MM_NUM_STATES  /* leave as last constant */
}
MmState;

#endif
