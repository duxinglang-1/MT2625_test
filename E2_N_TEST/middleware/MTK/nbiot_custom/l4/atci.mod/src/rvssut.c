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
 * Procedures for Supplementary Services AT command execution
 *
 * Contains implementations of the following AT commands:
 *
 *  AT+CPWD:  Change password.
 *  AT+CLCK:  Facility lock.
 **************************************************************************/

#define MODULE_NAME "RVSSUT"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvprof.h>
#include <rvssut.h>
#include <rvssdata.h>
#include <rvchman.h>
#include <rvutil.h>
#include <rvcrhand.h>
#include <rvccut.h>
#include <rvsldata.h>
#include <rvcrerr.h>

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/










/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Variables
 ***************************************************************************/

/* Table for fac code lookup containing string, and password max length.... */
static const VgSsNetworkFacilityDescriptor facLookup[] =
{
  { VG_AT_SS_FAC_SC, "SC", SIM_CHV_LENGTH },

#if defined (FEA_PHONEBOOK)
  { VG_AT_SS_FAC_FD, "FD", SIM_CHV_LENGTH },
  { VG_AT_SS_FAC_BD, "BN", SIM_CHV_LENGTH },
#endif /* FEA_PHONEBOOK */

#if defined (FEA_SIMLOCK)
  { VG_AT_SS_FAC_PS, "PS", MAXPASSWORDLEN },
  { VG_AT_SS_FAC_PN, "PN", MAXPASSWORDLEN },
  { VG_AT_SS_FAC_PU, "PU", MAXPASSWORDLEN },
  { VG_AT_SS_FAC_PP, "PP", MAXPASSWORDLEN },
  { VG_AT_SS_FAC_PC, "PC", MAXPASSWORDLEN },
#endif /* FEA_SIMLOCK */

  { VG_AT_SS_FAC_P2, "P2", SIM_CHV_LENGTH }

};

#define NUM_ENTRIES(X) (sizeof(X)/sizeof(X[0]))

#if defined (FEA_SIMLOCK)
static const AbsiMepSelector ssMepSelectors[5] =
{
  SIM_PERSONALISATION,  
  NETWORK_PERSONALISATION,
  NETWORK_SUBSET_PERSONALISATION,
  SERVICE_PROVIDER_PERSONALISATION,
  CORPORATE_PERSONALISATION
};
#endif /* FEA_SIMLOCK */

/***************************************************************************
 * Macros
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/
/*--------------------------------------------------------------------------
*
* Function:     sendPin1Request
*
* Parameters:
*               entity               -   Mux channel number

* Returns:      result code.
*
* Description:  Depending upon the type of card (GSM SIM or UICC) inserted,
 * sends the correct request to verify PIN1.
*-------------------------------------------------------------------------*/
static ResultCode_t sendPin1Request (const VgmuxChannelNumber  entity)
{
  ResultCode_t             result;
  SimLockGenericContext_t  *simLockGenericContext_p = ptrToSimLockGenericContext ();
  GeneralContext_t         *generalContext_p        = ptrToGeneralContext  (entity);
  VgSimInfo                *simInfo                 = &simLockGenericContext_p->simInfo;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  if (simInfo->cardIsUicc == TRUE)
  {
    generalContext_p->keyRef = simInfo->pin1KeyRef;
    result = vgChManContinueAction (entity, SIG_APEX_SIM_PIN_FUNCTION_REQ);
  }
  else
  {
    generalContext_p->chvNumber = SIM_CHV_1;
    result = vgChManContinueAction (entity, SIG_APEX_SIM_CHV_FUNCTION_REQ);
  }

  return (result);
}


/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
*
* Function:     vgSsCPWD
*
* Parameters:   commandBuffer_p - pointer to command line string
*               entity          - mux channel number
*
* Returns:      Resultcode_t    - result of function
*
* Description:  This function executes the AT+CPWD command which sets a
*               new password for the facility lock function defined by command
*               Facility Lock +CLCK.
*-------------------------------------------------------------------------*/

ResultCode_t vgSsCPWD (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t            result = RESULT_CODE_OK;
  Char                    facString[MAX_FAC_ID_LEN + NULL_TERMINATOR_LENGTH] = {0};
  Char                    oldPassword[SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH] = {0};
  Char                    newPassword[SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH] = {0};
  Int16                   index,
                          tmpIndex,
                          simOldPinLen,
                          simNewPinLen;
  Int16                   facStringLength = 0;
  Boolean                 foundFac = FALSE;
  SupplementaryContext_t  *supplementaryContext_p = ptrToSupplementaryContext (entity);
  GeneralContext_t        *generalContext_p       = ptrToGeneralContext       (entity);

  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(supplementaryContext_p != PNULL, entity, 0, 0);
  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  switch( getExtendedOperation(commandBuffer_p) )
  {
    case EXTENDED_RANGE:  /* AT+CPWD=? */
    {
      /* Print out list of faciltities and max lengths of passwords.... */
      vgPutNewLine(entity);
      vgPrintf(entity, (Char *)"+CPWD: ");

      for (index = 0; index < NUM_ENTRIES(facLookup); index++ )
      {
        /* This facility is covered, print out name and password
         * length....
         */

        if (index > 0)
        {
          vgPutc (entity, ',');
        }

        vgPrintf (entity,
                  (Char *)"(\"%s\",%d)",
                  facLookup[index].vgFacStr,
                  facLookup[index].vgPasswordLen);
      }
      vgPutNewLine(entity);

      break;
    }

    case EXTENDED_ASSIGN: /* AT+CPWD= */
    {
      /* Get fac string, oldpasswd and new password.... */
      if( (getExtendedString( commandBuffer_p,
                              &facString[0],
                                MAX_FAC_ID_LEN,
                                 &facStringLength) == TRUE)   &&
          (getExtendedString( commandBuffer_p,
                              &oldPassword[0],
                                SIM_CHV_LENGTH,
                                 &simOldPinLen) == TRUE)      &&
          (getExtendedString( commandBuffer_p,
                              &newPassword[0],
                                SIM_CHV_LENGTH,
                                 &simNewPinLen) == TRUE))
      {
        /* Check entered value corresponds to a known fac.... */
        index = 0;
        while ((index < NUM_ENTRIES(facLookup)) && (foundFac == FALSE))
        {
          if (memcmp(&facLookup[index].vgFacStr[0],
                      &facString[0],
                       facStringLength) == 0)
          {
            /* Matched string! */
            foundFac = TRUE;
          }

          else
          {
            /* Carry on searching.... */
            index++;
          }
        }

        /* If fac code valid, process request.... */
        if ((foundFac == TRUE) && (index < NUM_ENTRIES(facLookup)))
        {
          /* Process fac request.... */
          switch   (facLookup[index].vgFacId)
          {
            /**************************************/
            /* NOTE: For NB-IOT all call related  */
            /* <fac> are not supported            */
            /**************************************/
            case VG_AT_SS_FAC_SC:
            case VG_AT_SS_FAC_P2:
#if defined (FEA_PHONEBOOK)              
            case VG_AT_SS_FAC_BD:
            case VG_AT_SS_FAC_FD:   /* Note FD == P2 */
#endif /* FEA_PHONEBOOK */              
            {
              /* Sim lock/unlock codes.... */
              /* Job 109119: Check minimum Pin lengths as per GSM 11.11 - 9.3
                             and new one contains no alpha characters */
              if ((simOldPinLen >= MIN_CHV_SIZE) &&
                  (simNewPinLen >= MIN_CHV_SIZE) &&
                  (simOldPinLen <= SIM_CHV_LENGTH) &&
                  (simNewPinLen <= SIM_CHV_LENGTH) &&
                  (checkForNumericOnlyChars(newPassword)) &&
                  (checkForAdditionalPassword (commandBuffer_p,
                                                newPassword,
                                                 &simNewPinLen) == TRUE))
              {
                /* Copy password strings and pad out unused chars with 0xFF
                 * (required for SIM passwords)....
                 */
                memcpy(&generalContext_p->password[0], &oldPassword[0], simOldPinLen);
                memset( (Char *)&generalContext_p->password[simOldPinLen],
                        UCHAR_MAX,
                        (SIM_CHV_LENGTH - simOldPinLen));

                memcpy(&generalContext_p->newPassword[0], &newPassword[0], simNewPinLen);
                memset( (Char *)&generalContext_p->newPassword[simNewPinLen],
                        UCHAR_MAX,
                        (SIM_CHV_LENGTH - simNewPinLen));

                /* Initialise & send change request to SIM.... */
                if (facLookup[index].vgFacId == VG_AT_SS_FAC_SC)
                {
                  /* Pin 1.... */
                  if (simLockGenericContext_p->simInfo.cardIsUicc == TRUE)
                  {
                      generalContext_p->keyRef = simLockGenericContext_p->simInfo.pin1KeyRef;
                  }
                  else
                  {
                      generalContext_p->chvNumber = SIM_CHV_1;
                  }
                }

                else
                {
                  /* Pin2 change (FD/P2/BN).... */
                  if (simLockGenericContext_p->simInfo.cardIsUicc == TRUE)
                  {
                      generalContext_p->keyRef = simLockGenericContext_p->simInfo.pin2KeyRef;
                  }
                  else
                  {
                      generalContext_p->chvNumber = SIM_CHV_2;
                  }

                }
                generalContext_p->pinFunction = SIM_PIN_FUNCT_CHANGE;
                if (simLockGenericContext_p->simInfo.cardIsUicc == TRUE)
                {
                    result = vgChManContinueAction (entity, SIG_APEX_SIM_PIN_FUNCTION_REQ);
                }
                else
                {
                    result = vgChManContinueAction (entity, SIG_APEX_SIM_CHV_FUNCTION_REQ);
                }
              }

              else
              {
                /* Password string too long.... */
                result = VG_CME_INCORRECT_PASSWORD;
              }
              break;
            }

#if defined (FEA_SIMLOCK)
            case VG_AT_SS_FAC_PS:
            case VG_AT_SS_FAC_PN:
            case VG_AT_SS_FAC_PU:
            case VG_AT_SS_FAC_PP:
            case VG_AT_SS_FAC_PC:
            {
              tmpIndex = (Int16)(index - VG_AT_SS_FAC_PS);
                  
              FatalAssert (tmpIndex < NUM_ENTRIES(ssMepSelectors));
              
              generalContext_p->mepOperation = CHANGE_PASSWORD;  
              generalContext_p->mepSelector = ssMepSelectors[tmpIndex];
              if ((simOldPinLen == MAXPASSWORDLEN) &&
                  (simNewPinLen == MAXPASSWORDLEN) &&
                  (checkForNumericOnlyChars(newPassword)) &&
                  (checkForAdditionalPassword (commandBuffer_p,
                                                newPassword,
                                                 &simNewPinLen) == TRUE))
              {

                memcpy(&generalContext_p->password[0], &oldPassword[0], simOldPinLen);
                generalContext_p->passwordLength = simOldPinLen;
                memcpy(&generalContext_p->newPassword[0], &newPassword[0], simNewPinLen);
                generalContext_p->newPasswordLength = simNewPinLen;
                
                result = vgChManContinueAction (entity, APEX_SIM_MEP_REQ);
              }
              else
              {
                /* Password string too long.... */
                result = VG_CME_INCORRECT_PASSWORD;
              }
              break;
                
            }
#endif /* FEA_SIMLOCK */

            default:
            {
              /* These are unsupported.... */
              result = VG_CME_INVALID_INPUT_VALUE;
              break;
            }

          }
        }

        else
        {
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }

      else
      {
        result = VG_CME_INVALID_INPUT_VALUE;
      }
      break;
    }

    case EXTENDED_QUERY:  /* AT+CPWD? */
    case EXTENDED_ACTION: /* AT+CPWD */
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
* Function:     vgSsCLCK
*
* Parameters:   commandBuffer_p - pointer to command line string
*               entity          - mux channel number
*
* Returns:      Resultcode_t    - result of function
*
* Description:  Execute command is used to lock, unlock or interrogate a
*               ME or a network facility <fac>. Password is normally
*               needed to do such actions.
*-------------------------------------------------------------------------*/

ResultCode_t vgSsCLCK (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber  entity)
{
  ResultCode_t            result = RESULT_CODE_OK;
  Int32                   index;
  Char                    facString[MAX_FAC_ID_LEN + NULL_TERMINATOR_LENGTH] = {0};
  Char                    password[SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH] = {0};
  Int32                   mode,
                          callClass;
  Boolean                 foundFac = FALSE;
  Int16                   facStringLength = 0,
                          passwordLength = 0;
  SupplementaryContext_t  *supplementaryContext_p = ptrToSupplementaryContext (entity);
  GeneralContext_t        *generalContext_p       = ptrToGeneralContext       (entity);
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(supplementaryContext_p != PNULL, entity, 0, 0);
  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  switch( getExtendedOperation (commandBuffer_p) )
  {
    case EXTENDED_RANGE:  /* AT+CLCK=? */
    {
      /* Print out list of faciltities.... */
      vgPutNewLine(entity);
      vgPrintf(entity, (Char *)"+CLCK: (");

      for (index = 0;
           (index < NUM_ENTRIES(facLookup)) &&
           (facLookup[index].vgFacId != VG_AT_SS_FAC_P2);
            index++ )
      {
        /* print comma seperator if not the first facility in list */
        if (index > 0)
        {
          vgPutc (entity, ',');
        }

        /* This facility is covered, print out name.... */
        vgPrintf(entity, (Char *)"\"%s\"", facLookup[index].vgFacStr);
      }

      vgPuts (entity, (Char *)")");
      break;
    }

    case EXTENDED_ASSIGN: /* AT+CLCK= */
    {
      /* Get fac string and mode (mandatory).... */
      if ((getExtendedString (commandBuffer_p,
                               &facString[0],
                                MAX_FAC_ID_LEN,
                                 &facStringLength) == TRUE)                    &&
          (getExtendedParameter (commandBuffer_p,
                                  &mode,
                                   VG_AT_SS_MODE_MAX) == TRUE))
      {
        
        /* Check entered value corresponds to a known fac.  Do not include
         * P2 in this search....
         */
        index = 0;
        while ((index < NUM_ENTRIES(facLookup)) &&
               (facLookup[index].vgFacId != VG_AT_SS_FAC_P2) &&
               (foundFac == FALSE))
        {
          if (memcmp(&facLookup[index].vgFacStr[0],
                      &facString[0],
                       facStringLength) == 0)
          {
            /* Matched string! */
            foundFac = TRUE;
          }

          else
          {
            /* Carry on searching.... */
            index++;
          }
        }

        /* Fetch optional parameters (password and class).... */
        (void)(getExtendedString (commandBuffer_p,
                               &password[0],
                               SIM_CHV_LENGTH,
                               &passwordLength));
        
        (void)(getExtendedParameter (commandBuffer_p,
                                  &callClass,
                                  VG_AT_SS_CLASS_DEFAULT));
        /* Check parameters are okay.... */
        if ((foundFac == TRUE)                    &&
            (mode <= VG_AT_SS_MODE_QUERY_STATUS))
        {
          supplementaryContext_p->ssParams.currentFac = (VgSsNetworkFacilityType)index;

          /* Process fac request.... */
          switch (index)
          {
            /**************************************/
            /* NOTE: For NB-IOT all call related  */
            /* <fac> are not supported            */
            /**************************************/
            case VG_AT_SS_FAC_SC:
            {
              /* SIM Pin1 lock.... */
              memcpy (&generalContext_p->password[0], &password[0], passwordLength);
              generalContext_p->passwordLength = passwordLength;

              /* Job 109119: Check minimum Pin 1 length as per GSM 11.11 - 9.3 */
              if (((mode == VG_AT_SS_MODE_QUERY_STATUS) &&
                   (passwordLength == 0)) ||
                  ((passwordLength >= MIN_CHV_SIZE) &&
                   (passwordLength <= SIM_CHV_LENGTH)))
              {
                /* Pad out rest of string.... */
                memset( (Char *)&generalContext_p->password[passwordLength],
                        UCHAR_MAX,
                        (SIM_CHV_LENGTH - passwordLength));

                switch (mode)
                {
                  case VG_AT_SS_MODE_DISABLE:
                  {
                    if (simLockGenericContext_p->simInfo.disablePin1Allowed == TRUE)
                    {
                      /* Check pin isn't already disabled.... */
                      if (simLockGenericContext_p->simInfo.pinEnabled == FALSE)
                      {
                        /* Doesn't constitute an error.... */
                        result = RESULT_CODE_OK;
                      }

                      else
                      {
                        generalContext_p->pinFunction = SIM_PIN_FUNCT_DISABLE;
                        generalContext_p->altPinKeyReference = USIM_ACCESS_NO_PIN;
                        result = sendPin1Request (entity);
                      }
                    }

                    else
                    {
                      /* Not allowed to disable the pin code.... */
                      result = VG_CME_OPERATION_NOT_ALLOWED;
                    }
                    break;
                  }

                  case VG_AT_SS_MODE_ENABLE:
                  {
                    /* Check pin isn't already enabled.... */
                    if (simLockGenericContext_p->simInfo.pinEnabled == TRUE)
                    {
                      /* Doesn't constitute an error.... */
                      result = RESULT_CODE_OK;
                    }

                    else
                    {
                      generalContext_p->pinFunction = SIM_PIN_FUNCT_ENABLE;
                      generalContext_p->altPinKeyReference = USIM_ACCESS_NO_PIN;
                      result =  sendPin1Request (entity);
                    }
                    break;
                  }

                  case VG_AT_SS_MODE_QUERY_STATUS:
                  {
                    /* job125946: check that there is a SIM inserted */
                    if (simLockGenericContext_p->simInsertedState == VG_SIM_INSERTED)
                    {
                      /* Show current status for pin 1.... */
                      vgPutNewLine(entity);
                      vgPrintf (entity,
                                (Char *)"+CLCK: %d",
                                (simLockGenericContext_p->simInfo.pinEnabled ? 1:0));
                      vgPutNewLine(entity);
                    }
                    else
                    {
                      result = VG_CME_SIM_NOT_INSERTED;
                    }
                    break;
                  }

                  default:
                  {
                    /* Invalid mode param! */
                    WarnParam(entity, mode, 0);
                    result = VG_CME_INVALID_INPUT_VALUE;
                    break;
                  }
                }
              }

              else
              {
                result = VG_CME_INCORRECT_PASSWORD;
              }
              break;
            }
#if defined (FEA_PHONEBOOK)            
            case VG_AT_SS_FAC_BD:
            {
              /* Fixed dialling facility (pin2).... */
              memcpy (&generalContext_p->password[0], &password[0], passwordLength);
              generalContext_p->passwordLength = passwordLength;

              /* Job 109119: Check minimum Pin 2 lengths as per GSM 11.11 - 9.3 */
              if (((mode == VG_AT_SS_MODE_QUERY_STATUS) &&
                   (passwordLength == 0)) ||
                  ((passwordLength >= MIN_CHV_SIZE) &&
                   (passwordLength <= SIM_CHV_LENGTH)))
              {
                memset((char*)&generalContext_p->password[passwordLength],
                       UCHAR_MAX,
                       (SIM_CHV_LENGTH - passwordLength));

                /* Action request.... */
                switch (mode)
                {
                  case VG_AT_SS_MODE_DISABLE:
                  {
                    /* Check pin isn't already disabled.... */
                    if (simLockGenericContext_p->simInfo.bdnIsEnabled == FALSE)
                    {
                      /* Doesn't constitute an error.... */
                      result = RESULT_CODE_OK;
                    }
                    else if (simLockGenericContext_p->simInfo.pin2Verified == TRUE)
                    {
                      generalContext_p->enableBdn = FALSE;
                      result = vgChManContinueAction (entity, SIG_APEX_LM_BARRED_DIAL_REQ);
                    }
                    else
                    {
                       generalContext_p->pinFunction = SIM_PIN_FUNCT_VERIFY;
                       generalContext_p->enableBdn   = FALSE;
                       generalContext_p->updatingBdn = TRUE;
                       result =  sendVerifyPin2Request (entity);
                    }
                    break;
                  }

                  case VG_AT_SS_MODE_ENABLE:
                  {
                    /* Check pin isn't already disabled.... */
                    if (simLockGenericContext_p->simInfo.bdnIsEnabled == TRUE)
                    {
                      /* Doesn't constitute an error.... */
                      result = RESULT_CODE_OK;
                    }
                    else if (simLockGenericContext_p->simInfo.pin2Verified == TRUE)
                    {
                      generalContext_p->enableBdn = TRUE;
                      result = vgChManContinueAction (entity, SIG_APEX_LM_BARRED_DIAL_REQ);
                    }
                    else
                    {
                      generalContext_p->pinFunction = SIM_PIN_FUNCT_VERIFY;
                      generalContext_p->enableBdn   = TRUE;
                      generalContext_p->updatingBdn = TRUE;
                      result =  sendVerifyPin2Request (entity);
                    }
                    break;
                  }

                  case VG_AT_SS_MODE_QUERY_STATUS:
                  {
                    /* Show current status for pin 2.... */
                    vgPutNewLine(entity);
                    vgPrintf (entity,
                              (Char *)"+CLCK: %d",
                               (simLockGenericContext_p->simInfo.bdnIsEnabled ? 1:0));
                    vgPutNewLine(entity);
                    break;
                  }

                  default:
                  {
                    /* Invalid mode param! */
                    WarnParam(entity, mode, 0);
                    result = VG_CME_INVALID_INPUT_VALUE;
                    break;
                  }
                }
              }
              else
              {
                /* Password string too long.... */
                result = VG_CME_INCORRECT_PASSWORD;
              }
              break;
            }
            case VG_AT_SS_FAC_FD:
            {
              /* Fixed dialling facility (pin2).... */
              memcpy (&generalContext_p->password[0], &password[0], passwordLength);
              generalContext_p->passwordLength = passwordLength;

              /* Job 109119: Check minimum Pin 2 lengths as per GSM 11.11 - 9.3 */
              if (((mode == VG_AT_SS_MODE_QUERY_STATUS) &&
                   (passwordLength == 0)) ||
                  ((passwordLength >= MIN_CHV_SIZE) &&
                   (passwordLength <= SIM_CHV_LENGTH)))
              {
                memset((char*)&generalContext_p->password[passwordLength],
                       UCHAR_MAX,
                       (SIM_CHV_LENGTH - passwordLength));

                /* Action request.... */
                switch (mode)
                {
                  case VG_AT_SS_MODE_DISABLE:
                  {
                    /* Check pin isn't already disabled.... */
                    if (simLockGenericContext_p->simInfo.fdnIsEnabled == FALSE)
                    {
                      /* Doesn't constitute an error.... */
                      result = RESULT_CODE_OK;
                    }
                    else if (simLockGenericContext_p->simInfo.pin2Verified == TRUE)
                    {
                      generalContext_p->enableFdn = FALSE;
                      result = vgChManContinueAction (entity, SIG_APEX_LM_FIXED_DIAL_REQ);
                    }
                    else
                    {
                       generalContext_p->pinFunction = SIM_PIN_FUNCT_VERIFY;
                       generalContext_p->enableFdn   = FALSE;
                       generalContext_p->updatingBdn = FALSE;

                       result =  sendVerifyPin2Request (entity);
                    }
                    break;
                  }

                  case VG_AT_SS_MODE_ENABLE:
                  {
                    /* Check pin 2 isn't already enabled.... */
                    if (simLockGenericContext_p->simInfo.fdnIsEnabled == TRUE)
                    {
                      /* Doesn't constitute an error.... */
                      result = RESULT_CODE_OK;
                    }
                    else if (simLockGenericContext_p->simInfo.pin2Verified == TRUE)
                    {
                      generalContext_p->enableFdn = TRUE;
                      result = vgChManContinueAction (entity, SIG_APEX_LM_FIXED_DIAL_REQ);
                    }
                    else
                    {
                      generalContext_p->pinFunction = SIM_PIN_FUNCT_VERIFY;
                      generalContext_p->enableFdn   = TRUE;
                      generalContext_p->updatingBdn = FALSE;
                      result =  sendVerifyPin2Request (entity);
                    }
                    break;
                  }

                  case VG_AT_SS_MODE_QUERY_STATUS:
                  {
                    /* Show current status for pin 2.... */
                    vgPutNewLine(entity);
                    vgPrintf (entity,
                              (Char *)"+CLCK: %d",
                               (simLockGenericContext_p->simInfo.fdnIsEnabled ? 1:0));
                    vgPutNewLine(entity);
                    break;
                  }

                  default:
                  {
                    /* Invalid mode param! */
                    WarnParam(entity, mode, 0);
                    result = VG_CME_INVALID_INPUT_VALUE;
                    break;
                  }
                }
              }

              else
              {
                /* Password string too long.... */
                result = VG_CME_INCORRECT_PASSWORD;
              }
              break;
            }
#endif /* FEA_PHONEBOOK */              

#if defined (FEA_SIMLOCK)
            case VG_AT_SS_FAC_PS:
            case VG_AT_SS_FAC_PN:
            case VG_AT_SS_FAC_PU:
            case VG_AT_SS_FAC_PP:
            case VG_AT_SS_FAC_PC:
            {
              /* Mobile Equipement Personalisation (MEP) facilities.... */
              if (mode != VG_AT_SS_MODE_QUERY_STATUS)
              {
                /* Enable/disable facility.... */
                if (mode == VG_AT_SS_MODE_DISABLE)
                {
                  generalContext_p->mepOperation = DEACTIVATE_PERSONALISATION;
                }

                else
                {
                  generalContext_p->mepOperation = ACTIVATE_PERSONALISATION;
                }

                generalContext_p->mepSelector = ssMepSelectors[index - VG_AT_SS_FAC_PS];
                memcpy (&generalContext_p->password[0], &password[0], passwordLength);
                generalContext_p->passwordLength = passwordLength;


                result = vgChManContinueAction (entity, APEX_SIM_MEP_REQ);
              }

              else
              {
                /* Status of facility.... */
                result = vgChManContinueAction (entity, APEX_SIM_MEP_STATUS_REQ);
              }

              break;
            }
#endif /* FEA_SIMLOCK */

            default:
            {
              /* We don't implement these at the moment.... */
              result = VG_CME_UNSUPPORTED_MODE;
              break;
            }
          }
        }

        else
        {
          /* Invalid parameter values.... */
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }

      else
      {
        /* One of the mandatory values missing.... */
        result = VG_CME_INVALID_INPUT_VALUE;
      }
      break;
    }

    case EXTENDED_QUERY:  /* AT+CLCK? */
    case EXTENDED_ACTION: /* AT+CLCK */
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

