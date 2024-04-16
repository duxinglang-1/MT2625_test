/* Copyright Statement:
*
* (C) 2005-2017  MediaTek Inc. All rights reserved.
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

/********************************************************
 * Includes
 *
 ********************************************************/

#include <stdint.h>

#include "nb_custom_port_config.h"
#include "nb_custom_port.h"

/********************************************************
 * Function declaration
 *
 ********************************************************/

#ifdef MTK_PORT_SERVICE_ENABLE

serial_port_dev_t  nb_custom_get_default_port( uint8_t port_user )
{
    serial_port_dev_t  return_value = SERIAL_PORT_DEV_UART_3;

    switch( port_user )
    {
        case PORT_USER_EMMI:
            return_value = DEFAULT_PORT_FOR_EMMI_TRACE;
            break;

        case PORT_USER_CONN_LAYER:
            return_value = DEFAULT_PORT_FOR_AT_CMD;
            break;

        case PORT_USER_CONN_LAYER_2:
            return_value = DEFAULT_PORT_FOR_AT_CMD_2;
            break;

        case PORT_USER_CCCI:
            return_value = DEFAULT_PORT_FOR_CCCI;
            break;

        case PORT_USER_GPS:
            return_value = DEFAULT_PORT_FOR_GPS;
            break;

        case PORT_USER_ULS:
            return_value = DEFAULT_PORT_FOR_ULS_TRACE;
            break;

        default:
            break;

    }
    return return_value;
}

hal_uart_baudrate_t nb_custom_get_default_baud_rate( uint8_t port_user )
{
    hal_uart_baudrate_t return_value = HAL_UART_BAUDRATE_115200;

    switch( port_user )
    {
        case PORT_USER_EMMI:
            return_value = DEFAULT_BAUDRATE_FOR_EMMI_TRACE;
            break;

        case PORT_USER_CONN_LAYER:
            return_value = DEFAULT_BAUDRATE_FOR_AT_CMD;
            break;

        case PORT_USER_CONN_LAYER_2:
            return_value = DEFAULT_BAUDRATE_FOR_AT_CMD_2;
            break;

        case PORT_USER_CCCI:
            return_value = DEFAULT_BAUDRATE_FOR_CCCI;
            break;

        case PORT_USER_GPS:
            return_value = DEFAULT_BAUDRATE_FOR_GPS;
            break;

        case PORT_USER_ULS:
            return_value = DEFAULT_BAUDRATE_FOR_ULS_TRACE;
            break;

        default:
            break;

    }
    return return_value;
}

#endif

/* replace macro MT2503_2625_DUAL by a customizable config definition */
#ifdef MT2503_2625_DUAL
/* enable mt2503/mt2625 project specific UART configs */
int mt2625_dual_mode_port_config = 1;
#else
int mt2625_dual_mode_port_config = 0;
#endif /* MT2503_2625_DUAL */

/* replace macro MTK_AUTO_BAUD_RATE_ENABLE by a customizable config definition */
#ifdef MTK_AUTO_BAUD_RATE_ENABLE
int auto_baud_rate_enable = 1;
#else
int auto_baud_rate_enable = 0;
#endif /* MTK_AUTO_BAUD_RATE_ENABLE */

/*  AT+IPR set with immediate effect, does not need to restart */
#ifdef MTK_IPR_WIE_WITHOUT_RESTART
int ipr_wie = 1;
#else
int ipr_wie = 0;
#endif
