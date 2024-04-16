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
 ***************************************************************************/
 /** \File 
  * Buffer Memory Manager (BMM) Configuration.
  */

/***************************************************************************
* Nested Include Files
***************************************************************************/
#ifndef BMM_CFG_H
#define BMM_CFG_H


/***************************************************************************
 *  Constants
 **************************************************************************/

/***************************************************************************
 * Buffer Configuration
 ***************************************************************************/

/***************************************************************************
 * BMM BUFFER SIZE
 ***************************************************************************/
#define BMM_BUFFER_POWER_2_SIZE     16    /* 2 to the power N=2^16 = 65536 */
#define BMM_BUFFER_SIZE             (1 << BMM_BUFFER_POWER_2_SIZE)

/***************************************************************************
 * REFERENCE BLOCK (LTE/TGGE/WGGE)
 ***************************************************************************/
#define BMM_BLOCK_POWER_2_SIZE      11   /* 2 to the power N =2^11 =  2048 */

#define BMM_BLOCK_SIZE              (1 << BMM_BLOCK_POWER_2_SIZE)

#define BMM_MAX_REF_PER_BLOCK       255  /* Max number of allocations per block */
                                    

/*         Start                                         End   */
/* UL & DL  |                                             |    */
/* POOL     |xxxxxxxxxxxxxxxxxxx....xxxxxxxxxxxxxxxxxxxxxx|    */ 
/*           \  /\  /\  /\  /\                   \   /\  /     */
/* REF        \/  \/  \/  \/  \                   \ /  \/      */
/* Buffer     r   r    r  r  ...................   r    r      */

/***************************************************************************
 * BMM memory pool illustration:
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
 * 1. BMM_UL_DL_POOL_SIZE:
 *    Total BMM size = G
 * 2. BMM_UL_POOL_RATIO:
 *    The ratio of UL memory pool size to total pool size = (C / G)
 *    Used for computing UL & DL pool sizes (C & F) from the total pool size (G).
 * 3. HWM / pool size ratio = (B / C) and (E / F)
 *    These are defined per pool (by TMM_MEM_POOL_DEF macros) and are used to calculate UL & DL HWM (B & E)
 *    from UL & DL pool sizes (C & F).
 * 4. LWM / HWM ratio = (A / B) and (D / E)
 *    These are defined per pool (by TMM_MEM_POOL_DEF macros) and are used to calculate UL & DL LWM (A & D)
 *    from UL & DL HWM (B & E).
 ***************************************************************************/



#if defined (UPGRADE_LTE)
/***************************************************************************
 * UPLINK & DOWNLINK POOLS (LTE)
 ***************************************************************************/
#define BMM_CAT_4_MEMORY            (32*BMM_BUFFER_SIZE) /* = 2097152 or 2.048MB LTE CAT 4 Buffer */
#define BMM_CAT_3_MEMORY            (25*BMM_BUFFER_SIZE) /* = 1638400 or 1.6MB   LTE CAT 3 Buffer */    
#define BMM_CAT_TEST_MEMORY         (4*BMM_BUFFER_SIZE) /* =393216 Buffer Size ~400K */

#if defined (GX2)
#define BMM_UL_DL_POOL_SIZE         BMM_CAT_4_MEMORY
#else
#define BMM_UL_DL_POOL_SIZE         BMM_CAT_TEST_MEMORY 
#endif

/***************************************************************************
 * High and Low Watermark Requirement
 ***************************************************************************/
#define BMM_HWM_RATIO               5/6
#define BMM_LWM_HWM_RATIO           5/6
#define BMM_UL_HWM_THRESHOLD        (BMM_UL_DL_POOL_SIZE * BMM_HWM_RATIO)
#define BMM_UL_LWM_THRESHOLD        (BMM_UL_HWM_THRESHOLD * BMM_LWM_HWM_RATIO)

#define BMM_DL_HWM_THRESHOLD        (BMM_UL_DL_POOL_SIZE * BMM_HWM_RATIO)
#define BMM_DL_LWM_THRESHOLD        (BMM_DL_HWM_THRESHOLD * BMM_LWM_HWM_RATIO)

#else

/***************************************************************************
 * UPLINK & DOWNLINK POOLS (TGGE/WGGE)
 ***************************************************************************/
#define BMM_UL_DL_POOL_SIZE         (6*BMM_BUFFER_SIZE) /* =393216 Buffer Size ~400K */

#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_HSDPA)
#define BMM_UL_POOL_RATIO           1/2  
#define BMM_UL_POOL_HWM_RATIO       5/6     /*  Uplink High watermark ratio */
#define BMM_DL_POOL_HWM_RATIO       5/6     /* Downlink High watermark ratio */
# else
#define BMM_UL_POOL_RATIO           3/7
#define BMM_UL_POOL_HWM_RATIO       1/2     /* Uplink High watermark ratio */
#define BMM_DL_POOL_HWM_RATIO       5/6     /* Downlink High watermark ratio */
# endif
#else /* UPGRADE_3G */
# if defined (UPGRADE_EDGE)
#define BMM_UL_POOL_RATIO           3/4
#define BMM_UL_POOL_HWM_RATIO       1/2     /* Uplink High watermark ratio */
#define BMM_DL_POOL_HWM_RATIO       5/6     /* Downlink High watermark ratio */
# else /* UPGRADE_EDGE */
#define BMM_UL_POOL_RATIO           2/3
#define BMM_UL_POOL_HWM_RATIO       1/2     /* Uplink High watermark ratio */
#define BMM_DL_POOL_HWM_RATIO       5/6     /* Downlink High watermark ratio */
# endif /* UPGRADE_EDGE */
#endif /* UPGRADE_3G */

#define BMM_LWM_HWM_RATIO           5/6

#define BMM_UL_POOL_SIZE            (BMM_UL_DL_POOL_SIZE * BMM_UL_POOL_RATIO) 
#define BMM_DL_POOL_SIZE            (BMM_UL_DL_POOL_SIZE - BMM_UL_POOL_SIZE)


/***************************************************************************
 * High and Low Watermark Requirement
 ***************************************************************************/
#define BMM_UL_HWM_THRESHOLD        (BMM_UL_POOL_SIZE * BMM_UL_POOL_HWM_RATIO)
#define BMM_UL_LWM_THRESHOLD        (BMM_UL_HWM_THRESHOLD * BMM_LWM_HWM_RATIO)

#define BMM_DL_HWM_THRESHOLD        (BMM_DL_POOL_SIZE * BMM_DL_POOL_HWM_RATIO)
#define BMM_DL_LWM_THRESHOLD        (BMM_DL_HWM_THRESHOLD * BMM_LWM_HWM_RATIO)

#endif /* UPGRADE_LTE */

/***************************************************************************
 * CONTROL POOL (LTE/TGGE/WGGE)
 ***************************************************************************/
#define BMM_CONTROL_POOL_SIZE       BMM_BUFFER_SIZE

/***************************************************************************
 * Overall Memory Requirement
 ***************************************************************************/

#define BMM_TOTAL_MEMORY            (BMM_UL_DL_POOL_SIZE + BMM_CONTROL_POOL_SIZE)

#define BMM_NUM_REF_BLOCKS          (BMM_BUFFER_SIZE  >> BMM_BLOCK_POWER_2_SIZE)

#define BMM_NUM_BUFFERS             (BMM_TOTAL_MEMORY >> BMM_BUFFER_POWER_2_SIZE)

#define BMM_CONTROL_BUFFER          (BMM_NUM_BUFFERS - 1)
#define BMM_LAST_BUFFER             (BMM_CONTROL_BUFFER - 1)
#define BMM_FIRST_BUFFER            0
#define BMM_INVALID_BUFFER          0xFFFF

#define BMM_KI_DYNAMIC_NON_CACHEABLE_MEM_SIZE (150*1024)


#endif /* BMM_CFG_H */ 

/* END OF FILE */
