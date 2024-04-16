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
 * Utilities for the profile sub-system
 **************************************************************************/

#define MODULE_NAME "RVPROF"

#include <system.h>
#include <rvprof.h>
#include <rvpfsigo.h>
#include <rvomtime.h>
#include <rvoman.h>
#include <rvdata.h>
#include <rvcimux.h>
#include <rvcimxut.h>
#include <rvcimxsot.h>
#if defined (DEVELOPMENT_VERSION)
#  include <stdio.h>
#endif

#include <gkisig.h>

static const Char *cscsString[VG_AT_CSCS_MAX_VAL] =
{
  (Char *)"GSM",
  (Char *)"HEX",
  (Char *)"IRA",
  (Char *)"PCCP",
  (Char *)"PCDN",
  (Char *)"UCS2",
  (Char *)"8859-1"
};

static const VgLmInfo vgLmInfo[NUMBER_OF_PHONE_BOOKS] =
{
   /* phonebook */     /* read only */ /* protected */ /* hidden */
  { (Char *)"MC", DIAL_LIST_LNM,     TRUE,  FALSE, FALSE }, /* ME missed (unanswered) calls list */
  { (Char *)"RC", DIAL_LIST_LNR,     TRUE,  FALSE, FALSE }, /* ME received calls list            */
  { (Char *)"DC", DIAL_LIST_LND,     TRUE,  FALSE, FALSE }, /* ME dialled calls list (same as LD)*/
  { (Char *)"LD", DIAL_LIST_LND,     TRUE,  FALSE, FALSE }, /* SIM dialled calls list            */
                                 /* read/write */
  { (Char *)"ME", DIAL_LIST_ADR,     FALSE, FALSE, FALSE }, /* ME phonebook                      */
  { (Char *)"SM", DIAL_LIST_ADN_GLB, FALSE, FALSE, FALSE }, /* SIM global phonebook              */
  { (Char *)"AP", DIAL_LIST_ADN_APP, FALSE, FALSE, TRUE  }, /* SIM application phonebook         */
  { (Char *)"FD", DIAL_LIST_FDN,     FALSE, TRUE,  FALSE }, /* SIM fixdialling phone list        */
  { (Char *)"ON", DIAL_LIST_MSISDN,  FALSE, FALSE, FALSE }, /* SIM own numbers (MSISDNs) list    */
    /* proprietary phonebooks */
  { (Char *)"BN", DIAL_LIST_BDN,     FALSE, TRUE,  FALSE }, /* SIM barred dialled number         */
  { (Char *)"SD", DIAL_LIST_SDN,     FALSE, FALSE, FALSE }, /* SIM service dial number           */
  { (Char *)"VM", DIAL_LIST_CPHS_MN, FALSE, FALSE, FALSE }  /* SIM voice mailbox                 */
};

/* Profile values which are only relevant for current session.
 * At the power-up, the values are set to default and not retrieved from NVRAM.*/
static const ProfId volatileProfileValues[]=
{
  NUM_OF_PROF
};

#define VG_NUMBER_OF_VOLATILE_PROFILE_VALUES (sizeof (volatileProfileValues) / sizeof (ProfId))

/* Profile values which are not reset to Factory Defaults at AT&F.*/
static const ProfId nonFactoryResetProfileValues[]=
{
  NUM_OF_PROF
};

#define VG_NUMBER_OF_NON_FAC_RESET_PROFILE_VALUES (sizeof (nonFactoryResetProfileValues) / sizeof (ProfId))

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/
static void sendMuxConfigInfo (const VgmuxChannelNumber entity,
                                const ProfId profId);


static Boolean isVolatileProfile (ProfId profId);

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:    sendMuxConfigInfo
 *
 * Parameters:  entity - mux channel
 *              profId - the modified profile entry
 *
 * Returns:     Nothing
 *
 * Description: sends mux a configuration update, called after a profile entry
 *              is modified so that changes to the profile take affect.
 *
 *-------------------------------------------------------------------------*/

static void sendMuxConfigInfo (const VgmuxChannelNumber entity,
                                 const ProfId profId)
{
  switch (profId)
  {
    case PROF_CSMS:
    case (PROF_CNMI + (ProfId)1):
    case (PROF_CNMI + (ProfId)3): /* only need to send if these values change */
    {
      /* pass sms routing information to the background layer */
      vgApexSendSmProfileChangedReq (entity);
      break;
    }
    case PROF_DCD:
    case PROF_DTR:
    case PROF_S2:
    case PROF_S12:
    case PROF_S25:
    case PROF_IFC:
    case (ProfId)(PROF_IFC+1):
    case PROF_IPR:
    {     
      /*       
            * Suppress reconfiguration of MUX for IPR and ICF until the OK string has been sent out.
            */
      if ((getCommandId(entity) != VG_AT_PF_IPR) && (getCommandId(entity) != VG_AT_PF_ICF))
      {        
        vgSendCiMuxConfigureChannelReq (entity, FALSE);
      }
      break;
    }
    default:
    {
      break;
    }
  }
}

 /****************************************************************************
 *
 * Function:    isVolatileProfile
 *
 * Parameters:  profId
 *
 * Returns:     Boolean
 *
 * Description: Returns TRUE if the profile is only relevant to current session.
 *
 ****************************************************************************/
static Boolean isVolatileProfile (ProfId profId)
{
  Int32 i;

  for (i=0; i<VG_NUMBER_OF_VOLATILE_PROFILE_VALUES;i++)
    if (volatileProfileValues[i] == profId)
    {
    return TRUE;
    }

  return FALSE;
}

 /****************************************************************************
 *
 * Function:    isNonFactoryResetProfile
 *
 * Parameters:  profId
 *
 * Returns:     Boolean
 *
 * Description: Returns TRUE if the profile is not to be set to default at AT&F.
 *
 ****************************************************************************/
static Boolean isNonFactoryResetProfile (ProfId profId)
{
  Int32 i;

  for (i=0; i<VG_NUMBER_OF_NON_FAC_RESET_PROFILE_VALUES;i++)
    if (nonFactoryResetProfileValues[i] == profId)
    {
      return TRUE;
    }

  return FALSE;
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

 /****************************************************************************
 *
 * Function:    setProfileValue
 *
 * Parameters:  entity - entity number
 *              profId - profile entry to modify
 *              value  - new profile value
 *
 * Returns:     ResultCode_t - a result code indicating whether the profile
 *                             value change request succeeded
 *
 * Description: sets the given profile entry to a new value, making sure that
 *              generic profile entries are set over all the active entities
 *              and blocking changes that may affect current operations
 *
 ****************************************************************************/
ResultCode_t setProfileValue (const VgmuxChannelNumber entity,
                               const Int8 profId,
                                const Int8 value)
{
  ResultCode_t result = RESULT_CODE_OK;
  ProfileContext_t*  profileContext_p  = ptrToProfileContext (entity);
  ProfileGenericContext_t* profileGenricContext_p = ptrToProfileGenericContext ();
#if defined (ATCI_SLIM_DISABLE)  
  FatalCheck(profileContext_p != PNULL, entity, 0, 0);
#endif
  FatalAssert (profId < NUM_OF_PROF);
  
  if (profId < NUM_OF_PROF_SPECIFIC)  
  {
    if (profileContext_p->value[profId] != value)
    {
      /*
       * Only change the value if it has changed!
       */      
      profileContext_p->value[profId] = value;

      /*
       * Update the MUX with any port configuration changes.
       */
      sendMuxConfigInfo (entity, (ProfId)profId);
    }
  } 
  else  /* generic profile value, need to set all profiles to the same values */
  {     
      /* the entity may not yet be enabled, so set its profile first */
      profileGenricContext_p->value[profId - NUM_OF_PROF_SPECIFIC] = value;
      /* update mux config changes */
      sendMuxConfigInfo (entity, (ProfId)profId);   
  }

  return (result);
}

 /****************************************************************************
 *
 * Function:    setProfileValueBit
 *
 * Parameters:  entity  - entity number
 *              profId  - profile entry to modify
 *              profBit - profile bit to modify
 *              value   - new profile value
 *
 * Returns:     ResultCode_t - a result code indicating whether the profile
 *                             value change request succeeded
 *
 * Description: sets the given profile entry to a new value, making sure that
 *              generic profile entries are set over all the active entities
 *              and blocking changes that may affect current operations
 *
 ****************************************************************************/

ResultCode_t setProfileValueBit (const VgmuxChannelNumber entity,
                                  const Int8 profId,
                                   const VgProfileBit profBit,
                                    const Int8 value)
{
  ResultCode_t result       = RESULT_CODE_OK;
  Int8         profileValue = getProfileValue (entity, profId);

  if (value == 0)
  {
    /* job117183: use bitwise one's complement rather than logical NOT */
    profileValue &= (~profBit);
  }
  else
  {
    profileValue |= profBit;
  }

  result = setProfileValue (entity, profId, profileValue);

  return (result);
}

 /****************************************************************************
 *
 * Function:        getProfileValue
 *
 * Parameters:      entity - entity number
 *                  profId  - profile entry to modify
 *
 * Returns:         Int8 profile value
 *
 * Description:     Get profile value.
 *
 ****************************************************************************/
Int8 getProfileValue (const VgmuxChannelNumber entity,
                       const Int8 profId)
{
  Int8               result = 0;
  ProfileContext_t*  profileContext_p = ptrToProfileContext (entity);
  ProfileGenericContext_t*  profileGenericContext_p = ptrToProfileGenericContext ();

  FatalAssert (profId < NUM_OF_PROF);

  if (profId < NUM_OF_PROF_SPECIFIC)  
  {
    /* check entity is active */
    if (isEntityActive (entity))
    {
#if defined (ATCI_SLIM_DISABLE)

       FatalCheck(profileContext_p != PNULL, entity, 0, 0);
#endif
       result = profileContext_p->value[profId];
    }
  }
  else
  {
       result = profileGenericContext_p->value[profId - NUM_OF_PROF_SPECIFIC];
  }

  return (result);
}

 /****************************************************************************
 *
 * Function:    setDefaultSpecificProfile
 *
 * Parameters:  entity .
 *              Boolean powerUp  - whether called at power up.
 *
 * Returns:     ResultCode_t  - a result code indicating if all the profile
 *                              values were set successfully
 *
 * Description: Sets default profile for specified entity.
 *
 ****************************************************************************/

ResultCode_t setDefaultSpecificProfile (const VgmuxChannelNumber entity,
                                const Boolean powerUp )
{
  Int8                   pIndex;
  ResultCode_t           result             = RESULT_CODE_OK;
  ProfileContext_t*      profileContext_p   = ptrToProfileContext (entity);
  const ProfileContext_t* factorySpecificDefaults_p = getFactorySpecificDefaults ();
#if defined (ATCI_SLIM_DISABLE)  
  FatalCheck(profileContext_p != PNULL, entity, 0, 0);
#endif
  /* copy code word first otherwise cannot use profile values before entity
   * is enabled */
  memcpy (&(profileContext_p->value[NUM_OF_PROF_SPECIFIC]),
           &(factorySpecificDefaults_p->value[NUM_OF_PROF_SPECIFIC]),
            NUM_CHECKSUM * sizeof(Int8));

  /* set each profile value */
  for (pIndex = 0; pIndex < NUM_OF_PROF_SPECIFIC; pIndex++)
  {
    if ((powerUp) || (isNonFactoryResetProfile((ProfId)pIndex) == FALSE))
    {
      if((pIndex != PROF_IPR) && (pIndex != (PROF_IFC + 0)) && (pIndex != (PROF_IFC + 1)))
      {
          if (setProfileValue (entity,
                           pIndex,
                           factorySpecificDefaults_p->value[pIndex]) != RESULT_CODE_OK)
          {
            result = RESULT_CODE_ERROR;
          }
      }
    }
  }
  return (result);
}

 /****************************************************************************
 *
 * Function:    setDefaultGenericProfile
 *
 * Parameters:  entity .
 *              Boolean powerUp  - whether called at power up.
 *
 * Returns:     ResultCode_t  - a result code indicating if all the profile
 *                              values were set successfully
 *
 * Description: Sets default profile for specified entity.
 *
 ****************************************************************************/

ResultCode_t setDefaultGenericProfile (const VgmuxChannelNumber entity,
                                const Boolean powerUp )
{
  Int8                   pIndex;
  ResultCode_t           result             = RESULT_CODE_OK;
  ProfileGenericContext_t*   profileGenericContext_p   = ptrToProfileGenericContext ();
  const ProfileGenericContext_t* factoryGenericDefaults_p = getFactoryGenericDefaults ();
  
  PARAMETER_NOT_USED(entity);
  
  /* copy code word first otherwise cannot use profile values before entity
   * is enabled */
  memcpy (&(profileGenericContext_p->value[NUM_OF_PROF_GENERIC]),
           &(factoryGenericDefaults_p->value[NUM_OF_PROF_GENERIC]),
            NUM_CHECKSUM * sizeof(Int8));
    
  for (pIndex= 0; pIndex < NUM_OF_PROF_GENERIC; pIndex++)
    if ((powerUp) || (isNonFactoryResetProfile((ProfId)(pIndex + NUM_OF_PROF_SPECIFIC)) == FALSE))
    {
      if (setProfileValue (entity,
                           pIndex + NUM_OF_PROF_SPECIFIC,
                           factoryGenericDefaults_p->value[pIndex]) != RESULT_CODE_OK)
      {
        result = RESULT_CODE_ERROR;
      }
    }
    
  return (result);
}



/****************************************************************************
 *
 * Function:    setStoredSpecificProfile
 *
 * Parameters:  entity
 *              source_p            - source profile
 *
 * Returns:     ResultCode_t  - a result code indicating if all the profile
 *                              values were set successfully
 *
 * Description: Sets specific profile stored in NVRAM.
 *
 ****************************************************************************/

ResultCode_t setStoredSpecificProfile (const VgmuxChannelNumber entity,
                               const ProfileContext_t *source_p)
{
  Int8                   pIndex;
  ResultCode_t           result             = RESULT_CODE_OK;
  ProfileContext_t*      profileContext_p   = ptrToProfileContext (entity);
  const ProfileContext_t *factorySpecificDefaults_p = getFactorySpecificDefaults  ();
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(profileContext_p != PNULL, entity, 0, 0);
#endif
  FatalAssert (isSpecificProfileCodeWordValid (source_p) == TRUE);

  /* copy code word first otherwise cannot use profile values before entity
   * is enabled */
  memcpy (&(profileContext_p->value[NUM_OF_PROF_SPECIFIC]),
           &(source_p->value[NUM_OF_PROF_SPECIFIC]),
            NUM_CHECKSUM * sizeof(Int8));

  /* set each profile value */
  for (pIndex = 0; pIndex < NUM_OF_PROF_SPECIFIC; pIndex++)
  {
    if (isVolatileProfile((ProfId)pIndex))
    {
      if (setProfileValue (entity,
                           pIndex,
                           factorySpecificDefaults_p->value[pIndex]) != RESULT_CODE_OK)
      {
        result = RESULT_CODE_ERROR;
      }

    }
    else
    {
      if (setProfileValue (entity,
                           pIndex,
                           source_p->value[pIndex]) != RESULT_CODE_OK)
      {
        result = RESULT_CODE_ERROR;
      }
    }
  }

  return (result);
}

/****************************************************************************
 *
 * Function:    setStoredGenericProfile
 *
 * Parameters:  entity
 *              source_p            - source profile
 *
 * Returns:     ResultCode_t  - a result code indicating if all the profile
 *                              values were set successfully
 *
 * Description: Sets generic profile stored in NVRAM.
 *
 ****************************************************************************/

ResultCode_t setStoredGenericProfile (const VgmuxChannelNumber entity,
                               const ProfileGenericContext_t *source_p)
{
  Int8                   pIndex;
  ResultCode_t           result             = RESULT_CODE_OK;
  ProfileGenericContext_t*      profileGenericContext_p   = ptrToProfileGenericContext ();
  const ProfileGenericContext_t *factoryGenericDefaults_p = getFactoryGenericDefaults  ();

  FatalAssert (isGenericProfileCodeWordValid (source_p) == TRUE);

  /* copy code word first otherwise cannot use profile values before entity
   * is enabled */
  memcpy (&(profileGenericContext_p->value[NUM_OF_PROF_GENERIC]),
           &(source_p->value[NUM_OF_PROF_GENERIC]),
            NUM_CHECKSUM*sizeof(Int8));
  
  /* set each profile value */
  for (pIndex = 0; pIndex < NUM_OF_PROF_GENERIC; pIndex++)
  {
    if (isVolatileProfile((ProfId)pIndex + NUM_OF_PROF_SPECIFIC))
    {
      if (setProfileValue (entity,
                           pIndex + NUM_OF_PROF_SPECIFIC,
                           factoryGenericDefaults_p->value[pIndex]) != RESULT_CODE_OK)
      {
        result = RESULT_CODE_ERROR;
      }

    }
    else
    {
      if (setProfileValue (entity,
                           pIndex + NUM_OF_PROF_SPECIFIC,
                           source_p->value[pIndex]) != RESULT_CODE_OK)
      {
        result = RESULT_CODE_ERROR;
      }
    }
  }

  return (result);
}
 /****************************************************************************
 *
 * Function:    copySpecificProfile
 *
 * Parameters:  destination_p       - destintation profile
 *              source_p            - source profile
 *
 * Returns:     nothing
 *
 * Description: copies a specific profile
 *
 ****************************************************************************/

void copySpecificProfile (ProfileContext_t *destination_p,
                   const ProfileContext_t *source_p)
{
  Int8 index;

  FatalAssert (isSpecificProfileCodeWordValid (source_p) == TRUE);

  for (index = 0; index < NUM_OF_PROF_SPECIFIC_ENTRIES; index++ )
  {
    destination_p->value[index] = source_p->value[index];
  }
}

 /****************************************************************************
 *
 * Function:    copyGenericProfile
 *
 * Parameters:  destination_p       - destintation profile
 *              source_p            - source profile
 *
 * Returns:     nothing
 *
 * Description: copies a generic profile
 *
 ****************************************************************************/

void copyGenericProfile (ProfileGenericContext_t *destination_p,
                   const ProfileGenericContext_t *source_p)
{
  Int8 index;

  FatalAssert (isGenericProfileCodeWordValid (source_p) == TRUE);

  for (index = 0; index < NUM_OF_PROF_GENERIC_ENTRIES; index++ )
  {
    destination_p->value[index] = source_p->value[index];
  }
}


 /****************************************************************************
 *
 * Function:    setCresSpecificProfile
 *
 * Parameters:  entity   - entity number
 *              source_p - profile containing the new SMS settings
 *
 * Returns:     ResultCode_t - a result code indicating if all the profile
 *                             values were set successfully
 *
 * Description: updates the SMS profile configuration if possible
 *
 ****************************************************************************/

ResultCode_t setCresSpecificProfile (const VgmuxChannelNumber entity,
                              const ProfileContext_t *source_p)
{
  ResultCode_t result = RESULT_CODE_OK;

  FatalAssert (isSpecificProfileCodeWordValid (source_p) == TRUE);

  if ((setProfileValue (entity,
                         PROF_CMGF,
                          source_p->value[PROF_CMGF]) != RESULT_CODE_OK) ||
      (setProfileValue (entity,
                         PROF_CSDH,
                          source_p->value[PROF_CSDH]) != RESULT_CODE_OK))
  {
    result = RESULT_CODE_ERROR;
  }

  return (result);
}

 /****************************************************************************
 *
 * Function:    setCresGenericProfile
 *
 * Parameters:  entity   - entity number
 *              source_p - profile containing the new SMS settings
 *
 * Returns:     ResultCode_t - a result code indicating if all the profile
 *                             values were set successfully
 *
 * Description: updates the SMS profile configuration if possible
 *
 ****************************************************************************/
ResultCode_t setCresGenericProfile (const VgmuxChannelNumber entity,
                              const ProfileGenericContext_t *source_p)
{
  ResultCode_t result = RESULT_CODE_OK;
  Int8 index;

  FatalAssert (isGenericProfileCodeWordValid (source_p) == TRUE);
  
  for (index = 0; index < NUM_CNMI; index++ )
  {
    if (setProfileValue (entity,
                          (Int8) (PROF_CNMI + index),
                           source_p->value[PROF_CNMI - NUM_OF_PROF_SPECIFIC + index]) != RESULT_CODE_OK)
    {
      result = RESULT_CODE_ERROR;
    }
  }
  return (result);
}



 /****************************************************************************
 *
 * Function:    isGenericProfileCodeWordValid
 *
 * Parameters:  profile_p - profile to be checked
 *
 * Returns:     Boolean - indicating if codeword matched
 *
 * Description: Compares the codeword of the factory default to the given
 *              profile. This indicates whether the profile structures match.
 *
 ****************************************************************************/

Boolean isGenericProfileCodeWordValid (const ProfileGenericContext_t *profile_p)
{
  const ProfileGenericContext_t *factoryGenericDefaults_p = getFactoryGenericDefaults  ();
  Boolean result = TRUE;
  Int8    index;

  for (index = NUM_OF_PROF_GENERIC;
        (index < NUM_OF_PROF_GENERIC_ENTRIES) && (result == TRUE);
         index ++)
  {
    if (factoryGenericDefaults_p->value[index] != profile_p->value[index])
    {
      result = FALSE;
    }
  }

  return (result);
}

 /****************************************************************************
 *
 * Function:    isSpecificProfileCodeWordValid
 *
 * Parameters:  profile_p - profile to be checked
 *
 * Returns:     Boolean - indicating if codeword matched
 *
 * Description: Compares the codeword of the factory default to the given
 *              profile. This indicates whether the profile structures match.
 *
 ****************************************************************************/
Boolean isSpecificProfileCodeWordValid (const ProfileContext_t *profile_p)
{
  const ProfileContext_t *factorySpecificDefaults_p = getFactorySpecificDefaults  ();
  Boolean result = TRUE;
  Int8    index;

  for (index = NUM_OF_PROF_SPECIFIC;
        (index < NUM_OF_PROF_SPECIFIC_ENTRIES) && (result == TRUE);
         index ++)
  {
    if (factorySpecificDefaults_p->value[index] != profile_p->value[index])
    {
      result = FALSE;
    }
  }

  return (result);
}

 /****************************************************************************
 *
 * Function:        isProfileValueEqual
 *
 * Parameters:      profId  - profile entry to check
 *                  value   - value to check
 *
 * Returns:         Boolean
 *
 * Description:     Checks whether the specified profile is set to the
 *                  specified value on specific entity.
 *                  If the entity is set to VGMUX_CHANNEL_INVALID is defined, checks whether
 *                  the profile  is set to the specified value on ANY entity.
 ****************************************************************************/

Boolean isProfileValueEqual (const VgmuxChannelNumber entity, const Int8 profId, const Int8 value)
{
  Boolean returnCode = FALSE;
  if (entity == VGMUX_CHANNEL_INVALID)
  {
    VgmuxChannelNumber profileEntity = 0;

    while((profileEntity<CI_MAX_ENTITIES)&&(returnCode == FALSE))
    {
      if ((isEntityActive (profileEntity)) && (getProfileValue (profileEntity, profId) == value))
      {
        returnCode = TRUE;
      }
      profileEntity++;
    }
  }
  else
  {
    if ((isEntityActive (entity)) && (getProfileValue(entity, profId) == value ))
    {
      returnCode = TRUE;
    }
  }
  return returnCode;
}

 /****************************************************************************
 *
 * Function:        isProfileValueBitEqual
 *
 * Parameters:      entity  - entity number
 *                  profId  - profile entry to check
 *                  value   - value to check
 *                  profBit - profile bit to modify
 *
 * Returns:         Int8 profile value
 *
 * Description:     Checks whether the specified profile bit is set to the
 *                  specified value on specific entity.
 *                  If the entity is set to VGMUX_CHANNEL_INVALID, checks whether
 *                  the profile bit  is set to the specified value on ANY entity.
 ****************************************************************************/

Boolean isProfileValueBitEqual (const VgmuxChannelNumber entity,
                                const Int8               profId,
                                const VgProfileBit       profBit,
                                const Int8               value)
{
  Boolean returnCode = FALSE;

  if (entity == VGMUX_CHANNEL_INVALID)
  {
    VgmuxChannelNumber profileEntity = 0;

    while((profileEntity<CI_MAX_ENTITIES)&&(returnCode == FALSE))
    {
      if ((isEntityActive (profileEntity)) && (getProfileValueBit(profileEntity, profId,profBit) == value))
      {
        returnCode = TRUE;
      }
      profileEntity++;
    }
  }
  else
  {
    if ((isEntityActive (entity)) && (getProfileValueBit(entity, profId, profBit) == value ))
    {
      returnCode = TRUE;
    }
  }
  return returnCode;
}

 /****************************************************************************
 *
 * Function:    getProfileValueBit
 *
 * Parameters:  entity - entity number
 *              profId  - profile entry to modify
 *              profBit - profile bit to modify
 *
 * Returns:     Int8 profile value
 *
 * Description: Get profile value.
 *
 ****************************************************************************/

Int8 getProfileValueBit (const VgmuxChannelNumber entity,
                          const Int8 profId,
                           const VgProfileBit profBit)
{
  Int8 result;

  /* get profile value */
  result = getProfileValue (entity, profId);

  /* get profile bit requested */
  if ((result & profBit) == 0)
  {
    result = 0;
  }
  else
  {
    result = 1;
  }

  return (result);
}

/****************************************************************************
 *
 * Function:    getChannelConfOptions
 *
 * Parameters:  entity
 *
 * Returns:     confOptions
 *
 * Description: The function is called to fill ChannelConfOptions structure
 *              with appropriate profile values.
 ****************************************************************************/
void getChannelConfOptions ( const VgmuxChannelNumber entity, ChannelConfOptions* confOptions_p)
{
  confOptions_p->dcdFollows      = (Boolean) getProfileValue (entity, PROF_DCD);
  confOptions_p->actionOnDtrDrop = (ActionOnDtrDrop) getProfileValue (entity, PROF_DTR);
  confOptions_p->escChar         = getProfileValue (entity, PROF_S2);
  confOptions_p->escGuardPeriod  = getProfileValue (entity, PROF_S12);
  confOptions_p->dtrIgnoreTime   = getProfileValue (entity, PROF_S25);
  confOptions_p->escapeOnEscSeq  = (Boolean) (confOptions_p->escChar > 127) ? FALSE : TRUE;
}


/****************************************************************************
 *
 * Function:    getComPortSettings
 *
 * Parameters:  entity
 *
 * Returns:     comPortSettings
 *
 * Description: The function is called to fill ComPortSettings structure
 *              with appropriate profile values.
 ****************************************************************************/
void getComPortSettings (const VgmuxChannelNumber entity, ComPortSettings* comPortSettings_p)
{

  comPortSettings_p->portSpeed         = (PortSpeed)    getProfileValue (entity, PROF_IPR);
  comPortSettings_p->flowCtrl.uplink   = (FlowCtrlType) getProfileValue (entity, PROF_IFC + 0);
  comPortSettings_p->flowCtrl.downlink = (FlowCtrlType) getProfileValue (entity, PROF_IFC + 1);
}

 /****************************************************************************
 *
 * Function:    getCscsString
 *
 * Parameters:  Int8
 *
 * Returns:     Char*
 *
 * Description:
 *
 ****************************************************************************/

const Char *getCscsString (VgCSCSMode cscsIndex)
{
  return (cscsString[cscsIndex]);
}

 /****************************************************************************
 *
 * Function:    getVgLmInfo
 *
 * Parameters:  Int8
 *
 * Returns:     VgLmInfo
 *
 * Description:
 *
 ****************************************************************************/

const VgLmInfo *getVgLmInfoRec (void)
{
  return (vgLmInfo);
}

