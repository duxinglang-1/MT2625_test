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
 * Header for the interactive Assert Fail Interface.
 **************************************************************************/

#ifndef KIAFIUTL_H
#define KIAFIUTL_H

#  if defined (UPGRADE_SYS_TOOLS)
#    if defined (KI_ENABLE_INTERACTIVE_LOGGING)

/***************************************************************************
 * Nested Include Files
 **************************************************************************/
#include <kiaficli.h>
#      if defined (KI_ENABLE_RAM_LOGGING)
#include <kirlgutl.h>
#      endif /* KI_ENABLE_RAM_LOGGING */

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/***************************************************************************
 *  Macros
 **************************************************************************/
/***************************************************************************
 *
 * Macro:       M_KiAfiSaveRegs
 *
 * Description: Save the value of all CPU registers (to minimise the affect of this
 *              code, it saves PC and SP first); it adjusts the saved PC and SP values
 *              to compensate for this code and the fact that to get ARMSD to show the
 *              stack frame, it is necessary to first execute a step, the PC must be
 *              be adjusted accordingly, which means:
 *              -    Saved PC is set to start of this function
 *              -    Saved SP is set incremented by 8 to compensate for two PUSH's in the
 *                   assembly code below.
 *              To overcome ARM Thumb limitations, to save the high registers (R8-R15),
 *              they must be moved into a low register (use R1).
 *
 *              This is only run if we are not using KI_USE_ASSERT_SIGNALS and
 *              not CONTINUE_ON_ASSERTFAIL.
 *
 ***************************************************************************/
#if !defined (KI_USE_ASSERT_SIGNALS) && !defined (CONTINUE_ON_ASSERTFAIL)
/* RVCT or SDT */
# if (__ARMCC_VERSION > 210000)     /* RVCT */
#define M_KiAfiSaveRegs()       asmSaveRegisters()

extern void    asmSaveRegisters(void);

# else                              /* SDT */
#define M_KiAfiSaveRegs()       \
__asm                           \
{                               \
    PUSH    {R0};               \
    PUSH    {R1};               \
    MOV     R0,assertRegisters; \
    MOV     R1,assertfail;      \
    STR     R1,[R0,##(15*4)];   \
    MOV     R1,R13;             \
    ADD     R1,##8;             \
    STR     R1,[R0,##(13*4)];   \
    MOV     R1,R8;              \
    STR     R1,[R0,##(8*4)];    \
    MOV     R1,R9;              \
    STR     R1,[R0,##(9*4)];    \
    MOV     R1,R10;             \
    STR     R1,[R0,##(10*4)];   \
    MOV     R1,R11;             \
    STR     R1,[R0,##(11*4)];   \
    MOV     R1,R12;             \
    STR     R1,[R0,##(12*4)];   \
    MOV     R1,R14;             \
    STR     R1,[R0,##(14*4)];   \
    POP     {R1};               \
    ADD     R0,##(1*4);         \
    STMIA   R0!,{R1-R7};        \
    POP     {R0};               \
    PUSH    {R1};               \
    MOV     R1,assertRegisters; \
    STR     R0,[R1,##(0*4)];    \
    POP     {R1};               \
}
# endif
#else  /* else KI_USE_ASSERT_SIGNALS AND CONTINUE_ON_ASSERTFAIL */

#define M_KiAfiSaveRegs()

#endif /* KI_USE_ASSERT_SIGNALS AND CONTINUE_ON_ASSERTFAIL */

/***************************************************************************
 *
 * Macro:       M_KiAfiServiceHandler
 *
 * Description: Enables the interactive assert handler
 *
 ***************************************************************************/
#define M_KiAfiServiceHandler(fILE, lINE, cOND) \
    KiAfiServiceDevAssertHandler (fILE, lINE, cOND)

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
extern  void        KiAfiServiceDevAssertHandler (const char *file,
                                                  Int16       line,
                                                  const char *cond
                                                 );

/***************************************************************************
 *  Exported data
 **************************************************************************/
extern  Int32       assertRegisters [ASSERT_NUM_REGISTERS];
#    else /* else KI_ENABLE_INTERACTIVE_LOGGING */
/***************************************************************************
 *
 * Macro:       M_KiAfiSaveRegs
 *              M_KiAfiServiceHandler
 *
 * Description: Null macros
 *
 ***************************************************************************/
#    define M_KiAfiSaveRegs()
#    define M_KiAfiServiceHandler(fILE, lINE, cOND)
#    endif /* KI_ENABLE_INTERACTIVE_LOGGING */
#  else /* else UPGRADE_SYS_TOOLS */
/***************************************************************************
 *
 * Macro:       M_KiAfiSaveRegs
 *              M_KiAfiServiceHandler
 *
 * Description: Null macros
 *
 ***************************************************************************/
#  define M_KiAfiSaveRegs()
#  define M_KiAfiServiceHandler(fILE, lINE, cOND)


#  endif /* UPGRADE_SYS_TOOLS */
#endif /* KIAFIUTL_H */
