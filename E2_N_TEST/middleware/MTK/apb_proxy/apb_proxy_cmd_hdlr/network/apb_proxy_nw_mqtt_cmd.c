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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "syslog.h"

#include "apb_proxy.h"
#include "apb_proxy_nw_mqtt_cmd.h"
#include "apb_proxy_nw_cmd_gprot.h"
#include "sockets.h"
#include "MQTTClient.h"
#include "syslog.h"

log_create_module(mqtt_at, PRINT_LEVEL_INFO);

#if !defined(MTK_DEBUG_LEVEL_NONE)
#define MQTT_AT_LOGI(fmt, args...)      LOG_I(mqtt_at, "[MQTT_AT] "fmt, ##args)
#define MQTT_AT_LOGW(fmt, args...)      LOG_W(mqtt_at, "[MQTT_AT] "fmt, ##args)
#define MQTT_AT_LOGE(fmt, args...)      LOG_E(mqtt_at, "[MQTT_AT] "fmt, ##args)
#else
#define MQTT_AT_LOGI(fmt, args...)
#define MQTT_AT_LOGW(fmt, args...)
#define MQTT_AT_LOGE(fmt, args...)
#endif

#define MQTT_AT_TASK_NAME               "MQTT_AT"
#define MQTT_AT_TASK_STACK_SIZE         (1024 * 2)
#define MQTT_AT_TASK_PRIORITY           (TASK_PRIORITY_NORMAL)

#define MQTT_AT_INSTANCE_NUM            (5)
#define MQTT_AT_RESPONSE_DATA_LEN       (100)
#define MQTT_AT_CMD_STRING_LEN          (1024)
#define MQTT_AT_CMD_PARAM_NUM           (20)
#define MQTT_AT_CMD_FILED_NUM           (10)

#define MQTT_AT_ERRID_UNKNOWN_ERROR             (0)
#define MQTT_AT_ERRID_SYSTEM_ERROR              (1)
#define MQTT_AT_ERRID_NETWORK_ERROR             (2)
#define MQTT_AT_ERRID_REGISTRATION_FAILURE      (3)
#define MQTT_AT_ERRID_OK                        (100)
#define MQTT_AT_ERRID_MEMORY_ERROR              (MQTT_AT_ERRID_OK + 1)
#define MQTT_AT_ERRID_PARAMETER_ERROR           (MQTT_AT_ERRID_OK + 2)
#define MQTT_AT_ERRID_NOT_SUPPORT               (MQTT_AT_ERRID_OK + 3)
#define MQTT_AT_ERRID_SDK_ERROR                 (MQTT_AT_ERRID_OK + 4)
#define MQTT_AT_ERRID_NOT_FOUND                 (MQTT_AT_ERRID_OK + 5)

#define MQTT_AT_CMDID_NEW                       (0)
#define MQTT_AT_CMDID_DELETE                    (1)
#define MQTT_AT_CMDID_CONNECT                   (2)
#define MQTT_AT_CMDID_DISCONNECT                (3)
#define MQTT_AT_CMDID_SUBSCRIBE                 (4)
#define MQTT_AT_CMDID_UNSUBSCRIBE               (5)
#define MQTT_AT_CMDID_PUBLISH                   (6)

typedef struct {
    bool is_used;
    bool is_connected;
    uint32_t mqtt_id;
    Network *network;
    Client client;
    messageHandler message_handler;
} mqtt_at_context_t;

static mqtt_at_context_t g_mqtt_at_context[MQTT_AT_INSTANCE_NUM];
static bool g_mqtt_at_task_running;
static SemaphoreHandle_t g_mqtt_at_mutex_handle;
extern int g_mqtt_id;

static void mqtt_at_disconnect_handler(Network *network);
void m2m_mqtt_message_handler(uint32_t mqtt_id, char *topic,uint32_t qoS,char retained,char dup,uint32_t message_len,char *message);
void m2m_mqtt_disconect_msg_pro(uint32_t mqtt_id);
void m2m_mqtt_connect_rsp(int error_code);
void dbg_print(char *fmt,...);

static void mqtt_at_message_handler(uint32_t mqtt_id, MessageData *message)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response;
    char *response_data;
    uint32_t response_len;
    char topicName[MQTT_AT_RESPONSE_DATA_LEN];
	dbg_print("mqtt_at_message_handler");

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    configASSERT(message != NULL && message->message != NULL);
    MQTT_AT_LOGI("topicName: %d, payloadlen: %d", (int)message->topicName->lenstring.len, message->message->payloadlen);
    if (message->message->payloadlen < 0) message->message->payloadlen = 0;
    response_data = (char *)pvPortMalloc(message->message->payloadlen * 2 + MQTT_AT_RESPONSE_DATA_LEN + 50);
    if (response_data == NULL) {
        MQTT_AT_LOGE("memory error");
    } else {
        response.result_code = APB_PROXY_RESULT_UNSOLICITED;
        // +EMQPUB: <mqtt_id>, <topic>, <QoS>, <retained>, <dup>, <message_len>, <message>
        if (message->topicName->lenstring.data != NULL && message->topicName->lenstring.len > 0) {
            if (message->topicName->lenstring.len < MQTT_AT_RESPONSE_DATA_LEN) {
                memcpy(topicName, message->topicName->lenstring.data, message->topicName->lenstring.len);
                topicName[message->topicName->lenstring.len] = '\0';
            } else {
                memcpy(topicName, message->topicName->lenstring.data, MQTT_AT_RESPONSE_DATA_LEN - 1);
                topicName[MQTT_AT_RESPONSE_DATA_LEN - 1] = '\0';
            }
        } else {
            topicName[0] = '\0';
        }
		m2m_mqtt_message_handler(mqtt_id,topicName,message->message->qos,message->message->retained,message->message->dup,message->message->payloadlen,message->message->payload);
        sprintf(response_data, "+EMQPUB: %d, \"%s\", %d, %d, %d, %d, \"",
                (int)mqtt_id, topicName, (int)message->message->qos,
                (int)message->message->retained, (int)message->message->dup, (int)message->message->payloadlen * 2);
        response_len = strlen(response_data);
        onenet_at_bin_to_hex(response_data + response_len, message->message->payload, message->message->payloadlen * 2);
        response_len = strlen(response_data);
        response_data[response_len] = '\"';
        response_len++;
        response.pdata = response_data;
        response.length = response_len;

        response.cmd_id = APB_PROXY_INVALID_CMD_ID;
        apb_proxy_send_at_cmd_result(&response);
    }

    if (response_data != NULL) {
        vPortFree(response_data);
    }
}

static void mqtt_at_message_handler_0(MessageData *message)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	dbg_print("mqtt_at_message_handler_0");
    MQTT_AT_LOGI("mqtt_at_message_handler_0");
    if (g_mqtt_at_context[0].is_used) {
        mqtt_at_message_handler(g_mqtt_at_context[0].mqtt_id, message);
    }
}

static void mqtt_at_message_handler_1(MessageData *message)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MQTT_AT_LOGI("mqtt_at_message_handler_1");
    if (g_mqtt_at_context[1].is_used) {
        mqtt_at_message_handler(g_mqtt_at_context[1].mqtt_id, message);
    }
}

static void mqtt_at_message_handler_2(MessageData *message)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MQTT_AT_LOGI("mqtt_at_message_handler_2");
    if (g_mqtt_at_context[2].is_used) {
        mqtt_at_message_handler(g_mqtt_at_context[2].mqtt_id, message);
    }
}

static void mqtt_at_message_handler_3(MessageData *message)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MQTT_AT_LOGI("mqtt_at_message_handler_3");
    if (g_mqtt_at_context[3].is_used) {
        mqtt_at_message_handler(g_mqtt_at_context[3].mqtt_id, message);
    }
}

static void mqtt_at_message_handler_4(MessageData *message)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MQTT_AT_LOGI("mqtt_at_message_handler_4");
    if (g_mqtt_at_context[4].is_used) {
        mqtt_at_message_handler(g_mqtt_at_context[4].mqtt_id, message);
    }
}

static messageHandler g_mqtt_message_handler[MQTT_AT_INSTANCE_NUM] =  {
     mqtt_at_message_handler_0,
     mqtt_at_message_handler_1,
     mqtt_at_message_handler_2,
     mqtt_at_message_handler_3,
     mqtt_at_message_handler_4};

static mqtt_at_context_t *mqtt_at_new_instance(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    uint32_t i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MQTT_AT_INSTANCE_NUM; i++) {
        if (g_mqtt_at_context[i].is_used == false) {
            memset(&g_mqtt_at_context[i], 0, sizeof(mqtt_at_context_t));
            g_mqtt_at_context[i].is_used = true;
            g_mqtt_at_context[i].mqtt_id = i;
            g_mqtt_at_context[i].message_handler = g_mqtt_message_handler[i];
            return &g_mqtt_at_context[i];
        }
    }

    return NULL;
}

static mqtt_at_context_t *mqtt_at_search_instance(uint32_t mqtt_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    uint32_t i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MQTT_AT_INSTANCE_NUM; i++) {
        if (g_mqtt_at_context[i].is_used == true && g_mqtt_at_context[i].mqtt_id == mqtt_id) {
            return &g_mqtt_at_context[i];
        }
    }

    return NULL;
}

static uint32_t mqtt_at_search_instance_id(Network *network)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    uint32_t i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MQTT_AT_INSTANCE_NUM; i++) {
        if (g_mqtt_at_context[i].is_used == true && g_mqtt_at_context[i].network == network) {
            return i;
        }
    }

    return MQTT_AT_INSTANCE_NUM;
}

static void mqtt_at_delete_instance(mqtt_at_context_t *mqtt)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (mqtt->is_used == true) {
        mqtt->is_used = false;
        if (mqtt->network != NULL) {
            vPortFree(mqtt->network);
            mqtt->network = NULL;
        }
        if (mqtt->client.buf != NULL) {
            vPortFree(mqtt->client.buf);
            mqtt->client.buf = NULL;
        }
        if (mqtt->client.readbuf != NULL) {
            vPortFree(mqtt->client.readbuf);
            mqtt->client.readbuf = NULL;
        }
    }
}

static int32_t mqtt_at_new(const char *server, const char *port, uint32_t command_timeout_ms, uint32_t bufsize, uint32_t cid, uint32_t *mqtt_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int32_t mqtt_error = MQTT_AT_ERRID_OK;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mqtt_at_context_t *mqtt = mqtt_at_new_instance();
    if (mqtt == NULL) {
        MQTT_AT_LOGE("max instance reached");
        mqtt_error = MQTT_AT_ERRID_NOT_SUPPORT;
    } else {
        mqtt->network = (Network *)pvPortMalloc(sizeof(Network));
        if (mqtt->network == NULL) {
            MQTT_AT_LOGE("memory error");
            mqtt_error = MQTT_AT_ERRID_MEMORY_ERROR;
            mqtt_at_delete_instance(mqtt);
        } else {
            // NewNetwork
            NewNetwork(mqtt->network);
            mqtt->network->on_disconnect_callback = mqtt_at_disconnect_handler;
            MQTT_AT_LOGI("ConnectNetwork start: 0x%x, %s, %s", (unsigned int)mqtt->network, server, port);
            // ConnectNetwork
            int mqtt_ret = ConnectNetwork(mqtt->network, (char *)server, (char *)port);
            MQTT_AT_LOGI("ConnectNetwork end: %d", mqtt_ret);
            if (mqtt_ret != 0) {
                mqtt_error = MQTT_AT_ERRID_SDK_ERROR;
                mqtt_at_delete_instance(mqtt);
            } else {
                unsigned char *send_buf = (unsigned char *)pvPortMalloc(bufsize);
                if (send_buf == NULL) {
                    MQTT_AT_LOGE("memory error");
                    mqtt_error = MQTT_AT_ERRID_MEMORY_ERROR;
                    mqtt->network->disconnect(mqtt->network);
                    mqtt_at_delete_instance(mqtt);
                } else {
                    unsigned char *read_buf = (unsigned char *)pvPortMalloc(bufsize);
                    if (read_buf == NULL) {
                        MQTT_AT_LOGE("memory error");
                        mqtt_error = MQTT_AT_ERRID_MEMORY_ERROR;
                        mqtt->network->disconnect(mqtt->network);
                        mqtt_at_delete_instance(mqtt);
                        vPortFree(send_buf);
                    } else {
                        MQTT_AT_LOGI("MQTTClient: 0x%x, 0x%x, %s, %s, %d, 0x%x, %d, 0x%x, %d",
                                     (unsigned int)&mqtt->client, (unsigned int)mqtt->network, server, port,
                                     (int)command_timeout_ms, (unsigned int)send_buf, (int)bufsize, (unsigned int)read_buf, (int)bufsize);
                        // MQTTClient
                        MQTTClient(&mqtt->client, mqtt->network, command_timeout_ms, send_buf, bufsize, read_buf, bufsize);
                        if (cid != -1) {
                            MQTT_AT_LOGI("setsockopt start: %d, %d", mqtt->network->my_socket, (int)cid);
                            // setsockopt
                            mqtt_ret = setsockopt(mqtt->network->my_socket, SOL_SOCKET, SO_BINDTODEVICE, &cid, sizeof(cid));
                            MQTT_AT_LOGI("setsockopt end: %d", mqtt_ret);
#if 0
                            if (mqtt_ret < 0) {
                                mqtt_error = MQTT_AT_ERRID_SDK_ERROR;
                                mqtt->network->disconnect(mqtt->network);
                                mqtt_at_delete_instance(mqtt);
                            } else {
#endif
                                *mqtt_id = mqtt->mqtt_id;
#if 0
                            }
#endif
                        } else {
                            *mqtt_id = mqtt->mqtt_id;
                        }
                    }
                }
            }
        }
    }

    return mqtt_error;
}

static int32_t mqtt_at_connect(uint32_t mqtt_id, uint32_t version, const char *client_id, uint32_t keep_alive, uint32_t clean_session, uint32_t will_flag, MQTTPacket_willOptions *will_options, const char*user_name, const char*password)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int32_t mqtt_error = MQTT_AT_ERRID_OK;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_mqtt_at_mutex_handle == NULL) {
        g_mqtt_at_mutex_handle = xSemaphoreCreateRecursiveMutex();
        configASSERT(g_mqtt_at_mutex_handle != NULL);
    }
	dbg_print("mqtt_at_connect:%d,%d,%s,%d,%d,%d,%s,%s",mqtt_id,version,client_id,keep_alive,clean_session,will_flag,user_name,password);

    xSemaphoreTakeRecursive(g_mqtt_at_mutex_handle, portMAX_DELAY);

    mqtt_at_context_t *mqtt = mqtt_at_search_instance(mqtt_id);
    if (mqtt == NULL) {
        MQTT_AT_LOGE("<ref> not found");
        mqtt_error = MQTT_AT_ERRID_NOT_FOUND;
    } else {
        MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
        connect_data.MQTTVersion = version;
        connect_data.clientID.cstring = (char *)client_id;
        connect_data.keepAliveInterval = keep_alive;
        connect_data.cleansession = clean_session;
        connect_data.willFlag = will_flag;
        if (will_flag == true) {
            memcpy(&connect_data.will, will_options, sizeof(MQTTPacket_willOptions));
        }
        connect_data.username.cstring = (char *)user_name;
        connect_data.password.cstring = (char *)password;
        MQTT_AT_LOGI("MQTTConnect start: 0x%x, 0x%x", (unsigned int)&mqtt->client, (unsigned int)&connect_data);
        int mqtt_ret = MQTTConnect(&mqtt->client, &connect_data);
		dbg_print("MQTTConnect ret:%d",mqtt_ret);
		m2m_mqtt_connect_rsp(mqtt_ret);
        MQTT_AT_LOGI("MQTTConnect end: %d", (int)mqtt_ret);
        if (mqtt_ret != 0) {
            mqtt_error = MQTT_AT_ERRID_SDK_ERROR;
        } else {
            g_mqtt_at_context[mqtt_id].is_connected = true;
        }
    }

    xSemaphoreGiveRecursive(g_mqtt_at_mutex_handle);

    return mqtt_error;
}

static int32_t mqtt_at_disconnect(uint32_t mqtt_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int32_t mqtt_error = MQTT_AT_ERRID_OK;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_mqtt_at_mutex_handle == NULL) {
        g_mqtt_at_mutex_handle = xSemaphoreCreateRecursiveMutex();
        configASSERT(g_mqtt_at_mutex_handle != NULL);
    }

    xSemaphoreTakeRecursive(g_mqtt_at_mutex_handle, portMAX_DELAY);

    mqtt_at_context_t *mqtt = mqtt_at_search_instance(mqtt_id);
    if (mqtt == NULL) {
        MQTT_AT_LOGE("<ref> not found");
        mqtt_error = MQTT_AT_ERRID_NOT_FOUND;
    } else {
        MQTT_AT_LOGI("MQTTDisconnect start: 0x%x", (unsigned int)&mqtt->client);
        int mqtt_ret = MQTTDisconnect(&mqtt->client);
        MQTT_AT_LOGI("MQTTDisconnect end: %d", (int)mqtt_ret);
#if 0
        if (mqtt_ret != 0) {
            mqtt_error = MQTT_AT_ERRID_SDK_ERROR;
        } else {
#endif
            mqtt->network->disconnect(mqtt->network);
            mqtt_at_delete_instance(mqtt);
            g_mqtt_at_context[mqtt_id].is_connected = false;
#if 0
        }
#endif
    }

    xSemaphoreGiveRecursive(g_mqtt_at_mutex_handle);

    return mqtt_error;
}

static void mqtt_at_disconnect_handler(Network *network)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response;
    char response_data[MQTT_AT_RESPONSE_DATA_LEN];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    uint32_t mqtt_id = mqtt_at_search_instance_id(network);
    if (mqtt_id == MQTT_AT_INSTANCE_NUM) {
        MQTT_AT_LOGE("mqtt_id not found");
    } else if (mqtt_at_disconnect(mqtt_id) == MQTT_AT_ERRID_OK) {
        response.result_code = APB_PROXY_RESULT_UNSOLICITED;
        // +EMQDISCON: <mqtt_id>
        sprintf(response_data, "+EMQDISCON: %d", (int)mqtt_id);
        response.pdata = response_data;
        response.length = strlen(response.pdata);

        response.cmd_id = APB_PROXY_INVALID_CMD_ID;

		m2m_mqtt_disconect_msg_pro(mqtt_id);
        apb_proxy_send_at_cmd_result(&response);
    }
}

static int32_t mqtt_at_subscribe(uint32_t mqtt_id, const char *topic, uint32_t QoS)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int32_t mqtt_error = MQTT_AT_ERRID_OK;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mqtt_at_context_t *mqtt = mqtt_at_search_instance(mqtt_id);
    if (mqtt == NULL) {
        MQTT_AT_LOGE("<ref> not found");
        mqtt_error = MQTT_AT_ERRID_NOT_FOUND;
    } else {
        MQTT_AT_LOGI("MQTTSubscribe start: 0x%x, %s, %d, 0x%x", (unsigned int)&mqtt->client, topic, (int)QoS, (unsigned int)mqtt_at_message_handler);
        int mqtt_ret = MQTTSubscribe(&mqtt->client, topic, QoS, mqtt->message_handler);
        MQTT_AT_LOGI("MQTTSubscribe end: %d", (int)mqtt_ret);
        if (mqtt_ret < 0) {
            mqtt_error = MQTT_AT_ERRID_SDK_ERROR;
        }
    }

    return mqtt_error;
}

static int32_t mqtt_at_unsubscribe(uint32_t mqtt_id, const char *topic)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int32_t mqtt_error = MQTT_AT_ERRID_OK;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mqtt_at_context_t *mqtt = mqtt_at_search_instance(mqtt_id);
    if (mqtt == NULL) {
        MQTT_AT_LOGE("<ref> not found");
        mqtt_error = MQTT_AT_ERRID_NOT_FOUND;
    } else {
        MQTT_AT_LOGI("MQTTUnsubscribe start: 0x%x, %s", (unsigned int)&mqtt->client, topic);
        int mqtt_ret = MQTTUnsubscribe(&mqtt->client, topic);
        MQTT_AT_LOGI("MQTTUnsubscribe end: %d", (int)mqtt_ret);
        if (mqtt_ret != 0) {
            mqtt_error = MQTT_AT_ERRID_SDK_ERROR;
        }
    }

    return mqtt_error;
}

static int32_t mqtt_at_publish(uint32_t mqtt_id, const char *topic, uint32_t QoS, uint32_t retained, uint32_t dup, uint32_t message_len, const char *message)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int32_t mqtt_error = MQTT_AT_ERRID_OK;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mqtt_at_context_t *mqtt = mqtt_at_search_instance(mqtt_id);
    if (mqtt == NULL) {
        dbg_print("<ref> not found");
        mqtt_error = MQTT_AT_ERRID_NOT_FOUND;
    } else if (message_len % 2 != 0) {
        dbg_print("length of <message_len> is odd");
        mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
    } else {
        uint8_t *payload_bin;
        uint32_t payload_len;
        payload_bin = (uint8_t *)pvPortMalloc(message_len / 2 + 1);
        if (payload_bin == NULL) {
            dbg_print("memory error");
            mqtt_error = MQTT_AT_ERRID_MEMORY_ERROR;
        } else {
            MQTTMessage publish_data;
            payload_len = onenet_at_hex_to_bin(payload_bin, message, message_len / 2);
            publish_data.qos = QoS;
            publish_data.retained = retained;
            publish_data.dup = dup;
            publish_data.payload = payload_bin;
            publish_data.payloadlen = payload_len;
            dbg_print("MQTTPublish start: 0x%x, %s, 0x%x", (unsigned int)&mqtt->client, topic, (unsigned int)&publish_data);
            int mqtt_ret = MQTTPublish(&mqtt->client, topic, &publish_data);
            dbg_print("MQTTPublish end: %d", (int)mqtt_ret);
            if (mqtt_ret != 0) {
                mqtt_error = MQTT_AT_ERRID_SDK_ERROR;
            }
        }

        if (payload_bin != NULL) {
            vPortFree(payload_bin);
        }
    }

    return mqtt_error;
}

static void mqtt_at_task_processing(void *arg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    uint32_t i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_mqtt_at_mutex_handle == NULL) {
        g_mqtt_at_mutex_handle = xSemaphoreCreateRecursiveMutex();
        configASSERT(g_mqtt_at_mutex_handle != NULL);
    }

    while (1) {
        for (i = 0; i < MQTT_AT_INSTANCE_NUM; i++) {
            xSemaphoreTakeRecursive(g_mqtt_at_mutex_handle, portMAX_DELAY);
            if (g_mqtt_at_context[i].is_connected) {
                MQTTYield(&g_mqtt_at_context[i].client, 1000);
            }
            xSemaphoreGiveRecursive(g_mqtt_at_mutex_handle);
        }

        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

apb_proxy_status_t apb_proxy_hdlr_mqtt_new_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char *param_buffer;
    int32_t mqtt_error;
    uint32_t mqtt_id;
    char response_data[MQTT_AT_RESPONSE_DATA_LEN];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MQTT_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char *)pvPortMalloc(parse_cmd->string_len);
            if (param_buffer == NULL) {
                MQTT_AT_LOGE("memory error");
                mqtt_error = MQTT_AT_ERRID_MEMORY_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    MQTT_AT_LOGE("no parameter");
                    mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
                } else {
                    char *param_list[MQTT_AT_CMD_PARAM_NUM];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, MQTT_AT_CMD_PARAM_NUM);

                    // AT+EMQNEW=<server>, <port>, <command_timeout_ms>, <bufsize>[, <cid>]
                    if (param_num < 4) {
                        MQTT_AT_LOGE("parameter too short");
                        mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
                    } else {
                        char *server = param_list[0];
                        char *port = param_list[1];
                        uint32_t command_timeout_ms = atoi(param_list[2]);
                        uint32_t bufsize = atoi(param_list[3]);
                        uint32_t cid = -1;
                        if (param_num >= 5) {
                            cid = atoi(param_list[4]);
                        }
                        mqtt_error = mqtt_at_new(server, port, command_timeout_ms, bufsize, cid, &mqtt_id);
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }

            if (mqtt_error == MQTT_AT_ERRID_OK) {
                response.result_code = APB_PROXY_RESULT_OK;
                if (g_mqtt_at_task_running == false) {
                    g_mqtt_at_task_running = true;
                    xTaskCreate(mqtt_at_task_processing,
                                 MQTT_AT_TASK_NAME,
                                 MQTT_AT_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                                 NULL,
                                 MQTT_AT_TASK_PRIORITY,
                                 NULL);
                }
            } else {
                response.result_code = APB_PROXY_RESULT_ERROR;
            }
            break;

        default:
            MQTT_AT_LOGE("not support");
            mqtt_error = MQTT_AT_ERRID_NOT_SUPPORT;
            response.result_code = APB_PROXY_RESULT_ERROR;
            break;
    }

    if (mqtt_error == MQTT_AT_ERRID_OK) {
        sprintf(response_data, "+EMQNEW: %d", (int)mqtt_id);
        response.pdata = response_data;
        response.length = strlen(response.pdata);
		g_mqtt_id = mqtt_id;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_mqtt_connect_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char *param_buffer, *field_buffer;
    int32_t mqtt_error;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MQTT_AT_LOGI("%s", parse_cmd->string_ptr);
	//dbg_print("apb_proxy_hdlr_mqtt_connect_cmd:%d",parse_cmd->mode);
    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char *)pvPortMalloc(parse_cmd->string_len);
            if (param_buffer == NULL)
			{
                MQTT_AT_LOGE("memory error");
                mqtt_error = MQTT_AT_ERRID_MEMORY_ERROR;
            } 
			else 
			{
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
				//dbg_print("-----------xxxxxxxxxxxxxxxxxxxxxxxxx");
                if (!cmd_string || *(cmd_string + 1) == '\0') 
				{
                    MQTT_AT_LOGE("no parameter");
                    mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
                } 
				else 
				{
                    char *param_list[MQTT_AT_CMD_PARAM_NUM];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, MQTT_AT_CMD_PARAM_NUM);

					//dbg_print("\r\nconnect_cmd parse:%d,%s,%s,%s,%s,%s,%s,%s,%s,%s",param_num,param_list[0],param_list[1],param_list[2],param_list[3],param_list[4],param_list[5],param_list[6],param_list[7],param_list[8]);
					//dbg_print("-----------yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy");
                    // AT+EMQCON=<mqtt_id>, <version>, <client_id>, <keep_alive>, <clean_session>, <will_flag>[, <will_options>][, <user_name>, <password>]
                    if (param_num < 6) 
					{
						dbg_print("-----------MQTT_AT_ERRID_PARAMETER_ERROR");
                        MQTT_AT_LOGE("parameter too short");
                        mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
                    } 
					else
					{
                        uint32_t mqtt_id = atoi(param_list[0]);
                        uint32_t version = atoi(param_list[1]);
                        char *client_id = param_list[2];
                        uint32_t keep_alive = atoi(param_list[3]);
                        uint32_t clean_session = atoi(param_list[4]);
                        uint32_t will_flag = atoi(param_list[5]);
                        MQTTPacket_willOptions will_options = MQTTPacket_willOptions_initializer;
                        char *user_name = param_list[6];
                        char *password = param_list[7];
						
						//dbg_print("-----------zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
                        if (atoi(param_list[3]) < 0) 
						{
                            MQTT_AT_LOGE("<keep_alive> < 0");
                            mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
                        }
						else if (will_flag == true) 
						{
                            if (param_num < 7) 
							{
                                MQTT_AT_LOGE("parameter too short");
                                mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
                            } 
							else
							{
                                char *field_list[MQTT_AT_CMD_FILED_NUM];
                                const char *filed_key[] = {"topic", "QoS", "retained", "message_len", "message"};
                                field_buffer = (char *)pvPortMalloc(strlen(param_list[6]) + 1);
                                if (field_buffer == NULL) 
								{
                                    MQTT_AT_LOGE("memory error");
                                    mqtt_error = MQTT_AT_ERRID_MEMORY_ERROR;
                                } 
								else 
								{
                                    uint32_t filed_num = onenet_at_parse_param(param_list[6], field_buffer, filed_key, field_list, MQTT_AT_CMD_FILED_NUM, ",");
                                    // <topic=xxx,QoS=xxx,retained=xxx,message_len=xxx,message=xxx>
                                    if (filed_num < 5) 
									{
                                        MQTT_AT_LOGE("parameter too short");
                                        mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
										dbg_print("-----------MQTT_AT_ERRID_PARAMETER_ERROR");
                                    } 
									else 
									{
                                        char *topic = field_list[0];
                                        uint32_t QoS = atoi(field_list[1]);
                                        uint32_t retained = atoi(field_list[2]);
                                        uint32_t message_len = atoi(field_list[3]);
                                        char *message = field_list[4];

                                        will_options.topicName.lenstring.len = strlen(topic);
                                        will_options.topicName.lenstring.data = topic;
                                        will_options.qos = QoS;
                                        will_options.retained = retained;
                                        if (message_len % 2 != 0) 
										{
                                            MQTT_AT_LOGE("length of <message_len> is odd");
                                            mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
											dbg_print("-----------MQTT_AT_ERRID_PARAMETER_ERROR");
                                        } 
										else 
										{
                                            uint8_t *payload_bin;
                                            uint32_t payload_len;
                                            payload_bin = (uint8_t *)pvPortMalloc(message_len / 2 + 1);
                                            if (payload_bin == NULL) 
											{
                                                MQTT_AT_LOGE("memory error");
                                                mqtt_error = MQTT_AT_ERRID_MEMORY_ERROR;
												dbg_print("-----------MQTT_AT_ERRID_MEMORY_ERROR");
                                            } 
											else 
											{
                                                payload_len = onenet_at_hex_to_bin(payload_bin, message, message_len / 2);
                                                will_options.message.lenstring.len = (int)payload_len;
                                                will_options.message.lenstring.data = (char *)payload_bin;
                                                if (param_num >= 9) 
												{
                                                    user_name = param_list[7];
                                                    password = param_list[8];
                                                }
                                                MQTT_AT_LOGE("client_id=%s", client_id);
												dbg_print("-----------mqtt_at_connect start 1");
                                                mqtt_error = mqtt_at_connect(mqtt_id, version, client_id, keep_alive, clean_session, will_flag, &will_options, user_name, password);
												m2m_mqtt_connect_rsp(mqtt_error);
                                            }
                                            if (payload_bin != NULL) 
											{
                                                vPortFree(payload_bin);
                                            }
                                        }
                                    }
                                    vPortFree(field_buffer);
                                }
                            }
                        } else {
                        	dbg_print("%s,%s,%s,%s,%s,%s",param_list[0],param_list[1],param_list[2],param_list[3],param_list[4],param_list[5]);
                            if (param_num >= 8) {
                                user_name = param_list[6];
                                password = param_list[7];
								
								dbg_print("username:%s,%s",param_list[6],param_list[7]);
                            }
							dbg_print("-----------mqtt_at_connect start 2");
                            mqtt_error = mqtt_at_connect(mqtt_id, version, client_id, keep_alive, clean_session, will_flag, &will_options, user_name, password);
							m2m_mqtt_connect_rsp(mqtt_error);
                        }
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }

            if (mqtt_error == MQTT_AT_ERRID_OK) {
                response.result_code = APB_PROXY_RESULT_OK;
            } else {
                response.result_code = APB_PROXY_RESULT_ERROR;
            }
            break;

        default:
            MQTT_AT_LOGE("not support");
            mqtt_error = MQTT_AT_ERRID_NOT_SUPPORT;
            response.result_code = APB_PROXY_RESULT_ERROR;
            break;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_mqtt_disconnect_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char *param_buffer;
    int32_t mqtt_error;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MQTT_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char *)pvPortMalloc(parse_cmd->string_len);
            if (param_buffer == NULL) {
                MQTT_AT_LOGE("memory error");
                mqtt_error = MQTT_AT_ERRID_MEMORY_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    MQTT_AT_LOGE("no parameter");
                    mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
                } else {
                    char *param_list[MQTT_AT_CMD_PARAM_NUM];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, MQTT_AT_CMD_PARAM_NUM);

                    // AT+EMQDISCON=<mqtt_id>
                    if (param_num < 1) {
                        MQTT_AT_LOGE("parameter too short");
                        mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
                    } else {
                        uint32_t mqtt_id = atoi(param_list[0]);
                        mqtt_error = mqtt_at_disconnect(mqtt_id);
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }

            if (mqtt_error == MQTT_AT_ERRID_OK) {
                response.result_code = APB_PROXY_RESULT_OK;
            } else {
                response.result_code = APB_PROXY_RESULT_ERROR;
            }
            break;

        default:
            MQTT_AT_LOGE("not support");
            mqtt_error = MQTT_AT_ERRID_NOT_SUPPORT;
            response.result_code = APB_PROXY_RESULT_ERROR;
            break;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_mqtt_subscribe_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char *param_buffer;
    int32_t mqtt_error;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MQTT_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char *)pvPortMalloc(parse_cmd->string_len);
            if (param_buffer == NULL) {
                MQTT_AT_LOGE("memory error");
                mqtt_error = MQTT_AT_ERRID_MEMORY_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    MQTT_AT_LOGE("no parameter");
                    mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
                } else {
                    char *param_list[MQTT_AT_CMD_PARAM_NUM];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, MQTT_AT_CMD_PARAM_NUM);

                    // AT+EMQSUB=<mqtt_id>, <topic>, <QoS>
                    if (param_num < 3) {
                        MQTT_AT_LOGE("parameter too short");
                        mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
                    } else {
                        uint32_t mqtt_id = atoi(param_list[0]);
                        char *topic = param_list[1];
                        uint32_t QoS = atoi(param_list[2]);
                        mqtt_error = mqtt_at_subscribe(mqtt_id, topic, QoS);
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }

            if (mqtt_error == MQTT_AT_ERRID_OK) {
                response.result_code = APB_PROXY_RESULT_OK;
            } else {
                response.result_code = APB_PROXY_RESULT_ERROR;
            }
            break;

        default:
            MQTT_AT_LOGE("not support");
            mqtt_error = MQTT_AT_ERRID_NOT_SUPPORT;
            response.result_code = APB_PROXY_RESULT_ERROR;
            break;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_mqtt_unsubscribe_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char *param_buffer;
    int32_t mqtt_error;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MQTT_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char *)pvPortMalloc(parse_cmd->string_len);
            if (param_buffer == NULL) {
                MQTT_AT_LOGE("memory error");
                mqtt_error = MQTT_AT_ERRID_MEMORY_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    MQTT_AT_LOGE("no parameter");
                    mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
                } else {
                    char *param_list[MQTT_AT_CMD_PARAM_NUM];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, MQTT_AT_CMD_PARAM_NUM);

                    // AT+EMQUNSUB=<mqtt_id>, <topic>
                    if (param_num < 2) {
                        MQTT_AT_LOGE("parameter too short");
                        mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
                    } else {
                        uint32_t mqtt_id = atoi(param_list[0]);
                        char *topic = param_list[1];
                        mqtt_error = mqtt_at_unsubscribe(mqtt_id, topic);
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }

            if (mqtt_error == MQTT_AT_ERRID_OK) {
                response.result_code = APB_PROXY_RESULT_OK;
            } else {
                response.result_code = APB_PROXY_RESULT_ERROR;
            }
            break;

        default:
            MQTT_AT_LOGE("not support");
            mqtt_error = MQTT_AT_ERRID_NOT_SUPPORT;
            response.result_code = APB_PROXY_RESULT_ERROR;
            break;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_mqtt_publish_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char *param_buffer;
    int32_t mqtt_error;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MQTT_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char *)pvPortMalloc(parse_cmd->string_len);
            if (param_buffer == NULL) {
                MQTT_AT_LOGE("memory error");
                mqtt_error = MQTT_AT_ERRID_MEMORY_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    MQTT_AT_LOGE("no parameter");
                    mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
                } else {
                    char *param_list[MQTT_AT_CMD_PARAM_NUM];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, MQTT_AT_CMD_PARAM_NUM);

                    // AT+EMQPUB=<mqtt_id>, <topic>, <QoS>, <retained>, <dup>, <message_len>, <message>
                    if (param_num < 7) {
                        MQTT_AT_LOGE("parameter too short");
                        mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
                    } else {
                        uint32_t mqtt_id = atoi(param_list[0]);
                        char *topic = param_list[1];
                        uint32_t QoS = atoi(param_list[2]);
                        uint32_t retained = atoi(param_list[3]);
                        uint32_t dup = atoi(param_list[4]);
                        uint32_t message_len = atoi(param_list[5]);
                        char *message = param_list[6];
                        mqtt_error = mqtt_at_publish(mqtt_id, topic, QoS, retained, dup, message_len, message);
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }

            if (mqtt_error == MQTT_AT_ERRID_OK) {
                response.result_code = APB_PROXY_RESULT_OK;
            } else {
                response.result_code = APB_PROXY_RESULT_ERROR;
            }
            break;

        default:
            MQTT_AT_LOGE("not support");
            mqtt_error = MQTT_AT_ERRID_NOT_SUPPORT;
            response.result_code = APB_PROXY_RESULT_ERROR;
            break;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

