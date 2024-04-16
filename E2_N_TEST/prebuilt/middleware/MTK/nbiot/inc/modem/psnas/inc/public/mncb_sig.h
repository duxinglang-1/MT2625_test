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
 *  File Description :                                                   */
/** \file
 *      Cb - application interface
 **************************************************************************/

#ifndef MNCB_SIG_H
#define MNCB_SIG_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#include <l123_typ.h>
#include <cbsi_typ.h>
#include <grrcbsig.h>

/** \defgroup SigMNxxCb  MNCB Primitives
 * \ingroup SigMNxx
 * Primitives exchanged between CB and AL
 * This section defines primitives exchanged between CB and AL
 *
 * @{
 */
/****************************************************************************
 * Macros
 ****************************************************************************/

/** Maximum length of one page cell broadcast message*/
#define CB_MAX_MESSAGE_LENGTH       93
/** Maximum length of whole cell broadcast message*/
#define CB_MAX_MACRO_MESSAGE_LENGTH 1395

/** Maximum number of different cell broadcast messages that can be
 * simultainiously sought (It has an impact on internal CB arrays 
 * that are statically allocated) */
#define CB_MAX_NUM_MESSAGES         100

/** This number could match GSM_LANG_NUMBER_OF, max number of languages,
 * that if we want to enable all available languages  */
#define MAX_LP_ENTRIES              22

/** Maximum number of different cell broadcast message data download */
#define MAX_CBMID_ENTRIES           5

/** Number of bytes contain in a configure CBMI signal*/
#define CBMI_PAGE_SIZE              200
/** Number of configure CBMI signal that will be sent in order
*   to configure all the CBMI entries.*/
#define CBMI_NUM_PAGE               3
/** The capacity needs to store the full CBMI configuration*/
#define CBMI_NUM_BYTES              (CBMI_PAGE_SIZE*CBMI_NUM_PAGE)
/** The number of CBMI entries managed by the system. for all the entries
*   not managed ( interval [CBMI_NUM_ENTRIES+1 - 0xFFFF]), the received
*   SMS-CB for them will be discarded.*/
#define CBMI_NUM_ENTRIES            (CBMI_NUM_BYTES*8) /* 200*3*8 = 4800*/
/** The last entrie managed by the current system.
*   For the moment, the next entries definition aren't specified by the spec
*   so we ignore it and refuse CB with such messageID.
*/
#define CBMI_LAST_VALID_ENTRY       (CBMI_NUM_ENTRIES-1)

/****************************************************************************
 * Types
 ****************************************************************************/

/** Page modes which determine how the application layer will receive the 
 * message data
 */
typedef enum CbPageModeTag
{
    /** The application layer will receive the pages 
     * of the messages as and when they arrive.
     */
    CB_INDIVIDUAL_PAGES,
    /** The application layer will receive the whole 
     * cell broadcast message when all the individual pages have been received
     */
    CB_FULL_MACRO_MESSAGE,
    /** No more page modes*/
    noMorePageModes
}
CbPageMode;

/** Geography scope element */
typedef enum CbGeoScopeTag
{
    /** The message is only cell wide (Any message if received in the 
     * next cell is regarded as new) and the message is supposed to be 
     * on the display all the time */ 
    GS_IMMEDIATE_CELL_WIDE,
    /** The  Message Code and/or Update Number must change 
     * in the next cell for the message to be new and the message 
     * is supposed to be on the display only when the user wants to see it  
     */
    GS_NORMAL_PLMN_WIDE,
    /** The message with the same Message Code and Update Number may or 
     * may not be "new" in the next cell according to whether the next cell 
     * is in the same Location Area as the current cell.and the message 
     * is supposed to be on the display only when the user wants to see it  
     */
    GS_NORMAL_LOCATION_AREA_WIDE,
    /** The message is only cell wide (Any message if received in the 
     * next cell is regarded as new) and the message is supposed to be 
     * on the display only when the user wants to see it  */ 
    GS_NORMAL_CELL_WIDE
}
CbGeoScope;

/****************************************************************************
 * Signal types
 ****************************************************************************/

/** This signal configures the CBMI filter and follow the MncbConfigureReq
*   signal. It contain the desirated state for every CBMI/
*/
typedef struct MncbConfigureCbmiReqTag
{
    /** The signal page number starting to index 1*/
    Int8    pageNum;
    /** The total page number (must be equals to CBMI_NUM_PAGE)*/
    Int8    pageTotal;
    /** The CBMI configuration as a bit field with each bit a message type.*/
    Int8    messageTypeList[CBMI_PAGE_SIZE];
}MncbConfigureCbmiReq;

/** This signal configures the cell broadcast unit and ultimately the layer 1 
 * sub-system.
 * This signal configures the cell broadcast unit and ultimately the layer 1 
 * sub-system to receive cell broadcast data while the system is in idle mode 
 * (i.e. no dedicated connection to the GSM network). Any previous configuration 
 * and associated messages that have been part received will be discarded. 
 * All internal variables will be reset. The cell broadcast service may be turned 
 * off by sending a configure request with the numberOfTypes parameter set to zero.
 */
typedef struct MncbConfigureReqTag
{
    /** If equals to 0, the system will be configured to not receive cell broadcast
     * data (Note that the system will only receive cell broadcast data in
     * idle mode) and the signal will not be followed by MncbConfigureCbmiReq
     * signal. Else it is the number of active CBMI in the CBMI filter list.
     */
    Int16                           cbmiActiveEntries;
    /** Indicates the number of languages in languageList 
\assoc ARRAY \ref languageList */
    Int16                           numberOfLanguages;
    /** As the same message may be broadcast in a variety of languages, 
     * the languageList parameter specifies those languages, in priority 
     * order, that the user may wish to receive. A message that is received 
     * in a language not defined in the language list will be discarded, 
     * regardless of the message type. If a message of the same message 
     * type has been received in a lower priority language that the current 
     * message (of the same type), the application layer will be provided 
     * with the same message in the new language.
     */
    Language                        languageList[MAX_LP_ENTRIES];
    /** Specifies how the application layer will receive the message data.
     */
    CbPageMode                      pageMode;
    /** Tells the protocol stack to receive messages on the CBCH which 
     * have not been scheduled but which are marked as optional reading.  
     * Setting this flag will enable the MS to get CBCH schedule data 
     * soon as possible at the expense of battery power.
     */
    Boolean                         receiveOptionalFreeMessages;
    /** Tells the protocol stack to receive messages on the CBCH 
     * which have not been scheduled but which are marked as advised reading. 
     * Setting this flag will enable the MS to get CBCH messages as soon as 
     * possible, at the expense of battery power.
     */
    Boolean                         receiveAdvisedFreeMessages;
    /** Indicates the number of CBMID 
\assoc ARRAY \ref cbmidTypeList */
    Int8                            numOfCbmidTypes;
    /** Lists CBMID context data */
    Int16                           cbmidTypeList[MAX_CBMID_ENTRIES];
}
MncbConfigureReq;

/** This signal provides the application layer with a single page of a cell 
 * broadcast message in a language that has been requested.
 */
typedef struct MncbMessageIndTag
{
    /** Indicates binary the page number 
     */
    Int8                            pageNumber;
    /** indicates the binary value of the total number of pages in the message
     * (allow the application layer to re-build the macro message itself).*/
    Int8                            totalNumberOfPages;
    /** This parameter indicates the intended handling of the message at the MS,
     * the alphabet/coding, and the language (when applicable)
     */
    CbDataCoding                    dataCoding;
    /** RawDcs  */
    Int8                            rawDcs;
    /** Indicates the geographical area over which the Message Code 
     * is unique, and the display mode.
     */
    CbGeoScope                      geoScope;
    /** Differentiates between messages from the same source and type  */
    Int16                           messageCode;
    /** Indicates a change of the message content of the same message
     * (the message with the same Message Identifier, Geographical Scope, 
     * and Message Code).
     */
    Int8                            updateNumber;
    /** Identifies the source and type of the message */
    Int16                           messageId;
    /** Indicates the length of the message 
\assoc ARRAY \ref message */
    Int16                           messageLength;
    /** Lists single page cell broadcast message 
\assoc STRING   */
    Int8                            message[CB_MAX_MESSAGE_LENGTH];
}
MncbMessageInd;

/** This signal provides the application layer with a whole cell 
 * broadcast message in a language that has been requested.
 */
typedef struct MncbMacroMessageIndTag
{
    /** This parameter indicates the intended handling of the message at the MS,
     * the alphabet/coding, and the language (when applicable)
     */
    CbDataCoding                    dataCoding;
    /** Indicates the geographical area over which the Message Code 
     * is unique, and the display mode.
     */
    CbGeoScope                      geoScope;
    /** Differentiates between messages from the same source and type  */
    Int16                           messageCode;
    /** Indicates a change of the message content of the same message
     * (the message with the same Message Identifier, Geographical Scope, 
     * and Message Code).
     */
    Int8                            updateNumber;
    /** Identifies the source and type of the message */
    Int16                           messageId;
    /** Indicates the length of the message 
\assoc ARRAY \ref message */
    Int16                           messageLength;
    /** Lists whole cell broadcast message */
    Int8                            message[CB_MAX_MACRO_MESSAGE_LENGTH];
    /** RawDcs  */
    Int8                            rawDcs;                                  /*job 106638*/
}
MncbMacroMessageInd;

/** This signal provides the application layer with information 
 * on the new cell when a cell change occurs.
 */
typedef struct MncbCellChangeIndTag
{
    /** Indicates if cell broadcast is supported */
    Boolean                         cbSupported;
    /** Determines frequency info */
    Arfcn                           arfcn;
    /** Identifies uniquely a location area */
    Lai                             lai;
    /** Identifies the cell */
    Int32                           cellId;
}
MncbCellChangeInd;

/** @} */ /* End of SigMNxxCb group */
#endif
/* END OF FILE */

































