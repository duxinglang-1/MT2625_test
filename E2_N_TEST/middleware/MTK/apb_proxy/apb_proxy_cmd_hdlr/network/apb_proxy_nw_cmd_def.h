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


#ifndef __APB_PROXY_NW_CMD_DEF__H__
#define __APB_PROXY_NW_CMD_DEF__H__

/**
 * format: AT+EGACT=<cmd head>,<mode>,<param1>,<param2>,<para3>,...
 * mode:  0 -execute mode; 1 - active mode; 2 - read mode; 3 - test mode
 * notice: string param should be wrapped by "".
 *            if want to set a omitted param, please pass oint,  ostr.
 *            oint means omitted int,  ostr means omitted string
 * example: AT+RILUTS=+CMGS,0,14,"hello, world!"
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EGACT",          apb_proxy_hdlr_egact_cmd)

/**
 * format: AT+IPERF=<cmd head>,<mode>,<param1>,<param2>,<para3>,...
 * mode:  0 -execute mode; 1 - active mode; 2 - read mode; 3 - test mode
 * notice: string param should be wrapped by "".
 *            if want to set a omitted param, please pass oint,  ostr.
 *            oint means omitted int,  ostr means omitted string
 * example: AT+RILUTS=+CMGS,0,14,"hello, world!"
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+IPERF",          apb_proxy_hdlr_iperf_cmd)

/**
 * format: AT+IPERF=<cmd head>,<mode>,<param1>,<param2>,<para3>,...
 * mode:  0 -execute mode; 1 - active mode; 2 - read mode; 3 - test mode
 * notice: string param should be wrapped by "".
 *            if want to set a omitted param, please pass oint,  ostr.
 *            oint means omitted int,  ostr means omitted string
 * example: AT+RILUTS=+CMGS,0,14,"hello, world!"
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+PING",          apb_proxy_hdlr_ping_cmd)


/**
 * format: AT+EUPSM=<cmd head>,<mode>,<param1>,<param2>,<para3>,...
 * mode:  0 -execute mode; 1 - active mode; 2 - read mode; 3 - test mode
 * notice: string param should be wrapped by "".
 *            if want to set a omitted param, please pass oint,  ostr.
 *            oint means omitted int,  ostr means omitted string
 * example: AT+RILUTS=+CMGS,0,14,"hello, world!"
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EUPSM",          apb_proxy_hdlr_upsm_cmd)

/**
 * format: AT+TCMTEST=<test_cause_number>
 * test_cause_number:  1-7. There are 7 test cases for telphony connection manager
 * example: AT+TCMTEST=1
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+TCMTEST",          apb_proxy_hdlr_tcmtest_cmd)

/**
 * format: AT+NIDD=<test_cause_number>
 * test_cause_number:  1-2. There are 7 test cases for telphony connection manager
 * example: AT+NIDD=1, "APN"              create NIDD
 * example: AT+NIDD=2, "APN","send data"              send data via NIDD
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+NIDD",          apb_proxy_hdlr_niddtest_cmd)

/**
 * format: AT+ESOC=<domain>,<type>,<protocol>[,<CID>]
 * domain:  1-2.         1-IPv4    2-IPv6
 * type:      1-3.         1-TCP     2-UDP     3-RAW
 * protocol: 1-2.         1-IP       2-ICMP    
 * CID:       interger    PDP context id              [option]
 * example: AT+ESOC=1,1,1             create IPv4 TCP socket
 * example: AT+ESOC=1,2,1             create IPv4 UDP socket
 * Response:  socket_id\r\nOK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ESOC",          apb_proxy_hdlr_socket_cmd)

/**
 * format: AT+ESOB=<socket_id>,<family>,<local_port>[,<local_address>]
 * socket_id:         socket_id, AT+ESOC response
 * local_port:         interger.   port    
 * local_address:    ip address            [option]
 * example: AT+ESOB=1,1,1024             bind to local port: 1024
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ESOB",          apb_proxy_hdlr_bind_cmd)

/**
 * format: AT+ESOSTA=<socket_id>,<remote_port>,<remote_address>
 * socket_id:             socket_id, AT+ESOC response
 * remote_port:         interger.   port    
 * remote_address:    ip address            [option]
 * example: AT+ESOB=1,1024,172.26.109.79     
 * Response:  OK\r\n     ERROR\r\n        
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ESOCON",          apb_proxy_hdlr_connect_cmd)    
    

/**
 * format: AT+ESOST=<socket_id>,<data_len>,<data>
 * socket_id:             socket_id, AT+ESOC response
 * data_len:              interger.   len    
 * data:                    data
 * example: AT+ESOST=1,10,ababababab
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ESOSEND",          apb_proxy_hdlr_send_cmd)

/**
 * format: AT+ESOCL=<socket_id>
 * socket_id:             socket_id, AT+ESOC response
 * example: AT+ESOCL=1
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ESOCL",          apb_proxy_hdlr_close_cmd)

/**
 * format: AT+ESODATAMODE=<socket_id>,<data_mode>
 * socket_id:             socket_id, AT+ESOC response
 * data_mode:           create or close data mode, integer, 1 - create, 0 - close
 * example: AT+ESODATAMODE=1, 1
 * Response:  CONNECT\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ESODATAMODE",          apb_proxy_hdlr_datamode_cmd)

/**
 * format: AT+EDNS=<host>
 * host:             host name
 * example: AT+EDNS=baidu.com
 * Response:  ip_address\r\nOK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EDNS",          apb_proxy_hdlr_dns_cmd)

AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+IPCONFIG",      apb_proxy_hdlr_ipconfig_cmd)

/**
 * format: AT+ELMCONF=<size>,<config>
 * size:             config file size
 * config:          config file
 * example: AT+ELMCONF=100,xxxxxx
 * Response:  lwm2m_id\r\nOK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ELMCONF",          apb_proxy_hdlr_lwm2m_config_cmd)

/**
 * format: AT+ELMDEL=<ID>
 * ID:             lwm2m id
 * example: AT+ELMDEL=0
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ELMDEL",          apb_proxy_hdlr_lwm2m_delete_cmd)

/**
 * format: AT+ELMADDOBJ=<ID>,<object_id>,<instance_id>,<resource_count>
 * ID:                    lwm2m id
 * object_id:           object id
 * instance_id:        instance id
 * resource_count:   resource count
 * example: AT+ELMADDOBJ=0,10,1,4
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ELMADDOBJ",          apb_proxy_hdlr_lwm2m_addobj_cmd)

/**
 * format: AT+ELMDELOBJ=<ID>,<object_id>
 * ID:                    lwm2m id
 * object_id:           object id
 * example: AT+ELMDELOBJ=0,10
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ELMDELOBJ",          apb_proxy_hdlr_lwm2m_delobj_cmd)
    
/**
 * format: AT+ELMREAD=<ID>,<msg_id>,<object_id>,<instance_id>,<resource_id>,<valuetype>,<value_len>,<value>,<flag>
 * ID:                    lwm2m id
 * msg_id:             +ELMREAD msg_id
 * object_id:           object id
 * instance_id:        instance id
 * resource_count:   resource id
 * value_type:        value type   string = 1, opaque = 2, interger = 3, float = 4, bool = 5
 * value_len:          value len
 * value:                object value
 * flag:                  1-the last message, 0-there are more message
 * example: AT+ELMREAD=0,1,10,1,1,3,2,12,1
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ELMREAD",          apb_proxy_hdlr_lwm2m_read_cmd)
   
/**
 * format: AT+ELMWRITE=<ID>,<msg_id>,<object_id>,<result>
 * ID:                    lwm2m id
 * msg_id:             +ELMWRITE msg_id
 * object_id:           object id
 * result:                result of write command, 1-error, 0-success
 * example: AT+ELMWRITE=0,2,10,0
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ELMWRITE",          apb_proxy_hdlr_lwm2m_write_cmd)
   
/**
 * format: AT+ELMEXECUTE=<ID>,<msg_id>,<object_id>,<result>
 * ID:                    lwm2m id
 * msg_id:             +ELMEXECUTE msg_id
 * object_id:           object id
 * result:                result of execute command, 1-error, 0-success
 * example: AT+ELMEXECUTE=0,2,10,0
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ELMEXECUTE",          apb_proxy_hdlr_lwm2m_execute_cmd)

/**
 * format: AT+ELMNOTIFY=<ID>,<msg_id>,<object_id>,<instance_id>,<resource_id>,<valuetype>,<value_len>,<value>,<flag>
 * ID:                    lwm2m id
 * msg_id:             +ELMOBSERVE msg_id
 * object_id:           object id
 * instance_id:        instance id
 * resource_count:   resource id
 * value_type:        value type   string = 1, opaque = 2, interger = 3, float = 4, bool = 5
 * value_len:          value len
 * value:                object value
 * flag:                  1-the last message, 0-there are more message
 * example: AT+ELMNOTIFY=0,5,10,1,1,2,2,32,1
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ELMNOTIFY",          apb_proxy_hdlr_lwm2m_notify_cmd)

/**
 * format: AT+ELMINSTANCE?
 * Response:  lwm2m_id\r\nlwm2m_id\r\n...OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ELMINSTANCE",          apb_proxy_hdlr_lwm2m_query_cmd)

/**
 * format: AT+EMQNEW=<server>,<port>,<command_timeout_ms>,<buf_size>[,<CID>]
 * server: MQTT server
 * port: MQTT port
 * command_timeout_ms:  MQTT command timeout (ms)
 * buf_size: MQTT send and read buffer size
 * CID:       interger    PDP context id              [option]
 * Response:  +EMQNEW:mqtt_id\r\nOK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EMQNEW",          apb_proxy_hdlr_mqtt_new_cmd)

/**
 * format: AT+EMQCON=<mqtt_id>,<version>,<will_flag>,<keepalive_internal>,<cleansession>,<client_id>[,<username>,<password>]
 * mqtt_id: AT+EMQNEW response
 * version: MQTT version
 * will_flag:  MQTT connection packet's will flag set
 * keepalive_internal: MQTT connection packet's keepalive internal set
 * cleansession:       MQTT connection packet's clean session set
 * client_id: MQTT connection packet's client_id set
 * username:MQTT connection packet's username set
 * password: MQTT connection packet's password set
 * Response:  OK\r\n     ERROR\r\n
 *                 OK-receive CONACK
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EMQCON",          apb_proxy_hdlr_mqtt_connect_cmd)

/**
 * format: AT+EMQDISCON=<mqtt_id>
 * mqtt_id: AT+EMQNEW response
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EMQDISCON",          apb_proxy_hdlr_mqtt_disconnect_cmd)

/**
 * format: AT+EMQSUB=<mqtt_id>,<QoS>,<topic>
 * mqtt_id: AT+EMQNEW response
 * QoS: MQTT subscribe's Qos
 * topic: MQTT subscribe's topic
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EMQSUB",          apb_proxy_hdlr_mqtt_subscribe_cmd)

/**
 * format: AT+EMQUNSUB=<mqtt_id>,<topic>
 * mqtt_id: AT+EMQNEW response
 * topic: MQTT unsubscribe's topic
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EMQUNSUB",          apb_proxy_hdlr_mqtt_unsubscribe_cmd)

/**
 * format: AT+EMQSUB=<mqtt_id>,<QoS>,<topic>,<message_len>,<message>
 * mqtt_id: AT+EMQNEW response
 * QoS: MQTT pubilsh's Qos
 * topic: MQTT pubilsh's topic
 * message_len: MQTT pulish mesage's length
 * message: MQTT pulish mesage
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EMQPUB",          apb_proxy_hdlr_mqtt_publish_cmd)


/**
 * format: AT+EHTTPCON=<url>
 * url: http server url
 * cert: for https [option]
 * key: for https [option]
 * Response:  +EHTTPCON=id\r\nOK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EHTTPCON",          apb_proxy_hdlr_httpclient_connect_cmd)

/**
 * format: AT+EHTTPCON=<multi_pack_config>,<host>,[<auth_user>,<auth_password>,<server_cert_len>,<server_cert>,<client_cert_len>,<client_cert>,<client_pk_len>,<client_pk>]
 * multi_pack_config: Information to support multi packages for a long size command, it include 3 parameters: <flag>,<total_len>,<len>
 *      <flag>: 1 means there are more packages, 0 means this package is the last one
 *      <total_len>: The total length of the command, it does not include <multi_pack_config>
 *      <len>: The length of current package, it does not include <multi_pack_config>
 * host: http server host
 * auth_user: authorization name [option]
 * auth_password: authorization password [option]
 * server_cert_len: Server certification length, for https [option]
 * server_cert: Server certification, for https [option]
 * client_cert_len: Client certification length, for https [option]
 * client_cert: Client certification, for https [option]
 * client_pk_len: Client private key length, for https [option]
 * client_pk: Client private key, for https [option]
 * Response:  +EHTTPCON=id\r\nOK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EHTTPCREATE",       apb_proxy_hdlr_httpclient_create_cmd)

/**
 * format: AT+EHTTPSEND==<httpclient_id>,<method>,<path_len>,<path>,<customer_header_len>,<customer_header>.<content_type_len>,<content_type_len>,<content_string_len>,<content_string>
 * id: http client id
 * method: http method, GET = 0, POST = 1, PUT = 2, DELETE = 3,
 * path_len : length of path
 * path : The resource path on server, ex. "/html/login/index.html"
 * customer_header_len: Length of customer_header
 * customer_header :  The customer defined information in content, but don’t include host, path, content type, content length.
 * content_type_len : The length of Content_type
 * content_type: A string indicate the content type of the content, if the method is not POST and PUT, it must be empty
 * content_string_len: Must be the string length of content_string, equals hex data size * 2
 * content_string: The string converted from content hex data, the string length must equal hex data size * 2
 * Response: OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EHTTPSEND",          apb_proxy_hdlr_httpclient_send_cmd)


/**
 * format: AT+EHTTPDISCON=<id>
 * id: http client id
 * Response: OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EHTTPDISCON",          apb_proxy_hdlr_httpclient_disconnect_cmd)

/**
 * format: AT+EHTTPDESTROY=<id>
 * id: http client id
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EHTTPDESTROY",          apb_proxy_hdlr_httpclient_destroy_cmd)

/**
 * format: AT+ESNTPSTART=<server>
 * server: sntp server
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ESNTPSTART",          apb_proxy_hdlr_sntp_start_cmd)

/**
 * format: AT+ESNTPSTOP
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ESNTPSTOP",          apb_proxy_hdlr_sntp_stop_cmd)

/**
 * format: AT+MIPLCREATE=<totalsize>, <config>, <index>, <currentsize>, <flag>
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+MIPLCREATE",          apb_proxy_hdlr_onenet_create_cmd)

/**
 * format: AT+MIPLDELETE=<ref>
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+MIPLDELETE",          apb_proxy_hdlr_onenet_delete_cmd)

/**
 * format: AT+MIPLOPEN=<ref>, <lifetime>
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+MIPLOPEN",          apb_proxy_hdlr_onenet_open_cmd)

/**
 * format: AT+MIPLCLOSE=<ref>
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+MIPLCLOSE",          apb_proxy_hdlr_onenet_close_cmd)

/**
 * format: // AT+MIPLADDOBJ=<ref>, <objectid>, <instancecount>, <instancebitmap>, <attributecount>, <actioncount>
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+MIPLADDOBJ",          apb_proxy_hdlr_onenet_add_object_cmd)

/**
 * format: AT+MIPLDELOBJ=<ref>, <objectid>
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+MIPLDELOBJ",          apb_proxy_hdlr_onenet_delete_object_cmd)

/**
 * format: AT+MIPLUPDATE=<ref>, <lifetime>, <withObjectFlag>
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+MIPLUPDATE",          apb_proxy_hdlr_onenet_update_cmd)

/**
 * format: AT+MIPLNOTIFY=<ref>, <msgid>, <objectid>, <instanceid>, <resourceid>, <valuetype>, <len>, <value>, <index>, <flag>[, <ackid>]
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+MIPLNOTIFY",          apb_proxy_hdlr_onenet_notify_cmd)

/**
 * format: AT+MIPLREADRSP=<ref>, <msgid>, <result>, <objectid>, <instanceid>, <resourceid>, <valuetype>, <len>, <value>, <index>, <flag>
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+MIPLREADRSP",          apb_proxy_hdlr_onenet_read_response_cmd)

/**
 * format: AT+MIPLWRITERSP=<ref>, <msgid>, <result>
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+MIPLWRITERSP",          apb_proxy_hdlr_onenet_write_response_cmd)

/**
 * format: AT+MIPLEXECUTERSP=<ref>, <msgid>, <result>
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+MIPLEXECUTERSP",          apb_proxy_hdlr_onenet_execute_response_cmd)

/**
 * format: AT+MIPLOBSERVERSP=<ref>, <msgid>, <result>
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+MIPLOBSERVERSP",          apb_proxy_hdlr_onenet_observe_response_cmd)

/**
 * format: AT+MIPLDISCOVERRSP=<ref>, <msgid>, <result>, <length>, <valuestring>
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+MIPLDISCOVERRSP",          apb_proxy_hdlr_onenet_discover_response_cmd)

/**
 * format: AT+MIPLPARAMETERRSP=<ref>, <msgid>, <result>
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+MIPLPARAMETERRSP",          apb_proxy_hdlr_onenet_parameter_response_cmd)

/**
 * format: AT+MIPLVER?
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+MIPLVER",          apb_proxy_hdlr_onenet_version_cmd)

#ifdef MTK_ONENET_SUPPORT
/**
 * format: AT+DMCONFIG=<config>,<appkey>,<pwd>,<lifetime>[,<disable>]
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+DMCONFIG",              apb_proxy_hdlr_dm_config_cmd)
#endif

#ifdef MTK_TMO_CERT_SUPPORT
/**
 * format: AT+TMOCONFIG=<Bootstrap_Enabled>,<Sever_IP>,<Port>,<Endpoint_Name>,<lifetime>,<Security_Mode>[,<PSKID>,<PSK>]
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+TMOCONFIG",              apb_proxy_hdlr_tmo_config_cmd)

/**
 * format: AT+TMOREG
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+TMOREG",              apb_proxy_hdlr_tmo_register_cmd)

/**
 * format: AT+TMOUPDATE=<mode>,<lifetime or binding>
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+TMOUPDATE",              apb_proxy_hdlr_tmo_update_cmd)

/**
 * format: AT+TMODEREG
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+TMODEREG",              apb_proxy_hdlr_tmo_deregister_cmd)

/**
 * format: AT+TMOREPORT=<objectid>,<instanceid>,<resourceid>,<data>[,<ack>]
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+TMOREPORT",              apb_proxy_hdlr_tmo_report_cmd)
#endif

#ifdef MTK_CTM2M_SUPPORT
/**
 * format: AT+CTM2MINIT=<Sever_IP>,<Port>,<lifetime>,<Auto TAU Timer Update Indication>,<Security mode>[,(<PSKID>,<PSK>)]
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+CTM2MINIT",              apb_proxy_hdlr_ctm2m_initial_cmd)

/**
 * format: AT+CTM2MREG=[<Lifetime>]
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+CTM2MREG",               apb_proxy_hdlr_ctm2m_register_cmd)

/**
 * format: AT+CTM2MUPDATE=<Binding Mode>
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+CTM2MUPDATE",            apb_proxy_hdlr_ctm2m_update_cmd)

/**
 * format: AT+CTM2MDEREG
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+CTM2MDEREG",             apb_proxy_hdlr_ctm2m_deregister_cmd)

/**
 * format: AT+CTM2MSEND=<Data>[,<mode>]
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+CTM2MSEND",              apb_proxy_hdlr_ctm2m_send_cmd)

/**
 * format: AT+CTM2MVER
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+CTM2MVER",              apb_proxy_hdlr_ctm2m_version_cmd)
#endif

#ifdef MTK_CTIOT_SUPPORT
/**
 * format: AT+M2MCLINEW=<Sever_IP>,<Port>,<Endpoint_Name>,<lifetime>[,<pskid>,<psk>]
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+M2MCLINEW",          apb_proxy_hdlr_ctiot_register_cmd)

/**
 * format: AT+M2MCLIDEL
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+M2MCLIDEL",          apb_proxy_hdlr_ctiot_deregister_cmd)

/**
 * format: AT+M2MCLISEND=<Data>
 * Response:  OK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+M2MCLISEND",         apb_proxy_hdlr_ctiot_send_cmd)
#endif

#ifdef MTK_COAP_SUPPORT
/**
 * format: AT+EHTTPCON=<url>
 * url: http server url
 * cert: for https [option]
 * key: for https [option]
 * Response:  +EHTTPCON=id\r\nOK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ECOAPDEL",          apb_proxy_hdlr_coap_del_cmd)

/**
 * format: AT+EHTTPDISCON=<id>
 * id: http client id
 * Response:  +EHTTPCON=id\r\nOK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ECOAPNEW",          apb_proxy_hdlr_coap_new_cmd)

/**
 * format: AT+EHTTPSEND=<id>,<method>,<data_len>,<data>
 * id: http client id
 * method: http method
 * data_len: length of data
 * data: raw data
 * Response:  +EHTTPCON=id\r\nOK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ECOAPSEND",          apb_proxy_hdlr_coap_send_cmd)

/**
 * format: AT+EHTTPCON=<url>
 * url: http server url
 * cert: for https [option]
 * key: for https [option]
 * Response:  +EHTTPCON=id\r\nOK\r\n     ERROR\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ECOAPSTA",          apb_proxy_hdlr_coap_sta_cmd)
#endif

/**
 * format: AT+ETLSCFG=<tid>,<type>,<value>[,<type>,<value>[,<type>,<value>[,...]]]
 * mode:  0 -execute mode; 1 - active mode; 2 - read mode; 3 - test mode
 * notice: string param should be wrapped by "". Only pem format is supported for the server CA, client certificate
 *            and private-key. <tid> indicates the TLS connection to be created. <type> indicates the parameter to
 *            set while <value> indicates the parameter value. Here are the type-value paires supported:
 *            1-server name (string);
 *            2-port (int, default value is 443);
 *            3-socket type (0-tcp, tcp supported only, default value is 0);
 *            4-auth_mode (int, 0-none, 1-optional, 2-required, default value is 2);
 *            5-debug level (int, 0~4, 0-no log, 4-all log enabled, default value is 0); 
 *            6-server CA (<size><more><certificate>, size (int)-total size of the certificate without the terminate null;
 *                                  more(int)-is there more certificate content needed to be sent, 1-yes, 0-no; certificate (string)-
 *                                  the total or particial of the certificate content. default value for type 6 is null);
 *            7-client certificate (same as 6-server CA, default value for type 7 is null); 
 *            8-client private key (<size><more><private-key>, size and more is the same as 6-server CA, private-key (
 *                                            string)-the total or partical of the private-key, default value for type 8 is null)
 * example: AT+ETLSCFG=1,1,"www.example.com",2,443,3,0,4,2
 *                AT+ETLSCFG=1,6,510,1,"partial of server ca certificate content"
  *               AT+ETLSCFG=1,6,510,0,"the rest of server ca certificate content"
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ETLSCFG",          apb_proxy_hdlr_etlscfg_cmd)



AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ETLSCONN",          apb_proxy_hdlr_etlsconn_cmd)


AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ETLSCLOSE",          apb_proxy_hdlr_etlsclose_cmd)


AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ETLSSEND",          apb_proxy_hdlr_etlssend_cmd)


AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ETLSRECV",          apb_proxy_hdlr_etlsrecv_cmd)

#endif/* __APB_PROXY_NW_CMD_DEF__H__ */

