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
 * \ingroup FRPRIMELOG
 * \ingroup FRPRIMELOG
 * \brief This file contains initialisation, Dumping and Logging implementation of Prime Log DSP.
 */

#ifndef _FRPRIMELOG_H_
#define _FRPRIMELOG_H_
/*******************************************************************************
** Includes
*******************************************************************************/

#include <system.h>


//#define PRIME_LOG_FLUSH_STUB

#if defined(UPGRADE_HSL_OVER_SHMCONN) && defined(UPGRADE_PRIME_LOG)
  #error "UPGRADE_HSL_OVER_SHMCONN not allowed when UPGRADE_PRIME_LOG defined"
#endif

/*******************************************************************************
** Constants
*******************************************************************************/

#define FR_PRIME_HEAD                   0xA56B 
#define FR_PRIME_HEAD_CONFLICT          0xA56C
#define FR_PRIME_HEAD_PAD               0xA56E

/** define the message size of PRIME log */
#define FR_PRIME_LOG_BUFFER_SIZE        (1024/2)

#define FR_PRIME_BUFFER_OVERWRITE       (4)

/** flag to indicate the xxxHslPrintfx is only for PrimeLog 
 * note, for primeLog, bit(13~15) of arg1 is reserved.
  */
#define LOG_PRIME_ONLY              (1LL<<63)

/** flag to indicate the xxxHslPrintfx is for PrimeLog and HSL 
  * note, for primeLog, bit(13~15) of arg1 is reserved.
  */
#define LOG_PRIME_HSL               (LOG_PRIME_ONLY>>1)

/** internal used mask, to decide if the xxxHslPrintfx writes to PrimeLog
 */
#define LOG_PRIME_MASK               (LOG_PRIME_ONLY|LOG_PRIME_HSL)

/** Mask to retrieve the filter word 
 */
#define PRIME_LOG_FILTER_MASK        (0x3fffffff)

#define  FR_PRIME_LOG_FILTER_OFFSET (32)

/** Mask for the first argument for primeLog,
  * only 13 bit is allowed to log for arg1 */
#define FR_PRIME_ARG1_MASK          (0x1fff)


/** defines the Prime Log Module offset */
typedef enum PrimeLogDspModuleOffsetTag
{
/* 0 */ PRM_GRP_OFFSET_LEV0  = FR_PRIME_LOG_FILTER_OFFSET  
/* 1 */,PRM_GRP_OFFSET_LEV1
/* 2 */,PRM_GRP_OFFSET_AUD_UL                      
/* 3 */,PRM_GRP_OFFSET_AUD_DL                      
/* 4 */,PRM_GRP_OFFSET_FR                          
/* 5 */,PRM_GRP_OFFSET_RF                          
/* 6 */,PRM_GRP_OFFSET_SLCLK                       
/* 7 */,PRM_GRP_OFFSET_ABPG                        
/* 8 */,PRM_GRP_OFFSET_GGE_LOOP                    
/* 9 */,PRM_GRP_OFFSET_GGE_TX                      
/* 10*/,PRM_GRP_OFFSET_GGE_RX                      
/* 11*/,PRM_GRP_OFFSET_GGE_TIG                     
/* 12*/,PRM_GRP_OFFSET_GGE_FBSB                    
/* 13*/,PRM_GRP_OFFSET_GGE_RSSISCAN                
/* 14*/,PRM_GRP_OFFSET_GGE_RSV
/* 15*/,PRM_GRP_OFFSET_TPH_TX     /* TDD only starts */                 
/* 16*/,PRM_GRP_OFFSET_TPH_RX                      
/* 17*/,PRM_GRP_OFFSET_TPH_CSM                     
/* 18*/,PRM_GRP_OFFSET_TL1_LOOP                    
/* 19*/,PRM_GRP_OFFSET_TL1_ARB                     
/* 20*/,PRM_GRP_OFFSET_TL1_RX                      
/* 21*/,PRM_GRP_OFFSET_TL1_TX                      
/* 22*/,PRM_GRP_OFFSET_TL1_DM                      
/* 23*/,PRM_GRP_OFFSET_TL1_CSM                     
/* 24*/,PRM_GRP_OFFSET_TL1_IFIC                    
/* 25*/,PRM_GRP_OFFSET_TL1_SCAN /* ICS/RSSISCAN */ 
/* 26*/,PRM_GRP_OFFSET_TL1_DPA                     
/* 27*/,PRM_GRP_OFFSET_TL1_UPA                     
/* 28*/,PRM_GRP_OFFSET_TL1_IQ   
/* 29*/,PRM_GRP_OFFSET_RESV_DBG1 /* debug usage, with this group, no CL is allowed to submit */
/* 30*/,PRM_GRP_OFFSET_RESV_DBG0

}PrimeLogDspModuleOffset;
/** defines the Prime Log level control */
#define PRMLG_LEV0_MASK             (1LL)   
#define PRMLG_LEV1_MASK             (2LL)

/** defines the Prime Log level control */
#define PRML_LEV0                    (1LL<<PRM_GRP_OFFSET_LEV0)   
#define PRML_LEV1                    (1LL<<PRM_GRP_OFFSET_LEV1)

#define PRMLG_AUD_UL                 (1LL<<PRM_GRP_OFFSET_AUD_UL         )
#define PRMLG_AUD_DL                 (1LL<<PRM_GRP_OFFSET_AUD_DL         )
#define PRMLG_FR                     (1LL<<PRM_GRP_OFFSET_FR             )
#define PRMLG_RF                     (1LL<<PRM_GRP_OFFSET_RF             )
#define PRMLG_SLCLK                  (1LL<<PRM_GRP_OFFSET_SLCLK          )
#define PRMLG_ABPG                   (1LL<<PRM_GRP_OFFSET_ABPG           )
#define PRMLG_GGE_LOOP               (1LL<<PRM_GRP_OFFSET_GGE_LOOP       )
#define PRMLG_GGE_TX                 (1LL<<PRM_GRP_OFFSET_GGE_TX         )
#define PRMLG_GGE_RX                 (1LL<<PRM_GRP_OFFSET_GGE_RX         )
#define PRMLG_GGE_TIG                (1LL<<PRM_GRP_OFFSET_GGE_TIG        )
#define PRMLG_GGE_FBSB               (1LL<<PRM_GRP_OFFSET_GGE_FBSB       )
#define PRMLG_GGE_RSSISCAN           (1LL<<PRM_GRP_OFFSET_GGE_RSSISCAN   )
#define PRMLG_TPH_TX                 (1LL<<PRM_GRP_OFFSET_TPH_TX         )
#define PRMLG_TPH_RX                 (1LL<<PRM_GRP_OFFSET_TPH_RX         )
#define PRMLG_TPH_CSM                (1LL<<PRM_GRP_OFFSET_TPH_CSM        )
#define PRMLG_TL1_LOOP               (1LL<<PRM_GRP_OFFSET_TL1_LOOP       )
#define PRMLG_TL1_ARB                (1LL<<PRM_GRP_OFFSET_TL1_ARB        )
#define PRMLG_TL1_RX                 (1LL<<PRM_GRP_OFFSET_TL1_RX         )
#define PRMLG_TL1_TX                 (1LL<<PRM_GRP_OFFSET_TL1_TX         )
#define PRMLG_TL1_DM                 (1LL<<PRM_GRP_OFFSET_TL1_DM         )
#define PRMLG_TL1_CSM                (1LL<<PRM_GRP_OFFSET_TL1_CSM        )
#define PRMLG_TL1_IFIC               (1LL<<PRM_GRP_OFFSET_TL1_IFIC       )
#define PRMLG_TL1_SCAN               (1LL<<PRM_GRP_OFFSET_TL1_SCAN       )
#define PRMLG_TL1_DPA                (1LL<<PRM_GRP_OFFSET_TL1_DPA        )
#define PRMLG_TL1_UPA                (1LL<<PRM_GRP_OFFSET_TL1_UPA        )
#define PRMLG_TL1_IQ                 (1LL<<PRM_GRP_OFFSET_TL1_IQ         )
#define PRMLG_RESV_DBG0              (1LL<<PRM_GRP_OFFSET_RESV_DBG0      )/* debug usage, with this group, no CL is allowed to submit */
#define PRMLG_RESV_DBG1              (1LL<<PRM_GRP_OFFSET_RESV_DBG1      )/* debug usage, with this group, no CL is allowed to submit */

/*******************************************************************************
 * Macros
 ******************************************************************************/


#if defined(FR_PLP_BUILD) && defined(UPGRADE_PRIME_LOG)

#define M_FrPrimeLogInit()                          FrPrimeLogInit()

#define M_FrPrimeLogMsg1(wordA)                     FrPrimeLogMsg1(wordA)
#define M_FrPrimeLogMsg2( wordA,  wordB)            FrPrimeLogMsg2( wordA,  wordB)
#define M_FrPrimeLogMsg3(wordA,  wordB, wordC)      FrPrimeLogMsg3(wordA,  wordB, wordC)
#define M_FrPrimeLogMsg45( wordA,  wordB, wordC,wordD,  wordE, nbWord)\
                                                    FrPrimeLogMsg45( wordA,  wordB, wordC,wordD,  wordE, nbWord)
#define M_FrPrimeLogArray16(hDR, length, src_p )    FrPrimeLogArrayFromEnd(hDR, length, src_p)

#define M_FrPrimeLogLevel0IsOn()                    ((FR_PRIME_LOG_DATA.filterMask&PRMLG_LEV0_MASK) != 0)

#define M_FrPrimeLogCheckMask(mask)                 ((((mask)>>FR_PRIME_LOG_FILTER_OFFSET) & FR_PRIME_LOG_DATA.filterMask) \
                                                            == ((mask)>>FR_PRIME_LOG_FILTER_OFFSET))

#if defined(PRIME_LOG_FLUSH_STUB)
/** To decide if the log should be print to the PrimeLog
  **/
#define M_FrPrimeLogFilter(mDFlOGiD)  FALSE

/** To flush the prime log buffer
  **/
#define M_FrPrimeLogFlushBuffer()                   FrPrimeLogFlushABStub()


#else
#define M_FrPrimeLogGetDFltFlogId(mDFlOGiD)         (  (((long long)(mDFlOGiD))>>FR_PRIME_LOG_FILTER_OFFSET)&PRIME_LOG_FILTER_MASK)

/** To decide if the log should be print to the PrimeLog
  **/
#define M_FrPrimeLogFilter(mDFlOGiD) \
    (    (((mDFlOGiD)&(LOG_PRIME_MASK)) == 0)?  \
          FALSE:   \
          (( (M_FrPrimeLogGetDFltFlogId(mDFlOGiD)&PRMLG_LEV0_MASK) != 0)? \
                ((FR_PRIME_LOG_DATA.filterMask&(M_FrPrimeLogGetDFltFlogId(mDFlOGiD)&PRMLG_LEV0_MASK)) != 0):    \
                ((FR_PRIME_LOG_DATA.filterMask&M_FrPrimeLogGetDFltFlogId(mDFlOGiD))  \
                    ==(M_FrPrimeLogGetDFltFlogId(mDFlOGiD)&PRIME_LOG_FILTER_MASK))\
           )\
    )




/** To flush the prime log buffer
  **/
#define M_FrPrimeLogFlushBuffer() FrPrimeLogFlushAB()
#endif /* PRIME_LOG_FLUSH_STUB */


#else /* FR_PLP_BUILD && UPGRADE_PRIME_LOG */

/** To decide if the log should be print to the PrimeLog
  **/
#define M_FrPrimeLogFilter(mDFlOGiD) FALSE

#define M_FrPrimeLogLevel0IsOn() FALSE
#define M_FrPrimeLogCheckMask(mask) FALSE


#define M_FrPrimeLogFlushBuffer() 

#define M_FrPrimeLogInit()

#define M_FrPrimeLogMsg1(wordA)
#define M_FrPrimeLogMsg2(wordA,  wordB)
#define M_FrPrimeLogMsg3(wordA,  wordB, wordC)
#define M_FrPrimeLogMsg45(wordA,  wordB, wordC,wordD,  wordE, nbWord)
#define M_FrPrimeLogArray16(hDR, length, src_p)  

#endif /* FR_PLP_BUILD && UPGRADE_PRIME_LOG */

/** to decide if the log should print to HSL*/
#define M_FrPrimeLogFilterHslCheck(mDFlOGiD)  ((((mDFlOGiD)&LOG_PRIME_ONLY) != LOG_PRIME_ONLY)||(((mDFlOGiD)&LOG_PRIME_HSL) == LOG_PRIME_HSL))

/** to decide if the log should print to HSL*/
#define M_FrPrimeLogGetHslMdfId(mDFlOGiD)  ((mDFlOGiD)&PRIME_LOG_FILTER_MASK)

#define M_PrimeLogArg1(arg1)                ((arg1)&FR_PRIME_ARG1_MASK)

#define M_FrPrimeLogCheckInterface(mDFlOGiD) FatalAssert(( (mDFlOGiD) & LOG_PRIME_MASK ) == 0)

/*******************************************************************************
 * Typedefs
 ******************************************************************************/

/****  Module and Level control *****
 * level 0 log will always be print out if you enable PrimeLog, regardless module id.
 * level 1~3 will be module dependent, number Module (nbModule) allowed to print 
 * will be nbModule(L3) <nbModule(L2)<nbModule(L1)
 * IQ dump will be a module, L1.
 */
typedef struct FrPrimeLogDataTag
{
    /* bit 0~3, log level mask, log level 0~3. we re-use the HslPriority_x defines for the moment 
     */
    Int32  filterMask;
    /** index to 32bit array */
    Int16  writeIdx;
    Int16  lastFlushIdx;
    /** to protect the writeIdx, use the mask to ensure it resides in the buffer 
      */
    Int16  indexMask;
#if defined(PRIME_LOG_FLUSH_STUB)
    Int16  flushIndex;
#endif

    Int16  mWrEndIdx;
    /** to remember the if any buffer waiting write (for low priority task)
      * set on write, clear on finish
      */
    Int16  mMsgLoggingFlag;
    Int8   mMsgIdx;

}FrPrimeLogData;





/*******************************************************************************
** Defines
*******************************************************************************/




#if defined(FR_PLP_BUILD) && defined(UPGRADE_PRIME_LOG)

#define FR_PRIME_LOG_DATA   frPrimeLogData

/*******************************************************************************
** Global Data
*******************************************************************************/
extern FrPrimeLogData frPrimeLogData;

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
* Function    : FrPrimeLogMsg1
* Group       : T1, DSP
* Parameter   : hDR, log point for output
*               wordA, parameters for output
* Returns     : None
* Description : This Function is to output Prime log to buffer.
*******************************************************************************/
void FrPrimeLogMsg1(Int32 wordA);



/*******************************************************************************
* Function    : FrPrimeLogMsg2
* Group       : T1, DSP
* Parameter   : hDR, log point for output
*               wordA - wordB, parameters for output
* Returns     : None
* Description : This Function is to output Prime log to buffer.
*******************************************************************************/
void FrPrimeLogMsg2(Int32 wordA, Int32 wordB);


/*******************************************************************************
* Function    : FrPrimeLogMsg3
* Group       : T1, DSP
* Parameter   : hDR, log point for output
*               wordA - wordC, parameters for output
* Returns     : None
* Description : This Function is to output Prime log to buffer.
*******************************************************************************/
void FrPrimeLogMsg3(Int32 wordA, Int32 wordB, Int32 wordC);


/*******************************************************************************
* Function    : FrPrimeLogMsg5
* Group       : T1, MCU/DSP
* Parameter   : hDR, log point for output
*               wordA - wordE, parameters for output
*               nbWord, number of valid word to print
* Returns     : None
* Description : This Function is to output Prime log to buffer.
*******************************************************************************/
void FrPrimeLogMsg45(Int32 wordA, Int32 wordB, Int32 wordC,Int32 wordD,Int32 wordE,Int32 nbWord);

void FrPrimeLogArrayFromEnd(Int16 hDR, Int16 length, Int16* src_p);

void FrPrimeLogInit(void);

void FrPrimeLogFilterConfig(Int32 filterWord0, Int32 filterWord1);

/** function to dump the PrimeLog buffer.
 * The buffer will be flushed only when the accumulate number is beyond a threshold
 */
void FrPrimeLogFlushAB(void);


/** function to dump all of the accumulated buffer.
 */
void FrPrimeLogFlushAll(void);

#if defined(PRIME_LOG_FLUSH_STUB)

void FrPrimeLogFlushABStub(void);
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FR_PLP_BUILD && UPGRADE_PRIME_LOG */


#endif /* _FRPRIMELOG_H_ */
/* END OF FILE */
