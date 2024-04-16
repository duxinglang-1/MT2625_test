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
 *************************************************************************/
/** \file
 * TMM Memory Configuration Definitions for non-cacheable pool
 * This file is used for configuring the TMM pool size for non-cacheable
 * pool, which is used for user data. Cacheable pool configuration is
 * done in tmmmem.h (not here) because it is much simpler and does not
 * affect other parts of the system (e.g. KI memory configuration).
 *************************************************************************/

#if !defined (TMMMEMCFG_H)
#define       TMMMEMCFG_H

/***************************************************************************
* Nested Include Files
***************************************************************************/
#include <system.h>

#if defined (UPGRADE_3G)
#include <ups_l2_cfg.h>
#endif

/***************************************************************************
 *  Constants
 **************************************************************************/

#if defined (UPGRADE_3G)
/* ///////////////////// 3G /////////////////// */
/* Total pool size for 3G depends on RLC buffer size */
#define TMM_NON_CACHEABLE_POOL_SIZE     ((UPS_CFG_RLC_AM_BUFFER_SIZE_KB * 1024) + 0x16800) /* 90K overhead */
#else /* UPGRADE_3G */
# if defined (UPGRADE_EDGE)
/* ///////////////////// EDGE /////////////////// */
#define TMM_NON_CACHEABLE_POOL_SIZE     (0x15000) /* 84K */
# else /* UPGRADE_EDGE */
/* ///////////////////// GPRS /////////////////// */
#define TMM_NON_CACHEABLE_POOL_SIZE     (0x09000) /* 36K */
# endif /* UPGRADE_EDGE */
#endif /* UPGRADE_3G */

#define TMM_NON_CACHEABLE_INT_POOL_SIZE (9*1024)

#endif /* TMMMEMCFG_H */
/* END OF FILE */
