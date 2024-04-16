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

#ifndef __HAL_USB_INTERNAL_H__
#define __HAL_USB_INTERNAL_H__

#ifdef HAL_USB_MODULE_ENABLED

#if 1
/*USB_FADDR*/
#define USB_FADDR_ADDRMASK		(0x7f)   /*RO*/
#define USB_FADDR_UPDATE		(0x80)   /*RW*/

/*USB_POWER*/
#define USB_POWER_SETSUSPEND	(0x01)   /*RW*/
#define USB_POWER_SUSPENDSTAT	(0x02)   /*RO*/ /*Read clear by the intr. register*/
#define USB_POWER_RESUME		(0x04)   /*RW*/
#define USB_POWER_RESET			(0x08)   /*RO*/
#define USB_POWER_SWRSTENAB		(0x10)   /*RW*/
#define USB_POWER_ISOUPDATE		(0x80)   /*RW*/

/*USB_RSTCTRL*/
#define USB_RSTCTRL_SWRST		(0x80)	 /*RW*/ 

/*USB_INTRIN1, USB_INTRIN2 is not needed*/   
#define USB_INTRIN1_EP0			(0x01)   /*RO*/
#define USB_INTRIN1_EP1			(0x02)   /*RO*/
#define USB_INTRIN1_EP2			(0x04)   /*RO*/
#define USB_INTRIN1_EP3			(0x08)   /*RO*/

/*USB_INTROUT1, USB_INTROUT2 is not needed*/
#define USB_INTROUT1_EP1		(0x02)   /*RO*/
#define USB_INTROUT1_EP2		(0x04)   /*RO*/

/*USB_INTRUSB*/
#define USB_INTRUSB_SUSPEND		(0x01)   /*RO*/
#define USB_INTRUSB_RESUME		(0x02)   /*RO*/
#define USB_INTRUSB_RESET		(0x04)   /*RO*/
#define USB_INTRUSB_SOF			(0x08)   /*RO*/

/*USB_INTRIN1E, USB_INTRIN2E is not needed*/
#define USB_INTRIN1E_EPEN		(0x01)   /*RW*/

/*USB_INTROUT1E, USB_INTROUT2E is not needed*/
#define USB_INTROUT1E_EPEN		(0x01)   /*RW*/

/*USB_INTRUSBE*/
#define USB_INTRUSBE_SUSPEND	(0x01)   /*RW*/
#define USB_INTRUSBE_RESUME		(0x02)   /*RW*/
#define USB_INTRUSBE_RESET		(0x04)   /*RW*/
#define USB_INTRUSBE_SOF		(0x08)   /*RW*/

/*USB_INMAXP*/
#define USB_INMAXP_MASK			(0xff)   /*RW*/

/*USB_OUTMAXP*/
#define USB_OUTMAXP_MASK		(0xff)   /*RW*/

/*USB_CSR0*/
#define USB_CSR0_OUTPKTRDY				(0x01)   /*RO*/
#define USB_CSR0_INPKTRDY				(0x02)   /*RW,AC*/
#define USB_CSR0_SENTSTALL				(0x04)   /*RC*/
#define USB_CSR0_DATAEND				(0x08)   /*WO,AC*/
#define USB_CSR0_SETUPEND				(0x10)   /*RO*/
#define USB_CSR0_SENDSTALL				(0x20)   /*WO,AC*/
#define USB_CSR0_SERVICEDOUTPKTRDY		(0x40)   /*WO,AC*/
#define USB_CSR0_SERVICESETUPEND		(0x80)   /*WO,AC*/

/*USB_INCSR1*/
#define USB_INCSR1_INPKTRDY				(0x01)   /*RW*/
#define USB_INCSR1_FIFONOTEMPTY			(0x02)   /*RC*/
#define USB_INCSR1_UNDERRUN				(0x04)   /*RC*/
#define USB_INCSR1_FLUSHFIFO			(0x08)   /*WO*/
#define USB_INCSR1_SENDSTALL			(0x10)   /*RW*/
#define USB_INCSR1_SENTSTALL			(0x20)   /*RC*/
#define USB_INCSR1_CLRDATATOG			(0x40)   /*WO*/

/*USB_INCSR2*/
#define USB_INCSR2_FRCDATATOG			(0x08)   /*RW*/
#define USB_INCSR2_DMAENAB				(0x10)   /*RW*/
#define USB_INCSR2_MODE					(0x20)   /*RW*/
#define USB_INCSR2_ISO					(0x40)   /*RW*/
#define USB_INCSR2_AUTOSET				(0x80)   /*RW*/

/*USB_OUTCSR1*/
#define USB_OUTCSR1_OUTPKTRDY			(0x01)   /*RC*/
#define USB_OUTCSR1_FIFOFULL			(0x02)   /*R,AC*/
#define USB_OUTCSR1_OVERRUN				(0x04)   /*RC*/
#define USB_OUTCSR1_DATAERROR			(0x08)   /*RO*/
#define USB_OUTCSR1_FLUSHFIFO			(0x10)   /*WO,AC*/
#define USB_OUTCSR1_SENDSTALL			(0x20)   /*RW*/
#define USB_OUTCSR1_SENTSTALL			(0x40)   /*RC*/
#define USB_OUTCSR1_CLRDATATOG			(0x80)   /*WO*/

/*USB_OUTCSR2*/
#define USB_OUTCSR2_DMAMODE				(0x10)   /*RW*/
#define USB_OUTCSR2_DMAENAB				(0x20)   /*RW*/
#define USB_OUTCSR2_ISO					(0x40)   /*RW*/
#define USB_OUTCSR2_AUTOCLEAR			(0x80)   /*RW*/

/*USB_ENABLE*/
#define USB_ENABLE_EN					(0x1)   /*WO*/
#define USB_ENABLE_DIS					(0x0)   /*WO*/
#else

/* USB_POWER */
#define USB_POWER_ENABLESUSPENDM	(0x01)   /* RW */
#define USB_POWER_SUSPENDMODE		(0x02)   /* P: RO ; H: WO */ /*Read clear by the intr. register*/
#define USB_POWER_RESUME				(0x04)   /* RW */
#define USB_POWER_RESET				(0x08)   /* P: RO ; H: RW */
#define USB_POWER_HSMODE				(0x10)   /* RO */
#define USB_POWER_HSENAB				(0x20)   /* RW */
#define USB_POWER_SOFTCONN			(0x40)   /* RW */
#define USB_POWER_ISOUPDATE			(0x80)   /* RW */

/* USB_INTRTX */
#define USB_INTRTX_EP0			(0x01)   /*RO*/
#define USB_INTRTX_EP1			(0x02)   /*RO*/
#define USB_INTRTX_EP2			(0x04)   /*RO*/
#define USB_INTRTX_EP3			(0x08)   /*RO*/
#define USB_INTRTX_EP4			(0x10)   /*RO*/

/* USB_INTRRX */
#define USB_INTRRX_EP1			(0x02)   /*RO*/
#define USB_INTRRX_EP2			(0x04)   /*RO*/
#define USB_INTRRX_EP3			(0x08)   /*RO*/

/* USB_INTRTXE & USB_INTRRXE */
#define USB_INTRE_EPEN			(0x01)   /* RW */
#define USB_INTRE_EPDIS			(0x00)   /* RW */

/* USB_INTRUSB */
#define USB_INTRUSB_SUSPEND	(0x01)   /*RO*/
#define USB_INTRUSB_RESUME	(0x02)   /*RO*/
#define USB_INTRUSB_RESET		(0x04)   /*RO*/
#define USB_INTRUSB_BABBLE		(0x04)   /*RO*/
#define USB_INTRUSB_SOF		(0x08)   /*RO*/
#define USB_INTRUSB_CONN		(0x10)   /*RO*/
#define USB_INTRUSB_DISCON		(0x20)   /*RO*/
#define USB_INTRUSB_SESSREQ	(0x40)   /*RO*/
#define USB_INTRUSB_VBUSERROR	(0x80)   /*RO*/

/* USB_INTRUSBE */
#define USB_INTRUSBE_SUSPEND	(0x01)   /*RW*/
#define USB_INTRUSBE_RESUME	(0x02)   /*RW*/
#define USB_INTRUSBE_RESET		(0x04)   /*RW*/
#define USB_INTRUSBE_BABBLE	(0x04)   /*RW*/
#define USB_INTRUSBE_SOF		(0x08)   /*RW*/
#define USB_INTRUSBE_CONN		(0x10)   /*RW*/
#define USB_INTRUSBE_DISCON	(0x20)   /*RW*/
#define USB_INTRUSBE_SESSREQ	(0x40)   /*RW*/
#define USB_INTRUSBE_VBUSERROR (0x80)   /*RW*/

/* USB_TESTMODE */
#define USB_TESTMODE_TESTSE0NAK 	(0x01)	/* RW */
#define USB_TESTMODE_TESTJ 		(0x02)	/* RW */
#define USB_TESTMODE_TESTK 		(0x04)	/* RW */
#define USB_TESTMODE_TESTPACKET 	(0x08)	/* RW */
#define USB_TESTMODE_FORCEHS 		(0x10)	/* RW */
#define USB_TESTMODE_FORCEFS 		(0x20)	/* RW */
#define USB_TESTMODE_FIFOACCESS 	(0x40)	/* WO, AC */
#define USB_TESTMODE_FORCEHOST 	(0x80)	/* RW */

/* USB_DEVCTL */
#define USB_DEVCTL_SESSION			(0x01)	 /* RW */
#define USB_DEVCTL_HOSTREQ		(0x02)	 /* RW */
#define USB_DEVCTL_HOSTMODE		(0x04)	 /* RO */
#define USB_DEVCTL_VBUS			(0x18)	 /* RO */
#define USB_DEVCTL_LSDEV			(0x20)	 /* RO */
#define USB_DEVCTL_FSDEV			(0x40)	 /* RO */
#define USB_DEVCTL_BDEVICE			(0x80)	 /* RO */

#define USB_DEVCTL_ABOVE_VBUS_VALID			(0x18)
#define USB_DEVCTL_ABOVE_A_VALID				(0x10)
#define USB_DEVCTL_ABOVE_SESSION_END			(0x01)
#define USB_DEVCTL_BELOW_SESSION_END			(0x00)

/* USB_CSR0 */
#define USB_CSR0_RXPKTRDY				(0x0001)   /* RO */
#define USB_CSR0_TXPKTRDY				(0x0002)   /* RW, AC */
#define USB_CSR0_SENTSTALL				(0x0004)   /* RC */
#define USB_CSR0_DATAEND				(0x0008)   /* WO, AC */
#define USB_CSR0_SETUPEND				(0x0010)   /* RO */
#define USB_CSR0_SENDSTALL				(0x0020)   /* WO, AC */
#define USB_CSR0_SERVICEDRXPKTRDY		(0x0040)   /* WO, AC */
#define USB_CSR0_SERVICEDSETUPEND		(0x0080)   /* WO, AC */
#define USB_CSR0_FLUSHFIFO				(0x0100)   /* WO, AC */
/* in Host mode */
#define USB_CSR0_RXSTALL				(0x0004)
#define USB_CSR0_SETUPPKT				(0x0008)
#define USB_CSR0_ERROR					(0x0010)
#define USB_CSR0_REQPKT				(0x0020)
#define USB_CSR0_STATUSPKT				(0x0040)
#define USB_CSR0_NAKTIMEOUT			(0x0080)
#define USB_CSR0_FLUSHFIFO				(0x0100)
#define USB_CSR0_DISPING				(0x0800)

/* USB_TXMAXP */
#define USB_TXMAXP_MAXPAYLOAD_MASK	(0x07FF)
#define USB_TXMAXP_HIGHSPEED_MASK	(0xF800)

/* USB_TXCSR */
#define USB_TXCSR_TXPKTRDY				(0x0001)   /* RW */
#define USB_TXCSR_FIFONOTEMPTY		(0x0002)   /* RO */
#define USB_TXCSR_UNDERRUN			(0x0004)   /* RW */
#define USB_TXCSR_FLUSHFIFO			(0x0008)   /* WO */
#define USB_TXCSR_SENDSTALL			(0x0010)   /* RW */
#define USB_TXCSR_SENTSTALL			(0x0020)   /* RC */
#define USB_TXCSR_CLRDATATOG			(0x0040)   /* WO */
#define USB_TXCSR_INCOMPTX				(0x0080)   /* RC */
#define USB_TXCSR_SETTXPKTRDY_TWICE	(0x0100)   /* RC */
#define USB_TXCSR_DMAREQMODE			(0x0400)   /* RW */
#define USB_TXCSR_FRCDATATOG			(0x0800)   /* RW */
#define USB_TXCSR_DMAREQENAB			(0x1000)   /* RW */
#define USB_TXCSR_ISO					(0x4000)   /* RW */
#define USB_TXCSR_AUTOSET				(0x8000)   /* RW */
/* in Host mode */
#define USB_TXCSR_ERROR				(0x0004)
#define USB_TXCSR_RXSTALL				(0x0020)
#define USB_TXCSR_NAKTIMEOUT			(0x0080)

/* USB_RXMAXP */
#define USB_RXMAXP_MAXPAYLOAD_MASK	(0x07FF)
#define USB_RXMAXP_HIGHSPEED_MASK	(0xF800)

/* USB_RXCSR */
#define USB_RXCSR_RXPKTRDY				(0x0001)   /* RC */
#define USB_RXCSR_FIFOFULL				(0x0002)   /* RO, AC */
#define USB_RXCSR_OVERRUN				(0x0004)   /* RC */
#define USB_RXCSR_DATAERROR			(0x0008)   /* RO */
#define USB_RXCSR_FLUSHFIFO			(0x0010)   /* WO, AC */
#define USB_RXCSR_SENDSTALL			(0x0020)   /* RW */
#define USB_RXCSR_SENTSTALL			(0x0040)   /* RC */
#define USB_RXCSR_CLRDATATOG			(0x0080)   /* WO */
#define USB_RXCSR_INCOMPRX				(0x0100)	  /* RC */
#define USB_RXCSR_KEEPERRCTATUS		(0x0200)	  /* RC */
#define USB_RXCSR_DMAREQMODE			(0x0800)   /* RW */
#define USB_RXCSR_DISNYET				(0x1000)   /* RW */
#define USB_RXCSR_PIDERROR				(0x1000)   /* RO */
#define USB_RXCSR_DMAREQENAB			(0x2000)   /* RW */
#define USB_RXCSR_ISO					(0x4000)   /* RW */
#define USB_RXCSR_AUTOCLEAR			(0x8000)   /* RW */
/* in Host mode */
#define USB_RXCSR_ERROR				(0x0004)
#define USB_RXCSR_NAKTIMEOUT			(0x0008)
#define USB_RXCSR_REQPKT				(0x0020)
#define USB_RXCSR_RXSTALL				(0x0040)
#define USB_RXCSR_SETPEQPKT_TWICE	(0x0400)
#define USB_RXCSR_AUTOREQ				(0x4000)

/* USB_TXTYPE */
#define USB_TXTYPE_EPNUM_MASK		(0x0f)
#define USB_TXTYPE_ISO					(0x10)
#define USB_TXTYPE_BULK				(0x20)
#define USB_TXTYPE_INTR				(0x30)
#define USB_TXTYPE_PROTOCOL_MASK		(0x30)

/* USB_RXTYPE */
#define USB_RXTYPE_EPNUM_MASK		(0x0f)
#define USB_RXTYPE_ISO					(0x10)
#define USB_RXTYPE_BULK				(0x20)
#define USB_RXTYPE_INTR				(0x30)
#define USB_RXTYPE_PROTOCOL_MASK		(0x30)

/* USB_PWRUPCNT */
#define USB_PWRUPCNT_MASK				(0x0f)

/* USB_FIFOSZ */
#define USB_FIFOSZ_SIZE_MASK			(0x0F)
#define USB_FIFOSZ_DPB					(0x10)
#define USB_FIFOSZ_SIZE_8				(0x00)
#define USB_FIFOSZ_SIZE_16				(0x01)
#define USB_FIFOSZ_SIZE_32				(0x02)
#define USB_FIFOSZ_SIZE_64				(0x03)
#define USB_FIFOSZ_SIZE_128			(0x04)
#define USB_FIFOSZ_SIZE_256			(0x05)
#define USB_FIFOSZ_SIZE_512			(0x06)
#define USB_FIFOSZ_SIZE_1024			(0x07)
#define USB_FIFOSZ_SIZE_2048			(0x08)
#define USB_FIFOSZ_SIZE_4096			(0x09)

/* USB_FIFOADD */
#define USB_FIFOADD_MASK				(0x1FFF)

/* USB_RXFIFOADD */
#define USB_RXFIFOADD_DATAERRINTREN   (0x8000)
#define USB_RXFIFOADD_OVERRUNINTREN   (0x4000)

/* USB_FIFO2ADD */
#define USB_FIFO2ADD_MASK				(0x1FFF)
#define USB_FIFO2ADD_EN				(0x8000)

/* USB_BUSPERF3 */
#define USB_BUSPERF3_DISUSBRESET			(0x0001)
#define USB_BUSPERF3_SWRST				(0x0002)



/* USB_RSTINFO */
#define USB_RSTINFO_WTFSSE0			(0x00F0)
#define USB_RSTINFO_WTCHRP			(0x000F)



/* USB_L1INTS */
#define USB_L1INTS_TX_INT_STATUS			(0x0001)
#define USB_L1INTS_RX_INT_STATUS			(0x0002)
#define USB_L1INTS_USBCOM_INT_STATUS		(0x0004)
#define USB_L1INTS_DMA_INT_STATUS		(0x0008)
#define USB_L1INTS_PSR_INT_STATUS			(0x0010)
#define USB_L1INTS_QINT_STATUS			(0x0020)
#define USB_L1INTS_QHIF_INT_STATUS		(0x0040)
#define USB_L1INTS_DPDM_INT_STATUS		(0x0080)
#define USB_L1INTS_VBUSVALID_INT_STATUS	(0x0100)
#define USB_L1INTS_IDDIG_INT_STATUS		(0x0200)
#define USB_L1INTS_DRVVBUS_INT_STATUS	(0x0400)
#define USB_L1INTS_POWERDOWN_INT_STATUS (0x0800)


/* USB_L1INTM */
#define USB_L1INTM_TX_INT_UNMASK			(0x0001)
#define USB_L1INTM_RX_INT_UNMASK			(0x0002)
#define USB_L1INTM_USBCOM_INT_UNMASK	(0x0004)
#define USB_L1INTM_DMA_INT_UNMASK		(0x0008)
#define USB_L1INTM_PSR_INT_UNMASK		(0x0010)
#define USB_L1INTM_QINT_UNMASK			(0x0020)
#define USB_L1INTM_QHIF_INT_UNMASK		(0x0040)
#define USB_L1INTM_DPDM_INT_UNMASK		(0x0080)
#define USB_L1INTM_VBUSVALID_INT_UNMASK	(0x0100)
#define USB_L1INTM_IDDIG_INT_UNMASK		(0x0200)
#define USB_L1INTM_DRVVBUS_INT_UNMASK	(0x0400)
#define USB_L1INTM_POWERDOWN_INT_UNMASK	(0x0800)


/* USB_L1INTP */
#define USB_L1INTP_VBUSVALID_INT_POL		(0x0100)
#define USB_L1INTP_IDDIG_INT_POL			(0x0200)
#define USB_L1INTP_DRVVBUS_INT_POL		(0x0400)
#define USB_L1INTP_POWERDOWN_INT_POL		(0x0800)


/* USB_L1INTC */
#define USB_L1INTC_USB_INT_SYNC			(0x0001)

#endif


/* USB_DMACNTL */
#define USB_DMACNTL_DMAEN				(0x0001)
#define USB_DMACNTL_DMADIR			(0x0002)
#define USB_DMACNTL_DMAMODE			(0x0004)
#define USB_DMACNTL_INTEN				(0x0008)
#define USB_DMACNTL_EP_MASK			(0x00F0)
#define USB_DMACNTL_BUSERR			(0x0100)
#define USB_DMACNTL_BURSTMODE_MASK	(0x0000)
#define USB_DMACNTL_DMACHEN			(0x0800)


/* USB_DMA_TIMER */
#define USB_DMA_TIMER_ENTIMER				(0x0080)
#define USB_DMA_TIMER_TIMEOUT_MASK		(0x007F)
#define USB_DMA_TIMER_TIMEOUT_STATUS	(0x0100)		/* W1C */


/*USB_DMAQ_RQCSRn*/
#define USB_DMACNTL_DMAEN_RXQ_ACTIVE	(0x8000)
#define USB_DMACNTL_DMAEN_RXQ_STOP		(0x0004)
#define USB_DMACNTL_DMAEN_RXQ_RESUME	(0x0002)
#define USB_DMACNTL_DMAEN_RXQ_START		(0x0001)
#define USB_DBG_PRB_OPSTATE_MASK			(0x0000003F)
#define USB_DBG_PRB_LINESTATE_MASK		(0x000000C0)




//========================definition of global value===================//
#define USB_MAX_FIFO_SIZE				2304  //64(EP0)+64(INT)+512x2(Bulk TX)+512x2(Bulk RX)
#define USB_FIFO_START_ADDRESS		64
#define USB_BULK_FIFO_UNIT_SIZE		512
#define USB_ISO_FIFO_UNIT_SIZE			1024
#define USB_INTR_FIFO_UNIT_SIZE			32

//==============================================
#if 0
#define USB_DMACNTL(_n) 		(USB_BASE+0x1F4+(0x10*(_n))) 	/* RW, _n = 1~6 */
#define USB_DMAADDR(_n)			(USB_BASE+0x1F8+(0x10*(_n))) 	/* RW, _n = 1~6 */
#define USB_DMACNT(_n)			(USB_BASE+0x1FC+(0x10*(_n))) 	/* RW, _n = 1~6 */
#define USB_DMA_REALCNT(_n)		(USB_BASE+0x270+(0x10*(_n))) 	/* RO, _n = 1~6, 32bits  **/
#define USB_DMA_TIMER(_n)			(USB_BASE+0x274+(0x10*(_n))) 	/* RW, _n = 1~6, 16bits */

#define USBDMA_CheckRunStat(_n)			(USB_DRV_Reg(USB_DMACNTL(_n))&USB_DMACNTL_DMAEN)
#define USBDMA_CheckPPRunStat(_n)			(USB_DRV_Reg(USB_DMAPPCNTL(_n))&USB_DMACNTL_DMAEN)
#define USBDMA_Stop(_n)						USB_DRV_ClearBits(USB_DMACNTL(_n), USB_DMACNTL_DMAEN)


#define DMA_CheckRunStat(_n)           ((DRV_Reg32(DMA_GLBSTS) & DMA_GLBSTA_RUN(_n))!=0)
#endif



#define USB_DMA_TIMER_TIMEOUT_TICK 		0x7f
#define USB_DIS_DMA_LIMITER				0xff

//==============================================
#define DRV_WriteReg(addr,data)     ((*(volatile uint16_t *)(addr)) = (uint16_t)(data))
#define DRV_Reg(addr)               (*(volatile uint16_t *)(addr))
#define DRV_WriteReg32(addr,data)     ((*(volatile uint32_t *)(addr)) = (uint32_t)(data))
#define DRV_Reg32(addr)               (*(volatile uint32_t *)(addr))
#define DRV_WriteReg8(addr,data)     ((*(volatile uint8_t *)(addr)) = (uint8_t)(data))
#define DRV_Reg8(addr)               (*(volatile uint8_t *)(addr))
#define USB_DRV_WriteReg(addr,data)     ((*(volatile uint16_t *)(addr)) = (uint16_t)(data))
#define USB_DRV_Reg(addr)               (*(volatile uint16_t *)(addr))
#define USB_DRV_WriteReg32(addr,data)     ((*(volatile uint32_t *)(addr)) = (uint32_t)(data))
#define USB_DRV_Reg32(addr)               (*(volatile uint32_t *)(addr))
#define USB_DRV_WriteReg8(addr,data)     ((*(volatile uint8_t *)(addr)) = (uint8_t)(data))
#define USB_DRV_Reg8(addr)               (*(volatile uint8_t *)(addr))

#define USB_DRV_ClearBits(addr,data)     {\
   uint16_t temp;\
   temp = DRV_Reg(addr);\
   temp &=~(data);\
   DRV_WriteReg(addr,temp);\
}

#define USB_DRV_SetBits(addr,data)     {\
   uint16_t temp;\
   temp = DRV_Reg(addr);\
   temp |= (data);\
   DRV_WriteReg(addr,temp);\
}

#define USB_DRV_SetData(addr, bitmask, value)     {\
   uint16_t temp;\
   temp = (~(bitmask)) & DRV_Reg(addr);\
   temp |= ((value) & (bitmask));\
   DRV_WriteReg(addr,temp);\
}

#define USB_DRV_ClearBits32(addr,data)     {\
   uint32_t temp;\
   temp = DRV_Reg32(addr);\
   temp &=~(data);\
   DRV_WriteReg32(addr,temp);\
}

#define USB_DRV_SetBits32(addr,data)     {\
   uint32_t temp;\
   temp = DRV_Reg32(addr);\
   temp |= (data);\
   DRV_WriteReg32(addr,temp);\
}

#define USB_DRV_SetData32(addr, bitmask, value)     {\
   uint32_t temp;\
   temp = (~(bitmask)) & DRV_Reg32(addr);\
   temp |= ((value) & (bitmask));\
   DRV_WriteReg32(addr,temp);\
}

#define USB_DRV_ClearBits8(addr,data)     {\
   uint8_t temp;\
   temp = DRV_Reg8(addr);\
   temp &=~(data);\
   DRV_WriteReg8(addr,temp);\
}

#define USB_DRV_SetBits8(addr,data)     {\
   uint8_t temp;\
   temp = DRV_Reg8(addr);\
   temp |= (data);\
   DRV_WriteReg8(addr,temp);\
}

#define USB_DRV_SetData8(addr, bitmask, value)     {\
   uint8_t temp;\
   temp = (~(bitmask)) & DRV_Reg8(addr);\
   temp |= ((value) & (bitmask));\
   DRV_WriteReg8(addr,temp);\
}

#if 0
#define  USB_DMA_TX_CSR		(USB_TXCSR_AUTOSET|USB_TXCSR_DMAREQENAB|USB_TXCSR_DMAREQMODE)
#define  USB_DMA_TX_CSR_ISO	(USB_TXCSR_AUTOSET|USB_TXCSR_DMAREQENAB|USB_TXCSR_DMAREQMODE|USB_TXCSR_ISO)
#define  USB_DMA_RX_CSR		(USB_RXCSR_AUTOCLEAR|USB_RXCSR_DMAREQENAB)
#define  USB_DMA_RX_CSR_ISO	(USB_RXCSR_AUTOCLEAR|USB_RXCSR_DMAREQENAB|USB_RXCSR_ISO)
#endif

#define DRV_USB_HISR_ID	0

typedef struct {
    hal_usb_power_state_t			power_state;
    bool					is_unMaskUSB;	 /*Tricky, used to control interrupt */
    bool					is_ProUSBHISR;

    /* interrupt handler */
    hal_usb_driver_interrupt_handler_t	reset_hdlr;
    hal_usb_driver_interrupt_handler_t	suspend_hdlr;
    hal_usb_driver_interrupt_handler_t	resume_hdlr;
    hal_usb_driver_interrupt_handler_t	ep0_hdlr;
    hal_usb_driver_interrupt_handler_t	ep_tx_hdlr[HAL_USB_MAX_NUMBER_ENDPOINT_TX];
    hal_usb_driver_interrupt_handler_t	ep_rx_hdlr[HAL_USB_MAX_NUMBER_ENDPOINT_RX];

    /* Stall status */
    bool					ep_tx_stall_status[HAL_USB_MAX_NUMBER_ENDPOINT_TX];
    bool					ep_rx_stall_status[HAL_USB_MAX_NUMBER_ENDPOINT_RX];

    /* cable plug out : disconnect */
    bool          usb_disconnect;

    /* Endpoint current status */
    hal_usb_endpoint_state_t		ep_tx_enb_state[HAL_USB_MAX_NUMBER_ENDPOINT_TX];
    hal_usb_endpoint_state_t		ep_rx_enb_state[HAL_USB_MAX_NUMBER_ENDPOINT_RX];


    /* FIFO flush interrupt */
    bool					ep_tx_flush_intr[HAL_USB_MAX_NUMBER_ENDPOINT_TX];
    bool					ep_rx_flush_intr[HAL_USB_MAX_NUMBER_ENDPOINT_RX];
    uint8_t                            dma_burst_mode;
    /*HS FS flag*/
    bool					Is_HS_mode;

    /*USB FIFO full and receive Suspend signal */
    bool					suspend_with_FIFO_not_empty;
    uint8_t    suspend_count;

    /* DMA parameters */
    uint8_t					dma_port[2][HAL_USB_MAX_NUMBER_ENDPOINT]; 	/* record TX/RX ep's DMA port number */
    hal_usb_endpoint_direction_t		dma_dir[HAL_USB_MAX_NUMBER_DMA]; 			/* record  DMA port's direction */
    //usb_dmaq_CB			dmaq_callback[2][HAL_USB_MAX_NUMBER_DMA];			/* take PP buffer */
    //usb_dmaq_CB			dmaq_empty_callback[2][HAL_USB_MAX_NUMBER_DMA];			/* take PP buffer */
    void					*privdata[2][HAL_USB_MAX_NUMBER_DMA];			/* take PP buffer */

    bool					dma_pktrdy[HAL_USB_MAX_NUMBER_DMA];
    bool					dma_running[HAL_USB_MAX_NUMBER_DMA];
    bool					dma_callback_upd_run[HAL_USB_MAX_NUMBER_DMA]; /* take PP buffer */
    hal_usb_dma_handler_t			dma_callback[HAL_USB_MAX_NUMBER_DMA];			/* take PP buffer */
    uint32_t                  dma_tx_length[HAL_USB_MAX_NUMBER_DMA];


    bool 					is_bidirection_dma[HAL_USB_MAX_NUMBER_DMA];
    uint8_t					dma_channel;
    uint8_t					dma_tx_ep_num[HAL_USB_MAX_NUMBER_DMA];  		/* record  DMA port's TX ep number */
    uint8_t					dma_rx_ep_num[HAL_USB_MAX_NUMBER_DMA];  		/* record  DMA port's RX ep number */
    bool					b_enable_dma_burst_auto_chge;		/* For DVT use only, enable DMA auto changed burst mode */
#if 1
	uint8_t    IntrTx;
	uint8_t    IntrRx;
#endif	
} USB_Drv_Info;

typedef struct {
    /*0x00*/
    uint8_t		faddr;		/* FunctionAddressRegister(Devicemodeonly) */
    uint8_t		power;		/* PowerManagementRegister */
    uint8_t		intrin1;	/* TxInterruptStatusRegister */
    uint8_t		intrin2;	/* RxInterruptStatusRegister */
    uint8_t		introut1;	/* TxInterruptEnableRegister */
    uint8_t		introut2;	/* RxInterruptEnableRegister */
    uint8_t		intrusb;	/* CommonUSBInterruptRegister */
    uint8_t		intrin1e;	/* CommonUSBInterruptEnableRegister */
	uint8_t 	intrin2e;	/* CommonUSBInterruptEnableRegister */
    uint8_t		introut1e;	/* TxInterruptEnableRegister */
    uint8_t		introut2e;	/* RxInterruptEnableRegister */
    uint8_t		intrusbe;	/* CommonUSBInterruptRegister */
	uint8_t		frame1;		/* FrameNumberRegister */
	uint8_t		frame2;		/* FrameNumberRegister */
    uint8_t		index;		/* EndpointSelectionIndexRegister */
    uint8_t		rstctrl;		/* EndpointSelectionIndexRegister */
    /*0x10*/
    uint8_t		inmaxp;		/* TXMAPRegister */
//    uint8_t		csr0;		/* TxCSRRegister */
    uint8_t		incsr1;		/* RXMAPRegister */
    uint8_t		incsr2;		/* RXMAPRegister */
    uint8_t		outmaxp;	/* RXCSRRegister */
    uint8_t		outcsr1;	/* RXCSRRegister */
    uint8_t		outcsr2;	/* RXCSRRegister */
//    uint8_t		count0;	/* RxCountRegister */
    uint8_t		outcount1;	/* RxCountRegister */
    uint8_t		outcount2;	/* RxCountRegister */
    uint8_t	revered0[8];

    /*0x20*/
    uint8_t		ep0;	/* USBEndpoint0FIFORegister */
    uint8_t	revered1[527];

    /*0x230*/
    uint8_t		enable;	/* USBEndpoint0FIFORegister */
    uint8_t	revered2[15];

    uint8_t phyctl;
	
    uint8_t	revered3[3];

	/*0x244*/
	uint8_t 	dummy; /* USBEndpoint0FIFORegister */

} USB_REGISTER_T;


#define	U1PHYCR0_RG_USB11_RST		 		(1<<23)	//BIT15
#define U1PHYCR0_RG_USB11_FSLS_ENBGRI		(1<<11) //BIT11
#define	U1PHYCR1_RG_USB11_PHY_REV_7 		(1<<15)	//BIT15
#define U1PHYCR0_RG_USB_LVSH_EN             (1<<8) //BIT8


typedef struct {
    /*0x00*/
#if 0	
    uint32_t	revered[512];	//
    uint32_t	u2phyac0;     //0x800
    uint32_t	u2phyac1;     //0x804
    uint32_t	u2phyac2;     //0x808
    uint32_t	revered1;      //
    uint32_t	u2phyacr0;    //0x810
    uint32_t	u2phyacr1;    //0x814
    uint32_t	u2phyacr2;    //0x818
    uint32_t	u2phyacr3;    //0x81c
    uint32_t	u2phyacr4;    //0x820
    uint32_t	revered2[15];  //
    uint32_t	u2phydcr0;    //0x860
    uint32_t	u2phydcr1;    //0x864
    uint32_t	u2phydtm0;    //0x868
    uint32_t	u2phydtm1;    //0x86c
    uint32_t	u2phydmon0;   //0x870
    uint32_t	u2phydmon1;   //0x874
    uint32_t	u2phydmon2;   //0x878
    uint32_t	revered3[17];  //
#endif    
    uint32_t	u1phycr0;     //0x8c0
    uint32_t	u1phycr1;     //0x8c4
    uint32_t	u1phycr2;     //0x8c8
    uint32_t	revered4[5];   //
    uint32_t	regfppc;      //0x8e0
    uint32_t	revered5[3];   //
    uint32_t	versionc;     //0x8f0
    uint32_t	revered6[2];   //
    uint32_t	regfcom;			//0x8fc
    uint32_t	revered7[384];
    uint32_t	fmcr0;				//0xf00
    uint32_t	fmcr1;				//0xf04
    uint32_t	fmcr2;				//0xf08
    uint32_t	fmmonr0;			//0xf0c
    uint32_t	fmmonr1;			//0xf10
} USBPHY_REGISTER_T;

extern void hal_usbphy_poweron_initialize(void);
extern void hal_usbphy_save_current(void);
extern void hal_usbphy_recover(void);
extern void hal_usbphy_slew_rate_calibration(void);
extern void RestoreIRQMask(uint32_t);
extern uint32_t SaveAndSetIRQMask(void);



#endif /*HAL_USB_MODULE_ENABLED*/
#endif /*__HAL_USB_INTERNAL_H__*/
