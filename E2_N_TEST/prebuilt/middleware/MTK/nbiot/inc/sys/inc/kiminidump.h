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

#include "system.h"
#include "memory_attribute.h"
#include "kisystyp.h"

#ifndef GKI_MINIDUMP_H
#define GKI_MINIDUMP_H

#define MINIDUMP_FRT_INVALID_PTR                ((void*)0xFFFFFFFF)
#define MINIDUMP_EXCEPTION_ID_MASK              0x000001FF
#define MINIDUMP_NOT_EXCEPTION                  0x00000000
#define MINIDUMP_ERROR                          0xFFFFFFFF
#define MINIDUMP_MAGIC                          0xFDFDBABA
#define MINIDUMP_VERSION                        0x00000001
#define HISTORY_INFO_WRAP                       0x00
#define HISTORY_INFO_NOWRAP                     0x01
#define HISTORY_INFO_ERROR                      0x03
#define MINIDUMP_ASSERT_INFO_MAX_SIZE           128
#define MINIDUMP_CONTEXT_SWITCH_SIZE            3
#define MINIDUMP_CONTEXT_SWITCH_SLEEP_SIZE      6
#define MINIDUMP_AP_TASK_NAME_SIZE              7
#define MINIDUMP_AP_TASK_SWITCH_SIZE            10
#define MINIDUMP_SIGNAL_SEND_HISTORY_MAX_NUM    40
#define MINIDUMP_CONTEXT_SWITCH_HISTORY_MAX_NUM 1024
#define MINIDUMP_BLOCK_MAX_NUM                  32
#define MINIDUMP_ASSERT_MAX_SIZE                500
#define MINIDUMP_BOOT_UP_TIME_SIZE              100
#define MEMORY_ALLOCATE_EMPTY_ITEM_NUM          30
#define MEMORY_CORRUPTION_FORESEARCH_SIZE       383
#define MINIDUMP_SIGNAL_SEND_FLAG               0
#define MINIDUMP_SIGNAL_RECEIVE_FLAG            1
#define MINIDUMP_BLOCK_TASK_HISTORY_NUM         0x05
#define MINIDUMP_TASKSTATE_READY_FLAG           0x9F
#define MINIDUMP_TASKSTATE_RUNNING_FLAG         0X20
#define MINIDUMP_TASKSTATE_SUSPEND_FLAG         0x60
#define MINIDUMP_TASKSTATE_BLOCKED_FLAG         0x40
#define MINIDUMP_TASKSTATE_NUM_MASK             0X80
#define MINIDUMP_SIGNAL_SEND_CHECK              0x01
#define MINIDUMP_TASKSWITCH_CHECK               0x02
#define MINIDUMP_TASKINFO_CHECK                 0x03
#define MINIDUMP_TASKINFO_HISTORY_EMPTY_MASK    0xE0
#define MINIDUMP_TASK_HISTORY_INFO_NUM          10
#define MINIDUMP_GKI_TASK_FLAG                  0x01
#define MINIDUMP_AP_TASK_FLAG                   0x02
#define MINIDUMP_INTERRUPT_ENTER_FLAG           0x03
#define MINIDUMP_INTERRUPT_EXIT_FLAG            0x04
#define MINIDUMP_LIGHT_SLEEP_FLAG               0x05
#define MINIDUMP_SIG_SEND_FLAG                  0x80
#define MINIDUMP_SIG_RECEIVE_FLAG               0x7F
#define MINIDUMP_INT_OFFSET                     48
#define MINIDUMP_APTASK_OFFSET                  128
#define MINIDUMP_NO_GKI_SEMA                    0xFF
#define MINIDUMP_NO_SIG_ID                      0x00
#define MINIDUMP_MODEM_MAX_USER_ID              0x0A
#define MINIDUMP_MODEM_DUMP_SIZE                0x400
#define MINIDUMP_TASK_UNPROCESSED_MESSAGE_NUM   30
#define MINIDUMP_MEMORY_BLOCK_OFFSET            1024
#define MINIDUMP_BUFFER_QUEUE_MSG_COUNT_SIZE    4

#define MINIDUMP_ENTER_LIGHT_SLEEP              0xFEFEFEFE
#define MINIDUMP_EXIT_LIGHT_SLEEP               0xFFFFFFFF
#define MINIDUMP_ISR_END                        0xFDFDFDFD

/*Modem mini dump data*/
#define MINIDUMP_MODEM_CHECKVALUE_SIZE          4
#define MINIDUMP_MODEM_USER_HEADER_SIZE         5
#define MINIDUMP_MODEM_MAX_SIZE                 (MINIDUMP_MODEM_DUMP_SIZE \
                                                + MAX_USERID               \
                                                * (MINIDUMP_MODEM_CHECKVALUE_SIZE + MINIDUMP_MODEM_USER_HEADER_SIZE)\
                                                )

#if defined(__GNUC__)
#define STRUCT_PACKED                           __attribute__((packed))
#else
#define STRUCT_PACKED
#endif /* __GNUC__ */

/***************************************************
* Modem L1 mini dump data, modem user ID enum value
* If one user want to register and save data in mini dump data,
* you must add your user id enum value in  MiniDump_ModemUserIdT.

* For example, 
    typedef enum MiniDump_ModemUserIdType
    {
        USER_ID0 = 0x00,
        USER_ID1 = 0x01,
        MAX_USERID
    }MiniDump_ModemUserIdT;
* USER_ID0 and USER_ID1 are the new enum values.

* Note: the MAX_USERID value should not larger than the 0x0A,
* it will generate build error when the value of MAX_USERID is larger
* than 0x0A.
****************************************************/
typedef enum MiniDump_ModemUserIdType
{
    MAX_USERID
}MiniDump_ModemUserIdT;


/*system assert type*/
typedef enum MiniDump_AssertType
{
    /*System Assert start type*/
    SYSTEM_ASSERT_TYPE                                 = 0x0000,
    /*memory assert type*/
    MEMORY_ASSERT_TYPE                                 = 0x0100,
    /*memory os header(next value) is damaged by previous memory pool's block*/
    MEMORY_OSHEADER_NEXT_VALUE_OVERWRITE_BY_PREV_BLOCK = 0x0101,
    /*memory os header(next alue) is damaged by the above memory of current pool*/
    MEMORY_OSHEADER_NEXT_VALUE_OVERWRITE_ABOVE_POOL    = 0x0102,
    /*memory os header(pool value) is damaged by previous memory pool's block*/
    MEMORY_OSHEADER_POOL_VALUE_OVERWRITE_BY_PREV_BLOCK = 0x0103,
    /*memory os header(pool value) is damaged by the above memory of current pool*/
    MEMORY_OSHEADER_POOL_VALUE_OVERWRITE_ABOVE_POOL    = 0x0104,
    /*memory block check value is damaged by the current block or previous block*/
    MEMORY_MEM_FOOTER_OVERWRITE_BY_PREV_BLOCK          = 0x0105,
    /*memory block check value is damaged by the above memory of current pool*/
    MEMORY_MEM_FOOTER_OVERWRITE_ABOVE_POOL             = 0x0106,
    /*signal(create in task) check value is damaged by the current block or previous block*/
    MEMORY_SIG_FOOTER_OVERWRITE_BY_PREV_BLOCK          = 0x0107,
    /*signal(create in task)  check value is damaged by the above memory of current pool*/
    MEMORY_SIG_FOOTER_OVERWRITE_ABOVE_POOL             = 0x0108,
    /*signal(create in interrupt) check value is damaged by the current block or previous block*/
    MEMORY_SIG_FOOTER_OVERWRITE_BY_PREV_BLOCK_INT      = 0x0109,
    /*signal(create in interrupt)  check value is damaged by the above memory of current pool*/
    MEMORY_SIG_FOOTER_OVERWRITE_ABOVE_POOL_INT         = 0x010A,
    /*current memory pool is empty, all the blocks are allocated*/
    MEMORY_ALLOCATE_EMPTY                              = 0x010B,
    
    /*modem dump assert type*/
    MODEMDUMP_ASSERT_TYPE                              = 0x0200,
    /*modem invalid parameter(invalid userid, call back function or reqsize)*/
    MODEMDUMP_INVALID_PARAM                            = 0x0201,
    /*modem have not enough memory to complete this register*/
    MODEMDUMP_NO_ENOUGH_MEMORY                         = 0x0202,
    /*modem same user calls twice register functions*/
    MODEMDUMP_RE_REG_NORMAL_DUMP                       = 0x0203,
    /*modem  user(same user or different user) calls twice debug register functions*/
    MODEMDUMP_RE_REG_FORCE_DUMP                        = 0x0204,
     /*modem  user calls write data function with not in exception flow*/
    MODEMDUMP_CALL_WRITE_FUN_NOT_IN_EXCEPTION          = 0x0205,
     /*modem  checkvalue has been overwrite*/
    MODEMDUMP_FOOTER_OVERWRITE                         = 0x0206,

    /*system dev-check type*/
    SYSTEM_DEV_CHECK_TYPE                              = 0x0300,
    SYSTEM_DEV_CHECK_SIGNAL_TYPE_ERROR                 = 0x0301,
    SYSTEM_DEV_CHECK_SIGNAL_SIG_ERROR                  = 0x0302,
    SYSTEM_DEV_CHECK_INVALID_SIGNAL_ID                 = 0x0303,
    SYSTEM_DEV_CHECK_INVALID_TASK_STATE                = 0x0304,
    /*system invalid parameter type*/
    SYSTEM_DEV_PARAM_TYPE                              = 0x0400,
    SYSTEM_DEV_PARAM_SIG_SEND_INVALID_INDEX            = 0x0401,
    SYSTEM_DEV_PARAM_TASK_INFO_INVALID_INDEX           = 0x0402,
    SYSTEM_DEV_PARAM_TASK_INTERNEL_QUEUE_ERROR         = 0x0403,
    /*Other Assert type*/
    UNKNOWN_ASSERT_TYPE                                = 0xFFFF
}MiniDump_AssertT;

/*mini dump Section's ID*/
typedef enum MiniDump_SectionIDType
{
    ASSERT_SECTION_ID                                = 0x00,
    SIG_SECTION_ID                                   = 0x01,
    TASK_INFO_SECTION_ID                             = 0x02,
    TASK_UNPROCESSED_MESSAGE_SECTION_ID              = 0x03,
    CONTEXT_SWITCH_SECTION_ID                        = 0x04,
    MODEM_SECTION_ID                                 = 0x05,
    BOOT_UP_TIME_SECTION_ID                          = 0x06,
    
    SECTION_NUM               
}MiniDump_SectionIDT;


/***************************************************************************
 GKI minidump structures
 ***************************************************************************/
/****************************************/
/*RAM data information structure data*/
/****************************************/
typedef struct MiniDump_SignalSendType
{
    uint32_t SignalId;   
    uint32_t TimeStamp;  
    uint32_t ThreadId; 
    uint32_t DestTaskId;  
}MiniDump_SignalSendT;

typedef struct MiniDump_ContextSwitchType
{
    uint8_t Type;
    uint32_t TimeStamp;  
    uint32_t ContextArg;
    uint32_t SleepTime;
    char TaskName[7];
}MiniDump_ContextSwitchT;

typedef struct MiniDump_TaskInfoType
{
    uint32_t SignalId; 
    uint32_t TimeStamp;  

    uint8_t InfoTag;
    uint32_t TaskId;
}MiniDump_TaskInfoT;

typedef struct MiniDump_TaskInfoHistoryType
{
    MiniDump_TaskInfoT MiniDump_TaskHistory[MINIDUMP_TASK_HISTORY_INFO_NUM];
    uint8_t MiniDump_HistoryIndex;   
}MiniDump_TaskInfoHistoryT;

typedef struct MiniDump_MemoryGkiAssertType
{
    uint32_t AssertType;
    void *BlockAddress;
}MiniDump_MemoryGkiAssertT;

typedef struct MiniDump_AllocateEmptyType
{
    uint8_t AllocSource;
    uint8_t TaskId;
    uint32_t CallerAddress;
    uint32_t SignalId;
    uint16_t Num;
}MiniDump_AllocateEmptyT;

/*Modem L1 mini dump data*/
typedef void (*MiniDump_CallBackFun)(MiniDump_ModemUserIdT );
typedef struct MiniDump_ModemType
{
    uint32_t ReqSize;
    void *StartAddr;
    void (*CallBack)(MiniDump_ModemUserIdT );
}MiniDump_ModemT;

typedef struct MiniDump_MemoryModemAssertType
{
    uint32_t AssertType;
    MiniDump_ModemUserIdT UserId;
}MiniDump_MemoryModemAssertT;

typedef struct MiniDump_AssertInfoType
{
    MiniDump_AssertT Type;
    uint32_t DumpArg;
    uint32_t SysArg1;
    uint32_t SysArg2;
    uint32_t SysArg3;
}MiniDump_AssertInfoT;


/****************************************/
/*Mini dump Bin file packed struct information*/
/****************************************/
typedef struct MiniDump_BIN_HeaderType
{
    uint32_t Magic;
    uint32_t Version;
    uint8_t SectionNum;
}STRUCT_PACKED MiniDump_BIN_HeaderT;

typedef struct MiniDump_BIN_SectionType
{
    uint8_t SectionId;
    uint16_t SectionStartAddress;
    uint16_t SectionSize;
}STRUCT_PACKED MiniDump_BIN_SectionT;

typedef struct MiniDump_BIN_SignalSendType
{
    uint16_t TimeStamp;  
    uint32_t SignalId;  
    uint8_t SourceThreadId; 
    uint8_t DestTaskId;  
}STRUCT_PACKED MiniDump_BIN_SignalSendT;

typedef struct MiniDump_BIN_ContextSwitchHeaderType
{
    uint16_t TimeStamp;
    uint32_t StartAddr;
}STRUCT_PACKED MiniDump_BIN_ContextSwitchHeaderT;


typedef struct MiniDump_BIN_ContextSwitchType
{
    uint8_t ContextArg;
    uint16_t TimeStamp;
}STRUCT_PACKED MiniDump_BIN_ContextSwitchT;

typedef struct MiniDump_BIN_TaskInfoType
{
    uint8_t InfoTag;
    uint16_t TimeStamp;  
    uint8_t TaskID;
    uint32_t SignalID; 
}STRUCT_PACKED MiniDump_BIN_TaskInfoT;

typedef struct MiniDump_BIN_SuspendTaskType
{
    uint8_t InfoTag;
    uint32_t TimeStamp;
    uint8_t SemaphoreId;
}STRUCT_PACKED MiniDump_BIN_SuspendTaskT;

typedef struct MiniDump_BIN_UnProcessedMessageHeaderType
{
    uint8_t MessageNum;
}STRUCT_PACKED MiniDump_BIN_UnProcessedMessageHeaderT;


typedef struct MiniDump_BIN_UnProcessedMessageType
{
    uint32_t SignalID;
    uint8_t SourceTaskId;
}STRUCT_PACKED MiniDump_BIN_UnProcessedMessageT;

typedef struct MiniDump_BIN_AssertHeaderType
{
    uint32_t AssertType;
    uint8_t PoolNum;
    uint8_t ThreadId;
    char TaskName[16];
    char AssertInfo[MINIDUMP_ASSERT_INFO_MAX_SIZE];
}STRUCT_PACKED MiniDump_BIN_AssertHeaderT;

typedef struct MiniDump_BIN_AllocateEmptyType
{
    uint8_t AllocSource;
    uint8_t TaskId;
    uint32_t CallerAddress;
    uint32_t SignalId;
    uint16_t Num;
}STRUCT_PACKED MiniDump_BIN_AllocateEmptyT;

typedef struct MiniDump_BIN_CorruptionHeaderType
{
   uint32_t StartAddr;
   uint32_t DamageAddr;
   uint32_t DamageData;
}STRUCT_PACKED MiniDump_BIN_CorruptionHeaderT;

typedef struct MiniDump_BIN_ModemAssertType
{
   MiniDump_ModemUserIdT UserId;
   uint16_t ReqSize;
   uint32_t CallBackAddr;
}STRUCT_PACKED MiniDump_BIN_ModemAssertT;


typedef struct MiniDump_BIN_ModemUserHeaderType
{
    MiniDump_ModemUserIdT UserId;
    uint32_t ReqSize;
}STRUCT_PACKED MiniDump_BIN_ModemUserHeaderT;

/*Enable Mini dump*/
#ifdef MTK_MINI_DUMP_ENABLE
/** System check function
 * This function is used for system check and allocate corresponding error information in gki mini dump
 * This function can replace DevCheck() function.
 * \param[in] cond   check condition
 * \param[in] type  assert type
 * \param[in] dumparg  gki minidump information
 * \param[in] arg1  parameter 1
 * \param[in] arg2  parameter 2
 * \param[in] arg3  parameter 3
 */
extern MiniDump_AssertInfoT MiniDump_AssertInfo;
#define SysCheck(cond,type,dumparg,arg1,arg2,arg3) \
    {                                      \
        if(!(cond))                         \
        {                                  \
            MiniDump_AssertInfo.Type       = (MiniDump_AssertT)type;    \
            MiniDump_AssertInfo.DumpArg    = (uint32_t)dumparg;   \
            MiniDump_AssertInfo.SysArg1    = (uint32_t)arg1;      \
            MiniDump_AssertInfo.SysArg2    = (uint32_t)arg2;      \
            MiniDump_AssertInfo.SysArg3    = (uint32_t)arg3;      \
            DevParam(arg1,arg2,arg3);            \
        }                                           \
    }
/** System parameter function
 * This function is used for system parameter allocate and add the corresponding error information in gki mini dump
 * This function can replace DevParam() function.
 * \param[in] cond   Modem User Id
 * \param[in] type  assert type
 * \param[in] dumparg  gki minidump information
 * \param[in] arg1  parameter 1
 * \param[in] arg2  parameter 2
 * \param[in] arg3  parameter 3
 */
#define SysParam(type,dumparg,arg1,arg2,arg3) \
    {                                      \
        MiniDump_AssertInfo.Type       = (MiniDump_AssertT)type;    \
        MiniDump_AssertInfo.DumpArg    = (uint32_t)dumparg;   \
        MiniDump_AssertInfo.SysArg1    = (uint32_t)arg1;      \
        MiniDump_AssertInfo.SysArg2    = (uint32_t)arg2;      \
        MiniDump_AssertInfo.SysArg3    = (uint32_t)arg3;      \
        DevParam(arg1,arg2,arg3);            \
    }

/***************************************************************************
 * Kernel Function redirections
 **************************************************************************/

/** System mini dump initialization
 * This function is used to initialise the gki mini dump parameters during the system initialization stage.
*/
#define Md_Initialise()            MiniDump_Initialise()

/** L1 modem dump user register function
 * This function is used for modem user to register their mini dump functions.
 * This allows modem user to register their function to system mini dump functions.
 * User should provide the valid user id, required size and it's call back function.
 * When register success, system will return true. Otherwise, invalid UserId, no enough
 * memory size and invalid call back function will return false and resulted in system 
 * assert fail exception flow.
 * \param[in] I   Modem User Id
 * \param[in] S  Require Size
 * \param[in] C  Modem User register call back fucntion
 */
#define Md_ModemDumpReg(I,S,C)     MiniDump_ModemDumpRegister(I,S,C)

/** L1 modem dump user de-register function
 * This function is used for modem user to de-register their mini dump functions.
 * This allows modem user to de-register their function to system mini dump functions.
 * User should provide the valid user id which want to de-register from the gki mini dump.
 * When de-register success, system will return true. Otherwise, invalid UserId will return 
 * false and resulted in system assert fail exception flow.
 * When a user is de-register from the system, mini dump will not dump their data during
 * the system exception flow and it's require memory size will not recycle by the system.
 * \param[in] I   Modem User Id
 */
#define Md_ModemDumpDeReg(I)        MiniDump_ModemDumpDeRegister(I) 

/** L1 modem dump user debug register(modem register force) function.
 * This function is used for modem user to register force their mini dump functions.
 * User should provide the valid user id and Call back function which want to register force 
 * from the gki mini dump.
 * When register force success, system will return true. Otherwise, invalid UserId and call back
 * will return false and resulted in system assert fail exception flow.
 * When a user successed called debug register function from the system, other registered user will be
 * de-register from the system and the total modem dump size will be allocated to called user id.
 * This function can only been called once, multiple calls will cause system exceptions.
 * \param[in] I   Modem User Id
  * \param[in] C  Modem Call Back function
 */
#define Md_ModemDumpDebugReg(I,C)   MiniDump_ModemDumpDebugRegister(I,C)

/** L1 modem dump user write data in mini dump memory
 * This function is used for modem user to write data ti mini dump memory
 * User should provide the valid user id, the offset value that begins writing data
 * and write data size and the pointer to data.
 * When write to mini dump memory success, system will return true. Otherwise, invalid UserId or the
 * write operation out of bounds will return false and resulted in system assert fail exception flow.
 
 ***********************************************************************************
 * Note:This function can only be called in exception state, otherwise will trigger an assert fail error!!!
  ***********************************************************************************
  
 * \param[in] I   Modem User Id
  * \param[in] O  start write offset
   * \param[in] S   data size
  * \param[in] D  pointer to data
 */
 #define Md_ModemDumpWrite(I,O,S,D)  MiniDump_ModemDumpWriteData(I,O,S,D)
 
/** System record signal send history
 * This function is used to record the signal send history info.
 * Record the sinal send time stamp, signal id, the source task id and the dest task id of this signal.
 * \param[in] SigH     KiSigHeader address
 */ 
#define Md_RecordSigSend(SigH)     Minidump_RecordSignalSendHistory(SigH)

/** System record gki task swtich
 * This function is used to record the gki task switch info
 * \param[in] P    gki task priority
 */ 
#define Md_RecordGkiTaskSwitch(P)   MiniDump_RecordGkiTaskSwitchHistory(P)

/** System record ap task swtich
 * This function is used to record the ap task switch info
 * \param[in] A    ap task name address
 */ 
#define Md_RecordAPTaskSwitch(A)   MiniDump_RecordAPTaskSwitchHistory(A)

/** System record interrupt enter info
 * This function is used to record the interrupt enter callback function info
 * \param[in] I    interrupt id
 */ 
#define Md_RecordIntEnter(I)      MiniDump_RecordEnterInterruptInfo(I)

/** System record interrupt exit info
 * This function is used to record the interrupt exit callback function info
 */ 
#define Md_RecordIntExit()      MiniDump_RecordExitInterruptInfo()

/** Record system enter light sleep
 * This function is used to record the system exit from light sleep info
 */
#define Md_RecordEnterLightSleep()       MiniDump_RecordEnterLightSleepInfo()

/** Record system exit light sleep
 * This function is used to record the system exit from light sleep info
 * \param[in] S    Sleep Time
 */
#define Md_RecordExitLightSleep(S)       MiniDump_RecordExitLightSleepInfo(S)

/** System record task wait semaphore time stamp 
 * This function is used to record the task wait semaphore time stamp before task 
 * enter the blocked state
 */ 
#define Md_RecordWaitSemaTime()    Minidump_RecordTaskWaitSemaTime()

/** System record task receive or send a signal
 * This function is used to record the task receive a signal
 * If the task is in send signal state, record the signal dest task id. Otherwise, 
 * record the source task id.
 * \param[in] SigH     KiSigHeader address
 * \param[in] F   The flag of task send or receive a signal
 */ 
#define Md_RecordTaskInfo(SigH,F)   MiniDump_RecordTaskInfoHistory(SigH,F)

#else/*disable Mini dump*/
#define Md_Initialise()

#define Md_ModemDumpReg(I,S,C)

#define Md_ModemDumpDReg(I)

#define Md_ModemDumpDebugReg(I,C)

#define Md_ModemDumpWrite(I,O,S,D)

#define Md_RecordSigSend(SigH)

#define Md_RecordGkiTaskSwitch(P)

#define Md_RecordAPTaskSwitch(A)

#define Md_RecordIntSwitch(I)

#define Md_RecordEnterLightSleep()

#define Md_RecordExitLightSleep(S)

#define Md_RecordWaitSemaTime()

#define Md_RecordTaskInfo(SigH,F)

#define SysCheck(cond,type,dumparg,arg1,arg2,arg3) DevCheck(cond,arg1,arg2,arg3)
#define SysParam(type,dumparg,arg1,arg2,arg3)      DevParam(arg1,arg2,arg3)
#endif  /* MTK_MINI_DUMP_ENABLE */


#ifdef MTK_MINI_DUMP_ENABLE
/***************************************************************************
 * Function prototypes
 **************************************************************************/
void MiniDump_Initialise(void);

void MiniDump_ModemDumpRegister(MiniDump_ModemUserIdT UserId, 
                                            uint32_t ReqSize, 
                                            MiniDump_CallBackFun CallBackFun);

void MiniDump_ModemDumpDebugRegister(MiniDump_ModemUserIdT UserId, 
                                                   MiniDump_CallBackFun CallBackFun);

void MiniDump_ModemDumpWriteData(MiniDump_ModemUserIdT UserId, 
                                                                uint32_t offset, 
                                                                uint32_t size, 
                                                                void *data);

void Minidump_RecordSignalSendHistory(KiSigHeader *SigHeader);

void MiniDump_RecordGkiTaskSwitchHistory(uint32_t priority);

void MiniDump_RecordAPTaskSwitchHistory(char *NameAddr);

void MiniDump_RecordEnterInterruptInfo(uint32_t irqID);

void MiniDump_RecordExitInterruptInfo(void);

void MiniDump_RecordEnterLightSleepInfo(void);

void MiniDump_RecordExitLightSleepInfo(uint32_t SleepTime);

void Minidump_RecordTaskWaitSemaTime(void);

void MiniDump_RecordTaskInfoHistory(KiSigHeader *SigHeader, 
                                              uint8_t SendOrReceive);

/*FreeRTOS api*/
uint32_t xTaskGetWaitingSemaAddr(void *pxTCB);

uint8_t xTaskGetTaskState(void * TCB);

uint32_t xQueueGetWaitingSemaAddr(void *pxQueueAddr);

#endif  /* MTK_MINI_DUMP_ENABLE */
#endif  /* GKI_MINIDUMP_H */





