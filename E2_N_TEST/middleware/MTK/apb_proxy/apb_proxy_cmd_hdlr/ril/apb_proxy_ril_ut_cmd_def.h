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


#ifndef __APB_PROXY_RIL_UT_CMD_DEF__H__
#define __APB_PROXY_RIL_UT_CMD_DEF__H__

/**
 * format: AT+RILUTS=<cmd head>,<mode>,<param1>,<param2>,<para3>,...
 * mode:  0 -execute mode; 1 - active mode; 2 - read mode; 3 - test mode
 * notice: string param should be wrapped by "".
 *            if want to set a omitted param, please pass oint,  ostr.
 *            oint means omitted int,  ostr means omitted string
 * example: AT+RILUTS=+CMGS,0,14,"hello, world!"
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+RILUTS",          apb_proxy_hdlr_ril_ut_send)
/**
 * format: AT+RILUTR=<channel id>,<response string>
 * channel id:  should be equal to the channel through which command is sent, you can refer sending command log to get it.
 * example: AT+RILUTR=1,ATI0\r\nMTK2\r\nMAUI.11CW1418IOTMP.W16.53\r\nOK\r\n
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+RILUTR",          apb_proxy_hdlr_ril_ut_response)
/**
 * format: AT+RILUTU=<unsolicited result code string>
 * example: AT+RILUTU=+CGREG:xxxxxx
 */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+RILUTU",          apb_proxy_hdlr_ril_ut_urc)
/* ril ut test function */
AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+RIL",             apb_proxy_hdlr_ril_ut_test_func)

AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+RILCUSTOM",       apb_proxy_hdlr_ril_ut_send_custom_command)

AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+RILTEST",         apb_proxy_hdlr_ril_ut_channel_test)



#endif/* __APB_PROXY_RIL_UT_CMD_DEF__H__ */

