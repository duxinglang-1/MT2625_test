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

#ifndef NB_CUSTOM_PORT_H
#define NB_CUSTOM_PORT_H

/********************************************************
 * Macro & Define
 *
 ********************************************************/

#define PORT_USER_EMMI         0
#define PORT_USER_CONN_LAYER   1
#define PORT_USER_CONN_LAYER_2 2
#define PORT_USER_CCCI         3
#define PORT_USER_GPS          4
#define PORT_USER_ULS          5


/*--------------------------------------------------------------------------
 *
 * Function:    nb_custom_get_default_port
 *
 * Parameters:  port_user    Indicates port user.
 *
 * Returns:     default port
 *
 * Description: Returns default port for given port user.
 *
 *-------------------------------------------------------------------------*/

#ifdef MTK_PORT_SERVICE_ENABLE

serial_port_dev_t  nb_custom_get_default_port( uint8_t port_user );
hal_uart_baudrate_t nb_custom_get_default_baud_rate( uint8_t port_user );

#endif


#endif /* NB_CUSTOM_PORT_H */

