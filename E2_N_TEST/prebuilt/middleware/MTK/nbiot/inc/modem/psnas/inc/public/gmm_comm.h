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
 * Contains definitions of GMM data common to several other header files
 **************************************************************************/


#ifndef GMM_COMM_H
#define GMM_COMM_H

#include <system.h>
#include <gpllc.h>
#include <sir_typ.h>

/** GMM state element */
typedef enum GmmStateTag  /* defined in 4.1.2.1.1 */
{
    /** The GPRS capability is disabled in the MS
     * (No GPRS mobility management function shall be performed in this state)
     */
    GMM_NULL                                = 0,
    /** A GPRS attach or combined GPRS attach procedure has been started and
     * the MS is awaiting a response from the network 
     */
    GMM_REGISTERED_INITIATED                = 1,
    /** The MS has requested release of the GMM context by starting the GPRS 
     * detach or combined GPRS detach procedure
     */
    GMM_DEREGISTERED_INITIATED              = 2,
    /** A routing area updating procedure has been started and the MS is 
     * awaiting a response from the network
     */
    GMM_ROUTING_AREA_UPDATING_INITIATED     = 3,
    /** User data and signalling information may be sent and received */
    GMM_REGISTERED_NORMAL_SERVICE           = 10,
    /** The MS shall enter this substate when entering dedicated mode
     * and when the MS limitations makes it unable to communicate on GPRS
     * channels
     */
    GMM_REGISTERED_SUSPENDED                = 11,
    /** The MS has to perform a routing area updating procedure, but its 
     * access class is not allowed in the cell due to common access class 
     * control or PS domain specific access control
     */
    GMM_REGISTERED_UPDATE_NEEDED            = 12,
    /** A routing area updating procedure failed due to a missing response from the network; 
     * The MS retries the procedure controlled by timers and a GPRS attempt counter 
     */
    GMM_REGISTERED_ATTEMPTING_TO_UPDATE     = 13,
    /** GPRS coverage has been lost. In this substate, the MS shall not initiate 
     * any GMM procedures except of cell (and PLMN) reselection
     */
    GMM_REGISTERED_NO_CELL_AVAILABLE        = 14,
    /** A cell is selected, which is known not to be able to provide normal service */
    GMM_REGISTERED_LIMITED_SERVICE          = 15,
    /** A combined routing area updating procedure or a combined GPRS attach procedure
     * was successful for GPRS services only 
     */
    GMM_REGISTERED_ATTEMPTING_TO_UPDATE_MM  = 16,
    /** Valid subscriber data is available, the GPRS update status is GU1 or GU2, 
     * a cell has been selected
     */
    GMM_DEREGISTERED_NORMAL_SERVICE         = 20,
    /** Valid subscriber data is available, GPRS update status is GU3, and a 
     * cell is selected, which is known not to be able to provide normal service
     */
    GMM_DEREGISTERED_LIMITED_SERVICE        = 21,
    /** Valid subscriber data is available and for some reason a GPRS attach
     * must be performed as soon as possible 
     */
    GMM_DEREGISTERED_ATTACH_NEEDED          = 22,
    /** The GPRS update status is GU2, a cell is selected, a previous GPRS 
     * attach was rejected. The execution of further attach procedures depends 
     * on the GPRS attach attempt counter
     */
    GMM_DEREGISTERED_ATTEMPTING_TO_ATTACH   = 23,
    /** No valid subscriber data is available (no SIM/USIM, or the SIM/USIM
     * is not considered valid by the ME) and a cell has been selected 
     */
    GMM_DEREGISTERED_NO_IMSI                = 24,
    /** No cell can be selected  */
    GMM_DEREGISTERED_NO_CELL_AVAILABLE      = 25,
    /** The MS shall enter this substate when entering dedicated mode and the 
     * MS limitations make it unable to communicate on GPRS channels  
     */
    GMM_DEREGISTERED_SUSPENDED              = 26,
    /** The mobile station is searching for PLMNs */
    GMM_DEREGISTERED_PLMN_SEARCH            = 27,
    /** The MS performs a combined GPRS detach procedure for non-GPRS services 
     * only (detach type IMSI Detach)
     */
    GMM_REGISTERED_IMSI_DETACH_INITIATED    = 28,
#if defined (UPGRADE_3G)
    /** A service request procedure has been started and the MS is awaiting 
     * a response from the network
     */
    GMM_SERVICE_REQUEST_INITIATED           = 29,
#endif
    /** Number of elements in GmmState */
    GMM_NUM_STATES  /* leave as last constant */
}
GmmState;


typedef enum GmmSpecificProcedureTag
{
    GPRS_ONLY_ATTACH_PROC = 0,
    COMBINED_ATTACH_PROC,
    GMM_PERIODIC_RA_PROC,
    GMM_NORMAL_RA_PROC,
    GMM_COMBINED_RA_PROC,
    GMM_COMBINED_RA_WITH_IMSI_PROC,
    GPRS_ONLY_DETACH_PROC,
    GPRS_IMSI_DETACH_PROC,
    GPRS_ONLY_MT_DETACH,
    GPRS_MT_DETACH_REATTACH,
    COMBINED_DETACH_PROC,
    GMM_CELL_UPDATE_PROC,
    GMM_WAIT_FOR_SM_RESP_FOR_RAU,
# if defined(RAI_CHANGE_DURING_ESTABLISHING)   
    GMM_WAIT_FOR_PS_SIGNALLING_READY_FOR_DETACH,
# endif    
# if defined (UPGRADE_3G)
    GMM_SERVICE_REQUEST_PROC,
    GMM_WAIT_FOR_SM_RESP_FOR_PAGING,
# if defined(RAI_CHANGE_DURING_ESTABLISHING)    
    GMM_WAIT_FOR_CS_SIGNALLING_READY_FOR_RAU,
    GMM_WAIT_FOR_PS_SIGNALLING_READY_FOR_RAU,
    GMM_WAIT_FOR_CS_PS_SIGNALLING_READY_FOR_RAU,
# endif /* (RAI_CHANGE_DURING_ESTABLISHING) */    
# endif/* (UPGRADE_3G) */    
    NO_GMM_SPECIFIC_PROC
} GmmSpecificProcedure;

typedef enum GmmCommonProcedureTag
{
    GMM_GPRS_AUTHEN_PROC = 0,
    /*All the other GMM common procedures are single signal procedures so are not really needed here*/
    NO_GMM_COMMON_PROC
} GmmCommonProcedure;

typedef enum GmmProcedureStatusTag
{
    AWAITING_EXECUTION = 0,
    EXECUTING,
    AWAITING_COMPLETION,
    NO_ACTIVE_PROC
} GmmProcedureStatus;

typedef struct GmmProcedureTag
{
    GmmSpecificProcedure    gmmSpecificProc;
    GmmCommonProcedure      gmmCommonProc;
    GmmProcedureStatus      gmmSpecificProcStatus;
} GmmProcedure;

typedef enum TlliTypeTag
{
    LOCAL_TLLI = 0,
    FOREIGN_TLLI = 1,
    RANDOM_TLLI = 2,
    AUX_TLLI = 3,
    UNASSIGNED_TLLI = 4
} TlliType;

typedef struct TlliTag
{
    TlliType    tlliType;
    Int32       oldTlli;
    Int32       tlli;
    Mnc         tlliMnc;
} Tlli;

#endif
