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
 
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "semphr.h"

/* hal includes */
#include "memory_attribute.h"
#include "hal.h"
#include "sdio_reg.h"

#include "lwip/pbuf.h"
#include "wfcm_stub.h"
#include "hif_sdio.h"


#define MEM_QUEUE_SIZE                8   //8
#define SDIO_HRX_DATA_SIZE        1600
#define SDIO_HTX_DATA_SIZE        1600

#ifndef ALIGN_TO_BLOCK_SIZE
#define ALIGN_TO_BLOCK_SIZE(_value)     (((_value) + (WIFI_BLK_SIZE - 1)) & ~(WIFI_BLK_SIZE - 1))
#endif

#ifndef ALIGN_32
#define ALIGN_32(_value)             (((_value) + 31) & ~31u)
#endif /* ALIGN_32*/

#ifndef ALIGN_4
#define ALIGN_4(_value)             (((_value) + 3) & ~3u)
#endif /* ALIGN_4 */

typedef struct {
    union {
        struct {
            uint32_t  length: 16;
            uint32_t  reserved: 13;
            uint32_t  tx_type: 3;
        } bits;
        uint32_t         asUINT32;
    } u;
} brom_sdio_tx_sdu_header_t;  // 4byte tx header


SemaphoreHandle_t g_sem_qbuf = NULL;
SemaphoreHandle_t g_wfcm_sem_htx = NULL;
uint8_t g_memq_list[MEMQ_CTRL_MAX_NUM][MEM_QUEUE_SIZE];

ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t g_host_rx_buf[MEM_QUEUE_SIZE][SDIO_HRX_DATA_SIZE];
ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN  uint8_t g_host_tx_buf[MEM_QUEUE_SIZE][SDIO_HTX_DATA_SIZE];


int32_t wfcm_mq_get_buf(uint32_t ctrl_idx, uint32_t *qidx)
{
	uint32_t idx;
    xSemaphoreTake(g_sem_qbuf, portMAX_DELAY);

    for (idx=0; idx<(MEM_QUEUE_SIZE-1); idx++)
    {
    	if (g_memq_list[ctrl_idx][idx] == 0)
    	{
    		g_memq_list[ctrl_idx][idx] = 1; //allocated
    		*qidx = idx;

            xSemaphoreGive(g_sem_qbuf);
    		return idx;
    	}
    }	
    *qidx = WFC_MEMQ_INVALID;

    xSemaphoreGive(g_sem_qbuf);
    return -1;
}

void wfcm_mq_free(T_WFC_QBUF *qbuf)
{
    xSemaphoreTake(g_sem_qbuf, portMAX_DELAY);
    g_memq_list[qbuf->ctrl][qbuf->qidx] = 0; //free
    qbuf->qidx = WFC_MEMQ_INVALID;
    qbuf->size = 0;
    xSemaphoreGive(g_sem_qbuf);
}

void wfcm_mem_align_clr(uint8_t *pbuf, uint32_t bufSize)
{
	uint32_t sidx = bufSize;
	uint32_t eidx;

	//sidx = g_hrx_free_qbuf.size;
	eidx = ALIGN_TO_BLOCK_SIZE(bufSize);
	if (eidx > sidx)
	{	
	   memset(pbuf+sidx, 0, eidx-sidx);
    }
}

    
void wfcm_dma_memcpy(void *dst, void *src, unsigned int len)    
{                                                                      
    hal_gdma_status_t ret;                                             
    hal_gdma_running_status_t running_status;                          
    hal_gdma_channel_t channel = HAL_GDMA_CHANNEL_0;                   

    hal_gdma_init(HAL_GDMA_CHANNEL_0);

    ret = hal_gdma_start_polling(channel, (uint32_t)dst, (uint32_t)src, len);
    if(HAL_GDMA_STATUS_OK != ret)                                  
    {                                                              
        //printf("DMA unavailable...\n");                            
        memcpy(dst, src, len);                                     
    }                                                                                                                                     
    hal_gdma_get_running_status(channel,&running_status);          
    hal_gdma_stop(HAL_GDMA_CHANNEL_0); // Stop the GDMA.
    hal_gdma_deinit(HAL_GDMA_CHANNEL_0);   //De-initialize the GDMA.	  	
}                   

bool wfcm_sdio_check_init(void)
{
    uint32_t      wcir_reg = 0;
  
    do {
    } while (wifi_read_bus(SDIO_IP_WCIR, 4, (uint8_t *)&wcir_reg)==true); /*waiting Boot ROM release*/

    return true;
}

bool wfcm_sdio_hif_get_driver_own(void)
{
    bool ret ;
    uint32_t value ;
    uint32_t cnt = 50;

    printf("[wfcm_sdio_hif_get_driver_own]<==========>\r\n") ;

    //Set driver own
    value = W_FW_OWN_REQ_CLR ;
    if ((ret = wifi_write_bus(SDIO_IP_WHLPCR, 4, (uint8_t *)&value)) == false) {
        return false;
    }

    while (cnt--) {
        if ((ret = wifi_read_bus(SDIO_IP_WHLPCR, 4, (uint8_t *)&value)) == false) {
            return false ;
        }

        if (value & W_DRV_OWN_STATUS) {
            return true;
        }
        hal_gpt_delay_ms(10);
    }

    return false;
}


int wfcm_if_sdio_init(void)
{

    if(g_sem_qbuf == NULL) {
        g_sem_qbuf  = xSemaphoreCreateBinary();
    }
    xSemaphoreGive(g_sem_qbuf);

    if(g_wfcm_sem_htx == NULL) {
        g_wfcm_sem_htx  = xSemaphoreCreateBinary();
    }
    xSemaphoreGive(g_wfcm_sem_htx);

    if(wifi_bus_init() == false) {
        return HAL_SDIO_STATUS_ERROR;
    }

    if (false == wfcm_sdio_hif_get_driver_own()) {
        printf("1 get driver own fail. \r\n");
        return HAL_SDIO_STATUS_ERROR;
    } else {
        printf("1 get driver own success. \r\n");
    }
    
    memset(g_host_rx_buf, 0, MEM_QUEUE_SIZE*SDIO_HRX_DATA_SIZE);
    memset(g_host_tx_buf, 0, MEM_QUEUE_SIZE*SDIO_HTX_DATA_SIZE);
    
    return HAL_SDIO_STATUS_OK;
}

void wfcm_if_sdio_reinit(void)
{
    hal_pinmux_set_function(11, 4);
    hal_pinmux_set_function(12, 4);
    hal_pinmux_set_function(13, 4);
    hal_pinmux_set_function(14, 4);
    hal_pinmux_set_function(15, 4);
    hal_pinmux_set_function(16, 4);

    hal_gpio_set_pupd_register(11, 0, 0, 1);
    hal_gpio_set_pupd_register(12, 0, 0, 1);
    hal_gpio_set_pupd_register(13, 0, 0, 1);
    hal_gpio_set_pupd_register(14, 0, 0, 1);
    hal_gpio_set_pupd_register(15, 0, 0, 1);
    hal_gpio_set_pupd_register(16, 0, 0, 1);	
}


void wfcm_if_sdio_deinit(void)
{
	if(g_sem_qbuf != NULL) {
		wifi_os_semphr_delete(g_sem_qbuf);
		g_sem_qbuf = NULL;
	}
	if(g_wfcm_sem_htx != NULL) {
		wifi_os_semphr_delete(g_wfcm_sem_htx);
		g_wfcm_sem_htx = NULL;
	}
    wifi_bus_deinit();
}


void wfcm_memq_get_qbuf(T_WFC_QBUF *qbuf)
{
    // Get free index from Host TX receive queue buffer
    while ( wfcm_mq_get_buf(qbuf->ctrl, &(qbuf->qidx)) < 0)
    {
        hal_gpt_delay_us(10);
    }
}

void wfcm_sdio_cmd_cp(uint16_t opcode, T_WFC_QBUF *qbuf, uint8_t *param, uint32_t paramSize, uint8_t forISR)
{
    uint16_t *op;
    uint8_t  *pdst;

    qbuf->size = paramSize + sizeof(opcode); //opcode(2)

    if (forISR)
    {
        qbuf->qidx = MEM_QUEUE_SIZE-1;
    }
    // Clear HTX Buffer (After Buffer Size)
    wfcm_mem_align_clr(&(g_host_tx_buf[qbuf->qidx][0]), qbuf->size + sizeof(brom_sdio_tx_sdu_header_t));

    // Assign OpCode    
    op = (uint16_t *)(&(g_host_tx_buf[qbuf->qidx][0])+ sizeof(brom_sdio_tx_sdu_header_t));
    *op  = opcode;	

    // Copy Cmd Parameter
    if (param && paramSize)
    {
        pdst = (uint8_t *)(&(g_host_tx_buf[qbuf->qidx][0])+ sizeof(brom_sdio_tx_sdu_header_t));        
        pdst = pdst + sizeof(opcode);        //opcode(2)
        memcpy(pdst, param, paramSize); 
    }
}


void wfcm_sdio_htx_cp(uint16_t opcode, T_WFC_QBUF *qbuf, uint8_t *htxptr, uint32_t htxSize)
{
    uint16_t *op;
    struct pbuf *q;
    uint8_t  *pdst;

    qbuf->size = htxSize+4; //opcode(2)+2, 4 bytes alignment

    
    // Clear HTX Buffer (After Buffer Size 4 Bytes)
    wfcm_mem_align_clr(&(g_host_tx_buf[qbuf->qidx][0]), qbuf->size + sizeof(brom_sdio_tx_sdu_header_t));

    // Assign OpCode    
    op = (uint16_t *)(&(g_host_tx_buf[qbuf->qidx][0])+ sizeof(brom_sdio_tx_sdu_header_t));
    *op  = opcode;	

    // Copy HTX Data
    pdst = ((uint8_t*)op)+4; //opcode(2)+2, 4 bytes alignment 
        
    for(q = (struct pbuf *)htxptr; q != NULL; q = q->next) {
      if ( (((uint32_t)q->payload)%4) || (((uint32_t)pdst)%4) )
      {
      	  memcpy(pdst, (uint8_t*)q->payload, q->len);          
      }		
      else
      {	
          wfcm_dma_memcpy(pdst, (uint8_t*)q->payload, q->len);   
      }
      pdst += q->len;
    } 
}


typedef struct {
    uint32_t whisr;
    uint32_t wtsr0;
    uint32_t wtsr1;
    uint16_t rx0_num;
    uint16_t rx1_num;
    uint16_t rx0_len[16];
    uint16_t rx1_len[16];
    uint32_t d2hrm0r;
    uint32_t d2hrm1r;
}s_enhance_interrupt_mode_data;
s_enhance_interrupt_mode_data g_whisr_data;
volatile uint32_t g_tx_cnt = 0;

void wfcm_get_tx_rx_status(uint8_t rx_flag)
{
    s_enhance_interrupt_mode_data whisr_data;
    if(rx_flag == 1) {
    	do {
    	    wifi_read_bus(SDIO_IP_WHISR, sizeof(s_enhance_interrupt_mode_data), (uint8_t *)&whisr_data);
            g_tx_cnt += (whisr_data.wtsr0 & 0xff00)>>8;
    	} while (0 == (whisr_data.whisr & 0x02)); /*wait RX0_DONE_INT*/
        g_whisr_data = whisr_data;
    }else if(rx_flag == 0) {    
        do {
            wifi_read_bus(SDIO_IP_WHISR, sizeof(s_enhance_interrupt_mode_data), (uint8_t *)&whisr_data);
            if((whisr_data.whisr & 0x02) == 2){
                g_whisr_data = whisr_data;
            }
        } while (0 == (whisr_data.whisr & 0x01)); /*waiting TX_DONE_INT*/
    }
    return;
}
bool wfcm_sdio_send_data(T_WFC_QBUF *qbuf, uint8_t qbuf_release)
{

    uint32_t      whisr_reg = 0;
    bool          ret       = true;

    brom_sdio_tx_sdu_header_t *tx_header = (brom_sdio_tx_sdu_header_t *)&(g_host_tx_buf[qbuf->qidx][0]);
	
    tx_header->u.asUINT32 = 0;

    tx_header->u.bits.length = qbuf->size + sizeof(brom_sdio_tx_sdu_header_t);  

#if 0 //enhance interrupt mode
    if(g_tx_cnt == 0) {
        wfcm_get_tx_rx_status(0);
    }else {
        g_tx_cnt--;
    }
#else
    //Waiting TX_DONE_INT
    do {
        wifi_read_bus(SDIO_IP_WHISR, 4, (uint8_t *)&whisr_reg);
    } while (0 == (whisr_reg & 0x01)); /*waiting TX_DONE_INT*/
    
    //Clear TX_DONE
    wifi_read_bus(SDIO_IP_WTSR0, 4, (uint8_t *)&whisr_reg);
#endif    
    if (wifi_write_bus(SDIO_IP_WTDR1, (qbuf->size + sizeof(brom_sdio_tx_sdu_header_t)), &(g_host_tx_buf[qbuf->qidx][0])) == false) {
        printf("[ERR] sdio_send_pkt => wifi_write_bus 0x%08x len=%lu error\r\n", SDIO_IP_WTDR1, (qbuf->size + sizeof(brom_sdio_tx_sdu_header_t)));
        ret = false;
    }

    //Free HTX Buffer
    if (qbuf_release){
        wfcm_mq_free(qbuf);
    }
    return ret;
}


bool wfcm_sdio_receive_cmdrsp(uint8_t *rx_buf, uint32_t *bufSize)
{
	uint32_t whisr_reg = 0;
	uint32_t wrplr_reg = 0;
			
	do {
	    wifi_read_bus(SDIO_IP_WHISR, 4, (uint8_t *)&whisr_reg);
	} while (0 == (whisr_reg & 0x02)); /*wait RX0_DONE_INT */
	
    wifi_read_bus(SDIO_IP_WRPLR, 4, (uint8_t *)&wrplr_reg);
    *bufSize = (wrplr_reg&0xffff);

    if (wifi_read_bus(SDIO_IP_WRDR0, (wrplr_reg&0xffff), rx_buf) == false) {
        printf("[ERR],sdio_receive_pkt, wifi_read_bus SDIO_IP_WRDR0 fail\r\n");
    	return false;
    }

    return true;
}


bool wfcm_sdio_receive_data(uint8_t *pbuf, uint32_t bufSize)
{
	uint32_t whisr_reg = 0;
	uint32_t wrplr_reg = 0;	
	
	do {
	    wifi_read_bus(SDIO_IP_WHISR, 4, (uint8_t *)&whisr_reg);
	} while (0 == (whisr_reg & 0x02)); /*wait RX0_DONE_INT*/

    wifi_read_bus(SDIO_IP_WRPLR, 4, (uint8_t *)&wrplr_reg);
    
    if (wifi_read_bus(SDIO_IP_WRDR0, bufSize, pbuf) == false) {
        printf("[ERR],sdio_receive_pkt, wifi_read_bus SDIO_IP_WRDR0 fail\r\n");
    	return false;
    }

    return true;
}

bool wfcm_sdio_receive_data_size(uint32_t *bufSize)
{
	uint32_t whisr_reg = 0;
	uint32_t wrplr_reg = 0;

#if 0  //enhance interrupt mode  
    if((g_whisr_data.whisr & 0x02) == 0) {
        wfcm_get_tx_rx_status(1);
    }	
    if(g_whisr_data.rx0_num == 1) {
        *bufSize = g_whisr_data.rx0_len[0]&0xffff;
    }else {
        printf("get data size wrong.\r\n");
    }

    g_whisr_data.whisr = 0;
    //os_memset(&g_whisr_data, 0, sizeof(g_whisr_data));
#else
    do {
        wifi_read_bus(SDIO_IP_WHISR, 4, (uint8_t *)&whisr_reg);
    } while (0 == (whisr_reg & 0x02)); /*wait RX0_DONE_INT*/

    wifi_read_bus(SDIO_IP_WRPLR, 4, (uint8_t *)&wrplr_reg);

    *bufSize = wrplr_reg&0xffff;

#endif
    return true;
}

bool wfcm_sdio_receive_data_payload(uint8_t *pbuf, uint32_t bufSize)
{
	    
    if (wifi_read_bus(SDIO_IP_WRDR0, bufSize, pbuf) == false) {
        printf("[ERR],sdio_receive_pkt, wifi_read_bus SDIO_IP_WRDR0 fail\r\n");
    	return false;
    }

    return true;
}

