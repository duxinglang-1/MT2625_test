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
#include "ril_cmds_common.h"
#include "ril_cmds_proprietary.h"

/* AT cmd: AT*MLTS */
ril_status_t ril_request_get_local_timestamp_and_network_info(ril_request_mode_t mode,
        int32_t enable,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, enable, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MLTS, list_head, (void *)callback, user_data);
    }

}


/* AT*MSIMINS */
ril_status_t ril_request_sim_inserted_status_reporting(ril_request_mode_t mode,
        int32_t n,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, n, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MSIMINS, list_head, (void *)callback, user_data);
    }
}


/* AT*MSPN */
ril_status_t ril_request_get_service_provider_name_from_sim(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_MSPN, NULL, (void *)callback, user_data);
}


/* AT*MUNSOL */
ril_status_t ril_request_extra_unsolicited_indications(ril_request_mode_t mode,
        char *ind,
        int32_t op_mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_string_optional(list_head, ind, 2)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, op_mode, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MUNSOL, list_head, (void *)callback, user_data);
    }
}


/* AT*MGCOUNT */
ril_status_t ril_request_packet_domain_counters(ril_request_mode_t mode,
        int32_t action,
        int32_t cid,
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

            /* action, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, action, 2)) < 0) {
                break;
            }

            /* cid, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, cid, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MGCOUNT, list_head, (void *)callback, user_data);
    }
}


#ifndef __RIL_CMD_SET_SLIM_ENABLE__
/* AT*MGSINK */
ril_status_t ril_request_send_packet_to_discard(ril_request_mode_t mode,
        ril_send_packet_to_discard_param_t *param,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, param->nsapi, 1)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, param->pcksize, 1)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, param->pckcount, 1)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_string_optional(list_head, param->address, 1)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, param->port, 1)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MGSINK, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MCGDEFCONT */
ril_status_t ril_request_pdn_connection_set_default_psd_attach(ril_request_mode_t mode,
        ril_send_pdn_connection_set_default_psd_attach_req_t *config,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, config->pdp_type, 0)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_string_optional(list_head, config->apn, 0)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_string_optional(list_head, config->username, 0)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_string_optional(list_head, config->password, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MCGDEFCONT, list_head, (void *)callback, user_data);
    }
}



/* AT cmd: AT*MGTCSINK */
ril_status_t ril_request_send_tcpip_packet_to_discard(ril_request_mode_t mode,
        ril_send_send_tcpip_packet_to_discard_req_t *config,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, config->nsapi, 1)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, config->pcktsize, 1)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, config->pcktcount, 1)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, config->address, 1)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, config->port, 1)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MGTCSINK, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MSACL */
ril_status_t ril_request_control_ACL_feature(ril_request_mode_t mode,
        int32_t acl_mode,
        char *pin2,
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

            /* acl_mode, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, acl_mode, 2)) < 0) {
                break;
            }

            /* pin2, optional */
            if ((ret = ril_param_list_add_string_optional(list_head, pin2, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MSACL, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MLACL */
ril_status_t ril_request_display_ACL_list(ril_request_mode_t mode,
        int32_t from,
        int32_t to,
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

            /* from, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, from, 2)) < 0) {
                break;
            }

            /* to, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, to, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MLACL, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MWACL */
ril_status_t ril_request_write_ACL_entry(ril_request_mode_t mode,
        int32_t index,
        char *APN,
        char *pin2,
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

            /* APN, necessary */
            if ((ret = ril_param_list_add_string_optional(list_head, APN, 2)) < 0) {
                break;
            }

            /* pin2, optional */
            if ((ret = ril_param_list_add_string_optional(list_head, pin2, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MWACL, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MDACL */
ril_status_t ril_request_delete_ACL_entry(ril_request_mode_t mode,
        int32_t index,
        char *pin2,
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

            /* pin2, optional */
            if ((ret = ril_param_list_add_string_optional(list_head, pin2, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MDACL, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MSMEXTRAINFO */
ril_status_t ril_request_control_extra_info_on_sms(ril_request_mode_t mode,
        int32_t enable,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, enable, 2)) < 0) {
                break;
            }

        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MSMEXTRAINFO, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MSMEXTRAUNSOL */
ril_status_t ril_request_control_extra_unsolicited_messages(ril_request_mode_t mode,
        int32_t enable,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, enable, 2)) < 0) {
                break;
            }

        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MSMEXTRAUNSOL, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MSMSTATUS */
ril_status_t ril_request_obtain_status_of_sms_functionality(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_MSMSTATUS, NULL, (void *)callback, user_data);
}


/* AT cmd: AT*MMGI */
ril_status_t ril_request_control_sms_unsolicited_indication(ril_request_mode_t mode,
        int32_t op_mode,
        int32_t event_id,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, op_mode, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, event_id, 0)) < 0) {
                break;
            }

        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MMGI, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MMGRW */
ril_status_t ril_request_sms_location_rewrite(ril_request_mode_t mode,
        int32_t loc,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, loc, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MMGRW, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MMGSC */
ril_status_t ril_request_sms_location_status_change_txt(ril_request_mode_t mode,
        char *status,
        int32_t loc,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, status, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, loc, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MMGSC_TXT, list_head, (void *)callback, user_data);
    }
}

ril_status_t ril_request_sms_location_status_change_pdu(ril_request_mode_t mode,
        int32_t status,
        int32_t loc,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, status, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, loc, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MMGSC_PDU, list_head, (void *)callback, user_data);
    }
}
#endif /* __RIL_CMD_SET_SLIM_ENABLE__ */


/* AT cmd: AT*MUPIN */
ril_status_t ril_request_uicc_pin_information_access(ril_request_mode_t mode,
        ril_uicc_pin_information_access_req_t *req_param,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if (req_param == NULL) {
                return RIL_STATUS_INVALID_PARAM;
            }
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, req_param->mode, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, req_param->access_level, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, req_param->pin_or_puk, 0)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, req_param->new_pin, 0)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, req_param->new_pin_confirm, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MUPIN, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MUAPP */
ril_status_t ril_request_uicc_application_list_access(ril_request_mode_t mode,
        int32_t access_mode,
        int32_t index,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, access_mode, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, index, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MUAPP, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MSST */
ril_status_t ril_request_read_sst_ust_from_usim(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_MSST, NULL, (void *)callback, user_data);
}


/* AT cmd: AT*MABORT */
ril_status_t ril_request_abort_mm_related_at_command(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_MABORT, NULL, (void *)callback, user_data);
}


#ifndef __RIL_CMD_SET_SLIM_ENABLE__
/* AT cmd: AT*MCAL */
ril_status_t ril_request_radio_call(ril_request_mode_t mode,
        ril_radio_call_req_t *req,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    int32_t idx = 0;
    if (req->param_count > RIL_RADIO_CALL_REQ_MAX_PARAM_COUNT) {
        return RIL_STATUS_INVALID_PARAM;
    }

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, req->cmd_group, 2)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, req->item_index, 2)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, req->function_index, 2)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, req->param_count, 2)) < 0) {
                break;
            }
            while (idx < req->param_count) {
                if ((ret = ril_param_list_add_int_optional(list_head, req->param_array[idx++], 2)) < 0) {
                    break;
                }
            }
            if ((ret = ril_param_list_add_int_optional(list_head, req->check_sum, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MCAL, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MNON */
ril_status_t ril_request_network_operator_name(ril_request_mode_t mode,
        int32_t index,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, index, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MNON, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MOPL */
ril_status_t ril_request_network_operator_plmn_list(ril_request_mode_t mode,
        int32_t index,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, index, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MOPL, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MMUX */
ril_status_t ril_request_mux_configuration(ril_request_mode_t mode,
        int32_t atp_seg_ui,
        int32_t seg_ui_27010,
        int32_t esc_mon,
        int32_t cs_en_27010,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, atp_seg_ui, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, seg_ui_27010, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, esc_mon, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, cs_en_27010, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MMUX, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MROUTEMMI */
ril_status_t ril_request_mmi_at_channel_routing_configuration(ril_request_mode_t mode,
        int32_t control_mode,
        int32_t option_id,
        int32_t option_value,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, control_mode, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, option_id, 1)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, option_value, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MROUTEMMI, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MCEERMODE */
ril_status_t ril_request_ceer_response_mode(ril_request_mode_t mode,
        int32_t mode_setting,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, mode_setting, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MCEERMODE, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MFTRCFG */
ril_status_t ril_request_modem_feature_configuration(ril_request_mode_t mode,
        int32_t access_mode,
        int32_t modem_feature,
        int32_t feature_value,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, access_mode, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, access_mode, 0)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, access_mode, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MFTRCFG, list_head, (void *)callback, user_data);
    }
}
#endif /* __RIL_CMD_SET_SLIM_ENABLE__ */


/* AT cmd: AT^HVER */
ril_status_t ril_request_request_hw_version(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_HVER, NULL, (void *)callback, user_data);
}


#ifndef __RIL_CMD_SET_SLIM_ENABLE__
/* AT cmd: AT^MODE */
ril_status_t ril_request_indicate_system_mode(ril_request_mode_t mode,
        int32_t enable,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, enable, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MODE, list_head, (void *)callback, user_data);
    }
}



/* AT cmd: AT^SYSINFO */
ril_status_t ril_request_request_system_information(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_SYSINFO, NULL, (void *)callback, user_data);
}


/* AT cmd: AT^SYSCONFIG */
ril_status_t ril_request_configure_system_reference(ril_request_mode_t mode,
        ril_configure_system_reference_req_t *req,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, req->mode, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, req->acqorder, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, req->roam, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, req->srvdomain, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_SYSCONFIG, list_head, (void *)callback, user_data);
    }
}



/* AT cmd: AT^CARDMODE */
ril_status_t ril_request_request_sim_usim_mode(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_CARDMODE, NULL, (void *)callback, user_data);
}


/* AT cmd: AT^SPN */
ril_status_t ril_request_read_service_provider_name(ril_request_mode_t mode,
        int32_t spn_type,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, spn_type, 2)) < 0) {
                break;
            }

        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_SPN, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MSTLOCK */
ril_status_t ril_request_STK_registering(ril_request_mode_t mode,
        int32_t data,
        int32_t timeout,
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

            /* data, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, data, 2)) < 0) {
                break;
            }

            /* timeout, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, timeout, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MSTLOCK, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MSTPD */
ril_status_t ril_request_STK_terminal_profile_download(ril_request_mode_t mode,
        int32_t length,
        char *hex_data,
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

            /* data, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, length, 2)) < 0) {
                break;
            }

            /* hex_data, necessary */
            if ((ret = ril_param_list_add_string_optional(list_head, hex_data, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MSTPD, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MSTMODE */
ril_status_t ril_request_STK_output_format_setting(ril_request_mode_t mode,
        int32_t output_mode,
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

            /* data, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, output_mode, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MSTMODE, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MSTICREC */
ril_status_t ril_request_STK_obtain_icon_record(ril_request_mode_t mode,
        int32_t num_rec,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {

            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                RIL_LOGE("failed to create param list\r\n");
                break;
            }

            /* num_rec, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, num_rec, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MSTICREC, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MSTICIMG */
ril_status_t ril_request_STK_obtain_icon_image(ril_request_mode_t mode,
        int32_t efid,
        int32_t offset,
        int32_t length,
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

            /* efid, necessary */
            if ((ret = ril_param_list_add_hexint_optional(list_head, efid, 2)) < 0) {
                break;
            }

            /* offset, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, offset, 0)) < 0) {
                ret = 0;
            }

            /* length, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, length, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MSTICIMG, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MSTC */
#if 0
ril_status_t ril_request_STK_proactive_command_indication(ril_request_mode_t mode,
        int32_t cmdid,
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

            /* cmdid, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, cmdid, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MSTC, list_head, (void *)callback, user_data);
    }
}
#endif

/* AT cmd: AT*MSTGC */
ril_status_t ril_request_STK_parameters_associated_with_proactive_command(ril_request_mode_t mode,
        char *cmdid,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {

            ret = ril_param_list_create(&list_head);
            if (ret < 0) {
                RIL_LOGE("failed to create param list\r\n");
                break;
            }

            /* cmdid, necessary */
            if ((ret = ril_param_list_add_digital_string_optional(list_head, cmdid, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MSTGC, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MSTCR */
ril_status_t ril_request_STK_inform_response_to_proactive_command(ril_request_mode_t mode,
        char *cmdid,
        int32_t result,
        char *data,
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

            /* cmdid, necessary */
            if ((ret = ril_param_list_add_digital_string_optional(list_head, cmdid, 2)) < 0) {
                break;
            }

            /* result, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, result, 2)) < 0) {
                break;
            }

            /* data, optional */
            if ((ret = ril_param_list_add_string_optional(list_head, data, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MSTCR, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MSTUD */
#if 0
ril_status_t ril_request_STK_unsolicited_data(ril_request_mode_t mode,
        int32_t cmdid,
        char *data,
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

            /* cmdid, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, cmdid, 2)) < 0) {
                break;
            }

            /* data, optional */
            if ((ret = ril_param_list_add_string_optional(list_head, data, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MSTUD, list_head, (void *)callback, user_data);
    }
}
#endif

/* AT cmd: AT*MSTMS */
ril_status_t ril_request_STK_menu_selection(ril_request_mode_t mode,
        int32_t item,
        int32_t help,
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

            if ((ret = ril_param_list_add_int_optional(list_head, item, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, help, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MSTMS, list_head, (void *)callback, user_data);
    }
}


/* AT cmd:AT*MSTEV */
ril_status_t ril_request_STK_monitored_event_occurence(ril_request_mode_t mode,
        ril_STK_monitored_event_occurence_param_t *param,
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

            if (param == NULL) {
                ret = -1;
                break;
            }

            /* event, necessary */
            if ((ret = ril_param_list_add_digital_string_optional(list_head, param->event, 2)) < 0) {
                break;
            }

            /* language, optional */
            if ((ret = ril_param_list_add_string_optional(list_head, param->language, 0)) < 0) {
                break;
            }

            /* charsDownDisplay, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, param->charsDownDisplay, 0)) < 0) {
                break;
            }

            /* sizingSupported, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, param->sizingSupported, 0)) < 0) {
                break;
            }

            /* charsAcrossDisplay, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, param->charsAcrossDisplay, 0)) < 0) {
                break;
            }

            /* variableFontSupport, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, param->variableFontSupport, 0)) < 0) {
                break;
            }

            /* displayResize, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, param->displayResize, 0)) < 0) {
                break;
            }

            /* textWrapping, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, param->textWrapping, 0)) < 0) {
                break;
            }

            /* textScrolling, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, param->textScrolling, 0)) < 0) {
                break;
            }

            /* menuWidthReduction, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, param->menuWidthReduction, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MSTEV, list_head, (void *)callback, user_data);
    }
}
#endif /* __RIL_CMD_SET_SLIM_ENABLE__ */


/* AT cmd: AT*MICCID */
ril_status_t ril_request_read_usim_iccid(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_MICCID, NULL, (void *)callback, user_data);
}


/* AT cmd: AT*MHOMENW */
ril_status_t ril_request_display_home_network_information(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_MHOMENW, NULL, (void *)callback, user_data);
}


#ifndef __RIL_CMD_SET_SLIM_ENABLE__
/* AT cmd: AT*MCSIMLOCK */
ril_status_t ril_request_lock_csim_access(ril_request_mode_t mode,
        int32_t lock_state,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, lock_state, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MCSIMLOCK, list_head, (void *)callback, user_data);
    }
}



/* AT cmd: AT*MEMMREEST */
ril_status_t ril_request_emm_re_establishment_test(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_MEMMREEST, NULL, (void *)callback, user_data);
}


/* AT cmd: AT*MAPNURI */
ril_status_t ril_request_apn_rate_control_indication(ril_request_mode_t mode,
        int32_t control_mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, control_mode, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MAPNURI, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MPLMNURI */
ril_status_t ril_request_plmn_rate_control_indication(ril_request_mode_t mode,
        int32_t control_mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, control_mode, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MPLMNURI, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MPDI */
ril_status_t ril_request_packet_discard_indication(ril_request_mode_t mode,
        int32_t control_mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, control_mode, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MPDI, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MNBIOTDT */
ril_status_t ril_request_nbiot_data_type(ril_request_mode_t mode,
        ril_nbiot_data_type_req_t *req_param,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    int32_t i;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if (req_param == NULL) {
                return RIL_STATUS_INVALID_PARAM;
            }
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, req_param->type, 2)) < 0) {
                break;
            }

            for (i = 0; i < req_param->param_num; i++) {
                if ((ret = ril_param_list_add_int_optional(list_head, req_param->cid_list[i], 0)) < 0) {
                    break;
                }
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MNBIOTDT, list_head, (void *)callback, user_data);
    }
}


/* AT cmd:AT*MNBIOTRAI */
ril_status_t ril_request_nbiot_release_assistance_indication(ril_request_mode_t mode,
        int32_t rai,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, rai, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MNBIOTRAI, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MNVMQ */
ril_status_t ril_request_nvdm_status_query(ril_request_mode_t mode,
        char *tool_version,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, tool_version, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MNVMQ, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MNVMAUTH */
ril_status_t ril_request_nvdm_access_security_authorization(ril_request_mode_t mode,
        int32_t cert_data_length,
        char *certificate_bytes_data,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, cert_data_length, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, certificate_bytes_data, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MNVMAUTH, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MNVMW */
ril_status_t ril_request_nvdm_data_write(ril_request_mode_t mode,
        ril_nvdm_data_write_req_t *req_param,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if (req_param == NULL) {
                return RIL_STATUS_INVALID_PARAM;
            }
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, req_param->area_info, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, req_param->group_id, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, req_param->data_item_id, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, req_param->data_type, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, req_param->length, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, req_param->data, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MNVMW, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MNVMR */
ril_status_t ril_request_nvdm_data_read(ril_request_mode_t mode,
                                        int32_t area_info,
                                        char *group_id,
                                        char *data_item_id,
                                        int32_t length,
                                        ril_cmd_response_callback_t callback,
                                        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, area_info, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, group_id, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, data_item_id, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, length, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MNVMR, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MNVMGET */
ril_status_t ril_request_nvdm_get_all_data_item_id(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_MNVMGET, NULL, (void *)callback, user_data);
}


/* AT cmd:AT*MNVMINVD */
ril_status_t ril_request_nvdm_data_item_invalidate(ril_request_mode_t mode,
        int32_t area_info,
        char *group_id,
        char *data_item_id,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, area_info, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, group_id, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, data_item_id, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MNVMIVD, list_head, (void *)callback, user_data);
    }
}


/* AT cmd:AT*MNVMRSTONE */
ril_status_t ril_request_nvdm_data_item_factory_reset(ril_request_mode_t mode,
        int32_t area_info,
        char *group_id,
        char *data_item_id,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, area_info, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, group_id, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, data_item_id, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MNVMRSTONE, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MNVMRST */
ril_status_t ril_request_nvdm_factory_reset_all_data_item(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_MNVMRST, NULL, (void *)callback, user_data);
}


/* AT cmd: AT*MNVMMDNQ */
ril_status_t ril_request_nvdm_query_num_of_mini_dump(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_MNVMMDNQ, NULL, (void *)callback, user_data);
}


/* AT cmd: AT*MNVMMDR */
ril_status_t ril_request_nvdm_read_mini_dump(ril_request_mode_t mode,
        int32_t mini_dump_idx,
        int32_t offset,
        int32_t data_length,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, mini_dump_idx, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, offset, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, data_length, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MNVMMDR, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MNVMMDC */
ril_status_t ril_request_nvdm_clean_mini_dump(ril_request_mode_t mode,
        int32_t mini_dump_idx,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, mini_dump_idx, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MNVMMDC, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+IDCFREQ */
ril_status_t ril_request_set_idc_frequency_range(ril_request_mode_t mode,
        ril_set_idc_frequency_range_req_t *req_param,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if (req_param == NULL) {
                return RIL_STATUS_INVALID_PARAM;
            }
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, req_param->freq1_start, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, req_param->freq1_stop, 0)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, req_param->freq2_start, 0)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, req_param->freq2_stop, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, req_param->freq3_start, 0)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, req_param->freq3_stop, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_IDCFREQ, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+IDCPWRBACKOFF */
ril_status_t ril_request_set_tx_power_back_off(ril_request_mode_t mode,
        int32_t attenuation_power,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, attenuation_power, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_IDCPWRBACKOFF, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+IDCTX2GPS */
ril_status_t ril_request_generate_periodic_tx_for_gps(ril_request_mode_t mode,
        int32_t request_mode,
        int32_t freq,
        int32_t tx_pwr,
        int32_t period,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, request_mode, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, freq, 0)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, tx_pwr, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, period, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_IDCTX2GPS, list_head, (void *)callback, user_data);
    }
}
#endif /* __RIL_CMD_SET_SLIM_ENABLE__ */


/* AT*MCALDEV */
ril_status_t ril_request_enter_exit_rf_calibration_state(ril_request_mode_t mode,
        int32_t caldev_state,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, caldev_state, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MCALDEV, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MATWAKEUP */
ril_status_t ril_request_set_modem_wakeup_indication(ril_request_mode_t mode,
        int32_t n,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, n, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MATWAKEUP, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MBAND */
ril_status_t ril_request_query_modem_operating_band(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_MBAND, NULL, (void *)callback, user_data);
}


/* AT cmd: AT*MENGINFO */
ril_status_t ril_request_query_network_state(ril_request_mode_t mode,
        int32_t display_mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, display_mode, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MENGINFO, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MFRCLLCK */
ril_status_t ril_request_frequency_and_cell_lock(ril_request_mode_t mode,
        int32_t lock,
        int32_t earfcn,
        int32_t earfcn_offset,
        int32_t pci,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, lock, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, earfcn, 0)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, earfcn_offset, 1)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, pci, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MFRCLLCK, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MSPCHSC */
ril_status_t ril_request_set_scrambling_algorithm_for_npdsch(ril_request_mode_t mode,
        int32_t algo_mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, algo_mode, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MSPCHSC, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MDPDNP */
ril_status_t ril_request_default_pdn_parameter(ril_request_mode_t mode,
        int32_t n,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, n, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MDPDNP, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT*MEDRXCFG */
ril_status_t ril_request_eDRX_configuration(ril_request_mode_t mode,
        int32_t cfg_mode,
        int32_t act_type,
        char *requested_eDRX_value,
        char *requested_paging_time_window_value,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, cfg_mode, 1)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, act_type, 1)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_string_optional(list_head, requested_eDRX_value, 1)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_string_optional(list_head, requested_paging_time_window_value, 1)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MEDRXCFG, list_head, (void *)callback, user_data);
    }
}


/* AT command: AT*MCELLINFO */
ril_status_t ril_request_serving_and_neighbor_cell_info(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_MCELLINFO, NULL, (void *)callback, user_data);
}


/* AT command: AT*MUPDIR */
ril_status_t ril_request_indicate_packet_to_track(ril_request_mode_t mode,
        ril_indicate_packet_to_track_req_t *req_param,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if (req_param == NULL) {
                return RIL_STATUS_INVALID_PARAM;
            }
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if (req_param->mode == 0) {
                if ((ret = ril_param_list_add_int_optional(list_head, req_param->mode, 2)) < 0) {
                    break;
                }

                if ((ret = ril_param_list_add_int_optional(list_head, req_param->pattern_id, 2)) < 0) {
                    break;
                }
            } else if (req_param->mode == 1) {
                if ((ret = ril_param_list_add_int_optional(list_head, req_param->mode, 2)) < 0) {
                    break;
                }

                if ((ret = ril_param_list_add_string_optional(list_head, req_param->ip_type, 2)) < 0) {
                    break;
                }

                if ((ret = ril_param_list_add_string_optional(list_head, req_param->src_ip, 2)) < 0) {
                    break;
                }

                if ((ret = ril_param_list_add_int_optional(list_head, req_param->src_udp_port, 1)) < 0) {
                    break;
                }

                if ((ret = ril_param_list_add_string_optional(list_head, req_param->dst_ip, 2)) < 0) {
                    break;
                }

                if ((ret = ril_param_list_add_int_optional(list_head, req_param->dst_udp_port, 2)) < 0) {
                    break;
                }

                if ((ret = ril_param_list_add_int_optional(list_head, req_param->msg_id_offset, 2)) < 0) {
                    break;
                }

                if ((ret = ril_param_list_add_int_optional(list_head, req_param->msg_id_size, 2)) < 0) {
                    break;
                }
            } else {
                ret = -1;
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_MUPDIR, list_head, (void *)callback, user_data);
    }
}


/*************************************************
 *                 response hdlr
 *************************************************/

/* AT cmd: AT*MLTS */
int32_t ril_response_get_local_timestamp_and_network_info(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_get_local_timestamp_and_network_info_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.enable);
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


/* AT*MSIMINS */
int32_t ril_response_sim_inserted_status_reporting(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_sim_inserted_status_reporting_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.n);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.inserted);
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


/* AT*MSPN */
int32_t ril_response_get_service_provider_name_from_sim(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_get_service_provider_name_from_sim_rsp_t response;
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

            ret = at_tok_nextstr(&line, &response.spn);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.display_mode);
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


/* AT*MUNSOL */
int32_t ril_response_extra_unsolicited_indications(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT*MGCOUNT */
int32_t ril_response_packet_domain_counters(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT*MGSINK */
int32_t ril_response_send_packet_to_discard(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT cmd: AT*MCGDEFCONT */
int32_t ril_response_pdn_connection_set_default_psd_attach(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_send_pdn_connection_set_default_psd_attach_rsp_t response;
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

            ret = at_tok_nextstr(&line, &response.pdp_type);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &response.apn);
            if (ret < 0) {
                ret = 0;
                response.apn = RIL_OMITTED_STRING_PARAM;
            }

            ret = at_tok_nextstr(&line, &response.username);
            if (ret < 0) {
                ret = 0;
                response.username = RIL_OMITTED_STRING_PARAM;
            }

            ret = at_tok_nextstr(&line, &response.password);
            if (ret < 0) {
                ret = 0;
                response.password = RIL_OMITTED_STRING_PARAM;
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


/* AT cmd: AT*MGTCSINK */
int32_t ril_response_send_tcpip_packet_to_discard(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT cmd: AT*MSACL */
int32_t ril_response_control_ACL_feature(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_control_ACL_feature_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.supported);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.enabled);
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


/* AT cmd: AT*MLACL */
int32_t ril_response_display_ACL_list(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_display_ACL_list_rsp_t response;
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

            ret = at_tok_nextstr(&line, &response.APN);
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


/* AT cmd: AT*MWACL */
int32_t ril_response_write_ACL_entry(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT cmd: AT*MDACL */
int32_t ril_response_delete_ACL_entry(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT cmd: AT*MSMEXTRAINFO */
int32_t ril_response_control_extra_info_on_sms(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_control_extra_info_on_sms_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.enable);
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


/* AT cmd: AT*MSMEXTRAUNSOL */
int32_t ril_response_control_extra_unsolicited_messages(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_control_extra_unsolicited_messages_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.enable);
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


/* AT cmd: AT*MSMSTATUS */
int32_t ril_response_obtain_status_of_sms_functionality(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_obtain_status_of_sms_functionality_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.sms_status);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.unread_records);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.used_records);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.memory_exceeded);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.num_sms_records);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.first_free_record);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.num_smsp_records);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.service);
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


/* AT cmd: AT*MMGI */
int32_t ril_response_control_sms_unsolicited_indication(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_control_sms_unsolicited_indication_rsp_t response;
    ril_control_sms_unsolicited_indication_entry_t *entry, *list_head;

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
    response.entry_num = 0;
    response.param_list = NULL;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (message_num == 0) {
                list_head = (ril_control_sms_unsolicited_indication_entry_t *)ril_mem_malloc(sizeof(ril_control_sms_unsolicited_indication_entry_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_control_sms_unsolicited_indication_entry_t) * (message_num + 1));
                entry = list_head + message_num;
            }
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->event_id);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->status);
            if (ret < 0) {
                break;
            }

            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.param_list = list_head;
    response.entry_num = message_num;
    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}


/* AT cmd: AT*MMGRW */
int32_t ril_response_sms_location_rewrite(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_sms_location_rewrite_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.loc);
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


/* AT cmd: AT*MMGSC */
int32_t ril_response_sms_location_status_change(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT cmd: AT*MUPIN */
int32_t ril_response_uicc_pin_information_access(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_uicc_pin_information_access_rsp_t response;
    ril_uicc_pin_information_access_entry_t *entry, *list_head;

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
    response.entry_num = 0;
    response.param_list = NULL;

    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (message_num == 0) {
                list_head = (ril_uicc_pin_information_access_entry_t *)ril_mem_malloc(sizeof(ril_uicc_pin_information_access_entry_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_uicc_pin_information_access_entry_t) * (message_num + 1));
                entry = list_head + message_num;
            }
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->access_level);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->pin_status);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->pin_verification_state);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->pin_retry_counter);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->puk_retry_counter);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->key_reference);
            if (ret < 0) {
                ret = 0;
                entry->key_reference = NULL;
            }

            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.param_list = list_head;
    response.entry_num = message_num;
    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}


/* AT cmd: AT*MUAPP */
int32_t ril_response_uicc_application_list_access(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_uicc_application_list_access_rsp_t response;
    ril_uicc_application_list_access_entry_t *entry, *list_head;

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
    response.entry_num = 0;
    response.param_list = NULL;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (message_num == 0) {
                list_head = (ril_uicc_application_list_access_entry_t *)ril_mem_malloc(sizeof(ril_uicc_application_list_access_entry_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_uicc_application_list_access_entry_t) * (message_num + 1));
                entry = list_head + message_num;
            }
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->index);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->application_code);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->application_state);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->application_label);
            if (ret < 0) {
                ret = 0;
                entry->application_label = NULL;
                break;
            }

            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.param_list = list_head;
    response.entry_num = message_num;
    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}


/* AT cmd: AT*MSST */
int32_t ril_response_read_sst_ust_from_usim(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_read_sst_ust_from_usim_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.stt);
            if (ret < 0) {
                break;
            }
            
            response.sst_bitmap_allocated = RIL_OMITTED_STRING_PARAM;
            response.sst_bitmap_activated = RIL_OMITTED_STRING_PARAM;
            response.ust_bitmap_available = RIL_OMITTED_STRING_PARAM;
            response.est_bitmap = RIL_OMITTED_STRING_PARAM;
            ret = at_tok_nextstr(&line, &response.sst_bitmap_allocated);
            if (ret < 0) {
                ret = 0;
            }

            ret = at_tok_nextstr(&line, &response.sst_bitmap_activated);
            if (ret < 0) {
                ret = 0;
            }

            ret = at_tok_nextstr(&line, &response.ust_bitmap_available);
            if (ret < 0) {
                ret = 0;
            }

            ret = at_tok_nextstr(&line, &response.est_bitmap);
            if (ret < 0) {
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


/* AT cmd: AT*MABORT */
int32_t ril_response_abort_mm_related_at_command(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT cmd: AT*MCAL */
int32_t ril_response_radio_call(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_radio_call_rsp_t response;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;
    int32_t idx = 0;

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

            ret = at_tok_nextstr(&line, &response.cmd_group);
            if (ret < 0) {
                break;
            }
            ret = at_tok_nextint(&line, &response.item_index);
            if (ret < 0) {
                break;
            }
            ret = at_tok_nextint(&line, &response.function_index);
            if (ret < 0) {
                break;
            }
            ret = at_tok_nextint(&line, &response.status_code);
            if (ret < 0) {
                break;
            }
            ret = at_tok_nextint(&line, &response.param_count);
            if (ret < 0) {
                break;
            }

            while (response.param_count < RIL_RADIO_CALL_REQ_MAX_PARAM_COUNT && idx < response.param_count) {
                ret = at_tok_nextint(&line, &response.param_array[idx]);
                if (ret < 0) {
                    break;
                }
            }
            if (idx < response.param_count) {
                break;
            }
            ret = at_tok_nextint(&line, &response.check_sum);
            if (ret < 0) {
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


/* AT cmd: AT*MNON */
int32_t ril_response_network_operator_name(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_network_operator_name_rsp_t response;
    ril_network_operator_name_entry_t *entry, *list_head;

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
            /* information response */
            if (message_num == 0) {
                list_head = (ril_network_operator_name_entry_t *)ril_mem_malloc(sizeof(ril_network_operator_name_entry_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_network_operator_name_entry_t) * (message_num + 1));
                entry = list_head + message_num;
            }
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->index);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->operator_name);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->short_form);
            if (ret < 0) {
                ret = 0;
                entry->short_form = RIL_OMITTED_STRING_PARAM;
            }

            ret = at_tok_nextstr(&line, &entry->plmn_add_info);
            if (ret < 0) {
                ret = 0;
                entry->plmn_add_info = RIL_OMITTED_STRING_PARAM;
            }

            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.param_list = list_head;
    response.entry_num = message_num;
    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}


/* AT cmd: AT*MOPL */
int32_t ril_response_network_operator_plmn_list(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_network_operator_plmn_list_rsp_t response;
    ril_network_operator_plmn_list_entry_t *entry, *list_head;

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
            /* information response */
            if (message_num == 0) {
                list_head = (ril_network_operator_plmn_list_entry_t *)ril_mem_malloc(sizeof(ril_network_operator_plmn_list_entry_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_network_operator_plmn_list_entry_t) * (message_num + 1));
                entry = list_head + message_num;
            }
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->index);
            if (ret < 0) {
                break;
            }

            if (channel_p->curr_request_mode == RIL_EXECUTE_MODE) {
                ret = at_tok_nextstr(&line, &entry->mcc_mnc);
                if (ret < 0) {
                    break;
                }
            } else {
                entry->mcc_mnc = RIL_OMITTED_STRING_PARAM;
            }

            ret = at_tok_nextstr(&line, &entry->lac_range);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->pnn_id);
            if (ret < 0) {
                break;
            }

            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.param_list = list_head;
    response.entry_num = message_num;
    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}


/* AT cmd: AT*MMUX */
int32_t ril_response_mux_configuration(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT cmd: AT*MROUTEMMI */
int32_t ril_response_mmi_at_channel_routing_configuration(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_mmi_at_channel_routing_configuration_rsp_t response;
    ril_mmi_at_channel_routing_configuration_entry_t *entry, *list_head;

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
            /* information response */
            if (message_num == 0) {
                list_head = (ril_mmi_at_channel_routing_configuration_entry_t *)ril_mem_malloc(sizeof(ril_mmi_at_channel_routing_configuration_entry_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_mmi_at_channel_routing_configuration_entry_t) * (message_num + 1));
                entry = list_head + message_num;
            }
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->option_id);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->option_value);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->list_of_id);
            if (ret < 0) {
                break;
            }

            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.param_list = list_head;
    response.entry_num = message_num;
    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}


/* AT cmd: AT*MCEERMODE */
int32_t ril_response_ceer_response_mode(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_ceer_response_mode_rsp_t response;
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


/* AT cmd: AT*MFTRCFG */
int32_t ril_response_modem_feature_configuration(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_modem_feature_configuration_rsp_t response;
    ril_modem_feature_configuration_entry_t *entry, *list_head;

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
            /* information response */
            if (message_num == 0) {
                list_head = (ril_modem_feature_configuration_entry_t *)ril_mem_malloc(sizeof(ril_modem_feature_configuration_entry_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_modem_feature_configuration_entry_t) * (message_num + 1));
                entry = list_head + message_num;
            }
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->modem_feature);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->feature_value);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->need_reboot);
            if (ret < 0) {
                break;
            }

            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.param_list = list_head;
    response.entry_num = message_num;
    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}


/* AT cmd: AT^HVER */
int32_t ril_response_request_hw_version(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_request_hw_version_rsp_t response;
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

            ret = at_tok_nextstr(&line, &response.hw_version);
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


/* AT cmd: AT^MODE */
int32_t ril_response_indicate_system_mode(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_indicate_system_mode_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.enable);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.sys_mode);
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


/* AT cmd: AT^SYSINFO */
int32_t ril_response_request_system_information(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_request_system_information_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.srv_status);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.srv_domain);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.roam_status);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.sys_mode);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.sim_state);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.reserve);
            if (ret < 0) {
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


/* AT cmd: AT^SYSCONFIG */
int32_t ril_response_configure_system_reference(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_configure_system_reference_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.acqorder);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.roam);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.srvdomain);
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


/* AT cmd: AT^CARDMODE */
int32_t ril_response_request_sim_usim_mode(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_request_sim_usim_mode_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.sim_type);
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


/* AT cmd: AT^SPN */
int32_t ril_response_read_service_provider_name(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_read_service_provider_name_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.disp_rplmn);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.coding);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.spn_name);
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


/* AT cmd: AT*MSTLOCK */
int32_t ril_response_STK_registering(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT cmd: AT*MSTPD */
int32_t ril_response_STK_terminal_profile_download(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT cmd: AT*MSTMODE */
int32_t ril_response_STK_output_format_setting(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_STK_output_format_setting_rsp_t response;
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
            /* no information response */
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.output_mode);
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


int32_t ril_response_STK_obtain_icon_record(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_STK_obtain_icon_record_rsp_t response;
    ril_STK_obtain_icon_record_struct_t *entry, *list_head;
    bool is_first_line;

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

    response.num_rec = 0;
    response.num_instances = 0;
    response.instances_list = NULL;

    is_first_line = true;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */

            /* READ MODE, *MSTICREC: <num_rec> */
            if (channel_p->curr_request_mode == RIL_READ_MODE) {
                ret = at_tok_start(&line);
                if (ret < 0 ) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.num_rec);
                if (ret < 0 ) {
                    break;
                }
            } else {

                /* EXECUTE MODE format: */
                /* MSTICREC: <num_rec>,<num_instances><CR><LF> */
                /* MSTICREC: <width>,<height>,<cs>,<efld>,<offset>,<length><CR><LF> */
                /* MSTICREC: <width>,<height>,<cs>,<efld>,<offset>,<length><CR><LF> */
                /* ...... */
                if (is_first_line) {
                    ret = at_tok_start(&line);
                    if (ret < 0 ) {
                        break;
                    }

                    ret = at_tok_nextint(&line, &response.num_rec);
                    if (ret < 0 ) {
                        break;
                    }

                    ret = at_tok_nextint(&line, &response.num_instances);
                    if (ret < 0 ) {
                        break;
                    }

                    is_first_line = false;
                } else {
                    if (message_num == 0) {
                        list_head = (ril_STK_obtain_icon_record_struct_t *)ril_mem_malloc(sizeof(ril_STK_obtain_icon_record_struct_t));
                        entry = list_head;
                    } else {
                        list_head = ril_mem_realloc(list_head, sizeof(ril_STK_obtain_icon_record_struct_t) * (message_num + 1));
                        entry = list_head + message_num;
                    }
                    ret = at_tok_start(&line);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line, &entry->width);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line, &entry->height);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line, &entry->cs);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nexthexint(&line, &entry->efid);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line, &entry->offset);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line, &entry->length);
                    if (ret < 0) {
                        break;
                    }
                    message_num++;
                }
            }
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.instances_list = list_head;

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}


/* AT cmd:AT*MSTICIMG */
int32_t ril_response_STK_obtain_icon_image(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_STK_obtain_icon_image_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.length);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &response.data);
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


/* AT cmd: AT*MSTC */
#if 0
int32_t ril_response_STK_proactive_command_indication(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    void *response;
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
#endif


/* AT cmd: AT*MSTGC */
int32_t ril_response_STK_parameters_associated_with_proactive_command(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_STK_parameters_associated_with_proactive_command_rsp_t response;
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

            ret = at_tok_nextstr(&line, &response.cmdid);
            if (ret < 0) {
                break;
            }

            if (line && *line != '\0') {
                response.data = line;
            } else {
                response.data = RIL_OMITTED_STRING_PARAM;
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


/* AT cmd: AT*MSTCR */
int32_t ril_response_STK_inform_response_to_proactive_command(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT cmd: AT*MSTUD */
#if 0
int32_t ril_response_STK_unsolicited_data(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    void *response;
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
#endif


int32_t ril_response_STK_menu_selection(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


int32_t ril_response_STK_monitored_event_occurence(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT cmd: AT*MCCST */
#if 0
int32_t ril_response_STK_unsolicited_response_for_call_control(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    void *response;
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
#endif


/* AT cmd:AT*MICCID */
int32_t ril_response_read_usim_iccid(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_read_usim_iccid_rsp_t response;
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

            ret = at_tok_nextstr(&line, &response.iccid);
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


/* AT cmd:AT*MHOMENW */
int32_t ril_response_display_home_network_information(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_display_home_network_information_rsp_t response;
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

            ret = at_tok_nextstr(&line, &response.oper_long_alpha);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &response.oper_short_alpha);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.oper_numeric);
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


/* AT cmd: AT*MCSIMLOCK */
int32_t ril_response_lock_csim_access(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_lock_csim_access_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.lock_state);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.other_lock_state);
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


/* AT cmd: AT*MEMMREEST */
int32_t ril_response_emm_re_establishment_test(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    void *response;
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


/* AT cmd: AT*MAPNURI */
int32_t ril_response_apn_rate_control_indication(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_apn_rate_control_indication_rsp_t response;
    ril_apn_rate_control_indication_entry_t *entry, *list_head;

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
    response.entry_num = 0;
    response.param_list = NULL;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (message_num == 0) {
                list_head = (ril_apn_rate_control_indication_entry_t *)ril_mem_malloc(sizeof(ril_apn_rate_control_indication_entry_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_apn_rate_control_indication_entry_t) * (message_num + 1));
                entry = list_head + message_num;
            }
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->mode);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->cid);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->urc_active);
            if (ret < 0) {
                break;
            }

            entry->additional_exception_reports = RIL_OMITTED_INTEGER_PARAM;
            entry->uplink_time_unit = RIL_OMITTED_INTEGER_PARAM;
            entry->maximum_uplink_rate = RIL_OMITTED_INTEGER_PARAM;
            ret = at_tok_nextint(&line, &entry->additional_exception_reports);
            if (ret < 0) {
                ret = 0;
                break;
            }

            ret = at_tok_nextint(&line, &entry->uplink_time_unit);
            if (ret < 0) {
                ret = 0;
                break;
            }

            ret = at_tok_nextint(&line, &entry->maximum_uplink_rate);
            if (ret < 0) {
                ret = 0;
                break;
            }

            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.param_list = list_head;
    response.entry_num = message_num;

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}


/* AT cmd: AT*MPLMNURI */
int32_t ril_response_plmn_rate_control_indication(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_plmn_rate_control_indication_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.urc_active);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.serving_plmn_rate_control_value);
            if (ret < 0) {
                ret = 0;
                response.serving_plmn_rate_control_value = RIL_OMITTED_INTEGER_PARAM;
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


/* AT cmd: AT*MPDI */
int32_t ril_response_packet_discard_indication(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_packet_discard_indication_rsp_t response;
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


/* AT cmd: AT*MNBIOTDT */
int32_t ril_response_nbiot_data_type(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_nbiot_data_type_rsp_t response;
    ril_nbiot_data_type_entry_t *entry, *list_head;

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
    response.entry_num = 0;
    response.param_list = NULL;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (message_num == 0) {
                list_head = (ril_nbiot_data_type_entry_t *)ril_mem_malloc(sizeof(ril_nbiot_data_type_entry_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_nbiot_data_type_entry_t) * (message_num + 1));
                entry = list_head + message_num;
            }
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->cid);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->type);
            if (ret < 0) {
                break;
            }

            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.param_list = list_head;
    response.entry_num = message_num;
    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}


/* AT cmd: AT*MNBIOTRAI */
int32_t ril_response_nbiot_release_assistance_indication(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_nbiot_release_assistance_indication_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.rai);
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


/* AT cmd: AT*MNVMQ */
int32_t ril_response_nvdm_status_query(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_nvdm_status_query_rsp_t response;
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

            ret = at_tok_nextstr(&line, &response.chip_name);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.needs_security_authentication);
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


/* AT cmd: AT*MNVMAUTH */
int32_t ril_response_nvdm_access_security_authorization(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_nvdm_access_security_authorization_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.auth_result);
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


/* AT cmd: AT*MNVMW */
int32_t ril_response_nvdm_data_write(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_nvdm_data_write_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.write_status);
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


/* AT cmd: AT*MNVMR */
int32_t ril_response_nvdm_data_read(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_nvdm_data_read_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.read_status);
            if (ret < 0) {
                break;
            }

            response.area_info = RIL_OMITTED_INTEGER_PARAM;
            response.group_id = RIL_OMITTED_STRING_PARAM;
            response.data_item_id = RIL_OMITTED_STRING_PARAM;
            response.data_type = RIL_OMITTED_INTEGER_PARAM;
            response.length = RIL_OMITTED_INTEGER_PARAM;
            response.data = RIL_OMITTED_STRING_PARAM;
            /* optional param */
            do {
                ret = at_tok_nextint(&line, &response.area_info);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextstr(&line, &response.group_id);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextstr(&line, &response.data_item_id);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.data_type);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.length);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextstr(&line, &response.data);
                if (ret < 0) {
                    break;
                }
            } while (0);
            ret = 0;
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


/* AT cmd: AT*MNVMGET */
int32_t ril_response_nvdm_get_all_data_item_id(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_nvdm_get_all_data_item_id_rsp_t response;
    ril_nvdm_get_all_data_item_id_entry_t *entry, *list_head;

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
    response.entry_num = 0;
    response.param_list = NULL;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (str_starts_with(line, "*MNVMGETST:")) {
                /* fail to get id */
                ret = at_tok_start(&line);
                ret = at_tok_nextint(&line, &response.status);
                break;
            }

            if (message_num == 0) {
                list_head = (ril_nvdm_get_all_data_item_id_entry_t *)ril_mem_malloc(sizeof(ril_nvdm_get_all_data_item_id_entry_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_nvdm_get_all_data_item_id_entry_t) * (message_num + 1));
                entry = list_head + message_num;
            }
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->area_info);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->group_id);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->data_item_id);
            if (ret < 0) {
                break;
            }

            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.param_list = list_head;
    response.entry_num = message_num;
    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}



/* AT cmd: AT*MNVMIVD */
int32_t ril_response_nvdm_data_item_invalidate(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_nvdm_data_item_invalidate_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.status);
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


/* AT cmd: AT*MNVMRSTONE */
int32_t ril_response_nvdm_data_item_factory_reset(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_nvdm_data_item_factory_reset_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.status);
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


/* AT cmd: AT*MNVMRST */
int32_t ril_response_nvdm_factory_reset_all_data_item(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_nvdm_factory_reset_all_data_item_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.status);
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


/* AT cmd: AT*MNVMMDNQ */
int32_t ril_response_nvdm_query_num_of_mini_dump(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_nvdm_query_num_of_mini_dump_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.status);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.mini_dump_num);
            if (ret < 0) {
                ret = 0;
                response.mini_dump_num = RIL_OMITTED_INTEGER_PARAM;
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


/* AT cmd: AT*MNVMMDR */
int32_t ril_response_nvdm_read_mini_dump(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_nvdm_read_mini_dump_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.status);
            if (ret < 0) {
                break;
            }

            response.mini_dump_idx = RIL_OMITTED_INTEGER_PARAM;
            response.offset = RIL_OMITTED_INTEGER_PARAM;
            response.data_length = RIL_OMITTED_INTEGER_PARAM;
            response.data = RIL_OMITTED_STRING_PARAM;
            ret = at_tok_nextint(&line, &response.mini_dump_idx);
            if (ret < 0) {
                ret = 0;
            }

            ret = at_tok_nextint(&line, &response.offset);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.data_length);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &response.data);
            if (ret < 0) {
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


/* AT cmd: AT*MNVMMDC */
int32_t ril_response_nvdm_clean_mini_dump(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_nvdm_clean_mini_dump_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.status);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.mini_dump_idx);
            if (ret < 0) {
                ret = 0;
                response.mini_dump_idx = RIL_OMITTED_INTEGER_PARAM;
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


/* AT cmd: AT+IDCFREQ */
int32_t ril_response_set_idc_frequency_range(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT cmd: AT+IDCPWRBACKOFF */
int32_t ril_response_set_tx_power_back_off(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{    
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT cmd: AT+IDCTX2GPS */
int32_t ril_response_generate_periodic_tx_for_gps(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{    
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT cmd: AT*MCALDEV */
int32_t ril_response_enter_exit_rf_calibration_state(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_enter_exit_rf_calibration_state_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.caldev_state);
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


/* AT cmd: AT*MATWAKEUP */
int32_t ril_response_set_modem_wakeup_indication(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT cmd: AT*MBAND */
int32_t ril_response_query_modem_operating_band(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_query_modem_operating_band_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.current_band);
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


/* AT cmd: AT*MENGINFO */
int32_t ril_response_query_network_state(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_query_network_state_rsp_t response;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;
    int32_t display_mode = 0;
    bool first_line = true;
    ril_query_network_state_serving_cell_info_t sc_info;
    ril_query_network_state_data_transfer_info_t data_transfer_info;    
    ril_query_network_state_neighbor_cell_info_t *entry, *list_head;
    int32_t message_num = 0;
    int32_t *temp = NULL;
    int32_t i;

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

    temp = (int32_t *)&sc_info;
    for (i = 0; i < sizeof(ril_query_network_state_serving_cell_info_t)/sizeof(int32_t); i++) {
        temp[i] = RIL_OMITTED_INTEGER_PARAM;
    }
    sc_info.sc_cellid = RIL_OMITTED_STRING_PARAM;
    sc_info.sc_tac = RIL_OMITTED_STRING_PARAM;

    temp = (int32_t *)&data_transfer_info;
    for (i = 0; i < sizeof(ril_query_network_state_data_transfer_info_t)/sizeof(int32_t); i++) {
        temp[i] = RIL_OMITTED_INTEGER_PARAM;
    }
    
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (first_line) {
                if (str_starts_with(line, "*MENGINFOSC")) {
                    display_mode = 0;
                } else if (str_starts_with(line, "*MENGINFODT")) {
                    display_mode = 1;
                } else {
                    /* invalid format */
                    ret = -1;
                    break;
                }
            }

            if (display_mode == 0) {
                ret = at_tok_start(&line);
                if (ret < 0) {
                    break;
                }

                if (first_line) {
                    ret = at_tok_nextint(&line, &sc_info.sc_earfcn);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line, &sc_info.sc_earfcn_offset);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line, &sc_info.sc_pci);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextstr(&line, &sc_info.sc_cellid);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line, &sc_info.sc_rsrp);
                    if (ret < 0) {
                        //break;
                    }

                    ret = at_tok_nextint(&line, &sc_info.sc_rsrq);
                    if (ret < 0) {
                        //break;
                    }

                    ret = at_tok_nextint(&line, &sc_info.sc_rssi);
                    if (ret < 0) {
                        //break;
                    }

                    ret = at_tok_nextint(&line, &sc_info.sc_snr);
                    if (ret < 0) {
                        //break;
                    }

                    ret = at_tok_nextint(&line, &sc_info.sc_band);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextstr(&line, &sc_info.sc_tac);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line, &sc_info.sc_ecl);
                    if (ret < 0) {
                        ret = 0;
                    }

                    ret = at_tok_nextint(&line, &sc_info.sc_tx_pwr);
                    if (ret < 0) {
                        ret = 0;
                    }
                } else {
                    if (message_num == 0) {
                        list_head = (ril_query_network_state_neighbor_cell_info_t *)ril_mem_malloc(sizeof(ril_query_network_state_neighbor_cell_info_t));
                        entry = list_head;
                    } else {
                        list_head = ril_mem_realloc(list_head, sizeof(ril_query_network_state_neighbor_cell_info_t) * (message_num + 1));
                        entry = list_head + message_num;
                    }

                    ret = at_tok_nextint(&line, &entry->nc_earfcn);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line, &entry->nc_earfcn_offset);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line, &entry->nc_pci);
                    if (ret < 0) {
                        break;
                    }

                    ret = at_tok_nextint(&line, &entry->nc_rsrp);
                    if (ret < 0) {
                        break;
                    }

                    message_num++;
                }
            } else if (display_mode == 1) {
                ret = at_tok_start(&line);
                if (ret < 0) {
                    break;
                }
                
                ret = at_tok_nextint(&line, &data_transfer_info.RLC_UL_BLER);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &data_transfer_info.RLC_DL_BLER);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &data_transfer_info.MAC_UL_BLER);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &data_transfer_info.MAC_DL_BLER);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &data_transfer_info.MAC_UL_total_bytes);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &data_transfer_info.MAC_DL_total_bytes);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &data_transfer_info.MAC_UL_total_HARQ_TX);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &data_transfer_info.MAC_DL_total_HARQ_TX);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &data_transfer_info.MAC_UL_HARQ_re_TX);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &data_transfer_info.MAC_DL_HARQ_re_TX);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &data_transfer_info.RLC_UL_tput);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &data_transfer_info.RLC_DL_tput);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &data_transfer_info.MAC_UL_tput);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &data_transfer_info.MAC_DL_tput);
                if (ret < 0) {
                    break;
                }
            }
        }
        
        first_line = false;
        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    response.sc_info = (display_mode == 0 ? &sc_info : NULL);
    response.nc_num = message_num;
    response.nc_info = (display_mode == 0 ? list_head : NULL);
    response.data_transfer_info = (display_mode == 1 ? &data_transfer_info : NULL);

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }

    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}


/* AT cmd: AT*MFRCLLCK */
int32_t ril_response_frequency_and_cell_lock(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT cmd: AT*MSPCHSC */
int32_t ril_response_set_scrambling_algorithm_for_npdsch(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_set_scrambling_algorithm_for_npdsch_rsp_t response;
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


/* AT cmd: AT*MDPDNP */
int32_t ril_response_default_pdn_parameter(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_default_pdn_parameter_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.n);
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


/* AT cmd: AT*MEDRXCFG */
int32_t ril_response_eDRX_configuration(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_eDRX_configuration_rsp_t response;
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

            response.act_type = RIL_OMITTED_INTEGER_PARAM;
            response.requested_eDRX_value = RIL_OMITTED_STRING_PARAM;
            response.requested_paging_time_window_value = RIL_OMITTED_STRING_PARAM;
            ret = at_tok_nextint(&line, &response.act_type);
            if (ret < 0) {
                break;
            }
            ret = at_tok_nextstr(&line, &response.requested_eDRX_value);
            if (ret < 0) {
                break;
            }
            ret = at_tok_nextstr(&line, &response.requested_paging_time_window_value);
            if (ret < 0) {
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


/* AT cmd: AT*MCELLINFO */
int32_t ril_response_serving_and_neighbor_cell_info(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_serving_and_neighbor_cell_info_rsp_t response;
    int32_t ret = -1;
    bool need_parse = true;
    ril_result_code_t res_code = RIL_RESULT_CODE_NULL;
    int32_t display_mode = 0;
    bool first_line = true;
    ril_serving_and_neighbor_cell_info_t sc_info;
    ril_serving_and_neighbor_cell_info_t *entry, *list_head;
    int32_t message_num = 0;
    int32_t *temp = NULL;
    int32_t i;

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

    temp = (int32_t *)&sc_info;
    for (i = 0; i < sizeof(ril_query_network_state_serving_cell_info_t)/sizeof(int32_t); i++) {
        temp[i] = RIL_OMITTED_INTEGER_PARAM;
    }
    sc_info.cellid = RIL_OMITTED_STRING_PARAM;
    sc_info.tac = RIL_OMITTED_STRING_PARAM;
    
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (first_line) {
                if (str_starts_with(line, "*MCELLINFOSC")) {
                    display_mode = 0;
                } else if (str_starts_with(line, "*MCELLINFONC")) {
                    display_mode = 1;
                } else {
                    /* invalid format */
                    ret = -1;
                    break;
                }
            }

            if (display_mode == 0) {
                entry = &sc_info;
            } else { /* display mode == 1 */
                if (message_num == 0) {
                    list_head = (ril_serving_and_neighbor_cell_info_t *)ril_mem_malloc(sizeof(ril_serving_and_neighbor_cell_info_t));
                    entry = list_head;
                } else {
                    list_head = ril_mem_realloc(list_head, sizeof(ril_serving_and_neighbor_cell_info_t) * (message_num + 1));
                    entry = list_head + message_num;
                }
            }

            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }
            
            ret = at_tok_nextint(&line, &entry->earfcn);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->earfcn_offset);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->pci);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->rsrp);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->rsrq);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->rssi);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->sinr);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->mcc);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->mnc);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->tac);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->cellid);
            if (ret < 0) {
                break;
            }

            if (display_mode == 1) {
                message_num++;
            }
        }
        
        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    response.sc_info = &sc_info;
    response.nc_num = message_num;
    response.nc_info_list = list_head;

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }

    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}


/* AT command: AT*MUPDIR */
int32_t ril_response_indicate_packet_to_track(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}
/*************************************************
 *                 URC handlr
 *************************************************/

/* URC: *MLTS */
int32_t ril_urc_get_local_timestamp_and_network_info(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_get_local_timestamp_and_network_info_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    memset(&response, 0x00, sizeof(ril_get_local_timestamp_and_network_info_urc_t));
    response.coding_scheme_f = RIL_OMITTED_INTEGER_PARAM;
    response.coding_scheme_s = RIL_OMITTED_INTEGER_PARAM;
    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextstr(&line, &response.tds);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextstr(&line, &response.dst);
        if (ret < 0) {
            ret = 0;
            break;
        }

        ret = at_tok_nextstr(&line, &response.full_name);
        if (ret < 0) {
            ret = 0;
            break;
        }
        ret = at_tok_nextint(&line, &response.coding_scheme_f);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextstr(&line, &response.short_name);
        if (ret < 0) {
            ret = 0;
            break;
        }
        ret = at_tok_nextint(&line, &response.coding_scheme_s);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextstr(&line, &response.local_time_zone);
        if (ret < 0) {
            ret = 0;
            break;
        }

        ret = at_tok_nextstr(&line, &response.LSA_identify);
        if (ret < 0) {
            ret = 0;
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }
    return 0;
}


/* URC: *MSIMINS */
int32_t ril_urc_SIM_inserted_status_reporting(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_sim_inserted_status_reporting_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.inserted);
        if (ret < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }
    return 0;
}


/* URC: *MGCOUNT */
int32_t ril_urc_packet_domain_counters(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_packet_domain_counters_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        int32_t h32, l32;
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        if ((ret = at_tok_nextint(&line, &response.cid)) < 0) {
            break;
        }

        if ((ret = at_tok_nextint(&line, &h32)) < 0) {
            break;
        }
        if ((ret = at_tok_nextint(&line, &l32)) < 0) {
            break;
        }
        response.ul = ((long long)h32 << 32) + l32;

        if ((ret = at_tok_nextint(&line, &h32)) < 0) {
            break;
        }
        if ((ret = at_tok_nextint(&line, &l32)) < 0) {
            break;
        }
        response.dl = ((long long)h32 << 32) + l32;

        if ((ret = at_tok_nextstr(&line, &response.pdp_type)) < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }
    return 0;
}


/* AT URC: *MSMEXTRAUNSOL  */
int32_t ril_urc_control_extra_unsolicited_messages(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_control_extra_unsolicited_messages_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.unread_records);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.used_records);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.memory_exceeded);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.num_sms_records);
        if (ret < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }
    return 0;
}


/* AT URC: *MMGI */
int32_t ril_urc_control_sms_unsolicited_indication(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_control_sms_unsolicited_indication_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.event_id);
        if (ret < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }
    return 0;
}


/* AT URC: *MSMPUKBLKD */
int32_t ril_urc_sim_puk_blocked_unsolicited_indication(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_sim_puk_blocked_unsolicited_indication_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_nextstr(&line, &response.urc);
        if (ret < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }
    return 0;
}


/* AT URC: ^MODE */
int32_t ril_urc_indicate_system_mode(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_indicate_system_mode_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.sys_mode);
        if (ret < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }
    return 0;
}


/* AT URC: *MSTC */
int32_t ril_urc_STK_proactive_command_indication(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_STK_proactive_command_indication_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextstr(&line, &response.cmdid);
        if (ret < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }
    return 0;
}


/* AT URC: *MSTUD */
int32_t ril_urc_STK_unsolicited_data(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_STK_unsolicited_data_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextstr(&line, &response.cmdid);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextstr(&line, &response.data);
        if (ret < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }
    return 0;
}


/* AT URC: *MCCST */
int32_t ril_urc_STK_unsolicited_response_for_call_control(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_STK_unsolicited_response_for_call_control_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.cc_type);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.cc_event);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextstr(&line, &response.alpha_id);
        if (ret < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }
    return 0;
}


/* AT URC: *MCSIMLOCK */
int32_t ril_urc_lock_csim_access(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_lock_csim_access_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.state);
        if (ret < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }
    return 0;
}


/* AT URC: *MAPNURI */
int32_t ril_urc_apn_rate_control_indication(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_apn_rate_control_indication_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.cid);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.urc_active);
        if (ret < 0) {
            break;
        }

        response.additional_exception_reports = RIL_OMITTED_INTEGER_PARAM;
        response.uplink_time_unit = RIL_OMITTED_INTEGER_PARAM;
        response.maximum_uplink_rate = RIL_OMITTED_INTEGER_PARAM;

        ret = at_tok_nextint(&line, &response.additional_exception_reports);
        if (ret < 0) {
            ret = 0;
            break;
        }

        ret = at_tok_nextint(&line, &response.uplink_time_unit);
        if (ret < 0) {
            ret = 0;
            break;
        }

        ret = at_tok_nextint(&line, &response.maximum_uplink_rate);
        if (ret < 0) {
            ret = 0;
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }
    return 0;
}


/* AT URC: *MPLMNURI */
int32_t ril_urc_plmn_rate_control_indication(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_plmn_rate_control_indication_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.urc_active);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.serving_plmn_rate_control_value);
        if (ret < 0) {
            response.serving_plmn_rate_control_value = RIL_OMITTED_INTEGER_PARAM;
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }
    return 0;
}


/* AT URC: *MPDI */
int32_t ril_urc_packet_discard_indication(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_packet_discard_indication_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.status);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.cid);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.time);
        if (ret < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }
    return 0;
}


/* AT URC: +IDCSTATUS*/
int32_t ril_urc_set_idc_frequency_range(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_set_idc_frequency_range_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.tx_status);
        if (ret < 0) {
            break;
        }

        response.carrier_freq = RIL_OMITTED_INTEGER_PARAM;
        response.tx_power = RIL_OMITTED_INTEGER_PARAM;
        ret = at_tok_nextint(&line, &response.carrier_freq);
        if (ret < 0) {
            ret = 0;
            break;
        }

        ret = at_tok_nextint(&line, &response.tx_power);
        if (ret < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }
    return 0;
}


/* AT URC: *MSQN */
int32_t ril_urc_signal_quality_report(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_urc_propriteary_signal_quality_report_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.rssi);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.ber);
        if (ret < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }
    return 0;
}


/* AT URC: *MATWAKEUP */
int32_t ril_urc_set_modem_wakeup_indication(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
#ifndef MTK_MD_DEFERRED_WAKEUP_SUPPORT
    ril_init_channel_after_deep_sleep();
    mux_ap_resume_to_send();
#endif
    ril_notify_event(cmd_id, NULL, 0);
    //ril_general_setting();
    return 0;
}


/* AT URC: *MATREADY */
int32_t ril_urc_modem_ready_indication(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    //ril_general_setting();
    return 0;
}


/* AT URC: *MDPDNP */
int32_t ril_urc_default_pdn_parameter(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_default_pdn_parameter_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextstr(&line, &response.apn);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextstr(&line, &response.pdn_type);
        if (ret < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }
    return 0;
}


/* AT URC: *MUPDIR */
int32_t ril_urc_indicate_packet_to_track(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_indicate_packet_to_track_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.pattern_id);
        if (ret < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }
    return 0;
}


/* AT URC: *MUPDI */
int32_t ril_urc_indicate_packet_delivery_status(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    int32_t *msg_id_list = NULL;
    ril_indicate_packet_delivery_status_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        response.msg_id_list = NULL;
        ret = at_tok_nextint(&line, &response.pattern_id);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.status);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.num_msg_id);
        if (ret < 0) {
            break;
        }

        if (response.num_msg_id > 0) {
            msg_id_list = (int32_t *)ril_mem_malloc(response.num_msg_id * sizeof(int32_t));
            for (int32_t i = 0; i < response.num_msg_id; i++) {
                ret = at_tok_nextint(&line, &msg_id_list[i]);
                if (ret < 0) {
                    break;
                }
            }
            response.msg_id_list = msg_id_list;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(cmd_id, &response, sizeof(response));
    }

    if (msg_id_list != NULL) {
        ril_mem_free(msg_id_list);
    }
    return 0;
}
/*************************************************
 *                 UT test case
 *************************************************/
#if defined(__RIL_UT_TEST_CASES__)
int32_t ril_response_ut_callback_proprietary(ril_cmd_response_t *cmd_response)
{
    RIL_LOGUT("final result code: %d\r\n", cmd_response->res_code);
    RIL_LOGUT("request mode: %d\r\n", cmd_response->mode);
    if (cmd_response->mode == RIL_TEST_MODE &&
        cmd_response->test_mode_str != NULL &&
        cmd_response->test_mode_str_len > 0) {
        RIL_LOGDUMPSTRUT("test mode str: %s\r\n", cmd_response->test_mode_str_len, cmd_response->test_mode_str);
    } else {
        switch (cmd_response->cmd_id) {
            case RIL_CMD_ID_MLTS: {
                ril_get_local_timestamp_and_network_info_rsp_t *param = (ril_get_local_timestamp_and_network_info_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("enable: %d\r\n", (int)param->enable);
                }
            }
            break;

            case RIL_CMD_ID_MSIMINS: {
                ril_sim_inserted_status_reporting_rsp_t *param = (ril_sim_inserted_status_reporting_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("n: %d\r\n", (int)param->n);
                    RIL_LOGUT("inserted: %d\r\n", (int)param->inserted);
                }
            }
            break;

            case RIL_CMD_ID_MSPN: {
                ril_get_service_provider_name_from_sim_rsp_t *param = (ril_get_service_provider_name_from_sim_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("spn: %s\r\n", param->spn ? param->spn : UT_OMITTED_PARAM);
                    RIL_LOGUT("display_mode: %d\r\n", (int)param->display_mode);
                }
            }
            break;

            case RIL_CMD_ID_MUNSOL: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_MGCOUNT: {
                // no parameter.
            }
            break;

#ifndef __RIL_CMD_SET_SLIM_ENABLE__
            case RIL_CMD_ID_MGSINK: {
                // no parameter.
            }
            break;
            
            case RIL_CMD_ID_MCGDEFCONT: {
                ril_send_pdn_connection_set_default_psd_attach_rsp_t *param = (ril_send_pdn_connection_set_default_psd_attach_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("pdp_type: %s\r\n", param->pdp_type ? param->pdp_type : UT_OMITTED_PARAM);
                    RIL_LOGUT("apn: %s\r\n", param->apn ? param->apn : UT_OMITTED_PARAM);
                    RIL_LOGUT("username %s\r\n", param->username ? param->username : UT_OMITTED_PARAM);
                    RIL_LOGUT("password: %s\r\n", param->password ? param->password : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_MGTCSINK: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_MSACL: {           
                ril_control_ACL_feature_rsp_t *param = (ril_control_ACL_feature_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("supported: %d\r\n", (int)param->supported);
                    RIL_LOGUT("enabled: %d\r\n", (int)param->enabled);
                }
            }
            break;

            case RIL_CMD_ID_MLACL: {            
                ril_display_ACL_list_rsp_t *param = (ril_display_ACL_list_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("index: %d\r\n", (int)param->index);
                    RIL_LOGUT("APN: %s\r\n", param->APN ? param->APN : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_MWACL: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_MDACL: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_MSMEXTRAINFO: {            
                ril_control_extra_info_on_sms_rsp_t *param = (ril_control_extra_info_on_sms_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("enable: %d\r\n", (int)param->enable);
                }
            }
            break;

            case RIL_CMD_ID_MSMEXTRAUNSOL: {
                ril_control_extra_unsolicited_messages_rsp_t *param = (ril_control_extra_unsolicited_messages_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("enable: %d\r\n", (int)param->enable);
                }
            }
            break;

            case RIL_CMD_ID_MSMSTATUS: {
                ril_obtain_status_of_sms_functionality_rsp_t *param = (ril_obtain_status_of_sms_functionality_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("sms_status: %d\r\n", (int)param->sms_status);
                    RIL_LOGUT("unread_records: %d\r\n", (int)param->unread_records);
                    RIL_LOGUT("used_records: %d\r\n", (int)param->used_records);
                    RIL_LOGUT("memory_exceeded: %d\r\n", (int)param->memory_exceeded);
                    RIL_LOGUT("num_sms_records: %d\r\n", (int)param->num_sms_records);
                    RIL_LOGUT("first_free_record: %d\r\n", (int)param->first_free_record);
                    RIL_LOGUT("num_smsp_records: %d\r\n", (int)param->num_smsp_records);
                    RIL_LOGUT("service: %d\r\n", (int)param->service);
                }
            }
            break;

            case RIL_CMD_ID_MMGI: {
                ril_control_sms_unsolicited_indication_rsp_t *param = (ril_control_sms_unsolicited_indication_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    int32_t i;
                    RIL_LOGUT("entry_num: %d\r\n", (int)param->entry_num);
                    for (i = 0 ; i < param->entry_num; i++) {
                        RIL_LOGUT("entry#%d, event_id: %d\r\n", (int)(i + 1), (int)param->param_list[i].event_id);
                        RIL_LOGUT("entry#%d, status: %d\r\n", (int)(i + 1), (int)param->param_list[i].status);
                    }
                }
            }
            break;

            case RIL_CMD_ID_MMGRW: {
                ril_sms_location_rewrite_rsp_t *param = (ril_sms_location_rewrite_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("loc: %d\r\n", (int)param->loc);
                }
            }
            break;

            case RIL_CMD_ID_MMGSC_PDU:
            case RIL_CMD_ID_MMGSC_TXT: {
                // no parameter.
            }
            break;
#endif

            case RIL_CMD_ID_MUPIN: {            
                ril_uicc_pin_information_access_rsp_t *param = (ril_uicc_pin_information_access_rsp_t *)cmd_response->cmd_param;
                if ((cmd_response->mode == RIL_READ_MODE || cmd_response->mode == RIL_EXECUTE_MODE) && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    int32_t i;
                    RIL_LOGUT("entry_num: %d\r\n", (int)param->entry_num);
                    for (i = 0 ; i < param->entry_num; i++) {
                        RIL_LOGUT("entry#%d, access_level: %s\r\n", (int)(i + 1), param->param_list[i].access_level ? param->param_list[i].access_level : UT_OMITTED_PARAM);
                        RIL_LOGUT("entry#%d, pin_status: %d\r\n", (int)(i + 1), (int)param->param_list[i].pin_status);
                        RIL_LOGUT("entry#%d, pin_verification_state: %s\r\n", (int)(i + 1), param->param_list[i].pin_verification_state ? param->param_list[i].pin_verification_state : UT_OMITTED_PARAM);
                        RIL_LOGUT("entry#%d, pin_retry_counter: %d\r\n", (int)(i + 1), (int)param->param_list[i].pin_retry_counter);
                        RIL_LOGUT("entry#%d, puk_retry_counter: %d\r\n", (int)(i + 1), (int)param->param_list[i].puk_retry_counter);
                        RIL_LOGUT("entry#%d, key_reference: %s\r\n", (int)(i + 1), param->param_list[i].key_reference ? param->param_list[i].key_reference : UT_OMITTED_PARAM);
                    }

                }
            }
            break;

            case RIL_CMD_ID_MUAPP: {
                ril_uicc_application_list_access_rsp_t *param = (ril_uicc_application_list_access_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    int32_t i;
                    RIL_LOGUT("entry_num: %d\r\n", (int)param->entry_num);
                    for (i = 0 ; i < param->entry_num; i++) {
                        RIL_LOGUT("entry#%d, index: %d\r\n", (int)(i + 1), (int)param->param_list[i].index);
                        RIL_LOGUT("entry#%d, application_code: %d\r\n", (int)(i + 1), (int)param->param_list[i].application_code);
                        RIL_LOGUT("entry#%d, application_state: %d\r\n", (int)(i + 1), (int)param->param_list[i].application_state);
                        RIL_LOGUT("entry#%d, application_label: %s\r\n", (int)(i + 1), param->param_list[i].application_label ? param->param_list[i].application_label : UT_OMITTED_PARAM);
                    }

                }
            }
            break;

            case RIL_CMD_ID_MSST: {
                ril_read_sst_ust_from_usim_rsp_t *param = (ril_read_sst_ust_from_usim_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("stt: %d\r\n", (int)param->stt);
                    RIL_LOGUT("sst_bitmap_allocated: %s\r\n", param->sst_bitmap_allocated ? param->sst_bitmap_allocated : UT_OMITTED_PARAM);
                    RIL_LOGUT("sst_bitmap_activated: %s\r\n", param->sst_bitmap_activated ? param->sst_bitmap_activated : UT_OMITTED_PARAM);
                    RIL_LOGUT("ust_bitmap_available: %s\r\n", param->ust_bitmap_available ? param->ust_bitmap_available : UT_OMITTED_PARAM);
                    RIL_LOGUT("est_bitmap: %s\r\n", param->est_bitmap ? param->est_bitmap : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_MABORT: {
                // no parameter.
            }
            break;       

#ifndef __RIL_CMD_SET_SLIM_ENABLE__
            case RIL_CMD_ID_MCAL: {
                ril_radio_call_rsp_t *param = (ril_radio_call_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    int32_t i;
                    RIL_LOGUT("cmd_group: %s\r\n", param->cmd_group ? param->cmd_group : UT_OMITTED_PARAM);
                    RIL_LOGUT("item_index: %d\r\n", (int)param->item_index);
                    RIL_LOGUT("function_index: %d\r\n", (int)param->function_index);
                    RIL_LOGUT("status_code: %d\r\n", (int)param->status_code);
                    RIL_LOGUT("param_count: %d\r\n", (int)param->param_count);
                    RIL_LOGUT("param_array:\r\n");
                    for (i = 0; i < RIL_RADIO_CALL_REQ_MAX_PARAM_COUNT; i++) {
                        RIL_LOGUT("%d%s", (int)param->param_array[i], (i == (RIL_RADIO_CALL_REQ_MAX_PARAM_COUNT - 1)) ? "\r\n" : " ");
                    }
                    RIL_LOGUT("check_sum: %d\r\n", (int)param->check_sum);
                }
            }
            break;

            case RIL_CMD_ID_MNON: {
                ril_network_operator_name_rsp_t *param = (ril_network_operator_name_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    int32_t i;
                    RIL_LOGUT("entry_num: %d\r\n", (int)param->entry_num);
                    for (i = 0 ; i < param->entry_num; i++) {
                        RIL_LOGUT("entry#%d, index: %d\r\n", (int)(i + 1), (int)param->param_list[i].index);
                        RIL_LOGUT("entry#%d, operator_name: %s\r\n", (int)(i + 1), param->param_list[i].operator_name ? param->param_list[i].operator_name : UT_OMITTED_PARAM);
                        RIL_LOGUT("entry#%d, short_form: %s\r\n", (int)(i + 1), param->param_list[i].short_form ? param->param_list[i].short_form : UT_OMITTED_PARAM);
                        RIL_LOGUT("entry#%d, plmn_add_info: %s\r\n", (int)(i + 1), param->param_list[i].plmn_add_info ? param->param_list[i].plmn_add_info : UT_OMITTED_PARAM);
                    }
                }
            }
            break;

            case RIL_CMD_ID_MOPL: {
                ril_network_operator_plmn_list_rsp_t *param = (ril_network_operator_plmn_list_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    int32_t i;
                    RIL_LOGUT("entry_num: %d\r\n", (int)param->entry_num);
                    for (i = 0 ; i < param->entry_num; i++) {
                        RIL_LOGUT("entry#%d, index: %d\r\n", (int)(i + 1), (int)param->param_list[i].index);
                        RIL_LOGUT("entry#%d, mcc_mnc: %s\r\n", (int)(i + 1), param->param_list[i].mcc_mnc ? param->param_list[i].mcc_mnc : UT_OMITTED_PARAM);
                        RIL_LOGUT("entry#%d, lac_range: %s\r\n", (int)(i + 1), param->param_list[i].lac_range ? param->param_list[i].lac_range : UT_OMITTED_PARAM);
                        RIL_LOGUT("entry#%d, pnn_id: %d\r\n", (int)(i + 1), (int)param->param_list[i].pnn_id);
                    }
                }
            }
            break;

            case RIL_CMD_ID_MMUX: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_MROUTEMMI: {
                ril_mmi_at_channel_routing_configuration_rsp_t *param = (ril_mmi_at_channel_routing_configuration_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    int32_t i;
                    RIL_LOGUT("entry_num: %d\r\n", (int)param->entry_num);
                    for (i = 0 ; i < param->entry_num; i++) {
                        RIL_LOGUT("entry#%d, option_id: %d\r\n", (int)(i + 1), (int)param->param_list[i].option_id);
                        RIL_LOGUT("entry#%d, option_value: %d\r\n", (int)(i + 1), (int)param->param_list[i].option_value);
                        RIL_LOGUT("entry#%d, list_of_id: %s\r\n", (int)(i + 1), param->param_list[i].list_of_id ? param->param_list[i].list_of_id : UT_OMITTED_PARAM);
                    }
                }
            }
            break;

            case RIL_CMD_ID_MCEERMODE: {
                ril_ceer_response_mode_rsp_t *param = (ril_ceer_response_mode_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("mode: %d\r\n", (int)param->mode);
                }
            }
            break;

            case RIL_CMD_ID_MFTRCFG: {
                ril_modem_feature_configuration_rsp_t *param = (ril_modem_feature_configuration_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    int32_t i;
                    RIL_LOGUT("entry_num: %d\r\n", (int)param->entry_num);
                    for (i = 0 ; i < param->entry_num; i++) {
                        RIL_LOGUT("entry#%d, modem_feature: %d\r\n", (int)(i + 1), (int)param->param_list[i].modem_feature);
                        RIL_LOGUT("entry#%d, feature_value: %d\r\n", (int)(i + 1), (int)param->param_list[i].feature_value);
                        RIL_LOGUT("entry#%d, need_reboot: %d\r\n", (int)(i + 1), (int)param->param_list[i].need_reboot);
                    }
                }
            }
            break;
#endif

            case RIL_CMD_ID_HVER: {
                ril_request_hw_version_rsp_t *param = (ril_request_hw_version_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_ACTIVE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("hw_version: %s\r\n", param->hw_version ? param->hw_version : UT_OMITTED_PARAM);
                }
            }
            break;

#ifndef __RIL_CMD_SET_SLIM_ENABLE__
            case RIL_CMD_ID_MODE: {            
                ril_indicate_system_mode_rsp_t *param = (ril_indicate_system_mode_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("enable: %d\r\n", (int)param->enable);
                    RIL_LOGUT("sys_mode: %d\r\n", (int)param->sys_mode);
                }
            }
            break;

            case RIL_CMD_ID_SYSINFO: {            
                ril_request_system_information_rsp_t *param = (ril_request_system_information_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("srv_status: %d\r\n", (int)param->srv_status);
                    RIL_LOGUT("srv_domain: %d\r\n", (int)param->srv_domain);
                    RIL_LOGUT("roam_status: %d\r\n", (int)param->roam_status);
                    RIL_LOGUT("sys_mode: %d\r\n", (int)param->sys_mode);
                    RIL_LOGUT("sim_state: %d\r\n", (int)param->sim_state);
                    RIL_LOGUT("reserve: %d\r\n", (int)param->reserve);
                }
            }
            break;

            case RIL_CMD_ID_SYSCONFIG: {
                ril_configure_system_reference_rsp_t *param = (ril_configure_system_reference_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("mode: %d\r\n", (int)param->mode);
                    RIL_LOGUT("acqorder: %d\r\n", (int)param->acqorder);
                    RIL_LOGUT("roam: %d\r\n", (int)param->roam);
                    RIL_LOGUT("srvdomain: %d\r\n", (int)param->srvdomain);
                }
            }
            break;

            case RIL_CMD_ID_CARDMODE: {            
                ril_request_sim_usim_mode_rsp_t *param = (ril_request_sim_usim_mode_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("sim_type: %d\r\n", (int)param->sim_type);
                }
            }
            break;

            case RIL_CMD_ID_SPN: {
                ril_read_service_provider_name_rsp_t *param = (ril_read_service_provider_name_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("disp_rplmn: %d\r\n", (int)param->disp_rplmn);
                    RIL_LOGUT("coding: %d\r\n", (int)param->coding);
                    RIL_LOGUT("spn_name: %d\r\n", (int)param->spn_name);
                }
            }
            break;

            case RIL_CMD_ID_MSTLOCK: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_MSTPD: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_MSTMODE: {
                ril_STK_output_format_setting_rsp_t *param = (ril_STK_output_format_setting_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("output_mode: %d\r\n", (int)param->output_mode);
                }
            }
            break;

            case RIL_CMD_ID_MSTICREC: {
                ril_STK_obtain_icon_record_rsp_t *param = (ril_STK_obtain_icon_record_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    if (cmd_response->mode == RIL_READ_MODE) {
                        RIL_LOGUT("num_rec: %d\r\n", (int)param->num_rec);
                    } else if (cmd_response->mode == RIL_EXECUTE_MODE) {
                        int32_t i;
                        RIL_LOGUT("num_rec: %d\r\n", (int)param->num_rec);
                        RIL_LOGUT("num_instances: %d\r\n", (int)param->num_instances);
                        for (i = 0 ; i < param->num_instances; i++) {
                            RIL_LOGUT("entry#%d, width: %d\r\n", (int)(i + 1), (int)param->instances_list[i].width);
                            RIL_LOGUT("entry#%d, height: %d\r\n", (int)(i + 1), (int)param->instances_list[i].height);
                            RIL_LOGUT("entry#%d, cs: %d\r\n", (int)(i + 1), (int)param->instances_list[i].cs);
                            RIL_LOGUT("entry#%d, efid: %d\r\n", (int)(i + 1), (int)param->instances_list[i].efid);
                            RIL_LOGUT("entry#%d, offset: %d\r\n", (int)(i + 1), (int)param->instances_list[i].offset);
                            RIL_LOGUT("entry#%d, length: %d\r\n", (int)(i + 1), (int)param->instances_list[i].length);
                        }
                    }
                }
            }
            break;
            
            case RIL_CMD_ID_MSTICIMG: {
                ril_STK_obtain_icon_image_rsp_t *param = (ril_STK_obtain_icon_image_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("length: %d\r\n", (int)param->length);
                    RIL_LOGUT("data: %s\r\n", param->data ? param->data : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_MSTGC: {
                ril_STK_parameters_associated_with_proactive_command_rsp_t *param = (ril_STK_parameters_associated_with_proactive_command_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("cmdid: %s\r\n", param->cmdid ? param->cmdid : UT_OMITTED_PARAM);
                    RIL_LOGUT("data: %s\r\n", param->data ? param->data : UT_OMITTED_PARAM);
                }
            }
            break;
            
            case RIL_CMD_ID_MSTCR: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_MSTMS: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_MSTEV: {
                // no parameter.
            }
            break;
#endif

            case RIL_CMD_ID_MICCID: {
                ril_read_usim_iccid_rsp_t *param = (ril_read_usim_iccid_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_ACTIVE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("iccid: %s\r\n", param->iccid ? param->iccid : UT_OMITTED_PARAM);
                }
            }
            break;
            
            case RIL_CMD_ID_MHOMENW: {
                ril_display_home_network_information_rsp_t *param = (ril_display_home_network_information_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_ACTIVE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("oper_long_alpha: %s\r\n", param->oper_long_alpha ? param->oper_long_alpha : UT_OMITTED_PARAM);
                    RIL_LOGUT("oper_short_alpha: %s\r\n", param->oper_short_alpha ? param->oper_short_alpha : UT_OMITTED_PARAM);
                    RIL_LOGUT("oper_numeric: %d\r\n", (int)param->oper_numeric);
                }
            }
            break;

#ifndef __RIL_CMD_SET_SLIM_ENABLE__
            case RIL_CMD_ID_MCSIMLOCK: {
                ril_lock_csim_access_rsp_t *param = (ril_lock_csim_access_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("lock_state: %d\r\n", (int)param->lock_state);
                    RIL_LOGUT("other_lock_state: %d\r\n", (int)param->other_lock_state);
                }
            }
            break;

            case RIL_CMD_ID_MEMMREEST: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_MAPNURI: {
                ril_apn_rate_control_indication_rsp_t *param = (ril_apn_rate_control_indication_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    int32_t i;
                    RIL_LOGUT("entry_num: %d\r\n", (int)param->entry_num);
                    for (i = 0 ; i < param->entry_num; i++) {
                        RIL_LOGUT("entry#%d, mode: %d\r\n", (int)(i + 1), (int)param->param_list[i].mode);
                        RIL_LOGUT("entry#%d, cid: %d\r\n", (int)(i + 1), (int)param->param_list[i].cid);
                        RIL_LOGUT("entry#%d, urc_active: %d\r\n", (int)(i + 1), (int)param->param_list[i].urc_active);
                        RIL_LOGUT("entry#%d, additional_exception_reports: %d\r\n", (int)(i + 1), (int)param->param_list[i].additional_exception_reports);
                        RIL_LOGUT("entry#%d, uplink_time_unit: %d\r\n", (int)(i + 1), (int)param->param_list[i].uplink_time_unit);
                        RIL_LOGUT("entry#%d, maximum_uplink_rate: %d\r\n", (int)(i + 1), (int)param->param_list[i].maximum_uplink_rate);
                    }
                }
            }
            break;

            case RIL_CMD_ID_MPLMNURI: {
                ril_plmn_rate_control_indication_rsp_t *param = (ril_plmn_rate_control_indication_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("mode: %d\r\n", (int)param->mode);
                    RIL_LOGUT("urc_active: %d\r\n", (int)param->urc_active);
                    RIL_LOGUT("serving_plmn_rate_control_value: %d\r\n", (int)param->serving_plmn_rate_control_value);
                }
            }
            break;

            case RIL_CMD_ID_MPDI: {
                ril_packet_discard_indication_rsp_t *param = (ril_packet_discard_indication_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("mode: %d\r\n", (int)param->mode);
                }
            }
            break;

            case RIL_CMD_ID_MNBIOTDT: {
                ril_nbiot_data_type_rsp_t *param = (ril_nbiot_data_type_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    int32_t i;
                    RIL_LOGUT("entry_num: %d\r\n", (int)param->entry_num);
                    for (i = 0 ; i < param->entry_num; i++) {
                        RIL_LOGUT("entry#%d, cid: %d\r\n", (int)(i + 1), (int)param->param_list[i].cid);
                        RIL_LOGUT("entry#%d, type: %d\r\n", (int)(i + 1), (int)param->param_list[i].type);
                    }
                }
            }
            break;

            case RIL_CMD_ID_MNBIOTRAI: {
                ril_nbiot_release_assistance_indication_rsp_t *param = (ril_nbiot_release_assistance_indication_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("rai: %d\r\n", (int)param->rai);
                }
            }
            break;

            case RIL_CMD_ID_MNVMQ: {
                ril_nvdm_status_query_rsp_t *param = (ril_nvdm_status_query_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("chip_name: %s\r\n", param->chip_name ? param->chip_name : UT_OMITTED_PARAM);
                    RIL_LOGUT("needs_security_authentication: %d\r\n", (int)param->needs_security_authentication);
                }
            }
            break;

            case RIL_CMD_ID_MNVMAUTH: {
                ril_nvdm_access_security_authorization_rsp_t *param = (ril_nvdm_access_security_authorization_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("auth_result: %d\r\n", (int)param->auth_result);
                }
            }
            break;

            case RIL_CMD_ID_MNVMW: {
                ril_nvdm_data_write_rsp_t *param = (ril_nvdm_data_write_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("write_status: %d\r\n", (int)param->write_status);
                }
            }
            break;

            case RIL_CMD_ID_MNVMR: {            
                ril_nvdm_data_read_rsp_t *param = (ril_nvdm_data_read_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("read_status: %d\r\n", (int)param->read_status);
                    RIL_LOGUT("area_info: %d\r\n", (int)param->area_info);
                    RIL_LOGUT("group_id: %s\r\n", param->group_id ? param->group_id : UT_OMITTED_PARAM);
                    RIL_LOGUT("data_item_id: %s\r\n", param->data_item_id ? param->data_item_id : UT_OMITTED_PARAM);
                    RIL_LOGUT("length: %d\r\n", (int)param->length);
                    RIL_LOGUT("data: %s\r\n", param->data ? param->data : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_MNVMGET: {
                ril_nvdm_get_all_data_item_id_rsp_t *param = (ril_nvdm_get_all_data_item_id_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    int32_t i;
                    RIL_LOGUT("entry_num: %d\r\n", (int)param->entry_num);
                    for (i = 0 ; i < param->entry_num; i++) {
                        RIL_LOGUT("entry#%d, area_info: %d\r\n", (int)(i + 1), (int)param->param_list[i].area_info);
                        RIL_LOGUT("entry#%d, group_id %s\r\n", (int)i + 1, param->param_list[i].group_id ? param->param_list[i].group_id : UT_OMITTED_PARAM);
                        RIL_LOGUT("entry#%d, data_item_id %s\r\n", (int)i + 1, param->param_list[i].data_item_id ? param->param_list[i].data_item_id : UT_OMITTED_PARAM);
                    }
                }
            }
            break;

            case RIL_CMD_ID_MNVMIVD: {
                ril_nvdm_data_item_invalidate_rsp_t *param = (ril_nvdm_data_item_invalidate_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("status: %d\r\n", (int)param->status);
                }
            }
            break;

            case RIL_CMD_ID_MNVMRSTONE: {
                ril_nvdm_data_item_factory_reset_rsp_t *param = (ril_nvdm_data_item_factory_reset_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("status: %d\r\n", (int)param->status);
                }
            }
            break;

            case RIL_CMD_ID_MNVMRST: {
                ril_nvdm_factory_reset_all_data_item_rsp_t *param = (ril_nvdm_factory_reset_all_data_item_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("status: %d\r\n", (int)param->status);
                }
            }
            break;
            
            case RIL_CMD_ID_MNVMMDNQ: {
                ril_nvdm_query_num_of_mini_dump_rsp_t *param = (ril_nvdm_query_num_of_mini_dump_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("status: %d\r\n", (int)param->status);
                    RIL_LOGUT("mini_dump_num: %d\r\n", (int)param->mini_dump_num);
                }
            }
            break;

            case RIL_CMD_ID_MNVMMDR: {
                ril_nvdm_read_mini_dump_rsp_t *param = (ril_nvdm_read_mini_dump_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("status: %d\r\n", (int)param->status);
                    RIL_LOGUT("mini_dump_idx: %d\r\n", (int)param->mini_dump_idx);
                    RIL_LOGUT("offset: %d\r\n", (int)param->offset);
                    RIL_LOGUT("data_length: %d\r\n", (int)param->data_length);
                    RIL_LOGUT("data: %s\r\n", param->data ? param->data : UT_OMITTED_PARAM);
                }
            }
            break;
            
            case RIL_CMD_ID_MNVMMDC: {
                ril_nvdm_clean_mini_dump_rsp_t *param = (ril_nvdm_clean_mini_dump_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("status: %d\r\n", (int)param->status);
                    RIL_LOGUT("mini_dump_idx: %d\r\n", (int)param->mini_dump_idx);
                }
            }
            break;

            case RIL_CMD_ID_IDCFREQ: {
                // no parameter.            
            }
            break;

            case RIL_CMD_ID_IDCPWRBACKOFF: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_IDCTX2GPS: {
                // no parameter.
            }
            break;
#endif

            case RIL_CMD_ID_MCALDEV: {
                ril_enter_exit_rf_calibration_state_rsp_t *param = (ril_enter_exit_rf_calibration_state_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("caldev_state: %d\r\n", (int)param->caldev_state);
                }
            }
            break;
            
            case RIL_CMD_ID_MATWAKEUP: {            
            }
            break;

            case RIL_CMD_ID_MBAND: {
                ril_query_modem_operating_band_rsp_t *param = (ril_query_modem_operating_band_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("current_band: %d\r\n", (int)param->current_band);
                }
            }
            break;

            case RIL_CMD_ID_MENGINFO: {
                ril_query_network_state_rsp_t *param = (ril_query_network_state_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    if (param->sc_info != NULL) {
                        int32_t i;
                        RIL_LOGUT("serving cell info:\r\n");
                        RIL_LOGUT("sc_earfcn: %d\r\n", (int)param->sc_info->sc_earfcn);
                        RIL_LOGUT("sc_earfcn_offset: %d\r\n", (int)param->sc_info->sc_earfcn_offset);
                        RIL_LOGUT("sc_pci: %d\r\n", (int)param->sc_info->sc_pci);
                        RIL_LOGUT("sc_cellid: %s\r\n", param->sc_info->sc_cellid ? param->sc_info->sc_cellid : UT_OMITTED_PARAM);
                        RIL_LOGUT("sc_rsrp: %d\r\n", (int)param->sc_info->sc_rsrp);
                        RIL_LOGUT("sc_rsrq: %d\r\n", (int)param->sc_info->sc_rsrq);
                        RIL_LOGUT("sc_rssi: %d\r\n", (int)param->sc_info->sc_rssi);
                        RIL_LOGUT("sc_snr: %d\r\n", (int)param->sc_info->sc_snr);
                        RIL_LOGUT("sc_band: %d\r\n", (int)param->sc_info->sc_band);
                        RIL_LOGUT("sc_tac: %s\r\n", param->sc_info->sc_tac ? param->sc_info->sc_tac : UT_OMITTED_PARAM);
                        RIL_LOGUT("sc_ecl: %d\r\n", (int)param->sc_info->sc_ecl);
                        RIL_LOGUT("sc_tx_pwr: %d\r\n", (int)param->sc_info->sc_tx_pwr);

                        RIL_LOGUT("neighbor cell num: %d\r\n", (int)param->nc_num);
                        for (i = 0; i < param->nc_num; i++) {
                            RIL_LOGUT("entry#%d, nc_earfcn %d\r\n", (int)(i + 1), (int)param->nc_info[i].nc_earfcn);
                            RIL_LOGUT("entry#%d, nc_earfcn_offset %d\r\n", (int)(i + 1), (int)param->nc_info[i].nc_earfcn_offset);
                            RIL_LOGUT("entry#%d, nc_pci %d\r\n", (int)(i + 1), (int)param->nc_info[i].nc_pci);
                            RIL_LOGUT("entry#%d, nc_rsrp %d\r\n", (int)(i + 1), (int)param->nc_info[i].nc_rsrp);
                        }
                    } else if (param->data_transfer_info != NULL) {
                        RIL_LOGUT("data transfer info:\r\n");
                        RIL_LOGUT("RLC_UL_BLER: %d\r\n", (int)param->data_transfer_info->RLC_UL_BLER);
                        RIL_LOGUT("RLC_DL_BLER: %d\r\n", (int)param->data_transfer_info->RLC_DL_BLER);
                        RIL_LOGUT("MAP_UL_BLER: %d\r\n", (int)param->data_transfer_info->MAC_UL_BLER);
                        RIL_LOGUT("MAP_DL_BLER: %d\r\n", (int)param->data_transfer_info->MAC_DL_BLER);
                        RIL_LOGUT("MAC_UL_total_bytes: %d\r\n", (int)param->data_transfer_info->MAC_UL_total_bytes);
                        RIL_LOGUT("MAC_DL_total_bytes: %d\r\n", (int)param->data_transfer_info->MAC_DL_total_bytes);
                        RIL_LOGUT("MAC_UL_total_HARQ_TX: %d\r\n", (int)param->data_transfer_info->MAC_UL_total_HARQ_TX);
                        RIL_LOGUT("MAC_DL_total_HARQ_TX: %d\r\n", (int)param->data_transfer_info->MAC_DL_total_HARQ_TX);
                        RIL_LOGUT("MAC_UL_HARQ_re_TX: %d\r\n", (int)param->data_transfer_info->MAC_UL_HARQ_re_TX);
                        RIL_LOGUT("MAC_DL_HARQ_re_TX: %d\r\n", (int)param->data_transfer_info->MAC_DL_HARQ_re_TX);
                        RIL_LOGUT("RLC_UL_tput: %d\r\n", (int)param->data_transfer_info->RLC_UL_tput);
                        RIL_LOGUT("RLC_DL_tput: %d\r\n", (int)param->data_transfer_info->RLC_DL_tput);
                        RIL_LOGUT("MAC_UL_tput: %d\r\n", (int)param->data_transfer_info->MAC_UL_tput);
                        RIL_LOGUT("MAC_DL_tput: %d\r\n", (int)param->data_transfer_info->MAC_DL_tput);
                    }
                }
            }
            break;

            case RIL_CMD_ID_MFRCLLCK: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_MSPCHSC: {
                ril_set_scrambling_algorithm_for_npdsch_rsp_t *param = (ril_set_scrambling_algorithm_for_npdsch_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("mode: %d\r\n", (int)param->mode);
                }
            }
            break;

            case RIL_CMD_ID_MDPDNP: {
                ril_default_pdn_parameter_rsp_t *param = (ril_default_pdn_parameter_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("n: %d\r\n", (int)param->n);
                }
            }
            break;

            case RIL_CMD_ID_MEDRXCFG: {
                ril_eDRX_configuration_rsp_t *param = (ril_eDRX_configuration_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    RIL_LOGUT("act_type: %d\r\n", (int)param->act_type);
                    RIL_LOGUT("requested_eDRX_value: %s\r\n", param->requested_eDRX_value);
                    RIL_LOGUT("requested_paging_time_window_value: %s\r\n", param->requested_paging_time_window_value);
                }
            }
            break;

            case RIL_CMD_ID_MCELLINFO: {
                ril_serving_and_neighbor_cell_info_rsp_t *param = (ril_serving_and_neighbor_cell_info_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && param != NULL) {
                    int32_t i;
                    RIL_LOGUT("serving cell info:");
                    RIL_LOGUT("earfcn: %d\r\n", (int)param->sc_info->earfcn);
                    RIL_LOGUT("earfcn_offset: %d\r\n", (int)param->sc_info->earfcn_offset);
                    RIL_LOGUT("pci: %d\r\n", (int)param->sc_info->pci);
                    RIL_LOGUT("rsrp: %d\r\n", (int)param->sc_info->rsrp);
                    RIL_LOGUT("rsrq: %d\r\n", (int)param->sc_info->rsrq);
                    RIL_LOGUT("rssi: %d\r\n", (int)param->sc_info->rssi);
                    RIL_LOGUT("sinr: %d\r\n", (int)param->sc_info->sinr);
                    RIL_LOGUT("mcc: %d\r\n", (int)param->sc_info->mcc);
                    RIL_LOGUT("mnc: %d\r\n", (int)param->sc_info->mnc);
                    RIL_LOGUT("tac: %s\r\n", param->sc_info->tac);
                    RIL_LOGUT("cellid: %s\r\n", param->sc_info->cellid);

                    RIL_LOGUT("neighbor cell info:");
                    for (i = 0; i < param->nc_num; i++) {
                        RIL_LOGUT("entry#%d, earfcn: %d\r\n", (int)i + 1, (int)param->nc_info_list[i].earfcn);
                        RIL_LOGUT("entry#%d, earfcn_offset: %d\r\n", (int)i + 1, (int)param->nc_info_list[i].earfcn_offset);
                        RIL_LOGUT("entry#%d, pci: %d\r\n", (int)i + 1, (int)param->nc_info_list[i].pci);
                        RIL_LOGUT("entry#%d, rsrp: %d\r\n", (int)i + 1, (int)param->nc_info_list[i].rsrp);
                        RIL_LOGUT("entry#%d, rsrq: %d\r\n", (int)i + 1, (int)param->nc_info_list[i].rsrq);
                        RIL_LOGUT("entry#%d, rssi: %d\r\n", (int)i + 1, (int)param->nc_info_list[i].rssi);
                        RIL_LOGUT("entry#%d, sinr: %d\r\n", (int)i + 1, (int)param->nc_info_list[i].sinr);
                        RIL_LOGUT("entry#%d, mcc: %d\r\n", (int)i + 1, (int)param->nc_info_list[i].mcc);
                        RIL_LOGUT("entry#%d, mnc: %d\r\n", (int)i + 1, (int)param->nc_info_list[i].mnc);
                        RIL_LOGUT("entry#%d, tac: %s\r\n", (int)i + 1, param->nc_info_list[i].tac);
                        RIL_LOGUT("entry#%d, cellid: %s\r\n", (int)i + 1, param->nc_info_list[i].cellid);
                    }
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

