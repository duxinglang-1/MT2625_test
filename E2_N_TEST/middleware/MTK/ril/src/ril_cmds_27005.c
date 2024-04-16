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


#include "ril_general_types.h"

#include "ril.h"
#include "ril_internal_use.h"
#include "ril_cmds_def.h"
#include "ril_task.h"
#include "ril_utils.h"
#include "ril_log.h"

#include "ril_cmds_27005.h"
#include "ril_cmds_common.h"

#if defined(__RIL_SMS_COMMAND_SUPPORT__)
/* AT+CSMS */
ril_status_t ril_request_select_message_service(ril_request_mode_t mode,
        int32_t service,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, service, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CSMS, list_head, (void *)callback,  user_data);
    }
}


/* AT+CPMS*/
ril_status_t ril_request_preferred_message_storage(ril_request_mode_t mode,
        char *mem1,
        char *mem2,
        char *mem3,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {

            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* mem1, necessary */
            if ((ret = ril_param_list_add_string_optional(list_head, mem1, 2)) < 0) {
                break;
            }

            /* mem2, optional */
            if ((ret = ril_param_list_add_string_optional(list_head, mem2, 0)) < 0) {
                break;
            }

            /* mem3, optional */
            if ((ret = ril_param_list_add_string_optional(list_head, mem3, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CPMS, list_head, (void *)callback,  user_data);
    }
}


/* AT+CMGF */
ril_status_t ril_request_message_format(ril_request_mode_t mode,
                                        int32_t message_mode,
                                        ril_cmd_response_callback_t callback,
                                        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* mode, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, message_mode, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CMGF, list_head, (void *)callback,  user_data);
    }
}


/* AT+CSCA */
ril_status_t ril_request_service_centre_address(ril_request_mode_t mode,
        char *sca,
        int32_t tosca,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;;
            }

            /* sca, necessary */
            if ((ret = ril_param_list_add_string_optional(list_head, sca, 2)) < 0) {
                break;
            }

            /* tosca, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, tosca, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CSCA, list_head, (void *)callback,  user_data);
    }
}


/* AT+CSMP */
ril_status_t ril_request_set_text_mode_parameters(ril_request_mode_t mode,
        int32_t fo,
        int32_t vp,
        int32_t pid,
        int32_t dcs,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* fo, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, fo, 0)) < 0) {
                break;
            }

            /* vp, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, vp, 0)) < 0) {
                break;
            }

            /* pid, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, pid, 0)) < 0) {
                break;
            }

            /* dcs, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, dcs, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CSMP, list_head, (void *)callback,  user_data);
    }
}


/* AT+CSDH */
ril_status_t ril_request_show_text_mode_parameters(ril_request_mode_t mode,
        int32_t show,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* show, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, show, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CSDH, list_head, (void *)callback,  user_data);
    }
}


/* AT+CSAS */
ril_status_t ril_request_save_settings(ril_request_mode_t mode,
                                       int32_t profile,
                                       ril_cmd_response_callback_t callback,
                                       void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;;
            }

            /* profile, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, profile, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CSAS, list_head, (void *)callback,  user_data);
    }
}


/* AT+CRES */
ril_status_t ril_request_restore_settings(ril_request_mode_t mode,
        int32_t profile,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* profile, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, profile, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CRES, list_head, (void *)callback,  user_data);
    }
}


/* AT+CNMI */
ril_status_t ril_request_new_message_indication(ril_request_mode_t mode,
        ril_new_message_indication_param_t *param,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if (param == NULL) {
                return RIL_STATUS_INVALID_PARAM;
            }

            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* mode, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, param->mode, 0)) < 0) {
                break;
            }

            /* mt, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, param->mt, 0)) < 0) {
                break;
            }

            /* bm, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, param->bm, 0)) < 0) {
                break;
            }

            /* ds, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, param->ds, 0)) < 0) {
                break;
            }

            /* bfr, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, param->bfr, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CNMI, list_head, (void *)callback,  user_data);
    }
}


// pdu mode
/* AT+CMGL */
ril_status_t ril_request_list_messages_pdu(ril_request_mode_t mode,
        int32_t stat,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;;
            }

            /* stat, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, stat, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CMGL_PDU, list_head, (void *)callback,  user_data);
    }
}


/* AT+CMGR */
ril_status_t ril_request_read_message_pdu(ril_request_mode_t mode,
        int32_t index,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* index, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, index, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CMGR_PDU, list_head, (void *)callback,  user_data);
    }
}


/* AT+CMGS */
ril_status_t ril_request_send_message_pdu(ril_request_mode_t mode,
        int32_t length,
        char *pdu,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* length, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, length, 2)) < 0) {
                break;
            }

            /* pdu, necessary */
            if ((ret = ril_param_list_add_digital_string_optional(list_head, pdu, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CMGS_PDU, list_head, (void *)callback,  user_data);
    }
}


/* AT+CMGW */
ril_status_t ril_request_write_message_pdu(ril_request_mode_t mode,
        int32_t length,
        int32_t stat,
        char *pdu,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* length, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, length, 2)) < 0) {
                break;
            }

            /* stat, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, stat, 0)) < 0) {
                ret = 0;
            }

            /* pdu, necessary */
            if ((ret = ril_param_list_add_string_optional(list_head, pdu, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CMGW_PDU, list_head, (void *)callback,  user_data);
    }
}


/* AT+CMGD */
ril_status_t ril_request_delete_message(ril_request_mode_t mode,
                                        int32_t index,
                                        int32_t delflag,
                                        ril_cmd_response_callback_t callback,
                                        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* index, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, index, 2)) < 0) {
                break;
            }

            /* delflag, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, delflag, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CMGD, list_head, (void *)callback,  user_data);
    }
}


/* AT+CMGC */
ril_status_t ril_request_send_command_pdu(ril_request_mode_t mode,
        int32_t length,
        char *pdu,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* length, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, length, 2)) < 0) {
                break;
            }

            /* pdu, necessary */
            if ((ret = ril_param_list_add_string_optional(list_head, pdu, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CMGC_PDU, list_head, (void *)callback,  user_data);
    }
}


/* AT+CNMA */
ril_status_t ril_request_new_message_ack_pdu(ril_request_mode_t mode,
        int32_t n,
        int32_t length,
        char *pdu,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* n, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, n, 2)) < 0) {
                break;
            }

            /* length, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, length, 0)) < 0) {
                break;
            }

            /* pdu, optional */
            if ((ret = ril_param_list_add_string_optional(list_head, pdu, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CNMA_PDU, list_head, (void *)callback,  user_data);
    }
}


/* AT+CMSS */
ril_status_t ril_request_send_message_from_storage_pdu(ril_request_mode_t mode,
        int32_t index,
        char *da,
        int32_t toda,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* index, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, index, 2)) < 0) {
                break;
            }

            /* da, optional */
            if ((ret = ril_param_list_add_string_optional(list_head, da, 0)) < 0) {
                break;
            }

            /* toda, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, toda, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CMSS_PDU, list_head, (void *)callback,  user_data);
    }
}


// text mode
/* AT+CMGL */
ril_status_t ril_request_list_messages_txt(ril_request_mode_t mode,
        char *stat,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* stat, necessary */
            if ((ret = ril_param_list_add_string_optional(list_head, stat, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CMGL_TXT, list_head, (void *)callback,  user_data);
    }
}


/* AT+CMGR */
ril_status_t ril_request_read_message_txt(ril_request_mode_t mode,
        int32_t index,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* index, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, index, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CMGR_TXT, list_head, (void *)callback,  user_data);
    }
}


/* AT+CMGS */
ril_status_t ril_request_send_message_txt(ril_request_mode_t mode,
        char *da,
        int32_t toda,
        char *text,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* da, necessary */
            if ((ret = ril_param_list_add_string_optional(list_head, da, 2)) < 0) {
                break;
            }

            /* toda, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, toda, 0)) < 0) {
                ret = 0;
            }

            /* text, necessary */
            if ((ret = ril_param_list_add_string_optional(list_head, text, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CMGS_TXT, list_head, (void *)callback,  user_data);
    }
}


/* AT+CMGW */
ril_status_t ril_request_write_message_txt(ril_request_mode_t mode,
        char *addr,
        int32_t to_addr,
        char *stat,
        char *text,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* addr(oa/da), necessary */
            if ((ret = ril_param_list_add_string_optional(list_head, addr, 2)) < 0) {
                break;
            }

            /* to_addr(tooa/toda), optional */
            if ((ret = ril_param_list_add_int_optional(list_head, to_addr, 0)) < 0) {
                ret = 0;
            }

            /* stat, optional */
            if ((ret = ril_param_list_add_string_optional(list_head, stat, 0)) < 0) {
                break;
            }

            /* text, optional */
            if ((ret = ril_param_list_add_string_optional(list_head, text, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CMGW_TXT, list_head, (void *)callback,  user_data);
    }
}


/* AT+CMGC */
ril_status_t ril_request_send_command_txt(ril_request_mode_t mode,
        ril_send_command_param_t *cmd_param,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if (cmd_param == NULL) {
                return RIL_STATUS_INVALID_PARAM;
            }

            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* fo */
            if ((ret = ril_param_list_add_int_optional(list_head, cmd_param->fo, 2)) < 0) {
                break;
            }

            /* ct, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, cmd_param->ct, 2)) < 0) {
                break;
            }

            do {
                /* pid, optional */
                if ((ret = ril_param_list_add_int_optional(list_head, cmd_param->pid, 0)) < 0) {
                    break;
                }

                /* mn, optional */
                if ((ret = ril_param_list_add_int_optional(list_head, cmd_param->mn, 0)) < 0) {
                    break;
                }

                /* da, optional */
                if ((ret = ril_param_list_add_string_optional(list_head, cmd_param->da, 0)) < 0) {
                    break;
                }

                /* toda, optional */
                if ((ret = ril_param_list_add_int_optional(list_head, cmd_param->toda, 0)) < 0) {
                    break;
                }
            } while (0);

            /* cdata, necessary */
            if ((ret = ril_param_list_add_string_optional(list_head, cmd_param->cdata, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CMGC_TXT, list_head, (void *)callback,  user_data);
    }
}


/* AT+CNMA */
ril_status_t ril_request_new_message_ack_txt(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_CNMA_TXT, NULL, (void *)callback,  user_data);
}


/* AT+CMSS */
ril_status_t ril_request_send_message_from_storage_txt(ril_request_mode_t mode,
        int32_t index,
        char *da,
        int32_t toda,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                break;
            }

            /* index, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, index, 2)) < 0) {
                break;
            }

            /* da, optional */
            if ((ret = ril_param_list_add_string_optional(list_head, da, 0)) < 0) {
                ret = 0;
            }

            /* toda, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, toda, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CMSS_TXT, list_head, (void *)callback,  user_data);
    }
}


/*************************************************
 *                 send hdlr
 *************************************************/

/* AT+CMGW */
int32_t ril_cmd_send_write_message_pdu(uint32_t channel_id, char *buf, ril_request_info_t *info)
{
    ril_cmd_item_t *item = get_at_cmd_item((ril_cmd_id_t)info->request_id);
    ril_param_node_t *list_head = (ril_param_node_t *)info->request_param;
    ril_param_desc_t *param;
    char ctrl_z[2] = {26, 0};

    switch (info->mode) {
        case RIL_EXECUTE_MODE:
            /* consturct command head */
            snprintf(buf, RIL_BUF_SIZE - 1, "%s%s=", AT_PREFIX, item->cmd_head);

            /* append  params */
            ril_param_list_get(&list_head, &param);
            if (param->data_type == RIL_PARAM_TYPE_INTEGER) {
                ril_cmd_string_param_int_append(buf, param->data.val, false);
            }

            ril_param_list_get(&list_head, &param);
            if (param->data_type == RIL_PARAM_TYPE_INTEGER) {
                ril_cmd_string_param_int_append(buf, param->data.val, true);
                strcat(buf, "\r");
                ril_param_list_get(&list_head, &param);
                ril_cmd_string_param_string_append(buf, param->data.str, false);
                strcat(buf, ctrl_z);
            } else if (param->data_type == RIL_PARAM_TYPE_STRING) {
                strcat(buf, "\r");
                ril_cmd_string_param_string_append(buf, param->data.str, false);
                strcat(buf, ctrl_z);
            }
            strcat(buf, "\r\n");
            break;
        case RIL_ACTIVE_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, active_mode_format, AT_PREFIX, item->cmd_head);
            break;
        case RIL_READ_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, read_mode_format, AT_PREFIX, item->cmd_head);
            break;
        case RIL_TEST_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, test_mode_format, AT_PREFIX, item->cmd_head);
            break;
        default:
            configASSERT(0);
    }

    return strlen(buf);
}


/* AT+CMGS */
int32_t ril_cmd_send_send_message_pdu(uint32_t channel_id, char *buf, ril_request_info_t *info)
{
    ril_cmd_item_t *item = get_at_cmd_item((ril_cmd_id_t)info->request_id);
    ril_param_node_t *list_head = (ril_param_node_t *)info->request_param;
    ril_param_desc_t *param;
    int32_t round1_len = 0;
    int32_t round2_len;
    char ctrl_z[2] = {26, 0};

    switch (info->mode) {
        case RIL_EXECUTE_MODE:
            /* consturct command head */
            snprintf(buf, RIL_BUF_SIZE - 1, "%s%s=", AT_PREFIX, item->cmd_head);

            /* append  params */
            ril_param_list_get(&list_head, &param);
            if (param->data_type == RIL_PARAM_TYPE_INTEGER) {
                ril_cmd_string_param_int_append(buf, param->data.val, false);
                strcat(buf, "\r");
            }
            round1_len = strlen(buf);
            buf[round1_len] = '\0';  // ends up with termination char for round 1 sending buffer.

            buf += (round1_len + 1);
            /* pdu raw data, should be sent in second round. */
            ril_param_list_get(&list_head, &param);
            if (param->data_type == RIL_PARAM_TYPE_DIGITAL_STRING) {
                ril_cmd_string_param_digital_string_append(buf, param->data.str, false);
            }

            round2_len = strlen(buf);
            buf[round2_len] = '\0';

            buf += (round2_len + 1);

            /* ctrl + z */
            strcat(buf, ctrl_z);
            break;
        case RIL_ACTIVE_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, active_mode_format, AT_PREFIX, item->cmd_head);
            break;
        case RIL_READ_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, read_mode_format, AT_PREFIX, item->cmd_head);
            break;
        case RIL_TEST_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, test_mode_format, AT_PREFIX, item->cmd_head);
            break;
        default:
            RIL_LOGE("Not support mode\r\n");
            configASSERT(0);
    }

    return info->mode == RIL_EXECUTE_MODE ? round1_len : strlen(buf);
}


/* AT+CMGC */
int32_t ril_cmd_send_send_command_pdu(uint32_t channel_id, char *buf, ril_request_info_t *info)
{
    /* the same as CMGS cmd send hdlr */
    return ril_cmd_send_send_message_pdu(channel_id, buf, info);
}


/* AT+CNMA */
int32_t ril_cmd_send_new_message_ack_pdu(uint32_t channel_id, char *buf, ril_request_info_t *info)
{
    ril_cmd_item_t *item = get_at_cmd_item((ril_cmd_id_t)info->request_id);
    ril_param_node_t *list_head = (ril_param_node_t *)info->request_param;
    ril_param_desc_t *param;
    int32_t ret;
    char ctrl_z[2] = {26, 0};

    switch (info->mode) {
        case RIL_EXECUTE_MODE:
            /* consturct command head */
            snprintf(buf, RIL_BUF_SIZE - 1, "%s%s=", AT_PREFIX, item->cmd_head);

            /* append  params */

            /* n, necessary */
            ret = ril_param_list_get(&list_head, &param);
            if (param->data_type == RIL_PARAM_TYPE_INTEGER) {
                ril_cmd_string_param_int_append(buf, param->data.val, false);
            }

            /* length optional */
            ret = ril_param_list_get(&list_head, &param);
            if (!ret && param->data_type == RIL_PARAM_TYPE_INTEGER) {
                ril_cmd_string_param_int_append(buf, param->data.val, true);
                strcat(buf, ctrl_z);
            }

            /* pdu optional */
            ret = ril_param_list_get(&list_head, &param);
            if (!ret && param->data_type == RIL_PARAM_TYPE_STRING) {
                strcat(buf, "\r");
                ril_cmd_string_param_string_append(buf, param->data.str, false);
                strcat(buf, ctrl_z);
            }
            strcat(buf, "\r\n");
            break;
        case RIL_ACTIVE_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, active_mode_format, AT_PREFIX, item->cmd_head);
            break;
        case RIL_READ_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, read_mode_format, AT_PREFIX, item->cmd_head);
            break;
        case RIL_TEST_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, test_mode_format, AT_PREFIX, item->cmd_head);
            break;
        default:
            configASSERT(0);
    }

    return strlen(buf);
}


/* AT+CMGS */
int32_t ril_cmd_send_send_message_txt(uint32_t channel_id, char *buf, ril_request_info_t *info)
{
    ril_cmd_item_t *item = get_at_cmd_item((ril_cmd_id_t)info->request_id);
    ril_param_node_t *list_head = (ril_param_node_t *)info->request_param;
    ril_param_desc_t *param;
    int32_t param_num;
    char ctrl_z[2] = {26, 0};

    switch (info->mode) {
        case RIL_EXECUTE_MODE:
            /* consturct command head */
            snprintf(buf, RIL_BUF_SIZE - 1, "%s%s=", AT_PREFIX, item->cmd_head);
            param_num = ril_param_list_get_total_num(list_head);
            /* append  params */
            ril_param_list_get(&list_head, &param);
            if (param->data_type == RIL_PARAM_TYPE_STRING) {
                ril_cmd_string_param_string_append(buf, param->data.str, false);
            }

            if (param_num == 3) {
                ril_param_list_get(&list_head, &param);
                ril_cmd_string_param_int_append(buf, param->data.val, true);
            }

            ril_param_list_get(&list_head, &param);
            if (param->data_type == RIL_PARAM_TYPE_STRING) {
                strcat(buf, "\r");
                ril_cmd_string_param_string_append(buf, param->data.str, false);
                strcat(buf, ctrl_z);
            }
            strcat(buf, "\r\n");
            break;
        case RIL_ACTIVE_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, active_mode_format, AT_PREFIX, item->cmd_head);
            break;
        case RIL_READ_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, read_mode_format, AT_PREFIX, item->cmd_head);
            break;
        case RIL_TEST_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, test_mode_format, AT_PREFIX, item->cmd_head);
            break;
        default:
            configASSERT(0);
    }

    return strlen(buf);
}


/* AT+CMGW */
int32_t ril_cmd_send_write_message_txt(uint32_t channel_id, char *buf, ril_request_info_t *info)
{
    ril_cmd_item_t *item = get_at_cmd_item((ril_cmd_id_t)info->request_id);
    ril_param_node_t *list_head = (ril_param_node_t *)info->request_param;
    ril_param_desc_t *param;
    int32_t param_num;
    char ctrl_z[2] = {26, 0};

    switch (info->mode) {
        case RIL_EXECUTE_MODE:
            /* consturct command head */
            snprintf(buf, RIL_BUF_SIZE - 1, "%s%s", AT_PREFIX, item->cmd_head);
            param_num = ril_param_list_get_total_num(list_head);
            /* append  params */
            if (param_num > 1) {
                strcat(buf, "=");
                ril_param_list_get(&list_head, &param);
                ril_cmd_string_param_string_append(buf, param->data.str, false);
            }

            if (param_num > 2) {
                ril_param_list_get(&list_head, &param);
                ril_cmd_string_param_int_append(buf, param->data.val, true);
            }

            if (param_num > 3) {
                ril_param_list_get(&list_head, &param);
                ril_cmd_string_param_string_append(buf, param->data.str, true);
            }

            ril_param_list_get(&list_head, &param);
            if (param->data_type == RIL_PARAM_TYPE_STRING) {
                strcat(buf, "\r");
                ril_cmd_string_param_string_append(buf, param->data.str, false);
                strcat(buf, ctrl_z);
            }
            strcat(buf, "\r\n");
            break;
        case RIL_ACTIVE_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, active_mode_format, AT_PREFIX, item->cmd_head);
            break;
        case RIL_READ_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, read_mode_format, AT_PREFIX, item->cmd_head);
            break;
        case RIL_TEST_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, test_mode_format, AT_PREFIX, item->cmd_head);
            break;
        default:
            configASSERT(0);
    }

    return strlen(buf);
}


/* AT+CMGC */
int32_t ril_cmd_send_send_command_txt(uint32_t channel_id, char *buf, ril_request_info_t *info)
{
    ril_cmd_item_t *item = get_at_cmd_item((ril_cmd_id_t)info->request_id);
    ril_param_node_t *list_head = (ril_param_node_t *)info->request_param;
    ril_param_desc_t *param;
    int32_t param_num;
    char ctrl_z[2] = {26, 0};

    switch (info->mode) {
        case RIL_EXECUTE_MODE:
            /* consturct command head */
            snprintf(buf, RIL_BUF_SIZE - 1, "%s%s=", AT_PREFIX, item->cmd_head);
            param_num = ril_param_list_get_total_num(list_head);
            /* append  params */
            /* fo */
            ril_param_list_get(&list_head, &param);
            if (param->data_type == RIL_PARAM_TYPE_INTEGER) {
                ril_cmd_string_param_int_append(buf, param->data.val, false);
            }
            /* ct */
            ril_param_list_get(&list_head, &param);
            if (param->data_type == RIL_PARAM_TYPE_INTEGER) {
                ril_cmd_string_param_int_append(buf, param->data.val, true);
            }
            /* pid */
            if (param_num > 3) {
                ril_param_list_get(&list_head, &param);
                ril_cmd_string_param_int_append(buf, param->data.val, true);
            }
            /* mn */
            if (param_num > 4) {
                ril_param_list_get(&list_head, &param);
                ril_cmd_string_param_int_append(buf, param->data.val, true);
            }
            /* da */
            if (param_num > 5) {
                ril_param_list_get(&list_head, &param);
                ril_cmd_string_param_string_append(buf, param->data.str, true);
            }
            /* toda */
            if (param_num > 6) {
                ril_param_list_get(&list_head, &param);
                ril_cmd_string_param_int_append(buf, param->data.val, true);
            }

            /* cdata */
            ril_param_list_get(&list_head, &param);
            if (param->data_type == RIL_PARAM_TYPE_STRING) {
                strcat(buf, "\r");
                ril_cmd_string_param_string_append(buf, param->data.str, false);
                strcat(buf, ctrl_z);
            }
            strcat(buf, "\r\n");
            break;
        case RIL_ACTIVE_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, active_mode_format, AT_PREFIX, item->cmd_head);
            break;
        case RIL_READ_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, read_mode_format, AT_PREFIX, item->cmd_head);
            break;
        case RIL_TEST_MODE:
            snprintf(buf, RIL_BUF_SIZE - 1, test_mode_format, AT_PREFIX, item->cmd_head);
            break;
        default:
            configASSERT(0);
    }

    return strlen(buf);
}

/*************************************************
 *                 response hdlr
 *************************************************/

/* AT+CSMS */
int32_t ril_response_select_message_service(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    int32_t ret = -1;
    ril_channel_cntx_t *channel_p = ril_get_channel(channel_id);
    char *line;
    char *cur_pos = cmd_buf;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;
    ril_cmd_response_t cmd_response;
    ril_select_message_service_rsp_t response;
    ril_cmd_response_callback_t rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;
    bool need_parse = true;

    /* post raw data buff directly when test mode */
    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        res_code = RIL_RESULT_CODE_OK;
        need_parse = false;
    } else {
        line = at_get_one_line(&cur_pos);
    }
    /* parse reponse */
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (channel_p->curr_request_mode == RIL_READ_MODE) {
                /* read mode, service, mt, mo, bm*/
                ret = at_tok_start(&line);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.service);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.mt);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.mo);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.bm);
                if (ret < 0) {
                    break;
                }
            } else {
                /* execute mode, mt, mo, bm*/
                ret = at_tok_start(&line);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.mt);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.mo);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.bm);
                if (ret < 0) {
                    break;
                }
            }
        }

        line = at_get_one_line(&cur_pos);
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;
}


/* AT+CPMS*/
int32_t ril_response_preferred_message_storage(uint32_t channel_id,
        char *cmd_buf,
        uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_preferred_message_storage_rsp_t response;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;

    cur_pos = cmd_buf;
    channel_p = ril_get_channel(channel_id);
    rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (channel_p->curr_request_mode == RIL_READ_MODE) {
                ret = at_tok_start(&line);
                if (ret < 0) {
                    break;
                };

                ret = at_tok_nextstr(&line, &response.mem1);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.used1);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.total1);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextstr(&line, &response.mem2);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.used2);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.total2);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextstr(&line, &response.mem3);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.used3);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.total3);
                if (ret < 0) {
                    break;
                }
            } else {
                ret = at_tok_start(&line);
                if (ret < 0) {
                    break;
                };

                ret = at_tok_nextint(&line, &response.used1);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.total1);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.used2);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.total2);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.used3);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.total3);
                if (ret < 0) {
                    break;
                }
            }
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;
}


/* AT+CMGF */
int32_t ril_response_message_format(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_message_format_rsp_t response;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;

    cur_pos = cmd_buf;
    channel_p = ril_get_channel(channel_id);
    rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.message_mode);
            if (ret < 0) {
                break;
            }
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;
}


/* AT+CSCA */
int32_t ril_response_service_centre_address(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_service_centre_address_rsp_t response;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;

    cur_pos = cmd_buf;
    channel_p = ril_get_channel(channel_id);
    rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &response.sca);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.tosca);
            if (ret < 0) {
                break;
            }
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;
}


/* AT+CSMP */
int32_t ril_response_set_text_mode_parameters(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_set_text_mode_parameters_rsp_t response;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;

    cur_pos = cmd_buf;
    channel_p = ril_get_channel(channel_id);
    rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.fo);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.vp);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.pid);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.dcs);
            if (ret < 0) {
                break;
            }
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;

}


/* AT+CSDH */
int32_t ril_response_show_text_mode_parameters(uint32_t channel_id,
        char *cmd_buf,
        uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_show_text_mode_parameters_rsp_t response;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;

    cur_pos = cmd_buf;
    channel_p = ril_get_channel(channel_id);
    rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.show);
            if (ret < 0) {
                break;
            }
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;
}


/* AT+CSAS */
int32_t ril_response_save_settings(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT+CRES */
int32_t ril_response_restore_settings(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT+CNMI */
int32_t ril_response_new_message_indication(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_new_message_indication_rsp_t response;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;

    cur_pos = cmd_buf;
    channel_p = ril_get_channel(channel_id);
    rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.mode);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.mt);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.bm);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.ds);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.bfr);
            if (ret < 0) {
                break;
            }
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;
}


// pdu mode
/* AT+CMGL */
int32_t ril_response_list_messages_pdu(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line, *line2;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_list_messages_pdu_rsp_t response;
    ril_message_entry_pdu_desc_t *entry, *list_head;

    int32_t message_num = 0;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;

    list_head = NULL;
    cur_pos = cmd_buf;
    channel_p = ril_get_channel(channel_id);
    rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            if (message_num == 0) {
                list_head = (ril_message_entry_pdu_desc_t *)ril_mem_malloc(sizeof(ril_message_entry_pdu_desc_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_message_entry_pdu_desc_t) * (message_num + 1));
                entry = list_head + message_num;
            }
            if (list_head == NULL) {
                ret = -2;
                break;
            }

            line2 = at_get_one_line2(&line);
            /* information response */
            ret = at_tok_start(&line2);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line2, &entry->index);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line2, &entry->stat);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line2, &entry->alpha);
            if (ret < 0) {
                entry->alpha = RIL_OMITTED_STRING_PARAM;
            }

            ret = at_tok_nextint(&line2, &entry->length);
            if (ret < 0) {
                break;
            }

            /* pdu content */
            line2 = at_get_one_line2(&line);
            ret = at_tok_nextstr(&line2, &entry->pdu);
            if (ret < 0) {
                break;
            }

            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    response.entry_list = list_head;
    response.message_num = message_num;

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}


/* AT+CMGR */
int32_t ril_response_read_message_pdu(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line, *line2;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_read_message_pdu_rsp_t response;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;

    cur_pos = cmd_buf;
    channel_p = ril_get_channel(channel_id);
    rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */

            line2 = at_get_one_line2(&line);
            ret = at_tok_start(&line2);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line2, &response.stat);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line2, &response.alpha);
            if (ret < 0) {
                response.alpha = RIL_OMITTED_INTEGER_PARAM;
            }

            ret = at_tok_nextint(&line2, &response.length);
            if (ret < 0) {
                break;
            }

            line2 = at_get_one_line2(&line);

            ret = at_tok_nextstr(&line2, &response.pdu);
            if (ret < 0) {
                break;
            }
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;

}


/* AT+CMGS */
int32_t ril_response_send_message_pdu(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_send_message_pdu_rsp_t response;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;

    cur_pos = cmd_buf;
    channel_p = ril_get_channel(channel_id);
    rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.mr);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &response.ackpdu);
            if (ret < 0) {
                response.ackpdu = RIL_OMITTED_STRING_PARAM;
                ret = 0;
            }
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    RIL_LOGI("SMS PDU response hdlr, res_code: %d", res_code);

    if (res_code == RIL_RESULT_CODE_INTERMEDIATE) {
        /* send subsequent pdu raw data */   
        int32_t written;
        bool write_success = false;
        char *round2_str = channel_p->tx_buf_ptr + channel_p->tx_pos;
        int32_t round2_len = strlen(round2_str);
        uint8_t ctrl_z = 0x1A;

        RIL_LOGCT("cmux channel %d, send command data: %s\r\n", channel_id, round2_str);
        written = mux_ap_send_data(channel_p->channel_id, (uint8_t *)round2_str, round2_len, NULL);
        if (written == MUX_AP_STATUS_OK) {
            write_success = true;
        }

        /* send ctrl+z to terminate input */
        if (write_success == true) {
            RIL_LOGCT("cmux channel %d, send command data: %c\r\n", channel_id, ctrl_z);
            written = mux_ap_send_data(channel_p->channel_id, (uint8_t *)&ctrl_z, 1, NULL);
            write_success = written == 1 ? true : false;
        }
        
        RIL_LOGCT("send command data done, ret: %d", write_success == true ? 0 : -1);
        return write_success == true ? -1 : 0;
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;
}


/* AT+CMGW */
int32_t ril_response_write_message_pdu(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_write_message_pdu_rsp_t response;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;

    cur_pos = cmd_buf;
    channel_p = ril_get_channel(channel_id);
    rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.index);
            if (ret < 0) {
                break;
            }
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;
}


/* AT+CMGD */
int32_t ril_response_delete_message(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT+CMGC */
int32_t ril_response_send_command_pdu(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_send_command_pdu_rsp_t response;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;

    cur_pos = cmd_buf;
    channel_p = ril_get_channel(channel_id);
    rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.mr);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &response.ackpdu);
            if (ret < 0) {
                response.ackpdu = RIL_OMITTED_STRING_PARAM;
                ret = 0;
            }
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;
}


/* AT+CNMA */
int32_t ril_response_new_message_ack_pdu(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT+CMSS */
int32_t ril_response_send_message_from_storage_pdu(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    /* the same as CMGS response hdlr */
    return ril_response_send_message_pdu(channel_id, cmd_buf, cmd_buf_len);
}


// text mode
/* AT+CMGL */
void ril_initialize_list_message_entry_txt(ril_message_entry_txt_desc_t *entry)
{
    entry->index = RIL_OMITTED_INTEGER_PARAM;
    entry->stat = RIL_OMITTED_STRING_PARAM;
    entry->str_addr.oa = RIL_OMITTED_STRING_PARAM;
    entry->scts = RIL_OMITTED_STRING_PARAM;
    entry->int_addr.tooa = RIL_OMITTED_INTEGER_PARAM;
    entry->length = RIL_OMITTED_INTEGER_PARAM;
    entry->data = RIL_OMITTED_STRING_PARAM;
    entry->fo = RIL_OMITTED_INTEGER_PARAM;
    entry->mr = RIL_OMITTED_INTEGER_PARAM;
    entry->ra = RIL_OMITTED_STRING_PARAM;
    entry->tora = RIL_OMITTED_INTEGER_PARAM;
    entry->dt = RIL_OMITTED_STRING_PARAM;
    entry->st = RIL_OMITTED_INTEGER_PARAM;
    entry->ct = RIL_OMITTED_INTEGER_PARAM;
    entry->sn = RIL_OMITTED_INTEGER_PARAM;
    entry->mid = RIL_OMITTED_INTEGER_PARAM;
    entry->page = RIL_OMITTED_INTEGER_PARAM;
    entry->pages = RIL_OMITTED_INTEGER_PARAM;
}


int32_t ril_response_list_messages_txt(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line, *line2;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_list_messages_txt_rsp_t response;
    ril_message_entry_txt_desc_t *entry, *list_head;

    int32_t message_num = 0;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;

    list_head = NULL;
    cur_pos = cmd_buf;
    channel_p = ril_get_channel(channel_id);
    rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            ril_param_type_t param_type;
            if (message_num == 0) {
                list_head = (ril_message_entry_txt_desc_t *)ril_mem_malloc(sizeof(ril_message_entry_txt_desc_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_message_entry_txt_desc_t) * (message_num + 1));
                entry = list_head + message_num;
            }
            if (list_head == NULL) {
                ret = -2;
                break;
            }

            ril_initialize_list_message_entry_txt(entry);
            /* information response */
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->index);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->stat);
            if (ret < 0) {
                break;
            }

            param_type = at_tok_next_param_type(line);
            if (param_type == RIL_PARAM_TYPE_STRING) {
                /* SMS-SUBMIT or SMS-DELIVER */
                RIL_LOGI("this is SMS-SUBMIT or SMS-DELIVER\r\n");
                line2 = at_get_one_line2(&line);
                ret = at_tok_nextstr(&line2, &entry->str_addr.oa);
                if (ret < 0) {
                    break;
                }

                do {
                    ret = at_tok_nextstr(&line2, &entry->alpha);
                    if (ret < 0) {
                        entry->alpha = RIL_OMITTED_STRING_PARAM;
                    }

                    ret = at_tok_nextstr(&line2, &entry->scts);
                    if (ret < 0) {
                        entry->scts = RIL_OMITTED_STRING_PARAM;
                    }

                    ret = at_tok_nextint(&line2, &entry->int_addr.tooa);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line2, &entry->length);
                    if (ret < 0) {
                        break;
                    }
                } while (0);

                line2 = at_get_one_line2(&line);
                at_tok_nextstr(&line2, &entry->data);
            } else if (param_type == RIL_PARAM_TYPE_INTEGER) {
                int32_t param_num;
                line2 = at_get_one_line2(&line);
                param_num = at_tok_param_num(line2);
                if (param_num >= 6) {
                    /* SMS-STATUS-REPORT */
                    RIL_LOGI("this is SMS-STATUS-REPORT\r\n");
                    ret = at_tok_nextint(&line2, &entry->fo);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line2, &entry->mr);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextstr(&line2, &entry->ra);

                    ret = at_tok_nextint(&line2, &entry->tora);

                    ret = at_tok_nextstr(&line2, &entry->scts);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextstr(&line2, &entry->dt);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line2, &entry->st);
                    if (ret < 0) {
                        break;
                    }
                } else if (param_num >= 4) {
                    /* CBM storage */
                    RIL_LOGI("this is CBM storage\r\n");
                    ret = at_tok_nextint(&line2, &entry->sn);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line2, &entry->mid);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line2, &entry->page);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line2, &entry->pages);
                    if (ret < 0) {
                        break;
                    }

                    line2 = at_get_one_line2(&line);
                    at_tok_nextstr(&line2, &entry->data);
                } else {
                    /* SMS-COMMAND */
                    RIL_LOGI("this is SMS-COMMAND\r\n");
                    ret = at_tok_nextint(&line2, &entry->fo);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line2, &entry->ct);
                    if (ret < 0) {
                        break;
                    }
                }

            } else {
                ret = -1;
                RIL_LOGE("invalid response format\r\n");
                break;
            }

            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    response.entry_list = list_head;
    response.message_num = message_num;
    
    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}


/* AT+CMGR */
void ril_initialize_read_message_entry_txt(ril_read_message_txt_rsp_t *entry)
{
    entry->stat = RIL_OMITTED_STRING_PARAM;
    entry->oa = RIL_OMITTED_STRING_PARAM;
    entry->alpha = RIL_OMITTED_STRING_PARAM;
    entry->scts = RIL_OMITTED_STRING_PARAM;
    entry->tooa = RIL_OMITTED_INTEGER_PARAM;
    entry->fo = RIL_OMITTED_INTEGER_PARAM;
    entry->pid = RIL_OMITTED_INTEGER_PARAM;
    entry->dcs = RIL_OMITTED_INTEGER_PARAM;
    entry->sca = RIL_OMITTED_STRING_PARAM;
    entry->tosca = RIL_OMITTED_INTEGER_PARAM;
    entry->length = RIL_OMITTED_INTEGER_PARAM;
    entry->data = RIL_OMITTED_STRING_PARAM;
    entry->da = RIL_OMITTED_STRING_PARAM;
    entry->toda = RIL_OMITTED_INTEGER_PARAM;
    entry->vp = RIL_OMITTED_INTEGER_PARAM;
    entry->mr = RIL_OMITTED_INTEGER_PARAM;
    entry->ra = RIL_OMITTED_STRING_PARAM;
    entry->tora = RIL_OMITTED_INTEGER_PARAM;
    entry->dt = RIL_OMITTED_STRING_PARAM;
    entry->st = RIL_OMITTED_INTEGER_PARAM;
    entry->ct = RIL_OMITTED_INTEGER_PARAM;
    entry->mn = RIL_OMITTED_INTEGER_PARAM;
    entry->cdata = RIL_OMITTED_STRING_PARAM;
    entry->sn = RIL_OMITTED_INTEGER_PARAM;
    entry->mid = RIL_OMITTED_INTEGER_PARAM;
    entry->page = RIL_OMITTED_INTEGER_PARAM;
    entry->pages = RIL_OMITTED_INTEGER_PARAM;
}


int32_t ril_response_read_message_txt(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line, *line2;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_read_message_txt_rsp_t response;
    ril_param_type_t param_type;
    int32_t param_num;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;

    cur_pos = cmd_buf;
    channel_p = ril_get_channel(channel_id);
    rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    ril_initialize_read_message_entry_txt(&response);
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &response.stat);
            if (ret < 0) {
                break;
            }

            param_type = at_tok_next_param_type(line);
            if (param_type == RIL_PARAM_TYPE_STRING) {
                line2 = at_get_one_line2(&line);
                ret = at_tok_nextstr(&line2, &response.oa);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextstr(&line2, &response.alpha);
                if (ret < 0) {
                    response.alpha = RIL_OMITTED_STRING_PARAM;
                }

                param_type = at_tok_next_param_type(line2);
                if (param_type == RIL_PARAM_TYPE_STRING) {
                    /* SMS-DELIVER */
                    RIL_LOGI("this is SMS-DELIVER\r\n");
                    ret = at_tok_nextstr(&line2, &response.scts);
                    if (ret < 0 ) {
                        break;
                    }

                    do {
                        ret = at_tok_nextint(&line2, &response.tooa);
                        if (ret < 0) {
                            break;
                        }

                        ret = at_tok_nextint(&line2, &response.fo);
                        if (ret < 0) {
                            break;
                        }

                        ret = at_tok_nextint(&line2, &response.pid);
                        if (ret < 0) {
                            break;
                        }

                        ret = at_tok_nextint(&line2, &response.dcs);
                        if (ret < 0) {
                            break;
                        }

                        ret = at_tok_nextstr(&line2, &response.sca);
                        if (ret < 0) {
                            break;
                        }

                        ret = at_tok_nextint(&line2, &response.tosca);
                        if (ret < 0) {
                            break;
                        }

                        ret = at_tok_nextint(&line2, &response.length);
                        if (ret < 0) {
                            break;
                        }
                    } while (0);
                    ret = 0;
                    line2 = at_get_one_line2(&line);
                    at_tok_nextstr(&line2, &response.data);
                } else if (param_type == RIL_PARAM_TYPE_INTEGER || param_type == RIL_PARAM_TYPE_NONE ) {
                    /* SMS-SUBMIT */
                    RIL_LOGI("this is SMS-SUBMIT\r\n");
                    response.da = response.oa;
                    response.oa = RIL_OMITTED_STRING_PARAM;

                    do {
                        ret = at_tok_nextint(&line2, &response.toda);
                        if (ret < 0) {
                            break;
                        }

                        ret = at_tok_nextint(&line2, &response.fo);
                        if (ret < 0) {
                            break;
                        }

                        ret = at_tok_nextint(&line2, &response.pid);
                        if (ret < 0) {
                            break;
                        }

                        ret = at_tok_nextint(&line2, &response.dcs);
                        if (ret < 0) {
                            break;
                        }

                        ret = at_tok_nextint(&line2, &response.vp);
                        if (ret < 0) {
                            response.vp = RIL_OMITTED_INTEGER_PARAM;
                        }

                        ret = at_tok_nextstr(&line2, &response.sca);
                        if (ret < 0) {
                            break;
                        }

                        ret = at_tok_nextint(&line2, &response.tosca);
                        if (ret < 0) {
                            break;
                        }

                        ret = at_tok_nextint(&line2, &response.length);
                        if (ret < 0) {
                            break;
                        }
                    } while (0);
                    ret = 0;
                    line2 = at_get_one_line2(&line);
                    at_tok_nextstr(&line2, &response.data);
                } else {
                    RIL_LOGE("invalid response format\r\n");
                    ret = -1;
                    break;
                }
            } else if (param_type == RIL_PARAM_TYPE_INTEGER) {
                param_num = at_tok_param_num(line);
                if (param_num == 2) {
                    /* SMS-COMMAND */
                    RIL_LOGI("this is SMS-COMMAND short\r\n");
                    ret = at_tok_nextint(&line, &response.fo);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line, &response.ct);
                    if (ret < 0) {
                        break;
                    }
                } else {
                    char *delim = NULL;
                    delim = strstr(line, "\r\n");
                    if (delim == NULL) {
                        /* SMS-STATUS-REPORT */
                        RIL_LOGI("this is SMS-STATUS_REPORT\r\n");
                        ret = at_tok_nextint(&line, &response.fo);
                        if (ret < 0) {
                            break;
                        }

                        ret = at_tok_nextint(&line, &response.mr);
                        if (ret < 0) {
                            break;
                        }

                        ret = at_tok_nextstr(&line, &response.ra);
                        if (ret < 0) {
                            response.ra = RIL_OMITTED_STRING_PARAM;
                        }

                        ret = at_tok_nextint(&line, &response.tora);
                        if (ret < 0) {
                            response.tora = RIL_OMITTED_INTEGER_PARAM;
                        }

                        ret = at_tok_nextstr(&line, &response.scts);
                        if (ret < 0) {
                            break;
                        }

                        ret = at_tok_nextstr(&line, &response.dt);
                        if (ret < 0) {
                            break;
                        }

                        ret = at_tok_nextint(&line, &response.st);
                        if (ret < 0) {
                            break;
                        }

                    } else {
                        line2 = at_get_one_line2(&line);

                        param_num = at_tok_param_num(line2);
                        if (param_num == 5) {
                            /* CBM storage */
                            RIL_LOGI("this is CBM storage\r\n");
                            ret = at_tok_nextint(&line2, &response.sn);
                            if (ret < 0) {
                                break;
                            }

                            ret = at_tok_nextint(&line2, &response.mid);
                            if (ret < 0) {
                                break;
                            }

                            ret = at_tok_nextint(&line2, &response.dcs);
                            if (ret < 0) {
                                break;
                            }

                            ret = at_tok_nextint(&line2, &response.page);
                            if (ret < 0) {
                                break;
                            }

                            ret = at_tok_nextint(&line2, &response.pages);
                            if (ret < 0) {
                                break;
                            }

                            line2 = at_get_one_line2(&line);
                            at_tok_nextstr(&line2, &response.data);
                        } else if (param_num == 2 || param_num == 7) {
                            /* SMS-COMMAND */
                            RIL_LOGI("this is SMS-COMMAND\r\n");
                            ret = at_tok_nextint(&line2, &response.fo);
                            if (ret < 0) {
                                break;
                            }

                            ret = at_tok_nextint(&line2, &response.ct);
                            if (ret < 0) {
                                break;
                            }

                            ret = at_tok_nextint(&line2, &response.pid);
                            if (ret < 0) {
                                response.pid = RIL_OMITTED_INTEGER_PARAM;
                            }

                            ret = at_tok_nextint(&line2, &response.mn);
                            if (ret < 0) {
                                response.mn = RIL_OMITTED_INTEGER_PARAM;
                            }

                            ret = at_tok_nextstr(&line2, &response.da);
                            if (ret < 0) {
                                response.da = RIL_OMITTED_STRING_PARAM;
                            }

                            ret = at_tok_nextint(&line2, &response.toda);
                            if (ret < 0) {
                                response.toda = RIL_OMITTED_INTEGER_PARAM;
                            }

                            ret = at_tok_nextint(&line2, &response.length);
                            if (ret < 0) {
                                break;
                            }

                            line2 = at_get_one_line2(&line);
                            at_tok_nextstr(&line2, &response.cdata);
                        } else {
                            RIL_LOGE("invalid response format\r\n");
                            ret = 1;
                            break;
                        }
                    }
                }
            } else {
                RIL_LOGE("invalid response format\r\n");
                ret = -1;
                break;
            }

        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;

}


/* AT+CMGS */
int32_t ril_response_send_message_txt(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_send_message_txt_rsp_t response;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;

    cur_pos = cmd_buf;
    channel_p = ril_get_channel(channel_id);
    rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.mr);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &response.scts);
            if (ret < 0) {
                response.scts = RIL_OMITTED_STRING_PARAM;
                ret = 0;
            }
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;
}


/* AT+CMGW */
int32_t ril_response_write_message_txt(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_write_message_txt_rsp_t response;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;

    cur_pos = cmd_buf;
    channel_p = ril_get_channel(channel_id);
    rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.index);
            if (ret < 0) {
                break;
            }
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;
}


/* AT+CMGC */
int32_t ril_response_send_command_txt(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_send_command_txt_rsp_t response;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;

    cur_pos = cmd_buf;
    channel_p = ril_get_channel(channel_id);
    rsp_cb = (ril_cmd_response_callback_t)channel_p->usr_rsp_cb;

    if (channel_p->curr_request_mode == RIL_TEST_MODE) {
        need_parse = false;
        res_code = RIL_RESULT_CODE_OK;
    } else {
        line = at_get_one_line(&cur_pos);
    }

    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.mr);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &response.scts);
            if (ret < 0) {
                response.scts = RIL_OMITTED_STRING_PARAM;
                ret = 0;
            }
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    return 0;
}


/* AT+CNMA */
int32_t ril_response_new_message_ack_txt(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_new_message_ack_pdu(channel_id, cmd_buf, cmd_buf_len);
}


/* AT+CMSS */
int32_t ril_response_send_message_from_storage_txt(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    /* the same as CMGS response hdlr */
    return ril_response_send_message_txt(channel_id, cmd_buf, cmd_buf_len);
}



/*************************************************
 *                 URC handler
 *************************************************/
/* URC: +CMTI */
int32_t ril_urc_new_message_indication_txt(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_new_message_indication_txt_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextstr(&line, &response.mem);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.index);
        if (ret < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(RIL_URC_ID_CMTI, &response, sizeof(response));
    }
    return 0;
}


/* URC: +CMT */
int32_t ril_urc_new_message_indication_pdu(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line, *line2;
    ril_new_message_indication_pdu_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        line2 = at_get_one_line2(&line);
        ret = at_tok_nextint(&line2, &response.length);
        if (ret < 0) {
            break;
        }

        line2 = at_get_one_line2(&line);
        ret = at_tok_nextstr(&line2, &response.pdu);
        if (ret < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(RIL_URC_ID_CMT, &response, sizeof(response));
    }
    return 0;
}




/*************************************************
 *                 UT test case
 *************************************************/
#if defined(__RIL_UT_TEST_CASES__)
int32_t ril_response_ut_callback_27005(ril_cmd_response_t *cmd_response)
{
    RIL_LOGUT("final result code: %d\r\n", cmd_response->res_code);
    RIL_LOGUT("request mode: %d\r\n", cmd_response->mode);
    if (cmd_response->mode == RIL_TEST_MODE &&
        cmd_response->test_mode_str != NULL &&
        cmd_response->test_mode_str_len > 0) {
        RIL_LOGDUMPSTRUT("test mode str: %s\r\n", cmd_response->test_mode_str_len, cmd_response->test_mode_str);
    } else {
        switch (cmd_response->cmd_id) {
            case RIL_CMD_ID_CSMS: {
                ril_select_message_service_rsp_t *param = (ril_select_message_service_rsp_t *)cmd_response->cmd_param;
                if ((cmd_response->mode == RIL_EXECUTE_MODE || cmd_response->mode == RIL_READ_MODE) && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("service: %d\r\n", (int)param->service);
                    RIL_LOGUT("mt: %d\r\n", (int)param->mt);
                    RIL_LOGUT("mo: %d\r\n", (int)param->mo);
                    RIL_LOGUT("bm: %d\r\n", (int)param->bm);
                }
            }
            break;

            case RIL_CMD_ID_CPMS: {
                ril_preferred_message_storage_rsp_t *param = (ril_preferred_message_storage_rsp_t *)cmd_response->cmd_param;
                if ((cmd_response->mode == RIL_EXECUTE_MODE || cmd_response->mode == RIL_READ_MODE) && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("mem1: %s\r\n", param->mem1 ? param->mem1 : UT_OMITTED_PARAM);
                    RIL_LOGUT("used1: %d\r\n", (int)param->used1);
                    RIL_LOGUT("total1: %d\r\n", (int)param->total1);
                    RIL_LOGUT("mem2: %s\r\n", param->mem2 ? param->mem2 : UT_OMITTED_PARAM);
                    RIL_LOGUT("used2: %d\r\n", (int)param->used2);
                    RIL_LOGUT("total2: %d\r\n", (int)param->total2);
                    RIL_LOGUT("mem3: %s\r\n", param->mem3 ? param->mem3 : UT_OMITTED_PARAM);
                    RIL_LOGUT("used3: %d\r\n", (int)param->used3);
                    RIL_LOGUT("total3: %d\r\n", (int)param->total3);
                }
            }
            break;

            case RIL_CMD_ID_CMGF: {
                ril_message_format_rsp_t *param = (ril_message_format_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("message_mode: %d\r\n", (int)param->message_mode);
                }
            }
            break;

            case RIL_CMD_ID_CSCA: {
                ril_service_centre_address_rsp_t *param = (ril_service_centre_address_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("sca: %s\r\n", param->sca ? param->sca : UT_OMITTED_PARAM);
                    RIL_LOGUT("tosca: %d\r\n", (int)param->tosca);
                }
            }
            break;

            case RIL_CMD_ID_CSMP: {
                ril_set_text_mode_parameters_rsp_t *param = (ril_set_text_mode_parameters_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("fo: %d\r\n", (int)param->fo);
                    RIL_LOGUT("vp: %d\r\n", (int)param->vp);
                    RIL_LOGUT("pid: %d\r\n", (int)param->pid);
                    RIL_LOGUT("dcs: %d\r\n", (int)param->dcs);
                }
            }
            break;

            case RIL_CMD_ID_CSDH: {
                ril_show_text_mode_parameters_rsp_t *param = (ril_show_text_mode_parameters_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("show: %d\r\n", (int)param->show);
                }
            }
            break;

            case RIL_CMD_ID_CSAS: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_CRES: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_CNMI: {
                ril_new_message_indication_rsp_t *param = (ril_new_message_indication_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("mode: %d\r\n", (int)param->mode);
                    RIL_LOGUT("mt: %d\r\n", (int)param->mt);
                    RIL_LOGUT("bm: %d\r\n", (int)param->bm);
                    RIL_LOGUT("ds: %d\r\n", (int)param->ds);
                    RIL_LOGUT("bfr: %d\r\n", (int)param->bfr);
                }
            }
            break;

            case RIL_CMD_ID_CMGL_PDU: {
                ril_list_messages_pdu_rsp_t *param = (ril_list_messages_pdu_rsp_t *)cmd_response->cmd_param;
                if (param != NULL && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    int32_t i;
                    ril_message_entry_pdu_desc_t *entry;
                    RIL_LOGUT("=====param dump=====\r\n");
                    for (i = 0; i < param->message_num; i++) {
                        RIL_LOGUT("No. %d message entry:\r\n", (int)(i + 1));
                        entry = &param->entry_list[i];
                        RIL_LOGUT("index: %d\r\n", (int)entry->index);
                        RIL_LOGUT("stat: %d\r\n", (int)entry->stat);
                        RIL_LOGUT("alpha: %s\r\n", entry->alpha);
                        RIL_LOGUT("length: %d\r\n", (int)entry->length);
                        RIL_LOGUT("pdu: %s\r\n", entry->pdu);
                        RIL_LOGUT("\r\n");
                    }
                }
            }
            break;

            case RIL_CMD_ID_CMGR_PDU: {
                ril_read_message_pdu_rsp_t *param = (ril_read_message_pdu_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("stat: %d\r\n", (int)param->stat);
                    RIL_LOGUT("alpha: %d\r\n", (int)param->alpha);
                    RIL_LOGUT("length %d\r\n", (int)param->length);
                    RIL_LOGUT("pdu: %s\r\n", param->pdu ? param->pdu : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CMGS_PDU: {
                ril_send_message_pdu_rsp_t *param = (ril_send_message_pdu_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("mr: %d\r\n", (int)param->mr);
                    RIL_LOGUT("ackpdu: %s\r\n", param->ackpdu ? param->ackpdu : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CMGW_PDU: {
                ril_write_message_pdu_rsp_t *param = (ril_write_message_pdu_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("index: %d\r\n", (int)param->index);
                }   
            }
            break;

            case RIL_CMD_ID_CMGD: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_CMGC_PDU: {
                ril_send_command_pdu_rsp_t *param = (ril_send_command_pdu_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("mr: %d\r\n", (int)param->mr);
                    RIL_LOGUT("ackpdu: %s\r\n", param->ackpdu ? param->ackpdu : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CNMA_PDU: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_CMSS_PDU: {
                ril_send_message_from_storage_pdu_rsp_t *param = (ril_send_message_from_storage_pdu_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("mr: %d\r\n", (int)param->mr);
                    RIL_LOGUT("ackpdu: %s\r\n", param->ackpdu ? param->ackpdu : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CMGL_TXT: {  
                ril_list_messages_txt_rsp_t *param = (ril_list_messages_txt_rsp_t *)cmd_response->cmd_param;    
                if (param != NULL && cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                        int32_t i;
                        ril_message_entry_txt_desc_t *entry;
                        RIL_LOGUT("=====param dump=====\r\n");
                        for (i = 0; i < param->message_num; i++) {
                            RIL_LOGUT("No. %d message entry:\r\n", (int)(i + 1));
                            entry = &param->entry_list[i];
                            RIL_LOGUT("index: %d\r\n", (int)entry->index);
                            RIL_LOGUT("stat: %s\r\n", entry->stat);
                            RIL_LOGUT("oa/da: %s\r\n", entry->str_addr.oa);
                            RIL_LOGUT("alpha: %s\r\n", entry->alpha);
                            RIL_LOGUT("scts: %s\r\n", entry->scts);
                            RIL_LOGUT("tooa/toda: %d\r\n", (int)entry->int_addr.tooa);
                            RIL_LOGUT("length: %d\r\n", (int)entry->length);
                            RIL_LOGUT("data: %s\r\n", entry->data);
                            RIL_LOGUT("fo: %d\r\n", (int)entry->fo);
                            RIL_LOGUT("mr: %d\r\n", (int)entry->mr);
                            RIL_LOGUT("ra: %s\r\n", entry->ra);
                            RIL_LOGUT("tora: %d\r\n", (int)entry->tora);
                            RIL_LOGUT("dt: %s\r\n", entry->dt);
                            RIL_LOGUT("st: %d\r\n", (int)entry->st);
                            RIL_LOGUT("ct: %d\r\n", (int)entry->ct);
                            RIL_LOGUT("sn: %d\r\n", (int)entry->sn);
                            RIL_LOGUT("mid: %d\r\n", (int)entry->mid);
                            RIL_LOGUT("page: %d\r\n", (int)entry->page);
                            RIL_LOGUT("pages: %d\r\n", (int)entry->pages);
                            RIL_LOGUT("\r\n");
                        }
                    }
            }
            break;

            case RIL_CMD_ID_CMGR_TXT: {      
                ril_read_message_txt_rsp_t *param = (ril_read_message_txt_rsp_t *)cmd_response->cmd_param;
                if (param != NULL && cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("=====param dump=====\r\n");
                    RIL_LOGUT("stat: %s\r\n", param->stat ? param->stat : UT_OMITTED_PARAM);
                    RIL_LOGUT("oa: %s\r\n", param->oa ? param->oa : UT_OMITTED_PARAM);
                    RIL_LOGUT("alpha: %s\r\n", param->alpha ? param->alpha : UT_OMITTED_PARAM);
                    RIL_LOGUT("tooa: %d\r\n", (int)param->tooa);
                    RIL_LOGUT("fo: %d\r\n", (int)param->fo);
                    RIL_LOGUT("pid: %d\r\n", (int)param->pid);
                    RIL_LOGUT("dcs: %d\r\n", (int)param->dcs);
                    RIL_LOGUT("sca: %s\r\n", param->sca ? param->sca : UT_OMITTED_PARAM);
                    RIL_LOGUT("tosca: %d\r\n", (int)param->tosca);
                    RIL_LOGUT("data: %s\r\n", param->data ? param->data : UT_OMITTED_PARAM);
                    RIL_LOGUT("da: %s\r\n", param->da ? param->da : UT_OMITTED_PARAM);
                    RIL_LOGUT("toda: %d\r\n", (int)param->toda);
                    RIL_LOGUT("vp: %d\r\n", (int)param->vp);
                    RIL_LOGUT("mr: %d\r\n", (int)param->mr);
                    RIL_LOGUT("ra: %s\r\n", param->ra ? param->ra : UT_OMITTED_PARAM);
                    RIL_LOGUT("tora: %d\r\n", (int)param->tora);
                    RIL_LOGUT("dt: %s\r\n", param->dt ? param->dt : UT_OMITTED_PARAM);
                    RIL_LOGUT("st: %d\r\n", (int)param->st);
                    RIL_LOGUT("ct: %d\r\n", (int)param->ct);
                    RIL_LOGUT("mn: %d\r\n", (int)param->mn);
                    RIL_LOGUT("cdata: %s\r\n", param->cdata ? param->cdata : UT_OMITTED_PARAM);
                    RIL_LOGUT("sn: %d\r\n", (int)param->sn);
                    RIL_LOGUT("mid: %d\r\n", (int)param->mid);
                    RIL_LOGUT("page: %d\r\n", (int)param->page);
                    RIL_LOGUT("pages: %d\r\n", (int)param->pages);
                    RIL_LOGUT("\r\n");
                }
            }
            break;

            case RIL_CMD_ID_CMGS_TXT: {
                ril_send_message_txt_rsp_t *param = (ril_send_message_txt_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("mr: %d\r\n", (int)param->mr);
                    RIL_LOGUT("scts: %s\r\n", param->scts ? param->scts : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CMGW_TXT: {
                ril_write_message_txt_rsp_t *param = (ril_write_message_txt_rsp_t *)cmd_response->cmd_param;
                if ((cmd_response->mode == RIL_EXECUTE_MODE || cmd_response->mode == RIL_ACTIVE_MODE) && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("index: %d\r\n", (int)param->index);
                }
            }
            break;

            case RIL_CMD_ID_CMGC_TXT: {           
                ril_send_command_txt_rsp_t *param = (ril_send_command_txt_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("mr: %d\r\n", (int)param->mr);
                    RIL_LOGUT("scts %s\r\n", param->scts ? param->scts : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CNMA_TXT: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_CMSS_TXT: {
                ril_send_command_txt_rsp_t *param = (ril_send_command_txt_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("mr: %d\r\n", (int)param->mr);
                    RIL_LOGUT("scts %s\r\n", param->scts ? param->scts : UT_OMITTED_PARAM);
                }
            }
            break;
            
            default:
                break;
        }
    }
    return 0;
}
#endif /* __RIL_UT_TEST_CASES__ */
#endif /* __RIL_SMS_COMMAND_SUPPORT__ */

