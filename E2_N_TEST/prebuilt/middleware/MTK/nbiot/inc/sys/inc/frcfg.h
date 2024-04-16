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
 **************************************************************************
 * File Description: Generic Framework configuration
 **************************************************************************/

#ifndef FRCFG_H
#define FRCFG_H

/*******************************************************************************
** Docs
*******************************************************************************/

/*******************************************************************************
* Info        : Module file structure
* Group       : Framework
* Description : The following diagram illustrates the nominal file structure for
*               the physical layer framework.
*
*                  Image : frmodulestructure.png
*
*               A framework module will in general have a main platform
*               and application independent C and H file. In the above diagram
*               these are given by FRMMM.C and H, the MMM would be replaced by
*               the module name e.g. TCU, IRQ, BUF, PEL. The FRMMM.H include
*               file is the only file that application code should need to
*               include to access the functionality of a particular module.
*
*               Backwards compatibility files may be present to allow old code
*               using the Physical Layer Kernel (PLK) API to be compiled. These
*               are represented by FRMMBC.C and H in the above diagram.
*
*               Configuration files are normally present to allow platform
*               specifics to be configured. These are represented by
*               FRMMMCFG.C & H in the above diagram. Most of the configuration
*               in PLF is done using macros and defines. The main configuration
*               file will define defaults for configurable macros and defines,
*               these can then be overriden by defining them differently in a
*               platform specific configuration file. All platform specific
*               configuration files for a module are included at the top of that
*               modules configuration file under the relevant platform specific
*               conditional compile. The platform specific configuration files
*               are represented by FRPPPMMM.C in the above diagram. The PPP
*               would be replaced by the platform name while the MMM is the
*               framework module name.
*******************************************************************************/

/*******************************************************************************
** Includes
*******************************************************************************/

#include <system.h>

/*******************************************************************************
** Platform specific include files
** Conditionally include any platform specific files here. These can override
** the configurable defines and macros in this file.
*******************************************************************************/

#if defined (TTC_PLATFORM_RPV4_PLP)
# include <frrp4pcfg.h>
#endif

#if defined (TTC_PLATFORM_RPV4_MCU)
# include <frrp4mcfg.h>
#endif

#if defined (TTC_PLATFORM_RPV2_PLP)
# include <frrp2pcfg.h>
#endif

#if defined (TTC_PLATFORM_RPV2_MCU)
# include <frrp2mcfg.h>
#endif

#if defined (TTC_PLATFORM_S9C)
# include <frs9ccfg.h>
#endif

#if defined (TTC_PLATFORM_M25G_MCU)
# include <frm25gmcfg.h>
#endif

#if defined (TTC_PLATFORM_M25G_PLP)
# include <frm25gpcfg.h>
#endif

#if defined (TTC_PLATFORM_EX_MCU)
# include <frexmcfg.h>
#endif

#if defined (TTC_PLATFORM_EX_PLP)
# include <frexpcfg.h>
#endif

#if defined (TTC_PLATFORM_PC_SIM)
# include <frpccfg.h>
#endif

#if defined (UPGRADE_SPW6400_PLATFORM)
# include <frspw6400cfg.h>
#endif

#if defined (UPGRADE_DRACO_PLATFORM)
# include <frdracocfg.h>
#endif

#if defined (UPGRADE_CAPELLA_PLATFORM)
# include <frcapellacfg.h>
#endif

#if defined (PLATFROM_G1MCU) || defined (PLATFROM_G1DSP)
# include <frg1cfg.h>
#endif

#if defined (PLATFORM_POLEAXMCU) || defined (PLATFORM_POLEAXDSP)
# include <frplxcfg.h>
#endif

#if defined (PLATFORM_G2MCU) || defined (PLATFORM_G2DSP)
# include <frg2cfg.h>
#endif

#if defined (PLATFORM_S1MCU) || defined (PLATFORM_S1DSP)
# include <frs1cfg.h>
#endif

#if defined (MTK_PLATFORM_MT2625)
# include <fr2625cfg.h>
#endif

/*******************************************************************************
** Config Check
*******************************************************************************/

/*******************************************************************************
* Conditional : FR_ABP_2
* Group       : Framework, Config
* Description : Enable ABP 2.0 version of the Framework software. Without this
*                   conditional the framework code defaults to ABP 3.
*******************************************************************************/
/*******************************************************************************
* Conditional : FR_PLP_BUILD
* Group       : Framework, Config
* Description : This conditional compile indicates that a PLP build is being
*                   performed. If needed it should be defined in the platform
*                   specific configuration file included from this file.
*******************************************************************************/
/*******************************************************************************
* Conditional : FR_MCU_BUILD
* Group       : Framework, Config
* Description : This conditional compile indicates that a MCU build is being
*                   performed. If needed it should be defined in the platform
*                   specific configuration file included from this file.
*******************************************************************************/
/*******************************************************************************
* Conditional : FR_GKI_PRESENT
* Group       : Framework, Config
* Description : This conditional compile indicates GKI is also present on this
*                   processor. This enables any GKI functionality with the
*                   framework code.
*******************************************************************************/

/*******************************************************************************
* At least one of either FR_PLP_BUILD or FR_MCU_BUILD must be defined in the
* platform specific config file. Both might be defined in a simulation
* environment.
*******************************************************************************/
#if !defined (FR_PLP_BUILD) && !defined (FR_MCU_BUILD)
# error Must have at least one of FR_PLP_BUILD or FR_MCU_BUILD defined
#endif

/*******************************************************************************
** Config
*******************************************************************************/



#if !defined (FR_PKI_ACTION_NAMES)
# if defined (KI_ENABLE_RAM_LOGGING) && defined (KI_RLG_ENABLE_TCU_LOGGING)
   /* TCU RAM logging requires the action name lookup table */
#  define FR_PKI_ACTION_NAMES
# endif
#endif

/*******************************************************************************
** section info
*******************************************************************************/
#if !defined(PLATFORM_S1DSP)
/* file predominently containing critical code/data should exclude low priority code/data
 * from being placed in L1 memory by using the LOW attibute */
#ifdef FR_PLP_BUILD
    #ifdef PLP_DOWN_EMI_CLK
    #define FR_FUNC_ATTRIBUTE_LOW                  /*SECTION(".frinit.text")*/
    #define FR_DATA_ATTRIBUTE_LOW_BSS              /*SECTION(".frinit.bss")*/
    #define FR_DATA_ATTRIBUTE_LOW_CONST            /*SECTION(".frinit.rodata")*/
    #define FR_DATA_ATTRIBUTE_LOW_NOINIT           /*SECTION(".frinit.scommon")*/
    #else
    #define FR_FUNC_ATTRIBUTE_LOW                  SECTION(".frinit.text")
    #define FR_DATA_ATTRIBUTE_LOW_BSS              /*SECTION(".frinit.bss")*/
    #define FR_DATA_ATTRIBUTE_LOW_CONST            /*SECTION(".frinit.rodata")*/
    #define FR_DATA_ATTRIBUTE_LOW_NOINIT           /*SECTION(".frinit.scommon")*/
    #endif
#else
#if defined(CONFORMANCE_TESTING)

#define FR_FUNC_ATTRIBUTE_LOW
#define FR_DATA_ATTRIBUTE_LOW_BSS
#define FR_DATA_ATTRIBUTE_LOW_CONST
#define FR_DATA_ATTRIBUTE_LOW_NOINIT

#else
#define FR_FUNC_ATTRIBUTE_LOW                  SECTION(".frinit.text")
#define FR_DATA_ATTRIBUTE_LOW_BSS              SECTION(".frinit.bss")
#define FR_DATA_ATTRIBUTE_LOW_CONST            SECTION(".frinit.rodata")
#define FR_DATA_ATTRIBUTE_LOW_NOINIT           SECTION(".frinit.scommon")
#endif
#endif


/* file predominently containing non critical code/data should indicate critical code/data
 * using the HIGH attibute to be selectively placed in L1 memory */
#define FR_FUNC_ATTRIBUTE_HIGH                 SECTION(".frcritical.text")
#define FR_DATA_ATTRIBUTE_HIGH_BSS             SECTION(".frcritical.bss")
#define FR_DATA_ATTRIBUTE_HIGH_CONST           SECTION(".frcritical.rodata")
#define FR_DATA_ATTRIBUTE_HIGH_NOINIT          SECTION(".frcritical.scommon")

#if defined(PLATFORM_G2MCU) || defined(PLATFORM_G2DSP)
#define FR_FUNC_ATTRIBUTE_L2(func)             SECTION(".t1L2" #func) func
#else
#define FR_FUNC_ATTRIBUTE_L2(func)             func
#endif

#else
#define FR_FUNC_ATTRIBUTE_LOW
#define FR_DATA_ATTRIBUTE_LOW_BSS
#define FR_DATA_ATTRIBUTE_LOW_CONST
#define FR_DATA_ATTRIBUTE_LOW_NOINIT

#define FR_FUNC_ATTRIBUTE_HIGH
#define FR_DATA_ATTRIBUTE_HIGH_BSS
#define FR_DATA_ATTRIBUTE_HIGH_CONST
#define FR_DATA_ATTRIBUTE_HIGH_NOINIT


#define FR_FUNC_ATTRIBUTE_L2(func)             func

#endif

/*******************************************************************************
** Constants
*******************************************************************************/

/*******************************************************************************
** Macros
*******************************************************************************/
#if !defined M_WaitforWBEmpty
#define M_WaitforWBEmpty()
#endif

#if !defined (M_ReadReg32)
#define M_ReadReg32(regAddr)           (* (volatile Int32*) (regAddr))
#endif

#if !defined (M_WriteReg32)
#define M_WriteReg32(regAddr, regData)\
        (* (volatile Int32*) (regAddr)) = (Int32)(regData);
#endif

#if !defined (M_WriteReg16)
#define M_WriteReg16(regAddr, regData)\
        (* (volatile Int16*) (regAddr)) = (Int16)(regData);
#endif

#if !defined (M_WriteReg32Safe)
#define M_WriteReg32Safe(regAddr, regData)\
        (* (volatile Int32*) (regAddr)) = (Int32)(regData);\
           M_WaitforWBEmpty();
#endif

#if !defined (M_WriteReg16Safe)
#define M_WriteReg16Safe(regAddr, regData)\
        (* (volatile Int16*) (regAddr)) = (Int16)(regData);\
           M_WaitforWBEmpty();
#endif

#if !defined (M_WriteReg16SafeFunc)
#define M_WriteReg16SafeFunc(regAddr, regData)\
        (* (volatile Int16*) (regAddr)) = (Int16)(regData);\
        WaitforWBEmpty();
#endif


#endif
/* END OF FILE */
