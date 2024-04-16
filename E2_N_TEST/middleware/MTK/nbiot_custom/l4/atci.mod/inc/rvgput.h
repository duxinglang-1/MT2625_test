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
 * Header file for vggput.c
 **************************************************************************/

#ifndef RVGPUT_H
#define RVGPUT_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>

#include <rvutil.h>

#include <abpd_sig.h>

#include <rvsystem.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

typedef struct SupportedPDNTypesMapTag
{
  const Char     *str;
  PdnType        PDNType;
  Boolean        supported;
  Int8           arrIndx;
} SupportedPDNTypesMap;

typedef enum CountActionTag
{
  RESET_SNDCP_COUNTER,
  READ_SNDCP_COUNTER,
  START_PERIODIC_REPORTING,
  START_CONTEXT_DEACTIVATION_REPORTING,
  STOP_REPORTING_COUNTERS
}
CountAction;

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

Boolean vgAllocateRamToCid          (const Int32 cid );
void    vgFreeRamForCid             (const Int32 cid );

void         vgInitialiseCidData         (VgPsdStatusInfo *ptr, Int32  thisCid);
void vgInitialisePdnAddress (PdnAddress *pdnAddress_p);
ResultCode_t vgActivateContext           (Int8 cid,
                                          const VgmuxChannelNumber entity,
                                          AbpdPdpConnType connectionType);

#if defined (FEA_MT_PDN_ACT)
ResultCode_t vgActivateMtContext         (Int8 cid,
                                          const VgmuxChannelNumber entity,
                                          AbpdPdpConnType connectionType);

ResultCode_t vgPdRespondToMtpca          (const VgmuxChannelNumber entity,
                                          Int32 cid,
                                          AbpdPdpConnType connectionType);
#endif /* FEA_MT_PDN_ACT */

Boolean      vgPDNTypeToIndx             (PdnType pdnType,
                                          Int8 *arrIndx);

const SupportedPDNTypesMap *getSupportedPDNTypesMap (void);

Boolean vgDoesEntityHaveSeparateDataChannel    (const VgmuxChannelNumber entity);

Boolean vgIsEntityInPacketTransportMode    (const VgmuxChannelNumber entity);

VgmuxChannelNumber       vgFindEntityLinkedToCid (Int8 cid);
VgmuxChannelNumber       vgFindEntityLinkedToCidWithoutCheckEntity (Int8 cid);
Int8                     vgFindCidLinkedToConnId (Int8 connId);
Boolean                  vgDeactivateNextContextOwnedByEntity (const VgmuxChannelNumber entity);
Int8                     vgFindCidWithDataConnLinkedToEntity (const VgmuxChannelNumber entity);

#if defined (FEA_DEDICATED_BEARER)
Int8                     vgFindCidOfNextActiveSecondaryContextLinkedToPrimaryCid (Int8 cid);
#endif /* FEA_DEDICATED_BEARER */

Boolean vgGetUnusedCid (const VgmuxChannelNumber entity,
                        Int8 psdBearerId,
                        Int8 *cidValue);
Boolean vgGetFreeCidForDataConn (Int32 *cidValue,
                             const VgmuxChannelNumber entity);
void    vgActivateAttachDefBearerContext (const VgmuxChannelNumber entity);
void    vgGpAbortAttach                  (const VgmuxChannelNumber entity);
Int8    vgGpGetMinCidValue (const VgmuxChannelNumber entity);

Boolean vgAssignNewActiveContext (const Int8 cid,
                                   const VgmuxChannelNumber entity);
Boolean vgGetFreeCid (Int32 *cidValue,
                             const VgmuxChannelNumber entity);

#if defined (FEA_MT_PDN_ACT)
ResultCode_t vgGpCGANS   (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity);
ResultCode_t vgGpMMTPDPCID (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity);
#endif /* FEA_MT_PDN_ACT */

ResultCode_t vgGpCGQMIN  (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
ResultCode_t vgGpCGATT   (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
ResultCode_t vgGpCGACT   (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);

ResultCode_t vgGpMGCOUNT (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
ResultCode_t vgGpCGDATA  (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
ResultCode_t vgGpCGDCONT (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
ResultCode_t vgGpCGPADDR (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
ResultCode_t vgGpCGQREQ  (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
ResultCode_t vgGpCGREG   (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
ResultCode_t vgGpCGSMS   (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
ResultCode_t vgGpD       (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);

ResultCode_t vgGpMGAPNR  (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
ResultCode_t vgGpMGAPNS  (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
ResultCode_t vgGpMGAPNW  (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
ResultCode_t vgGpCGEQMIN (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGpCGEQNEG (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGpCGEQREQ (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);

#if defined (FEA_QOS_TFT)
ResultCode_t vgGpCGTFT   (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGpCGTFTRDP    (CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity);
ResultCode_t vgGpCGCMOD  (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);
ResultCode_t vgGpCGEQOS      (CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity);
ResultCode_t vgGpCGEQOSRDP   (CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity);
#endif /* FEA_QOS_TFT */

#if defined (FEA_DEDICATED_BEARER)
ResultCode_t vgGpCGDSCONT (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity);
#endif

ResultCode_t vgGpMGSINK (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity);

ResultCode_t vgGpMGTCSINK (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity);

#if defined (FEA_PPP)
ResultCode_t vgGpMLOOPPSD (CommandLine_t *commandBuffer_p,
                            const VgmuxChannelNumber entity);

ResultCode_t vgGpMPPPCONFIG (CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity);

ResultCode_t vgGpMPPPCONFIGAUTH (CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity);
#endif /* FEA_PPP */

ResultCode_t vgGpEnsureAttached (const VgmuxChannelNumber entity);
#if defined (FEA_ACL)
ResultCode_t vgGpMSACL (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
ResultCode_t vgGpMLACL (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);

ResultCode_t vgGpMWACL (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);

ResultCode_t vgGpMDACL (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
#endif
void vgProcMgSinkTxPdu (void);
void vgProcMgtcSinkTxPdu (void);

/* AT commands for NASMDL2 */
ResultCode_t vgGpCGCONTRDP   (CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity);

#if defined (FEA_DEDICATED_BEARER)
ResultCode_t vgGpCGSCONTRDP  (CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity);
#endif

ResultCode_t vgGpMCGDEFCONT  (CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity);
ResultCode_t vgGpCGDEL       (CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity);
ResultCode_t vgGpCGAUTH      (CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity);
ResultCode_t vgGpCEREG       (CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity);

ResultCode_t vgGpMDPDNP (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);

#if defined (FEA_UPDIR)
ResultCode_t vgGpMUPDIR (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity);
#endif
/* AT commands for NBIOT packet data */
ResultCode_t vgGpMNBIOTDT    (CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity);

ResultCode_t vgGpMNBIOTRAI (CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity);

ResultCode_t vgGpCGAPNRC(CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity);

ResultCode_t vgGpCSODCP(CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity);
#endif  /* RVGPUT_H */

/* END OF FILE */
