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
 * File Description:                                                     */
 /** \file
 * Data compression structures used in SNDCP
 **************************************************************************/

#ifndef GPDTCOMP_H
#define GPDTCOMP_H

/**** NESTED INCLUDE FILES *************************************************/

#include <system.h>
#include <gpnsapi.h>

/**** CONSTANTS ************************************************************/

#define DATA_COMP_ENTITY_OCTET       0
#define DATA_COMP_ALG_TYPE_OCTET     1
#define DATA_COMP_LENGTH_OCTET       2
#define DATA_COMP_DCOMP_VALUE_OCTET  3

#define DATA_COMP_P_BIT_MASK         0x80
#define DATA_COMP_P_BIT_SHIFT        7
#define DATA_COMP_P_BIT_SET          0x01
#define DATA_COMP_P_BIT_CLEAR        0x00
#define DATA_COMP_ENTITY_NO_MASK     0x1F
#define DATA_COMP_ALG_TYPE_MASK      0x1F
#define DATA_DCOMP_VALUE_ODD_MASK    0xF0
#define DATA_DCOMP_VALUE_ODD_SHIFT   4
#define DATA_DCOMP_VALUE_EVEN_MASK   0x0F

#define V42BIS_NUM_PCOMP_VALUES      1

/* V.42 Bis default data compression parameters when not specified by peer */

#define V42BIS_DEFAULT_P0  COMP_BOTH_DIR     /* compression direction */
#define V42BIS_DEFAULT_P1  2048              /* num codewords */
#define V42BIS_DEFAULT_P2  20                /* max uncompressed string */

/* V.42 Bis data compression parameters when compression is available */

#define V42BIS_COMP_DIR                 COMP_BOTH_DIR
#define V42BIS_NUM_CODEWORDS            1024
#define V42BIS_MAXSTRING                10

/**** TYPEDEFS *************************************************************/

/*
lengths of parameter blocks in XID block for data compression algorithms
*/

typedef enum XidDataCompLengthTypeTag
{
    DCOMPLEN_NO_PARAMS = 0,
    DCOMPLEN_NSAPIS = 2,
    /* algorithm-specific lengths */

    /* V.42bis */
    DCOMPLEN_V42BIS_NSAPIS_P0 = 3,
    DCOMPLEN_V42BIS_NSAPIS_P0P1 = 5,
    DCOMPLEN_V42BIS_NSAPIS_P0P1P2 = 6,
    DCOMPLEN_V42BIS_INC_DCOMP_VAL = 7
}
XidDataCompLengthType;

/** Specifies the type of data compression */
typedef enum DataCompressionTypeTag
{
    /** V.42bis */
    DATA_COMP_ALG_V42BIS = 0,
    /*
    add new data compression types here
    */
    /** Maximum number of header compression types */
    MAX_NUM_DATA_COMP_TYPES
}
DataCompressionType;

/** Indicates the usage of compression for each direction */
typedef enum V42CompressionDirectionTag
{
    /** compress neither direction */
    COMP_NONE = 0,
    /** compress MS-to-SGSN direction only */
    COMP_MS_TO_SGSN = 1,
    /** compress SGSN to-MS direction only */
    COMP_SGSN_TO_MS = 2,
    /** compress both directions */
    COMP_BOTH_DIR = 3
}
V42CompressionDirection;

/** Contains the details of V.42bis data compression parameters */
typedef struct V42BisParamsTag
{
    /** Indicates which direction to use compression in */
    V42CompressionDirection p0CompDir;
    /** The number of codewords in the compressor dictionary  */
    Int16                   p1NumCodewords;
    /** Maximum number of characters in an uncompressed data string 
     * that is accepted to be encoded
     */
    Int8                    p2MaxUncompressedString;
}
V42BisParams;

/** This is a union of V42BisParams and any other 
 * data compression algorithm parameters introduced 
 * in the future. 
 */
typedef union DataCompressionParamsTag
{
    /** Parameters of V.42bis */
    V42BisParams v42Bis;
    /* add new data compressors' parameters here */
}
DataCompressionParams;


#endif

/* END OF FILE */
