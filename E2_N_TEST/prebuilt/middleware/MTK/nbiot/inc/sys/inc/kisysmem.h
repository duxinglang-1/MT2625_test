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
 * GKI header file that defines the fixed block memory pools and the size
 * of the dynamic heap memory pool.
 * This should never be included directly by any file outside of the kernel.
 **************************************************************************/

/*
** This file deliberately does not have a
** #if !defined KISYSMEM_H
** as it can be legitimately included twice
*/

/* Fixed Pool Sizes
 *
** Pool definitions should always be ordered from smallest to largest block
** size.
**
** ==========================================================================
**                              Pool            Block     Block  Request
**                              Name            Length    Count  Maximum
**==========================================================================*/

#if defined (KI_USE_EXMEM)
# include <exmem.h>
#else

#if !defined (KI_DISABLE_DYNAMIC_MEMORY)
# if !defined (ENABLE_PLTSIM)
# include <tmmmemcfg.h>  /* Required for the non-cacheable memory size */
# endif /* ENABLE_PLTSIM */
#endif /* KI_DISABLE_DYNAMIC_MEMORY */

# if !defined (KI_DISABLE_DYNAMIC_MEMORY)
/* Note: XLARGE pools are used for MPH_INT_MEAS_INDs */

#    if defined (UPGRADE_3G)
       KI_MEM_POOL_DEF (        SMALL,               24,    300,     60)
#     if defined (UPGRADE_3G_HSUPA)
       KI_MEM_POOL_DEF (       MEDIUM,              142,    300,     42)
#     else /* UPGRADE_3G_HSUPA */
       KI_MEM_POOL_DEF (       MEDIUM,              142,    210,     42)
#     endif /* UPGRADE_3G_HSUPA */
       KI_MEM_POOL_DEF (        LARGE,              292,    100,     12)
#     if defined (UPGRADE_EDGE)
       /* Preserve the extra requirements for both EDGE and 3G */
       KI_MEM_POOL_DEF (       XLARGE,              444,      2,      0)
#     endif /* UPGRADE_EDGE */
       KI_MEM_POOL_DEF (      XXLARGE,             1392,      1,      0)
#    else /* UPGRADE_3G */
#     if defined (UPGRADE_EDGE)
       KI_MEM_POOL_DEF (        SMALL,               24,    150,     60)
       KI_MEM_POOL_DEF (       MEDIUM,              142,    150,     42)
       KI_MEM_POOL_DEF (        LARGE,              292,     15,     12)
       KI_MEM_POOL_DEF (       XLARGE,              444,      2,      0)
#     else /* UPGRADE_EDGE */
       KI_MEM_POOL_DEF (        SMALL,               24,    125,     60)
       KI_MEM_POOL_DEF (       MEDIUM,              142,     97,     42)
       KI_MEM_POOL_DEF (        LARGE,              292,     15,     12)
       KI_MEM_POOL_DEF (       XLARGE,              444,      1,      0)
#     endif /* UPGRADE_EDGE */
#    endif /* UPGRADE_3G */
# else /* KI_DISABLE_DYNAMIC_MEMORY */
      /*
      ** Protocol Stack Only System Memory pools (haven't been optimised).
         NOTE!! Must be aligned. Use always power of 4.
         NOTE!! Must be aligned. Use always power of 4.
         NOTE!! Must be aligned. Use always power of 4.
      */
#if defined (FPGA_ENV)
    KI_MEM_POOL_DEF (        SMALL,               64,    384,    12)
    KI_MEM_POOL_DEF (       MEDIUM,              540,     64,     8)
    KI_MEM_POOL_DEF (        LARGE,             2048,     64,     2)
    KI_MEM_POOL_DEF (       XLARGE,             6000,     28,     5)
    KI_MEM_POOL_DEF (         HUGE,            14000,     10,     0)
#else
    KI_MEM_POOL_DEF (        SMALL,               64,    960,   112)
    KI_MEM_POOL_DEF (       MEDIUM,              540,     96,     8)
    KI_MEM_POOL_DEF (        LARGE,             2048,     72,     2)
    KI_MEM_POOL_DEF (       XLARGE,             6000,     32,     5)
    KI_MEM_POOL_DEF (         HUGE,            14000,     12,     0)
#endif
# endif /* KI_DISABLE_DYNAMIC_MEMORY */
#endif /* KI_USE_EXMEM */



/**************************************************************************************************
 * Dynamic Memory Pool Sizes
 *
 * This section defines the dynamic memory requirements for optional modules. Modules that require
 * an additional dynamic memory overhead should define the maximum amount of dynamic memory
 * required in this section.
 **************************************************************************************************/



/**************************************************************************************************
 * Data on-board
 **************************************************************************************************/

#  define ATCI_DYNAMIC_MEM_SIZE                   (0)

#if !defined (KI_DISABLE_DYNAMIC_MEMORY)
#if !defined (KI_DYNAMIC_MEM_SIZE)

#if defined (UPGRADE_3G)
/**************************************************************************************************
 * 3G
 **************************************************************************************************/
# if defined (UPGRADE_3G_HSDPA)
#  if defined (UPGRADE_3G_TDD128)
#   define KI_3G_EXTRA_MEM_SIZE            0x60000 /* 384K extra memory for 3G tasks */
                                                  /* RRC need 300K for decode msg in worst case */
#  else
#   define KI_3G_EXTRA_MEM_SIZE            0x70000 /* 448K extra memory for 3G tasks */
                                                  /* RRC need 300K for decode msg in worst case */
#  endif   
# else
#  define KI_3G_EXTRA_MEM_SIZE            0x20000 /* 128K extra memory for 3G tasks */
# endif  /* UPGRADE_3G_HSDPA */
#else /* UPGRADE_3G */
# if defined (UPGRADE_EDGE)
#  define KI_3G_EXTRA_MEM_SIZE            0
# else
#  define KI_3G_EXTRA_MEM_SIZE            0
# endif  /* UPGRADE_EDGE */
#endif  /* UPGRADE_3G */

/**************************************************************************************************
 * EBMM
 **************************************************************************************************/
# if (!defined (DISABLE_SNDCP_DATA_COMP)) || (!defined (VG_DISABLE_V42_BIS))
#  define KI_EBMM_MEM_SIZE_V42_BIS       0x2858 /* 10.1K */
# else
#  define KI_EBMM_MEM_SIZE_V42_BIS            0
# endif

# if !defined (VG_DISABLE_14400_DATA)
#  define KI_EBMM_MEM_SIZE_BASE          0x7488 /* 29.2K */
# else
#  define KI_EBMM_MEM_SIZE_BASE          0x2a30 /* 10.6K */
# endif

/* Header compression on WAP CSD */
# if defined (UPGRADE_WAP_PPP)
#  define KI_EBMM_MEM_SIZE_WAP_PPP       0x110c /* 4.3K */
# else
#  define KI_EBMM_MEM_SIZE_WAP_PPP            0
# endif

/**************************************************************************************************
 * Total EBMM dynamic memory usage
 * Total up the EBMM dynamic memory requirements for the optional modules, and add them to the
 * minimum EBMM dynamic memory allocation requirement.
 **************************************************************************************************/
#define KI_EBMM_MEM_SIZE   (KI_EBMM_MEM_SIZE_BASE    + \
                            KI_EBMM_MEM_SIZE_V42_BIS + \
                            KI_EBMM_MEM_SIZE_WAP_PPP)

/**************************************************************************************************
 * Total dynamic memory usage
 * Total up the dynamic memory requirements for the optional modules, and add them to the
 * minimum dynamic memory allocation.
 **************************************************************************************************/

/** \ingroup PrdGki
 * This is the size of the dynamic heap memory pool in the system. */
#define KI_DYNAMIC_MEM_SIZE              ( 0x0c000         /* 48K */  \
                                           +  ATCI_DYNAMIC_MEM_SIZE    \
                                           +  KI_EBMM_MEM_SIZE        \
                                           +  KI_3G_EXTRA_MEM_SIZE    \
                                         )

#define KI_DYNAMIC_NON_CACHEABLE_MEM_SIZE ( TMM_NON_CACHEABLE_POOL_SIZE )
#define KI_DYNAMIC_NON_CACHEABLE_INT_MEM_SIZE ( TMM_NON_CACHEABLE_INT_POOL_SIZE )

/** \ingroup PrdGki
 * This defines the amount of memory in the dynamic heap that can be allocated
 * via 'request' GKI memory calls (e.g. #KiRequestMemory). */
#define KI_DYNAMIC_MEM_REQUEST_LIMIT     ( 0x09000         /* 36K */  \
                                           +  ATCI_DYNAMIC_MEM_SIZE    \
                                           +  KI_EBMM_MEM_SIZE        \
                                           +  KI_3G_EXTRA_MEM_SIZE    \
                                         )

/** \ingroup PrdGki
 * If signals are created by tasks with a size greater than this define, the memory
 * for the signal will be allocated from the dynamic heap memory pool. Otherwise
 * they will be allocated from a fixed block pool. Signals created using
 * #KiCreateIntSignal (interrupt creating a signal) are always allocated from
 * a fixed block pool for speed reasons (unless #KI_DISABLE_FIXED_BLOCK_MEMORY
 * is defined). */
#define KI_DYNAMIC_MEM_SIG_THRESHOLD       0x00100 /* 256 */

# endif /* !KI_DYNAMIC_MEM_SIZE */
#endif /* !KI_DISABLE_DYNAMIC_MEMORY */

/* END OF FILE */
