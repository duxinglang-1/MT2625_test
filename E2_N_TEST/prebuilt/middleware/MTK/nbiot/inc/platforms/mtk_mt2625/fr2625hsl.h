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
 * File Description: 2625 platform version of HSL controller
 **************************************************************************/

#ifndef FRPCHSL_H
#define FRPCHSL_H

/*******************************************************************************
** Includes
*******************************************************************************/

#include <frcfg.h>

#endif

#if 0
/*******************************************************************************
** Config
*******************************************************************************/

#if defined (UPGRADE_UPH_DEVELOPMENT)
# if !defined (UPH_LOGGING)
#  if !defined (FR_HSL_DISABLE)
#   define FR_HSL_DISABLE
#  endif
# endif
#endif

/*******************************************************************************
** Conditional Includes
*******************************************************************************/

#if defined (FR_PC_HSL_PRINTF_OUTPUT) && defined (TARGET_SIM_HW_IF)
#  include <string.h>
#  include <uphshwilog.h>
#  include <uphshwihslmap.h>
#endif

/*******************************************************************************
** Constants
*******************************************************************************/

/*******************************************************************************
* Define      : FR_HSL_BASE_ADDR
* Group       : PC Simulation, Framework, HSL, Config
* Description : The base address for the Core HSL Controller for the PC
*                   simulation.
*******************************************************************************/
#define FR_HSL_BASE_ADDR             (frPcHslRegisters)

/*******************************************************************************
** Macros
*******************************************************************************/

/*******************************************************************************
* Macro       : M_FrHslInitialiseBegin
* Group       : PC Simulation, Framework, HSL, Config
* Description : Macro overidden from the version in FRHSLCFG.H to allow the
*                   PC simulation to be initialised.
*******************************************************************************/
#define M_FrHslInitialiseBegin()    FrPcHslInitialise()

/*******************************************************************************
* Macro       : M_FrHslWriteInt16Reg
* Group       : PC Simulation, Framework, HSL, Config
* Parameter   : Int32 rEGaDDR = Absolute address of the register to access.
* Parameter   : Int16 dATA = Data to write to the given address.
* Description : Macro used to write to registers in the HSL controller.
*******************************************************************************/
#define M_FrHslWriteInt16Reg(rEGaDDR, dATA, dDATA2)   FrPcHslWriteRegister((Int16 *) (rEGaDDR), (Int16)(dATA), (Int16)(dDATA2))


#if defined (FR_PC_HSL_PRINTF_OUTPUT)
/*******************************************************************************
* Macro       : M_FrHsl0ByteMsg
* Group       : PC Simulation, Framework, HSL, Config
* Parameter   : Int16 hDR = Message header word.
* Description : When UPH_LOGGING is defined printf the data rather than sending
*                   to HSL, only applicable on PC platfrom.
*******************************************************************************/
#if defined (TARGET_SIM_HW_IF)
#  define M_FrHsl0ByteMsg(hDR)                                              \
         {                                                                  \
           char logMessage[1024];                                           \
           char *logPointName;                                              \
                                                                            \
           logPointName = UphShwiGetHslLogPointName(hDR);                  \
           printf( "%s\n", (char *)logPointName);                           \
           sprintf((char *)logMessage, "%s", (char *)logPointName);         \
           UphShwiLogMessage( logMessage );                                \
         }
#else
#  define M_FrHsl0ByteMsg(hDR)  printf( "Log: %x\n", (Int16)hDR);
#endif


/*******************************************************************************
* Macro       : M_FrHsl1ByteMsg
* Group       : PC Simulation, Framework, HSL, Config
* Parameter   : Int16 hDR = Message header word.
* Parameter   : Int16 aRG1 = First byte of the message.
* Description : When UPH_LOGGING is defined printf the data rather than sending
*                   to HSL, only applicable on PC platfrom.
*******************************************************************************/
#if defined (TARGET_SIM_HW_IF)
#  define M_FrHsl1ByteMsg(hDR, aRG1)                                        \
        {                                                                   \
           char logMessage[1024];                                           \
           char *logPointName;                                              \
                                                                            \
           logPointName = UphShwiGetHslLogPointName(hDR);                  \
           printf( "%s: %d\n", (char *)logPointName, (Int16)aRG1);          \
           sprintf((char *)logMessage, "%s: %d", (char *)logPointName,      \
                                                   (Int16)aRG1);            \
           UphShwiLogMessage( logMessage );                                \
         }
#else
#  define M_FrHsl1ByteMsg(hDR, aRG1)   printf( "Log: %x %d\n", (Int16)hDR, (Int16)aRG1);
#endif

/*******************************************************************************
* Macro       : M_FrHsl2ByteMsg
* Group       : PC Simulation, Framework, HSL, Config
* Parameter   : Int16 hDR = Message header word.
* Parameter   : Int16 aRG1 = First byte of the message.
* Parameter   : Int16 aRG2 = Second byte of the message.
* Description : When UPH_LOGGING is defined printf the data rather than sending
*                   to HSL, only applicable on PC platfrom.
*******************************************************************************/
#if defined (TARGET_SIM_HW_IF)
#  define M_FrHsl2ByteMsg(hDR, aRG1, aRG2)                                  \
        {                                                                   \
           char logMessage[1024];                                           \
           char *logPointName;                                              \
                                                                            \
           logPointName = UphShwiGetHslLogPointName(hDR);                  \
           printf( "%s: %d %d\n", (char *)logPointName,                     \
                                  (Int16)aRG1,                              \
                                  (Int16)aRG2);                             \
           sprintf((char *)logMessage, "%s: %d %d",                         \
                                  (char *)logPointName,                     \
                                  (Int16)aRG1,                              \
                                  (Int16)aRG2);                             \
           UphShwiLogMessage( logMessage );                                \
        }
#else
#  define M_FrHsl2ByteMsg(hDR, aRG1, aRG2)    printf( "Log: %x %d %d\n", (Int16)hDR, (Int16)aRG1, (Int16)aRG2);
#endif

/*******************************************************************************
* Macro       : M_FrHsl3ByteMsg
* Group       : PC Simulation, Framework, HSL, Config
* Parameter   : Int16 hDR = Message header word.
* Parameter   : Int16 aRG1 = First byte of the message.
* Parameter   : Int16 aRG2 = Second byte of the message.
* Parameter   : Int16 aRG3 = Third byte of the message.
* Description : When UPH_LOGGING is defined printf the data rather than sending
*                   to HSL, only applicable on PC platfrom.
*******************************************************************************/
#if defined (TARGET_SIM_HW_IF)
#  define M_FrHsl3ByteMsg(hDR, aRG1, aRG2, aRG3)                            \
      {                                                                     \
           char logMessage[1024];                                           \
           char *logPointName;                                              \
                                                                            \
           logPointName = UphShwiGetHslLogPointName(hDR);                  \
           printf( "%s: %d %d %d\n", (char *)logPointName,                  \
                                     (Int16)aRG1,                           \
                                     (Int16)aRG2,                           \
                                     (Int16)aRG3);                          \
           sprintf((char *)logMessage, "%s: %d %d %d",                      \
                                     (char *)logPointName,                  \
                                     (Int16)aRG1,                           \
                                     (Int16)aRG2,                           \
                                     (Int16)aRG3);                          \
           UphShwiLogMessage( logMessage );                                \
       }
#else
#  define M_FrHsl3ByteMsg(hDR, aRG1, aRG2, aRG3)      printf( "Log: %x %d %d %d\n", (Int16)hDR, (Int16)aRG1, (Int16)aRG2, (Int16)aRG3 );
#endif

/*******************************************************************************
* Macro       : M_FrHsl4ByteMsg
* Group       : PC Simulation, Framework, HSL, Config
* Parameter   : Int16 hDR = Message header word.
* Parameter   : Int16 aRG1 = First byte of the message.
* Parameter   : Int16 aRG2 = Second byte of the message.
* Parameter   : Int16 aRG3 = Third byte of the message.
* Parameter   : Int16 aRG4 = Fourth byte of the message.
* Description : When UPH_LOGGING is defined printf the data rather than sending
*                   to HSL, only applicable on PC platfrom.
*******************************************************************************/
#if defined (TARGET_SIM_HW_IF)
#  define M_FrHsl4ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4)                      \
        {                                                                   \
           char logMessage[1024];                                           \
           char *logPointName;                                              \
                                                                            \
           logPointName = UphShwiGetHslLogPointName(hDR);                  \
           printf( "%s: %d %d %d %d\n", (char *)logPointName,               \
                                        (Int16)aRG1,                        \
                                        (Int16)aRG2,                        \
                                        (Int16)aRG3,                        \
                                        (Int16)aRG4 );                      \
           sprintf((char *)logMessage, "%s: %d %d %d %d",                   \
                                        (char *)logPointName,               \
                                        (Int16)aRG1,                        \
                                        (Int16)aRG2,                        \
                                        (Int16)aRG3,                        \
                                        (Int16)aRG4 );                      \
           UphShwiLogMessage( logMessage );                                \
        }
#else
#  define M_FrHsl4ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4)       printf( "Log: %x %d %d %d %d\n", (Int16)hDR, (Int16)aRG1, (Int16)aRG2, (Int16)aRG3, (Int16)aRG4 );
#endif

/*******************************************************************************
* Macro       : M_FrHsl5ByteMsg
* Group       : PC Simulation, Framework, HSL, Config
* Parameter   : Int16 hDR = Message header word.
* Parameter   : Int16 aRG1 = First byte of the message.
* Parameter   : Int16 aRG2 = Second byte of the message.
* Parameter   : Int16 aRG3 = Third byte of the message.
* Parameter   : Int16 aRG4 = Fourth byte of the message.
* Parameter   : Int16 aRG5 = Fifth byte of the message.
* Description : When UPH_LOGGING is defined printf the data rather than sending
*                   to HSL, only applicable on PC platfrom.
*******************************************************************************/
#if defined (TARGET_SIM_HW_IF)
  #define M_FrHsl5ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4, aRG5)                \
        {                                                                   \
           char logMessage[1024];                                           \
           char *logPointName;                                              \
           logPointName = UphShwiGetHslLogPointName(hDR);                  \
           printf( "%s: %d %d %d %d %d\n", (char *)logPointName,            \
                                           (Int16)aRG1,                     \
                                           (Int16)aRG2,                     \
                                           (Int16)aRG3,                     \
                                           (Int16)aRG4,                     \
                                           (Int16)aRG5);                    \
           sprintf((char *)logMessage, "%s: %d %d %d %d %d",                \
                                           (char *)logPointName,            \
                                           (Int16)aRG1,                     \
                                           (Int16)aRG2,                     \
                                           (Int16)aRG3,                     \
                                           (Int16)aRG4,                     \
                                           (Int16)aRG5);                    \
           UphShwiLogMessage( logMessage );                                \
         }
#else
  #define M_FrHsl5ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4, aRG5)   printf( "Log: %x %d %d %d %d %d\n", (Int16)hDR, (Int16)aRG1, (Int16)aRG2, (Int16)aRG3, (Int16)aRG4, (Int16)aRG5 );
#endif

/*******************************************************************************
* Macro       : M_FrHsl6ByteMsg
* Group       : PC Simulation, Framework, HSL, Config
* Parameter   : Int16 hDR = Message header word.
* Parameter   : Int16 aRG1 = First byte of the message.
* Parameter   : Int16 aRG2 = Second byte of the message.
* Parameter   : Int16 aRG3 = Third byte of the message.
* Parameter   : Int16 aRG4 = Fourth byte of the message.
* Parameter   : Int16 aRG5 = Fifth byte of the message.
* Parameter   : Int16 aRG5 = Sixth byte of the message.
* Description : When UPH_LOGGING is defined printf the data rather than sending
*                   to HSL, only applicable on PC platfrom.
*******************************************************************************/
#if defined (TARGET_SIM_HW_IF)
  #define M_FrHsl6ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6)          \
        {                                                                   \
           char logMessage[1024];                                           \
           char *logPointName;                                              \
           logPointName = UphShwiGetHslLogPointName(hDR);                  \
           printf( "%s: %d %d %d %d %d %d\n", (char *)logPointName,         \
                                              (Int16)aRG1,                  \
                                              (Int16)aRG2,                  \
                                              (Int16)aRG3,                  \
                                              (Int16)aRG4,                  \
                                              (Int16)aRG5,                  \
                                              (Int16)aRG6);                 \
           sprintf((char *)logMessage, "%s: %d %d %d %d %d %d",             \
                                              (char *)logPointName,         \
                                              (Int16)aRG1,                  \
                                              (Int16)aRG2,                  \
                                              (Int16)aRG3,                  \
                                              (Int16)aRG4,                  \
                                              (Int16)aRG5,                  \
                                              (Int16)aRG6);                 \
           UphShwiLogMessage( logMessage );                                \
         }
#else
  #define M_FrHsl6ByteMsg(hDR, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6)   printf( "Log: %x %d %d %d %d %d %d\n", (Int16)hDR, (Int16)aRG1, (Int16)aRG2, (Int16)aRG3, (Int16)aRG4, (Int16)aRG5, (Int16)aRG6 );
#endif
/*******************************************************************************
* Macro       : M_FrHslInt16ArrayMsg
* Group       : PC Simulation, Framework, HSL, Config
* Parameter   : Int16 hDR = Message header word.
* Parameter   : Int16 nUM = Number of Int16's in the array.
* Parameter   : Int16 *aRRAY = Array of nUM Int16's.
* Description : When UPH_LOGGING is defined printf the data rather than sending
*                   to HSL, only applicable on PC platfrom.
*******************************************************************************/
#if defined (TARGET_SIM_HW_IF)
#  define M_FrHslInt16ArrayMsg(hDR, lENGTH, pDATA)                          \
        {                                                                   \
            Int16 cOUNT;                                                    \
            char logMessage[1024];                                          \
            char textValue[10];                                             \
            char *logPointName;                                             \
                                                                            \
            logPointName = UphShwiGetHslLogPointName(hDR);                 \
            printf( "%s LENGTH %d : ", (char *)logPointName,                \
                                       (Int16)lENGTH);                      \
            for (cOUNT = 0; cOUNT < lENGTH; cOUNT++)                        \
            {                                                               \
                printf( " %4x", (Int16)pDATA[cOUNT]);                       \
            }                                                               \
            printf( "\n");                                                  \
            sprintf((char *)logMessage, "% s LENGTH %d : ",                 \
                    (char *)logPointName,                                   \
                    (Int16)lENGTH);                                         \
            for (cOUNT = 0; cOUNT < lENGTH; cOUNT++)                        \
            {                                                               \
                sprintf((char *)textValue, " %4x", (Int16)pDATA[cOUNT]);    \
                if(  strlen((char *)logMessage) + strlen((char *)textValue) \
                   > 1023 )                                                 \
                {                                                           \
                    break;                                                  \
                }                                                           \
                strcat( (char *)logMessage, (char *)textValue );            \
            }                                                               \
            UphShwiLogMessage( logMessage );                               \
         }
#else
#  define M_FrHslInt16ArrayMsg(hDR, lENGTH, pDATA)                          \
        {                                                                   \
            Int16 cOUNT;                                                    \
            printf( "LG_INT16_DATA %4x LENGTH %d : ", (Int16)hDR,           \
                                                      (Int16)lENGTH);       \
            for (cOUNT = 0; cOUNT < lENGTH; cOUNT++)                        \
            {                                                               \
                printf( " %4x", (Int16)pDATA[cOUNT]);                       \
            }                                                               \
            printf( "\n");                                                  \
         }
#endif
#endif /* FR_PC_HSL_PRINTF_OUTPUT */

/*******************************************************************************
** Global Data
*******************************************************************************/
#ifdef  __cplusplus
extern "C" {
#endif

extern const Int8                    *frPcHslRegisters;

/*******************************************************************************
** Global Function Prototypes
*******************************************************************************/
void    FrPcHslInitialise          (void);
void    FrPcHslTerminate           (void);
void    FrPcHslWriteRegister       (Int16 *regAddr, Int16 data, Int16 data2);
void    FrPcHslPrintf(const char *fmt, ...);
#ifdef  __cplusplus
}
#endif

#endif
/* END OF FILE */

