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
 * RVCTCFG.C
 * Allows object customers to modify profile factory defaults and identification
 * command responses
 **************************************************************************/

#define MODULE_NAME "RVCTCFG"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <rvsystem.h>
#include <rvctcfg.h>
#include <abgl_typ.h>
#include <pdn_typ.h>
#include <rvdata.h>

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* Definition of profile factory defaults */

static const ProfileContext_t factorySpecificDefaultProfile =
{
  {
    1,  /*  E                 */
    0,  /*  L                 */
    0,  /*  M                 */
    0,  /*  Q                 */
#if defined (UPGRADE_RAVEN_NO_VERBOSE)
    0,  /*  V                 */
#else
    1,  /*  V                 */
#endif
    4,  /*  X                 */
    1,  /*  &C                */
    2,  /*  &D                */
    0,  /*  S0                */
    0,  /*  S1                */
    43, /*  S2                */
    13, /*  S3                */
    10, /*  S4                */
    8,  /*  S5                */
    2,  /*  S6                */
#if defined (ENABLE_PS_ONLY_TARGET)
    120,/*  S7                */
#else
/*according to 24.008 network MIN time is 180S*/
    255, /*  S7                */
#endif
    2,  /*  S8                */
    15, /*  S10               */
    50, /*  S12               */
    5,  /*  S25               */
    0,  /*  +CR               */
    0,  /*  +FCLASS           */
    2,  /*  +IFC RTS/CTS UL   */
    2,  /*  +IFC RTS/CTS DL   */
    3,  /*  +ICF  -format     */
    3,  /*        -parity     */

    0,  /*  +DR               */
    3,  /*  +DS   -p0         */
    0,  /*        -n          */
    2,  /*        -p1         */
    0,
    20, /*        -p2         */
    0,  /*  +CMGF             */
    0,  /*  +CSDH             */
 #if !defined (VG_DEFAULT_IPR)
    5,  /*  +IPR              */
#else
    VG_DEFAULT_IPR,
#endif
    0,  /*  +ILRR             */
    0,  /*  *MSIMINS          */
    2,  /*  +CSCS             */
#if defined  (DEVELOPMENT_VERSION)
#if defined (UPGRADE_RAVEN_NO_VERBOSE)
    1,  /*  +CMEE             */
#else
    2,  /*  +CMEE             */
#endif
#else
#if defined (UPGRADE_RAVEN_NO_VERBOSE)
    1,  /*  +CMEE             */
#else
    0,  /*  +CMEE             */
#endif
#endif
    0,  /*  +CREG             */
    0,  /*  +CGREG            */
    0,  /*  +CEREG            */
    0,  /*  +MUX              */
    0,  /*  *MUNSOL           */
    0,  /*  +CTZR             */
    0,  /*  +CDIP             */
    0,  /*  ^MODE             */
#if defined (FEA_MT_PDN_ACT)
    3,  /* +CGAUTO */
#endif
    0,  /* +CGEREP */
#if defined (FEA_MT_PDN_ACT)
    1,  /* *MGMTPCACT */
#endif
    2,  /* *MSTMODE             */
#if defined (FEA_PPP)    
    0,  /* *MGPPPLOG            */
#endif /* FEA_PPP */    
    0,  /* *MMGI                */
    0,  /* *MROUTEMMI           */
    0,  /* PROF_MUPBCFG         */
    0,  /* PROF_MCEERMODE       */
    0,  /* PROF_CGPIAF - IPV6 addr format    */
    0,  /*             - subnet notation     */
    0,  /*             - IPV6 leading zeros  */
    0,  /*             - IPV6 compress zeros */
    0,  /* PROF_MCGEUNSOL       */
    0,  /* PROF_MLTS            */
    0,  /* *MUAPP */
#if defined (ENABLE_AT_TRACE)
    0,  /* +TRACE*/
#endif
    0,  /* PROF_MLTEGCFLOCK */
    /* For NB-IOT */
    0,  /* PROF_CEDRXS */
    0,  /* PROF_CCIOTOPT */
#if defined (FEA_NFM)
    0,  /*   PROF_NFM */
#endif
    0,  /* PROF_MPDI */
    0,  /* PROF_MPLMNURI */
    0,  /* PROF_MAPNURI */
    0,  /* PROF_MATWAKEUP */
    0,  /* PROF_MDPDNP */
    0,  /* PROF_MNBIOTEVENT */
    0,  /* PROF_MOOSAIND */
    6,  /* NVRAM codeword       */
    1,
    8,
    9,
    3,
    7,
    4,
    11
  }
};

static const ProfileGenericContext_t factoryGenericDefaultProfile =
{
  {
    0,  /*  +CSMS             */
    2,  /*  +CNMI -mode       */
    1,  /*        -mt         */
    0,  /*        -bm         */
    0,  /*        -ds         */
    0,  /*        -bfr        */
    0,  /*  +CTZU             */

#if defined (ENABLE_CID0)
    2,	/* PROF_MLTEGCF */
#else
#if defined (DISABLE_ATCI_LTE_GCF_SUPPORT)
    0,  /* PROF_MLTEGCF */
#else /* DISABLE_ATCI_LTE_GCF_SUPPORT */
    1,  /* PROF_MLTEGCF */
#endif /* else DISABLE_ATCI_LTE_GCF_SUPPORT */
#endif /* ENABLE_CID0 */

    3,  /* NVRAM codeword       */
    4,
    5,
    9,
    3,
    10,
    4,
    11
  }
};

/* list of commands that may not be run  */
static const CommandId_t restrictedCommands[] =
{
  VG_AT_NO_COMMAND
};

static const Char defaultSourceAddressIPV4[] = "0.0.0.0";

static const Char defaultSourceAddressIPV6[] = "0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0";

static const Char defaultSubnetMaskIPV4[] = "255.255.255.0";

static const Char defaultSubnetMaskIPV6[] = "255.255.255.255.255.255.255.255.255.255.255.255.255.255.255.0";

#define NUM_ENTRIES(X) (sizeof(X) / sizeof(X[0]))

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

/***************************************************************************
 * Types
 ***************************************************************************/

/***************************************************************************
 * Variables
 ***************************************************************************/
/* the following variable defines the set of access control bits which are */
/* ignored when determining if a command is viable. A value of 0 means     */
/* that all make bits are used, a mask of all 1 will ignore the whole      */
/*access control mechanism                                                 */

AtCmdAccessMask restrictAccessMask = AT_CMD_ACCESS_NONE;

/***************************************************************************
 * Macros
 ***************************************************************************/

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

 /***************************************************************************
 * Static Functions
 ***************************************************************************/

/***************************************************************************
 * Global Functions
 ***************************************************************************/

 /*--------------------------------------------------------------------------
 *
 * Function:    isCommandExecutable
 *
 * Parameters:  commandId - command to look up in restricted table
 *
 * Returns:     Boolean - whether command can be run in SIM-locked state
 *
 * Description: Returns whether specified command is executable
 *              This routine can be used to apply any form of command
 *              restriction which the user wishes and can, along with the
 *              restrictAccessMask variable can override all standard
 *              command access restrictions
 *
 *              The example code implements a list of restricted commands
 *              but this can be rewritten as required
 *
 *-------------------------------------------------------------------------*/

Boolean isCommandExecutable (const CommandId_t commandId)
{
  Boolean commandExecutable = TRUE;
  Int32   commandIndex;

  for (commandIndex = 0;
       (commandIndex < NUM_ENTRIES(restrictedCommands)) && (commandExecutable);
         commandIndex++)
  {
    if (restrictedCommands[commandIndex] == commandId)
    {
      commandExecutable = FALSE;
    }
  }

  return (commandExecutable);
}

 /*--------------------------------------------------------------------------
 *
 * Function:    getFactorySpecificDefaults
 *
 * Parameters:  none
 *
 * Returns:     ProfileContext_t *
 *
 * Description: Returns specific profile factory default
 *
 *-------------------------------------------------------------------------*/

const ProfileContext_t *getFactorySpecificDefaults (void)
{
  return (&factorySpecificDefaultProfile);
}


 /*--------------------------------------------------------------------------
 *
 * Function:    getFactoryGenericDefaults
 *
 * Parameters:  none
 *
 * Returns:     ProfileGenericContext_t *
 *
 * Description: Returns generic profile factory default
 *
 *-------------------------------------------------------------------------*/

const ProfileGenericContext_t *getFactoryGenericDefaults (void)
{
  return (&factoryGenericDefaultProfile);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgGetDefaultSourceAddress 
 *
 * Parameters:  pdnType
 *
 * Returns:     const char * - defaultSourceAddress
 *
 * Description: Returns default source address
 *
 *-------------------------------------------------------------------------*/

const Char *vgGetDefaultSourceAddress (PdnType pdnType)
{
   
  if (PDN_TYPE_IPV4 == pdnType)
  {
    return (defaultSourceAddressIPV4);    
  }
  else
  if (PDN_TYPE_IPV6 == pdnType)
  {
    return (defaultSourceAddressIPV6);    
  }
  else
  {
    FatalParam(pdnType, 0, 0);
  }  
  
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgGetDefaultSubnetMask 
 *
 * Parameters:  pdnType
 *
 * Returns:     const char * - DefaultSubnetMask
 *
 * Description: Returns default subnet mask
 *
 *-------------------------------------------------------------------------*/
const Char *vgGetDefaultSubnetMask (PdnType pdnType)
{
    
  if (PDN_TYPE_IPV4 == pdnType)
  {
    return (defaultSubnetMaskIPV4);    
  }
  else
  if (PDN_TYPE_IPV6 == pdnType)
  {
    return (defaultSubnetMaskIPV6);    
  }
  else
  {
    FatalParam(pdnType, 0, 0);
  } 
}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

