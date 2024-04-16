#ifndef __LWM2M_LWM2M_APP_ITEM_MANAGER_H__
#define __LWM2M_LWM2M_APP_ITEM_MANAGER_H__

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

#include "liblwm2m.h"
#if defined(WITH_MBEDTLS)
#include "mbedtlsconnection.h"
#elif defined(WITH_TINYDTLS)
#include "dtlsconnection.h"
#else
#include "connection.h"
#endif
#include "FreeRTOS.h"
#include "semphr.h"

#define SUPPORT_LWM2M_ATCMD_INSTANCE_MAX_NUM   1

typedef struct
{
    lwm2m_object_t * securityObjP;
    lwm2m_object_t * serverObject;
    int sock;
#if defined(WITH_MBEDTLS)
    mbedtls_connection_t * connList;
#elif defined(WITH_TINYDTLS)
    dtls_connection_t * connList;
#else
    connection_t * connList;
#endif
    lwm2m_context_t * lwm2mH;
    int addressFamily;
} client_data_t;

struct lwm2m_atcmd_t
{
    bool is_used;
    uint8_t lwm2m_id;
    char * localPort;
    char * server;
    char * serverPort;
    int addressFamily;
    char * name;
    uint16_t pskLen;
    char * pskId;
    char * psk;
    char * pskBuffer;
    int lifetime;
    client_data_t client_data;
    lwm2m_context_t* context;
#if defined(WITH_MBEDTLS)
    mbedtls_connection_t * connP;
#elif defined(WITH_TINYDTLS)
    dtls_connection_t * connP;
#else
    connection_t * connP;
#endif
    SemaphoreHandle_t readWriteSemph;
    lwm2m_data_t *read_result_list;
    int         read_result_cnt;
    uint8_t     write_result;
};

#define LWM2M_ATCMD_MAX_PORT_LEN    8
#define LWM2M_ATCMD_MAX_SERVER_LEN  40
#define LWM2M_ATCMD_MAX_NAME_LEN    32
#define LWM2M_ATCMD_MAX_PSKID_LEN   32
#define LWM2M_ATCMD_MAX_PSK_LEN     32

void * lwm2m_atcmd_connect_server(uint16_t secObjInstID, void * userData);
void lwm2m_atcmd_close_connection(void * sessionH, void * userData);

#endif /* __LWM2M_LWM2M_APP_ITEM_MANAGER_H__ */
