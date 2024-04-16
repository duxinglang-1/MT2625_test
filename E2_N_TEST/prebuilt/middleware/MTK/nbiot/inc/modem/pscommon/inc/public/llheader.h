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
 *  File Description :
 *
 *      LLC frame header 
 *      Address and Control field formats.
 **************************************************************************/

#ifndef LLHEADER_H
#define LLHEADER_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

/***************************************************************************
 * Manifest Constants
 * Typed Constants
 * Types
 **************************************************************************/


/****************************************************************************
 * Address Field
 ****************************************************************************/

/* 
 * Protocol Discriminator bit (PD) in LLC frames is always set to 0. 
 */

/* C/R field bit usage: 
 *         Type    Direction  C/R value */
#define CR_COMMAND_TO_MS       1      /* SGSN side to MS side */
#define CR_COMMAND_TO_SGSN     0      /* MS side to SGSN side */
#define CR_RESPONSE_TO_MS      0      /* SGSN side to MS side */
#define CR_RESPONSE_TO_SGSN    1      /* MS side to SGSN side */

#if defined (LLC_AT_SGSN_SIDE)
#define CR_TX_COMMAND          CR_COMMAND_TO_MS
#define CR_RX_COMMAND          CR_COMMAND_TO_SGSN
#define CR_TX_RESPONSE         CR_RESPONSE_TO_MS
#define CR_RX_RESPONSE         CR_RESPONSE_TO_SGSN
#else
#define CR_RX_COMMAND          CR_COMMAND_TO_MS
#define CR_TX_COMMAND          CR_COMMAND_TO_SGSN
#define CR_RX_RESPONSE         CR_RESPONSE_TO_MS
#define CR_TX_RESPONSE         CR_RESPONSE_TO_SGSN
#endif

typedef struct AddressFieldTag 
{
    Int8    bitPD;  /* Protocol Discriminator bit      */
    Int8    bitCR;  /* Command/Response C/R bit        */
    Int8    sapi;   /* Service Access Point Identifier */
} 
AddressField;


/* 
 * Valid SAPIs are defined in GpSapi.h. All other values are reserved. 
 */

#define ADDRESS_FIELD_LENGTH              1


/****************************************************************************
 * Control Field formats
 ****************************************************************************/

/* Supervisory information (piggy-backed in I+S frame) */

/* Encoding of Supervisory functions bits: */
                     /* S1 S2 */
#define S_RR      0  /*  0  0 */
#define S_ACK     1  /*  0  1 */
#define S_RNR     2  /*  1  0 */
#define S_SACK    3  /*  1  1 */

/* Confirmed information transfer (I format, also considered as I+S) */

typedef struct FormatITag 
{                   /* Control Field (I+S format): */
    Int8    bitA;   /* Acknowledgement request bit */
    Int16   sNR;    /* Receive sequence number N(R) */
    Int8    bitSn;  /* Supervisory function bits ("Piggy-backed" S frame) */
    Int8    sackK;  /* SACK bitmap length indicator (K+1 octets), 0..31 */
    Int8   *sack ;  /* SACK bitmap, R(n) */
    /* This structure shares a common initial sequence with FormatS */
    Int16   sNS;    /* Send sequence number N(S) */
} 
FormatI;

#define HEADER_I_LENGTH                  (ADDRESS_FIELD_LENGTH +  3)
#define HEADER_I_LENGTH_MAX              (ADDRESS_FIELD_LENGTH + 36)


/* Supervisory functions (S format) */

typedef struct FormatSTag 
{                   /* Control Field (S format): */
    Int8    bitA;   /* Acknowledgement request bit */
    Int16   sNR;    /* Receive sequence number N(R) */
    Int8    bitSn;  /* Supervisory function bits */
    Int8    sackK;  /* SACK bitmap length is not encoded in S */
    Int8   *sack;   /* SACK bitmap, R(n) */
} 
FormatS;

#define HEADER_S_LENGTH                  (ADDRESS_FIELD_LENGTH + 2)
#define HEADER_S_LENGTH_MAX              (ADDRESS_FIELD_LENGTH + 34)


/* Unconfirmed information transfer (UI format) */

typedef struct FormatUITag 
{                   /* Control Field (UI format): */
    Int16   sNU;    /* Unconfirmed sequence number N(U) */
    Int8    bitE;   /* Encryption mode */
    Int8    bitPM;  /* Protected mode  */
} 
FormatUI;

#define HEADER_UI_LENGTH                 (ADDRESS_FIELD_LENGTH + 2)


/* Control functions (U format) */

/* Encoding of Unnumbered functions bits: */
                     /* M4 M3 M2 M1 */
#define U_NULL    0  /*  0  0  0  0 */
#define U_DM      1  /*  0  0  0  1 */
#define U_DISC    4  /*  0  1  0  0 */
#define U_UA      6  /*  0  1  1  0 */
#define U_SABM    7  /*  0  1  1  1 */
#define U_FRMR    8  /*  1  0  0  0 */
#define U_XID    11  /*  1  0  1  1 */

typedef struct FormatUTag 
{                   /* Control Field (U format): */
    Int8    bitPF;  /* Poll/Final bit (P/F) */
    Int8    bitMn;  /* Unnumbered function bits */
} 
FormatU;

#define HEADER_U_LENGTH                  (ADDRESS_FIELD_LENGTH + 1)


/****************************************************************************
 * Control Field 
 ****************************************************************************/

/* The type of LLC frame */
typedef enum LlcFrameFormatTag
{
    FRAME_I,
    FRAME_S,
    FRAME_UI,
    FRAME_U,
    FRAME_INVALID
}
LlcFrameFormat;

typedef union ControlFieldTag
{   /* Four types of LLC frame formats: */
    FormatI        formatI;      /* I+S */
    FormatS        formatS;
    FormatUI       formatUI;
    FormatU        formatU;
}
ControlField;



/* Frame Errors */

typedef enum FrameErrorTag 
{
    FRME_NO_ERROR,

    /* An Invalid frame is a frame that: 
     * - contains fewer octets than necessary to include the address field, 
     *   control field, information field, and FCS field necessary to constitute
     *   a complete frame according to the contents of the control field */
    FRMI_TOO_SHORT, 
    FRMI_PD_SET_TO_1,   /* - has the PD bit set to 1 */
    FRMI_RESERVED_SAPI, /* - contains a reserved or unsupported SAPI */
    FRMI_FCS_ERROR,     /* - contains an FCS error */
    FRMI_DUMMY_PDU,     /* - contains an FCS error, but is Dummy PDU */
    FRMI_P_SET_TO_0,    /* - command frame with the Poll bit set to 0 */

    /* Frame Reject (FRMR) response on receipt of: */
    FRMR_UNDEFINED,  /* - control field that is undefined or not implemented */
    FRMR_S_U_LENGTH, /* - a S or U frame with incorrect length */
    FRMR_I_LENGTH    /* - an I frame with an I field that exceeds */
}                    /*   the maximum established length */
FrameError;

#define LlHeaderIsErrorFRMI( error)  (error <  FRMR_UNDEFINED && error)
#define LlHeaderIsErrorFRMR( error)  (error >= FRMR_UNDEFINED)


/****************************************************************************
 * LLC frame header
 ****************************************************************************/

/* The elements of LLC frame */
typedef struct LlcHeaderTag
{
/** \assoc UNION \ref control */
    LlcFrameFormat format; /* I+S, S, UI or U */
    AddressField   address;
    ControlField   control;
    Int8           length; /* LLC header length (Address and Control fields) */
    FrameError     error;
}
LlcHeader;



/***************************************************************************
 * Functions
 **************************************************************************/

/* Encoding functions: */
extern void LlHeaderEncodeI  (LlcHeader *header, Int8 *buffer);
extern void LlHeaderEncodeS  (LlcHeader *header, Int8 *buffer);
extern void LlHeaderEncodeUI (LlcHeader *header, Int8 *buffer);
extern void LlHeaderEncodeU  (LlcHeader *header, Int8 *buffer);

/* Decoding functions: */
extern Int8 LlHeaderDecodeSapi (Int8 *frame);
extern void LlHeaderDecode (LlcHeader *header, Int8 *buffer, Int16 frameLength);

#endif

/* END OF FILE */
