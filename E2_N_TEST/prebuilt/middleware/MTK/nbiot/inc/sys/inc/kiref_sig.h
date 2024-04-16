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
 *************************************************************************/
/** \file
 * GKI header file that defines the signal structures of referenced data. 
 * Referenced data are C structures for which GKI signals only have members that 
 * point to them. 
 * The following definitions permit to log these data structures
 * in a dedicated GKI signal (contained within SYS signal base)
 **************************************************************************/

#ifndef KI_REFSIG_H
#define KI_REFSIG_H

/****************************************************************************
 * Referenced Data Signal Types
 ****************************************************************************/

/* Define the macros to create the associations logging functions */
#define SIGNAL REF_LOGGING_ASSOC
#include <kisigdef.h>

/** Enumeration of referenced types
 * Enumerated type to provide the union member association to
 * resolve in Genie the union typedef to use. */
typedef enum RefUnionAssociationMemberTag
{
# include <kisiginc.h>
  ASSOC_LAST_MEMBER_ASSOCIATION
}RefUnionAssociationMember;

/* Define the macros to create payload type declarations */
#define SIGNAL REF_LOGGING_PAYLOAD
#include <kisigdef.h>

/** Referenced Payload Data.
 * Creation of the union which will have all the typedefs for those structures
 * which are not sent by signals but only referenced by pointer members */
typedef union ReferencedPayloadsTag
{
# include <kisiginc.h>
  EmptySignal refEmpty;
}ReferencedPayloads;

/** Logging referenced data indication.
 * This signal is sent when the GKI has logged a signal which contains
 * referenced data. This signal will contain the contents of the 
 * referenced data */
typedef struct ReferencedDataIndTag
{
  /* This is the signal id of the original signal which contained the pointer
   * member; used on PC with addressRefData to uniquely identify the member 
   * in a structure to which the block of data belongs. Application on the 
   * PC will use this refSignalId to create an association with the original
   * GKI signal with that ID. */
  SignalId refSignalId;
  /* This is pointer address value of the pointer for which we are logging 
   * the referenced data. Needed with refSignalId for resolution on PC of 
   * multi-nested pointer members. If a custom reference type is used 
   * then this will be set to PNULL */
  Int32    addressRefData;

	/** This enumerated type will permit to resolve the typedef to use for 
   * the union ReferencedPayloads 
\assoc UNION \ref payload */
  RefUnionAssociationMember id;

  /* Union of all the typedefs which are referenced by pointer
   * members in GKI signals. */
  ReferencedPayloads payload;
}ReferencedDataInd;

#endif

/* END OF FILE */
