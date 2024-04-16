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
 * Procedures for General Phone Book AT command execution
 *
 * Contains implementations of the following phonebook AT commands
 *
 * AT+CNUM - retrieves subscriber number
 * AT+CPBW - writes to current phone book
 * AT+CPBF - finds an entry in the current phonebook
 * AT+CPBR - reads current phonebook entries
 * AT+CPBS - selects phone book storage
 **************************************************************************/

#define MODULE_NAME "RVGNCPB"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <gkisig.h>
#include <gkimem.h>

#include <rvutil.h>
#include <rvdata.h>
#include <rvgncpb.h>
#include <rvgnsigo.h>
#include <rvchman.h>
#include <rvcrhand.h>
#include <rvcrconv.h>
#include <rvccut.h>
#include <rvcrerr.h>
#include <rvcimxut.h>
#include <rvgnsigi.h>
#include <rvslut.h>
#include <frhsl.h>




/***************************************************************************
 * Signal definitions
 ***************************************************************************/

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

#if defined (FEA_PHONEBOOK)
#define MAX_INPUT_SIM_ALPHA_ID_SIZE     (SIM_ALPHA_ID_SIZE * VG_CR_SYMBOL_LENGTH_UCS2)
#define MAX_INPUT_SIM_EMAIL_ADD_SIZE    (SIM_UICC_EMAIL_ADDRESS_SIZE * VG_CR_SYMBOL_LENGTH_UCS2)
/*  Size for the buffer that will be used to read the CPBW parameters.
*   Must by the enought to store any of the command parameters, actually the bigger
*   parameter is the email.*/
#define VG_CPBW_INPUT_BUFFER_SIZE       (MAX_INPUT_SIM_EMAIL_ADD_SIZE + NULL_TERMINATOR_LENGTH)

#define TEMP_STORE_LENGTH   20
#endif /* FEA_PHONEBOOK */

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

#if defined (FEA_PHONEBOOK)

static Boolean  isPhoneBookReadOnly         (const Int8 phoneBookIndex);

static ResultCode_t vgGnCpbwReadSimExtParam(    CommandLine_t *commandBuffer_p,
                                                const VgmuxChannelNumber entity);

static void vgGnCpbwClearSimExtParam(           CommandLine_t *commandBuffer_p,
                                                const VgmuxChannelNumber entity);

static ResultCode_t vgGnProcessScpbwParams(     CommandLine_t *commandBuffer_p,
                                                const VgmuxChannelNumber entity);

static ResultCode_t vgGnReadNumberParams(   CommandLine_t              *commandBuffer_p,
                                            const VgmuxChannelNumber    entity,
                                            VgSimDialNum               *num_p,
                                            Int8                       *tempInputBuf_p);

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
*
* Function:        isPhoneBookReadOnly
*
* Parameters:      Int8 - phone book index to check read status of
*
* Returns:         Boolean - indicating if phonebook is read only
*
* Description:     Tests whether a phone book is read only
*
*-------------------------------------------------------------------------*/

static Boolean isPhoneBookReadOnly (const Int8 phoneBookIndex)
{
  Boolean result = FALSE;

  const VgLmInfo *lmInfo = getVgLmInfoRec ();
  WarnCheck (phoneBookIndex < NUMBER_OF_PHONE_BOOKS, phoneBookIndex, 0, 0 );

  if (phoneBookIndex < NUMBER_OF_PHONE_BOOKS)
  {
    result = lmInfo[phoneBookIndex].readOnly;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgGnProcessWriteNumParams
 *
 * Parameters:
 *
 * Returns:    Result code
 *
 * Description: The function is called to read and process Write Dial Number parameters of AT command.
 *              Used for AT+CPBW command.
 *-------------------------------------------------------------------------*/
ResultCode_t vgGnProcessWriteNumParams( CommandLine_t *commandBuffer_p,
                                        const VgmuxChannelNumber entity)
{
    ResultCode_t                result                  = RESULT_CODE_PROCEEDING;
    GeneralContext_t            *generalContext_p       = ptrToGeneralContext (entity);
    Int16                       alphaIdLength           = 0;
    Int8                        *tempInputBuf           = PNULL;
    VgLmData                    *vgLmData               = PNULL;
    Int16                       tempAlphaIdLength;
    Int32                       hiddenEntry;
    Int16                       emailIdLength;
    VgSimDialNum               *dialNum                 = PNULL;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData = &generalContext_p->vgLmData;

    /* Allocate input buffer (Only needed for write operations)*/
    KiAllocZeroMemory(  (Int16)VG_CPBW_INPUT_BUFFER_SIZE,
                        (void **)&tempInputBuf);
    KiAllocZeroMemory(  sizeof(VgSimDialNum),
                        (void **)&dialNum);

    result = vgGnReadNumberParams(  commandBuffer_p,
                                    entity,
                                    dialNum,
                                    tempInputBuf);

    /* Process alpha string parameter*/
    if( result == RESULT_CODE_PROCEEDING)
    {
        /* Copy previous data in LmContext*/
        memcpy( &vgLmData->writeNum[0],
                &dialNum->dialNum[0],
                (MAX_CALLED_BCD_NO_LENGTH + NULL_TERMINATOR_LENGTH) * sizeof(Char));
        vgLmData->phoneBookNumType = dialNum->dialNumType;
        vgLmData->phoneBookNumTypePresent = TRUE;
        vgLmData->writeNumLength = dialNum->dialNumLength;

        /* Process next parameter*/
        if( getExtendedString(  commandBuffer_p,
                                tempInputBuf,
                                vgGetMaxAlphaIdSize( (VgCSCSMode)getProfileValue(entity, PROF_CSCS)),
                                &alphaIdLength) == FALSE)
        {
            /* malformed alpha string*/
            result = RESULT_CODE_ERROR;
        }
        else
        {
            /* Convert alpha Id from current TE set */
            vgLmData->alphaLength
                = vgMapTEToAlphaId( vgLmData->alpha,
                                    SIM_ALPHA_ID_SIZE,
                                    tempInputBuf,
                                    alphaIdLength,
                                    (VgCSCSMode)getProfileValue(entity, PROF_CSCS),
                                    entity);
        }
    }

    /* Process group alpha Id parameter*/
    if( result == RESULT_CODE_PROCEEDING)
    {
        if( getExtendedString(  commandBuffer_p,
                                &tempInputBuf[0],
                                vgGetMaxAlphaIdSize ((VgCSCSMode)getProfileValue(entity, PROF_CSCS)),
                                &tempAlphaIdLength) == FALSE )
        {
            /* malformed string*/
            result = RESULT_CODE_ERROR;
        }
        else
        {
            vgLmData->grpInfo.grpAlphaId.length
                = vgMapTEToAlphaId (    vgLmData->grpInfo.grpAlphaId.data,
                                        SIM_ALPHA_ID_SIZE,
                                        &tempInputBuf[0],
                                        tempAlphaIdLength,
                                        (VgCSCSMode)getProfileValue(entity, PROF_CSCS),
                                        entity);
        }
    }

    /* Process additional number parameters*/
    if( result == RESULT_CODE_PROCEEDING)
    {
        result = vgGnReadNumberParams(  commandBuffer_p,
                                        entity,
                                        &vgLmData->adNumInfo.adNum,
                                        tempInputBuf);
    }

     /* Process second text information*/
    if( result == RESULT_CODE_PROCEEDING)
    {
        if( getExtendedString(  commandBuffer_p,
                                &tempInputBuf[0],
                                vgGetMaxAlphaIdSize ((VgCSCSMode)getProfileValue(entity, PROF_CSCS)),
                                &tempAlphaIdLength) == FALSE)
        {
            /* malformed string*/
            result = RESULT_CODE_ERROR;
        }
        else
        {
            vgLmData->secondName.length = vgMapTEToAlphaId (
                                            vgLmData->secondName.data,
                                            SIM_ALPHA_ID_SIZE,
                                            &tempInputBuf[0],
                                            tempAlphaIdLength,
                                            (VgCSCSMode)getProfileValue(entity, PROF_CSCS),
                                            entity);
        }
    }

    /* Process email information*/
    if( result == RESULT_CODE_PROCEEDING)
    {
        if( getExtendedString(  commandBuffer_p,
                                &tempInputBuf[0],
                                MAX_INPUT_SIM_EMAIL_ADD_SIZE + NULL_TERMINATOR_LENGTH,
                                &emailIdLength) == FALSE)
        {
            /* malformed string*/
            result = RESULT_CODE_ERROR;
        }
        else
        {
            vgLmData->emailInfo.email.length
                = vgMapTEToAlphaId( vgLmData->emailInfo.email.data,
                                    SIM_UICC_EMAIL_ADDRESS_SIZE,
                                    &tempInputBuf[0],
                                    emailIdLength,
                                    (VgCSCSMode)getProfileValue(entity, PROF_CSCS),
                                    entity);
        }
    }

    /* check is there hidden entry information*/
    if( result == RESULT_CODE_PROCEEDING)
    {
        if( getExtendedParameter(   commandBuffer_p,
                                    &hiddenEntry,
                                    0) == FALSE)
        {
            /* Invalid parameter*/
            result = RESULT_CODE_ERROR;
        }
        else
        {
            /* hidden entry can have only Boolean values */
            if( hiddenEntry > 1)
            {
                result = RESULT_CODE_ERROR;
            }
            else
            {
                vgLmData->hiddenEntry = (Boolean) hiddenEntry;
            }
        }
    }

  /* Release the input buffer memory*/
  KiFreeMemory ((void **)&tempInputBuf);
  KiFreeMemory ((void **)&dialNum);

  return result;
}
#endif /* FEA_PHONEBOOK */

/***************************************************************************
 * Global Functions
 ***************************************************************************/

#if defined (FEA_PHONEBOOK)

#if 0
/*--------------------------------------------------------------------------
*
* Function:    vgGnCNUM
*
* Parameters:  commandBuffer_p    - pointer to command line string
*              entity             - mux channel number
*
* Returns:     Resultcode_t       - result of function
*
* Description: executes the AT+CNUM command which displays the subscriber
*              number.
*-------------------------------------------------------------------------*/
ResultCode_t vgGnCNUM (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t        result = RESULT_CODE_ERROR;
  ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);

  switch (operation)
  {
    case EXTENDED_RANGE:  /* +CNUM=? */
    {
      result = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_ACTION:   /* AT+CNUM  */
    {
      /* get subscriber number, need to temporarily set phonebook to MSISDN */

      setTemporaryPhoneBook (entity, DIAL_LIST_MSISDN);

      result = vgChManContinueAction (entity, SIG_APEX_LM_DIALNUM_STATUS_REQ);
      break;
    }
    case EXTENDED_ASSIGN:   /* +CNUM=<n>  (command not supported) */
    case EXTENDED_QUERY:    /* +CNUM?     (command not supported) */
    default:
    {
      break;
    }
  }

  return (result);
}
#endif

/*--------------------------------------------------------------------------
*
* Function:        resetTemporaryPhoneBook
*
* Parameters:      VgmuxChannelNumber - entity which sent request
*
* Returns:         Nothing
*
* Description:     Resets phone to old phone if required
*
*-------------------------------------------------------------------------*/

void resetTemporaryPhoneBook (const VgmuxChannelNumber entity)
{
  GeneralContext_t *generalContext_p = ptrToGeneralContext (entity);

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  if (generalContext_p->vgLmData.needOldPhoneBook == TRUE)
  {
    generalContext_p->vgLmData.phoneBook        = generalContext_p->vgLmData.oldPhoneBook;
    generalContext_p->vgLmData.needOldPhoneBook = FALSE;
  }
}

/*--------------------------------------------------------------------------
*
* Function:        setTemporaryPhoneBook
*
* Parameters:      VgmuxChannelNumber - entity which sent request
*                  LmDialNumberFile   - phonebook to temporarily switch to
*
* Returns:         Nothing
*
* Description:     sets the phone book temporarily
*
*-------------------------------------------------------------------------*/

void setTemporaryPhoneBook (const VgmuxChannelNumber entity,
                             const LmDialNumberFile newPhoneBook)
{
  GeneralContext_t *generalContext_p = ptrToGeneralContext (entity);

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  generalContext_p->vgLmData.oldPhoneBook     = generalContext_p->vgLmData.phoneBook;
  generalContext_p->vgLmData.needOldPhoneBook = TRUE;
  generalContext_p->vgLmData.phoneBook        = newPhoneBook;
}




/*--------------------------------------------------------------------------
*
* Function:    vgGnCPBW
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+CPBW command which can create
*              and delete phonebook entries.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnCPBW ( CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
    ResultCode_t                result                  = RESULT_CODE_PROCEEDING;
    ExtendedOperation_t         operation               = getExtendedOperation (commandBuffer_p);
    GeneralContext_t           *generalContext_p        = ptrToGeneralContext (entity);
    GeneralGenericContext_t    *generalGenericContext_p = ptrToGeneralGenericContext();
    VgLmData                   *vgLmData                = PNULL;
    Int32                       index                   = 0;
    Boolean                     present                 = FALSE;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData = &generalContext_p->vgLmData;

    switch (operation)
    {
        case EXTENDED_RANGE:  /* AT+CPBW=? */
        {
            /* determine if current phone book storage can be written to */
            if (isPhoneBookReadOnly (vgLmData->phoneBookIndex))
            {
                vgPutNewLine (entity);
                vgPuts (entity, (const Char*)"+CPBW: (0)");
                result = RESULT_CODE_OK;
            }
            else
            {
                vgLmData->phoneBookOperation = VG_PB_RANGE;
                if( (vgLmData->phoneBook == DIAL_LIST_ADN_APP)||
                    (vgLmData->phoneBook == DIAL_LIST_ADN_GLB))
                {
                    /* Need to send phonebook status, not dialnumStatus
                    * as we need the length of the email/group/second name...
                    * ApexLmDialnumStatusCnf won' t give us this information*/
                    result = vgChManContinueAction (entity, SIG_APEX_LM_PHONEBOOK_STATUS_REQ);
                }
                else
                {
                    result = vgChManContinueAction (entity, SIG_APEX_LM_DIALNUM_STATUS_REQ);
                }
            }
        }
        break;

        case EXTENDED_ASSIGN: /* AT+CPBW=... */
        {
            /* determine if current phone book storage can be written to */
            if (isPhoneBookReadOnly (vgLmData->phoneBookIndex))
            {
                result = VG_CME_OPERATION_NOT_ALLOWED;
            }
            else
            {
                /* get phone book index and dial string */
                if( ( getExtendedParameterPresent(  commandBuffer_p,
                                                    &index,
                                                    USHRT_MAX,
                                                    &present) == FALSE) )
                {
                    /* invalid parameters entered */
                    result = RESULT_CODE_ERROR;
                }
            }

            /* Process the index parameter*/
            if( result == RESULT_CODE_PROCEEDING)
            {
                /* if phone book index is USHRT_MAX, then write to first free space */
                if( present == FALSE)
                {
                    vgLmData->phoneIndex1 = 0;
                    if( isRemainingParameter(commandBuffer_p) == TRUE)
                    {
                        vgLmData->phoneBookOperation = VG_PB_WRITE;
                    }
                    else
                    {
                        result = RESULT_CODE_ERROR;
                    }
                }
                else if(index == 0)
                {
                    result = VG_CME_INVALID_INDEX;
                }
                else
                {
                    vgLmData->phoneIndex1 = (LmRecordNumber)index;
                    if( isRemainingParameter(commandBuffer_p) == TRUE)
                    {
                        vgLmData->phoneBookOperation = VG_PB_WRITE;
                        M_FrGkiPrintf (ATCI, "pb improve: at+cpbw w: %d",index);
                    }
                    else
                    {
                        vgLmData->phoneBookOperation = VG_PB_DELETE;
                        M_FrGkiPrintf (ATCI, "pb improve: at+cpbw d: %d",index);
                    }
                }
            }

            /* For WRITE request, continue to proccess arguments*/
            if( (result == RESULT_CODE_PROCEEDING) &&
                (vgLmData->phoneBookOperation == VG_PB_WRITE) )
            {
                result = vgGnProcessWriteNumParams( commandBuffer_p, entity);

                /* If needed, read the extended parameters*/
                if( (result == RESULT_CODE_PROCEEDING) &&
                    (getProfileValue( entity, PROF_MUPBCFG) == VG_MUPBCFG_EXT_PARAM_ENABLE) &&
                    (   (vgLmData->phoneBook == DIAL_LIST_ADN_GLB) ||
                        (vgLmData->phoneBook == DIAL_LIST_ADN_APP) ) )
                {
                    vgGnCpbwReadSimExtParam( commandBuffer_p, entity);
                }
            } /* End of write operation specific bloc*/

            /* send write request off if parameters check out */
            if( result == RESULT_CODE_PROCEEDING)
            {
                /*send phonebook status in order to know whether group info, email, sne are supported*/
                if( (vgLmData->phoneBook == DIAL_LIST_ADN_GLB) ||
                    (vgLmData->phoneBook == DIAL_LIST_ADN_APP))
                {
                    result = vgChManContinueAction (entity, SIG_APEX_LM_PHONEBOOK_STATUS_REQ);
                }
                else
                {
                    result = vgChManContinueAction (entity, SIG_APEX_LM_DIALNUM_STATUS_REQ);
                }
            }
        }
        break;

        case EXTENDED_QUERY:  /* AT+CPBW? */
        case EXTENDED_ACTION: /* AT+CPBW... */
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
* Function:    vgGnCpbwClearSimExtParam
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Nothing
*
* Description: Set to 0 all the additional parameters
*
*-------------------------------------------------------------------------*/
static void vgGnCpbwClearSimExtParam(   CommandLine_t *commandBuffer_p,
                                        const VgmuxChannelNumber entity)
{
    GeneralContext_t           *generalContext_p        = ptrToGeneralContext (entity);
    VgLmData                   *vgLmData_p              = PNULL;
    Int8                        i                       = 0;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    for(    i=0; i<VG_MUPBCFG_MAX_AD_GROUPS; i++)
    {
        vgLmData_p->grpInfo.adGasIndex[i] = 0;
    }

    for(    i=0; i<VG_MUPBCFG_MAX_AD_NUMBER; i++)
    {
        vgLmData_p->adNumInfo.adAdNums[i].dialNumLength = 0;
    }

    for(    i=0; i<VG_MUPBCFG_MAX_AD_EMAIL; i++)
    {
        vgLmData_p->emailInfo.adEmails[i].length = 0;
    }

    for(    i=0; i<VG_MUPBCFG_MAX_AD_AAS; i++)
    {
        vgLmData_p->aasInfo.adAasIndex[i] = 0;
    }
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnCpbwReadExtParam
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Read the extended parameters from a CPBW command
*
*-------------------------------------------------------------------------*/
static ResultCode_t vgGnCpbwReadSimExtParam(    CommandLine_t *commandBuffer_p,
                                                const VgmuxChannelNumber entity)
{
    ResultCode_t                result                  = RESULT_CODE_PROCEEDING;
    GeneralContext_t           *generalContext_p        = ptrToGeneralContext (entity);
    VgLmData                   *vgLmData_p              = PNULL;
    Int32                       index                   = 0;
    Int8                        i                       = 0;
    Int16                       emailIdLength           = 0;
    Int8                       *tempInputBuf_p          = PNULL;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    KiAllocMemory(  (Int16)VG_CPBW_INPUT_BUFFER_SIZE,
                    (void **)&tempInputBuf_p);

    /* Read for the additional groups index*/
    for(    i=0;
            (   (i<VG_MUPBCFG_MAX_AD_GROUPS) &&
                (result==RESULT_CODE_PROCEEDING) );
            i++)
    {
        if( (getExtendedParameter(  commandBuffer_p,
                                    &index,
                                    0) == FALSE) ||
            (index > 0xFF) )
        {
            /* Invalid parameter*/
            result = RESULT_CODE_ERROR;
        }
        else
        {
            vgLmData_p->grpInfo.adGasIndex[i] = (Int8)index;
        }
    }

    /* Read for the additional numbers and types*/
    for(    i=0;
            (   (i<VG_MUPBCFG_MAX_AD_NUMBER) &&
                (result==RESULT_CODE_PROCEEDING) );
            i++)
    {
        result = vgGnReadNumberParams(  commandBuffer_p,
                                        entity,
                                        &vgLmData_p->adNumInfo.adAdNums[i],
                                        tempInputBuf_p);
    }

    /* Read for the additional emails*/
    for(i=0; (i < VG_MUPBCFG_MAX_AD_EMAIL) && (result == RESULT_CODE_PROCEEDING); i++)
    {
        if( getExtendedString(  commandBuffer_p,
                                &tempInputBuf_p[0],
                                MAX_INPUT_SIM_EMAIL_ADD_SIZE,
                                &emailIdLength) == FALSE)
        {
            /* malformed string*/
            result = RESULT_CODE_ERROR;
        }
        else
        {
            vgLmData_p->emailInfo.adEmails[i].length
                = vgMapTEToAlphaId( vgLmData_p->emailInfo.adEmails[i].data,
                                    SIM_UICC_EMAIL_ADDRESS_SIZE,
                                    &tempInputBuf_p[0],
                                    emailIdLength,
                                    (VgCSCSMode)getProfileValue(entity, PROF_CSCS),
                                    entity);
        }
    }

    /* Read for the additional AAS index*/
    for(    i=0;
            (   (i<VG_MUPBCFG_MAX_AD_AAS) &&
                (result==RESULT_CODE_PROCEEDING) );
            i++)
    {
        if( (getExtendedParameter(  commandBuffer_p,
                                    &index,
                                    0) == FALSE) ||
            (index > 0xFF) )
        {
            /* Invalid parameter*/
            result = RESULT_CODE_ERROR;
        }
        else
        {
            vgLmData_p->aasInfo.adAasIndex[i] = (Int8)index;
        }
    }

    /* Release the input buffer memory*/
    KiFreeMemory ((void **)&tempInputBuf_p);

    return result;
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnCPBR
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+CPBR command which reads the
*              current phonebook entries.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnCPBR (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
    ResultCode_t                result                  = RESULT_CODE_ERROR;
    ExtendedOperation_t         operation               = getExtendedOperation (commandBuffer_p);
    Int32                       param1;
    Int32                       param2;
    GeneralContext_t           *generalContext_p        = ptrToGeneralContext (entity);
    GeneralGenericContext_t    *generalGenericContext_p = ptrToGeneralGenericContext();
    VgLmData                   *vgLmData;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData = &generalContext_p->vgLmData;

    switch (operation)
    {
        case EXTENDED_RANGE:  /* AT+CPBR=? */
        {
            vgLmData->phoneBookOperation = VG_PB_RANGE;

            if( (vgLmData->phoneBook == DIAL_LIST_ADN_APP)||
                (vgLmData->phoneBook == DIAL_LIST_ADN_GLB))
            {
                /* Need to send phonebook status, not dialnumStatus
                * as we need the length of the email/group/second name...
                * ApexLmDialnumStatusCnf won' t give us this information*/
                result = vgChManContinueAction (entity, SIG_APEX_LM_PHONEBOOK_STATUS_REQ);
            }
            else
            {
                result = vgChManContinueAction (entity, SIG_APEX_LM_DIALNUM_STATUS_REQ);
            }
        }
        break;

        case EXTENDED_ASSIGN: /* AT+CPBR=... */
        {
            /* get lower bound phone book entry index */
            if (getExtendedParameter (commandBuffer_p, &param1, 0) == TRUE)
            {
                M_FrGkiPrintf (ATCI, "pb improve: at+cpbr: %d",param1);
                /* get upper bound phone book entry index */
                if (getExtendedParameter (commandBuffer_p, &param2, param1) == TRUE)
                {
                    if( (param1 > 0) &&
                        (param2 >= param1))
                    {
                        vgLmData->phoneIndex1 = (LmRecordNumber)param1;
                        vgLmData->phoneIndex2 = (LmRecordNumber)param2;
                        vgLmData->currentLnaRecord = 0;

                        if (vgLmData->phoneIndex2 == vgLmData->phoneIndex1)
                        {
                            /* read the single specified phonebook entry */
                            vgLmData->phoneIndex2 = 0;
                            vgLmData->readMode = LM_READ_ABSOLUTE;
                        }
                        else
                        {
                            /* read all phone book entries between lower and upper entry
                            * indexes */
                            vgLmData->phoneIndex1--;
                            vgLmData->currentLnaRecord = vgLmData->phoneIndex1;
                            vgLmData->readMode = LM_READ_NEXT;
                        }

                        vgLmData->phoneBookOperation = VG_PB_READ;

                        /*send phonebook status in order to know whether group info, email, sne are supported*/
                        if( (vgLmData->phoneBook == DIAL_LIST_ADN_GLB) ||
                            (vgLmData->phoneBook == DIAL_LIST_ADN_APP))
                        {
                            result = vgChManContinueAction (entity, SIG_APEX_LM_PHONEBOOK_STATUS_REQ);
                        }
                        else

                        {
                            result = vgChManContinueAction (entity, SIG_APEX_LM_DIALNUM_STATUS_REQ);
                        }
                    }
                    else
                    {
                        result = VG_CME_INVALID_INDEX;
                    }
                }
            }
        }
        break;

        case EXTENDED_QUERY:  /* AT+CPBR? */
        case EXTENDED_ACTION: /* AT+CPBR... */
        default:
        {
        }
        break;
    }

    return (result);
}



/*--------------------------------------------------------------------------
*
* Function:    vgGnCPBS
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+CPBS command which selects the
*              current phone book storage used by the other phone book commands
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnCPBS (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
    ResultCode_t            result                  = RESULT_CODE_OK;
    ExtendedOperation_t     operation               = getExtendedOperation (commandBuffer_p);
    Boolean                 storeFound              = FALSE;
    Int8                    index                   = 0;
    Char                    phoneStore[STRING_LENGTH_40 + NULL_TERMINATOR_LENGTH] = {0};
    Int16                   phoneStoreLength;
    GeneralContext_t       *generalContext_p        = ptrToGeneralContext (entity);
    VgLmData               *vgLmData;
    const VgLmInfo         *lmInfo                  = getVgLmInfoRec ();
    SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
    GeneralGenericContext_t     *generalGenericContext_p  = ptrToGeneralGenericContext();
    VgSimInfo              *simInfo = &(simLockGenericContext_p->simInfo);
    AbglFeatureConfigDataArea   *currentCfgVar = &generalGenericContext_p->vgMFtrCfgData.currentCfgVar;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData = &generalContext_p->vgLmData;

    switch (operation)
    {
        case EXTENDED_QUERY:  /* AT+CPBS? */
        {
            vgLmData->vgCpbsData.operation = EXTENDED_QUERY;
            result = vgChManContinueAction (entity, SIG_APEX_LM_DIALNUM_STATUS_REQ);
        }
        break;

        case EXTENDED_RANGE:  /* AT+CPBS=? */
        {
            vgLmData->vgCpbsData.operation = EXTENDED_RANGE;
            result = vgChManContinueAction (entity, SIG_APEX_LM_DIALNUM_STATUS_REQ);
        }
        break;

        case EXTENDED_ASSIGN: /* AT+CPBS=... */
        {
            M_FrGkiPrintf (ATCI, "pb improve AT+CPBS: select phone book");
            /*in 3G, can't set the phonebook until the SIM is ready as we may need to know
            * whether OCI/ICI are supported by the SIM in order to select the correct file*/
            if (simLockGenericContext_p->simState != VG_SIM_READY)
            {
                result = vgGetSimCmeErrorCode ();
            }
            else
            {
                /* get phone book storage identifier */
                if( getExtendedString(  commandBuffer_p,
                                        phoneStore,
                                        STRING_LENGTH_40,
                                        &phoneStoreLength) == TRUE)
                {
                    /* search through valid storage searching for a match */
                    while(  (storeFound == FALSE) &&
                            (index < NUMBER_OF_PHONE_BOOKS))
                    {
                        if (vgStrCmp (lmInfo[index].vgPhoneStore, phoneStore) == 0)
                        {
                            /* match has been found set phone book storage */
                            storeFound = TRUE;
                            /* if there is no password still set the phonebook storage
                            * as it should be possible to read entries */
                            vgLmData->vgCpbsData.phoneBook = lmInfo[index].file;
                            vgLmData->vgCpbsData.phoneBookIndex = index;

                            if( (lmInfo[index].file == DIAL_LIST_LND) &&
                                simInfo->oci &&
                                currentCfgVar->callInfoStorageMode == GL_FEATURE_SAVE_CALL_INFO_SIM)
                            {
                                /* AB expects an OCI (Outgoing Call Information) list request
                                 * else it will treat request as invalid */
                                vgLmData->vgCpbsData.phoneBook = DIAL_LIST_OCI;
                            }
                            else if(    (lmInfo[index].file == DIAL_LIST_LNM)  &&
                                        simInfo->ici &&
                                        currentCfgVar->callInfoStorageMode == GL_FEATURE_SAVE_CALL_INFO_SIM)
                            {
                                /* AB expects an ICI (Icoming Call Information) list request
                                 * else it will treat request as invalid */
                                vgLmData->vgCpbsData.phoneBook = DIAL_LIST_ICI;
                                vgLmData->vgCpbsData.iciType = LM_ICI_MISSED;
                                /*user requested missed calls only*/
                            }
                            else if(    (lmInfo[index].file == DIAL_LIST_LNR)  &&
                                        simInfo->ici &&
                                        currentCfgVar->callInfoStorageMode == GL_FEATURE_SAVE_CALL_INFO_SIM)
                            {
                                /* AB expects an ICI (Icoming Call Information) list request
                                 * else it will treat request as invalid */
                                vgLmData->vgCpbsData.phoneBook = DIAL_LIST_ICI;
                                vgLmData->vgCpbsData.iciType = LM_ICI_RECEIVED;
                                /*user requested received calls only*/
                            }

                        }
                        else
                        {
                            index++;
                        }
                    }

                    if (storeFound == FALSE)
                    {
                        result = RESULT_CODE_ERROR;
                    }
                }
                else /* phone book storage parameter not entered */
                {
                    result = RESULT_CODE_ERROR;
                }
            }
            if (result == RESULT_CODE_OK)
            {
                /* Get password (optional), it will be ignored if it is verified */
                vgLmData->vgCpbsData.passwordPresent
                    = getExtendedString(    commandBuffer_p,
                                            vgLmData->vgCpbsData.password,
                                            SIM_CHV_LENGTH,
                                            &vgLmData->vgCpbsData.passwordLength);

                /* Ask for phonebook status*/
                vgLmData->vgCpbsData.operation = EXTENDED_ASSIGN;
                result = vgChManContinueAction (entity, SIG_APEX_LM_DIALNUM_STATUS_REQ);
            }
        }
        break;

        case EXTENDED_ACTION: /* AT+CPBS... */
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
* Function:    vgGnCPBF
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+CPBF command which finds
*              phone book entries.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnCPBF(  CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
    ResultCode_t                result                  = RESULT_CODE_ERROR;
    ExtendedOperation_t         operation               = getExtendedOperation (commandBuffer_p);
    GeneralContext_t           *generalContext_p        = ptrToGeneralContext (entity);
    GeneralGenericContext_t    *generalGenericContext_p = ptrToGeneralGenericContext();
    VgLmData                   *vgLmData;
    Int8                       *alphaId_p               = PNULL;
    Int16                       alphaIdLength           = 0;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData = &generalContext_p->vgLmData;

    switch (operation)
    {
        case EXTENDED_RANGE:  /* AT+CPBF=? */
        {
            /* returns maximum number length and maximum text length */
            vgLmData->phoneBookOperation = VG_PB_RANGE;

            if( (vgLmData->phoneBook == DIAL_LIST_ADN_APP)||
                (vgLmData->phoneBook == DIAL_LIST_ADN_GLB))
            {
                /* Need to send phonebook status, not dialnumStatus
                * as we need the length of the email/group/second name...
                * ApexLmDialnumStatusCnf won' t give us this information*/
                result = vgChManContinueAction (entity, SIG_APEX_LM_PHONEBOOK_STATUS_REQ);
            }
            else

            {
                result = vgChManContinueAction (entity, SIG_APEX_LM_DIALNUM_STATUS_REQ);
            }
        }
        break;

        case EXTENDED_ASSIGN: /* AT+CPBF=... */
        {
            KiAllocZeroMemory(  sizeof(Int8) * (MAX_INPUT_SIM_ALPHA_ID_SIZE + NULL_TERMINATOR_LENGTH),
                        (void **)&alphaId_p);
            /* get alpha text to search for */
            if(getExtendedString(   commandBuffer_p,
                                    &alphaId_p[0],
                                    vgGetMaxAlphaIdSize ((VgCSCSMode)getProfileValue(entity, PROF_CSCS)),
                                    &alphaIdLength) == TRUE)
            {
                vgLmData->phoneBookOperation = VG_PB_FIND;

                /* Convert alphaId */
                vgLmData->alphaLength = vgMapTEToAlphaId (
                                            vgLmData->alpha,
                                            SIM_ALPHA_ID_SIZE,
                                            &alphaId_p[0],
                                            alphaIdLength,
                                            (VgCSCSMode)getProfileValue (entity, PROF_CSCS),
                                            entity);


                if( (vgLmData->phoneBook == DIAL_LIST_ADN_APP)||
                    (vgLmData->phoneBook == DIAL_LIST_ADN_GLB))
                {
                    /* Need to send phonebook status, not dialnumStatus
                    * as we need to know whether the email/group/second name...
                    * are supported.
                    * ApexLmDialnumStatusCnf won' t give us this information*/
                    result = vgChManContinueAction (entity, SIG_APEX_LM_PHONEBOOK_STATUS_REQ);
                }
                else

                {
                    result = vgChManContinueAction (entity, SIG_APEX_LM_DIALNUM_STATUS_REQ);
                }
            }
            else /* alpha parameter missing */
            {
                result = RESULT_CODE_ERROR;
            }
            KiFreeMemory( (void **)&alphaId_p);
        }
        break;

        case EXTENDED_QUERY:  /* AT+CPBF? */
        case EXTENDED_ACTION: /* AT+CPBF... */
        default:
        {
        }
        break;
    }

    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgGnMUPBSYNC
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     Non-standard AT command to read and USIM phonebook
 *                  synchronization data in a UICC for the current
 *                  3G Phonebook selected
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgGnMUPBSYNC(CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
    ResultCode_t        result = RESULT_CODE_OK;
    Int32               dataType;
    Int32               index;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    VgLmData           *vgLmData;
    Boolean             present;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData = &generalContext_p->vgLmData;

    switch (operation)
    {

        case EXTENDED_RANGE:  /* AT*MUPBSYNC=? */
            {
                if(     vgLmData->phoneBook == DIAL_LIST_ADN_APP ||
                        vgLmData->phoneBook == DIAL_LIST_ADN_GLB)
                {
                    /* Need reading either synchronisation status to know for which phonebook
                     * 3G synchronisation is avalaible and the selected phonebook status to
                     * knnow its record capacity */
                    generalContext_p->vgLmData.vgMupbsyncContext.operation = EXTENDED_RANGE;
                    result = vgChManContinueAction (entity, SIG_APEX_LM_DIALNUM_STATUS_REQ);
                }
                else
                {
                    result = VG_CME_OPERATION_NOT_ALLOWED;
                }
            }
            break;

        case EXTENDED_ASSIGN: /* AT*MUPBSYNC=... */
            {
                if ( getExtendedParameter (commandBuffer_p, &dataType, ULONG_MAX) != TRUE )
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else if( dataType == VG_MUPBSYNC_DATATYPE_PHONEBOOK )
                {
                    /* Test that current selected phonebook is ADN_APP or ADN_GLB*/
                    if(     vgLmData->phoneBook == DIAL_LIST_ADN_APP ||
                            vgLmData->phoneBook == DIAL_LIST_ADN_GLB)
                    {
                        /* Need reading the synchronisation status to get phonebooks synchronisation information*/
                        generalContext_p->vgLmData.vgMupbsyncContext.operation = EXTENDED_ASSIGN;
                        result = vgChManContinueAction (entity, SIG_APEX_LM_GET_SYNC_STATUS_REQ);
                    }
                    else
                    {
                        result = VG_CME_OPERATION_NOT_ALLOWED;
                    }
                }
                else if( dataType == VG_MUPBSYNC_DATATYPE_RECORD )
                {
                    if(     vgLmData->phoneBook == DIAL_LIST_ADN_APP ||
                            vgLmData->phoneBook == DIAL_LIST_ADN_GLB)
                    {
                        /*Try to read the index*/
                        if ( getExtendedParameterPresent (commandBuffer_p, &index, 0, &present) != TRUE )
                        {
                            result = VG_CME_INVALID_INPUT_VALUE;
                        }
                        else if( !present)
                        {
                            /* start to read all the records UID*/
                            generalContext_p->vgLmData.vgMupbsyncContext.uidReadMode    = LM_READ_FIRST;
                            generalContext_p->vgLmData.vgMupbsyncContext.uidIndex       = 0;
                            result = vgChManContinueAction (entity, SIG_APEX_LM_READ_RECORD_UID_REQ);
                        }
                        else if( index>0 && index<0xFFFF) /* index is coded on 16 bits and can't be 0*/
                        {
                            /* Ask for precise index*/
                            generalContext_p->vgLmData.vgMupbsyncContext.uidReadMode    = LM_READ_ABSOLUTE;
                            generalContext_p->vgLmData.vgMupbsyncContext.uidIndex       = (LmRecordNumber)index;
                            result = vgChManContinueAction (entity, SIG_APEX_LM_READ_RECORD_UID_REQ);
                        }
                        else
                        {
                            /*Invalid index*/
                            result = VG_CME_INVALID_INPUT_VALUE;
                        }
                    }
                    else
                    {
                        result = VG_CME_OPERATION_NOT_ALLOWED;
                    }
                }
                else
                {
                    /*Invalid mode*/
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
            }
            break;

        case EXTENDED_QUERY:  /* AT*MUPBSYNC? */
            {
                /* Need reading the synchronisation status to get phonebooks synchronisation information*/
                generalContext_p->vgLmData.vgMupbsyncContext.operation = EXTENDED_QUERY;
                result = vgChManContinueAction (entity, SIG_APEX_LM_GET_SYNC_STATUS_REQ);
            }
            break;

        case EXTENDED_ACTION: /* AT*MUPBSYNC... */
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
* Function:    vgMUpbhKey
*
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT*MUPBHKEY command, which
*              verifies or modifies Hidden Key.
*-------------------------------------------------------------------------*/

ResultCode_t vgMUpbhKey (CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
  ResultCode_t             result = RESULT_CODE_OK;
  ExtendedOperation_t      operation = getExtendedOperation (commandBuffer_p);
  GeneralContext_t         *generalContext_p = ptrToGeneralContext (entity);
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
#if defined(UPGRADE_3G)
  Int32                    mode = 0;
  Char                     confirmNewHiddenKeyString[HIDDEN_KEY_INOUT_STRING_LENGTH + NULL_TERMINATOR_LENGTH];
  Int16                    stringLength = 0;
#endif
  FatalCheck(generalContext_p != PNULL, entity, 0, 0);

  memset(generalContext_p->hiddenKey, 0, HIDDEN_KEY_INOUT_STRING_LENGTH + NULL_TERMINATOR_LENGTH);
  memset(generalContext_p->newHiddenKey, 0, HIDDEN_KEY_INOUT_STRING_LENGTH + NULL_TERMINATOR_LENGTH);

  switch (operation)
  {
    case EXTENDED_RANGE:  /* AT*MUPBHKEY=? */

#if defined(UPGRADE_3G)
      if (simLockGenericContext_p->simInfo.cardIsUicc == TRUE)
      {
        vgPutNewLine(entity);
        vgPrintf(entity, (const Char*)"*MUPBHKEY: (0-1)");
        vgPutNewLine(entity);
      }
      else
#endif
      {
        result = VG_CME_OPERATION_NOT_ALLOWED;
      }

      break;

    case EXTENDED_ASSIGN: /* AT*MUPBHKEY= */
#if defined(UPGRADE_3G)
      if (simLockGenericContext_p->simInfo.cardIsUicc == TRUE)
      {
        if(getExtendedParameter (
             commandBuffer_p,
              &mode,
               (Int32)NUM_OF_VG_HIDDEN_KEY_MODE) == TRUE)

        {
          switch(mode)
          {
            case VG_VERIFY_HIDDEN_KEY:
              if ((getExtendedString (
                    commandBuffer_p,
                     generalContext_p->hiddenKey,
                      HIDDEN_KEY_INOUT_STRING_LENGTH,
                       &stringLength)))
              {
                generalContext_p->hiddenKeyFunction = SIM_HIDDEN_KEY_VERIFY;
                result = vgChManContinueAction (entity, SIG_APEX_LM_HIDDEN_KEY_FUNCTION_REQ);
              }
              else
              {
                result = RESULT_CODE_ERROR;
              }
              break;

            case VG_CHANGE_HIDDEN_KEY:
              if ((getExtendedString (
                    commandBuffer_p,
                     generalContext_p->hiddenKey,
                      HIDDEN_KEY_INOUT_STRING_LENGTH,
                       &stringLength)) &&

                    (getExtendedString (
                      commandBuffer_p,
                       generalContext_p->newHiddenKey,
                        HIDDEN_KEY_INOUT_STRING_LENGTH,
                         &stringLength)) &&

                    (getExtendedString (
                      commandBuffer_p,
                       confirmNewHiddenKeyString,
                        HIDDEN_KEY_INOUT_STRING_LENGTH,
                         &stringLength)) )
              {
                if(vgStrCmp(generalContext_p->newHiddenKey, confirmNewHiddenKeyString))
                {
                  result = VG_CME_INCORRECT_PASSWORD;
                }
                else
                {
                  generalContext_p->hiddenKeyFunction = SIM_HIDDEN_KEY_CHANGE;
                  result = vgChManContinueAction (entity, SIG_APEX_LM_HIDDEN_KEY_FUNCTION_REQ);
                }
              }
              else
              {
                result = RESULT_CODE_ERROR;
              }

              break;

           case NUM_OF_VG_HIDDEN_KEY_MODE:
             default:
             {
               result = RESULT_CODE_ERROR;
             }

             break;
          }
        }
        else
        {
          result = RESULT_CODE_ERROR;
        }
      }
      else
#endif
      {
        result = VG_CME_OPERATION_NOT_ALLOWED;
      }
      break;

    case EXTENDED_QUERY:    /* AT*MUPBHKEY?  */
    case EXTENDED_ACTION:   /* AT*MUPBHKEY*/
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
 * Function:        vgGnMupbcfg
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     Non-standard AT command to enable/disable the extended
 *                  parameters list for +CPBR and +CPBW commands
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgGnMupbcfg(CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
    ResultCode_t        result = RESULT_CODE_OK;
    Int32               mode;
    GeneralContext_t   *generalContext_p = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    switch (operation)
    {

        case EXTENDED_RANGE:  /* AT*MUPBCFG=? */
            {
                if( (vgLmData_p->phoneBook == DIAL_LIST_ADN_APP)||
                    (vgLmData_p->phoneBook == DIAL_LIST_ADN_GLB))
                {
                    /* Ask for the phonebook status to get the extended parameters number limit*/
                    result = vgChManContinueAction (entity, SIG_APEX_LM_PHONEBOOK_STATUS_REQ);
                }
                else
                {
                    result = VG_CME_OPERATION_NOT_ALLOWED;
                }
            }
            break;

        case EXTENDED_ASSIGN: /* AT*MUPBCFG=... */
            {
                /* User want to configure unsollicited messages*/
                if( (getExtendedParameter (commandBuffer_p, &mode, ULONG_MAX) != TRUE) ||
                    (mode >= VG_MUPBCFG_EXT_PARAM_INVALID))
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                    result = setProfileValue( entity, PROF_MUPBCFG, (Int8)mode);
                }
            }
            break;

        case EXTENDED_QUERY:  /* AT*MUPBCFG? */
            {
                vgPutNewLine (entity);
                vgPrintf(   entity,
                            (const Char*)"%C: %d",
                            getProfileValue(entity, PROF_MUPBCFG) );
                vgPutNewLine (entity);
            }
            break;

        case EXTENDED_ACTION: /* AT*MUPBCFG... */
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
 * Function:        vgGnMupbgas
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     Non-standard AT command to manage the group alpha records (GAS)
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgGnMupbgas(CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ExtendedOperation_t operation               = getExtendedOperation (commandBuffer_p);
    ResultCode_t        result                  = RESULT_CODE_PROCEEDING;
    Int32               mode                    = 0;
    Int32               index                   = 0;
    GeneralContext_t   *generalContext_p        = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p              = PNULL;
    Int8               *tempInputBuf            = PNULL;
    Int16               alphaIdLength           = 0;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    switch (operation)
    {

        case EXTENDED_RANGE:  /* AT*MUPBGAS=? */
            {
                if( (vgLmData_p->phoneBook == DIAL_LIST_ADN_APP)||
                    (vgLmData_p->phoneBook == DIAL_LIST_ADN_GLB))
                {
                    /* Ask for the phonebook status*/
                    vgLmData_p->vgMupbgasContext.mupbgasMode = VG_MUPBGAS_MODE_RANGE;
                    result = vgChManContinueAction (entity, SIG_APEX_LM_PHONEBOOK_STATUS_REQ);
                }
                else
                {
                    result = VG_CME_OPERATION_NOT_ALLOWED;
                }
            }
            break;

        case EXTENDED_ASSIGN: /* AT*MUPBGAS=... */
            {
                if( (vgLmData_p->phoneBook != DIAL_LIST_ADN_APP) &&
                    (vgLmData_p->phoneBook != DIAL_LIST_ADN_GLB))
                {
                    result = VG_CME_OPERATION_NOT_ALLOWED;
                }

                /* Read mode*/
                if( result == RESULT_CODE_PROCEEDING)
                {
                    if( (getExtendedParameter (commandBuffer_p, &mode, ULONG_MAX) != TRUE) ||
                        (mode >= VG_MUPBGAS_MODE_INVALID) ||
                        (mode == 0))
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
                    }
                    else
                    {
                        vgLmData_p->vgMupbgasContext.mupbgasMode = (VgMupbgasModeValue)mode;
                    }
                }

                /* Read index parameter*/
                if( (result == RESULT_CODE_PROCEEDING) &&
                    (   (vgLmData_p->vgMupbgasContext.mupbgasMode == VG_MUPBGAS_MODE_READ) ||
                        (vgLmData_p->vgMupbgasContext.mupbgasMode == VG_MUPBGAS_MODE_WRITE) ) )
                {

                    if( (getExtendedParameter (commandBuffer_p, &index, ULONG_MAX) != TRUE) ||
                        (index > 0xFF) ||
                        (index == 0))
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
                    }
                    else
                    {
                        vgLmData_p->grpInfo.grpData.grpList[0] = (Int8)index;
                    }
                }

                /* Read GAS parameter*/
                if( (result == RESULT_CODE_PROCEEDING) &&
                    (vgLmData_p->vgMupbgasContext.mupbgasMode == VG_MUPBGAS_MODE_WRITE) )
                {
                    /* Allocate input buffer (Only needed for write operations)*/
                    KiAllocMemory ((Int16)(MAX_INPUT_SIM_ALPHA_ID_SIZE + NULL_TERMINATOR_LENGTH),
                                    (void **)&tempInputBuf);

                    if( getExtendedString (commandBuffer_p,
                                            tempInputBuf,
                                            vgGetMaxAlphaIdSize((VgCSCSMode)getProfileValue(entity, PROF_CSCS)),
                                            &alphaIdLength) == FALSE)
                    {
                        /* malformed alpha string*/
                        result = RESULT_CODE_ERROR;
                    }
                    else
                    {
                        /* Convert alpha Id from current TE set */
                        vgLmData_p->grpInfo.grpAlphaId.length
                            = vgMapTEToAlphaId( vgLmData_p->grpInfo.grpAlphaId.data,
                                                SIM_ALPHA_ID_SIZE,
                                                tempInputBuf,
                                                alphaIdLength,
                                                (VgCSCSMode)getProfileValue(entity, PROF_CSCS),
                                                entity);
                    }
                    KiFreeMemory( (void **)&tempInputBuf);
                }

                if( result == RESULT_CODE_PROCEEDING)
                {
                    /* First, to be able to continue, ask for the phonebook status*/
                    result = vgChManContinueAction (entity, SIG_APEX_LM_PHONEBOOK_STATUS_REQ);
                }
            }
            break;

        case EXTENDED_QUERY:  /* AT*MUPBGAS? */
            {
                if( (vgLmData_p->phoneBook == DIAL_LIST_ADN_APP) ||
                    (vgLmData_p->phoneBook == DIAL_LIST_ADN_GLB))
                {
                    vgLmData_p->startRecord = 1;
                    result = vgChManContinueAction (entity, SIG_APEX_LM_LIST_GAS_REQ);
                }
                else
                {
                    result = VG_CME_OPERATION_NOT_ALLOWED;
                }
            }
            break;

        case EXTENDED_ACTION: /* AT*MUPBGAS... */
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
 * Function:        vgGnMupbaas
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     Non-standard AT command to manage the addtional number
 *                  alpha string records (AAS)
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgGnMupbaas(CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ExtendedOperation_t operation               = getExtendedOperation (commandBuffer_p);
    ResultCode_t        result                  = RESULT_CODE_PROCEEDING;
    Int32               mode                    = 0;
    Int32               index                   = 0;
    GeneralContext_t   *generalContext_p        = ptrToGeneralContext (entity);
    VgLmData           *vgLmData_p              = PNULL;
    Int8               *tempInputBuf            = PNULL;
    Int16               alphaIdLength           = 0;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    switch (operation)
    {

        case EXTENDED_RANGE:  /* AT*MUPBAAS=? */
            {
                if( (vgLmData_p->phoneBook == DIAL_LIST_ADN_APP)||
                    (vgLmData_p->phoneBook == DIAL_LIST_ADN_GLB))
                {
                    /* Ask for the phonebook status*/
                    vgLmData_p->vgMupbaasContext.mupbaasMode = VG_MUPBAAS_MODE_RANGE;
                    result = vgChManContinueAction (entity, SIG_APEX_LM_PHONEBOOK_STATUS_REQ);
                }
                else
                {
                    result = VG_CME_OPERATION_NOT_ALLOWED;
                }
            }
            break;

        case EXTENDED_ASSIGN: /* AT*MUPBAAS=... */
            {
                if( (vgLmData_p->phoneBook != DIAL_LIST_ADN_APP) &&
                    (vgLmData_p->phoneBook != DIAL_LIST_ADN_GLB))
                {
                    result = VG_CME_OPERATION_NOT_ALLOWED;
                }

                /* Read mode*/
                if( result == RESULT_CODE_PROCEEDING)
                {
                    if( (getExtendedParameter (commandBuffer_p, &mode, ULONG_MAX) != TRUE) ||
                        (mode >= VG_MUPBAAS_MODE_INVALID) ||
                        (mode == 0))
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
                    }
                    else
                    {
                        vgLmData_p->vgMupbaasContext.mupbaasMode = (VgMupbaasModeValue)mode;
                    }
                }

                /* Read index parameter*/
                if( (result == RESULT_CODE_PROCEEDING) &&
                    (   (vgLmData_p->vgMupbaasContext.mupbaasMode == VG_MUPBAAS_MODE_READ) ||
                        (vgLmData_p->vgMupbaasContext.mupbaasMode == VG_MUPBAAS_MODE_WRITE) ) )
                {

                    if( (getExtendedParameter (commandBuffer_p, &index, ULONG_MAX) != TRUE) ||
                        (index > 0xFF) ||
                        (index == 0))
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
                    }
                    else
                    {
                        vgLmData_p->aasInfo.aasIndex = (Int8)index;
                    }
                }

                /* Read AAS parameter*/
                if( (result == RESULT_CODE_PROCEEDING) &&
                    (vgLmData_p->vgMupbaasContext.mupbaasMode == VG_MUPBAAS_MODE_WRITE) )
                {
                    /* Allocate input buffer (Only needed for write operations)*/
                    KiAllocMemory ((Int16)MAX_INPUT_SIM_ALPHA_ID_SIZE + NULL_TERMINATOR_LENGTH,
                                    (void **)&tempInputBuf);

                    if( getExtendedString (commandBuffer_p,
                                            tempInputBuf,
                                            vgGetMaxAlphaIdSize( (VgCSCSMode)getProfileValue(entity, PROF_CSCS)),
                                            &alphaIdLength) == FALSE)
                    {
                        /* malformed alpha string*/
                        result = RESULT_CODE_ERROR;
                    }
                    else
                    {
                        /* Convert alpha Id from current TE set */
                        vgLmData_p->aasInfo.aasAlphaId.length
                            = vgMapTEToAlphaId( vgLmData_p->aasInfo.aasAlphaId.data,
                                                SIM_ALPHA_ID_SIZE,
                                                tempInputBuf,
                                                alphaIdLength,
                                                (VgCSCSMode)getProfileValue(entity, PROF_CSCS),
                                                entity);
                    }
                    KiFreeMemory( (void **)&tempInputBuf);
                }

                if( result == RESULT_CODE_PROCEEDING)
                {
                    /* First, to be able to continue, ask for the phonebook status*/
                    result = vgChManContinueAction (entity, SIG_APEX_LM_PHONEBOOK_STATUS_REQ);
                }
            }
            break;

        case EXTENDED_QUERY:  /* AT*MUPBAAS? */
            {
                if( (vgLmData_p->phoneBook == DIAL_LIST_ADN_APP) ||
                    (vgLmData_p->phoneBook == DIAL_LIST_ADN_GLB))
                {
                    vgLmData_p->startRecord = 1;
                    vgLmData_p->vgMupbaasContext.mupbaasMode = VG_MUPBAAS_MODE_LIST;
                    result = vgChManContinueAction (entity, SIG_APEX_LM_LIST_AAS_REQ);
                }
                else
                {
                    result = VG_CME_OPERATION_NOT_ALLOWED;
                }
            }
            break;

        case EXTENDED_ACTION: /* AT*MUPBAAS... */
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
* Function:    vgGnSCPBW
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT^SCPBW command which can create
*              and delete phonebook entries.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgGnSCPBW ( CommandLine_t *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
    ResultCode_t                result                  = RESULT_CODE_PROCEEDING;
    ExtendedOperation_t         operation               = getExtendedOperation (commandBuffer_p);
    GeneralContext_t           *generalContext_p        = ptrToGeneralContext (entity);
    GeneralGenericContext_t    *generalGenericContext_p = ptrToGeneralGenericContext();
    VgLmData                   *vgLmData                = PNULL;
    Int32                       index                   = 0;
    Boolean                     present                 = FALSE;
    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData = &generalContext_p->vgLmData;

    switch (operation)
    {
        case EXTENDED_RANGE:  /* AT+SCPBW=? */
        {

            if( (vgLmData->phoneBook == DIAL_LIST_ADN_APP)||
                (vgLmData->phoneBook == DIAL_LIST_ADN_GLB))
            {
                vgLmData->phoneBookOperation = VG_PB_RANGE;
                result = vgChManContinueAction (entity, SIG_APEX_LM_PHONEBOOK_STATUS_REQ);
            }
            else
            {
                /* SCPBW only works for phonebooks SM or ME*/
                result = VG_CME_OPERATION_NOT_ALLOWED;
            }
        }
        break;

        case EXTENDED_ASSIGN: /* AT+SCPBW=... */
        {
            if( (vgLmData->phoneBook != DIAL_LIST_ADN_APP) &&
                (vgLmData->phoneBook != DIAL_LIST_ADN_GLB) )
            {
                /* SCPBW only works for phonebooks SM or ME*/
                result = VG_CME_OPERATION_NOT_ALLOWED;
            }
            else
            {
                /* get phone book index and dial string */
                if( ( getExtendedParameterPresent(  commandBuffer_p,
                                                    &index,
                                                    USHRT_MAX,
                                                    &present) == FALSE) ||
                    ( getExtendedString(    commandBuffer_p,
                                            vgLmData->writeNum,
                                            MAX_CALLED_BCD_NO_LENGTH,
                                            &vgLmData->writeNumLength) == FALSE) )
                {
                    /* invalid parameters entered */
                    result = RESULT_CODE_ERROR;
                }
            }

            /* Process the index parameter*/
            if( result == RESULT_CODE_PROCEEDING)
            {
                if( present == FALSE)
                {
                    vgLmData->phoneIndex1 = 0;
                    vgLmData->phoneBookOperation = VG_PB_WRITE;
                }
                else if(index == 0)
                {
                    result = VG_CME_INVALID_INDEX;
                }
                else
                {
                    vgLmData->phoneIndex1 = (LmRecordNumber)index;
                    /* if dialstring of zero length then operation is to delete entry */
                    if (vgLmData->writeNumLength == 0)
                    {
                        vgLmData->phoneBookOperation = VG_PB_DELETE;
                    }
                    else /* writing to absolute index */
                    {
                        vgLmData->phoneBookOperation = VG_PB_WRITE;
                    }
                }
            }

            /* For WRITE request, continue to proccess arguments*/
            if( (result == RESULT_CODE_PROCEEDING) &&
                (vgLmData->phoneBookOperation == VG_PB_WRITE) )
            {

                result = vgGnProcessScpbwParams( commandBuffer_p, entity);
            }

            /* send write request off if parameters check out */
            if( result == RESULT_CODE_PROCEEDING)
            {
                FatalCheck( (vgLmData->phoneBook == DIAL_LIST_ADN_GLB) ||
                            (vgLmData->phoneBook == DIAL_LIST_ADN_APP),
                            vgLmData->phoneBook, 0, 0);
                result = vgChManContinueAction (entity, SIG_APEX_LM_PHONEBOOK_STATUS_REQ);
            }
        }
        break;

        case EXTENDED_QUERY:  /* AT+CPBW? */
        case EXTENDED_ACTION: /* AT+CPBW... */
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
 * Function:    vgGnProcessScpbwParams
 *
 * Parameters:
 *
 * Returns:    Result code
 *
 * Description: The function is called to read and process parameters of SCPBW command.
 *-------------------------------------------------------------------------*/
ResultCode_t vgGnProcessScpbwParams(    CommandLine_t *commandBuffer_p,
                                        const VgmuxChannelNumber entity)
{
    ResultCode_t        result                  = RESULT_CODE_PROCEEDING;
    GeneralContext_t   *generalContext_p        = ptrToGeneralContext (entity);
    Int16               alphaIdLength           = 0;
    Int8               *tempInputBuf_p          = PNULL;
    VgLmData           *vgLmData_p              = PNULL;
    Int32               coding                  = 0;
    Int32               phoneBookNumType;
    Int16               emailIdLength;
    Int8                i;
    VgCSCSMode          cscsMode                = VG_AT_CSCS_GSM;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData_p = &generalContext_p->vgLmData;

    KiAllocMemory( (Int16)VG_CPBW_INPUT_BUFFER_SIZE,
                 (void **)&tempInputBuf_p);

    vgLmData_p->phoneBookNumTypePresent = FALSE;
    vgLmData_p->grpInfo.grpAlphaId.length = 0;
    vgLmData_p->secondName.length = 0;
    vgLmData_p->hiddenEntry = FALSE;

    if( vgLmData_p->writeNumLength == 0)
    {
        result = RESULT_CODE_ERROR;
    }
    else if( vgCheckTextBcdString(  vgLmData_p->writeNum,
                                    vgLmData_p->writeNumLength) == FALSE)
    {
        result = VG_CME_INVALID_DIALSTRING_CHARS;
    }
    else if( getExtendedParameter(  commandBuffer_p,
                                    &phoneBookNumType,
                                    VG_DIAL_NUMBER_UNKNOWN) == FALSE)
    {
        /* Invalid phone number type*/
        result = RESULT_CODE_ERROR;
    }
    else
    {
        /* Process the dial string argument*/
        switch (phoneBookNumType)
        {
            case VG_DIAL_NUMBER_INTERNATIONAL: /* international number */
            {
                /* if preceeding '+' not present then add it */
                if (vgLmData_p->writeNum[0] != INTERNATIONAL_PREFIX)
                {
                    if (vgLmData_p->writeNumLength < MAX_CALLED_BCD_NO_LENGTH)
                    {
                        tempInputBuf_p[0] = INTERNATIONAL_PREFIX;
                        memcpy( &tempInputBuf_p[1],
                                vgLmData_p->writeNum,
                                vgLmData_p->writeNumLength);
                        memcpy( vgLmData_p->writeNum,
                                tempInputBuf_p,
                                (vgLmData_p->writeNumLength + 1));
                        vgLmData_p->writeNumLength++;
                    }
                    else
                    {
                        result = RESULT_CODE_ERROR;
                    }
                }
                vgLmData_p->phoneBookNumType = NUM_TYPE_INTERNATIONAL;
                vgLmData_p->phoneBookNumTypePresent = TRUE;
            }
            break;

            case VG_DIAL_NUMBER_UNKNOWN:
            case VG_DIAL_NUMBER_NET_SPECIFIC:
            case VG_DIAL_NUMBER_NATIONAL: /* non-international number */
            {
                /* if preceeding '+' present then remove it */
                if (vgLmData_p->writeNum[0] == INTERNATIONAL_PREFIX)
                {
                    memcpy( tempInputBuf_p,
                            vgLmData_p->writeNum,
                            vgLmData_p->writeNumLength);
                    memset( vgLmData_p->writeNum,
                            ' ',
                            vgLmData_p->writeNumLength);
                    memcpy( vgLmData_p->writeNum,
                            &tempInputBuf_p[1],
                            (vgLmData_p->writeNumLength - 1));
                    vgLmData_p->writeNumLength--;

                    vgLmData_p->phoneBookNumType = NUM_TYPE_INTERNATIONAL;
                }
                else
                {
                    vgLmData_p->phoneBookNumType =
                        vgCharToBcdNumberType((VgDialNumberType) phoneBookNumType);
                }
                vgLmData_p->phoneBookNumTypePresent = TRUE;
            }
            break;

            default: /* an invalid phone number type has been entered */
            {
                result = RESULT_CODE_ERROR;
            }
            break;
        }
    }

    /* Process first additional number parameters*/
    if( result == RESULT_CODE_PROCEEDING)
    {
        result = vgGnReadNumberParams(  commandBuffer_p,
                                        entity,
                                        &vgLmData_p->adNumInfo.adNum,
                                        tempInputBuf_p);
    }

    /* Process nexts additional number parameters*/
    /* -1 because we already read an additional parameter rigth before*/
    for(    i=0;
            (   (i<VG_SCPBW_NUM_AD_NUMBER - 1) &&
                (result==RESULT_CODE_PROCEEDING) );
            i++)
    {
        result = vgGnReadNumberParams(  commandBuffer_p,
                                        entity,
                                        &vgLmData_p->adNumInfo.adAdNums[ i],
                                        tempInputBuf_p);
    }

    /* Process alpha string parameter*/
    if( result == RESULT_CODE_PROCEEDING)
    {
        if( getExtendedString(  commandBuffer_p,
                                tempInputBuf_p,
                                vgGetMaxAlphaIdSize( (VgCSCSMode)getProfileValue(entity, PROF_CSCS)),
                                &alphaIdLength) == FALSE)
        {
            /* malformed alpha string*/
            result = RESULT_CODE_ERROR;
        }
    }

    /* Process the coding parameter*/
    if( result == RESULT_CODE_PROCEEDING)
    {
        if( (getExtendedParameter(  commandBuffer_p,
                                    &coding,
                                    0) == FALSE) ||
            (coding >= VG_SCPBW_CODING_END))
        {
            result = RESULT_CODE_ERROR;
        }
        else
        {
            /* Decode the alpha string using the given coding format*/
            switch( coding)
            {
                case VG_SCPBW_CODING_GSM:
                {
                    cscsMode = VG_AT_CSCS_GSM;
                }
                break;

                case VG_SCPBW_CODING_RAW:
                {
                    cscsMode = (VgCSCSMode)getProfileValue(entity, PROF_CSCS);
                }
                break;

                default:
                {
                    FatalParam( coding, 0, 0);
                }
                break;
            }

            /* Convert alpha Id from current TE set */
            vgLmData_p->alphaLength
                = vgMapTEToAlphaId( vgLmData_p->alpha,
                                    SIM_ALPHA_ID_SIZE,
                                    tempInputBuf_p,
                                    alphaIdLength,
                                    cscsMode,
                                    entity);
        }
    }

    /* Process email information*/
    if( result == RESULT_CODE_PROCEEDING)
    {
        if( getExtendedString(  commandBuffer_p,
                                &tempInputBuf_p[0],
                                MAX_INPUT_SIM_EMAIL_ADD_SIZE,
                                &emailIdLength) == FALSE)
        {
            /* malformed string*/
            result = RESULT_CODE_ERROR;
        }
        else
        {
            vgLmData_p->emailInfo.email.length
                = vgMapTEToAlphaId( vgLmData_p->emailInfo.email.data,
                                    SIM_UICC_EMAIL_ADDRESS_SIZE,
                                    &tempInputBuf_p[0],
                                    emailIdLength,
                                    (VgCSCSMode)getProfileValue(entity, PROF_CSCS),
                                    entity);
        }
    }

    /* Release the input buffer memory*/
    KiFreeMemory ((void **)&tempInputBuf_p);

    return result;
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgGnProcessNumberParams
 *
 * Parameters: [Out] num_p      - Structure into which the phone number will be wrote
 *             tempInputBuf_p   - Just a temporary buffer
 *
 * Returns:    Result code
 *
 * Description: The function is called to read an phone number + type parameters
 *-------------------------------------------------------------------------*/
ResultCode_t vgGnReadNumberParams(  CommandLine_t              *commandBuffer_p,
                                    const VgmuxChannelNumber    entity,
                                    VgSimDialNum               *num_p,
                                    Int8                       *tempInputBuf_p)
{
    ResultCode_t    result              = RESULT_CODE_PROCEEDING;
    Int32           phoneBookNumType;

    if( (getExtendedString( commandBuffer_p,
                            num_p->dialNum,
                            MAX_CALLED_BCD_NO_LENGTH,
                            &num_p->dialNumLength) == FALSE) ||
        (getExtendedParameter(  commandBuffer_p,
                                &phoneBookNumType,
                                VG_DIAL_NUMBER_UNKNOWN) == FALSE) )
    {
        /* Invalid phone number string and type*/
        result = RESULT_CODE_ERROR;
    }
    else if( vgCheckTextBcdString(  num_p->dialNum,
                                    num_p->dialNumLength) == FALSE)
    {
      result = VG_CME_INVALID_DIALSTRING_CHARS;
    }
    else if (num_p->dialNumLength == 0)
    {
      result = RESULT_CODE_ERROR;    
    }  
    else
    {
        switch (phoneBookNumType)
        {
            case VG_DIAL_NUMBER_INTERNATIONAL: /* international number */
            {
                /* if preceeding '+' not present then add it */
                if (num_p->dialNum[0] != INTERNATIONAL_PREFIX)
                {
                    if (num_p->dialNumLength < MAX_CALLED_BCD_NO_LENGTH)
                    {
                        tempInputBuf_p[0] = INTERNATIONAL_PREFIX;
                        memcpy( &tempInputBuf_p[1],
                                num_p->dialNum,
                                num_p->dialNumLength);
                        memcpy( num_p->dialNum,
                                tempInputBuf_p,
                                (num_p->dialNumLength + 1));
                        num_p->dialNumLength++;
                    }
                    else
                    {
                        result = RESULT_CODE_ERROR;
                    }
                }
                num_p->dialNumType = NUM_TYPE_INTERNATIONAL;
            }
            break;

            case VG_DIAL_NUMBER_UNKNOWN:
            case VG_DIAL_NUMBER_NET_SPECIFIC:
            case VG_DIAL_NUMBER_NATIONAL: /* non-international number */
            {
                /* if preceeding '+' present then remove it */
                if (num_p->dialNum[0] == INTERNATIONAL_PREFIX)
                {
                    memcpy( tempInputBuf_p,
                            num_p->dialNum,
                            num_p->dialNumLength);
                    memset( num_p->dialNum,
                            ' ',
                            num_p->dialNumLength);
                    memcpy( num_p->dialNum,
                            &tempInputBuf_p[1],
                            (num_p->dialNumLength - 1));
                    num_p->dialNumLength--;

                    num_p->dialNumType = NUM_TYPE_INTERNATIONAL;
                }
                else
                {
                    num_p->dialNumType =
                        vgCharToBcdNumberType((VgDialNumberType) phoneBookNumType );
                }
            }
            break;

            default: /* an invalid phone number type has been entered */
            {
                result = RESULT_CODE_ERROR;
            }
            break;
        }
    }

    return result;
}

/*--------------------------------------------------------------------------
*
* Function:    vgGnSCPBR
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+SCPBR command which reads an
*              selected phonebook entry
*
*-------------------------------------------------------------------------*/
ResultCode_t vgGnSCPBR( CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
    ResultCode_t                result                  = RESULT_CODE_ERROR;
    ExtendedOperation_t         operation               = getExtendedOperation (commandBuffer_p);
    Int32                       param1;
    Int32                       param2;
    GeneralContext_t           *generalContext_p        = ptrToGeneralContext (entity);
    GeneralGenericContext_t    *generalGenericContext_p = ptrToGeneralGenericContext();
    VgLmData                   *vgLmData;

    FatalCheck(generalContext_p != PNULL, entity, 0, 0);

    vgLmData = &generalContext_p->vgLmData;

    switch (operation)
    {
        case EXTENDED_RANGE:  /* AT+SCPBR=? */
        {
            vgLmData->phoneBookOperation = VG_PB_RANGE;

            if( (vgLmData->phoneBook == DIAL_LIST_ADN_APP)||
                (vgLmData->phoneBook == DIAL_LIST_ADN_GLB))
            {
                result = vgChManContinueAction (entity, SIG_APEX_LM_PHONEBOOK_STATUS_REQ);
            }
            else
            {
                /* SCPBR only works for phonebooks SM or ME*/
                result = VG_CME_OPERATION_NOT_ALLOWED;
            }
        }
        break;

        case EXTENDED_ASSIGN: /* AT+SCPBR=... */
        {
            if( (vgLmData->phoneBook != DIAL_LIST_ADN_APP) &&
                (vgLmData->phoneBook != DIAL_LIST_ADN_GLB) )
            {
                /* SCPBW only works for phonebooks SM or ME*/
                result = VG_CME_OPERATION_NOT_ALLOWED;
            }
            else
            {
                /* get lower bound phone book entry index */
                if (getExtendedParameter (commandBuffer_p, &param1, 0) == TRUE)
                {
                    /* get upper bound phone book entry index */
                    if (getExtendedParameter (commandBuffer_p, &param2, param1) == TRUE)
                    {
                        if( (param1 > 0) &&
                            (param2 >= param1))
                        {
                            vgLmData->phoneIndex1 = (LmRecordNumber)param1;
                            vgLmData->phoneIndex2 = (LmRecordNumber)param2;
                            vgLmData->currentLnaRecord = 0;

                            if (vgLmData->phoneIndex2 == vgLmData->phoneIndex1)
                            {
                                /* read the single specified phonebook entry */
                                vgLmData->phoneIndex2 = 0;
                                vgLmData->readMode = LM_READ_ABSOLUTE;
                            }
                            else
                            {
                                /* read all phone book entries between lower and upper entry
                                * indexes */
                                vgLmData->phoneIndex1--;
                                vgLmData->currentLnaRecord = vgLmData->phoneIndex1;
                                vgLmData->readMode = LM_READ_NEXT;
                            }

                            vgLmData->phoneBookOperation = VG_PB_READ;

                            FatalCheck( (vgLmData->phoneBook == DIAL_LIST_ADN_GLB) ||
                                        (vgLmData->phoneBook == DIAL_LIST_ADN_APP),
                                        vgLmData->phoneBook, 0, 0);
                            result = vgChManContinueAction (entity, SIG_APEX_LM_PHONEBOOK_STATUS_REQ);
                        }
                        else
                        {
                            result = VG_CME_INVALID_INDEX;
                        }
                    }
                }
            }
        }
        break;

        case EXTENDED_QUERY:  /* AT+SCPBR? */
        case EXTENDED_ACTION: /* AT+SCPBR... */
        default:
        {
        }
        break;
    }

    return (result);
}

#endif /* FEA_PHONEBOOK */

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */







