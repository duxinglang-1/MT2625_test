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
 * definition of NSAPI
 *
 * From gpnpdu.h rev 1.1
 **************************************************************************/

#ifndef NPDU_TYP_H
#define NPDU_TYP_H

/**** NESTED INCLUDE FILES *************************************************/

#if !defined (SYSTEM_H)
#  include <system.h>
#endif


/* CONSTANTS ***************************************************************/

/*
This should be large enough to carry PPP MRUs in DsPpp signals, and V42bis
compressed data before it is being sent to the decompressor in SNDCP.
Currently the requested MRU size is 1600, and data compression may initially
cause the data length to increase, by around 500 bytes for a 1500 byte packet.
Therefore a value of 2000 should be enough.
*/
#define MAX_NPDU_SIZE           2000

#define MAX_RX_BUFFER_OFFSET    163  /* This size matches a calculation
                                      * performed in the GPRS headers.
                                      * A #error checks it in gpsystem.h */

#ifdef MDL_DL_PIPELINE
/* Define how many NPDU could be contains within a same signal*/
#define MAX_NPDU_PER_SIGNAL 52
#endif

/* TYPEDEFS ****************************************************************/

/** This structure contains IP data in an NPDU to be sent to the peer in the 
 * external packet-switched network, or data received from the external network. 
 * NPDU is an abbreviation of <I> network-layer protocol data unit </I>, the 
 * block of data sent between peers at the network layer (also known as the 
 * packet layer; or Layer 3 in OSI terminology). The actual data is of no 
 * significance to SNDCP/RABM. The actual received data might not start at the beginning 
 * of the npduData buffer in a SnDataInd or a SnUnitDataInd (see dataOffset).
 * The length field indicates how many bytes are relevant in the following 
 * npduData field, starting at dataOffset bytes into npduData. The length value 
 * may be less than the physical buffer size allocated.
 */
#if defined (DATA_IN_SIGNAL)

typedef struct NpduTag
{
    /** Number of user data bytes in NPDU*/
    Int16   length;
    /** Offset from npduData that user data starts*/
    Int16   dataOffset;
    /** provided to allow Genie to display the correct number of 
     * items in the npduData */
    Int16   lengthPlusOffset;
    /** An array of bytes containing the Network Layer PDU */
    Int8    npduData[MAX_NPDU_SIZE + MAX_RX_BUFFER_OFFSET];
}
Npdu;

#else

typedef struct NpduTag
{
    /** Number of user data bytes in NPDU*/
    Int16   length;       
    /** The number of bytes of memory in npduData before the actual 
     * received NPDU data appears. This allows in-place TCP/IP header 
     * decompression (or other algorithm decompression) to be done in SNDCP, 
     * if necessary, using the spare bytes
     */
    Int16   dataOffset;  
    /** Total size of buffer, set only when allocated */
    Int16   totalLength;  
    /** Points to an array of bytes containing the Network Layer PDU and, 
     * possibly, preceding spare bytes
     */
    Char   *npduData;    
}
Npdu;

typedef struct NpduDataDebugTag
{
    /** An array of bytes containing the Network Layer PDU */
    Int8    npduData[MAX_NPDU_SIZE + MAX_RX_BUFFER_OFFSET];
} 
NpduDataDebug;
#endif /* defined (DATA_IN_SIGNAL) */

#endif

/* END OF FILE */
