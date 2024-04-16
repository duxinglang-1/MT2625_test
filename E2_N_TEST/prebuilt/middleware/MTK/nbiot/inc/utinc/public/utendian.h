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
 **************************************************************************
 * File Description:
 *
 * GPRS Endianness definition. Change endianness to compile on different
 * platforms
 **************************************************************************/

#ifndef UTENDIAN_H
#define UTENDIAN_H

#if !defined BYTE_ORDER
/*
Definitions for byte order,
according to byte significance from low address to high.
*/
#define LITTLE_ENDIAN 1234    /* least-significant byte first */
#define BIG_ENDIAN    4321    /* most-significant byte first */
#define PDP_ENDIAN    3412    /* LSB first in word, MSW first in long (pdp) */

#define BYTE_ORDER    LITTLE_ENDIAN

#endif

#define __byte_swap_long(x) ((((x) >> 24) & 0xFF) + (((x) >> 8) & 0xFF00) + (((x) << 8) & 0xFF0000) + ((x)<<24) )

#define __byte_swap_word(x) (((((x) >> 8) & 0xFF) + ((x) << 8)) & 0xFFFF)

/*
 * Macros for network/external number representation conversion.
 */
#if BYTE_ORDER == BIG_ENDIAN
#define ntohl(x)    (x)
#define ntohs(x)    (x)
#define htonl(x)    (x)
#define	htons(x)	(x)

#define	NTOHL(x)	
#define	NTOHS(x)	
#define	HTONL(x)	
#define	HTONS(x)	

#elif BYTE_ORDER == LITTLE_ENDIAN

#if defined UPGRADE_LINUX
#include <arpa/inet.h>
#else
#define	ntohl	__byte_swap_long
#define	ntohs	__byte_swap_word
#define	htonl	__byte_swap_long
#define	htons	__byte_swap_word
#endif /* UPGRADE_LINUX */
#define NTOHL(x)        (x) = ntohl((Int32)x)
#define NTOHS(x)        (x) = ntohs((Int16)x)
#define HTONL(x)        (x) = htonl((Int32)x)
#define HTONS(x)        (x) = htons((Int16)x)
#else
#error  BYTE_ORDER must be defined
#endif



#endif

/* END OF FILE */
