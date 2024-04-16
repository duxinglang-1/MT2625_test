#include <stdio.h>
#include <string.h>
#include <cis_def.h>
#include <cis_api.h>
#include "FreeRTOS.h"
#include "task.h"

extern int cis_sample_entry(const uint8_t * config_bin,uint32_t config_size);
extern int dm_sample_dm_entry(const uint8_t * config_bin,uint32_t config_size);
void cis_sample_thread(void* arg);
void dm_sample_thread(void* arg);

#if 1
//183.230.40.39
static const uint8_t config_hex[] = {0x13,0x00,0x34,
    0xf1,0x00,0x03,
    0xf2,0x00,0x22,0x05,0x00/*mtu*/,0x11/*Link&bind type*/,0x80/*BS ENABLED*/,0x00,0x00/*vpn*/,0x00,0x00/*username*/,0x00,0x00/*password*/,
    0x00,0x0d/*host length*/,0x31,0x38,0x33,0x2e,0x32,0x33,0x30,0x2e,0x34,0x30,0x2e,0x33,0x39,0x00,0x04,0x4e,0x55,0x4c,0x4c,
    0xf3,0x00,0x0d,0xea,0x04,0x00,0x00,0x04,0x4e,0x55,0x4c,0x4c};
#endif


#if 0
//183.230.40.40
static const uint8_t config_hex[] = {0x13,0x00,0x34,
    0xf1,0x00,0x03,
    0xf2,0x00,0x22,0x05,0x00/*mtu*/,0x11/*Link&bind type*/,0x00/*BS DISENABLED*/,0x00,0x00/*vpn*/,0x00,0x00/*username*/,0x00,0x00/*password*/,
    0x00,0x0d/*host length*/,0x31,0x38,0x33,0x2e,0x32,0x33,0x30,0x2e,0x34,0x30,0x2e,0x34,0x30,0x00,0x04,0x4e,0x55,0x4c,0x4c,
    0xf3,0x00,0x0d,0xea,0x04,0x00,0x00,0x04,0x4e,0x55,0x4c,0x4c};
#endif

void cis_sample_thread(void* arg)
{
    
    if(1){
	  cis_sample_entry(config_hex,sizeof(config_hex));
   
    }
}
#if CIS_ENABLE_DM
static const uint8_t dm_config_hex[] = {0x13,0x00,0x32,
    0xf1,0x00,0x03,
    0xf2,0x00,0x20,0x05,0x00/*mtu*/,0x11/*Link&bind type*/,0x00/*BS ENABLED*/,0x00,0x00/*vpn*/,0x00,0x00/*username*/,0x00,0x00/*password*/,
    0x00,0x0b/*host length*/,0x31,0x31,0x37,0x2e,0x31,0x36,0x31,0x2e,0x32,0x2e,0x37,0x00,0x04,0x4e,0x55,0x4c,0x4c,
    0xf3,0x00,0x0d,0xea,0x04,0x00,0x00,0x04,0x4e,0x55,0x4c,0x4c};

void dm_sample_thread(void* arg)
{
    
    if(1){
	 dm_sample_dm_entry(dm_config_hex,sizeof(dm_config_hex));
       
    }
}
#endif

void cis_sample_init(void)
{
    cis_sample_thread(NULL);
}

void dm_sample_init(void)
{
    dm_sample_thread(NULL);
}