#ifndef STD_OBJECT_H_
#define STD_OBJECT_H_

#include "../cis_api.h"
#include "../cis_internals.h"
extern int g_reboot;


typedef enum et_std_objectid
{
    std_object_security	= 0,
#if CIS_ENABLE_BLOCK
	std_object_device = 3,
	std_object_conn = 4,
	std_object_firmware = 5,
    std_object_count = 4,
#else
	std_object_count =1,
#endif
}cis_std_objectid;


/*
 * Standard Object IDs
 */
#define CIS_SECURITY_OBJECT_ID          (0)



/*
 * Resource IDs for the LWM2M Security Object
 */
#define CIS_SECURITY_URI_ID                 0
#define CIS_SECURITY_BOOTSTRAP_ID           1
#define CIS_SECURITY_SECURITY_ID            2
#define CIS_SECURITY_SHORT_SERVER_ID        10
#define CIS_SECURITY_HOLD_OFF_ID            11

#define CIS_SECURITY_MODE_PRE_SHARED_KEY  0
#define CIS_SECURITY_MODE_RAW_PUBLIC_KEY  1
#define CIS_SECURITY_MODE_CERTIFICATE     2
#define CIS_SECURITY_MODE_NONE            3

typedef cis_coapret_t (*std_object_read_callback)(st_context_t * contextP,cis_iid_t instanceId,int * numDataP, st_data_t ** dataArrayP,st_object_t * objectP);
typedef cis_coapret_t (*std_object_exec_callback)(st_context_t * contextP,cis_iid_t instanceId,uint16_t resourceId,uint8_t * buffer,int length,st_object_t * objectP);
typedef cis_coapret_t (*std_object_write_callback)(st_context_t * contextP,cis_iid_t instanceId,int numData,st_data_t * dataArray,st_object_t * objectP);
typedef cis_coapret_t (*std_object_discover_callback)(st_context_t * contextP,cis_iid_t instanceId,int * numDataP, st_data_t ** dataArrayP,st_object_t * objectP);


struct st_std_object_callback_mapping
{
    cis_oid_t           stdObjectId;
    std_object_read_callback onRead;
    std_object_write_callback onWrite;
    std_object_exec_callback onExec;
	std_object_discover_callback onDiscover;
};


/*
 * object.c
 */
cis_coapret_t std_object_read_handler(st_context_t * contextP,cis_iid_t instanceId,int * numDataP, st_data_t ** dataArrayP,st_object_t * objectP);
cis_coapret_t std_object_exec_handler(st_context_t * contextP,cis_iid_t instanceId,uint16_t resourceId,uint8_t * buffer,int length,st_object_t * objectP);
cis_coapret_t std_object_write_handler(st_context_t * contextP,cis_iid_t instanceId,int numData,st_data_t * dataArray,st_object_t * objectP);
cis_coapret_t std_object_discover_handler(st_context_t * contextP,cis_iid_t instanceId,int * numDataP, st_data_t ** dataArrayP,st_object_t * objectP);

cis_coapret_t   std_object_writeInstance(st_context_t * contextP,st_uri_t * uriP, st_data_t * dataP);
cis_list_t*     std_object_get_securitys(st_context_t * contextP);
cis_list_t*     std_object_put_securitys(st_context_t * contextP,cis_list_t* targetP);
void            std_object_remove_securitys(st_context_t * contextP,cis_list_t* targetP);

#if CIS_ENABLE_UPDATE
cis_list_t* std_object_get_device(st_context_t * contextP,cis_iid_t instanceId);

cis_list_t* std_object_put_device(st_context_t * contextP,cis_list_t* targetP);
cis_list_t* std_object_get_firmware(st_context_t * contextP,cis_iid_t instanceId);
cis_list_t* std_object_put_firmware(st_context_t * contextP,cis_list_t* targetP);
void std_object_remove_device(st_context_t * contextP,cis_list_t* targetP);
void std_object_remove_firmware(st_context_t * contextP,cis_list_t* targetP);

cis_list_t* std_object_get_conn(st_context_t * contextP,cis_iid_t instanceId);
cis_list_t* std_object_put_conn(st_context_t * contextP,cis_list_t* targetP);
void std_object_remove_conn(st_context_t * contextP,cis_list_t* targetP);
#endif

/*
 * object_security.c
 */
bool            std_security_create(st_context_t * contextP,int instanceId, const char* serverHost,bool isBootstrap,st_object_t * securityObj);
void            std_security_clean(st_context_t * contextP);
char *          std_security_get_host(st_context_t * contextP,cis_iid_t instanceId);

#if CIS_ENABLE_UPDATE
/*
 * object_device.c
 */
bool            std_device_create(st_context_t * contextP,int instanceId,st_object_t * deviceObj);
void            std_device_clean(st_context_t * contextP);

/*
 * object_firmware.c
 */
#define PRV_FIRMWARE_URI  "COAP://NULL"
#define PRV_FIRMWARE_URI_LEN 32
typedef struct _firmware_data_
{
	struct _firmware_data_ * next;        // matches st_list_t::next
	cis_listid_t                 instanceId;  // matches st_list_t::id
	bool supported;
	cis_fw_context_t * fw_info;
	uint8_t delivery;
	char download_uri[PRV_FIRMWARE_URI_LEN];
} firmware_data_t;

bool            std_firmware_create(st_context_t * contextP,int instanceId,st_object_t * firmwareObj);
void            std_firmware_clean(st_context_t * contextP);

/*
 * object_conn_moniter.c
 */
bool            std_conn_moniter_create(st_context_t * contextP,int instanceId,st_object_t * connObj);
void            std_conn_moniter_clean(st_context_t * contextP);
#endif
#endif /* STD_OBJECT_H_ */
