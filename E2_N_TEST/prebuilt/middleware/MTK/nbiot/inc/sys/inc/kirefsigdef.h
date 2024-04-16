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
 * GKI header file that defines the signal definition macros for referenced 
 * data logging. 
 *
 *   SIG_DEF_1_REF       - Signal with single reference value 
 *   SIG_DEF_2_REF       - Signal with two reference values
 *   SIG_DEF_3_REF       - Signal with three reference values
 *   SIG_DEF_ARRAY_REF   - Signal with an array referenced
 *   SIG_DEF_CUSTOM_REF  - Signal which uses a custom logging function
 *
 * These are used to create the structures used in the 
 * ReferencedDataInd signal used to show the logged data in genie and
 * create the generic logging functions which actually copy the referenced
 * data into the logging signal.
 **************************************************************************/

#if SIGNAL == REF_LOGGING_FUNC_PROTO
/* Define the standard SIG_DEF to take no action so when the signal include
 * files are processed the standard signals are ignored */
#define SIG_DEF(ENUM,UNION)
#define SIG_DEF_1_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1)
#define SIG_DEF_2_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2)
#define SIG_DEF_3_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2, N_3, T_3)
#define SIG_DEF_ARRAY_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, S_1)

/* Define the macro for signal logging functions with a custom specified
 * logging function */
#define SIG_DEF_CUSTOM_REF(SIG_ID, SIG_TYPE, SIG_NAME, PAYLOAD_TYPE, LOGGING_FUNC)                  \
static Int16 SIG_NAME##_logpoint (                                                                  \
  SIG_TYPE *srcSig, SignalId srcSigId, ReferencedDataInd *refDataInd, Boolean getAllocSize);
#endif /* SIGNAL == REF_LOGGING_FUNC_PROTO */

#if SIGNAL == REF_LOGGING_FUNC
/* Define the standard SIG_DEF to take no action so when the signal include
 * files are processed the standard signals are ignored */
#define SIG_DEF(ENUM,UNION)
#define SIG_DEF_1_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1)
#define SIG_DEF_2_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2)
#define SIG_DEF_3_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2, N_3, T_3)
#define SIG_DEF_ARRAY_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, S_1)

/* Define the macro for signal logging functions with a custom specified
 * logging function */
#define SIG_DEF_CUSTOM_REF(SIG_ID, SIG_TYPE, SIG_NAME, PAYLOAD_TYPE, LOGGING_FUNC)                              \
static Int16 SIG_NAME##_logpoint (                                                                              \
  SIG_TYPE *srcSig, SignalId srcSigId, ReferencedDataInd *refDataInd, Boolean getAllocSize)                     \
{                                                                                                               \
  /* To allow complete control custom logging function is called with signal */                                 \
  return LOGGING_FUNC(srcSig, srcSigId, refDataInd, getAllocSize);                                              \
}
#endif /* SIGNAL == REF_LOGGING_FUNC */

#if SIGNAL == REF_LOGGING_SWITCH

/* Define the standard SIG_DEF to take no action so when the signal include
 * files are processed the standard signals are ignored */
#define SIG_DEF(ENUM,UNION)

/* Slightly different case statements are required when running using Geniexec.c */
#if !defined(GENIEXEC_C)

/* Macro for creating case statement for single reference value case */
#define SIG_DEF_1_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1)                                                   \
    case SIG_ID:                                                                                              \
        /* Attempt to allocate a logging block for first reference data */                                    \
        KiTtiCreateLoggingBlock (offsetof(ReferencedDataInd, payload) + sizeof(T_1),  &logBlock, FALSE);      \
        /* If the logging block is allocated call logging function again to copy into block */                \
        if (logBlock != PNULL)                                                                                \
        {                                                                                                     \
          /* Set header value */                                                                              \
          logBlock->header.format = KI_LOGGED_SIGNAL_FORMAT;                                                  \
          /* Set directives same as source signal */                                                          \
          logBlock->record.directives = block->record.directives;                                             \
          /* Set signal id */                                                                                 \
          logBlock->record.id = SIG_REF_DATA_IND;                                                             \
          /* Set length value (is return value of second call to logging func */                              \
          logBlock->record.length = offsetof(ReferencedDataInd, payload) + sizeof(T_1);                       \
          /* Check body size and copy into logging block */                                                   \
          memcpy(&((ReferencedDataInd *)&logBlock->record.body)->payload,                                     \
                 ((SIG_TYPE *)body)->N_1, sizeof(T_1));                                                       \
          /* Set other Referenced Data Ind signal values */                                                   \
          ((ReferencedDataInd *)&logBlock->record.body)->refSignalId = SIG_ID;                                \
          ((ReferencedDataInd *)&logBlock->record.body)->addressRefData =                                     \
                                       (Int32)((SIG_TYPE *)body)->N_1;                                        \
          ((ReferencedDataInd *)&logBlock->record.body)->id = ASSOC_##N_1##_##T_1;                            \
          /* Send logged signal (fills in time fields) */                                                     \
          KiTtiSendLoggedSignalStructure(logBlock);                                                           \
        }                                                                                                     \
        else                                                                                                  \
        {                                                                                                     \
          /* Cant allocate the memory do not continue logging */                                              \
        }                                                                                                     \
        break;

/* Macro for creating case statement for dual reference value case */
#define SIG_DEF_2_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2)                                         \
    case SIG_ID:                                                                                              \
        /* Attempt to allocate a logging block for first reference data */                                    \
        KiTtiCreateLoggingBlock (offsetof(ReferencedDataInd, payload) + sizeof(T_1),  &logBlock, FALSE);      \
        /* If the logging block is allocated call logging function again to copy into block */                \
        if (logBlock != PNULL)                                                                                \
        {                                                                                                     \
          /* Set header value */                                                                              \
          logBlock->header.format = KI_LOGGED_SIGNAL_FORMAT;                                                  \
          /* Set directives same as source signal */                                                          \
          logBlock->record.directives = *directives;                                                          \
          /* Set signal id */                                                                                 \
          logBlock->record.id = SIG_REF_DATA_IND;                                                             \
          /* Set length value (is return value of second call to logging func */                              \
          logBlock->record.length = offsetof(ReferencedDataInd, payload) + sizeof(T_1);                       \
          /* Check body size and copy into logging block */                                                   \
          memcpy(&((ReferencedDataInd *)&logBlock->record.body)->payload,                                     \
                 ((SIG_TYPE *)body)->N_1, sizeof(T_1));                                                       \
          /* Set other Referenced Data Ind signal values */                                                   \
          ((ReferencedDataInd *)&logBlock->record.body)->refSignalId = SIG_ID;                                \
          ((ReferencedDataInd *)&logBlock->record.body)->addressRefData =                                     \
                                               (Int32)((SIG_TYPE *)body)->N_1;                                \
          ((ReferencedDataInd *)&logBlock->record.body)->id = ASSOC_##N_1##_##T_1;                            \
          /* Send logged signal (fills in time fields) */                                                     \
          KiTtiSendLoggedSignalStructure(logBlock);                                                           \
          /* Attempt to allocate a logging block for second reference data */                                 \
          KiTtiCreateLoggingBlock (offsetof(ReferencedDataInd, payload) + sizeof(T_2),  &logBlock, FALSE);    \
          /* If the logging block is allocated call logging function again to copy into block */              \
          if (logBlock != PNULL)                                                                              \
          {                                                                                                   \
            /* Set header value */                                                                            \
            logBlock->header.format = KI_LOGGED_SIGNAL_FORMAT;                                                \
            /* Set directives same as source signal */                                                        \
            logBlock->record.directives = *directives;                                                        \
            /* Set signal id */                                                                               \
            logBlock->record.id = SIG_REF_DATA_IND;                                                           \
            /* Set length value (is return value of second call to logging func */                            \
            logBlock->record.length = offsetof(ReferencedDataInd, payload) + sizeof(T_2);                     \
            /* Check body size and copy into logging block */                                                 \
            memcpy(&((ReferencedDataInd *)&logBlock->record.body)->payload,                                   \
                   ((SIG_TYPE *)body)->N_2, sizeof(T_2));                                                     \
            /* Set other Referenced Data Ind signal values */                                                 \
            ((ReferencedDataInd *)&logBlock->record.body)->refSignalId = SIG_ID;                              \
            ((ReferencedDataInd *)&logBlock->record.body)->addressRefData =                                   \
                                                             (Int32)((SIG_TYPE *)body)->N_2;                  \
            ((ReferencedDataInd *)&logBlock->record.body)->id = ASSOC_##N_2##_##T_2;                          \
            /* Send logged signal (fills in time fields) */                                                   \
            KiTtiSendLoggedSignalStructure(logBlock);                                                         \
          }                                                                                                   \
          else                                                                                                \
          {                                                                                                   \
            /* Cant allocate the memory do not continue logging */                                            \
          }                                                                                                   \
        }                                                                                                     \
        else                                                                                                  \
        {                                                                                                     \
          /* Cant allocate the memory do not continue logging */                                              \
        }                                                                                                     \
        break;

/* Macro for creating case statement for triple reference value case */
#define SIG_DEF_3_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2, N_3, T_3)                                 \
    case SIG_ID:                                                                                                \
        /* Attempt to allocate a logging block for first reference data */                                      \
        KiTtiCreateLoggingBlock (offsetof(ReferencedDataInd, payload) + sizeof(T_1),  &logBlock, FALSE);        \
        /* If the logging block is allocated call logging function again to copy into block */                  \
        if (logBlock != PNULL)                                                                                  \
        {                                                                                                       \
          /* Set header value */                                                                                \
          logBlock->header.format = KI_LOGGED_SIGNAL_FORMAT;                                                    \
          /* Set directives same as source signal */                                                            \
          logBlock->record.directives = *directives;                                                            \
          /* Set signal id */                                                                                   \
          logBlock->record.id = SIG_REF_DATA_IND;                                                               \
          /* Set length value (is return value of second call to logging func */                                \
          logBlock->record.length = offsetof(ReferencedDataInd, payload) + sizeof(T_1);                         \
          /* Check body size and copy into logging block */                                                     \
          memcpy(&((ReferencedDataInd *)&logBlock->record.body)->payload,                                       \
                 ((SIG_TYPE *)body)->N_1, sizeof(T_1));                                                         \
          /* Set other Referenced Data Ind signal values */                                                     \
          ((ReferencedDataInd *)&logBlock->record.body)->refSignalId = SIG_ID;                                  \
          ((ReferencedDataInd *)&logBlock->record.body)->addressRefData =                                       \
                                               (Int32)((SIG_TYPE *)body)->N_1;                                  \
          ((ReferencedDataInd *)&logBlock->record.body)->id = ASSOC_##N_1##_##T_1;                              \
          /* Send logged signal (fills in time fields) */                                                       \
          KiTtiSendLoggedSignalStructure(logBlock);                                                             \
          /* Attempt to allocate a logging block for second reference data */                                   \
          KiTtiCreateLoggingBlock (offsetof(ReferencedDataInd, payload) + sizeof(T_2),  &logBlock, FALSE);      \
          /* If the logging block is allocated call logging function again to copy into block */                \
          if (logBlock != PNULL)                                                                                \
          {                                                                                                     \
            /* Set header value */                                                                              \
            logBlock->header.format = KI_LOGGED_SIGNAL_FORMAT;                                                  \
            /* Set directives same as source signal */                                                          \
            logBlock->record.directives = block->record.directives;                                             \
            /* Set signal id */                                                                                 \
            logBlock->record.id = SIG_REF_DATA_IND;                                                             \
            /* Set length value (is return value of second call to logging func */                              \
            logBlock->record.length = offsetof(ReferencedDataInd, payload) + sizeof(T_2);                       \
            /* Check body size and copy into logging block */                                                   \
            memcpy(&((ReferencedDataInd *)&logBlock->record.body)->payload,                                     \
                   ((SIG_TYPE *)body)->N_2, sizeof(T_2));                                                       \
            /* Set other Referenced Data Ind signal values */                                                   \
            ((ReferencedDataInd *)&logBlock->record.body)->refSignalId = SIG_ID;                                \
            ((ReferencedDataInd *)&logBlock->record.body)->addressRefData =                                     \
                                                 (Int32)((SIG_TYPE *)body)->N_2;                                \
            ((ReferencedDataInd *)&logBlock->record.body)->id = ASSOC_##N_2##_##T_2;                            \
            /* Send logged signal (fills in time fields) */                                                     \
            KiTtiSendLoggedSignalStructure(logBlock);                                                           \
            /* Attempt to allocate a logging block for third reference data */                                  \
            KiTtiCreateLoggingBlock (offsetof(ReferencedDataInd, payload) + sizeof(T_3),  &logBlock, FALSE);    \
            /* If the logging block is allocated call logging function again to copy into block */              \
            if (logBlock != PNULL)                                                                              \
            {                                                                                                   \
              /* Set header value */                                                                            \
              logBlock->header.format = KI_LOGGED_SIGNAL_FORMAT;                                                \
              /* Set directives same as source signal */                                                        \
              logBlock->record.directives = *directives;                                                        \
              /* Set signal id */                                                                               \
              logBlock->record.id = SIG_REF_DATA_IND;                                                           \
              /* Set length value (is return value of second call to logging func */                            \
              logBlock->record.length = offsetof(ReferencedDataInd, payload) + sizeof(T_3);                     \
              /* Check body size and copy into logging block */                                                 \
              memcpy(&((ReferencedDataInd *)&logBlock->record.body)->payload,                                   \
                     ((SIG_TYPE *)body)->N_3, sizeof(T_3));                                                     \
              /* Set other Referenced Data Ind signal values */                                                 \
              ((ReferencedDataInd *)&logBlock->record.body)->refSignalId = SIG_ID;                              \
              ((ReferencedDataInd *)&logBlock->record.body)->addressRefData =                                   \
                                                   (Int32)((SIG_TYPE *)body)->N_3;                              \
              ((ReferencedDataInd *)&logBlock->record.body)->id = ASSOC_##N_3##_##T_3;                          \
              /* Send logged signal (fills in time fields) */                                                   \
              KiTtiSendLoggedSignalStructure(logBlock);                                                         \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
              /* Cant allocate the memory do not continue logging */                                            \
            }                                                                                                   \
          }                                                                                                     \
          else                                                                                                  \
          {                                                                                                     \
            /* Cant allocate the memory do not continue logging */                                              \
          }                                                                                                     \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
          /* Cant allocate the memory do not continue logging */                                                \
        }                                                                                                       \
        break;

/* Macro for creating case statement for an array of reference values case */
#define SIG_DEF_ARRAY_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, S_1)                                            \
    case SIG_ID:                                                                                                \
        /* Attempt to allocate a logging block for first reference data */                                      \
        KiTtiCreateLoggingBlock (offsetof(ReferencedDataInd, payload) + (sizeof(T_1) * S_1),  &logBlock, FALSE);\
        /* If the logging block is allocated call logging function again to copy into block */                  \
        if (logBlock != PNULL)                                                                                  \
        {                                                                                                       \
          /* Set header value */                                                                                \
          logBlock->header.format = KI_LOGGED_SIGNAL_FORMAT;                                                    \
          /* Set directives same as source signal */                                                            \
          logBlock->record.directives = *directives;                                                            \
          /* Set signal id */                                                                                   \
          logBlock->record.id = SIG_REF_DATA_IND;                                                               \
          /* Set length value (is return value of second call to logging func */                                \
          logBlock->record.length = offsetof(ReferencedDataInd, payload) + (sizeof(T_1) * S_1);                 \
          /* Check body size and copy into logging block */                                                     \
          memcpy(&((ReferencedDataInd *)&logBlock->record.body)->payload,                                       \
                 ((SIG_TYPE *)body)->N_1, (sizeof(T_1) * S_1));                                                 \
          /* Set other Referenced Data Ind signal values */                                                     \
          ((ReferencedDataInd *)&logBlock->record.body)->refSignalId = SIG_ID;                                  \
          ((ReferencedDataInd *)&logBlock->record.body)->addressRefData =                                       \
                                              (Int32)((SIG_TYPE *)body)->N_1;                                   \
          ((ReferencedDataInd *)&logBlock->record.body)->id = ASSOC_ARRAY_##T_1;                                \
          /* Send logged signal (fills in time fields) */                                                       \
          KiTtiSendLoggedSignalStructure(logBlock);                                                             \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
          /* Cant allocate the memory do not continue logging */                                                \
        }                                                                                                       \
        break;

/* Macro for creating case statement for custom reference value case */
#define SIG_DEF_CUSTOM_REF(SIG_ID, SIG_TYPE, SIG_NAME, PAYLOAD_TYPE, LOGGING_FUNC)                              \
    case SIG_ID:                                                                                                \
        /* Get allocation size */                                                                               \
        loggingBlockAllocationSize = SIG_NAME##_logpoint((SIG_TYPE *)body, signalId, PNULL, TRUE);              \
        /* Add an Int32 +3 memory guard block to allocation size (the +3 is to 32bit align guard block) */      \
        loggingBlockAllocationSize += (sizeof(Int32) + 0x03);                                                   \
        /* Mask off last two bits of allocation size to make the end 32 bit aligned */                          \
        loggingBlockAllocationSize &= ~0x03;                                                                    \
        /* Attempt to allocate a logging block for first reference data */                                      \
        KiTtiCreateLoggingBlock (loggingBlockAllocationSize,  &logBlock, FALSE);                                \
        /* If the logging block is allocated call logging function again to copy into block */                  \
        if (logBlock != PNULL)                                                                                  \
        {                                                                                                       \
          /* Add the guard block value to end of allocation (4 bytes 0xCA 0xCE 0xCA 0xCE) */                    \
          *(Int32 *)(&((Int8 *)&logBlock->record.body)[loggingBlockAllocationSize - sizeof(Int32)]) = 0xCACECACE; \
          /* Set header value */                                                                                \
          logBlock->header.format = KI_LOGGED_SIGNAL_FORMAT;                                                    \
          /* Set directives same as source signal */                                                            \
          logBlock->record.directives = *directives;                                                            \
          /* Set signal id */                                                                                   \
          logBlock->record.id = SIG_REF_DATA_IND;                                                               \
          /* Set length value (is return value of second call to logging func */                                \
          logBlock->record.length =                                                                             \
            SIG_NAME##_logpoint(                                                                                \
              (SIG_TYPE *)body, signalId, (ReferencedDataInd *)&logBlock->record.body, FALSE);                  \
          /* Check the guard block at end of allocation (4 bytes 0xCA 0xCE 0xCA 0xCE) */                        \
          if (*(Int32 *)(&((Int8 *)&logBlock->record.body)[loggingBlockAllocationSize - sizeof(Int32)]) != 0xCACECACE) \
          {                                                                                                     \
             FatalFail("CST REF LG MEM GURD BLK FAIL");                                                         \
          }                                                                                                     \
          /* Send logged signal (fills in time fields) */                                                       \
          KiTtiSendLoggedSignalStructure(logBlock);                                                             \
          /* Set Referenced Data Ind signal values */                                                           \
          ((ReferencedDataInd *)&logBlock->record.body)->refSignalId = SIG_ID;                                  \
          ((ReferencedDataInd *)&logBlock->record.body)->id = ASSOC_CUSTOM_##PAYLOAD_TYPE;                      \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
          /* Cant allocate the memory do not continue logging */                                                \
        }                                                                                                       \
        break;              

#else /* GENIEXEC_C */

/* Macro for creating case statement for single reference value case */
#define SIG_DEF_1_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1)                                                     \
    case SIG_ID:                                                                                                \
          /* Attempt to allocate a logging block (signal buffer) for first reference data */                    \
          KiCreateSignal (SIG_REF_DATA_IND, offsetof(ReferencedDataInd, payload) + sizeof(T_1),  &logBlock);                      \
          /* Check body size and copy into logging block */                                                     \
          memcpy(&((ReferencedDataInd *)logBlock.sig)->payload, ((SIG_TYPE *)signal->sig)->N_1, sizeof(T_1));  \
          /* Set other Referenced Data Ind signal values */                                                     \
          ((ReferencedDataInd *)logBlock.sig)->refSignalId = SIG_ID;                                            \
          ((ReferencedDataInd *)logBlock.sig)->addressRefData = (Int32)((SIG_TYPE *)signal->sig)->N_1;         \
          ((ReferencedDataInd *)logBlock.sig)->id = ASSOC_##N_1##_##T_1;                                        \
          /* Send the logging block signal */                                                                   \
          KiLogSignal (&logBlock, TRUE);                                                                        \
        break;

/* Macro for creating case statement for dual reference value case */
#define SIG_DEF_2_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2)                                           \
    case SIG_ID:                                                                                                \
          /* Attempt to allocate a logging block (signal buffer) for first reference data */                    \
          KiCreateSignal (SIG_REF_DATA_IND, offsetof(ReferencedDataInd, payload) + sizeof(T_1),  &logBlock);                      \
          /* Check body size and copy into logging block */                                                     \
          memcpy(&((ReferencedDataInd *)logBlock.sig)->payload, ((SIG_TYPE *)signal->sig)->N_1, sizeof(T_1));  \
          /* Set other Referenced Data Ind signal values */                                                     \
          ((ReferencedDataInd *)logBlock.sig)->refSignalId = SIG_ID;                                            \
          ((ReferencedDataInd *)logBlock.sig)->addressRefData = (Int32)((SIG_TYPE *)signal->sig)->N_1;         \
          ((ReferencedDataInd *)logBlock.sig)->id = ASSOC_##N_1##_##T_1;                                        \
          /* Send the logging block signal */                                                                   \
          KiLogSignal (&logBlock, TRUE);                                                                        \
          /* Attempt to allocate a logging block (signal buffer) for first reference data */                    \
          KiCreateSignal (SIG_REF_DATA_IND, offsetof(ReferencedDataInd, payload) + sizeof(T_2),  &logBlock);                      \
          /* Check body size and copy into logging block */                                                     \
          memcpy(&((ReferencedDataInd *)logBlock.sig)->payload, ((SIG_TYPE *)signal->sig)->N_2, sizeof(T_2));  \
          /* Set other Referenced Data Ind signal values */                                                     \
          ((ReferencedDataInd *)logBlock.sig)->refSignalId = SIG_ID;                                            \
          ((ReferencedDataInd *)logBlock.sig)->addressRefData = (Int32)((SIG_TYPE *)signal->sig)->N_2;         \
          ((ReferencedDataInd *)logBlock.sig)->id = ASSOC_##N_2##_##T_2;                                        \
          /* Send the logging block signal */                                                                   \
          KiLogSignal (&logBlock, TRUE);                                                                        \
        break;

/* Macro for creating case statement for triple reference value case */
#define SIG_DEF_3_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2, N_3, T_3)                                 \
    case SIG_ID:                                                                                                \
          /* Attempt to allocate a logging block (signal buffer) for first reference data */                    \
          KiCreateSignal (SIG_REF_DATA_IND, offsetof(ReferencedDataInd, payload) + sizeof(T_1),  &logBlock);                      \
          /* Check body size and copy into logging block */                                                     \
          memcpy(&((ReferencedDataInd *)logBlock.sig)->payload, ((SIG_TYPE *)signal->sig)->N_1, sizeof(T_1));  \
          /* Set other Referenced Data Ind signal values */                                                     \
          ((ReferencedDataInd *)logBlock.sig)->refSignalId = SIG_ID;                                            \
          ((ReferencedDataInd *)logBlock.sig)->addressRefData = (Int32)((SIG_TYPE *)signal->sig)->N_1;         \
          ((ReferencedDataInd *)logBlock.sig)->id = ASSOC_##N_1##_##T_1;                                        \
          /* Send the logging block signal */                                                                   \
          KiLogSignal (&logBlock, TRUE);                                                                        \
          /* Attempt to allocate a logging block (signal buffer) for first reference data */                    \
          KiCreateSignal (SIG_REF_DATA_IND, offsetof(ReferencedDataInd, payload) + sizeof(T_2),  &logBlock);                      \
          /* Check body size and copy into logging block */                                                     \
          memcpy(&((ReferencedDataInd *)logBlock.sig)->payload, ((SIG_TYPE *)signal->sig)->N_2, sizeof(T_2));  \
          /* Set other Referenced Data Ind signal values */                                                     \
          ((ReferencedDataInd *)logBlock.sig)->refSignalId = SIG_ID;                                            \
          ((ReferencedDataInd *)logBlock.sig)->addressRefData = (Int32)((SIG_TYPE *)signal->sig)->N_2;         \
          ((ReferencedDataInd *)logBlock.sig)->id = ASSOC_##N_2##_##T_2;                                        \
          /* Send the logging block signal */                                                                   \
          KiLogSignal (&logBlock, TRUE);                                                                        \
          /* Attempt to allocate a logging block (signal buffer) for first reference data */                    \
          KiCreateSignal (SIG_REF_DATA_IND, offsetof(ReferencedDataInd, payload) + sizeof(T_3),  &logBlock);                      \
          /* Check body size and copy into logging block */                                                     \
          memcpy(&((ReferencedDataInd *)logBlock.sig)->payload, ((SIG_TYPE *)signal->sig)->N_3, sizeof(T_3));  \
          /* Set other Referenced Data Ind signal values */                                                     \
          ((ReferencedDataInd *)logBlock.sig)->refSignalId = SIG_ID;                                            \
          ((ReferencedDataInd *)logBlock.sig)->addressRefData = (Int32)((SIG_TYPE *)signal->sig)->N_3;         \
          ((ReferencedDataInd *)logBlock.sig)->id = ASSOC_##N_3##_##T_3;                                        \
          /* Send the logging block signal */                                                                   \
          KiLogSignal (&logBlock, TRUE);                                                                        \
        break;

/* Macro for creating case statement for an array of reference values case */
#define SIG_DEF_ARRAY_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, S_1)                                            \
    case SIG_ID:                                                                                                \
          /* Attempt to allocate a logging block (signal buffer) for first reference data */                    \
          KiCreateSignal (SIG_REF_DATA_IND, offsetof(ReferencedDataInd, payload) + (sizeof(T_1) * S_1),  &logBlock);              \
          /* Check body size and copy into logging block */                                                     \
          memcpy(&((ReferencedDataInd *)logBlock.sig)->payload,                                                 \
                 ((SIG_TYPE *)signal->sig)->N_1, (sizeof(T_1) * S_1));                                         \
          /* Set other Referenced Data Ind signal values */                                                     \
          ((ReferencedDataInd *)logBlock.sig)->refSignalId = SIG_ID;                                            \
          ((ReferencedDataInd *)logBlock.sig)->addressRefData = (Int32)((SIG_TYPE *)signal->sig)->N_1;         \
          ((ReferencedDataInd *)logBlock.sig)->id = ASSOC_ARRAY_##T_1;                                          \
          /* Send the logging block signal */                                                                   \
          KiLogSignal (&logBlock, TRUE);                                                                        \
        break;

/* Macro for creating case statement for custom reference value case */
#define SIG_DEF_CUSTOM_REF(SIG_ID, SIG_TYPE, SIG_NAME, PAYLOAD_TYPE, LOGGING_FUNC)                                \
    case SIG_ID:                                                                                                  \
          /* Get allocation size */                                                                               \
          loggingBlockAllocationSize = SIG_NAME##_logpoint((SIG_TYPE *)&signal->sig, *signal->type, PNULL, TRUE); \
          /* Add an Int32 +3 memory guard block to allocation size (the +3 is to 32bit align guard block) */      \
          loggingBlockAllocationSize += (sizeof(Int32) + 0x03);                                                   \
          /* Mask off last two bits of allocation size to make the end 32 bit aligned */                          \
          loggingBlockAllocationSize &= ~0x03;                                                                    \
          /* Attempt to allocate a logging block (signal buffer) for first reference data */                      \
          KiCreateSignal (SIG_REF_DATA_IND, loggingBlockAllocationSize,  &logBlock);                              \
          /* Add the guard block value to end of allocation (4 bytes 0xCA 0xCE 0xCA 0xCE) */                      \
          *(Int32 *)(&((Int8 *)logBlock.sig)[loggingBlockAllocationSize - sizeof(Int32)]) = 0xCACECACE;           \
          /* Call the logging function again to populate the logging block */                                     \
          SIG_NAME##_logpoint(                                                                                    \
            (SIG_TYPE *)signal->sig, *signal->type, (ReferencedDataInd *)logBlock.sig, FALSE);                    \
          /* Check the guard block at end of allocation (4 bytes 0xCA 0xCE 0xCA 0xCE) */                          \
          if (*(Int32 *)(&((Int8 *)logBlock.sig)[loggingBlockAllocationSize - sizeof(Int32)]) != 0xCACECACE)      \
          {                                                                                                       \
            FatalFail("CST REF LG MEM GURD BLK FAIL");                                                            \
          }                                                                                                       \
          /* Send the logging block signal */                                                                     \
          KiLogSignal (&logBlock, TRUE);                                                                          \
        break;
    
#endif /* !GENIEXEC_C */

#endif /* SIG_DEF_TYPE == REF_LOGGING_SWITCH */

#if SIGNAL == REF_LOGGING_ASSOC
/* Define the standard SIG_DEF to take no action so when the signal include 
 * files are processed the standard signals are ignored */
#define SIG_DEF(ENUM,UNION)

/* Define the signal associations */
#define SIG_DEF_1_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1) \
     ASSOC_##N_1##_##T_1,

#define SIG_DEF_2_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2) \
     ASSOC_##N_1##_##T_1,                                                     \
     ASSOC_##N_2##_##T_2,

#define SIG_DEF_3_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2, N_3, T_3) \
     ASSOC_##N_1##_##T_1,                                                       \
     ASSOC_##N_2##_##T_2,                                                       \
     ASSOC_##N_3##_##T_3,

#define SIG_DEF_ARRAY_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, S_1) \
     ASSOC_ARRAY_##T_1,

#define SIG_DEF_CUSTOM_REF(SIG_ID, SIG_TYPE, SIG_NAME, PAYLOAD_TYPE, LOGGING_FUNC) \
     ASSOC_CUSTOM_##PAYLOAD_TYPE,
#endif /* SIGNAL == SIGNAL_ASSOC */

#if SIGNAL == REF_LOGGING_PAYLOAD
/* Define the standard SIG_DEF to take no action so when the signal include files
 * are processed the standard signals are ignored */
#define SIG_DEF(ENUM,UNION)

/* Define signal definition macros for use to create referenced payload details */
#define SIG_DEF_1_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1) \
    T_1  ref##N_1##_##T_1;

#define SIG_DEF_2_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2) \
    T_1  ref##N_1##_##T_1;                                            \
    T_1  ref##N_2##_##T_2;

#define SIG_DEF_3_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2, N_3, T_3) \
    T_1  ref##N_1##_##T_1;                                                      \
    T_1  ref##N_2##_##T_2;                                                      \
    T_1  ref##N_3##_##T_3;

#define SIG_DEF_ARRAY_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, S_1) \
    T_1  refArray##T_1 [S_1];

#define SIG_DEF_CUSTOM_REF(SIG_ID, SIG_TYPE, SIG_NAME, PAYLOAD_TYPE, LOGGING_FUNC) \
    PAYLOAD_TYPE  refCustom##PAYLOAD_TYPE;
#endif /* SIGNAL == SIGNAL_REF_PAYLOAD */

/*
**
** Logging signals with referenced data pointers
**
** To enable reference logging signals replace SIG_DEF definition to the 
** required reference logging macro:
**
**   SIG_DEF_1_REF       - Signal with single reference value 
**   SIG_DEF_2_REF       - Signal with two reference values
**   SIG_DEF_3_REF       - Signal with three reference values
**   SIG_DEF_ARRAY_REF   - Signal with an array referenced
**   SIG_DEF_CUSTOM_REF  - Signal which uses a custom logging function
**
** Usage:
** SIG_DEF_1_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1)
** SIG_DEF_2_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2)
** SIG_DEF_3_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2, N_3, T_3)
** SIG_DEF_ARRAY_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, S_1)
** SIG_DEF_CUSTOM_REF(SIG_ID, SIG_TYPE, SIG_NAME, PAYLOAD_TYPE, LOGGING_FUNC)
**
** Where:
**  SIG_ID and SIG_TYPE are the signal ids and signal types respectively
**  N_1, T_2...N_3, T_3 are the names and types respectively of the reference values to be logged.
**  S_1 is the number of items in the array
**  PAYLOAD_TYPE is the newly specified type used to custom copy the data into
**  LOGGING_FUNC is the newly specified custom logging function to be called to do the copying
**
** Single Reference Example:
**
** To log the contents of the referenced data (type is Int8) pointed 
** to by data_ptr in the signal SIG_EXAMPLE...
**
** Signal Definition = "SIG_DEF_1_REF( SIG_EXAMPLE, SigExample, sigExample, data_ptr, Int8 )
** 
** Custom Reference Example:
**
** To customise the data logged, for example log the 2nd and 6th Byte from 
** a referenced array (data_ptr) in SIG_EXAMPLE and then copy either 5, or 500 bytes
** depending on a boolean in the source signal.
**
** Signal Definition = "SIG_DEF_CUSTOM_REF( SIG_EXAMPLE, SigExample, sigExample, TwoBytePayload, Log2ndAnd6thBytes )"
**
** Custom Payload type should be added to the appropriate signal types file (e.g. ####_sig.h)...
** 
**  typedef struct TwoBytePayloadTag
**  {
**    Int8 val2;
**    Int8 val6;
**    Int8 arrayData[500];
**  }TwoBytePayload;
** 
** Custom logging function is required in the reference logging function headers (e.g. ###reflogfunc.h)
**
** NOTE: The custom logging function must take all desired action including the allocation of
**       memory for a LoggedSignalStructure and the copying of data into it. It must also send
**       the log block using KiTtiSendLoggedSignalStructure (see example below).
**
** NOTE: The function interface should always be 
**                "(## Source signal type ##  *srcSignal, 
**                  SignalId                   srcSigId,
**                  ReferencedDataInd         *refDataInd, 
**                  Boolean                    getAllocSize)"
**
**  Int16 INLINE Log2ndAnd6thBytes(
**                  SigExample *srcSignal, 
**                  SignalId srcSigId, 
**                  ReferencedDataInd *refDataInd, 
**                  Boolean getAllocSize)
**  {
**    * Create the return value *
**    Int16 returnValue;
**     
**    * Check if should be returning allocation size or not *
**    if (getAllocSize == TRUE)
**    {
**      * Calculate max size to allocate, as max size required is known *
**      returnValue = (offsetof(ReferencedDataInd, payload) + (sizeof(TwoBytePayload)));
**    }
**    else
**    {
**      * As getAllocSize == FALSE then actually do the logging *
**
**      * Create pointer to expected payload *
**      TwoBytePayload *twoBytePayload = (TwoBytePayload *)&refDataInd->payload.refEmpty;
**      
**      * log 2nd and 6th bytes *
**      twoBytePayload->val2 = srcSignal->data_ptr[1];
**      twoBytePayload->val6 = srcSignal->data_ptr[5];
**
**      * Check bool to see if should copy 5 or 500 bytes *
**      if (twoBytePayload->copyFullData == TRUE)
**      {
**        * Copy full Data (500 bytes) *
**        memcpy(twoBytePayload->arrayData[0], srcSignal->data_ptr[0], 500);
**
**        * Set return value to full size as complete array used *
**        returnValue = (offsetof(ReferencedDataInd, payload) + (sizeof(TwoBytePayload));
**      }
**      else
**      {
**        * Only copy the first 5 bytes *
**        memcpy(twoBytePayload->arrayData[0], srcSignal->data_ptr[0], 5);
**        
**        * Set size to reflect the size of the data which needs to be sent *
**        * as only 5 bytes were used in the array the rest does not matter *
**        * so to save bandwidth indicate that only the first 5 bytes needs *
**        * to be taken into account when sending the signal. This can only *
**        * be done as the array is last item in payload. *
**        returnValue = (offsetof(ReferencedDataInd, payload) + 
**                          (offsetof(TwoBytePayload, arrayData) + 5;
**      }
**    }
**
**    * Return either the allocation size or the actual size to send *
**    return returnValue;
**  }
**
** Note: It can be seen in the above example that the first value returned when the getAllocSize
**       flag is set is the largest size that the signal will possibly need. It can then be seen
**       that when the actual logging is done (getAllocSize == FALSE) the return value is set
**       to the actual size of the used signal. This means that firstly only the memory possibly
**       required will be allocated for the signal, and only the memory actually used will be
**       be logged.
**
** Result: If the above is followed the data will be sent as SIG_REF_DATA_IND signals containing
** the referenced data in the payload.
*/


/* END OF FILE */
