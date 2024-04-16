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
 * Application NV Record Manager.  API Signals.
 **************************************************************************/

#ifndef ANRM2SIG_H
#define ANRM2SIG_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/
#include <nvstatus_typ.h> /* for NvStatus: NVRAM_OK etc. */

/* DON'T include anrm2typ.h here.
 * Correct (af_nv.h or ab_nv.h should be included first.*/

/** \defgroup Anrm ANRM
 * \ingroup PrdDevices
 * \ingroup Sig
 * Signalling API into NV storage record manager module.
 *
 * This section defines the API between the 'user' code and the NVRAM storage
 * facility. 
 *
 * @{
 */

/***************************************************************************
 * Manifest Constants
 **************************************************************************/
/** Number of files whose information is returned in #anrm2FileStatusCnf */
#define ANRM2_NUM_FILES_IN_FILE_STATUS 5
/** Maximum number of file records returned in #Anrm2ListFileRecordsCnf */
#define ANRM2_MAX_FILE_LIST_SIZE       10
/** The maximum number of records that could be returned in #Anrm2ListFileRecordsCnf */
#define ANRM2_MAX_FILE_DATA_LIST_SIZE  2048

/***************************************************************************
 * Typed Constants
 **************************************************************************/
/* none */

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/** Specifies the type of the file - simple flat file or cyclic file with old data being lost */
typedef enum Anrm2FileTypeTag
{
  /** Flat */
  ANRM2_FLAT_FILE,
  /** Cyclic */
  ANRM2_CYCLIC_FILE
} Anrm2FileType;

/** Anrm2RwMode means different things depending on whether the file is flat or cyclic 
 * and whether the record is being read, written or deleted
 */
typedef enum Anrm2RwModeTag
{
 /**
  * <TABLE>
  * <TR><TD><B> RecordNo value </B></TD>    <TD><B> Comment              </B></TD></TR>
  * <TR><TD>     recordNo = 0          </TD>    <TD>read/write/deletes current record          </TD></TR>
  * <TR><TD>     recordNo > 0 AND  recordNo < number of records    </TD>    
  * <TD>read/write/delete specified record          </TD></TR>
  * <TR><TD>     recordNo > number of records         </TD>    <TD>NVRAM_REC_NOT_FOUND   </TD></TR>
  * </TABLE>
  */
  ANRM2_RWMODE_CURRENT_ABSOLUTE,

  /** If current record is not last record then read/write/delete next record; else NVRAM_REC_NOT_FOUND
   */
  ANRM2_RWMODE_PREVIOUS,

  /** If current record is not first record then read/write/delete previous record; else NVRAM_REC_NOT_FOUND
   */
  ANRM2_RWMODE_NEXT,

  /** Write record if first free record, if no free records then NVRAM_REC_NOT_FOUND
   * 
   * \note Not applicable to reading or deleting.
   */
  ANRM2_RWMODE_ADD
} Anrm2RwMode;

/** Provides file information */
typedef struct Arnm2FileInfoTag
{
  /** Data identifier */
  Anrm2DataName     fileName;
  /** Number of records in the file */
  Int16             numRecords;
  /** The size of Alpha ID; only used by dial number files */
  Int8              alphaIdSize;     
} Anrm2FileInfo;


/***************************************************************************
 * Type Definitions for Signals
 **************************************************************************/

/** Read Data request.
 * The Anrm2ReadDataReq signal should be sent to the ANRM2 task to read an item. 
 * The item will be read from NVRAM, or if the value is unchanged, from ROM.
 */
typedef struct Anrm2ReadDataReqTag
{
  /** Task ID of caller */
  TaskId            sourceTaskId;
  /** Identifier for this instance of a command/response;
   * generated by the requesting task
   */
  Int16             commandRef;
  /** The name of the data item to be read  */
  Anrm2DataName     name;
}
Anrm2ReadDataReq;

/** Response to Read Data request.
 * The requested data is returned, within the Anrm2DataUnion. 
 *
 * If the data is found in NVRAM, the union is filled and the status is set to NVRAM_OK. 
 *
 * If the data is not found in NVRAM, the default data is returned and the status is set to NVRAM_OK.
 *
 * If the data item exists in NVRAM, but it cannot be read, the default data is returned and the 
 * layer 1 error message is relayed in the status field.
 *
 * If the data item in NVRAM and the default data are both missing or corrupt, the dataIsValid 
 * field is set FALSE. In all other cases it is set TRUE.
 */
typedef struct Anrm2ReadDataCnfTag
{
  /** Identifier for this instance of a command/response
   * copied from #Anrm2ReadDataReq
   */
  Int16             commandRef;
  /** Data identifier 
\assoc UNION \ref data */
  Anrm2DataName     name;
  /** Status report (NVRAM_OK or Layer One error code) */
  NvStatus          status;
  /** TRUE if data (in next field) is valid; FALSE if no data found(From NVRAM or default) */
  Boolean           isDataValid;
  /** Read data payload */
  Anrm2DataUnion    data;
}
Anrm2ReadDataCnf;


/** Write Data request.
 * A task wishing to overwrite an existing item with new data, or create a new item, 
 * sends an Anrm2WriteDataReq to the ANRM2 task.*/
typedef struct Anrm2WriteDataReqTag
{
  /** Task ID of caller */
  TaskId            sourceTaskId;
  /** Identifier for this instance of a command/response;
   * generated by the requesting task
   */
  Int16             commandRef;
  /** The name of the data item to be written or created 
\assoc UNION \ref data */
  Anrm2DataName     name;
  /** The data filled union to write */
  Anrm2DataUnion    data;
}
Anrm2WriteDataReq;

/** Response to data write.
 * A confirm signal is sent to the requesting task. If there was a problem writing the 
 * data item, the error is returned within the <I>status</I> field and wasWrittenOk is set to FALSE.
 * Otherwise, the <I>WasWrittenOk</I> field is set <I>TRUE</I> and the status field is set to <I>NVRAM_OK</I>
 */
typedef struct Anrm2WriteDataCnfTag
{
  /** Identifier for this instance of a command/response;
   * copied from #Anrm2WriteDataReq
   */
  Int16             commandRef;
  /** Data identifier */
  Anrm2DataName     name;
  /** TRUE if data written without error; FALSE if error occurred */
  Boolean           wasWrittenOk;
  /** Status report (NVRAM_OK or Layer One error code) */
  NvStatus          status;
}
Anrm2WriteDataCnf;

/** Used to discover if a file exists.
 * This signal is used to discover which files are present and how many records they have.
 */
typedef struct Anrm2FileStatusReqTag
{
  /** Task ID of caller */
  TaskId                  taskId;
  /** Identifier for this instance of a command/response;
   * generated by the requesting task
   */
  Int16                   commandRef;
  /** First file to be returned in Anrm2FileStatusCnf */
  Anrm2DataName           firstFileName;
} Anrm2FileStatusReq;

/** Response to file query - also returns file information.
 * This signal returns an array giving information about each file stored in NVRAM. 
 * If moreFiles is TRUE there are more files than are returned in the message in which 
 * case the caller should send another Anrm2FileStatusReq with firstFileName set to the 
 * last file name returned in this Anrm2FileStatusCnf
 */
typedef struct Anrm2FileStatusCnfTag
{
  /** Identifier for this instance of a command/response */
  Int16                   commandRef;
  /** Status report (NVRAM_OK or Layer One error code)*/
  NvStatus                status;
  /** Indicates if there are more files in NVRAM than are returned in current message*/
  Boolean                 moreFiles;
  /** returned file information */
  Anrm2FileInfo           fileInfo[ANRM2_NUM_FILES_IN_FILE_STATUS];
} Anrm2FileStatusCnf;

/** Create a new file */
typedef struct Anrm2CreateFileReqTag
{
  /** Identifier for this instance of a command/response */
  Int16                   commandRef;
  /** Data identifier */
  Anrm2DataName           fileName;
  /** Status report */
  NvStatus                status;
} Anrm2CreateFileReq;

/** Response to a file creation request */
typedef struct Anrm2CreateFileCnfTag
{
  /** Identifier for this instance of a command/response */
  Int16                   commandRef;
  /** Status report */
  NvStatus                status;
  /** Data identifier */
  Anrm2DataName           fileName;
} Anrm2CreateFileCnf;

/** Delete File request.
 * This signal is not used in any source code. */
typedef struct Anrm2DeleteFileReqTag
{
  /** Identifier for this instance of a command/response */
  Int16                   commandRef;
  /** Data identifier */
  Anrm2DataName           fileName;
} Anrm2DeleteFileReq;

/** Response to a file deletion request.
 * This signal is not used in any source code.
 */
typedef struct Anrm2DeleteFileCnfTag
{
  Int16                   commandRef;
  /** Status report */
  NvStatus                status;
  /** Data identifier */
  Anrm2DataName           fileName;
} Anrm2DeleteFileCnf;

/** Request to read a file record.
  This signal is used to read a file record from NVRAM.
  */
typedef struct Anrm2ReadFileRecordReqTag
{
  /** Task ID of caller */
  TaskId                  taskId;
  /** Identifier for this instance of a command/response;
   * generated by the requesting task
   */
  Int16                   commandRef;
  /** File to read from */
  Anrm2DataName           fileName;
  /** Record number to read */
  Int16                   recordNo;
  /** Read mode  */
  Anrm2RwMode             rwMode;
} Anrm2ReadFileRecordReq;

/** Response to a file record read request.
 * This signal returns the record to the requesting task.
 */
typedef struct Anrm2ReadFileRecordCnfTag
{
  /** Identifier for this instance of a command/response */
  Int16                   commandRef;
  /** Status report (NVRAM_OK or NVRAM_REC_NOT_FOUND)*/
  NvStatus                status;
  /** File record has been read from 
\assoc UNION \ref fileData */
  Anrm2DataName           fileName;
  /** Record number read */
  Int16                   recordNo;
  /** TRUE if data payload is valid data */
  Boolean                 isDataValid;
  /** Record data */
  Anrm2DataUnion          fileData;
} Anrm2ReadFileRecordCnf;

/** Request to write a file record.
 * This signal is used to write a record.
 */
typedef struct Anrm2WriteFileRecordReqTag
{
  /** Task ID of caller */
  TaskId                  taskId;
  /** Identifier for this instance of a command/response;
   * generated by the requesting task
   */
  Int16                   commandRef;
  /** File to delete record from 
\assoc UNION \ref fileData */
  Anrm2DataName           fileName;
  /** Record to write */
  Int16                   recordNo;
  /** Write mode */
  Anrm2RwMode             rwMode;
  /** Data to write into record */
  Anrm2DataUnion          fileData;
} Anrm2WriteFileRecordReq;

/** Response to a file record write request */
typedef struct Anrm2WriteFileRecordCnfTag
{
  /** Identifier for this instance of a command/response */
  Int16                   commandRef;
  /** Status report */
  NvStatus                status;
  /** Data identifier 
\assoc UNION \ref fileData */
  Anrm2DataName           fileName;
  /** Record number */
  Int16                   recordNo;
  /** Data payload */
  Anrm2DataUnion          fileData;
} Anrm2WriteFileRecordCnf;

/** Request to return a list of the file's records.
 * This signal is used to read several records from the specified file.
 */
typedef struct Anrm2ListFileRecordsReqTag
{
  /** Task ID of caller */
  TaskId                  taskId;
  /** Identifier for this instance of a command/response */
  Int16                   commandRef;
  /** Data identifier */
  Anrm2DataName           fileName;
  /** First record to read */
  Int16                   startRecordNo;
  /** Read mode */
  Anrm2RwMode             rwMode;
} Anrm2ListFileRecordsReq;

/** Response returning a list of the file's records.
 * This signal is sent as a response to a #Anrm2ListFileRecordsReq
 */
typedef struct Anrm2ListFileRecordsCnfTag
{
  /** Identifier for this instance of a command/response */
  Int16                   commandRef;
  /** Status report (NVRAM_OK or NVRAM_REC_NOT_FOUND)*/
  NvStatus                status;
  /** Data identifier */
  Anrm2DataName           fileName;
  /** Number of records returned 
\assoc ARRAY \ref recordNo */
  Int8                    listSize;
  /** The maximum number of records that could be returned */
  Int8                    maxInList;
  /** The record number of each record returned */
  Int16                   recordNo[ANRM2_MAX_FILE_LIST_SIZE];
  /** Array of Int8 containing an array of the data record type read
   * (Note: not an array of Anrm2DataUnion). 
   * User must allocate enough space which must be less than ANRM2_MAX_FILE_DATA_LIST_SIZE 
   * this needs to be aligned to 16 bit boundary 
   */
  Int8                    fileData[1];
} Anrm2ListFileRecordsCnf;

/** Response to a Data Deletion request */
typedef struct Anrm2DeleteDataCnfTag
{
  /** Identifier for this instance of a command/response */
  Int16             commandRef;
  /** Data identifier */
  Anrm2DataName     name;
  /** Status report */
  NvStatus          status;
}
Anrm2DeleteDataCnf;

/** @} */ /* End of group. */


/***************************************************************************
 *  Macros
 **************************************************************************/
/* none */

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
/* none */

#endif /*INCLUDE_GUARD*/

/* END OF FILE */

