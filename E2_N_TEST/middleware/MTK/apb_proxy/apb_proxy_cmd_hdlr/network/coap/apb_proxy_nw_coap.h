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

#ifndef __APB_PROXY_NW_COAP_H__
#define __APB_PROXY_NW_COAP_H__

#include "task.h"
#include "coap.h"


#define COAP_CLIENT_MAX_NUM          (2)
#define COAP_SERVER_MAX_NUM          (1)

#define COAP_MAX_NUM                 (COAP_CLIENT_MAX_NUM + COAP_SERVER_MAX_NUM)

#define COAP_CLIENT_DATA_MAX_LEN     (512)

/* CoAP sign mask */
#define COAP_DEV_SIGN_USED           (1 << 0)
#define COAP_DEV_SIGN_SRV            (1 << 7)               /* CoAP role mask */

/* CoAP context flag */
#define NW_COAP_FLAG_INIT            (1 << 0)
#define NW_COAP_FLAG_DATA_RETENTION  (1 << 1)

#define NW_CAOP_INVALID_CID          (0)

#define NW_COAP_INVALID_PDN          (0)

/* CoAP PDU length */
#define NW_COAP_MAX_PDU_LENGTH       (1024)


#if !defined (MTK_DEBUG_LEVEL_NONE)
#define NW_COAP_LOG(fmt, ...)               LOG_I(COAP, "[COAP]"fmt, ##__VA_ARGS__)
#else
#define NW_COAP_LOG(fmt, ...)               printf("[COAP]"fmt, ##__VA_ARGS__)
#endif


#define NW_COAP_CLI_TASK_NAME              "coapc"
#define NW_COAP_CLI_TASK_PRIORITY          TASK_PRIORITY_NORMAL
#define NW_COAP_CLI_TASK_STACK_SIZE        (2 * 1024)

#define NW_COAP_CLI_TASK_DELAY             (1000)      /* 1000ms */

#define NW_COAP_SRV_TASK_NAME              "coaps"
#define NW_COAP_SRV_TASK_PRIORITY          TASK_PRIORITY_NORMAL
#define NW_COAP_SRV_TASK_STACK_SIZE        (2 * 1024)

#define NW_COAP_SRV_TASK_DELAY             (1000)      /* 1000ms */

#define NW_COAP_SRV_MAX_AGE                (0x05)

typedef uint8_t COAP_ID;

#define NW_COAP_RET_SUCCESS          (0)
#define NW_COAP_RET_FAIL             (1)

typedef enum {
    COAP_DEV_ROLE_CLIENT,
    COAP_DEV_ROLE_SERVER,
} coap_dev_role_t;

typedef enum {
    COAP_DEV_TYPE_UNUSED,
    COAP_DEV_TYPE_SOCKET,
    COAP_DEV_TYPE_CID,
    COAP_DEV_TYPE_CTX,
    COAP_DEV_TYPE_SET,
} coap_dev_type_t;

typedef struct {
    uint8_t sign;
    COAP_ID cid;
    uint8_t pdn;
    coap_address_t dst;
    coap_context_t *coap_ctx;
    TaskHandle_t task_hd;
} coap_dev_info_t;

typedef struct {
    uint8_t flag;
    uint8_t enable_log;
    coap_dev_info_t coap_dev[COAP_MAX_NUM];
} nw_coap_context_t;

typedef struct {
    uint8_t pdn;
    uint8_t sa_family;
    uint16_t port;
    uint32_t ip_addr;
} nw_coap_retention_data_t;



#define NW_COAP_SET_FLAG(FLAG, MASK) do { \
    (FLAG) |= (MASK); \
} while(0);

#define NW_COAP_RESET_FLAG(FLAG, MASK) do { \
    (FLAG) &= ~(MASK); \
} while(0);



void nw_coap_init(void);

coap_dev_info_t *nw_coap_find_dev(coap_dev_type_t type, void *param);

void nw_coap_reset_dev(coap_dev_info_t *dev);

nw_coap_context_t *nw_coap_get_ctx();

int32_t nw_coap_new_context(coap_dev_info_t *dev);

void nw_coap_free_context(coap_dev_info_t *dev);

void nw_coap_retention_set(coap_dev_info_t *dev);

void nw_coap_retention_reset(coap_dev_info_t *dev);

uint8_t nw_coap_get_sa_family(char *addr);

int32_t nw_coap_send_request(COAP_ID cid, char *data_buf, uint32_t data_len);

void nw_coap_show_dev(coap_dev_info_t *dev);

void nw_coap_enable_log(void);

void nw_coap_disable_log(void);

void nw_coap_dump_buff(char *title, char *buf, uint32_t size);

void nw_coap_show_pdu(char *title, const coap_pdu_t *pdu);

void nw_coap_show_addr(char *title, const coap_address_t *addr);

#endif /* __APB_PROXY_NW_COAP_H__ */

