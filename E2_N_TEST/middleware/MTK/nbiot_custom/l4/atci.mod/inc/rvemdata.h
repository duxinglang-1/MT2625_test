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
 * RVEMDATA.H
 * Common data header file, used only by vgdata.h
 **************************************************************************/

#ifndef RVEMDATA_H
#define RVEMDATA_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>

#if defined (ENABLE_AT_ENG_MODE)

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/
typedef enum VgEmControlModeTag
{
  MENGMODE_STOP,
  MENGMODE_SINGLE_SHOT,
  MENGMODE_PERIODIC,
  MENGMODE_INVALID
} VgEmControlMode;

typedef struct VgEmEntityconfigTag
{
  Int32               infoLevel;
  VgEmControlMode     controlMode;
} VgEmEntityconfig;

typedef enum VgEmLastInfoStateTag
{
  MENGMODE_INFO_INIT,
  MENGMODE_INFO_GRR_IDLE,
  MENGMODE_INFO_GRR_SCELL_IDLE,
  MENGMODE_INFO_GRR_DED,
  MENGMODE_INFO_GRR_SCELL_DED,
  MENGMODE_INFO_GPRLC
} VgEmLastInfoState;

#define VGEM_NO_REPORT_BIT              0x00000000
#define VGEM_GSM_MS_STATE_BIT           0x00000001
#define VGEM_GSM_MMR_INFO_BIT           0x00000002
#define VGEM_GSM_CIPHER_BIT             0x00000004
#define VGEM_GSM_IDLE_INFO_BIT          0x00000008
#define VGEM_GSM_DED_INFO_BIT           0x00000010
#define VGEM_GSM_IDLE_SCELL_BIT         0x00000020
#define VGEM_GSM_DED_SCELL_BIT          0x00000040
#define VGEM_3G_RRC_MS_STATE_BIT        0x00000080
#define VGEM_3G_RRC_ENG_INFO_BIT        0x00000100
#define VGEM_3G_RRC_SCELL_INFO_BIT      0x00000200
#define VGEM_3G_INTRAFREQ_INFO_BIT      0x00000400
#define VGEM_3G_INTERFREQ_INFO_BIT      0x00000800
#define VGEM_3G_INTERRAT_INFO_BIT       0x00001000
#define VGEM_GPRS_INFO_BIT              0x00002000
#define VGEM_GRPS_GPSM_TEST_BIT         0x00004000
#define VGEM_GPRS_LLC_STATISTICS_BIT    0x00008000
#define VGEM_GRPS_LLC_PARAMETERS_BIT    0x00010000
#define VGEM_END_OF_REPORT_BIT          0x00020000  /* For the search algorithm to work
                                                       this must be most significant bit */

#define VGEM_REPORT_CELLINFO_LSB_DATA  (VGEM_GSM_MS_STATE_BIT      | \
                                        VGEM_GSM_MMR_INFO_BIT      | \
                                        VGEM_GSM_CIPHER_BIT        | \
                                        VGEM_GSM_IDLE_INFO_BIT     | \
                                        VGEM_GSM_DED_INFO_BIT      | \
                                        VGEM_GSM_IDLE_SCELL_BIT    | \
                                        VGEM_GSM_DED_SCELL_BIT     | \
                                        VGEM_3G_RRC_MS_STATE_BIT)

#define VGEM_REPORT_CELLINFO_MSB_DATA  (VGEM_3G_RRC_ENG_INFO_BIT   | \
                                        VGEM_3G_RRC_SCELL_INFO_BIT | \
                                        VGEM_3G_INTRAFREQ_INFO_BIT | \
                                        VGEM_3G_INTERFREQ_INFO_BIT | \
                                        VGEM_3G_INTERRAT_INFO_BIT  | \
                                        VGEM_GPRS_INFO_BIT         | \
                                        VGEM_GRPS_GPSM_TEST_BIT    | \
                                        VGEM_GPRS_LLC_STATISTICS_BIT)

#define VGEM_REPORT_CELLINFO_THIRD_DATA  (VGEM_GRPS_LLC_PARAMETERS_BIT)
                                        
#define VGEM_REPORT_CELLINFO_ALL_DATA  (VGEM_REPORT_CELLINFO_MSB_DATA | \
                                        VGEM_REPORT_CELLINFO_LSB_DATA | \
                                        VGEM_REPORT_CELLINFO_THIRD_DATA)

#define VGEM_MIN_TIME_PERIOD            0x05
#define VGEM_MAX_TIME_PERIOD            0x0A

#define VGEM_TIME_PERIOD_MASK           0xF0
#define VGEM_MODE_MASK                  0x0F



typedef struct VgEmAttributesTag
{
  Int32                     debugElementsBitField;
#if defined (UPGRADE_GGE)
  GrrTestIdleInfoInd        testIdleInfo;
  GrrTestDedInfoInd         testDedInfo;
  GrrTestIdleSCellInd       testIdleSCell;
  GrrTestDedSCellInd        testDedSCell;
#endif /* UPGRADE_GGE */
  MmrInfoInd                mmrInfo;
  MmrCipherInd              mmrCipherInd;
#if defined (UPGRADE_GGE)
  GrrTestGprsEngInfoInd     testGprsInfo;
#endif /* UPGRADE_GGE */
  AbgpSmTestInfoInd         gpSmTestInfo;
#if defined (UPGRADE_GGE)
  LlcStatistics             llcTestStatsInfo;
  LlcParameters             llcTestParamInfo;
#endif /* UPGRADE_GGE */
#if defined (UPGRADE_3G)
  EmRrcEngInfoInd           rrcEngInfo;
#endif /* UPGRADE_3G */
  VgEmLastInfoState         lastInfoState;
} VgEmAttributes;


/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
#endif  /* ENABLE_AT_ENG_MODE */
#endif  /* RVEMDATA_H */

/* END OF FILE */

