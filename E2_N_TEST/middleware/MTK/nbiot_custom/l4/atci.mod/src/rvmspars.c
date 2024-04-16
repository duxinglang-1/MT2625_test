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
 * Contains parsing routines for SMS, primarily for detecting message
 * waiting content and checking validity of user data header content.
 **************************************************************************/

#define MODULE_NAME "RVMSPARS"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <rvmsdata.h>
#include <rvdata.h>
#include <rvmspars.h>
#include <rvmsut.h>
#include <rvomtime.h>
#include <rvchman.h>
#include <rvcrhand.h>
#include <rvmssigo.h>
#include <rvcimxut.h>

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

typedef enum VgIEDecodeStatusTag
{
  IE_ILLEGAL_CODING,
  IE_SKIP,
  IE_OK
} VgIEDecodeStatus;

typedef VgIEDecodeStatus (*VgSMSUDHDecode)(Int8 blockLen, Int8 *blockData_p,
                                           Int8 smDataLen, Int8 *smData_p,
                                           Int8 udDataLen, Int8 *udData_p);
typedef struct VgSMSUDHDecodeLookupTag
{
  Boolean         isRange;
  Int8            startRange;
  Int8            endRange;
  VgSMSUDHDecode  handlerFnc;
  Boolean         isRepeatable;
} VgSMSUDHDecodeLookup;

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static Boolean decodeDeliverFO       (const Int8 *elementData_p,
                                      Int8       *elementLen_p);
static Boolean decodeSubmitFO        (const Int8 *elementData_p,
                                      Int8       *elementLen_p);
static Boolean decodeDeliverReportFO (const Int8 *elementData_p,
                                      Int8       *elementLen_p);
static Boolean decodeSubmitReportFO  (const Int8 *elementData_p,
                                      Int8       *elementLen_p);
static Boolean decodeStatusReportFO  (const Int8 *elementData_p,
                                      Int8 *elementLen_p);
static Boolean decodeSCA             (const Int8 *elementData_p,
                                      Int8       *elementLen_p);
static Boolean decodeOADA            (const Int8 *elementData_p,
                                      Int8       *elementLen_p);
static Boolean decodePID             (const Int8 *elementData_p,
                                      Int8       *elementLen_p);
static Boolean decodeDCS             (const Int8 *elementData_p,
                                      Int8       *elementLen_p);
static Boolean decodeSCTS            (const Int8 *elementData_p,
                                      Int8       *elementLen_p);
static Boolean decodeUD              (const Int8 *elementData_p,
                                      Int8       *elementLen_p);
static Boolean decodeMR              (const Int8 *elementData_p,
                                      Int8       *elementLen_p);
static Boolean decodeVP              (const Int8 *elementData_p,
                                      Int8       *elementLen_p);
static Boolean decodeDT              (const Int8 *elementData_p,
                                      Int8       *elementLen_p);
static Boolean decodeST              (const Int8 *elementData_p,
                                      Int8       *elementLen_p);
static Boolean decodePI              (const Int8 *elementData_p,
                                      Int8       *elementLen_p);
static Boolean processDCS(const Int8 dcs, const Int8 recordNumber);
static Boolean processUserDataHeader(Int8 *udh_p, const Int8 recordNumber);
static Boolean processOA(Int8 *origAddress_p, const Int8 recordNumber);

static VgIEDecodeStatus decodeConcatenatedSMS8BitRef( Int8 blockLen, Int8 *blockData_p,
                                                      Int8 smDataLen, Int8 *smData_p,
                                                      Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeSpecialSMSInd(Int8 blockLen, Int8 *blockData_p,
                                            Int8 smDataLen, Int8 *smData_p,
                                            Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeAppPort8Bit( Int8 blockLen, Int8 *blockData_p,
                                           Int8 smDataLen, Int8 *smData_p,
                                           Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeAppPort16Bit(Int8 blockLen, Int8 *blockData_p,
                                           Int8 smDataLen, Int8 *smData_p,
                                           Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeSMSCControlParam( Int8 blockLen, Int8 *blockData_p,
                                                Int8 smDataLen, Int8 *smData_p,
                                                Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeUDHSourceInd(Int8 blockLen, Int8 *blockData_p,
                                           Int8 smDataLen, Int8 *smData_p,
                                           Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeConcatenatedSMS16BitRef(Int8 blockLen, Int8 *blockData_p,
                                                      Int8 smDataLen, Int8 *smData_p,
                                                      Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeWirelessCtrlMsgProtocol(Int8 blockLen, Int8 *blockData_p,
                                                      Int8 smDataLen, Int8 *smData_p,
                                                      Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeSTKSecurityHeaders( Int8 blockLen, Int8 *blockData_p,
                                                  Int8 smDataLen, Int8 *smData_p,
                                                  Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeSMESpecific(Int8 blockLen, Int8 *blockData_p,
                                          Int8 smDataLen, Int8 *smData_p,
                                          Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeSCSpecific(Int8 blockLen, Int8 *blockData_p,
                                         Int8 smDataLen, Int8 *smData_p,
                                         Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeTextFormatting( Int8 blockLen, Int8 *blockData_p,
                                              Int8 smDataLen, Int8 *smData_p,
                                              Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodePredefinedSound(Int8 blockLen, Int8 *blockData_p,
                                              Int8 smDataLen, Int8 *smData_p,
                                              Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeUserDefinedSound( Int8 blockLen, Int8 *blockData_p,
                                                Int8 smDataLen, Int8 *smData_p,
                                                Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodePredefinedAnimation(Int8 blockLen, Int8 *blockData_p,
                                                  Int8 smDataLen, Int8 *smData_p,
                                                  Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeLargeAnimation( Int8 blockLen, Int8 *blockData_p,
                                              Int8 smDataLen, Int8 *smData_p,
                                              Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeSmallAnimation( Int8 blockLen, Int8 *blockData_p,
                                              Int8 smDataLen, Int8 *smData_p,
                                              Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeLargePicture(Int8 blockLen, Int8 *blockData_p,
                                           Int8 smDataLen, Int8 *smData_p,
                                           Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeSmallPicture(Int8 blockLen, Int8 *blockData_p,
                                           Int8 smDataLen, Int8 *smData_p,
                                           Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeVariablePicture(Int8 blockLen, Int8 *blockData_p,
                                              Int8 smDataLen, Int8 *smData_p,
                                              Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeUserPromptIndicator(Int8 blockLen, Int8 *blockData_p,
                                                  Int8 smDataLen, Int8 *smData_p,
                                                  Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeExtendedObjectDataRequest(Int8 blockLen, Int8 *blockData_p,
                                                        Int8 smDataLen, Int8 *smData_p,
                                                        Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeReusedExtendedObject( Int8 blockLen, Int8 *blockData_p,
                                                    Int8 smDataLen, Int8 *smData_p,
                                                    Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeCompressionControl( Int8 blockLen, Int8 *blockData_p,
                                                  Int8 smDataLen, Int8 *smData_p,
                                                  Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeObjectDistributionIndicator(Int8 blockLen, Int8 *blockData_p,
                                                          Int8 smDataLen, Int8 *smData_p,
                                                          Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeReplyAddressElement(Int8 blockLen, Int8 *blockData_p,
                                                  Int8 smDataLen, Int8 *smData_p,
                                                  Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeExtendedObjectDataRequest(Int8 blockLen, Int8 *blockData_p,
                                                        Int8 smDataLen, Int8 *smData_p,
                                                        Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeHyperlinkFormatElement( Int8 blockLen, Int8 *blockData_p,
                                                      Int8 smDataLen, Int8 *smData_p,
                                                      Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeRFC822EMailHeader(Int8 blockLen, Int8 *blockData_p,
                                                Int8 smDataLen, Int8 *smData_p,
                                                Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeStandardWVGObject(Int8 blockLen, Int8 *blockData_p,
                                                Int8 smDataLen, Int8 *smData_p,
                                                Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeCharacterSizeWVGObject(Int8 blockLen, Int8 *blockData_p,
                                                     Int8 smDataLen, Int8 *smData_p,
                                                     Int8 udDataLen, Int8 *udData_p);
static VgIEDecodeStatus decodeExtendedObject(Int8 blockLen, Int8 *blockData_p,
                                             Int8 smDataLen, Int8 *smData_p,
                                             Int8 udDataLen, Int8 *udData_p);

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

VgSMSTPDUDecode vgDeliverTPDUDecode[] =
{
  { VG_SMS_TP_SCA,  decodeSCA             },
  { VG_SMS_TP_FO,   decodeDeliverFO       },
  { VG_SMS_TP_OA,   decodeOADA            },
  { VG_SMS_TP_PID,  decodePID             },
  { VG_SMS_TP_DCS,  decodeDCS             },
  { VG_SMS_TP_SCTS, decodeSCTS            },
  { VG_SMS_TP_UD,   decodeUD              },
  { VG_SMS_TP_NULL, PNULL                 }
};

VgSMSTPDUDecode vgSubmitTPDUDecode[] =
{
  { VG_SMS_TP_SCA,  decodeSCA             },
  { VG_SMS_TP_FO,   decodeSubmitFO        },
  { VG_SMS_TP_MR,   decodeMR              },
  { VG_SMS_TP_DA,   decodeOADA            },
  { VG_SMS_TP_PID,  decodePID             },
  { VG_SMS_TP_DCS,  decodeDCS             },
  { VG_SMS_TP_VP,   decodeVP              },
  { VG_SMS_TP_UD,   decodeUD              },
  { VG_SMS_TP_NULL, PNULL                 }
};

VgSMSTPDUDecode vgDeliverReportTPDUDecode[] =
{
  { VG_SMS_TP_SCA,  decodeSCA             },
  { VG_SMS_TP_FO,   decodeDeliverReportFO },
  { VG_SMS_TP_NULL, PNULL                 }
};

VgSMSTPDUDecode vgSubmitReportTPDUDecode[] =
{
  { VG_SMS_TP_SCA,  decodeSCA             },
  { VG_SMS_TP_FO,   decodeSubmitReportFO  },
  { VG_SMS_TP_NULL, PNULL                 }
};

VgSMSTPDUDecode vgStatusReportTPDUDecode[] =
{
  { VG_SMS_TP_SCA,  decodeSCA             },
  { VG_SMS_TP_FO,   decodeStatusReportFO  },
  { VG_SMS_TP_MR,   decodeMR              },
  { VG_SMS_TP_RA,   decodeOADA            },
  { VG_SMS_TP_SCTS, decodeSCTS            },
  { VG_SMS_TP_DT,   decodeDT              },
  { VG_SMS_TP_ST,   decodeST              },
  { VG_SMS_TP_PI,   decodePI              },
  { VG_SMS_TP_PID,  decodePID             },
  { VG_SMS_TP_DCS,  decodeDCS             },
  { VG_SMS_TP_UD,   decodeUD              },
  { VG_SMS_TP_NULL, PNULL                 }
};

VgSMSTPDUDecode *vgTPDUDecode[] =
{
  vgSubmitTPDUDecode,
  vgDeliverTPDUDecode,
  vgSubmitReportTPDUDecode,
  vgDeliverReportTPDUDecode,
  vgStatusReportTPDUDecode
};

#define FO_MTI              (0x03)
#define FO_UDHI             (0x40)
#define FO_VPF              (0x18)

#define SMS_DELIVER         (0x00)
#define SMS_SUBMIT          (0x01)
#define SMS_REPORT          (0x01)
#define SMS_STATUS_REPORT   (0x02)

#define TP_FO_LEN           (0x01)
#define TP_PID_LEN          (0x01)
#define TP_DCS_LEN          (0x01)
#define TP_SCTS_LEN         (0x07)
#define TP_DT_LEN           (0x07)
#define TP_ST_LEN           (0x01)
#define TP_PI_LEN           (0x01)
#define TP_MR_LEN           (0x01)
#define TP_VP_NONE_LEN      (0x00)
#define TP_VP_REL_LEN       (0x01)
#define TP_VP_ABS_LEN       (0x07)
#define TP_VP_ENH_LEN       (0x07)

#define DCS_MSG_WAIT_STORE    (0xD0)
#define DCS_MSG_WAIT_DISCARD  (0xC0)
#define OA_MSG_WAITING_LEN    (0x04)
#define OA_MSG_WAITING_TYPE   (0xD0)

/* UDH Parameter constants.... */
#define VMAIL_WAITING           (0x00)
#define FAX_WAITING             (0x01)
#define EMAIL_WAITING           (0x02)
#define OTHER_WAITING           (0x03)
#define IE_PORT8_START          (0xF0)
#define IE_PORT16_END           (0x4267)
#define UDH_ORIGINAL_SENDER     (0x01)
#define UDH_ORIGINAL_RECEIVER   (0x02)
#define UDH_SMSC                (0x03)
#define IE_MAX_PREDEF_SOUND_ID  (0x09)
#define IE_MAX_PREDEF_ANIM_ID   (0x0E)
#define IE_MAX_USER_SOUND_LEN   (0x80)
#define IE_LARGE_ANIM_SIZE      (0x80)
#define IE_SMALL_ANIM_SIZE      (0x20)
#define IE_LARGE_PICTURE_SIZE   (0x80)
#define IE_SMALL_PICTURE_SIZE   (0x20)
#define IE_MAX_SM_ADDR_LEN      (0x0C)

#define IE_SMS_8BIT_IED_LEN           (0x03)
#define IE_SMS_SPEC_IND_LEN           (0x02)
#define IE_SMS_APP_PORT8_LEN          (0x02)
#define IE_SMS_APP_PORT16_LEN         (0x04)
#define IE_SMS_SMSC_CTRL_LEN          (0x01)
#define IE_SMS_UDH_SRC_IND_LEN        (0x01)
#define IE_SMS_16BIT_IED_LEN          (0x04)
#define IE_SMS_MIN_WCMP_MSG_LEN       (0x00)
#define IE_SMS_STK_SECURITY_LEN       (0x00)
#define IE_SMS_TEXT_FMT_MIN_LEN       (0x03)
#define IE_SMS_TEXT_FMT_MAX_LEN       (0x04)
#define IE_SMS_PREDEF_SOUND_LEN       (0x02)
#define IE_SMS_USERDEF_SOUND_LEN_MAX  (IE_MAX_USER_SOUND_LEN + 0x01)
#define IE_SMS_PREDEF_ANIM_LEN        (0x02)
#define IE_SMS_LARGE_ANIM_LEN         (IE_LARGE_ANIM_SIZE + 0x01)
#define IE_SMS_SMALL_ANIM_LEN         (IE_SMALL_ANIM_SIZE + 0x01)
#define IE_SMS_LARGE_PICTURE_LEN      (IE_LARGE_PICTURE_SIZE + 0x01)
#define IE_SMS_SMALL_PICTURE_LEN      (IE_SMALL_PICTURE_SIZE + 0x01)
#define IE_SMS_VARIABLE_PICTURE_LEN   (IE_LARGE_PICTURE_SIZE + 0x03)
#define IE_SMS_USER_PROMPT_IND_LEN    (0x01)
#define IE_SMS_EXT_OBJ_MIN_LEN        (0x00)
#define IE_SMS_REUSED_EXT_OBJ_LEN     (0x03)
#define IE_SMS_COMP_CONTROL_MIN_LEN   (0x00)
#define IE_SMS_OBJECT_DIST_IND_LEN    (0x02)
#define IE_SMS_REPLY_ADDR_LEN_MAX     (IE_MAX_SM_ADDR_LEN)
#define IE_SMS_EXT_OBJ_DATA_REQ_LEN   (0x00)
#define IE_SMS_HYPERLINK_FMT_LEN      (0x04)
#define IE_SMS_EMAIL_HDR_IND_LEN      (0x01)

#define MAX_SMS_IE_ELEMENTS           (0x2F)   /* 140/MIN_IE_LEN */

static const VgSMSUDHDecodeLookup vgSMSUDHDecodeLookup[] =
{
  { FALSE, 0x00, 0x00, decodeConcatenatedSMS8BitRef,      FALSE   },
  { FALSE, 0x01, 0x00, decodeSpecialSMSInd,               TRUE    },
  { FALSE, 0x04, 0x00, decodeAppPort8Bit,                 FALSE   },
  { FALSE, 0x05, 0x00, decodeAppPort16Bit,                FALSE   },
  { FALSE, 0x06, 0x00, decodeSMSCControlParam,            FALSE   },
  { FALSE, 0x07, 0x00, decodeUDHSourceInd,                TRUE    },
  { FALSE, 0x08, 0x00, decodeConcatenatedSMS16BitRef,     FALSE   },
  { FALSE, 0x09, 0x00, decodeWirelessCtrlMsgProtocol,     TRUE    },
  { FALSE, 0x0A, 0x00, decodeTextFormatting,              TRUE    },
  { FALSE, 0x0B, 0x00, decodePredefinedSound,             TRUE    },
  { FALSE, 0x0C, 0x00, decodeUserDefinedSound,            TRUE    },
  { FALSE, 0x0D, 0x00, decodePredefinedAnimation,         TRUE    },
  { FALSE, 0x0E, 0x00, decodeLargeAnimation,              TRUE    },
  { FALSE, 0x0F, 0x00, decodeSmallAnimation,              TRUE    },
  { FALSE, 0x10, 0x00, decodeLargePicture,                TRUE    },
  { FALSE, 0x11, 0x00, decodeSmallPicture,                TRUE    },
  { FALSE, 0x12, 0x00, decodeVariablePicture,             TRUE    },
  { FALSE, 0x13, 0x00, decodeUserPromptIndicator,         TRUE    },
  { FALSE, 0x14, 0x00, decodeExtendedObject,              TRUE    },
  { FALSE, 0x15, 0x00, decodeReusedExtendedObject,        TRUE    },
  { FALSE, 0x16, 0x00, decodeCompressionControl,          FALSE   },
  { FALSE, 0x17, 0x00, decodeObjectDistributionIndicator, TRUE    },
  { FALSE, 0x18, 0x00, decodeStandardWVGObject,           TRUE    },
  { FALSE, 0x19, 0x00, decodeCharacterSizeWVGObject,      TRUE    },
  { FALSE, 0x1A, 0x00, decodeExtendedObjectDataRequest,   FALSE   },
  { FALSE, 0x20, 0x00, decodeRFC822EMailHeader,           FALSE   },
  { FALSE, 0x21, 0x00, decodeHyperlinkFormatElement,      TRUE    },
  { FALSE, 0x22, 0x00, decodeReplyAddressElement,         FALSE   },
  { TRUE,  0x70, 0x7F, decodeSTKSecurityHeaders,          TRUE    },
  { TRUE,  0x80, 0x9F, decodeSMESpecific,                 TRUE    },
  { TRUE,  0xC0, 0xDF, decodeSCSpecific,                  TRUE    }
};

#define SMS_UDH_DECODE_ENTRIES  (sizeof(vgSMSUDHDecodeLookup)/sizeof(VgSMSUDHDecodeLookup))

/***************************************************************************
 * Global Function Prototypes
 ***************************************************************************/

/***************************************************************************
 * Variables
 ***************************************************************************/
static Boolean    udhPresent = FALSE;
static VgVpFormat vpFmt = VPF_NOT_PRESENT;
static Char       commandData[VG_CRSM_MAX_COMMAND_STRING];
static Int8       retryId = 0;

/***************************************************************************
 * Macros
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:     decodeDeliverFO
 *
 * Parameters:   elementData_p    - Pointer to PDU data for parsing
 *               elementLen_p     - Pointer to return length of processed
 *                                  PDU element.
 *
 * Returns:      Boolean          - TRUE if processing succesful, FALSE
 *                                  otherwise.
 *
 * Description:  Decode first octet of PDU.
 *-------------------------------------------------------------------------*/
static Boolean decodeDeliverFO(const Int8 *elementData_p,
                               Int8       *elementLen_p)
{
  const Int8  fo = elementData_p[0];
  Boolean     result = FALSE;

  udhPresent = (Boolean)((fo & FO_UDHI) ? TRUE : FALSE);

  if ((fo & FO_MTI) == SMS_DELIVER)
  {
    result = TRUE;
    *elementLen_p = TP_FO_LEN;
  }

  else
  {
    /* Invalid PDU encoding */
    WarnParam(fo & FO_MTI, 0, 0);
    result = FALSE;
  }

  return result;
}

/*--------------------------------------------------------------------------
 *
 * Function:     decodeSubmitFO
 *
 * Parameters:   elementData_p    - Pointer to PDU data for parsing
 *               elementLen_p     - Pointer to return length of processed
 *                                  PDU element.
 *
 * Returns:      Boolean          - TRUE if processing succesful, FALSE
 *                                  otherwise.
 *
 * Description:  Decode first octet of PDU.
 *-------------------------------------------------------------------------*/
static Boolean decodeSubmitFO(const Int8 *elementData_p,
                              Int8 *elementLen_p)
{
  const Int8  fo = elementData_p[0];
  Boolean     result = FALSE;

  vpFmt = (VgVpFormat)((fo & FO_VPF) >> 3);
  udhPresent = (Boolean)((fo & FO_UDHI) ? TRUE : FALSE);

  if ((fo & FO_MTI) == SMS_SUBMIT)
  {
    result = TRUE;
    *elementLen_p = TP_FO_LEN;
  }

  else
  {
    /* Invalid PDU encoding */
    WarnParam(fo & FO_MTI, 0, 0);
    result = FALSE;
  }

  return result;
}

/*--------------------------------------------------------------------------
 *
 * Function:     decodeDeliverReportFO
 *
 * Parameters:   elementData_p    - Pointer to PDU data for parsing
 *               elementLen_p     - Pointer to return length of processed
 *                                  PDU element.
 *
 * Returns:      Boolean          - TRUE if processing succesful, FALSE
 *                                  otherwise.
 *
 * Description:  Decode first octet of PDU.
 *-------------------------------------------------------------------------*/
static Boolean decodeDeliverReportFO(const Int8 *elementData_p,
                                     Int8 *elementLen_p)
{
  const Int8  fo = elementData_p[0];
  Boolean     result = FALSE;

  udhPresent = (Boolean)((fo & FO_UDHI) ? TRUE : FALSE);

  if ((fo & FO_MTI) == SMS_DELIVER)
  {
    result = TRUE;
    *elementLen_p = TP_FO_LEN;
  }

  else
  {
    /* Invalid PDU encoding */
    WarnParam(fo & FO_MTI, 0, 0);
    result = FALSE;
  }

  return result;
}

/*--------------------------------------------------------------------------
 *
 * Function:     decodeSubmitReportFO
 *
 * Parameters:   elementData_p    - Pointer to PDU data for parsing
 *               elementLen_p     - Pointer to return length of processed
 *                                  PDU element.
 *
 * Returns:      Boolean          - TRUE if processing succesful, FALSE
 *                                  otherwise.
 *
 * Description:  Decode first octet of PDU.
 *-------------------------------------------------------------------------*/
static Boolean decodeSubmitReportFO(const Int8 *elementData_p,
                                    Int8 *elementLen_p)
{
  const Int8  fo = elementData_p[0];
  Boolean     result = FALSE;

  udhPresent = (Boolean)((fo & FO_UDHI) ? TRUE : FALSE);

  if ((fo & FO_MTI) == SMS_REPORT)
  {
    result = TRUE;
    *elementLen_p = TP_FO_LEN;
  }

  else
  {
    /* Invalid PDU encoding */
    WarnParam(fo & FO_MTI, 0, 0);
    result = FALSE;
  }

  return result;
}

/*--------------------------------------------------------------------------
 *
 * Function:     decodeStatusReportFO
 *
 * Parameters:   elementData_p    - Pointer to PDU data for parsing
 *               elementLen_p     - Pointer to return length of processed
 *                                  PDU element.
 *
 * Returns:      Boolean          - TRUE if processing succesful, FALSE
 *                                  otherwise.
 *
 * Description:  Decode first octet of PDU.
 *-------------------------------------------------------------------------*/
static Boolean decodeStatusReportFO(const Int8 *elementData_p,
                                    Int8 *elementLen_p)
{
  const Int8  fo = elementData_p[0];
  Boolean     result = FALSE;

  udhPresent = (Boolean)((fo & FO_UDHI) ? TRUE : FALSE);

  if ((fo & FO_MTI) == SMS_STATUS_REPORT)
  {
    result = TRUE;
    *elementLen_p = TP_FO_LEN;
  }

  else
  {
    /* Invalid PDU encoding */
    WarnParam(fo & FO_MTI, 0, 0);
    result = FALSE;
  }

  return result;
}

/*--------------------------------------------------------------------------
 *
 * Function:     decodeSCA
 *
 * Parameters:   elementData_p    - Pointer to PDU data for parsing
 *               elementLen_p     - Pointer to return length of processed
 *                                  PDU element.
 *
 * Returns:      Boolean          - TRUE if processing succesful, FALSE
 *                                  otherwise.
 *
 * Description:  Decode Service Centre Address part of PDU.
 *-------------------------------------------------------------------------*/
static Boolean decodeSCA(const Int8 *elementData_p,
                         Int8       *elementLen_p)
{
  /* Length of SCA in octets is stored as first octet of PDU.
   * Add one to account for length field....
   */
  *elementLen_p = (elementData_p[0] +  1);

  return (TRUE);
}

/*--------------------------------------------------------------------------
 *
 * Function:     decodeOADA
 *
 * Parameters:   elementData_p    - Pointer to PDU data for parsing
 *               elementLen_p     - Pointer to return length of processed
 *                                  PDU element.
 *
 * Returns:      Boolean          - TRUE if processing succesful, FALSE
 *                                  otherwise.
 *
 * Description:  Decode Originator/Destination Address part of PDU.
 *-------------------------------------------------------------------------*/
static Boolean decodeOADA(const Int8 *elementData_p,
                          Int8       *elementLen_p)
{
  /* Length of OA/DA in semi octets stored as first octet.  Need
   * to caluclate number of whole octets and add on length and
   * type parameters
   */
  *elementLen_p = elementData_p[0];

  if (*elementLen_p > 0)
  {
    /* Account for odd len (padded with 0xF).... */
    if ((*elementLen_p % 2) != 0)
    {
      *elementLen_p += 1;
    }

    *elementLen_p /= 2;  /* Convert semi-octets to octets */
    *elementLen_p += 2;  /* Add on length and type of number */
  }
  else
  {
    /* Make sure we return offset for length.... */
    *elementLen_p = 1;
  }

  return (TRUE);
}

/*--------------------------------------------------------------------------
 *
 * Function:     decodePID
 *
 * Parameters:   elementData_p    - Pointer to PDU data for parsing
 *               elementLen_p     - Pointer to return length of processed
 *                                  PDU element.
 *
 * Returns:      Boolean          - TRUE if processing succesful, FALSE
 *                                  otherwise.
 *
 * Description:  Decode Protocol Identifier part of PDU.
 *-------------------------------------------------------------------------*/
static Boolean decodePID(const Int8 *elementData_p,
                         Int8       *elementLen_p)
{
  *elementLen_p = TP_PID_LEN;

  PARAMETER_NOT_USED (elementData_p);

  return (TRUE);
}

/*--------------------------------------------------------------------------
 *
 * Function:     decodeDCS
 *
 * Parameters:   elementData_p    - Pointer to PDU data for parsing
 *               elementLen_p     - Pointer to return length of processed
 *                                  PDU element.
 *
 * Returns:      Boolean          - TRUE if processing succesful, FALSE
 *                                  otherwise.
 *
 * Description:  Decode Data Coding Scheme part of PDU.
 *-------------------------------------------------------------------------*/
static Boolean decodeDCS(const Int8 *elementData_p,
                         Int8       *elementLen_p)
{
  *elementLen_p = TP_DCS_LEN;

  PARAMETER_NOT_USED (elementData_p);

  return (TRUE);
}

/*--------------------------------------------------------------------------
 *
 * Function:     decodeSCTS
 *
 * Parameters:   elementData_p    - Pointer to PDU data for parsing
 *               elementLen_p     - Pointer to return length of processed
 *                                  PDU element.
 *
 * Returns:      Boolean          - TRUE if processing succesful, FALSE
 *                                  otherwise.
 *
 * Description:  Decode Service Centre Time Stamp part of PDU.
 *-------------------------------------------------------------------------*/
static Boolean decodeSCTS(const Int8 *elementData_p,
                          Int8       *elementLen_p)
{
  *elementLen_p = TP_SCTS_LEN;

  PARAMETER_NOT_USED (elementData_p);

  return (TRUE);
}

/*--------------------------------------------------------------------------
 *
 * Function:     decodeUD
 *
 * Parameters:   elementData_p    - Pointer to PDU data for parsing
 *               elementLen_p     - Pointer to return length of processed
 *                                  PDU element.
 *
 * Returns:      Boolean          - TRUE if processing succesful, FALSE
 *                                  otherwise.
 *
 * Description:  Decode User Data part of PDU.
 *-------------------------------------------------------------------------*/
static Boolean decodeUD(const Int8 *elementData_p,
                        Int8       *elementLen_p)
{
  *elementLen_p = elementData_p[0];
  return (TRUE);
}

/*--------------------------------------------------------------------------
 *
 * Function:     decodeMR
 *
 * Parameters:   elementData_p    - Pointer to PDU data for parsing
 *               elementLen_p     - Pointer to return length of processed
 *                                  PDU element.
 *
 * Returns:      Boolean          - TRUE if processing succesful, FALSE
 *                                  otherwise.
 *
 * Description:  Decode Message Reference part of PDU.
 *-------------------------------------------------------------------------*/
static Boolean decodeMR(const Int8 *elementData_p,
                        Int8       *elementLen_p)
{
  PARAMETER_NOT_USED (elementData_p);

  *elementLen_p = TP_MR_LEN;
  return (TRUE);
}

/*--------------------------------------------------------------------------
 *
 * Function:     decodeVP
 *
 * Parameters:   elementData_p    - Pointer to PDU data for parsing
 *               elementLen_p     - Pointer to return length of processed
 *                                  PDU element.
 *
 * Returns:      Boolean          - TRUE if processing succesful, FALSE
 *                                  otherwise.
 *
 * Description:  Decode Validity Period part of PDU.
 *-------------------------------------------------------------------------*/
static Boolean decodeVP(const Int8 *elementData_p,
                        Int8       *elementLen_p)
{
  PARAMETER_NOT_USED (elementData_p);

  switch (vpFmt)
  {
    case VPF_NOT_PRESENT:
    {
      *elementLen_p = TP_VP_NONE_LEN;
      break;
    }
    case VPF_ENHANCED:
    {
      *elementLen_p = TP_VP_ENH_LEN;
      break;
    }
    case VPF_RELATIVE:
    {
      *elementLen_p = TP_VP_REL_LEN;
      break;
    }
    case VPF_ABSOLUTE:
    {
      *elementLen_p = TP_VP_ABS_LEN;
      break;
    }
    default:
    {
      /* Illegal VPF! */
      FatalParam(vpFmt, 0, 0);
      break;
    }
  }
  return (TRUE);
}

/*--------------------------------------------------------------------------
 *
 * Function:     decodeDT
 *
 * Parameters:   elementData_p    - Pointer to PDU data for parsing
 *               elementLen_p     - Pointer to return length of processed
 *                                  PDU element.
 *
 * Returns:      Boolean          - TRUE if processing succesful, FALSE
 *                                  otherwise.
 *
 * Description:  Decode Discharge Time element of TPDU.
 *-------------------------------------------------------------------------*/
static Boolean decodeDT(const Int8 *elementData_p,
                        Int8       *elementLen_p)
{
  PARAMETER_NOT_USED (elementData_p);

  *elementLen_p = TP_DT_LEN;
  return (TRUE);
}

/*--------------------------------------------------------------------------
 *
 * Function:     decodeST
 *
 * Parameters:   elementData_p    - Pointer to PDU data for parsing
 *               elementLen_p     - Pointer to return length of processed
 *                                  PDU element.
 *
 * Returns:      Boolean          - TRUE if processing succesful, FALSE
 *                                  otherwise.
 *
 * Description:  Decode Status element of TPDU.
 *-------------------------------------------------------------------------*/
static Boolean decodeST(const Int8 *elementData_p,
                        Int8       *elementLen_p)
{
  PARAMETER_NOT_USED (elementData_p);

  *elementLen_p = TP_ST_LEN;
  return (TRUE);
}

/*--------------------------------------------------------------------------
 *
 * Function:     decodePI
 *
 * Parameters:   elementData_p    - Pointer to PDU data for parsing
 *               elementLen_p     - Pointer to return length of processed
 *                                  PDU element.
 *
 * Returns:      Boolean          - TRUE if processing succesful, FALSE
 *                                  otherwise.
 *
 * Description:  Decode Parameter Indication element of TPDU.
 *-------------------------------------------------------------------------*/
static Boolean decodePI(const Int8 *elementData_p,
                        Int8       *elementLen_p)
{
  PARAMETER_NOT_USED (elementData_p);

  *elementLen_p = TP_PI_LEN;
  return (TRUE);
}

/*--------------------------------------------------------------------------
 *
 * Function:     processDCS
 *
 * Parameters:   dcs              - DCS from SMS to check to see if it
 *                                  contains a message waiting indication
 *               recordNumber     - Record number of SMS
 *
 * Returns:      Boolean          - TRUE if message waiting indication
 *                                  specfied by data, FALSE otherwise.
 *
 * Description:  Processes supplied data to check whether message waiting
 *               indications are specfied.
 *-------------------------------------------------------------------------*/
static Boolean processDCS(const Int8 dcs, const Int8 recordNumber)
{
  Boolean result = FALSE;
  Int8    i;

  /* Check if dcs specifies message waiting.... */
  if (((dcs & 0xF0) == DCS_MSG_WAIT_STORE) ||
      ((dcs & 0xF0) == DCS_MSG_WAIT_DISCARD))
  {

     VgmuxChannelNumber profileEntity;

     for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
     {
       if ((isEntityActive (profileEntity)) &&
           (getProfileValueBit (profileEntity,
                                PROF_MUNSOL,
                                PROF_BIT_MMWT) == REPORTING_ENABLED))
       {
         /* Yes, decode type and send message to terminal.... */
         vgPutNewLine(profileEntity);
         vgPrintf (profileEntity,
                   (const Char *)"*MMWT: \"SM\",%d,",
                   recordNumber);

         /* Print out indications.  If we have a notification then we print
          * out the activation status.  If we don't have a notification we
          * print nothing....
          */
         for (i = 0; i <= 3; i++)
         {
           /* Is this the current msg type.... */
           if (i == (dcs & 0x03))
           {
             vgPrintf( profileEntity,
                       (const Char *)"%d",
                       (dcs >> 3) & 0x01);
           }

           /* Print delimiter.... */
           if (i < 3)
           {
             vgPutc(profileEntity, ',');
           }
         }

         vgPutNewLine(profileEntity);
         vgFlushBuffer(profileEntity);
         result = TRUE;
       }
     }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:     processUserDataHeader
 *
 * Parameters:   udh_p            - UDH from SMS to check to see if it
 *                                  contains a message waiting indication
 *               recordNumber     - Record number of SMS
 *
 * Returns:      Boolean          - TRUE if message waiting indication
 *                                  specfied by data, FALSE otherwise.
 *
 * Description:  Processes supplied data to check whether message waiting
 *               indications are specfied.
 *-------------------------------------------------------------------------*/
static Boolean processUserDataHeader(Int8 *udh_p, const Int8 recordNumber)
{
  Boolean     result = FALSE;
  Int8        startPos = 0,
              headerPos = 0,
              endPos = 0,
              msgTypeCount[4] = { 0, 0, 0, 0 },
              i;
  Boolean     msgTypeUsed[4] = { FALSE, FALSE, FALSE, FALSE };

  /* Parse through UDH seaching for special message coding
   * indications.  When found, display notification for each....
   */
  while (vgGetNextIE( udh_p[1],   /* Header length */
                      &udh_p[2],  /* Header data */
                      udh_p[0],   /* SM Length */
                      &udh_p[1],  /* SM Data */
                      0x01,       /* IE element - Special SMS Indication */
                      startPos,   /* Parse start position */
                      &headerPos, /* Offset of required IE */
                      &endPos     /* Start pos for next parse */
                      ) == TRUE)
  {
    /* Process data.... */
    startPos = endPos;

    msgTypeUsed[(udh_p[endPos] & 0x03)] = TRUE;
    msgTypeCount[(udh_p[endPos] & 0x03)] += udh_p[endPos+1];
    result = TRUE;
  }

  /* We found some UDH message indications, print out results.... */
  if (result == TRUE)
  {

    VgmuxChannelNumber profileEntity;

     for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
     {
       if ((isEntityActive(profileEntity)) &&
           (getProfileValueBit (profileEntity,
                                PROF_MUNSOL,
                                PROF_BIT_MMWT) == REPORTING_ENABLED))
       {
         /* Produce unsolictited response for this indication.... */
         vgPutNewLine(profileEntity);

         vgPrintf( profileEntity,
                   (const Char *)"*MMWT: \"SM\",%d,",
                   recordNumber);

         for (i = 0; i <= 3; i++)
         {
           /* Is this the current msg type.... */
           if (msgTypeUsed[i] == TRUE)
           {
             vgPrintf( profileEntity,
                       (const Char *)"%d",
                       msgTypeCount[i]);
           }

           /* Print delimiter.... */
           if (i < 3)
           {
             vgPutc(profileEntity, ',');
           }
         }

         vgPutNewLine(profileEntity);
         vgFlushBuffer(profileEntity);
       }
     }
  }
  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:     processOA
 *
 * Parameters:   origAddress_p    - OA from SMS to check to see if it
 *                                  contains a message waiting indication
 *               recordNumber     - Record number of SMS
 *
 * Returns:      Boolean          - TRUE if message waiting indication
 *                                  specfied by data, FALSE otherwise.
 *
 * Description:  Processes supplied data to check whether message waiting
 *               indications are specfied.
 *-------------------------------------------------------------------------*/
static Boolean processOA(Int8 *origAddress_p, const Int8 recordNumber)
{
  Boolean     result = FALSE;
  const Int8  addressLen = origAddress_p[0],
              addressType = origAddress_p[1],
              indType = origAddress_p[2];
  Int8        i;

  /* Check if Originator Address is coded to indicate message
   * waiting....
   */
  if ((addressLen == OA_MSG_WAITING_LEN) &&
      (addressType == OA_MSG_WAITING_TYPE))
  {
    VgmuxChannelNumber profileEntity;

    for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
    {
      if ((isEntityActive (profileEntity)) &&
          (getProfileValueBit (profileEntity,
                               PROF_MUNSOL,
                               PROF_BIT_MMWT) == REPORTING_ENABLED))
      {
        vgPutNewLine(profileEntity);
        vgPrintf (profileEntity,
                  (const Char *)"*MMWT: \"SM\",%d,",
                  recordNumber);

        /* Print out indications.  If we have a notification then we print
         * out the activation status.  If we don't have a notification we
         * print nothing....
         */
        for (i = 0; i <= 3; i++)
        {
          /* Is this the current msg type.... */
          if (i == ((indType >> 1) & 0x07))
          {
            vgPrintf( profileEntity,
                      (const Char *)"%d",
                      (indType & 0x01));
          }

          /* Print delimiter.... */
          if (i < 3)
          {
            vgPutc(profileEntity, ',');
          }
        }

        vgPutNewLine(profileEntity);
        vgFlushBuffer(profileEntity);
        result = TRUE;
      }
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeConcatenatedSMS8BitRef
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses concatenated SMS (8-bit) IE to determine whether
 *              the encoding is correct.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeConcatenatedSMS8BitRef(Int8 blockLen, Int8 *blockData_p,
                                              Int8 smDataLen, Int8 *smData_p,
                                              Int8 udDataLen, Int8 *udData_p)
{
  Int8              dataPos  = 1, /* Skip msg reference value */
                    smMax    = blockData_p[dataPos++],
                    smSeqNum = blockData_p[dataPos++];
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen != IE_SMS_8BIT_IED_LEN)
  {
    status = IE_ILLEGAL_CODING;
  }
  else
  {
    if ((smMax == 0) || (smSeqNum == 0) || (smSeqNum > smMax))
    {
      status = IE_SKIP;
    }
    else
    {
      status = IE_OK;
    }
  }
  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeSpecialSMSInd
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses 'special' SMS indication IE.  This contains 'waiting'
 *              notifications such as voicemail and email.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeSpecialSMSInd(Int8 blockLen, Int8 *blockData_p,
                                     Int8 smDataLen, Int8 *smData_p,
                                     Int8 udDataLen, Int8 *udData_p)
{
  Int8              dataPos = 0,
                    miType = blockData_p[dataPos++];
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  /* Mask bit[7] which shows store/discard ind.... */
  miType &= 0x7F;

  /* Check data.... */
  if (blockLen != IE_SMS_SPEC_IND_LEN)
  {
    status = IE_ILLEGAL_CODING;
  }
  else
  {
    switch (miType)
    {
      case VMAIL_WAITING:
      case FAX_WAITING:
      case EMAIL_WAITING:
      case OTHER_WAITING:
      {
        status = IE_OK;
        break;
      }

      default:
      {
        status = IE_ILLEGAL_CODING;
        break;
      }
    }
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeAppPort8Bit
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses 8-bit application port indication.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeAppPort8Bit(Int8 blockLen, Int8 *blockData_p,
                                   Int8 smDataLen, Int8 *smData_p,
                                   Int8 udDataLen, Int8 *udData_p)
{
  Int8              dataPos = 0,
                    destPort = blockData_p[dataPos++],
                    origPort = blockData_p[dataPos++];
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen != IE_SMS_APP_PORT8_LEN)
  {
    status = IE_ILLEGAL_CODING;
  }
  else
  {
    if ((destPort >= IE_PORT8_START) &&
        (origPort >= IE_PORT8_START))
    {
      status = IE_OK;
    }
    else
    {
      status = IE_SKIP;
    }
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeAppPort16Bit
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses 16-bit application port indication.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeAppPort16Bit(Int8 blockLen, Int8 *blockData_p,
                                    Int8 smDataLen, Int8 *smData_p,
                                    Int8 udDataLen, Int8 *udData_p)
{
  Int8              dataPos = 0,
                    destPortMsb = blockData_p[dataPos++],
                    destPortLsb = blockData_p[dataPos++],
                    origPortMsb = blockData_p[dataPos++],
                    origPortLsb = blockData_p[dataPos++];
  Int16             destPort = (destPortMsb << 8) | destPortLsb,
                    origPort = (origPortMsb << 8) | origPortLsb;
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen != IE_SMS_APP_PORT16_LEN)
  {
    status = IE_ILLEGAL_CODING;
  }
  else
  {
    if ((destPort <= IE_PORT16_END) &&
        (origPort <= IE_PORT16_END))
    {
      status = IE_OK;
    }
    else
    {
      status = IE_SKIP;
    }
  }
  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeSMSCControlParam
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses SMCS control parameter block containing status info.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeSMSCControlParam(Int8 blockLen, Int8 *blockData_p,
                                        Int8 smDataLen, Int8 *smData_p,
                                        Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (blockData_p);
  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen != IE_SMS_SMSC_CTRL_LEN)
  {
    status = IE_ILLEGAL_CODING;
  }
  else
  {
    status = IE_OK;
  }
  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeUDHSourceInd
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses UDH source indicator.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeUDHSourceInd(Int8 blockLen, Int8 *blockData_p,
                                    Int8 smDataLen, Int8 *smData_p,
                                    Int8 udDataLen, Int8 *udData_p)
{
  Int8              coding = blockData_p[0];
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen != IE_SMS_UDH_SRC_IND_LEN)
  {
    status = IE_ILLEGAL_CODING;
  }
  else
  {
    switch (coding)
    {
      case UDH_ORIGINAL_SENDER:
      case UDH_ORIGINAL_RECEIVER:
      case UDH_SMSC:
        status = IE_OK;
        break;

      default:
        status = IE_ILLEGAL_CODING;
        break;
    }
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeConcatenatedSMS16BitRef
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses concatenated SMS block (16-bit).
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeConcatenatedSMS16BitRef( Int8 blockLen, Int8 *blockData_p,
                                                Int8 smDataLen, Int8 *smData_p,
                                                Int8 udDataLen, Int8 *udData_p)
{
  Int8              dataPos = 2,  /* Skip msg ref data */
                    smMax    = blockData_p[dataPos++],
                    smSeqNum = blockData_p[dataPos++];
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen != IE_SMS_16BIT_IED_LEN)
  {
    status = IE_ILLEGAL_CODING;
  }
  else
  {
    if ((smMax == 0) || (smSeqNum == 0) || (smSeqNum > smMax))
    {
      status = IE_SKIP;
    }
    else
    {
      status = IE_OK;
    }
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeWirelessCtrlMsgProtocol
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses WCMP data.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeWirelessCtrlMsgProtocol( Int8 blockLen, Int8 *blockData_p,
                                                Int8 smDataLen, Int8 *smData_p,
                                                Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (blockData_p);
  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen > IE_SMS_MIN_WCMP_MSG_LEN)
  {
    status = IE_OK;
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return status;
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeSTKSecurityHeaders
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses STK security header.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeSTKSecurityHeaders(  Int8 blockLen, Int8 *blockData_p,
                                           Int8 smDataLen, Int8 *smData_p,
                                           Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (blockData_p);
  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (IE_SMS_STK_SECURITY_LEN == blockLen)
  {
    status = IE_OK;
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeSMESpecific
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses SME specific data header.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeSMESpecific(Int8 blockLen, Int8 *blockData_p,
                                   Int8 smDataLen, Int8 *smData_p,
                                   Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_OK;

  PARAMETER_NOT_USED (blockLen);
  PARAMETER_NOT_USED (blockData_p);
  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  /* No data to analyse */

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeSCSpecific
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses SC specific data header.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeSCSpecific(Int8 blockLen, Int8 *blockData_p,
                                  Int8 smDataLen, Int8 *smData_p,
                                  Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_OK;

  PARAMETER_NOT_USED (blockLen);
  PARAMETER_NOT_USED (blockData_p);
  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  /* No data to analyse */

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeTextFormatting
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses text formatting header.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeTextFormatting(Int8 blockLen, Int8 *blockData_p,
                                      Int8 smDataLen, Int8 *smData_p,
                                      Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;
  Int8              dataPos = 0,
                    txtStartPos = blockData_p[dataPos++];

  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if ((blockLen >= IE_SMS_TEXT_FMT_MIN_LEN) &&
      (blockLen <= IE_SMS_TEXT_FMT_MAX_LEN))
  {
    if (txtStartPos < smDataLen)
    {
      status = IE_OK;
    }
    else
    {
      status = IE_ILLEGAL_CODING;
    }
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodePredefinedSound
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses predefined sound block.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodePredefinedSound(Int8 blockLen, Int8 *blockData_p,
                                       Int8 smDataLen, Int8 *smData_p,
                                       Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;
  Int8              dataPos = 0,
                    sndStartPos = blockData_p[dataPos++],
                    sndId = blockData_p[dataPos];

  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen == IE_SMS_PREDEF_SOUND_LEN)
  {
    if ((sndStartPos < smDataLen) && (sndId <= IE_MAX_PREDEF_SOUND_ID))
    {
      status = IE_OK;
    }
    else
    {
      status = IE_ILLEGAL_CODING;
    }
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeUserDefinedSound
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses user defined sound block.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeUserDefinedSound(Int8 blockLen, Int8 *blockData_p,
                                        Int8 smDataLen, Int8 *smData_p,
                                        Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;
  Int8              dataPos = 0,
                    sndStartPos = blockData_p[dataPos++];

  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen <= IE_SMS_USERDEF_SOUND_LEN_MAX)
  {
    if (sndStartPos < smDataLen)
    {
      status = IE_OK;
    }
    else
    {
      status = IE_ILLEGAL_CODING;
    }
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodePredefinedAnimation
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses predefined animation data
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodePredefinedAnimation(Int8 blockLen, Int8 *blockData_p,
                                           Int8 smDataLen, Int8 *smData_p,
                                           Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;
  Int8              dataPos = 0,
                    animStartPos = blockData_p[dataPos++],
                    animId = blockData_p[dataPos];

  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen == IE_SMS_PREDEF_ANIM_LEN)
  {
    if ((animStartPos < smDataLen) && (animId <= IE_MAX_PREDEF_ANIM_ID))
    {
      status = IE_OK;
    }
    else
    {
      status = IE_ILLEGAL_CODING;
    }
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeLargeAnimation
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses user defined large animation data
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeLargeAnimation(Int8 blockLen, Int8 *blockData_p,
                                      Int8 smDataLen, Int8 *smData_p,
                                      Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;
  Int8              dataPos = 0,
                    animStartPos = blockData_p[dataPos++];

  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen == IE_SMS_LARGE_ANIM_LEN)
  {
    if (animStartPos < smDataLen)
    {
      status = IE_OK;
    }
    else
    {
      status = IE_ILLEGAL_CODING;
    }
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeSmallAnimation
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses user defined small animation data
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeSmallAnimation(Int8 blockLen, Int8 *blockData_p,
                                      Int8 smDataLen, Int8 *smData_p,
                                      Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;
  Int8              dataPos = 0,
                    animStartPos = blockData_p[dataPos++];

  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen == IE_SMS_SMALL_ANIM_LEN)
  {
    if (animStartPos < smDataLen)
    {
      status = IE_OK;
    }
    else
    {
      status = IE_ILLEGAL_CODING;
    }
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeLargePicture
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses user defined large picture data
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeLargePicture(Int8 blockLen, Int8 *blockData_p,
                                    Int8 smDataLen, Int8 *smData_p,
                                    Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;
  Int8              dataPos = 0,
                    picStartPos = blockData_p[dataPos++];

  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen == IE_SMS_LARGE_PICTURE_LEN)
  {
    if (picStartPos < smDataLen)
    {
      status = IE_OK;
    }
    else
    {
      status = IE_ILLEGAL_CODING;
    }
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeSmallPicture
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses user defined small picture data
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeSmallPicture(Int8 blockLen, Int8 *blockData_p,
                                    Int8 smDataLen, Int8 *smData_p,
                                    Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;
  Int8              dataPos = 0,
                    picStartPos = blockData_p[dataPos++];

  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen == IE_SMS_SMALL_PICTURE_LEN)
  {
    if (picStartPos < smDataLen)
    {
      status = IE_OK;
    }
    else
    {
      status = IE_ILLEGAL_CODING;
    }
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeVariablePicture
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses user defined variable picture data
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeVariablePicture(Int8 blockLen, Int8 *blockData_p,
                                       Int8 smDataLen, Int8 *smData_p,
                                       Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;
  Int8              dataPos = 0,
                    picStartPos = blockData_p[dataPos++];

  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen <= IE_SMS_VARIABLE_PICTURE_LEN)
  {
    if (picStartPos < smDataLen)
    {
      status = IE_OK;
    }
    else
    {
      status = IE_ILLEGAL_CODING;
    }
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeUserPromptIndicator
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses user prompt indication
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeUserPromptIndicator(Int8 blockLen, Int8 *blockData_p,
                                           Int8 smDataLen, Int8 *smData_p,
                                           Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;
  Int8              dataPos = 0,
                    numObjects = blockData_p[dataPos++],
                    ieCount = 0,
                    iei,
                    ieLen;

  if (blockLen == IE_SMS_USER_PROMPT_IND_LEN)
  {
    status = IE_OK;

    /* Parse next n blocks to check they are valid objects for
     * User Prompt Indication....
     */
    while ((status == IE_OK) && (ieCount < numObjects))
    {
      if (&blockData_p[dataPos] < (udData_p + udDataLen))
      {
        iei = blockData_p[dataPos++];
        ieLen = blockData_p[dataPos++];

        /* Check length valid.... */
        if (ieLen >= udDataLen)
        {
          status = IE_ILLEGAL_CODING;
        }

        else
        {
          /* Check block is okay.... */
          switch (iei)
          {
            case 0x10:
            {
              status = decodeLargePicture(ieLen,
                                          &blockData_p[dataPos],
                                          smDataLen,
                                          smData_p,
                                          udDataLen,
                                          udData_p);
              break;
            }

            case 0x11:
            {
              status = decodeSmallPicture(ieLen,
                                          &blockData_p[dataPos],
                                          smDataLen,
                                          smData_p,
                                          udDataLen,
                                          udData_p);
              break;
            }

            case 0x12:
            {
              status = decodeVariablePicture( ieLen,
                                              &blockData_p[dataPos],
                                              smDataLen,
                                              smData_p,
                                              udDataLen,
                                              udData_p);
              break;
            }

            case 0x0C:
            {
              status = decodeUserDefinedSound(ieLen,
                                              &blockData_p[dataPos],
                                              smDataLen,
                                              smData_p,
                                              udDataLen,
                                              udData_p);
              break;
            }

            default:
            {
              status = IE_ILLEGAL_CODING;
              break;
            }
          }
          ieCount++;
          dataPos += ieLen;
        }
      }
      else
      {
        /*
         * Force loop to drop out.
         */        
        break;  
      }
    }
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeExtendedObject
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses user extended object
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeExtendedObject(Int8 blockLen, Int8 *blockData_p,
                                      Int8 smDataLen, Int8 *smData_p,
                                      Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (blockData_p);
  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen > IE_SMS_EXT_OBJ_MIN_LEN)
  {
    status = IE_OK;
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeReusedExtendedObject
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses reused extended object data
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeReusedExtendedObject(Int8 blockLen, Int8 *blockData_p,
                                            Int8 smDataLen, Int8 *smData_p,
                                            Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (blockData_p);
  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen == IE_SMS_REUSED_EXT_OBJ_LEN)
  {
    status = IE_OK;
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeCompressionControl
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses compression control used to indicate a compressed
 *              octet sequence.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeCompressionControl(Int8 blockLen, Int8 *blockData_p,
                                          Int8 smDataLen, Int8 *smData_p,
                                          Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (blockData_p);
  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen > IE_SMS_COMP_CONTROL_MIN_LEN)
  {
    status = IE_OK;
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeObjectDistributionIndicator
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses distribution control of particualar objects.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeObjectDistributionIndicator( Int8 blockLen, Int8 *blockData_p,
                                                    Int8 smDataLen, Int8 *smData_p,
                                                    Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (blockData_p);
  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen == IE_SMS_OBJECT_DIST_IND_LEN)
  {
    status = IE_OK;
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeReplyAddressElement
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses alternate reply address data.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeReplyAddressElement(Int8 blockLen, Int8 *blockData_p,
                                           Int8 smDataLen, Int8 *smData_p,
                                           Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (blockData_p);
  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen <= IE_SMS_REPLY_ADDR_LEN_MAX)
  {
    status = IE_OK;
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeExtendedObjectDataRequest
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses extended object data request.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeExtendedObjectDataRequest( Int8 blockLen, Int8 *blockData_p,
                                                  Int8 smDataLen, Int8 *smData_p,
                                                  Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (blockData_p);
  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen == IE_SMS_EXT_OBJ_DATA_REQ_LEN)
  {
    status = IE_OK;
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeHyperlinkFormatElement
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses extended object data request.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeHyperlinkFormatElement(Int8 blockLen, Int8 *blockData_p,
                                              Int8 smDataLen, Int8 *smData_p,
                                              Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (blockData_p);
  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen == IE_SMS_HYPERLINK_FMT_LEN)
  {
    status = IE_OK;
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeRFC822EMailHeader
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses RFC 822 Internet electronic mail existance indication.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeRFC822EMailHeader(Int8 blockLen, Int8 *blockData_p,
                                         Int8 smDataLen, Int8 *smData_p,
                                         Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (blockData_p);
  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  if (blockLen == IE_SMS_EMAIL_HDR_IND_LEN)
  {
    status = IE_OK;
  }
  else
  {
    status = IE_ILLEGAL_CODING;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeStandardWVGObject
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses standard WVG (Wireless Vector Graphic) IE to verify
 *              data correct.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeStandardWVGObject(Int8 blockLen, Int8 *blockData_p,
                                         Int8 smDataLen, Int8 *smData_p,
                                         Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (blockLen);
  PARAMETER_NOT_USED (blockData_p);
  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    decodeCharacterSizeWVGObject
 *
 * Parameters:  blockLen     - Length of IED for this IE
 *              blockData_p  - Pointer to IED
 *              smDataLen    - Length of SM data
 *              smData_p     - Pointer to SM data
 *              udDataLen    - Length of whole UDH
 *              udData_p     - Pointer to whole UDH
 *
 * Returns:     VgIEDecodeStatus - Status of parse operation
 *
 * Description: Parses character size WVG (Wireless Vector Graphic) IE to
 *              verify data correct.
 *-------------------------------------------------------------------------*/
VgIEDecodeStatus decodeCharacterSizeWVGObject(Int8 blockLen, Int8 *blockData_p,
                                              Int8 smDataLen, Int8 *smData_p,
                                              Int8 udDataLen, Int8 *udData_p)
{
  VgIEDecodeStatus  status = IE_SKIP;

  PARAMETER_NOT_USED (blockLen);
  PARAMETER_NOT_USED (blockData_p);
  PARAMETER_NOT_USED (smDataLen);
  PARAMETER_NOT_USED (smData_p);
  PARAMETER_NOT_USED (udDataLen);
  PARAMETER_NOT_USED (udData_p);

  return (status);
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:     vgMsSMSMsgFetch
 *
 * Parameters:   entity           - entity
 *               msgID            - SMS to read from SIM
 *
 * Returns:      Boolean          - TRUE if operation sucessful, FALSE
 *                                  otherwise.
 *
 * Description:  Sends the required signals to fetch a SMS from the SIM
 *               without changing the SMS status (i.e. unread to read).
 *-------------------------------------------------------------------------*/
Boolean vgMsSMSMsgFetch(const  VgmuxChannelNumber entity,
                        Int8   msgId)
{
  Boolean           result = TRUE;
  SimLockContext_t  *simLockContext_p = ptrToSimLockContext(entity);
  ResultCode_t      resultCode;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(simLockContext_p != PNULL, entity, 0, 0);
#endif
  commandData[0] = VG_CRSM_CLASS_GSM_APP;
  commandData[1] = VG_CRSM_COMMAND_READ_RECORD;
  commandData[2] = msgId;
  commandData[3] = 0x04;
  commandData[4] = 0xB0;

  /* We use command reference to distinguish our SIM reads from others.... */
  simLockContext_p->simGenAccess.length     = VG_CRSM_COMMAND_HEADER_SIZE;
  simLockContext_p->simGenAccess.efId       = SIM_EF_SMS;
  simLockContext_p->simGenAccess.dirId      = SIM_DIR_DF_TELECOM;
  simLockContext_p->simGenAccess.rootDirId  = SIM_DIR_INVALID;
  simLockContext_p->simGenAccess.commandRef = (VG_SMS_MSG_WAITING | msgId);

  memcpy (&simLockContext_p->simGenAccess.commandData[0],
           &commandData[0],
            VG_CRSM_COMMAND_HEADER_SIZE);

  resultCode = vgChManContinueAction(entity, SIG_APEX_SIM_GEN_ACCESS_REQ);

  /* If operation not proceeding (i.e. it failed) store msg ID and indicate
   * user should try again....
   */
  if (resultCode != RESULT_CODE_PROCEEDING)
  {
    result = FALSE;
    retryId = msgId;
  }

  return result;
}

/*--------------------------------------------------------------------------
 *
 * Function:     vgMsSMSMsgTimerExpiry
 *
 * Parameters:   entity           - entity
 *
 * Returns:      Nothing
 *
 * Description:  Handle timer expiry for SMS read.  Will be called in the
 *               event of the SMS read failing.  In which case we will
 *               keep attempting to read SMS until succesful.
 *-------------------------------------------------------------------------*/
void vgMsSMSMsgTimerExpiry (const VgmuxChannelNumber entity)
{
  if (vgMsSMSMsgFetch(entity,
                       retryId) == FALSE)
  {
    /* Can't read EFSMS - need to try again shortly.... */
    vgCiStartTimer(TIMER_TYPE_SMS_MSG, entity);
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:     vgMsSMSTr2mTimerExpiry
 *
 * Parameters:   entity           - entity
 *
 * Returns:      Nothing
 *
 * Description:  Handle timer expiry for SMS acknowledgment.  Will be called if
 *               SMS is not acknowledged - when expected - withing network timeout
 *               that is defined as TR2M of 12s
 *-------------------------------------------------------------------------*/
void vgMsSMSTr2mTimerExpiry (const VgmuxChannelNumber entity)
{
  SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext();

  smsCommonContext_p->msgToAck = FALSE;
  /* following CNMA 27.005 further routing is disabled, <mt> <ds> set to 0 */
  setProfileValue (entity, PROF_CNMI + 1, 0);
  setProfileValue (entity, PROF_CNMI + 3, 0);
}

/*--------------------------------------------------------------------------
 *
 * Function:     vgMsSMSCmmsTimerExpiry
 *
 * Parameters:   entity           - entity
 *
 * Returns:      Nothing
 *
 * Description:  Handle timer expiry for More messages to send functionality.
 *               Will be called for CMMS mode set to 1
 *-------------------------------------------------------------------------*/
void vgMsSMSCmmsTimerExpiry (const VgmuxChannelNumber entity)
{
  SmsCommonContext_t *smsCommonContext_p = ptrToSmsCommonContext();

  /* following CMMS 27.005 for moreMsgsToSend value 1, after the timer expiry
   * the value will be set back to 0 */
  smsCommonContext_p->moreMsgsToSendMode = 0;

  vgSigApexSmSendMoreReq (entity);
}

/*--------------------------------------------------------------------------
 *
 * Function:     vgMsSMSIsSimResponse
 *
 * Parameters:   commandRef       - command reference number from
 *                                  SIG_APEX_SIM_GEN_ACCESS_CNF signal
 *
 * Returns:      Boolean          - TRUE if signal was generated as part
 *                                  of the SMS msg waiting procedure.
 *
 * Description:  Check a message reference number to ascertain whether it
 *               was sent during part of a SMS decode.
 *-------------------------------------------------------------------------*/
Boolean vgMsSMSIsSimResponse(const Int16 commandRef)
{
  Boolean result = FALSE;

  if ((commandRef & 0xFF00) == VG_SMS_MSG_WAITING)
  {
    result= TRUE;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:     vgMsSMSMsgProcess
 *
 * Parameters:   efRecord_p       - PDU returned in the
 *                                  SIG_APEX_SIM_GEN_ACCESS_CNF signal.
 *               recordNumber     - ID of SMS
 *
 * Returns:      Nothing
 *
 * Description:  Process PDU returned from SIM.  Note that these PDUs will
 *               have an extra 'status' word appended to the start of them
 *               which contains the SMS status.
 *-------------------------------------------------------------------------*/
void vgMsSMSMsgProcess(const Int8 *efRecord_p, const Int8 recordNumber)
{
  Int8            status = efRecord_p[0],
                  pduOffset;
  VgSMSTPDUType   vgSMSTPDUType;
  Boolean         handled = FALSE;

  /* First byte contains message status and type of SM.  First bit contains
   * used/unused status of EFsms file - all the ones we read MUST be used
   * (obviously) so ignore if not... */
  if (status & 0x01)
  {
    status >>= 1;

    /* Process message status:
     *   7 6 5 4 3 2 1 0
     *   x x x x x x x 0  Unused
     *   x x x x x 0 0 1  Rx: Msg Read
     *   x x x x x 0 1 1  Rx: Msg to be Read
     *   x x x x x 1 1 1  Tx: To be sent
     *   x x x x x 1 0 1  Tx: Sent
     */
    switch (status & 0x03)
    {
      case 0x01:  /* Rx: To be read */
      case 0x00:  /* Rx: Read */
      {
        vgSMSTPDUType = VG_SMS_TPDU_DELIVER;
        break;
      }
      case 0x02:  /* Tx: Sent */
      case 0x03:  /* Tx: To be sent */
      {
        vgSMSTPDUType = VG_SMS_TPDU_SUBMIT;
        break;
      }
      default:
      {
        vgSMSTPDUType = VG_SMS_TPDU_MAX;
        /* Illegal message type */
        WarnParam(status, 0, 0);
        break;
      }
    }

    if (vgSMSTPDUType != VG_SMS_TPDU_MAX)
    {

      /* Run a pre-parse.  This builds up information on the PDU such as the
       * TP-UDHI and the TP-VPF....
       */
      vgMsParsePDUForElement( &efRecord_p[1],
                              vgSMSTPDUType,
                              VG_SMS_TP_UD,
                              &pduOffset);

      /* Check for user data header indication, if present, check for msg
       * waiting indications.  Note that each indication is ordered such that
       * if the highest order indication is set then none of the others can
       * over-ride it....
       */
      if (udhPresent == TRUE)
      {
        if (vgMsParsePDUForElement( &efRecord_p[1],
                                    vgSMSTPDUType,
                                    VG_SMS_TP_UD,
                                    &pduOffset) == TRUE)
        {
          handled = processUserDataHeader((Int8 *)&efRecord_p[1+pduOffset],
                                          recordNumber);
        }
      }

      /* Check DCS for message waiting type.... */
      if ((vgMsParsePDUForElement(&efRecord_p[1],
                                  vgSMSTPDUType,
                                  VG_SMS_TP_DCS,
                                  &pduOffset) == TRUE) &&
          (handled == FALSE))
      {
        /* Got DCS, now check for coding scheme.... */
        handled = processDCS( efRecord_p[1+pduOffset],
                              recordNumber);
      }

      /* Check for message waiting indication encoded in OA.... */
      if ((vgMsParsePDUForElement(&efRecord_p[1],
                                  vgSMSTPDUType,
                                  VG_SMS_TP_OA,
                                  &pduOffset) == TRUE) &&
          (handled == FALSE))
      {
        handled = processOA((Int8 *)&efRecord_p[1+pduOffset],
                            recordNumber);
      }
    }
  }

  /* Now we need to set the unsol event to free */
  vgSmsUtilSetUnsolicitedEventHandlerFree();  
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCheckUDHeader
 *
 * Parameters:  headerLen     - Length of user data header
 *              headerData_p  - Pointer to user data header
 *              smDataLen     - Length of SM data
 *              smData_p      - Pointer to SM data
 *
 * Returns:     Boolean       - TRUE if encoding correct, FALSE otherwise.
 *
 * Description: Parses user data header, checking each Information Element
 *              (IE) to determine whether it is correctly encoded.
 *-------------------------------------------------------------------------*/
Boolean vgCheckUDHeader(Int8 headerLen, Int8 *headerData_p,
                        Int8 smDataLen, Int8 *smData_p)
{
    Boolean             result = TRUE,
                        entryFound = FALSE;
    Int8                index,
                        iei,
                        ieiLen,
                        dataPos = 0,
                        presentIE[MAX_SMS_IE_ELEMENTS],
                        numPresentIE = 0,
                        i,
                        ieiCount;
    VgIEDecodeStatus    status = IE_OK;

    if( headerLen <= smDataLen)
    {
        while ((headerLen > dataPos) && (status != IE_ILLEGAL_CODING))
        {
            /* Fetch IEI and IELen parameters.... */
            entryFound = FALSE;
            iei = headerData_p[dataPos++];
            ieiLen = headerData_p[dataPos++];
            index = 0;

            /* Check lookup table to see if iei header matches known UDH elements.... */
            while ((index < SMS_UDH_DECODE_ENTRIES) && (entryFound == FALSE))
            {
                if (vgSMSUDHDecodeLookup[index].isRange == TRUE)
                {
                    if( (iei >= vgSMSUDHDecodeLookup[index].startRange) &&
                        (iei <= vgSMSUDHDecodeLookup[index].endRange))
                    {
                        entryFound = TRUE;
                    }
                }
                else
                {
                    if (iei == vgSMSUDHDecodeLookup[index].startRange)
                    {
                        entryFound = TRUE;
                    }
                }

                if (entryFound == FALSE)
                {
                    index++;
                }
            }

            /* Has entry been found.... */
            if ((entryFound == TRUE) && (index < SMS_UDH_DECODE_ENTRIES ))
            {
                /* Yes, run parser.... */
                status = vgSMSUDHDecodeLookup[index].handlerFnc(ieiLen,
                                                                &headerData_p[dataPos],
                                                                smDataLen,
                                                                &smData_p[0],
                                                                headerLen,
                                                                &headerData_p[0]);
                dataPos += ieiLen;

                /* Add this IE to list of present IE if status OK.... */
                if ((status == IE_OK) && (numPresentIE < MAX_SMS_IE_ELEMENTS))
                {
                    presentIE[numPresentIE++] = iei;
                    ieiCount = 0;

                    /* Check that non-repeatable IEs appear no more than once.... */
                    if (vgSMSUDHDecodeLookup[index].isRepeatable == FALSE)
                    {
                        for (i = 0; i < numPresentIE; i++)
                        {
                            if (presentIE[i] == iei)
                            {
                                ieiCount++;
                            }
                        }

                        if (ieiCount > 1)
                        {
                            status = IE_ILLEGAL_CODING;
                        }
                    }
                }
            }
            else
            {
                /* Not found, return error.... */
                status = IE_ILLEGAL_CODING;
            }
        }

        if (status == IE_ILLEGAL_CODING)
        {
            result = FALSE;
        }
    }
    else
    {
        result = FALSE;
    }

    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCheckUDHeader
 *
 * Parameters:  headerLen     - Length of user data header
 *              headerData_p  - Pointer to user data header
 *              smDataLen     - Length of SM data
 *              smData_p      - Pointer to SM data
 *
 * Returns:     Boolean       - TRUE if encoding correct, FALSE otherwise.
 *
 * Description: Parses user data header, checking each Information Element
 *              (IE) to determine whether it is correctly encoded.
 *-------------------------------------------------------------------------*/
Boolean vgGetNextIE(const Int8 headerLen,
                    Int8 *headerData_p,
                    const Int8 smDataLen,
                    Int8 *smData_p,
                    const Int8 ieElement,
                    const Int8 startPos,
                    Int8 *iePosition_p,
                    Int8 *nextScanStart_p)
{
  Boolean           result = TRUE,
                    entryFound = FALSE,
                    ieFound = FALSE;
  Int8              index,
                    iei,
                    ieiLen,
                    dataPos = startPos;
  VgIEDecodeStatus  status = IE_OK;

  while ((headerLen > dataPos) && (status != IE_ILLEGAL_CODING) && (ieFound == FALSE))
  {
    /* Fetch IEI and IELen parameters.... */
    entryFound = FALSE;
    iei = headerData_p[dataPos++];
    ieiLen = headerData_p[dataPos++];
    index = 0;

    /* Check lookup table to see if iei header matches known UDH elements.... */
    while ((index < SMS_UDH_DECODE_ENTRIES) && (entryFound == FALSE))
    {
      if (vgSMSUDHDecodeLookup[index].isRange == TRUE)
      {
        if ((iei >= vgSMSUDHDecodeLookup[index].startRange) &&
            (iei <= vgSMSUDHDecodeLookup[index].endRange))
        {
          entryFound = TRUE;
        }
      }
      else
      {
        if (iei == vgSMSUDHDecodeLookup[index].startRange)
        {
          entryFound = TRUE;
        }
      }

      if (entryFound == FALSE)
      {
        index++;
      }
    }

    /* Has entry been found.... */
    if ((entryFound == TRUE) && (index < SMS_UDH_DECODE_ENTRIES))
    {
      /* Found an IE, check if it's the one we're interseted in.... */
      if (vgSMSUDHDecodeLookup[index].isRange == TRUE)
      {
        if ((ieElement >= vgSMSUDHDecodeLookup[index].startRange) &&
            (ieElement <= vgSMSUDHDecodeLookup[index].endRange))
        {
          /* Return start of IE element (inc. header and len).... */
          *iePosition_p = (dataPos-2);
          ieFound = TRUE;
        }
      }

      else
      {
        if (ieElement == vgSMSUDHDecodeLookup[index].startRange)
        {
          /* Return start of IE element (inc. header and len).... */
          *iePosition_p = (dataPos-2);
          ieFound = TRUE;
        }
      }

      /* Yes, run parser.... */
      status = vgSMSUDHDecodeLookup[index].handlerFnc(ieiLen,
                                                      &headerData_p[dataPos],
                                                      smDataLen,
                                                      &smData_p[0],
                                                      headerLen,
                                                      &headerData_p[0]);
      dataPos += ieiLen;

      /* If done, record end position of ie (hence start position)
       * of next ie.... */
      if (ieFound == TRUE)
      {
        *nextScanStart_p = dataPos;
      }
    }
    else
    {
      /* Not found, return error.... */
      status = IE_ILLEGAL_CODING;
    }
  }

  if ((status == IE_ILLEGAL_CODING) || (ieFound == FALSE))
  {
    result = FALSE;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:     vgMsParsePDUForElement
 *
 * Parameters:   pdu_p            - PDU in hexadecimal format.
 *               vgSMSTPDUType    - Type of PDU provided
 *               vgSMSTPDUElement - Type of PDU element we're looking for
 *               pduElement_p     - (out) Pointer to element requested or
 *                                  PNULL if not found.
 *
 * Returns:      Boolean          - TRUE if element found, FALSE otherwise.
 *
 * Description:  Parse PDU for specified TPDU element.
 *-------------------------------------------------------------------------*/
Boolean vgMsParsePDUForElement( const Int8             *pdu_p,
                                const VgSMSTPDUType    vgSMSTPDUType,
                                const VgSMSTPDUElement vgSMSTPDUElement,
                                Int8                   *pduOffset_p)
{
  Int8            pduPosition = 0,
                  pduElementLen = 0;
  VgSMSTPDUDecode *vgSMSTPDUDecode = PNULL;
  Boolean         result = TRUE;

  /* Reset status variables.... */
  udhPresent = FALSE;
  vpFmt = VPF_NOT_PRESENT;

  FatalAssert(vgSMSTPDUType < VG_SMS_TPDU_MAX);

  /* Set-up pointer to correct TPDU decode 'matrix'.... */
  vgSMSTPDUDecode = vgTPDUDecode[vgSMSTPDUType];
  *pduOffset_p = 0;

  /* Decode TPDU type.  Each element will be decode in turn and the
   * offset (length) calculated until the desired PDU element is
   * located.  The offset of this element will then be returned....
   */
  while ( (vgSMSTPDUDecode->vgSMSTPDUElement != vgSMSTPDUElement)  &&
          (vgSMSTPDUDecode->vgSMSTPDUElement != VG_SMS_TP_NULL) &&
          (result == TRUE))
  {
    result = vgSMSTPDUDecode->vgHandlerFn(&pdu_p[pduPosition],
                                          &pduElementLen);
    pduPosition += pduElementLen;
    vgSMSTPDUDecode++;
  }

  /* If decode okay, set offset, otherwise offset is zero.... */
  if (result == TRUE)
  {
    *pduOffset_p = pduPosition;
  }

  return (result);
}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */


