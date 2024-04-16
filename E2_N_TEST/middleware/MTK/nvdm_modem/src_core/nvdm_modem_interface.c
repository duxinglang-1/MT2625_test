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


/********************************************************
 * Macro & Define
 *
 ********************************************************/
#define CRITICAL_SECTION(WORKS) {\
                                    nvdm_modem_port_mutex_take();\
                                    do{WORKS}while(0);\
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
nvdm_modem_status_t nvdm_modem_read_normal_data_item(const char *group_name,
        const char *data_item_name,
        nvdm_modem_data_item_type_t *type,
        uint8_t *buffer,
        uint32_t *size)
{
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_OK;

    CRITICAL_SECTION({
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NORMAL);
        ret = nvdm_modem_read_data_item(group_name, data_item_name, type, buffer, size);
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    });

    return ret;
}

nvdm_modem_status_t nvdm_modem_write_normal_data_item(const char *group_name,
        const char *data_item_name,
        nvdm_modem_data_item_type_t type,
        const uint8_t *buffer,
        uint32_t size,
        nvdm_modem_attr_enum_t attr)
{
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_OK;

    CRITICAL_SECTION({
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_PROTECTED);
        ret = nvdm_modem_is_data_item_found(group_name, data_item_name);
        if (ret == NVDM_MODEM_STATUS_OK)
        {
            nvdm_modem_port_log_notice("[WRITE][NORMAL][ERROR]data have same name on other area(%s,%s)", group_name, data_item_name);
            ret = NVDM_MODEM_STATUS_ERROR;
        } else
        {
            nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NORMAL);

            ret = nvdm_modem_write_data_item(group_name, data_item_name, type, buffer, size);
            if (NVDM_MODEM_STATUS_OK == ret && attr & NVDM_MODEM_ATTR_BACKUP)
            {
                nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_BACKUP);
                ret = nvdm_modem_write_data_item(group_name, data_item_name, type, buffer, size);
            }
        }
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    });

    return ret;
}

nvdm_modem_status_t nvdm_modem_invalidate_normal_data_item(const char *group_name,
        const char *data_item_name)
{
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_OK;

    CRITICAL_SECTION({
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NORMAL);
        ret = nvdm_modem_delete_data_item(group_name, data_item_name);
        if (ret != NVDM_MODEM_STATUS_OK) {
            nvdm_modem_port_log_notice("[INVALID][NORMAL][ERROR]normal area (%s,%s)", group_name, data_item_name);
        } else {
            nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_BACKUP);
            ret = nvdm_modem_delete_data_item(group_name, data_item_name);
            if (ret != NVDM_MODEM_STATUS_OK) {
                nvdm_modem_port_log_notice("[INVALID][NORMAL][ERROR]backup area (%s,%s)", group_name, data_item_name);
            }
            if (ret == NVDM_MODEM_STATUS_ITEM_NOT_FOUND) {
                ret = NVDM_MODEM_STATUS_OK;
            }
        }
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    });

    return ret;
}

nvdm_modem_status_t nvdm_modem_reset_normal_data_item(const char *group_name,
        const char *data_item_name)
{
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_OK;
    uint32_t size;

    CRITICAL_SECTION({
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_BACKUP);

        ret = nvdm_modem_get_data_item_size(group_name, data_item_name, &size);
        if (NVDM_MODEM_STATUS_OK == ret)
        {
            uint8_t buf[size];
            nvdm_modem_data_item_type_t type;

            ret = nvdm_modem_read_data_item(group_name, data_item_name, &type, buf, &size);
            if (NVDM_MODEM_STATUS_OK == ret) {
                nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NORMAL);
                ret = nvdm_modem_write_data_item(group_name, data_item_name, type, buf, size);
                if (NVDM_MODEM_STATUS_OK != ret) {
                    nvdm_modem_port_log_notice("[RESET][NORMAL][ERROR]write(%s,%s)", group_name, data_item_name);
                }
            } else {
                nvdm_modem_port_log_notice("[RESET][NORMAL][ERROR]read(%s,%s)", group_name, data_item_name);
                ret = NVDM_MODEM_STATUS_ERROR;
            }
        } else {
            nvdm_modem_port_log_notice("[RESET][NORMAL][ERROR]get size(%s,%s)", group_name, data_item_name);
            ret = NVDM_MODEM_STATUS_NO_BACKUP;
        }
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    });

    return ret;
}

nvdm_modem_status_t nvdm_modem_read_protected_data_item(const char *group_name,
        const char *data_item_name,
        nvdm_modem_data_item_type_t *type,
        uint8_t *buffer,
        uint32_t *size)
{
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_OK;

    CRITICAL_SECTION({
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_PROTECTED);
        ret = nvdm_modem_read_data_item(group_name, data_item_name, type, buffer, size);
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    });

    return ret;
}

nvdm_modem_status_t nvdm_modem_write_protected_data_item(const char *group_name,
        const char *data_item_name,
        nvdm_modem_data_item_type_t type,
        const uint8_t *buffer,
        uint32_t size,
        nvdm_modem_attr_enum_t attr)
{
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_OK;

    CRITICAL_SECTION({
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NORMAL);
        ret = nvdm_modem_is_data_item_found(group_name, data_item_name);
        if (ret == NVDM_MODEM_STATUS_OK)
        {
            nvdm_modem_port_log_notice("[WRITE][PROTECTED][ERROR]data have same name on other area(%s,%s)", group_name, data_item_name);
            ret = NVDM_MODEM_STATUS_ERROR;
        } else 
        {
            nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_PROTECTED);

            ret = nvdm_modem_write_data_item(group_name, data_item_name, type, buffer, size);
            if (NVDM_MODEM_STATUS_OK == ret && attr & NVDM_MODEM_ATTR_BACKUP)
            {
                nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_BACKUP);
                ret = nvdm_modem_write_data_item(group_name, data_item_name, type, buffer, size);
            }
        }
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    });

    return ret;
}

nvdm_modem_status_t nvdm_modem_invalidate_protected_data_item(const char *group_name,
        const char *data_item_name)
{
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_OK;

    CRITICAL_SECTION({
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_PROTECTED);
        ret = nvdm_modem_delete_data_item(group_name, data_item_name);
        if (ret != NVDM_MODEM_STATUS_OK) {
            nvdm_modem_port_log_notice("[INVALID][PROTECTED][ERROR]protected area (%s,%s)", group_name, data_item_name);
        } else {
            nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_BACKUP);
            ret = nvdm_modem_delete_data_item(group_name, data_item_name);
            if (ret != NVDM_MODEM_STATUS_OK) {
                nvdm_modem_port_log_notice("[INVALID][PROTECTED][ERROR]backup area (%s,%s)", group_name, data_item_name);
            }
            if (ret == NVDM_MODEM_STATUS_ITEM_NOT_FOUND) {
                ret = NVDM_MODEM_STATUS_OK;
            }
        }
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    });

    return ret;
}

nvdm_modem_status_t nvdm_modem_reset_protected_data_item(const char *group_name,
        const char *data_item_name)
{
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_OK;
    uint32_t size;

    CRITICAL_SECTION({
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_BACKUP);

        ret = nvdm_modem_get_data_item_size(group_name, data_item_name, &size);
        if (NVDM_MODEM_STATUS_OK == ret)
        {
            uint8_t buf[size];
            nvdm_modem_data_item_type_t type;

            ret = nvdm_modem_read_data_item(group_name, data_item_name, &type, buf, &size);
            if (NVDM_MODEM_STATUS_OK == ret) {
                nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_PROTECTED);
                ret = nvdm_modem_write_data_item(group_name, data_item_name, type, buf, size);
                if (NVDM_MODEM_STATUS_OK != ret) {
                    nvdm_modem_port_log_error("[RESET][PROTECTED][ERROR]write(%s,%s)", group_name, data_item_name);
                }
            } else {
                nvdm_modem_port_log_error("[RESET][PROTECTED][READ]read(%s,%s)", group_name, data_item_name);
                ret = NVDM_MODEM_STATUS_ERROR;
            }
        } else {
            nvdm_modem_port_log_info("[RESET][PROTECTED][ERROR]get size(%s,%s)", group_name, data_item_name);
            ret = NVDM_MODEM_STATUS_NO_BACKUP;
        }
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    });

    return ret;
}

nvdm_modem_status_t nvdm_modem_read_otp_data_item(const char *data_item_name,
        uint8_t *buffer,
        uint32_t *size)
{
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_OK;

    CRITICAL_SECTION({
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_OTP);
        ret = nvdm_modem_read_otp_data(data_item_name, buffer, size);
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    });

    return ret;
}

nvdm_modem_status_t nvdm_modem_write_otp_data_item(const char *data_item_name,
        const uint8_t *buffer,
        uint32_t size)
{
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_OK;

    CRITICAL_SECTION({
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_OTP);
        ret = nvdm_modem_write_otp_data(data_item_name, buffer, size);
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    });

    return ret;
}

nvdm_modem_status_t nvdm_modem_lock_otp_area(void)
{
    nvdm_modem_status_t ret = NVDM_MODEM_STATUS_OK;

    CRITICAL_SECTION({
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_OTP);
        ret = nvdm_modem_lock_otp();
        nvdm_modem_port_set_working_area(NVDM_MODEM_AREA_NONE);
    });

    return ret;
}

#endif

