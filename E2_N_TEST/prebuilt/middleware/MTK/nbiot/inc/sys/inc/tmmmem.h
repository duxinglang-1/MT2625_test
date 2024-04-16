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
 ***************************************************************************
 * File Description: Traffic Memory Manager Memory definitions
 *************************************************************************/

/* This this file may be included more than once in a source file.
 * Hence it is not protected by any compile time switches. */

/***************************************************************************
* Nested Include Files
***************************************************************************/
#include <tmmmemcfg.h>

/***************************************************************************
 *  Constants
 **************************************************************************/

/* Set the default pool ids to use */
#define TMM_USER_DATA_INT_KI_POOL_ID  KI_DYNAMIC_NON_CACHEABLE_INT_MEM_POOL
#define TMM_USER_DATA_KI_POOL_ID  KI_DYNAMIC_NON_CACHEABLE_MEM_POOL
#define TMM_CTRL_DATA_KI_POOL_ID  KI_DYNAMIC_MEM_POOL

/***************************************************************************
 *  Access Macros
 **************************************************************************/

#if defined (TMM_GET_VIRT_POOL_NAME)
#define TMM_MEM_POOL_DEF_VIRT( POOL_NAME, POOL_SIZE, HWM_RATIO, LWM_RATIO, MIN_TAIL, AC_MAX, AC_HWM, AC_LWM_RATIO, KI_POOL_ID) \
                               POOL_NAME,

#elif defined (TMM_GET_REAL_POOL_NAME)
#define TMM_MEM_POOL_DEF_REAL( POOL_NAME, POOL_SIZE, HWM_RATIO, LWM_RATIO, MIN_TAIL, AC_MAX, AC_HWM, AC_LWM_RATIO, KI_POOL_ID) \
                               POOL_NAME,

#elif defined (TMM_GET_MAPPED_POOL_NAME)
#define TMM_MEM_POOL_DEF_MAPPED( POOL_NAME, POOL_SIZE, HWM_RATIO, LWM_RATIO, MIN_TAIL, AC_MAX, AC_HWM, AC_LWM_RATIO, KI_POOL_ID) \
                                 POOL_NAME,

#elif defined (TMM_SET_VIRT_POOL_TABLE)
#define TMM_MEM_POOL_DEF_VIRT( POOL_NAME, POOL_SIZE, HWM_RATIO, LWM_RATIO, MIN_TAIL, AC_MAX, AC_HWM, AC_LWM_RATIO, KI_POOL_ID) \
                              {POOL_SIZE, (POOL_SIZE * HWM_RATIO), (POOL_SIZE * HWM_RATIO * LWM_RATIO), \
                               MIN_TAIL, AC_MAX, AC_HWM, (AC_HWM * AC_LWM_RATIO), KI_POOL_ID},

#elif defined (TMM_SET_REAL_POOL_TABLE)
#define TMM_MEM_POOL_DEF_REAL( POOL_NAME, POOL_SIZE, HWM_RATIO, LWM_RATIO, MIN_TAIL, AC_MAX, AC_HWM, AC_LWM_RATIO, KI_POOL_ID) \
                              {POOL_SIZE, (POOL_SIZE * HWM_RATIO), (POOL_SIZE * HWM_RATIO * LWM_RATIO), \
                               MIN_TAIL, AC_MAX, AC_HWM, (AC_HWM * AC_LWM_RATIO), KI_POOL_ID},

#elif defined (TMM_SET_MAPPED_POOL_TABLE)
#define TMM_MEM_POOL_DEF_MAPPED( POOL_NAME, POOL_SIZE, HWM_RATIO, LWM_RATIO, MIN_TAIL, AC_MAX, AC_HWM, AC_LWM_RATIO, KI_POOL_ID) \
                              {POOL_SIZE, (POOL_SIZE * HWM_RATIO), (POOL_SIZE * HWM_RATIO * LWM_RATIO), \
                               MIN_TAIL, AC_MAX, AC_HWM, (AC_HWM * AC_LWM_RATIO), KI_POOL_ID},

#elif defined (TMM_GET_PROFILE_NAME)
#define TMM_PROFILE_DEF( PROF_NAME, VIRT_POOL_1, VIRT_POOL_2, VIRT_POOL_3, VIRT_POOL_4, VIRT_POOL_5, VIRT_POOL_6) \
                         PROF_NAME,

#elif defined (TMM_SET_PROFILE_TABLE)
#define TMM_PROFILE_DEF( PROF_NAME, VIRT_POOL_1, VIRT_POOL_2, VIRT_POOL_3, VIRT_POOL_4, VIRT_POOL_5, VIRT_POOL_6) \
                         {{VIRT_POOL_1, VIRT_POOL_2, VIRT_POOL_3, VIRT_POOL_4, VIRT_POOL_5, VIRT_POOL_6}},

#endif

/***************************************************************************
 *  Define unused access Macros
 **************************************************************************/
#if !defined (TMM_PROFILE_DEF)
#define TMM_PROFILE_DEF( PROF_NAME, VIRT_POOL_1, VIRT_POOL_2, VIRT_POOL_3, VIRT_POOL_4, VIRT_POOL_5, VIRT_POOL_6)
#endif

#if !defined (TMM_MEM_POOL_DEF_VIRT)
#define TMM_MEM_POOL_DEF_VIRT( POOL_NAME, POOL_SIZE, HWM_RATIO, LWM_RATIO, MIN_TAIL, AC_MAX, AC_HWM, AC_LWM_RATIO, KI_POOL_ID)
#endif

#if !defined (TMM_MEM_POOL_DEF_REAL)
#define TMM_MEM_POOL_DEF_REAL( POOL_NAME, POOL_SIZE, HWM_RATIO, LWM_RATIO, MIN_TAIL, AC_MAX, AC_HWM, AC_LWM_RATIO, KI_POOL_ID)
#endif

#if !defined (TMM_MEM_POOL_DEF_MAPPED)
#define TMM_MEM_POOL_DEF_MAPPED( POOL_NAME, POOL_SIZE, HWM_RATIO, LWM_RATIO, MIN_TAIL, AC_MAX, AC_HWM, AC_LWM_RATIO, KI_POOL_ID)
#endif

/***************************************************************************
 *  Memory Pools and Sizes
 ***************************************************************************
 ***************************************************************************
 * TMM memory pool illustration:
 * =========================================================================================
 * |                  UL pool                  |                  DL pool                  |
 * =========================================================================================
 * |                 |            |            |                       |         |         |
 * |                LWM          HWM           |                      LWM       HWM        |
 * |<------A-------->                           <-----------D--------->                    |
 * |<------------B--------------->              <---------------E--------------->          |
 * |<------------------C----------------------> <--------------------F-------------------->|
 * |<-----------------------------------------G------------------------------------------->|
 *
 * Memory configuration parameters for non-cacheable (user data) pools:
 * Configuring non-cacheable (user data) pools is critical because it affects the data throughput
 * and has a big impact on the memory requirements of the system.
 * 1. TMM_NON_CACHEABLE_POOL_SIZE:
 *    Total TMM non-cacheable pool size = G
 *    This is defined in tmmmemcfg.h based on technology.
 * 2. TMM_NON_CACHEABLE_UL_POOL_SIZE_RATIO:
 *    The ratio of UL memory pool size to total pool size = (C / G)
 *    Used for computing UL & DL pool sizes (C & F) from the total pool size (G).
 * 3. HWM / pool size ratio = (B / C) and (E / F)
 *    These are defined per pool (by TMM_MEM_POOL_DEF macros) and are used to calculate UL & DL HWM (B & E)
 *    from UL & DL pool sizes (C & F).
 * 4. LWM / HWM ratio = (A / B) and (D / E)
 *    These are defined per pool (by TMM_MEM_POOL_DEF macros) and are used to calculate UL & DL LWM (A & D)
 *    from UL & DL HWM (B & E).
 ***************************************************************************/

/* In these calculations, we should take into account any overheads added by TMM and dynamic memory manager
 * (such as headers). Otherwise we might run out of dynamic memory before running out of TMM pool,
 * and this would cause an assert from dynamic memory.
 * The total overhead would be the overhead per TMM block * maximum number of TMM blocks that can be allocated
 * from pools. Currently the size of dynamic memory manager header is around 12 bytes. Maximum alloc count is
 * taken as 200 (from both UL and DL pools).
 */
#define TMM_MAX_ALLOCS                              200
#define DYN_MEM_OVERHEAD                            12
#define TMM_OVERHEAD                                sizeof(TmmBlockHeader) /* 12 */

#define TMM_POOL_SIZE_OVERHEAD                      ((DYN_MEM_OVERHEAD + TMM_OVERHEAD) * TMM_MAX_ALLOCS) /* 5K */

/* The available TMM pool is the non-cacheable pool size minus the overhead */
#define TMM_AVAILABLE_NON_CACHEABLE_POOL_SIZE       (TMM_NON_CACHEABLE_POOL_SIZE - TMM_POOL_SIZE_OVERHEAD)

#if defined (UPGRADE_3G)

/* ///////////////////// 3G /////////////////// */
# if defined (UPGRADE_3G_HSDPA)
#  if defined(UPGRADE_3G_HSUPA) && defined(UPS_CFG_E_DCH_CATEGORY_6)
#   if defined(UPS_CFG_HS_DSCH_CATEGORY_7) || defined(UPS_CFG_HS_DSCH_CATEGORY_8)
#    define TMM_NON_CACHEABLE_UL_POOL_SIZE_RATIO        2/3 /* from 3GPP recommandation */
#   elif defined(UPS_CFG_HS_DSCH_CATEGORY_6)
#    define TMM_NON_CACHEABLE_UL_POOL_SIZE_RATIO        3/7
#   else /* UPS_CFG_HS_DSCH_CATEGORY_6 */
#    define TMM_NON_CACHEABLE_UL_POOL_SIZE_RATIO        1/4
#   endif /* UPS_CFG_HS_DSCH_CATEGORY_6 */
#  endif /*UPGRADE_3G_HSUPA && UPS_CFG_E_DCH_CATEGORY_6*/
#  if !defined(TMM_NON_CACHEABLE_UL_POOL_SIZE_RATIO)
#define TMM_NON_CACHEABLE_UL_POOL_SIZE_RATIO        1/4
#  endif /* TMM_NON_CACHEABLE_UL_POOL_SIZE_RATIO */
# else /* UPGRADE_3G_HSDPA */
#define TMM_NON_CACHEABLE_UL_POOL_SIZE_RATIO        3/7
# endif  /* UPGRADE_3G_HSDPA */

#else /* UPGRADE_3G */

# if defined (UPGRADE_EDGE)

/* ///////////////////// EDGE /////////////////// */
#define TMM_NON_CACHEABLE_UL_POOL_SIZE_RATIO        3/4

# else /* UPGRADE_EDGE */

/* ///////////////////// GPRS /////////////////// */
#define TMM_NON_CACHEABLE_UL_POOL_SIZE_RATIO        2/3

# endif /* UPGRADE_EDGE */

#endif /* UPGRADE_3G */

/* Pool size definitions for non-cacheable pools */
/* TMM_POOL_SIZE_CTL are used only by RLC CONTROL PDUs and LLC for sending GMM control message.
 * (ALL control PDUS (status, reset, reset ack) on SRBs and radio bearers on the uplink and downlink)
 */
#define TMM_POOL_SIZE_CTL       (0x1400) /* 5K */
# if defined (UPGRADE_3G_HSUPA) && defined (UPGRADE_3G_TDD128)
#  if defined (PLATFORM_G1MCU)
#define TMM_POOL_SIZE_DL        (360 * 1024) /* 360k */
#  else
#define TMM_POOL_SIZE_DL        (300 * 1024) /* 300k */
#  endif
#define TMM_POOL_SIZE_UL        (TMM_AVAILABLE_NON_CACHEABLE_POOL_SIZE - TMM_POOL_SIZE_DL - TMM_POOL_SIZE_CTL)
# else
#define TMM_POOL_SIZE_UL        (TMM_AVAILABLE_NON_CACHEABLE_POOL_SIZE * TMM_NON_CACHEABLE_UL_POOL_SIZE_RATIO)
#define TMM_POOL_SIZE_DL        (TMM_AVAILABLE_NON_CACHEABLE_POOL_SIZE - TMM_POOL_SIZE_UL - TMM_POOL_SIZE_CTL)
# endif
#define TMM_POOL_SIZE_DL_INT     TMM_NON_CACHEABLE_INT_POOL_SIZE

/* This section defines the pool name and its properties.
 *
 * Pool Name
 * =========
 * The pool name will form the pool ID that calls to TMM will use.
 *
 * Pool Size
 * =========
 * This is the size of the pool in bytes.
 *
 * HWM / pool size ratio
 * =====================
 * The HWM is the High Water Mark in bytes. When allocated memory goes above this value,
 * tasks are sent signals to switch flow control off.
 *
 * The High Water Mark can typically be set to 1/2 the pool size.
 * Setting HWM to 0 disables flow control for the pool.
 *
 * LWM / HWM Ratio
 * ===============
 * This is the ratio of LWM / HWM and is used to calculate LWM from HWM.
 * The LWM is the Low Water Mark in bytes. When allocated memory drops below this value,
 * tasks are sent signals to switch flow control on.
 * Typically this value is (5/6)
 *
 * Min Tail Length
 * ===============
 * The Min Tail Length is the minimum difference in bytes between the current TMM block
 * length and the new TMM block length that TMM will tail the block. This value is used
 * in the TmmFreeTail () call.
 *
 * Alloc Count Max
 * ===============
 * This is the maximum number of TMM blocks that can be allocated from this pool by tasks
 * that are registered for flow control. If the number of allocations reaches this value,
 * then requests for blocks by tasks that are registered for flow control will be denied.
 * Tasks that are not registered for flow control will be allowed to allocate memory.
 *
 * Setting this value to 0 disables the max alloc count mechanism for the pool.
 *
 * Alloc Count HWM
 * ===============
 * This is the number of allocated TMM blocks from this pool that can occur before
 * TMM will signal to the registered flow control tasks to flow control.
 *
 * Setting this value to 0 disables the alloc count flow control mechanism for the pool.
 *
 * Alloc Count LWM / HWM Ratio
 * ===========================
 * This is the ratio of LWM / HWM for alloc count and is used to calculate LWM from HWM.
 * for the alloc count.
 * Typically this value is (4/5)
 */

/*
 *      Virtual Pools
 *
 * Virtual pools are called directly by users of TMM. Virtual pool parameters can change depending on the
 * current physical transport mechanism (ie. 3G, 2.5G etc..). Hence, Virtual pools are setup with their
 * parameters zero'ed. The parameter changes are performed according to TMM profiles. If you add a Virtual
 * pool, you MUST add the corresponding mapping information to the TMM Profile Definition below. Virtual
 * pools inherit properties from Mapped pools. The inheritance is defined by TMM profiles.
 *
 *=============================================================================================================================
 *                     Pool Name              Pool     HWM /   LWM /   Min     Alloc   Alloc   Alloc   KI PoolId
 *                                            Size     Pool    HWM     Tail    Count   Count   Count
 *                                            (bytes)  Size    Ratio   Length  Max     HWM     LWM/HWM
 *                                                     Ratio           (bytes)                 Ratio
 *============================================================================================================================*/

TMM_MEM_POOL_DEF_VIRT (TMM_DEFAULT_POOL_UL,    0,      0,      0,      0,      0,      0,      0,      TMM_USER_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_VIRT (TMM_DEFAULT_POOL_DL,    0,      0,      0,      0,      0,      0,      0,      TMM_USER_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_VIRT (TMM_DEFAULT_POOL_CTL,   0,      0,      0,      0,      0,      0,      0,      TMM_USER_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_VIRT (TMM_DEFAULT_POOL_UL_C,  0,      0,      0,      0,      0,      0,      0,      TMM_CTRL_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_VIRT (TMM_DEFAULT_POOL_DL_C,  0,      0,      0,      0,      0,      0,      0,      TMM_CTRL_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_VIRT (TMM_DEFAULT_POOL_DL_INT,   0,      0,      0,      0,      0,      0,      0,      TMM_USER_DATA_INT_KI_POOL_ID)

/*
 *      Real Pools
 *
 * Real pools can be called directly by users of TMM. Real pool parameters remain constant and are never
 * modified.
 *
 *=============================================================================================================================
 *                       Pool Name      Pool                HWM /   LWM /   Min     Alloc   Alloc   Alloc   KI PoolId
 *                                      Size                Pool    HWM     Tail    Count   Count   Count
 *                                      (bytes)             Size    Ratio   Length  Max     HWM     LWM/HWM
 *                                                          Ratio           (bytes)                 Ratio
 *============================================================================================================================*/
/* eg:
TMM_MEM_POOL_DEF_REAL (TMM_POOL_REAL1,  20000,              1/2,    0,      0,      0,      0,      0,      TMM_USER_DATA_KI_POOL_ID)
*/
TMM_MEM_POOL_DEF_REAL (TMM_POOL_BIP,        0,    0,    0,    0,    0,    0,    0,    TMM_CTRL_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_REAL (TMM_POOL_AP_BRIDGE,  0,    0,    0,    0,    0,    0,    0,    TMM_CTRL_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_REAL (TMM_POOL_SOFTSIM,    0,    0,    0,    0,    0,    0,    0,    TMM_CTRL_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_REAL (TMM_POOL_NVRAM,      0,    0,    0,    0,    0,    0,    0,    TMM_CTRL_DATA_KI_POOL_ID)

/*
 *      Mapped Pools
 *
 * Mapped pools are never referenced directly by users of TMM. Mapped pools are used to modify the parameters
 * of virtual pools during operation, depending on the current underlying physical transport mechanism
 * (ie. 3G, 2.5G etc..). The mapped pools are mapped according to TMM profiles.
 *
 *=============================================================================================================================
 *                       Pool Name      Pool                HWM /   LWM /   Min     Alloc   Alloc   Alloc   KI PoolId
 *                                      Size                Pool    HWM     Tail    Count   Count   Count
 *                                      (bytes)             Size    Ratio   Length  Max     HWM     LWM/HWM
 *                                                          Ratio           (bytes)                 Ratio
 *============================================================================================================================*/

/* NOTE: The pools ending with _C are allocated from cacheable memory area (typically used for storing data other than
 * user plane data), other pools are allocated from non-cacheable memory area (for user plane data).
 */

#if defined (UPGRADE_3G)
/* ///////////////////// 3G /////////////////// */
/*TODO_XDU: fine tuning has to be done*/
# if defined (UPGRADE_3G_HSUPA) && defined(UPS_CFG_E_DCH_CATEGORY_6)
#  if defined(UPS_CFG_HS_DSCH_CATEGORY_6)
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_UL,   TMM_POOL_SIZE_UL,   5/8,    7/8,    500,    240,    190,    4/5,    TMM_USER_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_DL,   TMM_POOL_SIZE_DL,   5/6,    5/6,    500,    0,      0,      0,      TMM_USER_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_CTL,  TMM_POOL_SIZE_CTL,  0,      5/6,    500,    0,      0,      0,      TMM_USER_DATA_KI_POOL_ID)
#  elif defined(UPS_CFG_HS_DSCH_CATEGORY_7) || defined(UPS_CFG_HS_DSCH_CATEGORY_8) || defined(UPS_CFG_HS_DSCH_CATEGORY_9) || defined(UPS_CFG_HS_DSCH_CATEGORY_10)/*TODO*/
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_UL,   TMM_POOL_SIZE_UL,   5/8,    7/8,    500,    240,    190,    4/5,    TMM_USER_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_DL,   TMM_POOL_SIZE_DL,   5/6,    5/6,    500,    0,      0,      0,      TMM_USER_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_CTL,  TMM_POOL_SIZE_CTL,  0,      5/6,    500,    0,      0,      0,      TMM_USER_DATA_KI_POOL_ID)
# else /* UPS_CFG_HS_DSCH_CATEGORY_7/8/9/10 */
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_UL,   TMM_POOL_SIZE_UL,   1/2,    5/6,    500,    240,   110,     4/5,    TMM_USER_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_DL,   TMM_POOL_SIZE_DL,   2/3,    5/6,    500,    0,      0,      0,      TMM_USER_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_CTL,  TMM_POOL_SIZE_CTL,  0,      5/6,    500,    0,      0,      0,      TMM_USER_DATA_KI_POOL_ID)
#  endif /*UPS_CFG_HS_DSCH_CATEGORY_7/8/9/10 */
# else /*UPGRADE_3G_HSUPA && UPS_CFG_E_DCH_CATEGORY_6*/
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_UL,   TMM_POOL_SIZE_UL,   1/2,    5/6,    500,    240,    110,    4/5,    TMM_USER_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_DL,   TMM_POOL_SIZE_DL,   5/6,    5/6,    500,    0,      0,      0,      TMM_USER_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_CTL,  TMM_POOL_SIZE_CTL,  0,      5/6,    500,    0,      0,      0,      TMM_USER_DATA_KI_POOL_ID)
# endif /*UPGRADE_3G_HSUPA && UPS_CFG_E_DCH_CATEGORY_6*/
#else /* UPGRADE_3G */

# if defined (UPGRADE_EDGE)
/* ///////////////////// EDGE /////////////////// */
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_UL,   TMM_POOL_SIZE_UL,   1/2,    5/6,    500,    160,    110,    4/5,    TMM_USER_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_DL,   TMM_POOL_SIZE_DL,   0,      5/6,    500,    0,      0,      0,      TMM_USER_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_CTL,  TMM_POOL_SIZE_CTL,  0,      5/6,    500,    0,      0,      0,      TMM_USER_DATA_KI_POOL_ID)

# else /* UPGRADE_EDGE */
/* ///////////////////// GPRS /////////////////// */
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_UL,   TMM_POOL_SIZE_UL,   1/2,    5/6,    500,    40,     35,     4/5,    TMM_USER_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_DL,   TMM_POOL_SIZE_DL,   0,      5/6,    500,    0,      0,      0,      TMM_USER_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_CTL,  TMM_POOL_SIZE_CTL,  0,      5/6,    500,    0,      0,      0,      TMM_USER_DATA_KI_POOL_ID)

# endif /* UPGRADE_EDGE */

#endif /* UPGRADE_3G */

/* ///////////////////// Control data pool configuration is currently same for all technologies /////////////////// */
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_UL_C, 10000,              0,      0,      0,      0,      0,      0,      TMM_CTRL_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_DL_C, 10000,              0,      0,      0,      0,      0,      0,      TMM_CTRL_DATA_KI_POOL_ID)
TMM_MEM_POOL_DEF_MAPPED (TMM_POOL_DL_INT, TMM_POOL_SIZE_DL_INT,  0,      0,    500,      0,      0,      0,      TMM_USER_DATA_INT_KI_POOL_ID)

/***************************************************************************
 *  TMM Profiles
 **************************************************************************/
/* TMM Profiles map Mapped pools to Virtual pools. The number of entries in each profile definition
 * after the profile name must be the same number as the number of virtual pools.
 *
 * For example:
 * if TMM_PROFILE_DEFAULT is selected, the pool properties of TMM_POOL_UL will be copied into
 * the TMM_DEFAULT_POOL_UL pool properties, and the pool properties of TMM_POOL_DL will
 * be copied into TMM_DEFAULT_POOL_DL and so on.
 *
 * On initialisation, the first Profile is set as the current profile.
 *
 *=============================================================================================================================
 *        Profile name TMM_DEFAULT_POOL_UL TMM_DEFAULT_POOL_DL TMM_DEFAULT_POOL_CTL TMM_DEFAULT_POOL_UL_C TMM_DEFAULT_POOL_DL_C
 *============================================================================================================================*/

/* We currently use one (default) profile */
TMM_PROFILE_DEF (TMM_PROFILE_DEFAULT,   TMM_POOL_UL,    TMM_POOL_DL,    TMM_POOL_CTL,   TMM_POOL_UL_C,  TMM_POOL_DL_C, TMM_POOL_DL_INT)

/* Undefine the access macros */
#undef TMM_GET_VIRT_POOL_NAME
#undef TMM_GET_REAL_POOL_NAME
#undef TMM_GET_MAPPED_POOL_NAME

#undef TMM_SET_VIRT_POOL_TABLE
#undef TMM_SET_REAL_POOL_TABLE
#undef TMM_SET_MAPPED_POOL_TABLE

#undef TMM_GET_PROFILE_NAME
#undef TMM_SET_PROFILE_TABLE

/* Undefine the access switches */
#undef TMM_MEM_POOL_DEF_VIRT
#undef TMM_MEM_POOL_DEF_REAL
#undef TMM_MEM_POOL_DEF_MAPPED
#undef TMM_PROFILE_DEF

/* END OF FILE */
