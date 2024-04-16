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
 * CI-NVRAM Interface module.
 **************************************************************************/

#define MODULE_NAME "RVNVRAM"

#include <system.h>
#include <kernel.h>
#include <sitl_typ.h>
#include <vgmx_sig.h>
#include <rvnvram.h>
#include <rvchman.h>
#include <rvutil.h>
#include <afnv_typ.h>
#include <rvdata.h>
#include <rvomut.h>
#include <rvoman.h>
#include <rvpfut.h>
#include <rvpfsigo.h>
#include <rvcimxut.h>
#include <rvcimxsot.h>
#include <rvcrhand.h>
#include <rvomtime.h>
#include <rvmmsigo.h>
#include <rvslut.h>
#include <rvcmux.h>
#include <rvutil.h>

/***************************************************************************
 * Macros
 ***************************************************************************/
#define VGCI_NPHY_SIG_GROUP_NAME    "NPHY_SIG"
#define VGCI_NPHY_SIG_ITEM1_NAME    "NPBCH_SYMBOL_ROTATION_MODE"
#define VGCI_NPHY_SIG_ITEM1_LENGTH  1

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/
 
/***************************************************************************
 * Signal definitions
 ***************************************************************************/
union Signal
{

  CiMuxChannelEnableInd   ciMuxChannelEnableInd;
  Anrm2ReadDataCnf        anrm2ReadDataCnf;
  Anrm2ReadDataReq        anrm2ReadDataReq;
  Anrm2WriteDataReq       anrm2WriteDataReq;
};

/***************************************************************************
 * Types
 ***************************************************************************/

/***************************************************************************
 * Variables
 ***************************************************************************/

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static void vgCiReadNvramCnf (Anrm2ReadDataCnf *sig,
                              const VgmuxChannelNumber  entity);
static void vgCiWriteNvramCnf (Anrm2WriteDataCnf *sig,
                               const VgmuxChannelNumber  entity);
static void vgCiSetNvramState (VgNvramState state,
                                const VgmuxChannelNumber entity);
static void vgCiReadNvram (const Anrm2DataName name,
                            const VgmuxChannelNumber  entity);
static void vgCiWriteNvram (const Anrm2DataName name,
                             const VgmuxChannelNumber  entity);
static void vgNvramOperationCompleted (const SignalBuffer *signal_p,
                                        const VgmuxChannelNumber entity);

static ProfileContext_t* vgCiGetProfileForWriteReq (Anrm2WriteDataReq *signal);

static ProfileContext_t* vgCiGetProfileForReadCnf (Anrm2ReadDataCnf *signal);

static void vgNvramInitialise (void);
extern const AtIdentificationInfo atIdentificationInfoDefaultInit;

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
*
* Function:    vgCiGetAnrm2DataName
*
* Parameters:  entity - mux channel number
*
*
* Returns:     profileName
*
* Description: Get the corresponding entity profile name according to entity
*
*
*-------------------------------------------------------------------------*/
Anrm2DataName vgCiGetAnrm2DataName (const VgmuxChannelNumber entity)
{
  Anrm2DataName profileName;
  switch (entity)
  {
    case VGMUX_CHANNEL_COMMAND_2:
      profileName = NRAM2_PROFILE_DATA_ENTITY1;
      break;

    case VGMUX_CHANNEL_COMMAND_3:
      profileName = NRAM2_PROFILE_DATA_ENTITY2;
      break;

    case VGMUX_CHANNEL_COMMAND_4:
      profileName = NRAM2_PROFILE_DATA_ENTITY3;
      break;

    case VGMUX_CHANNEL_COMMAND_5:
      profileName = NRAM2_PROFILE_DATA_ENTITY4;
      break;

    case VGMUX_CHANNEL_COMMAND_6:
      profileName = NRAM2_PROFILE_DATA_ENTITY5;
      break;

    case VGMUX_CHANNEL_COMMAND_7:
      profileName = NRAM2_PROFILE_DATA_ENTITY6;
      break;

    case VGMUX_CHANNEL_COMMAND_8:
      profileName = NRAM2_PROFILE_DATA_ENTITY7;
      break;

    case VGMUX_CHANNEL_COMMAND_9:
      profileName = NRAM2_PROFILE_DATA_ENTITY8;
      break;

    case VGMUX_CHANNEL_COMMAND_10:
      profileName = NRAM2_PROFILE_DATA_ENTITY9;
      break;

    case VGMUX_CHANNEL_COMMAND_11:
      profileName = NRAM2_PROFILE_DATA_ENTITY10;
      break;

    case VGMUX_CHANNEL_COMMAND_12:
      profileName = NRAM2_PROFILE_DATA_ENTITY11;
      break;

    case VGMUX_CHANNEL_COMMAND_13:
      profileName = NRAM2_PROFILE_DATA_ENTITY12;
      break;

    case VGMUX_CHANNEL_COMMAND_14:
      profileName = NRAM2_PROFILE_DATA_ENTITY13;
      break;

    /*For entity is bigger than VGMUX_CHANNEL_COMMAND_14, cache PROFILE0
    ** data is applied.
    */
    case VGMUX_CHANNEL_COMMAND_1:
    default:
      profileName = NRAM2_PROFILE_DATA_ENTITY0;
      break;
  }
  return (profileName);
}

/*--------------------------------------------------------------------------
*
* Function:    vgCiSetProfileTag
*
* Parameters:
*
*
* Returns:     Nothing
*
* Description:
*
*
*-------------------------------------------------------------------------*/
void vgCiSetProfileTag(VgmuxChannelNumber entity, SignalId signalType ,Boolean setSpecificProfile)
{
  NvramContext_t             *nvramContext_p          = ptrToNvramContext (entity);
  if (setSpecificProfile == FALSE)
  {
    switch(signalType)
    {
      case SIG_ANRM2_READ_DATA_REQ:
        nvramContext_p->readProfileFlag  = (Int8)((nvramContext_p->readProfileFlag)|(GENERIC_PROFILE_DATA_BIT));
        break;

      case SIG_ANRM2_READ_DATA_CNF:
        nvramContext_p->readProfileFlag  = (Int8)((nvramContext_p->readProfileFlag)&(~(GENERIC_PROFILE_DATA_BIT)));
        break;

      case SIG_ANRM2_WRITE_DATA_REQ:
        nvramContext_p->writeProfileFlag = (Int8)((nvramContext_p->writeProfileFlag)|(GENERIC_PROFILE_DATA_BIT));
        break;

      case SIG_ANRM2_WRITE_DATA_CNF:
        nvramContext_p->writeProfileFlag = (Int8)((nvramContext_p->writeProfileFlag)&(~(GENERIC_PROFILE_DATA_BIT)));
        break;

      default:
        break;
    }
  }
  else
  {
    switch(signalType)
    {
      case SIG_ANRM2_READ_DATA_REQ:
        nvramContext_p->readProfileFlag  = (Int8)((nvramContext_p->readProfileFlag)|(SPECIFIC_PROFILE_DATA_BIT));
        break;

      case SIG_ANRM2_READ_DATA_CNF:
        nvramContext_p->readProfileFlag  = (Int8)((nvramContext_p->readProfileFlag)&(~(SPECIFIC_PROFILE_DATA_BIT)));
        break;

      case SIG_ANRM2_WRITE_DATA_REQ:
        nvramContext_p->writeProfileFlag = (Int8)((nvramContext_p->writeProfileFlag)|(SPECIFIC_PROFILE_DATA_BIT));
        break;

      case SIG_ANRM2_WRITE_DATA_CNF:
        nvramContext_p->writeProfileFlag = (Int8)((nvramContext_p->writeProfileFlag)&(~(SPECIFIC_PROFILE_DATA_BIT)));
        break;

      default:
        break;
    }
  }
}

/*--------------------------------------------------------------------------
*
* Function:    vgCiGetProfileForWriteReq
*
* Parameters:  name - Anrm2DataName
*
*
* Returns:     ProfileContext_t*
*
* Description: Get the corresponding profile from signal according to Anrm2DataName
*
*
*-------------------------------------------------------------------------*/
ProfileContext_t* vgCiGetProfileForWriteReq (Anrm2WriteDataReq *signal)
{
  ProfileContext_t *profilecontext_p;
  switch (signal->name)
  {
    case NRAM2_PROFILE_DATA_ENTITY0:
      profilecontext_p = &signal->data.profileDataEntity0.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY1:
      profilecontext_p = &signal->data.profileDataEntity1.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY2:
      profilecontext_p = &signal->data.profileDataEntity2.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY3:
      profilecontext_p = &signal->data.profileDataEntity3.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY4:
      profilecontext_p = &signal->data.profileDataEntity4.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY5:
      profilecontext_p = &signal->data.profileDataEntity5.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY6:
      profilecontext_p = &signal->data.profileDataEntity6.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY7:
      profilecontext_p = &signal->data.profileDataEntity7.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY8:
      profilecontext_p = &signal->data.profileDataEntity8.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY9:
      profilecontext_p = &signal->data.profileDataEntity9.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY10:
      profilecontext_p = &signal->data.profileDataEntity10.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY11:
      profilecontext_p = &signal->data.profileDataEntity11.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY12:
      profilecontext_p = &signal->data.profileDataEntity12.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY13:
      profilecontext_p = &signal->data.profileDataEntity13.profile;
      break;

    default:
      profilecontext_p = PNULL;
      break;
  }
  return (profilecontext_p);
}

/*--------------------------------------------------------------------------
*
* Function:    vgCiGetProfileForReadCnf
*
* Parameters:  name - Anrm2DataName
*
*
* Returns:     ProfileContext_t*
*
* Description: Get the corresponding profile from signal according to Anrm2DataName
*
*
*-------------------------------------------------------------------------*/
ProfileContext_t* vgCiGetProfileForReadCnf (Anrm2ReadDataCnf *signal)
{
  ProfileContext_t *profilecontext_p;
  switch (signal->name)
  {
    case NRAM2_PROFILE_DATA_ENTITY0:
      profilecontext_p = &signal->data.profileDataEntity0.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY1:
      profilecontext_p = &signal->data.profileDataEntity1.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY2:
      profilecontext_p = &signal->data.profileDataEntity2.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY3:
      profilecontext_p = &signal->data.profileDataEntity3.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY4:
      profilecontext_p = &signal->data.profileDataEntity4.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY5:
      profilecontext_p = &signal->data.profileDataEntity5.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY6:
      profilecontext_p = &signal->data.profileDataEntity6.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY7:
      profilecontext_p = &signal->data.profileDataEntity7.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY8:
      profilecontext_p = &signal->data.profileDataEntity8.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY9:
      profilecontext_p = &signal->data.profileDataEntity9.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY10:
      profilecontext_p = &signal->data.profileDataEntity10.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY11:
      profilecontext_p = &signal->data.profileDataEntity11.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY12:
      profilecontext_p = &signal->data.profileDataEntity12.profile;
      break;

    case NRAM2_PROFILE_DATA_ENTITY13:
      profilecontext_p = &signal->data.profileDataEntity13.profile;
      break;

    default:
      profilecontext_p = PNULL;
      break;
  }
  return (profilecontext_p);
}

/*--------------------------------------------------------------------------
*
* Function:    vgCiSetNvramState
*
* Parameters:  state  - new nvram state
*              entity - mux channel number
*
* Returns:     nothing
*
* Description: The NVRAM state defines the action to be perfomed on NVRAM.  For
*              example, a PROFILE DATA request may be made an the state set
*              to JUST READ.
*
*-------------------------------------------------------------------------*/

static void vgCiSetNvramState (VgNvramState state,
                                const VgmuxChannelNumber entity)
{
  NvramContext_t  *nvramContext_p = ptrToNvramContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(nvramContext_p != PNULL, entity, 0, 0);
#endif
  nvramContext_p->nramState = state;
}


/*--------------------------------------------------------------------------
*
* Function:    vgCiReadNvram
*
* Parameters:  name   - nvram data to read
*              entity - mux channel number
*
* Returns:     nothing
*
* Description: Sends a read nvram req GKI signal to NRAM Task.  We expect the
*              the confirmation back which we decode and perform the necessary
*              actions.
*
*-------------------------------------------------------------------------*/

static void vgCiReadNvram (const Anrm2DataName name,
                            const VgmuxChannelNumber entity)
{
  SignalBuffer      sigBuff  = kiNullBuffer;
  Anrm2ReadDataReq  *request_p;


  KiCreateZeroSignal (SIG_ANRM2_READ_DATA_REQ,
                       sizeof (Anrm2ReadDataReq),
                        &sigBuff);

  request_p = (Anrm2ReadDataReq *) sigBuff.sig;

  request_p->sourceTaskId = VG_CI_TASK_ID;
  request_p->commandRef   = (Int16)entity;
  request_p->name         = name;

  KiSendSignal (TASK_ANRM2_ID, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgCiWriteNvram
*
* Parameters:  name   - nvram data to write
*              entity - mux channel number
*
* Returns:     nothing
*
* Description: Sends a write nvram req GKI signal to NRAM Task.  We expect
*              the write confirmation back which we decode and perform the
*              necessary actions.
*
*-------------------------------------------------------------------------*/

static void vgCiWriteNvram (const Anrm2DataName name,
                             const VgmuxChannelNumber entity)
{
  SignalBuffer              sigBuff                     = kiNullBuffer;
  NvramContext_t            *nvramContext_p            = ptrToNvramContext (entity);
  ProfileContext_t          *profileContext_p          = ptrToProfileContext (entity);
  ProfileGenericContext_t   *profileGenericContext_p  = ptrToProfileGenericContext ();
  Anrm2WriteDataReq         *request_p;

#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(nvramContext_p != PNULL, entity, 0, 0);
  FatalCheck(profileContext_p != PNULL, entity, 0, 0);
#endif
  sendSsRegistrationSignal (NVRAM,
                             entity,
                              SIG_ANRM2_WRITE_DATA_CNF);

  KiCreateZeroSignal (SIG_ANRM2_WRITE_DATA_REQ,
                       sizeof (Anrm2WriteDataReq),
                        &sigBuff);

  request_p = (Anrm2WriteDataReq *) sigBuff.sig;

  request_p->sourceTaskId = VG_CI_TASK_ID;
  request_p->commandRef   = (Int16)entity;
  request_p->name         = name;


  switch (name)
  {
    case NRAM2_PROFILE_DATA_ENTITY0:
    case NRAM2_PROFILE_DATA_ENTITY1:
    case NRAM2_PROFILE_DATA_ENTITY2:
    case NRAM2_PROFILE_DATA_ENTITY3:
    case NRAM2_PROFILE_DATA_ENTITY4:
    case NRAM2_PROFILE_DATA_ENTITY5:
    case NRAM2_PROFILE_DATA_ENTITY6:
    case NRAM2_PROFILE_DATA_ENTITY7:
    case NRAM2_PROFILE_DATA_ENTITY8:
    case NRAM2_PROFILE_DATA_ENTITY9:
    case NRAM2_PROFILE_DATA_ENTITY10:
    case NRAM2_PROFILE_DATA_ENTITY11:
    case NRAM2_PROFILE_DATA_ENTITY12:
    case NRAM2_PROFILE_DATA_ENTITY13:
    {
      if (nvramContext_p->nramState == VG_NVRAM_WRITE_CSAS)
      {
        copySpecificProfile (vgCiGetProfileForWriteReq(request_p),
                      &nvramContext_p->tmpSpecificProfile);
      }
      else
      {
        copySpecificProfile (vgCiGetProfileForWriteReq(request_p),
                      profileContext_p);
      }
      break;
    }

    case NRAM2_PROFILE_DATA_GENERIC:
    {
      if (nvramContext_p->nramState == VG_NVRAM_WRITE_CSAS)
      {
        copyGenericProfile (&request_p->data.profileDataGeneric.profile,
                          &nvramContext_p->tmpGenericProfile);
      }
      else
      {
        copyGenericProfile (&request_p->data.profileDataGeneric.profile,
                          profileGenericContext_p);
      }
      break;
    }

    default:
    {
      /* unexpected NVRAM data type */
      FatalParam (name, entity, 0);
      break;
    }
  }

  KiSendSignal (TASK_ANRM2_ID, &sigBuff);
}

/*--------------------------------------------------------------------------
*
* Function:    vgCiReadNvramCnf
*
* Parameters:  sig - a pointer to a structure of type
*              AnrmReadDataCnf contining the information
*              for the SIG_ANRM_READ_DATA_CNF signal.
*
* Returns:     nothing
*
* Description: Handles the SIG_ANRM_READ_DATA_CNF signal received
*              from the NRAM task's shell process.
*              The signal name is an alias of the NVRAM name.  The name
*              defines the type of NVRAM request and the state the operation
*              to be performed.  For example, a read request may be made
*              for the nvram profile data.
*
*-------------------------------------------------------------------------*/

static void vgCiReadNvramCnf( Anrm2ReadDataCnf         *sig,
                              const VgmuxChannelNumber  entity )
{
  Int32                         index;
  ResultCode_t                  result                      = RESULT_CODE_OK;
  ProfileContext_t              *profileContext_p           = ptrToProfileContext (entity);
  ProfileGenericContext_t       *profileGenericContext_p    = ptrToProfileGenericContext ();
  NvramContext_t                *nvramContext_p             = ptrToNvramContext (entity);
  SimLockGenericContext_t       *simLockGenericContext_p    = ptrToSimLockGenericContext ();
  AbmmWriteableData             *abmmWriteableData_p;
  const ProfileContext_t        *factorySpecificDefaults_p  = getFactorySpecificDefaults ();
  const ProfileGenericContext_t *factoryGenericDefaults_p   = getFactoryGenericDefaults ();

  ProfileContext_t              *signalProfileContext_p     = vgCiGetProfileForReadCnf(sig);
  GeneralContext_t              *generalContext_p           = ptrToGeneralContext(entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(generalContext_p != PNULL, entity, 0, 0);
#endif
  if ((nvramContext_p != PNULL) && (profileContext_p != PNULL))
  {
    if((sig->name == NRAM2_PROFILE_DATA_ENTITY0) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY1) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY2) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY3) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY4) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY5) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY6) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY7) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY8) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY9) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY10) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY11) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY12) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY13))
    {

      nvramContext_p->readProfileFlag = (Int8)((nvramContext_p->readProfileFlag)&(~SPECIFIC_PROFILE_DATA_BIT));
    }
    else if(sig->name == NRAM2_PROFILE_DATA_GENERIC)
    {
      nvramContext_p->readProfileFlag = (Int8)((nvramContext_p->readProfileFlag)&(~GENERIC_PROFILE_DATA_BIT));
    }
    else
    {
      /* nothing to do*/
    }

    if (((sig->status == NVRAM_OK) ||
         (sig->status == NVRAM_DEFAULT_CAL)) &&
        (sig->isDataValid == TRUE))
    {
      switch (sig->name)
      {
        case NRAM2_PROFILE_DATA_ENTITY0:
        case NRAM2_PROFILE_DATA_ENTITY1:
        case NRAM2_PROFILE_DATA_ENTITY2:
        case NRAM2_PROFILE_DATA_ENTITY3:
        case NRAM2_PROFILE_DATA_ENTITY4:
        case NRAM2_PROFILE_DATA_ENTITY5:
        case NRAM2_PROFILE_DATA_ENTITY6:
        case NRAM2_PROFILE_DATA_ENTITY7:
        case NRAM2_PROFILE_DATA_ENTITY8:
        case NRAM2_PROFILE_DATA_ENTITY9:
        case NRAM2_PROFILE_DATA_ENTITY10:
        case NRAM2_PROFILE_DATA_ENTITY11:
        case NRAM2_PROFILE_DATA_ENTITY12:
        case NRAM2_PROFILE_DATA_ENTITY13:
        case NRAM2_PROFILE_DATA_GENERIC:
        {
          switch (nvramContext_p->nramState)
          {
            case VG_NVRAM_READ_CRES:
            {
              switch(sig->name)
              {
                case NRAM2_PROFILE_DATA_ENTITY0:
                case NRAM2_PROFILE_DATA_ENTITY1:
                case NRAM2_PROFILE_DATA_ENTITY2:
                case NRAM2_PROFILE_DATA_ENTITY3:
                case NRAM2_PROFILE_DATA_ENTITY4:
                case NRAM2_PROFILE_DATA_ENTITY5:
                case NRAM2_PROFILE_DATA_ENTITY6:
                case NRAM2_PROFILE_DATA_ENTITY7:
                case NRAM2_PROFILE_DATA_ENTITY8:
                case NRAM2_PROFILE_DATA_ENTITY9:
                case NRAM2_PROFILE_DATA_ENTITY10:
                case NRAM2_PROFILE_DATA_ENTITY11:
                case NRAM2_PROFILE_DATA_ENTITY12:
                case NRAM2_PROFILE_DATA_ENTITY13:
                {
                  if (isSpecificProfileCodeWordValid (signalProfileContext_p) == TRUE)
                  {
                    result = setCresSpecificProfile (entity, signalProfileContext_p);
                  }
                  else
                  {
                    /* Incorrect profile in NVRAM */
                    result = setCresSpecificProfile (entity, factorySpecificDefaults_p);
                  }
                  break;
                }

                case NRAM2_PROFILE_DATA_GENERIC:
                {
                  if (isGenericProfileCodeWordValid (&sig->data.profileDataGeneric.profile) == TRUE)
                  {
                    result = setCresGenericProfile (entity, &sig->data.profileDataGeneric.profile);
                  }
                  else
                  {
                    /* Incorrect profile in NVRAM */
                    result = setCresGenericProfile (entity, factoryGenericDefaults_p);
                  }
                  break;
                }

                default:
                  break;
              }

              if (nvramContext_p->readProfileFlag == 0)
              {
                  nvramContext_p->nramState = VG_NVRAM_READ_ONLY;
                  setResultCode (entity, result);
              }
              break;
            }

            case VG_NVRAM_WRITE_CSAS:
            {
              switch(sig->name)
              {
                case NRAM2_PROFILE_DATA_ENTITY0:
                case NRAM2_PROFILE_DATA_ENTITY1:
                case NRAM2_PROFILE_DATA_ENTITY2:
                case NRAM2_PROFILE_DATA_ENTITY3:
                case NRAM2_PROFILE_DATA_ENTITY4:
                case NRAM2_PROFILE_DATA_ENTITY5:
                case NRAM2_PROFILE_DATA_ENTITY6:
                case NRAM2_PROFILE_DATA_ENTITY7:
                case NRAM2_PROFILE_DATA_ENTITY8:
                case NRAM2_PROFILE_DATA_ENTITY9:
                case NRAM2_PROFILE_DATA_ENTITY10:
                case NRAM2_PROFILE_DATA_ENTITY11:
                case NRAM2_PROFILE_DATA_ENTITY12:
                case NRAM2_PROFILE_DATA_ENTITY13:
                {
                  if (isSpecificProfileCodeWordValid (signalProfileContext_p) == TRUE)
                  {
                    copySpecificProfile (&nvramContext_p->tmpSpecificProfile,
                                  signalProfileContext_p);
                  }
                  else
                  {
                    /* Incorrect profile in NVRAM save whole factory default profile */
                    copySpecificProfile (&nvramContext_p->tmpSpecificProfile,
                                  factorySpecificDefaults_p);
                  }
                  nvramContext_p->tmpSpecificProfile.value[PROF_CMGF] =
                                         profileContext_p->value[PROF_CMGF];

                  nvramContext_p->tmpSpecificProfile.value[PROF_CSDH] =
                                         profileContext_p->value[PROF_CSDH];
                  break;
                }

                case NRAM2_PROFILE_DATA_GENERIC:
                {
                  if (isGenericProfileCodeWordValid (&sig->data.profileDataGeneric.profile) == TRUE)
                  {
                    copyGenericProfile (&nvramContext_p->tmpGenericProfile,
                                  &sig->data.profileDataGeneric.profile);
                  }
                  else
                  {
                    /* Incorrect profile in NVRAM save whole factory default profile */
                    copyGenericProfile (&nvramContext_p->tmpGenericProfile,
                                  factoryGenericDefaults_p);
                  }
                  /* copy sms profile settings */
                  for (index = 0; index < NUM_CNMI; index++ )
                  {
                    nvramContext_p->tmpGenericProfile.value[PROF_CNMI - NUM_OF_PROF_SPECIFIC + index] =
                                       profileGenericContext_p->value[PROF_CNMI - NUM_OF_PROF_SPECIFIC + index];
                  }
                  break;
                }

                default:
                  break;
              }

              if (nvramContext_p->readProfileFlag == 0)
              {
                  if (vgOpManAllocateConnection (entity, NVRAM_CONNECTION) == TRUE)
                  {
                      vgNvramDoNvramAccess (WRITE_REQUEST,
                                              entity,
                                                vgCiGetAnrm2DataName(entity),
                                                  VG_NVRAM_WRITE_CSAS);
                   }
              }
              break;
            }

            default:
            {
              /* send response using the profile values just set */
              if (!isEntityActive (entity))
              {
                /* if the data is not valid then reset to the factory defaults */
                switch(sig->name)
                {
                  case NRAM2_PROFILE_DATA_ENTITY0:
                  case NRAM2_PROFILE_DATA_ENTITY1:
                  case NRAM2_PROFILE_DATA_ENTITY2:
                  case NRAM2_PROFILE_DATA_ENTITY3:
                  case NRAM2_PROFILE_DATA_ENTITY4:
                  case NRAM2_PROFILE_DATA_ENTITY5:
                  case NRAM2_PROFILE_DATA_ENTITY6:
                  case NRAM2_PROFILE_DATA_ENTITY7:
                  case NRAM2_PROFILE_DATA_ENTITY8:
                  case NRAM2_PROFILE_DATA_ENTITY9:
                  case NRAM2_PROFILE_DATA_ENTITY10:
                  case NRAM2_PROFILE_DATA_ENTITY11:
                  case NRAM2_PROFILE_DATA_ENTITY12:
                  case NRAM2_PROFILE_DATA_ENTITY13:
                  {
                    if (isSpecificProfileCodeWordValid (signalProfileContext_p) == TRUE)
                    {
                      result = setStoredSpecificProfile (entity, signalProfileContext_p);
                    }
                    else
                    {
                      result = setDefaultSpecificProfile (entity, TRUE);
                    }
                    break;
                  }

                  case NRAM2_PROFILE_DATA_GENERIC:
                  {
                    if (isGenericProfileCodeWordValid (&sig->data.profileDataGeneric.profile) == TRUE)
                    {
                      result = setStoredGenericProfile (entity, &sig->data.profileDataGeneric.profile);
                    }
                    else
                    {
                      result = setDefaultGenericProfile (entity, TRUE);
                    }
                    break;
                  }

                  default:
                    break;
                }

                /* initialization of following values not success in  initialiseCrMan(),
                 * need get these values(currentAtCommand,connectionTypeMade,taskInitiated)
                 * when ATCI sends 'RING' to MMI, so reinitialises these values */
                if ( nvramContext_p->readProfileFlag == 0 )
                {
                  switch (sig->name)
                  {
                    case NRAM2_PROFILE_DATA_ENTITY0:
                    case NRAM2_PROFILE_DATA_ENTITY1:
                    case NRAM2_PROFILE_DATA_ENTITY2:
                    case NRAM2_PROFILE_DATA_ENTITY3:
                    case NRAM2_PROFILE_DATA_ENTITY4:
                    case NRAM2_PROFILE_DATA_ENTITY5:
                    case NRAM2_PROFILE_DATA_ENTITY6:
                    case NRAM2_PROFILE_DATA_ENTITY7:
                    case NRAM2_PROFILE_DATA_ENTITY8:
                    case NRAM2_PROFILE_DATA_ENTITY9:
                    case NRAM2_PROFILE_DATA_ENTITY10:
                    case NRAM2_PROFILE_DATA_ENTITY11:
                    case NRAM2_PROFILE_DATA_ENTITY12:
                    case NRAM2_PROFILE_DATA_ENTITY13:
                    case NRAM2_PROFILE_DATA_GENERIC:
                    {
                        setEntityState (entity, ENTITY_IDLE);
                        vgSendCiMuxChannelEnableRsp (entity, TRUE);
                        vgCiUserProfLoadedInd(entity);

                        setCommandId      (entity, VG_AT_NO_COMMAND);
                        setCommandName    (entity, (Char *) "");
                        setConnectionType (entity, CONNECTION_TERMINATOR);
                        setTaskInitiated  (entity, FALSE);

                        setEntityState (entity, ENTITY_NOT_ENABLED);
                        break;
                    }
                    default:
                        break;
                  }
                }
                break;
              }
              else
              {
                /*
                 * This part is for ATZ command.
                 */
                /* if the data is not valid then reset to the factory defaults */
                switch(sig->name)
                {
                  case NRAM2_PROFILE_DATA_ENTITY0:
                  case NRAM2_PROFILE_DATA_ENTITY1:
                  case NRAM2_PROFILE_DATA_ENTITY2:
                  case NRAM2_PROFILE_DATA_ENTITY3:
                  case NRAM2_PROFILE_DATA_ENTITY4:
                  case NRAM2_PROFILE_DATA_ENTITY5:
                  case NRAM2_PROFILE_DATA_ENTITY6:
                  case NRAM2_PROFILE_DATA_ENTITY7:
                  case NRAM2_PROFILE_DATA_ENTITY8:
                  case NRAM2_PROFILE_DATA_ENTITY9:
                  case NRAM2_PROFILE_DATA_ENTITY10:
                  case NRAM2_PROFILE_DATA_ENTITY11:
                  case NRAM2_PROFILE_DATA_ENTITY12:
                  case NRAM2_PROFILE_DATA_ENTITY13:
                  {
                    if (isSpecificProfileCodeWordValid (signalProfileContext_p) == TRUE)
                    {
                      result = setStoredSpecificProfile (entity, signalProfileContext_p);
                    }
                    else
                    {
                      result = setDefaultSpecificProfile (entity, TRUE);
                    }
                    break;
                  }

                  case NRAM2_PROFILE_DATA_GENERIC:
                  {
                    if (isGenericProfileCodeWordValid (&sig->data.profileDataGeneric.profile) == TRUE)
                    {
                      result = setStoredGenericProfile (entity, &sig->data.profileDataGeneric.profile);
                    }
                    else
                    {
                      result = setDefaultGenericProfile (entity, TRUE);
                    }
                    break;
                  }

                  default:
                    break;
                }
                if ((getCommandId (entity) == VG_AT_PF_Z) && (nvramContext_p->readProfileFlag == 0))
                {
                    setResultCode (entity, result);
                }
              }
              break;
            }
          }
          break;
        }
        case NRAM2_ABMM_WRITEABLE_DATA:
#if defined(UPGRADE_TWGGE_NRW)
        case NRAM2_ABMM_WRITEABLE_DATA_WGGE:
#endif
        {
          /* handle mm data.  This is used to specify the current
           * +CFUN state.
           */
          abmmWriteableData_p = (AbmmWriteableData *)&sig->data;

          simLockGenericContext_p->powerUpProtoStack = abmmWriteableData_p->powerUpProtoStack;
          simLockGenericContext_p->powerUpSim        = abmmWriteableData_p->powerUpSim;

          /* display system info */

          if(isEntityActive(entity)&&
             (!isEntityMmiNotUnsolicited(entity)))

          {
            /* send AT Ready */
            {
               vgPutNewLine (entity);

               vgPrintf (entity,
                             (const Char*)"*MATREADY: %d",
                               1);

               vgPutNewLine (entity);
               vgSetCirmDataIndIsUrc(entity, TRUE);
            }

            viewCFUN (entity);
            vgSetCirmDataIndIsUrc(entity, TRUE);

          }
          break;
        }

        default:
        {
          /* unexpected NVRAM data type */
          FatalParam (sig->name, entity, 0);
          break;
        }
      }
    }
    else
    {
      if ((getResultCode (entity) == RESULT_CODE_PROCEEDING) && (nvramContext_p->readProfileFlag == 0))
      {
        setResultCode (entity, RESULT_CODE_ERROR);
        nvramContext_p->nramState = VG_NVRAM_READ_ONLY;
      }
    }
  }
  else
  {
    if (getResultCode (entity) == RESULT_CODE_PROCEEDING)
    {
      setResultCode (entity, RESULT_CODE_ERROR);
    }
  }
}

/*--------------------------------------------------------------------------
*
* Function:    vgCiWriteNvramCnf
*
* Parameters:  sig - a pointer to a structure of type
*              AnrmWriteDataCnf contining the information
*              for the SIG_ANRM_WRITE_DATA_CNF signal.
*
* Returns:     nothing
*
* Description: Handles the SIG_ANRM_WRITE_DATA_CNF signal received
*              from the NRAM task's shell process.
*
*-------------------------------------------------------------------------*/

static void vgCiWriteNvramCnf (Anrm2WriteDataCnf *sig,
                                const VgmuxChannelNumber entity)
{

    NvramContext_t             *nvramContext_p              = ptrToNvramContext (entity);

    if((sig->name == NRAM2_PROFILE_DATA_ENTITY0) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY1) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY2) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY3) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY4) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY5) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY6) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY7) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY8) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY9) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY10) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY11) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY12) ||
       (sig->name == NRAM2_PROFILE_DATA_ENTITY13))
    {

      nvramContext_p->writeProfileFlag = (Int8)((nvramContext_p->writeProfileFlag)&(~SPECIFIC_PROFILE_DATA_BIT));
    }
    else if(sig->name == NRAM2_PROFILE_DATA_GENERIC)
    {
      nvramContext_p->writeProfileFlag = (Int8)((nvramContext_p->writeProfileFlag)&(~GENERIC_PROFILE_DATA_BIT));
    }
    else
    {
      /* nothing to do*/
    }

    if ((sig->status == NVRAM_OK) && (sig->wasWrittenOk == TRUE))
    {
        switch (sig->name)
        {
             case NRAM2_PROFILE_DATA_ENTITY0:
             case NRAM2_PROFILE_DATA_ENTITY1:
             case NRAM2_PROFILE_DATA_ENTITY2:
             case NRAM2_PROFILE_DATA_ENTITY3:
             case NRAM2_PROFILE_DATA_ENTITY4:
             case NRAM2_PROFILE_DATA_ENTITY5:
             case NRAM2_PROFILE_DATA_ENTITY6:
             case NRAM2_PROFILE_DATA_ENTITY7:
             case NRAM2_PROFILE_DATA_ENTITY8:
             case NRAM2_PROFILE_DATA_ENTITY9:
             case NRAM2_PROFILE_DATA_ENTITY10:
             case NRAM2_PROFILE_DATA_ENTITY11:
             case NRAM2_PROFILE_DATA_ENTITY12:
             case NRAM2_PROFILE_DATA_ENTITY13:
            {
                if ((getResultCode (entity) == RESULT_CODE_PROCEEDING) && (nvramContext_p->writeProfileFlag == 0))
                {
                    setResultCode (entity, RESULT_CODE_OK);
                }
                break;
            }

            case NRAM2_PROFILE_DATA_GENERIC:
            {
                if ((getResultCode (entity) == RESULT_CODE_PROCEEDING) && (nvramContext_p->writeProfileFlag == 0))
                {
                    setResultCode (entity, RESULT_CODE_OK);
                }
                break;
            }

            default:
            {
                /* unexpected NVRAM data type */
                FatalParam (sig->name, entity, 0);
                break;
            }
        }
    }
    else
    {
        if ((getResultCode (entity) == RESULT_CODE_PROCEEDING) && (nvramContext_p->writeProfileFlag == 0))
        {
            setResultCode (entity, RESULT_CODE_ERROR);
        }
    }


}

/*--------------------------------------------------------------------------
*
* Function:    vgNvramOperationCompleted
*
* Parameters:  signal_p - nvram read confirmation signal
*              entity   - mux channel number
*
* Returns:     nothing
*
* Description: Confirmation message received from the nvram task indicating
*              that the resource is now free.  If the initial nvram request
*              was a read then the signal will contain the extracted data.
*              If and entity is enabled by the mux then it needs to read the
*              profile data into nvram.
*              Code checks whether another entity has scheduled an operation
*              on the resource. If so executes that request and resets the
*              record to no_request.
*
*-------------------------------------------------------------------------*/

static void vgNvramOperationCompleted (const SignalBuffer*        signal_p,
                                        const VgmuxChannelNumber  entity)
{
  SignalBuffer           signal = kiNullBuffer;
  Int8                   eIndex;
  OpmanGenericContext_t* opManGenericContext_p = ptrToOpManGenericContext ();
  Anrm2ReadDataCnf       *anrmData_p   = (Anrm2ReadDataCnf *)(*signal_p).sig;
  Boolean                checkForPendingChannels = FALSE;
  NvramContext_t         *nvramContext_p         = ptrToNvramContext (entity);

#if defined (ENABLE_MATREADY_ON_ALL_CHANNELS)
  MuxContext_t           *muxContext_p = ptrToMuxContext ();
#endif

  /* if this is a read request then we need to check whether we should
  signal then mux and enable the entity.  It may be then the entity was
  blocked when it tried to enable itself */
  if ((*signal_p->type) == SIG_ANRM2_READ_DATA_CNF)
  {
#if defined(UPGRADE_TWGGE_NRW)
# if defined(UPGRADE_3G_TDD128)
    if (anrmData_p->name == NRAM2_ABMM_WRITEABLE_DATA)
# else
#  if defined(UPGRADE_3G_FDD)
    if (anrmData_p->name == NRAM2_ABMM_WRITEABLE_DATA_WGGE)
#  else
    if (anrmData_p->name == NRAM2_ABMM_WRITEABLE_DATA)
#  endif
# endif
#else
    if (anrmData_p->name == NRAM2_ABMM_WRITEABLE_DATA)
#endif
    {
      checkForPendingChannels = TRUE;
    }
    else if (anrmData_p->name == NRAM2_MUX_CONFIG_DATA)
    {
    }
    else
    {
      /* if this entity is not active then we need to enable it and let the mux know */

      if ( ( !isEntityActive (entity)) &&
           ( !opManGenericContext_p->peripheralControlBuffer [NVRAM_CONNECTION] ) &&
           ( nvramContext_p->readProfileFlag == 0))
      {

        if (!isEntityActive (entity))
        {
          setEntityState (entity, ENTITY_IDLE);
#if defined (ENABLE_MATREADY_ON_ALL_CHANNELS)
          /* This is actually where the channel is now up and ready to use after
           * being enabled.  So now sent *MATREADY.  NOTE: For the first channel
           * this is done elsewhere - so this call does not apply for the first channel
           */
          if (muxContext_p->atciHaveReadPhoneFunctionality)
          {
            /* Send *MATREADY on enabled channel.  The first enabled channel will
             * generate this later
             */
            vgPutNewLine (entity);

            vgPrintf (entity,
                         (const Char*)"*MATREADY: %d",
                           1);

            vgPutNewLine (entity); 
            vgSetCirmDataIndIsUrc(entity, TRUE);
            vgFlushBuffer(entity);
          }
#endif          
        }

        /* sync to the PS, this will only happen on the first entity */
        vgSyncWithPs (entity);

        checkForPendingChannels = TRUE;


        /* if just enabled unsolicited channel then get phone functionality information */
        checkForPendingChannels = vgReadAbmmWritableData (entity);
      }
    }

    if (checkForPendingChannels)
    {

      /* schedule any outstanding ENABLE request */
      for (eIndex = 0;
           eIndex < CI_MAX_ENTITIES;
           eIndex++)
      {
        if ( opManGenericContext_p->channelNeedsEnabling [eIndex] == TRUE )
        {
          signal = kiNullBuffer;

          KiCreateZeroSignal (SIG_CIMUX_CHANNEL_ENABLE_IND,
                              sizeof (CiMuxChannelEnableInd),
                              &signal);

          signal.sig->ciMuxChannelEnableInd.channelNumber = opManGenericContext_p->channelEnableIndChannelNumber[eIndex];

          KiSendSignal (VG_CI_TASK_ID, &signal);

          opManGenericContext_p->channelNeedsEnabling [eIndex] = FALSE;
          break;
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------
*
* Function:    vgNvramDoNvramAccess
*
* Parameters:  nvramOperation - type of request (read/write)
*              entity         - mux channel number
*              accessClass    - data to access
*              state          - type of read/write operation
*
* Returns:     nothing
*
* Description: Determines whether an NVRAM read or write operation is required
*
*-------------------------------------------------------------------------*/

void vgNvramDoNvramAccess (const NvramOperation_t  nvramOperation,
                            const VgmuxChannelNumber entity,                           
#if defined(UPGRADE_TWGGE_NRW)
				 Anrm2DataName  accessClass,	
#else
                             const Anrm2DataName  accessClass,
#endif                             
                              const VgNvramState  state)
{
  NvramContext_t         *nvramContext_p        = ptrToNvramContext (entity);
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck(nvramContext_p != PNULL, entity, 0, 0);
#endif
 /* Mux Data is read for Unsolicited entity, does not have an associated NVRAM state*/
  vgCiSetNvramState (state, entity);

#if defined(UPGRADE_TWGGE_NRW)
# if defined(UPGRADE_3G_FDD)
  if(accessClass == NRAM2_MOBILE_EQUIPMENT_DATA)
  {
    accessClass = NRAM2_MOBILE_EQUIPMENT_DATA_WGGE;
  }
  else if(accessClass == NRAM2_ABMM_WRITEABLE_DATA)
  {
    accessClass = NRAM2_ABMM_WRITEABLE_DATA_WGGE;
  }
# endif  
#endif

  switch (nvramOperation)
  {
    case READ_REQUEST:
    {
      switch (accessClass)
      {
        case NRAM2_PROFILE_DATA_ENTITY0:
        case NRAM2_PROFILE_DATA_ENTITY1:
        case NRAM2_PROFILE_DATA_ENTITY2:
        case NRAM2_PROFILE_DATA_ENTITY3:
        case NRAM2_PROFILE_DATA_ENTITY4:
        case NRAM2_PROFILE_DATA_ENTITY5:
        case NRAM2_PROFILE_DATA_ENTITY6:
        case NRAM2_PROFILE_DATA_ENTITY7:
        case NRAM2_PROFILE_DATA_ENTITY8:
        case NRAM2_PROFILE_DATA_ENTITY9:
        case NRAM2_PROFILE_DATA_ENTITY10:
        case NRAM2_PROFILE_DATA_ENTITY11:
        case NRAM2_PROFILE_DATA_ENTITY12:
        case NRAM2_PROFILE_DATA_ENTITY13:
        {
            nvramContext_p->readProfileFlag              = 0;
            nvramContext_p->readProfileFlag |= SPECIFIC_PROFILE_DATA_BIT;
            vgCiReadNvram (NRAM2_PROFILE_DATA_GENERIC, entity);
            nvramContext_p->readProfileFlag |= GENERIC_PROFILE_DATA_BIT;
            break;
        }
        default:
            break;
      }
      vgCiReadNvram (accessClass, entity);
      break;
    }

    case WRITE_REQUEST:
    {
      switch (accessClass)
      {
        case NRAM2_PROFILE_DATA_ENTITY0:
        case NRAM2_PROFILE_DATA_ENTITY1:
        case NRAM2_PROFILE_DATA_ENTITY2:
        case NRAM2_PROFILE_DATA_ENTITY3:
        case NRAM2_PROFILE_DATA_ENTITY4:
        case NRAM2_PROFILE_DATA_ENTITY5:
        case NRAM2_PROFILE_DATA_ENTITY6:
        case NRAM2_PROFILE_DATA_ENTITY7:
        case NRAM2_PROFILE_DATA_ENTITY8:
        case NRAM2_PROFILE_DATA_ENTITY9:
        case NRAM2_PROFILE_DATA_ENTITY10:
        case NRAM2_PROFILE_DATA_ENTITY11:
        case NRAM2_PROFILE_DATA_ENTITY12:
        case NRAM2_PROFILE_DATA_ENTITY13:
        {
            nvramContext_p->writeProfileFlag             = 0;
            nvramContext_p->writeProfileFlag |= SPECIFIC_PROFILE_DATA_BIT;
            vgCiWriteNvram (NRAM2_PROFILE_DATA_GENERIC, entity);
            nvramContext_p->writeProfileFlag |= GENERIC_PROFILE_DATA_BIT;
            break;
        }
        default:
            break;
      }
      vgCiWriteNvram (accessClass, entity);
      break;
    }

    default:
    {
      break;
    }
  }
}

/*--------------------------------------------------------------------------
*
* Function:    vgNvramInitialise
*
* Parameters:  none
*
* Returns:     nothing
*
* Description: Does nothing at present
*
*-------------------------------------------------------------------------*/

static void vgNvramInitialise (void)
{
}

/*--------------------------------------------------------------------------
*
* Function:    vgCiWriteSymbolRotationModeNvram
*
* Parameters:  newRotationModeActive - whether active new symbol rotation and scrambling code
*                      if true activate new algorithm, if false not
*
* Returns:     TRUE - Set and write successful,  FALSE, write nvram fail.
*
* Description:  Setting of the rotation mode is permanent. NPHY stores the value
*                   to non-volatile memory and applies the value until new value is given via this function
*
*-------------------------------------------------------------------------*/
Boolean vgCiWriteSymbolRotationModeNvram(Boolean newRotationModeActive)
{
    
    GeneralGenericContext_t *generalGenericContext_p = ptrToGeneralGenericContext();
    VgNvmMspchscContext     *mspchscContext_p        = &(generalGenericContext_p->vgNvmMspchscContext);
    Boolean                  result                  = TRUE;

    if ((newRotationModeActive != mspchscContext_p->curNpbchSymbolRotationMode)
     || (FALSE == mspchscContext_p->NpbchSymbolRotationModeInitDone))
     {
    #ifdef MTK_NVDM_MODEM_ENABLE

        nvdm_modem_status_t nvdm_status;

        /* * * Write mode to from non-volatile memory * * */
        nvdm_status = nvdm_modem_write_normal_data_item(VGCI_NPHY_SIG_GROUP_NAME,
            VGCI_NPHY_SIG_ITEM1_NAME,
            NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA,
            (const uint8_t *)&newRotationModeActive,
            VGCI_NPHY_SIG_ITEM1_LENGTH,
            NVDM_MODEM_ATTR_AVERAGE);

        /* Check non-volatile memory writing status */
        if (nvdm_status != NVDM_MODEM_STATUS_OK)
        {
            /* Writing to non-volatile memory failed */
            FatalParam (nvdm_status, newRotationModeActive, 0);
            result = FALSE;
        }
        else
    #endif/*MTK_NVDM_MODEM_ENABLE*/
        {
            /*updated *MSPCHSC context information*/
            mspchscContext_p->curNpbchSymbolRotationMode      = newRotationModeActive;
            mspchscContext_p->NpbchSymbolRotationModeInitDone = TRUE;
           
        }
     }
    
    return result;
    
}

/*--------------------------------------------------------------------------
*
* Function:    vgCiReadSymbolRotationModeNvram
*
* Parameters:  N/A
*
* Returns:     Current NPBCH symbol rotation mode value.
*
* Description:  query the currently active NPBCH symbol rotation mode.
*
*-------------------------------------------------------------------------*/
Boolean vgCiReadSymbolRotationModeNvram(void)
{
    GeneralGenericContext_t *generalGenericContext_p = ptrToGeneralGenericContext();
    VgNvmMspchscContext     *mspchscContext_p        = &(generalGenericContext_p->vgNvmMspchscContext);

#ifdef MTK_NVDM_MODEM_ENABLE
    Boolean     curSymbolRotationMode;
    
    /* If mode not initialized yet, read stored mode from non-volatile memory*/
    if (FALSE == mspchscContext_p->NpbchSymbolRotationModeInitDone)
    {
        nvdm_modem_status_t         nvdm_status;
        nvdm_modem_data_item_type_t type         = NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA;
        uint32_t                    dataLenBytes = VGCI_NPHY_SIG_ITEM1_LENGTH;

        /* * * Read mode from from non-volatile memory * * */
        nvdm_status = nvdm_modem_read_normal_data_item(VGCI_NPHY_SIG_GROUP_NAME,
            VGCI_NPHY_SIG_ITEM1_NAME,
            &type,
            (uint8_t *)&curSymbolRotationMode,
            &dataLenBytes);

        /*If non-volatile memory reading succeeded*/
        if (nvdm_status == NVDM_MODEM_STATUS_OK &&
            type == NVDM_MODEM_DATA_ITEM_TYPE_RAW_DATA &&
            dataLenBytes == VGCI_NPHY_SIG_ITEM1_LENGTH)
        {
            mspchscContext_p->NpbchSymbolRotationModeInitDone = TRUE;
            mspchscContext_p->curNpbchSymbolRotationMode      = curSymbolRotationMode;
        }
        else if(nvdm_status == NVDM_MODEM_STATUS_ITEM_NOT_FOUND)
        {
            /* Have not written to NVRAM, so read fail, set to default value*/
            mspchscContext_p->curNpbchSymbolRotationMode = PROF_MSPCHSC_NEW_SCRAMBLE;
        }
        else
        {
          /* Read NVRAM fail, Assert */
          FatalParam (nvdm_status, type, dataLenBytes);
        }
    }

#endif

    return mspchscContext_p->curNpbchSymbolRotationMode;

}
/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
*
* Function:    vgNvramInterfaceController
*
* Parameters:  signal_p - structure containing incoming signal
*              entity   - mux channel number
*
* Returns:     Boolean - indicates whether the sub-system has recognised and
*                        procssed the signal given.
*
* Description: determines action for received signals
*
*-------------------------------------------------------------------------*/

Boolean vgNvramInterfaceController (const SignalBuffer *signal_p,
                                     const VgmuxChannelNumber entity)
{
  Boolean accepted = FALSE;

  switch (*signal_p->type)
  {
    case SIG_INITIALISE:
    {
      vgNvramInitialise();
      break;
    }
    case SIG_ANRM2_READ_DATA_CNF:
    {
      vgOpManDropConnection (entity, NVRAM_CONNECTION);
      {
        vgCiReadNvramCnf ((Anrm2ReadDataCnf *) (*signal_p).sig, entity);
        vgNvramOperationCompleted (signal_p, entity);
      }
      accepted = TRUE;
      break;
    }
    case SIG_ANRM2_WRITE_DATA_CNF:
    {
      vgOpManDropConnection (entity, NVRAM_CONNECTION);
      vgCiWriteNvramCnf ((Anrm2WriteDataCnf *) (*signal_p).sig, entity);
      vgNvramOperationCompleted (signal_p, entity);
      accepted = TRUE;
      break;
    }
    default:
    {
      break;
    }
  }
  return (accepted);
}




