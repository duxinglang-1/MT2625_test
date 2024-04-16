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
 * definition of signal contents sent between SNDCP task and LLC task
 **************************************************************************/

#ifndef SNPDU_H
#define SNPDU_H

/**** NESTED INCLUDE FILES *************************************************/

#include <gpsapi.h>
#include <gpqos.h>

/**** CONSTANTS ************************************************************/

/* offsets and bit positions within SNPDU (ref. 04.65 7.2) */

/* common offsets for both acked and unacked SNPDUs */
#define SNPDU_OFFS_FLAGS_NSAPI 0
#define SNPDU_OFFS_PCOMP_DCOMP 1

/* bits within flag byte */
#define SNPDU_FLAG_FIRST_SEGMENT        (1 << 6)  /* F */
#define SNPDU_FLAG_IS_UNIT_DATA         (1 << 5)  /* T */
#define SNPDU_FLAG_MORE_SEGMENTS        (1 << 4)  /* M */

/* positions of fields within common bytes */
#define SNPDU_NSAPI_MASK                0x0F
#define SNPDU_DCOMP_MASK                0xF0
#define SNPDU_PCOMP_MASK                0x0F

#define SNPDU_NSAPI_POS                 0
#define SNPDU_DCOMP_POS                 4
#define SNPDU_PCOMP_POS                 0


/* offsets for acked SNPDUs only */
#define SNPDU_OFFS_ACKED_NPDU_NUM           2
#define SNPDU_OFFS_ACKED_DATA_FIRST         3
#define SNPDU_OFFS_ACKED_DATA_SUBSEQ        1
/* subsequent SNPDUS have no PCOMP/DCOMP or NPDU num field */

/* offsets for unacked SNPDUs only */
#define SNPDU_OFFS_UNACKED_SEGMENT_NUM_FIRST      2
#define SNPDU_OFFS_UNACKED_SEGMENT_NUM_SUBSEQ     1
#define SNPDU_OFFS_UNACKED_NPDU_NUM_MSB_FIRST     2
#define SNPDU_OFFS_UNACKED_NPDU_NUM_MSB_SUBSEQ    1
#define SNPDU_OFFS_UNACKED_NPDU_NUM_LSB_FIRST     3
#define SNPDU_OFFS_UNACKED_NPDU_NUM_LSB_SUBSEQ    2
#define SNPDU_OFFS_UNACKED_DATA_FIRST             4
#define SNPDU_OFFS_UNACKED_DATA_SUBSEQ            3
/* subsequent SNPDUS have no PCOMP/DCOMP num field */

/* positions of fields within unacked SNPDUs */
#define SNPDU_SEGMENT_NUM_MASK          0xF0
#define SNPDU_NPDU_NUM_MSB_MASK         0x0F

#define SNPDU_SEGMENT_NUM_POS           4
#define SNPDU_NPDU_NUM_MSB_POS          0


/**** TYPEDEFS *************************************************************/

/*
This field is present in an SnXidBlock. It indicates the XID
parameter being requested or negotiated; any parameters for the type follow
the type value in the XID block.
*/

typedef enum XidParameterTypeTag
{
    SN_XID_PAR_VERSION_NUM = 0,
    SN_XID_PAR_DATA_COMP = 1,
    SN_XID_PAR_HDR_COMP = 2
}
XidParameterType;


/**** XID Block ****

This is an array of bytes (Int8) containing Layer 3 configuration data in the
following format:

<parameter>[<parameter>...<parameter>]

where:

<parameter> = <xid parameter type>[<xid parameter length>]<parameter values>

if <xid parameter type> is 0, then <xid parameter length> is omitted.

if <xid parameter type> = 1 or 2, then <parameter values> = <algorithm type>
   <unacked NSAPI bitmap><acked NSAPI bitmap><algorithm params length>
   <algorithm params>

Parameter block component sizes: component size in bytes

<xid parameter type> : 1

<xid parameter length> : 1

<algorithm type> : 1

<unacked NSAPI bitmap> : 2

<algorithm params length> : 1

<algorithm params> : variable

Note that there is no length field associated with the SNDCP version number,
as this is always a fixed length of 1 octet.

This block is created from internal SNDCP data as context activations are
created and destroyed.

The response to a 'requesting' XID block is a  `negotiated' XID block
containing the requested values or alternative suggested values for header
compressions and data compressions.
*/



#endif

/* END OF FILE */
