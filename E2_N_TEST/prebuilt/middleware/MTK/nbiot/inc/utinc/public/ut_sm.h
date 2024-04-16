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
 * File Description
 * ----------------
 *
 * Short Messaging utility library: primarily stuff to deal with SMS
 * User Data Headers for the purposes of:
 *
 *     a. concatenated SMS
 *     b. application port based filtering
 *     c. EMS (de)composition
 *     d. etc.
 **************************************************************************/

#ifndef UT_SM_H
#define UT_SM_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/


#if !defined (SITS_TYP_H)
#  include <sits_typ.h>
#endif



/***************************************************************************
 * Manifest Constants
 **************************************************************************/
/*0210-20975 Information Element Id ranges*/
#define SM_IEI_SIMAT_SEC_START 0x70
#define SM_IEI_SIMAT_SEC_END   0x7F
#define SM_IEI_SME_SME_START   0x80
#define SM_IEI_SMS_SMS_END     0x9F
#define SM_IEI_SC_SPEC_START   0xC0
#define SM_IEI_SC_SPEC_END     0xDF

#define MAX_SMS_ADDR_LEN 20  /* Maximum length of SMS address */

/***************************************************************************
 * Type Definitions
 **************************************************************************/


/* header tags as defined by GSM TS 23.040 section 9.2.3.34,
 * note that the unnumbered enumerations actually represent
 * a range of possible tags. the exact value is included
 * in the header data (SmIeRaw.id). */
typedef enum SmUserDataHeaderTypeTag
{
  SM_IEI_CONCAT_8      =0x00, /* Concatenated SMS */
  SM_IEI_SPECIAL_SMS   =0x01, /* Special SMS Message indication */
  SM_IEI_RESERVED_02   =0x02,
  SM_IEI_LF_DUMMY_03   =0x03,
  SM_IEI_APPL_PORT_8   =0x04, /* Application port addressing, 8-bit address */
  SM_IEI_APPL_PORT_16  =0x05, /* Application port addressing, 16-bit address */
  SM_IEI_SMSC_CONTROL  =0x06, /* SMSC control parameters */
  SM_IEI_UDH_SOURCE    =0x07, /* UDH source indicator */
  SM_IEI_CONCAT_16     =0x08, /* Concatenated short message, 16-bit reference*/
  SM_IEI_WIRELESS_CTRL =0x09, /* Wireless Control Message Protocol */
  SM_IEI_TEXT_FORMAT   =0x0a, /* Test formatting */
  SM_IEI_PREDEF_SOUND  =0x0b, /* Predefined sound */
  SM_IEI_USERDEF_SOUND =0x0c, /* User defined sound */
  SM_IEI_PREDEF_ANIM   =0x0d, /* Predefined animation */
  SM_IEI_LARGE_ANIM    =0x0e, /* Large animation */
  SM_IEI_SMALL_ANIM    =0x0f, /* Small animation */
  SM_IEI_LARGE_PIC     =0x10, /* Large picture */
  SM_IEI_SMALL_PIC     =0x11, /* Small picture */
  SM_IEI_VARIABLE_PIC  =0x12, /* Variable picture */
  SM_IEI_USER_PROMPT   =0x13, /* User Prompt Indicator */
  SM_IEI_EXT_OBJ       =0x14, /* Extended object */
  SM_IEI_REUSED_EXT_OBJ=0x15, /* Reused Extended Object */
  SM_IEI_COMP_CTRL     =0x16, /* Compression Control */
  SM_IEI_ODI           =0x17, /* Object Distribution Indicator */
  SM_IEI_WVG_OBJECT           =0x18, /* Standard WVG object */
  SM_IEI_CHAR_SIZE_WVG_OBJECT =0x19, /* Character size WVG object */
  SM_IEI_EXT_OBJ_DATA_REQUEST =0x1A, /* Extended object data request */
  SM_IEI_RESERVED_1B          =0x1B,
  SM_IEI_RESERVED_1C          =0x1C,
  SM_IEI_RESERVED_1D          =0x1D,
  SM_IEI_RESERVED_1E          =0x1E,
  SM_IEI_RESERVED_1F          =0x1F,
  SM_IEI_RFC_822_HDR   =0x20, /* RFC 822 mail header indication */
  SM_IEI_HYPERLINK_FORMAT     =0x21, /* Hyperlink format */
  SM_IEI_REPLY_ADDRESS =0x22, /* Reply address element */
  SM_IEI_ENHANCED_VOICE_MAIL  =0x23, /* Enhanced Voice Mail Information */
  SM_IEI_SIMAT_SECURITY,  /* SIM Toolkit Security header */
  SM_IEI_SME_SME,	  /* SME to SME specific */
  SM_IEI_SC_SPECIFIC,	  /* SC specific header */
  SM_IEI_UNDEFINED,	  /* Undefined or reserved id */
  SM_IEI_NULL             /*Bad header element - error reading element*/
}  SmUserDataHeaderType;


typedef struct  SmIeConcatTag
{
  Int16	refNumber;
  Int8	maxNumber;
  Int8	seqNumber;
}  SmIeConcat;

typedef enum SmIeMsgIndTypeTag
{
  SM_IE_VOICE = 0x00,
  SM_IE_FAX   = 0x01,
  SM_IE_EMAIL = 0x02,
  SM_IE_OTHER = 0x03
} SmIeMsgIndType;

typedef struct  SmIeSpecialTag
{
  Boolean         storeMsg;
  SmIeMsgIndType  msgIndType;
  Int8	          number;
}  SmIeSpecial;

typedef struct  SmIeApplPortTag
{
  Int16	destination;
  Int16	source;
}  SmIeApplPort;

typedef struct  SmIeSmscControlTag
{
  Int8	smscControl;
}  SmIeSmscControl;

typedef enum SmIeSourceTag
{
  SM_IE_SENDER = 0x01,	/* UDH created by sender */
  SM_IE_RECVER = 0x02,	/* UDH created by original receiver */
  SM_IE_SMSC   = 0x03	/* UDH created by SMSC */
}  SmIeSource;

typedef struct  SmIeWirelessTag
{
  Int8	length;
  Int8  *cmp_p;
}  SmIeWireless;

typedef enum SmIeTextAlignTag
{
  SM_IE_TXT_ALIGN_LEFT,
  SM_IE_TXT_ALIGN_RIGHT,
  SM_IE_TXT_ALIGN_CENTRE,
  SM_IE_TXT_ALIGN_DEFAULT
}  SmIeTextAlign;

typedef enum  SmIeTextSizeTag
{
  SM_IE_TXT_SIZE_NORMAL,
  SM_IE_TXT_SIZE_LARGE,
  SM_IE_TXT_SIZE_SMALL,
  SM_IE_TXT_SIZE_UNDEF
}  SmIeTextSize;

typedef struct  SmIeTextFormatTypeTag
{
  SmIeTextAlign	align;
  SmIeTextSize	size;
  Boolean	isBold;
  Boolean	isUnderline;
  Boolean	isItalic;
  Boolean	isStrike;
}  SmIeTextFormatType;

typedef struct  SmIeTextFormatTag
{
  Int8	start;
  Int8 	length;
  SmIeTextFormatType type;
}  SmIeTextFormat;

typedef struct  SmIePredefSoundTag
{
  Int8	start;
  Int8	sound;
}  SmIePredefSound;

typedef struct  SmIeUserSoundTag
{
  Int8	start;
  Int16	length;
  Int8  *sound_p;
}  SmIeUserSound;

typedef struct  SmIePredefAnimTag
{
  Int8	start;
  Int8	animation;
}  SmIePredefAnim;

typedef struct  SmIeUserAnimTag
{
  Int8	start;
  Int16	length;
  Int8  *animation_p;
}  SmIeUserAnim;

typedef struct  SmIePictureTag
{
  Int8	start;
  Int16	width;
  Int16	height;
  Int8  *picture_p;
}  SmIePicture;

/*0206-18075 New structure for User Prompt Indicator*/
typedef struct SmIeUserPromptTag
{
  Int16 length;
} SmIeUserPrompt;

typedef enum SmTypeOfNumberTag
{
   SM_IE_NUM_UNKNOWN                      =   0,
   SM_IE_NUM_INTERNATIONAL,
   SM_IE_NUM_NATIONAL,
   SM_IE_NUM_NETWORK_SPEC,
   SM_IE_NUM_SUBSCRIBER,
   SM_IE_NUM_ALPHANUMERIC,
   SM_IE_NUM_ABBREVIATED,
   SM_IE_NUM_RESERVED
}
SmTypeOfNumber;

typedef enum SmNumberingPlanTag
{
   SM_IE_PLAN_UNKNOWN                     =   0,
   SM_IE_PLAN_ISDN                        =   1,
   SM_IE_PLAN_DATA                        =   3,
   SM_IE_PLAN_TELEX                       =   4,
   SM_IE_PLAN_NATIONAL                    =   8,
   SM_IE_PLAN_PRIVATE                     =   9,
   SM_IE_PLAN_ERMES                       =   10,
   SM_IE_PLAN_RESERVED                    =   15
}
SmNumberingPlan;

typedef struct SmIeReplyAddressTag
{
   Int8                             length;
   SmTypeOfNumber                   typeOfNumber;
   SmNumberingPlan                  numberingPlan;
   Int8                             addressValue[MAX_SMS_ADDR_LEN];
} SmIeReplyAddress;

typedef struct  SmIeRawTag
{
  Int16	length;
  Int8	id;
  Int8  *data_p;
}  SmIeRaw;


typedef union  SmUserDataHeaderElementTag
{
  SmIeConcat      smIeConcat;
  SmIeSpecial     smIeSpecial;
  SmIeApplPort    smIeApplPort;
  SmIeSmscControl smIeSmscControl;
  SmIeSource      smIeSource;
  SmIeWireless    smIeWireless;
  SmIeTextFormat  smIeTextFormat;
  SmIePredefSound smIePredefSound;
  SmIeUserSound   smIeUserSound;
  SmIePredefAnim  smIePredefAnim;
  SmIeUserAnim    smIeUserAnim;
  SmIePicture     smIePicture;
  Int8            smIeRfc822;
  SmIeUserPrompt  smIeUserPrompt;
  SmIeRaw         smIeRaw;
}  SmUserDataHeaderElement;


typedef struct  SmUserDataHeaderTag
{
  SmUserDataHeaderType	    type;
  SmUserDataHeaderElement   element;
}  SmUserDataHeader;

typedef enum SmConcatTypeTag
{
  SM_SINGLE,
  SM_CONCAT_8BIT,
  SM_CONCAT_16BIT,
  SM_CONCAT = SM_CONCAT_16BIT
} SmConcatType;

/* this is used internaly in ut_sm to represent context during
   construction phase */
struct UtsmMsgStructTag;
typedef struct UtsmMsgStructTag UtsmMsgStruct;

/***************************************************************************
 *  Macros
 **************************************************************************/


/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

extern Int8 utsmGetUserDataHeaderNumber( Int8  *shortMsgData_p );

extern void utsmGetUserHeaderElement( Int8  *shortMsgData_p,
				      SmUserDataHeader *hdr_p,
				      Int8 element );

extern Int8 utsmGetUserMessageLen( Int8 *shortMsgData_p, Int8 shortMsgLen );

extern Int8 utsmGetUserMessage( Int8 *buff_p,
				 Int8 *msg_p,
				 Int8 offset,
				 Int8 len );

extern Int8 *utsmGetUserMsgPtr ( Int8 *buff_p );


extern UtsmMsgStruct *utsmCreateUserMsg( Int8 * buff_p,
					 MsgCoding msgCoding,
					 SmConcatType concatType );

extern Boolean utsmAddUserMsgElement( UtsmMsgStruct *msgStruct_p,
				      SmUserDataHeader *hdr_p,
				      Int8 *text_p,
				      Int8 textLen,
				      Boolean forceNonConcat,
				      Int8 *msgSpace_p );

extern Int8 utsmCloseMsg( UtsmMsgStruct *msgStruct_p );

extern Int16 utsmGetHeaderLen( SmUserDataHeader *hdr_p );

extern Int8 utsmGetHeaderLengthLen( void );

extern Int8 utsmGetUserMsgLen( UtsmMsgStruct *msgStruct_p );

extern Int8 utsmGetUserMsgTextLen( Int8 textLen,
                                   MsgCoding msgCoding );

extern Int8 utsmGetUserMsgCharLen (UtsmMsgStruct *msgStruct_p);

extern Int8 utsmConvertOctetsToChars ( Int8 octets, MsgCoding msgCoding);
extern Int8 utsmConvertBytesToChars ( Int8 bytes, MsgCoding msgCoding );
extern Int8 utsmGetMaxCharsAvailable ( UtsmMsgStruct *msgStruct_p);
extern Int16 utsmConvertUCS2ArrayToInt8Array (Int16* ucs2Str_p, Int16 strLen);
extern Int8 utsmReadAddressData (const Int8 octetPosition, SmIeReplyAddress *addr, Int8 *basePtr);
extern Int8 utsmWriteAddressData
(const Boolean use0408Format,const Int8 octetPosition, SmIeReplyAddress *addr, Int8 *basePtr);
#endif /* !UT_SM_H */

/* END OF FILE */
