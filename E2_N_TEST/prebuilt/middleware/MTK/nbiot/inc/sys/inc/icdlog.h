/* Copyright Statement:
 *
 * (C) 2005-2018  MediaTek Inc. All rights reserved.
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

/*---------------------------------------------------------------------------
 | Filename:
 | ---------
 |   icdlog.h
 |
 | Description:
 | ------------
 |   This is an interface control document(ICD) log based on GKI log system.
 |
 ---------------------------------------------------------------------------*/
#ifndef __ICDLOG_H__
#define __ICDLOG_H__

#include <system.h>
#if 0
#include "icd_codes.h"
#else
#define ICD_EVENT_BASE_ID   0x8204
#define ICD_EVENT_MAX_ID    0x82E1
#define ICD_RECORD_BASE_ID  0x8014
#define ICD_RECORD_MAX_ID   0x8050
#endif

/******************************************************************************
 *
 *  Typedefs
 *
 *******************************************************************************/
#if defined(__GNUC__)
#define _PACKED_    __attribute__((packed))
#else
#define _PACKED_
#endif /* __GNUC__ */

 /****************************************************
 * One ICD log packet:
 * |ICD Log Header|ICD Header|ICD Data|ICD Log Tailer|
 *       6B           12B                   2B
 * ICD Data: aligned by users
 *****************************************************/
typedef struct
{
    uint16_t Head;
    uint16_t Sequence;
    uint16_t Len;                //Len: FrameNum + ICD Header + ICD Data
    uint32_t FrameNum;
}_PACKED_ IcdLogPktHeaderT;

typedef struct
{
    uint16_t Tail;
}_PACKED_ IcdLogPktTailerT;

typedef struct
{
    uint32_t type:4;             //event/record/command type
    uint32_t version:4;
    uint32_t total_len:16;       //include ICD Header and ICD Data
    uint32_t timestamp_type:4;   //0: timestamp 0 byte; 1: timestamp 4 bytes
    uint32_t reserved_1:4;
    uint16_t code_id;            //event/record/command id
    union
    {
        uint8_t reserved_2;      //for event/record
        uint8_t token;           //for command
    }data;
    uint8_t  header_checksum;    //sum of all bytes before header_checksum
    uint32_t timestamp;
}_PACKED_ IcdPacketHeaderT;

/*>>> LTE ICD header format */
typedef struct
{
    uint32_t type:4;
    uint32_t version:4;
    uint32_t total_len:24;
    uint32_t timestamp_type:4;
    uint32_t protocol_id:4;
    uint32_t reserved:8;
    uint32_t record_code:16;
    uint32_t header_checksum;
    uint32_t timestamp;
}_PACKED_ IcdTestRecordT;

typedef struct
{
    uint32_t type:4;
    uint32_t version:4;
    uint32_t total_len:16;
    uint32_t timestamp_type:4;
    uint32_t protocol_id:4;
    uint32_t event_code:16;
    uint32_t header_checksum:16;
    uint32_t timestamp;
}_PACKED_ IcdTestEventT;

typedef struct
{
    uint32_t type:4;
    uint32_t version:4;
    uint32_t total_len:24;
    uint32_t timestamp_type:4;
    uint32_t reserved:4;
    uint32_t token:8;
    uint32_t command_id:16;
    uint32_t header_checksum;
    uint32_t timestamp;
}_PACKED_ IcdTestCommandT;
/* LTE ICD header format <<<*/

typedef struct
{
    void *address;
    uint32_t length;
}IcdDataDesT;

typedef enum
{
    ICD_EVENT = 0,
    ICD_RECORD,
    ICD_COMMAND,
    ICD_MAX_TYPE = ICD_COMMAND
}IcdPacketType;

typedef enum
{
    ICD_SYSTEM_NOTIFICATION_EVENT_FILTER_UPDATE = 0,
    ICD_SYSTEM_NOTIFICATION_REOCRD_FILTER_UPDATE,
    ICD_SYSTEM_NOTIFICATION_START_LOGGING,
    ICD_SYSTEM_NOTIFICATION_STOP_lOGGING
}IcdSystemNotificationEvent;

typedef void (*IcdSystemNotificationCb) (IcdSystemNotificationEvent event, uint32_t flag, uint32_t notificationContextLen, char *notificaionContext);

/****************************************************
 * ICD Command Definition 
 ****************************************************/
/* Command code id */
#define ICD_CMD_BASE_ID                    0x0

#define ICD_CMD_ERROR_REPORT_RESPONSE      0x0
#define ICD_CMD_QUERY_VERSION_COMMAND      0x1
#define ICD_CMD_QUERY_VERSION_RESPONSE     0x2
#define ICD_CMD_LOG_CONFIGURATION_COMMAND  0x3
#define ICD_CMD_LOG_CONFIGURATION_RESPONSE 0x4
#define ICD_CMD_RETRIEVE_FILTER_COMMAND    0x5
#define ICD_CMD_RETRIEVE_FILTER_RESPONSE   0x6
#define ICD_CMD_SET_FILTER_COMMAND         0x7
#define ICD_CMD_SET_FILTER_RESPONSE        0x8

#define ICD_CMD_MAX_ID                     0x8

/* Command struct */
typedef struct
{
    uint8_t error_code;
}_PACKED_ icd_cmd_error_report_response_struct;

/* ICD_CMD_QUERY_VERSION_COMMAND: No payload */

typedef struct
{
    uint8_t software_version[64];
    uint8_t hardware_version[64];
    uint8_t major_version;
    uint8_t minor_version;
}_PACKED_ icd_cmd_query_version_response_struct;

typedef struct
{
    uint8_t operation_code;  //0: enable logging; 1: disable logging
}_PACKED_ icd_cmd_log_configuration_command_struct;

typedef struct
{
    uint8_t status;          //0: success; 1: fail
}_PACKED_ icd_cmd_log_configuration_response_struct;

typedef struct
{
    uint8_t type;
    uint8_t option;          //0: without filter data(retrieve base/max id only); 1: with filter data
}_PACKED_ icd_cmd_retrieve_filter_command_struct;

typedef struct
{
    uint16_t base_id;
    uint16_t max_id;
    uint8_t  filter_data;    //if available, length: (max_id - base_id + 8)/8
}_PACKED_ icd_cmd_retrieve_filter_response_struct;

typedef struct
{
    uint8_t  type;
    uint16_t start_id;       //start%8 == 0
    uint16_t end_id;         //(end - start + 1)%8 == 0
    uint8_t  filter_data;    //length: (end -start + 8)/8
}_PACKED_ icd_cmd_set_filter_command_struct;

typedef struct
{
    uint8_t status;          //0: success; 1: fail
}_PACKED_ icd_cmd_set_filter_response_struct;

/* Command enum */
typedef enum
{
    ICD_ERROR_CMD_PTOTOCOL_ERROR = 0,
    ICD_ERROR_CMD_ID_OUT_OF_RANGE,
    ICD_ERROR_CMD_UNEXPECTED_PAYLOAD_VALUE,
    ICD_ERROR_EXCEPTION_HAPPEND,
}IcdErrorID;

typedef enum
{
    ICD_TIMESTAMP_NO_USE    = 0,
    ICD_TIMESTAMP_IN_USE    = 1,

    ICD_CMD_EXECUTE_SUCCESS = 0,
    ICD_CMD_EXECUTE_FAIL    = 1,

    ICD_CMD_WITHOUT_FILTER  = 0,
    ICD_CMD_WITH_FILTER     = 1,

    ICD_CMD_START_LOGGING   = 0,
    ICD_CMD_STOP_LOGGING    = 1,
}IcdCmdOperationEnum;


/******************************************************************************
 *
 *  Macros
 *
 *******************************************************************************/
#define ICD_LOG_PACKET_HEADER_LABEL     0xCACA
#define ICD_LOG_PACKET_TAILER_LABEL     0xFDFD

#define ICD_LOG_HEAD_TAIL_SUM_SIZE      (sizeof(IcdLogPktHeaderT) + sizeof(IcdLogPktTailerT))

/*filters*/
#define ICD_ID_BYTE_LENGTH(begin, end)  ((end - begin + 8)/8)
#define ICD_ID_BYTE_OFFSET(begin, end)  ((end - begin)/8)
#define ICD_ID_BIT_OFFSET(begin, end)   ((end - begin)%8)

#define ICD_SYSTEM_NOTIFICATION_REGISTER_MAX_NUMBER 32

/* Align 4 bytes */
/* event/record/command header and tailer total 20/24/24 bytes */
#define ICD_LOG_DATA_ALIGN(_len_) ((_len_ + 3) & 0xFFFC)

/*******************************************************************************
 *
 *  Functions prototypes
 *
 *******************************************************************************/

/*******************************************************************************
 * Function   : IcdCheckFilter
 * Parameters : icdCodeId: ICD event/record id
 * Returns    : true:the icdCodeId filter is enable; flase: disable
 * Description: check whether the codeId filter is enable
 *******************************************************************************/
bool IcdCheckFilter(uint32_t icdCodeId);

/*******************************************************************************
 * Function   : IcdLogTrace
 * Parameters : icdCodeId: the icd event/record id;
                data: the icd log data address;
                length: the icd log data length;
 * Returns    : true for success, false for fail
 * Description: save a icd log data segment to log buffer, it must be continuous
 *******************************************************************************/
bool IcdLogTrace(uint32_t icdCodeId, void *data, uint32_t length);

/*******************************************************************************
 * Function   : IcdLogTraceMultiData
 * Parameters : icdCodeId: the icd event/record id;
                dataDes: the data segment address and length;
                dataDesNum: the num of data segment;
 * Returns    : true for success, false for fail
 * Description: save icd log multi data segment to log buffer, these can be
                discontinuous. This function will copy these data segment to log
                buffer continously.
 *******************************************************************************/
bool IcdLogTraceMultiData(uint32_t icdCodeId, IcdDataDesT *dataDes, uint32_t dataDesNum);

/*******************************************************************************
 * Function   : IcdSystemNotificationRegister
 * Parameters : cbFunc: the callback function
 * Returns    : true for success, false for fail
 * Description: register a cbFunc to the IcdSystemNotificationTable[],
                and the the cbFunc will be called when icd update filter or
                enable/disable logging
 *******************************************************************************/
bool IcdSystemNotificationRegister(IcdSystemNotificationCb cbFunc);

/*******************************************************************************
 * Function   : IcdSystemNotificationUnregister
 * Parameters : cbFunc: the callback function
 * Returns    : void
 * Description: unregister the cbFunc from IcdSystemNotificationTable[]
 *******************************************************************************/
void IcdSystemNotificationUnregister(IcdSystemNotificationCb cbFunc);

#endif /* __ICDLOG_H__ */

