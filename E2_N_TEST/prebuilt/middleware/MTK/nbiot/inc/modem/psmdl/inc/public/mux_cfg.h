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
/*
 **************************************************************************/
/** \file
 * Compile time configuration of the MUX task.
 **************************************************************************/

#ifndef     MUX_CFG_H
#define     MUX_CFG_H

/***************************************************************************
 * Nested Include Files
 ***************************************************************************/

/***************************************************************************
 *   Manifest Constants
 ***************************************************************************/

/** \addtogroup SigCimux
 *
 * @{
 */
/** Maximum length of AT data per signal.
 * This value defines the maximum length of AT data transmitted in a single \link
 * #CiMuxAtDataReqTag CiMuxAtDataReq \endlink signal or \link #CiMuxAtDataIndTag CiMuxAtDataInd
 * \endlink signal.
 */
#if defined (ENABLE_LONG_AT_CMD_RSP)
#define CIMUX_MAX_AT_DATA_LENGTH            500
#else
#if defined (ENABLE_AT_ENG_MODE)
#define CIMUX_MAX_AT_DATA_LENGTH            500
#else
#define CIMUX_MAX_AT_DATA_LENGTH            128
#endif
#endif

/** @} */ /* End of SigCimux group */

#define MUX_VERSION_MAJOR             1     /**< MUX major version number */
#define MUX_VERSION_MINOR             0     /**< MUX minor version number */
#define MUX_VERSION_BUILD             0     /**< MUX build version number */

/** Maximum number of multiplexers.
 * In a dual processor environment there are 2 multiplexer instances needed: one for all
 * connections to the application processor and one for all connections to the external PC.
 */
#if defined (ENABLE_DUAL_PROCESSOR_ENVIRONMENT)
#define MUX_MAX_NO_OF_MUX             2
#else
#define MUX_MAX_NO_OF_MUX             1
#endif

/* TODO: Make sure this define is not defined when UPGRADE_NASMDL2 is defined
 * use MAX_NUM_MDL_CHANNELS instead when UPGRADE_NASMDL2 is defined
 */
#if !defined (UPGRADE_NASMDL2)
#define MUX_MAX_NO_OF_CHANNELS       16   /**< Maximum number of channels */
#endif

/** DLCI which is used for the proxy channel.
 * A proxy channel is a data connection to an external PC which is tunneled through the
 * application processor-modem processor 27.010 multiplexer connection. This value is, therefore,
 * only required in a dual processor environment. The value is also limited to muxIndex 0 because
 * muxIndex 0 will handle the application processor-modem processor 27.010 multiplexer connection.
 */
#if defined (ENABLE_DUAL_PROCESSOR_ENVIRONMENT)
#define MUX_PROXY_DLCI_ON_MUX_0       7
#endif

/** DLCI which is used for the genie channel.
 * The genie channel transfers logging data to an external PC. The value is, therefore, limited to
 * the muxIndex which handles connections to the external PC.
 */
#if defined (ENABLE_DUAL_PROCESSOR_ENVIRONMENT)
#define MUX_GENIE_DLCI_ON_MUX_1      61
#else
#define MUX_GENIE_DLCI_ON_MUX_0      61
#endif

/** Default maximum data length in uplink L4MM buffers. */
#define MUX_DEFAULT_MAX_RX_DATA_LENGTH      (CIMUX_MAX_AT_DATA_LENGTH)
#define MUX_DEFAULT_RX_BUFFER_OFFSET        (0)     /**< Default offset in uplink l4MM buffers */
#define MUX_DEFAULT_RX_BUFFER_TAIL_SPACE    (0)     /**< Default tail space in uplink l4MM buffers */

#define MUX_DEFAULT_RX_WINDOW_SIZE          (1)     /**< Default Rx data signal window size on the MUXCONN interface */
#define MUX_DEFAULT_TX_WINDOW_SIZE          (1)     /**< Default Tx data signal window size on the MUXCONN interface */

/***************************************************************************
 *   Macro Functions
 ***************************************************************************/

/***************************************************************************
 *   Enumeration Types
 ***************************************************************************/

/***************************************************************************
 *   Structure Types
 ***************************************************************************/

#endif /* !MUX_CFG_H */

/* End of mux_cfg.h */


