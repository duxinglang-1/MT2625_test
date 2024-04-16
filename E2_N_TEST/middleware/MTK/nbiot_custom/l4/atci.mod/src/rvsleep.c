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
 *  ATCI deep sleep functions.
 **************************************************************************/
  
#define MODULE_NAME "RVSLEEP"

/***************************************************************************
 * Include Files
 **************************************************************************/
#include <memory_attribute.h>
#include <rvsleep.h>
#include <ki_sig.h>
#include <gkisig.h>
#include <rvsystem.h>
#include <rvprof.h>
#include <rvcrhand.h>
#include <rvcimux.h>
#include <rvgput.h>
#include <rvapss.h>
#include <rvccss.h>
#include <rvcrman.h>
#include <rvctss.h>
#include <rvgnss.h>
#include <rvgpss.h>
#include <rvmmss.h>
#include <rvmsdata.h>
#include <rvoman.h>
#include <rvpfss.h>
#include <rvslss.h>
#include <rvssss.h>

#if defined (MTK_NVDM_MODEM_ENABLE)
#include <nvdm_tool.h>
#endif

/***************************************************************************
 * Signal definitions
 ***************************************************************************/
union Signal
{
  KiInitialiseTask       initialise;
#if defined (ENABLE_ATCI_UNIT_TEST)
  CiUnitTestSleepModeInd  ciUnitTestSleepModeInd;
#endif
};

/*************************************************************************
 * Types
 *************************************************************************/
 
typedef struct MobilityContextRtcRamDataTag
{
  MOBILITY_CONTEXT_RTC_RAM_DATA;
}
MobilityContextRtcRamData;

typedef struct GprsGenericContextRtcRamDataTag
{
  GPRS_GENERIC_CONTEXT_RTC_RAM_DATA;
}
GprsGenericContextRtcRamData;

typedef struct EntityGenericContextRtcRamDataTag
{
  ENTITY_GENERIC_CONTEXT_RTC_RAM_DATA;
}
EntityGenericContextRtcRamData;

typedef struct EntityApbContextRtcRamDataTag
{
  ENTITY_APB_CONTEXT_RTC_RAM_DATA;
}
EntityApbContextRtcRamData;

/*************************************************************************
 * Variables
 *************************************************************************/
#if defined (ENABLE_ATCI_UNIT_TEST)
Boolean rebootAtci = FALSE;
#endif

ATTR_MD_RWDATA_IN_RETSRAM MobilityContextRtcRamData mobilityContextRtcRamData;
ATTR_MD_RWDATA_IN_RETSRAM GprsGenericContextRtcRamData gprsGenericContextRtcRamData;
ATTR_MD_RWDATA_IN_RETSRAM EntityGenericContextRtcRamData entityGenericContextRtcRamData;
ATTR_MD_RWDATA_IN_RETSRAM EntityApbContextRtcRamData entityApbContextRtcRamData;

ATTR_MD_NONINIT_DATA_IN_RAM RvNvramGenericData   mNvramGenericDataCache;
EntityNvContext_t    tempEntityData;  


/*************************************************************************
 * Macros
 *************************************************************************/

/*************************************************************************
 * Local Function Prototypes
 *************************************************************************/
static RvNvramResult RvReadFromNvramToCache( RvNvDataType rvNvDataType );
static RvNvramResult RvWriteGenericDataToNvram( RvNvDataType rvNvDataType );
static Boolean       RvUpdateEntityData(Int8 previousNumEnabledChannels);
static Boolean       RvUpdateCidData(void);
static void RvReinstateEntityData(void);
void InitialiseSleepManContextData(ControlResetCause powerOnCause);

#if defined (ENABLE_ATCI_UNIT_TEST)
static void vgSigCiUnitTestSleepModeInd (const SignalBuffer *signal_p);
#endif

/*************************************************************************
 * Local Functions
 *************************************************************************/

/******************************************************************************
  *
  * Function     : InitialiseSleepManContextData
  *
  * Scope        : LOCAL
  *
  * Parameters   : control reset cause
  *
  * Returns      : -
  *
  * Description  :  update the context data based upon cause
  *
 *****************************************************************************/

void InitialiseSleepManContextData(ControlResetCause cause)
{
  SleepManContext_t    *sleepManContext_p  = ptrToSleepManContext();
  
  sleepManContext_p->powerOnCause = cause;
  if (cause == RESET_WAKEUP_FROM_DEEP_SLEEP)
  {
#if defined (ATCI_DEBUG)
     printf("ATCI: wakeup cause: WAKEUP_FROM_DEEP_SLEEP");
#endif
     sleepManContext_p->firstSleep = FALSE;
     sleepManContext_p->atciInWakeupState = TRUE;
     sleepManContext_p->atciWokeUp = TRUE;
     sleepManContext_p->needNetworkStateInd = TRUE;
     sleepManContext_p->needSimNok = TRUE;    
     sleepManContext_p->needSimOk = TRUE;    
     sleepManContext_p->needSmReadyInd = TRUE;         

     /* For now don't check for these others - but may add later */     
     sleepManContext_p->needGlReadyInd = FALSE;         
#if defined (FEA_PHONEBOOK)     
     sleepManContext_p->needLmReadyInd = FALSE;         
#endif
  }
  else
  { 
     sleepManContext_p->firstSleep = TRUE;
     sleepManContext_p->atciInWakeupState = FALSE;
     sleepManContext_p->atciWokeUp = FALSE;
     sleepManContext_p->needNetworkStateInd = FALSE;
     sleepManContext_p->needSimNok = FALSE;    
     sleepManContext_p->needSimOk = FALSE;    
     sleepManContext_p->needSmReadyInd = FALSE;         

     sleepManContext_p->needGlReadyInd = FALSE;         
#if defined (FEA_PHONEBOOK)     
     sleepManContext_p->needLmReadyInd = FALSE;         
#endif
  }
  /* for deep sleep we need to set this to number of STATUS_INDs needed when we
   * know how many we should be expecting
   */
  sleepManContext_p->numPsdBearerStatusIndsNeeded = 0;
  
}


#if defined (ENABLE_ATCI_UNIT_TEST)
/******************************************************************************
  *
  * Function     : vgResetAllAtciStructures
  *
  * Scope        : LOCAL
  *
  * Parameters   : None
  *
  * Returns      : -
  *
  * Description  : Resets all ATCI data after deep sleep request - for unit
  *                testing.
  *
  *****************************************************************************/
void vgResetAllAtciStructures(void)
{
   Entity_t               *entityGeneric_p = ptrToAtciContextData();

   memset(entityGeneric_p,0, sizeof(Entity_t));
}

/******************************************************************************
  *
  * Function     : vgSigCiUnitTestSleepModeInd
  *
  * Scope        : LOCAL
  *
  * Parameters   : SignalBuffer *signal_p
  *
  * Returns      : -
  *
  * Description  : Actions request for deep sleep or wake-up
  *
  *****************************************************************************/

 static void vgSigCiUnitTestSleepModeInd (const SignalBuffer *signal_p)
 {
   SignalBuffer              sigBuff = kiNullBuffer;
   CiUnitTestSleepModeRsp    *reply_p;
   Boolean                   result;


    if (signal_p->sig->ciUnitTestSleepModeInd.wakeUp == TRUE)
    {
      RvWakeUpFromSleep();
    }
    else
    {
      result = RvPrepareForSleep();
      if (result == TRUE)
      {
          RvGoToSleep();
      }
      KiCreateZeroSignal( SIG_CI_UNIT_TEST_SLEEP_MODE_RSP,
                         sizeof (CiUnitTestSleepModeRsp),
                          &sigBuff);

      reply_p = (CiUnitTestSleepModeRsp *)sigBuff.sig;
      reply_p->success = result;
      KiSendSignal (VG_CI_TASK_ID, &sigBuff); 

       /* Cause ATCI to start again from the top */
      rebootAtci = TRUE;
    }
 }
#endif

/******************************************************************************
 *
 * Function     : RvReinstateEntityData
 *
 * Scope        : LOCAL
 *
 * Parameters   : None
 *
 * Returns      : -
 *
 * Description  :  for each enabled entity allocate RAM for the entity then populate with results of
 * the store of NVRAM data.
 *
 *****************************************************************************/
static void RvReinstateEntityData(void)
{
   Entity_t               *entityGeneric_p = ptrToAtciContextData();
   EntityContextData_t*    entityData_p = PNULL;
   Int8                    entityNum;
   Int8                    activeEntityIndex = 0;

   memset(&tempEntityData,0, sizeof(EntityNvContext_t));
   for (entityNum = 0; entityNum < CI_MAX_ENTITIES; entityNum++)
   {
      /* Check if this entity was active before - if so then read that data from
       * NVRAM.
       * NOTE: Active channels ALWAYS stored IN SEQUENCE, otherwise this won't
       * work
       */
      if (entityGeneric_p->entityActive[entityNum])
      {
#if defined(ATCI_DEBUG)
        printf ("ATCI: Re-activating entity %d, %d", entityNum, activeEntityIndex);
#endif
        /* if there is info for this entity.
         * Basically just check if there is something other than 0 there
         */
         
        /* Check if we've run out of entity stores */
        FatalAssert (activeEntityIndex < ATCI_MAX_NUM_ENABLED_AT_CHANNELS);        
        
        if (memcmp (&mNvramGenericDataCache.entityNvData[activeEntityIndex], &tempEntityData, sizeof(EntityNvContext_t)) !=0)
        {
             entityData_p = ptrToEntityContextData (entityNum);
             if  (PNULL == entityData_p)
             {        
                entityData_p = vgAllocateRamToEntity();
             }
#if defined (ATCI_SLIM_DISABLE)

             FatalCheck(PNULL != entityData_p, entityNum, activeEntityIndex, 0);

#endif

             if (PNULL != entityData_p)
             {
                allocateMemToEntity (entityNum, (void *)entityData_p);

                /* Now initialise the entity data */
                initialiseApSsEntitySpecific (entityNum);
                initialiseCcSsEntitySpecific (entityNum);
                initialiseCrMan (entityNum);
                initialiseGnss (entityNum);
                initialiseGpss (entityNum);

                initialiseMmss (entityNum);
                vgSmsInitialiseData (entityNum);
                vgOpManInitialise (entityNum, FALSE);
                initialisePfss (entityNum);
                initialiseSlssEntitySpecificData (entityNum);
                initialiseSsssEntitySpecificData (entityNum);

                memcpy(&entityData_p->channelContext.cmuxCmdParams, 
                       &mNvramGenericDataCache.entityNvData[activeEntityIndex].cmuxCmdParams, sizeof(CmuxCmdParams));
                memcpy(&entityData_p->profileContext,
                       &mNvramGenericDataCache.entityNvData[activeEntityIndex].entityProfileData, sizeof(ProfileContext_t));
                entityData_p->nvramContext.readProfileFlag = 
                       mNvramGenericDataCache.entityNvData[activeEntityIndex].nvrmReadfProfileFlag;
                entityData_p->nvramContext.writeProfileFlag = 
                       mNvramGenericDataCache.entityNvData[activeEntityIndex].nvrmWriteProfileFlag; 
                entityData_p->smsContext.enableExtraUnsol = 
                       mNvramGenericDataCache.entityNvData[activeEntityIndex].smsEnableExtraUnsol; 
                entityData_p->gprsContext.connectionActive = 
                       mNvramGenericDataCache.entityNvData[activeEntityIndex].gprsContextConnectionActive;

#if defined(ATCI_DEBUG)
                printf ("ATCI: Re-activated entity %d, %d", entityNum, activeEntityIndex);
#endif
             }
        }
        activeEntityIndex++;
      }        
   }
}

/******************************************************************************
 *
 * Function     : RvReadFromNvramToCache
 *
 * Scope        : LOCAL
 *
 * Parameters   : None
 *
 * Returns      : -
 *
 * Description  : Reads data from NVRAM to RAM variable.
 *
 *****************************************************************************/
static RvNvramResult RvReadFromNvramToCache( RvNvDataType nvDataType )
{

    uint32_t       size = 0;
    RvNvramResult  readResult = RV_NVRAM_RESULT_OK;

#if defined (MTK_NVDM_MODEM_ENABLE)
    nvdm_modem_data_item_type_t type = NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA;

    
    nvdm_modem_status_t  status = NVDM_MODEM_STATUS_OK;

    switch (nvDataType)
    {
      case RV_MUX_CONTEXT_DATA:
        size = sizeof(MuxContext_t);
        status = nvdm_modem_read_normal_data_item( RV_NVRAM_GENERIC_DATA_GROUP, RV_NVRAM_MUX_CONTEXT_DATA, 
                  &type,
                 (uint8_t*) &mNvramGenericDataCache.muxContextData, &size );
        break;

      case RV_OPMAN_DATA:
        size = sizeof(OpmanGenericContext_t);
        status = nvdm_modem_read_normal_data_item( RV_NVRAM_GENERIC_DATA_GROUP, RV_NVRAM_OPMAN_DATA, 
                  &type,          
                 (uint8_t*) &mNvramGenericDataCache.opmanGenericData, &size );
        break;

      case RV_PROFILE_DATA:
        size = sizeof(ProfileGenericContext_t);
        status = nvdm_modem_read_normal_data_item( RV_NVRAM_GENERIC_DATA_GROUP, RV_NVRAM_PROFILE_DATA, 
                  &type,          
                 (uint8_t*) &mNvramGenericDataCache.profileGenericData, &size );
        break; 

      case RV_CHMAN_DATA:
        size = sizeof(ChManagerContext_t);
        status = nvdm_modem_read_normal_data_item( RV_NVRAM_GENERIC_DATA_GROUP, RV_NVRAM_CHMAN_DATA, 
                  &type,          
                 (uint8_t*) &mNvramGenericDataCache.chmanData, &size );
        break;
      
      case RV_MOBILITY_DATA:
        size = sizeof(MobilityNv_t);
        status = nvdm_modem_read_normal_data_item( RV_NVRAM_GENERIC_DATA_GROUP, RV_NVRAM_MOBILITY_DATA,
                  &type,          
                 (uint8_t*) &mNvramGenericDataCache.mobilityData, &size );
        break;
      
      case RV_SIMLOCK_DATA:
        size = sizeof(SimLockNvContext_t);
        status = nvdm_modem_read_normal_data_item( RV_NVRAM_GENERIC_DATA_GROUP, RV_NVRAM_SIMLOCK_DATA, 
                  &type,        
                  (uint8_t*) &mNvramGenericDataCache.simLockData, &size );
        break;

      case RV_STK_DATA:
        size = sizeof(StkNvGenericContext_t);
        status = nvdm_modem_read_normal_data_item( RV_NVRAM_GENERIC_DATA_GROUP, RV_NVRAM_STK_DATA, 
                  &type,          
                  (uint8_t*) &mNvramGenericDataCache.stkData, &size );
        break;
      
      case RV_GENERAL_DATA:
        size = sizeof(GeneralNvGenericContext_t);
        status = nvdm_modem_read_normal_data_item( RV_NVRAM_GENERIC_DATA_GROUP, RV_NVRAM_GENERAL_DATA, 
                  &type,          
                  (uint8_t*) &mNvramGenericDataCache.generalGenericData, &size );
        break;
      
      case RV_PD_CONTEXT_DATA:
        size = sizeof(GprsNvGeneric_t);
        status = nvdm_modem_read_normal_data_item( RV_NVRAM_GENERIC_DATA_GROUP, RV_NVRAM_PD_CONTEXT_DATA,
                  &type,          
                 (uint8_t*) &mNvramGenericDataCache.pdContextData, &size );
        break;
    

      case RV_ENTITY_SPECIFIC_DATA:
        size = sizeof(EntityNvContext_t);
        status = nvdm_modem_read_normal_data_item(RV_NVRAM_ENTITY_DATA_GROUP,
                           RV_NVRAM_ACTIVE_ENTITY_1,
                           &type,                            
                           (uint8_t *) &mNvramGenericDataCache.entityNvData[0],
                           &size);
        if (status != NVDM_MODEM_STATUS_OK)
        {
            FatalParam(1, status, size);
        }
        status = nvdm_modem_read_normal_data_item(RV_NVRAM_ENTITY_DATA_GROUP,
                           RV_NVRAM_ACTIVE_ENTITY_2,
                           &type,                            
                           (uint8_t *) &mNvramGenericDataCache.entityNvData[1],
                           &size);
        if (status != NVDM_MODEM_STATUS_OK)
        {
            FatalParam(2, status, size);
        }        
        status = nvdm_modem_read_normal_data_item(RV_NVRAM_ENTITY_DATA_GROUP,
                           RV_NVRAM_ACTIVE_ENTITY_3,
                           &type,                            
                           (uint8_t *) &mNvramGenericDataCache.entityNvData[2],
                           &size);
        if (status != NVDM_MODEM_STATUS_OK)
        {
            FatalParam(3, status, size);
        } 
        status = nvdm_modem_read_normal_data_item(RV_NVRAM_ENTITY_DATA_GROUP,
                           RV_NVRAM_ACTIVE_ENTITY_4,
                           &type,                            
                           (uint8_t *) &mNvramGenericDataCache.entityNvData[3],
                           &size);
        if (status != NVDM_MODEM_STATUS_OK)
        {
            FatalParam(4, status, size);
        }
        status = nvdm_modem_read_normal_data_item(RV_NVRAM_ENTITY_DATA_GROUP,
                           RV_NVRAM_ACTIVE_ENTITY_5,
                           &type,                            
                           (uint8_t *) &mNvramGenericDataCache.entityNvData[4],
                           &size);
        if (status != NVDM_MODEM_STATUS_OK)
        {
            FatalParam(5, status, size);
        }
        status = nvdm_modem_read_normal_data_item(RV_NVRAM_ENTITY_DATA_GROUP,
                            RV_NVRAM_ACTIVE_ENTITY_6,
                            &type,                            
                            (uint8_t *) &mNvramGenericDataCache.entityNvData[5],
                            &size);
        if (status != NVDM_MODEM_STATUS_OK)
        {
            FatalParam(6, status, size);
        }                            
        status = nvdm_modem_read_normal_data_item(RV_NVRAM_ENTITY_DATA_GROUP,
                           RV_NVRAM_ACTIVE_ENTITY_7,
                           &type,                            
                           (uint8_t *) &mNvramGenericDataCache.entityNvData[6],
                           &size);
        if (status != NVDM_MODEM_STATUS_OK)
        {
            FatalParam(7, status, size);
        }                                    
        status = nvdm_modem_read_normal_data_item(RV_NVRAM_ENTITY_DATA_GROUP,
                           RV_NVRAM_ACTIVE_ENTITY_8,
                           &type,                            
                           (uint8_t *) &mNvramGenericDataCache.entityNvData[7],
                           &size);
        if (status != NVDM_MODEM_STATUS_OK)
        {
            FatalParam(8, status, size);
        }                                    
        status = nvdm_modem_read_normal_data_item(RV_NVRAM_ENTITY_DATA_GROUP,
                           RV_NVRAM_ACTIVE_ENTITY_9,
                           &type,                            
                           (uint8_t *) &mNvramGenericDataCache.entityNvData[8],
                           &size);
        if (status != NVDM_MODEM_STATUS_OK)
        {
            FatalParam(9, status, size);
        }                                    
        status = nvdm_modem_read_normal_data_item(RV_NVRAM_ENTITY_DATA_GROUP,
                           RV_NVRAM_ACTIVE_ENTITY_10,
                           &type,                            
                           (uint8_t *) &mNvramGenericDataCache.entityNvData[9],
                           &size);
        if (status != NVDM_MODEM_STATUS_OK)
        {
            FatalParam(10, status, size);
        }    
        status = nvdm_modem_read_normal_data_item(RV_NVRAM_ENTITY_DATA_GROUP,
                           RV_NVRAM_ACTIVE_ENTITY_11,
                           &type,                            
                           (uint8_t *) &mNvramGenericDataCache.entityNvData[10],
                           &size);
        if (status != NVDM_MODEM_STATUS_OK)
        {
            FatalParam(11, status, size);
        }                                    
        break;

      case RV_CID_DATA:
        size = (sizeof(CidNvInfo_t)*MAX_NUMBER_OF_CIDS);
        status = nvdm_modem_read_normal_data_item(RV_NVRAM_PSD_CONN_DATA_GROUP,
                               RV_NVRAM_PSD_CONN_DATA,
                               &type,                            
                               (uint8_t*)mNvramGenericDataCache.cidInfo,&size);
        if (status != NVDM_MODEM_STATUS_OK)
        {
           FatalParam(status, size, 0);                      
        }
        break;
        
      default:
        status  = NVDM_MODEM_STATUS_ERROR;
        break;     
   }
   if (status !=  NVDM_MODEM_STATUS_OK)
   {
     readResult =  RV_NVRAM_RESULT_FAILED;
     /* Also assert so we know if NVRAM read falied */
     FatalParam(status, nvDataType, size);
   }
 #endif
   return(readResult);      
}

/******************************************************************************
 *
 * Function     : RVWriteCacheToNvram
 *
 * Scope        : LOCAL
 *
 * Parameters   : None
 *
 * Returns      : Result
 *
 * Description  : Writes data from RAM variables to NVRAM.
 *
 *****************************************************************************/
static RvNvramResult RvWriteGenericDataToNvram(  RvNvDataType nvDataType )
{
   RvNvramResult writeResult = RV_NVRAM_RESULT_OK;

#if defined (MTK_NVDM_MODEM_ENABLE)
  nvdm_modem_status_t nvdmStatus;   
    
   switch (nvDataType)
   {
      case RV_MUX_CONTEXT_DATA:        
        nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_GENERIC_DATA_GROUP,
                            RV_NVRAM_MUX_CONTEXT_DATA,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.muxContextData,
                            sizeof(MuxContext_t),
                            NVDM_MODEM_ATTR_AVERAGE);
                                  
        break;
      case RV_OPMAN_DATA:
        nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_GENERIC_DATA_GROUP,
                            RV_NVRAM_OPMAN_DATA,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.opmanGenericData,
                            sizeof(OpmanGenericContext_t),
                            NVDM_MODEM_ATTR_AVERAGE);
        break;

      case RV_PROFILE_DATA:
        nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_GENERIC_DATA_GROUP,
                            RV_NVRAM_PROFILE_DATA,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.profileGenericData,
                            sizeof(ProfileGenericContext_t),
                            NVDM_MODEM_ATTR_AVERAGE);
        break;

        
      case RV_CHMAN_DATA:
        nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_GENERIC_DATA_GROUP,
                            RV_NVRAM_CHMAN_DATA,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.chmanData,
                            sizeof(ChManagerContext_t),
                            NVDM_MODEM_ATTR_AVERAGE);
                            
        break;

        
      case RV_MOBILITY_DATA:
        nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_GENERIC_DATA_GROUP,
                            RV_NVRAM_MOBILITY_DATA,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.mobilityData,
                            sizeof(MobilityNv_t),
                            NVDM_MODEM_ATTR_AVERAGE);
                            
        break;
         
      case RV_SIMLOCK_DATA:
        nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_GENERIC_DATA_GROUP,
                            RV_NVRAM_SIMLOCK_DATA,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.simLockData,
                            sizeof(SimLockNvContext_t),
                            NVDM_MODEM_ATTR_AVERAGE);
                             
        break;

      case RV_STK_DATA:
        nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_GENERIC_DATA_GROUP,
                            RV_NVRAM_STK_DATA,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.stkData,
                            sizeof(StkNvGenericContext_t),
                            NVDM_MODEM_ATTR_AVERAGE);
                            
        break;
           
      case RV_GENERAL_DATA:
        nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_GENERIC_DATA_GROUP,
                            RV_NVRAM_GENERAL_DATA,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.generalGenericData,
                             sizeof(GeneralNvGenericContext_t),
                            NVDM_MODEM_ATTR_AVERAGE);                             
        break;
          
      case RV_PD_CONTEXT_DATA:
        nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_GENERIC_DATA_GROUP,
                            RV_NVRAM_PD_CONTEXT_DATA,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.pdContextData,
                            sizeof(GprsNvGeneric_t),
                            NVDM_MODEM_ATTR_AVERAGE);                            
        break;
       
      default:
        /* toDo need to handle entity specific data */
       nvdmStatus = NVDM_MODEM_STATUS_ERROR;
      break;
   }

   if (nvdmStatus !=  NVDM_MODEM_STATUS_OK)
   {
     writeResult =  RV_NVRAM_RESULT_FAILED;

     /* Also assert so we know if NVRAM write falied */
     FatalParam(nvdmStatus, nvDataType, 0);
   }

#endif
   return(writeResult);      
}

/******************************************************************************
 *
 * Function     : RvUpdateEntityData
 *
 * Scope        : LOCAL
 *
 * Parameters   : Previous number of enabled channels (on last wakeup)
 *
 * Returns      : Boolean Result
 *
 * Description  : Updates store with Entity specific data then writes it to NVRAM
 *
 *****************************************************************************/

static Boolean RvUpdateEntityData(Int8 previousNumEnabledChannels)
{  
   SleepManContext_t    *sleepManContext_p  = ptrToSleepManContext();
   Boolean              updateSuccess = TRUE;
   
#if defined (MTK_NVDM_MODEM_ENABLE)
   Entity_t             *entityGeneric_p = ptrToAtciContextData();
   EntityContextData_t  *entity_p;
   VgmuxChannelNumber   currentChannelNumber; 
   Int8                 activeEntityIndex = 0;
   Int8                 remainingEntityIndex;
   Int8                 remainingEntityNum;
 
   nvdm_modem_status_t  nvdmStatus = NVDM_MODEM_STATUS_OK;

   Boolean              entityActive = FALSE;

   for (currentChannelNumber = 0; currentChannelNumber < CI_MAX_ENTITIES; currentChannelNumber++)
   {
       /* clear temp store so we overwrite entries that are no longer valid */
       memset(&tempEntityData,0, sizeof(EntityNvContext_t));
       entity_p = ptrToEntityContextData(currentChannelNumber);
       if (entity_p != PNULL)
       {
          /* Entity MUST be marked as active otherwise something has gone wrong */
          if (!entityGeneric_p->entityActive[currentChannelNumber])
          {
            FatalParam(currentChannelNumber, activeEntityIndex, 0);
          }            
          entityActive = TRUE;
          memcpy(&tempEntityData.cmuxCmdParams, &entity_p->channelContext.cmuxCmdParams, sizeof(CmuxCmdParams));
          memcpy(&tempEntityData.entityProfileData, &entity_p->profileContext, sizeof(ProfileContext_t));
          tempEntityData.nvrmReadfProfileFlag = entity_p->nvramContext.readProfileFlag;
          tempEntityData.nvrmWriteProfileFlag = entity_p->nvramContext.writeProfileFlag;
          tempEntityData.smsEnableExtraUnsol = entity_p->smsContext.enableExtraUnsol;
          tempEntityData.gprsContextConnectionActive = entity_p->gprsContext.connectionActive;
          tempEntityData.currentAtCommand = entity_p->crManagerContext.currentAtCommand;
#if defined (ENABLE_ATCI_UNIT_TEST) || defined(ATCI_DEBUG)
          printf("ATCI sleep: Entity active: %d, %d, %d", currentChannelNumber, activeEntityIndex,
                 memcmp (&mNvramGenericDataCache.entityNvData[activeEntityIndex], &tempEntityData, sizeof(EntityNvContext_t)));
#endif
          
       }
       else
       {
          entityActive = FALSE;
       }


       /* If first sleep - write all active entities anyway */
       if (entityActive && ((sleepManContext_p->firstSleep) ||
           (memcmp (&mNvramGenericDataCache.entityNvData[activeEntityIndex], &tempEntityData, sizeof(EntityNvContext_t)) !=0)))
       {

#if defined (ENABLE_ATCI_UNIT_TEST) || defined(ATCI_DEBUG)
          printf("ATCI sleep: Writing entity data to NVRAM: %d, %d, %d", currentChannelNumber, activeEntityIndex, sizeof(EntityNvContext_t));
#endif
         /* update the store in case don't go to sleep */
          memcpy (&mNvramGenericDataCache.entityNvData[activeEntityIndex], &tempEntityData, sizeof(EntityNvContext_t));
          switch (activeEntityIndex)
          {
            case 0:
              nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_ENTITY_DATA_GROUP,
                            RV_NVRAM_ACTIVE_ENTITY_1,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.entityNvData[0],
                             sizeof(EntityNvContext_t),
                            NVDM_MODEM_ATTR_AVERAGE);                             
              break;

            case 1:
              nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_ENTITY_DATA_GROUP,
                            RV_NVRAM_ACTIVE_ENTITY_2,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.entityNvData[1],
                             sizeof(EntityNvContext_t),
                            NVDM_MODEM_ATTR_AVERAGE);                             
              break;           

            case 2:
              nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_ENTITY_DATA_GROUP,
                            RV_NVRAM_ACTIVE_ENTITY_3,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.entityNvData[2],
                             sizeof(EntityNvContext_t),
                            NVDM_MODEM_ATTR_AVERAGE);                             
              break;   

            case 3:
              nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_ENTITY_DATA_GROUP,
                            RV_NVRAM_ACTIVE_ENTITY_4,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.entityNvData[3],
                             sizeof(EntityNvContext_t),
                            NVDM_MODEM_ATTR_AVERAGE);                             
              break;   

            case 4:
              nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_ENTITY_DATA_GROUP,
                            RV_NVRAM_ACTIVE_ENTITY_5,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.entityNvData[4],
                             sizeof(EntityNvContext_t),
                            NVDM_MODEM_ATTR_AVERAGE);                           
              break;
           
            case 5:
              nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_ENTITY_DATA_GROUP,
                            RV_NVRAM_ACTIVE_ENTITY_6,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.entityNvData[5],
                             sizeof(EntityNvContext_t),
                            NVDM_MODEM_ATTR_AVERAGE);                             
              break;
 
            case 6:
              nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_ENTITY_DATA_GROUP,
                            RV_NVRAM_ACTIVE_ENTITY_7,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.entityNvData[6],
                             sizeof(EntityNvContext_t),
                            NVDM_MODEM_ATTR_AVERAGE);                             
              break; 

            case 7:
              nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_ENTITY_DATA_GROUP,
                            RV_NVRAM_ACTIVE_ENTITY_8,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.entityNvData[7],
                             sizeof(EntityNvContext_t),                            
                            NVDM_MODEM_ATTR_AVERAGE);
                             
              break; 

            case 8:
              nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_ENTITY_DATA_GROUP,
                            RV_NVRAM_ACTIVE_ENTITY_9,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.entityNvData[8],
                             sizeof(EntityNvContext_t),
                            NVDM_MODEM_ATTR_AVERAGE);                             
              break; 

            case 9:
              nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_ENTITY_DATA_GROUP,
                            RV_NVRAM_ACTIVE_ENTITY_10,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.entityNvData[9],
                             sizeof(EntityNvContext_t),
                            NVDM_MODEM_ATTR_AVERAGE);                             
              break; 

            case 10:
              nvdmStatus = nvdm_modem_write_normal_data_item
                           (RV_NVRAM_ENTITY_DATA_GROUP,
                            RV_NVRAM_ACTIVE_ENTITY_11,
                            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                            (uint8_t *) &mNvramGenericDataCache.entityNvData[10],
                             sizeof(EntityNvContext_t),
                            NVDM_MODEM_ATTR_AVERAGE);                             
              break; 
            default:
              /* Entity value out of range - ASSERT! */
              FatalParam(currentChannelNumber, activeEntityIndex, entityActive);
//              nvdmStatus = NVDM_MODEM_STATUS_ERROR;
              break;
        }

        if (nvdmStatus != NVDM_MODEM_STATUS_OK)
        {
          updateSuccess = FALSE;
          FatalParam(nvdmStatus, previousNumEnabledChannels, activeEntityIndex);
        }
      }

      if (entityActive)
      {
        activeEntityIndex++;
        /* Check if we've run out of entity stores */
        FatalAssert (activeEntityIndex <= ATCI_MAX_NUM_ENABLED_AT_CHANNELS);
      }         
    }

   /* If first sleep and we have not written all NVRAM items - we need to write
    * the rest aswell (blank)
    */

   /*****************
    * Need to do this anyway - not just after first sleep - because
    * an entity may have been deactivated which would make them all shuffle.
    * So we would need to clear out the deactivated ones.  Use previousNumEnabledChannels for this..
    */
   if (((sleepManContext_p->firstSleep) || (previousNumEnabledChannels > activeEntityIndex))&&
       (activeEntityIndex < ATCI_MAX_NUM_ENABLED_AT_CHANNELS))
   {
     /* clear temp store so we overwrite entries that are no longer valid */
     memset(&tempEntityData,0, sizeof(EntityNvContext_t));

     if (sleepManContext_p->firstSleep)
     {
       remainingEntityNum = ATCI_MAX_NUM_ENABLED_AT_CHANNELS;
     }
     else
     {
       /* Must have been because previousNumEnableChannels > activeEntityIndex */
       remainingEntityNum = previousNumEnabledChannels;
     }

     for (remainingEntityIndex = activeEntityIndex; remainingEntityIndex < remainingEntityNum; remainingEntityIndex++)
     {
#if defined (ENABLE_ATCI_UNIT_TEST) || defined(ATCI_DEBUG)
       printf("ATCI sleep: Writing REMAINING entity data to NVRAM: %d, %d", currentChannelNumber, remainingEntityIndex);
#endif
      
       /* update the store in case don't go to sleep */
       memcpy (&mNvramGenericDataCache.entityNvData[activeEntityIndex], &tempEntityData, sizeof(EntityNvContext_t));

       switch (remainingEntityIndex)
       {
         case 0:
           nvdmStatus = nvdm_modem_write_normal_data_item
                        (RV_NVRAM_ENTITY_DATA_GROUP,
                         RV_NVRAM_ACTIVE_ENTITY_1,
                         NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                         (uint8_t *) &mNvramGenericDataCache.entityNvData[0],
                          sizeof(EntityNvContext_t),
                         NVDM_MODEM_ATTR_AVERAGE);                             
           break;
 
         case 1:
           nvdmStatus = nvdm_modem_write_normal_data_item
                        (RV_NVRAM_ENTITY_DATA_GROUP,
                         RV_NVRAM_ACTIVE_ENTITY_2,
                         NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                         (uint8_t *) &mNvramGenericDataCache.entityNvData[1],
                          sizeof(EntityNvContext_t),
                         NVDM_MODEM_ATTR_AVERAGE);                             
           break;           
 
         case 2:
           nvdmStatus = nvdm_modem_write_normal_data_item
                        (RV_NVRAM_ENTITY_DATA_GROUP,
                         RV_NVRAM_ACTIVE_ENTITY_3,
                         NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                         (uint8_t *) &mNvramGenericDataCache.entityNvData[2],
                          sizeof(EntityNvContext_t),
                         NVDM_MODEM_ATTR_AVERAGE);                             
           break;   
 
         case 3:
           nvdmStatus = nvdm_modem_write_normal_data_item
                        (RV_NVRAM_ENTITY_DATA_GROUP,
                         RV_NVRAM_ACTIVE_ENTITY_4,
                         NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                         (uint8_t *) &mNvramGenericDataCache.entityNvData[3],
                          sizeof(EntityNvContext_t),
                         NVDM_MODEM_ATTR_AVERAGE);                             
           break;   
 
         case 4:
           nvdmStatus = nvdm_modem_write_normal_data_item
                        (RV_NVRAM_ENTITY_DATA_GROUP,
                         RV_NVRAM_ACTIVE_ENTITY_5,
                         NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                         (uint8_t *) &mNvramGenericDataCache.entityNvData[4],
                          sizeof(EntityNvContext_t),
                         NVDM_MODEM_ATTR_AVERAGE);                           
           break;
        
         case 5:
           nvdmStatus = nvdm_modem_write_normal_data_item
                        (RV_NVRAM_ENTITY_DATA_GROUP,
                         RV_NVRAM_ACTIVE_ENTITY_6,
                         NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                         (uint8_t *) &mNvramGenericDataCache.entityNvData[5],
                          sizeof(EntityNvContext_t),
                         NVDM_MODEM_ATTR_AVERAGE);                             
           break;
 
         case 6:
           nvdmStatus = nvdm_modem_write_normal_data_item
                        (RV_NVRAM_ENTITY_DATA_GROUP,
                         RV_NVRAM_ACTIVE_ENTITY_7,
                         NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                         (uint8_t *) &mNvramGenericDataCache.entityNvData[6],
                          sizeof(EntityNvContext_t),
                         NVDM_MODEM_ATTR_AVERAGE);                             
           break; 
 
         case 7:
           nvdmStatus = nvdm_modem_write_normal_data_item
                        (RV_NVRAM_ENTITY_DATA_GROUP,
                         RV_NVRAM_ACTIVE_ENTITY_8,
                         NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                         (uint8_t *) &mNvramGenericDataCache.entityNvData[7],
                          sizeof(EntityNvContext_t),                            
                         NVDM_MODEM_ATTR_AVERAGE);
                          
           break; 
 
         case 8:
           nvdmStatus = nvdm_modem_write_normal_data_item
                        (RV_NVRAM_ENTITY_DATA_GROUP,
                         RV_NVRAM_ACTIVE_ENTITY_9,
                         NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                         (uint8_t *) &mNvramGenericDataCache.entityNvData[8],
                          sizeof(EntityNvContext_t),
                         NVDM_MODEM_ATTR_AVERAGE);                             
           break; 
 
         case 9:
           nvdmStatus = nvdm_modem_write_normal_data_item
                        (RV_NVRAM_ENTITY_DATA_GROUP,
                         RV_NVRAM_ACTIVE_ENTITY_10,
                         NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                         (uint8_t *) &mNvramGenericDataCache.entityNvData[9],
                          sizeof(EntityNvContext_t),
                         NVDM_MODEM_ATTR_AVERAGE);                             
           break; 

         case 10:
           nvdmStatus = nvdm_modem_write_normal_data_item
                        (RV_NVRAM_ENTITY_DATA_GROUP,
                         RV_NVRAM_ACTIVE_ENTITY_11,
                         NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                         (uint8_t *) &mNvramGenericDataCache.entityNvData[10],
                          sizeof(EntityNvContext_t),
                         NVDM_MODEM_ATTR_AVERAGE);                             
           break; 

         default:
           /* Entity value out of range - ASSERT! */
           FatalParam(currentChannelNumber, activeEntityIndex, entityActive);
//           nvdmStatus = NVDM_MODEM_STATUS_ERROR;
           break;
       }

       if (nvdmStatus != NVDM_MODEM_STATUS_OK)
       {
         updateSuccess = FALSE;
         FatalParam(nvdmStatus, activeEntityIndex, 0);
       }
     }
   }

   #endif
   
   return (updateSuccess); 
}

/******************************************************************************
 *
 * Function     : UpdateCidData
 *
 * Scope        : LOCAL
 *
 * Parameters   : None
 *
 * Returns      : Boolean 
 *
 * Description  : Updates cidInfo then writes to NVRAM
 *
 *****************************************************************************/
static Boolean RvUpdateCidData(void)
 
{
  SleepManContext_t    *sleepManContext_p  = ptrToSleepManContext();
  Boolean              updateSuccess = TRUE;
  
#if defined (MTK_NVDM_MODEM_ENABLE)
  Int8                  currentCidIndex; 
  GprsGenericContext_t  *gprsGenericContext_p = ptrToGprsGenericContext ();
  Boolean               needUpdate = FALSE;

  nvdm_modem_status_t        nvdmStatus = NVDM_MODEM_STATUS_OK;

  for (currentCidIndex =0; currentCidIndex < MAX_NUMBER_OF_CIDS; currentCidIndex++)
  {
     if (gprsGenericContext_p->cidUserData[currentCidIndex] == PNULL)      
     {
       /* Always update if first sleep */
       if ((sleepManContext_p->firstSleep) || (mNvramGenericDataCache.cidInfo[currentCidIndex].isActive))
       {
#if defined (ENABLE_ATCI_UNIT_TEST) || defined(ATCI_DEBUG)
        printf("ATCI Sleep: Need to update CID (now not defined): %d", currentCidIndex);
#endif
         mNvramGenericDataCache.cidInfo[currentCidIndex].isActive = FALSE;
         mNvramGenericDataCache.cidInfo[currentCidIndex].connId = INVALID_CONN_ID;
         needUpdate = TRUE;
       }
     }
     else
     {
       /* Non NULL pointer - so something in there */
       /* Always update if first sleep */
       if ((sleepManContext_p->firstSleep) ||
           (mNvramGenericDataCache.cidInfo[currentCidIndex].isActive != gprsGenericContext_p->cidUserData[currentCidIndex]->isActive))
       {
#if defined (ENABLE_ATCI_UNIT_TEST) || defined(ATCI_DEBUG)
          printf("ATCI Sleep: Need to update CID: %d, %d", gprsGenericContext_p->cidUserData[currentCidIndex]->isActive, currentCidIndex);
#endif
          mNvramGenericDataCache.cidInfo[currentCidIndex].isActive = gprsGenericContext_p->cidUserData[currentCidIndex]->isActive;
          needUpdate = TRUE;
       }       

       if (gprsGenericContext_p->cidUserData[currentCidIndex]->isActive)
       {
          /* Always update if first sleep */        
          if ((sleepManContext_p->firstSleep) ||
              (mNvramGenericDataCache.cidInfo[currentCidIndex].connId != gprsGenericContext_p->cidUserData[currentCidIndex]->psdBearerInfo.connId))
          {
             mNvramGenericDataCache.cidInfo[currentCidIndex].connId = gprsGenericContext_p->cidUserData[currentCidIndex]->psdBearerInfo.connId;
             needUpdate = TRUE;
          }
       }
       else if (mNvramGenericDataCache.cidInfo[currentCidIndex].connId != INVALID_CONN_ID)
       {
          mNvramGenericDataCache.cidInfo[currentCidIndex].connId = INVALID_CONN_ID;
          needUpdate = TRUE;
       }
     }     
  }

  /* only update once after loop...  NOTE: This may change if we have to retain
   * PSD data
   */
  if (needUpdate) 
  {
     nvdmStatus = nvdm_modem_write_normal_data_item
                   (RV_NVRAM_PSD_CONN_DATA_GROUP,
                    RV_NVRAM_PSD_CONN_DATA,
                    NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
                    (uint8_t *) mNvramGenericDataCache.cidInfo,
                    (sizeof(CidNvInfo_t)*MAX_NUMBER_OF_CIDS),
                    NVDM_MODEM_ATTR_AVERAGE);                       
  } 
  if (nvdmStatus != NVDM_MODEM_STATUS_OK)
  {
     updateSuccess = FALSE;
  }
  
  #endif
  return(updateSuccess);
}

/*************************************************************************
 * Global Functions
 *************************************************************************/
 /*--------------------------------------------------------------------------
 *
 * Function:    RvDeepSleepCb
 *
 * Parameters:  PscSleepAction sleepAction
 *                    PscPowerState powerState
 *
 * Returns:     Nothing
 *
 * Description:  Callback function for deep-sleep
 *-------------------------------------------------------------------------*/
  void RvDeepSleepCb (PscSleepAction sleepAction, PscPowerState powerState)
  {
    Boolean result;
    
    switch(sleepAction)
    {
       case PSC_SLEEP_ACTION_SLEEP:
          result = RvPrepareForSleep();
          if (result == TRUE)
          {
             RvGoToSleep();
             psc_release_active_lock(PSC_CLIENT_ATCI);
          }
          else
          {
            /* If NVRAM Write failed then we cannot go do sleep - assert */
            FatalFail("ATCI: NVRAM Write failed for deep sleep");
          }
          break;

       case PSC_SLEEP_ACTION_WAKEUP:
          /* This is a warm wakeup.  ATCI does nothing in this case */
          /* TODO: May need to re-start timers - but not likely */

          /* Reserve active lock again! */
          psc_reserve_active_lock(PSC_CLIENT_ATCI);
          break;
           
       default:
          break;
    }
  }

/*--------------------------------------------------------------------------
 *
 * Function:    RvPrepareForSleep
 *
 * Parameters:  Nothing
 *
 * Returns:     Nothing
 *
 * Description:  Prepare for sleep by storing updated data to NVRAM 
 *-------------------------------------------------------------------------*/
 Boolean RvPrepareForSleep(void)
 {
    Entity_t       *context_p     = ptrToAtciContextData();
    MobilityContext_t *mobility_p = ptrToMobilityContext();
    SimLockGenericContext_t  *simLock_p = ptrToSimLockGenericContext();
    SleepManContext_t    *sleepManContext_p  = ptrToSleepManContext();

    Boolean        allWriteOk = TRUE;
    RvNvramResult status = RV_NVRAM_RESULT_OK;

    Boolean         stkNeedsUpdate = FALSE;
    Boolean         pdContextNeedsUpdate = FALSE;
    Boolean         firstSleep = sleepManContext_p->firstSleep;
    Int8            previousNumberOfEnabledChannels;

#if defined (ENABLE_ATCI_UNIT_TEST) || defined(ATCI_DEBUG)
    printf("ATCI: Preparing to sleep now");
#endif

    if (firstSleep || (memcmp(&mNvramGenericDataCache.chmanData, &context_p->chManagerContext, sizeof(ChManagerContext_t) ) != 0))
    {
       memcpy(&mNvramGenericDataCache.chmanData, &context_p->chManagerContext, sizeof(ChManagerContext_t));
       status  =  RvWriteGenericDataToNvram(RV_CHMAN_DATA);
       if (status != RV_NVRAM_RESULT_OK)
       {
          allWriteOk = FALSE;
       }
    }
    if (firstSleep || (memcmp(&mNvramGenericDataCache.muxContextData, &context_p->muxContext, sizeof(MuxContext_t) ) != 0))
    {
       memcpy(&mNvramGenericDataCache.muxContextData, &context_p->muxContext, sizeof(MuxContext_t));
       status  =  RvWriteGenericDataToNvram(RV_MUX_CONTEXT_DATA);
       if (status != RV_NVRAM_RESULT_OK)
       {
          allWriteOk = FALSE;
       }
    }

    /* Need to remember the previous number of enabled channels to allw us to update entity/channel specific data correctly */
    previousNumberOfEnabledChannels = mNvramGenericDataCache.opmanGenericData.numberOfEnabledChannels;
    
    if (firstSleep || (memcmp(&mNvramGenericDataCache.opmanGenericData, &context_p->opmanGenericContext, sizeof(OpmanGenericContext_t) ) != 0))
    {
       memcpy(&mNvramGenericDataCache.opmanGenericData, &context_p->opmanGenericContext, sizeof(OpmanGenericContext_t));
       status  =  RvWriteGenericDataToNvram(RV_OPMAN_DATA);
       if (status != RV_NVRAM_RESULT_OK)
       {
          allWriteOk = FALSE;
       }
    }
    if (firstSleep || (memcmp(&mNvramGenericDataCache.profileGenericData, &context_p->profileGenericContext, sizeof(ProfileGenericContext_t) ) != 0))
    {
       memcpy(&mNvramGenericDataCache.profileGenericData, &context_p->profileGenericContext, sizeof(ProfileGenericContext_t));
       status  =  RvWriteGenericDataToNvram(RV_PROFILE_DATA);
       if (status != RV_NVRAM_RESULT_OK)
       {
          allWriteOk = FALSE;
       }
    }
    if (firstSleep || (memcmp(&mNvramGenericDataCache.mobilityData.vgCOPSData, &mobility_p->vgCOPSData, sizeof(VgCOPSData) ) != 0) ||
        (memcmp(&mNvramGenericDataCache.mobilityData.vgCSCONData, &mobility_p->vgCSCONData, sizeof(VgCSCONData) ) != 0) ||
        (memcmp(&mNvramGenericDataCache.mobilityData.vgMOOSAINDData, &mobility_p->vgMoosaIndData, sizeof(VgMOOSAINDData) ) != 0))
    {
       memcpy(&mNvramGenericDataCache.mobilityData.vgCOPSData, &mobility_p->vgCOPSData, sizeof(VgCOPSData));
       memcpy(&mNvramGenericDataCache.mobilityData.vgCSCONData, &mobility_p->vgCSCONData, sizeof(VgCSCONData));       
       memcpy(&mNvramGenericDataCache.mobilityData.vgMOOSAINDData, &mobility_p->vgMoosaIndData, sizeof(VgMOOSAINDData));
       status  =  RvWriteGenericDataToNvram(RV_MOBILITY_DATA);
       if (status != RV_NVRAM_RESULT_OK)
       {
          allWriteOk = FALSE;
       }       
    } 
    if (firstSleep || ((mNvramGenericDataCache.simLockData.powerUpProtoStack != simLock_p->powerUpProtoStack)|| 
       (mNvramGenericDataCache.simLockData.powerUpSim != simLock_p->powerUpSim) ||
       (mNvramGenericDataCache.simLockData.currentSimInsertedState != simLock_p->simInsertedState)))
    {
        mNvramGenericDataCache.simLockData.powerUpProtoStack = simLock_p->powerUpProtoStack;
        mNvramGenericDataCache.simLockData.powerUpSim = simLock_p->powerUpSim;
        mNvramGenericDataCache.simLockData.currentSimInsertedState = simLock_p->simInsertedState;
        status  =  RvWriteGenericDataToNvram(RV_SIMLOCK_DATA);       
        if (status != RV_NVRAM_RESULT_OK)
        {
          allWriteOk = FALSE;
        }      
    }
    if (firstSleep || (memcmp(mNvramGenericDataCache.stkData.registeredForStk, context_p->stkEntityGenericData.registeredForStk, CI_MAX_ENTITIES) != 0))
    {
        memcpy(mNvramGenericDataCache.stkData.registeredForStk, context_p->stkEntityGenericData.registeredForStk, CI_MAX_ENTITIES);
        stkNeedsUpdate = TRUE;
    }
    if (firstSleep ||(mNvramGenericDataCache.stkData.registeredStkEntity != context_p->stkEntityGenericData.registeredStkEntity))
    {
      
       mNvramGenericDataCache.stkData.registeredStkEntity = context_p->stkEntityGenericData.registeredStkEntity;
       stkNeedsUpdate = TRUE;
    }

    if (stkNeedsUpdate)
    {
       status  =  RvWriteGenericDataToNvram(RV_STK_DATA);       
       if (status != RV_NVRAM_RESULT_OK)
       {
          allWriteOk = FALSE;
       } 
    }
    if (firstSleep ||(memcmp(&mNvramGenericDataCache.generalGenericData.vgMFtrCfgData, &context_p->generalGenericContext.vgMFtrCfgData, sizeof(VgMFtrCfgData) ) != 0))
    {
       memcpy(&mNvramGenericDataCache.generalGenericData.vgMFtrCfgData, &context_p->generalGenericContext.vgMFtrCfgData, sizeof(VgMFtrCfgData));
       status  =  RvWriteGenericDataToNvram(RV_GENERAL_DATA);       
       if (status != RV_NVRAM_RESULT_OK)
       {
          allWriteOk = FALSE;
       }  
    }
    if (firstSleep || (memcmp(&mNvramGenericDataCache.pdContextData.currDefaultApn, &context_p->gprsGenericContext.currentDefaultAPN, sizeof(AbpdApn)) !=0))
    {
       memcpy(&mNvramGenericDataCache.pdContextData.currDefaultApn, &context_p->gprsGenericContext.currentDefaultAPN, sizeof(AbpdApn));
       pdContextNeedsUpdate = TRUE;
    }

    if (pdContextNeedsUpdate)
    {
       status  =  RvWriteGenericDataToNvram(RV_PD_CONTEXT_DATA);       
       if (status != RV_NVRAM_RESULT_OK)
       {
          allWriteOk = FALSE;
       } 
    } 
 
    if (!RvUpdateEntityData(previousNumberOfEnabledChannels)== TRUE)
    {
        allWriteOk = FALSE;
    }

    if (!RvUpdateCidData() == TRUE)
    {
        allWriteOk = FALSE;
    }
    return(allWriteOk);
 }

/*--------------------------------------------------------------------------
 *
 * Function:    RvGoToSleep
 *
 * Parameters:  Nothing
 *
 * Returns:     Nothing
 *
 * Description:  Copy data to be stored  to the RTC data structures and free CID data
 *  
 *-------------------------------------------------------------------------*/
 void RvGoToSleep(void)
 {
    MobilityContext_t    *mobility_p      = ptrToMobilityContext();
    GprsGenericContext_t *gprsGeneric_p   = ptrToGprsGenericContext();
    Entity_t             *entityGeneric_p = ptrToAtciContextData();
    SleepManContext_t    *sleepManContext_p  = ptrToSleepManContext();
    ApBridgeGenericContext_t* apBridgeGenericContext_p = ptrToApBridgeGenericContext();
#if defined(ATCI_ENABLE_DYN_AT_BUFF)   
    Int8                  entityNum;
    ScanParseContext_t   *scanParseContext_p; 
#endif   
#if defined (ENABLE_ATCI_UNIT_TEST) || defined(ATCI_DEBUG)
    printf("ATCI: Going to sleep now");
#endif
    /* Store RTC RAM Data */
    memcpy(&mobilityContextRtcRamData, mobility_p, sizeof(MobilityContextRtcRamData));
    memcpy(&gprsGenericContextRtcRamData, gprsGeneric_p, sizeof(GprsGenericContextRtcRamData));
    memcpy(&entityGenericContextRtcRamData, entityGeneric_p, sizeof(EntityGenericContextRtcRamData));
    memcpy(&entityApbContextRtcRamData, apBridgeGenericContext_p->apbChannels, sizeof(EntityApbContextRtcRamData));

#if defined(ATCI_ENABLE_DYN_AT_BUFF) && defined (ENABLE_ATCI_UNIT_TEST)
    /* free any hanging parsing pointers*/  
    for (entityNum = 0; entityNum < CI_MAX_ENTITIES; entityNum++)
    {
      scanParseContext_p = ptrToScanParseContext (entityNum);
      if ((scanParseContext_p != PNULL) && (scanParseContext_p->nextCommand.character != PNULL))
      {
        scanParseContext_p->nextCommand.character = PNULL;
      }          
    }
#endif 

    /* Clear firstSleep to prevent multiple NVRAM writes if we don't actually
     * go to sleep
     */
    sleepManContext_p->firstSleep = FALSE;
  }

/*--------------------------------------------------------------------------
 *
 * Function:   RvWakeUpFromSleep
 *
 * Parameters:  Nothing
 *
 * Returns:     Nothing
 *
 * Description:  Copy back  from RTC data structures and NVRAM and build active CID entries
 *               NOTE: This must happen AFTER SIG_INITIALISE has been received!
 *-------------------------------------------------------------------------*/
 void RvWakeUpFromSleep(void)
{
   Entity_t                 *context_p   = ptrToAtciContextData();
   MobilityContext_t        *mobility_p  = ptrToMobilityContext();
   SimLockGenericContext_t  *simLock_p   = ptrToSimLockGenericContext();
   GprsGenericContext_t     *gprsGeneric_p = ptrToGprsGenericContext();
   MuxContext_t              *muxContext_p = ptrToMuxContext ();
   GeneralGenericContext_t   *generalGenericContext_p = ptrToGeneralGenericContext ();
   SleepManContext_t        *sleepManContext_p  = ptrToSleepManContext();
   ApBridgeGenericContext_t* apBridgeGenericContext_p = ptrToApBridgeGenericContext();
   Int8                      cidNum;
   Int8                      psdConnection;
   Int8                      cid = 0;
   VgSimInsertedState        previousSimInsertedState;
   /* restore RTC RAM Data */

   memcpy(mobility_p, &mobilityContextRtcRamData, sizeof(MobilityContextRtcRamData));
   memcpy(gprsGeneric_p, &gprsGenericContextRtcRamData, sizeof(GprsGenericContextRtcRamData)); 
   memcpy(context_p, &entityGenericContextRtcRamData, sizeof(EntityGenericContextRtcRamData));
   memcpy(apBridgeGenericContext_p->apbChannels, &entityApbContextRtcRamData, sizeof(EntityApbContextRtcRamData));

   /* re-initialise context data */
   RvInitialiseNVRamMemStore();

   /* complete set-up for status signal requirements */
   sleepManContext_p->numPsdBearerStatusIndsNeeded = 0;

   while (cid < MAX_NUMBER_OF_CIDS)
   {
     if (gprsGeneric_p->cidOwner[cid] != VGMUX_CHANNEL_INVALID)
     {        
        sleepManContext_p->numPsdBearerStatusIndsNeeded++;
     }
     cid++;
   }

   FatalCheck(sleepManContext_p->numPsdBearerStatusIndsNeeded <= ATCI_MAX_NUM_PSD_CONNECTIONS,
              sleepManContext_p->numPsdBearerStatusIndsNeeded, gprsGeneric_p->cidOwner[0],
              gprsGeneric_p->cidOwner[1]);
   
   if (RvReadFromNvramToCache(RV_MUX_CONTEXT_DATA) == RV_NVRAM_RESULT_OK)
   {
      memcpy(&context_p->muxContext, &mNvramGenericDataCache.muxContextData, sizeof(MuxContext_t));
   }

   if (RvReadFromNvramToCache(RV_OPMAN_DATA) == RV_NVRAM_RESULT_OK)
   {
      memcpy(&context_p->opmanGenericContext, &mNvramGenericDataCache.opmanGenericData, sizeof(OpmanGenericContext_t));
   }

   if (RvReadFromNvramToCache(RV_PROFILE_DATA) == RV_NVRAM_RESULT_OK)
   {
      memcpy(&context_p->profileGenericContext, &mNvramGenericDataCache.profileGenericData, sizeof(ProfileGenericContext_t));
   }

   if (RvReadFromNvramToCache(RV_CHMAN_DATA) == RV_NVRAM_RESULT_OK)
   {
      memcpy(&context_p->chManagerContext, &mNvramGenericDataCache.chmanData, sizeof(ChManagerContext_t));
   }
   
   if (RvReadFromNvramToCache(RV_MOBILITY_DATA) == RV_NVRAM_RESULT_OK)
   {
      memcpy(&mobility_p->vgCOPSData, &mNvramGenericDataCache.mobilityData.vgCOPSData, sizeof(VgCOPSData));
      memcpy(&mobility_p->vgCSCONData, &mNvramGenericDataCache.mobilityData.vgCSCONData, sizeof(VgCSCONData));      
      memcpy(&mobility_p->vgMoosaIndData, &mNvramGenericDataCache.mobilityData.vgMOOSAINDData, sizeof(VgMOOSAINDData));
   }

   if (RvReadFromNvramToCache(RV_SIMLOCK_DATA) == RV_NVRAM_RESULT_OK)
   {
      simLock_p->powerUpProtoStack = mNvramGenericDataCache.simLockData.powerUpProtoStack;
      simLock_p->powerUpSim = mNvramGenericDataCache.simLockData.powerUpSim;

      /* We do not store the SIM inserted state back in the entity RAM structure because
       * we are just using it here to check which signals we will need to wait for from
       * the AB/protocol stack
       */
      previousSimInsertedState = mNvramGenericDataCache.simLockData.currentSimInsertedState;
      
      /* We now need to set the signals we are waiting for correctly based on the CFUN settings */
      /* Protocol stack off means we cannot receive network state indications or SM ready inds */
      if (!simLock_p->powerUpProtoStack)
      {
        sleepManContext_p->needNetworkStateInd = FALSE;
        sleepManContext_p->needSmReadyInd = FALSE;         
      }
      /* SIM off or previously not present means we cannot receive SIM OK Ind or SM ready inds */
      if ((!simLock_p->powerUpSim) || (previousSimInsertedState != VG_SIM_INSERTED))
      {
        if (!simLock_p->powerUpSim)
        {
          sleepManContext_p->needSimNok = FALSE;    
        }
        sleepManContext_p->needSimOk = FALSE;    
        sleepManContext_p->needSmReadyInd = FALSE;         
#if defined (FEA_PHONEBOOK)     
        sleepManContext_p->needLmReadyInd = FALSE;         
#endif
      }
      else
      {
        /* If SIM was OK - then don't need SIM_NOK_IND */
        sleepManContext_p->needSimNok = FALSE;    
      }
   } 

   if (RvReadFromNvramToCache(RV_STK_DATA) == RV_NVRAM_RESULT_OK)
   {
      memcpy(context_p->stkEntityGenericData.registeredForStk, mNvramGenericDataCache.stkData.registeredForStk, CI_MAX_ENTITIES);
      context_p->stkEntityGenericData.registeredStkEntity = mNvramGenericDataCache.stkData.registeredStkEntity;
   } 

   if (RvReadFromNvramToCache(RV_GENERAL_DATA) == RV_NVRAM_RESULT_OK)
   {
      memcpy(&context_p->generalGenericContext.vgMFtrCfgData, &mNvramGenericDataCache.generalGenericData.vgMFtrCfgData, sizeof(VgMFtrCfgData));
   }

   if (RvReadFromNvramToCache(RV_PD_CONTEXT_DATA) == RV_NVRAM_RESULT_OK)
   {
      memcpy(&context_p->gprsGenericContext.currentDefaultAPN, &mNvramGenericDataCache.pdContextData.currDefaultApn, sizeof(AbpdApn));
   }
 
   if (RvReadFromNvramToCache(RV_ENTITY_SPECIFIC_DATA) == RV_NVRAM_RESULT_OK)
   {
      RvReinstateEntityData();
   }
   else
   {
      FatalFail("ATCI entity data NVRAM read failed");
   }

   if (RvReadFromNvramToCache(RV_CID_DATA) == RV_NVRAM_RESULT_OK)
   {
      cidNum = 0;
      psdConnection = 0;
      
      while ((cidNum< MAX_NUMBER_OF_CIDS) && (psdConnection <ATCI_MAX_NUM_PSD_CONNECTIONS))
      {
         if (mNvramGenericDataCache.cidInfo[cidNum].isActive == TRUE)
         {
            /* only allocate RAM for CIDs which are active */
            if (vgAllocateRamToCid(cidNum))
            {
               /* isActive is set in reception of ABPD_PSD_BEARER_STATUS_IND signal */
               
               context_p->gprsGenericContext.cidUserData[cidNum]->psdBearerInfo.connId = 
                     mNvramGenericDataCache.cidInfo[cidNum].connId;
               psdConnection++;
            }
         }
         cidNum++;
      }
      
    }     

    /*******************************************************************
     * Reset data structure settings here where necessary to prevent ATCI
     * doing certain activities on wake-up
     *******************************************************************/

    /* Prevent us registering with AB again */     
    muxContext_p->atciRegisteredWithABPRocedures = TRUE;

    /* Prevent NVRAM read of phone functionality */
    muxContext_p->atciHaveReadPhoneFunctionality = TRUE;

    /* Don't need to wait for ABPD data for default bearer
     * activated during attach
     */
    generalGenericContext_p->initDataFromABPDState = READY;
  
    /* TODO: Add more here where necessary */
  }

/*--------------------------------------------------------------------------
 *
 * Function:   RvInitialiseNVRamMemStore
 *
 * Parameters:  Nothing
 *
 * Returns:     Nothing
 *
 * Description:  Initialise the NVRAM store prior to copy from NVRAM so any failed read
                      would have initialised data..
 *-------------------------------------------------------------------------*/
void RvInitialiseNVRamMemStore(void)
{
  memset (&mNvramGenericDataCache, 0 ,sizeof(RvNvramGenericData));
}

/*--------------------------------------------------------------------------
 *
 * Function:    RvWakeUpCompleteCheck
 *
 * Parameters:  Nothing
 *
 * Returns:     Nothing
 *
 * Description:  Checks if the ATCI is fully woken up.  If it has then 
 *               completed by informing PSC
 *  
 *-------------------------------------------------------------------------*/

void RvWakeUpCompleteCheck(void)
{
  SleepManContext_t    *sleepManContext_p  = ptrToSleepManContext();

  if (sleepManContext_p->atciInWakeupState)
  {
#if defined (ENABLE_ATCI_UNIT_TEST) || defined(ATCI_DEBUG)
    printf ("ATCI checking wakeup complete %d, %d, %d, %d, %d, %d, %d",
            sleepManContext_p->needNetworkStateInd,
            sleepManContext_p->needSimNok,
            sleepManContext_p->needSimOk,
            sleepManContext_p->needSmReadyInd,
            sleepManContext_p->needGlReadyInd,
#if defined (FEA_PHONEBOOK)     
            sleepManContext_p->needLmReadyInd,
#else
            0,
#endif
            
            sleepManContext_p->numPsdBearerStatusIndsNeeded);
#endif

    /* We are still waking up.  Check if all other flags cleared */
    if ((!sleepManContext_p->needNetworkStateInd) &&
        (!sleepManContext_p->needSimNok) &&
        (!sleepManContext_p->needSimOk) &&
        (!sleepManContext_p->needSmReadyInd) &&
        (!sleepManContext_p->needGlReadyInd) &&
#if defined (FEA_PHONEBOOK)     
        (!sleepManContext_p->needLmReadyInd) &&
#endif        
        (sleepManContext_p->numPsdBearerStatusIndsNeeded == 0))
    {
      /* Wakeup complete */
      sleepManContext_p->atciInWakeupState = FALSE;

#if defined (ENABLE_ATCI_UNIT_TEST) || defined(ATCI_DEBUG)
      printf ("ATCI Wakeup now complete");
#endif

      /* now we can allow deep sleep */
      psc_set_constraint(PSC_CLIENT_ATCI, PSC_CONSTRAINT_ALL, FALSE);
      atReadyInd();

      /* For now generate URC here - however, this will have to move after integration with AP */
      RvGenerateWakeupCompleteUrc();

    }
  }
}
Boolean RvWakeUpComplete()
{
    SleepManContext_t    *sleepManContext_p  = ptrToSleepManContext();
    return !sleepManContext_p->atciInWakeupState;
}

/*--------------------------------------------------------------------------
 *
 * Function:    RvGenerateWakeupCompleteUrc
 *
 * Parameters:  Nothing
 *
 * Returns:     Nothing
 *
 * Description:  Generates URC on pre-defined channel for AP to know
 *               when MD has woken up from deep sleep.
 *
 *               This needs to be called at the right time after wakeup
 *
 *-------------------------------------------------------------------------*/
void RvGenerateWakeupCompleteUrc(void)
{
  VgmuxChannelNumber          profileEntity         = 0;

  /* Generate URC */
  for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
  {
    if (isEntityActive(profileEntity) && 
        (getProfileValue (profileEntity, PROF_MATWAKEUP) == PROF_MATWAKEUP_ENABLE))
    {
      vgPutNewLine (profileEntity);
      vgPrintf (profileEntity, (const Char*)"*MATWAKEUP");

      vgPutNewLine (profileEntity);
      vgFlushBuffer(profileEntity);
    }
  }
}

/****************************************************************************
 *
 * Function:    vgSleepManInterfaceController
 *
 * Parameters:  SignalBuffer       - structure containing incoming signal
 *              VgmuxChannelNumber - mux channel number
 *
 * Returns:     Boolean - indicates whether the sub-system has recognised and
 *                        procssed the signal given.
 *
 * Description: determines action for received signals
 *
 ****************************************************************************/

Boolean VgSleepManInterfaceController(  const SignalBuffer *signal_p,
                                             const VgmuxChannelNumber entity)
{
  Boolean accepted = FALSE;
#if defined (ENABLE_ATCI_UNIT_TEST)
  /* Hack because for some reason we get called twice for SIG_CI_UNIT_TEST_SLEEP_MODE_IND
   */
  static Boolean gotSleepModeIndAlready = FALSE;
#endif

  /* Signal Handler */

  switch (*signal_p->type)
  {

    case SIG_INITIALISE:
    {
#if defined (ENABLE_ATCI_UNIT_TEST)
      gotSleepModeIndAlready = FALSE;
#endif

#if !defined (ENABLE_ATCI_UNIT_TEST)
      /* For unit testing use SIG_INITIALISE cause parameter - for target
       * use the psc_get_wakeup_reason
       */
      if (psc_get_wakeup_reason() == PSC_DEEP_SLEEP)
      {
        InitialiseSleepManContextData(RESET_WAKEUP_FROM_DEEP_SLEEP);
      }
      else
#endif        
      {
        InitialiseSleepManContextData(signal_p->sig->initialise.cause);
      }
      break;
    }
    
#if defined (ENABLE_ATCI_UNIT_TEST)
    case SIG_CI_UNIT_TEST_SLEEP_MODE_IND:
    {
      if (!gotSleepModeIndAlready)
      {
        printf ("ATCI: Got SIG_CI_UNIT_TEST_SLEEP_MODE_IND");
        vgSigCiUnitTestSleepModeInd (signal_p);
        gotSleepModeIndAlready = TRUE;
        accepted = TRUE;
      }        
      break;
    }
#endif
    default:
    {
      break;
    }
  }
  return (accepted);
} 

 /* END OF FILE */


