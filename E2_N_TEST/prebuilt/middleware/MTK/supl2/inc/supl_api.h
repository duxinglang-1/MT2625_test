#ifndef __SUPL_API_H__
#define __SUPL_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define SUPL_MAX_SERVER_ADDR_LEN 128
#define SUPL_MAX_CELL_SIZE 10

typedef enum {
    SUPL_RESULT_SUCCESS = 0,
    SUPL_RESULT_ERROR = -1
} supl_result_enum;

typedef enum{
    SUPL_RESULT_TYPE_OTDOA,
    SUPL_RESULT_TYPE_ECID
}supl_result_type_enum;

typedef struct{
    double longitude;
    double latitude;
}supl_otdoa_result_t;

typedef struct{
    double longitude;
    double latitude;
}supl_ecid_result_t;


typedef struct{
    supl_result_enum result;
    supl_result_type_enum data_type;
    void *data;
}supl_result_t;

typedef void (*supl_result_callback)(supl_result_t* result);

typedef enum{
    SUPL_LOC_TYPE_OTDOA, // supl_otdoa_result_t
    SUPL_LOC_TYPE_ECID,
    //SUPL_LOC_TYPE_AGPS //Not Support
}supl_location_type_enum;

typedef struct {
    char server_addr[SUPL_MAX_SERVER_ADDR_LEN];
    int32_t server_port;
    bool use_tls;
}supl_server_config_t;

typedef enum {
    SUPL_CELL_GSM = 0,
    SUPL_CELL_WCDMA,
    SUPL_CELL_LTE,
    SUPL_CELL_C2K,
} supl_cell_enum;

typedef struct {
    short mcc; //999
    short mnc; //999
    unsigned short lac; //65535
    unsigned short cid; //65535
} supl_gsm_cell;

typedef struct {
    short mcc; //999
    short mnc; //999
    unsigned short lac; //65535
    int uc; //268435455
} supl_wcdma_cell;

typedef struct {
    short mcc; //999
    short mnc; //999
    unsigned short tac; //65535
    int ci; //268435455
    short pci; //503
} supl_lte_cell;

typedef struct {
    unsigned short sid; //32767
    unsigned short nid; //65535
    unsigned short bid; //65535
} supl_cdma_cell;

typedef struct {
    supl_cell_enum cell_type;
    union {
        supl_gsm_cell gsm_cell;
        supl_wcdma_cell wcdma_cell;
        supl_lte_cell lte_cell;
        supl_cdma_cell cdma_cell;
    } cell;
} supl_cell_t;

typedef struct{
    int num;
    supl_cell_t cells[SUPL_MAX_CELL_SIZE];
}supl_cell_info_t;

typedef struct {
    bool has_config;
    supl_server_config_t server_config;
    bool has_cell_info;
    supl_cell_info_t cell_info;
    supl_location_type_enum location_type; 
    const char *cert_file;
    const char *peer_name;
} supl_config_t;

int supl_save_config(supl_config_t *config);

supl_config_t *supl_get_config();

//-1 means failure
int supl_start();

//-1 means failure
int supl_abort();

//-1 means failure
int supl_reset();

int supl_set_result_callback(supl_result_callback* result);

#ifdef __cplusplus
}
#endif

#endif
