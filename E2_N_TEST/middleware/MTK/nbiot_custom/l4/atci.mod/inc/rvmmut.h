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
 * RVMMUT.H
 * Header file for rvmmut.c
 **************************************************************************/

#ifndef RVMMUT_H
#define RVMMUT_H

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

/* macro to determine the difference of two *MSQN levels */

#define MSQN_LEVEL_DIFF(a, b) (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))

#define VG_CIPCA_EUTRAN_SETTING        3

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
Mnc  vgMmGetMncFromImsi (Boolean threeDigitMncDecoding, Imsi *rawImsi_p);
  
Mcc  vgMmGetMccFromImsi (Imsi *rawImsi_p);

void vgMmCPOLInitialiseData (Boolean clearReadFormat);

#if defined (UPGRADE_SHARE_MEMORY)  || defined(UPGRADE_SHMCL_SOLUTION)
void writeCregDataToShareMemory(void);
#endif /* UPGRADE_SHARE_MEMORY || UPGRADE_SHMCL_SOLUTION */

ResultCode_t vgMmCPOL   (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity);
ResultCode_t vgMmCOPS   (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity);
ResultCode_t vgMmCPLS   (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity);
void viewCESQ            (const VgmuxChannelNumber entity,
                         const CesqReason_t reason);
void displayCESQLevel   (const VgmuxChannelNumber entity,
                         const CesqReason_t reason,
                         Int8  rxLev,
                         Int16 rsrq,
                         Int16 rsrp);
void displayMSQNLevel   (const VgmuxChannelNumber entity,
                         const CesqReason_t reason,
                         int16_t  rxLev,
                         int8_t rsrq,
                         int16_t rsrp);

ResultCode_t vgMmCREG   (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity);

ResultCode_t vgMmGATTCFG(CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity);

void viewCREG           (const VgmuxChannelNumber profileEntity,
                         const Boolean atQuery);

ResultCode_t vgMmSYSCONFIG (CommandLine_t *commandBuffer_p,  
                            const VgmuxChannelNumber entity);  

ResultCode_t vgMmMBAND (CommandLine_t *commandBuffer_p, 
                        const VgmuxChannelNumber entity);
ResultCode_t vgMmMBANDSL (CommandLine_t *commandBuffer_p, 
                        const VgmuxChannelNumber entity);
void viewCSCON (const VgmuxChannelNumber profileEntity,
               const Boolean            atQuery);


ResultCode_t vgMmCSCON(CommandLine_t *commandBuffer_p, 
                        const VgmuxChannelNumber entity);

ResultCode_t vgMmMFRCLLCK(CommandLine_t *commandBuffer_p, 
                        const VgmuxChannelNumber entity);

void viewMENGINFORadioCellInfo (const VgmuxChannelNumber entity );
void viewMENGINFODataTransferInfo(const VgmuxChannelNumber entity);
ResultCode_t vgMmMENGINFO(CommandLine_t * commandBuffer_p,
                        const VgmuxChannelNumber entity);

ResultCode_t vgMmMNBIOTEVENT(CommandLine_t * commandBuffer_p,
                        const VgmuxChannelNumber entity);

void viewMOOSAIND (const VgmuxChannelNumber profileEntity,
                      const Boolean            atQuery);

ResultCode_t vgMmMOOSAIND(CommandLine_t *commandBuffer_p,
                                  const VgmuxChannelNumber entity);

ResultCode_t vgMmMCELLINFO(CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);

#if defined(UPGRADE_3G)
ResultCode_t vgMmMUserMode (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
#endif

ResultCode_t vgMmMNON (CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber  entity);

ResultCode_t vgMmMOPL (CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber  entity);

#if defined(UPGRADE_MTNET)
ResultCode_t vgMmOFF (CommandLine_t *commandBuffer_p,
                      const VgmuxChannelNumber entity);

ResultCode_t vgMmRESET (CommandLine_t *commandBuffer_p,
                      const VgmuxChannelNumber entity);
#endif

Boolean      vgIsCurrentAccessTechnologyLte (void);

Boolean      vgPsdAttached (void);

Boolean      vgCIPCAPermitsActivateAttachDefBearer (void);

ResultCode_t vgMmCESQ    (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);

ResultCode_t vgMmCSQ (CommandLine_t            *commandBuffer_p,
                       const VgmuxChannelNumber entity);

ResultCode_t vgMmMEMMREEST (CommandLine_t            *commandBuffer_p,
                            const VgmuxChannelNumber entity);

/* AT commands for NB-IOT */
ResultCode_t vgMmCEDRXS (CommandLine_t            *commandBuffer_p,
                            const VgmuxChannelNumber entity);

ResultCode_t vgMmCEDRXRDP (CommandLine_t            *commandBuffer_p,
                            const VgmuxChannelNumber entity);

ResultCode_t vgMmCCIOTOPT (CommandLine_t            *commandBuffer_p,
                            const VgmuxChannelNumber entity);

ResultCode_t vgMmCPSMS (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity);

ResultCode_t vgMmCIPCA (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity);
#if defined (FEA_NFM)
ResultCode_t vgMmNFM (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity);

ResultCode_t vgMmNFMC (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity);

ResultCode_t vgMmMNFM (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity);

ResultCode_t vgMmMTC (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity);


#endif

ResultCode_t vgMmMEDRXCFG (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity);
#if defined (FEA_RPM)
ResultCode_t vgMmMRPM (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgMmMRPMR (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity);
ResultCode_t vgMmMRPMSF (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity);
#endif
ResultCode_t vgMmMHPLMNS (CommandLine_t *commandBuffer_p, 
                         const VgmuxChannelNumber entity);

ResultCode_t vgMmMEDRXCFG (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity);

ResultCode_t vgMmMEHPLMN (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity);

#endif /* RVMMUT_H */
/* END OF FILE */

