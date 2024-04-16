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
 * L2 - SIM manager interface.
 **************************************************************************/

#ifndef L1SI_SIG_H
#define L1SI_SIG_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#include <system.h>
#include <ki_typ.h>

/****************************************************************************
 * Macros
 ****************************************************************************/

#define     SIM_MAX_MSG_SIZE                      261
#define     SIM_MAX_SMALL_MSG_SIZE                 18
#define     SIM_MAX_MEDIUM_MSG_SIZE                55
#define     SIM_MAX_LARGE_MSG_SIZE   SIM_MAX_MSG_SIZE
#define     SIM_MAX_EXTEND_MSG_SIZE              4000

#define     SIM_REQ_CLASS                       0
#define     SIM_REQ_INSTRUCTION                 1
#define     SIM_REQ_P1                          2
#define     SIM_REQ_P2                          3
#define     SIM_REQ_P3                          4
#define     SIM_REQ_DATA                        5
#define     USIM_CASE_1_HEADER                  4    /* CLA + INS + P1 + P2 */
#define     USIM_CASE_2_HEADER                  5    /* CLA + INS + P1 + P2 + Le*/
#define     USIM_CASE_3_HEADER                  5    /* CLA + INS + P1 + P2 + Lc*/
#define     USIM_CASE_4_HEADER                  6   /* CLA + INS + P1 + P2 + Lc + Le*/
#define     USIM_DATA_OFFSET                    5   /*data offset into the C-APDU*/
#define     USIM_EXPECTED_RSP_LENGTH            0

#define     SIM_LOG_CHAN_MAX_SESSION_NUM        20
#define     SIM_LOG_CHAN_BASIC_SESSION_NUM      4
#define     SIM_LOG_CHAN_EXTEND_MASK            0x40

#define     SIM_DEFAULT_WI                     10
#define     SIM_DEFAULT_CWI                    13      /*job 100951*/
#define     SIM_DEFAULT_BWI                    4
#define     SIM_DEFAULT_EXTRA_GUARD_TIME       0
#define     SIM_DEFAULT_IFSC                   32

#if defined( UPGRADE_AD6532 )
/* as recommended by ADI */
#define     SIM_DRV_MAX_ALLOWABLE_NUDGES       4
#define     SIM_DRV_NUM_CHECK_ITERATIONS       2000
#endif

/****************************************************************************
 * Types
 ****************************************************************************/
typedef enum SimClockRateFactorTag
{
   SIM_CRF_372 = 372,
   SIM_CRF_512 = 512,
   SIM_CRF_558 = 558,
   SIM_CRF_744 = 744,
   SIM_CRF_768 = 768,
   SIM_CRF_1024 = 1024,
   SIM_CRF_1116 = 1116,
   SIM_CRF_1536 = 1536,
   SIM_CRF_1488 = 1488,
   SIM_CRF_1860 = 1860,
   SIM_CRF_2048 = 2048

}SimClockRateFactor;

typedef enum SimBaudRateFactorTag
{
   SIM_BRF_1 = 1,
   SIM_BRF_2 = 2,
   SIM_BRF_4 = 4,
   SIM_BRF_8 = 8,
   SIM_BRF_12 = 12,
   SIM_BRF_16 = 16,
   SIM_BRF_20 = 20,
   SIM_BRF_32 = 32

}SimBaudRateFactor;

typedef enum SimUiccTransmissionProtocolTag
{
   SIM_UICC_T0 = 0,
   SIM_UICC_T1 = 1,
   SIM_UICC_T_UNKNOWN

}SimUiccTransmissionProtocol;
typedef Int16 SimL1TimerNum;

typedef enum  SimClockFreqTag
{
    SIM_FREQ_1_625_MHZ = 0,
    SIM_FREQ_3_25_MHZ  = 1,
    SIM_FREQ_4_MHZ     = 2,
    SIM_FREQ_5_MHZ     = 3,
    SIM_FREQ_6_MHZ     = 4,
    SIM_FREQ_7_5_MHZ   = 5,
    SIM_FREQ_8_MHZ     = 6,
    SIM_FREQ_10_MHZ    = 7,
    SIM_FREQ_12_MHZ    = 8,
    SIM_FREQ_15_MHZ    = 9,
    SIM_FREQ_16_MHZ    = 10,
    SIM_FREQ_20_MHZ    = 11
}
SimClockFreq;

#if defined (UPGRADE_SIM_EXPLICIT_CASE)
typedef enum SimCommandCaseTag
{
    CASE_NOT_SET = 0,
    CASE_1       = 1,
    CASE_2       = 2,
    CASE_3       = 3,
    CASE_4       = 4,
    CASE_PPS     = 5
}
SimCommandCase;
#endif

/* For table 7 in ISO/IEC 7816-3:1997 */
typedef struct crFactorParamTag
{
   Int8                 ta1;            /* 4 uppermost bits of TA1 is FI */
   SimClockRateFactor   crFactor;       /* Fi */
   SimClockFreq         freq;           /* job 100951: f (max) */

} crFactorParam;

/* For table 8 in ISO/IEC 7816-3:1997 */
typedef struct brFactorParamTag
{
   Int8                 ta1;            /* 4 lowermost bits of TA1 is DI */
   SimBaudRateFactor    brFactor;       /* Di */
} brFactorParam;


/* For F/D ratio table */
typedef struct ppsParamTag
{
   SimClockRateFactor   crFactor; /* F */
   SimBaudRateFactor    brFactor; /* D */
   Int8                 ta1;
} ppsParam;



typedef enum
{
    SIM_OVERRUN_ERROR,
    SIM_RECEIVE_ERROR,
    SIM_TRANSMIT_ERROR,
    SIM_NOT_RESPONDING
}
SimError;

typedef enum
{
    SIM_5_VOLTS,
    SIM_3_VOLTS,
    SIM_1_8_VOLTS,
    SIM_NUM_VOLTAGE_LEVELS,
    SIM_VOLTAGE_UNKNOWN = 0xFF
}
SimVoltage;

typedef enum
{
    SIM_RX_INT_OVERRUN,
    SIM_RX_INT_PARITY,
    SIM_TX_INT_ERROR,
    SIM_TX_INT_COMPLETE,
    SIM_RX_INT_COMPLETE,
    SIM_RX_INT_NULL_RECEIVED,
    SIM_RX_INT_TX_PENDING,
    SIM_RX_INT_BAD61,
    SIM_RX_SEND_RECEIVE_READY,
    SIM_TX_CONTINUE_CHAIN,
    SIM_RX_BLOCK_ERROR,
    SIM_TX_RESEND_BLOCK,
    SIM_RX_SUPER_REQ,
    SIM_RX_SUPER_RES,
    SIM_RX_SUPER_RESYNC,  /* For USIM COT test 7.3.12 */
    SIM_RX_INT_INVALID_PROC
}
SimIntResponse;

#if defined (ENABLE_DUAL_SIM_SOLUTION) || defined(UPGRADE_TWGGE_NRW)
#define MAX_SIM_HOLDERS 2
typedef enum SimHolderTypeTag
{
   SIM_0 = 0,
   SIM_1 = 1,
   SIM_UNSELECTED
}SimHolderType;
#endif

typedef Int8 L1siSmallMsgData[SIM_MAX_SMALL_MSG_SIZE];
typedef Int8 L1siMediumMsgData[SIM_MAX_MEDIUM_MSG_SIZE];
typedef Int8 L1siMsgData[SIM_MAX_MSG_SIZE];
typedef Int8 L1siExtendMsgData[SIM_MAX_EXTEND_MSG_SIZE];

typedef struct SimSmallDataBlockTag
{
/** \assoc ARRAY \ref data */
    Int16                           length;
    L1siSmallMsgData                data;
}
SimSmallDataBlock;

typedef struct SimMediumDataBlockTag
{
/** \assoc ARRAY \ref data */
    Int16                           length;
    L1siMediumMsgData               data;
}
SimMediumDataBlock;

typedef struct SimDataBlockTag
{
/** \assoc ARRAY \ref data */
    Int16                           length;
    L1siMsgData                     data;
}
SimDataBlock;

typedef struct SimExtendDataBlockTag
{
/** \assoc ARRAY \ref data */
    Int16                           length;
    L1siExtendMsgData               data;
}
SimExtendDataBlock;

/****************************************************************************
 * Signal types
 ****************************************************************************/

/** \defgroup SigL1Si L1SI Signaling Interface
 * \ingroup PrdGSMProtoStack
 * \ingroup Sig
 * Interface between the SIM driver and SIM manager.
 * This Interface is used by the SIM driver and SIM manager to convey the
 * physical status of the SIM or UICC as well as to transfer data and
 * commands to and from the SIM device.
 * @{
 *
 * Message sequence chart for SIM/UICC Card Initialisation Sequence
 *
 *\msc
 * msc {
 *  "USIM manager","USIM driver","SIM/UICC";
 *
 *  --- [label="Power On"];
 *  "USIM manager" -> "USIM driver" [label="SIG_INITIALISE"];
 *  "USIM manager" <- "USIM driver" [label="L1siConfigInd"];
 *  "USIM manager" <- "USIM driver" [label="L1siCardRemovedInd"];
 *  --- [label="Card Insertion"];
 *                    "USIM driver" => "SIM/UICC"[label="Resets the card"];
 *                    "USIM driver" <= "SIM/UICC"[label="Answer to reset data"];
 *  "USIM manager" <- "USIM driver" [label="L1siCardPresentInd"];
 *  "USIM manager" <- "USIM driver" [label="L1siDataInd(ATR data)"];
 *  "USIM manager" -> "USIM driver" [label="L1siDataReq(PPSrequest)"];
 *                    "USIM driver" => "SIM/UICC"[label="PPS request"];
 *                    "USIM driver" <= "SIM/UICC"[label="PPS response"];
 *  "USIM manager" <- "USIM driver" [label="L1siDataInd(PPS response)"];
 *  "USIM manager" -> "USIM driver" [label="L1siConfigure Req"];
 *  "USIM manager" -> "USIM driver" [label="L1siDataReq(C-APDU)"];
 *                    "USIM driver" => "SIM/UICC"[label="command"];
 *                    "USIM driver" <= "SIM/UICC"[label="Response from SIM"];
 *  "USIM manager" <- "USIM driver" [label="L1siDataInd(R-APDU)"];
 * }
 *\endmsc
 *
 *
 *
 * Message sequence chart for L1SI Interface Signal Exchange
 *
 *\msc
 * msc {
 *  "USIM manager","USIM driver";
 *
 *  "USIM manager" -> "USIM driver" [label="L1siDatatReq"];
 *  "USIM manager" <- "USIM driver" [label="L1siDataInd"];
 *  "USIM manager" <- "USIM driver" [label="L1siConfigInd"];
 *  "USIM manager" -> "USIM driver" [label="L1siConfigReq"];
 *  "USIM manager" <- "USIM driver" [label="L1siErrorInd"];
 *  "USIM manager" -> "USIM driver" [label="L1siInterruptReq"];
 *  "USIM manager" -> "USIM driver" [label="L1siResetReq"];
 *  "USIM manager" -> "USIM driver" [label="L1siRejectReq"];
 *  "USIM manager" -> "USIM driver" [label="L1siIdleReq"];
 *  "USIM manager" <- "USIM driver" [label="L1siCardPresentInd"];
 *  "USIM manager" <- "USIM driver" [label="L1siCardRemovedInd"];
 * }
 *\endmsc
 *
 *
 *
 */


typedef struct L1siSmallDataReqTag
{
    Int8                            reference;
    Int16                           expectedRespLength;
#if defined (UPGRADE_SIM_EXPLICIT_CASE)
    SimCommandCase                  commandCase;
#endif
    SimSmallDataBlock               dataBlock;
}
L1siSmallDataReq;

typedef struct L1siMediumDataReqTag
{
    Int8                            reference;
    Int16                           expectedRespLength;
#if defined (UPGRADE_SIM_EXPLICIT_CASE)
    SimCommandCase                  commandCase;
#endif
    SimMediumDataBlock              dataBlock;
}
L1siMediumDataReq;

typedef struct L1siLargeDataReqTag
{
    Int8                            reference;
    Int16                           expectedRespLength;
#if defined (UPGRADE_SIM_EXPLICIT_CASE)
    SimCommandCase                  commandCase;
#endif
    SimDataBlock                    dataBlock;
}
L1siLargeDataReq;


/** UM to Drvr - Request data from the SIM/UICC card.
 * This is sent by the USIM manager to the Driver task to request data from the
 * SIM/UICC card. In the case where a GSM SIM card is inserted inside the mobile,
 * this consists of a data block containing a command coded as required by
 * GSM 11.11, section 9 and TS 51.011 section 9. In the case where a UICC is
 * inserted inside the mobile, this consists of a data block containing a C-APDU
 * coded as described in TS 102 221 section 11.
 * Note that this implies that the application layer resides in the USIM manager,
 * and the transport layer resides in the USIM drivers. The L1siDataReq can also
 * convey a PPS request. The reference parameter is to allow replies to be matched
 * to the original requests, although only one such request can be active at a
 * given time.
 *
 * \note When IMPROVE_L1SI_LOGABILITY is defined, three different signals
 *       may be sent L1SiSmallDataReq, L1SiMediumDataReq, L1SiLargeDataReq
 *       depending on how much data is being requested).
 */
typedef struct L1siDataReqTag
{
    Int8                            reference;   /**< Message identifier */
    Int16                           expectedRespLength;  /**< Expected length of data to be returned */
#if defined (UPGRADE_SIM_EXPLICIT_CASE)
    SimCommandCase                  commandCase;
#endif
    SimDataBlock                    dataBlock; /**< Contains SIM/UICC command */
}
L1siDataReq;

typedef struct L1siSmallDataIndTag
{
    Int8                            reference;
    SimSmallDataBlock               dataBlock;
}
L1siSmallDataInd;

typedef struct L1siMediumDataIndTag
{
    Int8                            reference;
    SimMediumDataBlock              dataBlock;
}
L1siMediumDataInd;

typedef struct L1siLargeDataIndTag
{
    Int8                            reference;
    SimDataBlock                    dataBlock;
}
L1siLargeDataInd;

/** Indication that data has been received from the SIM card.
 * This is sent by the SIM/UICC Driver task to the USIM manager to indicate
 * that data has been received from the card. Normally this signal is only
 * sent in response to a request from the USIM manager, but at card insertion
 * it will be sent with the received 'Answer To Reset' data.
 */
typedef struct L1siDataIndTag
{
    Int8                            reference; /**< Message identifier  */
    SimDataBlock                    dataBlock; /**< Contains data received from SIM/UICC card */
}
L1siDataInd;


/** UM to Drvr - Request configuration of the clock signal and other parameters.
 * Sets other parameters (such as the Clock rate and Baud rate factor and transmission
 * protocols related parameters.)
 * This is sent by the USIM manager to the USIM Driver task to configure the clock signal
 * to the SIM/UICC card and other parameters. This signal may need to be sent several
 * times at initialisation:
 * An L1siConfigReq should be always be sent after the PPS procedure to configure the
 * parameters related to the transmission protocol T=0 or T=1, and eventually other parameters
 * if available. The GSM SIM card supplies information on the required clock configuration
 * in the response data obtained after selecting the GSM directory. In this case, a
 * L1siConfigReq will also be sent after selecting DF GSM to configure the clock.
 * The L1siConfigReq Work waiting time is an optional parameter in the ATR.
 * If the SIM/UICC clock can be stopped, this will be done by an L1siIdleReq
 */
typedef struct L1siConfigReqTag
{
    /** Frequency to set SIM/UICC card clock to */
    SimClockFreq                    clockFreq;
    /** Baud rate adjustment factor */
    SimBaudRateFactor               baudRateFactor;
    /** Clock rate conversion factor */
    SimClockRateFactor              clockRateFactor;
    /*clock info*/
    /**< Required clock line state when clock stopped TRUE = clock line stops high */
    Boolean                         clockOffHigh;
    /** Whether or not SIM/UICC clock can be stopped */
    Boolean                         clockOffAllowed;
    /** Maximum time between bytes received from SIM/UICC (work waiting time)*/
    Int8                            WI;
    /** Transmission protocol.
     * This indicates whether T=0 or T=1.
     */
    SimUiccTransmissionProtocol     transProtocol;
    /** parameter N in the ATR */
    Int8                            extraGuardTime;
    /** Information field size receivable by the card */
    Int8                            cardInfoFieldSize;
    /** Parameter to determine Character Waiting Time; maximum delay between characters */
    Int8                            charWaitingInt;
    /** Parameter to determine Block Waiting Time.
     * This is the maximum allowed time between the card receiving a block and
     * transmitting its response.
     */
    Int8                            blockWaitingInt;
}
L1siConfigReq;


/** Indication of SIM Driver configuration and capabilities.
 * This signal is sent when SIG_INITIALISE is received after mobile power-up,
 * and is sent to the SIM Manager to indicate SIM Driver configuration options
 * which the SIM Manager has to be aware of.
 * The signal lists the supported voltages and is used at card initialisation
 * in case the SIM/UICC needs to be reset to a different voltage.  The number
 * of possible voltage levels (SIM_NUM_VOLTAGE_LEVELS) is currently set to three.
 *
 * \note 3G terminals must support High Speed SIM, so the highSpeedSimSupported
 *       flag should always be set to TRUE for a 3G device.
 *
 * \note The disableTequal1 flag will force USIM manager to select T=0 instead
 *       of T=1, even if the SIM/USIM supports T=1.  3G terminals must support
 *       T=1, so in theory this flag should always be set to FALSE.  However,
 *       it may be useful to set this flag to TRUE for debugging purposes.
 */
#if !defined (ENABLE_DUAL_SIM_SOLUTION)
typedef struct L1siConfigIndTag
{
    /** Supported SIM voltages.
     * This allows SIM Manager to decide which voltage to use depending on the
     * SIM and the SIM Driver's capabilities.
     */
    Boolean                         supportedVoltages[SIM_NUM_VOLTAGE_LEVELS];
    /** Whether the drivers support High Speed SIM */
    Boolean                         highSpeedSimSupported;
    /** Flag to disable T = 1 protocol.
     * Even if the USIM supports it, this will force the USIM manager to
     * select T=0.
     */
    Boolean                         disableTequal1;
}
L1siConfigInd;
#endif



/** SIM socket dynamic change from NVRAM parameter.      
 */

/** Currently, 3 Sockets are available.      
 */
typedef enum SimSocketTypeTag
{
   SIM_SOCKET_0 = 0,
   SIM_SOCKET_1,
   SIM_SOCKET_2,
   SIM_SOCKET_MAX
} SimSocketType;

typedef struct L1siSimSocketSelectTag
{
    /* the SIM Socket to be selected */
    SimSocketType    simSocket;
}L1siSimSocketSelect;





/** Drvr to UM - "Unsolicited" signal sent when an error occurs accessing SIM/UICC card.
 * This is sent by the SIM/UICC Driver task to the USIM manager to indicate that an
 * error has occurred while accessing the SIM/UICC card
 */
typedef struct L1siErrorIndTag
{
    SimError                        simError;  /**< Type of error that has occurred */
}
L1siErrorInd;


/** Drvr to UM - Sent when a card insertion detected.
 * This is sent by the SIM/UICC Driver task to the USIM manager to indicate
 * that a card has been inserted and that some 'Answer To Reset' (ATR) data
 * has been received. It also gives the current operating voltage of the
 * SIM/UICC interface.
 */
typedef struct L1siCardPresentIndTag
{
    SimVoltage                      simVoltage; /**< SIM/UICC voltage to be used */
    SimUiccTransmissionProtocol     protocol;
}
L1siCardPresentInd;


typedef EmptySignal L1siCardRemovedInd;

/** UM to Drvr - Used by the SIM Manager to request resetting of the SIM card.
 * This is sent by the USIM manager to the SIM/UICC Driver task to request
 * it to reset the card. It contains the new voltage to reactivate the card at.
 */
typedef struct L1siResetReqTag
{
    /** Required SIM voltage level at which to reactivate the card. */
    SimVoltage                      simVoltage;
}
L1siResetReq;


typedef EmptySignal L1siRejectReq;


typedef EmptySignal L1siIdleReq;


/** Internal to the driver, used to indicate completion of transmit or receive.
 * This signal is used internally to the SIM Driver to indicate transfer phase
 * completion, reception completion and transmit/receive errors.
 *
 * \note This is an internal signal sent by the SIM/UICC interrupt to the
 *       SIM/UICC Driver task to inform it of an event in an interrupt handler.
 *       It should not be sent externally to the SIM/UICC Driver except for
 *       testing purposes.
 */
typedef struct L1siInterruptReqTag
{
    /** Event that has occurred in the interrupt handler */
    SimIntResponse                 intResponse;
}
L1siInterruptReq;

#if defined (UPGRADE_SIM_USE_DMA)
typedef struct L1siStopTimerReqTag
{
    SimL1TimerNum                  l1TimerNumToStop;
}
L1siStopTimerReq;
#else
typedef EmptySignal L1siStopTimerReq;
#endif /* (UPGRADE_SIM_USE_DMA) */

typedef struct L1siChangeTargetTaskIdReqTag
{
  TaskId    requestTaskId;
  TaskId    targetTaskId;
} L1siChangeTargetTaskIdReq;

typedef struct L1siChangeTargetTaskIdCnfTag
{
  Boolean   success;
} L1siChangeTargetTaskIdCnf;

#if defined( UPGRADE_AD6532 )
typedef struct L1siInterfaceCheckReqTag
{
  TaskId  taskId;
} L1siInterfaceCheckReq;

typedef struct L1siInterfaceCheckCnfTag
{
  Boolean ok;
  Int16   numNudgesRequired;
  Int32   iterationCount[SIM_DRV_MAX_ALLOWABLE_NUDGES];
} L1siInterfaceCheckCnf;
#else
typedef EmptySignal L1siInterfaceCheckReq;
typedef EmptySignal L1siInterfaceCheckCnf;
#endif

#if defined (ENABLE_USIM_EMU_COOLART_SCRIPTS)
typedef struct L1siSimImagPtrIndTag
{
  Int32* simImagePtr; /* For win32 PC host testing, all pointers are Int32 in size so it's fine to
                       * use to point to the SimImage object; Using SimImage* instead would create a lot
                       * of dependency on other files */
} L1siSimImagPtrInd;

typedef struct L1siSimImagPtrRspTag
{
   Boolean ok;
} L1siSimImagPtrRsp;


typedef struct L1siScriptReadyIndTag
{
   Boolean ok;
} L1siScriptReadyInd;

#endif /* ENABLE_USIM_EMU_COOLART_SCRIPTS */


#if defined (ENABLE_DUAL_SIM_SOLUTION)

typedef struct L1siSimSelectReqTag
{
    /** the SIM Holder to be selected */
    SimHolderType    simHolder;
}L1siSimSelectReq;

typedef struct L1siSimSelectCnfTag
{
     /** the selected SIM Holder*/
     SimHolderType    simHolder;
     /** the number of SIM holders present 1 or 2 */
     Int8             numSimHolders;
     /** Supported SIM voltages.
     * This allows SIM Manager to decide which voltage to use depending on the
     * SIM and the SIM Driver's capabilities.
     */
    Boolean                         supportedVoltages[SIM_NUM_VOLTAGE_LEVELS];
    /** Whether the drivers support High Speed SIM */
    Boolean                         highSpeedSimSupported;
    /** Flag to disable T = 1 protocol.
     * Even if the USIM supports it, this will force the USIM manager to
     * select T=0.
     */
    Boolean                         disableTequal1;
}L1siSimSelectCnf;

typedef EmptySignal L1siShutdownReq;

#endif /* ENABLE_DUAL_SIM_SOLUTION */

/** @} */ /* End of SigL1SI group */

#endif
/* END OF FILE */


































