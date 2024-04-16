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
 * Header file for vgpfut.c
 **************************************************************************/

#ifndef RVPFUT_H
#define RVPFUT_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <rvsystem.h>

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

/* Generic profile AT command procedures  */

ResultCode_t vgPfOther   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfExtOp   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfSregOp  (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfS1regOp  (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);

/* Specific profile AT command procedures */

ResultCode_t vgPfDS      (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfF       (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfZ       (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfW       (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfV       (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfIFC     (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfIPR     (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfICF     (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfCNMI    (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
ResultCode_t vgPfCRES    (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfCSAS    (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfCSCS    (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfDCD     (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfDTR     (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfKFC     (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfCSMP    (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfCSMS    (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);

#if defined (FEA_MT_PDN_ACT)
ResultCode_t vgPfCGAUTO  (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
#endif /* FEA_MT_PDN_ACT */

ResultCode_t vgPfCGEREP  (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
ResultCode_t vgPfMGMTPCACT (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity);
ResultCode_t vgPfMGPPPLOG (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
ResultCode_t vgPfN1    (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);                                                                    
ResultCode_t vgPfS95    (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
ResultCode_t vgPfCGPIAF  (CommandLine_t* commandBuffer_p,
                          const VgmuxChannelNumber entity);

/* For NB-IOT */
ResultCode_t vgPfMPLMNURI  (CommandLine_t* commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgPfMAPNURI  (CommandLine_t* commandBuffer_p,
                          const VgmuxChannelNumber entity);

#endif  /* RVPFUT_H */

/* END OF FILE */

