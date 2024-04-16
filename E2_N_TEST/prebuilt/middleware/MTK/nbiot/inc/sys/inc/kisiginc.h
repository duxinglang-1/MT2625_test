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
 * This file includes all the signal definition include files for each set
 * (*siginc.h). This file is then included into both the Signal union
 * (in KISIGUNI.H) and the #SignalId enumeration (in PSSIGNAL.H).
 *
 * There is no multiple inclusion protection as this file can be included
 * more than once in the same file.
 *
 * If you add a new include into this file you may also need to add an
 * include into KISIGUNI.H that defines the signal structures. Building a
 * signal database (that includes the new signals) in a PC test tool
 * (e.g. Genie, use Signal Maker to check signals appear with the correct
 * structures) will check that signal definitions have been correctly
 * added to the system. */

/***************************************************************************
 * IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT
 *
 * Please read the file description above as it contains information on
 * other changes that might be needed if you change this file.
 *
 * IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT
 ***************************************************************************/

/** \def EXS_SIGNALS
 * \ingroup PrdGki
 * Enable silicon client signal set.
 * This conditional enables the silicon client specific signal set. This can
 * be used by a silicon client to add their own signals specific to their
 * platform. */

/* SYS Sig set */
#include <syssiginc.h>

/* EX sig set */
/*#include <exsig.h>*/

/* EXS sig set */
#if defined (EXS_SIGNALS)
# include <exssig.h>
#endif

/* L1 sig set */
#include <l1siginc.h>

/* DM sig set */
#include <dmsiginc.h>

/* PS_COM sig set */
#include <pssiginc.h>

/* PS_GSM sig set */
#include <psgssiginc.h>

/* PS_3G sig set */
#include <ps3gsiginc.h>

#if defined (UPGRADE_LTE)
/* PS_LTE sig set */
#include <ps4gsiginc.h>
#endif /* UPGRADE_LTE */

/* AB sig set */
#include <absiginc.h>

/* TEST sig set */
#include <testsiginc.h>

/* MEDIA sig set */
#include <mediasiginc.h>

/**os driver test:
*_main_sigdef.h should be the first file since its has the signal base OSDRIVER_TEST_SIGNAL_BASE;
*other sigdef files of os driver of diffrent modules list after _main_sigdef.h
*/
#if defined (OSDRIVERS_TEST_ENABLE)
#include <osdriver_test_main_sigdef.h>

#if defined (OSDRV_GKI_PORTING_TEST)
/*Gki test sig set*/
#include <gki_porting_test_siginc.h>
#endif

#if defined (MEMORY_TEST_ENABLE)
/* memory test sig set */
#include <memory_test_siginc.h>
#endif


#endif

#if defined (NB_PC_HW_SIM)
#include <nb_pc_sig.h>
#endif

#if defined (NB_PC_L1_NET_SIM)
#include <nb_pc_l1netsim_sig.h>
#endif

#if defined (UE_SIMULATOR)
#include <nhlhwsim_sig.h>
#endif

/* END OF FILE */

