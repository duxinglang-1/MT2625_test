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
 * Definitions for TMM buffer offsets used in the protocol stack.
 * The offsets make it possible to reuse the same buffer without having to
 * reallocate it as it passes through the stack.
 * The offsets cover the worst case scenarios (maximum number of bytes that
 * may need to be reserved by a particular task in different scenarios).
 * It is the responsibility of the protocol stack layer that allocates the
 * TMM buffer to make sure enough space is reserved in the header and tail
 * when it passes data to the next layer.
 *
 * The following illustrations help clarify the requirements for the offsets:
 *
 * 27.010 MUX frame format (Basic Option):
 * \code
 * +----+----+----+----+----------------------------------------+-----+----+
 * |Flag|Addr|Ctrl|Len |            Information                 | FCS |Flag|
 * | (1)| (1)| (1)| (2)|                                        | (1) | (1)|
 * +----+----+----+----+----------------------------------------+-----+----+
 * \endcode
 *
 * Note that the 27.010 MUX offset requirements are multiplied by 2, in order
 * to support stacked 27.010 MUX to the PC (one between the modem and the
 * apps processor, and one between the PC and the modem running over the
 * first).
 *
 * PPP frame format:
 * \code
 * +----+----+----+--------+------------------------------------+-----+----+
 * |Flag|Addr|Ctrl|Protocol|            Information             | FCS |Flag|
 * | (1)| (1)| (1)|   (2)  |                                    | (2) | (1)|
 * +----+----+----+--------+------------------------------------+-----+----+
 * \endcode
 *
 */

#ifndef PS_OFFSETS_H
#define PS_OFFSETS_H

/**************************************************************************
 * Nested Include Files
 **************************************************************************/

/**************************************************************************
 * Manifest Constants
 **************************************************************************/

/** Number of bytes in 27.010 MUX header */
#define NUM_BYTES_HEADER_27010_MUX              (5)
/** Number of bytes in 27.010 MUX tail */
#define NUM_BYTES_TAIL_27010_MUX                (2)
/** Maximum number of stacked 27.010 MUXes (currently 2) */
#define MAX_NUM_STACKED_MUXES                   (2)

/** Number of bytes in PPP Protocol field (this should be considered when reserving
 * space in DL)
 */
#define NUM_BYTES_HEADER_PPP_PROTOCOL           (2)

/** Number of bytes that should be reserved for in-situ RFC1144 header compression */
#define RESERVED_OFFSET_LEN_HEADER_RFC1144      (128)

/** Maximum number of bytes in PDCP header */
#define MAX_NUM_BYTES_HEADER_PDCP               (3)

/** Number of bytes that should be reserved by 3G URLC at the beginning of the buffer in the DL direction */
#define RESERVED_OFFSET_LEN_DL_HEADER_URLC      ((MAX_NUM_STACKED_MUXES * NUM_BYTES_HEADER_27010_MUX) + NUM_BYTES_HEADER_PPP_PROTOCOL)
/** Number of bytes that should be reserved by 3G URLC at the end of the buffer in the DL direction */
#define RESERVED_OFFSET_LEN_DL_TAIL_URLC        (MAX_NUM_STACKED_MUXES * NUM_BYTES_TAIL_27010_MUX)

/** Number of bytes that should be reserved by UPDCP at the beginning of the buffer in the DL direction */
#define RESERVED_OFFSET_LEN_DL_HEADER_UPDCP     ((MAX_NUM_STACKED_MUXES * NUM_BYTES_HEADER_27010_MUX) + NUM_BYTES_HEADER_PPP_PROTOCOL)
/** Number of bytes that should be reserved by UPDCP at the end of the buffer in the DL direction */
#define RESERVED_OFFSET_LEN_DL_TAIL_UPDCP       (MAX_NUM_STACKED_MUXES * NUM_BYTES_TAIL_27010_MUX)

/** Number of bytes that should be reserved by 2G RLC/MAC (RD) at the beginning of the buffer in the DL direction */
#define RESERVED_OFFSET_LEN_DL_HEADER_RD        ((MAX_NUM_STACKED_MUXES * NUM_BYTES_HEADER_27010_MUX) + NUM_BYTES_HEADER_PPP_PROTOCOL)
/** Number of bytes that should be reserved by 2G RLC/MAC (RD) at the end of the buffer in the DL direction */
#define RESERVED_OFFSET_LEN_DL_TAIL_RD          (MAX_NUM_STACKED_MUXES * NUM_BYTES_TAIL_27010_MUX)

/** Number of bytes that should be reserved by SNDCP at the beginning of the buffer in the DL direction */
#define RESERVED_OFFSET_LEN_DL_HEADER_SNDCP     ((MAX_NUM_STACKED_MUXES * NUM_BYTES_HEADER_27010_MUX) + NUM_BYTES_HEADER_PPP_PROTOCOL)
/** Number of bytes that should be reserved by SNDCP at the end of the buffer in the DL direction */
#define RESERVED_OFFSET_LEN_DL_TAIL_SNDCP       (MAX_NUM_STACKED_MUXES * NUM_BYTES_TAIL_27010_MUX)

/** Number of bytes that should be reserved by PPP at the beginning of the buffer
 * PPP is special, because the same code could be running for both DL and UL directions
 * DL: When GPPP is connected to PC, running as a PPP server
 * UL: When SAT is using PPP client over a CSD connection to the network
 * The offsets below should be enough for the maximum requirements for header and tail offsets
 * The header offset will be used for either:
 * 27.010 MUX header in the DL direction (GPPP case, 2 * 5 bytes), or
 * PPP address, control, and protocol fields in the UL direction (SAT CSD PPP case, 4 bytes)
 */
#define RESERVED_OFFSET_LEN_HEADER_PPP          (MAX_NUM_STACKED_MUXES * NUM_BYTES_HEADER_27010_MUX)
/** Number of bytes that should be reserved by PPP at the end of the buffer
 * The tail offset will be used for either:
 * 27.010 MUX tail in the DL direction (GPPP case, 2 * 2 bytes), or
 * PPP FCS field in the UL direction (SAT CSD PPP case, 2 bytes)
 */
#define RESERVED_OFFSET_LEN_TAIL_PPP            (MAX_NUM_STACKED_MUXES * NUM_BYTES_TAIL_27010_MUX)

/** Number of bytes that should be reserved by 64K CSD (RTFDP) at the beginning of the buffer in the DL direction */
#define RESERVED_OFFSET_LEN_DL_HEADER_64K_CSD   (MAX_NUM_STACKED_MUXES * NUM_BYTES_HEADER_27010_MUX)
/** Number of bytes that should be reserved by 64K CSD (RTFDP) at the end of the buffer in the DL direction */
#define RESERVED_OFFSET_LEN_DL_TAIL_64K_CSD     (MAX_NUM_STACKED_MUXES * NUM_BYTES_TAIL_27010_MUX)

/** Number of bytes that should be reserved by PPPAD at the beginning of the buffer in the DL direction */
#define RESERVED_OFFSET_LEN_DL_HEADER_PPPAD     (MAX_NUM_STACKED_MUXES * NUM_BYTES_HEADER_27010_MUX)
/** Number of bytes that should be reserved by PPPAD at the end of the buffer in the DL direction */
#define RESERVED_OFFSET_LEN_DL_TAIL_PPPAD       (MAX_NUM_STACKED_MUXES * NUM_BYTES_TAIL_27010_MUX)

/** Number of bytes that should be reserved by PPPAD at the beginning of the buffer in the UL direction */
#define RESERVED_OFFSET_LEN_UL_HEADER_PPPAD     (MAX_NUM_BYTES_HEADER_PDCP)

#endif  /* PS_OFFSETS_H */

/* END OF FILE */
