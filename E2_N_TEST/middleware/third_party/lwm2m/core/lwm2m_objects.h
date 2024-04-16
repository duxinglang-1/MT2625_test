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

#ifndef __LWM2M_OBJECTS_H__
#define __LWM2M_OBJECTS_H__

#include "liblwm2m.h"

/*
 * object_security.c
 */
lwm2m_object_t * get_security_object(int serverId, const char* serverUri, char * bsPskId, char * psk, uint16_t pskLen, bool isBootstrap);
void clean_security_object(lwm2m_object_t * objectP);
char * get_server_uri(lwm2m_object_t * objectP, uint16_t secObjInstID);
void display_security_object(lwm2m_object_t * objectP);
void copy_security_object(lwm2m_object_t * objectDest, lwm2m_object_t * objectSrc);

/*
 * object_server.c
 */
lwm2m_object_t * get_server_object(int serverId, const char* binding, int lifetime, bool storing);
lwm2m_object_t * get_server_object_ex(int instanceId, int serverId, const char* binding, int lifetime, bool storing);
void clean_server_object(lwm2m_object_t * object);
void display_server_object(lwm2m_object_t * objectP);
void copy_server_object(lwm2m_object_t * objectDest, lwm2m_object_t * objectSrc);
void set_server_lifetime(lwm2m_context_t * contextP, time_t lifetime, uint16_t instanceId);
void set_server_binding(lwm2m_context_t * contextP, lwm2m_binding_t binding, uint16_t instanceId);

/*
 * object_access_control.c
 */
lwm2m_object_t* acc_ctrl_create_object(void);
void acl_ctrl_free_object(lwm2m_object_t * objectP);
bool acc_ctrl_obj_add_inst (lwm2m_object_t* accCtrlObjP, uint16_t instId,
                 uint16_t acObjectId, uint16_t acObjInstId, uint16_t acOwner);
bool acc_ctrl_oi_add_ac_val(lwm2m_object_t* accCtrlObjP, uint16_t instId,
                 uint16_t aclResId, uint16_t acValue);

/*
 * object_device.c
 */
lwm2m_object_t * get_object_device(void);
void free_object_device(lwm2m_object_t * objectP);
uint8_t device_change(lwm2m_data_t * dataArray, lwm2m_object_t * objectP);
void display_device_object(lwm2m_object_t * objectP);
char * device_get_hw_version(void);
char * device_get_sw_version(void);

/*
 * object_connectivity_moni.c
 */
lwm2m_object_t * get_object_conn_m(void);
void free_object_conn_m(lwm2m_object_t * objectP);
uint8_t connectivity_moni_change(lwm2m_data_t * dataArray, lwm2m_object_t * objectP);
void connectivity_moni_set_handle(lwm2m_context_t *lwm2m_handle);
void connectivity_moni_observe_notify(lwm2m_context_t *lwm2m_handle, bool observe, lwm2m_uri_t *uriP);
void connectivity_moni_set_parameter_notify(lwm2m_context_t *lwm2m_handle, lwm2m_uri_t *uriP, lwm2m_attributes_t *attrP);
void connectivity_moni_min_period_change(lwm2m_context_t *lwm2m_handle, uint32_t min_period);
void connectivity_moni_max_period_change(lwm2m_context_t *lwm2m_handle, uint32_t max_period);

/*
 * object_firmware.c
 */
lwm2m_object_t * get_object_firmware(void);
void free_object_firmware(lwm2m_object_t * objectP);
void display_firmware_object(lwm2m_object_t * objectP);
void firmware_set_instance(lwm2m_context_t* instance);

/*
 * object_location.c
 */
lwm2m_object_t * get_object_location(void);
void free_object_location(lwm2m_object_t * object);
void display_location_object(lwm2m_object_t * objectP);

/*
 * object_connectivity_stat.c
 */
lwm2m_object_t * get_object_conn_s(void);
void free_object_conn_s(lwm2m_object_t * objectP);
void conn_s_updateTxStatistic(lwm2m_object_t * objectP, uint16_t txDataByte, bool smsBased);
void conn_s_updateRxStatistic(lwm2m_object_t * objectP, uint16_t rxDataByte, bool smsBased);

/*
 * object_cellular_conn.c
 */
lwm2m_object_t * get_object_cellular_connectivity(void);
void free_object_cellular_connectivity(lwm2m_object_t * objectP);
uint8_t cellular_connectivity_change(lwm2m_data_t * dataArray, lwm2m_object_t * objectP);
void display_cellular_connectivity_object(lwm2m_object_t * objectP);

/* object_watermeter.c  */
lwm2m_object_t * get_object_watermeter(void);
void free_object_watermeter(lwm2m_object_t * object);
void display_watermeter_object(lwm2m_object_t * objectP);
uint8_t watermeter_change(lwm2m_data_t * dataArray, lwm2m_object_t * objectP);

/* object_accelerometer.c  */
lwm2m_object_t * get_object_accelerometer(void);
void free_object_accelerometer(lwm2m_object_t * object);
void display_accelerometer_object(lwm2m_object_t * objectP);
uint8_t accelerometer_change(lwm2m_data_t * dataArray, lwm2m_object_t * objectP);

/* object_barometer.c  */
lwm2m_object_t * get_object_barometer(void);
void free_object_barometer(lwm2m_object_t * object);
void display_barometer_object(lwm2m_object_t * objectP);
uint8_t barometer_change(lwm2m_data_t * dataArray, lwm2m_object_t * objectP);

#endif /* __LWM2M_OBJECTS_H__ */
