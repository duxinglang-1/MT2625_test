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

#include "hal_usb.h"
#include "hal_usb_internal.h"
#include "hal_eint.h"
#include "hal_log.h"
#include "hal_clock.h"
#include "hal_clock_internal.h"
//#include "hal_pmu_wrap_interface.h"
#include "hal_gpt.h"
#include "hal_nvic.h"
#include "hal_gpio.h"
#include "hal_pdma_internal.h"

#ifdef HAL_USB_MODULE_ENABLED

#include "mt2625.h"
#include "hal_nvic.h"
#include "hal_nvic_internal.h"
#include "memory_attribute.h"
#include "hal_pmu.h"

#if 0
ATTR_TEXT_IN_RAM  uint32_t SaveAndSetIRQMask(void)
{
    uint32_t mask;
    mask = save_and_set_interrupt_mask();
    return mask;

}

ATTR_TEXT_IN_RAM void RestoreIRQMask(uint32_t mask)
{
    restore_interrupt_mask(mask);
}
#endif

typedef enum {
    /* USB_COMMON */
    USB_CABLE_MSG                 = 1,
    /* USB_ACM */
    USB_ACM_MSG                 = 2,
    /* USB_MSC */
	USB_MSC_TX_MSG 				= 3,
	USB_MSC_RX_MSG              = 4,
	USB_MSC_CLR_STALL_MSG		= 5,
	USB_MSC_RESET_IND_MSG		= 6,
} usb_msg_type_t;

static volatile USB_REGISTER_T *musb = (USB_REGISTER_T *)USB_BASE;
static void usb_pdn_enable(void);
static void usb_pdn_disable(void);

/* Exception flag*/
USB_Drv_Info g_UsbDrvInfo;

/* EP0's FIFO address is fixed from 0~63 */
static uint32_t g_FIFOadd = USB_FIFO_START_ADDRESS;
static volatile bool usb_cable_in = false;

static uint8_t usb_get_dma_channel_num(uint8_t ep_num, hal_usb_endpoint_direction_t direction);
//static void usb_hw_stop_dma_channel(uint32_t ep_num, hal_usb_endpoint_direction_t direction);
static void usb_ep_check(uint32_t ep_num, hal_usb_endpoint_direction_t direction, uint32_t line);
//static void usb_ep_dma_running_check(uint32_t ep_num, hal_usb_endpoint_direction_t direction, uint32_t line);
static void usb_ep0en(void);
static void usb_hw_epfifowrite(uint32_t ep_num, uint16_t nBytes, void *pSrc);
//static bool usb_check_dma_time_out(uint8_t dma_chan);
//static void usb_dma_callback_func(uint8_t dma_chan);

extern void USB_Send_Message(usb_msg_type_t msg, void *data);

/************************************************************
	USB PDN
*************************************************************/



#define CLKSQ_CON0__F_DA_SRCLKENA                       ((volatile uint8_t *)0xA2040020)
#define MPLL_CON5__F_RG_MPLL_IBANK_FINETUNE             ((volatile uint32_t *)0xA2040116)
#define MPLL_CON0__F_DA_MPLL_EN                         ((volatile uint8_t *)0xA2040100)
#define MPLL_CON0__F_RG_MPLL_RDY                        ((volatile uint8_t *)0xA2040103)
#define CKSYS_CLK_CFG_2__F_CLK_48M_SEL                  ((volatile uint8_t *)0xA2020248)

#define PDN_SETD0               ((volatile uint32_t *)0xA21D0310) 
#define XO_PDN_SETD0            ((volatile uint32_t *)0xA2030B10)
#define PDN_CLRD0               ((volatile uint32_t *)0xA21D0320) 
#define XO_PDN_CLRD0            ((volatile uint32_t *)0xA2030B20)
#define RG_SW_48M_CG            0x00000001 
#define RG_SW_USB_BUS_CG        0x04000000 
#define RG_SW_USB48M_CG         0x02000000

static void usb_pdn_enable(void)
{
    hal_clock_disable(HAL_CLOCK_CG_48M);
    hal_clock_disable(HAL_CLOCK_CG_USB_BUS);
    hal_clock_disable(HAL_CLOCK_CG_USB48M);
    
    clock_mux_sel(CLK_48M_SEL, 0);
    
    //*XO_PDN_SETD0 = RG_SW_48M_CG;
    //*PDN_SETD0 = RG_SW_USB_BUS_CG | RG_SW_USB48M_CG;
    //*CKSYS_CLK_CFG_2__F_CLK_48M_SEL = 0x0;
    //pmu_set_vcore_voltage(PMU_VCORE_UNLOCK, PMU_VCORE_VOL_1P2V);
    //pmu_ctrl_driver_power(DRIVER_USB, PMIC_CTL_DISABLE);	/* Turn off 3.3V phy power */
}

/* USB PDN disable */
static void usb_pdn_disable(void)
{
    //pmu_ctrl_driver_power(DRIVER_USB, PMIC_CTL_ENABLE); /* Turn on 3.3V phy power */
    //pmu_set_vcore_voltage(PMU_VCORE_LOCK, PMU_VCORE_VOL_1P2V);    
    //*XO_PDN_CLRD0 = RG_SW_48M_CG;	
    //*PDN_CLRD0 = RG_SW_USB_BUS_CG | RG_SW_USB48M_CG;

    clock_mux_sel(CLK_48M_SEL, 1);
    
    hal_clock_enable(HAL_CLOCK_CG_USB48M);
    hal_clock_enable(HAL_CLOCK_CG_USB_BUS);
    hal_clock_enable(HAL_CLOCK_CG_48M);
}

/************************************************************
	Dump USB RG for debug
*************************************************************/
#if 0
static void hal_dump_regs(void)
{
    volatile USBPHY_REGISTER_T *phy = (USBPHY_REGISTER_T *)USB_SIFSLV_BASE;
    volatile USB_REGISTER_T *mac = (USB_REGISTER_T *)USB_BASE;

    log_hal_info("mac->faddr                  : 0x%X\r\n", &mac->faddr);
    log_hal_info("mac->power                  : 0x%X\r\n", &mac->power);
    log_hal_info("mac->intrtx                 : 0x%X\r\n", &mac->intrtx);
    log_hal_info("mac->intrrx                 : 0x%X\r\n", &mac->intrrx);
    log_hal_info("mac->intrtxe                : 0x%X\r\n", &mac->intrtxe);
    log_hal_info("mac->intrrxe                : 0x%X\r\n", &mac->intrrxe);
    log_hal_info("mac->intrusb                : 0x%X\r\n", &mac->intrusb);
    log_hal_info("mac->intrusbe               : 0x%X\r\n", &mac->intrusbe);
    log_hal_info("mac->frame                  : 0x%X\r\n", &mac->frame);
    log_hal_info("mac->index                  : 0x%X\r\n", &mac->index);
    log_hal_info("mac->testmode               : 0x%X\r\n", &mac->testmode);
    log_hal_info("mac->txmap                  : 0x%X\r\n", &mac->txmap);
    log_hal_info("mac->txcsr                  : 0x%X\r\n", &mac->txcsr);
    log_hal_info("mac->rxmap                  : 0x%X\r\n", &mac->rxmap);
    log_hal_info("mac->rxcsr                  : 0x%X\r\n", &mac->rxcsr);
    log_hal_info("mac->rxcount                : 0x%X\r\n", &mac->rxcount);
    log_hal_info("mac->txtype                 : 0x%X\r\n", &mac->txtype);
    log_hal_info("mac->txinterval             : 0x%X\r\n", &mac->txinterval);
    log_hal_info("mac->rxtype                 : 0x%X\r\n", &mac->rxtype);
    log_hal_info("mac->rxinterval             : 0x%X\r\n", &mac->rxinterval);
    log_hal_info("mac->fifosize               : 0x%X\r\n", &mac->fifosize);
    log_hal_info("mac->fifo0                  : 0x%X\r\n", &mac->fifo0);
    log_hal_info("mac->fifo1                  : 0x%X\r\n", &mac->fifo1);
    log_hal_info("mac->devctl                 : 0x%X\r\n", &mac->devctl);
    log_hal_info("mac->pwrupcnt               : 0x%X\r\n", &mac->pwrupcnt);
    log_hal_info("mac->txfifosz               : 0x%X\r\n", &mac->txfifosz);
    log_hal_info("mac->rxfifosz               : 0x%X\r\n", &mac->rxfifosz);
    log_hal_info("mac->txfifoadd              : 0x%X\r\n", &mac->txfifoadd);
    log_hal_info("mac->rxfifoadd              : 0x%X\r\n", &mac->rxfifoadd);
    log_hal_info("mac->hwcaps		              : 0x%X\r\n", &mac->hwcaps);
    log_hal_info("mac->hwsvers	              : 0x%X\r\n", &mac->hwsvers);
    log_hal_info("mac->busperf1	              : 0x%X\r\n", &mac->busperf1);
    log_hal_info("mac->busperf2	              : 0x%X\r\n", &mac->busperf2);
    log_hal_info("mac->busperf3	              : 0x%X\r\n", &mac->busperf3);
    log_hal_info("mac->epinfo		              : 0x%X\r\n", &mac->epinfo);
    log_hal_info("mac->raminfo	              : 0x%X\r\n", &mac->raminfo);
    log_hal_info("mac->linkinfo	              : 0x%X\r\n", &mac->linkinfo);
    log_hal_info("mac->vplen		              : 0x%X\r\n", &mac->vplen);
    log_hal_info("mac->hs_eof1	              : 0x%X\r\n", &mac->hs_eof1);
    log_hal_info("mac->fs_eof1	              : 0x%X\r\n", &mac->fs_eof1);
    log_hal_info("mac->ls_eof1	              : 0x%X\r\n", &mac->ls_eof1);
    log_hal_info("mac->rst_info	              : 0x%X\r\n", &mac->rst_info);
    log_hal_info("mac->rxtog		              : 0x%X\r\n", &mac->rxtog);
    log_hal_info("mac->rxtogen	              : 0x%X\r\n", &mac->rxtogen);
    log_hal_info("mac->txtog		              : 0x%X\r\n", &mac->txtog);
    log_hal_info("mac->txtogen	              : 0x%X\r\n", &mac->txtogen);
    log_hal_info("mac->usb_l1ints             : 0x%X\r\n", &mac->usb_l1ints);
    log_hal_info("mac->usb_l1intm             : 0x%X\r\n", &mac->usb_l1intm);
    log_hal_info("mac->usb_l1intp             : 0x%X\r\n", &mac->usb_l1intp);
    log_hal_info("mac->usb_l1intc             : 0x%X\r\n", &mac->usb_l1intc);
    log_hal_info("mac->csr0		                : 0x%X\r\n", &mac->csr0);
    log_hal_info("mac->count0		              : 0x%X\r\n", &mac->count0);
    log_hal_info("mac->type0		              : 0x%X\r\n", &mac->type0);
    log_hal_info("mac->naklimt0	              : 0x%X\r\n", &mac->naklimt0);
    log_hal_info("mac->sramconfigsize	        : 0x%X\r\n", &mac->sramconfigsize);
    log_hal_info("mac->hbconfigdata           : 0x%X\r\n", &mac->hbconfigdata);
    log_hal_info("mac->configdata	            : 0x%X\r\n", &mac->configdata);
    log_hal_info("mac->tx1map		              : 0x%X\r\n", &mac->tx1map);
    log_hal_info("mac->tx1csr		              : 0x%X\r\n", &mac->tx1csr);
    log_hal_info("mac->rx1map		              : 0x%X\r\n", &mac->rx1map);
    log_hal_info("mac->rx1csr		              : 0x%X\r\n", &mac->rx1csr);
    log_hal_info("mac->rx1count	              : 0x%X\r\n", &mac->rx1count);
    log_hal_info("mac->tx1type	              : 0x%X\r\n", &mac->tx1type);
    log_hal_info("mac->tx1interval	          : 0x%X\r\n", &mac->tx1interval);
    log_hal_info("mac->rx1type	              : 0x%X\r\n", &mac->rx1type);
    log_hal_info("mac->rx1interval	          : 0x%X\r\n", &mac->rx1interval);
    log_hal_info("mac->fifosize1	            : 0x%X\r\n", &mac->fifosize1);
    log_hal_info("mac->dma_intr_status        : 0x%X\r\n", &mac->dma_intr_status);
    log_hal_info("mac->dma_intr_unmask        : 0x%X\r\n", &mac->dma_intr_unmask);
    log_hal_info("mac->dma_intr_unmask_clear  : 0x%X\r\n", &mac->dma_intr_unmask_clear);
    log_hal_info("mac->dma_intr_unmask_set    : 0x%X\r\n", &mac->dma_intr_unmask_set);
    log_hal_info("mac->dma_cntl_0		          : 0x%X\r\n", &mac->dma_cntl_0);
    log_hal_info("mac->dma_addr_0	            : 0x%X\r\n", &mac->dma_addr_0);
    log_hal_info("mac->dma_count_0	          : 0x%X\r\n", &mac->dma_count_0);
    log_hal_info("mac->dma_limiter	          : 0x%X\r\n", &mac->dma_limiter);
    log_hal_info("mac->dma_config	            : 0x%X\r\n", &mac->dma_config);
    log_hal_info("mac->ep1rxpktcount	        : 0x%X\r\n", &mac->ep1rxpktcount);
    log_hal_info("mac->t0funcaddr	            : 0x%X\r\n", &mac->t0funcaddr);
    log_hal_info("mac->t0hubaddr	            : 0x%X\r\n", &mac->t0hubaddr);
    log_hal_info("mac->t1funcaddr	            : 0x%X\r\n", &mac->t1funcaddr);
    log_hal_info("mac->t1hubaddr	            : 0x%X\r\n", &mac->t1hubaddr);
    log_hal_info("mac->r1funcaddr	            : 0x%X\r\n", &mac->r1funcaddr);
    log_hal_info("mac->r1hubaddr	            : 0x%X\r\n", &mac->r1hubaddr);
    log_hal_info("mac->tm1		                : 0x%X\r\n", &mac->tm1);
    log_hal_info("mac->hwver_date             : 0x%X\r\n", &mac->hwver_date);
    log_hal_info("mac->srama	                : 0x%X\r\n", &mac->srama);
    log_hal_info("mac->sramd	                : 0x%X\r\n", &mac->sramd);
    log_hal_info("mac->risc_size	            : 0x%X\r\n", &mac->risc_size);
    log_hal_info("mac->resreg		              : 0x%X\r\n", &mac->resreg);
    log_hal_info("mac->otg20_csrl             : 0x%X\r\n", &mac->otg20_csrl);
    log_hal_info("mac->otg20_csrh             : 0x%X\r\n", &mac->otg20_csrh);


    log_hal_info("phy->u2phyac0   : 0x%X\r\n",   &phy->u2phyac0);
    log_hal_info("phy->u2phyac1   : 0x%X\r\n",   &phy->u2phyac1);
    log_hal_info("phy->u2phyac2   : 0x%X\r\n",   &phy->u2phyac2);
    log_hal_info("phy->u2phyacr0  : 0x%X\r\n",   &phy->u2phyacr0);
    log_hal_info("phy->u2phyacr1  : 0x%X\r\n",   &phy->u2phyacr1);
    log_hal_info("phy->u2phyacr2  : 0x%X\r\n",   &phy->u2phyacr2);
    log_hal_info("phy->u2phyacr3  : 0x%X\r\n",   &phy->u2phyacr3);
    log_hal_info("phy->u2phyacr4  : 0x%X\r\n",   &phy->u2phyacr4);
    log_hal_info("phy->u2phydcr0  : 0x%X\r\n",   &phy->u2phydcr0);
    log_hal_info("phy->u2phydcr1  : 0x%X\r\n",   &phy->u2phydcr1);
    log_hal_info("phy->u2phydtm0  : 0x%X\r\n",   &phy->u2phydtm0);
    log_hal_info("phy->u2phydtm1  : 0x%X\r\n",   &phy->u2phydtm1);
    log_hal_info("phy->u2phydmon0 : 0x%X\r\n",   &phy->u2phydmon0);
    log_hal_info("phy->u2phydmon1 : 0x%X\r\n",   &phy->u2phydmon1);
    log_hal_info("phy->u2phydmon2 : 0x%X\r\n",   &phy->u2phydmon2);
    log_hal_info("phy->u1phycr0   : 0x%X\r\n",   &phy->u1phycr0);
    log_hal_info("phy->u1phycr1   : 0x%X\r\n",   &phy->u1phycr1);
    log_hal_info("phy->u1phycr2   : 0x%X\r\n",   &phy->u1phycr2);
    log_hal_info("phy->regfppc    : 0x%X\r\n",   &phy->regfppc);
    log_hal_info("phy->versionc   : 0x%X\r\n",   &phy->versionc);
    log_hal_info("phy->regfcom    : 0x%X\r\n",   &phy->regfcom);
    log_hal_info("phy->fmcr0      : 0x%X\r\n",   &phy->fmcr0);
}
#endif
/************************************************************
	DMA utilities
*************************************************************/
static uint8_t usb_get_dma_channel_num(uint8_t ep_num, hal_usb_endpoint_direction_t direction)
{
    uint8_t  dma_chan;
    dma_chan = g_UsbDrvInfo.dma_port[direction][ep_num - 1];

    if ((dma_chan == 0) || (dma_chan > HAL_USB_MAX_NUMBER_DMA) || (ep_num == 0)) {
        log_hal_error("ASSERT\r\n");
    }

    return dma_chan;
}

#if 0
/* Stop DMA channel */
static void usb_hw_stop_dma_channel(uint32_t ep_num, hal_usb_endpoint_direction_t direction)
{
    uint32_t 	savedMask;
    uint8_t 	dma_chan;

	if(g_UsbDrvInfo.dma_port[ep_num - 1] != 0)
	{
		if (DMA_CheckRunStat(g_UsbDrvInfo.dma_port[ep_num - 1]))
		{
			DMA_Stop(g_UsbDrvInfo.dma_port[ep_num - 1]);
		}

		savedMask = SaveAndSetIRQMask();
		if (DMA_CheckITStat(g_UsbDrvInfo.dma_port[ep_num - 1]))
		{
			DMA_ACKI(g_UsbDrvInfo.dma_port[ep_num - 1]);
			//IRQClearInt(IRQ_DMA_CODE);
		}
		RestoreIRQMask(savedMask);
	}
    g_UsbDrvInfo.dma_pktrdy[dma_chan - 1] = false;
    g_UsbDrvInfo.dma_running[dma_chan - 1] = false;
}
#endif	

/************************************************************
	driver debug utility
*************************************************************/

void USB_Read_TX_Interrupt_Status(void)
{ //EP0 handler receive another Setup packet,need to clear EP0 interrupt
	g_UsbDrvInfo.IntrTx = DRV_Reg8(&musb->intrin1); 
	g_UsbDrvInfo.IntrTx &= ~USB_INTRIN1_EP0;
}


static void usb_ep_check(uint32_t ep_num, hal_usb_endpoint_direction_t direction, uint32_t line)
{
    if ((ep_num == 0) || ((direction == HAL_USB_EP_DIRECTION_TX) && (ep_num > HAL_USB_MAX_NUMBER_ENDPOINT_TX)) ||
            ((direction == HAL_USB_EP_DIRECTION_RX) && (ep_num > HAL_USB_MAX_NUMBER_ENDPOINT_RX))) {
        log_hal_info("ASSERT\r\n");
    }
}

#if 0
static void usb_ep_dma_running_check(uint32_t ep_num, hal_usb_endpoint_direction_t direction, uint32_t line)
{
#if 0

    uint8_t	dma_chan;

    dma_chan = g_UsbDrvInfo.dma_port[direction][ep_num - 1];

	if((dma_chan != 0)&&(DMA_CheckRunStat(g_UsbDrvInfo.dma_port[direction][ep_num-1]) != 0)&&(g_UsbDrvInfo.dma_port[direction][ep_num-1] != 0))
        /* Fix for USB compliance test program */
        usb_hw_stop_dma_channel(ep_num, direction);
    }
#endif
}
#endif

static void usb_ep0en(void)
{
    /* Default address is from 0 to 63  */   
	USB_DRV_SetBits8(&musb->intrin1e, USB_INTRIN1E_EPEN);
	
}

/*  Write data to FIFO EP */
static void usb_hw_epfifowrite(uint32_t ep_num, uint16_t nBytes, void *pSrc)
{
	uint16_t nCount = nBytes;
	uint8_t *pby;
	uint32_t nAddr;

	//log_hal_info("usb_hw_epfifowrite, nBytes:%d\r\n", nBytes);

    if ((nBytes != 0) && (pSrc == NULL)) {
        log_hal_error("ASSERT\r\n");
    }

    if (pSrc == NULL) {
        log_hal_info("usb_hw_epfifowrite Error: pSrc is NULL!!\r\n");
        return;
    }

    //log_hal_dump("epfifowrite dump\n", pSrc, nBytes);


	USB_DRV_WriteReg8(&musb->index, ep_num);
	nAddr = (uint32_t)&musb->ep0 + ep_num*4;
	pby = (uint8_t *)pSrc;

	//log_hal_info("usb_hw_epfifowrite-ep0 : 0x%x\r\n", nAddr);
	/* write by byte */
	while (nCount)
	{
		USB_DRV_WriteReg8(nAddr,*pby++);
		nCount--;
	}

}

hal_usb_endpoint_state_t hal_usb_check_rx_endpoint_usage(uint32_t ep_num)
{
    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_RX, 0);

    return g_UsbDrvInfo.ep_rx_enb_state[ep_num - 1];
}

hal_usb_endpoint_state_t hal_usb_check_tx_endpoint_usage(uint32_t ep_num)
{
    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_TX, 0);

    return g_UsbDrvInfo.ep_tx_enb_state[ep_num - 1];
}

hal_usb_status_t hal_usb_configure_rx_endpoint_type(uint32_t ep_num, hal_usb_endpoint_transfer_type_t ep_type, bool b_is_use_dma)
{
    uint32_t savedMask;
    bool b_is_switch_to_dma;


    b_is_switch_to_dma = b_is_use_dma;


    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_RX, 0);
    //usb_ep_dma_running_check(ep_num, HAL_USB_EP_DIRECTION_RX, 0);


	if(b_is_switch_to_dma == true)
	{
		if(g_UsbDrvInfo.ep_rx_enb_state[ep_num-1] == HAL_USB_EP_STATE_DMA)
			log_hal_error("ASSERT\r\n");

		g_UsbDrvInfo.ep_rx_enb_state[ep_num-1] = HAL_USB_EP_STATE_DMA;

		savedMask = SaveAndSetIRQMask();
		USB_DRV_ClearBits8(&musb->introut1e, (USB_INTROUT1E_EPEN<<ep_num));
		USB_DRV_WriteReg8(&musb->index, ep_num);
		// CR MAUI_00248052
		USB_DRV_WriteReg8(&musb->outcsr2, 0);
		USB_DRV_WriteReg8(&musb->outcsr2, (USB_OUTCSR2_AUTOCLEAR|USB_OUTCSR2_DMAENAB));
		RestoreIRQMask(savedMask);
	}
	else
	{
		if(g_UsbDrvInfo.ep_rx_enb_state[ep_num-1] == HAL_USB_EP_STATE_FIFO)
			log_hal_error("ASSERT\r\n");

		g_UsbDrvInfo.ep_rx_enb_state[ep_num-1] = HAL_USB_EP_STATE_FIFO;

		savedMask = SaveAndSetIRQMask();
		USB_DRV_WriteReg8(&musb->index, ep_num);

		/* When change ep state from DMA to FIFO, must make sure that FIFO is empty */
		if(USB_DRV_Reg8(&musb->outcsr1)&USB_OUTCSR1_OUTPKTRDY)
		{
			//if(USB_DRV_Reg8(&musb->OUTCOUNT1) == 0)	//jay
			if(USB_DRV_Reg8(&musb->outcount1) == 0)
			{
				USB_DRV_ClearBits8(&musb->outcsr1, USB_OUTCSR1_OUTPKTRDY);
			}
			else
				log_hal_error("ASSERT\r\n");
		}

		USB_DRV_WriteReg8(&musb->outcsr2, 0x00);
		USB_DRV_SetBits8(&musb->introut1e, (USB_INTROUT1E_EPEN<<ep_num));
		RestoreIRQMask(savedMask);
	}


    return HAL_USB_STATUS_OK;
}

hal_usb_status_t hal_usb_configure_tx_endpoint_type(uint32_t ep_num, hal_usb_endpoint_transfer_type_t ep_type, bool b_is_use_dma)
{
    uint32_t savedMask;
    bool b_is_switch_to_dma;


    b_is_switch_to_dma = b_is_use_dma;


    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_TX, 0);
    //usb_ep_dma_running_check(ep_num, HAL_USB_EP_DIRECTION_TX, 0);


	if(b_is_switch_to_dma == true)
	{
		if(g_UsbDrvInfo.ep_tx_enb_state[ep_num-1] == HAL_USB_EP_STATE_DMA)
			log_hal_error("ASSERT\r\n");
		g_UsbDrvInfo.ep_tx_enb_state[ep_num-1] = HAL_USB_EP_STATE_DMA;

		savedMask = SaveAndSetIRQMask();
		USB_DRV_ClearBits8(&musb->intrin1e, (USB_INTRIN1E_EPEN << ep_num));
		USB_DRV_WriteReg8(&musb->index, ep_num);
		// CR MAUI_00248052
		USB_DRV_WriteReg8(&musb->incsr2, 0);
		USB_DRV_WriteReg8(&musb->incsr2, (USB_INCSR2_AUTOSET|USB_INCSR2_DMAENAB));
		RestoreIRQMask(savedMask);
	}
	else
	{
		if(g_UsbDrvInfo.ep_tx_enb_state[ep_num-1] == HAL_USB_EP_STATE_FIFO)
			log_hal_error("ASSERT\r\n");
		g_UsbDrvInfo.ep_tx_enb_state[ep_num-1] = HAL_USB_EP_STATE_FIFO;

		savedMask = SaveAndSetIRQMask();
		USB_DRV_WriteReg8(&musb->index, ep_num);

		/* When change ep state from DMA to FIFO, must make sure that FIFO is empty */
		if(USB_DRV_Reg8(&musb->incsr1)&USB_INCSR1_INPKTRDY)
			log_hal_error("ASSERT\r\n");

		USB_DRV_WriteReg8(&musb->incsr2, 0);
		USB_DRV_SetBits8(&musb->intrin1e, (USB_INTRIN1E_EPEN << ep_num));
		RestoreIRQMask(savedMask);
	}



    return HAL_USB_STATUS_OK;
}

/************************************************************
	HISR/LISR   interrupt handler
*************************************************************/
static void usb_eint_hisr(void)
{
hal_eint_trigger_mode_t mode = HAL_EINT_LEVEL_HIGH;
hal_gpio_status_t status;
hal_gpio_data_t input_gpio_data = HAL_GPIO_DATA_LOW;


//log_hal_info("usb_eint_hisr\n");
hal_eint_mask((hal_eint_number_t)HAL_EINT_USB);

status = hal_gpio_get_input((hal_gpio_pin_t)HAL_GPIO_36, &input_gpio_data);

if(status == HAL_GPIO_STATUS_OK)
{
	if(input_gpio_data == HAL_GPIO_DATA_HIGH)
	{
		usb_cable_in = true;
		mode = HAL_EINT_LEVEL_LOW;	
		//log_hal_info("usb cable in \n");
	}
	else
	{
		usb_cable_in = false;
		mode = HAL_EINT_LEVEL_HIGH;	
		//log_hal_info("usb cable out \n");
	}
}
hal_eint_set_trigger_mode((hal_eint_number_t)HAL_EINT_USB, mode);
hal_eint_unmask((hal_eint_number_t)HAL_EINT_USB);

USB_Send_Message(USB_CABLE_MSG, NULL);

}

void usb_hisr(void)
{
#if 1

uint8_t		IntrUSB;
uint8_t	    IntrTx;
uint8_t    IntrRx;
uint32_t  ep_num;


//log_hal_error("usb_hisr\r\n");

	IntrUSB = USB_DRV_Reg8(&musb->intrusb);
	IntrTx = USB_DRV_Reg8(&musb->intrin1);
	IntrRx = USB_DRV_Reg8(&musb->introut1);

	g_UsbDrvInfo.IntrTx = 0;

   	/* Check for resume from suspend mode */
   	if (IntrUSB & USB_INTRUSB_RESUME)
   	{
   		g_UsbDrvInfo.power_state = HAL_USB_POWER_STATE_NORMAL;
		g_UsbDrvInfo.resume_hdlr();
   	}

	/* Check for reset interrupts */
	if (IntrUSB & USB_INTRUSB_RESET)
	{
		if(g_UsbDrvInfo.reset_hdlr == NULL)
			log_hal_error("ASSERT\r\n");
		g_UsbDrvInfo.reset_hdlr();
	}

	/* Check for endpoint 0 interrupt */
	if (IntrTx&USB_INTRIN1_EP0)
	{
		if(g_UsbDrvInfo.ep0_hdlr == NULL)
			//EXT_ASSERT(0, 0, 0, 0);
			log_hal_error("ASSERT\r\n");
		g_UsbDrvInfo.ep0_hdlr();
	}

//in EP0 handler, read TX interrupt value
	if ( g_UsbDrvInfo.IntrTx & IntrTx)
 	{
		log_hal_error("ASSERT\r\n");
	}
	else
	{
		IntrTx |= g_UsbDrvInfo.IntrTx;
	}
	
	// Task read fifo to flush data in USB_Clr_RX_EP_ISR()
	if ( g_UsbDrvInfo.IntrRx & IntrRx)
 	{ 
		log_hal_error("ASSERT\r\n");
	}
	else
	{
		IntrRx |= g_UsbDrvInfo.IntrRx;
		g_UsbDrvInfo.IntrRx = 0;
	}

	/* Check for Bulk TX interrupt */
	for(ep_num = 1; ep_num <= HAL_USB_MAX_NUMBER_ENDPOINT_TX; ep_num++)
	{
		if (IntrTx & (0x01 <<ep_num))
   		{
   			if(g_UsbDrvInfo.ep_tx_hdlr[ep_num-1] != NULL)
   			{
   				g_UsbDrvInfo.ep_tx_hdlr[ep_num -1]();
   			}
   			else
   			{
				/* if EP does not support ep_hdr, its interrupt should not be triggered */
				/* Save debug info */
				USB_DRV_WriteReg8(&musb->index, ep_num);
			}
		}
	}

	/* Check for Bulk RX interrupt */
	for(ep_num = 1; ep_num <= HAL_USB_MAX_NUMBER_ENDPOINT_RX; ep_num++)
	{
		if (IntrRx & (0x01 <<ep_num))
		{
			if(g_UsbDrvInfo.ep_rx_hdlr[ep_num-1]!=NULL)
			{			
				g_UsbDrvInfo.ep_rx_hdlr[ep_num -1]();
			}
			else
			{
				/* if EP does not support ep_hdr, its interrupt should not be triggered */
				/* Save debug info */
				USB_DRV_WriteReg8(&musb->index, ep_num);
			}
		}
	}

   	/* Check for suspend mode */
   	if (IntrUSB &  USB_INTRUSB_SUSPEND)
   	{

		g_UsbDrvInfo.power_state = HAL_USB_POWER_STATE_SUSPEND;
		g_UsbDrvInfo.suspend_hdlr();

   	}

	/* Clear interrupt and unmask interrupt if application agree on it */
   	//IRQClearInt(IRQ_USB_CODE);
	g_UsbDrvInfo.is_ProUSBHISR = false;

   	if (g_UsbDrvInfo.is_unMaskUSB == true)
   		NVIC_EnableIRQ(USB_IRQn);
#else
	uint8_t    IntrUSB;
	uint8_t    IntrIn;
	uint8_t    IntrOut;
	
	IntrUSB = DRV_Reg8(&musb->intrusb);
	IntrIn = DRV_Reg8(&musb->intrin1);
	IntrOut = DRV_Reg8(&musb->introut1);

	if((IntrUSB == 0)&&(IntrIn== 0)&&(IntrOut == 0))
		return;

   	/* Check for resume from suspend mode */
   	if (IntrUSB & USB_INTRUSB_RESUME)
   	{
//   		USB_Drv_Resume();
   		g_UsbDrvInfo.power_state = HAL_USB_POWER_STATE_NORMAL;
   	}

	/* Check for reset interrupts */
	if (IntrUSB & USB_INTRUSB_RESET)
	{
		g_UsbDrvInfo.reset_hdlr();
	}

	/* Check for endpoint 0 interrupt */
	if (IntrIn&USB_INTRIN1_EP0)
	{
		g_UsbDrvInfo.ep0_hdlr();
	}

	/* Check for Bulk TX interrupt */
	#if 0
	for(ep_num = 1; ep_num <= MAX_TX_EP_NUM; ep_num++)	
	{
		if (IntrIn & (0x01 <<ep_num))
   		{
   			if(g_UsbDrvInfo.ep_in_hdlr[ep_num-1] != NULL)
   			{
   				g_UsbDrvInfo.ep_in_hdlr[ep_num -1]();
   			}
   			else
   			{
				USB_CtrlEPStall(ep_num, USB_IN_EP_TYPE, KAL_TRUE, USB_CTRL_STALL_ENTRY_4);
			}
		}
	}
	#endif

	if (IntrIn&USB_INTRIN1_EP1)
		g_UsbDrvInfo.ep_tx_hdlr[0]();
	
	/* Check for Bulk RX interrupt */
	#if 0
	for(ep_num = 1; ep_num <= MAX_RX_EP_NUM; ep_num++)
	{
		if (IntrOut & (0x01 <<ep_num))
		{
			if(g_UsbDrvInfo.ep_out_hdlr[ep_num-1]!=NULL)
			{
				g_UsbDrvInfo.ep_out_hdlr[ep_num -1]();
			}
			else
			{
				USB_CtrlEPStall(ep_num, USB_OUT_EP_TYPE, KAL_TRUE, USB_CTRL_STALL_ENTRY_4);
			}
		}
	}
	#endif

	if (IntrOut&USB_INTROUT1_EP1)
		g_UsbDrvInfo.ep_rx_hdlr[0]();
	
   	/* Check for suspend mode */
   	if (IntrUSB &  USB_INTRUSB_SUSPEND)
   	{
//   		USB_Drv_Suspend();
		g_UsbDrvInfo.power_state = HAL_USB_POWER_STATE_SUSPEND;
   	}
#endif


	
}

hal_usb_status_t hal_usb_drv_create_isr(void)
{
    hal_eint_config_t eint_config;
    hal_eint_status_t result;

    /*interrupt*/
    //hal_nvic_register_isr_handler((hal_nvic_irq_t)USB_IRQn, (hal_nvic_isr_t)usb_hisr);
    //NVIC_EnableIRQ(USB_IRQn);

    /*eint*/
	usb_cable_in = false;
    eint_config.trigger_mode = HAL_EINT_EDGE_FALLING_AND_RISING;
    eint_config.debounce_time = 500;
    hal_eint_mask(HAL_EINT_USB);
    result = hal_eint_init(HAL_EINT_USB, &eint_config);
	//log_hal_info("hal_eint_init : %d\r\n", result);
    if (result != HAL_EINT_STATUS_OK) {
        //log_hal_info("hal_eint_init fail: %d\r\n", result);
        return HAL_USB_STATUS_ERROR;
    }
    result = hal_eint_register_callback((hal_eint_number_t)HAL_EINT_USB, (hal_eint_callback_t)usb_eint_hisr, NULL);
    if (result != HAL_EINT_STATUS_OK) {
        //log_hal_info("hal_eint_register_callback fail: %d\r\n", result);
        return HAL_USB_STATUS_ERROR;
    }
    hal_eint_unmask(HAL_EINT_USB);

    return HAL_USB_STATUS_OK;
}

/* enable system global interrupt*/
static void usb_en_sys_intr(void)
{
	USB_DRV_WriteReg8(&musb->intrin1e, 0);
	USB_DRV_WriteReg8(&musb->introut1e, 0);
	USB_DRV_WriteReg8(&musb->intrusbe, 0);
	USB_DRV_WriteReg8(&musb->intrusbe, (USB_INTRUSBE_SUSPEND|USB_INTRUSBE_RESUME|USB_INTRUSBE_RESET));
}

#if 0
/* EP TX data prepared ready, set ready bit */
static void usb_hw_ep_tx_ready(uint32_t ep_num, hal_usb_endpoint_transfer_type_t ep_type)
{
    uint32_t savedMask;

    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_TX, 0);
    //usb_ep_dma_running_check(ep_num, HAL_USB_EP_DIRECTION_TX, 0);

	savedMask = SaveAndSetIRQMask();
	USB_DRV_WriteReg8(&musb->index, ep_num);
	USB_DRV_WriteReg8(&musb->incsr1, USB_INCSR1_INPKTRDY);
	RestoreIRQMask(savedMask);
}


/* EP RX data already read out, clear the data */
static void usb_hw_ep_rx_ready(uint32_t ep_num)
{
    uint32_t savedMask;
    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_RX, 0);

	savedMask = SaveAndSetIRQMask();
	USB_DRV_WriteReg8(&musb->index, ep_num);

	if((USB_DRV_Reg8(&musb->outcsr1)&USB_OUTCSR1_OUTPKTRDY) == 0)  // check RX_PktReady bit
	{
		log_hal_error("ASSERT\r\n");
	}

	USB_DRV_ClearBits8(&musb->outcsr1, USB_OUTCSR1_OUTPKTRDY);
	RestoreIRQMask(savedMask);
}
#endif

/* Get status. See if ep in fifo is empty.
   If false, it means some data in fifo still wait to send out */
static bool usb_hw_is_ep_tx_empty(uint32_t ep_num)
{
    uint32_t savedMask;
	uint8_t	 byCSR;

    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_TX, 0);

	savedMask = SaveAndSetIRQMask();
	USB_DRV_WriteReg8(&musb->index, ep_num);
	byCSR = USB_DRV_Reg8(&musb->incsr1);
	RestoreIRQMask(savedMask);

	if(byCSR&USB_INCSR1_FIFONOTEMPTY)
		return  false;
	else
		return  true;

}


/************************************************************
	DMA control functions
*************************************************************/

/* DMA callback for endpoint 1*/
static void USB_DMA1_Callback(pdma_event_t event, void *user_data)
{
	hal_usb_dma_handler_t callback = g_UsbDrvInfo.dma_callback[0];

	//log_hal_error("USB_DMA1_Callback\r\n");

	g_UsbDrvInfo.dma_callback[0] = NULL;

	/* The last one pkt length not multiple of MAX_PKT_LEN, clear corresponding bits*/
	if(g_UsbDrvInfo.dma_pktrdy[0] == true)
	{
		if(g_UsbDrvInfo.dma_dir[0] == HAL_USB_EP_DIRECTION_TX)
		{
			hal_usb_set_endpoint_tx_ready(1);
		}
		else
		{
			hal_usb_set_endpoint_rx_ready(1);
		}
	}

	g_UsbDrvInfo.dma_pktrdy[0] = false;

	/* If application callback function does not request to clear drv_running set by itself, clear running state here */
	if(g_UsbDrvInfo.dma_callback_upd_run[0] == false)
		g_UsbDrvInfo.dma_running[0] = false;

	if(callback != NULL)
		callback();
		
}

/* DMA callback for endpoint 2*/
static void USB_DMA2_Callback(pdma_event_t event, void *user_data)
{
	hal_usb_dma_handler_t callback = g_UsbDrvInfo.dma_callback[1];

	//log_hal_error("USB_DMA2_Callback\r\n");

	g_UsbDrvInfo.dma_callback[1] = NULL;

	/* The last one pkt length not multiple of MAX_PKT_LEN, clear corresponding bits*/
	if (g_UsbDrvInfo.dma_pktrdy[1] == true)
	{
		if(g_UsbDrvInfo.dma_dir[1] == HAL_USB_EP_DIRECTION_TX)
		{			
			hal_usb_set_endpoint_tx_ready(2);
		}
		else
		{
			hal_usb_set_endpoint_rx_ready(2);
		}
	}

	g_UsbDrvInfo.dma_pktrdy[1] = false;

	/* if application callback function dose not request to clear drv_runnung set by itself, clear running state here*/
	if(g_UsbDrvInfo.dma_callback_upd_run[1] == false)
		g_UsbDrvInfo.dma_running[1] = false;

	if(callback != NULL)
		callback();
		
}

#if 0
static void usb_dma_callback_func(uint8_t dma_chan)
{
    hal_usb_dma_handler_t callback = g_UsbDrvInfo.dma_callback[dma_chan - 1];
    uint8_t ep_num;

    if (dma_chan == 0) {
        log_hal_error("ASSERT\r\n");
    }

    g_UsbDrvInfo.dma_callback[dma_chan - 1] = NULL;

    if (g_UsbDrvInfo.dma_dir[dma_chan - 1] == HAL_USB_EP_DIRECTION_TX) {
        ep_num = g_UsbDrvInfo.dma_tx_ep_num[dma_chan - 1];
    } else {
        ep_num = g_UsbDrvInfo.dma_rx_ep_num[dma_chan - 1];
    }


    if (usb_check_dma_time_out(dma_chan) == false) {
        /* ep0 do not do this */
        if (g_UsbDrvInfo.dma_pktrdy[dma_chan - 1] == true) {
            if (g_UsbDrvInfo.dma_dir[dma_chan - 1] == HAL_USB_EP_DIRECTION_TX) {
                usb_hw_ep_tx_ready(ep_num, HAL_USB_EP_TRANSFER_BULK);

            } else if (g_UsbDrvInfo.dma_dir[dma_chan - 1] == HAL_USB_EP_DIRECTION_RX) {
                usb_hw_ep_rx_ready(ep_num);
            }
        }
    }

    g_UsbDrvInfo.dma_pktrdy[dma_chan - 1] = false;

    /* if application callback function does not request to clear drv_running set by itself, clear running state here*/
    if (g_UsbDrvInfo.dma_callback_upd_run[dma_chan - 1] == false) {
        g_UsbDrvInfo.dma_running[dma_chan - 1] = false;
    }

    if (callback != NULL) {
        callback();
    }
}


#ifdef  __DMA_UNKNOWN_RX__
static void usb_enable_dma_timer_count(uint8_t dma_chan, bool enable, uint8_t timer_ticks)
{
    if (enable == true) {
        USB_DRV_WriteReg(USB_DMA_TIMER(dma_chan), USB_DMA_TIMER_ENTIMER | (timer_ticks & USB_DMA_TIMER_TIMEOUT_MASK));
    } else {
        // Disable DMA timer
        USB_DRV_WriteReg(USB_DMA_TIMER(dma_chan), 0);
    }
}
#endif

static bool usb_check_dma_time_out(uint8_t dma_chan)
{
    if ((USB_DRV_Reg(USB_DMA_TIMER(dma_chan))&USB_DMA_TIMER_TIMEOUT_STATUS) != 0) {
        return true;
    } else {
        return false;
    }
}
#endif
/*******************************************************************************
	USB Power Down Related
 *******************************************************************************/
static hal_usb_status_t hal_usb_pdn_mode(bool pdn_en)
{
    if (pdn_en == true) {
        /* Power down */
        NVIC_DisableIRQ(USB_IRQn);

		USB_DRV_Reg8(&musb->dummy);
		hal_usbphy_save_current();

    } else {
        /* Enable sequence: 1. UPLL, 2.PDN_USB(USB power), 3.USB(USB register), 4.GPIO(D+) */
        usb_pdn_disable();

		/* Reset 1.1 Mac*/
		USB_DRV_Reg8(&musb->dummy);

		// clear interrupt before previous power down PDN, the interrupts are read then clear
		hal_usbphy_recover();
		hal_gpt_delay_ms(2);

    }
    return HAL_USB_STATUS_OK;
}

/*******************************************************************************
	USB SLT/DVT related
 *******************************************************************************/
bool usb_dma_test_ep0_loopback(uint8_t *tx_buf, uint8_t *rx_buf)
{

    bool cmp_result = true;
#if 0

    uint16_t dma_ctrl;
    uint8_t dma_chan = 2;
    static uint32_t dma_burst_mode = 0;
    uint8_t ep_num = 0;
    uint32_t i;



    // initial buffer
    for (i = 0; i < 64; i++) {
        tx_buf[i] = i;
        rx_buf[i] = 0;
    }

    /* Disable INTR Setup */
    NVIC_DisableIRQ(USB_IRQn);

    /* Init Setup */
    dma_burst_mode = 0;
    {
        DRV_WriteReg32(USB_DMAADDR(dma_chan), tx_buf);
        DRV_WriteReg32(USB_DMACNT(dma_chan), 64);
        dma_ctrl = USB_DMACNTL_INTEN | (ep_num << 4);
        dma_ctrl |= ((dma_burst_mode & 0x03) << 9) | USB_DMACNTL_DMAEN;
        dma_ctrl |= USB_DMACNTL_DMADIR;
        DRV_WriteReg(USB_DMACNTL(dma_chan), dma_ctrl);
    }

    hal_gpt_delay_ms(20);
    if (DRV_Reg(USB_DMACNTL(dma_chan)) & 0x01) {
        log_hal_info("%s: dma fail\r\n", __func__);
        return false;
    }
    USB_DRV_WriteReg(&musb->csr0, USB_CSR0_TXPKTRDY);
    USB_DRV_WriteReg8(&musb->testmode, USB_TESTMODE_FIFOACCESS | USB_TESTMODE_FORCEHS);


    //RX
    {
        dma_ctrl = 0;
        DRV_WriteReg32(USB_DMAADDR(dma_chan), rx_buf);
        DRV_WriteReg32(USB_DMACNT(dma_chan), 64);
        dma_ctrl = USB_DMACNTL_INTEN | (ep_num << 4);
        dma_ctrl |= ((dma_burst_mode & 0x03) << 9) | USB_DMACNTL_DMAEN;
        DRV_WriteReg(USB_DMACNTL(dma_chan), dma_ctrl);
    }

    hal_gpt_delay_ms(20);
    for (i = 0; i < 64; i++) {
        if (tx_buf[i] != rx_buf[i]) {
            cmp_result = false;
            log_hal_info("%s: compare data fail\r\n", __func__);
            break;
        }
    }
    USB_DRV_WriteReg8(&musb->testmode, 0);
#endif	
    return cmp_result;
}


hal_usb_status_t hal_usb_dcm_enable(void)
{
#if 0
    /*
     * bit16: mcu dcm
     * bit18: usbip dcm
     */
    DRV_WriteReg32(&musb->resreg,  musb->resreg & (~(0x05 << 16)));
    log_hal_info("hal_usb_dcm_enable: 0x%x\r\n", musb->resreg);
#endif

    return HAL_USB_STATUS_OK;
}


/*******************************************************************************
	Control functions for USB_DRV
 *******************************************************************************/

/* Initialize usb driver SW information, called at USB_Init() */
hal_usb_status_t hal_usb_configure_driver(void)
{
    int32_t index;

    g_UsbDrvInfo.power_state = HAL_USB_POWER_STATE_NORMAL;
    g_UsbDrvInfo.is_unMaskUSB = true;
    g_UsbDrvInfo.reset_hdlr = NULL;
    g_UsbDrvInfo.ep0_hdlr = NULL;

    g_UsbDrvInfo.Is_HS_mode = false;

    for (index = (HAL_USB_MAX_NUMBER_ENDPOINT_TX - 1); index >= 0; index--) {
        g_UsbDrvInfo.ep_tx_enb_state[index] = HAL_USB_EP_STATE_DISABLE;
        g_UsbDrvInfo.ep_tx_hdlr[index] = NULL;
        g_UsbDrvInfo.ep_tx_stall_status[index] = false;
        g_UsbDrvInfo.ep_tx_flush_intr[index] = false;
    }

    for (index = (HAL_USB_MAX_NUMBER_ENDPOINT_RX - 1); index >= 0; index--) {
        g_UsbDrvInfo.ep_rx_enb_state[index] = HAL_USB_EP_STATE_DISABLE;
        g_UsbDrvInfo.ep_rx_hdlr[index] = NULL;
        g_UsbDrvInfo.ep_rx_stall_status[index] = false;
        g_UsbDrvInfo.ep_rx_flush_intr[index] = false;
    }

    return HAL_USB_STATUS_OK;
}

hal_usb_status_t hal_usb_register_driver_callback(hal_usb_driver_handler_type_t type, uint32_t ep_num, hal_usb_driver_interrupt_handler_t hdlr)
{
    switch (type) {
        case HAL_USB_DRV_HDLR_RESET:
            g_UsbDrvInfo.reset_hdlr = hdlr;
            break;
        case HAL_USB_DRV_HDLR_SUSPEND:
            g_UsbDrvInfo.suspend_hdlr = hdlr;
            break;
        case HAL_USB_DRV_HDLR_RESUME:
            g_UsbDrvInfo.resume_hdlr = hdlr;
            break;
        case HAL_USB_DRV_HDLR_EP0:
            g_UsbDrvInfo.ep0_hdlr = hdlr;
            break;
        case HAL_USB_DRV_HDLR_EP_TX:
            usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_TX, 0);
            g_UsbDrvInfo.ep_tx_hdlr[ep_num - 1] = hdlr;
            break;
        case HAL_USB_DRV_HDLR_EP_RX:
            usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_RX, 0);
            g_UsbDrvInfo.ep_rx_hdlr[ep_num - 1] = hdlr;
            break;
        default:
            log_hal_error("ASSERT\r\n");
            break;
    }

    return HAL_USB_STATUS_OK;
}


hal_usb_status_t hal_usb_reset_drv_info(void)
{
    int32_t index;

    g_UsbDrvInfo.power_state = HAL_USB_POWER_STATE_NORMAL;
    g_UsbDrvInfo.is_unMaskUSB = true;

    for (index = (HAL_USB_MAX_NUMBER_ENDPOINT_TX - 1); index >= 0; index--) {
        g_UsbDrvInfo.ep_tx_enb_state[index] = HAL_USB_EP_STATE_DISABLE;
        g_UsbDrvInfo.ep_tx_stall_status[index] = false;
    }

    for (index = (HAL_USB_MAX_NUMBER_ENDPOINT_RX - 1); index >= 0; index--) {
        g_UsbDrvInfo.ep_rx_enb_state[index] = HAL_USB_EP_STATE_DISABLE;
        g_UsbDrvInfo.ep_rx_stall_status[index] = false;
    }

    return HAL_USB_STATUS_OK;
}


bool hal_usb_get_endpoint_stall_status(uint32_t  ep_num, hal_usb_endpoint_direction_t direction)
{
    bool result;

    usb_ep_check(ep_num, direction, 0);
    if (direction == HAL_USB_EP_DIRECTION_TX) {
        result = (bool)g_UsbDrvInfo.ep_tx_stall_status[ep_num - 1];
    } else {
        result = (bool)g_UsbDrvInfo.ep_rx_stall_status[ep_num - 1];
    }

    return result;
}

hal_usb_status_t hal_usb_pull_up_dp_line(void)
{
    /* For: Clear all interrupt before DP pull high
     * Reason: Windows MTP class won't trigger new reset signal if cable plugout and re-plugin.
     *         We need a reset interrupt to reset IP else force clear all interrupt before DP pull high.
     */
    // read and clear
	USB_DRV_Reg8(&musb->intrusb);
	USB_DRV_Reg8(&musb->intrin1);
	USB_DRV_Reg8(&musb->introut1);

    /*Pull up DP here!!*/
	hal_usbphy_poweron_initialize();
	
	USB_DRV_WriteReg8(&musb->phyctl, USB_POWER_SETSUSPEND);
    g_UsbDrvInfo.usb_disconnect = false;
    log_hal_info("usb DP pull high\r\n");
    return HAL_USB_STATUS_OK;
}

hal_usb_status_t hal_usb_pull_down_dp_line(void)
{
    USB_DRV_WriteReg8(&musb->phyctl, USB_DRV_Reg8(&musb->phyctl) & (~USB_POWER_SETSUSPEND));

return HAL_USB_STATUS_OK;

}

hal_usb_status_t hal_usb_reset_hardware(void)
{
	USB_DRV_WriteReg8(&musb->power, USB_POWER_SETSUSPEND|USB_POWER_SWRSTENAB);
	NVIC_EnableIRQ(USB_IRQn);

    return HAL_USB_STATUS_OK;
}

hal_usb_status_t hal_usb_deinit(void)
{
    hal_usb_pdn_mode(true);
    usb_pdn_enable();
    return HAL_USB_STATUS_OK;
}


hal_usb_status_t hal_usb_set_address(uint8_t addr, hal_usb_set_address_state_t state)
{
    if (state == HAL_USB_SET_ADDR_DATA) {
        USB_DRV_WriteReg8(&musb->faddr, addr);
    }

    return HAL_USB_STATUS_OK;
}


hal_usb_status_t hal_usb_init_tx_endpoint(uint32_t ep_num, uint16_t data_size, hal_usb_endpoint_transfer_type_t type, bool double_fifo)
{
    uint32_t savedMask;

    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_TX, 0);

	savedMask = SaveAndSetIRQMask();
	USB_DRV_WriteReg8(&musb->index, ep_num);
	USB_DRV_WriteReg8(&musb->incsr1, (USB_INCSR1_CLRDATATOG|USB_INCSR1_FLUSHFIFO));
	USB_DRV_WriteReg8(&musb->inmaxp, (uint8_t)(data_size));
	RestoreIRQMask(savedMask);


    return HAL_USB_STATUS_OK;
}


hal_usb_status_t hal_usb_init_rx_endpoint(uint32_t ep_num, uint16_t data_size, hal_usb_endpoint_transfer_type_t type, bool double_fifo)
{
    uint32_t savedMask;

    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_RX, 0);

	savedMask = SaveAndSetIRQMask();
	USB_DRV_WriteReg8(&musb->index, ep_num);
	USB_DRV_WriteReg8(&musb->outcsr1, (USB_OUTCSR1_CLRDATATOG|USB_OUTCSR1_FLUSHFIFO));
	USB_DRV_WriteReg8(&musb->outmaxp, (uint8_t)(data_size));
	RestoreIRQMask(savedMask);


    return HAL_USB_STATUS_OK;
}


hal_usb_status_t hal_usb_enable_tx_endpoint(uint32_t ep_num, hal_usb_endpoint_transfer_type_t ep_type, hal_usb_dma_usage_t dma_usage_type, bool is_flush)
{
    uint32_t savedMask;

    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_TX, 0);
    //usb_ep_dma_running_check(ep_num, HAL_USB_EP_DIRECTION_TX, 0);

	if(dma_usage_type == HAL_USB_EP_USE_ONLY_DMA)
	{
		g_UsbDrvInfo.ep_tx_enb_state[ep_num-1] = HAL_USB_EP_STATE_DMA;

		savedMask = SaveAndSetIRQMask();
		USB_DRV_ClearBits8(&musb->intrin1e, (USB_INTRIN1E_EPEN<<ep_num));
		USB_DRV_WriteReg8(&musb->index, ep_num);

		if(is_flush == true)
		{
			USB_DRV_WriteReg8(&musb->incsr1, (USB_INCSR1_CLRDATATOG|USB_INCSR1_FLUSHFIFO));
			USB_DRV_WriteReg8(&musb->incsr1, (USB_INCSR1_CLRDATATOG|USB_INCSR1_FLUSHFIFO));
		}

		// CR MAUI_00248052
		USB_DRV_WriteReg8(&musb->incsr2, 0);
		USB_DRV_WriteReg8(&musb->incsr2, (USB_INCSR2_AUTOSET|USB_INCSR2_DMAENAB));
		RestoreIRQMask(savedMask);
	}
	else if(dma_usage_type == HAL_USB_EP_USE_NO_DMA)
	{
		/* EP default uses FIFO */
		g_UsbDrvInfo.ep_tx_enb_state[ep_num-1] = HAL_USB_EP_STATE_FIFO;

		savedMask = SaveAndSetIRQMask();
		USB_DRV_WriteReg8(&musb->index, ep_num);

		if(is_flush == true)
		{
			USB_DRV_WriteReg8(&musb->incsr1, (USB_INCSR1_CLRDATATOG | USB_INCSR1_FLUSHFIFO));
			USB_DRV_WriteReg8(&musb->incsr1, (USB_INCSR1_CLRDATATOG | USB_INCSR1_FLUSHFIFO));
		}

		USB_DRV_WriteReg8(&musb->incsr2, 0);
		USB_DRV_SetBits8(&musb->intrin1e, USB_INTRIN1E_EPEN<<ep_num);
		RestoreIRQMask(savedMask);
	}
	
    return HAL_USB_STATUS_OK;
}


hal_usb_status_t hal_usb_disable_tx_endpoint(uint32_t ep_num)
{
    uint32_t savedMask;

    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_TX, 0);
    //usb_ep_dma_running_check(ep_num, HAL_USB_EP_DIRECTION_TX, 0);

    g_UsbDrvInfo.ep_tx_enb_state[ep_num - 1] = HAL_USB_EP_STATE_DISABLE;

	savedMask = SaveAndSetIRQMask();
	USB_DRV_ClearBits8(&musb->intrin1e, (USB_INTRIN1E_EPEN<<ep_num));
	USB_DRV_WriteReg8(&musb->index, ep_num);
	USB_DRV_WriteReg8(&musb->incsr2, 0);
	RestoreIRQMask(savedMask);

    return HAL_USB_STATUS_OK;
}


hal_usb_status_t hal_usb_enable_rx_endpoint(uint32_t ep_num, hal_usb_endpoint_transfer_type_t ep_type, hal_usb_dma_usage_t dma_usage_type, bool is_flush)
{
    uint32_t savedMask;

    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_RX, 0);
    //usb_ep_dma_running_check(ep_num, HAL_USB_EP_DIRECTION_RX, 0);

	if(dma_usage_type == HAL_USB_EP_USE_ONLY_DMA)
	{
		g_UsbDrvInfo.ep_rx_enb_state[ep_num-1] = HAL_USB_EP_STATE_DMA;

		savedMask = SaveAndSetIRQMask();
		USB_DRV_ClearBits8(&musb->introut1e, (USB_INTROUT1E_EPEN<<ep_num));
		USB_DRV_WriteReg8(&musb->index, ep_num);

		if(is_flush == true)
		{
			USB_DRV_WriteReg8(&musb->outcsr1, (USB_OUTCSR1_CLRDATATOG|USB_OUTCSR1_FLUSHFIFO));
			USB_DRV_WriteReg8(&musb->outcsr1, (USB_OUTCSR1_CLRDATATOG|USB_OUTCSR1_FLUSHFIFO));
		}

		// CR MAUI_00248052
		USB_DRV_WriteReg8(&musb->outcsr2, 0);
		USB_DRV_WriteReg8(&musb->outcsr2, (USB_OUTCSR2_AUTOCLEAR|USB_OUTCSR2_DMAENAB));
		RestoreIRQMask(savedMask);
	}
	else if(dma_usage_type == HAL_USB_EP_USE_NO_DMA)
	{
		/* EP default uses FIFO */
		g_UsbDrvInfo.ep_rx_enb_state[ep_num-1] = HAL_USB_EP_STATE_FIFO;

		savedMask = SaveAndSetIRQMask();
		USB_DRV_WriteReg8(&musb->index, ep_num);

		if(is_flush == true)
		{
			USB_DRV_WriteReg8(&musb->outcsr1, (USB_OUTCSR1_CLRDATATOG|USB_OUTCSR1_FLUSHFIFO));
			USB_DRV_WriteReg8(&musb->outcsr1, (USB_OUTCSR1_CLRDATATOG|USB_OUTCSR1_FLUSHFIFO));
		}

		USB_DRV_WriteReg8(&musb->outcsr2, 0);
		USB_DRV_SetBits8(&musb->introut1e, (USB_INTROUT1E_EPEN<<ep_num));
		RestoreIRQMask(savedMask);
	}


	
    return HAL_USB_STATUS_OK;
}


hal_usb_status_t hal_usb_disable_rx_endpoint(uint32_t ep_num)
{
    uint32_t savedMask;

    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_RX, 0);
    //usb_ep_dma_running_check(ep_num, HAL_USB_EP_DIRECTION_RX, 0);

    g_UsbDrvInfo.ep_rx_enb_state[ep_num - 1] = HAL_USB_EP_STATE_DISABLE;

	savedMask = SaveAndSetIRQMask();
	USB_DRV_ClearBits8(&musb->introut1e, (USB_INTROUT1E_EPEN << ep_num));
	USB_DRV_WriteReg8(&musb->index, ep_num);
	USB_DRV_WriteReg8(&musb->outcsr2, 0);
	RestoreIRQMask(savedMask);

    return HAL_USB_STATUS_OK;
}


hal_usb_status_t hal_usb_clear_tx_endpoint_data_toggle(uint32_t ep_num)
{
    uint32_t savedMask;

    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_TX, 0);
    //usb_ep_dma_running_check(ep_num, HAL_USB_EP_DIRECTION_TX, 0);

	savedMask = SaveAndSetIRQMask();
	USB_DRV_WriteReg8(&musb->index, ep_num);
	USB_DRV_SetBits8(&musb->incsr1, USB_INCSR1_CLRDATATOG);
	RestoreIRQMask(savedMask);

    return HAL_USB_STATUS_OK;
}


hal_usb_status_t hal_usb_clear_rx_endpoint_data_toggle(uint32_t ep_num)
{
    uint32_t savedMask;

    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_RX, 0);

	savedMask = SaveAndSetIRQMask();
	USB_DRV_WriteReg8(&musb->index, ep_num);
	USB_DRV_SetBits8(&musb->outcsr1, USB_OUTCSR1_CLRDATATOG);
	RestoreIRQMask(savedMask);

    return HAL_USB_STATUS_OK;
}

hal_usb_status_t hal_usb_read_endpoint_fifo(uint32_t ep_num, uint16_t nBytes, void *pDst)
{
    uint32_t  nAddr;
    uint16_t  nCount;
    uint8_t   *pby;

    nCount = nBytes;

    if ((nBytes != 0) && (pDst == NULL)) {
        log_hal_error("ASSERT\r\n");
    }

    if (pDst == NULL) {
        return HAL_USB_STATUS_ERROR;
    }

	USB_DRV_WriteReg8(&musb->index, ep_num);
	nAddr = (uint32_t)&musb->ep0 + ep_num*4;
	pby = (uint8_t *)pDst;

	/* read byte by byte */
	while (nCount)
	{
		*pby++ = USB_DRV_Reg8(nAddr);
		nCount--;
	}

    //fifo dump
    //if(nBytes>10)
    //    log_hal_dump("hal_usb_fifo dump\n", pDst, nBytes);

    return HAL_USB_STATUS_OK;
}


hal_usb_status_t hal_usb_write_endpoint_fifo(uint32_t ep_num, uint16_t nBytes, void *pSrc)
{
    usb_hw_epfifowrite(ep_num, nBytes, pSrc);
    return HAL_USB_STATUS_OK;
}


hal_usb_status_t hal_usb_set_endpoint_stall(uint32_t ep_num, hal_usb_endpoint_direction_t direction, bool stall_enable)
{
    uint32_t savedMask;
	uint8_t	CSR1;

    usb_ep_check(ep_num, direction, 0);
    //usb_ep_dma_running_check(ep_num, direction, 0);

	savedMask = SaveAndSetIRQMask();
	USB_DRV_WriteReg8(&musb->index, ep_num);

	if(stall_enable == true)
	{
		/* Stall endpoint */
		if (direction == HAL_USB_EP_DIRECTION_RX)
		{
			USB_DRV_SetBits8(&musb->outcsr1, (USB_OUTCSR1_SENDSTALL|USB_OUTCSR1_FLUSHFIFO|USB_OUTCSR1_CLRDATATOG));
			USB_DRV_SetBits8(&musb->outcsr1, (USB_OUTCSR1_SENDSTALL|USB_OUTCSR1_FLUSHFIFO|USB_OUTCSR1_CLRDATATOG));
			g_UsbDrvInfo.ep_rx_stall_status[ep_num-1] = true;
		}
		else
		{
			USB_DRV_SetBits8(&musb->incsr1, (USB_INCSR1_SENDSTALL|USB_INCSR1_FLUSHFIFO|USB_INCSR1_CLRDATATOG));
			USB_DRV_SetBits8(&musb->incsr1, (USB_INCSR1_SENDSTALL|USB_INCSR1_FLUSHFIFO|USB_INCSR1_CLRDATATOG));
			g_UsbDrvInfo.ep_tx_stall_status[ep_num-1] = true;
		}
	}
	else
	{
		/* Clear stall */
		if (direction == HAL_USB_EP_DIRECTION_RX)
		{
			CSR1 = USB_DRV_Reg8(&musb->outcsr1);
			CSR1 &= ~USB_OUTCSR1_SENDSTALL;
			USB_DRV_WriteReg8(&musb->outcsr1, (CSR1|USB_OUTCSR1_FLUSHFIFO|USB_OUTCSR1_CLRDATATOG));
			USB_DRV_WriteReg8(&musb->outcsr1, (CSR1|USB_OUTCSR1_FLUSHFIFO|USB_OUTCSR1_CLRDATATOG));
			g_UsbDrvInfo.ep_rx_stall_status[ep_num-1] = false;
		}
		else
		{
			CSR1 = USB_DRV_Reg8(&musb->incsr1);
			CSR1 &= ~USB_INCSR1_SENDSTALL;
			USB_DRV_WriteReg8(&musb->incsr1, (CSR1|USB_INCSR1_FLUSHFIFO|USB_INCSR1_CLRDATATOG));
			USB_DRV_WriteReg8(&musb->incsr1, (CSR1|USB_INCSR1_FLUSHFIFO|USB_INCSR1_CLRDATATOG));
			g_UsbDrvInfo.ep_tx_stall_status[ep_num-1] = false;
		}
	}

	RestoreIRQMask(savedMask);


    return HAL_USB_STATUS_OK;
}


hal_usb_status_t hal_usb_get_endpoint_0_status(bool *p_transaction_end, bool *p_sent_stall)
{
	uint8_t byCSR0;
	
	DRV_WriteReg8(&musb->index,0);
	byCSR0 = DRV_Reg8(&musb->incsr1);

	if (byCSR0 & USB_CSR0_SENTSTALL) 
		*p_sent_stall = true;
	else
		*p_sent_stall = false;
	
	if (byCSR0 & USB_CSR0_SETUPEND) 
		*p_transaction_end = true;
	else
		*p_transaction_end = false;


    return HAL_USB_STATUS_OK;
}


hal_usb_status_t hal_usb_update_endpoint_0_state(hal_usb_endpoint_0_driver_state_t state, bool stall, bool end)
{
	//log_hal_info("hal_usb_update_endpoint_0_state\r\n");

	uint8_t   reg_state;
	uint8_t   byCSR0;

	/* clear sent stall*/
	if(state ==HAL_USB_EP0_DRV_STATE_CLEAR_SENT_STALL)
	{
		DRV_WriteReg8(&musb->index,0);   
		byCSR0 = DRV_Reg8(&musb->incsr1);
		DRV_WriteReg8(&musb->incsr1, byCSR0&(~USB_CSR0_SENTSTALL));
		return HAL_USB_STATUS_OK;
	}

	/* clear transaction end*/
	if(state ==HAL_USB_EP0_DRV_STATE_TRANSACTION_END)
	{
		DRV_WriteReg8(&musb->index,0);   
		DRV_WriteReg8(&musb->incsr1, (USB_CSR0_SERVICESETUPEND));
		return HAL_USB_STATUS_OK;
	}

	/* ep0 read end or write ready*/
	if(state == HAL_USB_EP0_DRV_STATE_READ_END)
	{
		reg_state = USB_CSR0_SERVICEDOUTPKTRDY;
	}
	else
	{
		reg_state = USB_CSR0_INPKTRDY;
	}

	/* error occured, sent stall*/	
	if(stall == true)
	{
		reg_state |= USB_CSR0_SENDSTALL; 
	}
	/* last data for this transaction, set data end bit*/
	if(end == true)
	{
		reg_state |= USB_CSR0_DATAEND; 
	}
	
	DRV_WriteReg8(&musb->index,0);   
	DRV_WriteReg8(&musb->incsr1, reg_state);

    return HAL_USB_STATUS_OK;
}


uint32_t hal_usb_ep0_pkt_len(void)
{

    uint32_t savedMask;
	uint32_t nCount = 0;
    int8_t  CSR0;

	savedMask = SaveAndSetIRQMask();
	USB_DRV_WriteReg8(&musb->index, 0);
	CSR0 = USB_DRV_Reg8(&musb->incsr1); /* csr0 */

	if(CSR0 & USB_CSR0_OUTPKTRDY)
	{
		nCount = (uint32_t)USB_DRV_Reg8(&musb->outcount1); /*count0*/
	}
	RestoreIRQMask(savedMask);

    return nCount;
}

uint32_t hal_usb_ep_pkt_len(uint32_t ep_num)
{
    uint32_t savedMask;
    uint16_t CSR;
    uint32_t nCount = 0;


    //prep_ctrl_func_3 = &data->rEP_Ctrl_Func_3;
    //ep_num = (kal_uint32)prep_ctrl_func_3->u4ep_num;

    //USB_EP_Check(ep_num, USB_EP_RX_DIR, 0);

	savedMask = SaveAndSetIRQMask();
	USB_DRV_WriteReg8(&musb->index, ep_num);
	CSR = USB_DRV_Reg8(&musb->outcsr1);

	if(CSR & USB_OUTCSR1_OUTPKTRDY)
	{
		nCount = (uint32_t)USB_DRV_Reg8(&musb->outcount1);
	}
	RestoreIRQMask(savedMask);

    //prep_ctrl_func_3->u4result = (DCL_UINT32)nCount;
    return nCount;
}


hal_usb_status_t hal_usb_set_endpoint_tx_ready(uint32_t ep_num)
{
    uint32_t savedMask;

    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_TX, 0);
    //usb_ep_dma_running_check(ep_num, HAL_USB_EP_DIRECTION_TX, 0);

	savedMask = SaveAndSetIRQMask();
	USB_DRV_WriteReg8(&musb->index, ep_num);
	USB_DRV_WriteReg8(&musb->incsr1, USB_INCSR1_INPKTRDY);
	RestoreIRQMask(savedMask);


    return HAL_USB_STATUS_OK;
}


uint32_t hal_usb_get_rx_packet_length(uint32_t ep_num)
{
    uint32_t savedMask;
    uint16_t CSR;
    uint32_t nCount = 0;

    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_RX, 0);

	savedMask = SaveAndSetIRQMask();
	USB_DRV_WriteReg8(&musb->index, ep_num);
	CSR = USB_DRV_Reg8(&musb->outcsr1);

	if(CSR & USB_OUTCSR1_OUTPKTRDY)
	{
		nCount = (uint32_t)USB_DRV_Reg8(&musb->outcount1);
	}
	RestoreIRQMask(savedMask);

    return nCount;
}


hal_usb_status_t hal_usb_set_endpoint_rx_ready(uint32_t ep_num)
{
    uint32_t savedMask;

    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_RX, 0);

	savedMask = SaveAndSetIRQMask();
	USB_DRV_WriteReg8(&musb->index, ep_num);
	
	if((USB_DRV_Reg8(&musb->outcsr1)&USB_OUTCSR1_OUTPKTRDY) == 0)	// check RX_PktReady bit
	{
		log_hal_error("ASSERT\r\n");
	}
	
	USB_DRV_ClearBits8(&musb->outcsr1, USB_OUTCSR1_OUTPKTRDY);
	RestoreIRQMask(savedMask);
		

    return HAL_USB_STATUS_OK;
}


bool hal_usb_is_endpoint_tx_empty(uint32_t ep_num)
{
    return usb_hw_is_ep_tx_empty(ep_num);
}

bool hal_usb_is_endpoint_rx_empty(uint32_t ep_num)
{
    uint32_t savedMask;
    uint16_t CSR;
    bool result;

    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_RX, 0);

    savedMask = SaveAndSetIRQMask();
    USB_DRV_WriteReg8(&musb->index, ep_num);
    CSR = USB_DRV_Reg(&musb->outcsr1);
    RestoreIRQMask(savedMask);

    if (CSR & USB_OUTCSR1_OUTPKTRDY) {
        result = false;
    } else {
        result = true;
    }

    return result;
}


hal_usb_status_t hal_usb_clear_tx_endpoint_fifo(uint32_t ep_num, hal_usb_endpoint_transfer_type_t ep_type, bool b_reset_toggle)
{
    uint32_t savedMask;

    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_TX, 0);
    //usb_ep_dma_running_check(ep_num, HAL_USB_EP_DIRECTION_TX, 0);

	savedMask = SaveAndSetIRQMask();
	USB_DRV_WriteReg8(&musb->index, ep_num);

	if(b_reset_toggle == true)
	{
		USB_DRV_SetBits8(&musb->incsr1, USB_INCSR1_FLUSHFIFO|USB_INCSR1_CLRDATATOG);
		USB_DRV_SetBits8(&musb->incsr1, USB_INCSR1_FLUSHFIFO|USB_INCSR1_CLRDATATOG);
	}
	else
	{
		USB_DRV_SetBits8(&musb->incsr1, USB_INCSR1_FLUSHFIFO);
		USB_DRV_SetBits8(&musb->incsr1, USB_INCSR1_FLUSHFIFO);
	}
	RestoreIRQMask(savedMask);

    return HAL_USB_STATUS_OK;
}


hal_usb_status_t hal_usb_clear_rx_endpoint_fifo(uint32_t ep_num, hal_usb_endpoint_transfer_type_t ep_type, bool b_reset_toggle)
{
    uint32_t savedMask;

    usb_ep_check(ep_num, HAL_USB_EP_DIRECTION_RX, 0);
    //usb_ep_dma_running_check(ep_num, HAL_USB_EP_DIRECTION_RX, 0);

	savedMask = SaveAndSetIRQMask();
	USB_DRV_WriteReg8(&musb->index, ep_num);

	if(b_reset_toggle == true)
	{
		USB_DRV_SetBits8(&musb->outcsr1, USB_OUTCSR1_FLUSHFIFO|USB_OUTCSR1_CLRDATATOG);
		USB_DRV_SetBits8(&musb->outcsr1, USB_OUTCSR1_FLUSHFIFO|USB_OUTCSR1_CLRDATATOG);
	}
	else
	{
		USB_DRV_SetBits8(&musb->outcsr1, USB_OUTCSR1_FLUSHFIFO);
		USB_DRV_SetBits8(&musb->outcsr1, USB_OUTCSR1_FLUSHFIFO);
	}
	RestoreIRQMask(savedMask);
	
    return HAL_USB_STATUS_OK;
}


hal_usb_status_t hal_usb_get_dma_channel(uint32_t ep_tx_num, uint32_t ep_rx_num, hal_usb_endpoint_direction_t direction, bool same_chan)
{
#if 0

    /* Should not allow re-entry */
    static bool race_check = false;

    if (race_check == true) {
        log_hal_error("ASSERT\r\n");
    }
    race_check = true;

    g_UsbDrvInfo.dma_channel++;

    if (g_UsbDrvInfo.dma_channel > HAL_USB_MAX_NUMBER_DMA) {
        log_hal_error("ASSERT\r\n");
    }

    if (same_chan == true) {
        usb_ep_check(ep_tx_num, HAL_USB_EP_DIRECTION_TX, 0);
        usb_ep_check(ep_rx_num, HAL_USB_EP_DIRECTION_RX, 0);

        g_UsbDrvInfo.is_bidirection_dma[g_UsbDrvInfo.dma_channel - 1] = true;
        /* the same channel */
        g_UsbDrvInfo.dma_port[HAL_USB_EP_DIRECTION_TX][ep_tx_num - 1] = g_UsbDrvInfo.dma_channel;
        g_UsbDrvInfo.dma_port[HAL_USB_EP_DIRECTION_RX][ep_rx_num - 1] = g_UsbDrvInfo.dma_channel;
        g_UsbDrvInfo.dma_tx_ep_num[g_UsbDrvInfo.dma_channel - 1] = ep_tx_num;
        g_UsbDrvInfo.dma_rx_ep_num[g_UsbDrvInfo.dma_channel - 1] = ep_rx_num;
    } else {
        g_UsbDrvInfo.is_bidirection_dma[g_UsbDrvInfo.dma_channel - 1] = false;
        g_UsbDrvInfo.dma_dir[g_UsbDrvInfo.dma_channel - 1] = direction;

        if (direction == HAL_USB_EP_DIRECTION_TX) {
            usb_ep_check(ep_tx_num, HAL_USB_EP_DIRECTION_TX, 0);
            g_UsbDrvInfo.dma_port[HAL_USB_EP_DIRECTION_TX][ep_tx_num - 1] = g_UsbDrvInfo.dma_channel;
            g_UsbDrvInfo.dma_tx_ep_num[g_UsbDrvInfo.dma_channel - 1] = ep_tx_num;
        } else {
            usb_ep_check(ep_rx_num, HAL_USB_EP_DIRECTION_RX, 0);
            g_UsbDrvInfo.dma_port[HAL_USB_EP_DIRECTION_RX][ep_rx_num - 1] = g_UsbDrvInfo.dma_channel;
            g_UsbDrvInfo.dma_rx_ep_num[g_UsbDrvInfo.dma_channel - 1] = ep_rx_num;
        }
    }

    race_check = false;
//#else
		switch (ep_tx_num)
		{
		case 1:
			g_UsbDrvInfo.dma_port[ep_tx_num - 1] = DMA_GetChannel(DMA_USB1TX);
			break;
#if defined(__USB_SUPPORT_MULTIPLE_DMA_CHANNEL__)
		case 2:
			g_UsbDrvInfo.dma_port[ep_tx_num- 1] = DMA_GetChannel(DMA_USB2TX);
			break;
#endif /* __USB_SUPPORT_MULTIPLE_DMA_CHANNEL__ */
		default:
			//EXT_ASSERT(0, (uint32_t)ep_tx_num, 0 , 0);
			log_hal_error("ASSERT\r\n");
			break;
		}

#endif

    return HAL_USB_STATUS_OK;
}


hal_usb_status_t hal_usb_stop_dma_channel(uint32_t ep_num, hal_usb_endpoint_direction_t direction)
{
	
#if 0

    uint32_t 	savedMask;
    uint8_t 	dma_chan;
    volatile uint32_t delay;
    uint32_t left_count;
    bool  dma_pktrdy;

    dma_chan = usb_get_dma_channel_num(ep_num, direction);
    savedMask = SaveAndSetIRQMask();	
    /* Stop DMA channel */
    USBDMA_Stop(dma_chan);
    /* Clear pending DMA interrupts */
    DRV_WriteReg8(&musb->dma_intr_status, (1 << (dma_chan - 1)));
    //DRV_WriteReg8(&musb->dma_intr_unmask_set, (1<<(dma_chan-1)));
    dma_pktrdy = g_UsbDrvInfo.dma_pktrdy[dma_chan - 1];
    g_UsbDrvInfo.dma_pktrdy[dma_chan - 1] = false;
    g_UsbDrvInfo.dma_running[dma_chan - 1] = false;
    RestoreIRQMask(savedMask);
    for (delay = 0 ; delay < 500 ; delay++); /* wait for dma stop */

    left_count = USB_DMACNT(dma_chan); //get DMA Real CNT
    if ((left_count == 0)  || (left_count > g_UsbDrvInfo.dma_tx_length[dma_chan - 1])) {  /* check for short pkt */
        /* drop data in FIFO*/
        if (dma_pktrdy == true) {
            if (g_UsbDrvInfo.dma_dir[dma_chan - 1] == HAL_USB_EP_DIRECTION_TX) {
                savedMask = SaveAndSetIRQMask();
                USB_DRV_WriteReg8(&musb->index, ep_num);
                USB_DRV_SetBits(&musb->txcsr, USB_TXCSR_FLUSHFIFO | USB_TXCSR_TXPKTRDY);
                RestoreIRQMask(savedMask);
            }
        }
    }
//#else
	if(g_UsbDrvInfo.dma_port[ep_num - 1] != 0)
	{
		if (DMA_CheckRunStat(g_UsbDrvInfo.dma_port[ep_num - 1]))
		{
			DMA_Stop(g_UsbDrvInfo.dma_port[ep_num - 1]);
		}

		savedMask = SaveAndSetIRQMask();
		if (DMA_CheckITStat(g_UsbDrvInfo.dma_port[ep_num - 1]))
		{
			DMA_ACKI(g_UsbDrvInfo.dma_port[ep_num - 1]);
			//IRQClearInt(IRQ_DMA_CODE);
		}
		RestoreIRQMask(savedMask);
	}

	g_UsbDrvInfo.dma_pktrdy[ep_num - 1] = false;
	g_UsbDrvInfo.dma_running[ep_num - 1] = false;

#endif
    return HAL_USB_STATUS_OK;
}


hal_usb_status_t hal_usb_release_dma_channel(uint32_t ep_tx_num, uint32_t ep_rx_num, hal_usb_endpoint_direction_t direction, bool same_chan)
{
#if 0

    uint8_t dma_chan;
    if (same_chan == true) {
        usb_ep_check(ep_tx_num, HAL_USB_EP_DIRECTION_TX, 0);
        usb_ep_check(ep_rx_num, HAL_USB_EP_DIRECTION_RX, 0);

        usb_hw_stop_dma_channel(ep_tx_num, HAL_USB_EP_DIRECTION_TX);
        dma_chan = usb_get_dma_channel_num(ep_tx_num, HAL_USB_EP_DIRECTION_TX);

        if (g_UsbDrvInfo.is_bidirection_dma[dma_chan - 1] == false) {
            log_hal_error("ASSERT\r\n");
        }

        g_UsbDrvInfo.dma_tx_ep_num[dma_chan - 1] = 0;
        g_UsbDrvInfo.dma_rx_ep_num[dma_chan - 1] = 0;
        g_UsbDrvInfo.dma_port[HAL_USB_EP_DIRECTION_TX][ep_tx_num - 1] = 0;
        g_UsbDrvInfo.dma_port[HAL_USB_EP_DIRECTION_RX][ep_rx_num - 1] = 0;
    } else {
        if (direction == HAL_USB_EP_DIRECTION_TX) {
            usb_ep_check(ep_tx_num, HAL_USB_EP_DIRECTION_TX, 0);
            usb_hw_stop_dma_channel(ep_tx_num, HAL_USB_EP_DIRECTION_TX);
            dma_chan = usb_get_dma_channel_num(ep_tx_num, HAL_USB_EP_DIRECTION_TX);

            if (g_UsbDrvInfo.is_bidirection_dma[dma_chan - 1] == true) {
                log_hal_error("ASSERT\r\n");
            }

            g_UsbDrvInfo.dma_tx_ep_num[dma_chan - 1] = 0;
            g_UsbDrvInfo.dma_port[HAL_USB_EP_DIRECTION_TX][ep_tx_num - 1] = 0;
        } else {
            usb_ep_check(ep_rx_num, HAL_USB_EP_DIRECTION_RX, 0);
            usb_hw_stop_dma_channel(ep_rx_num, HAL_USB_EP_DIRECTION_RX);
            dma_chan = usb_get_dma_channel_num(ep_rx_num, HAL_USB_EP_DIRECTION_RX);

            if (g_UsbDrvInfo.is_bidirection_dma[dma_chan - 1] == true) {
                log_hal_error("ASSERT\r\n");
            }

            g_UsbDrvInfo.dma_rx_ep_num[dma_chan - 1] = 0;
            g_UsbDrvInfo.dma_port[HAL_USB_EP_DIRECTION_RX][ep_rx_num - 1] = 0;
        }
    }
//#else
	usb_ep_check(ep_tx_num, direction, 0);

	if(g_UsbDrvInfo.dma_port[ep_tx_num - 1] != 0)
	{
		usb_hw_stop_dma_channel(ep_tx_num, direction);
		DMA_FreeChannel(g_UsbDrvInfo.dma_port[ep_tx_num - 1]);
	}

	g_UsbDrvInfo.dma_port[ep_tx_num - 1] = 0;

#endif

    return HAL_USB_STATUS_OK;
}

hal_usb_status_t hal_usb_start_dma_channel(uint32_t ep_num, hal_usb_endpoint_direction_t direction, hal_usb_endpoint_transfer_type_t ep_type, void *addr, uint32_t length,
        hal_usb_dma_handler_t callback, bool callback_upd_run, hal_usb_dma_type_t dma_type)
{

		pdma_channel_t dma_master = PDMA_START_CHANNEL;
	  pdma_config_t usb_dma_config;
		pdma_callback_t callback_hdr = NULL;
		pdma_status_t status;

				//log_hal_error("hal_usb_start_dma_channel\r\n");
		
				usb_ep_check(ep_num, direction, 0);			
			
				if(length == 0)
					log_hal_error("ASSERT\r\n");					
			
				//ASSERT(g_UsbDrvInfo.dma_running[ep_num - 1]==KAL_FALSE);
				g_UsbDrvInfo.dma_running[ep_num - 1] = true;
			
				//ASSERT(g_UsbDrvInfo.dma_port[ep_num - 1] !=0);
			
				g_UsbDrvInfo.dma_callback[ep_num - 1] = callback;
				g_UsbDrvInfo.dma_callback_upd_run[ep_num - 1] = callback_upd_run;
				g_UsbDrvInfo.dma_dir[ep_num - 1] = direction;
			
			
				/* DMA_CONFIG */
				/* Single mode */
				usb_dma_config.burst_mode = false;
			
				if (ep_num == 1)
				{
					if(direction == HAL_USB_EP_DIRECTION_TX)
					{
						dma_master = PDMA_USB1_TX;
					}
					else
					{
						dma_master = PDMA_USB1_RX;
					}
			
					callback_hdr = USB_DMA1_Callback;
				}
				else if (ep_num == 2)
				{
					if(direction == HAL_USB_EP_DIRECTION_TX)
					{
						dma_master = PDMA_USB2_TX;
					}
					else
					{
						dma_master = PDMA_USB2_RX;
					}

					callback_hdr = USB_DMA2_Callback;
				}
				else
				{
					//EXT_ASSERT(0, (kal_uint32)ep_num, 0, 0);
					log_hal_error("ASSERT\r\n");
				}			
			
				/* half channel */
				if(direction == HAL_USB_EP_DIRECTION_TX)
					usb_dma_config.master_type = PDMA_TX;
				else
					usb_dma_config.master_type = PDMA_RX;			
			
				// moduler 4
				if ((length&0x3) || ((uint32_t)addr&0x3))
				{
					/* byte transfer */
					usb_dma_config.size  = PDMA_BYTE;
					usb_dma_config.count = length;
				}
				else
				{
					/* word transfer */
					usb_dma_config.size  = PDMA_WORD;
					usb_dma_config.count = length>>2;
				}
			
				/* judge if it is mutiple of max packet length*/
				if (length % HAL_USB_MAX_PACKET_SIZE_ENDPOINT_BULK_FULL_SPEED)
					g_UsbDrvInfo.dma_pktrdy[ep_num - 1] = true;
				else
					g_UsbDrvInfo.dma_pktrdy[ep_num - 1] = false;
			
				/* Configure DMA */
				//DMA_Config(g_UsbDrvInfo.dma_port[ep_num - 1], &dma_input, KAL_TRUE);

				/* config dma hardware */
				status = pdma_init(dma_master);
				if(status != PDMA_OK)
				  log_hal_error("pdma_init, status = %d\r\n", status);

				/* total dma length = size * count */
				status = pdma_configure(dma_master, &usb_dma_config);
				if(status != PDMA_OK)
				  log_hal_error("pdma_configure, status = %d\r\n", status);

				status = pdma_register_callback(dma_master, callback_hdr, NULL);				
				if(status != PDMA_OK)
				  log_hal_error("pdma_register_callback, status = %d\r\n", status);

				/* start DMA */
				status = pdma_start_interrupt(dma_master, (uint32_t)addr);
				if(status != PDMA_OK)
					log_hal_error("pdma_start_interrupt, status = %d\r\n", status);

    return HAL_USB_STATUS_OK;
}


bool hal_usb_is_dma_running(uint32_t ep_num, hal_usb_endpoint_direction_t direction)
{
    bool result;
    uint8_t   dma_chan;

    dma_chan = usb_get_dma_channel_num(ep_num, direction);
    result = g_UsbDrvInfo.dma_running[dma_chan - 1];

    return result;;
}

void hal_usb_wait_dma_idle(uint32_t ep_num)
{
    bool result;
    uint8_t   dma_chan;
    pdma_running_status_t running_status;

    if (ep_num == 1)
    {
        dma_chan = PDMA_USB1_TX;
    }
    else if (ep_num == 2)
    {
        dma_chan = PDMA_USB2_TX;
    }
    else
    {
        log_hal_error("ASSERT\r\n");
    }

    do{
        pdma_get_running_status(dma_chan, &running_status);
    }while(running_status == PDMA_BUSY);
}

bool hal_usb_is_high_speed(void)
{
    return (bool)g_UsbDrvInfo.Is_HS_mode;;
}

hal_usb_status_t hal_usb_enter_test_mode(hal_usb_test_mode_type_t test_selector)
{
#if 0
    volatile uint32_t delay = 0;

    if (test_selector == HAL_USB_TEST_MODE_TYPE_J) {
        USB_DRV_WriteReg8(&musb->testmode, USB_TESTMODE_TESTJ);
    } else if (test_selector == HAL_USB_TEST_MODE_TYPE_K) {
        USB_DRV_WriteReg8(&musb->testmode, USB_TESTMODE_TESTK);
    } else if (test_selector == HAL_USB_TEST_MODE_TYPE_SE0_NAK) {
        USB_DRV_WriteReg8(&musb->testmode, USB_TESTMODE_TESTSE0NAK);
    } else if (test_selector == HAL_USB_TEST_MODE_TYPE_PACKET) {
        uint8_t packet_test[] = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
            0xAA, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE,
            0xEE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xBF, 0xDF,
            0xEF, 0xF7, 0xFB, 0xFD, 0xFC, 0x7E, 0xBF, 0xDF,
            0xEF, 0xF7, 0xFB, 0xFD, 0x7E
        };

        usb_hw_epfifowrite(0, 53, packet_test);

        for (delay = 0; delay != 1000; delay++) ;

        USB_DRV_WriteReg8(&musb->index, 0);
        USB_DRV_WriteReg8(&musb->testmode, USB_TESTMODE_TESTPACKET);

        for (delay = 0; delay != 1000; delay++) ;

        USB_DRV_WriteReg(&musb->incsr1, USB_CSR0_TXPKTRDY); /* csr 0 */
    } else {
        log_hal_error("ASSERT\r\n");
    }
#endif	

    return HAL_USB_STATUS_OK;

}


hal_usb_status_t hal_usb_reset_fifo(void)
{
    g_FIFOadd = USB_FIFO_START_ADDRESS;
    return HAL_USB_STATUS_OK;
}

/************************************************************
	Functions that is used whether USB_ENABLE is turned on or not
*************************************************************/

hal_usb_status_t hal_usb_init(void)
{
    hal_usb_pdn_mode(false);

 
 /* software reset */
 USB_DRV_WriteReg8(&musb->rstctrl, USB_RSTCTRL_SWRST);
 USB_DRV_WriteReg8(&musb->rstctrl, 0);
 /* When mcu set SWRST, the USB_POWER register will be clear as 0 */
 USB_DRV_WriteReg8(&musb->power, (USB_POWER_SETSUSPEND|USB_POWER_SWRSTENAB));
 usb_en_sys_intr();
 usb_ep0en();	 /* enable EP0 */
 
 hal_nvic_register_isr_handler((hal_nvic_irq_t)USB_IRQn, (hal_nvic_isr_t)usb_hisr);
 //NVIC_EnableIRQ(USB_IRQn);

	//log_hal_error("hal_usb_init\r\n");

    return HAL_USB_STATUS_OK;
}


#endif /*HAL_USB_MODULE_ENABLED*/
