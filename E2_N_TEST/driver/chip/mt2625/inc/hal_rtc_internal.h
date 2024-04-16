/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
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

#ifndef _HAL_RTC_INTERNAL_H_
#define _HAL_RTC_INTERNAL_H_

#ifdef HAL_RTC_MODULE_ENABLED
#include "hal_rtc.h"

#define RTC_KEY_BBPU                        (0x4300)
#define RTC_KEY_BBPU_0                      (0xa200)
#define RTC_KEY_BBPU_1                      (0x3900)
#define RTC_KEY_BBPU_2                      (0xda00)
#define RTC_KEY_BBPU_3                      (0xc800)
#define RTC_KEY_BBPU_4                      (0x6400)
#define RTC_KEY_BBPU_5                      (0xee00)
#define RTC_KEY_BBPU_CLEAR_POWERKEY         (0x8700)
#define RTC_KEY_BBPU_RELOAD                 (0x5800)

#define RTC_ALARM_PWREN_OFFSET              (0)
#define RTC_ALARM_PWREN_MASK                (0x1 << RTC_ALARM_PWREN_OFFSET)
/* ALARM or EINT occurred, BBPU[1] = 1*/
#define RTC_RTC_PU_OFFSET                   (1)
#define RTC_RTC_PU_MASK                     (0x1 << RTC_RTC_PU_OFFSET)
/* When an RTC tick occurs, BBPU[2] = 1*/
#define RTC_TICK_PWREN_OFFSET               (2)
#define RTC_TICK_PWREN_MASK                 (0x1 << RTC_TICK_PWREN_OFFSET)
/* When an RTC alarm occurs, BBPU[3] = 1*/
#define RTC_EINT_PWREN_OFFSET               (3)
#define RTC_EINT_PWREN_MASK                 (0x1 << RTC_EINT_PWREN_OFFSET)
/* System is in retention mode or wake up from retention mode.
Write 1 to enter retention mode, and core will power down.*/
#define RTC_RETENTION_MODE_OFFSET           (4)
#define RTC_RETENTION_MODE_MASK             (0x1 << RTC_RETENTION_MODE_OFFSET)
/* After RTC init done, need set BBPU[5] = 1*/
#define RTC_PWRBB_OFFSET                    (5)
#define RTC_PWRBB_MASK                      (0x1 << RTC_PWRBB_OFFSET)

#define RTC_ALSTA_OFFSET                    (0)
#define RTC_ALSTA_MASK                      (0x1 << RTC_ALSTA_OFFSET)
#define RTC_TCSTA_OFFSET                    (1)
#define RTC_TCSTA_MASK                      (0x1 << RTC_TCSTA_OFFSET)
#define RTC_LPSTA_OFFSET                    (2)
#define RTC_LPSTA_MASK                      (0x1 << RTC_LPSTA_OFFSET)
#define RTC_EINTSTA_OFFSET                  (3)
#define RTC_EINTSTA_MASK                    (0x1 << RTC_EINTSTA_OFFSET)

#define RTC_AL_EN_OFFSET                    (8)
#define RTC_AL_EN_MASK                      (0x1 << RTC_AL_EN_OFFSET)
#define RTC_AL_MASK_DOW_OFFSET              (4)
#define RTC_AL_MASK_DOW_MASK                (0x1 << RTC_AL_MASK_DOW_OFFSET)

#define RTC_AP_ALARM_OFFSET                 (15)
#define RTC_AP_ALARM_MASK                   (0x1 << RTC_AP_ALARM_OFFSET)
#define RTC_MD_ALARM_OFFSET                 (14)
#define RTC_MD_ALARM_MASK                   (0x1 << RTC_MD_ALARM_OFFSET)

#define RTC_TC_EN_OFFSET                    (8)
#define RTC_TC_EN_MASK                      (0x1 << RTC_TC_EN_OFFSET)

#define RTC_TC_SECOND_OFFSET                (0)
#define RTC_TC_SECOND_MASK                  (0x3F << RTC_TC_SECOND_OFFSET)
#define RTC_TC_MINUTE_OFFSET                (8)
#define RTC_TC_MINUTE_MASK                  (0x3F << RTC_TC_MINUTE_OFFSET)

#define RTC_TC_HOUR_OFFSET                  (0)
#define RTC_TC_HOUR_MASK                    (0x1F << RTC_TC_HOUR_OFFSET)
#define RTC_TC_DOM_OFFSET                   (8)
#define RTC_TC_DOM_MASK                     (0x1F << RTC_TC_DOM_OFFSET)

#define RTC_TC_DOW_OFFSET                   (0)
#define RTC_TC_DOW_MASK                     (0x7 << RTC_TC_DOW_OFFSET)
#define RTC_TC_MONTH_OFFSET                 (8)
#define RTC_TC_MONTH_MASK                   (0xF << RTC_TC_MONTH_OFFSET)

#define RTC_TC_YEAR_OFFSET                  (0)
#define RTC_TC_YEAR_MASK                    (0x7F << RTC_TC_YEAR_OFFSET)

#define RTC_AL_SECOND_OFFSET                (0)
#define RTC_AL_SECOND_MASK                  (0x3F << RTC_AL_SECOND_OFFSET)
#define RTC_AL_MINUTE_OFFSET                (8)
#define RTC_AL_MINUTE_MASK                  (0x3F << RTC_AL_MINUTE_OFFSET)

#define RTC_AL_HOUR_OFFSET                  (0)
#define RTC_AL_HOUR_MASK                    (0x1F << RTC_AL_HOUR_OFFSET)
#define RTC_AL_DOM_OFFSET                   (8)
#define RTC_AL_DOM_MASK                     (0x1F << RTC_AL_DOM_OFFSET)

#define RTC_AL_DOW_OFFSET                   (0)
#define RTC_AL_DOW_MASK                     (0x7 << RTC_AL_DOW_OFFSET)
#define RTC_AL_MONTH_OFFSET                 (8)
#define RTC_AL_MONTH_MASK                   (0xF << RTC_AL_MONTH_OFFSET)

#define RTC_AL_YEAR_OFFSET                  (0)
#define RTC_AL_YEAR_MASK                    (0x7F << RTC_AL_YEAR_OFFSET)
#define RTC_AL_MS_OFFSET                    (8)
#define RTC_AL_MS_MASK                      (0xF << RTC_AL_MS_OFFSET)

#define RTC_RTC_LPD_OPT_OFFSET              (0)
#define RTC_RTC_LPD_OPT_MASK                (0x1 << RTC_RTC_LPD_OPT_OFFSET)
#define RTC_LPSTA_RAW_OFFSET                (0)
#define RTC_LPSTA_RAW_MASK                  (0x1 << RTC_LPSTA_RAW_OFFSET)
#define RTC_EOSC32_LPEN_OFFSET              (1)
#define RTC_EOSC32_LPEN_MASK                (0x1 << RTC_EOSC32_LPEN_OFFSET)
#define RTC_LPRST_OFFSET                    (3)
#define RTC_LPRST_MASK                      (0x1 << RTC_LPRST_OFFSET)

#define RTC_INT_CNT_OFFSET                  (0)
#define RTC_INT_CNT_MASK                    (0x7fff << RTC_INT_CNT_OFFSET)
#define RTC_CALI_STATUS_OFFSET              (15)
#define RTC_CALI_STATUS_MASK                (0x1 << RTC_CALI_STATUS_OFFSET)
#define RTC_INT_CNT_LIMIT                   (0x7ffd << RTC_INT_CNT_OFFSET)

#define RTC_EINT_EINT_EN_OFFSET             (0)
#define RTC_EINT_EINT_EN_MASK               (0x1 << RTC_EINT_EINT_EN_OFFSET)
#define RTC_ENIT_DEB_OFFSET                 (1)
#define RTC_ENIT_DEB_MASK                   (0x1 << RTC_ENIT_DEB_OFFSET)
#define RTC_EINT_SYNC_EN_OFFSET             (2)
#define RTC_EINT_SYNC_EN_MASK               (0x1 << RTC_EINT_SYNC_EN_OFFSET)
#define RTC_EINT_INV_EN_OFFSET              (3)
#define RTC_EINT_INV_EN_MASK                (0x1 << RTC_EINT_INV_EN_OFFSET)
#define RTC_EINT_EINT_CLR_OFFSET            (4)
#define RTC_EINT_EINT_CLR_MASK              (0x1 << RTC_EINT_EINT_CLR_OFFSET)

#define RTC_TIMER_CG_EN_OFFSET              (0)
#define RTC_TIMER_CG_EN_MASK                (0x1 << RTC_TIMER_CG_EN_OFFSET)

#define RTC_XOSCCALI_OFFSET                 (0)
#define RTC_XOSCCALI_MASK                   (0x1F << RTC_XOSCCALI_OFFSET)
#define RTC_AMPCTL_EN_OFFSET                (5)
#define RTC_AMPCTL_EN_MASK                  (0x1 << RTC_AMPCTL_EN_OFFSET)
#define RTC_AMP_GSEL_OFFSET                 (6)
#define RTC_AMP_GSEL_MASK                   (0x1 << RTC_AMP_GSEL_OFFSET)
#define RTC_EOSC32_STP_PWD_OFFSET           (7)
#define RTC_EOSC32_STP_PWD_MASK             (0x1 << RTC_EOSC32_STP_PWD_OFFSET)
#define RTC_EOSC32_CHOP_EN_OFFSET           (8)
#define RTC_EOSC32_CHOP_EN_MASK             (0x1 << RTC_EOSC32_CHOP_EN_OFFSET)
#define RTC_EOSC32_VCT_EN_OFFSET            (9)
#define RTC_EOSC32_VCT_EN_MASK              (0x1 << RTC_EOSC32_VCT_EN_OFFSET)
#define RTC_EOSC32_RSV_OFFSET               (10)
#define RTC_EOSC32_RSV_MASK                 (0xF << RTC_EOSC32_RSV_OFFSET)

#define RTC_XOSC_PWDB_OFFSET                (0)
#define RTC_XOSC_PWDB_MASK                  (0x1 << RTC_XOSC_PWDB_OFFSET)
#define RTC_EOSC_PWDB_OFFSET                (1)
#define RTC_EOSC_PWDB_MASK                  (0x1 << RTC_EOSC_PWDB_OFFSET)

#define RTC_SRAM_PD_OFFSET                  (0)
#define RTC_SRAM_PD_MASK                    (0xF << RTC_SRAM_PD_OFFSET)
#define RTC_SRAM_SLEEPB_OFFSET              (8)
#define RTC_SRAM_SLEEPB_MASK                (0xF << RTC_SRAM_SLEEPB_OFFSET)

#define RTC_RTC_DIFF_OFFSET                 (0)
#define RTC_RTC_DIFF_MASK                   (0x7FF << RTC_RTC_DIFF_OFFSET)

#define RTC_RTC_CALI_OFFSET                 (0)
#define RTC_RTC_CALI_MASK                   (0x1FFF << RTC_RTC_CALI_OFFSET)
#define RTC_CALI_RW_SEL_OFFSET              (13)
#define RTC_CALI_RW_SEL_MASK                (0x1 << RTC_CALI_RW_SEL_OFFSET)

#define RTC_WRTGR_OFFSET                    (0)
#define RTC_WRTGR_MASK                      (0x1 << RTC_WRTGR_OFFSET)
#define RTC_CBUSY_OFFSET                    (0)
#define RTC_CBUSY_MASK                      (0x1 << RTC_CBUSY_OFFSET)
#define RTC_POWER_DETECTED_OFFSET           (1)
#define RTC_POWER_DETECTED_MASK             (0x1 << RTC_POWER_DETECTED_OFFSET)
#define RTC_STA_RETENTION_MODE_OFFSET       (2)
#define RTC_STA_RETENTION_MODE_MASK         (0x1 << RTC_STA_RETENTION_MODE_OFFSET)

#define RTC_SPAR_INIT_STATUS_OFFSET         (10)
#define RTC_SPAR_INIT_STATUS_MASK           (0x1 << RTC_SPAR_INIT_STATUS_OFFSET)
#define RTC_SPAR_POWERON_REASON_OFFSET      (11)
#define RTC_SPAR_POWERON_REASON_MASK        (0x7 << RTC_SPAR_POWERON_REASON_OFFSET)
#define RTC_SPAR_CG_CONTROL_OFFSET          (7)
#define RTC_SPAR_CG_CONTROL_MASK            (0x1 << RTC_SPAR_CG_CONTROL_OFFSET)

#define RTC_LOW_BYTE_OPERATION              (0)
#define RTC_HIGH_BYTE_OPERATION             (1)
#define RTC_WORD_OPERATION                  (2)

#define RTC_POWERKEY0_KEY                   0xa357
#define RTC_POWERKEY1_KEY                   0x67d2

#define RTC_PROTECT1                        0x586a
#define RTC_PROTECT2                        0x9136

#define RTC_OSC32CON0_MAGIC_KEY_1           0x1a57
#define RTC_OSC32CON0_MAGIC_KEY_2           0x2b68

#define RTC_OSC32CON1_MAGIC_KEY_1           0x1a85
#define RTC_OSC32CON1_MAGIC_KEY_2           0xe7cf

#define RTC_OSC32CON2_MAGIC_KEY_1           0x1653
#define RTC_OSC32CON2_MAGIC_KEY_2           0x8918

#define RTC_SRAMCON_MAGIC_KEY_1             0x5D4A
#define RTC_SRAMCON_MAGIC_KEY_2             0x943F

#define ABIST_FQMTR_BASE                    (0xa2020000 + 0x400)

#define CKSYS_TST_SEL_1_BASE                ((volatile uint32_t *)0xA2020224)
#define RTC_SLOW_SRC_B                      ((volatile uint8_t*)(0xA2010040))

#define RTC_QUARTER_HOUR_ADDR       2
#define DEFAULT_QUARTER_HOUR_VAL    32
#define SYS_OPER_MODE_ADDR          3

#define RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(cONDITION, mASK) \
do{ \
    mASK = save_and_set_interrupt_mask(); \
    while(!(cONDITION)) { \
        restore_interrupt_mask(mASK); \
        mASK = save_and_set_interrupt_mask(); \
    } \
}while(0)    
    
#define RTC_RESTORE_INT(mASK) \
do{ \
    restore_interrupt_mask(mASK); \
}while(0)    

#define rtc_set_sys_oper_mode(mode)

/* frequency meter start */
typedef struct {
    __IO uint32_t ABIST_FQMTR_CON0;     /* Frequency Meter Control Register 0 */
    __IO uint32_t ABIST_FQMTR_CON1;     /* Frequency Meter Control Register 1 */
    __IO uint32_t ABIST_FQMTR_CON2;     /* Frequency Meter Control Register 2 */
    __IO uint32_t ABIST_FQMTR_DATA;     /* Frequency Meter Data */
} ABIST_FQMTR_REGISTER_T;
/* frequency meter end */

/** @brief This enum defines the time zone. */
typedef enum {
    UTC_W11 = -11,    /* 165W */
    UTC_W10 = -10,    /* 150W */
    UTC_W9  = -9,     /* 135W */
    UTC_W8  = -8,     /* 120W */
    UTC_W7  = -7,     /* 105W */
    UTC_W6  = -6,     /* 90W */
    UTC_W5  = -5,     /* 75W */
    UTC_W4  = -4,     /* 60W */
    UTC_W3  = -3,     /* 45W */
    UTC_W2  = -2,     /* 30W */
    UTC_W1  = -1,     /* 15W */
    UTC_0   = 0,      /* 0 */
    UTC_E1  = 1,      /* 15E */
    UTC_E2  = 2,      /* 30E */
    UTC_E3  = 3,      /* 45E */
    UTC_E4  = 4,      /* 60E */
    UTC_E5  = 5,      /* 75E */
    UTC_E6  = 6,      /* 90E */
    UTC_E7  = 7,      /* 105E */
    UTC_E8  = 8,      /* 120E */
    UTC_E9  = 9,      /* 135E */
    UTC_E10 = 10,     /* 150E */
    UTC_E11 = 11,     /* 165E */
    UTC_12  = 12,     /* 180 */
    UTC_MAX = UTC_12,
    UTC_MIN = UTC_W11
} rtc_time_zone_t;

typedef enum {
    RTC_CALI_CTR_LOCK           = 0,         /**< The calibration value of rtc can't be changed. */
    RTC_CALI_CTR_UNLOCK         = 1          /**< The calibration value of rtc can be changed. */
} rtc_cali_ctr_t;

typedef enum {
    DEFAULT_SYS_OPER_MODE       = 0,
    FORCED_SHUT_DOWN_MODE       = 1,
    FORCED_RESET_MODE           = 2
} sys_oper_mode_t;

typedef enum{
    RTC_SYS_NO_SET = 0,     //default status
    RTC_SYS_DS_SM = 1,      //deep sleep, call by sleep management
    RTC_SYS_DS_AT = 2,      //deep sleep, call by AT command
    RTC_SYS_DS_UDF = 3,     //deep sleep, call by undefined func
    RTC_SYS_DP_SM = 4,      //deeper sleep, call by sleep management
    RTC_SYS_DP_FS = 5,      //deeper sleep, call by forced shut down API
    RTC_SYS_DP_UDF = 6,     //deeper sleep, undefined called
    RTC_SYS_WDT_EE = 7,     //WDT sw reset called in exception
    RTC_SYS_WDT_FOTA = 8,   //WDT sw reset called in FOTA
    RTC_SYS_WDT_AT = 9,     //WDT sw reset called in AT+EWDT=1
    RTC_SYS_WDT_FR = 10,    //WDT sw reset called in forced_reset API
    RTC_SYS_WDT_SR = 11,    //WDT sw reset called in system_reboot API
    RTC_SYS_WDT_UDF = 12,   //WDT sw reset called in undefined func
    RTC_SYS_MODE_MAX = 12
}rtc_sys_oper_mode_t;    //SPAR1_1[0:3]

bool rtc_query_cbusy(void);
void rtc_init_phase_2(void);
hal_rtc_status_t rtc_set_time_zone(rtc_time_zone_t time_zone);
hal_rtc_status_t rtc_get_time_zone(rtc_time_zone_t *pTime_zone);

#ifdef HAL_RTC_FEATURE_ELAPSED_TICK
uint32_t rtc_power_on_result();
void rtc_set_gpio_output(uint8_t pin, uint8_t value);
hal_rtc_status_t rtc_get_elapsed_tick_sram(bool is_after_sleep, uint64_t *elapsed_tick);
#endif

/* For internal user request, return frequency meter count with specific test. */
uint32_t f32k_measure_count(uint16_t fixed_clock, uint16_t tested_clock, uint16_t window_setting);

#ifdef HAL_RTC_FEATURE_SW_TIMER

#ifdef __UBL__
#include "bl_common.h"
#define RTC_ASSERT() while(1)
#else
#include <assert.h>
#define RTC_ASSERT() assert(0)
#endif

#define RTC_32K_DEBUG       0

#ifndef __UBL__
#define RTC_LOG_DEBUG       1
#else
#define RTC_LOG_DEBUG       1
#endif

#if RTC_LOG_DEBUG
#define RTC_SW_TIMER_PRINT_LIST_MESG(str)     rtc_sw_timer_print_list_message(str)
#define RTC_SW_ALARM_PRINT_LIST_MESG(str)     rtc_sw_alarm_print_list_message(str)

#ifndef __UBL__
#define RTC_LOG_INFO(_message,...)            log_hal_info(_message, ##__VA_ARGS__)
#define RTC_LOG_WARNING(_message,...)         log_hal_warning(_message, ##__VA_ARGS__)
#define RTC_LOG_ERROR(_message,...)           log_hal_error(_message, ##__VA_ARGS__)
#else
#define RTC_LOG_INFO(fmt,arg...)      bl_print(LOG_DEBUG,fmt,##arg)
#define RTC_LOG_WARNING(fmt,arg...)   bl_print(LOG_DEBUG,fmt,##arg)
#define RTC_LOG_ERROR(fmt,arg...)     bl_print(LOG_DEBUG,fmt,##arg)
#endif
#else
#define RTC_SW_TIMER_PRINT_LIST_MESG(str)
#define RTC_SW_ALARM_PRINT_LIST_MESG(str)

#define RTC_LOG_INFO(_message,...)
#define RTC_LOG_WARNING(_message,...)
#define RTC_LOG_ERROR(_message,...)
#endif

#ifndef __UBL__
#define RTC_LOG_IMPORTANT(_message,...)     log_hal_info(_message, ##__VA_ARGS__)
#else
#define RTC_LOG_IMPORTANT(fmt,arg...)      bl_print(LOG_DEBUG,fmt,##arg)
#endif

/*-----------------------------------------------------------------------------------------------*/


struct list_item_type
{
    struct list_item_type * volatile pxNext;
    struct list_item_type * volatile pxPrevious;
};
typedef struct list_item_type list_item_t;

typedef bool(* list_operation_callback_t)(list_item_t *, list_item_t *, void *);

/*-----------------------------------------------------------------------------------------------*/

/**
*  Note:Rtc sw timers can wake up system; no matter whether the system enters PSM mode, they work normally.
*       They are used for expiration of relative time.
*  - Step 1. Call #rtc_sw_timer_create() to create a rtc sw timer.
*  - Step 2. Call #rtc_sw_timer_start() to start the rtc sw timer.
*  - Step 3. Call #rtc_sw_timer_delete() to delete the rtc sw timer if it will not be used.
*  - sample code:
*    @code
*
*     ATTR_ZIDATA_IN_RETSRAM uint32_t rtc_sw_timer_handle; //This variable must be global
*     rtc_sw_timer_status_t rtc_status;
*     uint32_t time_period_ms = 2000; //means 2000ms(relative time)
*     bool is_periodic = true; //The sw timer is periodic
*     //bool is_periodic = false; //The sw timer is not periodic
*			
*     if(rtc_sw_timer_handle == 0) {
*         rtc_status = rtc_sw_timer_create(&rtc_sw_timer_handle, time_period_ms/100, is_periodic, rtc_sw_timer_callback);
*         if (rtc_status != RTC_SW_TIMER_STATUS_OK) {
*              // Error handler
*         }
*					
*         rtc_status = rtc_sw_timer_start(rtc_sw_timer_handle);
*         if (rtc_status != RTC_SW_TIMER_STATUS_OK) {
*              // Error handler
*         }
*      }
*
*    @endcode
*    @code
*       void rtc_sw_timer_callback(void *user_data)
*       {
*           // Add User-related processing
*
*           // If the rtc sw timer is not periodic, please add the following code:
*               // rtc_sw_timer_delete(rtc_sw_timer_handle);
*               // rtc_sw_timer_handle = 0; //Must be set to zero when the sw timer is deleted
*    }
*    @endcode
*
* @}
* @}
*/

#define RTC_SW_TIMER_NUMBER         7               /* RTC SW Timer maximum supported number*/
#define RTC_SW_TIMER_MAGIC          0x26250000      /* RTC SW Timer magic handle number*/
#define RTC_SW_TIMER_HANDLE_MASK    0xffff          /* RTC SW Timer handle mask section*/
#define RTC_SW_TIMER_MAXIMUM_100MS_TIME    (1342177280)


typedef void(* rtc_sw_timer_callback_t)(void *);

typedef enum {
    RTC_SW_TIMER_STATUS_ERROR_USED        = -4,         /**< All of the RTC SW Timers have beed used. */
    RTC_SW_TIMER_STATUS_ERROR             = -3,         /**< RTC function error occurred. */
    RTC_SW_TIMER_STATUS_ERROR_PORT        = -2,         /**< A wrong RTC SW Timer port is set. */
    RTC_SW_TIMER_STATUS_INVALID_PARAMETER = -1,         /**< An invalid parameter. */
    RTC_SW_TIMER_STATUS_OK   = 0                        /**< No error occurred during the function call.*/
} rtc_sw_timer_status_t;

typedef struct {
    list_item_t                         list_item;
    volatile int32_t                    diff_time;
    volatile uint32_t                   time_out_ms;
    volatile bool                       is_used;
    volatile bool                       is_active;
    volatile bool                       is_periodic;
    volatile rtc_sw_timer_callback_t    sw_timer_callback;
} rtc_sw_timer_t;

uint64_t rtc_sw_timer_time_to_ms(hal_rtc_time_t *rtc_time);
void rtc_sw_timer_update_basic_time(const char *str, hal_rtc_time_t *pCur_time);
rtc_sw_timer_status_t rtc_sw_timer_set_expiration_time(const char *str);


void rtc_sw_timer_isr(void);

/**
 * @brief This function creates a rtc sw timer; When a sw timer is created, it is inactive.
 * @param[out] handle is a pointer to a variable which is used to store the rtc sw timer handle.
 * @param[in] time_out_100ms is the relative expiration time; is_periodic represents whether the sw timer is periodic; 
 *            callback is a pointer to the user's callback function.
 * @return #RTC_SW_TIMER_STATUS_OK, if the operation completed successfully.
 */
rtc_sw_timer_status_t rtc_sw_timer_create(uint32_t *handle,
        const uint32_t time_out_100ms, // unit is 100ms, and 0 < time_out_100ms < 1342177280
        const bool is_periodic, rtc_sw_timer_callback_t callback);

/**
 * @brief This function starts a inactive rtc sw timer.
 * @param[in] handle is a rtc sw timer handle
 * @return #RTC_SW_TIMER_STATUS_OK, if the operation completed successfully.
 */
rtc_sw_timer_status_t rtc_sw_timer_start(uint32_t handle);

/**
 * @brief This function stops a active rtc sw timer.
 * @param[in] handle is a rtc sw timer handle
 * @return #RTC_SW_TIMER_STATUS_OK, if the operation completed successfully.
 */
rtc_sw_timer_status_t rtc_sw_timer_stop(uint32_t handle);

/**
 * @brief This function deletes a rtc sw timer, no matter it is active or not.
 * @param[in] handle is a rtc sw timer handle
 * @return #RTC_SW_TIMER_STATUS_OK, if the operation completed successfully.
 */
rtc_sw_timer_status_t rtc_sw_timer_delete(uint32_t handle);

/**
 * @brief This function gets the remaining time of a active rtc sw timer(Note: the unit is 62.5ms).
 * @param[in] handle is a rtc sw timer handle
 * @param[out] remaining_time is a pointer to a variable which is used to store the remaining time of a active rtc sw timer and the unit is 62.5ms.
 * @return #RTC_SW_TIMER_STATUS_OK, if the operation completed successfully.
 */
rtc_sw_timer_status_t rtc_sw_timer_get_remaining_time_ms(uint32_t handle, uint32_t *remaining_time);

/**
 * @brief This function gets the status of whether the rtc sw timer is active.
 * @param[in] handle is a rtc sw timer handle
 * @param[out] is_active is a pointer to a variable which is used to store the active status of a rtc sw timer.
 * @return #RTC_SW_TIMER_STATUS_OK, if the operation completed successfully.
 */
rtc_sw_timer_status_t rtc_sw_timer_is_timer_active(uint32_t handle, bool *is_active);


/*-----------------------------------------------------------------------------------------------*/

/**
*  Note:Rtc sw alarms can wake up system; no matter whether the system enters PSM mode, they work normally;
*       All rtc sw alarms are one-shot(not periodic) and they are used for expiration of absolute time.
*  - Step 1. Call #rtc_sw_alarm_create() to create a rtc sw alarm.
*  - Step 2. Call #rtc_sw_alarm_start() to start the rtc sw alarm.
*  - Step 3. Call #rtc_sw_alarm_delete() to delete the rtc sw alarm if it will not be used.
*  - sample code:
*    @code
*
*     #define BASE_YEAR 2000
*     ATTR_ZIDATA_IN_RETSRAM uint32_t rtc_sw_alarm_handle; //This variable must be global
*     rtc_sw_alarm_status_t rtc_status;
*     hal_rtc_time_t alarm_time; // It's a absolute time.
*     uint32_t user_data = xxx; //Or struct xxx(struct_name) user_data;
*     
*     //Set alarm_time to the expected absolute expiration time
*     alarm_time.rtc_sec = second;
*     alarm_time.rtc_min = minute;
*     alarm_time.rtc_hour = hour;
*           ..........       
*     alarm_time.rtc_year = year - BASE_YEAR; // The user has to define the base year and the RTC year is defined as an offset.
*                                             //For example, define the base year as 2000 and assign 18 to the RTC year to represent the year of 2018.
*			
*     if(rtc_sw_alarm_handle == 0) {
*         rtc_status = rtc_sw_alarm_create(&rtc_sw_alarm_handle);
*         if (rtc_status != RTC_SW_ALARM_STATUS_OK) {
*              // Error handler
*         }
*					
*         rtc_status = rtc_sw_alarm_start(rtc_sw_alarm_handle, alarm_time, rtc_sw_alarm_callback, (void *)(&user_data));
*         if (rtc_status != RTC_SW_ALARM_STATUS_OK) {
*              // Error handler
*         }
*      }
*
*    @endcode
*    @code
*       void rtc_sw_alarm_callback(void *user_data)
*       {
*           // Add User-related processing
*
*           // Case 1: if the rtc sw alarm is not used, please add the following code:
*                    // rtc_sw_alarm_delete(rtc_sw_alarm_handle);
*                    // rtc_sw_alarm_handle = 0; //Must be set to zero when the sw alarm is deleted
*
*           // Case 2: if the rtc sw alarm is used for another expiration of absolute time, please refer to the following code:
*                    // hal_rtc_time_t alarm_time;
*                    // rtc_sw_alarm_status_t rtc_status;
*            
*                    //Set alarm_time to the expected absolute expiration time
*                       // alarm_time.rtc_sec = second;
*                               ..........
*            
*                    // rtc_status = rtc_sw_alarm_start(rtc_sw_alarm_handle, alarm_time, rtc_sw_alarm_callback, (void *)(&user_data));
*                    // if (rtc_status != RTC_SW_ALARM_STATUS_OK) {
*                            // Error handler
*                    // }
*        }
*    @endcode
*
* @}
* @}
*/

#define RTC_SW_ALARM_NUMBER         5
#define RTC_SW_ALARM_MAGIC          0x26250000
#define RTC_SW_ALARM_HANDLE_MASK    0xffff

typedef void(* rtc_sw_alarm_callback_t)(void *);

typedef enum {
    RTC_SW_ALARM_STATUS_ERROR_USED        = -4,         /**< All of the RTC SW Alarms have beed used. */
    RTC_SW_ALARM_STATUS_ERROR             = -3,         /**< RTC function error occurred. */
    RTC_SW_ALARM_STATUS_ERROR_PORT        = -2,         /**< A wrong RTC SW Alarm port is set. */
    RTC_SW_ALARM_STATUS_INVALID_PARAMETER = -1,         /**< An invalid parameter. */
    RTC_SW_ALARM_STATUS_OK   = 0                        /**< No error occurred during the function call.*/
} rtc_sw_alarm_status_t;

typedef struct {
    volatile rtc_sw_alarm_callback_t callback;
    void * volatile user_data;
} rtc_sw_alarm_callback_context_t;


typedef struct {
    list_item_t                                 list_item;
    volatile hal_rtc_time_t                     alarm_time;
    volatile uint64_t                           alarm_time_value;
    volatile bool                               is_used;
    volatile bool                               is_active;
    rtc_sw_alarm_callback_context_t             callback_context;
} rtc_sw_alarm_t;

rtc_sw_alarm_status_t rtc_sw_alarm_set_expiration_time(const char *str);


void rtc_sw_alarm_isr(void);

/**
 * @brief This function creates a rtc sw alarm; When a sw alarm is created, it is inactive.
 * @param[out] handle is a pointer to a variable which is used to store the rtc sw alarm handle.
 * @return #RTC_SW_ALARM_STATUS_OK, if the operation completed successfully.
 */
rtc_sw_alarm_status_t rtc_sw_alarm_create(uint32_t *handle);

/**
 * @brief This function starts a inactive rtc sw alarm.
 * @param[in] handle is a rtc sw alarm handle; alarm_time is the absolute expiration time;
 *            callback is a pointer to the user's callback function; user_data is a pointer to the user-useful data.
 * @return #RTC_SW_ALARM_STATUS_OK, if the operation completed successfully.
 */
rtc_sw_alarm_status_t rtc_sw_alarm_start(uint32_t handle, hal_rtc_time_t alarm_time, 
                                            rtc_sw_alarm_callback_t callback, void *user_data);

/**
 * @brief This function stops a active rtc sw alarm.
 * @param[in] handle is a rtc sw alarm handle
 * @return #RTC_SW_ALARM_STATUS_OK, if the operation completed successfully.
 */
rtc_sw_alarm_status_t rtc_sw_alarm_stop(uint32_t handle);

/**
 * @brief This function deletes a rtc sw alarm, no matter it is active or not.
 * @param[in] handle is a rtc sw alarm handle
 * @return #RTC_SW_ALARM_STATUS_OK, if the operation completed successfully.
 */
rtc_sw_alarm_status_t rtc_sw_alarm_delete(uint32_t handle);

/**
 * @brief This function gets the information(active status and absolute expiration time) of a rtc sw alarm.
 * @param[in] handle is a rtc sw alarm handle
 * @param[out] is_active is a pointer to a variable which is used to store the active status of a rtc sw alarm;
 *             pAlarm_time is a pointer to a variable which is used to store the absolute expiration time of a rtc sw alarm;
 * @return #RTC_SW_ALARM_STATUS_OK, if the operation completed successfully.
 */
rtc_sw_alarm_status_t rtc_sw_alarm_query_info(uint32_t handle, bool *is_active, hal_rtc_time_t *pAlarm_time);

/**
 * @brief This function gets the remaining time of a active rtc sw alarm(Note: the unit is 62.5ms).
 * @param[in] handle is a rtc sw alarm handle
 * @param[out] remaining_time is a pointer to a variable which is used to store the remaining time of a active rtc sw alarm and the unit is 62.5ms.
 * @return #RTC_SW_ALARM_STATUS_OK, if the operation completed successfully.
 */
rtc_sw_alarm_status_t rtc_sw_alarm_get_remaining_time_ms(uint32_t handle, uint32_t *remaining_time);

/*-----------------------------------------------------------------------------------------------*/

typedef struct {
    rtc_sw_timer_t              sw_timer[RTC_SW_TIMER_NUMBER];
    rtc_sw_alarm_t              sw_alarm[RTC_SW_ALARM_NUMBER];
    volatile hal_rtc_time_t     sw_timer_ap_alarm_time;
    volatile hal_rtc_time_t     sw_alarm_md_alarm_time;
    volatile uint8_t            used_timer_count;
    volatile uint8_t            used_alarm_count;
    list_item_t * volatile      pTimer_list_head;
    list_item_t * volatile      pAlarm_list_head;
    volatile hal_rtc_time_t     sw_timer_basis_time;
    volatile uint64_t           sw_timer_basis_time_value;
} rtc_sw_context_t;

extern rtc_sw_context_t rtc_sw_context;

hal_rtc_status_t rtc_sw_get_remaining_time(uint32_t *remaining_time);

/*-----------------------------------------------------------------------------------------------*/

hal_rtc_status_t rtc_set_alarm(const hal_rtc_time_t *time);
hal_rtc_status_t rtc_set_md_alarm(const hal_rtc_time_t *time);
void rtc_enable_alarm(void);
void rtc_enable_md_alarm(void);
uint32_t rtc_calendar_time_to_seconds(hal_rtc_time_t *rtc_time);
void rtc_forward_time(hal_rtc_time_t *time, uint32_t second);
void rtc_write_trigger(void);
void rtc_wait_busy(void);
bool rtc_is_time_valid(const hal_rtc_time_t *time);

#endif


#endif /* #ifdef HAL_RTC_MODULE_ENABLED */
#endif /* #ifndef _HAL_RTC_INTERNAL_H_ */
