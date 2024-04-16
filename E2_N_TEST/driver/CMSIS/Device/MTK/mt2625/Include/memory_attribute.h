/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
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

/*****************************************************
  Usage:
   1. function: (a or b)
      a. ATTR_TEXT_IN_TCM int func(int par);
      b. ATTR_TEXT_IN_TCM int func(int par)
        {
        }
   2. RO data:
      a. ATTR_RODATA_IN_TCM const int b = 8;
   3. RW data:
      a. ATTR_RWDATA_IN_TCM int b = 8;
   4. ZI data:
      a. ATTR_ZIDATA_IN_TCM int b;
      
  Note: must put these attributes at HEAD of declaration.
*****************************************************/

#ifndef MEMORY_ATTRIBUTE_H_
#define MEMORY_ATTRIBUTE_H_

#ifndef __ICCARM__

/*AP part*/
#define ATTR_TEXT_IN_TCM              __attribute__ ((__section__(".tcm_code")))
#define ATTR_RODATA_IN_TCM            __attribute__ ((__section__(".tcm_rodata")))
#define ATTR_RWDATA_IN_TCM            __attribute__ ((__section__(".tcm_rwdata")))
#define ATTR_ZIDATA_IN_TCM            __attribute__ ((__section__(".tcm_zidata")))


#define ATTR_TEXT_IN_RAM              __attribute__ ((__section__(".ram_code")))
#define ATTR_RODATA_IN_RAM            __attribute__ ((__section__(".ram_rodata")))
#define ATTR_NONINIT_DATA_IN_RAM      __attribute__ ((__section__(".ram_non_init_data")))

#define ATTR_RWDATA_IN_NONCACHED_RAM  __attribute__ ((__section__(".noncached_rwdata")))
#define ATTR_ZIDATA_IN_NONCACHED_RAM  __attribute__ ((__section__(".noncached_zidata")))
#define ATTR_NONINIT_DATA_IN_NONCACHED_RAM  __attribute__ ((__section__(".noncached_non_init_data")))


#define ATTR_RWDATA_IN_NONCACHED_RAM_4BYTE_ALIGN  __attribute__ ((__section__(".noncached_rwdata"),__aligned__(4)))
#define ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN  __attribute__ ((__section__(".noncached_zidata"),__aligned__(4)))
#define ATTR_NONINIT_DATA_IN_NONCACHED_RAM_4BYTE_ALIGN  __attribute__ ((__section__(".noncached_non_init_data"),__aligned__(4)))


#define ATTR_RODATA_IN_SYSRAM            __attribute__ ((__section__(".sysram_rodata")))
#define ATTR_RWDATA_IN_SYSRAM            __attribute__ ((__section__(".sysram_rwdata")))
#define ATTR_ZIDATA_IN_SYSRAM             __attribute__ ((__section__(".sysram_zidata")))
 
#define ATTR_RODATA_IN_RETSRAM            __attribute__ ((__section__(".retsram_rodata")))
#define ATTR_RWDATA_IN_RETSRAM            __attribute__ ((__section__(".retsram_rwdata")))
#define ATTR_ZIDATA_IN_RETSRAM            __attribute__ ((__section__(".retsram_zidata")))

#define ATTR_GKI_STACK_IN_RAM            __attribute__ ((__section__(".gki_stack")))

/*MD part*/
#define ATTR_MD_TEXT_IN_FLASH              __attribute__ ((__section__(".md_text")))
#define ATTR_MD_RODATA_IN_FLASH            __attribute__ ((__section__(".md_rodata")))

#define ATTR_MD_TEXT_IN_TCM              __attribute__ ((__section__(".md_tcm_code")))
#define ATTR_MD_RODATA_IN_TCM            __attribute__ ((__section__(".md_tcm_rodata")))
#define ATTR_MD_RWDATA_IN_TCM            __attribute__ ((__section__(".md_tcm_rwdata")))
#define ATTR_MD_ZIDATA_IN_TCM            __attribute__ ((__section__(".md_tcm_zidata")))

#define ATTR_MD_TEXT_IN_RAM              __attribute__ ((__section__(".md_ram_code")))
#define ATTR_MD_RODATA_IN_RAM            __attribute__ ((__section__(".md_ram_rodata")))
#define ATTR_MD_RWDATA_IN_RAM            __attribute__ ((__section__(".md_ram_rwdata")))
#define ATTR_MD_ZIDATA_IN_RAM            __attribute__ ((__section__(".md_ram_zidata")))
#define ATTR_MD_NONINIT_DATA_IN_RAM      __attribute__ ((__section__(".md_ram_non_init_data")))

#define ATTR_MD_RWDATA_IN_NONCACHED_RAM  __attribute__ ((__section__(".md_noncached_ram_rwdata")))
#define ATTR_MD_ZIDATA_IN_NONCACHED_RAM  __attribute__ ((__section__(".md_noncached_ram_zidata")))
#define ATTR_MD_NONINIT_DATA_IN_NONCACHED_RAM        __attribute__ ((__section__(".md_noncached_ram_non_init_data")))

#define ATTR_MD_RWDATA_IN_NONCACHED_RAM_4BYTE_ALIGN  __attribute__ ((__section__(".md_noncached_ram_rwdata"),__aligned__(4)))
#define ATTR_MD_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN  __attribute__ ((__section__(".md_noncached_ram_zidata"),__aligned__(4)))
#define ATTR_MD_NONINIT_DATA_IN_NONCACHED_RAM_4BYTE_ALIGN  __attribute__ ((__section__(".md_noncached_ram_non_init_data"),__aligned__(4)))

  
#define ATTR_MD_RODATA_IN_RETSRAM            __attribute__ ((__section__(".md_retsram_rodata")))
#define ATTR_MD_RWDATA_IN_RETSRAM            __attribute__ ((__section__(".md_retsram_rwdata")))
#define ATTR_MD_ZIDATA_IN_RETSRAM            __attribute__ ((__section__(".md_retsram_zidata")))


#define ATTR_4BYTE_ALIGN              __attribute__ ((__aligned__(4)))

#else

#define ATTR_TEXT_IN_TCM              _Pragma("location=\".tcm_code\"")
#define ATTR_RODATA_IN_TCM            _Pragma("location=\".tcm_rodata\"")
#define ATTR_RWDATA_IN_TCM            _Pragma("location=\".tcm_rwdata\"")
#define ATTR_ZIDATA_IN_TCM            _Pragma("location=\".tcm_zidata\"")


#define ATTR_TEXT_IN_RAM              _Pragma("location=\".ram_code\"")
#define ATTR_RODATA_IN_RAM            _Pragma("location=\".ram_rodata\"")
#define ATTR_RWDATA_IN_NONCACHED_RAM  _Pragma("location=\".noncached_rwdata\"")
#define ATTR_ZIDATA_IN_NONCACHED_RAM  _Pragma("location=\".noncached_zidata\"")

#define ATTR_RWDATA_IN_NONCACHED_RAM_4BYTE_ALIGN  _Pragma("location=\".noncached_rwdata\"") \
                                                  _Pragma("data_alignment=4")
#define ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN  _Pragma("location=\".noncached_zidata\"") \
                                                  _Pragma("data_alignment=4")

#define ATTR_4BYTE_ALIGN              _Pragma("data_alignment=4")

#endif

#endif

