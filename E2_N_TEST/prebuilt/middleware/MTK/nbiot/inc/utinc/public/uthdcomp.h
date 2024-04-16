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
 * Header compression structures used in SNDCP and PPP
 **************************************************************************/

#ifndef UTHDCOMP_H
#define UTHDCOMP_H

/**** NESTED INCLUDE FILES *************************************************/
#include <system.h>

#if !defined (UTENDIAN_H)
#  include <utendian.h>
#endif

#if !defined (UTIP_H)
#  include <utip.h>
#endif

/**** CONSTANTS ************************************************************/

#define HDR_COMP_ENTITY_OCTET       0
#define HDR_COMP_ALG_TYPE_OCTET     1
#define HDR_COMP_LENGTH_OCTET       2
#define HDR_COMP_PCOMP_VALUE_OCTET  3

#define HDR_COMP_P_BIT_MASK         0x80
#define HDR_COMP_P_BIT_SHIFT        7
#define HDR_COMP_P_BIT_SET          0x01
#define HDR_COMP_P_BIT_CLEAR        0x00
#define HDR_COMP_ENTITY_NO_MASK     0x1F
#define HDR_COMP_ALG_TYPE_MASK      0x1F
#define HDR_PCOMP_VALUE_ODD_MASK    0xF0
#define HDR_PCOMP_VALUE_ODD_SHIFT   4
#define HDR_PCOMP_VALUE_EVEN_MASK   0x0F

#define RFC1144_NUM_PCOMP_VALUES    2

#define RFC1144_DEFAULT_NUM_STATE_SLOTS   15             /* S0 in RFC1144 */
#define RFC1144_HEADER_SIZE 128

/* RFC1144 (Van Jacobson) header compressor availability to SNDCP/PPP */

/* note: RFC1144_MAX_SLOTS is 1 less than the actual no of slots */

#define RFC1144_MAX_SLOTS               15
#define RFC1144_COMPRESS_SLOT_ID        TRUE

/*
    Byte ordering
    -------------
    ntohl(dat) converts (4 byte) long dat from network (serial line) byte
    order to host byte order. On a big-endian cpu this can be the no-op
    macro but on a little-endian byte order machine, this must transpose
    the bytes.

    ntohs(dat) is like ntohl but converts (2 byte) shorts instead of longs.
    The htonl(dat) and htons(dat) macros do the inverse transform (host to
    network/serial line byte order) for longs and shorts.
*/

#if BYTE_ORDER == BIG_ENDIAN
#define vjntohl(dat) (dat)
#define vjhtonl(dat) (dat)
#define vjntohs(dat) (dat)
#define vjhtons(dat) (dat)
#endif
#if BYTE_ORDER == LITTLE_ENDIAN
#define vjntohl(dat) (((dat) >> 24) + (((dat) & 0xff0000) >> 8) + (((dat) & 0xff00) << 8) + ((dat) << 24))
#define vjhtonl(dat) (((dat) >> 24) + (((dat) & 0xff0000) >> 8) + (((dat) & 0xff00) << 8) + ((dat) << 24))
#define vjntohs(dat) (((Int16)(dat) >> 8) + (Int16)((Int16)(dat) << 8))
#define vjhtons(dat) (((Int16)(dat) >> 8) + (Int16)((Int16)(dat) << 8))

#endif

#define MAX_HDR 128     /* max TCP+IP hdr length (by protocol def) */

/*
Bits in first octet of compressed packet
*/

/* flag bits for what changed in a packet */

#define NEW_C  0x40
#define NEW_I  0x20
#define TCP_PUSH_BIT 0x10

#define NEW_S  0x08
#define NEW_A  0x04
#define NEW_W  0x02
#define NEW_U  0x01

/* reserved, special-case values of above */
#define SPECIAL_I (NEW_S|NEW_W|NEW_U)        /* echoed interactive traffic */
#define SPECIAL_D (NEW_S|NEW_A|NEW_W|NEW_U)  /* unidirectional data */
#define SPECIALS_MASK (NEW_S|NEW_A|NEW_W|NEW_U)


/**** MACROS **************************************************************/

/*
The following macros are used to encode and decode numbers.  They all
assume that 'cp' points to a buffer where the next byte encoded (decoded)
is to be stored (retrieved). Since the decode routines do arithmetic,
they have to convert from and to network byte order.
*/

/*
ENCODE encodes a number that is known to be non-zero.  ENCODEZ checks for
zero (zero has to be encoded in the long, 3 byte form).
*/
#define ENCODE(n) { \
     if ((Int16)(n) >= 256) { \
          *cp++ = 0; \
          cp[1] = (Int8)(n); \
          cp[0] = (Int8)((n) >> 8); \
          cp += 2; \
     } else { \
          *cp++ = (Int8)(n); \
     } \
}
#define ENCODEZ(n) { \
     if ((Int16)(n) >= 256 || (Int16)(n) == 0) { \
          *cp++ = 0; \
          cp[1] = (Int8)(n); \
          cp[0] = (Int8)((n) >> 8); \
          cp += 2; \
     } else { \
          *cp++ = (Int8)(n); \
     } \
}

/*
DECODEL takes the (compressed) change at byte cp and adds it to the
current value of packet field 'f' (which must be a 4-byte (long) integer
in network byte order).  DECODES does the same for a 2-byte (short) field.
DECODEU takes the change at cp and stuffs it into the (short) field f.
'cp' is updated to point to the next field in the compressed header.
*/
#define DECODEL(f) { \
     if (*cp == 0) {\
          (f) = vjhtonl(vjntohl(f) + (((Int32)cp[1] << 8) | (Int32)cp[2])); \
          cp += 3; \
     } else { \
          (f) = vjhtonl(vjntohl(f) + (Int32)(*cp)); \
          cp++; \
     } \
}
#define DECODES(f) { \
     if (*cp == 0) {\
          (f) = vjhtons(vjntohs(f) + ((cp[1] << 8) | cp[2])); \
          cp += 3; \
     } else { \
          (f) = vjhtons(vjntohs(f) + (Int32)(*cp)); \
          cp++; \
     } \
}
#define DECODEU(f) { \
     if (*cp == 0) {\
          (f) = vjhtons((cp[1] << 8) | cp[2]); \
          cp += 3; \
     } else { \
          (f) = \
        /*lint -e(572) stops LINT complaining Excessive shift value
         * (precision 8 shifted right by 8)
         */ \
          vjhtons((Int32)(*cp)); \
          cp++; \
     } \
}

/* fast decode macros for endian-independent processing */

#define FDECODEL(f) { \
     if (*cp == 0) {\
          newTot = ((*(f)) << 24) + ((*(f+1)) << 16) + ((*(f+2)) << 8) + (*(f+3)) + ((cp[1] << 8) + cp[2]); \
          *(f) = (newTot >> 24); \
          *(f+1) = (newTot >> 16); \
          *(f+2) = (newTot >> 8); \
          *(f+3) = newTot; \
          cp += 3; \
     } else { \
          newTot = ((*(f)) << 24) + ((*(f+1)) << 16) + ((*(f+2)) << 8) + (*(f+3)) + (*cp); \
          *(f) = (newTot >> 24); \
          *(f+1) = (newTot >> 16); \
          *(f+2) = (newTot >> 8); \
          *(f+3) = newTot; \
          cp++; \
     } \
}
#define FDECODES(f) { \
     if (*cp == 0) {\
          newTot = ((*(f)) << 8) + (*(f+1)) + ((cp[1] << 8) + cp[2]); \
          *(f) = (newTot >> 8); \
          *(f+1) = newTot; \
          cp += 3; \
     } else { \
          newTot = ((*(f)) << 8) + (*(f+1)) + (*cp); \
          *(f) = (newTot >> 8); \
          *(f+1) = newTot; \
          cp++; \
     } \
}
#define FDECODEU(f) { \
     if (*cp == 0) {\
          *(f) = (cp[1]);  \
          *(f+1) = cp[2]; \
          cp += 3; \
     } else { \
          *(f) = 0;  \
          *(f+1) = (*cp); \
          cp++; \
     } \
}

/**** TYPEDEFS *************************************************************/

/*
lengths of parameter blocks in XID block for header compression algorithms
*/
typedef enum XidHdrCompLengthTypeTag
{
    /* common lengths valid for all algorithms */
    PCOMPLEN_NO_PARAMS = 0,
    PCOMPLEN_NSAPIS = 2,

    /* algorithm-specific lengths */

    /* RFC 1144 */
    PCOMPLEN_RFC1144_NSAPIS_NUMSLOTS = 3,
    PCOMPLEN_RFC1144_INC_PCOMP_VALS  = 4
}
XidHdrCompLengthType;


/** RFC1144 negotiated parameters.
 * contains the details of RFC1144 header compression parameters
 */
typedef struct Rfc1144ParamsTag
{
    /** Indicates how many slots are used for saving packet headers in
     * the transmit direction. This should be set to 16 by the proposer.
     * A minimum value of 4 is recommended.
     */
    Int8 numSlots;
    /** Not used in SNDCP version 0, and is treated as FALSE when received by SNDCP
     * (this is in the PC but not the GPRS network )
     */
    Boolean compressSlotId;
}
Rfc1144Params;


/*
"state" data for each active tcp conversation on the wire.  This is
basically a copy of the entire IP/TCP header from the last packet together
with a small identifier used by the transmit and receive ends of the line to
locate the saved header.
*/
struct cstate
{
    struct cstate *cs_next;  /* next most recently used cstate (xmit only) */
    Int16  cs_hlen;          /* size of hdr (receive only) */
    Int8   cs_id;            /* connection num associated with this state */
    Int8   cs_filler;
    union
    {
        Int8 hdr[MAX_HDR];
        struct iphdr csu_ip;   /* ip/tcp hdr from most recent packet */
    }
    slcs_u;
};

#define cs_ip slcs_u.csu_ip
#define cs_hdr slcs_u.hdr

/*
all the state data for one serial line (we need one of these per line).
*/
typedef struct VjConnStateTag
{
    struct cstate *last_cs;            /* most recently used tstate */
    Int8   last_recv;                  /* last rcvd conn. id */
    Int8   last_xmit;                  /* last sent conn. id */
    Int16  flags;
    struct cstate tstate[RFC1144_MAX_SLOTS + 1];  /* xmit connection states */
    struct cstate rstate[RFC1144_MAX_SLOTS + 1];  /* receive connection states */

    /* negotiated parameters - set up at compressor reset time */
    Rfc1144Params negParams;
}
VjConnState;

/* flag values */
#define SLF_TOSS 1       /* tossing rcvd frames because of input err */

/** Specifies the type of header compression */
typedef enum HeaderCompressionTypeTag
{
    /** RFC 1144 */
    HDR_COMP_ALG_RFC1144 = 0,
    /*
    add new header compression types here
    */
    /** Maximum number of header compression types */
    MAX_NUM_HDR_COMP_TYPES
}
HeaderCompressionType;

/** This is a union of Rfc1144Params and any other header compression
 * algorithm parameters introduced in the future.
 */
typedef union HeaderCompressionParamsTag
{
    /** Parameters of RFC1144 */
    Rfc1144Params rfc1144;
    /* add new header compressors' parameters here */
}
HeaderCompressionParams;

/*
context information used by RFC1144 compression module - an instance of this
structure is given as a pointer to the compression/decompression functions

note that the header compression entity excludes the numSlots parameter used
in the negotiation - this is stored separately in the hcpNeg field of the
SnXidHeaderCe structure as part of the SAP info
*/

typedef union HeaderCompressionEntityTag
{
    void        *context_p;
    VjConnState *rfc1144_p;
    /* add new header compression context structures here */
}
HeaderCompressionEntity;


/* Compression Entity Definition - Header Compression */

/*
each SAP has an array of MAX_NUM_HDR_COMP_ENTITIES of header compression
entities. Each compression entity (CE) has different settings. The
compression type and the parameters of the compression type combine to give
a unique setting. For each setting, the NSAPIs which use this CE are
indicated.

An NSAPI can negotiate the use of more than one header compression entity.
This results in a compression entity block being negotiated for each CE. All
negotiation relates to the SAP that the NSAPI is using.

Within each entity, the requested settings and the negotiated settings are
stored so that checks can be made when processing an XID response block.
*/

typedef struct SnXidHeaderCeTag
{
/** \assoc UNION \ref hcpReq
\assoc UNION \ref hcpNeg
\assoc UNION \ref hceContext */
    HeaderCompressionType   hct;
    HeaderCompressionParams hcpReq;
    HeaderCompressionParams hcpNeg;
    HeaderCompressionEntity hceContext;
    Boolean                 hceAcknowledgedLlcMode;

    /* NSAPI masks */
    Int16                   nsapisReq;      /* sent to network, or received */
    Int16                   nsapisNeg;      /* final values from MS or net */
    Int16                   nsapisPdpReq;   /* PDPs' request */
    Int16                   nsapisSndcpReq; /* SNDCP's request */
    Int16                   nsapisPdpNeg;   /* PDPs' response (negotiated) */
    Int16                   nsapisSndcpNeg; /* SNDCP's response (negotiated */
    Int16                   nsapisReqByNet; /* requested by the network */
    Int8                    headerCeNumber; /* Negotiated Entity number */
}
SnXidHeaderCe;

#endif

/* END OF FILE */
