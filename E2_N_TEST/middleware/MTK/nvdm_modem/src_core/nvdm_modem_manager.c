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

#ifdef MTK_NVDM_MODEM_ENABLE

#include "nvdm_modem.h"
#include "nvdm_modem_port.h"
#include "nvdm_modem_internal.h"
#include "nvdm_modem_minidump.h"


/********************************************************
 * Macro & Define
 *
 ********************************************************/
#define CRITICAL_SECTION(WORKS) {\
                                    nvdm_modem_port_mutex_take();\
                                    do{WORKS}while(0);\
                                    nvdm_modem_port_mutex_give();\
                                }
#define CRITICAL_SECTION_ISR(WORKS) {\
                                        if ((portNVIC_INT_CTRL_REG & 0xff) == 0) \
                                            nvdm_modem_port_mutex_take();\
                                        do{WORKS}while(0);\
                                        if ((portNVIC_INT_CTRL_REG & 0xff) == 0) \
                                            nvdm_modem_port_mutex_give();\
                                    }
/********************************************************
 * Enum & Structures & Global variable
 *
 ********************************************************/

/********************************************************
 * Function declaration
 *
 ********************************************************/
nvdm_modem_status_t nvdm_modem_query_data_item_area(const char *group_name,
        const char *data_item_name,
        nvdm_modem_area_t *area)
{
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_OK;
    uint32_t i = 0;

    CRITICAL_SECTION({
        *area = NVDM_MODEM_AREA_NONE;
        for (i=0; i<=(NVDM_MODEM_AREA_END>>1); i++)
        {
            nvdm_modem_port_set_working_area((nvdm_modem_area_t)0x1<<i);
            ret = nvdm_modem_is_data_item_found(group_name, data_item_name);
            if (NVDM_MODEM_STATUS_OK == ret) {
                *area |= 0x1<<i;
            }
        }
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    });

    return NVDM_MODEM_STATUS_OK;
}

nvdm_modem_status_t nvdm_modem_query_data_item_number(nvdm_modem_area_t area,
        uint32_t *data_number)
{
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_OK;

    CRITICAL_SECTION({
        nvdm_modem_port_set_working_area(area);
        ret = nvdm_modem_query_data_item_count(data_number);
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    });

    return ret;
}

nvdm_modem_status_t nvdm_modem_query_all_data_item_info(nvdm_modem_area_t area,
        nvdm_modem_data_item_info_t *info_list,
        uint32_t count)
{
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_OK;
    uint32_t i;

    CRITICAL_SECTION({
        nvdm_modem_port_set_working_area(area);
        ret = nvdm_modem_query_all_data_item(info_list, count);
        if (ret == NVDM_MODEM_STATUS_OK) {
            /*Check backup area*/
            nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_BACKUP);
            for (i=0; i<count; i++) {
                if (NVDM_MODEM_STATUS_OK == nvdm_modem_is_data_item_found(info_list[i].group_name, info_list[i].data_item_name))
                    info_list[i].area |= NVDM_MODEM_AREA_BACKUP;
            }
        }
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    });

    return ret;
}

nvdm_modem_status_t nvdm_modem_query_mini_dump_number(uint8_t *dump_number)
{
#ifdef MTK_MINI_DUMP_ENABLE
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_OK;

    CRITICAL_SECTION_ISR({
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_MINIDUMP);
        ret = nvdm_modem_query_mini_dump_number_internal(dump_number);
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    });

#else
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_ERROR;
#endif
    return ret;
}

nvdm_modem_status_t nvdm_modem_write_mini_dump_data(uint8_t *buffer, uint16_t size)
{
#ifdef MTK_MINI_DUMP_ENABLE
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_OK;

    CRITICAL_SECTION_ISR({
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_MINIDUMP);
        ret = nvdm_modem_write_mini_dump_data_internal(buffer, size);
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    });
#else
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_ERROR;
#endif
    return ret;
}

nvdm_modem_status_t nvdm_modem_read_mini_dump_data(uint8_t dump_index,
                                                                uint16_t offset,
                                                                uint8_t *buffer,
                                                                uint16_t *size)
{
#ifdef MTK_MINI_DUMP_ENABLE
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_OK;

    CRITICAL_SECTION_ISR({
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_MINIDUMP);
        ret = nvdm_modem_read_mini_dump_data_internal(dump_index, offset, buffer, size);
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    });
#else
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_ERROR;
#endif
    return ret;
}

nvdm_modem_status_t nvdm_modem_clean_mini_dump_data(uint8_t dump_index)
{
#ifdef MTK_MINI_DUMP_ENABLE
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_OK;

    CRITICAL_SECTION_ISR({
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_MINIDUMP);
        ret = nvdm_modem_clean_mini_dump_data_internal(dump_index);
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    });
#else
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_ERROR;
#endif
    return ret;
}

#endif
