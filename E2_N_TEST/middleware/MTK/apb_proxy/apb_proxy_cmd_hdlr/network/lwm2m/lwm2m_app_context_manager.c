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

#if defined(MTK_LWM2M_SUPPORT)

#include "lwm2m_app_context_manager.h"
#include "internals.h"

#if defined(WITH_MBEDTLS)
void * lwm2m_atcmd_connect_server(uint16_t secObjInstID,
                            void * userData)
{
  client_data_t * dataP;
  lwm2m_list_t * instance;
  mbedtls_connection_t * newConnP = NULL;
  dataP = (client_data_t *)userData;
  lwm2m_object_t  * securityObj = dataP->securityObjP;

  instance = LWM2M_LIST_FIND(dataP->securityObjP->instanceList, secObjInstID);
  if (instance == NULL) return NULL;

  newConnP = connection_create(dataP->connList, dataP->sock, securityObj, instance->id, dataP->lwm2mH, dataP->addressFamily);
  if (newConnP == NULL)
  {
      LOG("Connection creation failed.\n");
      if (dataP->lwm2mH->notify_callback != NULL) dataP->lwm2mH->notify_callback(LWM2M_NOTIFY_TYPE_CONNECT, LWM2M_NOTIFY_CODE_FAILED, 0);
      return NULL;
  }

  if (dataP->lwm2mH->notify_callback != NULL) dataP->lwm2mH->notify_callback(LWM2M_NOTIFY_TYPE_CONNECT, LWM2M_NOTIFY_CODE_SUCCESS, 0);

  dataP->connList = newConnP;
  return (void *)newConnP;
}
#elif defined(WITH_TINYDTLS)
void * lwm2m_atcmd_connect_server(uint16_t secObjInstID,
                            void * userData)
{
  client_data_t * dataP;
  lwm2m_list_t * instance;
  dtls_connection_t * newConnP = NULL;
  dataP = (client_data_t *)userData;
  lwm2m_object_t  * securityObj = dataP->securityObjP;

  instance = LWM2M_LIST_FIND(dataP->securityObjP->instanceList, secObjInstID);
  if (instance == NULL) return NULL;

  newConnP = connection_create(dataP->connList, dataP->sock, securityObj, instance->id, dataP->lwm2mH, dataP->addressFamily);
  if (newConnP == NULL)
  {
      LOG("Connection creation failed.\n");
      if (dataP->lwm2mH->notify_callback != NULL) dataP->lwm2mH->notify_callback(LWM2M_NOTIFY_TYPE_CONNECT, LWM2M_NOTIFY_CODE_FAILED, 0);
      return NULL;
  }

  if (dataP->lwm2mH->notify_callback != NULL) dataP->lwm2mH->notify_callback(LWM2M_NOTIFY_TYPE_CONNECT, LWM2M_NOTIFY_CODE_SUCCESS, 0);

  dataP->connList = newConnP;
  return (void *)newConnP;
}
#else
void * lwm2m_atcmd_connect_server(uint16_t secObjInstID,
                            void * userData)
{
    client_data_t * dataP;
    char * uri;
    char * host;
    char * port;
    connection_t * newConnP = NULL;

    dataP = (client_data_t *)userData;

    uri = get_server_uri(dataP->securityObjP, secObjInstID);
    LOG_ARG("App uri:%s\r\n", uri);
    if (uri == NULL) return NULL;

    // parse uri in the form "coaps://[host]:[port]"
    if (0==strncmp(uri, "coaps://", strlen("coaps://"))) {
        host = uri+strlen("coaps://");
    }
    else if (0==strncmp(uri, "coap://",  strlen("coap://"))) {
        host = uri+strlen("coap://");
    }
    else {
        goto exit;
    }
    port = strrchr(host, ':');
    if (port == NULL) goto exit;
    // remove brackets
    if (host[0] == '[')
    {
        host++;
        if (*(port - 1) == ']')
        {
            *(port - 1) = 0;
        }
        else goto exit;
    }
    // split strings
    *port = 0;
    port++;

    LOG_ARG("Opening connection to server at %s:%s\r\n", host, port);
    LOG_ARG("App Opening connection to server at %s:%s\r\n", host, port);
    newConnP = connection_create(dataP->connList, dataP->sock, host, port, dataP->addressFamily);
    if (newConnP == NULL) {
        LOG("Connection creation failed.\r\n");
        if (dataP->lwm2mH->notify_callback != NULL) dataP->lwm2mH->notify_callback(LWM2M_NOTIFY_TYPE_CONNECT, LWM2M_NOTIFY_CODE_FAILED, 0);
    }
    else {
        dataP->connList = newConnP;
        if (dataP->lwm2mH->notify_callback != NULL) dataP->lwm2mH->notify_callback(LWM2M_NOTIFY_TYPE_CONNECT, LWM2M_NOTIFY_CODE_SUCCESS, 0);
    }

exit:
    lwm2m_free(uri);
    return (void *)newConnP;
}
#endif

void lwm2m_atcmd_close_connection(void * sessionH,
                            void * userData)
{
    client_data_t * app_data;
#if defined(WITH_MBEDTLS)
    mbedtls_connection_t * targetP;
#elif defined(WITH_TINYDTLS)
    dtls_connection_t * targetP;
#else
    connection_t * targetP;
#endif

    app_data = (client_data_t *)userData;
#if defined(WITH_MBEDTLS)
    targetP = (mbedtls_connection_t *)sessionH;
#elif defined(WITH_TINYDTLS)
    targetP = (dtls_connection_t *)sessionH;
#else
    targetP = (connection_t *)sessionH;
#endif

    if (targetP == app_data->connList)
    {
        app_data->connList = targetP->next;
        lwm2m_free(targetP);
    }
    else
    {
#if defined(WITH_MBEDTLS)
        mbedtls_connection_t * parentP;
#elif defined(WITH_TINYDTLS)
        dtls_connection_t * parentP;
#else
        connection_t * parentP;
#endif

        parentP = app_data->connList;
        while (parentP != NULL && parentP->next != targetP)
        {
            parentP = parentP->next;
        }
        if (parentP != NULL)
        {
            parentP->next = targetP->next;
            lwm2m_free(targetP);
        }
    }
}

#endif
