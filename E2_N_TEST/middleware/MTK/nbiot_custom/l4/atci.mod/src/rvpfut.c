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
 * Procedures for Profile AT command execution
 * Handler for AT commands located in vgpfss.c
 *
 * Implemented commands:
 *
 * AT&C      - sets Data Carrier Connect (DCD) function mode
 * AT&D      - sets Data Terminal Ready (DTR) function mode
 * AT&K      - sets the flow control method
 * AT&F      - sets the current configuration to the factory defaults
 * AT&V      - displays the current configuration
 * AT&W      - stores the current configuration in non-volitile memory
 * AT+CGAUTO - like ATS0, for PDP contexts
 * AT+CGEREP  - configures notification of mobile and network initiated context
 *              deactivations
 * AT+CMEE   - configures error reporting control
 * AT+CMGF   - select SMS message format
 * AT+CNMI   - configures new SMS message indications
 * AT+CR     - configures service reporting control
 * AT+CRES   - restores SMS settings
 * AT+CSAS   - saves the current SMS settings
 * AT+CSCS   - sets the character set
 * AT+CSDH   - show SMS text mode parameter
 * AT+CSMP   - sets the SMS text mode parameters
 * AT+CSMS   - selects the message service
 * AT+DR     - configures data compression reporting control
 * AT+DS     - configures data compression control
 * AT+FCLASS - selects, reads or test fax service class
 * AT+ICF    - sets TE-TA control character framing
 * AT+IFC    - sets TE-TA local data flow control
 * AT+ILRR   - sets TE-TA local rate reporting mode
 * AT+IPR    - sets fixed local rate
 * ATE       - switches the command echo on or off
 * ATL       - sets speaker loudness
 * ATM       - sets speaker mode
 * ATQ       - sets result code presentation mode
 * ATS0      - sets number of rings before automatically answering the call
 * ATS1      - S1 is used as a ring counter
 * ATS10     - sets automatic disconnect delay in the absence of data carrier
 * ATS2      - sets escape character
 * ATS3      - sets command line termination character
 * ATS4      - sets response formatting character
 * ATS5      - sets command line editing character
 * ATS6      - sets pause before blind dialing
 * ATS7      - sets connection completion timeout
 * ATS8      - sets comma dial modifier timeout
 * ATV       - sets result code format mode
 * ATX       - sets connect result code format and call monitoring
 * ATZ       - gets the user defined profile from NVRAM
 **************************************************************************/

#define MODULE_NAME "RVPFUT"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <rvcimxsot.h>
#include <rvpfut.h>
#include <rvpfss.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvprof.h>
#include <rvnvram.h>
#include <rvoman.h>
#include <rvpfsigo.h>
#include <rvcimxut.h>
#include <rvomtime.h>
#include <rvcrhand.h>
#include <rvmssigo.h>
#include <smencdec.h>
#include <rvgput.h>
#include <rvmsut.h>
#include <rvcmux.h>
#include <rvutil.h>
#include <rvemdata.h>
#include <rvcrconv.h>
#include <rvcfg.h>
#include <simdec.h>
#include <rvchman.h>
#include <rvprof.h>

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

typedef struct PfCommandInfoTag
{
  const CommandId_t  commandId;   /* Command identifier */
  const ProfId       profId;      /* profId, e.g. PROF_ECHO */
  const Int8         numOfParams; /* number of parameters */
  const Int8         max;         /* max value of first parameter in simple cases */
} PfCommandInfo_t;

/* Profile command information table */

const PfCommandInfo_t pfAtCommandInfoTable[] =
{
  {VG_AT_PF_ECHO,      PROF_ECHO,      1, 1   },
  {VG_AT_PF_LOUDNESS,  PROF_LOUDNESS,  1, 3   },
  {VG_AT_PF_MONITOR,   PROF_MONITOR,   1, 2   },
  {VG_AT_PF_QUIET,     PROF_QUIET,     1, 1   },
  {VG_AT_PF_VERBOSE,   PROF_VERBOSE,   1, 1   },
  {VG_AT_PF_X,         PROF_X,         1, 4   },
  {VG_AT_PF_S0,        PROF_S0,        1, 255 },
  {VG_AT_PF_S1,        PROF_S1,        1, 255 },
  {VG_AT_PF_S2,        PROF_S2,        1, 255 },
  {VG_AT_PF_S3,        PROF_S3,        1, 127 },
  {VG_AT_PF_S4,        PROF_S4,        1, 127 },
  {VG_AT_PF_S5,        PROF_S5,        1, 127 },
  {VG_AT_PF_S6,        PROF_S6,        1, 10  },
  {VG_AT_PF_S7,        PROF_S7,        1, 255 },
  {VG_AT_PF_S8,        PROF_S8,        1, 255 },
  {VG_AT_PF_S10,       PROF_S10,       1, 254 },
  {VG_AT_PF_S12,       PROF_S12,       1, 255 },
  {VG_AT_PF_S25,       PROF_S25,       1, 255 },
#if defined (UPGRADE_3G)
  {VG_AT_PF_CR,        PROF_CR,        1, 2   },
#else
  {VG_AT_PF_CR,        PROF_CR,        1, 1   },
#endif
  {VG_AT_PF_FCLASS,    PROF_FCLASS,    1, 1   },
  {VG_AT_PF_DR,        PROF_DR,        1, 1   },
  {VG_AT_PF_CMGF,      PROF_CMGF,      1, 1   },
  {VG_AT_PF_CSDH,      PROF_CSDH,      1, 1   },
  {VG_AT_PF_ILRR,      PROF_ILRR,      1, 1   },
  {VG_AT_PF_CME,       PROF_CME,       1, 2   },
  {VG_AT_PF_IFC,       PROF_IFC,       2, 0   },
  {VG_AT_PF_ICF,       PROF_ICF,       2, 0   },
  {VG_AT_PF_CNMI,      PROF_CNMI,      5, 0   },
  {VG_AT_PF_CSCS,      PROF_CSCS,      1, 0   },
  {VG_AT_PF_IPR,       PROF_IPR,       1, 0   },
  {VG_AT_PF_DS,        PROF_DS,        5, 0   },
  {VG_AT_PF_DCD,       PROF_DCD,       1, 1   },
  {VG_AT_PF_DTR,       PROF_DTR,       1, 2   },
  {VG_AT_PF_CTZU,      PROF_CTZU,      1, 1   },
  {VG_AT_PF_CTZR,      PROF_CTZR,      1, 2   },
#if defined (FEA_MT_PDN_ACT)  
  {VG_AT_PF_CGAUTO,    PROF_CGAUTO,    1, 3   },
#endif /* FEA_MT_PDN_ACT */
  {VG_AT_PF_CGEREP,    PROF_CGEREP,    1, 1   },
#if defined (FEA_MT_PDN_ACT)  
  {VG_AT_PF_MGMTPCACT, PROF_MGMTPCACT, 1, 3   },
#endif /* FEA_MT_PDN_ACT */
#if defined (FEA_PPP)
  {VG_AT_PF_MGPPPLOG,  PROF_MGPPPLOG,  1, 2   },
#endif /* FEA_PPP */    
  {VG_AT_PF_MCEERMODE, PROF_MCEERMODE, 1, 1   },
  {VG_AT_PF_CGPIAF,    PROF_CGPIAF,    4, 1   },
  {VG_AT_PF_MCGEUNSOL, PROF_MCGEUNSOL, 1, 1   },
  {VG_AT_PF_MLTEGCF,       PROF_MLTEGCF,       1, 2   },
  {VG_AT_PF_MLTEGCFLOCK,   PROF_MLTEGCFLOCK,   1, 1   },

  /* For NB-IOT */
  {VG_AT_PF_MPDI,        PROF_MPDI,      1, 1   },
  {VG_AT_PF_MPLMNURI,    PROF_MPLMNURI,  1, 1  },
  {VG_AT_PF_MAPNURI,     PROF_MAPNURI,   1, 1   },
  {VG_AT_PF_MATWAKEUP,   PROF_MATWAKEUP, 1, 1   },

  {VG_AT_LAST_CODE,    NUM_OF_PROF,    0, 0   }
};

/***************************************************************************
 * Global Function Prototypes
 ***************************************************************************/

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

typedef enum SregOperationTag
{
  SREG_QUERY,
  SREG_ASSIGN,
  SREG_ERROR,
  NUM_OF_SREG_OPERATIONS
}
SregOperation_t;

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static const PfCommandInfo_t *getCommandInfo   (const CommandId_t commandId);
static SregOperation_t getSregOperation  (CommandLine_t *commandBuffer_p);

static void viewExt  (const VgmuxChannelNumber entity,
                       const CommandId_t commandId);
static void viewDS   (const VgmuxChannelNumber entity);

static void viewIPR  (const VgmuxChannelNumber entity);

static void viewCSCS (const VgmuxChannelNumber entity);

/***************************************************************************
 * Variables
 ***************************************************************************/

/***************************************************************************
 * Macros
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
*
* Function:    getCommandInfo
*
* Parameters:  commandId - the command Id required
*
* Returns:     PfCommandInfo_t - command information record
*
* Description: searches the the table pfAtCommandInfoTable for information
*              on the specified commandId
*
*-------------------------------------------------------------------------*/

static const PfCommandInfo_t *getCommandInfo (const CommandId_t commandId)
{
  Boolean               found    = FALSE;
  const PfCommandInfo_t *cmdInfo = pfAtCommandInfoTable;

  /* find command info by going through command table comparing commandIds */

  while ((cmdInfo->commandId != VG_AT_LAST_CODE) &&
         (found == FALSE))
  {
    if (cmdInfo->commandId == commandId)
    {
      found = TRUE;
    }
    else
    {
      cmdInfo++;
    }
  }
  FatalCheck (found == TRUE, commandId, 0, 0);

  return (cmdInfo);
}

/*--------------------------------------------------------------------------
*
* Function:    getSregOperation
*
* Parameters:  commandBuffer_p - pointer to the command buffer
*
* Returns:     SregOperation_t - type of S-register operation found
*
* Description: determines the type of s-register operation in the command
*              buffer
*
*-------------------------------------------------------------------------*/

static SregOperation_t getSregOperation (CommandLine_t *commandBuffer_p)
{
  SregOperation_t result = SREG_ERROR;

  if (commandBuffer_p->position < commandBuffer_p->length)
  {
    switch (commandBuffer_p->character[commandBuffer_p->position])
    {
      case QUERY_CHAR:  /* ATSn? */
      {
        commandBuffer_p->position++;
        result = SREG_QUERY;
        break;
      }
      case EQUALS_CHAR:  /* ATSn=... */
      {
        commandBuffer_p->position++;
        result = SREG_ASSIGN;
        break;
      }
      default:  /* AT+... */
      {
        break;
      }
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    viewExt
*
* Parameters:  cmdInfo    - pointer to entry in pfAtCommandTable
*              entity     - mux channel number
*
* Returns:     Nothing
*
* Description: creates query string for extended command pointed to by
*              cmdInfo for current entity
*
*-------------------------------------------------------------------------*/

static void viewExt (const VgmuxChannelNumber entity,
                      const CommandId_t commandId)
{
  const PfCommandInfo_t *commandInfo = getCommandInfo (commandId);
  Int8                  pIndex;
  const AtCmdControl* cmdTable_p = vgPtrToPfAtCommandTable (); 
  Boolean             found = FALSE;
  
  /* find command string by going through command table comparing commandIds */
  while ((cmdTable_p->commandId != VG_AT_LAST_CODE) &&
         (found == FALSE))
  {
    if (cmdTable_p->commandId == commandId)
    {
      found = TRUE;
    }
    else
    {
      cmdTable_p++;
    }
  }

  if (found  == TRUE)
  {
     vgPrintf (entity, (const Char*)"%s: ", (const Char*) cmdTable_p->string);

     for (pIndex = 0;
          pIndex < commandInfo->numOfParams - 1;
          pIndex++)
     {
        vgPrintf (entity, (const Char*)"%d,", (Int16)
               (getProfileValue (entity, (Int8) (commandInfo->profId + pIndex))));
     }

     vgPrintf (entity,
              (const Char*)"%d",
              (Int16)(getProfileValue (entity,
                       (Int8) (commandInfo->profId + commandInfo->numOfParams - 1))));
  }
}


/*--------------------------------------------------------------------------
*
* Function:    viewDS
*
* Parameters:  entity     - mux channel number
*
* Returns:     Nothing
*
* Description: creates data compression configuration string
*
*-------------------------------------------------------------------------*/

static void viewDS (const VgmuxChannelNumber entity)
{
  vgPrintf (entity,
            (const Char*)"+DS: %d,%d,%d,%d",
            getProfileValue (entity, PROF_DS + 0),
            (Int16)(getProfileValue (entity, PROF_DS + 1)),
               (Int16)(getProfileValue (entity, PROF_DS + 2) << 8) +
                (Int16)(getProfileValue (entity, PROF_DS + 3)),
                            (Int16)(getProfileValue (entity, PROF_DS + 4)));
}

/*--------------------------------------------------------------------------
*
* Function:    viewIPR
*
* Parameters:  entity     - mux channel number
*
* Returns:     Nothing
*
* Description: creates IPR string for current entity
*
*-------------------------------------------------------------------------*/

static void viewIPR (const VgmuxChannelNumber entity)
{
    vgGetUartPortBaudRate(entity);

    vgPrintf (entity,
            (const Char*)"+IPR: %s",
             getIprString (getProfileValue (entity, PROF_IPR))->iprString);

}

/*--------------------------------------------------------------------------
*
* Function:    viewCSCS
*
* Parameters:  entity     - mux channel number
*
* Returns:     Nothing
*
* Description: returns current CSCS string
*
*-------------------------------------------------------------------------*/

static void viewCSCS (const VgmuxChannelNumber entity)
{
  vgPrintf (entity,
             (const Char*)"+CSCS: \"%s\"",
              getCscsString ((VgCSCSMode)getProfileValue (entity, PROF_CSCS)));
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
*
* Function:    vgPfOther
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes some profile AT commands with similar implementation
*              requirements: AT?n, where n is a decimal number.
*              ATE, ATL, ATM, ATQ, ATV, ATX
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfOther (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity)
{
  const PfCommandInfo_t *commandInfo = getCommandInfo (getCommandId (entity));
  const ProfileContext_t* factorySpecificDefaults_p = getFactorySpecificDefaults ();
  ResultCode_t          result = RESULT_CODE_ERROR;
  Int32                 param;

  /* check the next character is a digit */
  if (getDecimalValueSafe (commandBuffer_p, &param) == TRUE)
  {
    /* test digit is within allowed ranges */
    if (param <= (Int32)commandInfo->max)
    {
#if defined(UPGRADE_RAVEN_NO_VERBOSE)
      if ((commandInfo->profId == PROF_VERBOSE) &&
          (param == REPORTING_ENABLED ))
      {
        result = VG_CME_OPERATION_NOT_ALLOWED;
      }
      else
#endif
      {
        /* set profile value */
        result = setProfileValue (entity, (Int8) commandInfo->profId, (Int8)param);
      }
    }
  }
  else
  {
    switch(commandInfo->profId)
    {
      case PROF_ECHO:
      case PROF_VERBOSE:
      case PROF_QUIET:
      {
        result = setProfileValue (entity,(Int8) commandInfo->profId, factorySpecificDefaults_p->value[commandInfo->profId]);
        break;
      }
      default:
      {
        break;
      }
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfExtOp
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: generic extended operation handler for profile AT commands:
*              AT+CR,   AT+FCLASS, AT+CMEE,
*              AT+DR,  AT+CMGF, AT+CSDH,   AT+ILRR,
*              AT*MDTX, AT+CTZU, AT+CTZR, AT*MLLC, plus others... (see rvpfss.c
*              pfAtCommandTable.
*              Operations: AT+...? query, current parameter values
*                          AT+...=? range, acceptable parameter values
*                          AT+...=<para list> assign, set parameter values
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfExtOp (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  const PfCommandInfo_t *commandInfo = getCommandInfo (getCommandId (entity));
  ResultCode_t          result       = RESULT_CODE_OK;
  ExtendedOperation_t   operation    = getExtendedOperation (commandBuffer_p);
  Int32  param;
  VgmuxChannelNumber    profileEntity;

  switch (operation)
  {
    case EXTENDED_QUERY:       /* AT+...? */
    {
      vgPutNewLine (entity);
      viewExt (entity, getCommandId (entity));
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_RANGE:       /* AT+...=? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity,
                (const Char*)"%s: ",
                 getCommandName (entity));

      if ((Int16)(commandInfo->max) == 0)
      {
        vgPrintf (entity, (const Char*)"(0)");
      }
      else
      {
        vgPrintf (entity, (const Char*)"(0-%d)", (Int16)(commandInfo->max));
      }
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_ASSIGN:      /* AT+...= */
    {
      /* extract new values from comand line */
      if (getExtendedParameter (
           commandBuffer_p,
            &param,
             (Int32)getProfileValue (entity, (Int8) commandInfo->profId)))
      {
        /* check new values are in range */
        if (param <= (Int32)commandInfo->max)
        {
          /* For LTEGCFLOCK - you are only allowed to set this for one channel
           */
          if ((commandInfo->profId == PROF_MLTEGCFLOCK) && (param == 1))
          {
            for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
            {
              if ((isEntityActive (profileEntity))&&
                  (getProfileValue(profileEntity, PROF_MLTEGCFLOCK)) &&
                  (profileEntity != entity))
              {
                result = VG_CME_OPERATION_NOT_ALLOWED;
              }
            }
          }

#if defined(UPGRADE_RAVEN_NO_VERBOSE)
          if ((commandInfo->profId == PROF_CME) &&
              (param != VG_CME_NUMERIC ))
          {
            result = VG_CME_OPERATION_NOT_ALLOWED;
          }
          else
#endif
          if (result == RESULT_CODE_OK)
          {
            /* set profile value */
            result = setProfileValue (entity, (Int8) commandInfo->profId, (Int8)param);
          }

        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    case EXTENDED_ACTION:      /* AT+... */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfSregOp
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: generic S register operation handler
*              ATS0,  ATS2,  ATS3, ATS4,  ATS5,
*              ATS6,  ATS7,  ATS8, ATS10
*              Operations: AT+Sn? query, current register value
*                          AT+Sn= assign, set register value
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfSregOp (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  const PfCommandInfo_t *commandInfo = getCommandInfo (getCommandId (entity));
  ResultCode_t          result = RESULT_CODE_ERROR;
  SregOperation_t       operation = getSregOperation (commandBuffer_p);
  Int32  param;

  switch (operation)
  {
    case SREG_QUERY:                          /* ATSn?  */
    {
      vgPutNewLine (entity);
      vgPrintf     (entity,
                    (const Char*)"%d",
                     (Int16)(getProfileValue (entity, (Int8) commandInfo->profId)));
      vgPutNewLine (entity);
      result = RESULT_CODE_OK;
      break;
    }
    case SREG_ASSIGN:                         /*ATSn=  */
    {
      /* assign new value to S-register */
      if ((getDecimalValueSafe (commandBuffer_p, &param)) &&
          (param <= (Int32)commandInfo->max))
      {
        /* check S7 & S10 non-zero requirement is met */
        if ( !(((commandInfo->profId == PROF_S7) ||
                (commandInfo->profId == PROF_S10)) &&
                (param == 0)))
        {
          result = setProfileValue (entity, (Int8) commandInfo->profId, (Int8)param);
        }
      }
      break;
    }
    case SREG_ERROR:
    default:
    {
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfS1regOp
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: S1 register operation handler *
*              Operations: ATS1? query, current register value
*                          ATS1= n will return error.can not support modify S1.
*-------------------------------------------------------------------------*/

ResultCode_t vgPfS1regOp (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  const PfCommandInfo_t *commandInfo = getCommandInfo (getCommandId (entity));
  ResultCode_t          result = RESULT_CODE_ERROR;
  SregOperation_t       operation = getSregOperation (commandBuffer_p);
  Int32               param[4];
              
  switch (operation)
  {
    case SREG_QUERY:                          /* ATSn?  */
    {
      vgPutNewLine (entity);
      vgPrintf     (entity,
                    (const Char*)"%d",
                     (Int16)(getProfileValue (entity, (Int8) commandInfo->profId)));
      vgPutNewLine (entity);
      result = RESULT_CODE_OK;
      break;
    }
    case SREG_ASSIGN:
    { 
      /* move past the input although the value is not used to generate the OK*/
      (void)getExtendedParameter (commandBuffer_p,&param[0],ULONG_MAX);
      result = RESULT_CODE_OK;
    }  
    case SREG_ERROR:
    default:
    {
      break;
    }
  }
  return (result);
}


/*--------------------------------------------------------------------------
*
* Function:    vgPfDS
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT+DS command which configures the V.42bis data
*              compression control
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfDS ( CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
  Int32               param[4];
  Boolean             paramsValid;

  switch (operation)
  {
    case EXTENDED_QUERY:  /* AT+DS? */
    {
      vgPutNewLine (entity);
      viewDS       (entity);
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_RANGE:  /* AT+DS=? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity,
                (const Char*)"+DS: (0-3),(0-1),(512-%d),(6-%d)",
                (Int16)VG_V42BIS_MAX_DICT_SIZE,
                (Int16)VG_V42_MAX_STRING_SIZE );
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_ASSIGN: /* AT+DS=... */
    {
      /* extract parameters */
      paramsValid = (Boolean)
                     ((getExtendedParameter (commandBuffer_p, &param[0],
                      (Int32)(getProfileValue (entity, PROF_DS + 0))) == TRUE) &&

                      (getExtendedParameter (commandBuffer_p, &param[1],
                      (Int32)(getProfileValue (entity, PROF_DS + 1))) == TRUE) &&

                      (getExtendedParameter (commandBuffer_p, &param[2],
                      (Int32)(((getProfileValue (entity, PROF_DS + 2)) << 8)
                             + (getProfileValue (entity, PROF_DS + 3)))) == TRUE) &&

                      (getExtendedParameter (commandBuffer_p, &param[3],
                      (Int32)(getProfileValue (entity, PROF_DS + 4))) == TRUE));

      /* check parameter ranges */
      if ((paramsValid == TRUE) &&
          (param[0] <= 3) &&
          (param[1] <= 1) &&
          (param[2] >= 512) && (param[2] <= VG_V42BIS_MAX_DICT_SIZE) &&
          (param[3] >= 6)   && (param[3] <= VG_V42_MAX_STRING_SIZE))

      {
        /* set the profiles values, if possible - may be blocked if there
         * are active calls */

        result = setProfileValue (entity,
                                   PROF_DS + 0,
                                    (Int8)param[0]);
        result = setProfileValue (entity,
                                   PROF_DS + 1,
                                    (Int8)param[1]);
        result = setProfileValue (entity,
                                   PROF_DS + 2,
                                    (Int8)(FIRST_BYTE(param[2])));
        result = setProfileValue (entity,
                                   PROF_DS + 3,
                                    (Int8)(SECOND_BYTE(param[2])));
        result = setProfileValue (entity,
                                   PROF_DS + 4,
                                    (Int8)param[3]);
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    case EXTENDED_ACTION: /* AT+DS... */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfF
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT&F command which sets the
*              current profile to the factory default.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfF (CommandLine_t *commandBuffer_p,
                     const VgmuxChannelNumber entity)
{
  ResultCode_t            result = RESULT_CODE_OK;
  Int32                   value;

  getDecimalValueSafe(commandBuffer_p, &value);

  if (value == 0)
  {
    if( (setDefaultGenericProfile (entity, FALSE) == RESULT_CODE_OK) &&
         (setDefaultSpecificProfile (entity, FALSE) == RESULT_CODE_OK) )
    {
         vgCiUserProfLoadedInd(entity);
    }

    /* this may return an error if changing some of the profile entries is
     * incompatible with current operations. All profile values which can be
     * set will be. */
  }
  else
  {
    result = RESULT_CODE_ERROR;
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfZ
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the ATZ command which reads the user
*              defined profile from NVRAM into the current active profile.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfZ (CommandLine_t *commandBuffer_p,
                     const VgmuxChannelNumber entity)
{
  ResultCode_t  result = RESULT_CODE_OK;
  Int32         value;

  getDecimalValueSafe (commandBuffer_p, &value);

  if (value == 0)
  {

    /* get NVRAM connection */
    if (vgOpManAllocateConnection (entity, NVRAM_CONNECTION) == TRUE)
    {
      /* send read profile request */
      vgNvramDoNvramAccess (READ_REQUEST,
                             entity,
                              vgCiGetAnrm2DataName(entity),
                               VG_NVRAM_READ_ONLY);
      result = RESULT_CODE_PROCEEDING;
    }
    else /* unable to get NVRAM connection */
    {
      result = RESULT_CODE_ERROR;
    }
  }
  else
  {
    result = RESULT_CODE_ERROR;
  }

  /* other commands are not allowed to be concatenated on the end of the line */
  commandBuffer_p->position = commandBuffer_p->length;

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfW
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT&W command (write profile)
*              + stores the current profile values in NVRAM
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfW (CommandLine_t *commandBuffer_p,
                     const VgmuxChannelNumber entity)
{
  ResultCode_t result = RESULT_CODE_PROCEEDING;
  Int32        value;

  getDecimalValueSafe (commandBuffer_p, &value);

  if (value == 0)
  {

    /* get an NVRAM connection to read the used defined profile */
    if (vgOpManAllocateConnection (entity, NVRAM_CONNECTION) == TRUE)
    {
      vgNvramDoNvramAccess (WRITE_REQUEST,
                             entity,
                              vgCiGetAnrm2DataName(entity),
                               VG_NVRAM_READ_WRITE);
    }
    else
    {
      result = RESULT_CODE_ERROR;
    }
  }
  else
  {
    result = RESULT_CODE_ERROR;
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfV
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT&V command (view profile)
*              + sends all active profile values to terminal
*              + no specific ordering
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfV (CommandLine_t *commandBuffer_p,
                     const VgmuxChannelNumber entity)
{
  ResultCode_t result = RESULT_CODE_PROCEEDING;
  Int8         index,
               param;
  Int32        value;
  
  typedef struct cmdProfTableTag
  {
    const Char *command;
    Int8  param;
    Int8  profId;
  } cmdProfTable;

/* profile values for commands which the associated command procedure is not in
 * the profile sub-system
 */

  static const cmdProfTable newCmdTable[] =
  {
    {(const Char*)"*MSIMINS",  1, PROF_MSIMINS  },
    {(const Char*)"+CDIP",     1, PROF_CDIP     },
    {(const Char*)"*MSTMODE",  1, PROF_MSTMODE  },
    {PNULL,      0, NUM_OF_PROF  }
  };

  const cmdProfTable *cmdTable_p = newCmdTable;



  getDecimalValueSafe (commandBuffer_p, &value);

  if (value == 0)
  {
    vgPutNewLine (entity);
    vgPuts (entity, (const Char*)"ACTIVE PROFILE");

    for (index = 0;
          pfAtCommandInfoTable[index].profId != NUM_OF_PROF;
           index++ )
    {
      switch (pfAtCommandInfoTable[index].profId)
      {
        case PROF_CSCS:
        {
          viewCSCS(entity);
          break;
        }
        case PROF_IPR:
        {
          viewIPR(entity);
          break;
        }
        case PROF_DS:
        {
          viewDS(entity);
          break;
        }

        default:
        {
          viewExt (entity, pfAtCommandInfoTable[index].commandId);
          break;
        }
      }

      vgPutNewLine (entity);
    }

    /* using the table cmdTable prints each profile entry that doesn't have an
       associated profile command */

    for (index = 0; cmdTable_p[index].profId != NUM_OF_PROF; index++)
    {
      vgPrintf (entity, (const Char*)"%s: ", cmdTable_p[index].command);
      for (param = 0; param < (cmdTable_p[index].param - 1); param++)
      {
        vgPrintf (entity, (const Char*)"%d,",
         getProfileValue (entity, (Int8) (cmdTable_p[index].profId + param)));
      }
      vgPrintf (entity, (const Char*)"%d",
       (Int16)(getProfileValue (entity,
                (Int8) (cmdTable_p[index].profId + cmdTable_p[index].param - 1))) );

      vgPutNewLine (entity);
    }

    /* Return CMUX parameter */
    vgCimuxReadCmuxParamReq(entity);
    
    result = RESULT_CODE_PROCEEDING;
  }
  else
  {
    result = RESULT_CODE_ERROR;
  }

  return (result);
}

 /*--------------------------------------------------------------------------
*
* Function:    vgPfIFC
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT+IFC command (DTE-DCE local flow control)
*              + display flow control configuration
*              + ranges of parameters
*              + modify the configuration
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfIFC ( CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
  Int32               param[2];

  switch (operation)
  {
    case EXTENDED_QUERY:  /* AT+IFC? */
    {
      vgPutNewLine (entity);
      vgGetUartFlowControlMode(entity);
      vgPrintf     (entity,
                    (const Char*)"+IFC: %d,%d",
                     (Int16)getProfileValue (entity, PROF_IFC + 0),
                      (Int16)getProfileValue (entity, PROF_IFC + 1));
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_RANGE:  /* AT+IFC=? */
    {
      vgPutNewLine (entity);
      vgPuts       (entity, (const Char*)"+IFC: (0-2),(0-2)");
      break;
    }
    case EXTENDED_ASSIGN: /* AT+IFC=... */
    {
      if ((getExtendedParameter (commandBuffer_p,
                                  &param[0],
                                   FC_RTS_CTS) == TRUE) &&
          (getExtendedParameter (commandBuffer_p,
                                  &param[1],
                                   FC_RTS_CTS) == TRUE))
      {
        if ((param[0] == param[1]) && (param[0] < NUMBER_OF_FC_METHODS))
        {
          /* set up the active profile with the new flow control formats */
          result = setProfileValue (entity, PROF_IFC + 0, (Int8)param[0]);
          result = setProfileValue (entity, PROF_IFC + 1, (Int8)param[1]);
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    case EXTENDED_ACTION: /* AT+IFC... */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfIPR
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT+IPR command (Fixed DTE rate)
*              + displays DTE rate setting
*              + accepted parameter value
*              + modify the rate setting
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfIPR ( CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t        result                = RESULT_CODE_OK;
  ExtendedOperation_t operation             = getExtendedOperation (commandBuffer_p);
  ChannelContext_t    *channelContext_p     = ptrToChannelContext (entity);
  Int8                iprSearch;
  Boolean             found;
  Int16               length = 0;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(channelContext_p != PNULL, entity, 0, 0);
#endif
  switch (operation)
  {
    case EXTENDED_QUERY:  /* AT+IPR? */
    {
      vgPutNewLine (entity);
      viewIPR      (entity);
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_RANGE:  /* AT+IPR=? */
    {
      vgPutNewLine (entity);
      /* Now supports auto rate detection */
      vgPrintf (entity, (const Char*)"+IPR: (),(");

#ifdef MTK_AUTO_BAUD_RATE_ENABLE
      for (iprSearch = PORTSPEED_AUTO;
           iprSearch < MAX_NUM_PORTSPEED;
           iprSearch++)
#else
      for (iprSearch = PORTSPEED_110;
           iprSearch < MAX_NUM_PORTSPEED;
           iprSearch++)
#endif
      {
        vgPrintf (entity, (const Char*)"%s", getIprString (iprSearch)->iprString);
        if (iprSearch < (MAX_NUM_PORTSPEED-1))
        {
          vgPrintf (entity, (const Char*)",");
        }
      }
      vgPrintf(entity, (const Char*)")");

      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_ASSIGN: /* AT+IPR=... */
    {
#ifdef MTK_AUTO_BAUD_RATE_ENABLE      
      iprSearch = PORTSPEED_AUTO;
#else
      iprSearch = PORTSPEED_110;
#endif
      found = FALSE;
      while ((found == FALSE) && (iprSearch < MAX_NUM_PORTSPEED))
      {
        length = (Int16)vgStrLen (getIprString (iprSearch)->iprString);

        /* compare ipr string with what is in command buffer */
        if (memcmp (getIprString (iprSearch)->iprString,
                           &commandBuffer_p->character[commandBuffer_p->position],
                            length) == 0)
        {
          found = TRUE;
        }
        else
        {
          iprSearch++;
        }
      }

      if (found == TRUE)
      {
        /* set the active port rate */
        result = setProfileValue (entity, PROF_IPR, iprSearch);

        commandBuffer_p->position += length;

        /*
         * Start timer to send channel config change after OK string is sent out.
         */
        if (channelContext_p->iprOrIcfSettingsChange == FALSE)
        {
          vgCiStartTimer (TIMER_TYPE_PORT_SETTING_CHANGE, entity);
        }
        channelContext_p->iprOrIcfSettingsChange = TRUE;
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    case EXTENDED_ACTION: /* AT+IPR... */
    default:
    {
      result = RESULT_CODE_ERROR;
    }
    break;
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfICF
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT+ICF command (DTE-DCE character framing)
*              + displays character framing configuration
*              + ranges of accepted parameters
*              + modify the configuration
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfICF ( CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t        result                = RESULT_CODE_OK;
  ExtendedOperation_t operation             = getExtendedOperation (commandBuffer_p);
  ChannelContext_t    *channelContext_p     = ptrToChannelContext (entity);
  Int32               param[2];
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(channelContext_p != PNULL, entity, 0, 0);
#endif
  switch (operation)
  {
    case EXTENDED_QUERY:  /* AT+ICF? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity,
                (const Char*)"+ICF: %d,%d",
                 (Int16)getProfileValue (entity, PROF_ICF + 0),
                  (Int16)getProfileValue (entity, PROF_ICF + 1));
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_RANGE:  /* AT+ICF=? */
    {
      vgPutNewLine (entity);
      vgPuts (entity, (const Char*)"+ICF: (1-6),(0-3)");
      break;
    }
    case EXTENDED_ASSIGN: /* AT+ICF=... */
    {
      if ((getExtendedParameter (commandBuffer_p,
                                  &param[0],
                                   VG_ICF_FORMAT_8_DATA_0_PARITY_1_STOP) == TRUE) &&
          (getExtendedParameter (commandBuffer_p,
                                  &param[1],
                                   VG_ICF_PARITY_SPACE) == TRUE))
      {
        if ((param[0] != VG_ICF_FORMAT_AUTO_DETECT) &&  /* auto detect not supported */
            (param[0] < VG_ICF_NUMBER_OF_FORMATS ) &&
            (param[1] < VG_ICF_NUMBER_OF_PARITYS))
        {
          /* set up the active profile with the new charcter formats */
          result = setProfileValue (entity, PROF_ICF + 0, (Int8)param[0]);
          result = setProfileValue (entity, PROF_ICF + 1, (Int8)param[1]);

          /*
           * Start timer to send channel config change after OK string is sent out.
           */
          if (channelContext_p->iprOrIcfSettingsChange == FALSE)
          {
            vgCiStartTimer (TIMER_TYPE_PORT_SETTING_CHANGE, entity);
          }
          channelContext_p->iprOrIcfSettingsChange = TRUE;
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    case EXTENDED_ACTION: /* AT+ICF... */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfCNMI
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT+CNMI command (new message indications to TE)
*              + displays current setting
*              + ranges of accepted parameters
*              + modify the configuration which may result in wiping or flushing
*                the unsolicited messge buffer
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfCNMI ( CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
  Int32               param[NUM_CNMI];
  Int16               index;

  switch (operation)
  {
    case EXTENDED_QUERY:       /* AT+CNMI? */
    {
         vgPutNewLine (entity);
         vgPrintf(entity, (const Char*)"+CNMI: ");
         for( index = 0; index < 4; index++ )
         {
             vgPrintf(entity, (const Char*)"%d,", (Int16)getProfileValue (entity, (Int8) (PROF_CNMI + index)));
         }
         vgPrintf(entity, (const Char*)"0");
         vgPutNewLine (entity);
         break;
    }
    case EXTENDED_RANGE:       /* AT+CNMI=? */
    {
         vgPutNewLine (entity);
         vgPuts (entity, (const Char*)"+CNMI: (0-3),(0-3),(0),(0-2),(0)");  /* CB messages not supported for NB-IOT */
         break;
    }
    case EXTENDED_ASSIGN:      /* AT+CNMI=... */
    {
         /* all parameters are optional */
         for( index = 0; index < NUM_CNMI; index++ )
         {
             if( getExtendedParameter(commandBuffer_p, &param[index],(Int32)(getProfileValue(entity, (Int8)(PROF_CNMI + index)))) != TRUE)
             {
                  result = VG_CMS_ERROR_INVALID_PARMETER;
             }
         }

         if( RESULT_CODE_OK == result )
         {
             if(    (param[0] <= 3)
                 && (param[1] <= 3)
                 && (param[2] <= 0)  /* CB Messages not supported for NB-IOT */
                 && (param[3] <= 2)
                 && (param[4] == 0) )
             {
                 if(    (setProfileValue(entity, PROF_CNMI + 0, (Int8)param[0]) != RESULT_CODE_OK)
                     || (setProfileValue(entity, PROF_CNMI + 1, (Int8)param[1]) != RESULT_CODE_OK)
                     || (setProfileValue(entity, PROF_CNMI + 2, (Int8)param[2]) != RESULT_CODE_OK)
                     || (setProfileValue(entity, PROF_CNMI + 3, (Int8)param[3]) != RESULT_CODE_OK)
                     || (setProfileValue(entity, PROF_CNMI + 4, (Int8)param[4]) != RESULT_CODE_OK) )
                 {
                     result = VG_CMS_ERROR_UNKNOWN;
                 }
             }
             else
             {
                 result = VG_CMS_ERROR_INVALID_PARMETER;
             }
         }
         break;
    }
    case EXTENDED_ACTION:      /* AT+CNMI */
    default:
    {
         result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
         break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfCRES
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT+CRES command (restore SMS settings)
*              + no query of settings
*              + range of accepted parameter
*              + restore settings through action or assignment (=0)
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfCRES ( CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
  Int32               param;

  switch (operation)
  {
    case EXTENDED_QUERY:       /* AT+CRES? */
    {
      result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
      break;
    }
    case EXTENDED_RANGE:       /* AT+CRES=? */
    {
      vgPutNewLine (entity);
      vgPuts (entity, (const Char*)"+CRES: (0)");
      break;
    }
    case EXTENDED_ASSIGN:      /* AT+CRES= */
    {
      /* <profile> is mandatory parameter */
      if (getExtendedParameter(commandBuffer_p, &param, ULONG_MAX) && (param == 0))
      {
        if (vgOpManAllocateConnection (entity, NVRAM_CONNECTION) == TRUE)
        {
          vgNvramDoNvramAccess (READ_REQUEST,
                                 entity,
                                  vgCiGetAnrm2DataName(entity),
                                   VG_NVRAM_READ_CRES);
          result = RESULT_CODE_PROCEEDING;

        }
        else
        {
          result = VG_CMS_ERROR_NVRAM_NOT_AVAILABLE;
        }
      }
      else
      {
        result = VG_CMS_ERROR_INVALID_PARMETER;
      }
      break;
    }
    case EXTENDED_ACTION:      /* AT+CRES */
    {
      if (vgOpManAllocateConnection (entity, NVRAM_CONNECTION) == TRUE)
      {
        vgNvramDoNvramAccess (READ_REQUEST,
                               entity,
                                vgCiGetAnrm2DataName(entity),
                                 VG_NVRAM_READ_CRES);
        result = RESULT_CODE_PROCEEDING;
      }
      else
      {
        result = VG_CMS_ERROR_NVRAM_NOT_AVAILABLE;
      }
      break;
    }
    default:
    {
      result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
      break;
    }
  }
  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfCSAS
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+CSAS command which saves the
*              current SMS settings
*-------------------------------------------------------------------------*/

ResultCode_t vgPfCSAS (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t         result = RESULT_CODE_OK;
  ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
  Int32                param;

  switch (operation)
  {
    case EXTENDED_QUERY:       /* AT+CSAS? */
    {
      result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
      break;
    }
    case EXTENDED_RANGE:       /* AT+CSAS=? */
    {
      vgPutNewLine (entity);
      vgPuts (entity, (const Char*)"+CSAS: (0)" );
      break;
    }
    case EXTENDED_ASSIGN:      /* AT+CSAS= */
    {
      /* <profile> is mandatory parameter */
      if (getExtendedParameter (commandBuffer_p, &param, ULONG_MAX) && (param == 0))
      {
        /* see if we can get an NVRAM connection */
        if (vgOpManAllocateConnection (entity, NVRAM_CONNECTION) == TRUE)
        {

          /* send off request to save SMS settings */
          vgNvramDoNvramAccess (READ_REQUEST,
                                 entity,
                                  vgCiGetAnrm2DataName(entity),
                                   VG_NVRAM_WRITE_CSAS);
          result = RESULT_CODE_PROCEEDING;
        }
        else
        {
          result = VG_CMS_ERROR_NVRAM_NOT_AVAILABLE;
        }
      }
      else
      {
        result = VG_CMS_ERROR_INVALID_PARMETER;
      }
      break;
    }
    case EXTENDED_ACTION:      /* AT+CSAS */
    {
      /* see if we can get an NVRAM connection */
      if (vgOpManAllocateConnection (entity, NVRAM_CONNECTION) == TRUE)
      {
        /* send off request to save SMS settings */
        vgNvramDoNvramAccess (READ_REQUEST,
                               entity,
                                vgCiGetAnrm2DataName(entity),
                                 VG_NVRAM_WRITE_CSAS);
        result = RESULT_CODE_PROCEEDING;
      }
      else
      {
        result = VG_CMS_ERROR_NVRAM_NOT_AVAILABLE;
      }
      break;
    }
    default:
    {
      result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfCSCS
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT+CSCS command (select character set)
*              + displays current character set
*              + ranges of parameters
*              + change the current character set
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfCSCS ( CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
  Char                param[STRING_LENGTH_40 + NULL_TERMINATOR_LENGTH] = {0};
  Int8                index;
  Int16               paramLength = 0;
  Boolean             matchFound = FALSE;

  switch (operation)
  {
    case EXTENDED_QUERY:       /* AT+CSCS? */
    {
      vgPutNewLine (entity);
      viewCSCS (entity);
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_RANGE:       /* AT+CSCS=? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"+CSCS: (");
      for (index = 0; index < (VG_AT_CSCS_MAX_VAL - 1); index++)
      {
        vgPrintf (entity,
                  (const Char*)"\"%s\",",
                    getCscsString ((VgCSCSMode)index));
      }
      vgPrintf (entity,
                (const Char*)"\"%s\")",
                  getCscsString ((VgCSCSMode)(VG_AT_CSCS_MAX_VAL - 1)));
      vgPutNewLine (entity);
      result = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_ASSIGN:      /* AT+CSCS= */
    {
      if (getExtendedString (commandBuffer_p,
                              param,
                               STRING_LENGTH_40,
                                &paramLength) == TRUE)
      {
        for (index = 0;
             (index < VG_AT_CSCS_MAX_VAL) && (matchFound == FALSE);
              index++)
        {
          if (vgStrCmp (param, getCscsString ((VgCSCSMode)index)) == 0)
          {
            result = setProfileValue (entity, PROF_CSCS, (Int8)index);
            matchFound = TRUE;
          }
        }
        if (matchFound == FALSE)
        {
          result = VG_CME_CSCS_MODE_NOT_FOUND;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    case EXTENDED_ACTION:      /* AT+CSCS */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfDCD
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT&C command - Data Carrier Detect(DCD) function mode
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfDCD (CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
  ResultCode_t result = RESULT_CODE_OK;
  Int32        value;

  getDecimalValueSafe (commandBuffer_p, &value);

  switch (value)
  {
    case AMPERC_DCD_SET_ON:
    case AMPERC_DCD_FOLLOWS:
    {
      setProfileValue (entity, PROF_DCD, (Int8)value);
      break;
    }
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfDTR
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT&D command
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfDTR (CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
  Int32        value;
  ResultCode_t result = RESULT_CODE_OK;

  getDecimalValueSafe (commandBuffer_p, &value);

  switch (value)
  {
    case AMPERD_NO_ACTION:
    case AMPERD_COMMAND_STATE:
    case AMPERD_DROP_CALL:
    {
      setProfileValue (entity, PROF_DTR, (Int8)value);
      break;
    }
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfKFC
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT&K command
*              It will overwrite the AT+IFC settings as we don't want to have
*              two profile values meaning the same thing.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfKFC  (CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
  Int32        value;
  ResultCode_t result = RESULT_CODE_OK;

  getDecimalValueSafe (commandBuffer_p, &value);

  switch (value)
  {
    case AMPERK_FC_NONE:
      result = setProfileValue (entity, PROF_IFC + 0, FC_NONE);
      result = setProfileValue (entity, PROF_IFC + 1, FC_NONE);
      break;
    case AMPERK_FC_RTS_CTS:
      result = setProfileValue (entity, PROF_IFC + 0, FC_RTS_CTS);
      result = setProfileValue (entity, PROF_IFC + 1, FC_RTS_CTS);
      break;
    case AMPERK_FC_XON_XOFF:
      result = setProfileValue (entity, PROF_IFC + 0, FC_XON_XOFF_NO_PASS);
      result = setProfileValue (entity, PROF_IFC + 1, FC_XON_XOFF_NO_PASS);
      break;
    default:
      result = RESULT_CODE_ERROR;
      break;
  }
  return (result);
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgPfCSMP
 *
 * Parameters:  (InOut) commandBuffer
 *
 * Returns:     ResultCode_t which is:
 *              - RESULT_CODE_OK if the command has been executed correctly,
 *              - RESULT_CODE_ERROR if not.
 *
 * Description: This function executes the AT+CSMP command which is used
 *              to set the parameters for the SMS text mode.
 *-------------------------------------------------------------------------*/
ResultCode_t vgPfCSMP (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
    ResultCode_t        result              = RESULT_CODE_OK;
    ExtendedOperation_t operation           = getExtendedOperation  (commandBuffer_p);
    SmsCommonContext_t  *smsCommonContext_p = ptrToSmsCommonContext ();
    SmRequestStatus     requestStatus;
    Char                vpAbsoluteChar[VG_CR_SMS_MAX_TIMESTAMP_LEN + NULL_TERMINATOR_LENGTH] = {0};
    Int16               vpAbsoluteLength = 0;
    SmsTimeStamp        vpAbsolute;

    Int32   firstOctetValue;
    Int32   validityPeriodParam;
    Int32   pid     = 0;
    Int32   dcs     = 0;
    Int8    pidByte = 0;
    Int8    dcsByte = 0;

    switch (operation)
    {
        case EXTENDED_RANGE:  /* +CSMP=? */
            {
                /* job115421: no output to be produced, according to 27.005 */
                break;
            }

        case EXTENDED_QUERY:  /* +CSMP? */
            {
                /* Validity Period read from BL - see vgmsprnt.c::vgSmsPrintValidityPeriod()
                for output. */
                result = vgSmsSigOutApexReadSmspReq(entity, VG_SMSP_READ_QUERY_VP);
                break;
            }

        case EXTENDED_ASSIGN: /* +CSMP=... */
        {
            requestStatus = smsCommonContext_p->smSimState.requestStatus;

            if (requestStatus != SM_REQ_OK)
            {
                result = vgSmsUtilConvertRequestError (entity, requestStatus);
            }
            else
            {
                /* all parameters are optional, use existing values, or defaults,
                ** for <fo> <vp> if they are not present */
                if( (getExtendedParameter(  commandBuffer_p,
                                            &firstOctetValue,
                                            smsCommonContext_p->firstOctet) == FALSE) ||
                    (   (cfRvSmHandleConcatSms==TRUE) &&
                        (DECODE_UDH(firstOctetValue) == 0) ) ||
                    (DECODE_MTI(firstOctetValue) == SM_MTI_RESERVED) ||
                    (DECODE_VPF(firstOctetValue) == SM_VPF_ENCHANCED)) /* Unsupported for the moment*/
                {
                    /* parameters not valid */
                    result = RESULT_CODE_ERROR;
                }

                /* Read validity period*/
                memcpy( &vpAbsolute[0],
                        &smsCommonContext_p->validityPeriodAbsolute[0],
                        sizeof( SmsTimeStamp));
                validityPeriodParam = smsCommonContext_p->validityPeriodValue;
                if( result == RESULT_CODE_OK)
                {
                    if( (DECODE_MTI(firstOctetValue) == SM_MTI_SUBMIT) &&
                        (DECODE_VPF(firstOctetValue) == SM_VPF_ABSOLUTE))
                    {
                        /* VP as timestamp*/
                        if( (getExtendedString( commandBuffer_p,
                                                vpAbsoluteChar,
                                                VG_TIMESTAMP_SIZE,
                                                &vpAbsoluteLength) == FALSE) )
                        {
                            /* parameters not valid */
                            result = RESULT_CODE_ERROR;
                        }
                        else if( vpAbsoluteLength != 0)
                        {
                            /* Convert string from current TE set.... */
                            vpAbsoluteLength = vgMapTEToGsm(    getCrOutputBuffer(entity),
                                                                VG_CR_CH_SET_OUTPUT_BUFFER_LEN,
                                                                vpAbsoluteChar,
                                                                vpAbsoluteLength,
                                                                (VgCSCSMode)getProfileValue(entity, PROF_CSCS),
                                                                entity);
                            vpAbsoluteChar[vpAbsoluteLength] = '\0';

                            if( vgSmsUtilEncodeTimestamp(   (Char *)getCrOutputBuffer(entity),
                                                            vpAbsoluteLength,
                                                            vpAbsolute) == FALSE )
                            {
                                /* parameters not valid */
                                result = RESULT_CODE_ERROR;
                            }

                            resetCrOutputBuffer( entity);
                        }
                    }
                    else
                    {
                        /* VP as integer*/
                        if( (getExtendedParameter(  commandBuffer_p,
                                                    &validityPeriodParam,
                                                    smsCommonContext_p->validityPeriodValue) == FALSE) ||
                            (validityPeriodParam > VG_SMS_MAX_VALIDITYPERIOD_VALUE) )
                        {
                            /* parameters not valid */
                            result = RESULT_CODE_ERROR;
                        }
                    }
                }

                /* Read PID :
                ** use existing value for <pid> if parameter is not present */
                pidByte = (Int8)(   smsCommonContext_p->protocolId.protocolMeaning +
                                    smsCommonContext_p->protocolId.protocolId.data);
                if( result == RESULT_CODE_OK)
                {
                    if( (getExtendedParameter(  commandBuffer_p,
                                                &pid,
                                                pidByte) == FALSE) ||
                        (pid > UCHAR_MAX) )
                    {
                        /* parameters not valid */
                        result = RESULT_CODE_ERROR;
                    }
                }

                /* Read DCS :
                ** use existing value for <dcs> if parameter is not present */
                EncodeSmsDataCodingScheme (&dcsByte, &smsCommonContext_p->dataCodingScheme);
                if( result == RESULT_CODE_OK)
                {
                    if( (getExtendedParameter(  commandBuffer_p,
                                                &dcs,
                                                dcsByte) == FALSE) ||
                        (dcs > UCHAR_MAX) )
                    {
                        /* parameters not valid */
                        result = RESULT_CODE_ERROR;
                    }
                }

                if( result == RESULT_CODE_OK)
                {
                     /* only here update all smsCommonContext values */
                    smsCommonContext_p->validityPeriodValue = (Int8)validityPeriodParam;
                    memcpy( &smsCommonContext_p->validityPeriodAbsolute[0],
                            &vpAbsolute[0],
                            sizeof( SmsTimeStamp));

                    smsCommonContext_p->protocolId.protocolMeaning
                        = (ProtocolMeaning)((Int8)pid & 0xE0);
                    smsCommonContext_p->protocolId.protocolId.data
                        =            (Int8)((Int8)pid & 0x1F);

                    /* job115421: save first octet value */
                    smsCommonContext_p->firstOctet = (Int8)firstOctetValue;

                    DecodeSmsDataCodingScheme( (Int8)dcs, &smsCommonContext_p->dataCodingScheme);

                    result = vgSmsSigOutApexReadSmspReq( entity, VG_SMSP_READ_WRITE_VP);
                }
            }
            break;
        }

        case EXTENDED_ACTION: /* AT+CSMP */
        default:
            {
                result = RESULT_CODE_ERROR;
                break;
            }
    }

    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgPfCSMS
 *
 * Parameters:  (InOut) commandBuffer - a pointer to a CommandLine_t
 *              (In)    entity - index to the SMS context to use
 *
 * Returns:     result of operation
 *
 * Description: Handle AT+CSMS command - select Message Service.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgPfCSMS (CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
  ResultCode_t         result = RESULT_CODE_OK;
  ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
  Int32 param;

  switch (operation)
  {
    case EXTENDED_QUERY:       /* AT+CSMS? */
    {
      vgPutNewLine (entity);
      vgPrintf     (entity,
                    (const Char*)"+CSMS: %d,1,1,0",
                     (Int16)getProfileValue (entity, PROF_CSMS));
      vgPutNewLine (entity);
      break;
    }

    case EXTENDED_RANGE:       /* AT+CSMS=? */
    {
      vgPutNewLine (entity);
      vgPuts (entity, (const Char*)"+CSMS: (0,1,128)");
      break;
    }

    case EXTENDED_ASSIGN:      /* AT+CSMS=... */
    {
      /* <service> parameter is mandatory */
      if ((getExtendedParameter (commandBuffer_p, &param, ULONG_MAX)) &&
          ((param == 0) || (param == 1) || (param == 128)))
      {
        result = setProfileValue (entity, PROF_CSMS, (Int8)param);
        vgPutNewLine (entity);
        vgPuts (entity, (const Char*)"+CSMS: 1,1,0");
      }
      else
      {
        result = VG_CMS_ERROR_INVALID_PARMETER;
      }
      break;
    }

    case EXTENDED_ACTION:      /* AT+CSMS */
    {
      result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
      break;
    }

    default:
    {
      result = VG_CMS_ERROR_OP_NOT_SUPPORTED;
      break;
    }
  }

  return (result);
}

#if defined (FEA_MT_PDN_ACT)
/*--------------------------------------------------------------------------
*
* Function:    vgPfCGAUTO
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: performs no operation
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfCGAUTO (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
  ResultCode_t        result    = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
  Int32               autoMode  = 0;

  PARAMETER_NOT_USED(entity);

  switch (operation)
  {
    case EXTENDED_ASSIGN:  /* AT+CGAUTO=  */
    {
      /* record the new setting */
      /* it's rather messy because this setting may also affect csd
         calls.  According to 27.007, the values mean:

         0: no auto-answer for pdp context activation requests from
         network;

         1: auto-answer for ditto; n.b. has the side-effect of causing
         an attach if we're not already attached.

         2: S0, A, H apply to pdp contexts only, and csd calls CANNOT
         be answered AT ALL;

         3: S0, A, H apply to pdp contexts and csd calls (and so A can
         be used for either).

         s.10.2.2.1 also describes the effect of ATS0.  It is not very
         clear what is meant to happen if we get ATS0=1 then
         AT+CGAUTO=2 or 3 -- should we attach if we're not already? I
         assume so. */
      if (getExtendedParameter (commandBuffer_p, &autoMode, ULONG_MAX))
      {
        switch (autoMode)
        {
          case CGAUTO_MODE_NO_PS_AUTOANSWER:
            result = setProfileValue (entity, PROF_CGAUTO, (Int8) autoMode);
            break;
          case CGAUTO_MODE_PS_AUTOANSWER:
            result = setProfileValue (entity, PROF_CGAUTO, (Int8) autoMode);
            result = vgGpEnsureAttached (entity);
            break;
          case CGAUTO_MODE_MODEM_COMPATIBILITY_PS_ONLY:
          case CGAUTO_MODE_MODEM_COMPATIBILITY_PS_AND_CS:
            result = setProfileValue (entity, PROF_CGAUTO, (Int8) autoMode);
            if (getProfileValue (entity, PROF_S0) > 0)
            {
              result = vgGpEnsureAttached (entity);
            }
            break;
          default:
            result = VG_CME_INVALID_INPUT_VALUE;
            break;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    case EXTENDED_RANGE: /* AT+CGAUTO=?  */
    {
      vgPutNewLine (entity);
      vgPrintf (entity,
                (const Char*)"+CGAUTO: (%d-%d)",
                CGAUTO_MODE_NO_PS_AUTOANSWER,
                CGAUTO_MODE_MODEM_COMPATIBILITY_PS_AND_CS);
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_QUERY: /* AT+CGAUTO?  */
    {
      vgPutNewLine (entity);
      vgPrintf (entity,
                (const Char*)"+CGAUTO: %d", getProfileValue (entity, PROF_CGAUTO));
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_ACTION: /* AT+CGAUTO  */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}
#endif /* FEA_MT_PDN_ACT */

/*--------------------------------------------------------------------------
*
* Function:    vgPfCGEREP
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Controls GPRS event reporting
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfCGEREP (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  ResultCode_t          result = RESULT_CODE_OK;
  Int32                 tmpMode;
  Int32                 tmpBfr;

  switch (operation)
  {
    case EXTENDED_RANGE:    /* AT+CGEREP=? */
    {
      vgPutNewLine (entity);
      vgPuts (entity, (const Char*)"+CGEREP: (0-1),(0)");
      break;
    }
    case EXTENDED_ASSIGN:   /* AT+CGEREP=  */
    {
      /* Read in EREP mode */
      if (getExtendedParameter (commandBuffer_p, &tmpMode, ULONG_MAX))
      {
        if ((tmpMode == (Int32)EREP_MODE_0) ||
            (tmpMode == (Int32)EREP_MODE_1))
        {
          result = setProfileValue (entity, PROF_CGEREP, (Int8)tmpMode);
        }
        else
        {
          result = VG_CME_INVALID_EREP_MODE;
        }
      }
      else
      {
        result = VG_CME_INVALID_TEXT_CHARS;
      }

      if (RESULT_CODE_OK == result)
      {
        if (getExtendedParameter (commandBuffer_p, &tmpBfr, ULONG_MAX))
        {
          if ((Int32)EREP_BUFFER_CLEAR != tmpBfr &&
               ULONG_MAX != tmpBfr)
          {
            result = RESULT_CODE_ERROR;
          }
        }
        else
        {
            result = RESULT_CODE_ERROR;
        }
      }
      break;
    }
    case EXTENDED_QUERY:    /* AT+CGEREP?  */
    {
      vgPutNewLine (entity);
      vgPrintf (entity,
                (const Char*)"+CGEREP: %d,0",
                getProfileValue (entity, PROF_CGEREP));
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_ACTION:   /* AT+CGEREP   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}

#if defined (FEA_MT_PDN_ACT)
/*--------------------------------------------------------------------------
*
* Function:    vgPfMGMTPCACT
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Set connection type  for MT PDP activations.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfMGMTPCACT (CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  ResultCode_t          result    = RESULT_CODE_OK;
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  Int32                 mtpcaMode = 0;

  PARAMETER_NOT_USED(entity);

  switch (operation)
  {
    case EXTENDED_ASSIGN:  /* AT*MGMTPCACT=  */
    {
      /* record the new setting */
      if (getExtendedParameter (commandBuffer_p, &mtpcaMode, ULONG_MAX)
          && (mtpcaMode <= 3))
      {
        result = setProfileValue (entity, PROF_MGMTPCACT, (Int8)mtpcaMode);
      }
      else
      {
        result = VG_CME_INVALID_INPUT_VALUE;
      }
      break;
    }
    case EXTENDED_RANGE: /* AT*MGMTPCACT=?  */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"*MGMTPCACT: (0-3)");
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_QUERY: /* AT*MGMTPCACT?  */
    {
      vgPutNewLine (entity);
      vgPrintf (entity,
                (const Char*)"*MGMTPCACT: %d", getProfileValue (entity, PROF_MGMTPCACT));
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_ACTION: /* AT*MGMTPCACT  */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}
#endif /* FEA_MT_PDN_ACT */

#if defined (FEA_PPP)
/*--------------------------------------------------------------------------
*
* Function:    vgPfMGPPPLOG
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: en/disable raw logging in gppp
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfMGPPPLOG (CommandLine_t *commandBuffer_p,
                           const VgmuxChannelNumber entity)
{
  ResultCode_t          result    = RESULT_CODE_OK;
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  Int32                 rawLogging = 0;

  PARAMETER_NOT_USED(entity);

  switch (operation)
  {
    case EXTENDED_ASSIGN:  /* AT*MGPPPLOG=  */
    {
      /* record the new setting */
      if (getExtendedParameter (commandBuffer_p, &rawLogging, ULONG_MAX) &&
          ((rawLogging == 0)
#if defined (ENABLE_PPP_RAW_LOGGING)
           /* Value 1 is permited only if the system is built with
              ENABLE_PPP_RAW_LOGGING */
           || (rawLogging <= 2)
#endif
           ))
      {
        result = setProfileValue (entity, PROF_MGPPPLOG, (Int8)rawLogging);
      }
      else
      {
        result = VG_CME_INVALID_INPUT_VALUE;
      }
      break;
    }
    case EXTENDED_RANGE: /* AT*MGPPPLOG=?  */
    {
      vgPutNewLine (entity);
#if defined (ENABLE_PPP_RAW_LOGGING)
      vgPrintf (entity, (const Char*)"*MGPPPLOG: (0-2)");
#else
      vgPrintf (entity, (const Char*)"*MGPPPLOG: (0)");
#endif
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_QUERY: /* AT*MGPPPLOG?  */
    {
      vgPutNewLine (entity);
      vgPrintf (entity,
                (const Char*)"*MGPPPLOG: %d", getProfileValue (entity, PROF_MGPPPLOG));
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_ACTION: /* AT*MGPPPLOG  */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}
#endif /* FEA_PPP */
 
/*--------------------------------------------------------------------------
*
* Function:    vgPfN1
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the ATN1 command which is an AT command
*              sent by the PC standard modem driver.
*              Just return OK without setting any parameter in the P3G,
*              because we need to use the PC standard modem driver to dial up
*              the internet.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfN1 (CommandLine_t*           commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);

  PARAMETER_NOT_USED(entity);

  switch (operation)
  {
    case EXTENDED_ACTION:    /* ATN1 */
    {

      /*
       * We just ignore this AT command ATN1, and return OK directly
       */
       result = RESULT_CODE_OK;
      break;
    }

    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfS95
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the ATS95 command which is an AT command
*              sent by the PC standard modem driver.
*              Just return OK without setting any parameter in the P3G,
*              because we need to use the PC standard modem driver to dial up
*              the internet.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfS95 (CommandLine_t*           commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
  Int32                param;

  PARAMETER_NOT_USED(entity);

  switch (operation)
  {
    case EXTENDED_ACTION:    /* ATS95 */
    case EXTENDED_QUERY:     /* ATS95? */
    case EXTENDED_RANGE:     /* ATS95=? */
    {
      /*
       * We just ignore this AT command ATS95, and return OK directly
       */
       result = RESULT_CODE_OK;
      break;
    }

    case EXTENDED_ASSIGN:    /* ATS95= */
    {
      if (getExtendedParameter (commandBuffer_p, &param, 0) == TRUE)
      {

      }
      else
      {

      }
      /*
       * We just ignore this AT command ATS95, and return OK directly
       */
      result = RESULT_CODE_OK;
      break;
    }

    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfCGPIAF
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT+CGPIAF command which sets how IPV6 addresses are
*              displayed and entered.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgPfCGPIAF ( CommandLine_t *commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
  Boolean             paramsValid = TRUE;
  Int32               index;
  Int32               param[NUM_CGPIAF];

  switch (operation)
  {
    case EXTENDED_QUERY:       /* AT+CGPIAF? */
    {
      vgPutNewLine (entity);
      viewExt      (entity, VG_AT_PF_CGPIAF);
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_RANGE:       /* AT*CGPIAF=? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"+CGPIAF: (%d,%d),(%d,%d),(%d,%d),(%d,%d)",
                        PROF_CGPIAF_DISABLE, PROF_CGPIAF_ENABLE,
                        PROF_CGPIAF_DISABLE, PROF_CGPIAF_ENABLE,
                        PROF_CGPIAF_DISABLE, PROF_CGPIAF_ENABLE,
                        PROF_CGPIAF_DISABLE, PROF_CGPIAF_ENABLE);
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_ASSIGN:      /* AT+CGPIAF= */
    {
      for (index = 0; index < NUM_CGPIAF; index++)
      {
        if (getExtendedParameter (commandBuffer_p, &param[index],
            (Int32)(getProfileValue (entity, (Int8) (PROF_CGPIAF + index)))) == FALSE)
        {
          paramsValid = FALSE;
        }
      }
      if (paramsValid == TRUE)
      {
        if ((param[PROF_CGPIAF_IPV6_ADDR_FORMAT]     <= PROF_CGPIAF_ENABLE) &&
            (param[PROF_CGPIAF_IPV6_SUBNET_NOTATION] <= PROF_CGPIAF_ENABLE) &&
            (param[PROF_CGPIAF_IPV6_LEADING_ZEROS]   <= PROF_CGPIAF_ENABLE) &&
            (param[PROF_CGPIAF_IPV6_COMPRESS_ZEROS]  <= PROF_CGPIAF_ENABLE))
        {
          for (index = 0; index < NUM_CGPIAF; index++)
          {
            result = setProfileValue (entity,
                                       (Int8) (PROF_CGPIAF + index),
                                        (Int8)param[index]);
          }
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    case EXTENDED_ACTION:      /* AT+CGPIAF */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfMPLMNURI
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT*MPLMNURI command which enables/disables PLMN
*              uplink rate control indications.
*
*-------------------------------------------------------------------------*/
ResultCode_t vgPfMPLMNURI  (CommandLine_t* commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  ResultCode_t         result = RESULT_CODE_OK;
  ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  Int32                param;

  switch (operation)
  {
    case EXTENDED_QUERY:       /* AT*MPLMNURI? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"*MPLMNURI: %d,%d",
                                     getProfileValue(entity, PROF_MPLMNURI),
                                     gprsGenericContext_p->plmnRateControlInfo.plmnRateControlPresent?1:0);
      
      if (gprsGenericContext_p->plmnRateControlInfo.plmnRateControlPresent)
      {
        vgPrintf (entity, (const Char*)",%d",
                                       gprsGenericContext_p->plmnRateControlInfo.plmnRateControlValue);
      }
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_RANGE:       /* AT*MPLMNURI=? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"*MPLMNURI: (%d,%d)",
                                     PROF_MPLMNAPNURI_DISABLE, PROF_MPLMNAPNURI_ENABLE);
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_ASSIGN:      /* AT*MPLMNURI= */
    {
        if (getExtendedParameter (commandBuffer_p, &param, ULONG_MAX) == FALSE)
        {
          result = RESULT_CODE_ERROR;
        }
        else if ((param == PROF_MPLMNAPNURI_DISABLE) || (param == PROF_MPLMNAPNURI_ENABLE))
        {
          result = setProfileValue (entity,
                                    PROF_MPLMNURI,
                                    (Int8)param);
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
      break;
    }
    case EXTENDED_ACTION:      /* AT*MPLMNURI */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgPfMAPNURI
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT*MAPNURI command which enables/disables APN
*              uplink rate control indications.
*
*-------------------------------------------------------------------------*/
ResultCode_t vgPfMAPNURI  (CommandLine_t* commandBuffer_p,
                          const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_OK;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
  GprsGenericContext_t *gprsGenericContext_p = ptrToGprsGenericContext ();
  VgPsdStatusInfo      *psdStatusInfo_p;
  Int32               cidCounter;
  Int32               param;
  Boolean             firstActiveCid = TRUE;

  switch (operation)
  {
    case EXTENDED_QUERY:       /* AT*MAPNURI? */
    {
      for (cidCounter = 0; cidCounter < MAX_NUMBER_OF_CIDS; cidCounter++)
      {
        psdStatusInfo_p = gprsGenericContext_p->cidUserData[cidCounter];
    
        if ((psdStatusInfo_p != PNULL) && 
            (psdStatusInfo_p->isActive))
        {
          if (firstActiveCid)
          {
            firstActiveCid = FALSE;
            vgPutNewLine (entity);                       
          }
          
          vgPrintf (entity, (const Char*)"*MAPNURI: %d,%d,%d",
                                         getProfileValue(entity, PROF_MAPNURI),
                                         cidCounter,
                                         psdStatusInfo_p->apnUplinkRateControlInfo.apnRateControlPresent?1:0);
          
          if (psdStatusInfo_p->apnUplinkRateControlInfo.apnRateControlPresent)
          {
            vgPrintf (entity, (const Char*)",%d,%d,%d",
                                           psdStatusInfo_p->apnUplinkRateControlInfo.apnRateControlAdditionalExceptionReportsAllowed,
                                           psdStatusInfo_p->apnUplinkRateControlInfo.apnRateControluplinkTimeUnit,
                                           psdStatusInfo_p->apnUplinkRateControlInfo.apnRateControlMaxUplinkRate);
          }
          
          vgPutNewLine (entity);        
        }  
      }    
      break;
    }
    case EXTENDED_RANGE:       /* AT*MAPNURI=? */
    {
      vgPutNewLine (entity);
      vgPrintf (entity, (const Char*)"*MAPNURI: (%d,%d)",
                        PROF_MPLMNAPNURI_DISABLE, PROF_MPLMNAPNURI_ENABLE);
      vgPutNewLine (entity);
      break;
    }
    case EXTENDED_ASSIGN:      /* AT*MAPNURI= */
    {
        if (getExtendedParameter (commandBuffer_p, &param, ULONG_MAX) == FALSE)
        {
          result = RESULT_CODE_ERROR;
        }
        else if ((param == PROF_MPLMNAPNURI_DISABLE) || (param == PROF_MPLMNAPNURI_ENABLE))
        {
          result = setProfileValue (entity,
                                    PROF_MAPNURI,
                                    (Int8)param);
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
      break;
    }
    case EXTENDED_ACTION:      /* AT*MAPNURI */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
  return (result);
}

/***************************************************************************
 * Processes
 ***************************************************************************/


/* END OF FILE */



