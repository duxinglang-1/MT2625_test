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

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "tel_conn_mgr_common.h"
#include "tel_conn_mgr_app_api_ext.h"
#include "tel_conn_mgr_example_ext.h"

#define TEL_CONN_MGR_EXAMPLE_EXT_MSG_QUEUE_MAX_SIZE (5)
#define MSG_ID_TEL_CONN_MGR_EXAMPLE_EXT_INFO    (10000)

typedef struct
{
    int msg_id;
    unsigned int app_id;
    tel_conn_mgr_info_type_enum info_type;
    tel_conn_mgr_bool result;
    tel_conn_mgr_err_cause_enum cause;
    tel_conn_mgr_pdp_type_enum pdp_type;
}tel_conn_mgr_example_ext_info_msg_struct;


static QueueHandle_t g_tcm_example_ext_msg_queue_hdl = NULL;
static int g_tcm_example_ext_msg_task_running = 0;


void tel_conn_mgr_example_ext_app_clean(void)
{
    /* App cleaning work. */
    TEL_CONN_MGR_EXAMPLE_EXT_LOG("App cleaning work");
}


void tel_conn_mgr_example_ext_app_do_things(unsigned int app_id)
{
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
    
    /* App can do its things here. */
    TEL_CONN_MGR_EXAMPLE_EXT_LOG("It is an example for the usage of tcm callback");

    /* When all is done and the bearer is not needed any more, deactivate the bearer. */
    ret = tel_conn_mgr_deactivate(app_id);
    TEL_CONN_MGR_EXAMPLE_EXT_LOG("tel_conn_mgr_deactivate() ret:%d", ret);
    if (TEL_CONN_MGR_RET_OK == ret || TEL_CONN_MGR_RET_IS_HOLD == ret)
    {
        /* The bearer is deactivated or hold by other app. Do app cleaning work. */
        tel_conn_mgr_example_ext_app_clean();

        /* Break the while loop in tel_conn_mgr_example_ext_task_main(). */
        g_tcm_example_ext_msg_task_running = 0;
    }
    else if (TEL_CONN_MGR_RET_WOULDBLOCK != ret)
    {
        /* Error handle */
    }
    /* else, wait for the callback being invoked. */
}


void tel_conn_mgr_example_ext_cb(unsigned int app_id,
                                           tel_conn_mgr_info_type_enum info_type,
                                           tel_conn_mgr_bool result,
                                           tel_conn_mgr_err_cause_enum cause,
                                           tel_conn_mgr_pdp_type_enum pdp_type)
{
    tel_conn_mgr_example_ext_info_msg_struct *msg = NULL;

    TEL_CONN_MGR_EXAMPLE_EXT_LOG("%s, app_id:%d, info_type:%d, result:%d, cause:%d, pdp_type:%d",
                                 __FUNCTION__, app_id, info_type, result, cause, pdp_type);
    if (!g_tcm_example_ext_msg_queue_hdl)
    {
        /* Cannot send message. Message losts. Error handle. */
    }
    else
    {
        msg = pvPortCalloc(1, sizeof(tel_conn_mgr_example_ext_info_msg_struct));
        if (!msg)
        {
            /* Allocation failed. Message losts. Error handle. */
        }
        else
        {
            /* Send message to the user's task. */
            msg->msg_id = MSG_ID_TEL_CONN_MGR_EXAMPLE_EXT_INFO;
            msg->app_id = app_id;
            msg->info_type = info_type;
            msg->result = result;
            msg->cause = cause;
            msg->pdp_type = pdp_type;

            if (xQueueSend(g_tcm_example_ext_msg_queue_hdl, (void *)&msg, (TickType_t)0) != pdPASS)
            {
                /* Message sending failed. Message losts. Error handle. */

                /* Free message since it is failed to send. */
                vPortFree(msg);
            }
            else
            {
                TEL_CONN_MGR_EXAMPLE_EXT_LOG("Succeed to send the info msg to user task");
            }
        }
    }
}


static void tel_conn_mgr_example_ext_task_main(void *param)
{
    tel_conn_mgr_example_ext_info_msg_struct *msg = NULL;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_ERROR;
    unsigned int app_id = 0;
    tel_conn_mgr_pdp_type_enum acted_pdp_type = TEL_CONN_MGR_PDP_TYPE_NONE;
    tel_conn_mgr_register_handle_t reg_hdl = NULL;

    g_tcm_example_ext_msg_task_running = 1;

    reg_hdl = tel_conn_mgr_register_callback(tel_conn_mgr_example_ext_cb);
    if (!reg_hdl)
    {
        /* Error handle */

        /* If waiting for no message, set g_tcm_example_ext_msg_task_running to 0. */
    }
    else
    {
        ret = tel_conn_mgr_activate_ext(TEL_CONN_MGR_BEARER_TYPE_NBIOT,
                                        TEL_CONN_MGR_SIM_ID_1,
                                        TEL_CONN_MGR_PDP_TYPE_IP,
                                        "apn_example_ext",
                                        "user_name_example",
                                        "password_example",
                                        reg_hdl,
                                        &app_id,
                                        &acted_pdp_type);
        TEL_CONN_MGR_EXAMPLE_EXT_LOG("tel_conn_mgr_activate_ext() ret:%d", ret);
        if (TEL_CONN_MGR_RET_OK == ret)
        {
            /* The bearer is activated. App can do its things now.
                        * When all is done and the bearer is not needed any more, deactivate the bearer. */
            tel_conn_mgr_example_ext_app_do_things(app_id);
        }
        else if (TEL_CONN_MGR_RET_WOULDBLOCK != ret)
        {
            /* Error handle */

            /* If waiting for no message, set g_tcm_example_ext_msg_task_running to 0. */
        }
        /* else, wait for the callback being invoked. */
    }

    while (g_tcm_example_ext_msg_task_running)
    {
        if (xQueueReceive(g_tcm_example_ext_msg_queue_hdl, &msg, portMAX_DELAY) == pdPASS)
        {
            switch (msg->msg_id)
            {
                case MSG_ID_TEL_CONN_MGR_EXAMPLE_EXT_INFO:
                {
                    TEL_CONN_MGR_EXAMPLE_EXT_LOG("info msg. app_id:%d, info_type:%d, result:%d, cause:%d, pdp_type:%d",
                                                 msg->app_id, msg->info_type, msg->result, msg->cause, msg->pdp_type);

                    switch (msg->info_type)
                    {
                        /* If there're more than one application within this task, user can check the app_id to identify them. */
                        case TEL_CONN_MGR_INFO_TYPE_ACTIVATION:
                        {
                            if (msg->result)
                            {
                                /* The bearer is activated. App can do its things now.
                                                             * When all is done and the bearer is not needed any more, deactivate the bearer. */
                                tel_conn_mgr_example_ext_app_do_things(app_id);
                            }
                            else
                            {
                                /* Error handle */
                            }
                            break;
                        }

                        case TEL_CONN_MGR_INFO_TYPE_ACTIVE_DEACTIVATION:
                        {
                            if (msg->result)
                            {
                                /* The bearer is deactivated. Do app cleaning work. */
                                tel_conn_mgr_example_ext_app_clean();
                                
                                /* Break the while loop in tel_conn_mgr_example_ext_task_main(). */
                                g_tcm_example_ext_msg_task_running = 0;
                            }
                            else
                            {
                                /* Error handle */
                            }
                            break;
                        }

                        case TEL_CONN_MGR_INFO_TYPE_PASSIVE_DEACTIVATION:
                        {
                            /* The bearer is inactive. Do app cleaning work. */
                            tel_conn_mgr_example_ext_app_clean(); 

                            /* Break the while loop in tel_conn_mgr_example_ext_task_main(). */
                            g_tcm_example_ext_msg_task_running = 0;
                            break;
                        }

                        default:
                            break;
                    }
                }

                default:
                    break;
            }

            /* Free the message after it has been proccessed. */
            vPortFree(msg);
            msg = NULL;
        }
    }

    /* Deregister callback */
    TEL_CONN_MGR_EXAMPLE_EXT_LOG("deregister callback. reg_hdl:%x.", reg_hdl);
    if (reg_hdl)
    {
        tel_conn_mgr_deregister_callback(reg_hdl);
    }
    
    TEL_CONN_MGR_EXAMPLE_EXT_LOG("Task delete.");

    if (g_tcm_example_ext_msg_queue_hdl)
    {
        vQueueDelete(g_tcm_example_ext_msg_queue_hdl);
        g_tcm_example_ext_msg_queue_hdl = NULL;
    }
    vTaskDelete(NULL);
}


void tel_conn_mgr_example_ext_entry(void)
{
    /* Init API should be called within the main() of the project for only one time. */
    // tel_conn_mgr_init();

    if (!g_tcm_example_ext_msg_queue_hdl)
    {
        /* Create a message queue to store the pointer of the message. */
        g_tcm_example_ext_msg_queue_hdl = xQueueCreate(TEL_CONN_MGR_EXAMPLE_EXT_MSG_QUEUE_MAX_SIZE,
                                                       sizeof(tel_conn_mgr_example_ext_info_msg_struct *));
    }

    if (g_tcm_example_ext_msg_queue_hdl)
    {
        xTaskCreate(tel_conn_mgr_example_ext_task_main,
                    TEL_CONN_MGR_EXAMPLE_EXT_TASK_NAME,
                    TEL_CONN_MGR_EXAMPLE_EXT_TASK_STACKSIZE/sizeof(portSTACK_TYPE),
                    NULL,
                    TEL_CONN_MGR_EXAMPLE_EXT_TASK_PRIO,
                    NULL);
    }
}

