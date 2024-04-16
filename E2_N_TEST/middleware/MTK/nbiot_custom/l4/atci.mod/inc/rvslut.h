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
 * File Description
 * ----------------
 *
 * Header file for vgslut.c
 **************************************************************************/

#ifndef RVSLUT_H
#define RVSLUT_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>

#if !defined (RVUTIL_H)
#  include <rvutil.h>
#endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/
#if defined(FEA_TEMP_INCLUDE_SIM_EMULATION) || defined (FEA_TEMP_ESIM_EMULATION)
typedef enum VgMemusimTypeTag
{
  VG_AT_EMULATE_SIM_OFF = 0,  /* Use the real SIM */
  VG_AT_EMULATE_SIM_ON  = 1,  /* Use the emulation SIM */
  VG_AT_EMULATE_SIM_INVALID
}
VgMemusimType;
#endif

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
ResultCode_t vgSlCPIN   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber  entity);
ResultCode_t vgSlCFUN   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgSlMSIMINS (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgSlMUPIN   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber  entity);
void vgSetSimInsertionState (const VgSimInsertedState  newSimInsertionState);
void vgSetSimState          (const VgSimState          newSimState);
void vgIndicatePukBlocked   (void);

ResultCode_t vgSlCRSM(CommandLine_t *commandBuffer_p,
                      const VgmuxChannelNumber entity);
ResultCode_t vgSlCSIM(CommandLine_t *commandBuffer_p,
                      const VgmuxChannelNumber entity);
ResultCode_t vgSlCMAR(CommandLine_t *commandBuffer_p,
                      const VgmuxChannelNumber  entity);

ResultCode_t vgSlCardmode(  CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber  entity);
ResultCode_t vgSlMSST(CommandLine_t *commandBuffer_p,
                      const VgmuxChannelNumber entity);
ResultCode_t vgSlMGID(CommandLine_t *commandBuffer_p,
                      const VgmuxChannelNumber entity);
#if defined(FEA_TEMP_INCLUDE_SIM_EMULATION) || defined (FEA_TEMP_ESIM_EMULATION)
ResultCode_t vgSlMEMSIM(CommandLine_t *commandBuffer_p,
                      const VgmuxChannelNumber entity);
#endif
#if defined (ENABLE_DUAL_SIM_SOLUTION)
ResultCode_t vgSlMSIMHSEL(CommandLine_t *commandBuffer_p,
                      const VgmuxChannelNumber  entity);
ResultCode_t vgSlCSUS (CommandLine_t *commandBuffer_p,
                      const VgmuxChannelNumber  entity);
#endif /* ENABLE_DUAL_SIM_SOLUTION */

#if defined (SIM_EMULATION_ON)
ResultCode_t vgSlMUSIMEMUW(CommandLine_t *commandBuffer_p,
                      const VgmuxChannelNumber  entity);
#endif /* SIM_EMULATION_ON */

ResultCode_t vgSlCPINR   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber  entity);

ResultCode_t vgSlCsimLock (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber  entity);

ResultCode_t vgGnCNUM ( CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity);

ResultCode_t vgGnCCHO ( CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity);

ResultCode_t vgGnCCHC ( CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity);

ResultCode_t vgGnCGLA ( CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity);

ResultCode_t vgGnCRLA ( CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity);

void viewCFUN (const VgmuxChannelNumber entity);

VgCFUNType vgGetCFUNMode(void);

ResultCode_t vgSlMasterReset(const VgmuxChannelNumber entity);

ResultCode_t vgSlMuapp( CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
void vgMuappPrintEvent( const VgMuappEvent *vgMuappEvent_p);

#if defined(FEA_SIM_IMSI_LOCK_CONTROL)
ResultCode_t vgSlMIMSILOCK(CommandLine_t *commandBuffer_p,
    const VgmuxChannelNumber entity);
#endif

#endif  /* RVSLUT_H */

/* END OF FILE */

