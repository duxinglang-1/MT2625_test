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
 * Header file for vggnut.c
 **************************************************************************/

#ifndef RVGNUT_H
#define RVGNUT_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <rvutil.h>

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
 *  Function Prototypes
 **************************************************************************/

ResultCode_t vgGnCOPN (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGnMLTS (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGnCCLK (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGnGOI  (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGnGSN  (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGnCGSN (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGnMCGSN (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);                        
ResultCode_t vgGnMCGHWN ( CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGnMOPTLOCK ( CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGnCIMI (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGnP    (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGnI    (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGnT    (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGnGMR  (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGnGMM  (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGnGCAP (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGnCEER (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGnGMI  (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGnMSPN (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGnSPN (CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber entity);
ResultCode_t vgGnMUNSOL (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGnMABORT (CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber entity);
ResultCode_t vgGnHVER (CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber entity);
ResultCode_t vgGnMFtrCfg(CommandLine_t *commandBuffer_p, 
                          const VgmuxChannelNumber entity);
ResultCode_t vgGnRouteMMI(CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
ResultCode_t vgGnMFASSERT(  CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity);

/**********************************/
/* AT commands for NB-IOT project */
/**********************************/

/* NVRAM access */
ResultCode_t vgGnMNVMQ  (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGnMNVMAUTH (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGnMNVMW  (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGnMNVMOTPW  (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGnMNVMR   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGnMNVMOTPR  (CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity);
ResultCode_t vgGnMNVMOTPW  (CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity);
ResultCode_t vgGnMNVMGET   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGnMNVMIVD   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGnMNVMRSTONE   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGnMNVMRST   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);

/* NVRAM mini dump */
#if defined (FEA_MINI_DUMP)

ResultCode_t vgGnMNVMMDNQ   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGnMNVMMDR   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGnMNVMMDC   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
#endif
ResultCode_t vgGnMNVUID   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
 #if defined (ATCI_IDC_ENABLE)
/* IDC RF Control */  
ResultCode_t vgGnIDCFREQ   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGnIDCPWRBACKOFF   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGnIDCTX2GPS   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
#endif
ResultCode_t vgGnIDCTEST     (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);

/* RF Calibration */
ResultCode_t vgGnMCALDEV ( CommandLine_t *commandBuffer_p, 
                           const VgmuxChannelNumber entity);
ResultCode_t vgGnMCAL    ( CommandLine_t *commandBuffer_p, 
                           const VgmuxChannelNumber entity);

ResultCode_t vgGnMICCID  (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGnMHOMENW (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);

ResultCode_t vgGnMSPCHSC  (CommandLine_t* commandBuffer_p,
                          const VgmuxChannelNumber entity);

ResultCode_t vgGnMN1DEBUG (CommandLine_t* commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGnMBSC   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t CH_AT_TEST (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);

#endif  /* RVGNUT_H */

/* END OF FILE */

