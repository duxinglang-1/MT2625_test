/* Copyright Statement:
 *
 * (C) 2005-2017 MediaTek Inc. All rights reserved.
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
 * the License Agreement ("Permitted User"). If you are not a Permitted User,
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

#include "tel_conn_mgr_platform.h"


#if TEL_CONN_MGR_DEBUG
log_create_module(tel_conn_mgr, PRINT_LEVEL_INFO);
#endif


tel_conn_mgr_mutex_hdl_t tel_conn_mgr_mutex_create(void)
{
    return xSemaphoreCreateMutex();
}


void tel_conn_mgr_mutex_free(tel_conn_mgr_mutex_hdl_t handle)
{
    if (handle)
    {
        vSemaphoreDelete(handle);
    }
}


void tel_conn_mgr_mutex_lock(tel_conn_mgr_mutex_hdl_t handle)
{
    if (handle)
    {
        //TEL_CONN_MGR_LOG_INFO("lock mutex.");
        //printf("lock mutex.\r\n");
        while (xSemaphoreTake(handle, portMAX_DELAY) != pdPASS);
        #if 0
        {
            TEL_CONN_MGR_LOG_INFO("Wait for mutex.");
            vTaskDelay(100);
        }
        #endif
        //TEL_CONN_MGR_LOG_INFO("lock mutex done.");
        //printf("lock mutex done.\r\n");
    }
}


void tel_conn_mgr_mutex_unlock(tel_conn_mgr_mutex_hdl_t handle)
{
    if (handle)
    {
        xSemaphoreGive(handle);
        //TEL_CONN_MGR_LOG_INFO("unlock mutex.");
        //printf("unlock mutex.\r\n");
    }
}


tel_conn_mgr_bool tel_conn_mgr_send_msg(tel_conn_mgr_queue_hdl_t queue_hdl, void *msg)
{
    if (!queue_hdl)
    {
        return TEL_CONN_MGR_FALSE;
    }

    /* xQueueSend(): The item is queued by copy, not by reference. */
    return xQueueSend(queue_hdl, (void *)&msg, (TickType_t)0) == pdPASS;
}

