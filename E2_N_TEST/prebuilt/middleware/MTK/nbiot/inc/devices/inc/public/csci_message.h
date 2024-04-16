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

#ifndef __CSCI_MESSAGE_H__
#define __CSCI_MESSAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

/** \defgroup CsciMessageInterface AP-MD CSCI Message Interface
 * This section introduces the CSCI Message interface which is used between on-chip
 * Application Processor domain (AP domain) and Modem (MD domain).
 * More specifically CSCI Messages are exchanged between the CMUX entity in the AP domain and
 * the MUX entity in the MD domain.
 *
 * CMUX and MUX use CSCI Messages to provide logical communication channels to higher layer entities
 * in their respective domain. These channels are then used for communication between
 * higher entities in the AP domain and higher layer entities in the MD domain. This is illustrated
 * in the Figure below:
 *
 * \image html Data_Mode_Architecture.png
 *
 * The low-level CSCI API provides a Block exchange mechanism between AP domain and MD domain.
 * This is used by CMUX and MUX to exchange Messages as shown in the Figure below:
 *
 * \image html CSCI_Message_Context.png
 *
 * Each new Message starts in a new Block. A Message may span over multiple Blocks.
 * The Figure below shows a CSCI Message in a single CSCI Block:
 *
 * \image html CSCI_Message_in_single_Block.png
 *
 * The Figure below shows a CSCI Message spanning over multiple CSCI Blocks:
 *
 * \image html CSCI_Message_in_single_Block.png
 *
 * The Figure below shows a stream of CSCI Messages:
 *
 * \image html Multiple_CSCI_Messages.png
 *
 * CMUX and MUX establish logical CSCI channels (LCCs) via the low-level CSCI Block exchange.
 * LCC creation is always initiated by the AP domain (i.e. by CMUX).
 * Each LCC maintains its own set of line states.
 * The lines RTS and CTS are used for LCC specific flow control of user data exchange.
 * It is recommended that both sides (CMUX and MUX) exchange line state information
 * as part of the LCC creation procedure. In case line states have not been exchanged
 * for a particular LCC yet then the following settings are assumed:
 * - DTR (Data Terminal Ready): on
 * - DSR (Data Set Ready): on
 * - RTS (Ready To Send): on
 * - CTS (Clear To Send): on
 * - DCD (Data Carrier Detect): off
 * - RI (Ring Indicator): off
 *
 *
 * The following MSC shows successful creation of a logical CSCI channel (LCC):
 *\msc
 *  "AP","MD";
 *
 *  "AP" -> "MD" [label="Channel Enable Request", url="\ref CSCI_MESSAGE_Channel_Enable_Request"];
 *  "AP" <- "MD" [label="Channel Enable Confirm", url="\ref CSCI_MESSAGE_Channel_Enable_Confirm"];
 *  "AP" -> "MD" [label="Line Status ( RTS=On )", url="\ref CSCI_MESSAGE_Line_Status"];
 *  "AP" <- "MD" [label="Line Status ( CTS=On )", url="\ref CSCI_MESSAGE_Line_Status"];
 *\endmsc
 *
 * The following MSC shows failed creation of a logical CSCI channel (LCC):
 *\msc
 *  "AP","MD";
 *
 *  "AP" -> "MD" [label="Channel Enable Request", url="\ref CSCI_MESSAGE_Channel_Enable_Request"];
 *  "AP" <- "MD" [label="Channel Disabled", url="\ref CSCI_MESSAGE_Channel_Disabled"];
 *\endmsc
 *
 * The following MSC shows an example of user data exchange with flow control:
 *\msc
 *  "AP","MD";
 *
 *  "AP" -> "MD" [label="User Data", url="\ref CSCI_MESSAGE_User_Data"];
 *  "AP" -> "MD" [label="User Data", url="\ref CSCI_MESSAGE_User_Data"];
 *  "AP" <- "MD" [label="Line Status ( CTS=Off )", url="\ref CSCI_MESSAGE_Line_Status"];
 *  ...;
 *  "AP" <- "MD" [label="Line Status ( CTS=On )", url="\ref CSCI_MESSAGE_Line_Status"];
 *  "AP" -> "MD" [label="User Data", url="\ref CSCI_MESSAGE_User_Data"];
 *  "AP" -> "MD" [label="User Data", url="\ref CSCI_MESSAGE_User_Data"];
 *\endmsc
 *
 * The following MSC shows destruction of a logical CSCI channel (LCC):
 *\msc
 *  "AP","MD";
 *
 *  "AP" -> "MD" [label="Channel Disabled", url="\ref CSCI_MESSAGE_Channel_Disabled"];
 *\endmsc
 *
 *
 * @{
 */



/** \defgroup CsciMessageFormat Message Format
 * This section describes the format of the messages exchanged on the CSCI interface.
 *
 * \section CSCI_MESSAGE_Generic_Format Generic Message Format
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Length                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Channel                            |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                             Type                              |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |    Data ...
 *          +-+-+-+-+
 *
 *    \endcode
 * Legend:
 * - All fields are transmitted in little-endian.
 * - Length – Total message length including this header (in bytes)
 * - Channel – Identifier of the logical CSCI channel (LCC) this message is for.
 *             There can be a maximum number of #CSCI_MAX_NO_OF_LCCS channels.
 *             Channel numbers range from zero to (#CSCI_MAX_NO_OF_LCCS - 1).
 * - Type – The Type field identifies the kind of CSCI message.
 *          The Type field can have the following values:
 *          - 1 - Channel Enable Request (sent from AP to MD) (#CSCI_MESSAGE_TYPE_CHANNEL_ENABLE_REQUEST)
 *          - 2 - Channel Enable Confirm (sent from MD to AP) (#CSCI_MESSAGE_TYPE_CHANNEL_ENABLE_CONFIRM)
 *          - 3 - Channel Disabled (#CSCI_MESSAGE_TYPE_CHANNEL_DISABLED)
 *          - 4 - User Data (#CSCI_MESSAGE_TYPE_USER_DATA)
 *          - 5 - Line Status (#CSCI_MESSAGE_TYPE_LINE_STATUS)
 * - Data – Message data (this field is optional and can be of arbitrary length)
 *
 *
 * \section CSCI_MESSAGE_Channel_Enable_Request Channel Enable Request
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                           Length=16                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Channel                            |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Type=1                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                         Channel Type                          |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *    \endcode
 * Legend:
 * - Channel – Identifier of the logical CSCI channel (LCC) this message is for.
 *             There can be a maximum number of #CSCI_MAX_NO_OF_LCCS channels.
 *             Channel numbers range from zero to (#CSCI_MAX_NO_OF_LCCS - 1).
 * - Channel Type – The type of the logical CSCI channel (LCC).
 *                  The Channel Type field can have the following values:
 *                  - 1 - AT and Data channel (#CSCI_CHANNEL_TYPE_AT_AND_DATA)
 *                  - 2 - AP Bridge channel (RPC channel for AP domain specific AT commands) (#CSCI_CHANNEL_TYPE_AP_BRIDGE)
 *                  - 3 - BIP channel (RPC channel between SIMAT in the MD domain and a Proxy in the AP domain) (#CSCI_CHANNEL_TYPE_BIP)
 *                  - 4 - SoftSIM channel (RPC channel between SoftSIM application in the AP domain and the SoftSIM driver in the MD domain) (#CSCI_CHANNEL_TYPE_SOFTSIM)
 *
 *
 * \section CSCI_MESSAGE_Channel_Enable_Confirm Channel Enable Confirm
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                           Length=12                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Channel                            |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Type=2                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *    \endcode
 * Legend:
 * - Channel – Identifier of the logical CSCI channel (LCC) this message is for.
 *             There can be a maximum number of #CSCI_MAX_NO_OF_LCCS channels.
 *             Channel numbers range from zero to (#CSCI_MAX_NO_OF_LCCS - 1).
 *
 *
 * \section CSCI_MESSAGE_Channel_Disabled Channel Disabled
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                           Length=12                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Channel                            |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Type=3                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *    \endcode
 * Legend:
 * - Channel – Identifier of the logical CSCI channel (LCC) this message is for.
 *             There can be a maximum number of #CSCI_MAX_NO_OF_LCCS channels.
 *             Channel numbers range from zero to (#CSCI_MAX_NO_OF_LCCS - 1).
 *
 *
 * \section CSCI_MESSAGE_User_Data User Data
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Length                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Channel                            |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Type=4                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |    User Data ...
 *          +-+-+-+-+-+-+-+
 *
 *    \endcode
 * Legend:
 * - Length – Total message length including this header (in bytes)
 * - Channel – Identifier of the logical CSCI channel (LCC) this message is for.
 *             There can be a maximum number of #CSCI_MAX_NO_OF_LCCS channels.
 *             Channel numbers range from zero to (#CSCI_MAX_NO_OF_LCCS - 1).
 * - User Data – The user data.
 *
 *
 * \section CSCI_MESSAGE_Line_Status Line Status
 *
 *    \code
 *
 *           0                   1                   2                   3
 *           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                           Length=36                           |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Channel                            |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                            Type=5                             |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                              DTR                              |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                              DSR                              |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                              RTS                              |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                              CTS                              |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                              DCD                              |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          |                              RI                               |
 *          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *    \endcode
 * Legend:
 * - Channel – Identifier of the logical CSCI channel (LCC) this message is for.
 *             There can be a maximum number of #CSCI_MAX_NO_OF_LCCS channels.
 *             Channel numbers range from zero to (#CSCI_MAX_NO_OF_LCCS - 1).
 * - DTR – Data Terminal Ready: send from AP to MD (0: off (#CSCI_CHANNEL_LINE_OFF); 1: on (#CSCI_CHANNEL_LINE_ON))
 *         (this field is ignored when message is sent from MD to AP)
 * - DSR – Data Set Ready: send from MD to AP (0: off (#CSCI_CHANNEL_LINE_OFF); 1: on (#CSCI_CHANNEL_LINE_ON))
 *         (this field is ignored when message is sent from AP to MD)
 * - RTS – Ready To Send: send from AP to MD (0: off (#CSCI_CHANNEL_LINE_OFF); 1: on (#CSCI_CHANNEL_LINE_ON))
 *         (this field is ignored when message is sent from MD to AP)
 * - CTS – Clear To Send: send from MD to AP (0: off (#CSCI_CHANNEL_LINE_OFF); 1: on (#CSCI_CHANNEL_LINE_ON))
 *         (this field is ignored when message is sent from AP to MD)
 * - DCD – Data Carrier Detect: send from MD to AP (0: off (#CSCI_CHANNEL_LINE_OFF); 1: on (#CSCI_CHANNEL_LINE_ON))
 *         (this field is ignored when message is sent from AP to MD)
 * - RI – Ring Indicator: send from MD to AP (0: off (#CSCI_CHANNEL_LINE_OFF); 1: on (#CSCI_CHANNEL_LINE_ON))
 *        (this field is ignored when message is sent from AP to MD)
 *
 * @{
 */

/* Channel number field */
#define CSCI_MAX_NO_OF_LCCS                         (64)    /**< Maximum number of Logical CSCI Channels (LCCs) */

/* Message Type field */
#define CSCI_MESSAGE_TYPE_CHANNEL_ENABLE_REQUEST    (1)     /**< Channel Enable Request (sent from AP to MD) */
#define CSCI_MESSAGE_TYPE_CHANNEL_ENABLE_CONFIRM    (2)     /**< Channel Enable Confirm (sent from MD to AP) */
#define CSCI_MESSAGE_TYPE_CHANNEL_DISABLED          (3)     /**< Channel Disabled */
#define CSCI_MESSAGE_TYPE_USER_DATA                 (4)     /**< User Data */
#define CSCI_MESSAGE_TYPE_LINE_STATUS               (5)     /**< Line Status */
#define CSCI_MESSAGE_TYPE_USER_DATA_IP              (6)     /**< IP User Data */

/* Channel Type field */
#define CSCI_CHANNEL_TYPE_AT_AND_DATA               (1)     /**< AT and Data channel */
#define CSCI_CHANNEL_TYPE_AP_BRIDGE                 (2)     /**< AP Bridge channel (RPC channel for AP domain specific AT commands) */
#define CSCI_CHANNEL_TYPE_BIP                       (3)     /**< BIP channel (RPC channel between SIMAT in the MD domain and a Proxy in the AP domain) */
#define CSCI_CHANNEL_TYPE_SOFTSIM                   (4)     /**< SoftSIM channel (RPC channel between SoftSIM application in the AP domain and the SoftSIM driver in the MD domain) */
#define CSCI_CHANNEL_TYPE_LBS                       (5)     /**< LBS channel (RPC channel between LBS application in the AP domain and the LBS task in the MD domain) */

/* Channel Line States for DTR, DSR, RTS, CTS, DCD and RI */
#define CSCI_CHANNEL_LINE_OFF                       (0)     /**< Line Status: Off */
#define CSCI_CHANNEL_LINE_ON                        (1)     /**< Line Status: On */

/** @} */ /* End of CsciMessageFormat group */

/** @} */ /* End of CsciMessageInterface group */

#ifdef __cplusplus
}
#endif


#endif /* !__CSCI_MESSAGE_H__ */

