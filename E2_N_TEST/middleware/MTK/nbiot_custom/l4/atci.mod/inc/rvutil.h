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
 **************************************************************************/

#ifndef RVUTIL_H
#define RVUTIL_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <ki_sigbuf.h>
#include <alsi_sig.h>
#include <ablm_typ.h>
#include <rvdata.h>
#include <cimux_sig.h>
#include <cici_sig.h>
#include <rvcrman.h>
#include <rvctcfg.h>
#include <mux_cfg.h>
#include <frhsl.h>
/***************************************************************************
 * Manifest Constants
 **************************************************************************/

#define  FIRST_BYTE(x) (Int8)((x & 0xFF00) >> 8)
#define SECOND_BYTE(x) (Int8)((x & 0x00FF) >> 0)

#define MAX_BAUD_RATE_STRING_LENGTH (7)   /* includes space for \0 */
#define MAX_IPS_TYPE_STRING_LENGTH (10)   /* includes space for \0 */

/***************************************************************************
 * Type Definitions
 **************************************************************************/

typedef ResultCode_t (*VgProcFunc)(CommandLine_t *commandBuffer_p,
                                    const VgmuxChannelNumber entity);

typedef struct AtCmdControlTag
{
  Char           *string;       /* Command string                   */
  VgProcFunc      procFunc;     /* Command line processing function */
  CommandId_t     commandId;    /* Command identifier               */
  AtCmdAccessMask accessMask;   /* Command Access mask              */
} AtCmdControl;

typedef void (*VgSigCnfProc)(const SignalBuffer *signal,
                              const VgmuxChannelNumber entity);

typedef struct SignalCnfFuncTag
{
  const SignalId     signalId;  /* signal identifier */
  const VgSigCnfProc procFunc;  /* signal processing function */
} SignalCnfFunc;


typedef void (*VgSigIndProc)(const SignalBuffer *signal);

typedef struct SignalIndFuncTag
{
  const SignalId     signalId;  /* signal identifier */
  const VgSigIndProc procFunc;  /* signal processing function */
} SignalIndFunc;

typedef struct IprTag
{
  const Char iprString [MAX_BAUD_RATE_STRING_LENGTH];
}
Ipr;

typedef struct IpsType
{
  const Char string [MAX_IPS_TYPE_STRING_LENGTH];
}
IpsType;

/***************************************************************************
 *  Macros
 **************************************************************************/
/* return number of elements in an array */
#define VG_ARRAY_LENGTH(X) (sizeof(X)/sizeof(X[0]))

/***************************************************************************
 *  Exported variables
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

/* Get rid of implicit cast errors from Char* -> char* */
#define vgStrLen(X)      (strlen((char*)(X)))
#define vgStrCpy(X, Y)   (strcpy((char*)(X),(char*)(Y)))
#define vgStrNCpy(X,Y,Z) (strncpy((char*)(X),(char*)(Y),(size_t)(Z)))
#define vgStrCmp(X,Y)    (strcmp((char*)(X),(char*)(Y)))
#define vgStrNCmp(X,Y,Z) (strncmp((char*)(X),(char*)(Y),(size_t)(Z)))
#define vgStrChr(X, Y)   (strchr((char*)(X),(Y)))


Int8 vgGetBcd (const Bcd *base_p, Int16 pos);
void vgPutBcd (Bcd *base, Int16  pos, Int8 val);

/* Generic Parser */

Boolean parseCommandBuffer (const AtCmdControl *atCommandTable_p,
                             const VgmuxChannelNumber entity);

/* Command parsing functions */

Boolean isRemainingParameter( const CommandLine_t *commandBuffer_p);

ExtendedOperation_t getExtendedOperation (CommandLine_t *commandBuffer_p);
Boolean getExtendedString (CommandLine_t *commandBuffer_p,
                            Char *outString,
                             Int16 maxStringLength,
                              Int16 *outStringLength);
Boolean getExtendedStringWithoutQuotes (CommandLine_t *commandBuffer_p,
                            Char *outString,
                             Int16 maxStringLength,
                             Int16 *outStringLength);
Boolean getExtendedStringWithoutComma (CommandLine_t *commandBuffer_p,
                                        Char *outString,
                                         Int16 maxStringLength,
                                           Int16 *outStringLength);

Boolean getExtendedParameterSigned (CommandLine_t *commandBuffer_p,
                                     SignedInt32 *value_p,
                                      Int32 inValue);

Boolean getExtendedParameter (CommandLine_t *commandBuffer_p,
                               Int32 *value_p,
                                Int32 inValue);

Boolean getExtendedParameterPresent (CommandLine_t *commandBuffer_p,
                                      Int32 *value_p,
                                       const Int32 inValue,
                                        Boolean *supplied_p);

/* job106314: modify 2nd parameter for function call */
Boolean checkForExtendedCommandSeparator (const VgmuxChannelNumber entity,
                                           ScanParseContext_t *scanParseContext_p);

Int32 getDecimalValue (CommandLine_t *commandBuffer_p);
Boolean getDecimalValueSafe (CommandLine_t *commandBuffer_p, Int32* value);
Boolean getExtendedHexParameter (CommandLine_t *commandBuffer_p,
                                  Int32 *value_p,
                                   const Int32 inValue,
                                    Boolean *supplied_p);
Int32 getHexaNum (CommandLine_t *commandBuffer_p);
Boolean getHexaValue (Char c, Int8 *value);
Boolean getDeciaValue (Char c, Int8 *value);


/* String manipulation and conversion functions */

void vgConvertStringToLower (const Char *inputString, Char *outputString);
void vgConvertStringToUpper (const Char *inputString, Char *outputString);

Char *vgPrintLine(Char *out, const Char *in);
Char *vgPrint32BitNum (Char *out, Int32 num);

Char *vgOp8BitHex (Int8 input, Char * output);
Char *vgOp32BitHex (Int32 input, Int8 bytes, Char * output);

Boolean vgHexaToBin (const Char *input, Int32 *output);

Boolean vgBinStrToBin (const Char *input, Int8 *output);

void vgInt8ToBinString( const Int8 input, Int8 num_bits, Char *output );

void vgInt16ToAscii (const Int16 input, Char *output);

#if defined (FEA_PHONEBOOK)
Char *vgConvBcdToText (BcdNumberType numType,
                        const Bcd *bcdData_p,
                         Int8 bcdDataLen,
                          Char *textDialNum_p);

Char *vgConvBcdToTextForPhonebook (BcdNumberType numType,
                        const Bcd *bcdData_p,
                         Int8 bcdDataLen,
                          Char *textDialNum_p);

void vgConvTextToBcd (const Char *asciiDialNum_p,
                       Int16 asciiLen,
                        BcdNumberType *numType_p,
                         BcdNumberPlan *numPlan_p,
                          Bcd *bcdData_p,
                           Int8 *bcdDataLen_p,
                            VgmuxChannelNumber entity);
#endif /* FEA_PHONEBOOK */

void vgConvTextToSparseBcd (const Char *asciiDialNum_p,
                             Int8 asciiLen,
                              BcdNumberType *numType_p,
                               BcdNumberPlan *numPlan_p,
                                Bcd *bcdData_p,
                                 Int8 *bcdDataLen_p);

Char *vgConvSparseBcdToText (BcdNumberType numType,
                              const Bcd *bcdData_p,
                               Int8 bcdDataLen,
                                Char *textDialNum_p );

Int16 vgPackBcdString (Bcd *outBcd_p,
                        const Bcd *inBcd_p,
                         const Int16 inBcdLen);

Int16 vgAsciiToInt16(const Int8 *hexString_p);

Int8 vgAsciiToInt8(const Char *hexString_p,
                    Boolean *error_p);

/* Register signals to be expected */

void sendSsRegistrationSignal (const VoyagerSubsystems_t ssCode,
                                const VgmuxChannelNumber entity,
                                 const SignalId signalId);

/* Sending information to CrManager */

void sendDataToCrm (const Char *string,
                     Boolean prefix,
                      Boolean postfix,
                       const VgmuxChannelNumber sendOnChannel);

void sendDataWithLengthToCrm (const Char* string_p,
                               Int16 length,
                                Boolean prefix,
                                 Boolean postfix,
                                  const VgmuxChannelNumber sendOnChannel);

void sendEchoDataToCrm (const Char *string,
                         Int16 length,
                           const VgmuxChannelNumber sendOnChannel);

void sendResultCodeToCrm (ResultCode_t resultCode,
                           const VgmuxChannelNumber sendOnChannel);

void sendIntermidateResultCodeToCrm (const Char *resultCodeText_p,
                                      const VgmuxChannelNumber sendOnChannel);

void sendUnsolicitedResultCodeToCrm (const VgmuxChannelNumber entity, ResultCode_t resultCode);

void sendLineDataToCrm (const ConnectionClass_t pendingConnection,
                         const VgmuxChannelNumber sendOnChannel);

void setResultCode (const VgmuxChannelNumber entity,
                     const ResultCode_t resultCode);

ResultCode_t getResultCode (const VgmuxChannelNumber entity);

void setCommandId (const VgmuxChannelNumber entity,
                    const CommandId_t commandId);

CommandId_t getCommandId (const VgmuxChannelNumber entity);

void setCommandName (const VgmuxChannelNumber entity,
                      const Char *commandName_p);

const Char *getCommandName (const VgmuxChannelNumber entity);

void setConnectionType (const VgmuxChannelNumber entity,
                         const ConnectionClass_t connectionClass);

ConnectionClass_t getConnectionType (const VgmuxChannelNumber entity);

void setTaskInitiated (const VgmuxChannelNumber entity,
                         const Boolean taskInitiated);

Boolean getTaskInitiated (const VgmuxChannelNumber entity);

CirmDataInd *getCirmDataBuffer( const VgmuxChannelNumber  entity);

void resetCirmDataBuffer ( const VgmuxChannelNumber  entity );

/* Miscellaneous */

void vgSendMSCStatusToMux (const Boolean dcdOn,
                            const Boolean riOn,
                             const Boolean dcdPinIsValid,
                              const Boolean riPinIsValid);

VgmuxChannelNumber findFirstEnabledChannel  (void);
VgmuxChannelNumber findFirstDisabledChannel (void);

void vgCiRunAtCommandInd (const VgmuxChannelNumber entity,
                           const Char *commandString);

void vgCiDataEntryModeInd (const VgmuxChannelNumber entity,
                            const VgDataEntryMode dataEntryMode);

void vgCiUserProfLoadedInd( const VgmuxChannelNumber entity );

Boolean vgIsSmDcsGsm (Int8 dcs);

void *getCrConversionBuffer   (const VgmuxChannelNumber entity);
void resetCrConversionBuffer  (const VgmuxChannelNumber entity);

Int8 *getCrOutputBuffer       (const VgmuxChannelNumber entity);
void resetCrOutputBuffer      (const VgmuxChannelNumber entity);

Int8 *getCrAlphaStringBuffer  (const VgmuxChannelNumber entity);
void resetCrAlphaStringBuffer (const VgmuxChannelNumber entity);

Int8 *getCrAlphaSearchBuffer  (const VgmuxChannelNumber entity);
void resetCrAlphaSearchBuffer (const VgmuxChannelNumber entity);

Boolean checkForAdditionalPassword (CommandLine_t *commandBuffer_p,
                                     Char *otherPassword,
                                      Int16 *otherPasswordLength);

/* Job 109119 - check passwords for alpha characters. */
Boolean checkForNumericOnlyChars(const Char *password);

Boolean vgCheckTextBcdString (const Char *asciiDialNum_p, Int16 asciiLen);
ResultCode_t sendVerifyPin2Request (const VgmuxChannelNumber  entity);

void sendTSmsInfo(ResultCode_t resultCode);

Int32 vgStriNCmp    (const Char *string1, const Char *string2, Int32 num);
Int32 vgStrWildNCmp (const Char *string1, const Char *string2, Int32 num);
const Ipr     *getIprString       (Int8     baudIndex);
const IpsType *getIpsTypeString   (Int8     typeIndex);

Int8 vgConvert2CharsIntoInt8 ( Int8* string, Int8 defaultValue);

void vgCharToBcd (Bcd *base, Int16 pos, Int8 val);
void vgTextToBcd( const Char *asciiNum_p, Int16 asciiLen, Int8 *bcdNum_p);

VgmuxChannelNumber vgGetMmiUnsolicitedChannel (void);

void writeHslString (const char *string);

void vgSetCirmDataIndForceTransmit(const VgmuxChannelNumber entity);

void vgSetCirmDataIndIsUrc(const VgmuxChannelNumber entity,
                         Boolean isUrc);

Boolean vgGetCirmDataIndIsUrc(const VgmuxChannelNumber entity);
#endif

/* END OF FILE */
