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

#ifndef __HAL_DWT_H__
#define __HAL_DWT_H__

#include "hal_platform.h"
#include "core_cm4.h"

#ifdef HAL_DWT_MODULE_ENABLED

typedef enum { // now only suppuort for EMITRANGE=0, CYCMATCH = 0
    COMP_DISABLE = 0,
    WDE_INST = 4, // 0x0100: Generate PC watchpoint debug event whem instruction fetches
    WDE_DATA_RO,  //    0x0101: Generate watchpoint debug event when read data accesses
    WDE_DATA_WO,  //    0x0110: Generate watchpoint debug event when write data accesses
    WDE_DATA_RW   //    0x0111: Generate watchpoint debug event when read/write data accesses
} DWT_FUNC_TYPE;

typedef enum {
    HAL_DWT_0 = 0,
    HAL_DWT_1 = 1,
    HAL_DWT_2 = 2,
    HAL_DWT_3 = 3,  /* used for pxCurrentTCB stack overflow check */
    HAL_DWT_MAX
} DWT_COMPARATOR_PORT;

typedef enum {
    SIZE_BYTE = 0,
    SIZE_HALFWORD = 1,
    SIZE_WORD = 2
} DWT_DATA_SIZE;

typedef enum {
    VALUE_COMP_LINK_NO_ADDR = 0,
    VALUE_COMP_LINK_ONE_ADDR = 1,
    VALUE_COMP_LINK_TWO_ADDR = 2
} DWT_COMP_ADDR_TYPE;

typedef struct {
    uint32_t value;
    DWT_DATA_SIZE size;
    DWT_FUNC_TYPE func;
    DWT_COMP_ADDR_TYPE comp_type;
    uint32_t addr_base0;
    uint32_t addr_mask0;
    uint32_t addr_base1;
    uint32_t addr_mask1;
} hal_dwt_value_comp_config_t;


/*  cycle count is deleted by HW, can not work on MT2625 */
#if 0
/* init DWT CYCNT function */
#define DWT_RESET_CYCLECOUNTER(time) \
do {\
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; \
    DWT->CYCCNT = 0; \
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; \
} while(0)

#define DWT_BENCHMARK_START(time) \
do{ \
time = DWT->CYCCNT; \
} while(0)

#define DWT_BENCHMARK_END(time) \
do{ \
time = DWT->CYCCNT; \
} while(0)

/*the read DWT_CYCCNT's SW overhead is 4 cycles*/
#define DWT_BENCHMARK_PRINTF(string,start,end) \
do{ \
printf(string); \
printf(" cycle(cpu count):%d\r\n",end-start-4); \
} while(0)
#else
#include "hal.h"

#define DWT_RESET_CYCLECOUNTER(time) \
do{\
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &time); \
} while(0)

#define DWT_BENCHMARK_START(time) \
do{ \
time = ((GPT_REGISTER_T *)(_GPT4_BASEADDR_))->GPT_COUNT; \
} while(0)

#define DWT_BENCHMARK_END(time) \
do{ \
time = ((GPT_REGISTER_T *)(_GPT4_BASEADDR_))->GPT_COUNT; \
} while(0)

#define DWT_BENCHMARK_PRINTF(string,start,end) \
do{ \
printf(string); \
printf(" cycle(us):%d\r\n",end-start); \
} while(0)

#endif

/* DWT APIs **********************************************************/
void hal_dwt_reset(void);
void hal_dwt_init(void);
void hal_dwt_dump_status(void);
void hal_dwt_port_disable(DWT_COMPARATOR_PORT port);
int32_t hal_dwt_request_watchpoint(uint32_t index, uint32_t addr_base, uint32_t addr_mask, DWT_FUNC_TYPE func);

/**
 * @brief This function enables DWT data value comparsion functions. User can use this API to configure some detailed setting for DWT data value
 *        comparsion functions and enable this function. For data value comparsion, user can configure three options for data match:
 *        1, data value comparsion is made for all read accesses(RO), all write accesses(WO),or all read and write accesses(RW);
 *        2, only if the address of the access matches the address specified by configuring the data access address region;
 *        3, in either of two address region.
 *        If user configure the data value match linking with one address region, the addr_base0 is configured for data comparsion address base,
 *        addr_mask0 for address mask, and DWT0 is for default address comparsion function. Or two address region, the addr_base0 and addr_base1
 *        is configured for data comparsion address base, addr_mask0 and addr_mask0 for address mask, DWT0 and DWT2 are for address comparsion function.
 * @param[in] dwt_config is the configuration setting for the data value watchpiont comparsion function.
 * @return -1, fail.
 *         0, success.
 *!! Note: the addr_base should be 2^(addr_mask) byte alignment, otherwise the behavior is UNPREDICTABLE !!
 *!! Note: only enable hardware stack overflow check by the DWT when halting debug is disabled, because under halting-mode, the ICE will take over the DWT function.
 *         The halting debug status can be checked by the bit(C_DEBUGEND), which is set when debugger is connected.
 *!! Note:
             Comparator 3 is used to check pxCurrentTCB stack
             Comparator 1 is used to data value comparsion function
             Comparator 2/0 is used to link data address comparsion function if Comparator 1 is configured for data value comparsion function
 * @par  Example
 * @code
 *       //track halfword data(0x1234) for all read access in any address
 *
 *       hal_dwt_value_comp_config_t dwt_config;
 *       dwt_config.value = 0x1234;
 *       dwt_config.size = SIZE_HALFWORD;
 *       dwt_config.func = WDE_DATA_RO;
 *       dwt_config.comp_type = VALUE_COMP_LINK_NO_ADDR;
 *
 *       hal_dwt_request_value_watchpoint(&dwt_config);
 *
 * @endcode
 * @code
 *       //track word data(0x12345678) for write access in both address of test_data[0] and test_data[1] address
 *       uint32_t test_data[2];
 *
 *       hal_dwt_value_comp_config_t dwt_config;
 *       dwt_config.value = 0x12345678;
 *       dwt_config.size = SIZE_WORD;
 *       dwt_config.func = WDE_DATA_WO;
 *       dwt_config.comp_type = VALUE_COMP_LINK_ONE_ADDR;
 *       dwt_config.addr_base0 = &test_data;                //compared data value address base
 *       dwt_config.addr_mask0 = 3;                         //mask base address bit[2:0], for 2 words
 *
 *       hal_dwt_request_value_watchpoint(&dwt_config);
 *
 * @endcode
 * @code
 *       //track word data(0x12345678) for read and write access in either address of test_data_1 and test_data_2 address
 *       uint32_t test_data_1;
 *       uint32_t test_data_2;
 *
 *       hal_dwt_value_comp_config_t dwt_config;
 *       dwt_config.value = 0x12345678;
 *       dwt_config.size = SIZE_WORD;
 *       dwt_config.func = WDE_DATA_RW;
 *       dwt_config.comp_type = VALUE_COMP_LINK_TWO_ADDR;
 *       dwt_config.addr_base0 = &test_data_1;              //one of compared data value address base
 *       dwt_config.addr_mask0 = 2;                         //mask base address bit[1:0], for 1 words
 *       dwt_config.addr_base1 = &test_data_2;              //the other compared data value address base
 *       dwt_config.addr_mask1 = 2;                         //mask base address bit[1:0], for 1 words
 *
 *       hal_dwt_request_value_watchpoint(&dwt_config);
 *
 * @endcode
 */
int32_t hal_dwt_request_value_watchpoint(hal_dwt_value_comp_config_t* dwt_config);

#endif /* HAL_DWT_MODULE_ENABLED */
#endif /* __DWT_H__ */
