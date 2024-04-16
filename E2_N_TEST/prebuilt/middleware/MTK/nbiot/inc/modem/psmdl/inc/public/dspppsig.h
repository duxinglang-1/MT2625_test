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
 *  File Description :                                                   */
/** \file
 *      MUX - PPP Task Signal Definitions
 **************************************************************************/

#ifndef DSPPPSIG_H
#define DSPPPSIG_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <nbiot_modem_common_config.h>
#if defined (FEA_PPP)
#include <npdu_typ.h>
#include <dsppptyp.h>

/** \defgroup SigGpdsppp GPDSPPP Signalling Interface
 * \ingroup PrdMW
 * \ingroup Sig
 * Signaling interface between the PPP task and MUX.
 * This section describes the signals sent between the PPP and MUX.
 *
 * This interface is for passing the PPP related configuration data to MUX and
 * transferring user plane data between the MUX and PPP.
 *
 * The context of GPDSPPP is shown in Figure below:
 *
 * \image html Middleware_Interface.png
 * @{
 */
/***************************************************************************
 * Manifest Constants
 **************************************************************************/
/** The maximum size of address and control data */
#define DS_PPP_MAX_ADDR_AND_CTRL_DATA 2
/** The maximum size of protocol */
#define DS_PPP_MAX_PROTOCOL           2
/** The maximum size of CRC */
#define DS_PPP_MAX_CRC                4

/* The purpose of `domain' is to distinguish among different series of
   numbers used in the associated cid.  For packet-switched data --
   the original use of the DsPppReq/Ind signals -- the cid should be a
   true cid, as used in the various AT commands (though this will only
   be the case once we implement multicontext, as it has been already
   in one customer's branch).  For circuit-switched data, the cid will
   probably contain the stream identifier (likewise, this will only be
   the case once we implement multicall). */
#if defined (ENABLE_SYNC_PPP)
/** CS domain. */
#define DS_PPP_CS_DOMAIN 1
#endif

/** PS domain.
 * This is not an enum, since we want to make sure it is small, to
 * avoid increasing the size of this signal above the size of the
 * signals of the small-signal pool.
 *
 * Since, before the change introducing these values, all signals were implicitly PS,
 * make PS 0, so that zeroised signals end up with the right value
 */
#define DS_PPP_PS_DOMAIN 0
/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Types
 **************************************************************************/

/***************************************************************************
 *  Signal Definitions
 **************************************************************************/
/** Error indication to PPP.
 * This signal is sent by MUX to PPP when a buffer overflow situation occurs in MUX
 * (caused by a missing or corrupted flag character), or when a receive error is
 * detected (e.g. character overrun, parity error or framing error). There is no
 * response to this signal.
 */
typedef struct DsPppErrorIndTag
{
    /** The cause of error */
    DsPppErrorCause cause;
    /** Connection ID */
    Int8            connId;
    /** Specifies if CS or PS domain */
    Int8            domain;
    /* the next are for any interesting numbers, like DevCheck */
    /** Parameter 1 for diagnostic */
    Int32           aux1;
    /** Parameter 2 for diagnostic */
    Int32           aux2;
    /** Parameter 3 for diagnostic */
    Int32           aux3;
}
DsPppErrorInd;

/** \todo DsPppSyncReq is obsolete, There is any references under tplgsm\modem;
 * This can be safely removed. A dropzone test for verification should be fine.
 */
typedef struct DsPppSyncTag
{
    /** Connection ID */
    Int8    connId;
    /** Specifies if CS or PS domain */
    Int8    domain;
}
DsPppSyncReq;

/** This signal is sent by PPP to MUX after PPP MRU negotiation has completed.
 * This signal indicates the size of buffer for MUX to allocate for incoming
 * data from the PC and what data offset to use from the start of that buffer
 * when writing received characters from the serial port. MUX checks that it
 * does not write beyond the end of the allocated buffer when writing received
 * characters.
 *
 * PPP uses an HDLC-like framing structure. MUX knows about the parameters of
 * this frame structure; these are the frame delimiter (flag) character, the
 * escape character and escape mask character. These values enable MUX to detect
 * the start and end of a IP packet, and to re-form characters that have been
 * sent as an escape pair.
 *
 * When the MUX buffer is full (indicated by a closing flag character) MUX sends
 * the data buffer to PPP in a DsPppDataInd.
 *
 * The escapeCharacterMapTx field is a 32-bit character map that defines the
 * characters in a IP packet that must be escaped for transmission. The exception
 * to the discarding is XOFF and XON characters, which MUX recognises as special
 * cases when software flow control is enabled.
 *
 * This signal may be sent at any time, and may redefine any or all of the parameters.
 * There is no response to this signal.
 */
typedef struct DsPppConfigReqTag
{
    /** The size of buffer to allocate */
    Int16   bufferSize;
    /** Data offset from the start of the allocated buffer */
    Int16   dataOffset;
    /** Defines the characters in a IP packet that must be escaped for
     * transmission
     */
    Int32   escapeCharacterMapTx;
    /** Connection ID */
    Int8    connId;
    /** Specifies if CS or PS domain */
    Int8    domain;
}
DsPppConfigReq;

/** This signal is sent from MUX to PPP when MUX has received a complete IP packet.
 * PPP, SNDCP, or PDCP is responsible for deallocating the buffer allocated by MUX.
 * MUX has allocated the NPDU using a call to the GPRS memory manager module.
 * The NPDU contains the entire HDLC frame, including any address, control fields
 * and the CRC but excluding the start and end flags. There is no response to this signal.
 * \warning As it stands in Feb 2006 DsPppDataInd has total
 * length 32, including the signal header. This means that any further increase would
 * make the signals be allocated from another pool, which might affect performance.
 */
typedef struct DsPppDataIndTag
{
  /** Contains the IP packet */
  Npdu       npdu;
   /** Connection ID */
  Int8       connId;
  /** Specifies if CS or PS domain */
  Int8       domain;
}
DsPppDataInd;

/** This signal is sent by PPP to MUX to send a complete IP packet to the peer PPP.
 * all arrays are to be sent to peer in order [0], [1] ...
 * \note Although the address and control, protocol and crc fields are
       here broken out, the MUX can cope just as easily with their
       being included in the NPDU data.  The reason for their being
       separate members of this structure is that the protocol stack (LLC,
       SNDCP and URLC) generates Npdus with no space for the ppp task to
       prepend these fields, and to avoid copying the whole packet the
       ppp task passes on what it received together with the ppp
       header and trailer fields as separate members.
 *
 * There is no response to this signal.
 */
typedef struct DsPppDataReqTag
{
  /** Indicates how many bytes in the addressAndControlData
   * field are to be transmitted after the opening flag

\assoc ARRAY \ref addressAndControlData */
  Int8    addressAndControlLength;
  /** Holds the address and control data */
  Int8    addressAndControlData[DS_PPP_MAX_ADDR_AND_CTRL_DATA];

  /** The length of protocol
\assoc ARRAY \ref protocol */
  Int8    protocolLength;
  /** Tells MUX what value to send (as two bytes) following the opening flag,
   * and any address and control bytes
   */
  Int8    protocol[DS_PPP_MAX_PROTOCOL];

  /** The length of CRC
\assoc ARRAY \ref crc */
  Int8    crcLength;
  /** Tells MUX what value to send following the data in NPDU */
  Int8    crc[DS_PPP_MAX_CRC];
  /** IP packet */
  Npdu    npdu;
  /** Connection ID */
  Int8    connId;
  /** Specifies if CS or PS domain.
   * This is not an enum, since we want to make sure it is small, to
   * avoid increasing the size of this signal above the size of the
   * signals of the small-signal pool
   */
  Int8    domain;
}
DsPppDataReq;

/** @} */ /* End of SigGpdsppp group */
#endif
#endif /* FEA_PPP */
/* END OF FILE */

