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
 **************************************************************************/
/** \file
 * This file contains the customer configuration options for the CI Task.
 **************************************************************************/

#ifndef RVCTCFG_H
#define RVCTCFG_H

/** \defgroup CfgCiTask CI Task Configuration
 * \ingroup PrdDataServices
 * \ingroup Cfg
 * The Command Interpreter (CI) Task configuration.
 * This includes functions that allow customer specific configurations. For
 * example, specifying the terminal identification text and the default
 * profile settings.
 * @{
 */

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>

#if !defined (RVPROF_H)
#  include <rvprof.h>
#endif

#include <pdn_typ.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/** The following enumerate values are used to form a mask which is used to
 * determine the features which must be present for each AT command to run.
 * All the values which are present in the mask for a command must be set or
 * the command will not be executed.
 */
typedef enum AtCmdAccessMaskTag
{
  AT_CMD_ACCESS_NONE                   = 0x00,  /**< No pre-requisites. */
  AT_CMD_ACCESS_CFUN_1                 = 0x01,  /**< Requires Application Stack and SIM to
                                                 * be powered up. */
  AT_CMD_ACCESS_SIM_PRESENT            = 0x02,  /**< SIM must be present. */
  AT_CMD_ACCESS_SIM_READY              = 0x04,  /**< SIM must be ready. */
  AT_CMD_ACCESS_SIM_READY_AND_PRESENT  = (AT_CMD_ACCESS_SIM_PRESENT | AT_CMD_ACCESS_SIM_READY),  /**< SIM must be ready and present. */
  AT_CMD_ACCESS_GSM_REGISTERED         = 0x08,  /**< Must be registered for GSM. */
  AT_CMD_ACCESS_GPRS_REGISTERED        = 0x10,   /**< Must be registered for GPRS. */
  AT_CMD_ACCESS_CFUN_7                 = 0x20    /**< Requires Application Stack to
                                                  * be powered up only */
} AtCmdAccessMask;

/** This structure is used to hold generic context data for the custom
 * sub-system. The information it contains is relevant to all entities.
 */
typedef struct CustomGenericContextTag
{
  /* VOP PRACTICAL (2):
   *   Variables required for adding AT command to perform polled ADC read:
   */
  Boolean   adcStateRunning;  /**< Whether polled ADC reads are taking place. */
  Int32     timeoutPeriod;    /**< The timeout value used between polled ADC
                               * reads. */
} CustomGenericContext_t;

/** This structure is used to hold entity-specific context data for the custom
 * sub-system.
 */
typedef struct CustomContextTag
{
  Int8  customEntityInt;  /**< An example integer value. */

  /* VOP PRACTICAL (1):
   *   Variables required for adding AT command to perform ADC read:
   */
  Int8  adcToUse;
} CustomContext_t;

/***************************************************************************
 * Variables
 ***************************************************************************/

/** This AtCmdAccessMask variable defines the set of access control bits which
 * are ignored when determining if a command is viable. A value of 0 means
 * that all make bits are used, a mask of all 1 will ignore the whole access
 * control mechanism.
 */
extern AtCmdAccessMask restrictAccessMask;

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

/** This function returns whether the specified command is executable.
 * This routine can be used to apply any form of command restriction which the
 * user wishes and can, along with the restrictAccessMask variable can override
 * all standard command access restrictions.
 *
 * \param commandId  An enumerate specifying an AT command.
 *
 * \retval Boolean TRUE if the command may be executed.
 *
 */
Boolean isCommandExecutable (const CommandId_t commandId);

/** This function returns the factory default profile values.
 *
 * \retval ProfileContext_t*  The default profile values.
 *
 */
const ProfileContext_t* getFactorySpecificDefaults (void);
const ProfileGenericContext_t* getFactoryGenericDefaults (void);

const Char *vgGetDefaultSourceAddress(PdnType pdnType);

const Char *vgGetDefaultSubnetMask(PdnType pdnType);

/** @} */ /* End of CfgCiTask group */

#endif  /* RVCTCFG_H */

/* END OF FILE */

