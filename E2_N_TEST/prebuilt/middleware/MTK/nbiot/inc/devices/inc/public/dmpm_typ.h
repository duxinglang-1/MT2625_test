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
 * DM Power Control Module types
 **************************************************************************/
#if !defined (DMPM_TYP_H)
#define       DMPM_TYP_H

/** \addtogroup PrdDevicesPmSig
 * .
 *
 * Power Management - Public types used in APIs and signalling.
 *
 * @{
 */

 /****************************************************************************
 * Type Definitions
 ****************************************************************************/

/** PowerOffReason is the reason code for a power down of the handset */
typedef enum PowerOffReasonTag
{
  POWER_OFF_NORMAL,               /**< User has pressed power off switch */
  POWER_OFF_RADIO_FAILURE,        /**< L1 has reported catastrophic radio failure */
  POWER_OFF_INTERNAL,             /**< Unhandlable Internal problem */
  POWER_OFF_ALARM,                /**< Alarm */
  POWER_OFF_BATTERY_EMPTY,        /**< No fuel in battery */
  POWER_OFF_CHARGER_OUT,          /**< Charger removed while charging */
  POWER_OFF_CUSTOMER_DEFINED_1,   /**< Customer Defined Power Off */
  POWER_OFF_CUSTOMER_DEFINED_2,   /**< Customer Defined Power Off */
  POWER_OFF_CUSTOMER_DEFINED_3,   /**< Customer Defined Power Off */
  POWER_OFF_CUSTOMER_DEFINED_4,   /**< Customer Defined Power Off */
  POWER_OFF_CUSTOMER_DEFINED_5,   /**< Customer Defined Power Off */
  POWER_OFF_CUSTOMER_DEFINED_6,   /**< Customer Defined Power Off */
  POWER_OFF_CUSTOMER_DEFINED_7,   /**< Customer Defined Power Off */
  POWER_OFF_CUSTOMER_DEFINED_8,   /**< Customer Defined Power Off */
  POWER_OFF_CUSTOMER_DEFINED_9,   /**< Customer Defined Power Off */
  POWER_OFF_CUSTOMER_DEFINED_10,  /**< Customer Defined Power Off */
  POWER_OFF_CUSTOMER_DEFINED_11,  /**< Customer Defined Power Off */
  POWER_OFF_CUSTOMER_DEFINED_12,  /**< Customer Defined Power Off */
  POWER_OFF_CUSTOMER_DEFINED_13,  /**< Customer Defined Power Off */
  POWER_OFF_CUSTOMER_DEFINED_14,  /**< Customer Defined Power Off */
  POWER_OFF_CUSTOMER_DEFINED_15,  /**< Customer Defined Power Off */
  /** Number of reasons */
  NUM_POWER_OFFS,
  /** Null setting */
  POWER_OFF_NULL
}
PowerOffReason;

/** Component power state.
 * PmSimPowerState is a data structure whose members describe the required power state of 
 * the protocol-stack/SIM and whether or not, this requirement should be made persistent between MS power 
 * cycles. 
 */
typedef struct PmSimPowerStateTag
{
   /** TRUE if the associated protocol-stack/SIM powerUp state should be stored to 
    * NVRAM (so it can take effect when the MS is power cycled); FALSE if this 
    * protocol-stack/SIM power-state is not to be remembered between MS power-cycles. 
    */
   Boolean  makePersistent;
   /** TRUE if the protocol-stack/SIM is to be powered up; 
    * FALSE if the protocol-stack/SIM is to be powered-down. 
    */
   Boolean  powerUp;
  /** TRUE if the protocol-stack/SIM needs to be powered up after protocol-stack/SIM power down; 
    * FALSE protocol-stack/SIM does not needs to be powered up after protocol-stack/SIM power down. 
    */
   Boolean  postPowerUp;
}
PmSimPowerState,
/** Describes the required power state of 
 * the protocol stack and whether or not, this requirement should be made persistent between MS power 
 * cycles. 
 */
PmProtoStackPowerState;

/** Power request structure.
 * PmRequiredPowerState is a data structure whose members describe the required state 
 * of the protocol stack and SIM, as used within the ApexPmModeChangeReq signal. 
 */
typedef struct PmRequiredPowerStateTag
{
   /** TRUE if new SIM/protocol-stack power state is to be implemented immediately;
    * FALSE if the SIM/protocol-stack power state is not to be implemented immediately.  
    */
   Boolean                 resetNow;
   /** SIM power, persistance and on/off state */
   PmSimPowerState         simState;
   /** Protocol Stack power, persistance and on/off state */
   PmProtoStackPowerState  protoStackState;
}
PmRequiredPowerState;


/** @} */ /* End of group. */


#endif
/* END Of FILE */
