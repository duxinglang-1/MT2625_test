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
*******************************************************************************/
/** \file
 *
 * CalDev signal structures for NB-IoT Layer 1.
 */

#ifndef N1CD_SIG_H
#define N1CD_SIG_H

/*******************************************************************************
 * Includes
 *******************************************************************************/
#include <system.h>
#include <gkitask.h>
#include <ki_typ.h>
#include <n1_sth_cds_type.h>
//#include <n1_l1tst.h>

/*******************************************************************************
 * Constants
 *******************************************************************************/

/** Value passed to Tx Manager test sequencer to indicate transmission may start
 * at any subframe without need to wait for a particular subframe.
 */
#define N1_CD_SEQ_TEST_TXM_START_ANY_SUBFRAME 0xFF
/*Array size may change between META - NL1TST versions; need to manually update in that case */
#define MAX_N1CD_RF_TEST_REQ_PARAM_LENGTH  (1024)
/*Array size may change between META - NL1TST versions; need to manually update in that case */
#define MAX_N1CD_RF_TEST_CNF_PARAM_LENGTH  (512)
/*Max Array Size for Params contained in N1CD_IDC_TEST_REQ */
#define MAX_N1CD_IDC_TEST_REQ_PARAM_LENGTH  (10)
/*Max Array Size for Params contained in N1CD_IDC_TEST_CNF */
#define MAX_N1CD_IDC_TEST_CNF_PARAM_LENGTH  (1)

/*******************************************************************************
 * Typedefs
 *******************************************************************************/

/* Enter caldev state. */
typedef struct N1CdEnterReqTag
{
    TaskId taskId;
    
} N1CdEnterReq;

typedef EmptySignal N1CdEnterCnf;

/* Exit caldev state. */
typedef EmptySignal N1CdExitReq;
typedef EmptySignal N1CdExitCnf;

#ifdef FEA_N1_TEST_SQ_ENABLE
/* Test sequencer. */
typedef struct N1CdSeqTestReqTag
{
    bool      callback;
    bool      suspend;
    uint16_t  resumeAfterFrames;
    uint8_t   resumeSubFrame;
    
} N1CdSeqTestReq;

typedef EmptySignal N1CdSeqTestCnf;

/** Specifies type of Tx Manager test. */
typedef enum N1CdSeqTestTxmTypeTag
{
    N1_CD_SEQ_TEST_TXM_PUSCH,
    N1_CD_SEQ_TEST_TXM_PRACH,
    NUM_N1_CD_SEQ_TEST_TXM_TYPES
    
} N1CdSeqTestTxmType;

/** Specifies source of uplink grant: DCI N0, DCI N1 (i.e. ACK/NACK), RAR (i.e. msg 3)
 * Note: this enum must be kept inline with N1TxmPuschUlGrantType to avoid format conversions.
 */
typedef enum N1CdSeqTestTxmGrantTypeTag
{
    N1_CD_SEQ_TEST_TXM_GRANT_TYPE_DCI_N0, /**< For data user transmission. */
    N1_CD_SEQ_TEST_TXM_GRANT_TYPE_DCI_N1, /**< For ACK/NACK transmission.  */
    N1_CD_SEQ_TEST_TXM_GRANT_TYPE_RAR,    /**< For message 3 transmission. */
    N1_CD_SEQ_TEST_TXM_GRANT_TYPES        /**< Number of different UL Grant sources. */
    
} N1CdSeqTestTxmGrantType;

/** Tx Manager parameters to test NPUSCH transmission. */
typedef struct N1CdSeqTestTxmPuschParamTag
{
    /** Specifies source of uplink grant: DCI N0, DCI N1 (i.e. ACK/NACK), RAR (i.e. msg 3) */
    N1CdSeqTestTxmGrantType grantType;

    /** In case of NPUSCH format 1, this represents I_sc (6-bit). See Table [213]/16.5.1.1-1.
     * In case of NPUSCH format 2, this represents HARQ-ACK resource (4-bit) see Table [213]/16.4.2.1.
     */
    uint8_t                 subcarrierIndication;

    /** I_RU (3-bit) to derive number of resource units (Nru). See Table [213]/16.5.1.1-2. */
    uint8_t                 resourceAssignment;

    /* Modulation and Coding Scheme*/
    uint8_t                 iMcs;

    /** Set to either N1_TXM_REDUNDANCY_VERSION_0 or N1_TXM_REDUNDANCY_VERSION_2. */
    uint8_t                 initialRV;

    /** I_Rep (3-bit). See Table [213]/16.5.1.1-3. */
    uint8_t                 repetitionNumber;

    /** C-RNTI necessary to initialise scrambler code. */
    uint16_t                crnti;

    /** Set to TRUE is subcarrier spacing is 15 kHz, and FALSE for 3.75kHz.  */
    bool                    is15kHzModeOn;
    
} N1CdSeqTestTxmPuschParam;

/* Tx Manager parameters to test NPRACH transmission. */
typedef struct N1CdSeqTestTxmPrachParamTag
{
    /** This corresponds to the value of ninit in section [211]/10.1.6.1.
     * Range = 0..prach-NumSubcarriers - 1, where prach-NumSubcarriers equals
     * 12, 24, 36 or 48.
     */
    uint8_t   nInit;

    /* Higher-layer parameter found in cell configuration data base. Possible
     * values: 0,12,24,36,2,18,34.
     */
    uint8_t   nprachSubcarrierOffset;

    /** TRUE for NPRACH format 1; FALSE for NPRACH format 0. */
    bool      isFormat1;

    /** Number of 4 symbol groups to be transmitted. */
    uint8_t   numRepPrach;
    
} N1CdSeqTestTxmPrachParam;

/* Tx Manager parameters for all tests driven from SIG_N1CD_SEQ_TEST_TXM_REQ. */
typedef union N1CdSeqTestTxmParamTag
{
    N1CdSeqTestTxmPuschParam pusch;
    N1CdSeqTestTxmPrachParam prach;
    
} N1CdSeqTestTxmParam;

/* Signal structure for Tx Manager test sequencer (SIG_N1CD_SEQ_TEST_TXM_REQ). */
typedef struct N1CdSeqTestTxmReqTag
{
    /** \assoc UNION \ref param */
    N1CdSeqTestTxmType   type;

    /** Indicates subframe number to start uplink transmission.
     * Set it to N1_CD_SEQ_TEST_TXM_START_ANY_SUBFRAME to start transmission as soon
     * as the signal is received (i.e. witout waiting for a specific subframe number).
     */
    int8_t               txSubframeStart;

    /** Physical Cell ID to unit test uplink. */
    uint16_t             cellId;

    /** Timing advance value. */
    uint16_t             timingAdvance;

    /** Configuration parameters. */
    N1CdSeqTestTxmParam  param;
    
} N1CdSeqTestTxmReq;

typedef EmptySignal N1CdSeqTestTxmCnf;

/* Test fse. */
typedef struct N1CdFseTestReqTag
{
    N1SthCdsFseTestType type;
    uint16_t               hSfn;
    uint16_t               hSfnOffset;
    uint16_t               sfn;
    uint32_t               sfnOffset;
    uint8_t                subFrame;
    uint32_t               subFrameOffset;

} N1CdFseTestReq;

typedef EmptySignal N1CdFseTestCnf;
#endif

/*Calibration Test Mode :  Request From ATCI to L1 Task (NL1TST) */
typedef struct N1CdRfTestReqTag
{
  uint16_t      token;
  uint8_t       cmd;
  /** No response expected. */
  uint8_t       silent;
  uint8_t       param[MAX_N1CD_RF_TEST_REQ_PARAM_LENGTH];
  
} N1CdRfTestReq;

/*Calibration Test Mode :  Confirmation from L1 Task (NL1TST) to ATCI */
typedef struct N1CdRfTestCnfTag
{
  uint16_t      token;
  uint8_t       cmd;
  uint8_t       dummy;/*for alignment*/
  uint16_t      length;
  uint16_t      dummy1;/*for alignment*/
  uint8_t       param[MAX_N1CD_RF_TEST_CNF_PARAM_LENGTH];
  
} N1CdRfTestCnf;

/*IDC Test Mode :  Request From Test PC to L1 Task */
typedef struct N1CdIdcTestReqTag
{
  uint16_t      token;
  uint8_t       cmd;
  /** No response expected. */
  uint8_t       silent;
  uint8_t       param[MAX_N1CD_IDC_TEST_REQ_PARAM_LENGTH];
  
} N1CdIdcTestReq;

/*IDC Test Mode :  Confirmation From Test PC to L1 Task */
typedef struct N1CdIdcTestCnfTag
{
  uint16_t      token;
  uint8_t       cmd;
  uint8_t       dummy;/*for alignment*/
  uint16_t      length;
  uint16_t      dummy1;/*for alignment*/
  uint8_t       param[MAX_N1CD_IDC_TEST_CNF_PARAM_LENGTH];

} N1CdIdcTestCnf;

#endif /* N1CD_SIG_H */

/* END OF FILE */
