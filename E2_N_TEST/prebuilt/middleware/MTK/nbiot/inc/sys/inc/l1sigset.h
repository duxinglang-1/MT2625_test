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
 * File Description : Signal Base definitions for the  set.
 **************************************************************************/

/*
** This file deliberately does not have multiple inclusion protection
** as it can be legitimately included twice in the same file.
**
** The KI_BASE_DEF() macro adds _SIGNAL_BASE to the end of the parameter
** given as the base name. This is done to enforce the GKI naming convention.
**
** This must never be included directly by any file outside of the kernel.
**
** WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
**
** THE BASES WITHIN A SIGNAL SET SHOULD NOT BE CONDITIONALLY COMPILED
** IF POSSIBLE AS THIS CAN CAUSE SIGNAL ID'S TO MOVE AROUND. NEW BASES MUST
** BE ADDED TO THE END OF THE SET (OR USE A REDUNDANT ENTRY) TO MAINTAIN
** THE BASE NUMBERING FOR THE EXISTING BASES. IF A BASE IS CONDITIONALLY
** COMPILED THE ELSE CONDITION MUST HAVE RESERVED BASES SO THAT THE
** NUMBERING IS CONSTANT WHETHER THE CONDITIONAL IS ON OR NOT.
**
** FILES SHOULD NOT BE INCLUDED INTO THIS FILE AS ANY ADDITIONS TO THE BASES
** WITHIN THE INCLUDED FILE WILL AFFECT THE NUMBERING OF ALL BASES DEFINED
** AFTER THE INCLUSION IN THIS FILE.
**
** WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
*/

/***************************************************************************
** Layer 1 Signal Set
***************************************************************************/
KI_BASE_DEF (MPH)
KI_BASE_DEF (PH)
KI_BASE_DEF (TI)
KI_BASE_DEF (L1UT)
KI_BASE_DEF (CDMG)
KI_BASE_DEF (GPGMPH)
KI_BASE_DEF (L1PROF)
KI_BASE_DEF (MIPSMAN)
KI_BASE_DEF (UMPH)
KI_BASE_DEF (CPHY)
KI_BASE_DEF (UPHY)
KI_BASE_DEF (U1CD)
KI_BASE_DEF (U1IT)
KI_BASE_DEF (L1TEST)
KI_BASE_DEF (L1MM)
KI_BASE_DEF (U1MM)
KI_BASE_DEF (ECPHY)
KI_BASE_DEF (ESPHY)
KI_BASE_DEF (EUPHY)
KI_BASE_DEF (E1CD)
KI_BASE_DEF (E1IT)
KI_BASE_DEF (VT)
KI_BASE_DEF (E1TEST)
KI_BASE_DEF (EMPH)
KI_BASE_DEF (NPHY)
KI_BASE_DEF (N1CD)
KI_BASE_DEF (N1TST)
