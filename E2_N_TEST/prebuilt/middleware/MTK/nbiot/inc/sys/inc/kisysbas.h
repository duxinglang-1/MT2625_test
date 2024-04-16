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
 **************************************************************************/
/** \file
 * GKI header file that defines the signal sets and includes the base
 * definition files for each set (*sigset.h files).
 **************************************************************************/

/*******************************************************************************
** Define dummy macros if they are not already defined.
*******************************************************************************/
#if !defined (KI_BASE_DEF)
# define KI_BASE_DEF(bASEnAME)
#endif
#if !defined (KI_SIGSET_START)
# define KI_SIGSET_START(sETnAME)
#endif

/*
** This file deliberately does not have a
** #if !defined KISYSBAS_H
** as it can be legitimately included twice
**
** This must never be included directly by any file outside of the kernel.
**
** WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
**
** DO NOT CONDITIONALLY COMPILE SIGNAL SETS. ALL SIGNAL SETS MUST ALWAYS
** BE PRESENT TO PREVENT SIGNAL IDS CHANGING. THE CONTENTS OF SIGNAL SETS
** CAN BE CONDITIONALLY COMPILED BUT THE NUMBERING OF BASES WITHIN THE
** SETS SHOULD ALSO BE CONSTANT (ADDITIONS AT THE END OF SETS ONLY).
**
** NEW SETS MUST ONLY BE ADDED AT THE END OF CURRENT SETS. A SIGNAL SET SHOULD
** ONLY BE ADDED IF THE NEW BASES DO NOT READILY ASSOCIATE WITH ONE OF THE
** EXISTING SETS. PLEASE CHECK WITH A SYSTEM ARCHITECT IF UNSURE.
**
** WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
*/

/*******************************************************************************
* Macro       : KI_BASE_DEF
* Group       : GKI, Config
* Parameter   : name bASEnAME = This is the unique base name that will have
*                   _SIGNAL_BASE appended to it.
* Description : The KI_BASE_DEF macro is used to define a signal base. This is
*                   normally placed in a XXsigset.h file or exsigbas.h for
*                   terminal clients or exssigbas.h for silicon clients.
*******************************************************************************/
/*******************************************************************************
* Macro       : KI_SIGSET_START
* Group       : GKI, Config
* Parameter   : name sETnAME = This is the unique signal set name.
* Description : The KI_SIGSET_START macro is used to define a signal set. This
*                   must only be used in this file, new sets must only be added
*                   after existing sets and they must not be conditionally
*                   compiled (their contents can be if needed).
*******************************************************************************/

KI_SIGSET_START(SYS)
    /***************************************************************************
    ** System/GKI Signal Set, Generic system use only, not product specific
    ***************************************************************************/
    #include <syssigset.h>

KI_SIGSET_START(TEST)

    /***************************************************************************
    ** Test Signal Set
    ***************************************************************************/
    #include <testsigset.h>

KI_SIGSET_START(EXS)
    /***************************************************************************
    ** External Silicon Client Signal Set, not for TTPCom use
    ***************************************************************************/
    #include <exssigset.h>

KI_SIGSET_START(L1)
    /***************************************************************************
    ** Layer 1 Signal Set
    ***************************************************************************/
    #include <l1sigset.h>

KI_SIGSET_START(DM)
    /***************************************************************************
    ** Device Management Signal Set
    ***************************************************************************/
    #include <dmsigset.h>

KI_SIGSET_START(PS_COM)
    /***************************************************************************
    ** Protocol Stack Common Signal Set
    ***************************************************************************/
    #include <pssigset.h>

KI_SIGSET_START(PS_GSM)
    /***************************************************************************
    ** Protocol Stack GSM/GPRS Signal Set
    ***************************************************************************/
    #include <psgssigset.h>

KI_SIGSET_START(PS_3G)
    /***************************************************************************
    ** Protocol Stack 3G Signal Set
    ***************************************************************************/
    #include <ps3gsigset.h>

KI_SIGSET_START(PS_LTE)
    /***************************************************************************
    ** Protocol Stack LTE Signal Set
    ***************************************************************************/
    #include <ps4gsigset.h>

KI_SIGSET_START(AB)
    /***************************************************************************
    ** Application Background Signal Set
    ***************************************************************************/
    #include <absigset.h>

KI_SIGSET_START(AF)
    /***************************************************************************
    ** Application Foreground Signal Set
    ***************************************************************************/
    #include <afsigset.h>

KI_SIGSET_START(BT)
    /***************************************************************************
    ** Bluetooth Signal Set. * Empty *
    ***************************************************************************/

KI_SIGSET_START(MEDIA)
    /***************************************************************************
    ** Media Signal Set
    ***************************************************************************/
    #include <mediasigset.h>

/*******************************************************************************
** Undefine the signal set and base macros. This must be after all signal set
** and base definitions (i.e. at the end of the file).
*******************************************************************************/
#if defined (KI_BASE_DEF)
# undef KI_BASE_DEF
#endif
#if defined (KI_SIGSET_START)
# undef KI_SIGSET_START
#endif

/* END OF FILE */
