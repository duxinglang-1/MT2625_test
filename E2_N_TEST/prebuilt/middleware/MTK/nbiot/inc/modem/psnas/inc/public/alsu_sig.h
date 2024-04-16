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
 *      3G phonebook application layer - sim manager interface
 **************************************************************************/

#if !defined (ALSU_SIG_H)
#define       ALSU_SIG_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/
#if !defined (ALSI_TYP_H)
#include     <alsi_typ.h>
#endif

#if !defined (ALSU_TYP_H)
#include     <alsu_typ.h>
#endif


/** \defgroup SigAlsu  ALSU Signalling Interface
 * \ingroup Prd3gDmNas
 * \ingroup Sig
 * Interface between USIM Manager and the Application Background Layer.
 * This interface is based on the phonebook-related files held on the UICC
 * as defined in the Specification TS 102 221 and TS 31.102.
 * The ALSU interface is specifically for access of the 3G phonebook on the UICC both global
 * and local phonebooks and all associated data emails, second name entries, synchronization
 * information etc and the signals are passed between the USIM Manager and the Application
 * Background Layer specifically ABLM.
 *
 * @{
 */

/** Email address size */
#define SIM_UICC_EMAIL_ADDRESS_SIZE     100

/** The maximum number of phonebook control records in the list */
#define SIM_MAX_NUM_PBC_RECORD_LIST     20
/** maximum size of group alpha strings list */
#define SIM_MAX_GAS_LIST                4
/** The maximum size of AAS list */
#define SIM_MAX_AAS_LIST                4
/*this number is specified by 131 102, so it should not be changed*/
#define SIM_MAX_NUM_GRP                 10
/** The length of hidden key value*/
#define SIM_HIDDEN_KEY_LENGTH           4

/** Specifies the type of phonebook (Local/Global) */
typedef enum SimUiccPhoneBookTypeTag
{
    /** Global phonebook */
    GLOBAL_PHONEBOOK,
    /** Local phonebook */
    LOCAL_PHONEBOOK
} SimUiccPhoneBookType;

/** Contains a list of email address data */
typedef struct SimUiccEmailDataTag
{
    /** Indicates the length of the list */
    Int8                     length;
    /** Holds a list of email address data */
    Int8                     data[SIM_UICC_EMAIL_ADDRESS_SIZE];
} SimUiccEmailData;

/** Contains a list of groups data */
typedef struct SimUiccGrpDataTag
{
    /** Indicates the number of groups */
    Int8                     numOfGrp;
    /** Holds a list of record numbers in the GAS file to identify
     * the selected groups.
     */
    Int8                     grpList[SIM_MAX_NUM_GRP];
} SimUiccGrpData;

/** This structure holds the details of one record from the Phonebook
 * Control file indicating if the entry is hidden or has been modified
 * by a GSM handset
 */
typedef struct SimUiccPbcRecordTag
{
   /** Hidden entry */
   Int8      hiddenEntry;
   /** Modified entry */
   Boolean   modifiedEntry;
} SimUiccPbcRecord;


/** This structure is able to hold an additional number.
 * Same structure as SimDialNum but without the alphaId
 */
typedef struct SimUiccAnrTag
{
    /** Set FALSE if SSC string etc. */
    Boolean                         isDiallingNumber;
    /** Type of Number (TON) */
    BcdNumberType                   typeOfNumber;
    /** Numbering Plan Id (NPI) */
    BcdNumberPlan                   numberPlan;
    /** The length of the number*/
    Int8                            dialStringLength;
    /** The number*/
    Bcd                             dialString[SIM_MAX_DIAL_NUMBER_LENGTH];
    /** Is a sub address for this dial number */
    Boolean                         subAddressDefined;
    /** called party subaddress */
    CalledPartySubaddress           calledPartySubaddress;

}
SimUiccAnr;

/** This enumeration holds identifiers for the  different hidden key functions.*/
 typedef enum SimUiccHiddenKeyFunctionTag
 {
     /** Hidden key verfy */
     SIM_HIDDEN_KEY_VERIFY = 0x00,
     /** Hidden key change */
     SIM_HIDDEN_KEY_CHANGE = 0x01

 } SimUiccHiddenKeyFunction;

 /** This structure is able to hold the hidden key required to view the
  * hidden phonebook entries.
  */
 typedef struct SimUiccHiddenKeyTag
 {
     /** Hidden key value */
     Int8  value[SIM_HIDDEN_KEY_LENGTH];
 } SimUiccHiddenKey;


 /** \defgroup SigAlsuSignals  ALSU Primitives
 * Primitives exchanged between  USIM Manager and the Application Background Layer (ABLM).
 * This section defines primitives exchanged between USIM Manager and the Application
 * Background Layer (ABLM).
 *
 * @{
 *
 */
/****************************************************************************
 * Signal types
 ****************************************************************************/
 /*-----------------23/07/2003 14:29-----------------------*/
 /** Signal to request some information about the phonebook.
  * When the USIM manager receives this request, it gathers different
  * information about the phonebook structure on the SIM/UICC.
  * This request signal should be sent by the BL imperatively before
  * any other phonebook-related request is sent.
  *
  * In the case when a 3G UICC is inserted, the USIM manager will read EF
  * PBR file. EF PBR describes the structure of the phonebook, and which
  * files (like EMAIL, ANR, SNE) are supported.  The USIM manager will
  * then select those files in order to determine for instance how many
  * records they support or what their record Length is.
  *
  * In the case when a GSM SIM is inserted in the terminal, the USIM manager
  * will select the ADN file, and determine how many records are supported by
  * the SIM, the length of the alpha ID, and whether EXT1 file and CCP files are
  * also supported.
  */
 typedef struct AlsuPhoneBookStatusReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
    /** Whether to access global or local phonebook */
    Boolean                         globalPhoneBook;

} AlsuPhoneBookStatusReq;

 /** This primitive is sent as a response to AlsuPhoneBookStatusReq.
  * Note: GSM SIMs are not able to support storage for additional
  * data such as EMAIL, ANR, SNE. Thus the different available
  * flags in the AlsuPhonebookStatusCnf for those files would be set to FALSE in this case.
  */
typedef struct AlsuPhoneBookStatusCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful */
    SimRequestStatus                requestStatus;
    /** Whether to access global or local phonebook*/
    Boolean                         globalPhoneBook;
    /** Indicates the total number of ADN entries (for instance,
     * if the phonebook includes ADN1 and ADN2 files, then
     * adnNumRecords = number of ADN1 records + number of ADN2 records)
     */
    Int16                           adnNumRecords;
    /** Indicates the maximum length of the alpha ID for the ADNs files
     * (The ADN files in each PBR record should have the same record length,
     * hence the length of their alpha ID should be the same).
     */
    Int8                            adnAlphaIdLength;
    /** Indicates whether a Second Name Entry is supported by the UICC */
    Boolean                         sneAvailable;
    /** Indicates the number of bytes available to store the SNE */
    Int8                            sneAlphaIdLength;
    /** Indicates how many email addresses per phonebook entry can be stored on the UICC*/
    Int8                            emailNumFiles;
    /** Indicates the maximum length of the EMAIL addresses. */
    Int8                            emailAddressLength;
    /** Indicates how many Additional numbers per phonebook entry can be stored on the UICC.*/
    Int8                            anrNumFiles;
    /** The length of additional per phonebook entry */
    Int8                            anrLength;
    /** Indicates whether storage for grouping information is available on the UICC
     * ( If this is set to TRUE, then both EF GRP and EF GAS should be present on the UICC.)
     */
    Boolean                         groupInfoAvailable;
    /** Indicates the length of the GRP record (If grpLength is set to value x,
     * this means that each phonebook entry can belong to a maximum of x groups)
     */
    Int8                            grpLength;
    /** Indicates whether the EXT1 file is available on the SIM/UICC*/
    Boolean                         ext1Available;
    /** Indicates whether EF PBC is available on the UICC.*/
    Boolean                         pbcAvailable;
    /** Indicates the maximum number of groups that the UICC is able to store.*/
    Int8                            gasNumRecords;
    /** Indicates the length of the GAS records */
    Int8                            gasLength;
    /** Indicates whether EF AAS is present on the UICC.
    * AAS (additional Alpha String) file stores the alpha ID associated with
    * the additional numbers. (AAS are likely to store strings like mobile
    * work home. They basically give an indication about the type of
    * additional number)
    */
    Boolean                         aasAvailable;
    /** Indicates the maximum permissible AAS length,
     * It is basically the length of the additional string that
     * we can store on the UICC
     */
    Int8                            aasLength;
    /** The record number of AAS */
    Int8                            aasNumRecords;
    /** Indicates whether EF CCP1 is present on the UICC
     * (EF CCP1 stores bearer capability information).
     */
    Boolean                         ccpAvailable;
    /** Indicates whether the files related to synchronisation
    * (such as EF UID, EF PSC, EF CC, EF PUID) are present on the UICC.
    */
    Boolean                         syncSupported;
    /** Whether or not hidden entries are supported */
    Boolean                         hiddenAvailable;
    /** Indicates if ADN is invalidated */
    Boolean                         adnInvalidated;
} AlsuPhoneBookStatusCnf;

/*-----------------23/07/2003 14:29-----------------*/
 /** signal to read EF PBR file.
  * Reads a record (indicated by RecordNumber) from EF PBR (phonebook reference).
  * The allowed values for RecordNumber are from 1 to 254.  EF PBR reports the
  * files present to support the phonebook.  This signal will be sent by BL in
  * DEVELOPMENT VERSION only.  This is used mainly for debugging.
  */
 typedef struct AlsuReadPbrRecReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
    /** Whether to access global or local phonebook*/
    Boolean                         globalPhoneBook;
    /** PBR record number*/
    Int8                            recordNumber;
}AlsuReadPbrRecReq;

 /** Confirmation to the AlsuReadPbrRecReq.
  * Returns the contents of a decoded PBR record (indicated by RecordNumber )
  * from EF PBR (phonebook reference).
  */
typedef struct AlsuReadPbrRecCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful */
    SimRequestStatus                requestStatus;
    /** Whether to access global or local phonebook*/
    Boolean                         globalPhoneBook;
    /** PBR record number to access*/
    Int8                            recordNumber;
    /** Content of the PBR record: description of files present, etc...*/
    SimUiccPbrRecord                simUiccPbrRecord;
    /** Total number of PBR records */
    Int8                            totalNumPbrRecords;
} AlsuReadPbrRecCnf;

/*-----------------23/07/2003 14:29-----------------*/
/** Signal to read EF HIDDEN KEY file */
/*--------------------------------------------------*/
typedef struct AlsuReadPbHiddenKeyReqTag
{
   /** Command reference which is mirrored in the confirmation signal */
   Int16                           commandRef;


} AlsuReadPbHiddenKeyReq;

/** Confirmation to the AlsuReadPbHiddenKeyReq.*/
typedef struct AlsuReadPbHiddenKeyCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful */
    SimRequestStatus                requestStatus;
    /** Hidden key value */
    Int32                           hiddenKey;

} AlsuReadPbHiddenKeyCnf;


/*-----------------23/07/2003 14:29-----------------*/
/** Signal to verify or change the HIDDEN KEY.
 * May be used to perform different operations on the hidden key: <UL>
 * <LI> Verifies that the hidden key value provided by the user is
 * the same as the one stored in EF HIDDEN key. (This file stores the
 * Pin code required to read Hidden entries in the phonebook.)
 * <LI> Change the hidden key value. </ul>
 */
typedef struct AlsuHiddenKeyFunctionReqTag
{
   /** Command reference which is mirrored in the confirmation signal */
   Int16                           commandRef;
   /** Verify or change the HIDDEN KEY */
   SimUiccHiddenKeyFunction        hiddenKeyFunction;
   /** Holds the hidden key value */
   SimUiccHiddenKey                hiddenKey;
   /** Holds the new hidden key if the hidden key value has been changed. */
   SimUiccHiddenKey                newHiddenKey;
} AlsuHiddenKeyFunctionReq;

/** Confirmation to the AlsuHiddenKeyFunctionReq. */
typedef struct AlsuHiddenKeyFunctionCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful */
    SimRequestStatus                requestStatus;
    /** Verify or change the HIDDEN KEY */
    SimUiccHiddenKeyFunction        hiddenKeyFunction;
} AlsuHiddenKeyFunctionCnf;

/*-----------------27/06/2003 11:31-----------------
 * signal to read an ADN record
 */
typedef struct AlsuReadPbDialNumReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
    /** Whether to access global or local phonebook*/
    Boolean                         globalPhoneBook;
    /** Absolute, next or previous.
     * <TABLE>
     * <TR><TD><B>ReadMode Value</B></TD> <TD><B> Process </B></TD>
     * <TD><B>adnRecordNumber </B></TD></TR>
     * <TR><TD> Absolute   </TD> <TD><B>USIM manager will read the record
     * number specified by adnRecordNumber </B></TD>   <TD>Should be different from 0 </TD></TR>
     * <TR><TD> Previous or next  </TD>  <TD><B> USIM manager will access the previous or next record.
     * </B></TD>  <TD> Should be set to 0  </TD></TR>
     * </TABLE>
     */
    SimRecordReadWriteMode          readMode;
    /** ADN record number (Zero implies CURRENT) */
    Int16                           adnRecordNumber;
} AlsuReadPbDialNumReq;

/** Confirmation to the AlsuReadPbDialnumReq.
 * Returns the first ADN record that matches the given pattern.
 * Or if no search was requested, the USIM manager will return the
 * requested ADN record.
 */
typedef struct AlsuReadPbDialNumCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful.
     * If the request was successful, but the data is not valid (i.e. the requested
     * record is empty), the requestStatus field will be set to SIM_REQ_OK_DATA_INVALID.
     */
    SimRequestStatus                requestStatus;
    /** Whether it is global or local phonebook*/
    Boolean                         globalPhoneBook;
    /** ADN record number */
    Int16                           adnRecordNumber;
    /** Contains the decoded ADN entry, including the alpha ID,
     * and the CCP record number. A separate request has to be sent
     * to read the CCP (Bearer capability) associated with the ADN record.
     * If any extension1 data is associated with this ADN record,
     * it will be appended to the dialling number string (in SimDialNumber).
     */
    SimDialNumber                   dialNum;           /*ADN data*/
    /** Whether it is an hidden entry or not*/
    Boolean                         hiddenEntry;
} AlsuReadPbDialNumCnf;

/*-----------------27/06/2003 11:31-----------------*/
/** Signal to write to a specific ADN record.
 * This signal can be used to overwrite an existing phonebook entry, or to add a new one.
 */

typedef struct AlsuWritePbDialNumReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
     /** Global or local phonebook*/
    Boolean                         globalPhoneBook;
    /** Absolute, next or previous...*/
    SimRecordReadWriteMode          writeMode;
    /** ADN record number.
     * <TABLE>
     * <TR><TD><B>ReadMode Value</B></TD> <TD><B> adnRecordNumber </B></TD>
     * <TD><B> Process</B></TD></TR>
     * <TR><TD> Absolute   </TD> <TD><B>set to 0 </B></TD>   <TD>USIM manager
     * will add the phonebook entry to the first free location on the UICC. </TD></TR>
     * <TR><TD>  Absolute   </TD>  <TD><B> is superior or equal to 1
     * </B></TD>  <TD> USIM manager will overwrite the specified record. (Note: record
     * number 0 does not exist on the SIM/UICC). </TD></TR>
     * <TR><TD>  Next or Previous  </TD>  <TD><B> is  set to 0
     * </B></TD>  <TD> USIM manager will write to the previous or next location on the UICC. </TD></TR>
     * </TABLE>
    */
    Int16                           adnRecordNumber;
    /** The data to write  */
    SimDialNumber                   dialNum;
    /** Specifies whether some bearer capability data should be
     * written as well to the SIM in association with this AND record.
     
\assoc PRESENT \ref bearerCapability */
    Boolean                         bearerCapabilityDefined;
    /** Bearer capability information */
    BearerCapability                bearerCapability;
     /** Whether it is an hidden entry or not*/
    Boolean                         hiddenEntry;

}AlsuWritePbDialNumReq;

/** This signal is the confirmation to the AlsiWritePbDialnumReq */
typedef struct AlsuWritePbDialNumCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful */
    SimRequestStatus                requestStatus;
    /** Global or local phonebook*/
    Boolean                         globalPhoneBook;
    /** ADN record number */
    Int16                           adnRecordNumber;
    /** If the dialnumber write was successful, the ccValue will carry the value
     * of the Change Counter file if synchronisation is supported, if synchronisation
     * is not supported this value will be set to zero.
     */
    Int16                           ccValue;
    /** New Dial Number written*/
    SimDialNumber                   dialNum;
    /** Shows the dialling number that was overwritten.
     * Deleted dial number, this is needed by BL to update the hash-table
     */
    SimDialNumber                   prevDialNum;
}AlsuWritePbDialNumCnf;


/*-----------------27/06/2003 11:31-----------------*/
/** Signal to delete an existing ADN record.
 * This will delete the ADN record but also all
 * the data (email, sne, anr...) associated with it.
 * --------------------------------------------------*/

typedef struct AlsuDeletePbDialNumReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
     /** Global or local phonebook*/
    Boolean                         globalPhoneBook;
    /** Absolute, next or previous...*/
    SimRecordReadWriteMode          deleteMode;
    /** Specifies the record number to delete, and should be set superior or equal to 1.*/
    Int16                           adnRecordNumber;
} AlsuDeletePbDialNumReq;

/** Confirmation to the AlsuPbDeleteDialnumReq.  */
typedef struct AlsuDeletePbDialNumCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful */
    SimRequestStatus                requestStatus;
     /** Global or local phonebook*/
    Boolean                         globalPhoneBook;
    /** ADN record number */
    Int16                           adnRecordNumber;
    /** If the deletion was successful and synchronisation is supported,
    * the ccValue will carry the value of the Change Counter file,
    * if synchronisation is not supported this value will be set to zero.
    */
    Int16                           ccValue;
    /** Deleted dial number*/
    SimDialNumber                   dialNum;

} AlsuDeletePbDialNumCnf;

/*-----------------27/06/2003 11:46-----------------*/
/** Signal to read a CCP record.
 * Reads EF CCP file. This file stores the bearer capability
 * --------------------------------------------------*/
 typedef struct AlsuReadPbCcpReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
    /** Indicates whether the requesting task wants to access the local or the global phonebook*/
    Boolean                         globalPhoneBook;
    /** Indicates which CCP record is to be read (The allowed values are 1-254) */
    Int8                            ccpRecordNumber;
    /** ADN record number */
    Int16                           adnRecordNumber;
} AlsuReadPbCcpReq;

 /** Confirmation to the AlsuReadPbCcpReq.
 * Returns the bearer capability stored in a CCP record and associated to an ADN record.
 */
typedef struct AlsuReadPbCcpCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful */
    SimRequestStatus                requestStatus;
    /** Global or local phonebook*/
    Boolean                         globalPhoneBook;
    /** Indicates which CCP record has been read */
    Int8                            ccpRecordNumber;
    /** ADN record number */
    Int16                           adnRecordNumber;
    /** Bearer capability info */
    BearerCapability                bearerCapability;

} AlsuReadPbCcpCnf;

/*-----------------19/06/2003 17:29-----------------------------------------*/
/** This signal finds the EMAIL address associated with the given ADN record.
* Reads EF EMAIL record associated with a particular ADN record.
* The adnRecordNumber may be more than 254 if more than one ADN file
* exists on the phonebook. More than one EMAIL address may be associated to one ADN record
*/
 typedef struct AlsuReadPbEmailReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
    /** Indicates whether the requesting task wants to access the local or the global phonebook*/
    Boolean                         globalPhoneBook;
    /** ADN record number */
    Int16                           adnRecordNumber;
    /** Indicate which email address is requested.
     * EMAIL file number (there can be several email addresses associated with each ADN record)
     * \note Value 0 is permitted, and refers to the first email address
     */
    Int8                            emailFileNumber;
} AlsuReadPbEmailReq;

 /** Confirmation to the AlsuReadPbEmailReq.
  * Returns the requested email address.
  */
typedef struct AlsuReadPbEmailCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful */
    SimRequestStatus                requestStatus;
    /** Global or local phonebook*/
    Boolean                         globalPhoneBook;
    /** ADN record number */
    Int16                           adnRecordNumber;
    /* EMAIL file number (there can be several email addresses associated with each ADN record) */
    /* Note: value 0 is permitted, and refers to the first email address*/
    Int8                            emailFileNumber;
    /** Email data */
    SimUiccEmailData                emailData;
} AlsuReadPbEmailCnf;

/*-----------------27/06/2003 11:50-----------------*/
/** This signal writes an email.
 * Writes EF EMAIL record associated with a particular ADN record.
 * The adnRecordNumber may be more than 254 if more than one ADN file exists on the phonebook.
 * More than one EMAIL address may be associated to one ADN record
 */
typedef struct AlsuWritePbEmailReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
    /** Indicates whether the requesting task wants to access the local or the global phonebook*/
    Boolean                         globalPhoneBook;
    /** ADN record number */
    Int16                           adnRecordNumber;
    /** Email address is associated with this ADN record*/
    SimUiccEmailData                simUiccEmailData;      /*EMAIL data*/
    /** Indicates which email address should be overwritten/added.
     * EMAIL file number (there can be several email addresses associated with each ADN record)
     *
     * \note Value 0 is permitted, and refers to the first email address
     */
    Int8                            emailFileNumber;
}AlsuWritePbEmailReq;

/** Confirmation to the AlsuWritePbEmailReq.
 * This signal is sent as a response to a AlsuWritePbEmailReq and returns the status of the request.
 */
typedef struct AlsuWritePbEmailCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful
    * (If the email address could not be written because there was
    * no free EMAIL record on the SIM, requestStatus is set to SIM_REQ_NO_FREE_RECORD)
    */
    SimRequestStatus                requestStatus;
    /** Local or global phonebook*/
    Boolean                         globalPhoneBook;
    /** Email address is associated with this ADN record*/
    Int16                           adnRecordNumber;
    /* EMAIL file number (there can be several email addresses associated with each ADN record) */
    /* Note: value 0 is permitted, and refers to the first email address*/
    Int8                            emailFileNumber;
    /** If the write of the email was successful, the ccValue will
    * carry the value of the Change Counter file if synchronisation is supported,
    * if synchronisation is not supported this value will be set to zero.
    */
    Int16                           ccValue;

}AlsuWritePbEmailCnf;

 /*--------------------------------------------------*/
 /** This signal deletes an email.
  * Deletes EF EMAIL record associated with a particular ADN record.
  * The adnRecordNumber may be more than 254 if more than one ADN file exists on the phonebook.
  * More than one EMAIL address may be associated to one ADN record
  */
typedef struct AlsuDeletePbEmailReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
    /** Indicates whether the requesting task wants to access the local or the global phonebook*/
    Boolean                         globalPhoneBook;
    Int16                           adnRecordNumber;       /**< email address is associated with this ADN record*/
    /** Indicates which email address should be overwritten/added.
     * EMAIL file number (there can be several email addresses
     * associated with each ADN record)
     * \note Value 0 is permitted, and refers to the first email address
     */
    Int8                            emailFileNumber;
}AlsuDeletePbEmailReq;

/** Confirmation to the AlsuDeletePbEmailReq.
 * This primitive is sent as a response to AlsuDeletePbEmailReq
 * and returns the status of the request
 */
typedef struct AlsuDeletePbEmailCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful */
    SimRequestStatus                requestStatus;
    /** Local or global phonebook*/
    Boolean                         globalPhoneBook;
    /** Email address is associated with this ADN record*/
    Int16                           adnRecordNumber;
    /** Indicates which email address should be overwritten/added.
     * EMAIL file number (there can be several email addresses
     * associated with each ADN record)
     * \note Value 0 is permitted, and refers to the first email address
     */
    Int8                            emailFileNumber;
    /** If the delete of the email was successful, the ccValue
     * will carry the value of the Change Counter file if synchronisation is supported.
     * If synchronisation is not supported this value will be set to zero.
     */
    Int16                           ccValue;

}AlsuDeletePbEmailCnf;


/*-----------------19/06/2003 17:29-------------------------------*/
/** This signal reads the SNE (second name entry) associated with
 * the given ADN record.
 */
 typedef struct AlsuReadPbSneReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
    /** Indicates whether the requesting task wants to access the local or the global phonebook*/
    Boolean                         globalPhoneBook;
    /** may be more than 254 if more than one ADN file exists on the phonebook.
     * Only one SNE (second name entry) may be associated to one ADN record.
     */
    Int16                           adnRecordNumber;
} AlsuReadPbSneReq;

 /** Confirmation to the AlsuReadPbSneReq.  Returns the requested
  * Second Name Entry (SNE) from EF SNE.  It might be that no Second Name
  * Entry has been associated with the ADN record.  In that case, requestStatus will be set to
  * SIM_REQ_OK_DATA_INVALID, and the data in the sne field will be invalid)
  */
typedef struct AlsuReadPbSneCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful */
    SimRequestStatus                requestStatus;
    /** Local or global phonebook*/
    Boolean                         globalPhoneBook;
    /** ADN record number */
    Int16                           adnRecordNumber;
    /** Second Name Entry String if it exists*/
    SimAlphaIdentifier              sne;

} AlsuReadPbSneCnf;

/*-----------------27/06/2003 11:50-----------------*/
/** This signal writes a SNE.
 * Writes EF SNE (second name entry) record associated with a particular ADN record.
 */
typedef struct AlsuWritePbSneReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
    /** Indicates whether the requesting task wants to access the local or the global phonebook*/
    Boolean                         globalPhoneBook;
    /** SNE is associated with this ADN record*/
    Int16                           adnRecordNumber;
    /** SNE data to write*/
    SimAlphaIdentifier              sne;
}AlsuWritePbSneReq;

/** Confirmation to the AlsuWritePbSneReq.
 * Returns the status of the request.
 */
typedef struct AlsuWritePbSneCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful
     * (If the SNE  could not be written because there was no free SNE
     * record on the SIM, requestStatus is set to SIM_REQ_NO_FREE_RECORD).
     */
    SimRequestStatus                requestStatus;
    /** Local or global phonebook*/
    Boolean                         globalPhoneBook;
    /** Second Name Entry is associated with this ADN record */
    Int16                           adnRecordNumber;
    /** If the write of SNE was successful, the ccValue
     * will carry the value of the Change Counter file if synchronisation is supported.
     * If synchronisation is not supported this value will be set to zero.
     */
    Int16                           ccValue;
}AlsuWritePbSneCnf;

/*-----------------27/06/2003 11:50-----------------*/
/** This signal deletes a SNE.
 * Deletes EF SNE (second name entry) record associated with
 * a particular ADN record.
 */
typedef struct AlsuDeletePbSneReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
    /** Indicates whether the requesting task wants to access the local or the global phonebook*/
    Boolean                         globalPhoneBook;
    /** SNE is associated with this ADN record
     * (May be more than 254 if more than one ADN file exists on the phonebook,
     * only one SNE may be associated to one ADN record
     */
    Int16                           adnRecordNumber;
}AlsuDeletePbSneReq;

/** Confirmation to the AlsuDeletePbSneReq.
 * This signal is sent asa response to a AlsuDeletePbSneReq and
 * returns the status
 */
typedef struct AlsuDeletePbSneCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful */
    SimRequestStatus                requestStatus;
    /** Indicates whether the requesting task wants to access the local or the global phonebook*/
    Boolean                         globalPhoneBook;
    /** Email address is associated with this ADN record*/
    Int16                           adnRecordNumber;
    /** If the delete of SNE was successful, the ccValue
     * will carry the value of the Change Counter file if synchronisation is supported.
     * If synchronisation is not supported this value will be set to zero.
     */
    Int16                           ccValue;
}AlsuDeletePbSneCnf;


/*-----------------19/06/2003 17:29--------------------------*/
/** This signal finds the ANR address associated with the given ADN record.
 * Reads EF ANR record associated with a particular ADN record.
 * The adnRecordNumber may be more than 254 if more than one ADN file exists on the phonebook.
 * More than one ANR (additional number) may be associated to one ADN record.
 */
 typedef struct AlsuReadPbAnrReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
    /** Indicates whether the requesting task wants to access the local or the global phonebook*/
    Boolean                         globalPhoneBook;
    /** ANR is associated with this ADN record */
    Int16                           adnRecordNumber;
    /** Indicate which additional number is requested.
     * ANR file number : there can be several associated with each ADN record.
     * \note Value 0 is permitted, and refers to the first anr address
     */
    Int8                            anrFileNumber;
} AlsuReadPbAnrReq;

 /** Confirmation to the AlsuReadPbAnrCnf.
  * This primitive is sent as a response to a AlsuReadPbAnrReq and
  * it returns the requested additional number from EF ANR.
  */
typedef struct AlsuReadPbAnrCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful
     * (It might be that no additional number has been associated
     * with the ADN record. In that case, requestStatus will
     * be set to SIM_REQ_OK_DATA_INVALID,
     * and the anr field and other fields will be invalid)
     */
    SimRequestStatus                requestStatus;
    /** Local or global phonebook */
    Boolean                         globalPhoneBook;
    /** ANR is associated with this ADN record */
    Int16                           adnRecordNumber;
    /* ANR file number : there can be several associated with
     * each ADN record  (Note: value 0 is permitted, and refers to the first
     * ANR address)
     */
    Int8                            anrFileNumber;
    /** Record number in EF AAS*/
    Int8                            aasRecordNumber;
    /** AAS associated with this additional number*/
    SimAlphaIdentifier              aas;
    /** ANR data */
    SimUiccAnr                      anr;
    /** The bearer capability record number */
    Int8                            bearerCapRecordNum;

} AlsuReadPbAnrCnf;


/*-----------------27/06/2003 11:53---------------------------------------*/
/** Signal to write an Additional number associated with a specific
 * ADN record.
 */
typedef struct AlsuWritePbAnrReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
    /** Indicates whether the requesting task wants to access the local or the global phonebook*/
    Boolean                         globalPhoneBook;
    /** ANR is associated with this ADN record */
    Int16                           adnRecordNumber;
    /** Indicates which ANR address should be overwritten/added.
     * ANR file number : there can be several associated with each ADN record
     * \note Value 0 is permitted,  and refers to the first ANR address
     */
    Int8                            anrFileNumber;
    /** AAS associated with this additional number*/
    Int8                            aasRecordNumber;
    /** ANR data that we want to write */
    SimUiccAnr                      anr;
    /** New bearer capability*/
    BearerCapability                bearerCapability;
    /** Indicates whether the new bearer capability is present
\assoc PRESENT \ref bearerCapability */
    Boolean                         bearerCapabilityDefined;

 }AlsuWritePbAnrReq;

/** Confirmation to the AlsuWritePbAnrReq.
 * This primitive is sent as a response to AlsuWritePbAnrReq.
 *
 * If a bearer capability object was defined, and could not be stored, the requestStatus will be
 * set to SIM_REQ_NO_FREE_CCP_REC; And as a consequence, the ANR will not be updated.
 *
 * If the dialling number provided was more than 20 digits, and an extension
 * record could not be found, the requestStatus would be set to
 * SIM_REQ_NO_FREE_CCP_REC; And as a consequence, the ANR will not be updated.
 */
typedef struct AlsuWritePbAnrCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful */
    SimRequestStatus                requestStatus;
    /** Local or global phonebook */
    Boolean                         globalPhoneBook;
    /** ADN record number */
    Int16                           adnRecordNumber;
    /** ANR file number : there can be several associated with each ADN record
     * ( value 0 is permitted, and refers to the first anr address )
     */
    Int8                            anrFileNumber;
    /** If the write of the anr was successful, the ccValue
     * will carry the value of the Change Counter file if synchronisation is supported.
     * If synchronisation is not supported this value will be set to zero.
     */
    Int16                           ccValue;
}AlsuWritePbAnrCnf;

 /*-----------------07/11/2003 -----------------------------*/
/** Signal to delete an Additional number associated with a specific
 * ADN record.
 * Deletes an ANR record associated with a particular ADN record.
 * The adnRecordNumber may be more than 254 if more than one ADN file exists on the phonebook.
 * More than one ANR (additional number) may be associated to one ADN record
 */
 typedef struct AlsuDeletePbAnrReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
    /** Indicates whether the requesting task wants to access the local or the global phonebook*/
    Boolean                         globalPhoneBook;
    /** ANR is associated with this ADN record */
    Int16                           adnRecordNumber;
    /** Indicates which additional number is requested
     * (value 0 is permitted, and refers to the first anr address)
     */
    Int8                            anrFileNumber;
}AlsuDeletePbAnrReq;

 /** Confirmation to the AlsuDeletePbAnrReq.*/
typedef struct AlsuDeletePbAnrCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful */
    SimRequestStatus                requestStatus;
    /** Local or global phonebook */
    Boolean                         globalPhoneBook;
    /** ANR is associated with this ADN record */
    Int16                           adnRecordNumber;
    /** Indicates which additional number is requested
     * (value 0 is permitted, and refers to the first anr address)
     */
    Int8                            anrFileNumber;
    /** If the request was successful, the ccValue
     * will carry the value of the Change Counter file if synchronisation is supported.
     * If synchronisation is not supported this value will be set to zero.
     */
    Int16                           ccValue;
}AlsuDeletePbAnrCnf;

/*-----------------27/06/2003 11:55-----------------*/
/** Reads up to SIM_MAX_NUM_PBC_RECORD_LIST records from EF PBC (phonebook control).
 * This signal provides a list of PBC records.
 * This file describes whether:
 * 1) the entry has been modified by a GSM terminal
 * 2) the entry is hidden
 * --------------------------------------------------*/
typedef struct AlsuListPbPbcReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
    /** Indicates whether the requesting task wants to access the local or the global phonebook*/
    Boolean                         globalPhoneBook;
    /** Indicates the record number to start from (first pbcRecord to be read).
     * The number of PBC files in the phonebook should be the same as the number of ADN files.
     * If there are more than one PBC file,
     * pbcRecordNumber will exceed 254, which is why pbcRecordNumber is an Int16.
     */
    Int16                           pbcRecordNumber;
 } AlsuListPbPbcReq;

/** Confirmation to the AlsuListPbPbcReq signals. Lists up to
 * SIM_MAX_NUM_PBC_RECORD_LIST records from EF PBC (phonebook control).
 *
 * \note The type of linking for EF PBC is always type 1,
 * which means that PBC record number Y matches ADN record Y.
 */
typedef struct AlsuListPbPbcCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful */
    SimRequestStatus                requestStatus;
    /** Local or global phonebook */
    Boolean                         globalPhoneBook;
    /** First record number to access */
    Int16                           pbcRecordNumber;
    /** Total number of entries in EF PBC (this includes
     * all the records from all the PBC files present on the phonebook.
     */
    Int16                           totalPbcEntries;
    /** Indicates how many PBC entries are listed in the array pbcRecord. */
    Int8                            numPbcEntries;
    /** Contains records from EF PBC */
    SimUiccPbcRecord                pbcRecord[SIM_MAX_NUM_PBC_RECORD_LIST];

} AlsuListPbPbcCnf;

/*-----------------27/06/2003 11:55-----------------*/
/** Signal to read a group alpha string.
 * Reads a record from EF GAS. This file lists the different
 * group names available in the phonebook.
 */
typedef struct AlsuReadPbGasReqTag
{
      /** Command reference which is mirrored in the confirmation signal */
      Int16                           commandRef;
      /** Indicates whether the requesting task wants to access the local or the global phonebook*/
      Boolean                         globalPhoneBook;
      /** Record to read in EF GAS*/
      Int8                            gasRecordNumber;
} AlsuReadPbGasReq;

/** Confirmation to the AlsuReadPbGasReq.
 * This signal is sent as a response to AlsuGetPbidReq and it
 * returns the requested GAS record
 */
typedef struct AlsuReadPbGasCnfTag
{
      /** Copy of the command reference given in the corresponding request
      * (This allows the application layer to uniquely identify the associated confirmation).
      */
      Int16                           commandRef;
      /** Indication of whether or not the request was successful */
      SimRequestStatus                requestStatus;
      /** Local or global phonebook */
      Boolean                         globalPhoneBook;
      /** GAS record that has been read .*/
      Int8                            gasRecordNumber;
      /** Requested GAS record */
      SimAlphaIdentifier              gas;
}  AlsuReadPbGasCnf;

/*-----------------27/06/2003 11:55-----------------*/
/** Signal to list some valid group alpha strings.
 * --------------------------------------------------*/
typedef struct AlsuListPbGasReqTag
{
      /** Command reference which is mirrored in the confirmation signal */
      Int16                           commandRef;
      /** Indicates whether the requesting task wants to access the local or the global phonebook*/
      Boolean                         globalPhoneBook;
      /** The first record number in the GAS file to read in EF GAS. */
      Int8                            startRecord;
} AlsuListPbGasReq;

/** Confirmation to the AlsuListPbGasReq.
 * The signal returns a list of valid GAS records and their record number.
 */
typedef struct AlsuListPbGasCnfTag
{
      /** Copy of the command reference given in the corresponding request
       * (This allows the application layer to uniquely identify the associated confirmation).
       */
      Int16                             commandRef;
      /** Indication of whether or not the request was successful */
      SimRequestStatus                  requestStatus;
      /** Local or global phonebook */
      Boolean                           globalPhoneBook;
      /** The first record number in the GAS file to read in EF GAS. */
      Int8                              startRecord;
      /** indicates whether there are more valid entries to request,
       * if so, another AlsuListPbGasReq can be sent to request the other valid entries
       */
      Boolean                           moreValidEntries;
      /** Indicates the size of the gasList list */
      Int8                              listSize;
      /** Lists the record numbers */
      Int8                              gasRecordNum[SIM_MAX_GAS_LIST];
      /** Holds a list of valid GAS */
      SimAlphaIdentifier                gasList [SIM_MAX_GAS_LIST];
}  AlsuListPbGasCnf;



/*-----------------27/06/2003 11:55-----------------*/
/** Signal to write a group alpha string.
 * Writes a new group name to EF GAS
 */
typedef struct AlsuWritePbGasReqTag
{
      /** Command reference which is mirrored in the confirmation signal */
      Int16                           commandRef;
      /** Indicates whether the requesting task wants to access the local or the global phonebook*/
      Boolean                         globalPhoneBook;
      /** Record to write in EF GAS.
       * Is in the range of 1 to 254 (record number 0 and record 255 do not exist on the SIM),
       * the specified GAS record number will be overwritten with the new value
       * If gasRecordNumber is set to 0, then the USIM manager will try to find an empty
       * record to add the new GAS.*/
      Int8                            gasRecordNumber;
      /** GAS data */
      SimAlphaIdentifier              gas;

} AlsuWritePbGasReq;

/** Confirmation to the AlsuWritePbGasReq */
typedef struct AlsuWritePbGasCnfTag
{
      /** Copy of the command reference given in the corresponding request
       * (This allows the application layer to uniquely identify the associated confirmation).
       */
      Int16                           commandRef;
      /** Indication of whether or not the request was successful */
      SimRequestStatus                requestStatus;
      /** Local or global phonebook */
      Boolean                         globalPhoneBook;
      /** specifies which record number was overwritten
       * (Only valid if requestStatus = SIM_REQ_OK).
       */
      Int8                            gasRecordNumber;
      /** If the request was successful, the ccValue
       * will carry the value of the Change Counter file if synchronisation is supported.
       * If synchronisation is not supported this value will be set to zero.
       */
      Int16                           ccValue;
}  AlsuWritePbGasCnf;

/*------------------------------------------------------*/
/** Signal to delete a group alpha string.
 * Deletes a group name in EF GAS.
 */
 typedef struct AlsuDeletePbGasReqTag
 {
      /** Command reference which is mirrored in the confirmation signal */
      Int16                              commandRef;
      /** Indicates whether the requesting task wants to access the local or the global phonebook*/
      Boolean                            globalPhoneBook;
      /** Specifies which recordNumber should be deleted.
       * The gasRecordNumber values must be in the range of 1 to 254.
       * (Record number 0 and record 255 do not exist on the SIM).
       */
      Int8                               gasRecordNumber;
  } AlsuDeletePbGasReq;

 /** Confirmation to the AlsuDeletePbGasCnf.*/
  typedef struct AlsuDeletePbGasCnfTag
  {
      /** Copy of the command reference given in the corresponding request
       * (This allows the application layer to uniquely identify the associated confirmation).
       */
      Int16                              commandRef;
      /** Indication of whether or not the request was successful */
      SimRequestStatus                   requestStatus;
      /** Local or global phonebook */
      Boolean                            globalPhoneBook;
      /** Specifies which record number was deleted (only valid if requestStatus = SIM_REQ_OK).*/
      Int8                               gasRecordNumber;
      /** If the request was successful, the ccValue
       * will carry the value of the Change Counter file if synchronisation is supported.
       * If synchronisation is not supported this value will be set to zero.
       */
      Int16                              ccValue;
  } AlsuDeletePbGasCnf;

/*------------------------------------------------------*/
/** Signal to delete all the entries from group alpha string.
 * Deletes all group name in EF GAS.
 */
 typedef struct AlsuClearPbGasReqTag
 {
      /** Command reference which is mirrored in the confirmation signal */
      Int16                              commandRef;
      /** Indicates whether the requesting task wants to access the local or the global phonebook*/
      Boolean                            globalPhoneBook;
  } AlsuClearPbGasReq;

 /** Confirmation to the AlsuClearPbGasCnf.*/
  typedef struct AlsuClearPbGasCnfTag
  {
      /** Copy of the command reference given in the corresponding request
       * (This allows the application layer to uniquely identify the associated confirmation).
       */
      Int16                              commandRef;
      /** Indication of whether or not the request was successful */
      SimRequestStatus                   requestStatus;
      /** Local or global phonebook */
      Boolean                            globalPhoneBook;
      /** If the request was successful, the ccValue
       * will carry the value of the Change Counter file if synchronisation is supported.
       * If synchronisation is not supported this value will be set to zero.
       */
      Int16                              ccValue;
  } AlsuClearPbGasCnf;

/*--------------------------------------------------------------------*/
/** Signal to read an additional number alpha string.
 * Reads an AAS record. The additional alpha strings are strings
 * that can be associated with an Additional number.
 * An AAS record may be shared by several ANR records.
 */
  typedef struct AlsuReadPbAasReqTag
  {
      /** Command reference which is mirrored in the confirmation signal */
      Int16                         commandRef;
      /** Indicates whether the requesting task wants to access the local or the global phonebook*/
      Boolean                       globalPhoneBook;
      /** Record to read in EF AAS*/
      Int8                          aasRecordNumber;
  }AlsuReadPbAasReq;

  /** Confirmation to the AlsuReadPbAasReq. */
  typedef struct AlsuReadPbAasCnfTag
  {
     /** Copy of the command reference given in the corresponding request
      * (This allows the application layer to uniquely identify the associated confirmation).
      */
      Int16                           commandRef;
      /** Indication of whether or not the request was successful */
      SimRequestStatus                requestStatus;
      /** Local or global phone */
      Boolean                         globalPhoneBook;
      /** Record to read in EF AAS*/
      Int8                            aasRecordNumber;
      /** AAS that is read */
      SimAlphaIdentifier              aas;
   }AlsuReadPbAasCnf;

 /*-----------------27/06/2003 11:55-----------------------*/
/** Lists some valid AAS records.
 * Signal to list some valid additional alpha strings
 * AAS are the strings associated with the Additional Numbers
 */
 typedef struct AlsuListPbAasReqTag
{
      /** Command reference which is mirrored in the confirmation signal */
      Int16                           commandRef;
      /** Indicates whether the requesting task wants to access the local or the global phonebook*/
      Boolean                         globalPhoneBook;
      /** The first record number in AAS to read in EF GAS. */
      Int8                            startRecord;
} AlsuListPbAasReq;

/** Confirmation to the AlsuListPbAasReq. The signal returns a list of valid AAS */
typedef struct AlsuListPbAasCnfTag
{
      /** Copy of the command reference given in the corresponding request
       * (This allows the application layer to uniquely identify the associated confirmation).
       */
      Int16                             commandRef;
      /** Indication of whether or not the request was successful */
      SimRequestStatus                  requestStatus;
      /** Local or global phonebook */
      Boolean                           globalPhoneBook;
      /** The first record number in AAS to read in EF GAS. */
      Int8                              startRecord;
      /** indicates whether there is more valid entries to be requested,
       * if so, another AlsuListPbGasReq can be sent to request the other valid entries
       */
      Boolean                           moreValidEntries;
      /** Indicates the size of the aasList list.*/
      Int8                              listSize;
      /** Valid AAS record numbers */
      Int8                              aasRecordNum[SIM_MAX_AAS_LIST];
      /** Contains the list of valid AAS */
      SimAlphaIdentifier                aasList [SIM_MAX_AAS_LIST];
}  AlsuListPbAasCnf;

 /*-----------------------------------------------------------*/
/** Signal to write an additional number alpha string.
 * This signal can update a given AAS record (Additional Number Alpha String)
 * in the given phonebook.
 */
 typedef struct AlsuWritePbAasReqTag
 {
      /** Command reference which is mirrored in the confirmation signal */
      Int16                              commandRef;
      /** Indicates whether the requesting task wants to access the local or the global phonebook*/
      Boolean                            globalPhoneBook;
      /** Record to write in EF AAS*/
      Int8                               aasRecordNumber;
      /** AAS to be written */
      SimAlphaIdentifier                 aas;
  } AlsuWritePbAasReq;

 /** Confirmation to the AlsuWritePbAasReq */
  typedef struct AlsuWritePbAasCnfTag
  {
      /** Copy of the command reference given in the corresponding request
       * (This allows the application layer to uniquely identify the associated confirmation).
       */
      Int16                              commandRef;
      /** Indication of whether or not the request was successful */
      SimRequestStatus                   requestStatus;
      /** Locol or global phonebook */
      Boolean                            globalPhoneBook;
      /** AAS record number */
      Int8                               aasRecordNumber;
     /** If the request was successful, the ccValue
      * will carry the value of the Change Counter file if synchronisation is supported.
      * If synchronisation is not supported this value will be set to zero.
      */
      Int16                              ccValue;
  } AlsuWritePbAasCnf;


/** Signal to delete an additional number alpha string.
 * This signal can delete a given AAS record (Additional number Alpha String)
 * in the given phonebook.
 */
 typedef struct AlsuDeletePbAasReqTag
 {
      /** Command reference which is mirrored in the confirmation signal */
      Int16                              commandRef;
      /** Indicates whether the requesting task wants to access the local or the global phonebook*/
      Boolean                            globalPhoneBook;
      /** Record number of AAS to be deleted */
      Int8                               aasRecordNumber;
  } AlsuDeletePbAasReq;

 /** Confirmation to the AlsuDeletePbAasReq.*/
  typedef struct AlsuDeletePbAasCnfTag
  {
      /** Copy of the command reference given in the corresponding request
       * (This allows the application layer to uniquely identify the associated confirmation).
       */
      Int16                              commandRef;
      /** Indication of whether or not the request was successful */
      SimRequestStatus                   requestStatus;
      /** Locol or global phonebook */
      Boolean                            globalPhoneBook;
      /** The record number of AAS that has been deleted */
      Int8                               aasRecordNumber;
      /** If the request was successful, the ccValue
       * will carry the value of the Change Counter file if synchronisation is supported.
       * If synchronisation is not supported this value will be set to zero.
       */
      Int16                              ccValue;
  } AlsuDeletePbAasCnf;

/*------------------------------------------------------*/
/** Signal to delete all the entries from additional number alpha string.
*   Deletes all additional number alpha in EF AAS.
*/
 typedef struct AlsuClearPbAasReqTag
 {
      /** Command reference which is mirrored in the confirmation signal */
      Int16                              commandRef;
      /** Indicates whether the requesting task wants to access the local or the global phonebook*/
      Boolean                            globalPhoneBook;
  } AlsuClearPbAasReq;

 /** Confirmation to the AlsuClearPbAasReq.*/
  typedef struct AlsuClearPbAasCnfTag
  {
      /** Copy of the command reference given in the corresponding request
       * (This allows the application layer to uniquely identify the associated confirmation).
       */
      Int16                              commandRef;
      /** Indication of whether or not the request was successful */
      SimRequestStatus                   requestStatus;
      /** Local or global phonebook */
      Boolean                            globalPhoneBook;
      /** If the request was successful, the ccValue
       * will carry the value of the Change Counter file if synchronisation is supported.
       * If synchronisation is not supported this value will be set to zero.
       */
      Int16                              ccValue;
  } AlsuClearPbAasCnf;

/*  This indication signal is sent by the USIM Manager when at least one ANR
*   has been changed for a given ADN record
*/
typedef struct AlsuPbAnrChangedIndTag
{
    /** Local or global phonebook */
    Boolean                         globalPhoneBook;
    /** The ADN record number for which the ANR has been modified*/
    Int16                           adnRecordNumber;
    /** The modified file number*/
    Int8                            anrFileNumber;
}  AlsuPbAnrChangedInd;

typedef EmptySignal AlsuPbExtEmailReq;

typedef struct AlsuPbExtEmailCnfTag
{
    SimUiccPbExtEmailnfo            smPhoneBook;
    SimUiccPbExtEmailnfo            apPhoneBook;    
}  AlsuPbExtEmailCnf;

/*-----------------27/06/2003 11:55-------------------------*/
/** Reads the GRP (grouping information) information associated to a specific ADN record
 * Each ADN entry can belong to several groups.
 * This signal reports the list of groups associated with a specific ADN entry
 */
typedef struct AlsuReadPbGrpReqTag
{
      /** Command reference which is mirrored in the confirmation signal */
      Int16                           commandRef;
      /** Indicates whether the requesting task wants to access the local or the global phonebook*/
      Boolean                         globalPhoneBook;
      /** The ADN record number*/
      Int16                           adnRecordNumber;

} AlsuReadPbGrpReq;

/** This signal is sent as a response to a AlsuReadPbGrpReq.
 * The confirmation signal returns the list of groups (grpData)
 * associated with an ADN record.
 * The maximum number of group is 10, and it might be even less depending
 * upon the UICC
 */
typedef struct AlsuReadPbGrpCnfTag
{
      /** Copy of the command reference given in the corresponding request
       * (This allows the application layer to uniquely identify the associated confirmation).
       */
      Int16                           commandRef;
      /** Indication of whether or not the request was successful */
      SimRequestStatus                requestStatus;
      /** Local or global phonebook */
      Boolean                         globalPhoneBook;
      /** Requested GRP data */
      SimUiccGrpData                  grpData;
      /** The ADN record number*/
      Int16                           adnRecordNumber;

}  AlsuReadPbGrpCnf;

/*-----------------27/06/2003 11:55---------------------------*/
/** This signal associates some Grouping Information to the given ADN record.
 * Writes the GRP (grouping information) information associated
 * to a specific ADN record.
 * Each ADN entry can belong to several groups.
 * This signal overwrites the list of groups associated with a
 * specific ADN entry
 */
typedef struct AlsuWritePbGrpReqTag
{
      /** Command reference which is mirrored in the confirmation signal */
      Int16                           commandRef;
      /** Indicates whether the requesting task wants to access the local or the global phonebook. */
      Boolean                         globalPhoneBook;
      /** The ADN record number.
       * Record numbers on the SIM start from 1. Hence, adnRecordNumber should not be set to 0.
       * The adnRecordNumber value may be superior to 254 if more than one ADN file exists on
       * the phonebook.
       *
       * Each ADN record may be associated with up to 10 groups.
       * (SIM_MAX_NUM_GRP is set to 10, as specified by the specification TS 131 102).
       *
       * \note Even if only one entry in the group list is modified, the whole list
       * associated with one ADN record has to be provided.  Example:
       *
       * If ADN record number x is part of group A, B and C, and the user
       * wants ADN record number x to be part of group D as well, then
       * grpData.numOfGrp should be set to 4, and <UL>
       * <LI> grpData.grpList[0] = group A ID
       * <LI> grpData.grpList [1] = group B ID
       * <LI> grpData.grpList [2] = group C ID
       * <LI> grpData.grpList [3] =  group D ID
       */
      Int16                           adnRecordNumber;
      /** GRP to be written */
      SimUiccGrpData                  grpData;
} AlsuWritePbGrpReq;

/** Confirmation to the AlsuWritePbGrpCnf.*/
typedef struct AlsuWritePbGrpCnfTag
{
      /** Copy of the command reference given in the corresponding request
       * (This allows the application layer to uniquely identify the associated confirmation).
       */
      Int16                           commandRef;
      /** Indication of whether or not the request was successful */
      SimRequestStatus                requestStatus;
      /** Local or global phonebook */
      Boolean                         globalPhoneBook;
      /** The ADN record number*/
      Int16                           adnRecordNumber;
      /** If the request was successful, the ccValue
       * will carry the value of the Change Counter file if synchronisation is supported.
       * If synchronisation is not supported this value will be set to zero.
       */
      Int16                           ccValue;
}  AlsuWritePbGrpCnf;


/** This signal deletes the GRP associated with the given ADN record.
 * Deletes the GRP (grouping information) information
 * associated to a specific ADN record.
 */
 typedef struct AlsuDeletePbGrpReqTag
 {
      /** Command reference which is mirrored in the confirmation signal */
      Int16                              commandRef;
      /** Indicates whether the requesting task wants to access the local or the global phonebook*/
      Boolean                            globalPhoneBook;
      /** ADN record number to be deleted */
      Int16                              adnRecordNumber;
 } AlsuDeletePbGrpReq;

 /** Confirmation to the AlsuDeletePbGrpCnf.*/
 typedef struct AlsuDeletePbGrpCnfTag
 {
      /** Copy of the command reference given in the corresponding request
       * (This allows the application layer to uniquely identify the associated confirmation).
       */
      Int16                              commandRef;
      /** Indication of whether or not the request was successful */
      SimRequestStatus                   requestStatus;
      /** Local or global phonebook */
      Boolean                            globalPhoneBook;
      /** ADN record number deleted */
      Int16                              adnRecordNumber;
      /** If the request was successful, the ccValue
       * will carry the value of the Change Counter file if synchronisation is supported.
       * If synchronisation is not supported this value will be set to zero.
       */
      Int16                              ccValue;
 } AlsuDeletePbGrpCnf;

 /** Request to list the valid AND records and their record number. */
typedef struct AlsuListPbDialNumExtReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
    /** Indicates whether the requesting task wants to access the local or the global phonebook*/
    Boolean                         globalPhoneBook;
    /** The first record number. */
    Int16                           startRecord;
}
AlsuListPbDialNumExtReq;

/** Confirmation to the AlsuListPbDialnumExtReq */
typedef struct AlsuListPbDialNumExtCnfTag
{
    /** Copy of the command reference given in the corresponding request
     * (This allows the application layer to uniquely identify the associated confirmation).
     */
    Int16                          commandRef;
    /** Indication of whether or not the request was successful */
    SimRequestStatus               requestStatus;
    /** Local or global phonebook */
    Boolean                        globalPhoneBook;
    /** Indicates the length of the list */
    Int8                           listSize;
    /** The record numbers of the requested list */
    Int16                          recordNumber[SIM_ALPHA_LIST_SIZE];
    /** Requested list */
    SimDialNumberWithoutSubAddr    dialNums[SIM_ALPHA_LIST_SIZE];
    /** Set to TRUE when a given entry is hidden */
    Boolean                        isHidden[SIM_ALPHA_LIST_SIZE];
}
AlsuListPbDialNumExtCnf;

/** This signal is sent to request the UID assigned to a specific adn entry.
 * The EFUID is used to uniquely identify a record to be able to
 * keep track of the entry in the phone book.  There is a direct
 * one to one mapping with entries in the ADN file. ADNs may be
 * deleted but the Unique Ids assigned to them are not re-used until
 * the PBID is regenerated.
 */
 typedef struct AlsuReadPbUidReqTag
 {
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
    /** Indicates whether the requesting task wants to access the local or the global phonebook*/
    Boolean                         globalPhoneBook;
    /** ADN record number */
    Int16                           adnRecordNumber;
 }
 AlsuReadPbUidReq;

 /** This confirmation signal returns the Unique Id associated with an ADN record.  */
 typedef struct AlsuReadPbUidCnfTag
 {
      /** Copy of the command reference given in the corresponding request
       * (This allows the application layer to uniquely identify the associated confirmation).
       */
      Int16                              commandRef;
      /** Indication of whether or not the request was successful */
      SimRequestStatus                   requestStatus;
      /** Local or global phonebook */
      Boolean                            globalPhoneBook;
      /** ADN record number */
      Int16                              adnRecordNumber;
      /** Unique Id associated with an ADN record */
      Int16                              uniqueId;
 }
 AlsuReadPbUidCnf;

/*-----------------18/12/2008 10:42----------------------------------------------------
 * New signals to read the synchronization information (CC, PBID and PaUID)
 * These info allows the ME to determine whether there was ny change in the phonebook
 * ------------------------------------------------------------------------------------*/

/** This signal is sent to request the synchronisation information on a
 *  specified phonebook. These information are only available on 3G USIM
 *  with synchronisation file.
 */
typedef struct AlsuReadPbSyncInfoReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                           commandRef;
    /** Local or global phonebook */
    Boolean                         globalPhoneBook;
} AlsuReadPbSyncInfoReq;

/** This confirmation signal returns the synchronisation info associated with a phonebook  */
typedef struct AlsuReadPbSyncInfoCnfTag
{
    /** Copy of the command reference given in the corresponding request
    * (This allows the application layer to uniquely identify the associated confirmation).
    */
    Int16                           commandRef;
    /** Indication of whether or not the request was successful */
    SimRequestStatus                requestStatus;
    /** Local or global phonebook */
    Boolean                         globalPhoneBook;
    /** Every update/deletion of an existing phone book entry or the addition
     *  of a new phone book entry causes the terminal to increment the EF_CC.
     */
    Int16                           changeCounter;
    /** The phone book synchronisation counter (PSC) is used to
     * construct the phone book identifier (PBID). This PSC is used
     * for synchronisation purposes.  When either the Change Counter
     * or Unique Id reach maximum value it is reset, and the PSC is
     * incremented, hence the PBID is changed.
     */
    SimUiccPbid                     pbid;
    /** The previous record unique ID */
    Int16                           puid;
}  AlsuReadPbSyncInfoCnf;

/* This indication signal is sent by the USIM Manager on when
 * synchronisation information of a phonebook has changed.
 */
typedef struct AlsuPbSyncInfoChangeIndTag
{
    /** Local or global phonebook */
    Boolean                         globalPhoneBook;
    /** Every update/deletion of an existing phone book entry or the addition
    *  of a new phone book entry causes the terminal to increment the EF_CC.
    */
    Int16                           changeCounter;
    /** True when the phonebook change counter has changed */
    Boolean                         changeCounterHasChanged;
    /** The phone book synchronisation counter (PSC) is used to
     * construct the phone book identifier (PBID). This PSC is used
     * for synchronisation purposes.  When either the Change Counter
     * or Unique Id reach maximum value it is reset, and the PSC is
     * incremented, hence the PBID is changed.
     */
    SimUiccPbid                     pbid;
    /** True when the phonebook ID has changed */
    Boolean                         pbidHasChanged;
    /** The previous record unique ID */
    Int16                           puid;
    /** True when the previous record unique ID has changed */
    Boolean                         puidHasChanged;
}  AlsuPbSyncInfoChangeInd;

/** @} */ /* End of SigAlsuSignals group */
/** @} */ /* End of SigAlsu group */
#endif
/* define  ALSU_SIG_H*/

/* END OF FILE */
