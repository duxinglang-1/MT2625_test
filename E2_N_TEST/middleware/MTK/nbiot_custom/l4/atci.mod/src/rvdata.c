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
 * Shared memory resource access functions
 **************************************************************************/

#define MODULE_NAME "RVDATA"

#include <system.h>
#include <gkimem.h>
#include <rvdata.h>
#include <rvprof.h>
#include <rvomtime.h>
#include <vgmx_sig.h>

/***************************************************************************
 * Macros
 ***************************************************************************/

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

/***************************************************************************
 * Types
 ***************************************************************************/

/***************************************************************************
 * Variables
 ***************************************************************************/

static Entity_t  ciEntities;

/* INTERFACE FUNCTIONS FOR MANIPUATING THE ENTITY CONTEXT DATA */

 /****************************************************************************
 *
 * Function:    ptrToAtciContextData
 *
 * Parameters:  None
 *
 * Returns:     pointer to main ciEntities context data;
 *
 * Description: Returns pointer to ATCI context data
 *
 ****************************************************************************/
 
 Entity_t* ptrToAtciContextData()
 {
    return(&ciEntities);
 }

 /****************************************************************************
 *
 * Function:    ptrToEntityContextData
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 * Description: Returns pointer to Entity specific context data
 *
 ****************************************************************************/

EntityContextData_t* ptrToEntityContextData (const VgmuxChannelNumber entity)
{
  FatalAssert ((Int8)entity < CI_MAX_ENTITIES);

  return (ciEntities.entitySpecificData[entity]);

}

 /****************************************************************************
 *
 * Function:    ptrToNvramContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 * ATTENTION: After calling this function, please check that the returned pointer is valid!!!
 *
 ****************************************************************************/

NvramContext_t* ptrToNvramContext (const VgmuxChannelNumber entity)
{
  if (ptrToEntityContextData (entity) == PNULL)
  {
    return PNULL;
  }
  return (&ptrToEntityContextData(entity)->nvramContext);
}
 
 /****************************************************************************
 *
 * Function:    ptrToScanParseContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 * ATTENTION: After calling this function, please check that the returned pointer is valid!!!
 *
 ****************************************************************************/

ScanParseContext_t* ptrToScanParseContext (const VgmuxChannelNumber entity)
{
  if (ptrToEntityContextData (entity) == PNULL)
  {
    return PNULL;
  }
  return (&ptrToEntityContextData(entity)->scanParseContext);
}

 /****************************************************************************
 *
 * Function:    ptrToMuxContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 ****************************************************************************/

MuxContext_t* ptrToMuxContext ( void )
{
  return (&ciEntities.muxContext);
}

 /****************************************************************************
 *
 * Function:    ptrToChannelContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 * ATTENTION: After calling this function, please check that the returned pointer is valid!!!
 *
 ****************************************************************************/

ChannelContext_t* ptrToChannelContext (const VgmuxChannelNumber entity)
{
  if (ptrToEntityContextData (entity) == PNULL)
  {
    return PNULL;
  }
  return (&ptrToEntityContextData(entity)->channelContext);
}

 /****************************************************************************
 *
 * Function:    ptrToProfileContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 * ATTENTION: After calling this function, please check that the returned pointer is valid!!!
 *
 ****************************************************************************/

ProfileContext_t* ptrToProfileContext (const VgmuxChannelNumber entity)
{
  if (ptrToEntityContextData (entity) == PNULL)
  {
    return PNULL;
  }
  return (&ptrToEntityContextData(entity)->profileContext);
}

 /****************************************************************************
 *
 * Function:    ptrToChManagerContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 ****************************************************************************/

ChManagerContext_t*  ptrToChManagerContext ( void )
{
  return (&ciEntities.chManagerContext);
}

 /****************************************************************************
 *
 * Function:    ptrToCrManagerContext
 *
 * Parameters:
 *
 * Returns:     Pointer to CR manager context data for the entity.
 *
 * Description:
 *
 ****************************************************************************/
CrManagerContext_t*  ptrToCrManagerContext (const VgmuxChannelNumber entity)
{
  EntityContextData_t *entityContextData_p;
  CrManagerContext_t  *ret_p = PNULL;

  FatalAssert ((Int8)entity < CI_MAX_ENTITIES);

  entityContextData_p = ptrToEntityContextData(entity);

  if (entityContextData_p == PNULL)
  {
    WarnParam(entity, 0, 0);  
  }  
  else
  {
    ret_p = &entityContextData_p->crManagerContext;
  }

  return (ret_p);
}

 /****************************************************************************
 *
 * Function:    ptrToCrManagerGenericContext
 *
 * Parameters:
 *
 * Returns:     pointer to generic CR Manager context data
 *
 * Description:
 *
 ****************************************************************************/

CrManagerGenericContext_t* ptrToCrManagerGenericContext ( void )
{
    return (&ciEntities.crManagerGenericContext);
}

 /****************************************************************************
 *
 * Function:    ptrToGeneralContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 * ATTENTION: After calling this function, please check that the returned pointer is valid!!!
 *
 ****************************************************************************/

GeneralContext_t*  ptrToGeneralContext (const VgmuxChannelNumber entity)
{
  if (ptrToEntityContextData (entity) == PNULL)
  {
    return PNULL;
  }
  return (&ptrToEntityContextData(entity)->generalContext);
}

 /****************************************************************************
 *
 * Function:    ptrToGeneralGenericContext
 *
 * Parameters:
 *
 * Returns:     pointer to generic general context data
 *
 * Description:
 *
 ****************************************************************************/

GeneralGenericContext_t* ptrToGeneralGenericContext ( void )
{
  return (&ciEntities.generalGenericContext);
}

 /****************************************************************************
 *
 * Function:    ptrToStkGenericContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 ****************************************************************************/

StkEntityGenericData_t*  ptrToStkGenericContext ( void )
{
  return (&ciEntities.stkEntityGenericData);
}

 /****************************************************************************
 *
 * Function:    ptrToMobilityContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 ****************************************************************************/

MobilityContext_t*  ptrToMobilityContext ( void )
{
  return (&ciEntities.mobilityContext);
}

 /****************************************************************************
 *
 * Function:    ptrToSleepManContext
 *
 * Parameters:
 *
 * Returns:     Pointer to SleepManager Context data
 *
 * Description:
 *
 ****************************************************************************/

 SleepManContext_t* ptrToSleepManContext ( void )
 {
    return (&ciEntities.sleepManContext);  
 }
 
 /****************************************************************************
 *
 * Function:    ptrToEntityMobilityContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 ****************************************************************************/

EntityMobilityContext_t*  ptrToEntityMobilityContext ( const VgmuxChannelNumber entity )
{
  if (ptrToEntityContextData (entity) == PNULL)
  {
    return PNULL;
  }
  return (&ptrToEntityContextData(entity)->entityMobilityContext);
}

#if defined (ENABLE_AT_ENG_MODE)
/****************************************************************************
 *
 * Function:    ptrToEngineeringModeContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 ****************************************************************************/

EngineeringModeContext_t*  ptrToEngineeringModeContext ( void )
{
  return (&ciEntities.engineeringModeContext);
}
#endif

 /****************************************************************************
 *
 * Function:    ptrToOpManContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 * ATTENTION: After calling this function, please check that the returned pointer is valid!!!
 *
 ****************************************************************************/

OpmanContext_t*  ptrToOpManContext ( const VgmuxChannelNumber entity )
{
  if (ptrToEntityContextData (entity) == PNULL)
  {
    return PNULL;
  }
  return (&ptrToEntityContextData(entity)->opManContext);
}

 /****************************************************************************
 *
 * Function:    ptrToOpManGenericContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 ****************************************************************************/

OpmanGenericContext_t*  ptrToOpManGenericContext (void)
{
  return (&ciEntities.opmanGenericContext);
}

/****************************************************************************
 *
 * Function:    ptrToGprsContext
 *
 * Parameters:
 *
 * Returns:     GprsContext_t
 *
 * Description:
 *
 * ATTENTION: After calling this function, please check that the returned pointer is valid!!!
 *
 ****************************************************************************/

GprsContext_t* ptrToGprsContext ( const VgmuxChannelNumber entity )
{
  if (ptrToEntityContextData (entity) == PNULL)
  {
    return PNULL;
  }
  return (&ptrToEntityContextData(entity)->gprsContext);
}

 /****************************************************************************
 *
 * Function:    ptrToGprsGenericContext
 *
 * Parameters:
 *
 * Returns:     pointer to generic GPRS context data
 *
 * Description:
 *
 ****************************************************************************/

GprsGenericContext_t* ptrToGprsGenericContext ( void )
{
  return (&ciEntities.gprsGenericContext);
}

 /****************************************************************************
 *
 * Function:    ptrToSmsContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 * ATTENTION: After calling this function, please check that the returned pointer is valid!!!
 *
 ****************************************************************************/
SmsContext_t* ptrToSmsContext ( const VgmuxChannelNumber entity )
{
  if (ptrToEntityContextData (entity) == PNULL)
  {
    return PNULL;
  }
  return (&ptrToEntityContextData(entity)->smsContext);
}

 /****************************************************************************
 *
 * Function:    ptrToSmsCommonContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 ****************************************************************************/

SmsCommonContext_t* ptrToSmsCommonContext ( void )
{
  return (&ciEntities.smsCommonContext);
}

 /****************************************************************************
 *
 * Function:    ptrToCallContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 * ATTENTION: After calling this function, please check that the returned pointer is valid!!!
 *
 ****************************************************************************/

CallContext_t* ptrToCallContext ( const VgmuxChannelNumber entity )
{
  if (ptrToEntityContextData (entity) == PNULL)
  {
    return PNULL;
  }
  return (&ptrToEntityContextData(entity)->callContext);
}

 /****************************************************************************
 *
 * Function:    ptrToProfileGenericContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 ****************************************************************************/

ProfileGenericContext_t* ptrToProfileGenericContext ( void )
{
  return (&ciEntities.profileGenericContext);
}


 /****************************************************************************
 *
 * Function:    ptrToSupplementaryContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 * ATTENTION: After calling this function, please check that the returned pointer is valid!!!
 *
 ****************************************************************************/

SupplementaryContext_t* ptrToSupplementaryContext ( const VgmuxChannelNumber entity )
{
  if (ptrToEntityContextData (entity) == PNULL)
  {
    return PNULL;
  }
  return (&ptrToEntityContextData(entity)->supplementaryContext);
}

 /****************************************************************************
 *
 * Function:    ptrToSSCallControlContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 ****************************************************************************/

SsCallRelatedContext_t* ptrToSsCallRelatedContext ( void )
{
  return (&ciEntities.ssCallRelatedContext);
}

 /****************************************************************************
 *
 * Function:    ptrToSimLockGenericContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 ****************************************************************************/

SimLockGenericContext_t* ptrToSimLockGenericContext ( void )
{
  return (&ciEntities.simLockGenericContext);
}

 /****************************************************************************
 *
 * Function:    ptrToSimLockContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 * ATTENTION: After calling this function, please check that the returned pointer is valid!!!
 *
 ****************************************************************************/

SimLockContext_t* ptrToSimLockContext( const VgmuxChannelNumber entity )
{
  if (ptrToEntityContextData (entity) == PNULL)
  {
    return PNULL;
  }
  return (&ptrToEntityContextData(entity)->simLockContext);
}

 /****************************************************************************
 *
 * Function:    allocateMemToEntity
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 ****************************************************************************/

void allocateMemToEntity ( const VgmuxChannelNumber entity, void  *dPtr )
{
  FatalAssert ((Int8)entity < CI_MAX_ENTITIES);

  ciEntities.entitySpecificData[entity] = dPtr;
  ciEntities.entityActive[entity] = TRUE;
}

 /****************************************************************************
 *
 * Function:    initialiseMemToEntity
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 ****************************************************************************/

void initialiseMemToEntity ( const VgmuxChannelNumber entity )
{
  FatalAssert ((Int8)entity < CI_MAX_ENTITIES);

  ciEntities.entityActive[entity] = FALSE;
  ciEntities.entitySpecificData[entity] = PNULL;
}

 /****************************************************************************
 *
 * Function:    allocateMemToGprsContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description: This function is used to add RAM to a cid or to initialise the
 * cid to PNULL.  Called when in static or dynamic mode.
 *
 ****************************************************************************/

void allocateMemToGprsContext ( const Int32 thisCid, void  *dPtr )
{
  /* job127550: allow for transient CID */
  FatalAssert (thisCid < MAX_NUMBER_OF_CIDS);

  ciEntities.gprsGenericContext.cidUserData [thisCid] = dPtr;
}

 /****************************************************************************
 *
 * Function:    ptrToCustomGenericContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 ****************************************************************************/

CustomGenericContext_t* ptrToCustomGenericContext ( void )
{
  return (&ciEntities.customGenericContext);
}

 /****************************************************************************
 *
 * Function:    ptrToCustomContext
 *
 * Parameters:
 *
 * Returns:     Nothing
 *
 * Description:
 *
 * ATTENTION: After calling this function, please check that the returned pointer is valid!!!
 *
 ****************************************************************************/

CustomContext_t* ptrToCustomContext ( const VgmuxChannelNumber entity )
{
  if (ptrToEntityContextData (entity) == PNULL)
  {
    return PNULL;
  }
  return (&ptrToEntityContextData(entity)->customContext);
}
 #ifdef ENABLE_AP_BRIDGE_FEATURE
 /****************************************************************************
 *
 * Function:    ptrToApBridgeContext
 *
 * Parameters:  entity - channel number
 *
 * Returns:     The pointer which points to AP Bridge context.
 *
 * Description: Get the AP Bridge context for specific channel.
 *
 * ATTENTION: After calling this function, please check that the returned pointer is valid!!!
 *
 ****************************************************************************/
 ApBridgeContext_t* ptrToApBridgeContext( VgmuxChannelNumber entity )
 {
   if (ptrToEntityContextData (entity) == PNULL)
   {
     return PNULL;
   }
   return (&ptrToEntityContextData(entity)->apBridgeContext);
 }

 /****************************************************************************
 *
 * Function:    ptrToApBridgeGenericContext
 *
 * Parameters:  None
 *
 * Returns:     The pointer which points to AP Bridge generic context.
 *
 * Description: Return the pointer which points to AP Bridge generic context.
 *
 * ATTENTION: The return pointer is always valid, so, no need to judge whether the returned
 *            pointer is null or not.
 ****************************************************************************/
 ApBridgeGenericContext_t* ptrToApBridgeGenericContext( void )
 {
   return (&ciEntities.apBridgeGenericContext);
 }
 #endif
/****************************************************************************
 *
 * Function:    initMcalContextData
 *
 * Parameters:  VgMcalContext       *mcalContext      - MCAL context pointer
 *
 * Returns:     Nothing
 *
 * Description: Init MCAL context data.
 *
 ****************************************************************************/
void initMcalContextData         ( VgMcalContext       *mcalContext_p)
{
  if(mcalContext_p != PNULL)
  { 
    memset((Int8*)(mcalContext_p), 0 , sizeof (VgMcalContext));
  }
}

