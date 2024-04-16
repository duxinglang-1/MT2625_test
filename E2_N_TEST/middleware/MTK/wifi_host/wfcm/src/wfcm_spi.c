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

#include "lwip/pbuf.h"
#include "wfcm_stub.h"

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

#define MEM_QUEUE_SIZE                8   //8
#define SDIO_HTX_DATA_SIZE        1600
#define SDIO_HRX_DATA_SIZE        1600
SemaphoreHandle_t g_sem_qbuf = NULL;
SemaphoreHandle_t g_wfcm_sem_htx = NULL;
#ifndef WIFI_BLK_SIZE
#define WIFI_BLK_SIZE (256)
#endif
#ifndef ALIGN_TO_BLOCK_SIZE
#define ALIGN_TO_BLOCK_SIZE(_value)     (((_value) + (WIFI_BLK_SIZE - 1)) & ~(WIFI_BLK_SIZE - 1))
#endif


uint8_t g_memq_list[MEMQ_CTRL_MAX_NUM][MEM_QUEUE_SIZE];
ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN  uint8_t g_host_tx_buf[MEM_QUEUE_SIZE][SDIO_HTX_DATA_SIZE];
ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t g_host_rx_buf[MEM_QUEUE_SIZE][SDIO_HRX_DATA_SIZE];

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

void wfcm_memq_get_qbuf(T_WFC_QBUF *qbuf)
{
    // Get free index from Host TX receive queue buffer
    while ( wfcm_mq_get_buf(qbuf->ctrl, &(qbuf->qidx)) < 0)
    {
        hal_gpt_delay_us(10);
    }
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
    wfcm_mem_align_clr(&(g_host_tx_buf[qbuf->qidx][0]), qbuf->size);

    // Assign OpCode    
    op = (uint16_t *)(&(g_host_tx_buf[qbuf->qidx][0]));
    *op  = opcode;	

    // Copy Cmd Parameter
    if (param && paramSize)
    {
        pdst = (uint8_t *)(&(g_host_tx_buf[qbuf->qidx][0]));        
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

int wfcm_if_spi_init(void)
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
        return -1;
    }

    memset(g_host_rx_buf, 0, MEM_QUEUE_SIZE*SDIO_HRX_DATA_SIZE);
    memset(g_host_tx_buf, 0, MEM_QUEUE_SIZE*SDIO_HTX_DATA_SIZE);

    hal_clock_FREF_force_output(0); //disable 26Mhz force output, then slave can control host give 26Mhz or not.
        
    return 0;
}

void wfcm_if_spi_reinit(void)
{
#if 0
    hal_spi_master_config_t          spi_config;
    hal_spi_master_advanced_config_t advanced_config;	
    
    /* Step2: Initializes SPI master with normal mode */
    spi_config.bit_order       = HAL_SPI_MASTER_LSB_FIRST;
    spi_config.clock_frequency = SPIM_TEST_FREQUENCY;
    spi_config.phase           = HAL_SPI_MASTER_CLOCK_PHASE0;
    spi_config.polarity        = HAL_SPI_MASTER_CLOCK_POLARITY0;
    
    if (HAL_SPI_MASTER_STATUS_OK != hal_spi_master_init(SPIM_TEST_PORT, &spi_config)) {
        printf("SPI master init failed\r\n");
        return;
    }
       
    advanced_config.byte_order    = HAL_SPI_MASTER_LITTLE_ENDIAN;
    advanced_config.chip_polarity = HAL_SPI_MASTER_CHIP_SELECT_LOW;
    advanced_config.get_tick      = HAL_SPI_MASTER_GET_TICK_DELAY2; /* for 48MHz, must adjust delay_tick */
    advanced_config.sample_select = HAL_SPI_MASTER_SAMPLE_POSITIVE;
    
    if (HAL_SPI_MASTER_STATUS_OK != hal_spi_master_set_advanced_config(SPIM_TEST_PORT, &advanced_config)) {
        printf("SPI master set advanced config failed\r\n");
        return;
    }
#endif
}

void wfcm_if_spi_deinit(void)
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

bool wfcm_spi_send_data(T_WFC_QBUF *qbuf, uint8_t qbuf_release)
{

    uint32_t      whisr_reg = 0;
    bool          ret       = true;

    //brom_sdio_tx_sdu_header_t *tx_header = (brom_sdio_tx_sdu_header_t *)&(g_host_tx_buf[qbuf->qidx][0]);
	
    //tx_header->u.asUINT32 = 0;

    //tx_header->u.bits.length = qbuf->size + sizeof(brom_sdio_tx_sdu_header_t);  

    //Waiting TX_DONE_INT
    if (wifi_write_bus(NULL, (qbuf->size), &(g_host_tx_buf[qbuf->qidx][0])) == false) {
        printf("[ERR] spi_send_pkt => wifi_write_bus len=%lu error\r\n", (qbuf->size + sizeof(brom_sdio_tx_sdu_header_t)));
        ret = false;
    }

    //Free HTX Buffer
    if (qbuf_release){
        wfcm_mq_free(qbuf);
    }
    return ret;
}

bool wfcm_spi_receive_data(uint8_t *pbuf, uint32_t bufSize)
{
    uint32_t data_size = 0;
    if(wifi_read_bus(NULL, sizeof(data_size), &data_size) == false) {
        printf("read data size fail.\r\n");
        return false;
    }
    
    if (wifi_read_bus(NULL, bufSize, pbuf) == false) {
        printf("[ERR],spi_receive_pkt, wifi_read_bus fail\r\n");
    	return false;
    }

    //hex_dump("RCV data",pbuf,bufSize);
    return true;
}


bool wfcm_spi_receive_cmdrsp(uint8_t *rx_buf, uint32_t *bufSize)
{
    uint16_t size;
    //read cmdrsp size
    if(wifi_read_bus(NULL, sizeof(bufSize), bufSize) == false) {
        printf("read data size fail.\r\n");
        return false;
    }

    size = *bufSize;

    vTaskDelay(10);
    //read buffer
    if (wifi_read_bus(NULL, size, rx_buf) == false) {
        printf("[ERR],spi_receive_pkt, wifi_read_bus fail\r\n");
    	return false;
    }

    return true;
}

bool wfcm_spi_receive_data_size(uint32_t *bufSize)
{
    if(wifi_read_bus(NULL, sizeof(bufSize), bufSize) == false) {
        printf("read data size fail.\r\n");
        return false;
    }
    return true;
}

bool wfcm_spi_receive_data_payload(uint8_t *pbuf, uint32_t bufSize)
{
	    
    if (wifi_read_bus(NULL, bufSize, pbuf) == false) {
        printf("[ERR],spi_receive_pkt, wifi_read_bus fail\r\n");
    	return false;
    }

    //hex_dump("RCV data",pbuf,bufSize);
    return true;
}

