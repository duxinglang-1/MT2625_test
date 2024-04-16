/* Copyright Statement:
 *
 * (C) 2005-2017 MediaTek Inc. All rights reserved.
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
 * the License Agreement ("Permitted User"). If you are not a Permitted User,
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

#include "tel_conn_mgr_bearer_timer.h"
#include "tel_conn_mgr_common.h"

#if defined(TEL_CONN_MGR_ACTIVATION_GUARD_TIMER_SUPPORT) || defined(TEL_CONN_MGR_DEACTIVATION_GUARD_TIMER_SUPPORT)

#define TEL_CONN_MGR_TIMER_POOL_SIZE (2)


typedef struct
{
    tel_conn_mgr_bool  is_used;
    unsigned char timer_id;
    eventid event_id;
    tel_conn_mgr_timer_expiry_hdlr expiry_hdlr;
    int cid;
} tel_conn_mgr_timer_info_struct;


tel_conn_mgr_timer_info_struct tel_conn_mgr_timer_pool[TEL_CONN_MGR_TIMER_POOL_SIZE] = {0};
event_scheduler *tel_conn_mgr_evt_scheduler = NULL;

static event_scheduler *tel_conn_mgr_get_evt_scheduler(void)
{
    return L4C_PTR->event_scheduler_ptr;
}

void tel_conn_mgr_timer_init(void)
{
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Create an Event Scheduler */
    //tel_conn_mgr_evt_scheduler = evshed_create("TEL_CONN_MGR_BASE_TIMER", MOD_L4C, 0, 0 );
    memset(&tel_conn_mgr_timer_pool, 0, sizeof(tel_conn_mgr_timer_info_struct) * TEL_CONN_MGR_TIMER_POOL_SIZE);
}


void tel_conn_mgr_timer_deinit(void)
{
    // TODO:
}


static tel_conn_mgr_timer_info_struct *tel_conn_mgr_timer_get_free_timer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    unsigned char id;
    tel_conn_mgr_timer_info_struct *timer_ptr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (id = 0; id < TEL_CONN_MGR_TIMER_POOL_SIZE; id++)    
    {
        timer_ptr = &tel_conn_mgr_timer_pool[id];
        if (timer_ptr->is_used == TEL_CONN_MGR_FALSE)
        {
            return timer_ptr;
        }
    }

    return NULL;

}


static void tel_conn_mgr_timer_expiry_handler(void *msg_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tel_conn_mgr_timer_info_struct *timer_ptr;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    assert(msg_ptr != NULL);

    timer_ptr = (tel_conn_mgr_timer_info_struct*) msg_ptr;

    if (timer_ptr->is_used == TEL_CONN_MGR_FALSE)
    {
        return;
    }

    if (timer_ptr->expiry_hdlr)
    {
        timer_ptr->expiry_hdlr(timer_ptr->timer_id, timer_ptr->cid);
    }

done:
    timer_ptr->event_id = NULL;
    timer_ptr->is_used = TEL_CONN_MGR_FALSE; /* mark as free */
    timer_ptr->expiry_hdlr = NULL;
}


void tel_conn_mgr_timer_start(unsigned char timer_id, int cid, unsigned int time_out, tel_conn_mgr_timer_expiry_hdlr expiry_hdlr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/          
    tel_conn_mgr_timer_info_struct *timer_ptr;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (TEL_CONN_MGR_MODME_TIMER_ID_NONE == timer_id ||
        TEL_CONN_MGR_MODME_TIMER_ID_MAX < timer_id ||
        !expiry_hdlr)
    {
        return;
    }

    timer_ptr = tel_conn_mgr_timer_get_free_timer();

    if (timer_ptr)
    {
        timer_ptr->is_used = TEL_CONN_MGR_TRUE;
        timer_ptr->timer_id = timer_id;
        timer_ptr->cid = cid;
        timer_ptr->expiry_hdlr = expiry_hdlr;
        timer_ptr->event_id = evshed_set_event(tel_conn_mgr_get_evt_scheduler(),
                                 (kal_timer_func_ptr)tel_conn_mgr_timer_expiry_handler,
                                 (void*)timer_ptr,
                                 time_out);
    }
}


static tel_conn_mgr_timer_info_struct *tel_conn_mgr_timer_get_timer_ptr(unsigned char timer_id, int cid)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tel_conn_mgr_timer_info_struct *timer_ptr = NULL;
    unsigned char id;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (id = 0; id < TEL_CONN_MGR_TIMER_POOL_SIZE; id++)    
    {
        timer_ptr = &tel_conn_mgr_timer_pool[id];

        if ((timer_ptr->is_used == TEL_CONN_MGR_TRUE) &&
            (timer_ptr->timer_id == timer_id) &&
            (timer_ptr->cid == cid))
        {
            return timer_ptr;
        }
    }

    return NULL;
}


void tel_conn_mgr_timer_stop(unsigned char timer_id, int cid)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    tel_conn_mgr_timer_info_struct *timer_ptr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    timer_ptr = tel_conn_mgr_timer_get_timer_ptr(timer_id, cid);

    if (timer_ptr == NULL)
    {
        /* Cannot find the timer to stop */
        return;
    }

    if (timer_ptr->is_used == TEL_CONN_MGR_TRUE)
    {
        evshed_cancel_event(tel_conn_mgr_get_evt_scheduler(), &timer_ptr->event_id);
        timer_ptr->is_used = TEL_CONN_MGR_FALSE;
    }
}
#endif /*defined(TEL_CONN_MGR_ACTIVATION_GUARD_TIMER_SUPPORT) || defined(TEL_CONN_MGR_DEACTIVATION_GUARD_TIMER_SUPPORT)*/

