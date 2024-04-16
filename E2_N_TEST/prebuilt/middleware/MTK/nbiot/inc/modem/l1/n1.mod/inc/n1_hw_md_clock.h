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
#ifndef __N1_HW_MD_CLOCK__
#define __N1_HW_MD_CLOCK__

/** @file n1_hw_md_clock.h
 * This file contains driver definitions for MDSYS clock module.
 **************************************************************************/

/*******************************************************************************
** Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
** Macros
*******************************************************************************/
#define MAX_MDSYS_CLOCK_HANDLE                32
#define INVALID_MDSYS_CLOCK_HANDLE            0xFF

#ifndef OFF
#define OFF false
#define ON  true
#endif

/* why are these defined as floats? Is this really necessary.
It will make the arithmetic much more costly !*/
#define MD_SLEEP_SLOW_CLOCK          32768.0
#define MD_SLEEP_STIME_CLOCK         1920000.0
#define MD_SLEEP_FAST_CLOCK          26000000.0
#define MD_SLEEP_FAST_CLOCK_DIV      26

#define MD_SLEEP_SLOW_CLOCK_INT      32768
#define MD_SLEEP_STIME_CLOCK_INT     1920000
#define MD_SLEEP_FAST_CLOCK_INT      26000000

 /*****************************************************************************
* Enum
*****************************************************************************/

/** @brief This enum defines the Clock Manager API return status*/
typedef enum {
    MDSYS_CLOCK_MANAGER_ERROR                           = -1,    /**< An undefined error occurred. */
    MDSYS_CLOCK_MANAGER_OK                              = 0,     /**< The operation completed successfully. */
} mdsys_clock_manager_status_t;

/** @brief This enum defines enable/disable mdsys clock register and bits structure */
typedef enum {
MDSYS_MDRGU_HCLK_CLOCK      = 1<<0,  // L1 SW, SW gate control
MDSYS_MDTOPSM_MCLK_CLOCK    = 1<<1,  // L1 SW, SW gate control
MDSYS_PHY_BUS_CK_CLOCK      = 1<<2,  // L1 SW, SW gate control
MDSYS_DBG_CK_CLOCK          = 1<<3,  // L1 SW, SW gate control
MDSYS_MD_PD_BUS_CK_CLOCK    = 1<<16, // L1 SW, SW and HW gate control (power down by md_topsm)
MDSYS_MD_ULS_FC_CLOCK       = 1<<17, // ULS DRV SW, SW and HW gate control (power down by md_topsm)
MDSYS_MD_BSI_BUS_CK_CLOCK   = 1<<18, // RFMAN SW, SW and HW gate control (power down by md_topsm)
MDSYS_MD_BSI_1X_CK_CLOCK    = 1<<19, // RFMAN SW, SW and HW gate control (power down by md_topsm)
MDSYS_MD_BSI_2X_CK_CLOCK    = 1<<20, // RFMAN SW, SW and HW gate control (power down by md_topsm)
MDSYS_MD_BSI_26M_CK_CLOCK   = 1<<21, // RFMAN SW, SW and HW gate control (power down by md_topsm)
MDSYS_MD_BB_HWA_CK_CLOCK    = 1<<24, // L1 SW, SW and HW gate control (powered down by md_topsm), DBG ForceOn
MDSYS_MD_BB_BUS_CK_CLOCK    = 1<<25, // L1 SW, SW and HW gate control (powered down by md_topsm), DBG ForceOn
MDSYS_MD_BB_MEM_CK_CLOCK    = 1<<26, // L1 SW, SW and HW gate control (powered down by md_topsm), DBG ForceOn
MDSYS_MD_BB_F26M_CK_CLOCK   = 1<<28, // L1 SW, SW and HW gate control (powered down by md_topsm)
} n1hwmdsysclockid_t;


/** @brief This enum defines the return status of MDSYS Clock APIs. User should check the return value after calling the APIs. */
typedef enum {
    MDSYS_CLOCK_STATUS_UNINITIALIZED = -3,        /**< Uninitialized clock driver. */
    MDSYS_CLOCK_STATUS_INVALID_PARAMETER = -2,    /**< Invalid parameter. */
    MDSYS_CLOCK_STATUS_ERROR = -1,                /**< Unknown error. */
    MDSYS_CLOCK_STATUS_OK = 0,                    /**< Successful. */
} mdsys_clock_status_t;

typedef struct {
    bool     handleInit; // the handleCount is initalized or not
    uint8_t    handleCount;
    uint32_t   ClockDisable;
} mdsysClockDriverStruct;


/*******************************************************************************
** Local Data
*******************************************************************************/

/*******************************************************************************
** Local Functions
*******************************************************************************/

/*******************************************************************************
** Global Data
*******************************************************************************/
extern mdsysClockDriverStruct mdsysClockDriver;

extern uint32_t mdsysClockHandleArray[MAX_MDSYS_CLOCK_HANDLE];
extern uint32_t mdsysClockHandleClockBit[MAX_MDSYS_CLOCK_HANDLE];

/*******************************************************************************
** Global Functions
*******************************************************************************/

/**
 * @brief       This function initializes the mdsys clock driver and CG before using any Clock API.
 * @return      #MDSYS_CLOCK_STATUS_OK, if the operation completed successfully.\n
 *              #MDSYS_CLOCK_STATUS_UNINITIALIZED, if the clock driver is not initialized.\n
 *              #MDSYS_CLOCK_STATUS_INVALID_PARAMETER, if the input parameter is invalid.\n
 *              #MDSYS_CLOCK_STATUS_ERROR, if the clock function detected a common error.\n
 */
mdsys_clock_status_t N1HwMdsysClockInit(void);

/**
 * @brief       This function enables a specific CG clock.
 * @param1[in]  handle_index
 * @param2[in]  clock_id is a unique clock identifier.
 * @return      #MDSYS_CLOCK_STATUS_OK, if the operation completed successfully.\n
 *              #MDSYS_CLOCK_STATUS_UNINITIALIZED, if the clock driver is not initialized.\n
 *              #MDSYS_CLOCK_STATUS_INVALID_PARAMETER, if the input parameter is invalid.\n
 *              #MDSYS_CLOCK_STATUS_ERROR, if the clock function detected a common error.\n
 */
mdsys_clock_status_t N1HwMdsysClockEnable(uint8_t handle_index, uint32_t clocks);

/**
 * @brief       This function disables a specific CG clock.
 * @param1[in]  handle_index
 * @param2[in]  clock_id is a unique clock identifier.
 *              #MDSYS_CLOCK_STATUS_UNINITIALIZED, if the clock driver is not initialized.\n
 *              #MDSYS_CLOCK_STATUS_INVALID_PARAMETER, if the input parameter is invalid.\n
 *              #MDSYS_CLOCK_STATUS_ERROR, if the clock function detected a common error.\n
 */
mdsys_clock_status_t N1HwMdsysClockDisable(uint8_t handle_index, uint32_t clocks);

/**
 * @brief       This function queries the status of a specific CG clock.
 * @param[in]   clock_id is a unique clock identifier.
 * @return      true, if the specified clock is enabled.\n
 *              false, if the specified clock is disabled or the clock driver is not initialized.\n
 */
bool N1HwMdsysClockEnabled(n1hwmdsysclockid_t clock_id);

/**
 * @brief       This function gets a mdsys clock handler.
 * @param1[in]  Name of the handler.
 * @return      handle index
 */
uint8_t N1HwMdsysClkDrvGetHandle(const char *handle_name);

/**
 * @brief       This function gets a mdsys clock handler.
 * @param1[in]  handle index.
 * @return
 */
void N1HwMdsysClkDrvReleaseHandle(uint8_t handle_index);

/**
 * @brief       This function goes through all handles
 * @param[in]
 * @return      true if some reaources are in use otherwise false.
 */
bool N1HwMdsysClkDrvResStatus(void);

/**
 * @brief       This function sets Mutex.
 * @param[in]
 * @return
 */
bool N1HwMdsysClkDrvGetMutex(void);
/**
 * @brief       This function releases Mutex.
 * @param[in]
 * @return
 */
void N1HwMdsysClkDrvReleaseMutex(void);

/**
 * @brief       This function reads status of the MDSYS CLK
 * @param[in]
 * @return      status of the mdsys Clk (1 = ON; O = OFF )
 */
uint32_t N1HwMdsysClkStatus(void);
/**
 * @brief       This function reads status of the MDSYS CLK CONF1
 * @param[in]
 * @return      return current configuration of MDSYS CLK
 */
uint32_t N1HwMdsysClkConf1Status(void);

/**
* @brief This function writes clk handle index and clock bit tables(uint32_t) to dst memory.
* @param[in]    Destination memory pointer
* @return
*/
void N1HwMdsysClkResTblSave(uint32_t *dst);

/**
* @brief This function writes clk handle index and clock bit tables(uint32_t) from src memory.
* @param[in]    Source memory pointer
* @return
*/
void N1HwMdsysClkResTblRestore(uint32_t *src);

/**
 * @brief       This function checks is clock is request by other module
 * @param1[in]  own handle index
 * @param2[in]  clock to be checked
 * @return      the number of the request
 */
uint8_t N1HwMdsysClkDrvCheckIsUsedByOthers(uint8_t ownHandle, n1hwmdsysclockid_t clock);

#ifdef __cplusplus
}
#endif

#endif /* __N1_HW_MD_CLOCK__ */