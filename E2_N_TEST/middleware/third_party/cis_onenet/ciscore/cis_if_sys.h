/*******************************************************************************
 *
 * Copyright (c) 2017 China Mobile and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Bai Jie & Long Rong, China Mobile - initial API and implementation
 *    Baijie & Longrong, China Mobile - Please refer to git log
 *
 *******************************************************************************/

/************************************************************************/
/* nb-iot middle software of china mobile api                           */
/************************************************************************/

#include "cis_def.h"
#include "cis_config.h"

#ifndef _CIS_INTERFACE_SYS_H_
#define _CIS_INTERFACE_SYS_H_

#define NBSYS_IMEI_MAXLENGTH    (16)
#define NBSYS_IMSI_MAXLENGTH    (16)

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
	FOTA_STATE_IDIL,
	FOTA_STATE_DOWNLOADING,
	FOTA_STATE_DOWNLOADED,
	FOTA_STATE_UPDATING,
}cis_fw_state;

typedef enum
{
	FOTA_RESULT_INIT,                //0 init
	FOTA_RESULT_SUCCESS,             //1 success
	FOTA_RESULT_NOFREE,              //2 no space
	FOTA_RESULT_OVERFLOW,            //3 downloading overflow
	FOTA_RESULT_DISCONNECT,          //4 downloading disconnect
	FOTA_RESULT_CHECKFAIL,           //5 validate fail
	FOTA_RESULT_NOSUPPORT,           //6 unsupport package
	FOTA_RESULT_URIINVALID,          //7 invalid uri
	FOTA_RESULT_UPDATEFAIL,          //8 update fail
	FOTA_RESULT_PROTOCOLFAIL        //9 unsupport protocol
}cissys_fw_result_type_t;

typedef enum
{
	cissys_event_unknow = 0,
	cissys_event_fw_erase_success,         //for erase, handle /5/0/26501
	cissys_event_fw_erase_fail,  
	cissys_event_write_success,            //for write
	cissys_event_write_fail,
	cissys_event_fw_validate_success,       //for validate
	cissys_event_fw_validate_fail,                 
	cissys_event_fw_update_success,   //for update
	cissys_event_fw_update_fail,
}cissys_event_t;

typedef cis_ret_t (*cissys_event_callback_t)(cissys_event_t id,void* param,void* userData,int *len);

typedef struct st_cissys_callback
{
	void* userData;
	cissys_event_callback_t onEvent;
}cissys_callback_t;

extern cis_cfg_sys_t g_sysconfig;
extern cissys_callback_t g_syscallback;

typedef struct cis_fw_context{
	uint8_t state;  // /5/0/3 
	uint8_t result;  // /5/0/5 
	int savebytes;
}cis_fw_context_t;

cis_ret_t cissys_request_endpoint_name(void);
cis_ret_t cissys_init(void *context,const cis_cfg_sys_t* cfg,cissys_callback_t* event_cb);

//negative return value for error;return millisecond;
uint32_t    cissys_gettime(void);
void        cissys_sleepms(uint32_t ms);
void        cissys_logwrite(uint8_t* buffer,uint32_t length);
void*       cissys_malloc(size_t length);
void        cissys_free(void* ptr);

void*       cissys_memset(void* s, int c, size_t n);
void*       cissys_memcpy(void* dst, const void* src, size_t n);
void*       cissys_memmove(void* dst, const void* src, size_t n);
int         cissys_memcmp(const void* s1, const void* s2, size_t n);
void        cissys_fault(uint16_t id);
void        cissys_assert(bool flag);

uint32_t    cissys_rand(void);
uint8_t     cissys_getIMEI(char* buffer,uint32_t maxlen);
uint8_t     cissys_getIMSI(char* buffer,uint32_t maxlen);

void        cissys_lockcreate(void** mutex);
void        cissys_lockdestory(void* mutex);
cis_ret_t   cissys_lock(void* mutex,uint32_t ms);
void        cissys_unlock(void* mutex);

bool        cissys_save(uint8_t* buffer,uint32_t length);
bool        cissys_load(uint8_t* buffer,uint32_t length);

uint32_t    cissys_getFwVersion(uint8_t** version);

#if CIS_ENABLE_UPDATE


//update
uint32_t    cissys_getFwBatteryLevel();
uint32_t    cissys_getFwBatteryVoltage();
uint32_t    cissys_getCellId();
uint32_t    cissys_getRadioSignalStrength();
uint32_t    cissys_getFwAvailableMemory();

bool        cissys_checkFwValidation(cissys_callback_t *cb);
bool        cissys_eraseFwFlash (cissys_callback_t *cb);
uint32_t    cissys_writeFwBytes(uint32_t size,uint8_t* buffer,cissys_callback_t *cb);
bool        cissys_updateFirmware(cissys_callback_t *cb);

bool		cissys_readContext(cis_fw_context_t * context);
//int         cissys_setFwSavedBytes(uint32_t length);
int         cissys_getFwSavedBytes();// -1 error  
bool        cissys_setFwState(uint8_t state);
bool        cissys_setFwUpdateResult(uint8_t result);
bool        cissys_setPSMMode(bool enable);



#endif
#ifdef __cplusplus
};
#endif

#endif//_CIS_INTERFACE_SYS_H_
