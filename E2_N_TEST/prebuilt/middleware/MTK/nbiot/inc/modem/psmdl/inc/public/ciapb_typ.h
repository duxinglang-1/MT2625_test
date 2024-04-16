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
 * Middleware Type definitions for the CIAPB Interface.
 **************************************************************************/

#ifndef CIAPB_TYP_H
#define CIAPB_TYP_H


/***************************************************************************
 * Nested Include Files
 ***************************************************************************/
#include <system.h>

/***************************************************************************
 *   Manifest Constants
 ***************************************************************************/

/***************************************************************************
 *   Macro Functions
 ***************************************************************************/

/***************************************************************************
 *   Enumeration Types
 ***************************************************************************/

/***************************************************************************
 *   Structure Types
 ***************************************************************************/

/** \addtogroup SigCiapb
 *
 * @{
 */

/** Configuration data of a TLV on the CIAPB interface.
 * The valid member of this union is specified by the \link CiApbTlvTag::type type \endlink and
 * \link CiApbTlvTag::length length \endlink field of the
 * \link #CiApbTlvTag CiApbTlv \endlink struct.
 */
typedef union CiApbTlvDataTag
{
    /** Signed 64bit integer value.
     * This field is only valid if the \link CiApbTlvTag::type type \endlink field of the
     * \link #CiApbTlvTag CiApbTlv \endlink struct
     * is set to #APBRIDGE_TLV_TYPE_INTEGER and the
     * \link CiApbTlvTag::length length \endlink field
     * of the \link #CiApbTlvTag CiApbTlv \endlink struct is set to 8.
     */
    SignedInt64                 integer64bit;

    /** Signed 32bit integer value.
     * This field is only valid if the \link CiApbTlvTag::type type \endlink field of the
     * \link #CiApbTlvTag CiApbTlv \endlink struct
     * is set to #APBRIDGE_TLV_TYPE_INTEGER and the
     * \link CiApbTlvTag::length length \endlink field
     * of the \link #CiApbTlvTag CiApbTlv \endlink struct is set to 4.
     */
    SignedInt32                 integer32bit;

    /** Points to an allocated buffer holding a string.
     * The buffer is dynamically allocated GKI memory.
     * This field is only valid if the \link CiApbTlvTag::type type \endlink field of the
     * \link #CiApbTlvTag CiApbTlv \endlink struct
     * is set to #APBRIDGE_TLV_TYPE_STRING.
     */
#ifdef ENABLE_AP_BRIDGE_UNIT_TEST
     Int8                       string_p[256];
#else
     Int8                      *string_p;
#endif
} CiApbTlvData;



/** TLV structure on the CIAPB interface.
 * This struct is used to describe an AT Command parameter or an AT Response parameter.
 */
typedef struct CiApbTlvTag
{
    /** The Type field indicates the type of AP Bridge TLV.
     * The Type field can have the following values:
     * - 1 - Integer (#APBRIDGE_TLV_TYPE_INTEGER)
     * - 2 - String (#APBRIDGE_TLV_TYPE_STRING)
     * - 3 - Void (parameter not provided on AT interface) (#APBRIDGE_TLV_TYPE_VOID)
     *
     * These values are defined in the \ref ApBridgeInterface.
     */
    Int32                       type;

    /** Number of data bytes.
     * Depending on the TLV #type this is, for example:
     * - 4 - for a 32bit signed Integer type
     * - 8 - for a 64bit signed Integer type
     * - Number of bytes in the data buffer - for a String type
     * - 0 - for a Void type
     */
    Int32                       length;

    /** Configuration data of this TLV. */
    CiApbTlvData                data;
} CiApbTlv;

/** @} */ /* End of SigCiapb group */

#endif /* !CIAPB_TYP_H */

/* End of ciapb_typ.h */

