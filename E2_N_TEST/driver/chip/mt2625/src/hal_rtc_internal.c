#include "hal_rtc.h"

#if defined(HAL_RTC_MODULE_ENABLED)

#ifdef HAL_RTC_FEATURE_SW_TIMER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hal_eint.h"
#include "hal_log.h"
#include "hal_gpt.h"
#include "hal_platform.h"
#include "hal_pmu.h"
#include "hal_nvic.h"
#include "hal_nvic_internal.h"
#include "hal_rtc_internal.h"
#include "memory_attribute.h"

#define LIST_IS_EMPTY(pList) ((bool)((pList) ==  NULL))
#define SET_ALARM_OVERHEAD  3

#define portNVIC_INT_CTRL_REG   ( * ( ( volatile uint32_t * ) 0xe000ed04 ) )
#define IS_IN_ISR ((bool)((portNVIC_INT_CTRL_REG & 0xFFL) != 0))


ATTR_ZIDATA_IN_RETSRAM rtc_sw_context_t rtc_sw_context;


/*-----------------------------------------------------------------------------------------------*/

ATTR_TEXT_IN_RAM void list_insert(list_item_t ** pList_head, list_item_t * pNew_list_item, 
                    list_operation_callback_t list_compare_item_callback)
{
    list_item_t * pPre_list_item = NULL;
    list_item_t * pList_item;

    if(pList_head == NULL || pNew_list_item == NULL || list_compare_item_callback == NULL) {
        RTC_ASSERT();
    }
    pNew_list_item->pxPrevious = pNew_list_item->pxNext = NULL;
    
    if(LIST_IS_EMPTY((*pList_head)) == true) {
        *pList_head = pNew_list_item;
    } else {
        pList_item = *pList_head;
        
        while(list_compare_item_callback(pNew_list_item, pList_item, NULL) != true) {
                pPre_list_item = pList_item;
                pList_item = pList_item->pxNext;
                
            if(pList_item == NULL) {
                break;
            } 
        }
        
        if(pList_item != NULL) {
            pList_item->pxPrevious = pNew_list_item;
            pNew_list_item->pxNext = pList_item;
        }

        if(pPre_list_item != NULL) {
            pNew_list_item->pxPrevious = pPre_list_item;
            pPre_list_item->pxNext = pNew_list_item;
        } else {
            *pList_head = pNew_list_item;
        }
    }
}

ATTR_TEXT_IN_RAM void list_remove(list_item_t ** pList_head, list_item_t * pItem_to_remove, 
                        list_operation_callback_t list_remove_item_callback)
{
    if(pList_head == NULL || pItem_to_remove == NULL) {
        RTC_ASSERT();
    }

    if(LIST_IS_EMPTY((*pList_head)) == true) {
        RTC_ASSERT();
    }
        
    if(list_remove_item_callback != NULL) {
        list_remove_item_callback(pItem_to_remove, NULL, NULL);
    }
    
    if((*pList_head) == pItem_to_remove) {
        *pList_head = pItem_to_remove->pxNext;
    }

    if(pItem_to_remove->pxPrevious != NULL) {
        pItem_to_remove->pxPrevious->pxNext = pItem_to_remove->pxNext;
    }

    if(pItem_to_remove->pxNext != NULL) {
        pItem_to_remove->pxNext->pxPrevious= pItem_to_remove->pxPrevious;
    }

    pItem_to_remove->pxPrevious = pItem_to_remove->pxNext = NULL;
}

ATTR_TEXT_IN_RAM bool list_remove_first_smaller(list_item_t ** pList_head, list_item_t * pCompared_item, 
                                    list_operation_callback_t list_compare_item_callback,
                                        list_operation_callback_t list_remove_item_callback,
                                            list_operation_callback_t list_deal_first_smaller_callback,
                                                void *user_data)
{
    list_item_t *plist_item = NULL;
    
    if(pList_head == NULL || list_compare_item_callback == NULL || pCompared_item == NULL
        || list_deal_first_smaller_callback == NULL) {
        RTC_ASSERT();
        return false;
    }
    
    if(LIST_IS_EMPTY((*pList_head)) == true) {
        return false;
    }
    plist_item = *pList_head;

    if(list_compare_item_callback(pCompared_item, plist_item, NULL) != true) {
        list_remove(pList_head, plist_item, list_remove_item_callback);
        list_deal_first_smaller_callback(plist_item, NULL, user_data);
        
        return true;
    } else {
        return false;
    }
    
}
#if 0
ATTR_TEXT_IN_RAM void list_remove_all_smaller(list_item_t ** pList_head, list_item_t * pCompared_item, 
                                    list_operation_callback_t list_compare_item_callback,
                                        list_operation_callback_t list_remove_smaller_items_callback,
                                            void *user_data)
{
    list_item_t *pStart_list_item = NULL;
    list_item_t *pEnd_list_item = NULL;
    
    if(pList_head == NULL || pCompared_item == NULL || list_compare_item_callback == NULL) {
        RTC_ASSERT();
    }
    
    if(LIST_IS_EMPTY((*pList_head)) == true) {
        return;
    }
    pStart_list_item = pEnd_list_item = *pList_head;

    while((pEnd_list_item != NULL) && (list_compare_item_callback(pCompared_item, pEnd_list_item, NULL) != true)) {
            pEnd_list_item = pEnd_list_item->pxNext;
    }
    *pList_head = pEnd_list_item;

    if(pEnd_list_item != NULL) {
        pEnd_list_item->pxPrevious = NULL;
    }
    list_remove_smaller_items_callback(pStart_list_item, pEnd_list_item, user_data);
}
#endif
ATTR_TEXT_IN_RAM bool list_search_item(list_item_t ** pList_head, list_item_t * pItem_to_search, 
                                    list_operation_callback_t list_search_item_callback, void *user_data)
{
    list_item_t * pList_item;
        
    if((pList_head == NULL) || (pItem_to_search == NULL)) {
        RTC_ASSERT();
        return false;
    }

    if(LIST_IS_EMPTY((*pList_head)) == true) {
        RTC_ASSERT();
        return false;
    }

    pList_item = *pList_head;

    while((pList_item != NULL) && (pList_item != pItem_to_search)) {
        
        if(list_search_item_callback != NULL) {
            list_search_item_callback(pList_item, NULL, user_data);
        }
        pList_item = pList_item->pxNext;
    }

    if((pList_item != NULL) && (pList_item == pItem_to_search)) {
        
        if(list_search_item_callback != NULL) {
            list_search_item_callback(pList_item, NULL, user_data);
        }
        return true;
    } else {
        return false;
    }
}

/*-----------------------------------------------------------------------------------------------*/

static void rtc_print_time(const char * str, hal_rtc_time_t * pTime)
{
    RTC_LOG_INFO("%s : %d/%d/%d %d:%d:%d(%d)\r\n", str, pTime->rtc_year, pTime->rtc_mon, pTime->rtc_day,
                     pTime->rtc_hour, pTime->rtc_min, pTime->rtc_sec, pTime->rtc_milli_sec);
}

static void rtc_sw_timer_print_list_message(const char * str)
{
    list_item_t *pList_item;
    rtc_sw_timer_t *pTimer;
    uint32_t index = 0;
    
    RTC_LOG_INFO("%s\r\n", str);
    pList_item = rtc_sw_context.pTimer_list_head;
    
    while(pList_item != NULL) {
        pTimer = (rtc_sw_timer_t *)pList_item;

        RTC_LOG_INFO("  index = %d, diff_time = %d, time_out_ms = %d\r\n", index, pTimer->diff_time, pTimer->time_out_ms);
        RTC_LOG_INFO("      is_used = %d, is_active = %d\r\n", pTimer->is_used, pTimer->is_active);
        RTC_LOG_INFO("      is_periodic = %d, sw_timer_callback = 0x%08lx\r\n", pTimer->is_periodic, pTimer->sw_timer_callback);

        pList_item = pList_item->pxNext;
        index++;
    }
}

static void rtc_sw_alarm_print_list_message(const char * str)
{
    list_item_t *pList_item;
    rtc_sw_alarm_t *pAlarm;
    uint32_t index = 0;
    
    RTC_LOG_INFO("%s\r\n", str);
    pList_item = rtc_sw_context.pAlarm_list_head;
    
    while(pList_item != NULL) {
        pAlarm = (rtc_sw_alarm_t *)pList_item;

        RTC_LOG_INFO("  index = %d, alarm_time_value_h = %d, alarm_time_value_l = %d\r\n", index, (uint32_t)(pAlarm->alarm_time_value >> 32), (uint32_t)(pAlarm->alarm_time_value & 0xffffffff));
        RTC_LOG_INFO("      is_used = %d, is_active = %d\r\n", pAlarm->is_used, pAlarm->is_active);
        RTC_LOG_INFO("      callback = 0x%08lx, user_data = 0x%08lx\r\n", pAlarm->callback_context.callback, pAlarm->callback_context.user_data);

        pList_item = pList_item->pxNext;
        index++;
    }
}


/*-----------------------------------------------------------------------------------------------*/

static uint32_t rtc_sw_get_free_timer_or_alarm(bool is_get_timer)
{
    uint32_t index;

    if(is_get_timer == true) {
        for (index = 0; index < RTC_SW_TIMER_NUMBER; index++) {
            if (rtc_sw_context.sw_timer[index].is_used != true) {
                return index;
            }
        }
        return RTC_SW_TIMER_NUMBER;
    } else {
        for (index = 0; index < RTC_SW_ALARM_NUMBER; index++) {
            if (rtc_sw_context.sw_alarm[index].is_used != true) {
                return index;
            }
        }
        return RTC_SW_ALARM_NUMBER;
    }
}

ATTR_TEXT_IN_TCM uint64_t rtc_sw_timer_time_to_ms(hal_rtc_time_t *rtc_time)
{
    uint64_t result;
    result = (uint64_t)rtc_calendar_time_to_seconds(rtc_time);

    /* multiple of 62.5ms*/
    result *= 16;
    result += rtc_time->rtc_milli_sec;

    return result;
}

ATTR_TEXT_IN_TCM void rtc_sw_timer_forward_time(hal_rtc_time_t *time, uint32_t forward_time_ms)
{
    /* forward_time_ms keeps as the multiple of 62.5ms */
    uint32_t second, mili_second;

    mili_second = forward_time_ms & 0xf;
    second = forward_time_ms >> 4;

    /* multiple of 62.5ms */
    mili_second += time->rtc_milli_sec;
    if (mili_second > 15) {
        second++;
        mili_second -= 16;
    }
    time->rtc_milli_sec = mili_second;

    rtc_forward_time(time, second);
}

ATTR_TEXT_IN_RAM static bool rtc_sw_timer_compare_item_callback(list_item_t *pCompared_item, list_item_t *pList_item, void *user_data_null)
{
    rtc_sw_timer_t *pCompared_timer, *pTimer;

    if(pCompared_item == NULL || pList_item == NULL) {
        RTC_ASSERT();
    }

    pCompared_timer = (rtc_sw_timer_t *)pCompared_item;
    pTimer = (rtc_sw_timer_t *)pList_item;
    
    if(pCompared_timer->diff_time < pTimer->diff_time) {
        pTimer->diff_time -= pCompared_timer->diff_time;
        return true;
    } else {
        pCompared_timer->diff_time -= pTimer->diff_time;
        return false;
    }
}

ATTR_TEXT_IN_RAM static bool rtc_sw_timer_remove_item_callback(list_item_t * pItem_to_remove, list_item_t *pList_null, void *user_data_null)
{
    rtc_sw_timer_t *pNext_timer, *pTimer_to_remove;

    if(pItem_to_remove == NULL) {
        RTC_ASSERT();
        return false;
    }

    if(pItem_to_remove->pxNext != NULL) {
        pTimer_to_remove        = (rtc_sw_timer_t *)pItem_to_remove;
        pNext_timer             = (rtc_sw_timer_t *)(pItem_to_remove->pxNext);
        pNext_timer->diff_time += pTimer_to_remove->diff_time;
    }

    return true;
}
#if 0
ATTR_TEXT_IN_RAM static bool rtc_sw_timer_remove_smaller_items_callback(list_item_t *pStart_list_item, 
                                                                    list_item_t *pEnd_list_item,
                                                                        void *user_data)
{
    rtc_sw_timer_t *pTimer;
    rtc_sw_timer_callback_t *sw_timer_callback;
    uint32_t index = 0;
    int32_t sum_diff_time = 0;

    if((pStart_list_item == NULL) && (pEnd_list_item != NULL)) {
        RTC_ASSERT();
        return false;
    }
    sw_timer_callback = (rtc_sw_timer_callback_t *)user_data;
    
    while(pStart_list_item != pEnd_list_item) {
        pTimer = (rtc_sw_timer_t *)pStart_list_item;
        pStart_list_item = pStart_list_item->pxNext;
        sum_diff_time += pTimer->diff_time;
        sw_timer_callback[index] = pTimer->sw_timer_callback;
        index++;
        
        if(pTimer->is_periodic == true) {
            pTimer->diff_time = sum_diff_time + (int32_t)(pTimer->time_out_ms);
            pTimer->is_active = true;
            list_insert(&(rtc_sw_context.pTimer_list_head), (list_item_t *)pTimer, 
                            rtc_sw_timer_compare_item_callback);
            RTC_SW_TIMER_PRINT_LIST_MESG("[timer isr]list auto re-insert periodic item");
        } else {
            pTimer->is_active = false;
        }
    }

    return true;
}
#endif

ATTR_TEXT_IN_RAM static bool rtc_sw_timer_deal_first_smaller_callback(list_item_t *plist_item, 
                                                                                list_item_t *plist_null,
                                                                                    void *user_data)
{
    rtc_sw_timer_t *pTimer;
    rtc_sw_timer_callback_t *pSw_timer_callback;

    if(plist_item == NULL || user_data == NULL) {
        RTC_ASSERT();
        return false;
    }
    pTimer = (rtc_sw_timer_t *)plist_item;
    pSw_timer_callback = (rtc_sw_timer_callback_t *)user_data;
    *pSw_timer_callback = pTimer->sw_timer_callback;

    if(pTimer->is_periodic == true) {
        pTimer->diff_time += (int32_t)(pTimer->time_out_ms);
        pTimer->is_active = true;
        list_insert(&(rtc_sw_context.pTimer_list_head), (list_item_t *)pTimer, 
                        rtc_sw_timer_compare_item_callback);
        RTC_SW_TIMER_PRINT_LIST_MESG("[timer isr]list auto re-insert periodic item");
    } else {
        pTimer->is_active = false;
    }

    return true;
}

ATTR_TEXT_IN_RAM static bool rtc_sw_timer_search_item_callback(list_item_t *pList_item, list_item_t *pList_null, void *user_data)
{
    int32_t *pSum_diff_time;
    rtc_sw_timer_t *pTimer;

    if(pList_item == NULL || user_data == NULL) {
        RTC_ASSERT();
        return false;
    }

    pSum_diff_time = (int32_t *)user_data;
    pTimer = (rtc_sw_timer_t *)pList_item;
    *pSum_diff_time += pTimer->diff_time;

    return true;
}


ATTR_TEXT_IN_RAM void rtc_sw_timer_update_basic_time(const char *str, hal_rtc_time_t *pCur_time)
{
    hal_rtc_time_t cur_time;
    uint64_t cur_time_value;
    rtc_sw_timer_t * pHead_timer;

    if(pCur_time == NULL) {
        hal_rtc_get_time(&cur_time);
        cur_time.rtc_milli_sec = (cur_time.rtc_milli_sec & RTC_INT_CNT_MASK) >> 11;
        pCur_time = &cur_time;
    }
    rtc_print_time(str, pCur_time);
    cur_time_value = rtc_sw_timer_time_to_ms(pCur_time);

    if(LIST_IS_EMPTY(rtc_sw_context.pTimer_list_head) != true) {
        pHead_timer = (rtc_sw_timer_t *)(rtc_sw_context.pTimer_list_head);
        pHead_timer->diff_time = rtc_sw_context.sw_timer_basis_time_value 
                                    + pHead_timer->diff_time - cur_time_value;
        if(pHead_timer->diff_time < -32) {
            RTC_ASSERT();        
        }
    }
    rtc_sw_context.sw_timer_basis_time = *pCur_time;
    rtc_sw_context.sw_timer_basis_time_value = cur_time_value;
}

ATTR_TEXT_IN_RAM rtc_sw_timer_status_t rtc_sw_timer_set_expiration_time(const char *str)
{
    uint32_t forward_time;
    hal_rtc_time_t ap_alarm_time;
    rtc_sw_timer_t * pHead_timer;

    if(LIST_IS_EMPTY(rtc_sw_context.pTimer_list_head) == true) {
        return RTC_SW_TIMER_STATUS_ERROR;
    }
    pHead_timer = (rtc_sw_timer_t *)(rtc_sw_context.pTimer_list_head);

    if(pHead_timer->diff_time <= SET_ALARM_OVERHEAD) {
        forward_time = SET_ALARM_OVERHEAD;
    } else {
        forward_time = pHead_timer->diff_time;
    }
    
    ap_alarm_time = rtc_sw_context.sw_timer_basis_time;
    rtc_sw_timer_forward_time(&ap_alarm_time, forward_time);
    rtc_sw_context.sw_timer_ap_alarm_time = ap_alarm_time;
    rtc_print_time(str, &ap_alarm_time);

    if(HAL_RTC_STATUS_OK != rtc_set_alarm(&ap_alarm_time)) {
        RTC_ASSERT();
        return RTC_SW_TIMER_STATUS_INVALID_PARAMETER;
    }
    rtc_enable_alarm();
    rtc_write_trigger();

    return RTC_SW_TIMER_STATUS_OK;
}

/*-----------------------------------------------------------------------------------------------*/

rtc_sw_timer_status_t rtc_sw_timer_create(uint32_t *handle, const uint32_t time_out_100ms,
                                        const bool is_periodic, rtc_sw_timer_callback_t callback)
{
    uint32_t index, mask;
    uint64_t time_out_62ms;
    
    if ((handle == NULL) || ((time_out_100ms == 0) || (time_out_100ms >= RTC_SW_TIMER_MAXIMUM_100MS_TIME))) {
        RTC_LOG_ERROR("Error:[timer create]handle = %x, time_out_100ms = %d, callback = %x\r\n", 
                                            handle, time_out_100ms, callback);
        return RTC_SW_TIMER_STATUS_INVALID_PARAMETER;
    }

    mask = save_and_set_interrupt_mask();
    if (rtc_sw_context.used_timer_count >= RTC_SW_TIMER_NUMBER) {
        restore_interrupt_mask(mask);
        RTC_LOG_ERROR("Error:[timer create]used_timer_count = %d\r\n", rtc_sw_context.used_timer_count);
        return RTC_SW_TIMER_STATUS_ERROR_USED;
    }

    index = rtc_sw_get_free_timer_or_alarm(true);
    *handle = index | RTC_SW_TIMER_MAGIC;
    RTC_LOG_INFO("[timer create]handle = %x, *handle = %x\r\n", handle, (*handle));

    if ((((uint64_t)time_out_100ms)*100*2)%125) {
        time_out_62ms = ((uint64_t)time_out_100ms)*100*2/125 + 1;
    } else {
        time_out_62ms = ((uint64_t)time_out_100ms)*100*2/125;
    }
    RTC_LOG_INFO("[timer create]time_out_100ms = %d, time_out_62ms (62.5) = %ld\r\n", time_out_100ms, time_out_62ms);

    rtc_sw_context.sw_timer[index].is_used = true;
    rtc_sw_context.sw_timer[index].is_active = false;
    rtc_sw_context.sw_timer[index].sw_timer_callback = callback;
    rtc_sw_context.sw_timer[index].is_periodic = is_periodic;
    rtc_sw_context.sw_timer[index].time_out_ms = (uint32_t)time_out_62ms;
    rtc_sw_context.used_timer_count++;
    restore_interrupt_mask(mask);

    return RTC_SW_TIMER_STATUS_OK;
}

ATTR_TEXT_IN_RAM rtc_sw_timer_status_t rtc_sw_timer_start(uint32_t handle)
{
    uint32_t mask, index;
    rtc_sw_timer_status_t result;
    
    if ((handle & RTC_SW_TIMER_MAGIC) != RTC_SW_TIMER_MAGIC) {
        RTC_LOG_ERROR("Error:[timer start]handle = %x\r\n", handle);
        return RTC_SW_TIMER_STATUS_INVALID_PARAMETER;
    }

    index = handle & RTC_SW_TIMER_HANDLE_MASK;
    RTC_LOG_INFO("[timer start]index = %d\r\n", index);

    if ((rtc_sw_context.sw_timer[index].is_used != true) ||
        (rtc_sw_context.sw_timer[index].is_active == true)) {
        RTC_LOG_ERROR("Error:[timer start]is_used = %d, is_active = %d\r\n", 
                        rtc_sw_context.sw_timer[index].is_used, 
                            rtc_sw_context.sw_timer[index].is_active);
        return RTC_SW_TIMER_STATUS_ERROR;
    }
    
    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    rtc_sw_timer_update_basic_time("[timer start]update basic time", NULL);
    RTC_RESTORE_INT(mask);
    
    mask = save_and_set_interrupt_mask();
    rtc_sw_context.sw_timer[index].diff_time = (int32_t)(rtc_sw_context.sw_timer[index].time_out_ms);
    rtc_sw_context.sw_timer[index].is_active = true;
    list_insert(&(rtc_sw_context.pTimer_list_head), (list_item_t *)(&(rtc_sw_context.sw_timer[index])), 
                    rtc_sw_timer_compare_item_callback);
    restore_interrupt_mask(mask);
    RTC_SW_TIMER_PRINT_LIST_MESG("[timer start]list insert");
    
    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    result = rtc_sw_timer_set_expiration_time("[timer start]set sw timer expiration time");
    RTC_RESTORE_INT(mask);
    
    if(result == RTC_SW_TIMER_STATUS_OK) {
        rtc_wait_busy();
    }

    return result;
}

ATTR_TEXT_IN_RAM rtc_sw_timer_status_t rtc_sw_timer_stop(uint32_t handle)
{
    uint32_t mask, index;
    rtc_sw_timer_status_t result;
    
    if ((handle & RTC_SW_TIMER_MAGIC) != RTC_SW_TIMER_MAGIC) {
        RTC_LOG_ERROR("Error:[timer stop]handle = %x\r\n", handle);
        return RTC_SW_TIMER_STATUS_INVALID_PARAMETER;
    }

    index = handle & RTC_SW_TIMER_HANDLE_MASK;

    if ((rtc_sw_context.sw_timer[index].is_used != true) ||
        (rtc_sw_context.sw_timer[index].is_active != true)) {
        RTC_LOG_ERROR("Error:[timer stop]is_used = %d, is_active = %d\r\n", 
                        rtc_sw_context.sw_timer[index].is_used, 
                            rtc_sw_context.sw_timer[index].is_active);
        return RTC_SW_TIMER_STATUS_ERROR;
    }

    if(LIST_IS_EMPTY(rtc_sw_context.pTimer_list_head) == true) {
        RTC_ASSERT();
        return RTC_SW_TIMER_STATUS_ERROR;
    }
    
    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    rtc_sw_timer_update_basic_time("[timer stop]update basic time", NULL);
    RTC_RESTORE_INT(mask);

    mask = save_and_set_interrupt_mask();
    rtc_sw_context.sw_timer[index].is_active = false;
    list_remove(&(rtc_sw_context.pTimer_list_head), (list_item_t *)(&(rtc_sw_context.sw_timer[index])), 
                                rtc_sw_timer_remove_item_callback);
    restore_interrupt_mask(mask);
    RTC_SW_TIMER_PRINT_LIST_MESG("[timer stop]list remove");
    
    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    result = rtc_sw_timer_set_expiration_time("[timer stop]set sw timer expiration time");
    RTC_RESTORE_INT(mask);
    
    if(result == RTC_SW_TIMER_STATUS_OK) {
        rtc_wait_busy();
    }

    return RTC_SW_TIMER_STATUS_OK;
}

rtc_sw_timer_status_t rtc_sw_timer_delete(uint32_t handle)
{
    uint32_t index, mask;

    if ((handle & RTC_SW_TIMER_MAGIC) != RTC_SW_TIMER_MAGIC) {
        RTC_LOG_ERROR("Error:[timer delete]handle = %x\r\n", handle);
        return RTC_SW_TIMER_STATUS_INVALID_PARAMETER;
    }
    index = handle & RTC_SW_TIMER_HANDLE_MASK;

    if ((rtc_sw_context.used_timer_count == 0) || (rtc_sw_context.sw_timer[index].is_used != true)) {
        RTC_LOG_ERROR("Error:[timer delete]used_timer_count = %d, is_used = %d\r\n", rtc_sw_context.used_timer_count, 
                                                                            rtc_sw_context.sw_timer[index].is_used);
        return RTC_SW_TIMER_STATUS_ERROR;
    }

    rtc_sw_timer_stop(handle);

    mask = save_and_set_interrupt_mask();
    rtc_sw_context.sw_timer[index].is_used = false;
    rtc_sw_context.used_timer_count--;
    restore_interrupt_mask(mask);

    return RTC_SW_TIMER_STATUS_OK;
}

ATTR_TEXT_IN_RAM void rtc_sw_timer_isr(void)
{
    rtc_sw_timer_t  cur_time_timer, *pTimer;
    rtc_sw_timer_callback_t sw_timer_callback[RTC_SW_TIMER_NUMBER]={NULL};
    rtc_sw_timer_status_t result;
    uint32_t index = 0, mask;
    bool is_found_smaller;

    if(LIST_IS_EMPTY(rtc_sw_context.pTimer_list_head) == true) {
        return;
    }
    
    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    rtc_sw_timer_update_basic_time("[timer isr]update basic time", NULL);
    RTC_RESTORE_INT(mask);
    
    if(IS_IN_ISR == true) {
        do {
            if(index >= (RTC_SW_TIMER_NUMBER + 1)) {
                RTC_ASSERT();
            }
            mask = save_and_set_interrupt_mask();
            cur_time_timer.diff_time = 0;
            is_found_smaller = list_remove_first_smaller(&(rtc_sw_context.pTimer_list_head), (list_item_t *)(&cur_time_timer), 
                                                            rtc_sw_timer_compare_item_callback, 
                                                                rtc_sw_timer_remove_item_callback,
                                                                    rtc_sw_timer_deal_first_smaller_callback,
                                                                        &(sw_timer_callback[index]));
            index++;
            restore_interrupt_mask(mask);
            RTC_SW_TIMER_PRINT_LIST_MESG("[timer isr]list remove first smaller");
        }while(is_found_smaller == true);
    } else {
        mask = save_and_set_interrupt_mask();
        pTimer = (rtc_sw_timer_t *)(rtc_sw_context.pTimer_list_head);
        pTimer->is_active = false;
        list_remove(&(rtc_sw_context.pTimer_list_head), (list_item_t *)pTimer, //REMOVE Jericho RTC SW TIMER
                                    rtc_sw_timer_remove_item_callback);
        restore_interrupt_mask(mask);
        RTC_SW_TIMER_PRINT_LIST_MESG("[timer isr]list remove");
    }

    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    result = rtc_sw_timer_set_expiration_time("[timer isr]set sw timer expiration time");
    RTC_RESTORE_INT(mask);
    
    if(result == RTC_SW_TIMER_STATUS_OK) {
        rtc_wait_busy();
    }

    for (index = 0; index < RTC_SW_TIMER_NUMBER; index++) {
        if (sw_timer_callback[index] != NULL) {
            RTC_LOG_IMPORTANT("[timer isr]sw timer callback[%d] = 0x%08lx\r\n",index, (uint32_t)sw_timer_callback[index]);
            sw_timer_callback[index](NULL);
        }
    }
}

rtc_sw_timer_status_t rtc_sw_timer_get_remaining_time_ms(uint32_t handle, uint32_t *remaining_time)
{
    uint32_t index, mask;
    int32_t sum_diff_time = 0;
    bool is_found_list_item;
    list_item_t * pItem_to_search;
    
    if ((handle & RTC_SW_TIMER_MAGIC) != RTC_SW_TIMER_MAGIC) {
        RTC_LOG_ERROR("Error:[get timer remaining time]handle = %x\r\n", handle);
        return RTC_SW_TIMER_STATUS_INVALID_PARAMETER;
    }
    index = handle & RTC_SW_TIMER_HANDLE_MASK;

    if ((index != RTC_SW_TIMER_NUMBER) && ((rtc_sw_context.sw_timer[index].is_used != true) ||
                                                (rtc_sw_context.sw_timer[index].is_active != true))) {
        RTC_LOG_ERROR("Error:[get timer remaining time]is_used = %d, is_active = %d\r\n", rtc_sw_context.sw_timer[index].is_used, 
                                                                    rtc_sw_context.sw_timer[index].is_active);
        return RTC_SW_TIMER_STATUS_ERROR;
    }
    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    rtc_sw_timer_update_basic_time("[get timer remaining time]update basic time", NULL);
    RTC_RESTORE_INT(mask);

    mask = save_and_set_interrupt_mask();
    if(LIST_IS_EMPTY(rtc_sw_context.pTimer_list_head) == true) {
        restore_interrupt_mask(mask);
        return RTC_SW_TIMER_STATUS_ERROR;
    }
        
    if(index == RTC_SW_TIMER_NUMBER) {
        pItem_to_search = rtc_sw_context.pTimer_list_head;
    } else {
        pItem_to_search = (list_item_t *)(&rtc_sw_context.sw_timer[index]);
    }
    is_found_list_item = list_search_item(&(rtc_sw_context.pTimer_list_head), pItem_to_search, 
                                                rtc_sw_timer_search_item_callback, &sum_diff_time);
    restore_interrupt_mask(mask);
    RTC_SW_TIMER_PRINT_LIST_MESG("[get timer remaining time]list search");

    if(is_found_list_item == true) {
        if(sum_diff_time < 0) {
            *remaining_time = 0;
        } else {
            *remaining_time = (uint32_t)sum_diff_time;
        }
        return RTC_SW_TIMER_STATUS_OK;
    } else {
        return RTC_SW_TIMER_STATUS_ERROR;
    }
}

rtc_sw_timer_status_t rtc_sw_timer_is_timer_active(uint32_t handle, bool *is_active)
{
    uint32_t index;

    if ((handle & RTC_SW_TIMER_MAGIC) != RTC_SW_TIMER_MAGIC) {
        return RTC_SW_TIMER_STATUS_INVALID_PARAMETER;
    }

    index = handle & RTC_SW_TIMER_HANDLE_MASK;

    if (rtc_sw_context.sw_timer[index].is_used != true) {
        return RTC_SW_TIMER_STATUS_ERROR;
    }

    *is_active = rtc_sw_context.sw_timer[index].is_active;

    return RTC_SW_TIMER_STATUS_OK;
}

/*-----------------------------------------------------------------------------------------------*/

ATTR_TEXT_IN_RAM static bool rtc_sw_alarm_compare_item_callback(list_item_t *pCompared_item, 
                                                                    list_item_t *pList_item, void *user_data_null)
{
    rtc_sw_alarm_t *pCompared_alarm, *pAlarm;

    if(pCompared_item == NULL || pList_item == NULL) {
        RTC_ASSERT();
    }

    pCompared_alarm = (rtc_sw_alarm_t *)pCompared_item;
    pAlarm = (rtc_sw_alarm_t *)pList_item;
    
    if(pCompared_alarm->alarm_time_value < pAlarm->alarm_time_value) {
        return true;
    } else {
        return false;
    }
}

ATTR_TEXT_IN_RAM static bool rtc_sw_alarm_deal_first_smaller_callback(list_item_t *plist_item, 
                                                                                list_item_t *plist_null,
                                                                                    void *user_data)
{
    rtc_sw_alarm_t *pAlarm;
    rtc_sw_alarm_callback_context_t *pSw_alarm_callback;

    if(plist_item == NULL || user_data == NULL) {
        RTC_ASSERT();
        return false;
    }
    pAlarm = (rtc_sw_alarm_t *)plist_item;
    pSw_alarm_callback = (rtc_sw_alarm_callback_context_t *)user_data;
    *pSw_alarm_callback = pAlarm->callback_context;
    pAlarm->is_active = false;
    
    return true;
}


/*-----------------------------------------------------------------------------------------------*/

ATTR_TEXT_IN_RAM rtc_sw_alarm_status_t rtc_sw_alarm_set_expiration_time(const char *str)
{
    hal_rtc_time_t md_alarm_time;
    rtc_sw_alarm_t * pHead_sw_alarm;
    uint64_t         cur_time_value;

    if(LIST_IS_EMPTY(rtc_sw_context.pAlarm_list_head) == true) {
        return RTC_SW_ALARM_STATUS_ERROR;
    }
    pHead_sw_alarm = (rtc_sw_alarm_t *)(rtc_sw_context.pAlarm_list_head);
    cur_time_value = rtc_sw_context.sw_timer_basis_time_value;

    if(pHead_sw_alarm->alarm_time_value <= cur_time_value + SET_ALARM_OVERHEAD) {
        md_alarm_time = rtc_sw_context.sw_timer_basis_time;
        rtc_sw_timer_forward_time(&md_alarm_time, SET_ALARM_OVERHEAD);
    } else {
        md_alarm_time = pHead_sw_alarm->alarm_time;
    }
    rtc_sw_context.sw_alarm_md_alarm_time = md_alarm_time;
    rtc_print_time(str, &md_alarm_time);

    if(HAL_RTC_STATUS_OK != rtc_set_md_alarm(&md_alarm_time)) {
        RTC_ASSERT();
        return RTC_SW_ALARM_STATUS_INVALID_PARAMETER;
    }
    rtc_enable_md_alarm();
    rtc_write_trigger();

    return RTC_SW_ALARM_STATUS_OK;
}


/*-----------------------------------------------------------------------------------------------*/

rtc_sw_alarm_status_t rtc_sw_alarm_create(uint32_t *handle)
{
    uint32_t index, mask;
    
    if (handle == NULL) {
        RTC_LOG_ERROR("Error:[alarm create]handle = %x\r\n", handle);
        return RTC_SW_ALARM_STATUS_INVALID_PARAMETER;
    }

    mask = save_and_set_interrupt_mask();
    if (rtc_sw_context.used_alarm_count >= RTC_SW_ALARM_NUMBER) {
        restore_interrupt_mask(mask);
        RTC_LOG_ERROR("Error:[alarm create]used_alarm_count = %d\r\n", rtc_sw_context.used_alarm_count);
        return RTC_SW_ALARM_STATUS_ERROR_USED;
    }

    index = rtc_sw_get_free_timer_or_alarm(false);
    *handle = index | RTC_SW_ALARM_MAGIC;
    RTC_LOG_INFO("[alarm create]handle = %x, *handle = %x\r\n", handle, (*handle));

    rtc_sw_context.sw_alarm[index].is_used = true;
    rtc_sw_context.sw_alarm[index].is_active = false;
    rtc_sw_context.used_alarm_count++;
    restore_interrupt_mask(mask);

    return RTC_SW_ALARM_STATUS_OK;
}

ATTR_TEXT_IN_RAM rtc_sw_alarm_status_t rtc_sw_alarm_start(uint32_t handle, hal_rtc_time_t alarm_time, 
                                                                rtc_sw_alarm_callback_t callback, void *user_data)
{
    uint32_t mask, index;
    rtc_sw_alarm_status_t result;

    if (((handle & RTC_SW_ALARM_MAGIC) != RTC_SW_ALARM_MAGIC) || (rtc_is_time_valid(&alarm_time) != true)) {
        RTC_LOG_ERROR("Error:[alarm start]handle = %x, callback = %x\r\n", handle, callback);
        rtc_print_time("[alarm start]alarm_time", &alarm_time);
        return RTC_SW_ALARM_STATUS_INVALID_PARAMETER;
    }
    
    index = handle & RTC_SW_ALARM_HANDLE_MASK;
    RTC_LOG_INFO("[alarm start]index = %d\r\n", index);
    
    if ((rtc_sw_context.sw_alarm[index].is_active == true) ||
        (rtc_sw_context.sw_alarm[index].is_used != true)) {
        RTC_LOG_ERROR("Error:[alarm start]is_used = %d, is_active = %d\r\n", 
                        rtc_sw_context.sw_alarm[index].is_used, 
                            rtc_sw_context.sw_alarm[index].is_active);
        return RTC_SW_ALARM_STATUS_ERROR;
    }

    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    rtc_sw_timer_update_basic_time("[alarm start]update basic time", NULL);
    RTC_RESTORE_INT(mask);

    mask = save_and_set_interrupt_mask();
    alarm_time.rtc_milli_sec = (alarm_time.rtc_milli_sec & RTC_INT_CNT_MASK) >> 11;
    rtc_sw_context.sw_alarm[index].is_active = true;
    rtc_sw_context.sw_alarm[index].alarm_time = alarm_time;
    rtc_sw_context.sw_alarm[index].alarm_time_value = rtc_sw_timer_time_to_ms(&alarm_time);
    rtc_sw_context.sw_alarm[index].callback_context.callback = callback;
    rtc_sw_context.sw_alarm[index].callback_context.user_data = user_data;
    list_insert(&(rtc_sw_context.pAlarm_list_head), (list_item_t *)(&(rtc_sw_context.sw_alarm[index])), 
                    rtc_sw_alarm_compare_item_callback);
    restore_interrupt_mask(mask);
    RTC_SW_ALARM_PRINT_LIST_MESG("[alarm start]list insert");

    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    result = rtc_sw_alarm_set_expiration_time("[alarm start]set sw alarm expiration time");
    RTC_RESTORE_INT(mask);
    
    if(result == RTC_SW_ALARM_STATUS_OK) {
        rtc_wait_busy();
    }

    return result;
}


ATTR_TEXT_IN_RAM rtc_sw_alarm_status_t rtc_sw_alarm_stop(uint32_t handle)
{
    uint32_t mask, index;
    rtc_sw_alarm_status_t result;
    
    if ((handle & RTC_SW_ALARM_MAGIC) != RTC_SW_ALARM_MAGIC) {
        RTC_LOG_ERROR("Error:[alarm stop]handle = %x\r\n", handle);
        return RTC_SW_ALARM_STATUS_INVALID_PARAMETER;
    }

    index = handle & RTC_SW_ALARM_HANDLE_MASK;

    if ((rtc_sw_context.sw_alarm[index].is_used != true) ||
        (rtc_sw_context.sw_alarm[index].is_active != true)) {
        RTC_LOG_ERROR("Error:[alarm stop]is_used = %d, is_active = %d\r\n", 
                        rtc_sw_context.sw_alarm[index].is_used, 
                            rtc_sw_context.sw_alarm[index].is_active);
        return RTC_SW_ALARM_STATUS_ERROR;
    }

    if(LIST_IS_EMPTY(rtc_sw_context.pAlarm_list_head) == true) {
        RTC_ASSERT();
        return RTC_SW_ALARM_STATUS_ERROR;
    }

    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    rtc_sw_timer_update_basic_time("[alarm stop]update basic time", NULL);
    RTC_RESTORE_INT(mask);

    mask = save_and_set_interrupt_mask();
    rtc_sw_context.sw_alarm[index].is_active = false;
    list_remove(&(rtc_sw_context.pAlarm_list_head), (list_item_t *)(&(rtc_sw_context.sw_alarm[index])), NULL);
    restore_interrupt_mask(mask);
    RTC_SW_ALARM_PRINT_LIST_MESG("[alarm stop]list remove");
    
    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    result = rtc_sw_alarm_set_expiration_time("[alarm stop]set sw alarm expiration time");
    RTC_RESTORE_INT(mask);

    if(result == RTC_SW_ALARM_STATUS_OK) {
       rtc_wait_busy();
    }

    return RTC_SW_ALARM_STATUS_OK;
}

rtc_sw_alarm_status_t rtc_sw_alarm_delete(uint32_t handle)
{
    uint32_t index, mask;

    if ((handle & RTC_SW_ALARM_MAGIC) != RTC_SW_ALARM_MAGIC) {
        RTC_LOG_ERROR("Error:[alarm delete]handle = %x\r\n", handle);
        return RTC_SW_ALARM_STATUS_INVALID_PARAMETER;
    }
    index = handle & RTC_SW_ALARM_HANDLE_MASK;

    if ((rtc_sw_context.used_alarm_count == 0) || (rtc_sw_context.sw_alarm[index].is_used != true)) {
        RTC_LOG_ERROR("Error:[alarm delete]used_alarm_count = %d, is_used = %d\r\n", rtc_sw_context.used_alarm_count, 
                                                                            rtc_sw_context.sw_alarm[index].is_used);
        return RTC_SW_ALARM_STATUS_ERROR;
    }

    rtc_sw_alarm_stop(handle);

    mask = save_and_set_interrupt_mask();
    rtc_sw_context.sw_alarm[index].is_used = false;
    rtc_sw_context.used_alarm_count--;
    restore_interrupt_mask(mask);

    return RTC_SW_ALARM_STATUS_OK;
}

ATTR_TEXT_IN_RAM void rtc_sw_alarm_isr(void)
{
    uint32_t index = 0, mask;
    bool is_found_smaller;
    rtc_sw_alarm_t  cur_time_alarm;
    rtc_sw_alarm_status_t result;
    rtc_sw_alarm_callback_context_t sw_alarm_callback_context[RTC_SW_ALARM_NUMBER]={{NULL, NULL}};
    
    if(LIST_IS_EMPTY(rtc_sw_context.pAlarm_list_head) == true) {
        return;
    }
    memset(sw_alarm_callback_context, 0, sizeof(sw_alarm_callback_context));
    
    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    rtc_sw_timer_update_basic_time("[alarm isr]update basic time", NULL);
    RTC_RESTORE_INT(mask);

    cur_time_alarm.alarm_time_value = rtc_sw_context.sw_timer_basis_time_value;

    do {
        if(index >= (RTC_SW_ALARM_NUMBER + 1)) {
            RTC_ASSERT();
        }
        mask = save_and_set_interrupt_mask();
        is_found_smaller = list_remove_first_smaller(&(rtc_sw_context.pAlarm_list_head), (list_item_t *)(&cur_time_alarm), 
                                                        rtc_sw_alarm_compare_item_callback, 
                                                            NULL,
                                                                rtc_sw_alarm_deal_first_smaller_callback,
                                                                    &(sw_alarm_callback_context[index]));
        index++;
        restore_interrupt_mask(mask);
        RTC_SW_ALARM_PRINT_LIST_MESG("[alarm isr]list remove first smaller");
    }while(is_found_smaller == true);

    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    result = rtc_sw_alarm_set_expiration_time("[alarm isr]set sw alarm expiration time");
    RTC_RESTORE_INT(mask);
    
    if(result == RTC_SW_ALARM_STATUS_OK) {
        rtc_wait_busy();
    }

    for (index = 0; index < RTC_SW_ALARM_NUMBER; index++) {
        if (sw_alarm_callback_context[index].callback != NULL) {
            RTC_LOG_IMPORTANT("[alarm isr]sw alarm callback[%d] = 0x%08lx\r\n",index, (uint32_t)sw_alarm_callback_context[index].callback);
            sw_alarm_callback_context[index].callback(sw_alarm_callback_context[index].user_data);
        }
    }
}

rtc_sw_alarm_status_t rtc_sw_alarm_query_info(uint32_t handle, bool *is_active, hal_rtc_time_t *pAlarm_time)
{
    uint32_t index;

    if ((handle & RTC_SW_ALARM_MAGIC) != RTC_SW_ALARM_MAGIC) {
        return RTC_SW_ALARM_STATUS_INVALID_PARAMETER;
    }

    index = handle & RTC_SW_ALARM_HANDLE_MASK;

    if (rtc_sw_context.sw_alarm[index].is_used != true) {
        return RTC_SW_ALARM_STATUS_ERROR;
    }

    *is_active = rtc_sw_context.sw_alarm[index].is_active;
    *pAlarm_time = rtc_sw_context.sw_alarm[index].alarm_time;

    return RTC_SW_ALARM_STATUS_OK;
}

rtc_sw_alarm_status_t rtc_sw_alarm_get_remaining_time_ms(uint32_t handle, uint32_t *remaining_time)
{
    uint32_t index, mask;
    rtc_sw_alarm_t * pAlarm;

    if ((handle & RTC_SW_ALARM_MAGIC) != RTC_SW_ALARM_MAGIC) {
        RTC_LOG_ERROR("Error:[get alarm remaining time]handle = %x\r\n", handle);
        return RTC_SW_ALARM_STATUS_INVALID_PARAMETER;
    }
    index = handle & RTC_SW_ALARM_HANDLE_MASK;

    if ((index != RTC_SW_ALARM_NUMBER) && ((rtc_sw_context.sw_alarm[index].is_used != true) ||
                                                (rtc_sw_context.sw_alarm[index].is_active != true))) {
        RTC_LOG_ERROR("Error:[get alarm remaining time]is_used = %d, is_active = %d\r\n", rtc_sw_context.sw_alarm[index].is_used, 
                                                                    rtc_sw_context.sw_alarm[index].is_active);
        return RTC_SW_ALARM_STATUS_ERROR;
    }
    RTC_MASK_INT_AND_WAIT_FOR_CONDITION_TO_MEET(rtc_query_cbusy() != true, mask);
    rtc_sw_timer_update_basic_time("[get alarm remaining time]update basic time", NULL);
    RTC_RESTORE_INT(mask);

    mask = save_and_set_interrupt_mask();
    if(LIST_IS_EMPTY(rtc_sw_context.pAlarm_list_head) == true) {
        restore_interrupt_mask(mask);
        return RTC_SW_ALARM_STATUS_ERROR;
    }
    
    if(index == RTC_SW_ALARM_NUMBER) {
        pAlarm = (rtc_sw_alarm_t *)rtc_sw_context.pAlarm_list_head;
    } else {
        pAlarm = &rtc_sw_context.sw_alarm[index];
    }

    if(pAlarm->alarm_time_value <= rtc_sw_context.sw_timer_basis_time_value) {
        *remaining_time = 0;
    } else {
        *remaining_time = (uint32_t)(pAlarm->alarm_time_value - rtc_sw_context.sw_timer_basis_time_value);
    }
    restore_interrupt_mask(mask);

    return RTC_SW_ALARM_STATUS_OK;
}

/*-----------------------------------------------------------------------------------------------*/


hal_rtc_status_t rtc_sw_get_remaining_time(uint32_t *remaining_time)
{
    uint32_t timer_min_remaining_time, alarm_min_remaining_time;
    rtc_sw_alarm_status_t alarm_status;
    rtc_sw_timer_status_t timer_status;

    timer_status = rtc_sw_timer_get_remaining_time_ms((RTC_SW_TIMER_MAGIC | RTC_SW_TIMER_NUMBER), 
                                                        &timer_min_remaining_time);
    alarm_status = rtc_sw_alarm_get_remaining_time_ms((RTC_SW_ALARM_MAGIC | RTC_SW_ALARM_NUMBER), 
                                                        &alarm_min_remaining_time);

    if((timer_status == RTC_SW_TIMER_STATUS_ERROR) && (alarm_status == RTC_SW_ALARM_STATUS_ERROR)) {
        return HAL_RTC_STATUS_ERROR;
    } else if(timer_status == RTC_SW_TIMER_STATUS_ERROR) {
        *remaining_time = alarm_min_remaining_time;
    } else if(alarm_status == RTC_SW_ALARM_STATUS_ERROR) {
        *remaining_time = timer_min_remaining_time;
    } else {
        if(timer_min_remaining_time < alarm_min_remaining_time) {
            *remaining_time = timer_min_remaining_time;
        } else {
            *remaining_time = alarm_min_remaining_time;
        }
    }

    return HAL_RTC_STATUS_OK;
}


#endif
#endif
