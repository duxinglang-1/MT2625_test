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
*******************************************************************************/
#ifndef __MDSYS_CLOCK_MANAGER_H__
#define __MDSYS_CLOCK_MANAGER_H__

/*****************************************************************************
* Include
*****************************************************************************/
#include "n1_hw_md_clock.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
* Enum
*****************************************************************************/

typedef struct MdsysClkMgrResSts
{
    uint32_t mdsysclksts;
    uint32_t mdsysclkconf1sts;

} MdsysClkMgrResSts;


/*****************************************************************************
* Functions
*****************************************************************************/

/**
* @brief       This function initializes the mdsys Clock Manager.
* @return      #MDSYS_CLOCK_MANAGER_OK, if this function completed successfully. */
mdsys_clock_manager_status_t N1MdsysClockManagerInit(void);

/**
* @brief       This function sets up a mdsys clock handle to control the clock request of the modem sub system.
*              This handle enables the mdsys Clock Manager to identify the clock states based on the mdsys clock handles assigned.
* @param [in]  handle_name is the name of the clock handle of type string.
* @return      The mdsys clock handle index, if the operation is successful. If (MDSYS_CLOCK_MANAGER_ERROR) is returned, the operation failed. */
uint8_t N1MdsysClockManagerRegister( const char *handle_name);

/**
 * @brief       This function releases the mdsys clock handle if it is no longer in use. \n
 * @param[in]   handle_index is the mdsys clock handle index returned from #N1MdsysClockManagerRegister(). \n
 * @return      #MDSYS_CLOCK_MANAGER_OK, if this function completed successfully.
 * @note        Call #N1MdsysClockManagerRegister() to set up the mdsys clock handle, before calling this function. */
mdsys_clock_manager_status_t N1MdsysClockManagerRelease( uint8_t handle_index);

/**
 * @brief       The mdsys clock reource status is represented by an unsigned integer (uint32_t). \n
 *              Each mdsys clock handle corresponds to a bit position in this unsigned integer. \n
 *              If the mdsys clock handle index is 5, for example, the value at the 5th bit position of this unsigned \n
 *              integer represents the status of the mdsys clock handle of index 5. If the value of the 5th bit is 1, \n
 *              it indicates that the mdsys clock handle with index 5 is currently holding the mdsys clock resouces are in use.
 * @param[in]
 * @return      return true when MDSYS clock resource are in use.
 * @note        Call #N1MdsysClockManagerRegister() to set up the mdsys clock handle, before calling this function. */
bool N1MdsysClockManagerResourceStatus(void);

/**
 * @brief       This function enables mdsys clock resource from modem sub system. \n
 * @param1[in]  handle_index is the mdsys clock handle index, returned from #N1MdsysClockManagerRegister().
 * @param2[in]  clock resource to be enabled.
 * @return      #MDSYS_CLOCK_MANAGER_OK, if mdsys clock resource is enabled.
 * @note        Call #N1MdsysClockManagerRegister() to set up the mdsys clock handle, before calling this function. */
mdsys_clock_manager_status_t N1MdsysClockManagerEnableClkResource(uint8_t handle_index, n1hwmdsysclockid_t clock);
/**
 * @brief       This function disables mdsys clock resource from modem sub system. \n
 * @param1[in]  handle_index is the mdsys clock handle index, returned from #N1MdsysClockManagerRegister().
 * @param2[in]  clock resource to be disabled.
 * @return      #MDSYS_CLOCK_MANAGER_OK, if mdsys clock resource is disabled.
 * @note        Call #N1MdsysClockManagerRegister() to set up the mdsys clock handle, before calling this function. */
mdsys_clock_manager_status_t N1MdsysClockManagerDisableClkResource(uint8_t handle_index, n1hwmdsysclockid_t clock);
/**
 * @brief       This function enables multiple mdsys clock resources from modem sub system at same time. \n
 * @param1[in]  handle_index is the mdsys clock handle index, returned from #N1MdsysClockManagerRegister().
 * @param2[in]  clock resources to be enabled.
 * @return      #MDSYS_CLOCK_MANAGER_OK, if mdsys clocks resources are enabled.
 * @note        Call #N1MdsysClockManagerRegister() to set up the mdsys clock handle, before calling this function. */
mdsys_clock_manager_status_t N1MdsysClockManagerEnableMultipleClkResources(uint8_t handle_index, uint32_t clocks);
/**
 * @brief       This function disables multiple mdsys clock resources from modem sub system at same time. \n
 * @param1[in]  handle_index is the mdsys clock handle index, returned from #N1MdsysClockManagerRegister().
 * @param2[in]  clock resources to be disabled.
 * @return      #MDSYS_CLOCK_MANAGER_OK, if mdsys clock resources are disabled.
 * @note        Call #N1MdsysClockManagerRegister() to set up the mdsys clock handle, before calling this function. */
mdsys_clock_manager_status_t N1MdsysClockManagerDisableMultipleClkResources(uint8_t handle_index, uint32_t clocks);

/**
 * @brief       This function saves mdsys clock resource configurations to destination memory. \n
 * @param[in]   uint32_t, pointer to destination memory,
 * @return      #MDSYS_CLOCK_MANAGER_OK */
mdsys_clock_manager_status_t N1MdsysClockManagerSaveClkResource(uint32_t *dest);

/**
 * @brief       This function restores mdsys clock resource configurations from source memory. \n
 * @param[in]   uint32_t, pointer to destination memory,
 * @return      #MDSYS_CLOCK_MANAGER_OK */
mdsys_clock_manager_status_t N1MdsysClockManagerRestoreClkResource(uint32_t *src);

/**
* @brief       This function sets clock dividers in ClkConf1 register. Exported for L1dvt usage.
* @return      - */
void N1HwMdsysClockManagerSetModemBusClockDivider(void);

/**
* @brief       This function forces modem power domain on at boot. To be used temporarily
               in FPGA environment until proper power management in implemented. Remove later.
* @return      - */
void N1HwMdsysClockManagerForceModemPowerDomainOnForFpga(void);

#ifdef __cplusplus
}
#endif

#endif /* __MDSYS_CLOCK_MANAGER_H__ */
