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
 *  File Description :
 *  Emulates a mono-application 3G SIM, fill the SIM data and Auth response.
 **************************************************************************/

#define MODULE_NAME "USIMCUSTEMU"

# include  <system.h>

/******************************************************************************
 * Include Files
 *****************************************************************************/
#include <string.h>

#if !defined (KERNEL_H)
#   include  "kernel.h"
#endif

#if !defined (SIMDATA_H)
#   include  "simdata.h"
#endif

#if !defined (USIMEMU_H)
#include "usimemu.h"
#endif

#if !defined (USIMCUSTDEF_H)
#   include  "usimcustdef.h"
#endif

#if !defined (USIMCUST_H)
#   include  "usimcust.h"
#endif

#if !defined (SIMDEC_H)
#   include  "simdec.h"
#endif

#include "nbiot_modem_common_config.h"

#if defined (FEA_TEMP_ESIM_EMULATION)

void SimemuInitialiseFiles (SimImage* simImage_p)
{
    /* Setup the SIM Emulation image*/
    Int8 i = 0;

    simImage_p->dirEfData     = defDirEfData;
    memcpy(simImage_p->dirData, defDirData, sizeof(defDirData));
    simImage_p->uImsiEfData     = defImsiUsimEfData;
    memcpy(simImage_p->uImsiData, defImsiUsimData, sizeof(defImsiUsimData));
    /*file is less than 4 bytes, so to avoid codeguard warning, we simply copy the bytes one by one*/
    simImage_p->uAccEfData    = defAccUsimEfData;
    for (i=0; i < sizeof(defAccUsimData); i++)
    {
        simImage_p->uAccData[i] =  defAccUsimData[i];
    }
    /*file is less than 4 bytes, so to avoid codeguard warning, we simply copy the bytes one by one*/
    simImage_p->uHplmnEfData = defHplmnUsimEfData;
    for (i=0; i < sizeof(defHplmnUsimData); i++)
    {
        simImage_p->uHplmnData[i] =  defHplmnUsimData[i];
    }

    simImage_p->uFplmnEfData     = defFplmnUsimEfData;
    memcpy(simImage_p->uFplmnData, defFplmnUsimData, sizeof(defFplmnUsimData));

    simImage_p->iccidEfData     = defIccidEfData;
    memcpy(simImage_p->iccidData, defIccidData, sizeof(defIccidData));
    simImage_p->uAdEfData      = defAdUsimEfData;
    memcpy(simImage_p->uAdData, defAdUsimData, sizeof(defAdUsimData));
    simImage_p->uUstEfData     = defUstUsimEfData;
    memcpy(simImage_p->uUstData, defUstUsimData, sizeof(defUstUsimData));
    simImage_p->uUserPlmnSelEfData     = defUserPlmnSelUsimEfData;
    memcpy(simImage_p->uUserPlmnSelData, defUserPlmnSelUsimData, sizeof(defUserPlmnSelUsimData));
    simImage_p->uOperatorPlmnSelEfData     = defOperatorPlmnSelUsimEfData;
    memcpy(simImage_p->uOperatorPlmnSelData, defOperatorPlmnSelUsimData, sizeof(defOperatorPlmnSelUsimData));
    simImage_p->uHplmnSelEfData     = defHplmnSelUsimEfData;
    memcpy(simImage_p->uHplmnSelData, defHplmnSelUsimData, sizeof(defHplmnSelUsimData));
    simImage_p->uOplEfData     = defOplUsimEfData;
    memcpy(simImage_p->uOplData, defOplUsimData, sizeof(defOplUsimData));

    simImage_p->uEhplmnEfData = defEhplmnUsimEfData;
    memcpy(simImage_p->uEhplmnData, defEhplmnUsimData, sizeof(defEhplmnUsimData));

    simImage_p->epsLociEfData = defEpsLociEfData;
    memcpy(simImage_p->epsLociData, defEpsLociData, sizeof(defEpsLociData));
    simImage_p->nasSecurityEfData = defNasSecurityEfData;
    memcpy(simImage_p->nasSecurityData, defNasSecurityData, sizeof(defNasSecurityData));
    simImage_p->nasConfigEfData = defNasConfigEfData;
    memcpy(simImage_p->nasConfigData, defNasConfigData, sizeof(defNasConfigData));

}

void SimemuExecuteCustAuthenticate(SiCmdMsg *siCmd, SiResponse *siResp)
{
    Int8 dataOffset = 0;

    /* use dummy values based upon what the test equipment is expecting */
    siResp->data[dataOffset++] = 0xdb; /* AUTH_3G_SUCCESS */
    
    siResp->data[dataOffset++] = sizeof(defAuthSres); /*length of SRES*/
    memcpy(&siResp->data[dataOffset], defAuthSres, sizeof(defAuthSres));
    dataOffset += sizeof(defAuthSres);
    
    siResp->data[dataOffset++] = sizeof(defAuthCk); /*length of ck*/
    memcpy(&siResp->data[dataOffset], defAuthCk, sizeof(defAuthCk));
    dataOffset += sizeof(defAuthCk);
    
    siResp->data[dataOffset++] = sizeof(defAuthIk); /*length of ik*/
    memcpy(&siResp->data[dataOffset], defAuthIk, sizeof(defAuthIk));
    dataOffset += sizeof(defAuthIk);
    
    siResp->dataLength = dataOffset;
    siResp->dataIsValid = TRUE;
    siResp->sw1         = SW1_NORMAL;
    siResp->sw2         = SW2_NORMAL;
}
#endif

/* END OF FILE */
