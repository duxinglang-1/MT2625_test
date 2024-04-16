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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "lwm2m_cli.h"
#include "syslog.h"
#include "liblwm2m.h"

#ifdef LWM2M_SERVER_MODE

extern void prv_output_clients_ex(uint8_t len, char *param[]);
extern void prv_read_client_ex(uint8_t len, char *param[]);
extern void prv_discover_client_ex(uint8_t len, char *param[]);
extern void prv_write_client_ex(uint8_t len, char *param[]);
extern void prv_time_client_ex(uint8_t len, char *param[]);
extern void prv_attr_client_ex(uint8_t len, char *param[]);
extern void prv_clear_client_ex(uint8_t len, char *param[]);
extern void prv_exec_client_ex(uint8_t len, char *param[]);
extern void prv_delete_client_ex(uint8_t len, char *param[]);
extern void prv_create_client_ex(uint8_t len, char *param[]);
extern void prv_observe_client_ex(uint8_t len, char *param[]);
extern void prv_cancel_client_ex(uint8_t len, char *param[]);
extern void prv_s_quit(char * buffer, void * user_data);

cmd_t lwm2m_s_cli[] = {
    { "-l",        "list",               prv_output_clients_ex   },
    { "-r",        "read",               prv_read_client_ex   },
    { "-f",        "Discover",           prv_discover_client_ex     },
    { "-w",        "Write",              prv_write_client_ex    },
    { "-t",        "time",               prv_time_client_ex    },
    { "-a",        "attributes",         prv_attr_client_ex    },
    { "-cl",       "clear",              prv_clear_client_ex    },
    { "-e",        "exec",               prv_exec_client_ex    },
    { "-d",        "del",                prv_delete_client_ex    },
    { "-c",        "create",             prv_create_client_ex    },
    { "-o",        "observe",            prv_observe_client_ex    },
    { "-cc",       "Cancel",             prv_cancel_client_ex    },
    { "-q",        "quit",               prv_s_quit    },
    { NULL }

};
#endif
#ifdef LWM2M_CLIENT_MODE
#if 0
extern void prv_output_servers(char * buffer, void * user_data);
extern void prv_change(char * buffer, void * user_data);
extern void prv_update(char * buffer,  void * user_data);
#ifdef LWM2M_BOOTSTRAP
extern void prv_initiate_bootstrap(char * buffer, void * user_data);
extern void prv_display_backup(char * buffer, void * user_data);
#endif
extern void prv_object_list(char * buffer, void * user_data);
extern void prv_display_objects(char * buffer, void * user_data);
extern void prv_object_dump(char * buffer, void * user_data);
extern void prv_add(char * buffer, void * user_data);
extern void prv_remove(char * buffer, void * user_data);
extern void prv_c_quit(char * buffer, void * user_data);

cmd_t lwm2m_c_cli[] = {
    { "-ls",       "list server",               prv_output_servers   },
    { "-c",        "change",                    prv_change   },
    { "-u",        "update",                    prv_update     },
#ifdef LWM2M_BOOTSTRAP
    { "-b",        "bootstrap",                 prv_initiate_bootstrap   },
    { "-db",       "Display backup objects",    prv_display_backup   },
    { "-do",       "disp current objects",      prv_display_objects   },
#endif
    { "-lo",       "List Objects",              prv_object_list     },
    { "-d",        "dump ",                     prv_object_dump   },
    { "-a",        "add",                       prv_add     },
    { "-r",        "rm",                        prv_remove     },
    { "-q",        "quit",                      prv_c_quit     },
    { NULL }
};
#else
extern void prv_c_quit(char * buffer, void * user_data);

cmd_t lwm2m_c_cli[] = {
    { "-q",        "quit",                      prv_c_quit     },
    { NULL }
};
#endif
#endif

