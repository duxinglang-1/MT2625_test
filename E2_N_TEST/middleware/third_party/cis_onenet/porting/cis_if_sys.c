#include <cis_if_sys.h>
#include <cis_def.h>
#include <cis_internals.h>
#include <cis_config.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hal.h"
#include "hal_gpt.h"
#include "hal_trng.h"
#include "cis_log.h"
#include "cis_list.h"
#include "ril.h"
#include <sys/time.h>

void testThread(void* arg);

static char cissys_imsi[20] = {0};
static char cissys_imei[20] = {0};

extern void *g_lockImei;
extern void *g_lockImsi;

int32_t cissys_cgsn_callback(ril_cmd_response_t *response)	
{
	ril_serial_number_rsp_t *param = NULL;
	
	if (!response || RIL_RESULT_CODE_OK != response->res_code)
	{
        cissys_unlock(g_lockImei);
		return 0;
	}

	param = (ril_serial_number_rsp_t *)response->cmd_param;

    if (param && param->value.imei)
    {
        memcpy(cissys_imei, param->value.imei, strlen(param->value.imei));
    }

    cissys_unlock(g_lockImei);
    return 0;
}

int32_t cissys_cimi_callback(ril_cmd_response_t *response)
{
	ril_imsi_rsp_t *param = NULL;

	if (!response || RIL_RESULT_CODE_OK != response->res_code)
	{
        cissys_unlock(g_lockImsi);
		return 0;
	}

	param = (ril_imsi_rsp_t *)response->cmd_param;

    if (param && param->imsi)
    {
        memcpy(cissys_imsi, param->imsi, strlen(param->imsi));
    }

    cissys_unlock(g_lockImsi);
    return 0;    
}

cis_ret_t cissys_request_endpoint_name(void)
{
    cissys_lock(g_lockImsi, CIS_CONFIG_LOCK_INFINITY);
    ril_request_imsi(RIL_ACTIVE_MODE,
                     cissys_cimi_callback,
                     NULL);

    cissys_lock(g_lockImei, CIS_CONFIG_LOCK_INFINITY);
    ril_request_serial_number(RIL_EXECUTE_MODE,
                              1,
                              cissys_cgsn_callback,
                              NULL);
    return 1;
}

cis_ret_t cissys_init(void *context,const cis_cfg_sys_t* cfg,cissys_callback_t* event_cb)
{
    TaskHandle_t fota_taskhandle;
	st_context_t * ctx=(struct st_cis_context *)context;
    memcpy(&(ctx->g_sysconfig),cfg,sizeof(cis_cfg_sys_t));
	
#if CIS_ENABLE_UPDATE
   ctx->g_fotacallback.onEvent =event_cb->onEvent;
   ctx->g_fotacallback.userData =event_cb->userData;	
   if(TRUE!=ctx->isDM){
    xTaskCreate(testThread,
                "cis_fota",
                2048 / sizeof(portSTACK_TYPE),
                &(ctx->g_fotacallback),
                TASK_PRIORITY_NORMAL,
                &fota_taskhandle);
   }
#endif
    return 1;
}

uint32_t cissys_gettime()
{
    struct timeval tv = {0};
    if (0 != gettimeofday(&tv, NULL))
    {
        return -1;
    }
    return tv.tv_sec * 1000;
}

void    cissys_sleepms(uint32_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void    cissys_logwrite(uint8_t* buffer,uint32_t length)
{
}

void*     cissys_malloc(size_t length)
{
    return pvPortMalloc(length);
}

void    cissys_free(void* buffer)
{
    vPortFree(buffer);
}


void* cissys_memset( void* s, int c, size_t n)
{
	return memset( s, c, n);
}

void* cissys_memcpy(void* dst, const void* src, size_t n)
{
	return memcpy( dst, src, n);
}

void* cissys_memmove(void* dst, const void* src, size_t n)
{
	return memmove( dst, src, n);
}

int cissys_memcmp( const void* s1, const void* s2, size_t n)
{
	return memcmp( s1, s2, n);
}

void    cissys_fault(uint16_t id)
{
    LOGE("fall in cissys_fault.id=%d\r\n",id);
    configASSERT(0);
}


void cissys_assert(bool flag)
{
    configASSERT(flag);
}


uint32_t 	cissys_rand()
{
    uint32_t random_seed;

    hal_trng_init();
    hal_trng_get_generated_random_number(&random_seed);
    hal_trng_deinit();

    return random_seed;
}



uint8_t	cissys_getIMEI(char* buffer,uint32_t maxlen)
{
    const char* str = cissys_imei;
    uint8_t len = 0;

    if (!str[0])
    {
        str = "imeiwb2";
    }
    len = strlen(str);

    if(maxlen < len)return 0;
    memcpy(buffer,str,len);

    return len;
}
uint8_t    cissys_getIMSI(char* buffer,uint32_t maxlen)
{
    const char* str = cissys_imsi;
    uint8_t len = 0;

    if (!str[0])
    {
        str = "imsiwb1";
    }
    len = strlen(str);

    if(maxlen < len)return 0;
    memcpy(buffer,str,len);
   
    return len;
}
void        cissys_lockcreate(void** mutex)
{
    SemaphoreHandle_t mutex_handle;
    mutex_handle = xSemaphoreCreateMutex();
    (*mutex) = mutex_handle;
}

void        cissys_lockdestory(void* mutex)
{
    vSemaphoreDelete((SemaphoreHandle_t)mutex);
}

cis_ret_t   cissys_lock(void* mutex,uint32_t ms)
{
    assert(mutex != NULL);
    xSemaphoreTake((SemaphoreHandle_t)mutex, portMAX_DELAY);
    return CIS_RET_OK;
}

void        cissys_unlock(void* mutex)
{
    xSemaphoreGive((SemaphoreHandle_t)mutex);
}

bool        cissys_save(uint8_t* buffer,uint32_t length)
{
    FILE* f = NULL;
    f = fopen("d:\\cis_serialize.bin","wb");
    if(f != NULL)
    {
        fwrite(buffer,1,length,f);
        fclose(f);
        return true;
    }
    return false;
}

bool        cissys_load(uint8_t* buffer,uint32_t length)
{
    uint32_t readlen;
    FILE* f = fopen("d:\\cis_serialize.bin","rb");
    if(f != NULL)
    {
        while(length)
        {
            readlen = fread(buffer,1,length,f);
            if(readlen == 0)
            {
                break;
            }
            length -=readlen;
        }
        if(length == 0)
        {
            return true;
        }
    }
    return false;
}
char VERSION[20]="2.2.0";
uint32_t    cissys_getFwVersion(uint8_t** version)
{
	int length = strlen(VERSION)+1;
	//pBuffer = (char*)malloc(sizeof(str)+1);
	//if(pBuffer == NULL)return 0;
	//if(length<)
	memcpy(*version,VERSION,length);
	//free(pBuffer);
	//*version = (uint8_t*) str;
	return length;
}
#if CIS_ENABLE_UPDATE

#define DEFAULT_BATTERY_LEVEL (99)
#define DEFAULT_BATTERY_VOLTAGE (3800)
#define DEFAULT_FREE_MEMORY   (554990) 
#define DEFAULT_CELL_ID (95)
#define DEFAULT_RADIO_SIGNAL_STRENGTH (99)


int LENGTH = 0;
int RESULT = 0;
int STATE = 0;


#if 1
int writeCallback = 0;
int validateCallback = 0;
int eraseCallback = 0;
void testThread(void* arg)
{
	cissys_callback_t *cb=(cissys_callback_t *)arg;
	while(1)
	{
		if(writeCallback == 1) //write callback
		{
			cissys_sleepms(2000);
			//FILE* f = NULL;
			//int i = 0;
			//OutputDebugString("cissys_writeFwBytes\n");

			//cissys_sleepms(100);
			//f = fopen("cis_serialize_package.bin","a+b");
			//fseek(f, 0, SEEK_END);
			//if(f != NULL)
			//{
			// fwrite(buffer,1,size,f);
			// fclose(f);
			//return true;
			//}
			writeCallback = 0;
			cb->onEvent(cissys_event_write_success,NULL,cb->userData,NULL);
			//cb->onEvent(cissys_event_write_fail,NULL,cb->userData,NULL);
			
		}
		else if(eraseCallback == 1) //erase callback
		{
			cissys_sleepms(5000);
			eraseCallback = 0;
			LENGTH=0;
			//cb->onEvent(cissys_event_fw_erase_fail,0,cb->userData,NULL);
			cb->onEvent(cissys_event_fw_erase_success,0,cb->userData,NULL);
			
		}
		else if(validateCallback == 1)//validate
		{
			cissys_sleepms(10000);
			validateCallback = 0;
			//cb->onEvent(cissys_event_fw_validate_fail,0,cb->userData,NULL);
			cb->onEvent(cissys_event_fw_validate_success,0,cb->userData,NULL);
			
		}
		/*else if(nThreadNo == 3)
		{
			cissys_sleepms(3000);
			g_syscallback.onEvent(cissys_event_fw_update_success,0,g_syscallback.userData,NULL);
			nThreadNo = -1;
		}*/
	}
}

#endif




uint32_t    cissys_getCellId()
{
	return DEFAULT_CELL_ID;
}

uint32_t    cissys_getRadioSignalStrength()
{
	return DEFAULT_RADIO_SIGNAL_STRENGTH;
}

uint32_t    cissys_getFwBatteryLevel()
{
	return DEFAULT_BATTERY_LEVEL;
}

uint32_t    cissys_getFwBatteryVoltage()
{
	return DEFAULT_BATTERY_VOLTAGE;
}
uint32_t    cissys_getFwAvailableMemory()
{
	return DEFAULT_FREE_MEMORY;
}

int    cissys_getFwSavedBytes()
{
	//FILE * pFile;
	//long size = 0;
	//pFile = fopen ("F:\\cissys_writeFwBytes.bin","rb");
	//if (pFile==NULL)
	//	perror ("Error opening file");
	//else
	//{
	//	fseek (pFile, 0, SEEK_END);
	//	size=ftell (pFile); 
	//	fclose (pFile);
	//}
	//return size;
	return LENGTH;
}

bool    cissys_checkFwValidation(cissys_callback_t *cb)
{
	validateCallback = 1;
	//cissys_sleepms(1000);
	//cb->onEvent(cissys_event_fw_validate_success,0,cb->userData,NULL);
	return true;
}

bool        cissys_eraseFwFlash (cissys_callback_t *cb)
{
	eraseCallback = 1;
	//cissys_sleepms(8000);
	//LENGTH=0;
    //cb->onEvent(cissys_event_fw_erase_success,0,cb->userData,NULL);
	return true;
}

uint32_t   cissys_writeFwBytes(uint32_t size,uint8_t* buffer,cissys_callback_t *cb)
{
	writeCallback = 1;
	//cissys_sleepms(5000);
	LENGTH+=size;
	//cb->onEvent(cissys_event_write_success,NULL,cb->userData,(int *)&size);
    return 1;
}

bool		cissys_updateFirmware (cissys_callback_t *cb)
{
	//nThreadNo = 3;
	cissys_sleepms(10000);
	cb->onEvent(cissys_event_fw_update_success,NULL,cb->userData,NULL);
	memcpy(VERSION,"1800606150",sizeof("1800606150"));
	return true;
}

bool    cissys_readContext(cis_fw_context_t * context)
{
	context->result = RESULT;
	context->savebytes = LENGTH;
	context->state = STATE;
	/*uint32_t readlen;
	char buffer[10];
    FILE* f = fopen("f:\\cis_setFwUpdateResult.bin","rb");
    if(f != NULL)
    {
        fread(buffer,1,sizeof(buffer),f);
		sscanf(buffer,"%d",&(context->result));
    }
	fclose(f);

	f = fopen("f:\\cis_setFwState.bin","rb");
	if(f != NULL)
	{
		fread(buffer,1,sizeof(buffer),f);
		sscanf(buffer,"%d",&(context->state));
	}
	fclose(f);

	f = fopen("f:\\cis_setFwSavedBytes.bin","rb");
    if(f != NULL)
    {
        fread(buffer,1,sizeof(buffer),f);
		sscanf(buffer,"%d",&(context->savebytes));
    }
	fclose(f);*/
	return 1;
}


bool        cissys_setFwState(uint8_t state)
{
	//FILE* f = NULL;
	//int i = 0;
	//char buffer[10];
	//OutputDebugString("cissys_setFwState\n");
	//sprintf(buffer,"%d",state);
	//f = fopen("f:\\cis_setFwState.bin","wb");
	////fseek(f, 0, SEEK_END);
	//if(f != NULL)
	//{
	//	fwrite(buffer,1,1,f);
	//	fclose(f);
	//}
	STATE=state;
	return true;
}
bool        cissys_setFwUpdateResult(uint8_t result)
{
	//FILE* f = NULL;
	//int i = 0;
	//char buffer[10];
	//OutputDebugString("cissys_setFwUpdateResult\n");
	//sprintf(buffer,"%d",result);
	//f = fopen("f:\\cis_setFwUpdateResult.bin","wb");
	////fseek(f, 0, SEEK_END);
	//if(f != NULL)
	//{
	//	fwrite(buffer,1,1,f);
	//	fclose(f);
	//}
	RESULT=result;
		return true;
}
#endif
