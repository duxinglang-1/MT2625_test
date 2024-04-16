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

#include "ril_cmds_27007.h"
#include "ril_cmds_common.h"

/*************************************************
 *                 send api
 *************************************************/

/* AT cmd: AT+CGMI */
ril_status_t ril_request_manufacturer_identification(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_CGMI, NULL, (void *)callback, user_data);
}



/* AT cmd: AT+CGMM */
ril_status_t ril_request_model_identification(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_CGMM, NULL, (void *)callback, user_data);
}


/* AT cmd: AT+CGMR */
ril_status_t ril_request_revision_identification(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_CGMR, NULL, (void *)callback, user_data);
}


/* AT cmd: AT+CGSN */
ril_status_t ril_request_serial_number(ril_request_mode_t mode,
                                       int32_t sn_type,
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

            if ((ret = ril_param_list_add_int_optional(list_head, sn_type, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CGSN, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CSCS */
ril_status_t ril_request_select_charcter_set(ril_request_mode_t mode,
        char *chset,
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

            if ((ret = ril_param_list_add_string_optional(list_head, chset, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CSCS, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CIMI */
ril_status_t ril_request_imsi(ril_request_mode_t mode,
                              ril_cmd_response_callback_t callback,
                              void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_CIMI, NULL, (void *)callback, user_data);
}


/* AT cmd: AT+CMUX */
ril_status_t ril_request_multiplexing_mode(ril_request_mode_t mode,
        ril_cmux_param_t *cmux_param,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if (cmux_param == NULL || cmux_param->mode == RIL_OMITTED_INTEGER_PARAM) {
                ret = RIL_STATUS_INVALID_PARAM;
                break;
            }
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            /* transparency, necessary */
            if ((ret = ril_param_list_add_int_optional(list_head, cmux_param->mode, 2)) < 0) {
                break;
            }

            /* subset, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, cmux_param->subset, 0)) < 0) {
                break;
            }
            /* port speed, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, cmux_param->port_speed, 0)) < 0) {
                break;
            }

            /* N1, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, cmux_param->N1, 0)) < 0) {
                break;
            }

            /* T1, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, cmux_param->T1, 0)) < 0) {
                break;
            }

            /* N2, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, cmux_param->N2, 0)) < 0) {
                break;
            }

            /* T2, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, cmux_param->T2, 0)) < 0) {
                break;
            }

            /* T3, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, cmux_param->T3, 0)) < 0) {
                break;
            }
            /* k, optional */
            if ((ret = ril_param_list_add_int_optional(list_head, cmux_param->k, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CMUX, list_head, (void *)callback, user_data);
    }
}


#ifndef __RIL_CMD_SET_SLIM_ENABLE__
/* AT cmd: AT+CR */
ril_status_t ril_request_service_reporting_control(ril_request_mode_t mode,
        int32_t cr_mode,
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

            if ((ret = ril_param_list_add_int_optional(list_head, cr_mode, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret < 0) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CR, list_head, (void *)callback, user_data);
    }
}
#endif


/* AT cmd: AT+CEER */
ril_status_t ril_request_extended_error_report(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_CEER, NULL, (void *)callback, user_data);
}


/* AT cmd: AT+CNUM */
ril_status_t ril_request_subscriber_number(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_CNUM, NULL, (void *)callback, user_data);
}


#ifndef __RIL_CMD_SET_SLIM_ENABLE__
/* AT cmd: AT+CREG */
ril_status_t ril_request_network_registration(ril_request_mode_t mode,
        int32_t action,
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
            if ((ret = ril_param_list_add_int_optional(list_head, action, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CREG, list_head, (void *)callback, user_data);
    }
}
#endif


/* AT cmd: AT+COPS */
ril_status_t ril_request_plmn_selection(ril_request_mode_t mode,
                                        int32_t cmode,
                                        int32_t format,
                                        char *oper,
                                        int32_t act,
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

            if ((ret = ril_param_list_add_int_optional(list_head, cmode, 0)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, format, 0)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_string_optional(list_head, oper, 0)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, act, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_COPS, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CLCK */
ril_status_t ril_request_facility_lock(ril_request_mode_t mode,
                                       char *fac,
                                       int32_t cmode,
                                       char *passwd,
                                       int32_t class_p,
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

            if ((ret = ril_param_list_add_string_optional(list_head, fac, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, cmode, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, passwd, 0)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, class_p, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CLCK, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CPWD */
ril_status_t ril_request_change_password(ril_request_mode_t mode,
        char *fac,
        char *oldpwd,
        char *newpwd,
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

            if ((ret = ril_param_list_add_string_optional(list_head, fac, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, oldpwd, 1)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, newpwd, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret < 0) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CPWD, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CPOL */
ril_status_t ril_request_preferred_plmn_list(ril_request_mode_t mode,
        ril_plmn_item_struct *plmn_p,
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
            if ((ret = ril_param_list_add_int_optional(list_head, plmn_p->index, 1)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, plmn_p->format, 0)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_string_optional(list_head, plmn_p->oper, 0)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, plmn_p->gsm_act, 0)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, plmn_p->gsm_compact_act, 2)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, plmn_p->utran_act, 2)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, plmn_p->e_utran_act, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CPOL, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CPLS */
ril_status_t ril_request_selection_of_preferred_plmn_list(ril_request_mode_t mode,
        int32_t list,
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
            if ((ret = ril_param_list_add_int_optional(list_head, list, 0)) < 0) {
                break;
            }

        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CPLS, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+COPN */
ril_status_t ril_request_read_operator_names(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_COPN, NULL, (void *)callback, user_data);

}


/* AT cmd: AT+CFUN */
ril_status_t ril_request_set_phone_functionality(ril_request_mode_t mode,
        int32_t fun,
        int32_t rst,
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
            if ((ret = ril_param_list_add_int_optional(list_head, fun, 0)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_int_optional(list_head, rst, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CFUN, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CPIN */
ril_status_t ril_request_enter_pin(ril_request_mode_t mode,
                                   char *pin,
                                   char *new_pin,
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
            if ((ret = ril_param_list_add_string_optional(list_head, pin, 2)) < 0) {
                break;
            }

            if ((ret = ril_param_list_add_string_optional(list_head, new_pin, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CPIN, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CSQ */
ril_status_t ril_request_signal_quality(ril_request_mode_t mode,
                                        ril_cmd_response_callback_t callback,
                                        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_CSQ, NULL, (void *)callback, user_data);
}


/* AT cmd: AT+CCLK */
ril_status_t ril_request_clock(ril_request_mode_t mode,
                               char *time,
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
            if ((ret = ril_param_list_add_string_optional(list_head, time, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CCLK, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CSIM */
ril_status_t ril_request_generic_sim_access(ril_request_mode_t mode,
        int32_t length,
        char *command,
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
            if ((ret = ril_param_list_add_int_optional(list_head, length, 2)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_string_optional(list_head, command, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CSIM, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CRSM */
ril_status_t ril_request_restricted_sim_access(ril_request_mode_t mode,
        ril_restricted_sim_access_req_t *req,
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
            if ((ret = ril_param_list_add_int_optional(list_head, req->command, 2)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, req->field, 0)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, req->p1, 0)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, req->p2, 2)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, req->p3, 2)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_string_optional(list_head, req->data, 0)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_string_optional(list_head, req->pathid, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CRSM, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CMAR */
ril_status_t ril_request_master_reset(ril_request_mode_t mode,
                                      char *phone_lock_code,
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
            if ((ret = ril_param_list_add_string_optional(list_head, phone_lock_code, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CMAR, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CTZU */
ril_status_t ril_request_automatic_time_zone_update(ril_request_mode_t mode,
        int32_t onoff,
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
            if ((ret = ril_param_list_add_int_optional(list_head, onoff, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CTZU, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CTZR */
ril_status_t ril_request_time_zone_reporting(ril_request_mode_t mode,
        int32_t reporting,
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
            if ((ret = ril_param_list_add_int_optional(list_head, reporting, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CTZR, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CGPIAF */
ril_status_t ril_request_printing_ip_address_format(ril_request_mode_t mode,
        int32_t ipv6_addr_format,
        int32_t ipv6_subnetnotation,
        int32_t ipv6_leadingzeros,
        int32_t ipv6_compresszeros,
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
            if ((ret = ril_param_list_add_int_optional(list_head, ipv6_addr_format, 0)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, ipv6_subnetnotation, 0)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, ipv6_leadingzeros, 0)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, ipv6_compresszeros, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CGPIAF, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CPINR */
ril_status_t ril_request_remaining_pin_retries(ril_request_mode_t mode,
        char *sel_code,
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
            if ((ret = ril_param_list_add_string_optional(list_head, sel_code, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CPINR, list_head, (void *)callback, user_data);
    }
}


#ifndef __RIL_CMD_SET_SLIM_ENABLE__
/* AT cmd: AT+CSUS */
ril_status_t ril_request_set_card_slot(ril_request_mode_t mode,
                                       int32_t card_slot,
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
            if ((ret = ril_param_list_add_int_optional(list_head, card_slot, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CSUS, list_head, (void *)callback, user_data);
    }
}
#endif


/* AT cmd: AT+CESQ */
ril_status_t ril_request_extended_signal_quality(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_CESQ, NULL, (void *)callback, user_data);

}


/* AT cmd: AT+CMEE */
ril_status_t ril_request_report_mobile_termination_error(ril_request_mode_t mode,
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
            if ((ret = ril_param_list_add_int_optional(list_head, n, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CMEE, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CGDCONT */
ril_status_t ril_request_define_pdp_context(ril_request_mode_t mode,
        ril_pdp_context_entity_t *req,
        ril_cmd_response_callback_t callback,
        void *user_data,
        int32_t channel_id)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_int_optional(list_head, req->cid, 0)) < 0) {
                break;
            }
            if ((ret = ril_param_list_add_string_optional(list_head, req->pdp_type, 0)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_string_optional(list_head, req->apn, 0)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_string_optional(list_head, req->pdp_addr, 0)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_int_optional(list_head, req->d_comp, 0)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_int_optional(list_head, req->h_comp, 0)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_int_optional(list_head, req->ipv4addralloc, 0)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_int_optional(list_head, req->request_type, 0)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_int_optional(list_head, req->pcscf_discovery, 0)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_int_optional(list_head, req->im_cn_signalling_flag_ind, 0)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_int_optional(list_head, req->nslpi, 0)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_int_optional(list_head, req->securepco, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send_via_channel(mode, RIL_CMD_ID_CGDCONT, list_head, (void *)callback, user_data, channel_id);
    }
}



/* AT cmd: AT+CGATT */
ril_status_t ril_request_ps_attach_or_detach(ril_request_mode_t mode,
        int32_t state,
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
            if ((ret =  ril_param_list_add_int_optional(list_head, state, 0)) < 0) {
                break;
            }

        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CGATT, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CGACT */
ril_status_t ril_request_pdp_context_activate_or_deactivate(ril_request_mode_t mode,
        ril_pdp_context_activate_or_deactivate_req_t *req,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    int32_t idx = 0;

    if (mode == RIL_EXECUTE_MODE) {
        do {
            if (req == NULL) {
                return RIL_STATUS_INVALID_PARAM;
            }

            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }

            if ((ret =  ril_param_list_add_int_optional(list_head, req->state, 0)) < 0) {
                break;
            }
            while (idx < req->cid_array_len && req->cid_array != NULL) {
                if ((ret =  ril_param_list_add_int_optional(list_head, req->cid_array[idx], 0)) < 0) {
                    break;
                }
                idx++;
            }

        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CGACT, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CGDATA */
ril_status_t ril_request_enter_data_state(ril_request_mode_t mode,
        ril_enter_data_state_req_t *req,
        ril_cmd_response_callback_t callback,
        void *user_data,
        int32_t channel_id)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    int32_t idx = 0;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_string_optional(list_head, req->l2p, 0)) < 0) {
                break;
            }
            while (idx < req->cid_array_len && req->cid_array != NULL) {
                if ((ret =  ril_param_list_add_int_optional(list_head, req->cid_array[idx], 0)) < 0) {
                    break;
                }
                idx++;
            }

        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send_via_channel(mode, RIL_CMD_ID_CGDATA, list_head, (void *)callback, user_data, channel_id);
    }
}


/* AT cmd: AT+CGPADDR */
ril_status_t ril_request_show_pdp_address(ril_request_mode_t mode,
        ril_show_pdp_address_req_t *req,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    int32_t idx = 0;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }
            while (idx < req->cid_array_len && req->cid_array != NULL) {
                if ((ret =  ril_param_list_add_int_optional(list_head, req->cid_array[idx], 0)) < 0) {
                    break;
                }
                idx++;
            }

        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CGPADDR, list_head, (void *)callback, user_data);
    }
}



/* AT cmd: AT+CGEREP */
ril_status_t ril_request_packet_domain_event_reporting(ril_request_mode_t mode,
        int32_t cmode,
        int32_t bfr,
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
            if ((ret =  ril_param_list_add_int_optional(list_head, cmode, 0)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_int_optional(list_head, bfr, 0)) < 0) {
                break;
            }

        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CGEREP, list_head, (void *)callback, user_data);
    }
}


#ifndef __RIL_CMD_SET_SLIM_ENABLE__
/* AT cmd: AT+CGREG */
ril_status_t ril_request_gprs_network_registration_status(ril_request_mode_t mode,
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
            if ((ret =  ril_param_list_add_int_optional(list_head, n, 0)) < 0) {
                break;
            }

        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CGREG, list_head, (void *)callback, user_data);
    }
}
#endif


/* AT cmd: AT+CEREG */
ril_status_t ril_request_eps_network_registration_status(ril_request_mode_t mode,
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
            if ((ret =  ril_param_list_add_int_optional(list_head, n, 0)) < 0) {
                break;
            }

        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CEREG, list_head, (void *)callback, user_data);
    }
}



/* AT cmd: AT+CGCONTRDP */
ril_status_t ril_request_pdp_context_read_dynamic_parameters(ril_request_mode_t mode,
        int32_t cid,
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
            if ((ret =  ril_param_list_add_int_optional(list_head, cid, 0)) < 0) {
                break;
            }

        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CGCONTRDP, list_head, (void *)callback, user_data);
    }
}



/* AT cmd: AT+CGDEL */
ril_status_t ril_request_delete_non_active_pdp_contexts(ril_request_mode_t mode,
        int32_t cid,
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
            if ((ret =  ril_param_list_add_int_optional(list_head, cid, 0)) < 0) {
                break;
            }

        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CGDEL, list_head, (void *)callback, user_data);
    }
}



/* AT cmd: AT+CGAUTH */
ril_status_t ril_request_define_pdp_context_authentication_parameters(ril_request_mode_t mode,
        int32_t cid,
        int32_t auth_port,
        char *userid,
        char *password,
        ril_cmd_response_callback_t callback,
        void *user_data,
        int32_t channel_id)
{
    int32_t ret = 0;
    ril_param_node_t *list_head = NULL;
    if (mode == RIL_EXECUTE_MODE) {
        do {
            if ((ret = ril_param_list_create(&list_head)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_int_optional(list_head, cid, 2)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_int_optional(list_head, auth_port, 0)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_string_optional(list_head, userid, 0)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_string_optional(list_head, password, 0)) < 0) {
                break;
            }

        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send_via_channel(mode, RIL_CMD_ID_CGAUTH, list_head, (void *)callback, user_data, channel_id);
    }
}



/* AT cmd: AT+CIPCA */
ril_status_t ril_request_initial_pdp_context_activation(ril_request_mode_t mode,
        int32_t n,
        int32_t attach_without_pdn,
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
            if ((ret =  ril_param_list_add_int_optional(list_head, n, 0)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_int_optional(list_head, attach_without_pdn, 0)) < 0) {
                break;
            }

        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CIPCA, list_head, (void *)callback, user_data);
    }
}



/* AT cmd: AT+CPSMS */
ril_status_t ril_request_power_saving_mode_setting(ril_request_mode_t mode,
        ril_power_saving_mode_setting_req_t *req,
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
            if ((ret =  ril_param_list_add_int_optional(list_head, req->mode, 1)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_string_optional(list_head, req->req_prdc_rau, 1)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_string_optional(list_head, req->req_gprs_rdy_tmr, 1)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_string_optional(list_head, req->req_prdc_tau, 1)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_string_optional(list_head, req->req_act_time, 1)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CPSMS, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CCIOTOPT */
ril_status_t ril_request_ciot_optimisation_configuration(ril_request_mode_t mode,
        int32_t n,
        int32_t supported_UE_opt,
        int32_t preferred_UE_opt,
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
            if ((ret =  ril_param_list_add_int_optional(list_head, n, 0)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_int_optional(list_head, supported_UE_opt, 0)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_int_optional(list_head, preferred_UE_opt, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CCIOTOPT, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CEDRXS */
ril_status_t ril_request_eDRX_setting(ril_request_mode_t mode,
                                      int32_t eDRX_mode,
                                      int32_t act_type,
                                      char *requested_eDRX_value,
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
            if ((ret =  ril_param_list_add_int_optional(list_head, eDRX_mode, 0)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_int_optional(list_head, act_type, 0)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_string_optional(list_head, requested_eDRX_value, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CEDRXS, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CEDRXRDP */
ril_status_t ril_request_read_eDRX_dynamic_parameters(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_CEDRXRDP, NULL, (void *)callback, user_data);
}


/* AT cmd: AT+CGAPNRC */
ril_status_t ril_request_APN_rate_control(ril_request_mode_t mode,
        int32_t cid,
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
            if ((ret =  ril_param_list_add_int_optional(list_head, cid, 0)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CGAPNRC, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CSCON */
ril_status_t ril_request_signaling_connection_status(ril_request_mode_t mode,
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
            if ((ret =  ril_param_list_add_int_optional(list_head, n, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CSCON, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CCHO */
ril_status_t ril_request_open_uicc_logical_channel(ril_request_mode_t mode,
        char *dfname,
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
            if ((ret =  ril_param_list_add_string_optional(list_head, dfname, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CCHO, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CCHC */
ril_status_t ril_request_close_uicc_logical_channel(ril_request_mode_t mode,
        int32_t sessionid,
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
            if ((ret =  ril_param_list_add_int_optional(list_head, sessionid, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CCHC, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CGLA */
ril_status_t ril_request_generic_uicc_logical_channel_access(ril_request_mode_t mode,
        int32_t sessionid,
        int32_t length,
        char *command,
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
            if ((ret =  ril_param_list_add_int_optional(list_head, sessionid, 2)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_int_optional(list_head, length, 2)) < 0) {
                break;
            }
            if ((ret =  ril_param_list_add_string_optional(list_head, command, 2)) < 0) {
                break;
            }
        } while (0);
    }

    if (ret == -1) {
        ril_param_list_delete(list_head);
        return RIL_STATUS_FAIL;
    } else {
        return ril_request_send(mode, RIL_CMD_ID_CGLA, list_head, (void *)callback, user_data);
    }
}


/* AT cmd: AT+CRCES */
ril_status_t ril_request_read_coverage_enhancement_status(ril_request_mode_t mode,
        ril_cmd_response_callback_t callback,
        void *user_data)
{
    return ril_request_send(mode, RIL_CMD_ID_CRCES, NULL, (void *)callback, user_data);
}


/*************************************************
 *                 response handler
 *************************************************/

/* AT cmd: AT+CGMI */
int32_t ril_response_manufacturer_identification(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_manufacturer_identification_rsp_t response;
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
            response.manufacturer = line;
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


/* AT cmd: AT+CGMM */
int32_t ril_response_model_identification(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_model_identification_rsp_t response;
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
            response.model = line;
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


/* AT cmd: AT+CGMR */
int32_t ril_response_revision_identification(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_revision_identification_rsp_t response;
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
            response.revision = line;
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


/* AT cmd: AT+CGSN */
int32_t ril_response_serial_number(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_serial_number_rsp_t response;
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
            skip_white_space(&line);
            /* response may not include command head. */
            if (str_starts_with(line, "+CGSN:")) {
                ret = at_tok_start(&line);
                if (ret < 0) {
                    break;
                }
            }

            ret = at_tok_nextstr(&line, &response.value.sn);
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


/* AT cmd: AT+CSCS */
int32_t ril_response_select_charcter_set(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_select_character_set_rsp_t response;
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

            ret = at_tok_nextstr(&line, &response.chset);
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


/* AT cmd: AT+CIMI */
int32_t ril_response_imsi(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_imsi_rsp_t response;
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
            response.imsi = line;
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


/* AT cmd: AT+CMUX */
int32_t ril_response_multiplexing_mode(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_cmux_param_rsp_t response;
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

            do {
                response.mode = RIL_OMITTED_INTEGER_PARAM;
                response.subset = RIL_OMITTED_INTEGER_PARAM;
                response.port_speed = RIL_OMITTED_INTEGER_PARAM;
                response.N1 = RIL_OMITTED_INTEGER_PARAM;
                response.T1 = RIL_OMITTED_INTEGER_PARAM;
                response.N2 = RIL_OMITTED_INTEGER_PARAM;
                response.T2 = RIL_OMITTED_INTEGER_PARAM;
                response.T3 = RIL_OMITTED_INTEGER_PARAM;
                response.k = RIL_OMITTED_INTEGER_PARAM;
                ret = at_tok_nextint(&line, &response.mode);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.subset);
                if (ret < 0) {
                    ret = 0;
                    break;
                }

                ret = at_tok_nextint(&line, &response.port_speed);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.N1);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.T1);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.N2);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.T2);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.T3);
                if (ret < 0) {
                    break;
                }

                ret = at_tok_nextint(&line, &response.k);
                if (ret < 0) {
                    ret = 0;
                    break;
                }
            } while (0);

            if (ret < 0) break;
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


/* AT cmd: AT+CR */
int32_t ril_response_service_reporting_control(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_service_reporting_control_rsp_t response;
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


/* AT cmd: AT+CEER */
int32_t ril_response_ceer_extended_error_report(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_extended_error_report_rsp_t response;
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
            response.report = line;
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



/* AT cmd: AT+CNUM */
int32_t ril_response_subscriber_number(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_subscriber_number_rsp_t response;
    ril_subscriber_number_entry_t *entry, *list_head;

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
    response.array_num = 0;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (message_num == 0) {
                list_head = (ril_subscriber_number_entry_t *)ril_mem_malloc(sizeof(ril_subscriber_number_entry_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_subscriber_number_entry_t) * (message_num + 1));
                entry = list_head + message_num;
            }
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            entry->alpha = RIL_OMITTED_STRING_PARAM;
            entry->number = RIL_OMITTED_STRING_PARAM;
            entry->type = RIL_OMITTED_INTEGER_PARAM;
            ret = at_tok_nextstr(&line, &entry->alpha);
            if (ret < 0) {                
                ret = 0;
            }

            ret = at_tok_nextstr(&line, &entry->number);
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
    response.entries_array = list_head;
    response.array_num = message_num;

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}



/* AT cmd: AT+CREG */
int32_t ril_response_creg_network_registration(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_network_registration_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.stat);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nexthexint(&line, &response.lac);
            if (ret < 0) {
                response.lac = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nexthexint(&line, &response.ci);
            if (ret < 0) {
                response.ci = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &response.act);
            if (ret < 0) {
                response.act = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &response.cause_type);
            if (ret < 0) {
                response.cause_type = response.reject_cause = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &response.reject_cause);
            if (ret < 0) {
                response.reject_cause = RIL_OMITTED_INTEGER_PARAM;
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


/* AT cmd: AT+COPS */
int32_t ril_response_cops_plmn_selection(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_plmn_selection_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.format);
            if (ret < 0) {
                response.format = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextstr(&line, &response.oper);
            if (ret < 0) {
                response.oper = RIL_OMITTED_STRING_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &response.act);
            if (ret < 0) {
                response.act = RIL_OMITTED_INTEGER_PARAM;
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



/* AT cmd: AT+CLCK */
int32_t ril_response_clck_facility_lock(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_facility_lock_rsp_t response;
    ril_facility_lock_struct_t *entry, *list_head;

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
    response.array_num = 0;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (message_num == 0) {
                list_head = (ril_facility_lock_struct_t *)ril_mem_malloc(sizeof(ril_facility_lock_struct_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_facility_lock_struct_t) * (message_num + 1));
                entry = list_head + message_num;
            }
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->status);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->classn);
            if (ret < 0) {
                entry->classn = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }
            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.status_array = list_head;
    response.array_num = message_num;

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}



/* AT cmd: AT+CPWD */
int32_t ril_response_cpwd_change_password(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT cmd: AT+CPOL */
int32_t ril_response_cpol_preferred_plmn_list(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_preferred_plmn_list_rsp_t response;
    ril_plmn_item_struct *entry, *list_head;

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
    response.array_num = 0;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (message_num == 0) {
                list_head = (ril_plmn_item_struct *)ril_mem_malloc(sizeof(ril_plmn_item_struct));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_plmn_item_struct) * (message_num + 1));
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

            ret = at_tok_nextint(&line, &entry->format);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->oper);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->gsm_act);
            if (ret < 0) {
                entry->gsm_act = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &entry->gsm_compact_act);
            if (ret < 0) {
                entry->gsm_compact_act = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &entry->utran_act);
            if (ret < 0) {
                entry->utran_act = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &entry->e_utran_act);
            if (ret < 0) {
                entry->e_utran_act = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }
            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.plmn_list = list_head;
    response.array_num = message_num;

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}


/* AT cmd: AT+CPLS */
int32_t ril_response_cpls_selection_of_preferred_plmn_list(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_selection_of_preferred_plmn_list_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.list);
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


/* AT cmd: AT+COPN */
int32_t ril_response_copn_read_operator_names(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_read_operator_names_rsp_t response;
    ril_operator_names_struct_t *entry, *list_head;

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
    response.array_num = 0;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (message_num == 0) {
                list_head = (ril_operator_names_struct_t *)ril_mem_malloc(sizeof(ril_operator_names_struct_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_operator_names_struct_t) * (message_num + 1));
                entry = list_head + message_num;
            }
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->numeric);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->alpha);
            if (ret < 0) {
                break;
            }
            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.oper_name = list_head;
    response.array_num = message_num;

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}



/* AT cmd: AT+CFUN */
int32_t ril_response_cfun_set_phone_functionality(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_set_phone_functionality_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.fun);
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




/* AT cmd: AT+CPIN */
int32_t ril_response_cpin_enter_pin(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_enter_pin_rsp_t response;
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

            ret = at_tok_nextstr(&line, &response.code);
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



/* AT cmd: AT+CSQ */
int32_t ril_response_csq_signal_quality(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_signal_quality_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.rssi);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.ber);
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


/* AT cmd: AT+CCLK */
int32_t ril_response_cclk_clock(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_clock_rsp_t response;
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

            response.time = line;
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



/* AT cmd: AT+CSIM */
int32_t ril_response_csim_generic_sim_access(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_generic_sim_access_rsp_t response;
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

            ret = at_tok_nextstr(&line, &response.response);
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



/* AT cmd: AT+CRSM */
int32_t ril_response_crsm_restricted_sim_access(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_restricted_sim_access_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.sw1);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.sw2);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &response.response);
            if (ret < 0) {
                response.response = RIL_OMITTED_STRING_PARAM;
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



/* AT cmd: AT+CMAR */
int32_t ril_response_cmar_master_reset(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}



/* AT cmd: AT+CTZU */
int32_t ril_response_ctzu_automatic_time_zone_update(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_automatic_time_zone_update_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.onoff);
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



/* AT cmd: AT+CTZR */
int32_t ril_response_ctzr_time_zone_reporting(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_time_zone_reporting_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.reporting);
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



/* AT cmd: AT+CGPIAF */
int32_t ril_response_cgpiaf_printing_ip_address_format(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_printing_ip_address_format_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.ipv6_addr_format);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.ipv6_subnetnotation);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.ipv6_leadingzeros);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.ipv6_compresszeros);
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



/* AT cmd: AT+CPINR */
int32_t ril_response_cpinr_remaining_pin_retries(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_remaining_pin_retries_rsp_t response;
    ril_remaining_pin_retries_entry_t *entry, *list_head;

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
    response.array_num = 0;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (message_num == 0) {
                list_head = (ril_remaining_pin_retries_entry_t *)ril_mem_malloc(sizeof(ril_remaining_pin_retries_entry_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_remaining_pin_retries_entry_t) * (message_num + 1));
                entry = list_head + message_num;
            }
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->code);
            if (ret < 0) {
                break;
            }


            ret = at_tok_nextint(&line, &entry->retries);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->default_retries);
            if (ret < 0) {
                entry->default_retries = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }
            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.retry_entries = list_head;
    response.array_num = message_num;

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}




/* AT cmd: AT+CSUS */
int32_t ril_response_csus_set_card_slot(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_set_card_slot_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.card_slot);
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



/* AT cmd: AT+CESQ */
int32_t ril_response_cesq_extended_signal_quality(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_extended_signal_quality_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.rxlev);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.ber);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.rscp);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.ecno);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.rsrq);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.rsrp);
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



/* AT cmd: AT+CMEE */
int32_t ril_response_cmee_report_mobile_termination_error(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_report_mobile_termination_error_rsp_t response;
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



/* AT cmd: AT+CGDCONT */
int32_t ril_response_cgdcont_define_pdp_context(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_define_pdp_context_rsp_t response;
    ril_pdp_context_entity_t *entry, *list_head;

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
    response.array_num = 0;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (message_num == 0) {
                list_head = (ril_pdp_context_entity_t *)ril_mem_malloc(sizeof(ril_pdp_context_entity_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_pdp_context_entity_t) * (message_num + 1));
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


            ret = at_tok_nextstr(&line, &entry->pdp_type);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->apn);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->pdp_addr);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->d_comp);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->h_comp);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->ipv4addralloc);
            if (ret < 0) {
                entry->ipv4addralloc = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &entry->request_type);
            if (ret < 0) {
                entry->request_type = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &entry->pcscf_discovery);
            if (ret < 0) {
                entry->pcscf_discovery = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &entry->im_cn_signalling_flag_ind);
            if (ret < 0) {
                entry->im_cn_signalling_flag_ind = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &entry->nslpi);
            if (ret < 0) {
                entry->nslpi = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &entry->securepco);
            if (ret < 0) {
                entry->securepco = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }
            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.pdp_context = list_head;
    response.array_num = message_num;

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}


/* AT cmd: AT+CGATT */
int32_t ril_response_cgatt_ps_attach_or_detach(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_ps_attach_or_detach_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.state);
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



/* AT cmd: AT+CGACT */
int32_t ril_response_cgact_pdp_context_activate_or_deactivate(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_pdp_context_activate_or_deactivate_rsp_t response;
    ril_cid_state_struct_t *entry, *list_head;

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
    response.array_num = 0;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (message_num == 0) {
                list_head = (ril_cid_state_struct_t *)ril_mem_malloc(sizeof(ril_cid_state_struct_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_cid_state_struct_t) * (message_num + 1));
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


            ret = at_tok_nextint(&line, &entry->state);
            if (ret < 0) {
                break;
            }
            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }

    response.cid_state = list_head;
    response.array_num = message_num;

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}



/* AT cmd: AT+CGDATA */
int32_t ril_response_cgdata_enter_data_state(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    return ril_response_common_no_parameter(channel_id, cmd_buf, cmd_buf_len);
}


/* AT cmd: AT+CGPADDR */
int32_t ril_response_cgpaddr_show_pdp_address(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_show_pdp_address_rsp_t response;
    ril_cid_addr_struct_t *entry, *list_head;

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
    response.array_num = 0;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (message_num == 0) {
                list_head = (ril_cid_addr_struct_t *)ril_mem_malloc(sizeof(ril_cid_addr_struct_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_cid_addr_struct_t) * (message_num + 1));
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


            ret = at_tok_nextstr(&line, &entry->pdp_addr_1);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->pdp_addr_2);
            if (ret < 0) {
                entry->pdp_addr_2 = RIL_OMITTED_STRING_PARAM;
                ret = 0;
            }
            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.cid_addr = list_head;
    response.array_num = message_num;

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}



/* AT cmd: AT+CGEREP */
int32_t ril_response_cgerep_packet_domain_event_reporting(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_packet_domain_event_reporting_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.cmode);
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



/* AT cmd: AT+CGREG */
int32_t ril_response_cgreg_gprs_network_registration_status(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_gprs_network_registration_status_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.stat);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nexthexint(&line, &response.lac);
            if (ret < 0) {
                response.lac = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nexthexint(&line, &response.ci);
            if (ret < 0) {
                response.ci = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &response.act);
            if (ret < 0) {
                response.act = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextbinint(&line, &response.rac);
            if (ret < 0) {
                response.rac = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &response.cause_type);
            if (ret < 0) {
                response.cause_type = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &response.reject_cause);
            if (ret < 0) {
                response.reject_cause = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextbinint(&line, &response.active_time);
            if (ret < 0) {
                response.reject_cause = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextbinint(&line, &response.periodic_rau);
            if (ret < 0) {
                response.reject_cause = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextbinint(&line, &response.gprs_readytimer);
            if (ret < 0) {
                response.reject_cause = RIL_OMITTED_INTEGER_PARAM;
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



/* AT cmd: AT+CEREG */
int32_t ril_response_cereg_eps_network_registration_status(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_eps_network_registration_status_rsp_t response;
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

            /* initialize response structure first */
            response.tac = RIL_OMITTED_INTEGER_PARAM;
            response.ci = RIL_OMITTED_INTEGER_PARAM;
            response.act = RIL_OMITTED_INTEGER_PARAM;
            response.cause_type = RIL_OMITTED_INTEGER_PARAM;
            response.reject_cause = RIL_OMITTED_INTEGER_PARAM;
            response.active_time = RIL_OMITTED_INTEGER_PARAM;
            response.periodic_tau = RIL_OMITTED_INTEGER_PARAM;

            ret = at_tok_nextint(&line, &response.n);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.stat);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nexthexint(&line, &response.tac);
            if (ret < 0) {
                response.tac = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nexthexint(&line, &response.ci);
            if (ret < 0) {
                response.ci = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &response.act);
            if (ret < 0) {
                response.act = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &response.cause_type);
            if (ret < 0) {
                response.cause_type = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &response.reject_cause);
            if (ret < 0) {
                response.reject_cause = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextbinint(&line, &response.active_time);
            if (ret < 0) {
                response.active_time = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextbinint(&line, &response.periodic_tau);
            if (ret < 0) {
                response.periodic_tau = RIL_OMITTED_INTEGER_PARAM;
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



/* AT cmd: AT+CGCONTRDP */
int32_t ril_response_cgcontrdp_pdp_context_read_dynamic_parameters(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_pdp_context_read_dynamic_parameters_rsp_t response;
    ril_pdp_context_entry_struct_t *entry, *list_head;

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
    response.array_num = 0;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (message_num == 0) {
                list_head = (ril_pdp_context_entry_struct_t *)ril_mem_malloc(sizeof(ril_pdp_context_entry_struct_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_pdp_context_entry_struct_t) * (message_num + 1));
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

            ret = at_tok_nextint(&line, &entry->bearer_id);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->apn);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->local_addr_and_subnet_mask);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->gw_addr);
            if (ret < 0) {
                entry->gw_addr = RIL_OMITTED_STRING_PARAM;
                ret = 0;
            }

            ret = at_tok_nextstr(&line, &entry->dns_prim_addr);
            if (ret < 0) {
                entry->dns_prim_addr = RIL_OMITTED_STRING_PARAM;
                ret = 0;
            }

            ret = at_tok_nextstr(&line, &entry->dns_sec_addr);
            if (ret < 0) {
                entry->dns_sec_addr = RIL_OMITTED_STRING_PARAM;
                ret = 0;
            }

            ret = at_tok_nextstr(&line, &entry->pcscf_prim_addr);
            if (ret < 0) {
                entry->pcscf_prim_addr = RIL_OMITTED_STRING_PARAM;
                ret = 0;
            }

            ret = at_tok_nextstr(&line, &entry->pcscf_sec_addr);
            if (ret < 0) {
                entry->pcscf_sec_addr = RIL_OMITTED_STRING_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &entry->im_cn_signalling_flag);
            if (ret < 0) {
                entry->im_cn_signalling_flag = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &entry->lipa_indication);
            if (ret < 0) {
                entry->lipa_indication = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            /* IPv4 MTU */
            ret = at_tok_nextint(&line, &entry->ipv4_mtu);
            if (ret < 0) {
                entry->ipv4_mtu = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            /* WLAN offload */
            ret = at_tok_nextint(&line, &entry->serving_plmn_rate_control_value);

            /* local addr ind */
            ret = at_tok_nextint(&line, &entry->serving_plmn_rate_control_value);

            /* Non-IP MTU*/
            ret = at_tok_nextint(&line, &entry->nonip_mtu);
            if (ret < 0) {
                entry->nonip_mtu = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }

            ret = at_tok_nextint(&line, &entry->serving_plmn_rate_control_value);
            if (ret < 0) {
                entry->serving_plmn_rate_control_value = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }
            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.pdp_context = list_head;
    response.array_num = message_num;

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}



/* AT cmd: AT+CGDEL */
int32_t ril_response_cgdel_delete_non_active_pdp_contexts(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_delete_non_active_pdp_contexts_rsp_t response;
    int32_t *entry, *list_head;

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
    response.array_num = 0;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */

            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            do {
                if (message_num == 0) {
                    list_head = (int32_t *)ril_mem_malloc(sizeof(int32_t));
                    entry = list_head;
                } else {
                    list_head = ril_mem_realloc(list_head, sizeof(int32_t) * (message_num + 1));
                    entry = list_head + message_num;
                }
                ret = at_tok_nextint(&line, entry);
                if (ret < 0) {
                    ret = 0;
                    message_num--;
                    break;
                }
                message_num++;
            }  while (1);
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.cid_array = list_head;
    response.array_num = message_num;

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}



/* AT cmd: AT+CGAUTH */
int32_t ril_response_cgauth_define_pdp_context_authentication_parameters(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_define_pdp_context_authentication_parameters_rsp_t response;
    ril_authentication_pdp_struct_t *entry, *list_head;

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
    response.array_num = 0;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (message_num == 0) {
                list_head = (ril_authentication_pdp_struct_t *)ril_mem_malloc(sizeof(ril_authentication_pdp_struct_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_authentication_pdp_struct_t) * (message_num + 1));
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

            ret = at_tok_nextint(&line, &entry->auth_prot);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->userid);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->password);
            if (ret < 0) {
                break;
            }
            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.cntx_array = list_head;
    response.array_num = message_num;

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}



/* AT cmd: AT+CIPCA */
int32_t ril_response_cipca_initial_pdp_context_activation(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_initial_pdp_context_activation_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.attach_without_pdn);
            if (ret < 0) {
                response.attach_without_pdn = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
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



/* AT cmd: AT+CPSMS */
int32_t ril_response_cpsms_power_saving_mode_setting(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_power_saving_mode_setting_rsp_t response;
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

            ret = at_tok_nextstr(&line, &response.req_prdc_rau);
            if (ret < 0) {
                response.req_prdc_rau = RIL_OMITTED_STRING_PARAM;
                ret = 0;
            }

            ret = at_tok_nextstr(&line, &response.req_gprs_rdy_tmr);
            if (ret < 0) {
                response.req_gprs_rdy_tmr = RIL_OMITTED_STRING_PARAM;
                ret = 0;
            }

            ret = at_tok_nextstr(&line, &response.req_prdc_tau);
            if (ret < 0) {
                response.req_prdc_tau = RIL_OMITTED_STRING_PARAM;
                ret = 0;
            }

            ret = at_tok_nextstr(&line, &response.req_act_time);
            if (ret < 0) {
                response.req_act_time = RIL_OMITTED_STRING_PARAM;
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


/* AT cmd: AT+CCIOTOPT */
int32_t ril_response_ciot_optimisation_configuration(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_ciot_optimisation_configuration_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.supported_UE_opt);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.preferred_UE_opt);
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


/* AT cmd: AT+CEDRXS */
int32_t ril_response_eDRX_setting(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_eDRX_setting_rsp_t response;
    ril_eDRX_setting_struct_t *entry, *list_head;

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
    response.array_num = 0;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (message_num == 0) {
                list_head = (ril_eDRX_setting_struct_t *)ril_mem_malloc(sizeof(ril_eDRX_setting_struct_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_eDRX_setting_struct_t) * (message_num + 1));
                entry = list_head + message_num;
            }
            ret = at_tok_start(&line);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &entry->act_type);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextstr(&line, &entry->requested_eDRX_value);
            if (ret < 0) {
                break;
            }

            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.eDRX_setting = list_head;
    response.array_num = message_num;

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}


/* AT cmd: AT+CEDRXRDP */
int32_t ril_response_read_eDRX_dynamic_parameters(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_read_eDRX_dynamic_parameters_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.act_type);
            if (ret < 0) {
                break;
            }

            response.requested_eDRX_value = NULL;
            response.nw_provided_eDRX_value = NULL;
            response.paging_time_window = NULL;
            ret = at_tok_nextstr(&line, &response.requested_eDRX_value);
            if (ret < 0) {
                ret = 0;
            }

            ret = at_tok_nextstr(&line, &response.nw_provided_eDRX_value);
            if (ret < 0) {
                ret = 0;
            }

            ret = at_tok_nextstr(&line, &response.paging_time_window);
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


/* AT cmd: AT+CGAPNRC */
int32_t ril_response_APN_rate_control(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_APN_rate_control_rsp_t response;
    ril_APN_rate_control_struct_t *entry, *list_head;

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
    response.array_num = 0;
    while (need_parse && line != NULL) {
        if (get_final_response(line, &res_code)) {
            break;
        } else {
            /* information response */
            if (message_num == 0) {
                list_head = (ril_APN_rate_control_struct_t *)ril_mem_malloc(sizeof(ril_APN_rate_control_struct_t));
                entry = list_head;
            } else {
                list_head = ril_mem_realloc(list_head, sizeof(ril_APN_rate_control_struct_t) * (message_num + 1));
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

            ret = at_tok_nextint(&line, &entry->additional_exception_reports);
            if (ret < 0) {
                ret = 0;
                entry->additional_exception_reports = RIL_OMITTED_INTEGER_PARAM;
            }

            ret = at_tok_nextint(&line, &entry->uplink_time_unit);
            if (ret < 0) {
                ret = 0;
                entry->uplink_time_unit = RIL_OMITTED_INTEGER_PARAM;
            }

            ret = at_tok_nextint(&line, &entry->maximum_uplink_rate);
            if (ret < 0) {
                ret = 0;
                entry->maximum_uplink_rate = RIL_OMITTED_INTEGER_PARAM;
            }

            message_num++;
        }

        /* get next line of string */
        line = at_get_one_line(&cur_pos);
    }
    response.param_list = list_head;
    response.array_num = message_num;

    RIL_CONSTRUCT_RESPONSE(cmd_response);
    if (rsp_cb) {
        (rsp_cb)(&cmd_response);
    }
    if (list_head) {
        ril_mem_free(list_head);
    }
    return 0;
}


/* AT cmd: AT+CSCON */
int32_t ril_response_signaling_connection_status(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_signaling_connection_status_rsp_t response;
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


/* AT cmd: AT+CCHO */
int32_t ril_response_open_uicc_logical_channel(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_open_uicc_logical_channel_rsp_t response;
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
            ret = at_tok_nextint(&line, &response.sessionid);
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


/* AT cmd: AT+CCHC */
int32_t ril_response_close_uicc_logical_channel(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_close_uicc_logical_channel_rsp_t response;
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
            ret = at_tok_nextstr(&line, &response.response_str);
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


/* AT cmd: AT+CGLA */
int32_t ril_response_generic_uicc_logical_channel_access(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_generic_uicc_logical_channel_access_rsp_t response;
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
            ret = at_tok_nextstr(&line, &response.response);
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


/* AT cmd: AT+CRCES */
int32_t ril_response_read_coverage_enhancement_status(uint32_t channel_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_channel_cntx_t *channel_p;
    ril_cmd_response_t cmd_response;
    ril_cmd_response_callback_t rsp_cb;
    ril_read_coverage_enhancement_status_rsp_t response;
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

            ret = at_tok_nextint(&line, &response.act);
            if (ret < 0) {
                break;
            }

            ret = at_tok_nextint(&line, &response.ce_level);
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


/*************************************************
 *                 URC handler
 *************************************************/
/* URC: +CREG */
int32_t ril_urc_creg_network_registration(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_network_registration_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.stat);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nexthexint(&line, &response.lac);
        if (ret < 0) {
            response.lac = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nexthexint(&line, &response.ci);
        if (ret < 0) {
            response.ci = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nextint(&line, &response.act);
        if (ret < 0) {
            response.act = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nextint(&line, &response.cause_type);
        if (ret < 0) {
            response.cause_type = response.reject_cause = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nextint(&line, &response.reject_cause);
        if (ret < 0) {
            response.reject_cause = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(RIL_URC_ID_CREG, &response, sizeof(response));
    }
    return 0;
}



/* URC: +CTZR */
int32_t ril_urc_ctzr_time_zone_reporting(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_time_zone_reporting_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextstr(&line, &response.tz);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nexthexint(&line, &response.dst);
        if (ret < 0) {
            response.dst = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nextstr(&line, &response.time);
        if (ret < 0) {
            response.time = RIL_OMITTED_STRING_PARAM;
            ret = 0;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(RIL_URC_ID_CTZR, &response, sizeof(response));
    }
    return 0;
}


/* URC: +CGEV*/
int32_t ril_urc_cgev_packet_domain_event_reporting(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_cgev_event_reporting_urc_t response;
    int32_t ret = -1;
    int32_t idx = 0;
    const char *prefix[RIL_URC_TYPE_MAX] = {
        "+CGEV: NW DETACH",
        "+CGEV: ME DETACH",
        "+CGEV: NW CLASS",
        "+CGEV: ME CLASS",
        "+CGEV: NW PDN ACT",
        "+CGEV: ME PDN ACT",
        "+CGEV: NW ACT",
        "+CGEV: ME ACT",
        "+CGEV: NW DEACT",
        "+CGEV: ME DEACT",
        "+CGEV: NW PDN DEACT",
        "+CGEV: ME PDN DEACT",
        /*"+CGEV: NW DEACT",
        "+CGEV: ME DEACT",*/
        "+CGEV: NW MODIFY",
        "+CGEV: ME MODIFY",
        "+CGEV: REJECT",
        "+CGEV: NW REACT"
    };
    const char param_format[RIL_URC_TYPE_MAX] = {
        0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
        0x03, 0x03, 0x00, 0x00, /*0x00, 0x00,*/ 0x00, 0x00,
        0x03, 0x03
    };

    cur_pos = cmd_buf;

    if ((line = at_get_one_line(&cur_pos)) == NULL) {
        return 0;
    }

    while (idx < RIL_URC_TYPE_MAX) {
        if (strstr(line, prefix[idx])) {
            line += strlen(prefix[idx]);
            break;
        }
        idx++;
    }

    if (idx >= RIL_URC_TYPE_MAX) {
        return 0;
    }
    /* information response */
    response.response_type = (ril_urc_cgev_type_enum)idx;
    do {
        if (param_format[idx] & 0x01) {
            ret = at_tok_nextstr(&line, (char **) (&response.response1));
            if (ret < 0) {
                response.response1.classb = RIL_OMITTED_STRING_PARAM;
                ret = 0;
            }
        } else {
            ret = at_tok_nextint(&line, (int32_t *) (&response.response1));
            if (ret < 0) {
                response.response1.cid = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }
        }

        if (param_format[idx] & 0x02) {
            ret = at_tok_nextstr(&line, (char **) (&response.response2));
            if (ret < 0) {
                response.response2.pdp_addr = RIL_OMITTED_STRING_PARAM;
                ret = 0;
            }
        } else {
            ret = at_tok_nextint(&line, (int32_t *) (&response.response2));
            if (ret < 0) {
                response.response2.cid = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }
        }

        if (param_format[idx] & 0x04) {
            ret = at_tok_nextstr(&line, (char **) (&response.response3));
            if (ret < 0) {
                response.response3.cid = (int) RIL_OMITTED_STRING_PARAM;
                ret = 0;
            }
        } else {
            ret = at_tok_nextint(&line, (int32_t *) (&response.response3));
            if (ret < 0) {
                response.response3.cid = RIL_OMITTED_INTEGER_PARAM;
                ret = 0;
            }
        }

    } while (0);

    if (ret >= 0) {
        ril_notify_event(RIL_URC_ID_CGEV, &response, sizeof(response));
    }
    return 0;
}



/* URC: +CGREG */
int32_t ril_urc_cgreg_gprs_network_registration_status(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_gprs_network_registration_status_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.stat);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nexthexint(&line, &response.lac);
        if (ret < 0) {
            response.lac = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nexthexint(&line, &response.ci);
        if (ret < 0) {
            response.ci = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nextint(&line, &response.act);
        if (ret < 0) {
            response.act = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nextbinint(&line, &response.rac);
        if (ret < 0) {
            response.rac = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nextint(&line, &response.cause_type);
        if (ret < 0) {
            response.cause_type = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nextint(&line, &response.reject_cause);
        if (ret < 0) {
            response.reject_cause = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nextbinint(&line, &response.active_time);
        if (ret < 0) {
            response.active_time = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nextbinint(&line, &response.periodic_rau);
        if (ret < 0) {
            response.periodic_rau = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nextbinint(&line, &response.gprs_readytimer);
        if (ret < 0) {
            response.gprs_readytimer = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(RIL_URC_ID_CGREG, &response, sizeof(response));
    }
    return 0;
}



/* URC: +CEREG */
int32_t ril_urc_cereg_eps_network_registration_status(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_eps_network_registration_status_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.stat);
        if (ret < 0) {
            break;
        }

        response.tac = RIL_OMITTED_INTEGER_PARAM;
        response.ci = RIL_OMITTED_INTEGER_PARAM;
        response.act = RIL_OMITTED_INTEGER_PARAM;
        response.cause_type = RIL_OMITTED_INTEGER_PARAM;
        response.reject_cause = RIL_OMITTED_INTEGER_PARAM;
        response.active_time = RIL_OMITTED_INTEGER_PARAM;
        response.periodic_tau = RIL_OMITTED_INTEGER_PARAM;

        ret = at_tok_nexthexint(&line, &response.tac);
        if (ret < 0) {
            response.tac = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nexthexint(&line, &response.ci);
        if (ret < 0) {
            response.ci = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nextint(&line, &response.act);
        if (ret < 0) {
            response.act = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nextint(&line, &response.cause_type);
        if (ret < 0) {
            response.cause_type = response.reject_cause = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nextint(&line, &response.reject_cause);
        if (ret < 0) {
            response.reject_cause = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nextbinint(&line, &response.active_time);
        if (ret < 0) {
            response.active_time = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }

        ret = at_tok_nextbinint(&line, &response.periodic_tau);
        if (ret < 0) {
            response.periodic_tau = RIL_OMITTED_INTEGER_PARAM;
            ret = 0;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(RIL_URC_ID_CEREG, &response, sizeof(response));
    }
    return 0;
}


/* URC: +CCIOTOPT */
int32_t ril_urc_ciot_optimisation_configuration(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_ciot_optimisation_configuration_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.supported_network_opt);
        if (ret < 0) {
            break;
        }

    } while (0);

    if (ret >= 0) {
        ril_notify_event(RIL_URC_ID_CCIOTOPT, &response, sizeof(response));
    }
    return 0;
}


/* URC: +CEDRXP */
int32_t ril_urc_eDRX_setting(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_eDRX_setting_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.act_type);
        if (ret < 0) {
            break;
        }

        response.requested_eDRX_value = NULL;
        response.nw_provided_eDRX_value = NULL;
        response.paging_time_window = NULL;
        ret = at_tok_nextstr(&line, &response.requested_eDRX_value);
        if (ret < 0) {
            ret = 0;
            break;
        }

        ret = at_tok_nextstr(&line, &response.nw_provided_eDRX_value);
        if (ret < 0) {
            ret = 0;
            break;
        }

        ret = at_tok_nextstr(&line, &response.paging_time_window);
        if (ret < 0) {
            ret = 0;
            break;
        }

    } while (0);

    if (ret >= 0) {
        ril_notify_event(RIL_URC_ID_CEDRXP, &response, sizeof(response));
    }
    return 0;
}


/* URC: +CPIN */
int32_t ril_urc_enter_pin(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_enter_pin_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextstr(&line, &response.code);
        if (ret < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(RIL_URC_ID_CPIN, &response, sizeof(response));        
    }
    return 0;
}


/* URC: +CSCON */
int32_t ril_urc_signaling_connection_status(ril_urc_id_t cmd_id, char *cmd_buf, uint32_t cmd_buf_len)
{
    char *cur_pos, *line;
    ril_signaling_connection_status_urc_t response;
    int32_t ret = -1;

    cur_pos = cmd_buf;

    line = at_get_one_line(&cur_pos);

    /* information response */
    do {
        ret = at_tok_start(&line);
        if (ret < 0) {
            break;
        }

        ret = at_tok_nextint(&line, &response.mode);
        if (ret < 0) {
            break;
        }
    } while (0);

    if (ret >= 0) {
        ril_notify_event(RIL_URC_ID_CSCON, &response, sizeof(response));
    }
    return 0;
}


/*************************************************
 *                 UT test case
 *************************************************/
#if defined(__RIL_UT_TEST_CASES__)
int32_t ril_response_ut_callback_27007(ril_cmd_response_t *cmd_response)
{
    RIL_LOGUT("final result code: %d\r\n", cmd_response->res_code);
    RIL_LOGUT("request mode: %d\r\n", cmd_response->mode);
    if (cmd_response->mode == RIL_TEST_MODE &&
        cmd_response->test_mode_str != NULL &&
        cmd_response->test_mode_str_len > 0) {
        RIL_LOGDUMPSTRUT("test mode str: %s\r\n", cmd_response->test_mode_str_len, cmd_response->test_mode_str);
    } else {
        switch (cmd_response->cmd_id) {
            case RIL_CMD_ID_CGMI: {
                ril_manufacturer_identification_rsp_t *response = (ril_manufacturer_identification_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("manufacturer: %s\r\n", response->manufacturer ? response->manufacturer : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CGMM: {
                ril_model_identification_rsp_t *response = (ril_model_identification_rsp_t *)cmd_response->cmd_param; 
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("model: %s\r\n", response->model ? response->model : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CGMR: {
                ril_revision_identification_rsp_t *response = (ril_revision_identification_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("revision: %s\r\n", response->revision ? response->revision : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CGSN: {
                ril_serial_number_rsp_t *response = (ril_serial_number_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("value: %s\r\n", response->value.sn ? response->value.sn : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CSCS: {
                ril_select_character_set_rsp_t *response = (ril_select_character_set_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("chset: %s\r\n", response->chset ? response->chset : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CIMI: {
                ril_imsi_rsp_t *response = (ril_imsi_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("imsi: %s\r\n", response->imsi ? response->imsi : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CMUX: {
                ril_cmux_param_rsp_t *response = (ril_cmux_param_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("mode: %d\r\n", (int) response->mode);
                    RIL_LOGUT("subset: %d\r\n", (int) response->subset);
                    RIL_LOGUT("port_speed: %d\r\n", (int) response->port_speed);
                    RIL_LOGUT("N1: %d\r\n", (int) response->N1);
                    RIL_LOGUT("T1: %d\r\n", (int) response->T1);
                    RIL_LOGUT("N2: %d\r\n", (int) response->N2);
                    RIL_LOGUT("T2: %d\r\n", (int) response->T2);
                    RIL_LOGUT("T3: %d\r\n", (int) response->T3);
                    RIL_LOGUT("k: %d\r\n", (int) response->k);
                }
            }
            break;

#ifndef __RIL_CMD_SET_SLIM_ENABLE__
            case RIL_CMD_ID_CR: {
                ril_service_reporting_control_rsp_t *response = (ril_service_reporting_control_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("mode: %d\r\n", (int) response->mode);
                }
            }
            break;
#endif

            case RIL_CMD_ID_CEER: {
                ril_extended_error_report_rsp_t *response = (ril_extended_error_report_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("report: %s\r\n", response->report ? response->report : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CNUM: {
                ril_subscriber_number_rsp_t *response = (ril_subscriber_number_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_ACTIVE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {                
                    int32_t idx;
                    RIL_LOGUT("array_num: %d\r\n", (int) response->array_num);
                    for (idx = 0; idx < response->array_num; idx++) {
                        RIL_LOGUT("idx: %d\r\n", (int) idx);
                        RIL_LOGUT("alpha: %s\r\n", response->entries_array[idx].alpha ? response->entries_array[idx].alpha : UT_OMITTED_PARAM);
                        RIL_LOGUT("number: %s\r\n", response->entries_array[idx].number ? response->entries_array[idx].number : UT_OMITTED_PARAM);
                        RIL_LOGUT("type: %d\r\n", (int) response->entries_array[idx].type);
                    }
                }
            }
            break;

#ifndef __RIL_CMD_SET_SLIM_ENABLE__
            case RIL_CMD_ID_CREG: {
                ril_network_registration_rsp_t *response = (ril_network_registration_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("n: %d\r\n", (int) response->n);
                    RIL_LOGUT("stat: %d\r\n", (int) response->stat);
                    RIL_LOGUT("lac: %d\r\n", (int) response->lac);
                    RIL_LOGUT("ci: %d\r\n", (int) response->ci);
                    RIL_LOGUT("act: %d\r\n", (int) response->act);
                    RIL_LOGUT("cause_type: %d\r\n", (int) response->cause_type);
                    RIL_LOGUT("reject_cause: %d\r\n", (int) response->reject_cause);
                }
            }
            break;
#endif

            case RIL_CMD_ID_COPS: {
                ril_plmn_selection_rsp_t *response = (ril_plmn_selection_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("mode: %d\r\n", (int) response->mode);
                    RIL_LOGUT("format: %d\r\n", (int) response->format);
                    RIL_LOGUT("oper: %s\r\n", response->oper ? response->oper : UT_OMITTED_PARAM);
                    RIL_LOGUT("act: %d\r\n", (int) response->act);
                }
            }
            break;

            case RIL_CMD_ID_CLCK: {
                ril_facility_lock_rsp_t *response = (ril_facility_lock_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {                
                    int32_t idx;
                    RIL_LOGUT("array_num: %d\r\n", (int) response->array_num);
                    for (idx = 0; idx < response->array_num; idx++) {
                        RIL_LOGUT("idx: %d\r\n", (int) idx);
                        RIL_LOGUT("classn: %d\r\n", (int) response->status_array[idx].classn);
                        RIL_LOGUT("status: %d\r\n", (int) response->status_array[idx].status);
                    }
                }
            }
            break;

            case RIL_CMD_ID_CPWD: {
                // no parameter
            }
            break;

            case RIL_CMD_ID_CPOL: {
                ril_preferred_plmn_list_rsp_t *response = (ril_preferred_plmn_list_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {                
                    int32_t idx;
                    RIL_LOGUT("array_num: %d\r\n", (int) response->array_num);
                    for (idx = 0; idx < response->array_num; idx++) {
                        RIL_LOGUT("idx: %d\r\n", (int) idx);
                        RIL_LOGUT("index: %d\r\n", (int) response->plmn_list[idx].index);
                        RIL_LOGUT("format: %d\r\n", (int) response->plmn_list[idx].format);
                        RIL_LOGUT("oper: %s\r\n", response->plmn_list[idx].oper ? response->plmn_list[idx].oper : UT_OMITTED_PARAM);
                        RIL_LOGUT("gsm_act: %d\r\n", (int) response->plmn_list[idx].gsm_act);
                        RIL_LOGUT("gsm_compact_act: %d\r\n", (int) response->plmn_list[idx].gsm_compact_act);
                        RIL_LOGUT("utran_act: %d\r\n", (int) response->plmn_list[idx].utran_act);
                        RIL_LOGUT("e_utran_act: %d\r\n", (int) response->plmn_list[idx].e_utran_act);
                    }
                }
            }
            break;

            case RIL_CMD_ID_CPLS: {
                ril_selection_of_preferred_plmn_list_rsp_t *response = (ril_selection_of_preferred_plmn_list_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("list: %d\r\n", (int) response->list);
                }
            }
            break;

            case RIL_CMD_ID_COPN: {
                ril_read_operator_names_rsp_t *response = (ril_read_operator_names_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {                
                    int32_t idx;
                    RIL_LOGUT("array_num: %d\r\n", (int) response->array_num);
                    for (idx = 0; idx < response->array_num; idx++) {
                        RIL_LOGUT("idx: %d\r\n", (int) idx);
                        RIL_LOGUT("numeric: %s\r\n", response->oper_name[idx].numeric ? response->oper_name[idx].numeric : UT_OMITTED_PARAM);
                        RIL_LOGUT("alpha: %s\r\n", response->oper_name[idx].alpha ? response->oper_name[idx].alpha : UT_OMITTED_PARAM);
                    }
                }
            }
            break;

            case RIL_CMD_ID_CFUN: {
                ril_set_phone_functionality_rsp_t *response = (ril_set_phone_functionality_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("fun: %d\r\n", (int) response->fun);
                } 
            }
            break;

            case RIL_CMD_ID_CPIN: {
                ril_enter_pin_rsp_t *response = (ril_enter_pin_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("code: %s\r\n", response->code ? response->code : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CSQ: {
                ril_signal_quality_rsp_t *response = (ril_signal_quality_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("rssi: %d\r\n", (int) response->rssi);
                    RIL_LOGUT("ber: %d\r\n", (int) response->ber);
                }
            }
            break;

            case RIL_CMD_ID_CCLK: {
                ril_clock_rsp_t *response = (ril_clock_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("time: %s\r\n", response->time ? response->time : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CSIM: {
                ril_generic_sim_access_rsp_t *response = (ril_generic_sim_access_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("length: %d\r\n", (int) response->length);
                    RIL_LOGUT("response: %s\r\n", response->response ? response->response : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CRSM: {
                ril_restricted_sim_access_rsp_t *response = (ril_restricted_sim_access_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("sw1: %d\r\n", (int) response->sw1);
                    RIL_LOGUT("sw2: %d\r\n", (int) response->sw2);
                    RIL_LOGUT("response: %s\r\n", response->response ? response->response : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CMAR: {
                // no parameter.
            }
            break;
            
            case RIL_CMD_ID_CTZU: {
                ril_automatic_time_zone_update_rsp_t *response = (ril_automatic_time_zone_update_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("onoff: %d\r\n", (int) response->onoff);
                }
            }
            break;

            case RIL_CMD_ID_CTZR: {
                ril_time_zone_reporting_rsp_t *response = (ril_time_zone_reporting_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("reporting: %d\r\n", (int) response->reporting);
                }
            }
            break;

            case RIL_CMD_ID_CGPIAF: {
                ril_printing_ip_address_format_rsp_t *response = (ril_printing_ip_address_format_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("ipv6_addr_format: %d\r\n", (int) response->ipv6_addr_format);
                    RIL_LOGUT("ipv6_subnetnotation: %d\r\n", (int) response->ipv6_subnetnotation);
                    RIL_LOGUT("ipv6_leadingzeros: %d\r\n", (int) response->ipv6_leadingzeros);
                    RIL_LOGUT("ipv6_compresszeros: %d\r\n", (int) response->ipv6_compresszeros);
                }
            }
            break;

            case RIL_CMD_ID_CPINR: {
                ril_remaining_pin_retries_rsp_t *response = (ril_remaining_pin_retries_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {                
                    int32_t idx;
                    RIL_LOGUT("array_num: %d\r\n", (int) response->array_num);
                    for (idx = 0; idx < response->array_num; idx++) {
                        RIL_LOGUT("idx: %d\r\n", (int) idx);
                        RIL_LOGUT("code: %s\r\n", response->retry_entries[idx].code ? response->retry_entries[idx].code : UT_OMITTED_PARAM);
                        RIL_LOGUT("retries: %d\r\n", (int)response->retry_entries[idx].retries);
                        RIL_LOGUT("default_retries: %d\r\n", (int)response->retry_entries[idx].default_retries);
                    }
                }
            }
            break;

#ifndef __RIL_CMD_SET_SLIM_ENABLE__
            case RIL_CMD_ID_CSUS: {
                ril_set_card_slot_rsp_t *response = (ril_set_card_slot_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("card_slot: %d\r\n", (int) response->card_slot);
                }
            }
            break;
#endif

            case RIL_CMD_ID_CESQ: {
                ril_extended_signal_quality_rsp_t *response = (ril_extended_signal_quality_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("rxlev: %d\r\n", (int) response->rxlev);
                    RIL_LOGUT("ber: %d\r\n", (int) response->ber);
                    RIL_LOGUT("rscp: %d\r\n", (int) response->rscp);
                    RIL_LOGUT("ecno: %d\r\n", (int) response->ecno);
                    RIL_LOGUT("rsrq: %d\r\n", (int) response->rsrq);
                    RIL_LOGUT("rsrp: %d\r\n", (int) response->rsrp);
                }
            }
            break;

            case RIL_CMD_ID_CMEE: {
                ril_report_mobile_termination_error_rsp_t *response = (ril_report_mobile_termination_error_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("n: %d\r\n", (int) response->n);
                }
            }
            break;

            case RIL_CMD_ID_CGDCONT: {            
                ril_define_pdp_context_rsp_t *response = (ril_define_pdp_context_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {                
                    int32_t idx;
                    RIL_LOGUT("array_num: %d\r\n", (int) response->array_num);
                    for (idx = 0; idx < response->array_num; idx++) {
                        RIL_LOGUT("idx: %d\r\n", (int) idx);
                        RIL_LOGUT("cid: %d\r\n", (int) response->pdp_context[idx].cid);
                        RIL_LOGUT("pdp_type: %s\r\n", response->pdp_context[idx].pdp_type ? response->pdp_context[idx].pdp_type : UT_OMITTED_PARAM);
                        RIL_LOGUT("apn: %s\r\n", response->pdp_context[idx].apn ? response->pdp_context[idx].apn : UT_OMITTED_PARAM);
                        RIL_LOGUT("pdp_addr: %s\r\n", response->pdp_context[idx].pdp_addr ? response->pdp_context[idx].pdp_addr : UT_OMITTED_PARAM);
                        RIL_LOGUT("d_comp: %d\r\n", (int) response->pdp_context[idx].d_comp);
                        RIL_LOGUT("h_comp: %d\r\n", (int) response->pdp_context[idx].h_comp);
                        RIL_LOGUT("ipv4addralloc: %d\r\n", (int) response->pdp_context[idx].ipv4addralloc);
                        RIL_LOGUT("request_type: %d\r\n", (int) response->pdp_context[idx].request_type);
                        RIL_LOGUT("pcscf_discovery: %d\r\n", (int) response->pdp_context[idx].pcscf_discovery);
                        RIL_LOGUT("im_cn_signalling_flag_ind: %d\r\n", (int) response->pdp_context[idx].im_cn_signalling_flag_ind);
                        RIL_LOGUT("nslpi: %d\r\n", (int) response->pdp_context[idx].nslpi);
                        RIL_LOGUT("securepco: %d\r\n", (int) response->pdp_context[idx].securepco);
                    }
                }
            }
            break;

            case RIL_CMD_ID_CGATT: {
                ril_ps_attach_or_detach_rsp_t *response = (ril_ps_attach_or_detach_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("state: %d\r\n", (int) response->state);
                }
            }
            break;

            case RIL_CMD_ID_CGACT: {
                ril_pdp_context_activate_or_deactivate_rsp_t *response = (ril_pdp_context_activate_or_deactivate_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL)  {
                    int32_t idx;
                    RIL_LOGUT("array_num: %d\r\n", (int) response->array_num);
                    for (idx = 0; idx < response->array_num; idx++) {
                        RIL_LOGUT("idx: %d\r\n", (int) idx);
                        RIL_LOGUT("state: %d\r\n", (int) response->cid_state[idx].state);
                        RIL_LOGUT("cid: %d\r\n", (int) response->cid_state[idx].cid);
                    }
                }
            }
            break;

            case RIL_CMD_ID_CGDATA: {
                // no parameter.
            }
            break;

            case RIL_CMD_ID_CGPADDR: {
                ril_show_pdp_address_rsp_t *response = (ril_show_pdp_address_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL){
                    int32_t idx;
                    RIL_LOGUT("array_num: %d\r\n", (int) response->array_num);
                    for (idx = 0; idx < response->array_num; idx++) {
                        RIL_LOGUT("idx: %d\r\n", (int) idx);
                        RIL_LOGUT("cid: %d\r\n", (int) response->cid_addr[idx].cid);
                        RIL_LOGUT("pdp_addr_1: %s\r\n", response->cid_addr[idx].pdp_addr_1 ? response->cid_addr[idx].pdp_addr_1 : UT_OMITTED_PARAM);
                        RIL_LOGUT("pdp_addr_2: %s\r\n", response->cid_addr[idx].pdp_addr_2 ? response->cid_addr[idx].pdp_addr_2 : UT_OMITTED_PARAM);
                    }
                }
            }
            break;

            case RIL_CMD_ID_CGEREP: {
                ril_packet_domain_event_reporting_rsp_t *response = (ril_packet_domain_event_reporting_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL){
                    RIL_LOGUT("cmode: %d\r\n", (int) response->cmode);
                    RIL_LOGUT("bfr: %d\r\n", (int) response->bfr);
                }
            }
            break;

#ifndef __RIL_CMD_SET_SLIM_ENABLE__
            case RIL_CMD_ID_CGREG: {
                ril_gprs_network_registration_status_rsp_t *response = (ril_gprs_network_registration_status_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("n: %d\r\n", (int) response->n);
                    RIL_LOGUT("stat: %d\r\n", (int) response->stat);
                    RIL_LOGUT("lac: %d\r\n", (int) response->lac);
                    RIL_LOGUT("ci: %d\r\n", (int) response->ci);
                    RIL_LOGUT("act: %d\r\n", (int) response->act);
                    RIL_LOGUT("rac: %d\r\n", (int) response->rac);
                    RIL_LOGUT("cause_type: %d\r\n", (int) response->cause_type);
                    RIL_LOGUT("reject_cause: %d\r\n", (int) response->reject_cause);
                    RIL_LOGUT("active_time: %d\r\n", (int) response->active_time);
                    RIL_LOGUT("periodic_rau: %d\r\n", (int) response->periodic_rau);
                    RIL_LOGUT("gprs_readytimer: %d\r\n", (int) response->gprs_readytimer);
                }
            }
            break;
#endif

            case RIL_CMD_ID_CEREG: {
                ril_eps_network_registration_status_rsp_t *response = (ril_eps_network_registration_status_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("n: %d\r\n", (int) response->n);
                    RIL_LOGUT("stat: %d\r\n", (int) response->stat);
                    RIL_LOGUT("tac: %d\r\n", (int) response->tac);
                    RIL_LOGUT("ci: %d\r\n", (int) response->ci);
                    RIL_LOGUT("act: %d\r\n", (int) response->act);
                    RIL_LOGUT("cause_type: %d\r\n", (int) response->cause_type);
                    RIL_LOGUT("reject_cause: %d\r\n", (int) response->reject_cause);
                    RIL_LOGUT("active_time: %d\r\n", (int) response->active_time);
                    RIL_LOGUT("periodic_tau: %d\r\n", (int) response->periodic_tau);
                }
            }
            break;

            case RIL_CMD_ID_CGCONTRDP: {
                ril_pdp_context_read_dynamic_parameters_rsp_t *response = (ril_pdp_context_read_dynamic_parameters_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL){
                    int32_t idx;
                    RIL_LOGUT("array_num: %d\r\n", (int) response->array_num);
                    for (idx = 0; idx < response->array_num; idx++) {
                        RIL_LOGUT("idx: %d\r\n", (int) idx);
                        RIL_LOGUT("cid: %d\r\n", (int) response->pdp_context[idx].cid);
                        RIL_LOGUT("bearer_id: %d\r\n", (int) response->pdp_context[idx].bearer_id);
                        RIL_LOGUT("apn: %s\r\n", response->pdp_context[idx].apn ? response->pdp_context[idx].apn : UT_OMITTED_PARAM);
                        RIL_LOGUT("local_addr_and_subnet_mask: %s\r\n", response->pdp_context[idx].local_addr_and_subnet_mask ? response->pdp_context[idx].local_addr_and_subnet_mask : UT_OMITTED_PARAM);
                        RIL_LOGUT("gw_addr: %s\r\n", response->pdp_context[idx].gw_addr ? response->pdp_context[idx].gw_addr : UT_OMITTED_PARAM);
                        RIL_LOGUT("dns_prim_addr: %s\r\n", response->pdp_context[idx].dns_prim_addr ? response->pdp_context[idx].dns_prim_addr : UT_OMITTED_PARAM);
                        RIL_LOGUT("dns_sec_addr: %s\r\n", response->pdp_context[idx].dns_sec_addr ? response->pdp_context[idx].dns_sec_addr : UT_OMITTED_PARAM);
                        RIL_LOGUT("pcscf_prim_addr: %s\r\n", response->pdp_context[idx].pcscf_prim_addr ? response->pdp_context[idx].pcscf_prim_addr : UT_OMITTED_PARAM);
                        RIL_LOGUT("pcscf_sec_addr: %s\r\n", response->pdp_context[idx].pcscf_sec_addr ? response->pdp_context[idx].pcscf_sec_addr : UT_OMITTED_PARAM);
                        RIL_LOGUT("im_cn_signalling_flag: %d\r\n", (int) response->pdp_context[idx].im_cn_signalling_flag);
                        RIL_LOGUT("lipa_indication: %d\r\n", (int) response->pdp_context[idx].lipa_indication);
                        RIL_LOGUT("IPv4_MTU: %d\r\n", (int) response->pdp_context[idx].ipv4_mtu);
                        RIL_LOGUT("NonIP_MTU: %d\r\n", (int) response->pdp_context[idx].nonip_mtu);
                        RIL_LOGUT("serving_plmn_rate_control_value: %d\r\n", (int) response->pdp_context[idx].serving_plmn_rate_control_value);
                    }
                }
            }
            break;

            case RIL_CMD_ID_CGDEL: {
                ril_delete_non_active_pdp_contexts_rsp_t *response = (ril_delete_non_active_pdp_contexts_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL){
                    int32_t idx;
                    RIL_LOGUT("array_num: %d\r\n", (int) response->array_num);
                    for (idx = 0; idx < response->array_num; idx++) {
                        RIL_LOGUT("idx: %d\r\n", (int) idx);
                        RIL_LOGUT("cid: %d\r\n", (int) response->cid_array[idx]);
                    }
                }
            }
            break;

            case RIL_CMD_ID_CGAUTH: {
                ril_define_pdp_context_authentication_parameters_rsp_t *response = (ril_define_pdp_context_authentication_parameters_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL){
                    int32_t idx;
                    RIL_LOGUT("array_num: %d\r\n", (int) response->array_num);
                    for (idx = 0; idx < response->array_num; idx++) {
                        RIL_LOGUT("idx: %d\r\n", (int) idx);
                        RIL_LOGUT("cid: %d\r\n", (int) response->cntx_array[idx].cid);
                        RIL_LOGUT("auth_prot: %d\r\n", (int) response->cntx_array[idx].auth_prot);
                        RIL_LOGUT("userid: %s\r\n", response->cntx_array[idx].userid ? response->cntx_array[idx].userid : UT_OMITTED_PARAM);
                        RIL_LOGUT("password: %s\r\n", response->cntx_array[idx].password ? response->cntx_array[idx].password : UT_OMITTED_PARAM);
                    }
                }
            }
            break;

            case RIL_CMD_ID_CIPCA: {           
                ril_initial_pdp_context_activation_rsp_t *response = (ril_initial_pdp_context_activation_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
                    RIL_LOGUT("n: %d\r\n", (int) response->n);
                    RIL_LOGUT("attach_without_pdn: %d\r\n", (int) response->attach_without_pdn);
                }
            }
            break;

            case RIL_CMD_ID_CPSMS: {
                ril_power_saving_mode_setting_rsp_t *response = (ril_power_saving_mode_setting_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL){
                    RIL_LOGUT("mode: %d\r\n", (int) response->mode);
                    RIL_LOGUT("req_prdc_rau: %s\r\n", response->req_prdc_rau ? response->req_prdc_rau : UT_OMITTED_PARAM);
                    RIL_LOGUT("req_gprs_rdy_tmr: %s\r\n", response->req_gprs_rdy_tmr ? response->req_gprs_rdy_tmr : UT_OMITTED_PARAM);
                    RIL_LOGUT("req_prdc_tau: %s\r\n", response->req_prdc_tau ? response->req_prdc_tau : UT_OMITTED_PARAM);
                    RIL_LOGUT("req_act_time: %s\r\n", response->req_act_time ? response->req_act_time : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CCIOTOPT: {
                ril_ciot_optimisation_configuration_rsp_t *response = (ril_ciot_optimisation_configuration_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("n: %d\r\n", (int)response->n);
                    RIL_LOGUT("supported_UE_opt: %d\r\n", (int)response->supported_UE_opt);
                    RIL_LOGUT("preferred_UE_opt: %d\r\n", (int)response->preferred_UE_opt);
                }
            }
            break;

            case RIL_CMD_ID_CEDRXS: {
                ril_eDRX_setting_rsp_t *response = (ril_eDRX_setting_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {                
                    int32_t array_num, i;
                    array_num = response->array_num;
                    if (array_num > 0) {
                        for (i = 0; i < array_num; i++) {
                            RIL_LOGUT("entry#%d, act_type: %d\r\n", (int)i + 1, (int)response->eDRX_setting[i].act_type);
                            RIL_LOGUT("entry#%d, requested_eDRX_value: %s\r\n", (int)i + 1, response->eDRX_setting[i].requested_eDRX_value ? response->eDRX_setting[i].requested_eDRX_value : UT_OMITTED_PARAM);
                        }
                    }
                }
            }
            break;

            case RIL_CMD_ID_CEDRXRDP: {
                ril_read_eDRX_dynamic_parameters_rsp_t *response = (ril_read_eDRX_dynamic_parameters_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_ACTIVE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("act_type: %d\r\n", (int)response->act_type);
                    RIL_LOGUT("requested_eDRX_value: %s\r\n", response->requested_eDRX_value ? response->requested_eDRX_value : UT_OMITTED_PARAM);
                    RIL_LOGUT("nw_provided_eDRX_value: %s\r\n", response->nw_provided_eDRX_value ? response->nw_provided_eDRX_value : UT_OMITTED_PARAM);
                    RIL_LOGUT("paging_time_window: %s\r\n", response->paging_time_window ? response->paging_time_window : UT_OMITTED_PARAM);
                }
            }
            break;

            case RIL_CMD_ID_CGAPNRC: {
                ril_APN_rate_control_rsp_t *response = (ril_APN_rate_control_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode== RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    int32_t array_num, i;
                    array_num = response->array_num;
                    if (array_num > 0) {
                        for (i = 0; i < array_num; i++) {
                            RIL_LOGUT("entry#%d, cid: %d\r\n", (int)i + 1, (int)response->param_list[i].cid);
                            RIL_LOGUT("entry#%d, additional_exception_reports: %d\r\n", (int)i + 1, (int)response->param_list[i].additional_exception_reports);
                            RIL_LOGUT("entry#%d, uplink_time_unit: %d\r\n", (int)i + 1, (int)response->param_list[i].uplink_time_unit);
                            RIL_LOGUT("entry#%d, maximum_uplink_rate: %d\r\n", (int)i + 1, (int)response->param_list[i].maximum_uplink_rate);
                        }
                    }
                }
            }
            break;

            case RIL_CMD_ID_CSCON: {
                ril_signaling_connection_status_rsp_t *response = (ril_signaling_connection_status_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_READ_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("n: %d\r\n", (int)response->n);
                    RIL_LOGUT("mode: %d\r\n", (int)response->mode);
                }
            }
            break;

            // here are reference test commands for +CCHO, +CCHC, +CGLA:
            // AT+CCHO="A000000087"
            // AT+CCHC=1
            // AT+CGLA=1,10,"81F2000000"
            // AT+CGLA=1,10,"01B000000A"
            // AT+CGLA=1,16,"01A40004026F3200"
            case RIL_CMD_ID_CCHO: {
                ril_open_uicc_logical_channel_rsp_t *response = (ril_open_uicc_logical_channel_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("sessionid: %d\r\n", (int)response->sessionid);
                }
            }
            break;

            case RIL_CMD_ID_CCHC: {
                ril_close_uicc_logical_channel_rsp_t *response = (ril_close_uicc_logical_channel_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("response_str: %s\r\n", response->response_str);
                }
            }
            break;

            case RIL_CMD_ID_CGLA: {
                ril_generic_uicc_logical_channel_access_rsp_t *response = (ril_generic_uicc_logical_channel_access_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_EXECUTE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("length: %d\r\n", (int)response->length);
                    RIL_LOGUT("response: %s\r\n", response->response);
                }
            }
            break;

            case RIL_CMD_ID_CRCES: {
                ril_read_coverage_enhancement_status_rsp_t *response = (ril_read_coverage_enhancement_status_rsp_t *)cmd_response->cmd_param;
                if (cmd_response->mode == RIL_ACTIVE_MODE && cmd_response->res_code == RIL_RESULT_CODE_OK) {
                    RIL_LOGUT("act: %d\r\n", (int)response->act);
                    RIL_LOGUT("ce_level: %d\r\n", (int)response->ce_level);
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

