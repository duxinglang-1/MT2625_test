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
 * Supplementary Service Parser.  This allows supplementary service strings
 * as well as dial strings to be passed to the dial interface (i.e ATD).
 * These strings can be used to activate/deactivate/interogate Supplementary
 * Services, access SIM functionality, send USSD commands over the network,
 * run user defined SS commands or just make or control call handling.
 *
 * This parser is implemented based around the specifications defined in
 * GSM 02.30.
 **************************************************************************/

#define MODULE_NAME "RVSSPARS"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvchman.h>
#include <rvcrhand.h>
#include <rvsspars.h>
#include <stdlib.h>
#include <rvccdata.h>
#include <rvgput.h>
#include <gkimem.h>

 /***************************************************************************
 * Type Definitions
 ***************************************************************************/

typedef enum VgSsSiTypeTag
{
  SI_TYPE_MIN = 0,
  SI_TYPE_NA = SI_TYPE_MIN, /* Not applicable */
  SI_TYPE_DN,               /* Directory Number */
  SI_TYPE_PW,               /* Password */
  SI_TYPE_BS,               /* Basic Service Group */
  SI_TYPE_T,                /* No reply condition timer */
  SI_TYPE_SC,               /* Service code */
  SI_TYPE_L2P,              /* GPRS L2P parameter */
  SI_TYPE_CID,              /* GPRS CID parameter */
  SI_TYPE_MAX
} VgSsSiType;

typedef enum VgSsActionTag
{
  SS_PARSE_ACTIVATE = 1,
  SS_PARSE_DEACTIVATE = 0,
  SS_PARSE_INTEROGATE = 2,
  SS_PARSE_REGISTER = 3,
  SS_PARSE_ERASE = 4,
  SS_PARSE_NO_ACTION
} VgSsAction;

typedef ResultCode_t (*VgSsHandler)(const VgSsAction          vgSsAction,
                                    const Char                *ssCommand_p,
                                    const Char                *ssServiceCode_p,
                                    const Char                *siaData_p,
                                    const Char                *sibData_p,
                                    const Char                *sicData_p,
                                    const Char                *sixData_p,
                                    const VgmuxChannelNumber  entity);

typedef struct VgSsParserTableTag
{
  const Char        *ssServiceCode_p;
  const Char        *ssCommand_p;
  const VgSsSiType  sia;
  const VgSsSiType  sib;
  const VgSsSiType  sic;
  const VgSsSiType  six;
  const VgSsHandler handler;
} VgSsParserTable;

typedef struct VgSsActionStringTag
{
  const Char *ssActionString_p;
  VgSsAction vgSsAction;
} VgSsActionString;

typedef enum VgSsParserStateTag
{
  WAIT_STAR_HASH_CHAR,
  GOT_STAR_CHAR,
  PROCESS_DATA,
  TERMINATE
} VgSsParserState;

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static Boolean checkParseString(CommandLine_t *commandBuffer_p,
                                const VgmuxChannelNumber entity);

static void addParsedCommand( const VgmuxChannelNumber entity,
                              Char                     *fmt_p,
                              ...);

static ResultCode_t runParsedCommand( const VgmuxChannelNumber entity );

static Boolean checkDirectoryNumberParam(const Char *siData_p,
                                         VgDialNumberType *dialType);

static Boolean checkL2PParam(const Char *siData_p, Int32 *l2p);

static Boolean checkCIDParam(const VgmuxChannelNumber  entity, const Char *siData_p, Int32 *cid);

static ResultCode_t handleSsIMEIParse(const VgSsAction          vgSsAction,
                                      const Char                *ssCommand_p,
                                      const Char                *ssServiceCode_p,
                                      const Char                *siaData_p,
                                      const Char                *sibData_p,
                                      const Char                *sicData_p,
                                      const Char                *sixData_p,
                                      const VgmuxChannelNumber  entity);

static ResultCode_t handleSsPinChange(const VgSsAction          vgSsAction,
                                      const Char                *ssCommand_p,
                                      const Char                *ssServiceCode_p,
                                      const Char                *siaData_p,
                                      const Char                *sibData_p,
                                      const Char                *sicData_p,
                                      const Char                *sixData_p,
                                      const VgmuxChannelNumber  entity);

static ResultCode_t handleSsPinEntry( const VgSsAction          vgSsAction,
                                      const Char                *ssCommand_p,
                                      const Char                *ssServiceCode_p,
                                      const Char                *siaData_p,
                                      const Char                *sibData_p,
                                      const Char                *sicData_p,
                                      const Char                *sixData_p,
                                      const VgmuxChannelNumber  entity);

static ResultCode_t handleSsPwdChange(const VgSsAction          vgSsAction,
                                      const Char                *ssCommand_p,
                                      const Char                *ssServiceCode_p,
                                      const Char                *siaData_p,
                                      const Char                *sibData_p,
                                      const Char                *sicData_p,
                                      const Char                *sixData_p,
                                      const VgmuxChannelNumber  entity);

static ResultCode_t handleSsGPRS99Parse(const VgSsAction          vgSsAction,
                                      const Char                *ssCommand_p,
                                      const Char                *ssServiceCode_p,
                                      const Char                *siaData_p,
                                      const Char                *sibData_p,
                                      const Char                *sicData_p,
                                      const Char                *sixData_p,
                                      const VgmuxChannelNumber  entity);

static ResultCode_t handleSsGPRS98Parse(const VgSsAction          vgSsAction,
                                      const Char                *ssCommand_p,
                                      const Char                *ssServiceCode_p,
                                      const Char                *siaData_p,
                                      const Char                *sibData_p,
                                      const Char                *sicData_p,
                                      const Char                *sixData_p,
                                      const VgmuxChannelNumber  entity);

static VgSsAction parseForActionCode(CommandLine_t *commandBuffer_p);

static void parseForServiceCode(CommandLine_t            *commandBuffer_p,
                                Char                     *serviceCode,
                                const VgmuxChannelNumber entity);

static const VgSsParserTable *findScCodeInTable(Char                  *serviceCode_p,
                                                const VgSsParserTable *vgSsParserTable,
                                                Int8                  tableEntries);



static ResultCode_t executeTableCommand(CommandLine_t     *commandBuffer_p,
                                        const VgSsAction  vgSsAction,
                                        const VgSsParserTable   *vgSsParserTableEntry_p,
                                        const VgmuxChannelNumber entity);

static ResultCode_t handleParsedData( CommandLine_t             *commandBuffer_p,
                                      const VgSsAction          vgSsAction,
                                      Char                      *serviceCode_p,
                                      const VgmuxChannelNumber  entity);

static ResultCode_t parseSsCommandLine( CommandLine_t             *commandBuffer_p,
                                        const VgmuxChannelNumber  entity);


/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

static const VgSsParserTable vgSsParserSsCodes[] =
{
  /* SS operations.  These are all based around the SS primative
   * functions such as APEX_SIM_MEP_REQ and so on....
   */

  /* SC                 COMMAND                     SIA          SIB          SIC          SIXT        Handler */
  /* GSM 02.81 Services (Call Line Identification): */
  /* Not supported */

  /* GSM 02.82 Services (Call Forwarding): */
  /* Not supported */

  /* GSM 02.83 Services (Call Hold & Waiting): */
  /* Not supported */

  /* GSM 02.88 Services (Call Barring): */  
  /* Not supported */

  /* SS Password change: */
  { (const Char*)"03",  (const Char*)"+CPWD",        SI_TYPE_SC, SI_TYPE_PW,  SI_TYPE_PW,  SI_TYPE_PW, handleSsPwdChange },

  /* GPRS 07.07 Service: */
  { (const Char*)"99",  (const Char*)"*MGPRVSS",     SI_TYPE_DN, SI_TYPE_L2P, SI_TYPE_CID, SI_TYPE_NA, handleSsGPRS99Parse },

  /* GPRS 27.07 Service: */
  { (const Char*)"98",  (const Char*)"*MGPRVSS",     SI_TYPE_CID,SI_TYPE_NA,  SI_TYPE_NA,  SI_TYPE_NA, handleSsGPRS98Parse }
};

static const VgSsParserTable vgSsParserSimCodes[] =
{
  /* SIM parser operations.  These are general SIM access based around already
   * existing functionality....
   */

  /* PIN PUK 1/2 Change: */
  { (const Char*)"04",  (const Char*)"+CPWD=\"SC\"", SI_TYPE_PW, SI_TYPE_PW, SI_TYPE_PW, SI_TYPE_NA, handleSsPinChange },
  { (const Char*)"042", (const Char*)"+CPWD=\"P2\"", SI_TYPE_PW, SI_TYPE_PW, SI_TYPE_PW, SI_TYPE_NA, handleSsPinChange },
  { (const Char*)"05",  (const Char*)"+CPIN",        SI_TYPE_PW, SI_TYPE_PW, SI_TYPE_PW, SI_TYPE_NA, handleSsPinEntry  },
  { (const Char*)"052", (const Char*)"+CPIN",        SI_TYPE_PW, SI_TYPE_PW, SI_TYPE_PW, SI_TYPE_NA, handleSsPinEntry  },

  /* Show IMEI: */
  { (const Char*)"06",  (const Char*)"+CGSN",        SI_TYPE_NA, SI_TYPE_NA, SI_TYPE_NA, SI_TYPE_NA, handleSsIMEIParse }
};

static const VgSsActionString vgSsActionStrings[] =
{
  { (const Char*)"*#", SS_PARSE_INTEROGATE },
  { (const Char*)"**", SS_PARSE_REGISTER   },
  { (const Char*)"##", SS_PARSE_ERASE      },
  { (const Char*)"*",  SS_PARSE_ACTIVATE   },
  { (const Char*)"#",  SS_PARSE_DEACTIVATE }
};

#define MAX_SS_CODES_ENTRIES            (sizeof(vgSsParserSsCodes)/sizeof(VgSsParserTable))
#define MAX_SIM_CODES_ENTRIES           (sizeof(vgSsParserSimCodes)/sizeof(VgSsParserTable))
#define MAX_ACTION_STRINGS              (sizeof(vgSsActionStrings)/sizeof(VgSsActionString))

#if defined (ENABLE_LONG_AT_CMD_RSP)
#define SS_PARSER_SC_MAX  (AT_LARGE_BUFF_SIZE)
#define SS_PARSER_SIA_MAX (AT_LARGE_BUFF_SIZE)
#define SS_PARSER_SIB_MAX (AT_LARGE_BUFF_SIZE)
#define SS_PARSER_SIC_MAX (AT_LARGE_BUFF_SIZE)
#define SS_PARSER_SIX_MAX (AT_LARGE_BUFF_SIZE)
#else
#define SS_PARSER_SC_MAX  (COMMAND_LINE_SIZE)
#define SS_PARSER_SIA_MAX (COMMAND_LINE_SIZE)
#define SS_PARSER_SIB_MAX (COMMAND_LINE_SIZE)
#define SS_PARSER_SIC_MAX (COMMAND_LINE_SIZE)
#define SS_PARSER_SIX_MAX (COMMAND_LINE_SIZE)
#endif
#define SS_PARSER_MAX_SI_ARGS (4)

#if defined (ENABLE_LONG_AT_CMD_RSP)
static Char commandsToExecute[AT_LARGE_BUFF_SIZE];
#else
static Char commandsToExecute[COMMAND_LINE_SIZE];
#endif

static const Char validDigits[] =
{
  /* Need to have all letters allowed as part of a regular
   * dial string including SS characters such as parser
   * characters and CLIR & CCUG suppression/invocation
   * characters....
   */
  '+','0','1','2','3','4','5','6','7','8','9','*',
  '#','A','B','C','D',';','I','i','G','g','^',' ',',','P','p',
  '\0'          /* Must be terminated for use with MAX_VALID_SS_PARSE_DIGITS */
};

#define MAX_VALID_SS_PARSE_DIGITS (vgStrLen(validDigits))

#define VG_SS_NRT_DEFAULT       (20)
#define VG_SS_NRT_MIN_RANGE     (1)
#define VG_SS_NRT_MAX_RANGE     (30)

/***************************************************************************
 * Local Function
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:     checkParseString
 *
 * Parameters:   commandBuffer_p  - command line to check for valid chars
 *               entity           - mux channel number
 *
 * Returns:      Boolean          - TRUE if all characters in cmd line are
 *                                  valid, FALSE otherwise.
 *
 * Description:  Checks all characters in command line for SS parsing are
 *               valid.
 *-------------------------------------------------------------------------*/
static Boolean checkParseString(CommandLine_t *commandBuffer_p,
                                const VgmuxChannelNumber entity)
{
  Int16                   i;
  Int8                    profileValue             = getProfileValue (entity, PROF_S3);
  Boolean                 stringOkay               = TRUE;
  SupplementaryContext_t  *supplementaryContext_p  = ptrToSupplementaryContext  (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(supplementaryContext_p != PNULL, entity, 0, 0);
#endif
  for ( i = commandBuffer_p->position;
        (i < commandBuffer_p->length) &&
        (stringOkay == TRUE) &&
        (commandBuffer_p->character[i] != profileValue);
        i++)
  {
    /* Check that each character appears in valid digits array.... */
    if (vgStrChr (validDigits, commandBuffer_p->character[i]) == PNULL)
    {
      stringOkay = FALSE;
    }
  }

  /* job109084: only add valid SS String for FDN/CCbySIM check */
  if (stringOkay == TRUE)
  {
    supplementaryContext_p->ssParams.ssStringLength = i - commandBuffer_p->position;
    /* truncate length if too long */
    if (supplementaryContext_p->ssParams.ssStringLength > MAX_ADDR_LEN)
    {
      supplementaryContext_p->ssParams.ssStringLength = MAX_ADDR_LEN;
    }
    /* copy SS from command buffer to context data */
    memcpy (supplementaryContext_p->ssParams.ssString,
            &commandBuffer_p->character[commandBuffer_p->position],
            supplementaryContext_p->ssParams.ssStringLength);
  }

  return (stringOkay);
}

/*--------------------------------------------------------------------------
 *
 * Function:     addParsedCommand
 *
 * Parameters:   entity             - mux channel number
 *               fmt_p              - format style spec (sprintf)
 *               ...                - arguments to commands.
 *
 * Returns:      ResultCode_t       - status of operation.
 *
 * Description:  Adds command to list of commands to execute.
 *
 *-------------------------------------------------------------------------*/
static void addParsedCommand( const VgmuxChannelNumber entity,
                                      Char                     *fmt_p,
                                      ...)
{
  Char               *tempBuffer = PNULL;
  Int16               numChars = 0;
  va_list             marker;
  Int16               executeLen = (Int16)vgStrLen(commandsToExecute);

  PARAMETER_NOT_USED(entity);

#if defined (ENABLE_LONG_AT_CMD_RSP)
  KiAllocZeroMemory(    sizeof(Char) * (AT_LARGE_BUFF_SIZE),
                        (void **)&tempBuffer);
#else
  KiAllocZeroMemory(    sizeof(Char) * (COMMAND_LINE_SIZE),
                        (void **)&tempBuffer);
#endif

  /* Format data.... */
  va_start( marker, fmt_p);

#if defined (__arm) && !defined(__GNUC__)
  numChars = _vsprintf((char*)tempBuffer, (char*)fmt_p, marker);
#else
#if defined (ENABLE_LONG_AT_CMD_RSP)
  numChars = (Int16)vsnprintf((char*)tempBuffer, AT_LARGE_BUFF_SIZE,(char*)fmt_p, marker);
#else
  numChars = (Int16)vsnprintf((char*)tempBuffer, COMMAND_LINE_SIZE,(char*)fmt_p, marker);
#endif
#endif

  va_end( marker );

#if !defined(DEVELOPMENT_VERSION)
  numChars = numChars;
#endif
#if defined (ENABLE_LONG_AT_CMD_RSP)
  FatalAssert(numChars <= AT_LARGE_BUFF_SIZE);
#else
  FatalAssert(numChars <= COMMAND_LINE_SIZE);
#endif

  /* Concatenate this string on to the commands to run string.... */
  commandsToExecute[executeLen] = ';';
  executeLen++;
#if defined (ENABLE_LONG_AT_CMD_RSP)
  vgStrNCpy(&commandsToExecute[executeLen], tempBuffer,
            AT_LARGE_BUFF_SIZE - executeLen);
#else
  vgStrNCpy(&commandsToExecute[executeLen], tempBuffer,
            COMMAND_LINE_SIZE - executeLen);
#endif

  KiFreeMemory( (void **)&tempBuffer);
}

/*--------------------------------------------------------------------------
 *
 * Function:     runParsedCommand
 *
 * Parameters:   entity             - mux channel number
 *
 * Returns:      ResultCode_t       - status of operation.
 *
 * Description:  Runs any commands found during parse list.
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t runParsedCommand( const VgmuxChannelNumber entity )
{
  ResultCode_t        result = RESULT_CODE_OK;
  ScanParseContext_t  *scanParseContext_p = ptrToScanParseContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(scanParseContext_p != PNULL, entity, 0, 0);
#endif
  if (vgStrLen(commandsToExecute) > 0)
  {

#if defined(ATCI_ENABLE_DYN_AT_BUFF)
    /* Allocate buffer if not already done so */
    if (scanParseContext_p->nextCommand.character == PNULL)
    {
      vgAllocAtCmdBuffer(entity);
    }
#endif         
    
    /* We concatenate any commands we want to run onto the end of this
     * one such that when the command completes we execute the next....
     */
#if defined (ENABLE_LONG_AT_CMD_RSP)
    vgStrNCpy (&scanParseContext_p->nextCommand.character[scanParseContext_p->nextCommand.position],
               commandsToExecute,
               (AT_LARGE_BUFF_SIZE - scanParseContext_p->nextCommand.position));
#else
    vgStrNCpy (&scanParseContext_p->nextCommand.character[scanParseContext_p->nextCommand.position],
               commandsToExecute,
               (COMMAND_LINE_SIZE - scanParseContext_p->nextCommand.position));
#endif
    scanParseContext_p->nextCommand.length += vgStrLen(commandsToExecute);
    scanParseContext_p->nextCommand.character[scanParseContext_p->nextCommand.length-1] = getProfileValue (entity, PROF_S3);
    scanParseContext_p->nextCommand.character[scanParseContext_p->nextCommand.length] = NULL_CHAR;
  }

  else
  {
    result = VG_CME_INVALID_DIALSTRING_CHARS;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:     checkDirectoryNumberParam
 *
 * Parameters:   siData_p    - Parameter data
 *
 *
 * Returns:      Boolean     - TRUE if valid dialnum, FALSE otherwise
 *
 * Description:  Checks that a dial number is valid (i.e. non-zero length,
 *               doesn't contain illegal characters).  Also returns
 *               type of dial number.
 *-------------------------------------------------------------------------*/
static Boolean checkDirectoryNumberParam(const Char *siData_p,
                                         VgDialNumberType *dialType)
{
  Boolean valid = TRUE;
  Int8    i, dialLen = (Int8)vgStrLen(siData_p);

  /* Check length of dial string.... */
  if (dialLen > 0)
  {
    if (siData_p[0] == INTERNATIONAL_PREFIX)
    {
      *dialType = VG_DIAL_NUMBER_INTERNATIONAL;
    }

    else
    {
      *dialType = VG_DIAL_NUMBER_UNKNOWN;
    }

    /* Length okay.... */
    for (i = 0; (i < dialLen) && (valid == TRUE); i++)
    {
      if (vgStrChr (validDigits, siData_p[i]) == PNULL)
      {
        valid = FALSE;
      }
    }
  }

  else
  {
    /* Invalid length.... */
    valid = FALSE;
  }

  return (valid);
}

/*--------------------------------------------------------------------------
 *
 * Function:     checkL2PParam
 *
 * Parameters:   siData_p    - Parameter data
 *
 * Returns:      Boolean - TRUE if value OK, FALSE otherwise.
 *
 * Description:  Checks that the L2P parameter is valid and in the correct
 *               range.
 *-------------------------------------------------------------------------*/
static Boolean checkL2PParam(const Char *siData_p, Int32 *l2p)
{
  Boolean result = TRUE;

  *l2p = atoi((const char *)siData_p);

  if (
#if defined (FEA_PPP)    
      *l2p != VG_SS_L2P_PPP_PROTOCOL &&
      *l2p != VG_SS_L2P_CORE_PPP_PROTOCOL &&
#endif /* FEA_PPP */      
      *l2p != VG_SS_L2P_PACKET_TRANSPORT_PROTOCOL
      )
  {
    result = FALSE;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:     checkCIDParam
 *
 * Parameters:   siData_p    - Parameter data
 *
 * Returns:      Boolean - TRUE if value OK, FALSE otherwise.
 *
 * Description:  Checks that the CID parameter is valid and in the correct
 *               range.
 *-------------------------------------------------------------------------*/
static Boolean checkCIDParam(const VgmuxChannelNumber  entity, const Char *siData_p, Int32 *cid)
{
  Boolean result = FALSE;

  *cid = atoi((const char *)siData_p);

  /* job132261: correct upper CID range value */
  if ((*cid >= vgGpGetMinCidValue(entity) ) && (*cid < MAX_NUMBER_OF_CIDS))
  {
    result = TRUE;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:     handleSsIMEIParse
 *
 * Parameters:   vgSsAction         - action for the SS operation
 *               vgSsParserTable_p  - table entry for this parser code
 *               si(a/b/c/x)Data    - Parameter data
 *               entity             - mux channel number
 *
 * Returns:      ResultCode_t       - status of operation.
 *
 * Description:  Handles the SS actions for the display of the IMEI.
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t handleSsIMEIParse(const VgSsAction          vgSsAction,
                                      const Char                *ssCommand_p,
                                      const Char                *ssServiceCode_p,
                                      const Char                *siaData_p,
                                      const Char                *sibData_p,
                                      const Char                *sicData_p,
                                      const Char                *sixData_p,
                                      const VgmuxChannelNumber  entity)
{
  ResultCode_t result = RESULT_CODE_OK;

  PARAMETER_NOT_USED(ssServiceCode_p);
  PARAMETER_NOT_USED(siaData_p);
  PARAMETER_NOT_USED(sibData_p);
  PARAMETER_NOT_USED(sicData_p);
  PARAMETER_NOT_USED(sixData_p);

  switch (vgSsAction)
  {
    case SS_PARSE_INTEROGATE:
    {
      addParsedCommand(entity, (Char *)"%s", ssCommand_p);
      break;
    }

    case SS_PARSE_ACTIVATE:
    case SS_PARSE_DEACTIVATE:
    case SS_PARSE_REGISTER:
    case SS_PARSE_ERASE:
    case SS_PARSE_NO_ACTION:
    {
      result = VG_CME_OPERATION_NOT_ALLOWED;
      break;
    }

    default:
    {
      result = VG_CME_OPERATION_NOT_ALLOWED;
      /* Illegal action */
      FatalParam(entity, vgSsAction, 0);
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:     handleSsPinChange
 *
 * Parameters:   vgSsAction         - action for the SS operation
 *               vgSsParserTable_p  - table entry for this parser code
 *               si(a/b/c/x)Data    - Parameter data
 *               entity             - mux channel number
 *
 * Returns:      ResultCode_t       - status of operation.
 *
 * Description:  Handles the SS actions for PIN1/PIN2 change.
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t handleSsPinChange(const VgSsAction          vgSsAction,
                                      const Char                *ssCommand_p,
                                      const Char                *ssServiceCode_p,
                                      const Char                *siaData_p,
                                      const Char                *sibData_p,
                                      const Char                *sicData_p,
                                      const Char                *sixData_p,
                                      const VgmuxChannelNumber  entity)
{
  ResultCode_t result = RESULT_CODE_OK;

  PARAMETER_NOT_USED(ssServiceCode_p);
  PARAMETER_NOT_USED(sixData_p);

  switch (vgSsAction)
  {
    case SS_PARSE_REGISTER:
    {
      addParsedCommand( entity,
                        (Char *)"%s,\"%s\",\"%s\",\"%s\"",
                        ssCommand_p,
                        siaData_p,
                        sibData_p,
                        sicData_p);
      break;
    }

    case SS_PARSE_INTEROGATE:
    case SS_PARSE_ACTIVATE:
    case SS_PARSE_DEACTIVATE:
    case SS_PARSE_ERASE:
    case SS_PARSE_NO_ACTION:
    {
      result = VG_CME_OPERATION_NOT_ALLOWED;
      break;
    }

    default:
    {
      result = VG_CME_OPERATION_NOT_ALLOWED;
      /* Illegal action */
      FatalParam(entity, vgSsAction, 0);
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:     handleSsPinEntry
 *
 * Parameters:   vgSsAction         - action for the SS operation
 *               vgSsParserTable_p  - table entry for this parser code
 *               si(a/b/c/x)Data    - Parameter data
 *               entity             - mux channel number
 *
 * Returns:      ResultCode_t       - status of operation.
 *
 * Description:  Handles the SS actions for PIN1/PIN2/PUK1/PUK2 entry.
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t handleSsPinEntry( const VgSsAction          vgSsAction,
                                      const Char                *ssCommand_p,
                                      const Char                *ssServiceCode_p,
                                      const Char                *siaData_p,
                                      const Char                *sibData_p,
                                      const Char                *sicData_p,
                                      const Char                *sixData_p,
                                      const VgmuxChannelNumber  entity)
{
  ResultCode_t result = RESULT_CODE_OK;

  PARAMETER_NOT_USED(ssServiceCode_p);
  PARAMETER_NOT_USED(sixData_p);

  switch (vgSsAction)
  {
    case SS_PARSE_REGISTER:
    {
      /* If PUK2 entry, provide extra parameter to +CPIN request to differentiate between CHV1 and CHV2.... */
      if (vgStrCmp(ssServiceCode_p, (const Char*)"052") == 0)
      {
        addParsedCommand( entity,
                          (Char *)"%s=\"%s\",\"%s\",\"%s\",52",
                          ssCommand_p,
                          siaData_p,
                          sibData_p,
                          sicData_p);
      }
      else
      {
        addParsedCommand( entity,
                          (Char *)"%s=\"%s\",\"%s\",\"%s\"",
                          ssCommand_p,
                          siaData_p,
                          sibData_p,
                          sicData_p);
      }
      break;
    }

    case SS_PARSE_ACTIVATE:
    case SS_PARSE_DEACTIVATE:
    case SS_PARSE_INTEROGATE:
    case SS_PARSE_ERASE:
    case SS_PARSE_NO_ACTION:
    {
      result = VG_CME_OPERATION_NOT_ALLOWED;
      break;
    }

    default:
    {
      result = VG_CME_OPERATION_NOT_ALLOWED;
      /* Illegal action */
      FatalParam(entity, vgSsAction, 0);
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:     handleSsPwdChange
 *
 * Parameters:   vgSsAction         - action for the SS operation
 *               vgSsParserTable_p  - table entry for this parser code
 *               si(a/b/c/x)Data    - Parameter data
 *               entity             - mux channel number
 *
 * Returns:      ResultCode_t       - status of operation.
 *
 * Description:  Handles the SS actions for SS password change.
 *
 *-------------------------------------------------------------------------*/
static ResultCode_t handleSsPwdChange(const VgSsAction          vgSsAction,
                                      const Char                *ssCommand_p,
                                      const Char                *ssServiceCode_p,
                                      const Char                *siaData_p,
                                      const Char                *sibData_p,
                                      const Char                *sicData_p,
                                      const Char                *sixData_p,
                                      const VgmuxChannelNumber  entity)
{
  ResultCode_t  result = VG_CME_OPERATION_NOT_ALLOWED;

  PARAMETER_NOT_USED(ssServiceCode_p);

  switch (vgSsAction)
  {
    case SS_PARSE_REGISTER:
      /* PWD change only supported for call related facilities
       * For NB-IOT we do not support those
       */       
    case SS_PARSE_INTEROGATE:
    case SS_PARSE_ACTIVATE:
    case SS_PARSE_DEACTIVATE:
    case SS_PARSE_ERASE:
    case SS_PARSE_NO_ACTION:
    {
      break;
    }

    default:
    {
      /* Illegal action */
      FatalParam(entity, vgSsAction, 0);
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:     handleSsGPRS99Parse
 *
 * Parameters:   vgSsAction         - action for the SS operation
 *               vgSsParserTable_p  - table entry for this parser code
 *               si(a/b/c/x)Data    - Parameter data
 *               entity             - mux channel number
 *
 * Returns:      ResultCode_t       - status of operation.
 *
 * Description:  Handles the SS actions to support 27.007 command -
 *               D*<GPRS_SC>[*[<called_address>][*[<L2P>][*[<cid>[,<cid>[,бн]]]]]]#
 *               for requesting GPRS service.
 *-------------------------------------------------------------------------*/
static ResultCode_t handleSsGPRS99Parse(const VgSsAction          vgSsAction,
                                      const Char                *ssCommand_p,
                                      const Char                *ssServiceCode_p,
                                      const Char                *siaData_p,
                                      const Char                *sibData_p,
                                      const Char                *sicData_p,
                                      const Char                *sixData_p,
                                      const VgmuxChannelNumber  entity)
{
  ResultCode_t      result = RESULT_CODE_OK;
  VgDialNumberType  dialNumType;
  Int32             l2pValue, cidValue;

  PARAMETER_NOT_USED(sixData_p);

  switch (vgSsAction)
  {
    case SS_PARSE_ACTIVATE:
    {
      /* Check arguments for optional GPRS set-up data.... */
      if (vgStrLen(siaData_p) > 0)
      {
        /* Provided a directory number - check it is valid.... */
        if (checkDirectoryNumberParam(siaData_p, &dialNumType) == FALSE)
        {
          result = VG_CME_INVALID_DIALSTRING_CHARS;
        }
      }

      if (vgStrLen(sibData_p) > 0)
      {
        /* Provided the L2P parameter - check it is valid.... */
        if (checkL2PParam(sibData_p, &l2pValue) == FALSE)
        {
          result = VG_CME_OPERATION_NOT_SUPPORTED;
        }
      }

      if (vgStrLen(sicData_p) > 0)
      {
        /* Provided the CID parameter - check it is valid.... */
        if (checkCIDParam(entity, sicData_p, &cidValue) == FALSE)
        {
          result = VG_CME_INVALID_CID_VALUE;
        }
      }

      /* If parameters are okay, run command.... */
      if (result == RESULT_CODE_OK)
      {
        /* Note all PSD dial strings require a preceeding # character.... */
        addParsedCommand( entity,
                          (Char *)"%s=\"#%s\",%s,%s,\"%s\"",
                          ssCommand_p,
                          siaData_p,
                          sibData_p,
                          sicData_p,
                          ssServiceCode_p);
      }
      break;
    }

    case SS_PARSE_INTEROGATE:
    case SS_PARSE_DEACTIVATE:
    case SS_PARSE_REGISTER:
    case SS_PARSE_ERASE:
    case SS_PARSE_NO_ACTION:
    {
      result = VG_CME_OPERATION_NOT_ALLOWED;
      break;
    }

    default:
    {
      /* Illegal action */
      FatalParam(entity, vgSsAction, 0);
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:     handleSsGPRS98Parse
 *
 * Parameters:   vgSsAction         - action for the SS operation
 *               vgSsParserTable_p  - table entry for this parser code
 *               si(a/b/c/x)Data    - Parameter data
 *               entity             - mux channel number
 *
 * Returns:      ResultCode_t       - status of operation.
 *
 * Description:  Handles the SS actions to support 27.007 command -
 *               D*<GPRS_SC_IP>[*<cid>[,<cid>[,бн]]]#
 *               for requesting GPRS service.
 *-------------------------------------------------------------------------*/
static ResultCode_t handleSsGPRS98Parse(const VgSsAction          vgSsAction,
                                      const Char                *ssCommand_p,
                                      const Char                *ssServiceCode_p,
                                      const Char                *siaData_p,
                                      const Char                *sibData_p,
                                      const Char                *sicData_p,
                                      const Char                *sixData_p,
                                      const VgmuxChannelNumber  entity)
{
  ResultCode_t      result = RESULT_CODE_OK;
  Int32             cidValue;

  PARAMETER_NOT_USED(sibData_p);
  PARAMETER_NOT_USED(sicData_p);
  PARAMETER_NOT_USED(sixData_p);

  switch (vgSsAction)
  {
    case SS_PARSE_ACTIVATE:
    {
      if (vgStrLen(siaData_p) > 0)
      {
        /* Provided the CID parameter - check it is valid.... */
        if (checkCIDParam(entity, siaData_p, &cidValue) == FALSE)
        {
          result = VG_CME_INVALID_CID_VALUE;
        }
      }

      /* If parameters are okay, run command.... */
      if (result == RESULT_CODE_OK)
      {
        /* Note all PSD dial strings require a preceeding # character.... */
        addParsedCommand( entity,
                          (Char *)"%s=\"#\",1,%s,\"%s\"",
                          ssCommand_p,
                          siaData_p,
                          ssServiceCode_p);
      }
      break;
    }

    case SS_PARSE_INTEROGATE:
    case SS_PARSE_DEACTIVATE:
    case SS_PARSE_REGISTER:
    case SS_PARSE_ERASE:
    case SS_PARSE_NO_ACTION:
    {
      result = VG_CME_OPERATION_NOT_ALLOWED;
      break;
    }

    default:
    {
      /* Illegal action */
      FatalParam(entity, vgSsAction, 0);
      break;
    }
  }

  return (result);
}
/*--------------------------------------------------------------------------
 *
 * Function:     parseForActionCode
 *
 * Parameters:   commandBuffer_p    - buffer to parse for action code
 *
 * Returns:      VgSsAction         - action code for this string
 *
 * Description:  Parses the supplied string for the action code.
 *
 *-------------------------------------------------------------------------*/
static VgSsAction parseForActionCode(CommandLine_t *commandBuffer_p)
{
  VgSsAction  vgSsAction = SS_PARSE_NO_ACTION;
  Int8        index;
  Boolean     found = FALSE;

  /* Strip out any whitespace.... */
  while ( (commandBuffer_p->character[commandBuffer_p->position] == SPACE_CHAR) &&
          (commandBuffer_p->position < commandBuffer_p->length))
  {
    commandBuffer_p->position++;
  }

  if (commandBuffer_p->position < commandBuffer_p->length)
  {
    for (index = 0; (index < MAX_ACTION_STRINGS) && (found == FALSE); index++)
    {
      if (vgStrNCmp(&commandBuffer_p->character[commandBuffer_p->position],
                    vgSsActionStrings[index].ssActionString_p,
                    vgStrLen(vgSsActionStrings[index].ssActionString_p)) == 0)
      {
        found = TRUE;
        vgSsAction = vgSsActionStrings[index].vgSsAction;
        commandBuffer_p->position += vgStrLen(vgSsActionStrings[index].ssActionString_p);
      }
    }
  }

  return (vgSsAction);
}

/*--------------------------------------------------------------------------
 *
 * Function:     parseForServiceCode
 *
 * Parameters:   commandBuffer_p    - buffer to parse for service code
 *               serviceCode        - pointer to string array in to which to put
 *                                    the service code string
 *               entity             - AT channel
 *
 * Returns:      VgSsParserTable    - action code for this string
 *
 * Description:  Parses the supplied string for the service code.  If found
 *               returns a pointer to the service string.
 *
 *-------------------------------------------------------------------------*/
static void parseForServiceCode (CommandLine_t *commandBuffer_p,
                                 Char          *serviceCode,
                                 const VgmuxChannelNumber entity)
{
  Int32           index = 0;

  memset(serviceCode, NULL_CHAR, SS_PARSER_SC_MAX);

  /* Read Service Code (SC) from command line.... */
  while ( (commandBuffer_p->position < commandBuffer_p->length) &&
          (commandBuffer_p->character[commandBuffer_p->position] != STAR_CHAR) &&
          (commandBuffer_p->character[commandBuffer_p->position] != HASH_CHAR) &&
          (commandBuffer_p->character[commandBuffer_p->position] != getProfileValue (entity, PROF_S3)) &&
          (commandBuffer_p->character[commandBuffer_p->position] != getProfileValue (entity, PROF_S4)) &&
          (index < SS_PARSER_SC_MAX) )
  {
    serviceCode[index] = commandBuffer_p->character[commandBuffer_p->position];
    commandBuffer_p->position++;
    index++;
  }

  /* Null terminate the buffer.... */
  serviceCode[index] = NULL_CHAR;
}

/*--------------------------------------------------------------------------
 *
 * Function:     findScCodeInTables
 *
 * Parameters:   serviceCode_p      - the service code to search for
 *               vgSsParserTable    - command table to search for SC
 *               tableEntries       - number of entries in table
 *
 * Returns:      VgSsParserTable    - pointer to table entry or PNULL
 *
 * Description:  Searches a table for a command returning a pointer to
 *               the table entry or PNULL if not found.
 *-------------------------------------------------------------------------*/
static const VgSsParserTable *findScCodeInTable(Char                  *serviceCode_p,
                                                const VgSsParserTable *vgSsParserTable_p,
                                                Int8                  tableEntries)
{
  const VgSsParserTable *vgSsParserTableEntry_p = PNULL;
  Int8                  index;

  for (index = 0; (index < tableEntries) && (vgSsParserTableEntry_p == PNULL); index++)
  {
    /* Search for SC.... */
    if (vgStrCmp(serviceCode_p, vgSsParserTable_p[index].ssServiceCode_p) == 0)
    {
      vgSsParserTableEntry_p = &vgSsParserTable_p[index];
    }
  }

  return (vgSsParserTableEntry_p);
}

/*--------------------------------------------------------------------------
 *
 * Function:     parseSIArguments
 *
 * Parameters:   commandBuffer_p        - command line to parse
 *               vgSsParserTableEntry_p - the table entry which describes the
 *                                        command running.
 *               sia                    - argument SIA
 *               sib                    - argument SIB
 *               sic                    - argument SIC
 *               six                    - argument SIX
 *               entity                 - entity to run command from
 *
 * Returns:      Boolean                - TRUE if valid, FALSE otherwise
 *
 * Description:  Parses the arguments for a service code.  Arguments are
 *               seperated by the '*' character.  Strings are ended by the
 *               '#' character.
 *-------------------------------------------------------------------------*/
static Boolean parseSIArguments(CommandLine_t            *commandBuffer_p,
                                const VgSsParserTable    *vgSsParserTableEntry_p,
                                Char                     *sia,
                                Char                     *sib,
                                Char                     *sic,
                                Char                     *six,
                                const VgmuxChannelNumber entity)
{
  Boolean         result,
                  parseComplete = FALSE;
  VgSsParserState parserState = WAIT_STAR_HASH_CHAR;
  Char            *argumentBuffer_p[SS_PARSER_MAX_SI_ARGS];
  SignedInt8      argumentBufferIndex = -1;
  Int32           siIndex = 0;

  PARAMETER_NOT_USED(vgSsParserTableEntry_p);

  argumentBuffer_p[0] = sia;
  argumentBuffer_p[1] = sib;
  argumentBuffer_p[2] = sic;
  argumentBuffer_p[3] = six;

  while ( (commandBuffer_p->position < commandBuffer_p->length) &&
          (commandBuffer_p->character[commandBuffer_p->position] != getProfileValue (entity, PROF_S3)) &&
          (commandBuffer_p->character[commandBuffer_p->position] != getProfileValue (entity, PROF_S4)) &&
          (parseComplete == FALSE) &&
          (siIndex < SS_PARSER_SIA_MAX) )
  {
    switch (parserState)
    {
      case WAIT_STAR_HASH_CHAR:
      {
        /* Run through the command buffer until we find a '*' or a '#'
         * character....
         */
        if (commandBuffer_p->character[commandBuffer_p->position] == HASH_CHAR)
        {
          /* Hash character marks end of SS string so terminate.... */
          parserState = TERMINATE;
        }

        else if (commandBuffer_p->character[commandBuffer_p->position] == STAR_CHAR)
        {
          /* Star delimates arguments so get parsing.... */
          parserState = GOT_STAR_CHAR;
        }

        commandBuffer_p->position++;
        break;
      }

      case GOT_STAR_CHAR:
      {
        /* Have a '*' character - this delimates the arguments.  Set up
         * buffer for argument storage and then commence reading data
         * into appropriate store....
         */
        argumentBufferIndex++;

        if (argumentBufferIndex < SS_PARSER_MAX_SI_ARGS)
        {
          siIndex = 0;
          parserState = PROCESS_DATA;
        }

        else
        {
          /* Too many parameters - terminate parsing.... */
          parseComplete = TRUE;
        }
        break;
      }

      case PROCESS_DATA:
      {
        if (commandBuffer_p->character[commandBuffer_p->position] == HASH_CHAR)
        {
          /* Hash character marks end of SS string so terminate.... */
          parserState = TERMINATE;
        }

        else if ((commandBuffer_p->character[commandBuffer_p->position] == STAR_CHAR) &&
                 (argumentBufferIndex >=0))
        {
          /* Star delimates arguments so get parsing.... */
          parserState = GOT_STAR_CHAR;
          argumentBuffer_p[argumentBufferIndex][siIndex] = NULL_CHAR;
        }

        else
        {
          if (argumentBufferIndex < 0)
          {
            /* Negative sign buffer index */
            FatalParam(entity, 0, 0);
//            argumentBufferIndex = 0;
          }

          argumentBuffer_p[argumentBufferIndex][siIndex] =
            commandBuffer_p->character[commandBuffer_p->position];
        }

        commandBuffer_p->position++;
        siIndex++;
        break;
      }

      case TERMINATE:
      {
        parseComplete = TRUE;
        break;
      }

      default:
      {
        /* Illegal SS parser state */
        FatalParam(entity, parserState, 0);
//        parseComplete = TRUE;
        break;
      }
    }
  }

  /* Command and arguments parse complete.  Check that last state was
   * TERMINATE otherwise parse failed....
   */
  if (parserState == TERMINATE)
  {
    result = TRUE;
  }

  else
  {
    result = FALSE;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:     executeTableCommand
 *
 * Parameters:   commandBuffer_p        - command line to parse
 *               vgSsAction             - action (as specified on command line)
 *               vgSsParserTableEntry_p - the table entry which describes the
 *                                        command running.
 *               entity                 - entity to run command from
 *
 * Returns:      ResultCode_t           - status of operation.
 *
 * Description:  Executes one of the SC specified in the tables.
 *-------------------------------------------------------------------------*/
static ResultCode_t executeTableCommand(CommandLine_t             *commandBuffer_p,
                                        const VgSsAction          vgSsAction,
                                        const VgSsParserTable     *vgSsParserTableEntry_p,
                                        const VgmuxChannelNumber  entity)
{
  ResultCode_t  result = VG_CME_OPERATION_NOT_ALLOWED;
  Char         *sia = PNULL,
               *sib = PNULL,
               *sic = PNULL,
               *six = PNULL;

  KiAllocZeroMemory(    sizeof(Char) * ( SS_PARSER_SIA_MAX),
                        (void **)&sia);
  KiAllocZeroMemory(    sizeof(Char) * ( SS_PARSER_SIB_MAX),
                        (void **)&sib);
  KiAllocZeroMemory(    sizeof(Char) * ( SS_PARSER_SIC_MAX),
                        (void **)&sic);
  KiAllocZeroMemory(    sizeof(Char) * ( SS_PARSER_SIX_MAX),
                        (void **)&six);

  /* Initialise arguments.... */
  memset(sia, NULL_CHAR, SS_PARSER_SIA_MAX);
  memset(sib, NULL_CHAR, SS_PARSER_SIA_MAX);
  memset(sic, NULL_CHAR, SS_PARSER_SIA_MAX);
  memset(six, NULL_CHAR, SS_PARSER_SIA_MAX);

  /* Parse arguments.  If the command has no action, ie. no proceding
   * '*' or '#' character just execute the command....
   */
  switch (vgSsAction)
  {
    case SS_PARSE_INTEROGATE:
    case SS_PARSE_ACTIVATE:
    case SS_PARSE_DEACTIVATE:
    case SS_PARSE_REGISTER:
    case SS_PARSE_ERASE:
    {
      /* Parse command buffer for arguments.... */
      if (parseSIArguments( commandBuffer_p,
                            vgSsParserTableEntry_p,
                            sia,
                            sib,
                            sic,
                            six,
                            entity) == TRUE)
      {
        /* Arguments okay, run command.... */
        result = vgSsParserTableEntry_p->handler( vgSsAction,
                                                  vgSsParserTableEntry_p->ssCommand_p,
                                                  vgSsParserTableEntry_p->ssServiceCode_p,
                                                  sia,
                                                  sib,
                                                  sic,
                                                  six,
                                                  entity);
      }

      else
      {
        result = VG_CME_OPERATION_NOT_ALLOWED;
      }
      break;
    }

    default:
    {
      result = VG_CME_OPERATION_NOT_ALLOWED;
      /* Illegal action */
      FatalParam(entity, vgSsAction, 0);
      break;
    }
  }

  KiFreeMemory( (void **)&sia);
  KiFreeMemory( (void **)&sib);
  KiFreeMemory( (void **)&sic);
  KiFreeMemory( (void **)&six);

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:     handleParsedData
 *
 * Parameters:   commandBuffer_p    - command line to parse
 *               vgSsAction         - action (as specified on command line)
 *               serviceCode_p      - the service code to execute
 *               entity             - entity to run command from
 *
 * Returns:      ResultCode_t       - status of operation.
 *
 * Description:  Deduces how to handle the provided service string and
 *               then, if found and valid command, executes it.
 *-------------------------------------------------------------------------*/
static ResultCode_t handleParsedData( CommandLine_t     *commandBuffer_p,
                                      const VgSsAction  vgSsAction,
                                      Char              *serviceCode_p,
                                      const VgmuxChannelNumber entity)
{
  const VgSsParserTable *vgSsParserTableEntry_p = PNULL;
  ResultCode_t          result = RESULT_CODE_ERROR;



  /* Is string a standard GSM SS code.... */
  vgSsParserTableEntry_p = findScCodeInTable( serviceCode_p,
                                              vgSsParserSsCodes,
                                              MAX_SS_CODES_ENTRIES);

  if (vgSsParserTableEntry_p == PNULL)
  {
    /* Not SS, check SIM.... */
    vgSsParserTableEntry_p = findScCodeInTable( serviceCode_p,
                                                vgSsParserSimCodes,
                                                MAX_SIM_CODES_ENTRIES);
  }

  /* If we've found the command then execute, otherwise, we have some more
   * checks to do....
   */
  if (vgSsParserTableEntry_p != PNULL)
  {
    /* Command is SS/SIM/Manufacturer specific.  Execute command.... */
    result = executeTableCommand( commandBuffer_p,
                                  vgSsAction,
                                  vgSsParserTableEntry_p,
                                  entity);
  }

  /* For NB-IOT we do not handle any USSD strings or dial numbers - just ATD*99#, ATD*98# and
    * SS strings for SIM lock */
  
  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:     parseSsCommandLine
 *
 * Parameters:   commandBuffer_p    - command line to parse
 *               entity             - entity to run command from
 *
 * Returns:      ResultCode_t       - status of operation.
 *
 * Description:  Parses a command line for SS strings.  When string (and
 *               type) have been deduced they are executed in the appropriate
 *               manner.
 *-------------------------------------------------------------------------*/
static ResultCode_t parseSsCommandLine( CommandLine_t *commandBuffer_p,
                                        const VgmuxChannelNumber entity)
{
  ResultCode_t    result = RESULT_CODE_ERROR;
  VgSsAction      vgSsAction;
  Char           *serviceCode= PNULL;

  KiAllocZeroMemory(    sizeof(Char) * (SS_PARSER_SC_MAX + 1),
                        (void **)&serviceCode);

  vgSsAction = parseForActionCode(commandBuffer_p);
  parseForServiceCode(commandBuffer_p, serviceCode, entity);
  result = handleParsedData(commandBuffer_p, vgSsAction, serviceCode, entity);

  KiFreeMemory( (void **)&serviceCode);

  return (result);
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

ResultCode_t vgParseSsString( CommandLine_t *commandBuffer_p,
                              const VgmuxChannelNumber entity)
{
  ResultCode_t            result = RESULT_CODE_OK;
  SupplementaryContext_t  *supplementaryContext_p = ptrToSupplementaryContext  (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(supplementaryContext_p != PNULL, entity, 0, 0);
#endif
  /* Reset variables.... */
#if defined (ENABLE_LONG_AT_CMD_RSP)
  memset(commandsToExecute, NULL_CHAR, AT_LARGE_BUFF_SIZE);
#else
  memset(commandsToExecute, NULL_CHAR, COMMAND_LINE_SIZE);
#endif

  /* Pre-parse command line to ensure that it doesn't contain any illegal
   * characters....
   */
  if (checkParseString(commandBuffer_p, entity) == TRUE)
  {
    /* Parse command line for SS data.  If found, parse string and run parser
     * as required....
     */
    while ( (commandBuffer_p->position < commandBuffer_p->length) &&
            (commandBuffer_p->character[commandBuffer_p->position] != getProfileValue (entity, PROF_S3)) &&
            (commandBuffer_p->character[commandBuffer_p->position] != getProfileValue (entity, PROF_S4)) &&
            (result == RESULT_CODE_OK))
    {
      result = parseSsCommandLine(commandBuffer_p, entity);
    }

    /* All of the parsing is now done, execute the commands we have to run
     * following command parsing....
     */
    if (result == RESULT_CODE_OK)
    {
      /* job109084: we need to indicate the parsed command to run is an SS string originating from
       * an ATD command to allow the FDN/CC by SIM code to run correctly. */
      supplementaryContext_p->ssParams.ssOpFromATD = TRUE;

      result = runParsedCommand(entity);
    }
    else
    {
      supplementaryContext_p->ssParams.ssOpFromATD = FALSE;
    }
  }

  else
  {
    result = VG_CME_INVALID_DIALSTRING_CHARS;
  }

  return (result);
}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

