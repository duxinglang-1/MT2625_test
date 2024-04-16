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

#ifndef __HAL_CLOCK_PLATFORM_MT2625_H__
#define __HAL_CLOCK_PLATFORM_MT2625_H__

#include "hal_platform.h"

#ifdef HAL_CLOCK_MODULE_ENABLED

#define STR_VALUE(arg)      #arg
#define __stringify(name) STR_VALUE(name)

#define DCXO_CAPID_DEFAULT_VALUE                        431 /* 0x1AF */
#define DCXO_CAPID_GOLDEN_VALUE                         411 /* 0x19B */
#define DCXO_CAPID_GOLDEN_RF_VALUE                      403 /* 0x193 */

#define DCXO_EFUSE_CAPID                                ((volatile uint32_t *)0xA20A0450)  /* [13:5] CAPID0 [22:14] CAPID1 [31:12] CAPID2 */
#define DCXO_EFUSE_MAGIC                                ((volatile uint32_t *)0xA20A0008)
#define DCXO_EFUSE_MAGIC_CODE                           0xA07923B6
#define DCXO_EFUSE_REFRESH                              ((volatile uint32_t *)0xA20A0000)
#define DCXO_EFUSE_REFRESH_SET                          0x10000

#define DCXO_EFUSE_CAPID0_BIT                           5
#define DCXO_EFUSE_CAPID0_MASK                          0x00003FE0
#define DCXO_EFUSE_CAPID1_BIT                           14
#define DCXO_EFUSE_CAPID1_MASK                          0x007FC000
#define DCXO_EFUSE_CAPID2_BIT                           23
#define DCXO_EFUSE_CAPID2_MASK                          0xFF800000

#define DCXO_CAP_ID                                     ((volatile uint32_t *)0xA2060018)  /*[24:16] cal data [0] sel*/
#define DCXO_CAP_ID_BIT                                 16
#define DCXO_CAP_ID_MASK                                0x01FF0000

#define HW_MISC0                                        ((volatile uint8_t *)0xA20A0230)

#define UPLL_CON5__F_RG_UPLL_IBANK_FINETUNE             ((volatile uint8_t *)0xA2040156)
#define MPLL_CON5__F_RG_MPLL_IBANK_FINETUNE             ((volatile uint8_t *)0xA2040116)
#define CLKSQ_CON0__F_DA_SRCLKENA                       ((volatile uint8_t *)0xA2040020)
#define DPM_CON1__F_UPLL_SETTLE_TIME                    ((volatile uint16_t *)0xA2040096)
#define DPM_CPN2__F_MPLL_SETTLE_TIME                    ((volatile uint16_t *)0xA2040098)
#define UPLL_CON0__F_DA_UPLL_EN                         ((volatile uint8_t *)0xA2040140)
#define PLL_CON2                                        ((volatile uint32_t *)0xA2040048)
#define PLL_CON3                                        ((volatile uint32_t *)0xA204004C)
#define PLLTD_CON0__F_BP_PLL_DLY                        ((volatile uint8_t *)0xA2040700)
#define UPLL_CON0__F_RG_UPLL_RDY                        ((volatile uint8_t *)0xA2040413)
#define MPLL_CON1__F_RG_MPLL_POSTDIV                    ((volatile uint8_t *)0xA204107)
#define PLL_CON4__F_RG_PLL_PGDET_EN                     ((volatile uint8_t *)0xA2040050)
#define MDDS_CON0__F_RG_MDDS_EN                         ((volatile uint8_t *)0xA2040640)
#define MPLL_CON0__F_DA_MPLL_EN                         ((volatile uint8_t *)0xA2040100)
#define MPLL_CON0__F_RG_MPLL_RDY                        ((volatile uint8_t *)0xA2040103)
#define PLLTD_CON2__F_S1_STB_TIME                       ((volatile uint8_t *)0xA2040248)

#define CKSYS_CLK_CFG_2__F_CLK_48M_SEL                  ((volatile uint8_t *)0xA2020248)
#define CKSYS_CLK_DIV_1__F_CLK_PLL2_D7_EN               ((volatile uint8_t *)0xA2020287)
#define CKSYS_CLK_DIV_1__F_CLK_PLL2_D5_EN               ((volatile uint8_t *)0xA2020286)
#define CKSYS_CLK_DIV_1__F_CLK_PLL2_D3_EN               ((volatile uint8_t *)0xA2020285)
#define CKSYS_CLK_DIV_1__F_CLK_PLL2_D2_EN               ((volatile uint8_t *)0xA2020284)
#define CKSYS_CLK_DIV_2__F_CLK_PLL1_DIV_EN              ((volatile uint8_t *)0xA2020288)
#define CKSYS_CLK_DIV_2__F_CLK_PLL2_DIV_EN              ((volatile uint8_t *)0xA2020289)
#define CKSYS_CLK_FORCE_ON_0                            ((volatile uint32_t *)0xA2020270)
#define CKSYS_CLK_FORCE_ON_1                            ((volatile uint32_t *)0xA2020274)
#define CKSYS_CLK_FORCE_ON_0__F_CLK_SYS_FORCE_ON        ((volatile uint8_t *)0xA2020270)
#define CKSYS_CLK_FORCE_ON_0__F_CLK_SFC_FORCE_ON        ((volatile uint8_t *)0xA2020271)
#define CKSYS_CLK_FORCE_ON_0__F_CLK_DSP_FORCE_ON        ((volatile uint8_t *)0xA2020272)
#define CKSYS_CLK_FORCE_ON_0__F_CLK_SPIMST_FORCE_ON     ((volatile uint8_t *)0xA2020273)
#define CKSYS_CLK_FORCE_ON_1__F_CLK_BSIBPI_FORCE_ON     ((volatile uint8_t *)0xA2020274)
#define CKSYS_CLK_FORCE_ON_1__F_CLK_SDIOMST0_FORCE_ON   ((volatile uint8_t *)0xA2020275)
#define CKSYS_CLK_FORCE_ON_1__F_CLK_SDIOMST1_FORCE_ON   ((volatile uint8_t *)0xA2020276)
#define CKSYS_CLK_FORCE_ON_1__F_CLK_SPISLV_FORCE_ON     ((volatile uint8_t *)0xA2020277)
#define CKSYS_CLK_CFG_0__F_CLK_SYS_SEL                  ((volatile uint8_t *)0xA2020240)
#define CKSYS_CLK_CFG_0__F_CLK_SFC_SEL                  ((volatile uint8_t *)0xA2020241)
#define CKSYS_CLK_CFG_0__F_CLK_DSP_SEL                  ((volatile uint8_t *)0xA2020242)
#define CKSYS_CLK_CFG_0__F_CLK_SPIMST_SEL               ((volatile uint8_t *)0xA2020243)
#define CKSYS_CLK_CFG_1__F_CLK_BSIBPI_SEL               ((volatile uint8_t *)0xA2020244)
#define CKSYS_CLK_CFG_1__F_CLK_SDIOMST0_SEL             ((volatile uint8_t *)0xA2020245)
#define CKSYS_CLK_CFG_1__F_CLK_SDIOMST1_SEL             ((volatile uint8_t *)0xA2020246)
#define CKSYS_CLK_CFG_1__F_CLK_SPISLV_SEL               ((volatile uint8_t *)0xA2020247)
#define CKSYS_CLK_CFG_2__F_CLK_48m_SEL                  ((volatile uint8_t *)0xA2020248)
#define CKSYS_CLK_UPDATE_0__F_CHG_SYS                   ((volatile uint8_t *)0xA2020250)
#define CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SYS_OK         ((volatile uint8_t *)0xA2020260)
#define CKSYS_CLK_UPDATE_0__F_CHG_SFC                   ((volatile uint8_t *)0xA2020251)
#define CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SFC_OK         ((volatile uint8_t *)0xA2020261)
#define CKSYS_CLK_UPDATE_0__F_CHG_DSP                   ((volatile uint8_t *)0xA2020252)
#define CKSYS_CLK_UPDATE_STATUS_0__F_CHG_DSP_OK         ((volatile uint8_t *)0xA2020262)
#define CKSYS_CLK_UPDATE_0__F_CHG_SPIMST                ((volatile uint8_t *)0xA2020253)
#define CKSYS_CLK_UPDATE_STATUS_0__F_CHG_SPIMST_OK      ((volatile uint8_t *)0xA2020263)
#define CKSYS_CLK_UPDATE_1__F_CHG_BSIBPI                ((volatile uint8_t *)0xA2020254)
#define CKSYS_CLK_UPDATE_STATUS_1__F_CHG_BSIBPI_OK      ((volatile uint8_t *)0xA2020264)
#define CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST0              ((volatile uint8_t *)0xA2020255)
#define CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SDIOMST0_OK    ((volatile uint8_t *)0xA2020265)
#define CKSYS_CLK_UPDATE_1__F_CHG_SDIOMST1              ((volatile uint8_t *)0xA2020256)
#define CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SDIOMST1_OK    ((volatile uint8_t *)0xA2020266)
#define CKSYS_CLK_UPDATE_1__F_CHG_SPISLV                ((volatile uint8_t *)0xA2020257)
#define CKSYS_CLK_UPDATE_STATUS_1__F_CHG_SPISLV_OK      ((volatile uint8_t *)0xA2020267)
#define SFC_DCM_CON_0__F_RG_SFC_DCM_DBC_NUM             ((volatile uint8_t *)0xA2020141)
#define SFC_DCM_CON_0__F_RG_SFC_DCM_DBC_EN              ((volatile uint8_t *)0xA2020142)
#define SFC_DCM_CON_1__F_RG_SFC_CLKOFF_EN               ((volatile uint8_t *)0xA2020144)
#define SFC_DCM_CON_0__F_RG_SFC_DCM_APB_SEL             ((volatile uint8_t *)0xA2020143)
#define SFC_DCM_CON_1__F_RG_SFC_DCM_APB_TOG             ((volatile uint8_t *)0xA2020147)
#define ABIST_FQMTR_CON0                                ((volatile uint16_t *)0xA2020400)
#define ABIST_FQMTR_CON1                                ((volatile uint16_t *)0xA2020404)
#define CKSYS_TST_SEL_1_F_TCKSEL                        ((volatile uint8_t *)0xA2020224)
#define CKSYS_TST_SEL_1_F_FCKSEL                        ((volatile uint8_t *)0xA2020225)
#define PLL_ABIST_FQMTR_DATA__F_FQMTR_DATA              ((volatile uint32_t *)0xA202040C)

#define BUS_DCM_CON_0__F_RG_BUS_SFSEL                   ((volatile uint8_t *)0xA21D0100)
#define CM4_DCM_CON_0__F_RG_CM_SFSEL                    ((volatile uint8_t *)0xA21D0110)
#define BUS_DCM_CON_1__F_RG_BUS_CLKSLOW_EN              ((volatile uint8_t *)0xA21D0105)
#define BUS_DCM_CON_1__F_RG_BUS_CLKOFF_EN               ((volatile uint8_t *)0xA21D0104)
#define BUS_DCM_CON_0__F_RG_BUS_DCM_EN                  ((volatile uint8_t *)0xA21D0103)
#define CM4_DCM_CON_1__F_RG_CM_CLKSLOW_EN               ((volatile uint8_t *)0xA21D0115)
#define CLK_FREQ_SWCH__F_RG_PLLCK_SEL                   ((volatile uint8_t *)0xA21D0170)

#define PDN_COND0_F_PDR_COND0                           ((volatile uint32_t *)0xA21D0300)
#define PDN_SETD0_F_PDR_SETD0                           ((volatile uint32_t *)0xA21D0310)
#define PDN_CLRD0_F_PDR_CLRD0                           ((volatile uint32_t *)0xA21D0320)

#define XO_PDN_COND0                                    ((volatile uint32_t *)0xA2030B00)
#define XO_PDN_SETD0                                    ((volatile uint32_t *)0xA2030B10)
#define XO_PDN_CLRD0                                    ((volatile uint32_t *)0xA2030B20)

#define XO_DCM_CON_1__F_RG_XO_CLKOFF_EN                 ((volatile uint8_t *)0xA2030C04)
#define XO_DCM_CON_1__F_RG_XO_CLKSLOW_EN                ((volatile uint8_t *)0xA2030C05)

#define DCXO_PCON5__F_DCXO_ACAL_EFUSE_SEL               ((volatile uint8_t *)0xA2060014)
#define DCXO_PCON5__F_DCXO_ACAL_EFUSE                   ((volatile uint8_t *)0xA2060016)
#define DCXO_PCON4__F_DCXO_PWR_EN_TD                    ((volatile uint8_t *)0XA2060010)
#define DCXO_PCON3__F_DCXO_EN_TD                        ((volatile uint8_t *)0XA206000F)
#define DCXO_PCON3__F_DCXO_BUF_EN_TD                    ((volatile uint8_t *)0XA206000E)
#define DCXO_PCON3__F_DCXO_BUF_EN_TD_1612               ((volatile uint8_t *)0XA206000D)
#define DCXO_PCON0__F_GSM_DCXO_CTL_EN                   ((volatile uint8_t *)0XA2060000)
#define DCXO_PCON1__F_FRC_COCLK_EN                      ((volatile uint8_t *)0XA2060006)
#define DCXO_PCON1__F_EXT_DCXO_CTL_EN                   ((volatile uint8_t *)0XA2060007)

#define FH_CON5__F_MPLL_FRDDS_DNLMT                     ((volatile uint8_t*)(0XA2040514))
#define FH_CON5__F_MPLL_FRDDS_UPLMT                     ((volatile uint8_t*)(0XA2040516))
#define FH_CON3__F_MPLL_FRDDS_DTS                       ((volatile uint8_t*)(0xA204050D))
#define FH_CON3__F_MPLL_FRDDS_DYS                       ((volatile uint8_t*)(0xA204050E))
#define FH_CON4__F_MPLL_FHCTL_EN                        ((volatile uint8_t*)(0xA2040510))
#define FH_CON4__F_MPLL_FRDDS_EN                        ((volatile uint8_t*)(0xA2040512))

#define FH_CON3__F_MPLL_SFSTR_DYS                       ((volatile uint8_t*)(0xA204050C))
#define FH_CON4__F_MPLL_SFSTR_DTS                       ((volatile uint8_t*)(0xA2040513))
#define FH_CON4__F_MPLL_SFSTR_EN                        ((volatile uint8_t*)(0xA2040511))
#define FH_CON0__F_PLL_FHSET                            ((volatile uint16_t*)(0xA2040500))
#define FH_CON0__F_PLL_FREQ_STR                         ((volatile uint8_t*)(0xA2040502))

/*************************************************************************
 * Define clock force on register and bits structure
 *************************************************************************/
typedef enum {
    CLK_VCORE_0P9V,
    CLK_VCORE_1P1V,
    CLK_VCORE_1P3V,
    NR_CLKVCORE_VOLTAGE,
} clock_vcore_voltage;

/*************************************************************************
 * Define clock force on register and bits structure
 *************************************************************************/
enum clock_force_on_id {
    CLK_SYS_FORCE_ON,
    CLK_SFC_FORCE_ON,
    CLK_CONN_FORCE_ON,
    CLK_SPIMST_FORCE_ON,
    CLK_SDIOMST_FORCE_ON,
    NR_CLKFORCE_ON,
};

/*************************************************************************
 * Define clock change bit register and bits structure
 *************************************************************************/
typedef enum {
    CHG_SYS,
    CHG_SFC,
    CHG_CONN,
    CHG_SPIMST,
    NR_CLKCHANGE_BIT,
} clock_change_bit_id;

/*************************************************************************
 * Define clock mux register and bits structure
 *************************************************************************/
typedef enum {
    CLK_SYS_SEL,
    CLK_SFC_SEL,
    CLK_DSP_SEL,
    CLK_SPIMST_SEL,
    CLK_BSIBPI_SEL,
    CLK_SDIOMST0_SEL,
    CLK_SDIOMST1_SEL,
    CLK_SPISLV_SEL,
    CLK_48M_SEL,
    NR_MUXS,
    CLK_MUX_SEL_NONE,
} clock_mux_sel_id;

/*************************************************************************
* Define clock gating register and bits structure
* Note: MUST modify clk_cg_mask in hal_clock.c if hal_clock_cg_id is changed
*************************************************************************/
enum {
    NR_CLOCKS           = HAL_CLOCK_CG_END
};

enum {
    GRP_PDN_COND0       = 0,
    GRP_XO_PDN_COND0    = 1,
    NR_GRPS             = 2,
};

#endif /* HAL_CLOCK_MODULE_ENABLED */
#endif /* __HAL_CLOCK_PLATFORM_MT2625_H__ */
